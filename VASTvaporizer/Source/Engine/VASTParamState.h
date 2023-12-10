/*
VAST Dynamics Audio Software (TM)
*/

#pragma once

#include "VASTEngineHeader.h"

const StringArray MODMATDEST_Array = StringArray("---", "Pitch", "VoiceGain", "LFO1Frequency", "LFO2Frequency", "LFO3Frequency", "LFO4Frequency", "LFO5Frequency", "OscAGain", "OscAPan", "OscACents", "OscADetune", "OscASpread", "OscAPhase", "OscAWTPos", "OscAMorph", "OscBGain", "OscBPan", "OscBCents", "OscBDetune", "OscBSpread", "OscBPhase", "OscBWTPos", "OscBMorph", "OscCGain", "OscCPan", "OscCCents", "OscCDetune", "OscCSpread", "OscCPhase", "OscCWTPos", "OscCMorph", "OscDGain", "OscDPan", "OscDCents", "OscDDetune", "OscDSpread", "OscDPhase", "OscDWTPos", "OscDMorph", "MSEG1Attack", "MSEG1Decay", "MSEG1Sustain", "MSEG1Release", "MSEG2Attack", "MSEG2Decay", "MSEG2Sustain", "MSEG2Release", "MSEG3Attack", "MSEG3Decay", "MSEG3Sustain", "MSEG3Release", "MSEG4Attack", "MSEG4Decay", "MSEG4Sustain", "MSEG4Release", "MSEG5Attack", "MSEG5Decay", "MSEG5Sustain", "MSEG5Release", "MSEG1AttackSteps", "MSEG1DecaySteps", "MSEG1ReleaseSteps", "MSEG2AttackSteps", "MSEG2DecaySteps", "MSEG2ReleaseSteps", "MSEG3AttackSteps", "MSEG3DecaySteps", "MSEG3ReleaseSteps", "MSEG4AttackSteps", "MSEG4DecaySteps", "MSEG4ReleaseSteps", "MSEG5AttackSteps", "MSEG5DecaySteps", "MSEG5ReleaseSteps", "SamplerPitch", "SamplerGain", "SamplerPan", "SamplerLoopRange", "NoiseGain", "NoisePan", "NoiseLowCut", "NoiseHighCut", "NoiseResonance", "Filter1Mixin", "Filter1Drive", "Filter1Cutoff", "Filter1Resonance", "Filter1EnvMod", "Filter1Scale", "Filter1Gain", "Filter2Mixin", "Filter2Drive", "Filter2Cutoff", "Filter2Resonance", "Filter2EnvMod", "Filter2Scale", "Filter2Gain", "Filter3Mixin", "Filter3Drive", "Filter3Cutoff", "Filter3Resonance", "Filter3EnvMod", "Filter3Scale", "Filter3Gain", "ArpSpeed", "EQGain", "ChorusDryWet", "ChorusFrequency", "ChorusDepth", "ChorusGain", "PhaserDryWet", "PhaserWidth", "PhaserFeedback", "PhaserLFOFrequency", "PhaserGain", "FlangerDryWet", "FlangerDelay", "FlangerWidth", "FlangerFeedback", "FlangerLFOFrequency", "FlangerGain", "AtomizerDryWet", "AtomizerLowCut", "AtomizerHighCut", "AtomizerHarmonics", "AtomizerEmphasis", "AtomizerDephase", "AtomizerLFOFrequency", "AtomizerGain", "DistortionDryWet", "DistortionPreGain", "DistortionLowCut", "DistortionDrive", "DistortionFuzz", "DistortionGain", "BitcrushDryWet", "BitcrushLowCut", "BitcrushBitdepth", "BitcrushBitrate", "BitcrushJitter", "BitcrushGain", "CombDryWet", "CombFrequency", "CombLevel", "CombDrive", "CombGain", "FormantFilterDryWet", "FormantFilterFirstVowel", "FormantFilterSecondVowel", "FormantFilterMixVowels", "FormantFilterGain", "CompressorHighFreq", "CompressorLowFreq", "CompressorLowGain", "CompressorMidGain", "CompressorHighGain", "CompressorThresholdHighBand", "CompressorRatioHighBand", "CompressorThresholdMidBand", "CompressorRatioMidBand", "CompressorThresholdLowBand", "CompressorRatioLowBand", "LimiterDryWet", "LimiterThreshold", "LimiterRatio", "LimiterGain", "DelayDryWet", "DelayLowCut", "DelayHighCut", "DelayFeedback", "DelayGain", "ReverbDryWet", "ReverbLowcut", "ReverbHighcut", "ReverbPredelay", "ReverbSize", "ReverbFeedback", "ReverbDamping", "ReverbGain", "Fxbus1MonoFrequency", "Fxbus2MonoFrequency", "Fxbus3MonoFrequency", "ModMatrixValue1", "ModMatrixValue2", "ModMatrixValue3", "ModMatrixValue4", "ModMatrixValue5", "ModMatrixValue6", "ModMatrixValue7", "ModMatrixValue8", "ModMatrixValue9", "ModMatrixValue10", "ModMatrixValue11", "ModMatrixValue12", "ModMatrixValue13", "ModMatrixValue14", "ModMatrixValue15", "ModMatrixValue16", "OscAWTFXVal", "OscBWTFXVal", "OscCWTFXVal", "OscDWTFXVal", "SamplerGrainInteronset", "SamplerGrainRandom", "SamplerGrainShape", "WaveshaperDryWet", "WaveshaperPreGain", "WaveshaperLowCut", "WaveshaperHighCut", "WaveshaperDrive", "WaveshaperGain");
enum MODMATDEST { NoDestination, Pitch, VoiceGain, LFO1Frequency, LFO2Frequency, LFO3Frequency, LFO4Frequency, LFO5Frequency, OscAGain, OscAPan, OscACents, OscADetune, OscASpread, OscAPhase, OscAWTPos, OscAMorph, OscBGain, OscBPan, OscBCents, OscBDetune, OscBSpread, OscBPhase, OscBWTPos, OscBMorph, OscCGain, OscCPan, OscCCents, OscCDetune, OscCSpread, OscCPhase, OscCWTPos, OscCMorph, OscDGain, OscDPan, OscDCents, OscDDetune, OscDSpread, OscDPhase, OscDWTPos, OscDMorph, MSEG1Attack, MSEG1Decay, MSEG1Sustain, MSEG1Release, MSEG2Attack, MSEG2Decay, MSEG2Sustain, MSEG2Release, MSEG3Attack, MSEG3Decay, MSEG3Sustain, MSEG3Release, MSEG4Attack, MSEG4Decay, MSEG4Sustain, MSEG4Release, MSEG5Attack, MSEG5Decay, MSEG5Sustain, MSEG5Release, MSEG1AttackSteps, MSEG1DecaySteps, MSEG1ReleaseSteps, MSEG2AttackSteps, MSEG2DecaySteps, MSEG2ReleaseSteps, MSEG3AttackSteps, MSEG3DecaySteps, MSEG3ReleaseSteps, MSEG4AttackSteps, MSEG4DecaySteps, MSEG4ReleaseSteps, MSEG5AttackSteps, MSEG5DecaySteps, MSEG5ReleaseSteps, SamplerPitch, SamplerGain, SamplerPan, SamplerLoopRange, NoiseGain, NoisePan, NoiseLowCut, NoiseHighCut, NoiseResonance, Filter1Mixin, Filter1Drive, Filter1Cutoff, Filter1Resonance, Filter1EnvMod, Filter1Scale, Filter1Gain, Filter2Mixin, Filter2Drive, Filter2Cutoff, Filter2Resonance, Filter2EnvMod, Filter2Scale, Filter2Gain, Filter3Mixin, Filter3Drive, Filter3Cutoff, Filter3Resonance, Filter3EnvMod, Filter3Scale, Filter3Gain, ArpSpeed, EQGain, ChorusDryWet, ChorusFrequency, ChorusDepth, ChorusGain, PhaserDryWet, PhaserWidth, PhaserFeedback, PhaserLFOFrequency, PhaserGain, FlangerDryWet, FlangerDelay, FlangerWidth, FlangerFeedback, FlangerLFOFrequency, FlangerGain, AtomizerDryWet, AtomizerLowCut, AtomizerHighCut, AtomizerHarmonics, AtomizerEmphasis, AtomizerDephase, AtomizerLFOFrequency, AtomizerGain, DistortionDryWet, DistortionPreGain, DistortionLowCut, DistortionDrive, DistortionFuzz, DistortionGain, BitcrushDryWet, BitcrushLowCut, BitcrushBitdepth, BitcrushBitrate, BitcrushJitter, BitcrushGain, CombDryWet, CombFrequency, CombLevel, CombDrive, CombGain, FormantFilterDryWet, FormantFilterFirstVowel, FormantFilterSecondVowel, FormantFilterMixVowels, FormantFilterGain, CompressorHighFreq, CompressorLowFreq, CompressorLowGain, CompressorMidGain, CompressorHighGain, CompressorThresholdHighBand, CompressorRatioHighBand, CompressorThresholdMidBand, CompressorRatioMidBand, CompressorThresholdLowBand, CompressorRatioLowBand, LimiterDryWet, LimiterThreshold, LimiterRatio, LimiterGain, DelayDryWet, DelayLowCut, DelayHighCut, DelayFeedback, DelayGain, ReverbDryWet, ReverbLowcut, ReverbHighcut, ReverbPredelay, ReverbSize, ReverbFeedback, ReverbDamping, ReverbGain, Fxbus1MonoFrequency, Fxbus2MonoFrequency, Fxbus3MonoFrequency, ModMatrixValue1, ModMatrixValue2, ModMatrixValue3, ModMatrixValue4, ModMatrixValue5, ModMatrixValue6, ModMatrixValue7, ModMatrixValue8, ModMatrixValue9, ModMatrixValue10, ModMatrixValue11, ModMatrixValue12, ModMatrixValue13, ModMatrixValue14, ModMatrixValue15, ModMatrixValue16, OscAWTFXVal, OscBWTFXVal, OscCWTFXVal, OscDWTFXVal, SamplerGrainInteronset, SamplerGrainRandom, SamplerGrainShape, WaveshaperDryWet, WaveshaperPreGain, WaveshaperLowCut, WaveshaperHighCut, WaveshaperDrive, WaveshaperGain}; //M_MODMATRIX_MAX_DESTINATIONS check

