/*
VAST Dynamics Audio Software (TM)

All modulators tested: OK
*/

#include "VASTEq.h"
#include "../VASTEngineHeader.h"
#include "../VASTSettings.h"
#include "../Filter/VASTBiQuadFilter.h"
#include "../../Plugin/VASTAudioProcessor.h"
#include "VASTMultibandCompressor.h" //IIR Filter!
#include "VASTEffect.h"

CVASTEq::CVASTEq(VASTAudioProcessor* processor, int busnr) {
	my_processor = processor;
	myBusnr = busnr;

	m_bIsOff = true;
	m_bShallBeOff = true;
	m_iSoftFade = 0;

	setEffectName("EQUALIZER");
}

void CVASTEq::initParameters() {
	AudioProcessorValueTreeState& parameters = my_processor->getParameterTree();

	createAndAddParameter(&m_bEQOnOff, parameters, 1, "m_bEQOnOff", "EQ on / off", "On", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 1.0f, 1.0f), 0.0f,
		CVASTParamState::toggleButtonValueToTextFunction,
		CVASTParamState::toggleButtonTextToValueFunction,
		false, true, true, true,
		true);

	createAndAddParameter(&m_fEQ1, parameters, 1, "m_fEQ1", "EQ low shelf", "Low", 1,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(-20, 20), 0,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);
	createAndAddParameter(&m_fEQ2, parameters, 1, "m_fEQ2", "EQ notch at +/- 250hz", "250hz", 2,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(-20, 20), 0,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);
	createAndAddParameter(&m_fEQ3, parameters, 1, "m_fEQ3", "EQ notch at +/- 500hz", "500hz", 3,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(-20, 20), 0,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);
	createAndAddParameter(&m_fEQ4, parameters, 1, "m_fEQ4", "EQ notch at +/- 1kHz", "1kHz", 4,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(-20, 20), 0,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);
	createAndAddParameter(&m_fEQ5, parameters, 1, "m_fEQ5", "EQ notch at +/- 2kHz", "2kHz", 5,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(-20, 20), 0,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);
	createAndAddParameter(&m_fEQ6, parameters, 1, "m_fEQ6", "EQ notch at +/- 4kHz", "4kHz", 6,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(-20, 20), 0,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);
	createAndAddParameter(&m_fEQ7, parameters, 1, "m_fEQ7", "EQ notch at +/- 8kHz", "8kHz", 7,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(-20, 20), 0,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);
	createAndAddParameter(&m_fEQ8, parameters, 1, "m_fEQ8", "EQ high shelf", "High", 8,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(-20, 20), 0,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);
	createAndAddParameter(&m_fEQGain, parameters, 1, "m_fEQGain", "EQ output gain", "Gain", 9,
		MODMATDEST::EQGain,
		NormalisableRange<float>(0, 200), 100,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

}

void CVASTEq::initCompatibilityParameters() {
	// all new parameters go here
}

CVASTEq::~CVASTEq(void) {
	AudioProcessorValueTreeState& parameters = ((VASTAudioProcessor*)my_processor)->getParameterTree();
	Array<AudioProcessorParameterWithID*> params = getParameters();
	for (int i = 0; i < params.size(); i++) {
		parameters.removeParameterListener(params[i]->paramID, (AudioProcessorValueTreeState::Listener*)this);
	}
}

void CVASTEq::releaseResources() {

}

void CVASTEq::init(CVASTSettings &set) {
	m_Set = &set;
	initParameters();
	
	reset();
}

