/*
VAST Dynamics Audio Software (TM)

Global settings for AUDIO THREAD!
*/

#include "VASTEngineHeader.h"
#include "VASTSettings.h"
#include "VASTPluginConstants.h"
#include "../Plugin/VASTAudioProcessor.h"
#include "../AnaMark-Tuning-Library/SCL_Import.h"
#include <cmath>
#include <string>

#ifdef _WINDOWS
	#include <time.h>
#else
	#include <sys/time.h>
#endif

//for debug info
#include <iostream>
#include <fstream>
#include <string>

CVASTSettings::CVASTSettings(VASTAudioProcessor* processor) : my_processor(processor) {
	m_bWriteDebug = false;
	m_bShallDump = false;
	m_fPitchBend = 1.0f; //middle
	CVASTSettings::loadCustomFonts();
	
	modMatrixInputState l_inputState{ 0, 0 };
	bufferInputState.store(l_inputState);

	for (int stepSeq = 0; stepSeq < 3; stepSeq++) {
		m_StepSeqData[stepSeq].initStepSeq(stepSeq);
		m_StepSeqData_changed[stepSeq].initStepSeq(stepSeq);
	}

	for (int i = 0; i < C_MAX_POLY; i++) {
		iAftertouch[i] = -1;
		iAftertouchMidiNote[i] = -1;
		iMPETimbre[i] = -1;
		iMPETimbreMidiNote[i] = -1;
	}
    
    //buffer white noise
	CVASTWaveTableOscillator wtosc(*this, nullptr);
    for (int i = 0; i < C_MAX_SAMPLE_RATE * 3; i++) {
        m_whiteNoiseBuffer[i] = wtosc.doWhiteNoiseFast();
    }

	//fill frequency lut
	for (int i = 0; i < M_CENTS_LUT_SIZE; i++)
		m_lookupTable_CentsFreqFactor[i] = powf(2.f, float(i) * 0.001f); // 2^x from 0.000 to 0.999 in 0.001 steps 
}

/* destructor()
Destroy variables allocated in the contructor()

*/
CVASTSettings::~CVASTSettings(void) {	
}

JUCE_BEGIN_IGNORE_WARNINGS_GCC_LIKE("-Wconversion")
JUCE_BEGIN_IGNORE_WARNINGS_MSVC(4244 4267)

float CVASTSettings::getFrequencyFactorFromLUT(float octave) {
	int intpart = floorf(octave);
	float fracpart = octave - intpart;
	jassert((intpart >= -11) && (intpart <= 11)); //m_lookupTable_OctaveFreqFactor[22] //-11...11	
	float factor = m_lookupTable_OctaveFreqFactor[intpart + 11] * m_lookupTable_CentsFreqFactor[int(fracpart * M_CENTS_LUT_SIZE)]; //-11...11
	return factor;
}

void CVASTSettings::sendStatusMessage(StringRef pText) {
	return;

#ifdef _WINDOWS // not on MAC
	#ifndef _DEBUG  //defined in project settings - praeprozessor
		return;
	#endif		
		juce::String writable;
		writable = pText;		
		writable += '\0'; // don't forget the terminating 0
		//VDBG(writable);
		
		if (m_bWriteDebug == true) {
			std::string filename = "C:\\Users\\d026198\\Desktop\\debuginfo.txt";

			std::ofstream debugfile(filename, std::ofstream::out | std::ofstream::app);
			if (debugfile.is_open())
			{
				debugfile << writable << std::endl;
			}
			debugfile.close();
		}
#endif
}

void CVASTSettings::sendStatusMessage(StringRef pText, int iDisp) {
	return;

#ifndef _DEBUG  //defined in project settings - praeprozessor
	return;
#endif

#ifndef _MACOSX
	String totalLine = pText + " : " + std::to_string(iDisp);
	sendStatusMessage(totalLine);

#endif
}

void CVASTSettings::sendStatusMessage(StringRef pText, int iDisp1, int iDisp2) {
	return;

#ifndef _DEBUG  //defined in project settings - praeprozessor
	return;
#endif

#ifndef _MACOSX
	String totalLine = pText + " : " + std::to_string(iDisp1) + " , " + std::to_string(iDisp2);
	sendStatusMessage(totalLine);
#endif
}

void CVASTSettings::sendStatusMessage(StringRef pText, int iDisp1, int iDisp2, int iDisp3) {
	return;

#ifndef _DEBUG  //defined in project settings - praeprozessor
	return;
#endif
	return;

#ifndef _MACOSX
	String totalLine = pText + " : " + std::to_string(iDisp1) + " , " + std::to_string(iDisp2) + " , " + std::to_string(iDisp3);
	sendStatusMessage(totalLine);
#endif
}


#if defined(_MSC_VER) || defined(_MSC_EXTENSIONS)
#define DELTA_EPOCH_IN_MICROSECS  11644473600000000Ui64
#else
#define DELTA_EPOCH_IN_MICROSECS  11644473600000000ULL
#endif

int CVASTSettings::_gettimeofday(struct timeval *tv)
{
#ifdef _WINDOWS
	FILETIME ft;
	ULong64_t tmpres = 0;
	//static int tzflag;

	if (NULL != tv)
	{
		GetSystemTimeAsFileTime(&ft);

		tmpres |= ft.dwHighDateTime;
		tmpres <<= 32;
		tmpres |= ft.dwLowDateTime;

		/*converting file time to unix epoch*/
		tmpres -= DELTA_EPOCH_IN_MICROSECS;
		tmpres /= 10;  /*convert into microseconds*/
		tv->tv_sec = (tmpres / 1000000UL);
		tv->tv_usec = (tmpres % 1000000UL);
	}
#else // MAC
	gettimeofday(tv, nullptr);
#endif

	return 0;
}

void CVASTSettings::setTuning(String tuningFile) {
	const char* sclFilepath = static_cast<const char*> (tuningFile.toUTF8());
	long res = m_scale.Read(sclFilepath);
	if (res != 1) { //1 means OK
		m_scale.Reset();
	}
}

float CVASTSettings::midiNoteGetBaseFreq(MYUINT uMIDINote, float oscMasterTune) {
	if (uMIDINote < 0)
		return 0.f;
	/*
	//original logic
	signed int key = uMIDINote;
	float divi = (key - 45.0 - (24)) / 12.0;
	float freq = powf(2.0, divi) * oscMasterTune;
	freq = (freq > m_nSampleRate - 1.0f) ? m_nSampleRate - 1.0f : freq;
	*/

	int iMidiNote = jlimit(0, 127, uMIDINote);
	float nfreq = m_scale.GetNoteFrequenciesHz()[iMidiNote];
	float freq = nfreq * (oscMasterTune / 440.f);
	return freq;
}

double CVASTSettings::getMillisecondsPerBeat() {
	double dawbpm = m_dPpqBpm.load();
	if (dawbpm < 3.0)
		dawbpm = 120.0; //standard value
	double millisecondsPerBeat = (1.0 / dawbpm) * 60.0 * 1000.0;
	return millisecondsPerBeat;
}

