#include "VASTQFilter.h"
//#include "VASTQFilterProcess.h"
//#include "VASTQFilterStep.h"
#include "VASTQFilterCoefficients.h"
#include "../Utils/VASTSynthfunctions.h"
#include "../VASTSynthesiser.h"
#include "../VASTEngineHeader.h"
#include "../VASTSingleNote.h"
#include "../VASTVcf.h"
#ifdef __aarch64__ //arm64
	#include "../../sse2neon.h"
#else
	#include "emmintrin.h"
#endif

#define c_blocksize_q 32
#define c_max_os_factor_q 4
float waveshapers alignas(16)[8][1024];
float sinctable alignas(16)[(FIRipol_M + 1) * FIRipol_N * 2];
float sinctable1X alignas(16)[(FIRipol_M + 1) * FIRipol_N];
short sinctableI16 alignas(16)[(FIRipol_M + 1) * FIRipolI16_N];

JUCE_BEGIN_IGNORE_WARNINGS_GCC_LIKE("-Wconversion")
JUCE_BEGIN_IGNORE_WARNINGS_MSVC(4244 4267) //C4244 conversion from 'type1' to 'type2', possible loss of data //C4267 conversion

double VASTQFilter::shafted_tanh(double x)
{
	return (exp(x) - exp(-x * 1.2)) / (exp(x) + exp(-x));
}

VASTQFilter::VASTQFilter() {
	FBQ[0] = nullptr;
	FBQ[1] = nullptr;
	FBQ[2] = nullptr;
}

VASTQFilter::~VASTQFilter() {
#if defined _MACOSX || defined JUCE_LINUX
	if (FBQ[0]) //can be that it was not even started yet
		delete FBQ[0];
	if (FBQ[1])
		delete FBQ[1];
	if (FBQ[2])
		delete FBQ[2];
#else
	if (FBQ[0]) //can be that it was not even started yet
		_aligned_free(FBQ[0]);
	if (FBQ[1])
		_aligned_free(FBQ[1]);
	if (FBQ[2])
		_aligned_free(FBQ[2]);
#endif

}

void VASTQFilter::updateVariables() {
	QFirstRun = true;
	m_paramType[0] = -1;
	m_paramType[1] = -1;
	m_paramType[2] = -1;
}

void VASTQFilter::initQuadFilter(CVASTSettings* m_Set) {

	QFirstRun = true;

#if defined _MACOSX || defined JUCE_LINUX
	FBQ[0] =
		(VASTQFilterProcessState*)malloc((C_MAX_POLY >> 2) * sizeof(VASTQFilterProcessState));
	FBQ[1] =
		(VASTQFilterProcessState*)malloc((C_MAX_POLY >> 2) * sizeof(VASTQFilterProcessState));
	FBQ[2] =
		(VASTQFilterProcessState*)malloc((C_MAX_POLY >> 2) * sizeof(VASTQFilterProcessState));
#else
	FBQ[0] =
		(VASTQFilterProcessState*)_aligned_malloc((C_MAX_POLY >> 2) * sizeof(VASTQFilterProcessState), 16); //not portable
	FBQ[1] =
		(VASTQFilterProcessState*)_aligned_malloc((C_MAX_POLY >> 2) * sizeof(VASTQFilterProcessState), 16); //not portable
	FBQ[2] =
		(VASTQFilterProcessState*)_aligned_malloc((C_MAX_POLY >> 2) * sizeof(VASTQFilterProcessState), 16); //not portable
#endif
	/*
	memset(FBQ[0], 0, sizeof(&FBQ[0]));
	memset(FBQ[1], 0, sizeof(&FBQ[1]));
	memset(FBQ[2], 0, sizeof(&FBQ[2]));
	*/

	for (int i = 0; i < (C_MAX_POLY >> 2); ++i)
	{
		InitQFilterProcessStateToZero(&(FBQ[0][i]));
		InitQFilterProcessStateToZero(&(FBQ[1][i]));
		InitQFilterProcessStateToZero(&(FBQ[2][i]));
	}

	inBufferUp = std::make_unique<AudioSampleBuffer>(2, c_blocksize_q * c_max_os_factor_q);

	double mult = 1.0 / 32.0;
	for (int i = 0; i < 1024; i++)
	{
		double x = ((double)i - 512.0) * mult;
		waveshapers[0][i] = (float)tanh(x);                            // wst_tanh,
		waveshapers[1][i] = (float)pow(tanh(pow(::abs(x), 5.0)), 0.2); // wst_hard
		if (x < 0)
			waveshapers[1][i] = -waveshapers[1][i];
		waveshapers[2][i] = (float)shafted_tanh(x + 0.5) - shafted_tanh(0.5);       // wst_assym
		waveshapers[3][i] = (float)sin((double)((double)i - 512.0) * M_PI / 512.0); // wst_sinus
		waveshapers[4][i] = (float)tanh((double)((double)i - 512.0) * mult);        // wst_digi
	}

	float cutoff = 0.455f;
	float cutoff1X = 0.85f;
	float cutoffI16 = 1.0f;
	int j;
	for (j = 0; j < FIRipol_M + 1; j++)
	{
		for (int i = 0; i < FIRipol_N; i++)
		{
			double t = -double(i) + double(FIRipol_N / 2.0) + double(j) / double(FIRipol_M) - 1.0;
			double val = (float)(symmetric_blackman(t, FIRipol_N) * cutoff * sincf(cutoff * t));
			double val1X = (float)(symmetric_blackman(t, FIRipol_N) * cutoff1X * sincf(cutoff1X * t));
			sinctable[j * FIRipol_N * 2 + i] = (float)val;
			sinctable1X[j * FIRipol_N + i] = (float)val1X;
		}
	}
	for (j = 0; j < FIRipol_M; j++)
	{
		for (int i = 0; i < FIRipol_N; i++)
		{
			sinctable[j * FIRipol_N * 2 + FIRipol_N + i] =
				(float)((sinctable[(j + 1) * FIRipol_N * 2 + i] - sinctable[j * FIRipol_N * 2 + i]) /
					65536.0);
		}
	}

	for (j = 0; j < FIRipol_M + 1; j++)
	{
		for (int i = 0; i < FIRipolI16_N; i++)
		{
			double t = -double(i) + double(FIRipolI16_N / 2.0) + double(j) / double(FIRipol_M) - 1.0;
			double val =
				(float)(symmetric_blackman(t, FIRipolI16_N) * cutoffI16 * sincf(cutoffI16 * t));

			sinctableI16[j * FIRipolI16_N + i] = (short)((float)val * 16384.f);
		}
	}
	updateVariables();
}

void VASTQFilter::filterTypeChanged(OwnedArray<VASTSynthesiserVoice>* voices, int filter, int ftype, int fsubtype, int ftype2, int fsubtype2, int ftype3, int fsubtype3, int osFactor, bool isUI, std::unique_ptr<CVASTVcf>* uiVCF, CVASTSettings* m_Set) {
	if (!isUI) {
		for (auto* voice : *voices) {
			//check prepareforplay comb filter!!

			//Filter A
			if (ftype != fut_none) {
				memset(&((CVASTSingleNote*)voice)->m_VCF[filter]->FBP.FU[0], 0, sizeof(((CVASTSingleNote*)voice)->m_VCF[filter]->FBP.FU[0]));
				memset(&((CVASTSingleNote*)voice)->m_VCF[filter]->FBP.FU[1], 0, sizeof(((CVASTSingleNote*)voice)->m_VCF[filter]->FBP.FU[1]));
				((CVASTSingleNote*)voice)->m_VCF[filter]->FBP.FU[0].type = ftype;
				((CVASTSingleNote*)voice)->m_VCF[filter]->FBP.FU[0].subtype = fsubtype;
				((CVASTSingleNote*)voice)->m_VCF[filter]->FBP.FU[1].type = ftype;
				((CVASTSingleNote*)voice)->m_VCF[filter]->FBP.FU[1].subtype = fsubtype;
				((CVASTSingleNote*)voice)->m_VCF[filter]->m_filterCoefficientMaker[0].Reset();
				((CVASTSingleNote*)voice)->m_VCF[filter]->m_filterCoefficientMaker[0].prepareForPlay(m_Set->m_nSampleRate, osFactor, m_Set->m_fMasterTune.load());
			}
			//Filter B
			if (ftype2 != fut_none) {
				memset(&((CVASTSingleNote*)voice)->m_VCF[filter]->FBP.FU[2], 0, sizeof(((CVASTSingleNote*)voice)->m_VCF[filter]->FBP.FU[2]));
				memset(&((CVASTSingleNote*)voice)->m_VCF[filter]->FBP.FU[3], 0, sizeof(((CVASTSingleNote*)voice)->m_VCF[filter]->FBP.FU[3]));
				((CVASTSingleNote*)voice)->m_VCF[filter]->FBP.FU[2].type = ftype2;
				((CVASTSingleNote*)voice)->m_VCF[filter]->FBP.FU[2].subtype = fsubtype2;
				((CVASTSingleNote*)voice)->m_VCF[filter]->FBP.FU[3].type = ftype2;
				((CVASTSingleNote*)voice)->m_VCF[filter]->FBP.FU[3].subtype = fsubtype2;
				((CVASTSingleNote*)voice)->m_VCF[filter]->m_filterCoefficientMaker[1].Reset();
				((CVASTSingleNote*)voice)->m_VCF[filter]->m_filterCoefficientMaker[1].prepareForPlay(m_Set->m_nSampleRate, osFactor, m_Set->m_fMasterTune.load());
			}

			//Filter C
			if (ftype3 != fut_none) {
				memset(&((CVASTSingleNote*)voice)->m_VCF[filter]->FBP.FU[4], 0, sizeof(((CVASTSingleNote*)voice)->m_VCF[filter]->FBP.FU[4]));
				memset(&((CVASTSingleNote*)voice)->m_VCF[filter]->FBP.FU[5], 0, sizeof(((CVASTSingleNote*)voice)->m_VCF[filter]->FBP.FU[5]));
				((CVASTSingleNote*)voice)->m_VCF[filter]->FBP.FU[4].type = ftype3;
				((CVASTSingleNote*)voice)->m_VCF[filter]->FBP.FU[4].subtype = fsubtype3;
				((CVASTSingleNote*)voice)->m_VCF[filter]->FBP.FU[5].type = ftype3;
				((CVASTSingleNote*)voice)->m_VCF[filter]->FBP.FU[5].subtype = fsubtype3;
				((CVASTSingleNote*)voice)->m_VCF[filter]->m_filterCoefficientMaker[2].Reset();
				((CVASTSingleNote*)voice)->m_VCF[filter]->m_filterCoefficientMaker[2].prepareForPlay(m_Set->m_nSampleRate, osFactor, m_Set->m_fMasterTune.load());
			}

			if (ftype == fut_vastscream)
				((CVASTSingleNote*)voice)->m_VCF[filter]->m_ladderFilter1.setMode(dsp::LadderFilter<float>::Mode::LPF12);
		}
	}
	else { //UI!
		//Filter A
		if (ftype != fut_none) {
			memset(&uiVCF[filter]->FBP.FU[0], 0, sizeof((uiVCF[filter]->FBP.FU[0])));
			memset(&uiVCF[filter]->FBP.FU[1], 0, sizeof((uiVCF[filter]->FBP.FU[1])));
			uiVCF[filter]->FBP.FU[0].type = ftype;
			uiVCF[filter]->FBP.FU[0].subtype = fsubtype;
			uiVCF[filter]->FBP.FU[1].type = ftype;
			uiVCF[filter]->FBP.FU[1].subtype = fsubtype;
			uiVCF[filter]->m_filterCoefficientMaker[0].Reset();
			uiVCF[filter]->m_filterCoefficientMaker[0].prepareForPlay(m_Set->m_nSampleRate, osFactor, m_Set->m_fMasterTune.load());
		}

		//Filter B
		if (ftype2 != fut_none) {
			memset(&uiVCF[filter]->FBP.FU[2], 0, sizeof((uiVCF[filter]->FBP.FU[2])));
			memset(&uiVCF[filter]->FBP.FU[3], 0, sizeof((uiVCF[filter]->FBP.FU[3])));
			uiVCF[filter]->FBP.FU[2].type = ftype2;
			uiVCF[filter]->FBP.FU[2].subtype = fsubtype2;
			uiVCF[filter]->FBP.FU[3].type = ftype2;
			uiVCF[filter]->FBP.FU[3].subtype = fsubtype2;
			uiVCF[filter]->m_filterCoefficientMaker[1].Reset();
			uiVCF[filter]->m_filterCoefficientMaker[1].prepareForPlay(m_Set->m_nSampleRate, osFactor, m_Set->m_fMasterTune.load());
		}
		//Filter C
		if (ftype3 != fut_none) {
			memset(&uiVCF[filter]->FBP.FU[4], 0, sizeof((uiVCF[filter]->FBP.FU[4])));
			memset(&uiVCF[filter]->FBP.FU[5], 0, sizeof((uiVCF[filter]->FBP.FU[5])));
			uiVCF[filter]->FBP.FU[4].type = ftype2;
			uiVCF[filter]->FBP.FU[4].subtype = fsubtype2;
			uiVCF[filter]->FBP.FU[5].type = ftype2;
			uiVCF[filter]->FBP.FU[5].subtype = fsubtype2;
			uiVCF[filter]->m_filterCoefficientMaker[2].Reset();
			uiVCF[filter]->m_filterCoefficientMaker[2].prepareForPlay(m_Set->m_nSampleRate, osFactor, m_Set->m_fMasterTune.load());
		}

		if (ftype == fut_vastscream)
			uiVCF[filter]->m_ladderFilter1.setMode(dsp::LadderFilter<float>::Mode::LPF12);
	}
    m_bInitFilterAfterMaxPolyChange[filter].store(false);
}

