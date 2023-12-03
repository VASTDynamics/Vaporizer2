/*	
VAST Dynamics Audio Software (TM)

All modulators tested: OK
*/

#include "VASTChorus.h"
#include "../VASTEngineHeader.h"
#include "../VASTSettings.h"
#include "../../Plugin/VASTAudioProcessor.h"
#include "VASTEffect.h"

CVASTChorus::CVASTChorus(VASTAudioProcessor* processor, CVASTSettings& set, int busnr) : m_Set(&set),
		m_ModDelayLeft(set),
		m_ModDelayCenter(set),
		m_ModDelayRight(set)
{
	my_processor = processor;
	myBusnr = busnr;
	setEffectName("CHORUS");

	m_bIsOff = true;
	m_bShallBeOff = true;
	m_iSoftFade = 0;

}

void CVASTChorus::initParameters() {
	AudioProcessorValueTreeState& parameters = my_processor->getParameterTree();

	createAndAddParameter(&m_bChorusOnOff, parameters, 1, "m_bChorusOnOff", "Chrorus effect on / off", "On", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 1.0f, 1.0f), 0.0f,
		CVASTParamState::toggleButtonValueToTextFunction,
		CVASTParamState::toggleButtonTextToValueFunction,
		false, true, true, true,
		true);

	createAndAddParameter(&m_fChorusDryWet, parameters, 1, "m_fChorusDryWet", "Chorus dry / wet (no effect - full effect)", "DryWet", 1,
		MODMATDEST::ChorusDryWet,
		NormalisableRange<float>(0, 100), 100.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);
	createAndAddParameter(&m_fChorusDepth, parameters, 1, "m_fChorusDepth", "Chorus depth", "Depth", 2,
		MODMATDEST::ChorusDepth,
		NormalisableRange<float>(0, 100), 100.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);
	createAndAddParameter(&m_bChorusSynch, parameters, 1, "m_bChorusSynch", "Chorus sync to DAW", "Sync", 3,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 1.0f, 1.0f), 0.0f,
		CVASTParamState::toggleButtonValueToTextFunction,
		CVASTParamState::toggleButtonTextToValueFunction,
		false, true, true, false,
		true);
	createAndAddParameter(&m_uChorusTimeBeats, parameters, 1, "m_uChorusTimeBeats", "Chorus time in beats when synced to DAW", "DAW", 4,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, TIMEBEATS_Array.size() - 1, 1.0f), TIMEBEATS::BEATS1,
		CVASTParamState::comboBoxValueToTextFunction_TIMEBEATS,
		CVASTParamState::comboBoxTextToValueFunction_TIMEBEATS,
		false, true, true, false,
		true);
	createAndAddParameter(&m_fChorusRate_hz, parameters, 1, "m_fChorusRate_hz", "Chorus frequency in hz (is spread amog the 6 chorus oscillators - left, mid, right in stereo)", "Frequency", 5,
		MODMATDEST::ChorusFrequency,
		NormalisableRange<float>(0.f, 10.f, 0.0001f, 0.3f, false), 0.10f, //skew
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);
	createAndAddParameter(&m_fChorusGain, parameters, 1, "m_fChorusGain", "Chorus output gain", "Gain", 6,
		MODMATDEST::ChorusGain,
		NormalisableRange<float>(0, 200), 100,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);
}

CVASTChorus::~CVASTChorus(void) {
	AudioProcessorValueTreeState& parameters = ((VASTAudioProcessor*)my_processor)->getParameterTree();
	Array<AudioProcessorParameterWithID*> params = getParameters();
	for (int i = 0; i < params.size(); i++) {
		parameters.removeParameterListener(params[i]->paramID, (AudioProcessorValueTreeState::Listener*)this);
	}
}

void CVASTChorus::initCompatibilityParameters() {
	// all new parameters go here
}

