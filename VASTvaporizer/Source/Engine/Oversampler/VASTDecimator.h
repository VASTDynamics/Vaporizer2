/*
VAST Dynamics Audio Software (TM)
*/

#pragma once
#include "../VASTEngineHeader.h"
#include "VASTRateConvertor.h"
#ifdef __aarch64__ //arm64
	#include "../../sse2neon.h"
#else
	#include "emmintrin.h"
#endif

#define C_FACTOR4_IR_LENGTH 512
#define C_FACTOR4_OVERSAMPLE 4

const unsigned int halfrate_max_M = 6;

class alignas(16) CDecimator //alignas(16) is important
{
public:
	CDecimator(void);
	virtual ~CDecimator(void);
	void init(int L, int FIRLen, float *pIRBuffer);
	// flush buffers
	void reset();

	void decimateSamples2(float* pLeftDeciBuffer, float* pRightDeciBuffer, 
		int numSamples = 64,
		float* outL = 0,
		float* outR = 0); // process in-place. the new block will be half the size
	void decimateSamples4(float* pLeftDeciBuffer, float* pRightDeciBuffer, float& ynL, float& ynR);
	bool decimateNextOutputSample4(float xnL, float xnR, float& fLeftOutput, float& fRightOutput, int i);

private:
	float *m_pIRBuffer;

	// buffers for the transversal delay lines, one for each input
	float *m_pLeftInputBuffer;
	float *m_pRightInputBuffer;

	// read index for delay lines (input x buffers)
	int m_nReadIndexDL;

	// read index for impulse response buffers
	int m_nReadIndexH;

	// write index for input x buffer
	int m_nWriteIndex;

	//for halfband
	void factor2DecimatorLoadCoefficients();
	void factor2DecimatorSetCoefficients(float* cA, float* cB);
	void factor2DecimatorReset();
	
	int M;
	bool steep;
	float oldoutL, oldoutR;
	__m128 va[halfrate_max_M];
	__m128 vx0[halfrate_max_M];
	__m128 vx1[halfrate_max_M];
	__m128 vx2[halfrate_max_M];
	__m128 vy0[halfrate_max_M];
	__m128 vy1[halfrate_max_M];
	__m128 vy2[halfrate_max_M];
	__m128 oldout;
};

