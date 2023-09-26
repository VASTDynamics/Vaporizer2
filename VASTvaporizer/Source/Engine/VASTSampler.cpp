/*
VAST Dynamics Audio Software (TM)
*/

#include "VASTSampler.h"
#include "VASTEngineHeader.h"
#include "VASTSettings.h"
#include "VASTPluginConstants.h"
#include "VASTSingleNote.h"

VASTSamplerSound::VASTSamplerSound(const juce::String& soundName,
	AudioFormatReader& source,
	const BigInteger& notes,
	int midiNoteForNormalPitch)
	: name(soundName),
	sourceSampleRate(source.sampleRate),
	midiNotes(notes),
	midiRootNote(midiNoteForNormalPitch)
{
	m_zeroCrossings.clear();
	bHasLoop = false;
	iLoopStart = 0;
	iLoopEnd = 0;
	bHasLoop_changed = false;
	iLoopStart_changed = 0;
	iLoopEnd_changed = 0;

	m_bChangedFlag = true;

	if ((sourceSampleRate > 0) && (source.lengthInSamples > 0))
	{
		int length = source.lengthInSamples;

		//data.reset(new AudioBuffer<float>(jmin(2, (int)source.numChannels), length + 4)); //no idea what the +4 is for
		//data_changed.reset(new AudioBuffer<float>(jmin(2, (int)source.numChannels), length + 4)); //no idea what the +4 is for
		data.reset(new AudioBuffer<float>(jmin(2, (int)source.numChannels), length));
		data_changed.reset(new AudioBuffer<float>(jmin(2, (int)source.numChannels), length)); 
		data.get()->clear();
		data_changed.get()->clear();
		//source.read(data.get(), 0, length + 4, 0, true, true);
		source.read(data.get(), 0, length, 0, true, true);
		source.read(data_changed.get(), 0, length, 0, true, true);

		attackSamples = roundToInt(0.0001f  * sourceSampleRate); //0.1ms - 4 samples
		releaseSamples = roundToInt(0.005f * sourceSampleRate); //5ms - 220 samples
	}
	calcZeroCrossings();
}

VASTSamplerSound::VASTSamplerSound(const juce::String& soundName,
	AudioSampleBuffer& source,
	int bufferLength,
	int sampleRate,
	const BigInteger& notes,
	int midiNoteForNormalPitch)
	: name(soundName),
	sourceSampleRate(sampleRate),
	midiNotes(notes),
	midiRootNote(midiNoteForNormalPitch) {

	m_zeroCrossings.clear();
	bHasLoop = false;
	iLoopStart = 0;
	iLoopEnd = 0;
	bHasLoop_changed = false;
	iLoopStart_changed = 0;
	iLoopEnd_changed = 0;
	m_bChangedFlag = true;

	int length = bufferLength;
	int channels = source.getNumChannels();

	//data.reset(new AudioBuffer<float>(jmin(2, channels), length + 4)); //no idea what the +4 is for
	//data_changed.reset(new AudioBuffer<float>(jmin(2, channels), length + 4)); //no idea what the +4 is for
	data.reset(new AudioBuffer<float>(jmin(2, channels), length)); 
	data_changed.reset(new AudioBuffer<float>(jmin(2, channels), length)); 
	data.get()->clear();
	data_changed.get()->clear();
	for (int ch = 0; ch < source.getNumChannels(); ch++) {
		data->copyFrom(ch, 0, source.getReadPointer(ch, 0), length);
		data_changed->copyFrom(ch, 0, source.getReadPointer(ch, 0), length);
	}	
	attackSamples = roundToInt(0.0001f  * sourceSampleRate); //1ms - 4 samples
	releaseSamples = roundToInt(0.005f * sourceSampleRate); //5ms - 220 samples
	calcZeroCrossings();
}

VASTSamplerSound::~VASTSamplerSound()
{
	data = nullptr;
}

void VASTSamplerSound::setMidiRootNode(int rootNote) {
	midiRootNote = rootNote;
}

bool VASTSamplerSound::appliesToNote(int midiNoteNumber)
{
	return midiNotes[midiNoteNumber];
}

bool VASTSamplerSound::appliesToChannel(int /*midiChannel*/)
{
	return true;
}

Array<int>* VASTSamplerSound::getZeroCrossings() {
	return &m_zeroCrossings;
}