int VASTQFilter::processBlock(OwnedArray<VASTSynthesiserVoice>* voices, modMatrixInputState* matrixInputState, sRoutingBuffers* routingBuffers, CVASTSettings* m_Set, int filter, dsp::AudioBlock<float> filterBlock, int startSample, int numSamples, bool isUI, bool hasNextFilter, std::unique_ptr<CVASTVcf>* uiVCF, bool warmup) {

	float fVCFEnvelopeMod = 0.f; //CHECK
	int paramType = 0;
	int paramFilterEnv = 0;
	std::atomic<float> paramFilterEnvMod = 0.f;
	int paramIDFilterEnvMod = 0;
	std::atomic<float> paramDrive = 0.f;
	int paramIDDrive = 0;
	std::atomic<float> paramCutoff = 0.f;
	int paramIDCutoff = 0;
	std::atomic<float> paramResonance = 0.f;
	int paramIDResonance = 0;
	std::atomic<float> paramScale = 0.f;
	int paramIDScale = 0;
	std::atomic<float> paramGain = 0.f;
	int paramIDGain = 0;

	switch (filter) {
	case(0):
		paramType = static_cast<int>(*m_Set->m_State->m_uFilterType_Filter1);
		paramDrive.store(*m_Set->m_State->m_fFilterDrive_Filter1);
		paramIDDrive = MODMATDEST::Filter1Drive;
		paramFilterEnv = *m_Set->m_State->m_uVCFEnv_Filter1;
		paramFilterEnvMod.store(*m_Set->m_State->m_fVCFEnvMod_Filter1);
		paramIDFilterEnvMod = MODMATDEST::Filter1EnvMod;
		paramCutoff.store(*m_Set->m_State->m_fFilterCutoff_Filter1);
		paramIDCutoff = MODMATDEST::Filter1Cutoff;
		paramResonance.store(*m_Set->m_State->m_fFilterReso_Filter1);
		paramIDResonance = MODMATDEST::Filter1Resonance;
		paramScale.store(*m_Set->m_State->m_fFilterScale_Filter1);
		paramIDScale = MODMATDEST::Filter1Scale;
		paramGain.store(*m_Set->m_State->m_fFilterGain_Filter1);
		paramIDGain = MODMATDEST::Filter1Gain;
		break;
	case(1):
		paramType = static_cast<int>(*m_Set->m_State->m_uFilterType_Filter2);
		paramDrive.store(*m_Set->m_State->m_fFilterDrive_Filter2);
		paramIDDrive = MODMATDEST::Filter2Drive;
		paramFilterEnv = *m_Set->m_State->m_uVCFEnv_Filter2;
		paramFilterEnvMod.store(*m_Set->m_State->m_fVCFEnvMod_Filter2);
		paramIDFilterEnvMod = MODMATDEST::Filter2EnvMod;
		paramCutoff.store(*m_Set->m_State->m_fFilterCutoff_Filter2);
		paramIDCutoff = MODMATDEST::Filter2Cutoff;
		paramResonance.store(*m_Set->m_State->m_fFilterReso_Filter2);
		paramIDResonance = MODMATDEST::Filter2Resonance;
		paramScale.store(*m_Set->m_State->m_fFilterScale_Filter2);
		paramIDScale = MODMATDEST::Filter2Scale;
		paramGain.store(*m_Set->m_State->m_fFilterGain_Filter2);
		paramIDGain = MODMATDEST::Filter2Gain;
		break;
	case(2):
		paramType = static_cast<int>(*m_Set->m_State->m_uFilterType_Filter3);
		paramDrive.store(*m_Set->m_State->m_fFilterDrive_Filter3);
		paramIDDrive = MODMATDEST::Filter3Drive;
		paramFilterEnv = *m_Set->m_State->m_uVCFEnv_Filter3;
		paramFilterEnvMod.store(*m_Set->m_State->m_fVCFEnvMod_Filter3);
		paramIDFilterEnvMod = MODMATDEST::Filter3EnvMod;
		paramCutoff.store(*m_Set->m_State->m_fFilterCutoff_Filter3);
		paramIDCutoff = MODMATDEST::Filter3Cutoff;
		paramResonance.store(*m_Set->m_State->m_fFilterReso_Filter3);
		paramIDResonance = MODMATDEST::Filter3Resonance;
		paramScale.store(*m_Set->m_State->m_fFilterScale_Filter3);
		paramIDScale = MODMATDEST::Filter3Scale;
		paramGain.store(*m_Set->m_State->m_fFilterGain_Filter3);
		paramIDGain = MODMATDEST::Filter3Gain;
		break;
	}

	int i = 0;
	int samplesToProcess = numSamples;

	int ftype = fut_lp12;
	int fsubtype = st_SVF;
	int ftype2 = fut_none;
	int fsubtype2 = fut_none;
	int ftype3 = fut_none;
	int fsubtype3 = fut_none;
	int fws = wst_asym;	//wst_digi does not work
	int fOsFactor = 1;
	bool fParameterSmoothing = true; 
	float fAttenuateFactor = 1.f;
	
	filterGetVariables(paramType, ftype, fsubtype, ftype2, fsubtype2, ftype3, fsubtype3, fws, fOsFactor, fAttenuateFactor, fParameterSmoothing);

	bool bTypeChanged = (paramType != m_paramType[filter]);
	m_paramType[filter] = paramType;
	if ((bTypeChanged) || (warmup || m_bInitFilterAfterMaxPolyChange[filter].load())) {
		filterTypeChanged(voices, filter, ftype, fsubtype, ftype2, fsubtype2, ftype3, fsubtype3, fOsFactor, isUI, uiVCF, m_Set);
	}
    int numVoices = 0;
	    
	//while (samplesToProcess >= c_blocksize_q) {
	while (samplesToProcess > 0) {
		matrixInputState->currentFrame = static_cast<uint64_t>(startSample) + i;

		int curNumSamples = (samplesToProcess >= c_blocksize_q) ? c_blocksize_q : samplesToProcess;

		CVASTVcf* vcf;
		bool complete = false;
		int voiceNo = -1;
		VASTSynthesiserVoice* voice;

		while (!complete)
		{
			bool doit = true;
			if (isUI) {
				vcf = uiVCF[filter].get();
				complete = true;
			}
			else {
				voiceNo++;
				voice = (*voices)[voiceNo];
				vcf = ((CVASTSingleNote*)voice)->m_VCF[filter];
				bool bPlayiningInRange = ((CVASTSingleNote*)voice)->isPlayingInRange(startSample, numSamples); //requires that mseg is processed before!
				doit = (bPlayiningInRange || (((CVASTSingleNote*)voice)->m_bLastFilterOutputZero[filter] == false));
				if (voiceNo == m_Set->m_uMaxPoly - 1)
					complete = true;
			}
			if (doit) {
				matrixInputState->voiceNo = voiceNo;
                float lFilterEnvMod{};
				if (isUI) {
					fVCFEnvelopeMod = 1.0f;
				} 
				else {
					{
						if (paramFilterEnv == static_cast<int>(MSEGENV::MSEG1)) {
							fVCFEnvelopeMod = routingBuffers->MSEGBuffer[0][voiceNo]->getSample(0, int(matrixInputState->currentFrame));
						}
						else if (paramFilterEnv == static_cast<int>(MSEGENV::MSEG2)) {
							fVCFEnvelopeMod = routingBuffers->MSEGBuffer[1][voiceNo]->getSample(0, int(matrixInputState->currentFrame));
						}
						else if (paramFilterEnv == static_cast<int>(MSEGENV::MSEG3)) {
							fVCFEnvelopeMod = routingBuffers->MSEGBuffer[2][voiceNo]->getSample(0, int(matrixInputState->currentFrame));
						}
						else if (paramFilterEnv == static_cast<int>(MSEGENV::MSEG4)) {
							fVCFEnvelopeMod = routingBuffers->MSEGBuffer[3][voiceNo]->getSample(0, int(matrixInputState->currentFrame));
						}
						else if (paramFilterEnv == static_cast<int>(MSEGENV::MSEG5)) {
							fVCFEnvelopeMod = routingBuffers->MSEGBuffer[4][voiceNo]->getSample(0, int(matrixInputState->currentFrame));
						}
						lFilterEnvMod = m_Set->getParameterValueWithMatrixModulation(&paramFilterEnvMod, paramIDFilterEnvMod, matrixInputState);
					}
					if (lFilterEnvMod != 0.0) {
						if (lFilterEnvMod > 0.0) {
							fVCFEnvelopeMod = (100 - lFilterEnvMod) * 0.01f + lFilterEnvMod * 0.01f * fVCFEnvelopeMod;
						}
						else {
							fVCFEnvelopeMod = 1.0 + fVCFEnvelopeMod * (lFilterEnvMod / 100.0); // 0.00 .. 1..0
						}
					}
					else {
						fVCFEnvelopeMod = 1.0f;
					}
				}				

				double cutoff1 = paramCutoff;
				double resonance1 = paramResonance;
				double scale1 = paramScale;
				if (!isUI) {
					cutoff1 = m_Set->getParameterValueWithMatrixModulation(&paramCutoff, paramIDCutoff, matrixInputState);
					resonance1 = m_Set->getParameterValueWithMatrixModulation(&paramResonance, paramIDResonance, matrixInputState);
					scale1 = m_Set->getParameterValueWithMatrixModulation(&paramScale, paramIDScale, matrixInputState);
				}
				double cutoff2 = cutoff1;
				double resonance2 = resonance1;
				double scale2 = scale1;
				double cutoff3 = cutoff1;
				double resonance3 = resonance1;
				double scale3 = scale1;
				if ((fParameterSmoothing) && (!isUI)) {
					if (vcf->m_bSmoothersTakeNextValue) {
						vcf->m_fCutoff_smoothed.setCurrentAndTargetValue(cutoff1);
						vcf->m_fResonance_smoothed.setCurrentAndTargetValue(resonance1);
						vcf->m_fScale_smoothed.setCurrentAndTargetValue(scale1);
					}
					else {
						vcf->m_fCutoff_smoothed.setTargetValue(cutoff1);
						cutoff1 = vcf->m_fCutoff_smoothed.getNextValue();
						vcf->m_fResonance_smoothed.setTargetValue(resonance1);
						resonance1 = vcf->m_fResonance_smoothed.getNextValue();
						vcf->m_fScale_smoothed.setTargetValue(scale1);
						scale1 = vcf->m_fScale_smoothed.getNextValue();
						cutoff2 = cutoff1;
						cutoff3 = cutoff1;
						resonance2 = resonance1;
						resonance3 = resonance1;
						scale2 = scale1;
						scale3 = scale1;
					}
					vcf->m_fCutoff_smoothed.skip(c_blocksize_q - 1);
					vcf->m_fResonance_smoothed.skip(c_blocksize_q - 1);
					vcf->m_fScale_smoothed.skip(c_blocksize_q - 1);
				}

				calcFilterCoefficients(isUI, ftype, fsubtype, ftype2, fsubtype2, ftype3, fsubtype3, fOsFactor, fVCFEnvelopeMod, cutoff1, resonance1, scale1, cutoff2, resonance2, scale2, cutoff3, resonance3, scale3, m_Set->m_nSampleRate, &vcf->m_filterCoefficientMaker[0], &vcf->m_filterCoefficientMaker[1]);

				if ((ftype == fut_vastcomb) || (ftype == fut_vastscream)) { //legacy
					vcf->m_combFilter.parameterChanged("m_fCombFrequOffset", cutoff1, vcf->m_bSmoothersTakeNextValue, (c_blocksize_q * fOsFactor * 4) - 1); //50..18000
					vcf->m_combFilter.parameterChanged("m_fCombLevel", scale1, vcf->m_bSmoothersTakeNextValue, (c_blocksize_q * fOsFactor * 4) - 1); //70 .. 100 
					if (ftype == fut_vastscream) {
						vcf->m_ladderFilter1.setCutoffFrequencyHz(cutoff2); //is smoothed
						vcf->m_ladderFilter1.setResonance(resonance2); //is smoothed
					}
				}
				else if (ftype == fut_vastkrg12) { //legacy
					vcf->m_TFHPF_dFc = cutoff1;
					vcf->m_TFHPF_dK = resonance1; //should be (0..2.f)
					vcf->m_TFHPF_dSaturation = scale1; //should be 1.0f..1.5f  - only used for for NLP
				}
				else {
					vcf->m_filterCoefficientMaker[0].MakeCoeffs(cutoff1, resonance1, ftype, fsubtype, scale1); //Filter A
					if (ftype2 != fut_none)
						vcf->m_filterCoefficientMaker[1].MakeCoeffs(cutoff2, resonance2, ftype2, fsubtype2, scale2); //Filter B
					if (ftype3 != fut_none)
						vcf->m_filterCoefficientMaker[2].MakeCoeffs(cutoff3, resonance3, ftype3, fsubtype3, scale3); //Filter C
				}
			}
			//vcf->m_bSmoothersTakeNextValue = false;
		}
		
		double feedback = paramScale;
		double gain = paramGain;  
		double drive = paramDrive;
		if (!isUI) {
			feedback = m_Set->getParameterValueWithMatrixModulation(&paramScale, paramIDScale, matrixInputState);
			gain = m_Set->getParameterValueWithMatrixModulation(&paramGain, paramIDGain, matrixInputState);
			drive = m_Set->getParameterValueWithMatrixModulation(&paramDrive, paramIDDrive, matrixInputState);
		}

		calcFilterSettings(isUI, ftype, fsubtype, ftype2, fsubtype2, ftype3, fsubtype3, fOsFactor, drive, feedback, gain);

		numVoices = processQFilter(filterBlock.getSubBlock(i, curNumSamples), routingBuffers, curNumSamples, startSample + i, ftype, fsubtype, ftype2, fsubtype2, ftype3, fsubtype3, fws, fOsFactor, voices, filter, fAttenuateFactor * gain, drive, feedback, isUI, hasNextFilter, uiVCF, warmup);
		m_lastOsFactor = fOsFactor;

		samplesToProcess -= c_blocksize_q;
		i += c_blocksize_q;

		if (!isUI) {
			for (auto* voice : *voices) {
				vcf = ((CVASTSingleNote*)voice)->m_VCF[filter];
				vcf->m_bSmoothersTakeNextValue = false;
			}
		}
	}
    
    return numVoices;
}

