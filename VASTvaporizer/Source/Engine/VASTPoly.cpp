/*
VAST Dynamics Audio Software (TM)

- Handles and dispatches multiple single notes
. Does ARP
*/

#include "VASTPoly.h"
#include "VASTSettings.h"
#include "VASTSingleNote.h"
#include "VASTEngineHeader.h"
#include "VASTSampler.h"
#include "VASTSynthesiser.h"
#include "../Plugin/VASTAudioProcessor.h"

//for debug info
#include <iostream>
#include <fstream>
#include <string>
CVASTPoly::CVASTPoly(CVASTSettings& set, VASTAudioProcessor* processor) :
	m_Set(&set), myProcessor(processor),
	m_StepSeq_Envelope{ {set, set.m_StepSeqData[0], set.m_StepSeqData_changed[0], 0, 0, 0},
						{set, set.m_StepSeqData[1], set.m_StepSeqData_changed[1], 0, 0, 1},
						{set, set.m_StepSeqData[2], set.m_StepSeqData_changed[2], 0, 0, 2} },
	m_global_LFO_Osc{ {set, nullptr}, {set, nullptr}, {set, nullptr}, {set, nullptr}, {set, nullptr} },
	m_OscBank{ {&set, processor, 0}, {&set, processor, 1}, {&set, processor, 2}, {&set, processor, 3} }
{
	//qfilter  init only once due to memory allocation
	m_QFilter.initQuadFilter(m_Set);
}

/* destructor()
Destroy variables allocated in the contructor()
*/
CVASTPoly::~CVASTPoly(void) {
}

JUCE_BEGIN_IGNORE_WARNINGS_GCC_LIKE("-Wconversion")
JUCE_BEGIN_IGNORE_WARNINGS_MSVC(4244 4267)

void CVASTPoly::init() {
	//executed at startup and whenever maxpoly changes
    if (m_lastInitPoly.load() != m_Set->m_uMaxPoly) {
        m_QFilter.m_bInitFilterAfterMaxPolyChange[0].store(true);
        m_QFilter.m_bInitFilterAfterMaxPolyChange[1].store(true);
        m_QFilter.m_bInitFilterAfterMaxPolyChange[2].store(true);
    }
    
	m_OscillatorSynthesizer.init();
    if (m_lastInitPoly.load() > m_Set->m_uMaxPoly) {
        for (int i = C_MAX_POLY - 1; i >= m_Set->m_uMaxPoly; i--) {
			if (myProcessor->lockedAndSafeToDoDeallocatios()) { //safety check
				m_OscillatorSynthesizer.removeVoice(i);
				m_singleNote[i] = nullptr; //release old voices
				VDBG("CVASTPoly::init Old voice released " << i);
			}
			else {
				myProcessor->setErrorState(myProcessor->vastErrorState::errorState26_maxPolyNotSet); //when this happens, there will be more voices than max poly im synthesizer
				vassertfalse;
			}
        }
    }

    if (m_lastInitPoly.load() < m_Set->m_uMaxPoly) {
        for (int i = m_lastInitPoly.load(); i < m_Set->m_uMaxPoly; i++) {
            m_singleNote[i] = new CVASTSingleNote(*m_Set, this, i); //new is OK - will be stored in owned array as voices
            m_singleNote[i]->init(); //voice->mVoiceNo
            m_OscillatorSynthesizer.addVoice((VASTSynthesiserVoice*)m_singleNote[i]);
        }
    }

	for (int i = 0; i < m_Set->m_uMaxPoly; i++) { //check if things duplicated here
		m_singleNote[i]->prepareForPlay();
	}

	m_global_LFO_Osc[0].init();
	m_global_LFO_Osc[0].updateMainVariables(m_Set->m_nSampleRate, static_cast<int>(*m_Set->m_State->m_uLFOWave_LFO1), 1, 0, 0, 0);

	m_global_LFO_Osc[1].init();
	m_global_LFO_Osc[1].updateMainVariables(m_Set->m_nSampleRate, static_cast<int>(*m_Set->m_State->m_uLFOWave_LFO2), 1, 0, 0, 0);

	m_global_LFO_Osc[2].init();
	m_global_LFO_Osc[2].updateMainVariables(m_Set->m_nSampleRate, static_cast<int>(*m_Set->m_State->m_uLFOWave_LFO3), 1, 0, 0, 0);

	m_global_LFO_Osc[3].init();
	m_global_LFO_Osc[3].updateMainVariables(m_Set->m_nSampleRate, static_cast<int>(*m_Set->m_State->m_uLFOWave_LFO4), 1, 0, 0, 0);

	m_global_LFO_Osc[4].init();
	m_global_LFO_Osc[4].updateMainVariables(m_Set->m_nSampleRate, static_cast<int>(*m_Set->m_State->m_uLFOWave_LFO5), 1, 0, 0, 0);

	updateLFO(0);
	updateLFO(1);
	updateLFO(2);
	updateLFO(3);
	updateLFO(4);

	for (int stepSeq = 0; stepSeq < 3; stepSeq++) {
		if (stepSeq == 0) {
			m_Set->m_StepSeqData[stepSeq].setStepSeqTime(*m_Set->m_State->m_fStepSeqSpeed_STEPSEQ1); //is in ms
			m_Set->m_StepSeqData_changed[stepSeq].setStepSeqTime(*m_Set->m_State->m_fStepSeqSpeed_STEPSEQ1); //is in ms
		} else  if (stepSeq == 1) {
			m_Set->m_StepSeqData[stepSeq].setStepSeqTime(*m_Set->m_State->m_fStepSeqSpeed_STEPSEQ2); //is in ms
			m_Set->m_StepSeqData_changed[stepSeq].setStepSeqTime(*m_Set->m_State->m_fStepSeqSpeed_STEPSEQ2); //is in ms
		} else if (stepSeq == 2) {
			m_Set->m_StepSeqData[stepSeq].setStepSeqTime(*m_Set->m_State->m_fStepSeqSpeed_STEPSEQ3); //is in ms
			m_Set->m_StepSeqData_changed[stepSeq].setStepSeqTime(*m_Set->m_State->m_fStepSeqSpeed_STEPSEQ3); //is in ms
		}

		m_StepSeq_Envelope[stepSeq].init(); //voiceno 0??
		struct timeval tp;
		m_Set->_gettimeofday(&tp);
		m_StepSeq_Envelope[stepSeq].noteOn(static_cast<ULong64_t>(tp.tv_sec) * 1000 + tp.tv_usec / 1000, false);
	}

	const double smoothTime = 0.005;
	m_fCustomModulator1_smoothed.reset(m_Set->m_nSampleRate, smoothTime);
	m_fCustomModulator2_smoothed.reset(m_Set->m_nSampleRate, smoothTime);
	m_fCustomModulator3_smoothed.reset(m_Set->m_nSampleRate, smoothTime);
	m_fCustomModulator4_smoothed.reset(m_Set->m_nSampleRate, smoothTime);
	m_fARP_Speed_smoothed.reset(m_Set->m_nSampleRate, 0.0005);

	//Oscillator voices
	m_OscillatorSynthesizer.setNoteStealingEnabled(true);	
	m_OscillatorSynthesizer.setMinimumRenderingSubdivisionSize(32, false); //not strict
	
	m_shallInitARP = false;
    
    m_lastInitPoly = m_Set->m_uMaxPoly;
}

