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
	friend class CVASTXperience;
	friend class CVASTSingleNote;

public:
	CVASTPoly(CVASTSettings& set, VASTAudioProcessor* processor);

	virtual ~CVASTPoly(void);

	VASTAudioProcessor* getProcessor() {
		return myProcessor;
	};
	void init();
	void releaseResources();
	void prepareForPlay();
	void updateVariables();
	void updateLFO(int lfono);
	void processAudioBuffer(sRoutingBuffers& routingBuffers, MidiBuffer& midiMessages);
	void resynchLFO();
	bool isVoicePlaying(int voiceNo) const;
	int numNotesPlaying() const;
	int numOscsPlaying() const;
	int getLastNotePlayed() const;
	int getOldestNotePlayed() const;
	void stopAllNotes(bool allowTailOff);
	bool voicesMSEGStillActive();	
	modMatrixInputState getLastNotePlayedInputState(int currentFrame);
	modMatrixInputState getOldestNotePlayedInputState(int currentFrame);

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

	std::atomic<double> m_samplerViewportPosMarker[C_MAX_SAMPLER_VIEWPORT_MARKERS]{}; //max markers that are displayed
	std::atomic<int> m_samplerViewportPosMarkerCount = 0;
	std::atomic<float>m_currentWTPosFloatPercentage[4][C_MAX_POLY]{}; //for oscilloscope, per bank
	std::atomic<float>m_safePhaseFloat[4][C_MAX_POLY]{}; //for oscilloscope, per bank
	std::atomic<float>m_fLastLFOOscValue[5][C_MAX_POLY]; //for LFO editor, per LFO1-5
	std::atomic<float>m_fLastGlobalLFOOscValue[5]; //for LFO editor, per LFO1-5

	LinearSmoothedValue<float> m_fCustomModulator1_smoothed;
	LinearSmoothedValue<float> m_fCustomModulator2_smoothed;
	LinearSmoothedValue<float> m_fCustomModulator3_smoothed;
	LinearSmoothedValue<float> m_fCustomModulator4_smoothed;
	LinearSmoothedValue<float> m_fARP_Speed_smoothed;

    void setKeyboardHoldMode(bool keyboardHoldMode);
    atomic<bool> m_keyboardHoldMode = false;
    
	float m_ARP_speed = 0.0f;

	CVASTSettings * m_Set;
	void initArp();
	SortedSet<int> m_ARP_midiInNotes;

private:
	VASTAudioProcessor* myProcessor;

	CVASTSingleNote* m_singleNote[C_MAX_POLY]; //pointer since it will be stored as voices in synthesiers ownned array // must not be accessed directly from the UI, not thread safe

	/** This is used to control access to the rendering callback and the note trigger methods. */
	CriticalSection lock;
	bool m_ppq_playing = false;			
	double m_last_bpm = 0.0;
	bool m_shallInitARP = false;
	bool m_arpHasActiveStepToFinish = false;
	double m_dLastRealPos = 0.0;
	void initArpInternal(MidiBuffer& midiMessages);	
    std::atomic<int> m_lastInitPoly = 0;

	void doArp(sRoutingBuffers &routingBuffers, MidiBuffer& midiMessages);	
	SortedSet<int> m_ARP_currentARPNoteValues;
	
	int m_ARP_currentNote, m_ARP_currentStep, m_ARP_direction = 0;
	double m_ARP_time = 0;
	
	VASTSynthesiser m_OscillatorSynthesizer{ myProcessor };

	JUCE_LEAK_DETECTOR(CVASTPoly)
};