const StringArray WAVE_Array = StringArray("Sine", "Saw", "Triangle", "Pulse", "Noise", "Sawsine", "Sample&Hold", "Wavetable");
const StringArray LFOWAVE_Array = StringArray("Sine", "Saw", "Triangle", "Pulse", "Noise", "Sawsine", "Sample&Hold");
enum WAVE { sine, saw, tri, square, noise, sawsine, samplehold, wavetable };

const StringArray MODMATSRCE_Array = StringArray("---", "Velocity", "AfterTouch", "ModWheel", "MSEG1Env", "MSEG2Env", "MSEG3Env", "MSEG4Env", "MSEG5Env", "LFO1", "LFO2", "LFO3", "LFO4", "LFO5", "StepSeq1", "StepSeq2", "StepSeq3", "CustomModulator1", "CustomModulator2", "CustomModulator3", "CustomModulator4", "KeyTrack", "InputWave", "InputEnvelope", "RandomDriftSlow", "RandomDriftFast", "MPETimbreMinMax", "MPETimbreMax", "MPETimbreMin");
enum MODMATSRCE { NoSource, Velocity, AfterTouch, ModWheel, MSEG1Env, MSEG2Env, MSEG3Env, MSEG4Env, MSEG5Env, LFO1, LFO2, LFO3, LFO4, LFO5, StepSeq1, StepSeq2, StepSeq3, CustomModulator1, CustomModulator2, CustomModulator3, CustomModulator4, KeyTrack, InputWave, InputEnvelope, RandomDriftSlow, RandomDriftFast, MPETimbreMinMax, MPETimbreMax, MPETimbreMin}; //30 is max

const StringArray OSCROUTE_Array = StringArray("---", "Filter1", "Filter2", "Filter3", "FXBus1", "FXBus2", "FXBus3", "Master");
enum OSCROUTE { OSCROUTE_NoRoute, OSCROUTE_Filter1, OSCROUTE_Filter2, OSCROUTE_Filter3, OSCROUTE_FXBus1, OSCROUTE_FXBus2, OSCROUTE_FXBus3, OSCROUTE_Master };

const StringArray FILTER1ROUTE_Array = StringArray("---", "---", "Filter2", "Filter3", "FXBus1", "FXBus2", "FXBus3", "Master");
enum FILTER1ROUTE { FILTER1ROUTE_NoRoute, FILTER1ROUTE_NoRoute2, FILTER1ROUTE_Filter2, FILTER1ROUTE_Filter3, FILTER1ROUTE_FXBus1, FILTER1ROUTE_FXBus2, FILTER1ROUTE_FXBus3, FILTER1ROUTE_Master };
const StringArray FILTER2ROUTE_Array = StringArray("---", "---", "---", "Filter3", "FXBus1", "FXBus2", "FXBus3", "Master");
enum FILTER2ROUTE { FILTER2ROUTE_NoRoute, FILTER2ROUTE_NoRoute2, FILTER2ROUTE_NoRoute3, FILTER2ROUTE_Filter3, FILTER2ROUTE_FXBus1, FILTER2ROUTE_FXBus2, FILTER2ROUTE_FXBus3, FILTER2ROUTE_Master };
const StringArray FILTER3ROUTE_Array = StringArray("---", "---", "---", "---", "FXBus1", "FXBus2", "FXBus3", "Master");
enum FILTER3ROUTE { FILTER3ROUTE_NoRoute, FILTER3ROUTE_NoRoute2, FILTER3ROUTE_NoRoute3, FILTER3ROUTE_NoRoute4, FILTER3ROUTE_FXBus1, FILTER3ROUTE_FXBus2, FILTER3ROUTE_FXBus3, FILTER3ROUTE_Master }; //compatibility!