void CVASTPoly::releaseResources() {
	init();
}

void CVASTPoly::prepareForPlay() {
	//executed multiple
	for (int bank = 0; bank < 4; bank++) {
		m_OscBank[bank].prepareForPlay(m_Set->m_nExpectedSamplesPerBlock);
	}
	for (int i = 0; i < m_Set->m_uMaxPoly; i++) { //check if things duplicated here
		if (m_singleNote[i] != nullptr) { //can happen when thread is updating
			m_singleNote[i]->prepareForPlay();
		}
	}
	
	initArp();
	m_ppq_playing = false;
	
	m_global_LFO_Osc[0].init();
	m_global_LFO_Osc[0].updateMainVariables(m_Set->m_nSampleRate, static_cast<int>(*m_Set->m_State->m_uLFOWave_LFO1), 1, 0, 0, 0);

	m_global_LFO_Osc[1].init();
	m_global_LFO_Osc[1].updateMainVariables(m_Set->m_nSampleRate, static_cast<int>(*m_Set->m_State->m_uLFOWave_LFO2), 1, 0, 0, 0);

	m_global_LFO_Osc[2].init();
	m_global_LFO_Osc[2].updateMainVariables(m_Set->m_nSampleRate, static_cast<int>(*m_Set->m_State->m_uLFOWave_LFO3), 1, 0, 0, 0);

	m_global_LFO_Osc[3].init();
	m_global_LFO_Osc[3].updateMainVariables(m_Set->m_nSampleRate, static_cast<int>(*m_Set->m_State->m_uLFOWave_LFO4), 1, 0, 0, 0);

	m_global_LFO_Osc[4].init();
	m_global_LFO_Osc[4].updateMainVariables(m_Set->m_nSampleRate, static_cast<int>(*m_Set->m_State->m_uLFOWave_LFO5), 1, 0, 0, 0);

	updateLFO(0);
	updateLFO(1);
	updateLFO(2);
	updateLFO(3);
	updateLFO(4);

	//Synthesiser
	m_OscillatorSynthesizer.initValues(); //CHECK TS
	m_OscillatorSynthesizer.setCurrentPlaybackSampleRate(m_Set->m_nSampleRate);

	updateVariables();
	m_QFilter.updateVariables();
}

void CVASTPoly::setKeyboardHoldMode(bool keyboardHoldMode) {
    m_keyboardHoldMode = keyboardHoldMode;
    if (!m_keyboardHoldMode) {
        stopAllNotes(true);
        initArp();
    }
}

void CVASTPoly::initArpInternal(MidiBuffer& midiMessages) {
	for (int note = 0; note < m_ARP_currentARPNoteValues.size(); note++) {
		if (m_ARP_currentARPNoteValues[note] > 0)
		{
			int samplePos = 0;
			//will it be started in that buffer - then no note off! also at first init
			bool willBeStarted = false;
            
            auto midiIterator = midiMessages.findNextSamplePosition (samplePos);
            std::for_each (midiIterator,
                           midiMessages.cend(),
                           [&] (const MidiMessageMetadata& metadata) {
				if ((metadata.getMessage().isNoteOn() && (metadata.getMessage().getNoteNumber() == m_ARP_currentARPNoteValues[note]))) {
                        willBeStarted = true;
                        //break;
                }
            });
			if (!willBeStarted) midiMessages.addEvent(MidiMessage::noteOff(1, m_ARP_currentARPNoteValues[note]), 0);
		}
	}

	m_fARP_Speed_smoothed.reset(m_Set->m_nSampleRate, 0.0005);
	m_ARP_midiInNotes.clear();
	m_ARP_currentARPNoteValues.clear();
	m_ARP_currentStep = 0;
	m_ARP_time = 0;
	m_ARP_direction = 1;
	m_arpHasActiveStepToFinish = false;
	m_shallInitARP = false;	
}

//void CVASTPoly::updateVariables(MYUINT OscType, MYUINT Polarity, float AttackTime, float DecayTime, float SustainLevel, float ReleaseTime, float OscDetune, int NumParallelOsc, float FilterCutoff, float FilterReso, int FilterOnOff, float LFOFreq) {
void CVASTPoly::updateVariables() {
	for (int i = 0; i < m_Set->m_uMaxPoly; i++) {
		if (m_singleNote[i] != nullptr) //can happen when thread is updating and poly parameter is not first
			m_singleNote[i]->updateVariables();
	}	
}

bool CVASTPoly::isVoicePlaying(int voiceNo) const { //UI only
	return m_OscillatorSynthesizer.m_voicePlaying[voiceNo].load();
}

int CVASTPoly::numNotesPlaying() const { //UI only
	return m_OscillatorSynthesizer.m_numVoicesPlaying.load();
}

int CVASTPoly::numOscsPlaying() const { //UI only
	return m_OscillatorSynthesizer.m_numOscsPlaying.load();
}

int CVASTPoly::getLastNotePlayed() const { //-1 if none playing
	return m_OscillatorSynthesizer.getLastPlayedVoiceNo();
}

int CVASTPoly::getOldestNotePlayed() const { //-1 if none playing
	return m_OscillatorSynthesizer.getOldestPlayedVoiceNo();
}

void CVASTPoly::stopAllNotes(bool allowTailOff) {
	for (int i = 0; i < m_Set->m_uMaxPoly; i++) {
        if (m_singleNote[i]!=nullptr) //can happen when thread is updating
            m_singleNote[i]->stopNote(0, allowTailOff);
	}
}

bool CVASTPoly::voicesMSEGStillActive() {
	for (int i = 0; i < m_Set->m_uMaxPoly; i++) {
        if (m_singleNote[i]!=nullptr) //can happen when thread is updating
            if (m_singleNote[i]->m_VCA.isActive()) return true;
	}
	return false;
}

