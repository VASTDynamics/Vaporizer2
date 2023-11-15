/*
VAST Dynamics

How to calculate quadratic beziers
x from 0 to 1: 
y = (1-x)^2  * startval  + 2x(1-x) * intermediateval + x^2 * endval

http://slideplayer.com/slide/6554987/

*/

#include "VASTMSEGData.h"
#include "VASTSettings.h"

//#include "../Engine/VASTEngineHeader.h"

VASTMSEGData::VASTMSEGData() {
	initToADSR(0); //CHECK
}

VASTMSEGData::VASTMSEGData(const VASTMSEGData &copyData) { //copy constructor
	copyDataFrom(copyData);
}

void VASTMSEGData::init() {	
    m_isDirty = true;
    m_needsUIUpdate = true;
    m_needsPositionUpdate = true;
    m_bADSR_updated = true;
    m_bStepSeq_updated = true;
	controlPoints.clear();

	memset(m_dispActiveSegment, 0, sizeof(int) * C_MAX_POLY);
	memset(m_dispSamplesSinceSegmentStart, 0, sizeof(int) * C_MAX_POLY);
	memset(m_dispSegmentLengthInSamples, 0, sizeof(int) * C_MAX_POLY);
	memset(m_dispVoicePlaying, false, sizeof(bool) * C_MAX_POLY);
}

void VASTMSEGData::copyDataFrom(const VASTMSEGData &copyData) {
	controlPoints = copyData.controlPoints;
	m_fAttackTime.store(copyData.m_fAttackTime.load());
    m_fDecayTime.store(copyData.m_fDecayTime.load());
    m_fSustainLevel.store(copyData.m_fSustainLevel.load());
    m_fReleaseTime.store(copyData.m_fReleaseTime.load());
    m_fAttackTimeExternalSet.store(copyData.m_fAttackTimeExternalSet.load());
    m_fDecayTimeExternalSet.store(copyData.m_fDecayTimeExternalSet.load());
    m_fReleaseTimeExternalSet.store(copyData.m_fReleaseTimeExternalSet.load());
    m_fSustainLevelExternalSet.store(copyData.m_fSustainLevelExternalSet.load());
	hasLoop = copyData.hasLoop;
	loopStartPoint.store(copyData.loopStartPoint);
	loopEndPoint.store(copyData.loopEndPoint);
	m_bSynch = copyData.m_bSynch;
	m_uTimeBeats.store(copyData.m_uTimeBeats.load());
	m_fAttackSteps.store( copyData.m_fAttackSteps.load());
	m_fDecaySteps.store(copyData.m_fDecaySteps.load());
	m_fReleaseSteps.store(copyData.m_fReleaseSteps.load());

	memcpy(m_dispActiveSegment, copyData.m_dispActiveSegment, C_MAX_POLY * sizeof(int));
	memcpy(m_dispSamplesSinceSegmentStart, copyData.m_dispSamplesSinceSegmentStart, C_MAX_POLY * sizeof(int));
	memcpy(m_dispSegmentLengthInSamples, copyData.m_dispSegmentLengthInSamples, C_MAX_POLY * sizeof(int));
	memcpy(m_dispVoicePlaying, copyData.m_dispVoicePlaying, C_MAX_POLY * sizeof(bool));

	env_mode = copyData.env_mode;
	patternName = copyData.patternName;
	invert = copyData.invert;
	m_ss_bars = copyData.m_ss_bars;
	m_ss_bars_num = copyData.m_ss_bars_num;
	m_ss_gate = copyData.m_ss_gate;
	m_ss_glide = copyData.m_ss_glide;
	m_iSampleRate = copyData.m_iSampleRate;
	m_isStepSeqData = copyData.m_isStepSeqData;
	m_stepSeqNo = copyData.m_stepSeqNo;
	m_msegNo = copyData.m_msegNo;
	m_fOrigStepSeqTime = copyData.m_fOrigStepSeqTime;

	m_bADSR_updated = true;
    m_bStepSeq_updated = true;
    m_isDirty = true;
    m_needsUIUpdate = true;
}

void VASTMSEGData::initToSine(int msegNo) {
	init();
	m_msegNo = msegNo;
	controlPoints.clear();
	patternName = "Sine";

	//setEnvMode(unipolar);
	ControlPoint point1;
	point1.xVal = 0.0f;
	point1.yVal = 0.5f;
	point1.isLoopStart = true;
	addPoint(point1);

	ControlPoint point2;
	point2.xVal = 0.25f;
	point2.yVal = 1.0f;
	point2.curvy = 0.75f;
	addPoint(point2);

	ControlPoint point3;
	point3.xVal = 0.5f;
	point3.yVal = 0.5f;
	point3.curvy = 0.75f;
	addPoint(point3);

	ControlPoint point4;
	point4.xVal = 0.75f;
	point4.yVal = 0.0f;
	point4.curvy = 0.25f;
	addPoint(point4);

	ControlPoint point5;
	point5.xVal = 1.00f;
	point5.yVal = 0.5f;
	point5.curvy = 0.25f;
	point5.isSustain = true;
	addPoint(point5);
	calcADSR();
}

void VASTMSEGData::initToRamp(int msegNo) {
	init();
	m_msegNo = msegNo;
	controlPoints.clear();
	patternName = "Ramp";

	//setEnvMode(unipolar);
	ControlPoint point1;
	point1.xVal = 0.0f;
	point1.yVal = 0.0f;
	point1.isLoopStart = false;
	addPoint(point1);

	ControlPoint point2;
	point2.xVal = 1.00f;
	point2.yVal = 1.0f;
	point2.curvy = 0.5f;
	point2.isDecay = true;
	point2.isSustain = false;	
	addPoint(point2);

	ControlPoint point3;
	point3.xVal = 1.00f;
	point3.yVal = 1.0f;
	point3.curvy = 0.5f;
	point3.isDecay = false;
	point3.isSustain = true;
	addPoint(point3);

	ControlPoint point4;
	point4.xVal = 1.00f;
	point4.yVal = 1.0f;
	point4.curvy = 0.5f;
	point4.isSustain = false;
	addPoint(point4);

	calcADSR();
}

void VASTMSEGData::initToStairs(int msegNo) {
	init();
	m_msegNo = msegNo;
	controlPoints.clear();
	patternName = "Stairs";

	//setEnvMode(unipolar);
	ControlPoint point1;
	point1.xVal = 0.0f;
	point1.yVal = 0.0f;
	point1.isLoopStart = true;
	addPoint(point1);

	ControlPoint point2;
	point2.xVal = 0.0f;
	point2.yVal = 0.25f;
	point2.curvy = 0.5f;
	addPoint(point2);

	ControlPoint point3;
	point3.xVal = 0.25f;
	point3.yVal = 0.25f;
	point3.curvy = 0.5f;
	addPoint(point3);

	ControlPoint point4;
	point4.xVal = 0.25f;
	point4.yVal = 0.5f;
	point4.curvy = 0.5f;
	addPoint(point4);

	ControlPoint point5;
	point5.xVal = 0.5f;
	point5.yVal = 0.5f;
	point5.curvy = 0.5f;
	addPoint(point5);

	ControlPoint point6;
	point6.xVal = 0.5f;
	point6.yVal = 0.75f;
	point6.curvy = 0.5f;
	addPoint(point6);

	ControlPoint point7;
	point7.xVal = 0.75f;
	point7.yVal = 0.75f;
	point7.curvy = 0.5f;
	addPoint(point7);

	ControlPoint point8;
	point8.xVal = 0.75f;
	point8.yVal = 1.f;
	point8.curvy = 0.5f;
	addPoint(point8);

	ControlPoint point9;
	point9.xVal = 1.f;
	point9.yVal = 1.f;
	point9.curvy = 0.5f;
	point9.isSustain = true;
	addPoint(point9);

	calcADSR();
}

