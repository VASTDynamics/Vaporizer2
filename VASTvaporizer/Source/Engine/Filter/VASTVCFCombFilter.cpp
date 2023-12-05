/*
VAST Dynamics Audio Software (TM)

All modulators tested: OK
*/

#include "VASTVCFCombFilter.h"
#include "../VASTEngineHeader.h"
#include "../VASTSettings.h"

JUCE_BEGIN_IGNORE_WARNINGS_GCC_LIKE("-Wconversion")
JUCE_BEGIN_IGNORE_WARNINGS_MSVC(4244 4267) //C4244 conversion from 'type1' to 'type2', possible loss of data //C4267 conversion

CVASTVCFCombFilter::CVASTVCFCombFilter(CVASTSettings& set) : m_Set(&set) {
}

void CVASTVCFCombFilter::parameterChanged(const String& parameterID, float newValue, bool takeNext, int skips) {
	if (parameterID.startsWith("m_fCombFrequOffset")) {
        takeNext ? m_fCombFrequOffset_smoothed.setCurrentAndTargetValue(newValue) : m_fCombFrequOffset_smoothed.setTargetValue(newValue);
		m_fCombFrequOffset_smoothed.skip(skips);
	}
	else if (parameterID.startsWith("m_fCombLevel")) {
        takeNext ? m_fCombLevel_smoothed.setCurrentAndTargetValue(newValue) : m_fCombLevel_smoothed.setTargetValue(newValue);
		m_fCombLevel_smoothed.skip(skips);
	}
	else if (parameterID.startsWith("m_fCombDrive")) {
        takeNext ? m_fCombDrive_smoothed.setCurrentAndTargetValue(newValue) : m_fCombDrive_smoothed.setTargetValue(newValue);
		m_fCombDrive_smoothed.skip(skips);
	}
	else if (parameterID.startsWith("m_fCombDryWet")) {
        takeNext ? m_fCombDryWet_smoothed.setCurrentAndTargetValue(newValue) : m_fCombDryWet_smoothed.setTargetValue(newValue);
		m_fCombDryWet_smoothed.skip(skips);
	}
	else if (parameterID.startsWith("m_fCombGain")) {
        takeNext ? m_fCombGain_smoothed.setCurrentAndTargetValue(newValue) : m_fCombGain_smoothed.setTargetValue(newValue);
		m_fCombGain_smoothed.skip(skips);
	}
}

void CVASTVCFCombFilter::prepareToPlay(double sampleRate, int samplesPerBlock) {
	m_iSampleRate = sampleRate;
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

void CVASTVCFCombFilter::reset() {
	mDelayLeft.resetDelay();
	mDelayRight.resetDelay();
}

void CVASTVCFCombFilter::init() {
	m_iSampleRate = m_Set->m_nSampleRate;
	mDelayLeft.init(1 * (m_iSampleRate)); //TESTING  1 sec
	mDelayRight.init(1 * (m_iSampleRate)); //TESTING  1 sec

	reset();
}

void CVASTVCFCombFilter::processBlock(dsp::AudioBlock<float> filterBlock, const int numSamples) {

	for (int currentFrame = 0; currentFrame <numSamples; currentFrame += 1) {
		//int currentFrameOSAdjusted = currentFrame;
		//if (m_bOversampling) currentFrameOSAdjusted /= C_OVERSAMPLING_RATIO;				
		//checkSoftFade();
		float lFreqOffset = m_fCombFrequOffset_smoothed.getNextValue();

		float lCombDrive = m_fCombDrive_smoothed.getNextValue();

		float lCombGain = m_fCombGain_smoothed.getNextValue();

		float lFreq = (lFreqOffset < 50.0f) ? 50.0f : lFreqOffset;
		if (getDelay_ms() != float(1000.f / lFreq)) {
			setDelay_mSec(float(1000.f / lFreq));
		}

		float fIn[2];
		float fOut[2];
		//do  drive
		
		fIn[0] = filterBlock.getChannelPointer(0)[currentFrame] * pow(10.0, lCombDrive / 20.0); //comb not envelope dependend
		fIn[1] = filterBlock.getChannelPointer(1)[currentFrame] * pow(10.0, lCombDrive / 20.0); //comb not envelope dependend
		//do filter

		processAudioFrame(fIn, fOut, 2, 2, currentFrame);

		filterBlock.getChannelPointer(0)[currentFrame] = fOut[0] * lCombGain * 0.01f;
		filterBlock.getChannelPointer(1)[currentFrame] = fOut[1] * lCombGain * 0.01f;
	}
}

bool CVASTVCFCombFilter::processAudioFrame(float* pInputBuffer, float* pOutputBuffer, MYUINT uNumInputChannels, MYUINT uNumOutputChannels, int currentFrameOSAdjusted) {
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
	
	float lCombLevel = m_fCombLevel_smoothed.getNextValue();

	float lCombDryWet = m_fCombDryWet_smoothed.getNextValue();

	float fbL = pInputBuffer[0] + powf(0.001f, (1.0f - (lCombLevel / 101.0f))) * ynL;
	float fbR = pInputBuffer[1] + powf(0.001f, (1.0f - (lCombLevel / 101.0f))) * ynR;

	// write delay line
	mDelayLeft.writeDelayAndInc(fbL);
	mDelayRight.writeDelayAndInc(fbR);

	// write the output sample (could be combined with above line)
	if (mDelayLeft.m_nReadIndex == mDelayLeft.m_nWriteIndex)
		ynL = pInputBuffer[0];
	if (mDelayRight.m_nReadIndex == mDelayRight.m_nWriteIndex)
		ynR = pInputBuffer[1];
	
	//mix filter in 
	//float fMixinComb = (lCombDryWet / 100.f) * (float(m_iSoftFade) / float(C_MAX_SOFTFADE));
	float fMixinComb = (lCombDryWet / 100.f);
	pOutputBuffer[0] = (pInputBuffer[0] * (1 - fMixinComb) + ynL * fMixinComb); //otherwise to loud
	pOutputBuffer[1] = (pInputBuffer[1] * (1 - fMixinComb) + ynR * fMixinComb); //otherwise to loud

	vassert((pOutputBuffer[0] > -10.0f) && (pOutputBuffer[0] <= 10.0));
	vassert((pOutputBuffer[1] > -10.0f) && (pOutputBuffer[1] <= 10.0));
	if (((pOutputBuffer[0] < -10.0f) || (pOutputBuffer[0] >= 10.0)) ||
		((pOutputBuffer[1] < -10.0f) || (pOutputBuffer[1] >= 10.0))) {
		reset();
	}

	return true;
}

float CVASTVCFCombFilter::getDelay_ms() {
	return mDelayLeft.m_fDelay_ms;
}

void CVASTVCFCombFilter::setDelay_mSec(float fmSec) {
	mDelayLeft.setDelay_mSec(fmSec);
	mDelayRight.setDelay_mSec(fmSec);
}

//==============================================================================

void CVASTVCFCombFilter::getStateInformation(MemoryBlock& destData)
{
	//std::unique_ptr<XmlElement> xml (parameters.valueTreeState.state.createXml());
	//copyXmlToBinary (*xml, destData);
}

void CVASTVCFCombFilter::setStateInformation(const void* data, int sizeInBytes)
{
	//std::unique_ptr<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
	//if (xmlState != nullptr)
	//  if (xmlState->hasTagName (parameters.valueTreeState.state.getType()))
	//    parameters.valueTreeState.state = ValueTree::fromXml (*xmlState);
}

JUCE_END_IGNORE_WARNINGS_GCC_LIKE
JUCE_END_IGNORE_WARNINGS_MSVC
