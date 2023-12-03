/*
VAST Dynamics Audio Software (TM)

All modulators tested: OK
*/

#include "VASTPhaser.h"
#include "../VASTEngineHeader.h"
#include "../VASTSettings.h"
#include "../../Plugin/VASTAudioProcessor.h"
#include "VASTEffect.h"

const int c_paramNumFilters = 4; // 2, 4 , 6, 8, 10

CVASTPhaser::CVASTPhaser(VASTAudioProcessor* processor, CVASTSettings& set, int busnr) : m_Set(&set) , m_LFO(set, nullptr) {
	my_processor = processor;
	myBusnr = busnr;

	m_bIsOff = true;
	m_bShallBeOff = true;
	m_iSoftFade = 0;

	setEffectName("PHASER");
}

void CVASTPhaser::initParameters() {
	AudioProcessorValueTreeState& parameters = my_processor->getParameterTree();
	createAndAddParameter(&m_bPhaserOnOff, parameters, 1, "m_bPhaserOnOff", "Phaser on / off", "On", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 1.0f, 1.0f), 0.0f,
		CVASTParamState::toggleButtonValueToTextFunction,
		CVASTParamState::toggleButtonTextToValueFunction,
		false, true, true, true,
		true);

	createAndAddParameter(&m_fPhaserDryWet, parameters, 1, "m_fPhaserDryWet", "Phaser filter mix in (dry - wet)", "DryWet", 1,
		MODMATDEST::PhaserDryWet,
		NormalisableRange<float>(0, 100), 100,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	createAndAddParameter(&m_fPhaserWidth, parameters, 1, "m_fPhaserWidth", "Phaser width", "Width", 2,
		MODMATDEST::PhaserWidth,
		NormalisableRange<float>(50.f, 3000.f), 1000.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	createAndAddParameter(&m_fPhaserFeedback, parameters, 1, "m_fPhaserFeedback", "Phase feedback", "Feedback", 3,
		MODMATDEST::PhaserFeedback,
		NormalisableRange<float>(0.f, 0.9f), 0.7f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	createAndAddParameter(&m_fPhaserMinFrequency, parameters, 1, "m_fPhaserMinFrequency", "Phaser min. frequency (Hz)", "Minfreq", 4,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(50.f, 1000.f), 80.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	createAndAddParameter(&m_uPhaserLFOWave, parameters, 1, "m_uPhaserLFOWave", "Phaser LFO waveform", "LFOWave", 5,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, LFOWAVE_Array.size() - 1, 1.0f), 0.0f,
		CVASTParamState::comboBoxValueToTextFunction_LFOWAVE,
		CVASTParamState::comboBoxTextToValueFunction_LFOWAVE,
		false, true, false, false,
		true);
	createAndAddParameter(&m_bPhaserSynch, parameters, 1, "m_bPhaserSynch", "Phaser sync to DAW", "Sync", 6,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 1.0f, 1.0f), 0.0f,
		CVASTParamState::toggleButtonValueToTextFunction,
		CVASTParamState::toggleButtonTextToValueFunction,
		false, true, true, false,
		true);
	createAndAddParameter(&m_uPhaserTimeBeats, parameters, 1, "m_uPhaserTimeBeats", "Phaser time in beats when synced to DAW", "DAW", 7,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, TIMEBEATS_Array.size() - 1, 1.0f), TIMEBEATS::BEATS1,
		CVASTParamState::comboBoxValueToTextFunction_TIMEBEATS,
		CVASTParamState::comboBoxTextToValueFunction_TIMEBEATS,
		false, true, true, false,
		true);
	createAndAddParameter(&m_fPhaserLFOFreq, parameters, 1, "m_fPhaserLFOFreq", "Phaser LFO frequency (Hz)", "Frequency", 8,
		MODMATDEST::PhaserLFOFrequency,
		NormalisableRange<float>(0.0f, 2.0f), 1.00f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);
	createAndAddParameter(&m_fPhaserGain, parameters, 1, "m_fPhaserGain", "Phaser output gain", "Gain", 9,
		MODMATDEST::PhaserGain,
		NormalisableRange<float>(0, 200), 100,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);
}

void CVASTPhaser::initCompatibilityParameters() {
	// all new parameters go here
}

CVASTPhaser::~CVASTPhaser(void) {
	AudioProcessorValueTreeState& parameters = ((VASTAudioProcessor*)my_processor)->getParameterTree();
	Array<AudioProcessorParameterWithID*> params = getParameters();
	for (int i = 0; i < params.size(); i++) {
		parameters.removeParameterListener(params[i]->paramID, (AudioProcessorValueTreeState::Listener*)this);
	}
}

void CVASTPhaser::init() {
	initParameters();
	reset();

	m_LFO.init();
	m_LFO.m_uPolarity = 0; //unipolar
	m_LFO.updateMainVariables(m_iSampleRate, WAVE::tri, 1, 1.0, 0, 0); //TODO CHECK
	m_LFO.startLFOFrequency(*m_fPhaserLFOFreq, -1);
}


void CVASTPhaser::releaseResources() {
}

