/*
VAST Dynamics Audio Software (TM)
*/

#pragma once

//#define doLinearInterp 1
#include <vector>
#include "../VASTEngineHeader.h"
#include "../VASTSettings.h"
#include "../../muFFT/fft.h"
#include "../../muFFT/fft_internal.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "../VASTSettings.h"

//no std::vector - is not trivially copyable!
typedef struct sWaveTableFreq {
	bool invalid;
	float topFreq;
	float bottomFreq;
	float topFreqOrig;
	float bottomFreqOrig;
	int waveTableLen;
	int maxHarmonics;
	float wtFxVal;
	int wtFxType;
	std::vector<float> waveTableSamples;
	bool dirty; //means that vector with samples has to be redone
	bool isBuffer; //is wx buffer freq
	std::vector<float> naiveTableFXBufferCopy; //not always required, only in Buffer
} sWaveTableFreq;

typedef struct sWaveTablePosition {
	int numWaveTableFreqs;
	int numWaveTableFreqsBuffer;
	int wtPos;
	std::vector<float> naiveTable; //always required
	std::vector<float> naiveTableFX; //not always required	
	std::vector<float> naiveTableFXDisplayCopy; //copied during softfade
	bool naiveTableFXDirty = false;
	bool hasFXTable = false;
	std::vector<dsp::Complex<float>> frequencyDomainBuffer;
	int maxHarmonics;
	bool isInitial; //describes if shall be saved or not
	std::vector<sWaveTableFreq> waveTableFreqs;
	std::vector<sWaveTableFreq> waveTableFreqsBuffer;
	bool dirty; //means that vector of wave table frequencies has to be redone
} sWaveTablePosition;

typedef struct sWaveTableHeader {
	juce::String waveTableName = "";
    std::atomic<int> numPositions;
	std::vector<sWaveTablePosition> waveTablePositions;
	std::atomic<int> changeCounter;
} sWaveTableHeader;

class CVASTOscillatorBank; //forward declaration
class CVASTWaveTableOscillator; //forward declaration
class CVASTWaveTable {
public:
	CVASTWaveTable(CVASTSettings &set);
	~CVASTWaveTable(void);
	CVASTWaveTable(const CVASTWaveTable &wavetable); //copy constructor

	//avoid all locking on the audio thread: http://www.rossbencina.com/code/real-time-audio-programming-101-time-waits-for-nothing
	std::shared_ptr<CVASTWaveTable> getClonedInstance(bool deleteGeneratedContent, bool copyAlsoFreqs);
	void deleteGeneratedContent();

	void duplicatePosition(int wtPos, int newPos);
	void copyPositionToOtherWavetable(int wtPos, int newPos, CVASTWaveTable* wavetable);
	void copyWTFreqsFrom(const CVASTWaveTable& wavetable);
	void addFromOtherWavetable(int newPos, sWaveTablePosition wtp);
	void clear();
	int getNumPositions() const;
	juce::String getWaveTableName();
	void setWaveTableName(StringRef wtname);
	
	std::vector<float>* getNaiveTableWithFXForDisplay(int wtPos, int wtFXType, float wtFXVal, bool forceRecalc);
	std::vector<float>* getNaiveTable(int wtPos);
	void frequencyDomainBufferFromNaive(int tableLen, const std::vector<float> &naiveTable, std::vector<dsp::Complex<float>> &frequencyDomainBuffer);
	std::vector<dsp::Complex<float>>* getFreqDomainBuffer(int wtPos);;

	//WT FX
	typedef void (CVASTWaveTable::*WTFXFuncPtr)(std::vector<float>* naive, float paramvalue); //function pointer
	void wTFX_PASS(std::vector<float>* naive, float paramvalue);
	void wTFX_BENDP(std::vector<float>* naive, float paramvalue);
	void wTFX_BENDM(std::vector<float>* naive, float paramvalue);
	void wTFX_BENDMP(std::vector<float>* naive, float paramvalue);
	void wTFX_BLOATP(std::vector<float>* naive, float paramvalue);
	void wTFX_BLOATM(std::vector<float>* naive, float paramvalue);
	void wTFX_BLOATMP(std::vector<float>* naive, float paramvalue);
	void wTFX_DIST(std::vector<float>* naive, float paramvalue);
	void wTFX_HARDSYNC(std::vector<float>* naive, float paramvalue);
	void wTFX_HARDSYNCWNDW(std::vector<float>* naive, float paramvalue);
	void wTFX_HARDSYNCMIX(std::vector<float>* naive, float paramvalue);
	void wTFX_PWM(std::vector<float>* naive, float paramvalue);
	void wTFX_FM(std::vector<float>* naive, float paramvalue);
	void wTFX_FMWNDW(std::vector<float>* naive, float paramvalue);
	void wTFX_FMMIX(std::vector<float>* naive, float paramvalue);
	void wTFX_FLIP(std::vector<float>* naive, float paramvalue);
	void wTFX_MIRROR(std::vector<float>* naive, float paramvalue);
	void wTFX_BREED(std::vector<float>* naive, float paramvalue);
	void wTFX_BREEDWNDW(std::vector<float>* naive, float paramvalue);
	void wTFX_BREEDMIX(std::vector<float>* naive, float paramvalue);
	void wTFX_BREW(std::vector<float>* naive, float paramvalue);
	void wTFX_BREWWNDW(std::vector<float>* naive, float paramvalue);
	void wTFX_BREWMIX(std::vector<float>* naive, float paramvalue);
	void wTFX_SINCIFY(std::vector<float>* naive, float paramvalue);
	void wTFX_SINCIFYWNDW(std::vector<float>* naive, float paramvalue);
	void wTFX_SINCIFYMIX(std::vector<float>* naive, float paramvalue);
	void wTFX_CRUSH(std::vector<float>* naive, float paramvalue);
	void wTFX_SMOOTH(std::vector<float>* naive, float paramvalue);
	void wTFX_WINDOW(std::vector<float>* naive, float paramvalue);
	WTFXFuncPtr getWTFXFuncPtr(int type);
	bool wtFreqCheckForChange(sWaveTableFreq &wtFreq, float wtFxVal, int wtFxType);
	//WT FX

