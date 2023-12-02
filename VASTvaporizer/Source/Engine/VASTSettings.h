/*
VAST Dynamics Audio Software (TM)

Global settings for AUDIO THREAD!
*/

#pragma once

#include "VASTEngineHeader.h"
#include "VASTPluginConstants.h"
#include "Utils/VASTSynthfunctions.h"
#include "Utils/VASTFFTSingletonFactory.h"
#include "VASTParamState.h"
#include "VASTMSEGData.h"
#include "VASTARPData.h"
#include "../AnaMark-Tuning-Library/SCL_Import.h"
#include <string>
#include <vector>

//#include "Oscillator/VASTWaveTableGenerator.h"
//#include "VASTXperience.h"  must not circular include here!

class VASTAudioProcessor; //forward declaration
typedef struct sRoutingBuffers {
public:
	int m_numSamples = 0;
	int m_numSamplesReal = 0;
	int m_currentMaxSamplesInit = 0;
    std::atomic<bool> msegUsed[5] = { false, false, false, false, false };
	std::atomic<bool> lfoUsed[5] = { false, false, false };
    std::atomic<bool> stepSeqUsed[3] = { false, false, false };
	ReadWriteLock mReadWriteLock; // the central mutex that shall prevent reads during buffer resize

	//per voice
	OwnedArray<AudioSampleBuffer> MSEGBuffer[5]; //[mseg][voice]
	bool MSEGActiveBuffer[5][C_MAX_POLY][C_MAX_BUFFER_SIZE]; //[mseg][voice][C_MAX_BUFFER_SIZE]	
	//OwnedArray<std::vector<bool>> MSEGActiveBuffer[5]; //[mseg][voice]

	OwnedArray<AudioSampleBuffer> LFOBuffer[5]; //[lfo][voice] LFO 1,2,3,4,5
	OwnedArray<AudioSampleBuffer> OscVoices[4]; //A, B, C, D [voice]
	OwnedArray<AudioSampleBuffer> NoiseVoices; // [voice]
	OwnedArray<AudioSampleBuffer> SamplerVoices; // [voice]
	OwnedArray<AudioSampleBuffer> MidiNoteBuffer; //[voice]
	OwnedArray<AudioSampleBuffer> VelocityBuffer; //[voice]
	OwnedArray<AudioSampleBuffer> FilterVoices[3]; // Filter 1..3 [voice]
	
	//global
	std::unique_ptr<AudioSampleBuffer> OscBuffer[4]; //A, B, C, D 
	std::unique_ptr<AudioSampleBuffer> NoiseBuffer;
	std::unique_ptr<AudioSampleBuffer> SamplerBuffer;
	std::unique_ptr<AudioSampleBuffer> FilterBuffer[3]; //Filter 1,2,3
	std::unique_ptr<AudioSampleBuffer> FxBusBuffer[3]; //FxBus 1,2,3
	std::unique_ptr<AudioSampleBuffer> MasterOutBuffer;
	std::unique_ptr<AudioSampleBuffer> LFOGlobalBuffer[5];
	std::unique_ptr<AudioSampleBuffer> StepSeqBuffer[3];
	std::unique_ptr<AudioSampleBuffer> CustomModulatorBuffer[4];
	
	// the buffers
	std::unique_ptr<AudioSampleBuffer> fAudioInputBuffer;
	std::unique_ptr<AudioSampleBuffer> fInputEnvelopeBuffer;

	~sRoutingBuffers() {
		OscBuffer[0] = nullptr; OscBuffer[1] = nullptr; OscBuffer[2] = nullptr; OscBuffer[3] = nullptr;
		NoiseBuffer = nullptr;
		SamplerBuffer = nullptr;
		FilterBuffer[0] = nullptr; FilterBuffer[1] = nullptr; FilterBuffer[2] = nullptr;
		FxBusBuffer[0] = nullptr; FxBusBuffer[1] = nullptr; FxBusBuffer[2] = nullptr;
		MasterOutBuffer = nullptr;
		StepSeqBuffer[0] = nullptr; StepSeqBuffer[1] = nullptr; StepSeqBuffer[2] = nullptr;
		LFOGlobalBuffer[0] = nullptr; LFOGlobalBuffer[1] = nullptr; LFOGlobalBuffer[2] = nullptr; LFOGlobalBuffer[3] = nullptr; LFOGlobalBuffer[4] = nullptr;
		CustomModulatorBuffer[0] = nullptr; CustomModulatorBuffer[1] = nullptr; CustomModulatorBuffer[2] = nullptr; CustomModulatorBuffer[3] = nullptr;
		fAudioInputBuffer = nullptr; fInputEnvelopeBuffer = nullptr;
	}

	void init() {
		const ScopedWriteLock myScopedLock(mReadWriteLock);
		int initSize = 16;

		//per voice
		for (int mseg = 0; mseg < 5; mseg++) {
			OwnedArray<AudioSampleBuffer> msegVoices;
			for (int voice = 0; voice < C_MAX_POLY; voice++) {
				MSEGBuffer[mseg].add(new AudioSampleBuffer(1, initSize));
				std::fill(MSEGActiveBuffer[mseg][voice], MSEGActiveBuffer[mseg][voice] + initSize, false);
			}
		}

		for (int lfo = 0; lfo < 5; lfo++) {
			OwnedArray<AudioSampleBuffer> lfoVoices;
			for (int voice = 0; voice < C_MAX_POLY; voice++) {
				LFOBuffer[lfo].add(new AudioSampleBuffer(1, initSize));
			}
		}

		for (int bank = 0; bank < 4; bank++) {
			for (int voice = 0; voice < C_MAX_POLY; voice++) {
				OscVoices[bank].add(new AudioSampleBuffer(2, initSize));
			}
		}

		for (int voice = 0; voice < C_MAX_POLY; voice++) {
			SamplerVoices.add(new AudioSampleBuffer(2, initSize));
		}

		for (int voice = 0; voice < C_MAX_POLY; voice++) {
			NoiseVoices.add(new AudioSampleBuffer(2, initSize));
		}

		for (int voice = 0; voice < C_MAX_POLY; voice++) {
			MidiNoteBuffer.add(new AudioSampleBuffer(1, initSize));
		}

		for (int voice = 0; voice < C_MAX_POLY; voice++) {
			VelocityBuffer.add(new AudioSampleBuffer(1, initSize));
		}

		for (int filter = 0; filter < 3; filter++) {
			for (int voice = 0; voice < C_MAX_POLY; voice++) {
				FilterVoices[filter].add(new AudioSampleBuffer(2, initSize));
			}
		}

		//stereos
		for (int bank = 0; bank < 4; bank++) {
			OscBuffer[bank].reset(new AudioSampleBuffer(2, initSize));
		}

		NoiseBuffer.reset(new AudioSampleBuffer(2, initSize));
		SamplerBuffer.reset(new AudioSampleBuffer(2, initSize));
        
		for (int filter = 0; filter < 3; filter++) {
            FilterBuffer[filter].reset(new AudioSampleBuffer(2, initSize));
		}

		for (int fxbus = 0; fxbus < 3; fxbus++) {
			FxBusBuffer[fxbus].reset(new AudioSampleBuffer(2, initSize));
		}

		MasterOutBuffer.reset(new AudioSampleBuffer(2, initSize));

		//LFO global
		for (int lfo = 0; lfo < 5; lfo++) {
			LFOGlobalBuffer[lfo].reset(new AudioSampleBuffer(1, initSize));
		}

		//stepseq
		for (int stepseq = 0; stepseq < 3; stepseq++) {
			StepSeqBuffer[stepseq].reset(new AudioSampleBuffer(1, initSize));
		}

		//custom modulators
		for (int custmod = 0; custmod < 4; custmod++) {
			CustomModulatorBuffer[custmod].reset(new AudioSampleBuffer(1, initSize));
		}

		fAudioInputBuffer.reset(new AudioSampleBuffer(1, initSize));
		fInputEnvelopeBuffer.reset(new AudioSampleBuffer(1, initSize));

		//CircularFilterOutput = new AudioSampleBuffer(1, m_circularFilterOutputLen);
		//CircularFilterOutput->clear();
	}

	void resize(int numSamples, bool dontFreeMem) { //dontFreeMem false means 20% additional no-op time
		const ScopedWriteLock myScopedLock(mReadWriteLock);

		bool doResize = false;
		if (numSamples > m_numSamplesReal) {
			m_numSamplesReal = numSamples; // dynamically expand buffers to new maximum
			doResize = true;
		}
		m_numSamples = numSamples;		

		//per voice
		for (int mseg = 0; mseg < 5; mseg++) {
			for (int voice = 0; voice < C_MAX_POLY; voice++) {
				if (doResize)
					MSEGBuffer[mseg][voice]->setSize(1, numSamples, false, false, dontFreeMem);
				MSEGBuffer[mseg][voice]->clear(0, numSamples); //needs clearing? safety		
				std::fill(MSEGActiveBuffer[mseg][voice], MSEGActiveBuffer[mseg][voice] + numSamples, false);
			}
		}

		if (doResize) {
			for (int lfo = 0; lfo < 5; lfo++) {
				for (int voice = 0; voice < C_MAX_POLY; voice++) {
					LFOBuffer[lfo][voice]->setSize(1, numSamples, false, false, dontFreeMem);
				}
			}
		}
		for (int bank = 0; bank < 4; bank++) {
			for (int voice = 0; voice < C_MAX_POLY; voice++) {				
				if (doResize)
					OscVoices[bank][voice]->setSize(2, numSamples, false, false, dontFreeMem);
				OscVoices[bank][voice]->clear(0, numSamples); //needs clearing
			}
		}
		for (int voice = 0; voice < C_MAX_POLY; voice++) {
			if (doResize)
				SamplerVoices[voice]->setSize(2, numSamples, false, false, dontFreeMem);
			SamplerVoices[voice]->clear(0, numSamples); //needs clearing
			if (doResize)
				NoiseVoices[voice]->setSize(2, numSamples, false, false, dontFreeMem);
			NoiseVoices[voice]->clear(0, numSamples); //needs clearing
		}
		

		for (int voice = 0; voice < C_MAX_POLY; voice++) {
			if (doResize) {
				MidiNoteBuffer[voice]->setSize(1, numSamples, false, false, dontFreeMem);
				VelocityBuffer[voice]->setSize(1, numSamples, false, false, dontFreeMem);
			}
			MidiNoteBuffer[voice]->clear(0, numSamples); //needs clearing
			VelocityBuffer[voice]->clear(0, numSamples); //needs clearing
		}

		for (int filter = 0; filter < 3; filter++) {
			for (int voice = 0; voice < C_MAX_POLY; voice++) {
				if (doResize)
					FilterVoices[filter][voice]->setSize(2, numSamples, false, false, dontFreeMem);
				FilterVoices[filter][voice]->clear(0, numSamples);; //needs clearing? safety
			}
		}

		//stereos
		for (int bank = 0; bank < 4; bank++) {
			if (doResize)
				OscBuffer[bank]->setSize(2, numSamples, false, false, dontFreeMem);
			OscBuffer[bank]->clear(0, numSamples); //needs clearing
		}
		
		for (int filter = 0; filter < 3; filter++) {
			if (doResize)
				FilterBuffer[filter]->setSize(2, numSamples, false, false, dontFreeMem);
			FilterBuffer[filter]->clear(0, numSamples); //needs clearing
		}

		for (int fxbus = 0; fxbus < 3; fxbus++) {
			if (doResize)
				FxBusBuffer[fxbus]->setSize(2, numSamples, false, false, dontFreeMem);
			FxBusBuffer[fxbus]->clear(0, numSamples); //needs clearing
		}

		if (doResize) {
			MasterOutBuffer->setSize(2, numSamples, false, false, dontFreeMem);
			NoiseBuffer->setSize(2, numSamples, false, false, dontFreeMem);
			SamplerBuffer->setSize(2, numSamples, false, false, dontFreeMem);
			fAudioInputBuffer->setSize(1, numSamples, false, false, dontFreeMem);
			fInputEnvelopeBuffer->setSize(1, numSamples, false, false, dontFreeMem);
			//monos
			for (int lfo = 0; lfo < 5; lfo++) {
				LFOGlobalBuffer[lfo]->setSize(1, numSamples, false, false, dontFreeMem);
			}
			for (int stepseq = 0; stepseq < 3; stepseq++) {
				StepSeqBuffer[stepseq]->setSize(1, numSamples, false, false, dontFreeMem);
			}
			for (int custmod = 0; custmod < 4; custmod++) {
				CustomModulatorBuffer[custmod]->setSize(1, numSamples, false, false, dontFreeMem);
			}
		}
		MasterOutBuffer->clear(0, numSamples); //needs clearing
		NoiseBuffer->clear(0, numSamples); //needs clearing
		SamplerBuffer->clear(0, numSamples);  //needs clearing
	}

	int getNumSamples() const {
		return m_numSamples;
	};

	/*
	-InputBuffer
	- OscABuffer - clear(Output Combobox 1 & 2) (Filter1, Filter2, Filter3, Bus1, Bus2, Bus3, Master)
	- OscBBuffer - clear(Output Combobox 1 & 2) (Filter1, Filter2, Filter3, Bus1, Bus2, Bus3, Master)
	- OscCBuffer - clear(Output Combobox 1 & 2) (Filter1, Filter2, Filter3, Bus1, Bus2, Bus3, Master)
	- OscDBuffer - clear(Output Combobox 1 & 2) (Filter1, Filter2, Filter3, Bus1, Bus2, Bus3, Master)
	- NoiseBuffer - clear(Output Combobox 1 & 2) (Filter1, Filter2, Filter3, Bus1, Bus2, Bus3, Master)
	- SampleBuffer - clear(Output Combobox 1 & 2) (Filter1, Filter2, Filter3, Bus1, Bus2, Bus3, Master)
	- Filter1Buffer - clear(Output Combobox) (Filter2, Filter3, Bus1, Bus2, Bus3, Master) \96 In : A, B, C, D, Noise, Sample
	- Filter2Buffer - clear(Output Combobox) (Filter3, Bus1, Bus2, Bus3, Master) \96 In : A, B, C, D, Noise, Sample, Filter2
	- Filter3Buffer - clear(Output Combobox) (Bus1, Bus2, Bus3, Master) \96 In : A, B, C, D, Noise, Sample, Filter1, Filter2
	- FxBus1Buffer - clear(Output Combobox) (Bus2, Bus3, Master) \96 In : A, B, C, D, Noise, Sample, Filter1, Filter2, Filter3
	- FxBus2Buffer - clear(Output Combobox) (Bus 3, Master) \96 In : A, B, C, D, Noise, Sample, Filter1, Filter2, Filter3, Bus1
	- FxBus3Buffer - clear(Output Combobox) (Master) \96 In : A, B, C, D, Noise, Sample, Filter1, Filter2, Filter3, Bus1, Bus2
	- MasterOutBuffer - clear - In : A, B, C, D, Noise, Sample, Filter1, Filter2, Filter3, Bus1, Bus2, Bus3
	- MSEG1Buffer - clear
	- MSEG2Buffer - clear
	- MSEG3Buffer - clear
	- MSEG4Buffer - clear
	- MSEG5Buffer - clear
	- LFO1Buffer - clear
	- LFO2Buffer - clear
	- LFO3Buffer - clear
	- LFO4Buffer - clear
	- LFO5Buffer - clear
	*/

} sRoutingBuffers;;

