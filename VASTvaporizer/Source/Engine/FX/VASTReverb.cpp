/*
VAST Dynamics Audio Software (TM)

All modulators tested: OK
*/

#include "VASTReverb.h"
#include "../VASTSettings.h"
#include "../VASTEngineHeader.h"
#include "../../Plugin/VASTAudioProcessor.h"
#include "VASTEffect.h"

CVASTReverb::CVASTReverb(VASTAudioProcessor* processor, CVASTSettings& set, int busnr) : m_Set(&set) {
	my_processor = processor;
	myBusnr = busnr;

	m_bIsOff = true;
	m_bShallBeOff = true;
	m_iSoftFade = 0;

	setEffectName("REVERB");
}

void CVASTReverb::initParameters() {
	AudioProcessorValueTreeState& parameters = my_processor->getParameterTree();
	createAndAddParameter(&m_bReverbOnOff, parameters, 1, "m_bReverbOnOff", "Reverb on / off", "On", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 1.0f, 1.0f), 0.0f,
		CVASTParamState::toggleButtonValueToTextFunction,
		CVASTParamState::toggleButtonTextToValueFunction,
		false, true, true, true,
		true);
	createAndAddParameter(&m_fReverbDryWet, parameters, 1, "m_fReverbDryWet", "Reverb dry / wet (no effect - full effect)", "DryWet", 1,
		MODMATDEST::ReverbDryWet,
		NormalisableRange<float>(0, 100), 10.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true); 
	
	createAndAddParameter(&m_uReverbMode, parameters, 1, "m_uReverbMode", "Mode", "Mode", 2,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 1.0f, 1.0f), 1.0f,
		[](float value) { return StringArray("Room", "Plate")[int(value)]; },
		[](String text) { return StringArray("Room", "Plate").indexOf(StringRef(text), true); },
		false, true, true, false,
		true);	
	createAndAddParameter(&m_fReverbLowcut, parameters, 1, "m_fReverbLowcut", "Reverb lowcut frequency", "Lowcut", 3,
		MODMATDEST::ReverbLowcut,
		NormalisableRange<float>(50.f, 18000.f, 0.001f, 0.3f, false), 100.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);
	createAndAddParameter(&m_fReverbPredelay, parameters, 1, "m_fReverbPredelay", "Reverb predelay (ms)", "Predelay", 5,
		MODMATDEST::ReverbPredelay,
		NormalisableRange<float>(0.f, 1000.f), 100.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);
	createAndAddParameter(&m_fReverbSize, parameters, 1, "m_fReverbSize", "Reverb size - 0 is small room , 100 is plate / cathedral", "Size", 6,
		MODMATDEST::ReverbSize,
		NormalisableRange<float>(0, 100), 100.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);
	createAndAddParameter(&m_fReverbDamping, parameters, 1, "m_fReverbDamping", "Reverb damping", "Damping", 7,
		MODMATDEST::ReverbDamping,
		NormalisableRange<float>(0, 100), 10.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);
	createAndAddParameter(&m_fReverbFeedback, parameters, 1, "m_fReverbFeedback", "Reverb feedback", "Feedback", 8,
		MODMATDEST::ReverbFeedback,
		NormalisableRange<float>(0, 5000), 2000.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);
	createAndAddParameter(&m_fReverbGain, parameters, 1, "m_fReverbGain", "Reverb output gain", "Gain", 9,
		MODMATDEST::ReverbGain,
		NormalisableRange<float>(0, 200), 100,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);
}

void CVASTReverb::initCompatibilityParameters() {
	// all new parameters go here
	AudioProcessorValueTreeState& parameters = my_processor->getParameterTree();
	createAndAddParameter(&m_fReverbHighcut, parameters, 1, "m_fReverbHighcut", "Reverb highcut frequency", "Highcut", 4,
		MODMATDEST::ReverbHighcut,
		NormalisableRange<float>(50.f, 18000.f, 0.001f, 0.3f, false), 18000.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);
}