void VASTMSEGData::initToADR(int msegNo) {
	init();
	m_msegNo = msegNo;
	controlPoints.clear();
	patternName = "ADR";

	//setEnvMode(unipolar);
	ControlPoint point1;
	point1.xVal = 0.0f;
	point1.yVal = 0.0f;
	addPoint(point1);

	ControlPoint point2;
	point2.xVal = 0.009009009f;
	point2.yVal = 1.0f;
	point2.curvy = 0.8f;
	point2.isDecay = true;
	addPoint(point2);

	ControlPoint point3;
	point3.xVal = 0.099099099f;
	point3.yVal = 1.0f;
	point3.curvy = 0.2f;
	point3.isSustain = false;
	addPoint(point3);

	ControlPoint point4;
	point4.xVal = 1.0f;
	point4.yVal = 0.0f;
	point4.curvy = 0.2f;
	addPoint(point4);
	calcADSR();
}

void VASTMSEGData::initToADSR(int msegNo) {
	init();
	m_msegNo = msegNo;
	controlPoints.clear();
	patternName = "ADSR";

	//setEnvMode(unipolar);
	ControlPoint point1;
	point1.xVal = 0.0f;
	point1.yVal = 0.0f;
	addPoint(point1);

	ControlPoint point2;
	//point2.xVal = 0.0f;
	point2.xVal = 0.009009009;
	point2.yVal = 1.0f;
	point2.curvy = 0.5f;
	//point2.curvy = 0.8f;
	point2.isDecay = true;
	addPoint(point2);

	ControlPoint point3;
	point3.xVal = 0.099099099f;
	point3.yVal = 1.0f;
	point3.curvy = 0.2f;
	point3.isSustain = true;
	addPoint(point3);

	ControlPoint point4;
	point4.xVal = 1.0f;
	point4.yVal = 0.0f;
	point4.curvy = 0.2f;
	addPoint(point4);
	calcADSR();
}

void VASTMSEGData::initToAHDSR(int msegNo) {
	init();
	m_msegNo = msegNo;
	controlPoints.clear();
	patternName = "AHDSR";

	//setEnvMode(unipolar);
	ControlPoint point1;
	point1.xVal = 0.0f;
	point1.yVal = 0.0f;
	addPoint(point1);

	ControlPoint point2;
	point2.xVal = 0.1;
	point2.yVal = 1.0f;
	point2.curvy = 0.6f;
	point2.isDecay = true;
	addPoint(point2);

	ControlPoint point2a;
	point2a.xVal = 0.2f;
	point2a.yVal = 1.0f;
	point2a.curvy = 0.5f;
	addPoint(point2a);

	ControlPoint point2b;
	point2b.xVal = 0.3f;
	point2b.yVal = 0.6f;
	point2b.curvy = 0.4f;
	addPoint(point2b);

	ControlPoint point3;
	point3.xVal = 0.9f;
	point3.yVal = 0.6f;
	point3.curvy = 0.5f;
	point3.isSustain = true;
	addPoint(point3);

	ControlPoint point4;
	point4.xVal = 1.0f;
	point4.yVal = 0.0f;
	point4.curvy = 0.2f;
	addPoint(point4);
	calcADSR();
}


void VASTMSEGData::calcSegmentCoefficients(int samplerate, ULong64_t, int activeSegment, int samplesSinceSegmentStart, int segmentLengthInSamples, CVASTSettings*, int voiceNo) {
	if (activeSegment >= getNumSegments()) {
		activeSegment = 0;
		m_isDirty = true;
	}
			   
    if (!((m_isDirty) || (samplerate != m_iSampleRate))) return;

	m_iSampleRate = samplerate; //samplerate changed?

	for (int segment = 0; segment < controlPoints.size() - 1; segment++) {
		VASTMSEGData::ControlPoint* startPoint = getSegmentStart(segment);
		VASTMSEGData::ControlPoint* endPoint = getSegmentEnd(segment);

		//float lMinSegmenDurationMs = 0.5f; //minimum segment 3ms? attack shall be 1ms filter 45ms?
		//float lMinSegmenDurationSamples = m_iSampleRate * (lMinSegmenDurationMs / 1000.0f);
        int totalDurationInSamples = int(calcTotalDuration() / 1000.f * samplerate);
		endPoint->segmentLengthInSamples = int((endPoint->xVal - startPoint->xVal) * totalDurationInSamples);
		//endPoint->segmentLengthInSamples = (endPoint->segmentLengthInSamples < lMinSegmenDurationSamples) ? lMinSegmenDurationSamples : endPoint->segmentLengthInSamples;

		vassert(endPoint->segmentLengthInSamples >= 0);
		endPoint->coeff = 0.0f;
		endPoint->offset = 0.0f;
		if (endPoint->segmentLengthInSamples != 0.0) { //else just 0
			if (controlPoints[segment + 1].curvy >= 0.5) { //above05
				double tco = exp(-(controlPoints[segment + 1].curvy - 0.5) * 40.0 + 5.0);
				if (endPoint->yVal > startPoint->yVal) { //rising
					endPoint->coeff = exp(-log((1.0 + tco) / tco) / endPoint->segmentLengthInSamples);
					endPoint->offset = (1.0 + tco) * (1.0 - endPoint->coeff);
				}
				else { //falling
					endPoint->coeff = 1.0 / (exp(-log((1.0 + tco) / tco) / endPoint->segmentLengthInSamples));
					endPoint->offset = (1.0 + tco) * (1.0 - endPoint->coeff);
				}
			}
			else { //below05
				double tco = exp(-(1.0 - controlPoints[segment + 1].curvy - 0.5) * 40.0 + 5.0);
				if (endPoint->yVal > startPoint->yVal) { //rising
					endPoint->coeff = 1.0 / (exp(-log((1.0 + tco) / tco) / endPoint->segmentLengthInSamples));
					vassert(endPoint->coeff != 0.0);
					endPoint->offset = (tco * endPoint->coeff) * (1.0 - 1.0 / endPoint->coeff);
				}
				else { //falling
					endPoint->coeff = exp(-log((1.0 + tco) / tco) / endPoint->segmentLengthInSamples);
					endPoint->offset = (-tco) * (1.0 - endPoint->coeff);
				}
			}
		}
	}

	m_isDirty = false;
	m_needsUIUpdate = true;
}

double VASTMSEGData::calcTotalDuration() {
	double dur = m_fDecayTimeExternalSet.load();
	if (hasAttackPhase()) dur += m_fAttackTimeExternalSet.load();
	else {
		m_fAttackTimeExternalSet.store(0.0);
        m_fAttackTime.store(0.0);
	}
	if (hasReleasePhase()) dur += m_fReleaseTimeExternalSet.load();
	else {
		m_fReleaseTimeExternalSet.store(0.0);
        m_fReleaseTime.store(0.0);
	}
	return dur;
}

void VASTMSEGData::addPoint(ControlPoint point) { //needs calcADSR() afterwards
	m_isDirty = true;
	m_needsUIUpdate = true;

	controlPoints.push_back(point);
	checkLoop();	
}

