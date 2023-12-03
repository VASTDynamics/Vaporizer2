/*
VAST Dynamics Audio Software (TM)

This code accompanies the textbook:

Digital Audio Effects: Theory, Implementation and Application
Joshua D. Reiss and Andrew P. McPherson
*/

#include "VASTMultibandCompressorSingleBand.h"
#include "../VASTEngineHeader.h"
#include "../../Plugin/VASTAudioProcessor.h"
#include "VASTEffect.h"
#include "../VASTSettings.h"

//==============================================================================


CVASTMultibandCompressorSingleBand::CVASTMultibandCompressorSingleBand(VASTAudioProcessor* processor, CVASTSettings& set, int busnr, int band) : m_Set(&set) {
	my_processor = processor;
	myBusnr = busnr;
	myBand = band;

	m_bIsOff = true;
	m_bShallBeOff = true;
	m_iSoftFade = 0;

}

void CVASTMultibandCompressorSingleBand::initParameters() {
	AudioProcessorValueTreeState& parameters = my_processor->getParameterTree();
	float initLowFreq = 0.f;
	float initHighFreq = 0.f;
	float initThreshold = 0.0f;
	float initRatio = 0.0f;
	float initGain = 0.f;
	String suffix = "";
	if (myBand == 0) {
		setEffectName("COMPRESSOR LOW");
		suffix = "_Low";
		initLowFreq = 10.f;   //check low freqs
		initHighFreq = 1500.0f;
		initThreshold = -40.0f;
		initRatio = 40.f;
		initGain = 12.f;
	}
	else if (myBand == 1) {
		suffix = "_Mid";
		setEffectName("COMPRESSOR MID");
		initLowFreq = 1500.0f;
		initHighFreq = 6700.0f;
		initThreshold = -10.0f;
		initRatio = 25.f;
		initGain = 10.f;
	}
	else if (myBand == 2) {
		suffix = "_High";
		setEffectName("COMPRESSOR HIGH");
		initLowFreq = 6700.0f;
		initHighFreq = 20000.f;
		initThreshold = -40.0f;
		initRatio = 55.f;
		initGain = 22.f;
	}

	createAndAddParameter(&m_bMBCompOffOn, parameters, 1, "m_bMBCompOffOn" + suffix, "Single-band compressor on / off", "On", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 1.0f, 1.0f), 0.0f,
		CVASTParamState::toggleButtonValueToTextFunction,
		CVASTParamState::toggleButtonTextToValueFunction,
		false, true, true, true,
		true);

	createAndAddParameter(&m_fMBCompFcLow, parameters, 1, "m_fMBCompFcLow" + suffix, "Single-band compressor low band cut frequency (hz)", "Lo(Hz)", 1,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(10.0f, 20000.f), initLowFreq,
		[](float value) {return String(value) + " Hz"; },
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);
	createAndAddParameter(&m_fMBCompFcHigh, parameters, 1, "m_fMBCompFcHigh" + suffix, "Single-band compressor high band cut frequency (hz)", "Hi(Hz)", 2,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(10.0f, 20000.f), initHighFreq,
		[](float value) {return String(value) + " Hz"; },
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);
	createAndAddParameter(&m_fMBCompThreshold, parameters, 1, "m_fMBCompThreshold" + suffix, "Single-band compressor threshold low band (dB)", "Threshold", 3,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(-60.0f, -0.0001f), initThreshold,
		[](float value) {return String(value) + " dB" ; },
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);
	createAndAddParameter(&m_fMBCompRatio, parameters, 1, "m_fMBCompRatio" + suffix, "Single-band compressor ratio low band (:1)", "Ratio", 4,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(1.f, 100, 0.000001f, 0.3f, false), initRatio,
		[](float value) {return String(value) + " :1"; },
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);
	createAndAddParameter(&m_fMBCompAttack, parameters, 1, "m_fMBCompAttack" + suffix, "Single-band compressor attack low band (ms)", "Attack", 5,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.1f, 80.0), 4.0f,
		[](float value) {return String(value) + " ms"; },
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);
	createAndAddParameter(&m_fMBCompRelease, parameters, 1, "m_fMBCompReleaseLB" + suffix, "Single-band compressor release low band (ms)", "Release", 6,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.1f, 1000.0), 4.0f,
		[](float value) {return String(value) + " ms"; },
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);
	createAndAddParameter(&m_fMBCompGain, parameters, 1, "m_fMBCompGain" + suffix, "Single-band compressor gain low band (dB)", "Gain", 7,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 40.f), initGain,
		[](float value) {return String(value) + " dB"; },
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);
}

