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


CVASTMultibandCompressor::CVASTMultibandCompressor(VASTAudioProcessor* processor, int busnr) {
	my_processor = processor;
	myBusnr = busnr;
	setEffectName("COMPRESSOR");

	m_bIsOff = true;
	m_bShallBeOff = true;
	m_iSoftFade = 0;
}

void CVASTMultibandCompressor::initParameters() {
	AudioProcessorValueTreeState& parameters = my_processor->getParameterTree();

	createAndAddParameter(&m_bMultibandCompressorOffOn, parameters, "m_bMultibandCompressorOffOn", "Compressor on / off", "On", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 1.0f, 1.0f), 0.0f,
		CVASTParamState::toggleButtonValueToTextFunction,
		CVASTParamState::toggleButtonTextToValueFunction,
		false, true, true, true,
		true);

	createAndAddParameter(&m_fMultibandCompressorFcLow, parameters, "m_fMultibandCompressorFcLow", "Compressor low band cut frequency (hz)", "Lo(Hz)", 1,
		MODMATDEST::CompressorLowFreq,
		NormalisableRange<float>(50.0f, 4000.0f), 1500.0f,
		[](float value) {return String(value) + " Hz"; },
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);
	createAndAddParameter(&m_fMultibandCompressorFcHigh, parameters, "m_fMultibandCompressorFcHigh", "Compressor high band cut frequency (hz)", "Hi(Hz)", 2,
		MODMATDEST::CompressorHighFreq,
		NormalisableRange<float>(5000.0f, 10000.0f), 6700.0f,
		[](float value) {return String(value) + " Hz"; },
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	//low band
	createAndAddParameter(&m_fMultibandCompressorThresholdLB, parameters, "m_fMultibandCompressorThresholdLB", "Compressor threshold low band (dB)", "Thres", 3,
		MODMATDEST::CompressorThresholdLowBand,
		NormalisableRange<float>(-60.0f, -0.0001), -40.0f,
		[](float value) {return String(value) + " dB" ; },
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);
	createAndAddParameter(&m_fMultibandCompressorRatioLB, parameters, "m_fMultibandCompressorRatioLB", "Compressor ratio low band (:1)", "Ratio", 4,
		MODMATDEST::CompressorRatioLowBand,
		NormalisableRange<float>(1.f, 100, 0.000001f, 0.3f, false), 40.0f,
		[](float value) {return String(value) + " :1"; },
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	createAndAddParameter(&m_fMultibandCompressorAttackLB, parameters, "m_fMultibandCompressorAttackLB", "Attack low band (ms)", "Atk", 5,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.1f, 80.0), 4.0f,
		[](float value) {return String(value) + " ms"; },
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);
	createAndAddParameter(&m_fMultibandCompressorReleaseLB, parameters, "m_fMultibandCompressorReleaseLB", "Release low band (ms)", "Rel", 6,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.1f, 1000.0), 4.0f,
		[](float value) {return String(value) + " ms"; },
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);

	createAndAddParameter(&m_fMultibandCompressorGainLB, parameters, "m_fMultibandCompressorGainLB", "Compressor gain low band (dB)", "LowGain", 7,
		MODMATDEST::CompressorLowGain,
		NormalisableRange<float>(0.0f, 40.f), 14.0f,
		[](float value) {return String(value) + " dB"; },
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	//mid band
	createAndAddParameter(&m_fMultibandCompressorThresholdMB, parameters, "m_fMultibandCompressorThresholdMB", "Compressor threshold mid band (dB)", "Thres", 8,
		MODMATDEST::CompressorThresholdMidBand,
		NormalisableRange<float>(-60.0f, -0.0001), -10.0f,
		[](float value) {return String(value) + " dB"; },
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);
	createAndAddParameter(&m_fMultibandCompressorRatioMB, parameters, "m_fMultibandCompressorRatioMB", "Compressor ratio mid band (:1)", "Ratio", 9,
		MODMATDEST::CompressorRatioMidBand,
		NormalisableRange<float>(1.f, 100, 0.000001f, 0.3f, false), 25.0f,
		[](float value) {return String(value) + " :1"; },
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);
	createAndAddParameter(&m_fMultibandCompressorAttackMB, parameters, "m_fMultibandCompressorAttackMB", "Attack mid band (ms)", "Atk", 10,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.1f, 80.0), 4.0f,
		[](float value) {return String(value) + " ms"; },
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);
	createAndAddParameter(&m_fMultibandCompressorReleaseMB, parameters, "m_fMultibandCompressorReleaseMB", "Release mid band (ms)", "Rel", 11,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.1f, 1000.0), 4.0f,
		[](float value) {return String(value) + " ms"; },
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);
	
	createAndAddParameter(&m_fMultibandCompressorGainMB, parameters, "m_fMultibandCompressorGainMB", "Compressor gain mid band (dB)", "MidGain", 12,
		MODMATDEST::CompressorMidGain,
		NormalisableRange<float>(0.0f, 40.f), 8.0f,
		[](float value) {return String(value) + " dB"; },
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	//high band	
	createAndAddParameter(&m_fMultibandCompressorThresholdHB, parameters, "m_fMultibandCompressorThresholdHB", "Compressor threshold high band (dB)", "Thres", 13,
		MODMATDEST::CompressorThresholdHighBand,
		NormalisableRange<float>(-60.0f, -0.0001), -40.0f,
		[](float value) {return String(value) + " dB"; },
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);
	createAndAddParameter(&m_fMultibandCompressorRatioHB, parameters, "m_fMultibandCompressorRatioHB", "Compressor ratio high band (:1)", "Ratio", 14,
		MODMATDEST::CompressorRatioHighBand,
		NormalisableRange<float>(1.f, 100, 0.000001f, 0.3f, false), 55.0f,
		[](float value) {return String(value) + " :1"; },
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);
	
	createAndAddParameter(&m_fMultibandCompressorAttackHB, parameters, "m_fMultibandCompressorAttackHB", "Attack high band (ms)", "Atk", 15,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.1f, 80.0), 4.0f,
		[](float value) {return String(value) + " ms"; },
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);
	createAndAddParameter(&m_fMultibandCompressorReleaseHB, parameters, "m_fMultibandCompressorReleaseHB", "Release high band (ms)", "Rel", 16,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.1f, 1000.0), 4.0f,
		[](float value) {return String(value) + " ms"; },
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);
	
	createAndAddParameter(&m_fMultibandCompressorGainHB, parameters, "m_fMultibandCompressorGainHB", "Compressor gain high band (dB)", "HighGain", 17,
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
	if (parameterID.startsWith("m_bMultibandCompressorOffOn")) {
		if (newValue == SWITCH::SWITCH_ON)
			switchOn();
		else
			switchOff();
	}
	else if (parameterID.startsWith("m_fMultibandCompressorFcLow")) {	
		m_fMultibandCompressorFcLow_smoothed.setValue(*m_fMultibandCompressorFcLow);
		adjustLowband();
	}
	else if (parameterID.startsWith("m_fMultibandCompressorFcHigh")) {
		m_fMultibandCompressorFcHigh_smoothed.setValue(*m_fMultibandCompressorFcHigh);
		adjustHighband();
	}
	else if (parameterID.startsWith("m_fMultibandCompressorThresholdLB")) {
		m_fMultibandCompressorThresholdLB_smoothed.setValue(newValue);
	}
	else if (parameterID.startsWith("m_fMultibandCompressorThresholdHB")) {
		m_fMultibandCompressorThresholdHB_smoothed.setValue(newValue);
	}
	else if (parameterID.startsWith("m_fMultibandCompressorThresholdMB")) {
		m_fMultibandCompressorThresholdMB_smoothed.setValue(newValue);
	}

	else if (parameterID.startsWith("m_fMultibandCompressorRatioLB")) {
		m_fMultibandCompressorRatioLB_smoothed.setValue(newValue);
	}
	else if (parameterID.startsWith("m_fMultibandCompressorRatioHB")) {
		m_fMultibandCompressorRatioHB_smoothed.setValue(newValue);
	}
	else if (parameterID.startsWith("m_fMultibandCompressorRatioMB")) {
		m_fMultibandCompressorRatioMB_smoothed.setValue(newValue);
	}

	else if (parameterID.startsWith("m_fMultibandCompressorGainLB")) {
		m_fMultibandCompressorGainLB_smoothed.setValue(newValue);
	}
	else if (parameterID.startsWith("m_fMultibandCompressorGainHB")) {
		m_fMultibandCompressorGainHB_smoothed.setValue(newValue);
	}
	else if (parameterID.startsWith("m_fMultibandCompressorGainMB")) {
		m_fMultibandCompressorGainMB_smoothed.setValue(newValue);
	}
}

