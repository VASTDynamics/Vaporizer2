#include "VASTQFilterCoefficients.h"

#include "../VASTEngineHeader.h"
#include "../Utils/VASTSynthfunctions.h"  //for pow
#ifdef __aarch64__ //arm64
	#include "../../sse2neon.h"
#else
	#include "emmintrin.h"
#endif
#include <stdio.h>
#include <string.h>
#include <math.h>
using namespace std;

JUCE_BEGIN_IGNORE_WARNINGS_GCC_LIKE("-Wconversion")
JUCE_BEGIN_IGNORE_WARNINGS_MSVC(4244 4267) //C4244 conversion from 'type1' to 'type2', possible loss of data //C4267 conversion

void VASTQFilterCoefficients::note_to_omega(float x, float& sinu, float& cosi)
{
	x += 256;
	int e = (int)x;
	float a = x - (float)e;

	if (e > 0x1fe)
		e = 0x1fe;
	else if (e < 0)
		e = 0;

	sinu = (1 - a) * table_note_omega[0][e & 0x1ff] + a * table_note_omega[0][(e + 1) & 0x1ff];
	cosi = (1 - a) * table_note_omega[1][e & 0x1ff] + a * table_note_omega[1][(e + 1) & 0x1ff];
}

float VASTQFilterCoefficients::note_to_pitch(float x)
{
	x += 256;
	int e = (int)x;
	float a = x - (float)e;

	if (e > 0x1fe)
		e = 0x1fe;

	return (1 - a) * table_pitch[e & 0x1ff] + a * table_pitch[(e + 1) & 0x1ff];
}

float VASTQFilterCoefficients::pitch_to_note(float x) {
	//17.9 -> 50
	//m_Set->midiNoteGetBaseFreq(uMIDINote, m_fOscMasterTune)

	float divi = logf(x) / logf(2.f);	
	float note = divi * 12.f;	

	return note;
}

float VASTQFilterCoefficients::note_to_pitch_inv(float x)
{
	x += 256;
	int e = (int)x;
	float a = x - (float)e;

	if (e > 0x1fe)
		e = 0x1fe;

	return (1 - a) * table_pitch_inv[e & 0x1ff] + a * table_pitch_inv[(e + 1) & 0x1ff];
}

float VASTQFilterCoefficients::db_to_linear(float x)
{
	x += 384;
	int e = (int)x;
	float a = x - (float)e;

	return (1 - a) * table_dB[e & 0x1ff] + a * table_dB[(e + 1) & 0x1ff];
}

const float smooth = 0.2f;

VASTQFilterCoefficients::VASTQFilterCoefficients()
{
   Reset();
}

void VASTQFilterCoefficients::prepareForPlay(double sampleRate, int osFactor, float masterTuneHz)
{
	bool bChanged = false;
	if (!approximatelyEqual(float(sampleRate), samplerate) ||
		!approximatelyEqual(float(dmasterTuneHz), masterTuneHz))
		bChanged = true;

	samplerate = sampleRate;
	dsamplerate_os = osFactor * samplerate; //2.f * samplerate; 
	dsamplerate_os_inv = 1.f / dsamplerate_os; 
	samplerate_inv = 1.f / samplerate;
	dmasterTuneHz = masterTuneHz;

	if (bChanged) 
		initTables();
	Reset();
}

void VASTQFilterCoefficients::initTables() {
	for (int i = 0; i < 512; i++)
	{
		table_dB[i] = powf(10.f, 0.05f * ((float)i - 384.f));
		table_pitch[i] = powf(2.f, ((float)i - 256.f) * (1.f / 12.f));
		table_pitch_inv[i] = 1.f / table_pitch[i];
		table_note_omega[0][i] =
			(float)sin(2 * M_PI * min(0.5, dmasterTuneHz * table_pitch[i] * dsamplerate_os_inv));
		table_note_omega[1][i] =
			(float)cos(2 * M_PI * min(0.5, dmasterTuneHz * table_pitch[i] * dsamplerate_os_inv));
	}
}