void CVASTMultibandCompressorSingleBand::initCompatibilityParameters() {
	// all new parameters go here
}

CVASTMultibandCompressorSingleBand::~CVASTMultibandCompressorSingleBand() {
	AudioProcessorValueTreeState& parameters = ((VASTAudioProcessor*)my_processor)->getParameterTree();
	Array<AudioProcessorParameterWithID*> params = getParameters();
	for (int i = 0; i < params.size(); i++) {
		parameters.removeParameterListener(params[i]->paramID, (AudioProcessorValueTreeState::Listener*)this);
	}
}

void CVASTMultibandCompressorSingleBand::parameterChanged(const String& parameterID, float newValue) {
	if (parameterID.startsWith("m_bMBCompOffOn")) {
		if (newValue == static_cast<int>(SWITCH::SWITCH_ON))
			switchOn();
		else
			switchOff();
	}
	else if (parameterID.startsWith("m_fMBCompFcLow")) {
		adjustLowband();
	}
	else if (parameterID.startsWith("m_fMBCompFcHigh")) {
		adjustHighband();
	}
}

void CVASTMultibandCompressorSingleBand::init() {
	initParameters();
}

void CVASTMultibandCompressorSingleBand::reset() { 
	m_yL_prev = 0.f;

	for (int i = 0; i < m_iExpectedSamplesPerBlock; ++i)
	{
		m_x_g[i] = 0.f; m_y_g[i] = 0.f;
		m_x_l[i] = 0.f; m_y_l[i] = 0.f;
		m_c[i] = 0.f;
	}

	if ((m_bIsOff == false) && (m_bShallBeOff == false)) {
		m_iSoftFade = 0;
		m_bShallBeOff = false; //refade in
		m_bIsOff = true;
	}
}

//==============================================================================

void CVASTMultibandCompressorSingleBand::prepareToPlay (double sampleRate, int samplesPerBlock) {
	m_iExpectedSamplesPerBlock = samplesPerBlock;
	//m_iSampleRate is set in userOversampling
	m_iNumChannels = 2;

	// Allocate a lot of dynamic memory here for low band
	m_x_g.allocate(samplesPerBlock, true);
	m_x_l.allocate(samplesPerBlock, true);
	m_y_g.allocate(samplesPerBlock, true);
	m_y_l.allocate(samplesPerBlock, true);
	m_c.allocate(samplesPerBlock, true);

	//autoTime = false;
	reset();

	//===================================================//
	//          Calculate Coefficients                   //
	//===================================================//

	adjustLowband();
	adjustHighband();
}

void CVASTMultibandCompressorSingleBand::releaseResources() {
}

void CVASTMultibandCompressorSingleBand::adjustLowband() {
	float fcLow = *m_fMBCompFcLow;
	lowBandPassL1.setCoefficients(coeff.makeHighPass(m_iSampleRate, fcLow));
	lowBandPassL2.setCoefficients(coeff.makeHighPass(m_iSampleRate, fcLow));
	lowBandPassR1.setCoefficients(coeff.makeHighPass(m_iSampleRate, fcLow));
	lowBandPassR2.setCoefficients(coeff.makeHighPass(m_iSampleRate, fcLow));
}

void CVASTMultibandCompressorSingleBand::adjustHighband() {
	float fcHigh = *m_fMBCompFcHigh;
	highBandPassL1.setCoefficients(coeff.makeLowPass(m_iSampleRate, fcHigh));
	highBandPassL2.setCoefficients(coeff.makeLowPass(m_iSampleRate, fcHigh));
	highBandPassR1.setCoefficients(coeff.makeLowPass(m_iSampleRate, fcHigh));
	highBandPassR2.setCoefficients(coeff.makeLowPass(m_iSampleRate, fcHigh));
}

