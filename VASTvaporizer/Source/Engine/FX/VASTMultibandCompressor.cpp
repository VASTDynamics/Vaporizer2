/*
VAST Dynamics Audio Software (TM)

All modulators tested: OK

This code accompanies the textbook:

Digital Audio Effects: Theory, Implementation and Application
Joshua D. Reiss and Andrew P. McPherson
*/

#include "VASTMultibandCompressor.h"
#include "../VASTEngineHeader.h"
#include "../../Plugin/VASTAudioProcessor.h"
#include "VASTEffect.h"
#include "../VASTSettings.h"

//==============================================================================


CVASTMultibandCompressor::CVASTMultibandCompressor(VASTAudioProcessor* processor, CVASTSettings& set, int busnr) : m_Set(&set) {
	my_processor = processor;
	myBusnr = busnr;
	setEffectName("COMPRESSOR");

	m_bIsOff = true;
	m_bShallBeOff = true;
	m_iSoftFade = 0;
}

void CVASTMultibandCompressor::initParameters() {
	AudioProcessorValueTreeState& parameters = my_processor->getParameterTree();

	createAndAddParameter(&m_bMBCompOffOn, parameters, 1, "m_bMBCompOffOn", "Compressor on / off", "On", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 1.0f, 1.0f), 0.0f,
		CVASTParamState::toggleButtonValueToTextFunction,
		CVASTParamState::toggleButtonTextToValueFunction,
		false, true, true, true,
		true);

	createAndAddParameter(&m_fMBCompFcLow, parameters, 1, "m_fMBCompFcLow", "Compressor low band cut frequency (hz)", "Lo(Hz)", 1,
		MODMATDEST::CompressorLowFreq,
		NormalisableRange<float>(50.0f, 4000.0f), 1500.0f,
		[](float value) {return String(value) + " Hz"; },
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);
	createAndAddParameter(&m_fMBCompFcHigh, parameters, 1, "m_fMBCompFcHigh", "Compressor high band cut frequency (hz)", "Hi(Hz)", 2,
		MODMATDEST::CompressorHighFreq,
		NormalisableRange<float>(5000.0f, 10000.0f), 6700.0f,
		[](float value) {return String(value) + " Hz"; },
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	//low band
	createAndAddParameter(&m_fMBCompThresholdLB, parameters, 1, "m_fMBCompThresholdLB", "Compressor threshold low band (dB)", "Thres", 3,
		MODMATDEST::CompressorThresholdLowBand,
		NormalisableRange<float>(-60.0f, -0.0001f), -40.0f,
		[](float value) {return String(value) + " dB" ; },
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);
	createAndAddParameter(&m_fMBCompRatioLB, parameters, 1, "m_fMBCompRatioLB", "Compressor ratio low band (:1)", "Ratio", 4,
		MODMATDEST::CompressorRatioLowBand,
		NormalisableRange<float>(1.f, 100.f, 0.000001f, 0.3f, false), 40.0f,
		[](float value) {return String(value) + " :1"; },
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	createAndAddParameter(&m_fMBCompAttackLB, parameters, 1, "m_fMBCompAttackLB", "Attack low band (ms)", "Atk", 5,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.1f, 80.0f), 4.0f,
		[](float value) {return String(value) + " ms"; },
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);
	createAndAddParameter(&m_fMBCompReleaseLB, parameters, 1, "m_fMBCompReleaseLB", "Release low band (ms)", "Rel", 6,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.1f, 1000.0f), 4.0f,
		[](float value) {return String(value) + " ms"; },
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);

	createAndAddParameter(&m_fMBCompGainLB, parameters, 1, "m_fMBCompGainLB", "Compressor gain low band (dB)", "LowGain", 7,
		MODMATDEST::CompressorLowGain,
		NormalisableRange<float>(0.0f, 40.f), 14.0f,
		[](float value) {return String(value) + " dB"; },
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	//mid band
	createAndAddParameter(&m_fMBCompThresholdMB, parameters, 1, "m_fMBCompThresholdMB", "Compressor threshold mid band (dB)", "Thres", 8,
		MODMATDEST::CompressorThresholdMidBand,
		NormalisableRange<float>(-60.0f, -0.0001f), -10.0f,
		[](float value) {return String(value) + " dB"; },
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);
	createAndAddParameter(&m_fMBCompRatioMB, parameters, 1, "m_fMBCompRatioMB", "Compressor ratio mid band (:1)", "Ratio", 9,
		MODMATDEST::CompressorRatioMidBand,
		NormalisableRange<float>(1.f, 100.f, 0.000001f, 0.3f, false), 25.0f,
		[](float value) {return String(value) + " :1"; },
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);
	createAndAddParameter(&m_fMBCompAttackMB, parameters, 1, "m_fMBCompAttackMB", "Attack mid band (ms)", "Atk", 10,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.1f, 80.0), 4.0f,
		[](float value) {return String(value) + " ms"; },
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);
	createAndAddParameter(&m_fMBCompReleaseMB, parameters, 1, "m_fMBCompReleaseMB", "Release mid band (ms)", "Rel", 11,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.1f, 1000.0f), 4.0f,
		[](float value) {return String(value) + " ms"; },
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);
	
	createAndAddParameter(&m_fMBCompGainMB, parameters, 1, "m_fMBCompGainMB", "Compressor gain mid band (dB)", "MidGain", 12,
		MODMATDEST::CompressorMidGain,
		NormalisableRange<float>(0.0f, 40.f), 8.0f,
		[](float value) {return String(value) + " dB"; },
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	//high band	
	createAndAddParameter(&m_fMBCompThresholdHB, parameters, 1, "m_fMBCompThresholdHB", "Compressor threshold high band (dB)", "Thres", 13,
		MODMATDEST::CompressorThresholdHighBand,
		NormalisableRange<float>(-60.0f, -0.0001f), -40.0f,
		[](float value) {return String(value) + " dB"; },
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);
	createAndAddParameter(&m_fMBCompRatioHB, parameters, 1, "m_fMBCompRatioHB", "Compressor ratio high band (:1)", "Ratio", 14,
		MODMATDEST::CompressorRatioHighBand,
		NormalisableRange<float>(1.f, 100.f, 0.000001f, 0.3f, false), 55.0f,
		[](float value) {return String(value) + " :1"; },
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);
	
	createAndAddParameter(&m_fMBCompAttackHB, parameters, 1, "m_fMBCompAttackHB", "Attack high band (ms)", "Atk", 15,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.1f, 80.0), 4.0f,
		[](float value) {return String(value) + " ms"; },
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);
	createAndAddParameter(&m_fMBCompReleaseHB, parameters, 1, "m_fMBCompReleaseHB", "Release high band (ms)", "Rel", 16,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.1f, 1000.0f), 4.0f,
		[](float value) {return String(value) + " ms"; },
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);
	
	createAndAddParameter(&m_fMBCompGainHB, parameters, 1, "m_fMBCompGainHB", "Compressor gain high band (dB)", "HighGain", 17,
		MODMATDEST::CompressorHighGain,
		NormalisableRange<float>(0.0f, 40.f), 10.0f,
		[](float value) {return String(value) + " dB"; },
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);
}