void CVASTReverb::parameterChanged(const String& parameterID, float newValue) {
	if (parameterID.startsWith("m_bReverbOnOff")) {
		if (newValue == static_cast<int>(SWITCH::SWITCH_ON)) {
			reset();
			switchOn();
		}
		else
			switchOff();
	}
	// reverb only effects reverb
	else if (parameterID.startsWith("m_uReverbMode")) {
		updateVariables();
	}
	else if (parameterID.startsWith("m_fReverbDamping")) {
		m_fReverbDamping_smoothed.setTargetValue(newValue);
	}
	else if (parameterID.startsWith("m_fReverbSize")) {
		m_fReverbSize_smoothed.setTargetValue(newValue);
	}
	else if (parameterID.startsWith("m_fReverbPredelay")) {
		m_fReverbPredelay_smoothed.setTargetValue(newValue);
	}
	else if (parameterID.startsWith("m_fReverbLowcut")) {
		m_fReverbLowcut_smoothed.setTargetValue(newValue);
	}
	else if (parameterID.startsWith("m_fReverbHighcut")) {
		m_fReverbHighcut_smoothed.setTargetValue(newValue);
	}
	else if (parameterID.startsWith("m_fReverbFeedback")) {
		m_fReverbFeedback_smoothed.setTargetValue(newValue);
	}
	else if (parameterID.startsWith("m_fReverbGain")) {
		m_fReverbGain_smoothed.setTargetValue(newValue);
	}
}


CVASTReverb::~CVASTReverb(void) {
	AudioProcessorValueTreeState& parameters = ((VASTAudioProcessor*)my_processor)->getParameterTree();
	Array<AudioProcessorParameterWithID*> params = getParameters();
	for (int i = 0; i < params.size(); i++) {
		parameters.removeParameterListener(params[i]->paramID, (AudioProcessorValueTreeState::Listener*)this);
	}
}

void CVASTReverb::releaseResources() {

}

void CVASTReverb::init() {
	initParameters();

	// up to 2 seconds predelay
	m_PreDelay.setSampleRate(m_iSampleRate);
	m_PreDelay.init(2.0*(m_iSampleRate));
	
	// init up to 100 mSec
	m_InputAPF_1.setSampleRate(m_iSampleRate);
	m_InputAPF_1.init(0.1*(m_iSampleRate));
	m_InputAPF_2.setSampleRate(m_iSampleRate);
	m_InputAPF_2.init(0.1*(m_iSampleRate));

	// 100 mSec each max
	m_ParallelCF_1.setSampleRate(m_iSampleRate);
	m_ParallelCF_1.init(0.1*(m_iSampleRate));
	m_ParallelCF_2.setSampleRate(m_iSampleRate);
	m_ParallelCF_2.init(0.1*(m_iSampleRate));
	m_ParallelCF_3.setSampleRate(m_iSampleRate);
	m_ParallelCF_3.init(0.1*(m_iSampleRate));
	m_ParallelCF_4.setSampleRate(m_iSampleRate);
	m_ParallelCF_4.init(0.1*(m_iSampleRate));
	m_ParallelCF_5.setSampleRate(m_iSampleRate);
	m_ParallelCF_5.init(0.1*(m_iSampleRate));
	m_ParallelCF_6.setSampleRate(m_iSampleRate);
	m_ParallelCF_6.init(0.1*(m_iSampleRate));
	m_ParallelCF_7.setSampleRate(m_iSampleRate);
	m_ParallelCF_7.init(0.1*(m_iSampleRate));
	m_ParallelCF_8.setSampleRate(m_iSampleRate);
	m_ParallelCF_8.init(0.1*(m_iSampleRate));

	// 100 mSec each max
	m_OutputAPF_3.setSampleRate(m_iSampleRate);
	m_OutputAPF_3.init(0.1*(m_iSampleRate));
	m_OutputAPF_4.setSampleRate(m_iSampleRate);
	m_OutputAPF_4.init(0.1*(m_iSampleRate));

	reset();
}