double CVASTSettings::getIntervalRatio(int beatindex) {
	double l_dRatio = 0.0;
	switch (beatindex) {
	case TIMEBEATS::BEATS1_256:
		l_dRatio = (1.0 / 256.0);
		break;
	case TIMEBEATS::BEATS1_128:
		l_dRatio = (1.0 / 128.0);
		break;
	case TIMEBEATS::BEATS1_64:
		l_dRatio = (1.0 / 64.0);
		break;
	case TIMEBEATS::BEATS1_32:
		l_dRatio = (1.0 / 32.0);
		break;
	case TIMEBEATS::BEATS1_16:
		l_dRatio = (1.0 / 16.0);
		break;
	case TIMEBEATS::BEATS1_9:
		l_dRatio = (1.0 / 9.0);
		break;
	case TIMEBEATS::BEATS1_8:
		l_dRatio = (1.0 / 8.0);
		break;
	case TIMEBEATS::BEATS1_4:
		l_dRatio = (1.0 / 4.0);
		break;
	case TIMEBEATS::BEATS1_3:
		l_dRatio = (1.0 / 3.0);
		break;
	case TIMEBEATS::BEATS1_2:
		l_dRatio = (1.0 / 2.0);
		break;
	case TIMEBEATS::BEATS1:
		l_dRatio = 1.0;
		break;
	case TIMEBEATS::BEATS3_2:
		l_dRatio = 1.5;
		break;
	case TIMEBEATS::BEATS2:
		l_dRatio = 2.0;
		break;
	case TIMEBEATS::BEATS3:
		l_dRatio = 3.0;
		break;
	case TIMEBEATS::BEATS4:
		l_dRatio = 4.0;
		break;
	case TIMEBEATS::BEATS8:
		l_dRatio = 8.0;
		break;
	case TIMEBEATS::BEATS16:
		l_dRatio = 16.0;
		break;
	case TIMEBEATS::BEATS32:
		l_dRatio = 32.0;
		break;
	case TIMEBEATS::BEATS64:
		l_dRatio = 64.0;
		break;
	}
	return l_dRatio;
}

double CVASTSettings::getIntervalTimeFromDAWBeats(int beatindex) {
	double l_dIntervalTime = 0.f;
	double millisecondsPerBeat = getMillisecondsPerBeat();
	l_dIntervalTime = millisecondsPerBeat * getIntervalRatio(beatindex);
	return l_dIntervalTime;
}

void CVASTSettings::initModMatrix() {
	for (int slot = 0; slot < M_MODMATRIX_MAX_SLOTS; slot++) {
		initModMatrixValueLookupTable(slot);
		m_fDriftLfoSlow[slot] = 0.f;
		m_fDriftLfoFast[slot] = 0.f;
		for (int slotSource = 0; slotSource < M_MODMATRIX_MAX_SOURCES; slotSource++)
			for (int voice= 0; voice < C_MAX_POLY; voice++)
			lastModMatrixSourceVal[slot][slotSource][voice] = 0.f;
	}
	modMatrixCalcBuffers();
}

void CVASTSettings::modMatrixCalcBuffers() {
	for (int slot = 0; slot < M_MODMATRIX_MAX_SLOTS; slot++) {
		int slotSource = 0;
		int slotDestination = 0;
		float slotValue = 0;
		double slotCurvy = 0;
		float lastSrceVals[C_MAX_POLY] {};
		int slotPolarity = 0;
		modMatrixSlotGetValues(slot, slotValue, slotCurvy, slotSource, slotDestination, slotPolarity, lastSrceVals);
		if ((slotSource != MODMATSRCE::NoSource) && (slotDestination != MODMATDEST::NoDestination)) //val can be 0? Check
			modMatrixSlotUsed[slot] = true;
		else
            modMatrixSlotUsed[slot] = false;
		modMatrixSlotDest[slot] = slotDestination; //onl for UI update
	}

	for (int j = 0; j < M_MODMATRIX_MAX_DESTINATIONS; j++) {
		modMatrixDestSet[j] = modMatrixDestinationSet(j);
	}
	for (int j = 0; j < M_MODMATRIX_MAX_SOURCES; j++) {
		modMatrixSrceSet[j] = modMatrixSourceSet(j);
	}
}

int CVASTSettings::modMatrixFindFirstFreeSlot() { //slot is 0...15
	for (int slot = 0; slot < M_MODMATRIX_MAX_SLOTS; slot++) {
		if (modMatrixSlotUsed[slot] == false)
			return slot;
	}
	return -1;
}

void CVASTSettings::modMatrixSwapSlots(int slot1, int slot2) {
	float slot1Value = 0;
	double slot1Curvy = 0;
	int slot1Source = 0;
	int slot1Destination = 0;
	int slot1Polarity = 0;
	float slot1LasSrceVals[C_MAX_POLY] {};
	float slot2Value = 0;
	double slot2Curvy = 0;
	int slot2Source = 0;
	int slot2Destination = 0;
	int slot2Polarity = 0;
	float slot2LasSrceVals[C_MAX_POLY] {};
	modMatrixSlotGetValues(slot1, slot1Value, slot1Curvy, slot1Source, slot1Destination, slot1Polarity, slot1LasSrceVals);
	modMatrixSlotGetValues(slot2, slot2Value, slot2Curvy, slot2Source, slot2Destination, slot2Polarity, slot2LasSrceVals);

	my_processor->setParameterText("m_uModMatDest" + String(slot1 + 1), CVASTParamState::comboBoxValueToTextFunction_MODMATDEST(slot2Destination), false);
	my_processor->setParameterText("m_uModMatSrce" + String(slot1 + 1), CVASTParamState::comboBoxValueToTextFunction_MODMATSRCE(slot2Source), false);
	my_processor->setParameterText("m_fModMatVal" + String(slot1 + 1), String(slot2Value), false);
	my_processor->setParameterText("m_fModMatCurve" + String(slot1 + 1), String(slot2Curvy), false);
	my_processor->setParameterText("m_uModMatPolarity" + String(slot1 + 1), CVASTParamState::comboBoxValueToTextFunction_POLARITY(slot2Polarity), false);

	my_processor->setParameterText("m_uModMatDest" + String(slot2 + 1), CVASTParamState::comboBoxValueToTextFunction_MODMATDEST(slot1Destination), false);
	my_processor->setParameterText("m_uModMatSrce" + String(slot2 + 1), CVASTParamState::comboBoxValueToTextFunction_MODMATSRCE(slot1Source), false);
	my_processor->setParameterText("m_fModMatVal" + String(slot2 + 1), String(slot1Value), false);
	my_processor->setParameterText("m_fModMatCurve" + String(slot2 + 1), String(slot1Curvy), false);
	my_processor->setParameterText("m_uModMatPolarity" + String(slot2 + 1), CVASTParamState::comboBoxValueToTextFunction_POLARITY(slot1Polarity), false);
}

bool CVASTSettings::modMatrixIsSet(MYUINT source, MYUINT destination) {
	for (int slot = 0; slot < M_MODMATRIX_MAX_SLOTS; slot++) {
		if (modMatrixSlotUsed[slot] == true) {
			int slotSource = 0;
			int slotDestination = 0;
			float slotValue = 0;
			double slotCurvy = 0;
			float lastSrceVals[C_MAX_POLY] {};
			int slotPolarity = 0;
			modMatrixSlotGetValues(slot, slotValue, slotCurvy, slotSource, slotDestination, slotPolarity, lastSrceVals);
			if ((slotSource == source) && (slotDestination == destination))
				return true;
		}
	}	
	return false;
}

int CVASTSettings::modMatrixGetFirstSlotWithDestination(MYUINT destination) { //slot is 0...15
	for (int slot = 0; slot < M_MODMATRIX_MAX_SLOTS; slot++) {
		if (modMatrixSlotUsed[slot] == true) {
			int slotSource = 0;
			int slotDestination = 0;
			float slotValue = 0;
			double slotCurvy = 0;
			float lastSrceVals[C_MAX_POLY] {};
			int slotPolarity = 0;
			modMatrixSlotGetValues(slot, slotValue, slotCurvy, slotSource, slotDestination, slotPolarity, lastSrceVals);
			if (slotDestination == destination)
				return slot;
		}
	}
	return -1;
}

void CVASTSettings::modMatrixSlotGetValues(int slot, float &value, double &curvy, int &polarity, float (&lastSrceVals)[C_MAX_POLY]) { //slot is 0...15
	int source = 0;
	int destination = 0;
	modMatrixSlotGetValues(slot, value, curvy, source, destination, polarity, lastSrceVals);
}