typedef struct {
//	uint64_t voiceNo; //to avoid error C2338
//	uint64_t currentFrame; //to avoid error C2338
	int voiceNo; //to avoid error C2338
	int currentFrame; //to avoid error C2338
} modMatrixInputState;

typedef struct {
	int length;
	String fullpathname;
	String wtname;
	float WTBuffer[2048][2048];
} wavFile;

typedef struct {
	std::atomic<float>* value;
	std::atomic<float>* curvy;
	std::atomic<float>* source;
	std::atomic<float>* destination;
	std::atomic<float>* polarity;
} modMatrixValueLookup;

class alignas(16) CVASTSettings
{
public:
	CVASTSettings(VASTAudioProcessor* processor);
	
	VASTAudioProcessor* my_processor;

	virtual ~CVASTSettings(void);

	void sendStatusMessage(StringRef pText);
	void sendStatusMessage(StringRef pText, int iDisp);
	void sendStatusMessage(StringRef pText, int iDisp1, int iDisp2);
	void sendStatusMessage(StringRef pText, int iDisp1, int iDisp2, int iDisp3);

	int modMatrixFindFirstFreeSlot();
	bool modMatrixDestinationSetFast(MYUINT destination);
	void modMatrixRemoveAllForDestination(MYUINT destination);
	void modMatrixRemoveAll();
	void modMatrixRemoveSlot(int slot);
	bool modMatrixSourceSetFast(MYUINT destination);
	bool modMatrixIsSet(MYUINT source, MYUINT destination); //fast enough
	void modMatrixSwapSlots(int slot1, int slot2);

