/*
VAST Dynamics Audio Software (TM)
*/

#pragma once

#include "../VASTEngineHeader.h"
#include "VASTEffect.h"

//==============================================================================

class CVASTMultibandCompressorSingleBand : public CVASTEffect
{
public:
    //==============================================================================

	CVASTMultibandCompressorSingleBand(VASTAudioProcessor* processor, CVASTSettings& set, int busnr, int band);
	~CVASTMultibandCompressorSingleBand();

    //==============================================================================

	void init() override;
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

	
    //======================================


private:
	//functions
	void compressorBand(AudioSampleBuffer &buffer, int m, const int numSamples);// compressor functions
	void adjustLowband();
	void adjustHighband();

    //==============================================================================	
	//parameters
	std::atomic<float>* m_bMBCompOffOn = nullptr;
	std::atomic<float>* m_fMBCompFcLow = nullptr;;
	std::atomic<float>* m_fMBCompFcHigh = nullptr;
	std::atomic<float>* m_fMBCompThreshold = nullptr;
	std::atomic<float>* m_fMBCompAttack = nullptr;
	std::atomic<float>* m_fMBCompRelease = nullptr;
	std::atomic<float>* m_fMBCompRatio = nullptr;
	std::atomic<float>* m_fMBCompGain = nullptr;

	//filters
	IIRFilter lowBandPassL1, lowBandPassL2, lowBandPassR1, lowBandPassR2; //Filters for low band pass
	IIRFilter highBandPassL1, highBandPassL2, highBandPassR1, highBandPassR2; //Filters for high band pass

	IIRCoefficients coeff; //Coefficient variable

	HeapBlock <float> m_x_g, m_x_l, m_y_g, m_y_l, m_c;// input, output, control

	//compressor parameters for each frequency band
	float m_yL_prev;

	float LCF;
	float HCF;

	int m_iNumChannels = 2;
	int myBand = 0;
	CVASTSettings* m_Set;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CVASTMultibandCompressorSingleBand)
};