void CVASTSettings::initModMatrixValueLookupTable(int slot) {
	switch (slot) {
	case 0:
		modMatrixValueLookupTable[slot].value = m_State->m_fModMatVal1;
		modMatrixValueLookupTable[slot].curvy = m_State->m_fModMatCurve1;
		modMatrixValueLookupTable[slot].source = m_State->m_uModMatSrce1;
		modMatrixValueLookupTable[slot].destination = m_State->m_uModMatDest1;
		modMatrixValueLookupTable[slot].polarity = m_State->m_uModMatPolarity1;
		break;
	case 1:
		modMatrixValueLookupTable[slot].value = m_State->m_fModMatVal2;
		modMatrixValueLookupTable[slot].curvy = m_State->m_fModMatCurve2;
		modMatrixValueLookupTable[slot].source = m_State->m_uModMatSrce2;
		modMatrixValueLookupTable[slot].destination = m_State->m_uModMatDest2;
		modMatrixValueLookupTable[slot].polarity = m_State->m_uModMatPolarity2;
		break;
	case 2:
		modMatrixValueLookupTable[slot].value = m_State->m_fModMatVal3;
		modMatrixValueLookupTable[slot].curvy = m_State->m_fModMatCurve3;
		modMatrixValueLookupTable[slot].source = m_State->m_uModMatSrce3;
		modMatrixValueLookupTable[slot].destination = m_State->m_uModMatDest3;
		modMatrixValueLookupTable[slot].polarity = m_State->m_uModMatPolarity3;
		break;
	case 3:
		modMatrixValueLookupTable[slot].value = m_State->m_fModMatVal4;
		modMatrixValueLookupTable[slot].curvy = m_State->m_fModMatCurve4;
		modMatrixValueLookupTable[slot].source = m_State->m_uModMatSrce4;
		modMatrixValueLookupTable[slot].destination = m_State->m_uModMatDest4;
		modMatrixValueLookupTable[slot].polarity = m_State->m_uModMatPolarity4;
		break;
	case 4:
		modMatrixValueLookupTable[slot].value = m_State->m_fModMatVal5;
		modMatrixValueLookupTable[slot].curvy = m_State->m_fModMatCurve5;
		modMatrixValueLookupTable[slot].source = m_State->m_uModMatSrce5;
		modMatrixValueLookupTable[slot].destination = m_State->m_uModMatDest5;
		modMatrixValueLookupTable[slot].polarity = m_State->m_uModMatPolarity5;
		break;
	case 5:
		modMatrixValueLookupTable[slot].value = m_State->m_fModMatVal6;
		modMatrixValueLookupTable[slot].curvy = m_State->m_fModMatCurve6;
		modMatrixValueLookupTable[slot].source = m_State->m_uModMatSrce6;
		modMatrixValueLookupTable[slot].destination = m_State->m_uModMatDest6;
		modMatrixValueLookupTable[slot].polarity = m_State->m_uModMatPolarity6;
		break;
	case 6:
		modMatrixValueLookupTable[slot].value = m_State->m_fModMatVal7;
		modMatrixValueLookupTable[slot].curvy = m_State->m_fModMatCurve7;
		modMatrixValueLookupTable[slot].source = m_State->m_uModMatSrce7;
		modMatrixValueLookupTable[slot].destination = m_State->m_uModMatDest7;
		modMatrixValueLookupTable[slot].polarity = m_State->m_uModMatPolarity7;
		break;
	case 7:
		modMatrixValueLookupTable[slot].value = m_State->m_fModMatVal8;
		modMatrixValueLookupTable[slot].curvy = m_State->m_fModMatCurve8;
		modMatrixValueLookupTable[slot].source = m_State->m_uModMatSrce8;
		modMatrixValueLookupTable[slot].destination = m_State->m_uModMatDest8;
		modMatrixValueLookupTable[slot].polarity = m_State->m_uModMatPolarity8;
		break;
	case 8:
		modMatrixValueLookupTable[slot].value = m_State->m_fModMatVal9;
		modMatrixValueLookupTable[slot].curvy = m_State->m_fModMatCurve9;
		modMatrixValueLookupTable[slot].source = m_State->m_uModMatSrce9;
		modMatrixValueLookupTable[slot].destination = m_State->m_uModMatDest9;
		modMatrixValueLookupTable[slot].polarity = m_State->m_uModMatPolarity9;
		break;
	case 9:
		modMatrixValueLookupTable[slot].value = m_State->m_fModMatVal10;
		modMatrixValueLookupTable[slot].curvy = m_State->m_fModMatCurve10;
		modMatrixValueLookupTable[slot].source = m_State->m_uModMatSrce10;
		modMatrixValueLookupTable[slot].destination = m_State->m_uModMatDest10;
		modMatrixValueLookupTable[slot].polarity = m_State->m_uModMatPolarity10;
		break;
	case 10:
		modMatrixValueLookupTable[slot].value = m_State->m_fModMatVal11;
		modMatrixValueLookupTable[slot].curvy = m_State->m_fModMatCurve11;
		modMatrixValueLookupTable[slot].source = m_State->m_uModMatSrce11;
		modMatrixValueLookupTable[slot].destination = m_State->m_uModMatDest11;
		modMatrixValueLookupTable[slot].polarity = m_State->m_uModMatPolarity11;
		break;
	case 11:
		modMatrixValueLookupTable[slot].value = m_State->m_fModMatVal12;
		modMatrixValueLookupTable[slot].curvy = m_State->m_fModMatCurve12;
		modMatrixValueLookupTable[slot].source = m_State->m_uModMatSrce12;
		modMatrixValueLookupTable[slot].destination = m_State->m_uModMatDest12;
		modMatrixValueLookupTable[slot].polarity = m_State->m_uModMatPolarity12;
		break;
	case 12:
		modMatrixValueLookupTable[slot].value = m_State->m_fModMatVal13;
		modMatrixValueLookupTable[slot].curvy = m_State->m_fModMatCurve13;
		modMatrixValueLookupTable[slot].source = m_State->m_uModMatSrce13;
		modMatrixValueLookupTable[slot].destination = m_State->m_uModMatDest13;
		modMatrixValueLookupTable[slot].polarity = m_State->m_uModMatPolarity13;
		break;
	case 13:
		modMatrixValueLookupTable[slot].value = m_State->m_fModMatVal14;
		modMatrixValueLookupTable[slot].curvy = m_State->m_fModMatCurve14;
		modMatrixValueLookupTable[slot].source = m_State->m_uModMatSrce14;
		modMatrixValueLookupTable[slot].destination = m_State->m_uModMatDest14;
		modMatrixValueLookupTable[slot].polarity = m_State->m_uModMatPolarity14;
		break;
	case 14:
		modMatrixValueLookupTable[slot].value = m_State->m_fModMatVal15;
		modMatrixValueLookupTable[slot].curvy = m_State->m_fModMatCurve15;
		modMatrixValueLookupTable[slot].source = m_State->m_uModMatSrce15;
		modMatrixValueLookupTable[slot].destination = m_State->m_uModMatDest15;
		modMatrixValueLookupTable[slot].polarity = m_State->m_uModMatPolarity15;
		break;
	case 15:
		modMatrixValueLookupTable[slot].value = m_State->m_fModMatVal16;
		modMatrixValueLookupTable[slot].curvy = m_State->m_fModMatCurve16;
		modMatrixValueLookupTable[slot].source = m_State->m_uModMatSrce16;
		modMatrixValueLookupTable[slot].destination = m_State->m_uModMatDest16;
		modMatrixValueLookupTable[slot].polarity = m_State->m_uModMatPolarity16;
		break;
	}	
}

