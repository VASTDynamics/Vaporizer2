/*
VAST Dynamics Audio Software (TM)
*/

#pragma once

#include "../VASTEngineHeader.h"
#include "../VASTPluginConstants.h"
#include "../VASTSettings.h"
#include "../Filter/VASTDelay.h"
#include "FXUtils/VASTDDLModule.h"
#include "FXUtils/VASTModDelayModule.h"
#include "VASTEffect.h"

class CVASTChorus : public CVASTEffect
{
public:
	//==============================================================================

	CVASTChorus(VASTAudioProcessor* processor, CVASTSettings& set, int busnr);
	~CVASTChorus();

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
		
	CModDelayModule m_ModDelayLeft;
	CModDelayModule m_ModDelayCenter;
	CModDelayModule m_ModDelayRight;

	void updateModules(); // you could split this out into smaller functions

	float m_fCrossMix;
	float m_fHPF_Fc;

private:
	void updateLFOFreq();

	std::atomic<float>* m_bChorusOnOff = nullptr;
	std::atomic<float>* m_fChorusDryWet = nullptr;
	std::atomic<float>* m_fChorusRate_hz = nullptr;
	std::atomic<float>* m_fChorusDepth = nullptr;
	std::atomic<float>* m_bChorusSynch = nullptr;
	std::atomic<float>* m_uChorusTimeBeats = nullptr;
	std::atomic<float>* m_fChorusGain = nullptr;

	LinearSmoothedValue<float> m_fChorusRate_hz_smoothed;
	LinearSmoothedValue<float> m_fChorusDepth_smoothed;
	LinearSmoothedValue<float> m_fChorusDryWet_smoothed;
	LinearSmoothedValue<float> m_fChorusGain_smoothed;

	CVASTSettings *m_Set;
	
	float m_fGain = 0.0f;
	float m_fFrequency = 1.0f;
	float m_fDepth = 1.0f;
	float m_fDrywetMod = 1.0f;

	JUCE_LEAK_DETECTOR(CVASTChorus)
};
