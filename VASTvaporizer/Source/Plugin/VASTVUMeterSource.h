/*
VAST Dynamics
*/

#ifndef VASTVUMETERSOURCE_H_INCLUDED
#define VASTVUMETERSOURCE_H_INCLUDED
#include <atomic>
#include <vector>
#include "../Engine/VASTEngineHeader.h"

class VASTVUMeterSource
{
JUCE_BEGIN_IGNORE_WARNINGS_GCC_LIKE("-Wconversion")
JUCE_BEGIN_IGNORE_WARNINGS_MSVC(4244 4267) //C4244 conversion from 'type1' to 'type2', possible loss of data //C4267 conversion

private:
	class ChannelData {
	public:
		ChannelData(const int rmsWindow = 8) :
			max(),
			maxOverall(),
			clip(false),
			reduction(1.0f),
			hold(0),
			rmsHistory(rmsWindow, 0.0),
			rmsSum(0.0),
			rmsPtr(0)
		{}

		ChannelData(const ChannelData& other) :
			max(other.max.load()),
			maxOverall(other.maxOverall.load()),
			clip(other.clip.load()),
			reduction(other.reduction.load()),
			hold(other.hold.load()),
			rmsHistory(8, 0.0),
			rmsSum(0.0),
			rmsPtr(0)
		{}

		ChannelData &operator=(const ChannelData &other) {
			if (this == &other)
			return *this;

			max = other.max.load();
			maxOverall = other.maxOverall.load();
			clip = other.clip.load();
			reduction = other.reduction.load();
			hold = other.hold.load();

			return *this;
		}
		std::atomic<float>       max;
		std::atomic<float>       maxOverall;
		std::atomic<bool>        clip;
		std::atomic<float>       reduction;

		float getAvgRMS() const {
			if (rmsHistory.size() > 0) {
				return sqrtf(rmsSum / rmsHistory.size());
			}
			return sqrtf(rmsSum);
		}
		void setLevels(const juce::int64 time, const float newMax, const float newRms, const juce::int64 holdMSecs)
		{
			if (newMax > 1.0 || newRms > 1.0) {
				clip = true;
			}
			maxOverall = fmaxf(maxOverall, newMax);
			if (newMax >= max) {
				max = std::min(1.0f, newMax);
				hold = time + holdMSecs;
			}
			else if (time > hold) {
				max = std::min(1.0f, newMax);
			}
			pushNextRMS(std::min(1.0f, newRms));
		}
		void setRMSsize(const int numBlocks) {
			rmsHistory.assign(numBlocks, 0.0f);
			rmsSum = 0.0;
			if (numBlocks > 1) {
                rmsPtr.store(rmsPtr.load() % rmsHistory.size());
			}
			else {
				rmsPtr.store(0);
			}
		}
	private:
		void pushNextRMS(const float newRMS) {
			const double squaredRMS = std::min(newRMS * newRMS, 1.0f);
			if (rmsHistory.size() > 0) {
				//                double s=0.0;
				//                for (int i=0; i<rmsHistory.size(); ++i) s+= rmsHistory [i];
				//                if (s != rmsSum)
				//                    DBG ("RMS: " + juce::String(squaredRMS) + " sum: " + juce::String (rmsSum) + " actual sum: " + juce::String (s) + " difference: " + juce::String (rmsSum - s));
				rmsSum = rmsSum + squaredRMS - rmsHistory[rmsPtr];
				rmsHistory[rmsPtr] = squaredRMS;
				rmsPtr = (rmsPtr + 1) % rmsHistory.size();
			}
			else {
				rmsSum = squaredRMS;
			}
		}

		std::atomic<juce::int64> hold;
		std::vector<double>      rmsHistory;
		std::atomic<double>      rmsSum;
        std::atomic<int>         rmsPtr;
	};

public:
	VASTVUMeterSource() :
		holdMSecs(500),
		lastMeasurement(0),
		suspended(false)
	{}

	~VASTVUMeterSource()
	{
		masterReference.clear();
	}

	/**
	Resize the meters data containers. Set the
	\param numChannels to the number of channels. If you don't do this in prepareToPlay,
	it will be done when calling measureBlock, but a few bytes will be allocated
	on the audio thread, so be aware.
	\param rmsWindow is the number of rms values to gather. Keep that aligned with
	the sampleRate and the blocksize to get reproducable results.
	e.g. `rmsWindow = msecs * 0.001f * sampleRate / blockSize;`
	\FIXME: don't call this when measureBlock is processing
	*/
	void resize(const int channels, const int rmsWindow) {
		levels.resize(channels, ChannelData(rmsWindow));
		for (ChannelData& l : levels) {
			l.setRMSsize(rmsWindow);
		}
	}