void VASTQFilterCoefficients::MakeCoeffs(float Freq, float Reso, int Type, int SubType, float Scale)
{
   switch (Type)
   {
   case fut_vastiir6:
		Coeff_VASTIIR(Freq, Reso, Scale, SubType);
	   break;   
   case fut_vastsvf:
	   Coeff_VASTsvf(Freq, Reso, Scale, SubType);
	   break;     
   case fut_vastldf:   
	   Coeff_VASTldf(Freq, Reso, Scale, SubType);
	   break;
   case fut_lp12:
      if (SubType == st_SVF)
         Coeff_SVF(Freq, Reso, false);
      else
         Coeff_LP12(Freq, Reso, SubType);
      break;
   case fut_hp12:
      if (SubType == st_SVF)
         Coeff_SVF(Freq, Reso, false);
      else
         Coeff_HP12(Freq, Reso, SubType);
      break;
   case fut_bp12:
      if ((SubType == st_SVF) || (SubType == 3))
         Coeff_SVF(Freq, Reso, false);
      else
         Coeff_BP12(Freq, Reso, SubType);
      break;
   case fut_br12:
      Coeff_BR12(Freq, Reso, SubType);
      break;
   case fut_lp24:
      if (SubType == st_SVF)
         Coeff_SVF(Freq, Reso, true);
      else
         Coeff_LP24(Freq, Reso, SubType);
      break;
   case fut_hp24:
      if (SubType == st_SVF)
         Coeff_SVF(Freq, Reso, true);
      else
         Coeff_HP24(Freq, Reso, SubType);
      break;
   case fut_lpmoog:
   
      Coeff_LP4L(Freq, Reso, SubType);
      break;
   case fut_comb:   
      Coeff_COMB(Freq, Reso, SubType);
      break;
   case fut_SNH:
      Coeff_SNH(Freq, Reso, SubType);
      break;
   
   case fut_vastcomb:	   
   case fut_vastscream:
   default:
	   vassertfalse; //error
	   break;
   };
}

float VASTQFilterCoefficients::clipscale(float freq, int subtype)
{
   switch (subtype)
   {
   case st_Rough:
      return (1.0f / 64.0f) * db_to_linear(freq * 0.55f);
   case st_Smooth:
      return (1.0f / 1024.0f); // * db_to_linear(freq*0.55f);
   };
   return 0;
}

#define boundfreq(freq)                                                                            \
   {                                                                                               \
      if (freq > 75)                                                                               \
         freq = 75;                                                                                \
      if (freq < -55)                                                                              \
         freq = -55;                                                                               \
   }

double VASTQFilterCoefficients::Map2PoleResonance(double reso, double freq, int subtype)
{
   switch (subtype)
   {
   case st_Medium:
      reso *= max(0.0, 1.0 - max(0.0, (freq - 58) * 0.05));
      return (0.99 - 1.0 * limit_range((float)(1 - (1 - reso) * (1 - reso)), 0.0f, 1.0f));
   case st_Rough:
      reso *= max(0.0, 1.0 - max(0.0, (freq - 58) * 0.05));
      return (1.0 - 1.05 * limit_range((float)(1 - (1 - reso) * (1 - reso)), 0.001f, 1.0f));
   default:
   case st_Smooth:
      return (2.5 - 2.45 * limit_range((float)(1 - (1 - reso) * (1 - reso)), 0.0f, 1.0f));
   }
   return 0;
}

double VASTQFilterCoefficients::Map2PoleResonance_noboost(double reso, double freq, int subtype)
{
   if (subtype == st_Rough)
	   return (1.0 - 0.99 * jlimit<double>(0.001, 1.0, 1 - (1 - reso) * (1 - reso)));
      //return (1.0 - 0.99 * limit_range((double)(1 - (1 - reso) * (1 - reso)), 0.001, 1.0));
   else
	   return (0.99 - 0.98 * jlimit<double>(0.0, 1.0, 1 - (1 - reso) * (1 - reso)));
      //return (0.99 - 0.98 * limit_range((double)(1 - (1 - reso) * (1 - reso)), 0.0, 1.0));
}

