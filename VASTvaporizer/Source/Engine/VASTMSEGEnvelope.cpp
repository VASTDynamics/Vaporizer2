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

CVASTMSEGEnvelope::CVASTMSEGEnvelope() {
}

void CVASTMSEGEnvelope::init(CVASTSettings &set, VASTMSEGData &data, VASTMSEGData &datalive, MYUINT voiceNo, int mseg, int stepSeq) {
	m_Set = &set;
	myData = &data;
	myDataLive = &datalive;
	m_voiceNo = voiceNo;
    m_mseg = mseg;
    m_stepSeq = stepSeq;
	m_dEnvelope = 0;
	reset();
}

void CVASTMSEGEnvelope::copyStateFrom(CVASTMSEGEnvelope &copyEnvelope) {
	m_iSamplesSinceNoteOn = copyEnvelope.m_iSamplesSinceNoteOn;
	m_iSamplesSinceSegmentStart = copyEnvelope.m_iSamplesSinceSegmentStart;
	m_activeSegment = copyEnvelope.m_activeSegment;
	m_lastActiveSegment = copyEnvelope.m_lastActiveSegment;
	m_dEnvelope = copyEnvelope.m_dEnvelope;
	m_dSegment = copyEnvelope.m_dSegment;
	m_bRisingSegment = copyEnvelope.m_bRisingSegment;
	m_bIsActive = copyEnvelope.m_bIsActive;
	m_bHardStop = copyEnvelope.m_bHardStop;
	m_bHardStopNoteOff = copyEnvelope.m_bHardStopNoteOff;
	m_bIsNoteOff = copyEnvelope.m_bIsNoteOff;
	m_bRelease = copyEnvelope.m_bRelease;
	m_dReleaseValue = copyEnvelope.m_dReleaseValue;
	m_dReleaseVelocity = copyEnvelope.m_dReleaseVelocity;
	m_dPlannedReleaseVal = copyEnvelope.m_dPlannedReleaseVal;
	m_startPoint = copyEnvelope.m_startPoint;
	m_endPoint = copyEnvelope.m_endPoint;
	m_mseg = copyEnvelope.m_mseg;
	m_stepSeq = copyEnvelope.m_stepSeq;
	m_startPlayTimestamp = copyEnvelope.m_startPlayTimestamp;
	m_numSegments = copyEnvelope.m_numSegments;
	myData = copyEnvelope.myData;
	myDataLive = copyEnvelope.myDataLive;
	//int m_voiceNo = -1; //not copied
}

void CVASTMSEGEnvelope::reset() {
	DBG("Voice " + String(m_voiceNo) + " MSEG " + String(m_mseg) + " reset");
	m_iSamplesSinceNoteOn = -1;
	m_iSamplesSinceSegmentStart = -1;
	m_activeSegment = 0;
	m_lastActiveSegment = -1;
	m_dEnvelope = 0.0;
	m_dSegment = 0.0; //0 to 1 or 1 to 0 per segment
	m_bRisingSegment = true;
	m_bIsActive = false;
	m_bHardStop = false;
	m_bHardStopNoteOff = false;
	m_bIsNoteOff = false;
	m_bRelease = false;
	m_dReleaseValue = 0.0f;
	m_dReleaseVelocity = 0.0f;
	m_dPlannedReleaseVal = 0.f;

	m_startPoint = myData->getSegmentStart(m_activeSegment);
	m_endPoint = myData->getSegmentEnd(m_activeSegment);

	myData->setUIDisplay(0, 0, 10000000, m_voiceNo, false);
	myDataLive->setUIDisplay(0, 0, 10000000, m_voiceNo, false);
}

