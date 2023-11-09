/*
VAST Dynamics Audio Software (TM)
*/

#pragma once

#include "../VASTEngineHeader.h"
#include "VASTEffect.h"

//==============================================================================

class CVASTCompressorExpander : public CVASTEffect
{
public:
    //==============================================================================

	CVASTCompressorExpander(VASTAudioProcessor* processor, int busnr);
	~CVASTCompressorExpander();

    //==============================================================================

	void init(CVASTSettings &set) override;
	void initParameters(); //must not add parameters here anymore - will change sequence
	void initCompatibilityParameters() override; //new parameters go here
	void initCompatibilityParameters5() override {}; //new parameters go here
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
	void reset();
    void releaseResources() override;
    void processBlock(AudioSampleBuffer& audioSampleBuffer, MidiBuffer& midiBuffer, const int numSamples) override;
	void updateTiming() override {};

    //==============================================================================

    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //==============================================================================

	void parameterChanged(const String& parameterID, float newValue) override;

	//==============================================================================


private:
    //==============================================================================

	//AudioSampleBuffer mixedDownInput;
	float xl[2];
	float yl[2];
	float xg[2];
	float yg[2];
	float control[2];
	float inputLevel[2];
	float ylPrev[2];

	float inverseSampleRate;
	float inverseE;

	float calculateAttackOrRelease(float value);
	std::atomic<float>* m_bLimiterOffOn = nullptr;
	std::atomic<float>* m_fLimiterDryWet = nullptr;
	std::atomic<float>* m_uLimiterMode = nullptr;
	std::atomic<float>* m_fLimiterThreshold = nullptr;
	std::atomic<float>* m_fLimiterRatio = nullptr;
	std::atomic<float>* m_fLimiterAttack = nullptr;
	std::atomic<float>* m_fLimiterRelease = nullptr;
	std::atomic<float>* m_fLimiterMakeupGain = nullptr;

	LinearSmoothedValue<float> m_fLimiterDryWet_smoothed;
	LinearSmoothedValue<float> m_fLimiterThreshold_smoothed;
	LinearSmoothedValue<float> m_fLimiterGain_smoothed;
	LinearSmoothedValue<float> m_fLimiterRatio_smoothed;
	
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CVASTCompressorExpander)
};
