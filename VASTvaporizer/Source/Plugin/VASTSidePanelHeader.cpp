/*
VAST Dynamics
*/

#include "VASTSidePanelHeader.h"
#include "../Engine/VASTEngineHeader.h"
#include "VASTUtils/VASTLookAndFeel.h"
#include "VASTVaporizerComponent.h"

VASTSidePanelHeader::VASTSidePanelHeader(VASTVaporizerComponent* o)
	: owner(o)
{
	setOpaque(true);
	titleLabel.setText(owner->getSidePanel()->getTitleText(), NotificationType::dontSendNotification);
	addAndMakeVisible(titleLabel);

	dismissButton.onClick = [this] { hidePanel(); };
	addAndMakeVisible(dismissButton);

	lookAndFeelChanged();
}

void VASTSidePanelHeader::hidePanel() {
	auto& sidePanel = *owner->getSidePanel();
	sidePanel.showOrHide(false);
}

void VASTSidePanelHeader::paint(Graphics& g) 
{
	Colour c1 = owner->getVASTProcessor()->getCurrentVASTLookAndFeel()->findVASTColour(colOscillatorComponentHeaderGradientFrom);
	g.fillAll(c1);
}

void VASTSidePanelHeader::resized() 
{
	auto bounds = getLocalBounds();
	
	dismissButton.setBounds(bounds.removeFromLeft(getHeight() * 0.8f ).withTrimmedLeft(getHeight() * 0.2f));
	titleLabel.setBounds(bounds.getX() + 5.f, getHeight() * 0.3f, bounds.getWidth() - 5.f, getHeight() * 0.4f);

}

void VASTSidePanelHeader::lookAndFeelChanged() 
{
	auto& sidePanel = *owner->getSidePanel();
	auto lf = owner->getVASTProcessor()->getCurrentVASTLookAndFeel();

	Font f1 = owner->getVASTProcessor()->getCurrentVASTLookAndFeel()->getDefaultFontBold();
	f1.setSizeAndStyle(8.f, f1.getStyleFlags(), 1.f, 0.f);
	titleLabel.setLookAndFeel(lf);
	titleLabel.setFont(f1);
	titleLabel.setJustificationType(Justification::centredLeft);
	titleLabel.setColour(Label::textColourId, owner->getVASTProcessor()->getCurrentVASTLookAndFeel()->findVASTColour(colHeaderLabelText));

	dismissButton.setShape(lf->getSidePanelDismissButtonShape(sidePanel), false, true, false);

	dismissButton.setColours(lf->findColour(SidePanel::dismissButtonNormalColour),
		lf->findColour(SidePanel::dismissButtonOverColour),
		lf->findColour(SidePanel::dismissButtonDownColour));

	lf->setColour(SidePanel::dismissButtonNormalColour, lf->findVASTColour(VASTColours::colOscillatorComponentHeaderGradientFrom).contrasting(.4f));
	lf->setColour(SidePanel::dismissButtonOverColour, lf->findVASTColour(VASTColours::colOscillatorComponentHeaderGradientFrom).contrasting(.6f));
	lf->setColour(SidePanel::dismissButtonDownColour, lf->findVASTColour(VASTColours::colOscillatorComponentHeaderGradientFrom).contrasting(.8f));
	lf->setColour(SidePanel::backgroundColour, lf->findVASTColour(VASTColours::colOscillatorComponentHeaderGradientFrom));
	
}
