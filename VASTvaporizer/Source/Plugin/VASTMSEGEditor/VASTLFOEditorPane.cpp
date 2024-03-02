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
#include "VASTLFOEditor.h"
//[/Headers]

#include "VASTLFOEditorPane.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
VASTLFOEditorPane::VASTLFOEditorPane (AudioProcessorEditor *editor, AudioProcessor* processor, String parameterSuffix, int lfoNo)
    : myEditor((VASTAudioProcessorEditor*)editor), myProcessor((VASTAudioProcessor*)processor),  mySuffix(parameterSuffix), myLFONo(lfoNo)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    c_lfoEditor.reset (new VASTLFOEditor (myProcessor, mySuffix));
    addAndMakeVisible (c_lfoEditor.get());
    c_lfoEditor->setName ("c_lfoEditor");

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

    label3.reset (new juce::Label ("new label",
                                   TRANS ("FREQ")));
    addAndMakeVisible (label3.get());
    label3->setFont (juce::Font ("Code Pro Demo", 11.00f, juce::Font::plain));
    label3->setJustificationType (juce::Justification::centredRight);
    label3->setEditable (false, false, false);
    label3->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label3->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label3->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label4.reset (new juce::Label ("new label",
                                   TRANS ("RAMP")));
    addAndMakeVisible (label4.get());
    label4->setFont (juce::Font ("Code Pro Demo", 11.00f, juce::Font::plain));
    label4->setJustificationType (juce::Justification::centredRight);
    label4->setEditable (false, false, false);
    label4->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label4->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label4->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    m_uLFOWave.reset (new VASTParameterComboBox ("m_uLFOWave"));
    addAndMakeVisible (m_uLFOWave.get());
    m_uLFOWave->setTooltip (TRANS ("LFO wave form"));
    m_uLFOWave->setEditableText (false);
    m_uLFOWave->setJustificationType (juce::Justification::centredLeft);
    m_uLFOWave->setTextWhenNothingSelected (juce::String());
    m_uLFOWave->setTextWhenNoChoicesAvailable (TRANS ("---"));
    m_uLFOWave->addListener (this);

    label6.reset (new juce::Label ("new label",
                                   TRANS ("RETRIG")));
    addAndMakeVisible (label6.get());
    label6->setFont (juce::Font ("Code Pro Demo", 11.00f, juce::Font::plain));
    label6->setJustificationType (juce::Justification::centredRight);
    label6->setEditable (false, false, false);
    label6->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label6->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label6->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    m_uLFOTimeBeats.reset (new VASTParameterComboBox ("m_uLFOTimeBeats"));
    addAndMakeVisible (m_uLFOTimeBeats.get());
    m_uLFOTimeBeats->setTooltip (TRANS ("LFO time in beats when synched to DAW"));
    m_uLFOTimeBeats->setEditableText (false);
    m_uLFOTimeBeats->setJustificationType (juce::Justification::centredLeft);
    m_uLFOTimeBeats->setTextWhenNothingSelected (TRANS ("choose env mode"));
    m_uLFOTimeBeats->setTextWhenNoChoicesAvailable (TRANS ("none"));
    m_uLFOTimeBeats->addItem (TRANS ("UNIPOLAR"), 1);
    m_uLFOTimeBeats->addItem (TRANS ("BIPOLAR"), 2);
    m_uLFOTimeBeats->addListener (this);

    m_bLFOSynch.reset (new VASTParameterButton ("m_bLFOSynch"));
    addAndMakeVisible (m_bLFOSynch.get());
    m_bLFOSynch->setTooltip (TRANS ("Synch LFO to DAW timecode (starts when played in DAW)"));
    m_bLFOSynch->setButtonText (juce::String());
    m_bLFOSynch->addListener (this);

    m_bLFORetrigOnOff.reset (new VASTParameterButton ("m_bLFORetrigOnOff"));
    addAndMakeVisible (m_bLFORetrigOnOff.get());
    m_bLFORetrigOnOff->setTooltip (TRANS ("Retrigger LFO oscillator with every new note played (otherwise free running)"));
    m_bLFORetrigOnOff->setButtonText (juce::String());
    m_bLFORetrigOnOff->addListener (this);

    label5.reset (new juce::Label ("new label",
                                   TRANS ("PER VOICE")));
    addAndMakeVisible (label5.get());
    label5->setFont (juce::Font ("Code Pro Demo", 11.00f, juce::Font::plain));
    label5->setJustificationType (juce::Justification::centredRight);
    label5->setEditable (false, false, false);
    label5->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label5->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label5->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    m_bLFOPerVoice.reset (new VASTParameterButton ("m_bLFOPerVoice"));
    addAndMakeVisible (m_bLFOPerVoice.get());
    m_bLFOPerVoice->setTooltip (TRANS ("Have a seperate LFO per voice or global"));
    m_bLFOPerVoice->setButtonText (juce::String());
    m_bLFOPerVoice->addListener (this);

    m_uLFOPolarity.reset (new VASTParameterComboBox ("m_uLFOPolarity"));
    addAndMakeVisible (m_uLFOPolarity.get());
    m_uLFOPolarity->setTooltip (TRANS ("LFO polarity (unipolar, bipolar)"));
    m_uLFOPolarity->setEditableText (false);
    m_uLFOPolarity->setJustificationType (juce::Justification::centredLeft);
    m_uLFOPolarity->setTextWhenNothingSelected (TRANS ("choose env mode"));
    m_uLFOPolarity->setTextWhenNoChoicesAvailable (TRANS ("none"));
    m_uLFOPolarity->addItem (TRANS ("UNIPOLAR"), 1);
    m_uLFOPolarity->addItem (TRANS ("BIPOLAR"), 2);
    m_uLFOPolarity->addListener (this);

    label7.reset (new juce::Label ("new label",
                                   TRANS ("PHASE")));
    addAndMakeVisible (label7.get());
    label7->setFont (juce::Font ("Code Pro Demo", 11.00f, juce::Font::plain));
    label7->setJustificationType (juce::Justification::centredRight);
    label7->setEditable (false, false, false);
    label7->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label7->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label7->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    m_fLFORamp.reset (new VASTParameterSlider ("m_fLFORamp"));
    addAndMakeVisible (m_fLFORamp.get());
    m_fLFORamp->setTooltip (TRANS ("LFO ramp beats"));
    m_fLFORamp->setRange (0, 100, 0.01);
    m_fLFORamp->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    m_fLFORamp->setTextBoxStyle (juce::Slider::NoTextBox, false, 30, 14);
    m_fLFORamp->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffc9c9c9));
    m_fLFORamp->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0x00ffffff));
    m_fLFORamp->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x00808080));
    m_fLFORamp->addListener (this);

    m_fLFOPhase.reset (new VASTParameterSlider ("m_fLFOPhase"));
    addAndMakeVisible (m_fLFOPhase.get());
    m_fLFOPhase->setTooltip (TRANS ("LFO phase"));
    m_fLFOPhase->setRange (0, 100, 0.01);
    m_fLFOPhase->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    m_fLFOPhase->setTextBoxStyle (juce::Slider::NoTextBox, false, 30, 14);
    m_fLFOPhase->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffc9c9c9));
    m_fLFOPhase->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0x00ffffff));
    m_fLFOPhase->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x00808080));
    m_fLFOPhase->addListener (this);

    m_fLFOFreq.reset (new VASTParameterSlider ("m_fLFOFreq"));
    addAndMakeVisible (m_fLFOFreq.get());
    m_fLFOFreq->setTooltip (TRANS ("LFO frequency (overridden when synced to DAW)"));
    m_fLFOFreq->setRange (0, 50, 0.01);
    m_fLFOFreq->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    m_fLFOFreq->setTextBoxStyle (juce::Slider::NoTextBox, false, 30, 14);
    m_fLFOFreq->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffc9c9c9));
    m_fLFOFreq->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0x00ffffff));
    m_fLFOFreq->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x00808080));
    m_fLFOFreq->addListener (this);

    m_uLFOMSEG.reset (new VASTParameterComboBox ("m_uLFOMSEG"));
    addAndMakeVisible (m_uLFOMSEG.get());
    m_uLFOMSEG->setTooltip (TRANS ("LFO MSEG enevelope"));
    m_uLFOMSEG->setEditableText (false);
    m_uLFOMSEG->setJustificationType (juce::Justification::centredLeft);
    m_uLFOMSEG->setTextWhenNothingSelected (TRANS ("choose env mode"));
    m_uLFOMSEG->setTextWhenNoChoicesAvailable (TRANS ("none"));
    m_uLFOMSEG->addListener (this);


    //[UserPreSize]
	//manage parameter bindings -> set ComponentID = Name and processor
	Array<Component*> children = this->getChildren();
	for (auto child : children) {
		if (child->getName().startsWith("m_")) {
			auto* aSlider = dynamic_cast<VASTParameterSlider*> (child);
			if (aSlider != nullptr) {
				aSlider->setAudioProcessor(*myProcessor);
				aSlider->bindParameter(myEditor, aSlider->getName() + "_" + mySuffix, VASTGUIRuntimeModel::GUIComponents::LFOEditorPane, myLFONo);
			}
			auto* aCombobox = dynamic_cast<VASTParameterComboBox*> (child);
			if (aCombobox != nullptr) {
				aCombobox->setAudioProcessor(*myProcessor);
				aCombobox->bindParameter(myEditor, aCombobox->getName() + "_" + mySuffix, VASTGUIRuntimeModel::GUIComponents::LFOEditorPane, myLFONo);
			}
			auto* aButton = dynamic_cast<VASTParameterButton*> (child);
			if (aButton != nullptr) {
				aButton->setAudioProcessor(*myProcessor);
				aButton->bindParameter(myEditor, aButton->getName() + "_" + mySuffix, VASTGUIRuntimeModel::GUIComponents::LFOEditorPane, myLFONo);
			}
		}
	}

	setOpaque(true); //avoid repaints of parents

    //[/UserPreSize]

    setSize (800, 287);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

