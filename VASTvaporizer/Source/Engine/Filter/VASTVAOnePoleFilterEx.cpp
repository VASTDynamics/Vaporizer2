/*
VAST Dynamics Audio Software (TM)
*/

#include "VASTVAOnePoleFilterEx.h"
#include <math.h>
#ifdef DEBUG
	#define _CRTDBG_MAP_ALLOC
#endif

CVAOnePoleFilterEx::CVAOnePoleFilterEx(void)
{
	m_dAlpha = 1.0;		
	m_dBeta = -1.0;		
	m_dGamma = 1.0;
	m_dDelta = 0.0;
	m_dEpsilon = 1.0;
	m_dFeedback = 0.0;
	m_da0 = 1.0;

	reset();
}

CVAOnePoleFilterEx::~CVAOnePoleFilterEx(void)
{
}

double CVAOnePoleFilterEx::getFeedbackOutput()
{
	return m_dBeta*(m_dZ1 + m_dFeedback*m_dDelta);
}

// do the filter
double CVAOnePoleFilterEx::doFilter(double xn)
{
	double x_in = (xn*m_dGamma + m_dFeedback + m_dEpsilon*getFeedbackOutput());
	
	double vn = (m_da0*x_in - m_dZ1)*m_dAlpha;
	double out = vn + m_dZ1;
	m_dZ1 = vn + out;

	return out;
}