void CVASTMultibandCompressor::initCompatibilityParameters() {
	// all new parameters go here
}

CVASTMultibandCompressor::~CVASTMultibandCompressor() {
	AudioProcessorValueTreeState& parameters = ((VASTAudioProcessor*)my_processor)->getParameterTree();
	Array<AudioProcessorParameterWithID*> params = getParameters();
	for (int i = 0; i < params.size(); i++) {
		parameters.removeParameterListener(params[i]->paramID, (AudioProcessorValueTreeState::Listener*)this);
	}
}

void CVASTMultibandCompressor::parameterChanged(const String& parameterID, float newValue) {
	if (parameterID.startsWith("m_bMBCompOffOn")) {
		if (newValue == static_cast<int>(SWITCH::SWITCH_ON))
			switchOn();
		else
			switchOff();
	}
	else if (parameterID.startsWith("m_fMBCompFcLow")) {	
		m_fMBCompFcLow_smoothed.setTargetValue(*m_fMBCompFcLow);
		adjustLowband();
	}
	else if (parameterID.startsWith("m_fMBCompFcHigh")) {
		m_fMBCompFcHigh_smoothed.setTargetValue(*m_fMBCompFcHigh);
		adjustHighband();
	}
	else if (parameterID.startsWith("m_fMBCompThresholdLB")) {
		m_fMBCompThresholdLB_smoothed.setTargetValue(newValue);
	}
	else if (parameterID.startsWith("m_fMBCompThresholdHB")) {
		m_fMBCompThresholdHB_smoothed.setTargetValue(newValue);
	}
	else if (parameterID.startsWith("m_fMBCompThresholdMB")) {
		m_fMBCompThresholdMB_smoothed.setTargetValue(newValue);
	}

	else if (parameterID.startsWith("m_fMBCompRatioLB")) {
		m_fMBCompRatioLB_smoothed.setTargetValue(newValue);
	}
	else if (parameterID.startsWith("m_fMBCompRatioHB")) {
		m_fMBCompRatioHB_smoothed.setTargetValue(newValue);
	}
	else if (parameterID.startsWith("m_fMBCompRatioMB")) {
		m_fMBCompRatioMB_smoothed.setTargetValue(newValue);
	}

	else if (parameterID.startsWith("m_fMBCompGainLB")) {
		m_fMBCompGainLB_smoothed.setTargetValue(newValue);
	}
	else if (parameterID.startsWith("m_fMBCompGainHB")) {
		m_fMBCompGainHB_smoothed.setTargetValue(newValue);
	}
	else if (parameterID.startsWith("m_fMBCompGainMB")) {
		m_fMBCompGainMB_smoothed.setTargetValue(newValue);
	}
}

