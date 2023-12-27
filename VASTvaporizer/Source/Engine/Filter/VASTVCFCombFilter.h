/*
VAST Dynamics Audio Software (TM)
*/

#pragma once
#include "../VASTEngineHeader.h"
#include "../FX/FXUtils/VASTDelaySoftAdjust.h"
#include "../VASTSettings.h"
#include "../FX/VASTEffect.h"

class CVASTVCFCombFilter 
{
public:
	//==============================================================================

	CVASTVCFCombFilter(CVASTSettings& set);

	//==============================================================================

	void init();
	void prepareToPlay(double sampleRate, int samplesPerBlock);
	void reset(); //called when switch toggled
	void processBlock(dsp::AudioBlock<float> filterBlock, const int numSamples);

	//==============================================================================

	void getStateInformation(MemoryBlock& destData);
	void setStateInformation(const void* data, int sizeInBytes);

	//==============================================================================

	void parameterChanged(const String& parameterID, float newValue, bool takeNext, int skips);

	//==============================================================================

	float getDelay_ms();

	bool processAudioFrame(float* pInputBuffer, float* pOutputBuffer, MYUINT uNumInputChannels, MYUINT uNumOutputChannels, int currentFrame);
	
	void setDelay_mSec(float fmSec);
	void needsInitBeforUsed();

private:
	bool m_needsInitBeforUsed = false;
	int m_iExpectedSamplesPerBlock = 0;
	int m_iSampleRate = 0;
	float* m_bCombOnOff = nullptr;
	float* m_fCombFrequOffset = nullptr;
	float* m_fCombDrive = nullptr;
	float* m_fCombLevel = nullptr;
	float* m_fCombDryWet = nullptr;
	float* m_fCombGain = nullptr; 

	LinearSmoothedValue<float> m_fCombFrequOffset_smoothed;
	LinearSmoothedValue<float> m_fCombDryWet_smoothed;
	LinearSmoothedValue<float> m_fCombGain_smoothed;
	LinearSmoothedValue<float> m_fCombDrive_smoothed;
	LinearSmoothedValue<float> m_fCombLevel_smoothed;

	CVASTSettings* m_Set;
	CVASTDelay mDelayLeft;
	CVASTDelay mDelayRight;

	JUCE_LEAK_DETECTOR(CVASTVCFCombFilter)
};
