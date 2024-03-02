/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 7.0.9

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
#include "../Engine/VASTEngineHeader.h"
#include "VASTControls/VASTParameterComboBox.h"
#include "VASTControls/VASTParameterSlider.h"
#include "VASTControls/VASTParameterButton.h"
#include "VASTControls/VASTComboBox.h"
#include "VASTAudioProcessorEditor.h"
#include "VASTFX/VASTGenericEditor.h"
#include "VASTFX/VASTFXPane.h"
//[/Headers]

#include "VASTFXComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
void VASTFXTabBarButton::itemDragEnter (const SourceDetails& dragSourceDetails) {
    if (tabbedComponent->getCurrentTabIndex() != tabIndex)
        tabbedComponent->setCurrentTabIndex(tabIndex);
};
TabBarButton* VASTTabbedFXComponent::createTabButton (const String& tabName, int tabIndex) {
    VASTFXTabBarButton* tabBarButton = new VASTFXTabBarButton (tabName, *tabs);
    tabBarButton->tabbedComponent = this;
    tabBarButton->tabIndex = tabIndex;
    return tabBarButton;
}
//[/MiscUserDefs]

//==============================================================================
VASTFXComponent::VASTFXComponent (AudioProcessorEditor *editor, AudioProcessor* processor)
    : myEditor((VASTAudioProcessorEditor*)editor), myProcessor((VASTAudioProcessor*)processor)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    c_fxBusTab.reset (new VASTTabbedFXComponent (juce::TabbedButtonBar::TabsAtTop));
    addAndMakeVisible (c_fxBusTab.get());
    c_fxBusTab->setTabBarDepth (30);
    c_fxBusTab->addTab (TRANS ("BUS1"), juce::Colour (0xff7e7e60), new VASTFXPane (myEditor, myProcessor, 0), true);
    c_fxBusTab->addTab (TRANS ("BUS2"), juce::Colour (0xff8a9362), new VASTFXPane (myEditor, myProcessor, 1), true);
    c_fxBusTab->addTab (TRANS ("BUS3"), juce::Colour (0xff9a9a90), new VASTFXPane (myEditor, myProcessor, 2), true);
    c_fxBusTab->setCurrentTabIndex (0);

    c_iconMaximizeEditor.reset (new VASTDrawableButton ("c_iconMaximize", arrow_right_corner_svg, arrow_right_corner_svgSize, "Maximize area"));
    addAndMakeVisible (c_iconMaximizeEditor.get());
    c_iconMaximizeEditor->setName ("c_iconMaximizeEditor");


    //[UserPreSize]
	//manage parameter bindings -> set ComponentID = Name and processor
	Array<Component*> children = this->getChildren();
	for (auto child : children) {
		if (child->getName().startsWith("m_")) {
			auto* aSlider = dynamic_cast<VASTParameterSlider*> (child);
			if (aSlider != nullptr) {
				aSlider->setAudioProcessor(*myProcessor);
				aSlider->bindParameter(myEditor, aSlider->getName(), VASTGUIRuntimeModel::GUIComponents::FXComponent, 0);
			}
			auto* aCombobox = dynamic_cast<VASTParameterComboBox*> (child);
			if (aCombobox != nullptr) {
				aCombobox->setAudioProcessor(*myProcessor);
				aCombobox->bindParameter(myEditor, aCombobox->getName(), VASTGUIRuntimeModel::GUIComponents::FXComponent, 0);
			}
			auto* aButton = dynamic_cast<VASTParameterButton*> (child);
			if (aButton != nullptr) {
				aButton->setAudioProcessor(*myProcessor);
				aButton->bindParameter(myEditor, aButton->getName(), VASTGUIRuntimeModel::GUIComponents::FXComponent, 0);
			}
		}
	}

	c_fxBusTab->getTabbedButtonBar().setColour(TabbedButtonBar::tabTextColourId, juce::Colour::fromFloatRGBA(0.f, 0.f, 0.f, 1.f));
	c_fxBusTab->getTabbedButtonBar().setColour(TabbedButtonBar::frontTextColourId, juce::Colour::fromFloatRGBA(1.f, 1.f, 1.f, 1.f));
	c_fxBusTab->setOutline(0);
	lastMouseWheelEvent = juce::Time::getCurrentTime();
	setOpaque(true);
	c_iconMaximizeEditor->addListener(this);

    return; //dont call setSize
    //[/UserPreSize]

    setSize (800, 575);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