void CVASTSettings::modMatrixSlotGetValues(int slot, float &value, double &curvy, int &source, int &destination, int &polarity, float(&lastSrceVals)[C_MAX_POLY]) { //slot is 0...15	
	value = *modMatrixValueLookupTable[slot].value;
	curvy = *modMatrixValueLookupTable[slot].curvy;
	source = *modMatrixValueLookupTable[slot].source;
	destination = *modMatrixValueLookupTable[slot].destination;
	polarity = *modMatrixValueLookupTable[slot].polarity;
	std::copy(std::begin(lastModMatrixSourceVal[slot][source]), std::end(lastModMatrixSourceVal[slot][source]), std::begin(lastSrceVals));
	
	return;
}

void CVASTSettings::modMatrixSlotGetValuesWithMod(int slot, float &slotValue, float &modValue, double &curvy, int &source, int &destination, int &polarity, modMatrixInputState* matrixInputState) {
	slotValue = *modMatrixValueLookupTable[slot].value;	
	curvy = *modMatrixValueLookupTable[slot].curvy;
	source = *modMatrixValueLookupTable[slot].source;
	destination = *modMatrixValueLookupTable[slot].destination;
	polarity = *modMatrixValueLookupTable[slot].polarity;
	
	modValue = getParameterValueWithMatrixModulation(modMatrixValueLookupTable[slot].value, MODMATDEST::ModMatrixValue1 + slot, matrixInputState); //CHECK
	//modValue = getParameterValueWithMatrixModulation(m_State->m_fModMatVal1, MODMATDEST::ModMatrixValue1, matrixInputState);	
}

bool CVASTSettings::modMatrixSourceSet(MYUINT source) {
	if (((*m_State->m_uModMatDest1 != static_cast<int>(MODMATDEST::NoDestination)) && (*m_State->m_uModMatSrce1 == source) && (*m_State->m_fModMatVal1 != 0.0f)) ||
		((*m_State->m_uModMatDest2 != static_cast<int>(MODMATDEST::NoDestination)) && (*m_State->m_uModMatSrce2 == source) && (*m_State->m_fModMatVal2 != 0.0f)) ||
		((*m_State->m_uModMatDest3 != static_cast<int>(MODMATDEST::NoDestination)) && (*m_State->m_uModMatSrce3 == source) && (*m_State->m_fModMatVal3 != 0.0f)) ||
		((*m_State->m_uModMatDest4 != static_cast<int>(MODMATDEST::NoDestination)) && (*m_State->m_uModMatSrce4 == source) && (*m_State->m_fModMatVal4 != 0.0f)) ||
		((*m_State->m_uModMatDest5 != static_cast<int>(MODMATDEST::NoDestination)) && (*m_State->m_uModMatSrce5 == source) && (*m_State->m_fModMatVal5 != 0.0f)) ||
		((*m_State->m_uModMatDest6 != static_cast<int>(MODMATDEST::NoDestination)) && (*m_State->m_uModMatSrce6 == source) && (*m_State->m_fModMatVal6 != 0.0f)) ||
		((*m_State->m_uModMatDest7 != static_cast<int>(MODMATDEST::NoDestination)) && (*m_State->m_uModMatSrce7 == source) && (*m_State->m_fModMatVal7 != 0.0f)) ||
		((*m_State->m_uModMatDest8 != static_cast<int>(MODMATDEST::NoDestination)) && (*m_State->m_uModMatSrce8 == source) && (*m_State->m_fModMatVal8 != 0.0f)) ||
		((*m_State->m_uModMatDest9 != static_cast<int>(MODMATDEST::NoDestination)) && (*m_State->m_uModMatSrce9 == source) && (*m_State->m_fModMatVal9 != 0.0f)) ||
		((*m_State->m_uModMatDest10 != static_cast<int>(MODMATDEST::NoDestination)) && (*m_State->m_uModMatSrce10 == source) && (*m_State->m_fModMatVal10 != 0.0f)) ||
		((*m_State->m_uModMatDest11 != static_cast<int>(MODMATDEST::NoDestination)) && (*m_State->m_uModMatSrce11 == source) && (*m_State->m_fModMatVal11 != 0.0f)) ||
		((*m_State->m_uModMatDest12 != static_cast<int>(MODMATDEST::NoDestination)) && (*m_State->m_uModMatSrce12 == source) && (*m_State->m_fModMatVal12 != 0.0f)) ||
		((*m_State->m_uModMatDest13 != static_cast<int>(MODMATDEST::NoDestination)) && (*m_State->m_uModMatSrce13 == source) && (*m_State->m_fModMatVal13 != 0.0f)) ||
		((*m_State->m_uModMatDest14 != static_cast<int>(MODMATDEST::NoDestination)) && (*m_State->m_uModMatSrce14 == source) && (*m_State->m_fModMatVal14 != 0.0f)) ||
		((*m_State->m_uModMatDest15 != static_cast<int>(MODMATDEST::NoDestination)) && (*m_State->m_uModMatSrce15 == source) && (*m_State->m_fModMatVal15 != 0.0f)) ||
		((*m_State->m_uModMatDest16 != static_cast<int>(MODMATDEST::NoDestination)) && (*m_State->m_uModMatSrce16 == source) && (*m_State->m_fModMatVal16 != 0.0f)))
		return true;
	return false;
}

void CVASTSettings::modMatrixRemoveSlot(int slot) {
	my_processor->setParameterText("m_uModMatDest" + String(slot + 1), CVASTParamState::comboBoxValueToTextFunction_MODMATDEST(MODMATDEST::NoDestination), false);
	my_processor->setParameterText("m_uModMatSrce" + String(slot + 1), CVASTParamState::comboBoxValueToTextFunction_MODMATSRCE(MODMATSRCE::NoSource), false);
	my_processor->setParameterText("m_fModMatVal" + String(slot + 1), "0.0", false);
	my_processor->setParameterText("m_fModMatCurve" + String(slot + 1), "0.0", false);
	my_processor->setParameterText("m_uModMatPolarity" + String(slot + 1), CVASTParamState::comboBoxValueToTextFunction_POLARITY(POLARITY::Bipolar), false);
	modMatrixCalcBuffers();
}

void CVASTSettings::modMatrixRemoveAll() {
	for (int slot = 0; slot < 16; slot++) {
		my_processor->setParameterText("m_uModMatDest" + String(slot + 1), CVASTParamState::comboBoxValueToTextFunction_MODMATDEST(MODMATDEST::NoDestination), false);
		my_processor->setParameterText("m_uModMatSrce" + String(slot + 1), CVASTParamState::comboBoxValueToTextFunction_MODMATSRCE(MODMATSRCE::NoSource), false);
		my_processor->setParameterText("m_fModMatVal" + String(slot + 1), "0.0", false);
		my_processor->setParameterText("m_fModMatCurve" + String(slot + 1), "0.0", false);
		my_processor->setParameterText("m_uModMatPolarity" + String(slot + 1), CVASTParamState::comboBoxValueToTextFunction_POLARITY(POLARITY::Bipolar), false);
	}
	modMatrixCalcBuffers();
}