const StringArray FXBUSROUTE_Array = StringArray("---", "FXBus2", "FXBus3", "Master");
enum FXBUSROUTE { FXBUSROUTE_NoRoute, FXBUSROUTE_FXBus2, FXBUSROUTE_FXBus3, FXBUSROUTE_Master };

const StringArray ARPMODE_Array = StringArray("Poly", "Up", "Down", "UpDown");
enum ARPMODE { POLY, UP, DOWN, UPDOWN };

const StringArray TIMEBEATS_Array = StringArray("1/256", "1/128", "1/64", "1/32", "1/16", "1/9", "1/8", "1/4", "1/3", "1/2", "1", "3/2", "2", "3", "4", "8", "16", "32", "64");
enum TIMEBEATS { BEATS1_256, BEATS1_128, BEATS1_64, BEATS1_32, BEATS1_16, BEATS1_9, BEATS1_8, BEATS1_4, BEATS1_3, BEATS1_2, BEATS1, BEATS3_2, BEATS2, BEATS3, BEATS4, BEATS8, BEATS16, BEATS32, BEATS64};

const StringArray NOISETYPE_Array = StringArray("White", "Pink", "Brown");
enum NOISETYPE { White, Pink, Brown };

const StringArray MSEGENV_Array = StringArray("MSEG1", "MSEG2", "MSEG3", "MSEG4", "MSEG5");
enum MSEGENV { MSEG1, MSEG2, MSEG3, MSEG4, MSEG5 };

const StringArray MSEGLFOENV_Array = StringArray("--", "MSEG1", "MSEG2", "MSEG3", "MSEG4", "MSEG5");
enum MSEGLFOENV { MSEGLFONONE, MSEGLFO1, MSEGLFO2, MSEGLFO3, MSEGLFO4, MSEGLFO5 };

const StringArray FILTERTYPE_Array = StringArray(
											 	 "DQF LPF 12", "DQF LPF 24", "DQF BRF 12", "DQF BPF 12", "DQF HPF 12", "DQF LDF 6", "DQF LDF 12", "DQF LDF 18", "DQF LDF 24", "DQF COMB",
										         "DQF LPF 12 SINE WS", "DQF LPF 24 SINE WS", "DQF BRF 12 SINE WS", "DQF BPF 12 SINE WS", "DQF HPF 12 SINE WS", "DQF LDF 12 SINE WS", "DQF COMB SINE WS",
												 "BQF LPF 6", "BQF HPF 6", "BQF BPF 6", "BQF ALLPASS 6", "BQF LOWSHELF 6", "BQF HIGHSHELF 6", "BQF NOTCH 6", "BQF PEAK 6",
												 "BQF LPF 12", "BQF HPF 12", "BQF BPF 12", "BQF ALLPASS 12", "BQF LOWSHELF 12", "BQF HIGHSHELF 12", "BQF NOTCH 12", "BQF PEAK 12",
												 "BQF LPF 18", "BQF HPF 18", "BQF BPF 18", "BQF ALLPASS 18", "BQF LOWSHELF 18", "BQF HIGHSHELF 18", "BQF NOTCH 18", "BQF PEAK 18",
												 "LDF LPF 12", "LDF HPF 12", "LDF LPF 24", "LDF HPF 24", "LDF LPF 36", "LDF HPF 36", "LDF LPF 48", "LDF HPF 48", 
												 "SVF LPF 12", "SVF HPF 12", "SVF BPF 12", "SVF LPF 24", "SVF HPF 24", "SVF BPF 24", "SVF LPF 36", "SVF HPF 36", "SVF BPF 36", 
												 "COMB FILTER", "SCREAMFILTER", "KRG HPF 12", "KRG HPFSAT 12"
);
enum FILTERTYPE {							     
								               	 DQFLPF12, DQFLPF24, DQFBRF12, DQFBPF12, DQFHPF12, DQFLDF6, DQFLDF12, DQFLDF18, DQFLDF24, DQFCOMB,
											     DQFLPF12WSSINE, DQFLPF24WSSINE, DQFBRF12WSSINE, DQFBPF12WSSINE, DQFHPF12WSSINE, DQFLDF12WSSINE, DQFCOMBWSSINE,
												 BQFLPF6, BQFHPF6, BQFBPF6, BQFALLPASS6, BQFLOWSHELF6, BQFHIGHSHELF6, BQFNOTCH6, BQFPEAK6, 
												 BQFLPF12, BQFHPF12, BQFBPF12, BQFALLPASS12, BQFLOWSHELF12, BQFHIGHSHELF12, BQFNOTCH12, BQFPEAK12, 
												 BQFLPF18, BQFHPF18, BQFBPF18, BQFALLPASS18, BQFLOWSHELF18, BQFHIGHSHELF18, BQFNOTCH18, BQFPEAK18,	
												 LDFLPF12, LDFHPF12, LDFLPF24, LDFHPF24, LDFLPF36, LDFHPF36, LDFLPF48, LDFHPF48, 
												 SVFLPF12, SVFHPF12, SVFBPF12, SVFLPF24, SVFHPF24, SVFBPF24, SVFLPF36, SVFHPF36, SVFBPF36, 
												 COMBFILTER, SCREAMFILTER, KRGHPF12, KRGHPFSAT12,
};

const StringArray POLARITY_Array = StringArray("Unipolar", "Bipolar");
enum POLARITY { Unipolar, Bipolar };

const StringArray SWITCH_Array = StringArray("Off", "On");
enum SWITCH { SWITCH_OFF, SWITCH_ON };

const StringArray POLYMODE_Array = StringArray("Mono", "Poly4", "Poly16", "Poly32");
enum POLYMODE { MONO, POLY4, POLY16, POLY32 };

