/*
VAST Dynamics
*/

#include "VASTARPData.h"
#include "../Engine/VASTEngineHeader.h"

const VASTARPData::ArpPattern VASTARPData::mARPPattern[] = {
	{
		"Default pattern",
		4,
		{   0,   0,   1,   1  }, //octave
		{   0,   0,   0,   7  }, //semitones
		{ 127,  30, 127,  60  }, //velocity
		{   3,   3,   3,   3  }  //gate
	},
	{
		"Single 75Gate",
		1,
		{  0  }, 
		{  0  }, 
		{ 127 }, 
		{   3 }  
	},
	{
		"Single 50Gate",
		1,
		{   0 },
		{   0 },
		{ 127 },
		{   2 }
	},
	{
		"Single 25Gate",
		1,
		{   0 },
		{   0 },
		{ 127 },
		{   1 }
	},
	{
		"Octaves",
		2,
		{   0,   1 },
		{   0,   0 },
		{ 127, 127 },
		{   2,   2 }
	},
	{
		"SEQ RaveChannel",
		16,
		{   0,   0,   0,  -1,   0,   0,  -1,   0,   0,   0,   0,   0,   1,   0,   0,   0 },
		{   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 },
		{ 110,   0,  90, 110,  90,   0, 110, 100,   0,   0, 110,  90, 100,  98,   0,  90 },
		{   3,   0,   3,   2,   4,   3,   3,   4,   3,   0,   4,   3,   3,   4,   0,   3 }
	},
	{
		"SEQ MinorScale",
		8,
		{   0,   0,   0,   0,   0,   0,   0,   1 },
		{   0,   2,   3,   5,   7,   8,  10,   0 },
		{ 127, 127, 127, 127, 127, 127, 127, 127 },
		{   3,   3,   3,   3,   3,   3,   3,   3 }
	},
	{
		"SEQ MajorScale",
		8,
		{   0,   0,   0,   0,   0,   0,   0,   1 },
		{   0,   2,   4,   5,   7,   9,  11,   0 },
		{ 127, 127, 127, 127, 127, 127, 127, 127 },
		{   3,   3,   3,   3,   3,   3,   3,   3 }
	},
	{
		"SEQ Trancer",
		16,
		{   0,   0,   0,   0,   0,   0,   0,   0 ,  0,   0,   0,   0,   0,   0,   0,   0 },
		{   8,   0,   7,   0,  10,   0,   7,   0,   8,   0,   7,   0,   3,   0,   7,   0 },
		{ 105,  84, 112,  82, 126,  94, 101,  85,  67,  44,  73,  55,  75,  48,  83,  69 },
		{   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3 }
	},
	{
		"SEQ Chords",
		16,
		{    -1,   0,  -2,   0,  -1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 },
		{     0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 ,  0,   0 },
		{   100,   0, 100,   0, 100,   0,  63,  21,   0, 100,   0,  25,  60,   0, 100, 100 },
		{     3,   0,   2,   0,   4,   3,   3,   3,   0,   4,   3,   3,   4,   3,   3,   3 }
	},
	{
		"SEQ Echoes",
		4,
		{     0,   0,   0,   0 },
		{     0,   0,   0,   0 },
		{   127, 100,  70,  10 },
		{     3,   2,   1,   1 }
	},
	{
		"SEQ Thrills",
		32,
		{     0,   0,  -1,   0,   0,  -1,   0,   0,  -1,   0,   0,  -1,   0,   0,  -1,   0,   0,   0,  -1,   0,   0,  -1,   0,   0,  -1,   0,   0,  -1,   0,   0,  -1,   0 },
		{    10,  10,   0,   10, 10,   0,  10,  10,   0,   3,   3,   0,   2,   2 ,  0,   0,   2,   2,   0,   2,   2,   0,   2,   2,   0,   3,   3,   0,   7,   7 ,  0,   0 },
		{   100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,   0, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,   0 },
		{     3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   0,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   0 }
	},
	{
		"SEQ Trance Hook",
		16,
		{     0,   0,  -1,   0,   0,  -1,   0,   0,  -1,   0,   0,  -1,   0,   0,  -1,  -1 },
		{     0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 ,  0,   0 },
		{   100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100 },
		{     3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3 }
	},
		{
		"SEQ Trance Invader",
		16,
		{     1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 },
		{     0,   0,   0,   8,   0,   0,   7,   0,   0,  10,   0,   0,   3,   0 ,  0,  10 },
		{   100, 100,  80,  90,  90,  70,  80,  80,  90,  70,  70,  90, 100, 100,  90,  60 },
		{     3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3 }
	},
		{
		"SEQ Minor 7",
		16,
		{     0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 ,  0,   0 },
		{     0,   4,   7,  11,   7,   4,   0,   4,   7,  11,   7,   4,   0,   4,   7,  11 },
		{   100, 100,  80,  90,  90,  70, 100,  80,  90,  70,  70,  90, 100, 100,  90,  60 },
		{     3,   2,   2,   2,   2,   2,   3,   2,   2,   2,   2,   2,   3,   2,   2,   2 }
	},
		{
		"SEQ Trance Hook Hold",
		16,
		{     0,   0,  -1,   0,   0,  -1,   0,   0,  -1,   0,   0,  -1,   0,   0,  -1,  -1 },
		{     0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 ,  0,   0 },
		{   100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100 },
		{     4,   3,   3,   3,   3,   3,   4,   3,   3,   4,   3,   3,   4,   3,   3,   3 }
	},
	{
		"SEQ Trance Invader Hold",
		16,
		{     1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 },
		{     0,   0,   0,   8,   8,   0,   7,   7,   0,  10,  10,   0,   3,   3 ,  0,  10 },
		{   100, 100,  80,  90,  90,  70,  80,  80,  90,  70,  70,  90, 100, 100,  90,  60 },
		{     4,   3,   3,   4,   3,   3,   4,   3,   3,   4,   3,   3,   4,   3,   3,   3 }
	},
};

