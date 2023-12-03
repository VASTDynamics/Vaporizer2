/*
VAST Dynamics Audio Software (TM)
*/

#pragma once
#include "../VASTSettings.h"
#include "VASTEffect.h"
#include "FXUtils/STFT.h"
#include "../Oscillator/VASTWaveTableOscillator.h"  

class CVASTAtomizer : public CVASTEffect, public STFT
{
public:
	//==============================================================================

	CVASTAtomizer(VASTAudioProcessor* processor, CVASTSettings& set, int busnr);
	~CVASTAtomizer();

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

	void setHarmonics(float harmonics);
	float m_harmonics = 0.0f;

	void setDephase(float dephase);
	float m_dephase = 0.0f;

	void setLowcut(float lowcut);
	float m_lowcut = 0.0f;

	void setHighcut(float highcut);
	float m_highcut = 0.0f;

	void setEmphasis(float emphasis);
	float m_emphasis = 0.0f;

	void setGain(float gain);
	float m_gain = 0.0f;

	float m_transients = 0.0f;
	void setLFO(float lfo);
	float m_lfo = 0.0f;

	void modification() override;
	void processSTFTBlock(AudioSampleBuffer& block, const int numSamples);

	//======================================

private:
	void updateLFOFreq();

	std::atomic<float>* m_bAtomizerOnOff = nullptr;
	std::atomic<float>* m_fAtomizerDryWet = nullptr;
	std::atomic<float>* m_fAtomizerLFOFreq = nullptr;
	std::atomic<float>* m_uAtomizerLFOWave = nullptr;
	std::atomic<float>* m_fAtomizerHarmonics = nullptr;
	std::atomic<float>* m_fAtomizerEmphasis = nullptr;
	std::atomic<float>* m_fAtomizerLowcut = nullptr;
	std::atomic<float>* m_fAtomizerHighcut = nullptr;
	std::atomic<float>* m_fAtomizerDephase = nullptr;
	std::atomic<float>* m_uAtomizerTimeBeats = nullptr;
	std::atomic<float>* m_bAtomizerSynch = nullptr;
	std::atomic<float>* m_fAtomizerGain = nullptr;

	LinearSmoothedValue<float> m_fAtomizerLFOFreq_smoothed;
	LinearSmoothedValue<float> m_fAtomizerDryWet_smoothed;
	LinearSmoothedValue<float> m_fAtomizerLowCut_smoothed;
	LinearSmoothedValue<float> m_fAtomizerHighCut_smoothed;
	LinearSmoothedValue<float> m_fAtomizerHarmonics_smoothed;
	LinearSmoothedValue<float> m_fAtomizerEmphasis_smoothed;
	LinearSmoothedValue<float> m_fAtomizerDephase_smoothed;
	LinearSmoothedValue<float> m_fAtomizerGain_smoothed;

	AudioSampleBuffer m_fBuffer;

	CVASTWaveTableOscillator m_LFO;

	CVASTSettings* m_Set;

	JUCE_LEAK_DETECTOR(CVASTAtomizer)
};
