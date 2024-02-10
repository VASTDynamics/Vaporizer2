/*
VAST Dynamics Audio Software (TM)

All modulators tested: OK
	- DelayTime is not good modulated! Took out.
*/

#include "../VASTEngineHeader.h"
#include "VASTStereoDelay.h"
#include "../VASTSettings.h"
#include "../../Plugin/VASTAudioProcessor.h"
#include "VASTEffect.h"

CVASTStereoDelay::CVASTStereoDelay(VASTAudioProcessor* processor, CVASTSettings& set, int busnr) : m_Set(&set) {
	my_processor = processor;
	myBusnr = busnr;

	m_bIsOff = true;
	m_bShallBeOff = true;
	m_iSoftFade = 0;

	setEffectName("STEREO DELAY");
}

void CVASTStereoDelay::initParameters() {
	AudioProcessorValueTreeState& parameters = my_processor->getParameterTree();

	createAndAddParameter(&m_bDelayOnOff, parameters, 1, "m_bDelayOnOff", "Delay on / off", "On", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 1.0f, 1.0f), 0.0f,
		CVASTParamState::toggleButtonValueToTextFunction,
		CVASTParamState::toggleButtonTextToValueFunction,
		false, true, true, true,
		true);
	createAndAddParameter(&m_fDelayWet, parameters, 1, "m_fDelayWet", "Delay dry / wet (no effect - full effect)", "DryWet", 1,
		MODMATDEST::DelayDryWet,
		NormalisableRange<float>(0, 100), 20.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);
	createAndAddParameter(&m_uDelayMode, parameters, 1, "m_uDelayMode", "Delay mode", "Mode", 2,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 2.0f, 1.0f), 0.0f,
		[](float value) { return StringArray("PingPong", "Cross", "Straight")[int(value)]; },
		[](String text) { return StringArray("PingPong", "Cross", "Straight").indexOf(StringRef(text), true); },
		false, true, true, false,
		true);
	createAndAddParameter(&m_fDelayLowcut, parameters, 1, "m_fDelayLowcut", "Delay lowcut frequency", "Lowcut", 3,
		MODMATDEST::DelayLowCut,
		NormalisableRange<float>(50.f, 18000.f, 0.001f, 0.3f, false), 50.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);
	createAndAddParameter(&m_fDelayFeedback, parameters, 1, "m_fDelayFeedback", "Delay feedback", "Feedback", 5,
		MODMATDEST::DelayFeedback,
		NormalisableRange<float>(0, 100), 40.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);
	createAndAddParameter(&m_bDelaySynch, parameters, 1, "m_bDelaySynch", "Delay sync to DAW", "Sync", 6,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 1.0f, 1.0f), 0.0f,
		CVASTParamState::toggleButtonValueToTextFunction,
		CVASTParamState::toggleButtonTextToValueFunction,
		false, true, true, false,
		true);
	createAndAddParameter(&m_uDelayTimeBeats, parameters, 1, "m_uDelayTimeBeats", "Delay time in beats when synced to DAW", "DAW", 7,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, TIMEBEATS_Array.size() - 1, 1.0f), TIMEBEATS::BEATS1,
		CVASTParamState::comboBoxValueToTextFunction_TIMEBEATS,
		CVASTParamState::comboBoxTextToValueFunction_TIMEBEATS,
		false, true, true, false,
		true);
	createAndAddParameter(&m_fDelayTime, parameters, 1, "m_fDelayTime", "Delay time in ms", "Time", 8,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0, 5000), 300.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);
	createAndAddParameter(&m_fDelayGain, parameters, 1, "m_fDelayGain", "Delay output gain", "Gain", 9,
		MODMATDEST::DelayGain,
		NormalisableRange<float>(0, 200), 100,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	// --- zero everything
	m_dDelayRatio = 0.0; // 1:1
}

void CVASTStereoDelay::initCompatibilityParameters() {
	// all new parameters go here
	AudioProcessorValueTreeState& parameters = my_processor->getParameterTree();
	createAndAddParameter(&m_fDelayHighcut, parameters, 1, "m_fDelayHighcut", "Delay highcut frequency", "Highcut", 4,
		MODMATDEST::DelayHighCut,
		NormalisableRange<float>(50.f, 18000.f, 0.001f, 0.3f, false), 18000.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);
}