VASTARPData::VASTARPData() {	
	initDefaultPattern(0);	
}

void VASTARPData::copyDataFrom(const VASTARPData &copyData) {
	arpSteps = copyData.arpSteps;
	arpStepNum = copyData.arpStepNum;
	patternName = copyData.patternName;

	m_dispActiveStep.store(copyData.m_dispActiveStep.load());
	m_isDirty.store(true);
}

int VASTARPData::getNumSteps() const {
	return arpStepNum;
}

void VASTARPData::setDirty() {
	m_isDirty.store(true);
}

void VASTARPData::initDefaultPattern(int pattern) {
	arpSteps.clear();
	patternName = VASTARPData::mARPPattern[pattern].name;
	for (int i = 0; i < VASTARPData::mARPPattern[pattern].length; i++) {
		ArpStep step;
		step.octave = VASTARPData::mARPPattern[pattern].octave[i]; //can be -2..2
		step.semitones = VASTARPData::mARPPattern[pattern].semitones[i]; //can be -11 .. +11		
		step.velocity = VASTARPData::mARPPattern[pattern].velocity[i]; // can be 0..127 - midi values
		step.gate = VASTARPData::mARPPattern[pattern].gate[i]; // can be 0..4
		addStep(step);
	}
	arpStepNum = int(arpSteps.size());
}

bool VASTARPData::getAndClearDirtyFlag() {
	bool lDirty = m_isDirty.load();
	m_isDirty.store(false);
	return lDirty;
}

void VASTARPData::setDispActiveStep(int step) {
	if (m_dispActiveStep.load() != step)
        m_isDirty.store(true);
	m_dispActiveStep = step;
}

bool VASTARPData::getIsDirty() {
	return m_isDirty.load();
}

void VASTARPData::clearDirtyFlag() {
	m_isDirty.store(false);
}

StringArray VASTARPData::getDefaultPatternNames() {
	int numElements = sizeof(VASTARPData::mARPPattern) / sizeof(VASTARPData::mARPPattern[0]);
	StringArray list;
	for (int i = 0; i < numElements; i++) {
		list.add(VASTARPData::mARPPattern[i].name);
	}
	return list;
}

