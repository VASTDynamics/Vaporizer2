/*
VAST Dynamics Audio Software (TM)

All modulators tested: OK
*/

#include "VASTWaveshaper.h"
#include "../Filter/VASTBiQuadFilter.h"
#include "../VASTEngineHeader.h"
#include "../../Plugin/VASTAudioProcessor.h"
#include "VASTEffect.h"
#include <math.h>
#ifdef __aarch64__ //arm64
	#include "../../sse2neon.h"
#else
	#include "emmintrin.h"
#endif

CVASTWaveshaper::CVASTWaveshaper(VASTAudioProcessor* processor, int busnr) {
	my_processor = processor;
	myBusnr = busnr;
	setEffectName("WAVESHAPER");

	m_bIsOff = true;
	m_bShallBeOff = true;
	m_iSoftFade = 0;
}

void CVASTWaveshaper::initParameters() {
}

void CVASTWaveshaper::initCompatibilityParameters() {
}

void CVASTWaveshaper::initCompatibilityParameters5() {
	AudioProcessorValueTreeState& parameters = my_processor->getParameterTree();

	createAndAddParameter(&m_bWaveshaperOnOff, parameters, 1, "m_bWaveshaperOnOff", "Waveshaper effect on / off", "On", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 1.0f, 1.0f), 0.0f,
		CVASTParamState::toggleButtonValueToTextFunction,
		CVASTParamState::toggleButtonTextToValueFunction,
		false, true, true, true,
		true);

	createAndAddParameter(&m_fWaveshaperDryWet, parameters, 1, "m_fWaveshaperDryWet", "Waveshaper dry / wet (no effect - full effect)", "DryWet", 1,
		MODMATDEST::WaveshaperDryWet,
		NormalisableRange<float>(0, 100), 10.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);


	createAndAddParameter(&m_uWaveshaperType, parameters, 1, "m_uWaveshaperType", "Waveshaper type", "Type", 2,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 3.0f, 1.0f), 0.0f,
		[](float value) { return StringArray("HardClip", "Asym", "Sinus", "Tanh")[int(value)]; },
		[](String text) { return StringArray("HardClip", "Asym", "Sinus", "Tanh").indexOf(StringRef(text), true); },
		false, true, false, false,
		true);

	createAndAddParameter(&m_fWaveshaperPreGain, parameters, 1, "m_fWaveshaperPreGain", "Waveshaper pre-gain", "PreGain", 3,
		MODMATDEST::WaveshaperPreGain,
		NormalisableRange<float>(0, 200), 100,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	createAndAddParameter(&m_fWaveshaperLowcut, parameters, 1, "m_fWaveshaperLowcut", "Waveshaper lowcut frequency", "Lowcut", 4,
		MODMATDEST::WaveshaperLowCut,
		NormalisableRange<float>(50.f, 18000.f, 0.001f, 0.3f, false), 50.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	createAndAddParameter(&m_fWaveshaperHighcut, parameters, 1, "m_fWaveshaperHighcut", "Waveshaper highcut frequency", "Highcut", 5,
		MODMATDEST::WaveshaperHighCut,
		NormalisableRange<float>(50.f, 18000.f, 0.001f, 0.3f, false), 18000.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	createAndAddParameter(&m_uWaveshaperPrePostEq, parameters, 1, "m_uWaveshaperPrePostEq", "Waveshaper pre / post EQ", "EQ", 6,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 1.0f, 1.0f), 0.0f,
		[](float value) { return StringArray("Pre", "Post")[int(value)]; },
		[](String text) { return StringArray("Pre", "Post").indexOf(StringRef(text), true); },
		false, true, false, false,
		true);

	createAndAddParameter(&m_fWaveshaperDrive, parameters, 1, "m_fWaveshaperDrive", "Waveshaper drive (pre gain before waveshaping)", "Drive", 7,
		MODMATDEST::WaveshaperDrive,
		NormalisableRange<float>(0, 100), 30.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	createAndAddParameter(&m_fWaveshaperGain, parameters, 1, "m_fWaveshaperGain", "Waveshaper output gain", "Gain", 8,
		MODMATDEST::WaveshaperGain,
		NormalisableRange<float>(0, 200), 100,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);
}

CVASTWaveshaper::~CVASTWaveshaper(void) {
	AudioProcessorValueTreeState& parameters = ((VASTAudioProcessor*)my_processor)->getParameterTree();
	Array<AudioProcessorParameterWithID*> params = getParameters();
	for (int i = 0; i < params.size(); i++) {
		parameters.removeParameterListener(params[i]->paramID, (AudioProcessorValueTreeState::Listener*)this);
	}
}

void CVASTWaveshaper::parameterChanged(const String& parameterID, float newValue) {
	if (parameterID.startsWith("m_bWaveshaperOnOff")) {
		if (newValue == static_cast<int>(SWITCH::SWITCH_ON))
			switchOn();
		else
			switchOff();
	}
	else if (parameterID.startsWith("m_fWaveshaperDryWet")) {
		m_fWaveshaperDryWet_smoothed.setTargetValue(newValue);
	}
	else if (parameterID.startsWith("m_fWaveshaperDrive")) {
		m_fWaveshaperDrive_smoothed.setTargetValue(newValue);
	}
	else if (parameterID.startsWith("m_fWaveshaperLowcut")) {
		m_fWaveshaperLowcut_smoothed.setTargetValue(newValue);
	}
	else if (parameterID.startsWith("m_fWaveshaperHighcut")) {
		m_fWaveshaperHighcut_smoothed.setTargetValue(newValue);
	}
	else if (parameterID.startsWith("m_fWaveshaperGain")) {
		m_fWaveshaperGain_smoothed.setTargetValue(newValue);
	}
	else if (parameterID.startsWith("m_fWaveshaperPreGain")) {
		m_fWaveshaperPreGain_smoothed.setTargetValue(newValue);
	}
}

void CVASTWaveshaper::init(CVASTSettings &set) {
	m_Set = &set;
	initParameters();

	m_biQuadPreL.flushDelays();
	m_biQuadPreR.flushDelays();
	m_biQuadPostL.flushDelays();
	m_biQuadPostR.flushDelays();

	m_lowCutBiquadL.flushDelays();
	m_lowCutBiquadR.flushDelays();
	m_highCutBiquadL.flushDelays();
	m_highCutBiquadR.flushDelays();

	float fQ = 1.0f;
	float preShelfGain = -20.0f;
	m_biQuadPreL.calcBiquad(CVASTBiQuad::LOWSHELF, 800.0f, m_iSampleRate, fQ, preShelfGain);
	m_biQuadPreR.calcBiquad(CVASTBiQuad::LOWSHELF, 800.0f, m_iSampleRate, fQ, preShelfGain);

	float postShelfGain = -20.0f;
	m_biQuadPostL.calcBiquad(CVASTBiQuad::HIGHSHELF, 2000.0f, m_iSampleRate, fQ, postShelfGain);
	m_biQuadPostR.calcBiquad(CVASTBiQuad::HIGHSHELF, 2000.0f, m_iSampleRate, fQ, postShelfGain);

	m_lowCutBiquadL.calcBiquad(CVASTBiQuad::HIGHPASS, 50.0f, m_iSampleRate, sqrt2over2, -18.0f);
	m_lowCutBiquadR.copySettingsFrom(&m_lowCutBiquadL);
	m_highCutBiquadL.calcBiquad(CVASTBiQuad::LOWPASS, 18000.0f, m_iSampleRate, sqrt2over2, -18.0f);
	m_highCutBiquadR.copySettingsFrom(&m_highCutBiquadL);
	reset();
}

void CVASTWaveshaper::reset() {
	m_biQuadPreL.flushDelays();
	m_biQuadPreR.flushDelays();
	m_biQuadPostL.flushDelays();
	m_biQuadPostR.flushDelays();

	m_lowCutBiquadL.flushDelays();
	m_lowCutBiquadR.flushDelays();
	m_highCutBiquadL.flushDelays();
	m_highCutBiquadR.flushDelays();

	if ((m_bIsOff == false) && (m_bShallBeOff == false)) {
		m_iSoftFade = 0;
		m_bShallBeOff = false; //refade in
		m_bIsOff = true;
	}
}

void CVASTWaveshaper::prepareToPlay(double sampleRate, int samplesPerBlock) {
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
	m_highCutBiquadL.calcBiquad(CVASTBiQuad::LOWPASS, 18000.0f, m_iSampleRate, sqrt2over2, -18.0f);
	m_highCutBiquadR.copySettingsFrom(&m_highCutBiquadL);

	const double smoothTime = 0.02;
	m_fWaveshaperDryWet_smoothed.reset(m_iSampleRate, smoothTime);
	m_fWaveshaperDrive_smoothed.reset(m_iSampleRate, smoothTime);
	m_fWaveshaperHighcut_smoothed.reset(m_iSampleRate, smoothTime);
	m_fWaveshaperLowcut_smoothed.reset(m_iSampleRate, smoothTime);
	m_fWaveshaperGain_smoothed.reset(m_iSampleRate, smoothTime);
	m_fWaveshaperPreGain_smoothed.reset(m_iSampleRate, smoothTime);

	reset();
}

void CVASTWaveshaper::releaseResources()
{
}

void CVASTWaveshaper::processBlock(AudioSampleBuffer& buffer, MidiBuffer& midiMessages, const int numSamples) {
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

		m_fWaveshaperGain_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_fWaveshaperGain, MODMATDEST::WaveshaperGain, &inputState));
		float lWaveshaperGain = m_fWaveshaperGain_smoothed.getNextValue();

		bufferWritePointerL[currentFrame] = fOut[0] * lWaveshaperGain * 0.01f;
		bufferWritePointerR[currentFrame] = fOut[1] * lWaveshaperGain * 0.01f;

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

bool CVASTWaveshaper::processAudioFrame(float* pInputBuffer, float* pOutputBuffer, MYUINT uNumInputChannels, MYUINT uNumOutputChannels, modMatrixInputState &inputState)
{
	//pre - eq(bandpass) > pre - gain > nonlinear waveshaping function > post - eq(lowpass) > post - gain and you may wish to eliminate aliasing using the bandlimited polynomials or by oversampling.
	//http://music.columbia.edu/cmc/music-dsp/FAQs/guitar_distortion_FAQ.html
		
	//DryWet Mod
	m_fWaveshaperDryWet_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_fWaveshaperDryWet, MODMATDEST::WaveshaperDryWet, &inputState));
	float lWaveshaperWet = m_fWaveshaperDryWet_smoothed.getNextValue();

	//Drive Mod
	m_fWaveshaperDrive_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_fWaveshaperDrive, MODMATDEST::WaveshaperDrive, &inputState));
	float lWaveshaperDrive = m_fWaveshaperDrive_smoothed.getNextValue();

	//Pregain Mod
	m_fWaveshaperPreGain_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_fWaveshaperPreGain, MODMATDEST::WaveshaperPreGain, &inputState));
	float lWaveshaperPreGain = m_fWaveshaperPreGain_smoothed.getNextValue();

	//Lowcut Mod
	m_fWaveshaperLowcut_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_fWaveshaperLowcut, MODMATDEST::WaveshaperLowCut, &inputState));
	if (m_fWaveshaperLowcut_smoothed.isSmoothing()) {
		float lWaveshaperLowcut = m_fWaveshaperLowcut_smoothed.getNextValue();
		float fQ = float(sqrt2over2);
		m_lowCutBiquadL.calcBiquad(CVASTBiQuad::HIGHPASS, lWaveshaperLowcut, m_iSampleRate, fQ, -18.0f);
		m_lowCutBiquadR.copySettingsFrom(&m_lowCutBiquadL);
	}

	//Highcut Mod
	m_fWaveshaperHighcut_smoothed.setTargetValue(m_Set->getParameterValueWithMatrixModulation(m_fWaveshaperHighcut, MODMATDEST::WaveshaperHighCut, &inputState));
	if (m_fWaveshaperHighcut_smoothed.isSmoothing()) {
		float lWaveshaperHighcut = m_fWaveshaperHighcut_smoothed.getNextValue();
		float fQ = float(sqrt2over2);
		m_highCutBiquadL.calcBiquad(CVASTBiQuad::LOWPASS, lWaveshaperHighcut, m_iSampleRate, fQ, -18.0f);
		m_highCutBiquadR.copySettingsFrom(&m_highCutBiquadL);
	}

	float xnL = pInputBuffer[0] * lWaveshaperPreGain * 0.01f;
	float xnR = pInputBuffer[1] * lWaveshaperPreGain * 0.01f;

	// final outputs for this sample period
	float ynL = 0.0; // = pInputBuffer[0];
	float ynR = 0.0; // = pInputBuffer[1];
	
	//xnL = m_biQuadPreL.doBiQuad(xnL);
	//xnR = m_biQuadPreL.doBiQuad(xnR);
	
	if (*m_uWaveshaperPrePostEq == 0) { //pre
		xnL = m_lowCutBiquadL.doBiQuad(xnL); //do pre filter
		xnR = m_lowCutBiquadR.doBiQuad(xnR); //do pre filter
		xnL = m_highCutBiquadL.doBiQuad(xnL); //do pre filter
		xnR = m_highCutBiquadR.doBiQuad(xnR); //do pre filter
	}
	
	float ldrive = (lWaveshaperDrive / 100.f) * 24.f; //-24db .. +24db --> 0,066 ... 15,84 amplitude
	ldrive = pow(10.0f, ldrive / 20.0f);

	__m128 inlr = _mm_set1_ps(0.f);
	set1f(inlr, 0, xnL);
	set1f(inlr, 1, xnR);	
	__m128 drive = _mm_set1_ps(ldrive);
	__m128 oulr;
	switch (int(*m_uWaveshaperType)) { //"HardClip", "SoftClip", "Asym", "Sinus", "Tanh"
	case 0: {}; //HardClip
			oulr = CLIP(inlr, drive);
			break;
/*
	case 1: {}; //SoftClip
			oulr = DIGI_SSE2(inlr, drive);
			break;
			*/
	case 1: {}; //Asym
			oulr = ASYM_SSE2(inlr, drive);
			break;
	case 2: {}; //Sinus
			oulr = SINUS_SSE2(inlr, drive);
			break;
	case 3: {}; //Tanh
			oulr = TANH(inlr, drive);
			break;
	default:
            oulr = CLIP(inlr, drive);
            vassertfalse;
	}
	ynL = get1f(oulr, 0);
	ynR = get1f(oulr, 1);

	// DO POST EQ
	//ynL = m_biQuadPostL.doBiQuad(ynL);
	//ynR = m_biQuadPostL.doBiQuad(ynR);

	if (*m_uWaveshaperPrePostEq == 1) { //post
		ynL = m_lowCutBiquadL.doBiQuad(ynL); //do pre filter
		ynR = m_lowCutBiquadR.doBiQuad(ynR); //do pre filter
		ynL = m_highCutBiquadL.doBiQuad(ynL); //do pre filter
		ynR = m_highCutBiquadR.doBiQuad(ynR); //do pre filter
	}

	float fWet = (lWaveshaperWet / 100.f) * (float(m_iSoftFade) / float(C_MAX_SOFTFADE));
	pOutputBuffer[0] = (1.0f - fWet) * pInputBuffer[0] + (ynL * fWet);   // mix in;
	pOutputBuffer[1] = (1.0f - fWet) * pInputBuffer[1] + (ynR * fWet);   // mix in;

	return true;
}

