/*
VAST Dynamics Audio Software (TM)

All modulators tested: OK
*/

#include "VASTCompressorExpander.h"
#include "../VASTEngineHeader.h"
#include "../../Plugin/VASTAudioProcessor.h"
#include "VASTEffect.h"
#include "../VASTSettings.h"

//==============================================================================


CVASTCompressorExpander::CVASTCompressorExpander(VASTAudioProcessor* processor, CVASTSettings& set, int busnr) : m_Set(&set) {
	my_processor = processor;
	myBusnr = busnr;
	setEffectName("LIMITER / GATE");

	m_bIsOff = true;
	m_bShallBeOff = true;
	m_iSoftFade = 0;
}

void CVASTCompressorExpander::initParameters() {
	AudioProcessorValueTreeState& parameters = my_processor->getParameterTree();

	createAndAddParameter(&m_bLimiterOffOn, parameters, 1, "m_bLimiterOffOn", "Limiter on / off", "On", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 1.0f, 1.0f), 0.0f,
		CVASTParamState::toggleButtonValueToTextFunction,
		CVASTParamState::toggleButtonTextToValueFunction,
		false, true, true, true,
		true);

	createAndAddParameter(&m_fLimiterDryWet, parameters, 1, "m_fLimiterDryWet", "Limiter dry / wet (no effect - full effect)", "DryWet", 1,
		MODMATDEST::LimiterDryWet,
		NormalisableRange<float>(0, 100), 100,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);
	createAndAddParameter(&m_uLimiterMode, parameters, 1, "m_uLimiterMode", "Limiter mode", "Mode", 2,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 1.0f, 1.0f), 0.0f,
		[](float value) { return StringArray("Limiter", "Noise gate")[int(value)]; },
		[](String text) { return StringArray("Limiter", "Noise gate").indexOf(StringRef(text), true); },
		false, true, true, false);
	createAndAddParameter(&m_fLimiterThreshold, parameters, 1, "m_fLimiterThreshold", "Limiter threshold (dB)", "Threshold", 3,
		MODMATDEST::LimiterThreshold,
		NormalisableRange<float>(-60.0f, 0.0f), -24.0f, 
		[](float value) {return String(value) + " dB" ; },
		[](String text) {return text.dropLastCharacters(3).getFloatValue(); },
		false, true, false, false,
		true);
	createAndAddParameter(&m_fLimiterRatio, parameters, 1, "m_fLimiterRatio", "Limiter ratio", "Ratio :1", 4,
		MODMATDEST::LimiterRatio,
		NormalisableRange<float>(1.0f, 100.0f), 50.0f, 
		[](float value) {return ":" + String(value); },
		[](String text) {return text.substring(1).getFloatValue(); },
		false, true, false, false, 
		true);
	createAndAddParameter(&m_fLimiterAttack, parameters, 1, "m_fLimiterAttack", "Limiter attack (ms)", "Attack", 5,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.1f, 100.0f), 2.0f, 
		[](float value) {return String(value) + " ms"; },
		[](String text) {return text.dropLastCharacters(3).getFloatValue(); }, 
		false, true, false, false);
	createAndAddParameter(&m_fLimiterRelease, parameters, 1, "m_fLimiterRelease", "Limiter release (ms)", "Release", 6,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(10.0f, 1000.0f), 300.0f, 
		[](float value) {return String(value) + " ms"; },
		[](String text) {return text.dropLastCharacters(3).getFloatValue(); },
		false, true, false, false);
	createAndAddParameter(&m_fLimiterMakeupGain, parameters, 1, "m_fLimiterMakeupGain", "Limiter makeup gain (dB)", "Gain", 7,
		MODMATDEST::LimiterGain,
		NormalisableRange<float>(-12.0f, 12.0f), 0.0f, 
		[](float value) {return String(value) + " dB"; },
		[](String text) {return text.dropLastCharacters(3).getFloatValue(); },
		false, true, false, false,
		true);
}

void CVASTCompressorExpander::initCompatibilityParameters() {
	// all new parameters go here
}

CVASTCompressorExpander::~CVASTCompressorExpander() {
	AudioProcessorValueTreeState& parameters = ((VASTAudioProcessor*)my_processor)->getParameterTree();
	Array<AudioProcessorParameterWithID*> params = getParameters();
	for (int i = 0; i < params.size(); i++) {
		parameters.removeParameterListener(params[i]->paramID, (AudioProcessorValueTreeState::Listener*)this);
	}
}

