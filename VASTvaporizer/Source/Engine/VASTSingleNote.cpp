/*
VAST Dynamics Audio Software (TM)

Has:
- VASTOscillator
- VASTVcf
- VASTVca
*/

#include "VASTSingleNote.h"
#include "VASTEngineHeader.h"
#include "VASTSettings.h"
#include "VASTSampler.h"
#include "VASTPoly.h"
#include "VASTVcf.h"
#include "VASTSynthesiser.h"
#include "VASTOscillatorBank.h"
#include "Utils/VASTSynthfunctions.h"
#include "Utils/VASTSSEHelper.h"
#include "../Plugin/VASTAudioProcessor.h"
#ifdef _WINDOWS
#include <time.h>
#else
#include <sys/time.h>
#endif

//for debug info
#include <iostream>
#include <fstream>
#include <string>

CVASTSingleNote::CVASTSingleNote(CVASTSettings& set, CVASTPoly* poly, MYUINT voiceNo):
	m_Set(&set), 
    m_Poly(poly),
	m_VCA{ set, voiceNo },
    m_Oscillator{{ set, &poly->m_OscBank[0] }, { set, &poly->m_OscBank[1] }, { set, &poly->m_OscBank[2] }, { set, &poly->m_OscBank[3]} }, //bank 1-4
    m_OscillatorNoise(set, nullptr),
    m_LFO_Osc{{ set, nullptr }, { set, nullptr }, { set, nullptr }, { set, nullptr }, { set, nullptr } } //LFO 1-5
{
	mVoiceNo.store(voiceNo);
	for (int bank = 0; bank < 4; bank++) {
        m_Oscillator[bank].init();
	}
	m_OscillatorNoise.init();
	
	m_VCA.init();
    for (int filter = 0; filter < 3; filter++) {
        m_VCF.add(new CVASTVcf(*m_Set, mVoiceNo, filter, false)); //has to be new alloacated due to alignas(16)
        m_VCF[filter]->init(); //not UI
    }	

    m_LFO_Osc[0].init();
    m_LFO_Osc[0].updateMainVariables(m_Set->m_nSampleRate, *m_Set->m_State->m_uLFOWave_LFO1, 1, 0, 0, 0);
    m_LFO_Osc[1].init();
    m_LFO_Osc[1].updateMainVariables(m_Set->m_nSampleRate, *m_Set->m_State->m_uLFOWave_LFO2, 1, 0, 0, 0);
    m_LFO_Osc[2].init();
    m_LFO_Osc[2].updateMainVariables(m_Set->m_nSampleRate, *m_Set->m_State->m_uLFOWave_LFO3, 1, 0, 0, 0);
    m_LFO_Osc[3].init();
    m_LFO_Osc[3].updateMainVariables(m_Set->m_nSampleRate, *m_Set->m_State->m_uLFOWave_LFO4, 1, 0, 0, 0);
    m_LFO_Osc[4].init();
    m_LFO_Osc[4].updateMainVariables(m_Set->m_nSampleRate, *m_Set->m_State->m_uLFOWave_LFO5, 1, 0, 0, 0);
}

/* destructor()
Destroy variables allocated in the contructor()

*/
CVASTSingleNote::~CVASTSingleNote()
{
	m_centerBuffer = nullptr;
	m_velocityBuffer = nullptr;
}

void CVASTSingleNote::init() { //called once
	m_iNumParallelOsc = 0;

	m_uChannel = 0;
	m_uChannelNext = 0;
	m_uMIDINote = 0;
	m_uMIDINoteNext = 0;
	m_uVelocity = 0;
	m_uVelocityNext = 0;

	m_uLastuNumOscAOscsPlaying = 0;
	m_uLastuNumOscBOscsPlaying = 0;
	m_uLastuNumOscCOscsPlaying = 0;
	m_uLastuNumOscDOscsPlaying = 0;

	resetSmoothers();

	int initSize = 16;
	for (int bank = 0; bank < 4; bank++) {
		m_currentWTPosFloatPercentage[bank] = 0.0f;
		//m_lastSectionWtPos[bank] = 0.f;
		m_safePhaseFloat[bank] = 0.f;
		mSpread[bank] = 0.f;
		m_wtFXVal[bank] = 0.f;
		m_wtFXType[bank] = 0;
		m_wtFXTypeChanged[bank] = false;
	}	
	m_centerBuffer = std::make_unique<AudioSampleBuffer>(1, initSize);
	m_velocityBuffer = std::make_unique<AudioSampleBuffer>(1, initSize);
}

void CVASTSingleNote::resetSmoothers() {
	//duplicate code here - see below
	double smoothTime = 0.002;
	m_fVoiceGain_smoothed.reset(m_Set->m_nSampleRate, smoothTime);
	m_fOscAGain_smoothed.reset(m_Set->m_nSampleRate, smoothTime);
	m_fOscBGain_smoothed.reset(m_Set->m_nSampleRate, smoothTime);
	m_fOscCGain_smoothed.reset(m_Set->m_nSampleRate, smoothTime);
	m_fOscDGain_smoothed.reset(m_Set->m_nSampleRate, smoothTime);
	m_fNoiseGain_smoothed.reset(m_Set->m_nSampleRate, smoothTime);
	m_fSamplerGain_smoothed.reset(m_Set->m_nSampleRate, smoothTime);

	m_fOscAPan_smoothed.reset(m_Set->m_nSampleRate, 0.03);
	m_fOscBPan_smoothed.reset(m_Set->m_nSampleRate, 0.03);
	m_fOscCPan_smoothed.reset(m_Set->m_nSampleRate, 0.03);
	m_fOscDPan_smoothed.reset(m_Set->m_nSampleRate, 0.03);
	m_fNoisePan_smoothed.reset(m_Set->m_nSampleRate, 0.03);
	m_fSamplerPan_smoothed.reset(m_Set->m_nSampleRate, 0.03);
	
	m_fSamplerCents_smoothed.reset(m_Set->m_nSampleRate, smoothTime);
	m_fPitchModSampler_smoothed.reset(m_Set->m_nSampleRate, smoothTime);

	
	smoothTime = 0.00002;
	m_fPitchMod_smoothed.reset(m_Set->m_nSampleRate, smoothTime);
	m_fOscACents_smoothed.reset(m_Set->m_nSampleRate, smoothTime);
	m_fOscBCents_smoothed.reset(m_Set->m_nSampleRate, smoothTime);
	m_fOscCCents_smoothed.reset(m_Set->m_nSampleRate, smoothTime);
	m_fOscDCents_smoothed.reset(m_Set->m_nSampleRate, smoothTime);

	//CHECK smoothTime = 0.1;  // 0.1 too low for hardsync?
	
	for (int bank = 0; bank < 4; bank++) {
		setWTPosSmooth(bank);
		//smoothTime = 0.000000001; //was 0.05 - changed 19.10.2019 - off
		//smoothTime = 0.05; //was 0.05 - changed 19.10.2019 - off
   	    //smoothTime = 0.00008; //was 0.05 - changed 19.10.2019 - 0.00008 ~ 3 samples
		//smoothTime = 0.001; //was 0.05 - changed 19.10.2019 - 0.001 ~ 44-48 samples
		//smoothTime = 0.0008; //was 0.05 - changed 19.10.2019
		//smoothTime = 0.00005; //was 0.05 - changed 19.10.2019
		//m_fOscWTPos_smoothed[bank].reset(m_Set->m_nSampleRate, smoothTime);
		smoothTime = 0.0001;  // 0.1 too low for hardsync?
		m_fPhaseOffset_smoothed[bank].reset(m_Set->m_nSampleRate, smoothTime);
		//m_fMorph_smoothed[bank].reset(m_Set->m_nSampleRate, smoothTime);

		smoothTime = 0.0001; //was 0.00005;
		m_wtFXVal_smoothed[bank].reset(m_Set->m_nSampleRate, smoothTime); //sample rate change??		
	}
	   
	m_bSmoothersTakeNextValue = true;	
}

void CVASTSingleNote::setWTPosSmooth(int bank, float morph) {
	double minVal = 0.000000001; //was 0.05 - changed 19.10.2019 - off
	double maxVal = 0.01; //500 samples
	double smoothTime = minVal + ((100.f - morph) * 0.01f) * (maxVal - minVal);
	float saveCur = m_fOscWTPos_smoothed[bank].getCurrentValue();
	float saveTarget = m_fOscWTPos_smoothed[bank].getCurrentValue();
	m_fOscWTPos_smoothed[bank].reset(m_Set->m_nSampleRate, smoothTime);
	m_fOscWTPos_smoothed[bank].setCurrentAndTargetValue(saveCur);
	m_fOscWTPos_smoothed[bank].setTargetValue(saveTarget);
}

void CVASTSingleNote::setWTPosSmooth(int bank) {
	float fMorph = 0.f;
	if (bank == 0)
		fMorph = *m_Set->m_State->m_fOscMorph_OscA;
	else if (bank == 1)
		fMorph = *m_Set->m_State->m_fOscMorph_OscB;
	else if (bank == 2)
		fMorph = *m_Set->m_State->m_fOscMorph_OscC;
	else if (bank == 3)
		fMorph = *m_Set->m_State->m_fOscMorph_OscD;
	setWTPosSmooth(bank, fMorph);
}

void CVASTSingleNote::prepareForPlay() {
	for (int filter = 0; filter < 3; filter++) {
		m_VCF[filter]->prepareForPlay();
	}
	for (int bank = 0; bank < 4; bank++) {
		m_Oscillator[bank].init();
		m_Oscillator[bank].prepareForPlay(m_Set->m_nExpectedSamplesPerBlock);
		//m_lastSectionWtPos[bank] = 0.f; //needed?
	}
	for (int mseg = 0; mseg < 5; mseg++) {
		m_VCA.m_MSEG_Envelope[mseg].reset();
	}

	m_centerBuffer->setSize(1, m_Set->m_nExpectedSamplesPerBlock, false, false, false); //free  memory
	m_velocityBuffer->setSize(1, m_Set->m_nExpectedSamplesPerBlock, false, false, false); //free  memory

	m_OscillatorNoise.init();
	updateVariables();
	
	m_LFO_Osc[0].init();
	m_LFO_Osc[0].updateMainVariables(m_Set->m_nSampleRate, *m_Set->m_State->m_uLFOWave_LFO1, 1, 0, 0, 0);
	m_LFO_Osc[1].init();
	m_LFO_Osc[1].updateMainVariables(m_Set->m_nSampleRate, *m_Set->m_State->m_uLFOWave_LFO2, 1, 0, 0, 0);
	m_LFO_Osc[2].init();
	m_LFO_Osc[2].updateMainVariables(m_Set->m_nSampleRate, *m_Set->m_State->m_uLFOWave_LFO3, 1, 0, 0, 0);
	m_LFO_Osc[3].init();
	m_LFO_Osc[3].updateMainVariables(m_Set->m_nSampleRate, *m_Set->m_State->m_uLFOWave_LFO4, 1, 0, 0, 0);
	m_LFO_Osc[4].init();
	m_LFO_Osc[4].updateMainVariables(m_Set->m_nSampleRate, *m_Set->m_State->m_uLFOWave_LFO5, 1, 0, 0, 0);

	m_bLastFilterOutputZero[0] = false; m_bLastFilterOutputZero[1] = false; m_bLastFilterOutputZero[2] = false;

	resetSmoothers();
}

//==============================================================================
//from SynthesiserVoice

void CVASTSingleNote::clearCurrentNote()
{
	if (currentlyPlayingNote != -1) {
		VDBG("Note is cleared - voice is not playing / active anymore: " << getVoiceNo());
		currentlyPlayingNote = -1;
		currentlyPlayingSound = nullptr;
		currentPlayingMidiChannel = 0;

		//remove oscbank softfade
		for (int bank = 0; bank < 4; bank++) {
			m_Poly->m_OscBank[bank].removeSingleNoteSoftFadeCycle(mVoiceNo);
		}
		resetSoftFadeState();
	}
}


bool CVASTSingleNote::canPlaySound(SynthesiserSound*)
{
	
	if ((*m_Set->m_State->m_bLegatoMode == static_cast<int>(SWITCH::SWITCH_OFF)) || (m_Set->m_uMaxPoly > 1)) {
		//polymode handled in synthesizer voice
		return true;
	} 
	else if (mVoiceNo < 1) //mono legato
		return true;
	return false;	
}

bool CVASTSingleNote::isPlayingButReleased() {
	bool isOff = m_VCA.isNoteOff();
	return (isVoiceActive() &&
		isOff);
	//return isVoiceActive() && !(isKeyDown() || isSostenutoPedalDown() || isSustainPedalDown());
}

void CVASTSingleNote::startNote(int midiNoteNumber, float velocity, SynthesiserSound* sound, int currentPitchWheelPosition, bool legato)
{		
	//retrig lfo?
	if (*m_Set->m_State->m_bLFORetrigOnOff_LFO1 == static_cast<int>(SWITCH::SWITCH_ON)) {
		if (*m_Set->m_State->m_bLFOPerVoice_LFO1 == static_cast<int>(SWITCH::SWITCH_ON)) {
			m_LFO_Osc[0].resynch(0, true);
		}
	}
	if (*m_Set->m_State->m_bLFORetrigOnOff_LFO2 == static_cast<int>(SWITCH::SWITCH_ON)) {
		if (*m_Set->m_State->m_bLFOPerVoice_LFO2 == static_cast<int>(SWITCH::SWITCH_ON)) {
			m_LFO_Osc[1].resynch(0, true);
		}
	}
	if (*m_Set->m_State->m_bLFORetrigOnOff_LFO3 == static_cast<int>(SWITCH::SWITCH_ON)) {
		if (*m_Set->m_State->m_bLFOPerVoice_LFO3 == static_cast<int>(SWITCH::SWITCH_ON)) {
			m_LFO_Osc[2].resynch(0, true);
		}
	}
	if (*m_Set->m_State->m_bLFORetrigOnOff_LFO4 == static_cast<int>(SWITCH::SWITCH_ON)) {
		if (*m_Set->m_State->m_bLFOPerVoice_LFO4 == static_cast<int>(SWITCH::SWITCH_ON)) {
			m_LFO_Osc[3].resynch(0, true);
		}
	}
	if (*m_Set->m_State->m_bLFORetrigOnOff_LFO5 == static_cast<int>(SWITCH::SWITCH_ON)) {
		if (*m_Set->m_State->m_bLFOPerVoice_LFO5 == static_cast<int>(SWITCH::SWITCH_ON)) {
			m_LFO_Osc[4].resynch(0, true);
		}
	}
	
	noteOn(0, midiNoteNumber, velocity, legato);
	pitchWheelMoved(currentPitchWheelPosition, true);  //zone

	//sampler
	if (auto* mySound = dynamic_cast<VASTSynthesiserSound*> (sound))
	{
		VASTSamplerSound* sSound = mySound->getSamplerSound();		
		
		m_grainSampleCount = 0;
		m_allSampleCount = 0;

		if (sSound != nullptr) {
			if (*m_Set->m_State->m_bSamplerPoly == static_cast<int>(SWITCH::SWITCH_OFF)) {
				for (int i = 0; i < m_Set->m_uMaxPoly; i++) {
					//clear all vother voices // all grains
					for (int grain = 0; grain < m_Poly->m_singleNote[i]->m_grainTable.size(); grain++) {
						m_Poly->m_singleNote[i]->m_grainTable[grain].sampler_isInAttack = false;
						m_Poly->m_singleNote[i]->m_grainTable[grain].sampler_isInRelease = true;		
					}
				}
			}

			if (legato) {
#ifdef _DEBUG
				VDBG("SingleNote Sampler startNote legato! voice: " << getVoiceNo());
				//if (m_sampler_isInAttack || m_sampler_isInRelease) 
					//VDBG("!!!! SingleNote Sampler is attack or release voice: " + String(getVoiceNo()));
#endif 
				
				for (int grain = 0; grain < m_grainTable.size(); grain++) {
					m_grainTable[grain].sampler_isInAttack = false;
					m_grainTable[grain].sampler_isInRelease = false;
					m_grainTable[grain].bLoopPingpong = false;
					m_grainTable[grain].bwasLooped = false;
				}

				m_samplerMidiNoteNumber = midiNoteNumber;
				samplerUpdatePitch(sSound, true);
			}
			else {
				m_samplerMidiNoteNumber = midiNoteNumber;
				samplerUpdatePitch(sSound, true);

				m_grainTable.clear();
				modMatrixInputState inputState{ getVoiceNo(), 0 };
				float samplerGrainShape = m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fSamplerGrainShape, MODMATDEST::SamplerGrainShape, &inputState);
				samplerNewGrain(0, samplerGrainShape, 0.f);
			}
		}
	}
	else
		jassertfalse; //can only play this
}

void CVASTSingleNote::stopNote(float velocity, bool allowTailOff)
{	
	//allowtailoff means release is still done. if false it stops immediately --> clicks, dont use
	if (allowTailOff == false)
		if (this->isVoiceActive()) {
			m_VCA.hardStop();
			for (int bank = 0; bank < 4; bank++) {
				m_Poly->m_OscBank[bank].removeSingleNoteSoftFadeCycle(mVoiceNo);
			}
		}

	if (this->isVoiceActive())
		noteOff(velocity);
}

void CVASTSingleNote::pitchWheelMoved(int newPitchWheelValue, bool zone)
{
	bool pbCenter = false;
	if (newPitchWheelValue == 8192)
		pbCenter = true; //fp accuracy
	else 
		pbCenter = false; //fp accuracy	
	
	if (!zone) {
		m_pitchBendNote = 1.f; //for sampler
		if (!pbCenter)
			m_pitchBendNote = powf(2.f, (48.f * (m_Poly->getProcessor()->m_presetData.getCurPatchData().mpebendrange / 48.f) * (newPitchWheelValue / 8192.f - 1.f)) / 12.0); //full bend range means full slide effect
		for (int bank = 0; bank < 4; bank++) {
			// do pitch bender
			m_Oscillator[bank].setPitchBendNote(m_pitchBendNote); 
		}		
	}
	else {
		m_Set->m_bPitchBendCenter = pbCenter;
		m_Set->m_fPitchBend = newPitchWheelValue / 8192.f; // 0..2 center 1	
	}
}

int CVASTSingleNote::getNumOscsPlaying() const {
    return m_uLast_NumTotalPlaying.load();
};

