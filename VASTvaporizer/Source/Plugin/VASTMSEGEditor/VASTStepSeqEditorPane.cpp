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
#include "../../Engine/VASTEngineHeader.h"
#include "VASTStepSeqEditor.h"
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

#include "VASTStepSeqEditorPane.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
VASTStepSeqEditorPane::VASTStepSeqEditorPane (AudioProcessorEditor *editor, AudioProcessor* processor, VASTMSEGData* data, VASTMSEGData* datalive, String parameterSuffix, int stepSeqNo)
    : myEditor((VASTAudioProcessorEditor*)editor), myProcessor((VASTAudioProcessor*)processor), myData(data), myDataLive(datalive), mySuffix(parameterSuffix), myStepSeqNo(stepSeqNo)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    c_stepseqEditor.reset (new VASTStepSeqEditor (myProcessor, myData, myDataLive));
    addAndMakeVisible (c_stepseqEditor.get());
    c_stepseqEditor->setName ("c_stepseqEditor");

    c_loadEnv.reset (new VASTComboBox ("c_loadEnv"));
    addAndMakeVisible (c_loadEnv.get());
    c_loadEnv->setEditableText (false);
    c_loadEnv->setJustificationType (juce::Justification::centredLeft);
    c_loadEnv->setTextWhenNothingSelected (TRANS ("load env"));
    c_loadEnv->setTextWhenNoChoicesAvailable (TRANS ("none"));
    c_loadEnv->addItem (TRANS ("Default pattern"), 1);
    c_loadEnv->addItem (TRANS ("Sidechain"), 2);
    c_loadEnv->addItem (TRANS ("Stairs"), 3);
    c_loadEnv->addListener (this);

    label3.reset (new juce::Label ("new label",
                                   TRANS ("STEPS")));
    addAndMakeVisible (label3.get());
    label3->setFont (juce::Font ("Code Pro Demo", 11.00f, juce::Font::plain));
    label3->setJustificationType (juce::Justification::centredRight);
    label3->setEditable (false, false, false);
    label3->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label3->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label3->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label4.reset (new juce::Label ("new label",
                                   TRANS ("GLIDE")));
    addAndMakeVisible (label4.get());
    label4->setFont (juce::Font ("Code Pro Demo", 11.00f, juce::Font::plain));
    label4->setJustificationType (juce::Justification::centredRight);
    label4->setEditable (false, false, false);
    label4->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label4->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label4->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label5.reset (new juce::Label ("new label",
                                   TRANS ("GATE")));
    addAndMakeVisible (label5.get());
    label5->setFont (juce::Font ("Code Pro Demo", 11.00f, juce::Font::plain));
    label5->setJustificationType (juce::Justification::centredRight);
    label5->setEditable (false, false, false);
    label5->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label5->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label5->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label108.reset (new juce::Label ("new label",
                                     TRANS ("BEATS (DAW)")));
    addAndMakeVisible (label108.get());
    label108->setFont (juce::Font (juce::Font::getDefaultSansSerifFontName(), 11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label108->setJustificationType (juce::Justification::centredRight);
    label108->setEditable (false, false, false);
    label108->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label108->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label108->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label2.reset (new juce::Label ("new label",
                                   TRANS ("SYNC")));
    addAndMakeVisible (label2.get());
    label2->setFont (juce::Font ("Code Pro Demo", 11.00f, juce::Font::plain));
    label2->setJustificationType (juce::Justification::centredRight);
    label2->setEditable (false, false, false);
    label2->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label2->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label2->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label7.reset (new juce::Label ("new label",
                                   TRANS ("SPEED")));
    addAndMakeVisible (label7.get());
    label7->setFont (juce::Font ("Code Pro Demo", 11.00f, juce::Font::plain));
    label7->setJustificationType (juce::Justification::centredRight);
    label7->setEditable (false, false, false);
    label7->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label7->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label7->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    m_uStepSeqTimeBeats.reset (new VASTParameterComboBox ("m_uStepSeqTimeBeats"));
    addAndMakeVisible (m_uStepSeqTimeBeats.get());
    m_uStepSeqTimeBeats->setTooltip (TRANS ("Step sequencer time in beats when synched to DAW"));
    m_uStepSeqTimeBeats->setEditableText (false);
    m_uStepSeqTimeBeats->setJustificationType (juce::Justification::centredLeft);
    m_uStepSeqTimeBeats->setTextWhenNothingSelected (TRANS ("choose env mode"));
    m_uStepSeqTimeBeats->setTextWhenNoChoicesAvailable (TRANS ("none"));
    m_uStepSeqTimeBeats->addItem (TRANS ("UNIPOLAR"), 1);
    m_uStepSeqTimeBeats->addItem (TRANS ("BIPOLAR"), 2);
    m_uStepSeqTimeBeats->addListener (this);

    m_bStepSeqSynch.reset (new VASTParameterButton ("m_bStepSeqSynch"));
    addAndMakeVisible (m_bStepSeqSynch.get());
    m_bStepSeqSynch->setTooltip (TRANS ("Synch LFO to DAW timecode (starts when played in DAW)"));
    m_bStepSeqSynch->setButtonText (juce::String());
    m_bStepSeqSynch->addListener (this);

    m_fStepSeqSpeed.reset (new VASTParameterSlider ("m_fStepSeqSpeed"));
    addAndMakeVisible (m_fStepSeqSpeed.get());
    m_fStepSeqSpeed->setTooltip (TRANS ("Step sequencer speed"));
    m_fStepSeqSpeed->setExplicitFocusOrder (1);
    m_fStepSeqSpeed->setRange (0, 100, 0.01);
    m_fStepSeqSpeed->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    m_fStepSeqSpeed->setTextBoxStyle (juce::Slider::NoTextBox, false, 30, 14);
    m_fStepSeqSpeed->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffc9c9c9));
    m_fStepSeqSpeed->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0x00ffffff));
    m_fStepSeqSpeed->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x00808080));
    m_fStepSeqSpeed->addListener (this);

    m_fGate.reset (new VASTParameterSlider ("m_fGate"));
    addAndMakeVisible (m_fGate.get());
    m_fGate->setTooltip (TRANS ("Gate amount step sequencer "));
    m_fGate->setExplicitFocusOrder (1);
    m_fGate->setRange (0, 100, 0.01);
    m_fGate->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    m_fGate->setTextBoxStyle (juce::Slider::NoTextBox, false, 30, 14);
    m_fGate->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffc9c9c9));
    m_fGate->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0x00ffffff));
    m_fGate->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x00808080));
    m_fGate->addListener (this);

    m_fGlide.reset (new VASTParameterSlider ("m_fGlide"));
    addAndMakeVisible (m_fGlide.get());
    m_fGlide->setTooltip (TRANS ("Glide mode time step sequencer"));
    m_fGlide->setRange (0, 100, 0.01);
    m_fGlide->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    m_fGlide->setTextBoxStyle (juce::Slider::NoTextBox, false, 30, 14);
    m_fGlide->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffc9c9c9));
    m_fGlide->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0x00ffffff));
    m_fGlide->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x00808080));
    m_fGlide->addListener (this);

    c_numSteps.reset (new VASTSlider ("c_numSteps"));
    addAndMakeVisible (c_numSteps.get());
    c_numSteps->setTooltip (TRANS ("Number of steps in step sequencer"));
    c_numSteps->setRange (1, 64, 1);
    c_numSteps->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    c_numSteps->setTextBoxStyle (juce::Slider::NoTextBox, false, 30, 14);
    c_numSteps->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffc9c9c9));
    c_numSteps->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0x00ffffff));
    c_numSteps->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x00808080));
    c_numSteps->addListener (this);

    label6.reset (new juce::Label ("new label",
                                   TRANS ("INVERT")));
    addAndMakeVisible (label6.get());
    label6->setFont (juce::Font ("Code Pro Demo", 11.00f, juce::Font::plain));
    label6->setJustificationType (juce::Justification::centredRight);
    label6->setEditable (false, false, false);
    label6->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label6->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label6->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    c_StepSeqInvert.reset (new VASTButton ("c_StepSeqInvert"));
    addAndMakeVisible (c_StepSeqInvert.get());
    c_StepSeqInvert->setTooltip (TRANS ("Invert step sequencer curve"));
    c_StepSeqInvert->setExplicitFocusOrder (1);
    c_StepSeqInvert->setButtonText (juce::String());
    c_StepSeqInvert->addListener (this);


    //[UserPreSize]
	//manage parameter bindings -> set ComponentID = Name and processor
	Array<Component*> children = this->getChildren();
	for (auto child : children) {
		if (child->getName().startsWith("m_")) {
			auto* aSlider = dynamic_cast<VASTParameterSlider*> (child);
			if (aSlider != nullptr) {
				aSlider->setAudioProcessor(*myProcessor);
				aSlider->bindParameter(myEditor, aSlider->getName() + "_" + mySuffix, VASTGUIRuntimeModel::GUIComponents::StepSeqEditorPane, myStepSeqNo);
			}
			auto* aCombobox = dynamic_cast<VASTParameterComboBox*> (child);
			if (aCombobox != nullptr) {
				aCombobox->setAudioProcessor(*myProcessor);
				aCombobox->bindParameter(myEditor, aCombobox->getName() + "_" + mySuffix, VASTGUIRuntimeModel::GUIComponents::StepSeqEditorPane, myStepSeqNo);
			}
			auto* aButton = dynamic_cast<VASTParameterButton*> (child);
			if (aButton != nullptr) {
				aButton->setAudioProcessor(*myProcessor);
				aButton->bindParameter(myEditor, aButton->getName() + "_" + mySuffix, VASTGUIRuntimeModel::GUIComponents::StepSeqEditorPane, myStepSeqNo);
			}
		}
	}

	c_loadEnv->setSelectedItemIndex(0, NotificationType::dontSendNotification);
	myData->patternName = "Default pattern";
	c_loadEnv->setText(myData->patternName);
	setOpaque(true); //avoid repaints of parents

	c_numSteps->setDefaultValue(4);
	c_numSteps->setValue(4, NotificationType::dontSendNotification);
	c_numSteps->setAudioProcessor(*myProcessor);
	c_numSteps->setEditor(*myEditor);
	c_numSteps->setSkewFactor(0.3f, false);
    //[/UserPreSize]

    setSize (800, 287);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