// --- one time init
void CVASTReverb::prepareToPlay(double , int samplesPerBlock) {
	m_iExpectedSamplesPerBlock = samplesPerBlock;
	//m_iSampleRate is set in useOversampling
	// up to 2 seconds predelay

	const double smoothTime = 0.02;  //check 1e-3?
	m_fReverbGain_smoothed.reset(m_iSampleRate, smoothTime);
	m_fReverbDryWet_smoothed.reset(m_iSampleRate, smoothTime);
	m_fReverbFeedback_smoothed.reset(m_iSampleRate, smoothTime);
	m_fReverbSize_smoothed.reset(m_iSampleRate, smoothTime);
	m_fReverbPredelay_smoothed.reset(m_iSampleRate, smoothTime);
	m_fReverbLowcut_smoothed.reset(m_iSampleRate, smoothTime);
	m_fReverbHighcut_smoothed.reset(m_iSampleRate, smoothTime);
	m_fReverbDamping_smoothed.reset(m_iSampleRate, smoothTime);

	float fQ = 0.707f;
	m_lowCutBiquadL.calcBiquad(CVASTBiQuad::HIGHPASS, *m_fReverbLowcut, m_iSampleRate, fQ, -18.0f);
	m_lowCutBiquadR.copySettingsFrom(&m_lowCutBiquadL);
	m_lowCutBiquad1.copySettingsFrom(&m_lowCutBiquadL);

	m_highCutBiquadL.calcBiquad(CVASTBiQuad::LOWPASS, *m_fReverbHighcut, m_iSampleRate, fQ, -18.0f);
	m_highCutBiquadR.copySettingsFrom(&m_highCutBiquadL);
	m_highCutBiquad1.copySettingsFrom(&m_highCutBiquadL);

	m_PreDelay.setSampleRate(m_iSampleRate);
	m_PreDelay.init(2.0 * m_iSampleRate);

	m_fReverbSize_smoothed.setTargetValue(*m_fReverbSize);
	m_fSizeMod = *m_fReverbSize;
	m_fReverbPredelay_smoothed.setTargetValue(*m_fReverbPredelay);
	m_fPredelayMod = *m_fReverbPredelay;
	m_fReverbDamping_smoothed.setTargetValue(*m_fReverbDamping);
	m_fDampingMod = *m_fReverbDamping;
	m_fReverbFeedback_smoothed.setTargetValue(*m_fReverbFeedback);
	m_fFeedbackMod = *m_fReverbFeedback;

	// init up to 100 mSec
	m_InputAPF_1.setSampleRate(m_iSampleRate);
	m_InputAPF_1.init(0.1 * m_iSampleRate);
	m_InputAPF_2.setSampleRate(m_iSampleRate);
	m_InputAPF_2.init(0.1 * m_iSampleRate);

	// 100 mSec each max
	m_ParallelCF_1.setSampleRate(m_iSampleRate);
	m_ParallelCF_1.init(0.1* m_iSampleRate);
	m_ParallelCF_2.setSampleRate(m_iSampleRate);
	m_ParallelCF_2.init(0.1* m_iSampleRate);
	m_ParallelCF_3.setSampleRate(m_iSampleRate);
	m_ParallelCF_3.init(0.1 * m_iSampleRate);
	m_ParallelCF_4.setSampleRate(m_iSampleRate);
	m_ParallelCF_4.init(0.1 * m_iSampleRate);
	m_ParallelCF_5.setSampleRate(m_iSampleRate);
	m_ParallelCF_5.init(0.1 * m_iSampleRate);
	m_ParallelCF_6.setSampleRate(m_iSampleRate);
	m_ParallelCF_6.init(0.1 * m_iSampleRate);
	m_ParallelCF_7.setSampleRate(m_iSampleRate);
	m_ParallelCF_7.init(0.1 * m_iSampleRate);
	m_ParallelCF_8.setSampleRate(m_iSampleRate);
	m_ParallelCF_8.init(0.1 * m_iSampleRate);

	// 100 mSec each max
	m_OutputAPF_3.setSampleRate(m_iSampleRate);
	m_OutputAPF_3.init(0.1*(m_iSampleRate));
	m_OutputAPF_4.setSampleRate(m_iSampleRate);
	m_OutputAPF_4.init(0.1*(m_iSampleRate));

	/*
	// up to 2 seconds predelay
	m_PreDelay.setSampleRate(m_iSampleRate);

	// init up to 100 mSec
	m_InputAPF_1.setSampleRate(m_iSampleRate);
	m_InputAPF_2.setSampleRate(m_iSampleRate);

	// 100 mSec each max
	m_ParallelCF_1.setSampleRate(m_iSampleRate);
	m_ParallelCF_2.setSampleRate(m_iSampleRate);
	m_ParallelCF_3.setSampleRate(m_iSampleRate);
	m_ParallelCF_4.setSampleRate(m_iSampleRate);
	m_ParallelCF_5.setSampleRate(m_iSampleRate);
	m_ParallelCF_6.setSampleRate(m_iSampleRate);
	m_ParallelCF_7.setSampleRate(m_iSampleRate);
	m_ParallelCF_8.setSampleRate(m_iSampleRate);

	// 100 mSec each max
	m_OutputAPF_3.setSampleRate(m_iSampleRate);
	m_OutputAPF_4.setSampleRate(m_iSampleRate);
	*/ 

	//Plate
	reset(); //CHECK NEW
	m_plateReverb.setSampleRate(m_iSampleRate);

	m_plateReverb.setParameter(MVerb<float>::BANDWIDTHFREQ, 1.0f);
	m_plateReverb.setParameter(MVerb<float>::DAMPINGFREQ, 0.9f);
//	m_plateReverb.setParameter(MVerb<float>::DECAY, 0.8f);
//	m_plateReverb.setParameter(MVerb<float>::MIX, 0.7f);
	m_plateReverb.setParameter(MVerb<float>::EARLYMIX, 1.0f);
	m_plateReverb.setParameter(MVerb<float>::GAIN, 1.0f);
//	m_plateReverb.setParameter(MVerb<float>::SIZE, 1.0f);
	m_plateReverb.setParameter(MVerb<float>::DENSITY, 1.0f);
	m_plateReverb.setParameter(MVerb<float>::PREDELAY, 0.1f);  
	
	// --- do the flush
	updateVariables();
	
}

