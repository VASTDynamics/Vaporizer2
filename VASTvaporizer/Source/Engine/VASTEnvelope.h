/*
VAST Dynamics Audio Software (TM)
*/

#pragma once

#include "VASTEngineHeader.h"
#include "VASTSettings.h"

#define M_MAX_SUSTAIN_TIME 1000  //not needed anymore!?

class CVASTEnvelope
{
public:
	CVASTEnvelope();
	virtual ~CVASTEnvelope(void);
	void init(CVASTSettings &set, MYUINT voiceNo, bool isVCA, bool isVCF);

	void updateVariables(float AttackTime, float DecayTime, float SustainLevel, float ReleaseTime, MYUINT egMode);
	bool isNoteOff();
	bool isHardStopNoteOff();

	float getEnvelope();
	float getValue();
	void noteOn();
	void noteOff();
	bool isActive();
	bool isHardStop();
	void reset();
	bool hardStop(); //returns success

	void calculateTimes();
	void setEGMode(MYUINT mode);

private: 
	CVASTSettings *m_Set;

	bool m_bAttack{};
	bool m_bDecay{};
	bool m_bSustain{};
	bool m_bRelease{};
	
	bool m_bIsNoteOff{};
	
	float m_fEnvelope{};
	bool m_bIsActive{};
	bool m_bHardStop{};
	bool m_bHardStopNoteOff{};

	float m_fAttackTime{};
	float m_fDecayTime{};
	float m_fSustainLevel{};
	float m_fReleaseTime{};

	float m_fSustainTime{};
	MYUINT m_voiceNo = 0;
	bool m_isVCA = false;
	bool m_isVCF = false;

	///new analog
	float m_fAttackCoeff = 0.0f;
	float m_fAttackTCO = 0.0f;
	float m_fAttackOffset = 0.0f;
	float m_fDecayCoeff = 0.0f;
	float m_fDecayTCO = 0.0f;
	float m_fDecayOffset = 0.0f;
	float m_fReleaseCoeff = 0.0f;
	float m_fReleaseTCO = 0.0f;
	float m_fReleaseOffset = 0.0f;

	MYUINT m_uEGMode = 0;

	JUCE_LEAK_DETECTOR(CVASTEnvelope)
};




