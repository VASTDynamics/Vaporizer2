/*
VAST Dynamics Audio Software (TM)
*/

#pragma once

#include "VASTEnvelope.h"
#include "Filter/VASTCombFilter.h" 
#include "Filter/VASTDelay.h" 
#include "Filter/VASTBiQuadFilter.h" 
#include "Filter/VASTVAOnePoleFilter.h" 
#include "Filter/VASTVAOnePoleFilterEx.h" 
#include "Filter/VASTIIRFilter.h" 
#include "Filter/VASTVCFCombFilter.h"
#include "Oversampler/VASTOversampler.h"
#include "VASTEngineHeader.h"

/**************************/
#ifdef __aarch64__ //arm64
	#include "../sse2neon.h"
#else
	#include "emmintrin.h"
#endif
#include "Filter/VASTQFilterCoefficients.h" 
#include "Utils/VASTSynthfunctions.h" 
#include "Filter/VASTQFilter.h"

struct VASTQFilterProcessState; //forward declaration
class alignas(16) CVASTVcf
{
    
private:
    CVASTSettings* m_Set; //on top due to initializers list
    
public:
	CVASTVcf(CVASTSettings& set, MYUINT voiceNo, MYUINT filterNo, bool isUI);
	virtual ~CVASTVcf();
	void init();
	void prepareForPlay();

	//void processBlock(dsp::AudioBlock<float> filterBlock, modMatrixInputState* matrixInputState, sRoutingBuffers& routingBuffers, int startSample, int numSamples, bool isUI);

	//==========================================================================================================================

	//void calculateLPF(MYUINT iorder, double dCutoffFreq, double dQ);
	//void calculateLPFCoeffs(CVASTBiQuad* lBiquadL, CVASTBiQuad* lBiquadR, double fCutoffFreq, double fQ);
	//void calculateMassbergLPFCoeffs(double fCutoffFreq, double fQ);
	//void setLFOMod(float lfoMod); // -1..1

	//==========================================================================================================================
	//QUAD FILTER ************************************************************
	VASTQFilterCoefficients m_filterCoefficientMaker[3]; //0 filter A, 1 filter B, 2 filter C
	
	void GetQFB();
	void SetQFB(VASTQFilterProcessState* Q, int e, float gain, float drive, float feedback, int ftype, int fsubtype, int ftype2, int fsubtype2, int ftype3, int fsubtype3, int numSamples); // Q == 0 means init(ialise)

	struct
	{
		float Gain, FB, Mix1, Mix2, OutL, OutR, Out2L, Out2R, Drive, wsLPF, FBlineL, FBlineR;
		float Delay[6][MAX_FB_COMB + FIRipol_N];
		struct
		{
			float C[n_cm_coeffs], R[n_filter_registers];
			unsigned int WP;
			int type, subtype; // used for comparison with the last run
		} FU[6]; //0 Filter 1 L, 1 Filter 1 R, 2 Filter 2 L, 3 Filter 2 R, 4 Filter 3 L , 5 Filter 3 R	//was 0 Filter 1 L, 1 Filter 2 L, 1 Filter 2 R, 3 Filter 2 R
	} FBP; //this is allocated immediately because name is at the end

	VASTQFilterProcessState* fbq;
	int fbqi;	
	//QUAD FILTER ************************************************************

	/*
	// LPF Reso
	CVASTBiQuad m_LeftLPF_Order12;  // think about destroying instances when not needed?
	CVASTBiQuad m_RightLPF_Order12;

	//testing
	// LPF Reso
	CVASTBiQuad m_LeftLPF_Order34;  // think about destroying instances when not needed?
	CVASTBiQuad m_RightLPF_Order34;

	CVASTBiQuad m_LeftLPF_Order56;  // think about destroying instances when not needed?
	CVASTBiQuad m_RightLPF_Order56;
	*/

