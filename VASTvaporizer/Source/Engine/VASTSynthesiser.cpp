/*
VAST Dynamics Audio Software (TM)
*/

#include "VASTSynthesiser.h"
#include "VASTEngineHeader.h"
#include "VASTSettings.h"
#include "VASTPluginConstants.h"
#include "VASTSingleNote.h"
#include "VASTPoly.h"
#include "../Plugin/VASTAudioProcessor.h"

//==============================================================================
VASTSynthesiserVoice::VASTSynthesiserVoice() {}
VASTSynthesiserVoice::~VASTSynthesiserVoice() {}

bool VASTSynthesiserVoice::isPlayingChannel(const int midiChannel) const
{
	return currentPlayingMidiChannel == midiChannel; 
}

void VASTSynthesiserVoice::setCurrentPlaybackSampleRate(const double newRate)
{
	currentSampleRate = newRate;
}

bool VASTSynthesiserVoice::isVoiceActive() const
{
	return getCurrentlyPlayingNote() >= 0;
}

bool VASTSynthesiserVoice::wasStartedBefore(const VASTSynthesiserVoice& other) const noexcept
{
	return noteOnTime < other.noteOnTime;
}


//==============================================================================
VASTSynthesiser::VASTSynthesiser(VASTAudioProcessor* processor) : myProcessor(processor)
{
	m_Set = &processor->m_pVASTXperience.m_Set;
	m_Poly = &processor->m_pVASTXperience.m_Poly;
	clearVoices();
	clearSounds();
	VASTSynthesiserSound* l_vastSound = new VASTSynthesiserSound();
	addSound(l_vastSound);
	initValues();
}

VASTSynthesiser::~VASTSynthesiser()
{
}

void VASTSynthesiser::initValues() {
	for (int i = 0; i < numElementsInArray(lastPitchWheelValues); ++i)
		lastPitchWheelValues[i] = 0x2000;
	lastPitchWheelUIValue = 0x2000;

	for (int i = 0; i < numElementsInArray(m_midiNotesKeyDown); ++i) {
		m_midiNotesKeyDown[i] = false;
		m_midiNotesKeyDownTime[i] = 0;
		m_oldChordStack[i] = false;
		m_newChordStack[i] = false;
	}

	m_midiNotesNumKeyDown = 0;
	m_bGlissandoUsed = false;
	lastNoteOnCounter = 0;
	m_newestPlaying = -1;
	m_oldestPlaying = -1;
	m_oldestPlayingKeyDown = -1;
	m_iGlissandoAssigned = 0;
	m_iGlissandoCollectSamples = 0;

	const uint8 noLSBValueReceived = 0xff;
	std::fill_n(lastPressureLowerBitReceivedOnChannel, 16, noLSBValueReceived);

	for (int midiChannel = 0; midiChannel < 16; midiChannel++) { //16 midi channels
		m_fPitchBendZone_smoothed[midiChannel].reset(m_Set->m_nSampleRate, 0.1f);
		m_fModWheel_smoothed[midiChannel].reset(m_Set->m_nSampleRate, 0.1f);
	}
}

//==============================================================================
VASTSynthesiserVoice* VASTSynthesiser::getVoice(const int index) const
{
	const ScopedLock sl(lock);
	return voices[index];
}

void VASTSynthesiser::clearVoices()
{
	const ScopedLock sl(lock);
	voices.clear();
}

VASTSynthesiserVoice* VASTSynthesiser::addVoice(VASTSynthesiserVoice* const newVoice)
{
	const ScopedLock sl(lock);
	newVoice->setCurrentPlaybackSampleRate(sampleRate);
	return voices.add(newVoice);
}

void VASTSynthesiser::removeVoice(const int index)
{
	const ScopedLock sl(lock);
	voices.remove(index);
}

void VASTSynthesiser::clearSounds()
{
	const ScopedLock sl(lock);
	sounds.clear();
}

juce::SynthesiserSound* VASTSynthesiser::addSound(const juce::SynthesiserSound::Ptr& newSound)
{
	const ScopedLock sl(lock);
	return sounds.add(newSound);
}

void VASTSynthesiser::removeSound(const int index)
{
	const ScopedLock sl(lock);
	sounds.remove(index);
}

void VASTSynthesiser::setNoteStealingEnabled(const bool shouldSteal)
{
	shouldStealNotes = shouldSteal;
}

void VASTSynthesiser::setMinimumRenderingSubdivisionSize(int numSamples, bool shouldBeStrict) noexcept
{
	jassert(numSamples > 0); // it wouldn't make much sense for this to be less than 1
	minimumSubBlockSize = numSamples;
	subBlockSubdivisionIsStrict = shouldBeStrict;
}

//==============================================================================
void VASTSynthesiser::setCurrentPlaybackSampleRate(const double newRate)
{
	if (sampleRate != newRate)
	{
		const ScopedLock sl(lock);
		allNotesOff(0, false);
		sampleRate = newRate;

		for (auto* voice : voices) {
            if (voice == nullptr) {
                vassertfalse;
                continue;
            }
			if (voice->mVoiceNo >= m_Set->m_uMaxPoly) {
				continue; //safety
			}
			voice->setCurrentPlaybackSampleRate(newRate);
		}
	}
}

void VASTSynthesiser::renderNextBlock(sRoutingBuffers& routingBuffers,
	const MidiBuffer& inputMidi,
	int startSample,
	int numSamples)
{
	//Clear UI atomics
	std::fill_n(m_voicePlaying, C_MAX_POLY, false);
	//Clear UI atomics

	if (*m_Set->m_State->m_fPortamento > 0.0f) {        
        auto midiIterator = inputMidi.findNextSamplePosition (startSample);
        std::for_each (midiIterator,
                       inputMidi.cend(),
                       [&] (const MidiMessageMetadata& metadata)
            {
                if (metadata.getMessage().isNoteOn()) {
                    m_bGlissandoUsed = true;
                    m_iGlissandoAssigned = 0;
                    m_newChordStack[metadata.getMessage().getNoteNumber()] = true;
                    m_iGlissandoCollectSamples = m_Set->m_nSampleRate * 0.01f; //TEST VALUE 1/100s
                    m_iGlissandoCollectSamples = 0;
                }
            });
	}
	
	//========================================================================================
        processNextBlock(routingBuffers, inputMidi, startSample, numSamples);
	//========================================================================================
        
	//CHVAST
	if (*m_Set->m_State->m_fPortamento > 0.0f) {
		if (m_bGlissandoUsed) {
			m_iGlissandoCollectSamples -= numSamples;
			if (m_iGlissandoCollectSamples <= 0) {
				int numOld = 0;
				int numNew = 0;

				for (int i = 0; i < numElementsInArray(m_midiNotesKeyDown); ++i) { //add whats still pressed
					if (m_midiNotesKeyDown[i]) {
						m_newChordStack[i] = true;
						numNew++;
					}
				}

				String outstr = "Old ChordStack: ";
				for (int i = 0; i < numElementsInArray(m_oldChordStack); ++i) {
					if (m_oldChordStack[i] == true) {
						outstr += String(i) + " , ";
						numOld++;
					}
				}
#ifdef _DEBUG
				VDBG(outstr);

				outstr = "New ChordStack: ";
				for (int i = 0; i < numElementsInArray(m_newChordStack); ++i) {
					if (m_newChordStack[i] == true) {
						outstr += String(i) + " , ";						
					}
				}
				VDBG(outstr);
#endif			
				for (auto* voice : voices) {
                    if (voice == nullptr) {
                        vassertfalse;
                        continue;
                    }
					if (voice->mVoiceNo >= m_Set->m_uMaxPoly) {
						continue; //safety
					}
                    if (voice->getCurrentlyPlayingNote()<0)
                        continue;
					if (m_newChordStack[voice->getCurrentlyPlayingNote()] == true) {

						//TODO check if voice is not hard stopping / tailing off!!
						if (!voice->isVoiceActive() || voice->isPlayingButReleased()) {
							VDBG("--------> Inactive or Playing but released!!!");
							continue;
						}

						int posInNewStack = 0;
						for (int i = 0; i < voice->getCurrentlyPlayingNote(); ++i) {
							if (m_newChordStack[i] == true) posInNewStack++;  
						}
						int posInOldStack = -1;
						int samePosOldStackMidiNote = -1;
						int oldStackMidiNote = -1;
						for (int i = 0; i < numElementsInArray(m_oldChordStack); ++i) {
							if (m_oldChordStack[i] == true) {
								posInOldStack++;
								oldStackMidiNote = i;
								if (posInOldStack == posInNewStack)
									samePosOldStackMidiNote = i;
							}
						}

						//CHTS mono legato
						//if (((m_Set->m_uMaxPoly) == 1) && (*m_Set->m_State->m_bLegatoMode == static_cast<int>(SWITCH::SWITCH_ON))) {
						if ((m_Set->m_uMaxPoly) == 1)  {
							if (*m_Set->m_State->m_bLegatoMode == static_cast<int>(SWITCH::SWITCH_ON)) {
								if (numNew == 1) 
									samePosOldStackMidiNote = -1; //reset in every case when mono and there is only one key pressed --> no porta

								//if ((numOld <= 1) && (numNew == 1)) samePosOldStackMidiNote = -1; //reset
								else
									if (oldStackMidiNote != -1) {
										if (oldStackMidiNote == voice->getCurrentlyPlayingNote()) {
											//bool rel = voice->isPlayingButReleased();
											VDBG("-------> sameNote transition");
										} else 
										if (samePosOldStackMidiNote == -1)
											samePosOldStackMidiNote = oldStackMidiNote;
									}
							} else
                                if (oldStackMidiNote != -1) {
                                    if (oldStackMidiNote == voice->getCurrentlyPlayingNote()) {
                                        //bool rel = voice->isPlayingButReleased();
                                        VDBG("-------> sameNote transition");
                                    } else
                                    if (samePosOldStackMidiNote == -1)
                                        samePosOldStackMidiNote = oldStackMidiNote;
                                }
						}

						int handledNote = -1;
						if (samePosOldStackMidiNote >= 0) {						

							handledNote = samePosOldStackMidiNote;
							((CVASTSingleNote*)voice)->setGlissandoStart(handledNote, false);
#ifdef _DEBUG
							outstr = "Transition for voice# " + String(voice->mVoiceNo) + " from " + String(samePosOldStackMidiNote) + " to " + String(voice->getCurrentlyPlayingNote());
							VDBG(outstr);
#endif
						}
						else {
							//reset glissando
							handledNote = voice->getCurrentlyPlayingNote();
							((CVASTSingleNote*)voice)->setGlissandoStart(handledNote, true);
#ifdef _DEBUG
							outstr = "Reset for voice# " + String(voice->mVoiceNo) + " to " + String(voice->getCurrentlyPlayingNote());
							VDBG(outstr);
#endif
						}
					
					}
				}

				std::copy(std::begin(m_newChordStack), std::end(m_newChordStack), std::begin(m_oldChordStack));
				//memcpy(m_oldChordStack, m_newChordStack, 256 * sizeof(bool)); //copy it
				std::fill(std::begin(m_newChordStack), std::end(m_newChordStack), false);
				//memset(m_newChordStack, false, 256 * sizeof(bool)); //init it
				m_bGlissandoUsed = false;
			}
		}
	}
}

void VASTSynthesiser::processNextBlock(sRoutingBuffers& routingBuffers,
	const MidiBuffer& midiData,
	int startSample,
	int numSamples)
{
	// must set the sample rate before using this!
	jassert(sampleRate != 0);
	const int targetChannels = 1; //HACK

    auto midiIterator = midiData.findNextSamplePosition (startSample);

	bool firstEvent = true;
	const ScopedLock sl(lock);

    for (; numSamples > 0; ++midiIterator)
    {
		//CHVAST
		for (auto* voice : voices) {
            if (voice == nullptr) {
                vassertfalse;
                continue;
            }
			if (voice->mVoiceNo >= m_Set->m_uMaxPoly) {
				continue; //safety
			}
			CVASTSingleNote* singleNote = (CVASTSingleNote*)voice;			
			FloatVectorOperations::fill(routingBuffers.MidiNoteBuffer[voice->mVoiceNo]->getWritePointer(0, startSample), singleNote->m_uMIDINote, numSamples);
			FloatVectorOperations::fill(routingBuffers.VelocityBuffer[voice->mVoiceNo]->getWritePointer(0, startSample), singleNote->m_uVelocity, numSamples);
		}
		//CHVAST
        
        if (midiIterator == midiData.cend())
               {
                   if (targetChannels > 0)
                       renderVoices (routingBuffers, startSample, numSamples);

                   return;
               }

        const auto metadata = *midiIterator;
        const int samplesToNextMidiMessage = metadata.samplePosition - startSample;

		if (samplesToNextMidiMessage >= numSamples)
		{
			if (targetChannels > 0)
				renderVoices(routingBuffers, startSample, numSamples);

			handleMidiEvent(metadata.getMessage());
			break;
		}

		if (samplesToNextMidiMessage < ((firstEvent && !subBlockSubdivisionIsStrict) ? 1 : minimumSubBlockSize))
		{
			handleMidiEvent(metadata.getMessage());
			continue;
		}

		firstEvent = false;

		if (targetChannels > 0)
			renderVoices(routingBuffers, startSample, samplesToNextMidiMessage);

		handleMidiEvent(metadata.getMessage());
		startSample += samplesToNextMidiMessage;
		numSamples -= samplesToNextMidiMessage;
	}

    std::for_each (midiIterator,
                   midiData.cend(),
                   [&] (const MidiMessageMetadata& meta) { handleMidiEvent (meta.getMessage()); });
}

