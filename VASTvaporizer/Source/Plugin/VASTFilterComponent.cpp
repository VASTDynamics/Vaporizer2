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
#include "VASTControls/VASTDrawableButton.h"
//[/Headers]

#include "VASTFilterComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
VASTFilterComponent::VASTFilterComponent (AudioProcessorEditor *editor, AudioProcessor* processor)
    : myEditor((VASTAudioProcessorEditor*)editor), myProcessor((VASTAudioProcessor*)processor)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    label40.reset (new juce::Label ("new label",
                                    TRANS ("CUTOFF")));
    addAndMakeVisible (label40.get());
    label40->setFont (juce::Font (9.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label40->setJustificationType (juce::Justification::centred);
    label40->setEditable (false, false, false);
    label40->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label40->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label40->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label42.reset (new juce::Label ("new label",
                                    TRANS ("RESO")));
    addAndMakeVisible (label42.get());
    label42->setFont (juce::Font (9.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label42->setJustificationType (juce::Justification::centred);
    label42->setEditable (false, false, false);
    label42->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label42->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label42->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label75.reset (new juce::Label ("new label",
                                    TRANS ("RESO")));
    addAndMakeVisible (label75.get());
    label75->setFont (juce::Font (9.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label75->setJustificationType (juce::Justification::centred);
    label75->setEditable (false, false, false);
    label75->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label75->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label75->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label26.reset (new juce::Label ("new label",
                                    TRANS ("CUTOFF")));
    addAndMakeVisible (label26.get());
    label26->setFont (juce::Font (9.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label26->setJustificationType (juce::Justification::centred);
    label26->setEditable (false, false, false);
    label26->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label26->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label26->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label28.reset (new juce::Label ("new label",
                                    TRANS ("DRIVE")));
    addAndMakeVisible (label28.get());
    label28->setFont (juce::Font (9.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label28->setJustificationType (juce::Justification::centred);
    label28->setEditable (false, false, false);
    label28->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label28->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label28->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label44.reset (new juce::Label ("new label",
                                    TRANS ("DRIVE")));
    addAndMakeVisible (label44.get());
    label44->setFont (juce::Font (9.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label44->setJustificationType (juce::Justification::centred);
    label44->setEditable (false, false, false);
    label44->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label44->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label44->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    m_fFilterCutoff_Filter2.reset (new VASTParameterSlider ("m_fFilterCutoff_Filter2"));
    addAndMakeVisible (m_fFilterCutoff_Filter2.get());
    m_fFilterCutoff_Filter2->setTooltip (TRANS ("Filter 2 cutoff (hz)"));
    m_fFilterCutoff_Filter2->setExplicitFocusOrder (16);
    m_fFilterCutoff_Filter2->setRange (20, 22000, 0.001);
    m_fFilterCutoff_Filter2->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    m_fFilterCutoff_Filter2->setTextBoxStyle (juce::Slider::NoTextBox, false, 30, 14);
    m_fFilterCutoff_Filter2->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffc9c9c9));
    m_fFilterCutoff_Filter2->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0x00ffffff));
    m_fFilterCutoff_Filter2->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x00808080));
    m_fFilterCutoff_Filter2->addListener (this);

    m_fFilterReso_Filter1.reset (new VASTParameterSlider ("m_fFilterReso_Filter1"));
    addAndMakeVisible (m_fFilterReso_Filter1.get());
    m_fFilterReso_Filter1->setTooltip (TRANS ("Filter 1 resonance"));
    m_fFilterReso_Filter1->setExplicitFocusOrder (5);
    m_fFilterReso_Filter1->setRange (0, 100, 0.001);
    m_fFilterReso_Filter1->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    m_fFilterReso_Filter1->setTextBoxStyle (juce::Slider::NoTextBox, false, 30, 14);
    m_fFilterReso_Filter1->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffc9c9c9));
    m_fFilterReso_Filter1->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0x00ffffff));
    m_fFilterReso_Filter1->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x00808080));
    m_fFilterReso_Filter1->addListener (this);

    m_fVCFEnvMod_Filter2.reset (new VASTParameterSlider ("m_fVCFEnvMod_Filter2"));
    addAndMakeVisible (m_fVCFEnvMod_Filter2.get());
    m_fVCFEnvMod_Filter2->setTooltip (TRANS ("VCF filter 2 envelope modulation"));
    m_fVCFEnvMod_Filter2->setExplicitFocusOrder (21);
    m_fVCFEnvMod_Filter2->setRange (-100, 100, 0.01);
    m_fVCFEnvMod_Filter2->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    m_fVCFEnvMod_Filter2->setTextBoxStyle (juce::Slider::NoTextBox, false, 30, 14);
    m_fVCFEnvMod_Filter2->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffc9c9c9));
    m_fVCFEnvMod_Filter2->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0x00ffffff));
    m_fVCFEnvMod_Filter2->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x00808080));
    m_fVCFEnvMod_Filter2->addListener (this);

    m_fFilterCutoff_Filter1.reset (new VASTParameterSlider ("m_fFilterCutoff_Filter1"));
    addAndMakeVisible (m_fFilterCutoff_Filter1.get());
    m_fFilterCutoff_Filter1->setTooltip (TRANS ("Filter 1 cutoff (hz)"));
    m_fFilterCutoff_Filter1->setExplicitFocusOrder (4);
    m_fFilterCutoff_Filter1->setRange (20, 22000, 0.001);
    m_fFilterCutoff_Filter1->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    m_fFilterCutoff_Filter1->setTextBoxStyle (juce::Slider::NoTextBox, false, 30, 14);
    m_fFilterCutoff_Filter1->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffc9c9c9));
    m_fFilterCutoff_Filter1->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0x00ffffff));
    m_fFilterCutoff_Filter1->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x00808080));
    m_fFilterCutoff_Filter1->addListener (this);

    m_fFilterDrive_Filter2.reset (new VASTParameterSlider ("m_fFilterDrive_Filter2"));
    addAndMakeVisible (m_fFilterDrive_Filter2.get());
    m_fFilterDrive_Filter2->setTooltip (TRANS ("Filter 2 drive in dB - pre gain (0 - full, >0 overdrive)"));
    m_fFilterDrive_Filter2->setExplicitFocusOrder (15);
    m_fFilterDrive_Filter2->setRange (-20, 20, 0.001);
    m_fFilterDrive_Filter2->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    m_fFilterDrive_Filter2->setTextBoxStyle (juce::Slider::NoTextBox, false, 30, 14);
    m_fFilterDrive_Filter2->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffc9c9c9));
    m_fFilterDrive_Filter2->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0x00ffffff));
    m_fFilterDrive_Filter2->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x00808080));
    m_fFilterDrive_Filter2->addListener (this);

    m_fFilterReso_Filter2.reset (new VASTParameterSlider ("m_fFilterReso_Filter2"));
    addAndMakeVisible (m_fFilterReso_Filter2.get());
    m_fFilterReso_Filter2->setTooltip (TRANS ("Filter 2 resonance"));
    m_fFilterReso_Filter2->setExplicitFocusOrder (17);
    m_fFilterReso_Filter2->setRange (0, 100, 0.001);
    m_fFilterReso_Filter2->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    m_fFilterReso_Filter2->setTextBoxStyle (juce::Slider::NoTextBox, false, 30, 14);
    m_fFilterReso_Filter2->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffc9c9c9));
    m_fFilterReso_Filter2->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0x00ffffff));
    m_fFilterReso_Filter2->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x00808080));
    m_fFilterReso_Filter2->addListener (this);

    m_fFilterDrive_Filter1.reset (new VASTParameterSlider ("m_fFilterDrive_Filter1"));
    addAndMakeVisible (m_fFilterDrive_Filter1.get());
    m_fFilterDrive_Filter1->setTooltip (TRANS ("Filter 1 drive in dB - pre gain (0 - full, >0 overdrive)"));
    m_fFilterDrive_Filter1->setExplicitFocusOrder (3);
    m_fFilterDrive_Filter1->setRange (-20, 20, 0.001);
    m_fFilterDrive_Filter1->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    m_fFilterDrive_Filter1->setTextBoxStyle (juce::Slider::NoTextBox, false, 30, 14);
    m_fFilterDrive_Filter1->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffc9c9c9));
    m_fFilterDrive_Filter1->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0x00ffffff));
    m_fFilterDrive_Filter1->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x00808080));
    m_fFilterDrive_Filter1->addListener (this);

    label77.reset (new juce::Label ("new label",
                                    TRANS ("CUTOFF")));
    addAndMakeVisible (label77.get());
    label77->setFont (juce::Font (9.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label77->setJustificationType (juce::Justification::centred);
    label77->setEditable (false, false, false);
    label77->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label77->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label77->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label78.reset (new juce::Label ("new label",
                                    TRANS ("RESO")));
    addAndMakeVisible (label78.get());
    label78->setFont (juce::Font (9.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label78->setJustificationType (juce::Justification::centred);
    label78->setEditable (false, false, false);
    label78->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label78->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label78->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label81.reset (new juce::Label ("new label",
                                    TRANS ("DRIVE")));
    addAndMakeVisible (label81.get());
    label81->setFont (juce::Font (9.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label81->setJustificationType (juce::Justification::centred);
    label81->setEditable (false, false, false);
    label81->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label81->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label81->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    m_fFilterCutoff_Filter3.reset (new VASTParameterSlider ("m_fFilterCutoff_Filter3"));
    addAndMakeVisible (m_fFilterCutoff_Filter3.get());
    m_fFilterCutoff_Filter3->setTooltip (TRANS ("Filter 3 cutoff (hz)"));
    m_fFilterCutoff_Filter3->setExplicitFocusOrder (28);
    m_fFilterCutoff_Filter3->setRange (20, 22000, 0.001);
    m_fFilterCutoff_Filter3->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    m_fFilterCutoff_Filter3->setTextBoxStyle (juce::Slider::NoTextBox, false, 30, 14);
    m_fFilterCutoff_Filter3->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffc9c9c9));
    m_fFilterCutoff_Filter3->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0x00ffffff));
    m_fFilterCutoff_Filter3->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x00808080));
    m_fFilterCutoff_Filter3->addListener (this);

    m_fFilterDrive_Filter3.reset (new VASTParameterSlider ("m_fFilterDrive_Filter3"));
    addAndMakeVisible (m_fFilterDrive_Filter3.get());
    m_fFilterDrive_Filter3->setTooltip (TRANS ("Filter 3 drive in dB - pre gain (0 - full, >0 overdrive)"));
    m_fFilterDrive_Filter3->setExplicitFocusOrder (27);
    m_fFilterDrive_Filter3->setRange (-20, 20, 0.001);
    m_fFilterDrive_Filter3->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    m_fFilterDrive_Filter3->setTextBoxStyle (juce::Slider::NoTextBox, false, 30, 14);
    m_fFilterDrive_Filter3->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffc9c9c9));
    m_fFilterDrive_Filter3->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0x00ffffff));
    m_fFilterDrive_Filter3->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x00808080));
    m_fFilterDrive_Filter3->addListener (this);

    m_fFilterReso_Filter3.reset (new VASTParameterSlider ("m_fFilterReso_Filter3"));
    addAndMakeVisible (m_fFilterReso_Filter3.get());
    m_fFilterReso_Filter3->setTooltip (TRANS ("Filter 3 resonance"));
    m_fFilterReso_Filter3->setExplicitFocusOrder (29);
    m_fFilterReso_Filter3->setRange (0, 100, 0.001);
    m_fFilterReso_Filter3->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    m_fFilterReso_Filter3->setTextBoxStyle (juce::Slider::NoTextBox, false, 30, 14);
    m_fFilterReso_Filter3->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffc9c9c9));
    m_fFilterReso_Filter3->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0x00ffffff));
    m_fFilterReso_Filter3->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x00808080));
    m_fFilterReso_Filter3->addListener (this);

    m_fFilterDryWet_Filter1.reset (new VASTParameterSlider ("m_fFilterDryWet_Filter1"));
    addAndMakeVisible (m_fFilterDryWet_Filter1.get());
    m_fFilterDryWet_Filter1->setTooltip (TRANS ("Filter 1 mix in (dry - wet)"));
    m_fFilterDryWet_Filter1->setExplicitFocusOrder (2);
    m_fFilterDryWet_Filter1->setRange (0, 100, 0.01);
    m_fFilterDryWet_Filter1->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    m_fFilterDryWet_Filter1->setTextBoxStyle (juce::Slider::NoTextBox, false, 30, 14);
    m_fFilterDryWet_Filter1->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffc9c9c9));
    m_fFilterDryWet_Filter1->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0x00ffffff));
    m_fFilterDryWet_Filter1->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x00808080));
    m_fFilterDryWet_Filter1->addListener (this);

    m_fFilterDryWet_Filter3.reset (new VASTParameterSlider ("m_fFilterDryWet_Filter3"));
    addAndMakeVisible (m_fFilterDryWet_Filter3.get());
    m_fFilterDryWet_Filter3->setTooltip (TRANS ("Filter 3 mix in (dry - wet)"));
    m_fFilterDryWet_Filter3->setExplicitFocusOrder (26);
    m_fFilterDryWet_Filter3->setRange (0, 100, 0.01);
    m_fFilterDryWet_Filter3->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    m_fFilterDryWet_Filter3->setTextBoxStyle (juce::Slider::NoTextBox, false, 30, 14);
    m_fFilterDryWet_Filter3->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffc9c9c9));
    m_fFilterDryWet_Filter3->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0x00ffffff));
    m_fFilterDryWet_Filter3->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x00808080));
    m_fFilterDryWet_Filter3->addListener (this);

    label97.reset (new juce::Label ("new label",
                                    TRANS ("WET")));
    addAndMakeVisible (label97.get());
    label97->setFont (juce::Font (9.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label97->setJustificationType (juce::Justification::centred);
    label97->setEditable (false, false, false);
    label97->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label97->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label97->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label112.reset (new juce::Label ("new label",
                                     TRANS ("WET")));
    addAndMakeVisible (label112.get());
    label112->setFont (juce::Font (9.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label112->setJustificationType (juce::Justification::centred);
    label112->setEditable (false, false, false);
    label112->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label112->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label112->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label113.reset (new juce::Label ("new label",
                                     TRANS ("WET")));
    addAndMakeVisible (label113.get());
    label113->setFont (juce::Font (9.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label113->setJustificationType (juce::Justification::centred);
    label113->setEditable (false, false, false);
    label113->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label113->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label113->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    m_fFilterDryWet_Filter2.reset (new VASTParameterSlider ("m_fFilterDryWet_Filter2"));
    addAndMakeVisible (m_fFilterDryWet_Filter2.get());
    m_fFilterDryWet_Filter2->setTooltip (TRANS ("Filter 2 mix in (dry - wet)"));
    m_fFilterDryWet_Filter2->setExplicitFocusOrder (14);
    m_fFilterDryWet_Filter2->setRange (0, 100, 0.01);
    m_fFilterDryWet_Filter2->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    m_fFilterDryWet_Filter2->setTextBoxStyle (juce::Slider::NoTextBox, false, 30, 14);
    m_fFilterDryWet_Filter2->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffc9c9c9));
    m_fFilterDryWet_Filter2->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0x00ffffff));
    m_fFilterDryWet_Filter2->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x00808080));
    m_fFilterDryWet_Filter2->addListener (this);

    m_fVCFEnvMod_Filter1.reset (new VASTParameterSlider ("m_fVCFEnvMod_Filter1"));
    addAndMakeVisible (m_fVCFEnvMod_Filter1.get());
    m_fVCFEnvMod_Filter1->setTooltip (TRANS ("VCF filter 1 envelope modulation"));
    m_fVCFEnvMod_Filter1->setExplicitFocusOrder (9);
    m_fVCFEnvMod_Filter1->setRange (-100, 100, 0.01);
    m_fVCFEnvMod_Filter1->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    m_fVCFEnvMod_Filter1->setTextBoxStyle (juce::Slider::NoTextBox, false, 30, 14);
    m_fVCFEnvMod_Filter1->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffc9c9c9));
    m_fVCFEnvMod_Filter1->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0x00ffffff));
    m_fVCFEnvMod_Filter1->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x00808080));
    m_fVCFEnvMod_Filter1->addListener (this);

    m_fVCFEnvMod_Filter3.reset (new VASTParameterSlider ("m_fVCFEnvMod_Filter3"));
    addAndMakeVisible (m_fVCFEnvMod_Filter3.get());
    m_fVCFEnvMod_Filter3->setTooltip (TRANS ("VCF filter 3 envelope modulation"));
    m_fVCFEnvMod_Filter3->setExplicitFocusOrder (33);
    m_fVCFEnvMod_Filter3->setRange (-100, 100, 0.01);
    m_fVCFEnvMod_Filter3->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    m_fVCFEnvMod_Filter3->setTextBoxStyle (juce::Slider::NoTextBox, false, 30, 14);
    m_fVCFEnvMod_Filter3->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffc9c9c9));
    m_fVCFEnvMod_Filter3->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0x00ffffff));
    m_fVCFEnvMod_Filter3->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x00808080));
    m_fVCFEnvMod_Filter3->addListener (this);

    m_bOnOff_Filter1.reset (new VASTParameterButton ("m_bOnOff_Filter1"));
    addAndMakeVisible (m_bOnOff_Filter1.get());
    m_bOnOff_Filter1->setTooltip (TRANS ("Switch filter 1 on / off"));
    m_bOnOff_Filter1->setButtonText (juce::String());
    m_bOnOff_Filter1->addListener (this);

    m_bOnOff_Filter2.reset (new VASTParameterButton ("m_bOnOff_Filter2"));
    addAndMakeVisible (m_bOnOff_Filter2.get());
    m_bOnOff_Filter2->setTooltip (TRANS ("Switch filter 2 on / off"));
    m_bOnOff_Filter2->setExplicitFocusOrder (12);
    m_bOnOff_Filter2->setButtonText (juce::String());
    m_bOnOff_Filter2->addListener (this);

    m_bOnOff_Filter3.reset (new VASTParameterButton ("m_bOnOff_Filter3"));
    addAndMakeVisible (m_bOnOff_Filter3.get());
    m_bOnOff_Filter3->setTooltip (TRANS ("Switch filter 3 on / off"));
    m_bOnOff_Filter3->setExplicitFocusOrder (24);
    m_bOnOff_Filter3->setButtonText (juce::String());
    m_bOnOff_Filter3->addListener (this);

    c_filterDisplay.reset (new VASTFilterDisplay (myProcessor));
    addAndMakeVisible (c_filterDisplay.get());
    c_filterDisplay->setName ("c_filterDisplay");

    m_uVCFEnv_Filter1.reset (new VASTParameterComboBox ("m_uVCFEnv_Filter1"));
    addAndMakeVisible (m_uVCFEnv_Filter1.get());
    m_uVCFEnv_Filter1->setTooltip (TRANS ("Envelope used as LPF envelope"));
    m_uVCFEnv_Filter1->setExplicitFocusOrder (8);
    m_uVCFEnv_Filter1->setEditableText (false);
    m_uVCFEnv_Filter1->setJustificationType (juce::Justification::centredLeft);
    m_uVCFEnv_Filter1->setTextWhenNothingSelected (juce::String());
    m_uVCFEnv_Filter1->setTextWhenNoChoicesAvailable (TRANS ("---"));
    m_uVCFEnv_Filter1->addListener (this);

    label13.reset (new juce::Label ("new label",
                                    TRANS ("VCFENV")));
    addAndMakeVisible (label13.get());
    label13->setFont (juce::Font ("Syntax", 11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label13->setJustificationType (juce::Justification::centred);
    label13->setEditable (false, false, false);
    label13->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label13->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label13->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    m_uVCFEnv_Filter2.reset (new VASTParameterComboBox ("m_uVCFEnv_Filter2"));
    addAndMakeVisible (m_uVCFEnv_Filter2.get());
    m_uVCFEnv_Filter2->setTooltip (TRANS ("Envelope used as filter 2 envelope"));
    m_uVCFEnv_Filter2->setExplicitFocusOrder (20);
    m_uVCFEnv_Filter2->setEditableText (false);
    m_uVCFEnv_Filter2->setJustificationType (juce::Justification::centredLeft);
    m_uVCFEnv_Filter2->setTextWhenNothingSelected (juce::String());
    m_uVCFEnv_Filter2->setTextWhenNoChoicesAvailable (TRANS ("---"));
    m_uVCFEnv_Filter2->addListener (this);

    m_uVCFEnv_Filter3.reset (new VASTParameterComboBox ("m_uVCFEnv_Filter3"));
    addAndMakeVisible (m_uVCFEnv_Filter3.get());
    m_uVCFEnv_Filter3->setTooltip (TRANS ("Envelope used as filter 3 envelope"));
    m_uVCFEnv_Filter3->setExplicitFocusOrder (32);
    m_uVCFEnv_Filter3->setEditableText (false);
    m_uVCFEnv_Filter3->setJustificationType (juce::Justification::centredLeft);
    m_uVCFEnv_Filter3->setTextWhenNothingSelected (juce::String());
    m_uVCFEnv_Filter3->setTextWhenNoChoicesAvailable (TRANS ("---"));
    m_uVCFEnv_Filter3->addListener (this);

    m_uFilterRouting_Filter1.reset (new VASTParameterComboBox ("m_uFilterRouting_Filter1"));
    addAndMakeVisible (m_uFilterRouting_Filter1.get());
    m_uFilterRouting_Filter1->setTooltip (TRANS ("Filter 1 routing"));
    m_uFilterRouting_Filter1->setExplicitFocusOrder (10);
    m_uFilterRouting_Filter1->setEditableText (false);
    m_uFilterRouting_Filter1->setJustificationType (juce::Justification::centredLeft);
    m_uFilterRouting_Filter1->setTextWhenNothingSelected (juce::String());
    m_uFilterRouting_Filter1->setTextWhenNoChoicesAvailable (TRANS ("---"));
    m_uFilterRouting_Filter1->addListener (this);

    label6.reset (new juce::Label ("new label",
                                   TRANS ("ROUTING 1")));
    addAndMakeVisible (label6.get());
    label6->setFont (juce::Font ("Syntax", 11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label6->setJustificationType (juce::Justification::centred);
    label6->setEditable (false, false, false);
    label6->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label6->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label6->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    m_uFilterRouting_Filter2.reset (new VASTParameterComboBox ("m_uFilterRouting_Filter2"));
    addAndMakeVisible (m_uFilterRouting_Filter2.get());
    m_uFilterRouting_Filter2->setTooltip (TRANS ("Filter 2 routing"));
    m_uFilterRouting_Filter2->setExplicitFocusOrder (22);
    m_uFilterRouting_Filter2->setEditableText (false);
    m_uFilterRouting_Filter2->setJustificationType (juce::Justification::centredLeft);
    m_uFilterRouting_Filter2->setTextWhenNothingSelected (juce::String());
    m_uFilterRouting_Filter2->setTextWhenNoChoicesAvailable (TRANS ("---"));
    m_uFilterRouting_Filter2->addListener (this);

    m_uFilterRouting_Filter3.reset (new VASTParameterComboBox ("m_uFilterRouting_Filter3"));
    addAndMakeVisible (m_uFilterRouting_Filter3.get());
    m_uFilterRouting_Filter3->setTooltip (TRANS ("Filter 3 routing"));
    m_uFilterRouting_Filter3->setExplicitFocusOrder (34);
    m_uFilterRouting_Filter3->setEditableText (false);
    m_uFilterRouting_Filter3->setJustificationType (juce::Justification::centredLeft);
    m_uFilterRouting_Filter3->setTextWhenNothingSelected (juce::String());
    m_uFilterRouting_Filter3->setTextWhenNoChoicesAvailable (TRANS ("---"));
    m_uFilterRouting_Filter3->addListener (this);

    m_uFilterType_Filter1.reset (new VASTParameterComboBox ("m_uFilterType_Filter1"));
    addAndMakeVisible (m_uFilterType_Filter1.get());
    m_uFilterType_Filter1->setTooltip (TRANS ("Filter 1 type"));
    m_uFilterType_Filter1->setExplicitFocusOrder (1);
    m_uFilterType_Filter1->setEditableText (false);
    m_uFilterType_Filter1->setJustificationType (juce::Justification::centredLeft);
    m_uFilterType_Filter1->setTextWhenNothingSelected (juce::String());
    m_uFilterType_Filter1->setTextWhenNoChoicesAvailable (TRANS ("---"));
    m_uFilterType_Filter1->addListener (this);

    m_uFilterType_Filter2.reset (new VASTParameterComboBox ("m_uFilterType_Filter2"));
    addAndMakeVisible (m_uFilterType_Filter2.get());
    m_uFilterType_Filter2->setTooltip (TRANS ("Filter 2 type"));
    m_uFilterType_Filter2->setExplicitFocusOrder (13);
    m_uFilterType_Filter2->setEditableText (false);
    m_uFilterType_Filter2->setJustificationType (juce::Justification::centredLeft);
    m_uFilterType_Filter2->setTextWhenNothingSelected (juce::String());
    m_uFilterType_Filter2->setTextWhenNoChoicesAvailable (TRANS ("---"));
    m_uFilterType_Filter2->addListener (this);

    m_uFilterType_Filter3.reset (new VASTParameterComboBox ("m_uFilterType_Filter3"));
    addAndMakeVisible (m_uFilterType_Filter3.get());
    m_uFilterType_Filter3->setTooltip (TRANS ("Filter 3 type"));
    m_uFilterType_Filter3->setExplicitFocusOrder (25);
    m_uFilterType_Filter3->setEditableText (false);
    m_uFilterType_Filter3->setJustificationType (juce::Justification::centredLeft);
    m_uFilterType_Filter3->setTextWhenNothingSelected (juce::String());
    m_uFilterType_Filter3->setTextWhenNoChoicesAvailable (TRANS ("---"));
    m_uFilterType_Filter3->addListener (this);

    label2.reset (new juce::Label ("new label",
                                   TRANS ("ENV MOD")));
    addAndMakeVisible (label2.get());
    label2->setFont (juce::Font ("Syntax", 11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label2->setJustificationType (juce::Justification::centred);
    label2->setEditable (false, false, false);
    label2->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label2->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label2->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    m_uFilterRouting2_Filter1.reset (new VASTParameterComboBox ("m_uFilterRouting2_Filter1"));
    addAndMakeVisible (m_uFilterRouting2_Filter1.get());
    m_uFilterRouting2_Filter1->setTooltip (TRANS ("Filter 1 routing 2"));
    m_uFilterRouting2_Filter1->setExplicitFocusOrder (11);
    m_uFilterRouting2_Filter1->setEditableText (false);
    m_uFilterRouting2_Filter1->setJustificationType (juce::Justification::centredLeft);
    m_uFilterRouting2_Filter1->setTextWhenNothingSelected (juce::String());
    m_uFilterRouting2_Filter1->setTextWhenNoChoicesAvailable (TRANS ("---"));
    m_uFilterRouting2_Filter1->addListener (this);

    label3.reset (new juce::Label ("new label",
                                   TRANS ("ROUTING 2")));
    addAndMakeVisible (label3.get());
    label3->setFont (juce::Font ("Syntax", 11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label3->setJustificationType (juce::Justification::centred);
    label3->setEditable (false, false, false);
    label3->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label3->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label3->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    m_uFilterRouting2_Filter2.reset (new VASTParameterComboBox ("m_uFilterRouting2_Filter2"));
    addAndMakeVisible (m_uFilterRouting2_Filter2.get());
    m_uFilterRouting2_Filter2->setTooltip (TRANS ("Filter 2 routing 2"));
    m_uFilterRouting2_Filter2->setExplicitFocusOrder (23);
    m_uFilterRouting2_Filter2->setEditableText (false);
    m_uFilterRouting2_Filter2->setJustificationType (juce::Justification::centredLeft);
    m_uFilterRouting2_Filter2->setTextWhenNothingSelected (juce::String());
    m_uFilterRouting2_Filter2->setTextWhenNoChoicesAvailable (TRANS ("---"));
    m_uFilterRouting2_Filter2->addListener (this);

    m_uFilterRouting2_Filter3.reset (new VASTParameterComboBox ("m_uFilterRouting2_Filter3"));
    addAndMakeVisible (m_uFilterRouting2_Filter3.get());
    m_uFilterRouting2_Filter3->setTooltip (TRANS ("Filter 3 routing 2"));
    m_uFilterRouting2_Filter3->setExplicitFocusOrder (35);
    m_uFilterRouting2_Filter3->setEditableText (false);
    m_uFilterRouting2_Filter3->setJustificationType (juce::Justification::centredLeft);
    m_uFilterRouting2_Filter3->setTextWhenNothingSelected (juce::String());
    m_uFilterRouting2_Filter3->setTextWhenNoChoicesAvailable (TRANS ("---"));
    m_uFilterRouting2_Filter3->addListener (this);

    label4.reset (new juce::Label ("new label",
                                   TRANS ("SCALE")));
    addAndMakeVisible (label4.get());
    label4->setFont (juce::Font (9.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label4->setJustificationType (juce::Justification::centred);
    label4->setEditable (false, false, false);
    label4->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label4->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label4->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label5.reset (new juce::Label ("new label",
                                   TRANS ("SCALE")));
    addAndMakeVisible (label5.get());
    label5->setFont (juce::Font (9.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label5->setJustificationType (juce::Justification::centred);
    label5->setEditable (false, false, false);
    label5->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label5->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label5->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    m_fFilterScale_Filter1.reset (new VASTParameterSlider ("m_fFilterScale_Filter1"));
    addAndMakeVisible (m_fFilterScale_Filter1.get());
    m_fFilterScale_Filter1->setTooltip (TRANS ("Filter 1 scale (gain for notch, peak, shelf filters)"));
    m_fFilterScale_Filter1->setExplicitFocusOrder (6);
    m_fFilterScale_Filter1->setRange (0, 100, 0.001);
    m_fFilterScale_Filter1->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    m_fFilterScale_Filter1->setTextBoxStyle (juce::Slider::NoTextBox, false, 30, 14);
    m_fFilterScale_Filter1->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffc9c9c9));
    m_fFilterScale_Filter1->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0x00ffffff));
    m_fFilterScale_Filter1->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x00808080));
    m_fFilterScale_Filter1->addListener (this);

    m_fFilterScale_Filter2.reset (new VASTParameterSlider ("m_fFilterScale_Filter2"));
    addAndMakeVisible (m_fFilterScale_Filter2.get());
    m_fFilterScale_Filter2->setTooltip (TRANS ("Filter 2 scale (gain for notch, peak, shelf filters)"));
    m_fFilterScale_Filter2->setExplicitFocusOrder (18);
    m_fFilterScale_Filter2->setRange (0, 100, 0.001);
    m_fFilterScale_Filter2->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    m_fFilterScale_Filter2->setTextBoxStyle (juce::Slider::NoTextBox, false, 30, 14);
    m_fFilterScale_Filter2->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffc9c9c9));
    m_fFilterScale_Filter2->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0x00ffffff));
    m_fFilterScale_Filter2->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x00808080));
    m_fFilterScale_Filter2->addListener (this);

    label7.reset (new juce::Label ("new label",
                                   TRANS ("SCALE")));
    addAndMakeVisible (label7.get());
    label7->setFont (juce::Font (9.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label7->setJustificationType (juce::Justification::centred);
    label7->setEditable (false, false, false);
    label7->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label7->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label7->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    m_fFilterScale_Filter3.reset (new VASTParameterSlider ("m_fFilterScale_Filter3"));
    addAndMakeVisible (m_fFilterScale_Filter3.get());
    m_fFilterScale_Filter3->setTooltip (TRANS ("Filter 3 scale (gain for notch, peak, shelf filters)"));
    m_fFilterScale_Filter3->setExplicitFocusOrder (30);
    m_fFilterScale_Filter3->setRange (0, 100, 0.001);
    m_fFilterScale_Filter3->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    m_fFilterScale_Filter3->setTextBoxStyle (juce::Slider::NoTextBox, false, 30, 14);
    m_fFilterScale_Filter3->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffc9c9c9));
    m_fFilterScale_Filter3->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0x00ffffff));
    m_fFilterScale_Filter3->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x00808080));
    m_fFilterScale_Filter3->addListener (this);

    label8.reset (new juce::Label ("new label",
                                   TRANS ("GAIN")));
    addAndMakeVisible (label8.get());
    label8->setFont (juce::Font (9.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label8->setJustificationType (juce::Justification::centred);
    label8->setEditable (false, false, false);
    label8->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label8->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label8->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label9.reset (new juce::Label ("new label",
                                   TRANS ("GAIN")));
    addAndMakeVisible (label9.get());
    label9->setFont (juce::Font (9.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label9->setJustificationType (juce::Justification::centred);
    label9->setEditable (false, false, false);
    label9->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label9->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label9->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    m_fFilterGain_Filter1.reset (new VASTParameterSlider ("m_fFilterGain_Filter1"));
    addAndMakeVisible (m_fFilterGain_Filter1.get());
    m_fFilterGain_Filter1->setTooltip (TRANS ("Filter 1 output gain"));
    m_fFilterGain_Filter1->setExplicitFocusOrder (7);
    m_fFilterGain_Filter1->setRange (0, 200, 0.001);
    m_fFilterGain_Filter1->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    m_fFilterGain_Filter1->setTextBoxStyle (juce::Slider::NoTextBox, false, 30, 14);
    m_fFilterGain_Filter1->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffc9c9c9));
    m_fFilterGain_Filter1->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0x00ffffff));
    m_fFilterGain_Filter1->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x00808080));
    m_fFilterGain_Filter1->addListener (this);

    m_fFilterGain_Filter2.reset (new VASTParameterSlider ("m_fFilterGain_Filter2"));
    addAndMakeVisible (m_fFilterGain_Filter2.get());
    m_fFilterGain_Filter2->setTooltip (TRANS ("Filter 2 output gain"));
    m_fFilterGain_Filter2->setExplicitFocusOrder (19);
    m_fFilterGain_Filter2->setRange (0, 200, 0.001);
    m_fFilterGain_Filter2->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    m_fFilterGain_Filter2->setTextBoxStyle (juce::Slider::NoTextBox, false, 30, 14);
    m_fFilterGain_Filter2->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffc9c9c9));
    m_fFilterGain_Filter2->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0x00ffffff));
    m_fFilterGain_Filter2->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x00808080));
    m_fFilterGain_Filter2->addListener (this);

    label10.reset (new juce::Label ("new label",
                                    TRANS ("GAIN")));
    addAndMakeVisible (label10.get());
    label10->setFont (juce::Font (9.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label10->setJustificationType (juce::Justification::centred);
    label10->setEditable (false, false, false);
    label10->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label10->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label10->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    m_fFilterGain_Filter3.reset (new VASTParameterSlider ("m_fFilterGain_Filter3"));
    addAndMakeVisible (m_fFilterGain_Filter3.get());
    m_fFilterGain_Filter3->setTooltip (TRANS ("Filter 3 output gain"));
    m_fFilterGain_Filter3->setExplicitFocusOrder (31);
    m_fFilterGain_Filter3->setRange (0, 200, 0.001);
    m_fFilterGain_Filter3->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    m_fFilterGain_Filter3->setTextBoxStyle (juce::Slider::NoTextBox, false, 30, 14);
    m_fFilterGain_Filter3->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffc9c9c9));
    m_fFilterGain_Filter3->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0x00ffffff));
    m_fFilterGain_Filter3->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x00808080));
    m_fFilterGain_Filter3->addListener (this);

    label11.reset (new juce::Label ("new label",
                                    TRANS ("TYPE")));
    addAndMakeVisible (label11.get());
    label11->setFont (juce::Font ("Syntax", 11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label11->setJustificationType (juce::Justification::centred);
    label11->setEditable (false, false, false);
    label11->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label11->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label11->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

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
				aSlider->bindParameter(aSlider->getName());
			}
			auto* aCombobox = dynamic_cast<VASTParameterComboBox*> (child);
			if (aCombobox != nullptr) {
				aCombobox->setAudioProcessor(*myProcessor);
				aCombobox->bindParameter(aCombobox->getName());
			}
			auto* aButton = dynamic_cast<VASTParameterButton*> (child);
			if (aButton != nullptr) {
				aButton->setAudioProcessor(*myProcessor);
				aButton->bindParameter(aButton->getName());
			}
		}
	}

	c_iconMaximizeEditor->addListener(this);

	c_filterDisplay->setProcessor(myProcessor);
	c_filterDisplay->setEditor(myEditor);
	setOpaque(true);

    return; //dont call setSizes
    //[/UserPreSize]

    setSize (800, 575);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

