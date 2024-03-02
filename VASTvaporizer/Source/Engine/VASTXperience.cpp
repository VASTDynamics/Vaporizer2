/*
VAST Dynamics Audio Software (TM)
*/

#include "VASTEngineHeader.h"
#include "VASTXperience.h"
#include "VASTSettings.h"
#include "VASTSampler.h"
#include "Utils/VASTSynthfunctions.h"
#include "../Plugin/VASTAudioProcessor.h"
#include "../Plugin/VASTScopeDisplay/VASTOscilloscopeOGL2D.h"
#include <string>
#include <sstream>
#include <chrono>

CVASTXperience::CVASTXperience(VASTAudioProcessor* processor) :
	m_Set(processor),
    myProcessor(processor),
    m_Poly(m_Set, processor),
	m_fxBus1(processor, m_Set, 0),
	m_fxBus2(processor, m_Set, 1),
	m_fxBus3(processor, m_Set, 2)
{

#if defined(_DEBUG)
#if defined _WINDOWS
	int tmpFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	tmpFlag |= _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF;  // Check heap alloc and dump mem leaks at exit
	_CrtSetDbgFlag(tmpFlag);
	//_CrtSetBreakAlloc(651510); // Set to break on allocation number in case you get a leak without a line number
#endif
#endif

	m_isPreparingForPlay.store(false);
	my_processor = processor; //in CVASTEffect
	setEffectName(JucePlugin_Name);
	m_midiBank.store(0);
}

/* destructor()
Destroy variables allocated in the contructor()

*/
CVASTXperience::~CVASTXperience(void)
{
	m_oversampledBuffer = nullptr;

	AudioProcessorValueTreeState& parameters = ((VASTAudioProcessor*)myProcessor)->getParameterTree();

	Array<AudioProcessorParameterWithID*> params = getParameters();
	for (int i = 0, max = params.size(); i < max; i++) {
		parameters.removeParameterListener(params[i]->paramID, (AudioProcessorValueTreeState::Listener*)this);
	}

	//_CrtDumpMemoryLeaks();
}

bool CVASTXperience::initializeEngine()
{	
	VDBG("Start InitializeEngine");
	m_Set.m_nSampleRate.store(m_nSampleRate.load());

	std::shared_ptr<CVASTParamState> state = std::make_shared<CVASTParamState>();
	m_Set.m_State.swap(state);

    AudioProcessorValueTreeState& parameters = myProcessor->getParameterTree();

	//--------------------------------------------------------------------------------
	//start of standard parameters that will never get changed anymore
	m_Set.m_State->initParameters(parameters, this);

	//copy first set over
	std::shared_ptr<CVASTParamState> copiedState(m_Set.m_State); //copy constructor! copies all
	m_Set.m_State = copiedState;

	const double smoothTime = 0.02;
	m_Set.m_fMasterVolume_smoothed.reset(m_Set.m_nSampleRate, smoothTime);

	m_Poly.init();

	m_fxBus1.init();
	m_fxBus2.init();
	m_fxBus3.init();
	//end of standard parameters that will never get changed anymore
	//--------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------
	//compatibility parameters 
	m_Set.m_State->initCompatibilityParameters(parameters, this);
	m_fxBus1.initCompatibilityParameters();
	m_fxBus2.initCompatibilityParameters();
	m_fxBus3.initCompatibilityParameters();
	//--------------------------------------------------------------------------------

	m_Set.m_State->initCompatibilityParameters2(parameters, this); //2019/11/1

	m_Set.m_State->initCompatibilityParameters3(parameters, this); //2020/1/24

	m_Set.m_State->initCompatibilityParameters4(parameters, this); //2020/4/23

	m_fxBus1.initCompatibilityParameters5(); //2020/5/10
	m_fxBus2.initCompatibilityParameters5(); //2020/5/10
	m_fxBus3.initCompatibilityParameters5(); //2020/5/10

	//--------------------------------------------------------------------------------
	//compatibility parameters - add new methods here for new parameters!
	//--------------------------------------------------------------------------------

	m_Set.initModMatrix(); //when all parameters are in

	m_BlockProcessing.store(false);
	m_BlockProcessingIsBlockedSuccessfully.store(false);

	///updateVariables(); // this is not really required here
	m_iFadeOutSamples = 0;
	m_iFadeInSamples = 0;

	// create buffers
	m_Set.m_RoutingBuffers.init();

	oscilloscopeRingBuffer.reset(new VASTRingBuffer<GLfloat>(1, 64 * 10)); //just init size
	VDBG("End InitializeEngine");
	return true;
}

bool CVASTXperience::audioProcessLock()
{
	if (myProcessor->isAudioThread()) { //no lock if called on AudioThread
		return true;
	}

	if (m_BlockProcessing.load()) { //it should not be locked already by another task!
		vassertfalse;
		return false;
	}
		
	VDBG("Audio process suspended / locked!");
	//myProcessor->suspendProcessing(true);

	//const ScopedLock sl(myProcessor->getCallbackLock()); //this is required here but why
	for (int bank = 0; bank < 4; bank ++)
		m_Poly.m_OscBank[bank].m_bWavetableSoftfadeStillRendered.store(false);
	m_BlockProcessing.store(true);
	m_BlockProcessingIsBlockedSuccessfully.store(false);
	return true;
}

bool CVASTXperience::audioProcessUnlock()
{
	if (myProcessor->isAudioThread()) { //no lock if called on AudioThread
		return true;
	}

	if (!m_BlockProcessing.load()) { //error if not locked!
		vassertfalse;
		return false;
	}
	//myProcessor->suspendProcessing(false);

	//const ScopedLock sl(myProcessor->getCallbackLock()); //this is required here but why
	m_BlockProcessing.store(false);
	m_BlockProcessingIsBlockedSuccessfully.store(false);
	VDBG("Audio process no longer suspended / unlocked!");
	return true;
}

bool CVASTXperience::getBlockProcessingIsBlockedSuccessfully() {
	//const ScopedLock sl(myProcessor->getCallbackLock()); //this is required here but why
	return m_BlockProcessingIsBlockedSuccessfully.load();
}

bool CVASTXperience::nonThreadsafeIsBlockedProcessingInfo() {
	return m_BlockProcessing.load();
}

bool CVASTXperience::getBlockProcessing() {
	//const ScopedLock sl(myProcessor->getCallbackLock()); //this is required here but why
	return m_BlockProcessing.load();
}

/* prepareForPlay()
Called by the client after Play() is initiated but before audio streams
*/
bool CVASTXperience::prepareForPlay(double sampleRate, int expectedSamplesPerBlock) {
	VDBG("Prepare for play called!");

	int waitstate = 0;
	while (m_isPreparingForPlay.load() == true) {
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		waitstate += 50;
		if (waitstate > 5000) {
			VDBG("ERROR! prepareForPlay() -  terminating load process!");
			return false; //end after 5s waiting time
		}
	} //check sleep thread
	m_isPreparingForPlay.store(true);

	int samplesPerBlock = expectedSamplesPerBlock;
	
	if (expectedSamplesPerBlock > C_MAX_BUFFER_SIZE) {
		myProcessor->setErrorState(myProcessor->vastErrorState::errorState25_maxBufferSizeExceeded); //buffer size to large
		samplesPerBlock = C_MAX_BUFFER_SIZE;
	}

	//audiprocesslock should be set outside
	m_Set.m_nSampleRate.store(sampleRate);
	m_Set.m_nExpectedSamplesPerBlock = samplesPerBlock;
	m_iMaxFadeSamples = m_Set.m_nSampleRate * 0.05f; //50ms

	m_bOversampleBus1_changed = false;
	m_bOversampleBus2_changed = false;
	m_bOversampleBus2_changed = false;

	//adjust all buffers
	m_Set.m_RoutingBuffers.resize(samplesPerBlock, false); //false means free memory

	//Poly
	m_Poly.prepareForPlay();

	//FX Bus
	m_fxBus1.prepareToPlay(m_Set.m_nSampleRate, samplesPerBlock);
	m_fxBus2.prepareToPlay(m_Set.m_nSampleRate, samplesPerBlock);
	m_fxBus3.prepareToPlay(m_Set.m_nSampleRate, samplesPerBlock);
	
	m_Set.modMatrixCalcBuffers();

	m_Set.m_fMasterVolume_smoothed.setCurrentAndTargetValue(pow(10.0, *m_Set.m_State->m_fMasterVolumedB / 20.0));  // take next value

	//for daw synchronization
	m_Set.m_dPpqPosition = 0;
	m_Set.m_dPpqBpm = 0;
	m_Set.m_bPpqIsPlaying = false;
	m_Set.m_bPpqIsLooping = false;
	m_Set.m_dPpqPositionOfLastBarStart = 0;
	
	oscilloscopeRingBuffer.reset(new VASTRingBuffer<GLfloat>(1, OGL2D_RING_BUFFER_SIZE));

	m_bLastChainBufferZero = false; //CHECK was false
	m_isPreparingForPlay.store(false);
	return true;
}

void CVASTXperience::beginSoftFade() {
	//check osc bank soft fade
	for (int bank = 0; bank < 4; bank++) {
		m_Poly.m_OscBank[bank].beginSoftFade();
	}
}

void CVASTXperience::endSoftFade() {
	for (int bank = 0; bank < 4; bank++) {
		if (!m_Poly.m_OscBank[bank].endSoftFade()) {
			vassertfalse; //dont treat as error for now
			//myProcessor->setErrorState(VASTAudioProcessor::vastErrorState::errorState17_internalWavetableEngineError);
		}
	}

	for (int mseg = 0; mseg < 5; mseg++) {
		if (m_Set.m_MSEGData_changed[mseg].isMSEGDirty()) {
			m_Set.m_MSEGData[mseg].copyDataFrom(m_Set.m_MSEGData_changed[mseg]);
			m_Set.m_MSEGData_changed[mseg].clearDirtyFlag();
		}
	}

	for (int stepseq = 0; stepseq < 3; stepseq++) {
		if (m_Set.m_StepSeqData_changed[stepseq].isMSEGDirty()) {
			m_Set.m_StepSeqData[stepseq].copyDataFrom(m_Set.m_StepSeqData_changed[stepseq]);
			m_Set.m_StepSeqData_changed[stepseq].clearDirtyFlag();
		}
	}

	if (m_Set.m_ARPData_changed.getIsDirty()) {
		m_Set.m_ARPData.copyDataFrom(m_Set.m_ARPData_changed);
		m_Set.m_ARPData_changed.clearDirtyFlag();
	}

	m_Poly.softFadeExchangeSample();
}

int CVASTXperience::getMidiBank()
{
	return m_midiBank.load();
}

//==========================================================================================================================

