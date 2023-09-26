/*
VAST Dynamics
*/

#pragma once

#include "VASTEngineHeader.h"
#include "VASTPluginConstants.h"
#include <vector>

class CVASTSettings; //forward declaration
class VASTMSEGData {
public:
	VASTMSEGData();
	VASTMSEGData(const VASTMSEGData &); //copy constructor
	void init();

	void copyDataFrom(const VASTMSEGData &copyData);
	
	enum EnvelopeMode { unipolar, biploar };
	enum CurveStyle { Hold, SingleCurve, HalfSine, Stairs };	
	struct ControlPoint {
	public:
		//settings
		//changes here have to be reflected in get and setValueTreeState!!
		bool isDecay = false;
		bool isSustain = false;
		bool isLoopStart = false;
		bool isMPELift = false;

		double xVal = 0.0f; //0.0 to 1.0
		double yVal = 0.0f; //0.0 to 1.0

		float curvy = 0.5f; //0.0 to 1.0 percentage, for segment to point on left side!	
		int curveStyle = CurveStyle::SingleCurve;

		//calculated values?
		double coeff = 0.0f;
		double offset = 0.0f;
		int segmentLengthInSamples = 0;
	};

	String patternName = "unnamed";
	int env_mode = VASTMSEGData::EnvelopeMode::unipolar;
	std::vector<VASTMSEGData::ControlPoint> controlPoints;
	bool invert = false;

	// design time interface
	//float getAttackLevel() { return 0.f; };
	double getAttackTime() {
		return m_fAttackTime; }; //ms
	double getDecayTime() {
		return m_fDecayTime; }; //ms
	double getSustainLevel() {
		return m_fSustainLevel;  };/** < 0.0 to 1.0. */
	double getReleaseTime() {
		return m_fReleaseTime; }; //ms
	bool hasAttackPhase();
	bool hasReleasePhase();

	void setSynch(bool synch) {
		m_bSynch = synch;
		isDirty = true;
	};
	bool getSynch() {
		return m_bSynch;
	};
	void setTimeBeats(int timeBeats) {
		m_uTimeBeats = timeBeats;
		isDirty = true;
	};
	int getTimeBeats() {
		return m_uTimeBeats;
	};

	void setAttackSteps(double attackSteps, CVASTSettings* set);
	void setDecaySteps(double decaySteps, CVASTSettings* set);
	void setReleaseSteps(double releaseSteps, CVASTSettings* set);
	float getAttackSteps();
	float getDecaySteps();
	float getReleaseSteps();
	float calcStepsFromTime(double time, CVASTSettings* set);

	void setAttackTime(double attackTime) {
		if (hasAttackPhase()) {
			m_fAttackTimeExternalSet = attackTime;
			m_fAttackTime = attackTime;
			doADSR();
			isDirty = true;
			m_needsUIUpdate = true;
		} else 
			m_fAttackTimeExternalSet = 0.0;
	};
	void setDecayTime(double decayTime) { 
		int decayPoint = getDecayPoint();
		if (decayPoint != (controlPoints.size() - 1)) { //shall not be last
			m_fDecayTimeExternalSet = decayTime;
			m_fDecayTime = decayTime;
			doADSR();
			isDirty = true;
			m_needsUIUpdate = true;
		}
	};
	void setReleaseTime(double releaseTime) {
		if (hasReleasePhase()) {
			m_fReleaseTimeExternalSet = releaseTime; //CHECK this was commented out but why??? with comment it cannot be automated!
			m_fReleaseTime = releaseTime;
			doADSR();
			isDirty = true;
			m_needsUIUpdate = true;
		} else 
			m_fReleaseTimeExternalSet = 0.0;
	};
	void setSustainLevel(double sustainLevel) { //0..1
		m_fSustainLevel = sustainLevel; 
		//m_fSustainLevelExternalSet = sustainLevel; //new
		if (m_fSustainLevel > 1.0) m_fSustainLevel = 1.0;
		if (m_fSustainLevel < 0.0) m_fSustainLevel = 0.0;
		int susPoint = getSustainPoint();
		if (susPoint != -1) {
			controlPoints[susPoint].yVal = sustainLevel;
			isDirty = true;
			m_needsUIUpdate = true;
		}
	};

