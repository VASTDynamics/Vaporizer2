/*
VAST Dynamics Audio Software (TM)
*/

#include "VASTVAOnePoleFilter.h"
#include "../VASTPluginConstants.h"
#include <math.h>
#ifdef DEBUG
	#define _CRTDBG_MAP_ALLOC
#endif

JUCE_BEGIN_IGNORE_WARNINGS_GCC_LIKE("-Wconversion")
JUCE_BEGIN_IGNORE_WARNINGS_MSVC(4244 4267) //C4244 conversion from 'type1' to 'type2', possible loss of data //C4267 conversion

CVAOnePoleFilter::CVAOnePoleFilter(void)
{
	m_dAlpha = 1.0;
	m_dBeta = 1.0;

	m_uFilterType = LPF1;
	reset();
}

CVAOnePoleFilter::~CVAOnePoleFilter(void)
{
}


// provide access to our feedback output

double CVAOnePoleFilter::getFeedbackOutput() { return m_dZ1 * m_dBeta; }

// -- CFilter Overrides --

void CVAOnePoleFilter::reset() { m_dZ1 = 0; }

// recalc coeffs -- NOTE: not used for Korg35 Filter
void CVAOnePoleFilter::updateFilter()
{
	double wd = 2 * M_PI * m_dFc;          
	double T  = 1/m_dSampleRate;
	double wa = (2/T) * tan(wd*T/2);
	double g  = wa * T/2;

	m_dAlpha = g/(1.0 + g);
}

// do the filter
float CVAOnePoleFilter::doFilter(float xn)
{
	// calculate v(n)
	double vn = (xn - m_dZ1) * m_dAlpha;

	// form LP output
	double lpf = vn + m_dZ1;

	// update memory
	m_dZ1 = vn + lpf;

	// do the HPF
	double hpf = xn - lpf;

	if(m_uFilterType == LPF1)
		return lpf;
	else if(m_uFilterType == HPF1)
		return hpf;

	// default
	return lpf;
}

JUCE_END_IGNORE_WARNINGS_GCC_LIKE
JUCE_END_IGNORE_WARNINGS_MSVC