VASTFXComponent::~VASTFXComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    VDBG("Destructing VASTFXComponent");
	this->setLookAndFeel(nullptr);
    //[/Destructor_pre]

    c_fxBusTab = nullptr;
    c_iconMaximizeEditor = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void VASTFXComponent::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff323e44));

    {
        int x = 0, y = 0, width = getWidth() - 0, height = getHeight() - 0;
        juce::Colour fillColour1 = juce::Colour (0xbd354000), fillColour2 = juce::Colour (0x430b0b01);
        juce::Colour strokeColour1 = juce::Colour (0xff202d2d), strokeColour2 = juce::Colour (0xff141515);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        fillColour1 = myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colFXComponentBackgroundGradientFrom);
        fillColour2 = myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colFXComponentBackgroundGradientTo);
        //[/UserPaintCustomArguments]
        g.setGradientFill (juce::ColourGradient (fillColour1,
                                             static_cast<float> (proportionOfWidth (0.1000f)) - 0.0f + x,
                                             static_cast<float> (proportionOfHeight (0.3448f)) - 0.0f + y,
                                             fillColour2,
                                             static_cast<float> (proportionOfWidth (0.2614f)) - 0.0f + x,
                                             static_cast<float> (proportionOfHeight (0.6595f)) - 0.0f + y,
                                             false));
        g.fillRect (x, y, width, height);
        g.setGradientFill (juce::ColourGradient (strokeColour1,
                                             static_cast<float> (proportionOfWidth (0.1000f)) - 0.0f + x,
                                             static_cast<float> (proportionOfHeight (0.3448f)) - 0.0f + y,
                                             strokeColour2,
                                             static_cast<float> (proportionOfWidth (0.7768f)) - 0.0f + x,
                                             static_cast<float> (proportionOfHeight (0.8411f)) - 0.0f + y,
                                             false));
        g.drawRect (x, y, width, height, 1);

    }

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void VASTFXComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    c_fxBusTab->setBounds (proportionOfWidth (0.0000f), proportionOfHeight (0.0000f), proportionOfWidth (1.0000f), proportionOfHeight (0.9983f));
    c_iconMaximizeEditor->setBounds (getWidth() - 1 - proportionOfWidth (0.0150f), 1, proportionOfWidth (0.0150f), proportionOfHeight (0.0209f));
    //[UserResized] Add your own custom resize handling here..
	c_fxBusTab->setTabBarDepth(c_fxBusTab->getHeight() * 0.04f);
    //[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void VASTFXComponent::updateAll() {

	VASTFXPane* bus1 = dynamic_cast<VASTFXPane*>(c_fxBusTab->getTabContentComponent(0));
	if (bus1!= nullptr)
        bus1->updateAll();

	VASTFXPane* bus2 = dynamic_cast<VASTFXPane*>(c_fxBusTab->getTabContentComponent(1));
	if (bus2 != nullptr)
        bus2->updateAll();

	VASTFXPane* bus3 = dynamic_cast<VASTFXPane*>(c_fxBusTab->getTabContentComponent(2));
	if (bus3 != nullptr)
        bus3->updateAll();

	repaint();
}

void VASTFXComponent::lookAndFeelChanged() {

	c_fxBusTab->getTabbedButtonBar().setTabBackgroundColour(0, myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colFXComponentTabBarFXBUS1));
	c_fxBusTab->getTabbedButtonBar().setTabBackgroundColour(1, myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colFXComponentTabBarFXBUS2));
	c_fxBusTab->getTabbedButtonBar().setTabBackgroundColour(2, myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colFXComponentTabBarFXBUS3));

	c_fxBusTab->getTabbedButtonBar().setColour(TabbedButtonBar::tabTextColourId, myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colVaporizerComponentTabBarText));
	c_fxBusTab->getTabbedButtonBar().setColour(TabbedButtonBar::frontTextColourId, myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colVaporizerComponentTabBarTextSelected));
}

