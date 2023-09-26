/*
  ==============================================================================

  VAST Dynamics Audio Software (TM)

    Notes:
    From The Art of VA Filter Design, by Vadim Zavalishin

    TPT State Variable Filter:

    TPT -> Topology Preserving Transform

    This filter is based off of the trapezoidal integrator, which produces
    a bilinear transformation. This filter does the proper cutoff prewarping
    needed for the TPT structure, as explained by Zavalishin. It also handles
    the problem of delayless feedback loops that the trapezoidal integrators
    introduce.

    The SVF has two feedback paths sharing a common path segment. In order to
    obtain a single feedback equation (rather than equation system), a signal
    on the common path should be selected as the unknown variable. The HP filter 
    path is chosen as the zero-delay feedback equation's unknown in this filter.
    Using the HP filter path, the rest of the filter's signals in the structure
    can be defined.

  ==============================================================================
*/

#ifndef VASTSTATEVARIABLEFILTER_H_INCLUDED
#define VASTSTATEVARIABLEFILTER_H_INCLUDED

//#define _USE_MATH_DEFINES		// to use M_PI

//==============================================================================

//#include <cmath>
#include "../VASTEngineHeader.h"

//==============================================================================

using std::pow;
using std::tan;
using std::sqrt;

//==============================================================================

/** The type of filter that the State Variable Filter will output. */
enum SVFType {
    SVFLowpass = 0,
    SVFBandpass,
    SVFHighpass,
    SVFUnitGainBandpass,
    SVFBandShelving,
    SVFNotch,
    SVFAllpass,
    SVFPeak
};

//==============================================================================
class CVAStateVariableFilter {
public:
    /** Create and initialize the filter with default values defined in constructor. */
    CVAStateVariableFilter();

    //------------------------------------------------------------------------------

    ~CVAStateVariableFilter();
    
    //------------------------------------------------------------------------------
    
    /**	Sets the type of the filter that processAudioSample() or processAudioBlock() will 
        output. This filter can choose between 8 different types using the enums listed 
        below or the int given to each.
        0: SVFLowpass
        1: SVFBandpass
        2: SVFHighpass
        3: SVFUnitGainBandpass
        4: SVFBandShelving
        5: SVFNotch
        6: SVFAllpass
        7: SVFPeak
    */
    void setFilterType(const int& newType);

    //------------------------------------------------------------------------------
    /**	Used for changing the filter's cutoff parameter logarithmically by 
        pitch (MIDI note #)
    */
    void setCutoffPitch(const double& newCutoff);

    //------------------------------------------------------------------------------
    /**	Used for changing the filter's cutoff parameter linearly by frequency (Hz) */
    void setCutoffFreq(const double& newCutoff);

    //------------------------------------------------------------------------------
    /** Used for setting the resonance amount. This is then converted to a Q
        value, which is used by the filter.
        Range: (0-1)
    */
    void setResonance(const double& newResonance);

    //------------------------------------------------------------------------------
    /** Used for setting the filter's Q amount. This is then converted to a
        damping parameter called R, which is used in the original filter.
    */
    void setQ(const double& newQ);

    //------------------------------------------------------------------------------
    /**	Sets the gain of the shelf for the BandShelving filter only. */
    void setShelfGain(const double& newGain);

    //------------------------------------------------------------------------------
    /**	Statically set the filters parameters. */
    void setFilter(const int& newType, const double& newCutoff, 
                   const double& newResonance, const double& newShelfGain);

    //------------------------------------------------------------------------------	
    /**	Set the sample rate used by the host. Needs to be used to accurately
        calculate the coefficients of the filter from the cutoff.
        Note: This is often used in AudioProcessor::prepareToPlay
    */
    void setSampleRate(const double& newSampleRate);

    //------------------------------------------------------------------------------
    /**	Sets the time that it takes to interpolate between the previous value and
        the current value. For this filter, the smoothing is only happening for
        the filters cutoff frequency.
    */
    //void setSmoothingTimeInMs(const double& newSmoothingTimeMs);

    //------------------------------------------------------------------------------
    /** Sets whether the filter will process data or not.
        - If (isActive = true) then the filter will process data
        - If (isActive = false) then the filter will be bypassed
    */
    void setIsActive(bool isActive);

    //------------------------------------------------------------------------------
    /**	Performs the actual processing for one sample of data, on 2 channels.
        If 2 channels are needed (stereo), use channel index (channelIdx) to 
        specify which channel is being processed (i.e. 0 for left, 1 for right).
    */
    double processAudioSample(const double& input, const int& channelIndex);