void VASTMSEGData::insertPoint(int newpos, ControlPoint point) { //needs calcADSR() afterwards
    m_isDirty = true;
	m_needsUIUpdate = true;

	controlPoints.insert(controlPoints.begin() + newpos, point);
	checkLoop();
}

void VASTMSEGData::removePoint(int delpos) {
	if (controlPoints.size() > 3) {//keep last three points to have decay and release
        m_isDirty = true;
        m_needsUIUpdate = true;

		controlPoints.erase(controlPoints.begin() + delpos);
		checkLoop();
		calcADSR();
	}
}

void VASTMSEGData::insertPointUI(int newpos, VASTMSEGData::ControlPoint point) {
	//const ScopedWriteLock myScopedLock(mReadWriteLock);
	insertPoint(newpos, point);
	calcADSR();
}

void VASTMSEGData::removePointUI(int delpos) {
	//const ScopedWriteLock myScopedLock(mReadWriteLock);
	removePoint(delpos);
}

void VASTMSEGData::setUIDisplay(int dispActiveSegment, int dispSamplesSinceSegmentStart, int dispSegmentLengthInSamples, int voiceNo, bool isPlaying) {
	if (dispSegmentLengthInSamples < 0)
		return;
	if ((m_dispActiveSegment[voiceNo].load() != dispActiveSegment) ||
		(m_dispSamplesSinceSegmentStart[voiceNo].load() != dispSamplesSinceSegmentStart) ||
		(m_dispSegmentLengthInSamples[voiceNo].load() != dispSegmentLengthInSamples) ||
		(m_dispVoicePlaying[voiceNo].load() != isPlaying)) {
		m_needsPositionUpdate = true;
	}
	m_dispVoicePlaying[voiceNo].store(isPlaying);
	m_dispActiveSegment[voiceNo].store(dispActiveSegment);
	m_dispSamplesSinceSegmentStart[voiceNo].store(jlimit<int>(0, dispSegmentLengthInSamples, dispSamplesSinceSegmentStart));
	m_dispSegmentLengthInSamples[voiceNo].store(dispSegmentLengthInSamples);
}

bool VASTMSEGData::getAndClearPositionUpdateFlag() {
	bool lUpdate = m_needsPositionUpdate;
	m_needsPositionUpdate = false;
	return lUpdate;
}

bool VASTMSEGData::getAndClearUIUpdateFlag() {
	bool lUpdate = m_needsUIUpdate;
	m_needsUIUpdate = false;
	return lUpdate;
}

//ReadWriteLock mReadWriteLock; //one lock per instance of class

void VASTMSEGData::setDirty() {
	m_isDirty = true;
}

void VASTMSEGData::clearDirtyFlag() {
    m_isDirty = false;
}

bool VASTMSEGData::isMSEGDirty() const {
	return m_isDirty;
}

bool VASTMSEGData::getADSRUpdated() const {
	return m_bADSR_updated;
}

void VASTMSEGData::resetADSRUpdated() {
	m_bADSR_updated = false;
}

int VASTMSEGData::getDispActiveSegment(int voiceNo) const {
	return m_dispActiveSegment[voiceNo].load();
}

int VASTMSEGData::getDispSamplesSinceSegmentStart(int voiceNo) const {
	return m_dispSamplesSinceSegmentStart[voiceNo].load();
}

int VASTMSEGData::getDispSegmentLengthInSamples(int voiceNo) const {
	return m_dispSegmentLengthInSamples[voiceNo].load();
}

bool VASTMSEGData::dispVoicePlaying(int voiceNo) {
	return m_dispVoicePlaying[voiceNo].load();
}

void VASTMSEGData::setEnvMode(int mode) {
	m_isDirty = true;
    m_needsUIUpdate = true;

	env_mode = mode;
}

int VASTMSEGData::getDecayPoint() const {
	int decayPoint = -1;
	for (int i = 0; i < controlPoints.size(); i++) {
		if (controlPoints[i].isDecay)
			decayPoint = i;
	}

	return decayPoint;
}

void VASTMSEGData::calcADSR() {
	if (controlPoints.size() < 3) return;

	//scale
	jassert(controlPoints.size() > 0);
	double firstx = controlPoints[0].xVal;
	if (firstx >= 0.0)
		for (int i = 0; i < controlPoints.size(); i++)
			controlPoints[i].xVal -= firstx; //bring first point to 0.0;
	double lastx = controlPoints[controlPoints.size() - 1].xVal;
	jassert(lastx > 0.0);
	if (lastx < 1.0)
		for (int i = 0; i < controlPoints.size(); i++)
			controlPoints[i].xVal /= lastx; //bring last point to 1.0;

	double totalBefore = calcTotalDuration();

	int susPoint = getSustainPoint();	
	if (susPoint != -1) {
		float newSustainLevel = float(controlPoints[susPoint].yVal);
		if (newSustainLevel != m_fSustainLevel) {
			m_fSustainLevel = newSustainLevel;			
			m_fSustainLevelExternalSet.store(m_fSustainLevel.load());
			m_bADSR_updated = true;
		}
	}

	int decayPoint = -1;
	for (int i = 0; i < controlPoints.size(); i++) {
		if (controlPoints[i].isDecay)
			decayPoint = i;
	}
	if (decayPoint == -1)
		//decayPoint = 1; //check min 2 points, second point is 1
		decayPoint = 0; //check min 2 points, second point is 1

	if (susPoint == -1) {
		//susPoint = controlPoints.size() - 2; // prelast point
		susPoint = int(controlPoints.size()) - 1; // last point
	}

	double attackPerc = 0.0f;
	double decayPerc = 0.0f;
	double releasePerc = 0.0f;
	double max = controlPoints[controlPoints.size() - 1].xVal - controlPoints[0].xVal;
	for (int i = 1; i < controlPoints.size(); i++) {
		if (i <= decayPoint) {
			attackPerc += (controlPoints[i].xVal - controlPoints[i - 1].xVal) / max;
		} else 
			if (i > susPoint) {
				releasePerc += (controlPoints[i].xVal - controlPoints[i - 1].xVal) / max;
			}
			else { //decay
				decayPerc += (controlPoints[i].xVal - controlPoints[i - 1].xVal) / max;
			}
	}

	double newAttackTime = totalBefore * attackPerc;
	jassert(abs((attackPerc + decayPerc + releasePerc) - 1.0) < 0.01);
	if (newAttackTime != m_fAttackTime.load()) {
		m_fAttackTime.store(newAttackTime);
		m_fAttackTimeExternalSet.store(m_fAttackTime);
		m_bADSR_updated  =true;
	}

	double newDecayTime = totalBefore * decayPerc;
	if (newDecayTime != m_fDecayTime.load()) {
		m_fDecayTime.store(newDecayTime);
		m_fDecayTimeExternalSet.store(m_fDecayTime.load());
        m_bADSR_updated = true;
	}

	double newReleaseTime = totalBefore * releasePerc;
	if (newReleaseTime != m_fReleaseTime.load()) {
		m_fReleaseTime.store(newReleaseTime);
		m_fReleaseTimeExternalSet.store(m_fReleaseTime.load());
        m_bADSR_updated = true;
	}

#if JUCE_DEBUG
	double totalAfter = calcTotalDuration(); //needed
	jassert(abs(totalAfter - totalBefore) < 0.001);
#else
    calcTotalDuration(); 
#endif
}

bool VASTMSEGData::hasReleasePhase() const {
	bool hasRelease = true;
	int sp = getSustainPoint();
	if ((sp == -1) || (controlPoints.size() <= sp + 1))
		hasRelease = false;
	return hasRelease;
}

