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
#include "../Engine/VASTEngineHeader.h"
#include "VASTAudioProcessor.h"
#include "VASTAudioProcessorEditor.h"
#include "VASTControls/VASTParameterComboBox.h"
#include "VASTControls/VASTParameterSlider.h"
#include "VASTControls/VASTParameterButton.h"
//[/Headers]

#include "VASTSamplerNoiseComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
VASTSamplerNoiseComponent::VASTSamplerNoiseComponent (AudioProcessorEditor *editor, AudioProcessor* processor)
    : myEditor((VASTAudioProcessorEditor*)editor), myProcessor((VASTAudioProcessor*)processor)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    m_uSamplerRootKey.reset (new VASTParameterSlider ("m_uSamplerRootKey"));
    addAndMakeVisible (m_uSamplerRootKey.get());
    m_uSamplerRootKey->setTooltip (TRANS("Sample root key"));
    m_uSamplerRootKey->setRange (0, 127, 0.01);
    m_uSamplerRootKey->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    m_uSamplerRootKey->setTextBoxStyle (juce::Slider::NoTextBox, false, 30, 14);
    m_uSamplerRootKey->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffc9c9c9));
    m_uSamplerRootKey->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0x00ffffff));
    m_uSamplerRootKey->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x00808080));
    m_uSamplerRootKey->addListener (this);

    m_fNoiseGain.reset (new VASTParameterSlider ("m_fNoiseGain"));
    addAndMakeVisible (m_fNoiseGain.get());
    m_fNoiseGain->setTooltip (TRANS("Noise gain (100 full, >100 overdrive)"));
    m_fNoiseGain->setRange (0, 200, 0.01);
    m_fNoiseGain->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    m_fNoiseGain->setTextBoxStyle (juce::Slider::NoTextBox, false, 30, 14);
    m_fNoiseGain->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffc9c9c9));
    m_fNoiseGain->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0x00ffffff));
    m_fNoiseGain->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x00808080));
    m_fNoiseGain->addListener (this);

    label4.reset (new juce::Label ("new label",
                                   TRANS("NOISE\n")));
    addAndMakeVisible (label4.get());
    label4->setFont (juce::Font ("Syntax", 13.00f, juce::Font::plain).withTypefaceStyle ("Bold"));
    label4->setJustificationType (juce::Justification::centredLeft);
    label4->setEditable (false, false, false);
    label4->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label4->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label4->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label2.reset (new juce::Label ("new label",
                                   TRANS("SAMPLER")));
    addAndMakeVisible (label2.get());
    label2->setFont (juce::Font ("Syntax", 13.00f, juce::Font::plain).withTypefaceStyle ("Bold"));
    label2->setJustificationType (juce::Justification::centredLeft);
    label2->setEditable (false, false, false);
    label2->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label2->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label2->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    m_bSamplerOnOff.reset (new VASTParameterButton ("m_bSamplerOnOff"));
    addAndMakeVisible (m_bSamplerOnOff.get());
    m_bSamplerOnOff->setTooltip (TRANS("Switch sampler on / off"));
    m_bSamplerOnOff->setButtonText (juce::String());
    m_bSamplerOnOff->addListener (this);

    label12.reset (new juce::Label ("new label",
                                    TRANS("GAIN")));
    addAndMakeVisible (label12.get());
    label12->setFont (juce::Font ("Syntax", 11.00f, juce::Font::plain));
    label12->setJustificationType (juce::Justification::centred);
    label12->setEditable (false, false, false);
    label12->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label12->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label12->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label7.reset (new juce::Label ("new label",
                                   TRANS("GAIN")));
    addAndMakeVisible (label7.get());
    label7->setFont (juce::Font ("Syntax", 11.00f, juce::Font::plain));
    label7->setJustificationType (juce::Justification::centred);
    label7->setEditable (false, false, false);
    label7->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label7->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label7->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label13.reset (new juce::Label ("new label",
                                    TRANS("PAN")));
    addAndMakeVisible (label13.get());
    label13->setFont (juce::Font ("Syntax", 11.00f, juce::Font::plain));
    label13->setJustificationType (juce::Justification::centred);
    label13->setEditable (false, false, false);
    label13->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label13->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label13->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label14.reset (new juce::Label ("new label",
                                    TRANS("ROOT")));
    addAndMakeVisible (label14.get());
    label14->setFont (juce::Font ("Syntax", 11.00f, juce::Font::plain));
    label14->setJustificationType (juce::Justification::centred);
    label14->setEditable (false, false, false);
    label14->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label14->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label14->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label15.reset (new juce::Label ("new label",
                                    TRANS("KEYTRACK")));
    addAndMakeVisible (label15.get());
    label15->setFont (juce::Font ("Syntax", 11.00f, juce::Font::plain));
    label15->setJustificationType (juce::Justification::centredLeft);
    label15->setEditable (false, false, false);
    label15->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label15->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label15->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label16.reset (new juce::Label ("new label",
                                    TRANS("LOOP")));
    addAndMakeVisible (label16.get());
    label16->setFont (juce::Font ("Syntax", 11.00f, juce::Font::plain));
    label16->setJustificationType (juce::Justification::centredLeft);
    label16->setEditable (false, false, false);
    label16->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label16->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label16->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label17.reset (new juce::Label ("new label",
                                    TRANS("TYPE")));
    addAndMakeVisible (label17.get());
    label17->setFont (juce::Font ("Syntax", 11.00f, juce::Font::plain));
    label17->setJustificationType (juce::Justification::centredRight);
    label17->setEditable (false, false, false);
    label17->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label17->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label17->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label5.reset (new juce::Label ("new label",
                                   TRANS("ENV")));
    addAndMakeVisible (label5.get());
    label5->setFont (juce::Font ("Syntax", 11.00f, juce::Font::plain));
    label5->setJustificationType (juce::Justification::centredRight);
    label5->setEditable (false, false, false);
    label5->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label5->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label5->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    m_fNoisePan.reset (new VASTParameterSlider ("m_fNoisePan"));
    addAndMakeVisible (m_fNoisePan.get());
    m_fNoisePan->setTooltip (TRANS("Stereo panning of noise oscillator"));
    m_fNoisePan->setRange (-100, 100, 0.01);
    m_fNoisePan->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    m_fNoisePan->setTextBoxStyle (juce::Slider::NoTextBox, false, 30, 14);
    m_fNoisePan->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffc9c9c9));
    m_fNoisePan->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0x00ffffff));
    m_fNoisePan->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x00808080));
    m_fNoisePan->addListener (this);

    label6.reset (new juce::Label ("new label",
                                   TRANS("PAN")));
    addAndMakeVisible (label6.get());
    label6->setFont (juce::Font ("Syntax", 11.00f, juce::Font::plain));
    label6->setJustificationType (juce::Justification::centred);
    label6->setEditable (false, false, false);
    label6->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label6->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label6->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    m_fNoiseLowCut.reset (new VASTParameterSlider ("m_fNoiseLowCut"));
    addAndMakeVisible (m_fNoiseLowCut.get());
    m_fNoiseLowCut->setTooltip (TRANS("Noise oscillator low frequency cutoff"));
    m_fNoiseLowCut->setRange (0, 200, 0.01);
    m_fNoiseLowCut->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    m_fNoiseLowCut->setTextBoxStyle (juce::Slider::NoTextBox, false, 30, 14);
    m_fNoiseLowCut->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffc9c9c9));
    m_fNoiseLowCut->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0x00ffffff));
    m_fNoiseLowCut->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x00808080));
    m_fNoiseLowCut->addListener (this);

    label8.reset (new juce::Label ("new label",
                                   TRANS("LOW CUT")));
    addAndMakeVisible (label8.get());
    label8->setFont (juce::Font ("Syntax", 11.00f, juce::Font::plain));
    label8->setJustificationType (juce::Justification::centred);
    label8->setEditable (false, false, false);
    label8->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label8->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label8->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label9.reset (new juce::Label ("new label",
                                   TRANS("ENV")));
    addAndMakeVisible (label9.get());
    label9->setFont (juce::Font ("Syntax", 11.00f, juce::Font::plain));
    label9->setJustificationType (juce::Justification::centredRight);
    label9->setEditable (false, false, false);
    label9->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label9->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label9->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label10.reset (new juce::Label ("new label",
                                    TRANS("2")));
    addAndMakeVisible (label10.get());
    label10->setFont (juce::Font ("Syntax", 11.00f, juce::Font::plain));
    label10->setJustificationType (juce::Justification::centredRight);
    label10->setEditable (false, false, false);
    label10->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label10->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label10->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label11.reset (new juce::Label ("new label",
                                    TRANS("1")));
    addAndMakeVisible (label11.get());
    label11->setFont (juce::Font ("Syntax", 11.00f, juce::Font::plain));
    label11->setJustificationType (juce::Justification::centredRight);
    label11->setEditable (false, false, false);
    label11->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label11->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label11->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label18.reset (new juce::Label ("new label",
                                    TRANS("2")));
    addAndMakeVisible (label18.get());
    label18->setFont (juce::Font ("Syntax", 11.00f, juce::Font::plain));
    label18->setJustificationType (juce::Justification::centredRight);
    label18->setEditable (false, false, false);
    label18->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label18->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label18->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label19.reset (new juce::Label ("new label",
                                    TRANS("1")));
    addAndMakeVisible (label19.get());
    label19->setFont (juce::Font ("Syntax", 11.00f, juce::Font::plain));
    label19->setJustificationType (juce::Justification::centredRight);
    label19->setEditable (false, false, false);
    label19->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label19->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label19->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label20.reset (new juce::Label ("new label",
                                    TRANS("POLY")));
    addAndMakeVisible (label20.get());
    label20->setFont (juce::Font ("Syntax", 11.00f, juce::Font::plain));
    label20->setJustificationType (juce::Justification::centredLeft);
    label20->setEditable (false, false, false);
    label20->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label20->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label20->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    m_uSamplerRouting2.reset (new VASTParameterComboBox ("m_uSamplerRouting2"));
    addAndMakeVisible (m_uSamplerRouting2.get());
    m_uSamplerRouting2->setTooltip (TRANS("Sampler routing 2"));
    m_uSamplerRouting2->setEditableText (false);
    m_uSamplerRouting2->setJustificationType (juce::Justification::centredLeft);
    m_uSamplerRouting2->setTextWhenNothingSelected (juce::String());
    m_uSamplerRouting2->setTextWhenNoChoicesAvailable (TRANS("---"));
    m_uSamplerRouting2->addListener (this);

    m_uSamplerRouting1.reset (new VASTParameterComboBox ("m_uSamplerRouting1"));
    addAndMakeVisible (m_uSamplerRouting1.get());
    m_uSamplerRouting1->setTooltip (TRANS("Sampler routing 1"));
    m_uSamplerRouting1->setEditableText (false);
    m_uSamplerRouting1->setJustificationType (juce::Justification::centredLeft);
    m_uSamplerRouting1->setTextWhenNothingSelected (juce::String());
    m_uSamplerRouting1->setTextWhenNoChoicesAvailable (TRANS("---"));
    m_uSamplerRouting1->addListener (this);

    m_bNoiseOnOff.reset (new VASTParameterButton ("m_bNoiseOnOff"));
    addAndMakeVisible (m_bNoiseOnOff.get());
    m_bNoiseOnOff->setTooltip (TRANS("Switch noise oscillator on / off"));
    m_bNoiseOnOff->setButtonText (juce::String());
    m_bNoiseOnOff->addListener (this);

    m_uNoiseRouting1.reset (new VASTParameterComboBox ("m_uNoiseRouting1"));
    addAndMakeVisible (m_uNoiseRouting1.get());
    m_uNoiseRouting1->setTooltip (TRANS("Noise routing 1"));
    m_uNoiseRouting1->setEditableText (false);
    m_uNoiseRouting1->setJustificationType (juce::Justification::centredLeft);
    m_uNoiseRouting1->setTextWhenNothingSelected (juce::String());
    m_uNoiseRouting1->setTextWhenNoChoicesAvailable (TRANS("---"));
    m_uNoiseRouting1->addListener (this);

    m_fSamplerGain.reset (new VASTParameterSlider ("m_fSamplerGain"));
    addAndMakeVisible (m_fSamplerGain.get());
    m_fSamplerGain->setTooltip (TRANS("Sampler gain (100 full, >100 overdrive)"));
    m_fSamplerGain->setRange (0, 200, 0.01);
    m_fSamplerGain->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    m_fSamplerGain->setTextBoxStyle (juce::Slider::NoTextBox, false, 30, 14);
    m_fSamplerGain->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffc9c9c9));
    m_fSamplerGain->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0x00ffffff));
    m_fSamplerGain->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x00808080));
    m_fSamplerGain->addListener (this);

    m_fSamplerPan.reset (new VASTParameterSlider ("m_fSamplerPan"));
    addAndMakeVisible (m_fSamplerPan.get());
    m_fSamplerPan->setTooltip (TRANS("Stereo panning of sampler"));
    m_fSamplerPan->setRange (-100, 100, 0.01);
    m_fSamplerPan->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    m_fSamplerPan->setTextBoxStyle (juce::Slider::NoTextBox, false, 30, 14);
    m_fSamplerPan->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffc9c9c9));
    m_fSamplerPan->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0x00ffffff));
    m_fSamplerPan->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x00808080));
    m_fSamplerPan->addListener (this);

    m_bSamplerPoly.reset (new VASTParameterButton ("m_bSamplerPoly"));
    addAndMakeVisible (m_bSamplerPoly.get());
    m_bSamplerPoly->setTooltip (TRANS("Play sample in polyphonic or monophonic mode"));
    m_bSamplerPoly->setButtonText (juce::String());
    m_bSamplerPoly->addListener (this);

    m_bSamplerKeytrack.reset (new VASTParameterButton ("m_bSamplerKeytrack"));
    addAndMakeVisible (m_bSamplerKeytrack.get());
    m_bSamplerKeytrack->setTooltip (TRANS("Sample frequencyis kextracked or static"));
    m_bSamplerKeytrack->setButtonText (juce::String());
    m_bSamplerKeytrack->addListener (this);

    m_bSamplerLoop.reset (new VASTParameterButton ("m_bSamplerLoop"));
    addAndMakeVisible (m_bSamplerLoop.get());
    m_bSamplerLoop->setTooltip (TRANS("Loop sample"));
    m_bSamplerLoop->setButtonText (juce::String());
    m_bSamplerLoop->addListener (this);

    m_uVCAEnv_Sampler.reset (new VASTParameterComboBox ("m_uVCAEnv_Sampler"));
    addAndMakeVisible (m_uVCAEnv_Sampler.get());
    m_uVCAEnv_Sampler->setTooltip (TRANS("Envelope used as sampler envelope"));
    m_uVCAEnv_Sampler->setEditableText (false);
    m_uVCAEnv_Sampler->setJustificationType (juce::Justification::centredLeft);
    m_uVCAEnv_Sampler->setTextWhenNothingSelected (juce::String());
    m_uVCAEnv_Sampler->setTextWhenNoChoicesAvailable (TRANS("---"));
    m_uVCAEnv_Sampler->addListener (this);

    m_uVCAEnv_Noise.reset (new VASTParameterComboBox ("m_uVCAEnv_Noise"));
    addAndMakeVisible (m_uVCAEnv_Noise.get());
    m_uVCAEnv_Noise->setTooltip (TRANS("Envelope used as noise envelope"));
    m_uVCAEnv_Noise->setEditableText (false);
    m_uVCAEnv_Noise->setJustificationType (juce::Justification::centredLeft);
    m_uVCAEnv_Noise->setTextWhenNothingSelected (juce::String());
    m_uVCAEnv_Noise->setTextWhenNoChoicesAvailable (TRANS("---"));
    m_uVCAEnv_Noise->addListener (this);

    m_uNoiseType.reset (new VASTParameterComboBox ("m_uNoiseType"));
    addAndMakeVisible (m_uNoiseType.get());
    m_uNoiseType->setTooltip (TRANS("Noise oscillator type"));
    m_uNoiseType->setEditableText (false);
    m_uNoiseType->setJustificationType (juce::Justification::centredLeft);
    m_uNoiseType->setTextWhenNothingSelected (juce::String());
    m_uNoiseType->setTextWhenNoChoicesAvailable (TRANS("---"));
    m_uNoiseType->addListener (this);

    m_uNoiseRouting2.reset (new VASTParameterComboBox ("m_uNoiseRouting2"));
    addAndMakeVisible (m_uNoiseRouting2.get());
    m_uNoiseRouting2->setTooltip (TRANS("Noise routing 2"));
    m_uNoiseRouting2->setEditableText (false);
    m_uNoiseRouting2->setJustificationType (juce::Justification::centredLeft);
    m_uNoiseRouting2->setTextWhenNothingSelected (juce::String());
    m_uNoiseRouting2->setTextWhenNoChoicesAvailable (TRANS("---"));
    m_uNoiseRouting2->addListener (this);

    label3.reset (new juce::Label ("new label",
                                   TRANS("ENV")));
    addAndMakeVisible (label3.get());
    label3->setFont (juce::Font ("Syntax", 11.00f, juce::Font::plain));
    label3->setJustificationType (juce::Justification::centredRight);
    label3->setEditable (false, false, false);
    label3->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label3->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label3->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    m_fSamplerCents.reset (new VASTParameterSlider ("m_fSamplerCents"));
    addAndMakeVisible (m_fSamplerCents.get());
    m_fSamplerCents->setTooltip (TRANS("Sampler cents (+/- 1 octave and half tones)"));
    m_fSamplerCents->setRange (0, 127, 0.01);
    m_fSamplerCents->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    m_fSamplerCents->setTextBoxStyle (juce::Slider::NoTextBox, false, 30, 14);
    m_fSamplerCents->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffc9c9c9));
    m_fSamplerCents->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0x00ffffff));
    m_fSamplerCents->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x00808080));
    m_fSamplerCents->addListener (this);

    label21.reset (new juce::Label ("new label",
                                    TRANS("CENTS")));
    addAndMakeVisible (label21.get());
    label21->setFont (juce::Font ("Syntax", 11.00f, juce::Font::plain));
    label21->setJustificationType (juce::Justification::centred);
    label21->setEditable (false, false, false);
    label21->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label21->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label21->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label22.reset (new juce::Label ("new label",
                                    TRANS("ONLY")));
    addAndMakeVisible (label22.get());
    label22->setFont (juce::Font ("Syntax", 11.00f, juce::Font::plain));
    label22->setJustificationType (juce::Justification::centredLeft);
    label22->setEditable (false, false, false);
    label22->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label22->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label22->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    m_bSamplerLoopOnly.reset (new VASTParameterButton ("m_bSamplerLoopOnly"));
    addAndMakeVisible (m_bSamplerLoopOnly.get());
    m_bSamplerLoopOnly->setTooltip (TRANS("Play only sample loop without start"));
    m_bSamplerLoopOnly->setButtonText (juce::String());
    m_bSamplerLoopOnly->addListener (this);

    label23.reset (new juce::Label ("new label",
                                    TRANS("PINGPONG")));
    addAndMakeVisible (label23.get());
    label23->setFont (juce::Font ("Syntax", 11.00f, juce::Font::plain));
    label23->setJustificationType (juce::Justification::centredLeft);
    label23->setEditable (false, false, false);
    label23->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label23->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label23->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    m_bSamplerLoopPingpong.reset (new VASTParameterButton ("m_bSamplerLoopPingpong"));
    addAndMakeVisible (m_bSamplerLoopPingpong.get());
    m_bSamplerLoopPingpong->setTooltip (TRANS("Play sample loop in pingpong mode"));
    m_bSamplerLoopPingpong->setButtonText (juce::String());
    m_bSamplerLoopPingpong->addListener (this);

    m_fSamplerLoopRange.reset (new VASTParameterSlider ("m_fSamplerLoopRange"));
    addAndMakeVisible (m_fSamplerLoopRange.get());
    m_fSamplerLoopRange->setTooltip (TRANS("Sampler loop range"));
    m_fSamplerLoopRange->setRange (1, 100, 0.001);
    m_fSamplerLoopRange->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    m_fSamplerLoopRange->setTextBoxStyle (juce::Slider::NoTextBox, false, 30, 14);
    m_fSamplerLoopRange->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffc9c9c9));
    m_fSamplerLoopRange->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0x00ffffff));
    m_fSamplerLoopRange->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x00808080));
    m_fSamplerLoopRange->addListener (this);

    label25.reset (new juce::Label ("new label",
                                    TRANS("RANGE")));
    addAndMakeVisible (label25.get());
    label25->setFont (juce::Font ("Syntax", 11.00f, juce::Font::plain));
    label25->setJustificationType (juce::Justification::centred);
    label25->setEditable (false, false, false);
    label25->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label25->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label25->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    m_fNoiseResonance.reset (new VASTParameterSlider ("m_fNoiseResonance"));
    addAndMakeVisible (m_fNoiseResonance.get());
    m_fNoiseResonance->setTooltip (TRANS("Noise resonance"));
    m_fNoiseResonance->setRange (1, 100, 0.001);
    m_fNoiseResonance->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    m_fNoiseResonance->setTextBoxStyle (juce::Slider::NoTextBox, false, 30, 14);
    m_fNoiseResonance->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffc9c9c9));
    m_fNoiseResonance->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0x00ffffff));
    m_fNoiseResonance->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x00808080));
    m_fNoiseResonance->addListener (this);

    label26.reset (new juce::Label ("new label",
                                    TRANS("RESO")));
    addAndMakeVisible (label26.get());
    label26->setFont (juce::Font ("Syntax", 11.00f, juce::Font::plain));
    label26->setJustificationType (juce::Justification::centred);
    label26->setEditable (false, false, false);
    label26->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label26->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label26->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    m_fNoiseHighCut.reset (new VASTParameterSlider ("m_fNoiseHighCut"));
    addAndMakeVisible (m_fNoiseHighCut.get());
    m_fNoiseHighCut->setTooltip (TRANS("Noise oscillator high frequency cutoff"));
    m_fNoiseHighCut->setRange (0, 200, 0.01);
    m_fNoiseHighCut->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    m_fNoiseHighCut->setTextBoxStyle (juce::Slider::NoTextBox, false, 30, 14);
    m_fNoiseHighCut->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffc9c9c9));
    m_fNoiseHighCut->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0x00ffffff));
    m_fNoiseHighCut->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x00808080));
    m_fNoiseHighCut->addListener (this);

    label24.reset (new juce::Label ("new label",
                                    TRANS("HI CUT")));
    addAndMakeVisible (label24.get());
    label24->setFont (juce::Font ("Syntax", 11.00f, juce::Font::plain));
    label24->setJustificationType (juce::Justification::centred);
    label24->setEditable (false, false, false);
    label24->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label24->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label24->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    m_fSamplerGrainInteronset.reset (new VASTParameterSlider ("m_fSamplerGrainInteronset"));
    addAndMakeVisible (m_fSamplerGrainInteronset.get());
    m_fSamplerGrainInteronset->setTooltip (TRANS("Sampler grain inter-onset (ms)"));
    m_fSamplerGrainInteronset->setRange (1, 100, 0.001);
    m_fSamplerGrainInteronset->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    m_fSamplerGrainInteronset->setTextBoxStyle (juce::Slider::NoTextBox, false, 30, 14);
    m_fSamplerGrainInteronset->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffc9c9c9));
    m_fSamplerGrainInteronset->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0x00ffffff));
    m_fSamplerGrainInteronset->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x00808080));
    m_fSamplerGrainInteronset->addListener (this);

    label27.reset (new juce::Label ("new label",
                                    TRANS("GRAIN")));
    addAndMakeVisible (label27.get());
    label27->setFont (juce::Font ("Syntax", 11.00f, juce::Font::plain));
    label27->setJustificationType (juce::Justification::centred);
    label27->setEditable (false, false, false);
    label27->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label27->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label27->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    m_fSamplerGrainRandom.reset (new VASTParameterSlider ("m_fSamplerGrainRandom"));
    addAndMakeVisible (m_fSamplerGrainRandom.get());
    m_fSamplerGrainRandom->setTooltip (TRANS("Sampler grain inter-onset and detune random"));
    m_fSamplerGrainRandom->setRange (1, 100, 0.001);
    m_fSamplerGrainRandom->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    m_fSamplerGrainRandom->setTextBoxStyle (juce::Slider::NoTextBox, false, 30, 14);
    m_fSamplerGrainRandom->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffc9c9c9));
    m_fSamplerGrainRandom->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0x00ffffff));
    m_fSamplerGrainRandom->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x00808080));
    m_fSamplerGrainRandom->addListener (this);

    label28.reset (new juce::Label ("new label",
                                    TRANS("RND")));
    addAndMakeVisible (label28.get());
    label28->setFont (juce::Font ("Syntax", 11.00f, juce::Font::plain));
    label28->setJustificationType (juce::Justification::centred);
    label28->setEditable (false, false, false);
    label28->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label28->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label28->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label29.reset (new juce::Label ("new label",
                                    TRANS("SHAPE")));
    addAndMakeVisible (label29.get());
    label29->setFont (juce::Font ("Syntax", 11.00f, juce::Font::plain));
    label29->setJustificationType (juce::Justification::centred);
    label29->setEditable (false, false, false);
    label29->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label29->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label29->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    m_fSamplerGrainShape.reset (new VASTParameterSlider ("m_fSamplerGrainShape"));
    addAndMakeVisible (m_fSamplerGrainShape.get());
    m_fSamplerGrainShape->setTooltip (TRANS("Sampler grain shape"));
    m_fSamplerGrainShape->setRange (1, 100, 0.001);
    m_fSamplerGrainShape->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    m_fSamplerGrainShape->setTextBoxStyle (juce::Slider::NoTextBox, false, 30, 14);
    m_fSamplerGrainShape->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffc9c9c9));
    m_fSamplerGrainShape->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0x00ffffff));
    m_fSamplerGrainShape->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x00808080));
    m_fSamplerGrainShape->addListener (this);


    //[UserPreSize]
	Array<Component*> children = this->getChildren();
	for (auto child : children) {
		if (child->getName().startsWith("m_")) {
			auto* aSlider = dynamic_cast<VASTParameterSlider*> (child);
			if (aSlider != nullptr) {
				aSlider->setAudioProcessor(*myProcessor);
				aSlider->bindParameter(myEditor, aSlider->getName(), VASTGUIRuntimeModel::GUIComponents::SamplerNoiseComponent, 0);
			}
			auto* aCombobox = dynamic_cast<VASTParameterComboBox*> (child);
			if (aCombobox != nullptr) {
				aCombobox->setAudioProcessor(*myProcessor);
				aCombobox->bindParameter(myEditor, aCombobox->getName(), VASTGUIRuntimeModel::GUIComponents::SamplerNoiseComponent, 0);
			}
			auto* aButton = dynamic_cast<VASTParameterButton*> (child);
			if (aButton != nullptr) {
				aButton->setAudioProcessor(*myProcessor);
				aButton->bindParameter(myEditor, aButton->getName(), VASTGUIRuntimeModel::GUIComponents::SamplerNoiseComponent, 0);
			}
		}
	}
	setOpaque(true);
    
    return; //dont call setSize
    //[/UserPreSize]

    setSize (536, 130);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

