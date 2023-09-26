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

	void recalcWavetable() {
		if (m_wavetable != nullptr)
			m_wavetable->markAllWTFreqsDirty();
		if (m_wavetable_soft_fade != nullptr) 
			m_wavetable_soft_fade->markAllWTFreqsDirty();
		if (m_wavetable_soft_fade_next != nullptr) 
			m_wavetable_soft_fade_next->markAllWTFreqsDirty();
	}

	void prepareForPlay(int expectedSamplesPerBlock);

	int getBankno() { return m_bankno; };
	
	void setChangedFlagOsc() {
		m_bWtSoftChangedOsc = true; //CHECK
	}

	void setChangedFlag() {
		m_bWtSoftChangedFdv = true; //CHECK
		m_bWtSoftChangedOsc = true; //CHECK
		m_bWtSoftChangedOscEditor = true; //CHECK
		m_bWtSoftChangedPos = true; //CHECK

	}; 
	bool isChanged() { 
		return
			(m_bWtSoftChangedFdv || m_bWtSoftChangedOsc || m_bWtSoftChangedOscEditor || m_bWtSoftChangedPos);
	};

	bool getAndClearSoftChangedFlagStructure() {
		bool l_changed = m_bWtSoftChangedStructure;
		m_bWtSoftChangedStructure = false;
		return l_changed;
	}

	bool getAndClearSoftChangedFlagFdv() {
		bool l_changed = m_bWtSoftChangedFdv;
		m_bWtSoftChangedFdv = false;
		return l_changed;
	};
	bool getAndClearSoftChangedFlagOsc() {
		bool l_changed = m_bWtSoftChangedOsc;
		m_bWtSoftChangedOsc = false;
		return l_changed;
	};
	bool getAndClearSoftChangedFlagOscEditor() {
		bool l_changed = m_bWtSoftChangedOscEditor;
		m_bWtSoftChangedOscEditor = false;
		return l_changed;
	};
	bool getAndClearSoftChangedFlagPos() {
		bool l_changed = m_bWtSoftChangedPos;
		m_bWtSoftChangedPos = false;
		return l_changed;
	};
	
	void setSoloMode(bool solo) {
		m_soloMode = solo;
		m_bWtSoftChangedOsc = true;
	}
	bool getSoloMode() {
		return m_soloMode;
	}

	std::shared_ptr<CVASTWaveTable> getSoftOrCopyWavetable(bool getCopy = false, bool copyAlsoFreqs = false); //returns a reference of an exisitng shared_ptr - not a new one - has to be ensured that SFNext is not used

	CriticalSection mSharedPtrSoftFadeLock;

	//https://stackoverflow.com/questions/11666610/how-to-give-priority-to-privileged-thread-in-mutex-locking
	void addSoftFadeEditor() { 
		m_numEditingSoftFadeNext++; };
	void removeSoftFadeEditor() { 
		m_numEditingSoftFadeNext--; };

	int m_numEditingSoftFadeNext = 0;
	
	void timerCallback() override;
	int lastWTchangeCounter = -1;
	bool needsUndo();
	void undoLastWTChange();
	
	void addSingleNoteSoftFadeCycle(int voiceNo) {
		m_iSingleNoteSoftFadeCycle[voiceNo].store(true);
		//DBG("addSingleNoteSoftFadeCycle SingleNoteSoftFadeCycle now " + String((isInSingleNoteSoftFadeCycle())));
	}
	void removeSingleNoteSoftFadeCycle(int voiceNo) {
		m_iSingleNoteSoftFadeCycle[voiceNo].store(false);
		//DBG("removeSingleNoteSoftFadeCycle SingleNoteSoftFadeCycle now " + String((isInSingleNoteSoftFadeCycle())));
	}
	int isInSingleNoteSoftFadeCycle() {
		return m_iSingleNoteSoftFadeCycle[0] + m_iSingleNoteSoftFadeCycle[1] + m_iSingleNoteSoftFadeCycle[2] + m_iSingleNoteSoftFadeCycle[3] +
			   m_iSingleNoteSoftFadeCycle[4] + m_iSingleNoteSoftFadeCycle[5] + m_iSingleNoteSoftFadeCycle[6] + m_iSingleNoteSoftFadeCycle[7] +
			   m_iSingleNoteSoftFadeCycle[8] + m_iSingleNoteSoftFadeCycle[9] + m_iSingleNoteSoftFadeCycle[10] + m_iSingleNoteSoftFadeCycle[11] +
			   m_iSingleNoteSoftFadeCycle[12] + m_iSingleNoteSoftFadeCycle[13] + m_iSingleNoteSoftFadeCycle[14] + m_iSingleNoteSoftFadeCycle[15];
	}
	void clearSingleNoteSoftFadeCycle() {
		m_iSingleNoteSoftFadeCycle->store(false); //CHECK
	}
	std::atomic<bool> m_iSingleNoteSoftFadeCycle[C_MAX_POLY] = { false, false, false, false, false, false, false, false,
																 false, false, false, false, false, false, false, false };

	std::atomic<bool> m_bWavetableSoftfadeStillNeeded = false;
	std::atomic<bool> m_bWavetableSoftfadePickedUp = false;

	int m_iSingleNoteSoftFadeID = 0;
	void startRecording(int wtPos) {
		m_bIsRecording = true;
		m_iRecordingPos = wtPos;
	};
	void stopRecording() {
		m_bIsRecording = false;		

		std::shared_ptr<CVASTWaveTable> l_wavetable = getSoftOrCopyWavetable();
		if ((m_iRecordingPos >= 0) && (l_wavetable->getNumPositions() > (m_iRecordingPos + 1))) {
			l_wavetable->deletePosition(m_iRecordingPos);
			setWavetableSoftFade(l_wavetable);
		}

		m_iRecordingPos = -1;
	};

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