void CVASTStereoDelay::parameterChanged(const String& parameterID, float newValue) {
	if (parameterID.startsWith("m_bDelayOnOff")) {
		if (newValue == static_cast<int>(SWITCH::SWITCH_ON)) {
			reset();
			switchOn();
		}
		else
			switchOff();
		return;
	}
	else if (parameterID.startsWith("m_uDelayMode")) {
		if (newValue == 0.0f) { //pingpong
			setMode(pingpong); 
			setDelayRatio(0.5); 
		}
		else if (newValue == 1.0f) { //cross
			setMode(pingpong);
			setDelayRatio(0.8); 
		}
		else if (newValue == 2.0f) { //straight
			setMode(pingpong);
			setDelayRatio(0.0); 
		}
		update();
	}
	else if (parameterID.startsWith("m_bDelaySynch")) {
		update();
	}
	else if (parameterID.startsWith("m_fDelayTime")) {
		//only if changed!!
		m_fDelayTime_smoothed.setTargetValue(newValue);
		update();
	}
	else if (parameterID.startsWith("m_fDelayDryWet")) {
		m_fDelayDryWet_smoothed.setTargetValue(newValue);
	}
	else if (parameterID.startsWith("m_fDelayGain")) {
		m_fDelayGain_smoothed.setTargetValue(newValue);
	}
	else if (parameterID.startsWith("m_fDelayFeedback")) {
		m_fDelayFeedback_smoothed.setTargetValue(newValue);
	}
	
	else if (parameterID.startsWith("m_uDelayTimeBeats")) {
		//only if changed!!
		//m_fDelayTime_smoothed.setTargetValue(*m_fDelayTime);
		update();
	}
	else if (parameterID.startsWith("m_fDelayLowcut")) {
		m_fDelayLowcut_smoothed.setTargetValue(newValue);
	}
	else if (parameterID.startsWith("m_fDelayHighcut")) {
		m_fDelayHighcut_smoothed.setTargetValue(newValue);
	}

}

CVASTStereoDelay::~CVASTStereoDelay(void) {
	AudioProcessorValueTreeState& parameters = ((VASTAudioProcessor*)my_processor)->getParameterTree();
	Array<AudioProcessorParameterWithID*> params = getParameters();
	for (int i = 0; i < params.size(); i++) {
		parameters.removeParameterListener(params[i]->paramID, (AudioProcessorValueTreeState::Listener*)this);
	}
}

void CVASTStereoDelay::updateTiming() {
	update();
}

void CVASTStereoDelay::releaseResources() {

}

void CVASTStereoDelay::init() {
	initParameters();

	m_LeftDelay.setSampleRate(m_iSampleRate);
	m_RightDelay.setSampleRate(m_iSampleRate);
	reset();
	setMode(pingpong); //pingpong
	setDelayRatio(0.5); //make configurable?
	
	m_fDrywetMod = 1.0f;
	m_fFeedbackMod = 1.0f;
	m_fTimeMod = 1.0f;
}

void CVASTStereoDelay::reset()
{
	// --- flush buffers and reset index values
	m_LeftDelay.resetDelay();
	m_RightDelay.resetDelay();

	m_lowCutBiquadL.flushDelays();
	m_lowCutBiquadR.flushDelays();
	m_highCutBiquadL.flushDelays();
	m_highCutBiquadR.flushDelays();

	if ((m_bIsOff == false) && (m_bShallBeOff == false)) {
		m_iSoftFade = 0;
		m_bShallBeOff = false; //refade in
		m_bIsOff = true;
	}
}

// --- one time init
void CVASTStereoDelay::prepareToPlay(double , int samplesPerBlock) {
	m_iExpectedSamplesPerBlock = samplesPerBlock;
	//m_iSampleRate is set in useOversampling

	// --- set sample rate first	
	m_LeftDelay.setSampleRate(m_iSampleRate);
	m_RightDelay.setSampleRate(m_iSampleRate);

	// --- initialize to 5 sec max delay
	m_LeftDelay.init(5.0 * m_iSampleRate);
	m_RightDelay.init(5.0 * m_iSampleRate);

	m_LeftDelay.setAPF_g(0.1f);
	m_RightDelay.setAPF_g(0.1f);

	// --- do the flush
	reset();
	
	float fQ = float(sqrt2over2);
	m_lowCutBiquadL.calcBiquad(CVASTBiQuad::HIGHPASS, *m_fDelayLowcut, m_iSampleRate, fQ, -18.0f);
	m_lowCutBiquadR.copySettingsFrom(&m_lowCutBiquadL);
	m_highCutBiquadL.calcBiquad(CVASTBiQuad::LOWPASS, *m_fDelayHighcut, m_iSampleRate, fQ, -18.0f);
	m_highCutBiquadR.copySettingsFrom(&m_highCutBiquadL);

	double smoothTime = 0.5;
	m_fDelayTime_smoothed.reset(m_iSampleRate, smoothTime);
	smoothTime = 0.02;
	m_fDelayDryWet_smoothed.reset(m_iSampleRate, smoothTime);
	m_fDelayGain_smoothed.reset(m_iSampleRate, smoothTime);
	m_fDelayFeedback_smoothed.reset(m_iSampleRate, smoothTime);
	m_fDelayLowcut_smoothed.reset(m_iSampleRate, smoothTime);
	m_fDelayHighcut_smoothed.reset(m_iSampleRate, smoothTime);
	
	m_fDelayTime_smoothed.setTargetValue(*m_fDelayTime);
	m_fTimeMod = *m_fDelayTime;
	update();
}

