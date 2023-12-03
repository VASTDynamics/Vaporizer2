/*
VAST Dynamics Audio Software (TM)
*/

#include "VASTDDLModule.h"
#include "../../VASTSettings.h"

/* constructor()
	You can initialize variables here.
	You can also allocate memory here as long is it does not
	require the plugin to be fully instantiated. If so, allocate in init()

*/
CDDLModule::CDDLModule(CVASTSettings& set) : m_Set(&set) {
	// built in initialization
	m_fDelayInSamples = 0;
	m_fFeedback = 0;
	m_fDelay_ms = 0;
	m_fFeedback_pct = 0;
	m_fWetLevel = 0;
	
	m_pBuffer = std::make_unique<AudioSampleBuffer>(1, 16); // inital value

	// reset 
	m_nReadIndex = 0;
	m_nWriteIndex = 0;
	m_fDDLOutput = 0;

}

JUCE_BEGIN_IGNORE_WARNINGS_GCC_LIKE("-Wconversion")
JUCE_BEGIN_IGNORE_WARNINGS_MSVC(4244 4267)

void CDDLModule::init() {
	// cook
	cookVariables();
}

void CDDLModule::resetDelay(int nDelayLength)
{
	m_pBuffer->setSize(1, nDelayLength, true, true, true);
	m_pBuffer->clear();

	// init read/write indices
	m_nWriteIndex = 0; // reset the Write index to top
	m_nReadIndex = 0; // reset the Read index to top

	m_fDDLOutput = 0;
}

// set the feedback sample


// current FB is fb*output

float CDDLModule::getCurrentFeedbackOutput() { return m_fFeedback * m_pBuffer->getReadPointer(0)[m_nReadIndex]; }

void CDDLModule::setCurrentFeedbackInput(float f) { m_fFeedbackIn = f; }

// enable/disable external FB source

void CDDLModule::setUsesExternalFeedback(bool) {
	m_bUseExternalFeedback = false;
}

/* destructor()
	Destroy variables allocated in the contructor()

*/
CDDLModule::~CDDLModule(void)
{
}



void CDDLModule::cookVariables()
{
	m_fFeedback = m_fFeedback_pct / 100.0;
	m_fWetLevel = m_fWetLevel_pct / 100.0;
	m_fDelayInSamples = m_fDelay_ms * (m_iSampleRate / 1000.0);
	
	// subtract to make read index
	m_nReadIndex = m_nWriteIndex - (int)m_fDelayInSamples;

	//  the check and wrap BACKWARDS if the index is negative
	if (m_nReadIndex < 0) 	
		m_nReadIndex += m_pBuffer->getNumSamples();	// amount of wrap is Read + Length

}

/* prepareForPlay()
	Called by the client after Play() is initiated but before audio streams

	You can perform buffer flushes and per-run intializations.
	You can check the following variables and use them if needed:

	m_nNumWAVEChannels;
	m_nSampleRate;
	m_nBitDepth;

	NOTE: the above values are only valid during prepareForPlay() and
		  processAudioFrame() because the user might change to another wave file,
		  or use the sound card, oscillators, or impulse response mechanisms

    NOTE: if you allocte memory in this function, destroy it in ::destroy() above
*/
bool CDDLModule::prepareForPlay(int iSampleRate, bool bOversampling)
{
	// Add your code here:
	// reset first
    m_iSampleRate = iSampleRate; //includes oversampling factor
    m_bOversampling = bOversampling;
    resetDelay(2 * iSampleRate);	// 2 seconds delay @ fs;
 
	// cook
	cookVariables();
	return true;
}


