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
#include "VASTControls/VASTComboBrev.h"
#include "VASTControls/VASTComboBox.h"
#include "VASTControls/VASTImageButton.h"
#include "VASTControls/VASTParameterComboBox.h"
#include "VASTControls/VASTParameterSlider.h"
#include "VASTControls/VASTParameterButton.h"
#include "../Engine/VASTXperience.h"
#include "../Engine/VASTSingleNote.h"
//[/Headers]

#include "VASTOscillatorComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
VASTOscillatorComponent::VASTOscillatorComponent (AudioProcessorEditor *editor, AudioProcessor* processor)
    : myEditor((VASTAudioProcessorEditor*)editor), myProcessor((VASTAudioProcessor*)processor)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    c_audioOscilloscope.reset (new VASTOscilloscope());
    addAndMakeVisible (c_audioOscilloscope.get());
    c_audioOscilloscope->setExplicitFocusOrder (1);
    c_audioOscilloscope->setName ("c_audioOscilloscope");

    m_fOscDetune.reset (new VASTParameterSlider ("m_fOscDetune"));
    addAndMakeVisible (m_fOscDetune.get());
    m_fOscDetune->setTooltip (TRANS ("Normalized detune with bank oscillators"));
    m_fOscDetune->setRange (0, 100, 0.001);
    m_fOscDetune->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    m_fOscDetune->setTextBoxStyle (juce::Slider::NoTextBox, false, 30, 14);
    m_fOscDetune->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffc9c9c9));
    m_fOscDetune->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0x00ffffff));
    m_fOscDetune->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x00808080));
    m_fOscDetune->addListener (this);

    m_fOscGain.reset (new VASTParameterSlider ("m_fOscGain"));
    addAndMakeVisible (m_fOscGain.get());
    m_fOscGain->setTooltip (TRANS ("Bank gain (100 full, >100 overdrive)"));
    m_fOscGain->setRange (0, 200, 0.01);
    m_fOscGain->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    m_fOscGain->setTextBoxStyle (juce::Slider::NoTextBox, false, 30, 14);
    m_fOscGain->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffc9c9c9));
    m_fOscGain->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0x00ffffff));
    m_fOscGain->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x00808080));
    m_fOscGain->addListener (this);

    m_fOscCents.reset (new VASTParameterSlider ("m_fOscCents"));
    addAndMakeVisible (m_fOscCents.get());
    m_fOscCents->setTooltip (TRANS ("Bank cents (+/- 1 octave and half tones)"));
    m_fOscCents->setRange (-1200, 1200, 0.01);
    m_fOscCents->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    m_fOscCents->setTextBoxStyle (juce::Slider::NoTextBox, false, 30, 14);
    m_fOscCents->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffc9c9c9));
    m_fOscCents->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0x00ffffff));
    m_fOscCents->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x00808080));
    m_fOscCents->addListener (this);

    m_fOscWTPos.reset (new VASTParameterSlider ("m_fOscWTPos"));
    addAndMakeVisible (m_fOscWTPos.get());
    m_fOscWTPos->setTooltip (TRANS ("Oscillator wavetable position"));
    m_fOscWTPos->setRange (0, 100, 0.01);
    m_fOscWTPos->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    m_fOscWTPos->setTextBoxStyle (juce::Slider::NoTextBox, false, 30, 14);
    m_fOscWTPos->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffc9c9c9));
    m_fOscWTPos->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0x00ffffff));
    m_fOscWTPos->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x00808080));
    m_fOscWTPos->addListener (this);

    m_fOscMorph.reset (new VASTParameterSlider ("m_fOscMorph"));
    addAndMakeVisible (m_fOscMorph.get());
    m_fOscMorph->setTooltip (TRANS ("Oscillator morphing"));
    m_fOscMorph->setRange (0, 100, 0.01);
    m_fOscMorph->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    m_fOscMorph->setTextBoxStyle (juce::Slider::NoTextBox, false, 30, 14);
    m_fOscMorph->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffc9c9c9));
    m_fOscMorph->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0x00ffffff));
    m_fOscMorph->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x00808080));
    m_fOscMorph->addListener (this);

    m_fSpread.reset (new VASTParameterSlider ("m_fSpread"));
    addAndMakeVisible (m_fSpread.get());
    m_fSpread->setTooltip (TRANS ("Stereo spread within bank oscillators"));
    m_fSpread->setRange (0, 100, 0.01);
    m_fSpread->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    m_fSpread->setTextBoxStyle (juce::Slider::NoTextBox, false, 30, 14);
    m_fSpread->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffc9c9c9));
    m_fSpread->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0x00ffffff));
    m_fSpread->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x00808080));
    m_fSpread->addListener (this);

    headerLabel.reset (new juce::Label ("new label",
                                        TRANS ("OSC A")));
    addAndMakeVisible (headerLabel.get());
    headerLabel->setFont (juce::Font ("Syntax", 13.00f, juce::Font::plain).withTypefaceStyle ("Bold"));
    headerLabel->setJustificationType (juce::Justification::centredLeft);
    headerLabel->setEditable (false, false, false);
    headerLabel->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    headerLabel->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    headerLabel->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label18.reset (new juce::Label ("new label",
                                    TRANS ("WTPOS")));
    addAndMakeVisible (label18.get());
    label18->setFont (juce::Font ("Syntax", 11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label18->setJustificationType (juce::Justification::centred);
    label18->setEditable (false, false, false);
    label18->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label18->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label18->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label108.reset (new juce::Label ("new label",
                                     TRANS ("SPREAD")));
    addAndMakeVisible (label108.get());
    label108->setFont (juce::Font ("Syntax", 11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label108->setJustificationType (juce::Justification::centred);
    label108->setEditable (false, false, false);
    label108->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label108->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label108->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label134.reset (new juce::Label ("new label",
                                     TRANS ("MORPH")));
    addAndMakeVisible (label134.get());
    label134->setFont (juce::Font ("Syntax", 11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label134->setJustificationType (juce::Justification::centred);
    label134->setEditable (false, false, false);
    label134->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label134->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label134->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label135.reset (new juce::Label ("new label",
                                     TRANS ("DETUNE")));
    addAndMakeVisible (label135.get());
    label135->setFont (juce::Font ("Syntax", 11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label135->setJustificationType (juce::Justification::centred);
    label135->setEditable (false, false, false);
    label135->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label135->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label135->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label2.reset (new juce::Label ("new label",
                                   TRANS ("GAIN")));
    addAndMakeVisible (label2.get());
    label2->setFont (juce::Font ("Syntax", 11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label2->setJustificationType (juce::Justification::centred);
    label2->setEditable (false, false, false);
    label2->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label2->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label2->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label3.reset (new juce::Label ("new label",
                                   TRANS ("CENTS")));
    addAndMakeVisible (label3.get());
    label3->setFont (juce::Font ("Syntax", 11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label3->setJustificationType (juce::Justification::centred);
    label3->setEditable (false, false, false);
    label3->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label3->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label3->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    m_fOscPan.reset (new VASTParameterSlider ("m_fOscPan"));
    addAndMakeVisible (m_fOscPan.get());
    m_fOscPan->setTooltip (TRANS ("Stereo panning of oscillator"));
    m_fOscPan->setRange (-100, 100, 0.01);
    m_fOscPan->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    m_fOscPan->setTextBoxStyle (juce::Slider::NoTextBox, false, 30, 14);
    m_fOscPan->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffc9c9c9));
    m_fOscPan->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0x00ffffff));
    m_fOscPan->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x00808080));
    m_fOscPan->addListener (this);

    m_fOscPhase.reset (new VASTParameterSlider ("m_fOscPhase"));
    addAndMakeVisible (m_fOscPhase.get());
    m_fOscPhase->setTooltip (TRANS ("Phase of oscillator"));
    m_fOscPhase->setRange (0, 100, 0.01);
    m_fOscPhase->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    m_fOscPhase->setTextBoxStyle (juce::Slider::NoTextBox, false, 30, 14);
    m_fOscPhase->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffc9c9c9));
    m_fOscPhase->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0x00ffffff));
    m_fOscPhase->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x00808080));
    m_fOscPhase->addListener (this);

    label4.reset (new juce::Label ("new label",
                                   TRANS ("PAN")));
    addAndMakeVisible (label4.get());
    label4->setFont (juce::Font ("Syntax", 11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label4->setJustificationType (juce::Justification::centred);
    label4->setEditable (false, false, false);
    label4->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label4->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label4->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label5.reset (new juce::Label ("new label",
                                   TRANS ("PHASE")));
    addAndMakeVisible (label5.get());
    label5->setFont (juce::Font ("Syntax", 11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label5->setJustificationType (juce::Justification::centred);
    label5->setEditable (false, false, false);
    label5->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label5->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label5->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label6.reset (new juce::Label ("new label",
                                   TRANS ("OCT")));
    addAndMakeVisible (label6.get());
    label6->setFont (juce::Font ("Syntax", 11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label6->setJustificationType (juce::Justification::centredRight);
    label6->setEditable (false, false, false);
    label6->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label6->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label6->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label7.reset (new juce::Label ("new label",
                                   TRANS ("UNISON")));
    addAndMakeVisible (label7.get());
    label7->setFont (juce::Font ("Syntax", 11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label7->setJustificationType (juce::Justification::centredRight);
    label7->setEditable (false, false, false);
    label7->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label7->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label7->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label8.reset (new juce::Label ("new label",
                                   TRANS ("EXCITER")));
    addAndMakeVisible (label8.get());
    label8->setFont (juce::Font ("Syntax", 11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label8->setJustificationType (juce::Justification::centredLeft);
    label8->setEditable (false, false, false);
    label8->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label8->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label8->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label9.reset (new juce::Label ("new label",
                                   TRANS ("RETRIG")));
    addAndMakeVisible (label9.get());
    label9->setFont (juce::Font ("Syntax", 11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label9->setJustificationType (juce::Justification::centredLeft);
    label9->setEditable (false, false, false);
    label9->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label9->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label9->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label13.reset (new juce::Label ("new label",
                                    TRANS ("ENV")));
    addAndMakeVisible (label13.get());
    label13->setFont (juce::Font ("Syntax", 11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label13->setJustificationType (juce::Justification::centredRight);
    label13->setEditable (false, false, false);
    label13->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label13->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label13->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label14.reset (new juce::Label ("new label",
                                    TRANS ("ROUTING")));
    addAndMakeVisible (label14.get());
    label14->setFont (juce::Font ("Syntax", 11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label14->setJustificationType (juce::Justification::centredRight);
    label14->setEditable (false, false, false);
    label14->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label14->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label14->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label15.reset (new juce::Label ("new label",
                                    TRANS ("2")));
    addAndMakeVisible (label15.get());
    label15->setFont (juce::Font ("Syntax", 11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label15->setJustificationType (juce::Justification::centredRight);
    label15->setEditable (false, false, false);
    label15->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label15->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label15->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label12.reset (new juce::Label ("new label",
                                    TRANS ("INVERT")));
    addAndMakeVisible (label12.get());
    label12->setFont (juce::Font ("Syntax", 11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label12->setJustificationType (juce::Justification::centredLeft);
    label12->setEditable (false, false, false);
    label12->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label12->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label12->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    m_uOscRouting2.reset (new VASTParameterComboBox ("m_uOscRouting2"));
    addAndMakeVisible (m_uOscRouting2.get());
    m_uOscRouting2->setTooltip (TRANS ("Oscillator routing 2"));
    m_uOscRouting2->setEditableText (false);
    m_uOscRouting2->setJustificationType (juce::Justification::centredLeft);
    m_uOscRouting2->setTextWhenNothingSelected (juce::String());
    m_uOscRouting2->setTextWhenNoChoicesAvailable (TRANS ("---"));
    m_uOscRouting2->addItem (TRANS ("MSEG1"), 1);
    m_uOscRouting2->addItem (TRANS ("MSEG2"), 2);
    m_uOscRouting2->addItem (TRANS ("MSEG3"), 3);
    m_uOscRouting2->addItem (TRANS ("MSEG4"), 4);
    m_uOscRouting2->addItem (TRANS ("MSEG5"), 5);
    m_uOscRouting2->addListener (this);

    m_uOscRouting1.reset (new VASTParameterComboBox ("m_uOscRouting1"));
    addAndMakeVisible (m_uOscRouting1.get());
    m_uOscRouting1->setTooltip (TRANS ("Oscillator routing 1"));
    m_uOscRouting1->setEditableText (false);
    m_uOscRouting1->setJustificationType (juce::Justification::centredLeft);
    m_uOscRouting1->setTextWhenNothingSelected (juce::String());
    m_uOscRouting1->setTextWhenNoChoicesAvailable (TRANS ("---"));
    m_uOscRouting1->addItem (TRANS ("MSEG1"), 1);
    m_uOscRouting1->addItem (TRANS ("MSEG2"), 2);
    m_uOscRouting1->addItem (TRANS ("MSEG3"), 3);
    m_uOscRouting1->addItem (TRANS ("MSEG4"), 4);
    m_uOscRouting1->addItem (TRANS ("MSEG5"), 5);
    m_uOscRouting1->addListener (this);

    m_iOscOct.reset (new VASTParameterSlider ("m_iOscOct"));
    addAndMakeVisible (m_iOscOct.get());
    m_iOscOct->setTooltip (TRANS ("Bank oscillators octave"));
    m_iOscOct->setRange (-2, 2, 1);
    m_iOscOct->setSliderStyle (juce::Slider::IncDecButtons);
    m_iOscOct->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 26, 20);
    m_iOscOct->setColour (juce::Slider::backgroundColourId, juce::Colour (0x00000000));
    m_iOscOct->setColour (juce::Slider::thumbColourId, juce::Colours::black);
    m_iOscOct->setColour (juce::Slider::trackColourId, juce::Colours::black);
    m_iOscOct->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffc9c9c9));
    m_iOscOct->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0x00ffffff));
    m_iOscOct->setColour (juce::Slider::textBoxHighlightColourId, juce::Colours::green);
    m_iOscOct->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0xff8e989b));
    m_iOscOct->addListener (this);

    m_iNumOscs.reset (new VASTParameterSlider ("m_iNumOscs"));
    addAndMakeVisible (m_iNumOscs.get());
    m_iNumOscs->setTooltip (TRANS ("Number of bank oscillators"));
    m_iNumOscs->setRange (1, 24, 1);
    m_iNumOscs->setSliderStyle (juce::Slider::IncDecButtons);
    m_iNumOscs->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 30, 20);
    m_iNumOscs->setColour (juce::Slider::backgroundColourId, juce::Colour (0x00000000));
    m_iNumOscs->setColour (juce::Slider::thumbColourId, juce::Colours::black);
    m_iNumOscs->setColour (juce::Slider::trackColourId, juce::Colours::black);
    m_iNumOscs->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffc9c9c9));
    m_iNumOscs->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0x00ffffff));
    m_iNumOscs->setColour (juce::Slider::textBoxHighlightColourId, juce::Colours::green);
    m_iNumOscs->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0xff8e989b));
    m_iNumOscs->addListener (this);

    m_bOscOnOff.reset (new VASTParameterButton ("m_bOscOnOff"));
    addAndMakeVisible (m_bOscOnOff.get());
    m_bOscOnOff->setTooltip (TRANS ("Switch oscillator bank on / off"));
    m_bOscOnOff->setButtonText (juce::String());
    m_bOscOnOff->addListener (this);

    m_uVCAEnv.reset (new VASTParameterComboBox ("m_uVCAEnv"));
    addAndMakeVisible (m_uVCAEnv.get());
    m_uVCAEnv->setTooltip (TRANS ("Envelope used as VCA envelope"));
    m_uVCAEnv->setEditableText (false);
    m_uVCAEnv->setJustificationType (juce::Justification::centredLeft);
    m_uVCAEnv->setTextWhenNothingSelected (juce::String());
    m_uVCAEnv->setTextWhenNoChoicesAvailable (TRANS ("---"));
    m_uVCAEnv->addItem (TRANS ("MSEG1"), 1);
    m_uVCAEnv->addItem (TRANS ("MSEG2"), 2);
    m_uVCAEnv->addItem (TRANS ("MSEG3"), 3);
    m_uVCAEnv->addItem (TRANS ("MSEG4"), 4);
    m_uVCAEnv->addItem (TRANS ("MSEG5"), 5);
    m_uVCAEnv->addListener (this);

    m_bOscInverterOnOff.reset (new VASTParameterButton ("m_bOscInverterOnOff"));
    addAndMakeVisible (m_bOscInverterOnOff.get());
    m_bOscInverterOnOff->setTooltip (TRANS ("Invert all voices not in the center (might cancel sound when not detuned)"));
    m_bOscInverterOnOff->setButtonText (juce::String());
    m_bOscInverterOnOff->addListener (this);

    m_bExciterOnOff.reset (new VASTParameterButton ("m_bExciterOnOff"));
    addAndMakeVisible (m_bExciterOnOff.get());
    m_bExciterOnOff->setTooltip (TRANS ("Exciter adds random detune by may +- 0.25hz per oscillator and played note"));
    m_bExciterOnOff->setButtonText (juce::String());
    m_bExciterOnOff->addListener (this);

    m_bOscRetrigOnOff.reset (new VASTParameterButton ("m_bOscRetrigOnOff"));
    addAndMakeVisible (m_bOscRetrigOnOff.get());
    m_bOscRetrigOnOff->setTooltip (TRANS ("Random phase offeset for each oscillator for each new note or sync at beginning of phase"));
    m_bOscRetrigOnOff->setButtonText (juce::String());
    m_bOscRetrigOnOff->addListener (this);

    label10.reset (new juce::Label ("new label",
                                    TRANS ("WTFX")));
    addAndMakeVisible (label10.get());
    label10->setFont (juce::Font ("Syntax", 11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label10->setJustificationType (juce::Justification::centredRight);
    label10->setEditable (false, false, false);
    label10->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label10->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label10->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    m_uWTFX.reset (new VASTParameterComboBox ("m_uWTFX"));
    addAndMakeVisible (m_uWTFX.get());
    m_uWTFX->setTooltip (TRANS ("Wavetable Effect"));
    m_uWTFX->setEditableText (false);
    m_uWTFX->setJustificationType (juce::Justification::centredLeft);
    m_uWTFX->setTextWhenNothingSelected (juce::String());
    m_uWTFX->setTextWhenNoChoicesAvailable (TRANS ("---"));
    m_uWTFX->addListener (this);

    m_fWTFXVal.reset (new VASTParameterSlider ("m_fWTFXVal"));
    addAndMakeVisible (m_fWTFXVal.get());
    m_fWTFXVal->setTooltip (TRANS ("Wavetable effect parameter value"));
    m_fWTFXVal->setRange (0, 100, 0.01);
    m_fWTFXVal->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    m_fWTFXVal->setTextBoxStyle (juce::Slider::NoTextBox, false, 30, 14);
    m_fWTFXVal->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffc9c9c9));
    m_fWTFXVal->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0x00ffffff));
    m_fWTFXVal->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x00808080));
    m_fWTFXVal->addListener (this);


    //[UserPreSize]
	Array<Component*> children = this->getChildren();
	for (auto child : children) {
		if (child->getName().startsWith("m_")) {
			auto* aSlider = dynamic_cast<VASTParameterSlider*> (child);
			if (aSlider != nullptr)
				aSlider->setAudioProcessor(*myProcessor);
			auto* aCombobox = dynamic_cast<VASTParameterComboBox*> (child);
			if (aCombobox != nullptr)
				aCombobox->setAudioProcessor(*myProcessor);
			auto* aButton = dynamic_cast<VASTParameterButton*> (child);
			if (aButton != nullptr)
				aButton->setAudioProcessor(*myProcessor);
		}
	}

	c_audioOscilloscope->setProcessor(myProcessor);
	c_audioOscilloscope->setEditor(myEditor);
	c_audioOscilloscope->startAutoUpdate(false); //not the wteditor
	c_audioOscilloscope->setOpaque(true);

	m_iNumOscs->setIncDecButtonsMode(Slider::incDecButtonsDraggable_Vertical);
	m_iNumOscs->setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxLeft, false, 30, 20);
	m_iNumOscs->setTextBoxIsEditable(true);
	m_iOscOct->setIncDecButtonsMode(Slider::incDecButtonsDraggable_Vertical);
	m_iOscOct->setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxLeft, false, 26, 20);
	m_iOscOct->setTextBoxIsEditable(true);

	setOpaque(true);

    return; //dont call setSize
    //[/UserPreSize]

    setSize (500, 130);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

