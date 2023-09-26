/*
VAST Dynamics Audio Software (TM)
*/

#pragma once

#include "../VASTEngineHeader.h"
#include "../VASTPluginConstants.h"
#include "../VASTSettings.h"

// --- BiQuad ---
// This class can be used alone or as the base class for a derived object
// Note the virtual destructor, required for proper clean-up of derived
// classes
//
// Implements a single Bi-Quad Structure
class CBiQuad
{
public:
	CBiQuad(void);
	virtual ~CBiQuad(void);

protected:
	double m_d_Xz_1; // x z-1 delay element
	double m_d_Xz_2; // x z-2 delay element
	double m_d_Yz_1; // y z-1 delay element
	double m_d_Yz_2; // y z-2 delay element

					 // allow other objects to set these directly since we have no cooking
					 // function or intelligence

public:
	double m_d_a0;
	double m_d_a1;
	double m_d_a2;
	double m_d_b1;
	double m_d_b2;

	// flush Delays
	void flushDelays()
	{
		m_d_Xz_1 = 0;
		m_d_Xz_2 = 0;
		m_d_Yz_1 = 0;
		m_d_Yz_2 = 0;
	}

	// Do the filter: given input xn, calculate output yn and return it
	float doBiQuad(double d_xn)
	{
		// just do the difference equation: y(n) = a0x(n) + a1x(n-1) + a2x(n-2) - b1y(n-1) - b2y(n-2)
		double yn = m_d_a0 * d_xn + m_d_a1 * m_d_Xz_1 + m_d_a2 * m_d_Xz_2 - m_d_b1 * m_d_Yz_1 - m_d_b2 * m_d_Yz_2;

		// underflow check - snap to zero
		//if ((yn > 0.0) && (yn < FLT_MIN_PLUS)) yn = 0;
		//if ((yn < 0.0) && (yn > FLT_MIN_MINUS)) yn = 0;

		if ((yn > 0.0) && (yn <  1.0e-8)) yn = 0;
		if ((yn < 0.0) && (yn > -1.0e-8)) yn = 0;

		if (isnan(yn)) { //NaN check
			yn = 0.0f;
		}

		//check new
		if (yn > 2.0)
			yn = 2.0;
		if (yn < -2.0)
			yn = -2.0;

		// shuffle delays
		// Y delays
		m_d_Yz_2 = m_d_Yz_1;
		m_d_Yz_1 = yn;

		// X delays
		m_d_Xz_2 = m_d_Xz_1;
		m_d_Xz_1 = d_xn;

		return  yn;
	}
};

// derived class of CBiQuad
class CVASTBiQuad : public CBiQuad
{
public:
	// constructor/destructor
	CVASTBiQuad(void);
	~CVASTBiQuad(void);

	enum{ LOWPASS, HIGHPASS, BANDPASS, NOTCH, PEAK, LOWSHELF, HIGHSHELF };
	void calcBiquad(MYUINT type, double dFrequency, double dSampleRate, double dQ, double dpeakGain);
	void copySettingsFrom(CVASTBiQuad *source);
};