/* processAudioFrame

// ALL VALUES IN AND OUT ON THE RANGE OF -1.0 TO + 1.0

LEFT INPUT = pInputBuffer[0];
RIGHT INPUT = pInputBuffer[1]

LEFT INPUT = pInputBuffer[0]
LEFT OUTPUT = pOutputBuffer[1]

*/
bool CDDLModule::processAudioFrame(float* pInputBuffer, float* pOutputBuffer, MYUINT uNumInputChannels, MYUINT uNumOutputChannels)
{
	// output = input -- change this for meaningful processing
	//
	// Do LEFT (MONO) Channel
	// Read the Input
	float xn = pInputBuffer[0];

	// Read the output of the delay at m_nReadIndex
	if ((m_nReadIndex >= m_pBuffer->getNumSamples()) || (m_nReadIndex <= 0))
		m_nReadIndex = 0;
	vassert(m_pBuffer != NULL);
	vassert((m_nReadIndex >= 0) && (m_nReadIndex <= m_pBuffer->getNumSamples()));
	float yn = m_pBuffer->getReadPointer(0)[m_nReadIndex];
	//vassert((yn > -10.0f) && (yn <= 10.0));
	if (!((yn > -10.0f) && (yn <= 10.0))) {
		yn = 0.0;
		prepareForPlay(m_iSampleRate, m_bOversampling); // try auto correct! BUT THIS IS NOT A SOLUTION!
	}

	// delay < 1 sample
	if (m_nReadIndex == m_nWriteIndex && m_fDelayInSamples < 1.00)
	{
		// yn = x(n), will interpolate with x(n-1) next
		yn = xn;
	}

	// Read the location ONE BEHIND yn at y(n-1)
	int nReadIndex_1 = m_nReadIndex - 1;
	if (nReadIndex_1 < 0)
		nReadIndex_1 = m_pBuffer->getNumSamples() - 1; // m_nBufferSize-1 is last location

	// get y(n-1)
	vassert((nReadIndex_1 >= 0) && (nReadIndex_1 <= m_pBuffer->getNumSamples()));
	float yn_1 = m_pBuffer->getReadPointer(0)[nReadIndex_1];
	vassert((yn_1 > -10.0f) && (yn_1 <= 10.0));
	if (!((yn_1 > -10.0f) && (yn_1 <= 10.0))) {
		yn = 0.0;
		prepareForPlay(m_iSampleRate, m_bOversampling); // try auto correct! BUT THIS IS NOT A SOLUTION!
	}

	// interpolate: (0, yn) and (1, yn_1) by the amount fracDelay
	float fFracDelay = m_fDelayInSamples - (int)m_fDelayInSamples;

	// linerp: x1, x2, y1, y2, x
	float fInterp = dLinTerp(0, 1, yn, yn_1, fFracDelay); // interp frac between them

	// use the interpolated value
	yn = fInterp;

	// provide an unaltered output
	m_fDDLOutput = yn;

	// write the intput to the delay
	/*
	if (!m_bUseExternalFeedback) {
		vassert((m_nWriteIndex >= 0) && (m_nWriteIndex <= m_nBufferSize)); //check out of bounds!
		m_pBuffer[m_nWriteIndex] = xn + m_fFeedback*yn; // normal
		}
	else {
		vassert((m_nWriteIndex >= 0) && (m_nWriteIndex <= m_nBufferSize)); //check out of bounds!
		m_pBuffer[m_nWriteIndex] = xn + m_fFeedbackIn; // external feedback sample
	}
	*/

	// write the intput to the delay
	vassert((m_nWriteIndex >= 0) && (m_nWriteIndex <= m_pBuffer->getNumSamples())); //check out of bounds!
	m_pBuffer->getWritePointer(0)[m_nWriteIndex] = xn + m_fFeedback*yn;

	// create the wet/dry mix and write to the output buffer
	// dry = 1 - wet
	//pOutputBuffer[0] = m_fWetLevel*yn + (1.0 - m_fWetLevel)*xn;
	pOutputBuffer[0] = m_fWetLevel*yn + xn;  //CHTS

	// incremnent the pointers and wrap if necessary
	m_nWriteIndex++;
	if (m_nWriteIndex >= m_pBuffer->getNumSamples())
		m_nWriteIndex = 0;

	m_nReadIndex++;
	if (m_nReadIndex >= m_pBuffer->getNumSamples())
		m_nReadIndex = 0;

	// Mono-In, Stereo-Out (AUX Effect)
	if (uNumInputChannels == 1 && uNumOutputChannels == 2)
		pOutputBuffer[1] = pOutputBuffer[0]; // copy MONO!

	// DDL Module is MONO - just do a copy here too
	// Stereo-In, Stereo-Out (INSERT Effect)
	if (uNumInputChannels == 2 && uNumOutputChannels == 2)
		pOutputBuffer[1] = pOutputBuffer[0];	// copy MONO!

	return true;
}

JUCE_END_IGNORE_WARNINGS_GCC_LIKE
JUCE_END_IGNORE_WARNINGS_MSVC
