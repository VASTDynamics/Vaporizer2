/*
VAST Dynamics Audio Software (TM)
*/
#pragma once

#include "../VASTEngineHeader.h"
//#include <mutex>
//#include <atomic>
#include "../../muFFT/fft.h"
#include "../../muFFT/fft_internal.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
using namespace std;

class VASTFFTSingletonFactory {
public:
	VASTFFTSingletonFactory();
	~VASTFFTSingletonFactory();

	mufft_plan_1d* muplanInverse2048 = nullptr;
	mufft_plan_1d* muplanForward2048 = nullptr;

	CriticalSection FFTLock;

	JUCE_LEAK_DETECTOR(VASTFFTSingletonFactory)
};


/*
class VASTFFTSingletonFactory {
public:
	static VASTFFTSingletonFactory* Instance();

	static void releaseInstance() {
		delete pinstance;
		pinstance = NULL;
	}

	mufft_plan_1d* muplanInverse2048 = nullptr;
	mufft_plan_1d* muplanForward2048 = nullptr;

private:
	VASTFFTSingletonFactory() {};
	~VASTFFTSingletonFactory() {
		if (muplanInverse2048 != nullptr)
			mufft_free_plan_1d(muplanInverse2048);
		if (muplanForward2048 != nullptr)
			mufft_free_plan_1d(muplanForward2048);
	};
	static atomic<VASTFFTSingletonFactory*> pinstance;
	static mutex m_;

	JUCE_LEAK_DETECTOR(VASTFFTSingletonFactory)
};
*/
