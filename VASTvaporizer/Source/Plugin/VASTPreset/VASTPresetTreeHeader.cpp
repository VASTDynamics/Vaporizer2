/*
VAST Dynamics
*/

#include "../../Engine/VASTEngineHeader.h"
#include "VASTPresetTreeHeader.h"
#include "VASTPresetComponent.h"

VASTPresetTreeHeader::VASTPresetTreeHeader(VASTPresetComponent* presetComponent, String caption) : m_caption(caption), _presetComponent(presetComponent) {
	headerLabel.reset(new juce::Label(m_caption, m_caption));
	Font font = _presetComponent->myProcessor->getCurrentVASTLookAndFeel()->getDefaultFontBold();

	addAndMakeVisible(headerLabel.get());
	headerLabel->setFont(font);
	headerLabel->setJustificationType(juce::Justification::centredLeft);
	headerLabel->setEditable(false, false, false);
}

VASTPresetTreeHeader::~VASTPresetTreeHeader() {
	//this->setLookAndFeel(nullptr);
}

void VASTPresetTreeHeader::paint(Graphics& g) {
	auto highlightColour = _presetComponent->myProcessor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscillatorComponentBackgroundGradientFrom);

	//g.fillAll(highlightColour.withMultipliedAlpha(0.625f));
	g.fillAll(highlightColour);

	auto r = getLocalBounds();
	r.reduce(1, 1);
	auto outlineColour = _presetComponent->myProcessor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colLabelOutline);
	outlineColour = Colours::darkgrey;
	g.setColour(outlineColour);
	g.drawRect(r, 1.f);
}

void VASTPresetTreeHeader::resized() {
	headerLabel->setBounds(proportionOfWidth(0.01f), proportionOfHeight(0.2f), proportionOfWidth(0.7f), proportionOfHeight(0.6f));
}