const StringArray MIDINOTES_Array = StringArray(	"c-2", "c#-2", "d-2", "d#-2", "e-2", "f-2", "f#-2", "g-2", "g#-2", "a-2", "a#-2", "b/h-2", 
													"c-1", "c#-1", "d-1", "d#-1", "e-1", "f-1", "f#-1", "g-1", "g#-1", "a-1", "a#-1", "b/h-1",
													"c0", "c#0", "d0", "d#0", "e0", "f0", "f#0", "g0", "g#0", "a0", "a#0", "b/h0",
													"c1", "c#1", "d1", "d#1", "e1", "f1", "f#1", "g1", "g#1", "a1", "a#1", "b/h1",
													"c2", "c#2", "d2", "d#2", "e2", "f2", "f#2", "g2", "g#2", "a2", "a#2", "b/h2",
													"c3", "c#3", "d3", "d#3", "e3", "f3", "f#3", "g3", "g#3", "a3", "a#3", "b/h3",
													"c4", "c#4", "d4", "d#4", "e4", "f4", "f#4", "g4", "g#4", "a4", "a#4", "b/h4",
													"c5", "c#5", "d5", "d#5", "e5", "f5", "f#5", "g5", "g#5", "a5", "a#5", "b/h5",
													"c6", "c#6", "d6", "d#6", "e6", "f6", "f#6", "g6", "g#6", "a6", "a#6", "b/h6",
													"c7", "c#7", "d7", "d#7", "e7", "f7", "f#7", "g7", "g#7", "a7", "a#7", "b/h7",
													"c8", "c#8", "d8", "d#8", "e8", "f8", "f#8", "g8"  ); //c3 = 60
// https://support.keithmcmillen.com/hc/en-us/articles/202658989-MIDI-notes-are-always-an-octave-off-in-insert-software-hardware-here-Is-something-wrong-

const StringArray WTFX_Array = StringArray("---", "Bend+", "Bend-", "Bend-/+", "Bloat+", "Bloat-", "Bloat-/+", "Dist", "HardSync", "HardSync/Wndw", "HardSync/Mix", 
										  "PWM", "FM", "FM/Wndw", "FM/Mix", "Flip", "Mirror", "Breed", "Breed/Wndw", "Breed/Mix", "Brew", "Brew/Wndw", "Brew/Mix",
										  "Sincify", "Sincify/Wndw", "Sincify/Mix", "Crush", "Smooth", "Window", "FM-from-Next");

class CVASTEffect; //forward declaration
class CVASTParamState
{
public:
	CVASTParamState();
	virtual ~CVASTParamState(void);
	//CVASTParamState(const CVASTParamState &stateToCopy); // this is the copy constructor

	bool initParameters(AudioProcessorValueTreeState& parameters, CVASTEffect* effect);
	bool initCompatibilityParameters(AudioProcessorValueTreeState& parameters, CVASTEffect* effect);
	bool initCompatibilityParameters2(AudioProcessorValueTreeState& parameters, CVASTEffect* effect);
	bool initCompatibilityParameters3(AudioProcessorValueTreeState& parameters, CVASTEffect* effect);
	bool initCompatibilityParameters4(AudioProcessorValueTreeState& parameters, CVASTEffect* effect);
	
	static float toggleButtonTextToValueFunction(const String& text);
	static String toggleButtonValueToTextFunction(float value);
	static float floatSliderTextToValueFunction(const String& text);
	static String floatSliderValueToTextFunction(float value);
	static float floatSliderTextToValueFunctionMaxIsOff(const String& text);
	static String floatSliderValueToTextFunctionMaxIsOff(float value);
	static float intSliderTextToValueFunction(const String& text);
	static String intSliderValueToTextFunction(float value);
	
	static String comboBoxValueToTextFunction_WAVE(float value);
	static float comboBoxTextToValueFunction_WAVE(const String& text);
	static String comboBoxValueToTextFunction_LFOWAVE(float value);
	static float comboBoxTextToValueFunction_LFOWAVE(const String& text);
	static String comboBoxValueToTextFunction_MODMATSRCE(float value);
	static float comboBoxTextToValueFunction_MODMATSRCE(const String& text);
	static String comboBoxValueToTextFunction_MODMATDEST(float value);
	static float comboBoxTextToValueFunction_MODMATDEST(const String& text);
	static String comboBoxValueToTextFunction_OSCROUTE(float value);
	static float comboBoxTextToValueFunction_OSCROUTE(const String& text);
	static String comboBoxValueToTextFunction_FILTER1ROUTE(float value);
	static float comboBoxTextToValueFunction_FILTER1ROUTE(const String& text);
	static String comboBoxValueToTextFunction_FILTER2ROUTE(float value);
	static float comboBoxTextToValueFunction_FILTER2ROUTE(const String& text);
	static String comboBoxValueToTextFunction_FILTER3ROUTE(float value);
	static float comboBoxTextToValueFunction_FILTER3ROUTE(const String& text);
	static String comboBoxValueToTextFunction_FXBUSROUTE(float value);
	static float comboBoxTextToValueFunction_FXBUSROUTE(const String& text);
	static String comboBoxValueToTextFunction_ARPMODE(float value);
	static float comboBoxTextToValueFunction_ARPMODE(const String& text);
	static String comboBoxValueToTextFunction_NOISETYPE(float value);
	static float comboBoxTextToValueFunction_NOISETYPE(const String& text);
	static String comboBoxValueToTextFunction_MSEGENV(float value);
	static float comboBoxTextToValueFunction_MSEGENV(const String& text);
	static String comboBoxValueToTextFunction_MSEGLFOENV(float value);
	static float comboBoxTextToValueFunction_MSEGLFOENV(const String& text);
	static String comboBoxValueToTextFunction_FILTERTYPE(float value);
	static float comboBoxTextToValueFunction_FILTERTYPE(const String& text);
	static String comboBoxValueToTextFunction_POLARITY(float value);
	static float comboBoxTextToValueFunction_POLARITY(const String& text);
	static String comboBoxValueToTextFunction_POLYMODE(float value);
	static float comboBoxTextToValueFunction_POLYMODE(const String& text);
	static String comboBoxValueToTextFunction_SWITCH(float value);
	static float comboBoxTextToValueFunction_SWITCH(const String& text);
	static String comboBoxValueToTextFunction_MIDINOTES(float value);
	static float comboBoxTextToValueFunction_MIDINOTES(const String& text);
	static String comboBoxValueToTextFunction_TIMEBEATS(float value);
	static float comboBoxTextToValueFunction_TIMEBEATS(const String& text);
	static String comboBoxValueToTextFunction_WTFX(float value);
	static float comboBoxTextToValueFunction_WTFX(const String& text);
// --------------------------------------------------------------------------------------------
	
