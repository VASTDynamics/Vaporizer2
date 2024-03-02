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
#include "VASTAudioProcessor.h"
#include "VASTAudioProcessorEditor.h"
//#include "VASTControls/VASTComboBrev.h"
#include "VASTControls/VASTComboBox.h"
//#include "VASTControls/VASTImageButton.h"
#include "VASTMSEGEditor/VASTMSEGEditor.h"
#include "VASTMSEGEditor/VASTMSEGEditorPane.h"
#include "VASTMSEGEditor/VASTLFOEditor.h"
#include "VASTMSEGEditor/VASTLFOEditorPane.h"
#include "VASTMSEGEditor/VASTLFOEditor.h"

//[/Headers]

#include "VASTLFOMsegComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
VASTLFOMsegComponent::VASTLFOMsegComponent (AudioProcessorEditor *editor, AudioProcessor* processor)
    : myEditor((VASTAudioProcessorEditor*)editor), myProcessor((VASTAudioProcessor*)processor)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    c_envTab.reset (new VASTTabbedMSEGComponent (myProcessor,
                                                 myEditor, juce::TabbedButtonBar::TabsAtTop));
    addAndMakeVisible (c_envTab.get());
    c_envTab->setName ("c_envTab");

    c_iconMaximizeEditor.reset (new VASTDrawableButton ("c_iconMaximize", arrow_right_corner_svg, arrow_right_corner_svgSize, "Maximize area"));
    addAndMakeVisible (c_iconMaximizeEditor.get());
    c_iconMaximizeEditor->setName ("c_iconMaximizeEditor");

    c_lfoTab.reset (new VASTTabbedLFOComponent (myProcessor,
                                                myEditor, juce::TabbedButtonBar::TabsAtTop));
    addAndMakeVisible (c_lfoTab.get());
    c_lfoTab->setName ("c_lfoTab3");


    //[UserPreSize]
	c_envTab->setTabBarDepth(30);
	c_envTab->addTab(TRANS("MSEG1"), juce::Colour(0x90bc6445), new VASTMSEGEditorPane(myEditor, myProcessor, &myProcessor->m_pVASTXperience.m_Set.m_MSEGData_changed[0], &myProcessor->m_pVASTXperience.m_Set.m_MSEGData[0], "MSEG1", 0), true);
	c_envTab->addTab(TRANS("MSEG2"), juce::Colour(0x80bc6445), new VASTMSEGEditorPane(myEditor, myProcessor, &myProcessor->m_pVASTXperience.m_Set.m_MSEGData_changed[1], &myProcessor->m_pVASTXperience.m_Set.m_MSEGData[1], "MSEG2", 1), true);
	c_envTab->addTab(TRANS("MSEG3"), juce::Colour(0x70bc6445), new VASTMSEGEditorPane(myEditor, myProcessor, &myProcessor->m_pVASTXperience.m_Set.m_MSEGData_changed[2], &myProcessor->m_pVASTXperience.m_Set.m_MSEGData[2], "MSEG3", 2), true);
	c_envTab->addTab(TRANS("MSEG4"), juce::Colour(0x60bc6554), new VASTMSEGEditorPane(myEditor, myProcessor, &myProcessor->m_pVASTXperience.m_Set.m_MSEGData_changed[3], &myProcessor->m_pVASTXperience.m_Set.m_MSEGData[3], "MSEG4", 3), true);
	c_envTab->addTab(TRANS("MSEG5"), juce::Colour(0x50bc6445), new VASTMSEGEditorPane(myEditor, myProcessor, &myProcessor->m_pVASTXperience.m_Set.m_MSEGData_changed[4], &myProcessor->m_pVASTXperience.m_Set.m_MSEGData[4], "MSEG5", 4), true);
	c_envTab->setCurrentTabIndex(0);

	c_lfoTab->setTabBarDepth(30);
	c_lfoTab->addTab(TRANS("LFO1"), juce::Colour(0xff002a32), new VASTLFOEditorPane(myEditor, myProcessor, "LFO1", 0), true);
	c_lfoTab->addTab(TRANS("LFO2"), juce::Colour(0xf0002a32), new VASTLFOEditorPane(myEditor, myProcessor, "LFO2", 1), true);
	c_lfoTab->addTab(TRANS("LFO3"), juce::Colour(0xe0002a32), new VASTLFOEditorPane(myEditor, myProcessor, "LFO3", 2), true);
	c_lfoTab->addTab(TRANS("LFO4"), juce::Colour(0xd0002a32), new VASTLFOEditorPane(myEditor, myProcessor, "LFO4", 3), true);
	c_lfoTab->addTab(TRANS("LFO5"), juce::Colour(0xc0002a32), new VASTLFOEditorPane(myEditor, myProcessor, "LFO5", 4), true);
	c_lfoTab->setCurrentTabIndex(0);

	setOpaque(true);
	c_iconMaximizeEditor->addListener(this);

	//manage parameter bindings -> set ComponentID = Name and processor
	Array<Component*> children = this->getChildren();
	for (auto child : children) {
		if (child->getName().startsWith("m_")) {
			auto* aSlider = dynamic_cast<VASTParameterSlider*> (child);
			if (aSlider != nullptr) {
				aSlider->setAudioProcessor(*myProcessor);
				aSlider->bindParameter(myEditor, aSlider->getName(), VASTGUIRuntimeModel::GUIComponents::LFOMSEGComponent, 0);
			}
			auto* aCombobox = dynamic_cast<VASTParameterComboBox*> (child);
			if (aCombobox != nullptr) {
				aCombobox->setAudioProcessor(*myProcessor);
				aCombobox->bindParameter(myEditor, aCombobox->getName(), VASTGUIRuntimeModel::GUIComponents::LFOMSEGComponent, 0);
			}
			auto* aButton = dynamic_cast<VASTParameterButton*> (child);
			if (aButton != nullptr) {
				aButton->setAudioProcessor(*myProcessor);
				aButton->bindParameter(myEditor, aButton->getName(), VASTGUIRuntimeModel::GUIComponents::LFOMSEGComponent, 0);
			}
		}
	}

	c_envTab->getTabbedButtonBar().setColour(TabbedButtonBar::tabTextColourId, juce::Colour::fromFloatRGBA(0.f, 0.f, 0.f, 1.f));
	c_envTab->getTabbedButtonBar().setColour(TabbedButtonBar::frontTextColourId, juce::Colour::fromFloatRGBA(1.f, 1.f, 1.f, 1.f));
	c_lfoTab->getTabbedButtonBar().setColour(TabbedButtonBar::tabTextColourId, juce::Colour::fromFloatRGBA(0.f, 0.f, 0.f, 1.f));
	c_lfoTab->getTabbedButtonBar().setColour(TabbedButtonBar::frontTextColourId, juce::Colour::fromFloatRGBA(1.f, 1.f, 1.f, 1.f));
	lastMouseWheelEvent = juce::Time::getCurrentTime();

    return; //dont call setSize
    //[/UserPreSize]

    setSize (800, 575);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