void CVASTMultibandCompressor::init(CVASTSettings &set) {
	m_Set = &set;

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
	m_fMultibandCompressorThresholdLB_smoothed.reset(m_iSampleRate, smoothTime);
	m_fMultibandCompressorThresholdHB_smoothed.reset(m_iSampleRate, smoothTime);
	m_fMultibandCompressorThresholdMB_smoothed.reset(m_iSampleRate, smoothTime);
	m_fMultibandCompressorRatioLB_smoothed.reset(m_iSampleRate, smoothTime);
	m_fMultibandCompressorRatioHB_smoothed.reset(m_iSampleRate, smoothTime);
	m_fMultibandCompressorRatioMB_smoothed.reset(m_iSampleRate, smoothTime);
	m_fMultibandCompressorGainLB_smoothed.reset(m_iSampleRate, smoothTime);
	m_fMultibandCompressorGainHB_smoothed.reset(m_iSampleRate, smoothTime);
	m_fMultibandCompressorGainMB_smoothed.reset(m_iSampleRate, smoothTime);
	m_fMultibandCompressorFcLow_smoothed.reset(m_iSampleRate, smoothTime);
	m_fMultibandCompressorFcHigh_smoothed.reset(m_iSampleRate, smoothTime);
	
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

	m_fMultibandCompressorFcLow_smoothed.setValue(*m_fMultibandCompressorFcLow);
	m_fMultibandCompressorFcHigh_smoothed.setValue(*m_fMultibandCompressorFcHigh);

	adjustLowband();
	adjustHighband();
}