	std::atomic<float>* m_fMasterVolumedB = nullptr;
	std::atomic<float>* m_fPortamento = nullptr;
	std::atomic<float>* m_fMasterTune = nullptr;
	std::atomic<float>* m_fWheelSensitivity = nullptr; //obsolete
	std::atomic<float>* m_fPitchMod = nullptr;
	std::atomic<float>* m_fVoiceGain = nullptr;
	std::atomic<float>* m_uPolyMode = nullptr; //dont use directly - only use m_Set.m_uMaxPoly
	std::atomic<float>* m_bLegatoMode = nullptr;
	std::atomic<float>* m_fOscDetune_OscA = nullptr;
	std::atomic<float>* m_fOscGain_OscA = nullptr;
	std::atomic<float>* m_fOscCents_OscA = nullptr;
	std::atomic<float>* m_fOscWTPos_OscA = nullptr;
	std::atomic<float>* m_fOscMorph_OscA = nullptr;
	std::atomic<float>* m_fSpread_OscA = nullptr;
	std::atomic<float>* m_iNumOscs_OscA = nullptr;
	std::atomic<float>* m_iOscOct_OscA = nullptr;
	std::atomic<float>* m_fOscPan_OscA = nullptr;
	std::atomic<float>* m_fOscPhase_OscA = nullptr;
	std::atomic<float>* m_bExciterOnOff_OscA = nullptr;
	std::atomic<float>* m_bOscInverterOnOff_OscA = nullptr;
	std::atomic<float>* m_bOscRetrigOnOff_OscA = nullptr;
	std::atomic<float>* m_bOscOnOff_OscA = nullptr;
	std::atomic<float>* m_fOscDetune_OscB = nullptr;
	std::atomic<float>* m_fOscGain_OscB = nullptr;
	std::atomic<float>* m_fOscCents_OscB = nullptr;
	std::atomic<float>* m_fOscWTPos_OscB = nullptr;
	std::atomic<float>* m_fOscMorph_OscB = nullptr;
	std::atomic<float>* m_fSpread_OscB = nullptr;
	std::atomic<float>* m_iNumOscs_OscB = nullptr;
	std::atomic<float>* m_iOscOct_OscB = nullptr;
	std::atomic<float>* m_fOscPan_OscB = nullptr;
	std::atomic<float>* m_fOscPhase_OscB = nullptr;	
	std::atomic<float>* m_bExciterOnOff_OscB = nullptr;
	std::atomic<float>* m_bOscInverterOnOff_OscB = nullptr;
	std::atomic<float>* m_bOscRetrigOnOff_OscB = nullptr;
	std::atomic<float>* m_bOscOnOff_OscB = nullptr;
	std::atomic<float>* m_fOscDetune_OscC = nullptr;
	std::atomic<float>* m_fOscGain_OscC = nullptr;
	std::atomic<float>* m_fOscCents_OscC = nullptr;
	std::atomic<float>* m_fOscWTPos_OscC = nullptr;
	std::atomic<float>* m_fOscMorph_OscC = nullptr;
	std::atomic<float>* m_fSpread_OscC = nullptr;
	std::atomic<float>* m_iNumOscs_OscC = nullptr;
	std::atomic<float>* m_iOscOct_OscC = nullptr;
	std::atomic<float>* m_fOscPan_OscC = nullptr;
	std::atomic<float>* m_fOscPhase_OscC = nullptr;	
	std::atomic<float>* m_bExciterOnOff_OscC = nullptr;
	std::atomic<float>* m_bOscInverterOnOff_OscC = nullptr;
	std::atomic<float>* m_bOscRetrigOnOff_OscC = nullptr;
	std::atomic<float>* m_bOscOnOff_OscC = nullptr;
	std::atomic<float>* m_fOscDetune_OscD = nullptr;
	std::atomic<float>* m_fOscGain_OscD = nullptr;
	std::atomic<float>* m_fOscCents_OscD = nullptr;
	std::atomic<float>* m_fOscWTPos_OscD = nullptr;
	std::atomic<float>* m_fOscMorph_OscD = nullptr;
	std::atomic<float>* m_fSpread_OscD = nullptr;
	std::atomic<float>* m_iNumOscs_OscD = nullptr;
	std::atomic<float>* m_iOscOct_OscD = nullptr;
	std::atomic<float>* m_fOscPan_OscD = nullptr;
	std::atomic<float>* m_fOscPhase_OscD = nullptr;
	std::atomic<float>* m_bExciterOnOff_OscD = nullptr;
	std::atomic<float>* m_bOscInverterOnOff_OscD = nullptr;
	std::atomic<float>* m_bOscRetrigOnOff_OscD = nullptr;
	std::atomic<float>* m_bOscOnOff_OscD = nullptr;
	
	std::atomic<float>* m_uVCAEnv_OscA = nullptr;
	std::atomic<float>* m_uVCAEnv_OscB = nullptr;
	std::atomic<float>* m_uVCAEnv_OscC = nullptr;
	std::atomic<float>* m_uVCAEnv_OscD = nullptr;

	std::atomic<float>* m_uWTFX_OscA = nullptr;
	std::atomic<float>* m_uWTFX_OscB = nullptr;
	std::atomic<float>* m_uWTFX_OscC = nullptr;
	std::atomic<float>* m_uWTFX_OscD = nullptr;
	std::atomic<float>* m_fWTFXVal_OscA = nullptr;
	std::atomic<float>* m_fWTFXVal_OscB = nullptr;
	std::atomic<float>* m_fWTFXVal_OscC = nullptr;
	std::atomic<float>* m_fWTFXVal_OscD = nullptr;
	
	std::atomic<float>* m_uOscRouting1_OscA = nullptr;
	std::atomic<float>* m_uOscRouting2_OscA = nullptr;
	std::atomic<float>* m_uOscRouting1_OscB = nullptr;
	std::atomic<float>* m_uOscRouting2_OscB = nullptr;
	std::atomic<float>* m_uOscRouting1_OscC = nullptr;
	std::atomic<float>* m_uOscRouting2_OscC = nullptr;
	std::atomic<float>* m_uOscRouting1_OscD = nullptr;
	std::atomic<float>* m_uOscRouting2_OscD = nullptr;
	std::atomic<float>* m_uNoiseRouting1 = nullptr;
	std::atomic<float>* m_uNoiseRouting2 = nullptr;
	