double VASTQFilterCoefficients::Map4PoleResonance(double reso, double freq, int subtype)
{
   switch (subtype)
   {
   case st_Medium:
      reso *= max(0.0f, 1.0f - max(0.0f, (float(freq) - 58.f) * 0.05f));
      return 0.99f - 0.9949f * limit_range((double)reso, 0.0f, 1.0f); // sqrt(1.01) = 1.004987562
   case st_Rough:
      reso *= max(0.0f, 1.0f - max(0.0f, (float(freq) - 58.f) * 0.05f));
      return (1.0f - 1.05f * limit_range((double)reso, 0.001f, 1.0f));
   default:
   case st_Smooth:
      return (2.5f - 2.3f * limit_range((double)reso, 0.0f, 1.0f));
   }
}

double VASTQFilterCoefficients::resoscale(double reso, int subtype)
{
   switch (subtype)
   {
   case st_Medium:
      return (1.0f - 0.75f * reso * reso);
   case st_Rough:
      return (1.0f - 0.5f * reso * reso);
   case st_Smooth:
      return (1.0f - 0.25f * reso * reso);
   }

   return 1.0;
}

double VASTQFilterCoefficients::resoscale4Pole(double reso, int subtype)
{
   switch (subtype)
   {
   case st_Medium:
      return (1.0 - 0.75 * reso);
   case st_Rough:
      return (1.0 - 0.5 * reso * reso);
   case st_Smooth:
      return (1.0 - 0.5 * reso);
   }

   return 1.0;
}

void VASTQFilterCoefficients::Coeff_VASTIIR(float Freq, float Reso, float Scale, int SubType) {
	IIRCoefficients coeff; //Coefficient variable
	if (SubType == st_iir_bp)
		coeff = IIRCoefficients::makeBandPass(dsamplerate_os, (Freq <= dsamplerate_os * 0.5 ? Freq : dsamplerate_os * 0.5), Reso);   
	else if (SubType == st_iir_lp)
		coeff = IIRCoefficients::makeLowPass(dsamplerate_os, (Freq <= dsamplerate_os * 0.5 ? Freq : dsamplerate_os * 0.5), Reso);
	else if (SubType == st_iir_ap)
		coeff = IIRCoefficients::makeAllPass(dsamplerate_os, (Freq <= dsamplerate_os * 0.5 ? Freq : dsamplerate_os * 0.5), Reso);
	else if (SubType == st_iir_hp)
		coeff = IIRCoefficients::makeHighPass(dsamplerate_os, (Freq <= dsamplerate_os * 0.5 ? Freq : dsamplerate_os * 0.5), Reso);
	else if (SubType == st_iir_no)
		coeff = IIRCoefficients::makeNotchFilter(dsamplerate_os, (Freq <= dsamplerate_os * 0.5 ? Freq : dsamplerate_os * 0.5), Reso);
	else if (SubType == st_iir_hs)
		coeff = IIRCoefficients::makeHighShelf(dsamplerate_os, (Freq <= dsamplerate_os * 0.5 ? Freq : dsamplerate_os * 0.5), Reso, Scale);
	else if (SubType == st_iir_ls)
		coeff = IIRCoefficients::makeLowShelf(dsamplerate_os, (Freq <= dsamplerate_os * 0.5 ? Freq : dsamplerate_os * 0.5), Reso, Scale);
	else if (SubType == st_iir_pk)
		coeff = IIRCoefficients::makePeakFilter(dsamplerate_os, (Freq <= dsamplerate_os * 0.5 ? Freq : dsamplerate_os * 0.5), Reso, Scale);

	float c[n_cm_coeffs];
	c[0] = coeff.coefficients[0];
	c[1] = coeff.coefficients[1];
	c[2] = coeff.coefficients[2];
	c[3] = coeff.coefficients[3];
	c[4] = coeff.coefficients[4];
	FromDirect(c);
}