modMatrixInputState CVASTPoly::getOldestNotePlayedInputState(int currentFrame) {
	int voiceNo = getOldestNotePlayed(); // make parameter oldest or newest
	modMatrixInputState inputState{ (voiceNo < 0) ? 0 : voiceNo, currentFrame };
	return inputState;
}

VASTSynthesiser* CVASTPoly::getSynthesizer() { return &m_OscillatorSynthesizer; }

VASTSynthesiserSound* CVASTPoly::getSynthSound() {	
	VASTSynthesiserSound* sound = dynamic_cast<VASTSynthesiserSound*>(getSynthesizer()->getSound(0));
	return sound;
}

VASTSamplerSound* CVASTPoly::getSamplerSound() { //live Data
	VASTSynthesiserSound* sound = dynamic_cast<VASTSynthesiserSound*>(getSynthesizer()->getSound(0));
	if (sound != nullptr)
		return sound->getSamplerSound();
	else 
		return nullptr;
}

VASTSamplerSound* CVASTPoly::getSamplerSoundChanged() {
	VASTSynthesiserSound* sound = dynamic_cast<VASTSynthesiserSound*>(getSynthesizer()->getSound(0));
	if (sound != nullptr)
		return sound->getSamplerSoundChanged();
	else
		return nullptr;
}

void CVASTPoly::clearSamplerSoundChanged() {
	VASTSynthesiserSound* sound = dynamic_cast<VASTSynthesiserSound*>(getSynthesizer()->getSound(0));
	if (sound != nullptr)
		sound->clearSamplerSoundChanged();
}

void CVASTPoly::softFadeExchangeSample() {
	VASTSynthesiserSound* sound = getSynthSound();
	if (sound != nullptr)
		sound->softFadeExchangeSample();
}

bool CVASTPoly::getLastSingleNoteCycleWasActive() {
	return m_iLastSingleNoteCycleCalls > 0;
}

void CVASTPoly::initArp() {
	m_shallInitARP = true;
}

modMatrixInputState CVASTPoly::getLastNotePlayedInputState(int currentFrame) {
	int voiceNo = getLastNotePlayed(); // make parameter oldest or newest
	modMatrixInputState inputState{ (voiceNo < 0) ? 0 : voiceNo , currentFrame };
	return inputState;
}

