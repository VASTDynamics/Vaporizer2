/*
VAST Dynamics Audio Software (TM)
*/

#pragma once
#include "../VASTEngineHeader.h"
#include "VASTDelay.h"

// derived class of CDelay
class CDelayAPF : public CDelay
{
public:
	// constructor/destructor
	CDelayAPF(void);
	~CDelayAPF(void);

	// members
protected:
	float m_fAPF_g; // one g coefficient

public:
	// set our g value
	void setAPF_g(float fAPFg);

	// overrides
	bool processAudio(float* pInput, float* pOutput);

};
