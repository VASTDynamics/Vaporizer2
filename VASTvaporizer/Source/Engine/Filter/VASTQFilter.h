#pragma once
#include "../Utils/VASTSynthfunctions.h"
//#include "VASTQFilterProcess.h"
#include "../VASTSynthesiser.h"
#include "../VASTSingleNote.h"
#include "../VASTEngineHeader.h"
//#include "../VASTVcf.h"
#include "VASTVCFCombFilter.h"
#ifdef __aarch64__ //arm64
	#include "../../sse2neon.h"
#else
	#include "emmintrin.h"
#endif

//not allocated structs can be global
struct VASTQFilterStepState
{
	__m128 C[n_cm_coeffs], dC[n_cm_coeffs]; // coefficients
	__m128 R[n_filter_registers];           // registers

	float* DB[4];                           // delay buffers //CHVAST
	//float DB[4][MAX_FB_COMB + FIRipol_N];                           // delay buffers

	int active[4]; // 0xffffffff if voice is active, 0 if not (usable as mask)
	int WP[4];     // comb write position
};

struct VASTQFilterProcessState // [3][4] exists 3 (filter) * 4 (voice registers) times
{
	VASTQFilterStepState FU[6]; //3 & 4 are for Filter B, 5 & 6 for filter C

	__m128 Gain, FB, Mix1, Mix2, Drive;
	__m128 dGain, dFB, dMix1, dMix2, dDrive;

	__m128 wsLPF, FBlineL, FBlineR;

	__m128 DL[BLOCK_SIZE_OS], DR[BLOCK_SIZE_OS]; // wavedata, is that the right constant here?

	__m128 OutL, OutR, dOutL, dOutR;
	__m128 Out2L, Out2R, dOut2L, dOut2R; // fb_stereo only
};

extern float waveshapers alignas(16)[8][1024];
extern float sinctable alignas(16)[(FIRipol_M + 1) * FIRipol_N * 2];
extern float sinctable1X alignas(16)[(FIRipol_M + 1) * FIRipol_N];
extern short sinctableI16 alignas(16)[(FIRipol_M + 1) * FIRipolI16_N];
class alignas(16) CVASTVcf; //forward declaration

class alignas(16) VASTQFilter
{
public:
	VASTQFilter();
	~VASTQFilter();
	void initQuadFilter(CVASTSettings* m_Set);
	int processBlock(OwnedArray<VASTSynthesiserVoice>* voices, modMatrixInputState* matrixInputState, sRoutingBuffers* routingBuffers, CVASTSettings* m_Set, int filter, dsp::AudioBlock<float> filterBlock, int startSample, int numSamples, bool isUI, bool hasNextFilter, std::unique_ptr<CVASTVcf>* uiVCF, bool warmup);
	int processQFilter(dsp::AudioBlock<float> filterBlock, sRoutingBuffers* routingBuffers, int numSamples, int startSample, int ftype, int fsubtype, int ftype2, int fsubtype2, int ftype3, int fsubtype3, int fws, int osFactor, OwnedArray<VASTSynthesiserVoice>* voices, int filter, float gain, float drive, float feedback, bool isUI, bool hasNextFilter, std::unique_ptr<CVASTVcf>* uiVCF, bool warmup);
	void calcFilterCoefficients(bool isUi, int ftype, int fsubtype, int ftype2, int fsubtype2, int ftype3, int fsubtype3, int osFactor, float fVCFEnvelopeMod, double &cutoff1, double &resonance1, double &scale1, double &cutoff2, double &resonance2, double &scale2, double &cutoff3, double &resonance3, double &scale3, int samplerate, VASTQFilterCoefficients* coeffMaker1, VASTQFilterCoefficients* coeffMaker2);
	void calcFilterSettings(bool isUi, int ftype, int fsubtype, int ftype2, int fsubtype2, int ftype3, int fsubtype3, int osFactor, double &drive, double &feedback, double &gain);
	void filterGetVariables(int paramType, int &ftype, int &fsubtype, int &ftype2, int &fsubtype2, int &ftype3, int &fsubtype3, int &fws, int &fOsFactor, float &fAttenuateFactor, bool &fParameterSmoothing);
	void filterTypeChanged(OwnedArray<VASTSynthesiserVoice>* voices, int filter, int ftype, int fsubtype, int ftype2, int fsubtype2, int ftype3, int fsubtype3, int osFactor, bool isUI, std::unique_ptr<CVASTVcf>* uiVCF, CVASTSettings* m_Set);
	void updateVariables();

	bool QFirstRun = false;
	double shafted_tanh(double x);
	int m_lastOsFactor = 1;

	//TESTING from VASTQFilterStep.h
	typedef __m128 (VASTQFilter::*FilterUnitQFPtr)(VASTQFilterStepState*, __m128); //see https://stackoverflow.com/questions/6265851/typedef-syntax-with-member-function-pointers
	typedef __m128 (VASTQFilter::*WaveshaperQFPtr)(__m128, __m128);

	FilterUnitQFPtr GetQFPtrFilterUnit(int type, int subtype);
	WaveshaperQFPtr GetQFPtrWaveshaper(int type);