VASTStepSeqEditorPane::~VASTStepSeqEditorPane()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    c_stepseqEditor = nullptr;
    c_loadEnv = nullptr;
    label3 = nullptr;
    label4 = nullptr;
    label5 = nullptr;
    label108 = nullptr;
    label2 = nullptr;
    label7 = nullptr;
    m_uStepSeqTimeBeats = nullptr;
    m_bStepSeqSynch = nullptr;
    m_fStepSeqSpeed = nullptr;
    m_fGate = nullptr;
    m_fGlide = nullptr;
    c_numSteps = nullptr;
    label6 = nullptr;
    c_StepSeqInvert = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void VASTStepSeqEditorPane::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff323e44));

    {
        int x = 0, y = proportionOfHeight (0.0000f), width = getWidth() - 0, height = getHeight() - 0;
        juce::Colour fillColour1 = juce::Colour (0x50bc6445), fillColour2 = juce::Colour (0xff141516);
        //[UserPaintCustomArguments] Customize the painting arguments here..
    		fillColour1 = myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colStepSeqEditorPaneBackgroundGradientFrom);
    		fillColour2 = myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colStepSeqEditorPaneBackgroundGradientTo);
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
    		fillColour1 = myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colStepSeqEditorPaneHeaderGradientFrom);
    		fillColour2 = myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colStepSeqEditorPaneHeaderGradientTo);
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

