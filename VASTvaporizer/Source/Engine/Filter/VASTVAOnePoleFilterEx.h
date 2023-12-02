/*
VAST Dynamics Audio Software (TM)
*/

#pragma once

#include "../VASTEngineHeader.h"

class CVAOnePoleFilterEx
{
public:
	CVAOnePoleFilterEx(void);
	~CVAOnePoleFilterEx(void);
	
	// common variables
	double m_dSampleRate;	/* sample rate*/
	double m_dFc;			/* cutoff frequency */

	// Trapezoidal Integrator Components
	// variables
	double m_dAlpha;		// Feed Forward coeff
	double m_dBeta;			// Feed Back coeff from s + FB_IN

	// extended functionality variables
	double m_dGamma;		// Pre-Gain
	double m_dDelta;		// FB_IN Coeff
	double m_dEpsilon;		
	double m_da0;			// filter gain
	double m_dFeedback;		// Feed Back storage register (not a delay reg)

	// provide access to our feedback output
	double getFeedbackOutput(); 
	
	// provide access to set our feedback input
	void setFeedback(double fb);

	// for s_N only
	double getStorageValue();

	// flush buffer
	void reset();

	// do the filter
	double doFilter(double xn);

protected:
	double m_dZ1;		// our z-1 storage location
};

