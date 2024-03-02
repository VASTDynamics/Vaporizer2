/*
VAST Dynamics Audio Software (TM)
*/

#include "../Engine/VASTEngineHeader.h"
#include "VASTAudioProcessor.h"
#include "VASTAudioProcessorEditor.h"
#include "../Engine/VASTSampler.h"
#include "VASTOscillatorComponent.h"
#include "../Engine/VASTXperience.h"
//#include "Utils/VASTSynthfunctions.h"
#include <math.h>       /* modf */
#include <iostream>
#include <fstream>
#include <string>
#include <time.h>       /* time */
#include "assert.h"
#include <thread>
#include <signal.h> 
#include "VASTUtils/VASTLookAndFeel.h"
#include "VASTUtils/VASTLookAndFeelThemeDefault.h"
#include "VASTUtils/VASTLookAndFeelThemeIce.h"
#include "VASTUtils/VASTLookAndFeelThemeTech.h"
#include "VASTUtils/VASTLookAndFeelThemeDark.h"
#include "../Engine/Utils/VASTFFTSingletonFactory.h"
#include "VASTPreset/VASTPresetData.h"

//#include <vld.h> //visual leak detector /* uncomment for leak detection */

using namespace std;
using namespace BinaryData;

#define MAX_PARAMS 512
#define S_KEY_UITEXTS "sjkdfhskjdfhkjsdfkhsdfkjs4" //this is not the license encryption key

thread_local bool VASTAudioProcessor::m_threadLocalIsAudioThread = false;

//==============================================================================
VASTAudioProcessor::VASTAudioProcessor() :
			AudioProcessor(BusesProperties().withInput("Input", juce::AudioChannelSet::stereo(), true).withOutput("Output", juce::AudioChannelSet::stereo(), true)), // default 2 inputs, 2outputs, enabled
            m_parameterState(*this, &m_undoManager),
            m_pVASTXperience(this)
{
                
//CRT memory leak detection	
/*
#if defined(_DEBUG) && defined JUCE_WINDOWS
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>	
	#define DEBUG_NEW new(_NORMAL_BLOCK , __FILE__, __LINE__)
	#define new DEBUG_NEW
	int tmpFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	tmpFlag |= _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF;  // Check heap alloc and dump mem leaks at exit	 
	//_CrtSetBreakAlloc(858105);
	//_crtBreakAlloc = 693499; // Set to break on allocation number in case you get a leak without a line number
#endif
*/

//set logger if needed
#ifdef VASTLOG
	juce::Logger::setCurrentLogger(this);
#endif
	VDBG("Start AudioProcesor constructor.");
	m_initCompleted.store(false);
    m_bAudioThreadCurrentlyRunning.store(false);
    m_wasBypassed.store(false);

	initLookAndFeels();

	//crash handler
    SystemStats::CrashHandlerFunction crashHandlerFunction = (SystemStats::CrashHandlerFunction)&VASTAudioProcessor::crashHandler;
	SystemStats::setApplicationCrashHandler(crashHandlerFunction);
    
	m_pVASTXperience.m_nSampleRate = 44100; //at this stage getSampleRate() does not work yet

	m_pVASTXperience.initializeEngine();

	//prepare ValueTree - All parameters must be created before giving this manager a ValueTree state!
	ValueTree stateTree = ValueTree("VASTvaporizer2"); //changed Identifier(JucePlugin_Name)
	m_parameterState.state = stateTree;

	m_iNowLearningMidiParameterVariableName = "";
	mIntPpq = 0;
	m_sLicenseInformation.m_bExpired = false;
	m_fTrialSeconds = 0.0;
    m_bShallComponentValueUpdate = false;

	mUIAlert.store(false);
    mUIUpdateFlag.store(false);
    mUIInitFlag.store(false);
	mUIInitFlagAfterPrestLoad.store(false);

	bIsInErrorState.store(false);
	iErrorState.store(vastErrorState::noError);
	m_iNumPassTreeThreads = 0;

	initSettings();

	readLicense();

	m_presetData.reloadPresetArray(false); //has to be done after load F01
 	
	m_parameterState.undoManager->clearUndoHistory();
	m_parameterState.undoManager->beginNewTransaction();

	m_initCompleted.store(true);

	m_presetData.exchangeCurPatchData(*m_presetData.getPreset(0));
	initializeToDefaults();
	VDBG("End AudioProcesor constructor.");
}

VASTAudioProcessor::~VASTAudioProcessor()
{
	AudioProcessorEditor* editor = getActiveEditor();
	if (editor)
	{
		MessageManager::getInstance()->callFunctionOnMessageThread(
			deleteComponent, editor);
	}

#ifdef VASTLOG
	Logger::setCurrentLogger(nullptr);
#endif

	m_bAudioThreadStarted.store(false);
/*
#if defined(_DEBUG) && defined JUCE_WINDOWS
	_CrtDumpMemoryLeaks();
#endif
*/
}

void* VASTAudioProcessor::deleteComponent(void* userData)
{
	Component* comp = static_cast<Component*>(userData);
	Component* top = comp->getTopLevelComponent();
	delete top;
	return 0;
}

//==============================================================================
const String VASTAudioProcessor::getName() const
{
	//check system architecture
	return JucePlugin_Name;
}

bool VASTAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
	//can take any layout, the main bus needs to be the same on the input and output
	int inSize = layouts.getMainInputChannelSet().size();
	int outSize = layouts.getMainOutputChannelSet().size();

	VDBG("layouts.getMainInputChannelSet().size():" << layouts.getMainInputChannelSet().size());
	VDBG("layouts.getMainOutputChannelSet().size():" << layouts.getMainOutputChannelSet().size());
	VDBG("layouts.getMainInputChannelSet().isDiscreteLayout():" << (layouts.getMainInputChannelSet().isDiscreteLayout() ? "X" : ""));
	VDBG("layouts.getMainOutputChannelSet().isDiscreteLayout():" << (layouts.getMainOutputChannelSet().isDiscreteLayout() ? "X" : ""));
	VDBG("layouts.getMainInputChannelSet().getSpeakerArrangementAsString():" << (layouts.getMainInputChannelSet().getSpeakerArrangementAsString()));
	VDBG("layouts.getMainOutputChannelSet().getSpeakerArrangementAsString():" << (layouts.getMainOutputChannelSet().getSpeakerArrangementAsString()));
	
	return (
		((layouts.getMainInputChannelSet() == juce::AudioChannelSet::stereo()) && (layouts.getMainOutputChannelSet() == juce::AudioChannelSet::stereo())) || //stereo layouts
		((layouts.getMainInputChannelSet() == juce::AudioChannelSet::disabled()) && (layouts.getMainOutputChannelSet() == juce::AudioChannelSet::stereo())) ||
		((layouts.getMainInputChannelSet() == juce::AudioChannelSet::mono()) && (layouts.getMainOutputChannelSet() == juce::AudioChannelSet::mono())) || //mono layouts
		((layouts.getMainInputChannelSet() == juce::AudioChannelSet::disabled()) && (layouts.getMainOutputChannelSet() == juce::AudioChannelSet::mono())) ||
		((layouts.getMainInputChannelSet() == juce::AudioChannelSet::disabled()) && (layouts.getMainOutputChannelSet() == juce::AudioChannelSet::disabled())) //all disabled is OK but not very useful
		);	
}

void VASTAudioProcessor::clearErrorState() {
	bIsInErrorState.store(false);
	iErrorState.store(vastErrorState::noError);
}

void VASTAudioProcessor::setErrorState(vastErrorState state) {
	bIsInErrorState.store(true);
	iErrorState.store(state);
}

VASTAudioProcessor::vastErrorState VASTAudioProcessor::getErrorState() const {
    return iErrorState.load(); }

bool VASTAudioProcessor::wantsUIAlert() const {
    return mUIAlert.load(); }

void VASTAudioProcessor::clearUIAlertFlag() {
    mUIAlert.store(false); }

void VASTAudioProcessor::requestUIAlert() {
    mUIAlert.store(true); }

void VASTAudioProcessor::requestUIPresetUpdate() {
	mUIUpdateFlag.store(true);
	mUIPresetUpdate.store(true);
}

bool VASTAudioProcessor::needsUIPresetUpdate() const {
    return mUIPresetUpdate.load();
}

void VASTAudioProcessor::clearUIPresetFlag() { 
    mUIPresetUpdate.store(false);
}

void VASTAudioProcessor::requestUIPresetReloadUpdate() {
	mUIUpdateFlag.store(true);
	mUIPresetReloadUpdate.store(true);
}

bool VASTAudioProcessor::needsUIPresetReloadUpdate() const {
    return mUIPresetReloadUpdate.load(); }

void VASTAudioProcessor::clearUIPresetReloadFlag() {
    mUIPresetReloadUpdate.store(false); }

bool VASTAudioProcessor::needsUIInit() const {
    return mUIInitFlag.load(); }

bool VASTAudioProcessor::needsUIInitAfterPresetLoad() const {
	return mUIInitFlagAfterPrestLoad.load();
}

void VASTAudioProcessor::clearUIInitFlag() {
    mUIInitFlag.store(false); }

void VASTAudioProcessor::clearUIInitFlagAfterPresetLoad() {
	mUIInitFlagAfterPrestLoad.store(false);
}

void VASTAudioProcessor::requestUIInitAfterPrestLoad() {
	mUIInitFlagAfterPrestLoad.store(true);
}

void VASTAudioProcessor::requestUIInit() {
    mUIInitFlag.store(true);
    requestUIUpdate(true, true, true); }

void VASTAudioProcessor::resetCurrentEditorInitialized()
{
	mCurrentEditorInitialized.store(false);
}

void VASTAudioProcessor::setCurrentEditorInitialized()
{
	mCurrentEditorInitialized.store(true);
}

bool VASTAudioProcessor::isCurrentEditorInitialized()
{
	return mCurrentEditorInitialized.load();
}

bool VASTAudioProcessor::needsUIUpdate() const {
    return mUIUpdateFlag.load(); }

bool VASTAudioProcessor::needsUIUpdate_tabs() const {
    return mUIUpdateFlag_tabs.load(); }

bool VASTAudioProcessor::needsUIUpdate_matrix() const {
    return mUIUpdateFlag_matrix.load(); }

bool VASTAudioProcessor::needsUIUpdate_sliders() const {
    return mUIUpdateFlag_sliders.load(); }

int VASTAudioProcessor::needsUIUpdate_slider1dest() const {
    return mUIUpdateFlag_slider1dest.load(); }

int VASTAudioProcessor::needsUIUpdate_slider2dest() const {
    return mUIUpdateFlag_slider2dest.load(); }

void VASTAudioProcessor::clearUIUpdateFlag() {
	mUIUpdateFlag.store(false);
	mUIUpdateFlag_tabs.store(false);
	mUIUpdateFlag_matrix.store(false);
	mUIUpdateFlag_sliders.store(false);
	mUIUpdateFlag_slider1dest.store(-1);
	mUIUpdateFlag_slider2dest.store(-1);
}

void VASTAudioProcessor::requestUIUpdate(bool tabs, bool matrix, bool sliders, int slider1dest, int slider2dest) {
	mUIUpdateFlag.store(true);
	mUIUpdateFlag_tabs.store(tabs);
	mUIUpdateFlag_matrix.store(matrix);
	mUIUpdateFlag_sliders.store(sliders);
	mUIUpdateFlag_slider1dest.store(slider1dest);
	mUIUpdateFlag_slider2dest.store(slider2dest);
}

void VASTAudioProcessor::requestUILoadAlert() { 
    mUIAlert.store(true); 
}

#ifdef VASTLOG
void* VASTAudioProcessor::sendlogMessage(void* userData)
{
	VASTAudioProcessor* processor = static_cast<VASTAudioProcessor*>(userData);
	std::chrono::high_resolution_clock::time_point l_now  = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(l_now - processor->m_processorStartTime).count();
	processor->logCodeDocument.insertText(0, String(duration) + ": " + processor->m_last_message + "\n");
	return 0;
}

void VASTAudioProcessor::logMessage(const String& message)
{
	m_last_message = message;
	MessageManager::getInstance()->callFunctionOnMessageThread(
		sendlogMessage, this);
}
#endif 

bool VASTAudioProcessor::isAudioThread()
{
	return VASTAudioProcessor::m_threadLocalIsAudioThread;
}

bool VASTAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
	return true;
#else
	return false;
#endif
}

bool VASTAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
	return true;
#else
	return false;
#endif
}

bool VASTAudioProcessor::silenceInProducesSilenceOut() const
{
	return false;
}

double VASTAudioProcessor::getTailLengthSeconds() const
{
	return 0.0;
}

