/*
VAST Dynamics Audio Software (TM)
*/

#include "VASTDelayLine.h"
#include "../../VASTSettings.h"
#include "../../VASTPluginConstants.h"

CDelayLine::CDelayLine(void)
{
	// --- zero everything
	//m_pBuffer = NULL;
	m_dDelay_ms = 0.0;
	m_dDelayInSamples = 0.0;
	m_nSampleRate = 0;
	m_pBuffer = std::make_unique<AudioSampleBuffer>(1, 44100 * 2); // inital value

	m_dDelay_ms_next = 0.0;
	m_nCrossFadeSteps = 0;

	// --- reset
	resetDelay();
}

CDelayLine::~CDelayLine(void)
{
}

JUCE_BEGIN_IGNORE_WARNINGS_GCC_LIKE("-Wconversion")
JUCE_BEGIN_IGNORE_WARNINGS_MSVC(4244 4267)

void CDelayLine::init(int nDelayLength)
{
	m_pBuffer->setSize(1, nDelayLength, false, false, true);
	m_pBuffer->clear();
}

void CDelayLine::resetDelay()
{
	m_pBuffer->clear();

	// --- init read/write indices
	m_nWriteIndex = 0; 
	m_nReadIndex = 0; 
	
	m_nCrossFadeSteps = 0;
	m_dDelay_ms_next = 0.0;

	// --- cook
	cookVariables();
}

void CDelayLine::setDelay_mSec(double dmSec)
{
	if (dmSec == m_dDelay_ms) return;

	//direct change	
	if (m_nCrossFadeSteps <= 0) { // wait for change
		if (abs(dmSec - m_dDelay_ms) < 15.0) { //small change?
			m_dDelayInSamples = m_dDelay_ms*((double)m_nSampleRate / 1000.0);
			// subtract to make read index
			m_nReadIndex = m_nWriteIndex - (int)m_dDelayInSamples;
			// --- the check and wrap BACKWARDS if the index is negative
			if (m_nReadIndex < 0)
				m_nReadIndex += m_pBuffer->getNumSamples();	// amount of wrap is Read + Length
			return;
		}
		m_dDelay_ms = dmSec;
		m_dDelay_ms_next = 0.0;
		cookVariables();
	}
	else { //intermediate change - buffer value
		m_dDelay_ms_next = dmSec;
	}
}

void CDelayLine::cookVariables()
{
	// --- calculate fractional delay

	if ((m_dDelayInSamples = 0.0) && (m_dDelay_ms != 0.0)) {
		m_dDelayInSamples = m_dDelay_ms*((double)m_nSampleRate / 1000.0);
		// subtract to make read index
		m_nReadIndex = m_nWriteIndex - (int)m_dDelayInSamples;
		// --- the check and wrap BACKWARDS if the index is negative
		if (m_nReadIndex < 0)
			m_nReadIndex += m_pBuffer->getNumSamples();	// amount of wrap is Read + Length
	}
	else
	if (m_dDelay_ms != 0.0)
		m_nCrossFadeSteps = C_MAX_CROSS_FADE_STEPS;	
}


// --- read delay at the prescribed delay value
double CDelayLine::readDelay()
{
	// --- Read the output of the delay at m_nReadIndex
	vassert(m_pBuffer != nullptr);
	if (m_pBuffer == nullptr) resetDelay();
	vassert((m_nReadIndex >= 0) && (m_nReadIndex <= m_pBuffer->getNumSamples()));
	double yn = m_pBuffer->getReadPointer(0)[m_nReadIndex];
	//vassert((yn > -10.0f) && (yn <= 10.0));
	if (!((yn > -10.0f) && (yn <= 10.0))) {
		VDBG("CDelayLine::readDelay() overflow > 10.f");
		yn = 0.0;
		resetDelay(); // try auto correct! BUT THIS IS NOT A SOLUTION!
	}

	// --- Read the location ONE BEHIND yn at y(n-1)
	int nReadIndex_1 = m_nReadIndex - 1;
	if(nReadIndex_1 < 0)
		nReadIndex_1 = m_pBuffer->getNumSamples() - 1; // m_nBufferSize-1 is last location

	// --- get y(n-1)
	vassert((nReadIndex_1 >= 0) && (nReadIndex_1 <= m_pBuffer->getNumSamples()));
	double yn_1 = m_pBuffer->getReadPointer(0)[nReadIndex_1];
	//vassert((yn_1 > -10.0f) && (yn_1 <= 10.0));
	if (!((yn_1 > -10.0f) && (yn_1 <= 10.0))) {
		VDBG("CDelayLine::readDelay() overflow > 10.f");
		yn_1 = 0.0;
		resetDelay(); // try auto correct! BUT THIS IS NOT A SOLUTION!
	}
	// --- get fractional component
	double dFracDelay = m_dDelayInSamples - (int)m_dDelayInSamples;
	
	// --- interpolate: (0, yn) and (1, yn_1) by the amount fracDelay	
	yn = dLinTerp(0, 1, yn, yn_1, dFracDelay); // interp frac between them

	//CHTS Cross fade ______________________________________________________
	// Read the output of the delay at m_nReadIndex
	if (m_nCrossFadeSteps > 0) {
		m_nCrossFadeSteps--;
		float ynTo = readDelayAt(m_dDelay_ms);

		// provide an unaltered output
		yn = ynTo  * (m_nCrossFadeSteps / C_MAX_CROSS_FADE_STEPS) + yn * ((C_MAX_CROSS_FADE_STEPS - m_nCrossFadeSteps) / C_MAX_CROSS_FADE_STEPS);
	}


	if (m_nCrossFadeSteps == 0) {
		m_nCrossFadeSteps = -1;
		m_dDelayInSamples = m_dDelay_ms*((double)m_nSampleRate / 1000.0);
		// subtract to make read index
		m_nReadIndex = m_nWriteIndex - (int)m_dDelayInSamples;
		// --- the check and wrap BACKWARDS if the index is negative
		if (m_nReadIndex < 0)
			m_nReadIndex += m_pBuffer->getNumSamples();	// amount of wrap is Read + Length

		if (m_dDelay_ms_next != 0.0)
			setDelay_mSec(m_dDelay_ms_next);
	}

	return yn;
}

