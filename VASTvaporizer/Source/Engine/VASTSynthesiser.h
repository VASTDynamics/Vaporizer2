/*
VAST Dynamics Audio Software (TM)
*/

#pragma once

#include "VASTEngineHeader.h"
#include "VASTSettings.h"
#include "VASTSampler.h"

class VASTAudioProcessor; //forward declaration
class VASTSynthesiserSound : public juce::SynthesiserSound {
public:
	VASTSynthesiserSound();

	bool appliesToNote(int);
	bool appliesToChannel(int);

	bool hasSamplerSound();
	VASTSamplerSound* getSamplerSound();

	void addSamplerSound(VASTSamplerSound* samplerSound);

	void clearSamplerSound();
	
	void softFadeExchangeSample();

	bool hasSamplerSoundChanged();
	VASTSamplerSound* getSamplerSoundChanged();
	void clearSamplerSoundChanged();
	void addSamplerSoundChanged(VASTSamplerSound* samplerSound);

	OwnedArray<VASTSamplerSound> m_samplerSound{};
	OwnedArray<VASTSamplerSound> m_samplerSound_changed{};
	bool m_changedFlag = true;
};

//==============================================================================
/**
Represents a voice that a Synthesiser can use to play a juce::SynthesiserSound.

A voice plays a single sound at a time, and a synthesiser holds an array of
voices so that it can play polyphonically.

@see Synthesiser, juce::SynthesiserSound

@tags{Audio}
*/
class VASTSynthesiserVoice
{
public:
	//==============================================================================
	/** Creates a voice. */
	VASTSynthesiserVoice();

	/** Destructor. */
	virtual ~VASTSynthesiserVoice();

	//==============================================================================
	/** Returns the midi note that this voice is currently playing.
	Returns a value less than 0 if no note is playing.
	*/
	int getCurrentlyPlayingNote() const noexcept { return currentlyPlayingNote; }

	/** Returns the sound that this voice is currently playing.
	Returns nullptr if it's not playing.
	*/
	juce::SynthesiserSound::Ptr getCurrentlyPlayingSound() const noexcept { return currentlyPlayingSound; }

	/** Must return true if this voice object is capable of playing the given sound.

	If there are different classes of sound, and different classes of voice, a voice can
	choose which ones it wants to take on.

	A typical implementation of this method may just return true if there's only one type
	of voice and sound, or it might check the type of the sound object passed-in and
	see if it's one that it understands.
	*/
	virtual bool canPlaySound(juce::SynthesiserSound*) = 0;

	/** Called to start a new note.
	This will be called during the rendering callback, so must be fast and thread-safe.
	*/
	virtual void startNote(int midiNoteNumber,
		float velocity,
		juce::SynthesiserSound* sound,
		int currentPitchWheelPosition,
		bool legato) = 0;

	/** Called to stop a note.

	This will be called during the rendering callback, so must be fast and thread-safe.

	The velocity indicates how quickly the note was released - 0 is slowly, 1 is quickly.

	If allowTailOff is false or the voice doesn't want to tail-off, then it must stop all
	sound immediately, and must call clearCurrentNote() to reset the state of this voice
	and allow the synth to reassign it another sound.

	If allowTailOff is true and the voice decides to do a tail-off, then it's allowed to
	begin fading out its sound, and it can stop playing until it's finished. As soon as it
	finishes playing (during the rendering callback), it must make sure that it calls
	clearCurrentNote().
	*/
	virtual void stopNote(float velocity, bool allowTailOff) = 0;

	/** Returns true if this voice is currently busy playing a sound.
	By default this just checks the getCurrentlyPlayingNote() value, but can
	be overridden for more advanced checking.
	*/
	virtual bool isVoiceActive() const;

	/** Called to let the voice know that the pitch wheel has been moved.
	This will be called during the rendering callback, so must be fast and thread-safe.
	*/
	virtual void pitchWheelMoved(int newPitchWheelValue, bool zone) = 0;

	/** Called to let the voice know that a midi controller has been moved.
	This will be called during the rendering callback, so must be fast and thread-safe.
	*/
	virtual void controllerMoved(int controllerNumber, int newControllerValue) = 0;

