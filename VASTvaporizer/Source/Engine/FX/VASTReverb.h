/*
VAST Dynamics Audio Software (TM)
*/

#pragma once

#include "../Filter/VASTCombFilter.h"
#include "../Filter/VASTDelay.h"
#include "../Filter/VASTDelayAPF.h"
#include "../Filter/VASTLPFCombFilter.h"
#include "../Filter/VASTOnePoleLPF.h"
#include "../Filter/VASTBiQuadFilter.h"
#include "../VASTSettings.h"
#include "FXUtils/VASTMVerb.h"
#include "VASTEffect.h"

class CVASTReverb : public CVASTEffect
{
public:

	//==============================================================================

	CVASTReverb(VASTAudioProcessor* processor, CVASTSettings& set, int busnr);
	~CVASTReverb();

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

	void updateVariables();
	bool processAudioFrame(float* pInputFrame, float* pOutputFrame, MYUINT uNumInputChannels, MYUINT uNumOutputChannels, int currentFrameOSAdjusted, float dryWet);

private:
	std::atomic<float>* m_bReverbOnOff = nullptr;
	std::atomic<float>* m_uReverbMode = nullptr;
	std::atomic<float>* m_fReverbDryWet = nullptr;
	std::atomic<float>* m_fReverbSize = nullptr;
	std::atomic<float>* m_fReverbPredelay = nullptr;
	std::atomic<float>* m_fReverbFeedback = nullptr;
	std::atomic<float>* m_fReverbLowcut = nullptr;
	std::atomic<float>* m_fReverbHighcut = nullptr;
	std::atomic<float>* m_fReverbGain = nullptr;
	std::atomic<float>* m_fReverbDamping = nullptr;

	LinearSmoothedValue<float> m_fReverbGain_smoothed;
	LinearSmoothedValue<float> m_fReverbDryWet_smoothed;
	LinearSmoothedValue<float> m_fReverbFeedback_smoothed;
	LinearSmoothedValue<float> m_fReverbSize_smoothed;
	LinearSmoothedValue<float> m_fReverbPredelay_smoothed;
	LinearSmoothedValue<float> m_fReverbLowcut_smoothed;
	LinearSmoothedValue<float> m_fReverbHighcut_smoothed;
	LinearSmoothedValue<float> m_fReverbDamping_smoothed;

	float m_fSizeMod = 1.f;
	float m_fFeedbackMod = 1.f;
	float m_fDampingMod = 1.f;
	float m_fPredelayMod = 1.f;

	CVASTSettings* m_Set;

	// Pre-Delay Block
	CDelay m_PreDelay;

	// input Diffusion
	COnePoleLPF m_InputLPF;
	CDelayAPF m_InputAPF_1;
	CDelayAPF m_InputAPF_2;

	// parallel Comb Bank 1
	CCombFilter m_ParallelCF_1;
	CCombFilter m_ParallelCF_2;
	CCombFilter m_ParallelCF_3;
	CCombFilter m_ParallelCF_4;

	// parallel Comb Bank 2
	CCombFilter m_ParallelCF_5;
	CCombFilter m_ParallelCF_6;
	CCombFilter m_ParallelCF_7;
	CCombFilter m_ParallelCF_8;

	// damping
	COnePoleLPF m_DampingLPF1;
	COnePoleLPF m_DampingLPF2;

	// output diffusion
	CDelayAPF m_OutputAPF_3;
	CDelayAPF m_OutputAPF_4;

	//Lowcut biquad
	CVASTBiQuad m_lowCutBiquad1;
	CVASTBiQuad m_lowCutBiquadL;
	CVASTBiQuad m_lowCutBiquadR;

	//Lowcut biquad
	CVASTBiQuad m_highCutBiquad1;
	CVASTBiQuad m_highCutBiquadL;
	CVASTBiQuad m_highCutBiquadR;

	//ui stuff
	//float m_fPreDelay_mSec;
	float m_fPreDelayAtten_dB;
	float m_fInputLPF_g;
	float m_fAPF_1_Delay_mSec;
	float m_fAPF_1_g;
	float m_fAPF_2_Delay_mSec;
	float m_fAPF_2_g;
	float m_fPComb_1_Delay_mSec;
	float m_fPComb_2_Delay_mSec;
	float m_fPComb_3_Delay_mSec;
	float m_fPComb_4_Delay_mSec;
	float m_fLPF2_g2;
	float m_fAPF_3_Delay_mSec;
	float m_fAPF_3_g;
	float m_fPComb_5_Delay_mSec;
	float m_fPComb_6_Delay_mSec;
	float m_fPComb_7_Delay_mSec;
	float m_fPComb_8_Delay_mSec;
	float m_fAPF_4_Delay_mSec;
	float m_fAPF_4_g;

	MVerb<float> m_plateReverb;
	JUCE_LEAK_DETECTOR(CVASTReverb)
};




