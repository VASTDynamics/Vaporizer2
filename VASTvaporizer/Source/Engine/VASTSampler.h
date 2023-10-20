/*
VAST Dynamics Audio Software (TM)
*/

#pragma once

#include "VASTEngineHeader.h"
#include "VASTSettings.h"

class VASTSamplerSound : public SynthesiserSound
{
public:
	//==============================================================================
	/** Creates a sampled sound from an audio reader.

	This will attempt to load the audio from the source into memory and store
	it in this object.

	@param name         a name for the sample
	@param source       the audio to load. This object can be safely deleted by the
	caller after this constructor returns
	@param midiNotes    the set of midi keys that this sound should be played on. This
	is used by the SynthesiserSound::appliesToNote() method
	@param midiNoteForNormalPitch   the midi note at which the sample should be played
	with its natural rate. All other notes will be pitched
	up or down relative to this one
	@param attackTimeSecs   the attack (fade-in) time, in seconds
	@param releaseTimeSecs  the decay (fade-out) time, in seconds
	@param maxSampleLengthSeconds   a maximum length of audio to read from the audio
	source, in seconds
	*/

	VASTSamplerSound() {};

	VASTSamplerSound(const juce::String& soundName,
		AudioFormatReader& source,
		const BigInteger& midiNotes,
		int midiNoteForNormalPitch
	);

	VASTSamplerSound(const juce::String& soundName,
		AudioSampleBuffer& source,
		int sampleLength,
		int sampleRate,
		const BigInteger& midiNotes,
		int midiNoteForNormalPitch
	);

	VASTSamplerSound(VASTSamplerSound *sound) { //copy constructor
		name = sound->name;
		data = nullptr;
		data_changed = nullptr;
		if (sound->data.get() != nullptr) {
			data.reset(new AudioBuffer<float>(sound->data->getNumChannels(), sound->data->getNumSamples()));
			for (int ch = 0; ch < sound->data->getNumChannels(); ch++) {
				data->copyFrom(ch, 0, sound->data->getReadPointer(ch, 0), sound->data->getNumSamples());
			}
		}
		if (sound->data_changed.get() != nullptr) {
			data_changed.reset(new AudioBuffer<float>(sound->data_changed->getNumChannels(), sound->data_changed->getNumSamples()));
			for (int ch = 0; ch < sound->data_changed->getNumChannels(); ch++) {
				data_changed->copyFrom(ch, 0, sound->data_changed->getReadPointer(ch, 0), sound->data_changed->getNumSamples());
			}
		}
	
		m_zeroCrossings = sound->m_zeroCrossings;
		m_bChangedFlag = sound->m_bChangedFlag;
		
		m_bAudioDataChangedFlag = sound->m_bAudioDataChangedFlag;
		m_PositionChanged = sound->m_PositionChanged;

		sourceSampleRate = sound->sourceSampleRate;
		midiNotes = sound->midiNotes;

		attackSamples = sound->attackSamples;
		releaseSamples = sound->releaseSamples;
		midiRootNote = sound->midiRootNote;
		bHasLoop = sound->bHasLoop;
		iLoopStart = sound->iLoopStart;
		iLoopEnd = sound->iLoopEnd;

		bHasLoop_changed = sound->bHasLoop_changed;
		iLoopStart_changed = sound->iLoopStart_changed;
		iLoopEnd_changed = sound->iLoopEnd_changed;
	};

	/** Destructor. */
	~VASTSamplerSound();

	void clearAllData() {
		
	};

	//==============================================================================
	/** Returns the sample's name */
	const String& getName() const noexcept { return name; }

	/** Returns the audio sample data.
	This could return nullptr if there was a problem loading the data.
	*/
	AudioBuffer<float>* getAudioData() const noexcept; //liveData
	AudioBuffer<float>* getAudioDataChanged();

	//int getLength() { return getAudioData()->getNumSamples() - 4; }; //-4??? CHECK
	//int getLengthChanged() { return getAudioDataChanged()->getNumSamples() - 4; }; //-4??? CHECK
	int getLength();
	int getLengthChanged();

	//==============================================================================
	bool appliesToNote(int midiNoteNumber) override;
	bool appliesToChannel(int midiChannel) override;

	//CHVAST
	void setName(String changedName) { name = changedName; };
	void getValueTreeState(ValueTree* tree, UndoManager* undoManager); //save
	static VASTSamplerSound* getSoundFromTree(ValueTree* tree); //load
	void setMidiRootNode(int rootNote);
	int getMidiRootNote() { return midiRootNote; };
	int getSourceSampleRate() { return sourceSampleRate; };

	bool softFadeExchangeSample();
	bool hasLoop();
	int getLoopStart();
	int getLoopEnd();
	void setLoop(int loopstart, int loopend);

	/*
	void clearLoop() { bHasLoop = false; 
		iLoopStart = 0; 
		iLoopEnd = 0; 
	}
	*/
	int getAttackSamples();
	int getReleaseSamples();

	void setLoopChanged(int loopstart, int loopend);
	void clearLoopChanged();
	int getLoopStartChanged();
	int getLoopEndChanged();
	bool hasLoopChanged();		
	void setChangedFlag();
	void setAudioDataChangedFlag();

	Array<int>* getZeroCrossings();	
	void calcZeroCrossings();
	void notifyPositionChanged();
	bool getPositionChanged();

private:
	//==============================================================================
	//friend class CVASTSingleNote;
	//friend class VASTSamplerVoice;

	String name;
	std::unique_ptr<AudioBuffer<float>> data;
	std::unique_ptr<AudioBuffer<float>> data_changed;
	bool m_bChangedFlag = true;
	bool m_bAudioDataChangedFlag = true;
	double sourceSampleRate = 0;
	BigInteger midiNotes;
	int attackSamples = 0, releaseSamples = 0;
	int midiRootNote = 0;
	bool bHasLoop = false;
	int iLoopStart = 0;
	int iLoopEnd = 0;

	bool bHasLoop_changed = false;
	int iLoopStart_changed = 0;
	int iLoopEnd_changed = 0;

	Array<int> m_zeroCrossings;
	bool m_PositionChanged = false;

	JUCE_LEAK_DETECTOR(VASTSamplerSound)
};
