/*
VAST Dynamics Audio Software (TM)
*/
#pragma once

#include "VASTFFTSingletonFactory.h"
//#include <mutex>
//#include <atomic>
#include "../../muFFT/fft.h"
#include "../../muFFT/fft_internal.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
using namespace std;

VASTFFTSingletonFactory::VASTFFTSingletonFactory() {
	int flags = 0;
	muplanInverse2048 = mufft_create_plan_1d_c2c(2048, MUFFT_INVERSE, flags);
	muplanForward2048 = mufft_create_plan_1d_c2c(2048, MUFFT_FORWARD, flags);
}

VASTFFTSingletonFactory::~VASTFFTSingletonFactory() {
	if (muplanInverse2048 != nullptr)
		mufft_free_plan_1d(muplanInverse2048);
	if (muplanForward2048 != nullptr)
		mufft_free_plan_1d(muplanForward2048);
};


/*

atomic<VASTFFTSingletonFactory*> VASTFFTSingletonFactory::pinstance{ nullptr };
std::mutex VASTFFTSingletonFactory::m_;

VASTFFTSingletonFactory* VASTFFTSingletonFactory::Instance() {
	if (pinstance == nullptr) {
		lock_guard<mutex> lock(m_);
		if (pinstance == nullptr) {
			pinstance = new VASTFFTSingletonFactory();

			int flags = 0;
			pinstance.load()->muplanInverse2048 = mufft_create_plan_1d_c2c(2048, MUFFT_INVERSE, flags);
			pinstance.load()->muplanForward2048 = mufft_create_plan_1d_c2c(2048, MUFFT_FORWARD, flags);
		}
	}
	return pinstance;
};
*/