void CVASTMultibandCompressor::init() {
	initParameters();
}

void CVASTMultibandCompressor::reset() { 
	m_yL_prevLB = 0;
	m_yL_prevMB = 0;
	m_yL_prevHB = 0;

	for (int i = 0; i < m_iExpectedSamplesPerBlock; ++i)
	{
		x_g_lf[i] = 0;	y_g_lf[i] = 0;
		x_l_lf[i] = 0;	y_l_lf[i] = 0;
		c_lf[i] = 0;
	}
	for (int i = 0; i < m_iExpectedSamplesPerBlock; ++i)
	{
		x_g_mf[i] = 0;	y_g_mf[i] = 0;
		x_l_mf[i] = 0;	y_l_mf[i] = 0;
		c_mf[i] = 0;
	}
	for (int i = 0; i < m_iExpectedSamplesPerBlock; ++i)
	{
		x_g_hf[i] = 0;	y_g_hf[i] = 0;
		x_l_hf[i] = 0;	y_l_hf[i] = 0;
		c_hf[i] = 0;
	}

	if ((m_bIsOff == false) && (m_bShallBeOff == false)) {
		m_iSoftFade = 0;
		m_bShallBeOff = false; //refade in
		m_bIsOff = true;
	}
}

//==============================================================================

void CVASTMultibandCompressor::prepareToPlay (double sampleRate, int samplesPerBlock) {
	//m_iSampleRate is set in userOversampling
	m_iExpectedSamplesPerBlock = samplesPerBlock;
	m_iNumChannels = 2;

	const double smoothTime = 0.02;
	m_fMBCompThresholdLB_smoothed.reset(m_iSampleRate, smoothTime);
	m_fMBCompThresholdHB_smoothed.reset(m_iSampleRate, smoothTime);
	m_fMBCompThresholdMB_smoothed.reset(m_iSampleRate, smoothTime);
	m_fMBCompRatioLB_smoothed.reset(m_iSampleRate, smoothTime);
	m_fMBCompRatioHB_smoothed.reset(m_iSampleRate, smoothTime);
	m_fMBCompRatioMB_smoothed.reset(m_iSampleRate, smoothTime);
	m_fMBCompGainLB_smoothed.reset(m_iSampleRate, smoothTime);
	m_fMBCompGainHB_smoothed.reset(m_iSampleRate, smoothTime);
	m_fMBCompGainMB_smoothed.reset(m_iSampleRate, smoothTime);
	m_fMBCompFcLow_smoothed.reset(m_iSampleRate, smoothTime);
	m_fMBCompFcHigh_smoothed.reset(m_iSampleRate, smoothTime);
	
	// Allocate a lot of dynamic memory here for low band
	x_g_lf.allocate(samplesPerBlock, true);
	x_l_lf.allocate(samplesPerBlock, true);
	y_g_lf.allocate(samplesPerBlock, true);
	y_l_lf.allocate(samplesPerBlock, true);
	c_lf.allocate(samplesPerBlock, true);

	// Allocate a lot of dynamic memory here for mid band
	x_g_mf.allocate(samplesPerBlock, true);
	x_l_mf.allocate(samplesPerBlock, true);
	y_g_mf.allocate(samplesPerBlock, true);
	y_l_mf.allocate(samplesPerBlock, true);
	c_mf.allocate(samplesPerBlock, true);

	// Allocate a lot of dynamic memory here for high band
	x_g_hf.allocate(samplesPerBlock, true);
	x_l_hf.allocate(samplesPerBlock, true);
	y_g_hf.allocate(samplesPerBlock, true);
	y_l_hf.allocate(samplesPerBlock, true);
	c_hf.allocate(samplesPerBlock, true);

	//autoTime = false;
	reset();

	//===================================================//
	//          Calculate Coefficients                   //
	//===================================================//

	m_fMBCompFcLow_smoothed.setTargetValue(*m_fMBCompFcLow);
	m_fMBCompFcHigh_smoothed.setTargetValue(*m_fMBCompFcHigh);

	adjustLowband();
	adjustHighband();
}

