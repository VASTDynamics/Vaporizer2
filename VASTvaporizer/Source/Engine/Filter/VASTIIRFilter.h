/*
VAST Dynamics Audio Software (TM)
*/

#pragma once

#include "../VASTEngineHeader.h"
#include "../VASTSettings.h"
#include <math.h>

//==============================================================================

class CVASTIIRFilter : public IIRFilter //seperate class due to missing nan check
{
public:
	void processSamples(float* const samples, const int numSamples) {
		const SpinLock::ScopedLockType sl(processLock);

		if (active)
		{
			auto c0 = coefficients.coefficients[0];
			auto c1 = coefficients.coefficients[1];
			auto c2 = coefficients.coefficients[2];
			auto c3 = coefficients.coefficients[3];
			auto c4 = coefficients.coefficients[4];
			auto lv1 = v1, lv2 = v2;

			for (int i = 0; i < numSamples; ++i)
			{
				auto in = samples[i];
				auto out = c0 * in + lv1;

				//CHVAST
				if (isnan(out)) { //NaN check
					out = 0.0f;
				}

				samples[i] = out;

				lv1 = c1 * in - c3 * out + lv2;
				lv2 = c2 * in - c4 * out;
			}

			JUCE_SNAP_TO_ZERO(lv1);  v1 = lv1;
			JUCE_SNAP_TO_ZERO(lv2);  v2 = lv2;
		}
	}
};