void CVASTPoly::updateLFO(int lfono) {
	switch (lfono) {
	case 0: {
		if (*m_Set->m_State->m_bLFOSynch_LFO1 == static_cast<int>(SWITCH::SWITCH_ON)) {
			float l_fIntervalTime = m_Set->getIntervalTimeFromDAWBeats(static_cast<int>(*m_Set->m_State->m_uLFOTimeBeats_LFO1));

			if (*m_Set->m_State->m_bLFOPerVoice_LFO1 == static_cast<int>(SWITCH::SWITCH_ON)) {
				for (int i = 0; i < m_Set->m_uMaxPoly; i++) {
					if (m_singleNote[i] != nullptr) { //can happen when thread is updating
						m_singleNote[i]->m_LFO_Osc[0].updateMainVariables(m_Set->m_nSampleRate, static_cast<int>(*m_Set->m_State->m_uLFOWave_LFO1), 1, 0, 0, 0);
						m_singleNote[i]->m_LFO_Osc[0].startLFOFrequency(1000.f / l_fIntervalTime, 0); //lfono 0
					}
				}
			}
			else {
				m_global_LFO_Osc[0].updateMainVariables(m_Set->m_nSampleRate, static_cast<int>(*m_Set->m_State->m_uLFOWave_LFO1), 1, 0, 0, 0);
				m_global_LFO_Osc[0].startLFOFrequency(1000.f / l_fIntervalTime, 0); //lfono 0
			}
		}
		else {
			if (*m_Set->m_State->m_bLFOPerVoice_LFO1 == static_cast<int>(SWITCH::SWITCH_ON)) {
				for (int i = 0; i < m_Set->m_uMaxPoly; i++) {
					if (m_singleNote[i] != nullptr) { //can happen when thread is updating
						m_singleNote[i]->m_LFO_Osc[0].updateMainVariables(m_Set->m_nSampleRate, static_cast<int>(*m_Set->m_State->m_uLFOWave_LFO1), 1, 0, 0, 0);
						m_singleNote[i]->m_LFO_Osc[0].startLFOFrequency(*m_Set->m_State->m_fLFOFreq_LFO1, 0); //lfono 0
					}
				}
			}
			else {
				m_global_LFO_Osc[0].updateMainVariables(m_Set->m_nSampleRate, static_cast<int>(*m_Set->m_State->m_uLFOWave_LFO1), 1, 0, 0, 0);
				m_global_LFO_Osc[0].startLFOFrequency(*m_Set->m_State->m_fLFOFreq_LFO1, 0); //lfono 0
			}
		}
		break;
	}
	case 1: {
		if (*m_Set->m_State->m_bLFOSynch_LFO2 == static_cast<int>(SWITCH::SWITCH_ON)) {
			float l_fIntervalTime = m_Set->getIntervalTimeFromDAWBeats(static_cast<int>(*m_Set->m_State->m_uLFOTimeBeats_LFO2));

			if (*m_Set->m_State->m_bLFOPerVoice_LFO2 == static_cast<int>(SWITCH::SWITCH_ON)) {
				for (int i = 0; i < m_Set->m_uMaxPoly; i++) {
					if (m_singleNote[i] != nullptr) { //can happen when thread is updating
						m_singleNote[i]->m_LFO_Osc[1].updateMainVariables(m_Set->m_nSampleRate, static_cast<int>(*m_Set->m_State->m_uLFOWave_LFO2), 1, 0, 0, 0);
						m_singleNote[i]->m_LFO_Osc[1].startLFOFrequency(1000.f / l_fIntervalTime, 1); //lfono 1
					}
				}
			}
			else {
				m_global_LFO_Osc[1].updateMainVariables(m_Set->m_nSampleRate, static_cast<int>(*m_Set->m_State->m_uLFOWave_LFO2), 1, 0, 0, 0);
				m_global_LFO_Osc[1].startLFOFrequency(1000.f / l_fIntervalTime, 1); //lfono 1
			}
		}
		else {
			if (*m_Set->m_State->m_bLFOPerVoice_LFO2 == static_cast<int>(SWITCH::SWITCH_ON)) {
				for (int i = 0; i < m_Set->m_uMaxPoly; i++) {
					if (m_singleNote[i] != nullptr) { //can happen when thread is updating
						m_singleNote[i]->m_LFO_Osc[1].updateMainVariables(m_Set->m_nSampleRate, static_cast<int>(*m_Set->m_State->m_uLFOWave_LFO2), 1, 0, 0, 0);
						m_singleNote[i]->m_LFO_Osc[1].startLFOFrequency(*m_Set->m_State->m_fLFOFreq_LFO2, 1); //lfono 1
					}
				}
			}
			else {
				m_global_LFO_Osc[1].updateMainVariables(m_Set->m_nSampleRate, static_cast<int>(*m_Set->m_State->m_uLFOWave_LFO2), 1, 0, 0, 0);
				m_global_LFO_Osc[1].startLFOFrequency(*m_Set->m_State->m_fLFOFreq_LFO2, 1); //lfono 1
			}
		}
		break;
	}
	case 2: {
		if (*m_Set->m_State->m_bLFOSynch_LFO3 == static_cast<int>(SWITCH::SWITCH_ON)) {
			float l_fIntervalTime = m_Set->getIntervalTimeFromDAWBeats(static_cast<int>(*m_Set->m_State->m_uLFOTimeBeats_LFO3));

			if (*m_Set->m_State->m_bLFOPerVoice_LFO3 == static_cast<int>(SWITCH::SWITCH_ON)) {
				for (int i = 0; i < m_Set->m_uMaxPoly; i++) {
					if (m_singleNote[i] != nullptr) { //can happen when thread is updating
						m_singleNote[i]->m_LFO_Osc[2].updateMainVariables(m_Set->m_nSampleRate, static_cast<int>(*m_Set->m_State->m_uLFOWave_LFO3), 1, 0, 0, 0);
						m_singleNote[i]->m_LFO_Osc[2].startLFOFrequency(1000.f / l_fIntervalTime, 2); //lfono 2
					}
				}
			}
			else {
				m_global_LFO_Osc[2].updateMainVariables(m_Set->m_nSampleRate, static_cast<int>(*m_Set->m_State->m_uLFOWave_LFO3), 1, 0, 0, 0);
				m_global_LFO_Osc[2].startLFOFrequency(1000.f / l_fIntervalTime, 2); //lfono 2
			}
		}
		else {
			if (*m_Set->m_State->m_bLFOPerVoice_LFO3 == static_cast<int>(SWITCH::SWITCH_ON)) {
				for (int i = 0; i < m_Set->m_uMaxPoly; i++) {
					if (m_singleNote[i] != nullptr) { //can happen when thread is updating
						m_singleNote[i]->m_LFO_Osc[2].updateMainVariables(m_Set->m_nSampleRate, static_cast<int>(*m_Set->m_State->m_uLFOWave_LFO3), 1, 0, 0, 0);
						m_singleNote[i]->m_LFO_Osc[2].startLFOFrequency(*m_Set->m_State->m_fLFOFreq_LFO3, 2); //lfono 2
					}
				}
			}
			else {
				m_global_LFO_Osc[2].updateMainVariables(m_Set->m_nSampleRate, static_cast<int>(*m_Set->m_State->m_uLFOWave_LFO3), 1, 0, 0, 0);
				m_global_LFO_Osc[2].startLFOFrequency(*m_Set->m_State->m_fLFOFreq_LFO3, 2); //lfono 2
			}
		}
		break;
	}
	case 3: {
		if (*m_Set->m_State->m_bLFOSynch_LFO4 == static_cast<int>(SWITCH::SWITCH_ON)) {
			float l_fIntervalTime = m_Set->getIntervalTimeFromDAWBeats(static_cast<int>(*m_Set->m_State->m_uLFOTimeBeats_LFO4));

			if (*m_Set->m_State->m_bLFOPerVoice_LFO4 == static_cast<int>(SWITCH::SWITCH_ON)) {
				for (int i = 0; i < m_Set->m_uMaxPoly; i++) {
					if (m_singleNote[i] != nullptr) { //can happen when thread is updating
						m_singleNote[i]->m_LFO_Osc[3].updateMainVariables(m_Set->m_nSampleRate, static_cast<int>(*m_Set->m_State->m_uLFOWave_LFO4), 1, 0, 0, 0);
						m_singleNote[i]->m_LFO_Osc[3].startLFOFrequency(1000.f / l_fIntervalTime, 3); //lfono 3
					}
				}
			}
			else {
				m_global_LFO_Osc[3].updateMainVariables(m_Set->m_nSampleRate, static_cast<int>(*m_Set->m_State->m_uLFOWave_LFO4), 1, 0, 0, 0);
				m_global_LFO_Osc[3].startLFOFrequency(1000.f / l_fIntervalTime, 2); //lfono 2
			}
		}
		else {
			if (*m_Set->m_State->m_bLFOPerVoice_LFO4 == static_cast<int>(SWITCH::SWITCH_ON)) {
				for (int i = 0; i < m_Set->m_uMaxPoly; i++) {
					if (m_singleNote[i] != nullptr) { //can happen when thread is updating
						m_singleNote[i]->m_LFO_Osc[3].updateMainVariables(m_Set->m_nSampleRate, static_cast<int>(*m_Set->m_State->m_uLFOWave_LFO4), 1, 0, 0, 0);
						m_singleNote[i]->m_LFO_Osc[3].startLFOFrequency(*m_Set->m_State->m_fLFOFreq_LFO4, 3); //lfono 2
					}
				}
			}
			else {
				m_global_LFO_Osc[3].updateMainVariables(m_Set->m_nSampleRate, static_cast<int>(*m_Set->m_State->m_uLFOWave_LFO4), 1, 0, 0, 0);
				m_global_LFO_Osc[3].startLFOFrequency(*m_Set->m_State->m_fLFOFreq_LFO4, 3); //lfono 3
			}
		}
		break;
	}
	case 4: {
		if (*m_Set->m_State->m_bLFOSynch_LFO5 == static_cast<int>(SWITCH::SWITCH_ON)) {
			float l_fIntervalTime = m_Set->getIntervalTimeFromDAWBeats(static_cast<int>(*m_Set->m_State->m_uLFOTimeBeats_LFO5));

			if (*m_Set->m_State->m_bLFOPerVoice_LFO5 == static_cast<int>(SWITCH::SWITCH_ON)) {
				for (int i = 0; i < m_Set->m_uMaxPoly; i++) {
					if (m_singleNote[i] != nullptr) { //can happen when thread is updating
						m_singleNote[i]->m_LFO_Osc[4].updateMainVariables(m_Set->m_nSampleRate, static_cast<int>(*m_Set->m_State->m_uLFOWave_LFO5), 1, 0, 0, 0);
						m_singleNote[i]->m_LFO_Osc[4].startLFOFrequency(1000.f / l_fIntervalTime, 4); //lfono 4
					}
				}
			}
			else {
				m_global_LFO_Osc[4].updateMainVariables(m_Set->m_nSampleRate, static_cast<int>(*m_Set->m_State->m_uLFOWave_LFO5), 1, 0, 0, 0);
				m_global_LFO_Osc[4].startLFOFrequency(1000.f / l_fIntervalTime, 4); //lfono 4
			}
		}
		else {
			if (*m_Set->m_State->m_bLFOPerVoice_LFO5 == static_cast<int>(SWITCH::SWITCH_ON)) {
				for (int i = 0; i < m_Set->m_uMaxPoly; i++) {
					if (m_singleNote[i] != nullptr) { //can happen when thread is updating
						m_singleNote[i]->m_LFO_Osc[4].updateMainVariables(m_Set->m_nSampleRate, static_cast<int>(*m_Set->m_State->m_uLFOWave_LFO5), 1, 0, 0, 0);
						m_singleNote[i]->m_LFO_Osc[4].startLFOFrequency(*m_Set->m_State->m_fLFOFreq_LFO5, 2); //lfono 4
					}
				}
			}
			else {
				m_global_LFO_Osc[4].updateMainVariables(m_Set->m_nSampleRate, static_cast<int>(*m_Set->m_State->m_uLFOWave_LFO5), 1, 0, 0, 0);
				m_global_LFO_Osc[4].startLFOFrequency(*m_Set->m_State->m_fLFOFreq_LFO5, 4); //lfono 4
			}
		}
		break;
	}
	}
}

