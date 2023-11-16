/*
VAST Dynamics Audio Software (TM)
*/

#include "VASTEngineHeader.h"
#include "VASTParamState.h"
#include "FX/VASTEffect.h"

CVASTParamState::CVASTParamState()
{
}

/* destructor()
	Destroy variables allocated in the contructor()

*/
CVASTParamState::~CVASTParamState(void)
{	
}

JUCE_BEGIN_IGNORE_WARNINGS_GCC_LIKE("-Wconversion")
JUCE_BEGIN_IGNORE_WARNINGS_MSVC(4244 4267) //C4244 conversion from 'type1' to 'type2', possible loss of data //C4267 conversion

float CVASTParamState::toggleButtonTextToValueFunction(const String& text) { //static
	return float(text.equalsIgnoreCase("On") || text.equalsIgnoreCase("1") || text.equalsIgnoreCase("SWITCH_ON"));
}

String CVASTParamState::toggleButtonValueToTextFunction(float value) { //static
	return value < 0.5f ? "Off" : "On";
}

float CVASTParamState::floatSliderTextToValueFunction(const String& text) { //static	
	return text.getFloatValue();
}

String CVASTParamState::floatSliderValueToTextFunction(float value) { //static
	return String(value);
}

float CVASTParamState::floatSliderTextToValueFunctionMaxIsOff(const String& text) { //static	
	if (text.equalsIgnoreCase("Off"))
		return 1.f;
	else 
		return 101.f - (text.getFloatValue());
}

String CVASTParamState::floatSliderValueToTextFunctionMaxIsOff(float value) { //static
	if (approximatelyEqual(value, 1.f))
		return String("Off");
	else 
		return String(101.f - value);
}

float CVASTParamState::intSliderTextToValueFunction(const String& text) { //static	
	return text.getIntValue();
}

String CVASTParamState::intSliderValueToTextFunction(float value) { //static
	return String(floor(value + 0.5f));
}

String CVASTParamState::comboBoxValueToTextFunction_WAVE(float value) { //static	
	return WAVE_Array[floor(value + 0.5f)];
}

float CVASTParamState::comboBoxTextToValueFunction_WAVE(const String& text) { //static	
	return WAVE_Array.indexOf(StringRef(text), true);
}

String CVASTParamState::comboBoxValueToTextFunction_LFOWAVE(float value) { //static	
	return LFOWAVE_Array[floor(value + 0.5f)];
}

float CVASTParamState::comboBoxTextToValueFunction_LFOWAVE(const String& text) { //static	
	return LFOWAVE_Array.indexOf(StringRef(text), true);
}

String CVASTParamState::comboBoxValueToTextFunction_MODMATSRCE(float value) { //static	
	return MODMATSRCE_Array[floor(value + 0.5f)];
}

float CVASTParamState::comboBoxTextToValueFunction_MODMATSRCE(const String& text) { //static	
	return MODMATSRCE_Array.indexOf(StringRef(text), true);
}

String CVASTParamState::comboBoxValueToTextFunction_MODMATDEST(float value) { //static	
	return MODMATDEST_Array[floor(value + 0.5f)];
}

float CVASTParamState::comboBoxTextToValueFunction_MODMATDEST(const String& text) { //static	
	return MODMATDEST_Array.indexOf(StringRef(text), true);
}

String CVASTParamState::comboBoxValueToTextFunction_POLYMODE(float value) { //static	
	return POLYMODE_Array[floor(value + 0.5f)];
}

float CVASTParamState::comboBoxTextToValueFunction_POLYMODE(const String& text) { //static	
	return POLYMODE_Array.indexOf(StringRef(text), true);
}

String CVASTParamState::comboBoxValueToTextFunction_SWITCH(float value) { //static	
	return SWITCH_Array[floor(value + 0.5f)];
}

float CVASTParamState::comboBoxTextToValueFunction_SWITCH(const String& text) { //static	
	return SWITCH_Array.indexOf(StringRef(text), true);
}

String CVASTParamState::comboBoxValueToTextFunction_POLARITY(float value) { //static	
	return POLARITY_Array[floor(value + 0.5f)];
}

float CVASTParamState::comboBoxTextToValueFunction_POLARITY(const String& text) { //static	
	return POLARITY_Array.indexOf(StringRef(text), true);
}

String CVASTParamState::comboBoxValueToTextFunction_OSCROUTE(float value) { //static	
	return OSCROUTE_Array[floor(value + 0.5f)];
}

float CVASTParamState::comboBoxTextToValueFunction_OSCROUTE(const String& text) { //static	
	return OSCROUTE_Array.indexOf(StringRef(text), true);
}

String CVASTParamState::comboBoxValueToTextFunction_FILTER1ROUTE(float value) { //static	
	return FILTER1ROUTE_Array[floor(value + 0.5f)];
}

float CVASTParamState::comboBoxTextToValueFunction_FILTER1ROUTE(const String& text) { //static	
	return FILTER1ROUTE_Array.indexOf(StringRef(text), true);
}

String CVASTParamState::comboBoxValueToTextFunction_FILTER2ROUTE(float value) { //static	
	return FILTER2ROUTE_Array[floor(value + 0.5f)];
}

float CVASTParamState::comboBoxTextToValueFunction_FILTER2ROUTE(const String& text) { //static	
	return FILTER2ROUTE_Array.indexOf(StringRef(text), true);
}

String CVASTParamState::comboBoxValueToTextFunction_FILTER3ROUTE(float value) { //static	
	return FILTER3ROUTE_Array[floor(value + 0.5f)];
}

float CVASTParamState::comboBoxTextToValueFunction_FILTER3ROUTE(const String& text) { //static	
	return FILTER3ROUTE_Array.indexOf(StringRef(text), true);
}

String CVASTParamState::comboBoxValueToTextFunction_FXBUSROUTE(float value) { //static	
	return FXBUSROUTE_Array[floor(value + 0.5f)];
}

float CVASTParamState::comboBoxTextToValueFunction_FXBUSROUTE(const String& text) { //static	
	return FXBUSROUTE_Array.indexOf(StringRef(text), true);
}

String CVASTParamState::comboBoxValueToTextFunction_ARPMODE(float value) { //static	
	return ARPMODE_Array[floor(value + 0.5f)];
}

float CVASTParamState::comboBoxTextToValueFunction_ARPMODE(const String& text) { //static	
	return ARPMODE_Array.indexOf(StringRef(text), true);
}

String CVASTParamState::comboBoxValueToTextFunction_NOISETYPE(float value) { //static	
	return NOISETYPE_Array[floor(value + 0.5f)];
}

float CVASTParamState::comboBoxTextToValueFunction_NOISETYPE(const String& text) { //static	
	return NOISETYPE_Array.indexOf(StringRef(text), true);
}

String CVASTParamState::comboBoxValueToTextFunction_MSEGENV(float value) { //static	
	return MSEGENV_Array[floor(value + 0.5f)];
}

float CVASTParamState::comboBoxTextToValueFunction_MSEGENV(const String& text) { //static	
	return MSEGENV_Array.indexOf(StringRef(text), true);
}

String CVASTParamState::comboBoxValueToTextFunction_MSEGLFOENV(float value) { //static	
	return MSEGLFOENV_Array[floor(value + 0.5f)];
}

float CVASTParamState::comboBoxTextToValueFunction_MSEGLFOENV(const String& text) { //static	
	return MSEGLFOENV_Array.indexOf(StringRef(text), true);
}

String CVASTParamState::comboBoxValueToTextFunction_FILTERTYPE(float value) { //static	
	return FILTERTYPE_Array[floor(value + 0.5f)];
}

float CVASTParamState::comboBoxTextToValueFunction_FILTERTYPE(const String& text) { //static
	return FILTERTYPE_Array.indexOf(StringRef(text), true);
}

String CVASTParamState::comboBoxValueToTextFunction_MIDINOTES(float value) { //static	
	return MIDINOTES_Array[floor(value + 0.5f)];
}

float CVASTParamState::comboBoxTextToValueFunction_MIDINOTES(const String& text) { //static	
	return MIDINOTES_Array.indexOf(StringRef(text), true);
}

String CVASTParamState::comboBoxValueToTextFunction_TIMEBEATS(float value) { //static	
	return TIMEBEATS_Array[floor(value + 0.5f)];
}

float CVASTParamState::comboBoxTextToValueFunction_TIMEBEATS(const String& text) { //static	
	return TIMEBEATS_Array.indexOf(StringRef(text), true);
}

String CVASTParamState::comboBoxValueToTextFunction_WTFX(float value) { //static	
	return WTFX_Array[floor(value + 0.5f)];
}

float CVASTParamState::comboBoxTextToValueFunction_WTFX(const String& text) { //static	
	return WTFX_Array.indexOf(StringRef(text), true);
}

/* IMPORTANT */
//the version number is important: new parameters have to always get higher numbers
/* IMPORTANT */

