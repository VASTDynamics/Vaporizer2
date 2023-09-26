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
	AudioBuffer<float>* getAudioData() const noexcept { 
		return data.get(); } //liveData
	AudioBuffer<float>* getAudioDataChanged() {
		return data_changed.get();
	};

	//int getLength() { return getAudioData()->getNumSamples() - 4; }; //-4??? CHECK
	//int getLengthChanged() { return getAudioDataChanged()->getNumSamples() - 4; }; //-4??? CHECK
	int getLength() { return getAudioData()->getNumSamples(); }; 
	int getLengthChanged() { return getAudioDataChanged()->getNumSamples(); };

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

	bool softFadeExchangeSample() {
		if (m_bChangedFlag) {
			bHasLoop = bHasLoop_changed;
			iLoopStart = iLoopStart_changed;
			iLoopEnd = iLoopEnd_changed;

			if (m_bAudioDataChangedFlag) {
				if (data_changed.get() != nullptr) {
					data->setSize(data_changed->getNumChannels(), data_changed->getNumSamples());
					int chan = data_changed->getNumChannels();
					for (int c = 0; c < chan; c++) {
						data->copyFrom(c, 0, data_changed->getReadPointer(c, 0), data_changed->getNumSamples());
					}
				}
			}
			m_bChangedFlag = false;
			m_bAudioDataChangedFlag = false;
			return true;
		}
		return false;
	}

	bool hasLoop() { 
		return bHasLoop; };
	int getLoopStart() { 
		return iLoopStart; };
	int getLoopEnd() { 
		return iLoopEnd; };

	void setLoop(int loopstart, int loopend) { 
		bHasLoop = true; 
		if (loopstart < 0) return;
		if (loopstart > loopend) return;
		if (loopend > (getLength() - 1)) return; //loop is on live data
		iLoopStart = loopstart;
		iLoopEnd = loopend; 
	};

	/*
	void clearLoop() { bHasLoop = false; 
		iLoopStart = 0; 
		iLoopEnd = 0; 
	}
	*/
	int getAttackSamples() { return attackSamples; };
	int getReleaseSamples() { return releaseSamples; };

	void setLoopChanged(int loopstart, int loopend) {
		bHasLoop_changed = true;
		if (loopstart < 0) return;
		if (loopstart > loopend) return;
		if (loopend > (getLengthChanged() - 1)) return; 
		iLoopStart_changed = loopstart;
		iLoopEnd_changed = loopend;
		setChangedFlag();
	};
	void clearLoopChanged() {
		bHasLoop_changed = false;
		iLoopStart_changed = 0;
		iLoopEnd_changed = 0;
		setChangedFlag();
	}
	int getLoopStartChanged() {
		return iLoopStart_changed;
	};
	int getLoopEndChanged() {
		return iLoopEnd_changed;
	};
	bool hasLoopChanged() {
		return bHasLoop_changed;
	};

	
	void setChangedFlag() {
		m_bChangedFlag = true;
	}
	void setAudioDataChangedFlag() {
		setChangedFlag();
		m_bAudioDataChangedFlag = true;
		calcZeroCrossings();
	}

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
