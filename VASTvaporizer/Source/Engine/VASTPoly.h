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
	void stopAllNotes(bool allowTailOff);
	bool voicesMSEGStillActive();
	modMatrixInputState getLastNotePlayedInputState(int currentFrame);
	modMatrixInputState getOldestNotePlayedInputState(int currentFrame);

	CVASTSingleNote* m_singleNote[C_MAX_POLY]; //pointer since it will be stored as voices in synthesiers ownned array

	CVASTMSEGEnvelope m_StepSeq_Envelope[3]; //not per voice

	CVASTWaveTableOscillator m_global_LFO_Osc[5]; // Global LFOs 1-5

	VASTQFilter m_QFilter;

	//std::vector<std::shared_ptr<CVASTOscillatorBank>> m_OscBank;
	OwnedArray<CVASTOscillatorBank> m_OscBank;

	VASTSynthesiser* getSynthesizer();
	VASTSynthesiserSound* getSynthSound();
	VASTSamplerSound* getSamplerSound();
	VASTSamplerSound* getSamplerSoundChanged();
	void clearSamplerSoundChanged();
	void softFadeExchangeSample();
	bool getLastSingleNoteCycleWasActive();
	int m_iLastSingleNoteCycleCalls = 0;

	LinearSmoothedValue<float> m_fCustomModulator1_smoothed;
	LinearSmoothedValue<float> m_fCustomModulator2_smoothed;
	LinearSmoothedValue<float> m_fCustomModulator3_smoothed;
	LinearSmoothedValue<float> m_fCustomModulator4_smoothed;
	LinearSmoothedValue<float> m_fARP_Speed_smoothed;

	float m_ARP_speed = 0.0f;

	CVASTSettings * m_Set;
	void initArp();
	SortedSet<int> m_ARP_midiInNotes;

private:
	VASTAudioProcessor* myProcessor;

	/** This is used to control access to the rendering callback and the note trigger methods. */
	CriticalSection lock;
	bool m_ppq_playing = false;			
	double m_last_bpm = 0.0;
	bool m_shallInitARP = false;
	bool m_arpHasActiveStepToFinish = false;
	double m_dLastRealPos = 0.0;
	void initArpInternal(MidiBuffer& midiMessages);	

	void doArp(sRoutingBuffers &routingBuffers, MidiBuffer& midiMessages);	
	SortedSet<int> m_ARP_currentARPNoteValues;
	
	int m_ARP_currentNote, m_ARP_currentStep, m_ARP_direction;
	double m_ARP_time;
	
	VASTSynthesiser m_OscillatorSynthesizer;

	JUCE_LEAK_DETECTOR(CVASTPoly)
};