void VASTQFilterCoefficients::Coeff_VASTldf(float Freq, float Reso, float Scale, int SubType) {

	float c[n_cm_coeffs];
	
	/*
	switch (newValue)
	{
	case Mode::LPF12:   A = { { Type(0), Type(0),  Type(1), Type(0),  Type(0) } }; comp = Type(0.5);  break;
	case Mode::HPF12:   A = { { Type(1), Type(-2), Type(1), Type(0),  Type(0) } }; comp = Type(0);    break;
	case Mode::LPF24:   A = { { Type(0), Type(0),  Type(0), Type(0),  Type(1) } }; comp = Type(0.5);  break;
	case Mode::HPF24:   A = { { Type(1), Type(-4), Type(6), Type(-4), Type(1) } }; comp = Type(0);    break;
	default:            jassertfalse;                                                                     break;
	}
	*/

	switch (SubType)
	{
	case st_vastldf_LPF12:   
		c[3] = 0.f; c[4] = 0.f; c[5] = 1.f; c[6] = 0.f; c[7] = 0.f; c[2] = 0.5f; //A = { { Type(0), Type(0),  Type(1), Type(0),  Type(0) } }; comp = Type(0.5);  break;
		break;
	case st_vastldf_HPF12:   
		c[3] = 1.f; c[4] = -2.f; c[5] = 1.f; c[6] = 0.f; c[7] = 0.f; c[2] = 0.f;	//A = { { Type(1), Type(-2), Type(1), Type(0),  Type(0) } }; comp = Type(0);    break;
		break;
	case st_vastldf_LPF24:   
		c[3] = 0.f; c[4] = 0.f; c[5] = 0.f; c[6] = 0.f; c[7] = 1.f; c[2] = 0.5f; //A = { { Type(0), Type(0),  Type(0), Type(0),  Type(1) } }; comp = Type(0.5);  break;
		break;
	case st_vastldf_HPF24:   
		c[3] = 1.f; c[4] = -4.f; c[5] = 6.f; c[6] = -4.f; c[7] = 1.f; c[2] = 0.f; //A = { { Type(1), Type(-4), Type(6), Type(-4), Type(1) } }; comp = Type(0);    break;
		break;
	default: jassertfalse; 
		break;
	}

	/*
	static constexpr auto outputGain = 1.2f;
	for (int i = 3; i < 8; i++)
		c[i] *= outputGain;
	*/

	jassert(Freq > 0.f);
	c[0] = std::exp(Freq * (-2.0f * M_PI) / samplerate);

	jassert((Reso >= 0.) && (Reso < 1.f));
	c[1] = jmap(Reso, 0.1f, 1.0f);

	FromDirect(c);
}

void VASTQFilterCoefficients::Coeff_VASTsvf(float Freq, float Reso, float Scale, int SubType) {
	//from juce::dsp::StateVariableFilter	
	jassert(dsamplerate_os > 0);
	jassert(Reso > 0.f);
	jassert((Freq > 0.f) && (Freq <= (dsamplerate_os * 0.5f)));

	float c[n_cm_coeffs];

	c[0] = std::tan(MathConstants<double>::pi * Freq / dsamplerate_os); //g
	c[1] = (1.0 / Reso); //R2
	c[2] = (1.0 / (1.0 + c[1] * c[0] + c[0] * c[0])); //h

	FromDirect(c);
}

void VASTQFilterCoefficients::Coeff_SVF(float Freq, float Reso, bool FourPole)
{
   double f = dmasterTuneHz * note_to_pitch(Freq);
   double F1 = 2.0 * sin(M_PI * min(0.11, f * (0.25 * samplerate_inv)));

   // double Q1 = 2.0 - Reso*2.0;

   Reso = sqrt(limit_range(Reso, 0.f, 1.f));

   // double Q1 = min(2.0*(1.0 - pow(Reso, 0.25f)), min(2.0, 2.0/F1 - F1*0.5));
   double overshoot = FourPole ? 0.1 : 0.15;
   double Q1 = 2.0 - Reso * (2.0 + overshoot) + F1 * F1 * overshoot * 0.9;
   // Q1 = min(Q1, min(2.00, 1.99/F1 - F1*0.51));
   Q1 = min(Q1, min(2.00, 2.00 - 1.52 * F1));

   double ClipDamp = 0.1 * Reso * F1;

   const double a = 0.65;
   // double Gain = a + Q1 * 0.5 * (1-a);
   double Gain = 1 - a * Reso;

   float c[n_cm_coeffs];
   memset(c, 0, sizeof(float) * n_cm_coeffs);
   c[0] = F1;
   c[1] = Q1;
   c[2] = ClipDamp;
   c[3] = Gain;
   FromDirect(c);
}