void CVASTMultibandCompressorSingleBand::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages, const int numSamples) {
	if (isOffAndShallBeOff() == true) return;

 	ScopedNoDenormals noDenormals;

	//===================================================//
	//   Create and Empty Buffers                        //
	//===================================================//

	AudioSampleBuffer bpFilterBuffer;
	AudioSampleBuffer bpCompFilterBuffer;

	bpFilterBuffer.setSize(2, numSamples);
	bpFilterBuffer.clear();

	bpCompFilterBuffer.setSize(2, numSamples);
	bpCompFilterBuffer.clear();

	bpFilterBuffer.copyFrom(0, 0, buffer.getReadPointer(0, 0), numSamples);
	bpFilterBuffer.copyFrom(1, 0, buffer.getReadPointer(1, 0), numSamples);

	//===================================================//
	//   Apply Filters to Buffers                        //
	//===================================================//


	lowBandPassL1.processSamples(bpFilterBuffer.getWritePointer(0), numSamples);
	lowBandPassR1.processSamples(bpFilterBuffer.getWritePointer(1), numSamples);
	lowBandPassL2.processSamples(bpFilterBuffer.getWritePointer(0), numSamples);
	lowBandPassR2.processSamples(bpFilterBuffer.getWritePointer(1), numSamples);

	highBandPassL1.processSamples(bpFilterBuffer.getWritePointer(0), numSamples);
	highBandPassR1.processSamples(bpFilterBuffer.getWritePointer(1), numSamples);
	highBandPassL2.processSamples(bpFilterBuffer.getWritePointer(0), numSamples);
	highBandPassR2.processSamples(bpFilterBuffer.getWritePointer(1), numSamples);

	//===================================================//
	//   Apply Compressors to Buffers                    //
	//===================================================//

	if (*m_bMBCompOffOn == static_cast<int>(SWITCH::SWITCH_ON))
	{
		int M = round(m_iNumChannels / 2);
		for (int m = 0; m < M; ++m) 
		{
			bpCompFilterBuffer.addFrom(m, 0, bpFilterBuffer, m * 2, 0, numSamples, 0.5);
			bpCompFilterBuffer.addFrom(m, 0, bpFilterBuffer, m * 2 + 1, 0, numSamples, 0.5);
			//compression : calculates the control voltage
			compressorBand(buffer, m, numSamples); //ADD BAND NUMBER INSTEAD OF 1
			for (int i = 0; i < numSamples; ++i)
			{
				bpFilterBuffer.getWritePointer(2 * m + 0)[i] *= m_c[i];
				bpFilterBuffer.getWritePointer(2 * m + 1)[i] *= m_c[i];
			}
			bpCompFilterBuffer.clear(m, 0, numSamples);

		}
		for (int channel = 0; channel < m_iNumChannels; channel++) {
			buffer.copyFrom(channel, 0, bpFilterBuffer, channel, 0, numSamples);
		}
	}

    //======================================
}

//==============================================================================

// Low compressor function
void CVASTMultibandCompressorSingleBand::compressorBand(AudioSampleBuffer &buffer, int m, const int numSamples) {
	float threshold = *m_fMBCompThreshold;
	float tauAttack = *m_fMBCompAttack;
	float tauRelease = *m_fMBCompRelease;
	float ratio = *m_fMBCompRatio;
	float makeUpGain = *m_fMBCompGain;

	float alphaAttack = exp(-1 / (0.001 * m_iSampleRate * tauAttack));
	float alphaRelease = exp(-1 / (0.001 * m_iSampleRate * tauRelease));
	for (int i = 0; i < numSamples; ++i)
	{
		//Level detection- estimate level using peak detector
		if (fabs(buffer.getWritePointer(m)[i]) < 0.000001) m_x_g[i] = -120;
		else m_x_g[i] = 20 * log10(fabs(buffer.getWritePointer(m)[i]));
		//Gain computer- static apply input/output curve
		if (m_x_g[i] >= threshold) m_y_g[i] = threshold + (m_x_g[i] - threshold) / ratio;
		else m_y_g[i] = m_x_g[i];
		m_x_l[i] = m_x_g[i] - m_y_g[i];
		//Ballistics- smoothing of the gain 
		if (m_x_l[i]>m_yL_prev)  m_y_l[i] = alphaAttack * m_yL_prev + (1 - alphaAttack) * m_x_l[i];
		else m_y_l[i] = alphaRelease * m_yL_prev + (1 - alphaRelease) * m_x_l[i];
		//find control
		m_c[i] = pow(10, (makeUpGain - m_y_l[i]) / 20);
		m_yL_prev = m_y_l[i];
	}
}

//==============================================================================

void CVASTMultibandCompressorSingleBand::getStateInformation (MemoryBlock& )
{
    //std::unique_ptr<XmlElement> xml (parameters.valueTreeState.state.createXml());
    //copyXmlToBinary (*xml, destData);
}

void CVASTMultibandCompressorSingleBand::setStateInformation (const void* , int )
{
    //std::unique_ptr<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    //if (xmlState != nullptr)
      //  if (xmlState->hasTagName (parameters.valueTreeState.state.getType()))
        //    parameters.valueTreeState.state = ValueTree::fromXml (*xmlState);
}