void VASTAudioProcessor::initializeToDefaults() {
	VDBG("Init to defaults");
	clearErrorState();
	m_pVASTXperience.m_Set.m_bBeforeV22CompatibilityMode = false;

	//int numparam = getNumParameters(); //deprecated
    int numparam = getParameters().size();
    
    m_pVASTXperience.setIsInitDefaults(true);
	for (int parameterIndex = 0; parameterIndex < numparam; parameterIndex++) {
		auto* param = getParameters()[parameterIndex];
		float defVal = param->getDefaultValue();
		param->setValueNotifyingHost(defVal);
	}
    m_pVASTXperience.setIsInitDefaults(false);
    m_pVASTXperience.parameterUpdatesAfterInit();

	//reset wts and wav here
	for (int bank = 0; bank < 4; bank++) {
		//m_pVASTXperience.m_Poly.m_OscBank[i]->init();
		m_pVASTXperience.m_Poly.m_OscBank[bank].recalcWavetable();
		std::shared_ptr<CVASTWaveTable> wavetable = std::make_shared<CVASTWaveTable>(m_pVASTXperience.m_Set);
		wavetable->addPosition();
		if (bank == 0) {
			//init bank A to saw
			std::vector<myFloat> *samples = wavetable->getNaiveTable(0);
			wavetable->getNaiveSamplesFromWave(*samples, WAVE::saw);
			wavetable->setWaveTableName(TRANS("Basic Saw"));
			wavetable->setNaiveTableFast(0, true, getWTmode());
		}
		if (m_bAudioThreadCurrentlyRunning)
			m_pVASTXperience.m_Poly.m_OscBank[bank].setWavetableSoftFade(wavetable);		
		else 
			m_pVASTXperience.m_Poly.m_OscBank[bank].setWavetable(wavetable);
	}
	
	//WAV File
	m_pVASTXperience.m_Poly.getSynthSound()->clearSamplerSound();
	m_pVASTXperience.m_Poly.getSynthSound()->clearSamplerSoundChanged();

	//mseg
	for (int i = 0; i < 5; i++) {
		m_pVASTXperience.m_Set.m_MSEGData[i].initToADSR(i);
		m_pVASTXperience.m_Set.m_MSEGData_changed[i].initToADSR(i);
		//force updates - required otherwise not initialized
		switch (i) {
		case 0:
			m_pVASTXperience.m_Set.m_MSEGData[i].setSynch(*m_pVASTXperience.m_Set.m_State->m_bMSEGSynch_MSEG1);
			m_pVASTXperience.m_Set.m_MSEGData_changed[i].setSynch(*m_pVASTXperience.m_Set.m_State->m_bMSEGSynch_MSEG1);
			m_pVASTXperience.m_Set.m_MSEGData[i].setTimeBeats(*m_pVASTXperience.m_Set.m_State->m_uMSEGTimeBeats_MSEG1);
			m_pVASTXperience.m_Set.m_MSEGData_changed[i].setTimeBeats(*m_pVASTXperience.m_Set.m_State->m_uMSEGTimeBeats_MSEG1);

			m_pVASTXperience.m_Set.m_MSEGData_changed[i].setSustainLevelSlider(*m_pVASTXperience.m_Set.m_State->m_fSustainLevel_MSEG1 / 100.f); // 0..1		
			m_pVASTXperience.m_Set.m_MSEGData[i].setSustainLevelSlider(*m_pVASTXperience.m_Set.m_State->m_fSustainLevel_MSEG1 / 100.f); // 0..1

			m_pVASTXperience.m_Set.m_MSEGData_changed[i].setAttackSteps(*m_pVASTXperience.m_Set.m_State->m_fAttackSteps_MSEG1, &m_pVASTXperience.m_Set);
			m_pVASTXperience.m_Set.m_MSEGData_changed[i].setDecaySteps(*m_pVASTXperience.m_Set.m_State->m_fDecaySteps_MSEG1, &m_pVASTXperience.m_Set);
			m_pVASTXperience.m_Set.m_MSEGData_changed[i].setReleaseSteps(*m_pVASTXperience.m_Set.m_State->m_fReleaseSteps_MSEG1, &m_pVASTXperience.m_Set);
			m_pVASTXperience.m_Set.m_MSEGData[i].setAttackSteps(*m_pVASTXperience.m_Set.m_State->m_fAttackSteps_MSEG1, &m_pVASTXperience.m_Set);
			m_pVASTXperience.m_Set.m_MSEGData[i].setDecaySteps(*m_pVASTXperience.m_Set.m_State->m_fDecaySteps_MSEG1, &m_pVASTXperience.m_Set);
			m_pVASTXperience.m_Set.m_MSEGData[i].setReleaseSteps(*m_pVASTXperience.m_Set.m_State->m_fReleaseSteps_MSEG1, &m_pVASTXperience.m_Set);

			m_pVASTXperience.m_Set.m_MSEGData_changed[i].setAttackTimeSlider(*m_pVASTXperience.m_Set.m_State->m_fAttackTime_MSEG1); // in ms
			m_pVASTXperience.m_Set.m_MSEGData_changed[i].setDecayTimeSlider(*m_pVASTXperience.m_Set.m_State->m_fDecayTime_MSEG1); // in ms
			m_pVASTXperience.m_Set.m_MSEGData_changed[i].setReleaseTimeSlider(*m_pVASTXperience.m_Set.m_State->m_fReleaseTime_MSEG1); // in ms
			m_pVASTXperience.m_Set.m_MSEGData[i].setAttackTimeSlider(*m_pVASTXperience.m_Set.m_State->m_fAttackTime_MSEG1); // in ms
			m_pVASTXperience.m_Set.m_MSEGData[i].setDecayTimeSlider(*m_pVASTXperience.m_Set.m_State->m_fDecayTime_MSEG1); // in ms
			m_pVASTXperience.m_Set.m_MSEGData[i].setReleaseTimeSlider(*m_pVASTXperience.m_Set.m_State->m_fReleaseTime_MSEG1); // in ms
			break;
		case 1:
			m_pVASTXperience.m_Set.m_MSEGData[i].setSynch(*m_pVASTXperience.m_Set.m_State->m_bMSEGSynch_MSEG2);
			m_pVASTXperience.m_Set.m_MSEGData_changed[i].setSynch(*m_pVASTXperience.m_Set.m_State->m_bMSEGSynch_MSEG2);
			m_pVASTXperience.m_Set.m_MSEGData[i].setTimeBeats(*m_pVASTXperience.m_Set.m_State->m_uMSEGTimeBeats_MSEG2);
			m_pVASTXperience.m_Set.m_MSEGData_changed[i].setTimeBeats(*m_pVASTXperience.m_Set.m_State->m_uMSEGTimeBeats_MSEG2);

			m_pVASTXperience.m_Set.m_MSEGData_changed[i].setSustainLevelSlider(*m_pVASTXperience.m_Set.m_State->m_fSustainLevel_MSEG2 / 100.f); // 0..1		
			m_pVASTXperience.m_Set.m_MSEGData[i].setSustainLevelSlider(*m_pVASTXperience.m_Set.m_State->m_fSustainLevel_MSEG2 / 100.f); // 0..1

			m_pVASTXperience.m_Set.m_MSEGData_changed[i].setAttackSteps(*m_pVASTXperience.m_Set.m_State->m_fAttackSteps_MSEG2, &m_pVASTXperience.m_Set);
			m_pVASTXperience.m_Set.m_MSEGData_changed[i].setDecaySteps(*m_pVASTXperience.m_Set.m_State->m_fDecaySteps_MSEG2, &m_pVASTXperience.m_Set);
			m_pVASTXperience.m_Set.m_MSEGData_changed[i].setReleaseSteps(*m_pVASTXperience.m_Set.m_State->m_fReleaseSteps_MSEG2, &m_pVASTXperience.m_Set);
			m_pVASTXperience.m_Set.m_MSEGData[i].setAttackSteps(*m_pVASTXperience.m_Set.m_State->m_fAttackSteps_MSEG2, &m_pVASTXperience.m_Set);
			m_pVASTXperience.m_Set.m_MSEGData[i].setDecaySteps(*m_pVASTXperience.m_Set.m_State->m_fDecaySteps_MSEG2, &m_pVASTXperience.m_Set);
			m_pVASTXperience.m_Set.m_MSEGData[i].setReleaseSteps(*m_pVASTXperience.m_Set.m_State->m_fReleaseSteps_MSEG2, &m_pVASTXperience.m_Set);

			m_pVASTXperience.m_Set.m_MSEGData_changed[i].setAttackTimeSlider(*m_pVASTXperience.m_Set.m_State->m_fAttackTime_MSEG2); // in ms
			m_pVASTXperience.m_Set.m_MSEGData_changed[i].setDecayTimeSlider(*m_pVASTXperience.m_Set.m_State->m_fDecayTime_MSEG2); // in ms
			m_pVASTXperience.m_Set.m_MSEGData_changed[i].setReleaseTimeSlider(*m_pVASTXperience.m_Set.m_State->m_fReleaseTime_MSEG2); // in ms
			m_pVASTXperience.m_Set.m_MSEGData[i].setAttackTimeSlider(*m_pVASTXperience.m_Set.m_State->m_fAttackTime_MSEG2); // in ms
			m_pVASTXperience.m_Set.m_MSEGData[i].setDecayTimeSlider(*m_pVASTXperience.m_Set.m_State->m_fDecayTime_MSEG2); // in ms
			m_pVASTXperience.m_Set.m_MSEGData[i].setReleaseTimeSlider(*m_pVASTXperience.m_Set.m_State->m_fReleaseTime_MSEG2); // in ms
			break;
		case 2:
			m_pVASTXperience.m_Set.m_MSEGData[i].setSynch(*m_pVASTXperience.m_Set.m_State->m_bMSEGSynch_MSEG3);
			m_pVASTXperience.m_Set.m_MSEGData_changed[i].setSynch(*m_pVASTXperience.m_Set.m_State->m_bMSEGSynch_MSEG3);
			m_pVASTXperience.m_Set.m_MSEGData[i].setTimeBeats(*m_pVASTXperience.m_Set.m_State->m_uMSEGTimeBeats_MSEG3);
			m_pVASTXperience.m_Set.m_MSEGData_changed[i].setTimeBeats(*m_pVASTXperience.m_Set.m_State->m_uMSEGTimeBeats_MSEG3);

			m_pVASTXperience.m_Set.m_MSEGData_changed[i].setSustainLevelSlider(*m_pVASTXperience.m_Set.m_State->m_fSustainLevel_MSEG3 / 100.f); // 0..1		
			m_pVASTXperience.m_Set.m_MSEGData[i].setSustainLevelSlider(*m_pVASTXperience.m_Set.m_State->m_fSustainLevel_MSEG3 / 100.f); // 0..1

			m_pVASTXperience.m_Set.m_MSEGData_changed[i].setAttackSteps(*m_pVASTXperience.m_Set.m_State->m_fAttackSteps_MSEG3, &m_pVASTXperience.m_Set);
			m_pVASTXperience.m_Set.m_MSEGData_changed[i].setDecaySteps(*m_pVASTXperience.m_Set.m_State->m_fDecaySteps_MSEG3, &m_pVASTXperience.m_Set);
			m_pVASTXperience.m_Set.m_MSEGData_changed[i].setReleaseSteps(*m_pVASTXperience.m_Set.m_State->m_fReleaseSteps_MSEG3, &m_pVASTXperience.m_Set);
			m_pVASTXperience.m_Set.m_MSEGData[i].setAttackSteps(*m_pVASTXperience.m_Set.m_State->m_fAttackSteps_MSEG3, &m_pVASTXperience.m_Set);
			m_pVASTXperience.m_Set.m_MSEGData[i].setDecaySteps(*m_pVASTXperience.m_Set.m_State->m_fDecaySteps_MSEG3, &m_pVASTXperience.m_Set);
			m_pVASTXperience.m_Set.m_MSEGData[i].setReleaseSteps(*m_pVASTXperience.m_Set.m_State->m_fReleaseSteps_MSEG3, &m_pVASTXperience.m_Set);

			m_pVASTXperience.m_Set.m_MSEGData_changed[i].setAttackTimeSlider(*m_pVASTXperience.m_Set.m_State->m_fAttackTime_MSEG3); // in ms
			m_pVASTXperience.m_Set.m_MSEGData_changed[i].setDecayTimeSlider(*m_pVASTXperience.m_Set.m_State->m_fDecayTime_MSEG3); // in ms
			m_pVASTXperience.m_Set.m_MSEGData_changed[i].setReleaseTimeSlider(*m_pVASTXperience.m_Set.m_State->m_fReleaseTime_MSEG3); // in ms
			m_pVASTXperience.m_Set.m_MSEGData[i].setAttackTimeSlider(*m_pVASTXperience.m_Set.m_State->m_fAttackTime_MSEG3); // in ms
			m_pVASTXperience.m_Set.m_MSEGData[i].setDecayTimeSlider(*m_pVASTXperience.m_Set.m_State->m_fDecayTime_MSEG3); // in ms
			m_pVASTXperience.m_Set.m_MSEGData[i].setReleaseTimeSlider(*m_pVASTXperience.m_Set.m_State->m_fReleaseTime_MSEG3); // in ms
			break;
		case 3:
			m_pVASTXperience.m_Set.m_MSEGData[i].setSynch(*m_pVASTXperience.m_Set.m_State->m_bMSEGSynch_MSEG4);
			m_pVASTXperience.m_Set.m_MSEGData_changed[i].setSynch(*m_pVASTXperience.m_Set.m_State->m_bMSEGSynch_MSEG4);
			m_pVASTXperience.m_Set.m_MSEGData[i].setTimeBeats(*m_pVASTXperience.m_Set.m_State->m_uMSEGTimeBeats_MSEG4);
			m_pVASTXperience.m_Set.m_MSEGData_changed[i].setTimeBeats(*m_pVASTXperience.m_Set.m_State->m_uMSEGTimeBeats_MSEG4);

			m_pVASTXperience.m_Set.m_MSEGData_changed[i].setSustainLevelSlider(*m_pVASTXperience.m_Set.m_State->m_fSustainLevel_MSEG4 / 100.f); // 0..1		
			m_pVASTXperience.m_Set.m_MSEGData[i].setSustainLevelSlider(*m_pVASTXperience.m_Set.m_State->m_fSustainLevel_MSEG4 / 100.f); // 0..1

			m_pVASTXperience.m_Set.m_MSEGData_changed[i].setAttackSteps(*m_pVASTXperience.m_Set.m_State->m_fAttackSteps_MSEG4, &m_pVASTXperience.m_Set);
			m_pVASTXperience.m_Set.m_MSEGData_changed[i].setDecaySteps(*m_pVASTXperience.m_Set.m_State->m_fDecaySteps_MSEG4, &m_pVASTXperience.m_Set);
			m_pVASTXperience.m_Set.m_MSEGData_changed[i].setReleaseSteps(*m_pVASTXperience.m_Set.m_State->m_fReleaseSteps_MSEG4, &m_pVASTXperience.m_Set);
			m_pVASTXperience.m_Set.m_MSEGData[i].setAttackSteps(*m_pVASTXperience.m_Set.m_State->m_fAttackSteps_MSEG4, &m_pVASTXperience.m_Set);
			m_pVASTXperience.m_Set.m_MSEGData[i].setDecaySteps(*m_pVASTXperience.m_Set.m_State->m_fDecaySteps_MSEG4, &m_pVASTXperience.m_Set);
			m_pVASTXperience.m_Set.m_MSEGData[i].setReleaseSteps(*m_pVASTXperience.m_Set.m_State->m_fReleaseSteps_MSEG4, &m_pVASTXperience.m_Set);

			m_pVASTXperience.m_Set.m_MSEGData_changed[i].setAttackTimeSlider(*m_pVASTXperience.m_Set.m_State->m_fAttackTime_MSEG4); // in ms
			m_pVASTXperience.m_Set.m_MSEGData_changed[i].setDecayTimeSlider(*m_pVASTXperience.m_Set.m_State->m_fDecayTime_MSEG4); // in ms
			m_pVASTXperience.m_Set.m_MSEGData_changed[i].setReleaseTimeSlider(*m_pVASTXperience.m_Set.m_State->m_fReleaseTime_MSEG4); // in ms
			m_pVASTXperience.m_Set.m_MSEGData[i].setAttackTimeSlider(*m_pVASTXperience.m_Set.m_State->m_fAttackTime_MSEG4); // in ms
			m_pVASTXperience.m_Set.m_MSEGData[i].setDecayTimeSlider(*m_pVASTXperience.m_Set.m_State->m_fDecayTime_MSEG4); // in ms
			m_pVASTXperience.m_Set.m_MSEGData[i].setReleaseTimeSlider(*m_pVASTXperience.m_Set.m_State->m_fReleaseTime_MSEG4); // in ms
			break;
		case 4:
			m_pVASTXperience.m_Set.m_MSEGData[i].setSynch(*m_pVASTXperience.m_Set.m_State->m_bMSEGSynch_MSEG5);
			m_pVASTXperience.m_Set.m_MSEGData_changed[i].setSynch(*m_pVASTXperience.m_Set.m_State->m_bMSEGSynch_MSEG5);
			m_pVASTXperience.m_Set.m_MSEGData[i].setTimeBeats(*m_pVASTXperience.m_Set.m_State->m_uMSEGTimeBeats_MSEG5);
			m_pVASTXperience.m_Set.m_MSEGData_changed[i].setTimeBeats(*m_pVASTXperience.m_Set.m_State->m_uMSEGTimeBeats_MSEG5);

			m_pVASTXperience.m_Set.m_MSEGData_changed[i].setSustainLevelSlider(*m_pVASTXperience.m_Set.m_State->m_fSustainLevel_MSEG5 / 100.f); // 0..1		
			m_pVASTXperience.m_Set.m_MSEGData[i].setSustainLevelSlider(*m_pVASTXperience.m_Set.m_State->m_fSustainLevel_MSEG5 / 100.f); // 0..1

			m_pVASTXperience.m_Set.m_MSEGData_changed[i].setAttackSteps(*m_pVASTXperience.m_Set.m_State->m_fAttackSteps_MSEG5, &m_pVASTXperience.m_Set);
			m_pVASTXperience.m_Set.m_MSEGData_changed[i].setDecaySteps(*m_pVASTXperience.m_Set.m_State->m_fDecaySteps_MSEG5, &m_pVASTXperience.m_Set);
			m_pVASTXperience.m_Set.m_MSEGData_changed[i].setReleaseSteps(*m_pVASTXperience.m_Set.m_State->m_fReleaseSteps_MSEG5, &m_pVASTXperience.m_Set);
			m_pVASTXperience.m_Set.m_MSEGData[i].setAttackSteps(*m_pVASTXperience.m_Set.m_State->m_fAttackSteps_MSEG5, &m_pVASTXperience.m_Set);
			m_pVASTXperience.m_Set.m_MSEGData[i].setDecaySteps(*m_pVASTXperience.m_Set.m_State->m_fDecaySteps_MSEG5, &m_pVASTXperience.m_Set);
			m_pVASTXperience.m_Set.m_MSEGData[i].setReleaseSteps(*m_pVASTXperience.m_Set.m_State->m_fReleaseSteps_MSEG5, &m_pVASTXperience.m_Set);

			m_pVASTXperience.m_Set.m_MSEGData_changed[i].setAttackTimeSlider(*m_pVASTXperience.m_Set.m_State->m_fAttackTime_MSEG5); // in ms
			m_pVASTXperience.m_Set.m_MSEGData_changed[i].setDecayTimeSlider(*m_pVASTXperience.m_Set.m_State->m_fDecayTime_MSEG5); // in ms
			m_pVASTXperience.m_Set.m_MSEGData_changed[i].setReleaseTimeSlider(*m_pVASTXperience.m_Set.m_State->m_fReleaseTime_MSEG5); // in ms
			m_pVASTXperience.m_Set.m_MSEGData[i].setAttackTimeSlider(*m_pVASTXperience.m_Set.m_State->m_fAttackTime_MSEG5); // in ms
			m_pVASTXperience.m_Set.m_MSEGData[i].setDecayTimeSlider(*m_pVASTXperience.m_Set.m_State->m_fDecayTime_MSEG5); // in ms
			m_pVASTXperience.m_Set.m_MSEGData[i].setReleaseTimeSlider(*m_pVASTXperience.m_Set.m_State->m_fReleaseTime_MSEG5); // in ms
			break;
		}
	}

	//step seqs
	for (int i = 0; i < 3; i++) {
		m_pVASTXperience.m_Set.m_StepSeqData[i].initStepSeq(i);
		m_pVASTXperience.m_Set.m_StepSeqData_changed[i].initStepSeq(i);
	}

	//arp
	for (int i = 0; i < 1; i++) {
		m_pVASTXperience.m_Set.m_ARPData.initDefaultPattern(0);
		m_pVASTXperience.m_Set.m_ARPData_changed.initDefaultPattern(0);
	}

	//fxbus
	m_pVASTXperience.m_fxBus1.initSequence();
	m_pVASTXperience.m_fxBus2.initSequence();
	m_pVASTXperience.m_fxBus3.initSequence();
    
	//m_parameterState.undoManager->clearUndoHistory(); //TODO: there is a sporadic segfault here!
	m_parameterState.undoManager->beginNewTransaction(); //start new transcation only here?
}

void VASTAudioProcessor::loadPresetFile(File presetvvpfile) {
	VASTPresetElement lPreset{};
	std::unique_ptr<juce::XmlDocument> xml(new XmlDocument(presetvvpfile));
	bool success = loadPatchXML(xml.get(), false, &lPreset, lPreset.presetarrayindex, lPreset); 
}

void VASTAudioProcessor::setCurrentProgram(int index)
{
	if (index >= getNumPrograms()) {
		vassert(false);
		return;
	}

	//if (index != 0) //initpatch shall always be used
		//if (m_presetData.getCurPatchData().presetarrayindex == index) return;

	if ((Time::getMillisecondCounter() - m_tSetChunkCalled) < 400) {  // see http://www.juce.com/forum/topic/problems-recalling-plugin-state-versus-current-preset
		return;
	}

	m_pVASTXperience.m_nSampleRate = getSampleRate();
    
    if (m_bAudioThreadStarted)
        registerLoadPreset(index);
    else
        loadPreset(index);  // index maps to array sequence
}

void VASTAudioProcessor::registerLoadPreset(int index) {
    m_presetToLoad = index;
}

const String VASTAudioProcessor::getProgramName(int index)
{
	if (index == CONST_USER_PATCH_INDEX)
		return m_presetData.getCurPatchData().presetname;
	else {
		String retStr = "n/a";
		if (m_presetData.getPreset(index) != nullptr) {
			if (m_presetData.getPreset(index)->category.equalsIgnoreCase(""))
				retStr = m_presetData.getPreset(index)->presetname;
			else 
				retStr = (m_presetData.getPreset(index)->category + " " + m_presetData.getPreset(index)->presetname);
			retStr.trim();
		}
		return retStr;
	}
}

void VASTAudioProcessor::changeProgramName(int index, const String& newName)
{
	//setCurrentProgram(CONST_USER_PATCH_INDEX);
	if (m_presetData.getPreset(index) == nullptr)
		return;
	m_presetData.getPreset(index)->presetname = newName;
	VASTPresetElement lCurPatch = m_presetData.getCurPatchData();
	lCurPatch.presetname = newName;
	m_presetData.exchangeCurPatchData(lCurPatch);
}

int VASTAudioProcessor::getNumPrograms()
{
	int number = 0;
	number += getNumFactoryPresets() + getNumUserPresets();
	return number;
}

int VASTAudioProcessor::getCurrentProgram()
{
	int l_prog = m_curPatchDataLoadRequestedIndex;
	return l_prog;
}

int VASTAudioProcessor::getCurrentPresetProgram() const
{
	int l_prog = m_curPatchDataLoadRequestedIndex;
	return l_prog;
}

//==============================================================================
void VASTAudioProcessor::prepareToPlay(double sampleRate, int expectedSamplesPerBlock)
{
	if ((sampleRate == 0) || (expectedSamplesPerBlock == 0)) return;

	VDBG("Audio processor Prepare to play called!");
	m_pVASTXperience.m_iFadeInSamples.store(m_pVASTXperience.m_iMaxFadeSamples);

	//bool b_wasLocked = m_pVASTXperience.getBlockProcessing();

	int waitstate = 0;
	if ((m_bAudioThreadStarted) && (!m_wasBypassed)) {
		while (m_pVASTXperience.getBlockProcessing()) {
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
			waitstate += 50;
			if (waitstate > 5000) {
				setErrorState(vastErrorState::errorState18_prepareToPlayFailed);
				return; //end after 5s waiting time
			}
		}
	}

	//if (!b_wasLocked) 
	if (!m_pVASTXperience.audioProcessLock()) {
		setErrorState(vastErrorState::errorState18_prepareToPlayFailed);
	}
	m_pVASTXperience.prepareForPlay(sampleRate, expectedSamplesPerBlock);
	//if (!b_wasLocked) 
	if (!m_pVASTXperience.audioProcessUnlock()) {
		setErrorState(vastErrorState::errorState18_prepareToPlayFailed);
	}

	// Use this method as the place to do any pre-playback
	// initialisation that you need..
}

void VASTAudioProcessor::releaseResources()
{
	// When playback stops, you can use this as an opportunity to free up any
	VDBG("VASTAudioProcessor:Releaseresources()");
	if (!m_bAudioThreadStarted)
		return;
	if (!m_pVASTXperience.audioProcessLock()) {
		vassertfalse;
		return; //some other task has already blocked it
	}
	bool done = false;
	int counter = 0;
	while (!done) {
		if ((counter<30) && (m_bAudioThreadCurrentlyRunning.load() && (!m_pVASTXperience.getBlockProcessingIsBlockedSuccessfully()))) {
			VDBG("VASTAudioProcessor:Releaseresources() - sleep");
			Thread::sleep(100);
			counter++;
			continue;
		}
		if (counter == 30) {
			vassertfalse;
			return; //dont unlock what is not locked
		}
        if (counter<30) {
			m_pVASTXperience.m_Poly.releaseResources();
			if (!m_pVASTXperience.audioProcessUnlock()) {
				vassertfalse;
			}
        }
		done = true;
	}
	//m_pVASTXperience.m_Set.m_RoutingBuffers.resize(0, false); //free memory! -> no, heap corruption!
	//Check this
}

void VASTAudioProcessor::processBlockBypassed(AudioBuffer<float>& buffer,
	MidiBuffer& midiMessages) {
	VASTAudioProcessor::m_threadLocalIsAudioThread = true;

	midiMessages.clear();
	if (!m_wasBypassed)
		m_pVASTXperience.m_Poly.stopAllNotes(false);
	m_wasBypassed = true;

	m_presetData.swapPresetArraysIfNeeded();

    if (m_presetToLoad >= 0)
        loadPreset(m_presetToLoad);
    
	for (int bank = 0; bank < 4; bank++)
		m_pVASTXperience.m_Poly.m_OscBank[bank].m_bWavetableSoftfadeStillRendered.store(false);
	m_pVASTXperience.beginSoftFade(); //CHECKTS
	m_pVASTXperience.endSoftFade(); //CHECKTS
    
    if (m_presetToLoad >= 0)
        loadPreset(m_presetToLoad);
	//VASTAudioProcessor::m_threadLocalIsAudioThread = false; //keep flag?
}

void VASTAudioProcessor::processBlock(AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
	VASTAudioProcessor::m_threadLocalIsAudioThread = true;
	if (isSuspended())
		return;
	m_bAudioThreadStarted.store(true);
	m_bAudioThreadCurrentlyRunning.store(true);

	if (m_wasBypassed) {
		prepareToPlay(getSampleRate(), getBlockSize()); //this takes long and does somthing with cubase
		m_wasBypassed = false;
	}

	m_presetData.swapPresetArraysIfNeeded();
    
    if (m_presetToLoad >= 0)
        loadPreset(m_presetToLoad);

	//if (buffer.getNumChannels() != 2) return;
    
	for (int i = getTotalNumInputChannels(); i < getTotalNumOutputChannels(); ++i)
		buffer.clear(i, 0, buffer.getNumSamples());
    
	m_midiKeyboardState.processNextMidiBuffer(midiMessages, 0, buffer.getNumSamples(), true);
    
	//do synth
     
    // send MIDI clock
    AudioPlayHead::PositionInfo positionDefault{};
    Optional<AudioPlayHead::PositionInfo> positionInfo = *getPlayHead()->getPosition();
     
	m_pVASTXperience.processAudioBuffer(buffer, midiMessages, getTotalNumOutputChannels(),
        positionInfo.orFallback(positionDefault).getIsPlaying(),
        positionInfo.orFallback(positionDefault).getPpqPosition().orFallback(0.0),
        positionInfo.orFallback(positionDefault).getIsLooping(),
        positionInfo.orFallback(positionDefault).getPpqPositionOfLastBarStart().orFallback(0.0),
        positionInfo.orFallback(positionDefault).getBpm().orFallback(0.0));
    
	//VU meter
	m_meterSource.measureBlock(buffer);

	//OpenGL Oscilloscope
	m_pVASTXperience.oscilloscopeRingBuffer->writeSamples(buffer, 0, buffer.getNumSamples());

	//check for click 	
	
	//if (buffer.getMagnitude(0, buffer.getNumSamples()) > 10.f) {
	//	dumpBuffers();
	//}

	//clear midi after processing (-->midi keyboard & arp generate some that shall not be passed through)
	midiMessages.clear();

	//check license
#ifdef VASTCOMMERCIAL
	if (isLicensed() == false) {
		if ((m_sLicenseInformation.m_bExpired) || (m_fTrialSeconds > MAX_TRIAL_SECONDS)) {
			m_sLicenseInformation.m_bExpired = true;
			buffer.clear();
		}
		else {
			m_fTrialSeconds += buffer.getNumSamples() / getSampleRate();
		}
	}
#endif
    
	m_bAudioThreadCurrentlyRunning.store(false); //CHECK
	//VASTAudioProcessor::m_threadLocalIsAudioThread = false; //keep flag?
}