void VASTSynthesiser::renderVoices(sRoutingBuffers& routingBuffers, int startSample, int numSamples)
{
	VASTSynthesiserVoice* newestVoice = dynamic_cast<VASTSynthesiserVoice*>(voices[0]); //just initialize with one
	VASTSynthesiserVoice* oldestVoice = dynamic_cast<VASTSynthesiserVoice*>(voices[0]); //just initialize with one
	VASTSynthesiserVoice* oldestVoiceKeyDown = dynamic_cast<VASTSynthesiserVoice*>(voices[0]); //just initialize with one
	if ((newestVoice == nullptr) || (oldestVoice == nullptr) || (oldestVoiceKeyDown == nullptr))
		return; //error state!
	
	float* custModBuffer0 = routingBuffers.CustomModulatorBuffer[0]->getWritePointer(0);
	float* custModBuffer1 = routingBuffers.CustomModulatorBuffer[1]->getWritePointer(0);
	float* custModBuffer2 = routingBuffers.CustomModulatorBuffer[2]->getWritePointer(0);
	float* custModBuffer3 = routingBuffers.CustomModulatorBuffer[3]->getWritePointer(0);
	int l_numVoicesPlaying = 0;
	int l_numOscsPlaying = 0;

	for (int i = startSample; i < startSample + numSamples; i++) {
		//do pitch bend
		for (int midiChannel = 0; midiChannel < 16; midiChannel++) {
			if (m_fPitchBendZone_smoothed[midiChannel].isSmoothing()) {
				float l_zoneWheelValueSmoothed = m_fPitchBendZone_smoothed[midiChannel].getNextValue();
				for (auto* voice : voices) {
                    if (voice == nullptr) {
                        vassertfalse;
                        continue;
                    }
					if (voice->mVoiceNo >= m_Set->m_uMaxPoly) {
						continue; //safety
					}
					//if (midiChannel <= 0 || voice->isPlayingChannel(midiChannel))		
					if (myProcessor->isMPEenabled()) {
						if (midiChannel == m_MPEMasterChannel)
							voice->pitchWheelMoved(l_zoneWheelValueSmoothed, true); //zone
						else if (voice->isPlayingChannel(midiChannel))
							voice->pitchWheelMoved(l_zoneWheelValueSmoothed, false);
					}
					else
						voice->pitchWheelMoved(l_zoneWheelValueSmoothed, true);
				}
			}
		}
		
		for (int midiChannel = 0; midiChannel < 16; midiChannel++) {
			if (m_fModWheel_smoothed[midiChannel].isSmoothing()) { //do mod wheel, nor midi channel sensitive	
				float l_modWheelPos = m_fModWheel_smoothed[midiChannel].getNextValue();

				for (auto* voice : voices) {
                    if (voice == nullptr) {
                        vassertfalse;
                        continue;
                    }
					if (voice->mVoiceNo >= m_Set->m_uMaxPoly) {
						continue; //safety
					}

					//if (midiChannel <= 0 || voice->isPlayingChannel(midiChannel))		
					if (myProcessor->isMPEenabled()) {
						if (voice->isPlayingChannel(midiChannel))
							voice->controllerMoved(01, l_modWheelPos);
					}
					else
						voice->controllerMoved(01, l_modWheelPos);
				}
			}
		}

		//custom modulators - first depends on nothing
		if (m_Set->modMatrixSourceSetFast(MODMATSRCE::CustomModulator1)) {
			custModBuffer0[i] = m_Poly->m_fCustomModulator1_smoothed.getNextValue();
		}
		if (m_Set->modMatrixSourceSetFast(MODMATSRCE::CustomModulator2)) {
			custModBuffer1[i] = m_Poly->m_fCustomModulator2_smoothed.getNextValue();
		}
		if (m_Set->modMatrixSourceSetFast(MODMATSRCE::CustomModulator3)) {
			custModBuffer2[i] = m_Poly->m_fCustomModulator3_smoothed.getNextValue();
		}
		if (m_Set->modMatrixSourceSetFast(MODMATSRCE::CustomModulator4)) {
			custModBuffer3[i] = m_Poly->m_fCustomModulator4_smoothed.getNextValue();
		}

		//global LFO
		if (routingBuffers.lfoUsed[0].load()) {
			if (m_Set->modMatrixDestinationSetFast(MODMATDEST::LFO1Frequency) == true) {
				float fMod = 0.0f;
				modMatrixInputState inputState = m_Poly->getOldestNotePlayedInputState(i); // make parameter oldest or newest
				fMod = m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fLFOFreq_LFO1, MODMATDEST::LFO1Frequency, &inputState);
				m_Poly->m_global_LFO_Osc[0].updatePitchMod(fMod);
				m_Poly->m_global_LFO_Osc[0].setFrequency(0, false);
			}

			if (*m_Set->m_State->m_bLFOPerVoice_LFO1 == static_cast<int>(SWITCH::SWITCH_OFF)) {
				float lfoval = 0.0f;
				m_Poly->m_global_LFO_Osc[0].getOscillation(&lfoval);
				routingBuffers.LFOGlobalBuffer[0]->getWritePointer(0)[i] = lfoval;
			}
		}
		if (routingBuffers.lfoUsed[1].load()) {
			if (m_Set->modMatrixDestinationSetFast(MODMATDEST::LFO2Frequency) == true) {
				float fMod = 0.0f;
				modMatrixInputState inputState = m_Poly->getOldestNotePlayedInputState(i); // make parameter oldest or newest
				fMod = m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fLFOFreq_LFO2, MODMATDEST::LFO2Frequency, &inputState);
				m_Poly->m_global_LFO_Osc[1].updatePitchMod(fMod);
				m_Poly->m_global_LFO_Osc[1].setFrequency(0, false);
			}

			if (*m_Set->m_State->m_bLFOPerVoice_LFO2 == static_cast<int>(SWITCH::SWITCH_OFF)) {
				float lfoval = 0.0f;
				m_Poly->m_global_LFO_Osc[1].getOscillation(&lfoval);
				routingBuffers.LFOGlobalBuffer[1]->getWritePointer(0)[i] = lfoval;
			}
		}
		if (routingBuffers.lfoUsed[2].load()) {
			if (m_Set->modMatrixDestinationSetFast(MODMATDEST::LFO3Frequency) == true) {
				float fMod = 0.0f;
				modMatrixInputState inputState = m_Poly->getOldestNotePlayedInputState(i); // make parameter oldest or newest
				fMod = m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fLFOFreq_LFO3, MODMATDEST::LFO3Frequency, &inputState);
				m_Poly->m_global_LFO_Osc[2].updatePitchMod(fMod);
				m_Poly->m_global_LFO_Osc[2].setFrequency(0, false);
			}

			if (*m_Set->m_State->m_bLFOPerVoice_LFO3 == static_cast<int>(SWITCH::SWITCH_OFF)) {
				float lfoval = 0.0f;
				m_Poly->m_global_LFO_Osc[2].getOscillation(&lfoval);
				routingBuffers.LFOGlobalBuffer[2]->getWritePointer(0)[i] = lfoval;
			}
		}
		if (routingBuffers.lfoUsed[3].load()) {
			if (m_Set->modMatrixDestinationSetFast(MODMATDEST::LFO4Frequency) == true) {
				float fMod = 0.0f;
				modMatrixInputState inputState = m_Poly->getOldestNotePlayedInputState(i); // make parameter oldest or newest
				fMod = m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fLFOFreq_LFO4, MODMATDEST::LFO4Frequency, &inputState);
				m_Poly->m_global_LFO_Osc[3].updatePitchMod(fMod);
				m_Poly->m_global_LFO_Osc[3].setFrequency(0, false);
			}

			if (*m_Set->m_State->m_bLFOPerVoice_LFO4 == static_cast<int>(SWITCH::SWITCH_OFF)) {
				float lfoval = 0.0f;
				m_Poly->m_global_LFO_Osc[3].getOscillation(&lfoval);
				routingBuffers.LFOGlobalBuffer[3]->getWritePointer(0)[i] = lfoval;
			}
		}
		if (routingBuffers.lfoUsed[4].load()) {
			if (m_Set->modMatrixDestinationSetFast(MODMATDEST::LFO5Frequency) == true) {
				float fMod = 0.0f;
				modMatrixInputState inputState = m_Poly->getOldestNotePlayedInputState(i); // make parameter oldest or newest
				fMod = m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fLFOFreq_LFO5, MODMATDEST::LFO5Frequency, &inputState);
				m_Poly->m_global_LFO_Osc[4].updatePitchMod(fMod);
				m_Poly->m_global_LFO_Osc[4].setFrequency(0, false);
			}

			if (*m_Set->m_State->m_bLFOPerVoice_LFO5 == static_cast<int>(SWITCH::SWITCH_OFF)) {
				float lfoval = 0.0f;
				m_Poly->m_global_LFO_Osc[4].getOscillation(&lfoval);
				routingBuffers.LFOGlobalBuffer[4]->getWritePointer(0)[i] = lfoval;
			}
		}

		//stepseqs		
		if (routingBuffers.stepSeqUsed[0].load()) { //performance optimization
			routingBuffers.StepSeqBuffer[0]->getWritePointer(0)[i] = m_Poly->m_StepSeq_Envelope[0].getEnvelopeStepSeq(startSample + i);
		}
		if (routingBuffers.stepSeqUsed[1].load()) { //performance optimization
			routingBuffers.StepSeqBuffer[1]->getWritePointer(0)[i] = m_Poly->m_StepSeq_Envelope[1].getEnvelopeStepSeq(startSample + i);
		}
		if (routingBuffers.stepSeqUsed[2].load()) { //performance optimization				
			routingBuffers.StepSeqBuffer[2]->getWritePointer(0)[i] = m_Poly->m_StepSeq_Envelope[2].getEnvelopeStepSeq(startSample + i);
		}
	}

	//per voice
	bool bMSEG1Sustain = m_Set->modMatrixDestinationSetFast(MODMATDEST::MSEG1Sustain);
	bool bMSEG1Attack = m_Set->modMatrixDestinationSetFast(MODMATDEST::MSEG1Attack);
	bool bMSEG1Decay = m_Set->modMatrixDestinationSetFast(MODMATDEST::MSEG1Decay);
	bool bMSEG1Release = m_Set->modMatrixDestinationSetFast(MODMATDEST::MSEG1Release);
	bool bMSEG1AttackSteps = m_Set->modMatrixDestinationSetFast(MODMATDEST::MSEG1AttackSteps);
	bool bMSEG1DecaySteps = m_Set->modMatrixDestinationSetFast(MODMATDEST::MSEG1DecaySteps);
	bool bMSEG1ReleaseSteps = m_Set->modMatrixDestinationSetFast(MODMATDEST::MSEG1ReleaseSteps);

	bool bMSEG2Sustain = m_Set->modMatrixDestinationSetFast(MODMATDEST::MSEG2Sustain);	
	bool bMSEG2Attack = m_Set->modMatrixDestinationSetFast(MODMATDEST::MSEG2Attack);
	bool bMSEG2Decay = m_Set->modMatrixDestinationSetFast(MODMATDEST::MSEG2Decay);
	bool bMSEG2Release = m_Set->modMatrixDestinationSetFast(MODMATDEST::MSEG2Release);
	bool bMSEG2AttackSteps = m_Set->modMatrixDestinationSetFast(MODMATDEST::MSEG2AttackSteps);
	bool bMSEG2DecaySteps = m_Set->modMatrixDestinationSetFast(MODMATDEST::MSEG2DecaySteps);
	bool bMSEG2ReleaseSteps = m_Set->modMatrixDestinationSetFast(MODMATDEST::MSEG2ReleaseSteps);

	bool bMSEG3Sustain = m_Set->modMatrixDestinationSetFast(MODMATDEST::MSEG3Sustain);
	bool bMSEG3Attack = m_Set->modMatrixDestinationSetFast(MODMATDEST::MSEG3Attack);
	bool bMSEG3Decay = m_Set->modMatrixDestinationSetFast(MODMATDEST::MSEG3Decay);
	bool bMSEG3Release = m_Set->modMatrixDestinationSetFast(MODMATDEST::MSEG3Release);
	bool bMSEG3AttackSteps = m_Set->modMatrixDestinationSetFast(MODMATDEST::MSEG3AttackSteps);
	bool bMSEG3DecaySteps = m_Set->modMatrixDestinationSetFast(MODMATDEST::MSEG3DecaySteps);
	bool bMSEG3ReleaseSteps = m_Set->modMatrixDestinationSetFast(MODMATDEST::MSEG3ReleaseSteps);

	bool bMSEG4Sustain = m_Set->modMatrixDestinationSetFast(MODMATDEST::MSEG4Sustain);
	bool bMSEG4Attack = m_Set->modMatrixDestinationSetFast(MODMATDEST::MSEG4Attack);
	bool bMSEG4Decay = m_Set->modMatrixDestinationSetFast(MODMATDEST::MSEG4Decay);
	bool bMSEG4Release = m_Set->modMatrixDestinationSetFast(MODMATDEST::MSEG4Release);
	bool bMSEG4AttackSteps = m_Set->modMatrixDestinationSetFast(MODMATDEST::MSEG4AttackSteps);
	bool bMSEG4DecaySteps = m_Set->modMatrixDestinationSetFast(MODMATDEST::MSEG4DecaySteps);
	bool bMSEG4ReleaseSteps = m_Set->modMatrixDestinationSetFast(MODMATDEST::MSEG4ReleaseSteps);

	bool bMSEG5Sustain = m_Set->modMatrixDestinationSetFast(MODMATDEST::MSEG5Sustain);
	bool bMSEG5Attack = m_Set->modMatrixDestinationSetFast(MODMATDEST::MSEG5Attack);
	bool bMSEG5Decay = m_Set->modMatrixDestinationSetFast(MODMATDEST::MSEG5Decay);	
	bool bMSEG5Release = m_Set->modMatrixDestinationSetFast(MODMATDEST::MSEG5Release);
	bool bMSEG5AttackSteps = m_Set->modMatrixDestinationSetFast(MODMATDEST::MSEG5AttackSteps);
	bool bMSEG5DecaySteps = m_Set->modMatrixDestinationSetFast(MODMATDEST::MSEG5DecaySteps);
	bool bMSEG5ReleaseSteps = m_Set->modMatrixDestinationSetFast(MODMATDEST::MSEG5ReleaseSteps);


	m_oldestPlayingKeyDown = -1;
	for (auto* voice : voices) {
        if (voice == nullptr) {
            vassertfalse;
            continue;
        }
		if (voice->mVoiceNo >= m_Set->m_uMaxPoly) {
			continue; //safety
		}
		modMatrixInputState inputState{ ((CVASTSingleNote*)voice)->getVoiceNo(), 0};

		//per voice LFO modulating mseg?
		bool blfoprocessed[5];
		blfoprocessed[0] = false; blfoprocessed[1] = false; blfoprocessed[2] = false; blfoprocessed[3] = false; blfoprocessed[4] = false;
		for (int slot = 0; slot < M_MODMATRIX_MAX_SLOTS; slot++) {
			float  l_value = 0.f;
			double l_curvy = 0.f;
			int l_srce = 0;
			int l_dest = 0;
			float lastSrceVals[C_MAX_POLY] {};
			int polarity = 0;
			m_Set->modMatrixSlotGetValues(slot, l_value, l_curvy, l_srce, l_dest, polarity, lastSrceVals);
			if ((l_srce == MODMATSRCE::LFO1) && ((l_dest >= MODMATDEST::MSEG1Attack) && (l_dest <= MODMATDEST::MSEG5Release))) {
				if (blfoprocessed[0] == false) {
					if (routingBuffers.lfoUsed[0].load()) {
						if (m_Set->modMatrixDestinationSetFast(MODMATDEST::LFO1Frequency) == true) {
							float fMod = 0.0f;
							inputState.currentFrame = startSample;
							fMod = m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fLFOFreq_LFO1, MODMATDEST::LFO1Frequency, &inputState);
							((CVASTSingleNote*)voice)->m_LFO_Osc[0].updatePitchMod(fMod);
							((CVASTSingleNote*)voice)->m_LFO_Osc[0].setFrequency(0, false);
						}

						if (*m_Set->m_State->m_bLFOPerVoice_LFO1 == static_cast<int>(SWITCH::SWITCH_ON)) {
							for (int currentFrame = startSample; currentFrame < startSample + numSamples; currentFrame++) {
								float lfoval = 0.0f;
								inputState.currentFrame = startSample;
								((CVASTSingleNote*)voice)->m_LFO_Osc[0].getOscillation(&lfoval);
								//routingBuffers.LFOBuffer[0][voice->mVoiceNo]->setSample(0, currentFrame, lfoval);
								routingBuffers.LFOBuffer[0][voice->mVoiceNo]->getWritePointer(0)[currentFrame] = lfoval;
							}
						}
					}
					blfoprocessed[0] = true;
				}
			}
			if ((l_srce == MODMATSRCE::LFO2) && ((l_dest >= MODMATDEST::MSEG1Attack) && (l_dest <= MODMATDEST::MSEG5Release))) {
				if (blfoprocessed[1] == false) {
					if (routingBuffers.lfoUsed[1].load()) {
						if (m_Set->modMatrixDestinationSetFast(MODMATDEST::LFO2Frequency) == true) {
							float fMod = 0.0f;
							inputState.currentFrame = startSample;
							fMod = m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fLFOFreq_LFO2, MODMATDEST::LFO2Frequency, &inputState);
							((CVASTSingleNote*)voice)->m_LFO_Osc[1].updatePitchMod(fMod);
							((CVASTSingleNote*)voice)->m_LFO_Osc[1].setFrequency(0, false);
						}

						if (*m_Set->m_State->m_bLFOPerVoice_LFO2 == static_cast<int>(SWITCH::SWITCH_ON)) {
							for (int currentFrame = startSample; currentFrame < startSample + numSamples; currentFrame++) {
								float lfoval = 0.0f;
								((CVASTSingleNote*)voice)->m_LFO_Osc[1].getOscillation(&lfoval);
								//routingBuffers.LFOBuffer[1][voice->mVoiceNo]->setSample(0, currentFrame, lfoval);
								routingBuffers.LFOBuffer[1][voice->mVoiceNo]->getWritePointer(0)[currentFrame] = lfoval;
							}
						}
					}
					blfoprocessed[1] = true;
				}
			}
			if ((l_srce == MODMATSRCE::LFO3) && ((l_dest >= MODMATDEST::MSEG1Attack) && (l_dest <= MODMATDEST::MSEG5Release))) {
				if (blfoprocessed[2] == false) {
					if (routingBuffers.lfoUsed[2].load()) {
						if (m_Set->modMatrixDestinationSetFast(MODMATDEST::LFO3Frequency) == true) {
							float fMod = 0.0f;
							inputState.currentFrame = startSample;
							fMod = m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fLFOFreq_LFO3, MODMATDEST::LFO3Frequency, &inputState);
							((CVASTSingleNote*)voice)->m_LFO_Osc[2].updatePitchMod(fMod);
							((CVASTSingleNote*)voice)->m_LFO_Osc[2].setFrequency(0, false);
						}

						if (*m_Set->m_State->m_bLFOPerVoice_LFO3 == static_cast<int>(SWITCH::SWITCH_ON)) {
							for (int currentFrame = startSample; currentFrame < startSample + numSamples; currentFrame++) {
								float lfoval = 0.0f;
								((CVASTSingleNote*)voice)->m_LFO_Osc[2].getOscillation(&lfoval);
								//routingBuffers.LFOBuffer[2][voice->mVoiceNo]->setSample(0, currentFrame, lfoval);
								routingBuffers.LFOBuffer[2][voice->mVoiceNo]->getWritePointer(0)[currentFrame] = lfoval;
							}
						}
					}
					blfoprocessed[2] = true;
				}
			}
			if ((l_srce == MODMATSRCE::LFO4) && ((l_dest >= MODMATDEST::MSEG1Attack) && (l_dest <= MODMATDEST::MSEG5Release))) {
				if (blfoprocessed[3] == false) {
					if (routingBuffers.lfoUsed[3].load()) {
						if (m_Set->modMatrixDestinationSetFast(MODMATDEST::LFO4Frequency) == true) {
							float fMod = 0.0f;
							inputState.currentFrame = startSample;
							fMod = m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fLFOFreq_LFO4, MODMATDEST::LFO4Frequency, &inputState);
							((CVASTSingleNote*)voice)->m_LFO_Osc[3].updatePitchMod(fMod);
							((CVASTSingleNote*)voice)->m_LFO_Osc[3].setFrequency(0, false);
						}

						if (*m_Set->m_State->m_bLFOPerVoice_LFO4 == static_cast<int>(SWITCH::SWITCH_ON)) {
							for (int currentFrame = startSample; currentFrame < startSample + numSamples; currentFrame++) {
								float lfoval = 0.0f;
								((CVASTSingleNote*)voice)->m_LFO_Osc[3].getOscillation(&lfoval);
								//routingBuffers.LFOBuffer[3][voice->mVoiceNo]->setSample(0, currentFrame, lfoval);
								routingBuffers.LFOBuffer[3][voice->mVoiceNo]->getWritePointer(0)[currentFrame] = lfoval;
							}
						}
					}
					blfoprocessed[3] = true;
				}
			}
			if ((l_srce == MODMATSRCE::LFO5) && ((l_dest >= MODMATDEST::MSEG1Attack) && (l_dest <= MODMATDEST::MSEG5Release))) {
				if (blfoprocessed[4] == false) {
					if (routingBuffers.lfoUsed[4].load()) {
						if (m_Set->modMatrixDestinationSetFast(MODMATDEST::LFO5Frequency) == true) {
							float fMod = 0.0f;
							inputState.currentFrame = startSample;
							fMod = m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fLFOFreq_LFO5, MODMATDEST::LFO5Frequency, &inputState);
							((CVASTSingleNote*)voice)->m_LFO_Osc[4].updatePitchMod(fMod);
							((CVASTSingleNote*)voice)->m_LFO_Osc[4].setFrequency(0, false);
						}

						if (*m_Set->m_State->m_bLFOPerVoice_LFO5 == static_cast<int>(SWITCH::SWITCH_ON)) {
							for (int currentFrame = startSample; currentFrame < startSample + numSamples; currentFrame++) {
								float lfoval = 0.0f;
								((CVASTSingleNote*)voice)->m_LFO_Osc[4].getOscillation(&lfoval);
								//routingBuffers.LFOBuffer[4][voice->mVoiceNo]->setSample(0, currentFrame, lfoval);
								routingBuffers.LFOBuffer[4][voice->mVoiceNo]->getWritePointer(0)[currentFrame] = lfoval;
							}
						}
					}
					blfoprocessed[4] = true;
				}
			}
		}

		for (int mseg = 4; mseg >= 0; mseg--) {
            /*
			bool msegpervoice = false;
			if ((mseg == 0) && (*m_Set->m_State->m_bMSEGPerVoice_MSEG1 == static_cast<int>(SWITCH::SWITCH_ON)))
				msegpervoice = true;
			if ((mseg == 1) && (*m_Set->m_State->m_bMSEGPerVoice_MSEG2 == static_cast<int>(SWITCH::SWITCH_ON)))
				msegpervoice = true;
			if ((mseg == 2) && (*m_Set->m_State->m_bMSEGPerVoice_MSEG3 == static_cast<int>(SWITCH::SWITCH_ON)))
				msegpervoice = true;
			if ((mseg == 3) && (*m_Set->m_State->m_bMSEGPerVoice_MSEG4 == static_cast<int>(SWITCH::SWITCH_ON)))
				msegpervoice = true;
			if ((mseg == 4) && (*m_Set->m_State->m_bMSEGPerVoice_MSEG5 == static_cast<int>(SWITCH::SWITCH_ON)))
				msegpervoice = true;
            */

			if (routingBuffers.msegUsed[mseg].load()) { //performance optimization
				//if (((CVASTSingleNote*)voice)->m_VCA.m_MSEG_Envelope[mseg].isActive() == true) { //This is not valid here!! leads to many issues!!

				float* msegWritePointer = routingBuffers.MSEGBuffer[mseg][voice->mVoiceNo]->getWritePointer(0);
				bool* msegActivePointer = routingBuffers.MSEGActiveBuffer[mseg][voice->mVoiceNo];
				bool bIsActive = ((CVASTSingleNote*)voice)->m_VCA.m_MSEG_Envelope[mseg].isActive();
			
				int stepsize = 16;
				int currentFrame = 0;
				for (currentFrame = startSample; currentFrame <= startSample + numSamples - stepsize; currentFrame += stepsize) {
					int numFrames = stepsize;
					jassert(currentFrame + numFrames - 1 < routingBuffers.getNumSamples());
					inputState.currentFrame = currentFrame;
					if (bIsActive) { //perf opt, check with phase and frequency that is not updated?			
						std::fill(msegActivePointer + currentFrame, msegActivePointer + currentFrame + numFrames, bIsActive);
						if (mseg == 0) {
							if (bMSEG1Sustain) m_Set->m_MSEGData[0].setSustainLevel(m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fSustainLevel_MSEG1, MODMATDEST::MSEG1Sustain, &inputState) * 0.01f);
							if (*m_Set->m_State->m_bMSEGSynch_MSEG1 == static_cast<int>(SWITCH::SWITCH_OFF)) {
								if (bMSEG1Attack) m_Set->m_MSEGData[0].setAttackTime(m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fAttackTime_MSEG1, MODMATDEST::MSEG1Attack, &inputState));
								if (bMSEG1Decay) m_Set->m_MSEGData[0].setDecayTime(m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fDecayTime_MSEG1, MODMATDEST::MSEG1Decay, &inputState));
								if (bMSEG1Release) m_Set->m_MSEGData[0].setReleaseTime(m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fReleaseTime_MSEG1, MODMATDEST::MSEG1Release, &inputState));
							}
							else {
								if (bMSEG1AttackSteps) m_Set->m_MSEGData[0].setAttackSteps(m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fAttackSteps_MSEG1, MODMATDEST::MSEG1AttackSteps, &inputState), m_Set);
								if (bMSEG1DecaySteps) m_Set->m_MSEGData[0].setDecaySteps(m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fDecaySteps_MSEG1, MODMATDEST::MSEG1DecaySteps, &inputState), m_Set);
								if (bMSEG1ReleaseSteps) m_Set->m_MSEGData[0].setReleaseSteps(m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fReleaseSteps_MSEG1, MODMATDEST::MSEG1ReleaseSteps, &inputState), m_Set);
							}
						}
						if (mseg == 1) {
							if (bMSEG2Sustain) m_Set->m_MSEGData[1].setSustainLevel(m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fSustainLevel_MSEG2, MODMATDEST::MSEG2Sustain, &inputState) * 0.01f);
							if (*m_Set->m_State->m_bMSEGSynch_MSEG2 == static_cast<int>(SWITCH::SWITCH_OFF)) {
								if (bMSEG2Attack) m_Set->m_MSEGData[1].setAttackTime(m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fAttackTime_MSEG2, MODMATDEST::MSEG2Attack, &inputState));
								if (bMSEG2Decay) m_Set->m_MSEGData[1].setDecayTime(m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fDecayTime_MSEG2, MODMATDEST::MSEG2Decay, &inputState));					
								if (bMSEG2Release) m_Set->m_MSEGData[1].setReleaseTime(m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fReleaseTime_MSEG2, MODMATDEST::MSEG2Release, &inputState));
							}
							else {
								if (bMSEG2AttackSteps) m_Set->m_MSEGData[1].setAttackSteps(m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fAttackSteps_MSEG2, MODMATDEST::MSEG2AttackSteps, &inputState), m_Set);
								if (bMSEG2DecaySteps) m_Set->m_MSEGData[1].setDecaySteps(m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fDecaySteps_MSEG2, MODMATDEST::MSEG2DecaySteps, &inputState), m_Set);
								if (bMSEG2ReleaseSteps) m_Set->m_MSEGData[1].setReleaseSteps(m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fReleaseSteps_MSEG2, MODMATDEST::MSEG2ReleaseSteps, &inputState), m_Set);
							}
						}
						if (mseg == 2) {
							if (bMSEG3Sustain) m_Set->m_MSEGData[2].setSustainLevel(m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fSustainLevel_MSEG3, MODMATDEST::MSEG3Sustain, &inputState) * 0.01f);
							if (*m_Set->m_State->m_bMSEGSynch_MSEG3 == static_cast<int>(SWITCH::SWITCH_OFF)) {
								if (bMSEG3Attack) m_Set->m_MSEGData[2].setAttackTime(m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fAttackTime_MSEG3, MODMATDEST::MSEG3Attack, &inputState));
								if (bMSEG3Decay) m_Set->m_MSEGData[2].setDecayTime(m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fDecayTime_MSEG3, MODMATDEST::MSEG3Decay, &inputState));					
								if (bMSEG3Release) m_Set->m_MSEGData[2].setReleaseTime(m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fReleaseTime_MSEG3, MODMATDEST::MSEG3Release, &inputState));
							}
							else {
								if (bMSEG3AttackSteps) m_Set->m_MSEGData[2].setAttackSteps(m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fAttackSteps_MSEG3, MODMATDEST::MSEG3AttackSteps, &inputState), m_Set);
								if (bMSEG3DecaySteps) m_Set->m_MSEGData[2].setDecaySteps(m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fDecaySteps_MSEG3, MODMATDEST::MSEG3DecaySteps, &inputState), m_Set);
								if (bMSEG3ReleaseSteps) m_Set->m_MSEGData[2].setReleaseSteps(m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fReleaseSteps_MSEG3, MODMATDEST::MSEG3ReleaseSteps, &inputState), m_Set);
							}

						}
						if (mseg == 3) {
							if (bMSEG4Sustain) m_Set->m_MSEGData[3].setSustainLevel(m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fSustainLevel_MSEG4, MODMATDEST::MSEG4Sustain, &inputState) * 0.01f);
							if (*m_Set->m_State->m_bMSEGSynch_MSEG4 == static_cast<int>(SWITCH::SWITCH_OFF)) {
								if (bMSEG4Attack) m_Set->m_MSEGData[3].setAttackTime(m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fAttackTime_MSEG4, MODMATDEST::MSEG4Attack, &inputState));
								if (bMSEG4Decay) m_Set->m_MSEGData[3].setDecayTime(m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fDecayTime_MSEG4, MODMATDEST::MSEG4Decay, &inputState));
								if (bMSEG4Release) m_Set->m_MSEGData[3].setReleaseTime(m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fReleaseTime_MSEG4, MODMATDEST::MSEG4Release, &inputState));
							}
							else {
								if (bMSEG4AttackSteps) m_Set->m_MSEGData[3].setAttackSteps(m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fAttackSteps_MSEG4, MODMATDEST::MSEG4AttackSteps, &inputState), m_Set);
								if (bMSEG4DecaySteps) m_Set->m_MSEGData[3].setDecaySteps(m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fDecaySteps_MSEG4, MODMATDEST::MSEG4DecaySteps, &inputState), m_Set);
								if (bMSEG4ReleaseSteps) m_Set->m_MSEGData[3].setReleaseSteps(m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fReleaseSteps_MSEG4, MODMATDEST::MSEG4ReleaseSteps, &inputState), m_Set);
							}
						}
						if (mseg == 4) {
							if (bMSEG5Sustain) m_Set->m_MSEGData[4].setSustainLevel(m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fSustainLevel_MSEG5, MODMATDEST::MSEG5Sustain, &inputState) * 0.01f);
							if (*m_Set->m_State->m_bMSEGSynch_MSEG5 == static_cast<int>(SWITCH::SWITCH_OFF)) {
								if (bMSEG5Attack) m_Set->m_MSEGData[4].setAttackTime(m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fAttackTime_MSEG5, MODMATDEST::MSEG5Attack, &inputState));
								if (bMSEG5Decay) m_Set->m_MSEGData[4].setDecayTime(m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fDecayTime_MSEG5, MODMATDEST::MSEG5Decay, &inputState));
								if (bMSEG5Release) m_Set->m_MSEGData[4].setReleaseTime(m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fReleaseTime_MSEG5, MODMATDEST::MSEG5Release, &inputState));
							}
							else {
								if (bMSEG5AttackSteps) m_Set->m_MSEGData[4].setAttackSteps(m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fAttackSteps_MSEG5, MODMATDEST::MSEG5AttackSteps, &inputState), m_Set);
								if (bMSEG5DecaySteps) m_Set->m_MSEGData[4].setDecaySteps(m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fDecaySteps_MSEG5, MODMATDEST::MSEG5DecaySteps, &inputState), m_Set);
								if (bMSEG5ReleaseSteps) m_Set->m_MSEGData[4].setReleaseSteps(m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fReleaseSteps_MSEG5, MODMATDEST::MSEG5ReleaseSteps, &inputState), m_Set);
							}
						}				
                        
                        //* expensive */ //per voice per MSEG
						((CVASTSingleNote*)voice)->m_VCA.m_MSEG_Envelope[mseg].getEnvelopeRange(msegWritePointer, currentFrame, numFrames);
                        //* expensive */
					}
				} 
				//rest
				if (bIsActive) {
					if (currentFrame < startSample + numSamples) {
						jassert((startSample + numSamples - currentFrame) < stepsize);
						int numFrames = (startSample + numSamples) - currentFrame;
						jassert(currentFrame + numFrames - 1 < routingBuffers.getNumSamples());
						std::fill(msegActivePointer + currentFrame, msegActivePointer + currentFrame + numFrames, bIsActive);
						inputState.currentFrame = currentFrame;
                        
                        //* expensive */
						((CVASTSingleNote*)voice)->m_VCA.m_MSEG_Envelope[mseg].getEnvelopeRange(msegWritePointer, currentFrame, numFrames);
                        //* expensive */
					}
				}
			}
		} //mseg
		
		//LFO
		bool bPlayiningInRange = ((CVASTSingleNote*)voice)->isPlayingInRange(startSample, numSamples); //requires that mseg is processed before!

		if (bPlayiningInRange) { //perf opt, check with phase and frequency that is not updated?
			m_voicePlaying[((CVASTSingleNote*)voice)->getVoiceNo()] = true;            
			l_numVoicesPlaying++;
			l_numOscsPlaying+=((CVASTSingleNote*)voice)->getNumOscsPlaying();
			if (routingBuffers.lfoUsed[0].load()) {
				if (blfoprocessed[0] == false) {
					if (m_Set->modMatrixDestinationSetFast(MODMATDEST::LFO1Frequency) == true) {
						float fMod = 0.0f;
						inputState.currentFrame = startSample;
						fMod = m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fLFOFreq_LFO1, MODMATDEST::LFO1Frequency, &inputState);
						((CVASTSingleNote*)voice)->m_LFO_Osc[0].updatePitchMod(fMod);
						((CVASTSingleNote*)voice)->m_LFO_Osc[0].setFrequency(0, false);
					}

					if (*m_Set->m_State->m_bLFOPerVoice_LFO1 == static_cast<int>(SWITCH::SWITCH_ON)) {
						float* lfoWriteBuffer = routingBuffers.LFOBuffer[0][voice->mVoiceNo]->getWritePointer(0);
						for (int currentFrame = startSample; currentFrame < startSample + numSamples; currentFrame++) {
							float lfoval = 0.0f;
							inputState.currentFrame = startSample;
							((CVASTSingleNote*)voice)->m_LFO_Osc[0].getOscillation(&lfoval);

							float msegmult = 1.f;
							if (*m_Set->m_State->m_uLFOMSEG_LFO1 != static_cast<int>(MSEGLFONONE)) {
								msegmult = routingBuffers.MSEGBuffer[int(*m_Set->m_State->m_uLFOMSEG_LFO1) - 1][voice->mVoiceNo]->getSample(0, currentFrame);
							}

							//routingBuffers.LFOBuffer[0][voice->mVoiceNo]->setSample(0, currentFrame, lfoval * msegmult);
							lfoWriteBuffer[currentFrame] = lfoval * msegmult;
						}
					}
				}
			}
			if (routingBuffers.lfoUsed[1].load()) {
				if (blfoprocessed[1] == false) {
					if (m_Set->modMatrixDestinationSetFast(MODMATDEST::LFO2Frequency) == true) {
						float fMod = 0.0f;
						inputState.currentFrame = startSample;
						fMod = m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fLFOFreq_LFO2, MODMATDEST::LFO2Frequency, &inputState);
						((CVASTSingleNote*)voice)->m_LFO_Osc[1].updatePitchMod(fMod);
						((CVASTSingleNote*)voice)->m_LFO_Osc[1].setFrequency(0, false);
					}

					if (*m_Set->m_State->m_bLFOPerVoice_LFO2 == static_cast<int>(SWITCH::SWITCH_ON)) {
						float* lfoWriteBuffer = routingBuffers.LFOBuffer[1][voice->mVoiceNo]->getWritePointer(0);
						for (int currentFrame = startSample; currentFrame < startSample + numSamples; currentFrame++) {
							float lfoval = 0.0f;
							((CVASTSingleNote*)voice)->m_LFO_Osc[1].getOscillation(&lfoval);

							float msegmult = 1.f;
							if (*m_Set->m_State->m_uLFOMSEG_LFO2 != static_cast<int>(MSEGLFONONE)) {
								msegmult = routingBuffers.MSEGBuffer[int(*m_Set->m_State->m_uLFOMSEG_LFO2) - 1][voice->mVoiceNo]->getSample(0, currentFrame);
							}

							//routingBuffers.LFOBuffer[1][voice->mVoiceNo]->setSample(0, currentFrame, lfoval * msegmult);
							lfoWriteBuffer[currentFrame] = lfoval * msegmult;
						}
					}
				}
			}
			if (routingBuffers.lfoUsed[2].load()) {
				if (blfoprocessed[2] == false) {
					if (m_Set->modMatrixDestinationSetFast(MODMATDEST::LFO3Frequency) == true) {
						float fMod = 0.0f;
						inputState.currentFrame = startSample;
						fMod = m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fLFOFreq_LFO3, MODMATDEST::LFO3Frequency, &inputState);
						((CVASTSingleNote*)voice)->m_LFO_Osc[2].updatePitchMod(fMod);
						((CVASTSingleNote*)voice)->m_LFO_Osc[2].setFrequency(0, false);
					}

					if (*m_Set->m_State->m_bLFOPerVoice_LFO3 == static_cast<int>(SWITCH::SWITCH_ON)) {
						float* lfoWriteBuffer = routingBuffers.LFOBuffer[2][voice->mVoiceNo]->getWritePointer(0);
						for (int currentFrame = startSample; currentFrame < startSample + numSamples; currentFrame++) {
							float lfoval = 0.0f;
							((CVASTSingleNote*)voice)->m_LFO_Osc[2].getOscillation(&lfoval);

							float msegmult = 1.f;
							if (*m_Set->m_State->m_uLFOMSEG_LFO3 != static_cast<int>(MSEGLFONONE)) {
								msegmult = routingBuffers.MSEGBuffer[int(*m_Set->m_State->m_uLFOMSEG_LFO3) - 1][voice->mVoiceNo]->getSample(0, currentFrame);
							}

							//routingBuffers.LFOBuffer[2][voice->mVoiceNo]->setSample(0, currentFrame, lfoval * msegmult);
							lfoWriteBuffer[currentFrame] = lfoval * msegmult;
						}
					}
				}
			}
			if (routingBuffers.lfoUsed[3].load()) {
				if (blfoprocessed[3] == false) {
					if (m_Set->modMatrixDestinationSetFast(MODMATDEST::LFO4Frequency) == true) {
						float fMod = 0.0f;
						inputState.currentFrame = startSample;
						fMod = m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fLFOFreq_LFO4, MODMATDEST::LFO4Frequency, &inputState);
						((CVASTSingleNote*)voice)->m_LFO_Osc[3].updatePitchMod(fMod);
						((CVASTSingleNote*)voice)->m_LFO_Osc[3].setFrequency(0, false);
					}

					if (*m_Set->m_State->m_bLFOPerVoice_LFO4 == static_cast<int>(SWITCH::SWITCH_ON)) {
						float* lfoWriteBuffer = routingBuffers.LFOBuffer[3][voice->mVoiceNo]->getWritePointer(0);
						for (int currentFrame = startSample; currentFrame < startSample + numSamples; currentFrame++) {
							float lfoval = 0.0f;
							((CVASTSingleNote*)voice)->m_LFO_Osc[3].getOscillation(&lfoval);

							float msegmult = 1.f;
							if (static_cast<int>(*m_Set->m_State->m_uLFOMSEG_LFO4) != MSEGLFONONE) {
								msegmult = routingBuffers.MSEGBuffer[int(*m_Set->m_State->m_uLFOMSEG_LFO4) - 1][voice->mVoiceNo]->getSample(0, currentFrame);
							}

							//routingBuffers.LFOBuffer[3][voice->mVoiceNo]->setSample(0, currentFrame, lfoval * msegmult);
							lfoWriteBuffer[currentFrame] = lfoval * msegmult;
						}
					}
				}
			}
			if (routingBuffers.lfoUsed[4].load()) {
				if (blfoprocessed[4] == false) {
					if (m_Set->modMatrixDestinationSetFast(MODMATDEST::LFO5Frequency) == true) {
						float fMod = 0.0f;
						inputState.currentFrame = startSample;
						fMod = m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fLFOFreq_LFO5, MODMATDEST::LFO5Frequency, &inputState);
						((CVASTSingleNote*)voice)->m_LFO_Osc[4].updatePitchMod(fMod);
						((CVASTSingleNote*)voice)->m_LFO_Osc[4].setFrequency(0, false);
					}

					if (*m_Set->m_State->m_bLFOPerVoice_LFO5 == static_cast<int>(SWITCH::SWITCH_ON)) {
						float* lfoWriteBuffer = routingBuffers.LFOBuffer[4][voice->mVoiceNo]->getWritePointer(0);
						for (int currentFrame = startSample; currentFrame < startSample + numSamples; currentFrame++) {
							float lfoval = 0.0f;
							((CVASTSingleNote*)voice)->m_LFO_Osc[4].getOscillation(&lfoval);

							float msegmult = 1.f;
							if (*m_Set->m_State->m_uLFOMSEG_LFO5 != static_cast<int>(MSEGLFONONE)) {
								msegmult = routingBuffers.MSEGBuffer[int(*m_Set->m_State->m_uLFOMSEG_LFO5) - 1][voice->mVoiceNo]->getSample(0, currentFrame);
							}

							//routingBuffers.LFOBuffer[4][voice->mVoiceNo]->setSample(0, currentFrame, lfoval * msegmult);
							lfoWriteBuffer[currentFrame] = lfoval * msegmult;
						}
					}
				}
			}
		}

		//=========================================================================================
		//determine oldest and newest voice
		if ((oldestVoiceKeyDown != nullptr) && (oldestVoiceKeyDown->isKeyDown() == false)) oldestVoiceKeyDown = nullptr;
		if ((oldestVoice != nullptr) && (oldestVoice->isVoiceActive() == false)) oldestVoice = nullptr;
		if ((newestVoice != nullptr) && (newestVoice->isVoiceActive() == false)) newestVoice = nullptr;
		
		if (voice->isVoiceActive()) {
			if ((oldestVoice == nullptr) || (voice->wasStartedBefore(*oldestVoice))) {
				oldestVoice = voice;
			}
			if (oldestVoice != nullptr)
				m_oldestPlaying.store(oldestVoice->mVoiceNo);

			if (voice->isKeyDown()) {
				if ((oldestVoiceKeyDown == nullptr) || (voice->wasStartedBefore(*oldestVoiceKeyDown))) {
						oldestVoiceKeyDown = voice;
				}
				if (oldestVoiceKeyDown != nullptr)
					m_oldestPlayingKeyDown.store(oldestVoiceKeyDown->mVoiceNo);
			}

			if ((newestVoice == nullptr) || (newestVoice->wasStartedBefore(*voice))) {
				newestVoice = voice;
			}
			if (newestVoice != nullptr)
				m_newestPlaying.store(newestVoice->mVoiceNo);
		}

		//UI atomics
		for (int bank = 0; bank < 4; bank++) {
			m_Poly->m_currentWTPosFloatPercentage[bank][((CVASTSingleNote*)voice)->getVoiceNo()].store(((CVASTSingleNote*)voice)->m_currentWTPosFloatPercentage[bank].load());
			m_Poly->m_safePhaseFloat[bank][((CVASTSingleNote*)voice)->getVoiceNo()].store(((CVASTSingleNote*)voice)->m_safePhaseFloat[bank].load());
		}
		for (int lfo = 0; lfo < 5; lfo++) {
			m_Poly->m_fLastLFOOscValue[lfo][((CVASTSingleNote*)voice)->getVoiceNo()].store(((CVASTSingleNote*)voice)->m_LFO_Osc[lfo].m_fLastValue);
		}
	} //voices

	//UI atomics global
	for (int lfo = 0; lfo < 5; lfo++) {
		m_Poly->m_fLastGlobalLFOOscValue[lfo].store(m_Poly->m_global_LFO_Osc[lfo].m_fLastValue);
	}

	m_Set->m_oldestPlaying = m_oldestPlaying;
	m_Set->m_oldestPlayingKeyDown = m_oldestPlayingKeyDown;
	m_Set->m_newestPlaying = m_newestPlaying;
	//=========================================================================================
	
	for (auto* voice : voices) {
        if (voice == nullptr) {
            vassertfalse;
            continue;
        }
		if (voice->mVoiceNo >= m_Set->m_uMaxPoly) {
			continue; //safety
		}
		voice->renderNextBlock(routingBuffers, startSample, numSamples);
	}
	
	//====================================================================================
	//Filters

	modMatrixInputState l_inputState{ 0, startSample };

	bool anyFilterContent[3][C_MAX_POLY]{};
	for (int filter = 0; filter < 3; filter++) {
		//float ftype = 0.f;
		float onoff = 0.f;
		switch (filter)
		{
		case 0:
			//ftype = *m_Set->m_State->m_uFilterType_Filter1;
			onoff = *m_Set->m_State->m_bOnOff_Filter1;
			break;
		case 1:
			//ftype = *m_Set->m_State->m_uFilterType_Filter2;
			onoff = *m_Set->m_State->m_bOnOff_Filter2;
			break;
		case 2:
			//ftype = *m_Set->m_State->m_uFilterType_Filter3;
			onoff = *m_Set->m_State->m_bOnOff_Filter3;
			break;
		}

		int numPlaying = 0;
		if (onoff == static_cast<int>(SWITCH::SWITCH_ON)) {
			bool bAddedSomething = false;
			bool bAllVoicesLastOutputZero = true;

			float fR = 0.f;
			float frFilter1 = 0.f;
			float frFilter2 = 0.f;
			switch (filter)
			{
			case 0:
				fR = OSCROUTE::OSCROUTE_Filter1;
				frFilter1 = -1;
				frFilter2 = -1;
				break;
			case 1:
				fR = OSCROUTE::OSCROUTE_Filter2;
				frFilter1 = FILTER1ROUTE::FILTER1ROUTE_Filter2;
				frFilter2 = -1;
				break;
			case 2:
				fR = OSCROUTE::OSCROUTE_Filter3;
				frFilter1 = FILTER1ROUTE::FILTER1ROUTE_Filter3;
				frFilter2 = FILTER2ROUTE::FILTER2ROUTE_Filter3;
				break;
			}
			bool hasNextFilter = false; //perf opt
			if (filter == 0)
				if ((*m_Set->m_State->m_uFilterRouting_Filter1 == static_cast<int>(FILTER1ROUTE::FILTER1ROUTE_Filter2)) ||
					(*m_Set->m_State->m_uFilterRouting2_Filter1 == static_cast<int>(FILTER1ROUTE::FILTER1ROUTE_Filter2)) ||
					(*m_Set->m_State->m_uFilterRouting_Filter1 == static_cast<int>(FILTER1ROUTE::FILTER1ROUTE_Filter3)) ||
					(*m_Set->m_State->m_uFilterRouting2_Filter1 == static_cast<int>(FILTER1ROUTE::FILTER1ROUTE_Filter3)))
					hasNextFilter = true;
			if (filter == 1)
				if ((*m_Set->m_State->m_uFilterRouting_Filter2 == static_cast<int>(FILTER1ROUTE::FILTER1ROUTE_Filter3)) ||
					(*m_Set->m_State->m_uFilterRouting2_Filter2 == static_cast<int>(FILTER1ROUTE::FILTER1ROUTE_Filter3)))
					hasNextFilter = true;

			for (auto* voice : voices) {
                if (voice == nullptr) {
                    vassertfalse;
                    continue;
                }
				if (voice->mVoiceNo >= m_Set->m_uMaxPoly) {
					continue; //safety
				}
				int mVoiceNo = voice->getVoiceNo();
				l_inputState.voiceNo = mVoiceNo;
				if (((CVASTSingleNote*)voice)->m_bLastFilterOutputZero[filter] == false)
					bAllVoicesLastOutputZero = false;

				routingBuffers.FilterVoices[filter][mVoiceNo]->clear(startSample, numSamples);

				bool bPlayiningInRange = ((CVASTSingleNote*)voice)->isPlayingInRange(startSample, numSamples); //requires that mseg is processed before!																		
				if (bPlayiningInRange || (((CVASTSingleNote*)voice)->m_bLastFilterOutputZero[filter] == false)) { //perf opt, check with phase and frequency hat is not updated?
					numPlaying++;
					//dry wet -- add dry before processing -- add part that is not processed
					l_inputState.currentFrame = startSample;
					float dryWet = 0.f;
					if (filter == 0)
						dryWet = m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fFilterDryWet_Filter1, static_cast<int>(MODMATDEST::Filter1Mixin), &l_inputState) * 0.01f;
					else if (filter == 1)
						dryWet = m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fFilterDryWet_Filter2, static_cast<int>(MODMATDEST::Filter2Mixin), &l_inputState) * 0.01f;
					else if (filter == 2)
						dryWet = m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fFilterDryWet_Filter3, static_cast<int>(MODMATDEST::Filter3Mixin), &l_inputState) * 0.01f;

					//if (bPlayiningInRange) {
					if ((*m_Set->m_State->m_bOscOnOff_OscA == static_cast<int>(SWITCH::SWITCH_ON)) &&
						((*m_Set->m_State->m_uOscRouting1_OscA == fR) || (*m_Set->m_State->m_uOscRouting2_OscA == fR))) {
						routingBuffers.FilterVoices[filter][mVoiceNo]->addFrom(0, startSample, routingBuffers.OscVoices[0][mVoiceNo]->getReadPointer(0, startSample), numSamples, dryWet);
						routingBuffers.FilterVoices[filter][mVoiceNo]->addFrom(1, startSample, routingBuffers.OscVoices[0][mVoiceNo]->getReadPointer(1, startSample), numSamples, dryWet);
						bAddedSomething = true;
					}
					if ((*m_Set->m_State->m_bOscOnOff_OscB == static_cast<int>(SWITCH::SWITCH_ON)) &&
						((*m_Set->m_State->m_uOscRouting1_OscB == fR) || (*m_Set->m_State->m_uOscRouting2_OscB == fR))) {
						routingBuffers.FilterVoices[filter][mVoiceNo]->addFrom(0, startSample, routingBuffers.OscVoices[1][mVoiceNo]->getReadPointer(0, startSample), numSamples, dryWet);
						routingBuffers.FilterVoices[filter][mVoiceNo]->addFrom(1, startSample, routingBuffers.OscVoices[1][mVoiceNo]->getReadPointer(1, startSample), numSamples, dryWet);
						bAddedSomething = true;
					}
					if ((*m_Set->m_State->m_bOscOnOff_OscC == static_cast<int>(SWITCH::SWITCH_ON)) &&
						((*m_Set->m_State->m_uOscRouting1_OscC == fR) || (*m_Set->m_State->m_uOscRouting2_OscC == fR))) {
						routingBuffers.FilterVoices[filter][mVoiceNo]->addFrom(0, startSample, routingBuffers.OscVoices[2][mVoiceNo]->getReadPointer(0, startSample), numSamples, dryWet);
						routingBuffers.FilterVoices[filter][mVoiceNo]->addFrom(1, startSample, routingBuffers.OscVoices[2][mVoiceNo]->getReadPointer(1, startSample), numSamples, dryWet);
						bAddedSomething = true;
					}
					if ((*m_Set->m_State->m_bOscOnOff_OscD == static_cast<int>(SWITCH::SWITCH_ON)) &&
						((*m_Set->m_State->m_uOscRouting1_OscD == fR) || (*m_Set->m_State->m_uOscRouting2_OscD == fR))) {
						routingBuffers.FilterVoices[filter][mVoiceNo]->addFrom(0, startSample, routingBuffers.OscVoices[3][mVoiceNo]->getReadPointer(0, startSample), numSamples, dryWet);
						routingBuffers.FilterVoices[filter][mVoiceNo]->addFrom(1, startSample, routingBuffers.OscVoices[3][mVoiceNo]->getReadPointer(1, startSample), numSamples, dryWet);
						bAddedSomething = true;
					}
					if ((*m_Set->m_State->m_bNoiseOnOff == static_cast<int>(SWITCH::SWITCH_ON)) &&
						((*m_Set->m_State->m_uNoiseRouting1 == fR) || (*m_Set->m_State->m_uNoiseRouting2 == fR))) {
						routingBuffers.FilterVoices[filter][mVoiceNo]->addFrom(0, startSample, routingBuffers.NoiseVoices[mVoiceNo]->getReadPointer(0, startSample), numSamples, dryWet);
						routingBuffers.FilterVoices[filter][mVoiceNo]->addFrom(1, startSample, routingBuffers.NoiseVoices[mVoiceNo]->getReadPointer(1, startSample), numSamples, dryWet);
						bAddedSomething = true;
					}
					if ((*m_Set->m_State->m_bSamplerOnOff == static_cast<int>(SWITCH::SWITCH_ON)) &&
						((*m_Set->m_State->m_uSamplerRouting1 == fR) || (*m_Set->m_State->m_uSamplerRouting2 == fR))) {
						routingBuffers.FilterVoices[filter][mVoiceNo]->addFrom(0, startSample, routingBuffers.SamplerVoices[mVoiceNo]->getReadPointer(0, startSample), numSamples, dryWet);
						routingBuffers.FilterVoices[filter][mVoiceNo]->addFrom(1, startSample, routingBuffers.SamplerVoices[mVoiceNo]->getReadPointer(1, startSample), numSamples, dryWet);
						bAddedSomething = true;
					}
					if ((*m_Set->m_State->m_bOnOff_Filter1 == static_cast<int>(SWITCH::SWITCH_ON)) &&
						(*m_Set->m_State->m_uFilterRouting_Filter1 == frFilter1)) {
						routingBuffers.FilterVoices[filter][mVoiceNo]->addFrom(0, startSample, routingBuffers.FilterVoices[0][mVoiceNo]->getReadPointer(0, startSample), numSamples, dryWet);
						routingBuffers.FilterVoices[filter][mVoiceNo]->addFrom(1, startSample, routingBuffers.FilterVoices[0][mVoiceNo]->getReadPointer(1, startSample), numSamples, dryWet);
						bAddedSomething = true;
					}
					if ((*m_Set->m_State->m_bOnOff_Filter1 == static_cast<int>(SWITCH::SWITCH_ON)) &&
						(*m_Set->m_State->m_uFilterRouting2_Filter1 == frFilter1)) {
						routingBuffers.FilterVoices[filter][mVoiceNo]->addFrom(0, startSample, routingBuffers.FilterVoices[0][mVoiceNo]->getReadPointer(0, startSample), numSamples, dryWet);
						routingBuffers.FilterVoices[filter][mVoiceNo]->addFrom(1, startSample, routingBuffers.FilterVoices[0][mVoiceNo]->getReadPointer(1, startSample), numSamples, dryWet);
						bAddedSomething = true;
					}
					if ((*m_Set->m_State->m_bOnOff_Filter2 == static_cast<int>(SWITCH::SWITCH_ON)) &&
						(*m_Set->m_State->m_uFilterRouting_Filter2 == frFilter2)) {
						routingBuffers.FilterVoices[filter][mVoiceNo]->addFrom(0, startSample, routingBuffers.FilterVoices[1][mVoiceNo]->getReadPointer(0, startSample), numSamples, dryWet);
						routingBuffers.FilterVoices[filter][mVoiceNo]->addFrom(1, startSample, routingBuffers.FilterVoices[1][mVoiceNo]->getReadPointer(1, startSample), numSamples, dryWet);
						bAddedSomething = true;
					}
					if ((*m_Set->m_State->m_bOnOff_Filter2 == static_cast<int>(SWITCH::SWITCH_ON)) &&
						(*m_Set->m_State->m_uFilterRouting2_Filter2 == frFilter2)) {
						routingBuffers.FilterVoices[filter][mVoiceNo]->addFrom(0, startSample, routingBuffers.FilterVoices[1][mVoiceNo]->getReadPointer(0, startSample), numSamples, dryWet);
						routingBuffers.FilterVoices[filter][mVoiceNo]->addFrom(1, startSample, routingBuffers.FilterVoices[1][mVoiceNo]->getReadPointer(1, startSample), numSamples, dryWet);
						bAddedSomething = true;
					}
				}
			}

			if ((bAddedSomething == false) && (bAllVoicesLastOutputZero == true)) {
				//do nothing
				//VDBG("((bAddedSomething == false) && (bAllVoicesLastOutputZero == true)) skip filter processing");
			}
			else {

				//process filter									
				l_inputState.currentFrame = startSample;
				qfilterbuffer.setSize(2, numSamples, false, true, true);
				qfilterbuffer.clear();
				dsp::AudioBlock<float> filterInput(qfilterbuffer);

				int numVoices = 0;
				if (numPlaying > 0)
					numVoices = m_Poly->m_QFilter.processBlock(&voices, &l_inputState, &routingBuffers, m_Set, filter, filterInput, startSample, numSamples, false, hasNextFilter, nullptr, false); //no warmup

				 //what is that??
				routingBuffers.FilterBuffer[filter]->addFrom(0, startSample,
					filterInput.getChannelPointer(0), numSamples);
				routingBuffers.FilterBuffer[filter]->addFrom(1, startSample,
					filterInput.getChannelPointer(1), numSamples);

				/* //does not work
				Range<float> rangeL = routingBuffers.FilterBuffer[filter]->findMinMax(0, startSample, numSamples);
				Range<float> rangeR = routingBuffers.FilterBuffer[filter]->findMinMax(1, startSample, numSamples);
				if (!(
					((abs(rangeL.getStart()) <= 0.00001f) && (abs(rangeL.getEnd()) <= 0.00001f)) &&
					((abs(rangeR.getStart()) <= 0.00001f) && (abs(rangeR.getEnd()) <= 0.00001f))
					)) {
					anyFilterContent[filter][0] = true; //check
				}
				*/

				//post drywet
				for (auto* voice : voices) {
                    if (voice == nullptr) {
                        vassertfalse;
                        continue;
                    }
					if (voice->mVoiceNo >= m_Set->m_uMaxPoly) {
						continue; //safety
					}
					int mVoiceNo = voice->getVoiceNo();
					l_inputState.currentFrame = startSample;
					l_inputState.voiceNo = mVoiceNo;

					bool bPlayiningInRange = ((CVASTSingleNote*)voice)->isPlayingInRange(startSample, numSamples); //requires that mseg is processed before!
					if (bPlayiningInRange || (((CVASTSingleNote*)voice)->m_bLastFilterOutputZero[filter] == false)) { //perf opt, check with phase and frequency hat is not updated?

						//String filts = String(numVoices) + " m_bLastFilterOutputZero " + (((CVASTSingleNote*)voice)->m_bLastFilterOutputZero[filter] == true ? "true" : "false");
						//VDBG("voice " + String(mVoiceNo) + "filter " + String(filter) + "num playing " + String(numPlaying) + " numVoices " + filts);

						float fR = 0.f;
						float frFilter1 = 0.f;
						float frFilter2 = 0.f;
						switch (filter)
						{
						case 0:
							fR = OSCROUTE::OSCROUTE_Filter1;
							frFilter1 = -1;
							frFilter2 = -1;
							break;
						case 1:
							fR = OSCROUTE::OSCROUTE_Filter2;
							frFilter1 = FILTER1ROUTE::FILTER1ROUTE_Filter2;
							frFilter2 = -1;
							break;
						case 2:
							fR = OSCROUTE::OSCROUTE_Filter3;
							frFilter1 = FILTER1ROUTE::FILTER1ROUTE_Filter3;
							frFilter2 = FILTER2ROUTE::FILTER2ROUTE_Filter3;
							break;
						}

						//dry wet -- add dry before processing -- add part that is not processed
						float dryWet = 0.f;
						l_inputState.currentFrame = startSample;
						if (filter == 0)
							dryWet = m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fFilterDryWet_Filter1, static_cast<int>(MODMATDEST::Filter1Mixin), &l_inputState) * 0.01f;
						else if (filter == 1)
							dryWet = m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fFilterDryWet_Filter2, static_cast<int>(MODMATDEST::Filter2Mixin), &l_inputState) * 0.01f;
						else if (filter == 2)
							dryWet = m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fFilterDryWet_Filter3, static_cast<int>(MODMATDEST::Filter3Mixin), &l_inputState) * 0.01f;

						Range<float> rangeL = routingBuffers.FilterVoices[filter][mVoiceNo]->findMinMax(0, startSample, numSamples);
						Range<float> rangeR = routingBuffers.FilterVoices[filter][mVoiceNo]->findMinMax(1, startSample, numSamples);
						if (!(
							((abs(rangeL.getStart()) <= 0.00001f) && (abs(rangeL.getEnd()) <= 0.00001f)) &&
							((abs(rangeR.getStart()) <= 0.00001f) && (abs(rangeR.getEnd()) <= 0.00001f))
							//((abs(rangeL.getStart()) <= 0.00000000001f) && (abs(rangeL.getEnd()) <= 0.00000000001f)) &&
							//((abs(rangeR.getStart()) <= 0.00000000001f) && (abs(rangeR.getEnd()) <= 0.00000000001f))
							)) {
							anyFilterContent[filter][mVoiceNo] = true;
						}
						else
							VDBG("No filter content anymore! Filter " << filter << " voice " << mVoiceNo);

						//post drywet
						if (dryWet != 1.f) {
							if ((*m_Set->m_State->m_uOscRouting1_OscA == fR) || (*m_Set->m_State->m_uOscRouting2_OscA == fR)) {
								routingBuffers.FilterBuffer[filter]->addFrom(0, startSample, routingBuffers.OscVoices[0][mVoiceNo]->getReadPointer(0, startSample), numSamples, 1.0f - dryWet);
								routingBuffers.FilterBuffer[filter]->addFrom(1, startSample, routingBuffers.OscVoices[0][mVoiceNo]->getReadPointer(1, startSample), numSamples, 1.0f - dryWet);
								if (hasNextFilter) {
									routingBuffers.FilterVoices[filter][mVoiceNo]->addFrom(0, startSample, routingBuffers.OscVoices[0][mVoiceNo]->getReadPointer(0, startSample), numSamples, 1.0f - dryWet);
									routingBuffers.FilterVoices[filter][mVoiceNo]->addFrom(1, startSample, routingBuffers.OscVoices[0][mVoiceNo]->getReadPointer(1, startSample), numSamples, 1.0f - dryWet);
								}
							}
							if ((*m_Set->m_State->m_uOscRouting1_OscB == fR) || (*m_Set->m_State->m_uOscRouting2_OscB == fR)) {
								routingBuffers.FilterBuffer[filter]->addFrom(0, startSample, routingBuffers.OscVoices[1][mVoiceNo]->getReadPointer(0, startSample), numSamples, 1.0f - dryWet);
								routingBuffers.FilterBuffer[filter]->addFrom(1, startSample, routingBuffers.OscVoices[1][mVoiceNo]->getReadPointer(1, startSample), numSamples, 1.0f - dryWet);
								if (hasNextFilter) {
									routingBuffers.FilterVoices[filter][mVoiceNo]->addFrom(0, startSample, routingBuffers.OscVoices[1][mVoiceNo]->getReadPointer(0, startSample), numSamples, 1.0f - dryWet);
									routingBuffers.FilterVoices[filter][mVoiceNo]->addFrom(1, startSample, routingBuffers.OscVoices[1][mVoiceNo]->getReadPointer(1, startSample), numSamples, 1.0f - dryWet);
								}
							}
							if ((*m_Set->m_State->m_uOscRouting1_OscC == fR) || (*m_Set->m_State->m_uOscRouting2_OscC == fR)) {
								routingBuffers.FilterBuffer[filter]->addFrom(0, startSample, routingBuffers.OscVoices[2][mVoiceNo]->getReadPointer(0, startSample), numSamples, 1.0f - dryWet);
								routingBuffers.FilterBuffer[filter]->addFrom(1, startSample, routingBuffers.OscVoices[2][mVoiceNo]->getReadPointer(1, startSample), numSamples, 1.0f - dryWet);
								if (hasNextFilter) {
									routingBuffers.FilterVoices[filter][mVoiceNo]->addFrom(0, startSample, routingBuffers.OscVoices[2][mVoiceNo]->getReadPointer(0, startSample), numSamples, 1.0f - dryWet);
									routingBuffers.FilterVoices[filter][mVoiceNo]->addFrom(1, startSample, routingBuffers.OscVoices[2][mVoiceNo]->getReadPointer(1, startSample), numSamples, 1.0f - dryWet);
								}
							}
							if ((*m_Set->m_State->m_uOscRouting1_OscD == fR) || (*m_Set->m_State->m_uOscRouting2_OscD == fR)) {
								routingBuffers.FilterBuffer[filter]->addFrom(0, startSample, routingBuffers.OscVoices[3][mVoiceNo]->getReadPointer(0, startSample), numSamples, 1.0f - dryWet);
								routingBuffers.FilterBuffer[filter]->addFrom(1, startSample, routingBuffers.OscVoices[3][mVoiceNo]->getReadPointer(1, startSample), numSamples, 1.0f - dryWet);
								if (hasNextFilter) {
									routingBuffers.FilterVoices[filter][mVoiceNo]->addFrom(0, startSample, routingBuffers.OscVoices[3][mVoiceNo]->getReadPointer(0, startSample), numSamples, 1.0f - dryWet);
									routingBuffers.FilterVoices[filter][mVoiceNo]->addFrom(1, startSample, routingBuffers.OscVoices[3][mVoiceNo]->getReadPointer(1, startSample), numSamples, 1.0f - dryWet);
								}
							}
							if ((*m_Set->m_State->m_uNoiseRouting1 == fR) || (*m_Set->m_State->m_uNoiseRouting2 == fR)) {
								routingBuffers.FilterBuffer[filter]->addFrom(0, startSample, routingBuffers.NoiseVoices[mVoiceNo]->getReadPointer(0, startSample), numSamples, 1.0f - dryWet);
								routingBuffers.FilterBuffer[filter]->addFrom(1, startSample, routingBuffers.NoiseVoices[mVoiceNo]->getReadPointer(1, startSample), numSamples, 1.0f - dryWet);
								if (hasNextFilter) {
									routingBuffers.FilterVoices[filter][mVoiceNo]->addFrom(0, startSample, routingBuffers.NoiseVoices[mVoiceNo]->getReadPointer(0, startSample), numSamples, 1.0f - dryWet);
									routingBuffers.FilterVoices[filter][mVoiceNo]->addFrom(1, startSample, routingBuffers.NoiseVoices[mVoiceNo]->getReadPointer(1, startSample), numSamples, 1.0f - dryWet);
								}
							}
							if ((*m_Set->m_State->m_uSamplerRouting1 == fR) || (*m_Set->m_State->m_uSamplerRouting2 == fR)) {
								routingBuffers.FilterBuffer[filter]->addFrom(0, startSample, routingBuffers.SamplerVoices[mVoiceNo]->getReadPointer(0, startSample), numSamples, 1.0f - dryWet);
								routingBuffers.FilterBuffer[filter]->addFrom(1, startSample, routingBuffers.SamplerVoices[mVoiceNo]->getReadPointer(1, startSample), numSamples, 1.0f - dryWet);
								if (hasNextFilter) {
									routingBuffers.FilterVoices[filter][mVoiceNo]->addFrom(0, startSample, routingBuffers.SamplerVoices[mVoiceNo]->getReadPointer(0, startSample), numSamples, 1.0f - dryWet);
									routingBuffers.FilterVoices[filter][mVoiceNo]->addFrom(1, startSample, routingBuffers.SamplerVoices[mVoiceNo]->getReadPointer(1, startSample), numSamples, 1.0f - dryWet);
								}
							}
							if (*m_Set->m_State->m_uFilterRouting_Filter1 == frFilter1) {
								routingBuffers.FilterBuffer[filter]->addFrom(0, startSample, routingBuffers.FilterVoices[0][mVoiceNo]->getReadPointer(0, startSample), numSamples, 1.0f - dryWet);
								routingBuffers.FilterBuffer[filter]->addFrom(1, startSample, routingBuffers.FilterVoices[0][mVoiceNo]->getReadPointer(1, startSample), numSamples, 1.0f - dryWet);
								if (hasNextFilter) {
									routingBuffers.FilterVoices[filter][mVoiceNo]->addFrom(0, startSample, routingBuffers.FilterVoices[0][mVoiceNo]->getReadPointer(0, startSample), numSamples, 1.0f - dryWet);
									routingBuffers.FilterVoices[filter][mVoiceNo]->addFrom(1, startSample, routingBuffers.FilterVoices[0][mVoiceNo]->getReadPointer(1, startSample), numSamples, 1.0f - dryWet);
								}
							}
							if (*m_Set->m_State->m_uFilterRouting2_Filter1 == frFilter1) {
								routingBuffers.FilterBuffer[filter]->addFrom(0, startSample, routingBuffers.FilterVoices[0][mVoiceNo]->getReadPointer(0, startSample), numSamples, 1.0f - dryWet);
								routingBuffers.FilterBuffer[filter]->addFrom(1, startSample, routingBuffers.FilterVoices[0][mVoiceNo]->getReadPointer(1, startSample), numSamples, 1.0f - dryWet);
								if (hasNextFilter) {
									routingBuffers.FilterVoices[filter][mVoiceNo]->addFrom(0, startSample, routingBuffers.FilterVoices[0][mVoiceNo]->getReadPointer(0, startSample), numSamples, 1.0f - dryWet);
									routingBuffers.FilterVoices[filter][mVoiceNo]->addFrom(1, startSample, routingBuffers.FilterVoices[0][mVoiceNo]->getReadPointer(1, startSample), numSamples, 1.0f - dryWet);
								}
							}
							if (*m_Set->m_State->m_uFilterRouting_Filter2 == frFilter2) {
								routingBuffers.FilterBuffer[filter]->addFrom(0, startSample, routingBuffers.FilterVoices[1][mVoiceNo]->getReadPointer(0, startSample), numSamples, 1.0f - dryWet);
								routingBuffers.FilterBuffer[filter]->addFrom(1, startSample, routingBuffers.FilterVoices[1][mVoiceNo]->getReadPointer(1, startSample), numSamples, 1.0f - dryWet);
								if (hasNextFilter) {
									routingBuffers.FilterVoices[filter][mVoiceNo]->addFrom(0, startSample, routingBuffers.FilterVoices[1][mVoiceNo]->getReadPointer(0, startSample), numSamples, 1.0f - dryWet);
									routingBuffers.FilterVoices[filter][mVoiceNo]->addFrom(1, startSample, routingBuffers.FilterVoices[1][mVoiceNo]->getReadPointer(1, startSample), numSamples, 1.0f - dryWet);
								}
							}
							if (*m_Set->m_State->m_uFilterRouting2_Filter2 == frFilter2) {
								routingBuffers.FilterBuffer[filter]->addFrom(0, startSample, routingBuffers.FilterVoices[1][mVoiceNo]->getReadPointer(0, startSample), numSamples, 1.0f - dryWet);
								routingBuffers.FilterBuffer[filter]->addFrom(1, startSample, routingBuffers.FilterVoices[1][mVoiceNo]->getReadPointer(1, startSample), numSamples, 1.0f - dryWet);
								if (hasNextFilter) {
									routingBuffers.FilterVoices[filter][mVoiceNo]->addFrom(0, startSample, routingBuffers.FilterVoices[1][mVoiceNo]->getReadPointer(0, startSample), numSamples, 1.0f - dryWet);
									routingBuffers.FilterVoices[filter][mVoiceNo]->addFrom(1, startSample, routingBuffers.FilterVoices[1][mVoiceNo]->getReadPointer(1, startSample), numSamples, 1.0f - dryWet);
								}
							}
						}
