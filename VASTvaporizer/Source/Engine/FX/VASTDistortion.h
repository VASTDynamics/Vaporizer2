/*
VAST Dynamics Audio Software (TM)
*/

#pragma once

#include "../VASTEngineHeader.h"
#include "../Filter/VASTBiQuadFilter.h"
#include "../VASTSettings.h"
#include "VASTEffect.h"

class CVASTDistortion : public CVASTEffect
{
public:
	//==============================================================================

	CVASTDistortion(VASTAudioProcessor* processor, CVASTSettings& set, int busnr);
	~CVASTDistortion();

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

	float* m_pLeftInterpBuffer;
	float* m_pRightInterpBuffer;
	float* m_pLeftDecipBuffer;
	float* m_pRightDeciBuffer;

	CVASTBiQuad m_biQuadPreL;
	CVASTBiQuad m_biQuadPreR;
	CVASTBiQuad m_biQuadPostL;
	CVASTBiQuad m_biQuadPostR;

	bool processAudioFrame(float* pInputBuffer, float* pOutputBuffer, MYUINT uNumInputChannels, MYUINT uNumOutputChannels, modMatrixInputState &inputState);

private:
	std::atomic<float>* m_bDistortionOnOff = nullptr;
	std::atomic<float>* m_fDistDryWet = nullptr;
	std::atomic<float>* m_fDistDrive = nullptr;
	std::atomic<float>* m_fDistFuzz = nullptr;
	std::atomic<float>* m_fDistLowcut = nullptr;
	std::atomic<float>* m_fDistPreGain = nullptr;
	std::atomic<float>* m_fDistGain = nullptr;
	LinearSmoothedValue<float> m_fDistDryWet_smoothed;
	LinearSmoothedValue<float> m_fDistDrive_smoothed;
	LinearSmoothedValue<float> m_fDistFuzz_smoothed;
	LinearSmoothedValue<float> m_fDistLowcut_smoothed;
	LinearSmoothedValue<float> m_fDistGain_smoothed;
	LinearSmoothedValue<float> m_fDistPreGain_smoothed;
	CVASTSettings* m_Set;

	//Lowcut biquad
	CVASTBiQuad m_lowCutBiquadL;
	CVASTBiQuad m_lowCutBiquadR;

	JUCE_LEAK_DETECTOR(CVASTDistortion)
};