void CVASTSingleNote::controllerMoved(int controllerNumber, int newControllerValue)
{
	if (controllerNumber == 1) { // MIDI CC 1 Modulation, e.g. VSTHost
		m_Set->m_uModWheel.store(jlimit(0, 127, newControllerValue)); //0..127	
	}
	else if (controllerNumber == 74) { // MPE Controller Sound Brightness // handleTimbreMSB 
		VDBG("Brightness MSB voice " << mVoiceNo << " " << newControllerValue);
		m_Set->iMPETimbreMidiNote[getVoiceNo()] = getCurrentlyPlayingNote();
		m_Set->iMPETimbre[getVoiceNo()] = newControllerValue;
		m_Set->iMPETimbre[getVoiceNo()] = jlimit( 0, 127, m_Set->iMPETimbre[getVoiceNo()]);
	}
	else if (controllerNumber == 106) { // MPE Controller Sound Brightness // handleTimbreLSB    
		VDBG("Brightness LSB voice " << mVoiceNo << newControllerValue);
	}
}

void CVASTSingleNote::aftertouchChanged(int newAftertouchValue) { //polyphonic aftertouch
	m_Set->iAftertouchMidiNote[getVoiceNo()] = getCurrentlyPlayingNote();
	m_Set->iAftertouch[getVoiceNo()] = newAftertouchValue;
	m_Set->iAftertouch[getVoiceNo()] = jlimit(0, 127, m_Set->iAftertouch[getVoiceNo()]);
}

void CVASTSingleNote::channelPressureChanged(int newChannelPressureValue) { //monophonic aftertouch
	m_Set->iAftertouchMidiNote[getVoiceNo()] = getCurrentlyPlayingNote(); 
	m_Set->iAftertouch[getVoiceNo()] = newChannelPressureValue;
	m_Set->iAftertouch[getVoiceNo()] = jlimit(0, 127, m_Set->iAftertouch[getVoiceNo()]);
}

void CVASTSingleNote::renderNextBlock(sRoutingBuffers& routingBuffers, int startSample, int numSamples)
{
	processBuffer(routingBuffers, startSample, numSamples);
}

void CVASTSingleNote::samplerAddGrain(sGrainTable newGrain) {
	m_grainTable.push_back(newGrain);
}

void CVASTSingleNote::samplerNewGrain(int delayedStart, float samplerGrainShape, float samplePositionRandomIncrement) {
	VASTSamplerSound* sSound;
	if (auto* playingSound = static_cast<VASTSynthesiserSound*> (getCurrentlyPlayingSound().get()))
	{
		if (playingSound->getSamplerSound() == nullptr) {
			return;
		}
		else
			sSound = playingSound->getSamplerSound();
	}
	else return;

	sGrainTable newGrain;
	newGrain.samplePitchRatio = 0.f; //CHECK
	newGrain.bLoopPingpong = false;
	newGrain.bwasLooped = false;
	newGrain.iLoopEnd_old = -1;
	newGrain.sampler_lgain = 1.f;
	newGrain.sampler_rgain = 1.f;
	newGrain.sampler_isInAttack = (sSound->getAttackSamples() > 0);
	newGrain.sampler_isInRelease = false;
	newGrain.delayedStart = delayedStart;
	newGrain.samplePositionRandomIncrement = samplePositionRandomIncrement;
	newGrain.sampler_fadeFactor = 1.f;

	if ((*m_Set->m_State->m_bSamplerLoopOnly == static_cast<int>(SWITCH::SWITCH_ON)) && (sSound->hasLoop()))
		newGrain.sourceSamplePosition = sSound->getLoopStart();
	else
		newGrain.sourceSamplePosition = 0.0;

	newGrain.origSampleStartPosition = newGrain.sourceSamplePosition;

	sSound->notifyPositionChanged();

	if (newGrain.sampler_isInAttack)
	{
		newGrain.sampler_attackReleaseLevel = 0.0f;
		newGrain.sampler_attackDelta = (float)(m_samplePitchRatio / sSound->getAttackSamples());
	}
	else
	{
		newGrain.sampler_attackReleaseLevel = 1.0f;
		newGrain.sampler_attackDelta = 0.0f;
	}


	if (sSound->getReleaseSamples() > 0)
		//newGrain.sampler_releaseDelta = (float)(-m_samplePitchRatio / sSound->getReleaseSamples());
		newGrain.sampler_releaseDelta = (float)(-1.f / sSound->getReleaseSamples());
	else
		newGrain.sampler_releaseDelta = -1.0f;

	samplerAddGrain(newGrain);
}

void CVASTSingleNote::samplerRenderNextBlock(AudioSampleBuffer* outputBuffer, int startSample, int numSamples)
{
	if (auto* playingSound = static_cast<VASTSynthesiserSound*> (getCurrentlyPlayingSound().get()))
	{
		if (playingSound->getSamplerSound() == nullptr) {
			return;
		}

		VASTSamplerSound* samplerSound = playingSound->getSamplerSound();

		modMatrixInputState inputState{ getVoiceNo(), startSample };
		const int iMaxFadeSteps = 20;
		auto& data = *samplerSound->getAudioData(); //play live data
		int soundLength = samplerSound->getLength(); //play live data
		int loopLength = (samplerSound->hasLoop()) ? samplerSound->getLoopEnd() - samplerSound->getLoopStart() : samplerSound->getLength();
		bool bSwitchSamplerLoop = *m_Set->m_State->m_bSamplerLoop == static_cast<int>(SWITCH::SWITCH_ON);
		bool bSwitchSamplerLoopPingpong = *m_Set->m_State->m_bSamplerLoopPingpong == static_cast<int>(SWITCH::SWITCH_ON);
		bool bSwitchSamplerLoopOnly = *m_Set->m_State->m_bSamplerLoopOnly == static_cast<int>(SWITCH::SWITCH_ON);
		float samplerGrainInteronset = m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fSamplerGrainInteronset, MODMATDEST::SamplerGrainInteronset, &inputState);
		float samplerGrainRandom = m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fSamplerGrainRandom, MODMATDEST::SamplerGrainRandom, &inputState);
		float samplerGrainShape = m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fSamplerGrainShape, MODMATDEST::SamplerGrainShape, &inputState);
		float samplerLoopRange = m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fSamplerLoopRange, MODMATDEST::SamplerLoopRange, &inputState);

		const float* const inL = data.getReadPointer(0);
		const float* const inR = data.getNumChannels() > 1 ? data.getReadPointer(1) : nullptr;

		float* outL = outputBuffer->getWritePointer(0, startSample);
		float* outR = outputBuffer->getNumChannels() > 1 ? outputBuffer->getWritePointer(1, startSample) : nullptr;

		int i_loopStart = 0;

		//inter onset http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.198.9880&rep=rep1&type=pdf
		if (samplerGrainInteronset > 1.f) {//1.f is off
			int maxNumberGrains = (1.f / sqrtf(float(loopLength))) * 20000.f; //500?
			int newGrainAfterSamples = ((101.f - samplerGrainInteronset) / 1000.f) * m_Set->m_nSampleRate; //101.f is param max
			const int mindelay = 10;
			if (newGrainAfterSamples <= mindelay) newGrainAfterSamples = mindelay;
			int delayedStart = 0;
			while (m_grainSampleCount > newGrainAfterSamples) {
				m_grainSampleCount -= newGrainAfterSamples;
				if (m_grainTable.size() < maxNumberGrains) {
					int random = 0;
					if (samplerGrainInteronset > 0.f)
						random = ((float(rand()) / RAND_MAX) - 0.5f) * samplerGrainRandom * 10.f;
					int newDelay = (delayedStart + random >= (0.1f * delayedStart) ? delayedStart + random : 0.1f * delayedStart);
					if (delayedStart == 0) newDelay = 0;
					else
						if (newDelay <= mindelay) newDelay = mindelay;
					float samplePositionRandomIncrement = ((float(rand()) / RAND_MAX) - 0.5f) * samplerGrainRandom * 0.001f; //detune
					samplerNewGrain(newDelay, samplerGrainShape, samplePositionRandomIncrement);
					delayedStart += newGrainAfterSamples;
				}
			}
			m_grainSampleCount += numSamples;
			m_allSampleCount += numSamples; // overflow after 13 hrs? maxint
		}
		else {
			for (int grain = 1; grain < m_grainTable.size(); grain++) {
				m_grainTable[grain].sampler_isInRelease = true;			
			}
		}

		int numGrains = int(m_grainTable.size());

		//For UI only
		for (int grain = 0; grain < numGrains; grain++) {
			m_Poly->m_samplerViewportPosMarker[m_Poly->m_samplerViewportPosMarkerCount] = m_grainTable[grain].realPos;
			m_Poly->m_samplerViewportPosMarkerCount++;
			m_Poly->m_samplerViewportPosMarkerCount = m_Poly->m_samplerViewportPosMarkerCount % C_MAX_SAMPLER_VIEWPORT_MARKERS;
		}

		//do the below per grain
		float gain = 1.f;
		int counter = -1;
		int lnumSamples = numSamples;
		while (--lnumSamples >= 0)
		{
			counter++;
			inputState.currentFrame = startSample + counter;

			float lValGrains = 0.f;
			float rValGrains = 0.f;
			float glSumAttackRelease = 0.f;
			for (int grain = 0; grain < numGrains; grain++) {

				sGrainTable* thisGrain = &m_grainTable[grain];
				thisGrain->samplePitchRatio = m_samplePitchRatio;

				i_loopStart = samplerSound->getLoopStart();
				
				if (samplerSound->hasLoop()) { //only oncem change loop end only on start and when looping
					if (thisGrain->iLoopEnd_old < 0) {
						thisGrain->iLoopEnd_old = i_loopStart + (samplerSound->getLoopEnd() - i_loopStart) * samplerLoopRange * 0.01f;
					}
				}
				else {
					if (thisGrain->iLoopEnd_old < 0) {
						thisGrain->iLoopEnd_old = (soundLength - 1) * samplerLoopRange * 0.01f;
					}
				}
				
				int actualFadeSteps = 0;
				double d_fadeOutPosition = 0;
				double d_fadeInPosition = 0;
				float lOutL = 0.f;
				float lOutR = 0.f;

				if (thisGrain->delayedStart > 0) {
					thisGrain->delayedStart--;
					continue;
				}

				bool l_break = false;

				if (!(bSwitchSamplerLoop || bSwitchSamplerLoopOnly || bSwitchSamplerLoopPingpong))
					thisGrain->iLoopEnd_old = (soundLength - 1);

				if ((thisGrain->iLoopEnd_old - i_loopStart) < (iMaxFadeSteps * thisGrain->samplePitchRatio * 10.f))
					actualFadeSteps = (thisGrain->iLoopEnd_old - i_loopStart) / thisGrain->samplePitchRatio * 0.1f;
				else
					actualFadeSteps = iMaxFadeSteps;
				actualFadeSteps = actualFadeSteps + (samplerGrainShape / 100.f) * (thisGrain->iLoopEnd_old - thisGrain->origSampleStartPosition - actualFadeSteps) * 0.5f;
				vassert(actualFadeSteps >= 0);
				//d_fadeOutPosition = thisGrain->iLoopEnd_old - actualFadeSteps * thisGrain->samplePitchRatio; //start here with fade out
				d_fadeOutPosition = thisGrain->iLoopEnd_old - actualFadeSteps; //start here with fade out
				if (d_fadeOutPosition < 0) d_fadeOutPosition = 0;
				//i_fadeInPosition = i_loopStart + actualFadeSteps * thisGrain->samplePitchRatio; //do fade in until here
				if (!thisGrain->bwasLooped)
					//d_fadeInPosition = thisGrain->origSampleStartPosition + actualFadeSteps * thisGrain->samplePitchRatio; //do fade in until here
					d_fadeInPosition = thisGrain->origSampleStartPosition + actualFadeSteps; //do fade in until here
				else
					//d_fadeInPosition = i_loopStart + actualFadeSteps * thisGrain->samplePitchRatio; //do fade in until here
					d_fadeInPosition = i_loopStart + actualFadeSteps; //do fade in until here

				if (d_fadeInPosition > soundLength - 1) d_fadeInPosition = soundLength - 1;

				if (d_fadeOutPosition < d_fadeInPosition) //CHECK
					d_fadeOutPosition = d_fadeInPosition;
				vassert(d_fadeOutPosition >= d_fadeInPosition);

				if (bSwitchSamplerLoop || bSwitchSamplerLoopOnly || bSwitchSamplerLoopPingpong) { //handle only and pingpong as loop
					if (thisGrain->sourceSamplePosition > thisGrain->iLoopEnd_old) {
						if ((bSwitchSamplerLoopOnly || bSwitchSamplerLoopPingpong) && !bSwitchSamplerLoop) { //handle only and pingpong as loop
							if (!bSwitchSamplerLoopPingpong)								
								thisGrain->sampler_isInRelease = true; 
							else
								if (thisGrain->bLoopPingpong)
									thisGrain->sampler_isInRelease = true;
						}

						thisGrain->bwasLooped = true;
						//if ((bSwitchSamplerLoop || bSwitchSamplerLoopOnly) && (bSwitchSamplerLoopPingpong))
						if (bSwitchSamplerLoopPingpong)
							thisGrain->bLoopPingpong = !thisGrain->bLoopPingpong;

						if (!thisGrain->sampler_isInRelease) { //do loop
							thisGrain->sourceSamplePosition = i_loopStart + (thisGrain->sourceSamplePosition - int(thisGrain->sourceSamplePosition)); //keep frac part

							//determine new loopend					
							float range = range = m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fSamplerLoopRange, MODMATDEST::SamplerLoopRange, &inputState) * 0.01f;
							if (!samplerSound->hasLoop())
								thisGrain->iLoopEnd_old = ((soundLength)-1) * range;
							else
								thisGrain->iLoopEnd_old = i_loopStart + (samplerSound->getLoopEnd() - i_loopStart) * range;
						}
					}
				}

				if (thisGrain->sourceSamplePosition > soundLength - 1)
				{
					if (thisGrain->sampler_isInRelease) {
						thisGrain->sampler_attackReleaseLevel = 0.f;
						l_break = true; //buffer is 0.0
					}
					else { //do loop
						if (bSwitchSamplerLoop || bSwitchSamplerLoopOnly || bSwitchSamplerLoopPingpong) { //handle only and pingpong as loop { //needed? sames as above?
							thisGrain->bwasLooped = true;
							thisGrain->sourceSamplePosition = thisGrain->sourceSamplePosition - int(thisGrain->sourceSamplePosition); //keep frac part
							if (bSwitchSamplerLoopPingpong)
								thisGrain->bLoopPingpong = !thisGrain->bLoopPingpong;

							//determine new loopend					
							float range = range = m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fSamplerLoopRange, MODMATDEST::SamplerLoopRange, &inputState) * 0.01f;
							if (!samplerSound->hasLoop())
								thisGrain->iLoopEnd_old = ((soundLength)-1) * range;
							else
								thisGrain->iLoopEnd_old = i_loopStart + (samplerSound->getLoopEnd() - i_loopStart) * range;
						}
						else {
							thisGrain->sampler_attackReleaseLevel = 0.f;
							thisGrain->sampler_isInRelease = true;
							l_break = true; //buffer is 0.0
						}
					}
				}

				float l = 0.f;
				float r = 0.f;
				if (!l_break) {
					thisGrain->realPos = thisGrain->sourceSamplePosition;
					//if ((bSwitchSamplerLoop || bSwitchSamplerLoopOnly) && (bSwitchSamplerLoopPingpong)) {
					if (bSwitchSamplerLoopPingpong) {
						if (thisGrain->bLoopPingpong)
							thisGrain->realPos = i_loopStart + thisGrain->iLoopEnd_old - thisGrain->realPos;
					}

					auto pos = (int)thisGrain->realPos;
					auto alpha = (float)(thisGrain->realPos - pos);
					auto invAlpha = 1.0f - alpha;

					// just using a very simple linear interpolation here..
					if (pos < soundLength - 1) {
						l = inL[pos] * invAlpha + inL[pos + 1] * alpha;
						r = (inR != nullptr) ? (inR[pos] * invAlpha + inR[pos + 1] * alpha) : l;
					}
					else {
						l = inL[soundLength - 1];
						r = (inR != nullptr) ? inR[soundLength - 1] : l;
					}

					//attack release stuff for fade in out
					l *= thisGrain->sampler_lgain;
					r *= thisGrain->sampler_rgain;

					if (thisGrain->sampler_isInAttack)
					{
						l *= thisGrain->sampler_attackReleaseLevel;
						r *= thisGrain->sampler_attackReleaseLevel;

						thisGrain->sampler_attackReleaseLevel += thisGrain->sampler_attackDelta;

						if (thisGrain->sampler_attackReleaseLevel >= 1.0f)
						{
							thisGrain->sampler_attackReleaseLevel = 1.0f;
							thisGrain->sampler_isInAttack = false;
						}
					}
					else if (thisGrain->sampler_isInRelease)
					{
						l *= thisGrain->sampler_attackReleaseLevel;
						r *= thisGrain->sampler_attackReleaseLevel;

						thisGrain->sampler_attackReleaseLevel += thisGrain->sampler_releaseDelta;

						if (thisGrain->sampler_attackReleaseLevel <= 0.0f)
						{
							l_break = true;
						}
					}
				}

				if (!l_break) {
					if (outR != nullptr)
					{
						lOutL += l;
						lOutR += r;
					}
					else
					{
						lOutL += (l + r) * 0.5f;
					}
					//jassert((lOutL >= -6.f) && (lOutL <= 6.f));
					//jassert((lOutR >= -6.f) && (lOutR <= 6.f));

					//do loop fade in out
					//if (bSwitchSamplerLoop || bSwitchSamplerLoopOnly || bSwitchSamplerLoopPingpong) {
						//if ((thisGrain->sourceSamplePosition <= thisGrain->iLoopEnd_old) && (thisGrain->sourceSamplePosition > d_fadeOutPosition)) {
					if (thisGrain->sourceSamplePosition > d_fadeOutPosition) {
						//float ffactor = (actualFadeSteps - (thisGrain->sourceSamplePosition - d_fadeOutPosition) / thisGrain->samplePitchRatio) / actualFadeSteps;
						thisGrain->sampler_fadeFactor = (actualFadeSteps - (thisGrain->sourceSamplePosition - d_fadeOutPosition)) / actualFadeSteps;
						if (thisGrain->sampler_fadeFactor < 0.f) thisGrain->sampler_fadeFactor = 0.f;
						if (thisGrain->sampler_fadeFactor > 1.f) thisGrain->sampler_fadeFactor = 1.f;

						lOutL *= thisGrain->sampler_fadeFactor;
						if (outR != nullptr) lOutR *= thisGrain->sampler_fadeFactor;
					}
					else if (thisGrain->sourceSamplePosition < d_fadeInPosition) {
						//if (thisGrain->bwasLooped) { //skip first entry
							//float ffactor = (actualFadeSteps + (thisGrain->sourceSamplePosition - d_fadeInPosition) / thisGrain->samplePitchRatio) / actualFadeSteps;
						thisGrain->sampler_fadeFactor = (actualFadeSteps + (thisGrain->sourceSamplePosition - d_fadeInPosition)) / actualFadeSteps;
						if (thisGrain->sampler_fadeFactor < 0.f) thisGrain->sampler_fadeFactor = 0.f;
						if (thisGrain->sampler_fadeFactor > 1.f) thisGrain->sampler_fadeFactor = 1.f;

						lOutL *= thisGrain->sampler_fadeFactor;
						if (outR != nullptr) lOutR *= thisGrain->sampler_fadeFactor;
						//}
					}
					//}
				}

				if (grain == 0) {
					samplerUpdatePitch(samplerSound, false); //only once!
				}

				lValGrains += lOutL;
				rValGrains += lOutR;
				glSumAttackRelease += (thisGrain->sampler_attackReleaseLevel * thisGrain->sampler_fadeFactor); //both relevant

				//new position
				thisGrain->sourceSamplePosition += m_samplePitchRatio + thisGrain->samplePositionRandomIncrement;				