void VASTMSEGData::setSynch(bool synch) {
    m_bSynch = synch;
    m_isDirty = true;
}

bool VASTMSEGData::getSynch() const {
	return m_bSynch;
}

void VASTMSEGData::setTimeBeats(int timeBeats) {
	m_uTimeBeats = timeBeats;
    m_isDirty = true;
}

int VASTMSEGData::getTimeBeats() const {
	return m_uTimeBeats;
}


// design time interface
//float getAttackLevel() { return 0.f; };

double VASTMSEGData::getAttackTime() const {
	return m_fAttackTime.load();
}

//ms

double VASTMSEGData::getDecayTime() const {
	return m_fDecayTime.load();
}

//ms

double VASTMSEGData::getSustainLevel() const {
	return m_fSustainLevel.load();
}

/** < 0.0 to 1.0. */

double VASTMSEGData::getReleaseTime() const {
	return m_fReleaseTime.load();
}

bool VASTMSEGData::hasAttackPhase() const {
	bool hasAttack = true;
	int decayPoint = -1;
	for (int i = 0; i < controlPoints.size(); i++) {
		if (controlPoints[i].isDecay)
			decayPoint = i;
	}
	if ((decayPoint == -1) || (decayPoint == 0))
		hasAttack = false;
	return hasAttack;
}

void VASTMSEGData::doADSR() {
	//const ScopedReadLock myScopedLock(mReadWriteLock); //not needed - not chaninging structure

	if (controlPoints.size() < 3) return;

	//m_isDirty = true;

	int susPoint = getSustainPoint();
	int decayPoint = -1;
	for (int i = 0; i < controlPoints.size(); i++) {
		if (controlPoints[i].isDecay)
			decayPoint = i;
	}
	if (decayPoint == -1)
		//decayPoint = 1;  //check min 2 points, second point is 1
		decayPoint = 0;  //check min 2 points, second point is 1

	if (susPoint == -1) {
		//susPoint = controlPoints.size() - 2; // prelast point
		susPoint = int(controlPoints.size()) - 1; // last point
	}

	double attackPerc = 0.0f;
	double decayPerc = 0.0f;
	double releasePerc = 0.0f;
	double max = controlPoints[controlPoints.size() - 1].xVal - controlPoints[0].xVal;
	bool attackPhase = false;
	bool releasePhase = false;
	//bool decayPhase = false;
	vassert(max > 0.0f);
	for (int i = 1; i < controlPoints.size(); i++) {
		if (i <= decayPoint) {
			attackPerc += (controlPoints[i].xVal - controlPoints[i - 1].xVal) / max;
			vassert(attackPerc >= 0.f);
			attackPhase = true;
		}
		else
			if (i > susPoint) {
				releasePerc += (controlPoints[i].xVal - controlPoints[i - 1].xVal) / max;
				vassert(releasePerc >= 0.f);
				releasePhase = true;
			}
			else { //decay
				decayPerc += (controlPoints[i].xVal - controlPoints[i - 1].xVal) / max;
				vassert(decayPerc >= 0.f);
			}
	}
	vassert((attackPerc + releasePerc + decayPerc - 1.f) < 0.1f);

	if (calcTotalDuration() == 0.f) return;

	//double newAttackPerc = ((m_fAttackTimeExternalSet + 0.000001) / getTotalDuration());
	//double newDecayPerc = ((m_fDecayTimeExternalSet + 0.000001) / getTotalDuration());
	//double newReleasePerc = ((m_fReleaseTimeExternalSet + 0.000001) / getTotalDuration());
	double newAttackPerc = ((m_fAttackTimeExternalSet.load() + 0.0000000001) / calcTotalDuration());
	double newDecayPerc = ((m_fDecayTimeExternalSet.load() + 0.0000000001) / calcTotalDuration());
	double newReleasePerc = ((m_fReleaseTimeExternalSet.load() + 0.0000000001) / calcTotalDuration());

	m_fAttackTime.store(m_fAttackTimeExternalSet.load()); //CHECK
	m_fDecayTime.store(m_fDecayTimeExternalSet.load());//CHECK
	m_fReleaseTime.store(m_fReleaseTimeExternalSet.load()); //CHECK

	double quotaDecay = (decayPerc == 0.0) ? 0.0000000001 : ((decayPerc >= 0.0) ? newDecayPerc / decayPerc : 0.0);

	double quotaAttack = 0.0;
	if (attackPhase)
		quotaAttack = (attackPerc == 0.0) ? 0.0000000001 : ((attackPerc > 0.0) ? newAttackPerc / attackPerc : 0.0);
	else
		newAttackPerc = 0.0;
	double quotaRelease = 0.0;
	if (releasePhase)
		quotaRelease = (releasePerc == 0.0) ? 0.0000000001 : ((releasePerc >= 0.0) ? newReleasePerc / releasePerc : 0.0);
	else
		newReleasePerc = 0.0;
	vassert((newAttackPerc + newDecayPerc + newReleasePerc - 1.f) < 0.01f);

	if (controlPoints.size() > 990) return; //error
	float controlPointsxValSave[999] = { 0.f };
	for (int i = 0; i < controlPoints.size(); i++) {
		controlPointsxValSave[i] = float(controlPoints[i].xVal);
	}

	for (int i = 1; i < controlPoints.size(); i++) {
		if (i <= decayPoint) {
			double dur = controlPointsxValSave[i] - controlPointsxValSave[i - 1];
			dur *= quotaAttack;
			//if (dur <= 0.000001) dur = 0.000001; //safety
			if (dur <= 0.0000000001) dur = 0.0000000001; //safety
			controlPoints[i].xVal = controlPoints[i - 1].xVal + dur;
			vassert(!isnan(controlPoints[i].xVal)); //nand check
		}
		else {
			if (i > susPoint) {
				double dur = controlPointsxValSave[i] - controlPointsxValSave[i - 1];
				dur *= quotaRelease;
				//if (dur <= 0.000001) dur = 0.000001; //safety
				if (dur <= 0.0000000001) dur = 0.0000000001; //safety
				controlPoints[i].xVal = controlPoints[i - 1].xVal + dur;

				vassert(!isnan(controlPoints[i].xVal)); //nand check
				vassert(controlPoints[i].xVal > 0.f);
			}
			else { //decay
				double dur = controlPointsxValSave[i] - controlPointsxValSave[i - 1];
				dur *= quotaDecay;
				//if (dur <= 0.000001) dur = 0.000001; //safety
				if (dur <= 0.0000000001) dur = 0.0000000001; //safety
				controlPoints[i].xVal = controlPoints[i - 1].xVal + dur;

				vassert(!isnan(controlPoints[i].xVal)); //nand check
			}
		}

		if (controlPoints[i].xVal > 1.f) controlPoints[i].xVal = 1.f;
		if (controlPoints[i].xVal < 0.f) controlPoints[i].xVal = 0.f;

		//scale
		jassert(controlPoints.size() > 0);
		double firstx = controlPoints[0].xVal;
		if (firstx > 0.0)
			for (int j = 0; j < controlPoints.size(); j++)
				controlPoints[j].xVal -= firstx; //bring first point to 0.0;
		double lastx = controlPoints[controlPoints.size() - 1].xVal;
		jassert(lastx > 0.0);
		if (lastx < 1.0)
			for (int j = 0; j < controlPoints.size(); j++)
				controlPoints[j].xVal /= lastx; //bring last point to 1.0;
	}

	//adjust to percentages
	int lastAttackPoint = 0;
	if (attackPhase) {
		lastAttackPoint = getDecayPoint();
		for (int i = 0; i <= lastAttackPoint; i++) {
			controlPoints[i].xVal = (controlPoints[i].xVal / controlPoints[lastAttackPoint].xVal) * newAttackPerc;
			if (controlPoints[i].xVal > 1.f) controlPoints[i].xVal = 1.f;
			if (controlPoints[i].xVal < 0.f) controlPoints[i].xVal = 0.f;
		}
	}
	int lastDecayPoint = getSustainPoint();
	if (!releasePhase) 
		lastDecayPoint = int(controlPoints.size()) - 1;
	for (int i = lastAttackPoint + 1; i <= lastDecayPoint; i++) {
		if (controlPoints[lastDecayPoint].xVal - controlPoints[lastAttackPoint].xVal)
			controlPoints[i].xVal = newAttackPerc + ((controlPoints[i].xVal - controlPoints[lastAttackPoint].xVal) / (controlPoints[lastDecayPoint].xVal - controlPoints[lastAttackPoint].xVal)) * newDecayPerc;
		if (controlPoints[i].xVal > 1.f) controlPoints[i].xVal = 1.f;
		if (controlPoints[i].xVal < 0.f) controlPoints[i].xVal = 0.f;
	}
	if (releasePhase) {
		int lastReleasePoint = int(controlPoints.size()) - 1;
		for (int i = lastDecayPoint + 1; i <= lastReleasePoint; i++) {
			if (controlPoints[lastReleasePoint].xVal != controlPoints[lastDecayPoint].xVal)
				controlPoints[i].xVal = newAttackPerc + newDecayPerc + ((controlPoints[i].xVal - controlPoints[lastDecayPoint].xVal) / (controlPoints[lastReleasePoint].xVal - controlPoints[lastDecayPoint].xVal)) * newReleasePerc;
			if (controlPoints[i].xVal > 1.f) controlPoints[i].xVal = 1.f;
			if (controlPoints[i].xVal < 0.f) controlPoints[i].xVal = 0.f;
		}
	}
	//safety ascending
	for (int i = 0; i < controlPoints.size() - 1; i++)
		if (controlPoints[i + 1].xVal < controlPoints[i].xVal)
			controlPoints[i + 1].xVal = controlPoints[i].xVal;

	//scale
	jassert(controlPoints.size() > 0);
	double firstx = controlPoints[0].xVal;
	if (firstx > 0.0)
		for (int i = 0; i < controlPoints.size(); i++)
			controlPoints[i].xVal -= firstx; //bring first point to 0.0;
	double lastx = controlPoints[controlPoints.size() - 1].xVal;
	jassert(lastx > 0.0);
	if (lastx != 1.0)
		for (int i = 0; i < controlPoints.size(); i++)
			controlPoints[i].xVal /= lastx; //bring last point to 1.0;	
	
#ifdef _DEBUG
	vassert(validate());
#endif
}