void VASTQFilter::filterGetVariables(int paramType, int &ftype, int &fsubtype, int &ftype2, int &fsubtype2, int &ftype3, int &fsubtype3, int &fws, int &fOsFactor, float &fAttenuateFactor, bool &fParameterSmoothing) {
	//*******************************************************************
	//DQF Filters
	//*******************************************************************
	switch (paramType) {
	case (FILTERTYPE::DQFLPF12):
			ftype = fut_lp12;
			fsubtype = st_SVF;		
			fws = wst_tanh;
			fOsFactor = 2; 
			fParameterSmoothing = false; 
			fAttenuateFactor = 1.f;
			break;
		
		case (FILTERTYPE::DQFLPF24):
			ftype = fut_lp24;
			fsubtype = st_SVF;
			fws = wst_tanh;
			fOsFactor = 2;
			fParameterSmoothing = false; 
			fAttenuateFactor = 1.f;
			break;
		
		case (FILTERTYPE::DQFBRF12):
			ftype = fut_br12;
			fws = wst_tanh;
			fOsFactor = 1;
			fParameterSmoothing = false; 
			fAttenuateFactor = 0.5f;
			break;
		
		case (FILTERTYPE::DQFBPF12):
			ftype = fut_bp12;
			fsubtype = st_SVF;
			fws = wst_tanh;
			fOsFactor = 1;
			fParameterSmoothing = false; 
			fAttenuateFactor = 1.0;
			break;
		
		case (FILTERTYPE::DQFHPF12):
			ftype = fut_hp12;
			fsubtype = st_SVF;
			fws = wst_tanh;
			fOsFactor = 2;
			fParameterSmoothing = false; 
			fAttenuateFactor = 1.f;
			break;
		
		case (FILTERTYPE::DQFLDF6):
			ftype = fut_lpmoog;
			fsubtype = st_moog6;
			fws = wst_tanh;
			fOsFactor = 2;
			fParameterSmoothing = false;
			fAttenuateFactor = 1.f;
			break;

		case (FILTERTYPE::DQFLDF12):
			ftype = fut_lpmoog;
			fsubtype = st_moog12;
			fws = wst_tanh;
			fOsFactor = 2;
			fParameterSmoothing = false; 
			fAttenuateFactor = 1.f;
			break;

		case (FILTERTYPE::DQFLDF18):
			ftype = fut_lpmoog;
			fsubtype = st_moog18;
			fws = wst_tanh;
			fOsFactor = 2;
			fParameterSmoothing = false;
			fAttenuateFactor = 1.f;
			break;

		case (FILTERTYPE::DQFLDF24):
			ftype = fut_lpmoog;
			fsubtype = st_moog24;
			fws = wst_tanh;
			fOsFactor = 2;
			fParameterSmoothing = false;
			fAttenuateFactor = 1.f;
			break;

		case (FILTERTYPE::DQFCOMB):
			ftype = fut_comb;
			fws = wst_tanh;
			fOsFactor = 2;
			fParameterSmoothing = false; 
			fAttenuateFactor = 1.f;
			break;
		
		case (FILTERTYPE::DQFLPF12WSSINE):
			ftype = fut_lp12;
			fsubtype = st_SVF;
			fws = wst_sinus;
			fOsFactor = 2;
			fParameterSmoothing = false; 
			fAttenuateFactor = 1.f;
			break;
		
		case (FILTERTYPE::DQFLPF24WSSINE):
			ftype = fut_lp24;
			fsubtype = st_SVF;
			fws = wst_sinus;
			fOsFactor = 2;
			fParameterSmoothing = false; 
			fAttenuateFactor = 1.f;
			break;
		
		case (FILTERTYPE::DQFBRF12WSSINE):
			ftype = fut_br12;
			fsubtype = st_SVF;
			fws = wst_sinus;
			fOsFactor = 1;
			fParameterSmoothing = false; 
			fAttenuateFactor = 0.5f;
			break;
		
		case (FILTERTYPE::DQFBPF12WSSINE):
			ftype = fut_bp12;
			fsubtype = st_SVF;
			fws = wst_sinus;
			fOsFactor = 1;
			fParameterSmoothing = false; 
			fAttenuateFactor = 1.0f;
			break;
		
		case (FILTERTYPE::DQFHPF12WSSINE):
			ftype = fut_hp12;
			fsubtype = st_SVF;
			fws = wst_sinus;
			fOsFactor = 2;
			fParameterSmoothing = false; 
			fAttenuateFactor = 1.f;
			break;
		
		case (FILTERTYPE::DQFLDF12WSSINE):
			ftype = fut_lpmoog;
			fsubtype = st_moog12;
			fws = wst_sinus;
			fOsFactor = 2;
			fParameterSmoothing = false; 
			fAttenuateFactor = 1.f;
			break;
		
		case (FILTERTYPE::DQFCOMBWSSINE):
			ftype = fut_comb;
			fws = wst_sinus;
			fOsFactor = 2;
			fParameterSmoothing = false; 
			fAttenuateFactor = 1.f;
			break;
		

	//*******************************************************************
	//BQF Filters
	//*******************************************************************
		case (FILTERTYPE::BQFLPF6):
			ftype = fut_vastiir6;
			fsubtype = st_iir_lp;
			fws = wst_tanh;
			fOsFactor = 4;
			fParameterSmoothing = true; 
			fAttenuateFactor = 1.f;
			break;
		
		case (FILTERTYPE::BQFHPF6):
			ftype = fut_vastiir6;
			fsubtype = st_iir_hp;
			fws = wst_tanh;
			fOsFactor = 1;
			fParameterSmoothing = true; 
			fAttenuateFactor = 1.f;
			break;
		
		case (FILTERTYPE::BQFBPF6):
			ftype = fut_vastiir6;
			fsubtype = st_iir_bp;
			fws = wst_tanh;
			fOsFactor = 1;
			fParameterSmoothing = true; 
			fAttenuateFactor = 1.f;
			break;
		
		case (FILTERTYPE::BQFALLPASS6):
			ftype = fut_vastiir6;
			fsubtype = st_iir_ap;
			fws = wst_tanh;
			fOsFactor = 1;
			fParameterSmoothing = true; 
			fAttenuateFactor = 1.f;
			break;
		
		case (FILTERTYPE::BQFLOWSHELF6):
			ftype = fut_vastiir6;
			fsubtype = st_iir_ls;
			fws = wst_tanh;
			fOsFactor = 1;
			fParameterSmoothing = true; 
			fAttenuateFactor = 1.f;
			break;
		
		case (FILTERTYPE::BQFHIGHSHELF6):
			ftype = fut_vastiir6;
			fsubtype = st_iir_hs;
			fws = wst_tanh;
			fOsFactor = 1;
			fParameterSmoothing = true; 
			fAttenuateFactor = 1.f;
			break;
		
		case (FILTERTYPE::BQFNOTCH6):
			ftype = fut_vastiir6;
			fsubtype = st_iir_no;
			fws = wst_tanh;
			fOsFactor = 1;
			fParameterSmoothing = true; 
			fAttenuateFactor = 1.f;
			break;
		
		case (FILTERTYPE::BQFPEAK6):
			ftype = fut_vastiir6;
			fsubtype = st_iir_pk;
			fws = wst_tanh;
			fOsFactor = 1;
			fParameterSmoothing = true; 
			fAttenuateFactor = 1.f;
			break;
		
		case (FILTERTYPE::BQFLPF12):
			ftype = fut_vastiir6;
			fsubtype = st_iir_lp;
			ftype2 = fut_vastiir6;
			fsubtype2 = st_iir_lp;
			fws = wst_tanh;
			fOsFactor = 4;
			fParameterSmoothing = true; 
			fAttenuateFactor = 1.f;
			break;
		
		case (FILTERTYPE::BQFHPF12):
			ftype = fut_vastiir6;
			fsubtype = st_iir_hp;
			ftype2 = fut_vastiir6;
			fsubtype2 = st_iir_hp;
			fws = wst_tanh;
			fOsFactor = 1;
			fParameterSmoothing = true; 
			fAttenuateFactor = 1.f;
			break;
		
		case (FILTERTYPE::BQFBPF12):
			ftype = fut_vastiir6;
			fsubtype = st_iir_bp;
			ftype2 = fut_vastiir6;
			fsubtype2 = st_iir_bp;
			fws = wst_tanh;
			fOsFactor = 1;
			fParameterSmoothing = true; 
			fAttenuateFactor = 1.f;
			break;
		
		case (FILTERTYPE::BQFALLPASS12):
			ftype = fut_vastiir6;
			fsubtype = st_iir_ap;
			ftype2 = fut_vastiir6;
			fsubtype2 = st_iir_ap;
			fws = wst_tanh;
			fOsFactor = 1;
			fParameterSmoothing = true; 
			fAttenuateFactor = 1.f;
			break;
		
		case (FILTERTYPE::BQFLOWSHELF12):
			ftype = fut_vastiir6;
			fsubtype = st_iir_ls;
			ftype2 = fut_vastiir6;
			fsubtype2 = st_iir_ls;
			fws = wst_tanh;
			fOsFactor = 1;
			fParameterSmoothing = true; 
			fAttenuateFactor = 1.f;
			break;
		
		case (FILTERTYPE::BQFHIGHSHELF12):
			ftype = fut_vastiir6;
			fsubtype = st_iir_hs;
			ftype2 = fut_vastiir6;
			fsubtype2 = st_iir_hs;
			fws = wst_tanh;
			fOsFactor = 1;
			fParameterSmoothing = true; 
			fAttenuateFactor = 1.f;
			break;
		
		case (FILTERTYPE::BQFNOTCH12):
			ftype = fut_vastiir6;
			fsubtype = st_iir_no;
			ftype2 = fut_vastiir6;
			fsubtype2 = st_iir_no;
			fws = wst_tanh;
			fOsFactor = 1;
			fParameterSmoothing = true; 
			fAttenuateFactor = 1.f;
			break;
		
		case (FILTERTYPE::BQFPEAK12):
			ftype = fut_vastiir6;
			fsubtype = st_iir_pk;
			ftype2 = fut_vastiir6;
			fsubtype2 = st_iir_pk;
			fws = wst_tanh;
			fOsFactor = 1;
			fParameterSmoothing = true; 
			fAttenuateFactor = 1.f;
			break;
		
		case (FILTERTYPE::BQFLPF18):
			ftype = fut_vastiir6;
			fsubtype = st_iir_lp;
			ftype2 = fut_vastiir6;
			fsubtype2 = st_iir_lp;
			ftype3 = fut_vastiir6;
			fsubtype3 = st_iir_lp;
			fws = wst_tanh;
			fOsFactor = 4;
			fParameterSmoothing = true; 
			fAttenuateFactor = 1.f;
			break;

		case (FILTERTYPE::BQFHPF18):
			ftype = fut_vastiir6;
			fsubtype = st_iir_hp;
			ftype2 = fut_vastiir6;
			fsubtype2 = st_iir_hp;
			ftype3 = fut_vastiir6;
			fsubtype3 = st_iir_hp;
			fws = wst_tanh;
			fOsFactor = 1;
			fParameterSmoothing = true;
			fAttenuateFactor = 1.f;
			break;

		case (FILTERTYPE::BQFBPF18):
			ftype = fut_vastiir6;
			fsubtype = st_iir_bp;
			ftype2 = fut_vastiir6;
			fsubtype2 = st_iir_bp;
			ftype3 = fut_vastiir6;
			fsubtype3 = st_iir_bp;
			fws = wst_tanh;
			fOsFactor = 1;
			fParameterSmoothing = true;
			fAttenuateFactor = 1.f;
			break;
		
		case (FILTERTYPE::BQFALLPASS18):
			ftype = fut_vastiir6;
			fsubtype = st_iir_ap;
			ftype2 = fut_vastiir6;
			fsubtype2 = st_iir_ap;
			ftype3 = fut_vastiir6;
			fsubtype3 = st_iir_ap;
			fws = wst_tanh;
			fOsFactor = 1;
			fParameterSmoothing = true;
			fAttenuateFactor = 1.f;
			break;

		case (FILTERTYPE::BQFLOWSHELF18):
			ftype = fut_vastiir6;
			fsubtype = st_iir_ls;
			ftype2 = fut_vastiir6;
			fsubtype2 = st_iir_ls;
			ftype3 = fut_vastiir6;
			fsubtype3 = st_iir_ls;
			fws = wst_tanh;
			fOsFactor = 1;
			fParameterSmoothing = true;
			fAttenuateFactor = 1.f;
			break;

		case (FILTERTYPE::BQFHIGHSHELF18):
			ftype = fut_vastiir6;
			fsubtype = st_iir_hs;
			ftype2 = fut_vastiir6;
			fsubtype2 = st_iir_hs;
			ftype3 = fut_vastiir6;
			fsubtype3 = st_iir_hs;
			fws = wst_tanh;
			fOsFactor = 1;
			fParameterSmoothing = true;
			fAttenuateFactor = 1.f;
			break;

		case (FILTERTYPE::BQFNOTCH18):
			ftype = fut_vastiir6;
			fsubtype = st_iir_no;
			ftype2 = fut_vastiir6;
			fsubtype2 = st_iir_no;
			ftype3 = fut_vastiir6;
			fsubtype3 = st_iir_no;
			fws = wst_tanh;
			fOsFactor = 1;
			fParameterSmoothing = true;
			fAttenuateFactor = 1.f;
			break;

		case (FILTERTYPE::BQFPEAK18):
			ftype = fut_vastiir6;
			fsubtype = st_iir_pk;
			ftype2 = fut_vastiir6;
			fsubtype2 = st_iir_pk;
			ftype3 = fut_vastiir6;
			fsubtype3 = st_iir_pk;
			fws = wst_tanh;
			fOsFactor = 1;
			fParameterSmoothing = true;
			fAttenuateFactor = 1.f;
			break;

	//*******************************************************************
	//SVF Filters
	//*******************************************************************
		case (FILTERTYPE::SVFLPF12):
			ftype = fut_vastsvf;
			fsubtype = st_vastsvf_lp;
			fws = wst_tanh;
			fOsFactor = 4;
			fParameterSmoothing = true; 
			fAttenuateFactor = 1.f;
			break;
		
		case (FILTERTYPE::SVFHPF12):
			ftype = fut_vastsvf;
			fsubtype = st_vastsvf_hp;
			fws = wst_tanh;
			fOsFactor = 1;
			fParameterSmoothing = true; 
			fAttenuateFactor = 1.f;
			break;
		
		case (FILTERTYPE::SVFBPF12):
			ftype = fut_vastsvf;
			fsubtype = st_vastsvf_bp;
			fws = wst_tanh;
			fOsFactor = 1;
			fParameterSmoothing = true; 
			fAttenuateFactor = 1.4f;
			break;
		
		case (FILTERTYPE::SVFLPF24):
			ftype = fut_vastsvf;
			fsubtype = st_vastsvf_lp;
			ftype2 = fut_vastsvf;
			fsubtype2 = st_vastsvf_lp;
			fws = wst_tanh;
			fOsFactor = 4;
			fParameterSmoothing = true; 
			fAttenuateFactor = 1.0f;
			break;
		
		case (FILTERTYPE::SVFHPF24):
			ftype = fut_vastsvf;
			fsubtype = st_vastsvf_hp;
			ftype2 = fut_vastsvf;
			fsubtype2 = st_vastsvf_hp;
			fws = wst_tanh;
			fOsFactor = 1;
			fParameterSmoothing = true; 
			fAttenuateFactor = 1.f;
			break;
		
		case (FILTERTYPE::SVFBPF24):
			ftype = fut_vastsvf;
			fsubtype = st_vastsvf_bp;
			ftype2 = fut_vastsvf;
			fsubtype2 = st_vastsvf_bp;
			fws = wst_tanh;
			fOsFactor = 1;
			fParameterSmoothing = true; 
			fAttenuateFactor = 4.7f;
			break;
		
		case (FILTERTYPE::SVFLPF36): 
			ftype = fut_vastsvf;
			fsubtype = st_vastsvf_lp;
			ftype2 = fut_vastsvf;
			fsubtype2 = st_vastsvf_lp;
			ftype3 = fut_vastsvf;
			fsubtype3 = st_vastsvf_lp;
			fws = wst_tanh;
			fOsFactor = 4;
			fParameterSmoothing = true; 
			fAttenuateFactor = 1.0f;
			break;
		
		case (FILTERTYPE::SVFHPF36): 
			ftype = fut_vastsvf;
			fsubtype = st_vastsvf_hp;
			ftype2 = fut_vastsvf;
			fsubtype2 = st_vastsvf_hp;
			ftype3 = fut_vastsvf;
			fsubtype3 = st_vastsvf_hp;
			fws = wst_tanh;
			fOsFactor = 1;
			fParameterSmoothing = true; 
			fAttenuateFactor = 1.f;
			break;
	
		case (FILTERTYPE::SVFBPF36): 
			ftype = fut_vastsvf;
			fsubtype = st_vastsvf_bp;
			ftype2 = fut_vastsvf;
			fsubtype2 = st_vastsvf_bp;
			ftype3 = fut_vastsvf; 
			fsubtype3 = st_vastsvf_bp;
			fws = wst_tanh;
			fOsFactor = 1;
			fParameterSmoothing = true; 
			fAttenuateFactor = 16.f;
			break;
		
	//*******************************************************************
	//LDF Filters
	//*******************************************************************
		case (FILTERTYPE::LDFLPF12):
			ftype = fut_vastldf;
			fsubtype = st_vastldf_LPF12;
			fws = wst_tanh;
			fOsFactor = 4;
			fParameterSmoothing = true; 
			fAttenuateFactor = 1.18f;
			break;
		
		case (FILTERTYPE::LDFHPF12):
			ftype = fut_vastldf;
			fsubtype = st_vastldf_HPF12;
			fws = wst_tanh;
			fOsFactor = 1;
			fParameterSmoothing = true; 
			fAttenuateFactor = 1.f;
			break;
		
		case (FILTERTYPE::LDFLPF24):
			ftype = fut_vastldf;
			fsubtype = st_vastldf_LPF24;
			fws = wst_tanh;
			fOsFactor = 4;
			fParameterSmoothing = true; 
			fAttenuateFactor = 1.18f;
			break;
		
		case (FILTERTYPE::LDFHPF24):
			ftype = fut_vastldf;
			fsubtype = st_vastldf_HPF24;
			fws = wst_tanh;
			fOsFactor = 1;
			fParameterSmoothing = true; 
			fAttenuateFactor = 1.f;
			break;
		
		case (FILTERTYPE::LDFLPF36):
			ftype = fut_vastldf;
			fsubtype = st_vastldf_LPF24;
			ftype2 = fut_vastldf;
			fsubtype2 = st_vastldf_LPF12;
			fws = wst_tanh;
			fOsFactor = 4;
			fParameterSmoothing = true; 
			fAttenuateFactor = 1.5f;
			break;
		
		case (FILTERTYPE::LDFHPF36):
			ftype = fut_vastldf;
			fsubtype = st_vastldf_HPF24;
			ftype2 = fut_vastldf;
			fsubtype2 = st_vastldf_HPF12;
			fws = wst_tanh;
			fOsFactor = 1;
			fParameterSmoothing = true; 
			fAttenuateFactor = 1.f;
			break;
		
		case (FILTERTYPE::LDFLPF48):
			ftype = fut_vastldf;
			fsubtype = st_vastldf_LPF24;
			ftype2 = fut_vastldf;
			fsubtype2 = st_vastldf_LPF24;
			fws = wst_tanh;
			fOsFactor = 4;
			fParameterSmoothing = true; 
			fAttenuateFactor = 1.5f;
			break;
		
		case (FILTERTYPE::LDFHPF48):
			ftype = fut_vastldf;
			fsubtype = st_vastldf_HPF24;
			ftype2 = fut_vastldf;
			fsubtype2 = st_vastldf_HPF24;
			fws = wst_tanh;
			fOsFactor = 1;
			fParameterSmoothing = true; 
			fAttenuateFactor = 1.f;
			break;		

	//*******************************************************************
	//SPECIAL Filters
	//*******************************************************************
		case (FILTERTYPE::COMBFILTER):
			ftype = fut_vastcomb;
			fOsFactor = 4;
			fParameterSmoothing = true;
			fAttenuateFactor = 1.f;
			break;

		case (FILTERTYPE::SCREAMFILTER):
			ftype = fut_vastscream;
			fOsFactor = 4;
			fParameterSmoothing = true;
			fAttenuateFactor = 1.0f;
			break;

		case (FILTERTYPE::KRGHPF12):
			ftype = fut_vastkrg12;
			fsubtype = st_vastkrg12;
			fws = wst_tanh;
			fOsFactor = 1;
			fParameterSmoothing = true;
			fAttenuateFactor = 1.f;
			break;

		case (FILTERTYPE::KRGHPFSAT12): //they are both the same now
			ftype = fut_vastkrg12; 
			fsubtype = st_vastkrg12sat;
			fws = wst_tanh;
			fOsFactor = 1;
			fParameterSmoothing = true;
			fAttenuateFactor = 1.f;
			break;

		default:
			vassertfalse;
	}
}

//per voice
void VASTQFilter::calcFilterCoefficients(bool isUi, int ftype, int fsubtype, int ftype2, int fsubtype2, int ftype3, int fsubtype3, int osFactor, float fVCFEnvelopeMod, double &cutoff1, double &resonance1, double &scale1, double &cutoff2, double &resonance2, double &scale2, double &cutoff3, double &resonance3, double &scale3, int samplerate, VASTQFilterCoefficients* coeffMaker1, VASTQFilterCoefficients* coeffMaker2) {
	float safeResonance1 = resonance1;
	switch (ftype) {
	case(fut_vastiir6):
		//TODO Smoothers!

		cutoff1 *= fVCFEnvelopeMod;
		//cutoff1 += 30.f;
		if (osFactor == 4) {
			cutoff1 /= 2.f; 
		}	
		cutoff1 = (cutoff1 > 200.f) ? cutoff1 : 200.f;
		cutoff1 = (cutoff1 < 22000.f) ? cutoff1 : 22000.f;

		cutoff2 = cutoff1;
		cutoff3 = cutoff1;
		scale1 = (((scale1)+1) / 101.f) * 2.f; //must not be zero - 0.0001 - 4.f		
		scale2 = scale1;
		scale3 = scale1;
		if (ftype3 == fut_vastiir6) {
			//cutoff2 = ((cutoff2 * fVCFEnvelopeMod) < 50.f) ? 50.0f : cutoff2 * fVCFEnvelopeMod;
			//cutoff2 = -60.f + cubicBezierApproximation(cutoff2 / 28000.f, 0.84f) * 130.f;
			resonance3 = safeResonance1 * 0.01f;
			resonance1 = 0.51763809f;
			resonance2 = sqrt2over2;
			resonance3 = 1.9318517f + (safeResonance1 * 0.01f);
		}
		else if (ftype2 == fut_vastiir6) {
			resonance2 = 1.3065630f + (safeResonance1 * 0.1f);
			resonance1 = 0.54119610f;
			//float test = Map2PoleResonance(safeResonance1, coeffMaker1->pitch_to_note(cutoff1 / coeffMaker1->dmasterTuneHz), st_Smooth);
		}
		else {
			resonance1 = (sqrt2over2 + (safeResonance1 * 0.1f));
		}

		//else { //order 6
		//	resval1 = 0.51763809f;
		//	resval2 = sqrt2over2;
		//	resval3 = 1.9318517f + resonance;
		//}
		//*/
		
		//resonance2 = Map4PoleResonance(resonance1, cutoff1, st_Smooth);
		break;

	case(fut_bp12):
	case(fut_br12):
	case(fut_hp12):
	case(fut_hp24):
	case(fut_lp12):
	case(fut_lp24):
	case(fut_lpmoog):
	case(fut_comb):		
		//if (ftype == fut_comb) //otherwise it is only going to 5k
			//cutoff1 /= 4.f;
		if ((ftype == fut_lp12) || (ftype == fut_lp24) || (ftype == fut_lpmoog) || (ftype == fut_comb))
			cutoff1 = ((cutoff1 * fVCFEnvelopeMod) < 20.f) ? 20.0f : cutoff1 * fVCFEnvelopeMod;
		else if (ftype == fut_bp12)
			cutoff1 = ((cutoff1 * fVCFEnvelopeMod * 2.f) < 50.f) ? 50.0f : cutoff1 * fVCFEnvelopeMod * 2.f;
		else
			cutoff1 = ((cutoff1 * fVCFEnvelopeMod) < 50.f) ? 50.0f : cutoff1 * fVCFEnvelopeMod;
		if (ftype == fut_br12)
			cutoff1 = cutoff1 > 21000.f ? 21000.f : cutoff1;
		cutoff1 = coeffMaker1->pitch_to_note(cutoff1 / coeffMaker1->dmasterTuneHz); //old // cutoff1 = -60.f + cubicBezierApproximation(cutoff1 / 28000.f, 0.84f) * 130.f;		

		resonance1 *= 0.01f;
		cutoff3 = cutoff1;
		//if (ftype == fut_vastscream) { //FILTERTYPE::SCREAMFILTER
			//cutoff2 = cutoff2 * 0.0625 * fVCFEnvelopeMod;
			//cutoff2 = coeffMaker1->pitch_to_note(cutoff2 / coeffMaker1->dmasterTuneHz); //old // cutoff1 = -60.f + cubicBezierApproximation(cutoff1 / 28000.f, 0.84f) * 130.f;
		//} else
			cutoff2 = cutoff1;
		resonance2 = resonance1;
		resonance3 = resonance1;
		scale2 = scale1;
		scale3 = scale1;
		break;
	case(fut_vastcomb):
	case(fut_vastscream):
		cutoff1 = cutoff1 * fVCFEnvelopeMod;;
		cutoff2 = cutoff1;
		cutoff3 = cutoff1;
		scale1 = scale1 * 0.125f + 75.f;
		resonance2 = resonance1;
		if (ftype == fut_vastscream) { //FILTERTYPE::SCREAMFILTER
			cutoff2 = cutoff1 + 1.f; //cutoff2 is ldf
			//cutoff2 /= osFactor;
			resonance2 = (resonance2 * 0.0099f); //reso must not become 1
		}
		resonance3 = resonance1;
		scale2 = scale1;
		scale3 = scale1;
		break;

	case(fut_vastsvf):
		//scale1 = 0.f;
		cutoff1 = (cutoff1 * fVCFEnvelopeMod) + 1.f;
		resonance1 = (resonance1 / 11.f + sqrt2over2);
		if (cutoff1 > 20000.f) cutoff1 = 20000.f;
		if (fsubtype == st_vastsvf_hp)
			if (cutoff1 > 10000.f) cutoff1 = 10000.f;
		if (fsubtype == st_vastsvf_bp)
			if (cutoff1 > 10000.f) cutoff1 = 10000.f;
		if (cutoff1 > samplerate * 0.5f) cutoff1 = samplerate * 0.5f;
		//if (fsubtype == st_vastsvf_lp) {
			//cutoff1 /= (4.f * 1.45f); //from 28.000 to 18.000
		//}
		//else
			//if (fsubtype == st_vastsvf_hp)
				//cutoff1 /= (4.f * 1.45f); //from 28.000 to 18.000
			//else 
				if (cutoff1 > samplerate * 0.5f) cutoff1 = samplerate * 0.5f;
		if (ftype3 == fut_vastsvf) {
			resonance3 = resonance1;
			resonance2 = 0.3f; //0.3 is the right value!
			resonance1 = 0.3f;
		} else 	if (ftype2 == fut_vastsvf) {
			resonance2 = resonance1;
			resonance1 = 0.3f;
		}
		cutoff2 = cutoff1;
		cutoff3 = cutoff1;	
		break;		

	case(fut_vastldf):	
		//scale1 = 0.f;
		cutoff1 = (cutoff1 * fVCFEnvelopeMod) + 1.f;
		cutoff1 /= osFactor; 
		cutoff2 = cutoff1;
		//if ((fsubtype == st_vastldf_HPF12) || (fsubtype == st_vastldf_HPF24))
		//	cutoff1 *= 0.5f;
		//if ((fsubtype2 == st_vastldf_HPF12) || (fsubtype2 == st_vastldf_HPF24))
		//	cutoff2 *= 0.5f;
		if (ftype2 == fut_vastldf) {
			resonance1 = 0.0f;
			resonance2 = (resonance2 * 0.0099f); //reso must not become 1
		}
		else {
			resonance1 = (resonance1 * 0.0099f); //reso must not become 1
			resonance2 = (resonance2 * 0.0099f); //reso must not become 1
		}
		break;

	case(fut_vastkrg12):
		cutoff1 = ((cutoff1 * fVCFEnvelopeMod) < 50.f) ? 50.0f : cutoff1 * fVCFEnvelopeMod;
		cutoff1 = (cutoff1 < 22000.f) ? cutoff1 : 22000.f;
		scale1 = static_cast<float> (1.f + 0.4f * (scale1 - 100.f) / 100.f); //1.f - 1.5f
		resonance1 = resonance1 / 51.0f + 0.02f;
		break;

	default:
		vassertfalse;
	}			
}

