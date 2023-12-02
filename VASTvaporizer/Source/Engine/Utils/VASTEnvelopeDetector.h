/*
VAST Dynamics Audio Software (TM)
*/
#pragma once

#include "../VASTEngineHeader.h"

class VASTEnvelopeDetector
{
public:
	VASTEnvelopeDetector(void);
	~VASTEnvelopeDetector(void);

public:

	// Call the Init Function to initialize and setup all at once; this can be called as many times
	// as you want
	void init(float samplerate, float attack_in_ms, float release_in_ms, bool bAnalogTC, MYUINT uDetect, bool bLogDetector);

	// these functions allow you to change modes and attack/release one at a time during
	// realtime operation
	void setTCModeAnalog(bool bAnalogTC); // {m_bAnalogTC = bAnalogTC;}

	// THEN do these after init
	void setAttackTime(float attack_in_ms);
	void setReleaseTime(float release_in_ms);

	// Use these "codes"
	// DETECT PEAK   = 0
	// DETECT MS	 = 1
	// DETECT RMS	 = 2
	//
	void setDetectMode(MYUINT uDetect);
	void setSampleRate(float f);
	void setLogDetect(bool b);

	// call this to detect; it returns the peak ms or rms value at that instant
	float detect(float fInput);

	// call this from your prepareForPlay() function each time to reset the detector
	void prepareForPlay();

protected:
	int  m_nSample;
	float m_fAttackTime;
	float m_fReleaseTime;
	float m_fAttackTime_mSec;
	float m_fReleaseTime_mSec;
	float m_fSampleRate;
	float m_fEnvelope;
	MYUINT  m_uDetectMode;
	bool  m_bAnalogTC;
	bool  m_bLogDetector;
};