	void setAttackTimeSlider(double attackTime) {
		if (hasAttackPhase()) {
			m_fAttackTimeExternalSet = attackTime;
			m_fAttackTime = attackTime;
			doADSR();
			isDirty = true;
			m_needsUIUpdate = true;
		}
		else
			m_fAttackTimeExternalSet = 0.0;
	};
	void setDecayTimeSlider(double decayTime) {
		int decayPoint = getDecayPoint();
		if (decayPoint != (controlPoints.size() - 1)) { //shall not be last
			m_fDecayTimeExternalSet = decayTime;
			m_fDecayTime = decayTime;
			doADSR();
			isDirty = true;
			m_needsUIUpdate = true;
		}
	};
	void setReleaseTimeSlider(double releaseTime) {
		if (hasReleasePhase()) {
			m_fReleaseTimeExternalSet = releaseTime;
			m_fReleaseTime = releaseTime;
			doADSR();
			isDirty = true;
			m_needsUIUpdate = true;
		}
		else
			m_fReleaseTimeExternalSet = 0.0;
	};
	void setSustainLevelSlider(double sustainLevel) { //0..1
		m_fSustainLevel = sustainLevel;
		m_fSustainLevelExternalSet = sustainLevel; //new
		if (m_fSustainLevel > 1.0) m_fSustainLevel = 1.0;
		if (m_fSustainLevel < 0.0) m_fSustainLevel = 0.0;
		int susPoint = getSustainPoint();
		if (susPoint != -1) {
			controlPoints[susPoint].yVal = sustainLevel;
			isDirty = true;
			m_needsUIUpdate = true;
		}
	};
	
	void setStepSeqTime(double stepSeqTime) {
		int cursteps = getStepSeqSteps();
		double decaytime = stepSeqTime * cursteps;
		m_fOrigStepSeqTime = stepSeqTime;		
		m_fDecayTimeExternalSet = decaytime;
		m_fAttackTime = 0.0;
		m_fReleaseTime = 0.0;
		m_fAttackTimeExternalSet = 0.0;
		m_fReleaseTimeExternalSet = 0.0;		
		m_fSustainLevelExternalSet = 0.0; //new
		m_fDecayTime = decaytime;

		isDirty = true;
		m_needsUIUpdate = true;
	};
	
	//dynamic
	bool hasLoop = false;
	int loopStartPoint = -1;
	int loopEndPoint = -1;

	int getNumSegments() { return controlPoints.size() - 1; };
	ControlPoint* getSegmentStart(int segment) {
		vassert(segment < getNumSegments());
		if (segment >= getNumSegments()) return nullptr;
		return &controlPoints[segment]; };
	ControlPoint* getSegmentEnd(int segment) { 
		vassert(segment < getNumSegments());
		if (segment >= getNumSegments()) return nullptr;
		return &controlPoints[segment+1]; };

	void calcSegmentCoefficients(int samplerate, ULong64_t startPlayTimestamp, int activeSegment, int samplesSinceSegmentStart, int segmentLengthInSamples, CVASTSettings* set, int voiceNo);

	double getTotalDuration() { 
		double dur = m_fDecayTimeExternalSet;
		if (hasAttackPhase()) dur += m_fAttackTimeExternalSet; 
		else {
			m_fAttackTimeExternalSet = 0.0; m_fAttackTime = 0.0;
		}
		if (hasReleasePhase()) dur += m_fReleaseTimeExternalSet;
		else {
			m_fReleaseTimeExternalSet = 0.0; m_fReleaseTime = 0.0;
		}
		return dur;  };  //this is per definition in this model!
	
	void setXYValues(int pointno, double newxval, double newyval);
	void setCurveValues(int pointno, double newval);
	void setEnvMode(int mode);
	void setCurveStyle(int pointno, int style);
	void toggleDecayPoint(int pointno);
	void toggleLoopStart(int pointno);
	void toggleSustainPoint(int pointno);
	void toggleMPELift(int pointno);
	void clearLoopPoints();
	int getSustainPoint();
	int getDecayPoint();
	bool validate();
	void getValueTreeState(ValueTree* tree, UndoManager* undoManager, bool isMseg);
	void setValueTreeState(ValueTree* tree, bool isMseg, CVASTSettings* set);
	bool getInvert() {
		return invert;
	}
	void setInvert(bool inv) {
		invert = inv;

		doStepSeq(m_ss_glide, m_ss_gate);

		isDirty = true;
		m_needsUIUpdate = true;
	}
	
	void doStepSeq(float glide, float gate);
	void stepSeqChangeBar(int step, float barheight);
	void stepSeqChangeGate(float gate);
	void stepSeqChangeGlide(float glide);
	void stepSeqChangeSteps(int steps, float glide, float gate);
	float getStepSeqGlide() {
		return m_ss_glide;
	}
	float getStepSeqGate() {
		return m_ss_gate;
	}
	int getStepSeqSteps() { 
		return m_ss_bars_num; 
	}
	float getStepSeqBar(int step);

	void initToADR(int msegNo);
	void initToADSR(int msegNo);
	void initToAHDSR(int msegNo);
	void initToStairs(int msegNo);
	void initToSine(int msegNo);
	void initToRamp(int msegNo);
	