void CVASTSettings::modMatrixRemoveAllForDestination(MYUINT destination) {
	for (int slot = 0; slot < 16; slot++) {
		bool clearIt = false;
		switch(slot) {
		case 0: if (*m_State->m_uModMatDest1 == destination) clearIt = true; break;
		case 1: if (*m_State->m_uModMatDest2 == destination) clearIt = true; break;
		case 2: if (*m_State->m_uModMatDest3 == destination) clearIt = true; break;
		case 3: if (*m_State->m_uModMatDest4 == destination) clearIt = true; break;
		case 4: if (*m_State->m_uModMatDest5 == destination) clearIt = true; break;
		case 5: if (*m_State->m_uModMatDest6 == destination) clearIt = true; break;
		case 6: if (*m_State->m_uModMatDest7 == destination) clearIt = true; break;
		case 7: if (*m_State->m_uModMatDest8 == destination) clearIt = true; break;
		case 8: if (*m_State->m_uModMatDest9 == destination) clearIt = true; break;
		case 9: if (*m_State->m_uModMatDest10 == destination) clearIt = true; break;
		case 10: if (*m_State->m_uModMatDest11 == destination) clearIt = true; break;
		case 11: if (*m_State->m_uModMatDest12 == destination) clearIt = true; break;
		case 12: if (*m_State->m_uModMatDest13 == destination) clearIt = true; break;
		case 13: if (*m_State->m_uModMatDest14 == destination) clearIt = true; break;
		case 14: if (*m_State->m_uModMatDest15 == destination) clearIt = true; break;
		case 15: if (*m_State->m_uModMatDest16 == destination) clearIt = true; break;
		}
		if (clearIt) {
			my_processor->setParameterText("m_uModMatDest" + String(slot + 1), CVASTParamState::comboBoxValueToTextFunction_MODMATDEST(MODMATDEST::NoDestination), false);
			my_processor->setParameterText("m_uModMatSrce" + String(slot + 1), CVASTParamState::comboBoxValueToTextFunction_MODMATSRCE(MODMATSRCE::NoSource), false);
			my_processor->setParameterText("m_fModMatVal" + String(slot + 1), "0.0", false);
			my_processor->setParameterText("m_fModMatCurve" + String(slot + 1), "0.0", false);
			my_processor->setParameterText("m_uModMatPolarity" + String(slot + 1), CVASTParamState::comboBoxValueToTextFunction_POLARITY(POLARITY::Bipolar), false);
		}
	}
	modMatrixCalcBuffers();
}

bool CVASTSettings::modMatrixDestinationSetFast(MYUINT destination) {
	return modMatrixDestSet[destination];
}

bool CVASTSettings::modMatrixSourceSetFast(MYUINT source) {
	return modMatrixSrceSet[source];
}

bool CVASTSettings::modMatrixDestinationSet(MYUINT destination) {
	if (((*m_State->m_uModMatSrce1 != static_cast<int>(MODMATSRCE::NoSource)) && (*m_State->m_uModMatDest1 == destination) && (*m_State->m_fModMatVal1 != 0.0f)) ||
		((*m_State->m_uModMatSrce2 != static_cast<int>(MODMATSRCE::NoSource)) && (*m_State->m_uModMatDest2 == destination) && (*m_State->m_fModMatVal2 != 0.0f)) ||
		((*m_State->m_uModMatSrce3 != static_cast<int>(MODMATSRCE::NoSource)) && (*m_State->m_uModMatDest3 == destination) && (*m_State->m_fModMatVal3 != 0.0f)) ||
		((*m_State->m_uModMatSrce4 != static_cast<int>(MODMATSRCE::NoSource)) && (*m_State->m_uModMatDest4 == destination) && (*m_State->m_fModMatVal4 != 0.0f)) ||
		((*m_State->m_uModMatSrce5 != static_cast<int>(MODMATSRCE::NoSource)) && (*m_State->m_uModMatDest5 == destination) && (*m_State->m_fModMatVal5 != 0.0f)) ||
		((*m_State->m_uModMatSrce6 != static_cast<int>(MODMATSRCE::NoSource)) && (*m_State->m_uModMatDest6 == destination) && (*m_State->m_fModMatVal6 != 0.0f)) ||
		((*m_State->m_uModMatSrce7 != static_cast<int>(MODMATSRCE::NoSource)) && (*m_State->m_uModMatDest7 == destination) && (*m_State->m_fModMatVal7 != 0.0f)) ||
		((*m_State->m_uModMatSrce8 != static_cast<int>(MODMATSRCE::NoSource)) && (*m_State->m_uModMatDest8 == destination) && (*m_State->m_fModMatVal8 != 0.0f)) ||
		((*m_State->m_uModMatSrce9 != static_cast<int>(MODMATSRCE::NoSource)) && (*m_State->m_uModMatDest9 == destination) && (*m_State->m_fModMatVal9 != 0.0f)) ||
		((*m_State->m_uModMatSrce10 != static_cast<int>(MODMATSRCE::NoSource)) && (*m_State->m_uModMatDest10 == destination) && (*m_State->m_fModMatVal10 != 0.0f)) ||
		((*m_State->m_uModMatSrce11 != static_cast<int>(MODMATSRCE::NoSource)) && (*m_State->m_uModMatDest11 == destination) && (*m_State->m_fModMatVal11 != 0.0f)) ||
		((*m_State->m_uModMatSrce12 != static_cast<int>(MODMATSRCE::NoSource)) && (*m_State->m_uModMatDest12 == destination) && (*m_State->m_fModMatVal12 != 0.0f)) ||
		((*m_State->m_uModMatSrce13 != static_cast<int>(MODMATSRCE::NoSource)) && (*m_State->m_uModMatDest13 == destination) && (*m_State->m_fModMatVal13 != 0.0f)) ||
		((*m_State->m_uModMatSrce14 != static_cast<int>(MODMATSRCE::NoSource)) && (*m_State->m_uModMatDest14 == destination) && (*m_State->m_fModMatVal14 != 0.0f)) ||
		((*m_State->m_uModMatSrce15 != static_cast<int>(MODMATSRCE::NoSource)) && (*m_State->m_uModMatDest15 == destination) && (*m_State->m_fModMatVal15 != 0.0f)) ||
		((*m_State->m_uModMatSrce16 != static_cast<int>(MODMATSRCE::NoSource)) && (*m_State->m_uModMatDest16 == destination) && (*m_State->m_fModMatVal16 != 0.0f)))
		return true;
	return false;
}