	// HPF 
	bool KorgThreeFiveHPFmkIIprepareForPlay();
	void KorgThreeFiveHPFmkIIupdateFilters();
	void KorgThreeFiveHPFmkIIdoFilter(double* pInputBuffer, double* pOutputBuffer);

	CVAOnePoleFilter m_TFHPF_LHPF1;
	CVAOnePoleFilter m_TFHPF_LHPF3;
	CVAOnePoleFilter m_TFHPF_LLPF1;
	CVAOnePoleFilter m_TFHPF_RHPF1;
	CVAOnePoleFilter m_TFHPF_RHPF3;
	CVAOnePoleFilter m_TFHPF_RLPF1;
	double m_TFHPF_dFc;
	double m_TFHPF_dK;
	double m_TFHPF_dSaturation;
	MYUINT m_TFHPF_uNonLinearProcessing;
	// HPF

	enum{ OFF, ON };
	double alpha0; // our G value
	enum{ LPF1, HPF1 }; // one short string for each

	// soft switching
	//void LPFReset();
	void HPFReset();
	//void VADLReset();

	void resetSmoothers();
	bool m_bSmoothersTakeNextValue = false;
	LinearSmoothedValue<float> m_fCutoff_smoothed;
	LinearSmoothedValue<float> m_fResonance_smoothed;
	LinearSmoothedValue<float> m_fScale_smoothed;

	CVASTVCFCombFilter m_combFilter;
	CVASTOversampler m_Oversampler;
    std::unique_ptr<AudioSampleBuffer> inBufferUp;
	dsp::LadderFilter<float> m_ladderFilter1;

private:
	MYUINT m_voiceNo = 0;
	MYUINT m_filterNo = 0;

	float mLPFCutoffFreqStored = 0; //to detect if changed
	float mLPFQStored = 0;
	float mHPFCutoffFreqStored = 0; //to detect if changed
	float mHPFQStored = 0;
	float mVADLCutoffFreqStored = 0; //to detect if changed

	float m_attenuateFactor = 1.f;
	bool m_useOversampling = true;

	LinearSmoothedValue<float> m_fDrive_smoothed; //not per voice?
	LinearSmoothedValue<float> m_fGain_smoothed; //not per voice?

	/*
	dsp::StateVariableFilter::Filter<float> m_dspStateVariableFilterL1;
	dsp::StateVariableFilter::Filter<float> m_dspStateVariableFilterR1;
	dsp::StateVariableFilter::Filter<float> m_dspStateVariableFilterL2;
	dsp::StateVariableFilter::Filter<float> m_dspStateVariableFilterR2;
	dsp::StateVariableFilter::Filter<float> m_dspStateVariableFilterL3;
	dsp::StateVariableFilter::Filter<float> m_dspStateVariableFilterR3;
	//dsp::ProcessorDuplicator<dsp::IIR::Filter<float>, dsp::IIR::Coefficients<float>> m_iir;
	*/

	//dsp::LadderFilter<float> m_ladderFilter2;

	//std::unique_ptr<dsp::LadderFilter<dsp::SIMDRegister<float>>> m_ladderFilterSimd1;;
	
	/*
	CVASTIIRFilter m_bqEndLowShelfL1;
	CVASTIIRFilter m_bqEndLowShelfR1;
	CVASTIIRFilter m_bqEndLowShelfL2;
	CVASTIIRFilter m_bqEndLowShelfR2;

	CVASTIIRFilter m_bqFilterL1;
	CVASTIIRFilter m_bqFilterR1;
	CVASTIIRFilter m_bqFilterL2;
	CVASTIIRFilter m_bqFilterR2;
	CVASTIIRFilter m_bqFilterL3;
	CVASTIIRFilter m_bqFilterR3;
	IIRCoefficients coeff; //Coefficient variable
	*/

	MYUINT m_paramType = 999;
	String msType = "";
	bool mbFilter2 = false;
	bool mbFilter3 = false;

	bool bfirstRun = true;

	JUCE_LEAK_DETECTOR(CVASTVcf)
};
