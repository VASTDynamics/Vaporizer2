/*
VAST Dynamics Audio Software (TM)

All modulators tested: OK
*/

#include "VASTAtomizer.h"
#include "../VASTEngineHeader.h"
#include "../VASTSettings.h"
#include "../../Plugin/VASTAudioProcessor.h"
#include "VASTEffect.h"
#include "FXUtils/STFT.h"
#include <ctime>
#include <chrono>

CVASTAtomizer::CVASTAtomizer(VASTAudioProcessor* processor, CVASTSettings& set, int busnr) : m_Set(&set), m_LFO(set, nullptr) {
	my_processor = processor;
	myBusnr = busnr;
	setEffectName("ATOMIZER");

	m_bIsOff = true;
	m_bShallBeOff = true;
	m_iSoftFade = 0;
}

CVASTAtomizer::~CVASTAtomizer(void) {
	AudioProcessorValueTreeState& parameters = ((VASTAudioProcessor*)my_processor)->getParameterTree();	
	Array<AudioProcessorParameterWithID*> params = getParameters();
	for (int i = 0; i < params.size(); i++) {
		parameters.removeParameterListener(params[i]->paramID, (AudioProcessorValueTreeState::Listener*)this);
	}
}

void CVASTAtomizer::initParameters() {
	AudioProcessorValueTreeState& parameters = my_processor->getParameterTree();

	createAndAddParameter(&m_bAtomizerOnOff, parameters, 1, "m_bAtomizerOnOff", "Atomizer effect on / off", "On", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 1.0f, 1.0f), 0.0f,
		CVASTParamState::toggleButtonValueToTextFunction,
		CVASTParamState::toggleButtonTextToValueFunction,
		false, true, true, true,
		true);

	createAndAddParameter(&m_fAtomizerDryWet, parameters, 1, "m_fAtomizerDryWet", "Atomizer filter mix in (dry - wet)", "DryWet", 1,
		MODMATDEST::AtomizerDryWet,
		NormalisableRange<float>(0, 100), 100,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	createAndAddParameter(&m_fAtomizerLowcut, parameters, 1, "m_fAtomizerLowcut", "Atomizer lowcut", "Lowcut", 2,
		MODMATDEST::AtomizerLowCut,
		NormalisableRange<float>(0, 100), 0.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	createAndAddParameter(&m_fAtomizerHighcut, parameters, 1, "m_fAtomizerHighcut", "Atomizer highcut", "Hicut", 3,
		MODMATDEST::AtomizerHighCut,
		NormalisableRange<float>(0, 100), 100.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	createAndAddParameter(&m_fAtomizerHarmonics, parameters, 1, "m_fAtomizerHarmonics", "Atomizer harmonics", "Harmonics", 4,
		MODMATDEST::AtomizerHarmonics,
		NormalisableRange<float>(0, 100), 0.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	createAndAddParameter(&m_fAtomizerEmphasis, parameters, 1, "m_fAtomizerEmphasis", "Atomizer emphasis", "Emphasis", 5,
		MODMATDEST::AtomizerEmphasis,
		NormalisableRange<float>(0, 100), 0.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	createAndAddParameter(&m_fAtomizerDephase, parameters, 1, "m_fAtomizerDephase", "Atomizer dephase", "Dephase", 6,
		MODMATDEST::AtomizerDephase,
		NormalisableRange<float>(0, 100), 0.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	createAndAddParameter(&m_uAtomizerLFOWave, parameters, 1, "m_uAtomizerLFOWave", "Atomizer LFO waveform", "LFOwave", 7,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, LFOWAVE_Array.size() - 1, 1.0f), 0.0f,
		CVASTParamState::comboBoxValueToTextFunction_LFOWAVE,
		CVASTParamState::comboBoxTextToValueFunction_LFOWAVE,
		false, true, false, false,
		true);

	createAndAddParameter(&m_bAtomizerSynch, parameters, 1, "m_bAtomizerSynch", "Atomizer sync to DAW", "Sync", 8,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 1.0f, 1.0f), 0.0f,
		CVASTParamState::toggleButtonValueToTextFunction,
		CVASTParamState::toggleButtonTextToValueFunction,
		false, true, true, false,
		true);

	createAndAddParameter(&m_uAtomizerTimeBeats, parameters, 1, "m_uAtomizerTimeBeats", "Atomizer time in beats when synced to DAW", "DAW", 9,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, TIMEBEATS_Array.size() - 1, 1.0f), TIMEBEATS::BEATS1,
		CVASTParamState::comboBoxValueToTextFunction_TIMEBEATS,
		CVASTParamState::comboBoxTextToValueFunction_TIMEBEATS,
		false, true, true, false,
		true);

	createAndAddParameter(&m_fAtomizerLFOFreq, parameters, 1, "m_fAtomizerLFOFreq", "Atomizer LFO frequency (Hz)", "Frequency", 10,
		MODMATDEST::AtomizerLFOFrequency,
		NormalisableRange<float>(0.0f, 20.0f), 1.00f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	createAndAddParameter(&m_fAtomizerGain, parameters, 1, "m_fAtomizerGain", "Atomizer output gain", "Gain", 11,
		MODMATDEST::AtomizerGain,
		NormalisableRange<float>(0, 200), 100,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);
}

void CVASTAtomizer::initCompatibilityParameters() {
	// all new parameters go here
}

void CVASTAtomizer::releaseResources() {
}

void CVASTAtomizer::parameterChanged(const String& parameterID, float newValue) {
	if (parameterID.startsWith("m_bAtomizerOnOff")) {
		if (newValue == static_cast<int>(SWITCH::SWITCH_ON))
			switchOn();
		else
			switchOff();
		return;
	}
	else if (parameterID.startsWith("m_fAtomizerDryWet")) {
		m_fAtomizerDryWet_smoothed.setTargetValue(newValue);
	}
	else if (parameterID.startsWith("m_fAtomizerHarmonics")) {
		m_fAtomizerHarmonics_smoothed.setTargetValue(newValue);
		setHarmonics(m_fAtomizerHarmonics_smoothed.getNextValue());
	}
	else if (parameterID.startsWith("m_fAtomizerGain")) {
		m_fAtomizerGain_smoothed.setTargetValue(newValue);
		setGain(m_fAtomizerGain_smoothed.getNextValue());
	}
	else if (parameterID.startsWith("m_fAtomizerEmphasis")) {
		m_fAtomizerEmphasis_smoothed.setTargetValue(newValue);
		setEmphasis(m_fAtomizerEmphasis_smoothed.getNextValue());
	}
	else if (parameterID.startsWith("m_fAtomizerLowcut")) {
		m_fAtomizerLowCut_smoothed.setTargetValue(newValue);
		setLowcut(m_fAtomizerLowCut_smoothed.getNextValue());
	}
	else if (parameterID.startsWith("m_fAtomizerHighcut")) {
		m_fAtomizerHighCut_smoothed.setTargetValue(newValue);
		setHighcut(m_fAtomizerHighCut_smoothed.getNextValue());
	}
	else if (parameterID.startsWith("m_fAtomizerDephase")) {
		m_fAtomizerDephase_smoothed.setTargetValue(newValue);
		setDephase(m_fAtomizerDephase_smoothed.getNextValue());
	}
	else if (parameterID.startsWith("m_fAtomizerLFOFreq")) {
		m_fAtomizerLFOFreq_smoothed.setTargetValue(newValue);
		updateLFOFreq();
	}
	else if (parameterID.startsWith("m_bAtomizerSynch")) {
		updateLFOFreq();
	}
	else if (parameterID.startsWith("m_uAtomizerTimeBeats")) {
		updateLFOFreq();
	}
	else if (parameterID.startsWith("m_uAtomizerLFOWave")) {
		m_LFO.updateMainVariables(m_iSampleRate, int(*m_uAtomizerLFOWave), 1, 1.0, 0, 0); //TODO CHECK
		m_LFO.startLFOFrequency(*m_fAtomizerLFOFreq, -1);
	}
}

inline void CVASTAtomizer::setHarmonics(float harmonics) { m_harmonics = harmonics; }

inline void CVASTAtomizer::setDephase(float dephase) { m_dephase = dephase; }

inline void CVASTAtomizer::setLowcut(float lowcut) { m_lowcut = lowcut; }

inline void CVASTAtomizer::setHighcut(float highcut) { m_highcut = highcut; }

inline void CVASTAtomizer::setEmphasis(float emphasis) { m_emphasis = emphasis; }

inline void CVASTAtomizer::setGain(float gain) { m_gain = gain; }

void CVASTAtomizer::updateTiming() {
	updateLFOFreq();
}

void CVASTAtomizer::updateLFOFreq() {
	if (*m_bAtomizerSynch == static_cast<int>(SWITCH::SWITCH_OFF)) {
		m_fAtomizerLFOFreq_smoothed.setTargetValue(*m_fAtomizerLFOFreq); //  *m_fTimeMod;
	}
	else { //bpm synch
		float l_fIntervalTime = 0.f;
		if (m_Set->m_dPpqBpm == 0.f) return;
		l_fIntervalTime  = m_Set->getIntervalTimeFromDAWBeats(*m_uAtomizerTimeBeats);
		if (l_fIntervalTime < 0.1f) l_fIntervalTime = 0.1f; // minimum
		//if (l_fIntervalTime > 5000.0f) l_fIntervalTime = 5000.f; // maximum
		if (l_fIntervalTime > 100000.0f) l_fIntervalTime = 100000.0f; // maximum  //CHTS 3.0.1

		m_fAtomizerLFOFreq_smoothed.setCurrentAndTargetValue(1); //reset it
		m_fAtomizerLFOFreq_smoothed.setTargetValue(1.0f / (l_fIntervalTime / 1000.f));
	}
}

void CVASTAtomizer::prepareToPlay(double , int samplesPerBlock) {
	//m_iSampleRate is set in useOversampling
	m_iExpectedSamplesPerBlock = samplesPerBlock;
	setup(2);
	if (m_bOversampling)
		m_fBuffer.setSize(2, samplesPerBlock * C_OVERSAMPLING_RATIO, false, false, false); 
	else 
		m_fBuffer.setSize(2, samplesPerBlock, false, false, false); 

	//stft.updateParameters((int)paramFftSize.getTargetValue(),
//		(int)paramHopSize.getTargetValue(),
		//(int)paramWindowType.getTargetValue());

	double smoothTime = 0.001;  //check 1e-3? ?????
	m_fAtomizerLFOFreq_smoothed.reset(m_iSampleRate, smoothTime);
	smoothTime = 0.02;
	m_fAtomizerDryWet_smoothed.reset(m_iSampleRate, smoothTime);
	m_fAtomizerLowCut_smoothed.reset(m_iSampleRate, smoothTime);
	m_fAtomizerHighCut_smoothed.reset(m_iSampleRate, smoothTime);
	m_fAtomizerHarmonics_smoothed.reset(m_iSampleRate, smoothTime);
	m_fAtomizerEmphasis_smoothed.reset(m_iSampleRate, smoothTime);
	m_fAtomizerDephase_smoothed.reset(m_iSampleRate, smoothTime);
	m_fAtomizerGain_smoothed.reset(m_iSampleRate, smoothTime);

	if (m_bOversampling)
		updateParameters(4096, 2, STFT::windowTypeHann); //recomendation 50% overlap and hann window
	else 
		updateParameters(1024, 2, STFT::windowTypeHann); //recomendation 50% overlap and hann window
	m_LFO.updateMainVariables(m_iSampleRate, int(*m_uAtomizerLFOWave), 1, 1.0, 0, 0); //TODO CHECK
	m_LFO.startLFOFrequency(*m_fAtomizerLFOFreq, -1);
	reset();
}

void CVASTAtomizer::reset() {
	if ((m_bIsOff == false) && (m_bShallBeOff == false)) {
		m_iSoftFade = 0;
		m_bShallBeOff = false; //refade in
		m_bIsOff = true;
	}
}

void CVASTAtomizer::init() {
	initParameters();
	reset();

	m_LFO.init();
	m_LFO.m_uPolarity = 0; //unipolar
	m_LFO.updateMainVariables(m_iSampleRate, WAVE::tri, 1, 1.0, 0, 0); //TODO CHECK
	m_LFO.startLFOFrequency(*m_fAtomizerLFOFreq, -1);
}

void CVASTAtomizer::processBlock(AudioSampleBuffer& buffer, MidiBuffer& , const int numSamples) {
	if (isOffAndShallBeOff() == true) return;

	ScopedNoDenormals noDenormals;

	//======================================

	processSTFTBlock(buffer, numSamples);

	//======================================

	buffer.applyGain(m_gain * 0.01f);
}

void CVASTAtomizer::processSTFTBlock(AudioSampleBuffer& block, const int numSamples) {
	float* channelDataL = block.getWritePointer(0);
	float* channelDataR = block.getWritePointer(1);

	currentInputBufferWritePosition = inputBufferWritePosition;
	currentOutputBufferWritePosition = outputBufferWritePosition;
	currentOutputBufferReadPosition = outputBufferReadPosition;
	currentSamplesSinceLastFFT = samplesSinceLastFFT;

	modMatrixInputState inputState = ((VASTAudioProcessor*)my_processor)->m_pVASTXperience.m_Poly.getOldestNotePlayedInputState(0); // make parameter oldest or newest

	float* inputBufferPointerL = inputBuffer.getWritePointer(0);
	float* inputBufferPointerR = inputBuffer.getWritePointer(1);
	float* outputBufferPointerL = outputBuffer.getWritePointer(0);
	float* outputBufferPointerR = outputBuffer.getWritePointer(1);

	for (int sample = 0; sample < numSamples; ++sample) {
		checkSoftFade();
		inputState.currentFrame = sample;
		m_fAtomizerDryWet_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_fAtomizerDryWet, MODMATDEST::AtomizerDryWet, &inputState));
		float lAtomizerDryWet = m_fAtomizerDryWet_smoothed.getNextValue();

		m_fAtomizerDephase_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_fAtomizerDephase, MODMATDEST::AtomizerDephase, &inputState));
		setDephase(m_fAtomizerDephase_smoothed.getNextValue());

		m_fAtomizerEmphasis_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_fAtomizerEmphasis, MODMATDEST::AtomizerEmphasis, &inputState));
		setEmphasis(m_fAtomizerEmphasis_smoothed.getNextValue());

		m_fAtomizerHarmonics_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_fAtomizerHarmonics, MODMATDEST::AtomizerHarmonics, &inputState));
		setHarmonics(m_fAtomizerHarmonics_smoothed.getNextValue());

		m_fAtomizerHighCut_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_fAtomizerHighcut, MODMATDEST::AtomizerHighCut, &inputState));
		setHighcut(m_fAtomizerHighCut_smoothed.getNextValue());

		m_fAtomizerLowCut_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_fAtomizerLowcut, MODMATDEST::AtomizerLowCut, &inputState));
		setLowcut(m_fAtomizerLowCut_smoothed.getNextValue());

		m_fAtomizerGain_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_fAtomizerGain, MODMATDEST::AtomizerGain, &inputState));
		setGain(m_fAtomizerGain_smoothed.getNextValue());

		if (*m_bAtomizerSynch == static_cast<int>(SWITCH::SWITCH_OFF))
			m_fAtomizerLFOFreq_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_fAtomizerLFOFreq, MODMATDEST::AtomizerLFOFrequency, &inputState));
		if (m_fAtomizerLFOFreq_smoothed.isSmoothing())
			m_LFO.startLFOFrequency(m_fAtomizerLFOFreq_smoothed.getNextValue(), -1);

		float fLFOOsc = 0.f;
		m_LFO.getOscillation(&fLFOOsc); //TODO
		setLFO(fLFOOsc);

		const float inputSampleL = channelDataL[sample];
		const float inputSampleR = channelDataR[sample];

		inputBufferPointerL[currentInputBufferWritePosition] = inputSampleL;
		inputBufferPointerR[currentInputBufferWritePosition] = inputSampleR;
		
		if (++currentInputBufferWritePosition >= inputBufferLength)
			currentInputBufferWritePosition = 0;

		channelDataL[sample] = outputBufferPointerL[currentOutputBufferReadPosition];
		channelDataR[sample] = outputBufferPointerR[currentOutputBufferReadPosition];

		outputBufferPointerL[currentOutputBufferReadPosition] = 0.0f;
		outputBufferPointerR[currentOutputBufferReadPosition] = 0.0f;

		if (++currentOutputBufferReadPosition >= outputBufferLength)
			currentOutputBufferReadPosition = 0;

		if (++currentSamplesSinceLastFFT >= hopSize) {
			currentSamplesSinceLastFFT = 0;
			analysis(0);
			modification();
			synthesis(0);

			analysis(1);
			modification();
			synthesis(1);

			currentOutputBufferWritePosition += hopSize;
			if (currentOutputBufferWritePosition >= outputBufferLength)
				currentOutputBufferWritePosition = 0;
		}
		
		float fMixin = (lAtomizerDryWet / 100.f) *  (float(m_iSoftFade) / float(C_MAX_SOFTFADE));
		channelDataL[sample] = inputSampleL * (1 - fMixin) + channelDataL[sample] * fMixin;
		channelDataR[sample] = inputSampleR * (1 - fMixin) + channelDataR[sample] * fMixin;
	}

	inputBufferWritePosition = currentInputBufferWritePosition;
	outputBufferWritePosition = currentOutputBufferWritePosition;
	outputBufferReadPosition = currentOutputBufferReadPosition;
	samplesSinceLastFFT = currentSamplesSinceLastFFT;
}

