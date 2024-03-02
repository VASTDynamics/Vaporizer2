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
#include "VASTMSEGEditor.h"
#include "../VASTAudioProcessor.h"
#include "../VASTAudioProcessorEditor.h"
#include "../VASTControls/VASTSlider.h"
#include "../VASTControls/VASTButton.h"
#include "../VASTControls/VASTComboBox.h"
#include "../VASTControls/VASTParameterSlider.h"
#include "../VASTControls/VASTParameterButton.h"
#include "../VASTControls/VASTParameterComboBox.h"
#include "../../Engine/VASTMSEGEnvelope.h"
#include "../../Engine/VASTMSEGData.h"
//[/Headers]

#include "VASTMSEGEditorPane.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
VASTMSEGEditorPane::VASTMSEGEditorPane (AudioProcessorEditor *editor, AudioProcessor* processor, VASTMSEGData* data, VASTMSEGData* datalive, String parameterSuffix, int msegNo)
    : myEditor((VASTAudioProcessorEditor*)editor), myProcessor((VASTAudioProcessor*)processor), myData(data), myDataLive(datalive), mySuffix(parameterSuffix), myMSEGNo(msegNo)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    c_msegEditor.reset (new VASTMSEGEditor (myProcessor,
                                            myEditor,
                                            myData, myDataLive, myMSEGNo));
    addAndMakeVisible (c_msegEditor.get());
    c_msegEditor->setName ("c_msegEditor");

    c_loadEnv.reset (new VASTComboBox ("c_loadEnv"));
    addAndMakeVisible (c_loadEnv.get());
    c_loadEnv->setExplicitFocusOrder (1);
    c_loadEnv->setEditableText (false);
    c_loadEnv->setJustificationType (juce::Justification::centredLeft);
    c_loadEnv->setTextWhenNothingSelected (TRANS ("load env"));
    c_loadEnv->setTextWhenNoChoicesAvailable (TRANS ("none"));
    c_loadEnv->addItem (TRANS ("ADSR"), 1);
    c_loadEnv->addItem (TRANS ("ADR"), 2);
    c_loadEnv->addItem (TRANS ("AHDSR"), 3);
    c_loadEnv->addItem (TRANS ("Ramp"), 4);
    c_loadEnv->addItem (TRANS ("Sine"), 5);
    c_loadEnv->addItem (TRANS ("Stairs"), 6);
    c_loadEnv->addSeparator();
    c_loadEnv->addSeparator();
    c_loadEnv->addListener (this);

    label3.reset (new juce::Label ("new label",
                                   TRANS ("ATK")));
    addAndMakeVisible (label3.get());
    label3->setFont (juce::Font ("Code Pro Demo", 11.00f, juce::Font::plain));
    label3->setJustificationType (juce::Justification::centredRight);
    label3->setEditable (false, false, false);
    label3->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label3->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label3->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label4.reset (new juce::Label ("new label",
                                   TRANS ("DEC")));
    addAndMakeVisible (label4.get());
    label4->setFont (juce::Font ("Code Pro Demo", 11.00f, juce::Font::plain));
    label4->setJustificationType (juce::Justification::centredRight);
    label4->setEditable (false, false, false);
    label4->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label4->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label4->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label5.reset (new juce::Label ("new label",
                                   TRANS ("SUS")));
    addAndMakeVisible (label5.get());
    label5->setFont (juce::Font ("Code Pro Demo", 11.00f, juce::Font::plain));
    label5->setJustificationType (juce::Justification::centredRight);
    label5->setEditable (false, false, false);
    label5->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label5->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label5->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label6.reset (new juce::Label ("new label",
                                   TRANS ("REL")));
    addAndMakeVisible (label6.get());
    label6->setFont (juce::Font ("Code Pro Demo", 11.00f, juce::Font::plain));
    label6->setJustificationType (juce::Justification::centredRight);
    label6->setEditable (false, false, false);
    label6->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label6->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label6->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    m_uMSEGPolarity.reset (new VASTParameterComboBox ("m_uMSEGPolarity"));
    addAndMakeVisible (m_uMSEGPolarity.get());
    m_uMSEGPolarity->setTooltip (TRANS ("MSEG polarity (unipolar, bipolar) - used when MSEG is modulation source"));
    m_uMSEGPolarity->setEditableText (false);
    m_uMSEGPolarity->setJustificationType (juce::Justification::centredLeft);
    m_uMSEGPolarity->setTextWhenNothingSelected (TRANS ("choose env mode"));
    m_uMSEGPolarity->setTextWhenNoChoicesAvailable (TRANS ("none"));
    m_uMSEGPolarity->addItem (TRANS ("UNIPOLAR"), 1);
    m_uMSEGPolarity->addItem (TRANS ("BIPOLAR"), 2);
    m_uMSEGPolarity->addListener (this);

    m_fAttackTime.reset (new VASTParameterSlider ("m_fAttackTime"));
    addAndMakeVisible (m_fAttackTime.get());
    m_fAttackTime->setRange (0, 100, 0.01);
    m_fAttackTime->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    m_fAttackTime->setTextBoxStyle (juce::Slider::NoTextBox, false, 30, 14);
    m_fAttackTime->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffc9c9c9));
    m_fAttackTime->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0x00ffffff));
    m_fAttackTime->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x00808080));
    m_fAttackTime->addListener (this);

    m_fDecayTime.reset (new VASTParameterSlider ("m_fDecayTime"));
    addAndMakeVisible (m_fDecayTime.get());
    m_fDecayTime->setRange (0, 100, 0.01);
    m_fDecayTime->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    m_fDecayTime->setTextBoxStyle (juce::Slider::NoTextBox, false, 30, 14);
    m_fDecayTime->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffc9c9c9));
    m_fDecayTime->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0x00ffffff));
    m_fDecayTime->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x00808080));
    m_fDecayTime->addListener (this);

    m_fSustainLevel.reset (new VASTParameterSlider ("m_fSustainLevel"));
    addAndMakeVisible (m_fSustainLevel.get());
    m_fSustainLevel->setRange (0, 100, 0.01);
    m_fSustainLevel->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    m_fSustainLevel->setTextBoxStyle (juce::Slider::NoTextBox, false, 30, 14);
    m_fSustainLevel->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffc9c9c9));
    m_fSustainLevel->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0x00ffffff));
    m_fSustainLevel->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x00808080));
    m_fSustainLevel->addListener (this);

    m_fReleaseTime.reset (new VASTParameterSlider ("m_fReleaseTime"));
    addAndMakeVisible (m_fReleaseTime.get());
    m_fReleaseTime->setRange (0, 100, 0.01);
    m_fReleaseTime->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    m_fReleaseTime->setTextBoxStyle (juce::Slider::NoTextBox, false, 30, 14);
    m_fReleaseTime->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffc9c9c9));
    m_fReleaseTime->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0x00ffffff));
    m_fReleaseTime->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x00808080));
    m_fReleaseTime->addListener (this);

    label108.reset (new juce::Label ("new label",
                                     TRANS ("BEATS (DAW)")));
    addAndMakeVisible (label108.get());
    label108->setFont (juce::Font (11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
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

    m_uMSEGTimeBeats.reset (new VASTParameterComboBox ("m_uMSEGTimeBeats"));
    addAndMakeVisible (m_uMSEGTimeBeats.get());
    m_uMSEGTimeBeats->setTooltip (TRANS ("LFO time in beats when synched to DAW"));
    m_uMSEGTimeBeats->setEditableText (false);
    m_uMSEGTimeBeats->setJustificationType (juce::Justification::centredLeft);
    m_uMSEGTimeBeats->setTextWhenNothingSelected (TRANS ("choose env mode"));
    m_uMSEGTimeBeats->setTextWhenNoChoicesAvailable (TRANS ("none"));
    m_uMSEGTimeBeats->addItem (TRANS ("UNIPOLAR"), 1);
    m_uMSEGTimeBeats->addItem (TRANS ("BIPOLAR"), 2);
    m_uMSEGTimeBeats->addListener (this);

    m_bMSEGSynch.reset (new VASTParameterButton ("m_bMSEGSynch"));
    addAndMakeVisible (m_bMSEGSynch.get());
    m_bMSEGSynch->setTooltip (TRANS ("Synch MSEG to DAW timecode "));
    m_bMSEGSynch->setButtonText (juce::String());
    m_bMSEGSynch->addListener (this);

    label7.reset (new juce::Label ("new label",
                                   TRANS ("PER VOICE")));
    addAndMakeVisible (label7.get());
    label7->setFont (juce::Font ("Code Pro Demo", 11.00f, juce::Font::plain));
    label7->setJustificationType (juce::Justification::centredRight);
    label7->setEditable (false, false, false);
    label7->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label7->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label7->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    m_bMSEGPerVoice.reset (new VASTParameterButton ("m_bMSEGPerVoice"));
    addAndMakeVisible (m_bMSEGPerVoice.get());
    m_bMSEGPerVoice->setTooltip (TRANS ("Have a seperate MSEG per voice or global"));
    m_bMSEGPerVoice->setButtonText (juce::String());
    m_bMSEGPerVoice->addListener (this);

    m_fAttackSteps.reset (new VASTParameterSlider ("m_fAttackSteps"));
    addAndMakeVisible (m_fAttackSteps.get());
    m_fAttackSteps->setTooltip (TRANS ("MSEG Attack steps of beats"));
    m_fAttackSteps->setRange (0, 100, 0.01);
    m_fAttackSteps->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    m_fAttackSteps->setTextBoxStyle (juce::Slider::NoTextBox, false, 30, 14);
    m_fAttackSteps->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffc9c9c9));
    m_fAttackSteps->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0x00ffffff));
    m_fAttackSteps->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x00808080));
    m_fAttackSteps->addListener (this);

    m_fDecaySteps.reset (new VASTParameterSlider ("m_fDecaySteps"));
    addAndMakeVisible (m_fDecaySteps.get());
    m_fDecaySteps->setTooltip (TRANS ("MSEG Decay steps of beats"));
    m_fDecaySteps->setRange (0, 100, 0.01);
    m_fDecaySteps->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    m_fDecaySteps->setTextBoxStyle (juce::Slider::NoTextBox, false, 30, 14);
    m_fDecaySteps->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffc9c9c9));
    m_fDecaySteps->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0x00ffffff));
    m_fDecaySteps->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x00808080));
    m_fDecaySteps->addListener (this);

    m_fReleaseSteps.reset (new VASTParameterSlider ("m_fReleaseSteps"));
    addAndMakeVisible (m_fReleaseSteps.get());
    m_fReleaseSteps->setTooltip (TRANS ("MSEG Release steps of beats"));
    m_fReleaseSteps->setRange (0, 100, 0.01);
    m_fReleaseSteps->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    m_fReleaseSteps->setTextBoxStyle (juce::Slider::NoTextBox, false, 30, 14);
    m_fReleaseSteps->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffc9c9c9));
    m_fReleaseSteps->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0x00ffffff));
    m_fReleaseSteps->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x00808080));
    m_fReleaseSteps->addListener (this);


    //[UserPreSize]
	//manage parameter bindings -> set ComponentID = Name and processor
	Array<Component*> children = this->getChildren();
	for (auto child : children) {
		if (child->getName().startsWith("m_")) {
			auto* aSlider = dynamic_cast<VASTParameterSlider*> (child);
			if (aSlider != nullptr) {
				aSlider->setAudioProcessor(*myProcessor);
				aSlider->bindParameter(myEditor, aSlider->getName() + "_" + mySuffix, VASTGUIRuntimeModel::GUIComponents::MSEGEditorPane, myMSEGNo);
			}
			auto* aCombobox = dynamic_cast<VASTParameterComboBox*> (child);
			if (aCombobox != nullptr) {
				aCombobox->setAudioProcessor(*myProcessor);
				aCombobox->bindParameter(myEditor, aCombobox->getName() + "_" + mySuffix, VASTGUIRuntimeModel::GUIComponents::MSEGEditorPane, myMSEGNo);
			}
			auto* aButton = dynamic_cast<VASTParameterButton*> (child);
			if (aButton != nullptr) {
				aButton->setAudioProcessor(*myProcessor);
				aButton->bindParameter(myEditor, aButton->getName() + "_" + mySuffix, VASTGUIRuntimeModel::GUIComponents::MSEGEditorPane, myMSEGNo);
			}
		}
	}

    c_loadEnv->setSelectedItemIndex(0, NotificationType::dontSendNotification);//TODO
	setOpaque(true); //avoid repaints of parents

    //[/UserPreSize]

    setSize (800, 287);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