void VASTQFilterCoefficients::Coeff_LP12(float freq, float reso, int subtype)
{
   float gain = resoscale(reso, subtype);

   boundfreq(freq)

       float cosi,
       sinu;
   note_to_omega(freq, sinu, cosi);

   double alpha = sinu * Map2PoleResonance(reso, freq, subtype);
   if (subtype != st_Smooth)
      alpha = min(alpha, sqrt(1.0 - cosi * cosi) - 0.0001);

   double b0 = (1 - cosi) * 0.5, b1 = 1 - cosi, b2 = (1 - cosi) * 0.5, a0 = 1 + alpha,
          a1 = -2 * cosi, a2 = 1 - alpha, a0inv = 1 / a0;

   // double sq = a1*a1 - 4.0*a2;
   // must be negative
   // dvs a2 > 0.25*a1*a1
   // 1-alpha / (1+alpha) > 0.25*(-2*cosi/(1+alpha))^2
   // 1-alpha > 0.25*(-2*cosi)^2 / (1+alpha)
   // (1-alpha)(1+alpha) > 0.25*(-2*cosi)^2
   // (1-alpha)(1+alpha) > (cosi)^2
   // >> alpha = +- sqrt(1 - cosi^2)

   // ar = 0.5 * -a1;
   // ai = 0.5 * sqrt(-sq);
   // ar = -(K*K - 1)/(1+alpha)
   // ai =

   if (subtype == st_Smooth)
      ToNormalizedLattice(a0inv, a1, a2, b0 * gain, b1 * gain, b2 * gain, clipscale(freq, subtype));
   else
      ToCoupledForm(a0inv, a1, a2, b0 * gain, b1 * gain, b2 * gain, clipscale(freq, subtype));
}

void VASTQFilterCoefficients::Coeff_LP24(float freq, float reso, int subtype)
{
   float gain = resoscale(reso, subtype);

   boundfreq(freq)

       double Q2inv = Map4PoleResonance((double)reso, (double)freq, subtype);
   float cosi, sinu;
   note_to_omega(freq, sinu, cosi);

   double alpha = sinu * Q2inv;
   if (subtype != st_Smooth)
      alpha = min(alpha, sqrt(1.0 - cosi * cosi) - 0.0001);

   double a0 = 1 + alpha, b0 = (1 - cosi) * 0.5, b1 = 1 - cosi, b2 = (1 - cosi) * 0.5,
          a0inv = 1 / a0, a1 = -2 * cosi, a2 = 1 - alpha;

   if (subtype == st_Smooth)
      ToNormalizedLattice(a0inv, a1, a2, b0 * gain, b1 * gain, b2 * gain, clipscale(freq, subtype));
   else
      ToCoupledForm(a0inv, a1, a2, b0 * gain, b1 * gain, b2 * gain, clipscale(freq, subtype));
}

void VASTQFilterCoefficients::Coeff_HP12(float freq, float reso, int subtype)
{
   float gain = resoscale(reso, subtype);

   boundfreq(freq)

       double Q2inv = Map2PoleResonance(reso, freq, subtype);
   float cosi, sinu;
   note_to_omega(freq, sinu, cosi);

   double alpha = sinu * Q2inv;
   if (subtype != 0)
      alpha = min(alpha, sqrt(1.0 - cosi * cosi) - 0.0001);

   double a0 = 1 + alpha, b0 = (1 + cosi) * 0.5, b1 = -(1 + cosi), b2 = (1 + cosi) * 0.5,
          a0inv = 1 / a0, a1 = -2 * cosi, a2 = 1 - alpha;

   if (subtype == st_Smooth)
      ToNormalizedLattice(a0inv, a1, a2, b0 * gain, b1 * gain, b2 * gain, clipscale(freq, subtype));
   else
      ToCoupledForm(a0inv, a1, a2, b0 * gain, b1 * gain, b2 * gain, clipscale(freq, subtype));
}