void CVASTMultibandCompressor::releaseResources() {
}

void CVASTMultibandCompressor::adjustLowband() {
	float fcLow = m_fMBCompFcLow_smoothed.getNextValue();
	lowPassL1.setCoefficients(coeff.makeLowPass(m_iSampleRate, fcLow));
	lowPassL2.setCoefficients(coeff.makeLowPass(m_iSampleRate, fcLow));
	lowPassR1.setCoefficients(coeff.makeLowPass(m_iSampleRate, fcLow));
	lowPassR2.setCoefficients(coeff.makeLowPass(m_iSampleRate, fcLow));

	lowBandPassL1.setCoefficients(coeff.makeHighPass(m_iSampleRate, fcLow));
	lowBandPassL2.setCoefficients(coeff.makeHighPass(m_iSampleRate, fcLow));
	lowBandPassR1.setCoefficients(coeff.makeHighPass(m_iSampleRate, fcLow));
	lowBandPassR2.setCoefficients(coeff.makeHighPass(m_iSampleRate, fcLow));
}

void CVASTMultibandCompressor::adjustHighband() {
	float fcHigh = m_fMBCompFcHigh_smoothed.getNextValue();
	highPassL1.setCoefficients(coeff.makeHighPass(m_iSampleRate, fcHigh));
	highPassL2.setCoefficients(coeff.makeHighPass(m_iSampleRate, fcHigh));
	highPassR1.setCoefficients(coeff.makeHighPass(m_iSampleRate, fcHigh));
	highPassR2.setCoefficients(coeff.makeHighPass(m_iSampleRate, fcHigh));

	highBandPassL1.setCoefficients(coeff.makeLowPass(m_iSampleRate, fcHigh));
	highBandPassL2.setCoefficients(coeff.makeLowPass(m_iSampleRate, fcHigh));
	highBandPassR1.setCoefficients(coeff.makeLowPass(m_iSampleRate, fcHigh));
	highBandPassR2.setCoefficients(coeff.makeLowPass(m_iSampleRate, fcHigh));
}

