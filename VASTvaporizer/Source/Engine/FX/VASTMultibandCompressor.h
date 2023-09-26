/*
VAST Dynamics Audio Software (TM)
*/

#pragma once

#include "../VASTEngineHeader.h"
#include "VASTEffect.h"
#include "../VASTSettings.h"
#include "../Filter/VASTIIRFilter.h"
#include <math.h>

//==============================================================================

class CVASTMultibandCompressor : public CVASTEffect
{
public:
    //==============================================================================

	CVASTMultibandCompressor(VASTAudioProcessor* processor, int busnr);
	~CVASTMultibandCompressor();

    //==============================================================================

	void init(CVASTSettings &set) override;
	void initParameters();
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

	
    //======================================


private:
	//functions
	void compressorLB(AudioSampleBuffer &buffer, int m, modMatrixInputState& inputState, const int numSamples);// compressor functions
	void compressorMB(AudioSampleBuffer &buffer, int m, modMatrixInputState& inputState, const int numSamples);// compressor functions
	void compressorHB(AudioSampleBuffer &buffer, int m, modMatrixInputState& inputState, const int numSamples);// compressor functions
	void adjustLowband();
	void adjustHighband();

    //==============================================================================	
	//parameters
	std::atomic<float>* m_bMultibandCompressorOffOn = nullptr;
	std::atomic<float>* m_fMultibandCompressorFcLow = nullptr;;
	std::atomic<float>* m_fMultibandCompressorFcHigh = nullptr;
	std::atomic<float>* m_fMultibandCompressorThresholdLB = nullptr;
	std::atomic<float>* m_fMultibandCompressorThresholdMB = nullptr;
	std::atomic<float>* m_fMultibandCompressorThresholdHB = nullptr;
	std::atomic<float>* m_fMultibandCompressorAttackLB = nullptr;
	std::atomic<float>* m_fMultibandCompressorAttackMB = nullptr;
	std::atomic<float>* m_fMultibandCompressorAttackHB = nullptr;
	std::atomic<float>* m_fMultibandCompressorReleaseLB = nullptr;
	std::atomic<float>* m_fMultibandCompressorReleaseMB = nullptr;
	std::atomic<float>* m_fMultibandCompressorReleaseHB = nullptr;
	std::atomic<float>* m_fMultibandCompressorRatioLB = nullptr;
	std::atomic<float>* m_fMultibandCompressorRatioMB = nullptr;
	std::atomic<float>* m_fMultibandCompressorRatioHB = nullptr;
	std::atomic<float>* m_fMultibandCompressorGainLB = nullptr;
	std::atomic<float>* m_fMultibandCompressorGainMB = nullptr;
	std::atomic<float>* m_fMultibandCompressorGainHB = nullptr;
	
	LinearSmoothedValue<float> m_fMultibandCompressorThresholdLB_smoothed;
	LinearSmoothedValue<float> m_fMultibandCompressorThresholdHB_smoothed;
	LinearSmoothedValue<float> m_fMultibandCompressorThresholdMB_smoothed;
	LinearSmoothedValue<float> m_fMultibandCompressorRatioLB_smoothed;
	LinearSmoothedValue<float> m_fMultibandCompressorRatioHB_smoothed;
	LinearSmoothedValue<float> m_fMultibandCompressorRatioMB_smoothed;
	LinearSmoothedValue<float> m_fMultibandCompressorGainLB_smoothed;
	LinearSmoothedValue<float> m_fMultibandCompressorGainHB_smoothed;
	LinearSmoothedValue<float> m_fMultibandCompressorGainMB_smoothed;
	LinearSmoothedValue<float> m_fMultibandCompressorFcLow_smoothed;
	LinearSmoothedValue<float> m_fMultibandCompressorFcHigh_smoothed;
	
	//filters
	CVASTIIRFilter lowPassL1, lowPassL2, lowPassR1, lowPassR2; //Filters for low band
	CVASTIIRFilter lowBandPassL1, lowBandPassL2, lowBandPassR1, lowBandPassR2; //Filters for low band pass
	CVASTIIRFilter highBandPassL1, highBandPassL2, highBandPassR1, highBandPassR2; //Filters for high band pass
	CVASTIIRFilter highPassL1, highPassL2, highPassR1, highPassR2; //Filters for high pass

	IIRCoefficients coeff; //Coefficient variable

	HeapBlock <float> x_g_lf, x_l_lf, y_g_lf, y_l_lf, c_lf;// input, output, control
	HeapBlock <float> x_g_mf, x_l_mf, y_g_mf, y_l_mf, c_mf;// input, output, control
	HeapBlock <float> x_g_hf, x_l_hf, y_g_hf, y_l_hf, c_hf;// input, output, control

	//compressor parameters for each frequency band
	float m_yL_prevLB, m_yL_prevMB, m_yL_prevHB;

	float LCF;
	float HCF;

	int m_iNumChannels = 2;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CVASTMultibandCompressor)
};