//called on noteOn
void CVASTPoly::resynchLFO() {
	#define FREERUN_LFO_RETRIGGER_RESET 1000 //1s

	if (*m_Set->m_State->m_bLFORetrigOnOff_LFO1 == static_cast<int>(SWITCH::SWITCH_ON)) {
		if (!(*m_Set->m_State->m_bLFOPerVoice_LFO1 == static_cast<int>(SWITCH::SWITCH_ON))) {
			m_global_LFO_Osc[0].resynchWithFade(true);
		}
	}
	else { //freerunning - retrigger when last note older than 1s?
		int voiceNo = getLastNotePlayed(); // make parameter oldest or newest		
		if (voiceNo >= 0) {
			CVASTSingleNote* note = m_singleNote[voiceNo];
			if (note->isKeyDown() == false) {
				struct timeval tp;
				m_Set->_gettimeofday(&tp);
				int diff = (static_cast<unsigned long long>(tp.tv_sec) * 1000 + tp.tv_usec / 1000) - note->m_startPlayTimestamp;
				if (diff > FREERUN_LFO_RETRIGGER_RESET) {
					m_global_LFO_Osc[0].resynchWithFade(true);
				}
			}
		}
	}

	if (*m_Set->m_State->m_bLFORetrigOnOff_LFO2 == static_cast<int>(SWITCH::SWITCH_ON)) {
		if (!(*m_Set->m_State->m_bLFOPerVoice_LFO2 == static_cast<int>(SWITCH::SWITCH_ON))) {
			m_global_LFO_Osc[1].resynchWithFade(true);
		}
	}
	else { //freerunning - retrigger when last note older than 1s?
		int voiceNo = getLastNotePlayed(); // make parameter oldest or newest		
		if (voiceNo >= 0) {
			CVASTSingleNote* note = m_singleNote[voiceNo];
			if (note->isKeyDown() == false) {
				struct timeval tp;
				m_Set->_gettimeofday(&tp);
				int diff = (static_cast<unsigned long long>(tp.tv_sec) * 1000 + tp.tv_usec / 1000) - note->m_startPlayTimestamp;
				if (diff > FREERUN_LFO_RETRIGGER_RESET) {
					m_global_LFO_Osc[1].resynchWithFade(true);
				}
			}
		}
	}

	if (*m_Set->m_State->m_bLFORetrigOnOff_LFO3 == static_cast<int>(SWITCH::SWITCH_ON)) {
		if (!(*m_Set->m_State->m_bLFOPerVoice_LFO3 == static_cast<int>(SWITCH::SWITCH_ON))) {
			m_global_LFO_Osc[2].resynchWithFade(true);
		}
	}
	else { //freerunning - retrigger when last note older than 1s?
		int voiceNo = getLastNotePlayed(); // make parameter oldest or newest		
		if (voiceNo >= 0) {
			CVASTSingleNote* note = m_singleNote[voiceNo];
			if (note->isKeyDown() == false) {
				struct timeval tp;
				m_Set->_gettimeofday(&tp);
				int diff = (static_cast<unsigned long long>(tp.tv_sec) * 1000 + tp.tv_usec / 1000) - note->m_startPlayTimestamp;
				if (diff > FREERUN_LFO_RETRIGGER_RESET) {
					m_global_LFO_Osc[2].resynchWithFade(true);
				}
			}
		}
	}

	if (*m_Set->m_State->m_bLFORetrigOnOff_LFO4 == static_cast<int>(SWITCH::SWITCH_ON)) {
		if (!(*m_Set->m_State->m_bLFOPerVoice_LFO4 == static_cast<int>(SWITCH::SWITCH_ON))) {
			m_global_LFO_Osc[3].resynchWithFade(true);
		}
	}
	else { //freerunning - retrigger when last note older than 1s?
		int voiceNo = getLastNotePlayed(); // make parameter oldest or newest		
		CVASTSingleNote* note = m_singleNote[voiceNo];
		if (voiceNo >= 0) {
			if (note->isKeyDown() == false) {
				struct timeval tp;
				m_Set->_gettimeofday(&tp);
				int diff = (static_cast<unsigned long long>(tp.tv_sec) * 1000 + tp.tv_usec / 1000) - note->m_startPlayTimestamp;
				if (diff > FREERUN_LFO_RETRIGGER_RESET) {
					m_global_LFO_Osc[3].resynchWithFade(true);
				}
			}
		}
	}

	if (*m_Set->m_State->m_bLFORetrigOnOff_LFO5 == static_cast<int>(SWITCH::SWITCH_ON)) {
		if (!(*m_Set->m_State->m_bLFOPerVoice_LFO5 == static_cast<int>(SWITCH::SWITCH_ON))) {
			m_global_LFO_Osc[4].resynchWithFade(true);
		}
	}
	else { //freerunning - retrigger when last note older than 1s?
		int voiceNo = getLastNotePlayed(); // make parameter oldest or newest		
		CVASTSingleNote* note = m_singleNote[voiceNo];
		if (voiceNo >= 0) {
			if (note->isKeyDown() == false) {
				struct timeval tp;
				m_Set->_gettimeofday(&tp);
				int diff = (static_cast<unsigned long long>(tp.tv_sec) * 1000 + tp.tv_usec / 1000) - note->m_startPlayTimestamp;
				if (diff > FREERUN_LFO_RETRIGGER_RESET) {
					m_global_LFO_Osc[4].resynchWithFade(true);
				}
			}
		}
	}
}