VASTLFOEditorPane::~VASTLFOEditorPane()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    c_lfoEditor = nullptr;
    label108 = nullptr;
    label2 = nullptr;
    label3 = nullptr;
    label4 = nullptr;
    m_uLFOWave = nullptr;
    label6 = nullptr;
    m_uLFOTimeBeats = nullptr;
    m_bLFOSynch = nullptr;
    m_bLFORetrigOnOff = nullptr;
    label5 = nullptr;
    m_bLFOPerVoice = nullptr;
    m_uLFOPolarity = nullptr;
    label7 = nullptr;
    m_fLFORamp = nullptr;
    m_fLFOPhase = nullptr;
    m_fLFOFreq = nullptr;
    m_uLFOMSEG = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void VASTLFOEditorPane::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff323e44));

    {
        int x = 0, y = proportionOfHeight (0.0000f), width = getWidth() - 0, height = getHeight() - 0;
        juce::Colour fillColour1 = juce::Colour (0x50bc6445), fillColour2 = juce::Colour (0xff141516);
        //[UserPaintCustomArguments] Customize the painting arguments here..
    		fillColour1 = myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colLFOEditorPaneBackgroundGradientFrom);
    		fillColour2 = myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colLFOEditorPaneBackgroundGradientTo);
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
    		fillColour1 = myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colLFOEditorPaneHeaderGradientFrom);
    		fillColour2 = myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colLFOEditorPaneHeaderGradientTo);
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