void CVASTMultibandCompressor::releaseResources() {
}

void CVASTMultibandCompressor::adjustLowband() {
	float fcLow = m_fMultibandCompressorFcLow_smoothed.getNextValue();
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
	float fcHigh = m_fMultibandCompressorFcHigh_smoothed.getNextValue();
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
		m_fMultibandCompressorFcLow_smoothed.setValue(m_Set->getParameterValueWithMatrixModulation(m_fMultibandCompressorFcLow, MODMATDEST::CompressorLowFreq, &l_inputState));
	}
	if (m_fMultibandCompressorFcLow_smoothed.isSmoothing())
		adjustLowband();
	m_fMultibandCompressorFcLow_smoothed.skip(numSamples);
	if (m_Set->modMatrixDestinationSetFast(MODMATDEST::CompressorHighFreq)) {
		m_fMultibandCompressorFcHigh_smoothed.setValue(m_Set->getParameterValueWithMatrixModulation(m_fMultibandCompressorFcHigh, MODMATDEST::CompressorHighFreq, &l_inputState));
	}
	if (m_fMultibandCompressorFcHigh_smoothed.isSmoothing())
		adjustHighband();
	m_fMultibandCompressorFcHigh_smoothed.skip(numSamples);

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

	if (*m_bMultibandCompressorOffOn == SWITCH::SWITCH_ON)
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
	float thresholdLB = *m_fMultibandCompressorThresholdLB;
	float tauAttackLB = *m_fMultibandCompressorAttackLB;
	float tauReleaseLB = *m_fMultibandCompressorReleaseLB;
	float ratioLB = *m_fMultibandCompressorRatioLB;
	float makeUpGainLB = *m_fMultibandCompressorGainLB;

	float alphaAttackLB = exp(-1 / (0.001 * m_iSampleRate * tauAttackLB));
	float alphaReleaseLB = exp(-1 / (0.001 * m_iSampleRate * tauReleaseLB));
	for (int i = 0; i < numSamples; ++i)
	{
		inputState.currentFrame = i;
		if (m_Set->modMatrixDestinationSetFast(MODMATDEST::CompressorThresholdLowBand)) {
			m_fMultibandCompressorThresholdLB_smoothed.setValue(m_Set->getParameterValueWithMatrixModulation(m_fMultibandCompressorThresholdLB, MODMATDEST::CompressorThresholdLowBand, &inputState));
		}
		thresholdLB = m_fMultibandCompressorThresholdLB_smoothed.getNextValue();
		
		if (m_Set->modMatrixDestinationSetFast(MODMATDEST::CompressorRatioLowBand)) {
			m_fMultibandCompressorRatioLB_smoothed.setValue(m_Set->getParameterValueWithMatrixModulation(m_fMultibandCompressorRatioLB, MODMATDEST::CompressorRatioLowBand, &inputState));
		}
		ratioLB = m_fMultibandCompressorRatioLB_smoothed.getNextValue();

		if (m_Set->modMatrixDestinationSetFast(MODMATDEST::CompressorLowGain)) {
			m_fMultibandCompressorGainLB_smoothed.setValue(m_Set->getParameterValueWithMatrixModulation(m_fMultibandCompressorGainLB, MODMATDEST::CompressorLowGain, &inputState));
		}
		makeUpGainLB = m_fMultibandCompressorGainLB_smoothed.getNextValue();

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
	float thresholdMB = *m_fMultibandCompressorThresholdMB;
	float tauAttackMB = *m_fMultibandCompressorAttackMB;
	float tauReleaseMB = *m_fMultibandCompressorReleaseMB;
	float ratioMB = *m_fMultibandCompressorRatioMB;
	float makeUpGainMB = *m_fMultibandCompressorGainMB;

	float alphaAttackMB = exp(-1 / (0.001 * m_iSampleRate * tauAttackMB));
	float alphaReleaseMB = exp(-1 / (0.001 * m_iSampleRate * tauReleaseMB));
	for (int i = 0; i < numSamples; ++i)
	{
		inputState.currentFrame = i;
		if (m_Set->modMatrixDestinationSetFast(MODMATDEST::CompressorThresholdMidBand)) {
			m_fMultibandCompressorThresholdMB_smoothed.setValue(m_Set->getParameterValueWithMatrixModulation(m_fMultibandCompressorThresholdMB, MODMATDEST::CompressorThresholdMidBand, &inputState));
		}
		thresholdMB = m_fMultibandCompressorThresholdMB_smoothed.getNextValue();

		if (m_Set->modMatrixDestinationSetFast(MODMATDEST::CompressorRatioMidBand)) {
			m_fMultibandCompressorRatioMB_smoothed.setValue(m_Set->getParameterValueWithMatrixModulation(m_fMultibandCompressorRatioMB, MODMATDEST::CompressorRatioMidBand, &inputState));
		}
		ratioMB = m_fMultibandCompressorRatioMB_smoothed.getNextValue();

		if (m_Set->modMatrixDestinationSetFast(MODMATDEST::CompressorMidGain)) {
			m_fMultibandCompressorGainMB_smoothed.setValue(m_Set->getParameterValueWithMatrixModulation(m_fMultibandCompressorGainMB, MODMATDEST::CompressorMidGain, &inputState));
		}
		makeUpGainMB = m_fMultibandCompressorGainMB_smoothed.getNextValue();

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
	float thresholdHB = *m_fMultibandCompressorThresholdHB;
	float tauAttackHB = *m_fMultibandCompressorAttackHB;
	float tauReleaseHB = *m_fMultibandCompressorReleaseHB;
	float ratioHB = *m_fMultibandCompressorRatioHB;
	float makeUpGainHB = *m_fMultibandCompressorGainHB;

	float alphaAttackHB = exp(-1 / (0.001 * m_iSampleRate * tauAttackHB));
	float alphaReleaseHB = exp(-1 / (0.001 * m_iSampleRate * tauReleaseHB));
	for (int i = 0; i < numSamples; ++i)
	{
		inputState.currentFrame = i;
		if (m_Set->modMatrixDestinationSetFast(MODMATDEST::CompressorThresholdHighBand)) {
			m_fMultibandCompressorThresholdHB_smoothed.setValue(m_Set->getParameterValueWithMatrixModulation(m_fMultibandCompressorThresholdHB, MODMATDEST::CompressorThresholdHighBand, &inputState));
		}
		thresholdHB = m_fMultibandCompressorThresholdHB_smoothed.getNextValue();

		if (m_Set->modMatrixDestinationSetFast(MODMATDEST::CompressorRatioHighBand)) {
			m_fMultibandCompressorRatioHB_smoothed.setValue(m_Set->getParameterValueWithMatrixModulation(m_fMultibandCompressorRatioHB, MODMATDEST::CompressorRatioHighBand, &inputState));
		}
		ratioHB = m_fMultibandCompressorRatioHB_smoothed.getNextValue();

		if (m_Set->modMatrixDestinationSetFast(MODMATDEST::CompressorHighGain)) {
			m_fMultibandCompressorGainHB_smoothed.setValue(m_Set->getParameterValueWithMatrixModulation(m_fMultibandCompressorGainHB, MODMATDEST::CompressorHighGain, &inputState));
		}
		makeUpGainHB = m_fMultibandCompressorGainHB_smoothed.getNextValue();

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

void CVASTMultibandCompressor::getStateInformation (MemoryBlock& destData)
{
    //ScopedPointer<XmlElement> xml (parameters.valueTreeState.state.createXml());
    //copyXmlToBinary (*xml, destData);
}

void CVASTMultibandCompressor::setStateInformation (const void* data, int sizeInBytes)
{
    //ScopedPointer<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    //if (xmlState != nullptr)
      //  if (xmlState->hasTagName (parameters.valueTreeState.state.getType()))
        //    parameters.valueTreeState.state = ValueTree::fromXml (*xmlState);
}
