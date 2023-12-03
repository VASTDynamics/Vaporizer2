/*
VAST Dynamics

How to calculate quadratic beziers
x from 0 to 1: 
y = (1-x)^2  * startval  + 2x(1-x) * intermediateval + x^2 * endval

http://slideplayer.com/slide/6554987/

*/

#include "VASTMSEGEnvelope.h"
#include "../Engine/VASTEngineHeader.h"
#include "VASTMSEGData.h"
#include "../Plugin/VASTAudioProcessor.h"

CVASTMSEGEnvelope::CVASTMSEGEnvelope(CVASTSettings& set, VASTMSEGData& data, VASTMSEGData& datalive, MYUINT voiceNo, int mseg, int stepSeq) :
								m_Set(&set), myData(&data), myDataLive(&datalive)
{
	m_voiceNo.store(voiceNo);
	m_mseg.store(mseg);
	m_stepSeq.store(stepSeq);
	m_dEnvelope.store(0);
}

void CVASTMSEGEnvelope::init() {
	m_dEnvelope.store(0);
	reset();
}

void CVASTMSEGEnvelope::copyStateFrom(CVASTMSEGEnvelope &copyEnvelope) {
	m_iSamplesSinceNoteOn.store(copyEnvelope.m_iSamplesSinceNoteOn.load());
	m_iSamplesSinceSegmentStart.store(copyEnvelope.m_iSamplesSinceSegmentStart.load());
	m_activeSegment.store(copyEnvelope.m_activeSegment.load());
	m_lastActiveSegment.store(copyEnvelope.m_lastActiveSegment.load());
	m_dEnvelope.store(copyEnvelope.m_dEnvelope.load());
	m_dSegment.store(copyEnvelope.m_dSegment.load());
	m_bRisingSegment.store(copyEnvelope.m_bRisingSegment.load());
	m_bIsActive.store(copyEnvelope.m_bIsActive.load());
	m_bHardStop.store(copyEnvelope.m_bHardStop.load());
	m_bHardStopNoteOff.store(copyEnvelope.m_bHardStopNoteOff.load());
	m_bIsNoteOff.store(copyEnvelope.m_bIsNoteOff.load());
	m_bRelease.store(copyEnvelope.m_bRelease.load());
	m_dReleaseValue.store(copyEnvelope.m_dReleaseValue.load());
	m_dReleaseVelocity.store(copyEnvelope.m_dReleaseVelocity.load());
	m_dPlannedReleaseVal.store(copyEnvelope.m_dPlannedReleaseVal.load());
	m_mseg.store(copyEnvelope.m_mseg.load());
	m_stepSeq.store(copyEnvelope.m_stepSeq.load());
	m_numSegments.store(copyEnvelope.m_numSegments.load());

    m_startPlayTimestamp = copyEnvelope.m_startPlayTimestamp;
    m_startPoint = copyEnvelope.m_startPoint;
    m_endPoint = copyEnvelope.m_endPoint;
	myData = copyEnvelope.myData;
	myDataLive = copyEnvelope.myDataLive;
	//int m_voiceNo = -1; //not copied
}

void CVASTMSEGEnvelope::reset() {
	VDBG("Voice " << m_voiceNo.load() << " MSEG " << m_mseg.load() << " reset");
	m_iSamplesSinceNoteOn.store(-1);
	m_iSamplesSinceSegmentStart.store(-1);
	m_activeSegment.store(0);
	m_lastActiveSegment.store(-1);
	m_dEnvelope.store(0.0);
	m_dSegment.store(0.0); //0 to 1 or 1 to 0 per segment
	m_bRisingSegment.store(true);
	m_bIsActive.store(false);
	m_bHardStop.store(false);
    m_bHardStopNoteOff.store(false);
    m_bIsNoteOff.store(false);
    m_bRelease.store(false);
	m_dReleaseValue.store(0.0f);
	m_dReleaseVelocity.store(0.0f);
	m_dPlannedReleaseVal.store(0.f);

	m_startPoint = myData->getSegmentStart(m_activeSegment.load());
	m_endPoint = myData->getSegmentEnd(m_activeSegment.load());

	myData->setUIDisplay(0, 0, 10000000, m_voiceNo.load(), false);
	myDataLive->setUIDisplay(0, 0, 10000000, m_voiceNo.load(), false);
}