#ifdef _DEBUG
						float lastSampleL = 0.f;
						float lastSampleR = 0.f;
						for (int i = 0; i < numSamples; i++) {
							float sampleL = routingBuffers.FilterVoices[filter][mVoiceNo]->getSample(0, startSample + i);
							float sampleR = routingBuffers.FilterVoices[filter][mVoiceNo]->getSample(1, startSample + i);
							if (i > 0) {
								//vassert(abs(lastSampleL - sampleL) < 0.05f); //click 
								//vassert(abs(lastSampleR - sampleR) < 0.05f); //click 
							}
							lastSampleL = sampleL;
							lastSampleR = sampleR;
						}
#endif
					}
				}
			}
		}

		for (auto* voice : voices) {
            if (voice == nullptr) {
                vassertfalse;
                continue;
            }
			if (voice->mVoiceNo >= m_Set->m_uMaxPoly) {
				continue; //safety
			}
			if (anyFilterContent[filter][voice->getVoiceNo()] == true)
				((CVASTSingleNote*)voice)->m_bLastFilterOutputZero[filter] = false;
			else
				((CVASTSingleNote*)voice)->m_bLastFilterOutputZero[filter] = true;
		}
	}
	m_numVoicesPlaying = l_numVoicesPlaying;
	m_numOscsPlaying = l_numOscsPlaying;
}

