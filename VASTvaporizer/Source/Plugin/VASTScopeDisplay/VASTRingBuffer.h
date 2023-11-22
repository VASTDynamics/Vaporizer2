/*
VAST Dynamics
*/

#pragma once

#include "../../Engine/VASTEngineHeader.h"

/** A circular, lock-free buffer for multiple channels of audio.
 
    Supports a single writer (producer) and any number of readers (consumers).
 
    Make sure that the number of samples read from the VASTRingBuffer in every
    readSamples() call is less than the bufferSize specified in the constructor.
 
    Also, ensure that the number of samples read from the VASTRingBuffer at any time
    plus the number of samples written to the VASTRingBuffer at any time never exceed
    the buffer size. This prevents read/write overlap.
*/
template <class Type>
class VASTRingBuffer
{
public:
    
    /** Initializes the VASTRingBuffer with the specified channels and size.
     
        @param numChannels  number of channels of audio to store in buffer
        @param bufferSize   size of the audio buffer
     */
    VASTRingBuffer (int numChannels, int bufferSize)
    {
        this->bufferSize = bufferSize;
        this->numChannels = numChannels;
        
        audioBuffer = std::make_unique<AudioBuffer<Type>>(numChannels, bufferSize);
		audioBuffer->clear();

        writePosition = 0;
		fifoReadPosition = 0;
    }
    
    
    /** Writes samples to all channels in the VASTRingBuffer.
     
        @param newAudioData     an audio buffer to write into the VASTRingBuffer
                                This AudioBuffer must have the same number of
                                channels as specified in the VASTRingBuffer's constructor.
        @param startSample      the starting sample in the newAudioData to write
                                into the VASTRingBuffer
        @param numSamples       the number of samples from newAudioData to write
                                into the VASTRingBuffer
     */
    void writeSamples (AudioBuffer<Type> & newAudioData, int startSample, int numSamples)
    {
        if (newAudioData.getNumChannels() < numChannels) 
            return;
        
        for (int i = 0; i < numChannels; ++i)
        {
            const int curWritePosition = writePosition.get();
            
            // If we need to loop around the ring
            //if (curWritePosition + numSamples > bufferSize.load() - 1)
			if (curWritePosition + numSamples > bufferSize.load())
            {
				int samplesToEdgeOfBuffer = bufferSize.load() - curWritePosition;
                
                audioBuffer->copyFrom (i, curWritePosition, newAudioData, i,
                                       startSample, samplesToEdgeOfBuffer);
                
                audioBuffer->copyFrom (i, 0, newAudioData, i,
                                       startSample + samplesToEdgeOfBuffer,
                                       numSamples - samplesToEdgeOfBuffer);
            }
            // If we stay inside the ring
            else
            {
                audioBuffer->copyFrom (i, curWritePosition, newAudioData, i,
                                       startSample, numSamples);
            }
        }
        
        //writePosition += numSamples;
		//writePosition = writePosition.get() % bufferSize.load();
		writePosition = (writePosition.get() + numSamples) % bufferSize.load();
        
        /*
            Although it would seem that the above two lines could cause a
            problem for the consumer calling readsSamples() since it uses the
            value of writePosition to calculate the readPosition, this is not
            the case.
            
            1.  Since writePosition is Atomic, there will be no torn reads.
         
            2.  In the case that the producer only executes the line:
        
                    writePosition += numSamples;
         
                then writePosition has the posibility of being outside the
                bounds of the buffer. If the consumer then calls readsSamples()
                and executes the line:
         
                    int readPosition = (writePosition.get() % bufferSize) - readSize;
         
                it protects itself from an out-of-bounds writePosition by
                calculating the modulus with the buffer size. If the
                writePosition is outside the bounds of the buffer momentarily,
                the consumer's call to readSamples() stays protected and accurate.
         */
    }
    
    /** Reads readSize number of samples in front of the write position from all
        channels in the VASTRingBuffer into the bufferToFill.
     
         @param bufferToFill    buffer to be filled with most recent audio
                                samples from the VASTRingBuffer
         @param readSize        number of samples to read from the VASTRingBuffer.
                                Note, this must be less than the buffer size
                                of the VASTRingBuffer specified in the constructor.
    */
    void readSamples (AudioBuffer<Type> & bufferToFill, int readSize)
    {
        // Ensure readSize does not exceed bufferSize
        jassert (readSize <= bufferSize.load());

        /*
            Further, as stated in the class comment, it is also bad to have a
            read zone that overlaps with the writing zone, but this wont't cause
            too much of a visual problem unless it happens often.
            To combat this, the user should avoid having any read size and any
            write size that when added together, exceed the bufferSize of the
            VASTRingBuffer.
         */
        
        // Calculate readPosition based on writePosition
        int readPosition = (writePosition.get() % bufferSize.load()) - readSize;
        
        // If read position goes into negative bounds, loop it around the ring
        if (readPosition < 0)
            readPosition = bufferSize + readPosition;
        
        for (int i = 0; i < numChannels; ++i)
        {
            // If we need to loop around the ring
            //if (readPosition + readSize > bufferSize - 1)
			if (readPosition + readSize > bufferSize.load())
            {
                int samplesToEdgeOfBuffer = bufferSize.load() - readPosition;
                
                bufferToFill.copyFrom (i, 0, *(audioBuffer.get()), i, readPosition,
                                       samplesToEdgeOfBuffer);
                
                bufferToFill.copyFrom (i, samplesToEdgeOfBuffer, *(audioBuffer.get()),
					//i, readPosition + samplesToEdgeOfBuffer, readSize - samplesToEdgeOfBuffer);
					i, 0, readSize - samplesToEdgeOfBuffer);
            }
            // If we stay inside the ring
            else
            {
                bufferToFill.copyFrom (i, 0, *(audioBuffer.get()), i, readPosition, readSize);
            }
        }
    }
    
