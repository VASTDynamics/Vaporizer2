/*
VAST Dynamics Audio Software (TM)
*/

#include "VASTStateVariableFilter.h"
#include "../Utils/VASTSynthfunctions.h"

//==============================================================================

CVAStateVariableFilter::CVAStateVariableFilter()
{
    sampleRate = 44100.0f;				// default sample rate when constructed
    filterType = SVFLowpass;			// lowpass filter by default

    gCoeff = 1.0f;
    RCoeff = 1.0f;
    KCoeff = 0.0f;

    cutoffFreq = 1000.0f;
    Q = static_cast<double>(resonanceToQ(0.5));

    z1_A[0] = z2_A[0] = 0.0f;
    z1_A[1] = z2_A[1] = 0.0f;

    //smoothTimeMs = 0.0;		        // 0.0 milliseconds
}

CVAStateVariableFilter::~CVAStateVariableFilter()
{
}

// Member functions for setting the filter's parameters (and sample rate).
//==============================================================================
void CVAStateVariableFilter::setFilterType(const int& newType)
{
    filterType = newType;
}

void CVAStateVariableFilter::setCutoffPitch(const double& newCutoffPitch)
{
    if (active) {
        cutoffFreq = static_cast<double>(pitchToFreq(newCutoffPitch));
        //cutoffLinSmooth.setValue(cutoffFreq);
        calcFilter();
    }
}

void CVAStateVariableFilter::setCutoffFreq(const double& newCutoffFreq)
{
    if (active) {
        cutoffFreq = newCutoffFreq;
        calcFilter();
    }
}

void CVAStateVariableFilter::setResonance(const double& newResonance)
{
    if (active) {
        Q = static_cast<double>(resonanceToQ(newResonance));
        calcFilter();
    }
}

void CVAStateVariableFilter::setQ(const double& newQ)
{
    if (active) {
        Q = newQ;
        calcFilter();
    }
}

void CVAStateVariableFilter::setShelfGain(const double& newGain)
{
    if (active) {
        shelfGain = newGain;
        calcFilter();
    }
}

void CVAStateVariableFilter::setFilter(const int& newType, const double& newCutoffFreq,
                                      const double& newResonance, const double& newShelfGain)
{
    filterType = newType;
    cutoffFreq = newCutoffFreq;
    Q = static_cast<double>(resonanceToQ(newResonance));
    shelfGain = newShelfGain;
    calcFilter();
}

void CVAStateVariableFilter::setSampleRate(const double& newSampleRate)
{
    sampleRate = newSampleRate;
    //cutoffLinSmooth.reset(sampleRate, smoothTimeMs);
    calcFilter();
}

/*void VAStateVariableFilter::setSmoothingTimeInMs(const double & newSmoothingTimeMs)
{
    smoothTimeMs = newSmoothingTimeMs;
}*/

void CVAStateVariableFilter::setIsActive(bool isActive)
{
    active = isActive;
}

//==============================================================================
void CVAStateVariableFilter::calcFilter()
{
    if (active) {

        // prewarp the cutoff (for bilinear-transform filters)
        double wd = static_cast<double>(cutoffFreq * 2.0f * M_PI);
        double T = 1.0f / (double)sampleRate;
        double wa = (2.0f / T) * tan(wd * T / 2.0f);

        // Calculate g (gain element of integrator)
        gCoeff = wa * T / 2.0f;			// Calculate g (gain element of integrator)

        // Calculate Zavalishin's R from Q (referred to as damping parameter)
        RCoeff = 1.0f / (2.0f * Q);		
        
        // Gain for BandShelving filter
        KCoeff = shelfGain;				
    }
}

double CVAStateVariableFilter::processAudioSample(const double& input, const int& channelIndex)
{
    if (active) {

        // Do the cutoff parameter smoothing per sample.
        //cutoffFreq = cutoffLinSmooth.getNextValue();
        //calcFilter();

        // Filter processing:
        const double HP = (input - (2.0f * RCoeff + gCoeff) * z1_A[channelIndex] - z2_A[channelIndex])
            / (1.0f + (2.0f * RCoeff * gCoeff) + gCoeff * gCoeff);

        const double BP = HP * gCoeff + z1_A[channelIndex];

        const double LP = BP * gCoeff + z2_A[channelIndex];

        const double UBP = 2.0f * RCoeff * BP;

        const double BShelf = input + UBP * KCoeff;

        const double Notch = input - UBP;

        const double AP = input - (4.0f * RCoeff * BP);

        const double Peak = LP - HP;

        z1_A[channelIndex] = gCoeff * HP + BP;		// unit delay (state variable)
        z2_A[channelIndex] = gCoeff * BP + LP;		// unit delay (state variable)

        // Selects which filter type this function will output.
        switch (filterType) {
        case SVFLowpass:
            return LP;
            break;
        case SVFBandpass:
            return BP;
            break;
        case SVFHighpass:
            return HP;
            break;
        case SVFUnitGainBandpass:
            return UBP;
            break;
        case SVFBandShelving:
            return BShelf;
            break;
        case SVFNotch:
            return Notch;
            break;
        case SVFAllpass:
            return AP;
            break;
        case SVFPeak:
            return Peak;
            break;
        default:
            return 0.0f;
            break;
        }
    }
    else {	// If not active, return input
        return input;
    }
}

void CVAStateVariableFilter::processAudioBlock(double* const samples,  const int& numSamples,
                                              const int& channelIndex)
{
    // Test if filter is active. If not, bypass it
    if (active) {

        // Loop through the sample block and process it
        for (int i = 0; i < numSamples; ++i) {
            
            // Do the cutoff parameter smoothing per sample.
            //cutoffFreq = cutoffLinSmooth.getNextValue();
            //calcFilter();       // calculate the coefficients for the smoother

            // Filter processing:
            const double input = samples[i];

            const double HP = (input - (2.0f * RCoeff + gCoeff) * z1_A[channelIndex] - z2_A[channelIndex])
                       / (1.0f + (2.0f * RCoeff * gCoeff) + gCoeff * gCoeff);
            
            const double BP = HP * gCoeff + z1_A[channelIndex];
            
            const double LP = BP * gCoeff + z2_A[channelIndex];

            const double UBP = 2.0f * RCoeff * BP;

            const double BShelf = input + UBP * KCoeff;

            const double Notch = input - UBP;

            const double AP = input - (4.0f * RCoeff * BP);

            const double Peak = LP - HP;

            z1_A[channelIndex] = gCoeff * HP + BP;		// unit delay (state variable)
            z2_A[channelIndex] = gCoeff * BP + LP;		// unit delay (state variable)

            // Selects which filter type this function will output.
            switch (filterType) {
            case SVFLowpass:
                samples[i] = LP;
                break;
            case SVFBandpass:
                samples[i] = BP;
                break;
            case SVFHighpass:
                samples[i] = HP;
                break;
            case SVFUnitGainBandpass:
                samples[i] = UBP;
                break;
            case SVFBandShelving:
                samples[i] = BShelf;
                break;
            case SVFNotch:
                samples[i] = Notch;
                break;
            case SVFAllpass:
                samples[i] = AP;
                break;
            case SVFPeak:
                samples[i] = Peak;
                break;
            default:
                samples[i] = 0.0f;
                break;
            }
        }
    }
}

//==============================================================================