//============================================================================================================
void CVASTSettings::getMatrixModulationSlotMultiplier(int slot, float& modVal, float& multiplier, int& slotPolarity, MYUINT destination, modMatrixInputState* matrixInputState) { //modVal -1..1	
	int slotSource = 0;
	int slotDestination = 0;
	float slotVal = 0.f;
	double curvy = 0.0; //-100 .. 100
	float lastSrceVals[C_MAX_POLY] {};
	modMatrixSlotGetValues(slot, slotVal, curvy, slotSource, slotDestination, slotPolarity, lastSrceVals); //this includes modmatval modulations!
	if (destination != slotDestination) {
		modVal = 0.f;  return;
	}
	modMatrixSlotGetValuesWithMod(slot, slotVal, modVal, curvy, slotSource, slotDestination, slotPolarity, matrixInputState); //this includes modmatval modulations!
	curvy = (100.f + curvy) * 0.005f; //0..1	
	float sourceVal = 0.f;

	if (slotVal != 0.0f) {
		switch (slotSource) {
		case MODMATSRCE::MSEG1Env: {
			sourceVal = m_RoutingBuffers.MSEGBuffer[0][matrixInputState->voiceNo]->getSample(0, matrixInputState->currentFrame); //0..1
			if (*m_State->m_uMSEGPolarity_MSEG1 == static_cast<int>(POLARITY::Bipolar))
				sourceVal = (sourceVal - 0.5f) * 2.f; //-1..1	
			break;
		}
		case MODMATSRCE::MSEG2Env: {
			sourceVal = m_RoutingBuffers.MSEGBuffer[1][matrixInputState->voiceNo]->getSample(0, matrixInputState->currentFrame); //0..1
			if (*m_State->m_uMSEGPolarity_MSEG2 == static_cast<int>(POLARITY::Bipolar))
				sourceVal = (sourceVal - 0.5f) * 2.f; //-1..1	
			break;
		}
		case MODMATSRCE::MSEG3Env: {
			sourceVal = m_RoutingBuffers.MSEGBuffer[2][matrixInputState->voiceNo]->getSample(0, matrixInputState->currentFrame); //0..1
			if (*m_State->m_uMSEGPolarity_MSEG3 == static_cast<int>(POLARITY::Bipolar))
				sourceVal = (sourceVal - 0.5f) * 2.f; //-1..1	
			break;
		}
		case MODMATSRCE::MSEG4Env: {
			sourceVal = m_RoutingBuffers.MSEGBuffer[3][matrixInputState->voiceNo]->getSample(0, matrixInputState->currentFrame); //0..1
			if (*m_State->m_uMSEGPolarity_MSEG4 == static_cast<int>(POLARITY::Bipolar))
				sourceVal = (sourceVal - 0.5f) * 2.f; //-1..1	
			break;
		}
		case MODMATSRCE::MSEG5Env: {
			sourceVal = m_RoutingBuffers.MSEGBuffer[4][matrixInputState->voiceNo]->getSample(0, matrixInputState->currentFrame); //0..1
			if (*m_State->m_uMSEGPolarity_MSEG5 == static_cast<int>(POLARITY::Bipolar))
				sourceVal = (sourceVal - 0.5f) * 2.f; //-1..1	
			break;
		}
		case MODMATSRCE::LFO1: {
			if (*m_State->m_bLFOPerVoice_LFO1 == static_cast<int>(SWITCH::SWITCH_ON))
				sourceVal = m_RoutingBuffers.LFOBuffer[0][matrixInputState->voiceNo]->getSample(0, matrixInputState->currentFrame); //-1 ..1 				
			else
				sourceVal = m_RoutingBuffers.LFOGlobalBuffer[0]->getSample(0, matrixInputState->currentFrame); //-1 ..1
			if (*m_State->m_uLFOPolarity_LFO1 == static_cast<int>(POLARITY::Unipolar))
				sourceVal = (sourceVal + 1.f) * 0.5f; //0..1				
			break;
		}
		case MODMATSRCE::LFO2: {
			if (*m_State->m_bLFOPerVoice_LFO2 == static_cast<int>(SWITCH::SWITCH_ON))
				sourceVal = m_RoutingBuffers.LFOBuffer[1][matrixInputState->voiceNo]->getSample(0, matrixInputState->currentFrame); //-1 ..1 				
			else
				sourceVal = m_RoutingBuffers.LFOGlobalBuffer[1]->getSample(0, matrixInputState->currentFrame); //-1 ..1
			if (*m_State->m_uLFOPolarity_LFO2 == static_cast<int>(POLARITY::Unipolar))
				sourceVal = (sourceVal + 1.f) * 0.5f; //0..1				
			break;
		}
		case MODMATSRCE::LFO3: {
			if (*m_State->m_bLFOPerVoice_LFO3 == static_cast<int>(SWITCH::SWITCH_ON))
				sourceVal = m_RoutingBuffers.LFOBuffer[2][matrixInputState->voiceNo]->getSample(0, matrixInputState->currentFrame); //-1 ..1 				
			else
				sourceVal = m_RoutingBuffers.LFOGlobalBuffer[2]->getSample(0, matrixInputState->currentFrame); //-1 ..1
			if (*m_State->m_uLFOPolarity_LFO3 == static_cast<int>(POLARITY::Unipolar))
				sourceVal = (sourceVal + 1.f) * 0.5f; //0..1				
			break;
		}
		case MODMATSRCE::LFO4: {
			if (*m_State->m_bLFOPerVoice_LFO4 == static_cast<int>(SWITCH::SWITCH_ON))
				sourceVal = m_RoutingBuffers.LFOBuffer[3][matrixInputState->voiceNo]->getSample(0, matrixInputState->currentFrame); //-1 ..1 				
			else
				sourceVal = m_RoutingBuffers.LFOGlobalBuffer[3]->getSample(0, matrixInputState->currentFrame); //-1 ..1
			if (*m_State->m_uLFOPolarity_LFO4 == static_cast<int>(POLARITY::Unipolar))
				sourceVal = (sourceVal + 1.f) * 0.5f; //0..1				
			break;
		}
		case MODMATSRCE::LFO5: {
			if (*m_State->m_bLFOPerVoice_LFO5 == static_cast<int>(SWITCH::SWITCH_ON))
				sourceVal = m_RoutingBuffers.LFOBuffer[4][matrixInputState->voiceNo]->getSample(0, matrixInputState->currentFrame); //-1 ..1 				
			else
				sourceVal = m_RoutingBuffers.LFOGlobalBuffer[4]->getSample(0, matrixInputState->currentFrame); //-1 ..1
			if (*m_State->m_uLFOPolarity_LFO5 == static_cast<int>(POLARITY::Unipolar))
				sourceVal = (sourceVal + 1.f) * 0.5f; //0..1				
			break;
		}
		case MODMATSRCE::StepSeq1: {
			sourceVal = m_RoutingBuffers.StepSeqBuffer[0]->getSample(0, matrixInputState->currentFrame); //0..1		
			sourceVal = (sourceVal - 0.5f) * 2.f; //-1..1	
			break;
		}
		case MODMATSRCE::StepSeq2: {
			sourceVal = m_RoutingBuffers.StepSeqBuffer[1]->getSample(0, matrixInputState->currentFrame); //0..1		
			sourceVal = (sourceVal - 0.5f) * 2.f; //-1..1	
			break;
		}
		case MODMATSRCE::StepSeq3: {
			sourceVal = m_RoutingBuffers.StepSeqBuffer[2]->getSample(0, matrixInputState->currentFrame); //0..1		
			sourceVal = (sourceVal - 0.5f) * 2.f; //-1..1	
			break;
		}
		case MODMATSRCE::CustomModulator1: {
			sourceVal = m_RoutingBuffers.CustomModulatorBuffer[0]->getSample(0, matrixInputState->currentFrame); //0..1		
			sourceVal = (sourceVal - 0.5f) * 2.f; //-1..1	
			break;
		}
		case MODMATSRCE::CustomModulator2: {
			sourceVal = m_RoutingBuffers.CustomModulatorBuffer[1]->getSample(0, matrixInputState->currentFrame); //0..1		
			sourceVal = (sourceVal - 0.5f) * 2.f; //-1..1	
			break;
		}
		case MODMATSRCE::CustomModulator3: {
			sourceVal = m_RoutingBuffers.CustomModulatorBuffer[2]->getSample(0, matrixInputState->currentFrame); //0..1		
			sourceVal = (sourceVal - 0.5f) * 2.f; //-1..1	
			break;
		}
		case MODMATSRCE::CustomModulator4: {
			sourceVal = m_RoutingBuffers.CustomModulatorBuffer[3]->getSample(0, matrixInputState->currentFrame); //0..1		
			sourceVal = (sourceVal - 0.5f) * 2.f; //-1..1	
			break;
		}

		case MODMATSRCE::InputWave: {
			//not used
			sourceVal = m_RoutingBuffers.fAudioInputBuffer->getWritePointer(0)[matrixInputState->currentFrame]; //-1..1
			sourceVal = (sourceVal > 1.0f) ? 1.0f : sourceVal; //clip it
			sourceVal = (sourceVal < -1.0f) ? -1.0f : sourceVal;
			break;
		}
		case MODMATSRCE::InputEnvelope: {
			sourceVal = m_RoutingBuffers.fInputEnvelopeBuffer->getWritePointer(0)[matrixInputState->currentFrame]; //0..1
			sourceVal = (sourceVal > 1.0f) ? 1.0f : sourceVal; //clip it
			sourceVal  = (sourceVal < 0.0f) ? 0.0f : sourceVal;
			sourceVal = (sourceVal - 0.5f) * 2.f; //-1..1	
			break;
		}
		case MODMATSRCE::ModWheel: {
			sourceVal = float(m_uModWheel) / 127.0f; //0..1
			sourceVal = (sourceVal - 0.5f) * 2.f; //-1..1	
			break;
		}
		case MODMATSRCE::AfterTouch: {
			int midiNote = m_RoutingBuffers.MidiNoteBuffer[matrixInputState->voiceNo]->getSample(0, matrixInputState->currentFrame);
			if (midiNote == iAftertouchMidiNote[matrixInputState->voiceNo]) {// is channel aftertouch
				sourceVal = float(iAftertouch[matrixInputState->voiceNo]) / 127.f; //0..1
				sourceVal = (sourceVal - 0.5f) * 2.f; //-1..1	
			}
			else
				sourceVal = -1.f;
			break;
		}
		case MODMATSRCE::MPETimbreMinMax: {
			int midiNote = m_RoutingBuffers.MidiNoteBuffer[matrixInputState->voiceNo]->getSample(0, matrixInputState->currentFrame);
			if (midiNote == iMPETimbreMidiNote[matrixInputState->voiceNo]) {
				sourceVal = float(iMPETimbre[matrixInputState->voiceNo]) / 127.f; //0..1 // 64 is center
				sourceVal = (sourceVal - 0.5f) * 2.f; //-1..1	
			}
			else
				sourceVal = -1.f;
			break;
		}
		case MODMATSRCE::MPETimbreMax: {
			int midiNote = m_RoutingBuffers.MidiNoteBuffer[matrixInputState->voiceNo]->getSample(0, matrixInputState->currentFrame);
			if (midiNote == iMPETimbreMidiNote[matrixInputState->voiceNo]) {
				sourceVal = ((jlimit<int>(64, 127, iMPETimbre[matrixInputState->voiceNo]) - 64) * 2) / 126.f; //scale from 64-127 to 0..1
				sourceVal = (sourceVal - 0.5f) * 2.f; //-1..1	
			}
			else
				sourceVal = -1.f;
			break;
		}
		case MODMATSRCE::MPETimbreMin: {
			int midiNote = m_RoutingBuffers.MidiNoteBuffer[matrixInputState->voiceNo]->getSample(0, matrixInputState->currentFrame);
			if (midiNote == iMPETimbreMidiNote[matrixInputState->voiceNo]) {
				sourceVal = (jlimit<int>(0, 64, iMPETimbre[matrixInputState->voiceNo]) * 2) / 128.f; //scale from 0 - 64 to 0..1
				sourceVal = (sourceVal - 0.5f) * 2.f; //-1..1	
			}
			else
				sourceVal = -1.f;
			break;
		}
		case MODMATSRCE::Velocity: {
			sourceVal = fastPrecisePow(m_RoutingBuffers.VelocityBuffer[matrixInputState->voiceNo]->getSample(0, matrixInputState->currentFrame), 2.0f) / C_F127SQUARED; // https://stackoverflow.com/questions/41042486/volume-velocity-to-gain-web-audio
			sourceVal = (sourceVal - 0.5f) * 2.f; //-1..1	
			break;
		}
		case MODMATSRCE::KeyTrack: {
			int midiNote = m_RoutingBuffers.MidiNoteBuffer[matrixInputState->voiceNo]->getSample(0, matrixInputState->currentFrame);
			int lNote = (midiNote > 23) ? midiNote - 23 : 0;
			lNote = (lNote < 60) ? lNote : 60; // 5 octaves
			sourceVal = (lNote / 60.0); //0..1
			sourceVal = (sourceVal - 0.5f) * 2.f; //-1..1
			break;
		}
		case MODMATSRCE::RandomDriftSlow: {
			sourceVal = driftNoiseSlow(slot);
			break;
		}
		case MODMATSRCE::RandomDriftFast: {
			sourceVal = driftNoiseFast(slot);
			break;
		}
		}
	}

	lastModMatrixSourceVal[slot][slotSource][matrixInputState->voiceNo] = sourceVal; //has to depend on slot since randomdrift sources are different per slot

	multiplier = sourceVal;

	if (multiplier < -1.f) {
		VDBG("had to clip mod mat multiplier!");
		multiplier = -1.f;
	}
	if (multiplier > 1.f) {
		VDBG("had to clip mod mat multiplier!");
		multiplier = 1.f;
	}

	//float multiplier0to1 = abs(multiplier);
	float multiplier0to1 = (multiplier + 1.f) * 0.5;
	multiplier0to1 = cubicBezierApproximation(multiplier0to1, curvy); //approximation is on 0..1
	//multiplier = (multiplier >= 0.f ? multiplier0to1 : -multiplier0to1); //convert to -1..1

	if (slotPolarity == POLARITY::Bipolar) { 
		multiplier = multiplier0to1 * 2.f - 1.f; //convert to -1..1
	}
	else
		multiplier = multiplier0to1; // 0..1

	jassert((multiplier >= -1.f) && (multiplier <= 1.f));
}

