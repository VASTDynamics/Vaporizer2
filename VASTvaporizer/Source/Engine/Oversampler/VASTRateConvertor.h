/*
VAST Dynamics Audio Software (TM)
*/

#pragma once
#include "../VASTPluginConstants.h"
#include <cstring>

class CRateConvertor
{
public:
	CRateConvertor(void);
	virtual ~CRateConvertor(void);

	// buffer for the impulse response h[n] of the FIR
	float *m_pIRBuffer;

	// buffers for the transversal delay lines, one for each input
	float* m_pLeftInputBuffer;
	float* m_pRightInputBuffer;

	// read index for delay lines (input x buffers)
	int m_nReadIndexDL;

	// read index for impulse response buffers
	int m_nReadIndexH;

	// write index for input x buffer
	int m_nWriteIndex;

	int m_nL; // OS value, 4 = 4X Oversampling

	// counters and index values for the convolutions
	int m_nCurrentL;
	int m_nIRLength;
	int m_nOSCounter;

	// initializer - creates the buffers and loads the FIR IR
	void init(int L, int FIRLen, float *pIRBuffer);

	// flush buffers
	void reset();
	
	// overrides for derived objects
	//
	// interpolateSamples: take one pair of L/R samples and produce L-length buffers of samples
	virtual void interpolateSamples(float xnL, float xnR, float* pLeftInterpBuffer, float* pRightInterpBuffer);

	// inner loop function that processes just one pair of inputs
	virtual void interpolateNextOutputSample(float xnL, float xnR, float& fLeftOutput, float& fRightOutput);

	// decimateSamples: take one pai rL-length buffers of samples and decimate down to just one pair of output samples 
	virtual void decimateSamples(float* pLeftDeciBuffer, float* pRightDeciBuffer, float& ynL, float& ynR);

	// inner loop function that processes just one pair of inputs
	virtual bool decimateNextOutputSample(float xnL, float xnR, float& fLeftOutput, float& fRightOutput);

};