float VASTMSEGData::getStepSeqBar(int step) {
	float barheight = m_ss_bars[step];
	return barheight;
}

void VASTMSEGData::stepSeqChangeBar(int step, float barheight) {
	//const ScopedWriteLock myScopedLock(mReadWriteLock);

	if (barheight > 1.0f) barheight = 1.0f;
	if (barheight < 0.0f) barheight = 0.0f;

	m_ss_bars[step] = barheight;
	doStepSeq(m_ss_glide, m_ss_gate);

	/*
	controlPoints[step * 3].yVal = 0.f; //CHECK
	controlPoints[step * 3 + 1].yVal = barheight;
	controlPoints[step * 3 + 2].yVal = barheight;
	*/

    m_isDirty = true;
    m_needsUIUpdate = true;
}

void VASTMSEGData::stepSeqChangeGate(float gate) {
	//const ScopedWriteLock myScopedLock(mReadWriteLock);
	m_ss_gate = gate;

	doStepSeq(m_ss_glide, m_ss_gate);
	/*
	int steps = controlPoints.size() / 3;
	float barlength = 1.0 / float(steps);
	for (int i = 0; i < controlPoints.size(); i++) {
		int stp = i / 3;
		int point = i % 3;
		if (point == 2) {
			controlPoints[i].xVal = (float(stp) + (gate / 100.f)) * barlength;
		}
	}
	*/
    m_isDirty= true;
    m_needsUIUpdate = true;
#ifdef _DEBUG
	vassert(validate());
#endif
}

void VASTMSEGData::stepSeqChangeGlide(float glide) {
	//const ScopedWriteLock myScopedLock(mReadWriteLock);
	m_ss_glide = glide;
	doStepSeq(m_ss_glide, m_ss_gate);
	/*
	int steps = controlPoints.size() / 3;
	float barlength = 1.0 / float(steps);
	for (int i = 0; i < controlPoints.size(); i++) {
		int stp = i / 3;
		int point = i % 3;
		if (point == 0) {
			controlPoints[i].curvy = glide / 100.f;
		}
	}
	*/

    m_isDirty = true;
    m_needsUIUpdate = true;
}

void VASTMSEGData::stepSeqChangeSteps(int steps, float glide, float gate) {
	//const ScopedWriteLock myScopedLock(mReadWriteLock);
	int bsize = int(m_ss_bars.size());
	if (steps < bsize) {
		//for (int i=0; i< bsize - steps; i++)
			//keep it
			//for (int i=0; i< bsize - steps; i++)
			//    m_ss_bars.pop_back();
	}
	else {
		for (int i = 0; i< steps - bsize; i++)
			m_ss_bars.push_back(1.f);
	}
	m_ss_glide = glide;
	m_ss_gate = gate;
	m_ss_bars_num = steps;

	doStepSeq(m_ss_glide, m_ss_gate);
	
    m_isDirty = true;
    m_needsUIUpdate = true;
#ifdef _DEBUG
	vassert(validate());
#endif
}

float VASTMSEGData::getStepSeqGlide() {
	return m_ss_glide;
}

float VASTMSEGData::getStepSeqGate() {
	return m_ss_gate;
}

int VASTMSEGData::getStepSeqSteps() {
	return m_ss_bars_num;
}

void VASTMSEGData::initStepSeq(int stepSeqNo) {
	invert = false;
	m_isStepSeqData = true;
	m_ss_bars_num = 4;
	patternName = TRANS("Default pattern");
	m_ss_bars.clear();
	for (int i = 0; i < m_ss_bars_num; i++)
		m_ss_bars.push_back((1.f / (m_ss_bars_num + 1)) * ((m_ss_bars_num + 1)- i));
	doStepSeq(25.0f, 65.f);
	m_needsUIUpdate = true;
}

void VASTMSEGData::initStepSeqSidechain() {
	invert = true;
	m_isStepSeqData = true;
	patternName = TRANS("Sidechain");
	m_ss_bars.clear();
	m_ss_bars.push_back(1.f);
	m_ss_bars_num = 1;
	doStepSeq(20.0f, 45.f);
	m_needsUIUpdate = true;
}

void VASTMSEGData::initStepSeqStairs() {
	invert = false;
	m_isStepSeqData = true;
	m_ss_bars_num = 8;
	patternName = TRANS("Stairs");
	m_ss_bars.clear();
	for (int i = 0; i < m_ss_bars_num; i++)
		m_ss_bars.push_back((float(i + 1) / float(m_ss_bars_num)));
	doStepSeq(50.0f, 100.f);
	m_needsUIUpdate = true;
}

