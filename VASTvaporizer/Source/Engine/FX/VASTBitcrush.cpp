/*
VAST Dynamics Audio Software (TM)

All modulators tested: OK
*/

#include "VASTBitcrush.h"
#include "../Filter/VASTBiQuadFilter.h"
#include "../VASTEngineHeader.h"
#include "../../Plugin/VASTAudioProcessor.h"
#include "VASTEffect.h"
#include <ctime>
#include <cmath>

CVASTBitcrush::CVASTBitcrush(VASTAudioProcessor* processor, int busnr) {
	my_processor = processor;
	myBusnr = busnr;
	setEffectName("BITCRUSH");

	m_bIsOff = true;
	m_bShallBeOff = true;
	m_iSoftFade = 0;

	srand(int(time(0)));
}

CVASTBitcrush::~CVASTBitcrush(void) {
	AudioProcessorValueTreeState& parameters = ((VASTAudioProcessor*)my_processor)->getParameterTree();
	Array<AudioProcessorParameterWithID*> params = getParameters();
	for (int i = 0; i < params.size(); i++) {
		parameters.removeParameterListener(params[i]->paramID, (AudioProcessorValueTreeState::Listener*)this);
	}
}

void CVASTBitcrush::initParameters() {
	AudioProcessorValueTreeState& parameters = my_processor->getParameterTree();

	createAndAddParameter(&m_bBitcrushOnOff, parameters, 1, "m_bBitcrushOnOff", "Bitcrush effect on / off", "On", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 1.0f, 1.0f), 0.0f,
		CVASTParamState::toggleButtonValueToTextFunction,
		CVASTParamState::toggleButtonTextToValueFunction,
		false, true, true, true,
		true);

	createAndAddParameter(&m_fBitcrushDryWet, parameters, 1, "m_fBitcrushDryWet", "Bitcrush dry / wet (no effect - full effect)", "DryWet", 1,
		MODMATDEST::BitcrushDryWet,
		NormalisableRange<float>(0, 100), 60,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);
	createAndAddParameter(&m_fBitcrushLowcut, parameters, 1, "m_fBitcrushLowcut", "Bitcrush lowcut frequency", "Lowcut", 2,
		MODMATDEST::BitcrushLowCut,
		NormalisableRange<float>(5.f, 18000.f, 0.001f, 0.3f, false), 50.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);
	createAndAddParameter(&m_fBitcrushBitdepth, parameters, 1, "m_fBitcrushBitdepth", "Bitcrush bitdepth", "Bitdepth", 3,
		MODMATDEST::BitcrushBitdepth,
		NormalisableRange<float>(2.6f, 32.0f, 0.001f, 0.3f, false), 4.0f, //skew
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);
	createAndAddParameter(&m_fBitcrushBitrate, parameters, 1, "m_fBitcrushBitrate", "Bitcrush bitrate", "Bitrate", 4,
		MODMATDEST::BitcrushBitrate,
		NormalisableRange<float>(0.0f, 100.0f, 1.0f, 3.8f, false), 100.0f, //skew
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, true, false,
		true);
	createAndAddParameter(&m_fBitcrushJitter, parameters, 1, "m_fBitcrushJitter", "Bitcrush jitter", "Jitter", 5,
		MODMATDEST::BitcrushJitter,
		NormalisableRange<float>(0.0f, 100.0f), 0.0f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);
	createAndAddParameter(&m_fBitcrushGain, parameters, 1, "m_fBitcrushGain", "Bitcrush output gain", "Gain", 6,
		MODMATDEST::BitcrushGain,
		NormalisableRange<float>(0, 200), 100,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);
}

void CVASTBitcrush::initCompatibilityParameters() {
	// all new parameters go here
}