void CVASTEq::reset() {
	
	m_biQuad1L.flushDelays();
	m_biQuad2L.flushDelays();
	m_biQuad3L.flushDelays();
	m_biQuad4L.flushDelays();
	m_biQuad5L.flushDelays();
	m_biQuad6L.flushDelays();
	m_biQuad7L.flushDelays();
	m_biQuad8L.flushDelays();

	m_biQuad1R.flushDelays();
	m_biQuad2R.flushDelays();
	m_biQuad3R.flushDelays();
	m_biQuad4R.flushDelays();
	m_biQuad5R.flushDelays();
	m_biQuad6R.flushDelays();
	m_biQuad7R.flushDelays();
	m_biQuad8R.flushDelays();

	/*
	m_biQuad1L.reset();
	m_biQuad2L.reset();
	m_biQuad3L.reset();
	m_biQuad4L.reset();
	m_biQuad5L.reset();
	m_biQuad6L.reset();
	m_biQuad7L.reset();
	m_biQuad8L.reset();

	m_biQuad1R.reset();
	m_biQuad2R.reset();
	m_biQuad3R.reset();
	m_biQuad4R.reset();
	m_biQuad5R.reset();
	m_biQuad6R.reset();
	m_biQuad7R.reset();
	m_biQuad8R.reset();
	*/

	if ((m_bIsOff == false) && (m_bShallBeOff == false)) {
		m_iSoftFade = 0;
		m_bShallBeOff = false; //refade in
		m_bIsOff = true; 
	}
}

void CVASTEq::prepareToPlay(double, int samplesPerBlock) {
	//m_iSampleRate is set in useroversampling
	m_iExpectedSamplesPerBlock = samplesPerBlock;
	
	const double smoothTime = 0.02;
	m_fEQGain_smoothed.reset(m_iSampleRate, smoothTime);

	updateVariables();
	reset();
}