	/** Called to let the voice know that the aftertouch has changed.
	This will be called during the rendering callback, so must be fast and thread-safe.
	*/
	virtual void aftertouchChanged(int newAftertouchValue) = 0;

	/** Called to let the voice know that the channel pressure has changed.
	This will be called during the rendering callback, so must be fast and thread-safe.
	*/
	virtual void channelPressureChanged(int newChannelPressureValue) = 0;

	//==============================================================================
	/** Renders the next block of data for this voice.

	The output audio data must be added to the current contents of the buffer provided.
	Only the region of the buffer between startSample and (startSample + numSamples)
	should be altered by this method.

	If the voice is currently silent, it should just return without doing anything.

	If the sound that the voice is playing finishes during the course of this rendered
	block, it must call clearCurrentNote(), to tell the synthesiser that it has finished.

	The size of the blocks that are rendered can change each time it is called, and may
	involve rendering as little as 1 sample at a time. In between rendering callbacks,
	the voice's methods will be called to tell it about note and controller events.
	*/
	virtual void renderNextBlock(sRoutingBuffers& routingBuffers,
		int startSample,
		int numSamples) = 0;

	/** Changes the voice's reference sample rate.

	The rate is set so that subclasses know the output rate and can set their pitch
	accordingly.

	This method is called by the synth, and subclasses can access the current rate with
	the currentSampleRate member.
	*/
	virtual void setCurrentPlaybackSampleRate(double newRate);

	/** Returns true if the voice is currently playing a sound which is mapped to the given
	midi channel.

	If it's not currently playing, this will return false.
	*/
	virtual bool isPlayingChannel(int midiChannel) const;

	/** Returns the current target sample rate at which rendering is being done.
	Subclasses may need to know this so that they can pitch things correctly.
	*/
	double getSampleRate() const noexcept { return currentSampleRate; }

	/** Returns true if the key that triggered this voice is still held down.
	Note that the voice may still be playing after the key was released (e.g because the
	sostenuto pedal is down).
	*/
	bool isKeyDown() const noexcept { return keyIsDown; }

	/** Allows you to modify the flag indicating that the key that triggered this voice is still held down.
	@see isKeyDown
	*/
	void setKeyDown(bool isNowDown) noexcept { keyIsDown = isNowDown; }

	/** Returns true if the sustain pedal is currently active for this voice. */
	bool isSustainPedalDown() const noexcept { return sustainPedalDown; }

	/** Modifies the sustain pedal flag. */
	void setSustainPedalDown(bool isNowDown) noexcept { sustainPedalDown = isNowDown; }

	/** Returns true if the sostenuto pedal is currently active for this voice. */
	bool isSostenutoPedalDown() const noexcept { return sostenutoPedalDown; }

	/** Modifies the sostenuto pedal flag. */
	void setSostenutoPedalDown(bool isNowDown) noexcept { sostenutoPedalDown = isNowDown; }

	/** Returns true if a voice is sounding in its release phase **/
	virtual bool isPlayingButReleased() = 0;

	/** Returns true if this voice started playing its current note before the other voice did. */
	bool wasStartedBefore(const VASTSynthesiserVoice& other) const noexcept;

	int getVoiceNo() const { return mVoiceNo; };

	virtual void clearCurrentNote() = 0;

protected:
	/** Resets the state of this voice after a sound has finished playing.

	The subclass must call this when it finishes playing a note and becomes available
	to play new ones.

	It must either call it in the stopNote() method, or if the voice is tailing off,
	then it should call it later during the renderNextBlock method, as soon as it
	finishes its tail-off.

	It can also be called at any time during the render callback if the sound happens
	to have finished, e.g. if it's playing a sample and the sample finishes.
	*/
    std::atomic<int> mVoiceNo = 0;
	std::atomic<int> currentlyPlayingNote = -1, currentPlayingMidiChannel = 0;
	juce::SynthesiserSound::Ptr currentlyPlayingSound;

private:
	//==============================================================================
	friend class VASTSynthesiser;