void CVASTMultibandCompressor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages, const int numSamples) {
	if (isOffAndShallBeOff() == true) return;

 	ScopedNoDenormals noDenormals;
    
	//===================================================//
	//   Create and Empty Buffers                        //
	//===================================================//

	AudioSampleBuffer lpFilterBuffer;
	AudioSampleBuffer hpFilterBuffer;
	AudioSampleBuffer bpFilterBuffer;

	AudioSampleBuffer lpCompFilterBuffer;
	AudioSampleBuffer hpCompFilterBuffer;
	AudioSampleBuffer bpCompFilterBuffer;

	lpFilterBuffer.setSize(2, numSamples);
	lpFilterBuffer.clear();

	hpFilterBuffer.setSize(2, numSamples);
	hpFilterBuffer.clear();

	bpFilterBuffer.setSize(2, numSamples);
	bpFilterBuffer.clear();

	lpCompFilterBuffer.setSize(2, numSamples);
	lpCompFilterBuffer.clear();

	hpCompFilterBuffer.setSize(2, numSamples);
	hpCompFilterBuffer.clear();

	bpCompFilterBuffer.setSize(2, numSamples);
	bpCompFilterBuffer.clear();

	lpFilterBuffer.copyFrom(0, 0, buffer.getReadPointer(0, 0), numSamples);
	lpFilterBuffer.copyFrom(1, 0, buffer.getReadPointer(1, 0), numSamples);
	hpFilterBuffer.copyFrom(0, 0, buffer.getReadPointer(0, 0), numSamples);
	hpFilterBuffer.copyFrom(1, 0, buffer.getReadPointer(1, 0), numSamples);
	bpFilterBuffer.copyFrom(0, 0, buffer.getReadPointer(0, 0), numSamples);
	bpFilterBuffer.copyFrom(1, 0, buffer.getReadPointer(1, 0), numSamples);

	//===================================================//
	//   Apply Filters to Buffers                        //
	//===================================================//

	//CHVAST Check for nands
	
	modMatrixInputState l_inputState = ((VASTAudioProcessor*)my_processor)->m_pVASTXperience.m_Poly.getOldestNotePlayedInputState(0); // make parameter oldest or newest
	if (m_Set->modMatrixDestinationSetFast(MODMATDEST::CompressorLowFreq)) {
		m_fMBCompFcLow_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_fMBCompFcLow, MODMATDEST::CompressorLowFreq, &l_inputState));
	}
	if (m_fMBCompFcLow_smoothed.isSmoothing())
		adjustLowband();
	m_fMBCompFcLow_smoothed.skip(numSamples);
	if (m_Set->modMatrixDestinationSetFast(MODMATDEST::CompressorHighFreq)) {
		m_fMBCompFcHigh_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_fMBCompFcHigh, MODMATDEST::CompressorHighFreq, &l_inputState));
	}
	if (m_fMBCompFcHigh_smoothed.isSmoothing())
		adjustHighband();
	m_fMBCompFcHigh_smoothed.skip(numSamples);

	lowPassL1.processSamples(lpFilterBuffer.getWritePointer(0), numSamples);
	lowPassR1.processSamples(lpFilterBuffer.getWritePointer(1), numSamples);
	lowPassL2.processSamples(lpFilterBuffer.getWritePointer(0), numSamples);
	lowPassR2.processSamples(lpFilterBuffer.getWritePointer(1), numSamples);

	lowBandPassL1.processSamples(bpFilterBuffer.getWritePointer(0), numSamples);
	lowBandPassR1.processSamples(bpFilterBuffer.getWritePointer(1), numSamples);
	lowBandPassL2.processSamples(bpFilterBuffer.getWritePointer(0), numSamples);
	lowBandPassR2.processSamples(bpFilterBuffer.getWritePointer(1), numSamples);

	highBandPassL1.processSamples(bpFilterBuffer.getWritePointer(0), numSamples);
	highBandPassR1.processSamples(bpFilterBuffer.getWritePointer(1), numSamples);
	highBandPassL2.processSamples(bpFilterBuffer.getWritePointer(0), numSamples);
	highBandPassR2.processSamples(bpFilterBuffer.getWritePointer(1), numSamples);

	highPassL1.processSamples(hpFilterBuffer.getWritePointer(0), numSamples);
	highPassR1.processSamples(hpFilterBuffer.getWritePointer(1), numSamples);
	highPassL2.processSamples(hpFilterBuffer.getWritePointer(0), numSamples);
	highPassR2.processSamples(hpFilterBuffer.getWritePointer(1), numSamples);


	//===================================================//
	//   Apply Compressors to Buffers                    //
	//===================================================//

	if (*m_bMBCompOffOn == static_cast<int>(SWITCH::SWITCH_ON))
	{
		lpCompFilterBuffer.addFrom(0, 0, lpFilterBuffer, 0, 0, numSamples, 0.5);
		lpCompFilterBuffer.addFrom(0, 0, lpFilterBuffer, 1, 0, numSamples, 0.5);
		//compression : calculates the control voltage
		compressorLB(buffer, 0, l_inputState, numSamples);
																 
		for (int i = 0; i < numSamples; ++i)
		{
			lpFilterBuffer.getWritePointer(0)[i] *= c_lf[i];
			lpFilterBuffer.getWritePointer(1)[i] *= c_lf[i];
		}
		lpCompFilterBuffer.clear(0, 0, numSamples);

		bpCompFilterBuffer.addFrom(0, 0, bpFilterBuffer, 0, 0, numSamples, 0.5);
		bpCompFilterBuffer.addFrom(0, 0, bpFilterBuffer, 1, 0, numSamples, 0.5);
		//compression : calculates the control voltage
		compressorMB(bpCompFilterBuffer, 0, l_inputState, numSamples);
											 
		for (int i = 0; i < numSamples; ++i)
		{
			bpFilterBuffer.getWritePointer(0)[i] *= c_mf[i];
			bpFilterBuffer.getWritePointer(1)[i] *= c_mf[i];
		}
		bpCompFilterBuffer.clear(0, 0, numSamples);

		hpCompFilterBuffer.addFrom(0, 0, hpFilterBuffer, 0, 0, numSamples, 0.5);
		hpCompFilterBuffer.addFrom(0, 0, hpFilterBuffer, 1, 0, numSamples, 0.5);
		//compression : calculates the control voltage
		compressorHB(hpCompFilterBuffer, 0, l_inputState, numSamples);
											 
		for (int i = 0; i < numSamples; ++i)
		{
			hpFilterBuffer.getWritePointer(0)[i] *= c_hf[i];
			hpFilterBuffer.getWritePointer(1)[i] *= c_hf[i];
		}
		hpCompFilterBuffer.clear(0, 0, numSamples);

		float fSoftmixStart = (float(m_iSoftFade) / float(C_MAX_SOFTFADE));
		for (int i = 0; i < numSamples; ++i)
			checkSoftFade();
		float fSoftmixEnd = (float(m_iSoftFade) / float(C_MAX_SOFTFADE));
		for (int channel = 0; channel < m_iNumChannels; channel++) {
			buffer.copyFrom(channel, 0, lpFilterBuffer, channel, 0, numSamples);
			buffer.addFrom(channel, 0, bpFilterBuffer, channel, 0, numSamples);
			buffer.addFrom(channel, 0, hpFilterBuffer, channel, 0, numSamples);
		}
		buffer.applyGainRamp(0, numSamples, fSoftmixStart, fSoftmixEnd);
	}

    //======================================
}

