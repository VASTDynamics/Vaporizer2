/*
VAST Dynamics Audio Software (TM)
*/

#include "VASTCombFilter.h"

CCombFilter::CCombFilter(void):CDelay()
{
	m_fComb_g = 0;
}

CCombFilter::~CCombFilter(void)
{
}
	
bool CCombFilter::processAudio(float* pInput, float* pOutput)
{
	// read the delay line to get w(n-D); call base class
	float yn = this->readDelay();

	if(m_nReadIndex == m_nWriteIndex)
		yn = 0;

	
	// form fb = x(n) + m_fComb_gy(n)
	float fb = *pInput + m_fComb_g*yn;
	

// test http://www.cs.cf.ac.uk/Dave/CM0268/PDF/10_CM0268_Audio_FX.pdf
	//float fb = *pInput + -0.5f *yn;


	// underflow check
	if(fb > 0.0 && fb < FLT_MIN_PLUS) fb = 0;
	if(fb < 0.0 && fb > FLT_MIN_MINUS) fb = 0;

	// write delay line
	this->writeDelayAndInc(fb);

	// write the output sample (could be combined with above line)
	if(m_nReadIndex == m_nWriteIndex)
		yn = *pInput;

	*pOutput = yn;
	//*pOutput = yn + 0.5f * fb;

	// all OK
	return true;
}