void CVASTMSEGEnvelope::noteOn(ULong64_t startPlayTimestamp, bool legatoStartSustain) {
#ifdef _DEBUG
	VDBG("Voice " << m_voiceNo.load() << " MSEG " << m_mseg.load() << " noteOn called");
	if (m_bRelease.load()) {
		VDBG("ERROR! Voice " << m_voiceNo.load() << " MSEG " << m_mseg.load() << " noteOn but still in Release mode!!!");
	}
#endif 

	if (legatoStartSustain) {
		//keep it all? is it the right mseg? CHECK
		//int spoint = myData->getSustainPoint();
		//if ((spoint >= 0) && (spoint < myData->getNumSegments())) {
		//	m_activeSegment = spoint - 1; //end is sustain point
		//	m_iSamplesSinceSegmentStart = myData->getSegmentEnd(m_activeSegment)->segmentLengthInSamples;
		//	m_dEnvelope = myData->getSegmentEnd(m_activeSegment)->yVal;
		//}
	}
	else {
		m_startPlayTimestamp = startPlayTimestamp;
		m_iSamplesSinceNoteOn.store(-1);
		m_iSamplesSinceSegmentStart.store(-1);
		m_activeSegment.store(0);
		m_lastActiveSegment.store(-1);
	}

	m_startPoint = myData->getSegmentStart(m_activeSegment.load());
	m_endPoint = myData->getSegmentEnd(m_activeSegment.load());

	if (!legatoStartSustain) {
		if (myData->getSegmentEnd(m_activeSegment.load())->yVal >= myData->getSegmentStart(m_activeSegment.load())->yVal) { //rising
			m_dSegment.store(0.0f); //0 to 1 or 1 to 0 per segment
			m_bRisingSegment.store(true);
		}
		else { //falling
			m_dSegment.store(1.0f); //0 to 1 or 1 to 0 per segment
			m_bRisingSegment.store(false);
		}
		m_dEnvelope.store(myData->getSegmentStart(m_activeSegment.load())->yVal);
	}

    m_bIsNoteOff.store(false);
	m_bIsActive.store(true);
    m_bHardStop.store(false);
    m_bHardStopNoteOff.store(false);
    m_bRelease.store(false);
	m_dReleaseValue.store(0.0f);
    m_dReleaseVelocity.store(0.0f);
    m_dPlannedReleaseVal.store(0.0f);
}

void CVASTMSEGEnvelope::noteOff(float releaseVelocity) {
	if (!isActive())
		return;

	if (m_bHardStop.load()) {
        m_bHardStopNoteOff.store(true);
		VDBG("Voice " << m_voiceNo.load() << " MSEG " << m_mseg.load() << " Hardstop noteoff");
		return;
	}
	m_bIsNoteOff.store(true);
	m_dReleaseVelocity.store(releaseVelocity);

	//set release here?
	int spoint = myData->getSustainPoint();
	if ((spoint >= 0) && (spoint < myData->getNumSegments())) { //dont release if sustainpoint is last
		m_activeSegment.store(spoint);
		m_iSamplesSinceSegmentStart.store(-1);
		m_bRelease.store(true);
		m_dReleaseValue.store(m_dEnvelope.load());
		if (myData->getSegmentEnd(m_activeSegment.load())->yVal > myData->getSegmentStart(m_activeSegment.load())->yVal) { //rising
			m_dSegment.store(0.f); //0 to 1 or 1 to 0 per segment
			m_bRisingSegment.store(true);
		}
		else { //falling
			m_dSegment.store(1.f); //0 to 1 or 1 to 0 per segment
			m_bRisingSegment.store(false);
		}
	}
	else if (spoint == myData->getNumSegments()) { //sustainpoint is last
		reset();
	}
}

void CVASTMSEGEnvelope::updateVariables(float AttackTime, float DecayTime, float SustainLevel, float ReleaseTime) {
//TODO
}

bool CVASTMSEGEnvelope::isActive() {
	return m_bIsActive.load();
}