//void VASTSynthesiser::handleMidiEventSmoothed(const MidiMessage& m) {
	//OwnedArray<LinearSmoothedValue<float>> midiEventSmoother;
//}

void VASTSynthesiser::handleMidiEvent(const MidiMessage& m)
{
	VDBG("handleMidiEvent: " << m.getDescription());
	const int channel = m.getChannel();
	
	if (m.isNoteOn())
	{
		if ((myProcessor->isMPEenabled()) && (channel == m_MPEMasterChannel)) {
			//dont do note on on master channnel
		}
		else {
			VDBG("NoteOn midinote: " << m.getNoteNumber());
			noteOn(channel, m.getNoteNumber(), m.getFloatVelocity());

			//set initial values for note: timbre
			if (myProcessor->isMPEenabled()) {
				m_fPitchBendZone_smoothed[channel].setCurrentAndTargetValue(m_fPitchBendZone_smoothed[channel].getTargetValue()); //force it

				for (auto* voice : voices) {
                    if (voice == nullptr) {
                        vassertfalse;
                        continue;
                    }
					if (voice->mVoiceNo >= m_Set->m_uMaxPoly) {
						continue; //safety
					}

					if (voice->isPlayingChannel(channel)) {
						voice->pitchWheelMoved(m_fPitchBendZone_smoothed[channel].getTargetValue(), false);
						voice->controllerMoved(74 /*timbre MSB*/, lastTimbreReceivedOnChannel[channel - 1].as7BitInt()); //7bit??
					}
				}
			}
		}
	}
	else if (m.isNoteOff())
	{
		if ((myProcessor->isMPEenabled()) && (channel == m_MPEMasterChannel)) {
			//dont do note on on master channnel
		}
		else
			noteOff(channel, m.getNoteNumber(), m.getFloatVelocity(), true);
	}
	else if (m.isAllNotesOff() || m.isAllSoundOff())
	{
		allNotesOff(channel, true);
	}
	else if (m.isPitchWheel())
	{
		const int wheelPos = m.getPitchWheelValue();
		if ((myProcessor->isMPEenabled()) && (channel == m_MPEMasterChannel)) {
			handlePitchWheel(channel, wheelPos);
		}
		else
			handlePitchWheel(channel, wheelPos);
	}
	else if (m.isAftertouch())
	{
		if ((myProcessor->isMPEenabled()) && (channel == m_MPEMasterChannel)) {
			//dont do on master channnel
		}
		else
			handleAftertouch(channel, m.getNoteNumber(), m.getAfterTouchValue());
	}
	else if (m.isChannelPressure())
	{
		if ((myProcessor->isMPEenabled()) && (channel == m_MPEMasterChannel)) {
			//dont do on master channnel
		}
		else
			handleChannelPressure(channel, m.getChannelPressureValue());
	}
	else if (m.isController())
	{
		if ((myProcessor->isMPEenabled()) && (channel == m_MPEMasterChannel)) {
			//dont do on master channnel
		}
		else
			handleController(channel, m.getControllerNumber(), m.getControllerValue());
	}
	else if (m.isProgramChange())
	{
		if ((myProcessor->isMPEenabled()) && (channel == m_MPEMasterChannel)) {
			//dont do on master channnel
		} 
		else
			handleProgramChange(channel, m.getProgramChangeNumber());
	}
}

