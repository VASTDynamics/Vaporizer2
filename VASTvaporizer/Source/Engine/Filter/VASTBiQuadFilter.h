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
	void flushDelays();
	// Do the filter: given input xn, calculate output yn and return it
	float doBiQuad(double d_xn);
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