//==============================================================================

// Low compressor function
void CVASTMultibandCompressor::compressorLB(AudioSampleBuffer &buffer, int m, modMatrixInputState& inputState, const int numSamples) {
    float thresholdLB {};
	float tauAttackLB = *m_fMBCompAttackLB;
	float tauReleaseLB = *m_fMBCompReleaseLB;
    float ratioLB {};
    float makeUpGainLB {};

	float alphaAttackLB = exp(-1 / (0.001 * m_iSampleRate * tauAttackLB));
	float alphaReleaseLB = exp(-1 / (0.001 * m_iSampleRate * tauReleaseLB));
	for (int i = 0; i < numSamples; ++i)
	{
		inputState.currentFrame = i;
		if (m_Set->modMatrixDestinationSetFast(MODMATDEST::CompressorThresholdLowBand)) {
			m_fMBCompThresholdLB_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_fMBCompThresholdLB, MODMATDEST::CompressorThresholdLowBand, &inputState));
		}
		thresholdLB = m_fMBCompThresholdLB_smoothed.getNextValue();
		
		if (m_Set->modMatrixDestinationSetFast(MODMATDEST::CompressorRatioLowBand)) {
			m_fMBCompRatioLB_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_fMBCompRatioLB, MODMATDEST::CompressorRatioLowBand, &inputState));
		}
		ratioLB = m_fMBCompRatioLB_smoothed.getNextValue();

		if (m_Set->modMatrixDestinationSetFast(MODMATDEST::CompressorLowGain)) {
			m_fMBCompGainLB_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_fMBCompGainLB, MODMATDEST::CompressorLowGain, &inputState));
		}
		makeUpGainLB = m_fMBCompGainLB_smoothed.getNextValue();

		//Level detection- estimate level using peak detector
		if (fabs(buffer.getWritePointer(m)[i]) < 0.000001) x_g_lf[i] = -120;
		else x_g_lf[i] = 20 * log10(fabs(buffer.getWritePointer(m)[i]));
		//Gain computer- static apply input/output curve
		if (x_g_lf[i] >= thresholdLB) y_g_lf[i] = thresholdLB + (x_g_lf[i] - thresholdLB) / ratioLB;
		else y_g_lf[i] = x_g_lf[i];
		x_l_lf[i] = x_g_lf[i] - y_g_lf[i];
		//Ballistics- smoothing of the gain 
		if (x_l_lf[i]>m_yL_prevLB)  y_l_lf[i] = alphaAttackLB * m_yL_prevLB + (1 - alphaAttackLB) * x_l_lf[i];
		else				 y_l_lf[i] = alphaReleaseLB * m_yL_prevLB + (1 - alphaReleaseLB) * x_l_lf[i];
		//find control
		c_lf[i] = pow(10, (makeUpGainLB - y_l_lf[i]) / 20);
		m_yL_prevLB = y_l_lf[i];
	}
}

