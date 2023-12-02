/*
VAST Dynamics Audio Software (TM)

All modulators tested: OK
*/

#include "VASTFormantFilter.h"
#include "../VASTEngineHeader.h"
#include "../VASTSettings.h"
#include "../../Plugin/VASTAudioProcessor.h"
#include "VASTEffect.h"

CVASTFormantFilter::CVASTFormantFilter(VASTAudioProcessor* processor, int busnr) {
	my_processor = processor;
	myBusnr = busnr;

	m_bIsOff = true;
	m_bShallBeOff = true;
	m_iSoftFade = 0;

	setEffectName("FORMANT FILTER");
}

void CVASTFormantFilter::initParameters() {
	AudioProcessorValueTreeState& parameters = my_processor->getParameterTree();

	createAndAddParameter(&m_bFormantOnOff, parameters, 1, "m_bFormantOnOff", "Formant filter on / off", "On", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 1.0f, 1.0f), 0.0f,
		CVASTParamState::toggleButtonValueToTextFunction,
		CVASTParamState::toggleButtonTextToValueFunction,
		false, true, true, true,
		true);

	createAndAddParameter(&m_fFormantDryWet, parameters, 1, "m_fFormantDryWet", "Formant filter dry wet", "DryWet", 1,
		MODMATDEST::FormantFilterDryWet, //tested OK
		NormalisableRange<float>(0, 100), 100,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	createAndAddParameter(&m_fFormantVowelOne, parameters, 1, "m_fFormantVowelOne", "Formant filter first formant vowel", "Vowel1", 2,
		MODMATDEST::FormantFilterFirstVowel, //tested OK
		NormalisableRange<float>(0.f, 4.f, 1.f), 0.f,
		[](float value) { return StringArray("A", "E", "I", "O", "U")[int(value)]; },
		[](String text) { return StringArray("A", "E", "I", "O", "U").indexOf(StringRef(text), true); },
		false, true, false, false);

	createAndAddParameter(&m_fFormantVowelTwo, parameters, 1, "m_fFormantVowelTwo", "Formant filter second formant vowel", "Vowel2", 3,
		MODMATDEST::FormantFilterSecondVowel, //tested OK
		NormalisableRange<float>(0.f, 4.f, 1.f), 0.f,
		[](float value) { return StringArray("A", "E", "I", "O", "U")[int(value)]; },
		[](String text) { return StringArray("A", "E", "I", "O", "U").indexOf(StringRef(text), true); },
		false, true, false, false);

	createAndAddParameter(&m_fFormantVowelMix, parameters, 1, "m_fFormantVowelMix", "Formant filter vowel mix", "Mix", 4,
		MODMATDEST::FormantFilterMixVowels, //tested OK
		NormalisableRange<float>(0, 100), 0,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);
	createAndAddParameter(&m_fFormantGain, parameters, 1, "m_fFormantGain", "Formant filter gain", "Gain", 5,
		MODMATDEST::FormantFilterGain,
		NormalisableRange<float>(0, 200), 100,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

}

void CVASTFormantFilter::initCompatibilityParameters() {
	// all new parameters go here
}

CVASTFormantFilter::~CVASTFormantFilter(void) {
	AudioProcessorValueTreeState& parameters = ((VASTAudioProcessor*)my_processor)->getParameterTree();
	Array<AudioProcessorParameterWithID*> params = getParameters();
	for (int i = 0; i < params.size(); i++) {
		parameters.removeParameterListener(params[i]->paramID, (AudioProcessorValueTreeState::Listener*)this);
	}
}

/*
Public source code by alex@smartelectronix.com
Simple example of implementation of formant filter
Vowelnum can be 0,1,2,3,4 <=> A,E,I,O,U
Good for spectral rich input like saw or square
*/
//-------------------------------------------------------------VOWEL COEFFICIENTS
const double coeff[5][11] = {
	{ 8.11044e-06,
	8.943665402,    -36.83889529,    92.01697887,    -154.337906,    181.6233289,
	-151.8651235,   89.09614114,    -35.10298511,    8.388101016,    -0.923313471  ///A
	},
{ 4.36215e-06,
8.90438318,    -36.55179099,    91.05750846,    -152.422234,    179.1170248,  ///E
-149.6496211,87.78352223,    -34.60687431,    8.282228154,    -0.914150747
},
{ 3.33819e-06,
8.893102966,    -36.49532826,    90.96543286,    -152.4545478,    179.4835618,
-150.315433,    88.43409371,    -34.98612086,    8.407803364,    -0.932568035  ///I
},
{ 1.13572e-06,
8.994734087,    -37.2084849,    93.22900521,    -156.6929844,    184.596544,   ///O
-154.3755513,    90.49663749,    -35.58964535,    8.478996281,    -0.929252233
},
{ 4.09431e-07,
8.997322763,    -37.20218544,    93.11385476,    -156.2530937,    183.7080141,  ///U
-153.2631681,    89.59539726,    -35.12454591,    8.338655623,    -0.910251753
}
};


void CVASTFormantFilter::releaseResources() {
}

void CVASTFormantFilter::parameterChanged(const String& parameterID, float newValue) {
	if (parameterID.startsWith("m_bFormantOnOff")) {
		if (newValue == static_cast<int>(SWITCH::SWITCH_ON))
			switchOn();
		else
			switchOff();
		return;
	}
	else if (parameterID.startsWith("m_fFormantVowelMix")) {
		m_fFormantVowelMix_smoothed.setTargetValue(newValue);
	}
	else if (parameterID.startsWith("m_fFormantDryWet")) {
		m_fFormantDryWetMix_smoothed.setTargetValue(newValue);
	}
	else if (parameterID.startsWith("m_fFormantGain")) {
		m_fFormantGain_smoothed.setTargetValue(newValue);
	}
}

void CVASTFormantFilter::prepareToPlay(double , int samplesPerBlock) {
	//m_iSampleRate is set in useroversampling;
	m_iExpectedSamplesPerBlock = samplesPerBlock;

	reset();
	const double smoothTime = 0.02; 
	m_fFormantVowelMix_smoothed.reset(m_iSampleRate, smoothTime);
	m_fFormantDryWetMix_smoothed.reset(m_iSampleRate, smoothTime);
	m_fFormantGain_smoothed.reset(m_iSampleRate, smoothTime);
}

void CVASTFormantFilter::reset() {
	memset(memoryOne_left, 0, 10 * sizeof(double));
	memset(memoryOne_right, 0, 10 * sizeof(double));
	memset(memoryTwo_left, 0, 10 * sizeof(double));
	memset(memoryTwo_right, 0, 10 * sizeof(double));

	if ((m_bIsOff == false) && (m_bShallBeOff == false)) {
		m_iSoftFade = 0;
		m_bShallBeOff = false; //refade in
		m_bIsOff = true;
	}
}

void CVASTFormantFilter::init(CVASTSettings &set) {
	m_Set = &set;
	initParameters();
	reset();
}

void CVASTFormantFilter::processBlock(AudioSampleBuffer& buffer, MidiBuffer& , const int numSamples) {
	if (isOffAndShallBeOff() == true) return;

	float* bufferWritePointerL = buffer.getWritePointer(0);
	float* bufferWritePointerR = buffer.getWritePointer(1);

	for (int currentFrame = 0; currentFrame < numSamples; currentFrame += ((m_bOversampling)?C_OVERSAMPLING_RATIO:1)) { //different logic than other effects since oversampling not possible
		//int currentFrameOSAdjusted = currentFrame;
		
		checkSoftFade();

		float fIn[2];
		float fOut[2];
		fIn[0] = bufferWritePointerL[currentFrame];
		fIn[1] = bufferWritePointerR[currentFrame];

		processAudioFrame(fIn, fOut, 2, 2, currentFrame);

		if (!m_bOversampling) {
			bufferWritePointerL[currentFrame] = fOut[0];
			bufferWritePointerR[currentFrame] = fOut[1];
		}
		else {
			bufferWritePointerL[currentFrame] = fOut[0];
			bufferWritePointerR[currentFrame] = fOut[1];
			bufferWritePointerL[currentFrame + 1] = fOut[0];
			bufferWritePointerR[currentFrame + 1] = fOut[1];
			bufferWritePointerL[currentFrame + 2] = fOut[0];
			bufferWritePointerR[currentFrame + 2] = fOut[1];
			bufferWritePointerL[currentFrame + 3] = fOut[0];
			bufferWritePointerR[currentFrame + 3] = fOut[1];
		}
	}
}

bool CVASTFormantFilter::processAudioFrame(float* pInputBuffer, float* pOutputBuffer, MYUINT , MYUINT , int currentFrameOSAdjusted) {

	modMatrixInputState inputState;
	inputState = ((VASTAudioProcessor*)my_processor)->m_pVASTXperience.m_Poly.getOldestNotePlayedInputState(currentFrameOSAdjusted); // make parameter oldest or newest

	m_fFormantVowelMix_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_fFormantVowelMix, MODMATDEST::FormantFilterMixVowels, &inputState));
	float lFormantVowelMix = m_fFormantVowelMix_smoothed.getNextValue() * 0.01f;
	
	m_fFormantDryWetMix_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_fFormantDryWet, MODMATDEST::FormantFilterDryWet, &inputState));
	float lFormantDryWet = m_fFormantDryWetMix_smoothed.getNextValue();

	m_fFormantGain_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_fFormantGain, MODMATDEST::FormantFilterGain, &inputState));
	float lFormantGain = m_fFormantGain_smoothed.getNextValue();

	float l_vowelOne = m_Set->getParameterValueWithMatrixModulation(m_fFormantVowelOne, MODMATDEST::FormantFilterFirstVowel, &inputState);
	float l_vowelTwo = m_Set->getParameterValueWithMatrixModulation(m_fFormantVowelTwo, MODMATDEST::FormantFilterSecondVowel, &inputState);
	
	float l_outputOneLeft = 0.0f;
	float l_outputOneRight = 0.0f;
	float l_outputTwoLeft = 0.0f;
	float l_outputTwoRight = 0.0f;
	int vowelnumOne = l_vowelOne;
	l_outputOneLeft = (float)(
		coeff[vowelnumOne][0] * pInputBuffer[0] +
		coeff[vowelnumOne][1] * memoryOne_left[0] +
		coeff[vowelnumOne][2] * memoryOne_left[1] +
		coeff[vowelnumOne][3] * memoryOne_left[2] +
		coeff[vowelnumOne][4] * memoryOne_left[3] +
		coeff[vowelnumOne][5] * memoryOne_left[4] +
		coeff[vowelnumOne][6] * memoryOne_left[5] +
		coeff[vowelnumOne][7] * memoryOne_left[6] +
		coeff[vowelnumOne][8] * memoryOne_left[7] +
		coeff[vowelnumOne][9] * memoryOne_left[8] +
		coeff[vowelnumOne][10] * memoryOne_left[9]);

	memoryOne_left[9] = memoryOne_left[8];
	memoryOne_left[8] = memoryOne_left[7];
	memoryOne_left[7] = memoryOne_left[6];
	memoryOne_left[6] = memoryOne_left[5];
	memoryOne_left[5] = memoryOne_left[4];
	memoryOne_left[4] = memoryOne_left[3];
	memoryOne_left[3] = memoryOne_left[2];
	memoryOne_left[2] = memoryOne_left[1];
	memoryOne_left[1] = memoryOne_left[0];
	memoryOne_left[0] = l_outputOneLeft;

	l_outputOneRight = (float)(
		coeff[vowelnumOne][0] * pInputBuffer[1] +
		coeff[vowelnumOne][1] * memoryOne_right[0] +
		coeff[vowelnumOne][2] * memoryOne_right[1] +
		coeff[vowelnumOne][3] * memoryOne_right[2] +
		coeff[vowelnumOne][4] * memoryOne_right[3] +
		coeff[vowelnumOne][5] * memoryOne_right[4] +
		coeff[vowelnumOne][6] * memoryOne_right[5] +
		coeff[vowelnumOne][7] * memoryOne_right[6] +
		coeff[vowelnumOne][8] * memoryOne_right[7] +
		coeff[vowelnumOne][9] * memoryOne_right[8] +
		coeff[vowelnumOne][10] * memoryOne_right[9]);

	memoryOne_right[9] = memoryOne_right[8];
	memoryOne_right[8] = memoryOne_right[7];
	memoryOne_right[7] = memoryOne_right[6];
	memoryOne_right[6] = memoryOne_right[5];
	memoryOne_right[5] = memoryOne_right[4];
	memoryOne_right[4] = memoryOne_right[3];
	memoryOne_right[3] = memoryOne_right[2];
	memoryOne_right[2] = memoryOne_right[1];
	memoryOne_right[1] = memoryOne_right[0];
	memoryOne_right[0] = l_outputOneRight;

	int vowelnumTwo = l_vowelTwo;
	l_outputTwoLeft = (float)(
		coeff[vowelnumTwo][0] * pInputBuffer[0] +
		coeff[vowelnumTwo][1] * memoryTwo_left[0] +
		coeff[vowelnumTwo][2] * memoryTwo_left[1] +
		coeff[vowelnumTwo][3] * memoryTwo_left[2] +
		coeff[vowelnumTwo][4] * memoryTwo_left[3] +
		coeff[vowelnumTwo][5] * memoryTwo_left[4] +
		coeff[vowelnumTwo][6] * memoryTwo_left[5] +
		coeff[vowelnumTwo][7] * memoryTwo_left[6] +
		coeff[vowelnumTwo][8] * memoryTwo_left[7] +
		coeff[vowelnumTwo][9] * memoryTwo_left[8] +
		coeff[vowelnumTwo][10] * memoryTwo_left[9]);

	memoryTwo_left[9] = memoryTwo_left[8];
	memoryTwo_left[8] = memoryTwo_left[7];
	memoryTwo_left[7] = memoryTwo_left[6];
	memoryTwo_left[6] = memoryTwo_left[5];
	memoryTwo_left[5] = memoryTwo_left[4];
	memoryTwo_left[4] = memoryTwo_left[3];
	memoryTwo_left[3] = memoryTwo_left[2];
	memoryTwo_left[2] = memoryTwo_left[1];
	memoryTwo_left[1] = memoryTwo_left[0];
	memoryTwo_left[0] = l_outputTwoLeft;

	l_outputTwoRight = (float)(
		coeff[vowelnumTwo][0] * pInputBuffer[1] +
		coeff[vowelnumTwo][1] * memoryTwo_right[0] +
		coeff[vowelnumTwo][2] * memoryTwo_right[1] +
		coeff[vowelnumTwo][3] * memoryTwo_right[2] +
		coeff[vowelnumTwo][4] * memoryTwo_right[3] +
		coeff[vowelnumTwo][5] * memoryTwo_right[4] +
		coeff[vowelnumTwo][6] * memoryTwo_right[5] +
		coeff[vowelnumTwo][7] * memoryTwo_right[6] +
		coeff[vowelnumTwo][8] * memoryTwo_right[7] +
		coeff[vowelnumTwo][9] * memoryTwo_right[8] +
		coeff[vowelnumTwo][10] * memoryTwo_right[9]);

	memoryTwo_right[9] = memoryTwo_right[8];
	memoryTwo_right[8] = memoryTwo_right[7];
	memoryTwo_right[7] = memoryTwo_right[6];
	memoryTwo_right[6] = memoryTwo_right[5];
	memoryTwo_right[5] = memoryTwo_right[4];
	memoryTwo_right[4] = memoryTwo_right[3];
	memoryTwo_right[3] = memoryTwo_right[2];
	memoryTwo_right[2] = memoryTwo_right[1];
	memoryTwo_right[1] = memoryTwo_right[0];
	memoryTwo_right[0] = l_outputTwoRight;

	//attenuate A
	if (l_vowelOne == 0) {
		l_outputOneLeft *= 0.7f;
		l_outputOneRight *= 0.7f;
	}
	if (l_vowelTwo == 0) {
		l_outputTwoLeft *= 0.7f;
		l_outputTwoRight *= 0.7f;
	}
	//mix the two
	pOutputBuffer[0] = l_outputOneLeft * (1 - lFormantVowelMix) + l_outputTwoLeft * lFormantVowelMix;
	pOutputBuffer[1] = l_outputOneRight * (1 - lFormantVowelMix) + l_outputTwoRight * lFormantVowelMix;
	
	//mix filter in 
	float formantMix = lFormantDryWet / 100.f * (float(m_iSoftFade) / float(C_MAX_SOFTFADE));
	formantMix *= 0.8f; //too loud otherwise
	pOutputBuffer[0] = pInputBuffer[0] * (1 - formantMix) + pOutputBuffer[0] * formantMix; //otherwise to loud
	pOutputBuffer[1] = pInputBuffer[1] * (1 - formantMix) + pOutputBuffer[1] * formantMix; //otherwise to loud

	pOutputBuffer[0] *= lFormantGain * 0.01f;
	pOutputBuffer[1] *= lFormantGain * 0.01f;

	vassert((pOutputBuffer[0] > -10.0f) && (pOutputBuffer[0] <= 10.0));
	vassert((pOutputBuffer[1] > -10.0f) && (pOutputBuffer[1] <= 10.0));
	if (((pOutputBuffer[0] < -10.0f) || (pOutputBuffer[0] >= 10.0)) ||
		((pOutputBuffer[1] < -10.0f) || (pOutputBuffer[1] >= 10.0))) {
		reset();
	}
	return true;
}

//==============================================================================

void CVASTFormantFilter::getStateInformation(MemoryBlock& )
{
	//std::unique_ptr<XmlElement> xml (parameters.valueTreeState.state.createXml());
	//copyXmlToBinary (*xml, destData);
}

void CVASTFormantFilter::setStateInformation(const void* , int )
{
	//std::unique_ptr<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
	//if (xmlState != nullptr)
	//  if (xmlState->hasTagName (parameters.valueTreeState.state.getType()))
	//    parameters.valueTreeState.state = ValueTree::fromXml (*xmlState);
}