//==============================================================================
bool VASTAudioProcessor::hasEditor() const
{
	return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* VASTAudioProcessor::createEditor()
{
	VDBG("Create VASTAudioProcessorEditor");
	readSettingsFromFile(); //to sync changed plugin sizes
	VASTAudioProcessorEditor* editor = new VASTAudioProcessorEditor(*this);

	editor->setActiveLookAndFeel(m_activeLookAndFeel);
	return editor;
}

//from plugin model to tree
void VASTAudioProcessor::addChunkTreeState(ValueTree* treeState) { //for save
																   //OscBanks
	VDBG("Appending chunk data for stateInformation");
	String childName = "chunkData";
	ValueTree l_chunkTree = ValueTree(Identifier(childName));
	ValueTree l_tree{};
	treeState->appendChild(l_chunkTree, nullptr);

	for (int i = 0; i < 4; i++) {
		childName = "oscBank" + String(i);
		l_tree = ValueTree(Identifier(childName));
		std::shared_ptr<CVASTWaveTable> wavetable = m_pVASTXperience.m_Poly.m_OscBank[i].getWavetablePointer();
		if (wavetable->bufferedValueTree.hasProperty("waveTableName")) {
			l_tree.copyPropertiesAndChildrenFrom(wavetable->bufferedValueTree, nullptr);
		}
		else 
			wavetable->getValueTreeState(&l_tree, nullptr /*&m_undoManager*/);		
		l_chunkTree.appendChild(l_tree, nullptr);
		vassert(l_tree.isValid());
	}

	//WAV File
	if (m_pVASTXperience.m_Poly.getSamplerSound() != nullptr) {
		VASTSamplerSound* sound = (VASTSamplerSound*)m_pVASTXperience.m_Poly.getSamplerSound();
		childName = "sampleData";
		l_tree = ValueTree(Identifier(childName));
		l_chunkTree.appendChild(l_tree, nullptr);
		vassert(l_tree.isValid());
		sound->getValueTreeState(&l_tree, nullptr /*&m_undoManager*/);
	}

	//msegs
	for (int i = 0; i < 5; i++) {
		childName = "msegData" + String(i);
		l_tree = ValueTree(Identifier(childName));
		l_chunkTree.appendChild(l_tree, nullptr);
		vassert(l_tree.isValid());
		if (l_tree.isValid())
			m_pVASTXperience.m_Set.m_MSEGData[i].getValueTreeState(&l_tree, nullptr /*&m_undoManager*/, true); //is mseg
		else 
			setErrorState(vastErrorState::errorState2_invalidMSEGData);
	}

	//step seqs
	for (int i = 0; i < 3; i++) {
		childName = "stepSeqData" + String(i);
		l_tree = ValueTree(Identifier(childName));
		l_chunkTree.appendChild(l_tree, nullptr);
		vassert(l_tree.isValid());
		if (l_tree.isValid())
			m_pVASTXperience.m_Set.m_StepSeqData[i].getValueTreeState(&l_tree, nullptr /*&m_undoManager*/, false); //is stepseq
		else
			setErrorState(vastErrorState::errorState3_invalidStepSeqData);
	}	

	//arp
	for (int i = 0; i < 1; i++) { //only one arp
		childName = "arpData" + String(i);
		l_tree = ValueTree(Identifier(childName));
		l_chunkTree.appendChild(l_tree, nullptr);
		vassert(l_tree.isValid());
		if (l_tree.isValid())
			m_pVASTXperience.m_Set.m_ARPData.getValueTreeState(&l_tree, nullptr /*&m_undoManager*/);
		else
			setErrorState(vastErrorState::errorState4_invalidArpData);
	}

	//fx bus
	{
		childName = "fxBusData" + String(1);
		l_tree = ValueTree(Identifier(childName));
		l_chunkTree.appendChild(l_tree, nullptr);
		vassert(l_tree.isValid());
		if (l_tree.isValid())
			m_pVASTXperience.m_fxBus1.getValueTreeState(&l_tree, nullptr /*&m_undoManager*/);
		else
			setErrorState(vastErrorState::errorState24_invalidFXBusData);
	}
	{
		childName = "fxBusData" + String(2);
		l_tree = ValueTree(Identifier(childName));
		l_chunkTree.appendChild(l_tree, nullptr);
		vassert(l_tree.isValid());
		if (l_tree.isValid())
			m_pVASTXperience.m_fxBus2.getValueTreeState(&l_tree, nullptr /*&m_undoManager*/);
		else
			setErrorState(vastErrorState::errorState24_invalidFXBusData);
	}
	{
		childName = "fxBusData" + String(3);
		l_tree = ValueTree(Identifier(childName));
		l_chunkTree.appendChild(l_tree, nullptr);
		vassert(l_tree.isValid());
		if (l_tree.isValid())
			m_pVASTXperience.m_fxBus3.getValueTreeState(&l_tree, nullptr /*&m_undoManager*/);
		else
			setErrorState(vastErrorState::errorState24_invalidFXBusData);
	}
}

bool VASTAudioProcessor::doUndo() {	
	bool success = m_parameterState.undoManager->undo();	
	VASTAudioProcessorEditor* editor = (VASTAudioProcessorEditor*)getActiveEditor();
	if (editor != nullptr) {
        if (editor->vaporizerComponent.get() != nullptr)
            editor->vaporizerComponent->updateAll();
	}

	return success;
}

//==============================================================================
//Save

void VASTAudioProcessor::getCurrentProgramStateInformation(juce::MemoryBlock &destData) { 
	// You should use this method to store your parameters in the memory block.
	// You could do that either as raw data, or use the XML or ValueTree classes
	// as intermediaries to make it easy to save and load complex data.

	//Save UserParams/Data to file
	//Make sure public data is current (through any param conversions)

	// save exactly one preset

	getStateInformation(destData);
}

void VASTAudioProcessor::getStateInformation(MemoryBlock& destData)
{
	// You should use this method to store your parameters in the memory block.
	// You could do that either as raw data, or use the XML or ValueTree classes
	// as intermediaries to make it easy to save and load complex data.

	//Save UserParams/Data to file
	//Make sure public data is current (through any param conversions)

	// save many presets / bank 

	VDBG("VASTAudioProcessor::getStateInformation called from Thread " << (Thread::getCurrentThread()==nullptr ? "Main UI Thread" : Thread::getCurrentThread()->getThreadName()));

	//const ScopedLock sl(getCallbackLock()); //do not lock here, this causes audio process locks and CPU spikes

	XmlElement xml = createPatchXML(true); //VASTVaporizerParamsV2.10000 behavior
	copyXmlToBinary(xml, destData);
}

//==============================================================================
//Load
void VASTAudioProcessor::setStateInformation(const void* data, int sizeInBytes) {

	// You should use this method to restore your parameters from this memory block,
	// whose contents will have been created by the getStateInformation() call.

	// load many presets - bank

	m_tSetChunkCalled = Time::getMillisecondCounter();
	
	//https://docs.juce.com/master/tutorial_audio_processor_value_tree_state.html
	std::unique_ptr<XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

	String version = xmlState->getStringAttribute("PatchVersion");
	VASTPresetElement lElem;
	if (version.equalsIgnoreCase("VASTVaporizerParamsV2.00000")) //old behavior: internal representation
		xmlParseToPatch(xmlState.get(), false, &m_presetData.getCurPatchData(), m_presetData.getCurPatchData().presetarrayindex, false, true, lElem); //state stores internal representation?? really, why? should be compatible as well
	else if (version.equalsIgnoreCase("VASTVaporizerParamsV2.10000")) //new behavior: external representation
		xmlParseToPatch(xmlState.get(), false, &m_presetData.getCurPatchData(), m_presetData.getCurPatchData().presetarrayindex, true, true, lElem);
    else if (version.equalsIgnoreCase("VASTVaporizerParamsV2.20000")) //new behavior: changed skew modulation
        xmlParseToPatch(xmlState.get(), false, &m_presetData.getCurPatchData(), m_presetData.getCurPatchData().presetarrayindex, true, true, lElem);
}

void VASTAudioProcessor::setCurrentProgramStateInformation(const void *data, int sizeInBytes) {
	// You should use this method to restore your parameters from this memory block,
	// whose contents will have been created by the getStateInformation() call.

	// load exactly one preset

	setStateInformation(data, sizeInBytes);
}

void VASTAudioProcessor::savePatchXML(File *selectedFile) {
	std::unique_ptr<XmlElement> root (new XmlElement(createPatchXML(true))); //use external representation for preset files
	//String fileText = root.createDocument("", false, true); //deprecated
	XmlElement::TextFormat xmlformat;
	String fileText = root->toString(xmlformat);
	selectedFile->deleteFile();
	FileOutputStream out(*selectedFile);
	if (out.failedToOpen()) {
		setErrorState(vastErrorState::errorState5_couldNotOpenFileForWritingPreset);
		assert(true);
	}
	out.writeText(fileText, false, false, "\n"); //slash n new?
	out.flush();
	root->deleteAllChildElements();
}

int VASTAudioProcessor::getNumFactoryPresets() const {
	return 1;
}

int VASTAudioProcessor::getNumUserPresets() const {
	return m_presetData.getNumPresets();
}

bool VASTAudioProcessor::loadUserPatchMetaData(File file, VASTPresetElement& lPreset) {
    std::unique_ptr<juce::XmlDocument> xml(new XmlDocument(file));
	const VASTPresetElement lElem = lPreset;
	bool success = loadPatchXML(xml.get(), true, &lElem, lPreset.presetarrayindex, lPreset); //names only
	return success;
}

//build presets here
//array of xml, xmlsize ...
//define factory bank
void VASTAudioProcessor::loadPreset(int index) {
	VDBG("Load preset " << index);
	//after this it has to be quick
	//CriticalSection::ScopedLockType lock(m_pVASTXperience.m_Set.audioProcessBlockRunning);
	//SpinLock::ScopedLockType lock(m_pVASTXperience.m_Set.audioProcessBlockRunning);
	//const ScopedLock sl(getCallbackLock());
    
	//XML files are defined in BinaryData.h
	if (index > 9000) return; // safety

	m_curPatchDataLoadRequestedIndex = index;

	//stop all playing notes
	m_pVASTXperience.m_Poly.stopAllNotes(true);
	m_pVASTXperience.m_iFadeOutSamples.store(m_pVASTXperience.m_iMaxFadeSamples);

	//array defines index!
	if (index >= getNumPrograms()) {
		vassertfalse;
		return;
	}
	if ((m_presetData.getPreset(index)->isFactory == true) || (m_presetData.getPreset(index)->invalid == true)) { //init patch or invalid
		//m_curPatchData = *m_PresetArray[index];
		ValueTree emptytree;
		VASTPresetElement lPatch = m_presetData.getCurPatchData();
		std::thread load_thread(&VASTAudioProcessor::passTreeToAudioThread, emptytree, false, lPatch, index, this, true, true); //init only
		load_thread.detach();
	}
	else { // user preset
		File jFile = File(m_presetData.getPreset(index)->internalid); //change to fullpathname
        std::unique_ptr<juce::XmlDocument> xmlDoc(new XmlDocument(jFile));

		VASTPresetElement lPatch = m_presetData.getCurPatchData();
		bool success = loadPatchXML(xmlDoc.get(), false, &m_presetData.getCurPatchData(), index, lPatch);
		if (!success) {
			m_presetData.reloadPresetArray(false);
			setCurrentProgram(0); //revert to init			
		}
		requestUIInitAfterPrestLoad();
	}
    m_presetToLoad = -1;
}

//load from file - not called when loading from state
bool VASTAudioProcessor::loadPatchXML(XmlDocument* xmlDoc, bool bNameOnly, const VASTPresetElement* preset, int index, VASTPresetElement& resultPresetData) {

	std::unique_ptr<XmlElement> xml = xmlDoc->getDocumentElement(bNameOnly); //if true then this is parsing! only read outer document element if bNameOnly
	//dont support vapo1 patches anymore !! if ((xml->getTagName() == "VASTVaporizerParamsV0.40000") && bNameOnly) xml = xmlDoc->getDocumentElement(false); //for old patches outer document is not sufficient (but size is small)

	if (xml != nullptr) {
		xmlParseToPatch(xml.get(), bNameOnly, preset, index, true, false, resultPresetData); //use external representation here
		xml->deleteAllChildElements();
	}
	else {
		String error = xmlDoc->getLastParseError();
		jassert(false); // broken XML?
		setErrorState(vastErrorState::errorState6_loadPresetFromFileXMLParsingFailed);
		return false;
	}
	return true;
}

XmlElement VASTAudioProcessor::createPatchXML(bool externalRepresentation) { //for saving
	auto copied_state = m_parameterState.copyState();
	if (externalRepresentation) {
		//first clear all parameters //starting with empty tree is better
		copied_state.removeAllChildren(nullptr);

		/*
		for (int i = 0; i < copied_state.getNumChildren(); i++) {
			ValueTree child = copied_state.getChild(i);
			if (child.getType().toString().equalsIgnoreCase("PARAM")) //"PARAM" ...
				copied_state.removeChild(copied_state.indexOf(child), nullptr);
		}
		*/
		
		for (int parameterIndex = 0, numparam = getParameters().size(); parameterIndex < numparam; parameterIndex++) {
			//AudioProcessorParameterWithID* param = (AudioProcessorParameterWithID*)getParameters()[parameterIndex];
			AudioProcessorParameterWithID* param = (AudioProcessorParameterWithID*)getParameters()[parameterIndex];

            std::unique_ptr<ValueTree> parameterChild(new ValueTree(Identifier("PARAM")));
			if (parameterChild->isValid()) {
				//add new text property
				parameterChild->setProperty(
					"id",
					param->paramID, nullptr);

				String valStr = "";
				if (param->paramID.startsWith("m_f")) { //take float as it is, here I would like to get the non-normalised floar value, but how?					
					juce::NormalisableRange<float> range = m_parameterState.getParameterRange(param->paramID);	
					valStr = String(range.convertFrom0to1(param->getValue()));
				}
				else {
					valStr = param->getCurrentValueAsText();
				}

				parameterChild->setProperty(
					"text",
					valStr, nullptr);
				copied_state.appendChild(*parameterChild.get(), nullptr);
			}
		}
	}

	copied_state.setProperty("PatchVersion", "VASTVaporizerParamsV2.20000", nullptr); //newest version 29.10.2019
	copied_state.setProperty("PatchName", m_presetData.getCurPatchData().presetname, nullptr);
	copied_state.setProperty("PatchCategory", m_presetData.getCurPatchData().category, nullptr);
	copied_state.setProperty("PatchTag", m_presetData.getCurPatchData().freetag, nullptr);
	copied_state.setProperty("PatchMPEPreset", (m_presetData.getCurPatchData().mpepreset ? "X" : ""), nullptr);
	copied_state.setProperty("PatchMPEBendRange", String(m_presetData.getCurPatchData().mpebendrange), nullptr);
	copied_state.setProperty("PatchAuthor", m_presetData.getCurPatchData().authorname, nullptr);
	copied_state.setProperty("PatchComments", m_presetData.getCurPatchData().comments, nullptr);
	copied_state.setProperty("CustomModulator1Text", m_presetData.getCurPatchData().customModulator1Text, nullptr);
	copied_state.setProperty("CustomModulator2Text", m_presetData.getCurPatchData().customModulator2Text, nullptr);
	copied_state.setProperty("CustomModulator3Text", m_presetData.getCurPatchData().customModulator3Text, nullptr);
	copied_state.setProperty("CustomModulator4Text", m_presetData.getCurPatchData().customModulator4Text, nullptr);

	addChunkTreeState(&copied_state); //add stuff that is not in the undoable tree (e.g. wavs, wts)
	std::unique_ptr<XmlElement> xml(copied_state.createXml());
		
	return *xml;
}

void VASTAudioProcessor::xmlParseToPatch(XmlElement *pRoot, bool bNameOnly, const VASTPresetElement* preset, int index, bool externalRepresentation, bool isFromState, VASTPresetElement& resultPresetData) {
	String version = pRoot->getStringAttribute("PatchVersion");
	if ((version.equalsIgnoreCase("VASTVaporizerParamsV2.00000")) || 	 	//version with patch meta data for vapo2 05.06.2018
		(version.equalsIgnoreCase("VASTVaporizerParamsV2.10000")) ||        //version 29.10.2019 with changed skew modulation behavior
        (version.equalsIgnoreCase("VASTVaporizerParamsV2.20000"))
        ) {		 	//version with patch meta data for vapo2 19.05.2019
		
		resultPresetData = *preset;
		resultPresetData.version = version;

		if (pRoot->hasTagName("VASTvaporizer2")) { //is it a <VASTvaporizer2> preset?
			ValueTree tree = ValueTree::fromXml(*pRoot);
			if (bNameOnly == true) {
				resultPresetData.presetname = tree.getProperty("PatchName").toString();
				resultPresetData.category = tree.getProperty("PatchCategory").toString();
				resultPresetData.freetag = tree.getProperty("PatchTag").toString();
				resultPresetData.mpepreset = tree.getProperty("PatchMPEPreset").toString().equalsIgnoreCase("X");
				resultPresetData.mpebendrange = tree.getProperty("PatchMPEBendRange").toString().getIntValue();
				resultPresetData.authorname = tree.getProperty("PatchAuthor").toString();
				resultPresetData.comments = tree.getProperty("PatchComments").toString();
				resultPresetData.customModulator1Text = tree.getProperty("CustomModulator1Text").toString();
				resultPresetData.customModulator2Text = tree.getProperty("CustomModulator2Text").toString();
				resultPresetData.customModulator3Text = tree.getProperty("CustomModulator3Text").toString();
				resultPresetData.customModulator4Text = tree.getProperty("CustomModulator4Text").toString();
			}
			else  { 
				if (isFromState) {
					passTreeToAudioThread(tree, externalRepresentation, resultPresetData, index, this, false, false); //unthreaded
				} else { //ui load threaded
					std::thread load_thread(&VASTAudioProcessor::passTreeToAudioThread, tree, externalRepresentation, resultPresetData, index, this, true, false);
					load_thread.detach();
				}				
			}
		} 
	}
	else {
		setErrorState(vastErrorState::errorState7_unknownPresetFormatVersion);
		jassert(false); // unknown patch format
	}
}

void VASTAudioProcessor::passTreeToAudioThread(ValueTree tree, bool externalRepresentation, VASTPresetElement preset, int index, VASTAudioProcessor* processor, bool isSeparateThread, bool initOnly) {
	VDBG("passTreeToAudioThread started.");
	if (processor == nullptr) return;

	if (!initOnly)
		if (!tree.isValid()) {
			processor->setErrorState(vastErrorState::errorState20_loadPresetInvalidTree);
			return;
		}

	int waitstate = 0;	
	if (isSeparateThread) {
		processor->registerThread();
		if (processor->getTreeThreadLock() == true) {
			processor->unregisterThread();
			//This is not an error state. The current load thread is just not executed. Another load will return. 
			return;
		}
	}	
	
	waitstate = 0;
	if (!processor->m_pVASTXperience.audioProcessLock()) {
		processor->unregisterThread();
		//This is not an error state. The current load thread is just not executed. Another load will return. 
		vassertfalse;
		return; //some other task has already blocked it
	}
	if (isSeparateThread) {
		if ((processor->m_bAudioThreadStarted) && (!processor->m_wasBypassed)) {
			while (processor->m_pVASTXperience.getBlockProcessingIsBlockedSuccessfully() == false) {
				VDBG("getBlockProcessingIsBlockedSuccessfully() is false - suspending load process!");				
				std::this_thread::sleep_for(std::chrono::milliseconds(250));
				waitstate += 50;
				if (waitstate > 2500) {
					//processor->m_pVASTXperience.audioProcessUnlock(); //must not unlock here
					processor->unregisterThread();
					processor->setErrorState(vastErrorState::errorState16_loadPresetLockUnsuccessful);
					VDBG("ERROR! getBlockProcessingIsBlockedSuccessfully() is false - terminating load process!");
					return; //end after 25s waiting time
				}
			}
		}
	}

	if (isSeparateThread) {
		if ((processor->m_bAudioThreadStarted) && (!processor->m_wasBypassed)) {
			if (!processor->m_pVASTXperience.getBlockProcessingIsBlockedSuccessfully()) {
				vassertfalse;
				processor->unregisterThread();
				if (!processor->m_pVASTXperience.audioProcessUnlock()) {
					//already in error state
				}
				processor->setErrorState(vastErrorState::errorState16_loadPresetLockUnsuccessful);
				return;
			}
		}
	}
	
    /*
	std::shared_ptr<CVASTWaveTable> m_bank_wavetableToUpdate[4] = { processor->m_pVASTXperience.m_Poly.m_OscBank[0].getSoftOrCopyWavetable(false),
        processor->m_pVASTXperience.m_Poly.m_OscBank[1].getSoftOrCopyWavetable(false),
        processor->m_pVASTXperience.m_Poly.m_OscBank[2].getSoftOrCopyWavetable(false),
        processor->m_pVASTXperience.m_Poly.m_OscBank[3].getSoftOrCopyWavetable(false) };
     */
    
    std::shared_ptr<CVASTWaveTable> m_bank_wavetableToUpdate[4] = { 
        std::make_shared<CVASTWaveTable>(processor->m_pVASTXperience.m_Set),
        std::make_shared<CVASTWaveTable>(processor->m_pVASTXperience.m_Set),
        std::make_shared<CVASTWaveTable>(processor->m_pVASTXperience.m_Set),
        std::make_shared<CVASTWaveTable>(processor->m_pVASTXperience.m_Set)
    };

	//adapt to formerly too long parameter lengthes for compatibility
	for (int i = 0; i < tree.getNumChildren(); i++) {
		ValueTree loadedTreeChild = tree.getChild(i);
		String ids = loadedTreeChild.getPropertyAsValue("id", nullptr, false).toString();
		if (ids.contains("MultibandCompressor")) {
			VDBG("Replaced old value: " << ids);
			loadedTreeChild.setProperty("id", ids.replaceFirstOccurrenceOf("MultibandCompressor", "MBComp", false), nullptr);
		}
	}

	VASTSamplerSound* m_soundToUpdate;
	bool b_success = false;
	if (!initOnly) {
		if (externalRepresentation) { //when loading from file
			for (int i = 0; i < tree.getNumChildren(); i++) {
				//check if valid element
				ValueTree loadedTreeChild = tree.getChild(i);
				Identifier elem = loadedTreeChild.getType(); //"PARAM" ...
				ValueTree wantedChild = processor->m_parameterState.state.getChildWithName(elem);
				if (!wantedChild.isValid()) {
					if (!elem.toString().equalsIgnoreCase("chunkData")) //chunkData is not in the active state, added on demand
						jassertfalse;
				}
				else {
					if (elem.toString().equalsIgnoreCase("PARAM")) {
						String loadedChildProperty = loadedTreeChild.getPropertyAsValue("id", nullptr, false).toString();
						ValueTree parameterChild = processor->m_parameterState.state.getChildWithProperty("id", loadedChildProperty);
						if (!parameterChild.isValid()) {
							//delete the crap
							VDBG("TreeValue removed: " << loadedTreeChild.getPropertyAsValue("id", nullptr, false).toString() << " " << loadedTreeChild.getPropertyAsValue("text", nullptr, false).toString());
							tree.removeChild(tree.indexOf(loadedTreeChild), nullptr);
							jassertfalse; //error state or tempered preset?
						}
						else {
							//change from "text" to "value" here
							String loadedChildText = loadedTreeChild.getPropertyAsValue("text", nullptr, false).toString();

							AudioProcessorParameterWithID* param = processor->m_parameterState.getParameter(loadedChildProperty);
							vassert(param != nullptr);
							String valStr = "";
							if (loadedChildProperty.startsWith("m_f") || loadedChildProperty.startsWith("m_i")) //take float and int as it is
								valStr = loadedChildText;
							else {
								//valStr = String(param->getAllValueStrings().indexOf(loadedChildText, true)); //bool and combobox need to be translated							
								juce::NormalisableRange<float> range = processor->m_parameterState.getParameterRange(param->paramID);
								valStr = String(range.convertFrom0to1(param->getValueForText(loadedChildText)));
							}

							loadedTreeChild.setProperty(
								"value",
								valStr, nullptr);
							loadedTreeChild.removeProperty("text", nullptr);                                                       
						}
					}
				}
			}
		}
		
		processor->initializeToDefaults();

		//chunks - for daw state and file!
		String childName = "chunkData";
		ValueTree l_chunkTree = tree.getChildWithName(Identifier(childName));
		if (l_chunkTree.isValid()) { //init patch does not have it			
		//Prepare OscBanks wavetables
			ValueTree l_tree{};
			for (int bank = 0; bank < 4; bank++) {
				childName = "oscBank" + String(bank);
				l_tree = l_chunkTree.getChildWithName(Identifier(childName));
				if (!l_tree.isValid()) {
					processor->setErrorState(vastErrorState::errorState8_loadPresetOscillatorTreeInvalid);
					processor->unregisterThread();
					if (!processor->m_pVASTXperience.audioProcessUnlock()) {
						//already in error state
					}
					return; //error handling
				}
				if (!m_bank_wavetableToUpdate[bank]->setValueTreeState(&l_tree, processor->getWTmode())) {
					processor->setErrorState(vastErrorState::errorState9_loadPresetWavetableDataInvalid);
					processor->unregisterThread();
					if (!processor->m_pVASTXperience.audioProcessUnlock()) {
						//already in error state
					}
					return; //error handling
				}
			}

			//WAV
			childName = "sampleData";
			m_soundToUpdate = nullptr;
			l_tree = l_chunkTree.getChildWithName(Identifier(childName));
			if (l_tree.isValid()) { //might not exist which is OK		
				m_soundToUpdate = VASTSamplerSound::getSoundFromTree(&l_tree); //check for memory leak here!!
			}
		}

		b_success = true;

		if ((preset.version.equalsIgnoreCase("VASTVaporizerParamsV2.00000")) || //have to adapt skew modulation for older patches
			(preset.version.equalsIgnoreCase("VASTVaporizerParamsV2.10000"))) {
			processor->m_pVASTXperience.m_Set.m_bBeforeV22CompatibilityMode = true;
		}

		if (l_chunkTree.isValid()) { //init patch does not have it
			ValueTree l_tree{};
			//---------------------------------------------------------------------------------------
			//after this it has to be quick

			//WAV
			if (m_soundToUpdate != nullptr) {
				VASTSynthesiserSound* sound = processor->m_pVASTXperience.m_Poly.getSynthSound();
				vassert(sound != nullptr);
				if (sound != nullptr) {
					sound->clearSamplerSound();
					sound->addSamplerSound(m_soundToUpdate); //changed done inside
				}
				else
					processor->setErrorState(vastErrorState::errorState10_loadPresetChunkDataInvalid);
			}

			//msegs
			for (int mseg = 0; mseg < 5; mseg++) {
				childName = "msegData" + String(mseg);
				l_tree = l_chunkTree.getChildWithName(Identifier(childName));
				if (l_tree.isValid()) {
					processor->m_pVASTXperience.m_Set.m_MSEGData_changed[mseg].setValueTreeState(&l_tree, true, &processor->m_pVASTXperience.m_Set); //is mseg
					processor->m_pVASTXperience.m_Set.m_MSEGData[mseg].setValueTreeState(&l_tree, true, &processor->m_pVASTXperience.m_Set); //is mseg
				}
			}

			//step seqs
			for (int i = 0; i < 3; i++) {
				childName = "stepSeqData" + String(i);
				l_tree = l_chunkTree.getChildWithName(Identifier(childName));
				if (l_tree.isValid()) {
					processor->m_pVASTXperience.m_Set.m_StepSeqData_changed[i].setValueTreeState(&l_tree, false, &processor->m_pVASTXperience.m_Set); //is stepseq
					processor->m_pVASTXperience.m_Set.m_StepSeqData[i].setValueTreeState(&l_tree, false, &processor->m_pVASTXperience.m_Set); //is stepseq
				}
			}

			//arp
			for (int i = 0; i < 1; i++) { //only one
				childName = "arpData" + String(i);
				l_tree = l_chunkTree.getChildWithName(Identifier(childName));
				if (l_tree.isValid()) {
					processor->m_pVASTXperience.m_Set.m_ARPData_changed.setValueTreeState(&l_tree);
					processor->m_pVASTXperience.m_Set.m_ARPData.setValueTreeState(&l_tree);
				}
			}

			//fx bus
			{
				childName = "fxBusData" + String(1);
				l_tree = l_chunkTree.getChildWithName(Identifier(childName));
				if (l_tree.isValid()) {
					processor->m_pVASTXperience.m_fxBus1.setValueTreeState(&l_tree);
				}
			}
			{
				childName = "fxBusData" + String(2);
				l_tree = l_chunkTree.getChildWithName(Identifier(childName));
				if (l_tree.isValid()) {
					processor->m_pVASTXperience.m_fxBus2.setValueTreeState(&l_tree);
				}
			}
			{
				childName = "fxBusData" + String(3);
				l_tree = l_chunkTree.getChildWithName(Identifier(childName));
				if (l_tree.isValid()) {
					processor->m_pVASTXperience.m_fxBus3.setValueTreeState(&l_tree);
				}
			}

			tree.removeChild(l_chunkTree, nullptr);
			VDBG("passTreeToAudioThread repalce tree start.");
			processor->m_parameterState.replaceState(tree);
			VDBG("passTreeToAudioThread repalce tree end.");

			processor->m_pVASTXperience.m_Poly.updateVariables();
			processor->m_pVASTXperience.m_Poly.updateLFO(0);
			processor->m_pVASTXperience.m_Poly.updateLFO(1);
			processor->m_pVASTXperience.m_Poly.updateLFO(2);
			processor->m_pVASTXperience.m_Poly.updateLFO(3);
			processor->m_pVASTXperience.m_Poly.updateLFO(4);

			//wavetables	
			for (int bank = 0; bank < 4; bank++) {
				int wtFxType = 0; 
				float wtFxVal = 0.f; 
				switch (bank) {
				case 0:
					wtFxType = processor->m_pVASTXperience.m_Set.m_State->m_uWTFX_OscA->load();
					wtFxVal = processor->m_pVASTXperience.m_Set.m_State->m_fWTFXVal_OscA->load();
					break;
				case 1:
					wtFxType = processor->m_pVASTXperience.m_Set.m_State->m_uWTFX_OscB->load();
					wtFxVal = processor->m_pVASTXperience.m_Set.m_State->m_fWTFXVal_OscB->load();
					break;
				case 2:
					wtFxType = processor->m_pVASTXperience.m_Set.m_State->m_uWTFX_OscC->load();
					wtFxVal = processor->m_pVASTXperience.m_Set.m_State->m_fWTFXVal_OscC->load();
					break;
				case 3:
					wtFxType = processor->m_pVASTXperience.m_Set.m_State->m_uWTFX_OscD->load();
					wtFxVal = processor->m_pVASTXperience.m_Set.m_State->m_fWTFXVal_OscD->load();
					break;
				}

				//Expensive and optional
				//m_bank_wavetableToUpdate[bank]->pregenerateWithWTFX(wtFxType, wtFxVal, processor->getWTmode());
				//Expensive and optional

				if (processor->m_bAudioThreadCurrentlyRunning)
					processor->m_pVASTXperience.m_Poly.m_OscBank[bank].setWavetableSoftFade(m_bank_wavetableToUpdate[bank]);
				else
					processor->m_pVASTXperience.m_Poly.m_OscBank[bank].setWavetable(m_bank_wavetableToUpdate[bank]); //save from state, audio process not running				
			}
		}
		else
			processor->setErrorState(vastErrorState::errorState10_loadPresetChunkDataInvalid);
	}
	else {
		processor->initializeToDefaults();
		b_success = true;
	}

	VASTPresetElement presetElement;
	presetElement = *processor->m_presetData.getPreset(index);
	if (!initOnly) { 
		presetElement.isFactory = false;
		//presetElement.internalid = 
		//presetElement.presetid = 
		presetElement.presetname = tree.getProperty("PatchName").toString();
		presetElement.category = tree.getProperty("PatchCategory").toString();
		presetElement.freetag = tree.getProperty("PatchTag").toString();
		presetElement.mpepreset = tree.getProperty("PatchMPEPreset").toString().equalsIgnoreCase("X");
		presetElement.mpebendrange = tree.getProperty("PatchMPEBendRange").toString().getIntValue();
		presetElement.authorname = tree.getProperty("PatchAuthor").toString();
		presetElement.comments = tree.getProperty("PatchComments").toString();
		presetElement.customModulator1Text = tree.getProperty("CustomModulator1Text").toString();
		presetElement.customModulator2Text = tree.getProperty("CustomModulator2Text").toString();
		presetElement.customModulator3Text = tree.getProperty("CustomModulator3Text").toString();
		presetElement.customModulator4Text = tree.getProperty("CustomModulator4Text").toString();
	}
	processor->m_presetData.exchangeCurPatchData(presetElement);
	
	processor->m_curPatchDataLoadRequestedIndex = presetElement.presetarrayindex;

	processor->m_parameterState.undoManager->clearUndoHistory();
	processor->m_parameterState.undoManager->beginNewTransaction(); //start new transcation only here?

	processor->m_pVASTXperience.m_Poly.m_OscBank[0].setChangedFlag();
	processor->m_pVASTXperience.m_Poly.m_OscBank[1].setChangedFlag();
	processor->m_pVASTXperience.m_Poly.m_OscBank[2].setChangedFlag();
	processor->m_pVASTXperience.m_Poly.m_OscBank[3].setChangedFlag();

	if (b_success == false) {
		processor->setErrorState(vastErrorState::errorState11_loadPresetUnsuccessful);
		processor->requestUIAlert();
		processor->requestUIInitAfterPrestLoad(); 
		//---------------------------------------------------------------------------------------
	}
	else {
		if (isSeparateThread) {
			if ((processor->m_bAudioThreadStarted) && (!processor->m_wasBypassed)) {
				if (!processor->m_pVASTXperience.nonThreadsafeIsBlockedProcessingInfo()) {
					if (!processor->m_pVASTXperience.audioProcessLock()) {
						//already locked by other process
						processor->setErrorState(vastErrorState::errorState16_loadPresetLockUnsuccessful);
						processor->unregisterThread();
						return; //dont unlock what is not locked
					}
					if ((processor->m_bAudioThreadStarted) && (!(processor->m_pVASTXperience.getBlockProcessingIsBlockedSuccessfully()))) {
						bool done = false;
						int counter = 30;						
						while (!done) {
							if ((counter < 30) && ((processor->m_bAudioThreadStarted) && (!(processor->m_pVASTXperience.getBlockProcessingIsBlockedSuccessfully())))) {
								VDBG("PassTree - sleep");
								Thread::sleep(100);
								counter++;
								continue;
							}
							vassert(counter < 30); //process lock failed
							if (counter == 30) {
								processor->setErrorState(vastErrorState::errorState16_loadPresetLockUnsuccessful);
								processor->unregisterThread();
								return; //dont unlock what is not locked
							}
							done = true;
						}
					}
				}
			}
		}
		if (processor->getSampleRate() != 0)
			processor->m_pVASTXperience.prepareForPlay(processor->getSampleRate(), processor->getBlockSize());
		processor->requestUIInitAfterPrestLoad(); 
		//---------------------------------------------------------------------------------------
	}

	processor->m_pVASTXperience.m_iFadeInSamples.store(processor->m_pVASTXperience.m_iMaxFadeSamples);
	//===============================

	processor->unregisterThread();
	processor->m_pVASTXperience.audioProcessUnlock(); 
	processor->m_loadedPresetIndexCount++;
	VDBG("passTreeToAudioThread ended.");
}

bool VASTAudioProcessor::lockedAndSafeToDoDeallocatios()
{
	if (isAudioThread()) //audioThread shall not lock itself
		return true;
	return ((!m_bAudioThreadStarted)
		|| ((m_pVASTXperience.m_BlockProcessing.load() && m_pVASTXperience.m_BlockProcessingIsBlockedSuccessfully.load())));
}

void VASTAudioProcessor::registerThread() {
	//const ScopedLock sl(getCallbackLock());
	m_iNumPassTreeThreads++;
	VDBG("Register Num Threads registered: " << m_iNumPassTreeThreads);
}

void VASTAudioProcessor::unregisterThread() {
	//const ScopedLock sl(getCallbackLock());
	m_iNumPassTreeThreads--;
	if (m_iNumPassTreeThreads < 0) m_iNumPassTreeThreads = 0;
	VDBG("Unregister Num Threads registered: " << m_iNumPassTreeThreads);
}

bool VASTAudioProcessor::getTreeThreadLock() {
	//const ScopedLock sl(getCallbackLock());
	return m_iNumPassTreeThreads > 1;
}

void VASTAudioProcessor::toggleKeyboardHoldMode() {
    m_keyboardHoldMode.store(!m_keyboardHoldMode.load());
    m_pVASTXperience.m_Poly.setKeyboardHoldMode(m_keyboardHoldMode.load());
}

/*
AudioProcessorParameterWithID* VASTAudioProcessor::getParameterWithID(String paramID) {
	std::map<String, AudioProcessorParameterWithID*>::iterator l_iter = m_mapParameterToID.find(paramID);
	if (l_iter->first == paramID) {
		if (l_iter->second != nullptr)
			return l_iter->second;
	}
	return nullptr;
}
*/

void VASTAudioProcessor::addModMatrixLookupTable(int modMatrixDestination, float rangeStart, float rangeEnd, float rangeSkew, StringRef paramID, AudioProcessorParameterWithID* param) {
	m_modMatrixLookupTable[modMatrixDestination].rangeStart = rangeStart;
	m_modMatrixLookupTable[modMatrixDestination].rangeEnd = rangeEnd;
	m_modMatrixLookupTable[modMatrixDestination].rangeSkew = rangeSkew;
	m_modMatrixLookupTable[modMatrixDestination].paramID = paramID;
	m_modMatrixLookupTable[modMatrixDestination].param = param;
}

bool VASTAudioProcessor::nonThreadsafeIsBlockedProcessingInfo() {
	return m_pVASTXperience.nonThreadsafeIsBlockedProcessingInfo();
}

VASTPresetElement VASTAudioProcessor::getCurPatchData() {
	return m_presetData.getCurPatchData();
}

float VASTAudioProcessor::getPluginScaleWidthFactor() {
	if (m_iDefaultPluginWidth != 0)
		return m_iUserTargetPluginWidth / float(m_iDefaultPluginWidth);
	return 1.f;
}

float VASTAudioProcessor::getPluginScaleHeightFactor() {
	if (m_iDefaultPluginHeight != 0)
		return m_iUserTargetPluginHeight / float(m_iDefaultPluginHeight);
	return 1.f;
}

void VASTAudioProcessor::togglePerspectiveDisplay(int lOscillatorBank) {
	m_bTogglePerspectiveDisplay[lOscillatorBank] = !m_bTogglePerspectiveDisplay[lOscillatorBank];
	writeSettingsToFileAsync();
}

void VASTAudioProcessor::setWTmode(int wtMode) {
	if (wtMode != m_pVASTXperience.m_Set.m_WTmode) {
		m_pVASTXperience.m_Set.m_WTmode = wtMode;
		//recalc WT
		for (int bank = 0; bank < 4; bank++)
			m_pVASTXperience.m_Poly.m_OscBank[bank].recalcWavetable();
	}
}

int VASTAudioProcessor::getWTmode() const {
	return m_pVASTXperience.m_Set.m_WTmode;
}

int VASTAudioProcessor::getMPEmode() const {
	return m_MPEmode;
}

void VASTAudioProcessor::setMPEmode(int mode) {
	m_MPEmode = jlimit<int>(0, 2, mode);
}

bool VASTAudioProcessor::isMPEenabled() {
	return (m_MPEmode == 1) || ((m_MPEmode == 0) && (m_presetData.getCurPatchData().mpepreset));
}

int VASTAudioProcessor::getUIFontSize() const {
	return m_uiFontSize;
}

void VASTAudioProcessor::setUIFontSize(int size) {
	m_uiFontSize = size;
	for (int i = 0; i < vastLookAndFeels.size(); i++)
		vastLookAndFeels[i]->setUIFontSize(size);
	requestUIInit();
}

void VASTAudioProcessor::setModWheelPermaLink(int permalink)
{
	m_ModWheelPermaLink = permalink;
}

int VASTAudioProcessor::getModWheelPermaLink() const
{
	return m_ModWheelPermaLink;
}

void VASTAudioProcessor::setUserTuningFile(String filename) {
	m_UserTuningFile = filename;
	m_pVASTXperience.m_Set.setTuning(m_UserTuningFile);
}

void VASTAudioProcessor::setBendRange(int bendRange) {
	m_pVASTXperience.m_Set.m_iBendRange = bendRange;
	requestUIUpdate(true, false, false);
}

int VASTAudioProcessor::getBendRange() const {
	return m_pVASTXperience.m_Set.m_iBendRange;
}

int VASTAudioProcessor::getDrawMode() const { return m_iWTEditorDrawMode; }

int VASTAudioProcessor::getGridMode() const { return m_iWTEditorGridMode; }

int VASTAudioProcessor::getBinMode() const { return m_iWTEditorBinMode; }

int VASTAudioProcessor::getBinEditMode() const { return m_iWTEditorBinEditMode; }

String VASTAudioProcessor::getMidiKeyboardCharLayout()
{
	return m_MidiKeyboardCharLayout;
}

int VASTAudioProcessor::getMidiKeyboardBaseOctave()
{
	return m_iMidiKeyboardBaseOctave;
}

void VASTAudioProcessor::setMidiKeyboardCharLayout(String charLayout)
{
	m_MidiKeyboardCharLayout = charLayout;
	VASTAudioProcessorEditor* editor = (VASTAudioProcessorEditor*)getActiveEditor();
	if (editor != nullptr) {
		if (editor->vaporizerComponent != nullptr) {
			editor->vaporizerComponent->getKeyboardComponent()->updateMidiKeyboardCharLayout();
		}
	}
}

void VASTAudioProcessor::setMidiKeyboardBaseOctave(int baseOctave)
{
	m_iMidiKeyboardBaseOctave = baseOctave;
	VASTAudioProcessorEditor* editor = (VASTAudioProcessorEditor*)getActiveEditor();
	if (editor != nullptr) {
		if (editor->vaporizerComponent != nullptr)
			editor->vaporizerComponent->getKeyboardComponent()->updateMidiKeyboardBaseOctave();
	}	
}

int VASTAudioProcessor::autoParamGetDestination(String parametername) {
	auto it = m_mapParameterNameToModdest.find(parametername); //binary search
    if (!it->first.equalsIgnoreCase(parametername)) return -1;
	return it->second;
};

String VASTAudioProcessor::autoDestinationGetParam(int modmatdest) {
    auto it = m_mapModdestToParameterName.find(modmatdest); //binary search
	if (it->first != modmatdest) return "";
	return it->second;
};

void VASTAudioProcessor::setParameterFloat01(StringRef parName, float nvalue, bool bSilent) {
	vassert((nvalue >= 0.f) && (nvalue <= 1.f));
	AudioProcessorParameterWithID* param = m_parameterState.getParameter(parName);
	if (param != nullptr) {
		if (bSilent)
			param->setValue(nvalue);
		else
			param->setValueNotifyingHost(nvalue);
	}
	else
		vassertfalse;
}

void VASTAudioProcessor::setParameterText(StringRef parName, StringRef textVal, bool bSilent) {
	AudioProcessorParameterWithID* param = m_parameterState.getParameter(parName);
	if (param != nullptr) {
		float nvalue = 0.0f;
		nvalue = param->getValueForText(textVal);

		if (bSilent)
			param->setValue(nvalue);
		else
			param->setValueNotifyingHost(nvalue);
	}
	else
		vassertfalse;
}

AudioProcessorValueTreeState& VASTAudioProcessor::getParameterTree() {
    return m_parameterState;
}

VASTVUMeterSource* VASTAudioProcessor::getMeterSource() {
	return &m_meterSource;
}

char* VASTAudioProcessor::_strncpy(char* dst, const char* src, size_t maxLen)
{
	char* result = strncpy(dst, src, maxLen);
	dst[maxLen] = 0;
	return result;
}

String VASTAudioProcessor::getVSTPath() {
	FileSearchPath defaultVSTPaths;
	const String programFiles(File::getSpecialLocation(File::globalApplicationsDirectory).getFullPathName());

#if JUCE_MAC
    String lPath = "/Applications/Vaporizer2";
    File file = File(lPath);
    if (file.exists()) { // exists on MAC
        return lPath;
	} else {
         const String currentDll(File::getSpecialLocation(File::currentApplicationFile).getFullPathName());
         File file = File(currentDll);
         if (file.isDirectory()) { // on Mac its a package folder
         return file.getParentDirectory().getFullPathName();
         }
        
         const String currentDll2(File::getSpecialLocation(File::currentExecutableFile).getFullPathName());
         File file2 = File(currentDll2);
         if (file2.existsAsFile()) {
         return file2.getParentDirectory().getFullPathName();
         }
    }
	return "";

#elif JUCE_LINUX
	String dataPath = CMAKE_INSTALL_FULL_DATAROOTDIR "/Vaporizer2";
	if (File(dataPath).exists()) {
		return dataPath;
	}
	return "/usr/share/Vaporizer2";

#elif JUCE_WINDOWS	
	String Vaporizer2InstallPath = "";
	Vaporizer2InstallPath = WindowsRegistry::getValue("HKEY_LOCAL_MACHINE\\SOFTWARE\\VAST Dynamics\\Vaporizer2\\Settings\\InstallPath", "", WindowsRegistry::WoW64_64bit); //Always use the 64-bit registry store.
	File file_instpath = File(Vaporizer2InstallPath+"\\license.txt"); //check for that file??
	if (file_instpath.existsAsFile()) {
		return file_instpath.getParentDirectory().getFullPathName();
	}

	const String currentDll(File::getSpecialLocation(File::currentApplicationFile).getFullPathName());
	File file = File(currentDll);
	if (file.existsAsFile()) {
		return file.getParentDirectory().getFullPathName();
	}

	const String currentDll2(File::getSpecialLocation(File::currentExecutableFile).getFullPathName());
	File file2 = File(currentDll2);
	if (file2.existsAsFile()) {
		return file2.getParentDirectory().getFullPathName();
	}

	defaultVSTPaths.add(WindowsRegistry::getValue("HKEY_LOCAL_MACHINE\\SOFTWARE\\VST\\VSTPluginsPath",
		programFiles + "\\Steinberg\\VstPlugins"));

	defaultVSTPaths.add(WindowsRegistry::getValue("HKEY_LOCAL_MACHINE\\SOFTWARE\\VST\\VSTPluginsPath",
		programFiles + "\\VST Plugins"));

	defaultVSTPaths.removeNonExistentPaths();

	jassert(defaultVSTPaths.getNumPaths() > 0); //There are no VST plugin folders on this machine...

	for (int i = 0; i < defaultVSTPaths.getNumPaths(); i++) {
		String filename = defaultVSTPaths[i].getFullPathName() + "\\" + CONST_DLL_NAME;
		File file = File(filename);
		if (file.existsAsFile())
			return defaultVSTPaths[i].getFullPathName();
	}
	for (int i = 0; i < defaultVSTPaths.getNumPaths(); i++) {
		String filename = defaultVSTPaths[i].getFullPathName() + "\\VASTvaporizer\\" + CONST_DLL_NAME;
		File file = File(filename);
		if (file.existsAsFile())
			return defaultVSTPaths[i].getFullPathName() + "\\VASTvaporizer\\";
	}
	return "";
#endif
}

String VASTAudioProcessor::getVSTPathAlternative() {
	FileSearchPath defaultVSTPaths;
	const String programFiles(File::getSpecialLocation(File::globalApplicationsDirectory).getFullPathName());

#if JUCE_MAC
	const String currentDll(File::getSpecialLocation(File::currentApplicationFile).getFullPathName());
	File file = File(currentDll);
	if (file.isDirectory()) { // on Mac its a package folder
		return file.getParentDirectory().getFullPathName();
	}

	const String currentDll2(File::getSpecialLocation(File::currentExecutableFile).getFullPathName());
	File file2 = File(currentDll2);
	if (file2.existsAsFile()) {
		return file2.getParentDirectory().getFullPathName();
	}

	String lPath = "~/Library/Audio/Plug-Ins/Components";
	defaultVSTPaths.add(lPath);
	lPath = "/Library/Audio/Plug-Ins/Components";
	defaultVSTPaths.add(lPath);
	lPath = "~/Library/Audio/Plug-Ins/VST";
	defaultVSTPaths.add(lPath);
	lPath = "/Library/Audio/Plug-Ins/VST";
	defaultVSTPaths.add(lPath);
	lPath = "~/Library/Audio/Plug-Ins/VST3";
	defaultVSTPaths.add(lPath);
	lPath = "/Library/Audio/Plug-Ins/VST3";
	defaultVSTPaths.add(lPath);
    lPath = "/Applications/Vaporizer2";
    defaultVSTPaths.add(lPath);

	defaultVSTPaths.removeNonExistentPaths();

	jassert(defaultVSTPaths.getNumPaths() > 0); //There are no VST plugin folders on this machine...

												/*  // no subfolder under Mac Os - otherwise AU will not be found
												for (int i = 0; i < defaultVSTPaths.getNumPaths(); i++) {
												String filename = defaultVSTPaths[i].getFullPathName() + "/VASTvaporizer/" + CONST_DLL_NAME;
												File file = File(filename);
												if (file.exists())  //exists on MAC
												return defaultVSTPaths[i].getFullPathName() + "/VASTvaporizer/";
												}
												*/
	for (int i = 0; i < defaultVSTPaths.getNumPaths(); i++) {
		String filename = defaultVSTPaths[i].getFullPathName() + "/" + CONST_DLL_NAME;
		File file = File(filename);
		if (file.exists()) // exists on MAC
			return defaultVSTPaths[i].getFullPathName();
	}
	return "";

#elif JUCE_LINUX
	return "/usr/share/Vaporizer2";

#elif JUCE_WINDOWS	
	const String currentDll(File::getSpecialLocation(File::currentApplicationFile).getFullPathName());
	File file = File(currentDll);
	if (file.existsAsFile()) {
		return file.getParentDirectory().getFullPathName();
	}

	const String currentDll2(File::getSpecialLocation(File::currentExecutableFile).getFullPathName());
	File file2 = File(currentDll2);
	if (file2.existsAsFile()) {
		return file2.getParentDirectory().getFullPathName();
	}

	defaultVSTPaths.add(WindowsRegistry::getValue("HKEY_LOCAL_MACHINE\\SOFTWARE\\VST\\VSTPluginsPath",
		programFiles + "\\Steinberg\\VstPlugins"));

	defaultVSTPaths.add(WindowsRegistry::getValue("HKEY_LOCAL_MACHINE\\SOFTWARE\\VST\\VSTPluginsPath",
		programFiles + "\\VST Plugins"));

	defaultVSTPaths.removeNonExistentPaths();

	jassert(defaultVSTPaths.getNumPaths() > 0); //There are no VST plugin folders on this machine...

	for (int i = 0; i < defaultVSTPaths.getNumPaths(); i++) {
		String filename = defaultVSTPaths[i].getFullPathName() + "\\" + CONST_DLL_NAME;
		file = File(filename);
		if (file.existsAsFile())
			return defaultVSTPaths[i].getFullPathName();
	}
	for (int i = 0; i < defaultVSTPaths.getNumPaths(); i++) {
		String filename = defaultVSTPaths[i].getFullPathName() + "\\VASTvaporizer\\" + CONST_DLL_NAME;
		file = File(filename);
		if (file.existsAsFile())
			return defaultVSTPaths[i].getFullPathName() + "\\VASTvaporizer\\";
	}
	return "";
#endif
}

bool VASTAudioProcessor::readLicense() {
	string line = "";
	String file_enc = "";
	m_sLicenseInformation.m_bIsLegacyLicense = true;
	m_sLicenseInformation.m_bIsLicensed = false;
	m_sLicenseInformation.m_bIsFreeVersion = false;
    m_sLicenseInformation.m_bExpired = false;
	m_sLicenseString = "Free version";

	String filename = File(getVSTPath()).getChildFile("VASTDynamics.actkey").getFullPathName();
	File activationfile(filename);
	if (activationfile.existsAsFile()) {
		String activationFileData = activationfile.loadFileAsString();
#if defined(VASTCOMMERCIAL) || defined(VASTBUILD)
		if (mLicense.validateActivationFile(activationFileData, S_VAPORIZER2_PRODUCTID, getLocalMachineID(), m_sLicenseString, m_sLicenseInformation.mActivationString, m_sLicenseInformation.mUserID, m_sLicenseInformation.mProduct_id, m_sLicenseInformation.mReg_name, m_sLicenseInformation.mEmail, m_sLicenseInformation.mPurchase_id, m_sLicenseInformation.mPurchase_date)) {
	#if defined(VASTCOMMERCIAL)
			m_sLicenseInformation.m_bIsLicensed = true;
			m_sLicenseInformation.m_bExpired = false;
			m_sLicenseInformation.m_bIsLegacyLicense = false;
	#else
        } else { //invalid actkey
            m_sLicenseString = "Free version";
        }
    #endif
#endif
	}

#ifndef VASTCOMMERCIAL
	m_sLicenseInformation.m_bIsLegacyLicense = false;
	m_sLicenseInformation.m_bIsLicensed = true;
	m_sLicenseInformation.m_bIsFreeVersion = true;
	return m_sLicenseInformation.m_bIsLicensed;
#endif	   	 

	if (!m_sLicenseInformation.m_bIsLicensed) {
		//legacy
		filename = File(getVSTPath()).getChildFile("VASTvaporizer.key").getFullPathName();
		ifstream licensefile;
		const char* fname = filename.toRawUTF8();
		licensefile.open(fname);
		//ifstream licesefile(filename.toStdString()); //due to OSX 10.6 compatibility
		if (licensefile.is_open())
		{
			while (std::getline(licensefile, line))
			{
				file_enc << line;
			}
			licensefile.close();
		}
		else { //to allow that keyfile can still be placed where the dll is even when install folder exists
			filename = File(getVSTPathAlternative()).getChildFile("VASTvaporizer.key").getFullPathName();
			const char* fname = filename.toRawUTF8();
			licensefile.open(fname);
			//ifstream licesefile(filename.toStdString()); //due to OSX 10.6 compatibility
			if (licensefile.is_open())
			{
				while (std::getline(licensefile, line))
				{
					file_enc << line;
				}
				licensefile.close();
			}
		}
#ifdef VASTCOMMERCIAL
		m_sLicenseInformation.m_bIsLicensed = mLicense.validateLicenseFile(file_enc, &m_sLicenseString, m_sLicenseInformation.mProduct_id, m_sLicenseInformation.mReg_name, m_sLicenseInformation.mEmail, m_sLicenseInformation.mPurchase_id, m_sLicenseInformation.mPurchase_date, m_sLicenseInformation.mLegacyLicenseString);
#endif
	}
	
	if (m_sLicenseInformation.m_bIsLicensed)
		m_sLicenseInformation.m_bExpired = false;
	return m_sLicenseInformation.m_bIsLicensed;
}

void VASTAudioProcessor::checkForNewerVersion(String resultString) {
	StringArray resultTokens;
	resultTokens.addTokens(resultString, ";", "\"");
	for (int i = 0; i < resultTokens.size(); i++) {
		StringArray wordToken;
		wordToken.addTokens(resultTokens[i], ":", "\"");
		if (wordToken.size() == 2) {
			if (wordToken[0].equalsIgnoreCase("newestversion")) {
				String newestversion = wordToken[1];
				String lVersionStr = ProjectInfo::versionString;
				StringArray versionToken;
				versionToken.addTokens(newestversion, ".", "\"");
				if (versionToken.size() == 3) {
					int newestVersionCount = versionToken[0].getIntValue() * 10000 + versionToken[1].getIntValue() * 100 + versionToken[2].getIntValue(); //all two digits
					versionToken.clear();
					versionToken.addTokens(lVersionStr, ".", "\"");
					if (versionToken.size() == 3) {
						int versionCount = versionToken[0].getIntValue() * 10000 + versionToken[1].getIntValue() * 100 + versionToken[2].getIntValue(); //all two digits
						if (versionCount < newestVersionCount) {
							VDBG("There is a newer version out!");
							m_showNewerVersionPopup = true;
							m_newerVersionThatIsAvailble = newestversion;
						}
					}
				}
			}
		}
	}
}

String VASTAudioProcessor::FloatArrayToString(float* fData, int numFloat)
{//Return String of multiple float values separated by commas 
	String result = "";
	if (numFloat<1)
		return result;
	for (int i = 0; i<(numFloat - 1); i++)
		result << String(fData[i]) << ",";//Use juce::String initializer for each value
	result << String(fData[numFloat - 1]);
	return result;
}

int VASTAudioProcessor::StringToFloatArray(String sFloatCSV, float* fData, int maxNumFloat)
{//Return is number of floats copied to the fData array
 //-1 if there were more in the string than maxNumFloat
	StringArray Tokenizer;
	int TokenCount = Tokenizer.addTokens(sFloatCSV, ",", "");
	int resultCount = (maxNumFloat <= TokenCount) ? maxNumFloat : TokenCount;
	for (int i = 0; i<resultCount; i++)//only go as far as resultCount for valid data
		fData[i] = Tokenizer[i].getFloatValue();//fill data using String class float conversion
	return ((TokenCount <= maxNumFloat) ? resultCount : -1);
}

String VASTAudioProcessor::StringArrayToString(String* sData, int numFloat)
{//Return String of multiple float values separated by commas 
	String result = "";
	if (numFloat<1)
		return result;
	for (int i = 0; i<(numFloat - 1); i++)
		result << String(sData[i]) << ",";//Use juce::String initializer for each value
	result << String(sData[numFloat - 1]);
	return result;
}

int VASTAudioProcessor::StringToStringArray(String sStringCSV, String* sData, int maxNumFloat)
{//Return is number of floats copied to the fData array
 //-1 if there were more in the string than maxNumFloat
	StringArray Tokenizer;
	int TokenCount = Tokenizer.addTokens(sStringCSV, ",", "");
	int resultCount = (maxNumFloat <= TokenCount) ? maxNumFloat : TokenCount;
	for (int i = 0; i<resultCount; i++)//only go as far as resultCount for valid data
		sData[i] = Tokenizer[i];//fill data using String class 
	return ((TokenCount <= maxNumFloat) ? resultCount : -1);
}

String VASTAudioProcessor::getLocalMachineID() {
	StringArray nums;

#if JUCE_WINDOWS
	// This file identifier stays the same for 32 and 64 bit applications.
	uint64 num = File::getSpecialLocation(File::userHomeDirectory).getFileIdentifier();
#elif JUCE_MAC
	uint64 num = File("~").getFileIdentifier();
#elif JUCE_LINUX
	uint64 num = File("~").getFileIdentifier();
#endif

	if (num != 0)
	{
		nums.add(getEncodedIDString(String::toHexString((int64)num)));
		return nums[0]; //only first?
	}

	// ..if that fails, use the MAC addresses..
	Array<MACAddress> addresses;
	MACAddress::findAllAddresses(addresses);

	for (int i = 0; i < addresses.size(); ++i)
		nums.add(getEncodedIDString(addresses[i].toString()));
	jassert(nums.size() > 0); // failed to create any IDs!
	return nums[0]; //only first?
}

const String VASTAudioProcessor::shiftHexEncryptString(const String& str)
{
    CharPointer_UTF8 cp = str.getCharPointer();
    String enc = String::toHexString(cp, int(cp.length()));
    enc = enc.removeCharacters(" ");
    CharPointer_UTF8 encbytes = enc.getCharPointer();
    String result = "";
    for (int i= 0; i< enc.length(); i++) {
		wchar_t wc = encbytes.getAndAdvance();
        wc += 3;
        result += String(&wc, 1);
    }
    //VDBG(result);
    return result;
}

String VASTAudioProcessor::getEncodedIDString(const String& input)
{
	auto platform = String::charToString(static_cast<juce_wchar> (getPlatformPrefix()));

	return platform + MD5((input + "salt_1" + platform).toUTF8())
		.toHexString().substring(0, 9).toUpperCase();
}

char VASTAudioProcessor::getPlatformPrefix()
{
#if JUCE_MAC
	return 'M';
#elif JUCE_WINDOWS
	return 'W';
#elif JUCE_LINUX
	return 'L';
#elif JUCE_IOS
	return 'I';
#elif JUCE_ANDROID
	return 'A';
#endif
}

String VASTAudioProcessor::getLicenseText() {
		return m_sLicenseString;
}

bool VASTAudioProcessor::isUserPatch() { return !m_presetData.getCurPatchData().isFactory; }

String VASTAudioProcessor::getUserPatchName() { return m_presetData.getCurPatchData().presetname; }


bool VASTAudioProcessor::isLicensed() {
	return m_sLicenseInformation.m_bIsLicensed;
}

// http://www.experts-exchange.com/Programming/Languages/CPP/Q_26823608.html
JUCE_BEGIN_IGNORE_WARNINGS_GCC_LIKE("-Wconversion")
JUCE_BEGIN_IGNORE_WARNINGS_MSVC(4244 4267)
std::string VASTAudioProcessor::XOREncrypt(std::string a_sValue, std::string a_sKey)
{
	std::string sRet;
	sRet = a_sValue;

	int idxVal = 0;
	int size_svalue = a_sValue.length();
	int idxKey = 0;
	int size_skey = a_sKey.length();

	while (idxVal < size_svalue)
	{
		sRet[idxVal] = sRet.at(idxVal) ^ a_sKey.at(idxKey);
		idxVal++;
		idxKey++;
		if (idxKey >= size_skey)
			idxKey = 0;
	}

	std::string  hexRepr;
	convertToASCIIhex(hexRepr, sRet);

	return hexRepr;
}

std::string  VASTAudioProcessor::XORDecrypt(std::string a_sValue, std::string  a_sKey)
{
	std::string  instring;
	convertASCIIhexToString(instring, a_sValue);

	std::string  sRet;
	sRet = instring;

	int idxVal = 0;
	int size_svalue = instring.length();
	int idxKey = 0;
	int size_skey = a_sKey.length();

	while (idxVal < size_svalue)
	{
		sRet[idxVal] = sRet.at(idxVal) ^ a_sKey.at(idxKey);
		idxVal++;
		idxKey++;
		if (idxKey >= size_skey)
			idxKey = 0;
	}

	return sRet;
}
JUCE_END_IGNORE_WARNINGS_MSVC
JUCE_END_IGNORE_WARNINGS_GCC_LIKE

void VASTAudioProcessor::convertASCIIhexToString(std::string & output, std::string & input) {
	const char* const lut = "0123456789ABCDEF";
	size_t len = input.length();
	if (len & 1) throw std::invalid_argument("odd length");

	string output_temp;
	string outstr;
	outstr.reserve(len / 2);
	for (size_t i = 0; i < len; i += 2)
	{
		char a = input[i];
		const char* p = std::lower_bound(lut, lut + 16, a);
		if (*p != a) throw std::invalid_argument("not a hex digit");

		char b = input[i + 1];
		const char* q = std::lower_bound(lut, lut + 16, b);
		if (*q != b) throw std::invalid_argument("not a hex digit");

		output_temp.push_back(((p - lut) << 4) | (q - lut));
	}
	output = output_temp;
}

void VASTAudioProcessor::convertToASCIIhex(std::string & output, std::string & input) {
	const char* const lut = "0123456789ABCDEF";
	size_t len = input.length();

	string output_temp;
	string outstr;
	outstr.reserve(2 * len);
	for (size_t i = 0; i < len; ++i)
	{
		const unsigned char c = input[i];
		output_temp.push_back(lut[c >> 4]);
		output_temp.push_back(lut[c & 15]);
	}
	output = output_temp;
}

//==============================================================================
// Midi Mapping
int VASTAudioProcessor::midiMappingGetParameterIndex(int iCC) {
	int lParamInternal = m_MidiMapping[iCC].paramID; //isParam?
	return lParamInternal;
}

String VASTAudioProcessor::midiMappingComponentVariableName(int iCC) {
	String varname= m_MidiMapping[iCC].componentVariableName; //isParam?
	return varname;
}

int VASTAudioProcessor::parameterIndexGetMidiMapping(int internalIndex) {
	int lCC = -1;
	for (int i = 0; i < 128; i++) {
		if (m_MidiMapping[i].paramID == internalIndex)
			return i;
	}
	return lCC;
}

void VASTAudioProcessor::initSettings() {
    bool lSuccess = readSettingsFromFile();

    if (lSuccess == false) {
        loadDefaultMidiMapping();
        
        //set default User Folders
        m_UserPresetRootFolder = File(getVSTPath()).getChildFile("Presets").getFullPathName(); //will be overwritten by settings if set
        m_UserWavetableRootFolder = File(getVSTPath()).getChildFile("Tables").getFullPathName(); // root folder for wavetables
        m_UserWavRootFolder = File(getVSTPath()).getChildFile("Noises").getFullPathName(); // root folder for WAV files

#ifdef JUCE_WINDOWS
        m_UserPresetRootFolder = File::getSpecialLocation(File::userDocumentsDirectory).getChildFile("Vaporizer2").getChildFile("Presets").getFullPathName();
        m_UserWavetableRootFolder = File::getSpecialLocation(File::userDocumentsDirectory).getChildFile("Vaporizer2").getChildFile("Tables").getFullPathName(); // root folder for wavetables
        m_UserWavRootFolder = File::getSpecialLocation(File::userDocumentsDirectory).getChildFile("Vaporizer2").getChildFile("Noises").getFullPathName(); // root folder for WAV files
#elif JUCE_MAC
        m_UserPresetRootFolder = File::getSpecialLocation(File::userDocumentsDirectory).getChildFile("Vaporizer2").getChildFile("Presets").getFullPathName();
        m_UserWavetableRootFolder = File::getSpecialLocation(File::userDocumentsDirectory).getChildFile("Vaporizer2").getChildFile("Tables").getFullPathName();
        m_UserWavRootFolder = File::getSpecialLocation(File::userDocumentsDirectory).getChildFile("Vaporizer2").getChildFile("Noises").getFullPathName();
#elif JUCE_LINUX
        m_UserPresetRootFolder = File::getSpecialLocation(File::userDocumentsDirectory).getChildFile("Vaporizer2").getChildFile("Presets").getFullPathName();
        m_UserWavetableRootFolder = File::getSpecialLocation(File::userDocumentsDirectory).getChildFile("Vaporizer2").getChildFile("Tables").getFullPathName();
        m_UserWavRootFolder = File::getSpecialLocation(File::userDocumentsDirectory).getChildFile("Vaporizer2").getChildFile("Noises").getFullPathName();
#endif

//store values passed from installer in settings
#ifdef JUCE_WINDOWS
        if (!(WindowsRegistry::getValue("HKEY_LOCAL_MACHINE\\SOFTWARE\\VAST Dynamics\\Vaporizer2\\Settings\\UserPresetFolder", m_UserPresetRootFolder).equalsIgnoreCase("")))
            m_UserPresetRootFolder = WindowsRegistry::getValue("HKEY_LOCAL_MACHINE\\SOFTWARE\\VAST Dynamics\\Vaporizer2\\Settings\\UserPresetFolder", m_UserPresetRootFolder);
        if (!(WindowsRegistry::getValue("HKEY_LOCAL_MACHINE\\SOFTWARE\\VAST Dynamics\\Vaporizer2\\Settings\\UserPresetFolder", m_UserWavetableRootFolder).equalsIgnoreCase("")))
            m_UserWavetableRootFolder = WindowsRegistry::getValue("HKEY_LOCAL_MACHINE\\SOFTWARE\\VAST Dynamics\\Vaporizer2\\Settings\\UserTableFolder", m_UserWavetableRootFolder);
        if (!(WindowsRegistry::getValue("HKEY_LOCAL_MACHINE\\SOFTWARE\\VAST Dynamics\\Vaporizer2\\Settings\\UserNoisesFolder", m_UserWavRootFolder).equalsIgnoreCase("")))
            m_UserWavRootFolder = WindowsRegistry::getValue("HKEY_LOCAL_MACHINE\\SOFTWARE\\VAST Dynamics\\Vaporizer2\\Settings\\UserNoisesFolder", m_UserWavRootFolder);
#endif
        m_iUserTargetPluginWidth = m_iDefaultPluginWidth; //default size from projucer
        m_iUserTargetPluginHeight = m_iDefaultPluginHeight; //default size from projucer

		m_ModWheelPermaLink = 1; //default now CustomModulator1

		m_MidiKeyboardCharLayout = "ysxdcvgbhnjmq2w3er5t6z7"; //FL Studio setup			
		m_iMidiKeyboardBaseOctave = 2; //FL Studio setup

        writeSettingsToFile();
    }
}

/*
https://steinbergmedia.github.io/vst3_dev_portal/pages/Technical+Documentation/Locations+Format/Preset+Locations.html
Folder structure:
**Windows:**
Program and plugin copies: \Program Files\Vaporizer2 (default - selectable in installer = [$APPFOLDER]) (read-only)
Settings: [Users/$USERNAME]\AppData\Roaming\Vaporizer2\VASTvaporizerSettings.xml [userApplicationDataDirectory] (write-enabled) (Installer needs to create folder and grant write access rights for everyone)
Factory Presets: [$APPFOLDER]\Presets (read-only, no choice of folder)
Factory Noises: [$APPFOLDER]\Noises (read-only, no choice of folder)
Factory Wavetables: [$APPFOLDER]\Tables (read-only, no choice of folder)
User Presets: [Users\$USERNAME\Documents]\Vaporizer2\Presets (write-enabled) [userDocumentsDirectory ]
User Noises: [Users\$USERNAME\Documents]\Vaporizer2\Noises (write-enabled, contains copy of factory [userDocumentsDirectory ]
User Tables: [Users\$USERNAME\Documents]\Vaporizer2\Tables (write-enabled, contains copy of factory) [userDocumentsDirectory ]

JUCE>> File::getSpecialLocation(File::userApplicationDataDirectory) = "[Users/$USERNAME]\AppData\Roaming"
JUCE>> File::getSpecialLocation(File::commonApplicationDataDirectory) ="C:\ProgramData"
JUCE>> File::getSpecialLocation(File::commonDocumentsDirectory ) ="C:\Users\Public\Documents"
JUCE>> File::getSpecialLocation(File::userDocumentsDirectory ) =[Users\$USERNAME\Documents]"

**macOS:**
Program and plugin copies:  /Applications/Vaporizer2 (default - selectable in installer = [$APPFOLDER]) (read-only)
Settings: ~/Library/Application Support/Vaporizer2/VASTvaporizerSettings.xml [userApplicationDataDirectory]/Application Support (write-enabled)
Factory Presets: [$APPFOLDER]/Presets (read-only)
Factory Noises: [$APPFOLDER]/Noises (read-only)
Factory Wavetables: [$APPFOLDER]/Tables (read-only)
User Presets: [Users/$USERNAME/Documents/Vaporizer2/Presets (write-enabled)
User Noises: [Users/$USERNAME/Documents/Vaporizer2/Noises (write-enabled, contains copy of factory)
User Tables: [Users/$USERNAME/Documents/Vaporizer2/Tables (write-enabled, contains copy of factory)

JUCE>> File::getSpecialLocation(File::userApplicationDataDirectory) = "~ /Library"
JUCE>> File::getSpecialLocation(File::commonApplicationDataDirectory) = "/Library"
JUCE>> File::getSpecialLocation(File::userDocumentsDirectory) = "~ /Documents"

**Linux:**
Program and plugin copies: /usr/share/Vaporizer2 (default - selectable in installer = [$APPFOLDER]) (read-only)
Settings: $HOME/.config/Vaporizer2/VASTvaporizerSettings.xml [userApplicationDataDirectory]/Vaporizer2 (write-enabled)
Factory Presets: [$APPFOLDER]/Presets (read-only)
Factory Noises: [$APPFOLDER]/Noises (read-only)
Factory Wavetables: [$APPFOLDER]/Tables (read-only)
User Presets: $HOME/.Documents/Vaporizer2/Presets (write-enabled) [userDocumentsDirectory]
User Noises: $HOME/Documents/Vaporizer2/Noises (write-enabled, contains copy of factory) [userDocumentsDirectory]
User Tables: $HOME/Documents/Vaporizer2/Tables (write-enabled, contains copy of factory) [userDocumentsDirectory]

JUCE>> File::getSpecialLocation(File::commonApplicationDataDirectory) = "/opt"
JUCE>> File::getSpecialLocation(File::userApplicationDataDirectory) =$HOME "~/.config"
JUCE> File::getSpecialLocation(File::userHomeDirectory)  =$HOME "~"
*/

void VASTAudioProcessor::writeSettingsToFileAsync() {
	Component::SafePointer<Label> spl(&safePointerLabel);
    VASTAudioProcessor* processor_ = this;
		Timer::callAfterDelay(50, [spl, processor_, this] {
			if (spl != nullptr)
				writeSettingsToFile();
		});
}

bool VASTAudioProcessor::writeSettingsToFile() {
	//readable version 20.2.15
	XmlElement root("VASTVaporizerSettingsV1.000");
	XmlElement* settings = root.createNewChildElement("Settings");
	settings->setAttribute("PresetRootFolder", m_UserPresetRootFolder);
	settings->setAttribute("WavetableRootFolder", m_UserWavetableRootFolder);
	settings->setAttribute("WavRootFolder", m_UserWavRootFolder);
	settings->setAttribute("TuningFile", m_UserTuningFile);
	settings->setAttribute("ModWheelPermaLink", String(m_ModWheelPermaLink));

	settings->setAttribute("MidiKeyboardCharLayout", String(m_MidiKeyboardCharLayout));
	settings->setAttribute("MidiKeyboardBaseOctave", String(m_iMidiKeyboardBaseOctave));

	settings->setAttribute("PluginWidth", String(m_iUserTargetPluginWidth));
	settings->setAttribute("PluginHeight", String(m_iUserTargetPluginHeight));

	settings->setAttribute("Skin", String(m_activeLookAndFeel));
	settings->setAttribute("UIFontSize", String(getUIFontSize()));
	settings->setAttribute("WavetableMode", String(getWTmode()));
	settings->setAttribute("DisableOpenGLGFX", (m_disableOpenGLGFX == true ? "X" : ""));
	settings->setAttribute("MPEMode", String(m_MPEmode));

	settings->setAttribute("BendRange", String(getBendRange()));

	settings->setAttribute("PresetBrowserSortColumn", getCurrentVASTLookAndFeel()->presetTableSortColumn);
	settings->setAttribute("PresetBrowserSortForward", (getCurrentVASTLookAndFeel()->presetTableSortColumnForward == true ? "X" : ""));

	settings->setAttribute("TogglePerspectiveDisplay_Osc1", (m_bTogglePerspectiveDisplay[0] == true ? "X" : ""));
	settings->setAttribute("TogglePerspectiveDisplay_Osc2", (m_bTogglePerspectiveDisplay[1] == true ? "X" : ""));
	settings->setAttribute("TogglePerspectiveDisplay_Osc3", (m_bTogglePerspectiveDisplay[2] == true ? "X" : ""));
	settings->setAttribute("TogglePerspectiveDisplay_Osc4", (m_bTogglePerspectiveDisplay[3] == true ? "X" : ""));

	settings->setAttribute("WTEditorDrawMode", String(getDrawMode()));
	settings->setAttribute("WTEditorGridMode", String(getGridMode()));
	settings->setAttribute("WTEditorBinMode", String(getBinMode()));
	settings->setAttribute("WTEditorBinEditMode", String(getBinEditMode()));

	XmlElement* presetdata = root.createNewChildElement("PresetData");
	if (m_presetData.m_favorites.size() > 0) {
		XmlElement* favorites = presetdata->createNewChildElement("Favorites");

		for (std::unordered_multimap<String, int>::iterator it = m_presetData.m_favorites.begin(); it != m_presetData.m_favorites.end(); ++it) {
			XmlElement* favoritesitems = favorites->createNewChildElement("Favorites");
			favoritesitems->setAttribute("PresetID", it->first);
			favoritesitems->setAttribute("Favorite", it->second);
		}
	}

	if (m_presetData.m_stars.size() > 0) {
		XmlElement* stars = presetdata->createNewChildElement("Stars");
		for (std::map<String, int>::iterator it = m_presetData.m_stars.begin(); it != m_presetData.m_stars.end(); ++it) {
			if (it->second > 0) {
				XmlElement* starsitem = stars->createNewChildElement("Stars");
				starsitem->setAttribute("PresetID", it->first);
				starsitem->setAttribute("Ranking", it->second);
			}
		}
	}

	XmlElement* mapping = root.createNewChildElement("MIDIMapping");
	XmlElement* controller = mapping->createNewChildElement("ControllerCC");
	for (int i = 0; i < 128; i++) {
        std::unique_ptr<String> ccStr(new String("CC" + String(i)));
        std::unique_ptr<String> paramString;
		if (m_MidiMapping[i].paramID >= 0) {
			if (m_MidiMapping[i].paramID == 9999) //UI only
				paramString = std::make_unique<String>(m_MidiMapping[i].componentVariableName);
			else {
				AudioProcessorParameterWithID* param = (AudioProcessorParameterWithID*)getParameters()[m_MidiMapping[i].paramID];
				paramString = std::make_unique<String>(param->paramID);
			}
		}
		else
			paramString = std::make_unique<String>("---");
		controller->setAttribute(*ccStr.get(), *paramString);
	}
	XmlElement* bankA = mapping->createNewChildElement("ProgramChangeBankA");
	StringArray pcd = m_presetData.getProgramChangeData(0);
	for (int i = 0; i < pcd.size(); i++) {
        std::unique_ptr<String> ccStr(new String("Prog" + String(i)));
        std::unique_ptr<String> paramString;
		paramString = std::make_unique<String>(pcd[i]);
		bankA->setAttribute(*ccStr.get(), *paramString);
	}
	XmlElement* bankB = mapping->createNewChildElement("ProgramChangeBankB");
	pcd = m_presetData.getProgramChangeData(1);
	for (int i = 0; i < pcd.size(); i++) {
        std::unique_ptr<String> ccStr(new String("Prog" + String(i)));
        std::unique_ptr<String> paramString;
		paramString = std::make_unique<String>(pcd[i]);
		bankB->setAttribute(*ccStr.get(), *paramString);
	}
	XmlElement* bankC = mapping->createNewChildElement("ProgramChangeBankC");
	pcd = m_presetData.getProgramChangeData(2);
	for (int i = 0; i < pcd.size(); i++) {
        std::unique_ptr<String> ccStr(new String("Prog" + String(i)));
        std::unique_ptr<String> paramString;
		paramString = std::make_unique<String>(pcd[i]);
		bankC->setAttribute(*ccStr.get(), *paramString);
	}
	XmlElement* bankD = mapping->createNewChildElement("ProgramChangeBankD");
	pcd = m_presetData.getProgramChangeData(3);
	for (int i = 0; i < pcd.size(); i++) {
        std::unique_ptr<String> ccStr(new String("Prog" + String(i)));
        std::unique_ptr<String> paramString;
		paramString = std::make_unique<String>(pcd[i]);
		bankD->setAttribute(*ccStr.get(), *paramString);
	}

	//String myXmlDoc = root.createDocument(String()); //empty is deprecated
	String myXmlDoc = root.toString();

	//check if user folders have to be created
	if (!File(m_UserPresetRootFolder).exists())
		(File(m_UserPresetRootFolder).createDirectory()); //recursively create also directories			
/*
	if (!File(m_UserPresetRootFolder).getChildFile("Factory").isSymbolicLink())
		if (!File(getVSTPath()).getChildFile("Presets").getFullPathName().equalsIgnoreCase(m_UserPresetRootFolder) &&
			File(getVSTPath()).getChildFile("Presets").exists()) {
			VDBG("Create Symlink to " + m_UserPresetRootFolder);
			if (!File(getVSTPath()).getChildFile("Presets").createSymbolicLink(File(m_UserPresetRootFolder).getChildFile("Factory"), true)) //add symlink to Factory
				setErrorState(vastErrorState::errorState15_couldNotCreateSymlink);
		}
		*/
	if (!File(m_UserWavetableRootFolder).exists())
		(File(m_UserWavetableRootFolder).createDirectory()); //recursively create also directories
/*
	if (!File(m_UserWavetableRootFolder).getChildFile("Factory").isSymbolicLink())
		if (!File(getVSTPath()).getChildFile("Tables").getFullPathName().equalsIgnoreCase(m_UserWavetableRootFolder) &&
			File(getVSTPath()).getChildFile("Tables").exists()) {
			VDBG("Create Symlink to " + m_UserWavetableRootFolder);
			if (!File(getVSTPath()).getChildFile("Tables").createSymbolicLink(File(m_UserWavetableRootFolder).getChildFile("Factory"), true)) //add symlink to Factory
				setErrorState(vastErrorState::errorState15_couldNotCreateSymlink);
		}
		*/
	if (!File(m_UserWavRootFolder).exists())
		(File(m_UserWavRootFolder).createDirectory()); //recursively create also directories
/*
	if (!File(m_UserWavRootFolder).getChildFile("Factory").isSymbolicLink())
		if (!File(getVSTPath()).getChildFile("Noises").getFullPathName().equalsIgnoreCase(m_UserWavRootFolder) &&
			File(getVSTPath()).getChildFile("Noises").exists()) {
			VDBG("Create Symlink to " + m_UserWavRootFolder);
			if (!File(getVSTPath()).getChildFile("Noises").createSymbolicLink(File(m_UserWavRootFolder).getChildFile("Factory"), true)) //add symlink to Factory		
				setErrorState(vastErrorState::errorState15_couldNotCreateSymlink);
		}
		*/
	bool migrate_legacy = false;
	String filename = getSettingsFilePath(false, migrate_legacy); //write always new path

	File file(filename);
	file.deleteFile();
	FileOutputStream out(file);
	if (out.failedToOpen()) {
		cerr << "open failure: " << strerror(errno) << '\n';
		setErrorState(vastErrorState::errorState12_writeSettingsToFileFailed);
		assert(true);
		return false;
	}
	out.writeText(myXmlDoc, false, false, "\n");  //slash n new?
	out.flush();
	root.deleteAllChildElements();
	VDBG("Wrote settings to file.");
	return true;
}

String VASTAudioProcessor::getSettingsFilePath(bool read, bool &migrate_legacy) {
	//JUCE_WINDOWS	File::getSpecialLocation(File::userApplicationDataDirectory) ="C:\Users\<username>\AppData\Roaming\"
	//JUCE_MAC		File::getSpecialLocation(File::userApplicationDataDirectory) ="~/Library/Application Support/Vaporizer2"
	//JUCE_LINUX	File::getSpecialLocation(File::userApplicationDataDirectory) ="~./config/Vaporizer2"
	const String settingsFile = "VASTvaporizerSettings.xml";
	String filename = File::getSpecialLocation(File::userApplicationDataDirectory).getChildFile("Vaporizer2").getChildFile(settingsFile).getFullPathName();

#ifdef JUCE_WINDOWS
	filename = File::getSpecialLocation(File::userApplicationDataDirectory).getChildFile("Vaporizer2").getChildFile(settingsFile).getFullPathName();
#elif JUCE_MAC
	filename = File::getSpecialLocation(File::userApplicationDataDirectory).getChildFile("Application Support").getChildFile("Vaporizer2").getChildFile(settingsFile).getFullPathName();
#elif JUCE_LINUX
	filename = File::getSpecialLocation(File::userApplicationDataDirectory).getChildFile("Vaporizer2").getChildFile(settingsFile).getFullPathName();
#endif

	if (read) {
		if (!File(filename).existsAsFile()) {
			File(filename).create(); //recursively create also directories
			File(filename).deleteFile();
			filename = File(getVSTPath()).getChildFile(settingsFile).getFullPathName(); //try compatibility
			migrate_legacy = true;
		}
	}
	return filename;
}

bool VASTAudioProcessor::readSettingsFromFile() {
	loadZeroMidiMapping();

	string line = "";
	String file_enc = "";

	bool migrate_legacy = false;
	String filename = getSettingsFilePath(true, migrate_legacy);

	ifstream mappingFile;
	const char* fname = filename.toRawUTF8();
	mappingFile.open(fname);
	//ifstream licesefile(filename.toStdString()); //due to OSX 10.6 compatibility
	if (mappingFile.is_open())
	{
		while (std::getline(mappingFile, line))
		{
			file_enc << line;
		}
		mappingFile.close();
	}
	else return false;

	//parse XML
	XmlDocument myDocument(file_enc);
	std::unique_ptr<XmlElement> xml = myDocument.getDocumentElement();

	if (xml != nullptr) {
		if (xml->getTagName() == "VASTVaporizerSettingsV1.000") { 	//generic version 20.2.15
			if (xml != NULL) {
				for (auto* (pChild) : ((*xml).getChildIterator())) {
					if (pChild->hasTagName("Settings"))
					{
						for (int i = 0; i < pChild->getNumAttributes(); i++) {
							if (pChild->getAttributeName(i).equalsIgnoreCase("PresetRootFolder") == true) {
								m_UserPresetRootFolder = pChild->getAttributeValue(i);
							}
							else if (pChild->getAttributeName(i).equalsIgnoreCase("WavetableRootFolder") == true) {
								m_UserWavetableRootFolder = pChild->getAttributeValue(i);
							}
							else if (pChild->getAttributeName(i).equalsIgnoreCase("WavRootFolder") == true) {
								m_UserWavRootFolder = pChild->getAttributeValue(i);
							}
							else if (pChild->getAttributeName(i).equalsIgnoreCase("TuningFile") == true) {
								m_UserTuningFile = pChild->getAttributeValue(i);
								setUserTuningFile(m_UserTuningFile);
							}										
							else if (pChild->getAttributeName(i).equalsIgnoreCase("ModWheelPermaLink") == true) {
								m_ModWheelPermaLink = pChild->getAttributeValue(i).getIntValue();
							}
							else if (pChild->getAttributeName(i).equalsIgnoreCase("MidiKeyboardCharLayout") == true) {
								m_MidiKeyboardCharLayout = pChild->getAttributeValue(i);
								setMidiKeyboardCharLayout(m_MidiKeyboardCharLayout);
							}
							else if (pChild->getAttributeName(i).equalsIgnoreCase("MidiKeyboardBaseOctave") == true) {
								m_iMidiKeyboardBaseOctave = pChild->getAttributeValue(i).getIntValue();
								setMidiKeyboardBaseOctave(m_iMidiKeyboardBaseOctave);
							}
							else if (pChild->getAttributeName(i).equalsIgnoreCase("PluginWidth") == true) {
								m_iUserTargetPluginWidth = pChild->getAttributeValue(i).getIntValue();
							}
							else if (pChild->getAttributeName(i).equalsIgnoreCase("PluginHeight") == true) {
								m_iUserTargetPluginHeight = pChild->getAttributeValue(i).getIntValue();
							}
							else if (pChild->getAttributeName(i).equalsIgnoreCase("Skin") == true) {
								if (pChild->getAttributeValue(i).getIntValue() != m_activeLookAndFeel) { //changed
									m_activeLookAndFeel = pChild->getAttributeValue(i).getIntValue();
								}
							}
							else if (pChild->getAttributeName(i).equalsIgnoreCase("UIFontSize") == true) {
								int size = pChild->getAttributeValue(i).getIntValue();
								setUIFontSize(size);
							}							
							else if (pChild->getAttributeName(i).equalsIgnoreCase("WavetableMode") == true) {
								int mode = pChild->getAttributeValue(i).getIntValue();
								setWTmode(mode);
							}
							else if (pChild->getAttributeName(i).equalsIgnoreCase("DisableOpenGLGFX") == true) {
								m_disableOpenGLGFX = (pChild->getAttributeValue(i) == "X" ? true : false);
							}
							else if (pChild->getAttributeName(i).equalsIgnoreCase("MPEMode") == true) {
								int mode = pChild->getAttributeValue(i).getIntValue();
								m_MPEmode = mode;
							}
							else if (pChild->getAttributeName(i).equalsIgnoreCase("BendRange") == true) {
								int bendRange = pChild->getAttributeValue(i).getIntValue();
								setBendRange(bendRange);
							}
							else if (pChild->getAttributeName(i).equalsIgnoreCase("PresetBrowserSortColumn") == true) {
								getCurrentVASTLookAndFeel()->presetTableSortColumn = pChild->getAttributeValue(i).getIntValue();
							}
							else if (pChild->getAttributeName(i).equalsIgnoreCase("PresetBrowserSortForward") == true) {
								getCurrentVASTLookAndFeel()->presetTableSortColumnForward = (pChild->getAttributeValue(i) == "X" ? true : false);
							}
							else if (pChild->getAttributeName(i).equalsIgnoreCase("TogglePerspectiveDisplay_Osc1") == true) {
								m_bTogglePerspectiveDisplay[0] = (pChild->getAttributeValue(i) == "X" ? true : false);
							}
							else if (pChild->getAttributeName(i).equalsIgnoreCase("TogglePerspectiveDisplay_Osc2") == true) {
								m_bTogglePerspectiveDisplay[1] = (pChild->getAttributeValue(i) == "X" ? true : false);
							}
							else if (pChild->getAttributeName(i).equalsIgnoreCase("TogglePerspectiveDisplay_Osc3") == true) {
								m_bTogglePerspectiveDisplay[2] = (pChild->getAttributeValue(i) == "X" ? true : false);
							}
							else if (pChild->getAttributeName(i).equalsIgnoreCase("TogglePerspectiveDisplay_Osc4") == true) {
								m_bTogglePerspectiveDisplay[3] = (pChild->getAttributeValue(i) == "X" ? true : false);
							}

							else if (pChild->getAttributeName(i).equalsIgnoreCase("WTEditorDrawMode") == true) {
								m_iWTEditorDrawMode = pChild->getAttributeValue(i).getIntValue();
								m_iWTEditorDrawMode = jlimit<int>(0, OscillatorEditMode::OscillatorEditMode_NUM - 1, m_iWTEditorDrawMode); //safety OscillatorEditMode
							}
							else if (pChild->getAttributeName(i).equalsIgnoreCase("WTEditorGridMode") == true) {
								m_iWTEditorGridMode = pChild->getAttributeValue(i).getIntValue();
								m_iWTEditorGridMode = jlimit<int>(0, OscillatorGridMode::OscillatorGridMode_NUM - 1, m_iWTEditorGridMode); //safety OscillatorGridMode
							}
							else if (pChild->getAttributeName(i).equalsIgnoreCase("WTEditorBinMode") == true) {
								m_iWTEditorBinMode = pChild->getAttributeValue(i).getIntValue();
								m_iWTEditorBinMode = jlimit<int>(0, BinMode::BinMode_NUM - 1, m_iWTEditorBinMode); //safety BinMode
							}
							else if (pChild->getAttributeName(i).equalsIgnoreCase("WTEditorBinEditMode") == true) {
								m_iWTEditorBinEditMode = pChild->getAttributeValue(i).getIntValue(); 
								m_iWTEditorBinEditMode = jlimit<int>(0, FreqEditMode::FreqEditMode_NUM - 1, m_iWTEditorBinEditMode); //safety FreqEditMode
							}
						}
					}
					if (pChild->hasTagName("PresetData")) {
						m_presetData.m_favorites.clear();
						m_presetData.m_stars.clear();
						for (auto* (pPresetData) : ((*pChild).getChildIterator())) {
							if (pPresetData->hasTagName("Favorites")) {
								for (auto* (pPresetFavorites) : ((*pPresetData).getChildIterator())) {
									String internalid = "";
									String favoriteNo = "";
									if (pPresetFavorites->getAttributeName(0) == "PresetID")
										internalid = pPresetFavorites->getAttributeValue(0);
									if (pPresetFavorites->getAttributeName(1) == "PresetID")
										internalid = pPresetFavorites->getAttributeValue(1);
									if (pPresetFavorites->getAttributeName(0) == "Favorite")
										favoriteNo = pPresetFavorites->getAttributeValue(0);
									if (pPresetFavorites->getAttributeName(1) == "Favorite")
										favoriteNo = pPresetFavorites->getAttributeValue(1);
									int i = favoriteNo.getIntValue();
									m_presetData.setFavorite(internalid, i);
								}
							}
							if (pPresetData->hasTagName("Stars")) {
								for (auto* (pPresetStars) : ((*pPresetData).getChildIterator())) {
									String internalid = "";
									String ranking = "";
									if (pPresetStars->getAttributeName(0) == "PresetID")
										internalid = pPresetStars->getAttributeValue(0);
									if (pPresetStars->getAttributeName(1) == "PresetID")
										internalid = pPresetStars->getAttributeValue(1);
									if (pPresetStars->getAttributeName(0) == "Ranking")
										ranking = pPresetStars->getAttributeValue(0);
									if (pPresetStars->getAttributeName(1) == "Ranking")
										ranking = pPresetStars->getAttributeValue(1);
									int i = ranking.getIntValue();
									m_presetData.setStars(internalid, i);
								}
							}
						}
						m_presetData.calcNumFavorites();
					}				
					if (pChild->hasTagName("MIDIMapping"))
					{
						for (auto* (pMIDIMapping) : ((*pChild).getChildIterator())) {
							if (pMIDIMapping->hasTagName("ControllerCC"))
							{
								for (int i = 0; i < pMIDIMapping->getNumAttributes(); i++) {
									int lCC = -1;
                                    std::unique_ptr<String> attName(new String(pMIDIMapping->getAttributeName(i)));
									lCC = attName->fromLastOccurrenceOf("CC", false, false).getIntValue();
                                    std::unique_ptr<String> attValue(new String(pMIDIMapping->getAttributeValue(i)));
									if (!(attValue->equalsIgnoreCase("---"))) {
										auto param = m_parameterState.getParameter(StringRef(*attValue.get()));
										if (param != nullptr) {
											int paramIndex = param->getParameterIndex();
											if ((paramIndex >= 0) && (lCC >= 0)) {
												m_MidiMapping[lCC].paramID = paramIndex;
											}
											else {
												jassert(false); //ignore - unused param in midi mapping
											}
										}
									}
								}
							}
							if (pMIDIMapping->hasTagName("ProgramChangeBankA")) {
								StringArray pcd;
								for (int j = 0; j < 128; j++) {
									bool found = false;
									for (int i = 0; i < pMIDIMapping->getNumAttributes(); i++) {
                                        std::unique_ptr<String> attName(new String(pMIDIMapping->getAttributeName(i)));
                                        if (i == j) {
											pcd.add(pMIDIMapping->getAttributeValue(i));
											found = true;
										}
									}
									if (!found)
										break;
								}
								m_presetData.setProgramChangeArray(0, pcd);
							}
							if (pMIDIMapping->hasTagName("ProgramChangeBankB")) {
								StringArray pcd;
								for (int j = 0; j < 128; j++) {
									bool found = false;
									for (int i = 0; i < pMIDIMapping->getNumAttributes(); i++) {
                                        std::unique_ptr<String> attName(new String(pMIDIMapping->getAttributeName(i)));
                                    	if (i == j) {
											pcd.add(pMIDIMapping->getAttributeValue(i));
											found = true;
										}
									}
									if (!found)
										break;
								}
								m_presetData.setProgramChangeArray(1, pcd);
							}
							if (pMIDIMapping->hasTagName("ProgramChangeBankC")) {
								StringArray pcd;
								for (int j = 0; j < 128; j++) {
									bool found = false;
									for (int i = 0; i < pMIDIMapping->getNumAttributes(); i++) {
                                        std::unique_ptr<String> attName(new String(pMIDIMapping->getAttributeName(i)));
                                        if (i == j) {
											pcd.add(pMIDIMapping->getAttributeValue(i));
											found = true;
										}
									}
									if (!found)
										break;
								}
								m_presetData.setProgramChangeArray(2, pcd);
							}
							if (pMIDIMapping->hasTagName("ProgramChangeBankD")) {
								StringArray pcd;
								for (int j = 0; j < 128; j++) {
									bool found = false;
									for (int i = 0; i < pMIDIMapping->getNumAttributes(); i++) {
                                        std::unique_ptr<String> attName(new String(pMIDIMapping->getAttributeName(i)));
                                        if (i == j) {
											pcd.add(pMIDIMapping->getAttributeValue(i));
											found = true;
										}
									}
									if (!found)
										break;
								}
								m_presetData.setProgramChangeArray(3, pcd);
							}

						}
					}
				}
			}
		}
		else {
			jassert(false); // unknown patch format
			return false;
		}
		xml->deleteAllChildElements();
	}
	else {
		String error = myDocument.getLastParseError();
		setErrorState(vastErrorState::errorState13_readSettingsFromFileFailed);
		jassert(false); // broken XML?
		return false;
	}

	m_iUserTargetPluginWidth = m_iUserTargetPluginHeight * m_dPluginRatio;

	if (migrate_legacy)
		writeSettingsToFileAsync();

	VDBG("Loaded settings from file.");
	return true;
}

void VASTAudioProcessor::initLookAndFeels() {
	vastLookAndFeels.add(new VASTLookAndFeelThemeDefault(m_pVASTXperience.m_Set, this));
	vastLookAndFeels.add(new VASTLookAndFeelThemeIce(m_pVASTXperience.m_Set, this));
	vastLookAndFeels.add(new VASTLookAndFeelThemeTech(m_pVASTXperience.m_Set, this));
	vastLookAndFeels.add(new VASTLookAndFeelThemeDark(m_pVASTXperience.m_Set, this));
}

VASTLookAndFeel* VASTAudioProcessor::getCurrentVASTLookAndFeel() {
	VASTLookAndFeel* lf = vastLookAndFeels[m_activeLookAndFeel];
	vassert(lf != nullptr);
	return lf;
}

void VASTAudioProcessor::loadDefaultMidiMapping() {
	typedef struct
	{
		int midiCC;
		const std::string variableName;
	} MIDI_MAPPING;

	MIDI_MAPPING lMapping[] = {
		//Master
		{
			5,
			"m_iPortamento"
		},
		{
			111,
			"m_fMasterVolumedB"
		}
		/*
		,
		//Oscillators
		{
			53,
			"m_fOscDetune"
		},
		{
			54,
			"m_fSpread"
		},
		{
			106,
			"m_fOscAGain"
		},
		{
			107,
			"m_fOscBGain"
		},
		{
			108,
			"m_fOscCGain"
		},
		{
			109,
			"m_fOscDGain"
		},
		{
			110,
			"m_fNoiseGain"
		},

		//VCA Envelope
		{
			73,
			"m_fAttackTime"
		},
		{
			36,
			"m_fDecayTime"
		},
		{
			37,
			"m_fSustainLevel"
		},
		{
			72,
			"m_fReleaseTime"
		},

		//Filter
		{
			38,
			"m_fAttackTimeVCF"
		},
		{
			39,
			"m_fDecayTimeVCF"
		},
		{
			40,
			"m_fSustainLevelVCF"
		},
		{
			41,
			"m_fReleaseTimeVCF"
		},

		{
			74,
			"m_fFilterCutoffLPF"
		},
		{
			42,
			"m_fFilterResoLPF"
		},
		{
			43,
			"m_fFilterCutoffHPF"
		},
		{
			44,
			"m_fFilterResoHPF"
		},
		{
			45,
			"m_fFilterCutoffVADL"
		},
		{
			46,
			"m_fFilterResoVADL"
		},

		//LFO
		{
			78,
			"m_fLFOFreq"
		},
		{
			79,
			"m_fLFODepth"
		},
		{
			80,
			"m_fLFO2Freq"
		},


		//Effects
		{
			81,
			"m_iDistortionOnOff"
		},
		{
			82,
			"m_fDistDryWet"
		},
		{
			83,
			"m_fDistFuzz"
		},
		{
			84,
			"m_fDistDrive"
		},
		{
			85,
			"m_uChorusOnOff"
		},
		{
			86,
			"m_fChorusDryWet"
		},
		{
			87,
			"m_fChorusDepth"
		},
		{
			88,
			"m_fChorusRate_hz"
		},
		{
			89,
			"m_iDelayOnOff"
		},
		{
			90,
			"m_fDelayWet"
		},
		{
			91,
			"m_fDelayTime"
		},
		{
			92,
			"m_fFeedback_Pct"
		},
		{
			93,
			"m_iReverbOnOff"
		},
		{
			94,
			"m_fWet_pct"
		},
		{
			95,
			"m_fReverbDamping"
		},
		{
			96,
			"m_fRT60"
		},


		//Modulation Matrix
		{
			112,
			"m_fModMatVal1"
		},
		{
			113,
			"m_fModMatVal2"
		},
		{
			114,
			"m_fModMatVal3"
		},
		{
			115,
			"m_fModMatVal4"
		},
		{
			116,
			"m_fModMatVal5"
		},
		{
			117,
			"m_fModMatVal6"
		},
		{
			118,
			"m_fModMatVal7"
		},
		{
			119,
			"m_fModMatVal8"
		}
		*/
	};

	for (int i = 0; i < 128; i++) {
		m_MidiMapping[i].paramID = -1;
	}
	int lSize = sizeof(lMapping) / sizeof(lMapping[0]); //size of array
	for (int i = 0; i < lSize; i++) {
        std::unique_ptr<String> variableName(new String(lMapping[i].variableName));
		for (int j = 0; j < getParameters().size(); j++) {
			AudioProcessorParameterWithID* param = (AudioProcessorParameterWithID*)getParameters()[j];			
			if (param->paramID.equalsIgnoreCase(*variableName.get())) {
				m_MidiMapping[lMapping[i].midiCC].paramID = j;
			}
		}
	}

#ifdef _DEBUG
	bool lWriteAsDocumentation = false;
	if (lWriteAsDocumentation == true) {
		String filename = File(getVSTPath()).getChildFile("VASTvaporizerMapDoc.txt").getFullPathName();
		File file(filename);
		file.deleteFile();
		FileOutputStream out(file);
		if (out.failedToOpen()) {
			cerr << "open failure: " << strerror(errno) << '\n';
			setErrorState(vastErrorState::errorState14_loadDefaultMidiMappingFailed);
			assert(true);
		}
		for (int i = 0; i < lSize; i++) {
            std::unique_ptr<String> variableName(new String(lMapping[i].variableName));
			for (int j = 0; j < getParameters().size(); j++) {
				AudioProcessorParameterWithID* param = (AudioProcessorParameterWithID*)getParameters()[j];
				if (param->paramID.equalsIgnoreCase(*variableName.get())) {
                    std::unique_ptr<String> outLine(new String(String(lMapping[i].midiCC) + "," + param->paramID + "," + lMapping[i].variableName + "\n"));
					out.writeText(*outLine.get(), false, false, "\n");
				}
			}
		}
		out.flush();
	}
#endif
}

void VASTAudioProcessor::loadZeroMidiMapping() {
	for (int i = 0; i < 128; i++) {
		m_MidiMapping[i].isParam = false;
		m_MidiMapping[i].paramID = -1;
		m_MidiMapping[i].componentVariableName = "";
	}
}

void VASTAudioProcessor::midiLearnParameter(int internalIndex, String compID) {
	m_iNowLearningMidiParameter = internalIndex;
	m_iNowLearningMidiParameterVariableName = compID;
}

void VASTAudioProcessor::midiForgetParameter(String componentName) { 
	for (int i = 0; i < 128; i++) {
		if (m_MidiMapping[i].componentVariableName == componentName)
			m_MidiMapping[i].paramID = -1;
	}
	m_iNowLearningMidiParameter = -1;
	m_iNowLearningMidiParameterVariableName = String(); //remains to update slider display in editor
	requestUIUpdate();
	writeSettingsToFileAsync();
}

void VASTAudioProcessor::midiParameterLearned(int iCC) {
	m_MidiMapping[iCC].isParam = m_iNowLearningMidiParameter != 9999;
	m_MidiMapping[iCC].paramID = m_iNowLearningMidiParameter;
	m_MidiMapping[iCC].componentVariableName = m_iNowLearningMidiParameterVariableName;
	m_iNowLearningMidiParameter = -1;
	m_iNowLearningMidiParameterVariableName = String(); //remains to update slider display in editor
	requestUIUpdate();
	writeSettingsToFileAsync();
}

bool VASTAudioProcessor::isEditorCurrentlyProbablyVisible()
{
	return m_editorIsProbablyVisible.load();
}

void VASTAudioProcessor::registerComponentValueUpdate(String uiComponentName, float lValue)
{
	m_bShallComponentValueUpdate.store(true);
	m_shallComponentUpdate = uiComponentName;
	m_shallComponentUpdateValue.store(lValue);
}

void VASTAudioProcessor::crashHandler(void*) {
	String filename = File(getVSTPath()).getChildFile("crashreport.log").getFullPathName();
	File crashFile = File(filename);
	crashFile.deleteFile();
	FileOutputStream fos(crashFile);
	fos << "VASTVaporizer2 Crash File" << newLine;
	fos << "------------------------------------------------------" << newLine;
	//fos.writeText("Version: " + m_versionString, true, true, "\n");
	fos << "ComputerName: " << SystemStats::getComputerName() << newLine;	
	fos << "JuceVersion: " << SystemStats::getJUCEVersion() << newLine;
	fos << "OperatingSystemType: " << SystemStats::getOperatingSystemType() << newLine;
	fos << "OperatingSystemName: " << SystemStats::getOperatingSystemName() << newLine;
	fos << "OperatingSystem64Bit: " << (SystemStats::isOperatingSystem64Bit() ? "YES" : "No") << newLine;
	fos << "CpuModel: " << SystemStats::getCpuModel() << newLine;
	fos << "CpuSpeedInMegaherz: " << String(SystemStats::getCpuSpeedInMegahertz()) << newLine;
	fos << "NumCpus: " << String(SystemStats::getNumCpus()) << newLine;
	fos << "NumPhysicalCpus: " << String(SystemStats::getNumPhysicalCpus()) << newLine;
	fos << "SSE2: " << (SystemStats::hasSSE2() ? "YES" : "No") << newLine;
	fos << "SSE3: " << (SystemStats::hasSSE3() ? "YES" : "No") << newLine;
	fos << "AVX: " << (SystemStats::hasAVX() ? "YES" : "No") << newLine;
	fos << "AVX2: " << (SystemStats::hasAVX2() ? "YES" : "No") << newLine;
	fos << "------------------------------------------------------" << newLine;
	fos << SystemStats::getStackBacktrace() << newLine;
	fos << "------------------------------------------------------" << newLine;
	//fos << "Signum: " << String(signum) << newLine;

	/*
	HANDLE hFile = &crashFile;

		MINIDUMP_EXCEPTION_INFORMATION exceptionInformation;
		LPEXCEPTION_POINTERS exceptionPointers = nullptr;
		exceptionInformation.ThreadId = GetCurrentThreadId();
		//exceptionInformation.ExceptionPointers = exceptionPointers;
		exceptionInformation.ClientPointers = FALSE;

		MINIDUMP_TYPE dumpType = MiniDumpNormal;

		BOOL dumpWriteResult = MiniDumpWriteDump(GetCurrentProcess(),
			GetCurrentProcessId(),
			hFile,
			dumpType,
			exceptionPointers != nullptr ? &exceptionInformation : 0,
			nullptr,
			nullptr);
			*/


	/*
	//https://stackoverflow.com/questions/28099965/windows-c-how-can-i-get-a-useful-stack-trace-from-a-signal-handler

	HANDLE process = GetCurrentProcess();
	SymInitialize(process, NULL, TRUE);

	// StackWalk64() may modify context record passed to it, so we will
	// use a copy.
	CONTEXT context_record = *pExceptionInfo->ContextRecord;
	// Initialize stack walking.
	STACKFRAME64 stack_frame;
	memset(&stack_frame, 0, sizeof(stack_frame));
#if defined(_WIN64)
	int machine_type = IMAGE_FILE_MACHINE_AMD64;
	stack_frame.AddrPC.Offset = context_record.Rip;
	stack_frame.AddrFrame.Offset = context_record.Rbp;
	stack_frame.AddrStack.Offset = context_record.Rsp;
#else
	int machine_type = IMAGE_FILE_MACHINE_I386;
	stack_frame.AddrPC.Offset = context_record.Eip;
	stack_frame.AddrFrame.Offset = context_record.Ebp;
	stack_frame.AddrStack.Offset = context_record.Esp;
#endif
	stack_frame.AddrPC.Mode = AddrModeFlat;
	stack_frame.AddrFrame.Mode = AddrModeFlat;
	stack_frame.AddrStack.Mode = AddrModeFlat;

	juce::HeapBlock<SYMBOL_INFO> symbol;
	symbol.calloc(sizeof(SYMBOL_INFO) + 256, 1);
	symbol->MaxNameLen = 255;
	symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

	while (StackWalk64(machine_type,
		GetCurrentProcess(),
		GetCurrentThread(),
		&stack_frame,
		&context_record,
		NULL,
		&SymFunctionTableAccess64,
		&SymGetModuleBase64,
		NULL)) {

		DWORD64 displacement = 0;

		if (SymFromAddr(process, (DWORD64)stack_frame.AddrPC.Offset, &displacement, symbol))
		{
			IMAGEHLP_MODULE64 moduleInfo;
			juce::zerostruct(moduleInfo);
			moduleInfo.SizeOfStruct = sizeof(moduleInfo);

			if (::SymGetModuleInfo64(process, symbol->ModBase, &moduleInfo))
				fos << moduleInfo.ModuleName << ": ";

			fos << symbol->Name << " + 0x" << String::toHexString((juce::int64)displacement) << std::endl;
		}

	}
	*/
	
	fos << newLine;
	fos << "------------------------------------------------------" << newLine;

	fos.flush();

	//return EXCEPTION_CONTINUE_SEARCH;
}

bool VASTAudioProcessor::isDumping() {
	return m_bDumpFileCreated;
}

void VASTAudioProcessor::dumpBuffersFlush() {
	//TXT
	m_bDumpFileCreated = false;
	m_DumpOutStream->flush();	
	m_DumpOutStream = nullptr;

	//WAV
	String filename = File(getVSTPath()).getChildFile("bufferDump").getFullPathName();
	// Write the file headers
	WavAudioFormat format;
	std::unique_ptr<AudioFormatWriter> writer;
	String nFile = filename + "_MasterOut.wav";
	File dumpFile = File(nFile);
	dumpFile.deleteFile();
	writer.reset(format.createWriterFor(new FileOutputStream(nFile),
		m_pVASTXperience.m_Set.m_nSampleRate, 
		m_WAVDumpBuffer_MasterOutBuffer->getNumChannels(),
		24,  //bitdepth
		{}, 0));

	if (writer != nullptr)
		writer->writeFromAudioSampleBuffer(*m_WAVDumpBuffer_MasterOutBuffer, 0, m_iDumpFileSampleCount);

	nFile = filename + "_FilterBuffer1.wav";
	dumpFile = File(nFile);
	dumpFile.deleteFile();	
	writer.reset(format.createWriterFor(new FileOutputStream(nFile),
		m_pVASTXperience.m_Set.m_nSampleRate,
		m_WAVDumpBuffer_FilterBuffer1->getNumChannels(),
		24,  //bitdepth
		{}, 0));

	if (writer != nullptr)
		writer->writeFromAudioSampleBuffer(*m_WAVDumpBuffer_FilterBuffer1, 0, m_iDumpFileSampleCount);

	nFile = filename + "_FilterBuffer2.wav";
	dumpFile = File(nFile);
	dumpFile.deleteFile();
	writer.reset(format.createWriterFor(new FileOutputStream(nFile),
		m_pVASTXperience.m_Set.m_nSampleRate,
		m_WAVDumpBuffer_FilterBuffer2->getNumChannels(),
		24,  //bitdepth
		{}, 0));

	if (writer != nullptr)
		writer->writeFromAudioSampleBuffer(*m_WAVDumpBuffer_FilterBuffer2, 0, m_iDumpFileSampleCount);

	nFile = filename + "_FilterBuffer3.wav";
	dumpFile = File(nFile);
	dumpFile.deleteFile();
	writer.reset(format.createWriterFor(new FileOutputStream(nFile),
		m_pVASTXperience.m_Set.m_nSampleRate,
		m_WAVDumpBuffer_FilterBuffer3->getNumChannels(),
		24,  //bitdepth
		{}, 0));

	if (writer != nullptr)
		writer->writeFromAudioSampleBuffer(*m_WAVDumpBuffer_FilterBuffer3, 0, m_iDumpFileSampleCount);

	nFile = filename + "_FilterVoices00.wav";
	dumpFile = File(nFile);
	dumpFile.deleteFile();
	writer.reset(format.createWriterFor(new FileOutputStream(nFile),
		m_pVASTXperience.m_Set.m_nSampleRate,
		m_WAVDumpBuffer_FilterVoices00->getNumChannels(),
		24,  //bitdepth
		{}, 0));

	if (writer != nullptr)
		writer->writeFromAudioSampleBuffer(*m_WAVDumpBuffer_FilterVoices00, 0, m_iDumpFileSampleCount);

	nFile = filename + "_FilterVoices01.wav";
	dumpFile = File(nFile);
	dumpFile.deleteFile();
	writer.reset(format.createWriterFor(new FileOutputStream(nFile),
		m_pVASTXperience.m_Set.m_nSampleRate,
		m_WAVDumpBuffer_FilterVoices01->getNumChannels(),
		24,  //bitdepth
		{}, 0));

	if (writer != nullptr)
		writer->writeFromAudioSampleBuffer(*m_WAVDumpBuffer_FilterVoices01, 0, m_iDumpFileSampleCount);

	nFile = filename + "_FilterVoices10.wav";
	dumpFile = File(nFile);
	dumpFile.deleteFile();
	writer.reset(format.createWriterFor(new FileOutputStream(nFile),
		m_pVASTXperience.m_Set.m_nSampleRate,
		m_WAVDumpBuffer_FilterVoices10->getNumChannels(),
		24,  //bitdepth
		{}, 0));

	if (writer != nullptr)
		writer->writeFromAudioSampleBuffer(*m_WAVDumpBuffer_FilterVoices10, 0, m_iDumpFileSampleCount);

	nFile = filename + "_FilterVoices11.wav";
	dumpFile = File(nFile);
	dumpFile.deleteFile();
	writer.reset(format.createWriterFor(new FileOutputStream(nFile),
		m_pVASTXperience.m_Set.m_nSampleRate,
		m_WAVDumpBuffer_FilterVoices11->getNumChannels(),
		24,  //bitdepth
		{}, 0));

	if (writer != nullptr)
		writer->writeFromAudioSampleBuffer(*m_WAVDumpBuffer_FilterVoices11, 0, m_iDumpFileSampleCount);
	
	m_WAVDumpBuffer_MasterOutBuffer = nullptr;
	m_WAVDumpBuffer_FilterBuffer1 = nullptr;
	m_WAVDumpBuffer_FilterBuffer2 = nullptr;
	m_WAVDumpBuffer_FilterBuffer3 = nullptr;
	m_WAVDumpBuffer_FilterVoices00 = nullptr;
	m_WAVDumpBuffer_FilterVoices01 = nullptr;
	m_WAVDumpBuffer_FilterVoices10 = nullptr;
	m_WAVDumpBuffer_FilterVoices11 = nullptr;
}

void VASTAudioProcessor::dumpBuffers() {
	int maxWavDump = 1000000; //~22s
	if (!m_bDumpFileCreated) {
		String filename = File(getVSTPath()).getChildFile("bufferDump.log").getFullPathName();
		File dumpFile = File(filename);	
		m_DumpOutStream = dumpFile.createOutputStream();
		if (m_DumpOutStream->openedOk()) {
			m_DumpOutStream->setPosition(0);
			m_DumpOutStream->truncate();
		}
		else {
			Result r = m_DumpOutStream->getStatus();
			VDBG("ERROR: " << r.getErrorMessage());
			vassertfalse;
		}
		m_bDumpFileCreated = true;
		m_iDumpFileSampleCount = 0;
		
		//WAV
		m_WAVDumpBuffer_MasterOutBuffer = new AudioSampleBuffer(2, maxWavDump);
		m_WAVDumpBuffer_MasterOutBuffer->clear();
		m_WAVDumpBuffer_FilterBuffer1 = new AudioSampleBuffer(2, maxWavDump);
		m_WAVDumpBuffer_FilterBuffer1->clear();
		m_WAVDumpBuffer_FilterBuffer2 = new AudioSampleBuffer(2, maxWavDump);
		m_WAVDumpBuffer_FilterBuffer2->clear();
		m_WAVDumpBuffer_FilterBuffer3 = new AudioSampleBuffer(2, maxWavDump);
		m_WAVDumpBuffer_FilterBuffer3->clear();
		m_WAVDumpBuffer_FilterVoices00 = new AudioSampleBuffer(2, maxWavDump);
		m_WAVDumpBuffer_FilterVoices00->clear();
		m_WAVDumpBuffer_FilterVoices01 = new AudioSampleBuffer(2, maxWavDump);
		m_WAVDumpBuffer_FilterVoices01->clear();
		m_WAVDumpBuffer_FilterVoices10 = new AudioSampleBuffer(2, maxWavDump);
		m_WAVDumpBuffer_FilterVoices10->clear();
		m_WAVDumpBuffer_FilterVoices11 = new AudioSampleBuffer(2, maxWavDump);
		m_WAVDumpBuffer_FilterVoices11->clear();
	}
	if (m_DumpOutStream == nullptr) return;
    
    //XmlElement root = createPatchXML(true);
    //String fileText = root.createDocument("", false, true);
	//out.writeText(fileText, false, false, "\n"); //slash n new?
    //root.deleteAllChildElements();

	int numSamples = m_pVASTXperience.m_Set.m_RoutingBuffers.getNumSamples();
	*m_DumpOutStream << "==============================================================================================================" << newLine;
	*m_DumpOutStream << "DUMP FILE FOR " << m_pVASTXperience.m_Set.m_RoutingBuffers.getNumSamples() << " SAMPLES " << newLine;
	*m_DumpOutStream << "FROM " << m_iDumpFileSampleCount << " TO " << m_iDumpFileSampleCount + numSamples << newLine;
	*m_DumpOutStream << "==============================================================================================================" << newLine;

	*m_DumpOutStream << newLine;
	*m_DumpOutStream << "msegUsed[5] = " << ((m_pVASTXperience.m_Set.m_RoutingBuffers.msegUsed[0].load() == true) ? "true" : "false") << " "
		<< ((m_pVASTXperience.m_Set.m_RoutingBuffers.msegUsed[1].load() == true) ? "true" : "false") << " "
		<< ((m_pVASTXperience.m_Set.m_RoutingBuffers.msegUsed[2].load() == true) ? "true" : "false") << " "
		<< ((m_pVASTXperience.m_Set.m_RoutingBuffers.msegUsed[3].load() == true) ? "true" : "false") << " "
		<< ((m_pVASTXperience.m_Set.m_RoutingBuffers.msegUsed[4].load() == true) ? "true" : "false") << newLine;
	*m_DumpOutStream << "lfoUsed[5] = " << ((m_pVASTXperience.m_Set.m_RoutingBuffers.lfoUsed[0].load() == true) ? "true" : "false") << " "
		<< ((m_pVASTXperience.m_Set.m_RoutingBuffers.lfoUsed[1].load() == true) ? "true" : "false") << " "
		<< ((m_pVASTXperience.m_Set.m_RoutingBuffers.lfoUsed[2].load() == true) ? "true" : "false") << " "
		<< ((m_pVASTXperience.m_Set.m_RoutingBuffers.lfoUsed[3].load() == true) ? "true" : "false") << " "
		<< ((m_pVASTXperience.m_Set.m_RoutingBuffers.lfoUsed[4].load() == true) ? "true" : "false") << newLine;
	*m_DumpOutStream << "stepSeqUsed[3] = " << ((m_pVASTXperience.m_Set.m_RoutingBuffers.stepSeqUsed[0].load() == true) ? "true" : "false") << " "
		<< ((m_pVASTXperience.m_Set.m_RoutingBuffers.stepSeqUsed[1].load() == true) ? "true" : "false") << " "
		<< ((m_pVASTXperience.m_Set.m_RoutingBuffers.stepSeqUsed[2].load() == true) ? "true" : "false") << newLine;
	
	int voicesToDump = 2; //C_MAX_POLY

	//per voice
	for (int mseg = 0; mseg < 5; mseg++) {
		for (int voice = 0; voice < voicesToDump; voice++) {
			*m_DumpOutStream << newLine;
			*m_DumpOutStream << "MSEGBuffer[" << mseg << "][" << voice << "]" << newLine;
			for (int samples = 0; samples < numSamples; samples++)
				*m_DumpOutStream << m_iDumpFileSampleCount + samples << " - " << samples << " : " << m_pVASTXperience.m_Set.m_RoutingBuffers.MSEGBuffer[mseg][voice]->getSample(0, samples) << newLine;
			//*m_DumpOutStream << "MSEGActiveBuffer[" << mseg << "][" << voice << "]" << newLine;
			//for (int samples = 0; samples < numSamples; samples++)
				//*m_DumpOutStream << m_iDumpFileSampleCount + samples << " - " << samples << " : " << (m_pVASTXperience.m_Set.m_RoutingBuffers.MSEGActiveBuffer[mseg][voice][samples] ? "true" : "false") << newLine;
		}
	}

	/*
	for (int lfo = 0; lfo < 5; lfo++) {
		for (int voice = 0; voice < voicesToDump; voice++) {
			*m_DumpOutStream << newLine;
			*m_DumpOutStream << "LFOBuffer[" << lfo << "][" << voice << "]" << newLine;
			for (int samples = 0; samples < numSamples; samples++)
				*m_DumpOutStream << m_iDumpFileSampleCount + samples << " - " << samples << " : " << m_pVASTXperience.m_Set.m_RoutingBuffers.LFOBuffer[lfo][voice]->getSample(0, samples) << newLine;
		}
	}
	*/

	for (int bank = 0; bank < 4; bank++) {
		for (int voice = 0; voice < voicesToDump; voice++) {
			*m_DumpOutStream << newLine;
			*m_DumpOutStream << "OscVoices[" << bank << "][" << voice << "]" << newLine;
			for (int samples = 0; samples < numSamples; samples++) {
				*m_DumpOutStream << m_iDumpFileSampleCount + samples << " - " << samples << " : " << m_pVASTXperience.m_Set.m_RoutingBuffers.OscVoices[bank][voice]->getSample(0, samples) << newLine;
				*m_DumpOutStream << m_iDumpFileSampleCount + samples << " - " << samples << " : " << m_pVASTXperience.m_Set.m_RoutingBuffers.OscVoices[bank][voice]->getSample(1, samples) << newLine;
			}
		}
	}

	/*
	for (int voice = 0; voice < voicesToDump; voice++) {
		*m_DumpOutStream << newLine;
		*m_DumpOutStream << "SamplerVoices[" << voice << "]" << newLine;
		for (int samples = 0; samples < numSamples; samples++) {
			*m_DumpOutStream << m_iDumpFileSampleCount + samples << " - " << samples << " L: " << m_pVASTXperience.m_Set.m_RoutingBuffers.SamplerVoices[voice]->getSample(0, samples) << newLine;
			*m_DumpOutStream << m_iDumpFileSampleCount + samples << " - " << samples << " R: " << m_pVASTXperience.m_Set.m_RoutingBuffers.SamplerVoices[voice]->getSample(1, samples) << newLine;
		}
	}

	for (int voice = 0; voice < voicesToDump; voice++) {
		*m_DumpOutStream << newLine;
		*m_DumpOutStream << "NoiseVoices[" << voice << "]" << newLine;
		for (int samples = 0; samples < numSamples; samples++) {
			*m_DumpOutStream << m_iDumpFileSampleCount + samples << " - " << samples << " L: " << m_pVASTXperience.m_Set.m_RoutingBuffers.NoiseVoices[voice]->getSample(0, samples) << newLine;
			*m_DumpOutStream << m_iDumpFileSampleCount + samples << " - " << samples << " R: " << m_pVASTXperience.m_Set.m_RoutingBuffers.NoiseVoices[voice]->getSample(1, samples) << newLine;
		}
	}
	*/

	/*
	for (int voice = 0; voice < voicesToDump; voice++) {
		*m_DumpOutStream << newLine;
		*m_DumpOutStream << "MidiNoteBuffer[" << voice << "]" << newLine;
		for (int samples = 0; samples < numSamples; samples++)
			*m_DumpOutStream << m_iDumpFileSampleCount + samples << " - " << samples << " : " << m_pVASTXperience.m_Set.m_RoutingBuffers.MidiNoteBuffer[voice]->getSample(0, samples) << newLine;
	}

	for (int voice = 0; voice < voicesToDump; voice++) {
		*m_DumpOutStream << newLine;
		*m_DumpOutStream << "VelocityBuffer[" << voice << "]" << newLine;
		for (int samples = 0; samples < numSamples; samples++)
			*m_DumpOutStream << m_iDumpFileSampleCount + samples << " - " << samples << " : " << m_pVASTXperience.m_Set.m_RoutingBuffers.VelocityBuffer[voice]->getSample(0, samples) << newLine;
	}
	*/


	for (int voice = 0; voice < voicesToDump; voice++) {
		for (int filter = 0; filter < 3; filter++) {
			*m_DumpOutStream << newLine;
			*m_DumpOutStream << "FilterVoices[" << filter << "][" << voice << "]" << newLine;
			for (int samples = 0; samples < numSamples; samples++) {
				*m_DumpOutStream << m_iDumpFileSampleCount + samples << " - " << samples << " L: " << m_pVASTXperience.m_Set.m_RoutingBuffers.FilterVoices[filter][voice]->getSample(0, samples) << newLine;
				*m_DumpOutStream << m_iDumpFileSampleCount + samples << " - " << samples << " R: " << m_pVASTXperience.m_Set.m_RoutingBuffers.FilterVoices[filter][voice]->getSample(1, samples) << newLine;
			}
		}
	}
	if ((m_iDumpFileSampleCount + numSamples) < maxWavDump) {
		m_WAVDumpBuffer_FilterVoices00->copyFrom(0, m_iDumpFileSampleCount, m_pVASTXperience.m_Set.m_RoutingBuffers.FilterVoices[0][0]->getReadPointer(0), numSamples);
		m_WAVDumpBuffer_FilterVoices00->copyFrom(1, m_iDumpFileSampleCount, m_pVASTXperience.m_Set.m_RoutingBuffers.FilterVoices[0][0]->getReadPointer(1), numSamples);
		m_WAVDumpBuffer_FilterVoices01->copyFrom(0, m_iDumpFileSampleCount, m_pVASTXperience.m_Set.m_RoutingBuffers.FilterVoices[0][1]->getReadPointer(0), numSamples);
		m_WAVDumpBuffer_FilterVoices01->copyFrom(1, m_iDumpFileSampleCount, m_pVASTXperience.m_Set.m_RoutingBuffers.FilterVoices[0][1]->getReadPointer(1), numSamples);
		m_WAVDumpBuffer_FilterVoices10->copyFrom(0, m_iDumpFileSampleCount, m_pVASTXperience.m_Set.m_RoutingBuffers.FilterVoices[1][0]->getReadPointer(0), numSamples);
		m_WAVDumpBuffer_FilterVoices10->copyFrom(1, m_iDumpFileSampleCount, m_pVASTXperience.m_Set.m_RoutingBuffers.FilterVoices[1][0]->getReadPointer(1), numSamples);
		m_WAVDumpBuffer_FilterVoices11->copyFrom(0, m_iDumpFileSampleCount, m_pVASTXperience.m_Set.m_RoutingBuffers.FilterVoices[1][1]->getReadPointer(0), numSamples);
		m_WAVDumpBuffer_FilterVoices11->copyFrom(1, m_iDumpFileSampleCount, m_pVASTXperience.m_Set.m_RoutingBuffers.FilterVoices[1][1]->getReadPointer(1), numSamples);
	}

	/*
	//stereos
	for (int bank = 0; bank < 4; bank++) {
		*m_DumpOutStream << newLine;
		*m_DumpOutStream << "OscBuffer[" << bank << "]" << newLine;
		for (int samples = 0; samples < numSamples; samples++) {
			*m_DumpOutStream << m_iDumpFileSampleCount + samples << " - " << samples << " L: " << m_pVASTXperience.m_Set.m_RoutingBuffers.OscBuffer[bank]->getSample(0, samples) << newLine;
			*m_DumpOutStream << m_iDumpFileSampleCount + samples << " - " << samples << " R: " << m_pVASTXperience.m_Set.m_RoutingBuffers.OscBuffer[bank]->getSample(1, samples) << newLine;
		}
	}

	*m_DumpOutStream << newLine;
	*m_DumpOutStream << "NoiseBuffer" << newLine;
	for (int samples = 0; samples < numSamples; samples++) {
		*m_DumpOutStream << m_iDumpFileSampleCount + samples << " - " << samples << " L: " << m_pVASTXperience.m_Set.m_RoutingBuffers.NoiseBuffer->getSample(0, samples) << newLine;
		*m_DumpOutStream << m_iDumpFileSampleCount + samples << " - " << samples << " R: " << m_pVASTXperience.m_Set.m_RoutingBuffers.NoiseBuffer->getSample(1, samples) << newLine;
	}

	*m_DumpOutStream << newLine;
	*m_DumpOutStream << "SamplerBuffer" << newLine;
	for (int samples = 0; samples < numSamples; samples++) {
		*m_DumpOutStream << m_iDumpFileSampleCount + samples << " - " << samples << " L: " << m_pVASTXperience.m_Set.m_RoutingBuffers.SamplerBuffer->getSample(0, samples) << newLine;
		*m_DumpOutStream << m_iDumpFileSampleCount + samples << " - " << samples << " R: " << m_pVASTXperience.m_Set.m_RoutingBuffers.SamplerBuffer->getSample(1, samples) << newLine;
	}
	*/

	for (int filter = 0; filter < 3; filter++) {
		*m_DumpOutStream << newLine;
		*m_DumpOutStream << "FilterBuffer[" << filter << "]" << newLine;
		for (int samples = 0; samples < numSamples; samples++) {
			*m_DumpOutStream << m_iDumpFileSampleCount + samples << " - " << samples << " L: " << m_pVASTXperience.m_Set.m_RoutingBuffers.FilterBuffer[filter]->getSample(0, samples) << newLine;
			*m_DumpOutStream << m_iDumpFileSampleCount + samples << " - " << samples << " R: " << m_pVASTXperience.m_Set.m_RoutingBuffers.FilterBuffer[filter]->getSample(1, samples) << newLine;
		}
	}
	if ((m_iDumpFileSampleCount + numSamples) < maxWavDump) {
		m_WAVDumpBuffer_FilterBuffer1->copyFrom(0, m_iDumpFileSampleCount, m_pVASTXperience.m_Set.m_RoutingBuffers.FilterBuffer[0]->getReadPointer(0), numSamples);
		m_WAVDumpBuffer_FilterBuffer1->copyFrom(1, m_iDumpFileSampleCount, m_pVASTXperience.m_Set.m_RoutingBuffers.FilterBuffer[0]->getReadPointer(1), numSamples);
		m_WAVDumpBuffer_FilterBuffer2->copyFrom(0, m_iDumpFileSampleCount, m_pVASTXperience.m_Set.m_RoutingBuffers.FilterBuffer[1]->getReadPointer(0), numSamples);
		m_WAVDumpBuffer_FilterBuffer2->copyFrom(1, m_iDumpFileSampleCount, m_pVASTXperience.m_Set.m_RoutingBuffers.FilterBuffer[1]->getReadPointer(1), numSamples);
		m_WAVDumpBuffer_FilterBuffer3->copyFrom(0, m_iDumpFileSampleCount, m_pVASTXperience.m_Set.m_RoutingBuffers.FilterBuffer[2]->getReadPointer(0), numSamples);
		m_WAVDumpBuffer_FilterBuffer3->copyFrom(1, m_iDumpFileSampleCount, m_pVASTXperience.m_Set.m_RoutingBuffers.FilterBuffer[2]->getReadPointer(1), numSamples);
	}

	/*
	for (int fxbus = 0; fxbus < 3; fxbus++) {
		*m_DumpOutStream << newLine;
		*m_DumpOutStream << "FxBusBuffer[" << fxbus << "]" << newLine;
		for (int samples = 0; samples < numSamples; samples++) {
			*m_DumpOutStream << m_iDumpFileSampleCount + samples << " - " << samples << " L: " << m_pVASTXperience.m_Set.m_RoutingBuffers.FxBusBuffer[fxbus]->getSample(0, samples) << newLine;
			*m_DumpOutStream << m_iDumpFileSampleCount + samples << " - " << samples << " R: " << m_pVASTXperience.m_Set.m_RoutingBuffers.FxBusBuffer[fxbus]->getSample(1, samples) << newLine;
		}
	}

	//monos
	for (int lfo = 0; lfo < 5; lfo++) {
		*m_DumpOutStream << newLine;
		*m_DumpOutStream << "LFOGlobalBuffer[" << lfo << "]" << newLine;
		for (int samples = 0; samples < numSamples; samples++)
			*m_DumpOutStream << m_iDumpFileSampleCount + samples << " - " << samples << " : " << m_pVASTXperience.m_Set.m_RoutingBuffers.LFOGlobalBuffer[lfo]->getSample(0, samples) << newLine;
	}

	for (int stepseq = 0; stepseq < 3; stepseq++) {
		*m_DumpOutStream << newLine;
		*m_DumpOutStream << "StepSeqBuffer[" << stepseq << "]" << newLine;
		for (int samples = 0; samples < numSamples; samples++)
			*m_DumpOutStream << m_iDumpFileSampleCount + samples << " - " << samples << " : " << m_pVASTXperience.m_Set.m_RoutingBuffers.StepSeqBuffer[stepseq]->getSample(0, samples) << newLine;
	}

	for (int custmod = 0; custmod < 4; custmod++) {
		*m_DumpOutStream << newLine;
		*m_DumpOutStream << "CustomModulatorBuffer[" << custmod << "]" << newLine;
		for (int samples = 0; samples < numSamples; samples++)
			*m_DumpOutStream << m_iDumpFileSampleCount + samples << " - " << samples << " : " << m_pVASTXperience.m_Set.m_RoutingBuffers.CustomModulatorBuffer[custmod]->getSample(0, samples) << newLine;
	}
	*/

	*m_DumpOutStream << newLine;
	*m_DumpOutStream << "MasterOutBuffer" << newLine;
	for (int samples = 0; samples < numSamples; samples++) {
		*m_DumpOutStream << m_iDumpFileSampleCount + samples << " - " << samples << " L: " << m_pVASTXperience.m_Set.m_RoutingBuffers.MasterOutBuffer->getSample(0, samples) << " R: " << m_pVASTXperience.m_Set.m_RoutingBuffers.MasterOutBuffer->getSample(1, samples) << newLine;
	}
	if ((m_iDumpFileSampleCount + numSamples) < maxWavDump) {
		m_WAVDumpBuffer_MasterOutBuffer->copyFrom(0, m_iDumpFileSampleCount, m_pVASTXperience.m_Set.m_RoutingBuffers.MasterOutBuffer->getReadPointer(0), numSamples);
		m_WAVDumpBuffer_MasterOutBuffer->copyFrom(1, m_iDumpFileSampleCount, m_pVASTXperience.m_Set.m_RoutingBuffers.MasterOutBuffer->getReadPointer(1), numSamples);
	}

	m_iDumpFileSampleCount += numSamples;
}

String VASTAudioProcessor::getVersionString() {
	// version
	String lVersionStr = ProjectInfo::versionString;
	String lVersion = "v" + lVersionStr + " ";

	String pluginType = "";
	switch (PluginHostType::getPluginLoadedAs())
	{
		//case AudioProcessor::wrapperType_Undefined:     pluginType = "undefined"; break;
		case AudioProcessor::wrapperType_Undefined:     pluginType = "CLAP"; break; //this is CLAP for now
		case AudioProcessor::wrapperType_VST:           pluginType = "VST"; break;
		case AudioProcessor::wrapperType_VST3:          pluginType = "VST3"; break;
		case AudioProcessor::wrapperType_AudioUnit:     pluginType = "AU"; break;
		case AudioProcessor::wrapperType_AudioUnitv3:   pluginType = "AUv3"; break;
		case AudioProcessor::wrapperType_Unity:         pluginType = "Unity"; break;
		case AudioProcessor::wrapperType_LV2:           pluginType = "LV2"; break;
		case AudioProcessor::wrapperType_AAX:           pluginType = "AAX"; break;
		case AudioProcessor::wrapperType_Standalone:    pluginType = "Standalone"; break;
		default: pluginType = "undefined"; break;
	}
	lVersion = lVersion + pluginType + " ";

#ifdef _MACOSX32BIT
	lVersion.append("OSX32", 5);
#elif  _MACOSX64BIT
	lVersion.append("OSX64", 5);
#elif  _WIN64BIT
	lVersion.append("Win64", 5);
	//_WIN32 also defined for WIN64!!
#elif  _WIN32BIT 
	lVersion.append("Win32", 5);
#endif
#ifdef _SSE2_VERSION
    #if JUCE_MAC
        #ifdef JUCE_ARM
            lVersion.append(" UBarm", 7);
        #else
            lVersion.append(" UBint", 7);
        #endif
    #else
        lVersion.append(" SSE2", 5);
    #endif
#endif
#ifdef _DEBUG
	lVersion.append(" (Debug)", 8);
#endif
#if defined(VASTBUILD) || defined(VASTCOMMERCIAL)
	lVersion.append(" v", 2);
#endif
#if defined(VASTLOG) 
	lVersion.append(" log", 4);
#endif
	return lVersion;
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	return new VASTAudioProcessor();
}
