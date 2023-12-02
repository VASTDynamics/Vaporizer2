/*
VAST Dynamics Audio Software (TM)

All modulators tested: OK
*/

#include "VASTDistortion.h"
#include "../Filter/VASTBiQuadFilter.h"
#include "../VASTEngineHeader.h"
#include "../../Plugin/VASTAudioProcessor.h"
#include "VASTEffect.h"
#include <math.h>

CVASTDistortion::CVASTDistortion(VASTAudioProcessor* processor, int busnr) {
	my_processor = processor;
	myBusnr = busnr;
	setEffectName("DISTORTION");

	m_bIsOff = true;
	m_bShallBeOff = true;
	m_iSoftFade = 0;
}

void CVASTDistortion::initParameters() {
	AudioProcessorValueTreeState& parameters = my_processor->getParameterTree();

	createAndAddParameter(&m_bDistortionOnOff, parameters, 1, "m_bDistortionOnOff", "Distortion effect on / off", "On", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 1.0f, 1.0f), 0.0f,
		CVASTParamState::toggleButtonValueToTextFunction,
		CVASTParamState::toggleButtonTextToValueFunction,
		false, true, true, true,
		true);

	createAndAddParameter(&m_fDistDryWet, parameters, 1, "m_fDistDryWet", "Distortion dry / wet (no effect - full effect)", "DryWet", 1,
		MODMATDEST::DistortionDryWet,
		NormalisableRange<float>(0, 100), 10.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	createAndAddParameter(&m_fDistPreGain, parameters, 1, "m_fDistPreGain", "Distortion pre-gain", "PreGain", 2,
		MODMATDEST::DistortionPreGain, 
		NormalisableRange<float>(0, 200), 100,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	createAndAddParameter(&m_fDistLowcut, parameters, 1, "m_fDistLowcut", "Distortion lowcut frequency", "Lowcut", 3,
		MODMATDEST::DistortionLowCut,
		NormalisableRange<float>(50.f, 18000.f, 0.001f, 0.3f, false), 50.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);
	createAndAddParameter(&m_fDistDrive, parameters, 1, "m_fDistDrive", "Distortion drive (pre gain before waveshaping)", "Drive", 4,
		MODMATDEST::DistortionDrive,
		NormalisableRange<float>(0, 100), 30.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);
	createAndAddParameter(&m_fDistFuzz, parameters, 1, "m_fDistFuzz", "Distortion fuzz level", "Fuzz", 5,
		MODMATDEST::DistortionFuzz,
		NormalisableRange<float>(0, 100), 10.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);
	createAndAddParameter(&m_fDistGain, parameters, 1, "m_fDistGain", "Distortion output gain", "Gain", 6,
		MODMATDEST::DistortionGain,
		NormalisableRange<float>(0, 200), 100,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);
}

void CVASTDistortion::initCompatibilityParameters() {
	// all new parameters go here
}

CVASTDistortion::~CVASTDistortion(void) {
	AudioProcessorValueTreeState& parameters = ((VASTAudioProcessor*)my_processor)->getParameterTree();
	Array<AudioProcessorParameterWithID*> params = getParameters();
	for (int i = 0; i < params.size(); i++) {
		parameters.removeParameterListener(params[i]->paramID, (AudioProcessorValueTreeState::Listener*)this);
	}
}

void CVASTDistortion::parameterChanged(const String& parameterID, float newValue) {
	if (parameterID.startsWith("m_bDistortionOnOff")) {
		if (newValue == static_cast<int>(SWITCH::SWITCH_ON))
			switchOn();
		else
			switchOff();
	}
	else if (parameterID.startsWith("m_fDistDryWet")) {
		m_fDistDryWet_smoothed.setTargetValue(newValue);
	}
	else if (parameterID.startsWith("m_fDistDrive")) {
		m_fDistDrive_smoothed.setTargetValue(newValue);
	}
	else if (parameterID.startsWith("m_fDistFuzz")) {
		m_fDistFuzz_smoothed.setTargetValue(newValue);
	}
	else if (parameterID.startsWith("m_fDistLowcut")) {
		m_fDistLowcut_smoothed.setTargetValue(newValue);
	}
	else if (parameterID.startsWith("m_fDistGain")) {
		m_fDistGain_smoothed.setTargetValue(newValue);
	}
	else if (parameterID.startsWith("m_fDistPreGain")) {
		m_fDistPreGain_smoothed.setTargetValue(newValue);
	}
}

void CVASTDistortion::init(CVASTSettings &set) {
	m_Set = &set;
	initParameters();

	m_biQuadPreL.flushDelays();
	m_biQuadPreR.flushDelays();
	m_biQuadPostL.flushDelays();
	m_biQuadPostR.flushDelays();

	m_lowCutBiquadL.flushDelays();
	m_lowCutBiquadR.flushDelays();

	float fQ = 1.0f;
	float preShelfGain = -20.0f;
	m_biQuadPreL.calcBiquad(CVASTBiQuad::LOWSHELF, 800.0f, m_iSampleRate, fQ, preShelfGain);
	m_biQuadPreR.calcBiquad(CVASTBiQuad::LOWSHELF, 800.0f, m_iSampleRate, fQ, preShelfGain);

	float postShelfGain = -20.0f;
	m_biQuadPostL.calcBiquad(CVASTBiQuad::HIGHSHELF, 2000.0f, m_iSampleRate, fQ, postShelfGain);
	m_biQuadPostR.calcBiquad(CVASTBiQuad::HIGHSHELF, 2000.0f, m_iSampleRate, fQ, postShelfGain);

	m_lowCutBiquadL.calcBiquad(CVASTBiQuad::HIGHPASS, 50.0f, m_iSampleRate, sqrt2over2, -18.0f);
	m_lowCutBiquadR.copySettingsFrom(&m_lowCutBiquadL);
	reset();
}

void CVASTDistortion::reset() {
	m_biQuadPreL.flushDelays();
	m_biQuadPreR.flushDelays();
	m_biQuadPostL.flushDelays();
	m_biQuadPostR.flushDelays();

	m_lowCutBiquadL.flushDelays();
	m_lowCutBiquadR.flushDelays();

	if ((m_bIsOff == false) && (m_bShallBeOff == false)) {
		m_iSoftFade = 0;
		m_bShallBeOff = false; //refade in
		m_bIsOff = true;
	}
}

void CVASTDistortion::prepareToPlay(double, int samplesPerBlock) {
	//m_iSampleRate is set in useroversampling
	m_iExpectedSamplesPerBlock = samplesPerBlock;

	float fQ = 1.0;
	float preShelfGain = -20.0f;
	m_biQuadPreL.calcBiquad(CVASTBiQuad::LOWSHELF, 800.0f, m_iSampleRate, fQ, preShelfGain);
	m_biQuadPreR.calcBiquad(CVASTBiQuad::LOWSHELF, 800.0f, m_iSampleRate, fQ, preShelfGain);

	float postShelfGain = -20.0f;
	m_biQuadPostL.calcBiquad(CVASTBiQuad::HIGHSHELF, 2000.0f, m_iSampleRate, fQ, postShelfGain);
	m_biQuadPostR.calcBiquad(CVASTBiQuad::HIGHSHELF, 2000.0f, m_iSampleRate, fQ, postShelfGain);

	m_lowCutBiquadL.calcBiquad(CVASTBiQuad::HIGHPASS, 50.0f, m_iSampleRate, sqrt2over2, -18.0f);
	m_lowCutBiquadR.copySettingsFrom(&m_lowCutBiquadL);

	const double smoothTime = 0.02;
	m_fDistDryWet_smoothed.reset(m_iSampleRate, smoothTime);
	m_fDistDrive_smoothed.reset(m_iSampleRate, smoothTime);
	m_fDistFuzz_smoothed.reset(m_iSampleRate, smoothTime);
	m_fDistLowcut_smoothed.reset(m_iSampleRate, smoothTime);
	m_fDistGain_smoothed.reset(m_iSampleRate, smoothTime);
	m_fDistPreGain_smoothed.reset(m_iSampleRate, smoothTime);

	reset();
}

void CVASTDistortion::releaseResources()
{
}

void CVASTDistortion::processBlock(AudioSampleBuffer& buffer, MidiBuffer&, const int numSamples) {
	if (isOffAndShallBeOff() == true) return;

	modMatrixInputState inputState;

	float* bufferWritePointerL = buffer.getWritePointer(0);
	float* bufferWritePointerR = buffer.getWritePointer(1);

	for (int currentFrame = 0; currentFrame < numSamples; currentFrame += 1) {
		int currentFrameOSAdjusted = currentFrame;
		if (m_bOversampling) currentFrameOSAdjusted /= C_OVERSAMPLING_RATIO;
		checkSoftFade();

		inputState = ((VASTAudioProcessor*)my_processor)->m_pVASTXperience.m_Poly.getOldestNotePlayedInputState(currentFrameOSAdjusted); // make parameter oldest or newest

		float fIn[2];
		float fOut[2];
		fIn[0] = bufferWritePointerL[currentFrame];
		fIn[1] = bufferWritePointerR[currentFrame];
		
		processAudioFrame(fIn, fOut, 2, 2, inputState);

		m_fDistGain_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_fDistGain, MODMATDEST::DistortionGain, &inputState));
		float lDistGain = m_fDistGain_smoothed.getNextValue();

		bufferWritePointerL[currentFrame] = fOut[0] * lDistGain * 0.01f;
		bufferWritePointerR[currentFrame] = fOut[1] * lDistGain * 0.01f;

		vassert((fOut[0] > -10.0f) && (fOut[0] <= 10.0));
		vassert((fOut[1] > -10.0f) && (fOut[1] <= 10.0));
		if (fOut[0] <= -10.0f) {
			fOut[0] = -10.0f;
			reset();
		}
		if (fOut[0]  >= 10.0f) {
			fOut[0] = 10.0f;
			reset();
		}
		if (fOut[1] <= -10.0f) {
			fOut[1] = -10.0f;
			reset();
		}
		if (fOut[1] >= 10.0f) {
			fOut[1] = 10.0f;
			reset();
		}
	}
}