void VASTLFOEditorPane::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    c_lfoEditor->setBounds (proportionOfWidth (0.0000f), proportionOfHeight (0.1938f), proportionOfWidth (1.1111f), proportionOfHeight (0.8578f));
    label108->setBounds (proportionOfWidth (0.9294f), proportionOfHeight (0.0876f), proportionOfWidth (0.1071f), proportionOfHeight (0.0399f));
    label2->setBounds (proportionOfWidth (0.7998f), proportionOfHeight (0.0876f), proportionOfWidth (0.1071f), proportionOfHeight (0.0399f));
    label3->setBounds (proportionOfWidth (0.2836f), proportionOfHeight (0.0876f), proportionOfWidth (0.1071f), proportionOfHeight (0.0399f));
    label4->setBounds (proportionOfWidth (0.3877f), proportionOfHeight (0.0876f), proportionOfWidth (0.1071f), proportionOfHeight (0.0399f));
    m_uLFOWave->setBounds (proportionOfWidth (0.0139f), proportionOfHeight (0.0847f), proportionOfWidth (0.1192f), proportionOfHeight (0.0516f));
    label6->setBounds (proportionOfWidth (0.7193f), proportionOfHeight (0.0876f), proportionOfWidth (0.1071f), proportionOfHeight (0.0399f));
    m_uLFOTimeBeats->setBounds (proportionOfWidth (0.9349f), proportionOfHeight (0.0806f), proportionOfWidth (0.0500f), proportionOfHeight (0.0491f));
    m_bLFOSynch->setBounds (proportionOfWidth (0.9068f), proportionOfHeight (0.0730f), proportionOfWidth (0.0249f), proportionOfHeight (0.0662f));
    m_bLFORetrigOnOff->setBounds (proportionOfWidth (0.8264f), proportionOfHeight (0.0730f), proportionOfWidth (0.0249f), proportionOfHeight (0.0662f));
    label5->setBounds (proportionOfWidth (0.6238f), proportionOfHeight (0.0876f), proportionOfWidth (0.1071f), proportionOfHeight (0.0399f));
    m_bLFOPerVoice->setBounds (proportionOfWidth (0.7303f), proportionOfHeight (0.0730f), proportionOfWidth (0.0249f), proportionOfHeight (0.0662f));
    m_uLFOPolarity->setBounds (proportionOfWidth (0.2442f), proportionOfHeight (0.0847f), proportionOfWidth (0.0891f), proportionOfHeight (0.0516f));
    label7->setBounds (proportionOfWidth (0.4861f), proportionOfHeight (0.0876f), proportionOfWidth (0.1071f), proportionOfHeight (0.0399f));
    m_fLFORamp->setBounds (proportionOfWidth (0.4861f), proportionOfHeight (0.0292f), proportionOfWidth (0.0556f), proportionOfHeight (0.1470f));
    m_fLFOPhase->setBounds (proportionOfWidth (0.5833f), proportionOfHeight (0.0292f), proportionOfWidth (0.0556f), proportionOfHeight (0.1470f));
    m_fLFOFreq->setBounds (proportionOfWidth (0.3889f), proportionOfHeight (0.0292f), proportionOfWidth (0.0556f), proportionOfHeight (0.1470f));
    m_uLFOMSEG->setBounds (proportionOfWidth (0.1447f), proportionOfHeight (0.0847f), proportionOfWidth (0.0891f), proportionOfHeight (0.0516f));
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void VASTLFOEditorPane::comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == m_uLFOWave.get())
    {
        //[UserComboBoxCode_m_uLFOWave] -- add your combo box handling code here..
        //[/UserComboBoxCode_m_uLFOWave]
    }
    else if (comboBoxThatHasChanged == m_uLFOTimeBeats.get())
    {
        //[UserComboBoxCode_m_uLFOTimeBeats] -- add your combo box handling code here..
        //[/UserComboBoxCode_m_uLFOTimeBeats]
    }
    else if (comboBoxThatHasChanged == m_uLFOPolarity.get())
    {
        //[UserComboBoxCode_m_uLFOPolarity] -- add your combo box handling code here..
		//data->
        //[/UserComboBoxCode_m_uLFOPolarity]
    }
    else if (comboBoxThatHasChanged == m_uLFOMSEG.get())
    {
        //[UserComboBoxCode_m_uLFOMSEG] -- add your combo box handling code here..
        //[/UserComboBoxCode_m_uLFOMSEG]
    }

    //[UsercomboBoxChanged_Post]
	c_lfoEditor->repaint();
    //[/UsercomboBoxChanged_Post]
}