void CVASTPhaser::parameterChanged(const String& parameterID, float newValue) {
	if (parameterID.startsWith("m_bPhaserOnOff")) {
		if (newValue == static_cast<int>(SWITCH::SWITCH_ON))
			switchOn();
		else
			switchOff();
		return;
	}
	else if (parameterID.startsWith("m_fPhaserWidth")) {
		m_fPhaserWidth_smoothed.setTargetValue(newValue);
	}
	else if (parameterID.startsWith("m_fPhaserFeedback")) {
		m_fPhaserFeedback_smoothed.setTargetValue(newValue);
	}
	else if (parameterID.startsWith("m_fPhaserMinFrequency")) {
		m_fPhaserMinFrequency_smoothed.setTargetValue(newValue);
	}
	else if (parameterID.startsWith("m_fPhaserDryWet")) {
		m_fPhaserDryWet_smoothed.setTargetValue(newValue);
	}
	else if (parameterID.startsWith("m_fPhaserGain")) {
		m_fPhaserGain_smoothed.setTargetValue(newValue);
	}
	else if (parameterID.startsWith("m_fPhaserLFOFreq")) {
		updateLFOFreq();
	}
	else if (parameterID.startsWith("m_uPhaserTimeBeats")) {
		updateLFOFreq();
	}
	else if (parameterID.startsWith("m_bPhaserSynch")) {
		updateLFOFreq();
	}
	else if (parameterID.startsWith("m_uPhaserLFOWave")) {
		m_LFO.updateMainVariables(m_iSampleRate, int(*m_uPhaserLFOWave), 1, 1.0, 0, 0); //TODO CHECK
		m_LFO.startLFOFrequency(*m_fPhaserLFOFreq, -1);
	}
}

void CVASTPhaser::updateLFOFreq() {
	if (*m_bPhaserSynch == static_cast<int>(SWITCH::SWITCH_OFF)) {
		m_fPhaserLFOFreq_smoothed.setTargetValue(*m_fPhaserLFOFreq); //  *m_fTimeMod;
	}
	else { //bpm synch
		float l_fIntervalTime = 0.f;
		if (m_Set->m_dPpqBpm == 0.f) 
			return;
		l_fIntervalTime = m_Set->getIntervalTimeFromDAWBeats(*m_uPhaserTimeBeats);
		if (l_fIntervalTime < 0.1f) l_fIntervalTime = 0.1f; // minimum
		//if (l_fIntervalTime > 5000.0f) l_fIntervalTime = 5000.f; // maximum
		if (l_fIntervalTime > 100000.0f) l_fIntervalTime = 100000.0f; // maximum  //CHTS 3.0.1

		m_fPhaserLFOFreq_smoothed.setCurrentAndTargetValue(1); //reset it
		m_fPhaserLFOFreq_smoothed.setTargetValue(1.0f / (l_fIntervalTime / 1000.f));
	}
}

void CVASTPhaser::prepareToPlay(double , int samplesPerBlock) {
	m_iExpectedSamplesPerBlock = samplesPerBlock;
	//m_iSampleRate is set in useOversampling
	if (m_bOversampling)
		m_fBuffer.setSize(2, samplesPerBlock * C_OVERSAMPLING_RATIO, false, false, false);
	else
		m_fBuffer.setSize(2, samplesPerBlock, false, false, false);

	const double smoothTime = 0.02;  //check 1e-3?
	m_fPhaserWidth_smoothed.reset(m_iSampleRate, smoothTime);
	m_fPhaserFeedback_smoothed.reset(m_iSampleRate, smoothTime);
	m_fPhaserMinFrequency_smoothed.reset(m_iSampleRate, smoothTime);
	m_fPhaserLFOFreq_smoothed.reset(m_iSampleRate, smoothTime);
	m_fPhaserDryWet_smoothed.reset(m_iSampleRate, smoothTime);
	m_fPhaserGain_smoothed.reset(m_iSampleRate, smoothTime);

	numFiltersPerChannel = c_paramNumFilters;

	filters.clear();
	for (int i = 0; i < 2 * numFiltersPerChannel; ++i) {
		filters.add(new Filter());
	}

	filteredOutputs.clear();
	for (int i = 0; i < 2; ++i)
		filteredOutputs.add(0.0f);

	sampleCountToUpdateFilters = 0;
	updateFiltersInterval = 32;
	inverseSampleRate = 1.0f / (float)m_iSampleRate;

	updateLFOFreq();
	m_LFO.updateMainVariables(m_iSampleRate, int(*m_uPhaserLFOWave), 1, 1.0, 0, 0); //TODO CHECK
	m_LFO.startLFOFrequency(*m_fPhaserLFOFreq, -1);
	reset();
}

void CVASTPhaser::reset() {
	if ((m_bIsOff == false) && (m_bShallBeOff == false)) {
		m_iSoftFade = 0;
		m_bShallBeOff = false; //refade in
		m_bIsOff = true;
	}
}

void CVASTPhaser::updateTiming() {
	updateLFOFreq();
}