void VASTMSEGData::setInvert(bool inv) {
	invert = inv;

	doStepSeq(m_ss_glide, m_ss_gate);

    m_isDirty = true;
    m_needsUIUpdate = true;
}

void VASTMSEGData::doStepSeq(float glide, float gate) { //0..100
	//const ScopedWriteLock myScopedLock(mReadWriteLock);
	m_ss_glide = glide;
	m_ss_gate = gate;

	//save vals
	m_fAttackTime.store(m_fAttackTimeExternalSet.load());
	m_fDecayTime.store(m_fDecayTimeExternalSet.load());
	m_fReleaseTime.store(m_fReleaseTimeExternalSet.load());

	controlPoints.clear();

    m_isDirty = true;
    m_needsUIUpdate = true;

	float l_gate = gate;
	float barlength = 1.0f / float(getStepSeqSteps());
	for (int stp = 0; stp < getStepSeqSteps(); stp++) {

		if (invert == false) {
			//first: bottom left
			ControlPoint point1;
			point1.xVal = stp * barlength;
			point1.yVal = 0.f;
			point1.curvy = glide / 100.f;
			if (stp == 0)
				point1.isLoopStart = true;
			controlPoints.push_back(point1);

			//second: top left
			ControlPoint point2;
			point2.xVal = stp * barlength;
			point2.yVal = m_ss_bars[stp];
			point2.curvy = 0.5f;
			controlPoints.push_back(point2);

			//third: top right
			ControlPoint point3;
			vassert(l_gate <= 100.f);
			point3.xVal = (float(stp) + (l_gate / 100.f)) * barlength;
			point3.yVal = m_ss_bars[stp];
			point3.curvy = 0.5f;
			controlPoints.push_back(point3);

			if (glide == 0.f) {
				ControlPoint point4;
				point4.xVal = (float(stp) + (l_gate / 100.f)) * barlength;
				point4.yVal = 0.f;
				point4.curvy = 0.5f;
				controlPoints.push_back(point4);
			}
		}
		else { //inverted
			   //first: bottom left
			l_gate = 100 - gate;
			ControlPoint point1;
			point1.xVal = stp * barlength;
			point1.yVal = 0.f;
			point1.curvy = 0;
			if (stp == 0)
				point1.isLoopStart = true;
			controlPoints.push_back(point1);

			//second: top
			ControlPoint point2;
			vassert(l_gate <= 100.f);
			point2.xVal = (float(stp) + (l_gate / 100.f)) * barlength;
			point2.yVal = m_ss_bars[stp];
			point2.curvy = glide / 100.f;

			if (glide == 0.f) {
				ControlPoint point4;
				point4.xVal = (float(stp) + (l_gate / 100.f)) * barlength;
				point4.yVal = 0.f;
				point4.curvy = 0.5f;
				controlPoints.push_back(point4);
			}
			controlPoints.push_back(point2);

			//third: top right
			ControlPoint point3;
			point3.xVal = (stp + 1) * barlength;
			point3.yVal = m_ss_bars[stp];
			point3.curvy = 0.5f;
			controlPoints.push_back(point3);
		}
	}

	//last point
	if (invert == false) {
		ControlPoint point1;
		point1.xVal = 1.0f;
		point1.yVal = 0.f;
		point1.isSustain = true;
		point1.curvy = glide / 100.f;
		controlPoints.push_back(point1);
	}
	else {
		controlPoints[controlPoints.size() - 1].isSustain = true;
	}
	checkLoop();
	setStepSeqTime(m_fOrigStepSeqTime);

#ifdef _DEBUG
	vassert(validate());
#endif
}

bool VASTMSEGData::validate() {

	for (int i = 1; i < controlPoints.size(); i++) {
		if ((controlPoints[i].xVal < 0) || (controlPoints[i].xVal > 1))
			return false;
		if (controlPoints[i].xVal < controlPoints[i-1].xVal)
			return false;
	}
	return true;
}

void VASTMSEGData::setXYValues(int pointno, double newxval, double newyval) {
    m_isDirty = true;
    m_needsUIUpdate = true;
	
	double xVal = newxval;
	double yVal = newyval;
	if (pointno > 0) {
		xVal = jmax(xVal, controlPoints[pointno - 1].xVal);
	} else 
		xVal = jmax(xVal, 0.0);
	if (pointno < controlPoints.size()-1) {
		xVal = jmin(xVal, controlPoints[pointno + 1].xVal);
	} else
		xVal = jmin(xVal, 1.0);
	yVal = jmax(0.0, yVal);
	yVal = jmin(1.0, yVal);
	controlPoints[pointno].xVal = xVal;
	controlPoints[pointno].yVal = yVal;
	calcADSR();
}

void VASTMSEGData::setCurveValues(int pointno, double newval) {
    m_isDirty = true;
    m_needsUIUpdate = true;

	double curveVal = newval;
	curveVal = jmax(0.0, curveVal);
	curveVal = jmin(1.0, curveVal);
	controlPoints[pointno].curvy = float(curveVal);
}

void VASTMSEGData::toggleDecayPoint(int pointno) {
//TODO check consistency
    m_isDirty = true;
    m_needsUIUpdate = true;

	bool oldVal = controlPoints[pointno].isDecay;
	for (int i = 0; i < controlPoints.size(); i++)
		controlPoints[i].isDecay = false;
	controlPoints[pointno].isDecay = !oldVal;
	if (controlPoints[pointno].isDecay) {
		for (int i = 0; i <= pointno; i++) //no sustain point here or before
			controlPoints[i].isSustain = false;
	}
	//double totalDuration =
    calcTotalDuration(); //to set externals
	calcADSR();
}

void VASTMSEGData::toggleLoopStart(int pointno) {
	//TODO check consistency
    m_isDirty = true;
    m_needsUIUpdate = true;

	bool oldVal = controlPoints[pointno].isLoopStart;
	for (int i = 0; i < controlPoints.size(); i++)
		controlPoints[i].isLoopStart = false;
	controlPoints[pointno].isLoopStart = !oldVal;
	checkLoop();
    //double totalDuration =
    calcTotalDuration(); //to set externals
}

void VASTMSEGData::toggleMPELift(int pointno) {
	//TODO check consistency
    m_isDirty = true;
    m_needsUIUpdate = true;

	bool oldVal = controlPoints[pointno].isMPELift;
	//for (int i = 0; i < controlPoints.size(); i++)
		//controlPoints[i].isMPELift = false;
	controlPoints[pointno].isMPELift = !oldVal; //can have multiple
	checkLoop();
	//double totalDuration =
	calcTotalDuration(); //to set externals
}

void VASTMSEGData::toggleSustainPoint(int pointno) {
	//TODO check consistency
    m_isDirty = true;
    m_needsUIUpdate = true;

	bool oldVal = controlPoints[pointno].isSustain;
	for (int i = 0; i < controlPoints.size(); i++)
		controlPoints[i].isSustain = false;
	controlPoints[pointno].isSustain = !oldVal;
	if (controlPoints[pointno].isSustain) {
		for (int i = pointno; i < controlPoints.size(); i++) //no decay point here or after
			controlPoints[i].isDecay = false;
	}
	checkLoop();
	//double totalDuration =
    calcTotalDuration(); //to set externals
	calcADSR();
}

int VASTMSEGData::getSustainPoint() const {
	int point = -1;
	for (int i = 0; i < controlPoints.size(); i++) {
		if (controlPoints[i].isSustain == true)
			point = i;
	}
	return point;
}