void CVASTReverb::reset() { //called when switch toggled
	// flush buffers
	m_InputLPF.init();
	m_DampingLPF1.init();
	m_DampingLPF2.init();

	// Call all delay resets
	m_PreDelay.resetDelay();
	m_InputAPF_1.resetDelay();
	m_InputAPF_2.resetDelay();

	m_ParallelCF_1.resetDelay();
	m_ParallelCF_2.resetDelay();
	m_ParallelCF_3.resetDelay();
	m_ParallelCF_4.resetDelay();
	m_ParallelCF_5.resetDelay();
	m_ParallelCF_6.resetDelay();
	m_ParallelCF_7.resetDelay();
	m_ParallelCF_8.resetDelay();

	m_OutputAPF_3.resetDelay();
	m_OutputAPF_4.resetDelay();

//Plate
	m_plateReverb.reset();
	m_plateReverb.setParameter(MVerb<float>::SIZE, 1.0f);

	m_lowCutBiquad1.flushDelays();
	m_lowCutBiquadL.flushDelays();
	m_lowCutBiquadR.flushDelays();

	m_highCutBiquad1.flushDelays();
	m_highCutBiquadL.flushDelays();
	m_highCutBiquadR.flushDelays();

	if ((m_bIsOff == false) && (m_bShallBeOff == false)) {
		m_iSoftFade = 0;
		m_bShallBeOff = false; //refade in
		m_bIsOff = true;
	}
}

