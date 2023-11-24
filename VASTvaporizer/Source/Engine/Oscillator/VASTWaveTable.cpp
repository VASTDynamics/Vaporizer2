/*
VAST Dynamics Audio Software (TM)

*/

#include "VASTWaveTable.h"
#include "VASTWaveTableOscillator.h"
#include "../VASTSettings.h"
#include <chrono>
#include <math.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "../Utils/VASTFFTSingletonFactory.h"
#include "../../Plugin/VASTWaveTableEditorComponent.h" //TODO better move to engine class
#include "../VASTOscillatorBank.h"

CVASTWaveTable::CVASTWaveTable(CVASTSettings &set) : m_Set(&set) {
	clear();

	int initSize = 16;
	m_x0_curPos = std::make_unique<AudioSampleBuffer>(1, initSize);
	m_y0_curPos = std::make_unique<AudioSampleBuffer>(1, initSize);
	m_y1_curPos = std::make_unique<AudioSampleBuffer>(1, initSize);
	m_x0_nextPos = std::make_unique<AudioSampleBuffer>(1, initSize);
	m_y0_nextPos = std::make_unique<AudioSampleBuffer>(1, initSize);
	m_y1_nextPos = std::make_unique<AudioSampleBuffer>(1, initSize);
	//m_interpolBuffer = std::make_unique<AudioSampleBuffer>(1, initSize);
	m_fracPartBuffer = std::make_unique<AudioSampleBuffer>(1, initSize);
	//m_fracPartQuad = std::make_unique<AudioSampleBuffer>(n1, initSize);
	m_oscBuffer = std::make_unique<AudioSampleBuffer>(1, initSize);
	//m_oscBufferStereo = std::make_unique<AudioSampleBuffer>(2, initSize);
	m_begin_nextPosBuffer = std::make_unique<AudioSampleBuffer>(2, initSize);
	m_begin_nextPosBufferNextWtfxtype = std::make_unique<AudioSampleBuffer>(2, initSize);
	m_begin_curPosBuffer = std::make_unique<AudioSampleBuffer>(2, initSize);
	m_begin_curPosBufferNextWtfxtype = std::make_unique<AudioSampleBuffer>(2, initSize);
	m_end_nextPosBuffer = std::make_unique<AudioSampleBuffer>(2, initSize);
	m_end_nextPosBufferNextWtfxtype = std::make_unique<AudioSampleBuffer>(2, initSize);
	m_end_curPosBuffer = std::make_unique<AudioSampleBuffer>(2, initSize);
	m_end_curPosBufferNextWtfxtype = std::make_unique<AudioSampleBuffer>(2, initSize);

	m_valBeginBuffer = std::make_unique<AudioSampleBuffer>(2, initSize);
	m_valBeginBufferNext = std::make_unique<AudioSampleBuffer>(2, initSize);
	m_valEndBuffer = std::make_unique<AudioSampleBuffer>(2, initSize);
	m_valEndBufferNext = std::make_unique<AudioSampleBuffer>(2, initSize);

	m_iMultiSelectBegin = 0;
	m_iMultiSelectEnd = 0;
	m_iSelectedPosition = 0;
	m_multiSelect = false;
	m_iWaveTableID = m_Set->m_sWaveTableID++;

	bufferedValueTree.removeAllChildren(nullptr);
	bufferedValueTree.removeAllProperties(nullptr);
}

CVASTWaveTable::CVASTWaveTable(const CVASTWaveTable &wavetable) : CVASTWaveTable(*wavetable.m_Set) { //copy constructor
	copyFrom(wavetable);
}

CVASTWaveTable::~CVASTWaveTable(void) {	
	VDBG("Wavetable deleted ID: " << getID());
}

void CVASTWaveTable::copyFrom(const CVASTWaveTable &wavetable) { //copies only the data, does not clone the shared instance
	//wavetable.mWavetableChangeLock.enter(); //dont lock here - this is causing the audio thread to pause e.g. in Bitwig

	clear();
	wtheader.waveTableName = wavetable.wtheader.waveTableName;
	wtheader.numPositions.store(wavetable.wtheader.numPositions.load());
	//wtheader.waveTablePositions = std::vector<sWaveTablePosition>{};

	//expensive part!!
	//wtheader.waveTablePositions = wavetable.wtheader.waveTablePositions; 

	wtheader.waveTablePositions.reserve(wavetable.wtheader.waveTablePositions.size()); //https://stackoverflow.com/questions/39075850/fastest-way-to-copy-a-vector-with-specific-changes-in-c
	std::transform(wavetable.wtheader.waveTablePositions.begin(), wavetable.wtheader.waveTablePositions.end(), std::back_inserter(wtheader.waveTablePositions),
		[](sWaveTablePosition e) { 
			sWaveTablePosition copypos;
			copypos.dirty = true;
			copypos.frequencyDomainBuffer = e.frequencyDomainBuffer;
			copypos.hasFXTable = false;
			copypos.isInitial = e.isInitial;
			copypos.maxHarmonics = e.maxHarmonics;
			copypos.naiveTable = e.naiveTable;
			copypos.wtPos = e.wtPos;
			copypos.numWaveTableFreqs = 0;
			copypos.numWaveTableFreqsBuffer = 0;
			copypos.naiveTableFX = std::vector<float>{};
			copypos.naiveTableFXDisplayCopy = std::vector<float>{};
			copypos.waveTableFreqs = std::vector<sWaveTableFreq>{};
			copypos.waveTableFreqsBuffer = std::vector<sWaveTableFreq>{};
			return copypos;
	});
	//expensive part!!

	wtheader.changeCounter.store(wavetable.wtheader.changeCounter.load());

	m_multiSelect = wavetable.m_multiSelect;
	m_iMultiSelectBegin = wavetable.m_iMultiSelectBegin;
	m_iMultiSelectEnd = wavetable.m_iMultiSelectEnd;
	m_iSelectedPosition = wavetable.m_iSelectedPosition;

	//wavetable.mWavetableChangeLock.exit();

#ifdef _DEBUG
	vassert(validate());
#endif
}

void CVASTWaveTable::copyWTFreqsFrom(const CVASTWaveTable& wavetable) {
	for (int wtPos = 0; wtPos < wtheader.numPositions.load(); wtPos++) {
		wtheader.waveTablePositions[wtPos].dirty = wavetable.wtheader.waveTablePositions[wtPos].dirty; //keep it dirty if it was so!
		wtheader.waveTablePositions[wtPos].numWaveTableFreqs = wavetable.wtheader.waveTablePositions[wtPos].numWaveTableFreqs;
		wtheader.waveTablePositions[wtPos].numWaveTableFreqsBuffer = wavetable.wtheader.waveTablePositions[wtPos].numWaveTableFreqsBuffer;
		wtheader.waveTablePositions[wtPos].waveTableFreqs = wavetable.wtheader.waveTablePositions[wtPos].waveTableFreqs;
		wtheader.waveTablePositions[wtPos].waveTableFreqsBuffer = wavetable.wtheader.waveTablePositions[wtPos].waveTableFreqsBuffer;
	};
}

std::shared_ptr<CVASTWaveTable> CVASTWaveTable::getClonedInstance(bool deleteGeneratedContent, bool copyAlsoFreqs) {
	//std::shared_ptr<CVASTWaveTable> wtshared = std::make_shared<CVASTWaveTable>(*this); //copy constructor implict
	std::shared_ptr<CVASTWaveTable> wtshared;
	std::atomic_store(&wtshared, std::make_shared<CVASTWaveTable>(*this)); //assigning a new instance to a shared pointer make thread safe: https://www.modernescpp.com/index.php/atomic-smart-pointers

	if (copyAlsoFreqs)
		wtshared->copyWTFreqsFrom(*this);
	if (deleteGeneratedContent)
		wtshared->deleteGeneratedContent();
	return wtshared;
}

void CVASTWaveTable::deleteGeneratedContent() {
	ScopedLock sl(mWavetableChangeLock);

	for (int wtPos = 0; wtPos < wtheader.numPositions.load(); wtPos++) {
		wtheader.waveTablePositions[wtPos].waveTableFreqsBuffer.clear();
		wtheader.waveTablePositions[wtPos].numWaveTableFreqsBuffer = 0;
		wtheader.waveTablePositions[wtPos].hasFXTable = false;
		wtheader.waveTablePositions[wtPos].naiveTableFXDisplayCopy.clear();
		wtheader.waveTablePositions[wtPos].naiveTableFX.clear();
		wtheader.waveTablePositions[wtPos].naiveTableFXDirty = false;
	}
};

//-------------------------------------------------------------------------------------------
// valueTree state
/*
float CVASTWaveTable::strToFloat(String* myString) {
	uint32_t num;
	sscanf_s(myString->toStdString().c_str(), "%x", &num);
	return *((float*)&num);
}

String CVASTWaveTable::floatToStr(float value) {
	char arr[9];
	snprintf(arr, 9, "%08x", *(int *)&value);
	return String(arr);
}
*/

void CVASTWaveTable::getValueTreeState(ValueTree* tree, UndoManager* undoManager) { //save
	ScopedLock sl(mWavetableChangeLock);

	tree->removeAllChildren(undoManager);
	tree->removeAllProperties(undoManager);

	tree->setProperty("waveTableName", wtheader.waveTableName, undoManager);
	tree->setProperty("numPositions", wtheader.numPositions.load(), undoManager);
	//positions
	for (int i = 0; i < wtheader.numPositions.load(); i++) {
        std::unique_ptr<ValueTree> waveTablePositionTree(new ValueTree(Identifier("waveTablePosition" + String(i))));

		/*
         std::unique_ptr<ValueTree> waveTablePositionNaiveTableTree(new ValueTree(Identifier("naiveTable")));
		for (int n = 0; n < C_WAVE_TABLE_SIZE; n++) {
			waveTablePositionNaiveTableTree->setProperty("val" + String(n), wtheader.waveTablePositions[i].naiveTable[n], undoManager);
		}
		waveTablePositionTree->appendChild(*waveTablePositionNaiveTableTree, undoManager);
		*/

		waveTablePositionTree->setProperty("isInitial", wtheader.waveTablePositions[i].isInitial, undoManager);

        /* old logic
        {
            std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
            if (wtheader.waveTablePositions[i].isInitial == false) {
                String nTable = "";
                //nTable.preallocateBytes(C_WAVE_TABLE_SIZE * 8);
                nTable.preallocateBytes(C_WAVE_TABLE_SIZE * 32); //due to UTF-8!
                auto* dest = (char*)nTable.getCharPointer().getAddress();

                for (int n = 0; n < C_WAVE_TABLE_SIZE; n++) {
                    float sval = wtheader.waveTablePositions[i].naiveTable[n];
                    unsigned int* sint = (unsigned int*)&sval;
                    String s1 = String::toHexString(*sint);
                    s1 += String::repeatedString("0", 8 - s1.length());

                    memcpy(dest + n * 8, s1.getCharPointer().getAddress(), 8);
                }
                waveTablePositionTree->setProperty("naiveTable", nTable, undoManager);
            }
            std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
            VDBG("Duration: " << duration);
        }
        */

        {
            //std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
            if (wtheader.waveTablePositions[i].isInitial == false) {
                String nTable = String::repeatedString("0", C_WAVE_TABLE_SIZE * 8); //fill with 0s
                auto* dest = (char*)nTable.getCharPointer().getAddress();
                for (int n = 0; n < C_WAVE_TABLE_SIZE; n++) {
                    float sval = wtheader.waveTablePositions[i].naiveTable[n];
                    unsigned int* sint = (unsigned int*)&sval;
                    String s1 = String::toHexString(*sint);
                    int s1Len = s1.length();
                    memcpy(dest + n * 8 +(8-s1Len), s1.getCharPointer().getAddress(), s1Len); //memcpy is OK since chars are ASCII
                }
                waveTablePositionTree->setProperty("naiveTable", nTable, undoManager);
            }
            //std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
            //auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
            //VDBG("Duration: " << duration);
        }

		waveTablePositionTree->setProperty("maxHarmonics", wtheader.waveTablePositions[i].maxHarmonics, undoManager);
		waveTablePositionTree->setProperty("numWaveTableFreqs", wtheader.waveTablePositions[i].numWaveTableFreqs, undoManager);
		waveTablePositionTree->setProperty("wtPos", wtheader.waveTablePositions[i].wtPos, undoManager);
		tree->appendChild(*waveTablePositionTree, undoManager);
	}
	bufferedValueTree = tree->createCopy();
	bufferedValueTree.copyPropertiesAndChildrenFrom(*tree, nullptr);
}

bool CVASTWaveTable::setValueTreeState(ValueTree* tree, int wtMode) { //load
	ScopedLock sl(mWavetableChangeLock);
    m_isBeingUpdated.store(true);
	clear();
	wtheader.waveTableName = tree->getProperty("waveTableName");
	wtheader.numPositions.store(tree->getProperty("numPositions"));
	wtheader.waveTablePositions = std::vector<sWaveTablePosition>{};
	wtheader.changeCounter.store(0);
	//positions
	for (int i = 0; i < wtheader.numPositions.load(); i++) {
		ValueTree waveTablePositionTree = tree->getChildWithName(Identifier("waveTablePosition" + String(i)));
		vassert(waveTablePositionTree.isValid());		
		sWaveTablePosition wtp = emptyPosition();

		//wtp.dirty = waveTablePositionTree.getProperty("dirty");
		wtp.dirty = true; //dirty on reload
		wtp.isInitial = waveTablePositionTree.getProperty("isInitial");

		/*
		ValueTree waveTablePositionNaiveTableTree = waveTablePositionTree.getChildWithName(Identifier("naiveTable"));
		vassert(waveTablePositionNaiveTableTree.isValid());
		wtp.naiveTable = std::vector<float>(C_WAVE_TABLE_SIZE);
		for (int n = 0; n < C_WAVE_TABLE_SIZE; n++) {
			float fl = waveTablePositionNaiveTableTree.getProperty("val"+String(n));
			wtp.naiveTable[n] = fl;
		}
		*/

		wtp.naiveTable = std::vector<float>(C_WAVE_TABLE_SIZE);
		if (wtp.isInitial == false) {
			String nTable = waveTablePositionTree.getProperty("naiveTable"); //Todo "naiveTable"			
			std::string substring = nTable.toStdString();
			int pos = 0;
			for (int n = 0; n < C_WAVE_TABLE_SIZE; n++) {				
				char charstr[9] = "        ";
				substring.copy(charstr, 8, pos);
				//float fl = hexStrToFloat(charstr); //8 hex chars = 4 bytes per float			

				unsigned int sint = (String(charstr)).getHexValue32();
				float fl = *(float*)(&sint);

				wtp.naiveTable[n] = fl;
				pos = pos + 8;
			}
		}

		//wtp.frequencyDomainBuffer = wavetable.wtheader.waveTablePositions[i].frequencyDomainBuffer; //is dynamic
		wtp.frequencyDomainBuffer = std::vector<dsp::Complex<float>>(C_WAVE_TABLE_SIZE, 0.f);

		wtp.maxHarmonics = waveTablePositionTree.getProperty("maxHarmonics");
		wtp.numWaveTableFreqs = waveTablePositionTree.getProperty("numWaveTableFreqs");
		wtp.wtPos = waveTablePositionTree.getProperty("wtPos");
		wtp.waveTableFreqs = std::vector<sWaveTableFreq>{};
		
		/*
		for (int j = 0; j <wavetable.wtheader.waveTablePositions[i].numWaveTableFreqs; j++) {
			sWaveTableFreq wtf;
			wtf.invalid = wavetable.wtheader.waveTablePositions[i].waveTableFreqs[j].invalid;
			wtf.dirty = wavetable.wtheader.waveTablePositions[i].waveTableFreqs[j].dirty;
			wtf.topFreq = wavetable.wtheader.waveTablePositions[i].waveTableFreqs[j].topFreq;
			wtf.bottomFreq = wavetable.wtheader.waveTablePositions[i].waveTableFreqs[j].bottomFreq;
			wtf.maxHarmonics = wavetable.wtheader.waveTablePositions[i].waveTableFreqs[j].maxHarmonics;
			wtf.waveTableLen = wavetable.wtheader.waveTablePositions[i].waveTableFreqs[j].waveTableLen;
			wtf.waveTableSamples = wavetable.wtheader.waveTablePositions[i].waveTableFreqs[j].waveTableSamples;
			wtp.waveTableFreqs.push_back(wtf);
			vassert(wtp.waveTableFreqs.size() > 0);
		}
		*/
		wtheader.waveTablePositions.push_back(wtp);		
	}

	for (int i = 0; i < wtheader.numPositions.load(); i++) {
		if (i != wtheader.waveTablePositions[i].wtPos) return false;
		//CHANGE!! no longer pregenerate - is done at the end with FX
        setNaiveTableFast(wtheader.waveTablePositions[i].wtPos, false, wtMode); //dont pregenerate here
	}

    m_isBeingUpdated.store(false);
	if (!validate()) return false;

	return true;
}

//-------------------------------------------------------------------------------------------

void CVASTWaveTable::markAllWTFreqsDirty() { //to recaclculate wavetable
	//lock here?
	for (int wtPos = 0; wtPos < wtheader.numPositions.load(); wtPos++) {
		for (int wtFreq = 0; wtFreq < wtheader.waveTablePositions[wtPos].waveTableFreqs.size(); wtFreq++) {
			wtheader.waveTablePositions[wtPos].waveTableFreqs[wtFreq].dirty = true;
		}
	}
}

void CVASTWaveTable::pregenerateWithWTFX(int wtFxType, float wtFxVal, int wtMode) {
	for (int wtPos = 0; wtPos < wtheader.numPositions.load(); wtPos++) {
		VDBG("Pregenerating WTPos: " << wtPos << " with wtfxtype - val: " << wtFxType << " " << wtFxVal);
		for (int wtFreq = 0; wtFreq < wtheader.waveTablePositions[wtPos].waveTableFreqs.size(); wtFreq++) {
			makeWaveTableFreq(	wtPos, 
								wtheader.waveTablePositions[wtPos].waveTableFreqs[wtFreq].waveTableLen, 
								wtheader.waveTablePositions[wtPos].waveTableFreqs[wtFreq].bottomFreq, 
								wtheader.waveTablePositions[wtPos].waveTableFreqs[wtFreq].topFreq,
								wtheader.waveTablePositions[wtPos].waveTableFreqs[wtFreq].maxHarmonics,
								true, wtFreq, true, wtMode, wtFxVal, wtFxType, -1.f); //curfreq -1 to force usage of topfreq
		}
	}
}