VASTLFOMsegComponent::~VASTLFOMsegComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    VDBG("Destructing VASTLFOMsegComponent");
	this->setLookAndFeel(nullptr);
    //[/Destructor_pre]

    c_envTab = nullptr;
    c_iconMaximizeEditor = nullptr;
    c_lfoTab = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void VASTLFOMsegComponent::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff323e44));

    {
        int x = 0, y = 0, width = getWidth() - 0, height = getHeight() - 0;
        juce::Colour fillColour1 = juce::Colour (0xbd4e1a0d), fillColour2 = juce::Colour (0x4301080b);
        //[UserPaintCustomArguments] Customize the painting arguments here..
    		fillColour1 = myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colLFOMsegComponentBackgroundGradientFrom);
    		fillColour2 = myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colLFOMsegComponentBackgroundGradientFrom);
        //[/UserPaintCustomArguments]
        g.setGradientFill (juce::ColourGradient (fillColour1,
                                             static_cast<float> (proportionOfWidth (0.1000f)) - 0.0f + x,
                                             static_cast<float> (proportionOfHeight (0.3448f)) - 0.0f + y,
                                             fillColour2,
                                             static_cast<float> (proportionOfWidth (0.4480f)) - 0.0f + x,
                                             static_cast<float> (proportionOfHeight (0.5538f)) - 0.0f + y,
                                             false));
        g.fillRect (x, y, width, height);
    }

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void VASTLFOMsegComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    c_envTab->setBounds (proportionOfWidth (0.0000f), proportionOfHeight (0.0000f), proportionOfWidth (1.0000f), proportionOfHeight (0.4991f));
    c_iconMaximizeEditor->setBounds (getWidth() - 1 - proportionOfWidth (0.0150f), 1, proportionOfWidth (0.0150f), proportionOfHeight (0.0209f));
    c_lfoTab->setBounds (proportionOfWidth (0.0000f), proportionOfHeight (0.5009f), proportionOfWidth (1.0000f), proportionOfHeight (0.4991f));
    //[UserResized] Add your own custom resize handling here..
	c_envTab->setTabBarDepth(c_envTab->getHeight() * 0.08f);
	c_lfoTab->setTabBarDepth(c_lfoTab->getHeight() * 0.08f);
    //[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void VASTLFOMsegComponent::lookAndFeelChanged() {

	c_envTab->getTabbedButtonBar().setTabBackgroundColour(0, myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colLFOMsegComponentTabBarMSEG1));
	c_envTab->getTabbedButtonBar().setTabBackgroundColour(1, myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colLFOMsegComponentTabBarMSEG2));
	c_envTab->getTabbedButtonBar().setTabBackgroundColour(2, myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colLFOMsegComponentTabBarMSEG3));
	c_envTab->getTabbedButtonBar().setTabBackgroundColour(3, myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colLFOMsegComponentTabBarMSEG4));
	c_envTab->getTabbedButtonBar().setTabBackgroundColour(4, myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colLFOMsegComponentTabBarMSEG5));

	c_envTab->getTabbedButtonBar().setColour(TabbedButtonBar::tabTextColourId, myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colVaporizerComponentTabBarText));
	c_envTab->getTabbedButtonBar().setColour(TabbedButtonBar::frontTextColourId, myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colVaporizerComponentTabBarTextSelected));

	c_lfoTab->getTabbedButtonBar().setTabBackgroundColour(0, myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colLFOMsegComponentTabBarLFO1));
	c_lfoTab->getTabbedButtonBar().setTabBackgroundColour(1, myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colLFOMsegComponentTabBarLFO2));
	c_lfoTab->getTabbedButtonBar().setTabBackgroundColour(2, myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colLFOMsegComponentTabBarLFO3));
	c_lfoTab->getTabbedButtonBar().setTabBackgroundColour(3, myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colLFOMsegComponentTabBarLFO4));
	c_lfoTab->getTabbedButtonBar().setTabBackgroundColour(4, myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colLFOMsegComponentTabBarLFO5));

	c_lfoTab->getTabbedButtonBar().setColour(TabbedButtonBar::tabTextColourId, myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colVaporizerComponentTabBarText));
	c_lfoTab->getTabbedButtonBar().setColour(TabbedButtonBar::frontTextColourId, myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colVaporizerComponentTabBarTextSelected));

}