	std::atomic<float>* m_bOnOff_Filter1 = nullptr;
	std::atomic<float>* m_bOnOff_Filter2 = nullptr;
	std::atomic<float>* m_bOnOff_Filter3 = nullptr;
	std::atomic<float>* m_uFilterRouting_Filter1 = nullptr;
	std::atomic<float>* m_uFilterRouting_Filter2 = nullptr;
	std::atomic<float>* m_uFilterRouting_Filter3 = nullptr;
	std::atomic<float>* m_uFilterRouting2_Filter1 = nullptr;
	std::atomic<float>* m_uFilterRouting2_Filter2 = nullptr;
	std::atomic<float>* m_uFilterRouting2_Filter3 = nullptr;
	std::atomic<float>* m_uFilterType_Filter1 = nullptr;
	std::atomic<float>* m_uFilterType_Filter2 = nullptr;
	std::atomic<float>* m_uFilterType_Filter3 = nullptr;
	std::atomic<float>* m_uVCFEnv_Filter1 = nullptr;
	std::atomic<float>* m_uVCFEnv_Filter2 = nullptr;
	std::atomic<float>* m_uVCFEnv_Filter3 = nullptr;
	std::atomic<float>* m_fVCFEnvMod_Filter1 = nullptr;
	std::atomic<float>* m_fVCFEnvMod_Filter2 = nullptr;
	std::atomic<float>* m_fVCFEnvMod_Filter3 = nullptr;
	std::atomic<float>* m_fFilterCutoff_Filter1 = nullptr;
	std::atomic<float>* m_fFilterCutoff_Filter2 = nullptr;
	std::atomic<float>* m_fFilterCutoff_Filter3 = nullptr;
	std::atomic<float>* m_fFilterReso_Filter1 = nullptr;
	std::atomic<float>* m_fFilterReso_Filter2 = nullptr;
	std::atomic<float>* m_fFilterReso_Filter3 = nullptr;
	std::atomic<float>* m_fFilterDryWet_Filter1 = nullptr;
	std::atomic<float>* m_fFilterDryWet_Filter2 = nullptr;
	std::atomic<float>* m_fFilterDryWet_Filter3 = nullptr;
	std::atomic<float>* m_fFilterDrive_Filter1 = nullptr;
	std::atomic<float>* m_fFilterDrive_Filter2 = nullptr;
	std::atomic<float>* m_fFilterDrive_Filter3 = nullptr;
	std::atomic<float>* m_fFilterScale_Filter1 = nullptr;
	std::atomic<float>* m_fFilterScale_Filter2 = nullptr;
	std::atomic<float>* m_fFilterScale_Filter3 = nullptr;
	std::atomic<float>* m_fFilterGain_Filter1 = nullptr;
	std::atomic<float>* m_fFilterGain_Filter2 = nullptr;
	std::atomic<float>* m_fFilterGain_Filter3 = nullptr;

	std::atomic<float>* m_uLFOWave_LFO1 = nullptr; 
	std::atomic<float>* m_uLFOWave_LFO2 = nullptr;
	std::atomic<float>* m_uLFOWave_LFO3 = nullptr;
	std::atomic<float>* m_uLFOWave_LFO4 = nullptr;
	std::atomic<float>* m_uLFOWave_LFO5 = nullptr;
	std::atomic<float>* m_fLFOFreq_LFO1 = nullptr;
	std::atomic<float>* m_fLFOFreq_LFO2 = nullptr;
	std::atomic<float>* m_fLFOFreq_LFO3 = nullptr;
	std::atomic<float>* m_fLFOFreq_LFO4 = nullptr;
	std::atomic<float>* m_fLFOFreq_LFO5 = nullptr;
	std::atomic<float>* m_bLFOSynch_LFO1 = nullptr; 
	std::atomic<float>* m_bLFOSynch_LFO2 = nullptr;
	std::atomic<float>* m_bLFOSynch_LFO3 = nullptr;
	std::atomic<float>* m_bLFOSynch_LFO4 = nullptr;
	std::atomic<float>* m_bLFOSynch_LFO5 = nullptr;
	std::atomic<float>* m_bLFORetrigOnOff_LFO1 = nullptr;
	std::atomic<float>* m_bLFORetrigOnOff_LFO2 = nullptr;
	std::atomic<float>* m_bLFORetrigOnOff_LFO3 = nullptr;
	std::atomic<float>* m_bLFORetrigOnOff_LFO4 = nullptr;
	std::atomic<float>* m_bLFORetrigOnOff_LFO5 = nullptr;
	std::atomic<float>* m_bLFOPerVoice_LFO1 = nullptr;
	std::atomic<float>* m_bLFOPerVoice_LFO2 = nullptr;
	std::atomic<float>* m_bLFOPerVoice_LFO3 = nullptr;
	std::atomic<float>* m_bLFOPerVoice_LFO4 = nullptr;
	std::atomic<float>* m_bLFOPerVoice_LFO5 = nullptr;
	std::atomic<float>* m_uLFOPolarity_LFO1 = nullptr;
	std::atomic<float>* m_uLFOPolarity_LFO2 = nullptr;
	std::atomic<float>* m_uLFOPolarity_LFO3 = nullptr;
	std::atomic<float>* m_uLFOPolarity_LFO4 = nullptr;
	std::atomic<float>* m_uLFOPolarity_LFO5 = nullptr;
	std::atomic<float>* m_fLFORamp_LFO1 = nullptr;
	std::atomic<float>* m_fLFORamp_LFO2 = nullptr;
	std::atomic<float>* m_fLFORamp_LFO3 = nullptr;
	std::atomic<float>* m_fLFORamp_LFO4 = nullptr;
	std::atomic<float>* m_fLFORamp_LFO5 = nullptr;
	std::atomic<float>* m_fLFOPhase_LFO1 = nullptr;
	std::atomic<float>* m_fLFOPhase_LFO2 = nullptr;
	std::atomic<float>* m_fLFOPhase_LFO3 = nullptr;
	std::atomic<float>* m_fLFOPhase_LFO4 = nullptr;
	std::atomic<float>* m_fLFOPhase_LFO5 = nullptr;
	std::atomic<float>* m_uLFOTimeBeats_LFO1 = nullptr;
	std::atomic<float>* m_uLFOTimeBeats_LFO2 = nullptr;
	std::atomic<float>* m_uLFOTimeBeats_LFO3 = nullptr;
	std::atomic<float>* m_uLFOTimeBeats_LFO4 = nullptr;	
	std::atomic<float>* m_uLFOTimeBeats_LFO5 = nullptr;
	std::atomic<float>* m_uLFOMSEG_LFO1 = nullptr;
	std::atomic<float>* m_uLFOMSEG_LFO2 = nullptr;
	std::atomic<float>* m_uLFOMSEG_LFO3 = nullptr;
	std::atomic<float>* m_uLFOMSEG_LFO4 = nullptr;
	std::atomic<float>* m_uLFOMSEG_LFO5 = nullptr;
	
	std::atomic<float>* m_bMSEGPerVoice_MSEG1 = nullptr;
	std::atomic<float>* m_bMSEGSynch_MSEG1 = nullptr;
	std::atomic<float>* m_uMSEGTimeBeats_MSEG1 = nullptr;
	std::atomic<float>* m_fAttackSteps_MSEG1 = nullptr;
	std::atomic<float>* m_fDecaySteps_MSEG1 = nullptr;
	std::atomic<float>* m_fReleaseSteps_MSEG1 = nullptr;
	std::atomic<float>* m_bMSEGPerVoice_MSEG2 = nullptr;
	std::atomic<float>* m_bMSEGSynch_MSEG2 = nullptr;
	std::atomic<float>* m_uMSEGTimeBeats_MSEG2 = nullptr;
	std::atomic<float>* m_fAttackSteps_MSEG2 = nullptr;
	std::atomic<float>* m_fDecaySteps_MSEG2 = nullptr;
	std::atomic<float>* m_fReleaseSteps_MSEG2 = nullptr;
	std::atomic<float>* m_bMSEGPerVoice_MSEG3 = nullptr;
	std::atomic<float>* m_bMSEGSynch_MSEG3 = nullptr;
	std::atomic<float>* m_uMSEGTimeBeats_MSEG3 = nullptr;
	std::atomic<float>* m_fAttackSteps_MSEG3 = nullptr;
	std::atomic<float>* m_fDecaySteps_MSEG3 = nullptr;
	std::atomic<float>* m_fReleaseSteps_MSEG3 = nullptr;
	std::atomic<float>* m_bMSEGPerVoice_MSEG4 = nullptr;
	std::atomic<float>* m_bMSEGSynch_MSEG4 = nullptr;
	std::atomic<float>* m_uMSEGTimeBeats_MSEG4 = nullptr;
	std::atomic<float>* m_fAttackSteps_MSEG4 = nullptr;
	std::atomic<float>* m_fDecaySteps_MSEG4 = nullptr;
	std::atomic<float>* m_fReleaseSteps_MSEG4 = nullptr;
	std::atomic<float>* m_bMSEGPerVoice_MSEG5 = nullptr;
	std::atomic<float>* m_bMSEGSynch_MSEG5 = nullptr;
	std::atomic<float>* m_uMSEGTimeBeats_MSEG5 = nullptr;
	std::atomic<float>* m_fAttackSteps_MSEG5 = nullptr;
	std::atomic<float>* m_fDecaySteps_MSEG5 = nullptr;
	std::atomic<float>* m_fReleaseSteps_MSEG5 = nullptr;