void CVASTWaveTable::setSelectedWtPos(int wtPos) {
	int numPositions = getNumPositions();
	vassert((numPositions > 0) && (numPositions <= C_MAX_NUM_POSITIONS));
	m_iSelectedPosition = jlimit<int>(0, numPositions - 1, wtPos);
	clearMultiSelect();
}

int CVASTWaveTable::getSelectedWtPos() const {
	return m_iSelectedPosition;
}

bool CVASTWaveTable::isMultiSelected() const {
#ifdef _DEBUG
	if (m_multiSelect)
		vassert((m_iSelectedPosition >= m_iMultiSelectBegin) && (m_iSelectedPosition <= m_iMultiSelectEnd));
#endif
	return m_multiSelect;
}

int CVASTWaveTable::getMultiSelectBegin() const {
	if (!m_multiSelect)
		return m_iSelectedPosition;
	return m_iMultiSelectBegin;
}

int CVASTWaveTable::getMultiSelectEnd() const {
	if (!m_multiSelect)
		return m_iSelectedPosition;
	return m_iMultiSelectEnd;
}

void CVASTWaveTable::multiSelectAll() {
	m_multiSelect = true;
	m_iMultiSelectBegin = 0;
	m_iMultiSelectEnd = getNumPositions() - 1;
}

void CVASTWaveTable::clearMultiSelect() {
	m_multiSelect = false;
	m_iMultiSelectBegin = 0;
	m_iMultiSelectEnd = 0;
}

void CVASTWaveTable::setMultiSelect(int wtPos) {
	m_multiSelect = true;
	if (wtPos < getSelectedWtPos()) {
		m_iMultiSelectBegin = wtPos;
		m_iMultiSelectEnd = (getSelectedWtPos() <= getNumPositions() - 1 ? getSelectedWtPos() : getNumPositions() - 1);
	}
	else {
		m_iMultiSelectBegin = getSelectedWtPos();
		m_iMultiSelectEnd = (wtPos <= getNumPositions() - 1 ? wtPos : getNumPositions() - 1);
	}
	m_iSelectedPosition = jlimit<int>(m_iMultiSelectBegin, m_iMultiSelectEnd, m_iSelectedPosition);
	vassert(m_iMultiSelectBegin <= m_iMultiSelectEnd);
}

void CVASTWaveTable::setSelection(int begin, int end) {
	m_iMultiSelectBegin = (begin < 0) ? 0 : begin;
	m_iMultiSelectEnd = (end > getNumPositions() - 1) ? getNumPositions() - 1 : end;
	if (m_iMultiSelectBegin != m_iMultiSelectEnd) {
		m_multiSelect = true;
		m_iSelectedPosition = jlimit<int>(m_iMultiSelectBegin, m_iMultiSelectEnd, m_iSelectedPosition);
	}
	else {
		m_multiSelect = false;
		m_iSelectedPosition = m_iMultiSelectBegin;
	}
}

void CVASTWaveTable::duplicatePosition(int wtPos, int newPos) {
	if (getNumPositions() >= C_MAX_NUM_POSITIONS) return;
	ScopedLock sl(mWavetableChangeLock);

	sWaveTablePosition wtp = wtheader.waveTablePositions[wtPos]; //copy the data

	wtheader.waveTablePositions.insert(wtheader.waveTablePositions.begin() + newPos, wtp);
	wtheader.numPositions+=1;
	wtheader.changeCounter+=1;
	for (int i = 0; i < getNumPositions(); i++) {
		wtheader.waveTablePositions[i].wtPos = i;
	}

#ifdef _DEBUG
	vassert(validate());
#endif
}

void CVASTWaveTable::addFromOtherWavetable(int newPos, sWaveTablePosition wtp) {
	ScopedLock sl(mWavetableChangeLock);
	wtheader.waveTablePositions.insert(wtheader.waveTablePositions.begin() + newPos, wtp);
	wtheader.numPositions+=1; //at the end
	wtheader.changeCounter+=1;
	for (int i = 0; i < getNumPositions(); i++) {
		wtheader.waveTablePositions[i].wtPos = i;
	}
}

void CVASTWaveTable::copyPositionToOtherWavetable(int wtPos, int newPos, CVASTWaveTable* wavetable) {
	if (wavetable->getNumPositions() >= C_MAX_NUM_POSITIONS) return;
	wavetable->addFromOtherWavetable(newPos, wtheader.waveTablePositions[wtPos]); //copy the data
} 

void CVASTWaveTable::clear() {
	ScopedLock sl(mWavetableChangeLock);

	wtheader.numPositions.store(0);
	wtheader.waveTablePositions.clear();
	wtheader.changeCounter.store(0);

	wtheader.waveTableName = String(TRANS(""));
	
	m_lastWaveTableIdx = 0; //performance optimize
	m_lastWaveTableIdxNext = 0; //performance optimize
	m_lastPhaseInc = -1; //performance optimize
}

int CVASTWaveTable::getNumPositions() const {
	return wtheader.numPositions.load();
}

juce::String CVASTWaveTable::getWaveTableName() {
	return wtheader.waveTableName;
}

void CVASTWaveTable::setWaveTableName(StringRef wtname) {
	wtheader.waveTableName = wtname;
}

void CVASTWaveTable::insertEmptyPosition(int wtPos) {
	if (getNumPositions() >= C_MAX_NUM_POSITIONS) return;
	ScopedLock sl(mWavetableChangeLock);

	sWaveTablePosition wtp = emptyPosition();
	wtp.wtPos = wtPos + 1;
	wtheader.waveTablePositions.insert(wtheader.waveTablePositions.begin() + wtPos + 1, wtp);
	wtheader.numPositions+=1; //at the end
	wtheader.changeCounter+=1;
	for (int i = 0; i < getNumPositions(); i++) {
		wtheader.waveTablePositions[i].wtPos = i;
	}

	//this is not atomic?
	float bottomFreq = 0.0f;
	float topFreq = 1.0f;
	int maxHarmonic = 0;
	std::vector<float> emptyTable(C_WAVE_TABLE_SIZE);
	addWaveTableFreq(wtp.wtPos, C_WAVE_TABLE_SIZE, emptyTable, bottomFreq, topFreq, maxHarmonic, false, false, 0.f, 0, false, wtp.naiveTableFX);
}

sWaveTablePosition CVASTWaveTable::emptyPosition() {
	sWaveTablePosition wtp;
	wtp.wtPos = -1; //has to be set later
	wtp.numWaveTableFreqs = 0;
	wtp.numWaveTableFreqsBuffer = 0;
	wtp.waveTableFreqsBuffer = std::vector<sWaveTableFreq>{};
	wtp.waveTableFreqs = std::vector<sWaveTableFreq>{};
	wtp.naiveTable = std::vector<float>(C_WAVE_TABLE_SIZE, 0.f);
	wtp.naiveTableFX = std::vector<float>(C_WAVE_TABLE_SIZE, 0.f);
	wtp.naiveTableFXDisplayCopy = std::vector<float>(C_WAVE_TABLE_SIZE, 0.f);
	wtp.naiveTableFXDirty = false;
	wtp.hasFXTable = false;
	wtp.frequencyDomainBuffer = std::vector<dsp::Complex<float>>(C_WAVE_TABLE_SIZE, 0.f);
	wtheader.changeCounter+=1;
	wtp.maxHarmonics = 0;
	wtp.dirty = false;
	wtp.isInitial = true;

	return wtp;	
}

void CVASTWaveTable::addPosition() {
	if (wtheader.numPositions < C_MAX_NUM_POSITIONS) {
		ScopedLock sl(mWavetableChangeLock);

		wtheader.numPositions+=1; // CHECK
		wtheader.changeCounter+=1;
		sWaveTablePosition wtp = emptyPosition();
		wtp.wtPos = wtheader.numPositions.load() - 1;

		wtheader.waveTablePositions.push_back(wtp);
		//this is not atomic?
		float bottomFreq = 0.0f;
		float topFreq = 1.0f;
		int maxHarmonic = 0;
		std::vector<float> emptyTable(C_WAVE_TABLE_SIZE);
		addWaveTableFreq(wtp.wtPos, C_WAVE_TABLE_SIZE, emptyTable, bottomFreq, topFreq, maxHarmonic, false, false, 0.f, 0, false, wtp.naiveTableFX);
		//this is not atomic?
	}
}

void CVASTWaveTable::addPositions(int numPos) {
	ScopedLock sl(mWavetableChangeLock);

	for (int i = 0; i < numPos; i++) {
		addPosition();
	}
}

//does also delete last one
void CVASTWaveTable::deletePosition(int numPos) {
	ScopedLock sl(mWavetableChangeLock);

	if (wtheader.numPositions.load()-1 >= numPos) {
		wtheader.numPositions-=1; // CHECK

		wtheader.waveTablePositions.erase(wtheader.waveTablePositions.begin() + numPos); //iterator
		wtheader.changeCounter+=1;
	}
	//reassign wtPos
	for (int i = 0; i < getNumPositions(); i++) {
		wtheader.waveTablePositions[i].wtPos = i;
	}
	if (getNumPositions() <= 0) {
		m_iSelectedPosition = 0;
		m_iMultiSelectBegin = 0;
		m_iMultiSelectEnd = 0;
		m_multiSelect = false;
	}
	else {
		m_iSelectedPosition = jlimit<int>(0, getNumPositions() - 1, m_iSelectedPosition);
		m_iMultiSelectBegin = jlimit<int>(0, getNumPositions() - 1, m_iMultiSelectBegin);
		m_iMultiSelectEnd = jlimit<int>(0, getNumPositions() - 1, m_iMultiSelectEnd);
	}
}

bool CVASTWaveTable::positionIsPrepared(int wtPos) const {
//	int wtsize = wtheader.waveTablePositions.size();
//	if (wtPos < wtheader.numPositionsPrepared) {
		//if (wtPos < wtsize) {
//		return (wtheader.waveTablePositions[wtPos].numWaveTableFreqs > 0); //is prepared then freqs exist
//	}
//	return false;
	//return !wtheader.dirty;
	return !wtheader.waveTablePositions[wtPos].dirty;
}

bool CVASTWaveTable::getWaveTablePosition(int wtPos, sWaveTablePosition* &waveTablePosition)  {
	vassert(wtheader.waveTablePositions.size() + 1 > wtPos);
	if (wtheader.waveTablePositions.size() < wtPos + 1) return false;
	waveTablePosition = &wtheader.waveTablePositions[wtPos];
	return true;
}

int CVASTWaveTable::getNumWaveTableFreqs(int wtPos) {
	sWaveTablePosition* wtp;
	bool ok = getWaveTablePosition(wtPos, wtp); // this prepares, CHECK!!
	if (!ok) return 0;
	return wtp->numWaveTableFreqs;
}

bool CVASTWaveTable::getWavetableFreq(int wtPos, int freqindex, float* bottomFreq, float* topFreq) {
	sWaveTablePosition* wtp;
	bool ok = getWaveTablePosition(wtPos, wtp);
	if (!ok) return false;
	*topFreq = wtp->waveTableFreqs[freqindex].topFreq;
	*bottomFreq = wtp->waveTableFreqs[freqindex].bottomFreq;
	return true;
}

bool CVASTWaveTable::getWaveTablePosition(int wtPos, sWaveTablePosition* &waveTablePosition, sWaveTablePosition* &waveTablePositionNextPos, bool &hasNextPos) {

	bool ok = getWaveTablePosition(wtPos, waveTablePosition);
	if (!ok) return false;

	//if ((wtheader.numPositionsPrepared > 1) && ((wtPos + 1) < wtheader.numPositionsPrepared)) {
	
	if (positionIsPrepared(wtPos)) {
		ok = getWaveTablePosition(wtPos + 1, waveTablePositionNextPos);
		if (!ok) return false;
		hasNextPos = true;
		//vassert(waveTablePositionNextPos->numWaveTableFreqs > 0);
	}
	else {
		waveTablePositionNextPos = waveTablePosition;
		hasNextPos = false;
	}
	return true;
}

void CVASTWaveTable::createFreqsIfNeeded(int wtPos, bool preGenerate, int wtMode)  { //prepared means that naive is there and freqs - but not necessarily samples
	ScopedLock sl(mWavetableChangeLock);
	VDBG("createFreqsIfNeeded wtPos " << wtPos);
	if (!positionIsPrepared(wtPos)) {
		VDBG("createFreqsIfNeeded - generate - wtPos " << wtPos);
		vassert(wtheader.waveTablePositions.size() >= (wtPos + 1));		
		generateWaveTableFreqsFromTimeDomain(wtPos, C_WAVE_TABLE_SIZE, *getNaiveTable(wtPos),  preGenerate, wtMode);

	}
	wtheader.waveTablePositions[wtPos].dirty = false;
	wtheader.waveTablePositions[wtPos].isInitial = false;
}

void CVASTWaveTable::setNaiveTableFast(int wtPos, bool preGenerate, int wtMode) {
	ScopedLock sl(mWavetableChangeLock);
	// isswtheader.waveTablePositions[wtPos].naiveTable
	wtheader.waveTablePositions[wtPos].dirty = true;
	wtheader.waveTablePositions[wtPos].isInitial = false;
	wtheader.changeCounter += 1;

	for (int i = 0; i < wtheader.waveTablePositions[wtPos].waveTableFreqs.size(); i++) {
		wtheader.waveTablePositions[wtPos].waveTableFreqs[i].invalid = true;
		wtheader.waveTablePositions[wtPos].waveTableFreqs[i].dirty = true;
	}

	createFreqsIfNeeded(wtPos, preGenerate, wtMode);

#ifdef _DEBUG
	vassert(validate());
#endif
}

void CVASTWaveTable::setNaiveTable(int wtPos, std::vector<float> vWave, bool preGenerate, int wtMode) {
	ScopedLock sl(mWavetableChangeLock);

	wtheader.waveTablePositions[wtPos].naiveTable = vWave; //copy the data a second time -> bad performance, use perf. method instead if possible
	wtheader.waveTablePositions[wtPos].dirty = true;
	wtheader.waveTablePositions[wtPos].isInitial = false;
	wtheader.changeCounter+=1;

	for (int i = 0; i < wtheader.waveTablePositions[wtPos].waveTableFreqs.size(); i++) {
		wtheader.waveTablePositions[wtPos].waveTableFreqs[i].invalid = true;
		wtheader.waveTablePositions[wtPos].waveTableFreqs[i].dirty = true;
	}

	createFreqsIfNeeded(wtPos, preGenerate, wtMode);

#ifdef _DEBUG
	vassert(validate());
#endif
}

bool CVASTWaveTable::validate() {
	if (!(wtheader.waveTablePositions.size() > 0)) {
		VDBG("CVASTWaveTable::validate() failed: waveTablePositions <= 0");
		return false;
	}
	if (!(wtheader.waveTablePositions.size() == wtheader.numPositions.load())) {
		VDBG("CVASTWaveTable::validate() failed: waveTablePositions.size() != wtheader.numPositions");
		return false;
	}
	for (int wtPos = 0; wtPos  < wtheader.numPositions.load(); wtPos++) {
		if (wtPos != wtheader.waveTablePositions[wtPos].wtPos) {
			VDBG("CVASTWaveTable::validate() failed: wtPos != wtheader.waveTablePositions[wtPos].wtPos");
			return false;
		}
		int validfreqs = 0;
		for (int wtFreq = 0; wtFreq < wtheader.waveTablePositions[wtPos].waveTableFreqs.size(); wtFreq++) {
			if (wtheader.waveTablePositions[wtPos].waveTableFreqs[wtFreq].invalid == false)
				validfreqs++;
		}
		for (int wtFreq = 0; wtFreq < wtheader.waveTablePositions[wtPos].waveTableFreqsBuffer.size(); wtFreq++) {
			if (wtheader.waveTablePositions[wtPos].waveTableFreqsBuffer[wtFreq].waveTableSamples.size() != C_WAVE_TABLE_SIZE) {
				VDBG("waveTableSamples empty wtPos " << wtPos << " wtFreq " << wtFreq);
				return false;
			}
		}
		if (wtheader.waveTablePositions[wtPos].numWaveTableFreqsBuffer != wtheader.waveTablePositions[wtPos].waveTableFreqsBuffer.size()) {
			VDBG("numWaveTableFreqsBuffer != waveTableFreqsBuffer size for wtPos " << wtPos);
			return false;
		}
		//vassert(validfreqs == wtheader.waveTablePositions[wtPos].numWaveTableFreqs);
	}		
	return true;
}

bool CVASTWaveTable::loadWavFile(wavFile* wavfile, bool preGenerate, int wtMode) {
	ScopedLock sl(mWavetableChangeLock);
	clear();
	bool success = true;
	//std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
	int positionstoprepare = wavfile->length;
	addPositions(positionstoprepare);
	for (int idx = 0; idx < positionstoprepare; idx++) { 
		std::vector<float> naive = naiveWaveFromWavFile(wavfile, idx);
		setNaiveTable(idx, naive, preGenerate, wtMode); //preGenerate true?
	}
	String wtname = wavfile->wtname;
	setWaveTableName(wtname);
	//std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
	//auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
	//check for different thread - can be 1s

	return success;
}

std::vector<float> CVASTWaveTable::naiveWaveFromWavFile(wavFile* wavfile, int wtPos) {
	std::vector<float> samples = std::vector<float>(C_WAVE_TABLE_SIZE);
	float stepratio = float(C_WAV_FORMAT_WT_SIZE) / float(C_WAVE_TABLE_SIZE); //for C_WAVE_TABLE_SIZE != 2048 only
	for (int i = 0; i < C_WAVE_TABLE_SIZE; i++) {
		if (wtPos < wavfile->length) {
			samples[i] = wavfile->WTBuffer[wtPos][int(i * stepratio)]; //check interpolation here
		}
		else
			samples[i] = 0.0f; //not part of the table
	}
	return samples;
}