	double currentSampleRate = 44100.0;
	uint32 noteOnTime = 0;
	bool keyIsDown = false, sustainPedalDown = false, sostenutoPedalDown = false;

	AudioBuffer<float> tempBuffer;

#if JUCE_CATCH_DEPRECATED_CODE_MISUSE
	// Note the new parameters for this method.
	virtual int stopNote(bool) { return 0; }
#endif

	JUCE_LEAK_DETECTOR(VASTSynthesiserVoice)
};

class CVASTPoly; //forward decalarion

//==============================================================================
/**
Base class for a musical device that can play sounds.

To create a synthesiser, you'll need to create a subclass of juce::SynthesiserSound
to describe each sound available to your synth, and a subclass of VASTSynthesiserVoice
which can play back one of these sounds.

Then you can use the addVoice() and addSound() methods to give the synthesiser a
set of sounds, and a set of voices it can use to play them. If you only give it
one voice it will be monophonic - the more voices it has, the more polyphony it'll
have available.

Then repeatedly call the renderNextBlock() method to produce the audio. Any midi
events that go in will be scanned for note on/off messages, and these are used to
start and stop the voices playing the appropriate sounds.

While it's playing, you can also cause notes to be triggered by calling the noteOn(),
noteOff() and other controller methods.

Before rendering, be sure to call the setCurrentPlaybackSampleRate() to tell it
what the target playback rate is. This value is passed on to the voices so that
they can pitch their output correctly.

@tags{Audio}
*/
class VASTSynthesiser
{
public:
	//==============================================================================
	/** Creates a new synthesiser.
	You'll need to add some sounds and voices before it'll make any sound.
	*/
	VASTSynthesiser(VASTAudioProcessor* processor);

	/** Destructor. */
	virtual ~VASTSynthesiser();

	//CHVAST
	void init() {
		initValues();
	}

	void initValues();

	//==============================================================================
	/** Deletes all voices. */
	void clearVoices();

	/** Returns the number of voices that have been added. */
	int getNumVoices() const noexcept { return voices.size(); }

	/** Returns one of the voices that have been added. */
	VASTSynthesiserVoice* getVoice(int index) const;

	/** Adds a new voice to the synth.

	All the voices should be the same class of object and are treated equally.

	The object passed in will be managed by the synthesiser, which will delete
	it later on when no longer needed. The caller should not retain a pointer to the
	voice.
	*/
	VASTSynthesiserVoice* addVoice(VASTSynthesiserVoice* newVoice);

	/** Deletes one of the voices. */
	void removeVoice(int index);

	//==============================================================================
	/** Deletes all sounds. */
	void clearSounds();

	/** Returns the number of sounds that have been added to the synth. */
	int getNumSounds() const noexcept { return sounds.size(); }

	/** Returns one of the sounds. */
	juce::SynthesiserSound* getSound(int index) const noexcept { return sounds[index]; }

	/** Adds a new sound to the synthesiser.

	The object passed in is reference counted, so will be deleted when the
	synthesiser and all voices are no longer using it.
	*/
	juce::SynthesiserSound* addSound(const juce::SynthesiserSound::Ptr& newSound);

	/** Removes and deletes one of the sounds. */
	void removeSound(int index);

	//==============================================================================
	/** If set to true, then the synth will try to take over an existing voice if
	it runs out and needs to play another note.

	The value of this boolean is passed into findFreeVoice(), so the result will
	depend on the implementation of this method.
	*/
	void setNoteStealingEnabled(bool shouldStealNotes);

	/** Returns true if note-stealing is enabled.
	@see setNoteStealingEnabled
	*/
	bool isNoteStealingEnabled() const noexcept { return shouldStealNotes; }

	//==============================================================================
	/** Triggers a note-on event.

	The default method here will find all the sounds that want to be triggered by
	this note/channel. For each sound, it'll try to find a free voice, and use the
	voice to start playing the sound.

	Subclasses might want to override this if they need a more complex algorithm.

	This method will be called automatically according to the midi data passed into
	renderNextBlock(), but may be called explicitly too.

	The midiChannel parameter is the channel, between 1 and 16 inclusive.
	*/
	virtual void noteOn(int midiChannel,
		int midiNoteNumber,
		float velocity);