void CVASTBitcrush::parameterChanged(const String& parameterID, float newValue) {
	if (parameterID.startsWith("m_bBitcrushOnOff")) {
		if (newValue == static_cast<int>(SWITCH::SWITCH_ON))
			switchOn();
		else
			switchOff();
	}
	else if (parameterID.startsWith("m_fBitcrushDryWet")) {
		m_fBitcrushDryWet_smoothed.setTargetValue(*m_fBitcrushDryWet);
	}
	else if (parameterID.startsWith("m_fBitcrushBitdepth")) {
		m_fBitcrushBitdepth_smoothed.setTargetValue(*m_fBitcrushBitdepth);
	}
	else if (parameterID.startsWith("m_fBitcrushJitter")) {
		m_fBitcrushJitter_smoothed.setTargetValue(*m_fBitcrushJitter);
	}
	else if (parameterID.startsWith("m_fBitcrushLowcut")) {
		m_fBitcrushLowcut_smoothed.setTargetValue(*m_fBitcrushLowcut);
		m_lowCutBiquadL.calcBiquad(CVASTBiQuad::HIGHPASS, m_fBitcrushLowcut_smoothed.getNextValue(), m_iSampleRate, 0.707f, -18.0f);
		m_lowCutBiquadR.copySettingsFrom(&m_lowCutBiquadL);
	}
}

void CVASTBitcrush::init(CVASTSettings &set) {
	m_Set = &set;
	initParameters();

	reset();

	struct timeval tp;
	m_Set->_gettimeofday(&tp);
	m_rand.setSeed(tp.tv_sec); //seed
}

void CVASTBitcrush::reset() {
	m_fDrywetMod = 1.0f;
	m_sampleCount = 0;
	m_repeatLength = 0;

	m_lowCutBiquadL.flushDelays();
	m_lowCutBiquadR.flushDelays();

	if ((m_bIsOff == false) && (m_bShallBeOff == false)) {
		m_iSoftFade = 0;
		m_bShallBeOff = false; //refade in
		m_bIsOff = true;
	}
}

void CVASTBitcrush::prepareToPlay(double, int samplesPerBlock) {
	//m_iSampleRate is set in useroversampling
	m_iExpectedSamplesPerBlock = samplesPerBlock;
	reset();

	const double smoothTime = 0.02;
	m_fBitcrushDryWet_smoothed.reset(m_iSampleRate, smoothTime);
	m_fBitcrushLowcut_smoothed.reset(m_iSampleRate, smoothTime);
	m_fBitcrushBitdepth_smoothed.reset(m_iSampleRate, smoothTime);
	m_fBitcrushJitter_smoothed.reset(m_iSampleRate, smoothTime);
}

void CVASTBitcrush::releaseResources()
{
}

void CVASTBitcrush::processBlock(AudioSampleBuffer& buffer, MidiBuffer& , const int numSamples) {
	if (isOffAndShallBeOff() == true) return;

	modMatrixInputState inputState;

	float* bufferWritePointerL = buffer.getWritePointer(0);
	float* bufferWritePointerR = buffer.getWritePointer(1);

	for (int currentFrame = 0; currentFrame < numSamples; currentFrame += 1) {
		int currentFrameOSAdjusted = currentFrame;
		if (m_bOversampling) currentFrameOSAdjusted /= C_OVERSAMPLING_RATIO;

		checkSoftFade();

		inputState = ((VASTAudioProcessor*)my_processor)->m_pVASTXperience.m_Poly.getOldestNotePlayedInputState(currentFrameOSAdjusted); // make parameter oldest or newest

		m_fBitcrushDryWet_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_fBitcrushDryWet, MODMATDEST::BitcrushDryWet, &inputState));
		float lBitcrushDryWet = m_fBitcrushDryWet_smoothed.getNextValue();

		m_fBitcrushBitdepth_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_fBitcrushBitdepth, MODMATDEST::BitcrushBitdepth, &inputState));
		float lBitcrushBitdepth = m_fBitcrushBitdepth_smoothed.getNextValue();

		m_fBitcrushJitter_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_fBitcrushJitter, MODMATDEST::BitcrushJitter, &inputState));
		float lfBitcrushJitter = m_fBitcrushJitter_smoothed.getNextValue();

		m_fBitcrushLowcut_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_fBitcrushLowcut, MODMATDEST::BitcrushLowCut, &inputState));
		if (m_fBitcrushLowcut_smoothed.isSmoothing()) {
			m_lowCutBiquadL.calcBiquad(CVASTBiQuad::HIGHPASS, m_fBitcrushLowcut_smoothed.getNextValue(), m_iSampleRate, 0.707f, -18.0f);
			m_lowCutBiquadR.copySettingsFrom(&m_lowCutBiquadL);
		}

		float lBitcrushBitrate = m_Set->getParameterValueWithMatrixModulation(m_fBitcrushBitrate, MODMATDEST::BitcrushBitrate, &inputState);
		float lBitcrushGain = m_Set->getParameterValueWithMatrixModulation(m_fBitcrushGain, MODMATDEST::BitcrushGain, &inputState);

		//https://github.com/hosackm/Bitcrusher/blob/master/Bitcrusher/bitcrusher.c


		float fIn[2];
		fIn[0] = bufferWritePointerL[currentFrame];
		fIn[1] = bufferWritePointerR[currentFrame];

		fIn[0] = m_lowCutBiquadL.doBiQuad(fIn[0]); //do pre filter
		fIn[1] = m_lowCutBiquadR.doBiQuad(fIn[1]); //do pre filter

		float fOut[2];