std::vector<float> CVASTWaveTable::getSyncNaiveWave(std::vector<float> &naive, float paramvalue) {
	float l_synclen = C_WAVE_TABLE_SIZE + (256.f / 255.f) * paramvalue;
	int l_intsynclen = l_synclen;
	
	std::vector<float> naiveNew(l_intsynclen);
	std::vector<float> samples(C_WAVE_TABLE_SIZE);

	bool bNotAllTheSame = false;
	float lastVal = 0.0f;
	for (int i = 0; i < int(l_synclen); i++) {
		naiveNew[i] = naive[i % C_WAVE_TABLE_SIZE];
		if ((i > 0) && (bNotAllTheSame == false)) {
			if (naiveNew[i] != lastVal) {
				bNotAllTheSame = true;
			}
		}
		lastVal = naiveNew[i];
	}

	if (bNotAllTheSame == false) { // would cause nan
		for (int i = 0; i < C_WAVE_TABLE_SIZE; i++) {
			samples[i] = 0.0f;
		}
		return samples;
	}

	for (int i = 0; i < C_WAVE_TABLE_SIZE; i++) {
		float npos = i * (l_synclen / float(C_WAVE_TABLE_SIZE - 1)); // stretch or compress to fit to WT SIZE
		int intpart = npos;
		float fracpart = npos - intpart;
		samples[i] = (1.0f - fracpart) * naiveNew[intpart % l_intsynclen] + fracpart * (naiveNew[(intpart + 1) % l_intsynclen]);
	}
	return samples;
}

std::vector<float> CVASTWaveTable::calcSyncNaiveWave(int wtPos, int naiveWtPos, float syncQuota) {	

	std::vector<float> naive(C_WAVE_TABLE_SIZE);
	std::vector<float> samples(C_WAVE_TABLE_SIZE);
	naive = *getNaiveTable(naiveWtPos);
	
	float paramvalue = (wtPos * syncQuota);
	samples = getSyncNaiveWave(naive, paramvalue);
	return samples;
}

std::vector<float> CVASTWaveTable::getPWMNaiveWave(std::vector<float> &naive, float paramvalue) {
	std::vector<float> samples(C_WAVE_TABLE_SIZE);
	//compress to that size - the rest shall be zeros
	float pos = (C_MAX_NUM_POSITIONS - paramvalue); // invert it
	float size = (float(pos + 1) / float(C_MAX_NUM_POSITIONS + 1)) * float(C_WAVE_TABLE_SIZE);

	for (int i = 0; i < C_WAVE_TABLE_SIZE; i++) {
		float npos = i * (float(C_WAVE_TABLE_SIZE) / float(size)); // stretch or compress to fit to WT SIZE
		int intpart = npos;
		float fracpart = npos - intpart;
		if (i < int(size)) {
			samples[i] = (1.0f - fracpart) * naive[intpart] + fracpart * (naive[(intpart + 1) % C_WAVE_TABLE_SIZE]);
		}
		else
			samples[i] = 0.f;
	}

	return samples;
}

std::vector<float> CVASTWaveTable::calcPWMNaiveWave(int wtPos, int naiveWtPos, float pwmQuota) {
	std::vector<float> samples(C_WAVE_TABLE_SIZE);
	std::vector<float> naive(C_WAVE_TABLE_SIZE);	
	naive = *getNaiveTable(naiveWtPos);

	float paramvalue = (wtPos * pwmQuota);
	samples = getPWMNaiveWave(naive, paramvalue);

	return samples;
}

void CVASTWaveTable::getNaiveSamplesFromWave(std::vector<float> &nsamples, int wave) {
	int len = C_WAVE_TABLE_SIZE;
	switch (wave) {
	case WAVE::sine:
		//Sine working - only one wavetable!
		for (int i = 0; i < len; i++) {
			nsamples[i] = sin(i * M_PI * 2.0f / len);
		}
		break;
	case WAVE::saw:
		//Sawtooth working - zero start (not normalized)
		for (int i = 0; i < len; i++) {
			if (i < len / 2)
				nsamples[i] = 2.0f * (1.0f / (len - 1) * i);
			else
				nsamples[i] = -2.0f + 2.0f * (1.0f / (len - 1) * i);
		}
		break;
	case WAVE::tri:
		for (int i = 0; i < len; i++) {
			nsamples[i] = -1.0f + (2.0f / (len / 2)) * (len / 2.0f - abs((i + len / 4) % len - len / 2.0f));
		}
		break;
	case WAVE::square:
		//*square working 
		for (int i = 0; i < len; i++) {
			if (i < len / 2)
				nsamples[i] = 1.0;
			else
				nsamples[i] = -1.0;
		}
		//wSamples[0] = 0.0f;
		//wSamples[len / 2] = 0.0f;

		break;
	case WAVE::noise:
		break;
	case WAVE::sawsine:
		//Sawsine working - look not normalized und nach oben verschoben?
		for (int i = 0; i < len; i++) {
			if (i < len / 2)
				nsamples[i] = 2.0f * (1.0f / len * i);
			else
				nsamples[i] = sin(i * M_PI * 2.0f / len);
		}
		break;
	case WAVE::samplehold:
		break;
	}
}

void CVASTWaveTable::setFreqDomainTables(int wtPos, std::vector<dsp::Complex<float>>* domainBuffer, bool preGenerate, bool clipBins, int wtMode) {
	ScopedLock sl(mWavetableChangeLock);

	//only called from freq domain viewport!
	int tableLen = int((*domainBuffer).size());

	//----------------------
	for (int i = 1; i < C_WAVE_TABLE_SIZE; i++) {
		if (i >= C_WAVE_TABLE_SIZE / 2) {
			(*domainBuffer)[i].real(0.f);
			(*domainBuffer)[i].imag(0.f);
		}
		else {
			(*domainBuffer)[i].real((*domainBuffer)[i].real() * 2.f); //https://dsp.stackexchange.com/questions/19817/using-only-positive-frequencies-in-fourier-domain-how-will-it-affect-the-ifft
			//if ((domainBuffer[i].imag() < 0.001f) && (domainBuffer[i].imag() > -0.001f))
				//domainBuffer[i].imag(0.f); // https://www.gaussianwaves.com/2015/11/interpreting-fft-results-obtaining-magnitude-and-phase-information/
			(*domainBuffer)[i].imag((*domainBuffer)[i].imag() * 2.f); //https://dsp.stackexchange.com/questions/19817/using-only-positive-frequencies-in-fourier-domain-how-will-it-affect-the-ifft
		}
	}

	sWaveTablePosition* wtp;
	bool ok = getWaveTablePosition(wtPos, wtp);
	if (!ok) {
		jassertfalse;
		return; //CHECKTS
	}
	
	wtp->frequencyDomainBuffer = *domainBuffer;
	wtp->isInitial = false;

	// determine maxHarmonic, the highest non-zero harmonic in the wave
	int maxHarmonic = tableLen >> 1;
	const float minVal = 0.000001f; // -120 dB
		
	while ((fabs(wtp->frequencyDomainBuffer[maxHarmonic].real()) + fabs(wtp->frequencyDomainBuffer[maxHarmonic].imag()) < minVal) && maxHarmonic) {
		--maxHarmonic;
	}
	wtp->maxHarmonics = maxHarmonic;
	float topFreq = 2.0 / 3.0 / maxHarmonic;
	float bottomFreq = 0.f;
	
	if (wtp->waveTableFreqs.size()==0)
		makeWaveTableFreq(wtPos, C_WAVE_TABLE_SIZE, bottomFreq, topFreq, maxHarmonic, false, 0, false, wtMode, 0.f, 0, 0.f); //add
	else 
		makeWaveTableFreq(wtPos, C_WAVE_TABLE_SIZE, bottomFreq, topFreq, maxHarmonic, true, 0, false, wtMode, 0.f, 0, 0.f); //change
	vassert(wtp->waveTableFreqs[0].waveTableSamples.size() == C_WAVE_TABLE_SIZE);

	//Clip it or normalize it
	if (clipBins) {
		for (int i = 0; i < C_WAVE_TABLE_SIZE; i++) {
			if (wtp->waveTableFreqs[0].waveTableSamples[i] > 1.0f) wtp->waveTableFreqs[0].waveTableSamples[i] = 1.0f;
			if (wtp->waveTableFreqs[0].waveTableSamples[i] < -1.0f) wtp->waveTableFreqs[0].waveTableSamples[i] = -1.0f;
		}
	}
	else { //normalize
		float fMax = 0.f;
		for (int i = 0; i < C_WAVE_TABLE_SIZE; i++) {
			if (abs(wtp->waveTableFreqs[0].waveTableSamples[i]) > fMax)
				fMax = abs(wtp->waveTableFreqs[0].waveTableSamples[i]);
		}
		if (fMax != 0.f)
			for (int i = 0; i < C_WAVE_TABLE_SIZE; i++) {
				wtp->waveTableFreqs[0].waveTableSamples[i] = wtp->waveTableFreqs[0].waveTableSamples[i] / fMax;
			}
	}

	setNaiveTable(wtPos, wtp->waveTableFreqs[0].waveTableSamples, preGenerate, wtMode); //first freq now has the new naive
}


void CVASTWaveTable::frequencyDomainBufferFromNaive(int tableLen, const std::vector<float>& naiveTable, std::vector<dsp::Complex<float>>& frequencyDomainBuffer) {
	std::vector<dsp::Complex<float>> timeBuffer = std::vector<dsp::Complex<float>>(C_WAVE_TABLE_SIZE);
	int idx;
	// convert to frequency domain
	for (idx = 0; idx < tableLen; idx++) {
		//freqWaveIm[idx] = naiveTable[idx];  //old fft
		//freqWaveRe[idx] = 0.0; //old fft

		timeBuffer[idx].imag(naiveTable[idx]);
		timeBuffer[idx].real(0.0f);
	}

	//========================================================================================================================================
	//
	{
#ifdef _DEBUG	
		std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
#endif
		//---------------------------------------------------------------------------------------
		// EXPENSIVE
		m_Set->fftSingletonFactory.performFFT(&*timeBuffer.begin(), &*frequencyDomainBuffer.begin(), true); //mufft inverse
		
#ifdef _DEBUG	
		std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
		VDBG("fftw3 execute frequencyDomainBufferFromNaive duration: " << duration);
#endif
	}
	//
	//========================================================================================================================================

	//remove imag noise
	for (idx = 0; idx < tableLen; idx++) {

		frequencyDomainBuffer[idx] /= float(C_WAVE_TABLE_SIZE);

		//if (abs(wtp->frequencyDomainBuffer[idx]) < 0.000000001) //check threshold
			//wtp->frequencyDomainBuffer[idx] = std::polar(abs(wtp->frequencyDomainBuffer[idx]), 0.f);
		//perfopt https://hardforum.com/threads/replacing-abs-with-something-faster-in-c.980522/ 

		double xx = frequencyDomainBuffer[idx].real();
		double yy = frequencyDomainBuffer[idx].imag();
		float fabs = xx * xx + yy * yy;
		if (fabs < 0.000000001)
			frequencyDomainBuffer[idx] = std::polar(sqrt(fabs), 0.f);
	}

	// EXPENSIVE
	//---------------------------------------------------------------------------------------

	frequencyDomainBuffer[0].real(0.0f);
	frequencyDomainBuffer[0].imag(0.0f);
	frequencyDomainBuffer[tableLen >> 1].real(0.0f);
	frequencyDomainBuffer[tableLen >> 1].imag(0.0f);

}

std::vector<dsp::Complex<float>>* CVASTWaveTable::getFreqDomainBuffer(int wtPos) {
	/*
	if (wtheader.waveTablePositions.size() <= wtPos)
	return std::vector<dsp::Complex<float>>(C_WAVE_TABLE_SIZE);
	else return wtheader.waveTablePositions[wtPos].frequencyDomainBuffer;
	*/

	if (wtheader.waveTablePositions.size() <= wtPos) {
		vassertfalse; //should not happen!
		return &wtheader.waveTablePositions[0].frequencyDomainBuffer;  //CHECK
	}
	else
		return &wtheader.waveTablePositions[wtPos].frequencyDomainBuffer;
}

// Create wavetables from time domain (arbitrary waveform)
// all Tables - create all freq tables
void CVASTWaveTable::generateWaveTableFreqsFromTimeDomain(int wtPos, int tableLen, const std::vector<float> &naiveTable, bool preGenerate, int wtMode) {	
	ScopedLock sl(mWavetableChangeLock);

	//set invalid
	sWaveTablePosition* wtp;
	bool ok = getWaveTablePosition(wtPos, wtp);
	if (!ok) {
		vassertfalse;
		return;
	}

	int wtsize = int(wtp->waveTableFreqs.size());
	for (int i = 0; i < wtsize; i++) {
		wtp->waveTableFreqs[i].invalid = true;
	}
	wtp->numWaveTableFreqs = 0;
	wtp->numWaveTableFreqsBuffer = 0;
	wtp->waveTableFreqsBuffer = std::vector<sWaveTableFreq>{};

	//get frequencyDomainBuffer via FFT from naive
	frequencyDomainBufferFromNaive(tableLen, naiveTable, wtp->frequencyDomainBuffer);

	// determine maxHarmonic, the highest non-zero harmonic in the wave
	int maxHarmonic = tableLen >> 1;
	const float minVal = 0.000001f; // -120 dB	
	while ((fabs(wtp->frequencyDomainBuffer[maxHarmonic].real()) + fabs(wtp->frequencyDomainBuffer[maxHarmonic].imag()) < minVal) && maxHarmonic) {
		//VDBG(String(maxHarmonic) + " : " + String(fabs(wtp->frequencyDomainBuffer[maxHarmonic].real()) + fabs(wtp->frequencyDomainBuffer[maxHarmonic].imag())));
		--maxHarmonic;
	}
	wtp->maxHarmonics = maxHarmonic;
	wtp->isInitial = false;

	if (maxHarmonic == 0) {
		float bottomFreq = 0.0f; 
		float topFreq = 1.0f; 
		//std::vector<float> m_emptyTable(C_WAVE_TABLE_SIZE);
		addWaveTableFreq(wtPos, tableLen, mc_emptyTable, bottomFreq, topFreq, maxHarmonic, false, false, 0.f, 0, false, wtp->naiveTableFX);
	}
	else {

		// calculate topFreq for the initial wavetable
		// maximum non-aliasing playback rate is 1 / (2 * maxHarmonic), but we allow
		// aliasing up to the point where the aliased harmonic would meet the next
		// octave table, which is an additional 1/3
		float topFreq = 2.0 / 3.0 / maxHarmonic; 			
		float bottomFreq = 0.f; //first

		// for subsquent tables, float topFreq and remove upper half of harmonics
		//std::vector<float> ar(C_WAVE_TABLE_SIZE);	//is this real amplitude and ai imaginary amplitude?
		//std::vector<float> ai(C_WAVE_TABLE_SIZE);
		while (maxHarmonic) {
			/*
			// fill the table in with the needed harmonics
			for (idx = 0; idx < C_WAVE_TABLE_SIZE; idx++) {
				ar[idx] = ai[idx] = 0.0;
			}
			for (idx = 1; idx <= maxHarmonic; idx++) {
				ar[idx] = freqWaveRe[idx];
				ai[idx] = freqWaveIm[idx];
				ar[C_WAVE_TABLE_SIZE - idx] = freqWaveRe[C_WAVE_TABLE_SIZE - idx];
				ai[C_WAVE_TABLE_SIZE - idx] = freqWaveIm[C_WAVE_TABLE_SIZE - idx];
			}
			*/
			// it can happen here that ar and ai are all 0 starting for a certain frequency (no harmonics) - check this

			// make the wavetable
			if (preGenerate == true) {
				makeWaveTableFreq(wtPos, tableLen, bottomFreq, topFreq, maxHarmonic, false, 0, true, wtMode, 0.f, 0, 0.f); //no change create new
			}
			else
			{
				//if ((phaseinc >= topFreq/2) && (phaseinc < topFreq))
				//	makeWaveTableFreq(wave, synclen, tableLen, ar, ai, topFreq, naiveTable, false, 0); //no change create new
				//else {
				std::vector<float> empty(0);
				addWaveTableFreq(wtPos, tableLen, empty, bottomFreq, topFreq, maxHarmonic, false, true, 0.f, 0, false, wtp->naiveTableFX);
			}

			// prepare for next table
			/*
			topFreq *= 2;
			bottomFreq = topFreq * 0.5f;
			maxHarmonic >>= 1;
			*/
			//CHTS try half octaves

			int maxHarmonicOld = maxHarmonic;
			maxHarmonic = maxHarmonic / 1.25;  //quarter octave

			bottomFreq = topFreq;
			topFreq = 2.0 / 3.0 / maxHarmonic;

			if (maxHarmonicOld == maxHarmonic) 
				maxHarmonic = 0; //stop
		}
	}
	wtheader.changeCounter+=1;
}

void CVASTWaveTable::wTFX_PASS(std::vector<float>* naive, float paramvalue)
{
	//do nothing - but should not be called
	vassertfalse;
}

void CVASTWaveTable::wTFX_BENDP(std::vector<float>* naive, float paramvalue)
{
	float lValue = paramvalue * (100.f / 101.f);
	std::vector<float> inSamples = *naive;
	VASTWaveTableEditorComponent::calcBend(inSamples, *naive, 0, C_WAVE_TABLE_SIZE - 1, lValue);

}

void CVASTWaveTable::wTFX_BENDM(std::vector<float>* naive, float paramvalue)
{
	float lValue = paramvalue * (-100.f / 101.f);
	std::vector<float> inSamples = *naive;
	VASTWaveTableEditorComponent::calcBend(inSamples, *naive, 0, C_WAVE_TABLE_SIZE - 1, lValue);
}

void CVASTWaveTable::wTFX_BENDMP(std::vector<float>* naive, float paramvalue)
{
	float lValue = (-100.f + (paramvalue * 2.f)) * (100.f / 101.f);
	std::vector<float> inSamples = *naive;
	VASTWaveTableEditorComponent::calcBend(inSamples, *naive, 0, C_WAVE_TABLE_SIZE - 1, lValue);
}