inline void CVASTAtomizer::setLFO(float lfo) { m_lfo = lfo; }

void CVASTAtomizer::modification() {
#ifdef _DEBUG
	std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
#endif

	fft->perform(timeDomainBuffer, frequencyDomainBuffer, false);

#ifdef _DEBUG
	std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
#endif

	switch (3) {
	case 0: {
		//for (int index = 2; index < fftSize; index+=index) { //zweierpotenzen
		for (int index = 0; index < fftSize / 2 + 1; ++index) {
			//float magnitude = abs(frequencyDomainBuffer[index]);
			if (index > int(((m_lfo + 1.f) / 2.f) * (fftSize / 2.0f + 1.f) * (m_transients / 10.f))) {
				frequencyDomainBuffer[index].real(0.0f);
				frequencyDomainBuffer[index].imag(0.0f);
				if (index > 0 && (index < fftSize / 2)) {
					frequencyDomainBuffer[fftSize - index].real(0.0f);
					frequencyDomainBuffer[fftSize - index].imag(0.0f);
				}
			}
			//frequencyDomainBuffer[index].real(realpart * m_lfo);

		}
		break;
		/*
		for (int index = 0; index < fftSize / 2 + 1; ++index) {
		float magnitude = abs(frequencyDomainBuffer[index]);
		float phase = 2.0f * M_PI * m_lfo;

		//frequencyDomainBuffer[index].real(magnitude);
		//frequencyDomainBuffer[index].imag(0.0f);

		float fMix1 = 1.f + (m_transients / 10.f) * magnitude * cosf(phase);
		float fMix2 = 0.f + (m_transients / 10.f) * magnitude * sinf(phase);
		frequencyDomainBuffer[index].real(magnitude * fMix1);
		frequencyDomainBuffer[index].imag(magnitude * fMix2);
		if (index > 0 && index < fftSize / 2) {
		frequencyDomainBuffer[fftSize - index].real(magnitude * fMix1);
		frequencyDomainBuffer[fftSize - index].imag(magnitude * fMix2);
		}
		}
		*/

	}
	case 1: {
		for (int index = 0; index < fftSize; ++index) {
			float magnitude = abs(frequencyDomainBuffer[index]);
			frequencyDomainBuffer[index].real(magnitude);
			frequencyDomainBuffer[index].imag(0.0f);
		}
		break;
	}
	case 2: {
		for (int index = 0; index < fftSize / 2 + 1; ++index) {
			float magnitude = abs(frequencyDomainBuffer[index]);
			float phase = 2.0f * M_PI * (float)rand() / (float)RAND_MAX;

			frequencyDomainBuffer[index].real(magnitude * cosf(phase));
			frequencyDomainBuffer[index].imag(magnitude * sinf(phase));
			if (index > 0 && index < fftSize / 2) {
				frequencyDomainBuffer[fftSize - index].real(magnitude * cosf(phase));
				frequencyDomainBuffer[fftSize - index].imag(magnitude * sinf(-phase));
			}
		}
		break;
	}
	case 3: {
		for (int index = 0; index < fftSize / 2 + 1; ++index) {
			float magnitude = abs(frequencyDomainBuffer[index]);
			float phase = arg(frequencyDomainBuffer[index]);

			//cuts
			if (index > int(((m_lfo + 1.f) / 2.f) * (fftSize / 2.0f + 1.f) * (m_highcut / 100.f))) {
				magnitude = 0.0f;
			}
			if (index < int(((m_lfo + 1.f) / 2.f) * (fftSize / 2.0f + 1.f) * (m_lowcut / 100.f))) {
				magnitude = 0.0f;
			}
			//harmonics
			magnitude = magnitude * (1.f + (float(index) / float(fftSize * 0.5f))  * (m_harmonics / 5.f)); //boost higher harmonics

																										   //dephase
			float fMix1 = (1.f * (m_dephase / 100.f) + ((100.f - m_dephase) / 100.f) * cosf(phase)) * magnitude;
			float fMix2 = (1.f * (m_dephase / 100.f) + ((100.f - m_dephase) / 100.f) * sinf(phase)) * magnitude;
			float fMix3 = (1.f * (m_dephase / 100.f) + ((100.f - m_dephase) / 100.f) * sinf(-phase)) * magnitude;

			//emphasis
			fMix2 *= ((100.f - m_emphasis) / 100.f); //0 is robotic
			fMix3 *= ((100.f - m_emphasis) / 100.f); //0 is robotic

			frequencyDomainBuffer[index].real(fMix1);
			frequencyDomainBuffer[index].imag(fMix2);
			if (index > 0 && index < fftSize / 2) {
				frequencyDomainBuffer[fftSize - index].real(fMix1);
				frequencyDomainBuffer[fftSize - index].imag(fMix3);
			}
		}
	}
	}

	fft->perform(frequencyDomainBuffer, timeDomainBuffer, true);
}

void CVASTAtomizer::getStateInformation(MemoryBlock& )
{
	//std::unique_ptr<XmlElement> xml (parameters.valueTreeState.state.createXml());
	//copyXmlToBinary (*xml, destData);
}

void CVASTAtomizer::setStateInformation(const void* , int )
{
	//std::unique_ptr<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
	//if (xmlState != nullptr)
	//  if (xmlState->hasTagName (parameters.valueTreeState.state.getType()))
	//    parameters.valueTreeState.state = ValueTree::fromXml (*xmlState);
}
