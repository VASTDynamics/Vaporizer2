/*
VAST Dynamics Audio Software (TM)
*/

#pragma once
#include "../VASTSettings.h"
#include "VASTEffect.h"
#include "../Oscillator/VASTWaveTableOscillator.h"  

class CVASTFlanger : public CVASTEffect
{
public:
	//==============================================================================

	CVASTFlanger(VASTAudioProcessor* processor, CVASTSettings& set, int busnr);
	~CVASTFlanger();

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

private:
	void updateLFOFreq();

	std::atomic<float>* m_bFlangerOnOff = nullptr;
	std::atomic<float>* m_fFlangerDryWet = nullptr;
	std::atomic<float>* m_fFlangerDelay = nullptr;
	std::atomic<float>* m_fFlangerWidth = nullptr;
	std::atomic<float>* m_fFlangerFeedback = nullptr;
	std::atomic<float>* m_fFlangerLFOFreq = nullptr;
	std::atomic<float>* m_uFlangerLFOWave = nullptr;
	std::atomic<float>* m_bFlangerSynch = nullptr;
	std::atomic<float>* m_uFlangerTimeBeats = nullptr;
	std::atomic<float>* m_fFlangerGain = nullptr;

	LinearSmoothedValue<float> m_fFlangerDelay_smoothed;
	LinearSmoothedValue<float> m_fFlangerWidth_smoothed;
	LinearSmoothedValue<float> m_fFlangerFeedback_smoothed;
	LinearSmoothedValue<float> m_fFlangerLFOFreq_smoothed;
	LinearSmoothedValue<float> m_fFlangerDryWet_smoothed;
	LinearSmoothedValue<float> m_fFlangerGain_smoothed;

	AudioSampleBuffer delayBuffer;
	AudioSampleBuffer m_fBuffer;

	int delayBufferSamples;
	int delayBufferChannels;
	int delayWritePosition;

	CVASTWaveTableOscillator m_LFO; 	
	CVASTSettings* m_Set;
	
	enum interpolationIndex {
		interpolationNearestNeighbour = 0,
		interpolationLinear,
		interpolationCubic,
	};

	JUCE_LEAK_DETECTOR(CVASTFlanger)
};