void CVASTWaveTable::wTFX_BLOATP(std::vector<float>* naive, float paramvalue)
{
	float lValue = paramvalue * (100.f / 101.f);
	std::vector<float> inSamples = *naive;
	VASTWaveTableEditorComponent::calcBloat(inSamples, *naive, 0, C_WAVE_TABLE_SIZE - 1, lValue);
}

void CVASTWaveTable::wTFX_BLOATM(std::vector<float>* naive, float paramvalue)
{
	float lValue = paramvalue * (-100.f / 101.f);
	std::vector<float> inSamples = *naive;
	VASTWaveTableEditorComponent::calcBloat(inSamples, *naive, 0, C_WAVE_TABLE_SIZE - 1, lValue);
}

void CVASTWaveTable::wTFX_BLOATMP(std::vector<float>* naive, float paramvalue)
{
	float lValue = (-100.f + (paramvalue * 2.f)) * (100.f / 101.f);
	std::vector<float> inSamples = *naive;
	VASTWaveTableEditorComponent::calcBloat(inSamples, *naive, 0, C_WAVE_TABLE_SIZE - 1, lValue);
}

void CVASTWaveTable::wTFX_DIST(std::vector<float>* naive, float paramvalue)
{
	float lValue = paramvalue * (100.f / 101.f);
	std::vector<float> inSamples = *naive;
	inSamples[1024 - 1] = (inSamples[1024 - 2] + inSamples[1024 + 1]) * 0.5f; //calc middle median
	inSamples[1024] = (inSamples[1024 - 2] + inSamples[1024 + 1]) * 0.5f; //calc middle median
	VASTWaveTableEditorComponent::calcBloat(inSamples, *naive, 0, C_WAVE_TABLE_SIZE - 1, lValue);
}

void CVASTWaveTable::wTFX_HARDSYNC(std::vector<float>* naive, float paramvalue)
{
	float lsyncQuota = abs(paramvalue * 256.f);
	*naive = getSyncNaiveWave(*naive, lsyncQuota);
}

void CVASTWaveTable::wTFX_HARDSYNCWNDW(std::vector<float>* naive, float paramvalue)
{
	float lsyncQuota = abs(paramvalue * 256.f);
	*naive = getSyncNaiveWave(*naive, lsyncQuota);
	std::vector<float> inSamples = *naive;
	VASTWaveTableEditorComponent::calcWindow(inSamples, *naive, 0, C_WAVE_TABLE_SIZE - 1, 35.f);
}

void CVASTWaveTable::wTFX_HARDSYNCMIX(std::vector<float>* naive, float paramvalue)
{
	float lsyncQuota = abs(paramvalue * 256.f);
	std::vector<float> inSamples = *naive;
	*naive = getSyncNaiveWave(*naive, lsyncQuota);
	FloatVectorOperations::add(&(*naive)[0], &(inSamples)[0], C_WAVE_TABLE_SIZE);
	juce::Range<float> rng = FloatVectorOperations::findMinAndMax(&(*naive)[0], C_WAVE_TABLE_SIZE);
	float fMax = abs(rng.getStart()) > abs(rng.getEnd()) ? abs(rng.getStart()) : abs(rng.getEnd());
	float fFactor = (fMax == 0.f) ? 0.f : 1.f / fMax;
	FloatVectorOperations::multiply(&(*naive)[0], fFactor, C_WAVE_TABLE_SIZE);
}

void CVASTWaveTable::wTFX_PWM(std::vector<float>* naive, float paramvalue)
{
	float lPWMQuota = abs(paramvalue * 0.01f * 250.f);
	*naive = getPWMNaiveWave(*naive, lPWMQuota);
}

void CVASTWaveTable::wTFX_FM(std::vector<float>* naive, float paramvalue)
{
	float sinPhaseInc = paramvalue * 0.01f * 16.f;
	std::vector<float> inSamples = *naive;
	VASTWaveTableEditorComponent::frequencyModulate(inSamples, *naive, 0, C_WAVE_TABLE_SIZE - 1, sinPhaseInc);
}

void CVASTWaveTable::wTFX_FMWNDW(std::vector<float>* naive, float paramvalue)
{
	float sinPhaseInc = paramvalue * 0.01f * 16.f;
	std::vector<float> inSamples = *naive;
	VASTWaveTableEditorComponent::frequencyModulate(inSamples, *naive, 0, C_WAVE_TABLE_SIZE - 1, sinPhaseInc);
	inSamples = *naive;
	VASTWaveTableEditorComponent::calcWindow(inSamples, *naive, 0, C_WAVE_TABLE_SIZE - 1, 35.f);
}

void CVASTWaveTable::wTFX_FMMIX(std::vector<float>* naive, float paramvalue)
{
	float sinPhaseInc = paramvalue * 0.01f * 16.f;
	std::vector<float> inSamples = *naive;
	VASTWaveTableEditorComponent::frequencyModulate(inSamples, *naive, 0, C_WAVE_TABLE_SIZE - 1, sinPhaseInc);
	FloatVectorOperations::add(&(*naive)[0], &(inSamples)[0], C_WAVE_TABLE_SIZE); 
	juce::Range<float> rng = FloatVectorOperations::findMinAndMax(&(*naive)[0], C_WAVE_TABLE_SIZE);
	float fMax = abs(rng.getStart()) > abs(rng.getEnd()) ? abs(rng.getStart()) : abs(rng.getEnd());
	float fFactor = (fMax == 0.f) ? 0.f : 1.f / fMax;
	FloatVectorOperations::multiply(&(*naive)[0], fFactor, C_WAVE_TABLE_SIZE);
}

void CVASTWaveTable::wTFX_FLIP(std::vector<float>* naive, float paramvalue)
{
	std::vector<float> inSamples = *naive;
	VASTWaveTableEditorComponent::calcFlip(inSamples, *naive, 0, C_WAVE_TABLE_SIZE - 1, paramvalue);
}

void CVASTWaveTable::wTFX_MIRROR(std::vector<float>* naive, float paramvalue)
{
	std::vector<float> inSamples = *naive;
	VASTWaveTableEditorComponent::calcMirror(inSamples, *naive, 0, C_WAVE_TABLE_SIZE - 1, paramvalue);
}

void CVASTWaveTable::wTFX_BREED(std::vector<float>* naive, float paramvalue)
{
	std::vector<float> inSamples = *naive;
	VASTWaveTableEditorComponent::calcBreed(inSamples, *naive, 0, C_WAVE_TABLE_SIZE - 1, paramvalue);
}

void CVASTWaveTable::wTFX_BREEDWNDW(std::vector<float>* naive, float paramvalue)
{
	std::vector<float> inSamples = *naive;
	VASTWaveTableEditorComponent::calcBreed(inSamples, *naive, 0, C_WAVE_TABLE_SIZE - 1, paramvalue);
	inSamples = *naive;
	VASTWaveTableEditorComponent::calcWindow(inSamples, *naive, 0, C_WAVE_TABLE_SIZE - 1, 35.f);
}

void CVASTWaveTable::wTFX_BREEDMIX(std::vector<float>* naive, float paramvalue)
{
	std::vector<float> inSamples = *naive;
	VASTWaveTableEditorComponent::calcBreed(inSamples, *naive, 0, C_WAVE_TABLE_SIZE - 1, paramvalue);
	FloatVectorOperations::add(&(*naive)[0], &(inSamples)[0], C_WAVE_TABLE_SIZE);
	juce::Range<float> rng = FloatVectorOperations::findMinAndMax(&(*naive)[0], C_WAVE_TABLE_SIZE);
	float fMax = abs(rng.getStart()) > abs(rng.getEnd()) ? abs(rng.getStart()) : abs(rng.getEnd());
	float fFactor = (fMax == 0.f) ? 0.f : 1.f / fMax;
	FloatVectorOperations::multiply(&(*naive)[0], fFactor, C_WAVE_TABLE_SIZE);
}

void CVASTWaveTable::wTFX_BREW(std::vector<float>* naive, float paramvalue)
{
	std::vector<float> inSamples = *naive;
	VASTWaveTableEditorComponent::calcBrew(inSamples, *naive, 0, C_WAVE_TABLE_SIZE - 1, paramvalue);
}

void CVASTWaveTable::wTFX_BREWWNDW(std::vector<float>* naive, float paramvalue)
{
	std::vector<float> inSamples = *naive;
	VASTWaveTableEditorComponent::calcBrew(inSamples, *naive, 0, C_WAVE_TABLE_SIZE - 1, paramvalue);
	inSamples = *naive;
	VASTWaveTableEditorComponent::calcWindow(inSamples, *naive, 0, C_WAVE_TABLE_SIZE - 1, 35.f);
}


void CVASTWaveTable::wTFX_BREWMIX(std::vector<float>* naive, float paramvalue)
{
	std::vector<float> inSamples = *naive;
	VASTWaveTableEditorComponent::calcBrew(inSamples, *naive, 0, C_WAVE_TABLE_SIZE - 1, paramvalue);
	FloatVectorOperations::add(&(*naive)[0], &(inSamples)[0], C_WAVE_TABLE_SIZE);
	juce::Range<float> rng = FloatVectorOperations::findMinAndMax(&(*naive)[0], C_WAVE_TABLE_SIZE);
	float fMax = abs(rng.getStart()) > abs(rng.getEnd()) ? abs(rng.getStart()) : abs(rng.getEnd());
	float fFactor = (fMax == 0.f) ? 0.f : 1.f / fMax;
	FloatVectorOperations::multiply(&(*naive)[0], fFactor, C_WAVE_TABLE_SIZE);
}

void CVASTWaveTable::wTFX_SINCIFY(std::vector<float>* naive, float paramvalue)
{
	std::vector<float> inSamples = *naive;
	VASTWaveTableEditorComponent::calcSincify(inSamples, *naive, 0, C_WAVE_TABLE_SIZE - 1, paramvalue);
}

void CVASTWaveTable::wTFX_SINCIFYWNDW(std::vector<float>* naive, float paramvalue)
{
	std::vector<float> inSamples = *naive;
	VASTWaveTableEditorComponent::calcSincify(inSamples, *naive, 0, C_WAVE_TABLE_SIZE - 1, paramvalue);
	inSamples = *naive;
	VASTWaveTableEditorComponent::calcWindow(inSamples, *naive, 0, C_WAVE_TABLE_SIZE - 1, 35.f);
}

void CVASTWaveTable::wTFX_SINCIFYMIX(std::vector<float>* naive, float paramvalue)
{
	std::vector<float> inSamples = *naive;
	VASTWaveTableEditorComponent::calcSincify(inSamples, *naive, 0, C_WAVE_TABLE_SIZE - 1, paramvalue);
	FloatVectorOperations::add(&(*naive)[0], &(inSamples)[0], C_WAVE_TABLE_SIZE);
	juce::Range<float> rng = FloatVectorOperations::findMinAndMax(&(*naive)[0], C_WAVE_TABLE_SIZE);
	float fMax = abs(rng.getStart()) > abs(rng.getEnd()) ? abs(rng.getStart()) : abs(rng.getEnd());
	float fFactor = (fMax == 0.f) ? 0.f : 1.f / fMax;
	FloatVectorOperations::multiply(&(*naive)[0], fFactor, C_WAVE_TABLE_SIZE);
}

void CVASTWaveTable::wTFX_CRUSH(std::vector<float>* naive, float paramvalue)
{
	float pval = 1.f + 7.f * ((100.f - paramvalue) / 100.f);
	int intpart = pval;
	float fracpart = pval - intpart;

	std::vector<float> inSamples = *naive;
	for (int i = 0; i < C_WAVE_TABLE_SIZE; i++) {		
		int n1 = pow(2, intpart) - 1;
		int n2 = pow(2, intpart + 1) - 1;
		double j1 = 1.0 / n1;
		double j2 = 1.0 / n2;
		double o1 = (inSamples[i] + 1.0) / 2.0;
		double o2 = (inSamples[i] + 1.0) / 2.0;
		o1 = round(o1 * n1) * j1;
		o2 = round(o2 * n2) * j2;
		(*naive)[i] = (1.f - fracpart) * (2 * o1 - 1) + fracpart * (2 * o2 - 1);
	}
}

void CVASTWaveTable::wTFX_SMOOTH(std::vector<float>* naive, float paramvalue)
{
	std::vector<float> inSamples = *naive;
	float e = paramvalue / 5.f; //no zero values
	int r = 0;
	float s = 2 * e + 1;
	for (int i = 0; i < C_WAVE_TABLE_SIZE; i++) {
		float o = 0;
		for (int n = 0; n < s; n++) {
			r = int(i - e * 4 + 4 * n + C_WAVE_TABLE_SIZE) % C_WAVE_TABLE_SIZE;
			o += inSamples[r];
		}
		(*naive)[i] = o / s;
	}
	juce::Range<float> rng = FloatVectorOperations::findMinAndMax(&(*naive)[0], C_WAVE_TABLE_SIZE);
	float fMax = abs(rng.getStart()) > abs(rng.getEnd()) ? abs(rng.getStart()) : abs(rng.getEnd());
	FloatVectorOperations::multiply(&(*naive)[0], 1.f / fMax, C_WAVE_TABLE_SIZE);
}

void CVASTWaveTable::wTFX_WINDOW(std::vector<float>* naive, float paramvalue)
{
	std::vector<float> inSamples = *naive;
	VASTWaveTableEditorComponent::calcWindow(inSamples, *naive, 0, C_WAVE_TABLE_SIZE - 1, paramvalue);
}

CVASTWaveTable::WTFXFuncPtr CVASTWaveTable::getWTFXFuncPtr(int type)
{
	switch (type)
	{
	case 0:
		return &CVASTWaveTable::wTFX_PASS;
	case 1:
		return &CVASTWaveTable::wTFX_BENDP;
	case 2:
		return &CVASTWaveTable::wTFX_BENDM;
	case 3:
		return &CVASTWaveTable::wTFX_BENDMP;
	case 4:
		return &CVASTWaveTable::wTFX_BLOATP;
	case 5:
		return &CVASTWaveTable::wTFX_BLOATM;
	case 6:
		return &CVASTWaveTable::wTFX_BLOATMP;
	case 7:
		return &CVASTWaveTable::wTFX_DIST;
	case 8:
		return &CVASTWaveTable::wTFX_HARDSYNC;
	case 9:
		return &CVASTWaveTable::wTFX_HARDSYNCWNDW;
	case 10:
		return &CVASTWaveTable::wTFX_HARDSYNCMIX;
	case 11:
		return &CVASTWaveTable::wTFX_PWM;
	case 12:
		return &CVASTWaveTable::wTFX_FM;
	case 13:
		return &CVASTWaveTable::wTFX_FMWNDW;
	case 14:
		return &CVASTWaveTable::wTFX_FMMIX;
	case 15:
		return &CVASTWaveTable::wTFX_FLIP;
	case 16:
		return &CVASTWaveTable::wTFX_MIRROR;
	case 17:
		return &CVASTWaveTable::wTFX_BREED;
	case 18:
		return &CVASTWaveTable::wTFX_BREEDWNDW;
	case 19:
		return &CVASTWaveTable::wTFX_BREEDMIX;
	case 20:
		return &CVASTWaveTable::wTFX_BREW;	
	case 21:
		return &CVASTWaveTable::wTFX_BREWWNDW;
	case 22:
		return &CVASTWaveTable::wTFX_BREWMIX;
	case 23:
		return &CVASTWaveTable::wTFX_SINCIFY;
	case 24:
		return &CVASTWaveTable::wTFX_SINCIFYWNDW;
	case 25:
		return &CVASTWaveTable::wTFX_SINCIFYMIX;
	case 26:
		return &CVASTWaveTable::wTFX_CRUSH;
	case 27:
		return &CVASTWaveTable::wTFX_SMOOTH;
	case 28:
		return &CVASTWaveTable::wTFX_WINDOW;
	case C_WTFXTYPE_FM:
		return &CVASTWaveTable::wTFX_PASS; //SHOULD ONLY ADD AT END OF LIST!!
	}
	vassertfalse;
	return 0;
}

bool CVASTWaveTable::wtFreqCheckForChange(sWaveTableFreq &wtFreq, float wtFxVal, int wtFxType) {
	int lwtFxType = wtFxType;
	if (wtFxType == C_WTFXTYPE_FM) lwtFxType = 0;
	if (!(approximatelyEqual(wtFreq.wtFxVal, wtFxVal)))
		return true;
	if (wtFreq.wtFxType != lwtFxType)
		return true;
	if (wtFreq.dirty)
		return true;
	return false;
}

int CVASTWaveTable::getID() const {
	return m_iWaveTableID;
}

int CVASTWaveTable::getChangeCounter() const {
	return wtheader.changeCounter.load();
}

void CVASTWaveTable::copyUIFXUpdates() {
	//ScopedLock sl(mWavetableChangeLock); //CHECK if really needed
	for (int i = 0; i < getNumPositions(); i++) {
		if (wtheader.waveTablePositions[i].naiveTableFXDirty) {
			m_isBeingUpdated.store(true);
			wtheader.waveTablePositions[i].naiveTableFXDisplayCopy = wtheader.waveTablePositions[i].naiveTableFX;
			wtheader.waveTablePositions[i].naiveTableFXDirty = false;
			m_isBeingUpdated.store(false);
		}
	}
}

std::vector<float>* CVASTWaveTable::getNaiveTableWithFXForDisplay(int wtPos, int wtFXType, float wtFXVal, bool forceRecalc) {
	if (wtPos >= wtheader.waveTablePositions.size()) {
		vassertfalse; //should not happen!
		return &wtheader.waveTablePositions[0].naiveTable; //CHECK
		//return &std::vector<float>(C_WAVE_TABLE_SIZE);
	}
	else {
		if ((wtheader.waveTablePositions[wtPos].hasFXTable) && !forceRecalc)
			return &wtheader.waveTablePositions[wtPos].naiveTableFXDisplayCopy; //check when FX are turned off!
		else
		{
			//calc ui display
			//calc fx if not from audio thread
			if ((wtFXType == 0) || (wtFXType == C_WTFXTYPE_FM)) 
				return &wtheader.waveTablePositions[wtPos].naiveTable;
			else {
				CVASTWaveTable::WTFXFuncPtr fxPtr = getWTFXFuncPtr(wtFXType);
				wtheader.waveTablePositions[wtPos].naiveTableFXDisplayCopy = wtheader.waveTablePositions[wtPos].naiveTable; //copy it, is it thread safe here?
				(this->*fxPtr)(&wtheader.waveTablePositions[wtPos].naiveTableFXDisplayCopy, wtFXVal);
				return &wtheader.waveTablePositions[wtPos].naiveTableFXDisplayCopy;
			}
		}
	}
}
std::vector<float>* CVASTWaveTable::getNaiveTable(int wtPos) {
	/*if (wtPos >= wtheader.waveTablePositions.size()) return std::vector<float>(C_WAVE_TABLE_SIZE);
	else
	return wtheader.waveTablePositions[wtPos].naiveTable;
	*/
	if (wtPos >= wtheader.waveTablePositions.size()) {
		vassertfalse; //should not happen!
		return &wtheader.waveTablePositions[0].naiveTable; //CHECK
	}
	else
		return &wtheader.waveTablePositions[wtPos].naiveTable;
}
;

