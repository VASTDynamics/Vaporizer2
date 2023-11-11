/*
VAST Dynamics Audio Software (TM)
*/

#include "VASTEnvelope.h"
#include "VASTSettings.h"
#include "VASTEngineHeader.h"

CVASTEnvelope::CVASTEnvelope() {
}

/* destructor()
	Destroy variables allocated in the contructor()

*/
CVASTEnvelope::~CVASTEnvelope(void) {
}

void CVASTEnvelope::init(CVASTSettings &set, MYUINT voiceNo, bool vca, bool vcf) {
	m_Set = &set;
	m_voiceNo = voiceNo;
	m_isVCA = vca;
	m_isVCF = vcf;
	m_fEnvelope = 0;
	m_fSustainTime = 0;	
	setEGMode(0); //new analog mode
	reset();
}

void CVASTEnvelope::reset() {
	//if (m_isVCA)
	//	m_Set->sendStatusMessage("------------CVASTEnvelope VCA::reset voice# ", m_voiceNo);
	//else m_Set->sendStatusMessage("------------CVASTEnvelope VCF::reset voice# ", m_voiceNo);
	m_fEnvelope = 0.0;
	m_bIsActive = false;
	m_bAttack   = false;
	m_bDecay    = false;
	m_bSustain  = false;
	m_bRelease  = false;
	m_bHardStop = false;
	m_bHardStopNoteOff = false;
	m_bIsNoteOff = false;
}

void CVASTEnvelope::noteOn() {
	m_bIsNoteOff = false;
	m_fEnvelope = 0;
	m_bIsActive = true;
	m_bAttack   = true;
	m_bDecay    = false;
	m_bSustain  = false;
	m_bRelease  = false;
	m_bHardStop = false;
	m_bHardStopNoteOff = false;
}

void CVASTEnvelope::noteOff() {
	if (m_bHardStop) {
		//if (m_isVCA == true)
		//	m_Set->sendStatusMessage("------------CVASTEnvelope VCA::noteOff but hardStop voice# ", m_voiceNo);
		//else if (m_isVCF == true) m_Set->sendStatusMessage("------------CVASTEnvelope VCF::noteOff but hardStop voice# ", m_voiceNo);
		m_bHardStopNoteOff = true;
		m_bRelease = true; //???? check!!
		return;
	}

	//if (m_isVCA == true)
	//	m_Set->sendStatusMessage("------------CVASTEnvelope VCA::note is now noteOff voice# ", m_voiceNo);
	//else if (m_isVCF == true)
	//	m_Set->sendStatusMessage("------------CVASTEnvelope VCF::note is now noteOff voice# ", m_voiceNo);


	//m_bSustain = false;
	//if (m_bRelease) m_Set->sendStatusMessage("------------CVASTEnvelope ------> m_bRelease true");
	m_bRelease = true;

	//if (m_bAttack == true) 
		//m_Set->sendStatusMessage("------------CVASTEnvelope ------> m_bAttack true");
	m_bAttack = false;
	//if (m_bDecay == true) 
	//	m_Set->sendStatusMessage("------------CVASTEnvelope ------> m_bDecay true");
	m_bDecay = false;
	//if (m_bSustain == true) 
		//m_Set->sendStatusMessage("------------CVASTEnvelope ------> m_bSustain true");
	//if (m_bIsNoteOff == true) 
	//		m_Set->sendStatusMessage("------------CVASTEnvelope ------> noteOff true");
	m_bIsNoteOff = true;
}

void CVASTEnvelope::updateVariables(float AttackTime, float DecayTime, float SustainLevel, float ReleaseTime, MYUINT egMode) {
	m_fAttackTime = AttackTime;
	m_fDecayTime = DecayTime;
	m_fSustainLevel = SustainLevel / 100.f; //max 1.0 here
	m_fReleaseTime = ReleaseTime;
	setEGMode(egMode); //calls calculateTimes();
}

bool CVASTEnvelope::isActive() {
	return m_bIsActive;
}

bool CVASTEnvelope::isNoteOff() {
	if (!m_bHardStop)
		return m_bIsNoteOff;
	else
		return false; 
}

bool CVASTEnvelope::isHardStop() {
	return m_bHardStop;
}

bool CVASTEnvelope::isHardStopNoteOff() { //NoteOff during Hardstop
	return m_bHardStopNoteOff;
}

void CVASTEnvelope::calculateTimes() {
	float lMinAttack = 1.0f; //minimum attack time 1 ms
	//if (*m_Set->m_State->m_bOscRetrigOnOff_OscA == static_cast<int>(SWITCH::SWITCH_ON)) //TODO CHECK OSCA!!!
	lMinAttack = 3.0; //if random retrig attack time 3 ms
	
	float fSamples = m_Set->m_nSampleRate * (((m_fAttackTime + lMinAttack) / 1000.0f));
	m_fAttackCoeff = exp(-log((1.f + m_fAttackTCO) / m_fAttackTCO) / fSamples);
	m_fAttackOffset = (1.f + m_fAttackTCO) * (1.0f - m_fAttackCoeff);

	float lMinDecay = 10.0f; //minimum decay time 10 ms
	fSamples = m_Set->m_nSampleRate * (((m_fDecayTime + lMinDecay) / 1000.0f));
	m_fDecayCoeff = exp(-log((1.f + m_fDecayTCO) / m_fDecayTCO) / fSamples);
	m_fDecayOffset = (m_fSustainLevel - m_fDecayTCO) * (1.0f - m_fDecayCoeff);

	float lMinRelease = 10.0f; //minimum release time 10 ms
	if (m_isVCF)
		lMinRelease = 45.0f; //minimum filter release time 45ms
	fSamples = m_Set->m_nSampleRate * (((m_fReleaseTime + lMinRelease) / 1000.0f));
	m_fReleaseCoeff = exp(-log((1.f + m_fReleaseTCO) / m_fReleaseTCO) / fSamples);
	m_fReleaseOffset = -m_fReleaseTCO* (1.0f - m_fReleaseCoeff);
}

