/*
VAST Dynamics Audio Software (TM)
*/

#pragma once

#include "../VASTEngineHeader.h"
#include "../Filter/VASTBiQuadFilter.h"
#include "../VASTSettings.h"
#include "VASTEffect.h"

class CVASTBitcrush : public CVASTEffect
{
public:
	//==============================================================================

	CVASTBitcrush(VASTAudioProcessor* processor, CVASTSettings& set, int busnr);
	~CVASTBitcrush();

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
	
private:
	std::atomic<float>* m_bBitcrushOnOff = nullptr;
	std::atomic<float>* m_fBitcrushDryWet = nullptr;
	std::atomic<float>* m_fBitcrushLowcut = nullptr;
	std::atomic<float>* m_fBitcrushBitdepth = nullptr;
	std::atomic<float>* m_fBitcrushBitrate = nullptr;
	std::atomic<float>* m_fBitcrushJitter = nullptr;
	std::atomic<float>* m_fBitcrushGain = nullptr;

	LinearSmoothedValue<float> m_fBitcrushDryWet_smoothed;
	LinearSmoothedValue<float> m_fBitcrushLowcut_smoothed;
	LinearSmoothedValue<float> m_fBitcrushBitdepth_smoothed;
	LinearSmoothedValue<float> m_fBitcrushJitter_smoothed;
	
	juce::Random m_rand;

	//Lowcut biquad
	CVASTBiQuad m_lowCutBiquadL;
	CVASTBiQuad m_lowCutBiquadR;

	float m_fDrywetMod = 1.0f;
	int m_sampleCount = 0;
	int m_repeatLength = 0;
	float m_leftFirstSample = 0.0f;
	float m_rightFirstSample = 0.0f;

	CVASTSettings* m_Set;


	JUCE_LEAK_DETECTOR(CVASTBitcrush)
};