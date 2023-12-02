/*
VAST Dynamics Audio Software (TM)
*/

#pragma once

#include "../../VASTPluginConstants.h"
#include "../../VASTSettings.h"

// abstract base class for DSP filters
class CDDLModule
{
public:
	CDDLModule(void);

	// 2. One Time Destruction
	virtual ~CDDLModule(void);

	// 3. The Prepare For Play Function is called just before audio streams
	bool prepareForPlay(int iSampleRate, bool bOversampling);

	// 4. processAudioFrame() processes an audio input to create an audio output
	bool processAudioFrame(float* pInputBuffer, float* pOutputBuffer, MYUINT uNumInputChannels, MYUINT uNumOutputChannels);

	void init(CVASTSettings &set);

	float m_fDelayInSamples;
	float m_fFeedback;
	float m_fWetLevel;

	float m_fDDLOutput; // added for dimension chorus

	void cookVariables();
	void resetDelay(int nDelayLength);
	
    std::unique_ptr<AudioSampleBuffer> m_pBuffer;

	int m_nReadIndex;
	int m_nWriteIndex;

	bool  m_bUseExternalFeedback; // flag for enabling/disabling
	float m_fFeedbackIn;		// the user supplied feedback sample value

	// current FB is fb*output
	float getCurrentFeedbackOutput();

	// set the feedback sample
	void  setCurrentFeedbackInput(float f);

	// enable/disable external FB source
	void  setUsesExternalFeedback(bool);

	float m_fDelay_ms;
	float m_fFeedback_pct;
	float m_fWetLevel_pct;
	CVASTSettings *m_Set;
    bool m_bOversampling = false;
    int m_iSampleRate = 44100;
};
