/*
VAST Dynamics
*/

#include "../../Engine/VASTEngineHeader.h"
#include "VASTComboBrev.h"

VASTComboBrev::VASTComboBrev(const juce::String &componentName) {
	this->setName(componentName);
//		ComboBox(componentName);
//		addListener(this);
	addAndMakeVisible(briefLabel = new Label());
	briefLabel->setMinimumHorizontalScale(.5f);
	briefLabel->setJustificationType(juce::Justification::left);
	vastPopupHandler.setComponentName(componentName);
	this->addMouseListener(&vastPopupHandler, true);
	this->setComponentID(componentName);

	setRepaintsOnMouseActivity(true); //performance
	setBufferedToImage(true); //performance

	m_editor = NULL;
	m_processor = NULL;
}

VASTComboBrev::~VASTComboBrev() {
	briefLabel->setLookAndFeel(nullptr);
	delete briefLabel;
	this->setLookAndFeel(nullptr);
}

void VASTComboBrev::resized() {
	ComboBox::resized();
	briefLabel->setTooltip(this->getTooltip());
	briefLabel->setComponentID(this->getComponentID());
	if (getHeight() > 0 && getWidth() > 0) {
		getLookAndFeel().positionComboBoxText(*this, *briefLabel);
		briefLabel->setBounds(2, 2, getWidth() - 4, getHeight() - 4);
	}
}

void VASTComboBrev::setEditor(VASTAudioProcessorEditor &editor) {
	m_editor = &editor;
}
VASTAudioProcessorEditor* VASTComboBrev::getEditor() {
	return m_editor;
}
void VASTComboBrev::setAudioProcessor(VASTAudioProcessor &processor) {
	m_processor = &processor;
}
VASTAudioProcessor* VASTComboBrev::getAudioProcessor() {
	return m_processor;
}