#define ROUND(f) ((float) ((f > 0.0f) ? floor(f+0.5f) : ceil(f-0.5f)))
		float max = ROUND(powf(2.0f, lBitcrushBitdepth) - 1);
		fOut[0] = ROUND((fIn[0] + 1.0f) * max) / max - 1.0f;
		fOut[1] = ROUND((fIn[1] + 1.0f) * max) / max - 1.0f;

		if (m_sampleCount == 0) {
			m_leftFirstSample = fOut[0];
			m_rightFirstSample = fOut[1];
			m_repeatLength = (100.f - lBitcrushBitrate) * (m_iSampleRate / 44100.f);
			if (lfBitcrushJitter > 0)
				m_repeatLength += (m_rand.nextInt64() % int(ceilf((lfBitcrushJitter + 1.f) * (float(m_iSampleRate) / 44100.f))));
		}

		//different approach
		if (m_sampleCount >= m_repeatLength) {
			m_sampleCount = -1;
		}
		m_sampleCount++;

		// this causes us to simulate a down-sample to a lower sample rate
		if (m_sampleCount < m_repeatLength) {
			fOut[0] = m_leftFirstSample;
			fOut[1] = m_rightFirstSample;
		}

		float fWet = (lBitcrushDryWet / 100.f) * (float(m_iSoftFade) / float(C_MAX_SOFTFADE));
		float outL = (1.0f - fWet) * bufferWritePointerL[currentFrame] + (fOut[0] * fWet);   // mix in;
		float outR = (1.0f - fWet) * bufferWritePointerR[currentFrame] + (fOut[1] * fWet);   // mix in;

		vassert((outL > -10.0f) && (outL <= 10.0));
		vassert((outR > -10.0f) && (outR <= 10.0));
		if (outL <= -10.0f) {
			outL = -10.0f;
			reset();
		}
		if (outL >= 10.0f) {
			outL = 10.0f;
			reset();
		}
		if (outR <= -10.0f) {
			outR = -10.0f;
			reset();
		}
		if (outR >= 10.0f) {
			outR = 10.0f;
			reset();
		}

		bufferWritePointerL[currentFrame] = outL * lBitcrushGain * 0.01f;
		bufferWritePointerR[currentFrame] = outR * lBitcrushGain * 0.01f;
	}
}


//==============================================================================

void CVASTBitcrush::getStateInformation(MemoryBlock& )
{
	//std::unique_ptr<XmlElement> xml (parameters.valueTreeState.state.createXml());
	//copyXmlToBinary (*xml, destData);
}

void CVASTBitcrush::setStateInformation(const void* , int )
{
	//std::unique_ptr<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
	//if (xmlState != nullptr)
	//  if (xmlState->hasTagName (parameters.valueTreeState.state.getType()))
	//    parameters.valueTreeState.state = ValueTree::fromXml (*xmlState);
}
