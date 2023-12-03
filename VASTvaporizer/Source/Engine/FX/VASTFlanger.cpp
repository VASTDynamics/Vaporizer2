/*
VAST Dynamics Audio Software (TM)

All modulators tested: OK
*/

#include "VASTFlanger.h"
#include "../VASTEngineHeader.h"
#include "../VASTSettings.h"
#include "../../Plugin/VASTAudioProcessor.h"
#include "../VASTParamState.h"
#include "VASTEffect.h"
#include <math.h>

CVASTFlanger::CVASTFlanger(VASTAudioProcessor* processor, CVASTSettings& set, int busnr) : m_Set(&set), m_LFO(set, nullptr) {
	my_processor = processor;
	myBusnr = busnr;

	m_bIsOff = true;
	m_bShallBeOff = true;
	m_iSoftFade = 0;

	setEffectName("FLANGER");
}

void CVASTFlanger::initParameters() {
	AudioProcessorValueTreeState& parameters = my_processor->getParameterTree();

	createAndAddParameter(&m_bFlangerOnOff, parameters, 1, "m_bFlangerOnOff", "Flanger on / off", "On", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 1.0f, 1.0f), 0.0f,
		CVASTParamState::toggleButtonValueToTextFunction,
		CVASTParamState::toggleButtonTextToValueFunction,
		false, true, true, true,
		true);
	
	createAndAddParameter(&m_fFlangerDryWet, parameters, 1, "m_fFlangerDryWet", "Flanger mix in (dry - wet)", "DryWet", 1,
		MODMATDEST::FlangerDryWet,
		NormalisableRange<float>(0, 100), 100,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	createAndAddParameter(&m_fFlangerDelay, parameters, 1, "m_fFlangerDelay", "Flanger delay (ms)", "Delay", 2,
		MODMATDEST::FlangerDelay,
		NormalisableRange<float>(1.0f, 20.f), 2.5f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	createAndAddParameter(&m_fFlangerWidth, parameters, 1, "m_fFlangerWidth", "Flanger width", "Width", 3,
		MODMATDEST::FlangerWidth,
		NormalisableRange<float>(1.0f, 20.f), 10.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	createAndAddParameter(&m_fFlangerFeedback, parameters, 1, "m_fFlangerFeedback", "Flanger feedback", "Feedback", 4,
		MODMATDEST::FlangerFeedback,
		NormalisableRange<float>(0.f, 0.5f), 0.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	createAndAddParameter(&m_uFlangerLFOWave, parameters, 1, "m_uFlangerLFOWave", "Flanger LFO waveform", "LFOWave", 5,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, LFOWAVE_Array.size() - 1, 1.0f), 0.0f,
		CVASTParamState::comboBoxValueToTextFunction_LFOWAVE,
		CVASTParamState::comboBoxTextToValueFunction_LFOWAVE,
		false, true, false, false,
		true);

	createAndAddParameter(&m_bFlangerSynch, parameters, 1, "m_bFlangerSynch", "Synch flanger to DAW", "Sync", 6,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 1.0f, 1.0f), 0.0f,
		CVASTParamState::toggleButtonValueToTextFunction,
		CVASTParamState::toggleButtonTextToValueFunction,
		false, true, true, false,
		true);
	createAndAddParameter(&m_uFlangerTimeBeats, parameters, 1, "m_uFlangerTimeBeats", "Flanger time in beats when synched to DAW", "DAW", 7,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, TIMEBEATS_Array.size() - 1, 1.0f), TIMEBEATS::BEATS1,
		CVASTParamState::comboBoxValueToTextFunction_TIMEBEATS,
		CVASTParamState::comboBoxTextToValueFunction_TIMEBEATS,
		false, true, true, false,
		true);
	createAndAddParameter(&m_fFlangerLFOFreq, parameters, 1, "m_fFlangerLFOFreq", "Flanger LFO frequency (Hz)", "Frequency", 8,
		MODMATDEST::FlangerLFOFrequency,
		NormalisableRange<float>(0.05f, 2.0f), 0.2f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);
	createAndAddParameter(&m_fFlangerGain, parameters, 1, "m_fFlangerGain", "Flanger output gain", "Gain", 9,
		MODMATDEST::FlangerGain,
		NormalisableRange<float>(0, 200), 100,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);
}

void CVASTFlanger::initCompatibilityParameters() {
	// all new parameters go here
}