void VASTQFilterCoefficients::Coeff_HP24(float freq, float reso, int subtype)
{
   float gain = resoscale(reso, subtype);

   boundfreq(freq)

       double Q2inv = Map4PoleResonance((double)reso, (double)freq, subtype);
   float cosi, sinu;
   note_to_omega(freq, sinu, cosi);

   double alpha = sinu * Q2inv;
   if (subtype != 0)
      alpha = min(alpha, sqrt(1.0 - cosi * cosi) - 0.0001);

   double a0 = 1 + alpha, b0 = (1 + cosi) * 0.5, b1 = -(1 + cosi), b2 = (1 + cosi) * 0.5,
          a0inv = 1 / a0, a1 = -2 * cosi, a2 = 1 - alpha;

   if (subtype == st_Smooth)
      ToNormalizedLattice(a0inv, a1, a2, b0 * gain, b1 * gain, b2 * gain, clipscale(freq, subtype));
   else
      ToCoupledForm(a0inv, a1, a2, b0 * gain, b1 * gain, b2 * gain, clipscale(freq, subtype));
}

void VASTQFilterCoefficients::Coeff_BP12(float freq, float reso, int subtype)
{
   float gain = resoscale(reso, subtype);
   if (subtype == 1)
      gain *= 2.f;

   boundfreq(freq)

       double Q2inv = Map2PoleResonance(reso, freq, subtype);
   double Q = 0.5 / Q2inv;
   float cosi, sinu;
   note_to_omega(freq, sinu, cosi);

   double alpha = sinu * Q2inv;
   if (subtype != 0)
      alpha = min(alpha, sqrt(1.0 - cosi * cosi) - 0.0001);
   double a0 = 1 + alpha, b0, b1, b2, a0inv = 1 / a0, a1 = -2 * cosi, a2 = 1 - alpha;

   {
      b0 = Q * alpha;
      b1 = 0;
      b2 = -Q * alpha;
   }

   if (subtype == st_Smooth)
      ToNormalizedLattice(a0inv, a1, a2, b0 * gain, b1 * gain, b2 * gain, clipscale(freq, subtype));
   else
      ToCoupledForm(a0inv, a1, a2, b0 * gain, b1 * gain, b2 * gain, clipscale(freq, subtype));
}

void VASTQFilterCoefficients::Coeff_BR12(float freq, float reso, int subtype)
{
   boundfreq(freq)

       // double Q2inv = (2.5-2.45*limit_range((double)(1-(1-reso)*(1-reso)),0.0,1.0));
       double Q2inv;

   if (subtype == 1)
      Q2inv = (1.00 - 0.99 * limit_range((double)(1 - (1 - reso) * (1 - reso)), 0.0, 1.0));
   else
      Q2inv = (2.5 - 2.49 * limit_range((double)(1 - (1 - reso) * (1 - reso)), 0.0, 1.0));

   float cosi, sinu;
   note_to_omega(freq, sinu, cosi);

   double alpha = sinu * Q2inv, b0 = 1, b1 = -2 * cosi, b2 = 1, a0 = 1 + alpha, a1 = -2 * cosi,
          a2 = 1 - alpha, a0inv = 1 / a0;

   ToNormalizedLattice(a0inv, a1, a2, b0, b1, b2, 0.005);
}

void VASTQFilterCoefficients::Coeff_LP4L(float freq, float reso, int subtype)
{
   double gg =
       limit_range(((double)dmasterTuneHz * note_to_pitch(freq) * dsamplerate_os_inv), 0.0f, 0.187f); // gg

   float t_b1 = 1.f - exp(-2 * M_PI * gg);
   float q = min(2.15f * limit_range(reso, 0.f, 1.f), 0.5f / (t_b1 * t_b1 * t_b1 * t_b1));

   float c[n_cm_coeffs];
   memset(c, 0, sizeof(float) * n_cm_coeffs);
   c[0] = (3.f / (3.f - q));
   c[1] = t_b1;
   c[2] = q;   
   FromDirect(c);
}

void VASTQFilterCoefficients::Coeff_COMB(float freq, float reso, int subtype)
{
   float dtime = (1.f / dmasterTuneHz) * note_to_pitch(-freq);   
   dtime = dtime * dsamplerate_os -
           FIRoffset; // 1 sample for feedback, 1 sample for the IIR-filter without resonance
   dtime = limit_range(dtime, (float)FIRipol_N, (float)MAX_FB_COMB - FIRipol_N);   
   reso = ((subtype & 2) ? -1.0f : 1.0f) * limit_range(reso, 0.f, 1.f);

   float c[n_cm_coeffs];
   memset(c, 0, sizeof(float) * n_cm_coeffs);
   c[0] = dtime;
   c[1] = reso;
   c[2] = (subtype & 1) ? 0.0f : 0.5f; // combmix
   c[3] = 1.f - c[2];
   FromDirect(c);
}

