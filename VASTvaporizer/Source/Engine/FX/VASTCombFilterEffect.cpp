/*
VAST Dynamics Audio Software (TM)

All modulators tested: OK
*/

#include "VASTCombFilterEffect.h"
#include "../VASTEngineHeader.h"
#include "../VASTSettings.h"
#include "../../Plugin/VASTAudioProcessor.h"
#include "VASTEffect.h"

CVASTCombFilterEffect::CVASTCombFilterEffect(VASTAudioProcessor* processor, int busnr) {
	my_processor = processor;
	myBusnr = busnr;
	setEffectName("COMB FILTER");

	m_bIsOff = true;
	m_bShallBeOff = true;
	m_iSoftFade = 0;

}

void CVASTCombFilterEffect::initParameters() {
	AudioProcessorValueTreeState& parameters = my_processor->getParameterTree();

	createAndAddParameter(&m_bCombOnOff, parameters, 1, "m_bCombOnOff", "Comb filter on / off (chain effect, not per vocie)", "On", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 1.0f, 1.0f), 0.0f,
		CVASTParamState::toggleButtonValueToTextFunction,
		CVASTParamState::toggleButtonTextToValueFunction,
		false, true, true, true,
		true);
	
	createAndAddParameter(&m_fCombDryWet, parameters, 1, "m_fCombDryWet", "Comb filter mix in (dry - wet)", "DryWet", 1,
		MODMATDEST::CombDryWet,
		NormalisableRange<float>(0, 100), 100.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	createAndAddParameter(&m_fCombFrequOffset, parameters, 1, "m_fCombFrequOffset", "Comb frequency (Hz)", "Frequency", 2,
		MODMATDEST::CombFrequency,
		NormalisableRange<float>(40.f, 18000.f, 0.001f, 0.3f, false), 440.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	createAndAddParameter(&m_fCombDrive, parameters, 1, "m_fCombDrive", "Comb filter drive in dB - pre gain (0 - full, >0 overdrive)", "Drive", 3,
		MODMATDEST::CombDrive,
		NormalisableRange<float>(-20, 20), 0.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	createAndAddParameter(&m_fCombLevel, parameters, 1, "m_fCombLevel", "Comb filter level", "Level", 4,
		MODMATDEST::CombLevel,
		NormalisableRange<float>(0.f, 100.f), 95.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);
	createAndAddParameter(&m_fCombGain, parameters, 1, "m_fCombGain", "Comb filter output gain", "Gain", 5,
		MODMATDEST::CombGain,
		NormalisableRange<float>(0, 200), 100,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);
}

void CVASTCombFilterEffect::initCompatibilityParameters() {
	// all new parameters go here
}

CVASTCombFilterEffect::~CVASTCombFilterEffect(void) {
	AudioProcessorValueTreeState& parameters = ((VASTAudioProcessor*)my_processor)->getParameterTree();
	Array<AudioProcessorParameterWithID*> params = getParameters();
	for (int i = 0; i < params.size(); i++) {
		parameters.removeParameterListener(params[i]->paramID, (AudioProcessorValueTreeState::Listener*)this);
	}
}

void CVASTCombFilterEffect::releaseResources() {
}

void CVASTCombFilterEffect::parameterChanged(const String& parameterID, float newValue) {
	if (parameterID.startsWith("m_bCombOnOff")) {
		if (newValue == static_cast<int>(SWITCH::SWITCH_ON))
			switchOn();
		else
			switchOff();
		return;
	}

	else if (parameterID.startsWith("m_fCombFrequOffset")) {
		m_fCombFrequOffset_smoothed.setTargetValue(newValue);
	}
	else if (parameterID.startsWith("m_fCombLevel")) {
		m_fCombLevel_smoothed.setTargetValue(newValue);
	}
	else if (parameterID.startsWith("m_fCombDrive")) {
		m_fCombDrive_smoothed.setTargetValue(newValue);
	}
	else if (parameterID.startsWith("m_fCombDryWet")) {
		m_fCombDryWet_smoothed.setTargetValue(newValue);
	}
	else if (parameterID.startsWith("m_fCombGain")) {
		m_fCombGain_smoothed.setTargetValue(newValue);
	}
}

void CVASTCombFilterEffect::prepareToPlay(double , int samplesPerBlock) {
	//m_iSampleRate is set in useOversampling
	m_iExpectedSamplesPerBlock = samplesPerBlock;
	mDelayLeft.resetDelay();
	mDelayLeft.setOutputAttenuation_dB(0);
	mDelayLeft.setSampleRate(m_iSampleRate);
	mDelayRight.resetDelay();
	mDelayRight.setOutputAttenuation_dB(0);
	mDelayRight.setSampleRate(m_iSampleRate);

	const double smoothTime = 0.15; //experimental - clicks at 0.1, 0.5 works, 
	m_fCombFrequOffset_smoothed.reset(m_iSampleRate, smoothTime);
	m_fCombDryWet_smoothed.reset(m_iSampleRate, smoothTime);
	m_fCombGain_smoothed.reset(m_iSampleRate, smoothTime);
	m_fCombDrive_smoothed.reset(m_iSampleRate, smoothTime);
	m_fCombLevel_smoothed.reset(m_iSampleRate, smoothTime);

	reset();
}

void CVASTCombFilterEffect::reset() {
	mDelayLeft.resetDelay();
	mDelayRight.resetDelay();

	if ((m_bIsOff == false) && (m_bShallBeOff == false)) {
		m_iSoftFade = 0;
		m_bShallBeOff = false; //refade in
		m_bIsOff = true;
	}
}

void CVASTCombFilterEffect::init(CVASTSettings &set) {
	m_Set = &set;
	initParameters();
	
	mDelayLeft.init(5 * (m_iSampleRate)); //TESTING  5 sec
	mDelayRight.init(5 * (m_iSampleRate)); //TESTING  5 sec

	reset();
}

void CVASTCombFilterEffect::processBlock(AudioSampleBuffer& buffer, MidiBuffer&, const int numSamples) {
	if (isOffAndShallBeOff() == true) return;

	modMatrixInputState inputState;

	float* bufferWritePointerL = buffer.getWritePointer(0);
	float* bufferWritePointerR = buffer.getWritePointer(1);

	for (int currentFrame = 0; currentFrame <numSamples; currentFrame += 1) {
		int currentFrameOSAdjusted = currentFrame;
		if (m_bOversampling) currentFrameOSAdjusted /= C_OVERSAMPLING_RATIO;				
		checkSoftFade();

		inputState = ((VASTAudioProcessor*)my_processor)->m_pVASTXperience.m_Poly.getOldestNotePlayedInputState(currentFrameOSAdjusted); // make parameter oldest or newest

		m_fCombFrequOffset_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_fCombFrequOffset, MODMATDEST::CombFrequency, &inputState));
		float lFreqOffset = m_fCombFrequOffset_smoothed.getNextValue();

		m_fCombDrive_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_fCombDrive, MODMATDEST::CombDrive, &inputState));
		float lCombDrive = m_fCombDrive_smoothed.getNextValue();

		m_fCombGain_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_fCombGain, MODMATDEST::CombGain, &inputState));
		float lCombGain = m_fCombGain_smoothed.getNextValue();

		float lFreq = (lFreqOffset < 50.0f) ? 50.0f : lFreqOffset;
		if (getDelay_ms() != float(1000.f / lFreq)) {
			setDelay_mSec(float(1000.f / lFreq));
		}

		float fIn[2];
		float fOut[2];
		//do  drive
		fIn[0] = bufferWritePointerL[currentFrame] * pow(10.0, lCombDrive / 20.0); //comb not envelope dependend
		fIn[1] = bufferWritePointerR[currentFrame] * pow(10.0, lCombDrive / 20.0); //comb not envelope dependend
		//do filter

		processAudioFrame(fIn, fOut, 2, 2, currentFrame, &inputState);

		bufferWritePointerL[currentFrame] = fOut[0] * lCombGain * 0.01f;
		bufferWritePointerR[currentFrame] = fOut[1] * lCombGain * 0.01f;
	}
}

