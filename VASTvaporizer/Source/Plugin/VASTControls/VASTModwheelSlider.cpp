
/*
VAST Dynamics
*/

#include "../../Engine/VASTEngineHeader.h"
#include "VASTModwheelSlider.h"
#include "../VASTAudioProcessor.h"
#include "../VASTAudioProcessorEditor.h"

VASTModwheelSlider::VASTModwheelSlider(const juce::String &componentName) {
	this->setName(componentName);
	this->setComponentID(componentName);
	this->addMouseListener(&vastPopupHandler, true);
	vastPopupHandler.setComponentName(this->getName());	

	setRepaintsOnMouseActivity(true); //performance
	setBufferedToImage(true); //performance

	setSliderStyle(Slider::SliderStyle::LinearVertical);
	setTextBoxStyle(Slider::NoTextBox, true, 0, 0);

	setDefaultValue(getMinimum());
	m_editor = NULL;
	m_processor = NULL;
}

VASTModwheelSlider::~VASTModwheelSlider() {
	this->setLookAndFeel(nullptr);
}
void VASTModwheelSlider::setEditor(VASTAudioProcessorEditor &editor) {
	m_editor = &editor;
}
VASTAudioProcessorEditor* VASTModwheelSlider::getEditor() {
	return m_editor;
}
void VASTModwheelSlider::setAudioProcessor(VASTAudioProcessor &processor) {
	m_processor = &processor;
}
VASTAudioProcessor* VASTModwheelSlider::getAudioProcessor() {
	return m_processor;
}

void VASTModwheelSlider::setDefaultValue(float defaultVal) {
	hasDefaultValue = true;
	mDefaultVal = defaultVal;
	setDoubleClickReturnValue(true, mDefaultVal);
}

void VASTModwheelSlider::mouseDown(const MouseEvent &e) {
	if (e.getNumberOfClicks() > 1) { //double click shall reset to default
		if (hasDefaultValue) {
			setValue(mDefaultVal, juce::NotificationType::sendNotificationAsync);
		}
	}

	Slider::mouseDown(e);
}