void CVASTMSEGEnvelope::noteOn(ULong64_t startPlayTimestamp, bool legatoStartSustain) {
#ifdef _DEBUG
	DBG("Voice " + String(m_voiceNo) + " MSEG " + String(m_mseg) + " noteOn called");
	if (m_bRelease) {
		DBG("ERROR! Voice " + String(m_voiceNo) + " MSEG " + String(m_mseg) + " noteOn but still in Release mode!!!");
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
		m_iSamplesSinceNoteOn = -1;
		m_iSamplesSinceSegmentStart = -1;
		m_activeSegment = 0;
		m_lastActiveSegment = -1;
	}

	m_startPoint = myData->getSegmentStart(m_activeSegment);
	m_endPoint = myData->getSegmentEnd(m_activeSegment);

	if (!legatoStartSustain) {
		if (myData->getSegmentEnd(m_activeSegment)->yVal >= myData->getSegmentStart(m_activeSegment)->yVal) { //rising
			m_dSegment = 0.0f; //0 to 1 or 1 to 0 per segment
			m_bRisingSegment = true;
		}
		else { //falling
			m_dSegment = 1.0f; //0 to 1 or 1 to 0 per segment
			m_bRisingSegment = false;
		}
		m_dEnvelope = myData->getSegmentStart(m_activeSegment)->yVal;
	}

	m_bIsNoteOff = false;
	m_bIsActive = true;
	m_bHardStop = false;
	m_bHardStopNoteOff = false;
	m_bRelease = false;
	m_dReleaseValue = 0.0f;
	m_dReleaseVelocity = 0.0f;
	m_dPlannedReleaseVal = 0.f;
}

