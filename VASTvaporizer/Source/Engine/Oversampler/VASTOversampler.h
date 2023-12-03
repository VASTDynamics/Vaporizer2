/*
VAST Dynamics Audio Software (TM)
*/

#pragma once

// base class
#include "VASTDecimator.h"
#include "VASTInterpolator.h"
#include "../VASTSettings.h"
#include <memory>

class CVASTOversampler
{
public:
	CVASTOversampler(CVASTSettings& set);

	~CVASTOversampler(void);

	bool prepareForPlay();
	
	void upsampleAudioBuffer4(dsp::AudioBlock<float> inBlock, dsp::AudioBlock<float> outBlock, const int numSamples);
	void downsampleAudioBuffer4(dsp::AudioBlock<float> inBlock, dsp::AudioBlock<float> outBlock, const int numSamples);

	bool init();

	int m_nIRLength;
	
	// impulse response buffers
	float m_h_Left[1024];
	float m_h_Right[1024];

	//CInterpolator m_Interpolator;
	CDecimator m_Decimator;

	float m_pLeftInterpBuffer[C_OVERSAMPLING_RATIO];
	float m_pRightInterpBuffer[C_OVERSAMPLING_RATIO];
	float m_pLeftDecipBuffer[C_OVERSAMPLING_RATIO];
	float m_pRightDeciBuffer[C_OVERSAMPLING_RATIO];

	void updateVariables();

	CVASTSettings *m_Set;

	int m_nOversamplingRatioLocal = 0;

};