bool CVASTParamState::initParameters(AudioProcessorValueTreeState& parameters, CVASTEffect* effect) {

	//master
	effect->createAndAddParameter(&m_fMasterVolumedB, parameters, 1, "m_fMasterVolumedB", "Master volume in dB. Parameter is saved as well with preset.", "Master volume in dB. Parameter is saved as well with preset.", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(-60, 20), 0,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_fPortamento, parameters, 1, "m_fPortamento", "Portamento time in ms - available in poly modes and mono mode", "Portamento time in ms - available in poly modes and mono mode", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0, 10000, 0.00001f, 0.3f, false), 0.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_fMasterTune, parameters, 1, "m_fMasterTune", "Master tune in hz. Parameter is saved as well with preset.", "Master tune in hz. Parameter is saved as well with preset.", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(400, 480), 440.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false, 
		true);

	effect->createAndAddParameter(&m_fWheelSensitivity, parameters, 1, "m_fWheelSensitivity", "Pitch bend wheel sensitivity", "Pitch bend wheel sensitivity", 0, //obsolete - not used
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0, 100), 50.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);

	effect->createAndAddParameter(&m_fPitchMod, parameters, 1, "m_fPitchMod", "Voice pitch for modulation", "Voice pitch for modulation", 0,
		MODMATDEST::Pitch, //tested: OK
		NormalisableRange<float>(-100, 100), 0.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);

	effect->createAndAddParameter(&m_fVoiceGain, parameters, 1, "m_fVoiceGain", "Voice gain percentage for velocity modulation", "Voice gain percentage for velocity modulation", 0,
		MODMATDEST::VoiceGain, //tested: OK
		NormalisableRange<float>(0, 100), 100.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);

	effect->createAndAddParameter(&m_uPolyMode, parameters, 1, "m_uPolyMode", "Poly mode: Mono mode, Poly4 mode, Poly16 mode, Poly32 mode", "Poly mode: Mono mode - one note at a time, Poly4 mode - four notes at at time - Poly16 mode - max. 16 notes at at time ...", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 3, 1.0f), POLYMODE_Array.indexOf("Poly16"),
		CVASTParamState::comboBoxValueToTextFunction_POLYMODE,
		CVASTParamState::comboBoxTextToValueFunction_POLYMODE,
		false, true, true, false,
		true);

	effect->createAndAddParameter(&m_bLegatoMode, parameters, 1, "m_bLegatoMode", "Legato mode for voicing", "Legato mode for voicing", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 1.0f, 1.0f), 1.0f,
		CVASTParamState::toggleButtonValueToTextFunction,
		CVASTParamState::toggleButtonTextToValueFunction,
		false, true, true, false);

	//custom modulators
	effect->createAndAddParameter(&m_fCustomModulator1, parameters, 1, "m_fCustomModulator1", "Custom modulator 1", "Custom modulator 1", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0, 100), 0.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_fCustomModulator2, parameters, 1, "m_fCustomModulator2", "Custom modulator 2", "Custom modulator 2", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0, 100), 0.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_fCustomModulator3, parameters, 1, "m_fCustomModulator3", "Custom modulator 3", "Custom modulator 3", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0, 100), 0.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_fCustomModulator4, parameters, 1, "m_fCustomModulator4", "Custom modulator 4", "Custom modulator 4", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0, 100), 0.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	//========================================================================================
	//generators
	//OscA
	effect->createAndAddParameter(&m_bOscOnOff_OscA, parameters, 1, "m_bOscOnOff_OscA", "OSC A Switch oscillator bank on / off", "Switch oscillator bank on / off", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 1.0f, 1.0f), 1.0f,
		CVASTParamState::toggleButtonValueToTextFunction,
		CVASTParamState::toggleButtonTextToValueFunction,
		false, true, true, false,
		true); //has to update oscs

	effect->createAndAddParameter(&m_uOscRouting1_OscA, parameters, 1, "m_uOscRouting1_OscA", "OSC A Oscillator routing 1", "Oscillator routing 1", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, OSCROUTE_Array.size() - 1, 1.0f), OSCROUTE_Array.indexOf("FXBus1"),
		CVASTParamState::comboBoxValueToTextFunction_OSCROUTE,
		CVASTParamState::comboBoxTextToValueFunction_OSCROUTE,
		false, true, true, false);

	effect->createAndAddParameter(&m_uOscRouting2_OscA, parameters, 1, "m_uOscRouting2_OscA", "OSC A Oscillator routing 2", "Oscillator routing 2", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, OSCROUTE_Array.size() - 1, 1.0f), 0.f,
		CVASTParamState::comboBoxValueToTextFunction_OSCROUTE,
		CVASTParamState::comboBoxTextToValueFunction_OSCROUTE,
		false, true, true, false);

	effect->createAndAddParameter(&m_fOscDetune_OscA, parameters, 1, "m_fOscDetune_OscA", "OSC A Normalized detune with bank oscillators", "Normalized detune with bank oscillators", 0,
		MODMATDEST::OscADetune, //tested: OK
		NormalisableRange<float>(0, 100, 0.00001f, 0.3f, false), 0.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_fOscGain_OscA, parameters, 1, "m_fOscGain_OscA", "OSC A Bank gain (100 full, >100 overdrive)", "Bank gain (100 full, >100 overdrive)", 0,
		MODMATDEST::OscAGain, //tested: OK
		NormalisableRange<float>(0, 200), 100,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);

	effect->createAndAddParameter(&m_fOscCents_OscA, parameters, 1, "m_fOscCents_OscA", "OSC A Bank cents (+/- 1 octave and half tones)", "Bank cents (+/- 1 octave and half tones)", 0,
		MODMATDEST::OscACents, //tested: OK
		NormalisableRange<float>(-2400, 2400), 0,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);

	effect->createAndAddParameter(&m_fOscWTPos_OscA, parameters, 1, "m_fOscWTPos_OscA", "OSC A Oscillator wavetable position", "Oscillator wavetable position", 0,
		MODMATDEST::OscAWTPos, //tested: OK
		NormalisableRange<float>(0, 100), 0,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_fOscMorph_OscA, parameters, 1, "m_fOscMorph_OscA", "OSC A Oscillator morphing", "Oscillator morphing", 0,
		MODMATDEST::OscAMorph, //tested: OK
		NormalisableRange<float>(0, 100), 0,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_fSpread_OscA, parameters, 1, "m_fSpread_OscA", "OSC A Stereo spread within bank oscillators", "Stereo spread within bank oscillators", 0,
		MODMATDEST::OscASpread, //tested: OK
		NormalisableRange<float>(0, 100), 0,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);

	effect->createAndAddParameter(&m_iNumOscs_OscA, parameters, 1, "m_iNumOscs_OscA", "OSC A Number of bank oscillators", "Number of bank oscillators", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(1, 24, 1), 1,
		CVASTParamState::intSliderValueToTextFunction,
		CVASTParamState::intSliderTextToValueFunction,
		false, true, true, false,
		true);

	effect->createAndAddParameter(&m_iOscOct_OscA, parameters, 1, "m_iOscOct_OscA", "OSC A Bank oscillators octave", "Bank oscillators octave", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(-4, 4, 1), 0,
		CVASTParamState::intSliderValueToTextFunction,
		CVASTParamState::intSliderTextToValueFunction,
		false, true, true, false,
		true);

	effect->createAndAddParameter(&m_fOscPan_OscA, parameters, 1, "m_fOscPan_OscA", "OSC A Stereo panning of oscillator", "Stereo panning of oscillator", 0,
		MODMATDEST::OscAPan, //tested: OK
		NormalisableRange<float>(-100, 100), 0,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);

	effect->createAndAddParameter(&m_fOscPhase_OscA, parameters, 1, "m_fOscPhase_OscA", "OSC A Phase of oscillator", "Phase of oscillator", 0,
		MODMATDEST::OscAPhase, //tested: OK
		NormalisableRange<float>(0, 100), 0,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);

	effect->createAndAddParameter(&m_bExciterOnOff_OscA, parameters, 1, "m_bExciterOnOff_OscA", "OSC A Exciter adds random detune by max +- 0.25hz per oscillator and played note", "Exciter adds random detune by max +- 0.25hz per oscillator and played note", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 1.0f, 1.0f), 0.0f,
		CVASTParamState::toggleButtonValueToTextFunction,
		CVASTParamState::toggleButtonTextToValueFunction,
		false, true, true, false);

	effect->createAndAddParameter(&m_bOscInverterOnOff_OscA, parameters, 1, "m_bOscInverterOnOff_OscA", "OSC A Invert all voices not in the center", "Invert all voices not in the center", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 1.0f, 1.0f), 0.0f,
		CVASTParamState::toggleButtonValueToTextFunction,
		CVASTParamState::toggleButtonTextToValueFunction,
		false, true, true, false);

	effect->createAndAddParameter(&m_bOscRetrigOnOff_OscA, parameters, 1, "m_bOscRetrigOnOff_OscA", "OSC A Random phase offeset for each oscillator for each new note or sync at beginning of phase", "Random phase offeset for each oscillator for each new note or sync at beginning of phase", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 1.0f, 1.0f), 1.0f,
		CVASTParamState::toggleButtonValueToTextFunction,
		CVASTParamState::toggleButtonTextToValueFunction,
		false, true, true, false);

	effect->createAndAddParameter(&m_uVCAEnv_OscA, parameters, 1, "m_uVCAEnv_OscA", "OSC A Envelope used as VCA envelope", "OSC A Envelope used as VCA envelope", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, MSEGENV_Array.size() - 1, 1.0f), 0.0f,
		CVASTParamState::comboBoxValueToTextFunction_MSEGENV,
		CVASTParamState::comboBoxTextToValueFunction_MSEGENV,
		false, true, true, false);

	//OscB
	effect->createAndAddParameter(&m_bOscOnOff_OscB, parameters, 1, "m_bOscOnOff_OscB", "OSC B Switch oscillator bank on / off", "Switch oscillator bank on / off", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 1.0f, 1.0f), 0.0f,
		CVASTParamState::toggleButtonValueToTextFunction,
		CVASTParamState::toggleButtonTextToValueFunction,
		false, true, true, false,
		true); //has to update oscs

	effect->createAndAddParameter(&m_uOscRouting1_OscB, parameters, 1, "m_uOscRouting1_OscB", "OSC B Oscillator routing 1", "Oscillator routing 1", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, OSCROUTE_Array.size() - 1, 1.0f), OSCROUTE_Array.indexOf("FXBus1"),
		CVASTParamState::comboBoxValueToTextFunction_OSCROUTE,
		CVASTParamState::comboBoxTextToValueFunction_OSCROUTE,
		false, true, true, false);

	effect->createAndAddParameter(&m_uOscRouting2_OscB, parameters, 1, "m_uOscRouting2_OscB", "OSC B Oscillator routing 2", "Oscillator routing 2", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, OSCROUTE_Array.size() - 1, 1.0f), 0.0f,
		CVASTParamState::comboBoxValueToTextFunction_OSCROUTE,
		CVASTParamState::comboBoxTextToValueFunction_OSCROUTE,
		false, true, true, false);
	
	effect->createAndAddParameter(&m_fOscDetune_OscB, parameters, 1, "m_fOscDetune_OscB", "OSC B Normalized detune with bank oscillators", "Normalized detune with bank oscillators", 0,
		MODMATDEST::OscBDetune, //tested: OK
		NormalisableRange<float>(0, 100, 0.00001f, 0.3f, false), 0.f, 
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_fOscGain_OscB, parameters, 1, "m_fOscGain_OscB", "OSC B Bank gain (100 full, >100 overdrive)", "Bank gain (100 full, >100 overdrive)", 0,
		MODMATDEST::OscBGain, //tested: OK
		NormalisableRange<float>(0, 200), 100,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);

	effect->createAndAddParameter(&m_fOscCents_OscB, parameters, 1, "m_fOscCents_OscB", "OSC B Bank cents (+/- 1 octave and half tones)", "Bank cents (+/- 1 octave and half tones)", 0,
		MODMATDEST::OscBCents, //tested: OK
		NormalisableRange<float>(-2400, 2400), 0,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);

	effect->createAndAddParameter(&m_fOscWTPos_OscB, parameters, 1, "m_fOscWTPos_OscB", "OSC B Oscillator wavetable position", "Oscillator wavetable position", 0,
		MODMATDEST::OscBWTPos, //tested: OK
		NormalisableRange<float>(0, 100), 0,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_fOscMorph_OscB, parameters, 1, "m_fOscMorph_OscB", "OSC B Oscillator morphing", "Oscillator morphing", 0,
		MODMATDEST::OscBMorph, //tested: OK
		NormalisableRange<float>(0, 100), 0,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_fSpread_OscB, parameters, 1, "m_fSpread_OscB", "OSC B Stereo spread within bank oscillators", "Stereo spread within bank oscillators", 0,
		MODMATDEST::OscBSpread, //tested: OK
		NormalisableRange<float>(0, 100), 0,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);

	effect->createAndAddParameter(&m_iNumOscs_OscB, parameters, 1, "m_iNumOscs_OscB", "OSC B Number of bank oscillators", "Number of bank oscillators", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(1, 24, 1), 1,
		CVASTParamState::intSliderValueToTextFunction,
		CVASTParamState::intSliderTextToValueFunction,
		false, true, true, false,
		true);

	effect->createAndAddParameter(&m_iOscOct_OscB, parameters, 1, "m_iOscOct_OscB", "OSC B Bank oscillators octave", "Bank oscillators octave", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(-4, 4, 1), 0,
		CVASTParamState::intSliderValueToTextFunction,
		CVASTParamState::intSliderTextToValueFunction,
		false, true, true, false,
		true);

	effect->createAndAddParameter(&m_fOscPan_OscB, parameters, 1, "m_fOscPan_OscB", "OSC B Stereo panning of oscillator", "Stereo panning of oscillator", 0,
		MODMATDEST::OscBPan, //tested: OK
		NormalisableRange<float>(-100, 100), 0,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);

	effect->createAndAddParameter(&m_fOscPhase_OscB, parameters, 1, "m_fOscPhase_OscB", "OSC B Phase of oscillator", "Phase of oscillator", 0,
		MODMATDEST::OscBPhase, //tested: OK
		NormalisableRange<float>(0, 100), 0,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);

	effect->createAndAddParameter(&m_bExciterOnOff_OscB, parameters, 1, "m_bExciterOnOff_OscB", "OSC B Exciter adds random detune by max +- 0.25hz per oscillator and played note", "Exciter adds random detune by max +- 0.25hz per oscillator and played note", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 1.0f, 1.0f), 0.0f,
		CVASTParamState::toggleButtonValueToTextFunction,
		CVASTParamState::toggleButtonTextToValueFunction,
		false, true, true, false);

	effect->createAndAddParameter(&m_bOscInverterOnOff_OscB, parameters, 1, "m_bOscInverterOnOff_OscB", "OSC B Invert all voices not in the center", "Invert all voices not in the center", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 1.0f, 1.0f), 0.0f,
		CVASTParamState::toggleButtonValueToTextFunction,
		CVASTParamState::toggleButtonTextToValueFunction,
		false, true, true, false);

	effect->createAndAddParameter(&m_bOscRetrigOnOff_OscB, parameters, 1, "m_bOscRetrigOnOff_OscB", "OSC B Random phase offeset for each oscillator for each new note or sync at beginning of phase", "Random phase offeset for each oscillator for each new note or sync at beginning of phase", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 1.0f, 1.0f), 1.0f,
		CVASTParamState::toggleButtonValueToTextFunction,
		CVASTParamState::toggleButtonTextToValueFunction,
		false, true, true, false);

	effect->createAndAddParameter(&m_uVCAEnv_OscB, parameters, 1, "m_uVCAEnv_OscB", "OSC B Envelope used as VCA envelope", "OSC B Envelope used as VCA envelope", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, MSEGENV_Array.size() - 1, 1.0f), 0.0f,
		CVASTParamState::comboBoxValueToTextFunction_MSEGENV,
		CVASTParamState::comboBoxTextToValueFunction_MSEGENV,
		false, true, true, false);

	//Osc C
	effect->createAndAddParameter(&m_bOscOnOff_OscC, parameters, 1, "m_bOscOnOff_OscC", "OSC C Switch oscillator bank on / off", "Switch oscillator bank on / off", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 1.0f, 1.0f), 0.0f,
		CVASTParamState::toggleButtonValueToTextFunction,
		CVASTParamState::toggleButtonTextToValueFunction,
		false, true, true, false,
		true); //has to update oscs

	effect->createAndAddParameter(&m_uOscRouting1_OscC, parameters, 1, "m_uOscRouting1_OscC", "OSC C Oscillator routing 1", "Oscillator routing 1", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, OSCROUTE_Array.size() - 1, 1.0f), OSCROUTE_Array.indexOf("FXBus1"),
		CVASTParamState::comboBoxValueToTextFunction_OSCROUTE,
		CVASTParamState::comboBoxTextToValueFunction_OSCROUTE,
		false, true, true, false);

	effect->createAndAddParameter(&m_uOscRouting2_OscC, parameters, 1, "m_uOscRouting2_OscC", "OSC C Oscillator routing 2", "Oscillator routing 2", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, OSCROUTE_Array.size() - 1, 1.0f), 0.0f,
		CVASTParamState::comboBoxValueToTextFunction_OSCROUTE,
		CVASTParamState::comboBoxTextToValueFunction_OSCROUTE,
		false, true, true, false);

	effect->createAndAddParameter(&m_fOscDetune_OscC, parameters, 1, "m_fOscDetune_OscC", "OSC C Normalized detune with bank oscillators", "Normalized detune with bank oscillators", 0,
		MODMATDEST::OscCDetune, //tested: OK
		NormalisableRange<float>(0, 100, 0.00001f, 0.3f, false), 0.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_fOscGain_OscC, parameters, 1, "m_fOscGain_OscC", "OSC C Bank gain (100 full, >100 overdrive)", "Bank gain (100 full, >100 overdrive)", 0,
		MODMATDEST::OscCGain, //tested: OK
		NormalisableRange<float>(0, 200), 100,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);

	effect->createAndAddParameter(&m_fOscCents_OscC, parameters, 1, "m_fOscCents_OscC", "OSC C Bank cents (+/- 1 octave and half tones)", "Bank cents (+/- 1 octave and half tones)", 0,
		MODMATDEST::OscCCents, //tested: OK
		NormalisableRange<float>(-2400, 2400), 0,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);

	effect->createAndAddParameter(&m_fOscWTPos_OscC, parameters, 1, "m_fOscWTPos_OscC", "OSC C Oscillator wavetable position", "Oscillator wavetable position", 0,
		MODMATDEST::OscCWTPos, //tested: OK
		NormalisableRange<float>(0, 100), 0,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_fOscMorph_OscC, parameters, 1, "m_fOscMorph_OscC", "OSC C Oscillator morphing", "Oscillator morphing", 0,
		MODMATDEST::OscCMorph, //tested: OK
		NormalisableRange<float>(0, 100), 0,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_fSpread_OscC, parameters, 1, "m_fSpread_OscC", "OSC C Stereo spread within bank oscillators", "Stereo spread within bank oscillators", 0,
		MODMATDEST::OscCSpread, //tested: OK
		NormalisableRange<float>(0, 100), 0,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);

	effect->createAndAddParameter(&m_iNumOscs_OscC, parameters, 1, "m_iNumOscs_OscC", "OSC C Number of bank oscillators", "Number of bank oscillators", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(1, 24, 1), 1,
		CVASTParamState::intSliderValueToTextFunction,
		CVASTParamState::intSliderTextToValueFunction,
		false, true, true, false,
		true);

	effect->createAndAddParameter(&m_iOscOct_OscC, parameters, 1, "m_iOscOct_OscC", "OSC C Bank oscillators octave", "Bank oscillators octave", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(-4, 4, 1), 0,
		CVASTParamState::intSliderValueToTextFunction,
		CVASTParamState::intSliderTextToValueFunction,
		false, true, true, false,
		true);

	effect->createAndAddParameter(&m_fOscPan_OscC, parameters, 1, "m_fOscPan_OscC", "OSC C Stereo panning of oscillator", "Stereo panning of oscillator", 0,
		MODMATDEST::OscCPan, //tested: OK
		NormalisableRange<float>(-100, 100), 0,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);

	effect->createAndAddParameter(&m_fOscPhase_OscC, parameters, 1, "m_fOscPhase_OscC", "OSC C Phase of oscillator", "Phase of oscillator", 0,
		MODMATDEST::OscCPhase, //tested: OK
		NormalisableRange<float>(0, 100), 0,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);

	effect->createAndAddParameter(&m_bExciterOnOff_OscC, parameters, 1, "m_bExciterOnOff_OscC", "OSC C Exciter adds random detune by max +- 0.25hz per oscillator and played note", "Exciter adds random detune by max +- 0.25hz per oscillator and played note", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 1.0f, 1.0f), 0.0f,
		CVASTParamState::toggleButtonValueToTextFunction,
		CVASTParamState::toggleButtonTextToValueFunction,
		false, true, true, false);

	effect->createAndAddParameter(&m_bOscInverterOnOff_OscC, parameters, 1, "m_bOscInverterOnOff_OscC", "OSC C Invert all voices not in the center", "Invert all voices not in the center", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 1.0f, 1.0f), 0.0f,
		CVASTParamState::toggleButtonValueToTextFunction,
		CVASTParamState::toggleButtonTextToValueFunction,
		false, true, true, false);

	effect->createAndAddParameter(&m_bOscRetrigOnOff_OscC, parameters, 1, "m_bOscRetrigOnOff_OscC", "OSC C Random phase offeset for each oscillator for each new note or sync at beginning of phase", "Random phase offeset for each oscillator for each new note or sync at beginning of phase", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 1.0f, 1.0f), 1.0f,
		CVASTParamState::toggleButtonValueToTextFunction,
		CVASTParamState::toggleButtonTextToValueFunction,
		false, true, true, false);

	effect->createAndAddParameter(&m_uVCAEnv_OscC, parameters, 1, "m_uVCAEnv_OscC", "OSC C Envelope used as VCA envelope", "OSC C Envelope used as VCA envelope", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, MSEGENV_Array.size() - 1, 1.0f), 0.0f,
		CVASTParamState::comboBoxValueToTextFunction_MSEGENV,
		CVASTParamState::comboBoxTextToValueFunction_MSEGENV,
		false, true, true, false);

	//Osc D
	effect->createAndAddParameter(&m_bOscOnOff_OscD, parameters, 1, "m_bOscOnOff_OscD", "OSC D Switch oscillator bank on / off", "Switch oscillator bank on / off", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 1.0f, 1.0f), 0.0f,
		CVASTParamState::toggleButtonValueToTextFunction,
		CVASTParamState::toggleButtonTextToValueFunction,
		false, true, true, false,
		true); //has to update oscs

	effect->createAndAddParameter(&m_uOscRouting1_OscD, parameters, 1, "m_uOscRouting1_OscD", "OSC D Oscillator routing 1", "Oscillator routing 1", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, OSCROUTE_Array.size() - 1, 1.0f), OSCROUTE_Array.indexOf("FXBus1"),
		CVASTParamState::comboBoxValueToTextFunction_OSCROUTE,
		CVASTParamState::comboBoxTextToValueFunction_OSCROUTE,
		false, true, true, false);

	effect->createAndAddParameter(&m_uOscRouting2_OscD, parameters, 1, "m_uOscRouting2_OscD", "OSC D Oscillator routing 2", "Oscillator routing 2", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, OSCROUTE_Array.size() - 1, 1.0f), 0.0f,
		CVASTParamState::comboBoxValueToTextFunction_OSCROUTE,
		CVASTParamState::comboBoxTextToValueFunction_OSCROUTE,
		false, true, true, false);

	effect->createAndAddParameter(&m_fOscDetune_OscD, parameters, 1, "m_fOscDetune_OscD", "OSC D Normalized detune with bank oscillators", "Normalized detune with bank oscillators", 0,
		MODMATDEST::OscDDetune, //tested: OK
		NormalisableRange<float>(0, 100, 0.00001f, 0.3f, false), 0.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_fOscGain_OscD, parameters, 1, "m_fOscGain_OscD", "OSC D Bank gain (100 full, >100 overdrive)", "Bank gain (100 full, >100 overdrive)", 0,
		MODMATDEST::OscDGain, //tested: OK
		NormalisableRange<float>(0, 200), 100,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);

	effect->createAndAddParameter(&m_fOscCents_OscD, parameters, 1, "m_fOscCents_OscD", "OSC D Bank cents (+/- 1 octave and half tones)", "Bank cents (+/- 1 octave and half tones)", 0,
		MODMATDEST::OscDCents, //tested: OK
		NormalisableRange<float>(-2400, 2400), 0,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);

	effect->createAndAddParameter(&m_fOscWTPos_OscD, parameters, 1, "m_fOscWTPos_OscD", "OSC D Oscillator wavetable position", "Oscillator wavetable position", 0,
		MODMATDEST::OscDWTPos, //tested: OK
		NormalisableRange<float>(0, 100), 0,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_fOscMorph_OscD, parameters, 1, "m_fOscMorph_OscD", "OSC D Oscillator morphing", "Oscillator morphing", 0,
		MODMATDEST::OscDMorph, //tested: OK
		NormalisableRange<float>(0, 100), 0,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_fSpread_OscD, parameters, 1, "m_fSpread_OscD", "OSC D Stereo spread within bank oscillators", "Stereo spread within bank oscillators", 0,
		MODMATDEST::OscDSpread, //tested: OK
		NormalisableRange<float>(0, 100), 0,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);

	effect->createAndAddParameter(&m_iNumOscs_OscD, parameters, 1, "m_iNumOscs_OscD", "OSC D Number of bank oscillators", "Number of bank oscillators", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(1, 24, 1), 1,
		CVASTParamState::intSliderValueToTextFunction,
		CVASTParamState::intSliderTextToValueFunction,
		false, true, true, false,
		true);

	effect->createAndAddParameter(&m_iOscOct_OscD, parameters, 1, "m_iOscOct_OscD", "OSC D Bank oscillators octave", "Bank oscillators octave", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(-4, 4, 1), 0,
		CVASTParamState::intSliderValueToTextFunction,
		CVASTParamState::intSliderTextToValueFunction,
		false, true, true, false,
		true);

	effect->createAndAddParameter(&m_fOscPan_OscD, parameters, 1, "m_fOscPan_OscD", "OSC D Stereo panning of oscillator", "Stereo panning of oscillator", 0,
		MODMATDEST::OscDPan, //tested: OK
		NormalisableRange<float>(-100, 100), 0,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);

	effect->createAndAddParameter(&m_fOscPhase_OscD, parameters, 1, "m_fOscPhase_OscD", "OSC D Phase of oscillator", "Phase of oscillator", 0,
		MODMATDEST::OscDPhase, //tested: OK
		NormalisableRange<float>(0, 100), 0,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);

	effect->createAndAddParameter(&m_bExciterOnOff_OscD, parameters, 1, "m_bExciterOnOff_OscD", "OSC D Exciter adds random detune by max +- 0.25hz per oscillator and played note", "Exciter adds random detune by max +- 0.25hz per oscillator and played note", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 1.0f, 1.0f), 0.0f,
		CVASTParamState::toggleButtonValueToTextFunction,
		CVASTParamState::toggleButtonTextToValueFunction,
		false, true, true, false);

	effect->createAndAddParameter(&m_bOscInverterOnOff_OscD, parameters, 1, "m_bOscInverterOnOff_OscD", "OSC D Invert all voices not in the center", "Invert all voices not in the center", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 1.0f, 1.0f), 0.0f,
		CVASTParamState::toggleButtonValueToTextFunction,
		CVASTParamState::toggleButtonTextToValueFunction,
		false, true, true, false);

	effect->createAndAddParameter(&m_bOscRetrigOnOff_OscD, parameters, 1, "m_bOscRetrigOnOff_OscD", "OSC D Random phase offeset for each oscillator for each new note or sync at beginning of phase", "Random phase offeset for each oscillator for each new note or sync at beginning of phase", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 1.0f, 1.0f), 1.0f,
		CVASTParamState::toggleButtonValueToTextFunction,
		CVASTParamState::toggleButtonTextToValueFunction,
		false, true, true, false);

	effect->createAndAddParameter(&m_uVCAEnv_OscD, parameters, 1, "m_uVCAEnv_OscD", "OSC D Envelope used as VCA envelope", "OSC D Envelope used as VCA envelope", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, MSEGENV_Array.size() - 1, 1.0f), 0.0f,
		CVASTParamState::comboBoxValueToTextFunction_MSEGENV,
		CVASTParamState::comboBoxTextToValueFunction_MSEGENV,
		false, true, true, false);

	//noise
	effect->createAndAddParameter(&m_bNoiseOnOff, parameters, 1, "m_bNoiseOnOff", "Noise On / off", "Switch noise oscillator on / off", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 1.0f, 1.0f), 0.0f,
		CVASTParamState::toggleButtonValueToTextFunction,
		CVASTParamState::toggleButtonTextToValueFunction,
		false, true, true, false,
		true); //has to update oscs

	effect->createAndAddParameter(&m_uNoiseRouting1, parameters, 1, "m_uNoiseRouting1", "Noise Routing 1", "Noise Routing 1", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, OSCROUTE_Array.size() - 1, 1.0f), OSCROUTE_Array.indexOf("FXBus1"),
		CVASTParamState::comboBoxValueToTextFunction_OSCROUTE,
		CVASTParamState::comboBoxTextToValueFunction_OSCROUTE,
		false, true, true, false);

	effect->createAndAddParameter(&m_uNoiseRouting2, parameters, 1, "m_uNoiseRouting2", "Noise Routing 2", "Noise Routing 2", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, OSCROUTE_Array.size() - 1, 1.0f), 0.0f,
		CVASTParamState::comboBoxValueToTextFunction_OSCROUTE,
		CVASTParamState::comboBoxTextToValueFunction_OSCROUTE,
		false, true, true, false);

	effect->createAndAddParameter(&m_uNoiseType, parameters, 1, "m_uNoiseType", "Noise Type of generation", "Type", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, NOISETYPE_Array.size() - 1, 1.0f), 0.0f,
		CVASTParamState::comboBoxValueToTextFunction_NOISETYPE,
		CVASTParamState::comboBoxTextToValueFunction_NOISETYPE,
		false, true, true, false);

	effect->createAndAddParameter(&m_fNoiseGain, parameters, 1, "m_fNoiseGain", "Noise Gain (100 full, >100 overdrive)", "Noise gain (100 full, >100 overdrive)", 0,
		MODMATDEST::NoiseGain, //tested OK
		NormalisableRange<float>(0, 200, 0.001f, 0.3f, false), 100.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);

	effect->createAndAddParameter(&m_fNoisePan, parameters, 1, "m_fNoisePan", "Noise Stereo panning", "Stereo panning of noise oscillator", 0,
		MODMATDEST::NoisePan, //tested OK
		NormalisableRange<float>(-100, 100), 0,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);

	effect->createAndAddParameter(&m_fNoiseResonance, parameters, 1, "m_fNoiseResonance", "Noise Resonance", "Noise Resonance", 0,
		MODMATDEST::NoiseResonance, //tested OK
		NormalisableRange<float>(0, 100, 0.0001f, 0.3f, false), 0,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);

	effect->createAndAddParameter(&m_uVCAEnv_Noise, parameters, 1, "m_uVCAEnv_Noise", "Noise Envelope", "Envelope used as noise envelope", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, MSEGENV_Array.size() - 1, 1.0f), 0.0f,
		CVASTParamState::comboBoxValueToTextFunction_MSEGENV,
		CVASTParamState::comboBoxTextToValueFunction_MSEGENV,
		false, true, true, false);

	effect->createAndAddParameter(&m_fNoiseLowCut, parameters, 1, "m_fNoiseLowCut", "Noise Low Frequency cutoff", "Noise oscillator low frequency cutoff", 0,
		MODMATDEST::NoiseLowCut, //tested OK
		NormalisableRange<float>(100, 18000.f, 0.001f, 0.3f, false), 100,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);

	effect->createAndAddParameter(&m_fNoiseHighCut, parameters, 1, "m_fNoiseHighCut", "Noise High Frequency cutoff", "Noise oscillator high frequency cutoff", 0,
		MODMATDEST::NoiseHighCut, //tested OK
		NormalisableRange<float>(500.f, 18000.f, 0.001f, 1.3f, false), 18000.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);

	// sampler
	effect->createAndAddParameter(&m_bSamplerOnOff, parameters, 1, "m_bSamplerOnOff", "Sampler On / off", "Switch sampler on / off", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 1.0f, 1.0f), 0.0f,
		CVASTParamState::toggleButtonValueToTextFunction,
		CVASTParamState::toggleButtonTextToValueFunction,
		false, true, true, false);

	effect->createAndAddParameter(&m_uSamplerRouting1, parameters, 1, "m_uSamplerRouting1", "Sampler Routing 1", "Sampler Routing 1", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, OSCROUTE_Array.size() - 1, 1.0f), OSCROUTE_Array.indexOf("FXBus1"),
		CVASTParamState::comboBoxValueToTextFunction_OSCROUTE,
		CVASTParamState::comboBoxTextToValueFunction_OSCROUTE,
		false, true, true, false);

	effect->createAndAddParameter(&m_uSamplerRouting2, parameters, 1, "m_uSamplerRouting2", "Sampler Routing 2", "Sampler Routing 2", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, OSCROUTE_Array.size() - 1, 1.0f), 0.0f,
		CVASTParamState::comboBoxValueToTextFunction_OSCROUTE,
		CVASTParamState::comboBoxTextToValueFunction_OSCROUTE,
		false, true, true, false);

	effect->createAndAddParameter(&m_fSamplerGain, parameters, 1, "m_fSamplerGain", "Sampler Gain", "Sampler Gain", 0,
		MODMATDEST::SamplerGain, //tested OK
		NormalisableRange<float>(0, 200, 0.001f, 0.3f, false), 100.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);

	effect->createAndAddParameter(&m_bSamplerKeytrack, parameters, 1, "m_bSamplerKeytrack", "Sampler Key tracking on / off", "Sampler Key tracking on / off", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 1.0f, 1.0f), 1.0f,
		CVASTParamState::toggleButtonValueToTextFunction,
		CVASTParamState::toggleButtonTextToValueFunction,
		false, true, true, false,
		true);

	effect->createAndAddParameter(&m_uSamplerRootKey, parameters, 1, "m_uSamplerRootKey", "Sampler Root key", "Sampler Root key", 0,
		MODMATDEST::NoDestination, 
		NormalisableRange<float>(0, 127, 1), MIDINOTES_Array.indexOf("c3"),  //midi note 60 is C3
		CVASTParamState::comboBoxValueToTextFunction_MIDINOTES,
		CVASTParamState::comboBoxTextToValueFunction_MIDINOTES,
		false, true, true, false,
		true);

	effect->createAndAddParameter(&m_fSamplerCents, parameters, 1, "m_fSamplerCents", "Sampler Cents (+/- 1 octave and half tones)", "Sampler Cents (+/- 1 octave and half tones)", 0,
		MODMATDEST::SamplerPitch, //tested: OK
		NormalisableRange<float>(-2400, 2400), 0,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_bSamplerLoop, parameters, 1, "m_bSamplerLoop", "Sampler Loop on / off", "Sampler Loop on / off", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 1.0f, 1.0f), 0.0f,
		CVASTParamState::toggleButtonValueToTextFunction,
		CVASTParamState::toggleButtonTextToValueFunction,
		false, true, true, false);

	effect->createAndAddParameter(&m_bSamplerLoopOnly, parameters, 1, "m_bSamplerLoopOnly", "Sampler Loop only without start", "Sampler Loop only without start", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 1.0f, 1.0f), 0.0f,
		CVASTParamState::toggleButtonValueToTextFunction,
		CVASTParamState::toggleButtonTextToValueFunction,
		false, true, true, false);

	effect->createAndAddParameter(&m_bSamplerLoopPingpong, parameters, 1, "m_bSamplerLoopPingpong", "Sampler Loop pingpong mode", "Sampler Loop pingpong mode", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 1.0f, 1.0f), 0.0f,
		CVASTParamState::toggleButtonValueToTextFunction,
		CVASTParamState::toggleButtonTextToValueFunction,
		false, true, true, false);

	effect->createAndAddParameter(&m_fSamplerLoopRange, parameters, 1, "m_fSamplerLoopRange", "Sampler Loop range", "Sampler Loop range", 0,
		MODMATDEST::SamplerLoopRange,
		NormalisableRange<float>(1.0f, 100.0f), 100.0f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);

	effect->createAndAddParameter(&m_bSamplerPoly, parameters, 1, "m_bSamplerPoly", "Sampler Polyphonic or monophonic mode", "Play sample in polyphonic or monophonic mode", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 1.0f, 1.0f), 1.0f,
		CVASTParamState::toggleButtonValueToTextFunction,
		CVASTParamState::toggleButtonTextToValueFunction,
		false, true, true, false);

	effect->createAndAddParameter(&m_fSamplerPan, parameters, 1, "m_fSamplerPan", "Sampler Stereo panning", "Stereo panning of sampler", 0,
		MODMATDEST::SamplerPan, //tested OK
		NormalisableRange<float>(-100, 100), 0,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);

	effect->createAndAddParameter(&m_uVCAEnv_Sampler, parameters, 1, "m_uVCAEnv_Sampler", "Sampler Envelope", "Envelope used as sampler envelope", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, MSEGENV_Array.size() - 1, 1.0f), 0.0f,
		CVASTParamState::comboBoxValueToTextFunction_MSEGENV,
		CVASTParamState::comboBoxTextToValueFunction_MSEGENV,
		false, true, true, false);

	//filter
	//filter 1
	effect->createAndAddParameter(&m_bOnOff_Filter1, parameters, 1, "m_bOnOff_Filter1", "Filter 1 On / off", "Switch filter 1 on / off", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 1.0f, 1.0f), 0.0f,
		CVASTParamState::toggleButtonValueToTextFunction,
		CVASTParamState::toggleButtonTextToValueFunction,
		false, true, true, false,
		true);

	effect->createAndAddParameter(&m_uFilterRouting_Filter1, parameters, 1, "m_uFilterRouting_Filter1", "Filter 1 Routing", "Filter 1 Routing", 0, //CHECK Own Array
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, FILTER1ROUTE_Array.size() - 1, 1.0f), FILTER1ROUTE_Array.indexOf("FXBus1"),
		CVASTParamState::comboBoxValueToTextFunction_FILTER1ROUTE,
		CVASTParamState::comboBoxTextToValueFunction_FILTER1ROUTE,
		false, true, true, false);

	effect->createAndAddParameter(&m_uFilterRouting2_Filter1, parameters, 1, "m_uFilterRouting2_Filter1", "Filter 1 Routing 2", "Filter 1 Routing 2", 0, //CHECK Own Array
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, FILTER1ROUTE_Array.size() - 1, 1.0f), 0.f,
		CVASTParamState::comboBoxValueToTextFunction_FILTER1ROUTE,
		CVASTParamState::comboBoxTextToValueFunction_FILTER1ROUTE,
		false, true, true, false);

	effect->createAndAddParameter(&m_uFilterType_Filter1, parameters, 1, "m_uFilterType_Filter1", "Filter 1 Type", "Filter 1 Type", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, FILTERTYPE_Array.size() - 1, 1.0f), comboBoxTextToValueFunction_FILTERTYPE("DQF LPF 12"),
		CVASTParamState::comboBoxValueToTextFunction_FILTERTYPE,
		CVASTParamState::comboBoxTextToValueFunction_FILTERTYPE,
		false, true, true, false);

	effect->createAndAddParameter(&m_fFilterCutoff_Filter1, parameters, 1, "m_fFilterCutoff_Filter1", "Filter 1 Cutoff (hz)", "Filter 1 Cutoff (hz)", 0,
		MODMATDEST::Filter1Cutoff, //tested: OK
		NormalisableRange<float>(20, 28000.f, 0.001f, 0.3f, false), 28000.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);

	effect->createAndAddParameter(&m_fFilterReso_Filter1, parameters, 1, "m_fFilterReso_Filter1", "Filter 1 Resonance", "Filter 1 Resonance", 0,
		MODMATDEST::Filter1Resonance, //tested: OK
		NormalisableRange<float>(0, 100, 0.0001f, 0.3f, false), 0,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);

	effect->createAndAddParameter(&m_fFilterDrive_Filter1, parameters, 1, "m_fFilterDrive_Filter1", "Filter 1 Drive in dB - pre gain (0 - full, >0 overdrive)", "Filter 1 Drive in dB - pre gain (0 - full, >0 overdrive)", 0,
		MODMATDEST::Filter1Drive, //tested: OK
		NormalisableRange<float>(-20, 20), 0,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);

	effect->createAndAddParameter(&m_fFilterDryWet_Filter1, parameters, 1, "m_fFilterDryWet_Filter1", "Filter 1 Dry - wet", "Filter 1 Dry - wet", 0,
		MODMATDEST::Filter1Mixin, //tested: OK
		NormalisableRange<float>(0, 100), 100,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);

	effect->createAndAddParameter(&m_uVCFEnv_Filter1, parameters, 1, "m_uVCFEnv_Filter1", "Filter 1 Envelope", "Envelope used as filter 1 envelope", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, MSEGENV_Array.size() - 1, 1.0f), MSEGENV::MSEG2,
		CVASTParamState::comboBoxValueToTextFunction_MSEGENV,
		CVASTParamState::comboBoxTextToValueFunction_MSEGENV,		
		false, true, true, false);	

	effect->createAndAddParameter(&m_fVCFEnvMod_Filter1, parameters, 1, "m_fVCFEnvMod_Filter1", "Filter 1 VCF envelope modulation", "Filter 1 VCF envelope modulation", 0,
		MODMATDEST::Filter1EnvMod,
		NormalisableRange<float>(-100, 100), 100,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);

	effect->createAndAddParameter(&m_fFilterScale_Filter1, parameters, 1, "m_fFilterScale_Filter1", "Filter 1 scale (gain for peak, shelf filters)", "Filter 1 scale (gain for notch, peak, shelf filters)", 0,
		MODMATDEST::Filter1Scale,
		NormalisableRange<float>(0, 200), 100,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);

	effect->createAndAddParameter(&m_fFilterGain_Filter1, parameters, 1, "m_fFilterGain_Filter1", "Filter 1 output gain", "Filter 1 output gain", 0,
		MODMATDEST::Filter1Gain,
		NormalisableRange<float>(0, 200), 100,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);

	//filter2
	effect->createAndAddParameter(&m_bOnOff_Filter2, parameters, 1, "m_bOnOff_Filter2", "Filter 2 On / off", "Switch filter 2 on / off", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 1.0f, 1.0f), 0.0f,
		CVASTParamState::toggleButtonValueToTextFunction,
		CVASTParamState::toggleButtonTextToValueFunction,
		false, true, true, false,
		true);

	effect->createAndAddParameter(&m_uFilterRouting_Filter2, parameters, 1, "m_uFilterRouting_Filter2", "Filter 2 Routing", "Filter 2 Routing", 0, //CHECK Own Array
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, FILTER2ROUTE_Array.size() - 1, 1.0f), FILTER2ROUTE_Array.indexOf("FXBus1"),
		CVASTParamState::comboBoxValueToTextFunction_FILTER2ROUTE,
		CVASTParamState::comboBoxTextToValueFunction_FILTER2ROUTE,
		false, true, true, false);

	effect->createAndAddParameter(&m_uFilterRouting2_Filter2, parameters, 1, "m_uFilterRouting2_Filter2", "Filter 2 Routing 2", "Filter 2 Routing 2", 0, //CHECK Own Array
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, FILTER2ROUTE_Array.size() - 1, 1.0f), 0.f,
		CVASTParamState::comboBoxValueToTextFunction_FILTER2ROUTE,
		CVASTParamState::comboBoxTextToValueFunction_FILTER2ROUTE,
		false, true, true, false);

	effect->createAndAddParameter(&m_uFilterType_Filter2, parameters, 1, "m_uFilterType_Filter2", "Filter 2 Type", "Filter 2 Type", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, FILTERTYPE_Array.size() - 1, 1.0f), comboBoxTextToValueFunction_FILTERTYPE("DQF HPF 12"),
		CVASTParamState::comboBoxValueToTextFunction_FILTERTYPE,
		CVASTParamState::comboBoxTextToValueFunction_FILTERTYPE,
		false, true, true, false);

	effect->createAndAddParameter(&m_fFilterCutoff_Filter2, parameters, 1, "m_fFilterCutoff_Filter2", "Filter 2 Cutoff (hz)", "Filter 2 Cutoff (hz)", 0,
		MODMATDEST::Filter2Cutoff, //tested: OK
		NormalisableRange<float>(20, 28000.f, 0.001f, 0.3f, false), 28000.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);
	
	effect->createAndAddParameter(&m_fFilterReso_Filter2, parameters, 1, "m_fFilterReso_Filter2", "Filter 2 Resonance", "Filter 2 Resonance", 0,
		MODMATDEST::Filter2Resonance, //tested: OK
		NormalisableRange<float>(0, 100, 0.0001f, 0.3f, false), 0,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);

	effect->createAndAddParameter(&m_fFilterDrive_Filter2, parameters, 1, "m_fFilterDrive_Filter2", "Filter 2 Drive in dB - pre gain (0 - full, >0 overdrive)", "Filter 2 Drive in dB - pre gain (0 - full, >0 overdrive)", 0,
		MODMATDEST::Filter2Drive, //tested: OK
		NormalisableRange<float>(-20, 20), 0,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);

	effect->createAndAddParameter(&m_fFilterDryWet_Filter2, parameters, 1, "m_fFilterDryWet_Filter2", "Filter 2 Dry - wet", "Filter 2 Dry - wet", 0,
		MODMATDEST::Filter2Mixin, //tested: OK
		NormalisableRange<float>(0, 100), 100,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);

	effect->createAndAddParameter(&m_uVCFEnv_Filter2, parameters, 1, "m_uVCFEnv_Filter2", "Filter 2 Envelope", "Envelope used as filter 2 envelope", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, MSEGENV_Array.size() - 1, 1.0f), MSEGENV::MSEG2,
		CVASTParamState::comboBoxValueToTextFunction_MSEGENV,
		CVASTParamState::comboBoxTextToValueFunction_MSEGENV,
		false, true, true, false);

	effect->createAndAddParameter(&m_fVCFEnvMod_Filter2, parameters, 1, "m_fVCFEnvMod_Filter2", "Filter 2 VCF envelope modulation", "Filter 2 VCF envelope modulation", 0,
		MODMATDEST::Filter2EnvMod,
		NormalisableRange<float>(-100, 100), 100,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);

	effect->createAndAddParameter(&m_fFilterScale_Filter2, parameters, 1, "m_fFilterScale_Filter2", "Filter 2 scale (gain for peak, shelf filters)", "Filter 2 scale (gain for notch, peak, shelf filters)", 0,
		MODMATDEST::Filter2Scale,
		NormalisableRange<float>(0, 200), 100,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);

	effect->createAndAddParameter(&m_fFilterGain_Filter2, parameters, 1, "m_fFilterGain_Filter2", "Filter 2 output gain", "Filter 2 output gain", 0,
		MODMATDEST::Filter2Gain,
		NormalisableRange<float>(0, 200), 100,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);

	//filter 3
	effect->createAndAddParameter(&m_bOnOff_Filter3, parameters, 1, "m_bOnOff_Filter3", "Filter 3 On / off", "Switch filter 3 on / off", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 1.0f, 1.0f), 0.0f,
		CVASTParamState::toggleButtonValueToTextFunction,
		CVASTParamState::toggleButtonTextToValueFunction,
		false, true, true, false,
		true);

	effect->createAndAddParameter(&m_uFilterRouting_Filter3, parameters, 1, "m_uFilterRouting_Filter3", "Filter 3 Routing", "Filter 3 Routing", 0, //CHECK Own Array
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, FILTER3ROUTE_Array.size() - 1, 1.0f), FILTER3ROUTE_Array.indexOf("FXBus1"),
		CVASTParamState::comboBoxValueToTextFunction_FILTER3ROUTE,
		CVASTParamState::comboBoxTextToValueFunction_FILTER3ROUTE,
		false, true, true, false);

	effect->createAndAddParameter(&m_uFilterRouting2_Filter3, parameters, 1, "m_uFilterRouting2_Filter3", "Filter 3 Routing 2", "Filter 3 Routing 2", 0, //CHECK Own Array
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, FILTER3ROUTE_Array.size() - 1, 1.0f), 0.f,
		CVASTParamState::comboBoxValueToTextFunction_FILTER3ROUTE,
		CVASTParamState::comboBoxTextToValueFunction_FILTER3ROUTE,
		false, true, true, false);

	effect->createAndAddParameter(&m_uFilterType_Filter3, parameters, 1, "m_uFilterType_Filter3", "Filter 3 Type", "Filter 3 Type", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, FILTERTYPE_Array.size() - 1, 1.0f), comboBoxTextToValueFunction_FILTERTYPE("DQF LDF 12"),
		CVASTParamState::comboBoxValueToTextFunction_FILTERTYPE,
		CVASTParamState::comboBoxTextToValueFunction_FILTERTYPE,
		false, true, true, false);

	effect->createAndAddParameter(&m_fFilterCutoff_Filter3, parameters, 1, "m_fFilterCutoff_Filter3", "Filter 3 Cutoff (hz)", "Filter 3 Cutoff (hz)", 0,
		MODMATDEST::Filter3Cutoff, //tested: OK
		NormalisableRange<float>(20, 28000.f, 0.001f, 0.3f, false), 28000.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);

	effect->createAndAddParameter(&m_fFilterReso_Filter3, parameters, 1, "m_fFilterReso_Filter3", "Filter 3 Resonance", "Filter 3 Resonance", 0,
		MODMATDEST::Filter3Resonance, //tested: OK
		NormalisableRange<float>(0, 100, 0.0001f, 0.3f, false), 0,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);

	effect->createAndAddParameter(&m_fFilterDrive_Filter3, parameters, 1, "m_fFilterDrive_Filter3", "Filter 3 Drive in dB - pre gain (0 - full, >0 overdrive)", "Filter 3 Drive in dB - pre gain (0 - full, >0 overdrive)", 0,
		MODMATDEST::Filter3Drive, //tested: OK
		NormalisableRange<float>(-20, 20), 0,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);
	
	effect->createAndAddParameter(&m_fFilterDryWet_Filter3, parameters, 1, "m_fFilterDryWet_Filter3", "Filter 3 Dry - wet", "Filter 3 Dry - wet", 0,
		MODMATDEST::Filter3Mixin, //tested: OK
		NormalisableRange<float>(0, 100), 100,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);

	effect->createAndAddParameter(&m_uVCFEnv_Filter3, parameters, 1, "m_uVCFEnv_Filter3", "Filter 3 Envelope", "Envelope used as filter 3 envelope", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, MSEGENV_Array.size() - 1, 1.0f), MSEGENV::MSEG2,
		CVASTParamState::comboBoxValueToTextFunction_MSEGENV,
		CVASTParamState::comboBoxTextToValueFunction_MSEGENV,
		false, true, true, false);

	effect->createAndAddParameter(&m_fVCFEnvMod_Filter3, parameters, 1, "m_fVCFEnvMod_Filter3", "Filter 3 VCF envelope modulation", "Filter 3 VCF envelope modulation", 0,
		MODMATDEST::Filter3EnvMod,
		NormalisableRange<float>(-100, 100), 100,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);

	effect->createAndAddParameter(&m_fFilterScale_Filter3, parameters, 1, "m_fFilterScale_Filter3", "Filter 3 scale (gain for peak, shelf filters)", "Filter 3 scale (gain for notch, peak, shelf filters)", 0,
		MODMATDEST::Filter3Scale,
		NormalisableRange<float>(0, 200), 100,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);

	effect->createAndAddParameter(&m_fFilterGain_Filter3, parameters, 1, "m_fFilterGain_Filter3", "Filter 3 output gain", "Filter 3 output gain", 0,
		MODMATDEST::Filter3Gain,
		NormalisableRange<float>(0, 200), 100,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);

	//lfo
	effect->createAndAddParameter(&m_fLFOFreq_LFO1, parameters, 1, "m_fLFOFreq_LFO1", "LFO 1 Frequency (overridden when synced to DAW)", "LFO 1 Frequency (overridden when synced to DAW)", 0,
		MODMATDEST::LFO1Frequency,
		NormalisableRange<float>(0, 2000, 0.000001f, 0.3f, false), 2.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_uLFOWave_LFO1, parameters, 1, "m_uLFOWave_LFO1", "LFO 1 Wave form", "LFO 1 Wave form", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, LFOWAVE_Array.size() - 1, 1.0f), WAVE::sine,
		CVASTParamState::comboBoxValueToTextFunction_LFOWAVE,
		CVASTParamState::comboBoxTextToValueFunction_LFOWAVE,
		false, true, true, false,
		true);

	effect->createAndAddParameter(&m_uLFOPolarity_LFO1, parameters, 1, "m_uLFOPolarity_LFO1", "LFO 1 Polarity (unipolar, bipolar)", "LFO 1 Polarity(unipolar, bipolar)", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, POLARITY_Array.size() - 1, 1.0f), 1.0f,
		CVASTParamState::comboBoxValueToTextFunction_POLARITY,
		CVASTParamState::comboBoxTextToValueFunction_POLARITY,
		false, true, false, false);

	effect->createAndAddParameter(&m_fLFORamp_LFO1, parameters, 1, "m_fLFORamp_LFO1", "LFO 1 Ramp beats", "LFO 1 Ramp beats", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 16.0f, 0.0001f, 0.3f, false), 1.0f, //skew
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,	
		false, true, false, false);

	effect->createAndAddParameter(&m_fLFOPhase_LFO1, parameters, 1, "m_fLFOPhase_LFO1", "LFO 1 Phase", "Phase of LFO 1", 0,
		MODMATDEST::NoDestination, 
		NormalisableRange<float>(0, 100), 0,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);

	effect->createAndAddParameter(&m_bLFOPerVoice_LFO1, parameters, 1, "m_bLFOPerVoice_LFO1", "LFO 1 Seperate LFO per voice or global", "LFO 1 Seperate LFO per voice or global", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 1.0f, 1.0f), 0.0f,
		CVASTParamState::toggleButtonValueToTextFunction,
		CVASTParamState::toggleButtonTextToValueFunction,
		false, true, true, false,
		true);

	effect->createAndAddParameter(&m_bLFORetrigOnOff_LFO1, parameters, 1, "m_bLFORetrigOnOff_LFO1", "LFO 1 Retrigger oscillator with every new note played (otherwise free running)", "Retrigger LFO 1 oscillator with every new note played (otherwise free running)", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 1.0f, 1.0f), 1.0f,
		CVASTParamState::toggleButtonValueToTextFunction,
		CVASTParamState::toggleButtonTextToValueFunction,
		false, true, true, false);

	effect->createAndAddParameter(&m_bLFOSynch_LFO1, parameters, 1, "m_bLFOSynch_LFO1", "LFO 1 Sync to DAW timecode (starts when played in DAW)", "Sync LFO 1 to DAW timecode (starts when played in DAW)", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 1.0f, 1.0f), 0.0f,
		CVASTParamState::toggleButtonValueToTextFunction,
		CVASTParamState::toggleButtonTextToValueFunction,
		false, true, true, false,
		true);

	effect->createAndAddParameter(&m_uLFOTimeBeats_LFO1, parameters, 1, "m_uLFOTimeBeats_LFO1", "LFO 1 Time in beats when synched to DAW", "Time (DAW)", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, TIMEBEATS_Array.size() - 1, 1.0f), TIMEBEATS::BEATS1,
		CVASTParamState::comboBoxValueToTextFunction_TIMEBEATS,
		CVASTParamState::comboBoxTextToValueFunction_TIMEBEATS,
		false, true, true, false,
		true);

	effect->createAndAddParameter(&m_fLFOFreq_LFO2, parameters, 1, "m_fLFOFreq_LFO2", "LFO 2 Frequency (overridden when synced to DAW)", "LFO 2 Frequency (overridden when synced to DAW)", 0,
		MODMATDEST::LFO2Frequency,
		NormalisableRange<float>(0, 2000, 0.000001f, 0.3f, false), 2.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_uLFOWave_LFO2, parameters, 1, "m_uLFOWave_LFO2", "LFO 2 Wave form", "LFO 2 Wave form", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, LFOWAVE_Array.size() - 1, 1.0f), WAVE::sine,
		CVASTParamState::comboBoxValueToTextFunction_LFOWAVE,
		CVASTParamState::comboBoxTextToValueFunction_LFOWAVE,
		false, true, true, false,
		true);

	effect->createAndAddParameter(&m_uLFOPolarity_LFO2, parameters, 1, "m_uLFOPolarity_LFO2", "LFO 2 Polarity (unipolar, bipolar)", "LFO 2 Polarity (unipolar, bipolar)", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, POLARITY_Array.size() - 1, 1.0f), 1.0f,
		CVASTParamState::comboBoxValueToTextFunction_POLARITY,
		CVASTParamState::comboBoxTextToValueFunction_POLARITY,
		false, true, false, false);

	effect->createAndAddParameter(&m_fLFORamp_LFO2, parameters, 1, "m_fLFORamp_LFO2", "LFO 2 Ramp beats", "LFO 2 Ramp beats", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 16.0f, 0.0001f, 0.3f, false), 1.0f, //skew
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);

	effect->createAndAddParameter(&m_fLFOPhase_LFO2, parameters, 1, "m_fLFOPhase_LFO2", "LFO 2 Phase", "Phase of LFO 2", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0, 100), 0,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);

	effect->createAndAddParameter(&m_bLFOPerVoice_LFO2, parameters, 1, "m_bLFOPerVoice_LFO2", "LFO 2 Seperate LFO per voice or global", "LFO 2 Seperate LFO per voice or global", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 1.0f, 1.0f), 0.0f,
		CVASTParamState::toggleButtonValueToTextFunction,
		CVASTParamState::toggleButtonTextToValueFunction,
		false, true, true, false,
		true);

	effect->createAndAddParameter(&m_bLFORetrigOnOff_LFO2, parameters, 1, "m_bLFORetrigOnOff_LFO2", "LFO 2 Retrigger oscillator with every new note played (otherwise free running)", "Retrigger LFO 2 oscillator with every new note played (otherwise free running)", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 1.0f, 1.0f), 1.0f,
		CVASTParamState::toggleButtonValueToTextFunction,
		CVASTParamState::toggleButtonTextToValueFunction,
		false, true, true, false);

	effect->createAndAddParameter(&m_bLFOSynch_LFO2, parameters, 1, "m_bLFOSynch_LFO2", "LFO 2 Sync to DAW timecode (starts when played in DAW)", "Sync LFO 2 to DAW timecode (starts when played in DAW)", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 1.0f, 1.0f), 0.0f,
		CVASTParamState::toggleButtonValueToTextFunction,
		CVASTParamState::toggleButtonTextToValueFunction,
		false, true, true, false,
		true);

	effect->createAndAddParameter(&m_uLFOTimeBeats_LFO2, parameters, 1, "m_uLFOTimeBeats_LFO2", "LFO 2 Time in beats when synched to DAW", "Time (DAW)", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, TIMEBEATS_Array.size() - 1, 1.0f), TIMEBEATS::BEATS1,
		CVASTParamState::comboBoxValueToTextFunction_TIMEBEATS,
		CVASTParamState::comboBoxTextToValueFunction_TIMEBEATS,
		false, true, true, false,
		true);

	effect->createAndAddParameter(&m_fLFOFreq_LFO3, parameters, 1, "m_fLFOFreq_LFO3", "LFO 3 Frequency (overridden when synced to DAW)", "LFO 3 Frequency (overridden when synced to DAW)", 0,
		MODMATDEST::LFO3Frequency,
		NormalisableRange<float>(0, 2000, 0.000001f, 0.3f, false), 2.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_uLFOWave_LFO3, parameters, 1, "m_uLFOWave_LFO3", "LFO 3 Wave form", "LFO 3 Wave form", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, LFOWAVE_Array.size() - 1, 1.0f), WAVE::sine,
		CVASTParamState::comboBoxValueToTextFunction_LFOWAVE,
		CVASTParamState::comboBoxTextToValueFunction_LFOWAVE,
		false, true, true, false,
		true);

	effect->createAndAddParameter(&m_uLFOPolarity_LFO3, parameters, 1, "m_uLFOPolarity_LFO3", "LFO 3 Polarity (unipolar, bipolar)", "LFO 3 Polarity(unipolar, bipolar)", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, POLARITY_Array.size() - 1, 1.0f), 1.0f,
		CVASTParamState::comboBoxValueToTextFunction_POLARITY,
		CVASTParamState::comboBoxTextToValueFunction_POLARITY,
		false, true, false, false);

	effect->createAndAddParameter(&m_fLFORamp_LFO3, parameters, 1, "m_fLFORamp_LFO3", "LFO 3 Ramp beats", "LFO 3 ramp beats", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 16.0f, 0.0001f, 0.3f, false), 1.0f, //skew
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);

	effect->createAndAddParameter(&m_fLFOPhase_LFO3, parameters, 1, "m_fLFOPhase_LFO3", "LFO 3 Phase", "Phase of LFO 3", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0, 100), 0,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);

	effect->createAndAddParameter(&m_bLFOPerVoice_LFO3, parameters, 1, "m_bLFOPerVoice_LFO3", "LFO 3 Seperate LFO per voice or global", "LFO 3 seperate LFO per voice or global", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 1.0f, 1.0f), 0.0f,
		CVASTParamState::toggleButtonValueToTextFunction,
		CVASTParamState::toggleButtonTextToValueFunction,
		false, true, true, false,
		true);

	effect->createAndAddParameter(&m_bLFORetrigOnOff_LFO3, parameters, 1, "m_bLFORetrigOnOff_LFO3", "LFO 3 Retrigger oscillator with every new note played (otherwise free running)", "Retrigger LFO 3 oscillator with every new note played (otherwise free running)", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 1.0f, 1.0f), 1.0f,
		CVASTParamState::toggleButtonValueToTextFunction,
		CVASTParamState::toggleButtonTextToValueFunction,
		false, true, true, false);

	effect->createAndAddParameter(&m_bLFOSynch_LFO3, parameters, 1, "m_bLFOSynch_LFO3", "LFO 3 Sync to DAW timecode (starts when played in DAW)", "Sync LFO 3 to DAW timecode (starts when played in DAW)", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 1.0f, 1.0f), 0.0f,
		CVASTParamState::toggleButtonValueToTextFunction,
		CVASTParamState::toggleButtonTextToValueFunction,
		false, true, true, false,
		true);

	effect->createAndAddParameter(&m_uLFOTimeBeats_LFO3, parameters, 1, "m_uLFOTimeBeats_LFO3", "LFO 3 Time in beats when synched to DAW", "Time (DAW)", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, TIMEBEATS_Array.size() - 1, 1.0f), TIMEBEATS::BEATS1,
		CVASTParamState::comboBoxValueToTextFunction_TIMEBEATS,
		CVASTParamState::comboBoxTextToValueFunction_TIMEBEATS,
		false, true, true, false,
		true);

	effect->createAndAddParameter(&m_fLFOFreq_LFO4, parameters, 1, "m_fLFOFreq_LFO4", "LFO 4 Frequency (overridden when synced to DAW)", "LFO 4 Frequency (overridden when synced to DAW)", 0,
		MODMATDEST::LFO4Frequency,
		NormalisableRange<float>(0, 2000, 0.000001f, 0.3f, false), 2.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_uLFOWave_LFO4, parameters, 1, "m_uLFOWave_LFO4", "LFO 4 Wave form", "LFO 4 Wave form", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, LFOWAVE_Array.size() - 1, 1.0f), WAVE::sine,
		CVASTParamState::comboBoxValueToTextFunction_LFOWAVE,
		CVASTParamState::comboBoxTextToValueFunction_LFOWAVE,
		false, true, true, false,
		true);

	effect->createAndAddParameter(&m_uLFOPolarity_LFO4, parameters, 1, "m_uLFOPolarity_LFO4", "LFO 4 Polarity (unipolar, bipolar)", "LFO 4 Polarity(unipolar, bipolar)", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, POLARITY_Array.size() - 1, 1.0f), 1.0f,
		CVASTParamState::comboBoxValueToTextFunction_POLARITY,
		CVASTParamState::comboBoxTextToValueFunction_POLARITY,
		false, true, false, false);

	effect->createAndAddParameter(&m_fLFORamp_LFO4, parameters, 1, "m_fLFORamp_LFO4", "LFO 4 Ramp beats", "LFO 4 ramp beats", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 16.0f, 0.0001f, 0.3f, false), 1.0f, //skew
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);

	effect->createAndAddParameter(&m_fLFOPhase_LFO4, parameters, 1, "m_fLFOPhase_LFO4", "LFO 4 Phase", "Phase of LFO 4", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0, 100), 0,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);

	effect->createAndAddParameter(&m_bLFOPerVoice_LFO4, parameters, 1, "m_bLFOPerVoice_LFO4", "LFO 4 Seperate LFO per voice or global", "LFO 4 seperate LFO per voice or global", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 1.0f, 1.0f), 0.0f,
		CVASTParamState::toggleButtonValueToTextFunction,
		CVASTParamState::toggleButtonTextToValueFunction,
		false, true, true, false,
		true);

	effect->createAndAddParameter(&m_bLFORetrigOnOff_LFO4, parameters, 1, "m_bLFORetrigOnOff_LFO4", "LFO 4 Retrigger oscillator with every new note played (otherwise free running)", "Retrigger LFO 4 oscillator with every new note played (otherwise free running)", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 1.0f, 1.0f), 1.0f,
		CVASTParamState::toggleButtonValueToTextFunction,
		CVASTParamState::toggleButtonTextToValueFunction,
		false, true, true, false);

	effect->createAndAddParameter(&m_bLFOSynch_LFO4, parameters, 1, "m_bLFOSynch_LFO4", "LFO 4 Sync to DAW timecode (starts when played in DAW)", "Sync LFO 4 to DAW timecode (starts when played in DAW)", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 1.0f, 1.0f), 0.0f,
		CVASTParamState::toggleButtonValueToTextFunction,
		CVASTParamState::toggleButtonTextToValueFunction,
		false, true, true, false,
		true);

	effect->createAndAddParameter(&m_uLFOTimeBeats_LFO4, parameters, 1, "m_uLFOTimeBeats_LFO4", "LFO 4 Time in beats when synched to DAW", "Time (DAW)", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, TIMEBEATS_Array.size() - 1, 1.0f), TIMEBEATS::BEATS1,
		CVASTParamState::comboBoxValueToTextFunction_TIMEBEATS,
		CVASTParamState::comboBoxTextToValueFunction_TIMEBEATS,
		false, true, true, false,
		true);

	effect->createAndAddParameter(&m_fLFOFreq_LFO5, parameters, 1, "m_fLFOFreq_LFO5", "LFO 5 Frequency (overridden when synced to DAW)", "LFO 5 Frequency (overridden when synced to DAW)", 0,
		MODMATDEST::LFO5Frequency,
		NormalisableRange<float>(0, 2000, 0.000001f, 0.3f, false), 2.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_uLFOWave_LFO5, parameters, 1, "m_uLFOWave_LFO5", "LFO 5 Wave form", "LFO 5 Wave form", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, LFOWAVE_Array.size() - 1, 1.0f), WAVE::sine,
		CVASTParamState::comboBoxValueToTextFunction_LFOWAVE,
		CVASTParamState::comboBoxTextToValueFunction_LFOWAVE,
		false, true, true, false,
		true);

	effect->createAndAddParameter(&m_uLFOPolarity_LFO5, parameters, 1, "m_uLFOPolarity_LFO5", "LFO 5 Polarity (unipolar, bipolar)", "LFO 5 Polarity(unipolar, bipolar)", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, POLARITY_Array.size() - 1, 1.0f), 1.0f,
		CVASTParamState::comboBoxValueToTextFunction_POLARITY,
		CVASTParamState::comboBoxTextToValueFunction_POLARITY,
		false, true, false, false);

	effect->createAndAddParameter(&m_fLFORamp_LFO5, parameters, 1, "m_fLFORamp_LFO5", "LFO 5 Ramp beats", "LFO 5 ramp beats", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 16.0f, 0.0001f, 0.3f, false), 1.0f, //skew
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);

	effect->createAndAddParameter(&m_fLFOPhase_LFO5, parameters, 1, "m_fLFOPhase_LFO5", "LFO 5 Phase", "Phase of LFO 5", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0, 100), 0,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);

	effect->createAndAddParameter(&m_bLFOPerVoice_LFO5, parameters, 1, "m_bLFOPerVoice_LFO5", "LFO 5 Seperate LFO per voice or global", "LFO 5 seperate LFO per voice or global", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 1.0f, 1.0f), 0.0f,
		CVASTParamState::toggleButtonValueToTextFunction,
		CVASTParamState::toggleButtonTextToValueFunction,
		false, true, true, false,
		true);

	effect->createAndAddParameter(&m_bLFORetrigOnOff_LFO5, parameters, 1, "m_bLFORetrigOnOff_LFO5", "LFO 5 Retrigger oscillator with every new note played (otherwise free running)", "Retrigger LFO 5 oscillator with every new note played (otherwise free running)", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 1.0f, 1.0f), 1.0f,
		CVASTParamState::toggleButtonValueToTextFunction,
		CVASTParamState::toggleButtonTextToValueFunction,
		false, true, true, false);

	effect->createAndAddParameter(&m_bLFOSynch_LFO5, parameters, 1, "m_bLFOSynch_LFO5", "LFO 5 Sync to DAW timecode (starts when played in DAW)", "Sync LFO 5 to DAW timecode (starts when played in DAW)", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 1.0f, 1.0f), 0.0f,
		CVASTParamState::toggleButtonValueToTextFunction,
		CVASTParamState::toggleButtonTextToValueFunction,
		false, true, true, false,
		true);

	effect->createAndAddParameter(&m_uLFOTimeBeats_LFO5, parameters, 1, "m_uLFOTimeBeats_LFO5", "LFO 5 Time in beats when synched to DAW", "Time (DAW)", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, TIMEBEATS_Array.size() - 1, 1.0f), TIMEBEATS::BEATS1,
		CVASTParamState::comboBoxValueToTextFunction_TIMEBEATS,
		CVASTParamState::comboBoxTextToValueFunction_TIMEBEATS,
		false, true, true, false,
		true);

	//Mseg	
	//mseg1
	effect->createAndAddParameter(&m_uMSEGPolarity_MSEG1, parameters, 1, "m_uMSEGPolarity_MSEG1", "MSEG 1 Polarity (unipolar, bipolar)", "MSEG 1 Polarity (unipolar, bipolar)", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, POLARITY_Array.size() - 1, 1.0f), 0.0f,
		CVASTParamState::comboBoxValueToTextFunction_POLARITY,
		CVASTParamState::comboBoxTextToValueFunction_POLARITY,
		false, true, false, false);

	effect->createAndAddParameter(&m_fAttackTime_MSEG1, parameters, 1, "m_fAttackTime_MSEG1", "MSEG 1 Attack time", "MSEG 1 Attack time", 0,
		MODMATDEST::MSEG1Attack,
		NormalisableRange<float>(0, 30000, 0.1f, 0.3f, false), 1.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_fDecayTime_MSEG1, parameters, 1, "m_fDecayTime_MSEG1", "MSEG 1 Decay time", "MSEG 1 Decay time", 0, 
		MODMATDEST::MSEG1Decay,
		NormalisableRange<float>(0, 30000, 0.1f, 0.3f, false), 10.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_fSustainLevel_MSEG1, parameters, 1, "m_fSustainLevel_MSEG1", "MSEG 1 Sustain level", "MSEG 1 Sustain level", 0, 
		MODMATDEST::MSEG1Sustain,
		NormalisableRange<float>(0, 100), 100.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false, 
		true);

	effect->createAndAddParameter(&m_fReleaseTime_MSEG1, parameters, 1, "m_fReleaseTime_MSEG1", "MSEG 1 Release time", "MSEG 1 Release time", 0, 
		MODMATDEST::MSEG1Release,
		NormalisableRange<float>(0, 10000, 0.1f, 0.3f, false), 100.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	//mseg2
	effect->createAndAddParameter(&m_uMSEGPolarity_MSEG2, parameters, 1, "m_uMSEGPolarity_MSEG2", "MSEG 2 Polarity (unipolar, bipolar)", "MSEG 2 Polarity (unipolar, bipolar)", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, POLARITY_Array.size() - 1, 1.0f), 0.0f,
		CVASTParamState::comboBoxValueToTextFunction_POLARITY,
		CVASTParamState::comboBoxTextToValueFunction_POLARITY,
		false, true, false, false);

	effect->createAndAddParameter(&m_fAttackTime_MSEG2, parameters, 1, "m_fAttackTime_MSEG2", "MSEG 2 Attack time", "MSEG 2 Attack time", 0,
		MODMATDEST::MSEG2Attack,
		NormalisableRange<float>(0, 30000, 0.1f, 0.3f, false), 1.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_fDecayTime_MSEG2, parameters, 1, "m_fDecayTime_MSEG2", "MSEG 2 Decay time", "MSEG 2 Decay time", 0,
		MODMATDEST::MSEG2Decay,
		NormalisableRange<float>(0, 30000, 0.1f, 0.3f, false), 10.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_fSustainLevel_MSEG2, parameters, 1, "m_fSustainLevel_MSEG2", "MSEG 2 Sustain level", "MSEG 2 Sustain level", 0,
		MODMATDEST::MSEG2Sustain,
		NormalisableRange<float>(0, 100), 100.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_fReleaseTime_MSEG2, parameters, 1, "m_fReleaseTime_MSEG2", "MSEG 2 Release time", "MSEG 2 Release time", 0,
		MODMATDEST::MSEG2Release,
		NormalisableRange<float>(0, 10000, 0.1f, 0.3f, false), 100.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	//mseg3
	effect->createAndAddParameter(&m_uMSEGPolarity_MSEG3, parameters, 1, "m_uMSEGPolarity_MSEG3", "MSEG 3 Polarity (unipolar, bipolar)", "MSEG 3 Polarity (unipolar, bipolar)", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, POLARITY_Array.size() - 1, 1.0f), 0.0f,
		CVASTParamState::comboBoxValueToTextFunction_POLARITY,
		CVASTParamState::comboBoxTextToValueFunction_POLARITY,
		false, true, false, false);

	effect->createAndAddParameter(&m_fAttackTime_MSEG3, parameters, 1, "m_fAttackTime_MSEG3", "MSEG 3 Attack time", "MSEG 3 Attack time", 0,
		MODMATDEST::MSEG3Attack,
		NormalisableRange<float>(0, 30000, 0.1f, 0.3f, false), 1.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_fDecayTime_MSEG3, parameters, 1, "m_fDecayTime_MSEG3", "MSEG 3 Decay time", "MSEG 3 Decay time", 0,
		MODMATDEST::MSEG3Decay,
		NormalisableRange<float>(0, 30000, 0.1f, 0.3f, false), 10.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_fSustainLevel_MSEG3, parameters, 1, "m_fSustainLevel_MSEG3", "MSEG 3 Sustain level", "MSEG 3 Sustain level", 0,
		MODMATDEST::MSEG3Sustain,
		NormalisableRange<float>(0, 100), 100.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_fReleaseTime_MSEG3, parameters, 1, "m_fReleaseTime_MSEG3", "MSEG 3 Release time", "MSEG 3 Release time", 0,
		MODMATDEST::MSEG3Release,
		NormalisableRange<float>(0, 10000, 0.1f, 0.3f, false), 100.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	//mseg4
	effect->createAndAddParameter(&m_uMSEGPolarity_MSEG4, parameters, 1, "m_uMSEGPolarity_MSEG4", "MSEG 4 Polarity (unipolar, bipolar)", "MSEG 4 Polarity (unipolar, bipolar)", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, POLARITY_Array.size() - 1, 1.0f), 0.0f,
		CVASTParamState::comboBoxValueToTextFunction_POLARITY,
		CVASTParamState::comboBoxTextToValueFunction_POLARITY,
		false, true, false, false);

	effect->createAndAddParameter(&m_fAttackTime_MSEG4, parameters, 1, "m_fAttackTime_MSEG4", "MSEG 4 Aattack time", "MSEG 4 Attack time", 0,
		MODMATDEST::MSEG4Attack,
		NormalisableRange<float>(0, 30000, 0.1f, 0.3f, false), 1.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_fDecayTime_MSEG4, parameters, 1, "m_fDecayTime_MSEG4", "MSEG 4 Decay time", "MSEG 4 Decay time", 0,
		MODMATDEST::MSEG4Decay,
		NormalisableRange<float>(0, 30000, 0.1f, 0.3f, false), 10.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_fSustainLevel_MSEG4, parameters, 1, "m_fSustainLevel_MSEG4", "MSEG 4 Sustain level", "MSEG 4 Sustain level", 0,
		MODMATDEST::MSEG4Sustain,
		NormalisableRange<float>(0, 100), 100.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_fReleaseTime_MSEG4, parameters, 1, "m_fReleaseTime_MSEG4", "MSEG 4 Release time", "MSEG 4 Release time", 0,
		MODMATDEST::MSEG4Release,
		NormalisableRange<float>(0, 10000, 0.1f, 0.3f, false), 100.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	//mseg5
	effect->createAndAddParameter(&m_uMSEGPolarity_MSEG5, parameters, 1, "m_uMSEGPolarity_MSEG5", "MSEG 5 Polarity (unipolar, bipolar)", "MSEG 5 Polarity (unipolar, bipolar)", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, POLARITY_Array.size() - 1, 1.0f), 0.0f,
		CVASTParamState::comboBoxValueToTextFunction_POLARITY,
		CVASTParamState::comboBoxTextToValueFunction_POLARITY,
		false, true, false, false);

	effect->createAndAddParameter(&m_fAttackTime_MSEG5, parameters, 1, "m_fAttackTime_MSEG5", "MSEG 5 Attack time", "MSEG 5 Attack time", 0,
		MODMATDEST::MSEG5Attack,
		NormalisableRange<float>(0, 30000, 0.1f, 0.3f, false), 1.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_fDecayTime_MSEG5, parameters, 1, "m_fDecayTime_MSEG5", "MSEG 5 Decay time", "MSEG 5 Decay time", 0,
		MODMATDEST::MSEG5Decay,
		NormalisableRange<float>(0, 30000, 0.1f, 0.3f, false), 10.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_fSustainLevel_MSEG5, parameters, 1, "m_fSustainLevel_MSEG5", "MSEG 5 Sustain level", "MSEG 5 Sustain level", 0,
		MODMATDEST::MSEG5Sustain,
		NormalisableRange<float>(0, 100), 100.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_fReleaseTime_MSEG5, parameters, 1, "m_fReleaseTime_MSEG5", "MSEG 5 Release time", "MSEG 5 Release time", 0,
		MODMATDEST::MSEG5Release,
		NormalisableRange<float>(0, 10000, 0.1f, 0.3f, false), 100.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	//stepseq
	//stepseq 1
	effect->createAndAddParameter(&m_fStepSeqSpeed_STEPSEQ1, parameters, 1, "m_fStepSeqSpeed_STEPSEQ1", "StepSeq 1 Speed (ms per step)", "StepSeq speed (ms per step)", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(5, 3000, 0.001f, 0.3f, false), 250.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);	

	effect->createAndAddParameter(&m_fGlide_STEPSEQ1, parameters, 1, "m_fGlide_STEPSEQ1", "StepSeq 1 Glide mode time step sequencer 1", "Glide mode time step sequencer 1", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0, 100), 25.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_fGate_STEPSEQ1, parameters, 1, "m_fGate_STEPSEQ1", "StepSeq 1 Gate amount step sequencer 1", "Gate amount step sequencer 1", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0, 100), 65.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_bStepSeqSynch_STEPSEQ1, parameters, 1, "m_bStepSeqSynch_STEPSEQ1", "StepSeq 1 Sync to DAW timecode (starts when played in DAW)", "Sync StepSeq 1 to DAW timecode (starts when played in DAW)", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 1.0f, 1.0f), 0.0f,
		CVASTParamState::toggleButtonValueToTextFunction,
		CVASTParamState::toggleButtonTextToValueFunction,
		false, true, true, false,
		true);

	effect->createAndAddParameter(&m_uStepSeqTimeBeats_STEPSEQ1, parameters, 1, "m_uStepSeqTimeBeats_STEPSEQ1", "StepSeq 1 Time in beats when synced to DAW", "Time (DAW)", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, TIMEBEATS_Array.size() - 1, 1.0f), TIMEBEATS::BEATS1,
		CVASTParamState::comboBoxValueToTextFunction_TIMEBEATS,
		CVASTParamState::comboBoxTextToValueFunction_TIMEBEATS,
		false, true, true, false,
		true);

	//stepseq 2
	effect->createAndAddParameter(&m_fStepSeqSpeed_STEPSEQ2, parameters, 1, "m_fStepSeqSpeed_STEPSEQ2", "StepSeq 2 Speed (ms per step)", "StepSeq speed (ms per step)", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(5, 3000, 0.001f, 0.3f, false), 250.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_fGlide_STEPSEQ2, parameters, 1, "m_fGlide_STEPSEQ2", "StepSeq 2 Glide mode time step sequencer 2", "Glide mode time step sequencer 2", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0, 100), 25.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_fGate_STEPSEQ2, parameters, 1, "m_fGate_STEPSEQ2", "StepSeq 2 Gate amount step sequencer 2", "Gate amount step sequencer 2", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0, 100), 65.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_bStepSeqSynch_STEPSEQ2, parameters, 1, "m_bStepSeqSynch_STEPSEQ2", "StepSeq 2 Sync to DAW timecode (starts when played in DAW)", "Sync StepSeq 1 to DAW timecode (starts when played in DAW)", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 1.0f, 1.0f), 0.0f,
		CVASTParamState::toggleButtonValueToTextFunction,
		CVASTParamState::toggleButtonTextToValueFunction,
		false, true, true, false,
		true);

	effect->createAndAddParameter(&m_uStepSeqTimeBeats_STEPSEQ2, parameters, 1, "m_uStepSeqTimeBeats_STEPSEQ2", "StepSeq 2 Time in beats when synced to DAW", "Time (DAW)", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, TIMEBEATS_Array.size() - 1, 1.0f), TIMEBEATS::BEATS1,
		CVASTParamState::comboBoxValueToTextFunction_TIMEBEATS,
		CVASTParamState::comboBoxTextToValueFunction_TIMEBEATS,
		false, true, true, false,
		true);

	//stepseq 3
	effect->createAndAddParameter(&m_fStepSeqSpeed_STEPSEQ3, parameters, 1, "m_fStepSeqSpeed_STEPSEQ3", "StepSeq 3 Speed (ms per step)", "StepSeq speed (ms per step))", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(5, 3000, 0.001f, 0.3f, false), 250.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_fGlide_STEPSEQ3, parameters, 1, "m_fGlide_STEPSEQ3", "StepSeq 3 Glide mode time step sequencer 3", "Glide mode time step sequencer 3", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0, 100), 25.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true); 

	effect->createAndAddParameter(&m_fGate_STEPSEQ3, parameters, 1, "m_fGate_STEPSEQ3", "StepSeq 3 Gate amount step sequencer 3", "Gate amount step sequencer 3", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0, 100), 65.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_bStepSeqSynch_STEPSEQ3, parameters, 1, "m_bStepSeqSynch_STEPSEQ3", "StepSeq 3 Sync to DAW timecode (starts when played in DAW)", "Sync StepSeq 1 to DAW timecode (starts when played in DAW)", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 1.0f, 1.0f), 0.0f,
		CVASTParamState::toggleButtonValueToTextFunction,
		CVASTParamState::toggleButtonTextToValueFunction,
		false, true, true, false,
		true);

	effect->createAndAddParameter(&m_uStepSeqTimeBeats_STEPSEQ3, parameters, 1, "m_uStepSeqTimeBeats_STEPSEQ3", "StepSeq 3 Time in beats when synced to DAW", "Time (DAW)", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, TIMEBEATS_Array.size() - 1, 1.0f), TIMEBEATS::BEATS1,
		CVASTParamState::comboBoxValueToTextFunction_TIMEBEATS,
		CVASTParamState::comboBoxTextToValueFunction_TIMEBEATS,
		false, true, true, false,
		true);
	
	//arp
	effect->createAndAddParameter(&m_fARPSpeed, parameters, 1, "m_fARPSpeed", "Arpeggiator Speed (ms per step)", "Arpeggiator speed(ms per step)", 0,
		MODMATDEST::ArpSpeed,
		NormalisableRange<float>(5, 3000, 0.001f, 0.3f, false), 250.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_uARPMode, parameters, 1, "m_uARPMode", "Arpeggiator Mode", "Arpeggiator mode", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, ARPMODE_Array.size() - 1, 1.0f), 0.0f,
		CVASTParamState::comboBoxValueToTextFunction_ARPMODE,
		CVASTParamState::comboBoxTextToValueFunction_ARPMODE,
		false, true, true, false);

	effect->createAndAddParameter(&m_uARPTimeBeats, parameters, 1, "m_uARPTimeBeats", "Arpeggiator Time in beats when synced to DAW", "Time (DAW)", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, TIMEBEATS_Array.size() - 1, 1.0f), TIMEBEATS::BEATS1,
		CVASTParamState::comboBoxValueToTextFunction_TIMEBEATS,
		CVASTParamState::comboBoxTextToValueFunction_TIMEBEATS,
		false, true, true, false);

	effect->createAndAddParameter(&m_bARPOnOff, parameters, 1, "m_bARPOnOff", "Arpeggiator On / off", "Switch arpeggiator on / off", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 1.0f, 1.0f), 0.0f,
		CVASTParamState::toggleButtonValueToTextFunction,
		CVASTParamState::toggleButtonTextToValueFunction,
		false, true, true, false,
		true);

	effect->createAndAddParameter(&m_bARPSynch, parameters, 1, "m_bARPSynch", "Arpeggiator Sync to DAW timecode (starts when played in DAW)", "Sync arpeggiator to DAW timecode (starts when played in DAW)", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 1.0f, 1.0f), 0.0f,
		CVASTParamState::toggleButtonValueToTextFunction,
		CVASTParamState::toggleButtonTextToValueFunction,
		false, true, true, false);

	//=================================================================================
	//modmatrix

	effect->createAndAddParameter(&m_uModMatSrce1, parameters, 1, "m_uModMatSrce1", "ModMatrix Source 1", "ModMatrix Source 1", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, MODMATSRCE_Array.size() - 1, 1.0f), 0.0f,
		CVASTParamState::comboBoxValueToTextFunction_MODMATSRCE,
		CVASTParamState::comboBoxTextToValueFunction_MODMATSRCE,
		false, true, true, false,
		true);

	effect->createAndAddParameter(&m_uModMatSrce2, parameters, 1, "m_uModMatSrce2", "ModMatrix Source 2", "ModMatrix Source 2", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, MODMATSRCE_Array.size() - 1, 1.0f), 0.0f,
		CVASTParamState::comboBoxValueToTextFunction_MODMATSRCE,
		CVASTParamState::comboBoxTextToValueFunction_MODMATSRCE,
		false, true, true, false, 
		true);

	effect->createAndAddParameter(&m_uModMatSrce3, parameters, 1, "m_uModMatSrce3", "ModMatrix Source 3", "ModMatrix Source 3", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, MODMATSRCE_Array.size() - 1, 1.0f), 0.0f,
		CVASTParamState::comboBoxValueToTextFunction_MODMATSRCE,
		CVASTParamState::comboBoxTextToValueFunction_MODMATSRCE,
		false, true, true, false,
		true);

	effect->createAndAddParameter(&m_uModMatSrce4, parameters, 1, "m_uModMatSrce4", "ModMatrix Source 4", "ModMatrix Source 4", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, MODMATSRCE_Array.size() - 1, 1.0f), 0.0f,
		CVASTParamState::comboBoxValueToTextFunction_MODMATSRCE,
		CVASTParamState::comboBoxTextToValueFunction_MODMATSRCE,
		false, true, true, false,
		true);

	effect->createAndAddParameter(&m_uModMatSrce5, parameters, 1, "m_uModMatSrce5", "ModMatrix Source 5", "ModMatrix Source 5", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, MODMATSRCE_Array.size() - 1, 1.0f), 0.0f,
		CVASTParamState::comboBoxValueToTextFunction_MODMATSRCE,
		CVASTParamState::comboBoxTextToValueFunction_MODMATSRCE,
		false, true, true, false,
		true);

	effect->createAndAddParameter(&m_uModMatSrce6, parameters, 1, "m_uModMatSrce6", "ModMatrix Source 6", "ModMatrix Source 6", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, MODMATSRCE_Array.size() - 1, 1.0f), 0.0f,
		CVASTParamState::comboBoxValueToTextFunction_MODMATSRCE,
		CVASTParamState::comboBoxTextToValueFunction_MODMATSRCE,
		false, true, true, false,
		true);


	effect->createAndAddParameter(&m_uModMatSrce7, parameters, 1, "m_uModMatSrce7", "ModMatrix Source 7", "ModMatrix Source 7", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, MODMATSRCE_Array.size() - 1, 1.0f), 0.0f,
		CVASTParamState::comboBoxValueToTextFunction_MODMATSRCE,
		CVASTParamState::comboBoxTextToValueFunction_MODMATSRCE,
		false, true, true, false,
		true);

	effect->createAndAddParameter(&m_uModMatSrce8, parameters, 1, "m_uModMatSrce8", "ModMatrix Source 8", "ModMatrix Source 8", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, MODMATSRCE_Array.size() - 1, 1.0f), 0.0f,
		CVASTParamState::comboBoxValueToTextFunction_MODMATSRCE,
		CVASTParamState::comboBoxTextToValueFunction_MODMATSRCE,
		false, true, true, false,
		true);

	effect->createAndAddParameter(&m_uModMatSrce9, parameters, 1, "m_uModMatSrce9", "ModMatrix Source 9", "ModMatrix Source 9", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, MODMATSRCE_Array.size() - 1, 1.0f), 0.0f,
		CVASTParamState::comboBoxValueToTextFunction_MODMATSRCE,
		CVASTParamState::comboBoxTextToValueFunction_MODMATSRCE,
		false, true, true, false,
		true);

	effect->createAndAddParameter(&m_uModMatSrce10, parameters, 1, "m_uModMatSrce10", "ModMatrix Source 10", "ModMatrix Source 10", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, MODMATSRCE_Array.size() - 1, 1.0f), 0.0f,
		CVASTParamState::comboBoxValueToTextFunction_MODMATSRCE,
		CVASTParamState::comboBoxTextToValueFunction_MODMATSRCE,
		false, true, true, false,
		true);

	effect->createAndAddParameter(&m_uModMatSrce11, parameters, 1, "m_uModMatSrce11", "ModMatrix Source 11", "ModMatrix Source 11", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, MODMATSRCE_Array.size() - 1, 1.0f), 0.0f,
		CVASTParamState::comboBoxValueToTextFunction_MODMATSRCE,
		CVASTParamState::comboBoxTextToValueFunction_MODMATSRCE,
		false, true, true, false,
		true);

	effect->createAndAddParameter(&m_uModMatSrce12, parameters, 1, "m_uModMatSrce12", "ModMatrix Source 12", "ModMatrix Source 12", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, MODMATSRCE_Array.size() - 1, 1.0f), 0.0f,
		CVASTParamState::comboBoxValueToTextFunction_MODMATSRCE,
		CVASTParamState::comboBoxTextToValueFunction_MODMATSRCE,
		false, true, true, false,
		true);

	effect->createAndAddParameter(&m_uModMatSrce13, parameters, 1, "m_uModMatSrce13", "ModMatrix Source 13", "ModMatrix Source 13", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, MODMATSRCE_Array.size() - 1, 1.0f), 0.0f,
		CVASTParamState::comboBoxValueToTextFunction_MODMATSRCE,
		CVASTParamState::comboBoxTextToValueFunction_MODMATSRCE,
		false, true, true, false,
		true);

	effect->createAndAddParameter(&m_uModMatSrce14, parameters, 1, "m_uModMatSrce14", "ModMatrix Source 14", "ModMatrix Source 14", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, MODMATSRCE_Array.size() - 1, 1.0f), 0.0f,
		CVASTParamState::comboBoxValueToTextFunction_MODMATSRCE,
		CVASTParamState::comboBoxTextToValueFunction_MODMATSRCE,
		false, true, true, false,
		true);

	effect->createAndAddParameter(&m_uModMatSrce15, parameters, 1, "m_uModMatSrce15", "ModMatrix Source 15", "ModMatrix Source 15", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, MODMATSRCE_Array.size() - 1, 1.0f), 0.0f,
		CVASTParamState::comboBoxValueToTextFunction_MODMATSRCE,
		CVASTParamState::comboBoxTextToValueFunction_MODMATSRCE,
		false, true, true, false,
		true);

	effect->createAndAddParameter(&m_uModMatSrce16, parameters, 1, "m_uModMatSrce16", "ModMatrix Source 16", "ModMatrix Source 16", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, MODMATSRCE_Array.size() - 1, 1.0f), 0.0f,
		CVASTParamState::comboBoxValueToTextFunction_MODMATSRCE,
		CVASTParamState::comboBoxTextToValueFunction_MODMATSRCE,
		false, true, true, false,
		true);
	effect->createAndAddParameter(&m_uModMatDest1, parameters, 1, "m_uModMatDest1", "ModMatrix Destination 1", "ModMatrix Destination 1", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, MODMATDEST_Array.size() - 1, 1.0f), 0.0f,
		CVASTParamState::comboBoxValueToTextFunction_MODMATDEST,
		CVASTParamState::comboBoxTextToValueFunction_MODMATDEST,
		false, true, true, false,
		true);

	effect->createAndAddParameter(&m_uModMatDest2, parameters, 1, "m_uModMatDest2", "ModMatrix Destination 2", "ModMatrix Destination 2", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, MODMATDEST_Array.size() - 1, 1.0f), 0.0f,
		CVASTParamState::comboBoxValueToTextFunction_MODMATDEST,
		CVASTParamState::comboBoxTextToValueFunction_MODMATDEST,
		false, true, true, false,
		true);

	effect->createAndAddParameter(&m_uModMatDest3, parameters, 1, "m_uModMatDest3", "ModMatrix Destination 3", "ModMatrix Destination 3", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, MODMATDEST_Array.size() - 1, 1.0f), 0.0f,
		CVASTParamState::comboBoxValueToTextFunction_MODMATDEST,
		CVASTParamState::comboBoxTextToValueFunction_MODMATDEST,
		false, true, true, false,
		true);

	effect->createAndAddParameter(&m_uModMatDest4, parameters, 1, "m_uModMatDest4", "ModMatrix Destination 4", "ModMatrix Destination 4", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, MODMATDEST_Array.size() - 1, 1.0f), 0.0f,
		CVASTParamState::comboBoxValueToTextFunction_MODMATDEST,
		CVASTParamState::comboBoxTextToValueFunction_MODMATDEST,
		false, true, true, false,
		true);

	effect->createAndAddParameter(&m_uModMatDest5, parameters, 1, "m_uModMatDest5", "ModMatrix Destination 5", "ModMatrix Destination 5", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, MODMATDEST_Array.size() - 1, 1.0f), 0.0f,
		CVASTParamState::comboBoxValueToTextFunction_MODMATDEST,
		CVASTParamState::comboBoxTextToValueFunction_MODMATDEST,
		false, true, true, false,
		true);

	effect->createAndAddParameter(&m_uModMatDest6, parameters, 1, "m_uModMatDest6", "ModMatrix Destination 6", "ModMatrix Destination 6", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, MODMATDEST_Array.size() - 1, 1.0f), 0.0f,
		CVASTParamState::comboBoxValueToTextFunction_MODMATDEST,
		CVASTParamState::comboBoxTextToValueFunction_MODMATDEST,
		false, true, true, false,
		true);

	effect->createAndAddParameter(&m_uModMatDest7, parameters, 1, "m_uModMatDest7", "ModMatrix Destination 7", "ModMatrix Destination 7", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, MODMATDEST_Array.size() - 1, 1.0f), 0.0f,
		CVASTParamState::comboBoxValueToTextFunction_MODMATDEST,
		CVASTParamState::comboBoxTextToValueFunction_MODMATDEST,
		false, true, true, false,
		true);

	effect->createAndAddParameter(&m_uModMatDest8, parameters, 1, "m_uModMatDest8", "ModMatrix Destination 8", "ModMatrix Destination 8", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, MODMATDEST_Array.size() - 1, 1.0f), 0.0f,
		CVASTParamState::comboBoxValueToTextFunction_MODMATDEST,
		CVASTParamState::comboBoxTextToValueFunction_MODMATDEST,
		false, true, true, false,
		true);

	effect->createAndAddParameter(&m_uModMatDest9, parameters, 1, "m_uModMatDest9", "ModMatrix Destination 9", "ModMatrix Destination 9", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, MODMATDEST_Array.size() - 1, 1.0f), 0.0f,
		CVASTParamState::comboBoxValueToTextFunction_MODMATDEST,
		CVASTParamState::comboBoxTextToValueFunction_MODMATDEST,
		false, true, true, false,
		true);

	effect->createAndAddParameter(&m_uModMatDest10, parameters, 1, "m_uModMatDest10", "ModMatrix Destination 10", "ModMatrix Destination 10", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, MODMATDEST_Array.size() - 1, 1.0f), 0.0f,
		CVASTParamState::comboBoxValueToTextFunction_MODMATDEST,
		CVASTParamState::comboBoxTextToValueFunction_MODMATDEST,
		false, true, true, false,
		true);

	effect->createAndAddParameter(&m_uModMatDest11, parameters, 1, "m_uModMatDest11", "ModMatrix Destination 11", "ModMatrix Destination 11", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, MODMATDEST_Array.size() - 1, 1.0f), 0.0f,
		CVASTParamState::comboBoxValueToTextFunction_MODMATDEST,
		CVASTParamState::comboBoxTextToValueFunction_MODMATDEST,
		false, true, true, false,
		true);

	effect->createAndAddParameter(&m_uModMatDest12, parameters, 1, "m_uModMatDest12", "ModMatrix Destination 12", "ModMatrix Destination 12", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, MODMATDEST_Array.size() - 1, 1.0f), 0.0f,
		CVASTParamState::comboBoxValueToTextFunction_MODMATDEST,
		CVASTParamState::comboBoxTextToValueFunction_MODMATDEST,
		false, true, true, false,
		true);

	effect->createAndAddParameter(&m_uModMatDest13, parameters, 1, "m_uModMatDest13", "ModMatrix Destination 13", "ModMatrix Destination 13", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, MODMATDEST_Array.size() - 1, 1.0f), 0.0f,
		CVASTParamState::comboBoxValueToTextFunction_MODMATDEST,
		CVASTParamState::comboBoxTextToValueFunction_MODMATDEST,
		false, true, true, false,
		true);

	effect->createAndAddParameter(&m_uModMatDest14, parameters, 1, "m_uModMatDest14", "ModMatrix Destination 14", "ModMatrix Destination 14", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, MODMATDEST_Array.size() - 1, 1.0f), 0.0f,
		CVASTParamState::comboBoxValueToTextFunction_MODMATDEST,
		CVASTParamState::comboBoxTextToValueFunction_MODMATDEST,
		false, true, true, false,
		true);

	effect->createAndAddParameter(&m_uModMatDest15, parameters, 1, "m_uModMatDest15", "ModMatrix Destination 15", "ModMatrix Destination 15", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, MODMATDEST_Array.size() - 1, 1.0f), 0.0f,
		CVASTParamState::comboBoxValueToTextFunction_MODMATDEST,
		CVASTParamState::comboBoxTextToValueFunction_MODMATDEST,
		false, true, true, false,
		true);

	effect->createAndAddParameter(&m_uModMatDest16, parameters, 1, "m_uModMatDest16", "ModMatrix Destination 16", "ModMatrix Destination 16", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, MODMATDEST_Array.size() - 1, 1.0f), 0.0f,
		CVASTParamState::comboBoxValueToTextFunction_MODMATDEST,
		CVASTParamState::comboBoxTextToValueFunction_MODMATDEST,
		false, true, true, false,
		true);

	effect->createAndAddParameter(&m_fModMatVal1, parameters, 1, "m_fModMatVal1", "ModMatrix Value 1 (negative effect, no effect, positive effect) ", "ModMatrix Value 1 (negative effect, no effect, positive effect) ", 0,
		MODMATDEST::ModMatrixValue1,
		NormalisableRange<float>(-100, 100), 0,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_fModMatVal2, parameters, 1, "m_fModMatVal2", "ModMatrix Value 2 (negative effect, no effect, positive effect) ", "ModMatrix Value 2 (negative effect, no effect, positive effect) ", 0,
		MODMATDEST::ModMatrixValue2,
		NormalisableRange<float>(-100, 100), 0,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_fModMatVal3, parameters, 1, "m_fModMatVal3", "ModMatrix Value 3 (negative effect, no effect, positive effect) ", "ModMatrix Value 3 (negative effect, no effect, positive effect) ", 0,
		MODMATDEST::ModMatrixValue3,
		NormalisableRange<float>(-100, 100), 0,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_fModMatVal4, parameters, 1, "m_fModMatVal4", "ModMatrix Value 4 (negative effect, no effect, positive effect) ", "ModMatrix Value 4 (negative effect, no effect, positive effect) ", 0,
		MODMATDEST::ModMatrixValue4,
		NormalisableRange<float>(-100, 100), 0,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_fModMatVal5, parameters, 1, "m_fModMatVal5", "ModMatrix Value 5 (negative effect, no effect, positive effect) ", "ModMatrix Value 5 (negative effect, no effect, positive effect) ", 0,
		MODMATDEST::ModMatrixValue5,
		NormalisableRange<float>(-100, 100), 0,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_fModMatVal6, parameters, 1, "m_fModMatVal6", "ModMatrix Value 6 (negative effect, no effect, positive effect) ", "ModMatrix Value 6 (negative effect, no effect, positive effect) ", 0,
		MODMATDEST::ModMatrixValue6,
		NormalisableRange<float>(-100, 100), 0,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_fModMatVal7, parameters, 1, "m_fModMatVal7", "ModMatrix Value 7 (negative effect, no effect, positive effect) ", "ModMatrix Value 7 (negative effect, no effect, positive effect) ", 0,
		MODMATDEST::ModMatrixValue7,
		NormalisableRange<float>(-100, 100), 0,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_fModMatVal8, parameters, 1, "m_fModMatVal8", "ModMatrix Value 8 (negative effect, no effect, positive effect) ", "ModMatrix Value 8 (negative effect, no effect, positive effect) ", 0,
		MODMATDEST::ModMatrixValue8,
		NormalisableRange<float>(-100, 100), 0,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_fModMatVal9, parameters, 1, "m_fModMatVal9", "ModMatrix Value 9 (negative effect, no effect, positive effect) ", "ModMatrix Value 9 (negative effect, no effect, positive effect) ", 0,
		MODMATDEST::ModMatrixValue9,
		NormalisableRange<float>(-100, 100), 0,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_fModMatVal10, parameters, 1, "m_fModMatVal10", "ModMatrix Value 10 (negative effect, no effect, positive effect) ", "ModMatrix Value 10 (negative effect, no effect, positive effect) ", 0,
		MODMATDEST::ModMatrixValue10,
		NormalisableRange<float>(-100, 100), 0,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_fModMatVal11, parameters, 1, "m_fModMatVal11", "ModMatrix Value 11 (negative effect, no effect, positive effect) ", "ModMatrix Value 11 (negative effect, no effect, positive effect) ", 0,
		MODMATDEST::ModMatrixValue11,
		NormalisableRange<float>(-100, 100), 0,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_fModMatVal12, parameters, 1, "m_fModMatVal12", "ModMatrix Value 12 (negative effect, no effect, positive effect) ", "ModMatrix Value 12 (negative effect, no effect, positive effect) ", 0,
		MODMATDEST::ModMatrixValue12,
		NormalisableRange<float>(-100, 100), 0,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_fModMatVal13, parameters, 1, "m_fModMatVal13", "ModMatrix Value 13 (negative effect, no effect, positive effect) ", "ModMatrix Value 13 (negative effect, no effect, positive effect) ", 0,
		MODMATDEST::ModMatrixValue13,
		NormalisableRange<float>(-100, 100), 0,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_fModMatVal14, parameters, 1, "m_fModMatVal14", "ModMatrix Value 14 (negative effect, no effect, positive effect) ", "ModMatrix Value 14 (negative effect, no effect, positive effect) ", 0,
		MODMATDEST::ModMatrixValue14,
		NormalisableRange<float>(-100, 100), 0,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_fModMatVal15, parameters, 1, "m_fModMatVal15", "ModMatrix Value 15 (negative effect, no effect, positive effect) ", "ModMatrix Value 15 (negative effect, no effect, positive effect) ", 0,
		MODMATDEST::ModMatrixValue15,
		NormalisableRange<float>(-100, 100), 0,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_fModMatVal16, parameters, 1, "m_fModMatVal16", "ModMatrix Value 16 (negative effect, no effect, positive effect) ", "ModMatrix Value 16 (negative effect, no effect, positive effect) ", 0,
		MODMATDEST::ModMatrixValue16,
		NormalisableRange<float>(-100, 100), 0,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_fModMatCurve1, parameters, 1, "m_fModMatCurve1", "ModMatrix Curve 1", "ModMatrix Curve 1", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(-100, 100), 0,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_fModMatCurve2, parameters, 1, "m_fModMatCurve2", "ModMatrix Curve 2", "ModMatrix Curve 2", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(-100, 100), 0,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_fModMatCurve3, parameters, 1, "m_fModMatCurve3", "ModMatrix Curve 3", "ModMatrix Curve 3", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(-100, 100), 0,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_fModMatCurve4, parameters, 1, "m_fModMatCurve4", "ModMatrix Curve 4", "ModMatrix Curve 4", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(-100, 100), 0,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_fModMatCurve5, parameters, 1, "m_fModMatCurve5", "ModMatrix Curve 5", "ModMatrix Curve 5", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(-100, 100), 0,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_fModMatCurve6, parameters, 1, "m_fModMatCurve6", "ModMatrix Curve 6", "ModMatrix Curve 6", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(-100, 100), 0,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_fModMatCurve7, parameters, 1, "m_fModMatCurve7", "ModMatrix Curve 7", "ModMatrix Curve 7", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(-100, 100), 0,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_fModMatCurve8, parameters, 1, "m_fModMatCurve8", "ModMatrix Curve 8", "ModMatrix Curve 8", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(-100, 100), 0,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_fModMatCurve9, parameters, 1, "m_fModMatCurve9", "ModMatrix Curve 9", "ModMatrix Curve 9", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(-100, 100), 0,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_fModMatCurve10, parameters, 1, "m_fModMatCurve10", "ModMatrix Curve 10", "ModMatrix Curve 10", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(-100, 100), 0,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_fModMatCurve11, parameters, 1, "m_fModMatCurve11", "ModMatrix Curve 11", "ModMatrix Curve 11", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(-100, 100), 0,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_fModMatCurve12, parameters, 1, "m_fModMatCurve12", "ModMatrix Curve 12", "ModMatrix Curve 12", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(-100, 100), 0,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_fModMatCurve13, parameters, 1, "m_fModMatCurve13", "ModMatrix Curve 13", "ModMatrix Curve 13", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(-100, 100), 0,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_fModMatCurve14, parameters, 1, "m_fModMatCurve14", "ModMatrix Curve 14", "ModMatrix Curve 14", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(-100, 100), 0,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_fModMatCurve15, parameters, 1, "m_fModMatCurve15", "ModMatrix Curve 15", "ModMatrix Curve 15", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(-100, 100), 0,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_fModMatCurve16, parameters, 1, "m_fModMatCurve16", "ModMatrix Curve 16", "ModMatrix Curve 16", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(-100, 100), 0,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	//not automatable
	effect->createAndAddParameter(&m_iWavWTPos, parameters, 1, "m_iWavWTPos", "WT Editor Number of positions to create from wav file", "WT Editor number of positions to create from wav file", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(1, 256, 1), 32,
		CVASTParamState::intSliderValueToTextFunction,
		CVASTParamState::intSliderTextToValueFunction,
		true, false, true, false);

	effect->createAndAddParameter(&m_iWTEditorZoom, parameters, 1, "m_iWTEditorZoom", "WT Editor Zoom level of frequency domain display", "Zoom level of frequency domain display", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(1, 16, 1), 8,
		CVASTParamState::intSliderValueToTextFunction,
		CVASTParamState::intSliderTextToValueFunction,
		true, false, true, false);

	effect->createAndAddParameter(&m_iWAVScale, parameters, 1, "m_iWAVScale", "WT Editor Display scaling of wav file", "WT Editor display scaling of wav file", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(1, 256, 1), 1,
		CVASTParamState::intSliderValueToTextFunction,
		CVASTParamState::intSliderTextToValueFunction,
		true, false, true, false);

	//====================================================================================
	//fx

	effect->createAndAddParameter(&m_fMBMonoFrequency, parameters, 1, "m_fMBMonoFrequency", "FXBUS 1 Multi band mono frequency - below only mono processing and no delay & reverb", "Multi band mono frequency bus 1- below only mono processing and no delay & reverb", 0,
		MODMATDEST::Fxbus1MonoFrequency,
		NormalisableRange<float>(0, 20000, 0.001f, 0.3f, false), 0,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);

	effect->createAndAddParameter(&m_fMBMonoFrequency_Bus2, parameters, 1, "m_fMBMonoFrequency_Bus2", "FXBUS 2 Multi band mono frequency - below only mono processing and no delay & reverb", "Multi band mono frequency bus 2- below only mono processing and no delay & reverb", 0,
		MODMATDEST::Fxbus2MonoFrequency,
		NormalisableRange<float>(0, 20000, 0.001f, 0.3f, false), 0,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);

	effect->createAndAddParameter(&m_fMBMonoFrequency_Bus3, parameters, 1, "m_fMBMonoFrequency_Bus3", "FXBUS 3 Multi band mono frequency - below only mono processing and no delay & reverb", "Multi band mono frequency bus 3- below only mono processing and no delay & reverb", 0,
		MODMATDEST::Fxbus3MonoFrequency,
		NormalisableRange<float>(0, 20000, 0.001f, 0.3f, false), 0,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);

	/*
	effect->createAndAddParameter(&m_bOversampleBus, parameters, 1, "m_bOversampleBus", "FXBUS 1 Oversampling (4x)- impacts performance / CPU", "4 x oversample the effect bus 1", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 1.0f, 1.0f), 0.0f,
		CVASTParamState::toggleButtonValueToTextFunction,
		CVASTParamState::toggleButtonTextToValueFunction,
		false, true, true, false,
		true);

	effect->createAndAddParameter(&m_bOversampleBus_Bus2, parameters, 1, "m_bOversampleBus_Bus2", "FXBUS 2 Oversampling g (4x) - impacts performance / CPU", "4 x oversample the effect bus 2", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 1.0f, 1.0f), 0.0f,
		CVASTParamState::toggleButtonValueToTextFunction,
		CVASTParamState::toggleButtonTextToValueFunction,
		false, true, true, false,
		true);

	effect->createAndAddParameter(&m_bOversampleBus_Bus3, parameters, 1, "m_bOversampleBus_Bus3", "FXBUS 3 Oversampling (4x) - impacts performance / CPU", "4 x oversample the effect bus 3", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 1.0f, 1.0f), 0.0f,
		CVASTParamState::toggleButtonValueToTextFunction,
		CVASTParamState::toggleButtonTextToValueFunction,
		false, true, true, false,
		true);
		*/

	effect->createAndAddParameter(&m_uFxBusRouting, parameters, 1, "m_uFxBusRouting", "FXBUS 1 Routing", "Effect bus 1 routing", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, FXBUSROUTE_Array.size() - 1, 1.0f), FXBUSROUTE_Array.indexOf("Master"),
		CVASTParamState::comboBoxValueToTextFunction_FXBUSROUTE,
		CVASTParamState::comboBoxTextToValueFunction_FXBUSROUTE,
		false, true, true, false);

	effect->createAndAddParameter(&m_uFxBusRouting_Bus2, parameters, 1, "m_uFxBusRouting_Bus2", "FXBUS 2 Routing", "Effect bus 2 routing", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, FXBUSROUTE_Array.size() - 1, 1.0f), FXBUSROUTE_Array.indexOf("Master"),
		CVASTParamState::comboBoxValueToTextFunction_FXBUSROUTE,
		CVASTParamState::comboBoxTextToValueFunction_FXBUSROUTE,
		false, true, true, false);

	effect->createAndAddParameter(&m_uFxBusRouting_Bus3, parameters, 1, "m_uFxBusRouting_Bus3", "FXBUS 3 Routing", "Effect bus 3 routing", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, FXBUSROUTE_Array.size() - 1, 1.0f), FXBUSROUTE_Array.indexOf("Master"),
		CVASTParamState::comboBoxValueToTextFunction_FXBUSROUTE,
		CVASTParamState::comboBoxTextToValueFunction_FXBUSROUTE,
		false, true, true, false);

	return true;
}

