/*
VAST Dynamics
*/

#pragma once

#include "VASTEngineHeader.h"
#include "VASTSettings.h"
#include "VASTMSEGData.h"

class alignas(16) CVASTMSEGEnvelope {
public:
	CVASTMSEGEnvelope();
	~CVASTMSEGEnvelope() {}

	// run time interface
	void init(CVASTSettings &set, VASTMSEGData &data, VASTMSEGData &datalive, int voiceNo, int mseg, int stepSeq);

	void copyStateFrom(CVASTMSEGEnvelope &copyEnvelope);

	void updateVariables(float AttackTime, float DecayTime, float SustainLevel, float ReleaseTime);
	bool isNoteOff();
	bool isHardStopNoteOff();
	//float getEnvelope(bool noSustain = false);
	void getEnvelopeRange(float* msegWritePointer, int currentFrame, int numFrames, bool noSustain = false);
	float getEnvelopeStepSeq(int bufferSample);
	float getValue();
	void noteOn(ULong64_t startPlayTimestamp, bool legatoStartSustain);
	void noteOff(float releaseVelocity = 0.f);
	bool isActive();
	bool isHardStop();
	void reset();
	bool hardStop(); //returns success
	void resynchNoteOn();

private:
	CVASTSettings * m_Set;

    std::atomic<int> m_iSamplesSinceNoteOn = -1;
    std::atomic<int> m_iSamplesSinceSegmentStart = -1;
    std::atomic<int> m_activeSegment = -1;
    std::atomic<bool> m_bRisingSegment;
    std::atomic<bool> m_bIsNoteOff;
    std::atomic<double> m_dEnvelope = 0.0;
    std::atomic<double> m_dSegment = 0.0;
	std::atomic<bool> m_bIsActive = false;
    std::atomic<bool> m_bHardStop;
    std::atomic<bool> m_bHardStopNoteOff;
    std::atomic<bool> m_bRelease = false;
    std::atomic<double> m_dReleaseValue = 0.0;
    std::atomic<double> m_dReleaseVelocity = 0.f;
    std::atomic<double> m_dPlannedReleaseVal = 0.f;
    std::atomic<int> m_voiceNo = -1;
    std::atomic<int> m_mseg = -1;
    std::atomic<int> m_stepSeq = -1;

	float m_iBufferSamples = 0;

	ULong64_t m_startPlayTimestamp = 0;
	VASTMSEGData* myData = nullptr;
	VASTMSEGData* myDataLive = nullptr;

	VASTMSEGData::ControlPoint* m_startPoint;
	VASTMSEGData::ControlPoint* m_endPoint;
    std::atomic<int> m_lastActiveSegment = 0;
    std::atomic<int> m_numSegments = 0;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CVASTMSEGEnvelope)
};
