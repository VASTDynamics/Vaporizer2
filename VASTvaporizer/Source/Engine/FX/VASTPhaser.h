/*
VAST Dynamics Audio Software (TM)
*/

#pragma once
#include "../VASTSettings.h"
#include "VASTEffect.h"
#include "../Oscillator/VASTWaveTableOscillator.h"  

class CVASTPhaser : public CVASTEffect
{
public:
	//==============================================================================

	CVASTPhaser(VASTAudioProcessor* processor, CVASTSettings& set, int busnr);
	~CVASTPhaser();

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

	class Filter : public IIRFilter
	{
	public:
		void updateCoefficients(const double discreteFrequency) noexcept
		{
			//jassert(discreteFrequency > 0);

			double wc = jmin(discreteFrequency, M_PI * 0.99);
			double tan_half_wc = tan(wc / 2.0);

			coefficients = IIRCoefficients(/* b0 */ tan_half_wc - 1.0,
				/* b1 */ tan_half_wc + 1.0,
				/* b2 */ 0.0,
				/* a0 */ tan_half_wc + 1.0,
				/* a1 */ tan_half_wc - 1.0,
				/* a2 */ 0.0);

			setCoefficients(coefficients);
		}
	};

private:
	void updateLFOFreq();

	std::atomic<float>* m_bPhaserOnOff = nullptr;
	std::atomic<float>* m_fPhaserDryWet = nullptr;
	std::atomic<float>* m_fPhaserWidth = nullptr;
	std::atomic<float>* m_fPhaserFeedback = nullptr;
	std::atomic<float>* m_fPhaserMinFrequency = nullptr;
	std::atomic<float>* m_fPhaserLFOFreq = nullptr;
	std::atomic<float>* m_uPhaserLFOWave = nullptr;
	std::atomic<float>* m_bPhaserSynch = nullptr;
	std::atomic<float>* m_uPhaserTimeBeats = nullptr;
	std::atomic<float>* m_fPhaserGain = nullptr;

	LinearSmoothedValue<float> m_fPhaserWidth_smoothed;
	LinearSmoothedValue<float> m_fPhaserFeedback_smoothed;
	LinearSmoothedValue<float> m_fPhaserMinFrequency_smoothed;
	LinearSmoothedValue<float> m_fPhaserLFOFreq_smoothed;
	LinearSmoothedValue<float> m_fPhaserDryWet_smoothed;
	LinearSmoothedValue<float> m_fPhaserGain_smoothed;

	CVASTWaveTableOscillator m_LFO;

	AudioSampleBuffer m_fBuffer;
	CVASTSettings* m_Set;

	OwnedArray<Filter> filters;
	Array<float> filteredOutputs;
	float inverseSampleRate = 0.f;
	int numFiltersPerChannel;
	unsigned int sampleCountToUpdateFilters;
	unsigned int updateFiltersInterval;

	void updateFilters(double centreFrequency);
	JUCE_LEAK_DETECTOR(CVASTPhaser)
};