bool CVASTParamState::initCompatibilityParameters(AudioProcessorValueTreeState& parameters, CVASTEffect* effect) {
	//added at the end of list for parameters after release to keep DAW sorting order

	/*
	effect->createAndAddParameter(&m_fMasterVolumedB123, parameters, 1, "m_fMasterVolumedB123", "Master volume in dB. Parameter is saved as well with preset.", "Master volume in dB. Parameter is saved as well with preset.", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(-60, 20), 0,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);
		*/


	effect->createAndAddParameter(&m_uLFOMSEG_LFO1, parameters, 1, "m_uLFOMSEG_LFO1", "MSEG Envelope for LFO 1", "MSEG Envelope for LFO 1", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, MSEGLFOENV_Array.size() - 1, 1.0f), 0.0f,
		CVASTParamState::comboBoxValueToTextFunction_MSEGLFOENV,
		CVASTParamState::comboBoxTextToValueFunction_MSEGLFOENV,
		false, true, true, false);

	effect->createAndAddParameter(&m_uLFOMSEG_LFO2, parameters, 1, "m_uLFOMSEG_LFO2", "MSEG Envelope for LFO 2", "MSEG Envelope for LFO 2", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, MSEGLFOENV_Array.size() - 1, 1.0f), 0.0f,
		CVASTParamState::comboBoxValueToTextFunction_MSEGLFOENV,
		CVASTParamState::comboBoxTextToValueFunction_MSEGLFOENV,
		false, true, true, false);

	effect->createAndAddParameter(&m_uLFOMSEG_LFO3, parameters, 1, "m_uLFOMSEG_LFO3", "MSEG Envelope for LFO 3", "MSEG Envelope for LFO 3", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, MSEGLFOENV_Array.size() - 1, 1.0f), 0.0f,
		CVASTParamState::comboBoxValueToTextFunction_MSEGLFOENV,
		CVASTParamState::comboBoxTextToValueFunction_MSEGLFOENV,
		false, true, true, false);

	effect->createAndAddParameter(&m_uLFOMSEG_LFO4, parameters, 1, "m_uLFOMSEG_LFO4", "MSEG Envelope for LFO 4", "MSEG Envelope for LFO 4", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, MSEGLFOENV_Array.size() - 1, 1.0f), 0.0f,
		CVASTParamState::comboBoxValueToTextFunction_MSEGLFOENV,
		CVASTParamState::comboBoxTextToValueFunction_MSEGLFOENV,
		false, true, true, false);

	effect->createAndAddParameter(&m_uLFOMSEG_LFO5, parameters, 1, "m_uLFOMSEG_LFO5", "MSEG Envelope for LFO 5", "MSEG Envelope for LFO 5", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, MSEGLFOENV_Array.size() - 1, 1.0f), 0.0f,
		CVASTParamState::comboBoxValueToTextFunction_MSEGLFOENV,
		CVASTParamState::comboBoxTextToValueFunction_MSEGLFOENV,
		false, true, true, false);


	return true;
}