void CVASTEnvelope::setEGMode(MYUINT mode) {
	/*
	m_uEGMode = mode;
	if (mode == CVASTParamState::ANALOG) { //analog
		m_fAttackTCO  = exp(-1.5);
		m_fDecayTCO = exp(-4.95);
		m_fReleaseTCO = m_fDecayTCO;
	}
	else if (mode == CVASTParamState::DIGITAL) { //digital
		m_fAttackTCO = 0.99999;
		m_fDecayTCO = exp(-11.05);
		m_fReleaseTCO = m_fDecayTCO;
	}
	else { //linear
		m_fAttackTCO = 0.99999;
		m_fDecayTCO = exp(-0.05);
		m_fReleaseTCO = m_fDecayTCO;
	}
	calculateTimes();
	*/
}

bool CVASTEnvelope::hardStop() { 
	if (m_bHardStop) { 
		//cause too many messages
//		if (m_isVCA)
//			m_Set->sendStatusMessage("------------CVASTEnvelope VCA::getEnvelope IS ALREADY HARD STOP!! voice# ", m_voiceNo);
//		else m_Set->sendStatusMessage("------------CVASTEnvelope VCF::getEnvelope IS ALREADY HARD STOP!! voice# ", m_voiceNo);

		return false; // not possible
	}
	if (m_isVCA) //only for vca //CHECK
		m_bHardStop = true;	
	return true;
}

float CVASTEnvelope::getValue() { // just to get the value
	if ((isActive() == false) || (m_fEnvelope < 0.0f))
		return 0.0f;
	return m_fEnvelope;
}

float CVASTEnvelope::getEnvelope() { //called during processAudio
	if (!isActive()) return 0;

	if (m_bHardStop) { 
		if (m_fEnvelope > 0.00) {
			m_fEnvelope -= float(1.0 / (m_Set->m_nSampleRate * (10.0 / 1000.0))); // 10 ms
		}
		else {
			m_fEnvelope = 0.00f;
			//if (m_isVCA)
			//	m_Set->sendStatusMessage("------------CVASTEnvelope VCA::getEnvelope End of HardStop voice# ", m_voiceNo);
			//else m_Set->sendStatusMessage("------------CVASTEnvelope VCF::getEnvelope End of HardStop voice# ", m_voiceNo);
			reset();
		}
		return m_fEnvelope;
	}

	if (m_bAttack) {
		if (m_fEnvelope < 0.9999) {
			m_fEnvelope = m_fAttackOffset + m_fEnvelope * m_fAttackCoeff;
		}
		else
		{
			m_fEnvelope = 0.9999f;
			m_bAttack  = false;
			m_bDecay   = true; 
			//if (m_isVCA)
			//	m_Set->sendStatusMessage("------------CVASTEnvelope VCA::getEnvelope end of attack voice# ", m_voiceNo);
			//else m_Set->sendStatusMessage("------------CVASTEnvelope VCF::getEnvelope end of attack voice# ", m_voiceNo);
		}
		return m_fEnvelope;
	}

	if (m_bDecay) {
		if (m_fEnvelope > m_fSustainLevel) {
			m_fEnvelope = m_fDecayOffset + m_fEnvelope * m_fDecayCoeff;
		}
		else
		{
			m_fEnvelope = m_fSustainLevel;
			m_bAttack = false;
			m_bDecay = false;
			m_bSustain = true;
			m_fSustainTime = 0;
			//if (m_isVCA)
			//	m_Set->sendStatusMessage("------------CVASTEnvelope VCA::getEnvelope end of decay voice# ", m_voiceNo);
			//else m_Set->sendStatusMessage("------------CVASTEnvelope VCF::getEnvelope end of decay voice# ", m_voiceNo);
		}
		return m_fEnvelope;
	}

	if (m_bRelease) {
		if (m_fEnvelope > 0.0f) {
			m_fEnvelope = m_fReleaseOffset + m_fEnvelope * m_fReleaseCoeff;
		}
		else {
			m_fEnvelope = 0.0f;
			//if (m_isVCA)
			//	m_Set->sendStatusMessage("------------CVASTEnvelope VCA::getEnvelope end of release voice# ", m_voiceNo);
			//else m_Set->sendStatusMessage("------------CVASTEnvelope VCF::getEnvelope end of release voice# ", m_voiceNo);
			reset();
		}
		return m_fEnvelope;
	}

	return m_fEnvelope; //sustain
}