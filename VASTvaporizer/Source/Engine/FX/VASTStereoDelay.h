/*
VAST Dynamics Audio Software (TM)
*/

#pragma once

#include "../VASTEngineHeader.h"
#include "../VASTPluginConstants.h"
#include "../VASTSettings.h"
#include "FXUtils/VASTDelayLineAPF.h"
#include "FXUtils/VASTDelayLine.h"
#include "FXUtils/VASTDDLModule.h"
#include "VASTEffect.h"
#include "../Filter/VASTBiQuadFilter.h"

class CVASTStereoDelay : public CVASTEffect
{
public:
	//==============================================================================

	CVASTStereoDelay(VASTAudioProcessor* processor, CVASTSettings& set, int busnr);
	~CVASTStereoDelay();

	//==============================================================================

	void init() override;
	void initParameters(); //must not add parameters here anymore - will change sequence
	void initCompatibilityParameters() override; //new parameters go here
	void initCompatibilityParameters5() override {}; //new parameters go here
	void prepareToPlay(double sampleRate, int samplesPerBlock) override;
	void reset(); //called when switch toggled
	void releaseResources() override;
	void processBlock(AudioSampleBuffer& audioSampleBuffer, MidiBuffer& midiBuffer, const int numSamples) override;
	void updateTiming() override;

	//==============================================================================

	void getStateInformation(MemoryBlock& destData) override;
	void setStateInformation(const void* data, int sizeInBytes) override;

	//==============================================================================

	void parameterChanged(const String& parameterID, float newValue) override;

	//==============================================================================

	void setMode(MYUINT u);
	void setDelayRatio(double d);

	void update();
	bool processAudioFrame(float* pInputBuffer, float* pInputBufferUnfiltered, float* pOutputBuffer, MYUINT uNumInputChannels, MYUINT uNumOutputChannels);
	
protected:
	CDelayLineAPF m_LeftDelay;
	CDelayLineAPF m_RightDelay;
	double m_dDelayRatio;	// -0.9 to + 0.9
	MYUINT m_uMode;
	enum { cross, pingpong, straight };

private:
	std::atomic<float>* m_bDelayOnOff = nullptr;
	std::atomic<float>* m_uDelayMode = nullptr;
	std::atomic<float>* m_fDelayWet = nullptr;
	std::atomic<float>* m_fDelayTime = nullptr;
	std::atomic<float>* m_uDelayTimeBeats = nullptr;
	std::atomic<float>* m_fDelayFeedback = nullptr;
	std::atomic<float>* m_bDelaySynch = nullptr;
	std::atomic<float>* m_fDelayLowcut = nullptr;
	std::atomic<float>* m_fDelayHighcut = nullptr;
	std::atomic<float>* m_fDelayGain = nullptr;

	LinearSmoothedValue<float> m_fDelayTime_smoothed;
	LinearSmoothedValue<float> m_fDelayDryWet_smoothed;
	LinearSmoothedValue<float> m_fDelayGain_smoothed;
	LinearSmoothedValue<float> m_fDelayFeedback_smoothed;
	LinearSmoothedValue<float> m_fDelayLowcut_smoothed;
	LinearSmoothedValue<float> m_fDelayHighcut_smoothed;

	CVASTSettings *m_Set;

	//Lowcut biquad
	CVASTBiQuad m_lowCutBiquadL;
	CVASTBiQuad m_lowCutBiquadR;
	//Highcut biquad
	CVASTBiQuad m_highCutBiquadL;
	CVASTBiQuad m_highCutBiquadR;

	float m_fDrywetMod = 1.0f;
	float m_fFeedbackMod = 1.0f;
	float m_fTimeMod = 1.0f;

	JUCE_LEAK_DETECTOR(CVASTStereoDelay)
};