// Mid compressor function
void CVASTMultibandCompressor::compressorMB(AudioSampleBuffer &buffer, int m, modMatrixInputState& inputState, const int numSamples) {
    float thresholdMB {};
	float tauAttackMB = *m_fMBCompAttackMB;
	float tauReleaseMB = *m_fMBCompReleaseMB;
    float ratioMB {};
    float makeUpGainMB {};

	float alphaAttackMB = exp(-1 / (0.001 * m_iSampleRate * tauAttackMB));
	float alphaReleaseMB = exp(-1 / (0.001 * m_iSampleRate * tauReleaseMB));
	for (int i = 0; i < numSamples; ++i)
	{
		inputState.currentFrame = i;
		if (m_Set->modMatrixDestinationSetFast(MODMATDEST::CompressorThresholdMidBand)) {
			m_fMBCompThresholdMB_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_fMBCompThresholdMB, MODMATDEST::CompressorThresholdMidBand, &inputState));
		}
		thresholdMB = m_fMBCompThresholdMB_smoothed.getNextValue();

		if (m_Set->modMatrixDestinationSetFast(MODMATDEST::CompressorRatioMidBand)) {
			m_fMBCompRatioMB_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_fMBCompRatioMB, MODMATDEST::CompressorRatioMidBand, &inputState));
		}
		ratioMB = m_fMBCompRatioMB_smoothed.getNextValue();

		if (m_Set->modMatrixDestinationSetFast(MODMATDEST::CompressorMidGain)) {
			m_fMBCompGainMB_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_fMBCompGainMB, MODMATDEST::CompressorMidGain, &inputState));
		}
		makeUpGainMB = m_fMBCompGainMB_smoothed.getNextValue();

		//Level detection- estimate level using peak detector
		if (fabs(buffer.getWritePointer(m)[i]) < 0.000001) x_g_mf[i] = -120;
		else x_g_mf[i] = 20 * log10(fabs(buffer.getWritePointer(m)[i]));
		//Gain computer- static apply input/output curve
		if (x_g_mf[i] >= thresholdMB) y_g_mf[i] = thresholdMB + (x_g_mf[i] - thresholdMB) / ratioMB;
		else y_g_mf[i] = x_g_mf[i];
		x_l_mf[i] = x_g_mf[i] - y_g_mf[i];
		//Ballistics- smoothing of the gain
		if (x_l_mf[i]>m_yL_prevMB)  y_l_mf[i] = alphaAttackMB * m_yL_prevMB + (1 - alphaAttackMB) * x_l_mf[i];
		else				 y_l_mf[i] = alphaReleaseMB * m_yL_prevMB + (1 - alphaReleaseMB) * x_l_mf[i];
		//find control
		c_mf[i] = pow(10, (makeUpGainMB - y_l_mf[i]) / 20);
		m_yL_prevMB = y_l_mf[i];
	}
}

