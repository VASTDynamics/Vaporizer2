/*
VAST Dynamics
*/

#include "../../Engine/VASTEngineHeader.h"
#include "VASTDragMatrix.h"
#include "../VASTAudioProcessor.h"
#include "../VASTAudioProcessorEditor.h"
#include "../../Engine/FX/VASTEffect.h"
#include "../VASTUtils/VASTGUINotifier.h"
#include "../../Engine/VASTFXBus.h"
#include "VASTParameterComboBox.h"
#include "VASTParameterComboBox.h"
#include "VASTParameterSlider.h"
#include "VASTDrawableButton.h"
#include "../VASTScopeDisplay/VASTModMatCurveDisplay.h"

VASTDragMatrix::VASTDragMatrix(VASTAudioProcessorEditor *editor, VASTAudioProcessor *processor, int slotno) :
    my_processor(processor), my_editor(editor), my_slotno(slotno)
{
	//setRepaintsOnMouseActivity(false); //performance
	//setBufferedToImage(true); //performance

	String componentName = "MatrixDragButton" + String(slotno);
	ddImageButton.reset(new VASTImageButton(componentName));
	addAndMakeVisible(ddImageButton.get());
	ddImageButton->setAudioProcessor(*my_processor);
	ddImageButton->setImages(false, true, true,
		ImageCache::getFromMemory(VASTHeaderComponent::move_grabber_png, VASTHeaderComponent::move_grabber_pngSize), 1.000f, Colour(0x451c6d2f),
		ImageCache::getFromMemory(VASTHeaderComponent::move_grabber_png, VASTHeaderComponent::move_grabber_pngSize), 1.000f, Colour(0x00000000),
		ImageCache::getFromMemory(VASTHeaderComponent::move_grabber_png, VASTHeaderComponent::move_grabber_pngSize), 1.000f, Colour(0x00000000));


	String paramID = "";
	paramID = "m_uModMatSrce" + String(my_slotno + 1);
	m_uModMatSrce.reset(new VASTParameterComboBox(paramID));
	m_uModMatSrce->setAudioProcessor(*my_processor);
	m_uModMatSrce->bindParameter(my_editor, paramID, VASTGUIRuntimeModel::GUIComponents::DragMatrix, 0);
	//m_uModMatSrce->addListener(this);
	addAndMakeVisible(m_uModMatSrce.get());

	paramID = "m_uModMatPolarity" + String(my_slotno + 1);
	m_uModMatPolarity.reset(new VASTParameterComboBox(paramID));
	m_uModMatPolarity->setAudioProcessor(*my_processor);
	m_uModMatPolarity->bindParameter(my_editor, paramID, VASTGUIRuntimeModel::GUIComponents::DragMatrix, 0);
	//m_uModMatPolarity->addListener(this);
	addAndMakeVisible(m_uModMatPolarity.get());

	paramID = "m_uModMatDest" + String(my_slotno + 1);
	m_uModMatDest.reset(new VASTParameterComboBox(paramID));
	m_uModMatDest->setAudioProcessor(*my_processor);
	m_uModMatDest->bindParameter(my_editor, paramID, VASTGUIRuntimeModel::GUIComponents::DragMatrix, 0);
	//m_uModMatDest->addListener(this);
	addAndMakeVisible(m_uModMatDest.get());
		
	paramID = "m_fModMatVal" + String(my_slotno + 1);
	m_fModMatVal.reset(new VASTParameterSlider(paramID));
	m_fModMatVal->setAudioProcessor(*my_processor);
	m_fModMatVal->bindParameter(my_editor, paramID, VASTGUIRuntimeModel::GUIComponents::DragMatrix, 0);
	m_fModMatVal->setSliderStyle(Slider::RotaryVerticalDrag);
	m_fModMatVal->setTextBoxStyle(Slider::NoTextBox, false, 40, 16);
	//m_fModMatVal->addListener(this);
	addAndMakeVisible(m_fModMatVal.get());

	paramID = "m_fModMatCurve" + String(my_slotno + 1);
	m_fModMatCurve.reset(new VASTParameterSlider(paramID));
	m_fModMatCurve->setAudioProcessor(*my_processor);
	m_fModMatCurve->bindParameter(my_editor, paramID, VASTGUIRuntimeModel::GUIComponents::DragMatrix, 0);
	m_fModMatCurve->setSliderStyle(Slider::RotaryVerticalDrag);
	m_fModMatCurve->setTextBoxStyle(Slider::NoTextBox, false, 40, 16);
	//m_fModMatCurve->addListener(this);
	addAndMakeVisible(m_fModMatCurve.get());

	c_curveDisplay.reset(new VASTModMatCurveDisplay(my_slotno, my_editor, my_processor));
	addAndMakeVisible(c_curveDisplay.get());

	c_iconDelSlot.reset(new VASTDrawableButton("c_iconDelSlot", VASTMatrixComponent::delete_svg, VASTMatrixComponent::delete_svgSize, "Delete matrix modulation slot"));
	c_iconDelSlot->addListener(this);
	addAndMakeVisible(c_iconDelSlot.get());
}