bool CVASTParamState::initCompatibilityParameters2(AudioProcessorValueTreeState& parameters, CVASTEffect* effect) {
	//added at the end of list for parameters after release to keep DAW sorting order

	effect->createAndAddParameter(&m_uModMatPolarity1, parameters, 1, "m_uModMatPolarity1", "ModMatrix 1 Polarity (unipolar, bipolar)", "ModMatrix 1 Polarity (unipolar, bipolar)", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, POLARITY_Array.size() - 1, 1.0f), 1.0f,
		CVASTParamState::comboBoxValueToTextFunction_POLARITY,
		CVASTParamState::comboBoxTextToValueFunction_POLARITY,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_uModMatPolarity2, parameters, 1, "m_uModMatPolarity2", "ModMatrix 2 Polarity (unipolar, bipolar)", "ModMatrix 2 Polarity (unipolar, bipolar)", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, POLARITY_Array.size() - 1, 1.0f), 1.0f,
		CVASTParamState::comboBoxValueToTextFunction_POLARITY,
		CVASTParamState::comboBoxTextToValueFunction_POLARITY,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_uModMatPolarity3, parameters, 1, "m_uModMatPolarity3", "ModMatrix 3 Polarity (unipolar, bipolar)", "ModMatrix 3 Polarity (unipolar, bipolar)", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, POLARITY_Array.size() - 1, 1.0f), 1.0f,
		CVASTParamState::comboBoxValueToTextFunction_POLARITY,
		CVASTParamState::comboBoxTextToValueFunction_POLARITY,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_uModMatPolarity4, parameters, 1, "m_uModMatPolarity4", "ModMatrix 4 Polarity (unipolar, bipolar)", "ModMatrix 4 Polarity (unipolar, bipolar)", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, POLARITY_Array.size() - 1, 1.0f), 1.0f,
		CVASTParamState::comboBoxValueToTextFunction_POLARITY,
		CVASTParamState::comboBoxTextToValueFunction_POLARITY,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_uModMatPolarity5, parameters, 1, "m_uModMatPolarity5", "ModMatrix 5 Polarity (unipolar, bipolar)", "ModMatrix 5 Polarity (unipolar, bipolar)", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, POLARITY_Array.size() - 1, 1.0f), 1.0f,
		CVASTParamState::comboBoxValueToTextFunction_POLARITY,
		CVASTParamState::comboBoxTextToValueFunction_POLARITY,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_uModMatPolarity6, parameters, 1, "m_uModMatPolarity6", "ModMatrix 6 Polarity (unipolar, bipolar)", "ModMatrix 6 Polarity (unipolar, bipolar)", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, POLARITY_Array.size() - 1, 1.0f), 1.0f,
		CVASTParamState::comboBoxValueToTextFunction_POLARITY,
		CVASTParamState::comboBoxTextToValueFunction_POLARITY,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_uModMatPolarity7, parameters, 1, "m_uModMatPolarity7", "ModMatrix 7 Polarity (unipolar, bipolar)", "ModMatrix 7 Polarity (unipolar, bipolar)", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, POLARITY_Array.size() - 1, 1.0f), 1.0f,
		CVASTParamState::comboBoxValueToTextFunction_POLARITY,
		CVASTParamState::comboBoxTextToValueFunction_POLARITY,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_uModMatPolarity8, parameters, 1, "m_uModMatPolarity8", "ModMatrix 8 Polarity (unipolar, bipolar)", "ModMatrix 8 Polarity (unipolar, bipolar)", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, POLARITY_Array.size() - 1, 1.0f), 1.0f,
		CVASTParamState::comboBoxValueToTextFunction_POLARITY,
		CVASTParamState::comboBoxTextToValueFunction_POLARITY,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_uModMatPolarity9, parameters, 1, "m_uModMatPolarity9", "ModMatrix 9 Polarity (unipolar, bipolar)", "ModMatrix 9 Polarity (unipolar, bipolar)", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, POLARITY_Array.size() - 1, 1.0f), 1.0f,
		CVASTParamState::comboBoxValueToTextFunction_POLARITY,
		CVASTParamState::comboBoxTextToValueFunction_POLARITY,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_uModMatPolarity10, parameters, 1, "m_uModMatPolarity10", "ModMatrix 10 Polarity (unipolar, bipolar)", "ModMatrix 10 Polarity (unipolar, bipolar)", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, POLARITY_Array.size() - 1, 1.0f), 1.0f,
		CVASTParamState::comboBoxValueToTextFunction_POLARITY,
		CVASTParamState::comboBoxTextToValueFunction_POLARITY,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_uModMatPolarity11, parameters, 1, "m_uModMatPolarity11", "ModMatrix 11 Polarity (unipolar, bipolar)", "ModMatrix 11 Polarity (unipolar, bipolar)", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, POLARITY_Array.size() - 1, 1.0f), 1.0f,
		CVASTParamState::comboBoxValueToTextFunction_POLARITY,
		CVASTParamState::comboBoxTextToValueFunction_POLARITY,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_uModMatPolarity12, parameters, 1, "m_uModMatPolarity12", "ModMatrix 12 Polarity (unipolar, bipolar)", "ModMatrix 12 Polarity (unipolar, bipolar)", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, POLARITY_Array.size() - 1, 1.0f), 1.0f,
		CVASTParamState::comboBoxValueToTextFunction_POLARITY,
		CVASTParamState::comboBoxTextToValueFunction_POLARITY,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_uModMatPolarity13, parameters, 1, "m_uModMatPolarity13", "ModMatrix 13 Polarity (unipolar, bipolar)", "ModMatrix 13 Polarity (unipolar, bipolar)", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, POLARITY_Array.size() - 1, 1.0f), 1.0f,
		CVASTParamState::comboBoxValueToTextFunction_POLARITY,
		CVASTParamState::comboBoxTextToValueFunction_POLARITY,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_uModMatPolarity14, parameters, 1, "m_uModMatPolarity14", "ModMatrix 14 Polarity (unipolar, bipolar)", "ModMatrix 14 Polarity (unipolar, bipolar)", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, POLARITY_Array.size() - 1, 1.0f), 1.0f,
		CVASTParamState::comboBoxValueToTextFunction_POLARITY,
		CVASTParamState::comboBoxTextToValueFunction_POLARITY,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_uModMatPolarity15, parameters, 1, "m_uModMatPolarity15", "ModMatrix 15 Polarity (unipolar, bipolar)", "ModMatrix 15 Polarity (unipolar, bipolar)", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, POLARITY_Array.size() - 1, 1.0f), 1.0f,
		CVASTParamState::comboBoxValueToTextFunction_POLARITY,
		CVASTParamState::comboBoxTextToValueFunction_POLARITY,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_uModMatPolarity16, parameters, 1, "m_uModMatPolarity16", "ModMatrix 16 Polarity (unipolar, bipolar)", "ModMatrix 16 Polarity (unipolar, bipolar)", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, POLARITY_Array.size() - 1, 1.0f), 1.0f,
		CVASTParamState::comboBoxValueToTextFunction_POLARITY,
		CVASTParamState::comboBoxTextToValueFunction_POLARITY,
		false, true, false, false,
		true);

	return true;
}