	/** Triggers a note-off event.

	This will turn off any voices that are playing a sound for the given note/channel.

	If allowTailOff is true, the voices will be allowed to fade out the notes gracefully
	(if they can do). If this is false, the notes will all be cut off immediately.

	This method will be called automatically according to the midi data passed into
	renderNextBlock(), but may be called explicitly too.

	The midiChannel parameter is the channel, between 1 and 16 inclusive.
	*/
	virtual void noteOff(int midiChannel,
		int midiNoteNumber,
		float velocity,
		bool allowTailOff);

	/** Turns off all notes.

	This will turn off any voices that are playing a sound on the given midi channel.

	If midiChannel is 0 or less, then all voices will be turned off, regardless of
	which channel they're playing. Otherwise it represents a valid midi channel, from
	1 to 16 inclusive.

	If allowTailOff is true, the voices will be allowed to fade out the notes gracefully
	(if they can do). If this is false, the notes will all be cut off immediately.

	This method will be called automatically according to the midi data passed into
	renderNextBlock(), but may be called explicitly too.
	*/
	virtual void allNotesOff(int midiChannel,
		bool allowTailOff);

	/** Sends a pitch-wheel message to any active voices.

	This will send a pitch-wheel message to any voices that are playing sounds on
	the given midi channel.

	This method will be called automatically according to the midi data passed into
	renderNextBlock(), but may be called explicitly too.

	@param midiChannel          the midi channel, from 1 to 16 inclusive
	@param wheelValue           the wheel position, from 0 to 0x3fff, as returned by MidiMessage::getPitchWheelValue()
	*/
	virtual void handlePitchWheel(int midiChannel,
		int wheelValue);

	/** Sends a midi controller message to any active voices.

	This will send a midi controller message to any voices that are playing sounds on
	the given midi channel.

	This method will be called automatically according to the midi data passed into
	renderNextBlock(), but may be called explicitly too.

	@param midiChannel          the midi channel, from 1 to 16 inclusive
	@param controllerNumber     the midi controller type, as returned by MidiMessage::getControllerNumber()
	@param controllerValue      the midi controller value, between 0 and 127, as returned by MidiMessage::getControllerValue()
	*/
	virtual void handleController(int midiChannel,
		int controllerNumber,
		int controllerValue);

	/** Sends an aftertouch message.

	This will send an aftertouch message to any voices that are playing sounds on
	the given midi channel and note number.

	This method will be called automatically according to the midi data passed into
	renderNextBlock(), but may be called explicitly too.

	@param midiChannel          the midi channel, from 1 to 16 inclusive
	@param midiNoteNumber       the midi note number, 0 to 127
	@param aftertouchValue      the aftertouch value, between 0 and 127,
	as returned by MidiMessage::getAftertouchValue()
	*/
	virtual void handleAftertouch(int midiChannel, int midiNoteNumber, int aftertouchValue);

	/** Sends a channel pressure message.

	This will send a channel pressure message to any voices that are playing sounds on
	the given midi channel.

	This method will be called automatically according to the midi data passed into
	renderNextBlock(), but may be called explicitly too.

	@param midiChannel              the midi channel, from 1 to 16 inclusive
	@param channelPressureValue     the pressure value, between 0 and 127, as returned
	by MidiMessage::getChannelPressureValue()
	*/
	virtual void handleChannelPressure(int midiChannel, int channelPressureValue);

	/** Handles a sustain pedal event. */
	virtual void handleSustainPedal(int midiChannel, bool isDown);

	/** Handles a sostenuto pedal event. */
	virtual void handleSostenutoPedal(int midiChannel, bool isDown);

	/** Can be overridden to handle soft pedal events. */
	virtual void handleSoftPedal(int midiChannel, bool isDown);