__m128 CVASTWaveshaper::CLIP(__m128 in, __m128 drive)
{
	const __m128 x_min = _mm_set1_ps(-1.0f);
	const __m128 x_max = _mm_set1_ps(1.0f);
	return _mm_max_ps(_mm_min_ps(_mm_mul_ps(in, drive), x_max), x_min);
}

/*
__m128 CVASTWaveshaper::DIGI_SSE2(__m128 in, __m128 drive)
{
	// v1.2: return (double)((int)((double)(x*p0inv*16.f+1.0)))*p0*0.0625f;
	const __m128 mzero = _mm_set1_ps(0.f);
	const __m128 m16 = _mm_set1_ps(16.f);
	const __m128 m16inv = _mm_set1_ps(0.0625f);
	const __m128 mofs = _mm_set1_ps(0.5f);

	__m128 invdrive = _mm_rcp_ps(drive);
	__m128i a = _mm_cvtps_epi32(_mm_add_ps(mofs, _mm_mul_ps(invdrive, _mm_mul_ps(m16, in))));

	if ((*((int*)&a + 0) == 0) && (*((int*)&a + 1) == 0))
		return mzero;

	//return _mm_mul_ps(drive, _mm_mul_ps(m16inv, _mm_sub_ps(_mm_cvtepi32_ps(a), mofs)));
	return _mm_mul_ps(drive, _mm_mul_ps(m16inv, _mm_cvtepi32_ps(a)));
}
*/