inline void CVASTStereoDelay::setMode(MYUINT u) { m_uMode = u; }

inline void CVASTStereoDelay::setDelayRatio(double d) { m_dDelayRatio = d; }

void CVASTStereoDelay::update() {
	float l_fDelayTime = 0.f;
	if (*m_bDelaySynch == static_cast<int>(SWITCH::SWITCH_OFF)) {
		l_fDelayTime = m_fTimeMod;
	}
	else { //bpm synch
		if (m_Set->m_dPpqBpm == 0.f) return;
		l_fDelayTime = m_Set->getIntervalTimeFromDAWBeats(*m_uDelayTimeBeats);
	}
	if (l_fDelayTime < 0.1f) l_fDelayTime = 0.1f; // minimum
	if (l_fDelayTime > 5000.0f) l_fDelayTime = 5000.f; // maximum

	if(m_dDelayRatio < 0)
	{
		// --- note negation of ratio!
		m_LeftDelay.setDelay_mSec(-m_dDelayRatio*l_fDelayTime);
		m_RightDelay.setDelay_mSec(l_fDelayTime);
	}
	else if(m_dDelayRatio > 0)
	{
		m_LeftDelay.setDelay_mSec(l_fDelayTime);
		m_RightDelay.setDelay_mSec(m_dDelayRatio*l_fDelayTime);
	}
	else
	{
		m_LeftDelay.setDelay_mSec(l_fDelayTime);
		m_RightDelay.setDelay_mSec(l_fDelayTime);
	}
}

void CVASTStereoDelay::processBlock(AudioSampleBuffer& buffer, MidiBuffer& , const int numSamples) {
	if (isOffAndShallBeOff() == true) return;

	modMatrixInputState inputState;
	float lDelayGain = 0.f;

	float* bufferWritePointerL = buffer.getWritePointer(0);
	float* bufferWritePointerR = buffer.getWritePointer(1);
	//for (int currentFrameOSAdjusted = 0; currentFrameOSAdjusted < buffer.getNumSamples() * m_Set->m_nOversamplingRatio; currentFrameOSAdjusted += m_Set->m_nOversamplingRatio) {
	for (int currentFrame = 0; currentFrame < numSamples; currentFrame += 1) {
		int currentFrameOSAdjusted = currentFrame;
		if (m_bOversampling) currentFrameOSAdjusted /= C_OVERSAMPLING_RATIO;
		
		checkSoftFade();

		inputState = ((VASTAudioProcessor*)my_processor)->m_pVASTXperience.m_Poly.getOldestNotePlayedInputState(currentFrameOSAdjusted); // make parameter oldest or newest

		m_fTimeMod = m_fDelayTime_smoothed.getNextValue();
		if (m_fDelayTime_smoothed.isSmoothing())
			update();

		m_fDelayFeedback_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_fDelayFeedback, MODMATDEST::DelayFeedback, &inputState));
		m_fFeedbackMod = m_fDelayFeedback_smoothed.getNextValue();

		m_fDelayDryWet_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_fDelayWet, MODMATDEST::DelayDryWet, &inputState));
		m_fDrywetMod = m_fDelayDryWet_smoothed.getNextValue();

		m_fDelayGain_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_fDelayGain, MODMATDEST::DelayGain, &inputState));
		lDelayGain = m_fDelayGain_smoothed.getNextValue();

		//Lowcut Mod
		m_fDelayLowcut_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_fDelayLowcut, MODMATDEST::DelayLowCut, &inputState));
		if (m_fDelayLowcut_smoothed.isSmoothing()) {
			float lDelayLowcut = m_fDelayLowcut_smoothed.getNextValue();
			float fQ = float(sqrt2over2);
			m_lowCutBiquadL.calcBiquad(CVASTBiQuad::HIGHPASS, lDelayLowcut, m_iSampleRate, fQ, -18.0f);
			m_lowCutBiquadR.copySettingsFrom(&m_lowCutBiquadL);
		}

		//Highcut Mod
		m_fDelayHighcut_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_fDelayHighcut, MODMATDEST::DelayHighCut, &inputState));
		if (m_fDelayHighcut_smoothed.isSmoothing()) {
			float lDelayHighcut = m_fDelayHighcut_smoothed.getNextValue();
			float fQ = float(sqrt2over2);
			m_highCutBiquadL.calcBiquad(CVASTBiQuad::LOWPASS, lDelayHighcut, m_iSampleRate, fQ, -18.0f);
			m_highCutBiquadR.copySettingsFrom(&m_highCutBiquadL);
		}

		float fIn[2];
		float fInUnfiltered[2];
		float fOut[2];
		fIn[0] = bufferWritePointerL[currentFrame];
		fIn[1] = bufferWritePointerR[currentFrame];
		fInUnfiltered[0] = bufferWritePointerL[currentFrame];
		fInUnfiltered[1] = bufferWritePointerR[currentFrame];

		fIn[0] = m_lowCutBiquadL.doBiQuad(fIn[0]); //do pre filter
		fIn[1] = m_lowCutBiquadR.doBiQuad(fIn[1]); //do pre filter
		fIn[0] = m_highCutBiquadL.doBiQuad(fIn[0]); //do pre filter
		fIn[1] = m_highCutBiquadR.doBiQuad(fIn[1]); //do pre filter

		processAudioFrame(fIn, fInUnfiltered, fOut, 2, 2);
		
		bufferWritePointerL[currentFrame] = fOut[0];
		bufferWritePointerR[currentFrame] = fOut[1];
		//vassert((fOut[0] > -3.0f) && (fOut[0] <= 3.0f));
		//vassert((fOut[1] > -3.0f) && (fOut[1] <= 3.0f));
	}
	buffer.applyGain(lDelayGain * 0.01f);
}

