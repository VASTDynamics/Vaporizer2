/*
VAST Dynamics Audio Software (TM)

Has:
- VASTOscillator
- VASTVca
- VASTVcf
*/

#pragma once

#include "VASTEngineHeader.h"
#include "Oscillator/VASTWaveTableOscillator.h"
#include "VASTVca.h"
#include "VASTVcf.h"
#include "VASTSettings.h"
#include "VASTSynthesiser.h"
#include <vector>

class CVASTPoly; //forward declaration
class CVASTVcf; //forward declaration

typedef struct sGrainTable {
	double samplePitchRatio = 0;
	double sourceSamplePosition = 0;
	double origSampleStartPosition = 0;
	double realPos = 0;
	double samplePositionRandomIncrement = 0;
	double delayedStart = 0;
	double rangeEnd = 0;
	float sampler_lgain = 0.f;
	float sampler_rgain = 0.f;
	float sampler_attackReleaseLevel = 0.f;
	float sampler_fadeFactor = 0.f;
	float sampler_attackDelta = 0.f;
	float sampler_releaseDelta = 0.f;
	bool sampler_isInAttack = false;
	bool sampler_isInRelease = false;
	bool bwasLooped = false;
	bool bLoopPingpong = false;
	int iLoopEnd_old = 0;
} sGrainTable;

class CVASTSingleNote : public VASTSynthesiserVoice
{
public:
	CVASTSingleNote();
	virtual ~CVASTSingleNote(void);

	CVASTSettings* m_Set; //public due to oscilloscope test
	CVASTPoly* m_Poly; 

	void init(CVASTSettings &set, CVASTPoly* poly, MYUINT voiceNo);

	void prepareForPlay();
	void updateVariables();

	MYUINT getChannel() const;
	MYUINT getMIDINote() const;
	bool noteOn(MYUINT uChannel, MYUINT uMIDINote, MYUINT uVelocity, bool legato); //returns success
	void noteOff(float releaseVelocity) ;
	bool isPlayingInRange(int startsample, int numsamples) const;
	bool isPlayingAtSamplePosition(int sample) const;
	bool isPlayingCalledFromUI() const;
	void processBuffer(sRoutingBuffers& routingBuffers, int startSample, int numSamples);

	void generate_normalized_irrationals(float *destination, int count);

	void setGlissandoStart(int midinote, bool reset);
	void setPortamentoTime(float time); //in s
	
    int getNumOscsPlaying() const;

	//==============================================================================
	//from SynthesiserVoice
	bool canPlaySound(SynthesiserSound*) override;
	void startNote(int midiNoteNumber, float velocity, SynthesiserSound* sound, int currentPitchWheelPosition, bool legato) override;
	void stopNote(float velocity, bool allowTailOff) override;
	void pitchWheelMoved(int newPitchWheelValue, bool zone) override;
	void controllerMoved(int controllerNumber, int newControllerValue) override;
	void aftertouchChanged(int newAftertouchValue) override;
	void channelPressureChanged(int newChannelPressureValue) override;
	void renderNextBlock(sRoutingBuffers& routingBuffers, int startSample, int numSamples) override;
	void samplerRenderNextBlock(AudioSampleBuffer* outputBuffer, int startSample, int numSamples);
	bool isPlayingButReleased() override;
	void clearCurrentNote() override;

	void samplerNewGrain(int delayedStart, float samplerGrainShape, float samplePositionRandomIncrement);
	void samplerAddGrain(sGrainTable newGrain);
	std::vector<sGrainTable> m_grainTable;
	double m_samplePitchRatio = 0;
	int m_grainSampleCount = 0;
	int m_allSampleCount = 0;

	//==============================================================================
	void samplerUpdatePitch(VASTSamplerSound* sound, bool force);
	int m_samplerMidiNoteNumber = 0;
	int getVoiceNo() const;
	ULong64_t m_startPlayTimestamp = 0;

	void setWTPosSmooth(int bank);
	void setWTPosSmooth(int bank, float morph);
	void resetSmoothers();
	
	// public instances
    std::unique_ptr<CVASTVca> m_VCA;
	OwnedArray<CVASTVcf> m_VCF;

	// instances
	OwnedArray<CVASTWaveTableOscillator> m_Oscillator;
    std::unique_ptr<CVASTWaveTableOscillator> m_OscillatorNoise;
	OwnedArray<CVASTWaveTableOscillator> m_LFO_Osc; // LFOs 1-5

	MYUINT m_uChannel = 0;
	MYUINT m_uMIDINote = 0;
	MYUINT m_uVelocity = 0;

    bool m_bLastFilterOutputZero[3] = {true, true, true}; //filters

	int m_iCurCycleSamples[4] = { 0, 0, 0, 0 };
	int m_iLastCycleSamples[4] = { 0, 0, 0, 0 };

	LinearSmoothedValue<float> m_wtFXVal_smoothed[4];
	float m_wtFXVal[4] {0.f,0.f,0.f,0.f};
	int m_wtFXType[4]{ 0, 0, 0, 0 };
	bool m_wtFXTypeChanged[4]{ false, false, false, false };
	std::atomic<float> m_currentWTPosFloatPercentage[4] { 0.f,0.f,0.f,0.f }; //bank
	std::atomic<float> m_safePhaseFloat[4]{ 0.f,0.f,0.f,0.f }; //bank

private:
	void nextNote(bool legato);

	MYUINT m_uChannelNext = 0;
	MYUINT m_uMIDINoteNext = 0;
	MYUINT m_uVelocityNext = 0;
	int m_iNumParallelOsc = 0;

