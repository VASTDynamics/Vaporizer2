/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 6.0.1

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
#include "../../Engine/VASTEngineHeader.h"
#include "VASTMSEGEditor.h"
#include "../VASTAudioProcessor.h"
#include "../VASTAudioProcessorEditor.h"
#include "../VASTControls/VASTSlider.h"
#include "../VASTControls/VASTButton.h"
#include "../VASTControls//VASTComboBox.h"
#include "../VASTControls/VASTParameterSlider.h"
#include "../VASTControls/VASTParameterButton.h"
#include "../VASTControls/VASTParameterComboBox.h"
#include "../../Engine/VASTMSEGEnvelope.h"
#include "../../Engine/VASTMSEGData.h"
//[/Headers]

#include "VASTARPEditorPane.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
VASTARPEditorPane::VASTARPEditorPane (AudioProcessorEditor *editor, AudioProcessor* processor, VASTARPData* data, VASTARPData* datalive)
    : myEditor((VASTAudioProcessorEditor*)editor), myProcessor((VASTAudioProcessor*)processor), myData(data), myDataLive(datalive)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    label122.reset (new juce::Label ("new label",
                                     TRANS("MODE")));
    addAndMakeVisible (label122.get());
    label122->setFont (juce::Font (11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label122->setJustificationType (juce::Justification::centredRight);
    label122->setEditable (false, false, false);
    label122->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label122->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label122->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label4.reset (new juce::Label ("new label",
                                   TRANS("ON/OFF")));
    addAndMakeVisible (label4.get());
    label4->setFont (juce::Font ("Syntax", 11.00f, juce::Font::plain));
    label4->setJustificationType (juce::Justification::centredRight);
    label4->setEditable (false, false, false);
    label4->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label4->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label4->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    c_arpEditor.reset (new VASTARPEditor (myProcessor, myData, myDataLive));
    addAndMakeVisible (c_arpEditor.get());
    c_arpEditor->setName ("c_arpEditor");

    label3.reset (new juce::Label ("new label",
                                   TRANS("STEPS")));
    addAndMakeVisible (label3.get());
    label3->setFont (juce::Font ("Code Pro Demo", 11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label3->setJustificationType (juce::Justification::centredRight);
    label3->setEditable (false, false, false);
    label3->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label3->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label3->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label108.reset (new juce::Label ("new label",
                                     TRANS("BEATS (DAW)")));
    addAndMakeVisible (label108.get());
    label108->setFont (juce::Font (11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label108->setJustificationType (juce::Justification::centredRight);
    label108->setEditable (false, false, false);
    label108->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label108->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label108->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label2.reset (new juce::Label ("new label",
                                   TRANS("SYNC")));
    addAndMakeVisible (label2.get());
    label2->setFont (juce::Font ("Code Pro Demo", 11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label2->setJustificationType (juce::Justification::centredRight);
    label2->setEditable (false, false, false);
    label2->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label2->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label2->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label7.reset (new juce::Label ("new label",
                                   TRANS("SPEED")));
    addAndMakeVisible (label7.get());
    label7->setFont (juce::Font ("Code Pro Demo", 11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label7->setJustificationType (juce::Justification::centredRight);
    label7->setEditable (false, false, false);
    label7->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label7->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label7->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    m_uARPTimeBeats.reset (new VASTParameterComboBox ("m_uARPTimeBeats"));
    addAndMakeVisible (m_uARPTimeBeats.get());
    m_uARPTimeBeats->setTooltip (TRANS("LFO time in beats when synched to DAW"));
    m_uARPTimeBeats->setEditableText (false);
    m_uARPTimeBeats->setJustificationType (juce::Justification::centredLeft);
    m_uARPTimeBeats->setTextWhenNothingSelected (TRANS("choose env mode"));
    m_uARPTimeBeats->setTextWhenNoChoicesAvailable (TRANS("none"));
    m_uARPTimeBeats->addItem (TRANS("UNIPOLAR"), 1);
    m_uARPTimeBeats->addItem (TRANS("BIPOLAR"), 2);
    m_uARPTimeBeats->addListener (this);

    c_ARPPattern.reset (new VASTComboBox ("c_ARPPattern"));
    addAndMakeVisible (c_ARPPattern.get());
    c_ARPPattern->setTooltip (TRANS("Arpeggiator patterns (will be editable soon)"));
    c_ARPPattern->setEditableText (false);
    c_ARPPattern->setJustificationType (juce::Justification::centredLeft);
    c_ARPPattern->setTextWhenNothingSelected (TRANS("---"));
    c_ARPPattern->setTextWhenNoChoicesAvailable (TRANS("---"));
    c_ARPPattern->addListener (this);

    m_bARPOnOff.reset (new VASTParameterButton ("m_bARPOnOff"));
    addAndMakeVisible (m_bARPOnOff.get());
    m_bARPOnOff->setTooltip (TRANS("Switch arpeggiator on / off"));
    m_bARPOnOff->setButtonText (juce::String());
    m_bARPOnOff->addListener (this);

    m_uARPMode.reset (new VASTParameterComboBox ("m_uARPMode"));
    addAndMakeVisible (m_uARPMode.get());
    m_uARPMode->setTooltip (TRANS("Arpeggiator mode"));
    m_uARPMode->setEditableText (false);
    m_uARPMode->setJustificationType (juce::Justification::centredLeft);
    m_uARPMode->setTextWhenNothingSelected (TRANS("---"));
    m_uARPMode->setTextWhenNoChoicesAvailable (TRANS("---"));
    m_uARPMode->addItem (TRANS("Up"), 1);
    m_uARPMode->addItem (TRANS("Down"), 2);
    m_uARPMode->addItem (TRANS("Up-Down"), 3);
    m_uARPMode->addListener (this);

    c_numSteps.reset (new VASTSlider ("c_numSteps"));
    addAndMakeVisible (c_numSteps.get());
    c_numSteps->setTooltip (TRANS("Number of steps in arpeggiatorr"));
    c_numSteps->setExplicitFocusOrder (1);
    c_numSteps->setRange (1, 64, 1);
    c_numSteps->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    c_numSteps->setTextBoxStyle (juce::Slider::NoTextBox, false, 30, 14);
    c_numSteps->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffc9c9c9));
    c_numSteps->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0x00ffffff));
    c_numSteps->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x00808080));
    c_numSteps->addListener (this);

    m_fARPSpeed.reset (new VASTParameterSlider ("m_fARPSpeed"));
    addAndMakeVisible (m_fARPSpeed.get());
    m_fARPSpeed->setTooltip (TRANS("Arpeggiator speed in BPM (beats per minute)"));
    m_fARPSpeed->setRange (5, 3000, 0.01);
    m_fARPSpeed->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    m_fARPSpeed->setTextBoxStyle (juce::Slider::NoTextBox, false, 40, 16);
    m_fARPSpeed->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffc9c9c9));
    m_fARPSpeed->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0x00ffffff));
    m_fARPSpeed->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x00808080));
    m_fARPSpeed->addListener (this);

    m_bARPSynch.reset (new VASTParameterButton ("m_bARPSynch"));
    addAndMakeVisible (m_bARPSynch.get());
    m_bARPSynch->setTooltip (TRANS("Synch ARP to DAW timecode (starts when played in DAW)"));
    m_bARPSynch->setButtonText (juce::String());
    m_bARPSynch->addListener (this);


    //[UserPreSize]
	//manage parameter bindings -> set ComponentID = Name and processor
	Array<Component*> children = this->getChildren();
	for (auto child : children) {
		if (child->getName().startsWith("m_")) {
			auto* aSlider = dynamic_cast<VASTParameterSlider*> (child);
			if (aSlider != nullptr) {
				aSlider->setAudioProcessor(*myProcessor);
				aSlider->bindParameter(myEditor, aSlider->getName(), VASTGUIRuntimeModel::GUIComponents::ARPEditorPane, 0); //no suffix
			}
			auto* aCombobox = dynamic_cast<VASTParameterComboBox*> (child);
			if (aCombobox != nullptr) {
				aCombobox->setAudioProcessor(*myProcessor);
				aCombobox->bindParameter(myEditor, aCombobox->getName(), VASTGUIRuntimeModel::GUIComponents::ARPEditorPane, 0); //no suffix
			}
			auto* aButton = dynamic_cast<VASTParameterButton*> (child);
			if (aButton != nullptr) {
				aButton->setAudioProcessor(*myProcessor);
				aButton->bindParameter(myEditor, aButton->getName(), VASTGUIRuntimeModel::GUIComponents::ARPEditorPane, 0); //no suffix
			}
		}
	}

	//c_ARPPattern->setText(myData->patternName);
	c_ARPPattern->addItemList(myData->getDefaultPatternNames(), 1);
	//c_ARPPattern->setSelectedId(1, true); //deprecated
    c_ARPPattern->setSelectedId(1, NotificationType::dontSendNotification);

	setOpaque(true); //avoid repaints of parents
	c_numSteps->setSkewFactor(0.3f, false);
	c_numSteps->setDefaultValue(4);
	c_numSteps->setAudioProcessor(*myProcessor);

    //[/UserPreSize]

    setSize (800, 287);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

