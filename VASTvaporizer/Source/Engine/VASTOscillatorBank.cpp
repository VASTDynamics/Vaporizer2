/*
VAST Dynamics Audio Software (TM)
*/

#include "VASTOscillatorBank.h"
#include "../Plugin/VASTAudioProcessor.h"
#include "../Plugin/VASTUtils/VASTUndoableWavetableAction.h"

CVASTOscillatorBank::CVASTOscillatorBank(CVASTSettings* set, VASTAudioProcessor* processor, int bankno) {
	//mReadWriteLock = new ReadWriteLock(); //one lock per instance of class
	m_Set = set;
	myProcessor = processor;
	m_bankno = bankno;
	m_bWtSoftChangedFdv = true; //CHECK
	m_bWtSoftChangedOsc = true; //CHECK
	m_bWtSoftChangedOscEditor = true; //CHECK
	m_bWtSoftChangedPos = true; //CHECK
	m_wavetable_undo_buffered = nullptr;
	m_wavetable_undo_buffered_before = nullptr;
	lastWTchangeCounter = -1;
	m_soloMode = false;
	m_bSoftFadeSinceLastUndoCheck = true;

	startTimer(1000); //Timer for WT Copy / Undo
	init();
}

CVASTOscillatorBank::~CVASTOscillatorBank() {
	//m_wavetable = nullptr;
	std::atomic_store(&m_wavetable, std::shared_ptr<CVASTWaveTable>{}); //thread safe nullptr https://stackoverflow.com/questions/30117975/is-thread-safe-to-assign-a-shared-ptr-nullptr
}

void CVASTOscillatorBank::init() {
	std::shared_ptr<CVASTWaveTable> wavetable(new CVASTWaveTable(*m_Set));
	wavetable->addPosition();
	if (std::atomic_load(&m_wavetable) == nullptr) //first init all //atomic check for nullptr https://stackoverflow.com/questions/30117975/is-thread-safe-to-assign-a-shared-ptr-nullptr
		setWavetable(wavetable);
	else 
		setWavetableSoftFade(wavetable);
}

void CVASTOscillatorBank::setWavetable(std::shared_ptr<CVASTWaveTable> wavetable) { //attention - causes clicks in audio process!
	//only call this when audio process is not running!
	//vassert(myProcessor->m_pVASTXperience.nonThreadsafeIsBlockedProcessingInfo());
	const ScopedLock sl(mSharedPtrSoftFadeLock);

	//m_wavetable = wavetable; 
	std::atomic_store(&m_wavetable, wavetable); //assigning a new instance to a shared pointer make thread safe: https://www.modernescpp.com/index.php/atomic-smart-pointers
	
	//m_wavetable_soft_fade = nullptr;
	std::atomic_store(&m_wavetable_soft_fade, std::shared_ptr<CVASTWaveTable>{}); //thread safe nullptr https://stackoverflow.com/questions/30117975/is-thread-safe-to-assign-a-shared-ptr-nullptr
	//m_wavetable_soft_fade_next = nullptr;
	std::atomic_store(&m_wavetable_soft_fade_next, std::shared_ptr<CVASTWaveTable>{}); //assigning a new instance to a shared pointer make thread safe: https://www.modernescpp.com/index.php/atomic-smart-pointers
    m_bWavetableSoftfadeStillRendered.store(false);
	m_iSingleNoteSoftFadeID.store(-1);
	m_bWavetableSoftfadePickedUp.store(false);

	m_bWtSoftChangedFdv.store(true);
	m_bWtSoftChangedOsc.store(true);
	m_bWtSoftChangedOscEditor.store(true);
	m_bWtSoftChangedPos.store(true);

	//m_wavetable_undo_buffered = nullptr;
	std::atomic_store(&m_wavetable_undo_buffered, std::shared_ptr<CVASTWaveTable>{}); //assigning a new instance to a shared pointer make thread safe: https://www.modernescpp.com/index.php/atomic-smart-pointers
	//m_wavetable_undo_buffered_before = nullptr;
	std::atomic_store(&m_wavetable_undo_buffered_before, std::shared_ptr<CVASTWaveTable>{}); //assigning a new instance to a shared pointer make thread safe: https://www.modernescpp.com/index.php/atomic-smart-pointers
	m_bSoftFadeSinceLastUndoCheck = true;
}