void VASTLFOMsegComponent::startAutoUpdate() {
	if (isShowing()) grabKeyboardFocus();
	c_envTab->currentTabChanged(c_envTab->getCurrentTabIndex(), "");
	c_lfoTab->currentTabChanged(c_lfoTab->getCurrentTabIndex(), "");
}

void VASTLFOMsegComponent::stopAutoUpdate() {
	VASTMSEGEditorPane* tab1 = dynamic_cast<VASTMSEGEditorPane*>(c_envTab->getTabContentComponent(0));
	if (tab1!=nullptr)
        tab1->stopAutoUpdate();
	VASTMSEGEditorPane* tab2 = dynamic_cast<VASTMSEGEditorPane*>(c_envTab->getTabContentComponent(1));
    if (tab2!=nullptr)
        tab2->stopAutoUpdate();
	VASTMSEGEditorPane* tab3 = dynamic_cast<VASTMSEGEditorPane*>(c_envTab->getTabContentComponent(2));
    if (tab3!=nullptr)
        tab3->stopAutoUpdate();
	VASTMSEGEditorPane* tab4 = dynamic_cast<VASTMSEGEditorPane*>(c_envTab->getTabContentComponent(3));
    if (tab4!=nullptr)
        tab4->stopAutoUpdate();
	VASTMSEGEditorPane* tab5 = dynamic_cast<VASTMSEGEditorPane*>(c_envTab->getTabContentComponent(4));
    if (tab5!=nullptr)
        tab5->stopAutoUpdate();

	VASTLFOEditorPane* tabl1 = dynamic_cast<VASTLFOEditorPane*>(c_lfoTab->getTabContentComponent(0));
    if (tabl1!=nullptr)
        tabl1->stopAutoUpdate();
	VASTLFOEditorPane* tabl2 = dynamic_cast<VASTLFOEditorPane*>(c_lfoTab->getTabContentComponent(1));
    if (tabl2!=nullptr)
        tabl2->stopAutoUpdate();
	VASTLFOEditorPane* tabl3 = dynamic_cast<VASTLFOEditorPane*>(c_lfoTab->getTabContentComponent(2));
    if (tabl3!=nullptr)
        tabl3->stopAutoUpdate();
	VASTLFOEditorPane* tabl4 = dynamic_cast<VASTLFOEditorPane*>(c_lfoTab->getTabContentComponent(3));
    if (tabl4!=nullptr)
        tabl4->stopAutoUpdate();
	VASTLFOEditorPane* tabl5 = dynamic_cast<VASTLFOEditorPane*>(c_lfoTab->getTabContentComponent(4));
    if (tabl5!=nullptr)
        tabl5->stopAutoUpdate();
}