VASTARPData::ArpStep VASTARPData::getStep(int step) {
	return arpSteps[step];
}

void VASTARPData::incGate(int step) {
	arpSteps[step].gate++;
	arpSteps[step].gate %= 5; //0..4
	m_isDirty.store(true);
}

void VASTARPData::setOctave(int step, int octave) {
	arpSteps[step].octave = octave;
    m_isDirty.store(true);
}

void VASTARPData::setSemitone(int step, int semitone) {
	arpSteps[step].semitones = semitone;
    m_isDirty.store(true);
}

void VASTARPData::setVelocity(int step, int velocity) {
	arpSteps[step].velocity = jlimit<int>(0, 127, velocity);
    m_isDirty.store(true);
}

void VASTARPData::addStep(ArpStep step) {
    m_isDirty.store(true);
	arpStepNum++;
	if (arpSteps.size() < getNumSteps())
		arpSteps.push_back(step);
}

void VASTARPData::removeStep(int delpos) {
	if ((delpos > 0) && (delpos <= getNumSteps()))
		arpStepNum = delpos;
		//but keep content
}

void VASTARPData::arpChangeSteps(int steps) {
	//const ScopedWriteLock myScopedLock(mReadWriteLock);

	int cursteps = getNumSteps();
	if (steps < cursteps) {
		//delete others		
		//for (int stp = 0; stp < (cursteps - steps); stp++) {
			removeStep(steps);
		//}
	}
	else if (steps > cursteps) {
		//adjust existing ones
		//new ones
		for (int stp = 0; stp < (steps - cursteps); stp++) {
			ArpStep step1;
			step1.octave = 0; //can be -2..2
			step1.semitones = 0; //can be -11 .. +11
			step1.gate = 3; // can be 0..4
			step1.velocity = 127; // can be 0..127 - midi values
			addStep(step1);
		}
	}
    m_isDirty.store(true);
}

//-------------------------------------------------------------------------------------------
// valueTree state
void VASTARPData::getValueTreeState(ValueTree* tree, UndoManager* undoManager) { //save
	//const ScopedReadLock myScopedLock(mReadWriteLock);

	tree->removeAllChildren(undoManager);
	tree->removeAllProperties(undoManager);

	tree->setProperty("patternName", patternName, undoManager);
	//steps
	tree->setProperty("numSteps", int(getNumSteps()), undoManager);
	for (int i = 0; i < arpSteps.size(); i++) {
        std::unique_ptr<ValueTree> subtree(new ValueTree(Identifier("arpStep"+String(i))));
		subtree->setProperty("octave", arpSteps[i].octave, undoManager);
		subtree->setProperty("semitones", arpSteps[i].semitones, undoManager);
		subtree->setProperty("gate", arpSteps[i].gate, undoManager);
		subtree->setProperty("velocity", arpSteps[i].velocity, undoManager);
		tree->appendChild(*subtree.get(), undoManager);
	}
}

void VASTARPData::setValueTreeState(ValueTree* tree) { //load
	//const ScopedWriteLock myScopedLock(mReadWriteLock);

	patternName = tree->getProperty("patternName");

	//points
	arpSteps.clear();
	arpStepNum = tree->getProperty("numSteps");
	for (int i = 0; i < arpStepNum; i++) {
		ValueTree subtree = tree->getChildWithName(Identifier("arpStep" + String(i)));
		vassert(subtree.isValid());
		ArpStep step;
		step.octave = subtree.getProperty("octave");
		step.semitones = subtree.getProperty("semitones");
		step.gate = subtree.getProperty("gate");
		step.velocity = subtree.getProperty("velocity");
		arpSteps.push_back(step); 
	}

	//Check old compatibility
	if (arpStepNum == 0) {
		initDefaultPattern(0);
	}

    m_isDirty.store(true);
}
//-------------------------------------------------------------------------------------------