void VASTLFOEditorPane::buttonClicked (juce::Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == m_bLFOSynch.get())
    {
        //[UserButtonCode_m_bLFOSynch] -- add your button handler code here..
        //[/UserButtonCode_m_bLFOSynch]
    }
    else if (buttonThatWasClicked == m_bLFORetrigOnOff.get())
    {
        //[UserButtonCode_m_bLFORetrigOnOff] -- add your button handler code here..
        //[/UserButtonCode_m_bLFORetrigOnOff]
    }
    else if (buttonThatWasClicked == m_bLFOPerVoice.get())
    {
        //[UserButtonCode_m_bLFOPerVoice] -- add your button handler code here..
        //[/UserButtonCode_m_bLFOPerVoice]
    }

    //[UserbuttonClicked_Post]
	c_lfoEditor->repaint();
    //[/UserbuttonClicked_Post]
}

void VASTLFOEditorPane::sliderValueChanged (juce::Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == m_fLFORamp.get())
    {
        //[UserSliderCode_m_fLFORamp] -- add your slider handling code here..
        //[/UserSliderCode_m_fLFORamp]
    }
    else if (sliderThatWasMoved == m_fLFOPhase.get())
    {
        //[UserSliderCode_m_fLFOPhase] -- add your slider handling code here..
        //[/UserSliderCode_m_fLFOPhase]
    }
    else if (sliderThatWasMoved == m_fLFOFreq.get())
    {
        //[UserSliderCode_m_fLFOFreq] -- add your slider handling code here..
        //[/UserSliderCode_m_fLFOFreq]
    }

    //[UsersliderValueChanged_Post]
	c_lfoEditor->repaint();
    //[/UsersliderValueChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void VASTLFOEditorPane::startAutoUpdate() {
	c_lfoEditor->startAutoUpdate();
}
void VASTLFOEditorPane::stopAutoUpdate() {
	c_lfoEditor->stopAutoUpdate();
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="VASTLFOEditorPane" componentName=""
                 parentClasses="public Component" constructorParams="AudioProcessorEditor *editor, AudioProcessor* processor, String parameterSuffix, int lfoNo"
                 variableInitialisers="myEditor((VASTAudioProcessorEditor*)editor), myProcessor((VASTAudioProcessor*)processor),  mySuffix(parameterSuffix), myLFONo(lfoNo)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="800" initialHeight="287">
  <BACKGROUND backgroundColour="ff323e44">
    <RECT pos="0 0% 0M 0M" fill="linear: -2% -175.61%, 100% 100%, 0=50bc6445, 1=ff141516"
          hasStroke="0"/>
    <RECT pos="0 0 0M 18.467%" fill="linear: 0 0, 0R 0R, 0=ff3f4446, 1=ff4c5457"
          hasStroke="0"/>
  </BACKGROUND>
  <GENERICCOMPONENT name="c_lfoEditor" id="5eb93e0013882f07" memberName="c_lfoEditor"
                    virtualName="" explicitFocusOrder="0" pos="0% 20.351% 123.438% 90.166%"
                    class="VASTLFOEditor" params="myProcessor, mySuffix"/>
  <LABEL name="new label" id="9050920a08f3ea65" memberName="label108"
         virtualName="" explicitFocusOrder="0" pos="92.917% 8.796% 10.729% 3.981%"
         textCol="ffe2e2e2" edTextCol="ff000000" edBkgCol="0" labelText="BEATS (DAW)"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="11.0" kerning="0.0" bold="0"
         italic="0" justification="34"/>
  <LABEL name="new label" id="852debe5e5f76751" memberName="label2" virtualName=""
         explicitFocusOrder="0" pos="88.889% 9.25% 11.921% 4.187%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="SYNC" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Code Pro Demo"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="34"/>
  <LABEL name="new label" id="b688b6b2463f3740" memberName="label3" virtualName=""
         explicitFocusOrder="0" pos="31.481% 9.25% 11.921% 4.187%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="FREQ" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Code Pro Demo"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="34"/>
  <LABEL name="new label" id="28184510b080f51e" memberName="label4" virtualName=""
         explicitFocusOrder="0" pos="43.056% 9.25% 11.921% 4.187%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="RAMP" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Code Pro Demo"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="34"/>
  <COMBOBOX name="m_uLFOWave" id="c27f0b9212832e6d" memberName="m_uLFOWave"
            virtualName="VASTParameterComboBox" explicitFocusOrder="0" pos="1.562% 8.861% 13.252% 5.453%"
            tooltip="LFO wave form" editable="0" layout="33" items="" textWhenNonSelected=""
            textWhenNoItems="---"/>
  <LABEL name="new label" id="934694a991b7eb85" memberName="label6" virtualName=""
         explicitFocusOrder="0" pos="79.919% 9.25% 11.921% 4.187%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="RETRIG" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Code Pro Demo"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="34"/>
  <COMBOBOX name="m_uLFOTimeBeats" id="e036b1cae3e29aa2" memberName="m_uLFOTimeBeats"
            virtualName="VASTParameterComboBox" explicitFocusOrder="0" pos="93.49% 8.056% 5% 4.907%"
            tooltip="LFO time in beats when synched to DAW" editable="0"
            layout="33" items="UNIPOLAR&#10;BIPOLAR" textWhenNonSelected="choose env mode"
            textWhenNoItems="none"/>
  <TOGGLEBUTTON name="m_bLFOSynch" id="2a4a8c0b3fb3a16a" memberName="m_bLFOSynch"
                virtualName="VASTParameterButton" explicitFocusOrder="0" pos="90.677% 7.315% 2.5% 6.667%"
                tooltip="Synch LFO to DAW timecode (starts when played in DAW)"
                buttonText="" connectedEdges="0" needsCallback="1" radioGroupId="0"
                state="0"/>
  <TOGGLEBUTTON name="m_bLFORetrigOnOff" id="750b55253d7aa8cb" memberName="m_bLFORetrigOnOff"
                virtualName="VASTParameterButton" explicitFocusOrder="0" pos="91.84% 7.692% 2.778% 7.011%"
                tooltip="Retrigger LFO oscillator with every new note played (otherwise free running)"
                buttonText="" connectedEdges="0" needsCallback="1" radioGroupId="0"
                state="0"/>
  <LABEL name="new label" id="35c64de5ec941734" memberName="label5" virtualName=""
         explicitFocusOrder="0" pos="69.329% 9.25% 11.921% 4.187%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="PER VOICE" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Code Pro Demo"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="34"/>
  <TOGGLEBUTTON name="m_bLFOPerVoice" id="3b7cb0db2f4b078a" memberName="m_bLFOPerVoice"
                virtualName="VASTParameterButton" explicitFocusOrder="0" pos="81.134% 7.692% 2.778% 7.011%"
                tooltip="Have a seperate LFO per voice or global" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <COMBOBOX name="m_uLFOPolarity" id="fbd393b799b5d766" memberName="m_uLFOPolarity"
            virtualName="VASTParameterComboBox" explicitFocusOrder="0" pos="27.141% 8.861% 9.896% 5.453%"
            tooltip="LFO polarity (unipolar, bipolar)" editable="0" layout="33"
            items="UNIPOLAR&#10;BIPOLAR" textWhenNonSelected="choose env mode"
            textWhenNoItems="none"/>
  <LABEL name="new label" id="ce460ed8854a1e9e" memberName="label7" virtualName=""
         explicitFocusOrder="0" pos="53.993% 9.25% 11.921% 4.187%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="PHASE" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Code Pro Demo"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="34"/>
  <SLIDER name="m_fLFORamp" id="d082b2f7dddc2836" memberName="m_fLFORamp"
          virtualName="VASTParameterSlider" explicitFocusOrder="0" pos="53.993% 3.116% 6.192% 15.482%"
          tooltip="LFO ramp beats" textboxtext="ffc9c9c9" textboxbkgd="ffffff"
          textboxoutline="808080" min="0.0" max="100.0" int="0.01" style="RotaryVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="30"
          textBoxHeight="14" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="m_fLFOPhase" id="c8204fb330320747" memberName="m_fLFOPhase"
          virtualName="VASTParameterSlider" explicitFocusOrder="0" pos="64.815% 3.116% 6.192% 15.482%"
          tooltip="LFO phase" textboxtext="ffc9c9c9" textboxbkgd="ffffff"
          textboxoutline="808080" min="0.0" max="100.0" int="0.01" style="RotaryVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="30"
          textBoxHeight="14" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="m_fLFOFreq" id="e40a662fd508c4e5" memberName="m_fLFOFreq"
          virtualName="VASTParameterSlider" explicitFocusOrder="0" pos="43.229% 3.116% 6.192% 15.482%"
          tooltip="LFO frequency (overridden when synced to DAW)" textboxtext="ffc9c9c9"
          textboxbkgd="ffffff" textboxoutline="808080" min="0.0" max="50.0"
          int="0.01" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="30" textBoxHeight="14" skewFactor="1.0"
          needsCallback="1"/>
  <COMBOBOX name="m_uLFOMSEG" id="7d6abdefc0a502f8" memberName="m_uLFOMSEG"
            virtualName="VASTParameterComboBox" explicitFocusOrder="0" pos="16.088% 8.861% 9.896% 5.453%"
            tooltip="LFO MSEG enevelope" editable="0" layout="33" items=""
            textWhenNonSelected="choose env mode" textWhenNoItems="none"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