/*
#ifdef _DEBUG
				if (thisGrain->sampler_attackReleaseLevel <= 0.f) {
					vassert(thisGrain->sourceSamplePosition > d_fadeOutPosition);
					float ffactor = (actualFadeSteps - (thisGrain->sourceSamplePosition - d_fadeOutPosition)) / actualFadeSteps;
					vassert(ffactor <= 0.f);
				}
#endif
*/
			} //grains

			if (glSumAttackRelease <= 1.f)
				gain = 1.f;
			else
				gain = sqrtf(1.f / glSumAttackRelease); //equal power gain

			outL[counter] += lValGrains * gain; //add all grains
			outR[counter] += rValGrains * gain;
			
		} //samples
		samplerSound->notifyPositionChanged();

		m_grainTable.erase(std::remove_if(
			m_grainTable.begin(), m_grainTable.end(),
			[](decltype(m_grainTable)::value_type const& elem) {
			return elem.sampler_attackReleaseLevel <= 0.f;
		}), m_grainTable.end());
	} 
}
//==============================================================================

void CVASTSingleNote::samplerUpdatePitch(VASTSamplerSound* sound, bool force) {

	float lPitchMod = m_fPitchModSampler_smoothed.getNextValue(); //-1 .. 1
	float lSamplerCents = m_fSamplerCents_smoothed.getNextValue(); //-1200 .. 1200

	if (*m_Set->m_State->m_bSamplerKeytrack == static_cast<int>(SWITCH::SWITCH_ON)) {
		float baseFreq = 0.f;
		if ((*m_Set->m_State->m_fPortamento == 0.f) || force) { //check unsafe comparison
			baseFreq = (m_samplerMidiNoteNumber - sound->getMidiRootNote()) / 12.0;

			if (m_fSamplerBaseFreqPortamento_smoothed.isSmoothing())
				m_fSamplerBaseFreqPortamento_smoothed.setTargetValue(baseFreq);
			else
				m_fSamplerBaseFreqPortamento_smoothed.setCurrentAndTargetValue(baseFreq);
		}
		if (*m_Set->m_State->m_fPortamento > 0.f) {
			baseFreq = m_fSamplerBaseFreqPortamento_smoothed.getNextValue();
		}
		m_samplePitchRatio = std::pow(2.0, baseFreq + lSamplerCents / 1200.0f)

			* (*m_Set->m_State->m_fMasterTune / 440.f) 
			* m_Set->m_fPitchBend * m_pitchBendNote * (lPitchMod + 1) // 0..2 center 1	
			* sound->getSourceSampleRate() / getSampleRate();
	}
	else {
		m_samplePitchRatio = std::pow(2.0, (60 - sound->getMidiRootNote()) / 12.0 + lSamplerCents / 1200.0f)  * m_Set->m_fPitchBend * m_pitchBendNote * (lPitchMod + 1) // 0..2 center 1	//60 is c3
			* sound->getSourceSampleRate() / getSampleRate();
	}
}

int CVASTSingleNote::getVoiceNo() const {
	return mVoiceNo;
}

void CVASTSingleNote::updateDetune(int bank, float detuneValue, bool updateFrequency) {
	int numOscs = 0;
	if (bank == 0)
		numOscs = *m_Set->m_State->m_iNumOscs_OscA;
	else if (bank == 1)
		numOscs = *m_Set->m_State->m_iNumOscs_OscB;
	else if (bank == 2)
		numOscs = *m_Set->m_State->m_iNumOscs_OscC;
	else if (bank == 3)
		numOscs = *m_Set->m_State->m_iNumOscs_OscD;

	float sum = 0.f;
	for (int i = 0; i < numOscs; i++) {
		delta[bank][i] = (normalized_irrational[bank][i + 1] - normalized_irrational[bank][i]) * detuneValue * 0.6945f; //100 is one octave
		detune[bank][i] = fastPrecisePow(M_TWELFTH_ROOT_TWO, delta[bank][i]);
		sum += detune[bank][i];
	}
	sum /= numOscs;
	sum -= 1.0f;

	for (int i = 0; i < numOscs; i++) {
		detune[bank][i] -= sum;
	}

	float lDetune = 0.f;
	for (int i = 0; i < C_MAX_PARALLEL_OSC; i++) {
		if (i < numOscs)
			lDetune = detune[bank][i];
		else
			lDetune = 1.0f;

		switch (bank) {
		case 0:
			if (*m_Set->m_State->m_iNumOscs_OscA == 1) lDetune = 1.0f; // do not detune single osc
			break;
		case 1:
			if (*m_Set->m_State->m_iNumOscs_OscB == 1) lDetune = 1.0f; // do not detune single osc
			break;
		case 2:
			if (*m_Set->m_State->m_iNumOscs_OscC == 1) lDetune = 1.0f; // do not detune single osc
			break;
		case 3:
			if (*m_Set->m_State->m_iNumOscs_OscD == 1) lDetune = 1.0f; // do not detune single osc
			break;
		}

		m_Oscillator[bank].setDetune(i, lDetune, updateFrequency); 
	}
}

void CVASTSingleNote::updateVariables() { //called once on prepareForPlay and on parameter change
	MYUINT lNumOscsNeeded =
		*m_Set->m_State->m_iNumOscs_OscA +
		*m_Set->m_State->m_iNumOscs_OscB +
		*m_Set->m_State->m_iNumOscs_OscC +
		*m_Set->m_State->m_iNumOscs_OscD +
		*m_Set->m_State->m_bNoiseOnOff;

	//MYUINT lMaxOsc = fmax(fmax(fmax(*m_Set->m_State->m_iNumOscs_OscA, *m_Set->m_State->m_iNumOscs_OscB), fmax(*m_Set->m_State->m_iNumOscs_OscC, *m_Set->m_State->m_iNumOscs_OscD)), *m_Set->m_State->m_bNoiseOnOff); //fmax as max not on Mac OS

	m_iNumParallelOsc = (lNumOscsNeeded > C_MAX_PARALLEL_OSC) ? C_MAX_PARALLEL_OSC : lNumOscsNeeded; ///CHECK!!!
	//m_fRetrigAttenuateFactor = (1.0 + m_Set->m_State->fastPrecisePow(m_iNumParallelOsc, 1.4) * 0.033);

	for (int bank = 0; bank < 4; bank++) {
		int lNumber = 0;
		if (bank == 0) {
			lNumber = *m_Set->m_State->m_iNumOscs_OscA;
			m_Oscillator[bank].updateMainVariables(m_Set->m_nSampleRate, WAVE::wavetable, lNumber, *m_Set->m_State->m_fMasterTune, *m_Set->m_State->m_iOscOct_OscA, *m_Set->m_State->m_fOscCents_OscA);
		}
		else if (bank == 1) {
			lNumber = *m_Set->m_State->m_iNumOscs_OscB;
			m_Oscillator[bank].updateMainVariables(m_Set->m_nSampleRate, WAVE::wavetable, lNumber, *m_Set->m_State->m_fMasterTune, *m_Set->m_State->m_iOscOct_OscB, *m_Set->m_State->m_fOscCents_OscB);
		}
		else if (bank == 2) {
			lNumber = *m_Set->m_State->m_iNumOscs_OscC;
			m_Oscillator[bank].updateMainVariables(m_Set->m_nSampleRate, WAVE::wavetable, lNumber, *m_Set->m_State->m_fMasterTune, *m_Set->m_State->m_iOscOct_OscC, *m_Set->m_State->m_fOscCents_OscC);
		}
		else if (bank == 3) {
			lNumber = *m_Set->m_State->m_iNumOscs_OscD;
			m_Oscillator[bank].updateMainVariables(m_Set->m_nSampleRate, WAVE::wavetable, lNumber, *m_Set->m_State->m_fMasterTune, *m_Set->m_State->m_iOscOct_OscD, *m_Set->m_State->m_fOscCents_OscD);
		}
	}

	//Detune A
	if (*m_Set->m_State->m_bOscOnOff_OscA == static_cast<int>(SWITCH::SWITCH_ON)) {
		generate_normalized_irrationals(normalized_irrational[0], *m_Set->m_State->m_iNumOscs_OscA + 1);
		updateDetune(0, *m_Set->m_State->m_fOscDetune_OscA, true); //call setFrequency	
	}

	//Detune B
	if (*m_Set->m_State->m_bOscOnOff_OscB == static_cast<int>(SWITCH::SWITCH_ON)) {
		generate_normalized_irrationals(normalized_irrational[1], *m_Set->m_State->m_iNumOscs_OscB + 1);
		updateDetune(1, *m_Set->m_State->m_fOscDetune_OscB, true); //call setFrequency
	}

	//Detune C	
	if (*m_Set->m_State->m_bOscOnOff_OscC == static_cast<int>(SWITCH::SWITCH_ON)) {
		generate_normalized_irrationals(normalized_irrational[2], *m_Set->m_State->m_iNumOscs_OscC + 1);
		updateDetune(2, *m_Set->m_State->m_fOscDetune_OscC, true); //call setFrequency
	}

	//Detune D
	if (*m_Set->m_State->m_bOscOnOff_OscD == static_cast<int>(SWITCH::SWITCH_ON)) {
		generate_normalized_irrationals(normalized_irrational[3], *m_Set->m_State->m_iNumOscs_OscD + 1);
		updateDetune(3, *m_Set->m_State->m_fOscDetune_OscD, true); //call setFrequency
	}
	
	if (*m_Set->m_State->m_bNoiseOnOff == static_cast<int>(SWITCH::SWITCH_ON)) {
		m_OscillatorNoise.updateMainVariables(m_Set->m_nSampleRate, WAVE::noise, 1, *m_Set->m_State->m_fMasterTune, 0, 0);
	}

	m_VCA.updateVariables();
}

void CVASTSingleNote::setPortamentoTime(float time) {
	for (int bank = 0; bank < 4; bank++)
		m_Oscillator[bank].setPortamentoTime(time);

	m_fSamplerBaseFreqPortamento_smoothed.reset(m_Set->m_nSampleRate, time * 0.001f);
}

void CVASTSingleNote::setGlissandoStart(int midinote, bool reset) {
	for (int bank = 0; bank < 4; bank++)
		m_Oscillator[bank].setPortamentoStart(midinote, reset);
	
	if (m_Poly->getSamplerSound() != nullptr) { //live data
		float startFreq = (midinote - m_Poly->getSamplerSound()->getMidiRootNote()) / 12.0;				
		if (m_fSamplerBaseFreqPortamento_smoothed.isSmoothing()) {
			float currentFreq = m_fSamplerBaseFreqPortamento_smoothed.getNextValue();
			startFreq = currentFreq;
		}
		float keepTarget = m_fSamplerBaseFreqPortamento_smoothed.getTargetValue();

		if (!reset) {
			m_fSamplerBaseFreqPortamento_smoothed.setCurrentAndTargetValue(startFreq); //force take it!
			m_fSamplerBaseFreqPortamento_smoothed.setTargetValue(keepTarget); //old target freq
		}
		else {
			m_fSamplerBaseFreqPortamento_smoothed.setCurrentAndTargetValue(keepTarget); //force take it!
		}
	}
}

void CVASTSingleNote::generate_normalized_irrationals(float *destination, int count)
{
	// can be used to locate an "island" of consecutive integers without a perfect square.
	// not really beneficial though.
	float square = 0;//find_square_gap(count);
	float offset = 1;//square*square;

	for (int i = 0, skips = 0; i - skips < count; i++) {
		float v = sqrtf(i + offset + 1) - square;

		// was perfect square?
		if (floorf(v) == v) {
			skips++;
			continue;
		}

		destination[i - skips] = v;
	}

	float v = 1.0f / destination[count - 1];

	for (int i = 0; i < count; i++) {
		destination[i] *= v;
	}
}

bool CVASTSingleNote::isPlayingCalledFromUI() const {
	if (isVoiceActive())
		return true;
	return false;
}

bool CVASTSingleNote::isPlayingInRange(int startsample, int numsamples) const {
	//approximation range is true if start or end is used - in between not checken
	if (m_Set->m_RoutingBuffers.MSEGActiveBuffer[0][mVoiceNo][startsample] || m_Set->m_RoutingBuffers.MSEGActiveBuffer[0][mVoiceNo][startsample + numsamples - 1])
		return true;
	if (m_Set->m_RoutingBuffers.MSEGActiveBuffer[1][mVoiceNo][startsample] || m_Set->m_RoutingBuffers.MSEGActiveBuffer[1][mVoiceNo][startsample + numsamples - 1])
		return true;
	if (m_Set->m_RoutingBuffers.MSEGActiveBuffer[2][mVoiceNo][startsample] || m_Set->m_RoutingBuffers.MSEGActiveBuffer[2][mVoiceNo][startsample + numsamples - 1])
		return true;
	if (m_Set->m_RoutingBuffers.MSEGActiveBuffer[3][mVoiceNo][startsample] || m_Set->m_RoutingBuffers.MSEGActiveBuffer[3][mVoiceNo][startsample + numsamples - 1])
		return true;
	if (m_Set->m_RoutingBuffers.MSEGActiveBuffer[4][mVoiceNo][startsample] || m_Set->m_RoutingBuffers.MSEGActiveBuffer[4][mVoiceNo][startsample + numsamples - 1])
		return true;
	return false;
}

bool CVASTSingleNote::isPlayingAtSamplePosition(int sample) const {
	if (m_Set->m_RoutingBuffers.MSEGActiveBuffer[0][mVoiceNo][sample])
		return true;
	if (m_Set->m_RoutingBuffers.MSEGActiveBuffer[1][mVoiceNo][sample])
		return true;
	if (m_Set->m_RoutingBuffers.MSEGActiveBuffer[2][mVoiceNo][sample])
		return true;
	if (m_Set->m_RoutingBuffers.MSEGActiveBuffer[3][mVoiceNo][sample])
		return true;
	if (m_Set->m_RoutingBuffers.MSEGActiveBuffer[4][mVoiceNo][sample])
		return true;
	return false;	
}

MYUINT CVASTSingleNote::getChannel() const {
	return m_uChannel;
}

MYUINT CVASTSingleNote::getMIDINote() const {
	return m_uMIDINote;
}