//High compressor function
void CVASTMultibandCompressor::compressorHB(AudioSampleBuffer &buffer, int m, modMatrixInputState& inputState, const int numSamples) {
    float thresholdHB {};
	float tauAttackHB = *m_fMBCompAttackHB;
	float tauReleaseHB = *m_fMBCompReleaseHB;
    float ratioHB {};
    float makeUpGainHB {};

	float alphaAttackHB = exp(-1 / (0.001 * m_iSampleRate * tauAttackHB));
	float alphaReleaseHB = exp(-1 / (0.001 * m_iSampleRate * tauReleaseHB));
	for (int i = 0; i < numSamples; ++i)
	{
		inputState.currentFrame = i;
		if (m_Set->modMatrixDestinationSetFast(MODMATDEST::CompressorThresholdHighBand)) {
			m_fMBCompThresholdHB_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_fMBCompThresholdHB, MODMATDEST::CompressorThresholdHighBand, &inputState));
		}
		thresholdHB = m_fMBCompThresholdHB_smoothed.getNextValue();

		if (m_Set->modMatrixDestinationSetFast(MODMATDEST::CompressorRatioHighBand)) {
			m_fMBCompRatioHB_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_fMBCompRatioHB, MODMATDEST::CompressorRatioHighBand, &inputState));
		}
		ratioHB = m_fMBCompRatioHB_smoothed.getNextValue();

		if (m_Set->modMatrixDestinationSetFast(MODMATDEST::CompressorHighGain)) {
			m_fMBCompGainHB_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_fMBCompGainHB, MODMATDEST::CompressorHighGain, &inputState));
		}
		makeUpGainHB = m_fMBCompGainHB_smoothed.getNextValue();

		//Level detection- estimate level using peak detector
		if (fabs(buffer.getWritePointer(m)[i]) < 0.000001) x_g_hf[i] = -120;
		else x_g_hf[i] = 20 * log10(fabs(buffer.getWritePointer(m)[i]));
		//Gain computer- static apply input/output curve
		if (x_g_hf[i] >= thresholdHB) y_g_hf[i] = thresholdHB + (x_g_hf[i] - thresholdHB) / ratioHB;
		else y_g_hf[i] = x_g_hf[i];
		x_l_hf[i] = x_g_hf[i] - y_g_hf[i];
		//Ballistics- smoothing of the gain
		if (x_l_hf[i]>m_yL_prevHB)  y_l_hf[i] = alphaAttackHB * m_yL_prevHB + (1 - alphaAttackHB) * x_l_hf[i];
		else				 y_l_hf[i] = alphaReleaseHB * m_yL_prevHB + (1 - alphaReleaseHB) * x_l_hf[i];
		//find control
		c_hf[i] = pow(10, (makeUpGainHB - y_l_hf[i]) / 20);
		m_yL_prevHB = y_l_hf[i];
	}
}

//==============================================================================

void CVASTMultibandCompressor::getStateInformation (MemoryBlock& )
{
    //std::unique_ptr<XmlElement> xml (parameters.valueTreeState.state.createXml());
    //copyXmlToBinary (*xml, destData);
}

void CVASTMultibandCompressor::setStateInformation (const void* , int )
{
    //std::unique_ptr<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    //if (xmlState != nullptr)
      //  if (xmlState->hasTagName (parameters.valueTreeState.state.getType()))
        //    parameters.valueTreeState.state = ValueTree::fromXml (*xmlState);
}