CVASTFlanger::~CVASTFlanger(void) {	
	AudioProcessorValueTreeState& parameters = ((VASTAudioProcessor*)my_processor)->getParameterTree();
	Array<AudioProcessorParameterWithID*> params = getParameters();
	for (int i = 0; i < params.size(); i++) {
		parameters.removeParameterListener(params[i]->paramID, (AudioProcessorValueTreeState::Listener*)this);
	}
}

void CVASTFlanger::releaseResources() {
}

void CVASTFlanger::updateTiming() {
	updateLFOFreq();
}

void CVASTFlanger::parameterChanged(const String& parameterID, float newValue) {
	if (parameterID.startsWith("m_bFlangerOnOff")) {
		if (newValue == static_cast<int>(SWITCH::SWITCH_ON))
			switchOn();
		else
			switchOff();
		return;
	}

	else if (parameterID.startsWith("m_fFlangerDelay")) {
		m_fFlangerDelay_smoothed.setTargetValue(newValue);
	}
	else if (parameterID.startsWith("m_fFlangerWidth")) {
		m_fFlangerWidth_smoothed.setTargetValue(newValue);
	}
	else if (parameterID.startsWith("m_fFlangerFeedback")) {
		m_fFlangerFeedback_smoothed.setTargetValue(newValue);
	}
	else if (parameterID.startsWith("m_fFlangerFeedback")) {
		m_fFlangerFeedback_smoothed.setTargetValue(newValue);
	}
	else if (parameterID.startsWith("m_fFlangerDryWet")) {
		m_fFlangerDryWet_smoothed.setTargetValue(newValue);
	}
	else if (parameterID.startsWith("m_fFlangerGain")) {
		m_fFlangerGain_smoothed.setTargetValue(newValue);
	}
	else if (parameterID.startsWith("m_fFlangerLFOFreq")) {
		updateLFOFreq();
	}
	else if (parameterID.startsWith("m_bFlangerSynch")) {
		updateLFOFreq();
	}
	else if (parameterID.startsWith("m_uFlangerTimeBeats")) {
		updateLFOFreq();
	}
	else if (parameterID.startsWith("m_uFlangerLFOWave")) {
		m_LFO.updateMainVariables(m_iSampleRate, int(*m_uFlangerLFOWave), 1, 1.0, 0, 0); //TODO CHECK
		m_LFO.startLFOFrequency(*m_fFlangerLFOFreq, -1);
	}
}

void CVASTFlanger::updateLFOFreq() {
	if (*m_bFlangerSynch == static_cast<int>(SWITCH::SWITCH_OFF)) {
		m_fFlangerLFOFreq_smoothed.setTargetValue(*m_fFlangerLFOFreq); //  *m_fTimeMod;
	}
	else { //bpm synch
		float l_fIntervalTime = 0.f;
		if (m_Set->m_dPpqBpm == 0.f) return;
		l_fIntervalTime = m_Set->getIntervalTimeFromDAWBeats(*m_uFlangerTimeBeats);
		if (l_fIntervalTime < 0.1f) l_fIntervalTime = 0.1f; // minimum
		//if (l_fIntervalTime > 5000.0f) l_fIntervalTime = 5000.f; // maximum
		if (l_fIntervalTime > 100000.0f) l_fIntervalTime = 100000.0f; // maximum  //CHTS 3.0.1

		m_fFlangerLFOFreq_smoothed.setCurrentAndTargetValue(1); //reset it
		m_fFlangerLFOFreq_smoothed.setTargetValue(1.0f / (l_fIntervalTime / 1000.f));
	}
}

void CVASTFlanger::prepareToPlay(double sampleRate, int samplesPerBlock) {
	//m_iSampleRate is set in userOversampling
	m_iExpectedSamplesPerBlock = samplesPerBlock;
	if (m_bOversampling)
		m_fBuffer.setSize(2, samplesPerBlock * C_OVERSAMPLING_RATIO, false, false, false);
	else
		m_fBuffer.setSize(2, samplesPerBlock, false, false, false);

	reset();
	
	const double smoothTime = 0.02;  //check 1e-3?
	m_fFlangerDelay_smoothed.reset(m_iSampleRate, smoothTime);
	m_fFlangerWidth_smoothed.reset(m_iSampleRate, smoothTime);
	m_fFlangerFeedback_smoothed.reset(m_iSampleRate, smoothTime);
	m_fFlangerLFOFreq_smoothed.reset(m_iSampleRate, smoothTime);
	m_fFlangerDryWet_smoothed.reset(m_iSampleRate, smoothTime);
	m_fFlangerGain_smoothed.reset(m_iSampleRate, smoothTime);

	float maxDelayTime =  40.f * 0.001f; // paramDelay.maxValue + paramWidth.maxValue;
	delayBufferSamples = (int)(maxDelayTime * (float)m_iSampleRate) + 1;
	if (delayBufferSamples < 1)
		delayBufferSamples = 1;

	delayBufferChannels = 2;
	delayBuffer.setSize(delayBufferChannels, delayBufferSamples, false, false, false);
	delayBuffer.clear();

	delayWritePosition = 0;

	m_LFO.updateMainVariables(m_iSampleRate, int(*m_uFlangerLFOWave), 1,  1.0, 0, 0); //TODO CHECK
	m_LFO.startLFOFrequency(*m_fFlangerLFOFreq, -1);
	reset();
}

