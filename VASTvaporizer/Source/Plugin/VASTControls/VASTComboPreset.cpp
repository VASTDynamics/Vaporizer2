/*
VAST Dynamics
*/

#include "../../Engine/VASTEngineHeader.h"
#include "VASTComboPreset.h"
#include "../VASTAudioProcessor.h"
#include "../VASTAudioProcessorEditor.h"
#include "../VASTPreset/VASTPresetComponent.h"

VASTComboPreset::VASTComboPreset(const juce::String &componentName) {
	this->setName(componentName);

	setRepaintsOnMouseActivity(true); //performance
	setBufferedToImage(true); //performance
}	

VASTComboPreset::~VASTComboPreset() {
	this->setLookAndFeel(nullptr);
	if (m_presetComponent != nullptr)
		delete m_presetComponent;
}

void VASTComboPreset::setAudioProcessor(VASTAudioProcessor &processor) {
	m_processor = &processor;
}

void VASTComboPreset::removePopup() {
	if (m_presetComponent == nullptr)
		return;
	VASTAudioProcessorEditor* editor = (VASTAudioProcessorEditor*)m_processor->getActiveEditor();

	editor->vaporizerComponent->getConcertinaPanel()->removePresetBroswerOverlay();		
	hidePopup();

	editor->vaporizerComponent->getOscillatorComponent(0)->getAudioOscilloscope()->setToOpenGLRender();
	editor->vaporizerComponent->getOscillatorComponent(1)->getAudioOscilloscope()->setToOpenGLRender();
	editor->vaporizerComponent->getOscillatorComponent(2)->getAudioOscilloscope()->setToOpenGLRender();
	editor->vaporizerComponent->getOscillatorComponent(3)->getAudioOscilloscope()->setToOpenGLRender();
	editor->vaporizerComponent->getWaveTableEditorComponent()->getEditorView()->getEditorOscilloscope()->setToOpenGLRender();
}

void VASTComboPreset::mouseDown(const MouseEvent &e) {
	if (m_presetComponent != nullptr)
		if (m_presetComponent->isShowing()) {
			removePopup();
			return;
		}
	ComboBox::mouseDown(e);
}

void VASTComboPreset::showPopup() {	
	if (m_processor == nullptr) return;
	if (m_presetComponent == nullptr)
		m_presetComponent = new VASTPresetComponent(m_processor, this); 
	else 
		m_presetComponent->updateAll();
	m_presetComponent->setLookAndFeel(m_processor->getCurrentVASTLookAndFeel());
	
	VASTAudioProcessorEditor* editor = (VASTAudioProcessorEditor*)m_processor->getActiveEditor();

	editor->vaporizerComponent->getOscillatorComponent(0)->getAudioOscilloscope()->setToStandardRender();
	editor->vaporizerComponent->getOscillatorComponent(1)->getAudioOscilloscope()->setToStandardRender();
	editor->vaporizerComponent->getOscillatorComponent(2)->getAudioOscilloscope()->setToStandardRender();
	editor->vaporizerComponent->getOscillatorComponent(3)->getAudioOscilloscope()->setToStandardRender();
	editor->vaporizerComponent->getWaveTableEditorComponent()->getEditorView()->getEditorOscilloscope()->setToStandardRender();
	
	//int width = editor->vaporizerComponent->getConcertinaPanel()->getLocalBounds().getWidth();
	//int height = editor->vaporizerComponent->getConcertinaPanel()->getLocalBounds().getHeight();	
	editor->vaporizerComponent->getConcertinaPanel()->addPresetBroswerOverlay(m_presetComponent);
	m_presetComponent->setBoundsInset(BorderSize<int>());

	/*
	Point<int> pnt;
	if (getParentComponent() == nullptr) return;
	pnt = getParentComponent()->getBoundsInParent().getTopLeft();
	pnt.addXY(this->getBoundsInParent().getCentreX(), this->getBoundsInParent().getY() + this->getHeight());
	Rectangle<int> rect = juce::Rectangle<int>(pnt, pnt);
	//CallOutBox &cb = juce::CallOutBox::launchAsynchronously(presetComponent, rect, getParentComponent()->getParentComponent());	
	CallOutBox &cb = juce::CallOutBox::launchAsynchronously(presetComponent, rect, editor->vaporizerComponent->getConcertinaPanel()->getFirstComponent());
	
	cb.setBoundsInset(BorderSize<int>());
	cb.runModalLoop(); //this is deprecated TODO
	hidePopup();
	if (getParentComponent() == nullptr) return;
	getParentComponent()->getParentComponent()->grabKeyboardFocus();
	*/
}