	float m_fOscAMaxPeak = 1.0f;
	float m_fOscADivisor = 1.0f;
	float m_fOscBMaxPeak = 1.0f;
	float m_fOscBDivisor = 1.0f;
	float m_fOscCMaxPeak = 1.0f;
	float m_fOscCDivisor = 1.0f;
	float m_fOscDMaxPeak = 1.0f;
	float m_fOscDDivisor = 1.0f;
	float m_fNoiseMaxPeak = 1.0f;
	float m_fNoiseDivisor = 1.0f;
	void writeDebugInfo();

    std::atomic<MYUINT> m_uLastuNumOscAOscsPlaying = 0;
    std::atomic<MYUINT> m_uLastuNumOscBOscsPlaying = 0;
    std::atomic<MYUINT> m_uLastuNumOscCOscsPlaying = 0;
    std::atomic<MYUINT> m_uLastuNumOscDOscsPlaying = 0;
	std::atomic<MYUINT> m_uLast_NumTotalPlaying = 0;

	void syncOscToMaster(int bank, int i);
	
    std::unique_ptr<AudioSampleBuffer> m_centerBuffer;
    std::unique_ptr<AudioSampleBuffer> m_velocityBuffer;

	float mSpread[4] { 0.f,0.f,0.f,0.f }; //per bank

	float m_pitchBendNote = 1.f; //for sampler

	LinearSmoothedValue<float> m_SamplerGrainsGain_smoothed;
	LinearSmoothedValue<float> m_fOscWTPos_smoothed[4]; //bank
	float m_fOscWTPos_atNoteOn[4] = { 0.f, 0.f, 0.f, 0.f };
	LinearSmoothedValue<float> m_fPhaseOffset_smoothed[4]; //bank

	void resetSoftFadeState();
	bool m_bSoftFadeCycleStarted[4] = {false, false, false, false};
	bool m_bSoftFadeCycleEnded[4] = { false, false, false, false };
	bool m_bIsStartOfCycle[4] = { false, false, false, false };
	bool m_bIsFirstCycle[4] = { false, false, false, false };	
	bool m_bFreqsHaveToBeDoneForEachSample[4] = { false, false, false, false };
	int m_iLastFreqUpdate[4] = { 0, 0, 0, 0 };
#ifdef _DEBUG
	int m_bLastRenderedWTID[4] = { -1, -1, -1, -1 };
	float m_fLastRenderedSample[4] = { 0.f, 0.f, 0.f, 0.f }; //leftonly
#endif

	std::shared_ptr<CVASTWaveTable> m_localVoiceBankWavetable[4] = { nullptr, nullptr, nullptr, nullptr }; //can be nullptr, can hold a WT longer than bank, bank is only copy template
	std::shared_ptr<CVASTWaveTable> m_localVoiceBankWavetableSoftfade[4] = { nullptr, nullptr, nullptr, nullptr }; //can be nullptr, can hold a WT longer than bank, bank is only copy template

	float normalized_irrational[4][C_MAX_PARALLEL_OSC + 1]{}; //for detune / "fixed detune"
	float delta[4][C_MAX_PARALLEL_OSC + 1]{};
	float detune[4][C_MAX_PARALLEL_OSC]{};

	void updateDetune(int bank, float detuneValue, bool updateFrequency);
	bool prepareNextPhaseCycle(int bank, int skips, int startSample, bool bTakeNextValue, bool wtfxFXTypeChanged);
	bool prepareEachSample(int bank, int currentFrame, bool &freqsHaveToBeDoneForEachSample, bool bTakeNextValue, CVASTWaveTableOscillator* l_Oscillator[]);
	bool prepareFrequency(int bank, int skips, int startSample, bool bTakeNextValue, bool bIsStartOfCycle);
	void initWavetableProcessing(int bank, sRoutingBuffers& routingBuffers, modMatrixInputState& inputState);
	void setTargetWTPos(int bank, float targetWTPosPercentage, bool takeNext);
	void doWavetableStep(const int bank, const int currentFrame, const int firstFrame);
	void doWavetableBufferGet(const int bank, CVASTWaveTableOscillator* mOscillator, const int osciCount, AudioSampleBuffer* lOscBuffer, const int startSample, const int numSamples);

	LinearSmoothedValue<float> m_fVoiceGain_smoothed;
	LinearSmoothedValue<float> m_fOscAGain_smoothed;
	LinearSmoothedValue<float> m_fOscBGain_smoothed;
	LinearSmoothedValue<float> m_fOscCGain_smoothed;
	LinearSmoothedValue<float> m_fOscDGain_smoothed;
	LinearSmoothedValue<float> m_fNoiseGain_smoothed;
	LinearSmoothedValue<float> m_fSamplerGain_smoothed;
	LinearSmoothedValue<float> m_fOscAPan_smoothed;
	LinearSmoothedValue<float> m_fOscBPan_smoothed;
	LinearSmoothedValue<float> m_fOscCPan_smoothed;
	LinearSmoothedValue<float> m_fOscDPan_smoothed;
	LinearSmoothedValue<float> m_fNoisePan_smoothed;
	LinearSmoothedValue<float> m_fSamplerPan_smoothed;
	
	LinearSmoothedValue<float> m_fPitchMod_smoothed;
	LinearSmoothedValue<float> m_fPitchModSampler_smoothed;
	LinearSmoothedValue<float> m_fOscACents_smoothed;
	LinearSmoothedValue<float> m_fOscBCents_smoothed;
	LinearSmoothedValue<float> m_fOscCCents_smoothed;
	LinearSmoothedValue<float> m_fOscDCents_smoothed;
	LinearSmoothedValue<float> m_fSamplerBaseFreqPortamento_smoothed;
	LinearSmoothedValue<float> m_fSamplerCents_smoothed;

	bool m_bSmoothersTakeNextValue = false;
	JUCE_LEAK_DETECTOR(CVASTSingleNote)
};