	void addPosition();
	void addPositions(int numPos);
	void insertEmptyPosition(int wtPos);
	
	atomic<bool> m_isBeingUpdated = false;

	void setNaiveTableFast(int wtPos, bool preGenerate, int wtMode);
	void setNaiveTable(int wtPos, std::vector<float> vWave, bool preGenerate, int wtMode);
	void setFreqDomainTables(int wtPos, std::vector<dsp::Complex<float>>* domainBuffer, bool preGenerate, bool clipBins, int wtMode);
	void deletePosition(int numPos);
	bool isPositionDirty(int wtPos) { return wtheader.waveTablePositions[wtPos].dirty; };
	
	bool getWavetableInterpolateBuffer(CVASTWaveTableOscillator* mOscillator, const int osciCount, AudioSampleBuffer* buffer, const int startSample, const int numSamples, const bool bInverter, int wtMode, int voiceNo, int bank, bool isFirstCycle, bool isStartOfCycle, float wtPosPercTarget, float wtFxVal, int wtFxType, int lastCycleSamples, int curCycleSamples, const CVASTWaveTable* masterWTForSync, bool &updated, bool soloMode);
	
	void getValueTreeState(ValueTree* tree, UndoManager* undoManager);
	bool setValueTreeState(ValueTree* tree, int wtMode);

	void prepareForPlay(int expectedSamplesPerBlock);
	
	void markAllWTFreqsDirty(); //need recalc
	void pregenerateWithWTFX(int wtFxType, float wtFxVal, int wtMode);

	std::vector<float>* getEmptySamplesWorkarea();
	std::vector<float>* getEmptySecondSamplesWorkarea();

	void setSelectedWtPos(int wtPos);
	int getSelectedWtPos() const;
	bool isMultiSelected() const;
	int getMultiSelectBegin() const;
	int getMultiSelectEnd() const;
	void multiSelectAll();
	void clearMultiSelect();
	void setMultiSelect(int wtPos);
	void setSelection(int begin, int end);
	bool loadWavFile(wavFile* wavfile, bool preGenerate, int wtMode);
	
	static void getNaiveSamplesFromWave(std::vector<float> &nsamples, int wave);
	std::vector<float> getSyncNaiveWave(std::vector<float> &naive, float paramvalue);
	std::vector<float> calcSyncNaiveWave(int wtPos, int naiveWtPos, float syncQuota);
	std::vector<float> getPWMNaiveWave(std::vector<float> &naive, float paramvalue);
	std::vector<float> calcPWMNaiveWave(int wtPos, int naiveWtPos, float pwmQuota);
	
	bool m_multiSelect = false;
	int m_iMultiSelectBegin = 0;
	int m_iMultiSelectEnd = 0;
	int m_iSelectedPosition = 0;
	int m_iWaveTableID = 0;
	int getID() const;
	int getChangeCounter() const;
	void copyUIFXUpdates();

	//make private
	float m_fLastWTPosPerc_begin[C_MAX_POLY] = { 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f }; //target at end of cycle
	float m_fLastWtFXVal_begin[C_MAX_POLY] = { 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f }; //target at end of cycle
	int m_iLastWtFXType_begin[C_MAX_POLY] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }; //target at end of cycle
	float m_fLastWTPosPerc_end[C_MAX_POLY] = { 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f }; //target at end of cycle
	float m_fLastWtFXVal_end[C_MAX_POLY] = { 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f }; //target at end of cycle
	int m_iLastWtFXType_end[C_MAX_POLY] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }; //target at end of cycle
	CVASTSettings* m_Set;
	ValueTree bufferedValueTree; //for save state performance optimization

