/*
VAST Dynamics Audio Software (TM)
*/

#include "VASTInterpolator.h"
#ifdef DEBUG
	#define _CRTDBG_MAP_ALLOC
#endif

CInterpolator::CInterpolator(void)
{
	
}

CInterpolator::~CInterpolator(void)
{

}

/* L-Point Interpolator
	Input:	Left and Right Input Samples xnL, xnR
	Output: Left and Right Output Buffers with L samples per buffer

	This function loops L times, interpolating the outputs and placing them into the output buffers

	CALLER SUPPLIES OUTPUT BUFFERS!
*/
void CInterpolator::interpolateSamples(float xnL, float xnR, float* pLeftOutputBuffer, float* pRightOutputBuffer)
{
	if(!pLeftOutputBuffer || !pRightOutputBuffer)
		return;

	for(int i=0; i<m_nL; i++)
	{
		// interp next sample
		this->interpolateNextOutputSample(xnL, xnR, pLeftOutputBuffer[i], pRightOutputBuffer[i]);
	}
}

/* interpolateNextOutputSample
	This does the work: 
		- first time through the loop apply inputs xnL and xnR and convolve
		- other L-1 times, insert a zero into the delay and convolve
*/
// sepcial kind of convolution! 
void CInterpolator::interpolateNextOutputSample(float xnL, float xnR, float& fLeftOutput, float& fRightOutput)
{
	// Read the Input
	// if current L == 0 read xn
	// else insert 0.0
	m_pLeftInputBuffer[m_nWriteIndex] = m_nCurrentL == 0.f ? xnL : 0.f; 
	m_pRightInputBuffer[m_nWriteIndex] = m_nCurrentL == 0.f ? xnR : 0.f; 

	// reset: read index for Delay Line -> write index
	m_nReadIndexDL = m_nWriteIndex;

	// reset: read index for IR - > top (0)
	m_nReadIndexH = 0;

	// accumulator
	float yn_accumL = 0;	
	float yn_accumR = 0;	

	// convolve: OPTIMIZE THIS; can skip over delay elements with 0s in them, there are L-1 of them
	for(int i=0; i<m_nIRLength; i++)
	{
		// do the sum of products
		yn_accumL += m_pLeftInputBuffer[m_nReadIndexDL]*m_pIRBuffer[m_nReadIndexH];
		yn_accumR += m_pRightInputBuffer[m_nReadIndexDL]*m_pIRBuffer[m_nReadIndexH];

		// advance the IR index
		m_nReadIndexH++;

		// decrement the Delay Line index
		m_nReadIndexDL--;

		// check for wrap of delay line (no need to check IR buffer)
		if(m_nReadIndexDL < 0)
			m_nReadIndexDL = m_nIRLength -1;
	}

	// out
	fLeftOutput = yn_accumL*(float)m_nL;

	// write out
	fRightOutput = yn_accumR*(float)m_nL; 

	// incremnent the pointers and wrap if necessary
	m_nWriteIndex++;
	if(m_nWriteIndex >= m_nIRLength)
		m_nWriteIndex = 0;

	m_nCurrentL++;
	if(m_nCurrentL > m_nL-1)
		m_nCurrentL = 0;
}