void CVASTCompressorExpander::parameterChanged(const String& parameterID, float newValue) {
	if (parameterID.startsWith("m_bLimiterOffOn")) {
		if (newValue == static_cast<int>(SWITCH::SWITCH_ON))
			switchOn();
		else
			switchOff();
	}

	else if (parameterID.startsWith("m_fLimiterDryWet")) {
		m_fLimiterDryWet_smoothed.setTargetValue(newValue);
	}
	else if (parameterID.startsWith("m_fLimiterThreshold")) {
		m_fLimiterThreshold_smoothed.setTargetValue(newValue);
	}
	else if (parameterID.startsWith("m_fLimiterMakeupGain")) {
		m_fLimiterGain_smoothed.setTargetValue(newValue);
	}
	else if (parameterID.startsWith("m_fLimiterRatio")) {
		m_fLimiterRatio_smoothed.setTargetValue(newValue);
	}
}

void CVASTCompressorExpander::init() {
	initParameters();
	reset();
}

void CVASTCompressorExpander::reset() {
	if ((m_bIsOff == false) && (m_bShallBeOff == false)) {
		m_iSoftFade = 0;
		m_bShallBeOff = false; //refade in
		m_bIsOff = true;
	}
}

//==============================================================================

void CVASTCompressorExpander::prepareToPlay (double , int samplesPerBlock) {
	//m_iSampleRate is set in useoversampling
	m_iExpectedSamplesPerBlock = samplesPerBlock;

	//Smoothing gain changes?
	/*
	const double smoothTime = 1e-3;
    paramThreshold.reset (sampleRate, smoothTime);
    paramRatio.reset (sampleRate, smoothTime);
    paramAttack.reset (sampleRate, smoothTime);
    paramRelease.reset (sampleRate, smoothTime);
    paramMakeupGain.reset (sampleRate, smoothTime);
    paramBypass.reset (sampleRate, smoothTime);
	*/

	const double smoothTime = 0.02; 
	m_fLimiterDryWet_smoothed.reset(m_iSampleRate, smoothTime);
	m_fLimiterThreshold_smoothed.reset(m_iSampleRate, smoothTime);
	m_fLimiterGain_smoothed.reset(m_iSampleRate, smoothTime);
	m_fLimiterRatio_smoothed.reset(m_iSampleRate, smoothTime);

    //======================================

    //mixedDownInput.setSize (2, samplesPerBlock);

	inputLevel[0] = 0.0f;
	ylPrev[0] = 0.0f;
	inputLevel[1] = 0.0f;
	ylPrev[1] = 0.0f;

    inverseSampleRate = 1.0f / (float)m_iSampleRate;
    inverseE = 1.0f / float(M_E);
}

void CVASTCompressorExpander::releaseResources()
{
}