void VASTLFOMsegComponent::updateAll() {
	VASTMSEGEditorPane* tab1 = ((VASTMSEGEditorPane*)c_envTab->getTabContentComponent(0));
	tab1->updateAll();
	VASTMSEGEditorPane* tab2 = ((VASTMSEGEditorPane*)c_envTab->getTabContentComponent(1));
	tab2->updateAll();
	VASTMSEGEditorPane* tab3 = ((VASTMSEGEditorPane*)c_envTab->getTabContentComponent(2));
	tab3->updateAll();
	VASTMSEGEditorPane* tab4 = ((VASTMSEGEditorPane*)c_envTab->getTabContentComponent(3));
	tab4->updateAll();
	VASTMSEGEditorPane* tab5 = ((VASTMSEGEditorPane*)c_envTab->getTabContentComponent(4));
	tab5->updateAll();
}

void VASTLFOMsegComponent::buttonClicked(Button* buttonThatWasClicked)
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

void VASTLFOMsegComponent::mouseWheelMove(const MouseEvent &event, const MouseWheelDetails &wheel) {
	if (dynamic_cast<TabBarButton*>(event.originalComponent) == 0)
		return;
	if (abs(lastMouseWheelEvent.getMilliseconds() - event.eventTime.getMilliseconds()) < 50)
		return;
	lastMouseWheelEvent = event.eventTime;
	if ((c_envTab->getTabbedButtonBar().getScreenBounds().contains(event.getScreenPosition()))) {
		int curidx = c_envTab->getTabbedButtonBar().getCurrentTabIndex();
		int numidx = c_envTab->getTabbedButtonBar().getNumTabs();
		if (wheel.deltaY > 0.f)
			curidx = (curidx + 1) % numidx;
		else if (wheel.deltaY < 0.f)
			curidx = ((curidx - 1) + numidx) % numidx;
		c_envTab->getTabbedButtonBar().setCurrentTabIndex(curidx);
	}
	else if ((c_lfoTab->getTabbedButtonBar().getScreenBounds().contains(event.getScreenPosition()))) {
		int curidx = c_lfoTab->getTabbedButtonBar().getCurrentTabIndex();
		int numidx = c_lfoTab->getTabbedButtonBar().getNumTabs();
		if (wheel.deltaY > 0.f)
			curidx = (curidx + 1) % numidx;
		else if (wheel.deltaY < 0.f)
			curidx = ((curidx - 1) + numidx) % numidx;
		c_lfoTab->getTabbedButtonBar().setCurrentTabIndex(curidx);
	}
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="VASTLFOMsegComponent" componentName=""
                 parentClasses="public Component, public Button::Listener" constructorParams="AudioProcessorEditor *editor, AudioProcessor* processor"
                 variableInitialisers="myEditor((VASTAudioProcessorEditor*)editor), myProcessor((VASTAudioProcessor*)processor)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="800" initialHeight="575">
  <BACKGROUND backgroundColour="ff323e44">
    <RECT pos="0 0 0M 0M" fill="linear: 10% 34.483%, 44.8% 55.385%, 0=bd4e1a0d, 1=4301080b"
          hasStroke="0"/>
  </BACKGROUND>
  <GENERICCOMPONENT name="c_envTab" id="3379d39a6d294a82" memberName="c_envTab" virtualName="VASTTabbedMSEGComponent"
                    explicitFocusOrder="0" pos="0% 0% 100% 49.913%" class="VASTTabbedMSEGComponent"
                    params="myProcessor,&#10;myEditor, juce::TabbedButtonBar::TabsAtTop"/>
  <GENERICCOMPONENT name="c_iconMaximizeEditor" id="ded548631676712b" memberName="c_iconMaximizeEditor"
                    virtualName="" explicitFocusOrder="0" pos="1Rr 1 1.5% 2.087%"
                    class="VASTDrawableButton" params="&quot;c_iconMaximize&quot;, arrow_right_corner_svg, arrow_right_corner_svgSize, &quot;Maximize area&quot;"/>
  <GENERICCOMPONENT name="c_lfoTab3" id="6964c7089eaacc93" memberName="c_lfoTab"
                    virtualName="VASTTabbedLFOComponent" explicitFocusOrder="0" pos="0% 50.087% 100% 49.913%"
                    class="VASTTabbedLFOComponent" params="myProcessor,&#10;myEditor, juce::TabbedButtonBar::TabsAtTop"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif

//==============================================================================
// Binary resources - be careful not to edit any of these sections!

// JUCER_RESOURCE: arrow_right_corner_svg, 1715, "../../Resources/Icons/arrow_right_corner.svg"
static const unsigned char resource_VASTLFOMsegComponent_arrow_right_corner_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,70,
45,56,34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,32,119,105,100,116,104,61,34,50,48,112,120,34,32,104,101,105,103,104,116,61,34,50,48,112,120,34,32,118,105,101,
119,66,111,120,61,34,48,32,48,32,50,48,32,50,48,34,32,118,101,114,115,105,111,110,61,34,49,46,49,34,32,120,109,108,110,115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,
48,48,47,115,118,103,34,32,120,109,108,110,115,58,120,108,105,110,107,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,120,108,105,110,107,34,62,10,32,32,32,32,60,
33,45,45,32,71,101,110,101,114,97,116,111,114,58,32,83,107,101,116,99,104,32,51,46,56,46,49,32,40,50,57,54,56,55,41,32,45,32,104,116,116,112,58,47,47,119,119,119,46,98,111,104,101,109,105,97,110,99,111,
100,105,110,103,46,99,111,109,47,115,107,101,116,99,104,32,45,45,62,10,32,32,32,32,60,116,105,116,108,101,62,97,114,114,111,119,95,114,105,103,104,116,95,99,111,114,110,101,114,32,91,35,50,55,57,93,60,
47,116,105,116,108,101,62,10,32,32,32,32,60,100,101,115,99,62,67,114,101,97,116,101,100,32,119,105,116,104,32,83,107,101,116,99,104,46,60,47,100,101,115,99,62,10,32,32,32,32,60,100,101,102,115,62,60,47,
100,101,102,115,62,10,32,32,32,32,60,103,32,105,100,61,34,80,97,103,101,45,49,34,32,115,116,114,111,107,101,61,34,110,111,110,101,34,32,115,116,114,111,107,101,45,119,105,100,116,104,61,34,49,34,32,102,
105,108,108,61,34,110,111,110,101,34,32,102,105,108,108,45,114,117,108,101,61,34,101,118,101,110,111,100,100,34,62,10,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,68,114,105,98,98,98,108,101,45,68,
97,114,107,45,80,114,101,118,105,101,119,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,45,49,56,48,46,48,48,48,48,48,48,44,32,45,54,57,49,57,46,48,48,48,48,48,48,
41,34,32,102,105,108,108,61,34,35,70,70,70,70,70,70,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,105,99,111,110,115,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,
115,108,97,116,101,40,53,54,46,48,48,48,48,48,48,44,32,49,54,48,46,48,48,48,48,48,48,41,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,60,112,97,116,104,32,100,61,34,77,49,51,50,44,54,55,54,
52,32,67,49,51,50,44,54,55,54,51,46,52,52,56,32,49,51,50,46,52,52,56,44,54,55,54,51,32,49,51,51,44,54,55,54,51,32,76,49,51,56,44,54,55,54,51,32,67,49,51,57,46,49,48,53,44,54,55,54,51,32,49,52,48,44,54,
55,54,51,46,56,57,53,32,49,52,48,44,54,55,54,53,32,76,49,52,48,44,54,55,55,48,32,67,49,52,48,44,54,55,55,48,46,53,53,50,32,49,51,57,46,53,53,50,44,54,55,55,49,32,49,51,57,44,54,55,55,49,32,67,49,51,56,
46,52,52,56,44,54,55,55,49,32,49,51,56,44,54,55,55,48,46,53,53,50,32,49,51,56,44,54,55,55,48,32,76,49,51,56,44,54,55,54,55,46,54,50,49,32,67,49,51,56,44,54,55,54,55,46,49,55,54,32,49,51,55,46,52,54,49,
44,54,55,54,54,46,57,53,51,32,49,51,55,46,49,52,54,44,54,55,54,55,46,50,54,56,32,76,49,51,52,46,49,50,49,44,54,55,55,48,46,50,57,51,32,67,49,51,51,46,55,51,49,44,54,55,55,48,46,54,56,51,32,49,51,51,46,
48,57,56,44,54,55,55,48,46,54,56,51,32,49,51,50,46,55,48,55,44,54,55,55,48,46,50,57,51,32,67,49,51,50,46,51,49,55,44,54,55,54,57,46,57,48,50,32,49,51,50,46,51,49,55,44,54,55,54,57,46,50,54,57,32,49,51,
50,46,55,48,55,44,54,55,54,56,46,56,55,57,32,76,49,51,53,46,55,51,50,44,54,55,54,53,46,56,53,52,32,67,49,51,54,46,48,52,55,44,54,55,54,53,46,53,51,57,32,49,51,53,46,56,50,52,44,54,55,54,53,32,49,51,53,
46,51,55,57,44,54,55,54,53,32,76,49,51,51,44,54,55,54,53,32,67,49,51,50,46,52,52,56,44,54,55,54,53,32,49,51,50,44,54,55,54,52,46,53,53,50,32,49,51,50,44,54,55,54,52,32,76,49,51,50,44,54,55,54,52,32,90,
32,77,49,50,54,44,54,55,54,50,32,67,49,50,54,44,54,55,54,49,46,52,52,56,32,49,50,54,46,52,52,56,44,54,55,54,49,32,49,50,55,44,54,55,54,49,32,76,49,52,49,44,54,55,54,49,32,67,49,52,49,46,53,53,50,44,54,
55,54,49,32,49,52,50,44,54,55,54,49,46,52,52,56,32,49,52,50,44,54,55,54,50,32,76,49,52,50,44,54,55,55,54,32,67,49,52,50,44,54,55,55,54,46,53,53,50,32,49,52,49,46,53,53,50,44,54,55,55,55,32,49,52,49,44,
54,55,55,55,32,76,49,51,51,44,54,55,55,55,32,67,49,51,50,46,52,52,56,44,54,55,55,55,32,49,51,50,44,54,55,55,54,46,53,53,50,32,49,51,50,44,54,55,55,54,32,76,49,51,50,44,54,55,55,51,32,67,49,51,50,44,54,
55,55,49,46,56,57,53,32,49,51,49,46,49,48,53,44,54,55,55,49,32,49,51,48,44,54,55,55,49,32,76,49,50,55,44,54,55,55,49,32,67,49,50,54,46,52,52,56,44,54,55,55,49,32,49,50,54,44,54,55,55,48,46,53,53,50,32,
49,50,54,44,54,55,55,48,32,76,49,50,54,44,54,55,54,50,32,90,32,77,49,52,52,44,54,55,54,49,32,67,49,52,52,44,54,55,53,57,46,56,57,53,32,49,52,51,46,49,48,53,44,54,55,53,57,32,49,52,50,44,54,55,53,57,32,
76,49,50,54,44,54,55,53,57,32,67,49,50,52,46,56,57,53,44,54,55,53,57,32,49,50,52,44,54,55,53,57,46,56,57,53,32,49,50,52,44,54,55,54,49,32,76,49,50,52,44,54,55,55,55,32,67,49,50,52,44,54,55,55,56,46,49,
48,53,32,49,50,52,46,56,57,53,44,54,55,55,57,32,49,50,54,44,54,55,55,57,32,76,49,52,50,44,54,55,55,57,32,67,49,52,51,46,49,48,53,44,54,55,55,57,32,49,52,52,44,54,55,55,56,46,49,48,53,32,49,52,52,44,54,
55,55,55,32,76,49,52,52,44,54,55,54,49,32,90,34,32,105,100,61,34,97,114,114,111,119,95,114,105,103,104,116,95,99,111,114,110,101,114,45,91,35,50,55,57,93,34,62,60,47,112,97,116,104,62,10,32,32,32,32,32,
32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,60,47,103,62,10,60,47,115,118,103,62,0,0};

const char* VASTLFOMsegComponent::arrow_right_corner_svg = (const char*) resource_VASTLFOMsegComponent_arrow_right_corner_svg;
const int VASTLFOMsegComponent::arrow_right_corner_svgSize = 1715;


//[EndFile] You can add extra defines here...
//[/EndFile]

