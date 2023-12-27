/*
VAST Dynamics Audio Software (TM)
*/

#include "VASTVcf.h"
#include "VASTEngineHeader.h"
#include "VASTSettings.h"
#include "Utils/VASTSynthfunctions.h"  //for pow
#include "FX/VASTMultibandCompressor.h" //IIR Filter
#include "Filter/VASTVCFCombFilter.h"
#include "Oversampler/VASTOversampler.h"
#include <math.h>
#include "Filter/VASTQFilter.h"
//#include "Filter/VASTQFilterProcess.h"
//#include "Filter/VASTQFilterStep.h"

#define C_FILTER_BLOCKSIZE 16
#define C_FILTER_OVERSAMPLINGFACTOR 4

CVASTVcf::CVASTVcf(CVASTSettings& set, MYUINT voiceNo, MYUINT filterNo, bool isUI) :
	m_Set(&set), m_Oversampler(set), m_combFilter(set), m_filterCoefficientMaker {{&set}, {&set}, {&set}}
{
	m_voiceNo = voiceNo;
	m_filterNo = filterNo;

	inBufferUp = std::make_unique<AudioSampleBuffer>(2, C_FILTER_BLOCKSIZE * C_FILTER_OVERSAMPLINGFACTOR);
}

/* destructor()
Destroy variables allocated in the contructor()

*/
CVASTVcf::~CVASTVcf(void) {
}

void CVASTVcf::init() { //once

	// init
	//LPFReset();
	HPFReset();

	/*
	// Finish initializations here
	m_VADL_dGAMMA = 0.0;
	m_VADL_dK = 0.0;

	// our feedback S values (global)
	m_VADL_dSG1_LEFT = 0.0;
	m_VADL_dSG2_LEFT = 0.0;
	m_VADL_dSG3_LEFT = 0.0;
	m_VADL_dSG4_LEFT = 0.0;

	// Filter coeffs that are constant
	// set a0s
	m_VADL_LPF1_LEFT.m_da0 = 1.0;
	m_VADL_LPF2_LEFT.m_da0 = 0.5;
	m_VADL_LPF3_LEFT.m_da0 = 0.5;
	m_VADL_LPF4_LEFT.m_da0 = 0.5;

	// last LPF has no feedback path
	m_VADL_LPF4_LEFT.m_dGamma = 1.0;
	m_VADL_LPF4_LEFT.m_dDelta = 0.0;
	m_VADL_LPF4_LEFT.m_dEpsilon = 0.0;
	m_VADL_LPF4_LEFT.setFeedback(0.0);

	// our feedback S values (global)
	m_VADL_dSG1_RIGHT = 0.0;
	m_VADL_dSG2_RIGHT = 0.0;
	m_VADL_dSG3_RIGHT = 0.0;
	m_VADL_dSG4_RIGHT = 0.0;

	// Filter coeffs that are constant
	// set a0s
	m_VADL_LPF1_RIGHT.m_da0 = 1.0;
	m_VADL_LPF2_RIGHT.m_da0 = 0.5;
	m_VADL_LPF3_RIGHT.m_da0 = 0.5;
	m_VADL_LPF4_RIGHT.m_da0 = 0.5;

	// last LPF has no feedback path
	m_VADL_LPF4_RIGHT.m_dGamma = 1.0;
	m_VADL_LPF4_RIGHT.m_dDelta = 0.0;
	m_VADL_LPF4_RIGHT.m_dEpsilon = 0.0;
	m_VADL_LPF4_RIGHT.setFeedback(0.0);
	VADLReset();
	*/

	//m_LFOMod = 0.0f;

	//lpf
	mLPFCutoffFreqStored = -1.0;
	mLPFQStored = -1.0;

	//hpf
	m_TFHPF_dFc = 0.0;
	mHPFCutoffFreqStored = -1.0;
	m_TFHPF_dK = 0.0;
	mHPFQStored = -1.0;

	/*
	//VADL
	m_VADL_dFc = 0.0;
	mVADLCutoffFreqStored = -1.0;
	*/

	/*
	m_StateVariableFilter.setFilterType(0);
	m_StateVariableFilter.setSampleRate(m_Set->m_nSampleRate);
	m_StateVariableFilter.setIsActive(true);

	m_StateVariableFilter2.setFilterType(0);
	m_StateVariableFilter2.setSampleRate(m_Set->m_nSampleRate);
	m_StateVariableFilter2.setIsActive(true);
	*/

	//TEST
	dsp::ProcessSpec spec;
	//spec.maximumBlockSize = C_MAX_BUFFER_SIZE;
	spec.maximumBlockSize = C_FILTER_BLOCKSIZE * C_FILTER_OVERSAMPLINGFACTOR;
	spec.numChannels = 2;
	spec.sampleRate = m_Set->m_nSampleRate;
	//spec.sampleRate = m_Set->m_nSampleRate * C_FILTER_OVERSAMPLINGFACTOR;

	/*
	m_dspStateVariableFilterL1.reset();
	m_dspStateVariableFilterR1.reset();
	m_dspStateVariableFilterL2.reset();
	m_dspStateVariableFilterR2.reset();
	m_dspStateVariableFilterL3.reset();
	m_dspStateVariableFilterR3.reset();

	//m_dspStateVariableFilterL1.state = new dsp::StateVariableFilter::Parameters<float>;
	//m_dspStateVariableFilterL1.prepare(spec); //not needed when single proc
	//m_dspStateVariableFilterR1.prepare(spec); //not needed when single proc

	//m_iir.state = dsp::IIR::Coefficients<float>::makeLowPass(m_Set->m_nSampleRate, 440.0);
	//m_iir.prepare(spec);
	*/

	m_ladderFilter1.prepare(spec);
	//m_ladderFilter2.prepare(spec);
	m_ladderFilter1.setMode(dsp::LadderFilter<float>::Mode::LPF12);
	//m_ladderFilter2.setMode(dsp::LadderFilter<float>::Mode::LPF12);

	//TEST SIMD
	//m_ladderFilterSimd1->prepare(spec);	
	//m_ladderFilterSimd1->setMode(dsp::LadderFilter<dsp::SIMDRegister<float>>::Mode::LPF12);
	//TEST SIMD

	//COMB
	m_combFilter.needsInitBeforUsed();
	m_Oversampler.init();

	//m_bqFilterL1.setCoefficients(coeff.makeLowPass(m_Set->m_nSampleRate, 100.f));
	//m_bqFilterR1.setCoefficients(coeff.makeLowPass(m_Set->m_nSampleRate, 100.f));

	/*
	if (!isUI) {
		//m_bqEndLowShelfL.setCoefficients(coeff.makeLowShelf(m_Set->m_nSampleRate, 30.f, sqrt2over2, 0.16f));
		//m_bqEndLowShelfR.setCoefficients(coeff.makeLowShelf(m_Set->m_nSampleRate, 30.f, sqrt2over2, 0.16f));
		m_bqEndLowShelfL1.reset();
		m_bqEndLowShelfR1.reset();
		m_bqEndLowShelfL2.reset();
		m_bqEndLowShelfR2.reset();
		m_bqEndLowShelfL1.setCoefficients(coeff.makeHighPass(m_Set->m_nSampleRate, 35.f, sqrt2over2));
		m_bqEndLowShelfR1.setCoefficients(coeff.makeHighPass(m_Set->m_nSampleRate, 35.f, sqrt2over2));
		m_bqEndLowShelfL2.setCoefficients(coeff.makeHighPass(m_Set->m_nSampleRate, 35.f, sqrt2over2));
		m_bqEndLowShelfR2.setCoefficients(coeff.makeHighPass(m_Set->m_nSampleRate, 35.f, sqrt2over2));
	}
	*/

	//Quad filter
	memset(&FBP, 0, sizeof(FBP));
	bfirstRun = true; 
	m_filterCoefficientMaker[0].prepareForPlay(m_Set->m_nSampleRate, 1, m_Set->m_fMasterTune.load());
	m_filterCoefficientMaker[1].prepareForPlay(m_Set->m_nSampleRate, 1, m_Set->m_fMasterTune.load());
	m_filterCoefficientMaker[2].prepareForPlay(m_Set->m_nSampleRate, 1, m_Set->m_fMasterTune.load());
	
	SetQFB(0, 0, 0.f, 0.f, 0.f, 0, 0, 0, 0, 0, 0, 0); // init qfilter 
	   	  
	resetSmoothers();
}

void CVASTVcf::prepareForPlay() { //called on program change
	resetSmoothers();

	//LPF
	//LPFReset();

	//HPF
	KorgThreeFiveHPFmkIIprepareForPlay();
	
	//COMB
	m_combFilter.prepareToPlay(m_Set->m_nSampleRate * C_FILTER_OVERSAMPLINGFACTOR, m_Set->m_nExpectedSamplesPerBlock * C_FILTER_OVERSAMPLINGFACTOR); //check
	m_Oversampler.prepareForPlay();

	/*
	//VA Diode Ladder
	VADLReset();
	VADiodeLadderFilterUpdateFilter();
	*/

	//Quad filter
	m_filterCoefficientMaker[0].prepareForPlay(m_Set->m_nSampleRate, 1, m_Set->m_fMasterTune.load()); //for real samplerate!
	m_filterCoefficientMaker[1].prepareForPlay(m_Set->m_nSampleRate, 1, m_Set->m_fMasterTune.load()); //for real samplerate!
	m_filterCoefficientMaker[2].prepareForPlay(m_Set->m_nSampleRate, 1, m_Set->m_fMasterTune.load()); //for real samplerate!
	
	SetQFB(0, 0, 0.f, 0.f, 0.f, 0, 0, 0, 0, 0, 0, 0); // init qfilter 

	/*
	m_bqFilterL1.reset();
	m_bqFilterL2.reset();
	m_bqFilterL3.reset();
	m_bqFilterR1.reset();
	m_bqFilterR2.reset();
	m_bqFilterR3.reset();
	*/

	m_paramType = 999;
}

void CVASTVcf::resetSmoothers() {
	//const double smoothTime = 0.03; //initially 0.002
	const double smoothTime = 0.005; //initially 0.002
	m_fCutoff_smoothed.reset(m_Set->m_nSampleRate, smoothTime);
	m_fResonance_smoothed.reset(m_Set->m_nSampleRate, smoothTime);
	m_fDrive_smoothed.reset(m_Set->m_nSampleRate, smoothTime);
	m_fScale_smoothed.reset(m_Set->m_nSampleRate, smoothTime);
	
	m_bSmoothersTakeNextValue = true; //need both?
	bfirstRun = true; //need both?

	m_fGain_smoothed.reset(m_Set->m_nSampleRate, smoothTime * 80.f);

	//memset(&FBP, 0, sizeof(FBP)); //causes clicks when same voice is played repeatedly
	
	//clear registers
	/*
	for (int j = 0; j < 6; j++) {
		for (int i = 0; i < n_filter_registers; i++)
			FBP.FU[j].R[i] = 0.f;
	}
	*/

	m_filterCoefficientMaker[0].Reset(); //reset smoothers
	m_filterCoefficientMaker[1].Reset(); //reset smoothers
	m_filterCoefficientMaker[2].Reset(); //reset smoothers
}

/*
void CVASTVcf::setLFOMod(float lfoMod) { //-1 .. 1 - center 0
	m_LFOMod = lfoMod;
}
*/

//LPF
// function to calclate the filter coefficients

/*
void CVASTVcf::calculateLPF(MYUINT iorder, double dCutoffFreq, double dQ) {
	//update Splane // only at init? - calcualate q values for butterworth response http://www.earlevel.com/main/2016/09/29/cascading-filters/
	double qvals[10];
	int pairs = iorder >> 1;
	//int oddPoles = order & 1; //only even 
	double poleInc = M_PI / (double)iorder;

	// show coefficients
	double firstAngle = poleInc / double(2.0);
	for (int idx = 0; idx < pairs; idx++) {
		qvals[idx] = 1.0 / (2.0 * cos(firstAngle + idx * poleInc));
	}
	//update Splane

	//calculateLPFCoeffs(&m_LeftLPF_Order12, &m_RightLPF_Order12, dCutoffFreq, qvals[0] + dQ);
	//calculateLPFCoeffs(&m_LeftLPF_Order12, &m_RightLPF_Order12, dCutoffFreq, dQ);
	calculateLPFCoeffs(&m_LeftLPF_Order12, &m_RightLPF_Order12, dCutoffFreq, dQ * qvals[0]);
	calculateLPFCoeffs(&m_LeftLPF_Order34, &m_RightLPF_Order34, dCutoffFreq, qvals[1]);
	calculateLPFCoeffs(&m_LeftLPF_Order56, &m_RightLPF_Order56, dCutoffFreq, qvals[2]);
}
*/

/*
void CVASTVcf::calculateLPFCoeffs(CVASTBiQuad* lBiquadL, CVASTBiQuad* lBiquadR, double dCutoffFreq, double dQ)
{
	
	//lBiquadL->calcBiquad(lBiquadL->LOWPASS, dCutoffFreq, (double)m_Set->m_nSampleRateOversampled, dQ, 0.0);
	//testing
	//return;
	

	// use same terms as in book:
	double theta_c = 2.0* M_PI *dCutoffFreq / (double)m_Set->m_nSampleRate;

	//CHTS
//	double d = 0.0f;
//	double q = dQ + 0.7f; // minimum resonance 0.7
	dQ = 1.0 / dQ; //CHECK

	// intermediate values
	double dBetaNumerator = 1.0 - ((dQ / 2.0)*(sin(theta_c)));
	double dBetaDenominator = 1.0 + ((dQ / 2.0)*(sin(theta_c)));

	// beta
	double dBeta = 0.0f;
	if (dBetaDenominator == 0.0f) dBeta = 100000.0f;
	else  dBeta = 0.5*(dBetaNumerator / dBetaDenominator); //check fBetaDenominator <> 0

	// gamma
	double dGamma = (0.5 + dBeta)*(cos(theta_c));

	// alpha
	double dAlpha = (0.5 + dBeta - dGamma) / 2.0;

	// left channel 
	lBiquadL->m_d_a0 = dAlpha;
	lBiquadL->m_d_a1 = 2.0 * dAlpha;
	lBiquadL->m_d_a2 = dAlpha;
	lBiquadL->m_d_b1 = -2.0 * dGamma;
	lBiquadL->m_d_b2 = 2.0 * dBeta;

	// right channel
	lBiquadR->m_d_a0 = lBiquadL->m_d_a0;
	lBiquadR->m_d_a1 = lBiquadL->m_d_a1;
	lBiquadR->m_d_a2 = lBiquadL->m_d_a2;
	lBiquadR->m_d_b1 = lBiquadL->m_d_b1;
	lBiquadR->m_d_b2 = lBiquadL->m_d_b2;

}
//--LPF
*/

