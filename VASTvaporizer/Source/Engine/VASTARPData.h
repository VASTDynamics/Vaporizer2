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
	int m_dispActiveStep = 0;

	int getNumSteps() { 
		return arpStepNum; ;
	};

	void setDirty() {
		isDirty = true;
	}

	void arpChangeSteps(int steps);
	StringArray getDefaultPatternNames();
	
	ArpStep getStep(int step) {
		return arpSteps[step];
	}

	void incGate(int step) {
		arpSteps[step].gate++;
		arpSteps[step].gate %= 5; //0..4
		isDirty = true;
	}
	void setOctave(int step, int octave) {
		arpSteps[step].octave = octave;
		isDirty = true;
	}
	void setSemitone(int step, int semitone) {
		arpSteps[step].semitones = semitone;
		isDirty = true;
	}	
	void setVelocity(int step, int velocity) {
		arpSteps[step].velocity = jlimit<int>(0, 127, velocity);
		isDirty = true;
	}
	/*
	void insertPoint(int newpos, VASTARPData::ControlPoint point);
	void removePoint(int delpos);
	*/
	//bool validate();
	void getValueTreeState(ValueTree* tree, UndoManager* undoManager);
	void setValueTreeState(ValueTree* tree);

	void initDefaultPattern(int pattern);
	bool getAndClearDirtyFlag() {
		bool lDirty = isDirty;
		isDirty = false;
		return lDirty;
	}
	void setDispActiveStep(int step) {
		if (m_dispActiveStep != step) isDirty = true;
		m_dispActiveStep = step;
	}
	bool getIsDirty() {
		return isDirty;
	}
	void clearDirtyFlag() {
		isDirty = false;
	}

	//ReadWriteLock mReadWriteLock; //one lock per instance of class

private:
	std::vector<VASTARPData::ArpStep> arpSteps;
	int arpStepNum = 0;
	bool isDirty = true;

	int m_iSampleRate = 0;

	void addStep(VASTARPData::ArpStep step); //internal - has to be locked
	void removeStep(int delpos); //internal - has to be locked

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VASTARPData)
};