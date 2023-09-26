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
	vassert(myProcessor->m_pVASTXperience.nonThreadsafeIsBlockedProcessingInfo());
	const ScopedLock sl(mSharedPtrSoftFadeLock);

	//m_wavetable = wavetable; 
	std::atomic_store(&m_wavetable, wavetable); //assigning a new instance to a shared pointer make thread safe: https://www.modernescpp.com/index.php/atomic-smart-pointers
	
	//m_wavetable_soft_fade = nullptr;
	std::atomic_store(&m_wavetable_soft_fade, std::shared_ptr<CVASTWaveTable>{}); //thread safe nullptr https://stackoverflow.com/questions/30117975/is-thread-safe-to-assign-a-shared-ptr-nullptr
	//m_wavetable_soft_fade_next = nullptr;
	std::atomic_store(&m_wavetable_soft_fade_next, std::shared_ptr<CVASTWaveTable>{}); //assigning a new instance to a shared pointer make thread safe: https://www.modernescpp.com/index.php/atomic-smart-pointers
	m_bWavetableSoftfadeStillNeeded = false;
	m_iSingleNoteSoftFadeID = -1;
	m_bWavetableSoftfadePickedUp = false;

	m_bWtSoftChangedFdv = true; //CHECK
	m_bWtSoftChangedOsc = true; //CHECK
	m_bWtSoftChangedOscEditor = true; //CHECK
	m_bWtSoftChangedPos = true; //CHECK

	//m_wavetable_undo_buffered = nullptr;
	std::atomic_store(&m_wavetable_undo_buffered, std::shared_ptr<CVASTWaveTable>{}); //assigning a new instance to a shared pointer make thread safe: https://www.modernescpp.com/index.php/atomic-smart-pointers
	//m_wavetable_undo_buffered_before = nullptr;
	std::atomic_store(&m_wavetable_undo_buffered_before, std::shared_ptr<CVASTWaveTable>{}); //assigning a new instance to a shared pointer make thread safe: https://www.modernescpp.com/index.php/atomic-smart-pointers
	m_bSoftFadeSinceLastUndoCheck = true;
}