	void initStepSeq(int stepSeqNo);
	void initStepSeqSidechain();
	void initStepSeqStairs();
	
	void insertPointUI(int newpos, VASTMSEGData::ControlPoint point); 
	void removePointUI(int delpos); 

	void setUIDisplay(int dispActiveSegment, int dispSamplesSinceSegmentStart, int dispSegmentLengthInSamples, int voiceNo, bool isPlaying) {
		if (dispSegmentLengthInSamples < 0)
			return;
		if ((m_dispActiveSegment[voiceNo] != dispActiveSegment) || 
			(m_dispSamplesSinceSegmentStart[voiceNo] != dispSamplesSinceSegmentStart) || 
			(m_dispSegmentLengthInSamples[voiceNo] != dispSegmentLengthInSamples) || 
			(m_dispVoicePlaying[voiceNo] != isPlaying))	{
			m_needsPositionUpdate = true;
		}
		m_dispVoicePlaying[voiceNo] = isPlaying;
		m_dispActiveSegment[voiceNo] = dispActiveSegment;
		m_dispSamplesSinceSegmentStart[voiceNo] = jlimit<float>(0.f, dispSegmentLengthInSamples, dispSamplesSinceSegmentStart);
		m_dispSegmentLengthInSamples[voiceNo] = dispSegmentLengthInSamples;
	}
	bool getAndClearPositionUpdateFlag() {
		bool lUpdate = m_needsPositionUpdate;
		m_needsPositionUpdate = false;
		return lUpdate;
	}
	bool getAndClearUIUpdateFlag() {
		bool lUpdate = m_needsUIUpdate;
		m_needsUIUpdate = false;
		return lUpdate;
	}

	bool m_isStepSeqData = false;
	int m_stepSeqNo = 0; //this is the STESEQ1-3 number 0..2
	int m_msegNo = 0;
	//ReadWriteLock mReadWriteLock; //one lock per instance of class
	void setDirty() { 
		isDirty = true; 
	};
	void clearDirtyFlag() { 
		isDirty = false; 
	};
	bool isMSEGDirty() { 
		return isDirty; 
	};	

	bool getADSRUpdated() {
		return m_bADSR_updated;
	}
	void resetADSRUpdated() {
		m_bADSR_updated = false;
	}
	int getDispActiveSegment(int voiceNo) {
		return m_dispActiveSegment[voiceNo];
	}
	int getDispSamplesSinceSegmentStart(int voiceNo) {
		return m_dispSamplesSinceSegmentStart[voiceNo];
	}
	int getDispSegmentLengthInSamples(int voiceNo) {
		return m_dispSegmentLengthInSamples[voiceNo];
	}
	bool dispVoicePlaying(int voiceNo) {
		return m_dispVoicePlaying[voiceNo];
	}

	double m_fAttackSteps = 1.f;
	double m_fDecaySteps = 1.f;
	double m_fReleaseSteps = 1.f;

private:		
	void checkLoop();
	void calcADSR();
	void doADSR();

	void addPoint(VASTMSEGData::ControlPoint point); //internal - needs to be locked
	void insertPoint(int newpos, VASTMSEGData::ControlPoint point); //internal - needs to be locked
	void removePoint(int delpos); //internal - needs to be locked

	bool isDirty = true;
	bool m_needsUIUpdate = true;
	bool m_needsPositionUpdate = true;
	
	bool m_bSynch = false;
	int m_uTimeBeats = 1;

	int m_iSampleRate = 0;
	double m_fAttackTime = 1.0f; // in ms
	double m_fDecayTime = 1.0f; // in ms
	double m_fReleaseTime = 1.0f; // in ms
	double m_fSustainLevel = 1.0f; // 0..1

	double m_fAttackTimeExternalSet = 1.0f; // in ms
	double m_fDecayTimeExternalSet = 1.0f; // in ms
	double m_fReleaseTimeExternalSet = 1.0f; // in ms
	double m_fSustainLevelExternalSet = 1.0f; // percent

	float m_ss_glide = 0.f;
	float m_ss_gate = 0.f;
	std::vector<float> m_ss_bars;
	int m_ss_bars_num = 0;
	float m_fOrigStepSeqTime = 0.f;

	bool m_bADSR_updated = true;
	bool m_bStepSeq_updated = true;

	int m_dispActiveSegment[C_MAX_POLY] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	int m_dispSamplesSinceSegmentStart[C_MAX_POLY] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	int m_dispSegmentLengthInSamples[C_MAX_POLY] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	bool m_dispVoicePlaying[C_MAX_POLY] = { false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false };

	JUCE_LEAK_DETECTOR(VASTMSEGData)
};