bool CVASTSingleNote::noteOn(MYUINT uChannel, MYUINT uMIDINote, MYUINT uVelocity, bool legato) {
	bool hardStop = m_VCA.isHardStop();
	bool noteOff = m_VCA.isNoteOff();
	if (hardStop && !noteOff) { //wait for hardstop tailoff
		VDBG("voice " << getVoiceNo() << " noteOn but m_VCA.isHardStop() and running!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
		if (m_Set->m_uMaxPoly == 1)
			legato = false;
			//return false;
	}
#ifdef _DEBUG
	if (hardStop)
		VDBG("voice " << getVoiceNo() << " m_VCA.isHardStop() and new noteOn");
#endif

	m_uChannelNext = uChannel;
	m_uMIDINoteNext = uMIDINote;
	m_uVelocityNext = uVelocity;

	nextNote(legato);
	return true;
}

void CVASTSingleNote::nextNote(bool legato) {
	m_Set->sendStatusMessage("------CVASTSingleNote::nextNote Voice#, note#", mVoiceNo, m_uMIDINoteNext);
	m_uChannel = m_uChannelNext;
	m_uMIDINote = m_uMIDINoteNext;
	m_uVelocity = m_uVelocityNext;

	struct timeval tp;
	m_Set->_gettimeofday(&tp);
	m_startPlayTimestamp = static_cast<ULong64_t>(tp.tv_sec) * 1000 + tp.tv_usec / 1000;

	m_Set->iAftertouch[getVoiceNo()] = -1;
	m_Set->iAftertouchMidiNote[getVoiceNo()] = -1;

	if (legato) {
		for (int bank = 0; bank < 4; bank++)
			m_Oscillator[bank].noteOn(m_uChannel, m_uMIDINote, m_uVelocity);
				
		m_OscillatorNoise.noteOn(m_uChannel, m_uMIDINote, m_uVelocity);
		m_VCA.noteOn(m_startPlayTimestamp, true); //but in sustain phase!!		
	}
	else {
		resetSmoothers();
		m_VCF[0]->resetSmoothers();
		m_VCF[1]->resetSmoothers();
		m_VCF[2]->resetSmoothers();
         
		for (int bank = 0; bank < 4; bank++) {
			//New start all oscs!
			
			m_Oscillator[bank].noteOn(m_uChannel, m_uMIDINote, m_uVelocity);
			for (int i = 0; i < C_MAX_PARALLEL_OSC; i++) {
				//VDBG("resynch voice: "+ String(mVoiceNo));
				m_Oscillator[bank].resynch(i, false); //re-synch - immendiately
													  //VDBG("resynch");
			}

			//CHECK
			///m_Poly->m_OscBank[bank].setChangedFlagOsc(); //redraw on new note??

			m_iCurCycleSamples[bank] = 0;
			m_iLastCycleSamples[bank] = 0;
		}
		m_OscillatorNoise.noteOn(m_uChannel, m_uMIDINote, m_uVelocity);
		m_OscillatorNoise.resynch(0, false); //re-synch - immediately

		m_fOscADivisor = *m_Set->m_State->m_iNumOscs_OscA;
		m_fOscAMaxPeak = 1.0;
		m_fOscBDivisor = *m_Set->m_State->m_iNumOscs_OscB;
		m_fOscBMaxPeak = 1.0;
		m_fOscCDivisor = *m_Set->m_State->m_iNumOscs_OscC;
		m_fOscCMaxPeak = 1.0;
		m_fOscDDivisor = *m_Set->m_State->m_iNumOscs_OscD;
		m_fOscDMaxPeak = 1.0;
		m_fNoiseDivisor = *m_Set->m_State->m_bNoiseOnOff; //bool but ok!
		m_fNoiseMaxPeak = 1.0;
		m_VCA.noteOn(m_startPlayTimestamp, false);
	}
}

void CVASTSingleNote::noteOff(float releaseVelocity) {
	//m_Set->m_State->sendStatusMessage("------CVASTSingleNote::noteOff voice#, osci#", mVoiceNo, i);
	m_VCA.noteOff(releaseVelocity);
}

void CVASTSingleNote::syncOscToMaster(int bank, int i) { //to align phases
	m_Oscillator[bank].syncPhasor(i);
}

void CVASTSingleNote::setTargetWTPos(int bank, float targetWTPosPercentage, bool takeNext) {

	jassert((targetWTPosPercentage >= 0.f) && (targetWTPosPercentage <= 1.f));
    if (takeNext)
        m_fOscWTPos_smoothed[bank].setCurrentAndTargetValue(targetWTPosPercentage); //0 to 1
    else
        m_fOscWTPos_smoothed[bank].setTargetValue(targetWTPosPercentage); //0 to 1
}

void CVASTSingleNote::initWavetableProcessing(int bank, sRoutingBuffers& routingBuffers, modMatrixInputState& inputState) {
	float lPhaseOffset = 0.0f;
	float lMorph = 0.0f;
	if (bank == 0) {
		lPhaseOffset = m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fOscPhase_OscA, MODMATDEST::OscAPhase, &inputState);
		lMorph = m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fOscMorph_OscA, MODMATDEST::OscAMorph, &inputState);
		setWTPosSmooth(bank, lMorph);
	}
	else if (bank == 1) {
		lPhaseOffset = m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fOscPhase_OscB, MODMATDEST::OscBPhase, &inputState);
		lMorph = m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fOscMorph_OscB, MODMATDEST::OscBMorph, &inputState);		
		setWTPosSmooth(bank, lMorph);
	}
	else if (bank  == 2) {
		lPhaseOffset = m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fOscPhase_OscC, MODMATDEST::OscCPhase, &inputState);
		lMorph = m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fOscMorph_OscC, MODMATDEST::OscCMorph, &inputState);
		setWTPosSmooth(bank, lMorph);
	}
	else if (bank == 3) {
		lPhaseOffset = m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fOscPhase_OscD, MODMATDEST::OscDPhase, &inputState);
		lMorph = m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fOscMorph_OscD, MODMATDEST::OscDMorph, &inputState);
		setWTPosSmooth(bank, lMorph);
	}
    if (m_bSmoothersTakeNextValue)
        m_fPhaseOffset_smoothed[bank].setCurrentAndTargetValue(lPhaseOffset);  //to singlenote
    else
        m_fPhaseOffset_smoothed[bank].setTargetValue(lPhaseOffset);  //to singlenote
}

void CVASTSingleNote::doWavetableStep(const int bank, const int currentFrame, const int firstFrame) {
	//CHECK move to wavetable
	//m_currentWTPosFloatPercentage[bank] = m_fOscWTPos_smoothed[bank].getNextValue(); // public due to oscilloscope
	
	//if (m_currentWTPosFloatPercentage[bank] < 0.f) m_currentWTPosFloatPercentage[bank] = 0.f; //safety - unclear where it comes from // TODO
	//if (m_currentWTPosFloatPercentage[bank] > 1.f) m_currentWTPosFloatPercentage[bank] = 1.f; //safety - unclear where it comes from // TODO

	//float curWtPosFloat = m_currentWTPosFloatPercentage[bank] * (m_localVoiceBankWavetable[bank]->getNumPositions() - 1);
	//if (m_Poly->m_OscBank[bank].getSoloMode()) {
		//curWtPosFloat = m_localVoiceBankWavetable[bank]->m_iBeginSelectionPosition + m_currentWTPosFloatPercentage[bank] * (m_localVoiceBankWavetable[bank]->m_iEndSelectionPosition - m_localVoiceBankWavetable[bank]->m_iBeginSelectionPosition);
	//}

	/*
	//CHECK right here?
	float lPhaseOffsetNext = m_fPhaseOffset_smoothed[bank].getNextValue() * 0.01f; //to singlenote
	if (m_safePhaseFloat[bank] != lPhaseOffsetNext) {
		m_safePhaseFloat[bank] = lPhaseOffsetNext;
		m_Poly->m_OscBank[bank].setChangedFlagOsc();
	}
	*/

	/*
	jassert(curWtPosFloat >= 0.f);	
	if (firstFrame) {
		m_iWTPosInt[bank] = int(curWtPosFloat);
	}

	if (m_safeWtPosFloat[bank] != curWtPosFloat) {
		m_bWTPos_allSameValues[bank] = false;
		if (m_iWTPosInt[bank] != int(curWtPosFloat))
			m_bWTPos_allSameInt[bank] = false;
		
		m_safeWtPosFloat[bank] = curWtPosFloat;

		//CHECK TODO
		m_Poly->m_OscBank[bank].setChangedFlagOsc();
		//setChangedFlag(bank); //for oscilloscope only
	}

	if (m_safePhaseFloat[bank] != lPhaseOffsetNext) {
		m_safePhaseFloat[bank] = lPhaseOffsetNext;
		
		m_Poly->m_OscBank[bank].setChangedFlagOsc();
	}

	m_wtPosBufferPointer[bank][currentFrame] = curWtPosFloat;  //to singlenote
	//m_morphBuffer[bank]->setSample(0, currentFrame, lmorph); //to singlenote

	//CVASTWaveTable* l_wavetableSoftFade = m_Poly->m_OscBank[bank].getSoftFadeWavetablePointer();
	if (m_localVoiceBankWavetableSoftfade[bank] != nullptr) {
		float curWtPosFloatNext = m_currentWTPosFloatPercentage[bank] * (m_localVoiceBankWavetableSoftfade[bank]->getNumPositions() - 1); //positions prepared?
		if (m_Poly->m_OscBank[bank].getSoloMode()) {
			curWtPosFloatNext = m_localVoiceBankWavetableSoftfade[bank]->m_iBeginSelectionPosition + m_currentWTPosFloatPercentage[bank] * (m_localVoiceBankWavetableSoftfade[bank]->m_iEndSelectionPosition - m_localVoiceBankWavetableSoftfade[bank]->m_iBeginSelectionPosition);
		}
		//m_wtPosBufferNext[bank]->setSample(0, currentFrame, curWtPosFloatNext);
		m_wtPosBufferNextPointer[bank][currentFrame] = curWtPosFloatNext;


		//TODO perfopt here as well? //there is an wt fx! -> perfopt, TODO check if it can be skipped always
	}
	*/
}

void CVASTSingleNote::resetSoftFadeState() { //when nothing is playing anymore	
	//must only set note stuff - not m_Poly->m_OscBank here!!! Some voices are always resetted
	for (int bank = 0; bank < 4; bank++) {
		m_localVoiceBankWavetable[bank] = nullptr;
		m_localVoiceBankWavetableSoftfade[bank] = nullptr;
		m_bSoftFadeCycleStarted[bank] = false;
		m_bSoftFadeCycleEnded[bank] = false;		
		m_bIsStartOfCycle[bank] = false;
		m_bIsFirstCycle[bank] = false;
		m_iCurCycleSamples[bank] = 0;
		m_iLastCycleSamples[bank] = 0;
#ifdef _DEBUG
		m_bLastRenderedWTID[bank] = -1;
		m_fLastRenderedSample[bank] = 0.f;
#endif
	}
}

bool CVASTSingleNote::prepareFrequency(int bank, int skips, int startSample, bool bTakeNextValue, bool bIsStartOfCycle) {
	modMatrixInputState l_inputState{ mVoiceNo, startSample };
	float fDetuneMod = 1.0;
	bool bHasToBeDoneForEachSample = false;
	bool isModulating = false;
	
	float fPitchMod = 0.f; //-1 .. 1 center 0	
	if (m_Set->modMatrixDestinationSetFast(MODMATDEST::Pitch))
		fPitchMod = m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fPitchMod, MODMATDEST::Pitch, &l_inputState, &isModulating) * 0.01f;
	if (!isModulating)
		fPitchMod = 0.f;
    bTakeNextValue ? m_fPitchMod_smoothed.setCurrentAndTargetValue(fPitchMod) : m_fPitchMod_smoothed.setTargetValue(fPitchMod);
	bHasToBeDoneForEachSample = isModulating;

	switch (bank) {
	case 0: {
		//pitch
		float l_OscACents = 0.f;
		if (bIsStartOfCycle || m_Set->modMatrixDestinationSetFast(MODMATDEST::OscACents)) {
			l_OscACents = m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fOscCents_OscA, MODMATDEST::OscACents, &l_inputState, &bHasToBeDoneForEachSample);
            bTakeNextValue ? m_fOscACents_smoothed.setCurrentAndTargetValue(l_OscACents) :
                m_fOscACents_smoothed.setTargetValue(l_OscACents);
		}
		l_OscACents = m_fOscACents_smoothed.getNextValue();
		m_fOscACents_smoothed.skip(skips - 1);
		m_Oscillator[bank].setCents(l_OscACents);
		//detune
		if (bIsStartOfCycle || m_Set->modMatrixDestinationSetFast(MODMATDEST::OscADetune)) {
			fDetuneMod = m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fOscDetune_OscA, MODMATDEST::OscADetune, &l_inputState, &bHasToBeDoneForEachSample);
			updateDetune(bank, fDetuneMod, false); //dont call setFrequency, should not be called for each sample, expensive
		}
	}
		break;
	case 1: {
		//pitch
		float l_OscBCents = 0.f;
		if (bIsStartOfCycle || m_Set->modMatrixDestinationSetFast(MODMATDEST::OscBCents)) {
			l_OscBCents = m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fOscCents_OscB, MODMATDEST::OscBCents, &l_inputState, &bHasToBeDoneForEachSample);
            bTakeNextValue ? m_fOscBCents_smoothed.setCurrentAndTargetValue(l_OscBCents) :
                m_fOscBCents_smoothed.setTargetValue(l_OscBCents);
		}
		l_OscBCents = m_fOscBCents_smoothed.getNextValue();
		m_fOscBCents_smoothed.skip(skips - 1);
		m_Oscillator[bank].setCents(l_OscBCents);
		//detune
		if (bIsStartOfCycle || m_Set->modMatrixDestinationSetFast(MODMATDEST::OscBDetune)) {
			fDetuneMod = m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fOscDetune_OscB, MODMATDEST::OscBDetune, &l_inputState, &bHasToBeDoneForEachSample);
			updateDetune(bank, fDetuneMod, false); //dont call setFrequency, should not be called for each sample, expensive
		}
	}
		break;
	case 2: {
		//pitch
		float l_OscCCents = 0.f;
		if (bIsStartOfCycle || m_Set->modMatrixDestinationSetFast(MODMATDEST::OscCCents)) {
			l_OscCCents = m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fOscCents_OscC, MODMATDEST::OscCCents, &l_inputState, &bHasToBeDoneForEachSample);
            bTakeNextValue ? m_fOscCCents_smoothed.setCurrentAndTargetValue(l_OscCCents) :
                m_fOscCCents_smoothed.setTargetValue(l_OscCCents);
		}
		l_OscCCents = m_fOscCCents_smoothed.getNextValue();
		m_fOscCCents_smoothed.skip(skips - 1);
		m_Oscillator[bank].setCents(l_OscCCents);
		//detune
		if (bIsStartOfCycle || m_Set->modMatrixDestinationSetFast(MODMATDEST::OscCDetune)) {
			fDetuneMod = m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fOscDetune_OscC, MODMATDEST::OscCDetune, &l_inputState, &bHasToBeDoneForEachSample);
			updateDetune(bank, fDetuneMod, false); //dont call setFrequency, should not be called for each sample, expensive		
		}
	}
		break;
	case 3: {
		//pitch
		float l_OscDCents = 0.f;
		if (bIsStartOfCycle || m_Set->modMatrixDestinationSetFast(MODMATDEST::OscDCents)) {
			l_OscDCents = m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fOscCents_OscD, MODMATDEST::OscDCents, &l_inputState, &bHasToBeDoneForEachSample);
            bTakeNextValue ? m_fOscDCents_smoothed.setCurrentAndTargetValue(l_OscDCents) :
                m_fOscDCents_smoothed.setTargetValue(l_OscDCents);
		}
		l_OscDCents = m_fOscDCents_smoothed.getNextValue();
		m_fOscDCents_smoothed.skip(skips - 1);
		m_Oscillator[bank].setCents(l_OscDCents);
		//detune
		if (bIsStartOfCycle || m_Set->modMatrixDestinationSetFast(MODMATDEST::OscDDetune)) {
			fDetuneMod = m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fOscDetune_OscD, MODMATDEST::OscDDetune, &l_inputState, &bHasToBeDoneForEachSample);
			updateDetune(bank, fDetuneMod, false); //dont call setFrequency, should not be called for each sample, expensive
		}
	}
		break;
	}

	// do pitch bender
	if (!m_Set->m_bPitchBendCenter)
		m_Oscillator[bank].setPitchBendZone(powf(2.f, (m_Set->m_iBendRange * (m_Set->m_fPitchBend - 1.f)) / 12.0));
	else
		m_Oscillator[bank].setPitchBendZone(1.f);

	if (fPitchMod != 0.0f) //check for accuracy?
		m_Oscillator[bank].updatePitchMod(m_fPitchMod_smoothed.getNextValue()); // -1 .. 1 center 0
	else
		m_Oscillator[bank].updatePitchMod(0.0f);
	m_fPitchMod_smoothed.skip(skips - 1);

	return bHasToBeDoneForEachSample;
}

bool CVASTSingleNote::prepareNextPhaseCycle(int bank, int skips, int startSample, bool bTakeNextValue, bool wtfxFXTypeChanged) {
	modMatrixInputState l_inputState{ mVoiceNo, startSample };
    
	float l_WTPosOscA = 0.f;
	float l_WTPosOscB = 0.f;
	float l_WTPosOscC = 0.f;
	float l_WTPosOscD = 0.f;
	
	bool bHasToBeDoneForEachSample = false;
	
	switch (bank) {
	case 0:
		l_WTPosOscA = m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fOscWTPos_OscA, MODMATDEST::OscAWTPos, &l_inputState);
		setTargetWTPos(bank, l_WTPosOscA * 0.01f, bTakeNextValue); //0 to 1
		break;
	case 1:
		l_WTPosOscB = m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fOscWTPos_OscB, MODMATDEST::OscBWTPos, &l_inputState);
		setTargetWTPos(bank, l_WTPosOscB * 0.01f, bTakeNextValue); //0 to 1
		break;
	case 2:
		l_WTPosOscC = m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fOscWTPos_OscC, MODMATDEST::OscCWTPos, &l_inputState);
		setTargetWTPos(bank, l_WTPosOscC * 0.01f, bTakeNextValue); //0 to 1
		break;
	case 3:
		l_WTPosOscD = m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fOscWTPos_OscD, MODMATDEST::OscDWTPos, &l_inputState);
		setTargetWTPos(bank, l_WTPosOscD * 0.01f, bTakeNextValue); //0 to 1
		break;
	}

	bHasToBeDoneForEachSample = prepareFrequency(bank, skips, startSample, bTakeNextValue, true); //check frequency skips when per sample!
	if (*m_Set->m_State->m_fPortamento > 0.0f)
		bHasToBeDoneForEachSample = true;
	
	m_currentWTPosFloatPercentage[bank] = jlimit<float>(0.f, 1.f, m_fOscWTPos_smoothed[bank].getNextValue()); // public due to oscilloscope
	vassert((m_currentWTPosFloatPercentage[bank] >= 0.f) && (m_currentWTPosFloatPercentage[bank] <= 1.f));
	m_wtFXVal[bank] = m_wtFXVal_smoothed[bank].getNextValue();

	if (!isPlayingAtSamplePosition(startSample)) {
		clearCurrentNote();
		resetSmoothers();
		m_VCF[0]->resetSmoothers();
		m_VCF[1]->resetSmoothers();
		m_VCF[2]->resetSmoothers();
	}

	if (!bHasToBeDoneForEachSample || wtfxFXTypeChanged) {
		for (int osci = 0; osci < m_Oscillator[bank].m_unisonOscis; osci++) {
			m_Oscillator[bank].setFrequency(osci, false);
		}
	}

	return bHasToBeDoneForEachSample;
}

