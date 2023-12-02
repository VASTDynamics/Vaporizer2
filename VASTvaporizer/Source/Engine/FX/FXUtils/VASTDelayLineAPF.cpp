/*
VAST Dynamics Audio Software (TM)
*/

#include "VASTDelayLineAPF.h"

CDelayLineAPF::CDelayLineAPF(void) :CDelayLine()
{
	m_fAPF_g = 0;
}

CDelayLineAPF::~CDelayLineAPF(void)
{
}

bool CDelayLineAPF::processAudio(float* pInput, float* pOutput)
{
	// read the delay line to get w(n-D); call base class
	float fw_n_D = float(this->readDelay());

	// for the APF if the delay is 0.0
	// need to pass input -> output
	if (m_nReadIndex == m_nWriteIndex)
	{
		this->writeDelayAndInc(*pInput);
		*pOutput = *pInput;
		return true;
	}

	// form w(n) = x(n) + gw(n-D)
	float fw_n = *pInput + m_fAPF_g*fw_n_D;

	// form y(n) = -gw(n) + w(n-D)
	float fy_n = -m_fAPF_g*fw_n + fw_n_D;

	// underflow check
	if (fy_n > 0.0 && fy_n < FLT_MIN_PLUS) fy_n = 0;
	if (fy_n < 0.0 && fy_n > FLT_MIN_MINUS) fy_n = 0;

	// write delay line
	this->writeDelayAndInc(fw_n);

	// write the output sample (could be combined with above line)
	*pOutput = fy_n;

	// all OK
	return true;
}
