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

    m_normalImage = normalImage;
    m_normalImageSize = normalImageSize;
    auto asyncImageLoad = [p = juce::Component::SafePointer<VASTDrawableButton> (this)]()
    {
        if (p) {
            std::unique_ptr<Drawable> image = Drawable::createFromImageData(p->m_normalImage, p->m_normalImageSize);
            p->setImages(image.get(), nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
        }
    };
    
	//setRepaintsOnMouseActivity(false); //performance
	//setBufferedToImage(true); //performance

	setOpaque(false);
    Timer::callAfterDelay (200, asyncImageLoad);
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

