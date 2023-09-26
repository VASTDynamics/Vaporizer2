/*
VAST Dynamics
*/
#ifndef VASTVUMETER_H_INCLUDED
#define VASTVUMETER_H_INCLUDED

#include "../Engine/VASTEngineHeader.h"
#include "VASTVUMeterSource.h"

class VASTVUMeter : public juce::Component, private juce::Timer
{
public:

	enum MeterFlags {
		Default = 0x0000, /**< Default is showing all channels in the VASTVUMeterSource without a border */
		Horizontal = 0x0001, /**< Displays the level bars horizontally */
		Vintage = 0x0002, /**< Switches to a special mode of old school meters (to come) */
		SingleChannel = 0x0004, /**< Display only one channel meter. \see setSelectedChannel */
		HasBorder = 0x0008, /**< Displays a rounded border around the meter. This is used with the default constructor */
		Reduction = 0x0010, /**< This turns the bar into a reduction bar.
							The additional reduction bar is automatically added, as soon a reduction value < 1.0 is set
							in the VASTVUMeterSource. \see VASTVUMeterSource::setReductionLevel */
		Minimal = 0x0020, /**< For a stereo meter, this tries to save space by showing only one line tickmarks in the middle and no max numbers */
		MaxNumber = 0x0040  /**< To add level meter to Minimal, set this flag */
	};

	enum ColourIds {
		lmTextColour = 0x2200001,   /**< Colour for the numbers etc. */
		lmTextDeactiveColour,       /**< Unused, will eventually be removed */
		lmTextClipColour,           /**< Colour to print the max number if it has clipped */
		lmTicksColour,              /**< Colour for the tick marks */
		lmOutlineColour,            /**< Colour for the frame around all */
		lmBackgroundColour,         /**< Background colour */
		lmBackgroundClipColour,     /**< This is the colour of the clip indicator if it has clipped */
		lmMeterForegroundColour,    /**< Unused, will eventually be removed */
		lmMeterOutlineColour,       /**< Colour for the outlines of meter bars etc. */
		lmMeterBackgroundColour,    /**< Background colour for the actual meter bar and the max number */
		lmMeterMaxNormalColour,     /**< Text colour for the max number, if under warn threshold */
		lmMeterMaxWarnColour,       /**< Text colour for the max number, if between warn threshold and clip threshold */
		lmMeterMaxOverColour,       /**< Text colour for the max number, if above the clip threshold */
		lmMeterGradientLowColour,   /**< Colour for the meter bar under the warn threshold */
		lmMeterGradientMidColour,   /**< Colour for the meter bar in the warn area */
		lmMeterGradientMaxColour,   /**< Colour for the meter bar at the clip threshold */
		lmMeterReductionColour      /**< Colour for the reduction meter displayed within the meter */
	};

	/**
	These methods define a interface for the LookAndFeel class of juce.
	The VASTVUMeter needs a LookAndFeel, that implements these methods.
	There is a default implementation to be included in your custom LookAndFeel class, \see LookAndFeelMethods.h
	*/
	class LookAndFeelMethods {
	public:
		virtual ~LookAndFeelMethods() {}

		/** Define your default colours in this callback */
		virtual void setupDefaultMeterColours() = 0;

		/** Call this to create the cached ColourGradients after changing colours of the meter gradients */
		virtual void updateMeterGradients() = 0;

		/** Override this to change the inner juce::Rectangle in case you want to paint a border e.g. */
		virtual juce::Rectangle<float> getMeterInnerBounds(const juce::Rectangle<float> bounds,
			const MeterFlags meterType) const = 0;

		/** Override this callback to define the placement of a meter channel. */
		virtual juce::Rectangle<float> getMeterBounds(const juce::Rectangle<float> bounds,
			const MeterFlags meterType,
			const int numChannels,
			const int channel) const = 0;

		/** Override this callback to define the placement of the actual meter bar. */
		virtual juce::Rectangle<float> getMeterBarBounds(const juce::Rectangle<float> bounds,
			const MeterFlags meterType) const = 0;

		/** Override this callback to define the placement of the tickmarks.
		To disable this feature return an empty juce::Rectangle. */
		virtual juce::Rectangle<float> getMeterTickmarksBounds(const juce::Rectangle<float> bounds,
			const MeterFlags meterType) const = 0;

		/** Override this callback to define the placement of the clip indicator light.
		To disable this feature return an empty juce::Rectangle. */
		virtual juce::Rectangle<float> getMeterClipIndicatorBounds(const juce::Rectangle<float> bounds,
			const MeterFlags meterType) const = 0;


		/** Override this to draw background and if wanted a frame. If the frame takes space away,
		it should return the reduced bounds */
		virtual juce::Rectangle<float> drawBackground(juce::Graphics&,
			const MeterFlags meterType,
			const juce::Rectangle<float> bounds) = 0;

		/** This is called to draw the actual numbers and bars on top of the static background */
		virtual void drawMeterBars(juce::Graphics&,
			const MeterFlags meterType,
			const juce::Rectangle<float> bounds,
			const VASTVUMeterSource* source,
			const int fixedNumChannels = -1,
			const int selectedChannel = -1) = 0;