//not per voice
void VASTQFilter::calcFilterSettings(bool isUi, int ftype, int fsubtype, int ftype2, int fsubtype2, int ftype3, int fsubtype3, int osFactor, double &drive, double &feedback, double &gain) {
	switch (ftype) {
	case(fut_vastiir6):
		//feedback = 0.f;
		feedback = 0.25f * ((feedback / 100.f) - 1.f);  //-1.f .. 1.f
		gain = gain * 0.01f;
		drive = (drive / 20.f) * 24.f; //-24db .. +24db --> 0,066 ... 15,84 amplitude
		drive = pow(10.0f, drive / 20.0f);
		break;

	case(fut_bp12):
	case(fut_br12):
	case(fut_hp12):
	case(fut_hp24):
	case(fut_lp12):
	case(fut_lp24):
	case(fut_lpmoog):
	case(fut_comb):
		feedback = 0.25f * ((feedback / 100.f) - 1.f);  //-1.f .. 1.f
		gain = 2.f * gain * 0.01f;
		drive = (drive / 20.f) * 24.f; //-24db .. +24db --> 0,066 ... 15,84 amplitude
		drive = pow(10.0f, drive / 20.0f);
		break;

	case(fut_vastcomb):
	case(fut_vastscream):
		break;

	case(fut_vastsvf):
		//feedback = 0.f;
		feedback = 0.25f * ((feedback / 100.f) - 1.f);  //-1.f .. 1.f
		gain = (gain * 0.01f);
		drive = (drive / 20.f) * 24.f; //-24db .. +24db --> 0,066 ... 15,84 amplitude
		drive = pow(10.0f, drive / 20.0f);
		break;

	case(fut_vastldf):
		feedback = 0.25f * ((feedback / 100.f) - 1.f);  //-1.f .. 1.f //CHECK not as before
		gain = (gain * 0.01f);
		drive = (drive / 20.f) * 24.f; //-24db .. +24db --> 0,066 ... 15,84 amplitude
		drive = pow(10.0f, drive / 20.0f);
		break;

	case(fut_vastkrg12):
		drive = (drive / 20.f) * 24.f; //-24db .. +24db --> 0,066 ... 15,84 amplitude
		drive = pow(10.0f, drive / 20.0f);
		break;

	default:
		vassertfalse;
	}

	//Surge:
	//float Drive = db_to_linear(localcopy[id_drive].f);
	//float Gain = db_to_linear(localcopy[id_vca].f + localcopy[id_vcavel].f * (1.f - state.fvel)) *
	//	modsources[ms_ampeg]->output;

}

int VASTQFilter::processQFilter(dsp::AudioBlock<float> filterBlock, sRoutingBuffers* routingBuffers, int numSamples, int startSample, int ftype, int fsubtype, int ftype2, int fsubtype2, int ftype3, int fsubtype3, int fws, int osFactor, OwnedArray<VASTSynthesiserVoice>* voices, int filter, float gain, float drive, float feedback, bool isUI, bool hasNextFilter, std::unique_ptr<CVASTVcf>* uiVCF, bool warmup) {
	int numFilterVoices = 0;

	bool isLegacy = false;
	if ((ftype == fut_vastcomb) || (ftype == fut_vastscream) || (ftype == fut_vastkrg12)) //legacy
		isLegacy = true;
	if (isLegacy) {
		ProcessLegacy(filterBlock, routingBuffers, numSamples, startSample, ftype, fsubtype, ftype2, fsubtype2, ftype3, fsubtype3, fws, osFactor, voices, filter, gain, drive, feedback, isUI, hasNextFilter, uiVCF, warmup);
	} 
	else {
		Fbq_global g{};
		g.FU1ptr = VASTQFilter::GetQFPtrFilterUnit(ftype, fsubtype); //first filter A	
		if (ftype2 != fut_none) //second filter B
			g.FU2ptr = VASTQFilter::GetQFPtrFilterUnit(ftype2, fsubtype2);
		else
			g.FU2ptr = nullptr;
		if (ftype3 != fut_none) //third filter C
			g.FU3ptr = VASTQFilter::GetQFPtrFilterUnit(ftype3, fsubtype3);
		else
			g.FU3ptr = nullptr;

		if ((drive == 1.f) && (fws != wst_sinus)) //sine ws does something at 1.0
			g.WSptr = nullptr; //perfopt
		else
			g.WSptr = VASTQFilter::GetQFPtrWaveshaper(fws); //wst_hard, asym, wst_sinus, wst_none

		this->ProcessQuadFB = GetFBQPointer(g.WSptr != nullptr, g.FU1ptr != nullptr, g.FU2ptr != nullptr, g.FU3ptr != nullptr);

		float outl alignas(16)[c_blocksize_q * c_max_os_factor_q];
		float outr alignas(16)[c_blocksize_q * c_max_os_factor_q];
		memset(outl, 0, c_blocksize_q * c_max_os_factor_q * sizeof(float));
		memset(outr, 0, c_blocksize_q * c_max_os_factor_q * sizeof(float));

		float voicesl alignas(16)[c_blocksize_q * c_max_os_factor_q][4]; //4 voices
		float voicesr alignas(16)[c_blocksize_q * c_max_os_factor_q][4]; //4 voices
		memset(voicesl, 0, c_blocksize_q * c_max_os_factor_q * 4 * sizeof(float));
		memset(voicesr, 0, c_blocksize_q * c_max_os_factor_q * 4 * sizeof(float));

		int voiceAssignment[C_MAX_POLY];
		memset(voiceAssignment, -1, C_MAX_POLY * sizeof(int));

		int highestVoicePlaying = -1;

		CVASTVcf* vcf;
		bool complete = false;
		int voiceNo = -1;
		VASTSynthesiserVoice* voice;

		while (!complete)
		{
			bool doit = true;
			if (isUI) {
				vcf = uiVCF[filter].get();
				complete = true;
			}
			else {
				voiceNo++;
				voice = (*voices)[voiceNo];
				vcf = ((CVASTSingleNote*)voice)->m_VCF[filter];
				bool bPlayiningInRange = ((CVASTSingleNote*)voice)->isPlayingInRange(startSample, numSamples); //requires that mseg is processed before!
				doit = (bPlayiningInRange || (((CVASTSingleNote*)voice)->m_bLastFilterOutputZero[filter] == false));				
				if (voiceNo == ((CVASTSingleNote*)voice)->m_Set->m_uMaxPoly - 1)
					complete = true;
			}
			if (doit) {
				VASTQFilterProcessState* Q = &FBQ[filter][numFilterVoices >> 2]; //>>2 means div 4									
				int Qe = numFilterVoices & 3; //& 3 means mod 4

				//upsample
				dsp::AudioBlock<float> inBlock;
				if (!isUI)
					inBlock = dsp::AudioBlock<float>(*routingBuffers->FilterVoices[filter][voiceNo]).getSubBlock(startSample, numSamples);
				else
					inBlock = dsp::AudioBlock<float>(filterBlock);
				for (int j = 0; j < numSamples * osFactor; j++) {
					float lVal = 0.f;
					float rVal = 0.f;
					if (j % osFactor == 0) {
						lVal = inBlock.getSample(0, j / osFactor);
						rVal = inBlock.getSample(1, j / osFactor);
					}
					set1f(Q->DL[j], Qe, lVal);
					set1f(Q->DR[j], Qe, rVal);
				}
				highestVoicePlaying = voiceNo;

				voiceAssignment[numFilterVoices] = voiceNo; //slot -> voice				

				vcf->SetQFB(Q, Qe, gain, drive, feedback, ftype, fsubtype, ftype2, fsubtype2, ftype3, fsubtype3, numSamples * osFactor);				

				numFilterVoices++; //num voices going in filter
				//vString += " " + String(mVoiceNo);
			}
		}
		//VDBG("Filter: " + String(filter) + " Filtervoices " + vString);

		//all voices	
		//numFilterVoices = highestVoicePlaying + 1;
		int numVoices = numFilterVoices;
		if (numVoices > 0)
			for (int e = 0; e < numVoices; e += 4)
			{
				VASTQFilterProcessState* Q = &FBQ[filter][e >> 2]; //>>2 means div 4
				int units = numVoices - e;
				int activeVoices = 4;
				for (int i = units; i < 4; i++)
				{ // loop only sets inactive for last slots - default is active
					activeVoices -= 1;
					Q->FU[0].active[i] = 0; // >> 2 means / 4 // FBQ[s] has 4 
					Q->FU[1].active[i] = 0;
					Q->FU[2].active[i] = 0;
					Q->FU[3].active[i] = 0;
					Q->FU[4].active[i] = 0;
					Q->FU[5].active[i] = 0;
				}

				(this->*ProcessQuadFB)(*Q, g, outl, outr, voicesl, voicesr, numSamples * osFactor);

				//Downsample active (max 4) voices // check if required - not required if e.g. no next filter! TODO!	
				//STOP: have to caluclate FilterVoices always as they are relevant for the m_bLastFilterOutputZero - why??
				//if ((!isUI) && (hasNextFilter)) {
				if (!isUI) {
					for (int i = 0; i < activeVoices; i++) {
						int voice = voiceAssignment[e + i];  //slot
						float* fwL = routingBuffers->FilterVoices[filter][voice]->getWritePointer(0);
						float* fwR = routingBuffers->FilterVoices[filter][voice]->getWritePointer(1);
						if (osFactor == 1) {
							for (int j = 0; j < numSamples * osFactor; j++) {
								if (j % osFactor == 0) {
									fwL[startSample + j / osFactor] = voicesl[j][i]; //samples, voice
									fwR[startSample + j / osFactor] = voicesr[j][i]; //samples, voice
								}
							}
						}
						else if (osFactor == 2) {
							
							float lf alignas(16)[c_blocksize_q * 2];
							float rf alignas(16)[c_blocksize_q * 2];
							memset(lf, 0, c_blocksize_q * 2 * sizeof(float));
							memset(rf, 0, c_blocksize_q * 2 * sizeof(float));
							for (int j = 0; j < numSamples * osFactor; j++) {
								lf[j] = voicesl[j][i];
								rf[j] = voicesr[j][i];
							}
					
							m_DecimatorVoices[filter][voice].decimateSamples2(lf, rf, numSamples * osFactor, fwL + startSample, fwR + startSample); //expects it to be dividable by 2 but no longer memory aligned to 16 bit!
							//causes clicks - why?
							

							/*
							for (int j = 0; j < numSamples * osFactor; j++) {
								if (j % osFactor == 0) {
									fwL[startSample + j / osFactor] = voicesl[j][i]; //samples, voice 
									fwR[startSample + j / osFactor] = voicesr[j][i]; //samples, voice 
								}
							}
							*/

						}
						else {
							//TODO
							//((CVASTSingleNote*)&voices[voice])->m_VCF[filter]->m_Oversampler.downsampleAudioBuffer4();

							for (int j = 0; j < numSamples * osFactor; j++) {
								if (j % osFactor == 0) {
									fwL[startSample + j / osFactor] = voicesl[j][i] * osFactor; //samples, voice //zero pading changes gain!
									fwR[startSample + j / osFactor] = voicesr[j][i] * osFactor; //samples, voice //zero pading changes gain!
								}
							}
						}
					}
				}
			}

		if (!warmup) {
			//downsample
			if (osFactor == 1) {
				std::copy(outl + 0, outl + numSamples, filterBlock.getChannelPointer(0));
				std::copy(outr + 0, outr + numSamples, filterBlock.getChannelPointer(1));
			}
			else
				//halfband filter if needed
				if (osFactor == 2) {
					//if (startSample % 4 != 0) 
						//VDBG("Not %4 StartSample!");
					m_Decimator[filter].decimateSamples2(outl, outr, numSamples * osFactor, filterBlock.getChannelPointer(0), filterBlock.getChannelPointer(1)); //expects it to be dividable by 2 but no longer memory aligned to 16 bit!
				}
				else
					//m_Oversampler[filter].downsampleAudioBuffer4();

					for (int j = 0; j < numSamples * osFactor; j++) {
						if (j % osFactor == 0) {
							filterBlock.setSample(0, j / osFactor, outl[j] * osFactor); //zero pading changes gain!
							filterBlock.setSample(1, j / osFactor, outr[j] * osFactor); //zero pading changes gain!
						}
					}
		}

	//per voices
		if (!isUI) {
			for (auto* voice : *voices)
			{
				CVASTSingleNote* myNote = (CVASTSingleNote*)voice;
				int mVoiceNo = voice->getVoiceNo();
				if (mVoiceNo <= highestVoicePlaying) {
					bool bPlayiningInRange = ((CVASTSingleNote*)voice)->isPlayingInRange(startSample, numSamples); //requires that mseg is processed before!

					if (bPlayiningInRange || (((CVASTSingleNote*)voice)->m_bLastFilterOutputZero[filter] == false)) {
						
						myNote->m_VCF[filter]->GetQFB();

					}
				}
			}
		}
		else {

			uiVCF[filter]->GetQFB();

		}
	}
    return numFilterVoices;
}

