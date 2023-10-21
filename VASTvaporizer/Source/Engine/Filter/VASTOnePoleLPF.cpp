/*
VAST Dynamics Audio Software (TM)
*/

#include "VASTOnePoleLPF.h"
#ifdef DEBUG
	#define _CRTDBG_MAP_ALLOC
#endif

COnePoleLPF::COnePoleLPF(void)
{
	m_fLPF_g = 0;
	m_fLPF_z1 = 0;
}

COnePoleLPF::~COnePoleLPF(void)
{
}


// set our one and only gain coefficient

void COnePoleLPF::setLPF_g(float fLPFg) { m_fLPF_g = fLPFg; }

void COnePoleLPF::init()
{
	m_fLPF_z1 = 0.0;
}

bool COnePoleLPF::processAudio(float* pInput, float* pOutput)
{
	// read the delay line to get w(n-D); call base class
	// read
	float yn_LPF = *pInput*(1.0f - m_fLPF_g) + m_fLPF_g*m_fLPF_z1;
	//float yn_LPF = *pInput*(m_fLPF_g) + (1.0 - m_fLPF_g)*m_fLPF_z1;
	
	// this just reverses the slider 
	//float yn_LPF = *pInput*(m_fLPF_g) + (1.0 - m_fLPF_g)*m_fLPF_z1;

	// underflow check
	if(yn_LPF > 0.0 && yn_LPF < FLT_MIN_PLUS) yn_LPF = 0;
	if(yn_LPF < 0.0 && yn_LPF > FLT_MIN_MINUS) yn_LPF = 0;

	// form fb & write
	m_fLPF_z1 = yn_LPF;

	*pOutput = yn_LPF;

	// all OK
	return true;
}