/*
// calculate the Massberg coefficients for the 2nd order LPF
void CVASTVcf::calculateMassbergLPFCoeffs(double dCutoffFreq, double dQ)
{
	//CHTS check if necessary
	if ((dCutoffFreq == mLPFCutoffFreqStored) && (dQ == mLPFQStored))
		return;
	mLPFCutoffFreqStored = dCutoffFreq;
	mLPFQStored = dQ;

	//CHTS - avoid bump at < 100hz when using resonance
	double lQ = dQ; //range 0.5 - 20.0
	if (dCutoffFreq < 200.0) { // range 100-200
		lQ = (dCutoffFreq - 100.0) * 0.01f * lQ;
	}
	lQ = (lQ < 0.5f) ? 0.5f : lQ;

	// use same terms as in book:
	double omegaC = 2.0* M_PI  * dCutoffFreq / (double)m_Set->m_nSampleRate;

	// just to make the math a little easier to compare with the book
	double m = fastPrecisePow((fastPrecisePow((double)2.0, (double)0.5)*(double)M_PI) / omegaC, (double)2.0);
	double n = fastPrecisePow((double)2.0 * (double)M_PI / (lQ*omegaC), (double)2.0);
	double denom = fastPrecisePow(fastPrecisePow((double)2.0 - m, (double)2.0) + n, (double)0.5);

	// step 1: find g1 the gain at Nyquist
	double g1 = 2.0 / denom;

	// branch on Q
	double omegaR = 0;
	double omegaS = 0;
	double omegaM = 0;

	// if > 0.707
	if (lQ > fastPrecisePow(0.5, 0.5))
	{
		// resonant gain (standard equation)
		double gr = 2.0*fastPrecisePow(lQ, (double)2.0) / fastPrecisePow((double)4.0*fastPrecisePow(lQ, (double)2.0) - (double)1.0, (double)0.5);
		double wr = omegaC*fastPrecisePow(1.0 - (1.0 / (2.0*lQ*lQ)), 0.5);
		omegaR = tan(wr / 2.0); // NOTE this is wr/2fs in the paper - do fs cancel out from wn calc?

		double o = (gr*gr - g1*g1) / (gr*gr - 1.0);
		omegaS = omegaR*fastPrecisePow(o, (double)0.25); // cube root
	}
	else
	{
		double a = 2.0 - 1 / (lQ*lQ);
		double b = fastPrecisePow((double)1.0 - ((double)4.0*lQ*lQ) / lQ*lQ*lQ*lQ + (double)4.0 / g1, (double)0.5);

		double coeff = fastPrecisePow((a + b) / (double)2.0, (double)0.5);
		double wr = omegaC*coeff;

		omegaM = tan(wr / 2.0);
		omegaS = (omegaC*fastPrecisePow((1.0 - g1*g1), 0.25)) / 2.0;
		omegaS = fmin(omegaS, omegaM);
	}

	// calc peak freq
	double wp = (2.0)*atan(omegaS);

	double q = fastPrecisePow(wp / omegaC, (double)2.0);
	double r = fastPrecisePow(wp / (lQ*omegaC), (double)2.0);

	// calc gain at POLE
	double gp = 1.0 / fastPrecisePow(fastPrecisePow((double)1.0 - q, (double)2.0) + r, (double)0.5);

	// calculate gain at ZERO
	double wz = (2.0)*atan(omegaS / fastPrecisePow(g1, (double)0.5));

	q = fastPrecisePow(wz / omegaC, (double)2.0);
	r = fastPrecisePow(wz / (lQ*omegaC), (double)2.0);

	// calc gain at ZERO
	double gz = 1.0 / fastPrecisePow(fastPrecisePow((double)1.0 - q, (double)2.0) + r, (double)0.5);

	// calculalte required Q @ POLE
	double qp = fastPrecisePow((g1*(gp*gp - gz*gz)) / ((g1 + gz*gz)*(fastPrecisePow(g1 - (double)1.0, (double)2.0))), (double)0.5);

	//CHTS
	if ((isnan(qp)) || (qp == 0.0)) {//is NaN - wurzel aus negative
		calculateLPF(6, dCutoffFreq, dQ); ///Check
		return;
	}

	// calculalte required Q @ ZERO
	double num = g1*g1*(gp*gp - gz*gz);
	double den = gz*gz*(g1 + gp*gp)*(g1 - 1)*(g1 - 1);
	double qz = fastPrecisePow(num / den, (double)0.5);

	// finallyc calc the coeffs
	double a0 = omegaS*omegaS + omegaS / qp + 1.0;

	double alpha0 = omegaS*omegaS + omegaS*fastPrecisePow(g1, (double)0.5) / qz + g1;
	double alpha1 = 2.0*(omegaS*omegaS - g1);
	double alpha2 = omegaS*omegaS - omegaS*fastPrecisePow(g1, (double)0.5) / qz + g1;

	double beta1 = 2.0*(omegaS*omegaS - 1.0);
	double beta2 = omegaS*omegaS - omegaS / qp + 1.0;

	// left channel order 1
	m_LeftLPF_Order12.m_d_a0 = alpha0 / a0;
	m_LeftLPF_Order12.m_d_a1 = alpha1 / a0;
	m_LeftLPF_Order12.m_d_a2 = alpha2 / a0;
	m_LeftLPF_Order12.m_d_b1 = beta1 / a0;
	m_LeftLPF_Order12.m_d_b2 = beta2 / a0;

	// right channel order 1
	m_RightLPF_Order12.m_d_a0 = m_LeftLPF_Order12.m_d_a0;
	m_RightLPF_Order12.m_d_a1 = m_LeftLPF_Order12.m_d_a1;
	m_RightLPF_Order12.m_d_a2 = m_LeftLPF_Order12.m_d_a2;
	m_RightLPF_Order12.m_d_b1 = m_LeftLPF_Order12.m_d_b1;
	m_RightLPF_Order12.m_d_b2 = m_LeftLPF_Order12.m_d_b2;

	// left channel order 2
	m_LeftLPF_Order34.m_d_a0 = alpha0 / a0;
	m_LeftLPF_Order34.m_d_a1 = alpha1 / a0;
	m_LeftLPF_Order34.m_d_a2 = alpha2 / a0;
	m_LeftLPF_Order34.m_d_b1 = beta1 / a0;
	m_LeftLPF_Order34.m_d_b2 = beta2 / a0;

	// right channel order 2
	m_RightLPF_Order34.m_d_a0 = m_LeftLPF_Order34.m_d_a0;
	m_RightLPF_Order34.m_d_a1 = m_LeftLPF_Order34.m_d_a1;
	m_RightLPF_Order34.m_d_a2 = m_LeftLPF_Order34.m_d_a2;
	m_RightLPF_Order34.m_d_b1 = m_LeftLPF_Order34.m_d_b1;
	m_RightLPF_Order34.m_d_b2 = m_LeftLPF_Order34.m_d_b2;

	// left channel order 3
	m_LeftLPF_Order56.m_d_a0 = alpha0 / a0;
	m_LeftLPF_Order56.m_d_a1 = alpha1 / a0;
	m_LeftLPF_Order56.m_d_a2 = alpha2 / a0;
	m_LeftLPF_Order56.m_d_b1 = beta1 / a0;
	m_LeftLPF_Order56.m_d_b2 = beta2 / a0;

	// right channel order 3
	m_RightLPF_Order56.m_d_a0 = m_LeftLPF_Order56.m_d_a0;
	m_RightLPF_Order56.m_d_a1 = m_LeftLPF_Order56.m_d_a1;
	m_RightLPF_Order56.m_d_a2 = m_LeftLPF_Order56.m_d_a2;
	m_RightLPF_Order56.m_d_b1 = m_LeftLPF_Order56.m_d_b1;
	m_RightLPF_Order56.m_d_b2 = m_LeftLPF_Order56.m_d_b2;

	//CHTS -probably not needed
	if ((isnan(m_LeftLPF_Order12.m_d_a0)) || (isnan(m_RightLPF_Order12.m_d_a0)) ||
		(isnan(m_LeftLPF_Order12.m_d_a1)) || (isnan(m_RightLPF_Order12.m_d_a1))) {//is NaN - wurzel aus negative
		calculateLPF(6, dCutoffFreq, dQ); //CHECK
		return;
	}

	return;
}
*/


//HPF
bool CVASTVcf::KorgThreeFiveHPFmkIIprepareForPlay()
{
	// Add your code here:
	// set types
	m_TFHPF_LHPF1.m_dSampleRate = m_Set->m_nSampleRate;
	m_TFHPF_LLPF1.m_dSampleRate = m_Set->m_nSampleRate;
	m_TFHPF_LHPF3.m_dSampleRate = m_Set->m_nSampleRate;
	m_TFHPF_RHPF1.m_dSampleRate = m_Set->m_nSampleRate;
	m_TFHPF_RLPF1.m_dSampleRate = m_Set->m_nSampleRate;
	m_TFHPF_RHPF3.m_dSampleRate = m_Set->m_nSampleRate;

	m_TFHPF_LHPF1.m_uFilterType = HPF1;
	m_TFHPF_LLPF1.m_uFilterType = LPF1;
	m_TFHPF_LHPF3.m_uFilterType = HPF1;
	m_TFHPF_RHPF1.m_uFilterType = HPF1;
	m_TFHPF_RLPF1.m_uFilterType = LPF1;
	m_TFHPF_RHPF3.m_uFilterType = HPF1;

	// flush everything
	m_TFHPF_LHPF1.reset();
	m_TFHPF_LLPF1.reset();
	m_TFHPF_LHPF3.reset();
	m_TFHPF_RHPF1.reset();
	m_TFHPF_RLPF1.reset();
	m_TFHPF_RHPF3.reset();

	// set initial coeff states
	KorgThreeFiveHPFmkIIupdateFilters();

	return true;
}

JUCE_BEGIN_IGNORE_WARNINGS_GCC_LIKE("-Wconversion")
JUCE_BEGIN_IGNORE_WARNINGS_MSVC(4244 4267)

void CVASTVcf::KorgThreeFiveHPFmkIIupdateFilters()
{
	//CHTS check if necessary
	if ((m_TFHPF_dFc == mHPFCutoffFreqStored) && (m_TFHPF_dK == mHPFQStored))
		return;
	mHPFCutoffFreqStored = m_TFHPF_dFc;
	mHPFQStored = m_TFHPF_dK;


	// prewarp for BZT
	double wd = 2 * M_PI * m_TFHPF_dFc;
	double T = 1 / (double)m_Set->m_nSampleRate;
	double wa = (2 / T)*tan(wd*T / 2);
	double g = wa*T / 2;

	// G - the feedforward coeff in the VA One Pole
	double G = g / (1.0 + g);

	// set alphas
	m_TFHPF_LHPF1.m_dAlpha = G;
	m_TFHPF_LHPF1.m_dFc = m_TFHPF_dFc;
	m_TFHPF_LLPF1.m_dAlpha = G;
	m_TFHPF_LLPF1.m_dFc = m_TFHPF_dFc;
	m_TFHPF_LHPF3.m_dAlpha = G;
	m_TFHPF_LHPF3.m_dFc = m_TFHPF_dFc;

	m_TFHPF_RHPF1.m_dAlpha = G;
	m_TFHPF_RHPF1.m_dFc = m_TFHPF_dFc;
	m_TFHPF_RLPF1.m_dAlpha = G;
	m_TFHPF_RLPF1.m_dFc = m_TFHPF_dFc;
	m_TFHPF_RHPF3.m_dAlpha = G;
	m_TFHPF_RHPF3.m_dFc = m_TFHPF_dFc;

	// set betas all are in the form of  <something>/((1 + g)(1 - kG + kG^2))
	m_TFHPF_LHPF3.m_dBeta = -1.0*G / (1.0 + g);
	m_TFHPF_LLPF1.m_dBeta = 1.0 / (1.0 + g);

	m_TFHPF_RHPF3.m_dBeta = -1.0*G / (1.0 + g);
	m_TFHPF_RLPF1.m_dBeta = 1.0 / (1.0 + g);

	// set th G35H variable
	alpha0 = 1.0 / (1.0 - m_TFHPF_dK*G + m_TFHPF_dK*G*G);
}

void CVASTVcf::KorgThreeFiveHPFmkIIdoFilter(double* pInputBuffer, double* pOutputBuffer)
{
	// Left Channel

	// process input through HPF1
	double y1l = m_TFHPF_LHPF1.doFilter(pInputBuffer[0]);

	// then: form feedback and feed forward values (read before write)
	double S35Hl = m_TFHPF_LHPF3.getFeedbackOutput() + m_TFHPF_LLPF1.getFeedbackOutput();

	// y = G35x + S35
	double ul = alpha0*(y1l + S35Hl);

	double yl = m_TFHPF_dK*ul;

	// NAIVE NLP
	if (m_TFHPF_uNonLinearProcessing == ON)
		yl = tanh(m_TFHPF_dSaturation*yl);

	// process y through BPF
	m_TFHPF_LLPF1.doFilter(m_TFHPF_LHPF3.doFilter(yl));

	// auto-normalize
	if (m_TFHPF_dK > 0)
		yl *= 1 / m_TFHPF_dK;

	pOutputBuffer[0] = yl;

	// Right Channel

	// process input through HPF1
	double y1r = m_TFHPF_RHPF1.doFilter(pInputBuffer[1]);

	// then: form feedback and feed forward values (read before write)
	double S35Hr = m_TFHPF_RHPF3.getFeedbackOutput() + m_TFHPF_RLPF1.getFeedbackOutput();

	// y = G35x + S35
	double ur = alpha0*(y1r + S35Hr);

	double yr = m_TFHPF_dK*ur;

	// NAIVE NLP
	if (m_TFHPF_uNonLinearProcessing == ON)
		yr = tanh(m_TFHPF_dSaturation*yr);

	// process y through BPF
	m_TFHPF_RLPF1.doFilter(m_TFHPF_RHPF3.doFilter(yr));

	// auto-normalize
	if (m_TFHPF_dK > 0)
		yr *= 1 / m_TFHPF_dK;

	pOutputBuffer[1] = yr;

}
//--HPF

JUCE_END_IGNORE_WARNINGS_GCC_LIKE
JUCE_END_IGNORE_WARNINGS_MSVC

/*
//Diode Ladder Filter
void CVASTVcf::VADiodeLadderFilterUpdateFilter()
{
	//CHTS check if necessary
	if (m_VADL_dFc == mVADLCutoffFreqStored)
		return;
	mVADLCutoffFreqStored = m_VADL_dFc;

	// calculate alphas
	double wd = 2 * M_PI * m_VADL_dFc;
	double T = 1 / (double)m_Set->m_nSampleRate;
	double wa = (2 / T)*tan(wd*T / 2);
	double g = wa*T / 2;

	// Big G's
	double G1, G2, G3, G4;

	G4 = 0.5*g / (1.0 + g);
	G3 = 0.5*g / (1.0 + g - 0.5*g*G4);
	G2 = 0.5*g / (1.0 + g - 0.5*g*G3);
	G1 = g / (1.0 + g - g*G2);

	// our big G value GAMMA
	m_VADL_dGAMMA = G4*G3*G2*G1;

	//LEFT
	m_VADL_dSG1_LEFT = G4*G3*G2;
	m_VADL_dSG2_LEFT = G4*G3;
	m_VADL_dSG3_LEFT = G4;
	m_VADL_dSG4_LEFT = 1.0;

	// set alphas
	m_VADL_LPF1_LEFT.m_dAlpha = g / (1.0 + g);
	m_VADL_LPF2_LEFT.m_dAlpha = g / (1.0 + g);
	m_VADL_LPF3_LEFT.m_dAlpha = g / (1.0 + g);
	m_VADL_LPF4_LEFT.m_dAlpha = g / (1.0 + g);

	// set betas
	m_VADL_LPF1_LEFT.m_dBeta = 1.0 / (1.0 + g - g*G2);
	m_VADL_LPF2_LEFT.m_dBeta = 1.0 / (1.0 + g - 0.5*g*G3);
	m_VADL_LPF3_LEFT.m_dBeta = 1.0 / (1.0 + g - 0.5*g*G4);
	m_VADL_LPF4_LEFT.m_dBeta = 1.0 / (1.0 + g);

	// set gammas
	m_VADL_LPF1_LEFT.m_dGamma = 1.0 + G1*G2;
	m_VADL_LPF2_LEFT.m_dGamma = 1.0 + G2*G3;
	m_VADL_LPF3_LEFT.m_dGamma = 1.0 + G3*G4;
	// m_VADL_LPF4.m_dGamma = 1.0; // constant - done in constructor

	// set deltas
	m_VADL_LPF1_LEFT.m_dDelta = g;
	m_VADL_LPF2_LEFT.m_dDelta = 0.5*g;
	m_VADL_LPF3_LEFT.m_dDelta = 0.5*g;
	// m_VADL_LPF4.m_dDelta = 0.0; // constant - done in constructor

	// set epsilons
	m_VADL_LPF1_LEFT.m_dEpsilon = G2;
	m_VADL_LPF2_LEFT.m_dEpsilon = G3;
	m_VADL_LPF3_LEFT.m_dEpsilon = G4;
	// m_VADL_LPF4.m_dEpsilon = 0.0; // constant - done in constructor

	//RIGHT
	m_VADL_dSG1_RIGHT = G4*G3*G2;
	m_VADL_dSG2_RIGHT = G4*G3;
	m_VADL_dSG3_RIGHT = G4;
	m_VADL_dSG4_RIGHT = 1.0;

	// set alphas
	m_VADL_LPF1_RIGHT.m_dAlpha = g / (1.0 + g);
	m_VADL_LPF2_RIGHT.m_dAlpha = g / (1.0 + g);
	m_VADL_LPF3_RIGHT.m_dAlpha = g / (1.0 + g);
	m_VADL_LPF4_RIGHT.m_dAlpha = g / (1.0 + g);

	// set betas
	m_VADL_LPF1_RIGHT.m_dBeta = 1.0 / (1.0 + g - g*G2);
	m_VADL_LPF2_RIGHT.m_dBeta = 1.0 / (1.0 + g - 0.5*g*G3);
	m_VADL_LPF3_RIGHT.m_dBeta = 1.0 / (1.0 + g - 0.5*g*G4);
	m_VADL_LPF4_RIGHT.m_dBeta = 1.0 / (1.0 + g);

	// set gammas
	m_VADL_LPF1_RIGHT.m_dGamma = 1.0 + G1*G2;
	m_VADL_LPF2_RIGHT.m_dGamma = 1.0 + G2*G3;
	m_VADL_LPF3_RIGHT.m_dGamma = 1.0 + G3*G4;
	// m_VADL_LPF4.m_dGamma = 1.0; // constant - done in constructor

	// set deltas
	m_VADL_LPF1_RIGHT.m_dDelta = g;
	m_VADL_LPF2_RIGHT.m_dDelta = 0.5*g;
	m_VADL_LPF3_RIGHT.m_dDelta = 0.5*g;
	// m_VADL_LPF4.m_dDelta = 0.0; // constant - done in constructor

	// set epsilons
	m_VADL_LPF1_RIGHT.m_dEpsilon = G2;
	m_VADL_LPF2_RIGHT.m_dEpsilon = G3;
	m_VADL_LPF3_RIGHT.m_dEpsilon = G4;
	// m_VADL_LPF4.m_dEpsilon = 0.0; // constant - done in constructor
}

// do the filter
void CVASTVcf::VADiodeLadderFilterDoFilter(double* pInputBuffer, double* pOutputBuffer)
{
	double xn = pInputBuffer[0]; //TODO STEREO here;
	double yn = pInputBuffer[1]; //TODO STEREO here;

	//LEFT
	//	m_VADL_LPF4.setFeedback(0.0); // constant - done in constructor
	m_VADL_LPF3_LEFT.setFeedback(m_VADL_LPF4_LEFT.getFeedbackOutput());
	m_VADL_LPF2_LEFT.setFeedback(m_VADL_LPF3_LEFT.getFeedbackOutput());
	m_VADL_LPF1_LEFT.setFeedback(m_VADL_LPF2_LEFT.getFeedbackOutput());

	// form input
	double SIGMA_LEFT = m_VADL_dSG1_LEFT*m_VADL_LPF1_LEFT.getFeedbackOutput() +
		m_VADL_dSG2_LEFT*m_VADL_LPF2_LEFT.getFeedbackOutput() +
		m_VADL_dSG3_LEFT*m_VADL_LPF3_LEFT.getFeedbackOutput() +
		m_VADL_dSG4_LEFT*m_VADL_LPF4_LEFT.getFeedbackOutput();

	//RIGHT
	//	m_VADL_LPF4.setFeedback(0.0); // constant - done in constructor
	m_VADL_LPF3_RIGHT.setFeedback(m_VADL_LPF4_RIGHT.getFeedbackOutput());
	m_VADL_LPF2_RIGHT.setFeedback(m_VADL_LPF3_RIGHT.getFeedbackOutput());
	m_VADL_LPF1_RIGHT.setFeedback(m_VADL_LPF2_RIGHT.getFeedbackOutput());

	// form input
	double SIGMA_RIGHT = m_VADL_dSG1_RIGHT*m_VADL_LPF1_RIGHT.getFeedbackOutput() +
		m_VADL_dSG2_RIGHT*m_VADL_LPF2_RIGHT.getFeedbackOutput() +
		m_VADL_dSG3_RIGHT*m_VADL_LPF3_RIGHT.getFeedbackOutput() +
		m_VADL_dSG4_RIGHT*m_VADL_LPF4_RIGHT.getFeedbackOutput();

	// "cheap" nonlinear model; just process input
	if (m_VADL_uNonLinearProcessing == ON)
	{
		// the 1/tanh(sat) is to normalize the function so x = [-1..+1] --> y = [-1..+1]
		// Normalized Version
		if (m_VADL_uNLPType == NORM) {
			xn = (1.0 / tanh(m_VADL_dSaturation))*tanh(m_VADL_dSaturation*xn);
			yn = (1.0 / tanh(m_VADL_dSaturation))*tanh(m_VADL_dSaturation*yn);
		}
		else {
			xn = tanh(m_VADL_dSaturation*xn);
			yn = tanh(m_VADL_dSaturation*yn);
		}
	}

	// form the input to the loop
	double un = (xn - m_VADL_dK*SIGMA_LEFT) / (1 + m_VADL_dK*m_VADL_dGAMMA);

	// form the input to the loop
	double uyn = (yn - m_VADL_dK*SIGMA_RIGHT) / (1 + m_VADL_dK*m_VADL_dGAMMA);

	// cascade of series filters
	pOutputBuffer[0] = m_VADL_LPF4_LEFT.doFilter(m_VADL_LPF3_LEFT.doFilter(m_VADL_LPF2_LEFT.doFilter(m_VADL_LPF1_LEFT.doFilter(un))));
	pOutputBuffer[1] = m_VADL_LPF4_RIGHT.doFilter(m_VADL_LPF3_RIGHT.doFilter(m_VADL_LPF2_RIGHT.doFilter(m_VADL_LPF1_RIGHT.doFilter(uyn))));
}
//--Diode Ladder Filter
*/