bool CVASTDistortion::processAudioFrame(float* pInputBuffer, float* pOutputBuffer, MYUINT uNumInputChannels, MYUINT, modMatrixInputState &inputState)
{
	//pre - eq(bandpass) > pre - gain > nonlinear waveshaping function > post - eq(lowpass) > post - gain and you may wish to eliminate aliasing using the bandlimited polynomials or by oversampling.
	//http://music.columbia.edu/cmc/music-dsp/FAQs/guitar_distortion_FAQ.html
		
	//DryWet Mod
	m_fDistDryWet_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_fDistDryWet, MODMATDEST::DistortionDryWet, &inputState));
	float lDistortionWet = m_fDistDryWet_smoothed.getNextValue();

	//Drive Mod
	m_fDistDrive_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_fDistDrive, MODMATDEST::DistortionDrive, &inputState));
	float lDistortionDrive = m_fDistDrive_smoothed.getNextValue();

	//Fuzz Mod
	m_fDistFuzz_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_fDistFuzz, MODMATDEST::DistortionFuzz, &inputState));
	float lDistortionFuzz = m_fDistFuzz_smoothed.getNextValue();

	//Pregain Mod
	m_fDistPreGain_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_fDistPreGain, MODMATDEST::DistortionPreGain, &inputState));
	float lDistortionPreGain = m_fDistPreGain_smoothed.getNextValue();

	//Lowcut Mod
	m_fDistLowcut_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_fDistLowcut, MODMATDEST::DistortionLowCut, &inputState));
	if (m_fDistLowcut_smoothed.isSmoothing()) {
		float lDistortionLowcut = m_fDistLowcut_smoothed.getNextValue();
		float fQ = float(sqrt2over2);
		m_lowCutBiquadL.calcBiquad(CVASTBiQuad::HIGHPASS, lDistortionLowcut, m_iSampleRate, fQ, -18.0f);
		m_lowCutBiquadR.copySettingsFrom(&m_lowCutBiquadL);
	}

	float xnL = 0.0; // = pInputBuffer[0];
	float xnR = 0.0; // = pInputBuffer[1];

	float interpL = 0.0;
	float interpR = 0.0;

	// final outputs for this sample period
	float ynL = 0.0; // = pInputBuffer[0];
	float ynR = 0.0; // = pInputBuffer[1];
	
	xnL = m_biQuadPreL.doBiQuad(pInputBuffer[0] * lDistortionPreGain * 0.01f);
	if(uNumInputChannels == 2)
		xnR = m_biQuadPreL.doBiQuad(pInputBuffer[1] * lDistortionPreGain * 0.01f);
	
	xnL = m_lowCutBiquadL.doBiQuad(xnL); //do pre filter
	xnR = m_lowCutBiquadR.doBiQuad(xnR); //do pre filter
	
	// interp next sample
	interpL = xnL;
	interpR = xnR;

	// overdrive http://www.cs.cf.ac.uk/Dave/CM0268/PDF/10_CM0268_Audio_FX.pdf
	float fDriveL = 0.f;
	float fDriveR = 0.f;

	// PRE GAIN
	fDriveL = interpL * (1 + (lDistortionDrive / 10.0f));
	fDriveL = (fDriveL > 1.0f) ? 1.0f : fDriveL;
	fDriveR = interpR * (1 + (lDistortionDrive / 10.0f));
	fDriveR = (fDriveR > 1.0f) ? 1.0f : fDriveL;

	// fuzz http://www.cs.cf.ac.uk/Dave/CM0268/PDF/10_CM0268_Audio_FX.pdf
	float fAlpha = lDistortionFuzz + 1.f; // should not be 0
	float fFuzzL = 0.f;
	float fFuzzR = 0.f;
	if (fDriveL == 0.0f) fFuzzL = 0.0f;
	else fFuzzL = (fDriveL / abs(fDriveL))*(1.f - powf(float(M_E), fAlpha * powf(fDriveL, 2.0f) / abs(fDriveL))) * 0.06f; //attenuate fuzz
	if (fFuzzL > 1.0f) fFuzzL = 1.0f;
	if (fFuzzL < -1.0f) fFuzzL = -1.0f;
	if (isnan(fFuzzL)) fFuzzL = 0.0f; //NaN check
	if (fDriveR == 0.0f) fFuzzR = 0.0f;
	else fFuzzR = (fDriveR / abs(fDriveR))*(1.f - powf(float(M_E), fAlpha * powf(fDriveR, 2.0f) / abs(fDriveR))) * 0.06f; //attenuate fuzz
	if (fFuzzR > 1.0f) fFuzzR = 1.0f;
	if (fFuzzR < -1.0f) fFuzzR = -1.0f;
	if (isnan(fFuzzR)) fFuzzR = 0.0f; //NaN check

	ynL = fFuzzL;
	ynR = fFuzzR;

	// DO POST EQ
	ynL = m_biQuadPostL.doBiQuad(ynL);
	ynR = m_biQuadPostL.doBiQuad(ynR);

	float fWet = (lDistortionWet / 100.f) * (float(m_iSoftFade) / float(C_MAX_SOFTFADE));
	pOutputBuffer[0] = (1.0f - fWet) * pInputBuffer[0] + (ynL * fWet);   // mix in;
	pOutputBuffer[1] = (1.0f - fWet) * pInputBuffer[1] + (ynR * fWet);   // mix in;

	return true;
}

//==============================================================================

void CVASTDistortion::getStateInformation(MemoryBlock&)
{
	//std::unique_ptr<XmlElement> xml (parameters.valueTreeState.state.createXml());
	//copyXmlToBinary (*xml, destData);
}

void CVASTDistortion::setStateInformation(const void*, int)
{
	//std::unique_ptr<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
	//if (xmlState != nullptr)
	//  if (xmlState->hasTagName (parameters.valueTreeState.state.getType()))
	//    parameters.valueTreeState.state = ValueTree::fromXml (*xmlState);
}