bool CVASTSingleNote::prepareEachSample(int bank, int currentFrame, bool &freqsHaveToBeDoneForEachSample, bool bTakeNextValue, CVASTWaveTableOscillator  l_Oscillator[]) {
	modMatrixInputState l_inputState{ mVoiceNo, currentFrame };

	bool wrap = false;
	int skips = l_Oscillator[bank].m_unisonOscis * 10 - 9; //1 - 11 - 21 ...

	bool bUpdateNow = false;
	m_iLastFreqUpdate[bank]++;
	if (m_iLastFreqUpdate[bank] >= skips) {
		m_iLastFreqUpdate[bank] = 0;
		bUpdateNow = true;

        l_Oscillator[bank].doPortamentoIfRequired(skips);
	}
	
	if (bUpdateNow) {
		float l_WTPosOscA = 0.f;
		float l_WTPosOscB = 0.f;
		float l_WTPosOscC = 0.f;
		float l_WTPosOscD = 0.f;
		switch (bank) {
		case 0:
			l_WTPosOscA = m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fOscWTPos_OscA, MODMATDEST::OscAWTPos, &l_inputState);
			setTargetWTPos(bank, l_WTPosOscA * 0.01f, bTakeNextValue); //0 to 1
			break;
		case 1:
			l_WTPosOscB = m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fOscWTPos_OscB, MODMATDEST::OscBWTPos, &l_inputState);
			setTargetWTPos(bank, l_WTPosOscB * 0.01f, bTakeNextValue); //0 to 1
			break;
		case 2:
			l_WTPosOscC = m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fOscWTPos_OscC, MODMATDEST::OscCWTPos, &l_inputState);
			setTargetWTPos(bank, l_WTPosOscC * 0.01f, bTakeNextValue); //0 to 1
			break;
		case 3:
			l_WTPosOscD = m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fOscWTPos_OscD, MODMATDEST::OscDWTPos, &l_inputState);
			setTargetWTPos(bank, l_WTPosOscD * 0.01f, bTakeNextValue); //0 to 1
			break;
		}
		//m_fOscWTPos_smoothed[bank].skip(m_iLastCycleSamples[bank]);
		m_currentWTPosFloatPercentage[bank] = jlimit<float>(0.f, 1.f, m_fOscWTPos_smoothed[bank].getNextValue()); // public due to oscilloscope	
		vassert((m_currentWTPosFloatPercentage[bank] >= 0.f) && (m_currentWTPosFloatPercentage[bank] <= 1.f));
		m_wtFXVal[bank] = m_wtFXVal_smoothed[bank].getNextValue();
	}
	else {
		m_fOscWTPos_smoothed[bank].skip(1);
	}

	//---------------------------------------------------------------------------------
    //most expensive loop
	float l_phaseOffset = 0.f;
	if (bank == 0) {
		l_phaseOffset = m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fOscPhase_OscA, MODMATDEST::OscAPhase, &l_inputState);
	}
	else if (bank == 1) {
		l_phaseOffset = m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fOscPhase_OscB, MODMATDEST::OscBPhase, &l_inputState);
	}
	else if (bank == 2) {
		l_phaseOffset = m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fOscPhase_OscC, MODMATDEST::OscCPhase, &l_inputState);
	}
	else if (bank == 3) {
		l_phaseOffset = m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fOscPhase_OscD, MODMATDEST::OscDPhase, &l_inputState);
	}
    bTakeNextValue ? m_fPhaseOffset_smoothed[bank].setCurrentAndTargetValue(l_phaseOffset) :   //to singlenote
    m_fPhaseOffset_smoothed[bank].setTargetValue(l_phaseOffset);
	l_phaseOffset = m_fPhaseOffset_smoothed[bank].getNextValue() * 0.01f;
	
    bool FM_flag = (((bank == 0) && (*m_Set->m_State->m_uWTFX_OscA == C_WTFXTYPE_FM)) ||
        ((bank == 1) && (*m_Set->m_State->m_uWTFX_OscB == C_WTFXTYPE_FM)) ||
                    ((bank == 2) && (*m_Set->m_State->m_uWTFX_OscC == C_WTFXTYPE_FM)));
    if (!FM_flag) {
        l_Oscillator[bank].setFMFreq(0.f);
    }
    
    for (int osci = 0; osci < l_Oscillator[bank].m_unisonOscis; osci++) {
		//FM here
		if (FM_flag) { //FM on
			if (osci == 0) {
				l_inputState.currentFrame = currentFrame;
				float fmOsc = m_Set->m_RoutingBuffers.OscVoices[bank + 1][mVoiceNo]->getReadPointer(0)[currentFrame];
				//+ m_Set->m_RoutingBuffers.OscVoices[bank + 1][mVoiceNo]->getReadPointer(1)[currentFrame]) * 0.707f; //A->B, B->C, C->D
				float fmVal = 0.f;
				const float fmMultiplier = 441.0f;
				if (bank == 0)
					fmVal = m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fWTFXVal_OscA, MODMATDEST::OscAWTFXVal, &l_inputState) * fmMultiplier;
				else if (bank == 1)
					fmVal = m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fWTFXVal_OscB, MODMATDEST::OscBWTFXVal, &l_inputState) * fmMultiplier;
				else if (bank == 2)
					fmVal = m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fWTFXVal_OscC, MODMATDEST::OscCWTFXVal, &l_inputState) * fmMultiplier;
                l_Oscillator[bank].setFMFreq(fmVal * fmOsc);
                l_Oscillator[bank].setFrequency(osci, false); //this bypasses the freqsHaveToBeDoneForEachSample logic!
			}
			else {
                l_Oscillator[bank].setFrequency(osci, false);
			}
		}
		
		if (l_Oscillator[bank].doWavetableBufferStep(osci, currentFrame, l_phaseOffset))
			if (osci == 0) //master only
				wrap = true;
	} //for osci
	//most expensive loop
    //---------------------------------------------------------------------------------
	   
	if (freqsHaveToBeDoneForEachSample) { //for portamento and cent /pitch modulations
	//but not for each single sample - skip some!
		if (bUpdateNow) {
			prepareFrequency(bank, skips, currentFrame, bTakeNextValue, false);
			for (int osci = 0; osci < l_Oscillator[bank].m_unisonOscis; osci++) {
                l_Oscillator[bank].setFrequency(osci, false);
			}
			m_iLastFreqUpdate[bank] = 0;
		}
	}

	if (m_safePhaseFloat[bank] != l_phaseOffset) {
		m_safePhaseFloat[bank] = l_phaseOffset;
		m_Poly->m_OscBank[bank].setChangedFlagOsc();
	}
	
	if (wrap) {
		//if ((m_iLastCycleSamples[bank] > 0) && (m_iCurCycleSamples[bank] / float(m_iLastCycleSamples[bank]) < 0.99f)) {
		if ((m_iLastCycleSamples[bank] > 0) && (m_iLastCycleSamples[bank] - m_iCurCycleSamples[bank] > 3)) {
			wrap = false; //otherwise click in pitch modulation			
			prepareFrequency(bank, skips, currentFrame, bTakeNextValue, true);
			//VDBG("wrap skipped due to unfinished crossfade m_iCurCycleSamples " + String(m_iCurCycleSamples[bank]) + " m_iLastCycleSamples " + String(m_iLastCycleSamples[bank]));
		}
	}

	return wrap;
}

void CVASTSingleNote::doWavetableBufferGet(const int bank, CVASTWaveTableOscillator* mOscillator, const int osciCount, AudioSampleBuffer* lOscBuffer, const int startSample, const int numSamples) {
	//VDBG("doWavetableBufferGet voice " + String(mVoiceNo) + " startSample " + String(startSample) + " numSamples " + String(numSamples));
	if (osciCount == 0) return;
	m_Poly->m_iLastSingleNoteCycleCalls++;

	// grab the appropriate wavetable
	if (m_bSoftFadeCycleStarted[bank] && m_bSoftFadeCycleEnded[bank]) {
		if ((m_localVoiceBankWavetableSoftfade[bank] != nullptr) && ((m_localVoiceBankWavetableSoftfade[bank]->getID() == m_Poly->m_OscBank[bank].m_iSingleNoteSoftFadeID.load()))) {
			//cycle already completed for this voice // all done
			m_localVoiceBankWavetable[bank] = m_Poly->m_OscBank[bank].getNewSharedSoftFadeWavetable(); //continue with soft fade wavetable
			m_localVoiceBankWavetableSoftfade[bank] = m_Poly->m_OscBank[bank].getNewSharedSoftFadeWavetable();
		}
		else {
			m_bSoftFadeCycleStarted[bank] = false;
			m_bSoftFadeCycleEnded[bank] = false;
		}
	}
	else {
		if (m_localVoiceBankWavetable[bank] == nullptr) {//get current template from bank
			m_localVoiceBankWavetable[bank] = m_Poly->m_OscBank[bank].getNewSharedWavetable(); //new shared instance, can live longer
			m_localVoiceBankWavetableSoftfade[bank] = m_Poly->m_OscBank[bank].getNewSharedSoftFadeWavetable(); //new shared instance per bank and voice, can live longer
		}
		else {
			if ((m_localVoiceBankWavetableSoftfade[bank] == nullptr) && m_Poly->m_OscBank[bank].m_bWavetableSoftfadeStillRendered.load()) {
				m_localVoiceBankWavetableSoftfade[bank] = m_Poly->m_OscBank[bank].getNewSharedSoftFadeWavetable(); //new shared instance per bank and voice, can live longer
			}
		}
	}

#ifdef _DEBUG
	if (m_bSoftFadeCycleStarted[bank] && !m_bSoftFadeCycleEnded[bank])
		vassert(m_localVoiceBankWavetableSoftfade[bank] != nullptr);
	vassert(m_localVoiceBankWavetable[bank] != nullptr);
	//VDBG("doWavetableBufferGet with WT ID " + String(m_localVoiceBankWavetable[bank]->getID()));
#endif
		
	bool bInverter = false;
	switch (bank) {
	case 0:
		bInverter = *m_Set->m_State->m_bOscInverterOnOff_OscA == static_cast<int>(SWITCH::SWITCH_ON);
		break;
	case 1:
		bInverter = *m_Set->m_State->m_bOscInverterOnOff_OscB == static_cast<int>(SWITCH::SWITCH_ON);
		break;
	case 2:
		bInverter = *m_Set->m_State->m_bOscInverterOnOff_OscC == static_cast<int>(SWITCH::SWITCH_ON);
		break;
	case 3:
		bInverter = *m_Set->m_State->m_bOscInverterOnOff_OscD == static_cast<int>(SWITCH::SWITCH_ON);
		break;
	}
	
	//zero phasor
	bool bTakeNextValue = m_bSmoothersTakeNextValue;
	if (m_wtFXTypeChanged[bank])
		bTakeNextValue = true;
	if (bTakeNextValue) { //note on 
		for (int osci = 0; osci < m_Oscillator[bank].m_unisonOscis; osci++)  //reset fm freq
			m_Oscillator[bank].setFMFreq(0.f);

		m_bFreqsHaveToBeDoneForEachSample[bank] = prepareNextPhaseCycle(bank, m_iLastCycleSamples[bank], startSample, bTakeNextValue, m_wtFXTypeChanged[bank]);

		m_currentWTPosFloatPercentage[bank] = m_fOscWTPos_smoothed[bank].getTargetValue();
		vassert((m_currentWTPosFloatPercentage[bank] >= 0.f) && (m_currentWTPosFloatPercentage[bank] <= 1.f));
		m_fOscWTPos_smoothed[bank].setCurrentAndTargetValue(m_fOscWTPos_smoothed[bank].getTargetValue());
		m_fOscWTPos_atNoteOn[bank] = m_fOscWTPos_smoothed[bank].getTargetValue();

		//if a note is started new while softfade cyle is still active this note shall start with softfade wt right away and not add a softfade counter
		if (m_Poly->m_OscBank[bank].isInSingleNoteSoftFadeCycle() || m_Poly->m_OscBank[bank].m_bWavetableSoftfadeStillRendered.load()) {
			//all done
			m_localVoiceBankWavetable[bank] = m_Poly->m_OscBank[bank].getNewSharedSoftFadeWavetable(); //continue with soft fade wavetable
			m_localVoiceBankWavetableSoftfade[bank] = m_Poly->m_OscBank[bank].getNewSharedSoftFadeWavetable();
			m_bSoftFadeCycleStarted[bank] = true;
			m_bSoftFadeCycleEnded[bank] = true;
            VDBG("Softfade - note is started new while softfade cyle is still active this note shall start with softfade wt right away and not add a softfade counter");
            VDBG("CHECK removeSingleNoteSoftFadeCycle " << bank << " " << mVoiceNo);
            m_Poly->m_OscBank[bank].removeSingleNoteSoftFadeCycle(mVoiceNo);
		}

		m_iLastFreqUpdate[bank] = 999999;
		m_iCurCycleSamples[bank] = 0;
		m_iLastCycleSamples[bank] = 0;
		m_bIsFirstCycle[bank] = true;
		m_bIsStartOfCycle[bank] = true;
		bTakeNextValue = false;
	}

	//TODO:
	// - check for changed oscicount!

	//split in master osc 0 phases - also on new note
	int currentFrame = startSample;
	int nextStart = startSample;
	int sectionlength = 0;

	while (currentFrame < startSample + numSamples) {

		bool wrap = prepareEachSample(bank, currentFrame, m_bFreqsHaveToBeDoneForEachSample[bank], bTakeNextValue, m_Oscillator); //can change bFreqsHaveToBeDoneForEachSample on startcycle
		
		m_iCurCycleSamples[bank]++;
		if (wrap) { //start next section - section should be from start of osci 0 wrap until all oscis have wrapped and ended the cycle!											
			//this is the start of a phase 0 - but we are processing what is left until then //next should get new wtfxvalue 
			//VDBG("Wrap at sample " + String(currentFrame) + " m_iCurCycleSamples " + String(m_iCurCycleSamples[bank]) + " m_iLastCycleSamples " + String(m_iLastCycleSamples[bank]));

            bool lUpdated = false;
			if (sectionlength > 0) {
				//VDBG("doWavetableBufferGet  getWavetableInterpolateBuffer m_localVoiceBankWavetable voice " + String(mVoiceNo) + " startSample " + String(nextStart) + " numSamples " + String(sectionlength) + " wtPosPerc " + String(m_currentWTPosFloatPercentage[bank]));
				m_localVoiceBankWavetable[bank]->getWavetableInterpolateBuffer(mOscillator, osciCount, lOscBuffer, nextStart, sectionlength, bInverter, m_Set->m_WTmode, mVoiceNo, bank, m_bIsFirstCycle[bank], m_bIsStartOfCycle[bank], m_currentWTPosFloatPercentage[bank], m_wtFXVal[bank], m_wtFXType[bank], m_iLastCycleSamples[bank], m_iCurCycleSamples[bank], nullptr, lUpdated, m_Poly->m_OscBank[bank].getSoloMode());
				//=====================================================================
				//next interpolate buffer call will be start of new cycle -> set values for next phasec cycle
#ifdef _DEBUG
				if ((m_bLastRenderedWTID[bank] != -1) && (m_bLastRenderedWTID[bank] != m_localVoiceBankWavetable[bank]->getID()))
					VDBG("WT RENDER CHAIN NOT CONTINOUS!");
				m_bLastRenderedWTID[bank] = m_localVoiceBankWavetable[bank]->getID();
#endif
				if (lUpdated)
					m_Poly->m_OscBank[bank].setChangedFlagOsc();
			}
			//do also for zero section!
			m_bFreqsHaveToBeDoneForEachSample[bank] = prepareNextPhaseCycle(bank, m_iLastCycleSamples[bank], startSample, bTakeNextValue, false);
			m_bIsFirstCycle[bank] = false; //for next
			m_bIsStartOfCycle[bank] = true; //for next

			//Softfade------------------------------------------
			if (m_bSoftFadeCycleStarted[bank] && !m_bSoftFadeCycleEnded[bank]) { //end softfade here
				//vassert(m_localVoiceBankWavetableSoftfade[bank]->getID() == m_Poly->m_OscBank[bank].m_iSingleNoteSoftFadeID.load()); //must not be changed in meantime //is changed by dynamic generation??
				if (m_localVoiceBankWavetableSoftfade[bank] != nullptr) { //check why??
                    std::unique_ptr<AudioSampleBuffer> softFadeBuffer(new AudioSampleBuffer(2, lOscBuffer->getNumSamples()));
					softFadeBuffer->clear();
					bool lUpdated = false;
					if (sectionlength > 0) {
						//VDBG("doWavetableBufferGet  getWavetableInterpolateBuffer m_localVoiceBankWavetableSoftfade voice " + String(mVoiceNo) + " startSample " + String(nextStart) + " numSamples " + String(sectionlength) + " wtPosPerc " + String(m_currentWTPosFloatPercentage[bank]));
						m_localVoiceBankWavetableSoftfade[bank]->getWavetableInterpolateBuffer(mOscillator, osciCount, softFadeBuffer.get(), nextStart, sectionlength, bInverter, m_Set->m_WTmode, mVoiceNo, bank, m_bIsFirstCycle[bank], m_bIsStartOfCycle[bank], m_currentWTPosFloatPercentage[bank], m_wtFXVal[bank], m_wtFXType[bank], m_iLastCycleSamples[bank], m_iCurCycleSamples[bank], m_localVoiceBankWavetable[bank].get(), lUpdated, m_Poly->m_OscBank[bank].getSoloMode());
						if (lUpdated)
							m_Poly->m_OscBank[bank].setChangedFlagOsc();
					}
					//interpolate
					float startFracVal = float(m_iCurCycleSamples[bank] - sectionlength) / float(m_iLastCycleSamples[bank]);
					float endFracVal = float(m_iCurCycleSamples[bank]) / float(m_iLastCycleSamples[bank]);
					if (m_iLastCycleSamples[bank] < m_iCurCycleSamples[bank]) {
						startFracVal = 1.f;
						endFracVal = 1.f;
					}
					softFadeBuffer->applyGainRamp(nextStart, sectionlength, startFracVal, endFracVal);
					lOscBuffer->applyGainRamp(nextStart, sectionlength, (1.f - startFracVal), (1.f - endFracVal));
					lOscBuffer->addFrom(0, nextStart, softFadeBuffer->getReadPointer(0, nextStart), sectionlength);
					lOscBuffer->addFrom(1, nextStart, softFadeBuffer->getReadPointer(1, nextStart), sectionlength);
				}

#ifdef _DEBUG
				//VDBG("Ending softfade cycle - doWavetableBufferGet Voice " + String(mVoiceNo) + " SoftFadeWTID " + String(m_Poly->m_OscBank[bank].m_iSingleNoteSoftFadeID.load()) + " wtPosPerc " + String(m_currentWTPosFloatPercentage[bank]) + " bank " + String(bank));
				m_bLastRenderedWTID[bank] = m_localVoiceBankWavetableSoftfade[bank]->getID();
#endif
				m_localVoiceBankWavetable[bank] = m_Poly->m_OscBank[bank].getNewSharedSoftFadeWavetable(); //continue with soft fade wavetable
				//m_localVoiceBankWavetableSoftfade[bank] = nullptr; leave soft fade wt for reference
				vassert(m_localVoiceBankWavetable[bank] != nullptr);

				m_bSoftFadeCycleEnded[bank] = true;
                VDBG("CHECK removeSingleNoteSoftFadeCycle " << bank << " " << mVoiceNo);
				m_Poly->m_OscBank[bank].removeSingleNoteSoftFadeCycle(mVoiceNo);

			}
			else if (!m_bSoftFadeCycleStarted[bank] && !m_bSoftFadeCycleEnded[bank]) { //reset ended when all has ended! todo
				if (m_Poly->m_OscBank[bank].m_bWavetableSoftfadeStillRendered.load()) { //cycling but not started
					m_Poly->m_OscBank[bank].m_bWavetableSoftfadePickedUp.store(true);
					//VDBG("m_bWavetableSoftfadePickedUp true");
					m_localVoiceBankWavetableSoftfade[bank] = m_Poly->m_OscBank[bank].getNewSharedSoftFadeWavetable();
					if (m_localVoiceBankWavetableSoftfade[bank] != nullptr) { //fade to nullptr???
						m_bSoftFadeCycleStarted[bank] = true; //other voice has started the cycle! pick up next zero loop
						m_Poly->m_OscBank[bank].m_iSingleNoteSoftFadeID.store(m_localVoiceBankWavetableSoftfade[bank]->getID());
                        VDBG("CHECK addSingleNoteSoftFadeCycle " << bank << " " << mVoiceNo);
						m_Poly->m_OscBank[bank].addSingleNoteSoftFadeCycle(mVoiceNo); //one more cycling voice
					}
				}
			}
			//Softfade------------------------------------------

			float masterOsciPhaseInc = mOscillator->m_phaseIncBuffer[0]->getSample(0, currentFrame);
			if (masterOsciPhaseInc != 0.f)
				m_iLastCycleSamples[bank] = 1.f / masterOsciPhaseInc + 3; //estimation based on played frequency, 3 is safety, min 2
			
			//m_iLastCycleSamples[bank] = m_iCurCycleSamples[bank] + 1; //old logic

			m_iCurCycleSamples[bank] = 1;
			nextStart = nextStart + sectionlength;
			sectionlength = 1;
		}
		else {
			sectionlength++;
		}
		currentFrame++;
		//m_lastPhase[bank] = lPhase;
	}
	//do rest
	vassert(nextStart < (startSample + numSamples)); //this is the end of the buffer // maybe starting with zero phase or not
	int restlength = numSamples - (nextStart - startSample);
	bool lUpdated = false;
	if (restlength > 0) {		
		//VDBG("doWavetableBufferGet  getWavetableInterpolateBuffer m_localVoiceBankWavetable restlength voice " + String(mVoiceNo) + " startSample " + String(nextStart) + " numSamples " + String(restlength) + " wtPosPerc " + String(m_currentWTPosFloatPercentage[bank]));

		if (m_bIsFirstCycle[bank]) { //align wtpos at note on to make it stable
			m_currentWTPosFloatPercentage[bank] = m_fOscWTPos_atNoteOn[bank];
		}

		m_localVoiceBankWavetable[bank]->getWavetableInterpolateBuffer(mOscillator, osciCount, lOscBuffer, nextStart, restlength, bInverter, m_Set->m_WTmode, mVoiceNo, bank, m_bIsFirstCycle[bank], m_bIsStartOfCycle[bank], m_currentWTPosFloatPercentage[bank], m_wtFXVal[bank], m_wtFXType[bank], m_iLastCycleSamples[bank], m_iCurCycleSamples[bank], nullptr, lUpdated, m_Poly->m_OscBank[bank].getSoloMode());
		m_bIsFirstCycle[bank] = false; //for next
		m_bIsStartOfCycle[bank] = false; //for next
#ifdef _DEBUG
		if ((m_bLastRenderedWTID[bank] != -1) && (m_bLastRenderedWTID[bank] != m_localVoiceBankWavetable[bank]->getID()))
			VDBG("WT RENDER CHAIN NOT CONTINOUS!");
		m_bLastRenderedWTID[bank] = m_localVoiceBankWavetable[bank]->getID();
#endif
		if (lUpdated)
			m_Poly->m_OscBank[bank].setChangedFlagOsc();
	}

	if (m_bSoftFadeCycleStarted[bank] && !m_bSoftFadeCycleEnded[bank]) {
		//vassert(m_localVoiceBankWavetableSoftfade[bank]->getID() == m_Poly->m_OscBank[bank].m_iSingleNoteSoftFadeID.load()); //must not be changed in meantime //is changed by dynamic generation??
		if (m_localVoiceBankWavetableSoftfade[bank] != nullptr) { //check why??		
            std::unique_ptr<AudioSampleBuffer> softFadeBuffer(new AudioSampleBuffer(2, lOscBuffer->getNumSamples()));
			softFadeBuffer->clear();
			bool lUpdated = false;
			if (restlength > 0) {
				//VDBG("doWavetableBufferGet  getWavetableInterpolateBuffer m_localVoiceBankWavetableSoftfade restlength voice " + String(mVoiceNo) + " startSample " + String(nextStart) + " numSamples " + String(restlength) + " wtPosPerc " + String(m_currentWTPosFloatPercentage[bank]));
				m_localVoiceBankWavetableSoftfade[bank]->getWavetableInterpolateBuffer(mOscillator, osciCount, softFadeBuffer.get(), nextStart, restlength, bInverter, m_Set->m_WTmode, mVoiceNo, bank,  m_bIsFirstCycle[bank], m_bIsStartOfCycle[bank], m_currentWTPosFloatPercentage[bank], m_wtFXVal[bank], m_wtFXType[bank], m_iLastCycleSamples[bank], m_iCurCycleSamples[bank], m_localVoiceBankWavetable[bank].get(), lUpdated, m_Poly->m_OscBank[bank].getSoloMode());
				if (lUpdated)
					m_Poly->m_OscBank[bank].setChangedFlagOsc();
			}
			//interpolate
			float startFracVal = float(m_iCurCycleSamples[bank] - restlength) / float(m_iLastCycleSamples[bank]);
			float endFracVal = float(m_iCurCycleSamples[bank]) / float(m_iLastCycleSamples[bank]);
			if (m_iLastCycleSamples[bank] < m_iCurCycleSamples[bank]) {
				startFracVal = 1.f;
				endFracVal = 1.f;
			}
			softFadeBuffer->applyGainRamp(nextStart, restlength, startFracVal, endFracVal);
			lOscBuffer->applyGainRamp(nextStart, restlength, (1.f - startFracVal), (1.f - endFracVal));
			lOscBuffer->addFrom(0, nextStart, softFadeBuffer->getReadPointer(0, nextStart), restlength);
			lOscBuffer->addFrom(1, nextStart, softFadeBuffer->getReadPointer(1, nextStart), restlength);
		}
	}
#ifdef _DEBUG
    if (m_Poly->m_OscBank[bank].m_iSingleNoteSoftFadeCycle[mVoiceNo].load())
        VDBG("Voice will take softFade to next buffer: " << mVoiceNo);
#endif
    
#ifdef _DEBUG
	//detector for strange random part
	const float l_tolerance = .4f;
	for (int i = startSample; i < numSamples - 1; i++) {
		//within buffer
		if (abs(lOscBuffer->getSample(0, i) - lOscBuffer->getSample(0, i + 1)) > l_tolerance) {
			const float* UNUSED(buffer) = lOscBuffer->getReadPointer(0);
			//VDBG("Outliers!!!!!");
		}
		if (abs(lOscBuffer->getSample(1, i) - lOscBuffer->getSample(1, i + 1)) > l_tolerance) {
			const float* UNUSED(buffer) = lOscBuffer->getReadPointer(1);
			//VDBG("Outliers!!!!!");
		}
	}
	//buffer wrap
	if (m_fLastRenderedSample[bank] != 0.f) {
		if (abs(lOscBuffer->getSample(1, startSample) - m_fLastRenderedSample[bank]) > l_tolerance) { //right only
			const float* UNUSED(buffer) = lOscBuffer->getReadPointer(1);
			//VDBG("Outlier at wrap!!!!!");
		}
	}
	m_fLastRenderedSample[bank] = lOscBuffer->getSample(1, numSamples - 1);
#endif
}

