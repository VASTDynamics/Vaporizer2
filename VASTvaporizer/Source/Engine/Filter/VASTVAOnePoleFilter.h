/*
VAST Dynamics Audio Software (TM)
*/

#pragma once

#include "../VASTEngineHeader.h"
#include "../VASTPluginConstants.h"

class CVAOnePoleFilter
{
public:
	CVAOnePoleFilter(void);
	virtual ~CVAOnePoleFilter(void);
	
	// common variables
	double m_dSampleRate;	/* sample rate*/
	double m_dFc;			/* cutoff frequency */
	
	MYUINT m_uFilterType;		/* filter selection */
	enum{LPF1,HPF1}; /* one short string for each */

	// Trapezoidal Integrator Components
	double m_dAlpha;			// Feed Forward coeff
	double m_dBeta;			// Feed Back coeff

	// provide access to our feedback output
	double getFeedbackOutput();

	// -- CFilter Overrides --
	virtual void reset();

	// recalc the coeff -- NOTE: not used for Korg35 Filter
	virtual void updateFilter();
	
	// do the filter
	virtual float doFilter(float xn);

protected:
	double m_dZ1;		// our z-1 storage location
};