void VASTStepSeqEditorPane::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    c_stepseqEditor->setBounds (proportionOfWidth (0.0000f), proportionOfHeight (0.1847f), proportionOfWidth (1.0000f), proportionOfHeight (0.8153f));
    c_loadEnv->setBounds (proportionOfWidth (0.0125f), proportionOfHeight (0.0801f), proportionOfWidth (0.1750f), proportionOfHeight (0.0488f));
    label3->setBounds (proportionOfWidth (0.2562f), proportionOfHeight (0.0836f), proportionOfWidth (0.0963f), proportionOfHeight (0.0383f));
    label4->setBounds (proportionOfWidth (0.3450f), proportionOfHeight (0.0836f), proportionOfWidth (0.0963f), proportionOfHeight (0.0383f));
    label5->setBounds (proportionOfWidth (0.4338f), proportionOfHeight (0.0836f), proportionOfWidth (0.0963f), proportionOfHeight (0.0383f));
    label108->setBounds (proportionOfWidth (0.8363f), proportionOfHeight (0.0836f), proportionOfWidth (0.0963f), proportionOfHeight (0.0383f));
    label2->setBounds (proportionOfWidth (0.7200f), proportionOfHeight (0.0836f), proportionOfWidth (0.0963f), proportionOfHeight (0.0383f));
    label7->setBounds (proportionOfWidth (0.5238f), proportionOfHeight (0.0836f), proportionOfWidth (0.0963f), proportionOfHeight (0.0383f));
    m_uStepSeqTimeBeats->setBounds (proportionOfWidth (0.9350f), proportionOfHeight (0.0801f), proportionOfWidth (0.0500f), proportionOfHeight (0.0488f));
    m_bStepSeqSynch->setBounds (proportionOfWidth (0.8163f), proportionOfHeight (0.0697f), proportionOfWidth (0.0225f), proportionOfHeight (0.0662f));
    m_fStepSeqSpeed->setBounds (proportionOfWidth (0.6125f), proportionOfHeight (0.0279f), proportionOfWidth (0.0500f), proportionOfHeight (0.1394f));
    m_fGate->setBounds (proportionOfWidth (0.5250f), proportionOfHeight (0.0279f), proportionOfWidth (0.0500f), proportionOfHeight (0.1394f));
    m_fGlide->setBounds (proportionOfWidth (0.4375f), proportionOfHeight (0.0279f), proportionOfWidth (0.0500f), proportionOfHeight (0.1394f));
    c_numSteps->setBounds (proportionOfWidth (0.3500f), proportionOfHeight (0.0279f), proportionOfWidth (0.0500f), proportionOfHeight (0.1394f));
    label6->setBounds (proportionOfWidth (0.1625f), proportionOfHeight (0.0836f), proportionOfWidth (0.0963f), proportionOfHeight (0.0383f));
    c_StepSeqInvert->setBounds (proportionOfWidth (0.2600f), proportionOfHeight (0.0697f), proportionOfWidth (0.0225f), proportionOfHeight (0.0662f));
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void VASTStepSeqEditorPane::comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == c_loadEnv.get())
    {
        //[UserComboBoxCode_c_loadEnv] -- add your combo box handling code here..

		if (c_loadEnv->getText().equalsIgnoreCase("Default pattern")) {
			myData->initStepSeq(myData->m_stepSeqNo);
		}
		else if (c_loadEnv->getText().equalsIgnoreCase("Sidechain")) {
			myData->initStepSeqSidechain();
		}
		else if (c_loadEnv->getText().equalsIgnoreCase("Stairs")) {
			myData->initStepSeqStairs();
		}
		c_numSteps->setValue(myData->getStepSeqSteps(), dontSendNotification);
		c_StepSeqInvert->setToggleState(myData->getInvert(), dontSendNotification);
		m_fGlide->setValue(myData->getStepSeqGlide(), sendNotification);
		m_fGate->setValue(myData->getStepSeqGate(), sendNotification);

        //[/UserComboBoxCode_c_loadEnv]
    }
    else if (comboBoxThatHasChanged == m_uStepSeqTimeBeats.get())
    {
        //[UserComboBoxCode_m_uStepSeqTimeBeats] -- add your combo box handling code here..
        //[/UserComboBoxCode_m_uStepSeqTimeBeats]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}

void VASTStepSeqEditorPane::buttonClicked (juce::Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == m_bStepSeqSynch.get())
    {
        //[UserButtonCode_m_bStepSeqSynch] -- add your button handler code here..
        //[/UserButtonCode_m_bStepSeqSynch]
    }
    else if (buttonThatWasClicked == c_StepSeqInvert.get())
    {
        //[UserButtonCode_c_StepSeqInvert] -- add your button handler code here..
		//myData is data changed - must not change data live here!
		myData->setInvert(c_StepSeqInvert->getToggleState());
        //[/UserButtonCode_c_StepSeqInvert]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void VASTStepSeqEditorPane::sliderValueChanged (juce::Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == m_fStepSeqSpeed.get())
    {
        //[UserSliderCode_m_fStepSeqSpeed] -- add your slider handling code here..
        //[/UserSliderCode_m_fStepSeqSpeed]
    }
    else if (sliderThatWasMoved == m_fGate.get())
    {
        //[UserSliderCode_m_fGate] -- add your slider handling code here..
        //[/UserSliderCode_m_fGate]
    }
    else if (sliderThatWasMoved == m_fGlide.get())
    {
        //[UserSliderCode_m_fGlide] -- add your slider handling code here..
        //[/UserSliderCode_m_fGlide]
    }
    else if (sliderThatWasMoved == c_numSteps.get())
    {
        //[UserSliderCode_c_numSteps] -- add your slider handling code here..
		//myData is data changed - must not change data live here!
		myData->stepSeqChangeSteps(c_numSteps.get()->getValue(), m_fGlide.get()->getValue(), m_fGate.get()->getValue());
		repaint();

        //[/UserSliderCode_c_numSteps]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void VASTStepSeqEditorPane::updateAll() {
	c_StepSeqInvert->setToggleState(myData->getInvert(), NotificationType::sendNotification);
}

void VASTStepSeqEditorPane::startAutoUpdate() {
	c_stepseqEditor->startAutoUpdate();
}
void VASTStepSeqEditorPane::stopAutoUpdate() {
	c_stepseqEditor->stopAutoUpdate();
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="VASTStepSeqEditorPane" componentName=""
                 parentClasses="public Component" constructorParams="AudioProcessorEditor *editor, AudioProcessor* processor, VASTMSEGData* data, VASTMSEGData* datalive, String parameterSuffix, int stepSeqNo"
                 variableInitialisers="myEditor((VASTAudioProcessorEditor*)editor), myProcessor((VASTAudioProcessor*)processor), myData(data), myDataLive(datalive), mySuffix(parameterSuffix), myStepSeqNo(stepSeqNo)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="800" initialHeight="287">
  <BACKGROUND backgroundColour="ff323e44">
    <RECT pos="0 0% 0M 0M" fill="linear: -16 -504, 0R 0R, 0=50bc6445, 1=ff141516"
          hasStroke="0"/>
    <RECT pos="0 0 0M 18.467%" fill="linear: 0 0, 0R 0R, 0=ff3f4446, 1=ff4c5457"
          hasStroke="0"/>
  </BACKGROUND>
  <GENERICCOMPONENT name="c_stepseqEditor" id="5eb93e0013882f07" memberName="c_stepseqEditor"
                    virtualName="" explicitFocusOrder="0" pos="0% 19.377% 111.111% 85.784%"
                    class="VASTStepSeqEditor" params="myProcessor, myData, myDataLive"/>
  <COMBOBOX name="c_loadEnv" id="75b6e174a60d379f" memberName="c_loadEnv"
            virtualName="VASTComboBox" explicitFocusOrder="0" pos="1.389% 8.471% 19.444% 5.161%"
            editable="0" layout="33" items="Default pattern&#10;Sidechain&#10;Stairs"
            textWhenNonSelected="load env" textWhenNoItems="none"/>
  <LABEL name="new label" id="b688b6b2463f3740" memberName="label3" virtualName=""
         explicitFocusOrder="0" pos="28.472% 8.763% 10.706% 3.992%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="STEPS" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Code Pro Demo"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="34"/>
  <LABEL name="new label" id="28184510b080f51e" memberName="label4" virtualName=""
         explicitFocusOrder="0" pos="38.31% 8.763% 10.706% 3.992%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="GLIDE" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Code Pro Demo"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="34"/>
  <LABEL name="new label" id="6025d63af0416cf7" memberName="label5" virtualName=""
         explicitFocusOrder="0" pos="48.206% 8.763% 10.706% 3.992%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="GATE" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Code Pro Demo"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="34"/>
  <LABEL name="new label" id="9050920a08f3ea65" memberName="label108"
         virtualName="" explicitFocusOrder="0" pos="92.94% 8.763% 10.706% 3.992%"
         textCol="ffe2e2e2" edTextCol="ff000000" edBkgCol="0" labelText="BEATS (DAW)"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default sans-serif font" fontsize="11.0" kerning="0.0"
         bold="0" italic="0" justification="34"/>
  <LABEL name="new label" id="852debe5e5f76751" memberName="label2" virtualName=""
         explicitFocusOrder="0" pos="79.977% 8.763% 10.706% 3.992%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="SYNC" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Code Pro Demo"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="34"/>
  <LABEL name="new label" id="8b1c744c8c6502ef" memberName="label7" virtualName=""
         explicitFocusOrder="0" pos="58.218% 8.763% 10.706% 3.992%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="SPEED" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Code Pro Demo"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="34"/>
  <COMBOBOX name="m_uStepSeqTimeBeats" id="e036b1cae3e29aa2" memberName="m_uStepSeqTimeBeats"
            virtualName="VASTParameterComboBox" explicitFocusOrder="0" pos="93.49% 8.056% 5% 4.907%"
            tooltip="Step sequencer time in beats when synched to DAW" editable="0"
            layout="33" items="UNIPOLAR&#10;BIPOLAR" textWhenNonSelected="choose env mode"
            textWhenNoItems="none"/>
  <TOGGLEBUTTON name="m_bStepSeqSynch" id="2a4a8c0b3fb3a16a" memberName="m_bStepSeqSynch"
                virtualName="VASTParameterButton" explicitFocusOrder="0" pos="90.683% 7.303% 2.488% 6.913%"
                tooltip="Synch LFO to DAW timecode (starts when played in DAW)"
                buttonText="" connectedEdges="0" needsCallback="1" radioGroupId="0"
                state="0"/>
  <SLIDER name="m_fStepSeqSpeed" id="711d91f72f6bf943" memberName="m_fStepSeqSpeed"
          virtualName="VASTParameterSlider" explicitFocusOrder="1" pos="68.056% 2.921% 5.556% 14.703%"
          tooltip="Step sequencer speed" textboxtext="ffc9c9c9" textboxbkgd="ffffff"
          textboxoutline="808080" min="0.0" max="100.0" int="0.01" style="RotaryVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="30"
          textBoxHeight="14" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="m_fGate" id="3b3f56fd4a7f6aa9" memberName="m_fGate" virtualName="VASTParameterSlider"
          explicitFocusOrder="1" pos="58.333% 2.921% 5.556% 14.703%" tooltip="Gate amount step sequencer "
          textboxtext="ffc9c9c9" textboxbkgd="ffffff" textboxoutline="808080"
          min="0.0" max="100.0" int="0.01" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="30" textBoxHeight="14" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="m_fGlide" id="d082b2f7dddc2836" memberName="m_fGlide" virtualName="VASTParameterSlider"
          explicitFocusOrder="0" pos="48.611% 2.921% 5.556% 14.703%" tooltip="Glide mode time step sequencer"
          textboxtext="ffc9c9c9" textboxbkgd="ffffff" textboxoutline="808080"
          min="0.0" max="100.0" int="0.01" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="30" textBoxHeight="14" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="c_numSteps" id="e40a662fd508c4e5" memberName="c_numSteps"
          virtualName="VASTSlider" explicitFocusOrder="0" pos="38.889% 2.921% 5.556% 14.703%"
          tooltip="Number of steps in step sequencer" textboxtext="ffc9c9c9"
          textboxbkgd="ffffff" textboxoutline="808080" min="1.0" max="64.0"
          int="1.0" style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="30" textBoxHeight="14" skewFactor="1.0" needsCallback="1"/>
  <LABEL name="new label" id="2ccc9143e2616ccd" memberName="label6" virtualName=""
         explicitFocusOrder="0" pos="18.056% 8.763% 10.706% 3.992%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="INVERT" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Code Pro Demo"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="34"/>
  <TOGGLEBUTTON name="c_StepSeqInvert" id="1d94b90683d2fb39" memberName="c_StepSeqInvert"
                virtualName="VASTButton" explicitFocusOrder="1" pos="28.877% 7.303% 2.488% 6.913%"
                tooltip="Invert step sequencer curve" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

