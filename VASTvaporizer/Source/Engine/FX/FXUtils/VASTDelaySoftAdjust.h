/*
VAST Dynamics Audio Software (TM)

Delay with Soft Logic
*/

#pragma once

#include "../../VASTSettings.h"

// underflow protection
#ifndef FLT_MIN_PLUS 
	#define FLT_MIN_PLUS          1.175494351e-38         /* min positive value */
#endif
#ifndef FLT_MIN_MINUS 
	#define FLT_MIN_MINUS        -1.175494351e-38         /* min negative value */
#endif

class CVASTDelay
{
public:
	// constructor/destructor
	CVASTDelay(void);
	virtual ~CVASTDelay(void);  // base class MUST declare virtual destructor

	void needsResetBeforUse();
	bool m_needsResetBeforUse = false;

protected:
	// member variables
	//
	// delay in samples; float supports fractional delay
	float m_fDelayInSamples;

	// output attenuation value, cooked
	float m_fOutputAttenuation;

    std::unique_ptr<AudioSampleBuffer> m_pBuffer;

	// sample rate (needed for other function)
	int m_nSampleRate;

	// Attenuation in dB, set by Parent Plug In
	float m_fOutputAttenuation_dB;

	// functions for the owner plug-in to call
public:
	// delay in mSec, set by Parent Plug In
	float m_fDelay_ms;

	// read/write index values for circ buffer
	int m_nReadIndex;
	int m_nWriteIndex;

	// declare buffer and zero
	void init(int nDelayLength);

	// function to cook variables
	void cookVariables();

	void softAdjustVariables(); //CHTS - do it smooth

	// flush buffer, reset pointers to top
	void resetDelay();

	// set functions for Parent Plug In
	void setDelay_mSec(float fmSec);
	void setSampleRate(int nFs){m_nSampleRate = nFs;};
	void setOutputAttenuation_dB(float fAttendB);

	// read the delay without writing or incrementing
	float readDelay();

	// read the delay at an arbitrary time without writing or incrementing
	float readDelayAt(float fmSec);

	// write the input and icrement pointers
	void writeDelayAndInc(float fDelayInput);

	// process audio
	bool processAudio(float* pInput, float* pOutput);

};