VASTMSEGEditorPane::~VASTMSEGEditorPane()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    c_msegEditor = nullptr;
    c_loadEnv = nullptr;
    label3 = nullptr;
    label4 = nullptr;
    label5 = nullptr;
    label6 = nullptr;
    m_uMSEGPolarity = nullptr;
    m_fAttackTime = nullptr;
    m_fDecayTime = nullptr;
    m_fSustainLevel = nullptr;
    m_fReleaseTime = nullptr;
    label108 = nullptr;
    label2 = nullptr;
    m_uMSEGTimeBeats = nullptr;
    m_bMSEGSynch = nullptr;
    label7 = nullptr;
    m_bMSEGPerVoice = nullptr;
    m_fAttackSteps = nullptr;
    m_fDecaySteps = nullptr;
    m_fReleaseSteps = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void VASTMSEGEditorPane::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff323e44));

    {
        int x = 0, y = proportionOfHeight (0.0000f), width = getWidth() - 0, height = getHeight() - 0;
        juce::Colour fillColour1 = juce::Colour (0x50bc6445), fillColour2 = juce::Colour (0xff141516);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setGradientFill (juce::ColourGradient (fillColour1,
                                             static_cast<float> (proportionOfWidth (-0.0200f)) - 0.0f + x,
                                             static_cast<float> (proportionOfHeight (-1.7561f)) - static_cast<float> (proportionOfHeight (0.0000f)) + y,
                                             fillColour2,
                                             static_cast<float> (proportionOfWidth (1.0000f)) - 0.0f + x,
                                             static_cast<float> (proportionOfHeight (1.0000f)) - static_cast<float> (proportionOfHeight (0.0000f)) + y,
                                             false));
        g.fillRect (x, y, width, height);
    }

    {
        int x = 0, y = 0, width = getWidth() - 0, height = proportionOfHeight (0.1847f);
        juce::Colour fillColour1 = juce::Colour (0xff3f4446), fillColour2 = juce::Colour (0xff4c5457);
        //[UserPaintCustomArguments] Customize the painting arguments here..
    		fillColour1 = myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colMSEGEditorPaneHeaderGradientFrom);
    		fillColour2 = myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colMSEGEditorPaneHeaderGradientTo);
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