	void readSamplesInterpolated(AudioBuffer<Type> & bufferToFill, int readSize, int visualSize)
	{
		// Ensure readSize does not exceed bufferSize
		jassert(readSize <= bufferSize.load());

		/*
			Further, as stated in the class comment, it is also bad to have a
			read zone that overlaps with the writing zone, but this wont't cause
			too much of a visual problem unless it happens often.
			To combat this, the user should avoid having any read size and any
			write size that when added together, exceed the bufferSize of the
			VASTRingBuffer.
		 */

		 // Calculate readPosition based on writePosition
		int readPosition = (writePosition.get() % bufferSize.load()) - readSize;

		// If read position goes into negative bounds, loop it around the ring
		if (readPosition < 0)
			readPosition = bufferSize.load() + readPosition;

		for (int i = 0; i < numChannels; ++i)
		{
			// If we need to loop around the ring
			//if (readPosition + readSize > bufferSize.load() - 1)
			if (readPosition + readSize > bufferSize.load())
			{
				int samplesToEdgeOfBuffer = bufferSize.load() - readPosition;

				bufferToFill.copyFrom(i, 0, *(audioBuffer.get()), i, readPosition,
					samplesToEdgeOfBuffer);

				bufferToFill.copyFrom(i, samplesToEdgeOfBuffer, *(audioBuffer.get()),
					//i, readPosition + samplesToEdgeOfBuffer, readSize - samplesToEdgeOfBuffer);
					i, 0, readSize - samplesToEdgeOfBuffer);
			}
			// If we stay inside the ring
			else
			{
				bufferToFill.copyFrom(i, 0, *(audioBuffer.get()), i, readPosition, readSize);
			}
		}

		jassert(visualSize < readSize);
		float* wp1 = bufferToFill.getWritePointer(0, 0);
		float* wp2 = nullptr;
		if (numChannels == 2)
			wp2 = bufferToFill.getWritePointer(1, 0);
		for (int i = 0; i < visualSize; i++) {
			float pos = i * (float(readSize) / float(visualSize));
			int intpos = i * (float(readSize) / float(visualSize));
			float frac = pos - intpos;
			jassert(pos < readSize);
			wp1[i] = (1.f - frac) * wp1[intpos] + frac * wp1[intpos + 1];
			if (numChannels == 2)
				wp2[i] = (1.f - frac) * wp2[intpos] + frac * wp2[intpos + 1];
		}
	}

	void readSamplesFIFO(AudioBuffer<Type> & bufferToFill, int readSize) {

		// Ensure readSize does not exceed bufferSize
		jassert(readSize <= bufferSize.load());

		 // Calculate readPosition based on writePosition
		int readPosition = fifoReadPosition.get();

		// If read position goes into negative bounds, loop it around the ring
		if (readPosition < 0)
			readPosition = bufferSize.load() + readPosition;

		for (int i = 0; i < numChannels; ++i)
		{
			// If we need to loop around the ring
			//if (readPosition + readSize > bufferSize - 1)
			if (readPosition + readSize > bufferSize.load())
			{
				int samplesToEdgeOfBuffer = bufferSize.load() - readPosition;

				bufferToFill.copyFrom(i, 0, *(audioBuffer.get()), i, readPosition,
					samplesToEdgeOfBuffer);

				bufferToFill.copyFrom(i, samplesToEdgeOfBuffer, *(audioBuffer.get()),
					//i, readPosition + samplesToEdgeOfBuffer, readSize - samplesToEdgeOfBuffer);
					i, 0, readSize - samplesToEdgeOfBuffer);
			}
			// If we stay inside the ring
			else
			{
				bufferToFill.copyFrom(i, 0, *(audioBuffer.get()), i, readPosition, readSize);
			}
		}
		fifoReadPosition = (fifoReadPosition.get() + readSize) % bufferSize.load();
	}


private:
    std::atomic<int> bufferSize;
    int numChannels;
    std::unique_ptr<AudioBuffer<Type>> audioBuffer;
    Atomic<int> writePosition; // This must be atomic so the consumer does
                               // not read it in a torn state as it is being
                               // changed.

	Atomic<int> fifoReadPosition;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VASTRingBuffer)
};