VASTFilterComponent::~VASTFilterComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    VDBG("Destructing VASTFilterComponent");
	this->setLookAndFeel(nullptr);
    //[/Destructor_pre]

    label40 = nullptr;
    label42 = nullptr;
    label75 = nullptr;
    label26 = nullptr;
    label28 = nullptr;
    label44 = nullptr;
    m_fFilterCutoff_Filter2 = nullptr;
    m_fFilterReso_Filter1 = nullptr;
    m_fVCFEnvMod_Filter2 = nullptr;
    m_fFilterCutoff_Filter1 = nullptr;
    m_fFilterDrive_Filter2 = nullptr;
    m_fFilterReso_Filter2 = nullptr;
    m_fFilterDrive_Filter1 = nullptr;
    label77 = nullptr;
    label78 = nullptr;
    label81 = nullptr;
    m_fFilterCutoff_Filter3 = nullptr;
    m_fFilterDrive_Filter3 = nullptr;
    m_fFilterReso_Filter3 = nullptr;
    m_fFilterDryWet_Filter1 = nullptr;
    m_fFilterDryWet_Filter3 = nullptr;
    label97 = nullptr;
    label112 = nullptr;
    label113 = nullptr;
    m_fFilterDryWet_Filter2 = nullptr;
    m_fVCFEnvMod_Filter1 = nullptr;
    m_fVCFEnvMod_Filter3 = nullptr;
    m_bOnOff_Filter1 = nullptr;
    m_bOnOff_Filter2 = nullptr;
    m_bOnOff_Filter3 = nullptr;
    c_filterDisplay = nullptr;
    m_uVCFEnv_Filter1 = nullptr;
    label13 = nullptr;
    m_uVCFEnv_Filter2 = nullptr;
    m_uVCFEnv_Filter3 = nullptr;
    m_uFilterRouting_Filter1 = nullptr;
    label6 = nullptr;
    m_uFilterRouting_Filter2 = nullptr;
    m_uFilterRouting_Filter3 = nullptr;
    m_uFilterType_Filter1 = nullptr;
    m_uFilterType_Filter2 = nullptr;
    m_uFilterType_Filter3 = nullptr;
    label2 = nullptr;
    m_uFilterRouting2_Filter1 = nullptr;
    label3 = nullptr;
    m_uFilterRouting2_Filter2 = nullptr;
    m_uFilterRouting2_Filter3 = nullptr;
    label4 = nullptr;
    label5 = nullptr;
    m_fFilterScale_Filter1 = nullptr;
    m_fFilterScale_Filter2 = nullptr;
    label7 = nullptr;
    m_fFilterScale_Filter3 = nullptr;
    label8 = nullptr;
    label9 = nullptr;
    m_fFilterGain_Filter1 = nullptr;
    m_fFilterGain_Filter2 = nullptr;
    label10 = nullptr;
    m_fFilterGain_Filter3 = nullptr;
    label11 = nullptr;
    c_iconMaximizeEditor = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void VASTFilterComponent::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff323e44));

    {
        int x = 0, y = 0, width = getWidth() - 0, height = getHeight() - 0;
        juce::Colour fillColour1 = juce::Colour (0xbd142b47), fillColour2 = juce::Colour (0x43121314);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        fillColour1 = myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colFilterComponentBackgroundGradientFrom);
        fillColour2 = myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colFilterComponentBackgroundGradientTo);
        //[/UserPaintCustomArguments]
        g.setGradientFill (juce::ColourGradient (fillColour1,
                                             static_cast<float> (proportionOfWidth (0.7544f)) - 0.0f + x,
                                             static_cast<float> (proportionOfHeight (0.0574f)) - 0.0f + y,
                                             fillColour2,
                                             static_cast<float> (proportionOfWidth (1.0133f)) - 0.0f + x,
                                             static_cast<float> (proportionOfHeight (0.5077f)) - 0.0f + y,
                                             false));
        g.fillRect (x, y, width, height);
    }

    {
        int x = proportionOfWidth (0.2400f), y = proportionOfHeight (0.1235f), width = proportionOfWidth (0.3500f), height = proportionOfHeight (0.0174f);
        juce::Colour fillColour1 = juce::Colour (0xff3a4145), fillColour2 = juce::Colour (0xff33393c);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        fillColour1 = myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscillatorComponentHeaderGradientFrom);
        fillColour2 = myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscillatorComponentHeaderGradientTo);
        //[/UserPaintCustomArguments]
        g.setGradientFill (juce::ColourGradient (fillColour1,
                                             static_cast<float> (proportionOfWidth (0.3300f)) - static_cast<float> (proportionOfWidth (0.2400f)) + x,
                                             static_cast<float> (proportionOfHeight (0.1113f)) - static_cast<float> (proportionOfHeight (0.1235f)) + y,
                                             fillColour2,
                                             static_cast<float> (proportionOfWidth (0.6300f)) - static_cast<float> (proportionOfWidth (0.2400f)) + x,
                                             static_cast<float> (proportionOfHeight (0.1530f)) - static_cast<float> (proportionOfHeight (0.1235f)) + y,
                                             false));
        g.fillRect (x, y, width, height);
    }

    {
        int x = proportionOfWidth (0.2400f), y = proportionOfHeight (0.2452f), width = proportionOfWidth (0.3500f), height = proportionOfHeight (0.0174f);
        juce::Colour fillColour1 = juce::Colour (0xff3a4145), fillColour2 = juce::Colour (0xff33393c);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        fillColour1 = myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscillatorComponentHeaderGradientFrom);
        fillColour2 = myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscillatorComponentHeaderGradientTo);
        //[/UserPaintCustomArguments]
        g.setGradientFill (juce::ColourGradient (fillColour1,
                                             static_cast<float> (proportionOfWidth (0.3400f)) - static_cast<float> (proportionOfWidth (0.2400f)) + x,
                                             static_cast<float> (proportionOfHeight (0.2226f)) - static_cast<float> (proportionOfHeight (0.2452f)) + y,
                                             fillColour2,
                                             static_cast<float> (proportionOfWidth (0.6300f)) - static_cast<float> (proportionOfWidth (0.2400f)) + x,
                                             static_cast<float> (proportionOfHeight (0.2783f)) - static_cast<float> (proportionOfHeight (0.2452f)) + y,
                                             false));
        g.fillRect (x, y, width, height);
    }

    {
        int x = proportionOfWidth (0.2400f), y = proportionOfHeight (0.3635f), width = proportionOfWidth (0.3500f), height = proportionOfHeight (0.0174f);
        juce::Colour fillColour1 = juce::Colour (0xff3a4145), fillColour2 = juce::Colour (0xff33393c);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        fillColour1 = myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscillatorComponentHeaderGradientFrom);
        fillColour2 = myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscillatorComponentHeaderGradientTo);
        //[/UserPaintCustomArguments]
        g.setGradientFill (juce::ColourGradient (fillColour1,
                                             static_cast<float> (proportionOfWidth (0.3400f)) - static_cast<float> (proportionOfWidth (0.2400f)) + x,
                                             static_cast<float> (proportionOfHeight (0.3478f)) - static_cast<float> (proportionOfHeight (0.3635f)) + y,
                                             fillColour2,
                                             static_cast<float> (proportionOfWidth (0.6200f)) - static_cast<float> (proportionOfWidth (0.2400f)) + x,
                                             static_cast<float> (proportionOfHeight (0.3896f)) - static_cast<float> (proportionOfHeight (0.3635f)) + y,
                                             false));
        g.fillRect (x, y, width, height);
    }

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void VASTFilterComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    label40->setBounds (proportionOfWidth (0.3513f), proportionOfHeight (0.2435f), proportionOfWidth (0.0663f), proportionOfHeight (0.0209f));
    label42->setBounds (proportionOfWidth (0.4188f), proportionOfHeight (0.2435f), proportionOfWidth (0.0663f), proportionOfHeight (0.0209f));
    label75->setBounds (proportionOfWidth (0.4188f), proportionOfHeight (0.1200f), proportionOfWidth (0.0663f), proportionOfHeight (0.0209f));
    label26->setBounds (proportionOfWidth (0.3513f), proportionOfHeight (0.1200f), proportionOfWidth (0.0663f), proportionOfHeight (0.0209f));
    label28->setBounds (proportionOfWidth (0.2863f), proportionOfHeight (0.1200f), proportionOfWidth (0.0663f), proportionOfHeight (0.0209f));
    label44->setBounds (proportionOfWidth (0.2863f), proportionOfHeight (0.2435f), proportionOfWidth (0.0663f), proportionOfHeight (0.0209f));
    m_fFilterCutoff_Filter2->setBounds (proportionOfWidth (0.3550f), proportionOfHeight (0.1565f), proportionOfWidth (0.0625f), proportionOfHeight (0.0870f));
    m_fFilterReso_Filter1->setBounds (proportionOfWidth (0.4263f), proportionOfHeight (0.0487f), proportionOfWidth (0.0500f), proportionOfHeight (0.0696f));
    m_fVCFEnvMod_Filter2->setBounds (proportionOfWidth (0.7225f), proportionOfHeight (0.1739f), proportionOfWidth (0.0500f), proportionOfHeight (0.0696f));
    m_fFilterCutoff_Filter1->setBounds (proportionOfWidth (0.3550f), proportionOfHeight (0.0330f), proportionOfWidth (0.0625f), proportionOfHeight (0.0870f));
    m_fFilterDrive_Filter2->setBounds (proportionOfWidth (0.2938f), proportionOfHeight (0.1739f), proportionOfWidth (0.0500f), proportionOfHeight (0.0696f));
    m_fFilterReso_Filter2->setBounds (proportionOfWidth (0.4263f), proportionOfHeight (0.1739f), proportionOfWidth (0.0500f), proportionOfHeight (0.0696f));
    m_fFilterDrive_Filter1->setBounds (proportionOfWidth (0.2938f), proportionOfHeight (0.0487f), proportionOfWidth (0.0500f), proportionOfHeight (0.0696f));
    label77->setBounds (proportionOfWidth (0.3513f), proportionOfHeight (0.3617f), proportionOfWidth (0.0663f), proportionOfHeight (0.0209f));
    label78->setBounds (proportionOfWidth (0.4188f), proportionOfHeight (0.3617f), proportionOfWidth (0.0663f), proportionOfHeight (0.0209f));
    label81->setBounds (proportionOfWidth (0.2863f), proportionOfHeight (0.3617f), proportionOfWidth (0.0663f), proportionOfHeight (0.0209f));
    m_fFilterCutoff_Filter3->setBounds (proportionOfWidth (0.3550f), proportionOfHeight (0.2730f), proportionOfWidth (0.0625f), proportionOfHeight (0.0870f));
    m_fFilterDrive_Filter3->setBounds (proportionOfWidth (0.2938f), proportionOfHeight (0.2852f), proportionOfWidth (0.0500f), proportionOfHeight (0.0696f));
    m_fFilterReso_Filter3->setBounds (proportionOfWidth (0.4263f), proportionOfHeight (0.2852f), proportionOfWidth (0.0500f), proportionOfHeight (0.0696f));
    m_fFilterDryWet_Filter1->setBounds (proportionOfWidth (0.2375f), proportionOfHeight (0.0487f), proportionOfWidth (0.0500f), proportionOfHeight (0.0696f));
    m_fFilterDryWet_Filter3->setBounds (proportionOfWidth (0.2375f), proportionOfHeight (0.2852f), proportionOfWidth (0.0500f), proportionOfHeight (0.0696f));
    label97->setBounds (proportionOfWidth (0.2288f), proportionOfHeight (0.1200f), proportionOfWidth (0.0663f), proportionOfHeight (0.0209f));
    label112->setBounds (proportionOfWidth (0.2288f), proportionOfHeight (0.2435f), proportionOfWidth (0.0663f), proportionOfHeight (0.0209f));
    label113->setBounds (proportionOfWidth (0.2288f), proportionOfHeight (0.3617f), proportionOfWidth (0.0663f), proportionOfHeight (0.0209f));
    m_fFilterDryWet_Filter2->setBounds (proportionOfWidth (0.2375f), proportionOfHeight (0.1739f), proportionOfWidth (0.0500f), proportionOfHeight (0.0696f));
    m_fVCFEnvMod_Filter1->setBounds (proportionOfWidth (0.7212f), proportionOfHeight (0.0487f), proportionOfWidth (0.0500f), proportionOfHeight (0.0696f));
    m_fVCFEnvMod_Filter3->setBounds (proportionOfWidth (0.7225f), proportionOfHeight (0.2852f), proportionOfWidth (0.0500f), proportionOfHeight (0.0696f));
    m_bOnOff_Filter1->setBounds (proportionOfWidth (0.0100f), proportionOfHeight (0.0713f), proportionOfWidth (0.0225f), proportionOfHeight (0.0313f));
    m_bOnOff_Filter2->setBounds (proportionOfWidth (0.0100f), proportionOfHeight (0.1861f), proportionOfWidth (0.0225f), proportionOfHeight (0.0313f));
    m_bOnOff_Filter3->setBounds (proportionOfWidth (0.0100f), proportionOfHeight (0.3009f), proportionOfWidth (0.0225f), proportionOfHeight (0.0313f));
    c_filterDisplay->setBounds (proportionOfWidth (0.9900f) - proportionOfWidth (0.9800f), proportionOfHeight (0.4035f), proportionOfWidth (0.9800f), proportionOfHeight (0.5844f));
    m_uVCFEnv_Filter1->setBounds (proportionOfWidth (0.6125f), proportionOfHeight (0.0730f), proportionOfWidth (0.0938f), proportionOfHeight (0.0244f));
    label13->setBounds (proportionOfWidth (0.6088f), proportionOfHeight (0.0139f), proportionOfWidth (0.0950f), proportionOfHeight (0.0209f));
    m_uVCFEnv_Filter2->setBounds (proportionOfWidth (0.6125f), proportionOfHeight (0.1878f), proportionOfWidth (0.0938f), proportionOfHeight (0.0244f));
    m_uVCFEnv_Filter3->setBounds (proportionOfWidth (0.6125f), proportionOfHeight (0.3026f), proportionOfWidth (0.0938f), proportionOfHeight (0.0244f));
    m_uFilterRouting_Filter1->setBounds (proportionOfWidth (0.7875f), proportionOfHeight (0.0730f), proportionOfWidth (0.0938f), proportionOfHeight (0.0244f));
    label6->setBounds (proportionOfWidth (0.7863f), proportionOfHeight (0.0139f), proportionOfWidth (0.0950f), proportionOfHeight (0.0209f));
    m_uFilterRouting_Filter2->setBounds (proportionOfWidth (0.7875f), proportionOfHeight (0.1878f), proportionOfWidth (0.0938f), proportionOfHeight (0.0244f));
    m_uFilterRouting_Filter3->setBounds (proportionOfWidth (0.7875f), proportionOfHeight (0.3026f), proportionOfWidth (0.0938f), proportionOfHeight (0.0244f));
    m_uFilterType_Filter1->setBounds (proportionOfWidth (0.0512f), proportionOfHeight (0.0730f), proportionOfWidth (0.1638f), proportionOfHeight (0.0278f));
    m_uFilterType_Filter2->setBounds (proportionOfWidth (0.0512f), proportionOfHeight (0.1878f), proportionOfWidth (0.1638f), proportionOfHeight (0.0278f));
    m_uFilterType_Filter3->setBounds (proportionOfWidth (0.0512f), proportionOfHeight (0.3026f), proportionOfWidth (0.1638f), proportionOfHeight (0.0278f));
    label2->setBounds (proportionOfWidth (0.6975f), proportionOfHeight (0.0139f), proportionOfWidth (0.0950f), proportionOfHeight (0.0209f));
    m_uFilterRouting2_Filter1->setBounds (proportionOfWidth (0.8950f), proportionOfHeight (0.0730f), proportionOfWidth (0.0938f), proportionOfHeight (0.0244f));
    label3->setBounds (proportionOfWidth (0.8950f), proportionOfHeight (0.0139f), proportionOfWidth (0.0950f), proportionOfHeight (0.0209f));
    m_uFilterRouting2_Filter2->setBounds (proportionOfWidth (0.8950f), proportionOfHeight (0.1878f), proportionOfWidth (0.0938f), proportionOfHeight (0.0244f));
    m_uFilterRouting2_Filter3->setBounds (proportionOfWidth (0.8950f), proportionOfHeight (0.3026f), proportionOfWidth (0.0938f), proportionOfHeight (0.0244f));
    label4->setBounds (proportionOfWidth (0.4763f), proportionOfHeight (0.2435f), proportionOfWidth (0.0663f), proportionOfHeight (0.0209f));
    label5->setBounds (proportionOfWidth (0.4763f), proportionOfHeight (0.1200f), proportionOfWidth (0.0663f), proportionOfHeight (0.0209f));
    m_fFilterScale_Filter1->setBounds (proportionOfWidth (0.4825f), proportionOfHeight (0.0487f), proportionOfWidth (0.0500f), proportionOfHeight (0.0696f));
    m_fFilterScale_Filter2->setBounds (proportionOfWidth (0.4825f), proportionOfHeight (0.1739f), proportionOfWidth (0.0500f), proportionOfHeight (0.0696f));
    label7->setBounds (proportionOfWidth (0.4763f), proportionOfHeight (0.3617f), proportionOfWidth (0.0663f), proportionOfHeight (0.0209f));
    m_fFilterScale_Filter3->setBounds (proportionOfWidth (0.4825f), proportionOfHeight (0.2852f), proportionOfWidth (0.0500f), proportionOfHeight (0.0696f));
    label8->setBounds (proportionOfWidth (0.5275f), proportionOfHeight (0.2435f), proportionOfWidth (0.0663f), proportionOfHeight (0.0209f));
    label9->setBounds (proportionOfWidth (0.5275f), proportionOfHeight (0.1200f), proportionOfWidth (0.0663f), proportionOfHeight (0.0209f));
    m_fFilterGain_Filter1->setBounds (proportionOfWidth (0.5387f), proportionOfHeight (0.0487f), proportionOfWidth (0.0500f), proportionOfHeight (0.0696f));
    m_fFilterGain_Filter2->setBounds (proportionOfWidth (0.5387f), proportionOfHeight (0.1739f), proportionOfWidth (0.0500f), proportionOfHeight (0.0696f));
    label10->setBounds (proportionOfWidth (0.5275f), proportionOfHeight (0.3617f), proportionOfWidth (0.0663f), proportionOfHeight (0.0209f));
    m_fFilterGain_Filter3->setBounds (proportionOfWidth (0.5387f), proportionOfHeight (0.2852f), proportionOfWidth (0.0500f), proportionOfHeight (0.0696f));
    label11->setBounds (proportionOfWidth (0.0800f), proportionOfHeight (0.0139f), proportionOfWidth (0.0950f), proportionOfHeight (0.0209f));
    c_iconMaximizeEditor->setBounds (getWidth() - 1 - proportionOfWidth (0.0150f), 1, proportionOfWidth (0.0150f), proportionOfHeight (0.0209f));
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void VASTFilterComponent::sliderValueChanged (juce::Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
	//c_filterDisplay->updateContent(true);
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == m_fFilterCutoff_Filter2.get())
    {
        //[UserSliderCode_m_fFilterCutoff_Filter2] -- add your slider handling code here..
        //[/UserSliderCode_m_fFilterCutoff_Filter2]
    }
    else if (sliderThatWasMoved == m_fFilterReso_Filter1.get())
    {
        //[UserSliderCode_m_fFilterReso_Filter1] -- add your slider handling code here..
        //[/UserSliderCode_m_fFilterReso_Filter1]
    }
    else if (sliderThatWasMoved == m_fVCFEnvMod_Filter2.get())
    {
        //[UserSliderCode_m_fVCFEnvMod_Filter2] -- add your slider handling code here..
        //[/UserSliderCode_m_fVCFEnvMod_Filter2]
    }
    else if (sliderThatWasMoved == m_fFilterCutoff_Filter1.get())
    {
        //[UserSliderCode_m_fFilterCutoff_Filter1] -- add your slider handling code here..
        //[/UserSliderCode_m_fFilterCutoff_Filter1]
    }
    else if (sliderThatWasMoved == m_fFilterDrive_Filter2.get())
    {
        //[UserSliderCode_m_fFilterDrive_Filter2] -- add your slider handling code here..
        //[/UserSliderCode_m_fFilterDrive_Filter2]
    }
    else if (sliderThatWasMoved == m_fFilterReso_Filter2.get())
    {
        //[UserSliderCode_m_fFilterReso_Filter2] -- add your slider handling code here..
        //[/UserSliderCode_m_fFilterReso_Filter2]
    }
    else if (sliderThatWasMoved == m_fFilterDrive_Filter1.get())
    {
        //[UserSliderCode_m_fFilterDrive_Filter1] -- add your slider handling code here..
        //[/UserSliderCode_m_fFilterDrive_Filter1]
    }
    else if (sliderThatWasMoved == m_fFilterCutoff_Filter3.get())
    {
        //[UserSliderCode_m_fFilterCutoff_Filter3] -- add your slider handling code here..
        //[/UserSliderCode_m_fFilterCutoff_Filter3]
    }
    else if (sliderThatWasMoved == m_fFilterDrive_Filter3.get())
    {
        //[UserSliderCode_m_fFilterDrive_Filter3] -- add your slider handling code here..
        //[/UserSliderCode_m_fFilterDrive_Filter3]
    }
    else if (sliderThatWasMoved == m_fFilterReso_Filter3.get())
    {
        //[UserSliderCode_m_fFilterReso_Filter3] -- add your slider handling code here..
        //[/UserSliderCode_m_fFilterReso_Filter3]
    }
    else if (sliderThatWasMoved == m_fFilterDryWet_Filter1.get())
    {
        //[UserSliderCode_m_fFilterDryWet_Filter1] -- add your slider handling code here..
        //[/UserSliderCode_m_fFilterDryWet_Filter1]
    }
    else if (sliderThatWasMoved == m_fFilterDryWet_Filter3.get())
    {
        //[UserSliderCode_m_fFilterDryWet_Filter3] -- add your slider handling code here..
        //[/UserSliderCode_m_fFilterDryWet_Filter3]
    }
    else if (sliderThatWasMoved == m_fFilterDryWet_Filter2.get())
    {
        //[UserSliderCode_m_fFilterDryWet_Filter2] -- add your slider handling code here..
        //[/UserSliderCode_m_fFilterDryWet_Filter2]
    }
    else if (sliderThatWasMoved == m_fVCFEnvMod_Filter1.get())
    {
        //[UserSliderCode_m_fVCFEnvMod_Filter1] -- add your slider handling code here..
        //[/UserSliderCode_m_fVCFEnvMod_Filter1]
    }
    else if (sliderThatWasMoved == m_fVCFEnvMod_Filter3.get())
    {
        //[UserSliderCode_m_fVCFEnvMod_Filter3] -- add your slider handling code here..
        //[/UserSliderCode_m_fVCFEnvMod_Filter3]
    }
    else if (sliderThatWasMoved == m_fFilterScale_Filter1.get())
    {
        //[UserSliderCode_m_fFilterScale_Filter1] -- add your slider handling code here..
        //[/UserSliderCode_m_fFilterScale_Filter1]
    }
    else if (sliderThatWasMoved == m_fFilterScale_Filter2.get())
    {
        //[UserSliderCode_m_fFilterScale_Filter2] -- add your slider handling code here..
        //[/UserSliderCode_m_fFilterScale_Filter2]
    }
    else if (sliderThatWasMoved == m_fFilterScale_Filter3.get())
    {
        //[UserSliderCode_m_fFilterScale_Filter3] -- add your slider handling code here..
        //[/UserSliderCode_m_fFilterScale_Filter3]
    }
    else if (sliderThatWasMoved == m_fFilterGain_Filter1.get())
    {
        //[UserSliderCode_m_fFilterGain_Filter1] -- add your slider handling code here..
        //[/UserSliderCode_m_fFilterGain_Filter1]
    }
    else if (sliderThatWasMoved == m_fFilterGain_Filter2.get())
    {
        //[UserSliderCode_m_fFilterGain_Filter2] -- add your slider handling code here..
        //[/UserSliderCode_m_fFilterGain_Filter2]
    }
    else if (sliderThatWasMoved == m_fFilterGain_Filter3.get())
    {
        //[UserSliderCode_m_fFilterGain_Filter3] -- add your slider handling code here..
        //[/UserSliderCode_m_fFilterGain_Filter3]
    }

    //[UsersliderValueChanged_Post]
	c_filterDisplay->doUpdates(false);
    //[/UsersliderValueChanged_Post]
}

