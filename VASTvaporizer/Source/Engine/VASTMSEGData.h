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
	double getAttackTime() const; //ms
	double getDecayTime() const; //ms
	double getSustainLevel() const; /** < 0.0 to 1.0. */
	double getReleaseTime() const; //ms
	bool hasAttackPhase() const;
	bool hasReleasePhase() const;

	void setSynch(bool synch);
	bool getSynch() const;
	void setTimeBeats(int timeBeats);
	int getTimeBeats() const;

	void setAttackSteps(double attackSteps, CVASTSettings* set);
	void setDecaySteps(double decaySteps, CVASTSettings* set);
	void setReleaseSteps(double releaseSteps, CVASTSettings* set);
	float getAttackSteps() const;
	float getDecaySteps() const;
	float getReleaseSteps() const;
	float calcStepsFromTime(double time, CVASTSettings* set);

	void setAttackTime(double attackTime);
	void setDecayTime(double decayTime);
	void setReleaseTime(double releaseTime);
	void setSustainLevel(double sustainLevel);

	void setAttackTimeSlider(double attackTime);
	void setDecayTimeSlider(double decayTime);
	void setReleaseTimeSlider(double releaseTime);
	void setSustainLevelSlider(double sustainLevel);
	
	void setStepSeqTime(double stepSeqTime);
	
	//dynamic
    bool hasLoop = false;
    std::atomic<int> loopStartPoint = -1;
    std::atomic<int> loopEndPoint = -1;

	int getNumSegments() const;
	ControlPoint* getSegmentStart(int segment); //not const
	ControlPoint* getSegmentEnd(int segment); //not const

	void calcSegmentCoefficients(int samplerate, ULong64_t startPlayTimestamp, int activeSegment, int samplesSinceSegmentStart, int segmentLengthInSamples, CVASTSettings* set, int voiceNo);

	double calcTotalDuration(); //this is per definition in this model! // not const
	
	void setXYValues(int pointno, double newxval, double newyval);
	void setCurveValues(int pointno, double newval);
	void setEnvMode(int mode);
	void setCurveStyle(int pointno, int style);
	void toggleDecayPoint(int pointno);
	void toggleLoopStart(int pointno);
	void toggleSustainPoint(int pointno);
	void toggleMPELift(int pointno);
	void clearLoopPoints();
	int getSustainPoint() const;
	int getDecayPoint() const;
	bool validate();
	void getValueTreeState(ValueTree* tree, UndoManager* undoManager, bool isMseg); //not const
	void setValueTreeState(ValueTree* tree, bool isMseg, CVASTSettings* set);
	bool getInvert() const;
	void setInvert(bool inv);
	
	void doStepSeq(float glide, float gate);
	void stepSeqChangeBar(int step, float barheight);
	void stepSeqChangeGate(float gate);
	void stepSeqChangeGlide(float glide);
	void stepSeqChangeSteps(int steps, float glide, float gate);
	float getStepSeqGlide();
	float getStepSeqGate();
	int getStepSeqSteps();
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

	void setUIDisplay(int dispActiveSegment, int dispSamplesSinceSegmentStart, int dispSegmentLengthInSamples, int voiceNo, bool isPlaying);
	bool getAndClearPositionUpdateFlag();
	bool getAndClearUIUpdateFlag();

	bool m_isStepSeqData = false;
	int m_stepSeqNo = 0; //this is the STESEQ1-3 number 0..2
	int m_msegNo = 0;
	//ReadWriteLock mReadWriteLock; //one lock per instance of class
	void setDirty();
	void clearDirtyFlag();
	bool isMSEGDirty() const;

	bool getADSRUpdated() const;
	void resetADSRUpdated();
	int getDispActiveSegment(int voiceNo) const;
	int getDispSamplesSinceSegmentStart(int voiceNo) const;
	int getDispSegmentLengthInSamples(int voiceNo) const;
	bool dispVoicePlaying(int voiceNo);

    std::atomic<double> m_fAttackSteps = 1.f;
    std::atomic<double> m_fDecaySteps = 1.f;
    std::atomic<double> m_fReleaseSteps = 1.f;

private:		
	void checkLoop();
	void calcADSR();
	void doADSR();

	void addPoint(VASTMSEGData::ControlPoint point); //internal - needs to be locked
	void insertPoint(int newpos, VASTMSEGData::ControlPoint point); //internal - needs to be locked
	void removePoint(int delpos); //internal - needs to be locked

    bool m_isDirty = true;
    bool m_needsUIUpdate = true;
    bool m_needsPositionUpdate = true;
	
    bool m_bSynch = false;
    std::atomic<int> m_uTimeBeats = 1;

	int m_iSampleRate = 0;
    std::atomic<double> m_fAttackTime = 1.0f; // in ms
    std::atomic<double> m_fDecayTime = 1.0f; // in ms
    std::atomic<double> m_fReleaseTime = 1.0f; // in ms
    std::atomic<double> m_fSustainLevel = 1.0f; // 0..1

    std::atomic<double> m_fAttackTimeExternalSet = 1.0f; // in ms
    std::atomic<double> m_fDecayTimeExternalSet = 1.0f; // in ms
    std::atomic<double> m_fReleaseTimeExternalSet = 1.0f; // in ms
    std::atomic<double> m_fSustainLevelExternalSet = 1.0f; // percent

	float m_ss_glide = 0.f;
	float m_ss_gate = 0.f;
	std::vector<float> m_ss_bars;
	int m_ss_bars_num = 0;
	float m_fOrigStepSeqTime = 0.f;

    bool m_bADSR_updated = true;
    bool m_bStepSeq_updated = true;

    std::atomic<int> m_dispActiveSegment[C_MAX_POLY] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    std::atomic<int> m_dispSamplesSinceSegmentStart[C_MAX_POLY] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    std::atomic<int> m_dispSegmentLengthInSamples[C_MAX_POLY] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    std::atomic<bool> m_dispVoicePlaying[C_MAX_POLY] = { false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false };

	JUCE_LEAK_DETECTOR(VASTMSEGData)
};