	//SSE Intrinsic Cheat Sheet (SSE3)
	//https://gist.github.com/detomon/fb9db687b154d67dbb50
	__m128 SVFLP12Aquad(VASTQFilterStepState* __restrict f, __m128 in);
	__m128 SVFLP24Aquad(VASTQFilterStepState* __restrict f, __m128 in);
	__m128 SVFHP24Aquad(VASTQFilterStepState* __restrict f, __m128 in);
	__m128 SVFBP24Aquad(VASTQFilterStepState* __restrict f, __m128 in);
	__m128 SVFHP12Aquad(VASTQFilterStepState* __restrict f, __m128 in);
	__m128 SVFBP12Aquad(VASTQFilterStepState* __restrict f, __m128 in);
	__m128 IIR12Aquad(VASTQFilterStepState* __restrict f, __m128 in);
	__m128 IIR12Bquad(VASTQFilterStepState* __restrict f, __m128 in);
	__m128 VASTsvfLP(VASTQFilterStepState* __restrict f, __m128 in);
	__m128 VASTsvfHP(VASTQFilterStepState* __restrict f, __m128 in);
	__m128 VASTsvfBP(VASTQFilterStepState* __restrict f, __m128 in);
	__m128 VASTldf(VASTQFilterStepState* __restrict f, __m128 in);
	__m128 IIR6VASTquad(VASTQFilterStepState* __restrict f, __m128 in);
	__m128 IIR12WDFquad(VASTQFilterStepState* __restrict f, __m128 in);
	__m128 IIR12CFCquad(VASTQFilterStepState* __restrict f, __m128 in);
	__m128 IIR12CFLquad(VASTQFilterStepState* __restrict f, __m128 in);
	__m128 IIR24CFCquad(VASTQFilterStepState* __restrict f, __m128 in);
	__m128 IIR24CFLquad(VASTQFilterStepState* __restrict f, __m128 in);
	__m128 IIR24Bquad(VASTQFilterStepState* __restrict f, __m128 in);
	__m128 LPMOOGquad(VASTQFilterStepState* __restrict f, __m128 in);
	__m128 SNHquad(VASTQFilterStepState* __restrict f, __m128 in);
	__m128 COMBquad_SSE1(VASTQFilterStepState* __restrict f, __m128 in);
	__m128 COMBquad_SSE2(VASTQFilterStepState* __restrict f, __m128 in);
	__m128 CLIP(__m128 in, __m128 drive);
	__m128 DIGI_SSE2(__m128 in, __m128 drive);
	__m128 DIGI_SSE1(__m128 in, __m128 drive);
	__m128 TANH(__m128 in, __m128 drive);
	__m128 SINUS_SSE1(__m128 in, __m128 drive);
	__m128 SINUS_SSE2(__m128 in, __m128 drive);
	__m128 ASYM_SSE1(__m128 in, __m128 drive);
	__m128 ASYM_SSE2(__m128 in, __m128 drive);
	// TESTING

	//TESTING from VASTQFilterProcess.h
	/*
	** I originally had this as a member but since moved it out of line so as to
	** not run any risk of alignment problems in VASTQFilterChainState where
	** only the head of the array is __align_malloced
	*/
	void InitQFilterProcessStateToZero(VASTQFilterProcessState *Q);

	struct Fbq_global
	{
		FilterUnitQFPtr FU1ptr, FU2ptr, FU3ptr; //Filter A, B, C 
		WaveshaperQFPtr WSptr;
	};

	typedef void (VASTQFilter::*FBQFPtr)(VASTQFilterProcessState&, Fbq_global&, float*, float*, float[][4], float[][4], int);
	template <bool WS, bool A, bool B, bool C>
	void ProcessFBQuad(VASTQFilterProcessState& d, Fbq_global& g, float* OutL, float* OutR, float VoicesL[][4], float VoicesR[][4], int numSamples);
	FBQFPtr GetFBQPointer(bool WS, bool A, bool B, bool C);
	//

	VASTQFilterProcessState* FBQ[3]; //3 filters
	FBQFPtr ProcessQuadFB{};

    std::atomic<bool> m_bInitFilterAfterMaxPolyChange[3] = {false,false,false};
    
private:
    std::unique_ptr<AudioSampleBuffer> inBufferUp;
	int m_paramType[3]{ 0,0,0 }; //per filter
	alignas(16) CDecimator m_Decimator[3]{}; //per filter
	alignas(16) CDecimator m_DecimatorVoices[3][C_MAX_POLY]{};
	void ProcessLegacy(dsp::AudioBlock<float> filterBlock, sRoutingBuffers* routingBuffers, int numSamples, int startSample, int ftype, int fsubtype, int ftype2, int fsubtype2, int ftype3, int fsubtype3, int fws, int osFactor, OwnedArray<VASTSynthesiserVoice>* voices, int filter, float gain, float drive, float feedback, bool isUI, bool hasNextFilter, std::unique_ptr<CVASTVcf>* uiVCF, bool warmup);
};