void VASTFilterComponent::buttonClicked (juce::Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
	if (buttonThatWasClicked == c_iconMaximizeEditor.get()) {
		if (!myEditor->vaporizerComponent->isTabConcertinaMaximized()) {
			myEditor->vaporizerComponent->maximizeTabConcertina();
		}
		else {
			myEditor->vaporizerComponent->defaultConcertina();
		}
		return;
	}
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == m_bOnOff_Filter1.get())
    {
        //[UserButtonCode_m_bOnOff_Filter1] -- add your button handler code here..
        //[/UserButtonCode_m_bOnOff_Filter1]
    }
    else if (buttonThatWasClicked == m_bOnOff_Filter2.get())
    {
        //[UserButtonCode_m_bOnOff_Filter2] -- add your button handler code here..
        //[/UserButtonCode_m_bOnOff_Filter2]
    }
    else if (buttonThatWasClicked == m_bOnOff_Filter3.get())
    {
        //[UserButtonCode_m_bOnOff_Filter3] -- add your button handler code here..
        //[/UserButtonCode_m_bOnOff_Filter3]
    }

    //[UserbuttonClicked_Post]
	c_filterDisplay->doUpdates(false);
    //[/UserbuttonClicked_Post]
}

void VASTFilterComponent::comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == m_uVCFEnv_Filter1.get())
    {
        //[UserComboBoxCode_m_uVCFEnv_Filter1] -- add your combo box handling code here..
        //[/UserComboBoxCode_m_uVCFEnv_Filter1]
    }
    else if (comboBoxThatHasChanged == m_uVCFEnv_Filter2.get())
    {
        //[UserComboBoxCode_m_uVCFEnv_Filter2] -- add your combo box handling code here..
        //[/UserComboBoxCode_m_uVCFEnv_Filter2]
    }
    else if (comboBoxThatHasChanged == m_uVCFEnv_Filter3.get())
    {
        //[UserComboBoxCode_m_uVCFEnv_Filter3] -- add your combo box handling code here..
        //[/UserComboBoxCode_m_uVCFEnv_Filter3]
    }
    else if (comboBoxThatHasChanged == m_uFilterRouting_Filter1.get())
    {
        //[UserComboBoxCode_m_uFilterRouting_Filter1] -- add your combo box handling code here..
        //[/UserComboBoxCode_m_uFilterRouting_Filter1]
    }
    else if (comboBoxThatHasChanged == m_uFilterRouting_Filter2.get())
    {
        //[UserComboBoxCode_m_uFilterRouting_Filter2] -- add your combo box handling code here..
        //[/UserComboBoxCode_m_uFilterRouting_Filter2]
    }
    else if (comboBoxThatHasChanged == m_uFilterRouting_Filter3.get())
    {
        //[UserComboBoxCode_m_uFilterRouting_Filter3] -- add your combo box handling code here..
        //[/UserComboBoxCode_m_uFilterRouting_Filter3]
    }
    else if (comboBoxThatHasChanged == m_uFilterType_Filter1.get())
    {
        //[UserComboBoxCode_m_uFilterType_Filter1] -- add your combo box handling code here..
        //[/UserComboBoxCode_m_uFilterType_Filter1]
    }
    else if (comboBoxThatHasChanged == m_uFilterType_Filter2.get())
    {
        //[UserComboBoxCode_m_uFilterType_Filter2] -- add your combo box handling code here..
        //[/UserComboBoxCode_m_uFilterType_Filter2]
    }
    else if (comboBoxThatHasChanged == m_uFilterType_Filter3.get())
    {
        //[UserComboBoxCode_m_uFilterType_Filter3] -- add your combo box handling code here..
        //[/UserComboBoxCode_m_uFilterType_Filter3]
    }
    else if (comboBoxThatHasChanged == m_uFilterRouting2_Filter1.get())
    {
        //[UserComboBoxCode_m_uFilterRouting2_Filter1] -- add your combo box handling code here..
        //[/UserComboBoxCode_m_uFilterRouting2_Filter1]
    }
    else if (comboBoxThatHasChanged == m_uFilterRouting2_Filter2.get())
    {
        //[UserComboBoxCode_m_uFilterRouting2_Filter2] -- add your combo box handling code here..
        //[/UserComboBoxCode_m_uFilterRouting2_Filter2]
    }
    else if (comboBoxThatHasChanged == m_uFilterRouting2_Filter3.get())
    {
        //[UserComboBoxCode_m_uFilterRouting2_Filter3] -- add your combo box handling code here..
        //[/UserComboBoxCode_m_uFilterRouting2_Filter3]
    }

    //[UsercomboBoxChanged_Post]
	c_filterDisplay->doUpdates(false);
    //[/UsercomboBoxChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void VASTFilterComponent::startAutoUpdate() {
    if (isShowing()) grabKeyboardFocus();
	//c_filterDisplay->startAutoUpdate();
}
void VASTFilterComponent::stopAutoUpdate() {
	//c_filterDisplay->stopAutoUpdate();
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="VASTFilterComponent" componentName=""
                 parentClasses="public Component" constructorParams="AudioProcessorEditor *editor, AudioProcessor* processor"
                 variableInitialisers="myEditor((VASTAudioProcessorEditor*)editor), myProcessor((VASTAudioProcessor*)processor)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="800" initialHeight="575">
  <BACKGROUND backgroundColour="ff323e44">
    <RECT pos="0 0 0M 0M" fill="linear: 75.444% 5.735%, 101.333% 50.769%, 0=bd142b47, 1=43121314"
          hasStroke="0"/>
    <RECT pos="24% 12.348% 35% 1.739%" fill="linear: 33% 11.13%, 63% 15.304%, 0=ff3a4145, 1=ff33393c"
          hasStroke="0"/>
    <RECT pos="24% 24.522% 35% 1.739%" fill="linear: 34% 22.261%, 63% 27.826%, 0=ff3a4145, 1=ff33393c"
          hasStroke="0"/>
    <RECT pos="24% 36.348% 35% 1.739%" fill="linear: 34% 34.783%, 62% 38.957%, 0=ff3a4145, 1=ff33393c"
          hasStroke="0"/>
  </BACKGROUND>
  <LABEL name="new label" id="b71576ce53f390aa" memberName="label40" virtualName=""
         explicitFocusOrder="0" pos="35.125% 24.348% 6.625% 2.087%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="CUTOFF" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="9.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <LABEL name="new label" id="73aa870b9c7e6178" memberName="label42" virtualName=""
         explicitFocusOrder="0" pos="41.875% 24.348% 6.625% 2.087%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="RESO" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="9.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <LABEL name="new label" id="ded6e6f5b3f5809c" memberName="label75" virtualName=""
         explicitFocusOrder="0" pos="41.875% 12% 6.625% 2.087%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="RESO" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="9.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <LABEL name="new label" id="1734b41d30cd089c" memberName="label26" virtualName=""
         explicitFocusOrder="0" pos="35.125% 12% 6.625% 2.087%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="CUTOFF" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="9.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <LABEL name="new label" id="3e7eb582f1ad541e" memberName="label28" virtualName=""
         explicitFocusOrder="0" pos="28.625% 12% 6.625% 2.087%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="DRIVE" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="9.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <LABEL name="new label" id="319dd083fb9c4b0f" memberName="label44" virtualName=""
         explicitFocusOrder="0" pos="28.625% 24.348% 6.625% 2.087%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="DRIVE" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="9.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <SLIDER name="m_fFilterCutoff_Filter2" id="bbe6720be4bc975d" memberName="m_fFilterCutoff_Filter2"
          virtualName="VASTParameterSlider" explicitFocusOrder="16" pos="35.5% 15.652% 6.25% 8.696%"
          tooltip="Filter 2 cutoff (hz)" textboxtext="ffc9c9c9" textboxbkgd="ffffff"
          textboxoutline="808080" min="20.0" max="22000.0" int="0.001"
          style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="30" textBoxHeight="14" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="m_fFilterReso_Filter1" id="b06ad0025b6f7bdb" memberName="m_fFilterReso_Filter1"
          virtualName="VASTParameterSlider" explicitFocusOrder="5" pos="42.625% 4.87% 5% 6.957%"
          tooltip="Filter 1 resonance" textboxtext="ffc9c9c9" textboxbkgd="ffffff"
          textboxoutline="808080" min="0.0" max="100.0" int="0.001" style="RotaryVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="30"
          textBoxHeight="14" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="m_fVCFEnvMod_Filter2" id="1d39004fb5e5920" memberName="m_fVCFEnvMod_Filter2"
          virtualName="VASTParameterSlider" explicitFocusOrder="21" pos="72.25% 17.391% 5% 6.957%"
          tooltip="VCF filter 2 envelope modulation" textboxtext="ffc9c9c9"
          textboxbkgd="ffffff" textboxoutline="808080" min="-100.0" max="100.0"
          int="0.01" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="30" textBoxHeight="14" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="m_fFilterCutoff_Filter1" id="b7dc88920025ece2" memberName="m_fFilterCutoff_Filter1"
          virtualName="VASTParameterSlider" explicitFocusOrder="4" pos="35.5% 3.304% 6.25% 8.696%"
          tooltip="Filter 1 cutoff (hz)" textboxtext="ffc9c9c9" textboxbkgd="ffffff"
          textboxoutline="808080" min="20.0" max="22000.0" int="0.001"
          style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="30" textBoxHeight="14" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="m_fFilterDrive_Filter2" id="e0d6763ad9c21a9b" memberName="m_fFilterDrive_Filter2"
          virtualName="VASTParameterSlider" explicitFocusOrder="15" pos="29.375% 17.391% 5% 6.957%"
          tooltip="Filter 2 drive in dB - pre gain (0 - full, &gt;0 overdrive)"
          textboxtext="ffc9c9c9" textboxbkgd="ffffff" textboxoutline="808080"
          min="-20.0" max="20.0" int="0.001" style="RotaryVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="30"
          textBoxHeight="14" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="m_fFilterReso_Filter2" id="82e6ff004a10907b" memberName="m_fFilterReso_Filter2"
          virtualName="VASTParameterSlider" explicitFocusOrder="17" pos="42.625% 17.391% 5% 6.957%"
          tooltip="Filter 2 resonance" textboxtext="ffc9c9c9" textboxbkgd="ffffff"
          textboxoutline="808080" min="0.0" max="100.0" int="0.001" style="RotaryVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="30"
          textBoxHeight="14" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="m_fFilterDrive_Filter1" id="103ed31e7b5dcef4" memberName="m_fFilterDrive_Filter1"
          virtualName="VASTParameterSlider" explicitFocusOrder="3" pos="29.375% 4.87% 5% 6.957%"
          tooltip="Filter 1 drive in dB - pre gain (0 - full, &gt;0 overdrive)"
          textboxtext="ffc9c9c9" textboxbkgd="ffffff" textboxoutline="808080"
          min="-20.0" max="20.0" int="0.001" style="RotaryVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="30"
          textBoxHeight="14" skewFactor="1.0" needsCallback="1"/>
  <LABEL name="new label" id="216a7ecd53961ed1" memberName="label77" virtualName=""
         explicitFocusOrder="0" pos="35.125% 36.174% 6.625% 2.087%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="CUTOFF" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="9.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <LABEL name="new label" id="c56aae83497ad028" memberName="label78" virtualName=""
         explicitFocusOrder="0" pos="41.875% 36.174% 6.625% 2.087%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="RESO" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="9.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <LABEL name="new label" id="fc017cbbc8c3b0ce" memberName="label81" virtualName=""
         explicitFocusOrder="0" pos="28.625% 36.174% 6.625% 2.087%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="DRIVE" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="9.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <SLIDER name="m_fFilterCutoff_Filter3" id="3d2b858609639e7e" memberName="m_fFilterCutoff_Filter3"
          virtualName="VASTParameterSlider" explicitFocusOrder="28" pos="35.5% 27.304% 6.25% 8.696%"
          tooltip="Filter 3 cutoff (hz)" textboxtext="ffc9c9c9" textboxbkgd="ffffff"
          textboxoutline="808080" min="20.0" max="22000.0" int="0.001"
          style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="30" textBoxHeight="14" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="m_fFilterDrive_Filter3" id="8957931312ec83ed" memberName="m_fFilterDrive_Filter3"
          virtualName="VASTParameterSlider" explicitFocusOrder="27" pos="29.375% 28.522% 5% 6.957%"
          tooltip="Filter 3 drive in dB - pre gain (0 - full, &gt;0 overdrive)"
          textboxtext="ffc9c9c9" textboxbkgd="ffffff" textboxoutline="808080"
          min="-20.0" max="20.0" int="0.001" style="RotaryVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="30"
          textBoxHeight="14" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="m_fFilterReso_Filter3" id="dd257729bda5d087" memberName="m_fFilterReso_Filter3"
          virtualName="VASTParameterSlider" explicitFocusOrder="29" pos="42.625% 28.522% 5% 6.957%"
          tooltip="Filter 3 resonance" textboxtext="ffc9c9c9" textboxbkgd="ffffff"
          textboxoutline="808080" min="0.0" max="100.0" int="0.001" style="RotaryVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="30"
          textBoxHeight="14" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="m_fFilterDryWet_Filter1" id="c180a5ef984b0b3b" memberName="m_fFilterDryWet_Filter1"
          virtualName="VASTParameterSlider" explicitFocusOrder="2" pos="23.75% 4.87% 5% 6.957%"
          tooltip="Filter 1 mix in (dry - wet)" textboxtext="ffc9c9c9"
          textboxbkgd="ffffff" textboxoutline="808080" min="0.0" max="100.0"
          int="0.01" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="30" textBoxHeight="14" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="m_fFilterDryWet_Filter3" id="52c210e1683bd103" memberName="m_fFilterDryWet_Filter3"
          virtualName="VASTParameterSlider" explicitFocusOrder="26" pos="23.75% 28.522% 5% 6.957%"
          tooltip="Filter 3 mix in (dry - wet)" textboxtext="ffc9c9c9"
          textboxbkgd="ffffff" textboxoutline="808080" min="0.0" max="100.0"
          int="0.01" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="30" textBoxHeight="14" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="new label" id="68d8ae7c05057f0a" memberName="label97" virtualName=""
         explicitFocusOrder="0" pos="22.875% 12% 6.625% 2.087%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="WET" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="9.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <LABEL name="new label" id="adf43d633c311145" memberName="label112"
         virtualName="" explicitFocusOrder="0" pos="22.875% 24.348% 6.625% 2.087%"
         textCol="ffe2e2e2" edTextCol="ff000000" edBkgCol="0" labelText="WET"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="9.0" kerning="0.0" bold="0"
         italic="0" justification="36"/>
  <LABEL name="new label" id="b56a280fb9ba482e" memberName="label113"
         virtualName="" explicitFocusOrder="0" pos="22.875% 36.174% 6.625% 2.087%"
         textCol="ffe2e2e2" edTextCol="ff000000" edBkgCol="0" labelText="WET"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="9.0" kerning="0.0" bold="0"
         italic="0" justification="36"/>
  <SLIDER name="m_fFilterDryWet_Filter2" id="808378557d32a184" memberName="m_fFilterDryWet_Filter2"
          virtualName="VASTParameterSlider" explicitFocusOrder="14" pos="23.75% 17.391% 5% 6.957%"
          tooltip="Filter 2 mix in (dry - wet)" textboxtext="ffc9c9c9"
          textboxbkgd="ffffff" textboxoutline="808080" min="0.0" max="100.0"
          int="0.01" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="30" textBoxHeight="14" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="m_fVCFEnvMod_Filter1" id="242701dd3c3a413a" memberName="m_fVCFEnvMod_Filter1"
          virtualName="VASTParameterSlider" explicitFocusOrder="9" pos="72.125% 4.87% 5% 6.957%"
          tooltip="VCF filter 1 envelope modulation" textboxtext="ffc9c9c9"
          textboxbkgd="ffffff" textboxoutline="808080" min="-100.0" max="100.0"
          int="0.01" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="30" textBoxHeight="14" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="m_fVCFEnvMod_Filter3" id="2f3bae407203a822" memberName="m_fVCFEnvMod_Filter3"
          virtualName="VASTParameterSlider" explicitFocusOrder="33" pos="72.25% 28.522% 5% 6.957%"
          tooltip="VCF filter 3 envelope modulation" textboxtext="ffc9c9c9"
          textboxbkgd="ffffff" textboxoutline="808080" min="-100.0" max="100.0"
          int="0.01" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="30" textBoxHeight="14" skewFactor="1.0"
          needsCallback="1"/>
  <TOGGLEBUTTON name="m_bOnOff_Filter1" id="1d94b90683d2fb39" memberName="m_bOnOff_Filter1"
                virtualName="VASTParameterButton" explicitFocusOrder="0" pos="1% 7.13% 2.25% 3.13%"
                tooltip="Switch filter 1 on / off" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="m_bOnOff_Filter2" id="e87f7f2fa6874050" memberName="m_bOnOff_Filter2"
                virtualName="VASTParameterButton" explicitFocusOrder="12" pos="1% 18.609% 2.25% 3.13%"
                tooltip="Switch filter 2 on / off" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="m_bOnOff_Filter3" id="c3c1464bac4e6682" memberName="m_bOnOff_Filter3"
                virtualName="VASTParameterButton" explicitFocusOrder="24" pos="1% 30.087% 2.25% 3.13%"
                tooltip="Switch filter 3 on / off" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <GENERICCOMPONENT name="c_filterDisplay" id="61bd7bec450e402b" memberName="c_filterDisplay"
                    virtualName="" explicitFocusOrder="0" pos="99%r 40.348% 98% 58.435%"
                    class="VASTFilterDisplay" params="myProcessor"/>
  <COMBOBOX name="m_uVCFEnv_Filter1" id="39afd5b0c0d4b799" memberName="m_uVCFEnv_Filter1"
            virtualName="VASTParameterComboBox" explicitFocusOrder="8" pos="61.25% 7.304% 9.375% 2.435%"
            tooltip="Envelope used as LPF envelope" editable="0" layout="33"
            items="" textWhenNonSelected="" textWhenNoItems="---"/>
  <LABEL name="new label" id="656ee062d26c6572" memberName="label13" virtualName=""
         explicitFocusOrder="0" pos="60.875% 1.391% 9.5% 2.087%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="VCFENV" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Syntax"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <COMBOBOX name="m_uVCFEnv_Filter2" id="60a82512c3e48357" memberName="m_uVCFEnv_Filter2"
            virtualName="VASTParameterComboBox" explicitFocusOrder="20" pos="61.25% 18.783% 9.375% 2.435%"
            tooltip="Envelope used as filter 2 envelope" editable="0" layout="33"
            items="" textWhenNonSelected="" textWhenNoItems="---"/>
  <COMBOBOX name="m_uVCFEnv_Filter3" id="5257d8b8055a418c" memberName="m_uVCFEnv_Filter3"
            virtualName="VASTParameterComboBox" explicitFocusOrder="32" pos="61.25% 30.261% 9.375% 2.435%"
            tooltip="Envelope used as filter 3 envelope" editable="0" layout="33"
            items="" textWhenNonSelected="" textWhenNoItems="---"/>
  <COMBOBOX name="m_uFilterRouting_Filter1" id="8b691584e485d074" memberName="m_uFilterRouting_Filter1"
            virtualName="VASTParameterComboBox" explicitFocusOrder="10" pos="78.75% 7.304% 9.375% 2.435%"
            tooltip="Filter 1 routing" editable="0" layout="33" items=""
            textWhenNonSelected="" textWhenNoItems="---"/>
  <LABEL name="new label" id="4bbd2a636a288132" memberName="label6" virtualName=""
         explicitFocusOrder="0" pos="78.625% 1.391% 9.5% 2.087%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="ROUTING 1" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Syntax"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <COMBOBOX name="m_uFilterRouting_Filter2" id="eac340e9f3693b88" memberName="m_uFilterRouting_Filter2"
            virtualName="VASTParameterComboBox" explicitFocusOrder="22" pos="78.75% 18.783% 9.375% 2.435%"
            tooltip="Filter 2 routing" editable="0" layout="33" items=""
            textWhenNonSelected="" textWhenNoItems="---"/>
  <COMBOBOX name="m_uFilterRouting_Filter3" id="951394a8a590021c" memberName="m_uFilterRouting_Filter3"
            virtualName="VASTParameterComboBox" explicitFocusOrder="34" pos="78.75% 30.261% 9.375% 2.435%"
            tooltip="Filter 3 routing" editable="0" layout="33" items=""
            textWhenNonSelected="" textWhenNoItems="---"/>
  <COMBOBOX name="m_uFilterType_Filter1" id="8722f2285367e2ba" memberName="m_uFilterType_Filter1"
            virtualName="VASTParameterComboBox" explicitFocusOrder="1" pos="5.125% 7.304% 16.375% 2.783%"
            tooltip="Filter 1 type" editable="0" layout="33" items="" textWhenNonSelected=""
            textWhenNoItems="---"/>
  <COMBOBOX name="m_uFilterType_Filter2" id="a1516f56e6530af3" memberName="m_uFilterType_Filter2"
            virtualName="VASTParameterComboBox" explicitFocusOrder="13" pos="5.125% 18.783% 16.375% 2.783%"
            tooltip="Filter 2 type" editable="0" layout="33" items="" textWhenNonSelected=""
            textWhenNoItems="---"/>
  <COMBOBOX name="m_uFilterType_Filter3" id="c842536da9130d98" memberName="m_uFilterType_Filter3"
            virtualName="VASTParameterComboBox" explicitFocusOrder="25" pos="5.125% 30.261% 16.375% 2.783%"
            tooltip="Filter 3 type" editable="0" layout="33" items="" textWhenNonSelected=""
            textWhenNoItems="---"/>
  <LABEL name="new label" id="4bd20270ac44d88f" memberName="label2" virtualName=""
         explicitFocusOrder="0" pos="69.75% 1.391% 9.5% 2.087%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="ENV MOD" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Syntax"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <COMBOBOX name="m_uFilterRouting2_Filter1" id="2fbf122a760259e4" memberName="m_uFilterRouting2_Filter1"
            virtualName="VASTParameterComboBox" explicitFocusOrder="11" pos="89.5% 7.304% 9.375% 2.435%"
            tooltip="Filter 1 routing 2" editable="0" layout="33" items=""
            textWhenNonSelected="" textWhenNoItems="---"/>
  <LABEL name="new label" id="340bc44e3b6b2b24" memberName="label3" virtualName=""
         explicitFocusOrder="0" pos="89.5% 1.391% 9.5% 2.087%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="ROUTING 2" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Syntax"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <COMBOBOX name="m_uFilterRouting2_Filter2" id="16e20b06c2ba70ee" memberName="m_uFilterRouting2_Filter2"
            virtualName="VASTParameterComboBox" explicitFocusOrder="23" pos="89.5% 18.783% 9.375% 2.435%"
            tooltip="Filter 2 routing 2" editable="0" layout="33" items=""
            textWhenNonSelected="" textWhenNoItems="---"/>
  <COMBOBOX name="m_uFilterRouting2_Filter3" id="1a2d0c1e62e636b8" memberName="m_uFilterRouting2_Filter3"
            virtualName="VASTParameterComboBox" explicitFocusOrder="35" pos="89.5% 30.261% 9.375% 2.435%"
            tooltip="Filter 3 routing 2" editable="0" layout="33" items=""
            textWhenNonSelected="" textWhenNoItems="---"/>
  <LABEL name="new label" id="7044351237551397" memberName="label4" virtualName=""
         explicitFocusOrder="0" pos="47.625% 24.348% 6.625% 2.087%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="SCALE" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="9.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <LABEL name="new label" id="7309142157370888" memberName="label5" virtualName=""
         explicitFocusOrder="0" pos="47.625% 12% 6.625% 2.087%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="SCALE" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="9.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <SLIDER name="m_fFilterScale_Filter1" id="47d418047dd36006" memberName="m_fFilterScale_Filter1"
          virtualName="VASTParameterSlider" explicitFocusOrder="6" pos="48.25% 4.87% 5% 6.957%"
          tooltip="Filter 1 scale (gain for notch, peak, shelf filters)"
          textboxtext="ffc9c9c9" textboxbkgd="ffffff" textboxoutline="808080"
          min="0.0" max="100.0" int="0.001" style="RotaryVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="30"
          textBoxHeight="14" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="m_fFilterScale_Filter2" id="2883e3c9b041430" memberName="m_fFilterScale_Filter2"
          virtualName="VASTParameterSlider" explicitFocusOrder="18" pos="48.25% 17.391% 5% 6.957%"
          tooltip="Filter 2 scale (gain for notch, peak, shelf filters)"
          textboxtext="ffc9c9c9" textboxbkgd="ffffff" textboxoutline="808080"
          min="0.0" max="100.0" int="0.001" style="RotaryVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="30"
          textBoxHeight="14" skewFactor="1.0" needsCallback="1"/>
  <LABEL name="new label" id="94468e7a5553e12" memberName="label7" virtualName=""
         explicitFocusOrder="0" pos="47.625% 36.174% 6.625% 2.087%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="SCALE" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="9.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <SLIDER name="m_fFilterScale_Filter3" id="b2ff81e7622335f" memberName="m_fFilterScale_Filter3"
          virtualName="VASTParameterSlider" explicitFocusOrder="30" pos="48.25% 28.522% 5% 6.957%"
          tooltip="Filter 3 scale (gain for notch, peak, shelf filters)"
          textboxtext="ffc9c9c9" textboxbkgd="ffffff" textboxoutline="808080"
          min="0.0" max="100.0" int="0.001" style="RotaryVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="30"
          textBoxHeight="14" skewFactor="1.0" needsCallback="1"/>
  <LABEL name="new label" id="fd25019bdef0ea5" memberName="label8" virtualName=""
         explicitFocusOrder="0" pos="52.75% 24.348% 6.625% 2.087%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="GAIN" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="9.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <LABEL name="new label" id="5a4152fe6efff9f0" memberName="label9" virtualName=""
         explicitFocusOrder="0" pos="52.75% 12% 6.625% 2.087%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="GAIN" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="9.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <SLIDER name="m_fFilterGain_Filter1" id="751a8b5dad3fbefe" memberName="m_fFilterGain_Filter1"
          virtualName="VASTParameterSlider" explicitFocusOrder="7" pos="53.875% 4.87% 5% 6.957%"
          tooltip="Filter 1 output gain" textboxtext="ffc9c9c9" textboxbkgd="ffffff"
          textboxoutline="808080" min="0.0" max="200.0" int="0.001" style="RotaryVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="30"
          textBoxHeight="14" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="m_fFilterGain_Filter2" id="7fdb40d7b3b0c0d2" memberName="m_fFilterGain_Filter2"
          virtualName="VASTParameterSlider" explicitFocusOrder="19" pos="53.875% 17.391% 5% 6.957%"
          tooltip="Filter 2 output gain" textboxtext="ffc9c9c9" textboxbkgd="ffffff"
          textboxoutline="808080" min="0.0" max="200.0" int="0.001" style="RotaryVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="30"
          textBoxHeight="14" skewFactor="1.0" needsCallback="1"/>
  <LABEL name="new label" id="dbbe91546a175f6b" memberName="label10" virtualName=""
         explicitFocusOrder="0" pos="52.75% 36.174% 6.625% 2.087%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="GAIN" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="9.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <SLIDER name="m_fFilterGain_Filter3" id="1ca8b5b3b44a3398" memberName="m_fFilterGain_Filter3"
          virtualName="VASTParameterSlider" explicitFocusOrder="31" pos="53.875% 28.522% 5% 6.957%"
          tooltip="Filter 3 output gain" textboxtext="ffc9c9c9" textboxbkgd="ffffff"
          textboxoutline="808080" min="0.0" max="200.0" int="0.001" style="RotaryVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="30"
          textBoxHeight="14" skewFactor="1.0" needsCallback="1"/>
  <LABEL name="new label" id="69f8ddaaf3efa712" memberName="label11" virtualName=""
         explicitFocusOrder="0" pos="8% 1.391% 9.5% 2.087%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="TYPE" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Syntax"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="36"/>
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
static const unsigned char resource_VASTFilterComponent_arrow_right_corner_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,70,
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

const char* VASTFilterComponent::arrow_right_corner_svg = (const char*) resource_VASTFilterComponent_arrow_right_corner_svg;
const int VASTFilterComponent::arrow_right_corner_svgSize = 1715;


//[EndFile] You can add extra defines here...
//[/EndFile]