void VASTQFilter::ProcessLegacy(dsp::AudioBlock<float> filterBlock, sRoutingBuffers* routingBuffers, int numSamples, int startSample, int ftype, int fsubtype, int ftype2, int fsubtype2, int ftype3, int fsubtype3, int fws, int osFactor, OwnedArray<VASTSynthesiserVoice>* voices, int filter, float gain, float drive, float feedback, bool isUI, bool hasNextFilter, std::unique_ptr<CVASTVcf>* uiVCF, bool warmup) {
	int filterBlocksize = numSamples; //??
	
	AudioSampleBuffer inBufferUp(2, numSamples * osFactor); //the on in vcf not needed anymore?

	CVASTVcf* vcf;	
	bool complete = false;
	int voiceNo = -1;
	VASTSynthesiserVoice* voice;
	while (!complete)
	{
		bool doit = true;
		if (isUI) {
			vcf = uiVCF[filter].get();
			complete = true;
		}
		else {
			voiceNo++;
			voice = (*voices)[voiceNo];
			vcf = ((CVASTSingleNote*)voice)->m_VCF[filter];
			bool bPlayiningInRange = ((CVASTSingleNote*)voice)->isPlayingInRange(startSample, numSamples); //requires that mseg is processed before!
			doit = (bPlayiningInRange || (((CVASTSingleNote*)voice)->m_bLastFilterOutputZero[filter] == false));
			if (voiceNo == ((CVASTSingleNote*)voice)->m_Set->m_uMaxPoly - 1)
				complete = true;
		}
		
		if (doit) {
			dsp::AudioBlock<float> inBlock;
			if (!isUI) {
				dsp::AudioBlock<float> fBlock(*routingBuffers->FilterVoices[filter][voiceNo]);
				inBlock = fBlock.getSubBlock(startSample, numSamples);
			}
			else
				inBlock = filterBlock;

			if ((ftype == fut_vastcomb) || (ftype == fut_vastscream)) {
				//upsample
				dsp::AudioBlock<float> inBlockUp = dsp::AudioBlock<float>(inBufferUp);
				vcf->m_Oversampler.upsampleAudioBuffer4(inBlock, inBlockUp, filterBlocksize); //only 4 as oversampling??

				vcf->m_combFilter.parameterChanged("m_fCombDryWet", 100.f, true, (filterBlocksize * osFactor) - 1); //0 .. 100
				vcf->m_combFilter.parameterChanged("m_fCombDrive", drive, vcf->m_bSmoothersTakeNextValue, (filterBlocksize * osFactor) - 1); //-20 .. 20 //m_bSmoothersTakeNextValue??
				vcf->m_combFilter.parameterChanged("m_fCombGain", gain, vcf->m_bSmoothersTakeNextValue, (filterBlocksize * osFactor) - 1); //0..200  //m_bSmoothersTakeNextValue??

				//process
				if (ftype == fut_vastscream) {
					vcf->m_ladderFilter1.process(dsp::ProcessContextReplacing<float>(inBlockUp));
				}
				vcf->m_combFilter.processBlock(inBlockUp, filterBlocksize * osFactor);

				//downsample
				if (!warmup) {
					vcf->m_Oversampler.downsampleAudioBuffer4(inBlockUp, inBlock, filterBlocksize * osFactor);
				}
			}
			else { 
				if (ftype == fut_vastkrg12) {	
					//if (fsubtype == st_vastkrg12sat)
					vcf->m_TFHPF_uNonLinearProcessing = vcf->ON; //causes aliasing	
					//else
						//vcf->m_TFHPF_uNonLinearProcessing = vcf->OFF; 
					vcf->KorgThreeFiveHPFmkIIupdateFilters();
					for (int i = 0; i < numSamples; i++) {
						double lFilterIn[2]{ inBlock.getSample(0, i), inBlock.getSample(1, i) };
						double fOut[2]{ 0, 0 };
						//do filter
						vcf->KorgThreeFiveHPFmkIIdoFilter(lFilterIn, fOut);
						if (!warmup) {
							inBlock.setSample(0, i, fOut[0]);
							inBlock.setSample(1, i, fOut[1]);
						}
					}
				}
				else
					vassertfalse;
			}

			if (!isUI)
				filterBlock += inBlock;
		}
	}
}

//SSE Intrinsic Cheat Sheet (SSE3)
//https://gist.github.com/detomon/fb9db687b154d67dbb50

__m128 VASTQFilter::SVFLP12Aquad(VASTQFilterStepState* __restrict f, __m128 in)
{
	f->C[0] = _mm_add_ps(f->C[0], f->dC[0]); // F1
	f->C[1] = _mm_add_ps(f->C[1], f->dC[1]); // Q1

	__m128 L = _mm_add_ps(f->R[1], _mm_mul_ps(f->C[0], f->R[0]));
	__m128 H = _mm_sub_ps(_mm_sub_ps(in, L), _mm_mul_ps(f->C[1], f->R[0]));
	__m128 B = _mm_add_ps(f->R[0], _mm_mul_ps(f->C[0], H));

	__m128 L2 = _mm_add_ps(L, _mm_mul_ps(f->C[0], B));
	__m128 H2 = _mm_sub_ps(_mm_sub_ps(in, L2), _mm_mul_ps(f->C[1], B));
	__m128 B2 = _mm_add_ps(B, _mm_mul_ps(f->C[0], H2));

	f->R[0] = _mm_mul_ps(B2, f->R[2]);
	f->R[1] = _mm_mul_ps(L2, f->R[2]);

	f->C[2] = _mm_add_ps(f->C[2], f->dC[2]);
	const __m128 m01 = _mm_set1_ps(0.1f);
	const __m128 m1 = _mm_set1_ps(1.0f);
	f->R[2] = _mm_max_ps(m01, _mm_sub_ps(m1, _mm_mul_ps(f->C[2], _mm_mul_ps(B, B))));

	f->C[3] = _mm_add_ps(f->C[3], f->dC[3]); // Gain
	return _mm_mul_ps(L2, f->C[3]);
}

__m128 VASTQFilter::SVFLP24Aquad(VASTQFilterStepState* __restrict f, __m128 in)
{
	f->C[0] = _mm_add_ps(f->C[0], f->dC[0]); // F1
	f->C[1] = _mm_add_ps(f->C[1], f->dC[1]); // Q1

	__m128 L = _mm_add_ps(f->R[1], _mm_mul_ps(f->C[0], f->R[0]));
	__m128 H = _mm_sub_ps(_mm_sub_ps(in, L), _mm_mul_ps(f->C[1], f->R[0]));
	__m128 B = _mm_add_ps(f->R[0], _mm_mul_ps(f->C[0], H));

	L = _mm_add_ps(L, _mm_mul_ps(f->C[0], B));
	H = _mm_sub_ps(_mm_sub_ps(in, L), _mm_mul_ps(f->C[1], B));
	B = _mm_add_ps(B, _mm_mul_ps(f->C[0], H));

	f->R[0] = _mm_mul_ps(B, f->R[2]);
	f->R[1] = _mm_mul_ps(L, f->R[2]);

	in = L;

	L = _mm_add_ps(f->R[4], _mm_mul_ps(f->C[0], f->R[3]));
	H = _mm_sub_ps(_mm_sub_ps(in, L), _mm_mul_ps(f->C[1], f->R[3]));
	B = _mm_add_ps(f->R[3], _mm_mul_ps(f->C[0], H));

	L = _mm_add_ps(L, _mm_mul_ps(f->C[0], B));
	H = _mm_sub_ps(_mm_sub_ps(in, L), _mm_mul_ps(f->C[1], B));
	B = _mm_add_ps(B, _mm_mul_ps(f->C[0], H));

	f->R[3] = _mm_mul_ps(B, f->R[2]);
	f->R[4] = _mm_mul_ps(L, f->R[2]);

	f->C[2] = _mm_add_ps(f->C[2], f->dC[2]);
	const __m128 m01 = _mm_set1_ps(0.1f);
	const __m128 m1 = _mm_set1_ps(1.0f);
	f->R[2] = _mm_max_ps(m01, _mm_sub_ps(m1, _mm_mul_ps(f->C[2], _mm_mul_ps(B, B))));

	f->C[3] = _mm_add_ps(f->C[3], f->dC[3]); // Gain
	return _mm_mul_ps(L, f->C[3]);
}

__m128 VASTQFilter::SVFHP24Aquad(VASTQFilterStepState* __restrict f, __m128 in)
{
	f->C[0] = _mm_add_ps(f->C[0], f->dC[0]); // F1
	f->C[1] = _mm_add_ps(f->C[1], f->dC[1]); // Q1

	__m128 L = _mm_add_ps(f->R[1], _mm_mul_ps(f->C[0], f->R[0]));
	__m128 H = _mm_sub_ps(_mm_sub_ps(in, L), _mm_mul_ps(f->C[1], f->R[0]));
	__m128 B = _mm_add_ps(f->R[0], _mm_mul_ps(f->C[0], H));

	L = _mm_add_ps(L, _mm_mul_ps(f->C[0], B));
	H = _mm_sub_ps(_mm_sub_ps(in, L), _mm_mul_ps(f->C[1], B));
	B = _mm_add_ps(B, _mm_mul_ps(f->C[0], H));

	f->R[0] = _mm_mul_ps(B, f->R[2]);
	f->R[1] = _mm_mul_ps(L, f->R[2]);

	in = H;

	L = _mm_add_ps(f->R[4], _mm_mul_ps(f->C[0], f->R[3]));
	H = _mm_sub_ps(_mm_sub_ps(in, L), _mm_mul_ps(f->C[1], f->R[3]));
	B = _mm_add_ps(f->R[3], _mm_mul_ps(f->C[0], H));

	L = _mm_add_ps(L, _mm_mul_ps(f->C[0], B));
	H = _mm_sub_ps(_mm_sub_ps(in, L), _mm_mul_ps(f->C[1], B));
	B = _mm_add_ps(B, _mm_mul_ps(f->C[0], H));

	f->R[3] = _mm_mul_ps(B, f->R[2]);
	f->R[4] = _mm_mul_ps(L, f->R[2]);

	f->C[2] = _mm_add_ps(f->C[2], f->dC[2]);
	const __m128 m01 = _mm_set1_ps(0.1f);
	const __m128 m1 = _mm_set1_ps(1.0f);
	f->R[2] = _mm_max_ps(m01, _mm_sub_ps(m1, _mm_mul_ps(f->C[2], _mm_mul_ps(B, B))));

	f->C[3] = _mm_add_ps(f->C[3], f->dC[3]); // Gain
	return _mm_mul_ps(H, f->C[3]);
}

__m128 VASTQFilter::SVFBP24Aquad(VASTQFilterStepState* __restrict f, __m128 in)
{
	f->C[0] = _mm_add_ps(f->C[0], f->dC[0]); // F1
	f->C[1] = _mm_add_ps(f->C[1], f->dC[1]); // Q1

	__m128 L = _mm_add_ps(f->R[1], _mm_mul_ps(f->C[0], f->R[0]));
	__m128 H = _mm_sub_ps(_mm_sub_ps(in, L), _mm_mul_ps(f->C[1], f->R[0]));
	__m128 B = _mm_add_ps(f->R[0], _mm_mul_ps(f->C[0], H));

	L = _mm_add_ps(L, _mm_mul_ps(f->C[0], B));
	H = _mm_sub_ps(_mm_sub_ps(in, L), _mm_mul_ps(f->C[1], B));
	B = _mm_add_ps(B, _mm_mul_ps(f->C[0], H));

	f->R[0] = _mm_mul_ps(B, f->R[2]);
	f->R[1] = _mm_mul_ps(L, f->R[2]);

	in = B;

	L = _mm_add_ps(f->R[4], _mm_mul_ps(f->C[0], f->R[3]));
	H = _mm_sub_ps(_mm_sub_ps(in, L), _mm_mul_ps(f->C[1], f->R[3]));
	B = _mm_add_ps(f->R[3], _mm_mul_ps(f->C[0], H));

	L = _mm_add_ps(L, _mm_mul_ps(f->C[0], B));
	H = _mm_sub_ps(_mm_sub_ps(in, L), _mm_mul_ps(f->C[1], B));
	B = _mm_add_ps(B, _mm_mul_ps(f->C[0], H));

	f->R[3] = _mm_mul_ps(B, f->R[2]);
	f->R[4] = _mm_mul_ps(L, f->R[2]);

	f->C[2] = _mm_add_ps(f->C[2], f->dC[2]);
	const __m128 m01 = _mm_set1_ps(0.1f);
	const __m128 m1 = _mm_set1_ps(1.0f);
	f->R[2] = _mm_max_ps(m01, _mm_sub_ps(m1, _mm_mul_ps(f->C[2], _mm_mul_ps(B, B))));

	f->C[3] = _mm_add_ps(f->C[3], f->dC[3]); // Gain
	return _mm_mul_ps(B, f->C[3]);
}

__m128 VASTQFilter::SVFHP12Aquad(VASTQFilterStepState* __restrict f, __m128 in)
{
	f->C[0] = _mm_add_ps(f->C[0], f->dC[0]); // F1
	f->C[1] = _mm_add_ps(f->C[1], f->dC[1]); // Q1

	__m128 L = _mm_add_ps(f->R[1], _mm_mul_ps(f->C[0], f->R[0]));
	__m128 H = _mm_sub_ps(_mm_sub_ps(in, L), _mm_mul_ps(f->C[1], f->R[0]));
	__m128 B = _mm_add_ps(f->R[0], _mm_mul_ps(f->C[0], H));

	__m128 L2 = _mm_add_ps(L, _mm_mul_ps(f->C[0], B));
	__m128 H2 = _mm_sub_ps(_mm_sub_ps(in, L2), _mm_mul_ps(f->C[1], B));
	__m128 B2 = _mm_add_ps(B, _mm_mul_ps(f->C[0], H2));

	f->R[0] = _mm_mul_ps(B2, f->R[2]);
	f->R[1] = _mm_mul_ps(L2, f->R[2]);

	f->C[2] = _mm_add_ps(f->C[2], f->dC[2]);
	const __m128 m01 = _mm_set1_ps(0.1f);
	const __m128 m1 = _mm_set1_ps(1.0f);
	f->R[2] = _mm_max_ps(m01, _mm_sub_ps(m1, _mm_mul_ps(f->C[2], _mm_mul_ps(B, B))));

	f->C[3] = _mm_add_ps(f->C[3], f->dC[3]); // Gain
	return _mm_mul_ps(H2, f->C[3]);
}

__m128 VASTQFilter::SVFBP12Aquad(VASTQFilterStepState* __restrict f, __m128 in)
{
	f->C[0] = _mm_add_ps(f->C[0], f->dC[0]); // F1
	f->C[1] = _mm_add_ps(f->C[1], f->dC[1]); // Q1

	__m128 L = _mm_add_ps(f->R[1], _mm_mul_ps(f->C[0], f->R[0]));
	__m128 H = _mm_sub_ps(_mm_sub_ps(in, L), _mm_mul_ps(f->C[1], f->R[0]));
	__m128 B = _mm_add_ps(f->R[0], _mm_mul_ps(f->C[0], H));

	__m128 L2 = _mm_add_ps(L, _mm_mul_ps(f->C[0], B));
	__m128 H2 = _mm_sub_ps(_mm_sub_ps(in, L2), _mm_mul_ps(f->C[1], B));
	__m128 B2 = _mm_add_ps(B, _mm_mul_ps(f->C[0], H2));

	f->R[0] = _mm_mul_ps(B2, f->R[2]);
	f->R[1] = _mm_mul_ps(L2, f->R[2]);

	f->C[2] = _mm_add_ps(f->C[2], f->dC[2]);
	const __m128 m01 = _mm_set1_ps(0.1f);
	const __m128 m1 = _mm_set1_ps(1.0f);
	f->R[2] = _mm_max_ps(m01, _mm_sub_ps(m1, _mm_mul_ps(f->C[2], _mm_mul_ps(B, B))));

	f->C[3] = _mm_add_ps(f->C[3], f->dC[3]); // Gain
	return _mm_mul_ps(B2, f->C[3]);
}

__m128 VASTQFilter::IIR12Aquad(VASTQFilterStepState* __restrict f, __m128 in)
{
	f->C[1] = _mm_add_ps(f->C[1], f->dC[1]);                                       // K2
	f->C[3] = _mm_add_ps(f->C[3], f->dC[3]);                                       // Q2
	__m128 f2 = _mm_sub_ps(_mm_mul_ps(f->C[3], in), _mm_mul_ps(f->C[1], f->R[1])); // Q2*in - K2*R1
	__m128 g2 = _mm_add_ps(_mm_mul_ps(f->C[1], in), _mm_mul_ps(f->C[3], f->R[1])); // K2*in + Q2*R1

	f->C[0] = _mm_add_ps(f->C[0], f->dC[0]);                                       // K1
	f->C[2] = _mm_add_ps(f->C[2], f->dC[2]);                                       // Q1
	__m128 f1 = _mm_sub_ps(_mm_mul_ps(f->C[2], f2), _mm_mul_ps(f->C[0], f->R[0])); // Q1*f2 - K1*R0
	__m128 g1 = _mm_add_ps(_mm_mul_ps(f->C[0], f2), _mm_mul_ps(f->C[2], f->R[0])); // K1*f2 + Q1*R0

	f->C[4] = _mm_add_ps(f->C[4], f->dC[4]); // V1
	f->C[5] = _mm_add_ps(f->C[5], f->dC[5]); // V2
	f->C[6] = _mm_add_ps(f->C[6], f->dC[6]); // V3
	__m128 y = _mm_add_ps(_mm_add_ps(_mm_mul_ps(f->C[6], g2), _mm_mul_ps(f->C[5], g1)),
		_mm_mul_ps(f->C[4], f1));

	f->R[0] = f1;
	f->R[1] = g1;

	return y;
}

__m128 VASTQFilter::IIR12Bquad(VASTQFilterStepState* __restrict f, __m128 in)
{
	__m128 f2 = _mm_sub_ps(_mm_mul_ps(f->C[3], in), _mm_mul_ps(f->C[1], f->R[1])); // Q2*in - K2*R1
	f->C[1] = _mm_add_ps(f->C[1], f->dC[1]);                                       // K2
	f->C[3] = _mm_add_ps(f->C[3], f->dC[3]);                                       // Q2
	__m128 g2 = _mm_add_ps(_mm_mul_ps(f->C[1], in), _mm_mul_ps(f->C[3], f->R[1])); // K2*in + Q2*R1

	__m128 f1 = _mm_sub_ps(_mm_mul_ps(f->C[2], f2), _mm_mul_ps(f->C[0], f->R[0])); // Q1*f2 - K1*R0
	f->C[0] = _mm_add_ps(f->C[0], f->dC[0]);                                       // K1
	f->C[2] = _mm_add_ps(f->C[2], f->dC[2]);                                       // Q1
	__m128 g1 = _mm_add_ps(_mm_mul_ps(f->C[0], f2), _mm_mul_ps(f->C[2], f->R[0])); // K1*f2 + Q1*R0

	f->C[4] = _mm_add_ps(f->C[4], f->dC[4]); // V1
	f->C[5] = _mm_add_ps(f->C[5], f->dC[5]); // V2
	f->C[6] = _mm_add_ps(f->C[6], f->dC[6]); // V3
	__m128 y = _mm_add_ps(_mm_add_ps(_mm_mul_ps(f->C[6], g2), _mm_mul_ps(f->C[5], g1)),
		_mm_mul_ps(f->C[4], f1));

	f->R[0] = _mm_mul_ps(f1, f->R[2]);
	f->R[1] = _mm_mul_ps(g1, f->R[2]);

	f->C[7] = _mm_add_ps(f->C[7], f->dC[7]); // Clipgain
	const __m128 m01 = _mm_set1_ps(0.1f);
	const __m128 m1 = _mm_set1_ps(1.0f);

	f->R[2] = _mm_max_ps(m01, _mm_sub_ps(m1, _mm_mul_ps(f->C[7], _mm_mul_ps(y, y))));

	return y;
}

