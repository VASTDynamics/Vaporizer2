/*
VAST Dynamics Audio Software (TM)
*/


#include "VASTModDelayModule.h"
#ifdef DEBUG
	#define _CRTDBG_MAP_ALLOC
#endif

CModDelayModule::CModDelayModule()
{
	// Finish initializations here
	m_fMinDelay_mSec = 0.0f;
	m_fMaxDelay_mSec = 0.0f;
	m_fChorusOffset = 0.0f;
	m_DDL.m_bUseExternalFeedback = false;
	m_DDL.m_fDelay_ms = 0.f;
	m_fDDLOutput = 0.f;
	m_fModFrequency_Hz = 1.0f; //CHECK for start
}

void CModDelayModule::init(CVASTSettings &set) {
	m_Set = &set;
	m_DDL.init(set);
	m_LFO.init(*m_Set);	
	m_LFO.m_uPolarity = 0; //unipolar
	m_LFO.updateMainVariables(m_iSampleRate, WAVE::tri, 1, 1.0f, 0, 0.f); //TODO CHECK
	m_LFO.startLFOFrequency(m_fModFrequency_Hz, -1);
}

CModDelayModule::~CModDelayModule(void)
{
}

bool CModDelayModule::prepareForPlay(int sampleRate, bool bOversampling)
{
	m_iSampleRate = sampleRate;
	// Add your code here:
    m_bOversampling = bOversampling;
	m_DDL.prepareForPlay(m_iSampleRate, m_bOversampling);

	m_LFO.reset(0);		  // reset it

	// initialize
	cookModType();
	updateLFO();
	updateDDL();

	// start the LFO!
	m_LFO.m_bNoteOn = true;
	m_LFO.updateMainVariables(m_iSampleRate, WAVE::tri, 1, 1.0, 0, 0); //TODO CHECK
	m_LFO.setFrequency(0, true);

	m_fDDLOutput = 0;

	return true;
}

// LFO function to set:
//			- the LFO Frequency
//			- the oscillator type
void CModDelayModule::updateLFO()
{
	// set raw data
	//m_LFO.updateMainVariables(m_uLFOType, m_fModFrequency_Hz, 0, 0);
	m_LFO.startLFOFrequency(m_fModFrequency_Hz, -1);
}

// DDL function to set:
//			- the DDL Feedback amount (disabled for Vibrato)
void CModDelayModule::updateDDL()
{
	// test and set if needed
	if(m_uModType != Vibrato)
		m_DDL.m_fFeedback_pct = m_fFeedback_pct;

	m_DDL.m_fWetLevel = m_fWetLevel/100.0f;
	
	// cook it
	m_DDL.cookVariables();
}
// cookMod() function:
/*
		Min Delay (mSec)	Max delay (mSec)	Wet/Dry(%)	Feedback(%)
Flanger		0			7-10			50/50		-100 to +100
Vibrato		0			7-10			100/0		0
Chorus		5			20-40			50/50		-100 to +100

*/
void CModDelayModule::cookModType()
{
	switch(m_uModType)
	{
		case Flanger:
		{
			if(m_uTZF == ON)
				m_fMinDelay_mSec = 0.0f;
			else
				m_fMinDelay_mSec = 0.1f;

			m_fMaxDelay_mSec = 7.f;
			m_DDL.m_fWetLevel_pct = 50.0f;
			m_DDL.m_fFeedback_pct = m_fFeedback_pct;
			
			break;
		}

		case Vibrato:
		{
			m_fMinDelay_mSec = 0.0f;
			m_fMaxDelay_mSec = 7.f;
			m_DDL.m_fWetLevel_pct = 100.0f;
			m_DDL.m_fFeedback_pct = 0.0f; // NOTE! no FB for vibrato
			break;
		}

		case Chorus:
		{
			m_fMinDelay_mSec = 5.f;
			m_fMaxDelay_mSec = 30.f;
			m_DDL.m_fWetLevel_pct = 50.0f;
			m_DDL.m_fFeedback_pct = m_fFeedback_pct;
			break;
		}

		default: // is Flanger
		{
			if(m_uTZF == ON)
				m_fMinDelay_mSec = 0.0f;
			else
				m_fMinDelay_mSec = 0.1f;

			m_fMaxDelay_mSec = 7;
			m_DDL.m_fWetLevel_pct = 50.0f;
			m_DDL.m_fFeedback_pct = m_fFeedback_pct;
			break;
		}
	}
}
// calculateDelayOffset():
/*
	fLFOSample: a value from 0.0 to 1.0 from the LFO object

	returns: the calculated delay time in mSec for each effect

	NOTES: - the range for the flanger/vibrato is simply mapped from min to max 
		  starting at min: fLFOSample*(m_fMaxDelay_mSec - m_fMinDelay_mSec)) + 
  m_fMinDelay_mSec

	       - the range for the Chorus includes the starting offset
		  fStart = m_fMinDelay_mSec + m_fChorusOffset;
*/
float CModDelayModule::calculateDelayOffset(float fLFOSample)
{
	if(m_uModType == Flanger || m_uModType == Vibrato)
	{
		// flanger 0->1 gets mapped to 0->maxdelay
		//float f = (m_fModDepth_pct/100.0)*(fLFOSample*(m_fMaxDelay_mSec - m_fMinDelay_mSec)) + m_fMinDelay_mSec;
		//if(f < 0)
		//	this->sendStatusWndText("delay less than 0!!!");

		return (m_fModDepth_pct/100.0f)*(fLFOSample*(m_fMaxDelay_mSec - m_fMinDelay_mSec)) + m_fMinDelay_mSec;
	}
	else if(m_uModType == Chorus)
	{
		// chorus adds starting offset to move delay range
		float fStart = m_fMinDelay_mSec + m_fChorusOffset;

		return (m_fModDepth_pct/100.0f)*(fLFOSample*(m_fMaxDelay_mSec - m_fMinDelay_mSec)) + fStart;
	}

	return 0;
}

/* processAudioFrame

// ALL VALUES IN AND OUT ON THE RANGE OF -1.0 TO + 1.0

LEFT INPUT = pInputBuffer[0];
RIGHT INPUT = pInputBuffer[1]

LEFT INPUT = pInputBuffer[0]
LEFT OUTPUT = pOutputBuffer[1]

*/
bool CModDelayModule::processAudioFrame(float* pInputBuffer, float* pOutputBuffer, MYUINT uNumInputChannels, MYUINT uNumOutputChannels)
{
	// Do LEFT (MONO) Channel
	//
	// 1. Get LFO Values, normal and quad phase
	float fYn = 0;
	m_LFO.getOscillation(&fYn);

	// 2. calculate delay offset
	float fDelay = 0.0;
	fDelay = calculateDelayOffset(fYn); // normal LFO
	vassert(fDelay > 0.0f);

	// 3. set the delay & cook
	m_DDL.m_fDelay_ms = fDelay;
	m_DDL.cookVariables();
	
	// 4. get the delay output one channel in/one channel out
	//
	m_DDL.processAudioFrame(pInputBuffer, pOutputBuffer, 1, 1);
	
	// save for dimension chorus
	m_fDDLOutput = m_DDL.m_fDDLOutput;
	
	// Mono-In, Stereo-Out (AUX Effect)
	if(uNumInputChannels == 1 && uNumOutputChannels == 2)
		pOutputBuffer[1] = pOutputBuffer[0];

	// Stereo-In, Stereo-Out (INSERT Effect)
	if(uNumInputChannels == 2 && uNumOutputChannels == 2)
		pOutputBuffer[1] = pOutputBuffer[0];

	return true;
}

