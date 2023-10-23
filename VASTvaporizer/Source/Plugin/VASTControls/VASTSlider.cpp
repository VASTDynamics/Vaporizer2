
/*
VAST Dynamics
*/

#include "../../Engine/VASTEngineHeader.h"
#include "VASTSlider.h"
#include "../VASTAudioProcessor.h"
#include "../VASTAudioProcessorEditor.h"

VASTSlider::VASTSlider(const juce::String &componentName) {
	this->setName(componentName);
	this->setComponentID(componentName);
	this->addMouseListener(&vastPopupHandler, true);
	vastPopupHandler.setComponentName(this->getName());	

	setRepaintsOnMouseActivity(true); //performance
	setBufferedToImage(true); //performance

	setDefaultValue(getMinimum());
	m_editor = NULL;
	m_processor = NULL;
}

VASTSlider::~VASTSlider() {
	this->setLookAndFeel(nullptr);
}
void VASTSlider::setEditor(VASTAudioProcessorEditor &editor) {
	m_editor = &editor;
}
VASTAudioProcessorEditor* VASTSlider::getEditor() {
	return m_editor;
}
void VASTSlider::setAudioProcessor(VASTAudioProcessor &processor) {
	m_processor = &processor;
}
VASTAudioProcessor* VASTSlider::getAudioProcessor() {
	return m_processor;
}

void VASTSlider::setDefaultValue(float defaultVal) {
	hasDefaultValue = true;
	mDefaultVal = defaultVal;
	setDoubleClickReturnValue(true, mDefaultVal);
}

void VASTSlider::mouseDown(const MouseEvent &e) {
	if (e.getNumberOfClicks() > 1) { //double click shall reset to default
		if (getSliderStyle() != SliderStyle::IncDecButtons) { //no doubleclick!
			if (hasDefaultValue) {
				setValue(mDefaultVal, juce::NotificationType::sendNotificationAsync);
			}
		}
	}

	Slider::mouseDown(e);
}