void VASTFXComponent::buttonClicked(Button* buttonThatWasClicked)
{
	if (buttonThatWasClicked == c_iconMaximizeEditor.get()) {
		if (!myEditor->vaporizerComponent->isTabConcertinaMaximized()) {
			myEditor->vaporizerComponent->maximizeTabConcertina();
		}
		else {
			myEditor->vaporizerComponent->defaultConcertina();
		}
		return;
	}
}

void VASTFXComponent::mouseWheelMove(const MouseEvent &event, const MouseWheelDetails &wheel) {
	if (dynamic_cast<TabBarButton*>(event.originalComponent) == 0)
		return;
	if (abs(lastMouseWheelEvent.getMilliseconds() - event.eventTime.getMilliseconds()) < 50)
		return;
	lastMouseWheelEvent = event.eventTime;
	if ((c_fxBusTab->getTabbedButtonBar().getScreenBounds().contains(event.getScreenPosition()))) {
		int curidx = c_fxBusTab->getTabbedButtonBar().getCurrentTabIndex();
		int numidx = c_fxBusTab->getTabbedButtonBar().getNumTabs();
		if (wheel.deltaY > 0.f)
			curidx = (curidx + 1) % numidx;
		else if (wheel.deltaY < 0.f)
			curidx = ((curidx - 1) + numidx) % numidx;
		c_fxBusTab->getTabbedButtonBar().setCurrentTabIndex(curidx);
	}
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="VASTFXComponent" componentName=""
                 parentClasses="public Component, public Button::Listener" constructorParams="AudioProcessorEditor *editor, AudioProcessor* processor"
                 variableInitialisers="myEditor((VASTAudioProcessorEditor*)editor), myProcessor((VASTAudioProcessor*)processor)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="800" initialHeight="575">
  <BACKGROUND backgroundColour="ff323e44">
    <RECT pos="0 0 0M 0M" fill="linear: 10% 34.483%, 26.144% 65.95%, 0=bd354000, 1=430b0b01"
          hasStroke="1" stroke="1, mitered, butt" strokeColour="linear: 10% 34.483%, 77.684% 84.11%, 0=ff202d2d, 1=ff141515"/>
  </BACKGROUND>
  <TABBEDCOMPONENT name="c_fxBusTab" id="eebc75da900c2fda" memberName="c_fxBusTab"
                   virtualName="VASTTabbedFXComponent" explicitFocusOrder="0" pos="0% 0% 100% 99.826%"
                   orientation="top" tabBarDepth="30" initialTab="0">
    <TAB name="BUS1" colour="ff7e7e60" useJucerComp="0" contentClassName="VASTFXPane"
         constructorParams="myEditor, myProcessor, 0" jucerComponentFile=""/>
    <TAB name="BUS2" colour="ff8a9362" useJucerComp="0" contentClassName="VASTFXPane"
         constructorParams="myEditor, myProcessor, 1" jucerComponentFile=""/>
    <TAB name="BUS3" colour="ff9a9a90" useJucerComp="0" contentClassName="VASTFXPane"
         constructorParams="myEditor, myProcessor, 2" jucerComponentFile=""/>
  </TABBEDCOMPONENT>
  <GENERICCOMPONENT name="c_iconMaximizeEditor" id="ded548631676712b" memberName="c_iconMaximizeEditor"
                    virtualName="" explicitFocusOrder="0" pos="1Rr 1 1.5% 2.087%"
                    class="VASTDrawableButton" params="&quot;c_iconMaximize&quot;, arrow_right_corner_svg, arrow_right_corner_svgSize, &quot;Maximize area&quot;"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif

//==============================================================================
// Binary resources - be careful not to edit any of these sections!

// JUCER_RESOURCE: arrow_right_corner_svg, 1715, "../../Resources/Icons/arrow_right_corner.svg"
static const unsigned char resource_VASTFXComponent_arrow_right_corner_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,70,45,
56,34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,32,119,105,100,116,104,61,34,50,48,112,120,34,32,104,101,105,103,104,116,61,34,50,48,112,120,34,32,118,105,101,119,
66,111,120,61,34,48,32,48,32,50,48,32,50,48,34,32,118,101,114,115,105,111,110,61,34,49,46,49,34,32,120,109,108,110,115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,
47,115,118,103,34,32,120,109,108,110,115,58,120,108,105,110,107,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,120,108,105,110,107,34,62,10,32,32,32,32,60,33,45,
45,32,71,101,110,101,114,97,116,111,114,58,32,83,107,101,116,99,104,32,51,46,56,46,49,32,40,50,57,54,56,55,41,32,45,32,104,116,116,112,58,47,47,119,119,119,46,98,111,104,101,109,105,97,110,99,111,100,
105,110,103,46,99,111,109,47,115,107,101,116,99,104,32,45,45,62,10,32,32,32,32,60,116,105,116,108,101,62,97,114,114,111,119,95,114,105,103,104,116,95,99,111,114,110,101,114,32,91,35,50,55,57,93,60,47,
116,105,116,108,101,62,10,32,32,32,32,60,100,101,115,99,62,67,114,101,97,116,101,100,32,119,105,116,104,32,83,107,101,116,99,104,46,60,47,100,101,115,99,62,10,32,32,32,32,60,100,101,102,115,62,60,47,100,
101,102,115,62,10,32,32,32,32,60,103,32,105,100,61,34,80,97,103,101,45,49,34,32,115,116,114,111,107,101,61,34,110,111,110,101,34,32,115,116,114,111,107,101,45,119,105,100,116,104,61,34,49,34,32,102,105,
108,108,61,34,110,111,110,101,34,32,102,105,108,108,45,114,117,108,101,61,34,101,118,101,110,111,100,100,34,62,10,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,68,114,105,98,98,98,108,101,45,68,97,114,
107,45,80,114,101,118,105,101,119,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,45,49,56,48,46,48,48,48,48,48,48,44,32,45,54,57,49,57,46,48,48,48,48,48,48,41,34,32,
102,105,108,108,61,34,35,70,70,70,70,70,70,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,105,99,111,110,115,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,
97,116,101,40,53,54,46,48,48,48,48,48,48,44,32,49,54,48,46,48,48,48,48,48,48,41,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,60,112,97,116,104,32,100,61,34,77,49,51,50,44,54,55,54,52,32,67,
49,51,50,44,54,55,54,51,46,52,52,56,32,49,51,50,46,52,52,56,44,54,55,54,51,32,49,51,51,44,54,55,54,51,32,76,49,51,56,44,54,55,54,51,32,67,49,51,57,46,49,48,53,44,54,55,54,51,32,49,52,48,44,54,55,54,51,
46,56,57,53,32,49,52,48,44,54,55,54,53,32,76,49,52,48,44,54,55,55,48,32,67,49,52,48,44,54,55,55,48,46,53,53,50,32,49,51,57,46,53,53,50,44,54,55,55,49,32,49,51,57,44,54,55,55,49,32,67,49,51,56,46,52,52,
56,44,54,55,55,49,32,49,51,56,44,54,55,55,48,46,53,53,50,32,49,51,56,44,54,55,55,48,32,76,49,51,56,44,54,55,54,55,46,54,50,49,32,67,49,51,56,44,54,55,54,55,46,49,55,54,32,49,51,55,46,52,54,49,44,54,55,
54,54,46,57,53,51,32,49,51,55,46,49,52,54,44,54,55,54,55,46,50,54,56,32,76,49,51,52,46,49,50,49,44,54,55,55,48,46,50,57,51,32,67,49,51,51,46,55,51,49,44,54,55,55,48,46,54,56,51,32,49,51,51,46,48,57,56,
44,54,55,55,48,46,54,56,51,32,49,51,50,46,55,48,55,44,54,55,55,48,46,50,57,51,32,67,49,51,50,46,51,49,55,44,54,55,54,57,46,57,48,50,32,49,51,50,46,51,49,55,44,54,55,54,57,46,50,54,57,32,49,51,50,46,55,
48,55,44,54,55,54,56,46,56,55,57,32,76,49,51,53,46,55,51,50,44,54,55,54,53,46,56,53,52,32,67,49,51,54,46,48,52,55,44,54,55,54,53,46,53,51,57,32,49,51,53,46,56,50,52,44,54,55,54,53,32,49,51,53,46,51,55,
57,44,54,55,54,53,32,76,49,51,51,44,54,55,54,53,32,67,49,51,50,46,52,52,56,44,54,55,54,53,32,49,51,50,44,54,55,54,52,46,53,53,50,32,49,51,50,44,54,55,54,52,32,76,49,51,50,44,54,55,54,52,32,90,32,77,49,
50,54,44,54,55,54,50,32,67,49,50,54,44,54,55,54,49,46,52,52,56,32,49,50,54,46,52,52,56,44,54,55,54,49,32,49,50,55,44,54,55,54,49,32,76,49,52,49,44,54,55,54,49,32,67,49,52,49,46,53,53,50,44,54,55,54,49,
32,49,52,50,44,54,55,54,49,46,52,52,56,32,49,52,50,44,54,55,54,50,32,76,49,52,50,44,54,55,55,54,32,67,49,52,50,44,54,55,55,54,46,53,53,50,32,49,52,49,46,53,53,50,44,54,55,55,55,32,49,52,49,44,54,55,55,
55,32,76,49,51,51,44,54,55,55,55,32,67,49,51,50,46,52,52,56,44,54,55,55,55,32,49,51,50,44,54,55,55,54,46,53,53,50,32,49,51,50,44,54,55,55,54,32,76,49,51,50,44,54,55,55,51,32,67,49,51,50,44,54,55,55,49,
46,56,57,53,32,49,51,49,46,49,48,53,44,54,55,55,49,32,49,51,48,44,54,55,55,49,32,76,49,50,55,44,54,55,55,49,32,67,49,50,54,46,52,52,56,44,54,55,55,49,32,49,50,54,44,54,55,55,48,46,53,53,50,32,49,50,54,
44,54,55,55,48,32,76,49,50,54,44,54,55,54,50,32,90,32,77,49,52,52,44,54,55,54,49,32,67,49,52,52,44,54,55,53,57,46,56,57,53,32,49,52,51,46,49,48,53,44,54,55,53,57,32,49,52,50,44,54,55,53,57,32,76,49,50,
54,44,54,55,53,57,32,67,49,50,52,46,56,57,53,44,54,55,53,57,32,49,50,52,44,54,55,53,57,46,56,57,53,32,49,50,52,44,54,55,54,49,32,76,49,50,52,44,54,55,55,55,32,67,49,50,52,44,54,55,55,56,46,49,48,53,32,
49,50,52,46,56,57,53,44,54,55,55,57,32,49,50,54,44,54,55,55,57,32,76,49,52,50,44,54,55,55,57,32,67,49,52,51,46,49,48,53,44,54,55,55,57,32,49,52,52,44,54,55,55,56,46,49,48,53,32,49,52,52,44,54,55,55,55,
32,76,49,52,52,44,54,55,54,49,32,90,34,32,105,100,61,34,97,114,114,111,119,95,114,105,103,104,116,95,99,111,114,110,101,114,45,91,35,50,55,57,93,34,62,60,47,112,97,116,104,62,10,32,32,32,32,32,32,32,32,
32,32,32,32,60,47,103,62,10,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,60,47,103,62,10,60,47,115,118,103,62,0,0};

const char* VASTFXComponent::arrow_right_corner_svg = (const char*) resource_VASTFXComponent_arrow_right_corner_svg;
const int VASTFXComponent::arrow_right_corner_svgSize = 1715;


//[EndFile] You can add extra defines here...
//[/EndFile]