void CVASTOscillatorBank::setWavetableSoftFade(std::shared_ptr<CVASTWaveTable> wavetable) {
	//first block all other processing //https://timur.audio/using-locks-in-real-time-audio-processing-safely
	VDBG("setWavetableSoftFade - next is ID:" << wavetable->getID());
	const ScopedLock sl(mSharedPtrSoftFadeLock);
	
	//m_wavetable_soft_fade_next = wavetable;
	std::atomic_store(&m_wavetable_soft_fade_next, wavetable); //assigning a new instance to a shared pointer make thread safe: https://www.modernescpp.com/index.php/atomic-smart-pointers

	m_bWtSoftChangedFdv = true; //CHECK
	m_bWtSoftChangedOsc = true; //CHECK
	m_bWtSoftChangedOscEditor = true; //CHECK
	m_bWtSoftChangedPos = true; //CHECK

	m_bSoftFadeSinceLastUndoCheck = true;		
}

void CVASTOscillatorBank::beginSoftFade() { //to be called at start of buffer processing
	//VDBG("beginSoftFade()");

	if (m_bWavetableSoftfadeStillRendered) {
		if (m_bWavetableSoftfadePickedUp && (!isInSingleNoteSoftFadeCycle())) {
			m_iSingleNoteSoftFadeID.store(-1);
            m_bWavetableSoftfadeStillRendered.store(false);
		}
	}
	else clearSingleNoteSoftFadeCycle();

	jassert(m_numEditingSoftFadeNext.load() >= 0);
	if (m_numEditingSoftFadeNext.load() < 0)
		m_numEditingSoftFadeNext.store(0); //Safety CHECK

	if ((m_wavetable_soft_fade_next.get() != nullptr) || (m_wavetable_soft_fade.get() != nullptr)) {  //avoid atomic lock
		if ((std::atomic_load(&m_wavetable_soft_fade_next) != nullptr) || (std::atomic_load(&m_wavetable_soft_fade) != nullptr)) { //atomic check for nullptr https://stackoverflow.com/questions/30117975/is-thread-safe-to-assign-a-shared-ptr-nullptr
			if (!mSharedPtrSoftFadeLock.tryEnter()) { //this is not blocking the audio thread - it is not waiting for anything
				VDBG("beginSoftFade LOCKED by setWavetableSoftFade!!!!!!");
				return;
			}

			if ((m_numEditingSoftFadeNext == 0) && (!m_bWavetableSoftfadeStillRendered.load())) {
                /* ------------------------------------------------------------------------------------------------ */
                /*                        Begin Softfade Cycle                                                      */
                /* ------------------------------------------------------------------------------------------------ */
#ifdef _DEBUG
				if (std::atomic_load(&m_wavetable_soft_fade_next) == nullptr) //atomic check for nullptr https://stackoverflow.com/questions/30117975/is-thread-safe-to-assign-a-shared-ptr-nullptr
					VDBG(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>beginSoftFade to NULLPTR");
				else
                    VDBG(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>beginSoftFade NOW editing threads#: " << m_numEditingSoftFadeNext <<  " will replace WT " << m_wavetable->getID() << " with next WT " << m_wavetable_soft_fade_next->getID());
#endif

				std::atomic_store(&m_wavetable_soft_fade, m_wavetable_soft_fade_next); //assigning a new instance to a shared pointer make thread safe: https://www.modernescpp.com/index.php/atomic-smart-pointers

				std::atomic_store(&m_wavetable_soft_fade_next, std::shared_ptr<CVASTWaveTable>{}); //thread safe nullptr https://stackoverflow.com/questions/30117975/is-thread-safe-to-assign-a-shared-ptr-nullptr

                m_bWavetableSoftfadeStillRendered.store(true);
				m_bWavetableSoftfadePickedUp.store(false);

				if (m_bIsRecording) {
					//VDBG("beginSoftFade is recording - adding position from WT " << m_wavetable->getID() << " to WT " << m_wavetable_soft_fade->getID());
					if (m_wavetable_soft_fade != nullptr) {
						int numPos = m_wavetable_soft_fade->getNumPositions();
						if (numPos < C_MAX_NUM_POSITIONS) {
							m_wavetable_soft_fade->copyPositionToOtherWavetable(m_iRecordingPos, numPos, m_wavetable_soft_fade.get()); //locks wt
						}
					}
				}
                /* ------------------------------------------------------------------------------------------------ */
                /*                        Begin Softfade Cycle                                                      */
                /* ------------------------------------------------------------------------------------------------ */
			}

#ifdef _DEBUG
			else {
                if (m_numEditingSoftFadeNext != 0) {
                    VDBG("beginSoftFade not done due to editing threads#: " << m_numEditingSoftFadeNext);
                }
				else
					VDBG("beginSoftFade not done due to SoftFadeStillNeeded ID: " << m_iSingleNoteSoftFadeID.load());
			}
#endif
			mSharedPtrSoftFadeLock.exit();
		}
	} //else do nothing
}

bool CVASTOscillatorBank::endSoftFade() { //to be called at start of buffer processing
	bool lResult = true;
	
	if (m_wavetable_soft_fade.get() != nullptr) { //avoid atomic lock
		if ((std::atomic_load(&m_wavetable_soft_fade) != nullptr)) { //atomic check for nullptr https://stackoverflow.com/questions/30117975/is-thread-safe-to-assign-a-shared-ptr-nullptr
			//only need to lock if there is something
			if (!mSharedPtrSoftFadeLock.tryEnter()) { //this is not blocking the audio thread - it is not waiting for anything
				VDBG("endSoftFade LOCKED by setWavetableSoftFade!!!!!!");
				return false;
			}

			if (m_bWavetableSoftfadeStillRendered.load()) {
				if (!myProcessor->m_pVASTXperience.m_Poly.voicesMSEGStillActive()) {
					m_bWavetableSoftfadePickedUp = true;
                    VDBG("endSoftFade m_bWavetableSoftfadePickedUp since no voicesMSEGStillActive!");
				}

				if (m_bWavetableSoftfadePickedUp && (!isInSingleNoteSoftFadeCycle())) {
                    m_bWavetableSoftfadeStillRendered.store(false);
					m_iSingleNoteSoftFadeID.store(-1);
					VDBG("endSoftFade Resetting m_bWavetableSoftfadeStillRendered!");
				}
			}
			else clearSingleNoteSoftFadeCycle();

			if (!m_bWavetableSoftfadeStillRendered.load()) {
                /* ------------------------------------------------------------------------------------------------ */
                /*                        Softfade Merge                                                            */
                /* ------------------------------------------------------------------------------------------------ */
				VDBG(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>endSoftFade merged - editing threads#: " << m_numEditingSoftFadeNext << " replacing WT " << m_wavetable->getID() << " with new WT " << m_wavetable_soft_fade->getID());

				std::atomic_store(&m_wavetable, m_wavetable_soft_fade); //assigning a new instance to a shared pointer make thread safe: https://www.modernescpp.com/index.php/atomic-smart-pointers

				std::atomic_store(&m_wavetable_soft_fade, std::shared_ptr<CVASTWaveTable>{}); //thread safe nullptr https://stackoverflow.com/questions/30117975/is-thread-safe-to-assign-a-shared-ptr-nullptr

				m_bWtSoftChangedFdv.store(true);
				m_bWtSoftChangedOsc.store(true);
				m_bWtSoftChangedOscEditor.store(true);
				m_bWtSoftChangedPos.store(true);
				m_bWtSoftChangedStructure.store(true);
                m_bWavetableSoftfadeStillRendered.store(false);
				m_iSingleNoteSoftFadeID.store(-1);
				m_bWavetableSoftfadePickedUp.store(false);

				lResult = true;
                /* ------------------------------------------------------------------------------------------------ */
                /*                        Softfade Merge                                                            */
                /* ------------------------------------------------------------------------------------------------ */
			}
			else
				VDBG("endSoftFade not done - m_wavetable_soft_fade still used ID: " << m_wavetable_soft_fade->getID());
			mSharedPtrSoftFadeLock.exit();
		}
	}
	return lResult;
}

std::shared_ptr<CVASTWaveTable> CVASTOscillatorBank::getNewSharedWavetable() const { //by value: share ownership
	//return m_wavetable;	//can be changed in meantime, need atomic_store here??
	std::shared_ptr<CVASTWaveTable> localtr;
	std::atomic_store(&localtr, m_wavetable); //assigning a new instance to a shared pointer make thread safe: https://www.modernescpp.com/index.php/atomic-smart-pointers
	return localtr;
}

const std::shared_ptr<CVASTWaveTable>& CVASTOscillatorBank::getWavetablePointer() const {
	return m_wavetable;
}

std::shared_ptr<CVASTWaveTable> CVASTOscillatorBank::getNewSharedSoftFadeWavetable() const {
//	return m_wavetable_soft_fade; //can be changed in meantime, need atomic_store here??
	std::shared_ptr<CVASTWaveTable> localtr;
	std::atomic_store(&localtr, m_wavetable_soft_fade); //assigning a new instance to a shared pointer make thread safe: https://www.modernescpp.com/index.php/atomic-smart-pointers
	return localtr;
}

const std::shared_ptr<CVASTWaveTable>& CVASTOscillatorBank::getSoftFadeWavetablePointer() const {
	return m_wavetable_soft_fade;
}

std::shared_ptr<CVASTWaveTable> CVASTOscillatorBank::getNewSharedSoftFadeWavetableNext() const {
	//return m_wavetable_soft_fade_next; //can be changed in meantime, need atomic_store here??
	std::shared_ptr<CVASTWaveTable> localtr;
	std::atomic_store(&localtr, m_wavetable_soft_fade_next); //assigning a new instance to a shared pointer make thread safe: https://www.modernescpp.com/index.php/atomic-smart-pointers
	return localtr;
}

const std::shared_ptr<CVASTWaveTable>& CVASTOscillatorBank::getSoftFadeWavetablePointerNext() const {
	return m_wavetable_soft_fade_next;
}

void CVASTOscillatorBank::recalcWavetable() {
	if (m_wavetable != nullptr)
		m_wavetable->markAllWTFreqsDirty();
	if (m_wavetable_soft_fade != nullptr)
		m_wavetable_soft_fade->markAllWTFreqsDirty();
	if (m_wavetable_soft_fade_next != nullptr)
		m_wavetable_soft_fade_next->markAllWTFreqsDirty();
}

void CVASTOscillatorBank::prepareForPlay(int expectedSamplesPerBlock) {
	m_wavetable->prepareForPlay(expectedSamplesPerBlock);
}

int CVASTOscillatorBank::getBankno() const { return m_bankno; }

void CVASTOscillatorBank::setChangedFlagOsc() {
	m_bWtSoftChangedOsc = true; //CHECK
}

void CVASTOscillatorBank::setChangedFlag() {
	m_bWtSoftChangedFdv = true; //CHECK
	m_bWtSoftChangedOsc = true; //CHECK
	m_bWtSoftChangedOscEditor = true; //CHECK
	m_bWtSoftChangedPos = true; //CHECK

}

bool CVASTOscillatorBank::isChanged() const {
	return
		(m_bWtSoftChangedFdv || m_bWtSoftChangedOsc || m_bWtSoftChangedOscEditor || m_bWtSoftChangedPos);
}

bool CVASTOscillatorBank::getAndClearSoftChangedFlagStructure() {
	bool l_changed = m_bWtSoftChangedStructure;
	m_bWtSoftChangedStructure = false;
	return l_changed;
}

bool CVASTOscillatorBank::getAndClearSoftChangedFlagFdv() {
	bool l_changed = m_bWtSoftChangedFdv;
	m_bWtSoftChangedFdv = false;
	return l_changed;
}

bool CVASTOscillatorBank::getAndClearSoftChangedFlagOsc() {
	bool l_changed = m_bWtSoftChangedOsc;
	m_bWtSoftChangedOsc = false;
	return l_changed;
}

bool CVASTOscillatorBank::getAndClearSoftChangedFlagOscEditor() {
	bool l_changed = m_bWtSoftChangedOscEditor;
	m_bWtSoftChangedOscEditor = false;
	return l_changed;
}

bool CVASTOscillatorBank::getAndClearSoftChangedFlagPos() {
	bool l_changed = m_bWtSoftChangedPos;
	m_bWtSoftChangedPos = false;
	return l_changed;
}

void CVASTOscillatorBank::setSoloMode(bool solo) {
	m_soloMode = solo;
	m_bWtSoftChangedOsc = true;
}

bool CVASTOscillatorBank::getSoloMode() const {
	return m_soloMode;
}

void CVASTOscillatorBank::timerCallback() { //for WT copy / undo
	if (std::atomic_load(&m_wavetable) == nullptr) //atomic check for nullptr https://stackoverflow.com/questions/30117975/is-thread-safe-to-assign-a-shared-ptr-nullptr
		return;
	if (m_bSoftFadeSinceLastUndoCheck) {		
		if (std::atomic_load(&m_wavetable_undo_buffered) == nullptr) { //atomic check for nullptr https://stackoverflow.com/questions/30117975/is-thread-safe-to-assign-a-shared-ptr-nullptr

			//m_wavetable_undo_buffered_before = std::shared_ptr<CVASTWaveTable>(m_wavetable); 
			std::atomic_store(&m_wavetable_undo_buffered_before, std::shared_ptr<CVASTWaveTable>(m_wavetable)); //copy constructor // assigning a new instance to a shared pointer make thread safe: https://www.modernescpp.com/index.php/atomic-smart-pointers
			//m_wavetable_undo_buffered = std::shared_ptr<CVASTWaveTable>(m_wavetable); //copy constructor		
			std::atomic_store(&m_wavetable_undo_buffered, std::shared_ptr<CVASTWaveTable>(m_wavetable)); //copy constructor // assigning a new instance to a shared pointer make thread safe: https://www.modernescpp.com/index.php/atomic-smart-pointers
		}
		else {
			//enter in undo buffer here
			myProcessor->m_undoManager.perform(new VASTUndoableWavetableAction(this));

			//m_wavetable_undo_buffered_before = m_wavetable_undo_buffered; 
			std::atomic_store(&m_wavetable_undo_buffered_before, m_wavetable_undo_buffered); //assigning a new instance to a shared pointer make thread safe: https://www.modernescpp.com/index.php/atomic-smart-pointers
			//m_wavetable_undo_buffered = std::shared_ptr<CVASTWaveTable>(m_wavetable); //copy constructor		
			std::atomic_store(&m_wavetable_undo_buffered, std::shared_ptr<CVASTWaveTable>(m_wavetable)); //copy constructor // assigning a new instance to a shared pointer make thread safe: https://www.modernescpp.com/index.php/atomic-smart-pointers
		}
	}

	m_bSoftFadeSinceLastUndoCheck = false;
}

bool CVASTOscillatorBank::needsUndo() const {
	return ((m_wavetable_undo_buffered_before != nullptr) && (m_wavetable_undo_buffered_before->getChangeCounter() != m_wavetable->getChangeCounter()));
}

void CVASTOscillatorBank::undoLastWTChange() {
	if (needsUndo()) {
		setWavetableSoftFade(std::shared_ptr<CVASTWaveTable>(m_wavetable_undo_buffered_before)); //copy constructor
		//m_wavetable_undo_buffered_before = nullptr;
		std::atomic_store(&m_wavetable_undo_buffered_before, std::shared_ptr<CVASTWaveTable>{}); //thread safe nullptr https://stackoverflow.com/questions/30117975/is-thread-safe-to-assign-a-shared-ptr-nullptr
		//m_wavetable_undo_buffered = nullptr;
		std::atomic_store(&m_wavetable_undo_buffered, std::shared_ptr<CVASTWaveTable>{}); //thread safe nullptr https://stackoverflow.com/questions/30117975/is-thread-safe-to-assign-a-shared-ptr-nullptr

		m_bSoftFadeSinceLastUndoCheck = true;
	}
}

void CVASTOscillatorBank::addSingleNoteSoftFadeCycle(int voiceNo) {
	m_iSingleNoteSoftFadeCycle[voiceNo].store(true);
	//VDBG("addSingleNoteSoftFadeCycle SingleNoteSoftFadeCycle now " + String((isInSingleNoteSoftFadeCycle())));
}

void CVASTOscillatorBank::removeSingleNoteSoftFadeCycle(int voiceNo) {
	m_iSingleNoteSoftFadeCycle[voiceNo].store(false);
	//VDBG("removeSingleNoteSoftFadeCycle SingleNoteSoftFadeCycle now " + String((isInSingleNoteSoftFadeCycle())));
}

bool CVASTOscillatorBank::isInSingleNoteSoftFadeCycle() const {
	int sum = std::accumulate(m_iSingleNoteSoftFadeCycle, m_iSingleNoteSoftFadeCycle + C_MAX_POLY, 0);
	return sum > 0;
}

void CVASTOscillatorBank::clearSingleNoteSoftFadeCycle() {
	std::fill(m_iSingleNoteSoftFadeCycle, m_iSingleNoteSoftFadeCycle + C_MAX_POLY, false);
}

void CVASTOscillatorBank::startRecording(int wtPos) {
	m_bIsRecording = true;
	m_iRecordingPos = wtPos;
}

void CVASTOscillatorBank::stopRecording() {
	m_bIsRecording = false;

	std::shared_ptr<CVASTWaveTable> l_wavetable = getSoftOrCopyWavetable();
	if ((m_iRecordingPos >= 0) && (l_wavetable->getNumPositions() > (m_iRecordingPos + 1))) {
		l_wavetable->deletePosition(m_iRecordingPos);
		setWavetableSoftFade(l_wavetable);
	}

	m_iRecordingPos = -1;
}

std::shared_ptr<CVASTWaveTable> CVASTOscillatorBank::getSoftOrCopyWavetable(bool getCopy, bool copyAlsoFreqs) {
	//make thread safe!! https://stackoverflow.com/questions/7688107/is-copy-thread-safe

	std::shared_ptr<CVASTWaveTable> wtshared;
	if ((!getCopy) && (getSoftFadeWavetablePointerNext() != nullptr)) {
		std::atomic_store(&wtshared, getNewSharedSoftFadeWavetableNext()); //check
	}
	else {
		vassert(m_wavetable->m_Set == m_Set);
		std::atomic_store(&wtshared, getNewSharedWavetable()->getClonedInstance(false, copyAlsoFreqs)); //check
		setWavetableSoftFade(wtshared);
	}
	vassert(wtshared != nullptr);
	if (wtshared == nullptr) {
		//safety hack
		std::shared_ptr<CVASTWaveTable> wavetable = std::make_shared<CVASTWaveTable>(*m_Set);
		std::atomic_store(&wtshared, wavetable); //create fresh one
	}

	return wtshared;
}

//https://stackoverflow.com/questions/11666610/how-to-give-priority-to-privileged-thread-in-mutex-locking

void CVASTOscillatorBank::addSoftFadeEditor() {
	m_numEditingSoftFadeNext+=1;
}

void CVASTOscillatorBank::removeSoftFadeEditor() {
	m_numEditingSoftFadeNext-=1;
}