void CVASTEq::parameterChanged(const String& parameterID, float newValue) {
	float fQ = float(sqrt2over2); //was 1.0f
		
	if (parameterID.startsWith("m_bEQOnOff")) {
		if (newValue == static_cast<int>(SWITCH::SWITCH_ON))
			switchOn();
		else
			switchOff();
		return;
	} 
	
	/*/
	else if (parameterID.startsWith("m_fEQ1")) {
		double V = powf(10.0f, *m_fEQ1 / 12.0f);
		m_biQuad1L.setCoefficients(m_coeff.makeLowShelf(m_iSampleRate, 50.f, fQ, V));
		m_biQuad1R.setCoefficients(m_coeff.makeLowShelf(m_iSampleRate, 50.f, fQ, V));
	}

	else if (parameterID.startsWith("m_fEQ2")) {
		double V = powf(10.0f, *m_fEQ2 / 20.0f);
		m_biQuad2L.setCoefficients(m_coeff.makePeakFilter(m_iSampleRate, 250.f, fQ, V));
		m_biQuad2R.setCoefficients(m_coeff.makePeakFilter(m_iSampleRate, 250.f, fQ, V));
	}

	else if (parameterID.startsWith("m_fEQ3")) {
		double V = powf(10.0f, *m_fEQ3 / 20.0f);
		m_biQuad3L.setCoefficients(m_coeff.makePeakFilter(m_iSampleRate, 500.f, fQ, V));
		m_biQuad3R.setCoefficients(m_coeff.makePeakFilter(m_iSampleRate, 500.f, fQ, V));
	}

	else if (parameterID.startsWith("m_fEQ4")) {
		double V = powf(10.0f, *m_fEQ4 / 20.0f);
		m_biQuad4L.setCoefficients(m_coeff.makePeakFilter(m_iSampleRate, 1000.f, fQ, V));
		m_biQuad4R.setCoefficients(m_coeff.makePeakFilter(m_iSampleRate, 1000.f, fQ, V));
	}

	else if (parameterID.startsWith("m_fEQ5")) {
		double V = powf(10.0f, *m_fEQ5 / 20.0f);
		m_biQuad5L.setCoefficients(m_coeff.makePeakFilter(m_iSampleRate, 2000.f, fQ, V));
		m_biQuad5R.setCoefficients(m_coeff.makePeakFilter(m_iSampleRate, 2000.f, fQ, V));
	}

	else if (parameterID.startsWith("m_fEQ6")) {
		double V = powf(10.0f, *m_fEQ6 / 20.0f);
		m_biQuad6L.setCoefficients(m_coeff.makePeakFilter(m_iSampleRate, 4000.f, fQ, V));
		m_biQuad6R.setCoefficients(m_coeff.makePeakFilter(m_iSampleRate, 4000.f, fQ, V));
	}

	else if (parameterID.startsWith("m_fEQ7")) {
		double V = powf(10.0f, *m_fEQ7 / 20.0f);
		m_biQuad7L.setCoefficients(m_coeff.makePeakFilter(m_iSampleRate, 8000.f, fQ, V));
		m_biQuad7R.setCoefficients(m_coeff.makePeakFilter(m_iSampleRate, 8000.f, fQ, V));
	}

	else if (parameterID.startsWith("m_fEQ8")) {
		double V = powf(10.0f, *m_fEQ8 / 20.0f);
		m_biQuad8L.setCoefficients(m_coeff.makeHighShelf(m_iSampleRate, 16000.f, fQ, V));
		m_biQuad8R.setCoefficients(m_coeff.makeHighShelf(m_iSampleRate, 16000.f, fQ, V));
	}

	else if (parameterID.startsWith("m_fEQGain")) {
		m_fEQGain_smoothed.setValue(newValue);
	}
	*/

	else if (parameterID.startsWith("m_fEQ1")) {
		m_biQuad1L.calcBiquad(CVASTBiQuad::LOWSHELF, 50.0f, m_iSampleRate, fQ, *m_fEQ1);
		m_biQuad1R.copySettingsFrom(&m_biQuad1L);
	}

	else if (parameterID.startsWith("m_fEQ2")) {
		m_biQuad2L.calcBiquad(CVASTBiQuad::PEAK, 250.0f, m_iSampleRate, fQ, *m_fEQ2);
		m_biQuad2R.copySettingsFrom(&m_biQuad2L);
	}

	else if (parameterID.startsWith("m_fEQ3")) {
		m_biQuad3L.calcBiquad(CVASTBiQuad::PEAK, 500.0f, m_iSampleRate, fQ, *m_fEQ3);
		m_biQuad3R.copySettingsFrom(&m_biQuad3L);
	}

	else if (parameterID.startsWith("m_fEQ4")) {
		m_biQuad4L.calcBiquad(CVASTBiQuad::PEAK, 1000.0f, m_iSampleRate, fQ, *m_fEQ4);
		m_biQuad4R.copySettingsFrom(&m_biQuad4L);
	}

	else if (parameterID.startsWith("m_fEQ5")) {
		m_biQuad5L.calcBiquad(CVASTBiQuad::PEAK, 2000.0f, m_iSampleRate, fQ, *m_fEQ5);
		m_biQuad5R.copySettingsFrom(&m_biQuad5L);
	}

	else if (parameterID.startsWith("m_fEQ6")) {
		m_biQuad6L.calcBiquad(CVASTBiQuad::PEAK, 4000.0f, m_iSampleRate, fQ, *m_fEQ6);
		m_biQuad6R.copySettingsFrom(&m_biQuad6L);
	}

	else if (parameterID.startsWith("m_fEQ7")) {
		m_biQuad7L.calcBiquad(CVASTBiQuad::PEAK, 8000.0f, m_iSampleRate, fQ, *m_fEQ7);
		m_biQuad7R.copySettingsFrom(&m_biQuad7L);
	}

	else if (parameterID.startsWith("m_fEQ8")) {
		m_biQuad8L.calcBiquad(CVASTBiQuad::HIGHSHELF, 16000.0f, m_iSampleRate, fQ, *m_fEQ8);
		m_biQuad8R.copySettingsFrom(&m_biQuad8L);
	}

	else if (parameterID.startsWith("m_fEQGain")) {
		m_fEQGain_smoothed.setTargetValue(newValue);
	}
}