//==============================================================================

void VASTSynthesiser::noteOn(const int midiChannel,
	const int midiNoteNumber,
	const float velocity)
{
	const ScopedLock sl(lock);
	
	if (m_midiNotesKeyDown[midiNoteNumber] == true) {
		//is already playing - can happen with MPE
		return;
	}

	m_midiNotesKeyDown[midiNoteNumber] = true;	
	m_midiNotesNumKeyDown++;

	struct timeval tp;
	m_Set->_gettimeofday(&tp);
	m_midiNotesKeyDownTime[midiNoteNumber] = tp.tv_sec * 1000 + tp.tv_usec / 1000;
	
	for (auto* sound : sounds)
	{
		if (sound->appliesToNote(midiNoteNumber) && sound->appliesToChannel(midiChannel))
		{
			//LFO retrigger
			m_Poly->resynchLFO();

			// If hitting a note that's still ringing, stop it first (it could be
			// still playing because of the sustain or sostenuto pedal).
			for (auto* voice : voices) {
                if (voice == nullptr) {
                    vassertfalse;
                    continue;
                }
				if (voice->mVoiceNo >= m_Set->m_uMaxPoly) {
					continue; //safety
				}

				//check for hard stop
				//CHECK IF THIS IS NEEDED!
				//if (((CVASTSingleNote*)voice)->m_VCA.isNoteOff() && !((CVASTSingleNote*)voice)->m_VCA.isHardStop())
				//	voice->clearCurrentNote();

				if (voice->getCurrentlyPlayingNote() == midiNoteNumber && voice->isPlayingChannel(midiChannel)) {				
					VDBG("Start same midi note");
					((CVASTSingleNote*)voice)->m_VCA.hardStop();
					//stopVoice(voice, 1.0f, true); //can be immediate here
				}
			}
			
			//==================================================================
			
			if ((m_Set->m_uMaxPoly) == 1) { //MONO MODE

				VASTSynthesiserVoice* voice = nullptr;

				bool shallSteal = false;
				if (*m_Set->m_State->m_bLegatoMode == static_cast<int>(SWITCH::SWITCH_OFF)) {
					for (auto* myvoice : voices) {
                        if (myvoice == nullptr) {
                            vassertfalse;
                            continue;
                        }
						if (myvoice->mVoiceNo >= m_Set->m_uMaxPoly) {
							continue; //safety
						}
						if (myvoice->isVoiceActive()) {
							((CVASTSingleNote*)myvoice)->m_VCA.hardStop(); //TODO Sampler???
						}
					}
					shallSteal = true; //check!! naming
					voice = findFreeVoice(sound, midiChannel, midiNoteNumber, shallSteal);
					if (voice == nullptr)
						continue;
					startVoice(voice, sound, midiChannel, midiNoteNumber, velocity);
				}
				else {
					//VDBG("Synthesizer noteOn Legato #notes down: " + String(m_midiNotesNumKeyDown));
					//if (m_midiNotesNumKeyDown == 1) { //

					//TEST
					//voice = voices[0];

					//Idea: assign a new voice but keep the one in release /tailing off
					//do everything only with voice not in release phase

					bool activeVoiceinReleaseExist = false;
					bool activeVoiceExist = false;
					for (auto* myvoice : voices) {
                        if (myvoice == nullptr) {
                            vassertfalse;
                            continue;
                        }
						if (myvoice->mVoiceNo >= m_Set->m_uMaxPoly) {
							continue; //safety
						}
						if (myvoice->isPlayingButReleased()) {
							activeVoiceinReleaseExist = true;
						}
						if (myvoice->isVoiceActive()) {
							activeVoiceExist = true;
						}
					}

					if (!activeVoiceinReleaseExist) {
						for (auto* myvoice : voices) {
                            if (myvoice == nullptr) {
                                vassertfalse;
                                continue;
                            }
							if (myvoice->mVoiceNo >= m_Set->m_uMaxPoly) {
								continue; //safety
							}
							if (activeVoiceExist) {
								if (myvoice->isVoiceActive()) {
									voice = myvoice;
									break;
								}
							}
							else {
								bool bHardStop = ((CVASTSingleNote*)myvoice)->m_VCA.isHardStop();
								if (!bHardStop) {
									voice = myvoice;
									break;
								}
							}
						}

						if (voice == nullptr) { //steal it 
							voice = voices[0];
							if (voice == nullptr)
								return; //error state
						}

					}
					else {
						for (auto* myvoice : voices) {
                            if (myvoice == nullptr) {
                                vassertfalse;
                                continue;
                            }
							if (myvoice->mVoiceNo >= m_Set->m_uMaxPoly) {
								continue; //safety
							}
							if (!myvoice->isVoiceActive()) {
								voice = myvoice;
								break;
							}
						}
					}

					for (auto* myvoice : voices) {
                        if (myvoice == nullptr) {
                            vassertfalse;
                            continue;
                        }
						if (myvoice->mVoiceNo >= m_Set->m_uMaxPoly) {
							continue; //safety
						}
						if (voice != myvoice)
							if (myvoice->isVoiceActive() && !((CVASTSingleNote*)myvoice)->m_VCA.isHardStop())
								myvoice->stopNote(0, false); //hard stop it
					}

					//CHECK
					if (voice == nullptr) { //steal it
						voice = voices[0];
						if (voice == nullptr)
							continue; //error state
					}

					shallSteal = true;
					startVoice(voice, sound, midiChannel, midiNoteNumber, velocity);
				}

				if ((!shallSteal) || (m_midiNotesNumKeyDown > 1))
					if (m_bGlissandoUsed) {
						bool sameStacks = true;
						for (int i = 0; i < 256; i++) {
							if (m_oldChordStack[i] != m_newChordStack[i]) {
								sameStacks = false;
								break;
							}
						}

						int newest = -1;
						ULong64_t newestTime = 0;
						for (int i = 0; i < 256; i++) {
							if ((m_oldChordStack[i] == true) &&
                               (m_newChordStack[i] == false)) { //looking for newest note in old chord stack!
								//jassert(m_midiNotesKeyDownTime[i] != 0);  //can happen if release
								if (m_midiNotesKeyDownTime[i] != 0) {
									if (m_midiNotesKeyDownTime[i] > newestTime) {
										newestTime = m_midiNotesKeyDownTime[i];
										newest = i;
									}
								}
							}
						}
						if (newest >= 0) {
							if (voice != nullptr) {
								((CVASTSingleNote*)voice)->setGlissandoStart(newest, false);
								m_iGlissandoAssigned++;
								VDBG("voice: " << voice->getVoiceNo() << " glissandoStart for newest note in old chord stack " << newest);
								break; //lowest old note
							}
						}
					}
			}
			else if ((m_Set->m_uMaxPoly) == 4) { //POLY 4 MODE
				int active = 0;
				bool shallSteal = false;
				for (auto* voice : voices) {
                    if (voice == nullptr) {
                        vassertfalse;
                        continue;
                    }
					if (voice->mVoiceNo >= m_Set->m_uMaxPoly) {
						continue; //safety
					}
					if (voice->isVoiceActive()) {
						active++;
					}
				}
				while (active > 3) {
					VASTSynthesiserVoice* stealVoice = findActiveVoiceToSteal(sound, midiChannel, midiNoteNumber);
					((CVASTSingleNote*)stealVoice)->m_VCA.hardStop();		
					stealVoice->currentlyPlayingNote = -1; 
					active--;
				}
				
				auto voice = findFreeVoice(sound, midiChannel, midiNoteNumber, shallSteal);
				startVoice(voice, sound, midiChannel, midiNoteNumber, velocity);

				if (m_bGlissandoUsed) {
					int assigned = 0;
					for (int i = 0; i < 256; i++)
						if ((m_oldChordStack[i] == true) && m_midiNotesKeyDown[i] == false) {
							assigned++;
							if ((assigned - 1) == m_iGlissandoAssigned) { //arbitrary order - good enough? dont know more here
								if (voice != nullptr) {
									((CVASTSingleNote*)voice)->setGlissandoStart(i, true);
									m_iGlissandoAssigned++;
									VDBG("voice: " << voice->getVoiceNo() << " glissandoStart for note  " << i);
									break; //lowest old note
								}
							}
						}
				}
			}
			else if (((m_Set->m_uMaxPoly) == 16) || (m_Set->m_uMaxPoly) == 32) {	//POLY 16 or POLY 32 MODES
				int active = 0;
				bool shallSteal = false;
				for (auto* voice : voices) {
                    if (voice == nullptr) {
                        vassertfalse;
                        continue;
                    }
					if (voice->mVoiceNo >= m_Set->m_uMaxPoly) {
						continue; //safety
					}
					if (voice->isVoiceActive()) {
						active++;
					}
				}
				while (active > m_Set->m_uMaxPoly - 1) { //no voice left
					VASTSynthesiserVoice* stealVoice = findActiveVoiceToSteal(sound, midiChannel, midiNoteNumber);
					((CVASTSingleNote*)stealVoice)->m_VCA.hardStop();
					stealVoice->currentlyPlayingNote = -1; //not playing
					active--;
				}

				auto voice = findFreeVoice(sound, midiChannel, midiNoteNumber, shallSteal);
				startVoice(voice, sound, midiChannel, midiNoteNumber, velocity);

				if (m_bGlissandoUsed) {
					int assigned = 0;
					for (int i = 0; i < 256; i++)
						if ((m_oldChordStack[i] == true) && m_midiNotesKeyDown[i] == false) {
							assigned++;
							if ((assigned - 1) == m_iGlissandoAssigned) { //arbitrary order - good enough? dont know more here
								if (voice != nullptr) {
									((CVASTSingleNote*)voice)->setGlissandoStart(i, true);
									m_iGlissandoAssigned++;
									VDBG("voice: " << voice->getVoiceNo() << " glissandoStart for note  " << i);
									break; //lowest old note
								}
							}
						}
				}
			}
		}
	}
}