void CVASTFlanger::reset() {
	if ((m_bIsOff == false) && (m_bShallBeOff == false)) {
		m_iSoftFade = 0;
		m_bShallBeOff = false; //refade in
		m_bIsOff = true;
	}
}

void CVASTFlanger::init() {
	initParameters();
	reset();

	m_LFO.init();
	m_LFO.m_uPolarity = 0; //unipolar
	m_LFO.updateMainVariables(m_iSampleRate, WAVE::tri, 1, 1.0, 0, 0); //TODO CHECK
	m_LFO.startLFOFrequency(*m_fFlangerLFOFreq, -1);
}

void CVASTFlanger::processBlock(AudioSampleBuffer& buffer, MidiBuffer& midiMessages, const int numSamples) {

	if (isOffAndShallBeOff() == true) return;

	ScopedNoDenormals noDenormals;
	
	modMatrixInputState inputState;

	//======================================

	int localWritePosition;

	float* bufferWritePointerL = buffer.getWritePointer(0);
	float* bufferWritePointerR = buffer.getWritePointer(1);
	float* delayData_left = delayBuffer.getWritePointer(0);
	float* delayData_right = delayBuffer.getWritePointer(1);

	localWritePosition = delayWritePosition;
	for (int currentFrame = 0; currentFrame < numSamples; currentFrame += 1) {
		int currentFrameOSAdjusted = currentFrame;
		if (m_bOversampling) currentFrameOSAdjusted /= C_OVERSAMPLING_RATIO;

		checkSoftFade();

		inputState = ((VASTAudioProcessor*)my_processor)->m_pVASTXperience.m_Poly.getOldestNotePlayedInputState(currentFrameOSAdjusted); // make parameter oldest or newest

		if (*m_bFlangerSynch == static_cast<int>(SWITCH::SWITCH_OFF))
			m_fFlangerLFOFreq_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_fFlangerLFOFreq, MODMATDEST::FlangerLFOFrequency, &inputState));
		if (m_fFlangerLFOFreq_smoothed.isSmoothing()) {
			m_LFO.startLFOFrequency(m_fFlangerLFOFreq_smoothed.getNextValue(), -1);
		}
		
		m_fFlangerDelay_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_fFlangerDelay, MODMATDEST::FlangerDelay, &inputState));
		float lFlangerDelay = m_fFlangerDelay_smoothed.getNextValue() * 0.001f;

		m_fFlangerWidth_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_fFlangerWidth, MODMATDEST::FlangerWidth, &inputState));
		float lFlangerWidth = m_fFlangerWidth_smoothed.getNextValue() * 0.001f;

		m_fFlangerFeedback_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_fFlangerFeedback, MODMATDEST::FlangerFeedback, &inputState));
		float lFlangerFeedback = m_fFlangerFeedback_smoothed.getNextValue();

		m_fFlangerDryWet_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_fFlangerDryWet, MODMATDEST::FlangerDryWet, &inputState));
		float lFlangerDryWet = m_fFlangerDryWet_smoothed.getNextValue();

		m_fFlangerGain_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_fFlangerGain, MODMATDEST::FlangerGain, &inputState));
		float lFlangerGain = m_fFlangerGain_smoothed.getNextValue();
		
		const float in_left = bufferWritePointerL[currentFrame];
		const float in_right = bufferWritePointerR[currentFrame];
		float out_left = 0.0f;
		float out_right = 0.0f;

		float fLFOOsc = 0.f;
		m_LFO.getOscillation(&fLFOOsc);
		float localDelayTime =
			(lFlangerDelay + lFlangerWidth * fLFOOsc) * (float)m_iSampleRate;

		float readPosition =
			fmodf((float)localWritePosition - localDelayTime + (float)delayBufferSamples, delayBufferSamples);
		int localReadPosition = floorf(readPosition);

		switch (2) { //HACK
		case interpolationNearestNeighbour: {
			float closestSample = delayData_left[localReadPosition % delayBufferSamples];
			out_left = closestSample;
			closestSample = delayData_right[localReadPosition % delayBufferSamples];
			out_right = closestSample;
			break;
		}
		case interpolationLinear: {
			float fraction = readPosition - (float)localReadPosition;
			float delayed0 = delayData_left[(localReadPosition + 0)];
			float delayed1 = delayData_left[(localReadPosition + 1) % delayBufferSamples];
			out_left = delayed0 + fraction * (delayed1 - delayed0);

			delayed0 = delayData_right[(localReadPosition + 0)];
			delayed1 = delayData_right[(localReadPosition + 1) % delayBufferSamples];
			out_right = delayed0 + fraction * (delayed1 - delayed0);
			break;
		}
		case interpolationCubic: {
			float fraction = readPosition - (float)localReadPosition;
			float fractionSqrt = fraction * fraction;
			float fractionCube = fractionSqrt * fraction;

			float sample0 = delayData_left[(localReadPosition - 1 + delayBufferSamples) % delayBufferSamples];
			float sample1 = delayData_left[(localReadPosition + 0)];
			float sample2 = delayData_left[(localReadPosition + 1) % delayBufferSamples];
			float sample3 = delayData_left[(localReadPosition + 2) % delayBufferSamples];

			float a0 = -0.5f * sample0 + 1.5f * sample1 - 1.5f * sample2 + 0.5f * sample3;
			float a1 = sample0 - 2.5f * sample1 + 2.0f * sample2 - 0.5f * sample3;
			float a2 = -0.5f * sample0 + 0.5f * sample2;
			float a3 = sample1;
			out_left = a0 * fractionCube + a1 * fractionSqrt + a2 * fraction + a3;

			sample0 = delayData_right[(localReadPosition - 1 + delayBufferSamples) % delayBufferSamples];
			sample1 = delayData_right[(localReadPosition + 0)];
			sample2 = delayData_right[(localReadPosition + 1) % delayBufferSamples];
			sample3 = delayData_right[(localReadPosition + 2) % delayBufferSamples];

			a0 = -0.5f * sample0 + 1.5f * sample1 - 1.5f * sample2 + 0.5f * sample3;
			a1 = sample0 - 2.5f * sample1 + 2.0f * sample2 - 0.5f * sample3;
			a2 = -0.5f * sample0 + 0.5f * sample2;
			a3 = sample1;
			out_right = a0 * fractionCube + a1 * fractionSqrt + a2 * fraction + a3;
			break;
		}
		}

		//NaN check
		if (isnan(out_left)) out_left = 0.0f;
		if (isnan(out_right)) out_right = 0.0f;

		float new_left = in_left + out_left;
		float new_right = in_right + out_right;
		
		//DryWet Mod
		float lMix = lFlangerDryWet * 0.01f * (float(m_iSoftFade) / float(C_MAX_SOFTFADE));
		bufferWritePointerL[currentFrame] = bufferWritePointerL[currentFrame] * (1 - lMix) + new_left * lMix;
		bufferWritePointerR[currentFrame] = bufferWritePointerR[currentFrame] * (1 - lMix) + new_right * lMix;
		bufferWritePointerL[currentFrame] *= lFlangerGain * 0.01f;
		bufferWritePointerR[currentFrame] *= lFlangerGain * 0.01f;

		delayData_left[localWritePosition] = in_left + out_left * lFlangerFeedback;
		delayData_right[localWritePosition] = in_right + out_right * lFlangerFeedback;

		if (++localWritePosition >= delayBufferSamples)
			localWritePosition -= delayBufferSamples;
	}

	delayWritePosition = localWritePosition;
}

//==============================================================================

void CVASTFlanger::getStateInformation(MemoryBlock& destData)
{
	//std::unique_ptr<XmlElement> xml (parameters.valueTreeState.state.createXml());
	//copyXmlToBinary (*xml, destData);
}

void CVASTFlanger::setStateInformation(const void* data, int sizeInBytes)
{
	//std::unique_ptr<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
	//if (xmlState != nullptr)
	//  if (xmlState->hasTagName (parameters.valueTreeState.state.getType()))
	//    parameters.valueTreeState.state = ValueTree::fromXml (*xmlState);
}