bool CVASTMSEGEnvelope::isNoteOff() {
	if (!isActive())
		return false; 

	if (!m_bHardStop.load())
		return m_bIsNoteOff.load();
	else
		return false;
}

bool CVASTMSEGEnvelope::isHardStop() {
	return m_bHardStop.load();
}

bool CVASTMSEGEnvelope::isHardStopNoteOff() { //NoteOff during Hardstop
	if (!isActive())
		return false;

	return m_bHardStopNoteOff.load();
}


bool CVASTMSEGEnvelope::hardStop() {
	if (!isActive())
		return false; // not possible

	if (m_bHardStop.load()) {
		return false; // not possible
	}
	m_bHardStop.store(true);
	return true;
}

float CVASTMSEGEnvelope::getValue() { // just to get the value
	if ((isActive() == false) || (m_dEnvelope < 0.0f)) //TODO CHECK WITH CURRENT FRAME!
		return 0.0f;
	return m_dEnvelope;
}

void CVASTMSEGEnvelope::resynchNoteOn() {
	struct timeval tp;
	m_Set->_gettimeofday(&tp);
	noteOn(static_cast<ULong64_t>(tp.tv_sec) * 1000 + tp.tv_usec / 1000, false);
}

void CVASTMSEGEnvelope::getEnvelopeRange(float* msegWritePointer, int currentFrame, int numFrames, bool noSustain) { //called during processAudio
	m_iBufferSamples += numFrames;
	for (int frame = 0; frame < numFrames; frame++) {
		m_iSamplesSinceNoteOn+=1;
		m_iSamplesSinceSegmentStart+=1;
		
		if (!isActive()) {
			msegWritePointer[currentFrame + frame] = 0.0f;
			continue;
		}
		if (m_bHardStop.load()) {
			if (m_dEnvelope > 0.00) {
				m_dEnvelope.store( m_dEnvelope.load() - float(1.0 / (m_Set->m_nSampleRate.load() * 0.02f))); // 20 ms
			}
			else {
				m_dEnvelope = 0.00f;
				VDBG(m_voiceNo.load() << " hardstop mode completed " << m_dEnvelope.load() << " MSEG " << m_mseg.load());
				reset();
			}
			if (m_dEnvelope < 0.f) m_dEnvelope = 0.f;
			msegWritePointer[currentFrame + frame] = m_dEnvelope;
			continue;
		}

		//check for changes
		if (m_iSamplesSinceSegmentStart.load() < 0) {
			resynchNoteOn();
			msegWritePointer[currentFrame + frame] = 0.0f;
			continue;
		}
		if (m_activeSegment.load() >= myData->getNumSegments()) {
			resynchNoteOn();
			msegWritePointer[currentFrame + frame] = 0.0f;
			continue;
		}

		// ==== expensive part
		if (m_lastActiveSegment.load() != m_activeSegment.load()) { //buffering
			m_startPoint = myData->getSegmentStart(m_activeSegment.load());
			vassert(m_startPoint != nullptr);
			m_endPoint = myData->getSegmentEnd(m_activeSegment.load());
			vassert(m_endPoint != nullptr);
			m_lastActiveSegment.store(m_activeSegment.load());
			m_numSegments = myData->getNumSegments();
			//VDBG("MSEG: " + String(m_mseg.load()) + " Segment: " + String(m_activeSegment.load()) + " length in Samples: " + String(m_endPoint->segmentLengthInSamples) + " since noteOn: " + String(m_iSamplesSinceNoteOn) + "  ppqPos: " + String(m_Set->m_dPpqPosition) + " bufferSamples: "+ String(m_iBufferSamples));
		}

		myData->calcSegmentCoefficients(m_Set->m_nSampleRate, m_startPlayTimestamp, m_activeSegment.load(), m_iSamplesSinceSegmentStart.load(), m_endPoint->segmentLengthInSamples, m_Set, m_voiceNo.load());
        myData->setUIDisplay(m_activeSegment.load(), m_iSamplesSinceSegmentStart.load(), m_endPoint->segmentLengthInSamples, m_voiceNo.load(), true);
        myDataLive->setUIDisplay(m_activeSegment.load(), m_iSamplesSinceSegmentStart.load(), m_endPoint->segmentLengthInSamples, m_voiceNo.load(), true);
        
		float startYVal = m_startPoint->yVal;
		float endyVal = m_endPoint->yVal;
		if (m_bRelease.load() && (m_startPoint->isSustain)) {
			m_dPlannedReleaseVal.store( m_startPoint->yVal);
			startYVal = m_dReleaseValue.load(); // release from where we are to avoid clicks - but keep time
		}
						
		if ((m_endPoint->isMPELift) && m_bRelease.load()) { //check if endpoint is MPE lift / release velocity point
			endyVal = m_dReleaseVelocity.load() * endyVal;
		}
		if ((m_startPoint->isMPELift) && m_bRelease.load()) {
			if (!m_startPoint->isSustain) { //points after lift point
				startYVal = m_dReleaseVelocity.load() * startYVal;
			}
		}

		bool bfirst = true;
		while ((!(m_iSamplesSinceSegmentStart.load() >= m_endPoint->segmentLengthInSamples) && (frame < (numFrames - 1))) || bfirst) {
			if (!bfirst) {
				frame++;
				m_iSamplesSinceNoteOn+=1;
				m_iSamplesSinceSegmentStart+=1;
			}

            //Dont do that per sample!
			//myData->setUIDisplay(m_activeSegment.load(), m_iSamplesSinceSegmentStart.load(), m_endPoint->segmentLengthInSamples, m_voiceNo.load(), true);
			//myDataLive->setUIDisplay(m_activeSegment.load(), m_iSamplesSinceSegmentStart.load(), m_endPoint->segmentLengthInSamples, m_voiceNo.load(), true);

			m_dSegment.store(m_endPoint->offset + m_dSegment * m_endPoint->coeff); //0 to 1 or 1 to 0 per segment
			if (isnan(m_dSegment.load()) || (m_endPoint->segmentLengthInSamples < 0)) { //safety for online step increase
				//vassertfalse;
				VDBG("MSEG error.");
				resynchNoteOn();				
				msegWritePointer[currentFrame + frame] = 0.0f;
				bfirst = false;
				continue;
			}
			vassert(!isnan(m_dSegment.load()));
			vassert(m_endPoint->segmentLengthInSamples >= 0);
			vassert(m_activeSegment.load() >= 0);

			if (m_bRisingSegment.load()) {
				m_dSegment.store((m_dSegment.load() > 1.0f) ? 1.0f : m_dSegment.load()); //check overrun
				m_dEnvelope.store(startYVal + m_dSegment.load() * (endyVal - startYVal));  //startpoint and release????
			}
			else {
				m_dSegment.store((m_dSegment.load() < 0.0f) ? 0.0f : m_dSegment.load()); //check overrun
				m_dEnvelope.store(startYVal - (1.0f - m_dSegment.load()) * (startYVal - endyVal));
			}
			msegWritePointer[currentFrame + frame] = m_dEnvelope;
			bfirst = false;
		}

		if (m_iSamplesSinceSegmentStart.load() >= m_endPoint->segmentLengthInSamples) {
			if (m_activeSegment.load() + 1 >= m_numSegments.load()) {
				if ((!m_bIsNoteOff.load()) && (myData->hasLoop) && (m_activeSegment == (myData->loopEndPoint - 1))) { //last segment is loop end?
					m_activeSegment.store(myData->loopStartPoint);

					m_iSamplesSinceSegmentStart.store(-1);
					if (myData->getSegmentEnd(m_activeSegment.load())->yVal >= myData->getSegmentStart(m_activeSegment.load())->yVal) { //rising
						m_dSegment.store(0.0f); //0 to 1 or 1 to 0 per segment
						m_bRisingSegment.store(true);
					}
					else { //falling
						m_dSegment.store(1.0f); //0 to 1 or 1 to 0 per segment
						m_bRisingSegment.store(false);
					}
				}
				else
					if ((!m_bIsNoteOff.load()) && (myData->hasLoop)) {
						//VDBG("MSEG: " + String(m_mseg.load()) + " Segment: " + String(m_activeSegment) + " length in Samples: " + String(m_endPoint->segmentLengthInSamples) + " since noteOn: " + String(m_iSamplesSinceNoteOn) + "  ppqPos: " + String(m_Set->m_dPpqPosition) + " bufferSamples: " + String(m_iBufferSamples));
						resynchNoteOn();
						msegWritePointer[currentFrame + frame] = 0.0f;
						continue;
					}
					else
						reset(); //switches note off
			}
			else {
				if ((!m_bIsNoteOff.load()) && (myData->hasLoop) && (m_activeSegment == (myData->loopEndPoint - 1))) { //loop end?
					//VDBG("MSEG: " + String(m_mseg.load()) + " Segment: " + String(m_activeSegment) + " length in Samples: " + String(m_endPoint->segmentLengthInSamples) + " since noteOn: " + String(m_iSamplesSinceNoteOn) + "  ppqPos: " + String(m_Set->m_dPpqPosition) + " bufferSamples: " + String(m_iBufferSamples));
					m_activeSegment.store(myData->loopStartPoint);
				}
				else if ((!noSustain) && ((!m_bIsNoteOff.load()) && (m_endPoint->isSustain))) { //is sustain?
					if (m_bRisingSegment.load())
						m_dSegment.store(1.0f);
					else
						m_dSegment.store(0.0f);
					m_iSamplesSinceSegmentStart.store(m_endPoint->segmentLengthInSamples - 1);
					msegWritePointer[currentFrame + frame] = m_dEnvelope;
					continue; // dont start new
				}
				else {
					m_activeSegment+=1; //next segment
				}

				//check for changes
				if (m_activeSegment.load() >= myData->getNumSegments()) {
					resynchNoteOn();
					msegWritePointer[currentFrame + frame] = 0.0f;
					continue;
				}
				m_iSamplesSinceSegmentStart.store(-1);
				if (myData->getSegmentEnd(m_activeSegment.load())->yVal >= myData->getSegmentStart(m_activeSegment.load())->yVal) { //rising
					m_dSegment.store(0.0f); //0 to 1 or 1 to 0 per segment
					m_bRisingSegment.store(true);
				}
				else { //falling
					m_dSegment.store(1.0f); //0 to 1 or 1 to 0 per segment
					m_bRisingSegment.store(false);
				}
			}
		}
	}	
}