// --- read delay from an arbitrary location given in mSec
double CDelayLine::readDelayAt(double dmSec)
{
	// --- local variales
	double dDelayInSamples = dmSec*((float)m_nSampleRate)/1000.0;

	// --- subtract to make read index
	int nReadIndex = m_nWriteIndex - (int)dDelayInSamples;

	// --- wrap if needed
	if (nReadIndex < 0)
		nReadIndex += m_pBuffer->getNumSamples();	// amount of wrap is Read + Length

	//---  Read the output of the delay at m_nReadIndex
	vassert(m_pBuffer != nullptr);
	if (m_pBuffer == nullptr) resetDelay();
	vassert((nReadIndex >= 0) && (nReadIndex <= m_pBuffer->getNumSamples()));
	double yn = m_pBuffer->getReadPointer(0)[nReadIndex];
	vassert((yn > -10.0f) && (yn <= 10.0));
	if (!((yn > -10.0f) && (yn <= 10.0))) {
		yn = 0.0;
		resetDelay(); // try auto correct! BUT THIS IS NOT A SOLUTION!
	}

	// --- Read the location ONE BEHIND yn at y(n-1)
	int nReadIndex_1 = nReadIndex - 1;
	if(nReadIndex_1 < 0)
		nReadIndex_1 = m_pBuffer->getNumSamples() - 1; // m_nBufferSize-1 is last location

	// -- get y(n-1)
	vassert((nReadIndex_1 >= 0) && (nReadIndex_1 <= m_pBuffer->getNumSamples()));
	double yn_1 = m_pBuffer->getReadPointer(0)[nReadIndex_1];
	vassert((yn_1 > -10.0f) && (yn_1 <= 10.0));
	if (!((yn_1 > -10.0f) && (yn_1 <= 10.0))) {
		yn_1 = 0.0;
		resetDelay(); // try auto correct! BUT THIS IS NOT A SOLUTION!
	}

	// --- get the fractional component
	double dFracDelay = dDelayInSamples - (int)dDelayInSamples;
	
	// --- interpolate: (0, yn) and (1, yn_1) by the amount fracDelay
	return dLinTerp(0, 1, yn, yn_1, dFracDelay); // interp frac between them

}

// --- this is used internally; normally you don't call it directly
//     but you can if you want to extend the modes of operation
void CDelayLine::writeDelayAndInc(double dDelayInput)
{
	// --- write to the delay line
	vassert((m_nWriteIndex >= 0) && (m_nWriteIndex <= m_pBuffer->getNumSamples())); //check out of bounds!
	m_pBuffer->getWritePointer(0)[m_nWriteIndex] = dDelayInput; // external feedback sample

	// --- increment the pointers and wrap if necessary
	m_nWriteIndex++;
	if(m_nWriteIndex >= m_pBuffer->getNumSamples())
		m_nWriteIndex = 0;

	m_nReadIndex++;
	if(m_nReadIndex >= m_pBuffer->getNumSamples())
		m_nReadIndex = 0;
}


// --- the normal way to use the object - this implements the delay
//     no feedback, no wet/dry - these must be done in owning object
bool CDelayLine::processAudio(double* pInput, double* pOutput)
{
	// read delayed output
	*pOutput = m_dDelayInSamples == 0 ? *pInput : readDelay();

	// write to the delay line
	writeDelayAndInc(*pInput);

	return true; // all OK
}

JUCE_END_IGNORE_WARNINGS_GCC_LIKE
JUCE_END_IGNORE_WARNINGS_MSVC