void CVASTEq::updateVariables() {

	// Biquad type PEAK (with BOOST & CUT)
	// see: http://www.musicdsp.org/files/biquad.c

	float fQ = float(sqrt2over2); //was 1.0f

	m_biQuad1L.calcBiquad(CVASTBiQuad::LOWSHELF, 50.0f, m_iSampleRate, fQ, *m_fEQ1);
	m_biQuad1R.copySettingsFrom(&m_biQuad1L);
	
	m_biQuad2L.calcBiquad(CVASTBiQuad::PEAK, 250.0f, m_iSampleRate, fQ, *m_fEQ2);
	m_biQuad2R.copySettingsFrom(&m_biQuad2L);

	m_biQuad3L.calcBiquad(CVASTBiQuad::PEAK, 500.0f, m_iSampleRate, fQ, *m_fEQ3);
	m_biQuad3R.copySettingsFrom(&m_biQuad3L);

	m_biQuad4L.calcBiquad(CVASTBiQuad::PEAK, 1000.0f, m_iSampleRate, fQ, *m_fEQ4);
	m_biQuad4R.copySettingsFrom(&m_biQuad4L);

	m_biQuad5L.calcBiquad(CVASTBiQuad::PEAK, 2000.0f, m_iSampleRate, fQ, *m_fEQ5);
	m_biQuad5R.copySettingsFrom(&m_biQuad5L);

	m_biQuad6L.calcBiquad(CVASTBiQuad::PEAK, 4000.0f, m_iSampleRate, fQ, *m_fEQ6);
	m_biQuad6R.copySettingsFrom(&m_biQuad6L);

	m_biQuad7L.calcBiquad(CVASTBiQuad::PEAK, 8000.0f, m_iSampleRate, fQ, *m_fEQ7);
	m_biQuad7R.copySettingsFrom(&m_biQuad7L);

	m_biQuad8L.calcBiquad(CVASTBiQuad::HIGHSHELF, 16000.0f, m_iSampleRate, fQ, *m_fEQ8);
	m_biQuad8R.copySettingsFrom(&m_biQuad8L);

	/*
	m_biQuad1L.setCoefficients(m_coeff.makeLowShelf(m_iSampleRate, 50.f, fQ, powf(10.0f, *m_fEQ1 / 12.0f)));
	m_biQuad1R.setCoefficients(m_coeff.makeLowShelf(m_iSampleRate, 50.f, fQ, powf(10.0f, *m_fEQ1 / 12.0f)));

	m_biQuad2L.setCoefficients(m_coeff.makePeakFilter(m_iSampleRate, 250.f, fQ, powf(10.0f, *m_fEQ2 / 20.0f)));
	m_biQuad2R.setCoefficients(m_coeff.makePeakFilter(m_iSampleRate, 250.f, fQ, powf(10.0f, *m_fEQ2 / 20.0f)));

	m_biQuad3L.setCoefficients(m_coeff.makePeakFilter(m_iSampleRate, 500.f, fQ, powf(10.0f, *m_fEQ3 / 20.0f)));
	m_biQuad3R.setCoefficients(m_coeff.makePeakFilter(m_iSampleRate, 500.f, fQ, powf(10.0f, *m_fEQ3 / 20.0f)));

	m_biQuad4L.setCoefficients(m_coeff.makePeakFilter(m_iSampleRate, 1000.f, fQ, powf(10.0f, *m_fEQ4 / 20.0f)));
	m_biQuad4R.setCoefficients(m_coeff.makePeakFilter(m_iSampleRate, 1000.f, fQ, powf(10.0f, *m_fEQ4 / 20.0f)));

	m_biQuad5L.setCoefficients(m_coeff.makePeakFilter(m_iSampleRate, 2000.f, fQ, powf(10.0f, *m_fEQ5 / 20.0f)));
	m_biQuad5R.setCoefficients(m_coeff.makePeakFilter(m_iSampleRate, 2000.f, fQ, powf(10.0f, *m_fEQ5 / 20.0f)));

	m_biQuad6L.setCoefficients(m_coeff.makePeakFilter(m_iSampleRate, 4000.f, fQ, powf(10.0f, *m_fEQ6 / 20.0f)));
	m_biQuad6R.setCoefficients(m_coeff.makePeakFilter(m_iSampleRate, 4000.f, fQ, powf(10.0f, *m_fEQ6 / 20.0f)));

	m_biQuad7L.setCoefficients(m_coeff.makePeakFilter(m_iSampleRate, 8000.f, fQ, powf(10.0f, *m_fEQ7 / 20.0f)));
	m_biQuad7R.setCoefficients(m_coeff.makePeakFilter(m_iSampleRate, 8000.f, fQ, powf(10.0f, *m_fEQ7 / 20.0f)));

	m_biQuad8L.setCoefficients(m_coeff.makeHighShelf(m_iSampleRate, 16000.f, fQ, powf(10.0f, *m_fEQ8 / 20.0f)));
	m_biQuad8R.setCoefficients(m_coeff.makeHighShelf(m_iSampleRate, 16000.f, fQ, powf(10.0f, *m_fEQ8 / 20.0f)));
	*/
}