/*
//--------------------------------------------------------------------------------------------
void CVASTVcf::processBlock(dsp::AudioBlock<float> filterBlock, modMatrixInputState* matrixInputState, sRoutingBuffers& routingBuffers, int startSample, int numSamples, bool isUI) {
	//VDBG("VCF do filter voice " + String(matrixInputState->voiceNo) + " " + String(startSample)  +" "  + String(numSamples));
	float fVCFEnvelopeMod = 0.f; //CHECK
	int paramType = 0;
	int paramFilterEnv = 0;	
	float paramFilterEnvMod = 0.f;
	int paramIDFilterEnvMod = 0;
	float paramDrive = 0.f;
	int paramIDDrive = 0;
	float paramCutoff = 0.f;
	int paramIDCutoff = 0;
	float paramResonance = 0.f;
	int paramIDResonance = 0;	
	float paramScale = 0.f;
	int paramIDScale = 0;
	float paramGain = 0.f;
	int paramIDGain = 0;
	
	switch (m_filterNo) {
	case(0):
		paramType = *m_Set->m_State->m_uFilterType_Filter1;
		paramDrive = *m_Set->m_State->m_fFilterDrive_Filter1;
		paramIDDrive= MODMATDEST::Filter1Drive;
		paramFilterEnv = *m_Set->m_State->m_uVCFEnv_Filter1;
		paramFilterEnvMod = *m_Set->m_State->m_fVCFEnvMod_Filter1;
		paramIDFilterEnvMod = MODMATDEST::Filter1EnvMod;
		paramCutoff = *m_Set->m_State->m_fFilterCutoff_Filter1;
		paramIDCutoff = MODMATDEST::Filter1Cutoff;
		paramResonance = *m_Set->m_State->m_fFilterReso_Filter1;
		paramIDResonance = MODMATDEST::Filter1Resonance;
		paramScale = *m_Set->m_State->m_fFilterScale_Filter1;
		paramIDScale = MODMATDEST::Filter1Scale;
		paramGain = *m_Set->m_State->m_fFilterGain_Filter1;
		paramIDGain = MODMATDEST::Filter1Gain;
		break;
	case(1):
		paramType = *m_Set->m_State->m_uFilterType_Filter2;
		paramDrive = *m_Set->m_State->m_fFilterDrive_Filter2;
		paramIDDrive = MODMATDEST::Filter2Drive;
		paramFilterEnv = *m_Set->m_State->m_uVCFEnv_Filter2;
		paramFilterEnvMod = *m_Set->m_State->m_fVCFEnvMod_Filter2;
		paramIDFilterEnvMod = MODMATDEST::Filter2EnvMod;
		paramCutoff = *m_Set->m_State->m_fFilterCutoff_Filter2;
		paramIDCutoff = MODMATDEST::Filter2Cutoff;
		paramResonance = *m_Set->m_State->m_fFilterReso_Filter2;
		paramIDResonance = MODMATDEST::Filter2Resonance;
		paramScale = *m_Set->m_State->m_fFilterScale_Filter2;
		paramIDScale = MODMATDEST::Filter2Scale;
		paramGain = *m_Set->m_State->m_fFilterGain_Filter2;
		paramIDGain = MODMATDEST::Filter2Gain;
		break;
	case(2):
		paramType = *m_Set->m_State->m_uFilterType_Filter3;
		paramDrive = *m_Set->m_State->m_fFilterDrive_Filter3;
		paramIDDrive = MODMATDEST::Filter3Drive;
		paramFilterEnv = *m_Set->m_State->m_uVCFEnv_Filter3;
		paramFilterEnvMod = *m_Set->m_State->m_fVCFEnvMod_Filter3;
		paramIDFilterEnvMod = MODMATDEST::Filter3EnvMod;
		paramCutoff = *m_Set->m_State->m_fFilterCutoff_Filter3;
		paramIDCutoff = MODMATDEST::Filter3Cutoff;
		paramResonance = *m_Set->m_State->m_fFilterReso_Filter3;
		paramIDResonance = MODMATDEST::Filter3Resonance;
		paramScale = *m_Set->m_State->m_fFilterScale_Filter3;
		paramIDScale = MODMATDEST::Filter3Scale;
		paramGain = *m_Set->m_State->m_fFilterGain_Filter3;
		paramIDGain = MODMATDEST::Filter3Gain;
		break;
	}

	if (isUI)
		m_bSmoothersTakeNextValue = true;

	bool bTypeChanged = (paramType != m_paramType);
	m_paramType = paramType;

	switch (paramType) {
		//===============================================================================================================================
	case(FILTERTYPE::BQFLPF6):  //BQF LPF 6
	case(FILTERTYPE::BQFHPF6):  //BQF HPF 6
	case(FILTERTYPE::BQFBPF6):  //BQF BPF 6
	case(FILTERTYPE::BQFALLPASS6):  //BQF ALLPASS 6
	case(FILTERTYPE::BQFLOWSHELF6):  //BQF LOWSHELF 6
	case(FILTERTYPE::BQFHIGHSHELF6):  //BQF HIGHSHELF 6
	case(FILTERTYPE::BQFNOTCH6):  //BQF NOTCH 6
	case(FILTERTYPE::BQFPEAK6):  //BQF PEAK 6
	case(FILTERTYPE::BQFLPF12):  //BQF LPF 12
	case(FILTERTYPE::BQFHPF12): //BQF HPF 12
	case(FILTERTYPE::BQFBPF12):  //BQF BPF 12
	case(FILTERTYPE::BQFALLPASS12):  //BQF ALLPASS 12
	case(FILTERTYPE::BQFLOWSHELF12):  //BQF LOWSHELF 12
	case(FILTERTYPE::BQFHIGHSHELF12):  //BQF HIGHSHELF 12
	case(FILTERTYPE::BQFNOTCH12):  //BQF NOTCH 12
	case(FILTERTYPE::BQFPEAK12):  //BQF PEAK 12
	case(FILTERTYPE::BQFLPF18):  //BQF LPF 18
	case(FILTERTYPE::BQFHPF18):  //BQF HPF 18
	case(FILTERTYPE::BQFBPF18):  //BQF BPF 18
	case(FILTERTYPE::BQFALLPASS18):  //BQF ALLPASS 18
	case(FILTERTYPE::BQFLOWSHELF18):  //BQF LOWSHELF 18
	case(FILTERTYPE::BQFHIGHSHELF18):  //BQF HIGHSHELF 18
	case(FILTERTYPE::BQFNOTCH18):  //BQF NOTCH 18
	case(FILTERTYPE::BQFPEAK18):  //BQF PEAK 18
	{
		if (bTypeChanged) {
			if (!isUI) {
				m_bqEndLowShelfL1.setCoefficients(coeff.makeHighPass(m_Set->m_nSampleRate, 35.f, sqrt2over2));
				m_bqEndLowShelfR1.setCoefficients(coeff.makeHighPass(m_Set->m_nSampleRate, 35.f, sqrt2over2));
				m_bqEndLowShelfL2.setCoefficients(coeff.makeHighPass(m_Set->m_nSampleRate, 35.f, sqrt2over2));
				m_bqEndLowShelfR2.setCoefficients(coeff.makeHighPass(m_Set->m_nSampleRate, 35.f, sqrt2over2));
			}

			switch (paramType) {
			case(FILTERTYPE::BQFLPF6): { //BQF LPF 6
				msType = "LPF";
				mbFilter2 = false;
				mbFilter3 = false;
				m_attenuateFactor = C_FILTER_OVERSAMPLINGFACTOR * 1.0f;
				m_useOversampling = true;
				break;
			}
			case(FILTERTYPE::BQFHPF6): { //BQF HPF 6
				msType = "HPF";
				mbFilter2 = false;
				mbFilter3 = false;
				m_attenuateFactor = 1.f;
				m_useOversampling = false;
				break;
			}
			case(FILTERTYPE::BQFBPF6): { //BQF BPF 6
				msType = "BPF";
				mbFilter2 = false;
				mbFilter3 = false;
				m_attenuateFactor = 1.f;
				m_useOversampling = false;
				break;
			}
			case(FILTERTYPE::BQFALLPASS6): { //BQF ALLPASS 6
				msType = "APF";
				mbFilter2 = false;
				mbFilter3 = false;
				m_attenuateFactor = 1.f;
				m_useOversampling = false;
				break;
			}
			case(FILTERTYPE::BQFLOWSHELF6): { //BQF LOWSHELF 6
				msType = "LSF";
				mbFilter2 = false;
				mbFilter3 = false;
				m_attenuateFactor = 1.f;
				m_useOversampling = false;
				break;
			}
			case(FILTERTYPE::BQFHIGHSHELF6): { //BQF HIGHSHELF 6
				msType = "HSF";
				mbFilter2 = false;
				mbFilter3 = false;
				m_attenuateFactor = 1.f;
				m_useOversampling = false;
				break;
			}
			case(FILTERTYPE::BQFNOTCH6): { //BQF NOTCH 6
				msType = "NF";
				mbFilter2 = false;
				mbFilter3 = false;
				m_attenuateFactor = 1.f;
				m_useOversampling = false;
				break;
			}
			case(FILTERTYPE::BQFPEAK6): { //BQF PEAK 6
				msType = "PF";
				mbFilter2 = false;
				mbFilter3 = false;
				m_attenuateFactor = 1.f;
				m_useOversampling = false;
				break;
			}
			case(FILTERTYPE::BQFLPF12): { //BQF LPF 12
				msType = "LPF";
				mbFilter2 = true;
				mbFilter3 = false;
				m_attenuateFactor = C_FILTER_OVERSAMPLINGFACTOR;
				m_useOversampling = true;
				break;
			}
			case(FILTERTYPE::BQFHPF12): { //BQF HPF 12
				msType = "HPF";
				mbFilter2 = true;
				mbFilter3 = false;
				m_attenuateFactor = 1.f;
				m_useOversampling = false;
				break;
			}
			case(FILTERTYPE::BQFBPF12): { //BQF BPF 12
				msType = "BPF";
				mbFilter2 = true;
				mbFilter3 = false;
				m_attenuateFactor = 1.f;
				m_useOversampling = false;
				break;
			}
			case(FILTERTYPE::BQFALLPASS12): { //BQF ALLPASS 12
				msType = "APF";
				mbFilter2 = true;
				mbFilter3 = false;
				m_attenuateFactor = 1.f;
				m_useOversampling = false;
				break;
			}
			case(FILTERTYPE::BQFLOWSHELF12): { //BQF LOWSHELF 12
				msType = "LSF";
				mbFilter2 = true;
				mbFilter3 = false;
				m_attenuateFactor = 1.f;
				m_useOversampling = false;
				break;
			}
			case(FILTERTYPE::BQFHIGHSHELF12): { //BQF HIGHSHELF 12
				msType = "HSF";
				mbFilter2 = true;
				mbFilter3 = false;
				m_attenuateFactor = 1.f;
				m_useOversampling = false;
				break;
			}
			case(FILTERTYPE::BQFNOTCH12): { //BQF NOTCH 12
				msType = "NF";
				mbFilter2 = true;
				mbFilter3 = false;
				m_attenuateFactor = 1.f;
				m_useOversampling = false;
				break;
			}
			case(FILTERTYPE::BQFPEAK12): { //BQF PEAK 12
				msType = "PF";
				mbFilter2 = true;
				mbFilter3 = false;
				m_attenuateFactor = 1.f;
				m_useOversampling = false;
				break;
			}
			case(FILTERTYPE::BQFLPF18): { //BQF LPF 18
				msType = "LPF";
				mbFilter2 = true;
				mbFilter3 = true;
				m_attenuateFactor = C_FILTER_OVERSAMPLINGFACTOR;
				m_useOversampling = true;
				break;
			}
			case(FILTERTYPE::BQFHPF18): { //BQF HPF 18
				msType = "HPF";
				mbFilter2 = true;
				mbFilter3 = true;
				m_attenuateFactor = 1.f;
				m_useOversampling = false;
				break;
			}
			case(FILTERTYPE::BQFBPF18): { //BQF BPF 18
				msType = "BPF";
				mbFilter2 = true;
				mbFilter3 = true;
				m_attenuateFactor = 1.f;
				m_useOversampling = false;
				break;
			}
			case(FILTERTYPE::BQFALLPASS18): { //BQF ALLPASS 18
				msType = "APF";
				mbFilter2 = true;
				mbFilter3 = true;
				m_attenuateFactor = 1.f;
				m_useOversampling = false;
				break;
			}
			case(FILTERTYPE::BQFLOWSHELF18): { //BQF LOWSHELF 18
				msType = "LSF";
				mbFilter2 = true;
				mbFilter3 = true;
				m_attenuateFactor = 1.f;
				m_useOversampling = false;
				break;
			}
			case(FILTERTYPE::BQFHIGHSHELF18): { //BQF HIGHSHELF 18
				msType = "HSF";
				mbFilter2 = true;
				mbFilter3 = true;
				m_attenuateFactor = 1.f;
				m_useOversampling = false;
				break;
			}
			case(FILTERTYPE::BQFNOTCH18): { //BQF NOTCH 18
				msType = "NF";
				mbFilter2 = true;
				mbFilter3 = true;
				m_attenuateFactor = 1.f;
				m_useOversampling = false;
				break;
			}
			case(FILTERTYPE::BQFPEAK18): { //BQF PEAK 18
				msType = "PF";
				mbFilter2 = true;
				mbFilter3 = true;
				m_attenuateFactor = 1.f;
				m_useOversampling = false;
				break;
			}
			}
		}
		
		float lDrive = paramDrive;
		float gainMult = 1.f;
		if (isUI)
			gainMult = fastPrecisePow(10.0, lDrive / 20.0);
		else {
			m_fDrive_smoothed.setValue(paramDrive, m_bSmoothersTakeNextValue);
			lDrive = m_fDrive_smoothed.getNextValue();
			m_fDrive_smoothed.skip(numSamples - 1);
			gainMult = fastPrecisePow(10.0, m_Set->getParameterValueWithMatrixModulation(&lDrive, paramIDDrive, matrixInputState) / 20.0);
		}
		if (gainMult != 1.f) {
			filterBlock.multiplyBy(gainMult);
			if (lDrive > 0.f) {
				float distortionLimit = 1.f;
				for (int i = startSample; i < numSamples; i++) {
					if (filterBlock.getChannelPointer(0)[i] > distortionLimit) filterBlock.getChannelPointer(0)[i] = distortionLimit;
					if (filterBlock.getChannelPointer(0)[i] < -distortionLimit) filterBlock.getChannelPointer(0)[i] = -distortionLimit;
					if (filterBlock.getChannelPointer(1)[i] > distortionLimit) filterBlock.getChannelPointer(1)[i] = distortionLimit;
					if (filterBlock.getChannelPointer(1)[i] < -distortionLimit) filterBlock.getChannelPointer(1)[i] = -distortionLimit;
				}
			}
		}

		float outputGain = paramGain * 0.01f;
		if (!isUI) {
			m_fGain_smoothed.setValue(m_Set->getParameterValueWithMatrixModulation(&paramGain, paramIDGain, matrixInputState), m_bSmoothersTakeNextValue);
			outputGain = static_cast<float> (m_fGain_smoothed.getNextValue() * 0.01f);
			m_fGain_smoothed.skip(numSamples - 1);
		}

		//float* lIn = filterBlock.getChannelPointer(0);
		//float* rIn = filterBlock.getChannelPointer(1);
		int i = 0;
		int samplesToProcess = numSamples;
		while (samplesToProcess >= C_FILTER_BLOCKSIZE) {
			matrixInputState->currentFrame = startSample + i;
			float lFilterEnvMod = paramFilterEnvMod;
			if (isUI) {
				fVCFEnvelopeMod = 1.0f;
			}
			else {
				if (paramFilterEnv == static_cast<int>(MSEGENV::MSEG1)) {
					fVCFEnvelopeMod = routingBuffers.MSEGBuffer[0][matrixInputState->voiceNo]->getSample(0, matrixInputState->currentFrame);
				}
				else if (paramFilterEnv == static_cast<int>(MSEGENV::MSEG2)) {
					fVCFEnvelopeMod = routingBuffers.MSEGBuffer[1][matrixInputState->voiceNo]->getSample(0, matrixInputState->currentFrame);
				}
				else if (paramFilterEnv == static_cast<int>(MSEGENV::MSEG3)) {
					fVCFEnvelopeMod = routingBuffers.MSEGBuffer[2][matrixInputState->voiceNo]->getSample(0, matrixInputState->currentFrame);
				}
				else if (paramFilterEnv == static_cast<int>(MSEGENV::MSEG4)) {
					fVCFEnvelopeMod = routingBuffers.MSEGBuffer[3][matrixInputState->voiceNo]->getSample(0, matrixInputState->currentFrame);
				}
				else if (paramFilterEnv == static_cast<int>(MSEGENV::MSEG5)) {
					fVCFEnvelopeMod = routingBuffers.MSEGBuffer[4][matrixInputState->voiceNo]->getSample(0, matrixInputState->currentFrame);
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

			double resonance = 0.0;
			double resonance_target = 0.0;
			double cutoff = 0.0;
			double cutoff_target = 0.0;
			double scale = 0.0;
			double scale_target = 0.0;
			if (isUI) {
				cutoff = float(paramCutoff * fVCFEnvelopeMod) + 30.f;
				resonance_target = paramResonance;
				//resonance = static_cast<float> (resonance_target + 0.3f);
				resonance = static_cast<float> (resonance_target * 0.1f); //10 is max - 100 is too much
				scale = ((paramScale + 1.f) / 101.f ) * 2.f; //must not be zero
			}
			else {
				cutoff_target = float(m_Set->getParameterValueWithMatrixModulation(&paramCutoff, paramIDCutoff, matrixInputState) * fVCFEnvelopeMod) + 30.f;
				m_fCutoff_smoothed.setValue(cutoff_target, m_bSmoothersTakeNextValue);
				cutoff = m_fCutoff_smoothed.getNextValue();
				m_fCutoff_smoothed.skip(C_FILTER_BLOCKSIZE * 0.25f - 1);
				resonance_target = m_Set->getParameterValueWithMatrixModulation(&paramResonance, paramIDResonance, matrixInputState);
				m_fResonance_smoothed.setValue(resonance_target, m_bSmoothersTakeNextValue);
				resonance = static_cast<float> (m_fResonance_smoothed.getNextValue() * 0.1f);
				m_fResonance_smoothed.skip(C_FILTER_BLOCKSIZE - 1);
				scale_target = m_Set->getParameterValueWithMatrixModulation(&paramScale, paramIDScale, matrixInputState);
				m_fScale_smoothed.setValue(scale_target, m_bSmoothersTakeNextValue);
				scale = static_cast<float> (((m_fScale_smoothed.getNextValue() + 1) / 101.f) * 2.f); //must not be zero - 0.0001 - 4.f
				m_fScale_smoothed.skip(C_FILTER_BLOCKSIZE - 1);
			}
			if (cutoff < 100.f) cutoff = 100.f;

			//for butterworth resonance of cascading filters: http://www.earlevel.com/main/2016/09/29/cascading-filters/
			//order 2:
			//			0.70710678	sqrt2over2
			//order 4:
			//			0.54119610
			//			1.3065630
			//order 6:
			//			0.51763809
			//			0.70710678	sqrt2over2
			//			1.9318517
			float resval1 = 0.f;
			float resval2 = 0.f;
			float resval3 = 0.f;
			if ((!mbFilter2) && (!mbFilter3)) { //order2
				resval1 = sqrt2over2 + resonance;
			}
			else if ((mbFilter2) && (!mbFilter3)) { //order4
				resval1 = 0.54119610f;
				resval2 = 1.3065630f + resonance;
			}
			else { //order 6
				resval1 = 0.51763809f;
				resval2 = sqrt2over2;
				resval3 = 1.9318517f + resonance;
			}

			if (m_useOversampling)
				cutoff /= C_FILTER_OVERSAMPLINGFACTOR;
			else
				cutoff = (cutoff < 22000.f) ? cutoff : 22000.f;

			if (msType == "LPF") {
				m_bqFilterL1.setCoefficients(coeff.makeLowPass(m_Set->m_nSampleRate, cutoff, resval1));
				m_bqFilterR1.setCoefficients(coeff.makeLowPass(m_Set->m_nSampleRate, cutoff, resval1));
			}
			else if (msType == "HPF") {
				m_bqFilterL1.setCoefficients(coeff.makeHighPass(m_Set->m_nSampleRate, cutoff, resval1));
				m_bqFilterR1.setCoefficients(coeff.makeHighPass(m_Set->m_nSampleRate, cutoff, resval1));
			}
			else if (msType == "BPF") {
				m_bqFilterL1.setCoefficients(coeff.makeBandPass(m_Set->m_nSampleRate, cutoff, resval1));
				m_bqFilterR1.setCoefficients(coeff.makeBandPass(m_Set->m_nSampleRate, cutoff, resval1));
			}
			else if (msType == "APF") {
				m_bqFilterL1.setCoefficients(coeff.makeAllPass(m_Set->m_nSampleRate, cutoff, resval1));
				m_bqFilterR1.setCoefficients(coeff.makeAllPass(m_Set->m_nSampleRate, cutoff, resval1));
			}
			else if (msType == "LSF") {
				m_bqFilterL1.setCoefficients(coeff.makeLowShelf(m_Set->m_nSampleRate, cutoff, resval1, scale)); 
				m_bqFilterR1.setCoefficients(coeff.makeLowShelf(m_Set->m_nSampleRate, cutoff, resval1, scale)); 
			}
			else if (msType == "HSF") {
				m_bqFilterL1.setCoefficients(coeff.makeHighShelf(m_Set->m_nSampleRate, cutoff, resval1, scale));
				m_bqFilterR1.setCoefficients(coeff.makeHighShelf(m_Set->m_nSampleRate, cutoff, resval1, scale));
			}
			else if (msType == "NF") {
				m_bqFilterL1.setCoefficients(coeff.makeNotchFilter(m_Set->m_nSampleRate, cutoff, resval1));
				m_bqFilterR1.setCoefficients(coeff.makeNotchFilter(m_Set->m_nSampleRate, cutoff, resval1));
			}
			else if (msType == "PF") {
				m_bqFilterL1.setCoefficients(coeff.makePeakFilter(m_Set->m_nSampleRate, cutoff, resval1, scale)); 
				m_bqFilterR1.setCoefficients(coeff.makePeakFilter(m_Set->m_nSampleRate, cutoff, resval1, scale)); 
			}

			if (mbFilter2) {
				if (msType == "LPF") {
					m_bqFilterL2.setCoefficients(coeff.makeLowPass(m_Set->m_nSampleRate, cutoff, resval2));
					m_bqFilterR2.setCoefficients(coeff.makeLowPass(m_Set->m_nSampleRate, cutoff, resval2));
				}
				else if (msType == "HPF") {
					m_bqFilterL2.setCoefficients(coeff.makeHighPass(m_Set->m_nSampleRate, cutoff, resval2));
					m_bqFilterR2.setCoefficients(coeff.makeHighPass(m_Set->m_nSampleRate, cutoff, resval2));
				}
				else if (msType == "BPF") {
					m_bqFilterL2.setCoefficients(coeff.makeBandPass(m_Set->m_nSampleRate, cutoff, resval2));
					m_bqFilterR2.setCoefficients(coeff.makeBandPass(m_Set->m_nSampleRate, cutoff, resval2));
				}
				else if (msType == "APF") {
					m_bqFilterL2.setCoefficients(coeff.makeAllPass(m_Set->m_nSampleRate, cutoff, resval2));
					m_bqFilterR2.setCoefficients(coeff.makeAllPass(m_Set->m_nSampleRate, cutoff, resval2));
				}
				else if (msType == "LSF") {
					m_bqFilterL2.setCoefficients(coeff.makeLowShelf(m_Set->m_nSampleRate, cutoff, resval2, scale)); 
					m_bqFilterR2.setCoefficients(coeff.makeLowShelf(m_Set->m_nSampleRate, cutoff, resval2, scale)); 
				}
				else if (msType == "HSF") {
					m_bqFilterL2.setCoefficients(coeff.makeHighShelf(m_Set->m_nSampleRate, cutoff, resval2, scale)); 
					m_bqFilterR2.setCoefficients(coeff.makeHighShelf(m_Set->m_nSampleRate, cutoff, resval2, scale)); 
				}
				else if (msType == "NF") {
					m_bqFilterL2.setCoefficients(coeff.makeNotchFilter(m_Set->m_nSampleRate, cutoff, resval2));
					m_bqFilterR2.setCoefficients(coeff.makeNotchFilter(m_Set->m_nSampleRate, cutoff, resval2));
				}
				else if (msType == "PF") {
					m_bqFilterL2.setCoefficients(coeff.makePeakFilter(m_Set->m_nSampleRate, cutoff, resval2, scale)); 
					m_bqFilterR2.setCoefficients(coeff.makePeakFilter(m_Set->m_nSampleRate, cutoff, resval2, scale)); 
				}
			}
			if (mbFilter3) {
				if (msType == "LPF") {
					m_bqFilterL3.setCoefficients(coeff.makeLowPass(m_Set->m_nSampleRate, cutoff, resval3));
					m_bqFilterR3.setCoefficients(coeff.makeLowPass(m_Set->m_nSampleRate, cutoff, resval3));
				}
				else if (msType == "HPF") {
					m_bqFilterL3.setCoefficients(coeff.makeHighPass(m_Set->m_nSampleRate, cutoff, resval3));
					m_bqFilterR3.setCoefficients(coeff.makeHighPass(m_Set->m_nSampleRate, cutoff, resval3));
				}
				else if (msType == "BPF") {
					m_bqFilterL3.setCoefficients(coeff.makeBandPass(m_Set->m_nSampleRate, cutoff, resval3));
					m_bqFilterR3.setCoefficients(coeff.makeBandPass(m_Set->m_nSampleRate, cutoff, resval3));
				}
				else if (msType == "APF") {
					m_bqFilterL3.setCoefficients(coeff.makeAllPass(m_Set->m_nSampleRate, cutoff, resval3));
					m_bqFilterR3.setCoefficients(coeff.makeAllPass(m_Set->m_nSampleRate, cutoff, resval3));
				}
				else if (msType == "LSF") {
					m_bqFilterL3.setCoefficients(coeff.makeLowShelf(m_Set->m_nSampleRate, cutoff, resval3, scale)); 
					m_bqFilterR3.setCoefficients(coeff.makeLowShelf(m_Set->m_nSampleRate, cutoff, resval3, scale)); 
				}
				else if (msType == "HSF") {
					m_bqFilterL3.setCoefficients(coeff.makeHighShelf(m_Set->m_nSampleRate, cutoff, resval3, scale)); 
					m_bqFilterR3.setCoefficients(coeff.makeHighShelf(m_Set->m_nSampleRate, cutoff, resval3, scale)); 
				}
				else if (msType == "NF") {
					m_bqFilterL3.setCoefficients(coeff.makeNotchFilter(m_Set->m_nSampleRate, cutoff, resval3));
					m_bqFilterR3.setCoefficients(coeff.makeNotchFilter(m_Set->m_nSampleRate, cutoff, resval3));
				}
				else if (msType == "PF") {
					m_bqFilterL3.setCoefficients(coeff.makePeakFilter(m_Set->m_nSampleRate, cutoff, resval3, scale)); 
					m_bqFilterR3.setCoefficients(coeff.makePeakFilter(m_Set->m_nSampleRate, cutoff, resval3, scale)); 
				}
			}

			if (!m_useOversampling) {
				m_bqFilterL1.processSamples(filterBlock.getSubBlock(i, C_FILTER_BLOCKSIZE).getChannelPointer(0), C_FILTER_BLOCKSIZE);
				m_bqFilterR1.processSamples(filterBlock.getSubBlock(i, C_FILTER_BLOCKSIZE).getChannelPointer(1), C_FILTER_BLOCKSIZE);
				if (mbFilter2) {
					m_bqFilterL2.processSamples(filterBlock.getSubBlock(i, C_FILTER_BLOCKSIZE).getChannelPointer(0), C_FILTER_BLOCKSIZE);
					m_bqFilterR2.processSamples(filterBlock.getSubBlock(i, C_FILTER_BLOCKSIZE).getChannelPointer(1), C_FILTER_BLOCKSIZE);
				}
				if (mbFilter3) {
					m_bqFilterL3.processSamples(filterBlock.getSubBlock(i, C_FILTER_BLOCKSIZE).getChannelPointer(0), C_FILTER_BLOCKSIZE);
					m_bqFilterR3.processSamples(filterBlock.getSubBlock(i, C_FILTER_BLOCKSIZE).getChannelPointer(1), C_FILTER_BLOCKSIZE);
				}
			}
			else { //oversample it
				//upsample
				dsp::AudioBlock<float> inBlock = filterBlock.getSubBlock(i, C_FILTER_BLOCKSIZE);
				dsp::AudioBlock<float> inBlockUp = dsp::AudioBlock<float>(*inBufferUp);

				for (int j = 0; j < C_FILTER_BLOCKSIZE * C_FILTER_OVERSAMPLINGFACTOR; j++) {
					if (j % C_FILTER_OVERSAMPLINGFACTOR == 0) {
						//inBlockUp.setSample(0, j, inBlock.getSample(0, j / C_FILTER_OVERSAMPLINGFACTOR));
						inBlockUp.setSample(0, j, inBlock.getSample(0, j / C_FILTER_OVERSAMPLINGFACTOR));
						inBlockUp.setSample(1, j, inBlock.getSample(1, j / C_FILTER_OVERSAMPLINGFACTOR));
					}
					else {
						inBlockUp.setSample(0, j, 0.f); //zero pad				
						inBlockUp.setSample(1, j, 0.f); //zero pad				
					}
				}
				//process
				m_bqFilterL1.processSamples(inBlockUp.getChannelPointer(0), C_FILTER_BLOCKSIZE * C_FILTER_OVERSAMPLINGFACTOR);
				m_bqFilterR1.processSamples(inBlockUp.getChannelPointer(1), C_FILTER_BLOCKSIZE * C_FILTER_OVERSAMPLINGFACTOR);
				if (mbFilter2) {
					m_bqFilterL2.processSamples(inBlockUp.getChannelPointer(0), C_FILTER_BLOCKSIZE * C_FILTER_OVERSAMPLINGFACTOR);
					m_bqFilterR2.processSamples(inBlockUp.getChannelPointer(1), C_FILTER_BLOCKSIZE * C_FILTER_OVERSAMPLINGFACTOR);
				}
				if (mbFilter3) {
					m_bqFilterL3.processSamples(inBlockUp.getChannelPointer(0), C_FILTER_BLOCKSIZE * C_FILTER_OVERSAMPLINGFACTOR);
					m_bqFilterR3.processSamples(inBlockUp.getChannelPointer(1), C_FILTER_BLOCKSIZE * C_FILTER_OVERSAMPLINGFACTOR);
				}

				//downsample
				for (int j = 0; j < C_FILTER_BLOCKSIZE * C_FILTER_OVERSAMPLINGFACTOR; j++) {
					if (j % C_FILTER_OVERSAMPLINGFACTOR == 0) {
						inBlock.setSample(0, j / C_FILTER_OVERSAMPLINGFACTOR, inBlockUp.getSample(0, j));
						inBlock.setSample(1, j / C_FILTER_OVERSAMPLINGFACTOR, inBlockUp.getSample(1, j));
					}
				}
			}
		
			samplesToProcess -= C_FILTER_BLOCKSIZE;
			i += C_FILTER_BLOCKSIZE;
			m_bSmoothersTakeNextValue = false;
		}
		if (samplesToProcess > 0) { //do rest
			if (!isUI) {
				m_fResonance_smoothed.skip(samplesToProcess);
				m_fCutoff_smoothed.skip(samplesToProcess);
			}

			if (!m_useOversampling) {
				m_bqFilterL1.processSamples(filterBlock.getSubBlock(i, samplesToProcess).getChannelPointer(0), samplesToProcess);
				m_bqFilterR1.processSamples(filterBlock.getSubBlock(i, samplesToProcess).getChannelPointer(1), samplesToProcess);
				if (mbFilter2) {
					m_bqFilterL2.processSamples(filterBlock.getSubBlock(i, samplesToProcess).getChannelPointer(0), samplesToProcess);
					m_bqFilterR2.processSamples(filterBlock.getSubBlock(i, samplesToProcess).getChannelPointer(1), samplesToProcess);
				}
				if (mbFilter3) {
					m_bqFilterL3.processSamples(filterBlock.getSubBlock(i, samplesToProcess).getChannelPointer(0), samplesToProcess);
					m_bqFilterR3.processSamples(filterBlock.getSubBlock(i, samplesToProcess).getChannelPointer(1), samplesToProcess);
				}
			}
			else {
				//upsample
				dsp::AudioBlock<float> inBlock = filterBlock.getSubBlock(i, samplesToProcess);
				dsp::AudioBlock<float> inBlockUp = dsp::AudioBlock<float>(*inBufferUp);

				for (int j = 0; j < samplesToProcess * C_FILTER_OVERSAMPLINGFACTOR; j++) {
					if (j % C_FILTER_OVERSAMPLINGFACTOR == 0) {
						inBlockUp.setSample(0, j, inBlock.getSample(0, j / C_FILTER_OVERSAMPLINGFACTOR));
						inBlockUp.setSample(1, j, inBlock.getSample(1, j / C_FILTER_OVERSAMPLINGFACTOR));
					}
					else {
						inBlockUp.setSample(0, j, 0.f); //zero pad				
						inBlockUp.setSample(1, j, 0.f); //zero pad				
					}
				}
				//process
				m_bqFilterL1.processSamples(inBlockUp.getChannelPointer(0), samplesToProcess * C_FILTER_OVERSAMPLINGFACTOR);
				m_bqFilterR1.processSamples(inBlockUp.getChannelPointer(1), samplesToProcess * C_FILTER_OVERSAMPLINGFACTOR);
				if (mbFilter2) {
					m_bqFilterL2.processSamples(inBlockUp.getChannelPointer(0), samplesToProcess * C_FILTER_OVERSAMPLINGFACTOR);
					m_bqFilterR2.processSamples(inBlockUp.getChannelPointer(1), samplesToProcess * C_FILTER_OVERSAMPLINGFACTOR);
				}
				if (mbFilter3) {
					m_bqFilterL3.processSamples(inBlockUp.getChannelPointer(0), samplesToProcess * C_FILTER_OVERSAMPLINGFACTOR);
					m_bqFilterR3.processSamples(inBlockUp.getChannelPointer(1), samplesToProcess * C_FILTER_OVERSAMPLINGFACTOR);
				}

				//downsample
				for (int j = 0; j < samplesToProcess * C_FILTER_OVERSAMPLINGFACTOR; j++) {
					if (j % C_FILTER_OVERSAMPLINGFACTOR == 0) {
						inBlock.setSample(0, j / C_FILTER_OVERSAMPLINGFACTOR, inBlockUp.getSample(0, j));
						inBlock.setSample(1, j / C_FILTER_OVERSAMPLINGFACTOR, inBlockUp.getSample(1, j));
					}
				}
			}
		}		

		//if (gainMult > 1.f) gainMult = 1.f + (gainMult - 1.f) * 0.5f;
		//if (gainMult != 0.f) filterBlock.multiply(1.0f / gainMult); //redo the original gain when using drive
		filterBlock.multiplyBy(m_attenuateFactor * outputGain);

		break;
	}

	//===============================================================================================================================
	case(FILTERTYPE::LDFLPF12):  //LDF LPF 12
	case(FILTERTYPE::LDFHPF12):  //LDF HPF 12
	case(FILTERTYPE::LDFLPF24):  //LDF LPF 24
	case(FILTERTYPE::LDFHPF24):  //LDF HPF 24
	case(FILTERTYPE::LDFLPF36):  //LDF LPF 36
	case(FILTERTYPE::LDFHPF36):  //LDF HPF 36
	case(FILTERTYPE::LDFLPF48):  //LDF LPF 48
	case(FILTERTYPE::LDFHPF48):  //LDF HPF 48
	{
		if (isUI) {
			bTypeChanged = true;
			m_ladderFilter1.reset();
			m_ladderFilter2.reset();
		}

		if (bTypeChanged) {
			if (!isUI) {
				m_bqEndLowShelfL1.setCoefficients(coeff.makeHighPass(m_Set->m_nSampleRate, 30.f, sqrt2over2));
				m_bqEndLowShelfR1.setCoefficients(coeff.makeHighPass(m_Set->m_nSampleRate, 30.f, sqrt2over2));
				m_bqEndLowShelfL2.setCoefficients(coeff.makeHighPass(m_Set->m_nSampleRate, 30.f, sqrt2over2));
				m_bqEndLowShelfR2.setCoefficients(coeff.makeHighPass(m_Set->m_nSampleRate, 30.f, sqrt2over2));
			}

			switch (paramType) {
			case(FILTERTYPE::LDFLPF12): { //LDF LPF 12
				m_ladderFilter1.setMode(dsp::LadderFilter<float>::Mode::LPF12);
				mbFilter2 = false;
				m_attenuateFactor = C_FILTER_OVERSAMPLINGFACTOR * 1.3f;
				m_useOversampling = true;
				break;
			}
			case(FILTERTYPE::LDFHPF12): { //LDF HPF 12
				m_ladderFilter1.setMode(dsp::LadderFilter<float>::Mode::HPF12);
				mbFilter2 = false;
				m_attenuateFactor = 1.f;
				m_useOversampling = false;
				break;
			}
			case(FILTERTYPE::LDFLPF24): { //LDF LPF 24
				m_ladderFilter1.setMode(dsp::LadderFilter<float>::Mode::LPF24);
				mbFilter2 = false;
				m_attenuateFactor = C_FILTER_OVERSAMPLINGFACTOR * 1.7f;
				m_useOversampling = true;
				break;
			}
			case(FILTERTYPE::LDFHPF24): { //LDF HPF 24
				m_ladderFilter1.setMode(dsp::LadderFilter<float>::Mode::HPF24);
				mbFilter2 = false;
				m_attenuateFactor = 1.f;
				m_useOversampling = false;
				break;
			}
			case(FILTERTYPE::LDFLPF36): { //LDF LPF 36
				m_ladderFilter1.setMode(dsp::LadderFilter<float>::Mode::LPF24);
				m_ladderFilter2.setMode(dsp::LadderFilter<float>::Mode::LPF12);
				mbFilter2 = true;
				m_attenuateFactor = C_FILTER_OVERSAMPLINGFACTOR * 1.7f;
				m_useOversampling = true;
				break;
			}
			case(FILTERTYPE::LDFHPF36): { //LDF HPF 36
				m_ladderFilter1.setMode(dsp::LadderFilter<float>::Mode::HPF24);
				m_ladderFilter2.setMode(dsp::LadderFilter<float>::Mode::HPF12);
				mbFilter2 = true;
				m_attenuateFactor = 1.f;
				m_useOversampling = false;
				break;
			}
			case(FILTERTYPE::LDFLPF48): { //LDF LPF 48
				m_ladderFilter1.setMode(dsp::LadderFilter<float>::Mode::LPF24);
				m_ladderFilter2.setMode(dsp::LadderFilter<float>::Mode::LPF24);
				mbFilter2 = true;
				m_attenuateFactor = C_FILTER_OVERSAMPLINGFACTOR * 1.7f;
				m_useOversampling = true;
				break;
			}
			case(FILTERTYPE::LDFHPF48): { //LDF HPF 48}
				m_ladderFilter1.setMode(dsp::LadderFilter<float>::Mode::HPF24);
				m_ladderFilter2.setMode(dsp::LadderFilter<float>::Mode::HPF24);
				mbFilter2 = true;
				m_attenuateFactor = 1.f;
				m_useOversampling = false;
				break;
			}
			}
		}

		float lDrive = paramDrive;
		float gainMult = 1.f;
		if (isUI)
			gainMult = fastPrecisePow(10.0, lDrive / 20.0);
		else {
			m_fDrive_smoothed.setValue(paramDrive, m_bSmoothersTakeNextValue);
			lDrive = m_fDrive_smoothed.getNextValue();
			m_fDrive_smoothed.skip(numSamples - 1);
			gainMult = fastPrecisePow(10.0, m_Set->getParameterValueWithMatrixModulation(&lDrive, paramIDDrive, matrixInputState) / 20.0);
		}
		//if (gainMult != 1.f) filterBlock.multiply(gainMult);

		if (gainMult != 1.f) {
			filterBlock.multiplyBy(gainMult);
			if (lDrive > 0.f) {
				float distortionLimit = 1.f;
				for (int i = startSample; i < numSamples; i++) {
					if (filterBlock.getChannelPointer(0)[i] > distortionLimit) filterBlock.getChannelPointer(0)[i] = distortionLimit;
					if (filterBlock.getChannelPointer(0)[i] < -distortionLimit) filterBlock.getChannelPointer(0)[i] = -distortionLimit;
					if (filterBlock.getChannelPointer(1)[i] > distortionLimit) filterBlock.getChannelPointer(1)[i] = distortionLimit;
					if (filterBlock.getChannelPointer(1)[i] < -distortionLimit) filterBlock.getChannelPointer(1)[i] = -distortionLimit;
				}
			}
		}

		float outputGain = paramGain * 0.01f;
		if (!isUI) {
			m_fGain_smoothed.setValue(m_Set->getParameterValueWithMatrixModulation(&paramGain, paramIDGain, matrixInputState), m_bSmoothersTakeNextValue);
			outputGain = static_cast<float> (m_fGain_smoothed.getNextValue() * 0.01f);
			m_fGain_smoothed.skip(numSamples - 1);
		}

		int i = 0;
		int samplesToProcess = numSamples;
		while (samplesToProcess >= C_FILTER_BLOCKSIZE) {
			matrixInputState->currentFrame = startSample + i;
			float lFilterEnvMod = paramFilterEnvMod;
			if (isUI) {
				fVCFEnvelopeMod = 1.0f;
			}
			else {
				if (paramFilterEnv == static_cast<int>(MSEGENV::MSEG1)) {
					fVCFEnvelopeMod = routingBuffers.MSEGBuffer[0][matrixInputState->voiceNo]->getSample(0, matrixInputState->currentFrame);
				}
				else if (paramFilterEnv == static_cast<int>(MSEGENV::MSEG2)) {
					fVCFEnvelopeMod = routingBuffers.MSEGBuffer[1][matrixInputState->voiceNo]->getSample(0, matrixInputState->currentFrame);
				}
				else if (paramFilterEnv == static_cast<int>(MSEGENV::MSEG3)) {
					fVCFEnvelopeMod = routingBuffers.MSEGBuffer[2][matrixInputState->voiceNo]->getSample(0, matrixInputState->currentFrame);
				}
				else if (paramFilterEnv == static_cast<int>(MSEGENV::MSEG4)) {
					fVCFEnvelopeMod = routingBuffers.MSEGBuffer[3][matrixInputState->voiceNo]->getSample(0, matrixInputState->currentFrame);
				}
				else if (paramFilterEnv == static_cast<int>(MSEGENV::MSEG5)) {
					fVCFEnvelopeMod = routingBuffers.MSEGBuffer[4][matrixInputState->voiceNo]->getSample(0, matrixInputState->currentFrame);
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

			double resonance = 0.0;
			double resonance_target = 0.0;
			double cutoff = 0.0;
			double cutoff_target = 0.0;
			if (isUI) {
				cutoff = float(paramCutoff * fVCFEnvelopeMod) + 1.f;
				resonance = paramResonance;
			}
			else {
				cutoff_target = float(m_Set->getParameterValueWithMatrixModulation(&paramCutoff, paramIDCutoff, matrixInputState) * fVCFEnvelopeMod) + 1.f;
				resonance_target = m_Set->getParameterValueWithMatrixModulation(&paramResonance, paramIDResonance, matrixInputState);
				m_fCutoff_smoothed.setValue(cutoff_target, m_bSmoothersTakeNextValue);
				cutoff = m_fCutoff_smoothed.getNextValue();
				m_fCutoff_smoothed.skip(C_FILTER_BLOCKSIZE - 1);
				m_fResonance_smoothed.setValue(resonance_target, m_bSmoothersTakeNextValue);
				resonance = m_fResonance_smoothed.getNextValue();
				m_fResonance_smoothed.skip(C_FILTER_BLOCKSIZE - 1);
			}
			//prevent low booms
			//cutoff = cutoff + 50.f;

			if (m_useOversampling)
				cutoff /= C_FILTER_OVERSAMPLINGFACTOR;

			m_ladderFilter1.setCutoffFrequencyHz(cutoff); //is smoothed
			if (mbFilter2) m_ladderFilter2.setCutoffFrequencyHz(cutoff); //is smoothed	
			if (mbFilter2) {
				m_ladderFilter1.setResonance(0.1f); //is smoothed
				m_ladderFilter2.setResonance(resonance / 100.f); //is smoothed
			}
			else
				m_ladderFilter1.setResonance(resonance / 100.f); //is smoothed

			
			if (!m_useOversampling) { 
				dsp::ProcessContextReplacing<float>::AudioBlockType _block = filterBlock.getSubBlock(i, C_FILTER_BLOCKSIZE);
				dsp::ProcessContextReplacing<float> _context = dsp::ProcessContextReplacing<float>(_block);

				m_ladderFilter1.process(_context);
				if (mbFilter2)
				m_ladderFilter2.process(_context);
			}			
			else {
				//upsample
				dsp::AudioBlock<float> inBlock = filterBlock.getSubBlock(i, C_FILTER_BLOCKSIZE);
				dsp::AudioBlock<float> inBlockUp = dsp::AudioBlock<float>(*inBufferUp);
				for (int j = 0; j < C_FILTER_BLOCKSIZE * C_FILTER_OVERSAMPLINGFACTOR; j++) {
					if (j % C_FILTER_OVERSAMPLINGFACTOR == 0) {
						inBlockUp.setSample(0, j, inBlock.getSample(0, j / C_FILTER_OVERSAMPLINGFACTOR));
						inBlockUp.setSample(1, j, inBlock.getSample(1, j / C_FILTER_OVERSAMPLINGFACTOR));
					}
					else {
						inBlockUp.setSample(0, j, 0.f); //zero pad				
						inBlockUp.setSample(1, j, 0.f); //zero pad				
					}
				}
				//process
				m_ladderFilter1.process(dsp::ProcessContextReplacing<float>(inBlockUp));
				if (mbFilter2)
					m_ladderFilter2.process(dsp::ProcessContextReplacing<float>(inBlockUp));
				//downsample
				for (int j = 0; j < C_FILTER_BLOCKSIZE * C_FILTER_OVERSAMPLINGFACTOR; j++) {
					if (j % C_FILTER_OVERSAMPLINGFACTOR == 0) {
						inBlock.setSample(0, j / C_FILTER_OVERSAMPLINGFACTOR, inBlockUp.getSample(0, j));
						inBlock.setSample(1, j / C_FILTER_OVERSAMPLINGFACTOR, inBlockUp.getSample(1, j));
					}
				}
			}

			samplesToProcess -= C_FILTER_BLOCKSIZE;
			i += C_FILTER_BLOCKSIZE;

			m_bSmoothersTakeNextValue = false;
		}
		if (samplesToProcess > 0) { //do rest
			if (!isUI) {
				m_fResonance_smoothed.skip(samplesToProcess);
				m_fCutoff_smoothed.skip(samplesToProcess);
			}


			if (!m_useOversampling) { //without oversampling	
				dsp::ProcessContextReplacing<float>::AudioBlockType _block = filterBlock.getSubBlock(i, samplesToProcess);
				dsp::ProcessContextReplacing<float> _context = dsp::ProcessContextReplacing<float>(_block);

				m_ladderFilter1.process(dsp::ProcessContextReplacing<float>(_context));
				if (mbFilter2)
					m_ladderFilter2.process(dsp::ProcessContextReplacing<float>(_context));
			}
			else {
				//upsample
				dsp::AudioBlock<float> inBlock = filterBlock.getSubBlock(i, samplesToProcess);
				dsp::AudioBlock<float> inBlockUp = dsp::AudioBlock<float>(*inBufferUp);
				for (int j = 0; j < samplesToProcess * C_FILTER_OVERSAMPLINGFACTOR; j++) {
					if (j % C_FILTER_OVERSAMPLINGFACTOR == 0) {
						inBlockUp.setSample(0, j, inBlock.getSample(0, j / C_FILTER_OVERSAMPLINGFACTOR));
						inBlockUp.setSample(1, j, inBlock.getSample(1, j / C_FILTER_OVERSAMPLINGFACTOR));
					}
					else {
						inBlockUp.setSample(0, j, 0.f); //zero pad				
						inBlockUp.setSample(1, j, 0.f); //zero pad				
					}
				}
				//process
				dsp::ProcessContextReplacing<float>::AudioBlockType _block = inBlockUp.getSubBlock(0, samplesToProcess * C_FILTER_OVERSAMPLINGFACTOR);
				dsp::ProcessContextReplacing<float> _context = dsp::ProcessContextReplacing<float>(_block);
				m_ladderFilter1.process(dsp::ProcessContextReplacing<float>(_context));
				if (mbFilter2)
					m_ladderFilter2.process(dsp::ProcessContextReplacing<float>(_context));
				//downsample
				for (int j = 0; j < samplesToProcess * C_FILTER_OVERSAMPLINGFACTOR; j++) {
					if (j % C_FILTER_OVERSAMPLINGFACTOR == 0) {
						inBlock.setSample(0, j / C_FILTER_OVERSAMPLINGFACTOR, inBlockUp.getSample(0, j));
						inBlock.setSample(1, j / C_FILTER_OVERSAMPLINGFACTOR, inBlockUp.getSample(1, j));
					}
				}
			}
		}

		//if (gainMult > 1.f) gainMult = 1.f + (gainMult - 1.f) * 0.2f;
		//if (gainMult != 0.f) filterBlock.multiply(1.0f / gainMult); //redo the original gain when using drive
		filterBlock.multiplyBy(m_attenuateFactor * outputGain); //oversampling factor
		
		break;
	}

	//===============================================================================================================================
	case(FILTERTYPE::SVFLPF12):  //SVF LPF 12
	case(FILTERTYPE::SVFHPF12):  //SVF HPF 12
	case(FILTERTYPE::SVFBPF12):  //SVF BPF 12
	case(FILTERTYPE::SVFLPF24):  //SVF LPF 24
	case(FILTERTYPE::SVFHPF24):  //SVF HPF 24
	case(FILTERTYPE::SVFBPF24):  //SVF BPF 24
	case(FILTERTYPE::SVFLPF36):  //SVF LPF 36
	case(FILTERTYPE::SVFHPF36):  //SVF HPF 36
	case(FILTERTYPE::SVFBPF36):  //SVF BPF 36
	{
		if (bTypeChanged) {
			if (!isUI) {
				m_bqEndLowShelfL1.setCoefficients(coeff.makeHighPass(m_Set->m_nSampleRate, 30.f, sqrt2over2));
				m_bqEndLowShelfR1.setCoefficients(coeff.makeHighPass(m_Set->m_nSampleRate, 30.f, sqrt2over2));
				m_bqEndLowShelfL2.setCoefficients(coeff.makeHighPass(m_Set->m_nSampleRate, 30.f, sqrt2over2));
				m_bqEndLowShelfR2.setCoefficients(coeff.makeHighPass(m_Set->m_nSampleRate, 30.f, sqrt2over2));
			}

			switch (paramType) {
			case(FILTERTYPE::SVFLPF12): { //SVF LPF 12
				auto type = static_cast<dsp::StateVariableFilter::Parameters<float>::Type> (dsp::StateVariableFilter::Parameters<float>::Type::lowPass);
				m_dspStateVariableFilterL1.parameters->type = type;
				m_dspStateVariableFilterR1.parameters->type = type;
				mbFilter2 = false;
				mbFilter3 = false;
				m_attenuateFactor = C_FILTER_OVERSAMPLINGFACTOR * 1.3f;
				m_useOversampling = true;
				break;
			}
			case(FILTERTYPE::SVFHPF12): { //SVF HPF 12
				auto type = static_cast<dsp::StateVariableFilter::Parameters<float>::Type> (dsp::StateVariableFilter::Parameters<float>::Type::highPass);
				m_dspStateVariableFilterL1.parameters->type = type;
				m_dspStateVariableFilterR1.parameters->type = type;
				mbFilter2 = false;
				mbFilter3 = false;
				m_attenuateFactor = 1.f;
				m_useOversampling = false;
				break;
			}
			case(FILTERTYPE::SVFBPF12): { //SVF BPF 12
				auto type = static_cast<dsp::StateVariableFilter::Parameters<float>::Type> (dsp::StateVariableFilter::Parameters<float>::Type::bandPass);
				m_dspStateVariableFilterL1.parameters->type = type;
				m_dspStateVariableFilterR1.parameters->type = type;
				mbFilter2 = false;
				mbFilter3 = false;
				m_attenuateFactor = 1.45f;
				m_useOversampling = false;
				break;
			}
			case(FILTERTYPE::SVFLPF24): { //SVF LPF 24
				auto type = static_cast<dsp::StateVariableFilter::Parameters<float>::Type> (dsp::StateVariableFilter::Parameters<float>::Type::lowPass);
				m_dspStateVariableFilterL1.parameters->type = type;
				m_dspStateVariableFilterR1.parameters->type = type;
				m_dspStateVariableFilterL2.parameters->type = type;
				m_dspStateVariableFilterR2.parameters->type = type;
				mbFilter2 = true;
				mbFilter3 = false;
				m_attenuateFactor = C_FILTER_OVERSAMPLINGFACTOR * 6.4f;
				m_useOversampling = true;
				break;
			}
			case(FILTERTYPE::SVFHPF24): { //SVF HPF 24
				auto type = static_cast<dsp::StateVariableFilter::Parameters<float>::Type> (dsp::StateVariableFilter::Parameters<float>::Type::highPass);
				m_dspStateVariableFilterL1.parameters->type = type;
				m_dspStateVariableFilterR1.parameters->type = type;
				m_dspStateVariableFilterL2.parameters->type = type;
				m_dspStateVariableFilterR2.parameters->type = type;
				mbFilter2 = true;
				mbFilter3 = false;
				m_attenuateFactor = 1.f;
				m_useOversampling = false;
				break;
			}
			case(FILTERTYPE::SVFBPF24): { //SVF BPF 24
				auto type = static_cast<dsp::StateVariableFilter::Parameters<float>::Type> (dsp::StateVariableFilter::Parameters<float>::Type::bandPass);
				m_dspStateVariableFilterL1.parameters->type = type;
				m_dspStateVariableFilterR1.parameters->type = type;
				m_dspStateVariableFilterL2.parameters->type = type;
				m_dspStateVariableFilterR2.parameters->type = type;
				mbFilter2 = true;
				mbFilter3 = false;
				m_attenuateFactor = 4.8f;
				m_useOversampling = false;
				break;
			}
			case(FILTERTYPE::SVFLPF36): { //SVF LPF 36
				auto type = static_cast<dsp::StateVariableFilter::Parameters<float>::Type> (dsp::StateVariableFilter::Parameters<float>::Type::lowPass);
				m_dspStateVariableFilterL1.parameters->type = type;
				m_dspStateVariableFilterR1.parameters->type = type;
				m_dspStateVariableFilterL2.parameters->type = type;
				m_dspStateVariableFilterR2.parameters->type = type;
				m_dspStateVariableFilterL3.parameters->type = type;
				m_dspStateVariableFilterR3.parameters->type = type;
				mbFilter2 = true;
				mbFilter3 = true;
				m_attenuateFactor = C_FILTER_OVERSAMPLINGFACTOR * 32.f;
				m_useOversampling = true;
				break;
			}
			case(FILTERTYPE::SVFHPF36): { //SVF HPF 36
				auto type = static_cast<dsp::StateVariableFilter::Parameters<float>::Type> (dsp::StateVariableFilter::Parameters<float>::Type::highPass);
				m_dspStateVariableFilterL1.parameters->type = type;
				m_dspStateVariableFilterR1.parameters->type = type;
				m_dspStateVariableFilterL2.parameters->type = type;
				m_dspStateVariableFilterR2.parameters->type = type;
				m_dspStateVariableFilterL3.parameters->type = type;
				m_dspStateVariableFilterR3.parameters->type = type;
				mbFilter2 = true;
				mbFilter3 = true;
				m_attenuateFactor = 1.f;
				m_useOversampling = false;
				break;
			}
			case(FILTERTYPE::SVFBPF36): { //SVF BPF 36
				auto type = static_cast<dsp::StateVariableFilter::Parameters<float>::Type> (dsp::StateVariableFilter::Parameters<float>::Type::bandPass);
				m_dspStateVariableFilterL1.parameters->type = type;
				m_dspStateVariableFilterR1.parameters->type = type;
				m_dspStateVariableFilterL2.parameters->type = type;
				m_dspStateVariableFilterR2.parameters->type = type;
				m_dspStateVariableFilterL3.parameters->type = type;
				m_dspStateVariableFilterR3.parameters->type = type;
				mbFilter2 = true;
				mbFilter3 = true;
				m_attenuateFactor = 16.f;
				m_useOversampling = false;
				break;
			}
			}
		}

		float lDrive = paramDrive;
		float gainMult = 1.f;
		if (isUI)
			gainMult = fastPrecisePow(10.0, lDrive / 20.0);
		else {
			m_fDrive_smoothed.setValue(paramDrive, m_bSmoothersTakeNextValue);
			lDrive = m_fDrive_smoothed.getNextValue();
			m_fDrive_smoothed.skip(numSamples - 1);
			gainMult = fastPrecisePow(10.0, m_Set->getParameterValueWithMatrixModulation(&lDrive, paramIDDrive, matrixInputState) / 20.0);
		}
		if (gainMult != 1.f) {
			filterBlock.multiplyBy(gainMult);
			if (lDrive > 0.f) {
				float distortionLimit = 1.f;
				for (int i = startSample; i < numSamples; i++) {
					if (filterBlock.getChannelPointer(0)[i] > distortionLimit) filterBlock.getChannelPointer(0)[i] = distortionLimit;
					if (filterBlock.getChannelPointer(0)[i] < -distortionLimit) filterBlock.getChannelPointer(0)[i] = -distortionLimit;
					if (filterBlock.getChannelPointer(1)[i] > distortionLimit) filterBlock.getChannelPointer(1)[i] = distortionLimit;
					if (filterBlock.getChannelPointer(1)[i] < -distortionLimit) filterBlock.getChannelPointer(1)[i] = -distortionLimit;
				}
			}
		}

		float outputGain = paramGain * 0.01f;
		if (!isUI) {
			m_fGain_smoothed.setValue(m_Set->getParameterValueWithMatrixModulation(&paramGain, paramIDGain, matrixInputState), m_bSmoothersTakeNextValue);
			outputGain = static_cast<float> (m_fGain_smoothed.getNextValue() * 0.01f);
			m_fGain_smoothed.skip(numSamples - 1);
		}

		double cutoff = 0.0;
		double resval1 = 0.0;
		double resval2 = 0.0;
		double resval3 = 0.0;
		float* lIn = filterBlock.getChannelPointer(0);
		float* rIn = filterBlock.getChannelPointer(1);
		int i = 0;
		int samplesToProcess = numSamples;
		while (samplesToProcess >= C_FILTER_BLOCKSIZE) {
			matrixInputState->currentFrame = startSample + i;
			float lFilterEnvMod = paramFilterEnvMod;
			if (isUI) {
				fVCFEnvelopeMod = 1.0f;
			}
			else {
				if (paramFilterEnv == static_cast<int>(MSEGENV::MSEG1)) {
					fVCFEnvelopeMod = routingBuffers.MSEGBuffer[0][matrixInputState->voiceNo]->getSample(0, matrixInputState->currentFrame);
				}
				else if (paramFilterEnv == static_cast<int>(MSEGENV::MSEG2)) {
					fVCFEnvelopeMod = routingBuffers.MSEGBuffer[1][matrixInputState->voiceNo]->getSample(0, matrixInputState->currentFrame);
				}
				else if (paramFilterEnv == static_cast<int>(MSEGENV::MSEG3)) {
					fVCFEnvelopeMod = routingBuffers.MSEGBuffer[2][matrixInputState->voiceNo]->getSample(0, matrixInputState->currentFrame);
				}
				else if (paramFilterEnv == static_cast<int>(MSEGENV::MSEG4)) {
					fVCFEnvelopeMod = routingBuffers.MSEGBuffer[3][matrixInputState->voiceNo]->getSample(0, matrixInputState->currentFrame);
				}
				else if (paramFilterEnv == static_cast<int>(MSEGENV::MSEG5)) {
					fVCFEnvelopeMod = routingBuffers.MSEGBuffer[4][matrixInputState->voiceNo]->getSample(0, matrixInputState->currentFrame);
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

			double resonance = 0.0;
			double resonance_target = 0.0;
			double cutoff_target = 0.0;
			if (isUI) {
				cutoff = float(paramCutoff * fVCFEnvelopeMod) + 1.f;
				resonance_target = paramResonance;
				//resonance = static_cast<float> (resonance_target + 0.3f);
				//resonance = static_cast<float> (resonance_target / 11.f + 0.3f);
				resonance = static_cast<float> (resonance_target / 11.f + sqrt2over2);
			}
			else {
				cutoff_target = float(m_Set->getParameterValueWithMatrixModulation(&paramCutoff, paramIDCutoff, matrixInputState) * fVCFEnvelopeMod) + 1.f;
				m_fCutoff_smoothed.setValue(cutoff_target, m_bSmoothersTakeNextValue);
				cutoff = m_fCutoff_smoothed.getNextValue();
				m_fCutoff_smoothed.skip(C_FILTER_BLOCKSIZE - 1);
		
				resonance_target = m_Set->getParameterValueWithMatrixModulation(&paramResonance, paramIDResonance, matrixInputState);
				m_fResonance_smoothed.setValue(resonance_target, m_bSmoothersTakeNextValue);
				//resonance = static_cast<float> (m_fResonance_smoothed.getNextValue() / 11.f + 0.3f);				
				resonance = static_cast<float> (m_fResonance_smoothed.getNextValue() / 11.f + sqrt2over2);
				m_fResonance_smoothed.skip(C_FILTER_BLOCKSIZE - 1);

				if ((paramType == FILTERTYPE::SVFBPF12) || (paramType == FILTERTYPE::SVFBPF24) || (paramType == FILTERTYPE::SVFBPF36)) {
					cutoff = (cutoff > 20000.0) ? 20000.0 : cutoff;
				}
			}
			
			if (m_useOversampling)
				cutoff /= C_FILTER_OVERSAMPLINGFACTOR * 1.45f; //from 28.000 to 18.000
			else
				if (cutoff > m_Set->m_nSampleRate * 0.5f) cutoff = m_Set->m_nSampleRate * 0.5f;
			
			resval1 = (mbFilter2 || mbFilter3) ? 0.3 : resonance;
			//m_dspStateVariableFilterL1.parameters->setCutOffFrequency(m_Set->m_nSampleRate * C_FILTER_OVERSAMPLINGFACTOR, cutoff, resval1);
			//m_dspStateVariableFilterR1.parameters->setCutOffFrequency(m_Set->m_nSampleRate * C_FILTER_OVERSAMPLINGFACTOR, cutoff, resval1);
			m_dspStateVariableFilterL1.parameters->setCutOffFrequency(m_Set->m_nSampleRate, cutoff, resval1);
			m_dspStateVariableFilterR1.parameters->setCutOffFrequency(m_Set->m_nSampleRate, cutoff, resval1);

			if (mbFilter2) {
				resval2 = (mbFilter3) ? 0.3 : resonance;
				m_dspStateVariableFilterL2.parameters->setCutOffFrequency(m_Set->m_nSampleRate, cutoff, resval2);
				m_dspStateVariableFilterR2.parameters->setCutOffFrequency(m_Set->m_nSampleRate, cutoff, resval2);
				//m_dspStateVariableFilterL2.parameters->setCutOffFrequency(m_Set->m_nSampleRate * C_FILTER_OVERSAMPLINGFACTOR, cutoff, resval2);
				//m_dspStateVariableFilterR2.parameters->setCutOffFrequency(m_Set->m_nSampleRate * C_FILTER_OVERSAMPLINGFACTOR, cutoff, resval2);
			}
			if (mbFilter3) {
				resval3 = resonance;
				m_dspStateVariableFilterL3.parameters->setCutOffFrequency(m_Set->m_nSampleRate, cutoff, resval3);
				m_dspStateVariableFilterR3.parameters->setCutOffFrequency(m_Set->m_nSampleRate, cutoff, resval3);
				//m_dspStateVariableFilterL3.parameters->setCutOffFrequency(m_Set->m_nSampleRate * C_FILTER_OVERSAMPLINGFACTOR, cutoff, resonance);
				//m_dspStateVariableFilterR3.parameters->setCutOffFrequency(m_Set->m_nSampleRate * C_FILTER_OVERSAMPLINGFACTOR, cutoff, resonance);
			}

			int loop = 0;
			if (!m_useOversampling) {
				while (loop < C_FILTER_BLOCKSIZE) {
					lIn[i] = m_dspStateVariableFilterL1.processSample(lIn[i]);
					rIn[i] = m_dspStateVariableFilterR1.processSample(rIn[i]);
					if (mbFilter2) {
						lIn[i] = m_dspStateVariableFilterL2.processSample(lIn[i]);
						rIn[i] = m_dspStateVariableFilterR2.processSample(rIn[i]);
					}
					if (mbFilter3) {
						lIn[i] = m_dspStateVariableFilterL3.processSample(lIn[i]);
						rIn[i] = m_dspStateVariableFilterR3.processSample(rIn[i]);
					}

					//NaN Check
					if (isnan(lIn[i])) {
						lIn[i] = 0.f;
						m_dspStateVariableFilterL1.reset(); m_dspStateVariableFilterL2.reset(); m_dspStateVariableFilterL3.reset();
					} //reset filter when it happens
					if (isnan(rIn[i])) {
						rIn[i] = 0.f;
						m_dspStateVariableFilterR1.reset(); m_dspStateVariableFilterR2.reset(); m_dspStateVariableFilterR3.reset();
					} //reset filter when it happens

					loop++;
					i++;
				}
			}
			else {
				while (loop < C_FILTER_BLOCKSIZE * C_FILTER_OVERSAMPLINGFACTOR) {
					if (loop % C_FILTER_OVERSAMPLINGFACTOR == 0) {
						lIn[i] = m_dspStateVariableFilterL1.processSample(lIn[i]);
						rIn[i] = m_dspStateVariableFilterR1.processSample(rIn[i]);

						if (mbFilter2) {
							lIn[i] = m_dspStateVariableFilterL2.processSample(lIn[i]);
							rIn[i] = m_dspStateVariableFilterR2.processSample(rIn[i]);
						}
						if (mbFilter3) {
							lIn[i] = m_dspStateVariableFilterL3.processSample(lIn[i]);
							rIn[i] = m_dspStateVariableFilterR3.processSample(rIn[i]);
						}

						//NaN Check
						if (isnan(lIn[i])) {
							lIn[i] = 0.f;
							m_dspStateVariableFilterL1.reset(); m_dspStateVariableFilterL2.reset(); m_dspStateVariableFilterL3.reset();
						} //reset filter when it happens
						if (isnan(rIn[i])) {
							rIn[i] = 0.f;
							m_dspStateVariableFilterR1.reset(); m_dspStateVariableFilterR2.reset(); m_dspStateVariableFilterR3.reset();
						} //reset filter when it happens
						i++;
					}
					else { //zero padding and skip
						m_dspStateVariableFilterL1.processSample(0.f);
						m_dspStateVariableFilterR1.processSample(0.f);
						if (mbFilter2) {
							m_dspStateVariableFilterL2.processSample(0.f);
							m_dspStateVariableFilterR2.processSample(0.f);
						}
						if (mbFilter3) {
							m_dspStateVariableFilterL3.processSample(0.f);
							m_dspStateVariableFilterR3.processSample(0.f);
						}
					}

					loop++;
				}
			}

			samplesToProcess -= C_FILTER_BLOCKSIZE;
			m_dspStateVariableFilterL1.snapToZero();
			m_dspStateVariableFilterR1.snapToZero();
			if (mbFilter2) {
				m_dspStateVariableFilterL2.snapToZero();
				m_dspStateVariableFilterR2.snapToZero();
			}
			if (mbFilter3) {
				m_dspStateVariableFilterL3.snapToZero();
				m_dspStateVariableFilterR3.snapToZero();
			}
			m_bSmoothersTakeNextValue = false;
		}

		if (!m_useOversampling) {
			while (samplesToProcess > 0) { //do rest
				if (!isUI) {
					m_fResonance_smoothed.skip(1);
					m_fCutoff_smoothed.skip(1);
				}
				lIn[i] = m_dspStateVariableFilterL1.processSample(lIn[i]);
				rIn[i] = m_dspStateVariableFilterR1.processSample(rIn[i]);
				if (mbFilter2) {
					lIn[i] = m_dspStateVariableFilterL2.processSample(lIn[i]);
					rIn[i] = m_dspStateVariableFilterR2.processSample(rIn[i]);
				}
				if (mbFilter3) {
					lIn[i] = m_dspStateVariableFilterL3.processSample(lIn[i]);
					rIn[i] = m_dspStateVariableFilterR3.processSample(rIn[i]);
				}

				//NaN Check
				if (isnan(lIn[i])) {
					lIn[i] = 0.f;
					m_dspStateVariableFilterL1.reset(); m_dspStateVariableFilterL2.reset(); m_dspStateVariableFilterL3.reset();
				} //reset filter when it happens
				if (isnan(rIn[i])) {
					rIn[i] = 0.f;
					m_dspStateVariableFilterR1.reset(); m_dspStateVariableFilterR2.reset(); m_dspStateVariableFilterR3.reset();
				} //reset filter when it happens

				i++;
				samplesToProcess--;
			}
		}
		else { //oversample
			int loop = samplesToProcess * C_FILTER_OVERSAMPLINGFACTOR;
			while (loop > 0) { //do rest
				if (loop % C_FILTER_OVERSAMPLINGFACTOR == 0) {
					if (!isUI) {
						m_fResonance_smoothed.skip(1);
						m_fCutoff_smoothed.skip(1);
						//CHECK set values here??
					}
					lIn[i] = m_dspStateVariableFilterL1.processSample(lIn[i]);
					rIn[i] = m_dspStateVariableFilterR1.processSample(rIn[i]);

					if (mbFilter2) {
						lIn[i] = m_dspStateVariableFilterL2.processSample(lIn[i]);
						rIn[i] = m_dspStateVariableFilterR2.processSample(rIn[i]);
					}
					if (mbFilter3) {
						lIn[i] = m_dspStateVariableFilterL3.processSample(lIn[i]);
						rIn[i] = m_dspStateVariableFilterR3.processSample(rIn[i]);
					}

					//NaN Check
					if (isnan(lIn[i])) {
						lIn[i] = 0.f;
						m_dspStateVariableFilterL1.reset(); m_dspStateVariableFilterL2.reset(); m_dspStateVariableFilterL3.reset();
					} //reset filter when it happens
					if (isnan(rIn[i])) {
						rIn[i] = 0.f;
						m_dspStateVariableFilterR1.reset(); m_dspStateVariableFilterR2.reset(); m_dspStateVariableFilterR3.reset();
					} //reset filter when it happens
					i++;
				}
				else { //zero padding and skip
					m_dspStateVariableFilterL1.processSample(0.f);
					m_dspStateVariableFilterR1.processSample(0.f);
					if (mbFilter2) {
						m_dspStateVariableFilterL2.processSample(0.f);
						m_dspStateVariableFilterR2.processSample(0.f);
					}
					if (mbFilter3) {
						m_dspStateVariableFilterL3.processSample(0.f);
						m_dspStateVariableFilterR3.processSample(0.f);
					}
				}

				loop--;
			}
		}

		m_dspStateVariableFilterL1.snapToZero();
		m_dspStateVariableFilterR1.snapToZero();
		if (mbFilter2) {
			m_dspStateVariableFilterL2.snapToZero();
			m_dspStateVariableFilterR2.snapToZero();
		}
		if (mbFilter3) {
			m_dspStateVariableFilterL3.snapToZero();
			m_dspStateVariableFilterR3.snapToZero();
		}

		//if (gainMult > 1.f) gainMult = 1.f + (gainMult - 1.f) * 0.45f;
		//if (gainMult != 0.f) filterBlock.multiply(1.0f / gainMult); //redo the original gain when using drive
		filterBlock.multiplyBy(m_attenuateFactor * outputGain);

		break;
	}

//===============================================================================================================================
	case(FILTERTYPE::KRGHPF12):  //KRG HPF 12
	case(FILTERTYPE::KRGHPFSAT12): //KRG HPFSAT 12
	{
			if (bTypeChanged) {
				if (!isUI) {
					m_bqEndLowShelfL1.setCoefficients(coeff.makeHighPass(m_Set->m_nSampleRate, 30.f, sqrt2over2));
					m_bqEndLowShelfR1.setCoefficients(coeff.makeHighPass(m_Set->m_nSampleRate, 30.f, sqrt2over2));
					m_bqEndLowShelfL2.setCoefficients(coeff.makeHighPass(m_Set->m_nSampleRate, 30.f, sqrt2over2));
					m_bqEndLowShelfR2.setCoefficients(coeff.makeHighPass(m_Set->m_nSampleRate, 30.f, sqrt2over2));
				}
			}

			double fOut[2];
			fOut[0] = 0;
			fOut[1] = 0;

			float lDrive = paramDrive;
			float gainMult = 1.f;
			if (isUI)
				gainMult = fastPrecisePow(10.0, lDrive / 20.0);
			else {
				m_fDrive_smoothed.setValue(paramDrive, m_bSmoothersTakeNextValue);
				lDrive = m_fDrive_smoothed.getNextValue();
				m_fDrive_smoothed.skip(numSamples - 1);
				gainMult = fastPrecisePow(10.0, m_Set->getParameterValueWithMatrixModulation(&lDrive, paramIDDrive, matrixInputState) / 20.0);
			}
			if (gainMult != 1.f) {
				filterBlock.multiplyBy(gainMult);
				if (lDrive > 0.f) {
					float distortionLimit = 1.f;
					for (int i = startSample; i < numSamples; i++) {
						if (filterBlock.getChannelPointer(0)[i] > distortionLimit) filterBlock.getChannelPointer(0)[i] = distortionLimit;
						if (filterBlock.getChannelPointer(0)[i] < -distortionLimit) filterBlock.getChannelPointer(0)[i] = -distortionLimit;
						if (filterBlock.getChannelPointer(1)[i] > distortionLimit) filterBlock.getChannelPointer(1)[i] = distortionLimit;
						if (filterBlock.getChannelPointer(1)[i] < -distortionLimit) filterBlock.getChannelPointer(1)[i] = -distortionLimit;
					}
				}
			}

			float outputGain = paramGain * 0.01f;
			if (!isUI) {
				m_fGain_smoothed.setValue(m_Set->getParameterValueWithMatrixModulation(&paramGain, paramIDGain, matrixInputState), m_bSmoothersTakeNextValue);
				outputGain = static_cast<float> (m_fGain_smoothed.getNextValue() * 0.01f);
				m_fGain_smoothed.skip(numSamples - 1);
			}

			float* lIn = filterBlock.getChannelPointer(0);
			float* rIn = filterBlock.getChannelPointer(1);
			int i = 0;
			int samplesToProcess = numSamples;
			while (samplesToProcess >= C_FILTER_BLOCKSIZE) {
				matrixInputState->currentFrame = startSample + i;
				float lFilterEnvMod = paramFilterEnvMod;
				if (isUI) {
					fVCFEnvelopeMod = 1.0f;
				}
				else {
					if (paramFilterEnv == static_cast<int>(MSEGENV::MSEG1)) {
						fVCFEnvelopeMod = routingBuffers.MSEGBuffer[0][matrixInputState->voiceNo]->getSample(0, matrixInputState->currentFrame);
					}
					else if (paramFilterEnv == static_cast<int>(MSEGENV::MSEG2)) {
						fVCFEnvelopeMod = routingBuffers.MSEGBuffer[1][matrixInputState->voiceNo]->getSample(0, matrixInputState->currentFrame);
					}
					else if (paramFilterEnv == static_cast<int>(MSEGENV::MSEG3)) {
						fVCFEnvelopeMod = routingBuffers.MSEGBuffer[2][matrixInputState->voiceNo]->getSample(0, matrixInputState->currentFrame);
					}
					else if (paramFilterEnv == static_cast<int>(MSEGENV::MSEG4)) {
						fVCFEnvelopeMod = routingBuffers.MSEGBuffer[3][matrixInputState->voiceNo]->getSample(0, matrixInputState->currentFrame);
					}
					else if (paramFilterEnv == static_cast<int>(MSEGENV::MSEG5)) {
						fVCFEnvelopeMod = routingBuffers.MSEGBuffer[4][matrixInputState->voiceNo]->getSample(0, matrixInputState->currentFrame);
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

				double resonance = 0.0;
				double resonance_target = 0.0;
				double cutoff = 0.0;
				double cutoff_target = 0.0;
				double scale = 0.0;
				double scale_target = 0.0;
				if (isUI) {
					cutoff = (float(paramCutoff * fVCFEnvelopeMod) < 50.0f) ? 50.0f : paramCutoff * fVCFEnvelopeMod;
					resonance = paramResonance;
					scale = static_cast<float> (1.f + 0.4f * (paramScale - 100.f) / 100.f); //1.f - 1.5f
				}
				else {
					//cutoff_target = (float(paramCutoff * m_Set->get0To1ModMatrixMultiplier(paramIDCutoff, matrixInputState) * fVCFEnvelopeMod) < 50.0f) ? 50.0f : float(paramCutoff* m_Set->get0To1ModMatrixMultiplier(paramIDCutoff, matrixInputState) * fVCFEnvelopeMod);
					cutoff_target = (float(m_Set->getParameterValueWithMatrixModulation(&paramCutoff, paramIDCutoff, matrixInputState) * fVCFEnvelopeMod) < 50.f) ? 50.0f : float(m_Set->getParameterValueWithMatrixModulation(&paramCutoff, paramIDCutoff, matrixInputState) * fVCFEnvelopeMod);
					m_fCutoff_smoothed.setValue(cutoff_target, m_bSmoothersTakeNextValue);
					cutoff = m_fCutoff_smoothed.getNextValue();
					resonance_target = m_Set->getParameterValueWithMatrixModulation(&paramResonance, paramIDResonance, matrixInputState);
					m_fResonance_smoothed.setValue(resonance_target, m_bSmoothersTakeNextValue);
					resonance = m_fResonance_smoothed.getNextValue();
					scale_target = m_Set->getParameterValueWithMatrixModulation(&paramScale, paramIDScale, matrixInputState);
					m_fScale_smoothed.setValue(scale_target, m_bSmoothersTakeNextValue);
					scale = static_cast<float> (1.f + 0.4f * (m_fScale_smoothed.getNextValue() - 100.f) / 100.f); //1.f - 1.5f
					m_fScale_smoothed.skip(C_FILTER_BLOCKSIZE - 1);
				}

				cutoff = (cutoff < 22000.f) ? cutoff : 22000.f;

				m_TFHPF_dFc = cutoff;
				m_TFHPF_dK = resonance / 51.0f + 0.02f; //should be (0..2.f)
				m_TFHPF_dSaturation = scale; //should be 1.0f..1.5f  - only used for for NLP
				if (paramType == FILTERTYPE::KRGHPF12)
					m_TFHPF_uNonLinearProcessing = OFF;
				else
					m_TFHPF_uNonLinearProcessing = ON; //causes aliasing		
				KorgThreeFiveHPFmkIIupdateFilters();

				int loop = 0;
				while (loop < C_FILTER_BLOCKSIZE) {
					double lFilterIn[2];
					lFilterIn[0] = lIn[i];
					lFilterIn[1] = rIn[i];
					double fOut[2];
					fOut[0] = 0;
					fOut[1] = 0;
					//do filter
					KorgThreeFiveHPFmkIIdoFilter(lFilterIn, fOut);
					lIn[i] = fOut[0];
					rIn[i] = fOut[1];

					loop++;
					i++;
				}

				samplesToProcess -= C_FILTER_BLOCKSIZE;
				m_bSmoothersTakeNextValue = false;
			}
			while (samplesToProcess > 0) { //do rest
				if (!isUI) {
					m_fResonance_smoothed.skip(1);
					m_fCutoff_smoothed.skip(1);
				}
				double lFilterIn[2];
				lFilterIn[0] = lIn[i];
				lFilterIn[1] = rIn[i];
				double fOut[2];
				fOut[0] = 0;
				fOut[1] = 0;
				//do filter
				KorgThreeFiveHPFmkIIdoFilter(lFilterIn, fOut);
				lIn[i] = fOut[0];
				rIn[i] = fOut[1];

				i++;
				samplesToProcess--;
			}

			//if (gainMult > 1.f) gainMult = 1.f + (gainMult - 1.f) * 0.125f;
			//if (gainMult != 0.f) filterBlock.multiplyBy(1.0f / gainMult); //redo the original gain when using drive
			if (outputGain != 1.f) filterBlock.multiplyBy(outputGain);

			break;  //is smoothed??
		}
		//===============================================================================================================================
	case(FILTERTYPE::COMBFILTER):	   //COMB FILTER
	case(FILTERTYPE::SCREAMFILTER): {  //SCREAM FILTER
		if (bTypeChanged) {
			if (!isUI) {
				m_bqEndLowShelfL1.setCoefficients(coeff.makeHighPass(m_Set->m_nSampleRate, 30.f, sqrt2over2));
				m_bqEndLowShelfR1.setCoefficients(coeff.makeHighPass(m_Set->m_nSampleRate, 30.f, sqrt2over2));
				m_bqEndLowShelfL2.setCoefficients(coeff.makeHighPass(m_Set->m_nSampleRate, 30.f, sqrt2over2));
				m_bqEndLowShelfR2.setCoefficients(coeff.makeHighPass(m_Set->m_nSampleRate, 30.f, sqrt2over2));
				m_ladderFilter1.reset();
			}
			switch (paramType) {
			case(FILTERTYPE::SCREAMFILTER): {
				m_ladderFilter1.setMode(dsp::LadderFilter<float>::Mode::LPF12);
				break;
			}
			}
		}

		int i = 0;
		int samplesToProcess = numSamples;
		while (samplesToProcess >= C_FILTER_BLOCKSIZE) {
			matrixInputState->currentFrame = startSample + i;
			float lFilterEnvMod = paramFilterEnvMod;
			if (isUI) {
				fVCFEnvelopeMod = 1.0f;
			}
			else {
				if (paramFilterEnv == static_cast<int>(MSEGENV::MSEG1)) {
					fVCFEnvelopeMod = routingBuffers.MSEGBuffer[0][matrixInputState->voiceNo]->getSample(0, matrixInputState->currentFrame);
				}
				else if (paramFilterEnv == static_cast<int>(MSEGENV::MSEG2)) {
					fVCFEnvelopeMod = routingBuffers.MSEGBuffer[1][matrixInputState->voiceNo]->getSample(0, matrixInputState->currentFrame);
				}
				else if (paramFilterEnv == static_cast<int>(MSEGENV::MSEG3)) {
					fVCFEnvelopeMod = routingBuffers.MSEGBuffer[2][matrixInputState->voiceNo]->getSample(0, matrixInputState->currentFrame);
				}
				else if (paramFilterEnv == static_cast<int>(MSEGENV::MSEG4)) {
					fVCFEnvelopeMod = routingBuffers.MSEGBuffer[3][matrixInputState->voiceNo]->getSample(0, matrixInputState->currentFrame);
				}
				else if (paramFilterEnv == static_cast<int>(MSEGENV::MSEG5)) {
					fVCFEnvelopeMod = routingBuffers.MSEGBuffer[4][matrixInputState->voiceNo]->getSample(0, matrixInputState->currentFrame);
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

			double cutoff_target = 50.0;
			double scale_target = 70.0;
			double drive_target = 20.0;
			double gain_target = 100.0;
			double resonance_target = 100.0;
			m_combFilter.parameterChanged("m_fCombDryWet", 100.f, true, C_FILTER_BLOCKSIZE - 1); //0 .. 100
			if (isUI) {
				m_combFilter.parameterChanged("m_fCombFrequOffset", paramCutoff, m_bSmoothersTakeNextValue, C_FILTER_BLOCKSIZE - 1); //50..18000
				m_combFilter.parameterChanged("m_fCombLevel", paramScale * 0.125f + 75.f, m_bSmoothersTakeNextValue, C_FILTER_BLOCKSIZE - 1); //70 .. 100
				m_combFilter.parameterChanged("m_fCombDrive", paramDrive, m_bSmoothersTakeNextValue, C_FILTER_BLOCKSIZE - 1); //-20 .. 20
				m_combFilter.parameterChanged("m_fCombGain", paramGain, m_bSmoothersTakeNextValue, C_FILTER_BLOCKSIZE - 1); //0..200

				if (paramType == FILTERTYPE::SCREAMFILTER) {
					m_ladderFilter1.setCutoffFrequencyHz(paramCutoff + 1.f); //is smoothed
					m_ladderFilter1.setResonance(paramResonance / 100.f); //is smoothed
				}
			}
			else {
				cutoff_target = m_Set->getParameterValueWithMatrixModulation(&paramCutoff, paramIDCutoff, matrixInputState) * fVCFEnvelopeMod;
				m_combFilter.parameterChanged("m_fCombFrequOffset", cutoff_target, m_bSmoothersTakeNextValue, C_FILTER_BLOCKSIZE - 1); //50..18000
				scale_target = m_Set->getParameterValueWithMatrixModulation(&paramScale, paramIDScale, matrixInputState);
				m_combFilter.parameterChanged("m_fCombLevel", scale_target * 0.125f + 75.f, m_bSmoothersTakeNextValue, C_FILTER_BLOCKSIZE - 1); //70 .. 100
				drive_target = m_Set->getParameterValueWithMatrixModulation(&paramDrive, paramIDDrive, matrixInputState);
				m_combFilter.parameterChanged("m_fCombDrive", drive_target, m_bSmoothersTakeNextValue, C_FILTER_BLOCKSIZE - 1); //-20 .. 20
				gain_target = m_Set->getParameterValueWithMatrixModulation(&paramGain, paramIDGain, matrixInputState);
				m_combFilter.parameterChanged("m_fCombGain", gain_target, m_bSmoothersTakeNextValue, C_FILTER_BLOCKSIZE - 1); //0..200
				resonance_target = m_Set->getParameterValueWithMatrixModulation(&paramResonance, paramIDResonance, matrixInputState);

				if (paramType == FILTERTYPE::SCREAMFILTER) {
					m_ladderFilter1.setCutoffFrequencyHz(cutoff_target + 1.f); //is smoothed
					m_ladderFilter1.setResonance(resonance_target / 100.f); //is smoothed
				}
			}

			m_useOversampling = true; //TODO
			if (!m_useOversampling) {
			}
			else {
				//upsample
				dsp::AudioBlock<float> inBlock = filterBlock.getSubBlock(i, C_FILTER_BLOCKSIZE);
				dsp::AudioBlock<float> inBlockUp = dsp::AudioBlock<float>(*inBufferUp);
				m_Oversampler.upsampleAudioBuffer4(inBlock, inBlockUp, C_FILTER_BLOCKSIZE); //only 4 as oversampling??

				//process
				if (paramType == FILTERTYPE::SCREAMFILTER) {
					m_ladderFilter1.process(dsp::ProcessContextReplacing<float>(inBlockUp));
				}
				m_combFilter.processBlock(inBlockUp, C_FILTER_BLOCKSIZE * C_FILTER_OVERSAMPLINGFACTOR);

				//downsample
				m_Oversampler.downsampleAudioBuffer4(inBlockUp, inBlock, C_FILTER_BLOCKSIZE * C_FILTER_OVERSAMPLINGFACTOR);
			}
			samplesToProcess -= C_FILTER_BLOCKSIZE;
			i += C_FILTER_BLOCKSIZE;
			m_bSmoothersTakeNextValue = false;
		}
		if (samplesToProcess > 0) { //do rest
			if (!isUI) {
			}
			if (!m_useOversampling) { //without oversampling	
			}
			else {
				//upsample
				dsp::AudioBlock<float> inBlock = filterBlock.getSubBlock(i, samplesToProcess);
				dsp::AudioBlock<float> inBlockUp = dsp::AudioBlock<float>(*inBufferUp);
				m_Oversampler.upsampleAudioBuffer4(inBlock, inBlockUp, samplesToProcess);

				//process
				if (paramType == FILTERTYPE::SCREAMFILTER) {
					dsp::ProcessContextReplacing<float>::AudioBlockType _block = inBlockUp.getSubBlock(0, samplesToProcess * C_FILTER_OVERSAMPLINGFACTOR);
					dsp::ProcessContextReplacing<float> _context = dsp::ProcessContextReplacing<float>(_block);
					m_ladderFilter1.process(dsp::ProcessContextReplacing<float>(_context));
				}
				
				m_combFilter.processBlock(inBlockUp.getSubBlock(0, samplesToProcess * C_FILTER_OVERSAMPLINGFACTOR), samplesToProcess * C_FILTER_OVERSAMPLINGFACTOR);

				//downsample
				m_Oversampler.downsampleAudioBuffer4(inBlockUp, inBlock, samplesToProcess * C_FILTER_OVERSAMPLINGFACTOR);
			}
		}

		break;
	}

	//===============================================================================================================================
	case(FILTERTYPE::DQFLPF12): { 
		//vassertfalse;
		//UI?
		break;  
	}


	//===============================================================================================================================
	}

	//end low shelf
	if (!isUI) {
		m_bqEndLowShelfL1.processSamples(filterBlock.getChannelPointer(0), numSamples);
		m_bqEndLowShelfR1.processSamples(filterBlock.getChannelPointer(1), numSamples);
		m_bqEndLowShelfL2.processSamples(filterBlock.getChannelPointer(0), numSamples);
		m_bqEndLowShelfR2.processSamples(filterBlock.getChannelPointer(1), numSamples);
	}
}
//===============================================================================================================================

*/

