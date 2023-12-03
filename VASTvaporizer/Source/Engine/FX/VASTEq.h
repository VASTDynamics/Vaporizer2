/*
VAST Dynamics Audio Software (TM)
*/

#pragma once

#include "../VASTSettings.h"
#include "../Filter/VASTCombFilter.h"
#include "../Filter/VASTDelay.h"
#include "../Filter/VASTDelayAPF.h"
#include "../Filter/VASTLPFCombFilter.h"
#include "../Filter/VASTOnePoleLPF.h"
#include "../Filter/VASTBiQuadFilter.h"
#include "VASTMultibandCompressor.h" //IIR Filter!

#include "VASTEffect.h"

class CVASTEq : public CVASTEffect
{
public:
	//==============================================================================

	CVASTEq(VASTAudioProcessor* processor, CVASTSettings& set, int busnr);
	~CVASTEq();

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
	bool processAudioFrame(float* pInputFrame, float* pOutputFrame, MYUINT uNumInputChannels, MYUINT uNumOutputChannels);

private:
	std::atomic<float>* m_bEQOnOff = nullptr;
	std::atomic<float>* m_fEQ8 = nullptr;
	std::atomic<float>* m_fEQ7 = nullptr;
	std::atomic<float>* m_fEQ6 = nullptr;
	std::atomic<float>* m_fEQ4 = nullptr;
	std::atomic<float>* m_fEQ5 = nullptr;
	std::atomic<float>* m_fEQ3 = nullptr;
	std::atomic<float>* m_fEQ2 = nullptr;
	std::atomic<float>* m_fEQ1 = nullptr;
	std::atomic<float>* m_fEQGain = nullptr;
	
	LinearSmoothedValue<float> m_fEQGain_smoothed;

	CVASTSettings *m_Set;

	CVASTBiQuad m_biQuad1L;
	CVASTBiQuad m_biQuad2L;
	CVASTBiQuad m_biQuad3L;
	CVASTBiQuad m_biQuad4L;
	CVASTBiQuad m_biQuad5L;
	CVASTBiQuad m_biQuad6L;
	CVASTBiQuad m_biQuad7L;
	CVASTBiQuad m_biQuad8L;

	CVASTBiQuad m_biQuad1R;
	CVASTBiQuad m_biQuad2R;
	CVASTBiQuad m_biQuad3R;
	CVASTBiQuad m_biQuad4R;
	CVASTBiQuad m_biQuad5R;
	CVASTBiQuad m_biQuad6R;
	CVASTBiQuad m_biQuad7R;
	CVASTBiQuad m_biQuad8R;
	
	/*
	CVASTIIRFilter m_biQuad1L;
	CVASTIIRFilter m_biQuad2L;
	CVASTIIRFilter m_biQuad3L;
	CVASTIIRFilter m_biQuad4L;
	CVASTIIRFilter m_biQuad5L;
	CVASTIIRFilter m_biQuad6L;
	CVASTIIRFilter m_biQuad7L;
	CVASTIIRFilter m_biQuad8L;

	CVASTIIRFilter m_biQuad1R;
	CVASTIIRFilter m_biQuad2R;
	CVASTIIRFilter m_biQuad3R;
	CVASTIIRFilter m_biQuad4R;
	CVASTIIRFilter m_biQuad5R;
	CVASTIIRFilter m_biQuad6R;
	CVASTIIRFilter m_biQuad7R;
	CVASTIIRFilter m_biQuad8R;
	IIRCoefficients m_coeff; //Coefficient variable
	*/

	JUCE_LEAK_DETECTOR(CVASTEq)
};