	/**
	Call this method to measure a block af levels to be displayed in the meters
	*/
	template<typename FloatType>
	void measureBlock(const juce::AudioBuffer<FloatType>& buffer)
	{
		lastMeasurement = juce::Time::currentTimeMillis();
		if (!suspended) {
			const int         numChannels = buffer.getNumChannels();
			const int         numSamples = buffer.getNumSamples();

			levels.resize(numChannels);

			for (int channel = 0; channel < numChannels; ++channel) {
				levels[channel].setLevels(lastMeasurement,
					buffer.getMagnitude(channel, 0, numSamples),
					buffer.getRMSLevel(channel, 0, numSamples),
					holdMSecs);
			}
		}
	}

	/**
	This is called from the GUI. If processing was stalled, this will pump zeroes into the buffer,
	until the readings return to zero.
	*/
	void decayIfNeeded()
	{
		juce::int64 time = juce::Time::currentTimeMillis();
		if (time - lastMeasurement > 100) {
			lastMeasurement = time;
			for (int channel = 0; channel < levels.size(); ++channel) {
				levels[channel].setLevels(lastMeasurement, 0.0f, 0.0f, holdMSecs);
				levels[channel].reduction = 1.0f;
			}
		}
	}

	/**
	With the reduction level you can add an extra bar do indicate, by what amount the level was reduced.
	This will be printed on top of the bar with half the width.
	@param channel the channel index, that was reduced
	@param reduction the factor for the reduction applied to the channel, 1.0=no reduction, 0.0=block completely
	*/
	void setReductionLevel(const int channel, const float reduction)
	{
		if (juce::isPositiveAndBelow(channel, static_cast<int> (levels.size())))
			levels[channel].reduction = reduction;
	}

	/**
	With the reduction level you can add an extra bar do indicate, by what amount the level was reduced.
	This will be printed on top of the bar with half the width.
	@param reduction the factor for the reduction applied to all channels, 1.0=no reduction, 0.0=muted completely
	*/
	void setReductionLevel(const float reduction)
	{
		for (auto& channel : levels)
			channel.reduction = reduction;
	}

	/**
	Set the timeout, how long the peak line will be displayed, before it resets to the
	current peak
	*/
	void setMaxHoldMS(const juce::int64 millis)
	{
		holdMSecs = millis;
	}

	/**
	Returns the reduction level. This value is not computed but can be set
	manually via \see setReductionLevel.
	*/
	float getReductionLevel(const int channel) const
	{
		if (juce::isPositiveAndBelow(channel, static_cast<int> (levels.size())))
			return levels[channel].reduction;
		return -1.0f;
	}

	/**
	This is the max level as displayed by the little line above the RMS bar.
	It is reset by \see setMaxHoldMS.
	*/
	float getMaxLevel(const int channel) const
	{
		return levels.at(channel).max;
	}

	/**
	This is the max level as displayed under the bar as number.
	It will stay up until \see clearMaxNum was called.
	*/
	float getMaxOverallLevel(const int channel) const
	{
		return levels.at(channel).maxOverall;
	}

	/**
	This is the RMS level that the bar will indicate. It is
	summed over rmsWindow number of blocks/measureBlock calls.
	*/
	float getRMSLevel(const int channel) const
	{
		return levels.at(channel).getAvgRMS();
	}

	/**
	Returns the status of the clip flag.
	*/
	bool getClipFlag(const int channel) const
	{
		return levels.at(channel).clip;
	}

	/**
	Reset the clip flag to reset the indicator in the meter
	*/
	void clearClipFlag(const int channel)
	{
		levels.at(channel).clip = false;
	}

	void clearAllClipFlags()
	{
		for (ChannelData& l : levels) {
			l.clip = false;
		}
	}

	/**
	Reset the max number to minus infinity
	*/
	void clearMaxNum(const int channel)
	{
		levels.at(channel).maxOverall = -80.0f;
	}

	/**
	Reset all max numbers
	*/
	void clearAllMaxNums()
	{
		for (ChannelData& l : levels) {
			l.maxOverall = -80.0f;
		}
	}

	/**
	Get the number of channels to be displayed
	*/
	int getNumChannels() const
	{
		return static_cast<int> (levels.size());
	}

	/**
	The measure can be suspended, e.g. to save CPU when no meter is displayed.
	In this case, the \see measureBlock will return immediately
	*/
	void setSuspended(const bool shouldBeSuspended)
	{
		suspended = shouldBeSuspended;
	}

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VASTVUMeterSource)
		juce::WeakReference<VASTVUMeterSource>::Master masterReference;
	friend class juce::WeakReference<VASTVUMeterSource>;

	std::vector<ChannelData> levels;

	juce::int64 holdMSecs;

	std::atomic<juce::int64> lastMeasurement;

	bool suspended;
	
	JUCE_END_IGNORE_WARNINGS_GCC_LIKE
	JUCE_END_IGNORE_WARNINGS_MSVC
};
#endif