/*
void CVASTVcf::LPFReset() {
	// flush everything
	m_LeftLPF_Order12.flushDelays();
	m_RightLPF_Order12.flushDelays();
	m_LeftLPF_Order34.flushDelays();
	m_RightLPF_Order34.flushDelays();
	m_LeftLPF_Order56.flushDelays();
	m_RightLPF_Order56.flushDelays();
}
*/

void CVASTVcf::HPFReset() {
	// flush everything
	m_TFHPF_LHPF1.reset();
	m_TFHPF_LLPF1.reset();
	m_TFHPF_LHPF3.reset();
	m_TFHPF_RHPF1.reset();
	m_TFHPF_RLPF1.reset();
	m_TFHPF_RHPF3.reset();
}

void CVASTVcf::SetQFB(VASTQFilterProcessState* Q, int e, float gain, float drive, float feedback, int ftype, int fsubtype, int ftype2, int fsubtype2, int ftype3, int fsubtype3, int numSamples) // Q == 0 means init(ialise)
{
	fbq = Q;
	fbqi = e;

	float bsize_inv = 1.f / numSamples;

	if (Q) {
		Q->FU[0].active[e] = 0xffffffff;
		Q->FU[1].active[e] = 0xffffffff;
		Q->FU[2].active[e] = 0xffffffff;
		Q->FU[3].active[e] = 0xffffffff;
		Q->FU[4].active[e] = 0xffffffff;
		Q->FU[5].active[e] = 0xffffffff;

		if (bfirstRun) {
			FBP.Gain = gain;
			FBP.Drive = drive;
			FBP.FB = feedback;
			bfirstRun = false;
		}

		set1f(Q->Gain, e, FBP.Gain); 
		set1f(Q->dGain, e, (gain - FBP.Gain) * bsize_inv); 
		set1f(Q->Drive, e, /* FBP.Drive */ drive);
		set1f(Q->dDrive, e, /*(Drive - FBP.Drive) * BLOCK_SIZE_OS_INV*/ (drive - FBP.Drive) * bsize_inv);
		set1f(Q->FB, e, /*FBP.FB*/ feedback);
		set1f(Q->dFB, e, /*(FB - FBP.FB) * BLOCK_SIZE_OS_INV*/ (feedback - FBP.FB) * bsize_inv);
		set1f(Q->Mix1, e, /*FBP.Mix1*/ 1.f);
		set1f(Q->dMix1, e, /*(FMix1 - FBP.Mix1) * BLOCK_SIZE_OS_INV*/ 0.f);
		set1f(Q->Mix2, e, /*FBP.Mix2*/ 1.f);
		set1f(Q->dMix2, e, /*(FMix2 - FBP.Mix2) * BLOCK_SIZE_OS_INV*/ 0.f);

		FBP.Gain = gain;
		FBP.Drive = drive;
		FBP.FB = feedback;

		//FBPMix1 = FMix1;
		//FBPMix2 = FMix2;

		set1f(Q->OutL, e, /*FBP.OutL*/ 1.f);
		set1f(Q->dOutL, e, /*(ampL - FBP.OutL) * */ 0.f);
		set1f(Q->OutR, e, /*FBP.OutR*/ 0.f);
		set1f(Q->dOutR, e, /*(ampR - FBP.OutR) * */ 0.f);

		set1f(Q->Out2L, e, /*FBP.OutL*/ 0.f);
		set1f(Q->dOut2L, e, /*(ampL - FBP.OutL) * */ 0.f);
		set1f(Q->Out2R, e, /*FBP.OutR*/ 1.f);
		set1f(Q->dOut2R, e, /*(ampR - FBP.OutR) * */ 0.f);

		set1f(Q->wsLPF, e, /*FBP.wsLPF*/ FBP.wsLPF); // remember state
		set1f(Q->FBlineL, e, /*FBP.FBlineL*/ FBP.FBlineL);
		set1f(Q->FBlineR, e, /*FBP.FBlineR*/ FBP.FBlineR);

		//Filter A
		if (ftype != fut_none) {
			for (int i = 0; i < n_cm_coeffs; i++)
			{
				set1f(Q->FU[0].C[i], e, m_filterCoefficientMaker[0].C[i]);	//L
				set1f(Q->FU[0].dC[i], e, m_filterCoefficientMaker[0].dC[i]);
				set1f(Q->FU[1].C[i], e, m_filterCoefficientMaker[0].C[i]);	//R
				set1f(Q->FU[1].dC[i], e, m_filterCoefficientMaker[0].dC[i]);
			}
			for (int i = 0; i < n_filter_registers; i++)
			{
				//set1f(Q->FU[0].R[i], e, FBPR[0][i]);
				//set1f(Q->FU[1].R[i], e, FBPR[1][i]);
				set1f(Q->FU[0].R[i], e, FBP.FU[0].R[i]);
				set1f(Q->FU[1].R[i], e, FBP.FU[1].R[i]);
			}
			Q->FU[0].DB[e] = FBP.Delay[0];
			Q->FU[1].DB[e] = FBP.Delay[1];
			Q->FU[0].WP[e] = FBP.FU[0].WP;
			Q->FU[1].WP[e] = FBP.FU[1].WP;
			if (ftype == fut_lpmoog) {
				int poles = 0;
				if (fsubtype == st_moog6)
					poles = 0;
				else if (fsubtype == st_moog12)
					poles = 1;
				else if (fsubtype == st_moog18)
					poles = 2;
				else if (fsubtype == st_moog24)
					poles = 3;
				else vassertfalse;
				Q->FU[0].WP[0] = poles; // LPMoog's stores number of poles - 0 = 6, 1 = 12, 2 = 18, 3 = 24
				Q->FU[1].WP[0] = poles; // LPMoog's stores number of poles - 0 = 6, 1 = 12, 2 = 18, 3 = 24
			}
		}

		//Filter B
		if (ftype2 != fut_none) {
			for (int i = 0; i < n_cm_coeffs; i++)
			{
				set1f(Q->FU[2].C[i], e, m_filterCoefficientMaker[1].C[i]);	//L
				set1f(Q->FU[2].dC[i], e, m_filterCoefficientMaker[1].dC[i]);
				set1f(Q->FU[3].C[i], e, m_filterCoefficientMaker[1].C[i]);	//R
				set1f(Q->FU[3].dC[i], e, m_filterCoefficientMaker[1].dC[i]);
			}
			for (int i = 0; i < n_filter_registers; i++)
			{
				//set1f(Q->FU[2].R[i], e, FBPR[2][i]);
				//set1f(Q->FU[3].R[i], e, FBPR[3][i]);
				set1f(Q->FU[2].R[i], e, FBP.FU[2].R[i]);
				set1f(Q->FU[3].R[i], e, FBP.FU[3].R[i]);
			}
			Q->FU[2].DB[e] = FBP.Delay[2];
			Q->FU[3].DB[e] = FBP.Delay[3];
			Q->FU[2].WP[e] = FBP.FU[2].WP;
			Q->FU[3].WP[e] = FBP.FU[3].WP;
			if (ftype2 == fut_lpmoog) {		
				int poles = 0;
				if (fsubtype2 == st_moog6)
					poles = 0;
				else if (fsubtype2 == st_moog12)
					poles = 1;
				else if (fsubtype2 == st_moog18)
					poles = 2;
				else if (fsubtype2 == st_moog24)
					poles = 3;
				else vassertfalse;
				Q->FU[2].WP[0] = poles; // LPMoog's stores number of poles - 0 = 6, 1 = 12, 2 = 18, 3 = 24
				Q->FU[3].WP[0] = poles; // LPMoog's stores number of poles - 0 = 6, 1 = 12, 2 = 18, 3 = 24
			}
		}

		//Filter C
		if (ftype3 != fut_none) {
			for (int i = 0; i < n_cm_coeffs; i++)
			{
				set1f(Q->FU[4].C[i], e, m_filterCoefficientMaker[2].C[i]);	//L
				set1f(Q->FU[4].dC[i], e, m_filterCoefficientMaker[2].dC[i]);
				set1f(Q->FU[5].C[i], e, m_filterCoefficientMaker[2].C[i]);	//R
				set1f(Q->FU[5].dC[i], e, m_filterCoefficientMaker[2].dC[i]);
			}
			for (int i = 0; i < n_filter_registers; i++)
			{
				//set1f(Q->FU[4].R[i], e, FBPR[4][i]);
				//set1f(Q->FU[5].R[i], e, FBPR[5][i]);
				set1f(Q->FU[4].R[i], e, FBP.FU[4].R[i]);
				set1f(Q->FU[5].R[i], e, FBP.FU[5].R[i]);
			}
			Q->FU[4].DB[e] = FBP.Delay[4];
			Q->FU[5].DB[e] = FBP.Delay[5];
			Q->FU[4].WP[e] = FBP.FU[4].WP;
			Q->FU[5].WP[e] = FBP.FU[5].WP;
			if (ftype3 == fut_lpmoog) {
				int poles = 0;
				if (fsubtype3 == st_moog6)
					poles = 0;
				else if (fsubtype3 == st_moog12)
					poles = 1;
				else if (fsubtype3 == st_moog18)
					poles = 2;
				else if (fsubtype3 == st_moog24)
					poles = 3;
				else vassertfalse;
				Q->FU[4].WP[0] = poles; // LPMoog's stores number of poles - 0 = 6, 1 = 12, 2 = 18, 3 = 24
				Q->FU[5].WP[0] = poles; // LPMoog's stores number of poles - 0 = 6, 1 = 12, 2 = 18, 3 = 24
			}
		}
	}
}