__m128 VASTQFilter::VASTsvfLP(VASTQFilterStepState* __restrict f, __m128 in) {

	f->C[0] = _mm_add_ps(f->C[0], f->dC[0]); //g state.g --> cutoff frequency
	f->C[1] = _mm_add_ps(f->C[1], f->dC[1]); //R2 state.R2 --> resonance
	f->C[2] = _mm_add_ps(f->C[2], f->dC[2]); //h  state.h --> (1.0 / (1.0 + R2 * g + g * g));

	f->R[4] = _mm_mul_ps(_mm_sub_ps(_mm_sub_ps(_mm_sub_ps(in, _mm_mul_ps(f->R[0], f->C[1])), _mm_mul_ps(f->R[0], f->C[0])), f->R[1]), f->C[2]); // y[2] = (sample - s1 * state.R2 - s1 * state.g - s2) * state.h;
	f->R[3] = _mm_add_ps(_mm_mul_ps(f->R[4], f->C[0]), f->R[0]); // y[1] = y[2] * state.g + s1;
	f->R[0] = _mm_add_ps(_mm_mul_ps(f->R[4], f->C[0]), f->R[3]); // s1   = y[2] * state.g + y[1];
	f->R[2] = _mm_add_ps(_mm_mul_ps(f->R[3], f->C[0]), f->R[1]); // y[0] = y[1] * state.g + s2;
	f->R[1] = _mm_add_ps(_mm_mul_ps(f->R[3], f->C[0]), f->R[2]); // s2   = y[1] * state.g + y[0];

	return f->R[2]; //0 is bp, 1 is lp, 2 is lp, 3 is bp, 4 is hp?
}

__m128 VASTQFilter::VASTsvfHP(VASTQFilterStepState* __restrict f, __m128 in) {

	f->C[0] = _mm_add_ps(f->C[0], f->dC[0]); //g state.g --> cutoff frequency
	f->C[1] = _mm_add_ps(f->C[1], f->dC[1]); //R2 state.R2 --> resonance
	f->C[2] = _mm_add_ps(f->C[2], f->dC[2]); //h  state.h --> (1.0 / (1.0 + R2 * g + g * g));

	f->R[4] = _mm_mul_ps(_mm_sub_ps(_mm_sub_ps(_mm_sub_ps(in, _mm_mul_ps(f->R[0], f->C[1])), _mm_mul_ps(f->R[0], f->C[0])), f->R[1]), f->C[2]); // y[2] = (sample - s1 * state.R2 - s1 * state.g - s2) * state.h;
	f->R[3] = _mm_add_ps(_mm_mul_ps(f->R[4], f->C[0]), f->R[0]); // y[1] = y[2] * state.g + s1;
	f->R[0] = _mm_add_ps(_mm_mul_ps(f->R[4], f->C[0]), f->R[3]); // s1   = y[2] * state.g + y[1];
	f->R[2] = _mm_add_ps(_mm_mul_ps(f->R[3], f->C[0]), f->R[1]); // y[0] = y[1] * state.g + s2;
	f->R[1] = _mm_add_ps(_mm_mul_ps(f->R[3], f->C[0]), f->R[2]); // s2   = y[1] * state.g + y[0];

	return f->R[4]; //0 is bp, 1 is lp, 2 is lp, 3 is bp, 4 is hp?
}

__m128 VASTQFilter::VASTsvfBP(VASTQFilterStepState* __restrict f, __m128 in) {

	f->C[0] = _mm_add_ps(f->C[0], f->dC[0]); //g state.g --> cutoff frequency
	f->C[1] = _mm_add_ps(f->C[1], f->dC[1]); //R2 state.R2 --> resonance
	f->C[2] = _mm_add_ps(f->C[2], f->dC[2]); //h  state.h --> (1.0 / (1.0 + R2 * g + g * g));

	f->R[4] = _mm_mul_ps(_mm_sub_ps(_mm_sub_ps(_mm_sub_ps(in, _mm_mul_ps(f->R[0], f->C[1])), _mm_mul_ps(f->R[0], f->C[0])), f->R[1]), f->C[2]); // y[2] = (sample - s1 * state.R2 - s1 * state.g - s2) * state.h;
	f->R[3] = _mm_add_ps(_mm_mul_ps(f->R[4], f->C[0]), f->R[0]); // y[1] = y[2] * state.g + s1;
	f->R[0] = _mm_add_ps(_mm_mul_ps(f->R[4], f->C[0]), f->R[3]); // s1   = y[2] * state.g + y[1];
	f->R[2] = _mm_add_ps(_mm_mul_ps(f->R[3], f->C[0]), f->R[1]); // y[0] = y[1] * state.g + s2;
	f->R[1] = _mm_add_ps(_mm_mul_ps(f->R[3], f->C[0]), f->R[2]); // s2   = y[1] * state.g + y[0];

	return f->R[0]; //0 is bp, 1 is lp, 2 is lp, 3 is bp, 4 is hp?
}

__m128 VASTQFilter::VASTldf(VASTQFilterStepState* __restrict f, __m128 in)
{
	f->C[0] = _mm_add_ps(f->C[0], f->dC[0]); //a1 cutoffTransformValue
	f->C[1] = _mm_add_ps(f->C[1], f->dC[1]); //scaledResonanceValue
	//f->C[2] = _mm_add_ps(f->C[2], f->dC[2]); //gain
	//f->C[3] = _mm_add_ps(f->C[3], f->dC[3]); //gain2
	//f->C[4] = _mm_add_ps(f->C[4], f->dC[4]); //drive
	//f->C[5] = _mm_add_ps(f->C[5], f->dC[5]); //drive2
	f->C[2] = _mm_add_ps(f->C[2], f->dC[2]); //comp

	f->C[3] = _mm_add_ps(f->C[3], f->dC[3]); //A[0]
	f->C[4] = _mm_add_ps(f->C[4], f->dC[4]); //A[1]
	f->C[5] = _mm_add_ps(f->C[5], f->dC[5]); //A[2]
	f->C[6] = _mm_add_ps(f->C[6], f->dC[6]); //A[3]
	f->C[7] = _mm_add_ps(f->C[7], f->dC[7]); //A[4]

	__m128 g = _mm_add_ps(_mm_mul_ps(f->C[0], m128_minus1), m128_one); // const auto g =  a1 * Type (-1) + Type (1);
	__m128 b0 = _mm_mul_ps(g, m128_ldfc1); // const auto b0 = g * Type (0.76923076923);
	__m128 b1 = _mm_mul_ps(g, m128_ldfc2); // const auto b1 = g * Type (0.23076923076);

	/*
	__m128 satLUT1 = _mm_mul_ps(f->C[4], in); //drive * inputValue
	__m128 saturation1 = _mm_setr_ps(
		saturationLUT(get1f(satLUT1, 0)),
		saturationLUT(get1f(satLUT1, 1)),
		saturationLUT(get1f(satLUT1, 2)),
		saturationLUT(get1f(satLUT1, 3)));
	__m128 dx = _mm_mul_ps(f->C[2], saturation1); // const auto dx = gain * saturationLUT (drive * inputValue);
	__m128 satLUT2 = _mm_mul_ps(f->C[5], f->R[4]); //saturationLUT (drive2 * s[4])

	__m128 saturation2 = _mm_setr_ps(
		saturationLUT(get1f(satLUT2, 0)),
		saturationLUT(get1f(satLUT2, 1)),
		saturationLUT(get1f(satLUT2, 2)),
		saturationLUT(get1f(satLUT2, 3)));
	__m128 a = _mm_add_ps(dx, _mm_mul_ps(f->C[1], _mm_mul_ps(m128_minus4, _mm_sub_ps(_mm_mul_ps(f->C[3], saturation2), _mm_mul_ps(dx, f->C[6]))))); // const auto a = dx + scaledResonanceValue * Type (-4) * (gain2 * saturationLUT (drive2 * s[4]) - dx * comp);
	*/

	__m128 a = _mm_add_ps(in, _mm_mul_ps(f->C[1], _mm_mul_ps(m128_minus4, _mm_sub_ps(f->R[4], _mm_mul_ps(in, f->C[2]))))); // const auto a = dx + scaledResonanceValue * Type (-4) * (gain2 * saturationLUT (drive2 * s[4]) - dx * comp);  

	__m128 b = _mm_add_ps(_mm_add_ps(_mm_mul_ps(b1, f->R[0]), _mm_mul_ps(f->C[0], f->R[1])), _mm_mul_ps(b0, a)); // const auto b = b1 * s[0] + a1 * s[1] + b0 * a;
	__m128 c = _mm_add_ps(_mm_add_ps(_mm_mul_ps(b1, f->R[1]), _mm_mul_ps(f->C[0], f->R[2])), _mm_mul_ps(b0, b)); // const auto c = b1 * s[1] + a1 * s[2] + b0 * b;
	__m128 d = _mm_add_ps(_mm_add_ps(_mm_mul_ps(b1, f->R[2]), _mm_mul_ps(f->C[0], f->R[3])), _mm_mul_ps(b0, c)); // const auto d = b1 * s[2] + a1 * s[3] + b0 * c;
	__m128 e = _mm_add_ps(_mm_add_ps(_mm_mul_ps(b1, f->R[3]), _mm_mul_ps(f->C[0], f->R[4])), _mm_mul_ps(b0, d)); // const auto e = b1 * s[3] + a1 * s[4] + b0 * d;

	f->R[0] = a; // s[0] = a;
	f->R[1] = b; // s[1] = b;
	f->R[2] = c; // s[2] = c;
	f->R[3] = d; // s[3] = d;
	f->R[4] = e; // s[4] = e;
	__m128 out = _mm_add_ps(_mm_add_ps(_mm_add_ps(_mm_add_ps(_mm_mul_ps(a, f->C[3]), _mm_mul_ps(b, f->C[4])), _mm_mul_ps(c, f->C[5])), _mm_mul_ps(d, f->C[6])), _mm_mul_ps(e, f->C[7]));  // return a * A[0] + b * A[1] + c * A[2] + d * A[3] + e * A[4];
	return out;
}


__m128 VASTQFilter::IIR6VASTquad(VASTQFilterStepState* __restrict f, __m128 in)
{
	f->C[0] = _mm_add_ps(f->C[0], f->dC[0]); // c0
	f->C[1] = _mm_add_ps(f->C[1], f->dC[1]); // c1
	f->C[2] = _mm_add_ps(f->C[2], f->dC[2]); // c2
	f->C[3] = _mm_add_ps(f->C[3], f->dC[3]); // c3
	f->C[4] = _mm_add_ps(f->C[4], f->dC[4]); // c4
	f->C[5] = _mm_add_ps(f->C[5], f->dC[5]); // not used?
	f->C[6] = _mm_add_ps(f->C[6], f->dC[6]); // not used?

	//->CVASTIIRFilter.h
	//out = c0 * in + lv1;
	//lv1 = c1 * in - c3 * out + lv2;
	//lv2 = c2 * in - c4 * out;

	__m128 y = _mm_add_ps(_mm_mul_ps(f->C[0], in), f->R[0]); //out = c0 * in + lv1;

	//Nan check on y and zero out NaNs
	for (int i = 0; i < 4; i++) {
		if (isnan(get1f(y, i)) || isinf(get1f(y, i))) { //NaN check
			set1f(y, i, 0.f);
		}
	}

	f->R[0] = _mm_add_ps(_mm_sub_ps(_mm_mul_ps(f->C[1], in), _mm_mul_ps(f->C[3], y)), f->R[1]); 	//lv1 = c1 * in - c3 * out + lv2;
	f->R[1] = _mm_sub_ps(_mm_mul_ps(f->C[2], in), _mm_mul_ps(f->C[4], y)); 	//lv2 = c2 * in - c4 * out;

	//CHECK
	f->C[7] = _mm_add_ps(f->C[7], f->dC[7]); // Clipgain
	const __m128 m01 = _mm_set1_ps(0.1f);
	const __m128 m1 = _mm_set1_ps(1.0f);
	f->R[2] = _mm_max_ps(m01, _mm_sub_ps(m1, _mm_mul_ps(f->C[7], _mm_mul_ps(y, y))));
	return y; //out
}

__m128 VASTQFilter::IIR12WDFquad(VASTQFilterStepState* __restrict f, __m128 in)
{
	f->C[0] = _mm_add_ps(f->C[0], f->dC[0]); // E1 * sc
	f->C[1] = _mm_add_ps(f->C[1], f->dC[1]); // E2 * sc
	f->C[2] = _mm_add_ps(f->C[2], f->dC[2]); // -E1 / sc
	f->C[3] = _mm_add_ps(f->C[3], f->dC[3]); // -E2 / sc
	f->C[4] = _mm_add_ps(f->C[4], f->dC[4]); // C1
	f->C[5] = _mm_add_ps(f->C[5], f->dC[5]); // C2
	f->C[6] = _mm_add_ps(f->C[6], f->dC[6]); // D

	__m128 y = _mm_add_ps(_mm_add_ps(_mm_mul_ps(f->C[4], f->R[0]), _mm_mul_ps(f->C[6], in)),
		_mm_mul_ps(f->C[5], f->R[1]));
	__m128 t =
		_mm_add_ps(in, _mm_add_ps(_mm_mul_ps(f->C[2], f->R[0]), _mm_mul_ps(f->C[3], f->R[1])));

	__m128 s1 = _mm_add_ps(_mm_mul_ps(t, f->C[0]), f->R[0]);
	__m128 s2 = _mm_sub_ps(_mm_setzero_ps(), _mm_add_ps(_mm_mul_ps(t, f->C[1]), f->R[1]));

	// f->R[0] = s1;
	// f->R[1] = s2;

	f->R[0] = _mm_mul_ps(s1, f->R[2]);
	f->R[1] = _mm_mul_ps(s2, f->R[2]);

	f->C[7] = _mm_add_ps(f->C[7], f->dC[7]); // Clipgain
	const __m128 m01 = _mm_set1_ps(0.1f);
	const __m128 m1 = _mm_set1_ps(1.0f);
	f->R[2] = _mm_max_ps(m01, _mm_sub_ps(m1, _mm_mul_ps(f->C[7], _mm_mul_ps(y, y))));

	return y;
}

__m128 VASTQFilter::IIR12CFCquad(VASTQFilterStepState* __restrict f, __m128 in)
{
	// State-space with clipgain (2nd order, limit within register)

	f->C[0] = _mm_add_ps(f->C[0], f->dC[0]); // ar
	f->C[1] = _mm_add_ps(f->C[1], f->dC[1]); // ai
	f->C[2] = _mm_add_ps(f->C[2], f->dC[2]); // b1
	f->C[4] = _mm_add_ps(f->C[4], f->dC[4]); // c1
	f->C[5] = _mm_add_ps(f->C[5], f->dC[5]); // c2
	f->C[6] = _mm_add_ps(f->C[6], f->dC[6]); // d

	// y(i) = c1.*s(1) + c2.*s(2) + d.*x(i);
	// s1 = ar.*s(1) - ai.*s(2) + x(i);
	// s2 = ai.*s(1) + ar.*s(2);

	__m128 y = _mm_add_ps(_mm_add_ps(_mm_mul_ps(f->C[4], f->R[0]), _mm_mul_ps(f->C[6], in)),
		_mm_mul_ps(f->C[5], f->R[1]));
	__m128 s1 = _mm_add_ps(_mm_mul_ps(in, f->C[2]),
		_mm_sub_ps(_mm_mul_ps(f->C[0], f->R[0]), _mm_mul_ps(f->C[1], f->R[1])));
	__m128 s2 = _mm_add_ps(_mm_mul_ps(f->C[1], f->R[0]), _mm_mul_ps(f->C[0], f->R[1]));

	f->R[0] = _mm_mul_ps(s1, f->R[2]);
	f->R[1] = _mm_mul_ps(s2, f->R[2]);

	f->C[7] = _mm_add_ps(f->C[7], f->dC[7]); // Clipgain
	const __m128 m01 = _mm_set1_ps(0.1f);
	const __m128 m1 = _mm_set1_ps(1.0f);
	f->R[2] = _mm_max_ps(m01, _mm_sub_ps(m1, _mm_mul_ps(f->C[7], _mm_mul_ps(y, y))));

	return y;
}

__m128 VASTQFilter::IIR12CFLquad(VASTQFilterStepState* __restrict f, __m128 in)
{
	// State-space with softer limiter

	f->C[0] = _mm_add_ps(f->C[0], f->dC[0]); // (ar)
	f->C[1] = _mm_add_ps(f->C[1], f->dC[1]); // (ai)
	f->C[2] = _mm_add_ps(f->C[2], f->dC[2]); // b1
	f->C[4] = _mm_add_ps(f->C[4], f->dC[4]); // c1
	f->C[5] = _mm_add_ps(f->C[5], f->dC[5]); // c2
	f->C[6] = _mm_add_ps(f->C[6], f->dC[6]); // d

	// y(i) = c1.*s(1) + c2.*s(2) + d.*x(i);
	// s1 = ar.*s(1) - ai.*s(2) + x(i);
	// s2 = ai.*s(1) + ar.*s(2);

	__m128 y = _mm_add_ps(_mm_add_ps(_mm_mul_ps(f->C[4], f->R[0]), _mm_mul_ps(f->C[6], in)),
		_mm_mul_ps(f->C[5], f->R[1]));
	__m128 ar = _mm_mul_ps(f->C[0], f->R[2]);
	__m128 ai = _mm_mul_ps(f->C[1], f->R[2]);
	__m128 s1 = _mm_add_ps(_mm_mul_ps(in, f->C[2]),
		_mm_sub_ps(_mm_mul_ps(ar, f->R[0]), _mm_mul_ps(ai, f->R[1])));
	__m128 s2 = _mm_add_ps(_mm_mul_ps(ai, f->R[0]), _mm_mul_ps(ar, f->R[1]));

	f->R[0] = s1;
	f->R[1] = s2;

	/*m = 1 ./ max(1,abs(y(i)));
	mr = mr.*0.99 + m.*0.01;*/

	// Limiter
	const __m128 m001 = _mm_set1_ps(0.001f);
	const __m128 m099 = _mm_set1_ps(0.999f);
	const __m128 m1 = _mm_set1_ps(1.0f);
	const __m128 m2 = _mm_set1_ps(2.0f);

	__m128 m = _mm_rsqrt_ps(_mm_max_ps(m1, _mm_mul_ps(m2, _mm_and_ps(y, m128_mask_absval))));
	f->R[2] = _mm_add_ps(_mm_mul_ps(f->R[2], m099), _mm_mul_ps(m, m001));

	return y;
}