//=============================================================================================================

void CVASTSingleNote::processBuffer(sRoutingBuffers& routingBuffers, int startSample, int numSamples) {
	modMatrixInputState l_inputState{ mVoiceNo, startSample };
	if (!isPlayingInRange(startSample, numSamples) || (m_iNumParallelOsc == 0)) {
		clearCurrentNote(); 
		
		resetSoftFadeState();

		if (m_bLastFilterOutputZero[0] && m_bLastFilterOutputZero[1] && m_bLastFilterOutputZero[2]) { //check 
			if (m_VCA.isActive())
				m_VCA.init(); //to reset envelopes
			return;
		}
	}
	else { 
#ifdef _DEBUG
		if (startSample > 0) 
			VDBG("StartSample > 0: " << startSample << " numSamples: " << numSamples << " bufferSamples: " << routingBuffers.getNumSamples());
#endif			
		m_Set->bufferInputState.store(l_inputState);

		//Mod matrix pitch
		float lSamplerPitch = m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fPitchMod, MODMATDEST::SamplerPitch, &l_inputState) * 0.01f;
        m_bSmoothersTakeNextValue ? m_fPitchModSampler_smoothed.setCurrentAndTargetValue(lSamplerPitch) :
        m_fPitchModSampler_smoothed.setTargetValue(lSamplerPitch);

		//who is really playing
		MYUINT uNumOscAOscsPlaying = 0;
		if (*m_Set->m_State->m_bOscOnOff_OscA == static_cast<int>(SWITCH::SWITCH_ON)) {
			//for (int i = 0; i < *m_Set->m_State->m_iNumOscs_OscA; i++) if (m_Oscillator[0][i].isPlaying() == true) uNumOscAOscsPlaying++;
			uNumOscAOscsPlaying = m_Oscillator[0].m_unisonOscis;
			if (*m_Set->m_State->m_bOscRetrigOnOff_OscA == static_cast<int>(SWITCH::SWITCH_ON))
				if (m_uLastuNumOscAOscsPlaying < uNumOscAOscsPlaying) { //added oscis generating
					for (int i = m_uLastuNumOscAOscsPlaying - 1; i < uNumOscAOscsPlaying; i++) { //sync new oscis to master
						if (i > 0)
							syncOscToMaster(0, i);
					}
				}
		}
		m_uLastuNumOscAOscsPlaying = uNumOscAOscsPlaying;

		MYUINT uNumOscBOscsPlaying = 0;
		if (*m_Set->m_State->m_bOscOnOff_OscB == static_cast<int>(SWITCH::SWITCH_ON)) {
			//for (int i = 0; i < *m_Set->m_State->m_iNumOscs_OscB; i++) if (m_Oscillator[1][i].isPlaying() == true) uNumOscBOscsPlaying++;
			uNumOscBOscsPlaying = m_Oscillator[1].m_unisonOscis;
			if (*m_Set->m_State->m_bOscRetrigOnOff_OscB == static_cast<int>(SWITCH::SWITCH_ON))
				if (m_uLastuNumOscBOscsPlaying < uNumOscBOscsPlaying) { //added oscis generating
					for (int i = m_uLastuNumOscBOscsPlaying - 1; i < uNumOscBOscsPlaying; i++) { //sync new oscis to master
						if (i > 0)
							syncOscToMaster(1, i);
					}
				}
		}
		m_uLastuNumOscBOscsPlaying = uNumOscBOscsPlaying;

		MYUINT uNumOscCOscsPlaying = 0;
		if (*m_Set->m_State->m_bOscOnOff_OscC == static_cast<int>(SWITCH::SWITCH_ON)) {
			//for (int i = 0; i < *m_Set->m_State->m_iNumOscs_OscC; i++) if (m_Oscillator[2][i].isPlaying() == true) uNumOscCOscsPlaying++;
			uNumOscCOscsPlaying = m_Oscillator[2].m_unisonOscis;
			if (*m_Set->m_State->m_bOscRetrigOnOff_OscC == static_cast<int>(SWITCH::SWITCH_ON))
				if (m_uLastuNumOscCOscsPlaying < uNumOscCOscsPlaying) { //added oscis generating
					for (int i = m_uLastuNumOscCOscsPlaying - 1; i < uNumOscCOscsPlaying; i++) { //sync new oscis to master
						if (i > 0)
							syncOscToMaster(2, i);
					}
				}
		}
		m_uLastuNumOscCOscsPlaying = uNumOscCOscsPlaying;

		MYUINT uNumOscDOscsPlaying = 0;
		if (*m_Set->m_State->m_bOscOnOff_OscD == static_cast<int>(SWITCH::SWITCH_ON)) {			
			//for (int i = 0; i < *m_Set->m_State->m_iNumOscs_OscD; i++) if (m_Oscillator[3][i].isPlaying() == true) uNumOscDOscsPlaying++;
			uNumOscDOscsPlaying = m_Oscillator[3].m_unisonOscis;
			if (*m_Set->m_State->m_bOscRetrigOnOff_OscD == static_cast<int>(SWITCH::SWITCH_ON))
				if (m_uLastuNumOscDOscsPlaying < uNumOscDOscsPlaying) { //added oscis generating
					for (int i = m_uLastuNumOscDOscsPlaying - 1; i < uNumOscDOscsPlaying; i++) { //sync new oscis to master
						if (i > 0)
							syncOscToMaster(3, i);
					}
				}
		}
		m_uLastuNumOscDOscsPlaying = uNumOscDOscsPlaying;

		MYUINT uNumOscNoisePlaying = 0;
		if (*m_Set->m_State->m_bNoiseOnOff == static_cast<int>(SWITCH::SWITCH_ON)) {
			if (m_OscillatorNoise.isPlaying() == true) uNumOscNoisePlaying++;
		}
		MYUINT uNumTotalPlaying = uNumOscAOscsPlaying + uNumOscBOscsPlaying + uNumOscCOscsPlaying + uNumOscDOscsPlaying + uNumOscNoisePlaying;
		m_uLast_NumTotalPlaying = uNumTotalPlaying;

		if (uNumOscAOscsPlaying > 0) {
			mSpread[0] = m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fSpread_OscA, MODMATDEST::OscASpread, &l_inputState) * 0.01f;
		}

		if (uNumOscBOscsPlaying > 0) {
			mSpread[1] = m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fSpread_OscB, MODMATDEST::OscBSpread, &l_inputState) * 0.01f;
		}

		if (uNumOscCOscsPlaying > 0) {
			mSpread[2] = m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fSpread_OscC, MODMATDEST::OscCSpread, &l_inputState) * 0.01f;
		}

		if (uNumOscDOscsPlaying > 0) {
			mSpread[3] = m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fSpread_OscD, MODMATDEST::OscDSpread, &l_inputState) * 0.01f;
		}

		for (int bank = 3; bank >= 0; bank--) { //main loop - start with D go to A for FX (A from B)
			if (m_bSmoothersTakeNextValue) {
				//reset softfade here? cannot harm - nothing is playing
				m_localVoiceBankWavetable[bank] = nullptr; //CHECK THIS HERE - THIS IS EXPENSIVE AND LEADS TO MANY ATOMIC LOCKS 
				m_localVoiceBankWavetableSoftfade[bank] = nullptr;
				m_bSoftFadeCycleStarted[bank] = false;
				m_bSoftFadeCycleEnded[bank] = false;
			}

			int lNumber = 0;
			if (bank == 0) lNumber = uNumOscAOscsPlaying;
			else if (bank == 1) lNumber = uNumOscBOscsPlaying;
			else if (bank == 2) lNumber = uNumOscCOscsPlaying;
			else if (bank == 3) lNumber = uNumOscDOscsPlaying;

			AudioSampleBuffer* l_OscBuffer;
			switch (bank) {
			case 0:
				l_OscBuffer = routingBuffers.OscVoices[0][mVoiceNo];
				m_wtFXTypeChanged[bank] = (m_wtFXType[bank] != m_Set->m_State->m_uWTFX_OscA->load());
				m_wtFXType[bank] = m_Set->m_State->m_uWTFX_OscA->load();
				if (m_wtFXType[bank] > 0) {
					float fVal = m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fWTFXVal_OscA, MODMATDEST::OscAWTFXVal, &l_inputState);
					if (m_bSmoothersTakeNextValue)
						m_wtFXVal_smoothed[bank].setCurrentAndTargetValue(fVal);
					else
						m_wtFXVal_smoothed[bank].setTargetValue(fVal);
				}
				else {
					m_wtFXVal_smoothed[bank].setCurrentAndTargetValue(0.f);
					m_wtFXVal[bank] = 0.f;
				}
				break;
			case 1:
				l_OscBuffer = routingBuffers.OscVoices[1][mVoiceNo];
				m_wtFXTypeChanged[bank] = (m_wtFXType[bank] != m_Set->m_State->m_uWTFX_OscB->load());
				m_wtFXType[bank] = m_Set->m_State->m_uWTFX_OscB->load();
				if (m_wtFXType[bank] > 0) {
					float fVal = m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fWTFXVal_OscB, MODMATDEST::OscBWTFXVal, &l_inputState);
					if (m_bSmoothersTakeNextValue)
						m_wtFXVal_smoothed[bank].setCurrentAndTargetValue(fVal);
					else 
						m_wtFXVal_smoothed[bank].setTargetValue(fVal);
				}
				else {
					m_wtFXVal_smoothed[bank].setCurrentAndTargetValue(0.f);
					m_wtFXVal[bank] = 0.f;
				}
				break;
			case 2:
				l_OscBuffer = routingBuffers.OscVoices[2][mVoiceNo];
				m_wtFXTypeChanged[bank] = (m_wtFXType[bank] != m_Set->m_State->m_uWTFX_OscC->load());
				m_wtFXType[bank] = m_Set->m_State->m_uWTFX_OscC->load();
				if (m_wtFXType[bank] > 0) {
					float fVal = m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fWTFXVal_OscC, MODMATDEST::OscCWTFXVal, &l_inputState);
					if (m_bSmoothersTakeNextValue)
						m_wtFXVal_smoothed[bank].setCurrentAndTargetValue(fVal);
					else
						m_wtFXVal_smoothed[bank].setTargetValue(fVal);
				}
				else {
					m_wtFXVal_smoothed[bank].setCurrentAndTargetValue(0.f);
					m_wtFXVal[bank] = 0.f;
				}
				break;
			case 3:
				l_OscBuffer = routingBuffers.OscVoices[3][mVoiceNo];
				m_wtFXTypeChanged[bank] = (m_wtFXType[bank] != m_Set->m_State->m_uWTFX_OscD->load());
				m_wtFXType[bank] = m_Set->m_State->m_uWTFX_OscD->load();
				if (m_wtFXType[bank] > 0) {
					float fVal = m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fWTFXVal_OscD, MODMATDEST::OscDWTFXVal, &l_inputState);
					if (m_bSmoothersTakeNextValue)
						m_wtFXVal_smoothed[bank].setCurrentAndTargetValue(fVal);
					else
						m_wtFXVal_smoothed[bank].setTargetValue(fVal);
				}
				else {
					m_wtFXVal_smoothed[bank].setCurrentAndTargetValue(0.f);
					m_wtFXVal[bank] = 0.f;
				}
				break;
			}

			//===============================================================================

			initWavetableProcessing(bank, routingBuffers, l_inputState); //per Bank

			//===============================================================================

			for (int osci = 0; osci < lNumber; osci++) {

				//===============================================================================

				m_Oscillator[bank].doWavetableBufferInit(osci, routingBuffers, l_inputState); //per Osc

				//===============================================================================
			}

			//===============================================================================

			doWavetableBufferGet(bank, &m_Oscillator[bank], lNumber, l_OscBuffer, startSample, numSamples);

			//===============================================================================

			/*
			//							0
			//	-100										+100
			//  -100					0					+100
			//  -100			-33			+33				+100
			//  -100	   -50			0		+50			+100

			//((-100...100 / 100.0f) + 1) * M_PI / 4.0;
			*/
		} //for (int bank)

		if (uNumOscNoisePlaying == 1) {
			AudioSampleBuffer* l_OscBuffer;
			l_OscBuffer = routingBuffers.NoiseVoices[mVoiceNo];
			m_OscillatorNoise.setFrequency(0, false); // do not update every frame!

			for (int currentFrame = startSample; currentFrame < startSample + numSamples; currentFrame++) {
				float fOsci = 0.0f;
				m_OscillatorNoise.getOscillation(&fOsci);
				l_OscBuffer->addSample(0, currentFrame, fOsci);
				m_OscillatorNoise.getOscillation(&fOsci); //stereo now
				l_OscBuffer->addSample(1, currentFrame, fOsci);
			}
		}

		//====================================================================================
		//calc velocity buffer
		bool doVelocity = false;
		m_velocityBuffer->setSize(1, routingBuffers.getNumSamples(), false, false, true); //keep size to be symmetrical
		m_velocityBuffer->clear();
		if (m_Set->modMatrixDestinationSetFast(MODMATDEST::VoiceGain)) {
			//check for optimization here
			doVelocity = true;
			bool takeNext = m_bSmoothersTakeNextValue;
			for (int i = startSample; i < startSample + numSamples; i++) {
				l_inputState.currentFrame = i;						
				float fVoiceGain = m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fVoiceGain, MODMATDEST::VoiceGain, &l_inputState);
                takeNext ? m_fVoiceGain_smoothed.setCurrentAndTargetValue(fVoiceGain * 0.01f) : m_fVoiceGain_smoothed.setTargetValue(fVoiceGain * 0.01f);
				m_velocityBuffer->getWritePointer(0)[i] = m_fVoiceGain_smoothed.getNextValue();
				takeNext = false;
			}
		}

		//====================================================================================
		//spread, normalize to max peaks and do pan	
		m_centerBuffer->setSize(1, routingBuffers.getNumSamples(), false, false, true); //keep size to be symmetrical
		if (uNumOscAOscsPlaying > 0) {
			m_centerBuffer->clear();
			m_centerBuffer->copyFrom(0, startSample, routingBuffers.OscVoices[0][mVoiceNo]->getReadPointer(0, startSample), numSamples);
			m_centerBuffer->addFrom(0, startSample, routingBuffers.OscVoices[0][mVoiceNo]->getReadPointer(1, startSample), numSamples);
			m_centerBuffer->applyGain(0.5f * (1.0 - mSpread[0]));
			routingBuffers.OscVoices[0][mVoiceNo]->applyGain(mSpread[0]);
			routingBuffers.OscVoices[0][mVoiceNo]->addFrom(0, startSample, m_centerBuffer->getReadPointer(0, startSample), numSamples);
			routingBuffers.OscVoices[0][mVoiceNo]->addFrom(1, startSample, m_centerBuffer->getReadPointer(0, startSample), numSamples);

            m_bSmoothersTakeNextValue ? m_fOscAPan_smoothed.setCurrentAndTargetValue(m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fOscPan_OscA, MODMATDEST::OscAPan, &l_inputState) * 0.01f) : m_fOscAPan_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fOscPan_OscA, MODMATDEST::OscAPan, &l_inputState) * 0.01f);
			float fPanBegin = (m_fOscAPan_smoothed.getNextValue() + 1.f) * M_PI / 4.0f; // equal power calculation
			m_fOscAPan_smoothed.skip(numSamples - 2);
			float fPanEnd = (m_fOscAPan_smoothed.getNextValue() + 1.f) * M_PI / 4.0f; // equal power calculation
			
            m_bSmoothersTakeNextValue ? m_fOscAGain_smoothed.setCurrentAndTargetValue(m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fOscGain_OscA, MODMATDEST::OscAGain, &l_inputState) * 0.01f) : m_fOscAGain_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fOscGain_OscA, MODMATDEST::OscAGain, &l_inputState) * 0.01f);
			float fAttenuateBegin = m_fOscAGain_smoothed.getNextValue();
			m_fOscAGain_smoothed.skip(numSamples - 2);
			float fAttenuateEnd = m_fOscAGain_smoothed.getNextValue();
			if (uNumOscAOscsPlaying > 1) {
				fAttenuateBegin *= 1.f / (0.707f * float(uNumOscAOscsPlaying));
				fAttenuateEnd *= 1.f / (0.707f * float(uNumOscAOscsPlaying));
			}
			if (doVelocity) {
				FloatVectorOperations::multiply(routingBuffers.OscVoices[0][mVoiceNo]->getWritePointer(0, startSample), m_velocityBuffer->getReadPointer(0, startSample), numSamples); //multiply with velocity		
				FloatVectorOperations::multiply(routingBuffers.OscVoices[0][mVoiceNo]->getWritePointer(1, startSample), m_velocityBuffer->getReadPointer(0, startSample), numSamples); //multiply with velocity		
			}

			routingBuffers.OscVoices[0][mVoiceNo]->applyGainRamp(0, startSample, numSamples, fAttenuateBegin * cosf(fPanBegin), fAttenuateEnd * cosf(fPanEnd));
			routingBuffers.OscVoices[0][mVoiceNo]->applyGainRamp(1, startSample, numSamples, fAttenuateBegin * sinf(fPanBegin), fAttenuateEnd * sinf(fPanEnd));
		}

		if (uNumOscBOscsPlaying > 0) {
			m_centerBuffer->clear();
			m_centerBuffer->copyFrom(0, startSample, routingBuffers.OscVoices[1][mVoiceNo]->getReadPointer(0, startSample), numSamples);
			m_centerBuffer->addFrom(0, startSample, routingBuffers.OscVoices[1][mVoiceNo]->getReadPointer(1, startSample), numSamples);
			m_centerBuffer->applyGain(0.5f * (1.0 - mSpread[1]));
			routingBuffers.OscVoices[1][mVoiceNo]->applyGain(mSpread[1]);
			routingBuffers.OscVoices[1][mVoiceNo]->addFrom(0, startSample, m_centerBuffer->getReadPointer(0, startSample), numSamples);
			routingBuffers.OscVoices[1][mVoiceNo]->addFrom(1, startSample, m_centerBuffer->getReadPointer(0, startSample), numSamples);

            m_bSmoothersTakeNextValue ? m_fOscBPan_smoothed.setCurrentAndTargetValue(m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fOscPan_OscB, MODMATDEST::OscBPan, &l_inputState) * 0.01f) : m_fOscBPan_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fOscPan_OscB, MODMATDEST::OscBPan, &l_inputState) * 0.01f);
			float fPanBegin = (m_fOscBPan_smoothed.getNextValue() + 1.f) * M_PI / 4.0f; // equal power calculation
			m_fOscBPan_smoothed.skip(numSamples - 2);
			float fPanEnd = (m_fOscBPan_smoothed.getNextValue() + 1.f) * M_PI / 4.0f; // equal power calculation

            m_bSmoothersTakeNextValue ? m_fOscBGain_smoothed.setCurrentAndTargetValue(m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fOscGain_OscB, MODMATDEST::OscBGain, &l_inputState) * 0.01f) : m_fOscBGain_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fOscGain_OscB, MODMATDEST::OscBGain, &l_inputState) * 0.01f);
			float fAttenuateBegin = m_fOscBGain_smoothed.getNextValue();
			m_fOscBGain_smoothed.skip(numSamples - 2);
			float fAttenuateEnd = m_fOscBGain_smoothed.getNextValue();
			if (uNumOscBOscsPlaying > 1) {
				fAttenuateBegin *= 1.f / (0.707f * float(uNumOscBOscsPlaying));
				fAttenuateEnd *= 1.f / (0.707f * float(uNumOscBOscsPlaying));
			}
			if (doVelocity) {
				FloatVectorOperations::multiply(routingBuffers.OscVoices[1][mVoiceNo]->getWritePointer(0, startSample), m_velocityBuffer->getReadPointer(0, startSample), numSamples); //multiply with velocity		
				FloatVectorOperations::multiply(routingBuffers.OscVoices[1][mVoiceNo]->getWritePointer(1, startSample), m_velocityBuffer->getReadPointer(0, startSample), numSamples); //multiply with velocity		
			}

			routingBuffers.OscVoices[1][mVoiceNo]->applyGainRamp(0, startSample, numSamples, fAttenuateBegin * cosf(fPanBegin), fAttenuateEnd * cosf(fPanEnd));
			routingBuffers.OscVoices[1][mVoiceNo]->applyGainRamp(1, startSample, numSamples, fAttenuateBegin * sinf(fPanBegin), fAttenuateEnd * sinf(fPanEnd));

		}
		if (uNumOscCOscsPlaying > 0) {
			m_centerBuffer->clear();
			m_centerBuffer->copyFrom(0, startSample, routingBuffers.OscVoices[2][mVoiceNo]->getReadPointer(0, startSample), numSamples);
			m_centerBuffer->addFrom(0, startSample, routingBuffers.OscVoices[2][mVoiceNo]->getReadPointer(1, startSample), numSamples);
			m_centerBuffer->applyGain(0.5f * (1.0 - mSpread[2]));
			routingBuffers.OscVoices[2][mVoiceNo]->applyGain(mSpread[2]);
			routingBuffers.OscVoices[2][mVoiceNo]->addFrom(0, startSample, m_centerBuffer->getReadPointer(0, startSample), numSamples);
			routingBuffers.OscVoices[2][mVoiceNo]->addFrom(1, startSample, m_centerBuffer->getReadPointer(0, startSample), numSamples);

            m_bSmoothersTakeNextValue ? m_fOscCPan_smoothed.setCurrentAndTargetValue(m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fOscPan_OscC, MODMATDEST::OscCPan, &l_inputState) * 0.01f) : m_fOscCPan_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fOscPan_OscC, MODMATDEST::OscCPan, &l_inputState) * 0.01f);
			float fPanBegin = (m_fOscCPan_smoothed.getNextValue() + 1.f) * M_PI / 4.0f; // equal power calculation
			m_fOscCPan_smoothed.skip(numSamples - 2);
			float fPanEnd = (m_fOscCPan_smoothed.getNextValue() + 1.f) * M_PI / 4.0f; // equal power calculation

            m_bSmoothersTakeNextValue ? m_fOscCGain_smoothed.setCurrentAndTargetValue(m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fOscGain_OscC, MODMATDEST::OscCGain, &l_inputState) * 0.01f) : m_fOscCGain_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fOscGain_OscC, MODMATDEST::OscCGain, &l_inputState) * 0.01f);
			float fAttenuateBegin = m_fOscCGain_smoothed.getNextValue();
			m_fOscCGain_smoothed.skip(numSamples - 2);
			float fAttenuateEnd = m_fOscCGain_smoothed.getNextValue();
			if (uNumOscCOscsPlaying > 1) {
				fAttenuateBegin *= 1.f / (0.707f * float(uNumOscCOscsPlaying));
				fAttenuateEnd *= 1.f / (0.707f * float(uNumOscCOscsPlaying));
			}
			if (doVelocity) {
				FloatVectorOperations::multiply(routingBuffers.OscVoices[2][mVoiceNo]->getWritePointer(0, startSample), m_velocityBuffer->getReadPointer(0, startSample), numSamples); //multiply with velocity		
				FloatVectorOperations::multiply(routingBuffers.OscVoices[2][mVoiceNo]->getWritePointer(1, startSample), m_velocityBuffer->getReadPointer(0, startSample), numSamples); //multiply with velocity
			}

			routingBuffers.OscVoices[2][mVoiceNo]->applyGainRamp(0, startSample, numSamples, fAttenuateBegin * cosf(fPanBegin), fAttenuateEnd * cosf(fPanEnd));
			routingBuffers.OscVoices[2][mVoiceNo]->applyGainRamp(1, startSample, numSamples, fAttenuateBegin * sinf(fPanBegin), fAttenuateEnd * sinf(fPanEnd));
		}
		if (uNumOscDOscsPlaying > 0) {
			m_centerBuffer->clear();
			m_centerBuffer->copyFrom(0, startSample, routingBuffers.OscVoices[3][mVoiceNo]->getReadPointer(0, startSample), numSamples);
			m_centerBuffer->addFrom(0, startSample, routingBuffers.OscVoices[3][mVoiceNo]->getReadPointer(1, startSample), numSamples);
			m_centerBuffer->applyGain(0.5f * (1.0 - mSpread[3]));
			routingBuffers.OscVoices[3][mVoiceNo]->applyGain(mSpread[3]);
			routingBuffers.OscVoices[3][mVoiceNo]->addFrom(0, startSample, m_centerBuffer->getReadPointer(0, startSample), numSamples);
			routingBuffers.OscVoices[3][mVoiceNo]->addFrom(1, startSample, m_centerBuffer->getReadPointer(0, startSample), numSamples);

            m_bSmoothersTakeNextValue ? m_fOscDPan_smoothed.setCurrentAndTargetValue(m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fOscPan_OscD, MODMATDEST::OscDPan, &l_inputState) * 0.01f) : m_fOscDPan_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fOscPan_OscD, MODMATDEST::OscDPan, &l_inputState) * 0.01f);
			float fPanBegin = (m_fOscDPan_smoothed.getNextValue() + 1.f) * M_PI / 4.0f; // equal power calculation
			m_fOscDPan_smoothed.skip(numSamples - 2);
			float fPanEnd = (m_fOscDPan_smoothed.getNextValue() + 1.f) * M_PI / 4.0f; // equal power calculation

            m_bSmoothersTakeNextValue ? m_fOscDGain_smoothed.setCurrentAndTargetValue(m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fOscGain_OscD, MODMATDEST::OscDGain, &l_inputState) * 0.01f) : m_fOscDGain_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fOscGain_OscD, MODMATDEST::OscDGain, &l_inputState) * 0.01f);
			float fAttenuateBegin = m_fOscDGain_smoothed.getNextValue();
			m_fOscDGain_smoothed.skip(numSamples - 2);
			float fAttenuateEnd = m_fOscDGain_smoothed.getNextValue();
			if (uNumOscDOscsPlaying > 1) {
				fAttenuateBegin *= 1.f / (0.707f * float(uNumOscDOscsPlaying));
				fAttenuateEnd *= 1.f / (0.707f * float(uNumOscDOscsPlaying));
			}
			if (doVelocity) {
				FloatVectorOperations::multiply(routingBuffers.OscVoices[3][mVoiceNo]->getWritePointer(0, startSample), m_velocityBuffer->getReadPointer(0, startSample), numSamples); //multiply with velocity		
				FloatVectorOperations::multiply(routingBuffers.OscVoices[3][mVoiceNo]->getWritePointer(1, startSample), m_velocityBuffer->getReadPointer(0, startSample), numSamples); //multiply with velocity		
			}

			routingBuffers.OscVoices[3][mVoiceNo]->applyGainRamp(0, startSample, numSamples, fAttenuateBegin * cosf(fPanBegin), fAttenuateEnd * cosf(fPanEnd));
			routingBuffers.OscVoices[3][mVoiceNo]->applyGainRamp(1, startSample, numSamples, fAttenuateBegin * sinf(fPanBegin), fAttenuateEnd * sinf(fPanEnd));
		}

		if (uNumOscNoisePlaying > 0) {
            m_bSmoothersTakeNextValue ? m_fNoisePan_smoothed.setCurrentAndTargetValue(m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fNoisePan, MODMATDEST::NoisePan, &l_inputState) * 0.01f) : m_fNoisePan_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fNoisePan, MODMATDEST::NoisePan, &l_inputState) * 0.01f);
			float fPanBegin = (m_fNoisePan_smoothed.getNextValue() + 1.f) * M_PI / 4.0f; // equal power calculation
			m_fNoisePan_smoothed.skip(numSamples - 2);
			float fPanEnd = (m_fNoisePan_smoothed.getNextValue() + 1.f) * M_PI / 4.0f; // equal power calculation

            m_bSmoothersTakeNextValue ? m_fNoiseGain_smoothed.setCurrentAndTargetValue(m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fNoiseGain, MODMATDEST::NoiseGain, &l_inputState) * 0.01f) :         m_fNoiseGain_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fNoiseGain, MODMATDEST::NoiseGain, &l_inputState) * 0.01f);
			float fAttenuateBegin = m_fNoiseGain_smoothed.getNextValue();
			m_fNoiseGain_smoothed.skip(numSamples - 2);
			float fAttenuateEnd = m_fNoiseGain_smoothed.getNextValue();
			if (doVelocity) {
				FloatVectorOperations::multiply(routingBuffers.NoiseVoices[mVoiceNo]->getWritePointer(0, startSample), m_velocityBuffer->getReadPointer(0, startSample), numSamples); //multiply with velocity		
				FloatVectorOperations::multiply(routingBuffers.NoiseVoices[mVoiceNo]->getWritePointer(1, startSample), m_velocityBuffer->getReadPointer(0, startSample), numSamples); //multiply with velocity		
			}
			routingBuffers.NoiseVoices[mVoiceNo]->applyGainRamp(0, startSample, numSamples, fAttenuateBegin * cosf(fPanBegin), fAttenuateEnd * cosf(fPanEnd));
			routingBuffers.NoiseVoices[mVoiceNo]->applyGainRamp(1, startSample, numSamples, fAttenuateBegin * sinf(fPanBegin), fAttenuateEnd * sinf(fPanEnd));
		}

		//====================================================================================
		//sampler
		if (*m_Set->m_State->m_bSamplerOnOff == static_cast<int>(SWITCH::SWITCH_ON)) {
            m_bSmoothersTakeNextValue ? m_fSamplerPan_smoothed.setCurrentAndTargetValue(m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fSamplerPan, MODMATDEST::SamplerPan, &l_inputState) * 0.01f) : m_fSamplerPan_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fSamplerPan, MODMATDEST::SamplerPan, &l_inputState) * 0.01f);
			float fPanBegin = (m_fSamplerPan_smoothed.getNextValue() + 1.f) * M_PI / 4.0f; // equal power calculation
			m_fSamplerPan_smoothed.skip(numSamples - 2);
			float fPanEnd = (m_fSamplerPan_smoothed.getNextValue() + 1.f) * M_PI / 4.0f; // equal power calculation

			float lPitchMod = 0.f; //-1 .. 1 center 0	
			if (m_Set->modMatrixDestinationSetFast(MODMATDEST::Pitch)) {
				lPitchMod = m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fPitchMod, MODMATDEST::Pitch, &l_inputState) * 0.01f;
			}
            m_bSmoothersTakeNextValue ? m_fPitchModSampler_smoothed.setCurrentAndTargetValue(lPitchMod) : m_fPitchModSampler_smoothed.setTargetValue(lPitchMod);
			
			float lSamplerCents = m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fSamplerCents, MODMATDEST::SamplerPitch, &l_inputState);
            m_bSmoothersTakeNextValue ? m_fSamplerCents_smoothed.setCurrentAndTargetValue(lSamplerCents) : m_fSamplerCents_smoothed.setTargetValue(lSamplerCents);

			samplerRenderNextBlock(routingBuffers.SamplerVoices[mVoiceNo], startSample, numSamples); // render sample