void CVASTChorus::init() {
	initParameters();

	m_fCrossMix = 45.000000; //not per UI?
	m_fHPF_Fc = 2000.000000; //not per UI?

	m_ModDelayLeft.init();
	m_ModDelayCenter.init();
	m_ModDelayRight.init();

	updateLFOFreq();

	reset();

	if ((m_bIsOff == false) && (m_bShallBeOff == false)) {
		m_iSoftFade = 0;
		m_bShallBeOff = false; //refade in
		m_bIsOff = true;
	}
}

void CVASTChorus::updateTiming() {
	updateLFOFreq();
}

void CVASTChorus::parameterChanged(const String& parameterID, float newValue) {
	if (parameterID.startsWith("m_bChorusOnOff")) {
		if (newValue == static_cast<int>(SWITCH::SWITCH_ON)) {
			switchOn();
		}
		else {
			switchOff();
		}
	}
	else if (parameterID.startsWith("m_fChorusGain")) {
		m_fChorusGain_smoothed.setTargetValue(newValue);
	}
	else if (parameterID.startsWith("m_fChorusDryWet")) {
		m_fChorusDryWet_smoothed.setTargetValue(newValue);
	}
	else if (parameterID.startsWith("m_fChorusDepth")) {
		m_fChorusDepth_smoothed.setTargetValue(newValue);
	}
	else if (parameterID.startsWith("m_fChorusRate_hz")) {
		updateLFOFreq();
	}
	else if (parameterID.startsWith("m_bChorusSynch")) {
		updateLFOFreq();
	}
	else if (parameterID.startsWith("m_uChorusTimeBeats")) {
		updateLFOFreq();
	}
	else if (parameterID.startsWith("m_fChorusDepth")) {
		updateModules();
	}
}

void CVASTChorus::updateLFOFreq() {
	if (*m_bChorusSynch == static_cast<int>(SWITCH::SWITCH_OFF)) {
		m_fChorusRate_hz_smoothed.setTargetValue(*m_fChorusRate_hz); //  *m_fTimeMod;
	}
	else { //bpm synch
		float l_fIntervalTime = 0.f;
		if (m_Set->m_dPpqBpm == 0.f) return;
		l_fIntervalTime = m_Set->getIntervalTimeFromDAWBeats(*m_uChorusTimeBeats);
		if (l_fIntervalTime < 0.1f) l_fIntervalTime = 0.1f; // minimum
		//if (l_fIntervalTime > 5000.0f) l_fIntervalTime = 5000.f; // maximum
		if (l_fIntervalTime > 100000.0f) l_fIntervalTime = 100000.0f; // maximum  //CHTS 3.0.1
		
		m_fChorusRate_hz_smoothed.setCurrentAndTargetValue(1); //reset it
		m_fChorusRate_hz_smoothed.setTargetValue(1.0f / (l_fIntervalTime / 1000.f));
	}
}