void CVASTOscillatorBank::setWavetableSoftFade(std::shared_ptr<CVASTWaveTable> wavetable) {
	//first block all other processing //https://timur.audio/using-locks-in-real-time-audio-processing-safely
	DBG("setWavetableSoftFade ID:" + String(wavetable->getID()));
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
	//DBG("beginSoftFade()");

	if (m_bWavetableSoftfadeStillNeeded) {
		if (m_bWavetableSoftfadePickedUp && (isInSingleNoteSoftFadeCycle() <= 0)) {
			m_iSingleNoteSoftFadeID = -1;
			m_bWavetableSoftfadeStillNeeded.store(false);
		}
	}
	else clearSingleNoteSoftFadeCycle();

	jassert(m_numEditingSoftFadeNext >= 0);
	if (m_numEditingSoftFadeNext < 0) 
		m_numEditingSoftFadeNext = 0; //Safety CHECK

	if ((m_wavetable_soft_fade_next.get() != nullptr) || (m_wavetable_soft_fade.get() != nullptr)) {  //avoid atomic lock
		if ((std::atomic_load(&m_wavetable_soft_fade_next) != nullptr) || (std::atomic_load(&m_wavetable_soft_fade) != nullptr)) { //atomic check for nullptr https://stackoverflow.com/questions/30117975/is-thread-safe-to-assign-a-shared-ptr-nullptr
			if (!mSharedPtrSoftFadeLock.tryEnter()) { //this is not blocking the audio thread - it is not waiting for anything
				DBG("beginSoftFade LOCKED by setWavetableSoftFade!!!!!!");
				return;
			}

			if ((m_numEditingSoftFadeNext == 0) && (!m_bWavetableSoftfadeStillNeeded)) {

#ifdef _DEBUG
				if (std::atomic_load(&m_wavetable_soft_fade_next) == nullptr) //atomic check for nullptr https://stackoverflow.com/questions/30117975/is-thread-safe-to-assign-a-shared-ptr-nullptr
					DBG("beginSoftFade to NULLPTR");
				else
					DBG("beginSoftFade NOW editing threads#: " + String(m_numEditingSoftFadeNext));
#endif
				//m_wavetable_soft_fade = m_wavetable_soft_fade_next;
				std::atomic_store(&m_wavetable_soft_fade, m_wavetable_soft_fade_next); //assigning a new instance to a shared pointer make thread safe: https://www.modernescpp.com/index.php/atomic-smart-pointers
				//m_wavetable_soft_fade_next = nullptr;
				std::atomic_store(&m_wavetable_soft_fade_next, std::shared_ptr<CVASTWaveTable>{}); //thread safe nullptr https://stackoverflow.com/questions/30117975/is-thread-safe-to-assign-a-shared-ptr-nullptr

				m_bWavetableSoftfadeStillNeeded = true;
				m_bWavetableSoftfadePickedUp = false;

				if (m_bIsRecording) {
					DBG("beginSoftFade is recording - adding position from WT " + String(m_wavetable->getID()) + " to WT " + String(m_wavetable_soft_fade->getID()));
					int numPos = m_wavetable_soft_fade->getNumPositions();
					if (numPos < C_MAX_NUM_POSITIONS) {
						m_wavetable_soft_fade->copyPositionToOtherWavetable(m_iRecordingPos, numPos, m_wavetable_soft_fade.get()); //locks wt
					}
				}
			}

#ifdef _DEBUG
			else {
				if (m_numEditingSoftFadeNext != 0)
					DBG("beginSoftFade not done due to editing threads#: " + String(m_numEditingSoftFadeNext));
				else
					DBG("beginSoftFade not done due to SoftFadeStillNeeded ID: " + String(m_iSingleNoteSoftFadeID));
			}
#endif
			mSharedPtrSoftFadeLock.exit();
		}
	} //else do nothing
}

bool CVASTOscillatorBank::endSoftFade() { //to be called at start of buffer processing
	bool lResult = false;
	
	if (m_wavetable_soft_fade.get() != nullptr) { //avoid atomic lock
		if ((std::atomic_load(&m_wavetable_soft_fade) != nullptr)) { //atomic check for nullptr https://stackoverflow.com/questions/30117975/is-thread-safe-to-assign-a-shared-ptr-nullptr
			//only need to lock if there is something
			if (!mSharedPtrSoftFadeLock.tryEnter()) { //this is not blocking the audio thread - it is not waiting for anything
				DBG("endSoftFade LOCKED by setWavetableSoftFade!!!!!!");
				return false;
			}

			if (m_bWavetableSoftfadeStillNeeded) {
				if (!myProcessor->m_pVASTXperience.m_Poly.voicesMSEGStillActive()) {
					m_bWavetableSoftfadePickedUp = true;
				}

				if (m_bWavetableSoftfadePickedUp && (isInSingleNoteSoftFadeCycle() <= 0)) {
					m_bWavetableSoftfadeStillNeeded.store(false);
					m_iSingleNoteSoftFadeID = -1;
					DBG("endSoftFade Resetting m_bWavetableSoftfadeStillNeeded!");
				}
			}
			else clearSingleNoteSoftFadeCycle();

			if (!m_bWavetableSoftfadeStillNeeded) {
				DBG("endSoftFade merged - editing threads#: " + String(m_numEditingSoftFadeNext) + " replacing WT " + String(m_wavetable->getID()) + " with new WT " + String(m_wavetable_soft_fade->getID()));
				//m_wavetable = m_wavetable_soft_fade;
				std::atomic_store(&m_wavetable, m_wavetable_soft_fade); //assigning a new instance to a shared pointer make thread safe: https://www.modernescpp.com/index.php/atomic-smart-pointers
				//m_wavetable_soft_fade = nullptr;
				std::atomic_store(&m_wavetable_soft_fade, std::shared_ptr<CVASTWaveTable>{}); //thread safe nullptr https://stackoverflow.com/questions/30117975/is-thread-safe-to-assign-a-shared-ptr-nullptr

				m_bWtSoftChangedFdv = true; //CHECK
				m_bWtSoftChangedOsc = true; //CHECK
				m_bWtSoftChangedOscEditor = true; //CHECK
				m_bWtSoftChangedPos = true; //CHECK
				m_bWtSoftChangedStructure = true;
				m_bWavetableSoftfadeStillNeeded = false;
				m_iSingleNoteSoftFadeID = -1;
				m_bWavetableSoftfadePickedUp = false;

				lResult = true;
			}
			else
				DBG("m_wavetable_soft_fade still used ID: " + String(m_wavetable_soft_fade->getID()));
			mSharedPtrSoftFadeLock.exit();
		}
	}
	return lResult;
}

