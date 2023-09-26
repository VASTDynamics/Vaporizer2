/*
VAST Dynamics
*/

#include "../../Engine/VASTEngineHeader.h"
#include "VASTDrawableButton.h"
#include "../VASTAudioProcessorEditor.h"

VASTDrawableButton::VASTDrawableButton(const juce::String &componentName, const char* normalImage, const int normalImageSize, const juce::String tooltip) : juce::DrawableButton(componentName, ButtonStyle::ImageOnButtonBackground) {
	this->setName(componentName);
	this->setComponentID(componentName);
	this->setTooltip(tooltip);
	//vastPopupHandler.setComponentName(componentName);
	//this->addMouseListener(&vastPopupHandler, true);

	std::unique_ptr<Drawable> image = Drawable::createFromImageData(normalImage, normalImageSize);
	setImages(image.get(), image.get(), image.get(), image.get(), image.get(), image.get(), image.get(), image.get());
	//setRepaintsOnMouseActivity(false); //performance
	//setBufferedToImage(true); //performance

	setOpaque(false);

	m_processor = NULL;
}

VASTDrawableButton::~VASTDrawableButton() {
	this->setLookAndFeel(nullptr);
}

void VASTDrawableButton::setAudioProcessor(VASTAudioProcessor &processor) {
	m_processor = &processor;
}
VASTAudioProcessor* VASTDrawableButton::getAudioProcessor() {
	return m_processor;
}