#ifdef _DEBUG
			vassert(routingBuffers.SamplerVoices[mVoiceNo]->getMagnitude(0, numSamples) < 20.f);
#endif

			//has to be louder, why?
            m_bSmoothersTakeNextValue ? m_fSamplerGain_smoothed.setCurrentAndTargetValue(m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fSamplerGain, MODMATDEST::SamplerGain, &l_inputState) * 0.01f * 4.f) : m_fSamplerGain_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fSamplerGain, MODMATDEST::SamplerGain, &l_inputState) * 0.01f * 4.f);
			float fAttenuateBegin = m_fSamplerGain_smoothed.getNextValue();
			m_fSamplerGain_smoothed.skip(numSamples - 2);
			float fAttenuateEnd = m_fSamplerGain_smoothed.getNextValue();
			if (doVelocity) {
				FloatVectorOperations::multiply(routingBuffers.SamplerVoices[mVoiceNo]->getWritePointer(0, startSample), m_velocityBuffer->getReadPointer(0, startSample), numSamples); //multiply with velocity		
				FloatVectorOperations::multiply(routingBuffers.SamplerVoices[mVoiceNo]->getWritePointer(1, startSample), m_velocityBuffer->getReadPointer(0, startSample), numSamples); //multiply with velocity		
			}

			routingBuffers.SamplerVoices[mVoiceNo]->applyGainRamp(0, startSample, numSamples, fAttenuateBegin * cosf(fPanBegin), fAttenuateEnd * cosf(fPanEnd));
			routingBuffers.SamplerVoices[mVoiceNo]->applyGainRamp(1, startSample, numSamples, fAttenuateBegin * sinf(fPanBegin), fAttenuateEnd * sinf(fPanEnd));
			m_uLast_NumTotalPlaying++;
		}

		//====================================================================================
		//VCA
		//if (!routingBuffers.OscVoices[0][mVoiceNo]->hasBeenCleared()) {
		if (uNumOscAOscsPlaying > 0) {
			const float* envBuffer = nullptr;
			if (*m_Set->m_State->m_uVCAEnv_OscA == static_cast<int>(MSEGENV::MSEG1)) {
				envBuffer = routingBuffers.MSEGBuffer[0][mVoiceNo]->getReadPointer(0, startSample);
			}
			else if (*m_Set->m_State->m_uVCAEnv_OscA == static_cast<int>(MSEGENV::MSEG2)) {
				envBuffer = routingBuffers.MSEGBuffer[1][mVoiceNo]->getReadPointer(0, startSample);
			}
			else if (*m_Set->m_State->m_uVCAEnv_OscA == static_cast<int>(MSEGENV::MSEG3)) {
				envBuffer = routingBuffers.MSEGBuffer[2][mVoiceNo]->getReadPointer(0, startSample);
			}
			else if (*m_Set->m_State->m_uVCAEnv_OscA == static_cast<int>(MSEGENV::MSEG4)) {
				envBuffer = routingBuffers.MSEGBuffer[3][mVoiceNo]->getReadPointer(0, startSample);
			}
			else if (*m_Set->m_State->m_uVCAEnv_OscA == static_cast<int>(MSEGENV::MSEG5)) {
				envBuffer = routingBuffers.MSEGBuffer[4][mVoiceNo]->getReadPointer(0, startSample);
			}
			if (envBuffer) {
				FloatVectorOperations::multiply(routingBuffers.OscVoices[0][mVoiceNo]->getWritePointer(0, startSample), envBuffer, numSamples); //voice = voice * mseg
				FloatVectorOperations::multiply(routingBuffers.OscVoices[0][mVoiceNo]->getWritePointer(1, startSample), envBuffer, numSamples); //voice = voice * mseg
			}
			routingBuffers.OscBuffer[0]->addFrom(0, startSample, routingBuffers.OscVoices[0][mVoiceNo]->getReadPointer(0, startSample), numSamples);
			routingBuffers.OscBuffer[0]->addFrom(1, startSample, routingBuffers.OscVoices[0][mVoiceNo]->getReadPointer(1, startSample), numSamples);

			//when to reset filter?		
			//m_bqEndLowShelfL[0].processSamples(routingBuffers.OscBuffer[0]->getWritePointer(0, startSample), numSamples);
			//m_bqEndLowShelfR[0].processSamples(routingBuffers.OscBuffer[0]->getWritePointer(1, startSample), numSamples);
		}
		//if (!routingBuffers.OscVoices[1][mVoiceNo]->hasBeenCleared()) {
		if (uNumOscBOscsPlaying > 0) {
			const float* envBuffer = nullptr;
			if (*m_Set->m_State->m_uVCAEnv_OscB == static_cast<int>(MSEGENV::MSEG1)) {
				envBuffer = routingBuffers.MSEGBuffer[0][mVoiceNo]->getReadPointer(0, startSample);
			}
			else if (*m_Set->m_State->m_uVCAEnv_OscB == static_cast<int>(MSEGENV::MSEG2)) {
				envBuffer = routingBuffers.MSEGBuffer[1][mVoiceNo]->getReadPointer(0, startSample);
			}
			else if (*m_Set->m_State->m_uVCAEnv_OscB == static_cast<int>(MSEGENV::MSEG3)) {
				envBuffer = routingBuffers.MSEGBuffer[2][mVoiceNo]->getReadPointer(0, startSample);
			}
			else if (*m_Set->m_State->m_uVCAEnv_OscB == static_cast<int>(MSEGENV::MSEG4)) {
				envBuffer = routingBuffers.MSEGBuffer[3][mVoiceNo]->getReadPointer(0, startSample);
			}
			else if (*m_Set->m_State->m_uVCAEnv_OscB == static_cast<int>(MSEGENV::MSEG5)) {
				envBuffer = routingBuffers.MSEGBuffer[4][mVoiceNo]->getReadPointer(0, startSample);
			}
			if (envBuffer) {
				FloatVectorOperations::multiply(routingBuffers.OscVoices[1][mVoiceNo]->getWritePointer(0, startSample), envBuffer, numSamples); //voice = voice * mseg
				FloatVectorOperations::multiply(routingBuffers.OscVoices[1][mVoiceNo]->getWritePointer(1, startSample), envBuffer, numSamples); //voice = voice * mseg
			}
			routingBuffers.OscBuffer[1]->addFrom(0, startSample, routingBuffers.OscVoices[1][mVoiceNo]->getReadPointer(0, startSample), numSamples);
			routingBuffers.OscBuffer[1]->addFrom(1, startSample, routingBuffers.OscVoices[1][mVoiceNo]->getReadPointer(1, startSample), numSamples);			
			//when to reset filter?		
			//m_bqEndLowShelfL[1].processSamples(routingBuffers.OscBuffer[1]->getWritePointer(0, startSample), numSamples);
			//m_bqEndLowShelfR[1].processSamples(routingBuffers.OscBuffer[1]->getWritePointer(1, startSample), numSamples);
		}
		//if (!routingBuffers.OscVoices[2][mVoiceNo]->hasBeenCleared()) {
		if (uNumOscCOscsPlaying > 0) {
			const float* envBuffer = nullptr;
			if (*m_Set->m_State->m_uVCAEnv_OscC == static_cast<int>(MSEGENV::MSEG1)) {
				envBuffer = routingBuffers.MSEGBuffer[0][mVoiceNo]->getReadPointer(0, startSample);
			}
			else if (*m_Set->m_State->m_uVCAEnv_OscC == static_cast<int>(MSEGENV::MSEG2)) {
				envBuffer = routingBuffers.MSEGBuffer[1][mVoiceNo]->getReadPointer(0, startSample);
			}
			else if (*m_Set->m_State->m_uVCAEnv_OscC == static_cast<int>(MSEGENV::MSEG3)) {
				envBuffer = routingBuffers.MSEGBuffer[2][mVoiceNo]->getReadPointer(0, startSample);
			}
			else if (*m_Set->m_State->m_uVCAEnv_OscC == static_cast<int>(MSEGENV::MSEG4)) {
				envBuffer = routingBuffers.MSEGBuffer[3][mVoiceNo]->getReadPointer(0, startSample);
			}
			else if (*m_Set->m_State->m_uVCAEnv_OscC == static_cast<int>(MSEGENV::MSEG5)) {
				envBuffer = routingBuffers.MSEGBuffer[4][mVoiceNo]->getReadPointer(0, startSample);
			}
			if (envBuffer) {
				FloatVectorOperations::multiply(routingBuffers.OscVoices[2][mVoiceNo]->getWritePointer(0, startSample), envBuffer, numSamples); //voice = voice * mseg
				FloatVectorOperations::multiply(routingBuffers.OscVoices[2][mVoiceNo]->getWritePointer(1, startSample), envBuffer, numSamples); //voice = voice * mseg
			}
			routingBuffers.OscBuffer[2]->addFrom(0, startSample, routingBuffers.OscVoices[2][mVoiceNo]->getReadPointer(0, startSample), numSamples);
			routingBuffers.OscBuffer[2]->addFrom(1, startSample, routingBuffers.OscVoices[2][mVoiceNo]->getReadPointer(1, startSample), numSamples);

			//when to reset filter?		
			//m_bqEndLowShelfL[2].processSamples(routingBuffers.OscBuffer[2]->getWritePointer(0, startSample), numSamples);
			//m_bqEndLowShelfR[2].processSamples(routingBuffers.OscBuffer[2]->getWritePointer(1, startSample), numSamples);
		}
		//if (!routingBuffers.OscVoices[3][mVoiceNo]->hasBeenCleared()) {
		if (uNumOscDOscsPlaying > 0) {
			const float* envBuffer = nullptr;
			if (*m_Set->m_State->m_uVCAEnv_OscD == static_cast<int>(MSEGENV::MSEG1)) {
				envBuffer = routingBuffers.MSEGBuffer[0][mVoiceNo]->getReadPointer(0, startSample);
			}
			else if (*m_Set->m_State->m_uVCAEnv_OscD == static_cast<int>(MSEGENV::MSEG2)) {
				envBuffer = routingBuffers.MSEGBuffer[1][mVoiceNo]->getReadPointer(0, startSample);
			}
			else if (*m_Set->m_State->m_uVCAEnv_OscD == static_cast<int>(MSEGENV::MSEG3)) {
				envBuffer = routingBuffers.MSEGBuffer[2][mVoiceNo]->getReadPointer(0, startSample);
			}
			else if (*m_Set->m_State->m_uVCAEnv_OscD == static_cast<int>(MSEGENV::MSEG4)) {
				envBuffer = routingBuffers.MSEGBuffer[3][mVoiceNo]->getReadPointer(0, startSample);
			}
			else if (*m_Set->m_State->m_uVCAEnv_OscD == static_cast<int>(MSEGENV::MSEG5)) {
				envBuffer = routingBuffers.MSEGBuffer[4][mVoiceNo]->getReadPointer(0, startSample);
			}
			if (envBuffer) {
				FloatVectorOperations::multiply(routingBuffers.OscVoices[3][mVoiceNo]->getWritePointer(0, startSample), envBuffer, numSamples); //voice = voice * mseg
				FloatVectorOperations::multiply(routingBuffers.OscVoices[3][mVoiceNo]->getWritePointer(1, startSample), envBuffer, numSamples); //voice = voice * mseg
			}
			routingBuffers.OscBuffer[3]->addFrom(0, startSample, routingBuffers.OscVoices[3][mVoiceNo]->getReadPointer(0, startSample), numSamples);
			routingBuffers.OscBuffer[3]->addFrom(1, startSample, routingBuffers.OscVoices[3][mVoiceNo]->getReadPointer(1, startSample), numSamples);		
			//when to reset filter?		
			//m_bqEndLowShelfL[3].processSamples(routingBuffers.OscBuffer[0]->getWritePointer(0, startSample), numSamples);
			//m_bqEndLowShelfR[3].processSamples(routingBuffers.OscBuffer[0]->getWritePointer(1, startSample), numSamples);
		}
		//if (!routingBuffers.NoiseVoices[mVoiceNo]->hasBeenCleared()) {
		if (uNumOscNoisePlaying > 0) {
			const float* envBuffer = nullptr;;
			if (*m_Set->m_State->m_uVCAEnv_Noise == static_cast<int>(MSEGENV::MSEG1)) {
				envBuffer = routingBuffers.MSEGBuffer[0][mVoiceNo]->getReadPointer(0, startSample);
			}
			else if (*m_Set->m_State->m_uVCAEnv_Noise == static_cast<int>(MSEGENV::MSEG2)) {
				envBuffer = routingBuffers.MSEGBuffer[1][mVoiceNo]->getReadPointer(0, startSample);
			}
			else if (*m_Set->m_State->m_uVCAEnv_Noise == static_cast<int>(MSEGENV::MSEG3)) {
				envBuffer = routingBuffers.MSEGBuffer[2][mVoiceNo]->getReadPointer(0, startSample);
			}
			else if (*m_Set->m_State->m_uVCAEnv_Noise == static_cast<int>(MSEGENV::MSEG4)) {
				envBuffer = routingBuffers.MSEGBuffer[3][mVoiceNo]->getReadPointer(0, startSample);
			}
			else if (*m_Set->m_State->m_uVCAEnv_Noise == static_cast<int>(MSEGENV::MSEG5)) {
				envBuffer = routingBuffers.MSEGBuffer[4][mVoiceNo]->getReadPointer(0, startSample);
			}
			FloatVectorOperations::multiply(routingBuffers.NoiseVoices[mVoiceNo]->getWritePointer(0, startSample), envBuffer, numSamples); //voice = voice * mseg
			FloatVectorOperations::multiply(routingBuffers.NoiseVoices[mVoiceNo]->getWritePointer(1, startSample), envBuffer, numSamples); //voice = voice * mseg			
			routingBuffers.NoiseBuffer->addFrom(0, startSample, routingBuffers.NoiseVoices[mVoiceNo]->getReadPointer(0, startSample), numSamples);
			routingBuffers.NoiseBuffer->addFrom(1, startSample, routingBuffers.NoiseVoices[mVoiceNo]->getReadPointer(1, startSample), numSamples);
		}		
		//if (!routingBuffers.SamplerVoices[mVoiceNo]->hasBeenCleared()) {
		if (*m_Set->m_State->m_bSamplerOnOff == static_cast<int>(SWITCH::SWITCH_ON)) {
			const float* envBuffer = nullptr;
			if (*m_Set->m_State->m_uVCAEnv_Sampler == static_cast<int>(MSEGENV::MSEG1)) {
				envBuffer = routingBuffers.MSEGBuffer[0][mVoiceNo]->getReadPointer(0, startSample);
			}
			else if (*m_Set->m_State->m_uVCAEnv_Sampler == static_cast<int>(MSEGENV::MSEG2)) {
				envBuffer = routingBuffers.MSEGBuffer[1][mVoiceNo]->getReadPointer(0, startSample);
			}
			else if (*m_Set->m_State->m_uVCAEnv_Sampler == static_cast<int>(MSEGENV::MSEG3)) {
				envBuffer = routingBuffers.MSEGBuffer[2][mVoiceNo]->getReadPointer(0, startSample);
			}
			else if (*m_Set->m_State->m_uVCAEnv_Sampler == static_cast<int>(MSEGENV::MSEG4)) {
				envBuffer = routingBuffers.MSEGBuffer[3][mVoiceNo]->getReadPointer(0, startSample);
			}
			else if (*m_Set->m_State->m_uVCAEnv_Sampler == static_cast<int>(MSEGENV::MSEG5)) {
				envBuffer = routingBuffers.MSEGBuffer[4][mVoiceNo]->getReadPointer(0, startSample);
			}
			if (envBuffer) {
				FloatVectorOperations::multiply(routingBuffers.SamplerVoices[mVoiceNo]->getWritePointer(0, startSample), envBuffer, numSamples); //voice = voice * mseg
				FloatVectorOperations::multiply(routingBuffers.SamplerVoices[mVoiceNo]->getWritePointer(1, startSample), envBuffer, numSamples); //voice = voice * mseg
			}
			routingBuffers.SamplerBuffer->addFrom(0, startSample, routingBuffers.SamplerVoices[mVoiceNo]->getReadPointer(0, startSample), numSamples);
			routingBuffers.SamplerBuffer->addFrom(1, startSample, routingBuffers.SamplerVoices[mVoiceNo]->getReadPointer(1, startSample), numSamples);
		}	
	}	
	m_bSmoothersTakeNextValue = false;

	//====================================================================================
}
//====================================================================================

void CVASTSingleNote::writeDebugInfo() {
#ifndef _MACOSX	
	if (m_Set->m_bWriteDebug == true) {
		std::string filename = "C:\\Users\\d026198\\Desktop\\debuginfo.txt";

		std::ofstream debugfile(filename, std::ofstream::out | std::ofstream::app);
		if (debugfile.is_open())
		{
			/*
			debugfile << "m_fOscADivisor ";
			debugfile << m_fOscADivisor;
			debugfile << "  m_fOscAMaxPeak ";
			debugfile << m_fOscAMaxPeak;
			*/
			//debugfile << m_VCA.m_VCA_Envelope.getValue();
			//debugfile << std::endl;
		}
		debugfile.close();
	}
#endif
}