	float driftNoiseSlow(int slot);
	float driftNoiseFast(int slot);

	void initModMatrix();
	void initModMatrixValueLookupTable(int slot);
	int modMatrixGetFirstSlotWithDestination(MYUINT destination);
	float getParameterValueWithMatrixModulation(std::atomic<float> *paramVal, MYUINT destination, modMatrixInputState* matrixInputState, bool *isModulating = nullptr);
	void modMatrixSlotGetValues(int slot, float &value, double &curvy, int &source, int &destination, int &polarity, float(&lastSrceVals)[C_MAX_POLY]);
	void modMatrixSlotGetValues(int slot, float &value, double &curvy, int &polarity, float(&lastSrceVals)[C_MAX_POLY]);
	void modMatrixSlotGetValuesWithMod(int slot, float &slotValue, float &modValue, double &curvy, int &source, int &destination, int &polarity, modMatrixInputState* matrixInputState);

	//float modMatrixGet(MYUINT source, MYUINT destination, modMatrixInputState* matrixInputState, float& curvy);
	//float get0To1ModMatrixMultiplier(MYUINT destination, modMatrixInputState* matrixInputState);
	//void doModMatrixModification(float *origVal, float minVal, float maxVal, MYUINT destination, modMatrixInputState* matrixInputState);

