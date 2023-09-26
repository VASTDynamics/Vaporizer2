/*
VAST Dynamics Audio Software (TM)
*/

#pragma once
#include "../VASTPluginConstants.h"
#include "VASTRateConvertor.h"

class CInterpolator : public CRateConvertor
{
public:
	CInterpolator(void);
	~CInterpolator(void);

	virtual void interpolateNextOutputSample(float xnL, float xnR, float& fLeftOutput, float& fRightOutput);
	virtual void interpolateSamples(float xnL, float xnR, float* pLeftInterpBuffer, float* pRightInterpBuffer);

};