VASTOscillatorComponent::~VASTOscillatorComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
	this->setLookAndFeel(nullptr);
    //[/Destructor_pre]

    c_audioOscilloscope = nullptr;
    m_fOscDetune = nullptr;
    m_fOscGain = nullptr;
    m_fOscCents = nullptr;
    m_fOscWTPos = nullptr;
    m_fOscMorph = nullptr;
    m_fSpread = nullptr;
    headerLabel = nullptr;
    label18 = nullptr;
    label108 = nullptr;
    label134 = nullptr;
    label135 = nullptr;
    label2 = nullptr;
    label3 = nullptr;
    m_fOscPan = nullptr;
    m_fOscPhase = nullptr;
    label4 = nullptr;
    label5 = nullptr;
    label6 = nullptr;
    label7 = nullptr;
    label8 = nullptr;
    label9 = nullptr;
    label13 = nullptr;
    label14 = nullptr;
    label15 = nullptr;
    label12 = nullptr;
    m_uOscRouting2 = nullptr;
    m_uOscRouting1 = nullptr;
    m_iOscOct = nullptr;
    m_iNumOscs = nullptr;
    m_bOscOnOff = nullptr;
    m_uVCAEnv = nullptr;
    m_bOscInverterOnOff = nullptr;
    m_bExciterOnOff = nullptr;
    m_bOscRetrigOnOff = nullptr;
    label10 = nullptr;
    m_uWTFX = nullptr;
    m_fWTFXVal = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void VASTOscillatorComponent::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    if (myEditor->getCurrentVASTLookAndFeel()->m_isUsingBackgroundImages) {
        return;
    }
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff323e44));

    {
        int x = 0, y = 0, width = getWidth() - 0, height = getHeight() - 0;
        juce::Colour fillColour1 = juce::Colour (0xff56646c), fillColour2 = juce::Colour (0xff232527);
        //[UserPaintCustomArguments] Customize the painting arguments here..
    	fillColour1 = myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscillatorComponentBackgroundGradientFrom);
    	fillColour2 = myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscillatorComponentBackgroundGradientTo);
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

    {
        int x = 1, y = 1, width = getWidth() - 2, height = proportionOfHeight (0.1231f);
        juce::Colour fillColour1 = juce::Colour (0xff3f4446), fillColour2 = juce::Colour (0xff4c5457);
        //[UserPaintCustomArguments] Customize the painting arguments here..
    		fillColour1 = myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscillatorComponentHeaderGradientFrom);
    		fillColour2 = myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscillatorComponentHeaderGradientTo);
        //[/UserPaintCustomArguments]
        g.setGradientFill (juce::ColourGradient (fillColour1,
                                             0.0f - 1.0f + x,
                                             0.0f - 1.0f + y,
                                             fillColour2,
                                             static_cast<float> (getWidth()) - 1.0f + x,
                                             static_cast<float> (getHeight()) - 1.0f + y,
                                             false));
        g.fillRect (x, y, width, height);
    }

    {
        int x = proportionOfWidth (0.0080f), y = proportionOfHeight (0.4231f), width = proportionOfWidth (0.3280f), height = proportionOfHeight (0.0846f);
        juce::Colour fillColour1 = juce::Colour (0xff3a4145), fillColour2 = juce::Colour (0xff33393c);
        //[UserPaintCustomArguments] Customize the painting arguments here..
    		fillColour1 = myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscillatorComponentHeaderGradientFrom);
    		fillColour2 = myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscillatorComponentHeaderGradientTo);
        //[/UserPaintCustomArguments]
        g.setGradientFill (juce::ColourGradient (fillColour1,
                                             static_cast<float> (proportionOfWidth (0.0160f)) - static_cast<float> (proportionOfWidth (0.0080f)) + x,
                                             static_cast<float> (proportionOfHeight (0.3692f)) - static_cast<float> (proportionOfHeight (0.4231f)) + y,
                                             fillColour2,
                                             static_cast<float> (proportionOfWidth (0.1760f)) - static_cast<float> (proportionOfWidth (0.0080f)) + x,
                                             static_cast<float> (proportionOfHeight (0.5538f)) - static_cast<float> (proportionOfHeight (0.4231f)) + y,
                                             false));
        g.fillRect (x, y, width, height);
    }

    {
        int x = proportionOfWidth (0.0080f), y = proportionOfHeight (0.8538f), width = proportionOfWidth (0.3920f), height = proportionOfHeight (0.0846f);
        juce::Colour fillColour1 = juce::Colour (0xff3a4145), fillColour2 = juce::Colour (0xff33393c);
        //[UserPaintCustomArguments] Customize the painting arguments here..
    		fillColour1 = myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscillatorComponentHeaderGradientFrom);
    		fillColour2 = myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscillatorComponentHeaderGradientTo);
        //[/UserPaintCustomArguments]
        g.setGradientFill (juce::ColourGradient (fillColour1,
                                             static_cast<float> (proportionOfWidth (0.0000f)) - static_cast<float> (proportionOfWidth (0.0080f)) + x,
                                             static_cast<float> (proportionOfHeight (0.8615f)) - static_cast<float> (proportionOfHeight (0.8538f)) + y,
                                             fillColour2,
                                             static_cast<float> (proportionOfWidth (0.1760f)) - static_cast<float> (proportionOfWidth (0.0080f)) + x,
                                             static_cast<float> (proportionOfHeight (0.9846f)) - static_cast<float> (proportionOfHeight (0.8538f)) + y,
                                             false));
        g.fillRect (x, y, width, height);
    }

    //[UserPaint] Add your own custom painting code here..
	String header = "";
	if (this->getComponentID() == "OscA") header = "OSC A";
	else if (this->getComponentID() == "OscB") header = "OSC B";
	else if (this->getComponentID() == "OscC") header = "OSC C";
	else if (this->getComponentID() == "OscD") header = "OSC D";
	headerLabel->setText(header, juce::NotificationType::dontSendNotification);
    //[/UserPaint]
}

void VASTOscillatorComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    c_audioOscilloscope->setBounds (getWidth() - 1 - proportionOfWidth (0.4700f), getHeight() - 1 - proportionOfHeight (0.8538f), proportionOfWidth (0.4700f), proportionOfHeight (0.8538f));
    m_fOscDetune->setBounds (proportionOfWidth (0.0960f), proportionOfHeight (0.1231f), proportionOfWidth (0.0800f), proportionOfHeight (0.3077f));
    m_fOscGain->setBounds (proportionOfWidth (0.1760f), proportionOfHeight (0.1231f), proportionOfWidth (0.0800f), proportionOfHeight (0.3077f));
    m_fOscCents->setBounds (proportionOfWidth (0.1760f), proportionOfHeight (0.5538f), proportionOfWidth (0.0800f), proportionOfHeight (0.3077f));
    m_fOscWTPos->setBounds (proportionOfWidth (0.0160f), proportionOfHeight (0.5538f), proportionOfWidth (0.0800f), proportionOfHeight (0.3077f));
    m_fOscMorph->setBounds (proportionOfWidth (0.0960f), proportionOfHeight (0.5538f), proportionOfWidth (0.0800f), proportionOfHeight (0.3077f));
    m_fSpread->setBounds (proportionOfWidth (0.0160f), proportionOfHeight (0.1231f), proportionOfWidth (0.0800f), proportionOfHeight (0.3077f));
    headerLabel->setBounds (proportionOfWidth (0.0320f), proportionOfHeight (0.0077f), proportionOfWidth (0.1440f), proportionOfHeight (0.1231f));
    label18->setBounds (proportionOfWidth (-0.0180f), proportionOfHeight (0.8538f), proportionOfWidth (0.1280f), proportionOfHeight (0.0923f));
    label108->setBounds (proportionOfWidth (-0.0160f), proportionOfHeight (0.4231f), proportionOfWidth (0.1280f), proportionOfHeight (0.0923f));
    label134->setBounds (proportionOfWidth (0.0700f), proportionOfHeight (0.8538f), proportionOfWidth (0.1280f), proportionOfHeight (0.0923f));
    label135->setBounds (proportionOfWidth (0.0760f), proportionOfHeight (0.4231f), proportionOfWidth (0.1280f), proportionOfHeight (0.0923f));
    label2->setBounds (proportionOfWidth (0.1540f), proportionOfHeight (0.4231f), proportionOfWidth (0.1280f), proportionOfHeight (0.0923f));
    label3->setBounds (proportionOfWidth (0.1560f), proportionOfHeight (0.8538f), proportionOfWidth (0.1260f), proportionOfHeight (0.0923f));
    m_fOscPan->setBounds (proportionOfWidth (0.2560f), proportionOfHeight (0.1231f), proportionOfWidth (0.0800f), proportionOfHeight (0.3077f));
    m_fOscPhase->setBounds (proportionOfWidth (0.2560f), proportionOfHeight (0.5538f), proportionOfWidth (0.0800f), proportionOfHeight (0.3077f));
    label4->setBounds (proportionOfWidth (0.2320f), proportionOfHeight (0.4231f), proportionOfWidth (0.1280f), proportionOfHeight (0.0923f));
    label5->setBounds (proportionOfWidth (0.2320f), proportionOfHeight (0.8538f), proportionOfWidth (0.1280f), proportionOfHeight (0.0923f));
    label6->setBounds (proportionOfWidth (0.2740f), proportionOfHeight (0.0231f), proportionOfWidth (0.1280f), proportionOfHeight (0.0923f));
    label7->setBounds (proportionOfWidth (0.0940f), proportionOfHeight (0.0231f), proportionOfWidth (0.1280f), proportionOfHeight (0.0923f));
    label8->setBounds (proportionOfWidth (0.3920f), proportionOfHeight (0.2923f), proportionOfWidth (0.1300f), proportionOfHeight (0.0923f));
    label9->setBounds (proportionOfWidth (0.3920f), proportionOfHeight (0.4154f), proportionOfWidth (0.1280f), proportionOfHeight (0.0923f));
    label13->setBounds (proportionOfWidth (0.3840f), proportionOfHeight (-0.3077f), proportionOfWidth (0.1280f), proportionOfHeight (0.0923f));
    label14->setBounds (proportionOfWidth (0.6320f), proportionOfHeight (0.0231f), proportionOfWidth (0.1280f), proportionOfHeight (0.0923f));
    label15->setBounds (proportionOfWidth (0.7640f), proportionOfHeight (0.0231f), proportionOfWidth (0.1280f), proportionOfHeight (0.0923f));
    label12->setBounds (proportionOfWidth (0.3920f), proportionOfHeight (0.1692f), proportionOfWidth (0.1280f), proportionOfHeight (0.0923f));
    m_uOscRouting2->setBounds (proportionOfWidth (0.8900f), proportionOfHeight (0.0154f), proportionOfWidth (0.1020f), proportionOfHeight (0.1077f));
    m_uOscRouting1->setBounds (proportionOfWidth (0.7560f), proportionOfHeight (0.0154f), proportionOfWidth (0.1020f), proportionOfHeight (0.1077f));
    m_iOscOct->setBounds (proportionOfWidth (0.3980f), proportionOfHeight (0.0154f), proportionOfWidth (0.1200f), proportionOfHeight (0.1077f));
    m_iNumOscs->setBounds (proportionOfWidth (0.2180f), proportionOfHeight (0.0154f), proportionOfWidth (0.1200f), proportionOfHeight (0.1077f));
    m_bOscOnOff->setBounds (proportionOfWidth (0.0000f), proportionOfHeight (0.0000f), proportionOfWidth (0.0360f), proportionOfHeight (0.1385f));
    m_uVCAEnv->setBounds (proportionOfWidth (0.5280f), proportionOfHeight (0.0154f), proportionOfWidth (0.1200f), proportionOfHeight (0.1077f));
    m_bOscInverterOnOff->setBounds (proportionOfWidth (0.3600f), proportionOfHeight (0.1462f), proportionOfWidth (0.0360f), proportionOfHeight (0.1385f));
    m_bExciterOnOff->setBounds (proportionOfWidth (0.3600f), proportionOfHeight (0.2692f), proportionOfWidth (0.0360f), proportionOfHeight (0.1385f));
    m_bOscRetrigOnOff->setBounds (proportionOfWidth (0.3600f), proportionOfHeight (0.3923f), proportionOfWidth (0.0360f), proportionOfHeight (0.1385f));
    label10->setBounds (proportionOfWidth (0.2760f), proportionOfHeight (0.8538f), proportionOfWidth (0.1280f), proportionOfHeight (0.0923f));
    m_uWTFX->setBounds (proportionOfWidth (0.3980f), proportionOfHeight (0.8461f), proportionOfWidth (0.1200f), proportionOfHeight (0.1077f));
    m_fWTFXVal->setBounds (proportionOfWidth (0.4140f), proportionOfHeight (0.5538f), proportionOfWidth (0.0800f), proportionOfHeight (0.3077f));
    //[UserResized] Add your own custom resize handling here..
	m_iNumOscs->setTextBoxStyle(Slider::TextBoxLeft, true, m_iNumOscs->getWidth() * 0.45f, m_iNumOscs->getHeight());
	m_iOscOct->setTextBoxStyle(Slider::TextBoxLeft, true, m_iOscOct->getWidth() * 0.45f, m_iOscOct->getHeight());
    //[/UserResized]
}