//============================================================================================================
float CVASTSettings::getParameterValueWithMatrixModulation(std::atomic<float> *paramVal, MYUINT destination, modMatrixInputState* matrixInputState, bool *isModulating) {
	if (!modMatrixDestinationSetFast(destination)) {
		return paramVal->load();
	}
	//from here it is a modulated parameter
	
	float multiplier = 1.f;
	float modVal = 0.f;
	int slotPolarity = 0;
	int firstRelevantSlotPolarity = -1;
	bool firstModSet = false;

	for (int slot = 0; slot < M_MODMATRIX_MAX_SLOTS; slot++) {
		if (!modMatrixSlotUsed[slot]) continue; //perfopt
		if (modMatrixSlotDest[slot] != destination) continue; //perfopt

		float slotModVal = 0.f;
		float slotMultiplier = 1.f;

		getMatrixModulationSlotMultiplier(slot, slotModVal, slotMultiplier, slotPolarity, destination, matrixInputState);
        if (slotModVal != 0.f) {
			if (!firstModSet) {
				firstModSet = true;
				modVal = slotModVal; //take first slots (souce / dest) modVal
				firstRelevantSlotPolarity = slotPolarity;
				multiplier = slotMultiplier; //-1..1
			}
            else {
				multiplier = (multiplier + slotMultiplier * (slotModVal * 0.01f)) / (1.f + abs(slotModVal) * 0.01f); //its a modulation on top -1..1

				if (firstRelevantSlotPolarity == POLARITY::Unipolar)
					multiplier = jlimit<float>(0.f, 1.f, multiplier);
            }
        }
	}

	//==============================================
	//do ranges

	double skew = my_processor->m_modMatrixLookupTable[destination].rangeSkew;
	double curValPercentage = (paramVal->load() - my_processor->m_modMatrixLookupTable[destination].rangeStart) / (my_processor->m_modMatrixLookupTable[destination].rangeEnd - my_processor->m_modMatrixLookupTable[destination].rangeStart);
	double modStartPercentage = curValPercentage - abs(modVal * 0.01f); 
	if (modStartPercentage > 1.f) modStartPercentage = 1.f;
	if (modStartPercentage < 0.f) modStartPercentage = 0.f;
	double  modEndPercentage = curValPercentage + abs(modVal * 0.01f);
	if (modEndPercentage > 1.f) modEndPercentage = 1.f;
	if (modEndPercentage < 0.f) modEndPercentage = 0.f;	
    if (firstRelevantSlotPolarity == POLARITY::Unipolar) {
        if (modVal > 0.f) {
            modStartPercentage = curValPercentage;
        }
        else {
            modEndPercentage = modStartPercentage;
            modStartPercentage = curValPercentage;
        }
    }
	
	jassert(skew > 0);
		
	if (skew != 1.f) {
		if (m_bBeforeV22CompatibilityMode) {
			modStartPercentage = std::pow(modStartPercentage, 1 / skew);
			modEndPercentage = std::pow(modEndPercentage, 1 / skew);
		}
	}
	
	double length = my_processor->m_modMatrixLookupTable[destination].rangeEnd - my_processor->m_modMatrixLookupTable[destination].rangeStart;
	double modLength = (modEndPercentage - modStartPercentage) * length;	
	if (multiplier <= -1.f) multiplier = -1.f; //CHECK
	if (multiplier >= 1.f) multiplier = 1.f; //CHECK
	double multiplier0to1 = multiplier;
	if (firstRelevantSlotPolarity == POLARITY::Bipolar) {
		if (modVal < 0.f) multiplier *= -1.f; //CHECK
		multiplier0to1 = (multiplier + 1.f) * 0.5; // 0..1
	}
	else {
		if (modVal < 0.f)
			multiplier0to1 = 1.f - multiplier0to1;
	}

	if (isModulating != nullptr)
		if (modVal != 0.f)
			*isModulating = true; //does not initialize to false

	jassert((multiplier0to1 >= 0.f) && (multiplier0to1 <= 1.f));
	double resultVal = my_processor->m_modMatrixLookupTable[destination].rangeStart + modStartPercentage * length + modLength * multiplier0to1; 
	resultVal = jlimit<double>(my_processor->m_modMatrixLookupTable[destination].rangeStart, my_processor->m_modMatrixLookupTable[destination].rangeEnd, resultVal);
	jassert((resultVal >= my_processor->m_modMatrixLookupTable[destination].rangeStart) && (resultVal <= my_processor->m_modMatrixLookupTable[destination].rangeEnd));
	return resultVal;
}