void CVASTChorus::updateModules()
{
	// setup quad phase
	m_ModDelayLeft.m_uLFOPhase = 1;  // 1: quad phase
	m_ModDelayCenter.m_uLFOPhase = 0; // 0: normal
	m_ModDelayRight.m_uLFOPhase = 0; // 

	m_ModDelayLeft.m_LFO.m_bInvert = false;
	m_ModDelayCenter.m_LFO.m_bInvert = false;
	m_ModDelayRight.m_LFO.m_bInvert = true; // this one is inverted

	//m_ModDelayLeft.m_fModDepth_pct = m_fModDepth_pct_L;
	//m_ModDelayCenter.m_fModDepth_pct = m_fModDepth_pct_C;
	//m_ModDelayRight.m_fModDepth_pct = m_fModDepth_pct_R;
	m_ModDelayLeft.m_fModDepth_pct = m_fDepth;
	m_ModDelayCenter.m_fModDepth_pct = m_fDepth;
	m_ModDelayRight.m_fModDepth_pct = m_fDepth;

	//m_ModDelayLeft.m_fModFrequency_Hz = m_fModFrequency_Hz_L;
	//m_ModDelayCenter.m_fModFrequency_Hz = m_fModFrequency_Hz_C;
	//m_ModDelayRight.m_fModFrequency_Hz = m_fModFrequency_Hz_R;
	m_ModDelayLeft.m_fModFrequency_Hz = m_fFrequency;
	m_ModDelayCenter.m_fModFrequency_Hz = m_fFrequency + 0.01f;
	m_ModDelayRight.m_fModFrequency_Hz = m_fFrequency + 0.02f;

	//m_ModDelayLeft.m_fFeedback_pct = m_fFeedback_pct_L;
	//m_ModDelayCenter.m_fFeedback_pct = m_fFeedback_pct_C;
	//m_ModDelayRight.m_fFeedback_pct = m_fFeedback_pct_R;
	m_ModDelayLeft.m_fFeedback_pct = m_fDepth / 2.0;
	m_ModDelayCenter.m_fFeedback_pct = m_fDepth / 3.0;
	m_ModDelayRight.m_fFeedback_pct = m_fDepth / 2.0;

	// CHORUS!
	m_ModDelayLeft.m_uModType = 2;
	m_ModDelayCenter.m_uModType = 2;
	m_ModDelayRight.m_uModType = 2;
	
	m_ModDelayLeft.m_uLFOType = WAVE::tri; // triangle
	m_ModDelayCenter.m_uLFOType = WAVE::tri;// triangle
	m_ModDelayRight.m_uLFOType = WAVE::tri;// triangle
	
	// cook them
	m_ModDelayLeft.updateLFO();
	m_ModDelayCenter.updateLFO();
	m_ModDelayRight.updateLFO();

	m_ModDelayLeft.updateDDL();
	m_ModDelayCenter.updateDDL();
	m_ModDelayRight.updateDDL();

}

void CVASTChorus::prepareToPlay(double, int samplesPerBlock) {	
	//m_iSampleRate is set in use oversampling
	m_iExpectedSamplesPerBlock = samplesPerBlock;
	m_ModDelayLeft.prepareForPlay(m_iSampleRate, m_bOversampling);
	m_ModDelayCenter.prepareForPlay(m_iSampleRate, m_bOversampling);
	m_ModDelayRight.prepareForPlay(m_iSampleRate, m_bOversampling);

	const double smoothTime = 0.02;  
	m_fChorusRate_hz_smoothed.reset(m_iSampleRate, smoothTime);	
	m_fChorusDepth_smoothed.reset(m_iSampleRate, smoothTime);
	m_fChorusDryWet_smoothed.reset(m_iSampleRate, smoothTime);
	m_fChorusGain_smoothed.reset(m_iSampleRate, smoothTime);

	// dont leave this out it inits and cooks
	updateModules();

	reset();
}

void CVASTChorus::reset() {
	m_ModDelayLeft.prepareForPlay(m_iSampleRate, m_bOversampling);
	m_ModDelayCenter.prepareForPlay(m_iSampleRate, m_bOversampling);
	m_ModDelayRight.prepareForPlay(m_iSampleRate, m_bOversampling);

	m_fFrequency = 1.0f;
	m_fDepth = 1.0f;
	m_fDrywetMod = 1.0f;

	if ((m_bIsOff == false) && (m_bShallBeOff == false)) {
		m_iSoftFade = 0;
		m_bShallBeOff = false; //refade in
		m_bIsOff = true;
	}

	updateModules();
}

void CVASTChorus::releaseResources() {
}