	/** Can be overridden to handle an incoming program change message.
	The base class implementation of this has no effect, but you may want to make your
	own synth react to program changes.
	*/
	virtual void handleProgramChange(int midiChannel,
		int programNumber);

	//==============================================================================
	/** Tells the synthesiser what the sample rate is for the audio it's being used to render.

	This value is propagated to the voices so that they can use it to render the correct
	pitches.
	*/
	virtual void setCurrentPlaybackSampleRate(double sampleRate);

	/** Creates the next block of audio output.

	This will process the next numSamples of data from all the voices, and add that output
	to the audio block supplied, starting from the offset specified. Note that the
	data will be added to the current contents of the buffer, so you should clear it
	before calling this method if necessary.

	The midi events in the inputMidi buffer are parsed for note and controller events,
	and these are used to trigger the voices. Note that the startSample offset applies
	both to the audio output buffer and the midi input buffer, so any midi events
	with timestamps outside the specified region will be ignored.
	*/
	/*
	inline void renderNextBlock (sRoutingBuffers& routingBuffers,
	const MidiBuffer& inputMidi,
	int startSample,
	int numSamples)
	{
	processNextBlock(routingBuffers, inputMidi, startSample, numSamples);
	}
	*/
	
	void renderNextBlock(sRoutingBuffers& routingBuffers,
		const MidiBuffer& inputMidi,
		int startSample,
		int numSamples);

	/** Returns the current target sample rate at which rendering is being done.
	Subclasses may need to know this so that they can pitch things correctly.
	*/
	double getSampleRate() const noexcept { return sampleRate; }

	/** Sets a minimum limit on the size to which audio sub-blocks will be divided when rendering.

	When rendering, the audio blocks that are passed into renderNextBlock() will be split up
	into smaller blocks that lie between all the incoming midi messages, and it is these smaller
	sub-blocks that are rendered with multiple calls to renderVoices().

	Obviously in a pathological case where there are midi messages on every sample, then
	renderVoices() could be called once per sample and lead to poor performance, so this
	setting allows you to set a lower limit on the block size.

	The default setting is 32, which means that midi messages are accurate to about < 1ms
	accuracy, which is probably fine for most purposes, but you may want to increase or
	decrease this value for your synth.

	If shouldBeStrict is true, the audio sub-blocks will strictly never be smaller than numSamples.

	If shouldBeStrict is false (default), the first audio sub-block in the buffer is allowed
	to be smaller, to make sure that the first MIDI event in a buffer will always be sample-accurate
	(this can sometimes help to avoid quantisation or phasing issues).
	*/
	void setMinimumRenderingSubdivisionSize(int numSamples, bool shouldBeStrict = false) noexcept;

	//CHVAST
	int getLastPlayedVoiceNo() const {
		return m_newestPlaying;
	};
	int getOldestPlayedVoiceNo() const {
		return m_oldestPlaying;
	};
	int getNumMidiNotesKeyDown() const { 
		return m_midiNotesNumKeyDown; 
	};

	LinearSmoothedValue<float> m_fPitchBendZone_smoothed[C_MAX_POLY]; //have to be 32 midi channels for MPE voices
	LinearSmoothedValue<float> m_fModWheel_smoothed[C_MAX_POLY]; //have to be 32 midi channels for MPE voices

	atomic<int> m_numVoicesPlaying = 0;
	atomic<int> m_numOscsPlaying = 0;
	atomic<bool> m_voicePlaying[C_MAX_POLY];

	int m_MPEMasterChannel = 1; //TODO MPE config messages
		/** The last pitch-wheel values for each midi channel. */
	atomic<int> lastPitchWheelValues[C_MAX_POLY]{}; //have to be 32 midi channels for MPE voices
	atomic<int> lastPitchWheelUIValue = 0x2000;

protected:
	//==============================================================================
	/** This is used to control access to the rendering callback and the note trigger methods. */
	CriticalSection lock;

	OwnedArray<VASTSynthesiserVoice> voices;
	ReferenceCountedArray<juce::SynthesiserSound> sounds;

