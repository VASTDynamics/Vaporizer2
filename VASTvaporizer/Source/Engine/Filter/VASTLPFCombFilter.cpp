/*
VAST Dynamics Audio Software (TM)
*/

#include "VASTLPFCombFilter.h"
#ifdef DEBUG
	#define _CRTDBG_MAP_ALLOC
#endif

CLPFCombFilter::CLPFCombFilter(void):CDelay()
{
	m_fComb_g = 0;
	m_fLPF_g = 0;
	m_fLPF_z1 = 0;
}

CLPFCombFilter::~CLPFCombFilter(void)
{
}

// set our g value using RT60

void CLPFCombFilter::setComb_g_with_RTSixty(float fRT)
{
	float fExponent = -3.0 * m_fDelayInSamples * (1.0 / m_nSampleRate);
	fRT /= 1000.0; // RT is in mSec!

	m_fComb_g = pow((float)10.0, fExponent / fRT);
}

// set the LPF gain
// NOTE: call setComb_g_with_RTSixty FIRST, then this

void CLPFCombFilter::setLPF_g(float fOverAllGain)
{
	// g2 = g*(1-g1)
	m_fLPF_g = fOverAllGain * (1.0 - m_fComb_g);
}

void CLPFCombFilter::init(int nDelayLength)
{
	m_fLPF_z1 = 0.0;

	CDelay::init(nDelayLength);
}

bool CLPFCombFilter::processAudio(float* pInput, float* pOutput)
{
	// read the delay line to get w(n-D); call base class
	float yn = this->readDelay();

	if(m_nReadIndex == m_nWriteIndex)
		yn = 0;

	// read
	float yn_LPF = yn + m_fLPF_g*m_fLPF_z1;

	// form fb & write
	m_fLPF_z1 = yn_LPF;

	// form fb = x(n) + m_fComb_g*yn_LPF)
	float fb = *pInput + m_fComb_g*yn_LPF;

	// underflow check
	if(fb > 0.0 && fb < FLT_MIN_PLUS) fb = 0;
	if(fb < 0.0 && fb > FLT_MIN_MINUS) fb = 0;

	// write delay line
	this->writeDelayAndInc(fb);

	// write the output sample (could be combined with above line)
	if(m_nReadIndex == m_nWriteIndex)
		yn = *pInput;

	*pOutput = yn;

	// all OK
	return true;
}