bool CVASTCombFilterEffect::processAudioFrame(float* pInputBuffer, float* pOutputBuffer, MYUINT uNumInputChannels, MYUINT uNumOutputChannels, int, modMatrixInputState* inputState) {
	// read the delay line to get w(n-D); call base class
	float ynL = mDelayLeft.readDelay();
	float ynR = mDelayRight.readDelay();

	if (mDelayLeft.m_nReadIndex == mDelayLeft.m_nWriteIndex)
		ynL = 0;

	if (mDelayRight.m_nReadIndex == mDelayRight.m_nWriteIndex)
		ynR = 0;

	// form fb = x(n) + m_fComb_gy(n)
	//float fb = *pInput + m_fComb_g*yn;
	
// test https://christianfloisand.wordpress.com/tag/comb-filter/
// test http://www.cs.cf.ac.uk/Dave/CM0268/PDF/10_CM0268_Audio_FX.pdf
	
	m_fCombLevel_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_fCombLevel, MODMATDEST::CombLevel, inputState));
	float lCombLevel = m_fCombLevel_smoothed.getNextValue();

	m_fCombDryWet_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_fCombDryWet, MODMATDEST::CombDryWet, inputState));
	float lCombDryWet = m_fCombDryWet_smoothed.getNextValue();

	float fbL = pInputBuffer[0] + powf(0.001f, (1.0f - (lCombLevel / 101.0f))) * ynL;
	float fbR = pInputBuffer[1] + powf(0.001f, (1.0f - (lCombLevel / 101.0f))) * ynR;

	// write delay line
	mDelayLeft.writeDelayAndInc(fbL);
	mDelayRight.writeDelayAndInc(fbR);

	// write the output sample (could be combined with above line)
	if (mDelayLeft.m_nReadIndex == mDelayLeft.m_nWriteIndex)
		ynL = pInputBuffer[0];
	if (mDelayRight.m_nReadIndex == mDelayLeft.m_nWriteIndex)
		ynR = pInputBuffer[1];
	
	//mix filter in 
	float fMixinComb = (lCombDryWet / 100.f) * (float(m_iSoftFade) / float(C_MAX_SOFTFADE));
	pOutputBuffer[0] = (pInputBuffer[0] * (1.f - fMixinComb) + ynL * fMixinComb); //otherwise to loud
	pOutputBuffer[1] = (pInputBuffer[1] * (1.f - fMixinComb) + ynR * fMixinComb); //otherwise to loud

	vassert((pOutputBuffer[0] > -10.0f) && (pOutputBuffer[0] <= 10.0));
	vassert((pOutputBuffer[1] > -10.0f) && (pOutputBuffer[1] <= 10.0));
	if (((pOutputBuffer[0] < -10.0f) || (pOutputBuffer[0] >= 10.0)) ||
		((pOutputBuffer[1] < -10.0f) || (pOutputBuffer[1] >= 10.0))) {
		reset();
	}

	return true;
}

float CVASTCombFilterEffect::getDelay_ms() {
	return mDelayLeft.m_fDelay_ms;
}

void CVASTCombFilterEffect::setDelay_mSec(float fmSec) {
	mDelayLeft.setDelay_mSec(fmSec);
	mDelayRight.setDelay_mSec(fmSec);
}

//==============================================================================

void CVASTCombFilterEffect::getStateInformation(MemoryBlock&)
{
	//std::unique_ptr<XmlElement> xml (parameters.valueTreeState.state.createXml());
	//copyXmlToBinary (*xml, destData);
}

void CVASTCombFilterEffect::setStateInformation(const void*, int)
{
	//std::unique_ptr<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
	//if (xmlState != nullptr)
	//  if (xmlState->hasTagName (parameters.valueTreeState.state.getType()))
	//    parameters.valueTreeState.state = ValueTree::fromXml (*xmlState);
}