__m128 VASTQFilter::IIR24CFCquad(VASTQFilterStepState* __restrict f, __m128 in)
{
	// State-space with clipgain (2nd order, limit within register)

	f->C[0] = _mm_add_ps(f->C[0], f->dC[0]); // ar
	f->C[1] = _mm_add_ps(f->C[1], f->dC[1]); // ai
	f->C[2] = _mm_add_ps(f->C[2], f->dC[2]); // b1

	f->C[4] = _mm_add_ps(f->C[4], f->dC[4]); // c1
	f->C[5] = _mm_add_ps(f->C[5], f->dC[5]); // c2
	f->C[6] = _mm_add_ps(f->C[6], f->dC[6]); // d

	__m128 y = _mm_add_ps(_mm_add_ps(_mm_mul_ps(f->C[4], f->R[0]), _mm_mul_ps(f->C[6], in)),
		_mm_mul_ps(f->C[5], f->R[1]));
	__m128 s1 = _mm_add_ps(_mm_mul_ps(in, f->C[2]),
		_mm_sub_ps(_mm_mul_ps(f->C[0], f->R[0]), _mm_mul_ps(f->C[1], f->R[1])));
	__m128 s2 = _mm_add_ps(_mm_mul_ps(f->C[1], f->R[0]), _mm_mul_ps(f->C[0], f->R[1]));

	f->R[0] = _mm_mul_ps(s1, f->R[2]);
	f->R[1] = _mm_mul_ps(s2, f->R[2]);

	__m128 y2 = _mm_add_ps(_mm_add_ps(_mm_mul_ps(f->C[4], f->R[3]), _mm_mul_ps(f->C[6], y)),
		_mm_mul_ps(f->C[5], f->R[4]));
	__m128 s3 = _mm_add_ps(_mm_mul_ps(y, f->C[2]),
		_mm_sub_ps(_mm_mul_ps(f->C[0], f->R[3]), _mm_mul_ps(f->C[1], f->R[4])));
	__m128 s4 = _mm_add_ps(_mm_mul_ps(f->C[1], f->R[3]), _mm_mul_ps(f->C[0], f->R[4]));

	f->R[3] = _mm_mul_ps(s3, f->R[2]);
	f->R[4] = _mm_mul_ps(s4, f->R[2]);

	f->C[7] = _mm_add_ps(f->C[7], f->dC[7]); // Clipgain
	const __m128 m01 = _mm_set1_ps(0.1f);
	const __m128 m1 = _mm_set1_ps(1.0f);
	f->R[2] = _mm_max_ps(m01, _mm_sub_ps(m1, _mm_mul_ps(f->C[7], _mm_mul_ps(y2, y2))));

	return y2;
}

__m128 VASTQFilter::IIR24CFLquad(VASTQFilterStepState* __restrict f, __m128 in)
{
	// State-space with softer limiter

	f->C[0] = _mm_add_ps(f->C[0], f->dC[0]); // (ar)
	f->C[1] = _mm_add_ps(f->C[1], f->dC[1]); // (ai)
	f->C[2] = _mm_add_ps(f->C[2], f->dC[2]); // b1
	f->C[4] = _mm_add_ps(f->C[4], f->dC[4]); // c1
	f->C[5] = _mm_add_ps(f->C[5], f->dC[5]); // c2
	f->C[6] = _mm_add_ps(f->C[6], f->dC[6]); // d

	__m128 ar = _mm_mul_ps(f->C[0], f->R[2]);
	__m128 ai = _mm_mul_ps(f->C[1], f->R[2]);

	__m128 y = _mm_add_ps(_mm_add_ps(_mm_mul_ps(f->C[4], f->R[0]), _mm_mul_ps(f->C[6], in)),
		_mm_mul_ps(f->C[5], f->R[1]));
	__m128 s1 = _mm_add_ps(_mm_mul_ps(in, f->C[2]),
		_mm_sub_ps(_mm_mul_ps(ar, f->R[0]), _mm_mul_ps(ai, f->R[1])));
	__m128 s2 = _mm_add_ps(_mm_mul_ps(ai, f->R[0]), _mm_mul_ps(ar, f->R[1]));

	f->R[0] = s1;
	f->R[1] = s2;

	__m128 y2 = _mm_add_ps(_mm_add_ps(_mm_mul_ps(f->C[4], f->R[3]), _mm_mul_ps(f->C[6], y)),
		_mm_mul_ps(f->C[5], f->R[4]));
	__m128 s3 = _mm_add_ps(_mm_mul_ps(y, f->C[2]),
		_mm_sub_ps(_mm_mul_ps(ar, f->R[3]), _mm_mul_ps(ai, f->R[4])));
	__m128 s4 = _mm_add_ps(_mm_mul_ps(ai, f->R[3]), _mm_mul_ps(ar, f->R[4]));

	f->R[3] = s3;
	f->R[4] = s4;

	/*m = 1 ./ max(1,abs(y(i)));
	mr = mr.*0.99 + m.*0.01;*/

	// Limiter
	const __m128 m001 = _mm_set1_ps(0.001f);
	const __m128 m099 = _mm_set1_ps(0.999f);
	const __m128 m1 = _mm_set1_ps(1.0f);
	const __m128 m2 = _mm_set1_ps(2.0f);

	__m128 m = _mm_rsqrt_ps(_mm_max_ps(m1, _mm_mul_ps(m2, _mm_and_ps(y2, m128_mask_absval))));
	f->R[2] = _mm_add_ps(_mm_mul_ps(f->R[2], m099), _mm_mul_ps(m, m001));

	return y2;
}

__m128 VASTQFilter::IIR24Bquad(VASTQFilterStepState* __restrict f, __m128 in)
{
	f->C[1] = _mm_add_ps(f->C[1], f->dC[1]); // K2
	f->C[3] = _mm_add_ps(f->C[3], f->dC[3]); // Q2
	f->C[0] = _mm_add_ps(f->C[0], f->dC[0]); // K1
	f->C[2] = _mm_add_ps(f->C[2], f->dC[2]); // Q1
	f->C[4] = _mm_add_ps(f->C[4], f->dC[4]); // V1
	f->C[5] = _mm_add_ps(f->C[5], f->dC[5]); // V2
	f->C[6] = _mm_add_ps(f->C[6], f->dC[6]); // V3

	__m128 f2 = _mm_sub_ps(_mm_mul_ps(f->C[3], in), _mm_mul_ps(f->C[1], f->R[1])); // Q2*in - K2*R1
	__m128 g2 = _mm_add_ps(_mm_mul_ps(f->C[1], in), _mm_mul_ps(f->C[3], f->R[1])); // K2*in + Q2*R1
	__m128 f1 = _mm_sub_ps(_mm_mul_ps(f->C[2], f2), _mm_mul_ps(f->C[0], f->R[0])); // Q1*f2 - K1*R0
	__m128 g1 = _mm_add_ps(_mm_mul_ps(f->C[0], f2), _mm_mul_ps(f->C[2], f->R[0])); // K1*f2 + Q1*R0
	f->R[0] = _mm_mul_ps(f1, f->R[4]);
	f->R[1] = _mm_mul_ps(g1, f->R[4]);
	__m128 y1 = _mm_add_ps(_mm_add_ps(_mm_mul_ps(f->C[6], g2), _mm_mul_ps(f->C[5], g1)),
		_mm_mul_ps(f->C[4], f1));

	f2 = _mm_sub_ps(_mm_mul_ps(f->C[3], y1), _mm_mul_ps(f->C[1], f->R[3])); // Q2*in - K2*R1
	g2 = _mm_add_ps(_mm_mul_ps(f->C[1], y1), _mm_mul_ps(f->C[3], f->R[3])); // K2*in + Q2*R1
	f1 = _mm_sub_ps(_mm_mul_ps(f->C[2], f2), _mm_mul_ps(f->C[0], f->R[2])); // Q1*f2 - K1*R0
	g1 = _mm_add_ps(_mm_mul_ps(f->C[0], f2), _mm_mul_ps(f->C[2], f->R[2])); // K1*f2 + Q1*R0
	f->R[2] = _mm_mul_ps(f1, f->R[4]);
	f->R[3] = _mm_mul_ps(g1, f->R[4]);
	__m128 y2 = _mm_add_ps(_mm_add_ps(_mm_mul_ps(f->C[6], g2), _mm_mul_ps(f->C[5], g1)),
		_mm_mul_ps(f->C[4], f1));

	f->C[7] = _mm_add_ps(f->C[7], f->dC[7]); // Clipgain
	const __m128 m01 = _mm_set1_ps(0.1f);
	const __m128 m1 = _mm_set1_ps(1.0f);
	f->R[4] = _mm_max_ps(m01, _mm_sub_ps(m1, _mm_mul_ps(f->C[7], _mm_mul_ps(y2, y2))));

	return y2;
}

__m128 VASTQFilter::LPMOOGquad(VASTQFilterStepState* __restrict f, __m128 in)
{
	f->C[0] = _mm_add_ps(f->C[0], f->dC[0]);
	f->C[1] = _mm_add_ps(f->C[1], f->dC[1]);
	f->C[2] = _mm_add_ps(f->C[2], f->dC[2]);

	f->R[0] = softclip8_ps(_mm_add_ps(
		f->R[0],
		_mm_mul_ps(f->C[1], _mm_sub_ps(_mm_sub_ps(_mm_mul_ps(in, f->C[0]),
			_mm_mul_ps(f->C[2], _mm_add_ps(f->R[3], f->R[4]))),
			f->R[0]))));
	f->R[1] = _mm_add_ps(f->R[1], _mm_mul_ps(f->C[1], _mm_sub_ps(f->R[0], f->R[1])));
	f->R[2] = _mm_add_ps(f->R[2], _mm_mul_ps(f->C[1], _mm_sub_ps(f->R[1], f->R[2])));
	f->R[4] = f->R[3];
	f->R[3] = _mm_add_ps(f->R[3], _mm_mul_ps(f->C[1], _mm_sub_ps(f->R[2], f->R[3])));

	return f->R[f->WP[0] & 3]; //f->WP[0] is number of poles!!  
}

__m128 VASTQFilter::SNHquad(VASTQFilterStepState* __restrict f, __m128 in)
{
	f->C[0] = _mm_add_ps(f->C[0], f->dC[0]);
	f->C[1] = _mm_add_ps(f->C[1], f->dC[1]);

	f->R[0] = _mm_add_ps(f->R[0], f->C[0]);

	__m128 mask = _mm_cmpgt_ps(f->R[0], _mm_setzero_ps());

	f->R[1] = _mm_or_ps(_mm_andnot_ps(mask, f->R[1]),
		_mm_and_ps(mask, softclip_ps(_mm_sub_ps(in, _mm_mul_ps(f->C[1], f->R[1])))));

	const __m128 m1 = _mm_set1_ps(-1.f);
	f->R[0] = _mm_add_ps(f->R[0], _mm_and_ps(m1, mask));

	return f->R[1];
}

#if !_M_X64
__m128 VASTQFilter::COMBquad_SSE1(VASTQFilterStepState* __restrict f, __m128 in)
{
	assert(FIRipol_M == 256); // changing the constant requires updating the code below
	const __m128 m256 = _mm_set1_ps(256.f);

	f->C[0] = _mm_add_ps(f->C[0], f->dC[0]);
	f->C[1] = _mm_add_ps(f->C[1], f->dC[1]);

	__m128 a = _mm_mul_ps(f->C[0], m256);
	__m128 DBRead = _mm_setzero_ps();

	for (int i = 0; i < 4; i++)
	{
		if (f->active[i])
		{
			int e = _mm_cvtss_si32(_mm_load_ss((float*)&a + i));
			int DT = e >> 8;
			int SE = (0xff - (e & 0xff)) * (FIRipol_N << 1);

			int RP = (f->WP[i] - DT - FIRoffset) & (MAX_FB_COMB - 1);

			// SINC interpolation (12 samples)
			__m128 a = _mm_loadu_ps(&f->DB[i][RP]);
			__m128 b = _mm_load_ps(&sinctable[SE]);
			__m128 o = _mm_mul_ps(a, b);

			a = _mm_loadu_ps(&f->DB[i][RP + 4]);
			b = _mm_load_ps(&sinctable[SE + 4]);
			o = _mm_add_ps(o, _mm_mul_ps(a, b));

			a = _mm_loadu_ps(&f->DB[i][RP + 8]);
			b = _mm_load_ps(&sinctable[SE + 8]);
			o = _mm_add_ps(o, _mm_mul_ps(a, b));

			_mm_store_ss((float*)&DBRead + i, sum_ps_to_ss(o));
		}
	}

	__m128 d = _mm_add_ps(in, _mm_mul_ps(DBRead, f->C[1]));
	d = softclip_ps(d);

	for (int i = 0; i < 4; i++)
	{
		if (f->active[i])
		{
			// Write to delaybuffer (with "anti-wrapping")
			__m128 t = _mm_load_ss((float*)&d + i);
			_mm_store_ss(&f->DB[i][f->WP[i]], t);
			if (f->WP[i] < FIRipol_N)
				_mm_store_ss(&f->DB[i][f->WP[i] + MAX_FB_COMB], t);

			// Increment write position
			f->WP[i] = (f->WP[i] + 1) & (MAX_FB_COMB - 1);
		}
	}
	return _mm_add_ps(_mm_mul_ps(f->C[3], DBRead), _mm_mul_ps(f->C[2], in));
}
#endif

__m128 VASTQFilter::COMBquad_SSE2(VASTQFilterStepState* __restrict f, __m128 in)
{
	assert(FIRipol_M == 256); // changing the constant requires updating the code below
	const __m128 m256 = _mm_set1_ps(256.f);
	const __m128i m0xff = _mm_set1_epi32(0xff);

	f->C[0] = _mm_add_ps(f->C[0], f->dC[0]);
	f->C[1] = _mm_add_ps(f->C[1], f->dC[1]);

	__m128 a = _mm_mul_ps(f->C[0], m256);
	__m128i e = _mm_cvtps_epi32(a);
	int DTi alignas(16)[4]{},
		SEi alignas(16)[4]{};	
	__m128i DT = _mm_srli_epi32(e, 8);
	_mm_store_si128((__m128i*)DTi, DT);
	__m128i SE = _mm_and_si128(e, m0xff);
	SE = _mm_sub_epi32(m0xff, SE);
	_mm_store_si128((__m128i*)SEi, SE);
	__m128 DBRead = _mm_setzero_ps();

	for (int i = 0; i < 4; i++)
	{
		if (f->active[i])
		{
			int RP = (f->WP[i] - DTi[i] - FIRoffset) & (MAX_FB_COMB - 1);

			// SINC interpolation (12 samples)
			__m128 a = _mm_loadu_ps(&f->DB[i][RP]);
			SEi[i] *= (FIRipol_N << 1);

			__m128 b = _mm_load_ps(&sinctable[SEi[i]]);
			__m128 o = _mm_mul_ps(a, b);
			a = _mm_loadu_ps(&f->DB[i][RP + 4]);
			b = _mm_load_ps(&sinctable[SEi[i] + 4]);
			o = _mm_add_ps(o, _mm_mul_ps(a, b));
			a = _mm_loadu_ps(&f->DB[i][RP + 8]);
			b = _mm_load_ps(&sinctable[SEi[i] + 8]);
			o = _mm_add_ps(o, _mm_mul_ps(a, b));
			_mm_store_ss((float*)&DBRead + i, sum_ps_to_ss(o));
		}
	}

	__m128 d = _mm_add_ps(in, _mm_mul_ps(DBRead, f->C[1]));
	d = softclip_ps(d);

	for (int i = 0; i < 4; i++)
	{
		if (f->active[i])
		{
			// Write to delaybuffer (with "anti-wrapping")
			__m128 t = _mm_load_ss((float*)&d + i);
			_mm_store_ss(&f->DB[i][f->WP[i]], t);
			if (f->WP[i] < FIRipol_N)
				_mm_store_ss(&f->DB[i][f->WP[i] + MAX_FB_COMB], t);

			// Increment write position
			f->WP[i] = (f->WP[i] + 1) & (MAX_FB_COMB - 1);
		}
	}
	return _mm_add_ps(_mm_mul_ps(f->C[3], DBRead), _mm_mul_ps(f->C[2], in));
}

__m128 VASTQFilter::CLIP(__m128 in, __m128 drive)
{
	const __m128 x_min = _mm_set1_ps(-1.0f);
	const __m128 x_max = _mm_set1_ps(1.0f);
	return _mm_max_ps(_mm_min_ps(_mm_mul_ps(in, drive), x_max), x_min);
}

__m128 VASTQFilter::DIGI_SSE2(__m128 in, __m128 drive)
{
	// v1.2: return (double)((int)((double)(x*p0inv*16.f+1.0)))*p0*0.0625f;
	const __m128 m16 = _mm_set1_ps(16.f);
	const __m128 m16inv = _mm_set1_ps(0.0625f);
	const __m128 mofs = _mm_set1_ps(0.5f);

	__m128 invdrive = _mm_rcp_ps(drive);
	__m128i a = _mm_cvtps_epi32(_mm_add_ps(mofs, _mm_mul_ps(invdrive, _mm_mul_ps(m16, in))));

	return _mm_mul_ps(drive, _mm_mul_ps(m16inv, _mm_sub_ps(_mm_cvtepi32_ps(a), mofs)));
}

