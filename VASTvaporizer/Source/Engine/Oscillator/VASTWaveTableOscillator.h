/*
VAST Dynamics Audio Software (TM)
*/

#pragma once

//#define doLinearInterp 1
#include <vector>
#include "../VASTSettings.h"
#include "VASTWaveTable.h"
#include "../Filter/VASTBiQuadFilter.h"
#include "../VASTOscillatorBank.h"

#define M_MAX_DELTA_STEPS 30 //fade in - 300?
#define C_WRAP_WINDOW_BEFORE_AND_AFTER_SAMPLES 4

struct sWaveTableFreq; //forward declaration
struct sWaveTablePosition; //forward declaration
struct sWaveTableHeader; //forward declaration
class CVASTOscillatorBank; //forward declaration
class CVASTWaveTableOscillator {
public:
	CVASTWaveTableOscillator(void);
	~CVASTWaveTableOscillator(void);

	void syncPhasor(int unisonOsci);
	//inline float getPhasor() { return phasor; };
	//inline float getPhaseInc() { return phaseInc; };

	void prepareForPlay(int expectedSamplesPerBlock);

	bool m_bNoteOn;
	void init(CVASTSettings &set, CVASTOscillatorBank* oscBank);
	void init(CVASTSettings &set);
	bool getOscillation(float* pOutput);

	void doWavetableBufferInit(int unisonOsci, sRoutingBuffers& routingBuffers, modMatrixInputState& inputState);
	bool doWavetableBufferStep(int unisonOsci, int currentFrame, float fPhaseOffset);

	// invert output
	bool m_bInvert;
	// reset the read index
	void resynch(int unisonOsci, bool lfo);
	void resynchWithFade(bool lfo);
	//float m_fLastBLEP = 0;

	//resynch fader
	MYUINT m_i_inc_delta_steps;
	MYUINT m_i_max_steps;
	MYUINT m_i_resynch_fade_steps;

	void updateMainVariables(int samplerate, MYUINT OscType, int unisonOscis, float OscTune, int OscOctave, float OscCents);

	//void setDetuneMod(float OscDetuneMod);
	void setCents(float OscCents);
	void setPitchBendNote(float pitchbend);
	void setPitchBendZone(float pitchbend);

	void setDetune(int unisonOsci, float OscDetune, bool updateFrequency);
	void setFrequency(int unisonOsci, bool restart);
	bool updatePhase(int unisonOsci);
	float drift_noise(float& lastval);

	MYUINT getOscType() const;
	void noteOn(MYUINT uChannel, MYUINT uMIDINote, MYUINT uVelocity);
	void noteOff();

	void startLFOFrequency(float LFOFreq, int LFOno);
	//void retriggerLFO();

	void updatePitchMod(float pitchMod);
	void updateNoiseBiquad(int skips);

	bool isPlaying();
	float m_fLastValue = 0.0f;
	void reset(int unisonOsci); // public only for osc sync!
	bool m_bResetAfter = false;

	MYUINT m_uPolarity; //needed?
		
	CVASTBiQuad m_noiseBiQuadLow;
	CVASTBiQuad m_noiseBiQuadHigh;

	void setPortamentoTime(float time);
	void setPortamentoStart(int midinote, bool reset);
	void doPortamentoIfRequired(int skips);

	void setFMFreq(float fmFreq);

	void resetSmoothers();

    std::unique_ptr<AudioSampleBuffer> m_phasedPhasorBuffer[C_MAX_PARALLEL_OSC];
	float* m_phasedPhasorBufferPointer[C_MAX_PARALLEL_OSC];
    std::unique_ptr<AudioSampleBuffer> m_phaseIncBuffer[C_MAX_PARALLEL_OSC];
	float* m_phaseIncBufferPointer[C_MAX_PARALLEL_OSC];
	int m_unisonOscis = 1;
	CVASTSettings *m_Set; 
	
	void syncAllPhasorsToMaster();
    float doWhiteNoiseFast();
    
protected:
	// this should be protected
	float phasor[C_MAX_PARALLEL_OSC];       // phase accumulator
	float phaseInc[C_MAX_PARALLEL_OSC];     // phase increment

private:
	void random_retrig(int unisonOsci);
    
	long unsigned int rand31_next();
	
	bool getOsci(float* pOutput); //int inFramesToProcess required to detect small variable buffersizes (FLStudio)
	
	//unison parameters
	float m_fOscDetune[C_MAX_PARALLEL_OSC];
	float m_fPlayedFrequency_Hz[C_MAX_PARALLEL_OSC];
	//float m_fExciterDetune[C_MAX_PARALLEL_OSC];
	float m_fDriftLfo[C_MAX_PARALLEL_OSC];
	float m_fDriftLfo2[C_MAX_PARALLEL_OSC];

    int m_whiteNoisePhasor = 0;
    
	//global parameters
	bool m_bIsPlaying;
	bool m_bNoteOnNextZero;
	MYUINT m_uMIDINote;
	float m_fBaseFrequency_Hz = 0.0;	
	MYUINT m_uOscType;
	float m_fOscMasterTune = 0.0f;
	int m_iOscOctave = 0;
	float m_fOscCents = 0.0f;
	//float m_fOscDetuneMod = 0.0f;
	float m_fPitchBendNote = 1.0f;
	float m_fPitchBendZone = 1.0f;
	float m_fOscPitchMod = 1.0f;
	float m_fPortamento = 0.f;
	
	MYUINT m_iPolyMode;
	bool m_bLFO = false;
	int m_LFOno = 0;
	float m_LFORamp = 1.f;

	CVASTOscillatorBank* m_oscBank;

    int m_noise_skips = 0;
	long m_seed = 0; // for random
	float m_fRsHoldValue = 0.0f; // sample & hold
	float m_fRshCounter = -1.0f;
	
	int m_iSampleRate = 44100; //is properly set in update variables
	
	LinearSmoothedValue<float> m_fBaseFreqPortamento_smoothed;
	LinearSmoothedValue<float> m_fNoiseLowCut_smoothed;
	LinearSmoothedValue<float> m_fNoiseHighCut_smoothed;
	LinearSmoothedValue<float> m_fNoiseResonance_smoothed;

	bool m_bSmoothersTakeNextValue = false;	

	//noise
	float m_y, m_z, m_buf0, m_buf1, m_buf2, m_buf3, m_buf4, m_buf5, m_buf6;
	float m_fNoiseLowCut = 0.f;
	float m_fNoiseHighCut = 0.f;
	float m_fNoiseResonance = 0.f;
	float m_fFMFreq = 0.f;
	JUCE_LEAK_DETECTOR(CVASTWaveTableOscillator)
};