VASTARPEditorPane::~VASTARPEditorPane()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    label122 = nullptr;
    label4 = nullptr;
    c_arpEditor = nullptr;
    label3 = nullptr;
    label108 = nullptr;
    label2 = nullptr;
    label7 = nullptr;
    m_uARPTimeBeats = nullptr;
    c_ARPPattern = nullptr;
    m_bARPOnOff = nullptr;
    m_uARPMode = nullptr;
    c_numSteps = nullptr;
    m_fARPSpeed = nullptr;
    m_bARPSynch = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void VASTARPEditorPane::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
	c_numSteps->setValue(myData->getNumSteps(), juce::NotificationType::dontSendNotification);
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff323e44));

    {
        int x = 0, y = proportionOfHeight (0.0000f), width = getWidth() - 0, height = getHeight() - 0;
        juce::Colour fillColour1 = juce::Colour (0x50494847), fillColour2 = juce::Colour (0xff141516);
        //[UserPaintCustomArguments] Customize the painting arguments here..
    		fillColour1 = myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colARPEditorPaneBackgroundGradientFrom);
    		fillColour2 = myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colARPEditorPaneBackgroundGradientTo);
        //[/UserPaintCustomArguments]
        g.setGradientFill (juce::ColourGradient (fillColour1,
                                             static_cast<float> (-16) - 0.0f + x,
                                             static_cast<float> (-504) - static_cast<float> (proportionOfHeight (0.0000f)) + y,
                                             fillColour2,
                                             static_cast<float> (getWidth()) - 0.0f + x,
                                             static_cast<float> (getHeight()) - static_cast<float> (proportionOfHeight (0.0000f)) + y,
                                             false));
        g.fillRect (x, y, width, height);
    }

    {
        int x = 0, y = 0, width = getWidth() - 0, height = proportionOfHeight (0.1847f);
        juce::Colour fillColour1 = juce::Colour (0xff3f4446), fillColour2 = juce::Colour (0xff4c5457);
        //[UserPaintCustomArguments] Customize the painting arguments here..
    		fillColour1 = myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colARPEditorPaneHeaderGradientFrom);
    		fillColour2 = myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colARPEditorPaneHeaderGradientTo);
        //[/UserPaintCustomArguments]
        g.setGradientFill (juce::ColourGradient (fillColour1,
                                             0.0f - 0.0f + x,
                                             0.0f - 0.0f + y,
                                             fillColour2,
                                             static_cast<float> (getWidth()) - 0.0f + x,
                                             static_cast<float> (getHeight()) - 0.0f + y,
                                             false));
        g.fillRect (x, y, width, height);
    }

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void VASTARPEditorPane::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    label122->setBounds (proportionOfWidth (0.2675f), proportionOfHeight (0.0836f), proportionOfWidth (0.0963f), proportionOfHeight (0.0383f));
    label4->setBounds (proportionOfWidth (0.1650f), proportionOfHeight (0.0836f), proportionOfWidth (0.0963f), proportionOfHeight (0.0383f));
    c_arpEditor->setBounds (proportionOfWidth (0.0000f), proportionOfHeight (0.1847f), proportionOfWidth (1.0000f), proportionOfHeight (0.8153f));
    label3->setBounds (proportionOfWidth (0.4363f), proportionOfHeight (0.0836f), proportionOfWidth (0.0963f), proportionOfHeight (0.0383f));
    label108->setBounds (proportionOfWidth (0.8363f), proportionOfHeight (0.0836f), proportionOfWidth (0.0963f), proportionOfHeight (0.0383f));
    label2->setBounds (proportionOfWidth (0.7200f), proportionOfHeight (0.0836f), proportionOfWidth (0.0963f), proportionOfHeight (0.0383f));
    label7->setBounds (proportionOfWidth (0.5238f), proportionOfHeight (0.0836f), proportionOfWidth (0.0963f), proportionOfHeight (0.0383f));
    m_uARPTimeBeats->setBounds (proportionOfWidth (0.9350f), proportionOfHeight (0.0801f), proportionOfWidth (0.0500f), proportionOfHeight (0.0488f));
    c_ARPPattern->setBounds (proportionOfWidth (0.0125f), proportionOfHeight (0.0801f), proportionOfWidth (0.1750f), proportionOfHeight (0.0488f));
    m_bARPOnOff->setBounds (proportionOfWidth (0.2625f), proportionOfHeight (0.0697f), proportionOfWidth (0.0225f), proportionOfHeight (0.0627f));
    m_uARPMode->setBounds (proportionOfWidth (0.3675f), proportionOfHeight (0.0801f), proportionOfWidth (0.0900f), proportionOfHeight (0.0488f));
    c_numSteps->setBounds (proportionOfWidth (0.5250f), proportionOfHeight (0.0279f), proportionOfWidth (0.0500f), proportionOfHeight (0.1394f));
    m_fARPSpeed->setBounds (proportionOfWidth (0.6125f), proportionOfHeight (0.0279f), proportionOfWidth (0.0500f), proportionOfHeight (0.1394f));
    m_bARPSynch->setBounds (proportionOfWidth (0.8163f), proportionOfHeight (0.0697f), proportionOfWidth (0.0225f), proportionOfHeight (0.0627f));
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void VASTARPEditorPane::comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == m_uARPTimeBeats.get())
    {
        //[UserComboBoxCode_m_uARPTimeBeats] -- add your combo box handling code here..
		myData->setDirty();
		repaint();
        //[/UserComboBoxCode_m_uARPTimeBeats]
    }
    else if (comboBoxThatHasChanged == c_ARPPattern.get())
    {
        //[UserComboBoxCode_c_ARPPattern] -- add your combo box handling code here..
		myData->initDefaultPattern(c_ARPPattern->getSelectedId() - 1);
		repaint();
        //[/UserComboBoxCode_c_ARPPattern]
    }
    else if (comboBoxThatHasChanged == m_uARPMode.get())
    {
        //[UserComboBoxCode_m_uARPMode] -- add your combo box handling code here..
		myData->setDirty();
		repaint();
        //[/UserComboBoxCode_m_uARPMode]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}

void VASTARPEditorPane::buttonClicked (juce::Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == m_bARPOnOff.get())
    {
        //[UserButtonCode_m_bARPOnOff] -- add your button handler code here..
		myData->setDirty();
		repaint();
        //[/UserButtonCode_m_bARPOnOff]
    }
    else if (buttonThatWasClicked == m_bARPSynch.get())
    {
        //[UserButtonCode_m_bARPSynch] -- add your button handler code here..
		myData->setDirty();
		repaint();
        //[/UserButtonCode_m_bARPSynch]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void VASTARPEditorPane::sliderValueChanged (juce::Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == c_numSteps.get())
    {
        //[UserSliderCode_c_numSteps] -- add your slider handling code here..
		myData->arpChangeSteps(c_numSteps.get()->getValue());
		repaint();
        //[/UserSliderCode_c_numSteps]
    }
    else if (sliderThatWasMoved == m_fARPSpeed.get())
    {
        //[UserSliderCode_m_fARPSpeed] -- add your slider handling code here..
		myData->setDirty();
		repaint();
        //[/UserSliderCode_m_fARPSpeed]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void VASTARPEditorPane::initAll() {
    c_ARPPattern->setSelectedId(1, NotificationType::dontSendNotification);
}

void VASTARPEditorPane::startAutoUpdate() {
	c_arpEditor->startAutoUpdate();
}
void VASTARPEditorPane::stopAutoUpdate() {
	c_arpEditor->stopAutoUpdate();
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="VASTARPEditorPane" componentName=""
                 parentClasses="public Component" constructorParams="AudioProcessorEditor *editor, AudioProcessor* processor, VASTARPData* data, VASTARPData* datalive"
                 variableInitialisers="myEditor((VASTAudioProcessorEditor*)editor), myProcessor((VASTAudioProcessor*)processor), myData(data), myDataLive(datalive) "
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="800" initialHeight="287">
  <BACKGROUND backgroundColour="ff323e44">
    <RECT pos="0 0% 0M 0M" fill="linear: -16 -504, 0R 0R, 0=50494847, 1=ff141516"
          hasStroke="0"/>
    <RECT pos="0 0 0M 18.467%" fill="linear: 0 0, 0R 0R, 0=ff3f4446, 1=ff4c5457"
          hasStroke="0"/>
  </BACKGROUND>
  <LABEL name="new label" id="2a6c4b3dd0eaee89" memberName="label122"
         virtualName="" explicitFocusOrder="0" pos="26.75% 8.362% 9.625% 3.833%"
         textCol="ffe2e2e2" edTextCol="ff000000" edBkgCol="0" labelText="MODE"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="11.0" kerning="0.0" bold="0"
         italic="0" justification="34"/>
  <LABEL name="new label" id="2ccc9143e2616ccd" memberName="label4" virtualName=""
         explicitFocusOrder="0" pos="16.5% 8.362% 9.625% 3.833%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="ON/OFF" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Syntax"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="34"/>
  <GENERICCOMPONENT name="c_arpEditor" id="5eb93e0013882f07" memberName="c_arpEditor"
                    virtualName="" explicitFocusOrder="0" pos="0% 18.467% 100% 81.533%"
                    class="VASTARPEditor" params="myProcessor, myData, myDataLive"/>
  <LABEL name="new label" id="b688b6b2463f3740" memberName="label3" virtualName=""
         explicitFocusOrder="0" pos="43.625% 8.362% 9.625% 3.833%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="STEPS" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Code Pro Demo"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="34"/>
  <LABEL name="new label" id="9050920a08f3ea65" memberName="label108"
         virtualName="" explicitFocusOrder="0" pos="83.625% 8.362% 9.625% 3.833%"
         textCol="ffe2e2e2" edTextCol="ff000000" edBkgCol="0" labelText="BEATS (DAW)"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="11.0" kerning="0.0" bold="0"
         italic="0" justification="34"/>
  <LABEL name="new label" id="852debe5e5f76751" memberName="label2" virtualName=""
         explicitFocusOrder="0" pos="72% 8.362% 9.625% 3.833%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="SYNC" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Code Pro Demo"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="34"/>
  <LABEL name="new label" id="8b1c744c8c6502ef" memberName="label7" virtualName=""
         explicitFocusOrder="0" pos="52.375% 8.362% 9.625% 3.833%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="SPEED" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Code Pro Demo"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="34"/>
  <COMBOBOX name="m_uARPTimeBeats" id="e036b1cae3e29aa2" memberName="m_uARPTimeBeats"
            virtualName="VASTParameterComboBox" explicitFocusOrder="0" pos="93.5% 8.014% 5% 4.878%"
            tooltip="LFO time in beats when synched to DAW" editable="0"
            layout="33" items="UNIPOLAR&#10;BIPOLAR" textWhenNonSelected="choose env mode"
            textWhenNoItems="none"/>
  <COMBOBOX name="c_ARPPattern" id="3aa9d153b6e7e5c5" memberName="c_ARPPattern"
            virtualName="VASTComboBox" explicitFocusOrder="0" pos="1.25% 8.014% 17.5% 4.878%"
            tooltip="Arpeggiator patterns (will be editable soon)" editable="0"
            layout="33" items="" textWhenNonSelected="---" textWhenNoItems="---"/>
  <TOGGLEBUTTON name="m_bARPOnOff" id="1d94b90683d2fb39" memberName="m_bARPOnOff"
                virtualName="VASTParameterButton" explicitFocusOrder="0" pos="26.25% 6.969% 2.25% 6.272%"
                tooltip="Switch arpeggiator on / off" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <COMBOBOX name="m_uARPMode" id="ba370ce03964de1d" memberName="m_uARPMode"
            virtualName="VASTParameterComboBox" explicitFocusOrder="0" pos="36.75% 8.014% 9% 4.878%"
            tooltip="Arpeggiator mode" editable="0" layout="33" items="Up&#10;Down&#10;Up-Down"
            textWhenNonSelected="---" textWhenNoItems="---"/>
  <SLIDER name="c_numSteps" id="e40a662fd508c4e5" memberName="c_numSteps"
          virtualName="VASTSlider" explicitFocusOrder="1" pos="52.5% 2.787% 5% 13.937%"
          tooltip="Number of steps in arpeggiatorr" textboxtext="ffc9c9c9"
          textboxbkgd="ffffff" textboxoutline="808080" min="1.0" max="64.0"
          int="1.0" style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="30" textBoxHeight="14" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="m_fARPSpeed" id="8181553dd200d4e0" memberName="m_fARPSpeed"
          virtualName="VASTParameterSlider" explicitFocusOrder="0" pos="61.25% 2.787% 5% 13.937%"
          tooltip="Arpeggiator speed in BPM (beats per minute)" textboxtext="ffc9c9c9"
          textboxbkgd="ffffff" textboxoutline="808080" min="5.0" max="3000.0"
          int="0.01" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="40" textBoxHeight="16" skewFactor="1.0"
          needsCallback="1"/>
  <TOGGLEBUTTON name="m_bARPSynch" id="2a4a8c0b3fb3a16a" memberName="m_bARPSynch"
                virtualName="VASTParameterButton" explicitFocusOrder="0" pos="81.625% 6.969% 2.25% 6.272%"
                tooltip="Synch ARP to DAW timecode (starts when played in DAW)"
                buttonText="" connectedEdges="0" needsCallback="1" radioGroupId="0"
                state="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

