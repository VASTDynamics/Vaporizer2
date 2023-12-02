/*
VAST Dynamics Audio Software (TM)
*/

#pragma once

#include "VASTEnvelope.h"
#include "VASTMSEGEnvelope.h"
#include "VASTSettings.h"

class CVASTVca
{
public:
	CVASTVca();
	virtual ~CVASTVca(void);
	void init(CVASTSettings &set, MYUINT voiceNo);

	void noteOn(ULong64_t startPlayTimestamp, bool legatoStartSustain);
	void noteOff(float releaseVelocity);
	void updateVariables();
	//bool processAudioFrame(sRoutingBuffers& routingBuffers, int currentFrame);

	bool isActive();
	bool hardStop();	//returns success
	bool isHardStop();
	bool isNoteOff();

	CVASTMSEGEnvelope m_MSEG_Envelope[5]; 

private:
	
	CVASTSettings* m_Set;
	MYUINT m_voiceNo = 0;
	std::atomic<bool> m_bNoteOff = true;
	ULong64_t m_startPlayTimestamp = 0;

	JUCE_LEAK_DETECTOR(CVASTVca)
};