void VASTQFilterCoefficients::Coeff_SNH(float freq, float reso, int)
{
   float dtime = (1.f / dmasterTuneHz) * note_to_pitch(-freq) * dsamplerate_os;
   double v1 = 1.0 / dtime;

   float c[n_cm_coeffs];
   memset(c, 0, sizeof(float) * n_cm_coeffs);
   c[0] = v1;
   c[1] = reso;
   FromDirect(c);
}

void VASTQFilterCoefficients::FromDirect(float N[n_cm_coeffs])
{
   if (FirstRun)
   {
      memset(dC, 0, sizeof(float) * n_cm_coeffs);
      memcpy(C, N, sizeof(float) * n_cm_coeffs);
      memcpy(tC, N, sizeof(float) * n_cm_coeffs);

      FirstRun = false;
   }
   else
   {
      for (int i = 0; i < n_cm_coeffs; i++)
      {
         tC[i] = (1.f - smooth) * tC[i] + smooth * N[i];
         dC[i] = (tC[i] - C[i]) * BLOCK_SIZE_OS_INV; //CHECK TODO!!!! Not always the same!!!!
      }
   }
}

void VASTQFilterCoefficients::ToNormalizedLattice(
    double a0inv, double a1, double a2, double b0, double b1, double b2, double g)
{
   b0 *= a0inv;
   b1 *= a0inv;
   b2 *= a0inv;
   a1 *= a0inv;
   a2 *= a0inv;

   float N[n_cm_coeffs];
   memset(N, 0, sizeof(float) * n_cm_coeffs);

   double k1 = a1 / (1.0 + a2);
   double k2 = a2;

   double q1 = 1.0 - k1 * k1;
   double q2 = 1.0 - k2 * k2;
   q1 = sqrt(fabs(q1));
   q2 = sqrt(fabs(q2));

   double v3 = b2;
   double v2 = (b1 - a1 * v3) / q2;
   double v1 = (b0 - k1 * v2 * q2 - k2 * v3) / (q1 * q2);

   N[0] = k1;
   N[1] = k2;
   N[2] = q1;
   N[3] = q2;
   N[4] = v1;
   N[5] = v2;
   N[6] = v3;
   N[7] = g;

   FromDirect(N);
}
void VASTQFilterCoefficients::ToCoupledForm(
    double a0inv, double a1, double a2, double b0, double b1, double b2, double g)
{
   b0 *= a0inv;
   b1 *= a0inv;
   b2 *= a0inv;
   a1 *= a0inv;
   a2 *= a0inv;

   float N[n_cm_coeffs];
   memset(N, 0, sizeof(float) * n_cm_coeffs);

   double ar, ai;
   double sq = a1 * a1 - 4.0 * a2;

   ar = 0.5 * -a1;
   sq = min(0.0, sq);
   ai = 0.5 * sqrt(-sq);
   ai = max(ai, 8.0 * 1.192092896e-07F);

   double bb1 = b1 - a1 * b0;
   double bb2 = b2 - a2 * b0;

   double d = b0;
   double c1 = bb1;
   double c2 = (bb1 * ar + bb2) / ai;

   //double scaler = 1.f; // 0.01 + 0.99*sqrt(c1*c1);

   N[0] = ar;
   N[1] = ai;
   N[2] = 1.f; // scaler;
   N[4] = c1;  // /scaler;
   N[5] = c2;  // /scaler;
   N[6] = d;
   N[7] = g;

   FromDirect(N);
}

void VASTQFilterCoefficients::Reset()
{
   FirstRun = true;
   memset(C, 0, sizeof(float) * n_cm_coeffs);
   memset(dC, 0, sizeof(float) * n_cm_coeffs);
   memset(tC, 0, sizeof(float) * n_cm_coeffs);
}

JUCE_END_IGNORE_WARNINGS_GCC_LIKE
JUCE_END_IGNORE_WARNINGS_MSVC
