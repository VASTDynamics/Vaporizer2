
/*
VAST Dynamics
*/

#include "../../Engine/VASTEngineHeader.h"
#include "VASTPitchbendSlider.h"
#include "../VASTAudioProcessor.h"
#include "../VASTAudioProcessorEditor.h"

VASTPitchbendSlider::VASTPitchbendSlider(const juce::String &componentName) {
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

VASTPitchbendSlider::~VASTPitchbendSlider() {
	this->setLookAndFeel(nullptr);
}
void VASTPitchbendSlider::setEditor(VASTAudioProcessorEditor &editor) {
	m_editor = &editor;
}
VASTAudioProcessorEditor* VASTPitchbendSlider::getEditor() {
	return m_editor;
}
void VASTPitchbendSlider::setAudioProcessor(VASTAudioProcessor &processor) {
	m_processor = &processor;
}
VASTAudioProcessor* VASTPitchbendSlider::getAudioProcessor() {
	return m_processor;
}

void VASTPitchbendSlider::setDefaultValue(float defaultVal) {
	hasDefaultValue = true;
	mDefaultVal = defaultVal;
	setDoubleClickReturnValue(true, mDefaultVal);
}

void VASTPitchbendSlider::mouseDown(const MouseEvent &e) {
	if (e.getNumberOfClicks() > 1) { //double click shall reset to default
		if (hasDefaultValue) {
			setValue(mDefaultVal, juce::NotificationType::sendNotificationAsync);
		}
	}

	Slider::mouseDown(e);
}

void VASTPitchbendSlider::mouseExit(const MouseEvent &) {
	if (hasDefaultValue) {
		setValue(mDefaultVal, juce::NotificationType::sendNotificationAsync);
	}
}

void VASTPitchbendSlider::mouseUp(const MouseEvent &) {
	if (hasDefaultValue) {
		setValue(mDefaultVal, juce::NotificationType::sendNotificationAsync);
	}
}