// if scale is 0, auto-scales
// returns wavetable in ai array
// change = true updates existing table for dynamic generation
//void CVASTWaveTable::makeWaveTableFreq(int wtPos, int len, float topFreq, int maxHarmonics, std::vector<float> &naiveTable, bool change, int tableindex) {
void CVASTWaveTable::makeWaveTableFreq(int wtPos, int len, float bottomFreq, float topFreq, int maxHarmonics, bool change, int tableindex, bool normalize, int wtMode, float wtFxVal, int wtFxType, float curFreq) {
	//ScopedLock sl(mWavetableChangeLock); //trying not to lock here to save audio thread - it should be save as it is not changing the wtpos structure

	int lmaxHarmonics = maxHarmonics;
	sWaveTablePosition* wtp;
	if (!getWaveTablePosition(wtPos, wtp)) {
		vassertfalse;
		return; //error
	}
	float ltopFreq = topFreq;
	// fill the table in with the needed harmonics
	// for subsquent tables, float topFreq and remove upper half of harmonics
	std::vector<float> ar(len);	//is this real amplitude and ai imaginary amplitude?
	std::vector<float> ai(len);

	std::vector<dsp::Complex<float>> timeBuffer = std::vector<dsp::Complex<float>>(C_WAVE_TABLE_SIZE, 0.0f);
	std::vector<dsp::Complex<float>> outBuffer = std::vector<dsp::Complex<float>>(C_WAVE_TABLE_SIZE, 0.0f);
	
	//***********************************************************
	std::vector<dsp::Complex<float>>* fdBuffer = &wtp->frequencyDomainBuffer;
	int lwtFxType = wtFxType;
	if (lwtFxType == C_WTFXTYPE_FM)
		lwtFxType = 0;
	if (lwtFxType > 0) {
		wtp->naiveTableFX = wtp->naiveTable; //copy it
		WTFXFuncPtr fxPtr = getWTFXFuncPtr(wtFxType);
		(this->*fxPtr)(&wtp->naiveTableFX, wtFxVal);
		wtp->naiveTableFXDirty = true;
		wtp->hasFXTable = true;
		fdBuffer = &mc_newBuffer;
		frequencyDomainBufferFromNaive(C_WAVE_TABLE_SIZE, wtp->naiveTableFX, *fdBuffer);

		/*
		int nmaxHarmonics = C_WAVE_TABLE_SIZE >> 1;
		const float minVal = 0.000001; // -120 dB
		while ((fabs((*fdBuffer)[nmaxHarmonics].real()) + fabs((*fdBuffer)[nmaxHarmonics].imag()) < minVal) && nmaxHarmonics) {
			--nmaxHarmonics;
		}
		lmaxHarmonics = nmaxHarmonics;
		*/

		ltopFreq = curFreq * 2.f;
		if (ltopFreq < bottomFreq)
			ltopFreq = topFreq;
		lmaxHarmonics = 1.0 / ltopFreq;
		if (lmaxHarmonics > C_WAVE_TABLE_SIZE >> 1)
			lmaxHarmonics = C_WAVE_TABLE_SIZE >> 1;
		vassert(lmaxHarmonics >= 0);
	}
	else
		wtp->hasFXTable = false;
	//***********************************************************

	//Idea: two different sound characteristics with not just cutting off beyond maxharmonics but also LPFing
	if (wtMode == 0) { //sharp -see Gibbs effect https://dsp.stackexchange.com/questions/6220/why-is-it-a-bad-idea-to-filter-by-zeroing-out-fft-bins?fbclid=IwAR1Mocwr3SYdFP5-D4ePk4LCZHkoCOAQCY6n0sNqjZ92rbSTTMka7nN7zd4
		for (int idx = 1; idx <= lmaxHarmonics; idx++) {
			timeBuffer[idx].real((*fdBuffer)[idx].real());
			timeBuffer[idx].imag((*fdBuffer)[idx].imag());
			timeBuffer[len - idx].real((*fdBuffer)[len - idx].real());
			timeBuffer[len - idx].imag((*fdBuffer)[len - idx].imag());
		}
	}
	else if (wtMode == 1) { //soft
		if (lmaxHarmonics > 10)
			lmaxHarmonics--;
		float fadePercent = 0.7f;
		//float breakH = (lmaxHarmonics * (fadePercent + 0.05f)) * 0.5f;
		float breakH = (lmaxHarmonics * fadePercent) * 0.5f;
		for (int idx = 1; idx <= lmaxHarmonics; idx++) {
			float amount = 1.0f;
			if (idx > (lmaxHarmonics * (1.f - fadePercent))) {
				int newidx = idx - (lmaxHarmonics * (1.f - fadePercent));
				amount = 1.0f / (1.f + powf(float(newidx) / breakH, 2));
				//amount = 1.0f / (1.f + (float(newidx) / breakH));
			}
			//VDBG("lmaxharmonics " + String(lmaxHarmonics) + " idx " + String(idx) + " amount " + String(amount));
			timeBuffer[idx].real((*fdBuffer)[idx].real() * amount);
			timeBuffer[idx].imag((*fdBuffer)[idx].imag() * amount);
			timeBuffer[len - idx].real((*fdBuffer)[len - idx].real() * amount);
			timeBuffer[len - idx].imag((*fdBuffer)[len - idx].imag() * amount);
		}
	}
	else if (wtMode == 2) { //dull - linear
		for (int idx = 1; idx <= lmaxHarmonics; idx++) {
			float amount = 1.0f - float(idx - 1) / float(lmaxHarmonics);
			//VDBG("lmaxharmonics " + String(lmaxHarmonics) + " idx " + String(idx) + " amount " + String(amount));
			timeBuffer[idx].real((*fdBuffer)[idx].real() * amount);
			timeBuffer[idx].imag((*fdBuffer)[idx].imag() * amount);
			timeBuffer[len - idx].real((*fdBuffer)[len - idx].real() * amount);
			timeBuffer[len - idx].imag((*fdBuffer)[len - idx].imag() * amount);
		}
	}
	else
		vassertfalse;

	vassert(ar[0] == ar[0]);

	//========================================================================================================================================
	//
	{
#ifdef _DEBUG	
		std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
#endif
		//---------------------------------------------------------------------------------------
		// EXPENSIVE
		m_Set->fftSingletonFactory.performFFT(&*timeBuffer.begin(), &*outBuffer.begin(), false); //mufft forward

#ifdef _DEBUG	
		std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
		VDBG("fftw3 execute makeWaveTableFreq duration: " << duration);
#endif
	}
	//
	//========================================================================================================================================

	/*
	float scale = 0.0f;
	//if no scale was supplied, find maximum sample amplitude, then derive scale
	float max = 0;
	for (int idx = 0; idx < len; idx++) {
		//float temp = fabs(ai[idx]);
		float temp = fabs(timeBuffer[idx].imag());
		if (max < temp)
			max = temp;
	}
	scale = 1.0 / max * .999;

	if (normalize) {

	}
	else {
		if (scale > (1.0f / len)) {
			scale = 1.0f / len;
		}
		//https://www.reddit.com/r/math/comments/3b95up/what_is_the_maximum_value_of_intensity_that_a_fft/
	}
	*/
	// normalize

	std::vector<float> fftwave(len);
	for (int idx = 0; idx < len; idx++) {
		//fftwave[idx] = ai[len - idx - 1] * scale;
		//fftwave[idx] = timeBuffer[len - idx - 1].imag() * scale;
		fftwave[idx] = outBuffer[idx].imag();
	}

	// check valid
	bool isInvalid = false;
	if (isnan(fftwave[0])) {//NaN check
		isInvalid = true;
	}

	vassert(fftwave.size() > 0);

	if (change == false) { //set generate wavetable flag
		addWaveTableFreq(wtPos, len, fftwave, bottomFreq, ltopFreq, maxHarmonics, isInvalid, false, wtFxVal, lwtFxType, false, wtp->naiveTableFX);
	}
	else //tableindex must be provided
		changeWaveTableFreq(wtPos, tableindex, len, fftwave, bottomFreq, ltopFreq, maxHarmonics, isInvalid, false, wtFxVal, lwtFxType);
	wtheader.changeCounter+=1;

	/*
	#ifdef _DEBUG
		//bug catcher
		float max = *max_element(wtp->waveTableFreqs[tableindex].waveTableSamples.begin(), wtp->waveTableFreqs[tableindex].waveTableSamples.end());
		if (max > 5.f)
			VDBG("Overflow!!!!");
	#endif
	*/
}

//
// addWaveTable
//
// add wavetables in order of lowest frequency to highest
// topFreq is the highest frequency supported by a wavetable
// wavetables within an oscillator can be different lengths
//
//void CVASTWaveTable::addWaveTableFreq(int wtPos, int len, std::vector<float> waveTableIn, float topFreq, int maxHarmonics, bool invalid, bool dirty, std::vector<float> naiveWave) {
void CVASTWaveTable::addWaveTableFreq(int wtPos, int len, std::vector<float> &waveTableIn, float bottomFreq, float topFreq, int maxHarmonics, bool invalid, bool dirty, float wtFxVal, int wtFxType, bool buffer, std::vector<float> &naiveTableFXBufferCopy) {
	//ScopedLock sl(mWavetableChangeLock); //trying not to lock here to save audio thread - it should be save as it is not changing the wtpos structure

	sWaveTablePosition* wtp;
	bool b_new = true;
    if (!getWaveTablePosition(wtPos, wtp)) {
        vassertfalse;
        return; //error case
    }
	vassert(wtp->numWaveTableFreqsBuffer == wtp->waveTableFreqsBuffer.size());

	sWaveTableFreq wtf;
	if (!buffer) {
		int wtsize = int(wtp->waveTableFreqs.size());
		int i = 0;
		while ((i < wtsize) && (b_new)) {
			if (wtp->waveTableFreqs[i].invalid == true) { // reassign first invalid
				wtp->waveTableFreqs[i].waveTableSamples = waveTableIn;
				wtp->waveTableFreqs[i].waveTableLen = len;
				wtp->waveTableFreqs[i].bottomFreq = bottomFreq;
				wtp->waveTableFreqs[i].bottomFreqOrig = bottomFreq;
				wtp->waveTableFreqs[i].topFreq = topFreq;
				wtp->waveTableFreqs[i].topFreqOrig = topFreq;
				wtp->waveTableFreqs[i].maxHarmonics = maxHarmonics;
				wtp->waveTableFreqs[i].invalid = invalid;
				wtp->waveTableFreqs[i].dirty = dirty;
				wtp->waveTableFreqs[i].wtFxVal = wtFxVal;
				wtp->waveTableFreqs[i].wtFxType = wtFxType;
				wtp->waveTableFreqs[i].isBuffer = buffer;
				wtp->waveTableFreqs[i].naiveTableFXBufferCopy = naiveTableFXBufferCopy;
				b_new = false;
			}
			i++;
		}
	}

	if (b_new) {
		wtf.waveTableSamples = waveTableIn;
		wtf.waveTableLen = len;
		wtf.bottomFreq = bottomFreq;
		wtf.topFreq = topFreq;
		wtf.bottomFreqOrig = bottomFreq;
		wtf.topFreqOrig = topFreq;
		wtf.maxHarmonics = maxHarmonics;
		wtf.invalid = invalid;
		wtf.dirty = dirty;
		wtf.wtFxVal = wtFxVal;
		wtf.wtFxType = wtFxType;
		wtf.isBuffer = buffer;
		wtf.naiveTableFXBufferCopy = naiveTableFXBufferCopy;
	}

	if ((b_new) && (wtp->numWaveTableFreqs < C_MAX_NUM_FREQS)) {
		if (!buffer) {
			++wtp->numWaveTableFreqs;
			wtp->waveTableFreqs.push_back(wtf);
		}
		else {
			++wtp->numWaveTableFreqsBuffer;
#ifdef _DEBUG
			if (wtp->waveTableFreqsBuffer.capacity() < wtp->waveTableFreqsBuffer.size() + 1)
				VDBG("wtp->waveTableFreqsBuffer.capacity() < wtp->waveTableFreqsBuffer.size() + 1 resizing");
#endif
			wtp->waveTableFreqsBuffer.push_back(wtf);
		}
		vassert(wtp->numWaveTableFreqs <= C_MAX_NUM_FREQS);
		//vassert(wtp->numWaveTableFreqs == wtp->waveTableFreqs.size()); //not right due to invalids
	}
	else {
		if (!invalid) ++wtp->numWaveTableFreqs;
		vassert(wtp->numWaveTableFreqs <= C_MAX_NUM_FREQS);
		//vassert(wtp->numWaveTableFreqs == wtp->waveTableFreqs.size()); //not right due to invalids
	}
	wtheader.changeCounter+=1;
	vassert(wtp->numWaveTableFreqsBuffer == wtp->waveTableFreqsBuffer.size());
}

void CVASTWaveTable::changeWaveTableFreq(int wtPos, int tableindex, int len, std::vector<float> &waveTableIn, float bottomFreq, float topFreq, int maxHarmonics, bool invalid, bool dirty, float wtFxVal, int wtFxType) {
	//ScopedLock sl(mWavetableChangeLock); //trying not to lock here to save audio thread - it should be save as it is not changing the wtpos structure

	sWaveTablePosition* wtp;
	bool ok = getWaveTablePosition(wtPos, wtp);
	if (!ok) {
		vassertfalse;
		return;
	}
	if (wtFxType == 0) {
		wtp->waveTableFreqs[tableindex].bottomFreq = wtp->waveTableFreqs[tableindex].bottomFreqOrig;
		wtp->waveTableFreqs[tableindex].topFreq = wtp->waveTableFreqs[tableindex].topFreqOrig;
	}
	else {
		wtp->waveTableFreqs[tableindex].bottomFreq = bottomFreq;
		wtp->waveTableFreqs[tableindex].topFreq = topFreq;
	}

	wtp->waveTableFreqs[tableindex].waveTableSamples = waveTableIn;
	wtp->waveTableFreqs[tableindex].waveTableLen = len;
	wtp->waveTableFreqs[tableindex].maxHarmonics = maxHarmonics;
	wtp->waveTableFreqs[tableindex].invalid = invalid;
	wtp->waveTableFreqs[tableindex].dirty = dirty;
	wtp->waveTableFreqs[tableindex].wtFxVal = wtFxVal;
	wtp->waveTableFreqs[tableindex].wtFxType = wtFxType;
	//wtp->waveTableFreqs[tableindex].isBuffer = buffer;
	//wtp->naiveTable = naiveWave;
	wtheader.changeCounter+=1;
}

void CVASTWaveTable::prepareForPlay(int expectedSamplesPerBlock) {
	bool releaseMemory = true;
	m_x0_curPos->setSize(1, expectedSamplesPerBlock, false, false, !releaseMemory); //keep symmetry
	m_y0_curPos->setSize(1, expectedSamplesPerBlock, false, false, !releaseMemory);
	m_y1_curPos->setSize(1, expectedSamplesPerBlock, false, false, !releaseMemory);
	m_x0_nextPos->setSize(1, expectedSamplesPerBlock, false, false, !releaseMemory);
	m_y0_nextPos->setSize(1, expectedSamplesPerBlock, false, false, !releaseMemory);
	m_y1_nextPos->setSize(1, expectedSamplesPerBlock, false, false, !releaseMemory);

	m_begin_nextPosBuffer->setSize(2, expectedSamplesPerBlock, false, false, !releaseMemory); //stereo!
	m_begin_nextPosBufferNextWtfxtype->setSize(2, expectedSamplesPerBlock, false, false, !releaseMemory); //stereo!
	m_begin_curPosBuffer->setSize(2, expectedSamplesPerBlock, false, false, !releaseMemory); //stereo!
	m_begin_curPosBufferNextWtfxtype->setSize(2, expectedSamplesPerBlock, false, false, !releaseMemory); //stereo!
	m_end_nextPosBuffer->setSize(2, expectedSamplesPerBlock, false, false, !releaseMemory); //stereo!
	m_end_nextPosBufferNextWtfxtype->setSize(2, expectedSamplesPerBlock, false, false, !releaseMemory); //stereo!
	m_end_curPosBuffer->setSize(2, expectedSamplesPerBlock, false, false, !releaseMemory); //stereo!
	m_end_curPosBufferNextWtfxtype->setSize(2, expectedSamplesPerBlock, false, false, !releaseMemory); //stereo! 

	m_valBeginBuffer->setSize(2, expectedSamplesPerBlock, false, false, !releaseMemory); //to keep startSample symmetrically
	m_valBeginBufferNext->setSize(2, expectedSamplesPerBlock, false, false, !releaseMemory); //to keep startSample symmetrically
	m_valEndBuffer->setSize(2, expectedSamplesPerBlock, false, false, !releaseMemory); //to keep startSample symmetrically
	m_valEndBufferNext->setSize(2, expectedSamplesPerBlock, false, false, !releaseMemory); //to keep startSample symmetrically

	//m_interpolBuffer->setSize(1, expectedSamplesPerBlock, false, false, !releaseMemory);
	m_fracPartBuffer->setSize(1, expectedSamplesPerBlock, false, false, !releaseMemory);
	//m_fracPartQuad->setSize(1, expectedSamplesPerBlock, false, false, !releaseMemory);	
}