		/** This draws the static background of the whole level meter group with all channels */
		virtual void drawMeterBarsBackground(juce::Graphics&,
			const MeterFlags meterType,
			const juce::Rectangle<float> bounds,
			const int numChannels,
			const int fixedNumChannels = -1) = 0;

		/** This draws a group of informations representing one channel */
		virtual void drawMeterChannel(juce::Graphics&,
			const MeterFlags meterType,
			const juce::Rectangle<float> bounds,
			const VASTVUMeterSource* source,
			const int selectedChannel) = 0;

		/** This draws the static backgrounds representing one channel */
		virtual void drawMeterChannelBackground(juce::Graphics&,
			const MeterFlags meterType,
			const juce::Rectangle<float> bounds) = 0;

		/** This callback draws the actual level bar. The background has an extra callback */
		virtual void drawMeterBar(juce::Graphics&,
			const MeterFlags meterType,
			const juce::Rectangle<float> bounds,
			const float rms, const float peak) = 0;

		/** This callback draws an reduction from top. Only triggered, if a reduction < 1.0 is set in the VASTVUMeterSource */
		virtual void drawMeterReduction(juce::Graphics& g,
			const VASTVUMeter::MeterFlags meterType,
			const juce::Rectangle<float> bounds,
			const float reduction) = 0;

		/** This draws the background for the actual level bar */
		virtual void drawMeterBarBackground(juce::Graphics&,
			const MeterFlags meterType,
			const juce::Rectangle<float> bounds) = 0;

		/** This draws the tickmarks for the level scale. It is painted on the static background */
		virtual void drawTickMarks(juce::Graphics&,
			const MeterFlags meterType,
			const juce::Rectangle<float> bounds) = 0;

		/** This callback draws the clip indicator. The background has an extra callback */
		virtual void drawClipIndicator(juce::Graphics&,
			const MeterFlags meterType,
			const juce::Rectangle<float> bounds,
			const bool hasClipped) = 0;

		/** This draws the background for the clip indicator LED */
		virtual void drawClipIndicatorBackground(juce::Graphics&,
			const MeterFlags meterType,
			const juce::Rectangle<float> bounds) = 0;

		/** Override this callback to define the placement of the max level.
		To disable this feature return an empty juce::Rectangle. */
		virtual juce::Rectangle<float> getMeterMaxNumberBounds(const juce::Rectangle<float> bounds,
			const MeterFlags meterType) const = 0;

		/** This callback draws the number of maximum level. The background has an extra callback */
		virtual void drawMaxNumber(juce::Graphics&,
			const MeterFlags meterType,
			const juce::Rectangle<float> bounds,
			const float maxGain) = 0;

		/** This draws the background for the maximum level display */
		virtual void drawMaxNumberBackground(juce::Graphics&,
			const MeterFlags meterType,
			const juce::Rectangle<float> bounds) = 0;

		/** This is called by the frontend to check, if the clip indicator was clicked (e.g. for reset) */
		virtual int hitTestClipIndicator(const juce::Point<int> position,
			const MeterFlags meterType,
			const juce::Rectangle<float> bounds,
			const VASTVUMeterSource* source) const = 0;

		/** This is called by the frontend to check, if the maximum level number was clicked (e.g. for reset) */
		virtual int hitTestMaxNumber(const juce::Point<int> position,
			const MeterFlags meterType,
			const juce::Rectangle<float> bounds,
			const VASTVUMeterSource* source) const = 0;
	};

	VASTVUMeter(const MeterFlags type = HasBorder);
	~VASTVUMeter();

	/**
	Allows to change the meter's configuration by setting a combination of MeterFlags
	*/
	void setMeterFlags(const MeterFlags type);

	void paint(juce::Graphics&) override;

	void resized() override;

	void visibilityChanged() override;

	void timerCallback() override;

	/**
	Set a VASTVUMeterSource to display. This separation is used, so the source can work in the processing and the
	GUI can display the values.
	*/
	void setMeterSource(VASTVUMeterSource* source);

	/**
	Set a specific channel to display. This is only useful, if MeterFlags::SingleChannel is set.
	*/
	void setSelectedChannel(const int c);

	/**
	If you don't know, how many channels will be in the processblock, you can set this number to avoid stretching
	the width of the channels.
	*/
	void setFixedNumChannels(const int numChannels);

	void setRefreshRateHz(const int newRefreshRate);

	/**
	Unset the clip indicator flag for a channel. Use -1 to reset all clip indicators.
	*/
	void clearClipIndicator(const int channel = -1);

	/**
	Set the max level display back to -inf for a channel. Use -1 to reset all max levels.
	*/
	void clearMaxLevelDisplay(const int channel = -1);
	
private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VASTVUMeter)

	juce::WeakReference<VASTVUMeterSource> source;
	int                                   selectedChannel;
	int                                   fixedNumChannels;
	MeterFlags                            meterType;
	int                                   refreshRate;
	bool                                  useBackgroundImage;
	juce::Image                           backgroundImage;
	bool                                  backgroundNeedsRepaint;
};

#endif