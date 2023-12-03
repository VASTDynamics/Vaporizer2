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

	CVASTMultibandCompressor(VASTAudioProcessor* processor, CVASTSettings& set, int busnr);
	~CVASTMultibandCompressor();

    //==============================================================================

	void init() override;
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
	std::atomic<float>* m_bMBCompOffOn = nullptr;
	std::atomic<float>* m_fMBCompFcLow = nullptr;;
	std::atomic<float>* m_fMBCompFcHigh = nullptr;
	std::atomic<float>* m_fMBCompThresholdLB = nullptr;
	std::atomic<float>* m_fMBCompThresholdMB = nullptr;
	std::atomic<float>* m_fMBCompThresholdHB = nullptr;
	std::atomic<float>* m_fMBCompAttackLB = nullptr;
	std::atomic<float>* m_fMBCompAttackMB = nullptr;
	std::atomic<float>* m_fMBCompAttackHB = nullptr;
	std::atomic<float>* m_fMBCompReleaseLB = nullptr;
	std::atomic<float>* m_fMBCompReleaseMB = nullptr;
	std::atomic<float>* m_fMBCompReleaseHB = nullptr;
	std::atomic<float>* m_fMBCompRatioLB = nullptr;
	std::atomic<float>* m_fMBCompRatioMB = nullptr;
	std::atomic<float>* m_fMBCompRatioHB = nullptr;
	std::atomic<float>* m_fMBCompGainLB = nullptr;
	std::atomic<float>* m_fMBCompGainMB = nullptr;
	std::atomic<float>* m_fMBCompGainHB = nullptr;
	
	LinearSmoothedValue<float> m_fMBCompThresholdLB_smoothed;
	LinearSmoothedValue<float> m_fMBCompThresholdHB_smoothed;
	LinearSmoothedValue<float> m_fMBCompThresholdMB_smoothed;
	LinearSmoothedValue<float> m_fMBCompRatioLB_smoothed;
	LinearSmoothedValue<float> m_fMBCompRatioHB_smoothed;
	LinearSmoothedValue<float> m_fMBCompRatioMB_smoothed;
	LinearSmoothedValue<float> m_fMBCompGainLB_smoothed;
	LinearSmoothedValue<float> m_fMBCompGainHB_smoothed;
	LinearSmoothedValue<float> m_fMBCompGainMB_smoothed;
	LinearSmoothedValue<float> m_fMBCompFcLow_smoothed;
	LinearSmoothedValue<float> m_fMBCompFcHigh_smoothed;
	
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
	CVASTSettings* m_Set;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CVASTMultibandCompressor)
};
