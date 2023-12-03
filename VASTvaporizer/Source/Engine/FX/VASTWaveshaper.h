/*
VAST Dynamics Audio Software (TM)
*/

#pragma once

#include "../VASTEngineHeader.h"
#include "../Filter/VASTBiQuadFilter.h"
#include "../VASTSettings.h"
#include "VASTEffect.h"
#ifdef __aarch64__ //arm64
	#include "../../sse2neon.h"
#else
	#include "emmintrin.h"
#endif

class CVASTWaveshaper : public CVASTEffect
{
public:
	//==============================================================================

	CVASTWaveshaper(VASTAudioProcessor* processor, CVASTSettings& set, int busnr);
	~CVASTWaveshaper();

	//==============================================================================

	void init() override;
	void initParameters(); //must not add parameters here anymore - will change sequence
	void initCompatibilityParameters() override; //new parameters go here
	void initCompatibilityParameters5() override; //new parameters go here
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
	std::atomic<float>* m_bWaveshaperOnOff = nullptr;
	std::atomic<float>* m_fWaveshaperDryWet = nullptr;
	std::atomic<float>* m_fWaveshaperDrive = nullptr;
	std::atomic<float>* m_uWaveshaperType = nullptr;
	std::atomic<float>* m_fWaveshaperLowcut = nullptr;
	std::atomic<float>* m_fWaveshaperHighcut = nullptr;
	std::atomic<float>* m_uWaveshaperPrePostEq = nullptr;
	std::atomic<float>* m_fWaveshaperPreGain = nullptr;
	std::atomic<float>* m_fWaveshaperGain = nullptr;

	LinearSmoothedValue<float> m_fWaveshaperDryWet_smoothed;
	LinearSmoothedValue<float> m_fWaveshaperDrive_smoothed;
	LinearSmoothedValue<float> m_fWaveshaperHighcut_smoothed;
	LinearSmoothedValue<float> m_fWaveshaperLowcut_smoothed;
	LinearSmoothedValue<float> m_fWaveshaperGain_smoothed;
	LinearSmoothedValue<float> m_fWaveshaperPreGain_smoothed;

	__m128 CLIP(__m128 in, __m128 drive);
	__m128 DIGI_SSE2(__m128 in, __m128 drive);
	__m128 TANH(__m128 in, __m128 drive);
	__m128 ASYM_SSE2(__m128 in, __m128 drive);
	__m128 SINUS_SSE2(__m128 in, __m128 drive);

	//Lowcut biquad
	CVASTBiQuad m_lowCutBiquadL;
	CVASTBiQuad m_lowCutBiquadR;

	//Lowcut biquad
	CVASTBiQuad m_highCutBiquadL;
	CVASTBiQuad m_highCutBiquadR;

	CVASTSettings* m_Set;

	JUCE_LEAK_DETECTOR(CVASTWaveshaper)
};