void CVASTWaveTable::putWaveTableFreqToBuffer(sWaveTablePosition* wtp, int tableindex) {
	vassert(wtp->waveTableFreqs[tableindex].waveTableSamples.size() == C_WAVE_TABLE_SIZE);
	VDBG("putWaveTableFreqToBuffer wtPos " << wtp->wtPos << " wtFreq " << tableindex << " current #freqbuffer " << wtp->waveTableFreqsBuffer.size());
	if (wtp->numWaveTableFreqsBuffer >= C_MAX_NUM_FREQS) {
		//delete old stuff
		ScopedLock sl(mWavetableChangeLock);		
		VDBG("putWaveTableFreqToBuffer delete old stuff");
		wtp->waveTableFreqsBuffer.clear(); 
		wtp->numWaveTableFreqsBuffer = 0;
	}
	addWaveTableFreq(wtp->wtPos, wtp->waveTableFreqs[tableindex].waveTableLen, wtp->waveTableFreqs[tableindex].waveTableSamples, wtp->waveTableFreqs[tableindex].bottomFreq, wtp->waveTableFreqs[tableindex].topFreq, wtp->waveTableFreqs[tableindex].maxHarmonics,
		false, false, wtp->waveTableFreqs[tableindex].wtFxVal, wtp->waveTableFreqs[tableindex].wtFxType, true, wtp->naiveTableFX); //buffer
	vassert(wtp->waveTableFreqsBuffer[wtp->numWaveTableFreqsBuffer - 1].waveTableSamples.size() == C_WAVE_TABLE_SIZE);
}