void VASTDragMatrix::startAutoUpdate() {
	c_curveDisplay->startAutoUpdate();
}

void VASTDragMatrix::stopAutoUpdate() {
	c_curveDisplay->stopAutoUpdate();
}

void VASTDragMatrix::updateContent(bool force) {
	c_curveDisplay->updateContent(force);
}

void VASTDragMatrix::resized() {
	m_uModMatSrce->setBounds(proportionOfWidth(0.02f), proportionOfHeight(0.25f), proportionOfWidth(0.2f), proportionOfHeight(0.5f));
	m_uModMatPolarity->setBounds(proportionOfWidth(0.24f), proportionOfHeight(0.25f), proportionOfWidth(0.12f), proportionOfHeight(0.5f));
	m_uModMatDest->setBounds(proportionOfWidth(0.38f), proportionOfHeight(0.25f), proportionOfWidth(0.2f), proportionOfHeight(0.5f));
	m_fModMatVal->setBounds(proportionOfWidth(0.6075f), proportionOfHeight(0.0f), proportionOfWidth(0.0500f), proportionOfHeight(1.f));
	m_fModMatCurve->setBounds(proportionOfWidth(0.6700f), proportionOfHeight(0.0f), proportionOfWidth(0.0500f), proportionOfHeight(1.f));
	c_curveDisplay->setBounds(proportionOfWidth(0.7488f), proportionOfHeight(0.0f), proportionOfWidth(0.0800f), proportionOfHeight(1.f));
	c_iconDelSlot->setBounds(proportionOfWidth(0.8500f), proportionOfHeight(0.25f), proportionOfWidth(0.0575f), proportionOfHeight(0.5f));
	ddImageButton->setBounds(proportionOfWidth(0.94f), proportionOfHeight(0.2f), proportionOfWidth(0.05f), proportionOfHeight(0.6f));
}

void VASTDragMatrix::lookAndFeelChanged() {
	if (my_processor == nullptr) return;
	Colour c1 = my_processor->getCurrentVASTLookAndFeel()->findVASTColour(colGroupComponentOutline).withAlpha(1.0f).contrasting(0.5f);
	ddImageButton->setImages(false, true, true,
		ImageCache::getFromMemory(VASTHeaderComponent::move_grabber_png, VASTHeaderComponent::move_grabber_pngSize), 1.000f, c1,
		ImageCache::getFromMemory(VASTHeaderComponent::move_grabber_png, VASTHeaderComponent::move_grabber_pngSize), 1.000f, c1.contrasting(0.3f),
		ImageCache::getFromMemory(VASTHeaderComponent::move_grabber_png, VASTHeaderComponent::move_grabber_pngSize), 1.000f, c1.contrasting(0.5f));
}

void VASTDragMatrix::paint(Graphics& g)
{
	if (my_processor == nullptr) return;
	Colour c1 = my_processor->getCurrentVASTLookAndFeel()->findVASTColour(colOscillatorComponentHeaderGradientFrom).withAlpha(0.8f);
	Colour c2 = my_processor->getCurrentVASTLookAndFeel()->findVASTColour(colOscillatorComponentHeaderGradientTo).withAlpha(0.8f);
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

VASTDragMatrix::~VASTDragMatrix() {
	this->setLookAndFeel(nullptr);
}

VASTAudioProcessor* VASTDragMatrix::getAudioProcessor() {
	return my_processor;
}

void VASTDragMatrix::buttonClicked(Button* b) { //needed for listener interface
	if (b == c_iconDelSlot.get()) {
		my_processor->m_pVASTXperience.m_Set.modMatrixRemoveSlot(my_slotno);
		my_processor->requestUIUpdate();
	}
}

void VASTDragMatrix::buttonStateChanged(Button* b) {
}

bool VASTDragMatrix::isInterestedInDragSource(const SourceDetails& dragSourceDetails) {
	if (my_processor == nullptr) return false;
	
	String cid = dragSourceDetails.sourceComponent->getComponentID();
	if (cid.contains("MatrixDragButton"))
		return true;
	return false;
};

void VASTDragMatrix::itemDropped(const SourceDetails& dragSourceDetails) {
	if (my_processor == nullptr) return;

	String cid = dragSourceDetails.sourceComponent->getComponentID();
	if (!cid.contains("MatrixDragButton"))
		return;
	cid = cid.substring(16, cid.length());
	int sourceslot = cid.getIntValue();

	my_processor->m_pVASTXperience.m_Set.modMatrixSwapSlots(my_slotno, sourceslot);
};