void CVASTPoly::doArp(sRoutingBuffers& routingBuffers, MidiBuffer& midiMessages) {
	VASTARPData* arpData = &m_Set->m_ARPData;
	//const ScopedReadLock myScopedLock(arpData->mReadWriteLock); //this seemed to cause an issue
	int numSteps = arpData->getNumSteps();
	auto numSamples = routingBuffers.getNumSamples();
	arpData->setDispActiveStep(m_ARP_currentStep); 
	bool bStart = false;

	if (!m_arpHasActiveStepToFinish) {
		if (m_ARP_midiInNotes.size() == 0) {
			m_ARP_currentStep = 0;
			arpData->setDispActiveStep(0);
			bStart = true;
		}
	}
	if (m_ARP_currentStep > (numSteps - 1))
		m_ARP_currentStep = 0;

	// get step duration
	double stepDuration = 0;
	if ((*m_Set->m_State->m_bARPSynch == static_cast<int>(SWITCH::SWITCH_ON)) && (m_Set->m_bPpqIsPlaying)) {
		if (m_last_bpm != m_Set->m_dPpqBpm) { //bpm was changed??
			bStart = true;
		}
		m_last_bpm = m_Set->m_dPpqBpm; 
		double l_fIntervalTime = m_Set->getIntervalTimeFromDAWBeats(*m_Set->m_State->m_uARPTimeBeats);
		stepDuration = m_Set->m_nSampleRate * (l_fIntervalTime * 0.001);  //interval time is in seconds
		if (bStart)
			m_ARP_time = stepDuration; //ARP time can only increase value!   
	}
	else {
		if (*m_Set->m_State->m_bARPSynch == static_cast<int>(SWITCH::SWITCH_ON)) { //synch but not playing
			double l_fIntervalTime = m_Set->getIntervalTimeFromDAWBeats(*m_Set->m_State->m_uARPTimeBeats);
			m_fARP_Speed_smoothed.setCurrentAndTargetValue(l_fIntervalTime);
		}

		double speed = m_fARP_Speed_smoothed.getNextValue();
		if (speed == 0.0) {
			m_fARP_Speed_smoothed.setCurrentAndTargetValue(*m_Set->m_State->m_fARPSpeed);
			speed = m_fARP_Speed_smoothed.getNextValue();
		}
		stepDuration = m_Set->m_nSampleRate * (speed * 0.001);
		if (bStart) 
			m_ARP_time = stepDuration; //ARP time can only increase value!
	}

	int lChannelForARP = 1;
	if (getSynthesizer()->m_MPEMasterChannel == lChannelForARP)
		lChannelForARP++;

	int samplePos = 0;
	MidiBuffer mb;
	mb.clear();

	Array<int> l_noteOffNotes;
    
    auto midiIterator = midiMessages.findNextSamplePosition (samplePos);
    std::for_each (midiIterator,
                   midiMessages.cend(),
                   [&] (const MidiMessageMetadata& metadata) {
        if (metadata.getMessage().isNoteOn()) {
            m_ARP_midiInNotes.add(metadata.getMessage().getNoteNumber());
        }
        else if (metadata.getMessage().isNoteOff()) {
            if (!m_ARP_midiInNotes.contains(metadata.getMessage().getNoteNumber())) //some leftovers
                mb.addEvent(metadata.getMessage(), 0);
            
            m_ARP_midiInNotes.removeValue(metadata.getMessage().getNoteNumber());
            l_noteOffNotes.add(metadata.getMessage().getNoteNumber());
        }
        else {
            mb.addEvent(metadata.getMessage(), samplePos);
        }
    });

	midiMessages.clear();
	midiMessages.addEvents(mb, 0, numSamples, 0);

	double realPos = m_Set->m_dPpqPosition / m_Set->getIntervalRatio(*m_Set->m_State->m_uARPTimeBeats); //only for synch
	double realPosEndOfBuffer = realPos + (numSamples - 1) / stepDuration;

	bool bStartNote = false;
	bool bStopNote = false;
	if (m_ARP_time < 0) { //when gui changes are done while paying, or for full bar wrap
		bStartNote = true;
		bStopNote = true;
		m_ARP_time = 0;
	}

	if (bStart)
		bStartNote = true; //always!
	else {
		if ((m_ARP_time + numSamples - 1) >= stepDuration) { //wrap in this buffer
			bStartNote = true;
			m_ARP_currentStep++;
			m_ARP_currentStep %= numSteps;
		}
	}

	//noteOff shall be send when time passes behind length of last note - and it is not hold
	bool isHold = false;
	int stepBefore = (m_ARP_currentStep > 0) ? m_ARP_currentStep - 1 : numSteps - 1;
	VASTARPData::ArpStep curStepData = arpData->getStep(m_ARP_currentStep);
	VASTARPData::ArpStep stepBeforeData = arpData->getStep(stepBefore);
	if ((m_ARP_time + numSamples - 1) >= (curStepData.gate * 0.25f) * stepDuration)
		bStopNote = true;

	if (bStopNote)
	{
		m_arpHasActiveStepToFinish = false;
		if ((stepBeforeData.gate == 4) &&
			(curStepData.semitones == stepBeforeData.semitones) &&
			(curStepData.octave == stepBeforeData.octave)) // this is hold
			isHold = true;
		if (curStepData.gate == 0) 
			isHold = false; //no hold when current note is 0
		if (stepBefore == m_ARP_currentStep) 
			isHold = false; //only one step and hold?
		bool allHold = true;
		for (int i = 0; i < arpData->getNumSteps(); i++) {
			VASTARPData::ArpStep thisStepData = arpData->getStep(i);
			if (thisStepData.gate != 4) allHold = false;
		}
		if (allHold && (stepBefore == (arpData->getNumSteps() - 1))) isHold = false; //no hold on last note when all is hold
		if (bStart) isHold = false; //when nothing is pressed

		if (!isHold) { //end all notes playing
			for (int note = 0; note < m_ARP_currentARPNoteValues.size(); note++) {
				if (m_ARP_currentARPNoteValues[note] > 0)
				{
					auto offset = jmax(0, jmin((int)((stepBeforeData.gate * 0.25f) * stepDuration - m_ARP_time), numSamples - 1));
					midiMessages.addEvent(MidiMessage::noteOff(lChannelForARP, m_ARP_currentARPNoteValues[note]), offset);
				}
			}
			m_ARP_currentARPNoteValues.clear();
		}
	}
	bool bTimeSet = false;
	if (bStartNote)
	{
		if ((m_ARP_midiInNotes.size() > 0) && (!isHold))
		{
			m_arpHasActiveStepToFinish = true;
			auto offset = jmax(0, jmin((int)(stepDuration - m_ARP_time), numSamples - 1));
			//collect what needs to be triggered
			m_ARP_currentARPNoteValues.clear();
			if (*m_Set->m_State->m_uARPMode == static_cast<int>(ARPMODE::POLY)) {
				for (int note = 0; note < m_ARP_midiInNotes.size(); note++) {
					m_ARP_currentARPNoteValues.add(m_ARP_midiInNotes[note]);
				}
			}
			else if (*m_Set->m_State->m_uARPMode == static_cast<int>(ARPMODE::UP)) {
				m_ARP_direction = +1;

				if (m_ARP_currentStep == 0) {
					m_ARP_currentNote = (m_ARP_midiInNotes.size() + m_ARP_currentNote + m_ARP_direction) % m_ARP_midiInNotes.size();
				}
				else {
					if (m_ARP_currentNote >= m_ARP_midiInNotes.size())
						m_ARP_currentNote--;
				}
				m_ARP_currentARPNoteValues.add(m_ARP_midiInNotes[m_ARP_currentNote]);
			}
			else if (*m_Set->m_State->m_uARPMode == static_cast<int>(ARPMODE::DOWN)) {
				m_ARP_direction = -1;
				if (m_ARP_currentStep == 0) {
					m_ARP_currentNote = (m_ARP_midiInNotes.size() + m_ARP_currentNote + m_ARP_direction) % m_ARP_midiInNotes.size();
				}
				else {
					if (m_ARP_currentNote >= m_ARP_midiInNotes.size())
						m_ARP_currentNote--;
				}
				m_ARP_currentARPNoteValues.add(m_ARP_midiInNotes[m_ARP_currentNote]);
			}
			else if (*m_Set->m_State->m_uARPMode == static_cast<int>(ARPMODE::UPDOWN)) {
				if (m_ARP_currentStep == 0) {
					m_ARP_currentNote = (m_ARP_midiInNotes.size() + m_ARP_currentNote + m_ARP_direction) % m_ARP_midiInNotes.size();
					if ((m_ARP_direction == 1) && (m_ARP_currentNote >= m_ARP_midiInNotes.size() - 1))
						m_ARP_direction *= -1;
					if ((m_ARP_direction == -1) && (m_ARP_currentNote <= 0))
						m_ARP_direction *= -1;

				}
				else {
					if (m_ARP_currentNote >= m_ARP_midiInNotes.size())
						m_ARP_currentNote--;
				}
				m_ARP_currentARPNoteValues.add(m_ARP_midiInNotes[m_ARP_currentNote]);
			}

			//trigger them all
			for (int note = 0; note < m_ARP_currentARPNoteValues.size(); note++) {
				VASTARPData::ArpStep curStepData = arpData->getStep(m_ARP_currentStep);
				float curVelocity = static_cast<int>(curStepData.velocity);
				int newNote = m_ARP_currentARPNoteValues[note] + curStepData.octave * 12 + curStepData.semitones;

				if ((curStepData.gate > 0) && (newNote > 0) && (newNote < 127)) {
					midiMessages.addEvent(MidiMessage::noteOn(lChannelForARP, newNote, (uint8)curVelocity), offset);
					m_ARP_currentARPNoteValues.getReference(note) = newNote;
				}
				else {
					m_ARP_currentARPNoteValues.getReference(note) = -1;
				}
			}

			m_ARP_time = -offset + numSamples; //0-numSamples
			bTimeSet = true;
		}	
	}

	if (!bTimeSet) {
		while ((m_ARP_time + numSamples - 1) >= stepDuration)
			m_ARP_time -= stepDuration; //wrap

		m_ARP_time = (m_ARP_time + numSamples);
	}
	m_dLastRealPos = realPosEndOfBuffer;
}

