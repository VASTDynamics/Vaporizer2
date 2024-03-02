/*
VAST Dynamics
*/

#include "../../Engine/VASTEngineHeader.h"
#include "VASTParameterButton.h"
#include "../VASTAudioProcessor.h"
#include "../VASTAudioProcessorEditor.h"

VASTParameterButton::VASTParameterButton(const juce::String &componentName) {
	this->setComponentID("");
	this->setName(componentName);
	//this->setComponentID(componentName); //set later
	vastPopupHandler.setComponentName(componentName);
	this->addMouseListener(&vastPopupHandler, true);

	setRepaintsOnMouseActivity(true); //performance
	setBufferedToImage(true); //performance

	m_processor = NULL;
}

VASTParameterButton::~VASTParameterButton() {
	this->setLookAndFeel(nullptr);
	buttonAttachment = nullptr;
}

void VASTParameterButton::bindParameter(VASTAudioProcessorEditor* editor, const String& newID, VASTGUIRuntimeModel::GUIComponents guiComponent, int tabNo) {
	ToggleButton::setComponentID(newID); //call super
	if (m_processor != nullptr) {
		buttonAttachment.reset(new ButtonAttachment(m_processor->getParameterTree(), this->getComponentID(), (juce::ToggleButton&)*this));

		auto* param = m_processor->getParameterTree().getParameter(newID);
		if (param == nullptr) {
			vassert(false);
			return;
		}
        setTooltip(m_processor->getParameters()[param->getParameterIndex()]->getName(1024));
		//setTooltip(m_processor->getParameters()[param->getParameterIndex()]->getLabel());        
	}
	else
		vassert(m_processor != nullptr);
}

void VASTParameterButton::setAudioProcessor(VASTAudioProcessor &processor) {
	m_processor = &processor;	
}

VASTAudioProcessor* VASTParameterButton::getAudioProcessor() {
	return m_processor;
}