bool CVASTXperience::processAudioBuffer(AudioSampleBuffer& buffer, MidiBuffer& midiMessages, MYUINT uNumOutChannels, bool isPlaying,
                                        double ppqPosition, bool isLooping, double ppqPositionOfLastBarStart, double bpm) {
    
    if (buffer.getNumSamples() > C_MAX_BUFFER_SIZE) {
        myProcessor->setErrorState(myProcessor->vastErrorState::errorState25_maxBufferSizeExceeded); //buffer size to large
        return false;
    }
    
#ifdef _DEBUG
    //-------------------------------
    //check processing time
    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
#endif
    
    //time code at buffer start position
    m_Set.m_dPpqPosition = ppqPosition;
    m_Set.m_bPpqIsPlaying = isPlaying;
    m_Set.m_bPpqIsLooping = isLooping;
    m_Set.m_dPpqPositionOfLastBarStart = ppqPositionOfLastBarStart;
    if (m_Set.m_dPpqBpm != bpm) {
        VDBG("BPM changed! Now: " << bpm);
        m_Set.m_dPpqBpm = bpm;
        if (*m_Set.m_State->m_bLFOSynch_LFO1 == static_cast<int>(SWITCH::SWITCH_ON)) {
            m_Poly.updateLFO(0);
        }
        if (*m_Set.m_State->m_bLFOSynch_LFO2 == static_cast<int>(SWITCH::SWITCH_ON)) {
            m_Poly.updateLFO(1);
        }
        if (*m_Set.m_State->m_bLFOSynch_LFO3 == static_cast<int>(SWITCH::SWITCH_ON)) {
            m_Poly.updateLFO(2);
        }
        if (*m_Set.m_State->m_bLFOSynch_LFO4 == static_cast<int>(SWITCH::SWITCH_ON)) {
            m_Poly.updateLFO(3);
        }
        if (*m_Set.m_State->m_bLFOSynch_LFO5 == static_cast<int>(SWITCH::SWITCH_ON)) {
            m_Poly.updateLFO(4);
        }
        m_fxBus1.updateTiming();
        m_fxBus2.updateTiming();
        m_fxBus3.updateTiming();
        
        for (int mseg = 0; mseg < 5; mseg++) {
            m_Set.m_MSEGData[mseg].setDirty();
            m_Set.m_MSEGData_changed[mseg].setDirty();
            
            //hack should be in set time beats!
            bool synch = false;
            if (mseg == 0) { synch = *m_Set.m_State->m_bMSEGSynch_MSEG1; }
            else if (mseg == 1) { synch = *m_Set.m_State->m_bMSEGSynch_MSEG2; }
            else if (mseg == 2) { synch = *m_Set.m_State->m_bMSEGSynch_MSEG3; }
            else if (mseg == 3) { synch = *m_Set.m_State->m_bMSEGSynch_MSEG4; }
            else if (mseg == 4) { synch = *m_Set.m_State->m_bMSEGSynch_MSEG5; }
            
            if (synch == static_cast<int>(SWITCH::SWITCH_ON)) {
                m_Set.m_MSEGData[mseg].setSynch(true);
                m_Set.m_MSEGData_changed[mseg].setSynch(true);
                m_Set.m_MSEGData[mseg].setAttackSteps(m_Set.m_MSEGData[mseg].getAttackSteps(), &m_Set);
                m_Set.m_MSEGData_changed[mseg].setAttackSteps(m_Set.m_MSEGData_changed[mseg].getAttackSteps(), &m_Set);
                m_Set.m_MSEGData[mseg].setDecaySteps(m_Set.m_MSEGData[mseg].getDecaySteps(), &m_Set);
                m_Set.m_MSEGData_changed[mseg].setDecaySteps(m_Set.m_MSEGData_changed[mseg].getDecaySteps(), &m_Set);
                m_Set.m_MSEGData[mseg].setReleaseSteps(m_Set.m_MSEGData[mseg].getReleaseSteps(), &m_Set);
                m_Set.m_MSEGData_changed[mseg].setReleaseSteps(m_Set.m_MSEGData_changed[mseg].getReleaseSteps(), &m_Set);
            }
            else {
                m_Set.m_MSEGData[mseg].setSynch(false);
                m_Set.m_MSEGData_changed[mseg].setSynch(false);
            }
        }

        //if (m_Set.m_bPpqIsPlaying) {
        float l_fIntervalTime = 0.f;
        if (*m_Set.m_State->m_bStepSeqSynch_STEPSEQ1 == static_cast<int>(SWITCH::SWITCH_ON)) {
            l_fIntervalTime = m_Set.getIntervalTimeFromDAWBeats(*m_Set.m_State->m_uStepSeqTimeBeats_STEPSEQ1);
            m_Set.m_StepSeqData[0].setStepSeqTime(l_fIntervalTime); //is in ms
            m_Set.m_StepSeqData_changed[0].setStepSeqTime(l_fIntervalTime); //is in ms
        }
        if (*m_Set.m_State->m_bStepSeqSynch_STEPSEQ2 == static_cast<int>(SWITCH::SWITCH_ON)) {
            l_fIntervalTime = m_Set.getIntervalTimeFromDAWBeats(*m_Set.m_State->m_uStepSeqTimeBeats_STEPSEQ2);
            m_Set.m_StepSeqData[1].setStepSeqTime(l_fIntervalTime); //is in ms
            m_Set.m_StepSeqData_changed[1].setStepSeqTime(l_fIntervalTime); //is in ms
        }
        if (*m_Set.m_State->m_bStepSeqSynch_STEPSEQ3 == static_cast<int>(SWITCH::SWITCH_ON)) {
            l_fIntervalTime = m_Set.getIntervalTimeFromDAWBeats(*m_Set.m_State->m_uStepSeqTimeBeats_STEPSEQ3);
            m_Set.m_StepSeqData[2].setStepSeqTime(l_fIntervalTime); //is in ms
            m_Set.m_StepSeqData_changed[2].setStepSeqTime(l_fIntervalTime); //is in ms
        }
        //}
    }
    
    m_Set.m_dPpqBpm = bpm;
    
    //=================================================================================================
    // Is prepare for play running?
	if ((m_iFadeOutSamples <= 0) || (m_BlockProcessingIsBlockedSuccessfully.load())) { //check new - wait for fadeout to complete
		if (m_BlockProcessing.load() == true) {
			//const ScopedLock sl(myProcessor->getCallbackLock());
			m_BlockProcessingIsBlockedSuccessfully = true;
			VDBG("BlockProcessingIsBlockedSuccessfully is true!");
			buffer.clear();
			m_iFadeOutSamples = 0;
			m_iFadeInSamples = 0;
			return false;
		}
	}
    
    beginSoftFade();
    
    //Hardcore check
    if ((m_bLastChainBufferZero) && (m_iFadeOutSamples == 0) && (m_iFadeInSamples == 0) &&
        (midiMessages.isEmpty() && (m_Poly.getSynthesizer()->getNumMidiNotesKeyDown() == 0) && (!m_Poly.voicesMSEGStillActive()) &&
         ((*m_Set.m_State->m_bARPOnOff == static_cast<int>(SWITCH::SWITCH_OFF)) || (m_Poly.m_ARP_midiInNotes.size() == 0))
         )) {
        
        for (int bank = 0; bank < 4; bank++)
            m_Poly.m_OscBank[bank].m_bWavetableSoftfadeStillRendered.store(false);
        
        if (m_bBufferZeroMilliSeconds > 2000.f) { //2s silence?
            buffer.clear();
            endSoftFade();
            m_iFadeOutSamples = 0;
            m_iFadeInSamples = 0;
            return false;
        }
    }
    
	//Dealloacations
	//===========================================================================================
	if (m_Set.m_uTargetMaxPoly != m_Set.m_uMaxPoly) {
		m_Set.m_uMaxPoly = m_Set.m_uTargetMaxPoly;
		m_Poly.init();
	}

    if (myProcessor->getActiveEditor() != nullptr) {
        if (myProcessor->isEditorCurrentlyProbablyVisible()) {
            for (int bank = 0; bank < 4; bank++) {
                std::shared_ptr<CVASTWaveTable> l_wavetable;
                l_wavetable = m_Poly.m_OscBank[bank].getNewSharedSoftFadeWavetable();
                if (l_wavetable == nullptr) {
                    l_wavetable = m_Poly.m_OscBank[bank].getWavetablePointer();
                }
                if (l_wavetable != nullptr)
                    l_wavetable->copyUIFXUpdates();
            }
        }
    }
    
    //===========================================================================================
    //MIDI learn processing
    int samplePosition = 0;    
    auto midiIterator = midiMessages.findNextSamplePosition(samplePosition);
    std::for_each(midiIterator,
                  midiMessages.cend(),
		[&](const MidiMessageMetadata& metadata)
		{
			if (metadata.getMessage().isController()) {
				if (myProcessor->m_iNowLearningMidiParameter >= 0) {
					myProcessor->midiParameterLearned(metadata.getMessage().getControllerNumber());
					myProcessor->requestUIUpdate();
				}
				else {
					//check for mapped parameters
					int lParamInternal = 0;
					lParamInternal = myProcessor->midiMappingGetParameterIndex(metadata.getMessage().getControllerNumber());
					if (lParamInternal >= 0) {
						float lValue = metadata.getMessage().getControllerValue() / 127.0f;
						if (lParamInternal == 9999) { //UI
							String uiComponentName = myProcessor->midiMappingComponentVariableName(metadata.getMessage().getControllerNumber());
							myProcessor->registerComponentValueUpdate(uiComponentName, lValue); //UI only parameter that is not in parameter state, e.g. WT functions
						}
						else if (lParamInternal < getParameters().size()) {
							getParameters()[lParamInternal]->setValueNotifyingHost(lValue);
						}
					}
				}
			}			
		});
    //===========================================================================================
    
    //resize buffers
    m_Set.m_RoutingBuffers.resize(buffer.getNumSamples(), true);

	//===========================================================================================
	//LFO used?
	m_Set.m_RoutingBuffers.lfoUsed[0].store(m_Set.modMatrixSourceSetFast(MODMATSRCE::LFO1));
	m_Set.m_RoutingBuffers.lfoUsed[1].store(m_Set.modMatrixSourceSetFast(MODMATSRCE::LFO2));
	m_Set.m_RoutingBuffers.lfoUsed[2].store(m_Set.modMatrixSourceSetFast(MODMATSRCE::LFO3));
	m_Set.m_RoutingBuffers.lfoUsed[3].store(m_Set.modMatrixSourceSetFast(MODMATSRCE::LFO4));
	m_Set.m_RoutingBuffers.lfoUsed[4].store(m_Set.modMatrixSourceSetFast(MODMATSRCE::LFO5));

	//MSEG is used?
	m_Set.m_RoutingBuffers.msegUsed[0].store((m_Set.modMatrixSourceSetFast(MODMATSRCE::MSEG1Env) ||
		((*m_Set.m_State->m_uVCAEnv_OscA == static_cast<int>(MSEGENV::MSEG1)) && (*m_Set.m_State->m_bOscOnOff_OscA == static_cast<int>(SWITCH::SWITCH_ON))) ||
		((*m_Set.m_State->m_uVCAEnv_OscB == static_cast<int>(MSEGENV::MSEG1)) && (*m_Set.m_State->m_bOscOnOff_OscB == static_cast<int>(SWITCH::SWITCH_ON))) ||
		((*m_Set.m_State->m_uVCAEnv_OscC == static_cast<int>(MSEGENV::MSEG1)) && (*m_Set.m_State->m_bOscOnOff_OscC == static_cast<int>(SWITCH::SWITCH_ON))) ||
		((*m_Set.m_State->m_uVCAEnv_OscD == static_cast<int>(MSEGENV::MSEG1)) && (*m_Set.m_State->m_bOscOnOff_OscD == static_cast<int>(SWITCH::SWITCH_ON))) ||
		((*m_Set.m_State->m_uVCAEnv_Noise == static_cast<int>(MSEGENV::MSEG1)) && (*m_Set.m_State->m_bNoiseOnOff == static_cast<int>(SWITCH::SWITCH_ON))) ||
		((*m_Set.m_State->m_uVCAEnv_Sampler == static_cast<int>(MSEGENV::MSEG1)) && (*m_Set.m_State->m_bSamplerOnOff == static_cast<int>(SWITCH::SWITCH_ON))) ||
		((*m_Set.m_State->m_uVCFEnv_Filter1 == static_cast<int>(MSEGENV::MSEG1)) && (*m_Set.m_State->m_bOnOff_Filter1 == static_cast<int>(SWITCH::SWITCH_ON))) ||
		((*m_Set.m_State->m_uVCFEnv_Filter2 == static_cast<int>(MSEGENV::MSEG1)) && (*m_Set.m_State->m_bOnOff_Filter2 == static_cast<int>(SWITCH::SWITCH_ON))) ||
		((*m_Set.m_State->m_uVCFEnv_Filter3 == static_cast<int>(MSEGENV::MSEG1)) && (*m_Set.m_State->m_bOnOff_Filter3 == static_cast<int>(SWITCH::SWITCH_ON))) ||
		((*m_Set.m_State->m_uLFOMSEG_LFO1 == static_cast<int>(MSEGLFOENV::MSEGLFO1)) && (m_Set.m_RoutingBuffers.lfoUsed[0].load() == true)) ||
		((*m_Set.m_State->m_uLFOMSEG_LFO2 == static_cast<int>(MSEGLFOENV::MSEGLFO1)) && (m_Set.m_RoutingBuffers.lfoUsed[1].load() == true)) ||
		((*m_Set.m_State->m_uLFOMSEG_LFO3 == static_cast<int>(MSEGLFOENV::MSEGLFO1)) && (m_Set.m_RoutingBuffers.lfoUsed[2].load() == true)) ||
		((*m_Set.m_State->m_uLFOMSEG_LFO4 == static_cast<int>(MSEGLFOENV::MSEGLFO1)) && (m_Set.m_RoutingBuffers.lfoUsed[3].load() == true)) ||
		((*m_Set.m_State->m_uLFOMSEG_LFO5 == static_cast<int>(MSEGLFOENV::MSEGLFO1)) && (m_Set.m_RoutingBuffers.lfoUsed[4].load() == true))));
	m_Set.m_RoutingBuffers.msegUsed[1] = (m_Set.modMatrixSourceSetFast(MODMATSRCE::MSEG2Env) ||
		((*m_Set.m_State->m_uVCAEnv_OscA == static_cast<int>(MSEGENV::MSEG2)) && (*m_Set.m_State->m_bOscOnOff_OscA == static_cast<int>(SWITCH::SWITCH_ON))) ||
		((*m_Set.m_State->m_uVCAEnv_OscB == static_cast<int>(MSEGENV::MSEG2)) && (*m_Set.m_State->m_bOscOnOff_OscB == static_cast<int>(SWITCH::SWITCH_ON))) ||
		((*m_Set.m_State->m_uVCAEnv_OscC == static_cast<int>(MSEGENV::MSEG2)) && (*m_Set.m_State->m_bOscOnOff_OscC == static_cast<int>(SWITCH::SWITCH_ON))) ||
		((*m_Set.m_State->m_uVCAEnv_OscD == static_cast<int>(MSEGENV::MSEG2)) && (*m_Set.m_State->m_bOscOnOff_OscD == static_cast<int>(SWITCH::SWITCH_ON))) ||
		((*m_Set.m_State->m_uVCAEnv_Noise == static_cast<int>(MSEGENV::MSEG2)) && (*m_Set.m_State->m_bNoiseOnOff == static_cast<int>(SWITCH::SWITCH_ON))) ||
		((*m_Set.m_State->m_uVCAEnv_Sampler == static_cast<int>(MSEGENV::MSEG2)) && (*m_Set.m_State->m_bSamplerOnOff == static_cast<int>(SWITCH::SWITCH_ON))) ||
		((*m_Set.m_State->m_uVCFEnv_Filter1 == static_cast<int>(MSEGENV::MSEG2)) && (*m_Set.m_State->m_bOnOff_Filter1 == static_cast<int>(SWITCH::SWITCH_ON))) ||
		((*m_Set.m_State->m_uVCFEnv_Filter2 == static_cast<int>(MSEGENV::MSEG2)) && (*m_Set.m_State->m_bOnOff_Filter2 == static_cast<int>(SWITCH::SWITCH_ON))) ||
		((*m_Set.m_State->m_uVCFEnv_Filter3 == static_cast<int>(MSEGENV::MSEG2)) && (*m_Set.m_State->m_bOnOff_Filter3 == static_cast<int>(SWITCH::SWITCH_ON))) ||
		((*m_Set.m_State->m_uLFOMSEG_LFO1 == static_cast<int>(MSEGLFOENV::MSEGLFO2)) && (m_Set.m_RoutingBuffers.lfoUsed[0].load() == true)) ||
		((*m_Set.m_State->m_uLFOMSEG_LFO2 == static_cast<int>(MSEGLFOENV::MSEGLFO2)) && (m_Set.m_RoutingBuffers.lfoUsed[1].load() == true)) ||
		((*m_Set.m_State->m_uLFOMSEG_LFO3 == static_cast<int>(MSEGLFOENV::MSEGLFO2)) && (m_Set.m_RoutingBuffers.lfoUsed[2].load() == true)) ||
		((*m_Set.m_State->m_uLFOMSEG_LFO4 == static_cast<int>(MSEGLFOENV::MSEGLFO2)) && (m_Set.m_RoutingBuffers.lfoUsed[3].load() == true)) ||
		((*m_Set.m_State->m_uLFOMSEG_LFO5 == static_cast<int>(MSEGLFOENV::MSEGLFO2)) && (m_Set.m_RoutingBuffers.lfoUsed[4].load() == true)));
	m_Set.m_RoutingBuffers.msegUsed[2].store((m_Set.modMatrixSourceSetFast(MODMATSRCE::MSEG3Env) ||
		((*m_Set.m_State->m_uVCAEnv_OscA == static_cast<int>(MSEGENV::MSEG3)) && (*m_Set.m_State->m_bOscOnOff_OscA == static_cast<int>(SWITCH::SWITCH_ON))) ||
		((*m_Set.m_State->m_uVCAEnv_OscB == static_cast<int>(MSEGENV::MSEG3)) && (*m_Set.m_State->m_bOscOnOff_OscB == static_cast<int>(SWITCH::SWITCH_ON))) ||
		((*m_Set.m_State->m_uVCAEnv_OscC == static_cast<int>(MSEGENV::MSEG3)) && (*m_Set.m_State->m_bOscOnOff_OscC == static_cast<int>(SWITCH::SWITCH_ON))) ||
		((*m_Set.m_State->m_uVCAEnv_OscD == static_cast<int>(MSEGENV::MSEG3)) && (*m_Set.m_State->m_bOscOnOff_OscD == static_cast<int>(SWITCH::SWITCH_ON))) ||
		((*m_Set.m_State->m_uVCAEnv_Noise == static_cast<int>(MSEGENV::MSEG3)) && (*m_Set.m_State->m_bNoiseOnOff == static_cast<int>(SWITCH::SWITCH_ON))) ||
		((*m_Set.m_State->m_uVCAEnv_Sampler == static_cast<int>(MSEGENV::MSEG3)) && (*m_Set.m_State->m_bSamplerOnOff == static_cast<int>(SWITCH::SWITCH_ON))) ||
		((*m_Set.m_State->m_uVCFEnv_Filter1 == static_cast<int>(MSEGENV::MSEG3)) && (*m_Set.m_State->m_bOnOff_Filter1 == static_cast<int>(SWITCH::SWITCH_ON))) ||
		((*m_Set.m_State->m_uVCFEnv_Filter2 == static_cast<int>(MSEGENV::MSEG3)) && (*m_Set.m_State->m_bOnOff_Filter2 == static_cast<int>(SWITCH::SWITCH_ON))) ||
		((*m_Set.m_State->m_uVCFEnv_Filter3 == static_cast<int>(MSEGENV::MSEG3)) && (*m_Set.m_State->m_bOnOff_Filter3 == static_cast<int>(SWITCH::SWITCH_ON))) ||
		((*m_Set.m_State->m_uLFOMSEG_LFO1 == static_cast<int>(MSEGLFOENV::MSEGLFO3)) && (m_Set.m_RoutingBuffers.lfoUsed[0].load() == true)) ||
		((*m_Set.m_State->m_uLFOMSEG_LFO2 == static_cast<int>(MSEGLFOENV::MSEGLFO3)) && (m_Set.m_RoutingBuffers.lfoUsed[1].load() == true)) ||
		((*m_Set.m_State->m_uLFOMSEG_LFO3 == static_cast<int>(MSEGLFOENV::MSEGLFO3)) && (m_Set.m_RoutingBuffers.lfoUsed[2].load() == true)) ||
		((*m_Set.m_State->m_uLFOMSEG_LFO4 == static_cast<int>(MSEGLFOENV::MSEGLFO3)) && (m_Set.m_RoutingBuffers.lfoUsed[3].load() == true)) ||
		((*m_Set.m_State->m_uLFOMSEG_LFO5 == static_cast<int>(MSEGLFOENV::MSEGLFO3)) && (m_Set.m_RoutingBuffers.lfoUsed[4].load() == true))));
	m_Set.m_RoutingBuffers.msegUsed[3].store((m_Set.modMatrixSourceSetFast(MODMATSRCE::MSEG4Env) ||
		((*m_Set.m_State->m_uVCAEnv_OscA == static_cast<int>(MSEGENV::MSEG4)) && (*m_Set.m_State->m_bOscOnOff_OscA == static_cast<int>(SWITCH::SWITCH_ON))) ||
		((*m_Set.m_State->m_uVCAEnv_OscB == static_cast<int>(MSEGENV::MSEG4)) && (*m_Set.m_State->m_bOscOnOff_OscB == static_cast<int>(SWITCH::SWITCH_ON))) ||
		((*m_Set.m_State->m_uVCAEnv_OscC == static_cast<int>(MSEGENV::MSEG4)) && (*m_Set.m_State->m_bOscOnOff_OscC == static_cast<int>(SWITCH::SWITCH_ON))) ||
		((*m_Set.m_State->m_uVCAEnv_OscD == static_cast<int>(MSEGENV::MSEG4)) && (*m_Set.m_State->m_bOscOnOff_OscD == static_cast<int>(SWITCH::SWITCH_ON))) ||
		((*m_Set.m_State->m_uVCAEnv_Noise == static_cast<int>(MSEGENV::MSEG4)) && (*m_Set.m_State->m_bNoiseOnOff == static_cast<int>(SWITCH::SWITCH_ON))) ||
		((*m_Set.m_State->m_uVCAEnv_Sampler == static_cast<int>(MSEGENV::MSEG4)) && (*m_Set.m_State->m_bSamplerOnOff == static_cast<int>(SWITCH::SWITCH_ON))) ||
		((*m_Set.m_State->m_uVCFEnv_Filter1 == static_cast<int>(MSEGENV::MSEG4)) && (*m_Set.m_State->m_bOnOff_Filter1 == static_cast<int>(SWITCH::SWITCH_ON))) ||
		((*m_Set.m_State->m_uVCFEnv_Filter2 == static_cast<int>(MSEGENV::MSEG4)) && (*m_Set.m_State->m_bOnOff_Filter2 == static_cast<int>(SWITCH::SWITCH_ON))) ||
		((*m_Set.m_State->m_uVCFEnv_Filter3 == static_cast<int>(MSEGENV::MSEG4)) && (*m_Set.m_State->m_bOnOff_Filter3 == static_cast<int>(SWITCH::SWITCH_ON))) ||
		((*m_Set.m_State->m_uLFOMSEG_LFO1 == static_cast<int>(MSEGLFOENV::MSEGLFO4)) && (m_Set.m_RoutingBuffers.lfoUsed[0].load() == true)) ||
		((*m_Set.m_State->m_uLFOMSEG_LFO2 == static_cast<int>(MSEGLFOENV::MSEGLFO4)) && (m_Set.m_RoutingBuffers.lfoUsed[1].load() == true)) ||
		((*m_Set.m_State->m_uLFOMSEG_LFO3 == static_cast<int>(MSEGLFOENV::MSEGLFO4)) && (m_Set.m_RoutingBuffers.lfoUsed[2].load() == true)) ||
		((*m_Set.m_State->m_uLFOMSEG_LFO4 == static_cast<int>(MSEGLFOENV::MSEGLFO4)) && (m_Set.m_RoutingBuffers.lfoUsed[3].load() == true)) ||
		((*m_Set.m_State->m_uLFOMSEG_LFO5 == static_cast<int>(MSEGLFOENV::MSEGLFO4)) && (m_Set.m_RoutingBuffers.lfoUsed[4].load() == true))));
	m_Set.m_RoutingBuffers.msegUsed[4].store((m_Set.modMatrixSourceSetFast(MODMATSRCE::MSEG5Env) ||
		((*m_Set.m_State->m_uVCAEnv_OscA == static_cast<int>(MSEGENV::MSEG5)) && (*m_Set.m_State->m_bOscOnOff_OscA == static_cast<int>(SWITCH::SWITCH_ON))) ||
		((*m_Set.m_State->m_uVCAEnv_OscB == static_cast<int>(MSEGENV::MSEG5)) && (*m_Set.m_State->m_bOscOnOff_OscB == static_cast<int>(SWITCH::SWITCH_ON))) ||
		((*m_Set.m_State->m_uVCAEnv_OscC == static_cast<int>(MSEGENV::MSEG5)) && (*m_Set.m_State->m_bOscOnOff_OscC == static_cast<int>(SWITCH::SWITCH_ON))) ||
		((*m_Set.m_State->m_uVCAEnv_OscD == static_cast<int>(MSEGENV::MSEG5)) && (*m_Set.m_State->m_bOscOnOff_OscD == static_cast<int>(SWITCH::SWITCH_ON))) ||
		((*m_Set.m_State->m_uVCAEnv_Noise == static_cast<int>(MSEGENV::MSEG5)) && (*m_Set.m_State->m_bNoiseOnOff == static_cast<int>(SWITCH::SWITCH_ON))) ||
		((*m_Set.m_State->m_uVCAEnv_Sampler == static_cast<int>(MSEGENV::MSEG5)) && (*m_Set.m_State->m_bSamplerOnOff == static_cast<int>(SWITCH::SWITCH_ON))) ||
		((*m_Set.m_State->m_uVCFEnv_Filter1 == static_cast<int>(MSEGENV::MSEG5)) && (*m_Set.m_State->m_bOnOff_Filter1 == static_cast<int>(SWITCH::SWITCH_ON))) ||
		((*m_Set.m_State->m_uVCFEnv_Filter2 == static_cast<int>(MSEGENV::MSEG5)) && (*m_Set.m_State->m_bOnOff_Filter2 == static_cast<int>(SWITCH::SWITCH_ON))) ||
		((*m_Set.m_State->m_uVCFEnv_Filter3 == static_cast<int>(MSEGENV::MSEG5)) && (*m_Set.m_State->m_bOnOff_Filter3 == static_cast<int>(SWITCH::SWITCH_ON))) ||
		((*m_Set.m_State->m_uLFOMSEG_LFO1 == static_cast<int>(MSEGLFOENV::MSEGLFO5)) && (m_Set.m_RoutingBuffers.lfoUsed[0].load() == true)) ||
		((*m_Set.m_State->m_uLFOMSEG_LFO2 == static_cast<int>(MSEGLFOENV::MSEGLFO5)) && (m_Set.m_RoutingBuffers.lfoUsed[1].load() == true)) ||
		((*m_Set.m_State->m_uLFOMSEG_LFO3 == static_cast<int>(MSEGLFOENV::MSEGLFO5)) && (m_Set.m_RoutingBuffers.lfoUsed[2].load() == true)) ||
		((*m_Set.m_State->m_uLFOMSEG_LFO4 == static_cast<int>(MSEGLFOENV::MSEGLFO5)) && (m_Set.m_RoutingBuffers.lfoUsed[3].load() == true)) ||
		((*m_Set.m_State->m_uLFOMSEG_LFO5 == static_cast<int>(MSEGLFOENV::MSEGLFO5)) && (m_Set.m_RoutingBuffers.lfoUsed[4].load() == true))));

    m_Set.m_RoutingBuffers.stepSeqUsed[0].store(m_Set.modMatrixSourceSetFast(MODMATSRCE::StepSeq1));
    m_Set.m_RoutingBuffers.stepSeqUsed[1].store(m_Set.modMatrixSourceSetFast(MODMATSRCE::StepSeq2));
    m_Set.m_RoutingBuffers.stepSeqUsed[2].store(m_Set.modMatrixSourceSetFast(MODMATSRCE::StepSeq3));
    
    //=================================================================================================
    
    const int numFrames = buffer.getNumSamples();
    
    //=================================================================================================
    
    m_Set.m_fMasterVolume = m_Set.m_fMasterVolume_smoothed.getNextValue();
    m_Set.m_fMasterVolume_smoothed.skip(numFrames - 1);
    
    //=================================================================================================
    //Check oversampling changes
    if (m_bOversampleBus1_changed) {
        m_bOversampleBus1_changed = false;
        m_fxBus1.prepareToPlay(m_Set.m_nSampleRate, m_Set.m_nExpectedSamplesPerBlock);
    }
    if (m_bOversampleBus2_changed) {
        m_bOversampleBus2_changed = false;
        m_fxBus2.prepareToPlay(m_Set.m_nSampleRate, m_Set.m_nExpectedSamplesPerBlock);
    }
    if (m_bOversampleBus3_changed) {
        m_bOversampleBus3_changed = false;
        m_fxBus3.prepareToPlay(m_Set.m_nSampleRate, m_Set.m_nExpectedSamplesPerBlock);
    }
    
    //=================================================================================================
    if (buffer.getNumChannels() > 0) {
        m_Set.m_RoutingBuffers.fAudioInputBuffer->copyFrom(0, 0, buffer.getReadPointer(0), numFrames); //MONO only
        if (m_Set.modMatrixSourceSetFast(MODMATSRCE::InputEnvelope) == true)
            m_Set.processEnvelope(numFrames);
    }
    
    //clear buffer before poly
    buffer.clear();
    m_Poly.processAudioBuffer(m_Set.m_RoutingBuffers, midiMessages);
    
    //=================================================================================================
    //FX Busses
    m_fxBus1.processBuffers(m_Set.m_RoutingBuffers, midiMessages);
    m_fxBus2.processBuffers(m_Set.m_RoutingBuffers, midiMessages);
    m_fxBus3.processBuffers(m_Set.m_RoutingBuffers, midiMessages);
    
    //=================================================================================================
    
    //Master Volume
    //Fill output buffer
    //do routing
    if ((*m_Set.m_State->m_uOscRouting1_OscA == static_cast<int>(OSCROUTE::OSCROUTE_Master)) || (*m_Set.m_State->m_uOscRouting2_OscA == static_cast<int>(OSCROUTE::OSCROUTE_Master))) {
        m_Set.m_RoutingBuffers.MasterOutBuffer->addFrom(0, 0, m_Set.m_RoutingBuffers.OscBuffer[0]->getReadPointer(0, 0), numFrames); //gain??
        m_Set.m_RoutingBuffers.MasterOutBuffer->addFrom(1, 0, m_Set.m_RoutingBuffers.OscBuffer[0]->getReadPointer(1, 0), numFrames); //gain??
    }
    if ((*m_Set.m_State->m_uOscRouting1_OscB == static_cast<int>(OSCROUTE::OSCROUTE_Master)) || (*m_Set.m_State->m_uOscRouting2_OscB == static_cast<int>(OSCROUTE::OSCROUTE_Master))) {
        m_Set.m_RoutingBuffers.MasterOutBuffer->addFrom(0, 0, m_Set.m_RoutingBuffers.OscBuffer[1]->getReadPointer(0, 0), numFrames); //gain??
        m_Set.m_RoutingBuffers.MasterOutBuffer->addFrom(1, 0, m_Set.m_RoutingBuffers.OscBuffer[1]->getReadPointer(1, 0), numFrames); //gain??
    }
    if ((*m_Set.m_State->m_uOscRouting1_OscC == static_cast<int>(OSCROUTE::OSCROUTE_Master)) || (*m_Set.m_State->m_uOscRouting2_OscC == static_cast<int>(OSCROUTE::OSCROUTE_Master))) {
        m_Set.m_RoutingBuffers.MasterOutBuffer->addFrom(0, 0, m_Set.m_RoutingBuffers.OscBuffer[2]->getReadPointer(0, 0), numFrames); //gain??
        m_Set.m_RoutingBuffers.MasterOutBuffer->addFrom(1, 0, m_Set.m_RoutingBuffers.OscBuffer[2]->getReadPointer(1, 0), numFrames); //gain??
    }
    if ((*m_Set.m_State->m_uOscRouting1_OscD == static_cast<int>(OSCROUTE::OSCROUTE_Master)) || (*m_Set.m_State->m_uOscRouting2_OscD == static_cast<int>(OSCROUTE::OSCROUTE_Master))) {
        m_Set.m_RoutingBuffers.MasterOutBuffer->addFrom(0, 0, m_Set.m_RoutingBuffers.OscBuffer[3]->getReadPointer(0, 0), numFrames); //gain??
        m_Set.m_RoutingBuffers.MasterOutBuffer->addFrom(1, 0, m_Set.m_RoutingBuffers.OscBuffer[3]->getReadPointer(1, 0), numFrames); //gain??
    }
    if ((*m_Set.m_State->m_uNoiseRouting1 == static_cast<int>(OSCROUTE::OSCROUTE_Master)) || (*m_Set.m_State->m_uNoiseRouting2 == static_cast<int>(OSCROUTE::OSCROUTE_Master))) {
        m_Set.m_RoutingBuffers.MasterOutBuffer->addFrom(0, 0, m_Set.m_RoutingBuffers.NoiseBuffer->getReadPointer(0, 0), numFrames); //gain??
        m_Set.m_RoutingBuffers.MasterOutBuffer->addFrom(1, 0, m_Set.m_RoutingBuffers.NoiseBuffer->getReadPointer(1, 0), numFrames); //gain??
    }
    if ((*m_Set.m_State->m_uSamplerRouting1 == static_cast<int>(OSCROUTE::OSCROUTE_Master)) || (*m_Set.m_State->m_uSamplerRouting2 == static_cast<int>(OSCROUTE::OSCROUTE_Master))) {
        m_Set.m_RoutingBuffers.MasterOutBuffer->addFrom(0, 0, m_Set.m_RoutingBuffers.SamplerBuffer->getReadPointer(0, 0), numFrames); //gain??
        m_Set.m_RoutingBuffers.MasterOutBuffer->addFrom(1, 0, m_Set.m_RoutingBuffers.SamplerBuffer->getReadPointer(1, 0), numFrames); //gain??
    }
    //filter
    if (*m_Set.m_State->m_uFilterRouting_Filter1 == static_cast<int>(FILTER1ROUTE::FILTER1ROUTE_Master)) {
        m_Set.m_RoutingBuffers.MasterOutBuffer->addFrom(0, 0, m_Set.m_RoutingBuffers.FilterBuffer[0]->getReadPointer(0, 0), numFrames); //gain??
        m_Set.m_RoutingBuffers.MasterOutBuffer->addFrom(1, 0, m_Set.m_RoutingBuffers.FilterBuffer[0]->getReadPointer(1, 0), numFrames); //gain??
    }
    if (*m_Set.m_State->m_uFilterRouting2_Filter1 == static_cast<int>(FILTER1ROUTE::FILTER1ROUTE_Master)) {
        m_Set.m_RoutingBuffers.MasterOutBuffer->addFrom(0, 0, m_Set.m_RoutingBuffers.FilterBuffer[0]->getReadPointer(0, 0), numFrames); //gain??
        m_Set.m_RoutingBuffers.MasterOutBuffer->addFrom(1, 0, m_Set.m_RoutingBuffers.FilterBuffer[0]->getReadPointer(1, 0), numFrames); //gain??
    }
    if (*m_Set.m_State->m_uFilterRouting_Filter2 == static_cast<int>(FILTER2ROUTE::FILTER2ROUTE_Master)) {
        m_Set.m_RoutingBuffers.MasterOutBuffer->addFrom(0, 0, m_Set.m_RoutingBuffers.FilterBuffer[1]->getReadPointer(0, 0), numFrames); //gain??
        m_Set.m_RoutingBuffers.MasterOutBuffer->addFrom(1, 0, m_Set.m_RoutingBuffers.FilterBuffer[1]->getReadPointer(1, 0), numFrames); //gain??
    }
    if (*m_Set.m_State->m_uFilterRouting2_Filter2 == static_cast<int>(FILTER2ROUTE::FILTER2ROUTE_Master)) {
        m_Set.m_RoutingBuffers.MasterOutBuffer->addFrom(0, 0, m_Set.m_RoutingBuffers.FilterBuffer[1]->getReadPointer(0, 0), numFrames); //gain??
        m_Set.m_RoutingBuffers.MasterOutBuffer->addFrom(1, 0, m_Set.m_RoutingBuffers.FilterBuffer[1]->getReadPointer(1, 0), numFrames); //gain??
    }
    if (*m_Set.m_State->m_uFilterRouting_Filter3 == static_cast<int>(FILTER3ROUTE::FILTER3ROUTE_Master)) {
        m_Set.m_RoutingBuffers.MasterOutBuffer->addFrom(0, 0, m_Set.m_RoutingBuffers.FilterBuffer[2]->getReadPointer(0, 0), numFrames); //gain??
        m_Set.m_RoutingBuffers.MasterOutBuffer->addFrom(1, 0, m_Set.m_RoutingBuffers.FilterBuffer[2]->getReadPointer(1, 0), numFrames); //gain??
    }
    if (*m_Set.m_State->m_uFilterRouting2_Filter3 == static_cast<int>(FILTER3ROUTE::FILTER3ROUTE_Master)) {
        m_Set.m_RoutingBuffers.MasterOutBuffer->addFrom(0, 0, m_Set.m_RoutingBuffers.FilterBuffer[2]->getReadPointer(0, 0), numFrames); //gain??
        m_Set.m_RoutingBuffers.MasterOutBuffer->addFrom(1, 0, m_Set.m_RoutingBuffers.FilterBuffer[2]->getReadPointer(1, 0), numFrames); //gain??
    }
    //fxbusses
    if (*m_Set.m_State->m_uFxBusRouting == static_cast<int>(FXBUSROUTE::FXBUSROUTE_Master)) {
        m_Set.m_RoutingBuffers.MasterOutBuffer->addFrom(0, 0, m_Set.m_RoutingBuffers.FxBusBuffer[0]->getReadPointer(0, 0), numFrames); //gain??
        m_Set.m_RoutingBuffers.MasterOutBuffer->addFrom(1, 0, m_Set.m_RoutingBuffers.FxBusBuffer[0]->getReadPointer(1, 0), numFrames); //gain??
    }
    if (*m_Set.m_State->m_uFxBusRouting_Bus2 == static_cast<int>(FXBUSROUTE::FXBUSROUTE_Master)) {
        m_Set.m_RoutingBuffers.MasterOutBuffer->addFrom(0, 0, m_Set.m_RoutingBuffers.FxBusBuffer[1]->getReadPointer(0, 0), numFrames); //gain??
        m_Set.m_RoutingBuffers.MasterOutBuffer->addFrom(1, 0, m_Set.m_RoutingBuffers.FxBusBuffer[1]->getReadPointer(1, 0), numFrames); //gain??
    }
    if (*m_Set.m_State->m_uFxBusRouting_Bus3 == static_cast<int>(FXBUSROUTE::FXBUSROUTE_Master)) {
        m_Set.m_RoutingBuffers.MasterOutBuffer->addFrom(0, 0, m_Set.m_RoutingBuffers.FxBusBuffer[2]->getReadPointer(0, 0), numFrames); //gain??
        m_Set.m_RoutingBuffers.MasterOutBuffer->addFrom(1, 0, m_Set.m_RoutingBuffers.FxBusBuffer[2]->getReadPointer(1, 0), numFrames); //gain??
    }
    
    if (uNumOutChannels > 0)
        buffer.copyFrom(0, 0, m_Set.m_RoutingBuffers.MasterOutBuffer->getReadPointer(0, 0), numFrames, m_Set.m_fMasterVolume); // directly apply gain!
    if (uNumOutChannels > 1)
        buffer.copyFrom(1, 0, m_Set.m_RoutingBuffers.MasterOutBuffer->getReadPointer(1, 0), numFrames, m_Set.m_fMasterVolume); // directly apply gain!
    
    //=================================================================================================
    
    //check filter empty and processing completed
    bool bClearBufferL = false;
	bool bClearBufferR = false;
    if (uNumOutChannels > 0) {
        Range<float> minmaxL = buffer.findMinMax(0, 0, numFrames);
        if ((minmaxL.getStart() >= OUTPUT_MIN_MINUS) && (minmaxL.getEnd() <= OUTPUT_MIN_PLUS))
			bClearBufferL = true;
    }
    if (uNumOutChannels > 1) {
        Range<float> minmaxR = buffer.findMinMax(1, 0, numFrames);
        if ((minmaxR.getStart() >= OUTPUT_MIN_MINUS) && (minmaxR.getEnd() <= OUTPUT_MIN_PLUS))
			bClearBufferR = true;
    }
    if (bClearBufferL && bClearBufferR) {
        m_bLastChainBufferZero = true;
        m_bBufferZeroMilliSeconds += ceil(1000.f * float(numFrames) / float(m_Set.m_nSampleRate));
        buffer.clear(); // underflow check
    }
    else {
        m_bLastChainBufferZero = false;
        m_bBufferZeroMilliSeconds = 0;
    }
    
    //Fade out when program change
    if (m_iFadeOutSamples > 0) {
        float startVal = m_iFadeOutSamples / float(m_iMaxFadeSamples);
        float endVal = (m_iFadeOutSamples - numFrames) / float(m_iMaxFadeSamples);
        startVal = jlimit<float>(0.f, 1.f, startVal);
        endVal = jlimit<float>(0.f, 1.f, endVal);
        buffer.applyGainRamp(0, numFrames, startVal, endVal);
        m_iFadeOutSamples -= numFrames;
        if (m_iFadeOutSamples < 0)
            m_iFadeOutSamples = 0;
    }
    if (m_iFadeInSamples > 0) {
        float startVal = 1 - m_iFadeInSamples / float(m_iMaxFadeSamples);
        float endVal = 1 - (m_iFadeInSamples - numFrames) / float(m_iMaxFadeSamples);
        startVal = jlimit<float>(0.f, 1.f, startVal);
        endVal = jlimit<float>(0.f, 1.f, endVal);
        buffer.applyGainRamp(0, numFrames, startVal, endVal);
        m_iFadeInSamples -= numFrames;
        if (m_iFadeInSamples < 0)
            m_iFadeInSamples = 0;
    }
    
    endSoftFade();
    
#ifdef _DEBUG
    if (uNumOutChannels > 1) {
        //-------------------------------
        //check processing time
        std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
        float mfMaxAllowedMicroSecs = (1.0f / m_nSampleRate) * float(buffer.getNumSamples()) * 1000000;
        if (duration > mfMaxAllowedMicroSecs) {
            m_Set.m_cUnderruns++;
        }
        
        if (m_Set.m_bShallCatchClicks) {
            float l_clickTolerance = (48000.f / m_Set.m_nSampleRate) * 0.2f; //larger than 0.2 step at 48khz is way too much (too high freq)
            float* clickBuffer = buffer.getWritePointer(0);
            for (int i = 0; i < buffer.getNumSamples() - 1; i++) {
                if ((abs(clickBuffer[i] - clickBuffer[i + 1]) > l_clickTolerance) &&
                    (clickBuffer[i] * clickBuffer[i + 1] > 0.f)) { //above tolerance and not + to -
                    m_Set.m_bShallDump = true;
                    VDBG("!!!Click detected at samplepos: " << i << "! Dumping Log!");
                }
            }
        }
        if (m_Set.m_bShallDump) {
            myProcessor->dumpBuffers();
        } else
            if (myProcessor->isDumping()) { //flush now
                myProcessor->dumpBuffersFlush();
            }
    }
#endif

	//=================================================================================================
	return true;
}

