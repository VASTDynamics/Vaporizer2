/*
VAST Dynamics Audio Software (TM)
*/
#pragma once

#include "../VASTEngineHeader.h"
#include "../VASTPluginConstants.h"
#ifdef VAST_FFT_MUFFT
	#include "../../muFFT/fft.h"
	#include "../../muFFT/fft_internal.h"
#endif
#include <math.h>
#include <stdlib.h>
#include <string.h>
using namespace std;

class VASTFFTSingletonFactory {
public:
	VASTFFTSingletonFactory();
	~VASTFFTSingletonFactory();

	void performFFT(const juce::dsp::Complex<float>* input, juce::dsp::Complex<float>* output, bool inverse);

#ifdef VAST_FFT_MUFFT
	mufft_plan_1d* muplanInverse2048 = nullptr;
	mufft_plan_1d* muplanForward2048 = nullptr;
#endif

#ifdef VAST_FFT_FFTW3
	juce::dsp::FFT m_fft{ static_cast<int>(log2(C_WAVE_TABLE_SIZE)) };
#endif

private:
	CriticalSection FFTLock;

	JUCE_LEAK_DETECTOR(VASTFFTSingletonFactory)
};
