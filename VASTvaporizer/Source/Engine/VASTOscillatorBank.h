/*
VAST Dynamics Audio Software (TM)
*/

#pragma once

#include "VASTEngineHeader.h"
#include "VASTSettings.h"
#include "VASTSettings.h"
#include "Oscillator/VASTWaveTable.h"

class VASTAudioProcessor; //forward declaration
struct sWaveTableFreq; //forward declaration
struct sWaveTablePosition; //forward declaration
struct sWaveTableHeader; //forward declaration
class CVASTOscillatorBank : public Timer {

public:
	CVASTOscillatorBank(CVASTSettings* set, VASTAudioProcessor* processor, int bankno);
	~CVASTOscillatorBank();
	void init();
	void setWavetable(std::shared_ptr<CVASTWaveTable> wavetable);
	
	void setWavetableSoftFade(std::shared_ptr<CVASTWaveTable> wavetable);
	void beginSoftFade();
	bool endSoftFade(); //returns if done

	std::shared_ptr<CVASTWaveTable> getNewSharedWavetable();
	const std::shared_ptr<CVASTWaveTable>& getWavetablePointer();

	std::shared_ptr<CVASTWaveTable> getNewSharedSoftFadeWavetable();
	const std::shared_ptr<CVASTWaveTable>& getSoftFadeWavetablePointer();

	std::shared_ptr<CVASTWaveTable> getNewSharedSoftFadeWavetableNext();
	const std::shared_ptr<CVASTWaveTable>& getSoftFadeWavetablePointerNext();

	void recalcWavetable();

	void prepareForPlay(int expectedSamplesPerBlock);

	int getBankno();
	
	void setChangedFlagOsc();
	void setChangedFlag();
	bool isChanged();
	bool getAndClearSoftChangedFlagStructure();
	bool getAndClearSoftChangedFlagFdv();
	bool getAndClearSoftChangedFlagOsc();
	bool getAndClearSoftChangedFlagOscEditor();
	bool getAndClearSoftChangedFlagPos();
	
	void setSoloMode(bool solo);
	bool getSoloMode();

	std::shared_ptr<CVASTWaveTable> getSoftOrCopyWavetable(bool getCopy = false, bool copyAlsoFreqs = false); //returns a reference of an exisitng shared_ptr - not a new one - has to be ensured that SFNext is not used

	CriticalSection mSharedPtrSoftFadeLock;

	//https://stackoverflow.com/questions/11666610/how-to-give-priority-to-privileged-thread-in-mutex-locking
	void addSoftFadeEditor();
	void removeSoftFadeEditor();

	int m_numEditingSoftFadeNext = 0;
	
	void timerCallback() override;
	int lastWTchangeCounter = -1;
	bool needsUndo();
	void undoLastWTChange();
	
	void addSingleNoteSoftFadeCycle(int voiceNo);
	void removeSingleNoteSoftFadeCycle(int voiceNo);
	int isInSingleNoteSoftFadeCycle();
	void clearSingleNoteSoftFadeCycle();
	std::atomic<bool> m_iSingleNoteSoftFadeCycle[C_MAX_POLY] = { false, false, false, false, false, false, false, false,
																 false, false, false, false, false, false, false, false };

	std::atomic<bool> m_bWavetableSoftfadeStillNeeded = false;
	std::atomic<bool> m_bWavetableSoftfadePickedUp = false;

	int m_iSingleNoteSoftFadeID = 0;
	void startRecording(int wtPos);
	void stopRecording();

private:
	std::shared_ptr<CVASTWaveTable> m_wavetable; // wavetable reference to buffer
	std::shared_ptr<CVASTWaveTable> m_wavetable_soft_fade; // wavetable reference to buffer
	std::shared_ptr<CVASTWaveTable> m_wavetable_soft_fade_next; // wavetable reference to buffer
	std::shared_ptr<CVASTWaveTable> m_wavetable_undo_buffered; 
	std::shared_ptr<CVASTWaveTable> m_wavetable_undo_buffered_before;

	VASTAudioProcessor* myProcessor;
	CVASTSettings* m_Set; 
	int m_bankno = 0;

	std::atomic<bool> m_bWtSoftChangedFdv = true;
	std::atomic<bool> m_bWtSoftChangedOsc = true;
	std::atomic<bool> m_bWtSoftChangedOscEditor = true;
	std::atomic<bool> m_bWtSoftChangedStructure = true;
	std::atomic<bool> m_bWtSoftChangedPos = true;
	std::atomic<bool> m_bIsRecording = false;
	std::atomic<int> m_iRecordingPos = -1;

	std::atomic<bool> m_soloMode = false;
	std::atomic<bool> m_bSoftFadeSinceLastUndoCheck = true;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CVASTOscillatorBank)
};