void CVASTReverb::updateVariables() {

	//set defaults (no gui yet)
	//m_fPreDelay_mSec = 40.000000;
	m_fPreDelayAtten_dB = 0.00f;
	m_fInputLPF_g = 0.50000f;
	m_fAPF_1_Delay_mSec = 13.28f;
	m_fAPF_1_g = 0.700000f;
	m_fAPF_2_Delay_mSec = 28.129999f;
	m_fAPF_2_g = - 0.700000f;
	m_fPComb_1_Delay_mSec = 31.709999f;
	m_fPComb_2_Delay_mSec = 37.110001f;
	m_fPComb_3_Delay_mSec = 40.230000f;
	m_fPComb_4_Delay_mSec = 44.139999f;
	
	//m_fLPF2_g2 = 0.10000; //for both
	m_fLPF2_g2 = m_fDampingMod * 0.01f;
	
	m_fAPF_3_Delay_mSec = 9.380000f;
	m_fAPF_3_g = -0.600000f;
	m_fPComb_5_Delay_mSec = 30.469999f;
	m_fPComb_6_Delay_mSec = 33.980000f;
	m_fPComb_7_Delay_mSec = 41.410000f;
	m_fPComb_8_Delay_mSec = 42.580002f;
	m_fAPF_4_Delay_mSec = 11.000000f;
	m_fAPF_4_g = 0.600000f;

	// Pre-Delay
	m_PreDelay.setDelay_mSec(m_fPredelayMod); //in ms
	m_PreDelay.setOutputAttenuation_dB(m_fPreDelayAtten_dB);

	// input diffusion
	m_InputAPF_1.setDelay_mSec(m_fAPF_1_Delay_mSec);
	m_InputAPF_1.setAPF_g(m_fAPF_1_g);

	m_InputAPF_2.setDelay_mSec(m_fAPF_2_Delay_mSec);
	m_InputAPF_2.setAPF_g(m_fAPF_2_g);

	// output diffusion
	m_OutputAPF_3.setDelay_mSec(m_fAPF_3_Delay_mSec);
	m_OutputAPF_3.setAPF_g(m_fAPF_3_g);

	m_OutputAPF_4.setDelay_mSec(m_fAPF_4_Delay_mSec);
	m_OutputAPF_4.setAPF_g(m_fAPF_4_g);

	// Comb Filters
	// set delays first...
	m_ParallelCF_1.setDelay_mSec(m_fPComb_1_Delay_mSec);
	m_ParallelCF_2.setDelay_mSec(m_fPComb_2_Delay_mSec);
	m_ParallelCF_3.setDelay_mSec(m_fPComb_3_Delay_mSec);
	m_ParallelCF_4.setDelay_mSec(m_fPComb_4_Delay_mSec);
	m_ParallelCF_5.setDelay_mSec(m_fPComb_5_Delay_mSec);
	m_ParallelCF_6.setDelay_mSec(m_fPComb_6_Delay_mSec);
	m_ParallelCF_7.setDelay_mSec(m_fPComb_7_Delay_mSec);
	m_ParallelCF_8.setDelay_mSec(m_fPComb_8_Delay_mSec);

	// ...then calcualte comb g's from RT60:
	m_ParallelCF_1.setComb_g_with_RTSixty(m_fFeedbackMod);
	m_ParallelCF_2.setComb_g_with_RTSixty(m_fFeedbackMod);
	m_ParallelCF_3.setComb_g_with_RTSixty(m_fFeedbackMod);
	m_ParallelCF_4.setComb_g_with_RTSixty(m_fFeedbackMod);
	m_ParallelCF_5.setComb_g_with_RTSixty(m_fFeedbackMod);
	m_ParallelCF_6.setComb_g_with_RTSixty(m_fFeedbackMod);
	m_ParallelCF_7.setComb_g_with_RTSixty(m_fFeedbackMod);
	m_ParallelCF_8.setComb_g_with_RTSixty(m_fFeedbackMod);

	// LPFs
	m_DampingLPF1.setLPF_g(m_fLPF2_g2);
	m_DampingLPF2.setLPF_g(m_fLPF2_g2);
	m_InputLPF.setLPF_g(m_fInputLPF_g);

	m_plateReverb.setParameter(MVerb<float>::DAMPINGFREQ, (m_fDampingMod * 0.01f));
	m_plateReverb.setParameter(MVerb<float>::PREDELAY, (m_fPredelayMod / 200.f)); //200?? why??
	m_plateReverb.setParameter(MVerb<float>::MIX, (m_fSizeMod * 0.01f));
	m_plateReverb.setParameter(MVerb<float>::DECAY, ((m_fFeedbackMod / 5000.0f) * 0.76f));  // <0.86 otherwise endless feedback!
}