	/** Renders the voices for the given range.
	By default this just calls renderNextBlock() on each voice, but you may need
	to override it to handle custom cases.
	*/
	virtual void renderVoices(sRoutingBuffers& routingBuffers,
		int startSample, int numSamples);

	/** Searches through the voices to find one that's not currently playing, and
	which can play the given sound.

	Returns nullptr if all voices are busy and stealing isn't enabled.

	To implement a custom note-stealing algorithm, you can either override this
	method, or (preferably) override findVoiceToSteal().
	*/
	virtual VASTSynthesiserVoice* findFreeVoice(juce::SynthesiserSound* soundToPlay,
		int midiChannel,
		int midiNoteNumber,
		bool stealIfNoneAvailable) const;

	/** Chooses a voice that is most suitable for being re-used.
	The default method will attempt to find the oldest voice that isn't the
	bottom or top note being played. If that's not suitable for your synth,
	you can override this method and do something more cunning instead.
	*/
	virtual VASTSynthesiserVoice* findVoiceToSteal(juce::SynthesiserSound* soundToPlay,
		int midiChannel,
		int midiNoteNumber) const;

	//CHVAST
	virtual VASTSynthesiserVoice* findActiveVoiceToSteal(juce::SynthesiserSound* soundToPlay,
		int midiChannel,
		int midiNoteNumber) const;


	/** Starts a specified voice playing a particular sound.
	You'll probably never need to call this, it's used internally by noteOn(), but
	may be needed by subclasses for custom behaviours.
	*/
	void startVoice(VASTSynthesiserVoice* voice,
		juce::SynthesiserSound* sound,
		int midiChannel,
		int midiNoteNumber,
		float velocity);

	/** Stops a given voice.
	You should never need to call this, it's used internally by noteOff, but is protected
	in case it's useful for some custom subclasses. It basically just calls through to
	VASTSynthesiserVoice::stopNote(), and has some assertions to sanity-check a few things.
	*/
	void stopVoice(VASTSynthesiserVoice*, float velocity, bool allowTailOff);

	/** Can be overridden to do custom handling of incoming midi events. */
	virtual void handleMidiEvent(const MidiMessage&);

private:
	//==============================================================================
	void processNextBlock(sRoutingBuffers& routingBuffers,
		const MidiBuffer& inputMidi,
		int startSample,
		int numSamples);
	//==============================================================================
	double sampleRate = 0;
	uint32 lastNoteOnCounter = 0;
	int minimumSubBlockSize = 32;
	bool subBlockSubdivisionIsStrict = false; //was false
	bool shouldStealNotes = true;
	BigInteger sustainPedalsDown;

	AudioSampleBuffer qfilterbuffer;
	std::atomic<int> m_newestPlaying = -1;
    std::atomic<int> m_oldestPlaying = -1;
    std::atomic<int> m_oldestPlayingKeyDown = -1;
	bool m_midiNotesKeyDown[256];
	ULong64_t m_midiNotesKeyDownTime[256];

	int m_midiNotesNumKeyDown = 0;
	//Glissando
	bool m_oldChordStack[256];
	bool m_newChordStack[256];
	bool m_bGlissandoUsed = false;	
	int m_iGlissandoAssigned = 0;
	int m_iGlissandoCollectSamples = 0; 
	CVASTSettings* m_Set = nullptr;
	CVASTPoly* m_Poly = nullptr;
	VASTAudioProcessor* myProcessor = nullptr;
	uint8 lastPressureLowerBitReceivedOnChannel[C_MAX_POLY]; //have to be 32 midi channels for MPE voices
	MPEValue lastTimbreReceivedOnChannel[C_MAX_POLY]; //have to be 32 midi channels for MPE voices

#if JUCE_CATCH_DEPRECATED_CODE_MISUSE
	// Note the new parameters for these methods.
	virtual int findFreeVoice(const bool) const { return 0; }
	virtual int noteOff(int, int, int) { return 0; }
	virtual int findFreeVoice(juce::SynthesiserSound*, const bool) { return 0; }
	virtual int findVoiceToSteal(juce::SynthesiserSound*) const { return 0; }
#endif

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VASTSynthesiser)
};
