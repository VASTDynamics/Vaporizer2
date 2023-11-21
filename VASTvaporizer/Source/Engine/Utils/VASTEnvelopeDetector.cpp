/*
VAST Dynamics Audio Software (TM)
*/

#include "../VASTEngineHeader.h"
#include "../VASTPluginConstants.h"
#include "VASTEnvelopeDetector.h"

const float DIGITAL_TC = -2.0f; // log(1%)
const float ANALOG_TC = -0.43533393574791066201247090699309f; // (log(36.7%)

VASTEnvelopeDetector::VASTEnvelopeDetector(void)
{
	m_fAttackTime_mSec = 0.0f;
	m_fReleaseTime_mSec = 0.0f;
	m_fAttackTime = 0.0f;
	m_fReleaseTime = 0.0f;
	m_fSampleRate = 44100.f;
	m_fEnvelope = 0.0f;
	m_uDetectMode = 0;
	m_nSample = 0;
	m_bAnalogTC = false;
	m_bLogDetector = false;
}

VASTEnvelopeDetector::~VASTEnvelopeDetector(void)
{
}

void VASTEnvelopeDetector::prepareForPlay()
{
	m_fEnvelope = 0.0f;
	m_nSample = 0;
}

void VASTEnvelopeDetector::init(float samplerate, float attack_in_ms, float release_in_ms, bool bAnalogTC, MYUINT uDetect, bool bLogDetector)
{
	m_fEnvelope = 0.0f;
	m_fSampleRate = samplerate;
	m_bAnalogTC = bAnalogTC;
	m_fAttackTime_mSec = attack_in_ms;
	m_fReleaseTime_mSec = release_in_ms;
	m_uDetectMode = uDetect;
	m_bLogDetector = bLogDetector;

	// set themm_uDetectMode = uDetect;
	setAttackTime(attack_in_ms);
	setReleaseTime(release_in_ms);
}

void VASTEnvelopeDetector::setAttackTime(float attack_in_ms)
{
	m_fAttackTime_mSec = attack_in_ms;

	if(m_bAnalogTC)
		m_fAttackTime = exp(ANALOG_TC/( attack_in_ms * m_fSampleRate * 0.001f));
	else
		m_fAttackTime = exp(DIGITAL_TC/( attack_in_ms * m_fSampleRate * 0.001f));
}

void VASTEnvelopeDetector::setReleaseTime(float release_in_ms)
{
	m_fReleaseTime_mSec = release_in_ms;

	if(m_bAnalogTC)
		m_fReleaseTime = exp(ANALOG_TC/( release_in_ms * m_fSampleRate * 0.001f));
	else
		m_fReleaseTime = exp(DIGITAL_TC/( release_in_ms * m_fSampleRate * 0.001f));
}

// Use these "codes"
// DETECT PEAK   = 0
// DETECT MS	 = 1
// DETECT RMS	 = 2
//

inline void VASTEnvelopeDetector::setDetectMode(MYUINT uDetect) { m_uDetectMode = uDetect; }

inline void VASTEnvelopeDetector::setSampleRate(float f) { m_fSampleRate = f; }

inline void VASTEnvelopeDetector::setLogDetect(bool b) { m_bLogDetector = b; }

void VASTEnvelopeDetector::setTCModeAnalog(bool bAnalogTC)
{
	m_bAnalogTC = bAnalogTC;
	setAttackTime(m_fAttackTime_mSec);
	setReleaseTime(m_fReleaseTime_mSec);
}

float VASTEnvelopeDetector::detect(float fInput)
{
	switch(m_uDetectMode)
	{
	case 0:
		fInput = fabs(fInput);
		break;
	case 1:
		fInput = fabs(fInput) * fabs(fInput);
		break;
	case 2:
		fInput = pow((float)fabs(fInput) * (float)fabs(fInput), (float)0.5);
		break;
	default:
		fInput = (float)fabs(fInput);
		break;
	}

	//float fOld = m_fEnvelope;
	if(fInput> m_fEnvelope)
		m_fEnvelope = m_fAttackTime * (m_fEnvelope - fInput) + fInput;
	else
		m_fEnvelope = m_fReleaseTime * (m_fEnvelope - fInput) + fInput;

	if(m_fEnvelope > 0.0f && m_fEnvelope < FLT_MIN_PLUS) m_fEnvelope = 0.f;
	if(m_fEnvelope < 0.0f && m_fEnvelope > FLT_MIN_MINUS) m_fEnvelope = 0.f;

	// bound them; can happen when using pre-detector gains of more than 1.0
	m_fEnvelope = fmin(m_fEnvelope, 1.0f); //CHTS due to OSX 10.6
	m_fEnvelope = fmax(m_fEnvelope, 0.0f); //CHTS due to OSX 10.6

	//16-bit scaling!
	if(m_bLogDetector)
	{
		if(m_fEnvelope <= 0.f)
			return -96.0f; // 16 bit noise floor

		return 20.f*log10	(m_fEnvelope);
	}

	return m_fEnvelope;
}