void CVASTReverb::processBlock(AudioSampleBuffer& buffer, MidiBuffer& , const int numSamples) {
	
	if (isOffAndShallBeOff() == true) return;

	modMatrixInputState inputState;
	float* bufferWritePointerL = buffer.getWritePointer(0);
	float* bufferWritePointerR = buffer.getWritePointer(1);

	//for (int currentFrameOSAdjusted = 0; currentFrameOSAdjusted < buffer.getNumSamples() * m_Set->m_nOversamplingRatio; currentFrameOSAdjusted += m_Set->m_nOversamplingRatio) {
	for (int currentFrame = 0; currentFrame < numSamples ; currentFrame += 1) {
		checkSoftFade();

		int currentFrameOSAdjusted = currentFrame;
		if (m_bOversampling) currentFrameOSAdjusted /= C_OVERSAMPLING_RATIO;

		inputState = ((VASTAudioProcessor*)my_processor)->m_pVASTXperience.m_Poly.getOldestNotePlayedInputState(currentFrameOSAdjusted); // make parameter oldest or newest

		m_fReverbGain_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_fReverbGain, MODMATDEST::ReverbGain, &inputState));
		float lReverbGain = m_fReverbGain_smoothed.getNextValue();

		m_fReverbDryWet_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_fReverbDryWet, MODMATDEST::ReverbDryWet, &inputState));
		float lReverbDryWet = m_fReverbDryWet_smoothed.getNextValue();

		m_fReverbLowcut_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_fReverbLowcut, MODMATDEST::ReverbLowcut, &inputState));
		if (m_fReverbLowcut_smoothed.isSmoothing()) {
			float lReverbLowcut = m_fReverbLowcut_smoothed.getNextValue();
			float fQ = 0.707f;
			m_lowCutBiquadL.calcBiquad(CVASTBiQuad::HIGHPASS, lReverbLowcut, m_iSampleRate, fQ, -18.0f);
			m_lowCutBiquadR.copySettingsFrom(&m_lowCutBiquadL);
			m_lowCutBiquad1.copySettingsFrom(&m_lowCutBiquadL);
		}

		m_fReverbHighcut_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_fReverbHighcut, MODMATDEST::ReverbHighcut, &inputState));
		if (m_fReverbHighcut_smoothed.isSmoothing()) {
			float lReverbHighcut = m_fReverbHighcut_smoothed.getNextValue();
			float fQ = 0.707f;
			m_highCutBiquadL.calcBiquad(CVASTBiQuad::LOWPASS, lReverbHighcut, m_iSampleRate, fQ, -18.0f);
			m_highCutBiquadR.copySettingsFrom(&m_highCutBiquadL);
			m_highCutBiquad1.copySettingsFrom(&m_highCutBiquadL);
		}

		m_fReverbFeedback_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_fReverbFeedback, MODMATDEST::ReverbFeedback, &inputState));
		m_fFeedbackMod = m_fReverbFeedback_smoothed.getNextValue();

		m_fReverbSize_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_fReverbSize, MODMATDEST::ReverbSize, &inputState));
		m_fSizeMod = m_fReverbSize_smoothed.getNextValue();

		m_fReverbPredelay_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_fReverbPredelay, MODMATDEST::ReverbPredelay, &inputState));
		m_fPredelayMod = m_fReverbPredelay_smoothed.getNextValue();

		m_fReverbDamping_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_fReverbDamping, MODMATDEST::ReverbDamping, &inputState));
		m_fDampingMod = m_fReverbDamping_smoothed.getNextValue();

		if (m_fReverbFeedback_smoothed.isSmoothing() || m_fReverbSize_smoothed.isSmoothing() || m_fReverbPredelay_smoothed.isSmoothing() || m_fReverbDamping_smoothed.isSmoothing())
			updateVariables();		

		//DryWet Mod
		float fIn[2];
		float fReverbOut[2];
		fIn[0] = bufferWritePointerL[currentFrame];
		fIn[1] = bufferWritePointerR[currentFrame];

		if (*m_uReverbMode == 0.f) {//room
			//pre filter and drywet is done inside
			processAudioFrame(fIn, fReverbOut, 2, 2, currentFrame, lReverbDryWet); //room reverb

		}
		else { //plate
			fIn[0] = m_lowCutBiquadL.doBiQuad(fIn[0]); //do pre filter
			fIn[1] = m_lowCutBiquadR.doBiQuad(fIn[1]); //do pre filter

			fIn[0] = m_highCutBiquadL.doBiQuad(fIn[0]); //do pre filter
			fIn[1] = m_highCutBiquadR.doBiQuad(fIn[1]); //do pre filter

			m_plateReverb.process(fIn, fReverbOut); //plate reverb

			fReverbOut[0] = bufferWritePointerL[currentFrame] + (float(m_iSoftFade) / float(C_MAX_SOFTFADE)) * (lReverbDryWet / 100.0) * (m_fSizeMod * 0.01f) * fReverbOut[0];
			fReverbOut[1] = bufferWritePointerR[currentFrame] + (float(m_iSoftFade) / float(C_MAX_SOFTFADE)) * (lReverbDryWet / 100.0) * (m_fSizeMod * 0.01f) * fReverbOut[1];
		}
		
		bufferWritePointerL[currentFrame] = fReverbOut[0] * lReverbGain * 0.01f;
		bufferWritePointerR[currentFrame] = fReverbOut[1] * lReverbGain * 0.01f;
	}
}