//===========================================================================================

void CVASTPoly::processAudioBuffer(sRoutingBuffers& routingBuffers, MidiBuffer& midiMessages) {
	
	m_iLastSingleNoteCycleCalls = 0; //for single not softfade cycle 

	//Clear UI atomics
	m_samplerViewportPosMarkerCount = 0;
	std::fill_n(&m_samplerViewportPosMarker[0], sizeof(m_samplerViewportPosMarker) / sizeof(m_samplerViewportPosMarker[0]), 0.0);
	std::fill_n(&m_currentWTPosFloatPercentage[0][0], sizeof(m_currentWTPosFloatPercentage) / sizeof(m_currentWTPosFloatPercentage[0][0]), 0.f);
	std::fill_n(&m_safePhaseFloat[0][0], sizeof(m_safePhaseFloat) / sizeof(m_safePhaseFloat[0][0]), 0.f);
	//std::fill_n(&m_fLastLFOOscValue[0][0], sizeof(m_fLastLFOOscValue) / sizeof(m_fLastLFOOscValue[0][0]), 0.f);
	//std::fill_n(&m_fLastGlobalLFOOscValue[0], sizeof(m_fLastGlobalLFOOscValue) / sizeof(m_fLastGlobalLFOOscValue[0]), 0.f);
	//Clear UI atomics

	//===========================================================================================
	//Check for StepSeq restart
	if (m_ppq_playing != m_Set->m_bPpqIsPlaying) {	//status changed
		if (m_Set->m_bPpqIsPlaying) { //now just started
			if (*m_Set->m_State->m_bARPSynch == static_cast<int>(SWITCH::SWITCH_ON))
				initArp();

			struct timeval tp;
			m_Set->_gettimeofday(&tp);
			if (*m_Set->m_State->m_bStepSeqSynch_STEPSEQ1 == static_cast<int>(SWITCH::SWITCH_ON)) {
				m_StepSeq_Envelope[0].noteOff();
			}
			if (*m_Set->m_State->m_bStepSeqSynch_STEPSEQ2 == static_cast<int>(SWITCH::SWITCH_ON)) {
				m_StepSeq_Envelope[1].noteOff();
			}
			if (*m_Set->m_State->m_bStepSeqSynch_STEPSEQ3 == static_cast<int>(SWITCH::SWITCH_ON)) {
				m_StepSeq_Envelope[2].noteOff();
			}

			//struct timeval tp;
			//m_Set->_gettimeofday(&tp);
			if (*m_Set->m_State->m_bStepSeqSynch_STEPSEQ1 == static_cast<int>(SWITCH::SWITCH_ON)) {
				m_StepSeq_Envelope[0].noteOn(static_cast<ULong64_t>(tp.tv_sec) * 1000 + tp.tv_usec / 1000, false);
			}
			if (*m_Set->m_State->m_bStepSeqSynch_STEPSEQ2 == static_cast<int>(SWITCH::SWITCH_ON)) {
				m_StepSeq_Envelope[1].noteOn(static_cast<ULong64_t>(tp.tv_sec) * 1000 + tp.tv_usec / 1000, false);
			}
			if (*m_Set->m_State->m_bStepSeqSynch_STEPSEQ3 == static_cast<int>(SWITCH::SWITCH_ON)) {
				m_StepSeq_Envelope[2].noteOn(static_cast<ULong64_t>(tp.tv_sec) * 1000 + tp.tv_usec / 1000, false);
			}

			if (*m_Set->m_State->m_bStepSeqSynch_STEPSEQ1 == static_cast<int>(SWITCH::SWITCH_ON)) {
				if (m_Set->m_dPpqBpm != 0.f) {
					float l_fIntervalTime = m_Set->getIntervalTimeFromDAWBeats(static_cast<int>(*m_Set->m_State->m_uStepSeqTimeBeats_STEPSEQ1));
					m_Set->m_StepSeqData[0].setStepSeqTime(l_fIntervalTime); //is in ms
				}
			}
			if (*m_Set->m_State->m_bStepSeqSynch_STEPSEQ2 == static_cast<int>(SWITCH::SWITCH_ON)) {
				if (m_Set->m_dPpqBpm != 0.f) {
					float l_fIntervalTime = m_Set->getIntervalTimeFromDAWBeats(static_cast<int>(*m_Set->m_State->m_uStepSeqTimeBeats_STEPSEQ2));
					m_Set->m_StepSeqData[1].setStepSeqTime(l_fIntervalTime); //is in ms
				}
			}
			if (*m_Set->m_State->m_bStepSeqSynch_STEPSEQ3 == static_cast<int>(SWITCH::SWITCH_ON)) {
				if (m_Set->m_dPpqBpm != 0.f) {
					float l_fIntervalTime = m_Set->getIntervalTimeFromDAWBeats(static_cast<int>(*m_Set->m_State->m_uStepSeqTimeBeats_STEPSEQ3));
					m_Set->m_StepSeqData[2].setStepSeqTime(l_fIntervalTime); //is in ms
				}
			}

		}
		else { //now just stopped			
			if (*m_Set->m_State->m_bARPSynch == static_cast<int>(SWITCH::SWITCH_ON))
				initArp();
			
			//do nothing
			/*
			if (*m_Set->m_State->m_bStepSeqSynch_STEPSEQ1 == static_cast<int>(SWITCH::SWITCH_ON))
				m_Set->m_StepSeqData[0].setStepSeqTime(*m_Set->m_State->m_fStepSeqSpeed_STEPSEQ1); //is in ms
			if (*m_Set->m_State->m_bStepSeqSynch_STEPSEQ2 == static_cast<int>(SWITCH::SWITCH_ON))
				m_Set->m_StepSeqData[1].setStepSeqTime(*m_Set->m_State->m_fStepSeqSpeed_STEPSEQ2); //is in ms
			if (*m_Set->m_State->m_bStepSeqSynch_STEPSEQ3 == static_cast<int>(SWITCH::SWITCH_ON))
				m_Set->m_StepSeqData[2].setStepSeqTime(*m_Set->m_State->m_fStepSeqSpeed_STEPSEQ3); //is in ms
			*/

		}
		m_ppq_playing = !m_ppq_playing;
	}

	//===========================================================================================
	//Check ARP
	
    if (m_keyboardHoldMode) { //delete noteOff events
        MidiBuffer newBuffer;
        int samplePos = 0;
        auto midiIterator = midiMessages.findNextSamplePosition (samplePos);
        std::for_each (midiIterator,
                       midiMessages.cend(),
                       [&] (const MidiMessageMetadata& metadata) {
            if (!metadata.getMessage().isNoteOff()) {
                newBuffer.addEvent(metadata.getMessage(), metadata.samplePosition);
            }
        });
        midiMessages.swapWith(newBuffer);
    }
    
    if (m_shallInitARP) initArpInternal(midiMessages);
	if (*m_Set->m_State->m_bARPOnOff == static_cast<int>(SWITCH::SWITCH_ON)) {
		doArp(routingBuffers, midiMessages);
	}
	else {
		m_Set->m_ARPData.setDispActiveStep(-1);
		int samplePos = 0;
        auto midiIterator = midiMessages.findNextSamplePosition (samplePos);
        std::for_each (midiIterator,
                       midiMessages.cend(),
                       [&] (const MidiMessageMetadata& metadata) {
            if (metadata.getMessage().isNoteOn()) {
                m_ARP_currentARPNoteValues.add(metadata.getMessage().getNoteNumber());
            }
        });
	}

	//===========================================================================================
	//oscillator voices

	m_OscillatorSynthesizer.renderNextBlock(routingBuffers, midiMessages, 0, routingBuffers.getNumSamples());

	//ARP speed mod
	modMatrixInputState inputState = getOldestNotePlayedInputState(0); // make parameter oldest or newest
	m_fARP_Speed_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fARPSpeed, MODMATDEST::ArpSpeed, &inputState));

	//===========================================================================================
	// attenuate
	//float lGain = 0.1875f; // float(1.0 / m_Set->m_uMaxPoly) * 3.0f; //attenuate  poly notes by maximum number to have the same gain for all
	float lGain = 0.27f; // float(1.0 / m_Set->m_uMaxPoly) * 3.0f; //attenuate  poly notes by maximum number to have the same gain for all
	routingBuffers.OscBuffer[0]->applyGain(lGain);
	routingBuffers.OscBuffer[1]->applyGain(lGain);
	routingBuffers.OscBuffer[2]->applyGain(lGain);
	routingBuffers.OscBuffer[3]->applyGain(lGain);	
	routingBuffers.SamplerBuffer->applyGain(lGain);
	routingBuffers.FilterBuffer[0]->applyGain(lGain);
	routingBuffers.FilterBuffer[1]->applyGain(lGain);
	routingBuffers.FilterBuffer[2]->applyGain(lGain);
	routingBuffers.NoiseBuffer->applyGain(lGain);
}

JUCE_END_IGNORE_WARNINGS_MSVC
JUCE_END_IGNORE_WARNINGS_GCC_LIKE
