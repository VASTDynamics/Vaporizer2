/*
VAST Dynamics
*/

#pragma once

#include "VASTEngineHeader.h"

class VASTARPData {
public:
	VASTARPData();

	typedef struct ArpPattern {
		const std::string name;
		int length;
		int octave[64];
		int semitones[64];
		int velocity[64];
		int gate[64];
	} ArpPattern;

	struct ArpStep {
		//settings
		//changes here have to be reflected in get and setValueTreeState!!

		int octave = 0; //can be -2..2
		int semitones = 0; //can be -11 .. +11
		int velocity = 127; // can be 0..127 - midi values
		int gate = 3; //can be 0..4 // 0 is pause and 4 is hold
	};

	static const ArpPattern mARPPattern[];
	
	String patternName = "unnamed";
	
	void copyDataFrom(const VASTARPData &copyData);

	//dynamic
	std::atomic<int> m_dispActiveStep = 0;

	int getNumSteps() const; 

	void setDirty();

	void arpChangeSteps(int steps);
	StringArray getDefaultPatternNames();
	
	ArpStep getStep(int step);

	void incGate(int step);
	void setOctave(int step, int octave);
	void setSemitone(int step, int semitone);
	void setVelocity(int step, int velocity);
	/*
	void insertPoint(int newpos, VASTARPData::ControlPoint point);
	void removePoint(int delpos);
	*/
	//bool validate();
	void getValueTreeState(ValueTree* tree, UndoManager* undoManager);
	void setValueTreeState(ValueTree* tree);

	void initDefaultPattern(int pattern);
	bool getAndClearDirtyFlag();
	void setDispActiveStep(int step);
	bool getIsDirty();
	void clearDirtyFlag();

	//ReadWriteLock mReadWriteLock; //one lock per instance of class

private:
	std::vector<VASTARPData::ArpStep> arpSteps;
	int arpStepNum = 0;
	std::atomic<bool> m_isDirty = true;

	int m_iSampleRate = 0;

	void addStep(VASTARPData::ArpStep step); //internal - has to be locked
	void removeStep(int delpos); //internal - has to be locked

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VASTARPData)
};