bool CVASTReverb::processAudioFrame(float* pInputBuffer, float* pOutputBuffer, MYUINT uNumInputChannels, MYUINT uNumOutputChannels, int , float dryWet) {
	//
	// Form our input = L + R (if there is a R)
	//
	float fInputSample = pInputBuffer[0];

	//TODO is that mono????

	if (uNumInputChannels == 2)
	{
		// mix
		fInputSample += pInputBuffer[1];

		// attenuate by 0.5
		fInputSample *= 0.5;
	}
	fInputSample = m_lowCutBiquad1.doBiQuad(fInputSample); //do pre filter
	fInputSample = m_highCutBiquad1.doBiQuad(fInputSample); //do pre filter

	if (m_bIsOff == true) {
		pOutputBuffer[0] = pInputBuffer[0];
		// Do RIGHT Channel if there is one
		if (uNumOutputChannels == 2)
		{
			pOutputBuffer[1] = pInputBuffer[1];
		}
		return true;
	}

	// begin the series/parallel signal push
	// Pre-Delay
	float fPreDelayOut = 0;
	m_PreDelay.processAudio(&fInputSample, &fPreDelayOut);

	// Pre-Delay Out -> fAPF_1_Out
	float fAPF_1_Out = 0;
	m_InputAPF_1.processAudio(&fPreDelayOut, &fAPF_1_Out);

	// fAPF_1_Out -> fAPF_2_Out
	float fAPF_2_Out = 0;
	m_InputAPF_2.processAudio(&fAPF_1_Out, &fAPF_2_Out);

	// fAPF_2_Out -> fInputLPF
	float fInputLPF = 0;
	m_InputLPF.processAudio(&fAPF_2_Out, &fInputLPF);

	// comb filter bank
	// variables for each output
	float fPC_1_Out = 0;
	float fPC_2_Out = 0;
	float fPC_3_Out = 0;
	float fPC_4_Out = 0;
	float fPC_5_Out = 0;
	float fPC_6_Out = 0;
	float fPC_7_Out = 0;
	float fPC_8_Out = 0;
	float fC1_Out = 0;
	float fC2_Out = 0;

	// fInputLPF -> fPC_1_Out, fPC_2_Out, fPC_3_Out, fPC_4_Out
	m_ParallelCF_1.processAudio(&fInputLPF, &fPC_1_Out);
	m_ParallelCF_2.processAudio(&fInputLPF, &fPC_2_Out);
	m_ParallelCF_3.processAudio(&fInputLPF, &fPC_3_Out);
	m_ParallelCF_4.processAudio(&fInputLPF, &fPC_4_Out);

	// fInputLPF -> fPC_5_Out, fPC_6_Out, fPC_7_Out, fPC_8_Out
	m_ParallelCF_5.processAudio(&fInputLPF, &fPC_5_Out);
	m_ParallelCF_6.processAudio(&fInputLPF, &fPC_6_Out);
	m_ParallelCF_7.processAudio(&fInputLPF, &fPC_7_Out);
	m_ParallelCF_8.processAudio(&fInputLPF, &fPC_8_Out);

	// form outputs: note attenuation by 0.25 for each and alternating signs
	fC1_Out = 0.25*fPC_1_Out - 0.25*fPC_2_Out + 0.25*fPC_3_Out - 0.25*fPC_4_Out;
	fC2_Out = 0.25*fPC_5_Out - 0.25*fPC_6_Out + 0.25*fPC_7_Out - 0.25*fPC_8_Out;

	// fC1_Out -> fDamping_LPF_1_Out
	float fDamping_LPF_1_Out = 0;
	m_DampingLPF1.processAudio(&fC1_Out, &fDamping_LPF_1_Out);

	// fC2_Out -> fDamping_LPF_2_Out
	float fDamping_LPF_2_Out = 0;
	m_DampingLPF2.processAudio(&fC2_Out, &fDamping_LPF_2_Out);

	// fDamping_LPF_1_Out -> fAPF_3_Out
	float fAPF_3_Out = 0;
	m_OutputAPF_3.processAudio(&fDamping_LPF_1_Out, &fAPF_3_Out);

	// fDamping_LPF_2_Out -> fAPF_4_Out
	float fAPF_4_Out = 0;
	m_OutputAPF_4.processAudio(&fDamping_LPF_2_Out, &fAPF_4_Out);

	// form output = (100-Wet)/100*x(n) + (Wet/100)*fAPF_3_Out
	//pOutputBuffer[0] = ((100.0 - m_Set->m_fWet_pct) / 100.0)*pInputBuffer[0] +
	//	(m_Set->m_fWet_pct / 100.0)*(fAPF_3_Out);
	
	//float lLFOModRev = m_Set->fLFOBuffer[currentFrameOSAdjusted] * m_Set->get0To1ModMatrixMultiplier(CVASTParamState::LFODepth, 1.0, 1.0, currentFrameOSAdjusted, 0, 0); // -1 .. 1 - center 0.0 //TODO check how it Reaches LFO MOD!
	
	pOutputBuffer[0] = pInputBuffer[0] + (float(m_iSoftFade) / float(C_MAX_SOFTFADE)) *
		(dryWet / 100.0) * (m_fSizeMod * 0.01) * (fAPF_3_Out);

	// Do RIGHT Channel if there is one
	if (uNumOutputChannels == 2)
	{
		// form output = (100-Wet)/100*x(n) + (Wet/100)*fAPF_4_Out
		//pOutputBuffer[1] = ((100.0 - m_Set->m_fWet_pct) / 100.0)*pInputBuffer[1] +
		//	(m_Set->m_fWet_pct / 100.0)*(fAPF_4_Out);
		pOutputBuffer[1] = pInputBuffer[1] + (float(m_iSoftFade) / float(C_MAX_SOFTFADE)) * 
			(dryWet / 100.0) * (m_fSizeMod * 0.01) * (fAPF_4_Out);
	}

	return true;
}

//==============================================================================

void CVASTReverb::getStateInformation(MemoryBlock& )
{
	//std::unique_ptr<XmlElement> xml (parameters.valueTreeState.state.createXml());
	//copyXmlToBinary (*xml, destData);
}

void CVASTReverb::setStateInformation(const void* , int )
{
	//std::unique_ptr<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
	//if (xmlState != nullptr)
	//  if (xmlState->hasTagName (parameters.valueTreeState.state.getType()))
	//    parameters.valueTreeState.state = ValueTree::fromXml (*xmlState);
}