void VASTMSEGData::checkLoop() {
	loopStartPoint = -1;
	loopEndPoint = -1;
	hasLoop = false;
	for (int i = 0; i < controlPoints.size(); i++) {
		if (controlPoints[i].isLoopStart == true)
			loopStartPoint = i;
		if (controlPoints[i].isSustain == true)
			loopEndPoint = i;
	}
	if (loopStartPoint>=0 && loopEndPoint >= 0)
		hasLoop = true;
}

void VASTMSEGData::clearLoopPoints() {
	//TODO check consistency
    m_isDirty = true;
    m_needsUIUpdate = true;

	for (int i = 0; i < controlPoints.size(); i++) {
		controlPoints[i].isSustain = false;
		controlPoints[i].isLoopStart = false;
	}
	//bool hasLoop = false;
	//int loopStartPoint = -1;
	//int loopEndPoint = -1;
}

void VASTMSEGData::setCurveStyle(int pointno, int style) {
    m_isDirty = true;
    m_needsUIUpdate = true;

	controlPoints[pointno].curveStyle = style;
}

//-------------------------------------------------------------------------------------------
// valueTree state
void VASTMSEGData::getValueTreeState(ValueTree* tree, UndoManager* undoManager, bool isMseg) { //save
	//const ScopedReadLock myScopedLock(mReadWriteLock);

	tree->removeAllChildren(undoManager);
	tree->removeAllProperties(undoManager);

	tree->setProperty("patternName", patternName, undoManager);
	if (isMseg) {
		tree->setProperty("env_mode", env_mode, undoManager);

		tree->setProperty("m_bSynch", m_bSynch, undoManager);
		tree->setProperty("m_uTimeBeats", m_uTimeBeats.load(), undoManager);
		tree->setProperty("m_fSustainLevelExternalSet", m_fSustainLevelExternalSet.load(), undoManager);
		tree->setProperty("m_fAttackTimeExternalSet", m_fAttackTimeExternalSet.load(), undoManager);
		tree->setProperty("m_fDecayTimeExternalSet", m_fDecayTimeExternalSet.load(), undoManager);
		tree->setProperty("m_fReleaseTimeExternalSet", m_fReleaseTimeExternalSet.load(), undoManager);
		tree->setProperty("m_fAttackSteps", m_fAttackSteps.load(), undoManager);
		tree->setProperty("m_fDecaySteps", m_fDecaySteps.load(), undoManager);
		tree->setProperty("m_fReleaseSteps", m_fReleaseSteps.load(), undoManager);

		//points
		tree->setProperty("numControlPoints", int(controlPoints.size()), undoManager);
		for (int i = 0; i < controlPoints.size(); i++) {
            std::unique_ptr<ValueTree> subtree(new ValueTree(Identifier("msegPoint" + String(i))));
			subtree->setProperty("isDecay", controlPoints[i].isDecay, undoManager);
			subtree->setProperty("isSustain", controlPoints[i].isSustain, undoManager);
			subtree->setProperty("isLoopStart", controlPoints[i].isLoopStart, undoManager);
			subtree->setProperty("isMPELift", controlPoints[i].isMPELift, undoManager);
			jassert(isnan(controlPoints[i].xVal) == false);
			subtree->setProperty("xVal", controlPoints[i].xVal, undoManager);
			subtree->setProperty("yVal", controlPoints[i].yVal, undoManager);
			subtree->setProperty("curvy", controlPoints[i].curvy, undoManager);
			subtree->setProperty("curveStyle", controlPoints[i].curveStyle, undoManager);
			tree->appendChild(*subtree.get(), undoManager);
		}
	}
	else { //StepSeq
		tree->setProperty("invert", invert, undoManager);
		tree->setProperty("numSteps", int(getStepSeqSteps()), undoManager);
		for (int i = 0; i < getStepSeqSteps(); i++) {
            std::unique_ptr<ValueTree> subtree(new ValueTree(Identifier("stepSeqStep" + String(i))));
			subtree->setProperty("barHeight", m_ss_bars[i], undoManager);
			tree->appendChild(*subtree.get(), undoManager);
		}
	}
}

void VASTMSEGData::setValueTreeState(ValueTree* tree, bool isMseg, CVASTSettings* set) { //load
	//const ScopedWriteLock myScopedLock(mReadWriteLock);
	init();
	patternName = tree->getProperty("patternName");

	if (isMseg) {
		env_mode = tree->getProperty("env_mode");
		m_bSynch = tree->getProperty("m_bSynch");
		m_uTimeBeats = tree->getProperty("m_uTimeBeats");

		m_fAttackTimeExternalSet.store(tree->getProperty("m_fAttackTimeExternalSet"));
		m_fAttackTime.store(m_fAttackTimeExternalSet.load());
		m_fDecayTimeExternalSet.store(tree->getProperty("m_fDecayTimeExternalSet"));
		m_fDecayTime.store(m_fDecayTimeExternalSet.load());
		m_fReleaseTimeExternalSet.store(tree->getProperty("m_fReleaseTimeExternalSet"));
		m_fReleaseTime.store(m_fReleaseTimeExternalSet.load());

		m_fAttackSteps.store(tree->getProperty("m_fAttackSteps"));
		m_fDecaySteps.store(tree->getProperty("m_fDecaySteps"));
		m_fReleaseSteps.store(tree->getProperty("m_fReleaseSteps"));
		
		//points
		controlPoints.clear();
		int numControlPoints = tree->getProperty("numControlPoints");
		float sustainYVal = 0.f;
		for (int i = 0; i < numControlPoints; i++) {
			ValueTree subtree;
			subtree = tree->getChildWithName(Identifier("msegPoint" + String(i)));
			vassert(subtree.isValid());
			ControlPoint point;
			point.isDecay = subtree.getProperty("isDecay");
			point.isSustain = subtree.getProperty("isSustain");
			point.isLoopStart = subtree.getProperty("isLoopStart");
			point.isMPELift = subtree.getProperty("isMPELift");
			point.xVal = subtree.getProperty("xVal");
			jassert(isnan(point.xVal) == false);
			point.yVal = subtree.getProperty("yVal");
			point.curvy = subtree.getProperty("curvy");
			point.curveStyle = subtree.getProperty("curveStyle");
			controlPoints.push_back(point); //addPoint(point);

			if (point.isSustain == true) sustainYVal = float(point.yVal); //for compatibility
		}

		//Check old compatibility
		if (numControlPoints < 2) {
			initToADR(m_msegNo);
		}
		if (tree->hasProperty("m_fSustainLevelExternalSet"))   //was added later – compatibility for old presets
			m_fSustainLevelExternalSet = tree->getProperty("m_fSustainLevelExternalSet");
		else
			m_fSustainLevelExternalSet.store(sustainYVal);
		m_fSustainLevel.store(m_fSustainLevelExternalSet.load());
	}
	else { //stepSeq
		if (!tree->hasProperty("invert")) {
			//old preset
			initStepSeq(m_stepSeqNo);
		}
		else {
			invert = tree->getProperty("invert");
			m_ss_bars_num = tree->getProperty("numSteps");
			m_ss_bars.clear();
			for (int i = 0; i < m_ss_bars_num; i++) {
				ValueTree subtree;
				subtree = tree->getChildWithName(Identifier("stepSeqStep" + String(i)));
				vassert(subtree.isValid());
				float val = subtree.getProperty("barHeight").toString().getFloatValue(); //TODO safety for corrupted files
				m_ss_bars.push_back(val);
			}
		}
	}
	
    m_isDirty = true;
    m_needsUIUpdate = true;

	if (isMseg) {
		if (m_bSynch) {
			setAttackSteps(m_fAttackSteps, set);
			setDecaySteps(m_fDecaySteps, set);
			setReleaseSteps(m_fReleaseSteps, set);
		}

		checkLoop();
		calcADSR();
        m_bADSR_updated = true;
	}
	else { //StepSeq
		doStepSeq(m_ss_glide, m_ss_gate);
	}
}

