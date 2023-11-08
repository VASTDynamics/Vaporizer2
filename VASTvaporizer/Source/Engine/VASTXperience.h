/*
VAST Dynamics Audio Software (TM)
*/

#pragma once

// base class
#include "VASTEngineHeader.h"
#include "VASTSettings.h"
#include "VASTPoly.h"
#include "FX/VASTEffect.h"
#include "FX/FXUtils/VASTDDLModule.h"
#include "VASTFXBus.h"
#include "Filter/VASTBiQuadFilter.h"
#include "VASTParamState.h"
#include "../Plugin//VASTScopeDisplay/VASTRingBuffer.h"

class VASTAudioProcessor; //forward declaration
						  //class CUIControlList; //forward declaration
						  //class CUICtrl; //forward declaration

class CVASTXperience : public CVASTEffect
{
public:
	CVASTXperience(VASTAudioProcessor* processor);
	virtual ~CVASTXperience(void);

	// Prepare For Play Function is called just before audio streams
	bool prepareForPlay(double sampleRate, int expectedSamplesPerBlock);

	bool initializeEngine();

	/*
	bool midiNoteOn(MYUINT uChannel, MYUINT uMIDINote, MYUINT uVelocity);
	bool midiNoteOff(MYUINT uChannel, MYUINT uMIDINote, MYUINT uVelocity, bool bAllNotesOff);
	bool midiModWheel(MYUINT uChannel, MYUINT uModValue);
	bool midiPitchBend(MYUINT uChannel, int nActualPitchBendValue, float fNormalizedPitchBendValue);
	bool midiAftertouch(MYUINT uChannel, int nAfterTouchValue);
	*/

	//bool midiClock();	
	//bool midiMessage(unsigned char cChannel, unsigned char cStatus, unsigned char cData1, unsigned char cData2);

	bool processAudioBuffer(AudioSampleBuffer& buffer, MidiBuffer& midiMessages, MYUINT uNumChannels, bool isPlaying,
		double ppqPosition, bool isLooping, double ppqLoopStart, double ppqLoopEnd, double ppqPositionOfLastBarStart, double bpm);

	//VASTEffectInterface
	void init(CVASTSettings &set) {};
	void initCompatibilityParameters() {}; //implemented in state
	void initCompatibilityParameters5() override {}; //new parameters go here
	void prepareToPlay(double sampleRate, int samplesPerBlock) {};
	void releaseResources() {};
	void processBlock(AudioSampleBuffer&, MidiBuffer&, const int numSamples) {};
	void getStateInformation(MemoryBlock& destData) {};
	void setStateInformation(const void* data, int sizeInBytes) {};
	void updateTiming() {};
	
	void parameterChanged(const String& parameterID, float newValue);

	CVASTSettings m_Set;
	VASTAudioProcessor* myProcessor;

	std::shared_ptr<CVASTParamState> m_UIState;   //the UI Param State - copied to m_Set atomically!

	CVASTPoly m_Poly;

	CVASTFXBus m_fxBus1;
	CVASTFXBus m_fxBus2;
	CVASTFXBus m_fxBus3;

	bool m_bOversampleBus1_changed = false;
	bool m_bOversampleBus2_changed = false;
	bool m_bOversampleBus3_changed = false;

	float fOutRLast = 0;
	float fOutLLast = 0;

	void audioProcessLock();
	void audioProcessUnlock();
	std::atomic<bool> m_BlockProcessing = false;
	std::atomic<bool> m_BlockProcessingIsBlockedSuccessfully = false;
	bool getBlockProcessingIsBlockedSuccessfully();
	bool getBlockProcessing();
	bool nonThreadsafeIsBlockedProcessingInfo();

    std::unique_ptr<AudioSampleBuffer> m_oversampledBuffer;

    std::atomic<bool> m_bLastChainBufferZero = false;
    std::atomic<int> m_bBufferZeroMilliSeconds = 0;

    std::atomic<int> m_nSampleRate = 44100; //default

    std::atomic<int> m_iFadeOutSamples = 0;
    std::atomic<int> m_iFadeInSamples = 0;
    std::atomic<int> m_iMaxFadeSamples = 2000;

	// I/O capabilities
	MYUINT m_uMaxInputChannels;
	MYUINT m_uMaxOutputChannels;

	// prebuilt table of MIDI Note Numbers --> Pitch Frequencies
	float m_MIDIFreqTable[128];

	// impulse response buffers
	float m_h_Left[1024];
	float m_h_Right[1024];

	void beginSoftFade();
	void endSoftFade();

	std::atomic<bool> m_isPreparingForPlay = false;

	// Audio & GL Audio Buffer
	std::unique_ptr<VASTRingBuffer<GLfloat>> oscilloscopeRingBuffer;

	JUCE_HEAVYWEIGHT_LEAK_DETECTOR(CVASTXperience)
	//JUCE_LEAK_DETECTOR(CVASTXperience)
};
