/*
VAST Dynamics Audio Software (TM)
*/

#pragma once
#include "../VASTPluginConstants.h"

class COnePoleLPF
{
public:
	// constructor/Destructor
	COnePoleLPF(void);
	~COnePoleLPF(void);

	// members
protected:
	float m_fLPF_g; // one gain coefficient
	float m_fLPF_z1; // one delay

public:

	// set our one and only gain coefficient
	void setLPF_g(float fLPFg);

	// function to init
	void init();

	// function to process audio
	bool processAudio(float* pInput, float* pOutput);

};