bool CVASTStereoDelay::processAudioFrame(float* pInputBuffer, float* pInputBufferUnfiltered, float* pOutputBuffer, MYUINT , MYUINT )
{	
	// --- do the delays
	//     common components:
	float dLeftDelayOut = m_LeftDelay.readDelay();
	float dRightDelayOut = m_RightDelay.readDelay();

	float dLeftDelayIn = 0.f;
	float dRightDelayIn = 0.f;

	// --- do the other modes:
	switch(int(*m_uDelayMode))
	{
		// --- NOTE: cross mode sounds identical to ping-pong if
		//     the input is mono (same signal applied to both)
		case cross:
		{
			// Left Input -> Left Delay; Left Feedback -> Right Delay
			// Right Input -> Right Delay; Right Feedback -> Left Delay
			dLeftDelayIn = pInputBuffer[0] + dRightDelayOut*(m_fFeedbackMod * 0.01f);
			dRightDelayIn = pInputBuffer[1] + dLeftDelayOut*(m_fFeedbackMod * 0.01f);
			break;
		}
		case pingpong:
		{
			// Left Input -> Right Delay; Left Feedback -> Right Delay
			// Right Input -> Left Delay; Right Feedback -> Left Delay
			dLeftDelayIn = pInputBuffer[1] + dRightDelayOut*(m_fFeedbackMod * 0.01f);
			dRightDelayIn = pInputBuffer[0] + dLeftDelayOut*(m_fFeedbackMod * 0.01f);
			break;
		}
		case straight:
		{
			// Left Input -> Right Delay; Left Feedback -> Left Delay
			// Right Input -> Left Delay; Right Feedback -> Right Delay
			dLeftDelayIn = pInputBuffer[0] + dLeftDelayOut * (m_fFeedbackMod * 0.01f);
			dRightDelayIn = pInputBuffer[1] + dRightDelayOut * (m_fFeedbackMod * 0.01f);
			break;
		}
	}

	// --- intermediate variables
	float dLeftOut = 0.0;
	float dRightOut = 0.0;

	// --- do the delay lines
	m_LeftDelay.processAudio(&dLeftDelayIn, &dLeftOut);
	m_RightDelay.processAudio(&dRightDelayIn, &dRightOut);

	// --- form outputs
	pOutputBuffer[0] = pInputBufferUnfiltered[0] + (float(m_iSoftFade) / float(C_MAX_SOFTFADE)) * m_fDrywetMod * 0.01f * dLeftOut;
	pOutputBuffer[1] = pInputBufferUnfiltered[1] + (float(m_iSoftFade) / float(C_MAX_SOFTFADE)) * m_fDrywetMod * 0.01f * dRightOut;

	// --- return All OK
	return true;
}

//==============================================================================

void CVASTStereoDelay::getStateInformation(MemoryBlock& )
{
	//std::unique_ptr<XmlElement> xml (parameters.valueTreeState.state.createXml());
	//copyXmlToBinary (*xml, destData);
}

void CVASTStereoDelay::setStateInformation(const void* , int )
{
	//std::unique_ptr<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
	//if (xmlState != nullptr)
	//  if (xmlState->hasTagName (parameters.valueTreeState.state.getType()))
	//    parameters.valueTreeState.state = ValueTree::fromXml (*xmlState);
}
