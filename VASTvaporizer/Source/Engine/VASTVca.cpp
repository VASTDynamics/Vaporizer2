/*
VAST Dynamics Audio Software (TM)

Controls VCA and Mod Envelope
*/

#include "VASTVca.h"
#include "VASTEngineHeader.h"
#include "VASTSettings.h"
#include "VASTPluginConstants.h"
//#include "VASTMSEGEnvelope.h"

CVASTVca::CVASTVca(){
}

/* destructor()
	Destroy variables allocated in the contructor()

*/
CVASTVca::~CVASTVca(void) {
	m_bNoteOff.store(true);
}

void CVASTVca::init(CVASTSettings &set, MYUINT voiceNo) {
	m_Set = &set;
	m_voiceNo = voiceNo;

	for (int mseg = 0; mseg < 5; mseg++) {
		m_MSEG_Envelope[mseg].init(*m_Set, m_Set->m_MSEGData[mseg], m_Set->m_MSEGData_changed[mseg], m_voiceNo, mseg, -1);
	}

	m_bNoteOff.store(true);
}

void CVASTVca::updateVariables() {
	//??
}

void CVASTVca::noteOn(ULong64_t startPlayTimestamp, bool legatoStartSustain) {
	m_startPlayTimestamp = startPlayTimestamp;
	updateVariables(); //update ADSR changes for new note only

	for (int mseg = 0; mseg < 5; mseg++) {
		if (m_Set->m_RoutingBuffers.msegUsed[mseg].load())
			m_MSEG_Envelope[mseg].noteOn(m_startPlayTimestamp, legatoStartSustain);
	}
	m_bNoteOff.store(false);
}

void CVASTVca::noteOff(float releaseVelocity) {
	m_bNoteOff.store(true);
	for (int mseg = 0; mseg < 5; mseg++) {
		if (m_Set->m_RoutingBuffers.msegUsed[mseg].load()) {
			m_MSEG_Envelope[mseg].noteOff(releaseVelocity);

			//testing
			if (m_MSEG_Envelope[mseg].isHardStopNoteOff())
				m_bNoteOff.store(false);
			//testing
		}
	}
}

bool CVASTVca::hardStop() {
	VDBG("hardstop voice " << m_voiceNo);
	for (int mseg = 0; mseg < 5; mseg++) {
		if (m_Set->m_RoutingBuffers.msegUsed[mseg].load())
			m_MSEG_Envelope[mseg].hardStop();
	}
	return true; //CHECK
}

bool CVASTVca::isActive() {
	for (int mseg = 0; mseg < 5; mseg++) {
		if (m_Set->m_RoutingBuffers.msegUsed[mseg].load())
			if (m_MSEG_Envelope[mseg].isActive())
				return true;
	}
	return false;
}

bool CVASTVca::isHardStop() {
	for (int mseg = 0; mseg < 5; mseg++) {
		if (m_Set->m_RoutingBuffers.msegUsed[mseg].load())
			if (m_MSEG_Envelope[mseg].isHardStop())
				return true;
	}
	return false;
}

bool CVASTVca::isNoteOff() {
    return m_bNoteOff.load();
}