std::shared_ptr<CVASTWaveTable> CVASTOscillatorBank::getNewSharedWavetable() { //by value: share ownership
	//return m_wavetable;	//can be changed in meantime, need atomic_store here??
	std::shared_ptr<CVASTWaveTable> localtr;
	std::atomic_store(&localtr, m_wavetable); //assigning a new instance to a shared pointer make thread safe: https://www.modernescpp.com/index.php/atomic-smart-pointers
	return localtr;
}

const std::shared_ptr<CVASTWaveTable>& CVASTOscillatorBank::getWavetablePointer() {
	return m_wavetable;
}

std::shared_ptr<CVASTWaveTable> CVASTOscillatorBank::getNewSharedSoftFadeWavetable() {
//	return m_wavetable_soft_fade; //can be changed in meantime, need atomic_store here??
	std::shared_ptr<CVASTWaveTable> localtr;
	std::atomic_store(&localtr, m_wavetable_soft_fade); //assigning a new instance to a shared pointer make thread safe: https://www.modernescpp.com/index.php/atomic-smart-pointers
	return localtr;
}

const std::shared_ptr<CVASTWaveTable>& CVASTOscillatorBank::getSoftFadeWavetablePointer() {
	return m_wavetable_soft_fade;
}

std::shared_ptr<CVASTWaveTable> CVASTOscillatorBank::getNewSharedSoftFadeWavetableNext() {
	//return m_wavetable_soft_fade_next; //can be changed in meantime, need atomic_store here??
	std::shared_ptr<CVASTWaveTable> localtr;
	std::atomic_store(&localtr, m_wavetable_soft_fade_next); //assigning a new instance to a shared pointer make thread safe: https://www.modernescpp.com/index.php/atomic-smart-pointers
	return localtr;
}

const std::shared_ptr<CVASTWaveTable>& CVASTOscillatorBank::getSoftFadeWavetablePointerNext() {
	return m_wavetable_soft_fade_next;
}

void CVASTOscillatorBank::prepareForPlay(int expectedSamplesPerBlock) {
	m_wavetable->prepareForPlay(expectedSamplesPerBlock);
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

bool CVASTOscillatorBank::needsUndo() {
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

std::shared_ptr<CVASTWaveTable> CVASTOscillatorBank::getSoftOrCopyWavetable(bool getCopy, bool copyAlsoFreqs) {
	//make thread safe!! https://stackoverflow.com/questions/7688107/is-copy-thread-safe

	std::shared_ptr<CVASTWaveTable> wtshared;
	if ((!getCopy) && (getSoftFadeWavetablePointerNext() != nullptr)) {
		//wtshared = getNewSharedSoftFadeWavetableNext(); 
		std::atomic_store(&wtshared, getNewSharedSoftFadeWavetableNext()); //check
	}
	else {
		vassert(m_wavetable->m_Set == m_Set);

		//wtshared = getNewSharedWavetable()->getClonedInstance(false); //dont delete generated stuff
		std::atomic_store(&wtshared, getNewSharedWavetable()->getClonedInstance(false, copyAlsoFreqs)); //check
		setWavetableSoftFade(wtshared);
	}
	vassert(wtshared != nullptr);
	return wtshared;
}