void CVASTChorus::processBlock(AudioSampleBuffer& buffer, MidiBuffer&, const int numSamples) {
	if (isOffAndShallBeOff() == true) return;

	modMatrixInputState inputState;

	float* bufferWritePointerL = buffer.getWritePointer(0);
	float* bufferWritePointerR = buffer.getWritePointer(1);

	for (int currentFrame = 0; currentFrame < numSamples; currentFrame += 1) {
		int currentFrameOSAdjusted = currentFrame;
		if (m_bOversampling) currentFrameOSAdjusted /= C_OVERSAMPLING_RATIO;		
		checkSoftFade();

		inputState = ((VASTAudioProcessor*)my_processor)->m_pVASTXperience.m_Poly.getOldestNotePlayedInputState(currentFrameOSAdjusted); // make parameter oldest or newest

		//Frequency Mod
		if (*m_bChorusSynch == static_cast<int>(SWITCH::SWITCH_OFF))
			m_fChorusRate_hz_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_fChorusRate_hz, MODMATDEST::ChorusFrequency, &inputState));
		bool bUpdate = false;
		if (m_fFrequency != m_fChorusRate_hz_smoothed.getNextValue()) {
			m_fFrequency = m_fChorusRate_hz_smoothed.getNextValue();
			bUpdate = true;
		}

		//Depth Mod
		m_fChorusDepth_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_fChorusDepth, MODMATDEST::ChorusDepth, &inputState));
		if (m_fDepth != m_fChorusDepth_smoothed.getNextValue()) { //max is 100
			m_fDepth = m_fChorusDepth_smoothed.getNextValue();
			bUpdate = true;
		}

		//DryWet Mod
		m_fChorusDryWet_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_fChorusDryWet, MODMATDEST::ChorusDryWet, &inputState));
		m_fDrywetMod = m_fChorusDryWet_smoothed.getNextValue() * 0.01f;

		//Gain Mod
		m_fChorusGain_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_fChorusGain, MODMATDEST::ChorusGain, &inputState));
		m_fGain = m_fChorusGain_smoothed.getNextValue();				

		if (m_fChorusRate_hz_smoothed.isSmoothing() || m_fChorusDepth_smoothed.isSmoothing() || bUpdate)
			updateModules();

		float fIn[2]{ bufferWritePointerL[currentFrame] , bufferWritePointerR[currentFrame] };
		float fOut[2]{ 0.f,0.f };
		
		//do chorus
		float fChorusOut_L = 0;
		float fChorusOut_C = 0;
		float fChorusOut_R = 0;

		float fLeftIn = fIn[0];
		float fRightIn = fIn[1];

		// form the center channel
		float fCenterIn = 0.5*fLeftIn + 0.5*fRightIn;

		// call the processAudioFrame()
		m_ModDelayLeft.processAudioFrame(&fLeftIn, &fChorusOut_L, 1, 1);
		m_ModDelayCenter.processAudioFrame(&fCenterIn, &fChorusOut_C, 1, 1);
		m_ModDelayRight.processAudioFrame(&fRightIn, &fChorusOut_R, 1, 1);

		//attenuate - otherwise to loud
		fChorusOut_L *= 0.8f;
		fChorusOut_C *= 0.1f;
		fChorusOut_R *= 0.8f;

		// add into mix
		float fSoftmix = m_fDrywetMod * (float(m_iSoftFade) / float(C_MAX_SOFTFADE));
		fOut[0] = (1.0f - fSoftmix) * fIn[0] + fSoftmix * (fChorusOut_L + fChorusOut_C);
		fOut[1] = (1.0f - fSoftmix) * fIn[1] + fSoftmix * (fChorusOut_R + fChorusOut_C);
			   		 	  	  	   	
		bufferWritePointerL[currentFrame] = fOut[0] * m_fGain * 0.01f;
		bufferWritePointerR[currentFrame] = fOut[1] * m_fGain * 0.01f;
		//vassert((fOut[0] > -10.0f) && (fOut[0] <= 10.0));
		//vassert((fOut[1] > -10.0f) && (fOut[1] <= 10.0));
	}
}

//==============================================================================

void CVASTChorus::getStateInformation(MemoryBlock&)
{
	//std::unique_ptr<XmlElement> xml (parameters.valueTreeState.state.createXml());
	//copyXmlToBinary (*xml, destData);
}

void CVASTChorus::setStateInformation(const void*, int)
{
	//std::unique_ptr<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
	//if (xmlState != nullptr)
	//  if (xmlState->hasTagName (parameters.valueTreeState.state.getType()))
	//    parameters.valueTreeState.state = ValueTree::fromXml (*xmlState);
}
