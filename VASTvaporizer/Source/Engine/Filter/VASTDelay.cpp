/*
VAST Dynamics Audio Software (TM)
*/

#include "VASTDelay.h"
#include "../VASTPluginConstants.h"
#include "../Utils/VASTSynthfunctions.h"

CDelay::CDelay(void)
{
	//m_pBuffer = NULL;

	m_fOutputAttenuation_dB = 0.f;
	m_fDelay_ms = 0.f;

	m_fOutputAttenuation = 0.f;
	m_fDelayInSamples = 0.f;
	m_nSampleRate = 0;

	m_pBuffer = std::make_unique<AudioSampleBuffer>(1, 44100 * 2); // inital value

	resetDelay();
}

CDelay::~CDelay(void)
{	
}

void CDelay::init(int nDelayLength)
{
	m_pBuffer->setSize(1, nDelayLength, false, false, true);
	m_pBuffer->clear();
}

void CDelay::resetDelay()
{
	m_pBuffer->clear();

	// init read/write indices
	m_nWriteIndex = 0; // reset the Write index to top
	m_nReadIndex = 0; // reset the Write index to top

	cookVariables();
}

void CDelay::setDelay_mSec(float fmSec)
{
	m_fDelay_ms = fmSec;
	cookVariables();
}

void CDelay::setOutputAttenuation_dB(float fAttendB)
{
	m_fOutputAttenuation_dB = fAttendB;
	cookVariables();
}

void CDelay::cookVariables()
{
	m_fOutputAttenuation = pow((float)10.0, (float)m_fOutputAttenuation_dB/(float)20.0);

	m_fDelayInSamples = m_fDelay_ms*((float)m_nSampleRate/1000.f);

	// subtract to make read index
	m_nReadIndex = m_nWriteIndex - (int)m_fDelayInSamples;

	//  the check and wrap BACKWARDS if the index is negative
	if (m_nReadIndex < 0)
		m_nReadIndex += m_pBuffer->getNumSamples();	// amount of wrap is Read + Length

}

void CDelay::writeDelayAndInc(float fDelayInput)
{
	// write to the delay line
	vassert((m_nWriteIndex >= 0) && (m_nWriteIndex <= m_pBuffer->getNumSamples())); //check out of bounds!
	m_pBuffer->getWritePointer(0)[m_nWriteIndex] = fDelayInput; // external feedback sample

	// incremnent the pointers and wrap if necessary
	m_nWriteIndex++;
	if(m_nWriteIndex >= m_pBuffer->getNumSamples())
		m_nWriteIndex = 0;

	m_nReadIndex++;
	if(m_nReadIndex >= m_pBuffer->getNumSamples())
		m_nReadIndex = 0;
}

float CDelay::readDelay()
{
	// Read the output of the delay at m_nReadIndex
	//if (m_pBuffer == nullptr) resetDelay();
	vassert((m_nReadIndex >= 0) && (m_nReadIndex <= m_pBuffer->getNumSamples()));
	float yn = m_pBuffer->getReadPointer(0)[m_nReadIndex];
	
	/*
	vassert((yn > -10.0f) && (yn<= 10.0));
	if (!((yn > -10.0f) && (yn <= 10.0))) {
		VDBG("CDelay::readDelay() overflow > 10.f");
		yn = 0.0;
		resetDelay(); // try auto correct! BUT THIS IS NOT A SOLUTION!
	}
	*/

	// Read the location ONE BEHIND yn at y(n-1)
	int nReadIndex_1 = m_nReadIndex - 1;
	if(nReadIndex_1 < 0)
		nReadIndex_1 = m_pBuffer->getNumSamples() - 1; // m_nBufferSize-1 is last location

	// get y(n-1)
	vassert((nReadIndex_1 >= 0) && (nReadIndex_1 <= m_pBuffer->getNumSamples()));
 	float yn_1 = m_pBuffer->getReadPointer(0)[nReadIndex_1];

	/*
	vassert((yn_1 > -10.0f) && (yn_1 <= 10.0));
	if (!((yn_1 > -10.0f) && (yn_1 <= 10.0))) {
		VDBG("CDelay::readDelay() overflow > 10.f");
		yn_1 = 0.0;
		resetDelay(); // try auto correct! BUT THIS IS NOT A SOLUTION!
	}
	*/

	// interpolate: (0, yn) and (1, yn_1) by the amount fracDelay
	float fFracDelay = m_fDelayInSamples - (int)m_fDelayInSamples;

	return dLinTerp(0, 1, yn, yn_1, fFracDelay); // interp frac between them
}

float CDelay::readDelayAt(float fmSec)
{
	float fDelayInSamples = fmSec*((float)m_nSampleRate)/1000.f;

	// subtract to make read index
	int nReadIndex = m_nWriteIndex - (int)fDelayInSamples;
	
	if (nReadIndex < 0)
		nReadIndex += m_pBuffer->getNumSamples();	// amount of wrap is Read + Length

	// Read the output of the delay at m_nReadIndex
	//if (m_pBuffer == nullptr) resetDelay();
	vassert((nReadIndex >= 0) && (nReadIndex <= m_pBuffer->getNumSamples()));
	float yn = m_pBuffer->getReadPointer(0)[nReadIndex];

	/*
	vassert((yn > -10.0f) && (yn <= 10.0));
	if (!((yn > -10.0f) && (yn <= 10.0))) {
		yn = 0.0;
		resetDelay(); // try auto correct! BUT THIS IS NOT A SOLUTION!
	}
	*/

	// Read the location ONE BEHIND yn at y(n-1)
	int nReadIndex_1 = nReadIndex - 1;
	if(nReadIndex_1 < 0)
		nReadIndex_1 = m_pBuffer->getNumSamples() - 1; // m_nBufferSize-1 is last location

	// get y(n-1)
	vassert((nReadIndex_1 >= 0) && (nReadIndex_1 <= m_pBuffer->getNumSamples()));
	float yn_1 = m_pBuffer->getReadPointer(0)[nReadIndex_1];

	/*
	vassert((yn_1 > -10.0f) && (yn_1 <= 10.0));
	if (!((yn_1 > -10.0f) && (yn_1 <= 10.0))) {
		yn = 0.0;
		resetDelay(); // try auto correct! BUT THIS IS NOT A SOLUTION!
	}
	*/

	// interpolate: (0, yn) and (1, yn_1) by the amount fracDelay
	float fFracDelay = fDelayInSamples - (int)fDelayInSamples;

	return dLinTerp(0.f, 1.f, yn, yn_1, fFracDelay); // interp frac between them
}

bool CDelay::processAudio(float* pInput, float* pOutput)
{
	// Read the Input
	float xn = *pInput;

	// read delayed output
	float yn = m_fDelayInSamples == 0 ? xn : readDelay();

	// write to the delay line
	writeDelayAndInc(xn);

	// output attenuation
	*pOutput = m_fOutputAttenuation*yn;

	return true; // all OK
}