void VASTSynthesiser::startVoice(VASTSynthesiserVoice* const voice,
	juce::SynthesiserSound* const sound,
	const int midiChannel,
	const int midiNoteNumber,
	const float velocity)
{
	if ((voice != nullptr) && (sound != nullptr))
	{
		bool bLegato = false;
		if ((*m_Set->m_State->m_bLegatoMode == static_cast<int>(SWITCH::SWITCH_OFF)) || (m_Set->m_uMaxPoly > 1)) {
			voice->stopNote(0.0f, false);
		}
		else { //mono legato mode
			if (midiNoteNumber != voice->currentlyPlayingNote) {
				if (m_midiNotesNumKeyDown > m_Set->m_uMaxPoly) {
					bLegato = true;
					VDBG("StartVoice Legato start (sustain) voice: " << voice->mVoiceNo);
				}
				else {
					VDBG("StartVoice start new voice: " << voice->mVoiceNo);
				}
			}
			else {
				VDBG("StartVoice start new voice: " << voice->mVoiceNo);
			}
		}
				   		 	  
		voice->currentlyPlayingNote = midiNoteNumber;
		voice->currentPlayingMidiChannel = midiChannel;
		voice->noteOnTime = ++lastNoteOnCounter;
		voice->currentlyPlayingSound = sound;
		voice->setKeyDown(true);
		voice->setSostenutoPedalDown(false);
		voice->setSustainPedalDown(sustainPedalsDown[midiChannel]);

		float lastpwval = 0.f;
		if (myProcessor->isMPEenabled())
			lastpwval = lastPitchWheelValues[m_MPEMasterChannel];
		else 
			lastpwval = lastPitchWheelValues[midiChannel];
		voice->startNote(midiNoteNumber, velocity * 127.f, sound,
			lastpwval, bLegato);

		//single mseg
		for (int mseg = 4; mseg >= 0; mseg--) {
			bool msegpervoice = false;
			if ((mseg == 0) && (*m_Set->m_State->m_bMSEGPerVoice_MSEG1 == static_cast<int>(SWITCH::SWITCH_ON)))
				msegpervoice = true;
			if ((mseg == 1) && (*m_Set->m_State->m_bMSEGPerVoice_MSEG2 == static_cast<int>(SWITCH::SWITCH_ON)))
				msegpervoice = true;
			if ((mseg == 2) && (*m_Set->m_State->m_bMSEGPerVoice_MSEG3 == static_cast<int>(SWITCH::SWITCH_ON)))
				msegpervoice = true;
			if ((mseg == 3) && (*m_Set->m_State->m_bMSEGPerVoice_MSEG4 == static_cast<int>(SWITCH::SWITCH_ON)))
				msegpervoice = true;
			if ((mseg == 4) && (*m_Set->m_State->m_bMSEGPerVoice_MSEG5 == static_cast<int>(SWITCH::SWITCH_ON)))
				msegpervoice = true;
			if (!msegpervoice) {
				if ((m_oldestPlayingKeyDown >= 0) && voices[m_oldestPlayingKeyDown]->isKeyDown()) { 
					VDBG("Single MSEG Copy m_oldestPlayingKeyDown: " << m_oldestPlayingKeyDown << " -> " << voice->mVoiceNo << " keyDown: " << (voices[m_oldestPlayingKeyDown]->isKeyDown() ? "true" : "false"));
					if (!(m_oldestPlayingKeyDown >= m_Set->m_uMaxPoly)) //safety check
						((CVASTSingleNote*)voice)->m_VCA.m_MSEG_Envelope[mseg].copyStateFrom(((CVASTSingleNote*)voices[m_oldestPlayingKeyDown])->m_VCA.m_MSEG_Envelope[mseg]);
					else
						vassertfalse; //errorstate
				}
			}
		}
	}
	else {
		VDBG("No voice and sound here");
		vassertfalse;
	}
}

