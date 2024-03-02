/*
VAST Dynamics
*/

#include "../../Engine/VASTEngineHeader.h"
#include "VASTDragFX.h"
#include "../VASTAudioProcessor.h"
#include "../VASTAudioProcessorEditor.h"
#include "../../Engine/FX/VASTEffect.h"
#include "../VASTUtils/VASTGUINotifier.h"
#include "../../Engine/VASTFXBus.h"

VASTDragFX::VASTDragFX(VASTAudioProcessorEditor *editor, VASTAudioProcessor *processor, int busnr, int effectno, String paramID, VASTGUINotifier* guinotifier) {
	my_processor = processor;
	my_editor = editor;
	myEffectno = effectno;
	myBusnr = busnr;
	myGUINotifier = guinotifier;

	//setRepaintsOnMouseActivity(false); //performance
	//setBufferedToImage(true); //performance

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

	String componentName = "FXDragButton" + String(effectno);
	ddImageButton.reset(new VASTImageButton(componentName));
	addAndMakeVisible(ddImageButton.get());
	ddImageButton->setAudioProcessor(*my_processor);
	ddImageButton->setImages(false, true, true,
		ImageCache::getFromMemory(VASTHeaderComponent::move_grabber_png, VASTHeaderComponent::move_grabber_pngSize), 1.000f, Colour(0x451c6d2f),
		ImageCache::getFromMemory(VASTHeaderComponent::move_grabber_png, VASTHeaderComponent::move_grabber_pngSize), 1.000f, Colour(0x00000000),
		ImageCache::getFromMemory(VASTHeaderComponent::move_grabber_png, VASTHeaderComponent::move_grabber_pngSize), 1.000f, Colour(0x00000000));

	String ename = fxBus->effectBus[effectno]->effectPlugin->getEffectName();
	ddLabel.reset(new Label("new label", ename));
	addAndMakeVisible(ddLabel.get());
	ddLabel->setJustificationType(Justification::centred);
	ddLabel->setEditable(false, false, false);

	ddToggleButton.reset(new VASTParameterButton(paramID));
	ddToggleButton->setAudioProcessor(*my_processor);
	ddToggleButton->bindParameter(editor, paramID, VASTGUIRuntimeModel::GUIComponents::DragFX, myBusnr);
	ddToggleButton->addListener(this);
	addAndMakeVisible(ddToggleButton.get());
}

void VASTDragFX::resized() {
	ddToggleButton->setBounds(proportionOfWidth(0.f), proportionOfHeight(0.05f), proportionOfWidth(0.2f), proportionOfHeight(0.9f));
	ddLabel->setBounds(proportionOfWidth(0.2f), proportionOfHeight(0.2f), proportionOfWidth(0.6f), proportionOfHeight(0.6f));
	ddImageButton->setBounds(proportionOfWidth(0.8f), proportionOfHeight(0.05f), proportionOfWidth(0.2f), proportionOfHeight(0.9f));
}

void VASTDragFX::lookAndFeelChanged() {
	if (my_processor == nullptr) return;
	Colour c1 = my_processor->getCurrentVASTLookAndFeel()->findVASTColour(colGroupComponentOutline).withAlpha(1.0f).contrasting(0.5f);
	ddImageButton->setImages(false, true, true,
		ImageCache::getFromMemory(VASTHeaderComponent::move_grabber_png, VASTHeaderComponent::move_grabber_pngSize), 1.000f, c1,
		ImageCache::getFromMemory(VASTHeaderComponent::move_grabber_png, VASTHeaderComponent::move_grabber_pngSize), 1.000f, c1.contrasting(0.3f),
		ImageCache::getFromMemory(VASTHeaderComponent::move_grabber_png, VASTHeaderComponent::move_grabber_pngSize), 1.000f, c1.contrasting(0.5f));
}

void VASTDragFX::paint(Graphics& g)
{
	if (my_processor == nullptr) return;
	Colour c1 = my_processor->getCurrentVASTLookAndFeel()->findVASTColour(colOscillatorComponentHeaderGradientFrom).withAlpha(0.4f);
	Colour c2 = my_processor->getCurrentVASTLookAndFeel()->findVASTColour(colOscillatorComponentHeaderGradientTo).withAlpha(0.4f);
	Colour c3 = my_processor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colGroupComponentOutline);
	g.setGradientFill(ColourGradient(c1,
		proportionOfWidth(0),
		proportionOfHeight(0),
		c2,
		proportionOfWidth(1),
		proportionOfHeight(1),
		false));
	g.fillRect(0, 0, getWidth(), getHeight());
	g.setColour(c3);
	g.drawRoundedRectangle(proportionOfWidth(0.f), proportionOfHeight(0.f), proportionOfWidth(1.f), proportionOfHeight(1.f), 3.f, 2.f);
}

VASTDragFX::~VASTDragFX() {
	this->setLookAndFeel(nullptr);
	ddImageButton = nullptr;
	ddLabel = nullptr;
}

VASTAudioProcessor* VASTDragFX::getAudioProcessor() {
	return my_processor;
}

inline void VASTDragFX::setModString(const juce::String& dragText) {
	ddLabel->setText(dragText, NotificationType::dontSendNotification);
}

void VASTDragFX::buttonClicked(Button* b) { //needed for listener interface
	String compID = b->getComponentID();
	if (!compID.equalsIgnoreCase(""))
		myGUINotifier->shallUpdateGUI(compID, b->getToggleState());
}

void VASTDragFX::buttonStateChanged(Button* b) {
	String compID = b->getComponentID();
	if (!compID.equalsIgnoreCase(""))
		myGUINotifier->shallUpdateGUI(compID, b->getToggleState());
}

bool VASTDragFX::isInterestedInDragSource(const SourceDetails& dragSourceDetails) {
	if (my_processor == nullptr) return false;
	
	String cid = dragSourceDetails.sourceComponent->getComponentID();
	if (cid.contains("FXDragButton"))
		return true;
	return false;
};

void VASTDragFX::itemDropped(const SourceDetails& dragSourceDetails) {
	if (my_processor == nullptr) return;
	
	String cid = dragSourceDetails.sourceComponent->getComponentID();
	if (!cid.contains("FXDragButton"))
		return;
	cid = cid.substring(12, cid.length());
	int sourceeffect = cid.getIntValue();
	if (sourceeffect != myEffectno) {
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

		fxBus->swapSlots(fxBus->findPosition(sourceeffect), fxBus->findPosition(myEffectno));
		my_editor->vaporizerComponent->updateAll();
	}
};