void CVASTVcf::GetQFB()
{
	jassert(fbq != nullptr);

	//Filter A
	for (int i = 0; i < n_filter_registers; i++)
	{
		FBP.FU[0].R[i] = get1f(fbq->FU[0].R[i], fbqi); //L
		FBP.FU[1].R[i] = get1f(fbq->FU[1].R[i], fbqi); //R
	}
	for (int i = 0; i < n_cm_coeffs; i++)
	{
		m_filterCoefficientMaker[0].C[i] = get1f(fbq->FU[0].C[i], fbqi); //L
		//m_filterCoefficientMaker[0].C[i] = get1f(fbq->FU[1].C[i], fbqi); //R
	}
	FBP.FU[0].WP = fbq->FU[0].WP[fbqi];
	FBP.FU[1].WP = fbq->FU[1].WP[fbqi];

	//Filter B
	for (int i = 0; i < n_filter_registers; i++)
	{
		FBP.FU[2].R[i] = get1f(fbq->FU[2].R[i], fbqi); //L
		FBP.FU[3].R[i] = get1f(fbq->FU[3].R[i], fbqi); //R
	}
	for (int i = 0; i < n_cm_coeffs; i++)
	{
		m_filterCoefficientMaker[1].C[i] = get1f(fbq->FU[2].C[i], fbqi); //L
		//m_filterCoefficientMaker[1].C[i] = get1f(fbq->FU[3].C[i], fbqi); //R
	}
	FBP.FU[2].WP = fbq->FU[2].WP[fbqi];
	FBP.FU[3].WP = fbq->FU[3].WP[fbqi];

	//Filter C
	for (int i = 0; i < n_filter_registers; i++)
	{
		FBP.FU[4].R[i] = get1f(fbq->FU[4].R[i], fbqi); //L
		FBP.FU[5].R[i] = get1f(fbq->FU[5].R[i], fbqi); //R
	}
	for (int i = 0; i < n_cm_coeffs; i++)
	{
		m_filterCoefficientMaker[2].C[i] = get1f(fbq->FU[4].C[i], fbqi); //L
		//m_filterCoefficientMaker[2].C[i] = get1f(fbq->FU[5].C[i], fbqi); //R
	}
	FBP.FU[4].WP = fbq->FU[4].WP[fbqi];
	FBP.FU[5].WP = fbq->FU[5].WP[fbqi];

	FBP.FBlineL = get1f(fbq->FBlineL, fbqi);
	FBP.FBlineR = get1f(fbq->FBlineR, fbqi);
	FBP.wsLPF = get1f(fbq->wsLPF, fbqi);
}
//************************************