void VASTSynthesiser::stopVoice(VASTSynthesiserVoice* voice, float velocity, const bool allowTailOff)
{
	jassert(voice != nullptr);

	VDBG("StopVoice " << voice->mVoiceNo);
    
	//int iLegatoNote = 0;
	if ((*m_Set->m_State->m_bLegatoMode == static_cast<int>(SWITCH::SWITCH_ON)) && m_Set->m_uMaxPoly == 1) { //mono legato
		int keyDown = -1;
		for (int i = numElementsInArray(m_midiNotesKeyDown) - 1; i >= 0; --i) {
			if ((m_midiNotesKeyDown[i] == true) && (m_midiNotesKeyDown[voice->currentlyPlayingNote] != true)) { //CHECK
				keyDown = i;
				break;
			}
		}
		if (keyDown == -1) {
			voice->stopNote(velocity, allowTailOff);
		}
		else {
			int oldNote = voice->currentlyPlayingNote;
			m_oldChordStack[oldNote] = false; //CHECK
			voice->currentlyPlayingNote = keyDown;
			voice->startNote(keyDown, velocity, voice->getCurrentlyPlayingSound(), lastPitchWheelValues[voice->currentPlayingMidiChannel], true); //legato
			((CVASTSingleNote*)voice)->setGlissandoStart(oldNote, false); //CHECK
		}
	}
	else 
		voice->stopNote(velocity, allowTailOff);

	// the subclass MUST call clearCurrentNote() if it's not tailing off! RTFM for stopNote()!
	jassert(allowTailOff || (voice->getCurrentlyPlayingNote() < 0 && voice->getCurrentlyPlayingSound() == nullptr));
}

void VASTSynthesiser::noteOff(const int midiChannel,
	const int midiNoteNumber,
	const float velocity,
	const bool allowTailOff)
{
	const ScopedLock sl(lock);
    
	if (m_midiNotesKeyDown[midiNoteNumber] == true) {
		m_midiNotesKeyDown[midiNoteNumber] = false;
		m_midiNotesKeyDownTime[midiNoteNumber] = 0;
		m_midiNotesNumKeyDown--;
	}
#ifdef _DEBUG
	else {
		VDBG("Duplicate noteOff send from DAW!");
	}
#endif

	for (auto* voice : voices)
	{
        if (voice == nullptr) {
            vassertfalse;
            continue;
        }
		if (voice->mVoiceNo >= m_Set->m_uMaxPoly) {
			continue; //safety
		}
		if (voice->getCurrentlyPlayingNote() == midiNoteNumber
			//&& voice->isPlayingChannel(midiChannel))
			)
		{
			if (juce::SynthesiserSound* const sound = voice->getCurrentlyPlayingSound())
			{
				if (sound->appliesToNote(midiNoteNumber))
					//&& sound->appliesToChannel(midiChannel))
				{
					jassert(!voice->keyIsDown || voice->isSustainPedalDown() == sustainPedalsDown[midiChannel]);

					voice->setKeyDown(false);

					if (!(voice->isSustainPedalDown() || voice->isSostenutoPedalDown())) {
						stopVoice(voice, velocity, allowTailOff);
						//stoppedVoice = voice;
					}
				}
			}
		}
	}
}

void VASTSynthesiser::allNotesOff(const int midiChannel, const bool allowTailOff)
{
	const ScopedLock sl(lock);

	for (auto* voice : voices) {
        if (voice == nullptr) {
            vassertfalse;
            continue;
        }
		if (voice->mVoiceNo >= m_Set->m_uMaxPoly) {
			continue; //safety
		}
		//if (midiChannel <= 0 || voice->isPlayingChannel(midiChannel))
		//if (voice->isPlayingChannel(midiChannel))
		voice->stopNote(1.0f, allowTailOff);
	}

	sustainPedalsDown.clear();
}

void VASTSynthesiser::handlePitchWheel(const int midiChannel, const int wheelValue) ////0..16384
{
	const ScopedLock sl(lock);
	lastPitchWheelUIValue.store(wheelValue);
	lastPitchWheelValues[midiChannel].store(wheelValue);

	m_fPitchBendZone_smoothed[midiChannel].setTargetValue(wheelValue);
}

