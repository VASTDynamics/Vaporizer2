#pragma once
#include "../Utils/VASTSynthfunctions.h"

class VASTQFilterCoefficients
{
public:
	
	//static const int n_cm_coeffs = 8; //in synthfunctions
	float samplerate = 111.f; //something weird 
	double dmasterTuneHz = 11.f; //something weird 
	double dsamplerate_os = 2.f * samplerate; //CHVAST
	double dsamplerate_os_inv = 1.f / dsamplerate_os; //CHVAST
	double samplerate_inv = 1.f / samplerate;
	
	float C[n_cm_coeffs], dC[n_cm_coeffs], tC[n_cm_coeffs]; // K1,K2,Q1,Q2,V1,V2,V3,etc

	VASTQFilterCoefficients();
	void MakeCoeffs(float Freq, float Reso, int Type, int SubType, float Scale);
	void Reset();

	void prepareForPlay(double sampleRate, int osFactor, float masterTuneHz);
	void initTables();
	void note_to_omega(float x, float& sinu, float& cosi);
	float note_to_pitch(float x);
	float note_to_pitch_inv(float x);
	float db_to_linear(float x);
	float clipscale(float freq, int subtype);
	float pitch_to_note(float x);

	double Map2PoleResonance(double reso, double freq, int subtype);
	double Map2PoleResonance_noboost(double reso, double freq, int subtype);
	double Map4PoleResonance(double reso, double freq, int subtype);
	double resoscale(double reso, int subtype);
	double resoscale4Pole(double reso, int subtype);

	float table_pitch alignas(16)[512];
	float table_note_omega alignas(16)[2][512];
	float table_dB alignas(16)[512];
	float table_pitch_inv alignas(16)[512];	
	
private:
	void FromDirect(float N[n_cm_coeffs]);
	void
	ToCoupledForm(double A0inv, double A1, double A2, double B0, double B1, double B2, double G);
	void ToNormalizedLattice(
		double A0inv, double A1, double A2, double B0, double B1, double B2, double G);
	void Coeff_LP12(float Freq, float Reso, int SubType);
	void Coeff_HP12(float Freq, float Reso, int SubType);
	void Coeff_BP12(float Freq, float Reso, int SubType);
	void Coeff_BR12(float Freq, float Reso, int SubType);
	void Coeff_LP24(float Freq, float Reso, int SubType);
	void Coeff_HP24(float Freq, float Reso, int SubType);
	void Coeff_LP4L(float Freq, float Reso, int SubType);
	void Coeff_COMB(float Freq, float Reso, int SubType);
	void Coeff_SNH(float Freq, float Reso, int SubType);
	void Coeff_SVF(float Freq, float Reso, bool);
   
	void Coeff_VASTIIR(float Freq, float Reso, float Scale, int SubType);
	void Coeff_VASTsvf(float Freq, float Reso, float Scale, int SubType);
	void Coeff_VASTldf(float Freq, float Reso, float Scale, int SubType);

	bool FirstRun;
};