void CVASTCompressorExpander::processBlock (AudioSampleBuffer& buffer, MidiBuffer& , const int numSamples) {
	if (isOffAndShallBeOff() == true) return;

 	ScopedNoDenormals noDenormals;

    //======================================

	int expander = (int)*m_uLimiterMode;
	float alphaA = calculateAttackOrRelease(*m_fLimiterAttack * 0.001f);
	float alphaR = calculateAttackOrRelease(*m_fLimiterRelease * 0.001f);

	modMatrixInputState inputState = ((VASTAudioProcessor*)my_processor)->m_pVASTXperience.m_Poly.getOldestNotePlayedInputState(0); // make parameter oldest or newest

	float* bufferWritePointerL = buffer.getWritePointer(0);
	float* bufferWritePointerR = buffer.getWritePointer(1);

	for (int sample = 0; sample < numSamples; ++sample) {
		checkSoftFade();

		inputState.currentFrame = sample;
		m_fLimiterDryWet_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_fLimiterDryWet, MODMATDEST::LimiterDryWet, &inputState));
		float lLimiterDryWet = m_fLimiterDryWet_smoothed.getNextValue();

		m_fLimiterThreshold_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_fLimiterThreshold, MODMATDEST::LimiterThreshold, &inputState));
		float T = m_fLimiterThreshold_smoothed.getNextValue();

		m_fLimiterRatio_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_fLimiterRatio, MODMATDEST::LimiterRatio, &inputState));
		float R = m_fLimiterRatio_smoothed.getNextValue();

		m_fLimiterGain_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_fLimiterMakeupGain, MODMATDEST::LimiterGain, &inputState));
		float lLimiterGain = m_fLimiterGain_smoothed.getNextValue();
		
		float inputSquared_left = powf(bufferWritePointerL[sample], 2.0f);
		float inputSquared_right = powf(bufferWritePointerR[sample], 2.0f);
		if (expander == 1) {
			const float averageFactor = 0.9999f;
			inputLevel[0] = averageFactor * inputLevel[0] + (1.0f - averageFactor) * inputSquared_left;
			inputLevel[1] = averageFactor * inputLevel[1] + (1.0f - averageFactor) * inputSquared_right;
		}
		else {
			inputLevel[0] = inputSquared_left;
			inputLevel[1] = inputSquared_right;
		}
		xg[0] = (inputLevel[0] <= 1e-6f) ? -60.0f : 10.0f * log10f(inputLevel[0]);
		xg[1] = (inputLevel[1] <= 1e-6f) ? -60.0f : 10.0f * log10f(inputLevel[1]);

		// Expander
		if (expander == 1) {
			if (xg[0] > T)
				yg[0] = xg[0];
			else
				yg[0] = T + (xg[0] - T) * R;

			if (xg[1] > T)
				yg[1] = xg[1];
			else
				yg[1] = T + (xg[1] - T) * R;

			xl[0] = xg[0] - yg[0];
			xl[1] = xg[1] - yg[1];

			if (xl[0] < ylPrev[0])
				yl[0] = alphaA * ylPrev[0] + (1.0f - alphaA) * xl[0];
			else
				yl[0] = alphaR * ylPrev[0] + (1.0f - alphaR) * xl[0];

			if (xl[1] < ylPrev[1])
				yl[1] = alphaA * ylPrev[1] + (1.0f - alphaA) * xl[1];
			else
				yl[1] = alphaR * ylPrev[1] + (1.0f - alphaR) * xl[1];
			// Compressor
		}
		else {
			if (xg[0] < T)
				yg[0] = xg[0];
			else
				yg[0] = T + (xg[0] - T) / R;

			if (xg[1] < T)
				yg[1] = xg[1];
			else
				yg[1] = T + (xg[1] - T) / R;

			xl[0] = xg[0] - yg[0];
			xl[1] = xg[1] - yg[1];

			if (xl[0] > ylPrev[0])
				yl[0] = alphaA * ylPrev[0] + (1.0f - alphaA) * xl[0];
			else
				yl[0] = alphaR * ylPrev[0] + (1.0f - alphaR) * xl[0];

			if (xl[1] > ylPrev[1])
				yl[1] = alphaA * ylPrev[1] + (1.0f - alphaA) * xl[1];
			else
				yl[1] = alphaR * ylPrev[1] + (1.0f - alphaR) * xl[1];
		}

		control[0] = powf(10.0f, (lLimiterGain - yl[0]) * 0.05f);
		control[1] = powf(10.0f, (lLimiterGain - yl[1]) * 0.05f);
		ylPrev[0] = yl[0];
		ylPrev[1] = yl[1];

		float lMix = lLimiterDryWet / 100.f * (float(m_iSoftFade) / float(C_MAX_SOFTFADE));
		float newValueleft = bufferWritePointerL[sample] * control[0];
		float newValueright = bufferWritePointerR[sample] * control[1];

		newValueleft = (1.0f - lMix) * bufferWritePointerL[sample] + (newValueleft * lMix);
		newValueright = (1.0f - lMix) * bufferWritePointerR[sample] + (newValueright * lMix);

		bufferWritePointerL[sample] = newValueleft;
		bufferWritePointerR[sample] = newValueright;
	}
    //======================================
}

//==============================================================================

float CVASTCompressorExpander::calculateAttackOrRelease (float value)
{
    if (value == 0.0f)
        return 0.0f;
    else
        return pow (inverseE, inverseSampleRate / value);
}

//==============================================================================

//==============================================================================

void CVASTCompressorExpander::getStateInformation (MemoryBlock& )
{
    //std::unique_ptr<XmlElement> xml (parameters.valueTreeState.state.createXml());
    //copyXmlToBinary (*xml, destData);
}

void CVASTCompressorExpander::setStateInformation (const void* , int )
{
    //std::unique_ptr<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    //if (xmlState != nullptr)
      //  if (xmlState->hasTagName (parameters.valueTreeState.state.getType()))
        //    parameters.valueTreeState.state = ValueTree::fromXml (*xmlState);
}