void VASTSynthesiser::handleController(const int midiChannel,
	const int controllerNumber,
	const int controllerValue)
{	
	if (midiChannel > C_MAX_POLY) //safety check for MPE
		return; 

	const uint8 noLSBValueReceived = 0xff;
	switch (controllerNumber)
	{		
	case 0x00: /*00*/ handleProgramChange(midiChannel, controllerValue); break; //CC00 is bank change
	case 0x40: /*64*/ handleSustainPedal(midiChannel, controllerValue >= 64); break;
	case 0x42: /*66*/ handleSostenutoPedal(midiChannel, controllerValue >= 64); break;
	case 0x43: /*67*/ handleSoftPedal(midiChannel, controllerValue >= 64); break;
	case 0x4a: /*74*/ //handleTimbreMSB 
	{
		auto lsb = lastPressureLowerBitReceivedOnChannel[midiChannel - 1];
		lastTimbreReceivedOnChannel[midiChannel - 1] = (lsb == noLSBValueReceived ? MPEValue::from7BitInt(controllerValue) : MPEValue::from14BitInt(lsb + (controllerValue << 7)));
		break;
	}
	case 0x6a: /*106*/ //handleTimbreLSB
		lastPressureLowerBitReceivedOnChannel[midiChannel - 1] = uint8(controllerValue); break;
	default:    break;
	}

	if (controllerNumber == 1) { // MIDI CC 1 Modulation, e.g. VSTHost
		m_fModWheel_smoothed[midiChannel].setTargetValue(controllerValue);

		//check for permalink
		int permalink = myProcessor->getModWheelPermaLink();

		if (permalink != 0) {
			float l_modWheelTargetPos = m_fModWheel_smoothed[midiChannel].getTargetValue();

			if (((permalink == 1) && (m_CustomMod1Param == nullptr)) ||
				((permalink == 2) && (m_CustomMod2Param == nullptr)) ||
				((permalink == 3) && (m_CustomMod3Param == nullptr)) ||
				((permalink == 4) && (m_CustomMod4Param == nullptr))) {

				Array<AudioProcessorParameter*> params = myProcessor->getParameters();
				for (int i = 0; i < params.size(); i++) {
					if ((permalink == 1) && (params[i]->getName(18).equalsIgnoreCase("Custom modulator 1"))) {
						m_CustomMod1Param = params[i];
						break;
					}
					if ((permalink == 2) && (params[i]->getName(18).equalsIgnoreCase("Custom modulator 2"))) {
						m_CustomMod2Param = params[i];
						break;
					}
					if ((permalink == 3) && (params[i]->getName(18).equalsIgnoreCase("Custom modulator 3"))) {
						m_CustomMod3Param = params[i];
						break;
					}
					if ((permalink == 4) && (params[i]->getName(18).equalsIgnoreCase("Custom modulator 4"))) {
						m_CustomMod4Param = params[i];
						break;
					}
				}
			}

			if (permalink == 1) {
				m_CustomMod1Param->setValueNotifyingHost(l_modWheelTargetPos / 127.f);
			}
			else if (permalink == 2) {
				m_CustomMod2Param->setValueNotifyingHost(l_modWheelTargetPos / 127.f);
			}
			else if (permalink == 3) {
				m_CustomMod3Param->setValueNotifyingHost(l_modWheelTargetPos / 127.f);
			}
			else if (permalink == 4) {
				m_CustomMod4Param->setValueNotifyingHost(l_modWheelTargetPos / 127.f);
			}
		}
	}
	else {
		const ScopedLock sl(lock);

		for (auto* voice : voices) {
            if (voice == nullptr) {
                vassertfalse;
                continue;
            }
			if (voice->mVoiceNo >= m_Set->m_uMaxPoly) {
				continue; //safety
			}
			//if (midiChannel <= 0 || voice->isPlayingChannel(midiChannel))		
			if (myProcessor->isMPEenabled()) {
				if (voice->isPlayingChannel(midiChannel))
					voice->controllerMoved(controllerNumber, controllerValue);
			}
			else
				voice->controllerMoved(controllerNumber, controllerValue);
		}
	}
}

void VASTSynthesiser::handleAftertouch(int midiChannel, int midiNoteNumber, int aftertouchValue)
{
	const ScopedLock sl(lock);

	for (auto* voice : voices) { //polyphonic
        if (voice == nullptr) {
            vassertfalse;
            continue;
        }
		if (voice->mVoiceNo >= m_Set->m_uMaxPoly) {
			continue; //safety
		}

		if (myProcessor->isMPEenabled()) {
			if (voice->isPlayingChannel(midiChannel))
				voice->aftertouchChanged(aftertouchValue);
		}
		else {
			if (voice->getCurrentlyPlayingNote() == midiNoteNumber)
				voice->aftertouchChanged(aftertouchValue);
		}
	}
}

void VASTSynthesiser::handleChannelPressure(int midiChannel, int channelPressureValue)
{
	const ScopedLock sl(lock);

	for (auto* voice : voices) {
        if (voice == nullptr) {
            vassertfalse;
            continue;
        }
		if (voice->mVoiceNo >= m_Set->m_uMaxPoly) {
			continue; //safety
		}

		//if (midiChannel <= 0 || voice->isPlayingChannel(midiChannel))

		if (myProcessor->isMPEenabled()) {
			if (voice->isPlayingChannel(midiChannel))
				voice->channelPressureChanged(channelPressureValue);
		}
		else
			voice->channelPressureChanged(channelPressureValue);
	}
}

void VASTSynthesiser::handleSustainPedal(int midiChannel, bool isDown)
{
	jassert(midiChannel > 0 && midiChannel <= 16);
	const ScopedLock sl(lock);

	if (isDown)
	{
		sustainPedalsDown.setBit(midiChannel);

		for (auto* voice : voices) {
            if (voice == nullptr) {
                vassertfalse;
                continue;
            }
			if (voice->mVoiceNo >= m_Set->m_uMaxPoly) {
				continue; //safety
			}

			//if (voice->isPlayingChannel(midiChannel) && voice->isKeyDown())
			if (voice->isKeyDown())
				voice->setSustainPedalDown(true);
		}
	}
	else
	{
		for (auto* voice : voices)
		{
            if (voice == nullptr) {
                vassertfalse;
                continue;
            }
			if (voice->mVoiceNo >= m_Set->m_uMaxPoly) {
				continue; //safety
			}

			if (voice->isPlayingChannel(midiChannel))
			{
				voice->setSustainPedalDown(false);

				if (!(voice->isKeyDown() || voice->isSostenutoPedalDown()))
					stopVoice(voice, 1.0f, true);
			}
		}

		sustainPedalsDown.clearBit(midiChannel);
	}
}

void VASTSynthesiser::handleSostenutoPedal(int midiChannel, bool isDown)
{
	jassert(midiChannel > 0 && midiChannel <= 16);
	const ScopedLock sl(lock);

	for (auto* voice : voices)
	{
        if (voice == nullptr) {
            vassertfalse;
            continue;
        }
		if (voice->mVoiceNo >= m_Set->m_uMaxPoly) {
			continue; //safety
		}

		if (voice->isPlayingChannel(midiChannel))
		{
			if (isDown)
				voice->setSostenutoPedalDown(true);
			else if (voice->isSostenutoPedalDown())
				stopVoice(voice, 1.0f, true);
		}
	}
}

void VASTSynthesiser::handleSoftPedal(int midiChannel, bool /*isDown*/)
{
	ignoreUnused(midiChannel);
	jassert(midiChannel > 0 && midiChannel <= 16);
}

void VASTSynthesiser::handleProgramChange(int midiChannel, int programNumber)
{
	jassert(midiChannel > 0 && midiChannel <= 16);

	int presetindex = myProcessor->m_presetData.bankProgramGetPresetIndex(myProcessor->m_pVASTXperience.getMidiBank() , programNumber);
	if (presetindex >= 0) {
		myProcessor->setCurrentProgram(presetindex); //will set block process
		VDBG("Program Change " << presetindex);
	}
}

//==============================================================================
VASTSynthesiserVoice* VASTSynthesiser::findFreeVoice(juce::SynthesiserSound* soundToPlay,
	int midiChannel, int midiNoteNumber,
	const bool stealIfNoneAvailable) const
{
	const ScopedLock sl(lock);

	for (auto* voice : voices) {
        if (voice == nullptr) {
            vassertfalse;
            continue;
        }
		if (voice->mVoiceNo >= m_Set->m_uMaxPoly) {
			continue; //safety
		}
		if ((!voice->isVoiceActive()) && voice->canPlaySound(soundToPlay))
			return voice;
	}

	if (stealIfNoneAvailable)
		return findVoiceToSteal(soundToPlay, midiChannel, midiNoteNumber);

	return nullptr;
}

VASTSynthesiserVoice* VASTSynthesiser::findVoiceToSteal(juce::SynthesiserSound* soundToPlay,
	int /*midiChannel*/, int midiNoteNumber) const
{
	// This voice-stealing algorithm applies the following heuristics:
	// - Re-use the oldest notes first
	// - Protect the lowest & topmost notes, even if sustained, but not if they've been released.

	// apparently you are trying to render audio without having any voices...
	jassert(!voices.isEmpty());

	// These are the voices we want to protect (ie: only steal if unavoidable)
	VASTSynthesiserVoice* low = nullptr; // Lowest sounding note, might be sustained, but NOT in release phase
	VASTSynthesiserVoice* top = nullptr; // Highest sounding note, might be sustained, but NOT in release phase

										 // this is a list of voices we can steal, sorted by how long they've been running
	Array<VASTSynthesiserVoice*> usableVoices;
	usableVoices.ensureStorageAllocated(voices.size());

	for (auto* voice : voices)
	{
        if (voice == nullptr) {
            vassertfalse;
            continue;
        }
		if (voice->mVoiceNo >= m_Set->m_uMaxPoly) {
			continue; //safety
		}
		if (voice->canPlaySound(soundToPlay))
		{
			jassert(voice->isVoiceActive()); // We wouldn't be here otherwise

			usableVoices.add(voice);

			// NB: Using a functor rather than a lambda here due to scare-stories about
			// compilers generating code containing heap allocations..
			struct Sorter
			{
				bool operator() (const VASTSynthesiserVoice* a, const VASTSynthesiserVoice* b) const noexcept { return a->wasStartedBefore(*b); }
			};

			std::sort(usableVoices.begin(), usableVoices.end(), Sorter());

			if (!voice->isPlayingButReleased()) // Don't protect released notes
			{
				auto note = voice->getCurrentlyPlayingNote();

				if (low == nullptr || note < low->getCurrentlyPlayingNote())
					low = voice;

				if (top == nullptr || note > top->getCurrentlyPlayingNote())
					top = voice;
			}
		}
	}

	// Eliminate pathological cases (ie: only 1 note playing): we always give precedence to the lowest note(s)
	if (top == low)
		top = nullptr;

	// The oldest note that's playing with the target pitch is ideal..
	for (auto* voice : usableVoices)
		if (voice->getCurrentlyPlayingNote() == midiNoteNumber)
			return voice;

	// Oldest voice that has been released (no finger on it and not held by sustain pedal)
	for (auto* voice : usableVoices)
		if (voice != low && voice != top && voice->isPlayingButReleased())
			return voice;

	// Oldest voice that doesn't have a finger on it:
	for (auto* voice : usableVoices)
		if (voice != low && voice != top && !voice->isKeyDown())
			return voice;

	// Oldest voice that isn't protected
	for (auto* voice : usableVoices)
		if (voice != low && voice != top)
			return voice;

	// We've only got "protected" voices now: lowest note takes priority
	//jassert(low != nullptr);

	// Duophonic synth: give priority to the bass note:
	if (top != nullptr)
		return top;

	return low;
}


VASTSynthesiserVoice* VASTSynthesiser::findActiveVoiceToSteal(juce::SynthesiserSound* soundToPlay,
	int /*midiChannel*/, int midiNoteNumber) const
{
	// This voice-stealing algorithm applies the following heuristics:
	// - Re-use the oldest notes first
	// - Protect the lowest & topmost notes, even if sustained, but not if they've been released.

	// apparently you are trying to render audio without having any voices...
	jassert(!voices.isEmpty());

	// These are the voices we want to protect (ie: only steal if unavoidable)
	VASTSynthesiserVoice* low = nullptr; // Lowest sounding note, might be sustained, but NOT in release phase
	VASTSynthesiserVoice* top = nullptr; // Highest sounding note, might be sustained, but NOT in release phase

										 // this is a list of voices we can steal, sorted by how long they've been running
	Array<VASTSynthesiserVoice*> usableVoices;
	usableVoices.ensureStorageAllocated(voices.size());

	for (auto* voice : voices)
	{
        if (voice == nullptr) {
            vassertfalse;
            continue;
        }
		if (voice->mVoiceNo >= m_Set->m_uMaxPoly) {
			continue; //safety
		}

		if (voice->canPlaySound(soundToPlay) && voice->isVoiceActive())
		{
			usableVoices.add(voice);

			// NB: Using a functor rather than a lambda here due to scare-stories about
			// compilers generating code containing heap allocations..
			struct Sorter
			{
				bool operator() (const VASTSynthesiserVoice* a, const VASTSynthesiserVoice* b) const noexcept { return a->wasStartedBefore(*b); }
			};

			std::sort(usableVoices.begin(), usableVoices.end(), Sorter());

			if (!voice->isPlayingButReleased()) // Don't protect released notes
			{
				auto note = voice->getCurrentlyPlayingNote();

				if (low == nullptr || note < low->getCurrentlyPlayingNote())
					low = voice;

				if (top == nullptr || note > top->getCurrentlyPlayingNote())
					top = voice;
			}
		}
	}

	// Eliminate pathological cases (ie: only 1 note playing): we always give precedence to the lowest note(s)
	if (top == low)
		top = nullptr;

	// The oldest note that's playing with the target pitch is ideal..
	for (auto* voice : usableVoices)
		if (voice->getCurrentlyPlayingNote() == midiNoteNumber)
			return voice;

	// Oldest voice that has been released (no finger on it and not held by sustain pedal)
	for (auto* voice : usableVoices)
		if (voice != low && voice != top && voice->isPlayingButReleased())
			return voice;

	// Oldest voice that doesn't have a finger on it:
	for (auto* voice : usableVoices)
		if (voice != low && voice != top && !voice->isKeyDown())
			return voice;

	// Oldest voice that isn't protected
	for (auto* voice : usableVoices)
		if (voice != low && voice != top)
			return voice;

	// We've only got "protected" voices now: lowest note takes priority
	//jassert(low != nullptr);

	// Duophonic synth: give priority to the bass note:
	if (top != nullptr)
		return top;

	return low;
}

VASTSynthesiserSound::VASTSynthesiserSound() {
	m_samplerSound.clear();
	m_samplerSound_changed.clear();
}

bool VASTSynthesiserSound::appliesToNote(int) {
	return true;
}

bool VASTSynthesiserSound::appliesToChannel(int) {
	return true;
}

bool VASTSynthesiserSound::hasSamplerSound() {
	return m_samplerSound.size() > 0;
}

VASTSamplerSound* VASTSynthesiserSound::getSamplerSound() {
	if (hasSamplerSound()) return m_samplerSound[0];
	else return nullptr;
}

void VASTSynthesiserSound::addSamplerSound(VASTSamplerSound* samplerSound) { //only for load preset
	VASTSamplerSound* newSound = new VASTSamplerSound(samplerSound);
	m_samplerSound.clear();
	m_samplerSound.add(samplerSound);
	addSamplerSoundChanged(newSound); //add a copy to the changed
	m_changedFlag = true;
}

void VASTSynthesiserSound::clearSamplerSound() {
	m_samplerSound.clear();
}

void VASTSynthesiserSound::softFadeExchangeSample() {
	VASTSamplerSound* sound = getSamplerSoundChanged();
	if (sound != nullptr)
		m_changedFlag = sound->softFadeExchangeSample();

	//copy change to live here!!
	if (m_changedFlag) {
		VASTSamplerSound* oldSound = getSamplerSoundChanged();
		if (oldSound != nullptr) {
			VASTSamplerSound* newSound = new VASTSamplerSound(getSamplerSoundChanged()); //do safety here
			m_samplerSound.clear();
			m_samplerSound.add(newSound);
		}
		else {
			m_samplerSound.clear();
		}
		m_changedFlag = false;
	}
}

bool VASTSynthesiserSound::hasSamplerSoundChanged() {
	return m_samplerSound_changed.size() > 0;
}

VASTSamplerSound* VASTSynthesiserSound::getSamplerSoundChanged() {
	if (hasSamplerSoundChanged()) return m_samplerSound_changed[0];
	else return nullptr;
}

void VASTSynthesiserSound::clearSamplerSoundChanged() {
	m_samplerSound_changed.clear();
	m_changedFlag = true;
}

void VASTSynthesiserSound::addSamplerSoundChanged(VASTSamplerSound* samplerSound) {
	m_samplerSound_changed.clear();
	m_samplerSound_changed.add(samplerSound);
	m_changedFlag = true;
}