	std::atomic<float>* m_fAttackTime_MSEG1 = nullptr;
	std::atomic<float>* m_fAttackTime_MSEG2 = nullptr;
	std::atomic<float>* m_fAttackTime_MSEG3 = nullptr;
	std::atomic<float>* m_fAttackTime_MSEG4 = nullptr;
	std::atomic<float>* m_fAttackTime_MSEG5 = nullptr;
	std::atomic<float>* m_fDecayTime_MSEG1 = nullptr;
	std::atomic<float>* m_fDecayTime_MSEG2 = nullptr;
	std::atomic<float>* m_fDecayTime_MSEG3 = nullptr;
	std::atomic<float>* m_fDecayTime_MSEG4 = nullptr;
	std::atomic<float>* m_fDecayTime_MSEG5 = nullptr;
	std::atomic<float>* m_fSustainLevel_MSEG1 = nullptr;
	std::atomic<float>* m_fSustainLevel_MSEG2 = nullptr;
	std::atomic<float>* m_fSustainLevel_MSEG3 = nullptr;
	std::atomic<float>* m_fSustainLevel_MSEG4 = nullptr;
	std::atomic<float>* m_fSustainLevel_MSEG5 = nullptr;
	std::atomic<float>* m_fReleaseTime_MSEG1 = nullptr;
	std::atomic<float>* m_fReleaseTime_MSEG2 = nullptr;
	std::atomic<float>* m_fReleaseTime_MSEG3 = nullptr;
	std::atomic<float>* m_fReleaseTime_MSEG4 = nullptr;
	std::atomic<float>* m_fReleaseTime_MSEG5 = nullptr;
	std::atomic<float>* m_uMSEGPolarity_MSEG1 = nullptr;
	std::atomic<float>* m_uMSEGPolarity_MSEG2 = nullptr;
	std::atomic<float>* m_uMSEGPolarity_MSEG3 = nullptr;
	std::atomic<float>* m_uMSEGPolarity_MSEG4 = nullptr;
	std::atomic<float>* m_uMSEGPolarity_MSEG5 = nullptr;

	//Stepseq arp
	std::atomic<float>* m_fGlide_STEPSEQ1 = nullptr;
	std::atomic<float>* m_fGlide_STEPSEQ2 = nullptr;
	std::atomic<float>* m_fGlide_STEPSEQ3 = nullptr;
	std::atomic<float>* m_fGate_STEPSEQ1 = nullptr;
	std::atomic<float>* m_fGate_STEPSEQ2 = nullptr;
	std::atomic<float>* m_fGate_STEPSEQ3 = nullptr;
	std::atomic<float>* m_bStepSeqSynch_STEPSEQ1 = nullptr;
	std::atomic<float>* m_bStepSeqSynch_STEPSEQ2 = nullptr;
	std::atomic<float>* m_bStepSeqSynch_STEPSEQ3 = nullptr;
	std::atomic<float>* m_uStepSeqTimeBeats_STEPSEQ1 = nullptr;
	std::atomic<float>* m_uStepSeqTimeBeats_STEPSEQ2 = nullptr;
	std::atomic<float>* m_uStepSeqTimeBeats_STEPSEQ3 = nullptr;
	std::atomic<float>* m_fStepSeqSpeed_STEPSEQ1 = nullptr;
	std::atomic<float>* m_fStepSeqSpeed_STEPSEQ2 = nullptr;
	std::atomic<float>* m_fStepSeqSpeed_STEPSEQ3 = nullptr;