void CVASTXperience::parameterUpdatesAfterInit() {
    m_Poly.updateVariables();
    m_Poly.updateLFO(0);
    m_Poly.updateLFO(1);
    m_Poly.updateLFO(2);
    m_Poly.updateLFO(3);
    m_Poly.updateLFO(4);
    m_Poly.initArp();
    m_Set.modMatrixCalcBuffers();
}

void CVASTXperience::setIsInitDefaults(bool defaults) {
    m_isInitDefaults.store(defaults);
}

bool CVASTXperience::getIsInitDefaults() {
    return m_isInitDefaults.load();
}

//// called only from GUI THREAD
void CVASTXperience::parameterChanged(const String& parameterID, float newValue) {
	if (myProcessor->initNotCompleted()) return;

	//-------------------------------------------------------------------------------------
	// here stuff that execs only if param was changed
		//only when changed - example: if (*m_Set.m_State->m_iLFOSteps_LFO1 != *m_Set.m_State->m_iLFOSteps_LFO1) {

	//-------------------------------------------------------------------------------------
	// normal post procs

	bool l_isBlocked = nonThreadsafeIsBlockedProcessingInfo();
    bool l_isInitDefaults = getIsInitDefaults();
	const ScopedLock sl(paramChangeLock);

	if (0 == parameterID.compare("m_uPolyMode")) {
				
		if (*m_Set.m_State->m_uPolyMode == static_cast<int>(POLYMODE::MONO))
			m_Set.m_uTargetMaxPoly = 1;
		else
			if (*m_Set.m_State->m_uPolyMode == static_cast<int>(POLYMODE::POLY4))
				m_Set.m_uTargetMaxPoly = 4;
			else
				if (*m_Set.m_State->m_uPolyMode == static_cast<int>(POLYMODE::POLY16))
					m_Set.m_uTargetMaxPoly = 16;
				else
					if (*m_Set.m_State->m_uPolyMode == static_cast<int>(POLYMODE::POLY32))
						m_Set.m_uTargetMaxPoly = 32;
					else
					{
						vassertfalse;
						m_Set.m_uTargetMaxPoly = 16;
					}

		/*
		for (int i = 0; i < m_Set.m_uMaxPoly; i++)
			if (m_Poly.m_singleNote[i] != nullptr) //safety check for multiple host threads editing simultaneously
				m_Poly.m_singleNote[i]->stopNote(0, true); //allow tail off
			else
				return;

		bool bWasLocked = nonThreadsafeIsBlockedProcessingInfo();
		if (!bWasLocked) 
			if (!audioProcessLock()) {
				myProcessor->setErrorState(myProcessor->vastErrorState::errorState26_maxPolyNotSet);
				return; 
			}
		bool done = false;
		int counter = 0;
		while (!done) {
			if ((counter<30) && (!myProcessor->lockedAndSafeToDoDeallocatios())) {
				VDBG("PolyMode - sleep");
				Thread::sleep(100);
				counter++;
				continue;
			}
            vassert(counter<30);
			if (counter == 30) {
				myProcessor->setErrorState(myProcessor->vastErrorState::errorState26_maxPolyNotSet); 
				return; //dont unlock what is not locked
			}
			int olduMaxPoly = m_Set.m_uMaxPoly;
			if (*m_Set.m_State->m_uPolyMode == static_cast<int>(POLYMODE::MONO))
				m_Set.m_uMaxPoly = 1;
			else
				if (*m_Set.m_State->m_uPolyMode == static_cast<int>(POLYMODE::POLY4))
					m_Set.m_uMaxPoly = 4;
				else
					if (*m_Set.m_State->m_uPolyMode == static_cast<int>(POLYMODE::POLY16))
						m_Set.m_uMaxPoly = 16;
					else
						if (*m_Set.m_State->m_uPolyMode == static_cast<int>(POLYMODE::POLY32))
							m_Set.m_uMaxPoly = 32;
					else
					{
						vassertfalse;
						m_Set.m_uMaxPoly = 16;
					}
			VDBG("Polymode changed to " + String(m_Set.m_uMaxPoly));
			if (olduMaxPoly != m_Set.m_uMaxPoly)
				m_Poly.releaseResources();
			if (!bWasLocked)
				if (!audioProcessUnlock()) {
					myProcessor->setErrorState(myProcessor->vastErrorState::errorState26_maxPolyNotSet);
				}
			done = true;
		}
		*/
		return;		
	}

	// master volume - can be online
	if (0 == parameterID.compare("m_fMasterVolumedB")) {
		if (m_Poly.getLastSingleNoteCycleWasActive())
			m_Set.m_fMasterVolume_smoothed.setTargetValue(pow(10.0, *m_Set.m_State->m_fMasterVolumedB / 20.0));  // target -> soft update
		else 
			m_Set.m_fMasterVolume_smoothed.setCurrentAndTargetValue(pow(10.0, *m_Set.m_State->m_fMasterVolumedB / 20.0));  // direct
		return;
	}

	if (0 == parameterID.compare("m_fPortamento")) {
		for (int i = 0; i < m_Set.m_uMaxPoly; i++) {
			if (m_Poly.m_singleNote[i] != nullptr) //safety check for multiple host threads editing simultaneously
				m_Poly.m_singleNote[i]->setPortamentoTime(*m_Set.m_State->m_fPortamento);
		}
		if (!l_isBlocked && !l_isInitDefaults)
			m_Poly.updateVariables();
		return;
	}

	if (0 == parameterID.compare("m_fMasterTune")) {
		if (!l_isBlocked && !l_isInitDefaults)
			m_Poly.updateVariables();
		return;
	}

	//LFO 
	if (0 == parameterID.compare("m_uLFOTimeBeats_LFO1")) {
		if (!l_isBlocked && !l_isInitDefaults)
			m_Poly.updateLFO(0);
		return;
	}

	if (0 == parameterID.compare("m_uLFOTimeBeats_LFO2")) {
		if (!l_isBlocked && !l_isInitDefaults)
			m_Poly.updateLFO(1);
		return;
	}

	if (0 == parameterID.compare("m_uLFOTimeBeats_LFO3")) {
		if (!l_isBlocked && !l_isInitDefaults)
			m_Poly.updateLFO(2);
		return;
	}

	if (0 == parameterID.compare("m_uLFOTimeBeats_LFO4")) {
		if (!l_isBlocked && !l_isInitDefaults)
			m_Poly.updateLFO(3);
		return;
	}

	if (0 == parameterID.compare("m_uLFOTimeBeats_LFO5")) {
		if (!l_isBlocked && !l_isInitDefaults)
			m_Poly.updateLFO(4);
		return;
	}

	if (0 == parameterID.compare("m_bLFOPerVoice_LFO1")) {
		if (!l_isBlocked && !l_isInitDefaults)
			m_Poly.updateLFO(0);
		return;
	}

	if (0 == parameterID.compare("m_bLFOPerVoice_LFO2")) {
		if (!l_isBlocked && !l_isInitDefaults)
			m_Poly.updateLFO(1);
		return;
	}

	if (0 == parameterID.compare("m_bLFOPerVoice_LFO3")) {
		if (!l_isBlocked && !l_isInitDefaults)
			m_Poly.updateLFO(2);
		return;
	}

	if (0 == parameterID.compare("m_bLFOPerVoice_LFO4")) {
		if (!l_isBlocked && !l_isInitDefaults)
			m_Poly.updateLFO(3);
		return;
	}

	if (0 == parameterID.compare("m_bLFOPerVoice_LFO5")) {
		if (!l_isBlocked && !l_isInitDefaults)
			m_Poly.updateLFO(4);
		return;
	}
	if (0 == parameterID.compare("m_fLFOFreq_LFO1")) {
		if (!l_isBlocked && !l_isInitDefaults)
			m_Poly.updateLFO(0);
		return;
	}

	if (0 == parameterID.compare("m_fLFOFreq_LFO2")) {
		if (!l_isBlocked && !l_isInitDefaults)
			m_Poly.updateLFO(1);
		return;
	}
	
	if (0 == parameterID.compare("m_fLFOFreq_LFO3")) {
		if (!l_isBlocked && !l_isInitDefaults)
			m_Poly.updateLFO(2);
		return;
	}

	if (0 == parameterID.compare("m_fLFOFreq_LFO4")) {
		if (!l_isBlocked && !l_isInitDefaults)
			m_Poly.updateLFO(3);
		return;
	}

	if (0 == parameterID.compare("m_fLFOFreq_LFO5")) {
		if (!l_isBlocked && !l_isInitDefaults)
			m_Poly.updateLFO(4);
		return;
	}

	if (0 == parameterID.compare("m_uLFOWave_LFO1")) {
		if (!l_isBlocked && !l_isInitDefaults)
			m_Poly.updateLFO(0);
		return;
	}
	
	if (0 == parameterID.compare("m_uLFOWave_LFO2")) {
		if (!l_isBlocked && !l_isInitDefaults)
			m_Poly.updateLFO(1);
		return;
	}
	
	if (0 == parameterID.compare("m_uLFOWave_LFO3")) {		
		if (!l_isBlocked && !l_isInitDefaults)
			m_Poly.updateLFO(2);
		return;
	}

	if (0 == parameterID.compare("m_uLFOWave_LFO4")) {
		if (!l_isBlocked && !l_isInitDefaults)
			m_Poly.updateLFO(3);
		return;
	}
	
	if (0 == parameterID.compare("m_uLFOWave_LFO5")) {
		if (!l_isBlocked && !l_isInitDefaults)
			m_Poly.updateLFO(4);
		return;
	}

	if (0 == parameterID.compare("m_bLFOSynch_LFO1")) {
		if (!l_isBlocked && !l_isInitDefaults)
			m_Poly.updateLFO(0);
		return;
	}
	
	if (0 == parameterID.compare("m_bLFOSynch_LFO2")) {
		if (!l_isBlocked && !l_isInitDefaults)
			m_Poly.updateLFO(1);
		return;
	}
	
	if (0 == parameterID.compare("m_bLFOSynch_LFO3")) {
		if (!l_isBlocked && !l_isInitDefaults)
			m_Poly.updateLFO(2);
		return;
	}

	if (0 == parameterID.compare("m_bLFOSynch_LFO4")) {
		if (!l_isBlocked && !l_isInitDefaults)
			m_Poly.updateLFO(3);
		return;
	}

	if (0 == parameterID.compare("m_bLFOSynch_LFO5")) {
		if (!l_isBlocked && !l_isInitDefaults)
			m_Poly.updateLFO(4);
		return;
	}
	if (0 == parameterID.compare("m_uOscWave_OscA")) {
		if (!l_isBlocked && !l_isInitDefaults)
			m_Poly.updateVariables();
		return;
	}
	
	if (0 == parameterID.compare("m_uOscWave_OscB")) {
		if (!l_isBlocked && !l_isInitDefaults)
			m_Poly.updateVariables();
		return;
	}
	
	if (0 == parameterID.compare("m_uOscWave_OscC")) {
		if (!l_isBlocked && !l_isInitDefaults)
			m_Poly.updateVariables();
		return;
	}
	
	if (0 == parameterID.compare("m_uOscWave_OscD")) {
		if (!l_isBlocked && !l_isInitDefaults)
			m_Poly.updateVariables();
		return;
	}
	
	if ((0 == parameterID.compare("m_iNumOscs_OscA"))) {
		if (!l_isBlocked && !l_isInitDefaults)
			m_Poly.updateVariables();
		return;
	}
	
	if ((0 == parameterID.compare("m_iNumOscs_OscB"))) {
		if (!l_isBlocked && !l_isInitDefaults)
			m_Poly.updateVariables();
		return;
	}
	
	if ((0 == parameterID.compare("m_iNumOscs_OscC"))) {
		if (!l_isBlocked && !l_isInitDefaults)
			m_Poly.updateVariables();
		return;
	}

	if ((0 == parameterID.compare("m_iNumOscs_OscD"))) {
		if (!l_isBlocked && !l_isInitDefaults)
			m_Poly.updateVariables();
		return;
	}
	
	if ((0 == parameterID.compare("m_fOscWTPos_OscA"))) {
		m_Poly.m_OscBank[0].setChangedFlag(); 
		return;
	}

	if ((0 == parameterID.compare("m_fOscWTPos_OscB"))) {
		m_Poly.m_OscBank[1].setChangedFlag();
		return;
	}

	if ((0 == parameterID.compare("m_fOscWTPos_OscC"))) {
		m_Poly.m_OscBank[2].setChangedFlag();
		return;
	}

	if ((0 == parameterID.compare("m_fOscWTPos_OscD"))) {
		m_Poly.m_OscBank[3].setChangedFlag();
		return;
	}

	if ((0 == parameterID.compare("m_bOscOnOff_OscA"))) {
		m_Poly.m_OscBank[0].setChangedFlag();
		if (!l_isBlocked && !l_isInitDefaults)
			m_Poly.updateVariables();
		return;
	}
	
	if ((0 == parameterID.compare("m_bOscOnOff_OscB"))) {
		m_Poly.m_OscBank[1].setChangedFlag();
		if (!l_isBlocked && !l_isInitDefaults)
			m_Poly.updateVariables();
		return;
	}
	
	if ((0 == parameterID.compare("m_bOscOnOff_OscC"))) {
		m_Poly.m_OscBank[2].setChangedFlag();
		if (!l_isBlocked && !l_isInitDefaults)
			m_Poly.updateVariables();
		return;
	}
	
	if ((0 == parameterID.compare("m_bOscOnOff_OscD"))) {
		m_Poly.m_OscBank[3].setChangedFlag();
		if (!l_isBlocked && !l_isInitDefaults)
			m_Poly.updateVariables();
		return;
	}
	
	if ((0 == parameterID.compare("m_bNoiseOnOff"))) {
		if (!l_isBlocked && !l_isInitDefaults)
			m_Poly.updateVariables();
		return;
	}

	if (0 == parameterID.compare("m_iOscOct_OscA")) {
		if (!l_isBlocked && !l_isInitDefaults)
			m_Poly.updateVariables();
		return;
	}
	
	if (0 == parameterID.compare("m_iOscOct_OscB")) {
		if (!l_isBlocked && !l_isInitDefaults)
			m_Poly.updateVariables();
		return;
	}
	
	if (0 == parameterID.compare("m_iOscOct_OscC")) {
		if (!l_isBlocked && !l_isInitDefaults)
			m_Poly.updateVariables();
		return;
	}
	
	if (0 == parameterID.compare("m_iOscOct_OscD")) {
		if (!l_isBlocked && !l_isInitDefaults)
			m_Poly.updateVariables();
		return;
	}
	
	if (0 == parameterID.compare("m_fOscDetune_OscA")) {
		if (!l_isBlocked && !l_isInitDefaults)
			m_Poly.updateVariables();
		return;
	}
	
	if (0 == parameterID.compare("m_fOscDetune_OscB")) {
		if (!l_isBlocked && !l_isInitDefaults)
			m_Poly.updateVariables();
		return;
	}
	
	if (0 == parameterID.compare("m_fOscDetune_OscC")) {
		if (!l_isBlocked && !l_isInitDefaults)
			m_Poly.updateVariables();
		return;
	}
	
	if (0 == parameterID.compare("m_fOscDetune_OscD")) {
		if (!l_isBlocked && !l_isInitDefaults)
			m_Poly.updateVariables();
		return;
	}
	
	if (0 == parameterID.compare("m_fOscMorph_OscA")) {
		for (int i=0; i < m_Set.m_uMaxPoly; i++)
			if (m_Poly.m_singleNote[i] != nullptr) //safety check for multiple host threads editing simultaneously
				m_Poly.m_singleNote[i]->setWTPosSmooth(0); //bank 0
		return;
	}
	if (0 == parameterID.compare("m_fOscMorph_OscB")) {
		for (int i = 0; i < m_Set.m_uMaxPoly; i++)
			if (m_Poly.m_singleNote[i] != nullptr) //safety check for multiple host threads editing simultaneously
				m_Poly.m_singleNote[i]->setWTPosSmooth(1); //bank 1
		return;
	}
	if (0 == parameterID.compare("m_fOscMorph_OscC")) {
		for (int i = 0; i < m_Set.m_uMaxPoly; i++)
			if (m_Poly.m_singleNote[i] != nullptr) //safety check for multiple host threads editing simultaneously
				m_Poly.m_singleNote[i]->setWTPosSmooth(3); //bank 2
		return;
	}
	if (0 == parameterID.compare("m_fOscMorph_OscD")) {
		for (int i = 0; i < m_Set.m_uMaxPoly; i++)
			if (m_Poly.m_singleNote[i] != nullptr) //safety check for multiple host threads editing simultaneously
				m_Poly.m_singleNote[i]->setWTPosSmooth(4); //bank 3
		return;
	}
	
	if (0 == parameterID.compare("m_uVCAEnvMode")) {
		if (!l_isBlocked && !l_isInitDefaults)
			m_Poly.updateVariables();
		return;
	}
	
	if (0 == parameterID.compare("m_uVCFEnvMode")) {
		if (!l_isBlocked && !l_isInitDefaults)
			m_Poly.updateVariables();
		return;
	}
	//FX

	if (0 == parameterID.compare("m_bOversampleBus")) {
		m_bOversampleBus1_changed = true;
		return;
	}
	
	if (0 == parameterID.compare("m_bOversampleBus_Bus2")) {
		m_bOversampleBus2_changed = true;
		return;
	}
	
	if (0 == parameterID.compare("m_bOversampleBus_Bus3")) {
		m_bOversampleBus3_changed = true;
		return;
	}

	//SAMPLER
	
	if (0 == parameterID.compare("m_uSamplerRootKey")) {
		VASTSamplerSound* sound = (VASTSamplerSound*)m_Poly.getSamplerSoundChanged();
		if (sound != nullptr)
			sound->setMidiRootNode(*m_Set.m_State->m_uSamplerRootKey);
		sound = (VASTSamplerSound*)m_Poly.getSamplerSound();
		if (sound != nullptr) {
			sound->setMidiRootNode(*m_Set.m_State->m_uSamplerRootKey);
			for (int i = 0; i < m_Set.m_uMaxPoly; i++) {
				if (m_Poly.m_singleNote[i] != nullptr) //safety check for multiple host threads editing simultaneously
					m_Poly.m_singleNote[i]->samplerUpdatePitch(sound, true);
			}
		}
		return;
	}

	if (0 == parameterID.compare("m_fSamplerCents")) {
		VASTSamplerSound* sound = (VASTSamplerSound*)m_Poly.getSamplerSound();
		if (sound != nullptr) {
			for (int i = 0; i < m_Set.m_uMaxPoly; i++) {
				if (m_Poly.m_singleNote[i] != nullptr) //safety check for multiple host threads editing simultaneously
					m_Poly.m_singleNote[i]->samplerUpdatePitch(sound, true);
			}
		}
		return;
	}

	if (0 == parameterID.compare("m_bSamplerKeytrack")) {
		//check todo
		return;
	}

	//ARP
	if (0 == parameterID.compare("m_bARPOnOff")) {
        if (!l_isInitDefaults)
            m_Poly.initArp();
		return;
	}

	if (0 == parameterID.compare("m_bARPOnOff")) {
        if (!l_isInitDefaults)
            m_Poly.initArp();
		return;
	}

	if (0 == parameterID.compare("m_fARPSpeed")) {
		//m_Poly.initArp(); //CHECK
		return;
	}

	// MOD MATRIX
	if (true == parameterID.startsWith("m_uModMatSrce")) { //starts with!
		int slot = String(parameterID.removeCharacters("m_uModMatSrce")).getIntValue() - 1;
		int oldVal = m_Set.modMatrixSlotDest[slot];
        if (!l_isInitDefaults)
            m_Set.modMatrixCalcBuffers();
		int newVal = m_Set.modMatrixSlotDest[slot];
		myProcessor->requestUIUpdate(false, false, true, oldVal, newVal); //only particular sliders shall be updated
		return;
	}

	if (true == parameterID.startsWith("m_uModMatDest")) { //starts with!
		int slot = String(parameterID.removeCharacters("m_uModMatDest")).getIntValue() - 1;		
		int oldVal = m_Set.modMatrixSlotDest[slot];
        if (!l_isInitDefaults)
            m_Set.modMatrixCalcBuffers();
		int newVal = m_Set.modMatrixSlotDest[slot];
		myProcessor->requestUIUpdate(false, false, true, oldVal, newVal); //only particular sliders shall be updated
		return;
	}

	if (true == parameterID.startsWith("m_fModMatVal")) { //starts with!
		int slot = String(parameterID.removeCharacters("m_fModMatVal")).getIntValue() - 1;
		int oldVal = m_Set.modMatrixSlotDest[slot];
        if (!l_isInitDefaults)
            m_Set.modMatrixCalcBuffers();
		int newVal = m_Set.modMatrixSlotDest[slot];
		myProcessor->requestUIUpdate(false, true, true, oldVal, newVal); //only particular sliders shall be updated
		return;
	}

	if (true == parameterID.startsWith("m_fModMatCurve")) { //starts with!
		myProcessor->requestUIUpdate(false, true, false);		
		return;
	}

	if (true == parameterID.startsWith("m_uModMatPolarity")) { //starts with!
		int slot = String(parameterID.removeCharacters("m_uModMatPolarity")).getIntValue() - 1;
		int oldVal = m_Set.modMatrixSlotDest[slot];
        if (!l_isInitDefaults)
            m_Set.modMatrixCalcBuffers();
		int newVal = m_Set.modMatrixSlotDest[slot];
		myProcessor->requestUIUpdate(false, true, true, oldVal, newVal); //only particular sliders shall be updated
		return;
	}

	//MSEGs
	if (true == parameterID.startsWith("m_fAttackSteps_MSEG")) {
		int mseg = String(parameterID.removeCharacters("m_fAttackSteps_MSEG")).getIntValue() - 1;
		if (getBlockProcessing() == false) {	
			m_Set.m_MSEGData[mseg].setAttackSteps(newValue, &m_Set);
			m_Set.m_MSEGData_changed[mseg].setAttackSteps(newValue, &m_Set);
		}
		return;
	}
	if (true == parameterID.startsWith("m_fDecaySteps_MSEG")) {
		int mseg = String(parameterID.removeCharacters("m_fDecaySteps_MSEG")).getIntValue() - 1;
		if (getBlockProcessing() == false) {
			m_Set.m_MSEGData[mseg].setDecaySteps(newValue, &m_Set);
			m_Set.m_MSEGData_changed[mseg].setDecaySteps(newValue, &m_Set);
		}
		return;
	}
	if (true == parameterID.startsWith("m_fReleaseSteps_MSEG")) {
		int mseg = String(parameterID.removeCharacters("m_fReleaseSteps_MSEG")).getIntValue() - 1;
		if (getBlockProcessing() == false) {
			m_Set.m_MSEGData[mseg].setReleaseSteps(newValue, &m_Set);
			m_Set.m_MSEGData_changed[mseg].setReleaseSteps(newValue, &m_Set);
		}
		return;
	}

	if (true == parameterID.startsWith("m_bMSEGSynch_MSEG")) {
		int mseg = String(parameterID.removeCharacters("m_bMSEGSynch_MSEG")).getIntValue() - 1;
		if (getBlockProcessing() == false) {
			m_Set.m_MSEGData[mseg].setSynch(newValue); //float to bool?
			m_Set.m_MSEGData_changed[mseg].setSynch(newValue); //float to bool?
			if (mseg == 0) {
				m_Set.m_MSEGData[mseg].setTimeBeats(*m_Set.m_State->m_uMSEGTimeBeats_MSEG1);
				m_Set.m_MSEGData_changed[mseg].setTimeBeats(*m_Set.m_State->m_uMSEGTimeBeats_MSEG1); //float to int?
			} else 
				if (mseg == 1) {
					m_Set.m_MSEGData[mseg].setTimeBeats(*m_Set.m_State->m_uMSEGTimeBeats_MSEG2);
					m_Set.m_MSEGData_changed[mseg].setTimeBeats(*m_Set.m_State->m_uMSEGTimeBeats_MSEG2); //float to int?
				}
				else
					if (mseg == 2) {
						m_Set.m_MSEGData[mseg].setTimeBeats(*m_Set.m_State->m_uMSEGTimeBeats_MSEG3);
						m_Set.m_MSEGData_changed[mseg].setTimeBeats(*m_Set.m_State->m_uMSEGTimeBeats_MSEG3); //float to int?
					}
					else
						if (mseg == 3) {
							m_Set.m_MSEGData[mseg].setTimeBeats(*m_Set.m_State->m_uMSEGTimeBeats_MSEG4);
							m_Set.m_MSEGData_changed[mseg].setTimeBeats(*m_Set.m_State->m_uMSEGTimeBeats_MSEG4); //float to int?
						}
						else
							if (mseg == 4) {
								m_Set.m_MSEGData[mseg].setTimeBeats(*m_Set.m_State->m_uMSEGTimeBeats_MSEG5);
								m_Set.m_MSEGData_changed[mseg].setTimeBeats(*m_Set.m_State->m_uMSEGTimeBeats_MSEG5); //float to int?
							};

			//hack should be in set time beats!
			if (bool(newValue) == true) {				
				m_Set.m_MSEGData[mseg].setAttackSteps(m_Set.m_MSEGData[mseg].calcStepsFromTime(m_Set.m_MSEGData[mseg].getAttackTime(), &m_Set), &m_Set);
				m_Set.m_MSEGData_changed[mseg].setAttackSteps(m_Set.m_MSEGData_changed[mseg].calcStepsFromTime(m_Set.m_MSEGData_changed[mseg].getAttackTime(), &m_Set), &m_Set);
				m_Set.m_MSEGData[mseg].setDecaySteps(m_Set.m_MSEGData[mseg].calcStepsFromTime(m_Set.m_MSEGData[mseg].getDecayTime(), &m_Set), &m_Set);
				m_Set.m_MSEGData_changed[mseg].setDecaySteps(m_Set.m_MSEGData_changed[mseg].calcStepsFromTime(m_Set.m_MSEGData_changed[mseg].getDecayTime(), &m_Set), &m_Set);
				m_Set.m_MSEGData[mseg].setReleaseSteps(m_Set.m_MSEGData[mseg].calcStepsFromTime(m_Set.m_MSEGData[mseg].getReleaseTime(), &m_Set), &m_Set);
				m_Set.m_MSEGData_changed[mseg].setReleaseSteps(m_Set.m_MSEGData_changed[mseg].calcStepsFromTime(m_Set.m_MSEGData_changed[mseg].getReleaseTime(), &m_Set), &m_Set);
			}
		}
		return;
	}
	if (true == parameterID.startsWith("m_uMSEGTimeBeats_MSEG")) {
		int mseg = String(parameterID.removeCharacters("m_uMSEGTimeBeats_MSEG")).getIntValue() - 1;
		if (getBlockProcessing() == false) {
			m_Set.m_MSEGData[mseg].setTimeBeats(newValue); //float to int?
			m_Set.m_MSEGData_changed[mseg].setTimeBeats(newValue); //float to int?

			//hack should be in set time beats!
			if (m_Set.m_MSEGData[mseg].getSynch()) {
				m_Set.m_MSEGData[mseg].setAttackSteps(m_Set.m_MSEGData[mseg].getAttackSteps(), &m_Set);
				m_Set.m_MSEGData_changed[mseg].setAttackSteps(m_Set.m_MSEGData_changed[mseg].getAttackSteps(), &m_Set);
				m_Set.m_MSEGData[mseg].setDecaySteps(m_Set.m_MSEGData[mseg].getDecaySteps(), &m_Set);
				m_Set.m_MSEGData_changed[mseg].setDecaySteps(m_Set.m_MSEGData_changed[mseg].getDecaySteps(), &m_Set);
				m_Set.m_MSEGData[mseg].setReleaseSteps(m_Set.m_MSEGData[mseg].getReleaseSteps(), &m_Set);
				m_Set.m_MSEGData_changed[mseg].setReleaseSteps(m_Set.m_MSEGData_changed[mseg].getReleaseSteps(), &m_Set);
			}
		}
		return;
	}

	if (true == parameterID.startsWith("m_fAttackTime_MSEG")) {
		int mseg = String(parameterID.removeCharacters("m_fAttackTime_MSEG")).getIntValue() - 1;
		if (getBlockProcessing() == false) {
			m_Set.m_MSEGData[mseg].setAttackTimeSlider(newValue); // in ms	
			m_Set.m_MSEGData_changed[mseg].setAttackTimeSlider(newValue); // in ms
		}
		return;
	}
	
	if (true == parameterID.startsWith("m_fDecayTime_MSEG")) {
		int mseg = String(parameterID.removeCharacters("m_fDecayTime_MSEG")).getIntValue() - 1;
		if (getBlockProcessing() == false) {
			m_Set.m_MSEGData[mseg].setDecayTimeSlider(newValue); // in ms	
			m_Set.m_MSEGData_changed[mseg].setDecayTimeSlider(newValue); // in ms
		}
		return;
	}
	
	if (true == parameterID.startsWith("m_fReleaseTime_MSEG")) {
		int mseg = String(parameterID.removeCharacters("m_fReleaseTime_MSEG")).getIntValue() - 1;
		if (getBlockProcessing() == false) {
			m_Set.m_MSEGData[mseg].setReleaseTimeSlider(newValue); // in ms	
			m_Set.m_MSEGData_changed[mseg].setReleaseTimeSlider(newValue); // in ms
		}
		return;
	}
	
	if (true == parameterID.startsWith("m_fSustainLevel_MSEG")) {
		int mseg = String(parameterID.removeCharacters("m_fSustainLevel_MSEG")).getIntValue() - 1;
		if (getBlockProcessing() == false) {
			m_Set.m_MSEGData[mseg].setSustainLevelSlider(newValue / 100.f); // 0..1	
			m_Set.m_MSEGData_changed[mseg].setSustainLevelSlider(newValue / 100.f); // 0..1
		}
		return;
	}


	//Stepseq
	if (0 == parameterID.compare("m_fGlide_STEPSEQ1")) {
		//must not change m_StepSeqData due to vector
		m_Set.m_StepSeqData_changed[0].stepSeqChangeGlide(*m_Set.m_State->m_fGlide_STEPSEQ1);
		return;
	}
	
	if (0 == parameterID.compare("m_fGate_STEPSEQ1")) {
		//must not change m_StepSeqData due to vector
		m_Set.m_StepSeqData_changed[0].stepSeqChangeGate(*m_Set.m_State->m_fGate_STEPSEQ1);
		return;
	}

	if (0 == parameterID.compare("m_fGlide_STEPSEQ2")) {
		//must not change m_StepSeqData due to vector
		m_Set.m_StepSeqData_changed[1].stepSeqChangeGlide(*m_Set.m_State->m_fGlide_STEPSEQ2);
		return;
	}
	
	if (0 == parameterID.compare("m_fGate_STEPSEQ2")) {
		//must not change m_StepSeqData due to vector
		m_Set.m_StepSeqData_changed[1].stepSeqChangeGate(*m_Set.m_State->m_fGate_STEPSEQ2);
		return;
	}
	
	if (0 == parameterID.compare("m_fGlide_STEPSEQ3")) {
		//must not change m_StepSeqData due to vector
		m_Set.m_StepSeqData_changed[2].stepSeqChangeGlide(*m_Set.m_State->m_fGlide_STEPSEQ3);
		return;
	}

	if (0 == parameterID.compare("m_fGate_STEPSEQ3")) {
		//must not change m_StepSeqData due to vector
		m_Set.m_StepSeqData_changed[2].stepSeqChangeGate(*m_Set.m_State->m_fGate_STEPSEQ3);
		return;
	}

	if (0 == parameterID.compare("m_bStepSeqSynch_STEPSEQ1")) {

		if (*m_Set.m_State->m_bStepSeqSynch_STEPSEQ1 == static_cast<int>(SWITCH::SWITCH_ON)) {
			float l_fIntervalTime = m_Set.getIntervalTimeFromDAWBeats(*m_Set.m_State->m_uStepSeqTimeBeats_STEPSEQ1);
			m_Set.m_StepSeqData[0].setStepSeqTime(l_fIntervalTime); //is in ms
			m_Set.m_StepSeqData_changed[0].setStepSeqTime(l_fIntervalTime); //is in ms

		} else {
			m_Set.m_StepSeqData[0].setStepSeqTime(*m_Set.m_State->m_fStepSeqSpeed_STEPSEQ1); //is in ms
			m_Set.m_StepSeqData_changed[0].setStepSeqTime(*m_Set.m_State->m_fStepSeqSpeed_STEPSEQ1); //is in ms
		}

		/*
		if (*m_Set->m_State->m_bStepSeqSynch_STEPSEQ1 == static_cast<int>(SWITCH::SWITCH_ON))
		m_Set->m_StepSeqData[0].setStepSeqTime(*m_Set->m_State->m_fStepSeqSpeed_STEPSEQ1); //is in ms
		if (*m_Set->m_State->m_bStepSeqSynch_STEPSEQ2 == static_cast<int>(SWITCH::SWITCH_ON))
		m_Set->m_StepSeqData[1].setStepSeqTime(*m_Set->m_State->m_fStepSeqSpeed_STEPSEQ2); //is in ms
		if (*m_Set->m_State->m_bStepSeqSynch_STEPSEQ3 == static_cast<int>(SWITCH::SWITCH_ON))
		m_Set->m_StepSeqData[2].setStepSeqTime(*m_Set->m_State->m_fStepSeqSpeed_STEPSEQ3); //is in ms
		*/

		/*
		if (*m_Set.m_State->m_bStepSeqSynch_STEPSEQ1 == static_cast<int>(SWITCH::SWITCH_ON)) {
			if (m_Set.m_dPpqBpm == 0.f) return;
			float l_fIntervalTime = m_Set.getIntervalTimeFromDAWBeats(*m_Set.m_State->m_uStepSeqTimeBeats_STEPSEQ1);
			//if (l_fIntervalTime < 0.1f) l_fIntervalTime = 0.1f; // minimum //CHECK
			//if (l_fIntervalTime > 5000.0f) l_fIntervalTime = 5000.f; // maximum //CHECK
			m_Set.m_StepSeqData[0].setStepSeqTime(l_fIntervalTime); //is in ms
		}
		else {
			m_Poly.m_StepSeq_Envelope[0].init(m_Set, m_Set.m_StepSeqData[0], 0); //voiceno 0??
			struct timeval tp;
			m_Set._gettimeofday(&tp);
			m_Poly.m_StepSeq_Envelope[0].noteOn(tp.tv_sec * 1000 + tp.tv_usec / 1000, false);
		}
		*/
		return;
	}

	if (0 == parameterID.compare("m_bStepSeqSynch_STEPSEQ2")) {
		if (*m_Set.m_State->m_bStepSeqSynch_STEPSEQ2 == static_cast<int>(SWITCH::SWITCH_ON)) {
			float l_fIntervalTime = m_Set.getIntervalTimeFromDAWBeats(*m_Set.m_State->m_uStepSeqTimeBeats_STEPSEQ2);
			m_Set.m_StepSeqData[1].setStepSeqTime(l_fIntervalTime); //is in ms
			m_Set.m_StepSeqData_changed[1].setStepSeqTime(l_fIntervalTime); //is in ms

		}
		else {
			m_Set.m_StepSeqData[1].setStepSeqTime(*m_Set.m_State->m_fStepSeqSpeed_STEPSEQ2); //is in ms
			m_Set.m_StepSeqData_changed[1].setStepSeqTime(*m_Set.m_State->m_fStepSeqSpeed_STEPSEQ2); //is in ms
		}


		/*
		if (*m_Set.m_State->m_bStepSeqSynch_STEPSEQ2 == static_cast<int>(SWITCH::SWITCH_ON)) {
			if (m_Set.m_dPpqBpm == 0.f) return;
			float l_fIntervalTime = m_Set.getIntervalTimeFromDAWBeats(*m_Set.m_State->m_uStepSeqTimeBeats_STEPSEQ2);
			//if (l_fIntervalTime < 0.1f) l_fIntervalTime = 0.1f; // minimum //CHECK
			//if (l_fIntervalTime > 5000.0f) l_fIntervalTime = 5000.f; // maximum //CHECK
			m_Set.m_StepSeqData[1].setStepSeqTime(l_fIntervalTime); //is in ms

		}
		else {
			m_Poly.m_StepSeq_Envelope[1].init(m_Set, m_Set.m_StepSeqData[1], 0); //voiceno 0??
			struct timeval tp;
			m_Set._gettimeofday(&tp);
			m_Poly.m_StepSeq_Envelope[1].noteOn(tp.tv_sec * 1000 + tp.tv_usec / 1000, false);
		}
		*/
		return;
	}

	if (0 == parameterID.compare("m_bStepSeqSynch_STEPSEQ3")) {
		if (*m_Set.m_State->m_bStepSeqSynch_STEPSEQ3 == static_cast<int>(SWITCH::SWITCH_ON)) {
			float l_fIntervalTime = m_Set.getIntervalTimeFromDAWBeats(*m_Set.m_State->m_uStepSeqTimeBeats_STEPSEQ3);
			m_Set.m_StepSeqData[2].setStepSeqTime(l_fIntervalTime); //is in ms
			m_Set.m_StepSeqData_changed[2].setStepSeqTime(l_fIntervalTime); //is in ms

		}
		else {
			m_Set.m_StepSeqData[2].setStepSeqTime(*m_Set.m_State->m_fStepSeqSpeed_STEPSEQ3); //is in ms
			m_Set.m_StepSeqData_changed[2].setStepSeqTime(*m_Set.m_State->m_fStepSeqSpeed_STEPSEQ3); //is in ms
		}

		/*
		if (*m_Set.m_State->m_bStepSeqSynch_STEPSEQ3 == static_cast<int>(SWITCH::SWITCH_ON)) {
			if (m_Set.m_dPpqBpm == 0.f) return;
			float l_fIntervalTime = m_Set.getIntervalTimeFromDAWBeats(*m_Set.m_State->m_uStepSeqTimeBeats_STEPSEQ3);
			//if (l_fIntervalTime < 0.1f) l_fIntervalTime = 0.1f; // minimum //CHECK
			//if (l_fIntervalTime > 5000.0f) l_fIntervalTime = 5000.f; // maximum //CHECK
			m_Set.m_StepSeqData[2].setStepSeqTime(l_fIntervalTime); //is in ms
		}
		else {
			m_Poly.m_StepSeq_Envelope[2].init(m_Set, m_Set.m_StepSeqData[2], 0); //voiceno 0??
			struct timeval tp;
			m_Set._gettimeofday(&tp);
			m_Poly.m_StepSeq_Envelope[2].noteOn(tp.tv_sec * 1000 + tp.tv_usec / 1000, false);
		}
		*/
		return;
	}

	if (0 == parameterID.compare("m_fStepSeqSpeed_STEPSEQ1")) {
		if (*m_Set.m_State->m_bStepSeqSynch_STEPSEQ1 == static_cast<int>(SWITCH::SWITCH_OFF)) {
			m_Set.m_StepSeqData[0].setStepSeqTime(*m_Set.m_State->m_fStepSeqSpeed_STEPSEQ1); //is in ms
			m_Set.m_StepSeqData_changed[0].setStepSeqTime(*m_Set.m_State->m_fStepSeqSpeed_STEPSEQ1); //is in ms
		}
		return;
	}

	if (0 == parameterID.compare("m_fStepSeqSpeed_STEPSEQ2")) {
		if (*m_Set.m_State->m_bStepSeqSynch_STEPSEQ2 == static_cast<int>(SWITCH::SWITCH_OFF)) {
			m_Set.m_StepSeqData[1].setStepSeqTime(*m_Set.m_State->m_fStepSeqSpeed_STEPSEQ2); //is in ms
			m_Set.m_StepSeqData_changed[1].setStepSeqTime(*m_Set.m_State->m_fStepSeqSpeed_STEPSEQ2); //is in ms
		}
		return;
	}

	if (0 == parameterID.compare("m_fStepSeqSpeed_STEPSEQ3")) {
		if (*m_Set.m_State->m_bStepSeqSynch_STEPSEQ3 == static_cast<int>(SWITCH::SWITCH_OFF)) {
			m_Set.m_StepSeqData[2].setStepSeqTime(*m_Set.m_State->m_fStepSeqSpeed_STEPSEQ3); //is in ms
			m_Set.m_StepSeqData_changed[2].setStepSeqTime(*m_Set.m_State->m_fStepSeqSpeed_STEPSEQ3); //is in ms
		}
		return;
	}

	if (0 == parameterID.compare("m_uStepSeqTimeBeats_STEPSEQ1")) {
		if (*m_Set.m_State->m_bStepSeqSynch_STEPSEQ1 == static_cast<int>(SWITCH::SWITCH_ON)) {
			float l_fIntervalTime = m_Set.getIntervalTimeFromDAWBeats(*m_Set.m_State->m_uStepSeqTimeBeats_STEPSEQ1);
			m_Set.m_StepSeqData[0].setStepSeqTime(l_fIntervalTime); //is in ms
			m_Set.m_StepSeqData_changed[0].setStepSeqTime(l_fIntervalTime); //is in ms
		}

		/*
		if (*m_Set.m_State->m_bStepSeqSynch_STEPSEQ1 == static_cast<int>(SWITCH::SWITCH_ON)) {
			if (m_Set.m_dPpqBpm == 0.f) return;
			float l_fIntervalTime = m_Set.getIntervalTimeFromDAWBeats(*m_Set.m_State->m_uStepSeqTimeBeats_STEPSEQ1);
			//if (l_fIntervalTime < 0.1f) l_fIntervalTime = 0.1f; // minimum //CHECK
			//if (l_fIntervalTime > 5000.0f) l_fIntervalTime = 5000.f; // maximum //CHECK
			m_Set.m_StepSeqData[0].setStepSeqTime(l_fIntervalTime); //is in ms
		}
		*/
		return;
	}

	if (0 == parameterID.compare("m_uStepSeqTimeBeats_STEPSEQ2")) {
		if (*m_Set.m_State->m_bStepSeqSynch_STEPSEQ2 == static_cast<int>(SWITCH::SWITCH_ON)) {
			float l_fIntervalTime = m_Set.getIntervalTimeFromDAWBeats(*m_Set.m_State->m_uStepSeqTimeBeats_STEPSEQ2);
			m_Set.m_StepSeqData[1].setStepSeqTime(l_fIntervalTime); //is in ms
			m_Set.m_StepSeqData_changed[1].setStepSeqTime(l_fIntervalTime); //is in ms
		}

		/*
		if (*m_Set.m_State->m_bStepSeqSynch_STEPSEQ2 == static_cast<int>(SWITCH::SWITCH_ON)) {
			if (m_Set.m_dPpqBpm == 0.f) return;
			float l_fIntervalTime = m_Set.getIntervalTimeFromDAWBeats(*m_Set.m_State->m_uStepSeqTimeBeats_STEPSEQ2);
			//if (l_fIntervalTime < 0.1f) l_fIntervalTime = 0.1f; // minimum //CHECK
			//if (l_fIntervalTime > 5000.0f) l_fIntervalTime = 5000.f; // maximum //CHECK
			m_Set.m_StepSeqData[1].setStepSeqTime(l_fIntervalTime); //is in ms
		}
		*/
		return;
	}

	if (0 == parameterID.compare("m_uStepSeqTimeBeats_STEPSEQ3")) {
		if (*m_Set.m_State->m_bStepSeqSynch_STEPSEQ3 == static_cast<int>(SWITCH::SWITCH_ON)) {
			float l_fIntervalTime = m_Set.getIntervalTimeFromDAWBeats(*m_Set.m_State->m_uStepSeqTimeBeats_STEPSEQ3);
			m_Set.m_StepSeqData[2].setStepSeqTime(l_fIntervalTime); //is in ms
			m_Set.m_StepSeqData_changed[2].setStepSeqTime(l_fIntervalTime); //is in ms
		}

		/*
		if (*m_Set.m_State->m_bStepSeqSynch_STEPSEQ3 == static_cast<int>(SWITCH::SWITCH_ON)) {
			if (m_Set.m_dPpqBpm == 0.f) return;
			float l_fIntervalTime = m_Set.getIntervalTimeFromDAWBeats(*m_Set.m_State->m_uStepSeqTimeBeats_STEPSEQ3);
			//if (l_fIntervalTime < 0.1f) l_fIntervalTime = 0.1f; // minimum //CHECK
			//if (l_fIntervalTime > 5000.0f) l_fIntervalTime = 5000.f; // maximum //CHECK
			m_Set.m_StepSeqData[2].setStepSeqTime(l_fIntervalTime); //is in ms			
		}
		*/
		return;
	}

	//custom modulators
	if (0 == parameterID.compare("m_fCustomModulator1")) {
		m_Poly.m_fCustomModulator1_smoothed.setTargetValue(*m_Set.m_State->m_fCustomModulator1 * 0.01f);
		return;
	}
	
	if (0 == parameterID.compare("m_fCustomModulator2")) {
		m_Poly.m_fCustomModulator2_smoothed.setTargetValue(*m_Set.m_State->m_fCustomModulator2 * 0.01f);
		return;
	}
	
	if (0 == parameterID.compare("m_fCustomModulator3")) {
		m_Poly.m_fCustomModulator3_smoothed.setTargetValue(*m_Set.m_State->m_fCustomModulator3 * 0.01f);
		return;
	}
	
	if (0 == parameterID.compare("m_fCustomModulator4")) {
		m_Poly.m_fCustomModulator4_smoothed.setTargetValue(*m_Set.m_State->m_fCustomModulator4 * 0.01f);
		return;
	}

	return;
}

unsigned int str2int(const char* str, int h = 0)
{
	return !str[h] ? 5381 : (str2int(str, h + 1) * 33) ^ str[h];
}