void CVASTPhaser::processBlock(AudioSampleBuffer& buffer, MidiBuffer& , const int numSamples) {

	if (isOffAndShallBeOff() == true) return;

	ScopedNoDenormals noDenormals;

	modMatrixInputState inputState;

	//======================================

	unsigned int sampleCount;

	float* bufferWritePointerL = buffer.getWritePointer(0);
	float* bufferWritePointerR = buffer.getWritePointer(1);

	sampleCount = sampleCountToUpdateFilters;

	for (int currentFrame = 0; currentFrame < numSamples; currentFrame += 1) {
		int currentFrameOSAdjusted = currentFrame;
		if (m_bOversampling) currentFrameOSAdjusted /= C_OVERSAMPLING_RATIO;

		checkSoftFade();
		
		inputState = ((VASTAudioProcessor*)my_processor)->m_pVASTXperience.m_Poly.getOldestNotePlayedInputState(currentFrameOSAdjusted); // make parameter oldest or newest
		if (*m_bPhaserSynch == static_cast<int>(SWITCH::SWITCH_OFF))
			m_fPhaserLFOFreq_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_fPhaserLFOFreq, MODMATDEST::PhaserLFOFrequency, &inputState));
		if (m_fPhaserLFOFreq_smoothed.isSmoothing()) {
			m_LFO.startLFOFrequency(m_fPhaserLFOFreq_smoothed.getNextValue(), -1);
		}
																																									 
		m_fPhaserDryWet_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_fPhaserDryWet, MODMATDEST::PhaserDryWet, &inputState));
		float lPhaserDryWet = m_fPhaserDryWet_smoothed.getNextValue();

		m_fPhaserGain_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_fPhaserGain, MODMATDEST::PhaserGain, &inputState));
		float lPhaserGain = m_fPhaserGain_smoothed.getNextValue();

		m_fPhaserWidth_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_fPhaserWidth, MODMATDEST::PhaserWidth, &inputState));
		float lPhaserWidth = m_fPhaserWidth_smoothed.getNextValue();

		m_fPhaserFeedback_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_fPhaserFeedback, MODMATDEST::PhaserFeedback, &inputState));
		float lPhaserFeedback = m_fPhaserFeedback_smoothed.getNextValue();

		float fLFOOsc = 0.f;
		m_LFO.getOscillation(&fLFOOsc);

		float in_left = bufferWritePointerL[currentFrame];
		float in_right = bufferWritePointerR[currentFrame];

		float centreFrequency = fLFOOsc;
		centreFrequency *= lPhaserWidth;
		centreFrequency += m_fPhaserMinFrequency_smoothed.getNextValue();

		if (sampleCount++ % updateFiltersInterval == 0)
			updateFilters(centreFrequency);

		float filtered_left = in_left + lPhaserFeedback * filteredOutputs[0];
		float filtered_right = in_right + lPhaserFeedback * filteredOutputs[1];

		for (int i = 0; i < c_paramNumFilters; ++i) {
			filtered_left = filters[i]->processSingleSampleRaw(filtered_left);
			filtered_right = filters[c_paramNumFilters + i]->processSingleSampleRaw(filtered_right);
		}

		filteredOutputs.set(0, filtered_left);
		filteredOutputs.set(1, filtered_right);

		float lMix = lPhaserDryWet / 100.f * (float(m_iSoftFade) / float(C_MAX_SOFTFADE));
		bufferWritePointerL[currentFrame] = in_left * (1 - lMix) + filtered_left * lMix;
		bufferWritePointerR[currentFrame] = in_right * (1 - lMix) + filtered_right * lMix;

		bufferWritePointerL[currentFrame] *= lPhaserGain * 0.01f;
		bufferWritePointerR[currentFrame] *= lPhaserGain * 0.01f;
	}

	sampleCountToUpdateFilters = sampleCount;
}

void CVASTPhaser::updateFilters(double centreFrequency) {
	double discreteFrequency = M_2PI * centreFrequency * inverseSampleRate;

	for (int i = 0; i < filters.size(); ++i)
		filters[i]->updateCoefficients(discreteFrequency);

	/*
	for (int i = 0; i < filters.size(); ++i) {
		double wc = jmin(discreteFrequency, M_PI * 0.99);
		double tan_half_wc = tan(wc / 2.0);

		//filters[i]->m_d_b0 = tan_half_wc - 1.0,
		filters[i]->m_d_b1 = tan_half_wc + 1.0;
		filters[i]->m_d_b2 = 0.0;
		filters[i]->m_d_a0 = tan_half_wc + 1.0;
		filters[i]->m_d_a1 = tan_half_wc - 1.0;
		filters[i]->m_d_a2 = 0.0;
	}
	*/
}

//==============================================================================

void CVASTPhaser::getStateInformation(MemoryBlock& )
{
	//std::unique_ptr<XmlElement> xml (parameters.valueTreeState.state.createXml());
	//copyXmlToBinary (*xml, destData);
}

void CVASTPhaser::setStateInformation(const void* , int )
{
	//std::unique_ptr<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
	//if (xmlState != nullptr)
	//  if (xmlState->hasTagName (parameters.valueTreeState.state.getType()))
	//    parameters.valueTreeState.state = ValueTree::fromXml (*xmlState);
}