void CVASTEq::processBlock(AudioSampleBuffer& buffer, MidiBuffer&, const int numSamples) {
	if (isOffAndShallBeOff() == true) return;

	float* bufferWritePointerL = buffer.getWritePointer(0);
	float* bufferWritePointerR = buffer.getWritePointer(1);

	for (int i = 0; i < numSamples; i++) {
		checkSoftFade();

		float fIn[2];
		float fOut[2];
		fIn[0] = bufferWritePointerL[i];
		fIn[1] = bufferWritePointerR[i];

		processAudioFrame(fIn, fOut, 2, 2);

#ifdef _DEBUG
		//Safety check
		jassert((fOut[0] >= -3.f) && (fOut[0] <= 3.f));
		jassert((fOut[1] >= -3.f) && (fOut[1] <= 3.f));
#endif
		float fWet = float(m_iSoftFade) / float(C_MAX_SOFTFADE);
		fOut[0] = (1.0f - fWet) * fIn[0] + fWet * fOut[0];
		fOut[1] = (1.0f - fWet) * fIn[1] + fWet * fOut[1];

		bufferWritePointerL[i] = fOut[0];
		bufferWritePointerR[i] = fOut[1];
	}

	modMatrixInputState inputState = ((VASTAudioProcessor*)my_processor)->m_pVASTXperience.m_Poly.getOldestNotePlayedInputState(0); // make parameter oldest or newest
	//Gain Mod
	m_fEQGain_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_fEQGain, MODMATDEST::EQGain, &inputState));
	float lEQGain = m_fEQGain_smoothed.getNextValue();
	m_fEQGain_smoothed.skip(numSamples - 1);

	buffer.applyGain(lEQGain * 0.01f);
	
	//=================================================================================================	
}

