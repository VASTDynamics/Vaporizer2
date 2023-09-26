/*
VAST Dynamics Audio Software (TM)
*/

#include "VASTRateConvertor.h"
#ifdef DEBUG
	#define _CRTDBG_MAP_ALLOC
#endif

CRateConvertor::CRateConvertor(void)
{
	m_pIRBuffer = NULL;

	m_pLeftInputBuffer = NULL;
	m_pRightInputBuffer = NULL;

	m_nL = 1; 
	m_nIRLength = 0;
	m_nOSCounter = 0;
	
	// reset all indices
	m_nReadIndexDL = 0;
	m_nReadIndexH = 0;
	m_nWriteIndex = 0;
	m_nCurrentL = 0;
}


CRateConvertor::~CRateConvertor(void)
{
	// free up our input buffers
	if(m_pLeftInputBuffer) delete [] m_pLeftInputBuffer;
	if(m_pRightInputBuffer) delete [] m_pRightInputBuffer;
	if(m_pIRBuffer) delete [] m_pIRBuffer;
}


void CRateConvertor::init(int L, int FIRLen, float *pIRBuffer)
{
	m_nIRLength = FIRLen;
	m_nL = L;
	
	if(m_pIRBuffer)
		delete []  m_pIRBuffer;

	m_pIRBuffer = new float[m_nIRLength];

	// flush buffer
	memset(m_pIRBuffer, 0, m_nIRLength*sizeof(float));

	// load it
	for(int i = 0; i<m_nIRLength; i++)
	{
		m_pIRBuffer[i] = pIRBuffer[i];
	}

	// dynamically allocate the input x buffers and save the pointers
	if(m_pLeftInputBuffer)
		delete []  m_pLeftInputBuffer;

	if(m_pRightInputBuffer)
		delete []  m_pRightInputBuffer;
	
	m_pLeftInputBuffer = new float[m_nIRLength];
	m_pRightInputBuffer = new float[m_nIRLength];
	
	// flush x buffers
	memset(m_pLeftInputBuffer, 0, m_nIRLength*sizeof(float));
	memset(m_pRightInputBuffer, 0, m_nIRLength*sizeof(float));

	// reset all indices
	m_nReadIndexDL = 0;
	m_nReadIndexH = 0;
	m_nWriteIndex = 0;
	m_nCurrentL = 0;

}

void CRateConvertor::reset()
{	
	// flush x buffers
	if(m_pLeftInputBuffer)
		memset(m_pLeftInputBuffer, 0, m_nIRLength*sizeof(float));
	if(m_pRightInputBuffer)
		memset(m_pRightInputBuffer, 0, m_nIRLength*sizeof(float));
	
	// reset all indices
	m_nReadIndexDL = 0;
	m_nReadIndexH = 0;
	m_nWriteIndex = 0;
	m_nCurrentL = 0;
}

void CRateConvertor::interpolateNextOutputSample(float xnL, float xnR, float& fLeftOutput, float& fRightOutput)
{
}

bool CRateConvertor::decimateNextOutputSample(float xnL, float xnR, float& fLeftOutput, float& fRightOutput)
{
	return true;
}


void CRateConvertor::interpolateSamples(float xnL, float xnR, float* pLeftInterpBuffer, float* pRightInterpBuffer)
{
}

void CRateConvertor::decimateSamples(float* pLeftDeciBuffer, float* pRightDeciBuffer,float& ynL, float& ynR)
{
}
