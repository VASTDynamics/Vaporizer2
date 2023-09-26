/*
VAST Dynamics Audio Software (TM)
*/

#pragma once
#include "VASTDelay.h"


// derived class: CDelay does most of the work
class CCombFilter : public CDelay
{
public:
	// constructor/destructor
	CCombFilter(void);
	~CCombFilter(void);

	// members
protected:
	float m_fComb_g; // one coefficient

public:
	// set our g value directly
	void setComb_g(float fCombg){m_fComb_g = fCombg;}

	// set gain using RT60 time
	void setComb_g_with_RTSixty(float fRT)
	{
		float fExponent = -3.0*m_fDelayInSamples*(1.0/m_nSampleRate);
		fRT /= 1000.0; // RT is in mSec!

		m_fComb_g = pow((float)10.0, fExponent/fRT);
	}

	// do some audio processing
	bool processAudio(float* pInput, float* pOutput);
};