	std::atomic<float>* m_uModMatSrce2 = nullptr;
	std::atomic<float>* m_uModMatSrce3 = nullptr;
	std::atomic<float>* m_uModMatSrce4 = nullptr;
	std::atomic<float>* m_uModMatDest2 = nullptr;
	std::atomic<float>* m_uModMatDest3 = nullptr;
	std::atomic<float>* m_uModMatDest4 = nullptr;
	std::atomic<float>* m_uModMatSrce5 = nullptr;
	std::atomic<float>* m_uModMatDest5 = nullptr;
	std::atomic<float>* m_uModMatSrce1 = nullptr;
	std::atomic<float>* m_uModMatDest1 = nullptr;
	std::atomic<float>* m_uModMatSrce6 = nullptr;
	std::atomic<float>* m_uModMatDest6 = nullptr;
	std::atomic<float>* m_fModMatVal6 = nullptr;
	std::atomic<float>* m_fModMatVal5 = nullptr;
	std::atomic<float>* m_fModMatVal4 = nullptr;
	std::atomic<float>* m_fModMatVal3 = nullptr;
	std::atomic<float>* m_fModMatVal2 = nullptr;
	std::atomic<float>* m_fModMatVal1 = nullptr;
	std::atomic<float>* m_uModMatSrce7 = nullptr;
	std::atomic<float>* m_uModMatDest7 = nullptr;
	std::atomic<float>* m_uModMatSrce8 = nullptr;
	std::atomic<float>* m_uModMatDest8 = nullptr;
	std::atomic<float>* m_fModMatVal8 = nullptr;
	std::atomic<float>* m_fModMatVal7 = nullptr;
	std::atomic<float>* m_uModMatDest9 = nullptr;
	std::atomic<float>* m_uModMatDest10 = nullptr;
	std::atomic<float>* m_uModMatDest11 = nullptr;
	std::atomic<float>* m_uModMatDest12 = nullptr;
	std::atomic<float>* m_uModMatDest13 = nullptr;
	std::atomic<float>* m_uModMatDest14 = nullptr;
	std::atomic<float>* m_uModMatDest15 = nullptr;
	std::atomic<float>* m_uModMatDest16 = nullptr;
	std::atomic<float>* m_uModMatSrce9 = nullptr;
	std::atomic<float>* m_uModMatSrce10 = nullptr;
	std::atomic<float>* m_uModMatSrce11 = nullptr;
	std::atomic<float>* m_uModMatSrce12 = nullptr;
	std::atomic<float>* m_uModMatSrce13 = nullptr;
	std::atomic<float>* m_uModMatSrce14 = nullptr;
	std::atomic<float>* m_uModMatSrce15 = nullptr;
	std::atomic<float>* m_uModMatSrce16 = nullptr;
	std::atomic<float>* m_fModMatVal9 = nullptr;
	std::atomic<float>* m_fModMatVal10 = nullptr;
	std::atomic<float>* m_fModMatVal11 = nullptr;
	std::atomic<float>* m_fModMatVal12 = nullptr;
	std::atomic<float>* m_fModMatVal13 = nullptr;
	std::atomic<float>* m_fModMatVal14 = nullptr;
	std::atomic<float>* m_fModMatVal15 = nullptr;
	std::atomic<float>* m_fModMatVal16 = nullptr;
	std::atomic<float>* m_fModMatCurve1 = nullptr;
	std::atomic<float>* m_fModMatCurve2 = nullptr;
	std::atomic<float>* m_fModMatCurve3 = nullptr;
	std::atomic<float>* m_fModMatCurve4 = nullptr;
	std::atomic<float>* m_fModMatCurve5 = nullptr;
	std::atomic<float>* m_fModMatCurve6 = nullptr;
	std::atomic<float>* m_fModMatCurve7 = nullptr;
	std::atomic<float>* m_fModMatCurve8 = nullptr;
	std::atomic<float>* m_fModMatCurve9 = nullptr;
	std::atomic<float>* m_fModMatCurve10 = nullptr;
	std::atomic<float>* m_fModMatCurve11 = nullptr;
	std::atomic<float>* m_fModMatCurve12 = nullptr;
	std::atomic<float>* m_fModMatCurve13 = nullptr;
	std::atomic<float>* m_fModMatCurve14 = nullptr;
	std::atomic<float>* m_fModMatCurve15 = nullptr;
	std::atomic<float>* m_fModMatCurve16 = nullptr;
	std::atomic<float>* m_uModMatPolarity1 = nullptr;
	std::atomic<float>* m_uModMatPolarity2 = nullptr;
	std::atomic<float>* m_uModMatPolarity3 = nullptr;
	std::atomic<float>* m_uModMatPolarity4 = nullptr;
	std::atomic<float>* m_uModMatPolarity5 = nullptr;
	std::atomic<float>* m_uModMatPolarity6 = nullptr;
	std::atomic<float>* m_uModMatPolarity7 = nullptr;
	std::atomic<float>* m_uModMatPolarity8 = nullptr;
	std::atomic<float>* m_uModMatPolarity9 = nullptr;
	std::atomic<float>* m_uModMatPolarity10 = nullptr;
	std::atomic<float>* m_uModMatPolarity11 = nullptr;
	std::atomic<float>* m_uModMatPolarity12 = nullptr;
	std::atomic<float>* m_uModMatPolarity13 = nullptr;
	std::atomic<float>* m_uModMatPolarity14 = nullptr;
	std::atomic<float>* m_uModMatPolarity15 = nullptr;
	std::atomic<float>* m_uModMatPolarity16 = nullptr;

	std::atomic<float>* m_fMBMonoFrequency = nullptr;
	std::atomic<float>* m_fMBMonoFrequency_Bus2 = nullptr;
	std::atomic<float>* m_fMBMonoFrequency_Bus3 = nullptr;
	/*
	std::atomic<float>* m_bOversampleBus = nullptr;
	std::atomic<float>* m_bOversampleBus_Bus2 = nullptr;
	std::atomic<float>* m_bOversampleBus_Bus3 = nullptr;
	*/

	std::atomic<float>* m_uFxBusRouting = nullptr;
	std::atomic<float>* m_uFxBusRouting_Bus2 = nullptr;
	std::atomic<float>* m_uFxBusRouting_Bus3 = nullptr;

	std::atomic<float>* m_fARPSpeed = nullptr;
	std::atomic<float>* m_uARPMode = nullptr;
	std::atomic<float>* m_bARPOnOff = nullptr;
	std::atomic<float>* m_bARPSynch = nullptr;
	std::atomic<float>* m_uARPTimeBeats = nullptr;

	std::atomic<float>* m_bNoiseOnOff = nullptr;
	std::atomic<float>* m_fNoiseGain = nullptr;
	std::atomic<float>* m_fNoisePan = nullptr;
	std::atomic<float>* m_uNoiseType = nullptr;	
	std::atomic<float>* m_uVCAEnv_Noise = nullptr;
	std::atomic<float>* m_fNoiseLowCut = nullptr;
	std::atomic<float>* m_fNoiseHighCut = nullptr;
	std::atomic<float>* m_fNoiseResonance = nullptr;

	std::atomic<float>* m_bSubOnOff = nullptr;
	std::atomic<float>* m_uSubWave = nullptr;
	
	std::atomic<float>* m_bSamplerOnOff = nullptr;
	std::atomic<float>* m_fSamplerGain = nullptr; 
	std::atomic<float>* m_bSamplerKeytrack = nullptr;
	std::atomic<float>* m_uSamplerRootKey = nullptr;
	std::atomic<float>* m_fSamplerCents = nullptr;
	std::atomic<float>* m_bSamplerPoly = nullptr;
	std::atomic<float>* m_bSamplerLoop = nullptr;
	std::atomic<float>* m_bSamplerLoopOnly = nullptr;
	std::atomic<float>* m_fSamplerPan = nullptr;
	std::atomic<float>* m_bSamplerLoopPingpong = nullptr;
	std::atomic<float>* m_fSamplerLoopRange = nullptr;
	std::atomic<float>* m_uVCAEnv_Sampler = nullptr;
	std::atomic<float>* m_uSamplerRouting1 = nullptr;
	std::atomic<float>* m_uSamplerRouting2 = nullptr;
	std::atomic<float>* m_fSamplerGrainInteronset = nullptr;
	std::atomic<float>* m_fSamplerGrainRandom = nullptr;
	std::atomic<float>* m_fSamplerGrainShape = nullptr;

	std::atomic<float>* m_fCustomModulator1 = nullptr;
	std::atomic<float>* m_fCustomModulator2 = nullptr;
	std::atomic<float>* m_fCustomModulator3 = nullptr;
	std::atomic<float>* m_fCustomModulator4 = nullptr;

	std::atomic<float>* m_iWavWTPos = nullptr;
	std::atomic<float>* m_iWTEditorZoom = nullptr;
	std::atomic<float>* m_iWAVScale = nullptr;	

private:
	JUCE_LEAK_DETECTOR(CVASTParamState)
};