void VASTMSEGEditorPane::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    c_msegEditor->setBounds (proportionOfWidth (0.0000f), proportionOfHeight (0.1847f), proportionOfWidth (1.0000f), proportionOfHeight (0.8153f));
    c_loadEnv->setBounds (proportionOfWidth (0.0125f), proportionOfHeight (0.0801f), proportionOfWidth (0.1075f), proportionOfHeight (0.0488f));
    label3->setBounds (proportionOfWidth (0.2087f), proportionOfHeight (0.0836f), proportionOfWidth (0.0963f), proportionOfHeight (0.0383f));
    label4->setBounds (proportionOfWidth (0.2963f), proportionOfHeight (0.0836f), proportionOfWidth (0.0963f), proportionOfHeight (0.0383f));
    label5->setBounds (proportionOfWidth (0.3825f), proportionOfHeight (0.0836f), proportionOfWidth (0.0963f), proportionOfHeight (0.0383f));
    label6->setBounds (proportionOfWidth (0.4688f), proportionOfHeight (0.0836f), proportionOfWidth (0.0963f), proportionOfHeight (0.0383f));
    m_uMSEGPolarity->setBounds (proportionOfWidth (0.1300f), proportionOfHeight (0.0801f), proportionOfWidth (0.0800f), proportionOfHeight (0.0488f));
    m_fAttackTime->setBounds (proportionOfWidth (0.3025f), proportionOfHeight (0.0279f), proportionOfWidth (0.0500f), proportionOfHeight (0.1394f));
    m_fDecayTime->setBounds (proportionOfWidth (0.3900f), proportionOfHeight (0.0279f), proportionOfWidth (0.0500f), proportionOfHeight (0.1394f));
    m_fSustainLevel->setBounds (proportionOfWidth (0.4775f), proportionOfHeight (0.0279f), proportionOfWidth (0.0500f), proportionOfHeight (0.1394f));
    m_fReleaseTime->setBounds (proportionOfWidth (0.5650f), proportionOfHeight (0.0279f), proportionOfWidth (0.0500f), proportionOfHeight (0.1394f));
    label108->setBounds (proportionOfWidth (0.8363f), proportionOfHeight (0.0836f), proportionOfWidth (0.0963f), proportionOfHeight (0.0383f));
    label2->setBounds (proportionOfWidth (0.7200f), proportionOfHeight (0.0836f), proportionOfWidth (0.0963f), proportionOfHeight (0.0383f));
    m_uMSEGTimeBeats->setBounds (proportionOfWidth (0.9350f), proportionOfHeight (0.0801f), proportionOfWidth (0.0500f), proportionOfHeight (0.0488f));
    m_bMSEGSynch->setBounds (proportionOfWidth (0.8163f), proportionOfHeight (0.0697f), proportionOfWidth (0.0225f), proportionOfHeight (0.0627f));
    label7->setBounds (proportionOfWidth (0.6425f), proportionOfHeight (0.0836f), proportionOfWidth (0.0963f), proportionOfHeight (0.0383f));
    m_bMSEGPerVoice->setBounds (proportionOfWidth (0.7438f), proportionOfHeight (0.0697f), proportionOfWidth (0.0213f), proportionOfHeight (0.0627f));
    m_fAttackSteps->setBounds (proportionOfWidth (0.3025f), proportionOfHeight (0.0279f), proportionOfWidth (0.0500f), proportionOfHeight (0.1394f));
    m_fDecaySteps->setBounds (proportionOfWidth (0.3900f), proportionOfHeight (0.0279f), proportionOfWidth (0.0500f), proportionOfHeight (0.1394f));
    m_fReleaseSteps->setBounds (proportionOfWidth (0.5650f), proportionOfHeight (0.0279f), proportionOfWidth (0.0500f), proportionOfHeight (0.1394f));
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void VASTMSEGEditorPane::comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == c_loadEnv.get())
    {
        //[UserComboBoxCode_c_loadEnv] -- add your combo box handling code here..
		if (c_loadEnv->getText().equalsIgnoreCase("ADR"))
			myData->initToADR(myData->m_msegNo);
		else if (c_loadEnv->getText().equalsIgnoreCase("ADSR"))
			myData->initToADSR(myData->m_msegNo);
		else if (c_loadEnv->getText().equalsIgnoreCase("AHDSR"))
			myData->initToAHDSR(myData->m_msegNo);
		else if (c_loadEnv->getText().equalsIgnoreCase("Ramp"))
			myData->initToRamp(myData->m_msegNo);
		else if (c_loadEnv->getText().equalsIgnoreCase("Sine"))
			myData->initToSine(myData->m_msegNo);
		else if (c_loadEnv->getText().equalsIgnoreCase("Stairs"))
			myData->initToStairs(myData->m_msegNo);

        //[/UserComboBoxCode_c_loadEnv]
    }
    else if (comboBoxThatHasChanged == m_uMSEGPolarity.get())
    {
        //[UserComboBoxCode_m_uMSEGPolarity] -- add your combo box handling code here..
		myData->setEnvMode(m_uMSEGPolarity->getSelectedItemIndex());
        //[/UserComboBoxCode_m_uMSEGPolarity]
    }
    else if (comboBoxThatHasChanged == m_uMSEGTimeBeats.get())
    {
        //[UserComboBoxCode_m_uMSEGTimeBeats] -- add your combo box handling code here..
        updateAll();
        //[/UserComboBoxCode_m_uMSEGTimeBeats]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}

void VASTMSEGEditorPane::sliderValueChanged (juce::Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == m_fAttackTime.get())
    {
        //[UserSliderCode_m_fAttackTime] -- add your slider handling code here..
        //[/UserSliderCode_m_fAttackTime]
    }
    else if (sliderThatWasMoved == m_fDecayTime.get())
    {
        //[UserSliderCode_m_fDecayTime] -- add your slider handling code here..
        //[/UserSliderCode_m_fDecayTime]
    }
    else if (sliderThatWasMoved == m_fSustainLevel.get())
    {
        //[UserSliderCode_m_fSustainLevel] -- add your slider handling code here..
        //[/UserSliderCode_m_fSustainLevel]
    }
    else if (sliderThatWasMoved == m_fReleaseTime.get())
    {
        //[UserSliderCode_m_fReleaseTime] -- add your slider handling code here..
        //[/UserSliderCode_m_fReleaseTime]
    }
    else if (sliderThatWasMoved == m_fAttackSteps.get())
    {
        //[UserSliderCode_m_fAttackSteps] -- add your slider handling code here..
        //[/UserSliderCode_m_fAttackSteps]
    }
    else if (sliderThatWasMoved == m_fDecaySteps.get())
    {
        //[UserSliderCode_m_fDecaySteps] -- add your slider handling code here..
        //[/UserSliderCode_m_fDecaySteps]
    }
    else if (sliderThatWasMoved == m_fReleaseSteps.get())
    {
        //[UserSliderCode_m_fReleaseSteps] -- add your slider handling code here..
        //[/UserSliderCode_m_fReleaseSteps]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void VASTMSEGEditorPane::buttonClicked (juce::Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == m_bMSEGSynch.get())
    {
        //[UserButtonCode_m_bMSEGSynch] -- add your button handler code here..
        synchToggled();
        updateAll();
        //[/UserButtonCode_m_bMSEGSynch]
    }
    else if (buttonThatWasClicked == m_bMSEGPerVoice.get())
    {
        //[UserButtonCode_m_bMSEGPerVoice] -- add your button handler code here..
        //[/UserButtonCode_m_bMSEGPerVoice]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void VASTMSEGEditorPane::synchToggled() {
    bool isSync = m_bMSEGSynch.get()->getToggleState();
    if (isSync) {
            float millisPerBeat = myProcessor->m_pVASTXperience.m_Set.getMillisecondsPerBeat();
            float intRatio = myProcessor->m_pVASTXperience.m_Set.getIntervalRatio(m_uMSEGTimeBeats->getSelectedItemIndex());
            double steps = roundToInt(m_fAttackTime->getValue() / (intRatio * millisPerBeat));
            if (m_fAttackSteps->getValue() == 0.0)
                m_fAttackSteps->setValue(steps, NotificationType::sendNotification);
            steps = roundToInt(m_fDecayTime->getValue() / (intRatio * millisPerBeat));
            if (m_fAttackSteps->getValue() == 0.0)
                m_fDecaySteps->setValue(steps, NotificationType::sendNotification);
            steps = roundToInt(m_fReleaseTime->getValue() / (intRatio * millisPerBeat));
            if (m_fReleaseSteps->getValue() == 0.0)
                m_fReleaseSteps->setValue(steps, NotificationType::sendNotification);
        }
    else {
        float millisPerBeat = myProcessor->m_pVASTXperience.m_Set.getMillisecondsPerBeat();
        float intRatio = myProcessor->m_pVASTXperience.m_Set.getIntervalRatio(m_uMSEGTimeBeats->getSelectedItemIndex());
        double time = m_fAttackSteps->getValue() * (intRatio * millisPerBeat);
        if (m_fAttackSteps->getValue() != 0.0)
            m_fAttackTime->setValue(time, NotificationType::sendNotification);
        time = m_fDecaySteps->getValue() * (intRatio * millisPerBeat);
        if (m_fDecaySteps->getValue() != 0.0)
            m_fDecayTime->setValue(time, NotificationType::sendNotification);
        time = m_fReleaseSteps->getValue() * (intRatio * millisPerBeat);
        if (m_fReleaseSteps->getValue() != 0.0)
            m_fReleaseTime->setValue(time, NotificationType::sendNotification);
    }
}

void VASTMSEGEditorPane::updateAll() {
	bool isSync = m_bMSEGSynch.get()->getToggleState();
	if (isSync) {
		m_fAttackTime->setVisible(false);
		m_fDecayTime->setVisible(false);
		m_fReleaseTime->setVisible(false);
		m_fAttackSteps->setVisible(true);
		m_fDecaySteps->setVisible(true);
		m_fReleaseSteps->setVisible(true);

	}
    else {
        m_fAttackTime->setVisible(true);
        m_fDecayTime->setVisible(true);
        m_fReleaseTime->setVisible(true);
        m_fAttackSteps->setVisible(false);
        m_fDecaySteps->setVisible(false);
        m_fReleaseSteps->setVisible(false);
    }
}

void VASTMSEGEditorPane::startAutoUpdate() {
	c_msegEditor->startAutoUpdate();
}
void VASTMSEGEditorPane::stopAutoUpdate() {
	c_msegEditor->stopAutoUpdate();
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="VASTMSEGEditorPane" componentName=""
                 parentClasses="public Component" constructorParams="AudioProcessorEditor *editor, AudioProcessor* processor, VASTMSEGData* data, VASTMSEGData* datalive, String parameterSuffix, int msegNo"
                 variableInitialisers="myEditor((VASTAudioProcessorEditor*)editor), myProcessor((VASTAudioProcessor*)processor), myData(data), myDataLive(datalive), mySuffix(parameterSuffix), myMSEGNo(msegNo)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="800" initialHeight="287">
  <BACKGROUND backgroundColour="ff323e44">
    <RECT pos="0 0% 0M 0M" fill="linear: -2% -175.61%, 100% 100%, 0=50bc6445, 1=ff141516"
          hasStroke="0"/>
    <RECT pos="0 0 0M 18.467%" fill="linear: 0 0, 0R 0R, 0=ff3f4446, 1=ff4c5457"
          hasStroke="0"/>
  </BACKGROUND>
  <GENERICCOMPONENT name="c_msegEditor" id="5eb93e0013882f07" memberName="c_msegEditor"
                    virtualName="" explicitFocusOrder="0" pos="0% 18.467% 100% 81.533%"
                    class="VASTMSEGEditor" params="myProcessor, &#10;myEditor,&#10;myData, myDataLive, myMSEGNo"/>
  <COMBOBOX name="c_loadEnv" id="75b6e174a60d379f" memberName="c_loadEnv"
            virtualName="VASTComboBox" explicitFocusOrder="1" pos="1.25% 8.014% 10.75% 4.878%"
            editable="0" layout="33" items="ADSR&#10;ADR&#10;AHDSR&#10;Ramp&#10;Sine&#10;Stairs&#10;&#10;"
            textWhenNonSelected="load env" textWhenNoItems="none"/>
  <LABEL name="new label" id="b688b6b2463f3740" memberName="label3" virtualName=""
         explicitFocusOrder="0" pos="20.875% 8.362% 9.625% 3.833%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="ATK" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Code Pro Demo"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="34"/>
  <LABEL name="new label" id="28184510b080f51e" memberName="label4" virtualName=""
         explicitFocusOrder="0" pos="29.625% 8.362% 9.625% 3.833%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="DEC" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Code Pro Demo"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="34"/>
  <LABEL name="new label" id="51ae5b51997b6d0d" memberName="label5" virtualName=""
         explicitFocusOrder="0" pos="38.25% 8.362% 9.625% 3.833%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="SUS" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Code Pro Demo"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="34"/>
  <LABEL name="new label" id="7406194615564dee" memberName="label6" virtualName=""
         explicitFocusOrder="0" pos="46.875% 8.362% 9.625% 3.833%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="REL" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Code Pro Demo"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="34"/>
  <COMBOBOX name="m_uMSEGPolarity" id="fbd393b799b5d766" memberName="m_uMSEGPolarity"
            virtualName="VASTParameterComboBox" explicitFocusOrder="0" pos="13% 8.014% 8% 4.878%"
            tooltip="MSEG polarity (unipolar, bipolar) - used when MSEG is modulation source"
            editable="0" layout="33" items="UNIPOLAR&#10;BIPOLAR" textWhenNonSelected="choose env mode"
            textWhenNoItems="none"/>
  <SLIDER name="m_fAttackTime" id="e40a662fd508c4e5" memberName="m_fAttackTime"
          virtualName="VASTParameterSlider" explicitFocusOrder="0" pos="30.25% 2.787% 5% 13.937%"
          textboxtext="ffc9c9c9" textboxbkgd="ffffff" textboxoutline="808080"
          min="0.0" max="100.0" int="0.01" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="30" textBoxHeight="14" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="m_fDecayTime" id="d082b2f7dddc2836" memberName="m_fDecayTime"
          virtualName="VASTParameterSlider" explicitFocusOrder="0" pos="39% 2.787% 5% 13.937%"
          textboxtext="ffc9c9c9" textboxbkgd="ffffff" textboxoutline="808080"
          min="0.0" max="100.0" int="0.01" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="30" textBoxHeight="14" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="m_fSustainLevel" id="d7912d24e5284aa5" memberName="m_fSustainLevel"
          virtualName="VASTParameterSlider" explicitFocusOrder="0" pos="47.75% 2.787% 5% 13.937%"
          textboxtext="ffc9c9c9" textboxbkgd="ffffff" textboxoutline="808080"
          min="0.0" max="100.0" int="0.01" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="30" textBoxHeight="14" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="m_fReleaseTime" id="d097a5eedfd901a1" memberName="m_fReleaseTime"
          virtualName="VASTParameterSlider" explicitFocusOrder="0" pos="56.5% 2.787% 5% 13.937%"
          textboxtext="ffc9c9c9" textboxbkgd="ffffff" textboxoutline="808080"
          min="0.0" max="100.0" int="0.01" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="30" textBoxHeight="14" skewFactor="1.0"
          needsCallback="1"/>
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
  <COMBOBOX name="m_uMSEGTimeBeats" id="e036b1cae3e29aa2" memberName="m_uMSEGTimeBeats"
            virtualName="VASTParameterComboBox" explicitFocusOrder="0" pos="93.5% 8.014% 5% 4.878%"
            tooltip="LFO time in beats when synched to DAW" editable="0"
            layout="33" items="UNIPOLAR&#10;BIPOLAR" textWhenNonSelected="choose env mode"
            textWhenNoItems="none"/>
  <TOGGLEBUTTON name="m_bMSEGSynch" id="2a4a8c0b3fb3a16a" memberName="m_bMSEGSynch"
                virtualName="VASTParameterButton" explicitFocusOrder="0" pos="81.625% 6.969% 2.25% 6.272%"
                tooltip="Synch MSEG to DAW timecode " buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <LABEL name="new label" id="35c64de5ec941734" memberName="label7" virtualName=""
         explicitFocusOrder="0" pos="64.25% 8.362% 9.625% 3.833%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="PER VOICE" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Code Pro Demo"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="34"/>
  <TOGGLEBUTTON name="m_bMSEGPerVoice" id="3b7cb0db2f4b078a" memberName="m_bMSEGPerVoice"
                virtualName="VASTParameterButton" explicitFocusOrder="0" pos="74.375% 6.969% 2.125% 6.272%"
                tooltip="Have a seperate MSEG per voice or global" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <SLIDER name="m_fAttackSteps" id="150d8c14d5957006" memberName="m_fAttackSteps"
          virtualName="VASTParameterSlider" explicitFocusOrder="0" pos="30.25% 2.787% 5% 13.937%"
          tooltip="MSEG Attack steps of beats" textboxtext="ffc9c9c9" textboxbkgd="ffffff"
          textboxoutline="808080" min="0.0" max="100.0" int="0.01" style="RotaryVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="30"
          textBoxHeight="14" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="m_fDecaySteps" id="f8fca8d7bca9c0d0" memberName="m_fDecaySteps"
          virtualName="VASTParameterSlider" explicitFocusOrder="0" pos="39% 2.787% 5% 13.937%"
          tooltip="MSEG Decay steps of beats" textboxtext="ffc9c9c9" textboxbkgd="ffffff"
          textboxoutline="808080" min="0.0" max="100.0" int="0.01" style="RotaryVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="30"
          textBoxHeight="14" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="m_fReleaseSteps" id="6b9182d800dd2fc9" memberName="m_fReleaseSteps"
          virtualName="VASTParameterSlider" explicitFocusOrder="0" pos="56.5% 2.787% 5% 13.937%"
          tooltip="MSEG Release steps of beats" textboxtext="ffc9c9c9"
          textboxbkgd="ffffff" textboxoutline="808080" min="0.0" max="100.0"
          int="0.01" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="30" textBoxHeight="14" skewFactor="1.0"
          needsCallback="1"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