float CVASTMSEGEnvelope::getEnvelopeStepSeq(int bufferSample) { //called during processAudio
	if (!isActive()) return 0.0f;
	if (m_bHardStop.load()) {
		if (m_dEnvelope > 0.00) {
			m_dEnvelope.store(m_dEnvelope.load() - float(1.0 / (m_Set->m_nSampleRate.load() * 0.02f))); // 20 ms
			VDBG(m_voiceNo.load() << " in hardstop mode returning " << m_dEnvelope.load() << " STEPSEQ " << m_stepSeq.load());
		}
		else {
			m_dEnvelope = 0.00f;
			reset();
		}
		if (m_dEnvelope < 0.f) m_dEnvelope = 0.f;
		return m_dEnvelope;
	}

	//const ScopedReadLock myScopedLock(myData->mReadWriteLock);
	m_iSamplesSinceNoteOn+=1;
	m_iSamplesSinceSegmentStart+=1;

	//check for changes
	if (m_iSamplesSinceSegmentStart.load() < 0) {
		resynchNoteOn();
		return 0.f;
	}
	if (m_activeSegment.load() >= myData->getNumSegments()) {
		resynchNoteOn();
		return 0.f;
	}

	// ==== expensive part
	if (m_lastActiveSegment.load() != m_activeSegment.load()) { //buffering
		m_startPoint = myData->getSegmentStart(m_activeSegment.load());
		vassert(m_startPoint != nullptr);
		m_endPoint = myData->getSegmentEnd(m_activeSegment.load());
		vassert(m_endPoint != nullptr);
		m_lastActiveSegment.store(m_activeSegment.load());
		m_numSegments.store(myData->getNumSegments());
	}

	bool synch = false;
	int beats = 0;
	if (myData->m_stepSeqNo == 0) {
		synch = *m_Set->m_State->m_bStepSeqSynch_STEPSEQ1;
		beats = *m_Set->m_State->m_uStepSeqTimeBeats_STEPSEQ1;
	}
	else if (myData->m_stepSeqNo == 1) {
		synch = *m_Set->m_State->m_bStepSeqSynch_STEPSEQ2;
		beats = *m_Set->m_State->m_uStepSeqTimeBeats_STEPSEQ2;
	}
	else if (myData->m_stepSeqNo == 2) {
		synch = *m_Set->m_State->m_bStepSeqSynch_STEPSEQ3;
		beats = *m_Set->m_State->m_uStepSeqTimeBeats_STEPSEQ3;
	};

	if ((synch == static_cast<bool>(SWITCH::SWITCH_ON)) && (m_Set->m_bPpqIsPlaying)) {
		if (!m_Set->m_bPpqIsPlaying) return 0.f;
		float l_fIntervalTime = m_Set->getIntervalTimeFromDAWBeats(beats);
		float stepDuration = static_cast<int> (std::ceil(m_Set->m_nSampleRate * (l_fIntervalTime / 1000.f)));  //interval time is in milliseconds, stepduration in samples
		int totalDurationInSamples = myData->getStepSeqSteps() * stepDuration;
		float realPos = m_Set->m_dPpqPosition / m_Set->getIntervalRatio(beats);
		float plusDelta = (float(bufferSample) / float(m_Set->m_nSampleRate)) / (l_fIntervalTime * 0.001f); //in beats
		int shallSamplesSinceBegin = int(stepDuration * (realPos + plusDelta)) % totalDurationInSamples;
		int sumSamples = 0;		
		for (int segment = 0; segment < myData->controlPoints.size() - 1; segment++) {
			m_startPoint = myData->getSegmentStart(segment);
			m_endPoint = myData->getSegmentEnd(segment);

			m_endPoint->segmentLengthInSamples = (m_endPoint->xVal - m_startPoint->xVal) * totalDurationInSamples;

			sumSamples += m_endPoint->segmentLengthInSamples;
			if (sumSamples > shallSamplesSinceBegin) {
				m_activeSegment.store(segment);
				m_iSamplesSinceSegmentStart.store(shallSamplesSinceBegin - (sumSamples - m_endPoint->segmentLengthInSamples));
				break;
			}
		}

		myData->setUIDisplay(m_activeSegment.load(), m_iSamplesSinceSegmentStart.load(), m_endPoint->segmentLengthInSamples, m_voiceNo.load(), true);
		myDataLive->setUIDisplay(m_activeSegment.load(), m_iSamplesSinceSegmentStart.load(), m_endPoint->segmentLengthInSamples, m_voiceNo.load(), true);

		float x = float(m_iSamplesSinceSegmentStart.load()) / float(m_endPoint->segmentLengthInSamples);

		//y = u0(1−x)^3 + 3u1((1−x)^2)x + 3u2(1−x)x^2 + u3x^3 //cubic bezier
		//y = 3u1((1−x)^2)x + 3u2(1−x)x^2 + x^3 //u0 = 0, u1 = 1, u2 = u3 = curvy!
		
		if (myData->getSegmentEnd(m_activeSegment.load())->yVal >= myData->getSegmentStart(m_activeSegment.load())->yVal) { //rising
			m_bRisingSegment.store(true);
		}
		else { //falling
			m_bRisingSegment.store(false);
		}
	
		double curve = m_endPoint->curvy;

		if (!m_bRisingSegment.load())
			curve = 1.f - curve;
		m_bRisingSegment.store(true);

		//m_dSegment = 3.f * curve * powf(1.f - x, 2.f) * x +
		//			3.f * curve * (1.f - x) * x * x +
		//	x * x * x;
		m_dSegment.store(cubicBezierApproximation(x, curve)); //check performance
				
		if (isnan(m_dSegment.load()) || (m_endPoint->segmentLengthInSamples < 0)) { //safety for online step increase
			resynchNoteOn();
			return 0.f;
		}
		vassert(!isnan(m_dSegment.load()));
		vassert(m_endPoint->segmentLengthInSamples >= 0);
	}
	else {

		myData->calcSegmentCoefficients(m_Set->m_nSampleRate.load(), m_startPlayTimestamp, m_activeSegment.load(), m_iSamplesSinceSegmentStart.load(), m_endPoint->segmentLengthInSamples, m_Set, m_voiceNo.load());
        if (bufferSample==0) { //only once in the buffer
            myData->setUIDisplay(m_activeSegment.load(), m_iSamplesSinceSegmentStart.load(), m_endPoint->segmentLengthInSamples, m_voiceNo.load(), true); //duplicate
            myDataLive->setUIDisplay(m_activeSegment.load(), m_iSamplesSinceSegmentStart.load(), m_endPoint->segmentLengthInSamples, m_voiceNo.load(), true); //CHECK
        }
		m_dSegment.store(m_endPoint->offset + m_dSegment.load() * m_endPoint->coeff); //0 to 1 or 1 to 0 per segment
		if (isnan(m_dSegment.load()) || (m_endPoint->segmentLengthInSamples < 0)) { //safety for online step increase
			resynchNoteOn();
			return 0.f;
		}
		vassert(!isnan(m_dSegment.load()));
		vassert(m_endPoint->segmentLengthInSamples >= 0);
	}

	float startYVal = m_startPoint->yVal;
	if (m_bRelease.load() && (m_startPoint->isSustain))
		startYVal = m_dReleaseValue.load(); // release from where we are to avoid clicks - but keep time
	if (m_bRisingSegment.load()) {
		m_dSegment.store((m_dSegment.load() > 1.0f) ? 1.0f : m_dSegment.load()); //check overrun
		m_dEnvelope.store( startYVal + m_dSegment.load() * (m_endPoint->yVal - startYVal));  //startpoint and release????
	}
	else {
		m_dSegment.store((m_dSegment.load() < 0.0f) ? 0.0f : m_dSegment.load()); //check overrun
		m_dEnvelope.store(startYVal - (1.0f - m_dSegment.load()) * (startYVal - m_endPoint->yVal));
	}

	if (m_iSamplesSinceSegmentStart.load() >= m_endPoint->segmentLengthInSamples) {
		if (m_activeSegment.load() + 1 >= m_numSegments) {
			if ((!m_bIsNoteOff.load()) && (myData->hasLoop) && (m_activeSegment.load() == (myData->loopEndPoint - 1))) { //last segment is loop end?
				m_activeSegment.store( myData->loopStartPoint);
				m_iSamplesSinceSegmentStart.store(-1);
				if (myData->getSegmentEnd(m_activeSegment.load())->yVal >= myData->getSegmentStart(m_activeSegment.load())->yVal) { //rising
					m_dSegment.store(0.0f); //0 to 1 or 1 to 0 per segment
					m_bRisingSegment.store(true);
				}
				else { //falling
					m_dSegment.store(1.0f); //0 to 1 or 1 to 0 per segment
					m_bRisingSegment.store(false);
				}
			}
			else
				if ((!m_bIsNoteOff.load()) && (myData->hasLoop)) {
					resynchNoteOn();
					return 0.f;
				}
				else
					reset(); //switches note off
		}
		else {
			if ((!m_bIsNoteOff.load()) && (myData->hasLoop) && (m_activeSegment.load() == (myData->loopEndPoint - 1))) { //loop end?
				m_activeSegment.store(myData->loopStartPoint);
			}
			else if ((!m_bIsNoteOff.load()) && (m_endPoint->isSustain)) { //is sustain?
				if (m_bRisingSegment.load())
					m_dSegment.store(1.0f);
				else
					m_dSegment.store(0.0f);
				m_iSamplesSinceSegmentStart.store( m_endPoint->segmentLengthInSamples - 1);

				return m_dEnvelope.load(); // dont start new
			}
			else {
				m_activeSegment+=1; //next segment
			}

			//check for changes
			if (m_activeSegment.load() >= myData->getNumSegments()) {
				resynchNoteOn();
				return 0.f;
			}

			m_iSamplesSinceSegmentStart.store(-1);
			if (myData->getSegmentEnd(m_activeSegment.load())->yVal >= myData->getSegmentStart(m_activeSegment.load())->yVal) { //rising
				m_dSegment.store(0.0f); //0 to 1 or 1 to 0 per segment
				m_bRisingSegment.store(true);
			}
			else { //falling
				m_dSegment.store(1.0f); //0 to 1 or 1 to 0 per segment
				m_bRisingSegment.store(false);
			}
		}
	}

	return m_dEnvelope.load();
}
