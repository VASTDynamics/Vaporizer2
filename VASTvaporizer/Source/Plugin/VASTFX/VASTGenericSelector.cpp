/*
VAST Dynamics Audio Software (TM)
*/

#include "../../Engine/VASTEngineHeader.h"
#include "VASTGenericSelector.h"
#include "../VASTControls/VASTParameterSlider.h"
#include "../VASTControls/VASTParameterButton.h"
#include "../VASTControls/VASTParameterComboBox.h"
#include "../VASTAudioProcessor.h"
#include "../VASTAudioProcessorEditor.h"
#include "../../Engine/FX/VASTEffect.h"
#include "../VASTUtils/VASTGUINotifier.h"
#include "../../Engine/VASTFXBus.h"
#include "../VASTControls/VASTDragFX.h"

//==============================================================================

VASTGenericSelector::VASTGenericSelector(VASTAudioProcessorEditor *editor, VASTAudioProcessor *processor, int busnr, VASTGUINotifier* guinotifier) {
	my_processor = processor;
	my_editor = editor;
	myBusnr = busnr;
	myGUINotifier = guinotifier;

	VASTAudioProcessor* vastprocessor = (VASTAudioProcessor *)processor;

	CVASTFXBus* fxBus;
	switch (myBusnr) {
	case 0:
		fxBus = &vastprocessor->m_pVASTXperience.m_fxBus1;
		break;
	case 1:
		fxBus = &vastprocessor->m_pVASTXperience.m_fxBus2;
		break;
	case 2:
		fxBus = &vastprocessor->m_pVASTXperience.m_fxBus3;
		break;
    default:
        jassert(false);
        return;
	}

	setSize(200, 900);

	for (int i = 0; i < fxBus->effectBus.size(); i++) {
		
		String paramID = fxBus->effectBus[i]->effectPlugin->getOnOffParameter();
		VASTDragFX* dragFX = new VASTDragFX(my_editor, my_processor, myBusnr, i, paramID, guinotifier);
		addAndMakeVisible(dragFX);
		dragFXArray.add(dragFX);
	}
}

VASTGenericSelector::~VASTGenericSelector() {
	this->setLookAndFeel(nullptr);
}

void VASTGenericSelector::buttonClicked(Button* b) { //needed for listener interface
	String compID = b->getComponentID();
	if (!compID.equalsIgnoreCase(""))
		myGUINotifier->shallUpdateGUI(compID, b->getToggleState());
}

void VASTGenericSelector::buttonStateChanged(Button* b) {
	String compID = b->getComponentID();
	if (!compID.equalsIgnoreCase(""))
		myGUINotifier->shallUpdateGUI(compID, b->getToggleState());
}
	
void VASTGenericSelector::updateContent() {
	float insideWidth = getWidth();
	float insideHeight = getHeight();

	CVASTFXBus* fxBus;
	switch (myBusnr) {
	case 0:
		fxBus = &my_processor->m_pVASTXperience.m_fxBus1;
		break;
	case 1:
		fxBus = &my_processor->m_pVASTXperience.m_fxBus2;
		break;
	case 2:
		fxBus = &my_processor->m_pVASTXperience.m_fxBus3;
		break;
	default:
		jassert(false);
		return;
	}

	for (auto i = 0; i < dragFXArray.size(); ++i)
	{
		int fxno = fxBus->getSequence(i);
		dragFXArray[fxno]->setBounds(0, insideHeight / 20.f * i, insideWidth, insideHeight / 24.f); //check	
	}
}

void VASTGenericSelector::resized()
{
	updateContent();
}

void VASTGenericSelector::paint(Graphics& g) 
{
}