bool CVASTWaveTable::getWavetableInterpolateBuffer(CVASTWaveTableOscillator* mOscillator, const int osciCount, AudioSampleBuffer* buffer, const int startSample, const int numSamples, const bool bInverter, int wtMode, int voiceNo, int bank, OwnedArray<CVASTOscillatorBank>* oscBank, bool isFirstCycle, bool isStartOfCycle, float wtPosPercTarget, float wtFxVal, int wtFxType, int lastCycleSamples, int curCycleSamples, const CVASTWaveTable* masterWTForSync, bool &updated, bool soloMode) {
	//take from last
	float wtFxVal_begin = m_fLastWtFXVal_begin[voiceNo];
	int wtFxType_begin = m_iLastWtFXType_begin[voiceNo];
	float wtPos_begin = m_fLastWTPosPerc_begin[voiceNo] * (getNumPositions() - 1);
	float lWtPosPerc_begin = m_fLastWTPosPerc_begin[voiceNo];
	float wtFxVal_end = m_fLastWtFXVal_end[voiceNo];
	int wtFxType_end = m_iLastWtFXType_end[voiceNo];
	float wtPos_end = m_fLastWTPosPerc_end[voiceNo] * (getNumPositions() - 1);
	float lWtPosPerc_end = m_fLastWTPosPerc_end[voiceNo];

	if (isStartOfCycle || isFirstCycle || (masterWTForSync != nullptr)) { //next zero crossing - start of cycle? or softfade!
		if (masterWTForSync != nullptr) {
			//softfade - take from master wt
			wtFxVal_begin = masterWTForSync->m_fLastWtFXVal_begin[voiceNo]; //begin! was processed already
			wtFxType_begin = masterWTForSync->m_iLastWtFXType_begin[voiceNo]; //begin! was processed already
			//wtPos_begin = masterWTForSync->m_fLastWTPosPerc_begin[voiceNo] * (masterWTForSync->wtheader.numPositions.load()- 1); //begin! was processed already, has to be num positions of master as it can be more or less!
			wtPos_begin = masterWTForSync->m_fLastWTPosPerc_begin[voiceNo] * (getNumPositions() - 1);
			lWtPosPerc_begin = masterWTForSync->m_fLastWTPosPerc_begin[voiceNo];
			wtFxVal_end = masterWTForSync->m_fLastWtFXVal_end[voiceNo];
			wtFxType_end = masterWTForSync->m_iLastWtFXType_end[voiceNo];
			wtPos_end = masterWTForSync->m_fLastWTPosPerc_end[voiceNo] * (getNumPositions() - 1);
			lWtPosPerc_end = masterWTForSync->m_fLastWTPosPerc_end[voiceNo];
			//if (getNumPositions() != masterWTForSync->wtheader.numPositions.load())
				//VDBG("Softfade with different wtpos!");
		}
		else {
			wtFxVal_end = wtFxVal;
			wtFxType_end = wtFxType;
			if (soloMode) {
				if (m_multiSelect)
					wtPos_end = m_iMultiSelectBegin + wtPosPercTarget * (m_iMultiSelectEnd - m_iMultiSelectBegin);
				else
					wtPos_end = m_iSelectedPosition;
				if ((getNumPositions() - 1) > 0)
					lWtPosPerc_end = wtPos_end / (getNumPositions() - 1);
				else
					lWtPosPerc_end = 0.f; //CHECK
			}
			else {
				wtPos_end = wtPosPercTarget * (getNumPositions() - 1);
				lWtPosPerc_end = wtPosPercTarget;
			}			
			if (isFirstCycle) {
				//on note on set start = end
				wtFxVal_begin = wtFxVal_end;
				wtFxType_begin = wtFxType_end;
				wtPos_begin = wtPos_end;
				lWtPosPerc_begin = lWtPosPerc_end;
			}
			else {
				//new zero crossing, take begin from last end
				wtFxVal_begin = m_fLastWtFXVal_end[voiceNo];
				wtFxType_begin = m_iLastWtFXType_end[voiceNo];
				wtPos_begin = m_fLastWTPosPerc_end[voiceNo] * (getNumPositions() - 1);
				lWtPosPerc_begin = m_fLastWTPosPerc_end[voiceNo];
			}
		}

		//store changes
		m_fLastWtFXVal_begin[voiceNo] = wtFxVal_begin;
		m_iLastWtFXType_begin[voiceNo] = wtFxType_begin;
		m_fLastWTPosPerc_begin[voiceNo] = lWtPosPerc_begin;
		m_fLastWtFXVal_end[voiceNo] = wtFxVal_end;
		m_iLastWtFXType_end[voiceNo] = wtFxType_end;
		m_fLastWTPosPerc_end[voiceNo] = lWtPosPerc_end;

		//VDBG("StartOfCycle curCycleSamples: " + String(curCycleSamples) + " startSample: " + String(startSample) + " numSamples: " + String(numSamples) + " using wtPos_begin: " + String(wtPos_begin) + " wtPos_end: " + String(wtPos_end));
	}
	//else {
		//VDBG("curCycleSamples: " + String(curCycleSamples) + " startSample: " + String(startSample) + " numSamples: " + String(numSamples) + " using wtPos_begin: " + String(wtPos_begin) + " wtPos_end: " + String(wtPos_end));
	//}

	sWaveTablePosition* wtpbegin = nullptr;
	sWaveTablePosition* wtpend = nullptr;
	sWaveTablePosition* wtpbeginnextpos = nullptr;
	sWaveTablePosition* wtpendnextpos = nullptr;
	bool releaseMemory = false;
	int bufferNumSamples = buffer->getNumSamples();
	m_x0_curPos->setSize(1, bufferNumSamples, false, false, !releaseMemory); //keep symmetry
	m_x0_curPos_writePointer = m_x0_curPos->getWritePointer(0);
	m_y0_curPos->setSize(1, bufferNumSamples, false, false, !releaseMemory);
	m_y0_curPos_writePointer = m_y0_curPos->getWritePointer(0);
	m_y1_curPos->setSize(1, bufferNumSamples, false, false, !releaseMemory);
	m_y1_curPos_writePointer = m_y1_curPos->getWritePointer(0);
	m_x0_nextPos->setSize(1, bufferNumSamples, false, false, !releaseMemory);
	m_x0_nextPos_writePointer = m_x0_nextPos->getWritePointer(0);
	m_y0_nextPos->setSize(1, bufferNumSamples, false, false, !releaseMemory);
	m_y0_nextPos_writePointer = m_y0_nextPos->getWritePointer(0);
	m_y1_nextPos->setSize(1, bufferNumSamples, false, false, !releaseMemory);
	m_y1_nextPos_writePointer = m_y1_nextPos->getWritePointer(0);
	m_oscBuffer->setSize(1, bufferNumSamples, false, false, !releaseMemory); //to keep startSample symmetrically
	//m_oscBufferStereo->setSize(2, bufferNumSamples, false, false, !releaseMemory); //to keep startSample symmetrically
	
	m_valBeginBuffer->setSize(2, bufferNumSamples, false, false, !releaseMemory); //to keep startSample symmetrically
	m_valBeginBufferNext->setSize(2, bufferNumSamples, false, false, !releaseMemory); //to keep startSample symmetrically
	m_valEndBuffer->setSize(2, bufferNumSamples, false, false, !releaseMemory); //to keep startSample symmetrically
	m_valEndBufferNext->setSize(2, bufferNumSamples, false, false, !releaseMemory); //to keep startSample symmetrically

	//m_interpolBuffer->setSize(1, bufferNumSamples, false, false, !releaseMemory);
	m_fracPartBuffer->setSize(1, bufferNumSamples, false, false, !releaseMemory);
	//m_fracPartQuad->setSize(1, bufferNumSamples, false, false, !releaseMemory);

	//VDBG("getWavetableInterpolateBuffer ID: " + String(getID()) + " bank " + String(bank) + " voiceNo " + String(voiceNo) + " startSample " + String(startSample) + " numSamples " + String(numSamples) + " first Phasor " + String(mOscillator->m_phasedPhasorBufferPointer[0][startSample]) + " wtPosStart " + String(wtPos_begin) + " wtPosEnd " + String(wtPos_end) + " lastCycleSamples " + String(lastCycleSamples) + " curCycleSamples " + String(curCycleSamples) + " wtFxVal_begin " + String(wtFxVal_begin) + " wtFxType_begin " + String(wtFxType_begin) + " wtFxVal_end " + String(wtFxVal_end) + " wtFxType_end " + String(wtFxType_end));

	bool hasNextWtfxtype = (wtFxVal_begin != wtFxVal_end) || (wtFxType_begin != wtFxType_end);
	bool wtPosint_begin_hasNextPos = false;
	bool wtPosint_end_hasNextPos = false;

	bool doPosFade = false;
	if (abs(wtPos_end - wtPos_begin) > 0.00000001)
		doPosFade = true;

	int wtPosint_begin = wtPos_begin;
	int wtPosint_end = wtPos_end;

	wtpbegin = &wtheader.waveTablePositions[wtPosint_begin];
	wtpend = &wtheader.waveTablePositions[wtPosint_end];

	//check dynamic dirty wtpos
	if (wtpbegin->dirty) 
		createFreqsIfNeeded(wtpbegin->wtPos, false, wtMode);
	if (wtpend->dirty)
		createFreqsIfNeeded(wtpend->wtPos, false, wtMode);

	if (wtPosint_begin < (getNumPositions() - 1)) {
		wtPosint_begin_hasNextPos = true;
		wtpbeginnextpos = &wtheader.waveTablePositions[wtPosint_begin + 1];
		if (wtpbeginnextpos->dirty) {
			jassert(wtpbeginnextpos->wtPos == wtPosint_begin + 1);
			createFreqsIfNeeded(wtpbeginnextpos->wtPos, false, wtMode);
		}
	}
	if (wtPosint_end < (getNumPositions() - 1)) {
		wtPosint_end_hasNextPos = true;
		wtpendnextpos = &wtheader.waveTablePositions[wtPosint_end + 1];
		if (wtpendnextpos->dirty) {
			jassert(wtpendnextpos->wtPos == wtPosint_end + 1);
			createFreqsIfNeeded(wtpendnextpos->wtPos, false, wtMode);
		}
	}

	bool doExit = false;

	// =====================================================================================================================
	// begin position	
	m_begin_curPosBuffer->setSize(2, bufferNumSamples, false, false, !releaseMemory); //stereo!
	m_begin_curPosBuffer->clear(startSample, numSamples);
	bool ok = fillInterpolateBuffersRange(false, wtpbegin, m_x0_curPos_writePointer, m_y0_curPos_writePointer, m_y1_curPos_writePointer, mOscillator, osciCount, startSample, numSamples, m_begin_curPosBuffer.get(), bInverter, wtMode, wtFxVal_begin, wtFxType_begin, voiceNo, bank, oscBank, !doPosFade && !hasNextWtfxtype && !wtPosint_begin_hasNextPos, updated);
	if (!ok) doExit = true;

	if (hasNextWtfxtype) {
		m_begin_curPosBufferNextWtfxtype->setSize(2, bufferNumSamples, false, false, !releaseMemory); //stereo!
		m_begin_curPosBufferNextWtfxtype->clear(startSample, numSamples);
		bool ok = fillInterpolateBuffersRange(false, wtpbegin, m_x0_curPos_writePointer, m_y0_curPos_writePointer, m_y1_curPos_writePointer, mOscillator, osciCount, startSample, numSamples, m_begin_curPosBufferNextWtfxtype.get(), bInverter, wtMode, wtFxVal_end, wtFxType_end, voiceNo, bank, oscBank, !doPosFade && !wtPosint_begin_hasNextPos, updated);
		if (!ok) doExit = true;
	}

	// =====================================================================================================================
	// begin next position
	if (wtPosint_begin_hasNextPos) { //table has positions
		m_begin_nextPosBuffer->setSize(2, bufferNumSamples, false, false, !releaseMemory); //stereo!
		m_begin_nextPosBuffer->clear(startSample, numSamples);
		ok = fillInterpolateBuffersRange(true, wtpbeginnextpos, m_x0_nextPos_writePointer, m_y0_nextPos_writePointer, m_y1_nextPos_writePointer, mOscillator, osciCount, startSample, numSamples, m_begin_nextPosBuffer.get(), bInverter, wtMode, wtFxVal_begin, wtFxType_begin, voiceNo, bank, oscBank, !doPosFade && !hasNextWtfxtype, updated);
		if (!ok) doExit = true;

		if (hasNextWtfxtype) {
			m_begin_nextPosBufferNextWtfxtype->setSize(2, bufferNumSamples, false, false, !releaseMemory); //stereo!
			m_begin_nextPosBufferNextWtfxtype->clear(startSample, numSamples);
			ok = fillInterpolateBuffersRange(true, wtpbeginnextpos, m_x0_nextPos_writePointer, m_y0_nextPos_writePointer, m_y1_nextPos_writePointer, mOscillator, osciCount, startSample, numSamples, m_begin_nextPosBufferNextWtfxtype.get(), bInverter, wtMode, wtFxVal_end, wtFxType_end, voiceNo, bank, oscBank, !doPosFade, updated);
			if (!ok) doExit = true;
		}
	}
	
	if (doPosFade) {
		// =====================================================================================================================
		// end position
		m_end_curPosBuffer->setSize(2, bufferNumSamples, false, false, !releaseMemory); //stereo!		
		m_end_curPosBuffer->clear(startSample, numSamples);
		bool ok = fillInterpolateBuffersRange(false, wtpend, m_x0_curPos_writePointer, m_y0_curPos_writePointer, m_y1_curPos_writePointer, mOscillator, osciCount, startSample, numSamples, m_end_curPosBuffer.get(), bInverter, wtMode, wtFxVal_begin, wtFxType_begin, voiceNo, bank, oscBank, !hasNextWtfxtype && !wtPosint_end_hasNextPos, updated);
		if (!ok) doExit = true;

		if (hasNextWtfxtype) {
			m_end_curPosBufferNextWtfxtype->setSize(2, bufferNumSamples, false, false, !releaseMemory); //stereo! 
			m_end_curPosBufferNextWtfxtype->clear(startSample, numSamples);
			bool ok = fillInterpolateBuffersRange(false, wtpend, m_x0_curPos_writePointer, m_y0_curPos_writePointer, m_y1_curPos_writePointer, mOscillator, osciCount, startSample, numSamples, m_end_curPosBufferNextWtfxtype.get(), bInverter, wtMode, wtFxVal_end, wtFxType_end, voiceNo, bank, oscBank, !wtPosint_end_hasNextPos, updated);
			if (!ok) doExit = true;
		}

		// =====================================================================================================================
		// end next position
		if (wtPosint_end_hasNextPos) { //table has positions
			m_end_nextPosBuffer->setSize(2, bufferNumSamples, false, false, !releaseMemory); //stereo!
			m_end_nextPosBuffer->clear(startSample, numSamples);
			ok = fillInterpolateBuffersRange(true, wtpendnextpos, m_x0_nextPos_writePointer, m_y0_nextPos_writePointer, m_y1_nextPos_writePointer, mOscillator, osciCount, startSample, numSamples, m_end_nextPosBuffer.get(), bInverter, wtMode, wtFxVal_begin, wtFxType_begin, voiceNo, bank, oscBank, !hasNextWtfxtype, updated);
			if (!ok) doExit = true;

			if (hasNextWtfxtype) {
				m_end_nextPosBufferNextWtfxtype->setSize(2, bufferNumSamples, false, false, !releaseMemory); //stereo!
				m_end_nextPosBufferNextWtfxtype->clear(startSample, numSamples);
				ok = fillInterpolateBuffersRange(true, wtpendnextpos, m_x0_nextPos_writePointer, m_y0_nextPos_writePointer, m_y1_nextPos_writePointer, mOscillator, osciCount, startSample, numSamples, m_end_nextPosBufferNextWtfxtype.get(), bInverter, wtMode, wtFxVal_end, wtFxType_end, voiceNo, bank, oscBank, true, updated);
				if (!ok) doExit = true;
			}
		}
	}
	
	if (doExit) {
		buffer->clear(startSample, numSamples);
		return false;
	}

	// =====================================================================================================================
	//optimized linterp	
	//interpolate between wavetable freq n and n+1
	if (wtPosint_begin_hasNextPos || hasNextWtfxtype || doPosFade) {
		//vassert(iFxSamples <= iLastFxSamples);

		if (doPosFade || hasNextWtfxtype) { //otherwise not needed
			m_fracPartBuffer->setSize(1, m_valBeginBuffer->getNumSamples(), false, false, !releaseMemory); //keep symmetry
			float startFracVal = float(curCycleSamples - numSamples) / float(lastCycleSamples);
			float endFracVal = float(curCycleSamples) / float(lastCycleSamples);
			int rampSamples = numSamples;
			if (curCycleSamples > lastCycleSamples) { //CHECK HACK!!!
				//in this case create ramp from startSample to curCycleSamples - lastCycleSamples and let the rest be 1.f (no xfade anymore)
				rampSamples = curCycleSamples - lastCycleSamples;
				endFracVal = 1.f;
			}					
			FloatVectorOperations::fill(m_fracPartBuffer->getWritePointer(0, startSample), 1.f, numSamples);
			if (startFracVal <= 1.f)
				m_fracPartBuffer->applyGainRamp(0, startSample, rampSamples, startFracVal, endFracVal); //float frac = float(iFxSamples - numSamples + count) / float(iLastFxSamples);
		}

		m_valBeginBuffer->copyFrom(0, startSample, m_begin_curPosBuffer->getReadPointer(0, startSample), numSamples);
		m_valBeginBuffer->copyFrom(1, startSample, m_begin_curPosBuffer->getReadPointer(1, startSample), numSamples);

		if (hasNextWtfxtype) {
			FloatVectorOperations::subtractWithMultiply(m_valBeginBuffer->getWritePointer(0, startSample), m_fracPartBuffer->getReadPointer(0, startSample), m_begin_curPosBuffer->getReadPointer(0, startSample), numSamples);  // result = y0 – x0 * y0
			FloatVectorOperations::subtractWithMultiply(m_valBeginBuffer->getWritePointer(1, startSample), m_fracPartBuffer->getReadPointer(0, startSample), m_begin_curPosBuffer->getReadPointer(1, startSample), numSamples);  // result = y0 – x0 * y0
			FloatVectorOperations::addWithMultiply(m_valBeginBuffer->getWritePointer(0, startSample), m_fracPartBuffer->getReadPointer(0, startSample), m_begin_curPosBufferNextWtfxtype->getReadPointer(0, startSample), numSamples);  // result = y0 – x0 * y0 + x0 * y1
			FloatVectorOperations::addWithMultiply(m_valBeginBuffer->getWritePointer(1, startSample), m_fracPartBuffer->getReadPointer(0, startSample), m_begin_curPosBufferNextWtfxtype->getReadPointer(1, startSample), numSamples);  // result = y0 – x0 * y0 + x0 * y1
		}

		if (wtPosint_begin_hasNextPos) {
			m_valBeginBufferNext->copyFrom(0, startSample, m_begin_nextPosBuffer->getReadPointer(0, startSample), numSamples);
			m_valBeginBufferNext->copyFrom(1, startSample, m_begin_nextPosBuffer->getReadPointer(1, startSample), numSamples);

			if (hasNextWtfxtype) {
				FloatVectorOperations::subtractWithMultiply(m_valBeginBuffer->getWritePointer(0, startSample), m_fracPartBuffer->getReadPointer(0, startSample), m_begin_nextPosBuffer->getReadPointer(0, startSample), numSamples);  // result = y0 – x0 * y0
				FloatVectorOperations::subtractWithMultiply(m_valBeginBuffer->getWritePointer(1, startSample), m_fracPartBuffer->getReadPointer(0, startSample), m_begin_nextPosBuffer->getReadPointer(1, startSample), numSamples);  // result = y0 – x0 * y0
				FloatVectorOperations::addWithMultiply(m_valBeginBuffer->getWritePointer(0, startSample), m_fracPartBuffer->getReadPointer(0, startSample), m_begin_nextPosBufferNextWtfxtype->getReadPointer(0, startSample), numSamples);  // result = y0 – x0 * y0 + x0 * y1
				FloatVectorOperations::addWithMultiply(m_valBeginBuffer->getWritePointer(1, startSample), m_fracPartBuffer->getReadPointer(0, startSample), m_begin_nextPosBufferNextWtfxtype->getReadPointer(1, startSample), numSamples);  // result = y0 – x0 * y0 + x0 * y1
			}

			float bnp_frac = wtPos_begin - wtPosint_begin;
			m_valBeginBuffer->copyFrom(0, startSample, m_valBeginBuffer->getReadPointer(0, startSample), numSamples, (1 - bnp_frac));   //buffer = (1- fracpart) * curposBuffer + fracpart * nextposBuffer
			m_valBeginBuffer->copyFrom(1, startSample, m_valBeginBuffer->getReadPointer(1, startSample), numSamples, (1 - bnp_frac));   //buffer = (1- fracpart) * curposBuffer + fracpart * nextposBuffer
			FloatVectorOperations::addWithMultiply(m_valBeginBuffer->getWritePointer(0, startSample), m_valBeginBufferNext->getReadPointer(0, startSample), bnp_frac, numSamples);  // buffer = (curPosBuffer – fractPart * curPosBuffer) + (fractPart * nextPos)
			FloatVectorOperations::addWithMultiply(m_valBeginBuffer->getWritePointer(1, startSample), m_valBeginBufferNext->getReadPointer(1, startSample), bnp_frac, numSamples);  // buffer = (curPosBuffer – fractPart * curPosBuffer) + (fractPart * nextPos)			   
		}

		if (doPosFade) {
			//fade wtpos with frac when wtPos_begin != wtPos_end
			//simultaneously fade wtfx?
			m_valEndBuffer->copyFrom(0, startSample, m_end_curPosBuffer->getReadPointer(0, startSample), numSamples);
			m_valEndBuffer->copyFrom(1, startSample, m_end_curPosBuffer->getReadPointer(1, startSample), numSamples);

			if (hasNextWtfxtype) {
				FloatVectorOperations::subtractWithMultiply(m_valEndBuffer->getWritePointer(0, startSample), m_fracPartBuffer->getReadPointer(0, startSample), m_end_curPosBuffer->getReadPointer(0, startSample), numSamples);  // result = y0 – x0 * y0
				FloatVectorOperations::subtractWithMultiply(m_valEndBuffer->getWritePointer(1, startSample), m_fracPartBuffer->getReadPointer(0, startSample), m_end_curPosBuffer->getReadPointer(1, startSample), numSamples);  // result = y0 – x0 * y0
				FloatVectorOperations::addWithMultiply(m_valEndBuffer->getWritePointer(0, startSample), m_fracPartBuffer->getReadPointer(0, startSample), m_end_curPosBufferNextWtfxtype->getReadPointer(0, startSample), numSamples);  // result = y0 – x0 * y0 + x0 * y1
				FloatVectorOperations::addWithMultiply(m_valEndBuffer->getWritePointer(1, startSample), m_fracPartBuffer->getReadPointer(0, startSample), m_end_curPosBufferNextWtfxtype->getReadPointer(1, startSample), numSamples);  // result = y0 – x0 * y0 + x0 * y1
			}

			if (wtPosint_end_hasNextPos) {
				m_valEndBufferNext->copyFrom(0, startSample, m_end_nextPosBuffer->getReadPointer(0, startSample), numSamples);
				m_valEndBufferNext->copyFrom(1, startSample, m_end_nextPosBuffer->getReadPointer(1, startSample), numSamples);

				if (hasNextWtfxtype) {
					FloatVectorOperations::subtractWithMultiply(m_valEndBufferNext->getWritePointer(0, startSample), m_fracPartBuffer->getReadPointer(0, startSample), m_end_nextPosBuffer->getReadPointer(0, startSample), numSamples);  // result = y0 – x0 * y0
					FloatVectorOperations::subtractWithMultiply(m_valEndBufferNext->getWritePointer(1, startSample), m_fracPartBuffer->getReadPointer(0, startSample), m_end_nextPosBuffer->getReadPointer(1, startSample), numSamples);  // result = y0 – x0 * y0
					FloatVectorOperations::addWithMultiply(m_valEndBufferNext->getWritePointer(0, startSample), m_fracPartBuffer->getReadPointer(0, startSample), m_end_nextPosBufferNextWtfxtype->getReadPointer(0, startSample), numSamples);  // result = y0 – x0 * y0 + x0 * y1
					FloatVectorOperations::addWithMultiply(m_valEndBufferNext->getWritePointer(1, startSample), m_fracPartBuffer->getReadPointer(0, startSample), m_end_nextPosBufferNextWtfxtype->getReadPointer(1, startSample), numSamples);  // result = y0 – x0 * y0 + x0 * y1
				}

				float bnp_frac = wtPos_end - wtPosint_end;
				m_valEndBuffer->copyFrom(0, startSample, m_valEndBuffer->getReadPointer(0, startSample), numSamples, (1 - bnp_frac));   //buffer = (1- fracpart) * curposBuffer + fracpart * nextposBuffer
				m_valEndBuffer->copyFrom(1, startSample, m_valEndBuffer->getReadPointer(1, startSample), numSamples, (1 - bnp_frac));   //buffer = (1- fracpart) * curposBuffer + fracpart * nextposBuffer
				FloatVectorOperations::addWithMultiply(m_valEndBuffer->getWritePointer(0, startSample), m_valEndBufferNext->getReadPointer(0, startSample), bnp_frac, numSamples);  // buffer = (curPosBuffer – fractPart * curPosBuffer) + (fractPart * nextPos)
				FloatVectorOperations::addWithMultiply(m_valEndBuffer->getWritePointer(1, startSample), m_valEndBufferNext->getReadPointer(1, startSample), bnp_frac, numSamples);  // buffer = (curPosBuffer – fractPart * curPosBuffer) + (fractPart * nextPos)			   
			}

			//crossfade
			buffer->copyFrom(0, startSample, m_valBeginBuffer->getReadPointer(0, startSample), numSamples);
			buffer->copyFrom(1, startSample, m_valBeginBuffer->getReadPointer(1, startSample), numSamples);
			FloatVectorOperations::subtractWithMultiply(buffer->getWritePointer(0, startSample), m_fracPartBuffer->getReadPointer(0, startSample), m_valBeginBuffer->getReadPointer(0, startSample), numSamples);  // result = y0 – x0 * y0
			FloatVectorOperations::subtractWithMultiply(buffer->getWritePointer(1, startSample), m_fracPartBuffer->getReadPointer(0, startSample), m_valBeginBuffer->getReadPointer(1, startSample), numSamples);  // result = y0 – x0 * y0
			FloatVectorOperations::addWithMultiply(buffer->getWritePointer(0, startSample), m_fracPartBuffer->getReadPointer(0, startSample), m_valEndBuffer->getReadPointer(0, startSample), numSamples);  // result = y0 – x0 * y0 + x0 * y1
			FloatVectorOperations::addWithMultiply(buffer->getWritePointer(1, startSample), m_fracPartBuffer->getReadPointer(0, startSample), m_valEndBuffer->getReadPointer(1, startSample), numSamples);  // result = y0 – x0 * y0 + x0 * y1
		}
		else {
			buffer->addFrom(0, startSample, m_valBeginBuffer->getReadPointer(0, startSample), numSamples); //stereo spread
			buffer->addFrom(1, startSample, m_valBeginBuffer->getReadPointer(1, startSample), numSamples); //stereo spread
		}
	}
	else {
		//VDBG("No next pos");
		buffer->addFrom(0, startSample, m_begin_curPosBuffer->getReadPointer(0, startSample), numSamples); //stereo spread
		buffer->addFrom(1, startSample, m_begin_curPosBuffer->getReadPointer(1, startSample), numSamples); //stereo spread
	}
	
	/*
	if (wtPosint_begin_hasNextPos || hasNextWtfxtype || doPosFade) {
		//vassert(iFxSamples <= iLastFxSamples);

		int count = 0;
		int length = numSamples;
		for (int i = startSample; i < (startSample + numSamples); i++) {
			float frac = float(iFxSamples - numSamples + count) / float(iLastFxSamples);				
			
			//if (i == startSample) VDBG("Frac_beg " + String(frac));
			//if (i == startSample + numSamples - 1) VDBG("Frac_end " + String(frac));

			if (iFxSamples > iLastFxSamples) //CHECK HACK!!!
				frac = 1.f;

			float lValBegin = m_begin_curPosBuffer->getSample(0, i);
			float rValBegin = m_begin_curPosBuffer->getSample(1, i);
			float lValEnd = 0.f;
			float rValEnd = 0.f;

			if (hasNextWtfxtype) {
				lValBegin = (1.f - frac) * lValBegin + frac * m_begin_curPosBufferNextWtfxtype->getSample(0, i);
				rValBegin = (1.f - frac) * rValBegin + frac * m_begin_curPosBufferNextWtfxtype->getSample(1, i);
			}

			if (wtPosint_begin_hasNextPos) {
				float lValBeginNext = m_begin_nextPosBuffer->getSample(0, i);
				float rValBeginNext = m_begin_nextPosBuffer->getSample(1, i);

				if (hasNextWtfxtype) {
					lValBeginNext = (1.f - frac) * lValBeginNext + frac * m_begin_nextPosBufferNextWtfxtype->getSample(0, i);
					rValBeginNext = (1.f - frac) * rValBeginNext + frac * m_begin_nextPosBufferNextWtfxtype->getSample(1, i);
				}

				float bnp_frac = wtPos_begin - wtPosint_begin;
				lValBegin= (1.f - bnp_frac) * lValBegin + bnp_frac * lValBeginNext; 
				rValBegin = (1.f - bnp_frac) * rValBegin + bnp_frac * rValBeginNext;
			}

			float lVal = lValBegin;
			float rVal = rValBegin;

			if (doPosFade) {
				//fade wtpos with frac when wtPos_begin != wtPos_end
				//simultaneously fade wtfx?
				lValEnd = m_end_curPosBuffer->getSample(0, i);
				rValEnd = m_end_curPosBuffer->getSample(1, i);

				if (hasNextWtfxtype) {
					lValEnd = (1.f - frac) * lValEnd + frac * m_end_curPosBufferNextWtfxtype->getSample(0, i);
					rValEnd = (1.f - frac) * rValEnd + frac * m_end_curPosBufferNextWtfxtype->getSample(1, i);
				}

				if (wtPosint_end_hasNextPos) {
					float lValEndNext = m_end_nextPosBuffer->getSample(0, i);
					float rValEndNext = m_end_nextPosBuffer->getSample(1, i);

					if (hasNextWtfxtype) {
						lValEndNext = (1.f - frac) * lValEndNext + frac * m_end_nextPosBufferNextWtfxtype->getSample(0, i);
						rValEndNext = (1.f - frac) * rValEndNext + frac * m_end_nextPosBufferNextWtfxtype->getSample(1, i);
					}

					float bnp_frac = wtPos_end - wtPosint_end;
					lValEnd = (1.f - bnp_frac) * lValEnd + bnp_frac * lValEndNext;
					rValEnd = (1.f - bnp_frac) * rValEnd + bnp_frac * rValEndNext;
				}

				lVal = (1.f - frac) * lValBegin + frac * lValEnd; //crossfade
				rVal = (1.f - frac) * rValBegin + frac * rValEnd; //crossfade
			}

			buffer->setSample(0, i, lVal);
			buffer->setSample(1, i, rVal);
			count++;
		}	
	}
	else {
		//VDBG("No next pos");
		buffer->addFrom(0, startSample, m_begin_curPosBuffer->getReadPointer(0, startSample), numSamples); //stereo spread
		buffer->addFrom(1, startSample, m_begin_curPosBuffer->getReadPointer(1, startSample), numSamples); //stereo spread
	}
	*/

	/*
	if (hasNextPos) { //perf opt
		//interpolate between wavetable n and n+1	
		m_interpolBuffer->setSize(1, m_oscBufferStereo->getNumSamples(), false, false, !releaseMemory); //keep symmetry
		m_fracPartBuffer->setSize(1, m_oscBufferStereo->getNumSamples(), false, false, !releaseMemory); //keep symmetry

		if (bWTPos_allSameValues) { //best case!
			//VDBG("bWTPos_allSameValues");
			float fracPart = wtPosBuffer->getSample(0, startSample) - int(wtPosBuffer->getSample(0, startSample)); //0..1																												 
			m_oscBufferStereo->copyFrom(0, startSample, m_curPosBuffer->getReadPointer(0, startSample), numSamples, (1 - fracPart));   //buffer = (1- fracpart) * curposBuffer + fracpart * nextposBuffer
			m_oscBufferStereo->copyFrom(1, startSample, m_curPosBuffer->getReadPointer(1, startSample), numSamples, (1 - fracPart));   //buffer = (1- fracpart) * curposBuffer + fracpart * nextposBuffer
			FloatVectorOperations::addWithMultiply(m_oscBufferStereo->getWritePointer(0, startSample), m_nextPosBuffer->getReadPointer(0, startSample), fracPart, numSamples);  // buffer = (curPosBuffer – fractPart * curPosBuffer) + (fractPart * nextPos)
			FloatVectorOperations::addWithMultiply(m_oscBufferStereo->getWritePointer(1, startSample), m_nextPosBuffer->getReadPointer(1, startSample), fracPart, numSamples);  // buffer = (curPosBuffer – fractPart * curPosBuffer) + (fractPart * nextPos)
		}
		else {
			//VDBG("!bWTPos_allSameValues");
			VASTFloatVectorOperations::getFraction(m_fracPartBuffer->getWritePointer(0, startSample), wtPosBuffer->getReadPointer(0, startSample), numSamples); //dest is new buffer, source is wtPosBuffer // float fracPart = wtPosBuffer->getSample(0, currentFrame) - int(wtPosBuffer->getSample(0, currentFrame)); //0..1		
			m_oscBufferStereo->copyFrom(0, startSample, m_curPosBuffer->getReadPointer(0, startSample), numSamples);
			m_oscBufferStereo->copyFrom(1, startSample, m_curPosBuffer->getReadPointer(1, startSample), numSamples);
			FloatVectorOperations::subtractWithMultiply(m_oscBufferStereo->getWritePointer(0, startSample), m_fracPartBuffer->getReadPointer(0, startSample), m_curPosBuffer->getReadPointer(0, startSample), numSamples);  // result = y0 – x0 * y0
			FloatVectorOperations::subtractWithMultiply(m_oscBufferStereo->getWritePointer(1, startSample), m_fracPartBuffer->getReadPointer(0, startSample), m_curPosBuffer->getReadPointer(1, startSample), numSamples);  // result = y0 – x0 * y0
			FloatVectorOperations::addWithMultiply(m_oscBufferStereo->getWritePointer(0, startSample), m_fracPartBuffer->getReadPointer(0, startSample), m_nextPosBuffer->getReadPointer(0, startSample), numSamples);  // result = y0 – x0 * y0 + x0 * y1
			FloatVectorOperations::addWithMultiply(m_oscBufferStereo->getWritePointer(1, startSample), m_fracPartBuffer->getReadPointer(0, startSample), m_nextPosBuffer->getReadPointer(1, startSample), numSamples);  // result = y0 – x0 * y0 + x0 * y1
		}

		buffer->addFrom(0, startSample, m_oscBufferStereo->getReadPointer(0, startSample), numSamples); //stereo spread
		buffer->addFrom(1, startSample, m_oscBufferStereo->getReadPointer(1, startSample), numSamples); //stereo spread
	}
	else {
		//VDBG("No next pos");
		buffer->addFrom(0, startSample, m_curPosBuffer->getReadPointer(0, startSample), numSamples); //stereo spread
		buffer->addFrom(1, startSample, m_curPosBuffer->getReadPointer(1, startSample), numSamples); //stereo spread
	}
	*/

	return true;
}