bool CVASTEq::processAudioFrame(float* pInputBuffer, float* pOutputBuffer, MYUINT, MYUINT)
{
	//if (m_iSoftFade != C_MAX_SOFTFADE)
		//updateVariables();

	/*
		//Left
		float fEQOut1L = 0;
		float fEQOut2L = 0;
		float fEQOut3L = 0;
		float fEQOut4L = 0;
		float fEQOut5L = 0;
		float fEQOut6L = 0;
		float fEQOut7L = 0;
		float fEQOut8L = 0;

		if (*m_fEQ1 != 0.0f)
		fEQOut1L = m_biQuad1L.processSingleSampleRaw(pInputBuffer[0]);
		else
		fEQOut1L = pInputBuffer[0];
		if (*m_fEQ2 != 0.0f)
		fEQOut2L = m_biQuad2L.processSingleSampleRaw(fEQOut1L);
		else
		fEQOut2L = fEQOut1L;
		if (*m_fEQ3 != 0.0f)
		fEQOut3L = m_biQuad3L.processSingleSampleRaw(fEQOut2L);
		else
		fEQOut3L = fEQOut2L;
		if (*m_fEQ4 != 0.0f)
		fEQOut4L = m_biQuad4L.processSingleSampleRaw(fEQOut3L);
		else
		fEQOut4L = fEQOut3L;
		if (*m_fEQ5 != 0.0f)
		fEQOut5L = m_biQuad5L.processSingleSampleRaw(fEQOut4L);
		else
		fEQOut5L = fEQOut4L;
		if (*m_fEQ6 != 0.0f)
		fEQOut6L = m_biQuad6L.processSingleSampleRaw(fEQOut5L);
		else
		fEQOut6L = fEQOut5L;
		if (*m_fEQ7 != 0.0f)
		fEQOut7L = m_biQuad7L.processSingleSampleRaw(fEQOut6L);
		else
		fEQOut7L = fEQOut6L;
		if (*m_fEQ8 != 0.0f)
		fEQOut8L = m_biQuad8L.processSingleSampleRaw(fEQOut7L);
		else
		fEQOut8L = fEQOut7L;
		pOutputBuffer[0] = fEQOut8L;

		//Right
		float fEQOut1R = 0;
		float fEQOut2R = 0;
		float fEQOut3R = 0;
		float fEQOut4R = 0;
		float fEQOut5R = 0;
		float fEQOut6R = 0;
		float fEQOut7R = 0;
		float fEQOut8R = 0;

		if (*m_fEQ1 != 0.0f)
		fEQOut1R = m_biQuad1R.processSingleSampleRaw(pInputBuffer[1]);
		else
		fEQOut1R = pInputBuffer[1];
		if (*m_fEQ2 != 0.0f)
		fEQOut2R = m_biQuad2R.processSingleSampleRaw(fEQOut1R);
		else
		fEQOut2R = fEQOut1R;
		if (*m_fEQ3 != 0.0f)
		fEQOut3R = m_biQuad3R.processSingleSampleRaw(fEQOut2R);
		else
		fEQOut3R = fEQOut2R;
		if (*m_fEQ4 != 0.0f)
		fEQOut4R = m_biQuad4R.processSingleSampleRaw(fEQOut3R);
		else
		fEQOut4R = fEQOut3R;
		if (*m_fEQ5 != 0.0f)
		fEQOut5R = m_biQuad5R.processSingleSampleRaw(fEQOut4R);
		else
		fEQOut5R = fEQOut4R;
		if (*m_fEQ6 != 0.0f)
		fEQOut6R = m_biQuad6R.processSingleSampleRaw(fEQOut5R);
		else
		fEQOut6R = fEQOut5R;
		if (*m_fEQ7 != 0.0f)
		fEQOut7R = m_biQuad7R.processSingleSampleRaw(fEQOut6R);
		else
		fEQOut7R = fEQOut6R;
		if (*m_fEQ8 != 0.0f)
		fEQOut8R = m_biQuad8R.processSingleSampleRaw(fEQOut7R);
		else
		fEQOut8R = fEQOut7R;
		pOutputBuffer[1] = fEQOut8R;

		//vassert((pOutputBuffer[0] > -10.0f) && (pOutputBuffer[0] <= 10.0));
		//vassert((pOutputBuffer[1] > -10.0f) && (pOutputBuffer[1] <= 10.0));
		*/

	//Left
	float fEQOut1L = 0;
	float fEQOut2L = 0;
	float fEQOut3L = 0;
	float fEQOut4L = 0;
	float fEQOut5L = 0;
	float fEQOut6L = 0;
	float fEQOut7L = 0;
	float fEQOut8L = 0;

	if (*m_fEQ1 != 0.0f)
		fEQOut1L = m_biQuad1L.doBiQuad(pInputBuffer[0]);
	else
		fEQOut1L = pInputBuffer[0];
	if (*m_fEQ2 != 0.0f)
		fEQOut2L = m_biQuad2L.doBiQuad(fEQOut1L);
	else
		fEQOut2L = fEQOut1L;
	if (*m_fEQ3 != 0.0f)
		fEQOut3L = m_biQuad3L.doBiQuad(fEQOut2L);
	else
		fEQOut3L = fEQOut2L;
	if (*m_fEQ4 != 0.0f)
		fEQOut4L = m_biQuad4L.doBiQuad(fEQOut3L);
	else
		fEQOut4L = fEQOut3L;
	if (*m_fEQ5 != 0.0f)
		fEQOut5L = m_biQuad5L.doBiQuad(fEQOut4L);
	else
		fEQOut5L = fEQOut4L;
	if (*m_fEQ6 != 0.0f)
		fEQOut6L = m_biQuad6L.doBiQuad(fEQOut5L);
	else
		fEQOut6L = fEQOut5L;
	if (*m_fEQ7 != 0.0f)
		fEQOut7L = m_biQuad7L.doBiQuad(fEQOut6L);
	else
		fEQOut7L = fEQOut6L;
	if (*m_fEQ8 != 0.0f)
		fEQOut8L = m_biQuad8L.doBiQuad(fEQOut7L);
	else
		fEQOut8L = fEQOut7L;
	pOutputBuffer[0] = fEQOut8L;

	//Right
	float fEQOut1R = 0;
	float fEQOut2R = 0;
	float fEQOut3R = 0;
	float fEQOut4R = 0;
	float fEQOut5R = 0;
	float fEQOut6R = 0;
	float fEQOut7R = 0;
	float fEQOut8R = 0;

	if (*m_fEQ1 != 0.0f)
		fEQOut1R = m_biQuad1R.doBiQuad(pInputBuffer[1]);
	else
		fEQOut1R = pInputBuffer[1];
	if (*m_fEQ2 != 0.0f)
		fEQOut2R = m_biQuad2R.doBiQuad(fEQOut1R);
	else
		fEQOut2R = fEQOut1R;
	if (*m_fEQ3 != 0.0f)
		fEQOut3R = m_biQuad3R.doBiQuad(fEQOut2R);
	else
		fEQOut3R = fEQOut2R;
	if (*m_fEQ4 != 0.0f)
		fEQOut4R = m_biQuad4R.doBiQuad(fEQOut3R);
	else
		fEQOut4R = fEQOut3R;
	if (*m_fEQ5 != 0.0f)
		fEQOut5R = m_biQuad5R.doBiQuad(fEQOut4R);
	else
		fEQOut5R = fEQOut4R;
	if (*m_fEQ6 != 0.0f)
		fEQOut6R = m_biQuad6R.doBiQuad(fEQOut5R);
	else
		fEQOut6R = fEQOut5R;
	if (*m_fEQ7 != 0.0f)
		fEQOut7R = m_biQuad7R.doBiQuad(fEQOut6R);
	else
		fEQOut7R = fEQOut6R;
	if (*m_fEQ8 != 0.0f)
		fEQOut8R = m_biQuad8R.doBiQuad(fEQOut7R);
	else
		fEQOut8R = fEQOut7R;
	pOutputBuffer[1] = fEQOut8R;

	//vassert((pOutputBuffer[0] > -10.0f) && (pOutputBuffer[0] <= 10.0));
	//vassert((pOutputBuffer[1] > -10.0f) && (pOutputBuffer[1] <= 10.0));

	return true;
}

//==============================================================================

void CVASTEq::getStateInformation(MemoryBlock&)
{
	//std::unique_ptr<XmlElement> xml (parameters.valueTreeState.state.createXml());
	//copyXmlToBinary (*xml, destData);
}

void CVASTEq::setStateInformation(const void*, int)
{
	//std::unique_ptr<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
	//if (xmlState != nullptr)
	//  if (xmlState->hasTagName (parameters.valueTreeState.state.getType()))
	//    parameters.valueTreeState.state = ValueTree::fromXml (*xmlState);
}