bool CVASTParamState::initCompatibilityParameters3(AudioProcessorValueTreeState& parameters, CVASTEffect* effect) {
	//added at the end of list for parameters after release to keep DAW sorting order

	effect->createAndAddParameter(&m_bMSEGPerVoice_MSEG1, parameters, 1, "m_bMSEGPerVoice_MSEG1", "MSEG 1 Seperate MSEG per voice or global", "MSEG 1 Seperate MSEG per voice or global", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 1.0f, 1.0f), 1.0f,
		CVASTParamState::toggleButtonValueToTextFunction,
		CVASTParamState::toggleButtonTextToValueFunction,
		false, true, true, false,
		true);

	effect->createAndAddParameter(&m_bMSEGSynch_MSEG1, parameters, 1, "m_bMSEGSynch_MSEG1", "MSEG 1 Sync to DAW timecode", "MSEG 1 Sync to DAW timecode", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 1.0f, 1.0f), 0.0f,
		CVASTParamState::toggleButtonValueToTextFunction,
		CVASTParamState::toggleButtonTextToValueFunction,
		false, true, true, false,
		true);

	effect->createAndAddParameter(&m_uMSEGTimeBeats_MSEG1, parameters, 1, "m_uMSEGTimeBeats_MSEG1", "MSEG 1 Time in beats when synched to DAW", "Time (DAW)", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, TIMEBEATS_Array.size() - 1, 1.0f), TIMEBEATS::BEATS1,
		CVASTParamState::comboBoxValueToTextFunction_TIMEBEATS,
		CVASTParamState::comboBoxTextToValueFunction_TIMEBEATS,
		false, true, true, false,
		true);

	effect->createAndAddParameter(&m_fAttackSteps_MSEG1, parameters, 1, "m_fAttackSteps_MSEG1", "MSEG 1 Attack steps of beats", "MSEG 1 Attack steps", 0,
		MODMATDEST::MSEG1AttackSteps,
		NormalisableRange<float>(0, 64, 1), 0.f, //discrete int
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_fDecaySteps_MSEG1, parameters, 1, "m_fDecaySteps_MSEG1", "MSEG 1 Decay steps of beats", "MSEG 1 Decay steps", 0,
		MODMATDEST::MSEG1DecaySteps,
		NormalisableRange<float>(0, 64, 1), 1.f, //discrete int
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_fReleaseSteps_MSEG1, parameters, 1, "m_fReleaseSteps_MSEG1", "MSEG 2 Release steps", "MSEG 2 Release steps", 0,
		MODMATDEST::MSEG1ReleaseSteps,
		NormalisableRange<float>(0, 64, 1), 1.f, //discrete int
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_bMSEGPerVoice_MSEG2, parameters, 1, "m_bMSEGPerVoice_MSEG2", "MSEG 2 Seperate MSEG per voice or global", "MSEG 2 Seperate MSEG per voice or global", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 1.0f, 1.0f), 1.0f,
		CVASTParamState::toggleButtonValueToTextFunction,
		CVASTParamState::toggleButtonTextToValueFunction,
		false, true, true, false,
		true);

	effect->createAndAddParameter(&m_bMSEGSynch_MSEG2, parameters, 1, "m_bMSEGSynch_MSEG2", "MSEG 2 Sync to DAW timecode", "MSEG 2 Sync to DAW timecode", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 1.0f, 1.0f), 0.0f,
		CVASTParamState::toggleButtonValueToTextFunction,
		CVASTParamState::toggleButtonTextToValueFunction,
		false, true, true, false,
		true);

	effect->createAndAddParameter(&m_uMSEGTimeBeats_MSEG2, parameters, 1, "m_uMSEGTimeBeats_MSEG2", "MSEG 2 Time in beats when synched to DAW", "Time (DAW)", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, TIMEBEATS_Array.size() - 1, 1.0f), TIMEBEATS::BEATS1,
		CVASTParamState::comboBoxValueToTextFunction_TIMEBEATS,
		CVASTParamState::comboBoxTextToValueFunction_TIMEBEATS,
		false, true, true, false,
		true);

	effect->createAndAddParameter(&m_fAttackSteps_MSEG2, parameters, 1, "m_fAttackSteps_MSEG2", "MSEG 2 Attack steps of beats", "MSEG 2 Attack steps", 0,
		MODMATDEST::MSEG2AttackSteps,
		NormalisableRange<float>(0, 64, 1), 0.f, //discrete int
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_fDecaySteps_MSEG2, parameters, 1, "m_fDecaySteps_MSEG2", "MSEG 2 Decay steps of beats", "MSEG 2 Decay steps", 0,
		MODMATDEST::MSEG2DecaySteps,
		NormalisableRange<float>(0, 64, 1), 1.f, //discrete int
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_fReleaseSteps_MSEG2, parameters, 1, "m_fReleaseSteps_MSEG2", "MSEG 2 Release steps", "MSEG 2 Release steps", 0,
		MODMATDEST::MSEG2ReleaseSteps,
		NormalisableRange<float>(0, 64, 1), 1.f, //discrete int
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_bMSEGPerVoice_MSEG3, parameters, 1, "m_bMSEGPerVoice_MSEG3", "MSEG 3 Seperate MSEG per voice or global", "MSEG 3 Seperate MSEG per voice or global", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 1.0f, 1.0f), 1.0f,
		CVASTParamState::toggleButtonValueToTextFunction,
		CVASTParamState::toggleButtonTextToValueFunction,
		false, true, true, false,
		true);

	effect->createAndAddParameter(&m_bMSEGSynch_MSEG3, parameters, 1, "m_bMSEGSynch_MSEG3", "MSEG 3 Sync to DAW timecode", "MSEG 3 Sync to DAW timecode", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 1.0f, 1.0f), 0.0f,
		CVASTParamState::toggleButtonValueToTextFunction,
		CVASTParamState::toggleButtonTextToValueFunction,
		false, true, true, false,
		true);

	effect->createAndAddParameter(&m_uMSEGTimeBeats_MSEG3, parameters, 1, "m_uMSEGTimeBeats_MSEG3", "MSEG 3 Time in beats when synched to DAW", "Time (DAW)", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, TIMEBEATS_Array.size() - 1, 1.0f), TIMEBEATS::BEATS1,
		CVASTParamState::comboBoxValueToTextFunction_TIMEBEATS,
		CVASTParamState::comboBoxTextToValueFunction_TIMEBEATS,
		false, true, true, false,
		true);

	effect->createAndAddParameter(&m_fAttackSteps_MSEG3, parameters, 1, "m_fAttackSteps_MSEG3", "MSEG 3 Attack steps of beats", "MSEG 3 Attack steps", 0,
		MODMATDEST::MSEG3AttackSteps,
		NormalisableRange<float>(0, 64, 1), 0.f, //discrete int
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_fDecaySteps_MSEG3, parameters, 1, "m_fDecaySteps_MSEG3", "MSEG 3 Decay steps of beats", "MSEG 3 Decay steps", 0,
		MODMATDEST::MSEG3DecaySteps,
		NormalisableRange<float>(0, 64, 1), 1.f, //discrete int
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_fReleaseSteps_MSEG3, parameters, 1, "m_fReleaseSteps_MSEG3", "MSEG 3 Release steps", "MSEG 3 Release steps", 0,
		MODMATDEST::MSEG3ReleaseSteps,
		NormalisableRange<float>(0, 64, 1), 1.f, //discrete int
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_bMSEGPerVoice_MSEG4, parameters, 1, "m_bMSEGPerVoice_MSEG4", "MSEG 4 Seperate MSEG per voice or global", "MSEG 4 Seperate MSEG per voice or global", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 1.0f, 1.0f), 1.0f,
		CVASTParamState::toggleButtonValueToTextFunction,
		CVASTParamState::toggleButtonTextToValueFunction,
		false, true, true, false,
		true);

	effect->createAndAddParameter(&m_bMSEGSynch_MSEG4, parameters, 1, "m_bMSEGSynch_MSEG4", "MSEG 4 Sync to DAW timecode", "MSEG 4 Sync to DAW timecode", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 1.0f, 1.0f), 0.0f,
		CVASTParamState::toggleButtonValueToTextFunction,
		CVASTParamState::toggleButtonTextToValueFunction,
		false, true, true, false,
		true);

	effect->createAndAddParameter(&m_uMSEGTimeBeats_MSEG4, parameters, 1, "m_uMSEGTimeBeats_MSEG4", "MSEG 4 Time in beats when synched to DAW", "Time (DAW)", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, TIMEBEATS_Array.size() - 1, 1.0f), TIMEBEATS::BEATS1,
		CVASTParamState::comboBoxValueToTextFunction_TIMEBEATS,
		CVASTParamState::comboBoxTextToValueFunction_TIMEBEATS,
		false, true, true, false,
		true);

	effect->createAndAddParameter(&m_fAttackSteps_MSEG4, parameters, 1, "m_fAttackSteps_MSEG4", "MSEG 4 Attack steps of beats", "MSEG 4 Attack steps", 0,
		MODMATDEST::MSEG4AttackSteps,
		NormalisableRange<float>(0, 64, 1), 0.f, //discrete int
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_fDecaySteps_MSEG4, parameters, 1, "m_fDecaySteps_MSEG4", "MSEG 4 Decay steps of beats", "MSEG 4 Decay steps", 0,
		MODMATDEST::MSEG4DecaySteps,
		NormalisableRange<float>(0, 64, 1), 1.f, //discrete int
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_fReleaseSteps_MSEG4, parameters, 1, "m_fReleaseSteps_MSEG4", "MSEG 4 Release steps", "MSEG 4 Release steps", 0,
		MODMATDEST::MSEG4ReleaseSteps,
		NormalisableRange<float>(0, 64, 1), 1.f, //discrete int
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_bMSEGPerVoice_MSEG5, parameters, 1, "m_bMSEGPerVoice_MSEG5", "MSEG 5 Seperate MSEG per voice or global", "MSEG 5 Seperate MSEG per voice or global", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 1.0f, 1.0f), 1.0f,
		CVASTParamState::toggleButtonValueToTextFunction,
		CVASTParamState::toggleButtonTextToValueFunction,
		false, true, true, false,
		true);

	effect->createAndAddParameter(&m_bMSEGSynch_MSEG5, parameters, 1, "m_bMSEGSynch_MSEG5", "MSEG 5 Sync to DAW timecode", "MSEG 5 Sync to DAW timecode", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, 1.0f, 1.0f), 0.0f,
		CVASTParamState::toggleButtonValueToTextFunction,
		CVASTParamState::toggleButtonTextToValueFunction,
		false, true, true, false,
		true);

	effect->createAndAddParameter(&m_uMSEGTimeBeats_MSEG5, parameters, 1, "m_uMSEGTimeBeats_MSEG5", "MSEG 5 Time in beats when synched to DAW", "Time (DAW)", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, TIMEBEATS_Array.size() - 1, 1.0f), TIMEBEATS::BEATS1,
		CVASTParamState::comboBoxValueToTextFunction_TIMEBEATS,
		CVASTParamState::comboBoxTextToValueFunction_TIMEBEATS,
		false, true, true, false,
		true);

	effect->createAndAddParameter(&m_fAttackSteps_MSEG5, parameters, 1, "m_fAttackSteps_MSEG5", "MSEG 5 Attack steps of beats", "MSEG 5 Attack steps", 0,
		MODMATDEST::MSEG5AttackSteps,
		NormalisableRange<float>(0, 64, 1), 0.f, //discrete int
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_fDecaySteps_MSEG5, parameters, 1, "m_fDecaySteps_MSEG5", "MSEG 5 Decay steps of beats", "MSEG 5 Decay steps", 0,
		MODMATDEST::MSEG5DecaySteps,
		NormalisableRange<float>(0, 64, 1), 1.f, //discrete int
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	effect->createAndAddParameter(&m_fReleaseSteps_MSEG5, parameters, 1, "m_fReleaseSteps_MSEG5", "MSEG 5 Release steps", "MSEG 5 Release steps", 0,
		MODMATDEST::MSEG5ReleaseSteps,
		NormalisableRange<float>(0, 64, 1), 1.f, //discrete int
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false,
		true);

	return true;
}

