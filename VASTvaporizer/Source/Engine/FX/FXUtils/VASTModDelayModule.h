/*
VAST Dynamics Audio Software (TM)
*/

#pragma once

#include "../../VASTPluginConstants.h"
#include "../../VASTSettings.h"
#include "VASTDDLModule.h"
#include "../../Oscillator/VASTWaveTableOscillator.h"  

// abstract base class for DSP filters
class CModDelayModule
{
public:	// Plug-In API Functions
	// Plug-In API Member Methods:
	// The followung 5 methods must be impelemented for a meaningful Plug-In
	//
	// 1. One Time Initialization
	CModDelayModule(CVASTSettings& set);

	// 2. One Time Destruction
	virtual ~CModDelayModule(void);

	// 3. The Prepare For Play Function is called just before audio streams
	bool prepareForPlay(int sampleRate, bool bOversampling);

	// 4. processAudioFrame() processes an audio input to create an audio output
	bool processAudioFrame(float* pInputBuffer, float* pOutputBuffer, MYUINT uNumInputChannels, MYUINT uNumOutputChannels);
	
	void init();

	CVASTWaveTableOscillator m_LFO; 	// our LFO
	CDDLModule m_DDL;	// our delay line module

	// these will depend on the type of mod
	float m_fMinDelay_mSec;
	float m_fMaxDelay_mSec;

	// functions to update the member objects
	void updateLFO();
	void updateDDL();

	// cooking function for mod type
	void cookModType();

	// convert a LFO value to a delay offset value
	float calculateDelayOffset(float fLFOSample);

	float m_fDDLOutput; // added for dimension chorus
	float m_fWetLevel;

	float m_fModDepth_pct;
	float m_fModFrequency_Hz;
	float m_fFeedback_pct;
	float m_fChorusOffset;
	MYUINT m_uModType;
	enum{Flanger,Vibrato,Chorus};
	MYUINT m_uLFOType;
	enum{tri,sin};
	MYUINT m_uLFOPhase;
	//enum{normal,quad,invert};
	MYUINT m_uTZF;
	enum{OFF,ON};

private:
	CVASTSettings *m_Set;
	int m_iSampleRate = 44100;
    bool m_bOversampling = false;
};
