bool VASTMSEGData::getInvert() const {
	return invert;
}

void VASTMSEGData::setAttackSteps(double attackSteps, CVASTSettings* set) {
	if (hasAttackPhase()) {
		m_fAttackSteps = attackSteps;
		float millisPerBeat = float(set->getMillisecondsPerBeat());
		float intRatio = float(set->getIntervalRatio(m_uTimeBeats.load()));
		float time = float((intRatio * attackSteps) * millisPerBeat);
		setAttackTime(time);
        m_isDirty = true;
	}
}
void VASTMSEGData::setDecaySteps(double decaySteps, CVASTSettings* set) {
	int decayPoint = getDecayPoint();
	if (decayPoint != (controlPoints.size() - 1)) { //shall not be last
		m_fDecaySteps = decaySteps;
		float millisPerBeat = float(set->getMillisecondsPerBeat());
		float intRatio = float(set->getIntervalRatio(m_uTimeBeats.load()));
		float time = float((intRatio * decaySteps) * millisPerBeat);
		setDecayTime(time);
        m_isDirty = true;
	}
}
void VASTMSEGData::setReleaseSteps(double releaseSteps, CVASTSettings* set) {
	if (hasReleasePhase()) {
		m_fReleaseSteps = releaseSteps;
		float millisPerBeat = float(set->getMillisecondsPerBeat());
		float intRatio = float(set->getIntervalRatio(m_uTimeBeats.load()));
		float time = float((intRatio * releaseSteps) * millisPerBeat);
		setReleaseTime(time);
        m_isDirty = true;
	}
}

float VASTMSEGData::getAttackSteps() const {
	return float(m_fAttackSteps.load());
}

float VASTMSEGData::calcStepsFromTime(double time, CVASTSettings* set) {
	float millisPerBeat = float(set->getMillisecondsPerBeat());
	float intRatio = float(set->getIntervalRatio(m_uTimeBeats.load()));
	float steps = float((time / millisPerBeat) / intRatio);
	return steps;
}

void VASTMSEGData::setAttackTime(double attackTime) {
	if (hasAttackPhase()) {
		m_fAttackTimeExternalSet = attackTime;
		m_fAttackTime = attackTime;
		doADSR();
        m_isDirty = true;
        m_needsUIUpdate = true;
	}
	else
		m_fAttackTimeExternalSet.store(0.0);
}

void VASTMSEGData::setDecayTime(double decayTime) {
	int decayPoint = getDecayPoint();
	if (decayPoint != (controlPoints.size() - 1)) { //shall not be last
		m_fDecayTimeExternalSet.store(decayTime);
		m_fDecayTime.store(decayTime);
		doADSR();
        m_isDirty = true;
        m_needsUIUpdate = true;
	}
}

void VASTMSEGData::setReleaseTime(double releaseTime) {
	if (hasReleasePhase()) {
		m_fReleaseTimeExternalSet.store(releaseTime); //CHECK this was commented out but why??? with comment it cannot be automated!
		m_fReleaseTime.store(releaseTime);
		doADSR();
        m_isDirty = true;
        m_needsUIUpdate = true;
	}
	else
		m_fReleaseTimeExternalSet.store(0.0);
}

void VASTMSEGData::setSustainLevel(double sustainLevel) { //0..1
	m_fSustainLevel.store(sustainLevel);
	//m_fSustainLevelExternalSet = sustainLevel; //new
	if (m_fSustainLevel.load() > 1.0)
        m_fSustainLevel.store(1.0);
	if (m_fSustainLevel.load() < 0.0)
        m_fSustainLevel.store(0.0);
	int susPoint = getSustainPoint();
	if (susPoint != -1) {
		controlPoints[susPoint].yVal = sustainLevel;
        m_isDirty = true;
        m_needsUIUpdate = true;
	}
}

void VASTMSEGData::setAttackTimeSlider(double attackTime) {
	if (hasAttackPhase()) {
		m_fAttackTimeExternalSet.store(float(attackTime));
		m_fAttackTime.store(float(attackTime));
		doADSR();
        m_isDirty = true;
        m_needsUIUpdate = true;
	}
	else
		m_fAttackTimeExternalSet.store(0.0);
}

void VASTMSEGData::setDecayTimeSlider(double decayTime) {
	int decayPoint = getDecayPoint();
	if (decayPoint != (controlPoints.size() - 1)) { //shall not be last
		m_fDecayTimeExternalSet.store(float(decayTime));
		m_fDecayTime.store(float(decayTime));
		doADSR();
        m_isDirty = true;
        m_needsUIUpdate = true;
	}
}

void VASTMSEGData::setReleaseTimeSlider(double releaseTime) {
	if (hasReleasePhase()) {
		m_fReleaseTimeExternalSet.store(float(releaseTime));
		m_fReleaseTime.store(float(releaseTime));
		doADSR();
        m_isDirty = true;
        m_needsUIUpdate = true;
	}
	else
		m_fReleaseTimeExternalSet.store(0.0);
}

void VASTMSEGData::setSustainLevelSlider(double sustainLevel) { //0..1
	m_fSustainLevel.store(sustainLevel);
    m_fSustainLevelExternalSet.store(sustainLevel);
	if (m_fSustainLevel.load() > 1.0) 
        m_fSustainLevel.store(1.0);
	if (m_fSustainLevel.load() < 0.0)
        m_fSustainLevel.store(0.0);
	int susPoint = getSustainPoint();
	if (susPoint != -1) {
		controlPoints[susPoint].yVal = sustainLevel;
        m_isDirty = true;
        m_needsUIUpdate = true;
	}
}

void VASTMSEGData::setStepSeqTime(double stepSeqTime) {
	int cursteps = getStepSeqSteps();
	double decaytime = stepSeqTime * cursteps;
	m_fOrigStepSeqTime = float(stepSeqTime);
	m_fDecayTimeExternalSet.store(float(decaytime));
	m_fAttackTime.store(0.0);
	m_fReleaseTime.store(0.0);
	m_fAttackTimeExternalSet.store(0.0);
	m_fReleaseTimeExternalSet.store(0.0);
	m_fSustainLevelExternalSet.store(0.0);
	m_fDecayTime.store(decaytime);

    m_isDirty = true;
    m_needsUIUpdate = true;
}

int VASTMSEGData::getNumSegments() const { return int(controlPoints.size()) - 1; }

VASTMSEGData::ControlPoint* VASTMSEGData::getSegmentStart(int segment) {
	vassert(segment < getNumSegments());
	if (segment >= getNumSegments()) return nullptr;
	return &controlPoints[segment];
}

VASTMSEGData::ControlPoint* VASTMSEGData::getSegmentEnd(int segment) {
	vassert(segment < getNumSegments());
	if (segment >= getNumSegments()) return nullptr;
	return &controlPoints[segment + 1];
}

float VASTMSEGData::getDecaySteps() const {
	return float(m_fDecaySteps.load());
}

float VASTMSEGData::getReleaseSteps() const {
	return float(m_fReleaseSteps.load());
}