//Envelope follower
void CVASTSettings::processEnvelope(int inFramesToProcess) {
	/** Sets the times for the vaious stages of the envelope.
	1 is an instant attack/release, 0 will never change the value.
	*/
	float envAttack = 0.9f;	//coefficient param
	float envRelease = 0.1f; //coefficient param

	for (int i = 0; i < inFramesToProcess; i++) {
		float envIn = fabsf(m_RoutingBuffers.fAudioInputBuffer->getReadPointer(0)[i]);

		if (mInputEnvelope < envIn)
			mInputEnvelope += envAttack * (envIn - mInputEnvelope);
		else if (mInputEnvelope > envIn)
			mInputEnvelope -= envRelease * (mInputEnvelope - envIn);
		m_RoutingBuffers.fInputEnvelopeBuffer->getWritePointer(0)[i] = mInputEnvelope;
	}
}

float CVASTSettings::driftNoiseSlow(int slot)
{
	float rand11 = (((float)rand() / RAND_MAX) * 2.f - 1.f); //-1..1
	float newVal = m_fDriftLfoSlow[slot] * (1.f - c_randdriftfilter_slow) + rand11 * c_randdriftfilter_slow;
	if ((newVal * c_randdriftm_slow > 1.f) || (newVal * c_randdriftm_slow < -1.f))
		newVal = m_fDriftLfoSlow[slot] * (1.f - c_randdriftfilter_slow) - rand11 * c_randdriftfilter_slow;
	m_fDriftLfoSlow[slot] = newVal;
	return m_fDriftLfoSlow[slot] * c_randdriftm_slow;
}

float CVASTSettings::driftNoiseFast(int slot)
{
	float rand11 = (((float)rand() / RAND_MAX) * 2.f - 1.f); //-1..1
	float newVal = m_fDriftLfoFast[slot] * (1.f - c_randdriftfilter_fast) + rand11 * c_randdriftfilter_fast;
	if ((newVal * c_randdriftm_fast > 1.f) || (newVal * c_randdriftm_fast < -1.f))
		newVal = m_fDriftLfoFast[slot] * (1.f - c_randdriftfilter_fast) - rand11 * c_randdriftfilter_fast;
	m_fDriftLfoFast[slot] = newVal;
	return m_fDriftLfoFast[slot] * c_randdriftm_fast;
}

//enum class customFonts { OpenSans, OpenSansBold, AlteHaasGrotesk, AlteHaasGroteskBold, SFUIDisplayRegular, SFUIDisplayBold, TradeGothicLT, TradeGothicLTBold };
void CVASTSettings::loadCustomFonts() {
	customFontBuffer[0] = Font(Typeface::createSystemTypefaceFor(BinaryData::OpenSansRegular_ttf, BinaryData::OpenSansRegular_ttfSize));
	customFontBuffer[1] = Font(Typeface::createSystemTypefaceFor(BinaryData::OpenSansBold_ttf, BinaryData::OpenSansBold_ttfSize));
	customFontBuffer[2] = Font(Typeface::createSystemTypefaceFor(BinaryData::AlteHaasGroteskRegular_ttf, BinaryData::AlteHaasGroteskRegular_ttfSize));
	customFontBuffer[3] = Font(Typeface::createSystemTypefaceFor(BinaryData::AlteHaasGroteskBold_ttf, BinaryData::AlteHaasGroteskBold_ttfSize));
	customFontBuffer[4] = Font(Typeface::createSystemTypefaceFor(BinaryData::SFUIDisplayRegular_ttf, BinaryData::SFUIDisplayRegular_ttfSize));
	customFontBuffer[5] = Font(Typeface::createSystemTypefaceFor(BinaryData::SFUIDisplayBold_ttf, BinaryData::SFUIDisplayBold_ttfSize));
	customFontBuffer[6] = Font(Typeface::createSystemTypefaceFor(BinaryData::Trade_Gothic_LT_ttf, BinaryData::Trade_Gothic_LT_ttfSize));
	customFontBuffer[7] = Font(Typeface::createSystemTypefaceFor(BinaryData::Trade_Gothic_LT_Bold_ttf, BinaryData::Trade_Gothic_LT_Bold_ttfSize));
}

Font CVASTSettings::getCustomFont(CVASTSettings::customFonts customFont) {
	return customFontBuffer[static_cast<int>(customFont)];
}

void CVASTSettings::qFilterCoefficientsInitTables() {

	if ((filterCoeffLastSampleRate == m_nSampleRate) &&
		(approximatelyEqual(float(filterCoeffLastMmasterTuneHz), m_fMasterTune.load())))
		return;
	
	filterCoeffLastSampleRate = m_nSampleRate;
	filterCoeffLastMmasterTuneHz = m_fMasterTune.load();

	for (int i = 0; i < 512; i++)
	{
		table_dB[i] = powf(10.f, 0.05f * ((float)i - 384.f));
		table_pitch[i] = powf(2.f, ((float)i - 256.f) * (1.f / 12.f));
		table_pitch_inv[i] = 1.f / table_pitch[i];

		int osFactor = 1;
		double dsamplerate_os = double(osFactor) * m_nSampleRate;
		double dsamplerate_os_inv = 1.0 / dsamplerate_os;
		table_note_omega_os1[0][i] =
			(float)sin(2 * M_PI * min(0.5, m_fMasterTune.load() * table_pitch[i] * dsamplerate_os_inv));
		table_note_omega_os1[1][i] =
			(float)cos(2 * M_PI * min(0.5, m_fMasterTune.load() * table_pitch[i] * dsamplerate_os_inv));
		osFactor = 2;
		dsamplerate_os = double(osFactor) * m_nSampleRate;
		dsamplerate_os_inv = 1.0 / dsamplerate_os;
		table_note_omega_os2[0][i] =
			(float)sin(2 * M_PI * min(0.5, m_fMasterTune.load() * table_pitch[i] * dsamplerate_os_inv));
		table_note_omega_os2[1][i] =
			(float)cos(2 * M_PI * min(0.5, m_fMasterTune.load() * table_pitch[i] * dsamplerate_os_inv));
		osFactor = 4;
		dsamplerate_os = double(osFactor) * m_nSampleRate;
		dsamplerate_os_inv = 1.0 / dsamplerate_os;
		table_note_omega_os4[0][i] =
			(float)sin(2 * M_PI * min(0.5, m_fMasterTune.load() * table_pitch[i] * dsamplerate_os_inv));
		table_note_omega_os4[1][i] =
			(float)cos(2 * M_PI * min(0.5, m_fMasterTune.load() * table_pitch[i] * dsamplerate_os_inv));
	}
}

JUCE_END_IGNORE_WARNINGS_GCC_LIKE
JUCE_END_IGNORE_WARNINGS_MSVC