__m128 CVASTWaveshaper::TANH(__m128 in, __m128 drive)
{
	// Closer to ideal than TANH0
	// y = x * ( 27 + x * x ) / ( 27 + 9 * x * x );
	// y = clip(y)

	const __m128 m9 = _mm_set1_ps(9.f);
	const __m128 m27 = _mm_set1_ps(27.f);

	__m128 x = _mm_mul_ps(in, drive);
	__m128 xx = _mm_mul_ps(x, x);
	__m128 denom = _mm_add_ps(m27, _mm_mul_ps(m9, xx));
	__m128 y = _mm_mul_ps(x, _mm_add_ps(m27, xx));
	y = _mm_mul_ps(y, _mm_rcp_ps(denom));

	const __m128 y_min = _mm_set1_ps(-1.0f);
	const __m128 y_max = _mm_set1_ps(1.0f);
	return _mm_max_ps(_mm_min_ps(y, y_max), y_min);
}

__m128 CVASTWaveshaper::SINUS_SSE2(__m128 in, __m128 drive)
{
	const __m128 one = _mm_set1_ps(1.f);
	const __m128 m256 = _mm_set1_ps(256.f);
	const __m128 m512 = _mm_set1_ps(512.f);

	__m128 x = _mm_mul_ps(in, drive);
	x = _mm_add_ps(_mm_mul_ps(x, m256), m512);

	__m128i e = _mm_cvtps_epi32(x);
	__m128 a = _mm_sub_ps(x, _mm_cvtepi32_ps(e));
	e = _mm_packs_epi32(e, e);

#if MAC
	// this should be very fast on C2D/C1D (and there are no macs with K8's)
	// GCC seems to optimize around the XMM -> int transfers so this is needed here
	int e4 alignas(16)[4];
	e4[0] = _mm_cvtsi128_si32(e);
	e4[1] = _mm_cvtsi128_si32(_mm_shufflelo_epi16(e, _MM_SHUFFLE(1, 1, 1, 1)));
	e4[2] = _mm_cvtsi128_si32(_mm_shufflelo_epi16(e, _MM_SHUFFLE(2, 2, 2, 2)));
	e4[3] = _mm_cvtsi128_si32(_mm_shufflelo_epi16(e, _MM_SHUFFLE(3, 3, 3, 3)));
#else
	// on PC write to memory & back as XMM -> GPR is slow on K8
	short e4 alignas(16)[8];
	_mm_store_si128((__m128i*)&e4, e);
#endif

	__m128 ws1 = _mm_load_ss(&waveshapers[3][e4[0] & 0x3ff]);
	__m128 ws2 = _mm_load_ss(&waveshapers[3][e4[1] & 0x3ff]);
	__m128 ws3 = _mm_load_ss(&waveshapers[3][e4[2] & 0x3ff]);
	__m128 ws4 = _mm_load_ss(&waveshapers[3][e4[3] & 0x3ff]);
	__m128 ws = _mm_movelh_ps(_mm_unpacklo_ps(ws1, ws2), _mm_unpacklo_ps(ws3, ws4));
	ws1 = _mm_load_ss(&waveshapers[3][(e4[0] + 1) & 0x3ff]);
	ws2 = _mm_load_ss(&waveshapers[3][(e4[1] + 1) & 0x3ff]);
	ws3 = _mm_load_ss(&waveshapers[3][(e4[2] + 1) & 0x3ff]);
	ws4 = _mm_load_ss(&waveshapers[3][(e4[3] + 1) & 0x3ff]);
	__m128 wsn = _mm_movelh_ps(_mm_unpacklo_ps(ws1, ws2), _mm_unpacklo_ps(ws3, ws4));

	x = _mm_add_ps(_mm_mul_ps(_mm_sub_ps(one, a), ws), _mm_mul_ps(a, wsn));

	return x;
}