void VASTOscillatorComponent::sliderValueChanged (juce::Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == m_fOscDetune.get())
    {
        //[UserSliderCode_m_fOscDetune] -- add your slider handling code here..
        //[/UserSliderCode_m_fOscDetune]
    }
    else if (sliderThatWasMoved == m_fOscGain.get())
    {
        //[UserSliderCode_m_fOscGain] -- add your slider handling code here..
        //[/UserSliderCode_m_fOscGain]
    }
    else if (sliderThatWasMoved == m_fOscCents.get())
    {
        //[UserSliderCode_m_fOscCents] -- add your slider handling code here..
        //[/UserSliderCode_m_fOscCents]
    }
    else if (sliderThatWasMoved == m_fOscWTPos.get())
    {
        //[UserSliderCode_m_fOscWTPos] -- add your slider handling code here..
        //[/UserSliderCode_m_fOscWTPos]
    }
    else if (sliderThatWasMoved == m_fOscMorph.get())
    {
        //[UserSliderCode_m_fOscMorph] -- add your slider handling code here..
        //[/UserSliderCode_m_fOscMorph]
    }
    else if (sliderThatWasMoved == m_fSpread.get())
    {
        //[UserSliderCode_m_fSpread] -- add your slider handling code here..
        //[/UserSliderCode_m_fSpread]
    }
    else if (sliderThatWasMoved == m_fOscPan.get())
    {
        //[UserSliderCode_m_fOscPan] -- add your slider handling code here..
        //[/UserSliderCode_m_fOscPan]
    }
    else if (sliderThatWasMoved == m_fOscPhase.get())
    {
        //[UserSliderCode_m_fOscPhase] -- add your slider handling code here..
        //[/UserSliderCode_m_fOscPhase]
    }
    else if (sliderThatWasMoved == m_iOscOct.get())
    {
        //[UserSliderCode_m_iOscOct] -- add your slider handling code here..
        //[/UserSliderCode_m_iOscOct]
    }
    else if (sliderThatWasMoved == m_iNumOscs.get())
    {
        //[UserSliderCode_m_iNumOscs] -- add your slider handling code here..
        //[/UserSliderCode_m_iNumOscs]
    }
    else if (sliderThatWasMoved == m_fWTFXVal.get())
    {
        //[UserSliderCode_m_fWTFXVal] -- add your slider handling code here..
        //[/UserSliderCode_m_fWTFXVal]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void VASTOscillatorComponent::comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == m_uOscRouting2.get())
    {
        //[UserComboBoxCode_m_uOscRouting2] -- add your combo box handling code here..
        //[/UserComboBoxCode_m_uOscRouting2]
    }
    else if (comboBoxThatHasChanged == m_uOscRouting1.get())
    {
        //[UserComboBoxCode_m_uOscRouting1] -- add your combo box handling code here..
        //[/UserComboBoxCode_m_uOscRouting1]
    }
    else if (comboBoxThatHasChanged == m_uVCAEnv.get())
    {
        //[UserComboBoxCode_m_uVCAEnv] -- add your combo box handling code here..
        //[/UserComboBoxCode_m_uVCAEnv]
    }
    else if (comboBoxThatHasChanged == m_uWTFX.get())
    {
        //[UserComboBoxCode_m_uWTFX] -- add your combo box handling code here..
        //[/UserComboBoxCode_m_uWTFX]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}

void VASTOscillatorComponent::buttonClicked (juce::Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == m_bOscOnOff.get())
    {
        //[UserButtonCode_m_bOscOnOff] -- add your button handler code here..
        //[/UserButtonCode_m_bOscOnOff]
    }
    else if (buttonThatWasClicked == m_bOscInverterOnOff.get())
    {
        //[UserButtonCode_m_bOscInverterOnOff] -- add your button handler code here..
        //[/UserButtonCode_m_bOscInverterOnOff]
    }
    else if (buttonThatWasClicked == m_bExciterOnOff.get())
    {
        //[UserButtonCode_m_bExciterOnOff] -- add your button handler code here..
        //[/UserButtonCode_m_bExciterOnOff]
    }
    else if (buttonThatWasClicked == m_bOscRetrigOnOff.get())
    {
        //[UserButtonCode_m_bOscRetrigOnOff] -- add your button handler code here..
        //[/UserButtonCode_m_bOscRetrigOnOff]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void VASTOscillatorComponent::assignComponentIDsForChildren(const String &compID) {
	this->setComponentID(compID);
	juce::Array<Component*> _children = this->getChildren();
	for (int i = 0; i < _children.size(); i++) {
		auto* aSlider = dynamic_cast<VASTParameterSlider*> (_children[i]);
		if (aSlider != nullptr)
			aSlider->bindParameter(myEditor, aSlider->getName() + "_" + compID, VASTGUIRuntimeModel::GUIComponents::OscillatorComponent, 0);
		auto* aCombobox = dynamic_cast<VASTParameterComboBox*> (_children[i]);
		if (aCombobox != nullptr)
			aCombobox->bindParameter(myEditor, aCombobox->getName() + "_" + compID, VASTGUIRuntimeModel::GUIComponents::OscillatorComponent, 0);
		auto* aButton = dynamic_cast<VASTParameterButton*> (_children[i]);
		if (aButton != nullptr)
			aButton->bindParameter(myEditor, aButton->getName() + "_" + compID, VASTGUIRuntimeModel::GUIComponents::OscillatorComponent, 0);
	}
}

void VASTOscillatorComponent::updateAll() {
	c_audioOscilloscope->updateContentAsync();
}

void VASTOscillatorComponent::initAll() {
	c_audioOscilloscope->updateContent(true);
}

void VASTOscillatorComponent::lookAndFeelChanged() {
    if (myEditor->getCurrentVASTLookAndFeel()->m_isUsingBackgroundImages) {
        setOpaque(false);
    }
    else {
        setOpaque(true);
    }
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="VASTOscillatorComponent"
                 componentName="" parentClasses="public Component" constructorParams="AudioProcessorEditor *editor, AudioProcessor* processor"
                 variableInitialisers="myEditor((VASTAudioProcessorEditor*)editor), myProcessor((VASTAudioProcessor*)processor)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="500" initialHeight="130">
  <BACKGROUND backgroundColour="ff323e44">
    <RECT pos="0 0 0M 0M" fill="linear: 0 0, 0R 0R, 0=ff56646c, 1=ff232527"
          hasStroke="0"/>
    <RECT pos="1 1 2M 12.308%" fill="linear: 0 0, 0R 0R, 0=ff3f4446, 1=ff4c5457"
          hasStroke="0"/>
    <RECT pos="0.8% 42.308% 32.8% 8.462%" fill="linear: 1.6% 36.923%, 17.6% 55.385%, 0=ff3a4145, 1=ff33393c"
          hasStroke="0"/>
    <RECT pos="0.8% 85.385% 39.2% 8.462%" fill="linear: 0% 86.154%, 17.6% 98.462%, 0=ff3a4145, 1=ff33393c"
          hasStroke="0"/>
  </BACKGROUND>
  <GENERICCOMPONENT name="c_audioOscilloscope" id="61bd7bec450e402b" memberName="c_audioOscilloscope"
                    virtualName="" explicitFocusOrder="1" pos="1Rr 1Rr 47% 85.385%"
                    class="VASTOscilloscope" params=""/>
  <SLIDER name="m_fOscDetune" id="47a723282bb529a9" memberName="m_fOscDetune"
          virtualName="VASTParameterSlider" explicitFocusOrder="0" pos="9.6% 12.308% 8% 30.769%"
          tooltip="Normalized detune with bank oscillators" textboxtext="ffc9c9c9"
          textboxbkgd="ffffff" textboxoutline="808080" min="0.0" max="100.0"
          int="0.001" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="30" textBoxHeight="14" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="m_fOscGain" id="ec183e546629b544" memberName="m_fOscGain"
          virtualName="VASTParameterSlider" explicitFocusOrder="0" pos="17.6% 12.308% 8% 30.769%"
          tooltip="Bank gain (100 full, &gt;100 overdrive)" textboxtext="ffc9c9c9"
          textboxbkgd="ffffff" textboxoutline="808080" min="0.0" max="200.0"
          int="0.01" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="30" textBoxHeight="14" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="m_fOscCents" id="3eaf6fbef9ce511c" memberName="m_fOscCents"
          virtualName="VASTParameterSlider" explicitFocusOrder="0" pos="17.6% 55.385% 8% 30.769%"
          tooltip="Bank cents (+/- 1 octave and half tones)" textboxtext="ffc9c9c9"
          textboxbkgd="ffffff" textboxoutline="808080" min="-1200.0" max="1200.0"
          int="0.01" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="30" textBoxHeight="14" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="m_fOscWTPos" id="b5adf4c8a4228f81" memberName="m_fOscWTPos"
          virtualName="VASTParameterSlider" explicitFocusOrder="0" pos="1.6% 55.385% 8% 30.769%"
          tooltip="Oscillator wavetable position" textboxtext="ffc9c9c9"
          textboxbkgd="ffffff" textboxoutline="808080" min="0.0" max="100.0"
          int="0.01" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="30" textBoxHeight="14" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="m_fOscMorph" id="4e8fb0aa9098c083" memberName="m_fOscMorph"
          virtualName="VASTParameterSlider" explicitFocusOrder="0" pos="9.6% 55.385% 8% 30.769%"
          tooltip="Oscillator morphing" textboxtext="ffc9c9c9" textboxbkgd="ffffff"
          textboxoutline="808080" min="0.0" max="100.0" int="0.01" style="RotaryVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="30"
          textBoxHeight="14" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="m_fSpread" id="780d89d221ce1a79" memberName="m_fSpread"
          virtualName="VASTParameterSlider" explicitFocusOrder="0" pos="1.6% 12.308% 8% 30.769%"
          tooltip="Stereo spread within bank oscillators" textboxtext="ffc9c9c9"
          textboxbkgd="ffffff" textboxoutline="808080" min="0.0" max="100.0"
          int="0.01" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="30" textBoxHeight="14" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="new label" id="9742071c5ced5441" memberName="headerLabel"
         virtualName="" explicitFocusOrder="0" pos="3.2% 0.769% 14.4% 12.308%"
         textCol="ffe2e2e2" edTextCol="ff000000" edBkgCol="0" labelText="OSC A"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Syntax" fontsize="13.0" kerning="0.0" bold="1" italic="0"
         justification="33" typefaceStyle="Bold"/>
  <LABEL name="new label" id="6aefa24cc29e3bd5" memberName="label18" virtualName=""
         explicitFocusOrder="0" pos="-1.8% 85.385% 12.8% 9.231%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="WTPOS" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Syntax"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <LABEL name="new label" id="9050920a08f3ea65" memberName="label108"
         virtualName="" explicitFocusOrder="0" pos="-1.6% 42.308% 12.8% 9.231%"
         textCol="ffe2e2e2" edTextCol="ff000000" edBkgCol="0" labelText="SPREAD"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Syntax" fontsize="11.0" kerning="0.0" bold="0" italic="0"
         justification="36"/>
  <LABEL name="new label" id="99cfd5489fa26382" memberName="label134"
         virtualName="" explicitFocusOrder="0" pos="7% 85.385% 12.8% 9.231%"
         textCol="ffe2e2e2" edTextCol="ff000000" edBkgCol="0" labelText="MORPH"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Syntax" fontsize="11.0" kerning="0.0" bold="0" italic="0"
         justification="36"/>
  <LABEL name="new label" id="ebcfe7ba251e1261" memberName="label135"
         virtualName="" explicitFocusOrder="0" pos="7.6% 42.308% 12.8% 9.231%"
         textCol="ffe2e2e2" edTextCol="ff000000" edBkgCol="0" labelText="DETUNE"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Syntax" fontsize="11.0" kerning="0.0" bold="0" italic="0"
         justification="36"/>
  <LABEL name="new label" id="2ca2709f0430a7af" memberName="label2" virtualName=""
         explicitFocusOrder="0" pos="15.4% 42.308% 12.8% 9.231%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="GAIN" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Syntax"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <LABEL name="new label" id="c7a808becc21f7cd" memberName="label3" virtualName=""
         explicitFocusOrder="0" pos="15.6% 85.385% 12.6% 9.231%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="CENTS" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Syntax"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <SLIDER name="m_fOscPan" id="50c4ade39fc1b8e7" memberName="m_fOscPan"
          virtualName="VASTParameterSlider" explicitFocusOrder="0" pos="25.6% 12.308% 8% 30.769%"
          tooltip="Stereo panning of oscillator" textboxtext="ffc9c9c9"
          textboxbkgd="ffffff" textboxoutline="808080" min="-100.0" max="100.0"
          int="0.01" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="30" textBoxHeight="14" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="m_fOscPhase" id="e360d509dcd6c508" memberName="m_fOscPhase"
          virtualName="VASTParameterSlider" explicitFocusOrder="0" pos="25.6% 55.385% 8% 30.769%"
          tooltip="Phase of oscillator" textboxtext="ffc9c9c9" textboxbkgd="ffffff"
          textboxoutline="808080" min="0.0" max="100.0" int="0.01" style="RotaryVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="30"
          textBoxHeight="14" skewFactor="1.0" needsCallback="1"/>
  <LABEL name="new label" id="e74ba06e7d0faa51" memberName="label4" virtualName=""
         explicitFocusOrder="0" pos="23.2% 42.308% 12.8% 9.231%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="PAN" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Syntax"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <LABEL name="new label" id="58eccb297081ba51" memberName="label5" virtualName=""
         explicitFocusOrder="0" pos="23.2% 85.385% 12.8% 9.231%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="PHASE" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Syntax"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <LABEL name="new label" id="b5d061f660319d63" memberName="label6" virtualName=""
         explicitFocusOrder="0" pos="27.4% 2.308% 12.8% 9.231%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="OCT" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Syntax"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="34"/>
  <LABEL name="new label" id="c0cc2c7ae9d8c05c" memberName="label7" virtualName=""
         explicitFocusOrder="0" pos="9.4% 2.308% 12.8% 9.231%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="UNISON" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Syntax"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="34"/>
  <LABEL name="new label" id="8c03adfd3daa2621" memberName="label8" virtualName=""
         explicitFocusOrder="0" pos="39.2% 29.231% 13% 9.231%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="EXCITER" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Syntax"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="170dffe71809c688" memberName="label9" virtualName=""
         explicitFocusOrder="0" pos="39.2% 41.538% 12.8% 9.231%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="RETRIG" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Syntax"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="656ee062d26c6572" memberName="label13" virtualName=""
         explicitFocusOrder="0" pos="38.4% -30.769% 12.8% 9.231%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="ENV" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Syntax"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="34"/>
  <LABEL name="new label" id="4cdfea23a10875e0" memberName="label14" virtualName=""
         explicitFocusOrder="0" pos="63.2% 2.308% 12.8% 9.231%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="ROUTING" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Syntax"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="34"/>
  <LABEL name="new label" id="954abb7b105862c1" memberName="label15" virtualName=""
         explicitFocusOrder="0" pos="76.4% 2.308% 12.8% 9.231%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="2" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Syntax"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="34"/>
  <LABEL name="new label" id="e2d20eeb03534a1b" memberName="label12" virtualName=""
         explicitFocusOrder="0" pos="39.2% 16.923% 12.8% 9.231%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="INVERT" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Syntax"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <COMBOBOX name="m_uOscRouting2" id="38371de209f59a17" memberName="m_uOscRouting2"
            virtualName="VASTParameterComboBox" explicitFocusOrder="0" pos="89% 1.538% 10.2% 10.769%"
            tooltip="Oscillator routing 2" editable="0" layout="33" items="MSEG1&#10;MSEG2&#10;MSEG3&#10;MSEG4&#10;MSEG5"
            textWhenNonSelected="" textWhenNoItems="---"/>
  <COMBOBOX name="m_uOscRouting1" id="c4f75fd447cf8661" memberName="m_uOscRouting1"
            virtualName="VASTParameterComboBox" explicitFocusOrder="0" pos="75.6% 1.538% 10.2% 10.769%"
            tooltip="Oscillator routing 1" editable="0" layout="33" items="MSEG1&#10;MSEG2&#10;MSEG3&#10;MSEG4&#10;MSEG5"
            textWhenNonSelected="" textWhenNoItems="---"/>
  <SLIDER name="m_iOscOct" id="7d9a12f4751c7263" memberName="m_iOscOct"
          virtualName="VASTParameterSlider" explicitFocusOrder="0" pos="39.8% 1.538% 12% 10.769%"
          tooltip="Bank oscillators octave" bkgcol="0" thumbcol="ff000000"
          trackcol="ff000000" textboxtext="ffc9c9c9" textboxbkgd="ffffff"
          textboxhighlight="ff008000" textboxoutline="ff8e989b" min="-2.0"
          max="2.0" int="1.0" style="IncDecButtons" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="26" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="m_iNumOscs" id="3867d9d545bdc100" memberName="m_iNumOscs"
          virtualName="VASTParameterSlider" explicitFocusOrder="0" pos="21.8% 1.538% 12% 10.769%"
          tooltip="Number of bank oscillators" bkgcol="0" thumbcol="ff000000"
          trackcol="ff000000" textboxtext="ffc9c9c9" textboxbkgd="ffffff"
          textboxhighlight="ff008000" textboxoutline="ff8e989b" min="1.0"
          max="24.0" int="1.0" style="IncDecButtons" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="30" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <TOGGLEBUTTON name="m_bOscOnOff" id="57d84eaa242e7be8" memberName="m_bOscOnOff"
                virtualName="VASTParameterButton" explicitFocusOrder="0" pos="0% 0% 3.6% 13.846%"
                tooltip="Switch oscillator bank on / off" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <COMBOBOX name="m_uVCAEnv" id="5a1165970dadc00b" memberName="m_uVCAEnv"
            virtualName="VASTParameterComboBox" explicitFocusOrder="0" pos="52.8% 1.538% 12% 10.769%"
            tooltip="Envelope used as VCA envelope" editable="0" layout="33"
            items="MSEG1&#10;MSEG2&#10;MSEG3&#10;MSEG4&#10;MSEG5" textWhenNonSelected=""
            textWhenNoItems="---"/>
  <TOGGLEBUTTON name="m_bOscInverterOnOff" id="5ffda229d155c86d" memberName="m_bOscInverterOnOff"
                virtualName="VASTParameterButton" explicitFocusOrder="0" pos="36% 14.615% 3.6% 13.846%"
                tooltip="Invert all voices not in the center (might cancel sound when not detuned)"
                buttonText="" connectedEdges="0" needsCallback="1" radioGroupId="0"
                state="0"/>
  <TOGGLEBUTTON name="m_bExciterOnOff" id="1d94b90683d2fb39" memberName="m_bExciterOnOff"
                virtualName="VASTParameterButton" explicitFocusOrder="0" pos="36% 26.923% 3.6% 13.846%"
                tooltip="Exciter adds random detune by may +- 0.25hz per oscillator and played note"
                buttonText="" connectedEdges="0" needsCallback="1" radioGroupId="0"
                state="0"/>
  <TOGGLEBUTTON name="m_bOscRetrigOnOff" id="f9d596c373ab1af8" memberName="m_bOscRetrigOnOff"
                virtualName="VASTParameterButton" explicitFocusOrder="0" pos="36% 39.231% 3.6% 13.846%"
                tooltip="Random phase offeset for each oscillator for each new note or sync at beginning of phase"
                buttonText="" connectedEdges="0" needsCallback="1" radioGroupId="0"
                state="0"/>
  <LABEL name="new label" id="f61ab4233bf9cd6e" memberName="label10" virtualName=""
         explicitFocusOrder="0" pos="27.6% 85.385% 12.8% 9.231%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="WTFX" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Syntax"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="34"/>
  <COMBOBOX name="m_uWTFX" id="131524e89f7eb615" memberName="m_uWTFX" virtualName="VASTParameterComboBox"
            explicitFocusOrder="0" pos="39.8% 84.615% 12% 10.769%" tooltip="Wavetable Effect"
            editable="0" layout="33" items="" textWhenNonSelected="" textWhenNoItems="---"/>
  <SLIDER name="m_fWTFXVal" id="d914fa4c5d9b7bb8" memberName="m_fWTFXVal"
          virtualName="VASTParameterSlider" explicitFocusOrder="0" pos="41.4% 55.385% 8% 30.769%"
          tooltip="Wavetable effect parameter value" textboxtext="ffc9c9c9"
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

