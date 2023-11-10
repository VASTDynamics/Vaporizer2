/*
VAST Dynamics
*/

#include "../../Engine/VASTEngineHeader.h"
#include "VASTImageButton.h"
#include "../VASTAudioProcessorEditor.h"

VASTImageButton::VASTImageButton(const juce::String &componentName) {
	this->setName(componentName);
	this->setComponentID(componentName);
	vastPopupHandler.setComponentName(componentName);
	this->addMouseListener(&vastPopupHandler, true);

	setRepaintsOnMouseActivity(true); //performance
	setBufferedToImage(true); //performance

	m_processor = NULL;
}

VASTImageButton::~VASTImageButton() {
	this->setLookAndFeel(nullptr);
}

void VASTImageButton::setAudioProcessor(VASTAudioProcessor &processor) {
	m_processor = &processor;
}
VASTAudioProcessor* VASTImageButton::getAudioProcessor() {
	return m_processor;
}

void VASTImageButton::mouseDrag(const MouseEvent &e) {
	if (m_processor == nullptr) 
		return; //not draggable
	VASTAudioProcessorEditor* editor = (VASTAudioProcessorEditor*)m_processor->getActiveEditor();
	if (editor == nullptr) 
		return;
	Image img = this->getOverImage().rescaled(32, 32, juce::Graphics::ResamplingQuality::highResamplingQuality);
	Image result = img.createCopy();	
	Graphics g(result);
	g.setColour(Colours::transparentBlack);
	g.fillAll();
	g.setColour(m_processor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colDragSource));
	g.drawImage(img, result.getBounds().getSmallestIntegerContainer().toFloat(), RectanglePlacement::onlyReduceInSize, true);

	editor->vaporizerComponent->startDragging(getName(), this, (ScaledImage)result, false);
	ImageButton::mouseDrag(e);
}

/*
void VASTImageButton::mouseDown(const MouseEvent &e) {
	ImageButton::mouseDown(e);
}
*/