__m128 CVASTWaveshaper::ASYM_SSE2(__m128 in, __m128 drive)
{
	const __m128 one = _mm_set1_ps(1.f);
	const __m128 m32 = _mm_set1_ps(32.f);
	const __m128 m512 = _mm_set1_ps(512.f);
	const __m128i UB = _mm_set1_epi16(0x3fe);

	__m128 x = _mm_mul_ps(in, drive);
	x = _mm_add_ps(_mm_mul_ps(x, m32), m512);

	__m128i e = _mm_cvtps_epi32(x);
	__m128 a = _mm_sub_ps(x, _mm_cvtepi32_ps(e));
	e = _mm_packs_epi32(e, e);
	e = _mm_max_epi16(_mm_min_epi16(e, UB), _mm_setzero_si128());

#if MAC
	// this should be very fast on C2D/C1D (and there are no macs with K8's)
	int e4 alignas(16)[4];
	e4[0] = _mm_cvtsi128_si32(e);
	e4[1] = _mm_cvtsi128_si32(_mm_shufflelo_epi16(e, _MM_SHUFFLE(1, 1, 1, 1)));
	e4[2] = _mm_cvtsi128_si32(_mm_shufflelo_epi16(e, _MM_SHUFFLE(2, 2, 2, 2)));
	e4[3] = _mm_cvtsi128_si32(_mm_shufflelo_epi16(e, _MM_SHUFFLE(3, 3, 3, 3)));

#else
	// on PC write to memory & back as XMM -> GPR is slow on K8
	short e4 alignas(16)[8];
	_mm_store_si128((__m128i*)&e4, e);
#endif

	__m128 ws1 = _mm_load_ss(&waveshapers[2][e4[0] & 0x3ff]);
	__m128 ws2 = _mm_load_ss(&waveshapers[2][e4[1] & 0x3ff]);
	__m128 ws3 = _mm_load_ss(&waveshapers[2][e4[2] & 0x3ff]);
	__m128 ws4 = _mm_load_ss(&waveshapers[2][e4[3] & 0x3ff]);
	__m128 ws = _mm_movelh_ps(_mm_unpacklo_ps(ws1, ws2), _mm_unpacklo_ps(ws3, ws4));
	ws1 = _mm_load_ss(&waveshapers[2][(e4[0] + 1) & 0x3ff]);
	ws2 = _mm_load_ss(&waveshapers[2][(e4[1] + 1) & 0x3ff]);
	ws3 = _mm_load_ss(&waveshapers[2][(e4[2] + 1) & 0x3ff]);
	ws4 = _mm_load_ss(&waveshapers[2][(e4[3] + 1) & 0x3ff]);
	__m128 wsn = _mm_movelh_ps(_mm_unpacklo_ps(ws1, ws2), _mm_unpacklo_ps(ws3, ws4));

	x = _mm_add_ps(_mm_mul_ps(_mm_sub_ps(one, a), ws), _mm_mul_ps(a, wsn));

	return x;
}

//==============================================================================

void CVASTWaveshaper::getStateInformation(MemoryBlock& destData)
{
	//std::unique_ptr<XmlElement> xml (parameters.valueTreeState.state.createXml());
	//copyXmlToBinary (*xml, destData);
}

void CVASTWaveshaper::setStateInformation(const void* data, int sizeInBytes)
{
	//std::unique_ptr<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
	//if (xmlState != nullptr)
	//  if (xmlState->hasTagName (parameters.valueTreeState.state.getType()))
	//    parameters.valueTreeState.state = ValueTree::fromXml (*xmlState);
}
