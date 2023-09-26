/*
VAST Dynamics Audio Software (TM)

Handles and dispatches multiple single notes
Has LFO
- One LFO for all poly notes
- Does ARP
*/

#pragma once

#include "VASTSingleNote.h"
#include "VASTSettings.h"
#include "Oscillator/VASTWaveTableOscillator.h"
#include "VASTOscillatorBank.h"
#include "VASTSynthesiser.h"
#include "Filter/VASTQFilter.h"
#include <vector>

class CVASTPoly
{
public:
	CVASTPoly(CVASTSettings &set, VASTAudioProcessor* processor) :
		m_Set(&set), myProcessor(processor)
	{
		for (int i = 0; i < 4; i++) {
			CVASTOscillatorBank* bank = new CVASTOscillatorBank(m_Set, myProcessor, i);
			//m_OscBank.push_back(std::make_shared<CVASTOscillatorBank>(bank));
			m_OscBank.add(bank);
		}
	};

	virtual ~CVASTPoly(void);

	VASTAudioProcessor* getProcessor() {
		return myProcessor;
	};
	void init();
	void prepareForPlay();
	void updateVariables();
	void updateLFO(int lfono);
	void processAudioBuffer(sRoutingBuffers& routingBuffers, MidiBuffer& midiMessages);
	void resynchLFO();
	int numNotesPlaying();
	int numOscsPlaying();
	int getLastNotePlayed();
	int getOldestNotePlayed();
	void stopAllNotes(bool allowTailOff) {
		for (int i = 0; i < C_MAX_POLY; i++) {
			m_singleNote[i]->stopNote(0, allowTailOff);
		}
	};
	bool voicesMSEGStillActive() {
		for (int i = 0; i < C_MAX_POLY; i++) {
			if (m_singleNote[i]->m_VCA->isActive()) return true;
		}
		return false;
	}
	modMatrixInputState getLastNotePlayedInputState(int currentFrame);
	modMatrixInputState getOldestNotePlayedInputState(int currentFrame);

	CVASTSingleNote* m_singleNote[C_MAX_POLY]; //pointer since it will be stored as voices in synthesiers ownned array

	CVASTMSEGEnvelope m_StepSeq_Envelope[3]; //not per voice

	CVASTWaveTableOscillator m_global_LFO_Osc[5]; // Global LFOs 1-5

	VASTQFilter m_QFilter;

	//std::vector<std::shared_ptr<CVASTOscillatorBank>> m_OscBank;
	OwnedArray<CVASTOscillatorBank> m_OscBank;

	VASTSynthesiser* getSynthesizer() { return &m_OscillatorSynthesizer; };
	VASTSynthesiserSound* getSynthSound() {
		VASTSynthesiserSound* sound = (VASTSynthesiserSound*)getSynthesizer()->getSound(0);
		return sound;
	}

	VASTSamplerSound* getSamplerSound() { //live Data
		VASTSynthesiserSound* sound = (VASTSynthesiserSound*)getSynthesizer()->getSound(0);
		return sound->getSamplerSound();
	};

	VASTSamplerSound* getSamplerSoundChanged() {
		VASTSynthesiserSound* sound = (VASTSynthesiserSound*)getSynthesizer()->getSound(0);
		return sound->getSamplerSoundChanged();
	};
	void clearSamplerSoundChanged() {
		VASTSynthesiserSound* sound = (VASTSynthesiserSound*)getSynthesizer()->getSound(0);
		sound->clearSamplerSoundChanged();
	}

	void softFadeExchangeSample() {
		VASTSynthesiserSound* sound = getSynthSound();
		if (sound != nullptr)
			sound->softFadeExchangeSample();
	}

	int m_iLastSingleNoteCycleCalls = 0;
	bool getLastSingleNoteCycleWasActive() {
		return m_iLastSingleNoteCycleCalls > 0;
	}

	LinearSmoothedValue<float> m_fCustomModulator1_smoothed;
	LinearSmoothedValue<float> m_fCustomModulator2_smoothed;
	LinearSmoothedValue<float> m_fCustomModulator3_smoothed;
	LinearSmoothedValue<float> m_fCustomModulator4_smoothed;
	LinearSmoothedValue<float> m_fARP_Speed_smoothed;

	float m_ARP_speed = 0.0f;

	CVASTSettings * m_Set;
	void initArp() {
		m_shallInitARP = true;
	};
	SortedSet<int> m_ARP_midiInNotes;

private:
	VASTAudioProcessor* myProcessor;

	/** This is used to control access to the rendering callback and the note trigger methods. */
	CriticalSection lock;
	bool m_ppq_playing = false;		
	double m_last_bpm = 0.0;
	bool m_shallInitARP = false;
	bool m_arpHasActiveStepToFinish = false;
	void initArpInternal(MidiBuffer& midiMessages);	

	void doArp(sRoutingBuffers &routingBuffers, MidiBuffer& midiMessages);	
	SortedSet<int> m_ARP_currentARPNoteValues;
	
	int m_ARP_currentNote, m_ARP_currentStep, m_ARP_direction;
	double m_ARP_time;
	
	VASTSynthesiser m_OscillatorSynthesizer;

	JUCE_LEAK_DETECTOR(CVASTPoly)
};