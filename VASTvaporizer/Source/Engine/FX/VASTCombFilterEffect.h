/*
VAST Dynamics Audio Software (TM)
*/

#pragma once
#include "FXUtils/VASTDelaySoftAdjust.h"
#include "../VASTSettings.h"
#include "VASTEffect.h"

class CVASTCombFilterEffect : public CVASTEffect
{
public:
	//==============================================================================

	CVASTCombFilterEffect(VASTAudioProcessor* processor, CVASTSettings& set, int busnr);
	~CVASTCombFilterEffect();

	//==============================================================================

	void init() override;
	void initParameters(); //must not add parameters here anymore - will change sequence
	void initCompatibilityParameters() override; //new parameters go here
	void initCompatibilityParameters5() override {}; //new parameters go here
	void prepareToPlay(double sampleRate, int samplesPerBlock) override;
	void reset(); //called when switch toggled
	void releaseResources() override;
	void processBlock(AudioSampleBuffer& audioSampleBuffer, MidiBuffer& midiBuffer, const int numSamples) override;
	void updateTiming() override {};

	//==============================================================================

	void getStateInformation(MemoryBlock& destData) override;
	void setStateInformation(const void* data, int sizeInBytes) override;

	//==============================================================================

	void parameterChanged(const String& parameterID, float newValue) override;

	//==============================================================================

	float getDelay_ms();

	bool processAudioFrame(float* pInputBuffer, float* pOutputBuffer, MYUINT uNumInputChannels, MYUINT uNumOutputChannels, int currentFrame, modMatrixInputState* inputState);

	void setDelay_mSec(float fmSec);

private:
	std::atomic<float>* m_bCombOnOff = nullptr;
	std::atomic<float>* m_fCombFrequOffset = nullptr;
	std::atomic<float>* m_fCombDrive = nullptr;
	std::atomic<float>* m_fCombLevel = nullptr;
	std::atomic<float>* m_fCombDryWet = nullptr;
	std::atomic<float>* m_fCombGain = nullptr;

	LinearSmoothedValue<float> m_fCombFrequOffset_smoothed;
	LinearSmoothedValue<float> m_fCombDryWet_smoothed;
	LinearSmoothedValue<float> m_fCombGain_smoothed;
	LinearSmoothedValue<float> m_fCombDrive_smoothed;
	LinearSmoothedValue<float> m_fCombLevel_smoothed;

	CVASTSettings* m_Set;
	CVASTDelay mDelayLeft;
	CVASTDelay mDelayRight;

	JUCE_LEAK_DETECTOR(CVASTCombFilterEffect)
};