	float midiNoteGetBaseFreq(MYUINT uMIDINote, float oscMasterTune);

	double getMillisecondsPerBeat();
	double getIntervalRatio(int beatindex);
	double getIntervalTimeFromDAWBeats(int beatindex);

	int64 m_cUnderruns = 0; 

	void modMatrixCalcBuffers();
	
	void processEnvelope(int inFramesToProcess);
	float mInputEnvelope = 0.0;

	int _gettimeofday(struct timeval *tv); //for note stealing
	
	// Global 
	std::atomic<int> m_nSampleRate = 44100; //standard
    std::atomic<int> m_nExpectedSamplesPerBlock = 0;
    std::atomic<float> m_fMasterTune = 440; //Kammerton a

	//int m_nOversamplingRatio = 4;
	//int m_nSampleRateOversampled = m_nSampleRate * m_nOversamplingRatio;
	
	sRoutingBuffers m_RoutingBuffers;

	//Pitchbend
	float m_fPitchBend = 1.0f; //middle
	bool m_bPitchBendCenter = true;
	int m_iBendRange = 1; //up and down in semitones

	int m_uMaxPoly = C_MAX_POLY;

	//MPE Brightness
	MYUINT iMPETimbre[C_MAX_POLY];
	MYUINT iMPETimbreMidiNote[C_MAX_POLY];