#if !_M_X64
__m128 VASTQFilter::DIGI_SSE1(__m128 in, __m128 drive)
{
	const __m128 mofs = _mm_set1_ps(0.5f);
	const __m128 m16 = _mm_set1_ps(16.f);
	const __m128 m16inv = _mm_set1_ps(0.0625f);

	__m128 invdrive = _mm_rcp_ps(drive);
	__m128 a = _mm_add_ps(mofs, _mm_mul_ps(invdrive, _mm_mul_ps(m16, in)));
	__m64 a1 = _mm_cvtps_pi32(a);
	__m64 a2 = _mm_cvtps_pi32(_mm_movehl_ps(a, a));
	a = _mm_cvtpi32_ps(_mm_movelh_ps(a, _mm_cvtpi32_ps(a, a2)), a1);
	__m128 b = _mm_mul_ps(drive, _mm_mul_ps(m16inv, _mm_sub_ps(a, mofs)));
	_mm_empty();
	return b;
}
#endif

__m128 VASTQFilter::TANH(__m128 in, __m128 drive)
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

#if !_M_X64
__m128 VASTQFilter::SINUS_SSE1(__m128 in, __m128 drive)
{
	const __m128 one = _mm_set1_ps(1.f);
	const __m128 m256 = _mm_set1_ps(256.f);
	const __m128 m512 = _mm_set1_ps(512.f);

	__m128 x = _mm_mul_ps(in, drive);
	x = _mm_add_ps(_mm_mul_ps(x, m256), m512);

	__m64 e1 = _mm_cvtps_pi32(x);
	__m64 e2 = _mm_cvtps_pi32(_mm_movehl_ps(x, x));
	__m128 a = _mm_sub_ps(x, _mm_cvtpi32_ps(_mm_movelh_ps(x, _mm_cvtpi32_ps(x, e2)), e1));
	int e11 = *(int*)&e1;
	int e12 = *((int*)&e1 + 1);
	int e21 = *(int*)&e2;
	int e22 = *((int*)&e2 + 1);

	__m128 ws1 = _mm_load_ss(&waveshapers[3][e11 & 0x3ff]);
	__m128 ws2 = _mm_load_ss(&waveshapers[3][e12 & 0x3ff]);
	__m128 ws3 = _mm_load_ss(&waveshapers[3][e21 & 0x3ff]);
	__m128 ws4 = _mm_load_ss(&waveshapers[3][e22 & 0x3ff]);
	__m128 ws = _mm_movelh_ps(_mm_unpacklo_ps(ws1, ws2), _mm_unpacklo_ps(ws3, ws4));
	ws1 = _mm_load_ss(&waveshapers[3][(e11 + 1) & 0x3ff]);
	ws2 = _mm_load_ss(&waveshapers[3][(e12 + 1) & 0x3ff]);
	ws3 = _mm_load_ss(&waveshapers[3][(e21 + 1) & 0x3ff]);
	ws4 = _mm_load_ss(&waveshapers[3][(e22 + 1) & 0x3ff]);
	__m128 wsn = _mm_movelh_ps(_mm_unpacklo_ps(ws1, ws2), _mm_unpacklo_ps(ws3, ws4));

	x = _mm_add_ps(_mm_mul_ps(_mm_sub_ps(one, a), ws), _mm_mul_ps(a, wsn));

	_mm_empty();

	return x;
}
#endif
__m128 VASTQFilter::SINUS_SSE2(__m128 in, __m128 drive)
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
	short e4 alignas(16)[8]{};
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
/*
#if !_M_X64
__m128 VASTQFilter::ASYM_SSE1(__m128 in, __m128 drive)
{
	const __m128 one = _mm_set1_ps(1.f);
	const __m128 m32 = _mm_set1_ps(32.f);
	const __m128 m512 = _mm_set1_ps(512.f);
	const __m64 LB = _mm_set1_pi16(0);
	const __m64 UB = _mm_set1_pi16(0x3fe);

	__m128 x = _mm_mul_ps(in, drive);
	x = _mm_add_ps(_mm_mul_ps(x, m32), m512);

	__m64 e1 = _mm_cvtps_pi32(x);
	__m64 e2 = _mm_cvtps_pi32(_mm_movehl_ps(x, x));
	__m128 a = _mm_sub_ps(x, _mm_cvtpi32_ps(_mm_movelh_ps(x, _mm_cvtpi32_ps(x, e2)), e1));

	e1 = _mm_packs_pi32(e1, e2);
	e1 = _mm_max_pi16(_mm_min_pi16(e1, UB), LB);

	int e11 = *(int*)&e1;
	int e12 = *((int*)&e1 + 1);
	int e21 = *(int*)&e2;
	int e22 = *((int*)&e2 + 1);

	__m128 ws1 = _mm_load_ss(&waveshapers[2][e11 & 0x3ff]);
	__m128 ws2 = _mm_load_ss(&waveshapers[2][e12 & 0x3ff]);
	__m128 ws3 = _mm_load_ss(&waveshapers[2][e21 & 0x3ff]);
	__m128 ws4 = _mm_load_ss(&waveshapers[2][e22 & 0x3ff]);
	__m128 ws = _mm_movelh_ps(_mm_unpacklo_ps(ws1, ws2), _mm_unpacklo_ps(ws3, ws4));
	ws1 = _mm_load_ss(&waveshapers[2][(e11 + 1) & 0x3ff]);
	ws2 = _mm_load_ss(&waveshapers[2][(e12 + 1) & 0x3ff]);
	ws3 = _mm_load_ss(&waveshapers[2][(e21 + 1) & 0x3ff]);
	ws4 = _mm_load_ss(&waveshapers[2][(e22 + 1) & 0x3ff]);
	__m128 wsn = _mm_movelh_ps(_mm_unpacklo_ps(ws1, ws2), _mm_unpacklo_ps(ws3, ws4));

	x = _mm_add_ps(_mm_mul_ps(_mm_sub_ps(one, a), ws), _mm_mul_ps(a, wsn));

	_mm_empty();

	return x;
}
#endif
 */

__m128 VASTQFilter::ASYM_SSE2(__m128 in, __m128 drive)
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
	short e4 alignas(16)[8]{};
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

VASTQFilter::FilterUnitQFPtr VASTQFilter::GetQFPtrFilterUnit(int type, int subtype)
{
	switch (type)
	{
	case fut_vastiir6:
	{
		return &VASTQFilter::IIR6VASTquad;
	}
	case fut_vastsvf:
	{
		if (subtype == st_vastsvf_lp)
			return &VASTQFilter::VASTsvfLP;
		else if (subtype == st_vastsvf_hp)
			return &VASTQFilter::VASTsvfHP;
		else
			return &VASTQFilter::VASTsvfBP;
	}
	case fut_vastldf:
	{
		return &VASTQFilter::VASTldf;
	}
	case fut_lp12:
	{
		if (subtype == st_SVF)
			return &VASTQFilter::SVFLP12Aquad;
		else if (subtype == st_Rough)
			return &VASTQFilter::IIR12CFCquad;
		//			else if(subtype==st_Medium)
		//				return IIR12CFLquad;
		return &VASTQFilter::IIR12Bquad;
	}
	case fut_hp12:
	{
		if (subtype == st_SVF)
			return &VASTQFilter::SVFHP12Aquad;
		else if (subtype == st_Rough)
			return &VASTQFilter::IIR12CFCquad;
		//			else if(subtype==st_Medium)
		//				return IIR12CFLquad;
		return &VASTQFilter::IIR12Bquad;
	}
	case fut_bp12:
	{
		if (subtype == st_SVF)
			return &VASTQFilter::SVFBP12Aquad;
		else if (subtype == 3)
			return &VASTQFilter::SVFBP24Aquad;
		else if (subtype == st_Rough)
			return &VASTQFilter::IIR12CFCquad;
		//			else if(subtype==st_Medium)
		//				return SVFBP24Aquad;
		return &VASTQFilter::IIR12Bquad;
	}
	case fut_br12:
		return &VASTQFilter::IIR12Bquad;
	case fut_lp24:
		if (subtype == st_SVF)
			return &VASTQFilter::SVFLP24Aquad;
		else if (subtype == st_Rough)
			return &VASTQFilter::IIR24CFCquad;
		//		else if(subtype==st_Medium)
		//			return IIR24CFLquad;
		return &VASTQFilter::IIR24Bquad;
	case fut_hp24:
		if (subtype == st_SVF)
			return &VASTQFilter::SVFHP24Aquad;
		else if (subtype == st_Rough)
			return &VASTQFilter::IIR24CFCquad;
		//		else if(subtype==st_Medium)
		//			return IIR24CFLquad;
		return &VASTQFilter::IIR24Bquad;
	case fut_lpmoog:
		return &VASTQFilter::LPMOOGquad;
	case fut_SNH:
		return &VASTQFilter::SNHquad;
	case fut_comb:
		return &VASTQFilter::COMBquad_SSE2;

	case fut_vastscream:
	case fut_vastcomb:
	case fut_vastkrg12:
	default:
		vassertfalse;
	}
	return 0;
}

VASTQFilter::WaveshaperQFPtr VASTQFilter::GetQFPtrWaveshaper(int type)
{
	switch (type)
	{
	case wst_digi:
		return &VASTQFilter::DIGI_SSE2;
	case wst_hard:
		return &VASTQFilter::CLIP;
	case wst_sinus:
		return &VASTQFilter::SINUS_SSE2;
	case wst_asym:
		return &VASTQFilter::ASYM_SSE2;
	case wst_tanh:
		return &VASTQFilter::TANH;
	}
	return 0;
}

#define MWriteOutputs(x)                                                                           \
   d.OutL = _mm_add_ps(d.OutL, d.dOutL);                                                           \
   d.OutR = _mm_add_ps(d.OutR, d.dOutR);                                                           \
   __m128 outL = _mm_mul_ps(x, d.OutL);                                                            \
   __m128 outR = _mm_mul_ps(x, d.OutR);                                                            \
   _mm_store_ss(&OutL[k], _mm_add_ss(_mm_load_ss(&OutL[k]), sum_ps_to_ss(outL)));                  \
   _mm_store_ss(&OutR[k], _mm_add_ss(_mm_load_ss(&OutR[k]), sum_ps_to_ss(outR)));

#define MWriteOutputsDual(x, y)                                                                    \
   d.OutL = _mm_add_ps(d.OutL, d.dOutL);                                                           \
   d.OutR = _mm_add_ps(d.OutR, d.dOutR);                                                           \
   d.Out2L = _mm_add_ps(d.Out2L, d.dOut2L);                                                        \
   d.Out2R = _mm_add_ps(d.Out2R, d.dOut2R);                                                        \
   __m128 outL = vMAdd(x, d.OutL, vMul(y, d.Out2L));                                               \
   __m128 outR = vMAdd(x, d.OutR, vMul(y, d.Out2R));                                               \
   _mm_store_ss(&OutL[k], _mm_add_ss(_mm_load_ss(&OutL[k]), sum_ps_to_ss(outL)));                  \
   _mm_store_ss(&OutR[k], _mm_add_ss(_mm_load_ss(&OutR[k]), sum_ps_to_ss(outR)));				   \
   _mm_store_ps(&VoicesL[k][0], outL);															   \
   _mm_store_ps(&VoicesR[k][0], outR); 
//voices can be optimized - only needed when next filter

#ifdef _DEBUG
#define AssertReasonableAudioFloat(x) assert(x<32.f && x> - 32.f);
#else
#define AssertReasonableAudioFloat(x)
#endif

template <bool WS, bool A, bool B, bool C>
void VASTQFilter::ProcessFBQuad(VASTQFilterProcessState& d, Fbq_global& g, float* OutL, float* OutR, float VoicesL[][4], float VoicesR[][4], int numSamples)
{
	for (int k = 0; k < numSamples; k++)
	{
		d.FB = _mm_add_ps(d.FB, d.dFB); //TODO Check feedback - only to A?
		__m128 fbL = _mm_mul_ps(d.FB, d.FBlineL);
		__m128 fbR = _mm_mul_ps(d.FB, d.FBlineR);
		__m128 xin = _mm_add_ps(d.DL[k], softclip_ps(fbL));
		__m128 yin = _mm_add_ps(d.DR[k], softclip_ps(fbR));
		__m128 x = xin;
		__m128 y = yin;

		if (WS) //sequence changed
		{
			d.Drive = _mm_add_ps(d.Drive, d.dDrive);
			x = (this->*g.WSptr)(x, d.Drive);	//https://stackoverflow.com/questions/2402579/function-pointer-to-member-function
			y = (this->*g.WSptr)(y, d.Drive);
		}

		if (A)
		{
			x = (this->*g.FU1ptr)(&d.FU[0], x); //A left
			y = (this->*g.FU1ptr)(&d.FU[1], y); //A right
		}

		if (B)
		{
			x = (this->*g.FU2ptr)(&d.FU[2], x); //B left
			y = (this->*g.FU2ptr)(&d.FU[3], y); //B right
		}

		if (C)
		{
			x = (this->*g.FU3ptr)(&d.FU[4], x); //C left
			y = (this->*g.FU3ptr)(&d.FU[5], y); //C right
		}

		d.Gain = _mm_add_ps(d.Gain, d.dGain);
		__m128 mask = _mm_load_ps((float*)&d.FU[0].active); //why 0?
		x = _mm_and_ps(mask, _mm_mul_ps(x, d.Gain));
		y = _mm_and_ps(mask, _mm_mul_ps(y, d.Gain));

		//CHECK
		//if (std::isnan(get1f(x, 0)) || std::isinf(get1f(x, 0)))
			//vassertfalse;
		//x = softclip8_ps(x);
		//y = softclip8_ps(y);
		//CHECK

		d.FBlineL = x;
		d.FBlineR = y;

		// output stage
		MWriteOutputsDual(x, y)
			//AssertReasonableAudioFloat(OutL[k]);
			//AssertReasonableAudioFloat(OutR[k]);
	}

	//snap to zero - at least for vastsvf
	for (int filters = 0; filters < 6; filters++)
		for (int registers = 0; registers < n_filter_registers; registers++)
			snap_to_zero_ps(d.FU[filters].R[registers]);
}

VASTQFilter::FBQFPtr VASTQFilter::GetFBQPointer(bool WS, bool A, bool B, bool C)
{
	if (WS) {
		if (A)
		{
			if (B)
			{
				if (C)
					return &VASTQFilter::ProcessFBQuad<1, 1, 1, 1>;
				else
					return &VASTQFilter::ProcessFBQuad<1, 1, 1, 0>;
			}
			else
			{
				if (C)
					return &VASTQFilter::ProcessFBQuad<1, 1, 0, 1>;
				else
					return &VASTQFilter::ProcessFBQuad<1, 1, 0, 0>;
			}
		}
		else
		{
			if (B)
			{
				if (C)
					return &VASTQFilter::ProcessFBQuad<1, 0, 1, 1>;
				else
					return &VASTQFilter::ProcessFBQuad<1, 0, 1, 0>;
			}
			else
			{
				if (C)
					return &VASTQFilter::ProcessFBQuad<1, 0, 0, 1>;
				else
					return &VASTQFilter::ProcessFBQuad<1, 0, 0, 0>;
			}
		}
	}
	else {
		if (A)
		{
			if (B)
			{
				if (C)
					return &VASTQFilter::ProcessFBQuad<0, 1, 1, 1>;
				else
					return &VASTQFilter::ProcessFBQuad<0, 1, 1, 0>;
			}
			else
			{
				if (C)
					return &VASTQFilter::ProcessFBQuad<0, 1, 0, 1>;
				else
					return &VASTQFilter::ProcessFBQuad<0, 1, 0, 0>;
			}
		}
		else
		{
			if (B)
			{
				if (C)
					return &VASTQFilter::ProcessFBQuad<0, 0, 1, 1>;
				else
					return &VASTQFilter::ProcessFBQuad<0, 0, 1, 0>;
			}
			else
			{
				if (C)
					return &VASTQFilter::ProcessFBQuad<0, 0, 0, 1>;
				else
					return &VASTQFilter::ProcessFBQuad<0, 0, 0, 0>;
			}
		}
	}
	return 0;
}

void VASTQFilter::InitQFilterProcessStateToZero(VASTQFilterProcessState *Q)
{
	Q->Gain = _mm_setzero_ps();
	Q->FB = _mm_setzero_ps();
	Q->Mix1 = _mm_setzero_ps();
	Q->Mix2 = _mm_setzero_ps();
	Q->Drive = _mm_setzero_ps();
	Q->dGain = _mm_setzero_ps();
	Q->dFB = _mm_setzero_ps();
	Q->dMix1 = _mm_setzero_ps();
	Q->dMix2 = _mm_setzero_ps();
	Q->dDrive = _mm_setzero_ps();

	Q->wsLPF = _mm_setzero_ps();
	Q->FBlineL = _mm_setzero_ps();
	Q->FBlineR = _mm_setzero_ps();

	for (auto i = 0; i < BLOCK_SIZE_OS; ++i)
	{
		Q->DL[i] = _mm_setzero_ps();
		Q->DR[i] = _mm_setzero_ps();
	}

	Q->OutL = _mm_setzero_ps();
	Q->OutR = _mm_setzero_ps();
	Q->dOutL = _mm_setzero_ps();
	Q->dOutR = _mm_setzero_ps();
	Q->Out2L = _mm_setzero_ps();
	Q->Out2R = _mm_setzero_ps();
	Q->dOut2L = _mm_setzero_ps();
	Q->dOut2R = _mm_setzero_ps();
}

JUCE_END_IGNORE_WARNINGS_GCC_LIKE
JUCE_END_IGNORE_WARNINGS_MSVC