void CVASTMSEGEnvelope::noteOff(float releaseVelocity) {
	if (!isActive())
		return;

	if (m_bHardStop) {
		m_bHardStopNoteOff = true;
		DBG("Voice " + String(m_voiceNo) + " MSEG " + String(m_mseg) + " Hardstop noteoff");
		return;
	}
	m_bIsNoteOff = true;
	m_dReleaseVelocity = releaseVelocity;

	//set release here?
	int spoint = myData->getSustainPoint();
	if ((spoint >= 0) && (spoint < myData->getNumSegments())) { //dont release if sustainpoint is last
		m_activeSegment = spoint;
		m_iSamplesSinceSegmentStart = -1;
		m_bRelease = true;
		m_dReleaseValue = m_dEnvelope;
		if (myData->getSegmentEnd(m_activeSegment)->yVal > myData->getSegmentStart(m_activeSegment)->yVal) { //rising
			m_dSegment = 0.f; //0 to 1 or 1 to 0 per segment
			m_bRisingSegment = true;
		}
		else { //falling
			m_dSegment = 1.f; //0 to 1 or 1 to 0 per segment
			m_bRisingSegment = false;
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
	return m_bIsActive;
}

bool CVASTMSEGEnvelope::isNoteOff() {
	if (!isActive())
		return false; 

	if (!m_bHardStop)
		return m_bIsNoteOff;
	else
		return false;
}

bool CVASTMSEGEnvelope::isHardStop() {
	return m_bHardStop;
}

bool CVASTMSEGEnvelope::isHardStopNoteOff() { //NoteOff during Hardstop
	if (!isActive())
		return false;

	return m_bHardStopNoteOff;
}


bool CVASTMSEGEnvelope::hardStop() {
	if (!isActive())
		return false; // not possible

	if (m_bHardStop) {
		return false; // not possible
	}
	m_bHardStop = true;
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
	noteOn(tp.tv_sec * 1000 + tp.tv_usec / 1000, false);
}


void CVASTMSEGEnvelope::getEnvelopeRange(float* msegWritePointer, int currentFrame, int numFrames, bool noSustain) { //called during processAudio
	m_iBufferSamples += numFrames;
	for (int frame = 0; frame < numFrames; frame++) {
		m_iSamplesSinceNoteOn++;
		m_iSamplesSinceSegmentStart++;
		
		if (!isActive()) {
			msegWritePointer[currentFrame + frame] = 0.0f;
			continue;
		}
		if (m_bHardStop) {
			if (m_dEnvelope > 0.00) {
				m_dEnvelope -= float(1.0 / (m_Set->m_nSampleRate * 0.02f)); // 20 ms
			}
			else {
				m_dEnvelope = 0.00f;
				DBG(String(m_voiceNo) + " hardstop mode completed " + String(m_dEnvelope) + " MSEG " + String(m_mseg));
				reset();
			}
			if (m_dEnvelope < 0.f) m_dEnvelope = 0.f;
			msegWritePointer[currentFrame + frame] = m_dEnvelope;
			continue;
		}

		//check for changes
		if (m_iSamplesSinceSegmentStart < 0) {
			resynchNoteOn();
			msegWritePointer[currentFrame + frame] = 0.0f;
			continue;
		}
		if (m_activeSegment >= myData->getNumSegments()) {
			resynchNoteOn();
			msegWritePointer[currentFrame + frame] = 0.0f;
			continue;
		}

		// ==== expensive part
		if (m_lastActiveSegment != m_activeSegment) { //buffering
			m_startPoint = myData->getSegmentStart(m_activeSegment);
			vassert(m_startPoint != nullptr);
			m_endPoint = myData->getSegmentEnd(m_activeSegment);
			vassert(m_endPoint != nullptr);
			m_lastActiveSegment = m_activeSegment;
			m_numSegments = myData->getNumSegments();
			//DBG("MSEG: " + String(m_mseg) + " Segment: " + String(m_activeSegment) + " length in Samples: " + String(m_endPoint->segmentLengthInSamples) + " since noteOn: " + String(m_iSamplesSinceNoteOn) + "  ppqPos: " + String(m_Set->m_dPpqPosition) + " bufferSamples: "+ String(m_iBufferSamples));
		}

		myData->calcSegmentCoefficients(m_Set->m_nSampleRate, m_startPlayTimestamp, m_activeSegment, m_iSamplesSinceSegmentStart, m_endPoint->segmentLengthInSamples, m_Set, m_voiceNo);

		float startYVal = m_startPoint->yVal;
		float endyVal = m_endPoint->yVal;
		if (m_bRelease && (m_startPoint->isSustain)) {
			m_dPlannedReleaseVal = m_startPoint->yVal;
			startYVal = m_dReleaseValue; // release from where we are to avoid clicks - but keep time
		}
						
		if ((m_endPoint->isMPELift) && m_bRelease) { //check if endpoint is MPE lift / release velocity point 			
			endyVal = m_dReleaseVelocity * endyVal;
		}
		if ((m_startPoint->isMPELift) && m_bRelease) { 
			if (!m_startPoint->isSustain) { //points after lift point
				startYVal = m_dReleaseVelocity * startYVal;
			}
		}

		bool bfirst = true;
		while ((!(m_iSamplesSinceSegmentStart >= m_endPoint->segmentLengthInSamples) && (frame < (numFrames - 1))) || bfirst) {
			if (!bfirst) {
				frame++;
				m_iSamplesSinceNoteOn++;
				m_iSamplesSinceSegmentStart++;
			}

			myData->setUIDisplay(m_activeSegment, m_iSamplesSinceSegmentStart, m_endPoint->segmentLengthInSamples, m_voiceNo, true);
			myDataLive->setUIDisplay(m_activeSegment, m_iSamplesSinceSegmentStart, m_endPoint->segmentLengthInSamples, m_voiceNo, true);

			m_dSegment = m_endPoint->offset + m_dSegment * m_endPoint->coeff; //0 to 1 or 1 to 0 per segment
			if (isnan(m_dSegment) || (m_endPoint->segmentLengthInSamples < 0)) { //safety for online step increase
				//vassertfalse;
				DBG("MSEG error.");
				resynchNoteOn();				
				msegWritePointer[currentFrame + frame] = 0.0f;
				bfirst = false;
				continue;
			}
			vassert(!isnan(m_dSegment));
			vassert(m_endPoint->segmentLengthInSamples >= 0);
			vassert(m_activeSegment >= 0);

			if (m_bRisingSegment) {
				m_dSegment = (m_dSegment > 1.0f) ? 1.0f : m_dSegment; //check overrun 
				m_dEnvelope = startYVal + m_dSegment * (endyVal - startYVal);  //startpoint and release????
			}
			else {
				m_dSegment = (m_dSegment < 0.0f) ? 0.0f : m_dSegment; //check overrun
				m_dEnvelope = startYVal - (1.0f - m_dSegment) * (startYVal - endyVal);
			}
			msegWritePointer[currentFrame + frame] = m_dEnvelope;
			bfirst = false;
		}

		/*
		myData->calcSegmentCoefficients(m_Set->m_nSampleRate, m_startPlayTimestamp, m_activeSegment, m_iSamplesSinceSegmentStart, m_endPoint->segmentLengthInSamples, m_Set);
		if (approximatelyEqual(m_endPoint->curvy, 0.5f)) { //accuracy for linear curves
			if (m_bRisingSegment)
				m_dSegment = double(m_iSamplesSinceSegmentStart) / double(m_endPoint->segmentLengthInSamples); //0 to 1 or 1 to 0 per segment
			else 
				m_dSegment = 1.f - double(m_iSamplesSinceSegmentStart) / double(m_endPoint->segmentLengthInSamples); //0 to 1 or 1 to 0 per segment
		}
		else
			m_dSegment = m_endPoint->offset + m_dSegment * m_endPoint->coeff; //0 to 1 or 1 to 0 per segment
		if (isnan(m_dSegment) || (m_endPoint->segmentLengthInSamples < 0)) { //safety for online step increase
			resynchNoteOn();
			msegWritePointer[currentFrame + frame] = 0.0f;
			continue;
		}
		vassert(!isnan(m_dSegment));
		vassert(m_endPoint->segmentLengthInSamples >= 0);
		vassert(m_activeSegment >= 0);

		double startYVal = m_startPoint->yVal;
		if (m_bRelease && (m_startPoint->isSustain))
			startYVal = m_dReleaseValue; // release from where we are to avoid clicks - but keep time

		__m128 _fSegment;
		__m128 _fEnvelope;
		//calc 4 values simultaneoulsy - maybe not all are used
		_fSegment = _mm_setr_ps(m_dSegment, 
							   m_endPoint->offset + m_dSegment * m_endPoint->coeff, 
			                   m_endPoint->offset + (m_endPoint->offset + m_dSegment * m_endPoint->coeff) * m_endPoint->coeff, 
							   m_endPoint->offset + (m_endPoint->offset + (m_endPoint->offset + (m_endPoint->offset + m_dSegment * m_endPoint->coeff) * m_endPoint->coeff) * m_endPoint->coeff) * m_endPoint->coeff);
		if (m_bRisingSegment) {							
			_fSegment = _mm_min_ps(_fSegment, _allOne); //m_dSegment = (m_dSegment > 1.0f) ? 1.0f : m_dSegment; //check overrun 
			_fEnvelope = _mm_set1_ps(startYVal); //all = startYVal 
			__m128 _mul = _mm_set1_ps(m_endPoint->yVal - startYVal); //all = m_endPoint->yVal - startYVal
			__m128 _dd = _mm_mul_ps(_fSegment, _mul); // m_dSegment * (m_endPoint->yVal - startYVal);  
			_fEnvelope = _mm_add_ps(_fEnvelope, _dd); //m_dEnvelope = startYVal + m_dSegment * (m_endPoint->yVal - startYVal);  //startpoint and release????
		}
		else {
			_fSegment = _mm_max_ps(_fSegment, _allZero); // m_dSegment = (m_dSegment < 0.0f) ? 0.0f : m_dSegment; //check overrun
			_fEnvelope = _mm_set1_ps(startYVal); //all = startYVal 
			__m128 _mul = _mm_set1_ps(startYVal - m_endPoint->yVal); //all = startYVal - m_endPoint->yVal
			__m128 _sub = _mm_sub_ps(_allOne, _fSegment); //(1.0f - m_dSegment)
			__m128 _dd = _mm_mul_ps(_sub, _mul); // (1.0f - m_dSegment) * (startYVal - m_endPoint->yVal); 
			_fEnvelope = _mm_sub_ps(_fEnvelope, _dd); //m_dEnvelope = startYVal - (1.0f - m_dSegment) * (startYVal - m_endPoint->yVal);
		}

		if (m_iSamplesSinceSegmentStart + 3 < m_endPoint->segmentLengthInSamples) { //check 3!
            if ((reinterpret_cast<uintptr_t>(msegWritePointer) & 15) == 0) //check for 16 bit alignment
                //Check other option: copy to 16 bit aligned buffer before!
                _mm_storeu_ps(msegWritePointer + currentFrame + frame, _fEnvelope); //4th value is overwritten //check 16bit alignment for better performance
            else
                _mm_store_ps(msegWritePointer + currentFrame + frame, _fEnvelope); //4th value is overwritten //check 16bit alignment for better performance
			frame += 3;
			m_dSegment = get1f(_fSegment, 3); //TEST
			m_dEnvelope = get1f(_fEnvelope, 3); //TEST
			m_iSamplesSinceSegmentStart += 3;
		}
		else {
			m_dSegment = get1f(_fSegment, 0); //TEST
			m_dEnvelope = get1f(_fEnvelope, 0); //TEST
		}
		*/
		//optimization is calc with floats but need doubles!

		if (m_iSamplesSinceSegmentStart >= m_endPoint->segmentLengthInSamples) {
			if (m_activeSegment + 1 >= m_numSegments) {
				if ((!m_bIsNoteOff) && (myData->hasLoop) && (m_activeSegment == (myData->loopEndPoint - 1))) { //last segment is loop end?
					m_activeSegment = myData->loopStartPoint;

					m_iSamplesSinceSegmentStart = -1;
					if (myData->getSegmentEnd(m_activeSegment)->yVal >= myData->getSegmentStart(m_activeSegment)->yVal) { //rising
						m_dSegment = 0.0f; //0 to 1 or 1 to 0 per segment
						m_bRisingSegment = true;
					}
					else { //falling
						m_dSegment = 1.0f; //0 to 1 or 1 to 0 per segment
						m_bRisingSegment = false;
					}
				}
				else
					if ((!m_bIsNoteOff) && (myData->hasLoop)) {
						//DBG("MSEG: " + String(m_mseg) + " Segment: " + String(m_activeSegment) + " length in Samples: " + String(m_endPoint->segmentLengthInSamples) + " since noteOn: " + String(m_iSamplesSinceNoteOn) + "  ppqPos: " + String(m_Set->m_dPpqPosition) + " bufferSamples: " + String(m_iBufferSamples));
						resynchNoteOn();
						msegWritePointer[currentFrame + frame] = 0.0f;
						continue;
					}
					else
						reset(); //switches note off
			}
			else {
				if ((!m_bIsNoteOff) && (myData->hasLoop) && (m_activeSegment == (myData->loopEndPoint - 1))) { //loop end?
					//DBG("MSEG: " + String(m_mseg) + " Segment: " + String(m_activeSegment) + " length in Samples: " + String(m_endPoint->segmentLengthInSamples) + " since noteOn: " + String(m_iSamplesSinceNoteOn) + "  ppqPos: " + String(m_Set->m_dPpqPosition) + " bufferSamples: " + String(m_iBufferSamples));					
					m_activeSegment = myData->loopStartPoint;
				}
				else if ((!noSustain) && ((!m_bIsNoteOff) && (m_endPoint->isSustain))) { //is sustain?
					if (m_bRisingSegment)
						m_dSegment = 1.0f;
					else
						m_dSegment = 0.0f;
					m_iSamplesSinceSegmentStart = m_endPoint->segmentLengthInSamples - 1;
					msegWritePointer[currentFrame + frame] = m_dEnvelope;
					continue; // dont start new
				}
				else {
					m_activeSegment++; //next segment
				}

				//check for changes
				if (m_activeSegment >= myData->getNumSegments()) {
					resynchNoteOn();
					msegWritePointer[currentFrame + frame] = 0.0f;
					continue;
				}
				m_iSamplesSinceSegmentStart = -1;
				if (myData->getSegmentEnd(m_activeSegment)->yVal >= myData->getSegmentStart(m_activeSegment)->yVal) { //rising
					m_dSegment = 0.0f; //0 to 1 or 1 to 0 per segment
					m_bRisingSegment = true;
				}
				else { //falling
					m_dSegment = 1.0f; //0 to 1 or 1 to 0 per segment
					m_bRisingSegment = false;
				}
			}
		}
	}	
}

/*
float CVASTMSEGEnvelope::getEnvelope(bool noSustain) { //called during processAudio
	if (!isActive()) return 0.0f; 
	if (m_bHardStop) {
		if (m_dEnvelope > 0.00) {
			m_dEnvelope -= float(1.0 / (m_Set->m_nSampleRate * 0.02f)); // 20 ms
		}
		else {
			m_dEnvelope = 0.00f;
			DBG(String(m_voiceNo) + " hardstop mode completed " + String(m_dEnvelope) + " MSEG " + String(m_mseg));
			reset();
		}
		if (m_dEnvelope < 0.f) m_dEnvelope = 0.f;		
		return m_dEnvelope;
	}

	m_iSamplesSinceNoteOn++;
	m_iSamplesSinceSegmentStart++;

	//check for changes
	if (m_iSamplesSinceSegmentStart < 0) {
		resynchNoteOn();
		return 0.f;
	}
	if (m_activeSegment >= myData->getNumSegments()) {
		resynchNoteOn();
		return 0.f;
	}

	// ==== expensive part
	if (m_lastActiveSegment != m_activeSegment) { //buffering
		m_startPoint = myData->getSegmentStart(m_activeSegment);
		vassert(m_startPoint != nullptr);
		m_endPoint = myData->getSegmentEnd(m_activeSegment);
		vassert(m_endPoint != nullptr);
		m_lastActiveSegment = m_activeSegment;
		m_numSegments = myData->getNumSegments();
	}

	myData->calcSegmentCoefficients(m_Set->m_nSampleRate, m_startPlayTimestamp, m_activeSegment, m_iSamplesSinceSegmentStart, m_endPoint->segmentLengthInSamples, m_Set);
	m_dSegment = m_endPoint->offset + m_dSegment * m_endPoint->coeff; //0 to 1 or 1 to 0 per segment
	if (isnan(m_dSegment) || (m_endPoint->segmentLengthInSamples < 0)) { //safety for online step increase
		resynchNoteOn();
		return 0.f;
	}	
	vassert(!isnan(m_dSegment)); 
	vassert(m_endPoint->segmentLengthInSamples >= 0);
	vassert(m_activeSegment >= 0);

	float startYVal = m_startPoint->yVal;
	if (m_bRelease && (m_startPoint->isSustain))
		startYVal = m_dReleaseValue; // release from where we are to avoid clicks - but keep time
	if (m_bRisingSegment) {
		m_dSegment = (m_dSegment > 1.0f) ? 1.0f : m_dSegment; //check overrun 
		m_dEnvelope = startYVal + m_dSegment * (m_endPoint->yVal - startYVal);  //startpoint and release????
	}
	else {
		m_dSegment = (m_dSegment < 0.0f) ? 0.0f : m_dSegment; //check overrun
		m_dEnvelope = startYVal - (1.0f - m_dSegment) * (startYVal - m_endPoint->yVal);
	}

	if (m_iSamplesSinceSegmentStart >= m_endPoint->segmentLengthInSamples) {
		if (m_activeSegment + 1 >= m_numSegments) {
			if ((!m_bIsNoteOff) && (myData->hasLoop) && (m_activeSegment == (myData->loopEndPoint - 1))) { //last segment is loop end?
				m_activeSegment = myData->loopStartPoint;
				m_dSegment = 0.f; // restart segment
				m_iSamplesSinceSegmentStart = 0;
			}
			else
				if ((!m_bIsNoteOff) && (myData->hasLoop)) {
					resynchNoteOn();
					return 0.f;
				}
				else
					reset(); //switches note off
		}
		else {
			if ((!m_bIsNoteOff) && (myData->hasLoop) && (m_activeSegment == (myData->loopEndPoint-1))) { //loop end?
				m_activeSegment = myData->loopStartPoint;
			}
			else if ((!noSustain) && ((!m_bIsNoteOff) && (m_endPoint->isSustain))) { //is sustain?
				if (m_bRisingSegment) 
					m_dSegment = 1.0f;
				else 
					m_dSegment = 0.0f;
				m_iSamplesSinceSegmentStart = m_endPoint->segmentLengthInSamples; 
		
				return m_dEnvelope; // dont start new
			}
			else {
				m_activeSegment++; //next segment
			}

			//check for changes
			if (m_activeSegment >= myData->getNumSegments()) {
				resynchNoteOn();
				return 0.f;
			}
			m_iSamplesSinceSegmentStart = -1;
			if (myData->getSegmentEnd(m_activeSegment)->yVal >= myData->getSegmentStart(m_activeSegment)->yVal) { //rising
				m_dSegment = 0.0f; //0 to 1 or 1 to 0 per segment
				m_bRisingSegment = true;
			}
			else { //falling
				m_dSegment = 1.0f; //0 to 1 or 1 to 0 per segment
				m_bRisingSegment = false;
			}
		}
	}	

	return m_dEnvelope;
}
*/

float CVASTMSEGEnvelope::getEnvelopeStepSeq(int bufferSample) { //called during processAudio
	if (!isActive()) return 0.0f;
	if (m_bHardStop) {
		if (m_dEnvelope > 0.00) {			
			m_dEnvelope -= float(1.0 / (m_Set->m_nSampleRate * 0.02f)); // 20 ms
			DBG(String(m_voiceNo) + " in hardstop mode returning " + String(m_dEnvelope) + " STEPSEQ " + String(m_stepSeq));
		}
		else {
			m_dEnvelope = 0.00f;
			reset();
		}
		if (m_dEnvelope < 0.f) m_dEnvelope = 0.f;
		return m_dEnvelope;
	}

	//const ScopedReadLock myScopedLock(myData->mReadWriteLock);
	m_iSamplesSinceNoteOn++;
	m_iSamplesSinceSegmentStart++;

	//check for changes
	if (m_iSamplesSinceSegmentStart < 0) {
		resynchNoteOn();
		return 0.f;
	}
	if (m_activeSegment >= myData->getNumSegments()) {
		resynchNoteOn();
		return 0.f;
	}

	// ==== expensive part
	if (m_lastActiveSegment != m_activeSegment) { //buffering
		m_startPoint = myData->getSegmentStart(m_activeSegment);
		vassert(m_startPoint != nullptr);
		m_endPoint = myData->getSegmentEnd(m_activeSegment);
		vassert(m_endPoint != nullptr);
		m_lastActiveSegment = m_activeSegment;
		m_numSegments = myData->getNumSegments();
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

	if ((synch == SWITCH::SWITCH_ON) && (m_Set->m_bPpqIsPlaying)) {
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
				m_activeSegment = segment;
				m_iSamplesSinceSegmentStart = shallSamplesSinceBegin - (sumSamples - m_endPoint->segmentLengthInSamples);
				break;
			}
		}

		myData->setUIDisplay(m_activeSegment, m_iSamplesSinceSegmentStart, m_endPoint->segmentLengthInSamples, m_voiceNo, true);
		myDataLive->setUIDisplay(m_activeSegment, m_iSamplesSinceSegmentStart, m_endPoint->segmentLengthInSamples, m_voiceNo, true);

		float x = float(m_iSamplesSinceSegmentStart) / float(m_endPoint->segmentLengthInSamples);

		//y = u0(1−x)^3 + 3u1((1−x)^2)x + 3u2(1−x)x^2 + u3x^3 //cubic bezier
		//y = 3u1((1−x)^2)x + 3u2(1−x)x^2 + x^3 //u0 = 0, u1 = 1, u2 = u3 = curvy!
		
		if (myData->getSegmentEnd(m_activeSegment)->yVal >= myData->getSegmentStart(m_activeSegment)->yVal) { //rising
			m_bRisingSegment = true;
		}
		else { //falling
			m_bRisingSegment = false;
		}
	
		double curve = m_endPoint->curvy;

		if (!m_bRisingSegment) 
			curve = 1.f - curve;
		m_bRisingSegment = true;

		//m_dSegment = 3.f * curve * powf(1.f - x, 2.f) * x +
		//			3.f * curve * (1.f - x) * x * x +
		//	x * x * x;
		m_dSegment = cubicBezierApproximation(x, curve); //check performance
				
		if (isnan(m_dSegment) || (m_endPoint->segmentLengthInSamples < 0)) { //safety for online step increase
			resynchNoteOn();
			return 0.f;
		}
		vassert(!isnan(m_dSegment));
		vassert(m_endPoint->segmentLengthInSamples >= 0);
	}
	else {

		myData->calcSegmentCoefficients(m_Set->m_nSampleRate, m_startPlayTimestamp, m_activeSegment, m_iSamplesSinceSegmentStart, m_endPoint->segmentLengthInSamples, m_Set, m_voiceNo);
		myData->setUIDisplay(m_activeSegment, m_iSamplesSinceSegmentStart, m_endPoint->segmentLengthInSamples, m_voiceNo, true); //CHECK
		myDataLive->setUIDisplay(m_activeSegment, m_iSamplesSinceSegmentStart, m_endPoint->segmentLengthInSamples, m_voiceNo, true); //CHECK

		m_dSegment = m_endPoint->offset + m_dSegment * m_endPoint->coeff; //0 to 1 or 1 to 0 per segment
		if (isnan(m_dSegment) || (m_endPoint->segmentLengthInSamples < 0)) { //safety for online step increase
			resynchNoteOn();
			return 0.f;
		}
		vassert(!isnan(m_dSegment));
		vassert(m_endPoint->segmentLengthInSamples >= 0);
	}

	float startYVal = m_startPoint->yVal;
	if (m_bRelease && (m_startPoint->isSustain))
		startYVal = m_dReleaseValue; // release from where we are to avoid clicks - but keep time
	if (m_bRisingSegment) {
		m_dSegment = (m_dSegment > 1.0f) ? 1.0f : m_dSegment; //check overrun 
		m_dEnvelope = startYVal + m_dSegment * (m_endPoint->yVal - startYVal);  //startpoint and release????
	}
	else {
		m_dSegment = (m_dSegment < 0.0f) ? 0.0f : m_dSegment; //check overrun
		m_dEnvelope = startYVal - (1.0f - m_dSegment) * (startYVal - m_endPoint->yVal);
	}

	if (m_iSamplesSinceSegmentStart >= m_endPoint->segmentLengthInSamples) {
		if (m_activeSegment + 1 >= m_numSegments) {
			if ((!m_bIsNoteOff) && (myData->hasLoop) && (m_activeSegment == (myData->loopEndPoint - 1))) { //last segment is loop end?
				m_activeSegment = myData->loopStartPoint;
				m_iSamplesSinceSegmentStart = -1;
				if (myData->getSegmentEnd(m_activeSegment)->yVal >= myData->getSegmentStart(m_activeSegment)->yVal) { //rising
					m_dSegment = 0.0f; //0 to 1 or 1 to 0 per segment
					m_bRisingSegment = true;
				}
				else { //falling
					m_dSegment = 1.0f; //0 to 1 or 1 to 0 per segment
					m_bRisingSegment = false;
				}
			}
			else
				if ((!m_bIsNoteOff) && (myData->hasLoop)) {
					resynchNoteOn();
					return 0.f;
				}
				else
					reset(); //switches note off
		}
		else {
			if ((!m_bIsNoteOff) && (myData->hasLoop) && (m_activeSegment == (myData->loopEndPoint - 1))) { //loop end?
				m_activeSegment = myData->loopStartPoint;
			}
			else if ((!m_bIsNoteOff) && (m_endPoint->isSustain)) { //is sustain?
				if (m_bRisingSegment)
					m_dSegment = 1.0f;
				else
					m_dSegment = 0.0f;
				m_iSamplesSinceSegmentStart = m_endPoint->segmentLengthInSamples - 1;

				return m_dEnvelope; // dont start new
			}
			else {
				m_activeSegment++; //next segment
			}

			//check for changes
			if (m_activeSegment >= myData->getNumSegments()) {
				resynchNoteOn();
				return 0.f;
			}

			m_iSamplesSinceSegmentStart = -1;
			if (myData->getSegmentEnd(m_activeSegment)->yVal >= myData->getSegmentStart(m_activeSegment)->yVal) { //rising
				m_dSegment = 0.0f; //0 to 1 or 1 to 0 per segment
				m_bRisingSegment = true;
			}
			else { //falling
				m_dSegment = 1.0f; //0 to 1 or 1 to 0 per segment
				m_bRisingSegment = false;
			}
		}
	}

	return m_dEnvelope;
}
