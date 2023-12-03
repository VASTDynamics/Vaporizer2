/*
VAST Dynamics Audio Software (TM)
*/

#pragma once
#include "../VASTSettings.h"
#include "VASTEffect.h"

class CVASTFormantFilter : public CVASTEffect
{
public:
	//==============================================================================

	CVASTFormantFilter(VASTAudioProcessor* processor, CVASTSettings& set, int busnr);
	~CVASTFormantFilter();

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

	bool processAudioFrame(float* pInputBuffer, float* pOutputBuffer, MYUINT uNumInputChannels, MYUINT uNumOutputChannels, int currentFrameOSAdjusted);

private:
	std::atomic<float>* m_bFormantOnOff = nullptr;
	std::atomic<float>* m_fFormantDryWet = nullptr;
	std::atomic<float>* m_fFormantVowelOne = nullptr;
	std::atomic<float>* m_fFormantVowelTwo = nullptr;
	std::atomic<float>* m_fFormantVowelMix = nullptr;
	std::atomic<float>* m_fFormantGain = nullptr;

	LinearSmoothedValue<float> m_fFormantVowelMix_smoothed;
	LinearSmoothedValue<float> m_fFormantDryWetMix_smoothed;
	LinearSmoothedValue<float> m_fFormantGain_smoothed;
	
	CVASTSettings* m_Set;
	double memoryOne_left[10] = { 0,0,0,0,0,0,0,0,0,0 };
	double memoryOne_right[10] = { 0,0,0,0,0,0,0,0,0,0 };
	double memoryTwo_left[10] = { 0,0,0,0,0,0,0,0,0,0 };
	double memoryTwo_right[10] = { 0,0,0,0,0,0,0,0,0,0 };

	JUCE_LEAK_DETECTOR(CVASTFormantFilter)
};