VASTSamplerNoiseComponent::~VASTSamplerNoiseComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    m_uSamplerRootKey = nullptr;
    m_fNoiseGain = nullptr;
    label4 = nullptr;
    label2 = nullptr;
    m_bSamplerOnOff = nullptr;
    label12 = nullptr;
    label7 = nullptr;
    label13 = nullptr;
    label14 = nullptr;
    label15 = nullptr;
    label16 = nullptr;
    label17 = nullptr;
    label5 = nullptr;
    m_fNoisePan = nullptr;
    label6 = nullptr;
    m_fNoiseLowCut = nullptr;
    label8 = nullptr;
    label9 = nullptr;
    label10 = nullptr;
    label11 = nullptr;
    label18 = nullptr;
    label19 = nullptr;
    label20 = nullptr;
    m_uSamplerRouting2 = nullptr;
    m_uSamplerRouting1 = nullptr;
    m_bNoiseOnOff = nullptr;
    m_uNoiseRouting1 = nullptr;
    m_fSamplerGain = nullptr;
    m_fSamplerPan = nullptr;
    m_bSamplerPoly = nullptr;
    m_bSamplerKeytrack = nullptr;
    m_bSamplerLoop = nullptr;
    m_uVCAEnv_Sampler = nullptr;
    m_uVCAEnv_Noise = nullptr;
    m_uNoiseType = nullptr;
    m_uNoiseRouting2 = nullptr;
    label3 = nullptr;
    m_fSamplerCents = nullptr;
    label21 = nullptr;
    label22 = nullptr;
    m_bSamplerLoopOnly = nullptr;
    label23 = nullptr;
    m_bSamplerLoopPingpong = nullptr;
    m_fSamplerLoopRange = nullptr;
    label25 = nullptr;
    m_fNoiseResonance = nullptr;
    label26 = nullptr;
    m_fNoiseHighCut = nullptr;
    label24 = nullptr;
    m_fSamplerGrainInteronset = nullptr;
    label27 = nullptr;
    m_fSamplerGrainRandom = nullptr;
    label28 = nullptr;
    label29 = nullptr;
    m_fSamplerGrainShape = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void VASTSamplerNoiseComponent::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colours::black);

    {
        int x = proportionOfWidth (0.0000f), y = proportionOfHeight (0.0000f), width = proportionOfWidth (0.5280f), height = proportionOfHeight (1.0000f);
        juce::Colour fillColour1 = juce::Colour (0xff56646c), fillColour2 = juce::Colour (0xff232527);
        //[UserPaintCustomArguments] Customize the painting arguments here..
    		fillColour1 = myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscillatorComponentBackgroundGradientFrom);
    		fillColour2 = myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscillatorComponentBackgroundGradientTo);
        //[/UserPaintCustomArguments]
        g.setGradientFill (juce::ColourGradient (fillColour1,
                                             static_cast<float> (proportionOfWidth (-0.0113f)) - static_cast<float> (proportionOfWidth (0.0000f)) + x,
                                             static_cast<float> (proportionOfHeight (-0.0293f)) - static_cast<float> (proportionOfHeight (0.0000f)) + y,
                                             fillColour2,
                                             static_cast<float> (proportionOfWidth (1.0149f)) - static_cast<float> (proportionOfWidth (0.0000f)) + x,
                                             static_cast<float> (proportionOfHeight (1.0461f)) - static_cast<float> (proportionOfHeight (0.0000f)) + y,
                                             false));
        g.fillRect (x, y, width, height);
    }

    {
        int x = 1, y = 1, width = proportionOfWidth (0.5261f), height = proportionOfHeight (0.1231f);
        juce::Colour fillColour1 = juce::Colour (0xff3f4446), fillColour2 = juce::Colour (0xff4c5457);
        //[UserPaintCustomArguments] Customize the painting arguments here..
    		fillColour1 = myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscillatorComponentHeaderGradientFrom);
    		fillColour2 = myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscillatorComponentHeaderGradientTo);
        //[/UserPaintCustomArguments]
        g.setGradientFill (juce::ColourGradient (fillColour1,
                                             static_cast<float> (proportionOfWidth (-0.0113f)) - 1.0f + x,
                                             static_cast<float> (proportionOfHeight (-0.0293f)) - 1.0f + y,
                                             fillColour2,
                                             static_cast<float> (proportionOfWidth (0.5224f)) - 1.0f + x,
                                             static_cast<float> (proportionOfHeight (0.4546f)) - 1.0f + y,
                                             false));
        g.fillRect (x, y, width, height);
    }

    {
        int x = getWidth() - proportionOfWidth (0.4720f), y = proportionOfHeight (0.0000f), width = proportionOfWidth (0.4720f), height = proportionOfHeight (1.0000f);
        juce::Colour fillColour1 = juce::Colour (0xff56646c), fillColour2 = juce::Colour (0xff232527);
        //[UserPaintCustomArguments] Customize the painting arguments here..
    		fillColour1 = myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscillatorComponentBackgroundGradientFrom);
    		fillColour2 = myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscillatorComponentBackgroundGradientTo);
        //[/UserPaintCustomArguments]
        g.setGradientFill (juce::ColourGradient (fillColour1,
                                             static_cast<float> (proportionOfWidth (0.5075f)) - static_cast<float> (getWidth() - proportionOfWidth (0.4720f)) + x,
                                             static_cast<float> (proportionOfHeight (0.0000f)) - static_cast<float> (proportionOfHeight (0.0000f)) + y,
                                             fillColour2,
                                             static_cast<float> (proportionOfWidth (1.2985f)) - static_cast<float> (getWidth() - proportionOfWidth (0.4720f)) + x,
                                             static_cast<float> (proportionOfHeight (1.1692f)) - static_cast<float> (proportionOfHeight (0.0000f)) + y,
                                             false));
        g.fillRect (x, y, width, height);
    }

    {
        int x = getWidth() - proportionOfWidth (0.4720f), y = 1, width = proportionOfWidth (0.4720f), height = proportionOfHeight (0.1231f);
        juce::Colour fillColour1 = juce::Colour (0xff3f4446), fillColour2 = juce::Colour (0xff4c5457);
        //[UserPaintCustomArguments] Customize the painting arguments here..
    		fillColour1 = myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscillatorComponentHeaderGradientFrom);
    		fillColour2 = myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscillatorComponentHeaderGradientTo);
        //[/UserPaintCustomArguments]
        g.setGradientFill (juce::ColourGradient (fillColour1,
                                             static_cast<float> (proportionOfWidth (0.5075f)) - static_cast<float> (getWidth() - proportionOfWidth (0.4720f)) + x,
                                             static_cast<float> (proportionOfHeight (-0.0909f)) - 1.0f + y,
                                             fillColour2,
                                             static_cast<float> (proportionOfWidth (1.0448f)) - static_cast<float> (getWidth() - proportionOfWidth (0.4720f)) + x,
                                             static_cast<float> (proportionOfHeight (0.4546f)) - 1.0f + y,
                                             false));
        g.fillRect (x, y, width, height);
    }

    {
        int x = proportionOfWidth (0.0075f), y = proportionOfHeight (0.8538f), width = proportionOfWidth (0.3881f), height = proportionOfHeight (0.0846f);
        juce::Colour fillColour1 = juce::Colour (0xff3a4145), fillColour2 = juce::Colour (0xff33393c);
        //[UserPaintCustomArguments] Customize the painting arguments here..
    		fillColour1 = myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscillatorComponentHeaderGradientFrom);
    		fillColour2 = myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscillatorComponentHeaderGradientTo);
        //[/UserPaintCustomArguments]
        g.setGradientFill (juce::ColourGradient (fillColour1,
                                             static_cast<float> (proportionOfWidth (0.0000f)) - static_cast<float> (proportionOfWidth (0.0075f)) + x,
                                             static_cast<float> (proportionOfHeight (0.8000f)) - static_cast<float> (proportionOfHeight (0.8538f)) + y,
                                             fillColour2,
                                             static_cast<float> (proportionOfWidth (0.2687f)) - static_cast<float> (proportionOfWidth (0.0075f)) + x,
                                             static_cast<float> (proportionOfHeight (0.9231f)) - static_cast<float> (proportionOfHeight (0.8538f)) + y,
                                             false));
        g.fillRect (x, y, width, height);
    }

    {
        int x = proportionOfWidth (0.5373f), y = proportionOfHeight (0.8538f), width = proportionOfWidth (0.2481f), height = proportionOfHeight (0.0846f);
        juce::Colour fillColour1 = juce::Colour (0xff3a4145), fillColour2 = juce::Colour (0xff33393c);
        //[UserPaintCustomArguments] Customize the painting arguments here..
    		fillColour1 = myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscillatorComponentHeaderGradientFrom);
    		fillColour2 = myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscillatorComponentHeaderGradientTo);

        //[/UserPaintCustomArguments]
        g.setGradientFill (juce::ColourGradient (fillColour1,
                                             static_cast<float> (proportionOfWidth (0.5224f)) - static_cast<float> (proportionOfWidth (0.5373f)) + x,
                                             static_cast<float> (proportionOfHeight (0.8000f)) - static_cast<float> (proportionOfHeight (0.8538f)) + y,
                                             fillColour2,
                                             static_cast<float> (proportionOfWidth (0.8060f)) - static_cast<float> (proportionOfWidth (0.5373f)) + x,
                                             static_cast<float> (proportionOfHeight (0.9846f)) - static_cast<float> (proportionOfHeight (0.8538f)) + y,
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
                                             static_cast<float> (proportionOfWidth (0.0000f)) - static_cast<float> (proportionOfWidth (0.0080f)) + x,
                                             static_cast<float> (proportionOfHeight (0.3692f)) - static_cast<float> (proportionOfHeight (0.4231f)) + y,
                                             fillColour2,
                                             static_cast<float> (proportionOfWidth (0.1791f)) - static_cast<float> (proportionOfWidth (0.0080f)) + x,
                                             static_cast<float> (proportionOfHeight (0.5538f)) - static_cast<float> (proportionOfHeight (0.4231f)) + y,
                                             false));
        g.fillRect (x, y, width, height);
    }

    {
        int x = proportionOfWidth (0.5373f), y = proportionOfHeight (0.4231f), width = proportionOfWidth (0.1642f), height = proportionOfHeight (0.0846f);
        juce::Colour fillColour1 = juce::Colour (0xff3a4145), fillColour2 = juce::Colour (0xff33393c);
        //[UserPaintCustomArguments] Customize the painting arguments here..
    		fillColour1 = myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscillatorComponentHeaderGradientFrom);
    		fillColour2 = myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscillatorComponentHeaderGradientTo);
        //[/UserPaintCustomArguments]
        g.setGradientFill (juce::ColourGradient (fillColour1,
                                             static_cast<float> (proportionOfWidth (0.7313f)) - static_cast<float> (proportionOfWidth (0.5373f)) + x,
                                             static_cast<float> (proportionOfHeight (0.5538f)) - static_cast<float> (proportionOfHeight (0.4231f)) + y,
                                             fillColour2,
                                             static_cast<float> (proportionOfWidth (0.5373f)) - static_cast<float> (proportionOfWidth (0.5373f)) + x,
                                             static_cast<float> (proportionOfHeight (0.3692f)) - static_cast<float> (proportionOfHeight (0.4231f)) + y,
                                             false));
        g.fillRect (x, y, width, height);
    }

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void VASTSamplerNoiseComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    m_uSamplerRootKey->setBounds (proportionOfWidth (0.0168f), proportionOfHeight (0.5462f), proportionOfWidth (0.0746f), proportionOfHeight (0.3077f));
    m_fNoiseGain->setBounds (proportionOfWidth (0.5504f), proportionOfHeight (0.1231f), proportionOfWidth (0.0746f), proportionOfHeight (0.3077f));
    label4->setBounds (proportionOfWidth (0.5616f), proportionOfHeight (0.0000f), proportionOfWidth (0.1530f), proportionOfHeight (0.1231f));
    label2->setBounds (proportionOfWidth (0.0317f), proportionOfHeight (0.0000f), proportionOfWidth (0.1530f), proportionOfHeight (0.1231f));
    m_bSamplerOnOff->setBounds (proportionOfWidth (0.0000f), proportionOfHeight (0.0000f), proportionOfWidth (0.0336f), proportionOfHeight (0.1385f));
    label12->setBounds (proportionOfWidth (0.5224f), proportionOfHeight (0.4231f), proportionOfWidth (0.1287f), proportionOfHeight (0.0923f));
    label7->setBounds (proportionOfWidth (-0.0112f), proportionOfHeight (0.4231f), proportionOfWidth (0.1287f), proportionOfHeight (0.0923f));
    label13->setBounds (proportionOfWidth (0.0690f), proportionOfHeight (0.4231f), proportionOfWidth (0.1287f), proportionOfHeight (0.0923f));
    label14->setBounds (proportionOfWidth (-0.0093f), proportionOfHeight (0.8461f), proportionOfWidth (0.1287f), proportionOfHeight (0.0923f));
    label15->setBounds (proportionOfWidth (0.3918f), proportionOfHeight (0.6615f), proportionOfWidth (0.1287f), proportionOfHeight (0.0923f));
    label16->setBounds (proportionOfWidth (0.3918f), proportionOfHeight (0.1692f), proportionOfWidth (0.1287f), proportionOfHeight (0.0923f));
    label17->setBounds (proportionOfWidth (0.6866f), proportionOfHeight (0.1692f), proportionOfWidth (0.1287f), proportionOfHeight (0.0923f));
    label5->setBounds (proportionOfWidth (0.7463f), proportionOfHeight (0.8461f), proportionOfWidth (0.1287f), proportionOfHeight (0.0923f));
    m_fNoisePan->setBounds (proportionOfWidth (0.6287f), proportionOfHeight (0.1231f), proportionOfWidth (0.0746f), proportionOfHeight (0.3077f));
    label6->setBounds (proportionOfWidth (0.6082f), proportionOfHeight (0.4231f), proportionOfWidth (0.1287f), proportionOfHeight (0.0923f));
    m_fNoiseLowCut->setBounds (proportionOfWidth (0.5504f), proportionOfHeight (0.5462f), proportionOfWidth (0.0746f), proportionOfHeight (0.3077f));
    label8->setBounds (proportionOfWidth (0.5187f), proportionOfHeight (0.8461f), proportionOfWidth (0.1287f), proportionOfHeight (0.0923f));
    label9->setBounds (proportionOfWidth (0.1101f), proportionOfHeight (0.0462f), proportionOfWidth (0.0448f), proportionOfHeight (0.0077f));
    label10->setBounds (proportionOfWidth (0.2892f), proportionOfHeight (0.0154f), proportionOfWidth (0.1287f), proportionOfHeight (0.0923f));
    label11->setBounds (proportionOfWidth (0.1418f), proportionOfHeight (0.0154f), proportionOfWidth (0.1287f), proportionOfHeight (0.0923f));
    label18->setBounds (proportionOfWidth (0.7649f), proportionOfHeight (0.0154f), proportionOfWidth (0.1287f), proportionOfHeight (0.0923f));
    label19->setBounds (proportionOfWidth (0.6381f), proportionOfHeight (0.0154f), proportionOfWidth (0.1194f), proportionOfHeight (0.0923f));
    label20->setBounds (proportionOfWidth (0.3918f), proportionOfHeight (0.5385f), proportionOfWidth (0.1287f), proportionOfHeight (0.0923f));
    m_uSamplerRouting2->setBounds (proportionOfWidth (0.4142f), proportionOfHeight (0.0154f), proportionOfWidth (0.1026f), proportionOfHeight (0.1077f));
    m_uSamplerRouting1->setBounds (proportionOfWidth (0.2687f), proportionOfHeight (0.0154f), proportionOfWidth (0.1026f), proportionOfHeight (0.1077f));
    m_bNoiseOnOff->setBounds (proportionOfWidth (0.5299f), proportionOfHeight (0.0000f), proportionOfWidth (0.0336f), proportionOfHeight (0.1385f));
    m_uNoiseRouting1->setBounds (proportionOfWidth (0.7556f), proportionOfHeight (0.0154f), proportionOfWidth (0.1026f), proportionOfHeight (0.1077f));
    m_fSamplerGain->setBounds (proportionOfWidth (0.0168f), proportionOfHeight (0.1231f), proportionOfWidth (0.0746f), proportionOfHeight (0.3077f));
    m_fSamplerPan->setBounds (proportionOfWidth (0.0952f), proportionOfHeight (0.1231f), proportionOfWidth (0.0746f), proportionOfHeight (0.3077f));
    m_bSamplerPoly->setBounds (proportionOfWidth (0.3601f), proportionOfHeight (0.5154f), proportionOfWidth (0.0336f), proportionOfHeight (0.1385f));
    m_bSamplerKeytrack->setBounds (proportionOfWidth (0.3601f), proportionOfHeight (0.6385f), proportionOfWidth (0.0336f), proportionOfHeight (0.1385f));
    m_bSamplerLoop->setBounds (proportionOfWidth (0.3601f), proportionOfHeight (0.1462f), proportionOfWidth (0.0336f), proportionOfHeight (0.1385f));
    m_uVCAEnv_Sampler->setBounds (proportionOfWidth (0.3974f), proportionOfHeight (0.8385f), proportionOfWidth (0.1194f), proportionOfHeight (0.1077f));
    m_uVCAEnv_Noise->setBounds (proportionOfWidth (0.8713f), proportionOfHeight (0.8308f), proportionOfWidth (0.1194f), proportionOfHeight (0.1077f));
    m_uNoiseType->setBounds (proportionOfWidth (0.8134f), proportionOfHeight (0.1538f), proportionOfWidth (0.1791f), proportionOfHeight (0.1077f));
    m_uNoiseRouting2->setBounds (proportionOfWidth (0.8899f), proportionOfHeight (0.0154f), proportionOfWidth (0.1026f), proportionOfHeight (0.1077f));
    label3->setBounds (proportionOfWidth (0.2742f), proportionOfHeight (0.8461f), proportionOfWidth (0.1287f), proportionOfHeight (0.0923f));
    m_fSamplerCents->setBounds (proportionOfWidth (0.0952f), proportionOfHeight (0.5462f), proportionOfWidth (0.0746f), proportionOfHeight (0.3077f));
    label21->setBounds (proportionOfWidth (0.0672f), proportionOfHeight (0.8461f), proportionOfWidth (0.1287f), proportionOfHeight (0.0923f));
    label22->setBounds (proportionOfWidth (0.3918f), proportionOfHeight (0.2923f), proportionOfWidth (0.1287f), proportionOfHeight (0.0923f));
    m_bSamplerLoopOnly->setBounds (proportionOfWidth (0.3601f), proportionOfHeight (0.2692f), proportionOfWidth (0.0336f), proportionOfHeight (0.1385f));
    label23->setBounds (proportionOfWidth (0.3918f), proportionOfHeight (0.4154f), proportionOfWidth (0.1287f), proportionOfHeight (0.0923f));
    m_bSamplerLoopPingpong->setBounds (proportionOfWidth (0.3601f), proportionOfHeight (0.3923f), proportionOfWidth (0.0336f), proportionOfHeight (0.1385f));
    m_fSamplerLoopRange->setBounds (proportionOfWidth (0.1754f), proportionOfHeight (0.5462f), proportionOfWidth (0.0746f), proportionOfHeight (0.3077f));
    label25->setBounds (proportionOfWidth (0.1530f), proportionOfHeight (0.8461f), proportionOfWidth (0.1287f), proportionOfHeight (0.0923f));
    m_fNoiseResonance->setBounds (proportionOfWidth (0.7146f), proportionOfHeight (0.5462f), proportionOfWidth (0.0746f), proportionOfHeight (0.3077f));
    label26->setBounds (proportionOfWidth (0.6847f), proportionOfHeight (0.8461f), proportionOfWidth (0.1287f), proportionOfHeight (0.0923f));
    m_fNoiseHighCut->setBounds (proportionOfWidth (0.6287f), proportionOfHeight (0.5462f), proportionOfWidth (0.0746f), proportionOfHeight (0.3077f));
    label24->setBounds (proportionOfWidth (0.6082f), proportionOfHeight (0.8461f), proportionOfWidth (0.1287f), proportionOfHeight (0.0923f));
    m_fSamplerGrainInteronset->setBounds (proportionOfWidth (0.1754f), proportionOfHeight (0.1231f), proportionOfWidth (0.0746f), proportionOfHeight (0.3077f));
    label27->setBounds (proportionOfWidth (0.1511f), proportionOfHeight (0.4231f), proportionOfWidth (0.1287f), proportionOfHeight (0.0846f));
    m_fSamplerGrainRandom->setBounds (proportionOfWidth (0.2556f), proportionOfHeight (0.1231f), proportionOfWidth (0.0746f), proportionOfHeight (0.3077f));
    label28->setBounds (proportionOfWidth (0.2295f), proportionOfHeight (0.4231f), proportionOfWidth (0.1287f), proportionOfHeight (0.0846f));
    label29->setBounds (proportionOfWidth (0.2332f), proportionOfHeight (0.8461f), proportionOfWidth (0.1287f), proportionOfHeight (0.0923f));
    m_fSamplerGrainShape->setBounds (proportionOfWidth (0.2556f), proportionOfHeight (0.5462f), proportionOfWidth (0.0746f), proportionOfHeight (0.3077f));
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void VASTSamplerNoiseComponent::sliderValueChanged (juce::Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == m_uSamplerRootKey.get())
    {
        //[UserSliderCode_m_uSamplerRootKey] -- add your slider handling code here..
        //[/UserSliderCode_m_uSamplerRootKey]
    }
    else if (sliderThatWasMoved == m_fNoiseGain.get())
    {
        //[UserSliderCode_m_fNoiseGain] -- add your slider handling code here..
        //[/UserSliderCode_m_fNoiseGain]
    }
    else if (sliderThatWasMoved == m_fNoisePan.get())
    {
        //[UserSliderCode_m_fNoisePan] -- add your slider handling code here..
        //[/UserSliderCode_m_fNoisePan]
    }
    else if (sliderThatWasMoved == m_fNoiseLowCut.get())
    {
        //[UserSliderCode_m_fNoiseLowCut] -- add your slider handling code here..
        //[/UserSliderCode_m_fNoiseLowCut]
    }
    else if (sliderThatWasMoved == m_fSamplerGain.get())
    {
        //[UserSliderCode_m_fSamplerGain] -- add your slider handling code here..
        //[/UserSliderCode_m_fSamplerGain]
    }
    else if (sliderThatWasMoved == m_fSamplerPan.get())
    {
        //[UserSliderCode_m_fSamplerPan] -- add your slider handling code here..
        //[/UserSliderCode_m_fSamplerPan]
    }
    else if (sliderThatWasMoved == m_fSamplerCents.get())
    {
        //[UserSliderCode_m_fSamplerCents] -- add your slider handling code here..
        //[/UserSliderCode_m_fSamplerCents]
    }
    else if (sliderThatWasMoved == m_fSamplerLoopRange.get())
    {
        //[UserSliderCode_m_fSamplerLoopRange] -- add your slider handling code here..
        //[/UserSliderCode_m_fSamplerLoopRange]
    }
    else if (sliderThatWasMoved == m_fNoiseResonance.get())
    {
        //[UserSliderCode_m_fNoiseResonance] -- add your slider handling code here..
        //[/UserSliderCode_m_fNoiseResonance]
    }
    else if (sliderThatWasMoved == m_fNoiseHighCut.get())
    {
        //[UserSliderCode_m_fNoiseHighCut] -- add your slider handling code here..
        //[/UserSliderCode_m_fNoiseHighCut]
    }
    else if (sliderThatWasMoved == m_fSamplerGrainInteronset.get())
    {
        //[UserSliderCode_m_fSamplerGrainInteronset] -- add your slider handling code here..
        //[/UserSliderCode_m_fSamplerGrainInteronset]
    }
    else if (sliderThatWasMoved == m_fSamplerGrainRandom.get())
    {
        //[UserSliderCode_m_fSamplerGrainRandom] -- add your slider handling code here..
        //[/UserSliderCode_m_fSamplerGrainRandom]
    }
    else if (sliderThatWasMoved == m_fSamplerGrainShape.get())
    {
        //[UserSliderCode_m_fSamplerGrainShape] -- add your slider handling code here..
        //[/UserSliderCode_m_fSamplerGrainShape]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void VASTSamplerNoiseComponent::buttonClicked (juce::Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == m_bSamplerOnOff.get())
    {
        //[UserButtonCode_m_bSamplerOnOff] -- add your button handler code here..
        //[/UserButtonCode_m_bSamplerOnOff]
    }
    else if (buttonThatWasClicked == m_bNoiseOnOff.get())
    {
        //[UserButtonCode_m_bNoiseOnOff] -- add your button handler code here..
        //[/UserButtonCode_m_bNoiseOnOff]
    }
    else if (buttonThatWasClicked == m_bSamplerPoly.get())
    {
        //[UserButtonCode_m_bSamplerPoly] -- add your button handler code here..
        //[/UserButtonCode_m_bSamplerPoly]
    }
    else if (buttonThatWasClicked == m_bSamplerKeytrack.get())
    {
        //[UserButtonCode_m_bSamplerKeytrack] -- add your button handler code here..
        //[/UserButtonCode_m_bSamplerKeytrack]
    }
    else if (buttonThatWasClicked == m_bSamplerLoop.get())
    {
        //[UserButtonCode_m_bSamplerLoop] -- add your button handler code here..
        //[/UserButtonCode_m_bSamplerLoop]
    }
    else if (buttonThatWasClicked == m_bSamplerLoopOnly.get())
    {
        //[UserButtonCode_m_bSamplerLoopOnly] -- add your button handler code here..
        //[/UserButtonCode_m_bSamplerLoopOnly]
    }
    else if (buttonThatWasClicked == m_bSamplerLoopPingpong.get())
    {
        //[UserButtonCode_m_bSamplerLoopPingpong] -- add your button handler code here..
        //[/UserButtonCode_m_bSamplerLoopPingpong]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void VASTSamplerNoiseComponent::comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == m_uSamplerRouting2.get())
    {
        //[UserComboBoxCode_m_uSamplerRouting2] -- add your combo box handling code here..
        //[/UserComboBoxCode_m_uSamplerRouting2]
    }
    else if (comboBoxThatHasChanged == m_uSamplerRouting1.get())
    {
        //[UserComboBoxCode_m_uSamplerRouting1] -- add your combo box handling code here..
        //[/UserComboBoxCode_m_uSamplerRouting1]
    }
    else if (comboBoxThatHasChanged == m_uNoiseRouting1.get())
    {
        //[UserComboBoxCode_m_uNoiseRouting1] -- add your combo box handling code here..
        //[/UserComboBoxCode_m_uNoiseRouting1]
    }
    else if (comboBoxThatHasChanged == m_uVCAEnv_Sampler.get())
    {
        //[UserComboBoxCode_m_uVCAEnv_Sampler] -- add your combo box handling code here..
        //[/UserComboBoxCode_m_uVCAEnv_Sampler]
    }
    else if (comboBoxThatHasChanged == m_uVCAEnv_Noise.get())
    {
        //[UserComboBoxCode_m_uVCAEnv_Noise] -- add your combo box handling code here..
        //[/UserComboBoxCode_m_uVCAEnv_Noise]
    }
    else if (comboBoxThatHasChanged == m_uNoiseType.get())
    {
        //[UserComboBoxCode_m_uNoiseType] -- add your combo box handling code here..
        //[/UserComboBoxCode_m_uNoiseType]
    }
    else if (comboBoxThatHasChanged == m_uNoiseRouting2.get())
    {
        //[UserComboBoxCode_m_uNoiseRouting2] -- add your combo box handling code here..
        //[/UserComboBoxCode_m_uNoiseRouting2]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="VASTSamplerNoiseComponent"
                 componentName="" parentClasses="public Component" constructorParams="AudioProcessorEditor *editor, AudioProcessor* processor"
                 variableInitialisers="myEditor((VASTAudioProcessorEditor*)editor), myProcessor((VASTAudioProcessor*)processor)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="536" initialHeight="130">
  <BACKGROUND backgroundColour="ff000000">
    <RECT pos="0% 0% 52.799% 100%" fill="linear: -1.127% -2.927%, 101.493% 104.615%, 0=ff56646c, 1=ff232527"
          hasStroke="0"/>
    <RECT pos="1 1 52.612% 12.308%" fill="linear: -1.127% -2.927%, 52.239% 45.455%, 0=ff3f4446, 1=ff4c5457"
          hasStroke="0"/>
    <RECT pos="0Rr 0% 47.201% 100%" fill="linear: 50.746% 0%, 129.851% 116.923%, 0=ff56646c, 1=ff232527"
          hasStroke="0"/>
    <RECT pos="0Rr 1 47.201% 12.308%" fill="linear: 50.746% -9.091%, 104.478% 45.455%, 0=ff3f4446, 1=ff4c5457"
          hasStroke="0"/>
    <RECT pos="0.746% 85.385% 38.806% 8.462%" fill="linear: 0% 80%, 26.866% 92.308%, 0=ff3a4145, 1=ff33393c"
          hasStroke="0"/>
    <RECT pos="53.731% 85.385% 24.813% 8.462%" fill="linear: 52.239% 80%, 80.597% 98.462%, 0=ff3a4145, 1=ff33393c"
          hasStroke="0"/>
    <RECT pos="0.8% 42.308% 32.8% 8.462%" fill="linear: 0% 36.923%, 17.91% 55.385%, 0=ff3a4145, 1=ff33393c"
          hasStroke="0"/>
    <RECT pos="53.731% 42.308% 16.418% 8.462%" fill="linear: 73.134% 55.385%, 53.731% 36.923%, 0=ff3a4145, 1=ff33393c"
          hasStroke="0"/>
  </BACKGROUND>
  <SLIDER name="m_uSamplerRootKey" id="ba7db9f8eda22687" memberName="m_uSamplerRootKey"
          virtualName="VASTParameterSlider" explicitFocusOrder="0" pos="1.679% 54.615% 7.463% 30.769%"
          tooltip="Sample root key" textboxtext="ffc9c9c9" textboxbkgd="ffffff"
          textboxoutline="808080" min="0.0" max="127.0" int="0.01" style="RotaryVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="30"
          textBoxHeight="14" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="m_fNoiseGain" id="d292cebda6f4790" memberName="m_fNoiseGain"
          virtualName="VASTParameterSlider" explicitFocusOrder="0" pos="55.037% 12.308% 7.463% 30.769%"
          tooltip="Noise gain (100 full, &gt;100 overdrive)" textboxtext="ffc9c9c9"
          textboxbkgd="ffffff" textboxoutline="808080" min="0.0" max="200.0"
          int="0.01" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="30" textBoxHeight="14" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="new label" id="f7f752a0137656ad" memberName="label4" virtualName=""
         explicitFocusOrder="0" pos="56.157% 0% 15.299% 12.308%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="NOISE&#10;" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Syntax"
         fontsize="13.0" kerning="0.0" bold="1" italic="0" justification="33"
         typefaceStyle="Bold"/>
  <LABEL name="new label" id="2ca9266a15d215" memberName="label2" virtualName=""
         explicitFocusOrder="0" pos="3.172% 0% 15.299% 12.308%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="SAMPLER" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Syntax"
         fontsize="13.0" kerning="0.0" bold="1" italic="0" justification="33"
         typefaceStyle="Bold"/>
  <TOGGLEBUTTON name="m_bSamplerOnOff" id="7111c76cdff6e560" memberName="m_bSamplerOnOff"
                virtualName="VASTParameterButton" explicitFocusOrder="0" pos="0% 0% 3.358% 13.846%"
                tooltip="Switch sampler on / off" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <LABEL name="new label" id="d2755e2f4cbff0e1" memberName="label12" virtualName=""
         explicitFocusOrder="0" pos="52.239% 42.308% 12.873% 9.231%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="GAIN" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Syntax"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <LABEL name="new label" id="5949ff15abefaf84" memberName="label7" virtualName=""
         explicitFocusOrder="0" pos="-1.119% 42.308% 12.873% 9.231%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="GAIN" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Syntax"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <LABEL name="new label" id="99a528138afc1cfa" memberName="label13" virtualName=""
         explicitFocusOrder="0" pos="6.903% 42.308% 12.873% 9.231%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="PAN" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Syntax"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <LABEL name="new label" id="befb37f7cfb402fc" memberName="label14" virtualName=""
         explicitFocusOrder="0" pos="-0.933% 84.615% 12.873% 9.231%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="ROOT" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Syntax"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <LABEL name="new label" id="91985a39bf01aa31" memberName="label15" virtualName=""
         explicitFocusOrder="0" pos="39.179% 66.154% 12.873% 9.231%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="KEYTRACK" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Syntax"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="76b4c39b2e8d1219" memberName="label16" virtualName=""
         explicitFocusOrder="0" pos="39.179% 16.923% 12.873% 9.231%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="LOOP" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Syntax"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="75c7c2a88e5fe84c" memberName="label17" virtualName=""
         explicitFocusOrder="0" pos="68.657% 16.923% 12.873% 9.231%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="TYPE" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Syntax"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="34"/>
  <LABEL name="new label" id="656ee062d26c6572" memberName="label5" virtualName=""
         explicitFocusOrder="0" pos="74.627% 84.615% 12.873% 9.231%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="ENV" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Syntax"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="34"/>
  <SLIDER name="m_fNoisePan" id="a9fecf9f85e3f9ff" memberName="m_fNoisePan"
          virtualName="VASTParameterSlider" explicitFocusOrder="0" pos="62.873% 12.308% 7.463% 30.769%"
          tooltip="Stereo panning of noise oscillator" textboxtext="ffc9c9c9"
          textboxbkgd="ffffff" textboxoutline="808080" min="-100.0" max="100.0"
          int="0.01" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="30" textBoxHeight="14" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="new label" id="74b934651c8b4b00" memberName="label6" virtualName=""
         explicitFocusOrder="0" pos="60.821% 42.308% 12.873% 9.231%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="PAN" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Syntax"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <SLIDER name="m_fNoiseLowCut" id="a9bed200d5ebe456" memberName="m_fNoiseLowCut"
          virtualName="VASTParameterSlider" explicitFocusOrder="0" pos="55.037% 54.615% 7.463% 30.769%"
          tooltip="Noise oscillator low frequency cutoff" textboxtext="ffc9c9c9"
          textboxbkgd="ffffff" textboxoutline="808080" min="0.0" max="200.0"
          int="0.01" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="30" textBoxHeight="14" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="new label" id="35320255c0ab1b2d" memberName="label8" virtualName=""
         explicitFocusOrder="0" pos="51.866% 84.615% 12.873% 9.231%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="LOW CUT" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Syntax"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <LABEL name="new label" id="11fc1cb2555e01c" memberName="label9" virtualName=""
         explicitFocusOrder="0" pos="11.007% 4.615% 4.478% 0.769%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="ENV" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Syntax"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="34"/>
  <LABEL name="new label" id="10147451e85e63c6" memberName="label10" virtualName=""
         explicitFocusOrder="0" pos="28.918% 1.538% 12.873% 9.231%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="2" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Syntax"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="34"/>
  <LABEL name="new label" id="9a6abf4d75c47200" memberName="label11" virtualName=""
         explicitFocusOrder="0" pos="14.179% 1.538% 12.873% 9.231%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="1" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Syntax"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="34"/>
  <LABEL name="new label" id="c3245d406fb40568" memberName="label18" virtualName=""
         explicitFocusOrder="0" pos="76.493% 1.538% 12.873% 9.231%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="2" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Syntax"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="34"/>
  <LABEL name="new label" id="554ab5da0022dd89" memberName="label19" virtualName=""
         explicitFocusOrder="0" pos="63.806% 1.538% 11.94% 9.231%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="1" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Syntax"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="34"/>
  <LABEL name="new label" id="a700dbc29793c510" memberName="label20" virtualName=""
         explicitFocusOrder="0" pos="39.179% 53.846% 12.873% 9.231%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="POLY" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Syntax"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <COMBOBOX name="m_uSamplerRouting2" id="5d008205c52feb96" memberName="m_uSamplerRouting2"
            virtualName="VASTParameterComboBox" explicitFocusOrder="0" pos="41.418% 1.538% 10.261% 10.769%"
            tooltip="Sampler routing 2" editable="0" layout="33" items=""
            textWhenNonSelected="" textWhenNoItems="---"/>
  <COMBOBOX name="m_uSamplerRouting1" id="3140ae73e012af62" memberName="m_uSamplerRouting1"
            virtualName="VASTParameterComboBox" explicitFocusOrder="0" pos="26.866% 1.538% 10.261% 10.769%"
            tooltip="Sampler routing 1" editable="0" layout="33" items=""
            textWhenNonSelected="" textWhenNoItems="---"/>
  <TOGGLEBUTTON name="m_bNoiseOnOff" id="da0498fbce59c72b" memberName="m_bNoiseOnOff"
                virtualName="VASTParameterButton" explicitFocusOrder="0" pos="52.985% 0% 3.358% 13.846%"
                tooltip="Switch noise oscillator on / off" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <COMBOBOX name="m_uNoiseRouting1" id="c4f75fd447cf8661" memberName="m_uNoiseRouting1"
            virtualName="VASTParameterComboBox" explicitFocusOrder="0" pos="75.56% 1.538% 10.261% 10.769%"
            tooltip="Noise routing 1" editable="0" layout="33" items="" textWhenNonSelected=""
            textWhenNoItems="---"/>
  <SLIDER name="m_fSamplerGain" id="371d831fe853b076" memberName="m_fSamplerGain"
          virtualName="VASTParameterSlider" explicitFocusOrder="0" pos="1.679% 12.308% 7.463% 30.769%"
          tooltip="Sampler gain (100 full, &gt;100 overdrive)" textboxtext="ffc9c9c9"
          textboxbkgd="ffffff" textboxoutline="808080" min="0.0" max="200.0"
          int="0.01" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="30" textBoxHeight="14" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="m_fSamplerPan" id="b1848df78b165a75" memberName="m_fSamplerPan"
          virtualName="VASTParameterSlider" explicitFocusOrder="0" pos="9.515% 12.308% 7.463% 30.769%"
          tooltip="Stereo panning of sampler" textboxtext="ffc9c9c9" textboxbkgd="ffffff"
          textboxoutline="808080" min="-100.0" max="100.0" int="0.01" style="RotaryVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="30"
          textBoxHeight="14" skewFactor="1.0" needsCallback="1"/>
  <TOGGLEBUTTON name="m_bSamplerPoly" id="647dd3236163f512" memberName="m_bSamplerPoly"
                virtualName="VASTParameterButton" explicitFocusOrder="0" pos="36.007% 51.538% 3.358% 13.846%"
                tooltip="Play sample in polyphonic or monophonic mode" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="m_bSamplerKeytrack" id="2c55c34f9f661244" memberName="m_bSamplerKeytrack"
                virtualName="VASTParameterButton" explicitFocusOrder="0" pos="36.007% 63.846% 3.358% 13.846%"
                tooltip="Sample frequencyis kextracked or static" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="m_bSamplerLoop" id="36a3c26c661e7327" memberName="m_bSamplerLoop"
                virtualName="VASTParameterButton" explicitFocusOrder="0" pos="36.007% 14.615% 3.358% 13.846%"
                tooltip="Loop sample" buttonText="" connectedEdges="0" needsCallback="1"
                radioGroupId="0" state="0"/>
  <COMBOBOX name="m_uVCAEnv_Sampler" id="9c9de616db70e4d3" memberName="m_uVCAEnv_Sampler"
            virtualName="VASTParameterComboBox" explicitFocusOrder="0" pos="39.739% 83.846% 11.94% 10.769%"
            tooltip="Envelope used as sampler envelope" editable="0" layout="33"
            items="" textWhenNonSelected="" textWhenNoItems="---"/>
  <COMBOBOX name="m_uVCAEnv_Noise" id="5a1165970dadc00b" memberName="m_uVCAEnv_Noise"
            virtualName="VASTParameterComboBox" explicitFocusOrder="0" pos="87.127% 83.077% 11.94% 10.769%"
            tooltip="Envelope used as noise envelope" editable="0" layout="33"
            items="" textWhenNonSelected="" textWhenNoItems="---"/>
  <COMBOBOX name="m_uNoiseType" id="79c1fd6a0b6bb01a" memberName="m_uNoiseType"
            virtualName="VASTParameterComboBox" explicitFocusOrder="0" pos="81.343% 15.385% 17.91% 10.769%"
            tooltip="Noise oscillator type" editable="0" layout="33" items=""
            textWhenNonSelected="" textWhenNoItems="---"/>
  <COMBOBOX name="m_uNoiseRouting2" id="38371de209f59a17" memberName="m_uNoiseRouting2"
            virtualName="VASTParameterComboBox" explicitFocusOrder="0" pos="88.993% 1.538% 10.261% 10.769%"
            tooltip="Noise routing 2" editable="0" layout="33" items="" textWhenNonSelected=""
            textWhenNoItems="---"/>
  <LABEL name="new label" id="eac7883133563094" memberName="label3" virtualName=""
         explicitFocusOrder="0" pos="27.425% 84.615% 12.873% 9.231%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="ENV" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Syntax"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="34"/>
  <SLIDER name="m_fSamplerCents" id="d9acce403cfe3a38" memberName="m_fSamplerCents"
          virtualName="VASTParameterSlider" explicitFocusOrder="0" pos="9.515% 54.615% 7.463% 30.769%"
          tooltip="Sampler cents (+/- 1 octave and half tones)" textboxtext="ffc9c9c9"
          textboxbkgd="ffffff" textboxoutline="808080" min="0.0" max="127.0"
          int="0.01" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="30" textBoxHeight="14" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="new label" id="519c528bb6635879" memberName="label21" virtualName=""
         explicitFocusOrder="0" pos="6.716% 84.615% 12.873% 9.231%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="CENTS" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Syntax"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <LABEL name="new label" id="39dc80864c375f6b" memberName="label22" virtualName=""
         explicitFocusOrder="0" pos="39.179% 29.231% 12.873% 9.231%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="ONLY" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Syntax"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <TOGGLEBUTTON name="m_bSamplerLoopOnly" id="76043fcbdeb95ca9" memberName="m_bSamplerLoopOnly"
                virtualName="VASTParameterButton" explicitFocusOrder="0" pos="36.007% 26.923% 3.358% 13.846%"
                tooltip="Play only sample loop without start" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <LABEL name="new label" id="67cb22aee6d12fd6" memberName="label23" virtualName=""
         explicitFocusOrder="0" pos="39.179% 41.538% 12.873% 9.231%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="PINGPONG" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Syntax"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <TOGGLEBUTTON name="m_bSamplerLoopPingpong" id="db47c113ac69a26d" memberName="m_bSamplerLoopPingpong"
                virtualName="VASTParameterButton" explicitFocusOrder="0" pos="36.007% 39.231% 3.358% 13.846%"
                tooltip="Play sample loop in pingpong mode" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <SLIDER name="m_fSamplerLoopRange" id="1ef725339a9fe90c" memberName="m_fSamplerLoopRange"
          virtualName="VASTParameterSlider" explicitFocusOrder="0" pos="17.537% 54.615% 7.463% 30.769%"
          tooltip="Sampler loop range" textboxtext="ffc9c9c9" textboxbkgd="ffffff"
          textboxoutline="808080" min="1.0" max="100.0" int="0.001" style="RotaryVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="30"
          textBoxHeight="14" skewFactor="1.0" needsCallback="1"/>
  <LABEL name="new label" id="dded717c5085636b" memberName="label25" virtualName=""
         explicitFocusOrder="0" pos="15.299% 84.615% 12.873% 9.231%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="RANGE" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Syntax"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <SLIDER name="m_fNoiseResonance" id="dd9e0cf25296c1b3" memberName="m_fNoiseResonance"
          virtualName="VASTParameterSlider" explicitFocusOrder="0" pos="71.455% 54.615% 7.463% 30.769%"
          tooltip="Noise resonance" textboxtext="ffc9c9c9" textboxbkgd="ffffff"
          textboxoutline="808080" min="1.0" max="100.0" int="0.001" style="RotaryVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="30"
          textBoxHeight="14" skewFactor="1.0" needsCallback="1"/>
  <LABEL name="new label" id="4d5c17eee7119d02" memberName="label26" virtualName=""
         explicitFocusOrder="0" pos="68.47% 84.615% 12.873% 9.231%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="RESO" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Syntax"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <SLIDER name="m_fNoiseHighCut" id="7dfd2a29e08a5b5" memberName="m_fNoiseHighCut"
          virtualName="VASTParameterSlider" explicitFocusOrder="0" pos="62.873% 54.615% 7.463% 30.769%"
          tooltip="Noise oscillator high frequency cutoff" textboxtext="ffc9c9c9"
          textboxbkgd="ffffff" textboxoutline="808080" min="0.0" max="200.0"
          int="0.01" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="30" textBoxHeight="14" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="new label" id="a6f1e1da4e594217" memberName="label24" virtualName=""
         explicitFocusOrder="0" pos="60.821% 84.615% 12.873% 9.231%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="HI CUT" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Syntax"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <SLIDER name="m_fSamplerGrainInteronset" id="df67dadbcdeb5e52" memberName="m_fSamplerGrainInteronset"
          virtualName="VASTParameterSlider" explicitFocusOrder="0" pos="17.537% 12.308% 7.463% 30.769%"
          tooltip="Sampler grain inter-onset (ms)" textboxtext="ffc9c9c9"
          textboxbkgd="ffffff" textboxoutline="808080" min="1.0" max="100.0"
          int="0.001" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="30" textBoxHeight="14" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="new label" id="87e911cca6bf6314" memberName="label27" virtualName=""
         explicitFocusOrder="0" pos="15.112% 42.308% 12.873% 8.462%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="GRAIN" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Syntax"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <SLIDER name="m_fSamplerGrainRandom" id="1896824eb4d642b9" memberName="m_fSamplerGrainRandom"
          virtualName="VASTParameterSlider" explicitFocusOrder="0" pos="25.56% 12.308% 7.463% 30.769%"
          tooltip="Sampler grain inter-onset and detune random" textboxtext="ffc9c9c9"
          textboxbkgd="ffffff" textboxoutline="808080" min="1.0" max="100.0"
          int="0.001" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="30" textBoxHeight="14" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="new label" id="78e7a9f7a1884460" memberName="label28" virtualName=""
         explicitFocusOrder="0" pos="22.948% 42.308% 12.873% 8.462%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="RND" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Syntax"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <LABEL name="new label" id="f7bfdea80540da12" memberName="label29" virtualName=""
         explicitFocusOrder="0" pos="23.321% 84.615% 12.873% 9.231%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="SHAPE" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Syntax"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <SLIDER name="m_fSamplerGrainShape" id="a7295e9603827fc2" memberName="m_fSamplerGrainShape"
          virtualName="VASTParameterSlider" explicitFocusOrder="0" pos="25.56% 54.615% 7.463% 30.769%"
          tooltip="Sampler grain shape" textboxtext="ffc9c9c9" textboxbkgd="ffffff"
          textboxoutline="808080" min="1.0" max="100.0" int="0.001" style="RotaryVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="30"
          textBoxHeight="14" skewFactor="1.0" needsCallback="1"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

