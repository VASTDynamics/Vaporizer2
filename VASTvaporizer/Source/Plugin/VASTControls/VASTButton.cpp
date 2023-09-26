/*
VAST Dynamics
*/

#include "../../Engine/VASTEngineHeader.h"
#include "VASTButton.h"

VASTButton::VASTButton(const juce::String &componentName) {
	this->setName(componentName);
	this->setComponentID(componentName);
	vastPopupHandler.setComponentName(componentName);
	this->addMouseListener(&vastPopupHandler, true);

	setRepaintsOnMouseActivity(true); //performance
	setBufferedToImage(true); //performance

	m_editor = NULL;
	m_processor = NULL;
}

VASTButton::~VASTButton() {
	this->setLookAndFeel(nullptr);
}

void VASTButton::setEditor(VASTAudioProcessorEditor &editor) {
	m_editor = &editor;
}
VASTAudioProcessorEditor* VASTButton::getEditor() {
	return m_editor;
}
void VASTButton::setAudioProcessor(VASTAudioProcessor &processor) {
	m_processor = &processor;
}
VASTAudioProcessor* VASTButton::getAudioProcessor() {
	return m_processor;
}