bool CVASTWaveTable::fillInterpolateBuffersRange(const bool next, sWaveTablePosition* wtp, float* x0, float* y0, float* y1, CVASTWaveTableOscillator* mOscillator, const int osciCount, const int startSample, const int numSamples, AudioSampleBuffer* posBuffer, const bool bInverter, int wtMode, float wtFxVal, int wtFxType, int voiceNo, int bank, OwnedArray<CVASTOscillatorBank>* oscBank, bool updateUI, bool &updated) {
	/*
	String test = "fillInterpolateBuffersRange: " + (next == true) ? " next " : " cur ";
	test += " start: " + String(startSample) + " end: " + String(startSample + numSamples - 1) +  " num: " + String(numSamples) + " wtPos: " + String(wtp->wtPos);
	VDBG(test);
	*/

	const sWaveTableFreq* curPosWTFreq = nullptr;
	int currentFrame = startSample; //only once per buffer the freq is changed

	int masterOsci = 0; //master
	//AudioSampleBuffer* phasedPhasorBuffer; //= mOscillator->m_phasedPhasorBuffer[masterOsci].get();
	AudioSampleBuffer* phaseIncBuffer = mOscillator->m_phaseIncBuffer[masterOsci].get();

	int numWaveTableFreqs = wtp->numWaveTableFreqs;
	if (numWaveTableFreqs == 0) {
		vassert(false); //how does it happen?
		return false;
	}

	//perfoptimize - check if last position still valid - or startpoint
	int waveTableIdx = m_lastWaveTableIdx;
	if (next)
		waveTableIdx = m_lastWaveTableIdxNext;

	//if (wtp->waveTableFreqs.size() <= waveTableIdx) //wt was changed in between!
	if ((wtp->waveTableFreqs.size() <= waveTableIdx) || (wtp->numWaveTableFreqs <= waveTableIdx)) //wt was changed in between!
		//waveTableIdx = wtp->waveTableFreqs.size() - 1;
		waveTableIdx = wtp->numWaveTableFreqs - 1;

	if (!((phaseIncBuffer->getSample(0, currentFrame) > wtp->waveTableFreqs[waveTableIdx].bottomFreq) &&
		(phaseIncBuffer->getSample(0, currentFrame) <= wtp->waveTableFreqs[waveTableIdx].topFreq)))
	{
		if (phaseIncBuffer->getSample(0, currentFrame) > wtp->waveTableFreqs[waveTableIdx].topFreq)
			while ((phaseIncBuffer->getSample(0, currentFrame) >= wtp->waveTableFreqs[waveTableIdx].topFreq) &&
				(waveTableIdx < (numWaveTableFreqs - 1)) &&
				(wtp->waveTableFreqs[waveTableIdx].invalid == false))
				++waveTableIdx;
		else
			while ((waveTableIdx > 0) && (phaseIncBuffer->getSample(0, currentFrame) <= wtp->waveTableFreqs[waveTableIdx - 1].topFreq) &&
				(wtp->waveTableFreqs[waveTableIdx - 1].invalid == false))
				--waveTableIdx;
	}
	if (!((phaseIncBuffer->getSample(0, currentFrame) > wtp->waveTableFreqs[waveTableIdx].bottomFreq) && (phaseIncBuffer->getSample(0, currentFrame) <= wtp->waveTableFreqs[waveTableIdx].topFreq))) {
		//VDBG("CVASTWaveTable::fillInterpolateBuffers - out of range - using highest buffer: " + String(phaseIncBuffer->getSample(0, currentFrame)));
		//cannot be played - out of frequency range
		//check: use highest freq that is not invalid!
		waveTableIdx = wtp->numWaveTableFreqs - 1;
		//return false;
	}
	if (!next)
		m_lastWaveTableIdx = waveTableIdx;
	else
		m_lastWaveTableIdxNext = waveTableIdx;

	bool regenerated = false;
	if ((wtp->waveTableFreqs[waveTableIdx].dirty == true) ||
		(wtp->numWaveTableFreqs <= waveTableIdx)) {

		//dynamic wavetable generation
		VDBG("CVASTWaveTable::fillInterpolateBuffers - dynamic wavetable freq generation! WtPos: " << wtp->wtPos << " WtFreq: " << waveTableIdx);
		//VDBG("dynamic wavetable generation phasedPhasorBufferPointer[startSample]: " + String(mOscillator->m_phasedPhasorBufferPointer[0][startSample]));
		makeWaveTableFreq(wtp->wtPos, wtp->waveTableFreqs[waveTableIdx].waveTableLen,
			wtp->waveTableFreqs[waveTableIdx].bottomFreq, wtp->waveTableFreqs[waveTableIdx].topFreq, wtp->waveTableFreqs[waveTableIdx].maxHarmonics,
			true, waveTableIdx, true, wtMode, wtFxVal, wtFxType, phaseIncBuffer->getSample(0, currentFrame)); //change true update idx
		regenerated = true;
		updated = true;
		bool ok = getWaveTablePosition(wtp->wtPos, wtp);
		if (!ok) {
			vassert(false);
			return false;
		}

		/*
		#ifdef _DEBUG
				//bug catcher
				float max = *max_element(wtp->waveTableFreqs[waveTableIdx].waveTableSamples.begin(), wtp->waveTableFreqs[waveTableIdx].waveTableSamples.end());
				if (max > 5.f)
					VDBG("Overflow!!!!");
		#endif
		*/
	}
	curPosWTFreq = &wtp->waveTableFreqs[waveTableIdx];
	
	//WAVETABLE FX HERE			
	if (regenerated == false) {
		if (wtFreqCheckForChange(wtp->waveTableFreqs[waveTableIdx], wtFxVal, wtFxType)) {
			updated = true;
			m_isBeingUpdated.store(true);
			if (wtp->waveTableFreqs[waveTableIdx].wtFxType != wtFxType) { //reset it
				ScopedLock sl(mWavetableChangeLock);
				wtp->hasFXTable = false;
				VDBG("fillInterpolateBuffersRange delete old stuff");
				wtp->numWaveTableFreqsBuffer = 0;
				wtp->waveTableFreqsBuffer.clear();
				for (int i = 0; i < wtp->waveTableFreqs.size(); i++) {
					wtp->waveTableFreqs[i].bottomFreq = wtp->waveTableFreqs[i].bottomFreqOrig;
					wtp->waveTableFreqs[i].topFreq = wtp->waveTableFreqs[i].topFreqOrig;
				}
				//generateWaveTableFreqsFromTimeDomain(wtp->wtPos, C_WAVE_TABLE_SIZE, *getNaiveTable(wtp->wtPos), true, wtMode);
			}

			//look in buffer
			bool foundInBuffer = false;
			int windex = int(wtp->waveTableFreqsBuffer.size());
			if ((wtFxType > 0) && (wtp->waveTableFreqs[waveTableIdx].wtFxType == wtFxType)) {
				while (windex > 0) {
					windex--;
					if (((phaseIncBuffer->getSample(0, currentFrame) > wtp->waveTableFreqsBuffer[windex].bottomFreq)
						&& (phaseIncBuffer->getSample(0, currentFrame) < wtp->waveTableFreqsBuffer[windex].topFreq)) &&
						//((wtp->waveTableFreqsBuffer[windex].wtFxType == wtFxType) && (abs(wtp->waveTableFreqsBuffer[windex].wtFxVal - wtFxVal) < 0.001f))) {
						((wtp->waveTableFreqsBuffer[windex].wtFxType == wtFxType) && (abs(wtp->waveTableFreqsBuffer[windex].wtFxVal - wtFxVal) < 0.05f))) { //check this parameter
						foundInBuffer = true;
						VDBG("Found in Buffer!!: " << wtFxVal);
						break;
					}
				}
			}

			if ((foundInBuffer) && (wtFxType != C_WTFXTYPE_FM)) { 
				curPosWTFreq = &wtp->waveTableFreqsBuffer[windex];
				//CHECK COPY BACK
				wtp->waveTableFreqs[waveTableIdx] = wtp->waveTableFreqsBuffer[windex];
				//CHECK COPY BACK
				vassert(curPosWTFreq->waveTableSamples.size() == C_WAVE_TABLE_SIZE);
				if (updateUI) {
					wtp->naiveTableFX = curPosWTFreq->naiveTableFXBufferCopy;					
					wtp->naiveTableFXDirty = true;
				}
			}
			else {
				//add current freq to buffer
				if (wtFxType != C_WTFXTYPE_FM) //FM not useful in buffer
					putWaveTableFreqToBuffer(wtp, waveTableIdx);

				makeWaveTableFreq(wtp->wtPos, wtp->waveTableFreqs[waveTableIdx].waveTableLen,
					wtp->waveTableFreqs[waveTableIdx].bottomFreq, wtp->waveTableFreqs[waveTableIdx].topFreq, wtp->waveTableFreqs[waveTableIdx].maxHarmonics,
					true, waveTableIdx, true, wtMode, wtFxVal, wtFxType, phaseIncBuffer->getSample(0, currentFrame));

				VDBG("makeWaveTableFreq phasedPhasorBufferPointer, startSample: " << startSample << " wtPos "  << wtp->wtPos << " phasor: " << mOscillator->m_phasedPhasorBufferPointer[0][startSample] << " wtFxVal: " << wtFxVal << " index: " << waveTableIdx);
			}
			if ((wtFxType == 0) || (wtFxType == C_WTFXTYPE_FM)) { //reset it
				ScopedLock sl(mWavetableChangeLock);
				wtp->hasFXTable = false;
				VDBG("fillInterpolateBuffersRange delete old stuff");
				wtp->numWaveTableFreqsBuffer = 0;
				wtp->waveTableFreqsBuffer.clear();
			}
			m_isBeingUpdated.store(false);
		}		
	}	
	
	if (curPosWTFreq->dirty)
		vassert(false);
	vassert(curPosWTFreq->waveTableSamples.size() == C_WAVE_TABLE_SIZE);

	//========================================================================
	// All oscis here
	const float * l_oscBufferReadPointer = m_oscBuffer->getWritePointer(0, startSample);
	float * l_oscBufferWritePointer = m_oscBuffer->getWritePointer(0, startSample);

	for (int osci = 0; osci < osciCount; osci++) {
		//phasedPhasorBuffer = mOscillator->m_phasedPhasorBuffer[osci].get();
		const float* phasedPhasorBufferPointer = mOscillator->m_phasedPhasorBufferPointer[osci];
		
		//std::unique_ptr<AudioSampleBuffer> l_phasorbuf;
		//l_phasorbuf = new AudioSampleBuffer(1, m_x0_curPos->getNumSamples());
		//VASTFloatVectorOperations::multiply(l_phasorbuf->getWritePointer(0) + startSample, phasedPhasorBuffer->getReadPointer(0, startSample), float(C_WAVE_TABLE_SIZE), numSamples);
		//VASTFloatVectorOperations::getFraction(x0 + startSample, l_phasorbuf->getWritePointer(0) + startSample, numSamples);

		//expensive loop - optimize here
		for (int currentFrame = startSample; currentFrame < startSample + numSamples; currentFrame++) {

			/* working but slightly slower?!
			//std::unique_ptr<AudioSampleBuffer> l_x0_curPos;
			//std::unique_ptr<AudioSampleBuffer> l_y0_curPos;
			//l_x0_curPos = new AudioSampleBuffer(1, m_x0_curPos->getNumSamples());
			//l_y0_curPos = new AudioSampleBuffer(1, m_y0_curPos->getNumSamples());
			//float* l_x0_curPos_writePointer = l_x0_curPos->getWritePointer(0);
			//float* l_y0_curPos_writePointer = l_y0_curPos->getWritePointer(0);
			//VASTFloatVectorOperations::getFraction(l_x0_curPos_writePointer + startSample, phasedPhasorBuffer->getReadPointer(0, startSample), numSamples); //does not work on sub buffers
			//VASTFloatVectorOperations::subtract(l_y0_curPos_writePointer + startSample, phasedPhasorBuffer->getReadPointer(0, startSample), l_x0_curPos_writePointer + startSample, numSamples);

			if (SystemStats::hasAVX2()) {
				if (startSample + numSamples - currentFrame >= 8) {
					int idx0 = phasedPhasorBuffer->getSample(0, currentFrame) * C_WAVE_TABLE_SIZE;
					int idx1 = phasedPhasorBuffer->getSample(0, currentFrame + 1) * C_WAVE_TABLE_SIZE;
					int idx2 = phasedPhasorBuffer->getSample(0, currentFrame + 2) * C_WAVE_TABLE_SIZE;
					int idx3 = phasedPhasorBuffer->getSample(0, currentFrame + 3) * C_WAVE_TABLE_SIZE;
					int idx4 = phasedPhasorBuffer->getSample(0, currentFrame + 4) * C_WAVE_TABLE_SIZE;
					int idx5 = phasedPhasorBuffer->getSample(0, currentFrame + 5) * C_WAVE_TABLE_SIZE;
					int idx6 = phasedPhasorBuffer->getSample(0, currentFrame + 6) * C_WAVE_TABLE_SIZE;
					int idx7 = phasedPhasorBuffer->getSample(0, currentFrame + 7) * C_WAVE_TABLE_SIZE;
					int ndx0 = (idx0 + 1) % C_WAVE_TABLE_SIZE;
					int ndx1 = (idx1 + 1) % C_WAVE_TABLE_SIZE;
					int ndx2 = (idx2 + 1) % C_WAVE_TABLE_SIZE;
					int ndx3 = (idx3 + 1) % C_WAVE_TABLE_SIZE;
					int ndx4 = (idx4 + 1) % C_WAVE_TABLE_SIZE;
					int ndx5 = (idx5 + 1) % C_WAVE_TABLE_SIZE;
					int ndx6 = (idx6 + 1) % C_WAVE_TABLE_SIZE;
					int ndx7 = (idx7 + 1) % C_WAVE_TABLE_SIZE;
					__m256i idx = _mm256_set_epi32(idx7, idx6, idx5, idx4, idx3, idx2, idx1, idx0);
					__m256i ndx = _mm256_set_epi32(ndx7, ndx6, ndx5, ndx4, ndx3, ndx2, ndx1, ndx0);
					__m256 ivalues = _mm256_i32gather_ps(&*curPosWTFreq->waveTableSamples.begin(), idx, 4); //4 is scale = sizeof(float)
					_mm256_storeu_ps(y0 + currentFrame, ivalues);
					__m256 nvalues = _mm256_i32gather_ps(&*curPosWTFreq->waveTableSamples.begin(), ndx, 4); //4 is scale = sizeof(float)
					_mm256_storeu_ps(y1 + currentFrame, nvalues);
					currentFrame += 7;
					continue;
				}
			}			
			*/
		
			//https://en.wikipedia.org/wiki/Gather-scatter_(vector_addressing)
			float phasor = phasedPhasorBufferPointer[currentFrame] * C_WAVE_TABLE_SIZE;
			int intPart = phasor;
			float fracPart = phasor- intPart;
			x0[currentFrame] = fracPart;		
			y0[currentFrame] = curPosWTFreq->waveTableSamples[intPart];
			y1[currentFrame] = curPosWTFreq->waveTableSamples[(intPart + 1) % C_WAVE_TABLE_SIZE];		
		}
			
		m_oscBuffer->copyFrom(0, startSample, y0 + startSample, numSamples);
		FloatVectorOperations::subtractWithMultiply(l_oscBufferWritePointer, x0 + startSample, y0 + startSample, numSamples);  // result = y0 – x0 * y0
		FloatVectorOperations::addWithMultiply(l_oscBufferWritePointer, x0 + startSample, y1 + startSample, numSamples);  // result = y0 – x0 * y0 + x0 * y1		

		if ((osci == 0) && (bInverter)) //only first is inverted
			m_oscBuffer->applyGain(-1.f);

		if ((osciCount == 1) || ((osci == 0) && (osciCount % 2 == 1))) { //if only one or first of odd number
			posBuffer->addFrom(0, startSample, l_oscBufferReadPointer, numSamples, 0.707f); //stereo spread, equal power - sin(PI /4)
			posBuffer->addFrom(1, startSample, l_oscBufferReadPointer, numSamples, 0.707f); //stereo spread, equal power - cos(PI /4)
		}
		else {
			if ((osci % 2) == 0) {
				posBuffer->addFrom(0, startSample, l_oscBufferReadPointer, numSamples); //hard left
			}
			else {
				posBuffer->addFrom(1, startSample, l_oscBufferReadPointer, numSamples); //hard right
			}
		}
	}
	//expensive loop - optimize here

	return true;	
}

/*
test: clearing an existing wavetable vector is ten times faster than creating a new one	
{
	std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < 10000; i++) {
		std::vector<float> a1 = std::vector<float>(C_WAVE_TABLE_SIZE, 0.f);
	}
	std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
	VDBG("Duration: " << duration);
}

{
	std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
	std::vector<float> a2 = std::vector<float>(C_WAVE_TABLE_SIZE);
	for (int i = 0; i < 10000; i++) {
		std::fill(a2.begin(), a2.end(), 0.f);
	}
	std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
	VDBG("Duration: " << duration);
}	
*/
