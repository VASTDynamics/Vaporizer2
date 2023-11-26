/*
VAST Dynamics Audio Software (TM)
*/

#include "VASTFFTSingletonFactory.h"
#ifdef VAST_FFT_MUFFT
	#include "../../muFFT/fft.h"
	#include "../../muFFT/fft_internal.h"
#endif
#include "../VASTPluginConstants.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
using namespace std;

VASTFFTSingletonFactory::VASTFFTSingletonFactory() {
#ifdef VAST_FFT_MUFFT
	int flags = 0;
	muplanInverse2048 = mufft_create_plan_1d_c2c(C_WAVE_TABLE_SIZE, MUFFT_INVERSE, flags);
	muplanForward2048 = mufft_create_plan_1d_c2c(C_WAVE_TABLE_SIZE, MUFFT_FORWARD, flags);
#endif

//#ifdef VAST_FFT_FFTW3
//	m_fft.reset(new dsp::FFT(log2(int(C_WAVE_TABLE_SIZE))));
//#endif
}

VASTFFTSingletonFactory::~VASTFFTSingletonFactory() {
#ifdef VAST_FFT_MUFFT
	if (muplanInverse2048 != nullptr)
		mufft_free_plan_1d(muplanInverse2048);
	if (muplanForward2048 != nullptr)
		mufft_free_plan_1d(muplanForward2048);
#endif

#ifdef VAST_FFT_FFTW3
	//m_fft = nullptr;
#endif
}

void VASTFFTSingletonFactory::performFFT(const juce::dsp::Complex<float>* input, juce::dsp::Complex<float>* output, bool inverse) {
	
#ifdef VAST_FFT_MUFFT
	ScopedLock sl(FFTLock);
	if (inverse)
		mufft_execute_plan_1d(muplanInverse2048, output, input); //frequencyDomainBufferFromNaive >> muplanInverse2048
	else 
		mufft_execute_plan_1d(muplanForward2048, output, input); //makeWaveTableFreq >> muplanForward2048	
	
#ifdef _DEBUG
		if (abs(output[0].real()) > 2.f)
			VDBG("Check mufft output - seams to be broken!");
	#endif
#endif

#ifdef VAST_FFT_FFTW3
	m_fft.perform(input, output, inverse);

	if (inverse) {
		auto n = m_fft.getSize();
		FloatVectorOperations::multiply((float*)output, static_cast<float> (n), (int)n << 1); //why is juce doing it? Has to be undone here.
	}

#endif
}
