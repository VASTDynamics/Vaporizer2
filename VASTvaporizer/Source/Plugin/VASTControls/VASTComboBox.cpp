/*
VAST Dynamics
*/

#include "../../Engine/VASTEngineHeader.h"
#include "VASTComboBox.h"

VASTComboBox::VASTComboBox(const juce::String &componentName) {
	this->setName(componentName);
	vastPopupHandler.setComponentName(componentName);
	this->setComponentID(componentName);
	this->addMouseListener(&vastPopupHandler, true);

	setRepaintsOnMouseActivity(true); //performance
	setBufferedToImage(true); //performance

	m_editor = NULL;
	m_processor = NULL;
}

VASTComboBox::~VASTComboBox() {
	this->setLookAndFeel(nullptr);
}

void VASTComboBox::setEditor(VASTAudioProcessorEditor &editor) {
	m_editor = &editor;
}
VASTAudioProcessorEditor* VASTComboBox::getEditor() {
	return m_editor;
}
void VASTComboBox::setAudioProcessor(VASTAudioProcessor &processor) {
	m_processor = &processor;
}
VASTAudioProcessor* VASTComboBox::getAudioProcessor() {
	return m_processor;
}