bool CVASTParamState::initCompatibilityParameters4(AudioProcessorValueTreeState& parameters, CVASTEffect* effect) {
	//added at the end of list for parameters after release to keep DAW sorting order

	effect->createAndAddParameter(&m_uWTFX_OscA, parameters, 1, "m_uWTFX_OscA", "OSC A Wavetable effect", "OSC A Wavetable effect", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, WTFX_Array.size() - 1, 1.0f), 0.0f,
		CVASTParamState::comboBoxValueToTextFunction_WTFX,
		CVASTParamState::comboBoxTextToValueFunction_WTFX,
		false, true, true, false);

	effect->createAndAddParameter(&m_uWTFX_OscB, parameters, 1, "m_uWTFX_OscB", "OSC B Wavetable effect", "OSC B Wavetable effect", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, WTFX_Array.size() - 1, 1.0f), 0.0f,
		CVASTParamState::comboBoxValueToTextFunction_WTFX,
		CVASTParamState::comboBoxTextToValueFunction_WTFX,
		false, true, true, false);

	effect->createAndAddParameter(&m_uWTFX_OscC, parameters, 1, "m_uWTFX_OscC", "OSC C Wavetable effect", "OSC C Wavetable effect", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, WTFX_Array.size() - 1, 1.0f), 0.0f,
		CVASTParamState::comboBoxValueToTextFunction_WTFX,
		CVASTParamState::comboBoxTextToValueFunction_WTFX,
		false, true, true, false);

	effect->createAndAddParameter(&m_uWTFX_OscD, parameters, 1, "m_uWTFX_OscD", "OSC D Wavetable effect", "OSC D Wavetable effect", 0,
		MODMATDEST::NoDestination,
		NormalisableRange<float>(0.0f, WTFX_Array.size() - 1, 1.0f), 0.0f,
		CVASTParamState::comboBoxValueToTextFunction_WTFX,
		CVASTParamState::comboBoxTextToValueFunction_WTFX,
		false, true, true, false);

	effect->createAndAddParameter(&m_fWTFXVal_OscA, parameters, 1, "m_fWTFXVal_OscA", "OSC A Wavetable effect parameter value", "OSC A Wavetable effect parameter value", 0,
		MODMATDEST::OscAWTFXVal, 
		NormalisableRange<float>(0, 100), 0.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);

	effect->createAndAddParameter(&m_fWTFXVal_OscB, parameters, 1, "m_fWTFXVal_OscB", "OSC B Wavetable effect parameter value", "OSC B Wavetable effect parameter value", 0,
		MODMATDEST::OscBWTFXVal,
		NormalisableRange<float>(0, 100), 0.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);

	effect->createAndAddParameter(&m_fWTFXVal_OscC, parameters, 1, "m_fWTFXVal_OscC", "OSC C Wavetable effect parameter value", "OSC C Wavetable effect parameter value", 0,
		MODMATDEST::OscCWTFXVal,
		NormalisableRange<float>(0, 100), 0.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);

	effect->createAndAddParameter(&m_fWTFXVal_OscD, parameters, 1, "m_fWTFXVal_OscD", "OSC D Wavetable effect parameter value", "OSC D Wavetable effect parameter value", 0,
		MODMATDEST::OscDWTFXVal,
		NormalisableRange<float>(0, 100), 0.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);
	
	effect->createAndAddParameter(&m_fSamplerGrainInteronset, parameters, 1, "m_fSamplerGrainInteronset", "Sampler Grain inter-onset (ms)", "Sampler Grain inter-onset (ms)", 0,
		MODMATDEST::SamplerGrainInteronset,
		NormalisableRange<float>(1.f, 101.f, 0.00001f, 0.99f, false), 1.f, //1.f is off
		CVASTParamState::floatSliderValueToTextFunctionMaxIsOff, //100 is hardcoded
		CVASTParamState::floatSliderTextToValueFunctionMaxIsOff, //100 is hardcoded
		false, true, false, false);

	effect->createAndAddParameter(&m_fSamplerGrainRandom, parameters, 1, "m_fSamplerGrainRandom", "Sampler grain inter-onset and detune random", "Sampler grain inter-onset and detune random", 0,
		MODMATDEST::SamplerGrainRandom,
		NormalisableRange<float>(0, 100), 0.f,
		CVASTParamState::floatSliderValueToTextFunction, 
		CVASTParamState::floatSliderTextToValueFunction, 
		false, true, false, false);

	effect->createAndAddParameter(&m_fSamplerGrainShape, parameters, 1, "m_fSamplerGrainShape", "Sampler Grain shape", "Sampler Grain shape", 0,
		MODMATDEST::SamplerGrainShape,
		NormalisableRange<float>(0, 100), 0.f,
		CVASTParamState::floatSliderValueToTextFunction,
		CVASTParamState::floatSliderTextToValueFunction,
		false, true, false, false);

	return true;
    JUCE_END_IGNORE_WARNINGS_MSVC
    JUCE_END_IGNORE_WARNINGS_GCC_LIKE
}