    //------------------------------------------------------------------------------	
    /**	Performs the actual processing for a block of samples, on 2 channels.
        If 2 channels are needed (stereo), use channel index (channelIdx) to 
        specify which channel is being processed (i.e. 0 for left, 1 for right).
        Note:
        This processes the information sent to the samples argument and 
        does it through a pointer. Therefore, no value needs to be
        returned.
    */
    void processAudioBlock(double* const samples, const int& numSamples,
                           const int& channelIndex);

    //------------------------------------------------------------------------------

private:
    //==============================================================================
    //	Calculate the coefficients for the filter based on parameters.
    void calcFilter();

    //	Parameters:
    int filterType;
    double cutoffFreq;
    double Q;
    double shelfGain;

    double sampleRate;
    bool active = true;	// is the filter processing or not

    //	Coefficients:
    double gCoeff;		// gain element 
    double RCoeff;		// feedback damping element
    double KCoeff;		// shelf gain element

    double z1_A[2], z2_A[2];		// state variables (z^-1)

    // Parameter smoothers:
    //LinearSmoothedValue cutoffLinSmooth;
    //double smoothTimeMs;


	//==============================================================================
	/*
	Created: 1 May 2015 12 : 43 : 46am
	Author : Jordan Harris

	Useful DSP utilities. For instance, calculating frequency from a given
	pitch (MIDI) value, or vice versa.
	*/

	//==============================================================================

	// Calculates the frequency of a given pitch (MIDI) value.
	double pitchToFreq(double pitch)
	{
	return pow(2, (pitch - 69) / 12) * 440;
	}

	//==============================================================================
	// Calculates the pitch (MIDI) of a given frequency value
	double freqToPitch(double freq)
	{
	return 69 + 12 * log2(freq / 440);
	}

	//==============================================================================


	//Takes a value as input and clips it according to the min and max values.
	//Returns the input if (minValue <= in <= maxValue).
	//If (in < minValue), then return minValue.
	//If (in > maxValue), then return maxValue.
	double clipMinMax(double in, double minValue, double maxValue)
	{
	if (in < minValue)
	return minValue;
	else if (in > maxValue)
	return maxValue;
	else
	return in;
	}

	//==============================================================================


	//Takes a value as input and clips it according to the min value.
	//Returns the input if (minValue <= in).
	//If (in < minValue), then return minValue.
	double clipMin(double in, double minValue)
	{
	if (in < minValue)
	return minValue;
	else
	return in;
	}

	//==============================================================================

	//Crossfades linearly between two values (in0, in1). The value returned is
	//determined by the value of the xFadeCtrl argument.
	//xFadeCtrl Range: 0->1
	//- xFadeCtrl = 0    (only in0 comes through)
	//- xFadeCtrl = 0.5  (equal mix of in0 and in1)
	//- xfadeCtrl = 1    (only in1 comes through)
	double xFadeLin(double xFadeCtrl, double in0, double in1)
	{
	// Clip the xFade parameter to only have range of 0->1
	xFadeCtrl = clipMinMax(xFadeCtrl, 0.0, 1.0);
	// Perform crossfading and return the value
	return (in0 * (1.0 - xFadeCtrl) + in1 * xFadeCtrl);
	}

	//==============================================================================

	//Parabolic Controller Shaper:
	//"Bends" the controller curve torwards the X or Y axis.
	//input range: (-1..0..1) maps to output range: (-1..0..1).
	//bend range: (-1..0..1)
	//- bend = -1 (max bend towards X axis)
	//- bend = 0 (don't bend)
	//- bend = 1 (max bend towards Y axis)
	double parCtrlShaper(double input, double bend)
	{
	// clip input and bend because the shaper only works in that range.
	input = clipMinMax(input, -1.0, 1.0);
	bend = clipMinMax(bend, -1.0, 1.0);
	return input * ((bend + 1) - abs(input) * bend);
	}

	//==============================================================================

	//Normalizes a range of values to the range 0->1.
	//(start/end should probably be the range of a parameter)
	//- input: the value to be normalized
	//- start: the start of the input's range
	//- end: the end of the input's range
	//Note: (start < end) and (start > end) are both valid.
	double normalizeRange(double input, double start, double end)
	{
	return (input - start) / (end - start);
	}

	double resonanceToQ(double resonance)
	{
	return 1.0 / (2.0 * (1.0 - resonance));
	}
	
};

//==============================================================================
#endif  // VASTSTATEVARIABLEFILTER_H_INCLUDED
