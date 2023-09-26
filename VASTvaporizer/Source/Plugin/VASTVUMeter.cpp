/*
VAST Dynamics
*/

#include "../Engine/VASTEngineHeader.h"
#include "VASTVUMeter.h"
#include "VASTVUMeterSource.h"
#include "VASTUtils/VASTLookAndFeel.h"

VASTVUMeter::VASTVUMeter(const MeterFlags type)
	: source(nullptr),
	selectedChannel(-1),
	fixedNumChannels(-1),
	meterType(type),
	refreshRate(10), //10hz = 100ms
	useBackgroundImage(false),
	backgroundNeedsRepaint(true)
{
	startTimerHz(refreshRate);
	setOpaque(true);
	setRepaintsOnMouseActivity(false); //performance
	setBufferedToImage(false); //performance
}

VASTVUMeter::~VASTVUMeter()
{
	stopTimer();
}

void VASTVUMeter::setMeterFlags(const MeterFlags type)
{
	meterType = type;
}

void VASTVUMeter::setMeterSource(VASTVUMeterSource* src)
{
	source = src;
}

void VASTVUMeter::setSelectedChannel(const int c)
{
	selectedChannel = c;
}

void VASTVUMeter::setFixedNumChannels(const int numChannels)
{
	fixedNumChannels = numChannels;
}

void VASTVUMeter::setRefreshRateHz(const int newRefreshRate)
{
	refreshRate = newRefreshRate;
	startTimerHz(refreshRate);
}

void VASTVUMeter::paint(Graphics& g)
{
	Graphics::ScopedSaveState saved(g);

	if (VASTLookAndFeel* lnf = dynamic_cast<VASTLookAndFeel*> (&getLookAndFeel())) {
		const juce::Rectangle<float> bounds = getLocalBounds().toFloat();
		int numChannels = source ? source->getNumChannels() : 1;
		if (useBackgroundImage) {
			// This seems to only speed up, if you use complex drawings on the background. For
			// "normal" vector graphics the image approach seems actually slower
			if (backgroundNeedsRepaint) {
				backgroundImage = Image(Image::ARGB, getWidth(), getHeight(), true);
				Graphics backGraphics(backgroundImage);
				lnf->drawBackground(backGraphics, meterType, bounds);
				lnf->drawMeterBarsBackground(backGraphics, meterType, bounds, numChannels, fixedNumChannels);
				backgroundNeedsRepaint = false;
			}
			g.drawImageAt(backgroundImage, 0, 0);
			lnf->drawMeterBars(g, meterType, bounds, source, fixedNumChannels, selectedChannel);
		}
		else {
			lnf->drawBackground(g, meterType, bounds);
			lnf->drawMeterBarsBackground(g, meterType, bounds, numChannels, fixedNumChannels);
			lnf->drawMeterBars(g, meterType, bounds, source, fixedNumChannels, selectedChannel);
		}
	}
	else {
		// This LookAndFeel is missing the VASTVUMeter::LookAndFeelMethods.
		// If you work with the default LookAndFeel, set an instance of
		// VASTVUMeterLookAndFeel as LookAndFeel of this component.
		//
		// If you write a LookAndFeel from scratch, inherit also
		// VASTVUMeter::LookAndFeelMethods
		//jassertfalse;
	}

	if (source)
		source->decayIfNeeded();
}

void VASTVUMeter::resized()
{
	LookAndFeel& l = getLookAndFeel();
	if (VASTLookAndFeel* lnf = dynamic_cast<VASTLookAndFeel*> (&l)) {
		lnf->updateMeterGradients();
	}

	backgroundNeedsRepaint = true;
}
void VASTVUMeter::visibilityChanged()
{
	if (this->isVisible()) { //skip on close
		backgroundNeedsRepaint = true;
	}
}

void VASTVUMeter::timerCallback()
{
	LookAndFeel& l = getLookAndFeel();
	if (VASTLookAndFeel* lnf = dynamic_cast<VASTLookAndFeel*> (&l)) {
		int numChannels = source ? source->getNumChannels() : 1;
		for (int i = 0; i < numChannels; ++i) {
			auto channelBounds = lnf->getMeterBounds(getLocalBounds().toFloat(), meterType, numChannels, i);
			repaint(lnf->getMeterBarBounds(channelBounds, meterType).toNearestInt());
			repaint(lnf->getMeterMaxNumberBounds(channelBounds, meterType).toNearestInt());
			repaint(lnf->getMeterClipIndicatorBounds(channelBounds, meterType).toNearestInt());
		}
	}
	else
		repaint();
}

void VASTVUMeter::clearClipIndicator(const int channel)
{
	if (source) {
		if (channel < 0) {
			source->clearAllClipFlags();
		}
		else {
			source->clearClipFlag(channel);
		}
	}
}

void VASTVUMeter::clearMaxLevelDisplay(const int channel)
{
	if (source) {
		if (channel < 0) {
			source->clearAllMaxNums();
		}
		else {
			source->clearMaxNum(channel);
		}
	}
}