void VASTSamplerSound::calcZeroCrossings() {
	//find and store all zero crossings
	int counter = -1;
	m_zeroCrossings.clear();
	while (counter < getAudioDataChanged()->getNumSamples() - 2) {
		counter++;
		float val = getAudioDataChanged()->getSample(0, counter);
		float valp1 = getAudioDataChanged()->getSample(0, counter + 1);
		if ((val >= 0.f) && (valp1 < 0.f)) {
			m_zeroCrossings.add(counter);
		}
		else
			if ((val <= 0.f) && (valp1 > 0.f)) {
				m_zeroCrossings.add(counter);
			}
	}
}

void VASTSamplerSound::notifyPositionChanged() {
	m_PositionChanged = true;
}

bool VASTSamplerSound::getPositionChanged() {
	bool l_PositionChanged = m_PositionChanged;
	m_PositionChanged = false;
	return l_PositionChanged;
}

void VASTSamplerSound::getValueTreeState(ValueTree* tree, UndoManager* undoManager) { //save
	tree->removeAllChildren(undoManager);
	tree->removeAllProperties(undoManager);

	tree->setProperty("sampleName", name, undoManager);
	tree->setProperty("midiRootNote", midiRootNote, undoManager);
	tree->setProperty("sourceSampleRate", sourceSampleRate, undoManager);
	tree->setProperty("isInitial", false, undoManager);
	tree->setProperty("audioDataChannels", data->getNumChannels(), undoManager);
	tree->setProperty("audioDataLength", data->getNumSamples(), undoManager);
	tree->setProperty("hasLoop", bHasLoop, undoManager);
	tree->setProperty("loopStart", iLoopStart, undoManager);
	tree->setProperty("loopEnd", iLoopEnd, undoManager);

	for (int ch = 0; ch < data->getNumChannels(); ch++) {
		String nTable = "";
		nTable.preallocateBytes(data->getNumSamples() * 8);
		auto* dest = (char*)nTable.getCharPointer().getAddress();

		for (int n = 0; n < data->getNumSamples(); n++) {
			float sval = data->getSample(ch, n);			
			unsigned int* sint = (unsigned int*) &sval;
			String s1 = String::toHexString(*sint);
			s1 += String::repeatedString("0", 8 - s1.length());
			memcpy(dest + n * 8, s1.getCharPointer().getAddress(), 8);
		}
		tree->setProperty("audioChannelData" + String(ch), nTable, undoManager);
	}
}

VASTSamplerSound* VASTSamplerSound::getSoundFromTree(ValueTree* tree) { //load //static method
	String l_name = tree->getProperty("sampleName");
	bool l_initial = tree->getProperty("isInitial");
	if (l_initial == false) {
		int channels = tree->getProperty("audioDataChannels");
		int l_length = tree->getProperty("audioDataLength");
		AudioSampleBuffer audioBuffer = AudioSampleBuffer(channels, l_length);
		for (int ch = 0; ch < channels; ch++) {
			String nTable = tree->getProperty("audioChannelData" + String(ch));
			std::string substring = nTable.toStdString();
			int pos = 0;
			for (int n = 0; n < l_length; n++) {
				char charstr[9] = "        ";
				substring.copy(charstr, 8, pos);

				unsigned int sint = (String(charstr)).getHexValue32();
				float fl = *(float*)(&sint);
#ifdef _DEBUG
				jassert(abs(fl) < 6.f);
#endif
				audioBuffer.setSample(ch, n, fl);
				pos += 8;
			}
		}

		BigInteger allNotes;
		allNotes.setRange(0, 128, true);
		int l_midiRootNote = tree->getProperty("midiRootNote");
		double l_sourceSampleRate = tree->getProperty("sourceSampleRate");
		bool l_hasLoop = tree->getProperty("hasLoop");
		int l_loopStart = tree->getProperty("loopStart");
		int l_loopEnd = tree->getProperty("loopEnd");
		VASTSamplerSound* sound = new VASTSamplerSound(l_name, audioBuffer, l_length, l_sourceSampleRate, allNotes, l_midiRootNote);
		if (l_hasLoop) {
			sound->setLoop(l_loopStart, l_loopEnd);
			sound->setLoopChanged(l_loopStart, l_loopEnd);
		}
		return sound;
	}
	return nullptr;
}