	//Aftertouch	
	MYUINT iAftertouch[C_MAX_POLY];
	MYUINT iAftertouchMidiNote[C_MAX_POLY];

	float m_fMasterVolume = 1.f;
	LinearSmoothedValue<float> m_fMasterVolume_smoothed;	

    std::atomic<double> m_dPpqPosition = 0;
    std::atomic<double> m_dPpqBpm = 0;
    std::atomic<bool> m_bPpqIsPlaying = false;
    std::atomic<bool> m_bPpqIsLooping = false;
    std::atomic<double> m_dPpqPositionOfLastBarStart = 0;
	
	//Mod Wheel
    std::atomic<MYUINT> m_uModWheel = 0;

	std::shared_ptr<CVASTParamState> m_State;	  //the active state
	
	VASTMSEGData m_MSEGData[5];
	VASTMSEGData m_MSEGData_changed[5];
	VASTMSEGData m_StepSeqData[3];
	VASTMSEGData m_StepSeqData_changed[3];
	VASTMSEGData m_MSEGCopyBuffer;
	VASTARPData m_ARPData; 
	VASTARPData m_ARPData_changed;
	
	std::atomic<modMatrixInputState> bufferInputState;

	bool modMatrixDestSet[M_MODMATRIX_MAX_DESTINATIONS];
    bool modMatrixSrceSet[M_MODMATRIX_MAX_SOURCES];
    bool modMatrixSlotUsed[M_MODMATRIX_MAX_SLOTS];
    int modMatrixSlotDest[M_MODMATRIX_MAX_SLOTS];
	float lastModMatrixSourceVal[M_MODMATRIX_MAX_SLOTS][M_MODMATRIX_MAX_SOURCES][C_MAX_POLY];
	modMatrixValueLookup modMatrixValueLookupTable[M_MODMATRIX_MAX_SLOTS];

