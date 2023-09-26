/*
VAST Dynamics Audio Software (TM)
*/

#pragma once
#include "../../Filter/VASTDelay.h"
#include "VASTDelayLine.h"

// derived class of CDelay
class CDelayLineAPF : public CDelayLine
{
public:
	// constructor/destructor
	CDelayLineAPF(void);
	~CDelayLineAPF(void);

	// members
protected:
	float m_fAPF_g; // one g coefficient

public:
	// set our g value
	void setAPF_g(float fAPFg){ m_fAPF_g = fAPFg; }

	// overrides
	bool processAudio(float* pInput, float* pOutput);

};