private:	
	sWaveTableHeader wtheader;
	std::vector<float> mc_emptyTable = std::vector<float>(C_WAVE_TABLE_SIZE, 0.0f);
	std::vector<dsp::Complex<float>> mc_newBuffer = std::vector<dsp::Complex<float>>(C_WAVE_TABLE_SIZE, 0.0f);

	void copyFrom(const CVASTWaveTable &wavetable);
	bool positionIsPrepared(int wtPos) const;
	void createFreqsIfNeeded(int wtPos, bool preGenerate, int wtMode);  //created means that naive is there and freqs - but not necessarily samples - pregenerated m,eans with samples
	bool getWaveTablePosition(int wtPos, sWaveTablePosition* &waveTablePosition); //not const
	bool getWaveTablePosition(int wtPos, sWaveTablePosition* &waveTablePosition, sWaveTablePosition* &waveTablePositionNextPos, bool &hasNextFrame); //not const
	void generateWaveTableFreqsFromTimeDomain(int wtPos, int tableLen, const std::vector<float> &waveSamples, bool preGenerate, int wtMode);
	void makeWaveTableFreq(int wtPos, int len, float bottomFreq, float topFreq, int maxHarmonics, bool change, int tableindex, bool normalize, int wtMode, float wtFxVal, int wtFxType, float curFreq);
	void addWaveTableFreq(int wtPos, int len, std::vector<float> &waveTableIn, float bottomFreq, float topFreq, int maxHarmonics, bool isInvalid, bool dirty, float wtFxVal, int wtFxType, bool buffer, std::vector<float> &naiveTableFXBufferCopy);
	void changeWaveTableFreq(int wtPos, int tableindex, int len, std::vector<float> &waveTableIn, float bottomFreq, float topFreq, int maxHarmonics, bool invalid, bool dirty, float wtFxVal, int wtFxType);
	void putWaveTableFreqToBuffer(sWaveTablePosition* wtp, int tableindex);
	int getNumWaveTableFreqs(int wtPos); //not const
	bool getWavetableFreq(int wtPos, int freqindex, float* bottomFreq, float* topFreq);
	sWaveTablePosition emptyPosition();	
	bool fillInterpolateBuffersRange(const bool next, sWaveTablePosition* wtp, float* xo, float* y0, float* y1, CVASTWaveTableOscillator* mOscillator, const int osciCount, const int startSample, const int numSamples, AudioSampleBuffer* posBuffer, const bool bInverter, int wtMode, float wtFxVal, int wtFxType, int voiceNo, int bank, bool updateUI, bool &updated);

	std::vector<float> naiveWaveFromWavFile(wavFile* wavfile, int wtPos);
	bool validate();

	CriticalSection mWavetableChangeLock;

	int m_lastWaveTableIdx = 0; //performance optimize
	int m_lastWaveTableIdxNext = 0; //performance optimize
	double m_lastPhaseInc = 0; //performance optimize

    std::unique_ptr<AudioSampleBuffer> m_x0_curPos;
    std::unique_ptr<AudioSampleBuffer> m_y0_curPos;
    std::unique_ptr<AudioSampleBuffer> m_y1_curPos;
    std::unique_ptr<AudioSampleBuffer> m_x0_nextPos;
    std::unique_ptr<AudioSampleBuffer> m_y0_nextPos;
    std::unique_ptr<AudioSampleBuffer> m_y1_nextPos;
    std::unique_ptr<AudioSampleBuffer> m_begin_curPosBuffer;
    std::unique_ptr<AudioSampleBuffer> m_begin_curPosBufferNextWtfxtype;
    std::unique_ptr<AudioSampleBuffer> m_begin_nextPosBuffer;
    std::unique_ptr<AudioSampleBuffer> m_begin_nextPosBufferNextWtfxtype;
    std::unique_ptr<AudioSampleBuffer> m_end_curPosBuffer;
    std::unique_ptr<AudioSampleBuffer> m_end_curPosBufferNextWtfxtype;
    std::unique_ptr<AudioSampleBuffer> m_end_nextPosBuffer;
    std::unique_ptr<AudioSampleBuffer> m_end_nextPosBufferNextWtfxtype;
    std::unique_ptr<AudioSampleBuffer> m_oscBuffer;

    std::unique_ptr<AudioSampleBuffer> m_valBeginBuffer;
    std::unique_ptr<AudioSampleBuffer> m_valBeginBufferNext;
    std::unique_ptr<AudioSampleBuffer> m_valEndBuffer;
    std::unique_ptr<AudioSampleBuffer> m_valEndBufferNext;

	float* m_x0_curPos_writePointer = nullptr;
	float* m_y0_curPos_writePointer = nullptr;
	float* m_y1_curPos_writePointer = nullptr;
	float* m_x0_nextPos_writePointer = nullptr;
	float* m_y0_nextPos_writePointer = nullptr;
	float* m_y1_nextPos_writePointer = nullptr;
	float* m_nextPosBuffer_writePointer = nullptr;
	float* m_curPosBuffer_writePointer = nullptr;
	
    std::unique_ptr<AudioSampleBuffer> m_fracPartBuffer;

	JUCE_LEAK_DETECTOR(CVASTWaveTable)
};