	float m_fDriftLfoSlow[M_MODMATRIX_MAX_SLOTS];
	float m_fDriftLfoFast[M_MODMATRIX_MAX_SLOTS];

	bool m_bWriteDebug = false; 
	bool m_bShallDump = false;
	bool m_bShallCatchClicks = false;
	
	bool m_bBeforeV22CompatibilityMode = false; //to deactivate new skew heavior
	int m_WTmode = 0; //0 sharp 1 soft 2 dull
	
	int m_newestPlaying = -1; //copied here - for display only
	int m_oldestPlaying = -1; //copied here - for display only
	int m_oldestPlayingKeyDown = -1; //copied here - for display only

	VASTFFTSingletonFactory fftSingletonFactory;
	int m_sWaveTableID = 0; 

	float getFrequencyFactorFromLUT(float octave);

	void setTuning(String tuningFile);
	TUN::CSingleScale m_scale;
    
    float m_whiteNoiseBuffer[C_MAX_SAMPLE_RATE * 3]; //3 seconds at highest rate
	
	enum class customFonts{ OpenSans, OpenSansBold, AlteHaasGrotesk, AlteHaasGroteskBold, SFUIDisplayRegular, SFUIDisplayBold, TradeGothicLT, TradeGothicLTBold };
	Font customFontBuffer[8];
	void loadCustomFonts();
	Font getCustomFont(CVASTSettings::customFonts customFont);

	private:	
		bool modMatrixDestinationSet(MYUINT destination); //use fast instead
		bool modMatrixSourceSet(MYUINT source); //use fast instead
		void getMatrixModulationSlotMultiplier(int slot, float& modVal, float& multiplier, int& polarity, MYUINT destination, modMatrixInputState* matrixInputState); //modVal -1..1	

		float m_lookupTable_OctaveFreqFactor[22] = { 0.00048828125f, 0.0009765625f, 0.001953125f, 0.00390625f, 0.0078125f, 0.015625f, 0.03125f, 0.0625f, 0.125f, 0.25f, 0.5f, 1.f, 2.f, 4.f, 8.f, 16.f, 32.f, 64.f, 128.f, 256.f, 512.f, 1024.f }; //-11 .. +11
		float m_lookupTable_CentsFreqFactor[M_CENTS_LUT_SIZE]; // 2^x from 0.000 to 0.999 in 0.001 steps 

		JUCE_LEAK_DETECTOR(CVASTSettings)
};
