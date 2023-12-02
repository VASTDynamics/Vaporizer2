/*
VAST Dynamics Audio Software (TM)
*/

#include "VASTDelaySoftAdjust.h"
#include "../../VASTEngineHeader.h"
#include "../../VASTSettings.h"
#include "../../VASTPluginConstants.h"

CVASTDelay::CVASTDelay(void)
{
	//m_pBuffer = NULL;

	m_fOutputAttenuation_dB = 0;
	m_fDelay_ms = 0.0;

	m_fOutputAttenuation = 0.0;
	m_fDelayInSamples = 0.0;
	m_nSampleRate = 0;

	m_pBuffer = std::make_unique<AudioSampleBuffer>(1, 44100 * 2); // inital value

	resetDelay();
}

CVASTDelay::~CVASTDelay(void)
{
}

void CVASTDelay::init(int nDelayLength)
{
	m_pBuffer->setSize(1, nDelayLength, false, false, true);
	m_pBuffer->clear();
}

void CVASTDelay::resetDelay()
{
	// flush buffer
	m_pBuffer->clear();

	// init read/write indices
	m_nWriteIndex = 0; // reset the Write index to top
	m_nReadIndex = 0; // reset the Write index to top

	cookVariables();
}

void CVASTDelay::setDelay_mSec(float fmSec)
{
	m_fDelay_ms = fmSec;
	cookVariables();
}

void CVASTDelay::setOutputAttenuation_dB(float fAttendB)
{
	m_fOutputAttenuation_dB = fAttendB;
	cookVariables();
}

void CVASTDelay::softAdjustVariables(){
	if (m_fDelayInSamples < m_fDelay_ms*((float)m_nSampleRate / 1000.0)) m_fDelayInSamples++; 
	else
	if (m_fDelayInSamples > m_fDelay_ms*((float)m_nSampleRate / 1000.0)) m_fDelayInSamples--;
	else return;

	// subtract to make read index
	m_nReadIndex = m_nWriteIndex - (int)m_fDelayInSamples;

	//  the check and wrap BACKWARDS if the index is negative
	if (m_nReadIndex < 0)
		m_nReadIndex += m_pBuffer->getNumSamples();	// amount of wrap is Read + Length

}

void CVASTDelay::cookVariables()
{
	m_fOutputAttenuation = pow((float)10.0, (float)m_fOutputAttenuation_dB/(float)20.0);

	if (m_fDelayInSamples == 0) {
		m_fDelayInSamples = m_fDelay_ms*((float)m_nSampleRate / 1000.0f);
	}

	/*
	//CHTS
	//DONE SOFT

	// subtract to make read index
	m_nReadIndex = m_nWriteIndex - (int)m_fDelayInSamples;

	//  the check and wrap BACKWARDS if the index is negative
	if (m_nReadIndex < 0)
		m_nReadIndex += m_pBuffer->getNumSamples();	// amount of wrap is Read + Length
		*/
}

void CVASTDelay::writeDelayAndInc(float fDelayInput)
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

float CVASTDelay::readDelay()
{
	softAdjustVariables();

	// Read the output of the delay at m_nReadIndex
	vassert(m_pBuffer != nullptr);
	if (m_pBuffer == nullptr) resetDelay();
	vassert((m_nReadIndex >= 0) && (m_nReadIndex <= m_pBuffer->getNumSamples()));
	float yn = m_pBuffer->getReadPointer(0)[m_nReadIndex];
	//vassert((yn > -10.0f) && (yn <= 10.0));
	if (!((yn > -10.0f) && (yn <= 10.0))) {
		yn = 0.0;
		resetDelay(); // try auto correct! BUT THIS IS NOT A SOLUTION!
	}

	// Read the location ONE BEHIND yn at y(n-1)
	int nReadIndex_1 = m_nReadIndex - 1;
	if(nReadIndex_1 < 0)
		nReadIndex_1 = m_pBuffer->getNumSamples()-1; // m_pBuffer->getNumSamples()-1 is last location

	// get y(n-1)
	vassert((nReadIndex_1 >= 0) && (nReadIndex_1 <= m_pBuffer->getNumSamples()));
	float yn_1 = m_pBuffer->getReadPointer(0)[nReadIndex_1];
	//vassert((yn_1 > -10.0f) && (yn_1 <= 10.0));
	if (!((yn_1 > -10.0f) && (yn_1 <= 10.0))) {
		yn_1 = 0.0;
		resetDelay(); // try auto correct! BUT THIS IS NOT A SOLUTION!
	}
	// interpolate: (0, yn) and (1, yn_1) by the amount fraCVASTDelay
	float fFraCVASTDelay = m_fDelayInSamples - (int)m_fDelayInSamples;

	return dLinTerp(0, 1, yn, yn_1, fFraCVASTDelay); // interp frac between them
}

float CVASTDelay::readDelayAt(float fmSec)
{
	softAdjustVariables();

	float fDelayInSamples = fmSec*((float)m_nSampleRate)/1000.0f;

	// subtract to make read index
	int nReadIndex = m_nWriteIndex - (int)fDelayInSamples;
	
	if (nReadIndex < 0)
		nReadIndex += m_pBuffer->getNumSamples();	// amount of wrap is Read + Length

	// Read the output of the delay at m_nReadIndex
	vassert(m_pBuffer != nullptr);
	if (m_pBuffer == nullptr) resetDelay();
	vassert((nReadIndex >= 0) && (nReadIndex <= m_pBuffer->getNumSamples()));
	float yn = m_pBuffer->getReadPointer(0)[nReadIndex];
	//vassert((yn > -10.0f) && (yn <= 10.0));
	if (!((yn > -10.0f) && (yn <= 10.0))) {
		yn = 0.0;
		resetDelay(); // try auto correct! BUT THIS IS NOT A SOLUTION!
	}

	// Read the location ONE BEHIND yn at y(n-1)
	int nReadIndex_1 = nReadIndex - 1;
	if(nReadIndex_1 < 0)
		nReadIndex_1 = m_pBuffer->getNumSamples()-1; // m_pBuffer->getNumSamples()-1 is last location

	// get y(n-1)
	vassert((nReadIndex_1 >= 0) && (nReadIndex_1 <= m_pBuffer->getNumSamples()));
	float yn_1 = m_pBuffer->getReadPointer(0)[nReadIndex_1];
	//vassert((yn_1 > -10.0f) && (yn_1 <= 10.0));
	if (!((yn_1 > -10.0f) && (yn_1 <= 10.0))) {
		yn = 0.0;
		resetDelay(); // try auto correct! BUT THIS IS NOT A SOLUTION!
	}

	// interpolate: (0, yn) and (1, yn_1) by the amount fraCVASTDelay
	float fFraCVASTDelay = fDelayInSamples - (int)fDelayInSamples;

	return dLinTerp(0, 1, yn, yn_1, fFraCVASTDelay); // interp frac between them
}

bool CVASTDelay::processAudio(float* pInput, float* pOutput)
{
	softAdjustVariables();
	
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
