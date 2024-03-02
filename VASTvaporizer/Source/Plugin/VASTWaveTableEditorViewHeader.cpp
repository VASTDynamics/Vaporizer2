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
#include "../Engine/VASTSampler.h"
#include "VASTControls/VASTButton.h"
#include "VASTControls/VASTSlider.h"
#include "VASTScopeDisplay/VASTOscilloscope.h"
#include "VASTWTEditor/VASTFreqDomainViewport.h"
#include "VASTWTEditor/VASTPositionViewport.h"
#include "../Engine/Oscillator/VASTWaveTable.h"
#include "VASTSamplerEditorComponentHeader.h"
#include <thread>
//[/Headers]

#include "VASTWaveTableEditorViewHeader.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
VASTWaveTableEditorViewHeader::VASTWaveTableEditorViewHeader (AudioProcessorEditor *editor, AudioProcessor* processor, VASTWaveTableEditorComponent *wteditor )
    : myEditor((VASTAudioProcessorEditor*)editor), myProcessor((VASTAudioProcessor*)processor), myWTEditor(wteditor)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    label10.reset (new juce::Label ("new label",
                                    TRANS ("OSC D\n")));
    addAndMakeVisible (label10.get());
    label10->setFont (juce::Font ("Code Pro Demo", 11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label10->setJustificationType (juce::Justification::centredLeft);
    label10->setEditable (false, false, false);
    label10->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label10->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label10->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label9.reset (new juce::Label ("new label",
                                   TRANS ("OSC C")));
    addAndMakeVisible (label9.get());
    label9->setFont (juce::Font ("Code Pro Demo", 11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label9->setJustificationType (juce::Justification::centredLeft);
    label9->setEditable (false, false, false);
    label9->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label9->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label9->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label7.reset (new juce::Label ("new label",
                                   TRANS ("OSC A")));
    addAndMakeVisible (label7.get());
    label7->setFont (juce::Font ("Code Pro Demo", 11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label7->setJustificationType (juce::Justification::centredLeft);
    label7->setEditable (false, false, false);
    label7->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label7->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label7->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label8.reset (new juce::Label ("new label",
                                   TRANS ("OSC B")));
    addAndMakeVisible (label8.get());
    label8->setFont (juce::Font ("Code Pro Demo", 11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label8->setJustificationType (juce::Justification::centredLeft);
    label8->setEditable (false, false, false);
    label8->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label8->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label8->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    c_Phase.reset (new VASTSlider ("c_Phase"));
    addAndMakeVisible (c_Phase.get());
    c_Phase->setTooltip (TRANS ("Change phase of selection"));
    c_Phase->setRange (-100, 100, 0.001);
    c_Phase->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    c_Phase->setTextBoxStyle (juce::Slider::NoTextBox, false, 30, 14);
    c_Phase->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffc9c9c9));
    c_Phase->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0x00ffffff));
    c_Phase->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x00808080));

    label108.reset (new juce::Label ("new label",
                                     TRANS ("LEVEL")));
    addAndMakeVisible (label108.get());
    label108->setFont (juce::Font ("Code Pro Demo", 11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label108->setJustificationType (juce::Justification::centred);
    label108->setEditable (false, false, false);
    label108->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label108->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label108->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label135.reset (new juce::Label ("new label",
                                     TRANS ("PHASE")));
    addAndMakeVisible (label135.get());
    label135->setFont (juce::Font ("Code Pro Demo", 11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label135->setJustificationType (juce::Justification::centred);
    label135->setEditable (false, false, false);
    label135->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label135->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label135->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    c_Freq.reset (new VASTSlider ("c_Freq"));
    addAndMakeVisible (c_Freq.get());
    c_Freq->setTooltip (TRANS ("Change frequency of selection"));
    c_Freq->setRange (-4, 4, 0.001);
    c_Freq->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    c_Freq->setTextBoxStyle (juce::Slider::NoTextBox, false, 30, 14);
    c_Freq->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffc9c9c9));
    c_Freq->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0x00ffffff));
    c_Freq->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x00808080));

    label2.reset (new juce::Label ("new label",
                                   TRANS ("FREQ")));
    addAndMakeVisible (label2.get());
    label2->setFont (juce::Font ("Code Pro Demo", 11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label2->setJustificationType (juce::Justification::centred);
    label2->setEditable (false, false, false);
    label2->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label2->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label2->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    c_Offset.reset (new VASTSlider ("c_Offset"));
    addAndMakeVisible (c_Offset.get());
    c_Offset->setTooltip (TRANS ("Change DC offset of selection"));
    c_Offset->setRange (-100, 100, 0.001);
    c_Offset->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    c_Offset->setTextBoxStyle (juce::Slider::NoTextBox, false, 30, 14);
    c_Offset->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffc9c9c9));
    c_Offset->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0x00ffffff));
    c_Offset->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x00808080));

    c_bitDepth.reset (new VASTSlider ("c_bitDepth"));
    addAndMakeVisible (c_bitDepth.get());
    c_bitDepth->setTooltip (TRANS ("Change bit depth of selection"));
    c_bitDepth->setRange (1, 32, 1);
    c_bitDepth->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    c_bitDepth->setTextBoxStyle (juce::Slider::NoTextBox, false, 30, 14);
    c_bitDepth->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffc9c9c9));
    c_bitDepth->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0x00ffffff));
    c_bitDepth->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x00808080));

    label3.reset (new juce::Label ("new label",
                                   TRANS ("BITS")));
    addAndMakeVisible (label3.get());
    label3->setFont (juce::Font ("Code Pro Demo", 11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label3->setJustificationType (juce::Justification::centred);
    label3->setEditable (false, false, false);
    label3->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label3->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label3->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label4.reset (new juce::Label ("new label",
                                   TRANS ("OFFSET")));
    addAndMakeVisible (label4.get());
    label4->setFont (juce::Font ("Code Pro Demo", 11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label4->setJustificationType (juce::Justification::centred);
    label4->setEditable (false, false, false);
    label4->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label4->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label4->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    c_Smooth.reset (new VASTSlider ("c_Smooth"));
    addAndMakeVisible (c_Smooth.get());
    c_Smooth->setTooltip (TRANS ("Smooth selection"));
    c_Smooth->setRange (0, 100, 0.001);
    c_Smooth->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    c_Smooth->setTextBoxStyle (juce::Slider::NoTextBox, false, 30, 14);
    c_Smooth->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffc9c9c9));
    c_Smooth->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0x00ffffff));
    c_Smooth->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x00808080));

    label5.reset (new juce::Label ("new label",
                                   TRANS ("SMOOTH")));
    addAndMakeVisible (label5.get());
    label5->setFont (juce::Font ("Code Pro Demo", 11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label5->setJustificationType (juce::Justification::centred);
    label5->setEditable (false, false, false);
    label5->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label5->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label5->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    c_Clean.reset (new VASTSlider ("c_Clean"));
    addAndMakeVisible (c_Clean.get());
    c_Clean->setTooltip (TRANS ("Clean selection (start and end of cycle)"));
    c_Clean->setRange (0, 100, 0.001);
    c_Clean->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    c_Clean->setTextBoxStyle (juce::Slider::NoTextBox, false, 30, 14);
    c_Clean->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffc9c9c9));
    c_Clean->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0x00ffffff));
    c_Clean->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x00808080));

    label6.reset (new juce::Label ("new label",
                                   TRANS ("CLEAN")));
    addAndMakeVisible (label6.get());
    label6->setFont (juce::Font ("Code Pro Demo", 11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label6->setJustificationType (juce::Justification::centred);
    label6->setEditable (false, false, false);
    label6->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label6->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label6->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    c_oscA.reset (new juce::ToggleButton ("c_oscA"));
    addAndMakeVisible (c_oscA.get());
    c_oscA->setButtonText (juce::String());
    c_oscA->setConnectedEdges (juce::Button::ConnectedOnBottom);
    c_oscA->setRadioGroupId (11);

    c_oscB.reset (new juce::ToggleButton ("c_oscB"));
    addAndMakeVisible (c_oscB.get());
    c_oscB->setButtonText (juce::String());
    c_oscB->setConnectedEdges (juce::Button::ConnectedOnTop | juce::Button::ConnectedOnBottom);
    c_oscB->setRadioGroupId (11);

    c_oscC.reset (new juce::ToggleButton ("c_oscC"));
    addAndMakeVisible (c_oscC.get());
    c_oscC->setButtonText (juce::String());
    c_oscC->setConnectedEdges (juce::Button::ConnectedOnTop | juce::Button::ConnectedOnBottom);
    c_oscC->setRadioGroupId (11);

    c_oscD.reset (new juce::ToggleButton ("c_oscD"));
    addAndMakeVisible (c_oscD.get());
    c_oscD->setButtonText (juce::String());
    c_oscD->setConnectedEdges (juce::Button::ConnectedOnTop);
    c_oscD->setRadioGroupId (11);

    m_iWTEditorZoom.reset (new VASTParameterSlider ("m_iWTEditorZoom"));
    addAndMakeVisible (m_iWTEditorZoom.get());
    m_iWTEditorZoom->setTooltip (TRANS ("Zoom frequency domain display"));
    m_iWTEditorZoom->setExplicitFocusOrder (1);
    m_iWTEditorZoom->setRange (1, 16, 1);
    m_iWTEditorZoom->setSliderStyle (juce::Slider::IncDecButtons);
    m_iWTEditorZoom->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 28, 20);

    c_wtName.reset (new juce::TextEditor ("c_wtName "));
    addAndMakeVisible (c_wtName.get());
    c_wtName->setMultiLine (false);
    c_wtName->setReturnKeyStartsNewLine (false);
    c_wtName->setReadOnly (false);
    c_wtName->setScrollbarsShown (false);
    c_wtName->setCaretVisible (true);
    c_wtName->setPopupMenuEnabled (true);
    c_wtName->setText (juce::String());

    label13.reset (new juce::Label ("new label",
                                    TRANS ("TABLE")));
    addAndMakeVisible (label13.get());
    label13->setFont (juce::Font ("Code Pro Demo", 11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label13->setJustificationType (juce::Justification::centredRight);
    label13->setEditable (false, false, false);
    label13->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label13->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label13->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    c_FreqMod.reset (new VASTSlider ("c_FreqMod"));
    addAndMakeVisible (c_FreqMod.get());
    c_FreqMod->setTooltip (TRANS ("Frequency modulation of selection"));
    c_FreqMod->setRange (0, 20, 0.001);
    c_FreqMod->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    c_FreqMod->setTextBoxStyle (juce::Slider::NoTextBox, false, 30, 14);
    c_FreqMod->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffc9c9c9));
    c_FreqMod->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0x00ffffff));
    c_FreqMod->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x00808080));

    label15.reset (new juce::Label ("new label",
                                    TRANS ("FMOD")));
    addAndMakeVisible (label15.get());
    label15->setFont (juce::Font ("Code Pro Demo", 11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label15->setJustificationType (juce::Justification::centred);
    label15->setEditable (false, false, false);
    label15->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label15->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label15->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    c_genMax.reset (new VASTSlider ("c_genMax"));
    addAndMakeVisible (c_genMax.get());
    c_genMax->setTooltip (TRANS ("Maximum modulation value of generation"));
    c_genMax->setRange (0, 100, 0.001);
    c_genMax->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    c_genMax->setTextBoxStyle (juce::Slider::NoTextBox, false, 30, 14);
    c_genMax->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffc9c9c9));
    c_genMax->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0x00ffffff));
    c_genMax->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x00808080));
    c_genMax->addListener (this);

    c_Bend.reset (new VASTSlider ("c_Bend"));
    addAndMakeVisible (c_Bend.get());
    c_Bend->setTooltip (TRANS ("Bend selection"));
    c_Bend->setRange (-100, 100, 0.001);
    c_Bend->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    c_Bend->setTextBoxStyle (juce::Slider::NoTextBox, false, 30, 14);
    c_Bend->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffc9c9c9));
    c_Bend->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0x00ffffff));
    c_Bend->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x00808080));

    label18.reset (new juce::Label ("new label",
                                    TRANS ("BEND")));
    addAndMakeVisible (label18.get());
    label18->setFont (juce::Font ("Code Pro Demo", 11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label18->setJustificationType (juce::Justification::centred);
    label18->setEditable (false, false, false);
    label18->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label18->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label18->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    c_Bloat.reset (new VASTSlider ("c_Bloat"));
    addAndMakeVisible (c_Bloat.get());
    c_Bloat->setTooltip (TRANS ("Inflate selection"));
    c_Bloat->setRange (-100, 100, 0.001);
    c_Bloat->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    c_Bloat->setTextBoxStyle (juce::Slider::NoTextBox, false, 30, 14);
    c_Bloat->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffc9c9c9));
    c_Bloat->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0x00ffffff));
    c_Bloat->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x00808080));

    label19.reset (new juce::Label ("new label",
                                    TRANS ("BLOAT")));
    addAndMakeVisible (label19.get());
    label19->setFont (juce::Font ("Code Pro Demo", 11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label19->setJustificationType (juce::Justification::centred);
    label19->setEditable (false, false, false);
    label19->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label19->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label19->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    c_Level.reset (new VASTSlider ("c_Level"));
    addAndMakeVisible (c_Level.get());
    c_Level->setTooltip (TRANS ("Change level of selection"));
    c_Level->setRange (-100, 100, 0.01);
    c_Level->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    c_Level->setTextBoxStyle (juce::Slider::NoTextBox, false, 30, 14);
    c_Level->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffc9c9c9));
    c_Level->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0x00ffffff));
    c_Level->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x00808080));

    c_iconSelectAll.reset (new VASTDrawableButton ("c_iconSelectAll", select_all_svg, select_all_svgSize, "Select all positions and whole wavetable"));
    addAndMakeVisible (c_iconSelectAll.get());
    c_iconSelectAll->setName ("c_iconSelectAll");

    c_iconAddPos.reset (new VASTDrawableButton ("c_iconAddPos", add_svg, add_svgSize, "Add position after selected position"));
    addAndMakeVisible (c_iconAddPos.get());
    c_iconAddPos->setName ("c_iconAddPos");

    c_iconDelPos.reset (new VASTDrawableButton ("c_iconDelPos", delete_svg, delete_svgSize, "Delete multiselected positions"));
    addAndMakeVisible (c_iconDelPos.get());
    c_iconDelPos->setName ("c_iconDelPos");

    c_iconPaste.reset (new VASTDrawableButton ("c_iconPaste", paste_svg, paste_svgSize, "Insert multiselected positions"));
    addAndMakeVisible (c_iconPaste.get());
    c_iconPaste->setName ("c_iconPaste");

    c_iconDuplicate.reset (new VASTDrawableButton ("c_iconDuplicate",duplicate_svg, duplicate_svgSize, "Duplicate multiselected positions"));
    addAndMakeVisible (c_iconDuplicate.get());
    c_iconDuplicate->setName ("c_iconDuplicate");

    c_iconThinOut.reset (new VASTDrawableButton ("c_iconThinOut",minus_svg, minus_svgSize, "Delete even positions in  multiselection"));
    addAndMakeVisible (c_iconThinOut.get());
    c_iconThinOut->setName ("c_iconThinOut");

    c_iconNormalize.reset (new VASTDrawableButton ("c_iconNormalize",normalize3_svg, normalize3_svgSize, "Normalize selected positions"));
    addAndMakeVisible (c_iconNormalize.get());
    c_iconNormalize->setName ("c_iconNormalize");

    c_iconCopy.reset (new VASTDrawableButton ("c_iconCopy", copy_svg, copy_svgSize, "Copy multiselected positions"));
    addAndMakeVisible (c_iconCopy.get());
    c_iconCopy->setName ("c_iconCopy");

    c_iconSine.reset (new VASTDrawableButton ("c_iconSine", waves_sine_svg, waves_sine_svgSize, "Initialize  selection of position with sine wave"));
    addAndMakeVisible (c_iconSine.get());
    c_iconSine->setName ("c_iconSine");

    c_iconHalfSine.reset (new VASTDrawableButton ("c_iconHalfSine", waves_halfsine_svg, waves_halfsine_svgSize, "Initialize  selection of position with half (upper) sine wave"));
    addAndMakeVisible (c_iconHalfSine.get());
    c_iconHalfSine->setName ("c_iconHalfSine");

    c_iconTriangle.reset (new VASTDrawableButton ("c_iconTriangle", waves_triangle_svg, waves_triangle_svgSize, "Initialize  selection of position with triangle wave"));
    addAndMakeVisible (c_iconTriangle.get());
    c_iconTriangle->setName ("c_iconTriangle");

    c_iconZeros.reset (new VASTDrawableButton ("c_iconZeros", waves_zeros_svg, waves_zeros_svgSize, "Initialize selection of position with zeros"));
    addAndMakeVisible (c_iconZeros.get());
    c_iconZeros->setName ("c_iconZeros");

    c_iconSaw.reset (new VASTDrawableButton ("c_iconSaw", waves_saw_svg, waves_saw_svgSize, "Initialize  selection of position with sawtooth wave"));
    addAndMakeVisible (c_iconSaw.get());
    c_iconSaw->setName ("c_iconSaw");

    c_iconSquare.reset (new VASTDrawableButton ("c_iconSquare", waves_square_svg, waves_square_svgSize, "Initialize  selection of position with square / pulse wave" ));
    addAndMakeVisible (c_iconSquare.get());
    c_iconSquare->setName ("c_iconSquare");

    c_iconRamp.reset (new VASTDrawableButton ("c_iconRamp", waves_ramp_svg, waves_ramp_svgSize, "Initialize  selection of position with ramp"));
    addAndMakeVisible (c_iconRamp.get());
    c_iconRamp->setName ("c_iconRamp");

    c_iconRandom.reset (new VASTDrawableButton ("c_iconRandom",waves_random_svg, waves_random_svgSize, "Initialize  selection of position with random wave"));
    addAndMakeVisible (c_iconRandom.get());
    c_iconRandom->setName ("c_iconRandom");

    c_iconSinc.reset (new VASTDrawableButton ("c_iconSinc",waves_sinc_svg, waves_sinc_svgSize, "Initialize  selection of position with sinc wave"));
    addAndMakeVisible (c_iconSinc.get());
    c_iconSinc->setName ("c_iconSinc");

    c_iconReverse.reset (new VASTDrawableButton ("c_iconReverse",reverse_svg, reverse_svgSize, "Reverse selection of position"));
    addAndMakeVisible (c_iconReverse.get());
    c_iconReverse->setName ("c_iconReverse");

    c_iconInvert.reset (new VASTDrawableButton ("c_iconInvert", invert_svg, invert_svgSize, "Invert selection of position"));
    addAndMakeVisible (c_iconInvert.get());
    c_iconInvert->setName ("c_iconInvert");

    c_iconClearWT.reset (new VASTDrawableButton ("c_iconClearWT",close_svg, close_svgSize, "Clear wavetable"));
    addAndMakeVisible (c_iconClearWT.get());
    c_iconClearWT->setName ("c_iconClearWT");

    c_iconLoadWT.reset (new VASTDrawableButton ("c_iconLoadWT",load_svg, load_svgSize, "Load wavetable"));
    addAndMakeVisible (c_iconLoadWT.get());
    c_iconLoadWT->setName ("c_iconLoadWT");

    c_iconArcConnect.reset (new VASTDrawableButton ("c_iconArcConnect", arc_connect_svg, arc_connect_svgSize, "Draw arc from first value of selection to last value of selection in postion"));
    addAndMakeVisible (c_iconArcConnect.get());
    c_iconArcConnect->setName ("c_iconArcConnect");

    c_iconRampConnect.reset (new VASTDrawableButton ("c_iconRampConnect", ramp_connect_svg, ramp_connect_svgSize, "Draw ramp from first value of selection to last value of selection in postion"));
    addAndMakeVisible (c_iconRampConnect.get());
    c_iconRampConnect->setName ("c_iconRampConnect");

    c_iconSave.reset (new VASTDrawableButton ("c_iconSave", save_svg, save_svgSize, "Save wavetable to file"));
    addAndMakeVisible (c_iconSave.get());
    c_iconSave->setName ("c_iconSave");

    c_applyOptions.reset (new VASTComboBox ("c_applyOptions"));
    addAndMakeVisible (c_applyOptions.get());
    c_applyOptions->setTooltip (TRANS ("Option that shall be applied from begin to end of multi selection"));
    c_applyOptions->setExplicitFocusOrder (1);
    c_applyOptions->setEditableText (false);
    c_applyOptions->setJustificationType (juce::Justification::centredLeft);
    c_applyOptions->setTextWhenNothingSelected (juce::String());
    c_applyOptions->setTextWhenNoChoicesAvailable (TRANS ("(no choices)"));
    c_applyOptions->addItem (TRANS ("Interpolation"), 1);
    c_applyOptions->addItem (TRANS ("Amplitude Modulation"), 2);
    c_applyOptions->addItem (TRANS ("Mix In"), 3);
    c_applyOptions->addItem (TRANS ("Convolution"), 4);
    c_applyOptions->addItem (TRANS ("Time > Frequency"), 5);
    c_applyOptions->addItem (TRANS ("Frequency > Time"), 6);
    c_applyOptions->addItem (TRANS ("Normalize (Table)"), 7);
    c_applyOptions->addItem (TRANS ("Normalize (Each)"), 8);
    c_applyOptions->addListener (this);

    c_generationOptions.reset (new VASTComboBox ("c_generationOptions"));
    addAndMakeVisible (c_generationOptions.get());
    c_generationOptions->setTooltip (TRANS ("Option that shall be generated"));
    c_generationOptions->setExplicitFocusOrder (1);
    c_generationOptions->setEditableText (false);
    c_generationOptions->setJustificationType (juce::Justification::centredLeft);
    c_generationOptions->setTextWhenNothingSelected (juce::String());
    c_generationOptions->setTextWhenNoChoicesAvailable (TRANS ("(no choices)"));
    c_generationOptions->addItem (TRANS ("HardSync"), 1);
    c_generationOptions->addItem (TRANS ("Pulse Mod (PWM)"), 2);
    c_generationOptions->addItem (TRANS ("Frequency Mod (FM)"), 3);
    c_generationOptions->addItem (TRANS ("Bloat"), 4);
    c_generationOptions->addItem (TRANS ("Bend"), 5);
    c_generationOptions->addItem (TRANS ("Formants (Male)"), 6);
    c_generationOptions->addItem (TRANS ("Formants (Female)"), 7);
    c_generationOptions->addItem (TRANS ("Harmonic Morph"), 8);
    c_generationOptions->addItem (TRANS ("Harmonic Reduce"), 9);
    c_generationOptions->addItem (TRANS ("Highpass Filter"), 10);
    c_generationOptions->addItem (TRANS ("Lowpass Filter"), 11);
    c_generationOptions->addItem (TRANS ("Lowpass Soft"), 12);
    c_generationOptions->addItem (TRANS ("Bandpass Filter"), 13);
    c_generationOptions->addItem (TRANS ("Pulse Filter"), 14);
    c_generationOptions->addItem (TRANS ("Comb Filter"), 15);
    c_generationOptions->addListener (this);

    c_iconApply.reset (new VASTDrawableButton ("c_iconApply",apply_svg, apply_svgSize, "Apply option from begin to end of multi selection"));
    addAndMakeVisible (c_iconApply.get());
    c_iconApply->setName ("c_iconApply");

    c_iconGenerate.reset (new VASTDrawableButton ("c_iconGenerate",generate_svg, generate_svgSize, "Generate wavetable per option based on selected single cycle"));
    addAndMakeVisible (c_iconGenerate.get());
    c_iconGenerate->setName ("c_iconGenerate");

    c_Connect.reset (new VASTSlider ("c_Connect"));
    addAndMakeVisible (c_Connect.get());
    c_Connect->setTooltip (TRANS ("Connect start value of selection with end value of selection with arc or ramp "));
    c_Connect->setRange (-100, 100, 0.001);
    c_Connect->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    c_Connect->setTextBoxStyle (juce::Slider::NoTextBox, false, 30, 14);
    c_Connect->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffc9c9c9));
    c_Connect->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0x00ffffff));
    c_Connect->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x00808080));

    label11.reset (new juce::Label ("new label",
                                    TRANS ("CNECT")));
    addAndMakeVisible (label11.get());
    label11->setFont (juce::Font ("Code Pro Demo", 11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label11->setJustificationType (juce::Justification::centred);
    label11->setEditable (false, false, false);
    label11->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label11->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label11->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    c_applyMax.reset (new VASTSlider ("c_applyMax"));
    addAndMakeVisible (c_applyMax.get());
    c_applyMax->setTooltip (TRANS ("Maximum modulation value for applied modulation"));
    c_applyMax->setRange (0, 100, 0.001);
    c_applyMax->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    c_applyMax->setTextBoxStyle (juce::Slider::NoTextBox, false, 30, 14);
    c_applyMax->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffc9c9c9));
    c_applyMax->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0x00ffffff));
    c_applyMax->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x00808080));
    c_applyMax->addListener (this);

    c_WTableNext.reset (new juce::TextButton ("c_WTableNext"));
    addAndMakeVisible (c_WTableNext.get());
    c_WTableNext->setTooltip (TRANS ("Load next wavetable"));
    c_WTableNext->setButtonText (TRANS (">"));

    c_WTablePrevious.reset (new juce::TextButton ("c_WTablePrevious"));
    addAndMakeVisible (c_WTablePrevious.get());
    c_WTablePrevious->setTooltip (TRANS ("Load previous wavetable"));
    c_WTablePrevious->setButtonText (TRANS ("<"));

    label12.reset (new juce::Label ("new label",
                                    TRANS ("SOLO")));
    addAndMakeVisible (label12.get());
    label12->setFont (juce::Font ("Code Pro Demo", 11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label12->setJustificationType (juce::Justification::centredLeft);
    label12->setEditable (false, false, false);
    label12->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label12->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label12->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    c_soloSelection.reset (new juce::ToggleButton ("c_soloSelection"));
    addAndMakeVisible (c_soloSelection.get());
    c_soloSelection->setTooltip (TRANS ("Only play the selected wavetable positions in oscillator"));
    c_soloSelection->setButtonText (juce::String());

    c_gridMode.reset (new VASTComboBox ("c_gridMode"));
    addAndMakeVisible (c_gridMode.get());
    c_gridMode->setTooltip (TRANS ("Grid mode in wavetable editor"));
    c_gridMode->setExplicitFocusOrder (1);
    c_gridMode->setEditableText (false);
    c_gridMode->setJustificationType (juce::Justification::centredLeft);
    c_gridMode->setTextWhenNothingSelected (juce::String());
    c_gridMode->setTextWhenNoChoicesAvailable (TRANS ("(no choices)"));
    c_gridMode->addItem (TRANS ("No Grid"), 1);
    c_gridMode->addItem (TRANS ("4x4"), 2);
    c_gridMode->addItem (TRANS ("8x8"), 3);
    c_gridMode->addItem (TRANS ("16x16"), 4);
    c_gridMode->addItem (TRANS ("32x32"), 5);
    c_gridMode->addItem (TRANS ("64x64"), 6);
    c_gridMode->addItem (TRANS ("128x128"), 7);
    c_gridMode->addListener (this);

    c_drawMode.reset (new VASTComboBox ("c_drawMode"));
    addAndMakeVisible (c_drawMode.get());
    c_drawMode->setTooltip (TRANS ("Draw mode in wavetable editor"));
    c_drawMode->setExplicitFocusOrder (1);
    c_drawMode->setEditableText (false);
    c_drawMode->setJustificationType (juce::Justification::centredLeft);
    c_drawMode->setTextWhenNothingSelected (juce::String());
    c_drawMode->setTextWhenNoChoicesAvailable (TRANS ("(no choices)"));
    c_drawMode->addItem (TRANS ("Select"), 1);
    c_drawMode->addItem (TRANS ("Line/Arc"), 2);
    c_drawMode->addItem (TRANS ("Draw"), 3);
    c_drawMode->addListener (this);

    c_binMode.reset (new VASTComboBox ("c_binMode"));
    addAndMakeVisible (c_binMode.get());
    c_binMode->setTooltip (TRANS ("Bin mode for editing frequency domain"));
    c_binMode->setExplicitFocusOrder (1);
    c_binMode->setEditableText (false);
    c_binMode->setJustificationType (juce::Justification::centredLeft);
    c_binMode->setTextWhenNothingSelected (juce::String());
    c_binMode->setTextWhenNoChoicesAvailable (TRANS ("(no choices)"));
    c_binMode->addItem (TRANS ("Clip"), 1);
    c_binMode->addItem (TRANS ("Normalize"), 2);
    c_binMode->addListener (this);

    c_binEditMode.reset (new VASTComboBox ("c_binEditMode"));
    addAndMakeVisible (c_binEditMode.get());
    c_binEditMode->setTooltip (TRANS ("Bin edit mode for editing frequency domain"));
    c_binEditMode->setExplicitFocusOrder (1);
    c_binEditMode->setEditableText (false);
    c_binEditMode->setJustificationType (juce::Justification::centredLeft);
    c_binEditMode->setTextWhenNothingSelected (juce::String());
    c_binEditMode->setTextWhenNoChoicesAvailable (TRANS ("(no choices)"));
    c_binEditMode->addItem (TRANS ("SingleBin"), 1);
    c_binEditMode->addItem (TRANS ("Every2nd"), 2);
    c_binEditMode->addItem (TRANS ("Every3rd"), 3);
    c_binEditMode->addItem (TRANS ("Pile"), 4);
    c_binEditMode->addItem (TRANS ("DragShift"), 5);
    c_binEditMode->addItem (TRANS ("DragStretch"), 6);
    c_binEditMode->addListener (this);

    c_iconSampleHold.reset (new VASTDrawableButton ("c_iconSampleHold",waves_samplehold_svg, waves_samplehold_svgSize, "Initialize  selection of position with sample and hold wave"));
    addAndMakeVisible (c_iconSampleHold.get());
    c_iconSampleHold->setName ("c_iconSampleHold");

    c_positionLabel.reset (new juce::Label ("c_positionLabel",
                                            TRANS ("-")));
    addAndMakeVisible (c_positionLabel.get());
    c_positionLabel->setFont (juce::Font ("Code Pro Demo", 11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    c_positionLabel->setJustificationType (juce::Justification::centred);
    c_positionLabel->setEditable (false, false, false);
    c_positionLabel->setColour (juce::Label::textColourId, juce::Colour (0xffa4a4a4));
    c_positionLabel->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    c_positionLabel->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label14.reset (new juce::Label ("new label",
                                    TRANS ("SNAP")));
    addAndMakeVisible (label14.get());
    label14->setFont (juce::Font ("Code Pro Demo", 11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label14->setJustificationType (juce::Justification::centredLeft);
    label14->setEditable (false, false, false);
    label14->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label14->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label14->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    c_iconMaximizeEditor.reset (new VASTDrawableButton ("c_iconMaximize", arrow_right_corner_svg, arrow_right_corner_svgSize, "Maximize area"));
    addAndMakeVisible (c_iconMaximizeEditor.get());
    c_iconMaximizeEditor->setName ("c_iconMaximizeEditor");

    c_snap.reset (new juce::ToggleButton ("c_snap"));
    addAndMakeVisible (c_snap.get());
    c_snap->setTooltip (TRANS ("Snap editor functions to zero crossings"));
    c_snap->setButtonText (juce::String());

    c_stretch.reset (new juce::ToggleButton ("c_stretch"));
    addAndMakeVisible (c_stretch.get());
    c_stretch->setTooltip (TRANS ("Stretch editor functions to zero crossings"));
    c_stretch->setButtonText (juce::String());

    label16.reset (new juce::Label ("new label",
                                    TRANS ("STRETCH")));
    addAndMakeVisible (label16.get());
    label16->setFont (juce::Font ("Code Pro Demo", 11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label16->setJustificationType (juce::Justification::centredLeft);
    label16->setEditable (false, false, false);
    label16->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label16->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label16->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    c_iconRecordWT.reset (new VASTDrawableButton ("c_iconRecordWT", record_svg, record_svgSize, "Record single cycle changes into wavetable"));
    addAndMakeVisible (c_iconRecordWT.get());
    c_iconRecordWT->setName ("c_iconRecordWT");

    drawable1 = juce::Drawable::createFromImageData (zoom_in_svg, zoom_in_svgSize);

    //[UserPreSize]
	m_iWTEditorZoom->setIncDecButtonsMode(Slider::incDecButtonsDraggable_Vertical);
	m_iWTEditorZoom->setTextBoxIsEditable(true);
	m_iWTEditorZoom->addListener(myWTEditor);
	c_iconSave->addListener(myWTEditor);
	c_iconSelectAll->addListener(myWTEditor);
	c_iconAddPos->addListener(myWTEditor);
	c_iconDelPos->addListener(myWTEditor);
	c_iconCopy->addListener(myWTEditor);
	c_iconPaste->addListener(myWTEditor);
	c_iconDuplicate->addListener(myWTEditor);
	c_iconThinOut->addListener(myWTEditor);
	c_iconNormalize->addListener(myWTEditor);

	c_iconLoadWT->addListener(myWTEditor);
	c_iconClearWT->addListener(myWTEditor);

	c_iconSine->addListener(myWTEditor);
	c_iconHalfSine->addListener(myWTEditor);
	c_iconTriangle->addListener(myWTEditor);
	c_iconZeros->addListener(myWTEditor);
	c_iconSaw->addListener(myWTEditor);
	c_iconSquare->addListener(myWTEditor);
	c_iconRamp->addListener(myWTEditor);
	c_iconRandom->addListener(myWTEditor);
	c_iconSinc->addListener(myWTEditor);
	c_iconReverse->addListener(myWTEditor);
	c_iconInvert->addListener(myWTEditor);
	c_iconArcConnect->addListener(myWTEditor);
	c_iconRampConnect->addListener(myWTEditor);
	c_iconSampleHold->addListener(myWTEditor);

	c_iconApply->addListener(myWTEditor);
	c_iconGenerate->addListener(myWTEditor);

	c_iconMaximizeEditor->addListener(myWTEditor);

	c_WTablePrevious->addListener(myWTEditor);
	c_WTableNext->addListener(myWTEditor);

	c_soloSelection->addListener(myWTEditor);
	c_snap->addListener(myWTEditor);
	c_stretch->addListener(myWTEditor);

	//manage parameter bindings -> set ComponentID = Name and processor
	Array<Component*> children = this->getChildren();
	for (auto child : children) {
		if (child->getName().startsWith("m_")) {
			auto* aSlider = dynamic_cast<VASTParameterSlider*> (child);
			if (aSlider != nullptr) {
				aSlider->setAudioProcessor(*myProcessor);
				aSlider->bindParameter(myEditor, aSlider->getName(), VASTGUIRuntimeModel::GUIComponents::WavetableEditorViewHeader, 0);
			}
			auto* aCombobox = dynamic_cast<VASTParameterComboBox*> (child);
			if (aCombobox != nullptr) {
				aCombobox->setAudioProcessor(*myProcessor);
				aCombobox->bindParameter(myEditor, aCombobox->getName(), VASTGUIRuntimeModel::GUIComponents::WavetableEditorViewHeader, 0);
			}
			auto* aButton = dynamic_cast<VASTParameterButton*> (child);
			if (aButton != nullptr) {
				aButton->setAudioProcessor(*myProcessor);
				aButton->bindParameter(myEditor, aButton->getName(), VASTGUIRuntimeModel::GUIComponents::WavetableEditorViewHeader, 0);
			}
		}
	}
	c_genMax->setDefaultValue(100);
	c_genMax->setSkewFactor(0.42f, false);
	c_applyMax->setDefaultValue(100);
	c_applyMax->setSkewFactor(0.42f, false);
	c_bitDepth->setDefaultValue(32);
	c_bitDepth->setSkewFactor(0.22f, false);
	c_wtName->applyFontToAllText(myEditor->getCurrentVASTLookAndFeel()->myDefaultFontBold, true); //after bindings
	c_wtName->addListener(myWTEditor);
	c_wtName->setComponentID("textEditor");
	c_wtName->onTextChange = [this]() {
	c_wtName->applyFontToAllText(myEditor->getCurrentVASTLookAndFeel()->getTextEditorFont(*c_wtName.get())); };

	c_genMax->setAudioProcessor(*myProcessor);
	c_genMax->addListener(myWTEditor);
	c_applyMax->setAudioProcessor(*myProcessor);
	c_applyMax->addListener(myWTEditor);
	c_Bend->setAudioProcessor(*myProcessor);
	c_Bend->addListener(myWTEditor);
	c_Bloat->setAudioProcessor(*myProcessor);
	c_Bloat->addListener(myWTEditor);
	c_Phase->setAudioProcessor(*myProcessor);
	c_Phase->addListener(myWTEditor);
	c_Level->setAudioProcessor(*myProcessor);
	c_Level->addListener(myWTEditor);
	c_Freq->setAudioProcessor(*myProcessor);
	c_Freq->addListener(myWTEditor);
	c_FreqMod->setAudioProcessor(*myProcessor);
	c_FreqMod->addListener(myWTEditor);
	c_bitDepth->setAudioProcessor(*myProcessor);
	c_bitDepth->addListener(myWTEditor);
	c_Offset->setAudioProcessor(*myProcessor);
	c_Offset->addListener(myWTEditor);
	c_Smooth->setAudioProcessor(*myProcessor);
	c_Smooth->addListener(myWTEditor);
	c_Clean->setAudioProcessor(*myProcessor);
	c_Clean->addListener(myWTEditor);
	c_Connect->setAudioProcessor(*myProcessor);
	c_Connect->addListener(myWTEditor);
	c_iconRecordWT->setAudioProcessor(*myProcessor);
	c_iconRecordWT->addListener(myWTEditor);

	c_oscA->addListener(myWTEditor);
	c_oscB->addListener(myWTEditor);
	c_oscC->addListener(myWTEditor);
	c_oscD->addListener(myWTEditor);

	c_applyOptions->setSelectedItemIndex(ApplyOption::ApplyInterpolation, NotificationType::dontSendNotification);
	c_generationOptions->setSelectedItemIndex(GenerateOption::GenerateHardSync, NotificationType::dontSendNotification);

	c_gridMode->setSelectedItemIndex(OscillatorGridMode::NoGrid, NotificationType::dontSendNotification);
	c_binMode->setSelectedItemIndex(BinMode::ClipBin, NotificationType::dontSendNotification);
	c_binEditMode->setSelectedItemIndex(FreqEditMode::SingleBin, NotificationType::dontSendNotification);
	c_drawMode->setSelectedItemIndex(OscillatorEditMode::SelectMode, NotificationType::dontSendNotification);

	setOpaque(true);
    //[/UserPreSize]

    setSize (800, 96);


    //[Constructor] You can add your own custom stuff here..
	//setLookAndFeel(myEditor->getCurrentVASTLookAndFeel());
	c_oscA->setToggleState(true, juce::NotificationType::sendNotification);
    //[/Constructor]
}

VASTWaveTableEditorViewHeader::~VASTWaveTableEditorViewHeader()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
	this->setLookAndFeel(nullptr);
    //[/Destructor_pre]

    label10 = nullptr;
    label9 = nullptr;
    label7 = nullptr;
    label8 = nullptr;
    c_Phase = nullptr;
    label108 = nullptr;
    label135 = nullptr;
    c_Freq = nullptr;
    label2 = nullptr;
    c_Offset = nullptr;
    c_bitDepth = nullptr;
    label3 = nullptr;
    label4 = nullptr;
    c_Smooth = nullptr;
    label5 = nullptr;
    c_Clean = nullptr;
    label6 = nullptr;
    c_oscA = nullptr;
    c_oscB = nullptr;
    c_oscC = nullptr;
    c_oscD = nullptr;
    m_iWTEditorZoom = nullptr;
    c_wtName = nullptr;
    label13 = nullptr;
    c_FreqMod = nullptr;
    label15 = nullptr;
    c_genMax = nullptr;
    c_Bend = nullptr;
    label18 = nullptr;
    c_Bloat = nullptr;
    label19 = nullptr;
    c_Level = nullptr;
    c_iconSelectAll = nullptr;
    c_iconAddPos = nullptr;
    c_iconDelPos = nullptr;
    c_iconPaste = nullptr;
    c_iconDuplicate = nullptr;
    c_iconThinOut = nullptr;
    c_iconNormalize = nullptr;
    c_iconCopy = nullptr;
    c_iconSine = nullptr;
    c_iconHalfSine = nullptr;
    c_iconTriangle = nullptr;
    c_iconZeros = nullptr;
    c_iconSaw = nullptr;
    c_iconSquare = nullptr;
    c_iconRamp = nullptr;
    c_iconRandom = nullptr;
    c_iconSinc = nullptr;
    c_iconReverse = nullptr;
    c_iconInvert = nullptr;
    c_iconClearWT = nullptr;
    c_iconLoadWT = nullptr;
    c_iconArcConnect = nullptr;
    c_iconRampConnect = nullptr;
    c_iconSave = nullptr;
    c_applyOptions = nullptr;
    c_generationOptions = nullptr;
    c_iconApply = nullptr;
    c_iconGenerate = nullptr;
    c_Connect = nullptr;
    label11 = nullptr;
    c_applyMax = nullptr;
    c_WTableNext = nullptr;
    c_WTablePrevious = nullptr;
    label12 = nullptr;
    c_soloSelection = nullptr;
    c_gridMode = nullptr;
    c_drawMode = nullptr;
    c_binMode = nullptr;
    c_binEditMode = nullptr;
    c_iconSampleHold = nullptr;
    c_positionLabel = nullptr;
    label14 = nullptr;
    c_iconMaximizeEditor = nullptr;
    c_snap = nullptr;
    c_stretch = nullptr;
    label16 = nullptr;
    c_iconRecordWT = nullptr;
    drawable1 = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void VASTWaveTableEditorViewHeader::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff323e44));

    {
        int x = 0, y = 0, width = getWidth() - 0, height = getHeight() - 0;
        juce::Colour fillColour1 = juce::Colour (0x4fa2a7b5), fillColour2 = juce::Colour (0xbd141618);
        //[UserPaintCustomArguments] Customize the painting arguments here..
    		fillColour1 = myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colWTEditorHeaderBackgroundGradientFrom);
    		fillColour2 = myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colWTEditorHeaderBackgroundGradientTo);
        //[/UserPaintCustomArguments]
        g.setGradientFill (juce::ColourGradient (fillColour1,
                                             static_cast<float> (proportionOfWidth (-0.0400f)) - 0.0f + x,
                                             static_cast<float> (proportionOfHeight (-0.0154f)) - 0.0f + y,
                                             fillColour2,
                                             static_cast<float> (proportionOfWidth (0.9467f)) - 0.0f + x,
                                             static_cast<float> (proportionOfHeight (1.0615f)) - 0.0f + y,
                                             false));
        g.fillRect (x, y, width, height);
    }

    {
        int x = proportionOfWidth (0.1700f), y = proportionOfHeight (0.5833f), width = proportionOfWidth (0.5300f), height = proportionOfHeight (0.1250f);
        juce::Colour fillColour1 = juce::Colour (0xff3a4145), fillColour2 = juce::Colour (0xff33393c);
        //[UserPaintCustomArguments] Customize the painting arguments here..
    		fillColour1 = myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscillatorComponentHeaderGradientFrom); //TODO?
    		fillColour2 = myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscillatorComponentHeaderGradientTo); //TODO?
        //[/UserPaintCustomArguments]
        g.setGradientFill (juce::ColourGradient (fillColour1,
                                             static_cast<float> (proportionOfWidth (0.0700f)) - static_cast<float> (proportionOfWidth (0.1700f)) + x,
                                             static_cast<float> (proportionOfHeight (0.0974f)) - static_cast<float> (proportionOfHeight (0.5833f)) + y,
                                             fillColour2,
                                             static_cast<float> (proportionOfWidth (0.5700f)) - static_cast<float> (proportionOfWidth (0.1700f)) + x,
                                             static_cast<float> (proportionOfHeight (0.1252f)) - static_cast<float> (proportionOfHeight (0.5833f)) + y,
                                             false));
        g.fillRect (x, y, width, height);
    }

    {
        int x = proportionOfWidth (0.9000f), y = proportionOfHeight (0.7700f), width = proportionOfWidth (0.0150f), height = proportionOfHeight (0.1771f);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (juce::Colours::black);
        jassert (drawable1 != nullptr);
        if (drawable1 != nullptr)
            drawable1->drawWithin (g, juce::Rectangle<int> (x, y, width, height).toFloat(),
                                   juce::RectanglePlacement::centred, 1.000f);
    }

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void VASTWaveTableEditorViewHeader::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    label10->setBounds (proportionOfWidth (0.1013f), proportionOfHeight (0.5312f), proportionOfWidth (0.0788f), proportionOfHeight (0.1250f));
    label9->setBounds (proportionOfWidth (0.1013f), proportionOfHeight (0.3333f), proportionOfWidth (0.0788f), proportionOfHeight (0.1250f));
    label7->setBounds (proportionOfWidth (0.0238f), proportionOfHeight (0.3333f), proportionOfWidth (0.0788f), proportionOfHeight (0.1250f));
    label8->setBounds (proportionOfWidth (0.0238f), proportionOfHeight (0.5312f), proportionOfWidth (0.0788f), proportionOfHeight (0.1250f));
    c_Phase->setBounds (proportionOfWidth (0.2175f), proportionOfHeight (0.2292f), proportionOfWidth (0.0462f), proportionOfHeight (0.3750f));
    label108->setBounds (proportionOfWidth (0.1537f), proportionOfHeight (0.5938f), proportionOfWidth (0.0775f), proportionOfHeight (0.1250f));
    label135->setBounds (proportionOfWidth (0.2013f), proportionOfHeight (0.5938f), proportionOfWidth (0.0775f), proportionOfHeight (0.1250f));
    c_Freq->setBounds (proportionOfWidth (0.2650f), proportionOfHeight (0.2292f), proportionOfWidth (0.0462f), proportionOfHeight (0.3750f));
    label2->setBounds (proportionOfWidth (0.2475f), proportionOfHeight (0.5938f), proportionOfWidth (0.0775f), proportionOfHeight (0.1250f));
    c_Offset->setBounds (proportionOfWidth (0.4062f), proportionOfHeight (0.2292f), proportionOfWidth (0.0462f), proportionOfHeight (0.3750f));
    c_bitDepth->setBounds (proportionOfWidth (0.3588f), proportionOfHeight (0.2292f), proportionOfWidth (0.0462f), proportionOfHeight (0.3750f));
    label3->setBounds (proportionOfWidth (0.3400f), proportionOfHeight (0.5938f), proportionOfWidth (0.0775f), proportionOfHeight (0.1250f));
    label4->setBounds (proportionOfWidth (0.3887f), proportionOfHeight (0.5938f), proportionOfWidth (0.0775f), proportionOfHeight (0.1250f));
    c_Smooth->setBounds (proportionOfWidth (0.5475f), proportionOfHeight (0.2292f), proportionOfWidth (0.0462f), proportionOfHeight (0.3750f));
    label5->setBounds (proportionOfWidth (0.5312f), proportionOfHeight (0.5938f), proportionOfWidth (0.0775f), proportionOfHeight (0.1250f));
    c_Clean->setBounds (proportionOfWidth (0.5950f), proportionOfHeight (0.2292f), proportionOfWidth (0.0462f), proportionOfHeight (0.3750f));
    label6->setBounds (proportionOfWidth (0.5825f), proportionOfHeight (0.5938f), proportionOfWidth (0.0775f), proportionOfHeight (0.1250f));
    c_oscA->setBounds (proportionOfWidth (0.0050f), proportionOfHeight (0.3125f), proportionOfWidth (0.0213f), proportionOfHeight (0.1771f));
    c_oscB->setBounds (proportionOfWidth (0.0050f), proportionOfHeight (0.5104f), proportionOfWidth (0.0213f), proportionOfHeight (0.1771f));
    c_oscC->setBounds (proportionOfWidth (0.0800f), proportionOfHeight (0.3125f), proportionOfWidth (0.0213f), proportionOfHeight (0.1771f));
    c_oscD->setBounds (proportionOfWidth (0.0800f), proportionOfHeight (0.5000f), proportionOfWidth (0.0213f), proportionOfHeight (0.1771f));
    m_iWTEditorZoom->setBounds (proportionOfWidth (0.9200f), proportionOfHeight (0.7708f), proportionOfWidth (0.0788f), proportionOfHeight (0.1667f));
    c_wtName->setBounds (proportionOfWidth (0.2325f), proportionOfHeight (0.0417f), proportionOfWidth (0.1888f), proportionOfHeight (0.1562f));
    label13->setBounds (proportionOfWidth (0.1437f), proportionOfHeight (0.0625f), proportionOfWidth (0.0900f), proportionOfHeight (0.1250f));
    c_FreqMod->setBounds (proportionOfWidth (0.3113f), proportionOfHeight (0.2292f), proportionOfWidth (0.0462f), proportionOfHeight (0.3750f));
    label15->setBounds (proportionOfWidth (0.2950f), proportionOfHeight (0.5938f), proportionOfWidth (0.0775f), proportionOfHeight (0.1250f));
    c_genMax->setBounds (proportionOfWidth (0.6562f), proportionOfHeight (0.7604f), proportionOfWidth (0.0300f), proportionOfHeight (0.1875f));
    c_Bend->setBounds (proportionOfWidth (0.4537f), proportionOfHeight (0.2292f), proportionOfWidth (0.0462f), proportionOfHeight (0.3750f));
    label18->setBounds (proportionOfWidth (0.4375f), proportionOfHeight (0.5938f), proportionOfWidth (0.0775f), proportionOfHeight (0.1250f));
    c_Bloat->setBounds (proportionOfWidth (0.5012f), proportionOfHeight (0.2292f), proportionOfWidth (0.0462f), proportionOfHeight (0.3750f));
    label19->setBounds (proportionOfWidth (0.4825f), proportionOfHeight (0.5938f), proportionOfWidth (0.0775f), proportionOfHeight (0.1250f));
    c_Level->setBounds (proportionOfWidth (0.1700f), proportionOfHeight (0.2292f), proportionOfWidth (0.0462f), proportionOfHeight (0.3750f));
    c_iconSelectAll->setBounds (proportionOfWidth (0.0050f), proportionOfHeight (0.7708f), proportionOfWidth (0.0250f), proportionOfHeight (0.1771f));
    c_iconAddPos->setBounds (proportionOfWidth (0.0312f), proportionOfHeight (0.7708f), proportionOfWidth (0.0250f), proportionOfHeight (0.1771f));
    c_iconDelPos->setBounds (proportionOfWidth (0.0587f), proportionOfHeight (0.7708f), proportionOfWidth (0.0250f), proportionOfHeight (0.1771f));
    c_iconPaste->setBounds (proportionOfWidth (0.1525f), proportionOfHeight (0.7708f), proportionOfWidth (0.0250f), proportionOfHeight (0.1771f));
    c_iconDuplicate->setBounds (proportionOfWidth (0.0862f), proportionOfHeight (0.7708f), proportionOfWidth (0.0250f), proportionOfHeight (0.1771f));
    c_iconThinOut->setBounds (proportionOfWidth (0.1950f), proportionOfHeight (0.7708f), proportionOfWidth (0.0250f), proportionOfHeight (0.1771f));
    c_iconNormalize->setBounds (proportionOfWidth (0.2213f), proportionOfHeight (0.7708f), proportionOfWidth (0.0250f), proportionOfHeight (0.1771f));
    c_iconCopy->setBounds (proportionOfWidth (0.1263f), proportionOfHeight (0.7708f), proportionOfWidth (0.0250f), proportionOfHeight (0.1771f));
    c_iconSine->setBounds (proportionOfWidth (0.7125f) + proportionOfWidth (0.0437f), proportionOfHeight (0.3125f), proportionOfWidth (0.0437f), proportionOfHeight (0.1771f));
    c_iconHalfSine->setBounds ((proportionOfWidth (0.7125f) + proportionOfWidth (0.0437f)) + proportionOfWidth (0.0437f), proportionOfHeight (0.3125f), proportionOfWidth (0.0437f), proportionOfHeight (0.1771f));
    c_iconTriangle->setBounds ((proportionOfWidth (0.7125f) + proportionOfWidth (0.0437f)) + proportionOfWidth (0.0437f), proportionOfHeight (0.4896f), proportionOfWidth (0.0437f), proportionOfHeight (0.1771f));
    c_iconZeros->setBounds (proportionOfWidth (0.7125f), proportionOfHeight (0.3125f), proportionOfWidth (0.0437f), proportionOfHeight (0.1771f));
    c_iconSaw->setBounds (proportionOfWidth (0.7125f) + proportionOfWidth (0.0437f), proportionOfHeight (0.4896f), proportionOfWidth (0.0437f), proportionOfHeight (0.1771f));
    c_iconSquare->setBounds (proportionOfWidth (0.7125f), proportionOfHeight (0.4896f), proportionOfWidth (0.0437f), proportionOfHeight (0.1771f));
    c_iconRamp->setBounds (((proportionOfWidth (0.7125f) + proportionOfWidth (0.0437f)) + proportionOfWidth (0.0437f)) + proportionOfWidth (0.0437f), proportionOfHeight (0.4896f), proportionOfWidth (0.0437f), proportionOfHeight (0.1771f));
    c_iconRandom->setBounds ((((proportionOfWidth (0.7125f) + proportionOfWidth (0.0437f)) + proportionOfWidth (0.0437f)) + proportionOfWidth (0.0437f)) + proportionOfWidth (0.0437f), proportionOfHeight (0.4896f), proportionOfWidth (0.0437f), proportionOfHeight (0.1771f));
    c_iconSinc->setBounds (((proportionOfWidth (0.7125f) + proportionOfWidth (0.0437f)) + proportionOfWidth (0.0437f)) + proportionOfWidth (0.0437f), proportionOfHeight (0.3125f), proportionOfWidth (0.0437f), proportionOfHeight (0.1771f));
    c_iconReverse->setBounds (proportionOfWidth (0.9525f), proportionOfHeight (0.4896f), proportionOfWidth (0.0437f), proportionOfHeight (0.1771f));
    c_iconInvert->setBounds (proportionOfWidth (0.9525f), proportionOfHeight (0.3125f), proportionOfWidth (0.0437f), proportionOfHeight (0.1771f));
    c_iconClearWT->setBounds (proportionOfWidth (0.0050f) + proportionOfWidth (0.0437f), proportionOfHeight (0.0312f), proportionOfWidth (0.0437f), proportionOfHeight (0.1771f));
    c_iconLoadWT->setBounds (proportionOfWidth (0.0050f), proportionOfHeight (0.0312f), proportionOfWidth (0.0437f), proportionOfHeight (0.1771f));
    c_iconArcConnect->setBounds (proportionOfWidth (1.0112f), proportionOfHeight (0.3125f), proportionOfWidth (0.0250f), proportionOfHeight (0.1771f));
    c_iconRampConnect->setBounds (proportionOfWidth (1.0112f), proportionOfHeight (0.4896f), proportionOfWidth (0.0250f), proportionOfHeight (0.1771f));
    c_iconSave->setBounds ((proportionOfWidth (0.0050f) + proportionOfWidth (0.0437f)) + proportionOfWidth (0.0437f), proportionOfHeight (0.0312f), proportionOfWidth (0.0437f), proportionOfHeight (0.1771f));
    c_applyOptions->setBounds (proportionOfWidth (0.2650f), proportionOfHeight (0.7812f), proportionOfWidth (0.1400f), proportionOfHeight (0.1562f));
    c_generationOptions->setBounds (proportionOfWidth (0.4838f), proportionOfHeight (0.7812f), proportionOfWidth (0.1400f), proportionOfHeight (0.1562f));
    c_iconApply->setBounds (proportionOfWidth (0.4138f), proportionOfHeight (0.7708f), proportionOfWidth (0.0250f), proportionOfHeight (0.1771f));
    c_iconGenerate->setBounds (proportionOfWidth (0.6325f), proportionOfHeight (0.7708f), proportionOfWidth (0.0250f), proportionOfHeight (0.1771f));
    c_Connect->setBounds (proportionOfWidth (0.6425f), proportionOfHeight (0.2292f), proportionOfWidth (0.0462f), proportionOfHeight (0.3750f));
    label11->setBounds (proportionOfWidth (0.6300f), proportionOfHeight (0.5938f), proportionOfWidth (0.0775f), proportionOfHeight (0.1250f));
    c_applyMax->setBounds (proportionOfWidth (0.4387f), proportionOfHeight (0.7500f), proportionOfWidth (0.0300f), proportionOfHeight (0.1875f));
    c_WTableNext->setBounds (proportionOfWidth (0.4500f), proportionOfHeight (0.0312f), proportionOfWidth (0.0250f), proportionOfHeight (0.1771f));
    c_WTablePrevious->setBounds (proportionOfWidth (0.4250f), proportionOfHeight (0.0312f), proportionOfWidth (0.0250f), proportionOfHeight (0.1771f));
    label12->setBounds (proportionOfWidth (0.9275f), proportionOfHeight (0.0625f), proportionOfWidth (0.0775f), proportionOfHeight (0.1250f));
    c_soloSelection->setBounds (proportionOfWidth (0.9062f), proportionOfHeight (0.0417f), proportionOfWidth (0.0213f), proportionOfHeight (0.1771f));
    c_gridMode->setBounds (proportionOfWidth (0.8075f), proportionOfHeight (0.0521f), proportionOfWidth (0.0838f), proportionOfHeight (0.1562f));
    c_drawMode->setBounds (proportionOfWidth (0.7125f), proportionOfHeight (0.0521f), proportionOfWidth (0.0838f), proportionOfHeight (0.1562f));
    c_binMode->setBounds (proportionOfWidth (0.8075f), proportionOfHeight (0.7812f), proportionOfWidth (0.0838f), proportionOfHeight (0.1562f));
    c_binEditMode->setBounds (proportionOfWidth (0.7125f), proportionOfHeight (0.7812f), proportionOfWidth (0.0838f), proportionOfHeight (0.1562f));
    c_iconSampleHold->setBounds ((((proportionOfWidth (0.7125f) + proportionOfWidth (0.0437f)) + proportionOfWidth (0.0437f)) + proportionOfWidth (0.0437f)) + proportionOfWidth (0.0437f), proportionOfHeight (0.3125f), proportionOfWidth (0.0437f), proportionOfHeight (0.1771f));
    c_positionLabel->setBounds (proportionOfWidth (0.6250f), proportionOfHeight (0.0625f), proportionOfWidth (0.0862f), proportionOfHeight (0.1250f));
    label14->setBounds (proportionOfWidth (0.5025f), proportionOfHeight (0.0625f), proportionOfWidth (0.0775f), proportionOfHeight (0.1250f));
    c_iconMaximizeEditor->setBounds (getWidth() - 1 - proportionOfWidth (0.0150f), 1, proportionOfWidth (0.0150f), proportionOfHeight (0.1250f));
    c_snap->setBounds (proportionOfWidth (0.4813f), proportionOfHeight (0.0417f), proportionOfWidth (0.0213f), proportionOfHeight (0.1771f));
    c_stretch->setBounds (proportionOfWidth (0.5463f), proportionOfHeight (0.0417f), proportionOfWidth (0.0213f), proportionOfHeight (0.1771f));
    label16->setBounds (proportionOfWidth (0.5663f), proportionOfHeight (0.0625f), proportionOfWidth (0.0775f), proportionOfHeight (0.1250f));
    c_iconRecordWT->setBounds (((proportionOfWidth (0.0050f) + proportionOfWidth (0.0437f)) + proportionOfWidth (0.0437f)) + proportionOfWidth (0.0437f) - 2, proportionOfHeight (0.0312f), proportionOfWidth (0.0437f), juce::roundToInt (proportionOfHeight (0.1771f) * 1.0000f));
    //[UserResized] Add your own custom resize handling here..

	//c_editorOscilloscope->resized();
	c_wtName->applyFontToAllText(myEditor->getCurrentVASTLookAndFeel()->getTextEditorFont(*c_wtName.get()));
	m_iWTEditorZoom->setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxLeft, false, m_iWTEditorZoom->getWidth() * 0.45f, m_iWTEditorZoom->getHeight());
    //[/UserResized]
}

void VASTWaveTableEditorViewHeader::sliderValueChanged (juce::Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == c_genMax.get())
    {
        //[UserSliderCode_c_genMax] -- add your slider handling code here..
        //[/UserSliderCode_c_genMax]
    }
    else if (sliderThatWasMoved == c_applyMax.get())
    {
        //[UserSliderCode_c_applyMax] -- add your slider handling code here..
        //[/UserSliderCode_c_applyMax]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void VASTWaveTableEditorViewHeader::comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == c_applyOptions.get())
    {
        //[UserComboBoxCode_c_applyOptions] -- add your combo box handling code here..
        //[/UserComboBoxCode_c_applyOptions]
    }
    else if (comboBoxThatHasChanged == c_generationOptions.get())
    {
        //[UserComboBoxCode_c_generationOptions] -- add your combo box handling code here..
        //[/UserComboBoxCode_c_generationOptions]
    }
    else if (comboBoxThatHasChanged == c_gridMode.get())
    {
        //[UserComboBoxCode_c_gridMode] -- add your combo box handling code here..
		myWTEditor->setGridMode(c_gridMode->getSelectedItemIndex());
        //[/UserComboBoxCode_c_gridMode]
    }
    else if (comboBoxThatHasChanged == c_drawMode.get())
    {
        //[UserComboBoxCode_c_drawMode] -- add your combo box handling code here..
		myWTEditor->setDrawMode(c_drawMode->getSelectedItemIndex());
        //[/UserComboBoxCode_c_drawMode]
    }
    else if (comboBoxThatHasChanged == c_binMode.get())
    {
        //[UserComboBoxCode_c_binMode] -- add your combo box handling code here..
		myWTEditor->setBinMode(c_binMode->getSelectedItemIndex());
        //[/UserComboBoxCode_c_binMode]
    }
    else if (comboBoxThatHasChanged == c_binEditMode.get())
    {
        //[UserComboBoxCode_c_binEditMode] -- add your combo box handling code here..
		myWTEditor->setBinEditMode(c_binEditMode->getSelectedItemIndex());
        //[/UserComboBoxCode_c_binEditMode]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}

void VASTWaveTableEditorViewHeader::visibilityChanged()
{
    //[UserCode_visibilityChanged] -- Add your code here...
    //[/UserCode_visibilityChanged]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void VASTWaveTableEditorViewHeader::lookAndFeelChanged() {
	c_wtName->applyFontToAllText(myEditor->getCurrentVASTLookAndFeel()->getTextEditorFont(*c_wtName.get()));
	c_wtName->applyColourToAllText(myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colBoxText), true);
	c_wtName->setColour(TextEditor::backgroundColourId, myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colBoxTextBackground));
}

void VASTWaveTableEditorViewHeader::updateAll() {
	c_gridMode->setSelectedItemIndex(myProcessor->getGridMode(), NotificationType::sendNotification);
	c_drawMode->setSelectedItemIndex(myProcessor->getDrawMode(), NotificationType::sendNotification);
	c_binMode->setSelectedItemIndex(myProcessor->getBinMode(), NotificationType::sendNotification);
	c_binEditMode->setSelectedItemIndex(myProcessor->getBinEditMode(), NotificationType::sendNotification);
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="VASTWaveTableEditorViewHeader"
                 componentName="" parentClasses="public Component" constructorParams="AudioProcessorEditor *editor, AudioProcessor* processor, VASTWaveTableEditorComponent *wteditor "
                 variableInitialisers="myEditor((VASTAudioProcessorEditor*)editor), myProcessor((VASTAudioProcessor*)processor), myWTEditor(wteditor)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="800" initialHeight="96">
  <METHODS>
    <METHOD name="visibilityChanged()"/>
  </METHODS>
  <BACKGROUND backgroundColour="ff323e44">
    <RECT pos="0 0 0M 0M" fill="linear: -4% -1.538%, 94.667% 106.154%, 0=4fa2a7b5, 1=bd141618"
          hasStroke="0"/>
    <RECT pos="17% 58.333% 53% 12.5%" fill="linear: 7% 9.739%, 57% 12.522%, 0=ff3a4145, 1=ff33393c"
          hasStroke="0"/>
    <IMAGE pos="90% 77% 1.5% 17.708%" resource="zoom_in_svg" opacity="1.0"
           mode="1"/>
  </BACKGROUND>
  <LABEL name="new label" id="450f274481840cfe" memberName="label10" virtualName=""
         explicitFocusOrder="0" pos="10.125% 53.125% 7.875% 12.5%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="OSC D&#10;" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Code Pro Demo"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="242e032e25e506b3" memberName="label9" virtualName=""
         explicitFocusOrder="0" pos="10.125% 33.333% 7.875% 12.5%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="OSC C" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Code Pro Demo"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="7cb2c0a75e75e0c2" memberName="label7" virtualName=""
         explicitFocusOrder="0" pos="2.375% 33.333% 7.875% 12.5%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="OSC A" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Code Pro Demo"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="656ca1527af03374" memberName="label8" virtualName=""
         explicitFocusOrder="0" pos="2.375% 53.125% 7.875% 12.5%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="OSC B" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Code Pro Demo"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <SLIDER name="c_Phase" id="47a723282bb529a9" memberName="c_Phase" virtualName="VASTSlider"
          explicitFocusOrder="0" pos="21.75% 22.917% 4.625% 37.5%" tooltip="Change phase of selection"
          textboxtext="ffc9c9c9" textboxbkgd="ffffff" textboxoutline="808080"
          min="-100.0" max="100.0" int="0.001" style="RotaryVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="30"
          textBoxHeight="14" skewFactor="1.0" needsCallback="0"/>
  <LABEL name="new label" id="9050920a08f3ea65" memberName="label108"
         virtualName="" explicitFocusOrder="0" pos="15.375% 59.375% 7.75% 12.5%"
         textCol="ffe2e2e2" edTextCol="ff000000" edBkgCol="0" labelText="LEVEL"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Code Pro Demo" fontsize="11.0" kerning="0.0" bold="0"
         italic="0" justification="36"/>
  <LABEL name="new label" id="ebcfe7ba251e1261" memberName="label135"
         virtualName="" explicitFocusOrder="0" pos="20.125% 59.375% 7.75% 12.5%"
         textCol="ffe2e2e2" edTextCol="ff000000" edBkgCol="0" labelText="PHASE"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Code Pro Demo" fontsize="11.0" kerning="0.0" bold="0"
         italic="0" justification="36"/>
  <SLIDER name="c_Freq" id="d646964ef33d0ba1" memberName="c_Freq" virtualName="VASTSlider"
          explicitFocusOrder="0" pos="26.5% 22.917% 4.625% 37.5%" tooltip="Change frequency of selection"
          textboxtext="ffc9c9c9" textboxbkgd="ffffff" textboxoutline="808080"
          min="-4.0" max="4.0" int="0.001" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="30" textBoxHeight="14" skewFactor="1.0"
          needsCallback="0"/>
  <LABEL name="new label" id="128eb68295cb6409" memberName="label2" virtualName=""
         explicitFocusOrder="0" pos="24.75% 59.375% 7.75% 12.5%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="FREQ" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Code Pro Demo"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <SLIDER name="c_Offset" id="167585f25261bf82" memberName="c_Offset" virtualName="VASTSlider"
          explicitFocusOrder="0" pos="40.625% 22.917% 4.625% 37.5%" tooltip="Change DC offset of selection"
          textboxtext="ffc9c9c9" textboxbkgd="ffffff" textboxoutline="808080"
          min="-100.0" max="100.0" int="0.001" style="RotaryVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="30"
          textBoxHeight="14" skewFactor="1.0" needsCallback="0"/>
  <SLIDER name="c_bitDepth" id="4996bb4f1f244f7d" memberName="c_bitDepth"
          virtualName="VASTSlider" explicitFocusOrder="0" pos="35.875% 22.917% 4.625% 37.5%"
          tooltip="Change bit depth of selection" textboxtext="ffc9c9c9"
          textboxbkgd="ffffff" textboxoutline="808080" min="1.0" max="32.0"
          int="1.0" style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="30" textBoxHeight="14" skewFactor="1.0" needsCallback="0"/>
  <LABEL name="new label" id="d036a7d37d1c6d38" memberName="label3" virtualName=""
         explicitFocusOrder="0" pos="34% 59.375% 7.75% 12.5%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="BITS" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Code Pro Demo"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <LABEL name="new label" id="a9c7bc952a48ea4" memberName="label4" virtualName=""
         explicitFocusOrder="0" pos="38.875% 59.375% 7.75% 12.5%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="OFFSET" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Code Pro Demo"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <SLIDER name="c_Smooth" id="9d6b14ad6baff941" memberName="c_Smooth" virtualName="VASTSlider"
          explicitFocusOrder="0" pos="54.75% 22.917% 4.625% 37.5%" tooltip="Smooth selection"
          textboxtext="ffc9c9c9" textboxbkgd="ffffff" textboxoutline="808080"
          min="0.0" max="100.0" int="0.001" style="RotaryVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="30"
          textBoxHeight="14" skewFactor="1.0" needsCallback="0"/>
  <LABEL name="new label" id="20dee70a77501975" memberName="label5" virtualName=""
         explicitFocusOrder="0" pos="53.125% 59.375% 7.75% 12.5%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="SMOOTH" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Code Pro Demo"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <SLIDER name="c_Clean" id="83e56c23473a8ef7" memberName="c_Clean" virtualName="VASTSlider"
          explicitFocusOrder="0" pos="59.5% 22.917% 4.625% 37.5%" tooltip="Clean selection (start and end of cycle)"
          textboxtext="ffc9c9c9" textboxbkgd="ffffff" textboxoutline="808080"
          min="0.0" max="100.0" int="0.001" style="RotaryVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="30"
          textBoxHeight="14" skewFactor="1.0" needsCallback="0"/>
  <LABEL name="new label" id="b5a40a334160e5ea" memberName="label6" virtualName=""
         explicitFocusOrder="0" pos="58.25% 59.375% 7.75% 12.5%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="CLEAN" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Code Pro Demo"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <TOGGLEBUTTON name="c_oscA" id="e2dfc243b8a3e0e8" memberName="c_oscA" virtualName=""
                explicitFocusOrder="0" pos="0.5% 31.25% 2.125% 17.708%" buttonText=""
                connectedEdges="8" needsCallback="0" radioGroupId="11" state="0"/>
  <TOGGLEBUTTON name="c_oscB" id="c712aba8dd21d213" memberName="c_oscB" virtualName=""
                explicitFocusOrder="0" pos="0.5% 51.042% 2.125% 17.708%" buttonText=""
                connectedEdges="12" needsCallback="0" radioGroupId="11" state="0"/>
  <TOGGLEBUTTON name="c_oscC" id="d73609719438c19f" memberName="c_oscC" virtualName=""
                explicitFocusOrder="0" pos="8% 31.25% 2.125% 17.708%" buttonText=""
                connectedEdges="12" needsCallback="0" radioGroupId="11" state="0"/>
  <TOGGLEBUTTON name="c_oscD" id="197159402e1a9ff" memberName="c_oscD" virtualName=""
                explicitFocusOrder="0" pos="8% 50% 2.125% 17.708%" buttonText=""
                connectedEdges="4" needsCallback="0" radioGroupId="11" state="0"/>
  <SLIDER name="m_iWTEditorZoom" id="6a281b071d356eaa" memberName="m_iWTEditorZoom"
          virtualName="VASTParameterSlider" explicitFocusOrder="1" pos="92% 77.083% 7.875% 16.667%"
          tooltip="Zoom frequency domain display" min="1.0" max="16.0"
          int="1.0" style="IncDecButtons" textBoxPos="TextBoxLeft" textBoxEditable="1"
          textBoxWidth="28" textBoxHeight="20" skewFactor="1.0" needsCallback="0"/>
  <TEXTEDITOR name="c_wtName " id="1f7770d839b337bf" memberName="c_wtName"
              virtualName="" explicitFocusOrder="0" pos="23.25% 4.167% 18.875% 15.625%"
              initialText="" multiline="0" retKeyStartsLine="0" readonly="0"
              scrollbars="0" caret="1" popupmenu="1"/>
  <LABEL name="new label" id="7969f6422cf2ee4e" memberName="label13" virtualName=""
         explicitFocusOrder="0" pos="14.375% 6.25% 9% 12.5%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="TABLE" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Code Pro Demo"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="34"/>
  <SLIDER name="c_FreqMod" id="a78311261b14467c" memberName="c_FreqMod"
          virtualName="VASTSlider" explicitFocusOrder="0" pos="31.125% 22.917% 4.625% 37.5%"
          tooltip="Frequency modulation of selection" textboxtext="ffc9c9c9"
          textboxbkgd="ffffff" textboxoutline="808080" min="0.0" max="20.0"
          int="0.001" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="30" textBoxHeight="14" skewFactor="1.0"
          needsCallback="0"/>
  <LABEL name="new label" id="2dd2467c09c8de99" memberName="label15" virtualName=""
         explicitFocusOrder="0" pos="29.5% 59.375% 7.75% 12.5%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="FMOD" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Code Pro Demo"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <SLIDER name="c_genMax" id="4a7cec6a60f6220f" memberName="c_genMax" virtualName="VASTSlider"
          explicitFocusOrder="0" pos="65.625% 76.042% 3% 18.75%" tooltip="Maximum modulation value of generation"
          textboxtext="ffc9c9c9" textboxbkgd="ffffff" textboxoutline="808080"
          min="0.0" max="100.0" int="0.001" style="RotaryVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="30"
          textBoxHeight="14" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="c_Bend" id="c8510340e1ce223b" memberName="c_Bend" virtualName="VASTSlider"
          explicitFocusOrder="0" pos="45.375% 22.917% 4.625% 37.5%" tooltip="Bend selection"
          textboxtext="ffc9c9c9" textboxbkgd="ffffff" textboxoutline="808080"
          min="-100.0" max="100.0" int="0.001" style="RotaryVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="30"
          textBoxHeight="14" skewFactor="1.0" needsCallback="0"/>
  <LABEL name="new label" id="be156406dcefa94d" memberName="label18" virtualName=""
         explicitFocusOrder="0" pos="43.75% 59.375% 7.75% 12.5%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="BEND" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Code Pro Demo"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <SLIDER name="c_Bloat" id="eeab075fdb98e324" memberName="c_Bloat" virtualName="VASTSlider"
          explicitFocusOrder="0" pos="50.125% 22.917% 4.625% 37.5%" tooltip="Inflate selection"
          textboxtext="ffc9c9c9" textboxbkgd="ffffff" textboxoutline="808080"
          min="-100.0" max="100.0" int="0.001" style="RotaryVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="30"
          textBoxHeight="14" skewFactor="1.0" needsCallback="0"/>
  <LABEL name="new label" id="220bf9ece960254e" memberName="label19" virtualName=""
         explicitFocusOrder="0" pos="48.25% 59.375% 7.75% 12.5%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="BLOAT" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Code Pro Demo"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <SLIDER name="c_Level" id="780d89d221ce1a79" memberName="c_Level" virtualName="VASTSlider"
          explicitFocusOrder="0" pos="17% 22.917% 4.625% 37.5%" tooltip="Change level of selection"
          textboxtext="ffc9c9c9" textboxbkgd="ffffff" textboxoutline="808080"
          min="-100.0" max="100.0" int="0.01" style="RotaryVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="30"
          textBoxHeight="14" skewFactor="1.0" needsCallback="0"/>
  <GENERICCOMPONENT name="c_iconSelectAll" id="17499d966c201187" memberName="c_iconSelectAll"
                    virtualName="" explicitFocusOrder="0" pos="0.5% 77.083% 2.5% 17.708%"
                    class="VASTDrawableButton" params="&quot;c_iconSelectAll&quot;, select_all_svg, select_all_svgSize, &quot;Select all positions and whole wavetable&quot;"/>
  <GENERICCOMPONENT name="c_iconAddPos" id="26802ded8e5f5c3" memberName="c_iconAddPos"
                    virtualName="" explicitFocusOrder="0" pos="3.125% 77.083% 2.5% 17.708%"
                    class="VASTDrawableButton" params="&quot;c_iconAddPos&quot;, add_svg, add_svgSize, &quot;Add position after selected position&quot;"/>
  <GENERICCOMPONENT name="c_iconDelPos" id="1f6315974ecb9524" memberName="c_iconDelPos"
                    virtualName="" explicitFocusOrder="0" pos="5.875% 77.083% 2.5% 17.708%"
                    class="VASTDrawableButton" params="&quot;c_iconDelPos&quot;, delete_svg, delete_svgSize, &quot;Delete multiselected positions&quot;"/>
  <GENERICCOMPONENT name="c_iconPaste" id="238413c3554d4cd4" memberName="c_iconPaste"
                    virtualName="" explicitFocusOrder="0" pos="15.25% 77.083% 2.5% 17.708%"
                    class="VASTDrawableButton" params="&quot;c_iconPaste&quot;, paste_svg, paste_svgSize, &quot;Insert multiselected positions&quot;"/>
  <GENERICCOMPONENT name="c_iconDuplicate" id="d0c99b8621fcf79f" memberName="c_iconDuplicate"
                    virtualName="" explicitFocusOrder="0" pos="8.625% 77.083% 2.5% 17.708%"
                    class="VASTDrawableButton" params="&quot;c_iconDuplicate&quot;,duplicate_svg, duplicate_svgSize, &quot;Duplicate multiselected positions&quot;"/>
  <GENERICCOMPONENT name="c_iconThinOut" id="ba6139d01d1c4367" memberName="c_iconThinOut"
                    virtualName="" explicitFocusOrder="0" pos="19.5% 77.083% 2.5% 17.708%"
                    class="VASTDrawableButton" params="&quot;c_iconThinOut&quot;,minus_svg, minus_svgSize, &quot;Delete even positions in  multiselection&quot;"/>
  <GENERICCOMPONENT name="c_iconNormalize" id="9b8db803e32ff61f" memberName="c_iconNormalize"
                    virtualName="" explicitFocusOrder="0" pos="22.125% 77.083% 2.5% 17.708%"
                    class="VASTDrawableButton" params="&quot;c_iconNormalize&quot;,normalize3_svg, normalize3_svgSize, &quot;Normalize selected positions&quot;"/>
  <GENERICCOMPONENT name="c_iconCopy" id="9aec362d3f3eeaae" memberName="c_iconCopy"
                    virtualName="" explicitFocusOrder="0" pos="12.625% 77.083% 2.5% 17.708%"
                    class="VASTDrawableButton" params="&quot;c_iconCopy&quot;, copy_svg, copy_svgSize, &quot;Copy multiselected positions&quot;"/>
  <GENERICCOMPONENT name="c_iconSine" id="956a104b78162128" memberName="c_iconSine"
                    virtualName="" explicitFocusOrder="0" pos="0R 31.25% 4.375% 17.708%"
                    posRelativeX="415e8d0582cd584a" class="VASTDrawableButton" params="&quot;c_iconSine&quot;, waves_sine_svg, waves_sine_svgSize, &quot;Initialize  selection of position with sine wave&quot;"/>
  <GENERICCOMPONENT name="c_iconHalfSine" id="58c465bc2346cdd6" memberName="c_iconHalfSine"
                    virtualName="" explicitFocusOrder="0" pos="0R 31.25% 4.375% 17.708%"
                    posRelativeX="956a104b78162128" class="VASTDrawableButton" params="&quot;c_iconHalfSine&quot;, waves_halfsine_svg, waves_halfsine_svgSize, &quot;Initialize  selection of position with half (upper) sine wave&quot;"/>
  <GENERICCOMPONENT name="c_iconTriangle" id="2b2b8cfc818adf98" memberName="c_iconTriangle"
                    virtualName="" explicitFocusOrder="0" pos="0R 48.958% 4.375% 17.708%"
                    posRelativeX="1296379f0a16af1c" class="VASTDrawableButton" params="&quot;c_iconTriangle&quot;, waves_triangle_svg, waves_triangle_svgSize, &quot;Initialize  selection of position with triangle wave&quot;"/>
  <GENERICCOMPONENT name="c_iconZeros" id="415e8d0582cd584a" memberName="c_iconZeros"
                    virtualName="" explicitFocusOrder="0" pos="71.25% 31.25% 4.375% 17.708%"
                    class="VASTDrawableButton" params="&quot;c_iconZeros&quot;, waves_zeros_svg, waves_zeros_svgSize, &quot;Initialize selection of position with zeros&quot;"/>
  <GENERICCOMPONENT name="c_iconSaw" id="1296379f0a16af1c" memberName="c_iconSaw"
                    virtualName="" explicitFocusOrder="0" pos="0R 48.958% 4.375% 17.708%"
                    posRelativeX="6cffcc4570fe0585" class="VASTDrawableButton" params="&quot;c_iconSaw&quot;, waves_saw_svg, waves_saw_svgSize, &quot;Initialize  selection of position with sawtooth wave&quot;"/>
  <GENERICCOMPONENT name="c_iconSquare" id="6cffcc4570fe0585" memberName="c_iconSquare"
                    virtualName="" explicitFocusOrder="0" pos="71.25% 48.958% 4.375% 17.708%"
                    class="VASTDrawableButton" params="&quot;c_iconSquare&quot;, waves_square_svg, waves_square_svgSize, &quot;Initialize  selection of position with square / pulse wave&quot; "/>
  <GENERICCOMPONENT name="c_iconRamp" id="88916ec4694c0132" memberName="c_iconRamp"
                    virtualName="" explicitFocusOrder="0" pos="0R 48.958% 4.375% 17.708%"
                    posRelativeX="2b2b8cfc818adf98" class="VASTDrawableButton" params="&quot;c_iconRamp&quot;, waves_ramp_svg, waves_ramp_svgSize, &quot;Initialize  selection of position with ramp&quot;"/>
  <GENERICCOMPONENT name="c_iconRandom" id="36b6eda502933c61" memberName="c_iconRandom"
                    virtualName="" explicitFocusOrder="0" pos="0R 48.958% 4.375% 17.708%"
                    posRelativeX="88916ec4694c0132" class="VASTDrawableButton" params="&quot;c_iconRandom&quot;,waves_random_svg, waves_random_svgSize, &quot;Initialize  selection of position with random wave&quot;"/>
  <GENERICCOMPONENT name="c_iconSinc" id="f6736563a76ce0a4" memberName="c_iconSinc"
                    virtualName="" explicitFocusOrder="0" pos="0R 31.25% 4.375% 17.708%"
                    posRelativeX="58c465bc2346cdd6" class="VASTDrawableButton" params="&quot;c_iconSinc&quot;,waves_sinc_svg, waves_sinc_svgSize, &quot;Initialize  selection of position with sinc wave&quot;"/>
  <GENERICCOMPONENT name="c_iconReverse" id="26692e0a72a0ae83" memberName="c_iconReverse"
                    virtualName="" explicitFocusOrder="0" pos="95.25% 48.958% 4.375% 17.708%"
                    class="VASTDrawableButton" params="&quot;c_iconReverse&quot;,reverse_svg, reverse_svgSize, &quot;Reverse selection of position&quot;"/>
  <GENERICCOMPONENT name="c_iconInvert" id="33089cb93795c7dd" memberName="c_iconInvert"
                    virtualName="" explicitFocusOrder="0" pos="95.25% 31.25% 4.375% 17.708%"
                    class="VASTDrawableButton" params="&quot;c_iconInvert&quot;, invert_svg, invert_svgSize, &quot;Invert selection of position&quot;"/>
  <GENERICCOMPONENT name="c_iconClearWT" id="e87cdaeb46838f2a" memberName="c_iconClearWT"
                    virtualName="" explicitFocusOrder="0" pos="0R 3.125% 4.375% 17.708%"
                    posRelativeX="821c06a6d0b8e0ca" class="VASTDrawableButton" params="&quot;c_iconClearWT&quot;,close_svg, close_svgSize, &quot;Clear wavetable&quot;"/>
  <GENERICCOMPONENT name="c_iconLoadWT" id="821c06a6d0b8e0ca" memberName="c_iconLoadWT"
                    virtualName="" explicitFocusOrder="0" pos="0.5% 3.125% 4.375% 17.708%"
                    class="VASTDrawableButton" params="&quot;c_iconLoadWT&quot;,load_svg, load_svgSize, &quot;Load wavetable&quot;"/>
  <GENERICCOMPONENT name="c_iconArcConnect" id="85b2aa91db42cd81" memberName="c_iconArcConnect"
                    virtualName="" explicitFocusOrder="0" pos="101.125% 31.25% 2.5% 17.708%"
                    class="VASTDrawableButton" params="&quot;c_iconArcConnect&quot;, arc_connect_svg, arc_connect_svgSize, &quot;Draw arc from first value of selection to last value of selection in postion&quot;"/>
  <GENERICCOMPONENT name="c_iconRampConnect" id="9f4202f6ee71e8ec" memberName="c_iconRampConnect"
                    virtualName="" explicitFocusOrder="0" pos="101.125% 48.958% 2.5% 17.708%"
                    class="VASTDrawableButton" params="&quot;c_iconRampConnect&quot;, ramp_connect_svg, ramp_connect_svgSize, &quot;Draw ramp from first value of selection to last value of selection in postion&quot;"/>
  <GENERICCOMPONENT name="c_iconSave" id="3122dea5fcf9e0be" memberName="c_iconSave"
                    virtualName="" explicitFocusOrder="0" pos="0R 3.125% 4.375% 17.708%"
                    posRelativeX="e87cdaeb46838f2a" class="VASTDrawableButton" params="&quot;c_iconSave&quot;, save_svg, save_svgSize, &quot;Save wavetable to file&quot;"/>
  <COMBOBOX name="c_applyOptions" id="90780feb0180733d" memberName="c_applyOptions"
            virtualName="VASTComboBox" explicitFocusOrder="1" pos="26.5% 78.125% 14% 15.625%"
            tooltip="Option that shall be applied from begin to end of multi selection"
            editable="0" layout="33" items="Interpolation&#10;Amplitude Modulation&#10;Mix In&#10;Convolution&#10;Time &gt; Frequency&#10;Frequency &gt; Time&#10;Normalize (Table)&#10;Normalize (Each)"
            textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <COMBOBOX name="c_generationOptions" id="523d6b83bf6d50d3" memberName="c_generationOptions"
            virtualName="VASTComboBox" explicitFocusOrder="1" pos="48.375% 78.125% 14% 15.625%"
            tooltip="Option that shall be generated" editable="0" layout="33"
            items="HardSync&#10;Pulse Mod (PWM)&#10;Frequency Mod (FM)&#10;Bloat&#10;Bend&#10;Formants (Male)&#10;Formants (Female)&#10;Harmonic Morph&#10;Harmonic Reduce&#10;Highpass Filter&#10;Lowpass Filter&#10;Lowpass Soft&#10;Bandpass Filter&#10;Pulse Filter&#10;Comb Filter"
            textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <GENERICCOMPONENT name="c_iconApply" id="a98168546452220a" memberName="c_iconApply"
                    virtualName="" explicitFocusOrder="0" pos="41.375% 77.083% 2.5% 17.708%"
                    class="VASTDrawableButton" params="&quot;c_iconApply&quot;,apply_svg, apply_svgSize, &quot;Apply option from begin to end of multi selection&quot;"/>
  <GENERICCOMPONENT name="c_iconGenerate" id="d2d42417315c483c" memberName="c_iconGenerate"
                    virtualName="" explicitFocusOrder="0" pos="63.25% 77.083% 2.5% 17.708%"
                    class="VASTDrawableButton" params="&quot;c_iconGenerate&quot;,generate_svg, generate_svgSize, &quot;Generate wavetable per option based on selected single cycle&quot;"/>
  <SLIDER name="c_Connect" id="83c70ecdc5c26b3f" memberName="c_Connect"
          virtualName="VASTSlider" explicitFocusOrder="0" pos="64.25% 22.917% 4.625% 37.5%"
          tooltip="Connect start value of selection with end value of selection with arc or ramp "
          textboxtext="ffc9c9c9" textboxbkgd="ffffff" textboxoutline="808080"
          min="-100.0" max="100.0" int="0.001" style="RotaryVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="30"
          textBoxHeight="14" skewFactor="1.0" needsCallback="0"/>
  <LABEL name="new label" id="6d1de2d51afec5a2" memberName="label11" virtualName=""
         explicitFocusOrder="0" pos="63% 59.375% 7.75% 12.5%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="CNECT" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Code Pro Demo"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <SLIDER name="c_applyMax" id="1062b6a8341eb892" memberName="c_applyMax"
          virtualName="VASTSlider" explicitFocusOrder="0" pos="43.875% 75% 3% 18.75%"
          tooltip="Maximum modulation value for applied modulation" textboxtext="ffc9c9c9"
          textboxbkgd="ffffff" textboxoutline="808080" min="0.0" max="100.0"
          int="0.001" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="30" textBoxHeight="14" skewFactor="1.0"
          needsCallback="1"/>
  <TEXTBUTTON name="c_WTableNext" id="8f9bd8b448847d40" memberName="c_WTableNext"
              virtualName="" explicitFocusOrder="0" pos="45% 3.125% 2.5% 17.708%"
              tooltip="Load next wavetable" buttonText="&gt;" connectedEdges="0"
              needsCallback="0" radioGroupId="0"/>
  <TEXTBUTTON name="c_WTablePrevious" id="b58b0ccbb24d70e6" memberName="c_WTablePrevious"
              virtualName="" explicitFocusOrder="0" pos="42.5% 3.125% 2.5% 17.708%"
              tooltip="Load previous wavetable" buttonText="&lt;" connectedEdges="0"
              needsCallback="0" radioGroupId="0"/>
  <LABEL name="new label" id="19372a24ea9dc2c0" memberName="label12" virtualName=""
         explicitFocusOrder="0" pos="92.75% 6.25% 7.75% 12.5%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="SOLO" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Code Pro Demo"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <TOGGLEBUTTON name="c_soloSelection" id="e840f09d6b856837" memberName="c_soloSelection"
                virtualName="" explicitFocusOrder="0" pos="90.625% 4.167% 2.125% 17.708%"
                tooltip="Only play the selected wavetable positions in oscillator"
                buttonText="" connectedEdges="0" needsCallback="0" radioGroupId="0"
                state="0"/>
  <COMBOBOX name="c_gridMode" id="2bfaf1a4fde4fc07" memberName="c_gridMode"
            virtualName="VASTComboBox" explicitFocusOrder="1" pos="80.75% 5.208% 8.375% 15.625%"
            tooltip="Grid mode in wavetable editor" editable="0" layout="33"
            items="No Grid&#10;4x4&#10;8x8&#10;16x16&#10;32x32&#10;64x64&#10;128x128"
            textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <COMBOBOX name="c_drawMode" id="fa495fb2a9d2dda" memberName="c_drawMode"
            virtualName="VASTComboBox" explicitFocusOrder="1" pos="71.25% 5.208% 8.375% 15.625%"
            tooltip="Draw mode in wavetable editor" editable="0" layout="33"
            items="Select&#10;Line/Arc&#10;Draw" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <COMBOBOX name="c_binMode" id="780c6f4634cd3ae7" memberName="c_binMode"
            virtualName="VASTComboBox" explicitFocusOrder="1" pos="80.75% 78.125% 8.375% 15.625%"
            tooltip="Bin mode for editing frequency domain" editable="0"
            layout="33" items="Clip&#10;Normalize" textWhenNonSelected=""
            textWhenNoItems="(no choices)"/>
  <COMBOBOX name="c_binEditMode" id="540cefbca3dff824" memberName="c_binEditMode"
            virtualName="VASTComboBox" explicitFocusOrder="1" pos="71.25% 78.125% 8.375% 15.625%"
            tooltip="Bin edit mode for editing frequency domain" editable="0"
            layout="33" items="SingleBin&#10;Every2nd&#10;Every3rd&#10;Pile&#10;DragShift&#10;DragStretch"
            textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <GENERICCOMPONENT name="c_iconSampleHold" id="67170b731f13a2a6" memberName="c_iconSampleHold"
                    virtualName="" explicitFocusOrder="0" pos="0R 31.25% 4.375% 17.708%"
                    posRelativeX="f6736563a76ce0a4" class="VASTDrawableButton" params="&quot;c_iconSampleHold&quot;,waves_samplehold_svg, waves_samplehold_svgSize, &quot;Initialize  selection of position with sample and hold wave&quot;"/>
  <LABEL name="c_positionLabel" id="635b3821ed9d82a" memberName="c_positionLabel"
         virtualName="" explicitFocusOrder="0" pos="62.5% 6.25% 8.625% 12.5%"
         textCol="ffa4a4a4" edTextCol="ff000000" edBkgCol="0" labelText="-"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Code Pro Demo" fontsize="11.0" kerning="0.0" bold="0"
         italic="0" justification="36"/>
  <LABEL name="new label" id="bc0c315892ddf7cb" memberName="label14" virtualName=""
         explicitFocusOrder="0" pos="50.25% 6.25% 7.75% 12.5%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="SNAP" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Code Pro Demo"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <GENERICCOMPONENT name="c_iconMaximizeEditor" id="ded548631676712b" memberName="c_iconMaximizeEditor"
                    virtualName="" explicitFocusOrder="0" pos="1Rr 1 1.5% 12.5%"
                    class="VASTDrawableButton" params="&quot;c_iconMaximize&quot;, arrow_right_corner_svg, arrow_right_corner_svgSize, &quot;Maximize area&quot;"/>
  <TOGGLEBUTTON name="c_snap" id="ec7ed958fca494e8" memberName="c_snap" virtualName=""
                explicitFocusOrder="0" pos="48.125% 4.167% 2.125% 17.708%" tooltip="Snap editor functions to zero crossings"
                buttonText="" connectedEdges="0" needsCallback="0" radioGroupId="0"
                state="0"/>
  <TOGGLEBUTTON name="c_stretch" id="cbf4b6938774aa7" memberName="c_stretch"
                virtualName="" explicitFocusOrder="0" pos="54.625% 4.167% 2.125% 17.708%"
                tooltip="Stretch editor functions to zero crossings" buttonText=""
                connectedEdges="0" needsCallback="0" radioGroupId="0" state="0"/>
  <LABEL name="new label" id="39af1f2d4c03ec68" memberName="label16" virtualName=""
         explicitFocusOrder="0" pos="56.625% 6.25% 7.75% 12.5%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="STRETCH" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Code Pro Demo"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <GENERICCOMPONENT name="c_iconRecordWT" id="e6d158839da639a" memberName="c_iconRecordWT"
                    virtualName="" explicitFocusOrder="0" pos="2R 3.125% 4.375% 100%"
                    posRelativeX="3122dea5fcf9e0be" posRelativeH="3122dea5fcf9e0be"
                    class="VASTDrawableButton" params="&quot;c_iconRecordWT&quot;, record_svg, record_svgSize, &quot;Record single cycle changes into wavetable&quot;"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif

//==============================================================================
// Binary resources - be careful not to edit any of these sections!

// JUCER_RESOURCE: add_svg, 1396, "../../Resources/Icons/add.svg"
static const unsigned char resource_VASTWaveTableEditorViewHeader_add_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,70,45,56,
34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,32,119,105,100,116,104,61,34,50,49,112,120,34,32,104,101,105,103,104,116,61,34,50,48,112,120,34,32,118,105,101,119,
66,111,120,61,34,48,32,48,32,50,49,32,50,48,34,32,118,101,114,115,105,111,110,61,34,49,46,49,34,32,120,109,108,110,115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,
47,115,118,103,34,32,120,109,108,110,115,58,120,108,105,110,107,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,120,108,105,110,107,34,62,10,32,32,32,32,60,33,45,
45,32,71,101,110,101,114,97,116,111,114,58,32,83,107,101,116,99,104,32,51,46,56,46,49,32,40,50,57,54,56,55,41,32,45,32,104,116,116,112,58,47,47,119,119,119,46,98,111,104,101,109,105,97,110,99,111,100,
105,110,103,46,99,111,109,47,115,107,101,116,99,104,32,45,45,62,10,32,32,32,32,60,116,105,116,108,101,62,112,108,117,115,32,91,35,49,52,53,53,93,60,47,116,105,116,108,101,62,10,32,32,32,32,60,100,101,
115,99,62,67,114,101,97,116,101,100,32,119,105,116,104,32,83,107,101,116,99,104,46,60,47,100,101,115,99,62,10,32,32,32,32,60,100,101,102,115,62,60,47,100,101,102,115,62,10,32,32,32,32,60,103,32,105,100,
61,34,80,97,103,101,45,49,34,32,115,116,114,111,107,101,61,34,110,111,110,101,34,32,115,116,114,111,107,101,45,119,105,100,116,104,61,34,49,34,32,102,105,108,108,61,34,110,111,110,101,34,32,102,105,108,
108,45,114,117,108,101,61,34,101,118,101,110,111,100,100,34,62,10,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,68,114,105,98,98,98,108,101,45,68,97,114,107,45,80,114,101,118,105,101,119,34,32,116,114,
97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,45,50,53,57,46,48,48,48,48,48,48,44,32,45,52,56,48,46,48,48,48,48,48,48,41,34,32,102,105,108,108,61,34,35,70,70,70,70,70,70,34,62,
10,32,32,32,32,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,105,99,111,110,115,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,53,54,46,48,48,48,48,48,48,44,32,49,
54,48,46,48,48,48,48,48,48,41,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,60,112,97,116,104,32,100,61,34,77,50,49,52,46,53,53,44,51,50,54,32,76,50,49,50,46,52,53,44,51,50,54,32,76,50,49,50,
46,52,53,44,51,50,57,32,76,50,48,57,46,51,44,51,50,57,32,76,50,48,57,46,51,44,51,51,49,32,76,50,49,50,46,52,53,44,51,51,49,32,76,50,49,50,46,52,53,44,51,51,52,32,76,50,49,52,46,53,53,44,51,51,52,32,76,
50,49,52,46,53,53,44,51,51,49,32,76,50,49,55,46,55,44,51,51,49,32,76,50,49,55,46,55,44,51,50,57,32,76,50,49,52,46,53,53,44,51,50,57,32,76,50,49,52,46,53,53,44,51,50,54,32,90,32,77,50,50,49,46,57,44,51,
51,52,32,76,50,50,52,44,51,51,52,32,76,50,50,52,44,51,50,54,32,76,50,50,49,46,57,44,51,50,54,32,76,50,50,49,46,57,44,51,51,52,32,90,32,77,50,50,49,46,57,44,51,51,56,32,76,50,49,57,46,56,44,51,51,56,32,
76,50,49,57,46,56,44,51,52,48,32,76,50,50,52,44,51,52,48,32,76,50,50,52,44,51,51,54,32,76,50,50,49,46,57,44,51,51,54,32,76,50,50,49,46,57,44,51,51,56,32,90,32,77,50,49,57,46,56,44,51,50,48,32,76,50,49,
57,46,56,44,51,50,50,32,76,50,50,49,46,57,44,51,50,50,32,76,50,50,49,46,57,44,51,50,52,32,76,50,50,52,44,51,50,52,32,76,50,50,52,44,51,50,48,32,76,50,49,57,46,56,44,51,50,48,32,90,32,77,50,48,51,44,51,
51,52,32,76,50,48,53,46,49,44,51,51,52,32,76,50,48,53,46,49,44,51,50,54,32,76,50,48,51,44,51,50,54,32,76,50,48,51,44,51,51,52,32,90,32,77,50,48,53,46,49,44,51,51,54,32,76,50,48,51,44,51,51,54,32,76,50,
48,51,44,51,52,48,32,76,50,48,55,46,50,44,51,52,48,32,76,50,48,55,46,50,44,51,51,56,32,76,50,48,53,46,49,44,51,51,56,32,76,50,48,53,46,49,44,51,51,54,32,90,32,77,50,48,51,44,51,50,48,32,76,50,48,51,44,
51,50,52,32,76,50,48,53,46,49,44,51,50,52,32,76,50,48,53,46,49,44,51,50,50,32,76,50,48,55,46,50,44,51,50,50,32,76,50,48,55,46,50,44,51,50,48,32,76,50,48,51,44,51,50,48,32,90,32,77,50,48,57,46,51,44,51,
52,48,32,76,50,49,55,46,55,44,51,52,48,32,76,50,49,55,46,55,44,51,51,56,32,76,50,48,57,46,51,44,51,51,56,32,76,50,48,57,46,51,44,51,52,48,32,90,32,77,50,48,57,46,51,44,51,50,50,32,76,50,49,55,46,55,44,
51,50,50,32,76,50,49,55,46,55,44,51,50,48,32,76,50,48,57,46,51,44,51,50,48,32,76,50,48,57,46,51,44,51,50,50,32,90,34,32,105,100,61,34,112,108,117,115,45,91,35,49,52,53,53,93,34,62,60,47,112,97,116,104,
62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,60,47,103,62,10,60,47,115,118,103,62,0,0};

const char* VASTWaveTableEditorViewHeader::add_svg = (const char*) resource_VASTWaveTableEditorViewHeader_add_svg;
const int VASTWaveTableEditorViewHeader::add_svgSize = 1396;

// JUCER_RESOURCE: apply_svg, 1969, "../../Resources/Icons/apply.svg"
static const unsigned char resource_VASTWaveTableEditorViewHeader_apply_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,70,45,
56,34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,32,119,105,100,116,104,61,34,50,48,112,120,34,32,104,101,105,103,104,116,61,34,49,55,112,120,34,32,118,105,101,119,
66,111,120,61,34,48,32,48,32,50,48,32,49,55,34,32,118,101,114,115,105,111,110,61,34,49,46,49,34,32,120,109,108,110,115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,
47,115,118,103,34,32,120,109,108,110,115,58,120,108,105,110,107,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,120,108,105,110,107,34,62,10,32,32,32,32,60,33,45,
45,32,71,101,110,101,114,97,116,111,114,58,32,83,107,101,116,99,104,32,51,46,56,46,49,32,40,50,57,54,56,55,41,32,45,32,104,116,116,112,58,47,47,119,119,119,46,98,111,104,101,109,105,97,110,99,111,100,
105,110,103,46,99,111,109,47,115,107,101,116,99,104,32,45,45,62,10,32,32,32,32,60,116,105,116,108,101,62,115,107,101,116,99,104,32,91,35,49,50,53,93,60,47,116,105,116,108,101,62,10,32,32,32,32,60,100,
101,115,99,62,67,114,101,97,116,101,100,32,119,105,116,104,32,83,107,101,116,99,104,46,60,47,100,101,115,99,62,10,32,32,32,32,60,100,101,102,115,62,60,47,100,101,102,115,62,10,32,32,32,32,60,103,32,105,
100,61,34,80,97,103,101,45,49,34,32,115,116,114,111,107,101,61,34,110,111,110,101,34,32,115,116,114,111,107,101,45,119,105,100,116,104,61,34,49,34,32,102,105,108,108,61,34,110,111,110,101,34,32,102,105,
108,108,45,114,117,108,101,61,34,101,118,101,110,111,100,100,34,62,10,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,68,114,105,98,98,98,108,101,45,68,97,114,107,45,80,114,101,118,105,101,119,34,32,116,
114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,45,52,50,48,46,48,48,48,48,48,48,44,32,45,55,54,48,49,46,48,48,48,48,48,48,41,34,32,102,105,108,108,61,34,35,70,70,70,70,70,70,
34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,105,99,111,110,115,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,53,54,46,48,48,48,48,48,48,44,
32,49,54,48,46,48,48,48,48,48,48,41,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,60,112,97,116,104,32,100,61,34,77,51,54,56,46,50,51,56,51,50,53,44,55,52,52,51,46,51,52,57,54,57,32,76,51,54,
57,46,51,57,57,51,50,53,44,55,52,52,54,46,49,54,51,57,55,32,76,51,54,54,46,55,51,51,51,50,53,44,55,52,52,54,46,49,54,51,57,55,32,76,51,54,56,46,50,51,56,51,50,53,44,55,52,52,51,46,51,52,57,54,57,32,90,
32,77,51,55,55,46,53,56,52,51,50,53,44,55,52,52,56,46,50,50,52,50,32,76,51,56,48,46,53,55,55,51,50,53,44,55,52,52,56,46,50,50,52,50,32,76,51,55,53,46,48,57,56,51,50,53,44,55,52,53,52,46,50,52,56,51,50,
32,76,51,55,55,46,53,56,52,51,50,53,44,55,52,52,56,46,50,50,52,50,32,90,32,77,51,55,50,46,55,52,49,51,50,53,44,55,52,53,52,46,50,54,50,55,52,32,76,51,54,55,46,49,48,48,51,50,53,44,55,52,52,56,46,50,50,
52,50,32,76,51,55,48,46,50,52,57,51,50,53,44,55,52,52,56,46,50,50,52,50,32,76,51,55,50,46,55,52,49,51,50,53,44,55,52,53,52,46,50,54,50,55,52,32,90,32,77,51,55,49,46,53,54,56,51,50,53,44,55,52,52,54,46,
49,54,51,57,55,32,76,51,55,48,46,50,57,51,51,50,53,44,55,52,52,51,46,48,55,51,54,50,32,76,51,55,55,46,53,52,48,51,50,53,44,55,52,52,51,46,48,55,51,54,50,32,76,51,55,54,46,50,54,53,51,50,53,44,55,52,52,
54,46,49,54,51,57,55,32,76,51,55,49,46,53,54,56,51,50,53,44,55,52,52,54,46,49,54,51,57,55,32,90,32,77,51,55,53,46,52,49,53,51,50,53,44,55,52,52,56,46,50,50,52,50,32,76,51,55,51,46,57,49,55,51,50,53,44,
55,52,53,49,46,56,53,53,51,54,32,76,51,55,50,46,52,49,56,51,50,53,44,55,52,52,56,46,50,50,52,50,32,76,51,55,53,46,52,49,53,51,50,53,44,55,52,52,56,46,50,50,52,50,32,90,32,77,51,56,49,46,50,54,48,51,50,
53,44,55,52,52,54,46,49,54,51,57,55,32,76,51,55,56,46,52,51,52,51,50,53,44,55,52,52,54,46,49,54,51,57,55,32,76,51,55,57,46,54,49,49,51,50,53,44,55,52,52,51,46,51,49,48,53,53,32,76,51,56,49,46,50,54,48,
51,50,53,44,55,52,52,54,46,49,54,51,57,55,32,90,32,77,51,56,51,46,55,56,55,51,50,53,44,55,52,52,54,46,52,51,53,57,50,32,76,51,56,48,46,57,51,51,51,50,53,44,55,52,52,49,46,50,54,55,56,51,32,67,51,56,48,
46,56,49,55,51,50,53,44,55,52,52,49,46,48,56,54,53,51,32,51,56,48,46,54,54,49,51,50,53,44,55,52,52,49,32,51,56,48,46,52,56,50,51,50,53,44,55,52,52,49,32,67,51,56,48,46,50,57,51,51,50,53,44,55,52,52,49,
32,51,56,48,46,48,55,55,51,50,53,44,55,52,52,49,46,48,49,51,51,57,32,51,55,57,46,56,53,51,51,50,53,44,55,52,52,49,46,48,49,51,51,57,32,76,51,54,56,46,49,56,56,51,50,53,44,55,52,52,49,46,48,49,51,51,57,
32,67,51,54,55,46,57,54,55,51,50,53,44,55,52,52,49,46,48,49,51,51,57,32,51,54,55,46,55,53,52,51,50,53,44,55,52,52,49,46,48,48,53,49,53,32,51,54,55,46,53,54,55,51,50,53,44,55,52,52,49,46,48,48,53,49,53,
32,67,51,54,55,46,51,55,55,51,50,53,44,55,52,52,49,46,48,48,53,49,53,32,51,54,55,46,50,49,51,51,50,53,44,55,52,52,49,46,48,55,51,49,52,32,51,54,55,46,48,57,52,51,50,53,44,55,52,52,49,46,50,54,55,56,51,
32,76,51,54,52,46,49,57,56,51,50,53,44,55,52,52,54,46,55,53,50,49,55,32,67,51,54,51,46,56,57,50,51,50,53,44,55,52,52,55,46,50,53,51,56,51,32,51,54,51,46,57,52,52,51,50,53,44,55,52,52,55,46,56,51,51,55,
57,32,51,54,52,46,51,50,55,51,50,53,44,55,52,52,56,46,50,55,53,55,49,32,76,51,55,51,46,48,50,49,51,50,53,44,55,52,53,55,46,53,53,52,57,57,32,67,51,55,51,46,50,55,55,51,50,53,44,55,52,53,55,46,56,53,49,
54,54,32,51,55,51,46,54,51,48,51,50,53,44,55,52,53,56,32,51,55,51,46,57,56,51,51,50,53,44,55,52,53,56,32,67,51,55,52,46,51,52,50,51,50,53,44,55,52,53,56,32,51,55,52,46,55,48,49,51,50,53,44,55,52,53,55,
46,56,52,56,53,55,32,51,55,52,46,57,53,57,51,50,53,44,55,52,53,55,46,53,52,51,54,54,32,76,51,56,51,46,54,56,49,51,50,53,44,55,52,52,56,46,48,49,53,48,57,32,67,51,56,52,46,48,54,51,51,50,53,44,55,52,52,
55,46,53,54,51,57,32,51,56,52,46,49,48,54,51,50,53,44,55,52,52,54,46,57,51,52,53,32,51,56,51,46,55,56,55,51,50,53,44,55,52,52,54,46,52,51,53,57,50,32,76,51,56,51,46,55,56,55,51,50,53,44,55,52,52,54,46,
52,51,53,57,50,32,90,34,32,105,100,61,34,115,107,101,116,99,104,45,91,35,49,50,53,93,34,62,60,47,112,97,116,104,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,32,32,32,32,60,47,
103,62,10,32,32,32,32,60,47,103,62,10,60,47,115,118,103,62,0,0};

const char* VASTWaveTableEditorViewHeader::apply_svg = (const char*) resource_VASTWaveTableEditorViewHeader_apply_svg;
const int VASTWaveTableEditorViewHeader::apply_svgSize = 1969;

// JUCER_RESOURCE: arc_connect_svg, 3051, "../../Resources/Icons/arc_connect.svg"
static const unsigned char resource_VASTWaveTableEditorViewHeader_arc_connect_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,
70,45,56,34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,10,32,32,32,120,109,108,110,115,58,100,99,61,34,104,116,116,112,58,47,47,112,117,114,108,46,111,114,103,47,
100,99,47,101,108,101,109,101,110,116,115,47,49,46,49,47,34,10,32,32,32,120,109,108,110,115,58,99,99,61,34,104,116,116,112,58,47,47,99,114,101,97,116,105,118,101,99,111,109,109,111,110,115,46,111,114,
103,47,110,115,35,34,10,32,32,32,120,109,108,110,115,58,114,100,102,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,48,50,47,50,50,45,114,100,102,45,115,121,110,116,
97,120,45,110,115,35,34,10,32,32,32,120,109,108,110,115,58,115,118,103,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,118,103,34,10,32,32,32,120,109,108,110,
115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,118,103,34,10,32,32,32,120,109,108,110,115,58,115,111,100,105,112,111,100,105,61,34,104,116,116,112,58,47,
47,115,111,100,105,112,111,100,105,46,115,111,117,114,99,101,102,111,114,103,101,46,110,101,116,47,68,84,68,47,115,111,100,105,112,111,100,105,45,48,46,100,116,100,34,10,32,32,32,120,109,108,110,115,58,
105,110,107,115,99,97,112,101,61,34,104,116,116,112,58,47,47,119,119,119,46,105,110,107,115,99,97,112,101,46,111,114,103,47,110,97,109,101,115,112,97,99,101,115,47,105,110,107,115,99,97,112,101,34,10,
32,32,32,119,105,100,116,104,61,34,50,48,112,120,34,10,32,32,32,104,101,105,103,104,116,61,34,50,48,112,120,34,10,32,32,32,118,105,101,119,66,111,120,61,34,48,32,48,32,50,48,32,50,48,34,10,32,32,32,118,
101,114,115,105,111,110,61,34,49,46,49,34,10,32,32,32,105,100,61,34,115,118,103,49,50,34,10,32,32,32,115,111,100,105,112,111,100,105,58,100,111,99,110,97,109,101,61,34,97,114,99,95,99,111,110,110,101,
99,116,46,115,118,103,34,10,32,32,32,105,110,107,115,99,97,112,101,58,118,101,114,115,105,111,110,61,34,48,46,57,50,46,51,32,40,50,52,48,53,53,52,54,44,32,50,48,49,56,45,48,51,45,49,49,41,34,62,10,32,
32,60,109,101,116,97,100,97,116,97,10,32,32,32,32,32,105,100,61,34,109,101,116,97,100,97,116,97,49,54,34,62,10,32,32,32,32,60,114,100,102,58,82,68,70,62,10,32,32,32,32,32,32,60,99,99,58,87,111,114,107,
10,32,32,32,32,32,32,32,32,32,114,100,102,58,97,98,111,117,116,61,34,34,62,10,32,32,32,32,32,32,32,32,60,100,99,58,102,111,114,109,97,116,62,105,109,97,103,101,47,115,118,103,43,120,109,108,60,47,100,
99,58,102,111,114,109,97,116,62,10,32,32,32,32,32,32,32,32,60,100,99,58,116,121,112,101,10,32,32,32,32,32,32,32,32,32,32,32,114,100,102,58,114,101,115,111,117,114,99,101,61,34,104,116,116,112,58,47,47,
112,117,114,108,46,111,114,103,47,100,99,47,100,99,109,105,116,121,112,101,47,83,116,105,108,108,73,109,97,103,101,34,32,47,62,10,32,32,32,32,32,32,32,32,60,100,99,58,116,105,116,108,101,62,115,116,97,
116,115,32,91,35,49,49,57,48,93,60,47,100,99,58,116,105,116,108,101,62,10,32,32,32,32,32,32,60,47,99,99,58,87,111,114,107,62,10,32,32,32,32,60,47,114,100,102,58,82,68,70,62,10,32,32,60,47,109,101,116,
97,100,97,116,97,62,10,32,32,60,115,111,100,105,112,111,100,105,58,110,97,109,101,100,118,105,101,119,10,32,32,32,32,32,112,97,103,101,99,111,108,111,114,61,34,35,57,53,57,53,57,53,34,10,32,32,32,32,32,
98,111,114,100,101,114,99,111,108,111,114,61,34,35,54,54,54,54,54,54,34,10,32,32,32,32,32,98,111,114,100,101,114,111,112,97,99,105,116,121,61,34,49,34,10,32,32,32,32,32,111,98,106,101,99,116,116,111,108,
101,114,97,110,99,101,61,34,49,48,34,10,32,32,32,32,32,103,114,105,100,116,111,108,101,114,97,110,99,101,61,34,49,48,34,10,32,32,32,32,32,103,117,105,100,101,116,111,108,101,114,97,110,99,101,61,34,49,
48,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,112,97,103,101,111,112,97,99,105,116,121,61,34,49,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,112,97,103,101,115,104,97,100,111,119,61,
34,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,119,105,100,116,104,61,34,51,56,52,48,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,
45,104,101,105,103,104,116,61,34,50,48,53,48,34,10,32,32,32,32,32,105,100,61,34,110,97,109,101,100,118,105,101,119,49,52,34,10,32,32,32,32,32,115,104,111,119,103,114,105,100,61,34,102,97,108,115,101,34,
10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,122,111,111,109,61,34,54,54,46,55,53,48,56,56,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,120,61,34,56,46,52,53,50,56,50,52,53,34,10,32,
32,32,32,32,105,110,107,115,99,97,112,101,58,99,121,61,34,55,46,52,48,51,56,54,49,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,120,61,34,45,49,50,34,10,32,32,32,
32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,121,61,34,45,49,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,109,97,120,105,109,105,122,101,100,
61,34,49,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,117,114,114,101,110,116,45,108,97,121,101,114,61,34,115,118,103,49,50,34,32,47,62,10,32,32,60,33,45,45,32,71,101,110,101,114,97,116,111,
114,58,32,83,107,101,116,99,104,32,51,46,56,46,49,32,40,50,57,54,56,55,41,32,45,32,104,116,116,112,58,47,47,119,119,119,46,98,111,104,101,109,105,97,110,99,111,100,105,110,103,46,99,111,109,47,115,107,
101,116,99,104,32,45,45,62,10,32,32,60,116,105,116,108,101,10,32,32,32,32,32,105,100,61,34,116,105,116,108,101,50,34,62,115,116,97,116,115,32,91,35,49,49,57,48,93,60,47,116,105,116,108,101,62,10,32,32,
60,100,101,115,99,10,32,32,32,32,32,105,100,61,34,100,101,115,99,52,34,62,67,114,101,97,116,101,100,32,119,105,116,104,32,83,107,101,116,99,104,46,60,47,100,101,115,99,62,10,32,32,60,100,101,102,115,10,
32,32,32,32,32,105,100,61,34,100,101,102,115,54,34,32,47,62,10,32,32,60,103,10,32,32,32,32,32,105,100,61,34,103,56,50,56,34,10,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,109,97,116,114,105,
120,40,48,46,56,52,50,57,49,51,54,49,44,48,44,48,44,48,46,55,52,54,54,56,49,56,52,44,45,48,46,49,56,55,55,48,48,52,52,44,51,46,51,56,49,55,49,51,52,41,34,10,32,32,32,32,32,115,116,121,108,101,61,34,115,
116,114,111,107,101,45,119,105,100,116,104,58,49,46,50,54,48,52,57,51,55,54,34,62,10,32,32,32,32,60,101,108,108,105,112,115,101,10,32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,56,49,57,34,10,32,32,
32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,102,102,102,102,102,102,59,102,105,108,108,45,111,112,97,99,105,116,121,58,48,46,48,54,53,48,52,48,54,56,59,115,116,114,111,107,101,58,35,
102,102,102,102,102,102,59,115,116,114,111,107,101,45,119,105,100,116,104,58,49,46,50,54,48,52,57,51,55,54,59,115,116,114,111,107,101,45,108,105,110,101,99,97,112,58,114,111,117,110,100,59,115,116,114,
111,107,101,45,108,105,110,101,106,111,105,110,58,114,111,117,110,100,59,115,116,114,111,107,101,45,109,105,116,101,114,108,105,109,105,116,58,52,59,115,116,114,111,107,101,45,100,97,115,104,97,114,114,
97,121,58,110,111,110,101,59,115,116,114,111,107,101,45,100,97,115,104,111,102,102,115,101,116,58,50,46,50,48,48,48,48,48,48,53,59,115,116,114,111,107,101,45,111,112,97,99,105,116,121,58,49,34,10,32,32,
32,32,32,32,32,114,121,61,34,50,46,50,57,57,53,57,53,49,34,10,32,32,32,32,32,32,32,114,120,61,34,50,46,50,54,50,49,52,50,52,34,10,32,32,32,32,32,32,32,99,121,61,34,49,56,46,53,50,52,51,54,52,34,10,32,
32,32,32,32,32,32,99,120,61,34,51,46,57,53,53,48,48,52,34,32,47,62,10,32,32,60,47,103,62,10,32,32,60,103,10,32,32,32,32,32,105,100,61,34,103,56,50,56,45,54,34,10,32,32,32,32,32,116,114,97,110,115,102,
111,114,109,61,34,109,97,116,114,105,120,40,48,46,57,48,55,55,49,55,54,56,44,48,44,48,44,48,46,55,52,54,54,56,49,56,52,44,49,51,46,54,56,51,49,53,51,44,45,49,49,46,52,57,52,52,57,53,41,34,10,32,32,32,
32,32,115,116,121,108,101,61,34,115,116,114,111,107,101,45,119,105,100,116,104,58,49,46,50,49,52,54,54,53,55,55,34,62,10,32,32,32,32,60,101,108,108,105,112,115,101,10,32,32,32,32,32,32,32,105,100,61,34,
112,97,116,104,56,49,57,45,49,34,10,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,102,102,102,102,102,102,59,102,105,108,108,45,111,112,97,99,105,116,121,58,48,46,48,54,53,48,52,
48,54,56,59,115,116,114,111,107,101,58,35,102,102,102,102,102,102,59,115,116,114,111,107,101,45,119,105,100,116,104,58,49,46,50,49,52,54,54,53,55,55,59,115,116,114,111,107,101,45,108,105,110,101,99,97,
112,58,114,111,117,110,100,59,115,116,114,111,107,101,45,108,105,110,101,106,111,105,110,58,114,111,117,110,100,59,115,116,114,111,107,101,45,109,105,116,101,114,108,105,109,105,116,58,52,59,115,116,114,
111,107,101,45,100,97,115,104,97,114,114,97,121,58,110,111,110,101,59,115,116,114,111,107,101,45,100,97,115,104,111,102,102,115,101,116,58,50,46,50,48,48,48,48,48,48,53,59,115,116,114,111,107,101,45,111,
112,97,99,105,116,121,58,49,34,10,32,32,32,32,32,32,32,114,121,61,34,50,46,50,57,57,53,57,53,49,34,10,32,32,32,32,32,32,32,114,120,61,34,50,46,50,54,50,49,52,50,52,34,10,32,32,32,32,32,32,32,99,121,61,
34,49,56,46,53,50,52,51,54,52,34,10,32,32,32,32,32,32,32,99,120,61,34,51,46,57,53,53,48,48,52,34,32,47,62,10,32,32,60,47,103,62,10,32,32,60,112,97,116,104,10,32,32,32,32,32,115,116,121,108,101,61,34,102,
105,108,108,58,110,111,110,101,59,115,116,114,111,107,101,58,35,102,102,102,102,102,102,59,115,116,114,111,107,101,45,119,105,100,116,104,58,49,46,53,59,115,116,114,111,107,101,45,108,105,110,101,99,97,
112,58,98,117,116,116,59,115,116,114,111,107,101,45,108,105,110,101,106,111,105,110,58,109,105,116,101,114,59,115,116,114,111,107,101,45,109,105,116,101,114,108,105,109,105,116,58,52,59,115,116,114,111,
107,101,45,100,97,115,104,97,114,114,97,121,58,110,111,110,101,59,115,116,114,111,107,101,45,111,112,97,99,105,116,121,58,49,34,10,32,32,32,32,32,100,61,34,109,32,51,46,52,54,48,54,50,56,54,44,49,52,46,
50,57,50,50,49,32,99,32,48,44,48,32,45,48,46,50,56,52,54,52,48,53,44,45,53,46,57,49,55,53,50,52,57,32,51,46,56,50,48,49,55,52,51,44,45,56,46,52,52,57,51,50,54,55,32,67,32,49,49,46,50,50,51,54,55,54,44,
51,46,52,49,48,57,54,53,32,49,51,46,56,49,50,53,53,50,44,51,46,48,55,49,51,56,52,50,32,49,51,46,56,49,50,53,53,50,44,51,46,48,55,49,51,56,52,50,34,10,32,32,32,32,32,105,100,61,34,112,97,116,104,52,53,
53,49,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,111,110,110,101,99,116,111,114,45,99,117,114,118,97,116,117,114,101,61,34,48,34,10,32,32,32,32,32,115,111,100,105,112,111,100,105,58,110,
111,100,101,116,121,112,101,115,61,34,99,115,99,34,32,47,62,10,60,47,115,118,103,62,10,0,0};

const char* VASTWaveTableEditorViewHeader::arc_connect_svg = (const char*) resource_VASTWaveTableEditorViewHeader_arc_connect_svg;
const int VASTWaveTableEditorViewHeader::arc_connect_svgSize = 3051;

// JUCER_RESOURCE: close_svg, 1012, "../../Resources/Icons/close.svg"
static const unsigned char resource_VASTWaveTableEditorViewHeader_close_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,70,45,
56,34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,32,119,105,100,116,104,61,34,50,49,112,120,34,32,104,101,105,103,104,116,61,34,50,48,112,120,34,32,118,105,101,119,
66,111,120,61,34,48,32,48,32,50,49,32,50,48,34,32,118,101,114,115,105,111,110,61,34,49,46,49,34,32,120,109,108,110,115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,
47,115,118,103,34,32,120,109,108,110,115,58,120,108,105,110,107,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,120,108,105,110,107,34,62,10,32,32,32,32,60,33,45,
45,32,71,101,110,101,114,97,116,111,114,58,32,83,107,101,116,99,104,32,51,46,56,46,49,32,40,50,57,54,56,55,41,32,45,32,104,116,116,112,58,47,47,119,119,119,46,98,111,104,101,109,105,97,110,99,111,100,
105,110,103,46,99,111,109,47,115,107,101,116,99,104,32,45,45,62,10,32,32,32,32,60,116,105,116,108,101,62,99,108,111,115,101,32,91,35,49,53,51,56,93,60,47,116,105,116,108,101,62,10,32,32,32,32,60,100,101,
115,99,62,67,114,101,97,116,101,100,32,119,105,116,104,32,83,107,101,116,99,104,46,60,47,100,101,115,99,62,10,32,32,32,32,60,100,101,102,115,62,60,47,100,101,102,115,62,10,32,32,32,32,60,103,32,105,100,
61,34,80,97,103,101,45,49,34,32,115,116,114,111,107,101,61,34,110,111,110,101,34,32,115,116,114,111,107,101,45,119,105,100,116,104,61,34,49,34,32,102,105,108,108,61,34,110,111,110,101,34,32,102,105,108,
108,45,114,117,108,101,61,34,101,118,101,110,111,100,100,34,62,10,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,68,114,105,98,98,98,108,101,45,68,97,114,107,45,80,114,101,118,105,101,119,34,32,116,114,
97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,45,49,51,57,46,48,48,48,48,48,48,44,32,45,49,54,48,46,48,48,48,48,48,48,41,34,32,102,105,108,108,61,34,35,70,70,70,70,70,70,34,62,
10,32,32,32,32,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,105,99,111,110,115,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,53,54,46,48,48,48,48,48,48,44,32,49,
54,48,46,48,48,48,48,48,48,41,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,60,112,97,116,104,32,100,61,34,77,56,53,46,49,44,49,56,32,76,49,48,49,46,57,44,49,56,32,76,49,48,49,46,57,44,50,32,
76,56,53,46,49,44,50,32,76,56,53,46,49,44,49,56,32,90,32,77,56,51,44,50,48,32,76,49,48,52,44,50,48,32,76,49,48,52,44,48,32,76,56,51,44,48,32,76,56,51,44,50,48,32,90,32,77,56,57,46,55,56,55,50,44,49,49,
46,56,53,54,32,76,57,50,46,48,49,52,50,53,44,57,46,55,51,53,32,76,56,57,46,55,56,55,50,44,55,46,54,49,51,32,76,57,49,46,50,55,49,57,44,54,46,49,57,57,32,76,57,51,46,53,44,56,46,51,50,49,32,76,57,53,46,
55,50,55,48,53,44,54,46,49,57,57,32,76,57,55,46,50,49,49,55,53,44,55,46,54,49,51,32,76,57,52,46,57,56,52,55,44,57,46,55,51,53,32,76,57,55,46,50,49,49,55,53,44,49,49,46,56,53,54,32,76,57,53,46,55,50,55,
48,53,44,49,51,46,50,55,32,76,57,51,46,53,44,49,49,46,49,52,57,32,76,57,49,46,50,55,49,57,44,49,51,46,50,55,32,76,56,57,46,55,56,55,50,44,49,49,46,56,53,54,32,90,34,32,105,100,61,34,99,108,111,115,101,
45,91,35,49,53,51,56,93,34,62,60,47,112,97,116,104,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,60,47,103,62,10,60,47,115,118,103,62,0,0};

const char* VASTWaveTableEditorViewHeader::close_svg = (const char*) resource_VASTWaveTableEditorViewHeader_close_svg;
const int VASTWaveTableEditorViewHeader::close_svgSize = 1012;

// JUCER_RESOURCE: copy_svg, 1135, "../../Resources/Icons/copy.svg"
static const unsigned char resource_VASTWaveTableEditorViewHeader_copy_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,70,45,
56,34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,32,119,105,100,116,104,61,34,50,48,112,120,34,32,104,101,105,103,104,116,61,34,50,48,112,120,34,32,118,105,101,119,
66,111,120,61,34,48,32,48,32,50,48,32,50,48,34,32,118,101,114,115,105,111,110,61,34,49,46,49,34,32,120,109,108,110,115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,
47,115,118,103,34,32,120,109,108,110,115,58,120,108,105,110,107,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,120,108,105,110,107,34,62,10,32,32,32,32,60,33,45,
45,32,71,101,110,101,114,97,116,111,114,58,32,83,107,101,116,99,104,32,51,46,56,46,49,32,40,50,57,54,56,55,41,32,45,32,104,116,116,112,58,47,47,119,119,119,46,98,111,104,101,109,105,97,110,99,111,100,
105,110,103,46,99,111,109,47,115,107,101,116,99,104,32,45,45,62,10,32,32,32,32,60,116,105,116,108,101,62,111,98,106,101,99,116,95,112,108,97,99,101,109,101,110,116,32,91,35,54,54,93,60,47,116,105,116,
108,101,62,10,32,32,32,32,60,100,101,115,99,62,67,114,101,97,116,101,100,32,119,105,116,104,32,83,107,101,116,99,104,46,60,47,100,101,115,99,62,10,32,32,32,32,60,100,101,102,115,62,60,47,100,101,102,115,
62,10,32,32,32,32,60,103,32,105,100,61,34,80,97,103,101,45,49,34,32,115,116,114,111,107,101,61,34,110,111,110,101,34,32,115,116,114,111,107,101,45,119,105,100,116,104,61,34,49,34,32,102,105,108,108,61,
34,110,111,110,101,34,32,102,105,108,108,45,114,117,108,101,61,34,101,118,101,110,111,100,100,34,62,10,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,68,114,105,98,98,98,108,101,45,68,97,114,107,45,80,
114,101,118,105,101,119,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,45,49,52,48,46,48,48,48,48,48,48,44,32,45,55,57,49,57,46,48,48,48,48,48,48,41,34,32,102,105,
108,108,61,34,35,70,70,70,70,70,70,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,105,99,111,110,115,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,
101,40,53,54,46,48,48,48,48,48,48,44,32,49,54,48,46,48,48,48,48,48,48,41,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,60,112,97,116,104,32,100,61,34,77,57,51,44,55,55,55,57,32,76,57,53,44,
55,55,55,57,32,76,57,53,44,55,55,53,57,32,76,57,51,44,55,55,53,57,32,76,57,51,44,55,55,55,57,32,90,32,77,56,54,44,55,55,55,53,32,76,56,57,44,55,55,55,53,32,76,56,57,44,55,55,54,51,32,76,56,54,44,55,55,
54,51,32,76,56,54,44,55,55,55,53,32,90,32,77,56,54,44,55,55,54,49,32,76,56,52,44,55,55,54,49,32,76,56,52,44,55,55,54,51,32,76,56,52,44,55,55,55,53,32,76,56,52,44,55,55,55,55,32,76,56,54,44,55,55,55,55,
32,76,56,57,44,55,55,55,55,32,76,57,49,44,55,55,55,55,32,76,57,49,44,55,55,55,53,32,76,57,49,44,55,55,54,51,32,76,57,49,44,55,55,54,49,32,76,56,57,44,55,55,54,49,32,76,56,54,44,55,55,54,49,32,90,32,77,
57,57,44,55,55,55,53,32,76,49,48,50,44,55,55,55,53,32,76,49,48,50,44,55,55,54,51,32,76,57,57,44,55,55,54,51,32,76,57,57,44,55,55,55,53,32,90,32,77,49,48,50,44,55,55,54,49,32,76,57,57,44,55,55,54,49,32,
76,57,55,44,55,55,54,49,32,76,57,55,44,55,55,54,51,32,76,57,55,44,55,55,55,53,32,76,57,55,44,55,55,55,55,32,76,57,57,44,55,55,55,55,32,76,49,48,50,44,55,55,55,55,32,76,49,48,52,44,55,55,55,55,32,76,49,
48,52,44,55,55,55,53,32,76,49,48,52,44,55,55,54,51,32,76,49,48,52,44,55,55,54,49,32,76,49,48,50,44,55,55,54,49,32,90,34,32,105,100,61,34,111,98,106,101,99,116,95,112,108,97,99,101,109,101,110,116,45,91,
35,54,54,93,34,62,60,47,112,97,116,104,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,60,47,103,62,10,60,47,115,118,103,62,0,0};

const char* VASTWaveTableEditorViewHeader::copy_svg = (const char*) resource_VASTWaveTableEditorViewHeader_copy_svg;
const int VASTWaveTableEditorViewHeader::copy_svgSize = 1135;

// JUCER_RESOURCE: cut_svg, 1095, "../../Resources/Icons/cut.svg"
static const unsigned char resource_VASTWaveTableEditorViewHeader_cut_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,70,45,56,
34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,32,119,105,100,116,104,61,34,50,48,112,120,34,32,104,101,105,103,104,116,61,34,50,48,112,120,34,32,118,105,101,119,
66,111,120,61,34,48,32,48,32,50,48,32,50,48,34,32,118,101,114,115,105,111,110,61,34,49,46,49,34,32,120,109,108,110,115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,
47,115,118,103,34,32,120,109,108,110,115,58,120,108,105,110,107,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,120,108,105,110,107,34,62,10,32,32,32,32,60,33,45,
45,32,71,101,110,101,114,97,116,111,114,58,32,83,107,101,116,99,104,32,51,46,56,46,49,32,40,50,57,54,56,55,41,32,45,32,104,116,116,112,58,47,47,119,119,119,46,98,111,104,101,109,105,97,110,99,111,100,
105,110,103,46,99,111,109,47,115,107,101,116,99,104,32,45,45,62,10,32,32,32,32,60,116,105,116,108,101,62,99,117,116,35,52,32,91,35,56,50,49,93,60,47,116,105,116,108,101,62,10,32,32,32,32,60,100,101,115,
99,62,67,114,101,97,116,101,100,32,119,105,116,104,32,83,107,101,116,99,104,46,60,47,100,101,115,99,62,10,32,32,32,32,60,100,101,102,115,62,60,47,100,101,102,115,62,10,32,32,32,32,60,103,32,105,100,61,
34,80,97,103,101,45,49,34,32,115,116,114,111,107,101,61,34,110,111,110,101,34,32,115,116,114,111,107,101,45,119,105,100,116,104,61,34,49,34,32,102,105,108,108,61,34,110,111,110,101,34,32,102,105,108,108,
45,114,117,108,101,61,34,101,118,101,110,111,100,100,34,62,10,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,68,114,105,98,98,98,108,101,45,68,97,114,107,45,80,114,101,118,105,101,119,34,32,116,114,97,
110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,45,52,50,48,46,48,48,48,48,48,48,44,32,45,52,53,49,57,46,48,48,48,48,48,48,41,34,32,102,105,108,108,61,34,35,70,70,70,70,70,70,34,62,
10,32,32,32,32,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,105,99,111,110,115,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,53,54,46,48,48,48,48,48,48,44,32,49,
54,48,46,48,48,48,48,48,48,41,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,60,112,97,116,104,32,100,61,34,77,51,55,56,44,52,51,55,57,32,76,51,56,48,44,52,51,55,57,32,76,51,56,48,44,52,51,55,
54,32,76,51,55,56,44,52,51,55,54,32,76,51,55,56,44,52,51,55,57,32,90,32,77,51,56,48,44,52,51,55,51,32,76,51,55,56,44,52,51,55,51,32,76,51,55,48,44,52,51,55,51,32,76,51,55,48,44,52,51,54,54,32,76,51,54,
56,44,52,51,54,54,32,76,51,54,56,44,52,51,55,51,32,76,51,54,56,44,52,51,55,53,32,76,51,55,48,44,52,51,55,53,32,76,51,55,56,44,52,51,55,53,32,76,51,56,48,44,52,51,55,53,32,76,51,56,52,44,52,51,55,53,32,
76,51,56,52,44,52,51,55,51,32,76,51,56,48,44,52,51,55,51,32,90,32,77,51,55,48,44,52,51,54,53,32,76,51,55,56,44,52,51,54,53,32,76,51,55,56,44,52,51,55,50,32,76,51,56,48,44,52,51,55,50,32,76,51,56,48,44,
52,51,54,53,32,76,51,56,48,44,52,51,54,51,32,76,51,55,56,44,52,51,54,51,32,76,51,55,48,44,52,51,54,51,32,76,51,54,56,44,52,51,54,51,32,76,51,54,52,44,52,51,54,51,32,76,51,54,52,44,52,51,54,53,32,76,51,
54,56,44,52,51,54,53,32,76,51,55,48,44,52,51,54,53,32,90,32,77,51,54,56,44,52,51,54,50,32,76,51,55,48,44,52,51,54,50,32,76,51,55,48,44,52,51,53,57,32,76,51,54,56,44,52,51,53,57,32,76,51,54,56,44,52,51,
54,50,32,90,34,32,105,100,61,34,99,117,116,35,52,45,91,35,56,50,49,93,34,62,60,47,112,97,116,104,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,
32,32,60,47,103,62,10,60,47,115,118,103,62,0,0};

const char* VASTWaveTableEditorViewHeader::cut_svg = (const char*) resource_VASTWaveTableEditorViewHeader_cut_svg;
const int VASTWaveTableEditorViewHeader::cut_svgSize = 1095;

// JUCER_RESOURCE: delete_svg, 1124, "../../Resources/Icons/delete.svg"
static const unsigned char resource_VASTWaveTableEditorViewHeader_delete_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,70,45,
56,34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,32,119,105,100,116,104,61,34,50,49,112,120,34,32,104,101,105,103,104,116,61,34,50,48,112,120,34,32,118,105,101,119,
66,111,120,61,34,48,32,48,32,50,49,32,50,48,34,32,118,101,114,115,105,111,110,61,34,49,46,49,34,32,120,109,108,110,115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,
47,115,118,103,34,32,120,109,108,110,115,58,120,108,105,110,107,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,120,108,105,110,107,34,62,10,32,32,32,32,60,33,45,
45,32,71,101,110,101,114,97,116,111,114,58,32,83,107,101,116,99,104,32,51,46,56,46,49,32,40,50,57,54,56,55,41,32,45,32,104,116,116,112,58,47,47,119,119,119,46,98,111,104,101,109,105,97,110,99,111,100,
105,110,103,46,99,111,109,47,115,107,101,116,99,104,32,45,45,62,10,32,32,32,32,60,116,105,116,108,101,62,100,101,108,101,116,101,32,91,35,49,52,56,55,93,60,47,116,105,116,108,101,62,10,32,32,32,32,60,
100,101,115,99,62,67,114,101,97,116,101,100,32,119,105,116,104,32,83,107,101,116,99,104,46,60,47,100,101,115,99,62,10,32,32,32,32,60,100,101,102,115,62,60,47,100,101,102,115,62,10,32,32,32,32,60,103,32,
105,100,61,34,80,97,103,101,45,49,34,32,115,116,114,111,107,101,61,34,110,111,110,101,34,32,115,116,114,111,107,101,45,119,105,100,116,104,61,34,49,34,32,102,105,108,108,61,34,110,111,110,101,34,32,102,
105,108,108,45,114,117,108,101,61,34,101,118,101,110,111,100,100,34,62,10,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,68,114,105,98,98,98,108,101,45,68,97,114,107,45,80,114,101,118,105,101,119,34,
32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,45,49,55,57,46,48,48,48,48,48,48,44,32,45,51,54,48,46,48,48,48,48,48,48,41,34,32,102,105,108,108,61,34,35,70,70,70,70,70,
70,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,105,99,111,110,115,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,53,54,46,48,48,48,48,48,48,
44,32,49,54,48,46,48,48,48,48,48,48,41,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,60,112,97,116,104,32,100,61,34,77,49,51,48,46,51,53,44,50,49,54,32,76,49,51,50,46,52,53,44,50,49,54,32,76,
49,51,50,46,52,53,44,50,48,56,32,76,49,51,48,46,51,53,44,50,48,56,32,76,49,51,48,46,51,53,44,50,49,54,32,90,32,77,49,51,52,46,53,53,44,50,49,54,32,76,49,51,54,46,54,53,44,50,49,54,32,76,49,51,54,46,54,
53,44,50,48,56,32,76,49,51,52,46,53,53,44,50,48,56,32,76,49,51,52,46,53,53,44,50,49,54,32,90,32,77,49,50,56,46,50,53,44,50,49,56,32,76,49,51,56,46,55,53,44,50,49,56,32,76,49,51,56,46,55,53,44,50,48,54,
32,76,49,50,56,46,50,53,44,50,48,54,32,76,49,50,56,46,50,53,44,50,49,56,32,90,32,77,49,51,48,46,51,53,44,50,48,52,32,76,49,51,54,46,54,53,44,50,48,52,32,76,49,51,54,46,54,53,44,50,48,50,32,76,49,51,48,
46,51,53,44,50,48,50,32,76,49,51,48,46,51,53,44,50,48,52,32,90,32,77,49,51,56,46,55,53,44,50,48,52,32,76,49,51,56,46,55,53,44,50,48,48,32,76,49,50,56,46,50,53,44,50,48,48,32,76,49,50,56,46,50,53,44,50,
48,52,32,76,49,50,51,44,50,48,52,32,76,49,50,51,44,50,48,54,32,76,49,50,54,46,49,53,44,50,48,54,32,76,49,50,54,46,49,53,44,50,50,48,32,76,49,52,48,46,56,53,44,50,50,48,32,76,49,52,48,46,56,53,44,50,48,
54,32,76,49,52,52,44,50,48,54,32,76,49,52,52,44,50,48,52,32,76,49,51,56,46,55,53,44,50,48,52,32,90,34,32,105,100,61,34,100,101,108,101,116,101,45,91,35,49,52,56,55,93,34,62,60,47,112,97,116,104,62,10,
32,32,32,32,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,60,47,103,62,10,60,47,115,118,103,62,0,0};

const char* VASTWaveTableEditorViewHeader::delete_svg = (const char*) resource_VASTWaveTableEditorViewHeader_delete_svg;
const int VASTWaveTableEditorViewHeader::delete_svgSize = 1124;

// JUCER_RESOURCE: duplicate_svg, 1641, "../../Resources/Icons/duplicate.svg"
static const unsigned char resource_VASTWaveTableEditorViewHeader_duplicate_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,70,
45,56,34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,32,119,105,100,116,104,61,34,50,48,112,120,34,32,104,101,105,103,104,116,61,34,50,48,112,120,34,32,118,105,101,
119,66,111,120,61,34,48,32,48,32,50,48,32,50,48,34,32,118,101,114,115,105,111,110,61,34,49,46,49,34,32,120,109,108,110,115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,
48,48,47,115,118,103,34,32,120,109,108,110,115,58,120,108,105,110,107,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,120,108,105,110,107,34,62,10,32,32,32,32,60,
33,45,45,32,71,101,110,101,114,97,116,111,114,58,32,83,107,101,116,99,104,32,51,46,56,46,49,32,40,50,57,54,56,55,41,32,45,32,104,116,116,112,58,47,47,119,119,119,46,98,111,104,101,109,105,97,110,99,111,
100,105,110,103,46,99,111,109,47,115,107,101,116,99,104,32,45,45,62,10,32,32,32,32,60,116,105,116,108,101,62,100,117,112,108,105,99,97,116,101,95,114,111,117,110,100,32,91,35,56,51,53,93,60,47,116,105,
116,108,101,62,10,32,32,32,32,60,100,101,115,99,62,67,114,101,97,116,101,100,32,119,105,116,104,32,83,107,101,116,99,104,46,60,47,100,101,115,99,62,10,32,32,32,32,60,100,101,102,115,62,60,47,100,101,102,
115,62,10,32,32,32,32,60,103,32,105,100,61,34,80,97,103,101,45,49,34,32,115,116,114,111,107,101,61,34,110,111,110,101,34,32,115,116,114,111,107,101,45,119,105,100,116,104,61,34,49,34,32,102,105,108,108,
61,34,110,111,110,101,34,32,102,105,108,108,45,114,117,108,101,61,34,101,118,101,110,111,100,100,34,62,10,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,68,114,105,98,98,98,108,101,45,68,97,114,107,45,
80,114,101,118,105,101,119,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,45,50,54,48,46,48,48,48,48,48,48,44,32,45,52,52,55,57,46,48,48,48,48,48,48,41,34,32,102,105,
108,108,61,34,35,70,70,70,70,70,70,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,105,99,111,110,115,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,
101,40,53,54,46,48,48,48,48,48,48,44,32,49,54,48,46,48,48,48,48,48,48,41,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,60,112,97,116,104,32,100,61,34,77,50,50,49,44,52,51,51,51,32,76,50,50,
48,44,52,51,51,51,32,76,50,50,48,44,52,51,50,53,32,67,50,50,48,44,52,51,50,51,46,56,57,53,32,50,49,57,46,49,48,53,44,52,51,50,51,32,50,49,56,44,52,51,50,51,32,76,50,49,48,44,52,51,50,51,32,76,50,49,48,
44,52,51,50,50,32,67,50,49,48,44,52,51,50,49,46,52,52,56,32,50,49,48,46,52,52,56,44,52,51,50,49,32,50,49,49,44,52,51,50,49,32,76,50,50,49,44,52,51,50,49,32,67,50,50,49,46,53,53,50,44,52,51,50,49,32,50,
50,50,44,52,51,50,49,46,52,52,56,32,50,50,50,44,52,51,50,50,32,76,50,50,50,44,52,51,51,50,32,67,50,50,50,44,52,51,51,50,46,53,53,50,32,50,50,49,46,53,53,50,44,52,51,51,51,32,50,50,49,44,52,51,51,51,32,
76,50,50,49,44,52,51,51,51,32,90,32,77,50,49,56,44,52,51,51,51,32,76,50,49,49,44,52,51,51,51,32,67,50,49,48,46,52,52,56,44,52,51,51,51,32,50,49,48,44,52,51,51,50,46,53,53,50,32,50,49,48,44,52,51,51,50,
32,76,50,49,48,44,52,51,50,53,32,76,50,49,55,44,52,51,50,53,32,67,50,49,55,46,53,53,50,44,52,51,50,53,32,50,49,56,44,52,51,50,53,46,52,52,56,32,50,49,56,44,52,51,50,54,32,76,50,49,56,44,52,51,51,51,32,
90,32,77,50,49,55,44,52,51,51,55,32,76,50,48,55,44,52,51,51,55,32,67,50,48,54,46,52,52,56,44,52,51,51,55,32,50,48,54,44,52,51,51,54,46,53,53,50,32,50,48,54,44,52,51,51,54,32,76,50,48,54,44,52,51,50,54,
32,67,50,48,54,44,52,51,50,53,46,52,52,56,32,50,48,54,46,52,52,56,44,52,51,50,53,32,50,48,55,44,52,51,50,53,32,76,50,48,56,44,52,51,50,53,32,76,50,48,56,44,52,51,51,51,32,67,50,48,56,44,52,51,51,52,46,
49,48,53,32,50,48,56,46,56,57,53,44,52,51,51,53,32,50,49,48,44,52,51,51,53,32,76,50,49,56,44,52,51,51,53,32,76,50,49,56,44,52,51,51,54,32,67,50,49,56,44,52,51,51,54,46,53,53,50,32,50,49,55,46,53,53,50,
44,52,51,51,55,32,50,49,55,44,52,51,51,55,32,76,50,49,55,44,52,51,51,55,32,90,32,77,50,50,50,44,52,51,49,57,32,76,50,49,48,44,52,51,49,57,32,67,50,48,56,46,56,57,53,44,52,51,49,57,32,50,48,56,44,52,51,
49,57,46,56,57,53,32,50,48,56,44,52,51,50,49,32,76,50,48,56,44,52,51,50,51,32,76,50,48,54,44,52,51,50,51,32,67,50,48,52,46,56,57,53,44,52,51,50,51,32,50,48,52,44,52,51,50,51,46,56,57,53,32,50,48,52,44,
52,51,50,53,32,76,50,48,52,44,52,51,51,55,32,67,50,48,52,44,52,51,51,56,46,49,48,53,32,50,48,52,46,56,57,53,44,52,51,51,57,32,50,48,54,44,52,51,51,57,32,76,50,49,56,44,52,51,51,57,32,67,50,49,57,46,49,
48,53,44,52,51,51,57,32,50,50,48,44,52,51,51,56,46,49,48,53,32,50,50,48,44,52,51,51,55,32,76,50,50,48,44,52,51,51,53,32,76,50,50,50,44,52,51,51,53,32,67,50,50,51,46,49,48,53,44,52,51,51,53,32,50,50,52,
44,52,51,51,52,46,49,48,53,32,50,50,52,44,52,51,51,51,32,76,50,50,52,44,52,51,50,49,32,67,50,50,52,44,52,51,49,57,46,56,57,53,32,50,50,51,46,49,48,53,44,52,51,49,57,32,50,50,50,44,52,51,49,57,32,76,50,
50,50,44,52,51,49,57,32,90,34,32,105,100,61,34,100,117,112,108,105,99,97,116,101,95,114,111,117,110,100,45,91,35,56,51,53,93,34,62,60,47,112,97,116,104,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,47,
103,62,10,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,60,47,103,62,10,60,47,115,118,103,62,0,0};

const char* VASTWaveTableEditorViewHeader::duplicate_svg = (const char*) resource_VASTWaveTableEditorViewHeader_duplicate_svg;
const int VASTWaveTableEditorViewHeader::duplicate_svgSize = 1641;

// JUCER_RESOURCE: generate_svg, 1044, "../../Resources/Icons/generate.svg"
static const unsigned char resource_VASTWaveTableEditorViewHeader_generate_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,70,
45,56,34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,32,119,105,100,116,104,61,34,50,48,112,120,34,32,104,101,105,103,104,116,61,34,50,48,112,120,34,32,118,105,101,
119,66,111,120,61,34,48,32,48,32,50,48,32,50,48,34,32,118,101,114,115,105,111,110,61,34,49,46,49,34,32,120,109,108,110,115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,
48,48,47,115,118,103,34,32,120,109,108,110,115,58,120,108,105,110,107,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,120,108,105,110,107,34,62,10,32,32,32,32,60,
33,45,45,32,71,101,110,101,114,97,116,111,114,58,32,83,107,101,116,99,104,32,51,46,56,46,49,32,40,50,57,54,56,55,41,32,45,32,104,116,116,112,58,47,47,119,119,119,46,98,111,104,101,109,105,97,110,99,111,
100,105,110,103,46,99,111,109,47,115,107,101,116,99,104,32,45,45,62,10,32,32,32,32,60,116,105,116,108,101,62,119,97,110,100,32,91,35,55,50,49,93,60,47,116,105,116,108,101,62,10,32,32,32,32,60,100,101,
115,99,62,67,114,101,97,116,101,100,32,119,105,116,104,32,83,107,101,116,99,104,46,60,47,100,101,115,99,62,10,32,32,32,32,60,100,101,102,115,62,60,47,100,101,102,115,62,10,32,32,32,32,60,103,32,105,100,
61,34,80,97,103,101,45,49,34,32,115,116,114,111,107,101,61,34,110,111,110,101,34,32,115,116,114,111,107,101,45,119,105,100,116,104,61,34,49,34,32,102,105,108,108,61,34,110,111,110,101,34,32,102,105,108,
108,45,114,117,108,101,61,34,101,118,101,110,111,100,100,34,62,10,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,68,114,105,98,98,98,108,101,45,68,97,114,107,45,80,114,101,118,105,101,119,34,32,116,114,
97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,45,51,52,48,46,48,48,48,48,48,48,44,32,45,52,57,57,57,46,48,48,48,48,48,48,41,34,32,102,105,108,108,61,34,35,70,70,70,70,70,70,34,
62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,105,99,111,110,115,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,53,54,46,48,48,48,48,48,48,44,32,
49,54,48,46,48,48,48,48,48,48,41,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,60,112,111,108,121,103,111,110,32,105,100,61,34,119,97,110,100,45,91,35,55,50,49,93,34,32,112,111,105,110,116,
115,61,34,51,48,52,32,52,56,52,51,32,51,48,49,46,52,54,55,32,52,56,52,51,32,51,48,51,46,50,57,54,32,52,56,52,49,46,49,55,50,32,51,48,49,46,56,53,53,32,52,56,51,57,46,55,53,55,32,51,48,48,32,52,56,52,49,
46,53,56,54,32,51,48,48,32,52,56,51,57,32,50,57,56,32,52,56,51,57,32,50,57,56,32,52,56,52,49,46,53,56,54,32,50,57,54,46,49,57,56,32,52,56,51,57,46,55,53,55,32,50,57,52,46,55,57,55,32,52,56,52,49,46,49,
55,50,32,50,57,54,46,54,51,57,32,52,56,52,51,32,50,57,52,32,52,56,52,51,32,50,57,52,32,52,56,52,53,32,50,57,54,46,54,51,57,32,52,56,52,53,32,50,56,52,32,52,56,53,55,46,54,51,57,32,50,56,53,46,51,56,56,
32,52,56,53,57,46,48,53,51,32,50,57,56,32,52,56,52,54,46,52,49,52,32,50,57,56,32,52,56,52,57,32,51,48,48,32,52,56,52,57,32,51,48,48,32,52,56,52,54,46,52,49,52,32,51,48,49,46,56,53,53,32,52,56,52,56,46,
50,52,51,32,51,48,51,46,50,56,50,32,52,56,52,54,46,56,50,56,32,51,48,49,46,52,54,55,32,52,56,52,53,32,51,48,52,32,52,56,52,53,34,62,60,47,112,111,108,121,103,111,110,62,10,32,32,32,32,32,32,32,32,32,32,
32,32,60,47,103,62,10,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,60,47,103,62,10,60,47,115,118,103,62,0,0};

const char* VASTWaveTableEditorViewHeader::generate_svg = (const char*) resource_VASTWaveTableEditorViewHeader_generate_svg;
const int VASTWaveTableEditorViewHeader::generate_svgSize = 1044;

// JUCER_RESOURCE: invert_svg, 1014, "../../Resources/Icons/invert.svg"
static const unsigned char resource_VASTWaveTableEditorViewHeader_invert_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,70,45,
56,34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,32,119,105,100,116,104,61,34,50,53,112,120,34,32,104,101,105,103,104,116,61,34,50,49,112,120,34,32,118,105,101,119,
66,111,120,61,34,48,32,48,32,50,53,32,50,49,34,32,118,101,114,115,105,111,110,61,34,49,46,49,34,32,120,109,108,110,115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,
47,115,118,103,34,32,120,109,108,110,115,58,120,108,105,110,107,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,120,108,105,110,107,34,62,10,32,32,32,32,60,33,45,
45,32,71,101,110,101,114,97,116,111,114,58,32,83,107,101,116,99,104,32,51,46,56,46,49,32,40,50,57,54,56,55,41,32,45,32,104,116,116,112,58,47,47,119,119,119,46,98,111,104,101,109,105,97,110,99,111,100,
105,110,103,46,99,111,109,47,115,107,101,116,99,104,32,45,45,62,10,32,32,32,32,60,116,105,116,108,101,62,97,114,114,111,119,95,117,112,32,91,35,51,54,57,93,60,47,116,105,116,108,101,62,10,32,32,32,32,
60,100,101,115,99,62,67,114,101,97,116,101,100,32,119,105,116,104,32,83,107,101,116,99,104,46,60,47,100,101,115,99,62,10,32,32,32,32,60,100,101,102,115,62,60,47,100,101,102,115,62,10,32,32,32,32,60,103,
32,105,100,61,34,80,97,103,101,45,49,34,32,115,116,114,111,107,101,61,34,110,111,110,101,34,32,115,116,114,111,107,101,45,119,105,100,116,104,61,34,49,34,32,102,105,108,108,61,34,110,111,110,101,34,32,
102,105,108,108,45,114,117,108,101,61,34,101,118,101,110,111,100,100,34,62,10,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,68,114,105,98,98,98,108,101,45,68,97,114,107,45,80,114,101,118,105,101,119,
34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,45,49,56,48,46,48,48,48,48,48,48,44,32,45,54,53,53,57,46,48,48,48,48,48,48,41,34,32,102,105,108,108,61,34,35,70,70,70,
70,70,70,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,105,99,111,110,115,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,53,54,46,48,48,48,48,
48,48,44,32,49,54,48,46,48,48,48,48,48,48,41,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,60,112,97,116,104,32,100,61,34,77,49,52,50,44,54,52,49,55,46,49,52,54,32,76,49,52,50,44,54,52,48,50,
46,56,50,56,32,76,49,51,56,46,54,56,52,44,54,52,48,54,46,48,55,49,32,76,49,51,55,46,50,51,52,44,54,52,48,52,46,54,53,55,32,67,49,51,57,46,51,53,53,44,54,52,48,50,46,53,51,53,32,49,52,48,46,55,50,56,44,
54,52,48,49,46,49,52,52,32,49,52,50,46,56,55,50,44,54,51,57,57,32,67,49,52,52,46,57,51,56,44,54,52,48,49,46,48,54,54,32,49,52,54,46,53,49,53,44,54,52,48,50,46,54,53,50,32,49,52,56,46,53,50,44,54,52,48,
52,46,54,53,55,32,76,49,52,55,46,49,55,52,44,54,52,48,54,46,48,55,49,32,76,49,52,52,44,54,52,48,50,46,56,50,56,32,76,49,52,52,44,54,52,49,57,46,49,52,54,32,76,49,50,52,44,54,52,49,57,46,49,52,54,32,76,
49,50,52,44,54,51,57,57,46,49,52,54,32,76,49,50,54,44,54,51,57,57,46,49,52,54,32,76,49,50,54,44,54,52,49,55,46,49,52,54,32,76,49,52,50,44,54,52,49,55,46,49,52,54,32,90,34,32,105,100,61,34,97,114,114,111,
119,95,117,112,45,91,35,51,54,57,93,34,62,60,47,112,97,116,104,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,60,47,103,62,10,60,47,115,118,
103,62,0,0};

const char* VASTWaveTableEditorViewHeader::invert_svg = (const char*) resource_VASTWaveTableEditorViewHeader_invert_svg;
const int VASTWaveTableEditorViewHeader::invert_svgSize = 1014;

// JUCER_RESOURCE: load_svg, 907, "../../Resources/Icons/load.svg"
static const unsigned char resource_VASTWaveTableEditorViewHeader_load_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,70,45,
56,34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,32,119,105,100,116,104,61,34,50,48,112,120,34,32,104,101,105,103,104,116,61,34,50,48,112,120,34,32,118,105,101,119,
66,111,120,61,34,48,32,48,32,50,48,32,50,48,34,32,118,101,114,115,105,111,110,61,34,49,46,49,34,32,120,109,108,110,115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,
47,115,118,103,34,32,120,109,108,110,115,58,120,108,105,110,107,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,120,108,105,110,107,34,62,10,32,32,32,32,60,33,45,
45,32,71,101,110,101,114,97,116,111,114,58,32,83,107,101,116,99,104,32,51,46,56,46,49,32,40,50,57,54,56,55,41,32,45,32,104,116,116,112,58,47,47,119,119,119,46,98,111,104,101,109,105,97,110,99,111,100,
105,110,103,46,99,111,109,47,115,107,101,116,99,104,32,45,45,62,10,32,32,32,32,60,116,105,116,108,101,62,98,114,111,119,115,101,114,32,91,35,50,51,51,93,60,47,116,105,116,108,101,62,10,32,32,32,32,60,
100,101,115,99,62,67,114,101,97,116,101,100,32,119,105,116,104,32,83,107,101,116,99,104,46,60,47,100,101,115,99,62,10,32,32,32,32,60,100,101,102,115,62,60,47,100,101,102,115,62,10,32,32,32,32,60,103,32,
105,100,61,34,80,97,103,101,45,49,34,32,115,116,114,111,107,101,61,34,110,111,110,101,34,32,115,116,114,111,107,101,45,119,105,100,116,104,61,34,49,34,32,102,105,108,108,61,34,110,111,110,101,34,32,102,
105,108,108,45,114,117,108,101,61,34,101,118,101,110,111,100,100,34,62,10,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,68,114,105,98,98,98,108,101,45,68,97,114,107,45,80,114,101,118,105,101,119,34,
32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,45,54,48,46,48,48,48,48,48,48,44,32,45,55,49,53,57,46,48,48,48,48,48,48,41,34,32,102,105,108,108,61,34,35,70,70,70,70,70,
70,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,105,99,111,110,115,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,53,54,46,48,48,48,48,48,48,
44,32,49,54,48,46,48,48,48,48,48,48,41,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,60,112,97,116,104,32,100,61,34,77,49,48,44,55,48,48,51,32,76,50,50,44,55,48,48,51,32,76,50,50,44,55,48,48,
49,32,76,49,48,44,55,48,48,49,32,76,49,48,44,55,48,48,51,32,90,32,77,54,44,55,48,49,55,32,76,50,50,44,55,48,49,55,32,76,50,50,44,55,48,48,53,32,76,54,44,55,48,48,53,32,76,54,44,55,48,49,55,32,90,32,77,
54,44,55,48,48,51,32,76,56,44,55,48,48,51,32,76,56,44,55,48,48,49,32,76,54,44,55,48,48,49,32,76,54,44,55,48,48,51,32,90,32,77,52,44,55,48,49,57,32,76,50,52,44,55,48,49,57,32,76,50,52,44,54,57,57,57,32,
76,52,44,54,57,57,57,32,76,52,44,55,48,49,57,32,90,34,32,105,100,61,34,98,114,111,119,115,101,114,45,91,35,50,51,51,93,34,62,60,47,112,97,116,104,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,47,103,62,
10,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,60,47,103,62,10,60,47,115,118,103,62,0,0};

const char* VASTWaveTableEditorViewHeader::load_svg = (const char*) resource_VASTWaveTableEditorViewHeader_load_svg;
const int VASTWaveTableEditorViewHeader::load_svgSize = 907;

// JUCER_RESOURCE: minus_svg, 1301, "../../Resources/Icons/minus.svg"
static const unsigned char resource_VASTWaveTableEditorViewHeader_minus_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,70,45,
56,34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,32,119,105,100,116,104,61,34,50,49,112,120,34,32,104,101,105,103,104,116,61,34,50,48,112,120,34,32,118,105,101,119,
66,111,120,61,34,48,32,48,32,50,49,32,50,48,34,32,118,101,114,115,105,111,110,61,34,49,46,49,34,32,120,109,108,110,115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,
47,115,118,103,34,32,120,109,108,110,115,58,120,108,105,110,107,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,120,108,105,110,107,34,62,10,32,32,32,32,60,33,45,
45,32,71,101,110,101,114,97,116,111,114,58,32,83,107,101,116,99,104,32,51,46,56,46,49,32,40,50,57,54,56,55,41,32,45,32,104,116,116,112,58,47,47,119,119,119,46,98,111,104,101,109,105,97,110,99,111,100,
105,110,103,46,99,111,109,47,115,107,101,116,99,104,32,45,45,62,10,32,32,32,32,60,116,105,116,108,101,62,109,105,110,117,115,32,91,35,49,52,53,54,93,60,47,116,105,116,108,101,62,10,32,32,32,32,60,100,
101,115,99,62,67,114,101,97,116,101,100,32,119,105,116,104,32,83,107,101,116,99,104,46,60,47,100,101,115,99,62,10,32,32,32,32,60,100,101,102,115,62,60,47,100,101,102,115,62,10,32,32,32,32,60,103,32,105,
100,61,34,80,97,103,101,45,49,34,32,115,116,114,111,107,101,61,34,110,111,110,101,34,32,115,116,114,111,107,101,45,119,105,100,116,104,61,34,49,34,32,102,105,108,108,61,34,110,111,110,101,34,32,102,105,
108,108,45,114,117,108,101,61,34,101,118,101,110,111,100,100,34,62,10,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,68,114,105,98,98,98,108,101,45,68,97,114,107,45,80,114,101,118,105,101,119,34,32,116,
114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,45,50,49,57,46,48,48,48,48,48,48,44,32,45,52,56,48,46,48,48,48,48,48,48,41,34,32,102,105,108,108,61,34,35,70,70,70,70,70,70,34,
62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,105,99,111,110,115,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,53,54,46,48,48,48,48,48,48,44,32,
49,54,48,46,48,48,48,48,48,48,41,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,60,112,97,116,104,32,100,61,34,77,49,54,57,46,51,44,51,51,49,32,76,49,55,55,46,55,44,51,51,49,32,76,49,55,55,46,
55,44,51,50,57,32,76,49,54,57,46,51,44,51,50,57,32,76,49,54,57,46,51,44,51,51,49,32,90,32,77,49,56,49,46,57,44,51,51,52,32,76,49,56,52,44,51,51,52,32,76,49,56,52,44,51,50,54,32,76,49,56,49,46,57,44,51,
50,54,32,76,49,56,49,46,57,44,51,51,52,32,90,32,77,49,56,49,46,57,44,51,51,56,32,76,49,55,57,46,56,44,51,51,56,32,76,49,55,57,46,56,44,51,52,48,32,76,49,56,52,44,51,52,48,32,76,49,56,52,44,51,51,54,32,
76,49,56,49,46,57,44,51,51,54,32,76,49,56,49,46,57,44,51,51,56,32,90,32,77,49,55,57,46,56,44,51,50,48,32,76,49,55,57,46,56,44,51,50,50,32,76,49,56,49,46,57,44,51,50,50,32,76,49,56,49,46,57,44,51,50,52,
32,76,49,56,52,44,51,50,52,32,76,49,56,52,44,51,50,48,32,76,49,55,57,46,56,44,51,50,48,32,90,32,77,49,54,51,44,51,51,52,32,76,49,54,53,46,49,44,51,51,52,32,76,49,54,53,46,49,44,51,50,54,32,76,49,54,51,
44,51,50,54,32,76,49,54,51,44,51,51,52,32,90,32,77,49,54,53,46,49,44,51,51,54,32,76,49,54,51,44,51,51,54,32,76,49,54,51,44,51,52,48,32,76,49,54,55,46,50,44,51,52,48,32,76,49,54,55,46,50,44,51,51,56,32,
76,49,54,53,46,49,44,51,51,56,32,76,49,54,53,46,49,44,51,51,54,32,90,32,77,49,54,51,44,51,50,48,32,76,49,54,51,44,51,50,52,32,76,49,54,53,46,49,44,51,50,52,32,76,49,54,53,46,49,44,51,50,50,32,76,49,54,
55,46,50,44,51,50,50,32,76,49,54,55,46,50,44,51,50,48,32,76,49,54,51,44,51,50,48,32,90,32,77,49,54,57,46,51,44,51,52,48,32,76,49,55,55,46,55,44,51,52,48,32,76,49,55,55,46,55,44,51,51,56,32,76,49,54,57,
46,51,44,51,51,56,32,76,49,54,57,46,51,44,51,52,48,32,90,32,77,49,54,57,46,51,44,51,50,50,32,76,49,55,55,46,55,44,51,50,50,32,76,49,55,55,46,55,44,51,50,48,32,76,49,54,57,46,51,44,51,50,48,32,76,49,54,
57,46,51,44,51,50,50,32,90,34,32,105,100,61,34,109,105,110,117,115,45,91,35,49,52,53,54,93,34,62,60,47,112,97,116,104,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,32,32,32,32,
60,47,103,62,10,32,32,32,32,60,47,103,62,10,60,47,115,118,103,62,0,0};

const char* VASTWaveTableEditorViewHeader::minus_svg = (const char*) resource_VASTWaveTableEditorViewHeader_minus_svg;
const int VASTWaveTableEditorViewHeader::minus_svgSize = 1301;

// JUCER_RESOURCE: normalize_svg, 1091, "../../Resources/Icons/normalize.svg"
static const unsigned char resource_VASTWaveTableEditorViewHeader_normalize_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,70,
45,56,34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,32,119,105,100,116,104,61,34,50,48,112,120,34,32,104,101,105,103,104,116,61,34,50,48,112,120,34,32,118,105,101,
119,66,111,120,61,34,48,32,48,32,50,48,32,50,48,34,32,118,101,114,115,105,111,110,61,34,49,46,49,34,32,120,109,108,110,115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,
48,48,47,115,118,103,34,32,120,109,108,110,115,58,120,108,105,110,107,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,120,108,105,110,107,34,62,10,32,32,32,32,60,
33,45,45,32,71,101,110,101,114,97,116,111,114,58,32,83,107,101,116,99,104,32,51,46,56,46,49,32,40,50,57,54,56,55,41,32,45,32,104,116,116,112,58,47,47,119,119,119,46,98,111,104,101,109,105,97,110,99,111,
100,105,110,103,46,99,111,109,47,115,107,101,116,99,104,32,45,45,62,10,32,32,32,32,60,116,105,116,108,101,62,118,111,108,117,109,101,95,108,111,119,32,91,35,57,50,53,93,60,47,116,105,116,108,101,62,10,
32,32,32,32,60,100,101,115,99,62,67,114,101,97,116,101,100,32,119,105,116,104,32,83,107,101,116,99,104,46,60,47,100,101,115,99,62,10,32,32,32,32,60,100,101,102,115,62,60,47,100,101,102,115,62,10,32,32,
32,32,60,103,32,105,100,61,34,80,97,103,101,45,49,34,32,115,116,114,111,107,101,61,34,110,111,110,101,34,32,115,116,114,111,107,101,45,119,105,100,116,104,61,34,49,34,32,102,105,108,108,61,34,110,111,
110,101,34,32,102,105,108,108,45,114,117,108,101,61,34,101,118,101,110,111,100,100,34,62,10,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,68,114,105,98,98,98,108,101,45,68,97,114,107,45,80,114,101,118,
105,101,119,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,45,50,54,48,46,48,48,48,48,48,48,44,32,45,52,48,55,57,46,48,48,48,48,48,48,41,34,32,102,105,108,108,61,34,
35,70,70,70,70,70,70,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,105,99,111,110,115,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,53,54,46,
48,48,48,48,48,48,44,32,49,54,48,46,48,48,48,48,48,48,41,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,60,112,97,116,104,32,100,61,34,77,50,50,50,44,51,57,51,51,46,53,32,76,50,50,52,44,51,57,
51,51,46,53,32,76,50,50,52,44,51,57,50,53,46,53,32,76,50,50,50,44,51,57,50,53,46,53,32,76,50,50,50,44,51,57,51,51,46,53,32,90,32,77,50,48,54,44,51,57,51,49,46,53,32,76,50,48,56,44,51,57,51,49,46,53,32,
76,50,48,56,44,51,57,50,55,46,53,32,76,50,48,54,44,51,57,50,55,46,53,32,76,50,48,54,44,51,57,51,49,46,53,32,90,32,77,50,49,48,44,51,57,50,54,46,50,51,54,32,67,50,49,50,46,57,50,50,44,51,57,50,52,46,55,
55,53,32,50,49,53,46,49,54,52,44,51,57,50,51,46,54,53,52,32,50,49,56,44,51,57,50,50,46,50,51,54,32,76,50,49,56,44,51,57,51,53,46,55,54,52,32,67,50,49,53,46,49,54,53,44,51,57,51,52,46,51,52,54,32,50,49,
50,46,57,50,49,44,51,57,51,51,46,50,50,53,32,50,49,48,44,51,57,51,49,46,55,54,52,32,76,50,49,48,44,51,57,50,54,46,50,51,54,32,90,32,77,50,48,52,44,51,57,50,53,46,53,32,76,50,48,52,44,51,57,51,51,46,53,
32,76,50,48,55,46,54,54,55,44,51,57,51,51,46,53,32,76,50,50,48,44,51,57,51,57,32,76,50,50,48,44,51,57,49,57,32,76,50,48,55,46,54,54,55,44,51,57,50,53,46,53,32,76,50,48,52,44,51,57,50,53,46,53,32,90,34,
32,105,100,61,34,118,111,108,117,109,101,95,108,111,119,45,91,35,57,50,53,93,34,62,60,47,112,97,116,104,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,32,32,32,32,60,47,103,62,10,
32,32,32,32,60,47,103,62,10,60,47,115,118,103,62,0,0};

const char* VASTWaveTableEditorViewHeader::normalize_svg = (const char*) resource_VASTWaveTableEditorViewHeader_normalize_svg;
const int VASTWaveTableEditorViewHeader::normalize_svgSize = 1091;

// JUCER_RESOURCE: normalize2_svg, 960, "../../Resources/Icons/normalize2.svg"
static const unsigned char resource_VASTWaveTableEditorViewHeader_normalize2_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,
70,45,56,34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,32,119,105,100,116,104,61,34,50,49,112,120,34,32,104,101,105,103,104,116,61,34,50,48,112,120,34,32,118,105,
101,119,66,111,120,61,34,48,32,48,32,50,49,32,50,48,34,32,118,101,114,115,105,111,110,61,34,49,46,49,34,32,120,109,108,110,115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,
48,48,48,47,115,118,103,34,32,120,109,108,110,115,58,120,108,105,110,107,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,120,108,105,110,107,34,62,10,32,32,32,32,
60,33,45,45,32,71,101,110,101,114,97,116,111,114,58,32,83,107,101,116,99,104,32,51,46,56,46,49,32,40,50,57,54,56,55,41,32,45,32,104,116,116,112,58,47,47,119,119,119,46,98,111,104,101,109,105,97,110,99,
111,100,105,110,103,46,99,111,109,47,115,107,101,116,99,104,32,45,45,62,10,32,32,32,32,60,116,105,116,108,101,62,119,105,110,100,111,119,95,101,113,117,97,108,32,91,35,49,52,54,53,93,60,47,116,105,116,
108,101,62,10,32,32,32,32,60,100,101,115,99,62,67,114,101,97,116,101,100,32,119,105,116,104,32,83,107,101,116,99,104,46,60,47,100,101,115,99,62,10,32,32,32,32,60,100,101,102,115,62,60,47,100,101,102,115,
62,10,32,32,32,32,60,103,32,105,100,61,34,80,97,103,101,45,49,34,32,115,116,114,111,107,101,61,34,110,111,110,101,34,32,115,116,114,111,107,101,45,119,105,100,116,104,61,34,49,34,32,102,105,108,108,61,
34,110,111,110,101,34,32,102,105,108,108,45,114,117,108,101,61,34,101,118,101,110,111,100,100,34,62,10,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,68,114,105,98,98,98,108,101,45,68,97,114,107,45,80,
114,101,118,105,101,119,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,45,50,53,57,46,48,48,48,48,48,48,44,32,45,52,52,48,46,48,48,48,48,48,48,41,34,32,102,105,108,
108,61,34,35,70,70,70,70,70,70,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,105,99,111,110,115,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,
40,53,54,46,48,48,48,48,48,48,44,32,49,54,48,46,48,48,48,48,48,48,41,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,60,112,97,116,104,32,100,61,34,77,50,48,55,46,50,44,50,57,51,32,76,50,49,57,
46,56,44,50,57,51,32,76,50,49,57,46,56,44,50,57,49,32,76,50,48,55,46,50,44,50,57,49,32,76,50,48,55,46,50,44,50,57,51,32,90,32,77,50,48,55,46,50,44,50,56,57,32,76,50,49,57,46,56,44,50,56,57,32,76,50,49,
57,46,56,44,50,56,55,32,76,50,48,55,46,50,44,50,56,55,32,76,50,48,55,46,50,44,50,56,57,32,90,32,77,50,48,53,46,49,44,50,57,56,32,76,50,50,49,46,57,44,50,57,56,32,76,50,50,49,46,57,44,50,56,50,32,76,50,
48,53,46,49,44,50,56,50,32,76,50,48,53,46,49,44,50,57,56,32,90,32,77,50,48,51,44,51,48,48,32,76,50,50,52,44,51,48,48,32,76,50,50,52,44,50,56,48,32,76,50,48,51,44,50,56,48,32,76,50,48,51,44,51,48,48,32,
90,34,32,105,100,61,34,119,105,110,100,111,119,95,101,113,117,97,108,45,91,35,49,52,54,53,93,34,62,60,47,112,97,116,104,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,32,32,32,32,
60,47,103,62,10,32,32,32,32,60,47,103,62,10,60,47,115,118,103,62,0,0};

const char* VASTWaveTableEditorViewHeader::normalize2_svg = (const char*) resource_VASTWaveTableEditorViewHeader_normalize2_svg;
const int VASTWaveTableEditorViewHeader::normalize2_svgSize = 960;

// JUCER_RESOURCE: normalize3_svg, 1964, "../../Resources/Icons/normalize3.svg"
static const unsigned char resource_VASTWaveTableEditorViewHeader_normalize3_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,
70,45,56,34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,32,119,105,100,116,104,61,34,50,48,112,120,34,32,104,101,105,103,104,116,61,34,50,48,112,120,34,32,118,105,
101,119,66,111,120,61,34,48,32,48,32,50,48,32,50,48,34,32,118,101,114,115,105,111,110,61,34,49,46,49,34,32,120,109,108,110,115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,
48,48,48,47,115,118,103,34,32,120,109,108,110,115,58,120,108,105,110,107,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,120,108,105,110,107,34,62,10,32,32,32,32,
60,33,45,45,32,71,101,110,101,114,97,116,111,114,58,32,83,107,101,116,99,104,32,51,46,56,46,49,32,40,50,57,54,56,55,41,32,45,32,104,116,116,112,58,47,47,119,119,119,46,98,111,104,101,109,105,97,110,99,
111,100,105,110,103,46,99,111,109,47,115,107,101,116,99,104,32,45,45,62,10,32,32,32,32,60,116,105,116,108,101,62,97,114,114,111,119,95,105,110,115,105,100,101,32,91,35,50,54,49,93,60,47,116,105,116,108,
101,62,10,32,32,32,32,60,100,101,115,99,62,67,114,101,97,116,101,100,32,119,105,116,104,32,83,107,101,116,99,104,46,60,47,100,101,115,99,62,10,32,32,32,32,60,100,101,102,115,62,60,47,100,101,102,115,62,
10,32,32,32,32,60,103,32,105,100,61,34,80,97,103,101,45,49,34,32,115,116,114,111,107,101,61,34,110,111,110,101,34,32,115,116,114,111,107,101,45,119,105,100,116,104,61,34,49,34,32,102,105,108,108,61,34,
110,111,110,101,34,32,102,105,108,108,45,114,117,108,101,61,34,101,118,101,110,111,100,100,34,62,10,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,68,114,105,98,98,98,108,101,45,68,97,114,107,45,80,114,
101,118,105,101,119,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,45,49,48,48,46,48,48,48,48,48,48,44,32,45,54,57,57,57,46,48,48,48,48,48,48,41,34,32,102,105,108,
108,61,34,35,70,70,70,70,70,70,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,105,99,111,110,115,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,
40,53,54,46,48,48,48,48,48,48,44,32,49,54,48,46,48,48,48,48,48,48,41,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,60,112,97,116,104,32,100,61,34,77,54,51,44,54,56,53,55,32,76,53,56,44,54,56,
53,55,32,76,53,56,44,54,56,53,50,46,56,54,51,32,67,53,56,44,54,56,53,50,46,52,49,56,32,53,56,46,53,51,57,44,54,56,53,50,46,49,57,53,32,53,56,46,56,53,52,44,54,56,53,50,46,53,49,32,76,54,48,46,53,51,54,
44,54,56,53,52,46,49,57,51,32,67,54,48,46,57,50,54,44,54,56,53,52,46,53,56,51,32,54,49,46,53,54,44,54,56,53,52,46,53,56,51,32,54,49,46,57,53,44,54,56,53,52,46,49,57,51,32,67,54,50,46,51,52,49,44,54,56,
53,51,46,56,48,50,32,54,50,46,51,52,49,44,54,56,53,51,46,49,54,57,32,54,49,46,57,53,44,54,56,53,50,46,55,55,57,32,76,53,56,46,52,49,51,44,54,56,52,57,46,50,52,50,32,67,53,55,46,54,51,51,44,54,56,52,56,
46,52,54,49,32,53,54,46,51,54,55,44,54,56,52,56,46,52,54,49,32,53,53,46,53,56,55,44,54,56,52,57,46,50,52,50,32,67,53,53,46,48,50,49,44,54,56,52,57,46,56,48,56,32,53,52,46,57,48,49,44,54,56,52,57,46,57,
50,55,32,53,50,46,48,53,44,54,56,53,50,46,55,55,56,32,67,53,49,46,54,54,44,54,56,53,51,46,49,54,57,32,53,49,46,54,54,44,54,56,53,51,46,56,48,50,32,53,50,46,48,53,49,44,54,56,53,52,46,49,57,51,32,67,53,
50,46,52,52,49,44,54,56,53,52,46,53,56,51,32,53,51,46,48,55,52,44,54,56,53,52,46,53,56,51,32,53,51,46,52,54,53,44,54,56,53,52,46,49,57,51,32,76,53,53,46,49,52,54,44,54,56,53,50,46,53,49,50,32,67,53,53,
46,52,54,49,44,54,56,53,50,46,49,57,55,32,53,54,44,54,56,53,50,46,52,50,32,53,54,44,54,56,53,50,46,56,54,53,32,76,53,54,44,54,56,53,55,32,76,52,53,44,54,56,53,55,32,67,52,52,46,52,52,56,44,54,56,53,55,
32,52,52,44,54,56,53,55,46,52,52,56,32,52,52,44,54,56,53,56,32,67,52,52,44,54,56,53,56,46,53,53,50,32,52,52,46,52,52,56,44,54,56,53,57,32,52,53,44,54,56,53,57,32,76,54,51,44,54,56,53,57,32,67,54,51,46,
53,53,50,44,54,56,53,57,32,54,52,44,54,56,53,56,46,53,53,50,32,54,52,44,54,56,53,56,32,67,54,52,44,54,56,53,55,46,52,52,56,32,54,51,46,53,53,50,44,54,56,53,55,32,54,51,44,54,56,53,55,32,77,52,53,44,54,
56,52,49,32,76,53,48,44,54,56,52,49,32,76,53,48,44,54,56,52,53,46,49,51,55,32,67,53,48,44,54,56,52,53,46,53,56,50,32,52,57,46,52,54,49,44,54,56,52,53,46,56,48,53,32,52,57,46,49,52,54,44,54,56,52,53,46,
52,57,32,76,52,55,46,52,54,52,44,54,56,52,51,46,56,48,55,32,67,52,55,46,48,55,52,44,54,56,52,51,46,52,49,55,32,52,54,46,52,52,44,54,56,52,51,46,52,49,55,32,52,54,46,48,53,44,54,56,52,51,46,56,48,55,32,
67,52,53,46,54,54,44,54,56,52,52,46,49,57,56,32,52,53,46,54,54,44,54,56,52,52,46,56,51,49,32,52,54,46,48,53,44,54,56,52,53,46,50,50,49,32,76,52,57,46,53,56,55,44,54,56,52,56,46,55,53,56,32,67,53,48,46,
51,54,56,44,54,56,52,57,46,53,51,57,32,53,49,46,54,51,51,44,54,56,52,57,46,53,51,57,32,53,50,46,52,49,51,44,54,56,52,56,46,55,53,56,32,67,53,50,46,57,56,51,44,54,56,52,56,46,49,56,57,32,53,51,46,48,57,
56,44,54,56,52,56,46,48,55,52,32,53,53,46,57,53,44,54,56,52,53,46,50,50,50,32,67,53,54,46,51,52,44,54,56,52,52,46,56,51,49,32,53,54,46,51,52,44,54,56,52,52,46,49,57,56,32,53,53,46,57,53,44,54,56,52,51,
46,56,48,55,32,67,53,53,46,53,53,57,44,54,56,52,51,46,52,49,55,32,53,52,46,57,50,54,44,54,56,52,51,46,52,49,55,32,53,52,46,53,51,53,44,54,56,52,51,46,56,48,55,32,76,53,50,46,56,53,52,44,54,56,52,53,46,
52,56,56,32,67,53,50,46,53,51,57,44,54,56,52,53,46,56,48,51,32,53,50,44,54,56,52,53,46,53,56,32,53,50,44,54,56,52,53,46,49,51,53,32,76,53,50,44,54,56,52,49,32,76,54,51,44,54,56,52,49,32,67,54,51,46,53,
53,50,44,54,56,52,49,32,54,52,44,54,56,52,48,46,53,53,50,32,54,52,44,54,56,52,48,32,67,54,52,44,54,56,51,57,46,52,52,56,32,54,51,46,53,53,50,44,54,56,51,57,32,54,51,44,54,56,51,57,32,76,52,53,44,54,56,
51,57,32,67,52,52,46,52,52,56,44,54,56,51,57,32,52,52,44,54,56,51,57,46,52,52,56,32,52,52,44,54,56,52,48,32,67,52,52,44,54,56,52,48,46,53,53,50,32,52,52,46,52,52,56,44,54,56,52,49,32,52,53,44,54,56,52,
49,34,32,105,100,61,34,97,114,114,111,119,95,105,110,115,105,100,101,45,91,35,50,54,49,93,34,62,60,47,112,97,116,104,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,32,32,32,32,60,
47,103,62,10,32,32,32,32,60,47,103,62,10,60,47,115,118,103,62,0,0};

const char* VASTWaveTableEditorViewHeader::normalize3_svg = (const char*) resource_VASTWaveTableEditorViewHeader_normalize3_svg;
const int VASTWaveTableEditorViewHeader::normalize3_svgSize = 1964;

// JUCER_RESOURCE: paste_svg, 1977, "../../Resources/Icons/paste.svg"
static const unsigned char resource_VASTWaveTableEditorViewHeader_paste_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,70,45,
56,34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,32,119,105,100,116,104,61,34,50,48,112,120,34,32,104,101,105,103,104,116,61,34,50,48,112,120,34,32,118,105,101,119,
66,111,120,61,34,48,32,48,32,50,48,32,50,48,34,32,118,101,114,115,105,111,110,61,34,49,46,49,34,32,120,109,108,110,115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,
47,115,118,103,34,32,120,109,108,110,115,58,120,108,105,110,107,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,120,108,105,110,107,34,62,10,32,32,32,32,60,33,45,
45,32,71,101,110,101,114,97,116,111,114,58,32,83,107,101,116,99,104,32,51,46,56,46,49,32,40,50,57,54,56,55,41,32,45,32,104,116,116,112,58,47,47,119,119,119,46,98,111,104,101,109,105,97,110,99,111,100,
105,110,103,46,99,111,109,47,115,107,101,116,99,104,32,45,45,62,10,32,32,32,32,60,116,105,116,108,101,62,97,114,114,111,119,95,105,110,95,100,111,119,110,32,91,35,51,52,53,93,60,47,116,105,116,108,101,
62,10,32,32,32,32,60,100,101,115,99,62,67,114,101,97,116,101,100,32,119,105,116,104,32,83,107,101,116,99,104,46,60,47,100,101,115,99,62,10,32,32,32,32,60,100,101,102,115,62,60,47,100,101,102,115,62,10,
32,32,32,32,60,103,32,105,100,61,34,80,97,103,101,45,49,34,32,115,116,114,111,107,101,61,34,110,111,110,101,34,32,115,116,114,111,107,101,45,119,105,100,116,104,61,34,49,34,32,102,105,108,108,61,34,110,
111,110,101,34,32,102,105,108,108,45,114,117,108,101,61,34,101,118,101,110,111,100,100,34,62,10,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,68,114,105,98,98,98,108,101,45,68,97,114,107,45,80,114,101,
118,105,101,119,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,45,51,52,48,46,48,48,48,48,48,48,44,32,45,54,54,51,57,46,48,48,48,48,48,48,41,34,32,102,105,108,108,
61,34,35,70,70,70,70,70,70,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,105,99,111,110,115,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,53,
54,46,48,48,48,48,48,48,44,32,49,54,48,46,48,48,48,48,48,48,41,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,60,112,97,116,104,32,100,61,34,77,50,57,56,46,50,55,52,44,54,52,56,53,46,54,55,51,
32,76,50,57,56,46,50,55,49,44,54,52,56,53,46,54,54,57,32,67,50,57,55,46,56,57,54,44,54,52,56,53,46,50,54,54,32,50,57,55,46,50,54,53,44,54,52,56,53,46,50,52,52,32,50,57,54,46,56,54,51,44,54,52,56,53,46,
54,50,32,76,50,57,53,46,56,52,50,44,54,52,56,54,46,53,55,55,32,67,50,57,53,46,53,50,50,44,54,52,56,54,46,56,55,54,32,50,57,53,44,54,52,56,54,46,54,52,57,32,50,57,53,44,54,52,56,54,46,50,49,50,32,76,50,
57,53,44,54,52,56,48,32,67,50,57,53,44,54,52,55,57,46,52,52,55,32,50,57,52,46,53,53,50,44,54,52,55,57,32,50,57,52,44,54,52,55,57,32,67,50,57,51,46,52,52,56,44,54,52,55,57,32,50,57,51,44,54,52,55,57,46,
52,52,55,32,50,57,51,44,54,52,56,48,32,76,50,57,51,44,54,52,56,54,46,49,57,55,32,67,50,57,51,44,54,52,56,54,46,54,51,55,32,50,57,50,46,52,55,51,44,54,52,56,54,46,56,54,50,32,50,57,50,46,49,53,53,44,54,
52,56,54,46,53,53,56,32,76,50,57,49,46,49,56,49,44,54,52,56,53,46,54,51,32,67,50,57,48,46,55,56,50,44,54,52,56,53,46,50,52,56,32,50,57,48,46,49,52,56,44,54,52,56,53,46,50,54,54,32,50,56,57,46,55,55,49,
44,54,52,56,53,46,54,54,57,32,67,50,56,57,46,51,57,55,44,54,52,56,54,46,48,54,56,32,50,56,57,46,52,49,54,44,54,52,56,54,46,54,57,53,32,50,56,57,46,56,49,52,44,54,52,56,55,46,48,55,49,32,76,50,57,50,46,
54,54,50,44,54,52,56,57,46,55,53,57,32,67,50,57,51,46,52,51,54,44,54,52,57,48,46,52,57,32,50,57,52,46,54,52,54,44,54,52,57,48,46,52,56,55,32,50,57,53,46,52,49,53,44,54,52,56,57,46,55,53,51,32,76,50,57,
56,46,50,51,49,44,54,52,56,55,46,48,54,55,32,67,50,57,56,46,54,50,52,44,54,52,56,54,46,54,57,50,32,50,57,56,46,54,52,51,44,54,52,56,54,46,48,55,49,32,50,57,56,46,50,55,52,44,54,52,56,53,46,54,55,51,32,
76,50,57,56,46,50,55,52,44,54,52,56,53,46,54,55,51,32,90,32,77,51,48,52,44,54,52,57,55,46,50,50,50,32,67,51,48,52,44,54,52,57,56,46,50,32,51,48,51,46,51,50,44,54,52,57,57,32,51,48,49,46,56,56,56,44,54,
52,57,57,32,76,50,56,53,46,55,55,56,44,54,52,57,57,32,67,50,56,52,46,55,57,54,44,54,52,57,57,32,50,56,52,44,54,52,57,56,46,50,48,52,32,50,56,52,44,54,52,57,55,46,50,50,50,32,76,50,56,52,44,54,52,57,48,
46,55,55,55,32,67,50,56,52,44,54,52,56,57,46,56,52,55,32,50,56,52,46,55,48,55,44,54,52,56,57,32,50,56,53,46,56,56,56,44,54,52,56,57,32,76,50,56,55,46,48,48,55,44,54,52,56,57,32,67,50,56,55,46,53,53,53,
44,54,52,56,57,32,50,56,56,44,54,52,56,57,46,52,52,52,32,50,56,56,44,54,52,56,57,46,57,57,51,32,76,50,56,56,44,54,52,57,48,46,48,48,54,32,67,50,56,56,44,54,52,57,48,46,53,53,53,32,50,56,55,46,53,53,53,
44,54,52,57,49,32,50,56,55,46,48,48,55,44,54,52,57,49,32,76,50,56,54,46,57,57,51,44,54,52,57,49,32,67,50,56,54,46,52,52,53,44,54,52,57,49,32,50,56,54,44,54,52,57,49,46,52,52,52,32,50,56,54,44,54,52,57,
49,46,57,57,51,32,76,50,56,54,44,54,52,57,54,46,48,48,54,32,67,50,56,54,44,54,52,57,54,46,53,53,53,32,50,56,54,46,52,52,53,44,54,52,57,55,32,50,56,54,46,57,57,51,44,54,52,57,55,32,76,51,48,49,46,48,48,
55,44,54,52,57,55,32,67,51,48,49,46,53,53,53,44,54,52,57,55,32,51,48,50,44,54,52,57,54,46,53,53,53,32,51,48,50,44,54,52,57,54,46,48,48,54,32,76,51,48,50,44,54,52,57,49,46,57,57,51,32,67,51,48,50,44,54,
52,57,49,46,52,52,52,32,51,48,49,46,53,53,53,44,54,52,57,49,32,51,48,49,46,48,48,55,44,54,52,57,49,32,76,51,48,48,46,57,57,51,44,54,52,57,49,32,67,51,48,48,46,52,52,53,44,54,52,57,49,32,51,48,48,44,54,
52,57,48,46,53,53,53,32,51,48,48,44,54,52,57,48,46,48,48,54,32,76,51,48,48,44,54,52,56,57,46,57,57,51,32,67,51,48,48,44,54,52,56,57,46,52,52,52,32,51,48,48,46,52,52,53,44,54,52,56,57,32,51,48,48,46,57,
57,51,44,54,52,56,57,32,76,51,48,50,46,50,50,50,44,54,52,56,57,32,67,51,48,51,46,50,48,52,44,54,52,56,57,32,51,48,52,44,54,52,56,57,46,55,57,54,32,51,48,52,44,54,52,57,48,46,55,55,55,32,76,51,48,52,44,
54,52,57,55,46,50,50,50,32,90,34,32,105,100,61,34,97,114,114,111,119,95,105,110,95,100,111,119,110,45,91,35,51,52,53,93,34,62,60,47,112,97,116,104,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,47,103,62,
10,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,60,47,103,62,10,60,47,115,118,103,62,0,0};

const char* VASTWaveTableEditorViewHeader::paste_svg = (const char*) resource_VASTWaveTableEditorViewHeader_paste_svg;
const int VASTWaveTableEditorViewHeader::paste_svgSize = 1977;

// JUCER_RESOURCE: plus_svg, 1650, "../../Resources/Icons/plus.svg"
static const unsigned char resource_VASTWaveTableEditorViewHeader_plus_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,70,45,
56,34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,32,119,105,100,116,104,61,34,50,49,112,120,34,32,104,101,105,103,104,116,61,34,50,48,112,120,34,32,118,105,101,119,
66,111,120,61,34,48,32,48,32,50,49,32,50,48,34,32,118,101,114,115,105,111,110,61,34,49,46,49,34,32,120,109,108,110,115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,
47,115,118,103,34,32,120,109,108,110,115,58,120,108,105,110,107,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,120,108,105,110,107,34,62,10,32,32,32,32,60,33,45,
45,32,71,101,110,101,114,97,116,111,114,58,32,83,107,101,116,99,104,32,51,46,56,46,49,32,40,50,57,54,56,55,41,32,45,32,104,116,116,112,58,47,47,119,119,119,46,98,111,104,101,109,105,97,110,99,111,100,
105,110,103,46,99,111,109,47,115,107,101,116,99,104,32,45,45,62,10,32,32,32,32,60,116,105,116,108,101,62,110,101,119,95,116,97,98,32,91,35,49,53,48,50,93,60,47,116,105,116,108,101,62,10,32,32,32,32,60,
100,101,115,99,62,67,114,101,97,116,101,100,32,119,105,116,104,32,83,107,101,116,99,104,46,60,47,100,101,115,99,62,10,32,32,32,32,60,100,101,102,115,62,60,47,100,101,102,115,62,10,32,32,32,32,60,103,32,
105,100,61,34,80,97,103,101,45,49,34,32,115,116,114,111,107,101,61,34,110,111,110,101,34,32,115,116,114,111,107,101,45,119,105,100,116,104,61,34,49,34,32,102,105,108,108,61,34,110,111,110,101,34,32,102,
105,108,108,45,114,117,108,101,61,34,101,118,101,110,111,100,100,34,62,10,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,68,114,105,98,98,98,108,101,45,68,97,114,107,45,80,114,101,118,105,101,119,34,
32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,45,51,55,57,46,48,48,48,48,48,48,44,32,45,50,56,48,46,48,48,48,48,48,48,41,34,32,102,105,108,108,61,34,35,70,70,70,70,70,
70,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,105,99,111,110,115,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,53,54,46,48,48,48,48,48,48,
44,32,49,54,48,46,48,48,48,48,48,48,41,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,60,112,97,116,104,32,100,61,34,77,51,52,49,46,57,44,49,51,55,46,50,54,49,32,67,51,52,49,46,57,44,49,51,55,
46,56,49,49,32,51,52,49,46,52,57,55,56,53,44,49,51,56,32,51,52,48,46,57,50,48,51,53,44,49,51,56,32,76,51,51,49,46,52,55,48,51,53,44,49,51,56,32,67,51,51,48,46,56,57,50,56,53,44,49,51,56,32,51,51,48,46,
51,53,44,49,51,55,46,56,49,49,32,51,51,48,46,51,53,44,49,51,55,46,50,54,49,32,76,51,51,48,46,51,53,44,49,51,53,32,76,51,51,55,46,50,52,53,51,53,44,49,51,53,32,67,51,51,56,46,49,49,52,55,53,44,49,51,53,
32,51,51,56,46,55,53,44,49,51,52,46,53,56,57,32,51,51,56,46,55,53,44,49,51,51,46,55,54,49,32,76,51,51,56,46,55,53,44,49,50,55,32,76,51,52,48,46,57,50,48,51,53,44,49,50,55,32,67,51,52,49,46,52,57,57,57,
53,44,49,50,55,32,51,52,49,46,57,44,49,50,55,46,55,48,57,32,51,52,49,46,57,44,49,50,56,46,50,54,49,32,76,51,52,49,46,57,44,49,51,55,46,50,54,49,32,90,32,77,51,50,54,46,50,50,48,51,53,44,49,51,51,32,67,
51,50,53,46,54,52,50,56,53,44,49,51,51,32,51,50,53,46,49,44,49,51,50,46,56,49,49,32,51,50,53,46,49,44,49,51,50,46,50,54,49,32,76,51,50,53,46,49,44,49,50,51,46,50,54,49,32,67,51,50,53,46,49,44,49,50,50,
46,55,48,57,32,51,50,53,46,54,51,57,55,44,49,50,50,32,51,50,54,46,50,50,48,51,53,44,49,50,50,32,76,51,51,53,46,54,55,48,51,53,44,49,50,50,32,67,51,51,54,46,50,52,57,57,53,44,49,50,50,32,51,51,54,46,54,
53,44,49,50,50,46,55,48,57,32,51,51,54,46,54,53,44,49,50,51,46,50,54,49,32,76,51,51,54,46,54,53,44,49,51,50,46,50,54,49,32,67,51,51,54,46,54,53,44,49,51,50,46,56,49,49,32,51,51,54,46,50,52,55,56,53,44,
49,51,51,32,51,51,53,46,54,55,48,51,53,44,49,51,51,32,76,51,50,54,46,50,50,48,51,53,44,49,51,51,32,90,32,77,51,52,50,46,52,57,53,51,53,44,49,50,53,32,76,51,51,56,46,55,53,44,49,50,53,32,76,51,51,56,46,
55,53,44,49,50,49,46,55,54,49,32,67,51,51,56,46,55,53,44,49,50,48,46,57,51,51,32,51,51,56,46,49,49,52,55,53,44,49,50,48,32,51,51,55,46,50,52,53,51,53,44,49,50,48,32,76,51,50,52,46,54,52,53,51,53,44,49,
50,48,32,67,51,50,51,46,55,55,57,49,44,49,50,48,32,51,50,51,44,49,50,48,46,57,51,54,32,51,50,51,44,49,50,49,46,55,54,49,32,76,51,50,51,44,49,51,51,46,55,54,49,32,67,51,50,51,44,49,51,52,46,53,56,54,32,
51,50,51,46,55,55,57,49,44,49,51,53,32,51,50,52,46,54,52,53,51,53,44,49,51,53,32,76,51,50,56,46,50,53,44,49,51,53,32,76,51,50,56,46,50,53,44,49,51,56,46,55,54,49,32,67,51,50,56,46,50,53,44,49,51,57,46,
53,56,54,32,51,50,57,46,48,50,57,49,44,49,52,48,32,51,50,57,46,56,57,53,51,53,44,49,52,48,32,76,51,52,50,46,52,57,53,51,53,44,49,52,48,32,67,51,52,51,46,51,54,52,55,53,44,49,52,48,32,51,52,52,44,49,51,
57,46,53,56,57,32,51,52,52,44,49,51,56,46,55,54,49,32,76,51,52,52,44,49,50,54,46,55,54,49,32,67,51,52,52,44,49,50,53,46,57,51,51,32,51,52,51,46,51,54,52,55,53,44,49,50,53,32,51,52,50,46,52,57,53,51,53,
44,49,50,53,32,76,51,52,50,46,52,57,53,51,53,44,49,50,53,32,90,34,32,105,100,61,34,110,101,119,95,116,97,98,45,91,35,49,53,48,50,93,34,62,60,47,112,97,116,104,62,10,32,32,32,32,32,32,32,32,32,32,32,32,
60,47,103,62,10,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,60,47,103,62,10,60,47,115,118,103,62,0,0};

const char* VASTWaveTableEditorViewHeader::plus_svg = (const char*) resource_VASTWaveTableEditorViewHeader_plus_svg;
const int VASTWaveTableEditorViewHeader::plus_svgSize = 1650;

// JUCER_RESOURCE: ramp_svg, 3304, "../../Resources/Icons/ramp.svg"
static const unsigned char resource_VASTWaveTableEditorViewHeader_ramp_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,70,45,
56,34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,33,45,45,32,67,114,101,97,116,101,100,32,119,105,116,104,32,73,110,107,115,99,97,112,101,32,40,104,116,116,112,58,47,47,119,
119,119,46,105,110,107,115,99,97,112,101,46,111,114,103,47,41,32,45,45,62,10,10,60,115,118,103,10,32,32,32,120,109,108,110,115,58,100,99,61,34,104,116,116,112,58,47,47,112,117,114,108,46,111,114,103,47,
100,99,47,101,108,101,109,101,110,116,115,47,49,46,49,47,34,10,32,32,32,120,109,108,110,115,58,99,99,61,34,104,116,116,112,58,47,47,99,114,101,97,116,105,118,101,99,111,109,109,111,110,115,46,111,114,
103,47,110,115,35,34,10,32,32,32,120,109,108,110,115,58,114,100,102,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,48,50,47,50,50,45,114,100,102,45,115,121,110,116,
97,120,45,110,115,35,34,10,32,32,32,120,109,108,110,115,58,115,118,103,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,118,103,34,10,32,32,32,120,109,108,110,
115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,118,103,34,10,32,32,32,120,109,108,110,115,58,115,111,100,105,112,111,100,105,61,34,104,116,116,112,58,47,
47,115,111,100,105,112,111,100,105,46,115,111,117,114,99,101,102,111,114,103,101,46,110,101,116,47,68,84,68,47,115,111,100,105,112,111,100,105,45,48,46,100,116,100,34,10,32,32,32,120,109,108,110,115,58,
105,110,107,115,99,97,112,101,61,34,104,116,116,112,58,47,47,119,119,119,46,105,110,107,115,99,97,112,101,46,111,114,103,47,110,97,109,101,115,112,97,99,101,115,47,105,110,107,115,99,97,112,101,34,10,
32,32,32,119,105,100,116,104,61,34,50,49,48,109,109,34,10,32,32,32,104,101,105,103,104,116,61,34,50,57,55,109,109,34,10,32,32,32,118,105,101,119,66,111,120,61,34,48,32,48,32,50,49,48,32,50,57,55,34,10,
32,32,32,118,101,114,115,105,111,110,61,34,49,46,49,34,10,32,32,32,105,100,61,34,115,118,103,55,53,49,53,34,10,32,32,32,105,110,107,115,99,97,112,101,58,118,101,114,115,105,111,110,61,34,48,46,57,50,46,
51,32,40,50,52,48,53,53,52,54,44,32,50,48,49,56,45,48,51,45,49,49,41,34,10,32,32,32,115,111,100,105,112,111,100,105,58,100,111,99,110,97,109,101,61,34,114,97,109,112,46,115,118,103,34,62,10,32,32,60,100,
101,102,115,10,32,32,32,32,32,105,100,61,34,100,101,102,115,55,53,48,57,34,32,47,62,10,32,32,60,115,111,100,105,112,111,100,105,58,110,97,109,101,100,118,105,101,119,10,32,32,32,32,32,105,100,61,34,98,
97,115,101,34,10,32,32,32,32,32,112,97,103,101,99,111,108,111,114,61,34,35,102,102,102,102,102,102,34,10,32,32,32,32,32,98,111,114,100,101,114,99,111,108,111,114,61,34,35,54,54,54,54,54,54,34,10,32,32,
32,32,32,98,111,114,100,101,114,111,112,97,99,105,116,121,61,34,49,46,48,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,112,97,103,101,111,112,97,99,105,116,121,61,34,48,46,48,34,10,32,32,32,32,
32,105,110,107,115,99,97,112,101,58,112,97,103,101,115,104,97,100,111,119,61,34,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,122,111,111,109,61,34,49,46,52,34,10,32,32,32,32,32,105,110,107,
115,99,97,112,101,58,99,120,61,34,50,49,49,46,56,51,54,55,51,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,121,61,34,54,53,49,46,53,53,51,55,53,34,10,32,32,32,32,32,105,110,107,115,99,97,112,
101,58,100,111,99,117,109,101,110,116,45,117,110,105,116,115,61,34,109,109,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,117,114,114,101,110,116,45,108,97,121,101,114,61,34,108,97,121,101,114,
49,34,10,32,32,32,32,32,115,104,111,119,103,114,105,100,61,34,102,97,108,115,101,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,119,105,100,116,104,61,34,51,56,52,48,
34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,104,101,105,103,104,116,61,34,50,48,53,48,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,
45,120,61,34,45,49,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,121,61,34,45,49,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,
45,109,97,120,105,109,105,122,101,100,61,34,49,34,32,47,62,10,32,32,60,109,101,116,97,100,97,116,97,10,32,32,32,32,32,105,100,61,34,109,101,116,97,100,97,116,97,55,53,49,50,34,62,10,32,32,32,32,60,114,
100,102,58,82,68,70,62,10,32,32,32,32,32,32,60,99,99,58,87,111,114,107,10,32,32,32,32,32,32,32,32,32,114,100,102,58,97,98,111,117,116,61,34,34,62,10,32,32,32,32,32,32,32,32,60,100,99,58,102,111,114,109,
97,116,62,105,109,97,103,101,47,115,118,103,43,120,109,108,60,47,100,99,58,102,111,114,109,97,116,62,10,32,32,32,32,32,32,32,32,60,100,99,58,116,121,112,101,10,32,32,32,32,32,32,32,32,32,32,32,114,100,
102,58,114,101,115,111,117,114,99,101,61,34,104,116,116,112,58,47,47,112,117,114,108,46,111,114,103,47,100,99,47,100,99,109,105,116,121,112,101,47,83,116,105,108,108,73,109,97,103,101,34,32,47,62,10,32,
32,32,32,32,32,32,32,60,100,99,58,116,105,116,108,101,62,60,47,100,99,58,116,105,116,108,101,62,10,32,32,32,32,32,32,60,47,99,99,58,87,111,114,107,62,10,32,32,32,32,60,47,114,100,102,58,82,68,70,62,10,
32,32,60,47,109,101,116,97,100,97,116,97,62,10,32,32,60,103,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,108,97,98,101,108,61,34,69,98,101,110,101,32,49,34,10,32,32,32,32,32,105,110,107,115,99,97,
112,101,58,103,114,111,117,112,109,111,100,101,61,34,108,97,121,101,114,34,10,32,32,32,32,32,105,100,61,34,108,97,121,101,114,49,34,62,10,32,32,32,32,60,112,97,116,104,10,32,32,32,32,32,32,32,115,116,
121,108,101,61,34,99,111,108,111,114,58,35,48,48,48,48,48,48,59,102,111,110,116,45,115,116,121,108,101,58,110,111,114,109,97,108,59,102,111,110,116,45,118,97,114,105,97,110,116,58,110,111,114,109,97,108,
59,102,111,110,116,45,119,101,105,103,104,116,58,110,111,114,109,97,108,59,102,111,110,116,45,115,116,114,101,116,99,104,58,110,111,114,109,97,108,59,102,111,110,116,45,115,105,122,101,58,109,101,100,
105,117,109,59,108,105,110,101,45,104,101,105,103,104,116,58,110,111,114,109,97,108,59,102,111,110,116,45,102,97,109,105,108,121,58,115,97,110,115,45,115,101,114,105,102,59,102,111,110,116,45,118,97,114,
105,97,110,116,45,108,105,103,97,116,117,114,101,115,58,110,111,114,109,97,108,59,102,111,110,116,45,118,97,114,105,97,110,116,45,112,111,115,105,116,105,111,110,58,110,111,114,109,97,108,59,102,111,110,
116,45,118,97,114,105,97,110,116,45,99,97,112,115,58,110,111,114,109,97,108,59,102,111,110,116,45,118,97,114,105,97,110,116,45,110,117,109,101,114,105,99,58,110,111,114,109,97,108,59,102,111,110,116,45,
118,97,114,105,97,110,116,45,97,108,116,101,114,110,97,116,101,115,58,110,111,114,109,97,108,59,102,111,110,116,45,102,101,97,116,117,114,101,45,115,101,116,116,105,110,103,115,58,110,111,114,109,97,108,
59,116,101,120,116,45,105,110,100,101,110,116,58,48,59,116,101,120,116,45,97,108,105,103,110,58,115,116,97,114,116,59,116,101,120,116,45,100,101,99,111,114,97,116,105,111,110,58,110,111,110,101,59,116,
101,120,116,45,100,101,99,111,114,97,116,105,111,110,45,108,105,110,101,58,110,111,110,101,59,116,101,120,116,45,100,101,99,111,114,97,116,105,111,110,45,115,116,121,108,101,58,115,111,108,105,100,59,
116,101,120,116,45,100,101,99,111,114,97,116,105,111,110,45,99,111,108,111,114,58,35,48,48,48,48,48,48,59,108,101,116,116,101,114,45,115,112,97,99,105,110,103,58,110,111,114,109,97,108,59,119,111,114,
100,45,115,112,97,99,105,110,103,58,110,111,114,109,97,108,59,116,101,120,116,45,116,114,97,110,115,102,111,114,109,58,110,111,110,101,59,119,114,105,116,105,110,103,45,109,111,100,101,58,108,114,45,116,
98,59,100,105,114,101,99,116,105,111,110,58,108,116,114,59,116,101,120,116,45,111,114,105,101,110,116,97,116,105,111,110,58,109,105,120,101,100,59,100,111,109,105,110,97,110,116,45,98,97,115,101,108,105,
110,101,58,97,117,116,111,59,98,97,115,101,108,105,110,101,45,115,104,105,102,116,58,98,97,115,101,108,105,110,101,59,116,101,120,116,45,97,110,99,104,111,114,58,115,116,97,114,116,59,119,104,105,116,
101,45,115,112,97,99,101,58,110,111,114,109,97,108,59,115,104,97,112,101,45,112,97,100,100,105,110,103,58,48,59,99,108,105,112,45,114,117,108,101,58,110,111,110,122,101,114,111,59,100,105,115,112,108,
97,121,58,105,110,108,105,110,101,59,111,118,101,114,102,108,111,119,58,118,105,115,105,98,108,101,59,118,105,115,105,98,105,108,105,116,121,58,118,105,115,105,98,108,101,59,111,112,97,99,105,116,121,
58,49,59,105,115,111,108,97,116,105,111,110,58,97,117,116,111,59,109,105,120,45,98,108,101,110,100,45,109,111,100,101,58,110,111,114,109,97,108,59,99,111,108,111,114,45,105,110,116,101,114,112,111,108,
97,116,105,111,110,58,115,82,71,66,59,99,111,108,111,114,45,105,110,116,101,114,112,111,108,97,116,105,111,110,45,102,105,108,116,101,114,115,58,108,105,110,101,97,114,82,71,66,59,115,111,108,105,100,
45,99,111,108,111,114,58,35,48,48,48,48,48,48,59,115,111,108,105,100,45,111,112,97,99,105,116,121,58,49,59,118,101,99,116,111,114,45,101,102,102,101,99,116,58,110,111,110,101,59,102,105,108,108,58,35,
102,102,102,102,102,102,59,102,105,108,108,45,111,112,97,99,105,116,121,58,49,59,102,105,108,108,45,114,117,108,101,58,110,111,110,122,101,114,111,59,115,116,114,111,107,101,58,110,111,110,101,59,115,
116,114,111,107,101,45,119,105,100,116,104,58,48,46,50,54,52,53,56,51,51,50,112,120,59,115,116,114,111,107,101,45,108,105,110,101,99,97,112,58,98,117,116,116,59,115,116,114,111,107,101,45,108,105,110,
101,106,111,105,110,58,109,105,116,101,114,59,115,116,114,111,107,101,45,109,105,116,101,114,108,105,109,105,116,58,52,59,115,116,114,111,107,101,45,100,97,115,104,97,114,114,97,121,58,110,111,110,101,
59,115,116,114,111,107,101,45,100,97,115,104,111,102,102,115,101,116,58,48,59,115,116,114,111,107,101,45,111,112,97,99,105,116,121,58,49,59,99,111,108,111,114,45,114,101,110,100,101,114,105,110,103,58,
97,117,116,111,59,105,109,97,103,101,45,114,101,110,100,101,114,105,110,103,58,97,117,116,111,59,115,104,97,112,101,45,114,101,110,100,101,114,105,110,103,58,97,117,116,111,59,116,101,120,116,45,114,101,
110,100,101,114,105,110,103,58,97,117,116,111,59,101,110,97,98,108,101,45,98,97,99,107,103,114,111,117,110,100,58,97,99,99,117,109,117,108,97,116,101,34,10,32,32,32,32,32,32,32,100,61,34,109,32,49,54,
53,46,54,50,51,48,53,44,55,51,46,51,51,53,57,51,56,32,99,32,45,55,46,50,50,51,49,57,44,49,46,56,51,52,50,53,54,32,45,49,50,46,57,56,50,55,53,44,55,46,56,50,48,57,56,32,45,49,57,46,55,48,52,57,52,44,49,
49,46,48,51,54,57,53,52,32,67,32,49,48,48,46,49,52,57,54,54,44,49,49,50,46,51,48,50,50,55,32,53,52,46,54,52,53,51,51,54,44,49,52,48,46,54,54,52,55,51,32,56,46,57,51,57,52,53,51,49,44,49,54,56,46,55,48,
49,49,55,32,55,46,52,53,52,50,54,54,44,49,55,57,46,48,48,54,53,56,32,50,49,46,53,56,50,48,49,57,44,49,56,55,46,55,56,56,53,51,32,50,57,46,52,53,55,48,51,49,44,49,56,48,46,50,51,54,51,51,32,56,50,46,54,
57,53,52,50,57,44,49,52,54,46,53,56,54,48,56,32,49,51,54,46,54,56,54,50,57,44,49,49,52,46,49,48,54,53,55,32,49,57,48,46,49,54,55,57,55,44,56,48,46,56,49,50,53,32,99,32,45,53,46,48,56,56,51,54,44,45,49,
46,48,57,56,50,52,55,32,45,49,49,46,48,50,56,48,50,44,49,46,53,48,56,56,55,50,32,45,49,53,46,55,51,50,52,50,44,45,48,46,55,55,55,51,52,52,32,45,49,46,52,51,50,49,57,44,45,51,46,53,56,50,53,51,50,32,45,
52,46,55,48,50,57,53,44,45,54,46,56,48,56,54,49,49,32,45,56,46,56,49,50,53,44,45,54,46,54,57,57,50,49,56,32,122,34,10,32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,56,48,54,48,34,10,32,32,32,32,32,
32,32,105,110,107,115,99,97,112,101,58,99,111,110,110,101,99,116,111,114,45,99,117,114,118,97,116,117,114,101,61,34,48,34,32,47,62,10,32,32,60,47,103,62,10,60,47,115,118,103,62,10,0,0};

const char* VASTWaveTableEditorViewHeader::ramp_svg = (const char*) resource_VASTWaveTableEditorViewHeader_ramp_svg;
const int VASTWaveTableEditorViewHeader::ramp_svgSize = 3304;

// JUCER_RESOURCE: ramp_connect_svg, 2958, "../../Resources/Icons/ramp_connect.svg"
static const unsigned char resource_VASTWaveTableEditorViewHeader_ramp_connect_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,
70,45,56,34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,10,32,32,32,120,109,108,110,115,58,100,99,61,34,104,116,116,112,58,47,47,112,117,114,108,46,111,114,103,47,
100,99,47,101,108,101,109,101,110,116,115,47,49,46,49,47,34,10,32,32,32,120,109,108,110,115,58,99,99,61,34,104,116,116,112,58,47,47,99,114,101,97,116,105,118,101,99,111,109,109,111,110,115,46,111,114,
103,47,110,115,35,34,10,32,32,32,120,109,108,110,115,58,114,100,102,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,48,50,47,50,50,45,114,100,102,45,115,121,110,116,
97,120,45,110,115,35,34,10,32,32,32,120,109,108,110,115,58,115,118,103,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,118,103,34,10,32,32,32,120,109,108,110,
115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,118,103,34,10,32,32,32,120,109,108,110,115,58,115,111,100,105,112,111,100,105,61,34,104,116,116,112,58,47,
47,115,111,100,105,112,111,100,105,46,115,111,117,114,99,101,102,111,114,103,101,46,110,101,116,47,68,84,68,47,115,111,100,105,112,111,100,105,45,48,46,100,116,100,34,10,32,32,32,120,109,108,110,115,58,
105,110,107,115,99,97,112,101,61,34,104,116,116,112,58,47,47,119,119,119,46,105,110,107,115,99,97,112,101,46,111,114,103,47,110,97,109,101,115,112,97,99,101,115,47,105,110,107,115,99,97,112,101,34,10,
32,32,32,119,105,100,116,104,61,34,50,48,112,120,34,10,32,32,32,104,101,105,103,104,116,61,34,50,48,112,120,34,10,32,32,32,118,105,101,119,66,111,120,61,34,48,32,48,32,50,48,32,50,48,34,10,32,32,32,118,
101,114,115,105,111,110,61,34,49,46,49,34,10,32,32,32,105,100,61,34,115,118,103,49,50,34,10,32,32,32,115,111,100,105,112,111,100,105,58,100,111,99,110,97,109,101,61,34,114,97,109,112,95,99,111,110,110,
101,99,116,46,115,118,103,34,10,32,32,32,105,110,107,115,99,97,112,101,58,118,101,114,115,105,111,110,61,34,48,46,57,50,46,51,32,40,50,52,48,53,53,52,54,44,32,50,48,49,56,45,48,51,45,49,49,41,34,62,10,
32,32,60,109,101,116,97,100,97,116,97,10,32,32,32,32,32,105,100,61,34,109,101,116,97,100,97,116,97,49,54,34,62,10,32,32,32,32,60,114,100,102,58,82,68,70,62,10,32,32,32,32,32,32,60,99,99,58,87,111,114,
107,10,32,32,32,32,32,32,32,32,32,114,100,102,58,97,98,111,117,116,61,34,34,62,10,32,32,32,32,32,32,32,32,60,100,99,58,102,111,114,109,97,116,62,105,109,97,103,101,47,115,118,103,43,120,109,108,60,47,
100,99,58,102,111,114,109,97,116,62,10,32,32,32,32,32,32,32,32,60,100,99,58,116,121,112,101,10,32,32,32,32,32,32,32,32,32,32,32,114,100,102,58,114,101,115,111,117,114,99,101,61,34,104,116,116,112,58,47,
47,112,117,114,108,46,111,114,103,47,100,99,47,100,99,109,105,116,121,112,101,47,83,116,105,108,108,73,109,97,103,101,34,32,47,62,10,32,32,32,32,32,32,32,32,60,100,99,58,116,105,116,108,101,62,115,116,
97,116,115,32,91,35,49,49,57,48,93,60,47,100,99,58,116,105,116,108,101,62,10,32,32,32,32,32,32,60,47,99,99,58,87,111,114,107,62,10,32,32,32,32,60,47,114,100,102,58,82,68,70,62,10,32,32,60,47,109,101,116,
97,100,97,116,97,62,10,32,32,60,115,111,100,105,112,111,100,105,58,110,97,109,101,100,118,105,101,119,10,32,32,32,32,32,112,97,103,101,99,111,108,111,114,61,34,35,57,53,57,53,57,53,34,10,32,32,32,32,32,
98,111,114,100,101,114,99,111,108,111,114,61,34,35,54,54,54,54,54,54,34,10,32,32,32,32,32,98,111,114,100,101,114,111,112,97,99,105,116,121,61,34,49,34,10,32,32,32,32,32,111,98,106,101,99,116,116,111,108,
101,114,97,110,99,101,61,34,49,48,34,10,32,32,32,32,32,103,114,105,100,116,111,108,101,114,97,110,99,101,61,34,49,48,34,10,32,32,32,32,32,103,117,105,100,101,116,111,108,101,114,97,110,99,101,61,34,49,
48,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,112,97,103,101,111,112,97,99,105,116,121,61,34,49,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,112,97,103,101,115,104,97,100,111,119,61,
34,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,119,105,100,116,104,61,34,51,56,52,48,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,
45,104,101,105,103,104,116,61,34,50,48,53,48,34,10,32,32,32,32,32,105,100,61,34,110,97,109,101,100,118,105,101,119,49,52,34,10,32,32,32,32,32,115,104,111,119,103,114,105,100,61,34,102,97,108,115,101,34,
10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,122,111,111,109,61,34,54,54,46,55,53,48,56,56,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,120,61,34,56,46,52,53,50,56,50,52,53,34,10,32,
32,32,32,32,105,110,107,115,99,97,112,101,58,99,121,61,34,55,46,52,48,51,56,54,49,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,120,61,34,45,49,50,34,10,32,32,32,
32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,121,61,34,45,49,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,109,97,120,105,109,105,122,101,100,
61,34,49,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,117,114,114,101,110,116,45,108,97,121,101,114,61,34,115,118,103,49,50,34,10,32,32,32,32,32,115,104,111,119,103,117,105,100,101,115,61,
34,102,97,108,115,101,34,32,47,62,10,32,32,60,33,45,45,32,71,101,110,101,114,97,116,111,114,58,32,83,107,101,116,99,104,32,51,46,56,46,49,32,40,50,57,54,56,55,41,32,45,32,104,116,116,112,58,47,47,119,
119,119,46,98,111,104,101,109,105,97,110,99,111,100,105,110,103,46,99,111,109,47,115,107,101,116,99,104,32,45,45,62,10,32,32,60,116,105,116,108,101,10,32,32,32,32,32,105,100,61,34,116,105,116,108,101,
50,34,62,115,116,97,116,115,32,91,35,49,49,57,48,93,60,47,116,105,116,108,101,62,10,32,32,60,100,101,115,99,10,32,32,32,32,32,105,100,61,34,100,101,115,99,52,34,62,67,114,101,97,116,101,100,32,119,105,
116,104,32,83,107,101,116,99,104,46,60,47,100,101,115,99,62,10,32,32,60,100,101,102,115,10,32,32,32,32,32,105,100,61,34,100,101,102,115,54,34,32,47,62,10,32,32,60,103,10,32,32,32,32,32,105,100,61,34,103,
56,50,56,34,10,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,109,97,116,114,105,120,40,48,46,56,52,50,57,49,51,54,49,44,48,44,48,44,48,46,55,52,54,54,56,49,56,52,44,45,48,46,49,56,55,55,48,48,
52,52,44,51,46,51,56,49,55,49,51,52,41,34,10,32,32,32,32,32,115,116,121,108,101,61,34,115,116,114,111,107,101,45,119,105,100,116,104,58,49,46,50,54,48,52,57,51,55,54,34,62,10,32,32,32,32,60,101,108,108,
105,112,115,101,10,32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,56,49,57,34,10,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,102,102,102,102,102,102,59,102,105,108,108,45,111,
112,97,99,105,116,121,58,48,46,48,54,53,48,52,48,54,56,59,115,116,114,111,107,101,58,35,102,102,102,102,102,102,59,115,116,114,111,107,101,45,119,105,100,116,104,58,49,46,50,54,48,52,57,51,55,54,59,115,
116,114,111,107,101,45,108,105,110,101,99,97,112,58,114,111,117,110,100,59,115,116,114,111,107,101,45,108,105,110,101,106,111,105,110,58,114,111,117,110,100,59,115,116,114,111,107,101,45,109,105,116,101,
114,108,105,109,105,116,58,52,59,115,116,114,111,107,101,45,100,97,115,104,97,114,114,97,121,58,110,111,110,101,59,115,116,114,111,107,101,45,100,97,115,104,111,102,102,115,101,116,58,50,46,50,48,48,48,
48,48,48,53,59,115,116,114,111,107,101,45,111,112,97,99,105,116,121,58,49,34,10,32,32,32,32,32,32,32,114,121,61,34,50,46,50,57,57,53,57,53,49,34,10,32,32,32,32,32,32,32,114,120,61,34,50,46,50,54,50,49,
52,50,52,34,10,32,32,32,32,32,32,32,99,121,61,34,49,56,46,53,50,52,51,54,52,34,10,32,32,32,32,32,32,32,99,120,61,34,51,46,57,53,53,48,48,52,34,32,47,62,10,32,32,60,47,103,62,10,32,32,60,103,10,32,32,32,
32,32,105,100,61,34,103,56,50,56,45,54,34,10,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,109,97,116,114,105,120,40,48,46,57,48,55,55,49,55,54,56,44,48,44,48,44,48,46,55,52,54,54,56,49,56,52,
44,49,51,46,54,56,51,49,53,51,44,45,49,49,46,52,57,52,52,57,53,41,34,10,32,32,32,32,32,115,116,121,108,101,61,34,115,116,114,111,107,101,45,119,105,100,116,104,58,49,46,50,49,52,54,54,53,55,55,34,62,10,
32,32,32,32,60,101,108,108,105,112,115,101,10,32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,56,49,57,45,49,34,10,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,102,102,102,102,
102,102,59,102,105,108,108,45,111,112,97,99,105,116,121,58,48,46,48,54,53,48,52,48,54,56,59,115,116,114,111,107,101,58,35,102,102,102,102,102,102,59,115,116,114,111,107,101,45,119,105,100,116,104,58,49,
46,50,49,52,54,54,53,55,55,59,115,116,114,111,107,101,45,108,105,110,101,99,97,112,58,114,111,117,110,100,59,115,116,114,111,107,101,45,108,105,110,101,106,111,105,110,58,114,111,117,110,100,59,115,116,
114,111,107,101,45,109,105,116,101,114,108,105,109,105,116,58,52,59,115,116,114,111,107,101,45,100,97,115,104,97,114,114,97,121,58,110,111,110,101,59,115,116,114,111,107,101,45,100,97,115,104,111,102,
102,115,101,116,58,50,46,50,48,48,48,48,48,48,53,59,115,116,114,111,107,101,45,111,112,97,99,105,116,121,58,49,34,10,32,32,32,32,32,32,32,114,121,61,34,50,46,50,57,57,53,57,53,49,34,10,32,32,32,32,32,
32,32,114,120,61,34,50,46,50,54,50,49,52,50,52,34,10,32,32,32,32,32,32,32,99,121,61,34,49,56,46,53,50,52,51,54,52,34,10,32,32,32,32,32,32,32,99,120,61,34,51,46,57,53,53,48,48,52,34,32,47,62,10,32,32,60,
47,103,62,10,32,32,60,112,97,116,104,10,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,110,111,110,101,59,115,116,114,111,107,101,58,35,102,102,102,102,102,102,59,115,116,114,111,107,101,
45,119,105,100,116,104,58,49,46,53,59,115,116,114,111,107,101,45,108,105,110,101,99,97,112,58,98,117,116,116,59,115,116,114,111,107,101,45,108,105,110,101,106,111,105,110,58,109,105,116,101,114,59,115,
116,114,111,107,101,45,111,112,97,99,105,116,121,58,49,59,115,116,114,111,107,101,45,109,105,116,101,114,108,105,109,105,116,58,52,59,115,116,114,111,107,101,45,100,97,115,104,97,114,114,97,121,58,110,
111,110,101,34,10,32,32,32,32,32,100,61,34,77,32,53,46,51,48,51,51,48,48,57,44,49,52,46,57,53,49,51,55,55,32,49,53,46,48,52,49,44,52,46,57,55,51,57,56,48,57,32,118,32,48,34,10,32,32,32,32,32,105,100,61,
34,112,97,116,104,52,53,53,53,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,111,110,110,101,99,116,111,114,45,99,117,114,118,97,116,117,114,101,61,34,48,34,32,47,62,10,60,47,115,118,103,62,
10,0,0};

const char* VASTWaveTableEditorViewHeader::ramp_connect_svg = (const char*) resource_VASTWaveTableEditorViewHeader_ramp_connect_svg;
const int VASTWaveTableEditorViewHeader::ramp_connect_svgSize = 2958;

// JUCER_RESOURCE: rampconnect_svg, 6750, "../../Resources/Icons/rampconnect.svg"
static const unsigned char resource_VASTWaveTableEditorViewHeader_rampconnect_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,
70,45,56,34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,33,45,45,32,67,114,101,97,116,101,100,32,119,105,116,104,32,73,110,107,115,99,97,112,101,32,40,104,116,116,112,58,47,47,
119,119,119,46,105,110,107,115,99,97,112,101,46,111,114,103,47,41,32,45,45,62,10,10,60,115,118,103,10,32,32,32,120,109,108,110,115,58,100,99,61,34,104,116,116,112,58,47,47,112,117,114,108,46,111,114,103,
47,100,99,47,101,108,101,109,101,110,116,115,47,49,46,49,47,34,10,32,32,32,120,109,108,110,115,58,99,99,61,34,104,116,116,112,58,47,47,99,114,101,97,116,105,118,101,99,111,109,109,111,110,115,46,111,114,
103,47,110,115,35,34,10,32,32,32,120,109,108,110,115,58,114,100,102,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,48,50,47,50,50,45,114,100,102,45,115,121,110,116,
97,120,45,110,115,35,34,10,32,32,32,120,109,108,110,115,58,115,118,103,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,118,103,34,10,32,32,32,120,109,108,110,
115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,118,103,34,10,32,32,32,120,109,108,110,115,58,115,111,100,105,112,111,100,105,61,34,104,116,116,112,58,47,
47,115,111,100,105,112,111,100,105,46,115,111,117,114,99,101,102,111,114,103,101,46,110,101,116,47,68,84,68,47,115,111,100,105,112,111,100,105,45,48,46,100,116,100,34,10,32,32,32,120,109,108,110,115,58,
105,110,107,115,99,97,112,101,61,34,104,116,116,112,58,47,47,119,119,119,46,105,110,107,115,99,97,112,101,46,111,114,103,47,110,97,109,101,115,112,97,99,101,115,47,105,110,107,115,99,97,112,101,34,10,
32,32,32,119,105,100,116,104,61,34,50,49,48,109,109,34,10,32,32,32,104,101,105,103,104,116,61,34,50,57,55,109,109,34,10,32,32,32,118,105,101,119,66,111,120,61,34,48,32,48,32,50,49,48,32,50,57,55,34,10,
32,32,32,118,101,114,115,105,111,110,61,34,49,46,49,34,10,32,32,32,105,100,61,34,115,118,103,56,34,10,32,32,32,105,110,107,115,99,97,112,101,58,118,101,114,115,105,111,110,61,34,48,46,57,50,46,51,32,40,
50,52,48,53,53,52,54,44,32,50,48,49,56,45,48,51,45,49,49,41,34,10,32,32,32,115,111,100,105,112,111,100,105,58,100,111,99,110,97,109,101,61,34,114,97,109,112,99,111,110,110,101,99,116,46,115,118,103,34,
62,10,32,32,60,100,101,102,115,10,32,32,32,32,32,105,100,61,34,100,101,102,115,50,34,62,10,32,32,32,32,60,109,97,114,107,101,114,10,32,32,32,32,32,32,32,105,110,107,115,99,97,112,101,58,105,115,115,116,
111,99,107,61,34,116,114,117,101,34,10,32,32,32,32,32,32,32,115,116,121,108,101,61,34,111,118,101,114,102,108,111,119,58,118,105,115,105,98,108,101,34,10,32,32,32,32,32,32,32,105,100,61,34,109,97,114,
107,101,114,49,49,54,49,34,10,32,32,32,32,32,32,32,114,101,102,88,61,34,48,46,48,34,10,32,32,32,32,32,32,32,114,101,102,89,61,34,48,46,48,34,10,32,32,32,32,32,32,32,111,114,105,101,110,116,61,34,97,117,
116,111,34,10,32,32,32,32,32,32,32,105,110,107,115,99,97,112,101,58,115,116,111,99,107,105,100,61,34,65,114,114,111,119,49,76,115,116,97,114,116,34,62,10,32,32,32,32,32,32,60,112,97,116,104,10,32,32,32,
32,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,115,99,97,108,101,40,48,46,56,41,32,116,114,97,110,115,108,97,116,101,40,49,50,46,53,44,48,41,34,10,32,32,32,32,32,32,32,32,32,115,116,121,108,
101,61,34,102,105,108,108,45,114,117,108,101,58,101,118,101,110,111,100,100,59,115,116,114,111,107,101,58,35,49,57,49,57,49,57,59,115,116,114,111,107,101,45,119,105,100,116,104,58,49,112,116,59,115,116,
114,111,107,101,45,111,112,97,99,105,116,121,58,49,59,102,105,108,108,58,35,49,57,49,57,49,57,59,102,105,108,108,45,111,112,97,99,105,116,121,58,49,34,10,32,32,32,32,32,32,32,32,32,100,61,34,77,32,48,
46,48,44,48,46,48,32,76,32,53,46,48,44,45,53,46,48,32,76,32,45,49,50,46,53,44,48,46,48,32,76,32,53,46,48,44,53,46,48,32,76,32,48,46,48,44,48,46,48,32,122,32,34,10,32,32,32,32,32,32,32,32,32,105,100,61,
34,112,97,116,104,49,49,53,57,34,32,47,62,10,32,32,32,32,60,47,109,97,114,107,101,114,62,10,32,32,32,32,60,109,97,114,107,101,114,10,32,32,32,32,32,32,32,105,110,107,115,99,97,112,101,58,115,116,111,99,
107,105,100,61,34,65,114,114,111,119,49,76,115,116,97,114,116,34,10,32,32,32,32,32,32,32,111,114,105,101,110,116,61,34,97,117,116,111,34,10,32,32,32,32,32,32,32,114,101,102,89,61,34,48,46,48,34,10,32,
32,32,32,32,32,32,114,101,102,88,61,34,48,46,48,34,10,32,32,32,32,32,32,32,105,100,61,34,65,114,114,111,119,49,76,115,116,97,114,116,34,10,32,32,32,32,32,32,32,115,116,121,108,101,61,34,111,118,101,114,
102,108,111,119,58,118,105,115,105,98,108,101,34,10,32,32,32,32,32,32,32,105,110,107,115,99,97,112,101,58,105,115,115,116,111,99,107,61,34,116,114,117,101,34,10,32,32,32,32,32,32,32,105,110,107,115,99,
97,112,101,58,99,111,108,108,101,99,116,61,34,97,108,119,97,121,115,34,62,10,32,32,32,32,32,32,60,112,97,116,104,10,32,32,32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,56,53,49,34,10,32,32,32,32,32,
32,32,32,32,100,61,34,77,32,48,46,48,44,48,46,48,32,76,32,53,46,48,44,45,53,46,48,32,76,32,45,49,50,46,53,44,48,46,48,32,76,32,53,46,48,44,53,46,48,32,76,32,48,46,48,44,48,46,48,32,122,32,34,10,32,32,
32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,45,114,117,108,101,58,101,118,101,110,111,100,100,59,115,116,114,111,107,101,58,35,49,57,49,57,49,57,59,115,116,114,111,107,101,45,119,105,
100,116,104,58,49,112,116,59,115,116,114,111,107,101,45,111,112,97,99,105,116,121,58,49,59,102,105,108,108,58,35,49,57,49,57,49,57,59,102,105,108,108,45,111,112,97,99,105,116,121,58,49,34,10,32,32,32,
32,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,115,99,97,108,101,40,48,46,56,41,32,116,114,97,110,115,108,97,116,101,40,49,50,46,53,44,48,41,34,32,47,62,10,32,32,32,32,60,47,109,97,114,107,
101,114,62,10,32,32,32,32,60,102,105,108,116,101,114,10,32,32,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,111,108,108,101,99,116,61,34,97,108,119,97,121,115,34,10,32,32,32,32,32,32,32,115,116,121,
108,101,61,34,99,111,108,111,114,45,105,110,116,101,114,112,111,108,97,116,105,111,110,45,102,105,108,116,101,114,115,58,115,82,71,66,34,10,32,32,32,32,32,32,32,105,100,61,34,102,105,108,116,101,114,49,
50,53,55,34,10,32,32,32,32,32,32,32,120,61,34,45,49,46,49,52,56,51,57,50,56,34,10,32,32,32,32,32,32,32,119,105,100,116,104,61,34,51,46,50,57,54,55,56,53,54,34,10,32,32,32,32,32,32,32,121,61,34,45,49,46,
51,51,52,54,50,50,51,34,10,32,32,32,32,32,32,32,104,101,105,103,104,116,61,34,51,46,54,54,57,50,52,52,55,34,62,10,32,32,32,32,32,32,60,102,101,71,97,117,115,115,105,97,110,66,108,117,114,10,32,32,32,32,
32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,111,108,108,101,99,116,61,34,97,108,119,97,121,115,34,10,32,32,32,32,32,32,32,32,32,115,116,100,68,101,118,105,97,116,105,111,110,61,34,51,52,46,57,57,
54,54,51,57,34,10,32,32,32,32,32,32,32,32,32,105,100,61,34,102,101,71,97,117,115,115,105,97,110,66,108,117,114,49,50,53,57,34,32,47,62,10,32,32,32,32,60,47,102,105,108,116,101,114,62,10,32,32,32,32,60,
102,105,108,116,101,114,10,32,32,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,111,108,108,101,99,116,61,34,97,108,119,97,121,115,34,10,32,32,32,32,32,32,32,115,116,121,108,101,61,34,99,111,108,111,
114,45,105,110,116,101,114,112,111,108,97,116,105,111,110,45,102,105,108,116,101,114,115,58,115,82,71,66,34,10,32,32,32,32,32,32,32,105,100,61,34,102,105,108,116,101,114,49,50,54,49,34,10,32,32,32,32,
32,32,32,120,61,34,45,48,46,53,54,49,56,53,56,49,49,34,10,32,32,32,32,32,32,32,119,105,100,116,104,61,34,50,46,49,50,51,55,49,54,50,34,10,32,32,32,32,32,32,32,121,61,34,45,49,46,51,51,52,54,50,50,51,34,
10,32,32,32,32,32,32,32,104,101,105,103,104,116,61,34,51,46,54,54,57,50,52,52,54,34,62,10,32,32,32,32,32,32,60,102,101,71,97,117,115,115,105,97,110,66,108,117,114,10,32,32,32,32,32,32,32,32,32,105,110,
107,115,99,97,112,101,58,99,111,108,108,101,99,116,61,34,97,108,119,97,121,115,34,10,32,32,32,32,32,32,32,32,32,115,116,100,68,101,118,105,97,116,105,111,110,61,34,49,51,50,46,50,55,48,55,54,34,10,32,
32,32,32,32,32,32,32,32,105,100,61,34,102,101,71,97,117,115,115,105,97,110,66,108,117,114,49,50,54,51,34,32,47,62,10,32,32,32,32,60,47,102,105,108,116,101,114,62,10,32,32,60,47,100,101,102,115,62,10,32,
32,60,115,111,100,105,112,111,100,105,58,110,97,109,101,100,118,105,101,119,10,32,32,32,32,32,105,100,61,34,98,97,115,101,34,10,32,32,32,32,32,112,97,103,101,99,111,108,111,114,61,34,35,102,102,102,102,
102,102,34,10,32,32,32,32,32,98,111,114,100,101,114,99,111,108,111,114,61,34,35,54,54,54,54,54,54,34,10,32,32,32,32,32,98,111,114,100,101,114,111,112,97,99,105,116,121,61,34,49,46,48,34,10,32,32,32,32,
32,105,110,107,115,99,97,112,101,58,112,97,103,101,111,112,97,99,105,116,121,61,34,48,46,48,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,112,97,103,101,115,104,97,100,111,119,61,34,50,34,10,32,
32,32,32,32,105,110,107,115,99,97,112,101,58,122,111,111,109,61,34,49,46,52,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,120,61,34,50,57,53,46,53,56,52,57,51,34,10,32,32,32,32,32,105,110,107,
115,99,97,112,101,58,99,121,61,34,53,50,55,46,56,56,49,52,49,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,100,111,99,117,109,101,110,116,45,117,110,105,116,115,61,34,109,109,34,10,32,32,32,32,
32,105,110,107,115,99,97,112,101,58,99,117,114,114,101,110,116,45,108,97,121,101,114,61,34,108,97,121,101,114,49,34,10,32,32,32,32,32,115,104,111,119,103,114,105,100,61,34,102,97,108,115,101,34,10,32,
32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,119,105,100,116,104,61,34,51,56,52,48,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,104,101,105,
103,104,116,61,34,50,48,53,48,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,120,61,34,45,49,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,
111,119,45,121,61,34,45,49,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,109,97,120,105,109,105,122,101,100,61,34,49,34,32,47,62,10,32,32,60,109,101,116,97,100,97,
116,97,10,32,32,32,32,32,105,100,61,34,109,101,116,97,100,97,116,97,53,34,62,10,32,32,32,32,60,114,100,102,58,82,68,70,62,10,32,32,32,32,32,32,60,99,99,58,87,111,114,107,10,32,32,32,32,32,32,32,32,32,
114,100,102,58,97,98,111,117,116,61,34,34,62,10,32,32,32,32,32,32,32,32,60,100,99,58,102,111,114,109,97,116,62,105,109,97,103,101,47,115,118,103,43,120,109,108,60,47,100,99,58,102,111,114,109,97,116,62,
10,32,32,32,32,32,32,32,32,60,100,99,58,116,121,112,101,10,32,32,32,32,32,32,32,32,32,32,32,114,100,102,58,114,101,115,111,117,114,99,101,61,34,104,116,116,112,58,47,47,112,117,114,108,46,111,114,103,
47,100,99,47,100,99,109,105,116,121,112,101,47,83,116,105,108,108,73,109,97,103,101,34,32,47,62,10,32,32,32,32,32,32,32,32,60,100,99,58,116,105,116,108,101,62,60,47,100,99,58,116,105,116,108,101,62,10,
32,32,32,32,32,32,60,47,99,99,58,87,111,114,107,62,10,32,32,32,32,60,47,114,100,102,58,82,68,70,62,10,32,32,60,47,109,101,116,97,100,97,116,97,62,10,32,32,60,103,10,32,32,32,32,32,105,110,107,115,99,97,
112,101,58,108,97,98,101,108,61,34,69,98,101,110,101,32,49,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,103,114,111,117,112,109,111,100,101,61,34,108,97,121,101,114,34,10,32,32,32,32,32,105,100,
61,34,108,97,121,101,114,49,34,62,10,32,32,32,32,60,112,97,116,104,10,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,110,111,110,101,59,115,116,114,111,107,101,58,35,49,57,49,57,49,
57,59,115,116,114,111,107,101,45,119,105,100,116,104,58,48,46,48,49,48,48,49,53,55,53,59,115,116,114,111,107,101,45,108,105,110,101,99,97,112,58,98,117,116,116,59,115,116,114,111,107,101,45,108,105,110,
101,106,111,105,110,58,109,105,116,101,114,59,115,116,114,111,107,101,45,111,112,97,99,105,116,121,58,49,59,115,116,114,111,107,101,45,109,105,116,101,114,108,105,109,105,116,58,52,59,115,116,114,111,
107,101,45,100,97,115,104,97,114,114,97,121,58,48,46,48,49,48,48,49,53,55,53,44,32,48,46,48,49,48,48,49,53,55,53,48,48,48,48,48,48,48,48,48,59,115,116,114,111,107,101,45,100,97,115,104,111,102,102,115,
101,116,58,48,46,49,48,48,49,53,55,53,59,109,97,114,107,101,114,45,115,116,97,114,116,58,117,114,108,40,35,65,114,114,111,119,49,76,115,116,97,114,116,41,59,102,105,108,116,101,114,58,117,114,108,40,35,
102,105,108,116,101,114,49,50,54,49,41,34,10,32,32,32,32,32,32,32,100,61,34,77,32,52,54,50,46,56,53,55,52,50,32,52,53,48,46,51,55,54,57,53,32,76,32,54,49,52,46,50,56,53,49,54,32,52,53,48,46,51,55,54,57,
53,32,76,32,52,54,50,46,56,53,55,52,50,32,52,53,48,46,51,55,54,57,53,32,122,32,77,32,52,57,46,50,56,53,49,53,54,32,54,56,56,46,50,51,52,51,56,32,76,32,49,56,54,46,52,50,55,55,51,32,54,56,56,46,50,51,52,
51,56,32,76,32,52,57,46,50,56,53,49,53,54,32,54,56,56,46,50,51,52,51,56,32,122,32,34,10,32,32,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,115,99,97,108,101,40,48,46,50,54,52,53,56,51,51,51,
41,34,10,32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,56,49,53,34,32,47,62,10,32,32,32,32,60,112,97,116,104,10,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,110,111,110,101,59,
115,116,114,111,107,101,58,35,49,57,49,57,49,57,59,115,116,114,111,107,101,45,119,105,100,116,104,58,48,59,115,116,114,111,107,101,45,108,105,110,101,99,97,112,58,98,117,116,116,59,115,116,114,111,107,
101,45,108,105,110,101,106,111,105,110,58,109,105,116,101,114,59,115,116,114,111,107,101,45,111,112,97,99,105,116,121,58,49,59,115,116,114,111,107,101,45,109,105,116,101,114,108,105,109,105,116,58,52,
59,115,116,114,111,107,101,45,100,97,115,104,97,114,114,97,121,58,48,44,32,48,59,115,116,114,111,107,101,45,100,97,115,104,111,102,102,115,101,116,58,48,59,109,97,114,107,101,114,45,115,116,97,114,116,
58,117,114,108,40,35,109,97,114,107,101,114,49,49,54,49,41,59,102,105,108,116,101,114,58,117,114,108,40,35,102,105,108,116,101,114,49,50,53,55,41,34,10,32,32,32,32,32,32,32,100,61,34,109,32,52,57,46,51,
50,53,54,55,44,49,56,50,46,48,57,53,51,52,32,99,32,55,51,46,49,51,56,54,56,44,45,54,50,46,57,51,51,49,49,32,55,51,46,49,51,56,54,56,44,45,54,50,46,57,51,51,49,49,32,55,51,46,49,51,56,54,56,44,45,54,50,
46,57,51,51,49,49,34,10,32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,56,51,54,34,10,32,32,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,111,110,110,101,99,116,111,114,45,99,117,114,118,97,116,
117,114,101,61,34,48,34,32,47,62,10,32,32,32,32,60,112,97,116,104,10,32,32,32,32,32,32,32,115,116,121,108,101,61,34,99,111,108,111,114,58,35,48,48,48,48,48,48,59,102,111,110,116,45,115,116,121,108,101,
58,110,111,114,109,97,108,59,102,111,110,116,45,118,97,114,105,97,110,116,58,110,111,114,109,97,108,59,102,111,110,116,45,119,101,105,103,104,116,58,110,111,114,109,97,108,59,102,111,110,116,45,115,116,
114,101,116,99,104,58,110,111,114,109,97,108,59,102,111,110,116,45,115,105,122,101,58,109,101,100,105,117,109,59,108,105,110,101,45,104,101,105,103,104,116,58,110,111,114,109,97,108,59,102,111,110,116,
45,102,97,109,105,108,121,58,115,97,110,115,45,115,101,114,105,102,59,102,111,110,116,45,118,97,114,105,97,110,116,45,108,105,103,97,116,117,114,101,115,58,110,111,114,109,97,108,59,102,111,110,116,45,
118,97,114,105,97,110,116,45,112,111,115,105,116,105,111,110,58,110,111,114,109,97,108,59,102,111,110,116,45,118,97,114,105,97,110,116,45,99,97,112,115,58,110,111,114,109,97,108,59,102,111,110,116,45,
118,97,114,105,97,110,116,45,110,117,109,101,114,105,99,58,110,111,114,109,97,108,59,102,111,110,116,45,118,97,114,105,97,110,116,45,97,108,116,101,114,110,97,116,101,115,58,110,111,114,109,97,108,59,
102,111,110,116,45,102,101,97,116,117,114,101,45,115,101,116,116,105,110,103,115,58,110,111,114,109,97,108,59,116,101,120,116,45,105,110,100,101,110,116,58,48,59,116,101,120,116,45,97,108,105,103,110,
58,115,116,97,114,116,59,116,101,120,116,45,100,101,99,111,114,97,116,105,111,110,58,110,111,110,101,59,116,101,120,116,45,100,101,99,111,114,97,116,105,111,110,45,108,105,110,101,58,110,111,110,101,59,
116,101,120,116,45,100,101,99,111,114,97,116,105,111,110,45,115,116,121,108,101,58,115,111,108,105,100,59,116,101,120,116,45,100,101,99,111,114,97,116,105,111,110,45,99,111,108,111,114,58,35,48,48,48,
48,48,48,59,108,101,116,116,101,114,45,115,112,97,99,105,110,103,58,110,111,114,109,97,108,59,119,111,114,100,45,115,112,97,99,105,110,103,58,110,111,114,109,97,108,59,116,101,120,116,45,116,114,97,110,
115,102,111,114,109,58,110,111,110,101,59,119,114,105,116,105,110,103,45,109,111,100,101,58,108,114,45,116,98,59,100,105,114,101,99,116,105,111,110,58,108,116,114,59,116,101,120,116,45,111,114,105,101,
110,116,97,116,105,111,110,58,109,105,120,101,100,59,100,111,109,105,110,97,110,116,45,98,97,115,101,108,105,110,101,58,97,117,116,111,59,98,97,115,101,108,105,110,101,45,115,104,105,102,116,58,98,97,
115,101,108,105,110,101,59,116,101,120,116,45,97,110,99,104,111,114,58,115,116,97,114,116,59,119,104,105,116,101,45,115,112,97,99,101,58,110,111,114,109,97,108,59,115,104,97,112,101,45,112,97,100,100,
105,110,103,58,48,59,99,108,105,112,45,114,117,108,101,58,110,111,110,122,101,114,111,59,100,105,115,112,108,97,121,58,105,110,108,105,110,101,59,111,118,101,114,102,108,111,119,58,118,105,115,105,98,
108,101,59,118,105,115,105,98,105,108,105,116,121,58,118,105,115,105,98,108,101,59,111,112,97,99,105,116,121,58,49,59,105,115,111,108,97,116,105,111,110,58,97,117,116,111,59,109,105,120,45,98,108,101,
110,100,45,109,111,100,101,58,110,111,114,109,97,108,59,99,111,108,111,114,45,105,110,116,101,114,112,111,108,97,116,105,111,110,58,115,82,71,66,59,99,111,108,111,114,45,105,110,116,101,114,112,111,108,
97,116,105,111,110,45,102,105,108,116,101,114,115,58,108,105,110,101,97,114,82,71,66,59,115,111,108,105,100,45,99,111,108,111,114,58,35,48,48,48,48,48,48,59,115,111,108,105,100,45,111,112,97,99,105,116,
121,58,49,59,118,101,99,116,111,114,45,101,102,102,101,99,116,58,110,111,110,101,59,102,105,108,108,58,35,102,102,102,102,102,102,59,102,105,108,108,45,111,112,97,99,105,116,121,58,49,59,102,105,108,108,
45,114,117,108,101,58,110,111,110,122,101,114,111,59,115,116,114,111,107,101,58,110,111,110,101,59,115,116,114,111,107,101,45,119,105,100,116,104,58,48,46,50,54,52,53,56,51,51,50,112,120,59,115,116,114,
111,107,101,45,108,105,110,101,99,97,112,58,98,117,116,116,59,115,116,114,111,107,101,45,108,105,110,101,106,111,105,110,58,109,105,116,101,114,59,115,116,114,111,107,101,45,109,105,116,101,114,108,105,
109,105,116,58,52,59,115,116,114,111,107,101,45,100,97,115,104,97,114,114,97,121,58,110,111,110,101,59,115,116,114,111,107,101,45,100,97,115,104,111,102,102,115,101,116,58,48,59,115,116,114,111,107,101,
45,111,112,97,99,105,116,121,58,49,59,99,111,108,111,114,45,114,101,110,100,101,114,105,110,103,58,97,117,116,111,59,105,109,97,103,101,45,114,101,110,100,101,114,105,110,103,58,97,117,116,111,59,115,
104,97,112,101,45,114,101,110,100,101,114,105,110,103,58,97,117,116,111,59,116,101,120,116,45,114,101,110,100,101,114,105,110,103,58,97,117,116,111,59,101,110,97,98,108,101,45,98,97,99,107,103,114,111,
117,110,100,58,97,99,99,117,109,117,108,97,116,101,34,10,32,32,32,32,32,32,32,100,61,34,109,32,49,54,54,46,53,55,50,50,55,44,55,54,46,54,55,49,56,55,53,32,99,32,45,49,52,46,53,48,52,57,44,49,46,53,54,
57,50,54,49,32,45,50,57,46,56,57,49,50,50,44,45,49,46,54,49,54,50,49,50,32,45,52,52,46,51,55,53,44,49,46,52,57,54,48,57,52,32,45,51,46,51,57,53,50,51,44,49,46,51,56,52,55,52,32,45,49,46,55,57,54,52,57,
44,53,46,56,55,52,51,54,49,32,45,51,46,51,52,51,55,53,44,55,46,57,55,54,53,54,50,32,45,52,46,53,56,52,49,54,44,49,46,51,54,49,56,54,53,32,45,55,46,49,56,57,57,44,54,46,55,51,48,55,53,53,32,45,55,46,48,
52,50,57,55,44,49,48,46,56,50,50,50,54,54,32,45,55,46,57,52,51,53,44,52,46,51,54,53,52,49,51,32,45,49,50,46,57,53,48,50,56,55,44,49,50,46,50,55,50,50,57,51,32,45,49,57,46,52,54,54,56,44,49,56,46,51,49,
56,51,54,51,32,45,48,46,48,56,55,54,53,44,55,46,56,50,54,54,51,32,49,49,46,51,49,56,50,56,44,49,48,46,49,54,51,51,50,32,49,53,46,48,55,48,51,49,44,51,46,55,54,57,53,51,32,53,46,48,56,52,44,45,54,46,48,
49,48,48,53,32,49,49,46,54,56,51,57,56,44,45,49,48,46,56,49,49,57,50,32,49,53,46,52,49,48,49,54,44,45,49,55,46,57,48,56,50,49,32,45,50,46,54,56,50,48,56,44,45,48,46,55,55,49,48,57,32,50,46,54,53,55,51,
52,44,45,50,46,48,57,49,55,50,53,32,51,46,50,51,48,52,55,44,45,51,46,54,57,49,52,48,50,32,51,46,52,54,49,53,57,44,45,52,46,48,51,56,54,53,52,32,55,46,55,48,51,53,54,44,45,56,46,54,54,52,56,52,57,32,49,
51,46,54,49,57,49,51,44,45,55,46,50,55,52,53,52,54,32,57,46,51,53,49,54,50,44,48,46,52,57,55,55,51,56,32,49,56,46,55,56,49,57,56,44,48,46,48,51,55,56,55,32,50,56,46,49,50,54,57,54,44,48,46,51,49,55,53,
49,53,32,53,46,57,54,56,50,57,44,48,46,48,49,54,32,56,46,53,49,57,55,52,44,45,55,46,57,55,48,55,50,50,32,51,46,56,56,50,56,49,44,45,49,49,46,54,48,51,53,49,54,32,45,49,46,51,53,56,55,49,44,45,49,46,51,
50,53,52,51,56,32,45,51,46,50,48,50,57,53,44,45,50,46,49,55,55,56,49,54,32,45,53,46,49,49,49,51,50,44,45,50,46,50,50,50,54,53,54,32,122,32,77,32,56,51,46,49,49,49,51,50,56,44,49,50,55,46,53,54,50,53,32,
99,32,45,56,46,57,52,56,56,57,53,44,50,46,51,52,54,56,57,32,45,49,51,46,50,56,54,55,56,44,49,50,46,48,48,55,51,57,32,45,50,48,46,51,54,51,50,56,49,44,49,55,46,50,56,55,49,49,32,48,46,50,48,51,53,51,50,
44,56,46,51,56,48,52,51,32,49,50,46,56,49,51,49,54,49,44,49,48,46,49,55,55,57,52,32,49,54,46,48,54,52,52,53,51,44,50,46,56,55,54,57,53,32,52,46,57,53,56,53,54,51,44,45,53,46,49,48,53,48,56,32,49,48,46,
48,57,52,50,50,53,44,45,49,48,46,48,57,53,51,50,32,49,53,46,48,54,50,53,44,45,49,53,46,50,49,52,56,52,32,45,52,46,53,49,49,49,48,55,44,48,46,48,51,54,49,32,45,54,46,52,56,52,50,51,52,44,45,52,46,56,50,
57,49,55,32,45,49,48,46,55,54,51,54,55,50,44,45,52,46,57,52,57,50,50,32,122,32,109,32,45,51,48,46,49,57,57,50,49,57,44,51,49,46,50,57,56,56,51,32,99,32,45,56,46,52,55,57,51,51,54,44,49,46,51,57,49,54,
49,32,45,49,49,46,49,57,52,51,48,53,44,49,52,46,51,56,52,51,53,32,45,50,48,46,56,48,52,54,49,51,44,49,50,46,51,50,53,55,51,32,45,56,46,55,49,51,52,49,56,44,45,48,46,49,48,48,57,52,32,45,49,55,46,52,54,
53,57,56,55,44,45,48,46,53,49,57,54,32,45,50,54,46,49,52,50,54,53,50,50,44,48,46,53,50,57,55,52,32,45,52,46,56,48,50,54,57,51,57,44,54,46,52,52,52,54,56,32,50,46,57,48,50,53,55,49,57,44,49,53,46,50,53,
52,54,55,32,49,48,46,48,50,53,51,57,48,50,44,49,50,46,56,54,51,50,56,32,49,48,46,55,56,50,55,48,50,44,45,49,46,55,48,51,55,57,32,50,52,46,52,54,51,53,56,44,51,46,56,48,54,57,55,32,51,50,46,57,52,51,51,
54,44,45,53,46,53,56,50,48,51,32,53,46,48,57,57,56,56,54,44,45,52,46,56,48,56,53,50,32,49,48,46,50,54,56,56,57,49,44,45,57,46,54,56,56,51,57,32,49,51,46,57,49,48,49,53,54,44,45,49,53,46,55,53,32,45,51,
46,56,55,53,52,48,57,44,45,48,46,51,48,53,54,57,32,45,54,46,48,49,52,54,55,50,44,45,52,46,53,49,49,48,54,32,45,57,46,57,51,49,54,52,49,44,45,52,46,51,56,54,55,50,32,122,34,10,32,32,32,32,32,32,32,105,
100,61,34,112,97,116,104,49,50,54,53,34,10,32,32,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,111,110,110,101,99,116,111,114,45,99,117,114,118,97,116,117,114,101,61,34,48,34,32,47,62,10,32,32,60,
47,103,62,10,60,47,115,118,103,62,10,0,0};

const char* VASTWaveTableEditorViewHeader::rampconnect_svg = (const char*) resource_VASTWaveTableEditorViewHeader_rampconnect_svg;
const int VASTWaveTableEditorViewHeader::rampconnect_svgSize = 6750;

// JUCER_RESOURCE: reverse_svg, 996, "../../Resources/Icons/reverse.svg"
static const unsigned char resource_VASTWaveTableEditorViewHeader_reverse_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,70,
45,56,34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,32,119,105,100,116,104,61,34,50,48,112,120,34,32,104,101,105,103,104,116,61,34,50,53,112,120,34,32,118,105,101,
119,66,111,120,61,34,48,32,48,32,50,48,32,50,53,34,32,118,101,114,115,105,111,110,61,34,49,46,49,34,32,120,109,108,110,115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,
48,48,47,115,118,103,34,32,120,109,108,110,115,58,120,108,105,110,107,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,120,108,105,110,107,34,62,10,32,32,32,32,60,
33,45,45,32,71,101,110,101,114,97,116,111,114,58,32,83,107,101,116,99,104,32,51,46,56,46,49,32,40,50,57,54,56,55,41,32,45,32,104,116,116,112,58,47,47,119,119,119,46,98,111,104,101,109,105,97,110,99,111,
100,105,110,103,46,99,111,109,47,115,107,101,116,99,104,32,45,45,62,10,32,32,32,32,60,116,105,116,108,101,62,97,114,114,111,119,95,114,105,103,104,116,32,91,35,51,55,48,93,60,47,116,105,116,108,101,62,
10,32,32,32,32,60,100,101,115,99,62,67,114,101,97,116,101,100,32,119,105,116,104,32,83,107,101,116,99,104,46,60,47,100,101,115,99,62,10,32,32,32,32,60,100,101,102,115,62,60,47,100,101,102,115,62,10,32,
32,32,32,60,103,32,105,100,61,34,80,97,103,101,45,49,34,32,115,116,114,111,107,101,61,34,110,111,110,101,34,32,115,116,114,111,107,101,45,119,105,100,116,104,61,34,49,34,32,102,105,108,108,61,34,110,111,
110,101,34,32,102,105,108,108,45,114,117,108,101,61,34,101,118,101,110,111,100,100,34,62,10,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,68,114,105,98,98,98,108,101,45,68,97,114,107,45,80,114,101,118,
105,101,119,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,45,49,52,48,46,48,48,48,48,48,48,44,32,45,54,53,53,57,46,48,48,48,48,48,48,41,34,32,102,105,108,108,61,34,
35,70,70,70,70,70,70,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,105,99,111,110,115,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,53,54,46,
48,48,48,48,48,48,44,32,49,54,48,46,48,48,48,48,48,48,41,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,60,112,97,116,104,32,100,61,34,77,56,54,44,54,52,49,55,46,48,48,50,49,57,32,76,57,57,46,
56,50,56,44,54,52,49,55,46,48,48,50,49,57,32,76,57,54,46,53,56,54,44,54,52,49,51,46,55,53,56,56,32,76,57,56,44,54,52,49,50,46,51,52,52,54,50,32,76,49,48,51,46,54,53,55,44,54,52,49,56,46,48,48,50,51,49,
32,67,49,48,49,46,53,57,49,44,54,52,50,48,46,48,54,56,53,54,32,49,48,48,46,48,48,53,44,54,52,50,49,46,54,53,52,55,54,32,57,56,44,54,52,50,51,46,54,54,32,76,57,54,46,53,56,54,44,54,52,50,50,46,50,52,53,
56,51,32,76,57,57,46,56,50,56,44,54,52,49,57,46,48,48,50,52,51,32,76,56,52,44,54,52,49,57,46,48,48,50,52,51,32,76,56,52,44,54,51,57,57,32,76,49,48,52,44,54,51,57,57,32,76,49,48,52,44,54,52,48,49,46,48,
48,48,50,52,32,76,56,54,44,54,52,48,49,46,48,48,48,50,52,32,76,56,54,44,54,52,49,55,46,48,48,50,49,57,32,90,34,32,105,100,61,34,97,114,114,111,119,95,114,105,103,104,116,45,91,35,51,55,48,93,34,62,60,
47,112,97,116,104,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,60,47,103,62,10,60,47,115,118,103,62,0,0};

const char* VASTWaveTableEditorViewHeader::reverse_svg = (const char*) resource_VASTWaveTableEditorViewHeader_reverse_svg;
const int VASTWaveTableEditorViewHeader::reverse_svgSize = 996;

// JUCER_RESOURCE: save_svg, 1353, "../../Resources/Icons/save.svg"
static const unsigned char resource_VASTWaveTableEditorViewHeader_save_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,70,45,
56,34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,32,119,105,100,116,104,61,34,50,49,112,120,34,32,104,101,105,103,104,116,61,34,50,48,112,120,34,32,118,105,101,119,
66,111,120,61,34,48,32,48,32,50,49,32,50,48,34,32,118,101,114,115,105,111,110,61,34,49,46,49,34,32,120,109,108,110,115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,
47,115,118,103,34,32,120,109,108,110,115,58,120,108,105,110,107,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,120,108,105,110,107,34,62,10,32,32,32,32,60,33,45,
45,32,71,101,110,101,114,97,116,111,114,58,32,83,107,101,116,99,104,32,51,46,56,46,49,32,40,50,57,54,56,55,41,32,45,32,104,116,116,112,58,47,47,119,119,119,46,98,111,104,101,109,105,97,110,99,111,100,
105,110,103,46,99,111,109,47,115,107,101,116,99,104,32,45,45,62,10,32,32,32,32,60,116,105,116,108,101,62,115,97,118,101,95,105,116,101,109,32,91,35,49,52,48,56,93,60,47,116,105,116,108,101,62,10,32,32,
32,32,60,100,101,115,99,62,67,114,101,97,116,101,100,32,119,105,116,104,32,83,107,101,116,99,104,46,60,47,100,101,115,99,62,10,32,32,32,32,60,100,101,102,115,62,60,47,100,101,102,115,62,10,32,32,32,32,
60,103,32,105,100,61,34,80,97,103,101,45,49,34,32,115,116,114,111,107,101,61,34,110,111,110,101,34,32,115,116,114,111,107,101,45,119,105,100,116,104,61,34,49,34,32,102,105,108,108,61,34,110,111,110,101,
34,32,102,105,108,108,45,114,117,108,101,61,34,101,118,101,110,111,100,100,34,62,10,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,68,114,105,98,98,98,108,101,45,68,97,114,107,45,80,114,101,118,105,101,
119,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,45,49,51,57,46,48,48,48,48,48,48,44,32,45,54,56,48,46,48,48,48,48,48,48,41,34,32,102,105,108,108,61,34,35,70,70,
70,70,70,70,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,105,99,111,110,115,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,53,54,46,48,48,48,
48,48,48,44,32,49,54,48,46,48,48,48,48,48,48,41,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,60,112,97,116,104,32,100,61,34,77,57,48,46,50,49,56,55,53,44,53,50,53,32,76,57,50,46,51,49,56,55,
53,44,53,50,53,32,76,57,50,46,51,49,56,55,53,44,53,50,51,32,76,57,48,46,50,49,56,55,53,44,53,50,51,32,76,57,48,46,50,49,56,55,53,44,53,50,53,32,90,32,77,56,55,46,50,44,53,51,54,32,76,57,57,46,56,44,53,
51,54,32,76,57,57,46,56,44,53,51,52,32,76,56,55,46,50,44,53,51,52,32,76,56,55,46,50,44,53,51,54,32,90,32,77,56,55,46,50,44,53,51,50,32,76,57,57,46,56,44,53,51,50,32,76,57,57,46,56,44,53,51,48,32,76,56,
55,46,50,44,53,51,48,32,76,56,55,46,50,44,53,51,50,32,90,32,77,49,48,49,46,57,44,53,51,56,32,76,56,53,46,49,44,53,51,56,32,76,56,53,46,49,44,53,50,54,46,56,51,55,32,76,56,55,46,50,44,53,50,52,46,56,51,
55,32,76,56,55,46,50,44,53,50,56,32,76,56,56,46,49,49,56,55,53,44,53,50,56,32,76,56,57,46,51,44,53,50,56,32,76,57,55,46,55,44,53,50,56,32,76,57,57,46,52,55,49,51,53,44,53,50,56,32,76,57,57,46,56,44,53,
50,56,32,76,57,57,46,56,44,53,50,50,32,76,49,48,49,46,57,44,53,50,50,32,76,49,48,49,46,57,44,53,51,56,32,90,32,77,56,57,46,51,44,53,50,50,46,56,51,55,32,76,57,48,46,49,55,56,56,53,44,53,50,50,32,76,57,
55,46,55,44,53,50,50,32,76,57,55,46,55,44,53,50,54,32,76,56,57,46,51,44,53,50,54,32,76,56,57,46,51,44,53,50,50,46,56,51,55,32,90,32,77,49,48,51,46,57,54,54,52,44,53,50,48,32,76,49,48,49,46,56,54,54,52,
44,53,50,48,32,76,56,57,46,51,44,53,50,48,32,76,56,57,46,51,44,53,50,48,46,48,48,56,32,76,56,55,46,50,48,56,52,44,53,50,50,32,76,56,55,46,50,44,53,50,50,32,76,56,55,46,50,44,53,50,50,46,48,48,56,32,76,
56,51,46,48,48,56,52,44,53,50,54,32,76,56,51,44,53,50,54,32,76,56,51,44,53,51,56,32,76,56,51,44,53,52,48,32,76,56,53,46,49,44,53,52,48,32,76,49,48,49,46,56,54,54,52,44,53,52,48,32,76,49,48,51,46,57,54,
54,52,44,53,52,48,32,76,49,48,52,44,53,52,48,32,76,49,48,52,44,53,51,56,32,76,49,48,52,44,53,50,50,32,76,49,48,52,44,53,50,48,32,76,49,48,51,46,57,54,54,52,44,53,50,48,32,90,34,32,105,100,61,34,115,97,
118,101,95,105,116,101,109,45,91,35,49,52,48,56,93,34,62,60,47,112,97,116,104,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,60,47,103,62,
10,60,47,115,118,103,62,0,0};

const char* VASTWaveTableEditorViewHeader::save_svg = (const char*) resource_VASTWaveTableEditorViewHeader_save_svg;
const int VASTWaveTableEditorViewHeader::save_svgSize = 1353;

// JUCER_RESOURCE: select_all_svg, 1470, "../../Resources/Icons/select_all.svg"
static const unsigned char resource_VASTWaveTableEditorViewHeader_select_all_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,
70,45,56,34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,32,119,105,100,116,104,61,34,50,48,112,120,34,32,104,101,105,103,104,116,61,34,50,48,112,120,34,32,118,105,
101,119,66,111,120,61,34,48,32,48,32,50,48,32,50,48,34,32,118,101,114,115,105,111,110,61,34,49,46,49,34,32,120,109,108,110,115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,
48,48,48,47,115,118,103,34,32,120,109,108,110,115,58,120,108,105,110,107,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,120,108,105,110,107,34,62,10,32,32,32,32,
60,33,45,45,32,71,101,110,101,114,97,116,111,114,58,32,83,107,101,116,99,104,32,51,46,56,46,49,32,40,50,57,54,56,55,41,32,45,32,104,116,116,112,58,47,47,119,119,119,46,98,111,104,101,109,105,97,110,99,
111,100,105,110,103,46,99,111,109,47,115,107,101,116,99,104,32,45,45,62,10,32,32,32,32,60,116,105,116,108,101,62,97,114,114,111,119,95,97,108,108,32,91,35,51,50,56,93,60,47,116,105,116,108,101,62,10,32,
32,32,32,60,100,101,115,99,62,67,114,101,97,116,101,100,32,119,105,116,104,32,83,107,101,116,99,104,46,60,47,100,101,115,99,62,10,32,32,32,32,60,100,101,102,115,62,60,47,100,101,102,115,62,10,32,32,32,
32,60,103,32,105,100,61,34,80,97,103,101,45,49,34,32,115,116,114,111,107,101,61,34,110,111,110,101,34,32,115,116,114,111,107,101,45,119,105,100,116,104,61,34,49,34,32,102,105,108,108,61,34,110,111,110,
101,34,32,102,105,108,108,45,114,117,108,101,61,34,101,118,101,110,111,100,100,34,62,10,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,68,114,105,98,98,98,108,101,45,68,97,114,107,45,80,114,101,118,105,
101,119,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,45,50,50,48,46,48,48,48,48,48,48,44,32,45,54,55,49,57,46,48,48,48,48,48,48,41,34,32,102,105,108,108,61,34,35,
70,70,70,70,70,70,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,105,99,111,110,115,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,53,54,46,48,
48,48,48,48,48,44,32,49,54,48,46,48,48,48,48,48,48,41,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,60,112,97,116,104,32,100,61,34,77,49,55,50,44,54,53,55,49,32,76,49,55,54,44,54,53,55,49,32,
76,49,55,54,44,54,53,54,55,32,76,49,55,50,44,54,53,54,55,32,76,49,55,50,44,54,53,55,49,32,90,32,77,49,56,52,44,54,53,54,56,46,57,57,32,76,49,56,48,46,52,54,53,44,54,53,54,53,46,52,53,53,32,76,49,55,57,
46,48,53,44,54,53,54,54,46,56,54,57,32,76,49,56,48,46,49,56,49,44,54,53,54,56,32,76,49,55,56,44,54,53,54,56,32,76,49,55,56,44,54,53,54,53,32,76,49,55,53,44,54,53,54,53,32,76,49,55,53,44,54,53,54,50,46,
56,49,57,32,76,49,55,54,46,49,51,49,44,54,53,54,51,46,57,53,32,76,49,55,55,46,53,52,53,44,54,53,54,50,46,53,51,53,32,76,49,55,52,46,48,49,44,54,53,53,57,32,76,49,55,52,44,54,53,53,57,46,48,49,32,76,49,
55,51,46,57,57,44,54,53,53,57,32,76,49,55,48,46,52,53,53,44,54,53,54,50,46,53,51,53,32,76,49,55,49,46,56,54,57,44,54,53,54,51,46,57,53,32,76,49,55,51,44,54,53,54,50,46,56,49,57,32,76,49,55,51,44,54,53,
54,53,32,76,49,55,48,44,54,53,54,53,32,76,49,55,48,44,54,53,54,56,32,76,49,54,55,46,56,49,57,44,54,53,54,56,32,76,49,54,56,46,57,53,44,54,53,54,54,46,56,54,57,32,76,49,54,55,46,53,51,54,44,54,53,54,53,
46,52,53,53,32,76,49,54,52,44,54,53,54,56,46,57,57,32,76,49,54,52,46,48,49,44,54,53,54,57,32,76,49,54,52,44,54,53,54,57,46,48,49,32,76,49,54,55,46,53,51,54,44,54,53,55,50,46,53,52,53,32,76,49,54,56,46,
57,53,44,54,53,55,49,46,49,51,49,32,76,49,54,55,46,56,49,57,44,54,53,55,48,32,76,49,55,48,44,54,53,55,48,32,76,49,55,48,44,54,53,55,51,32,76,49,55,51,44,54,53,55,51,32,76,49,55,51,44,54,53,55,53,46,49,
56,49,32,76,49,55,49,46,56,54,57,44,54,53,55,52,46,48,53,32,76,49,55,48,46,52,53,53,44,54,53,55,53,46,52,54,52,32,76,49,55,51,46,57,57,44,54,53,55,57,32,76,49,55,52,44,54,53,55,56,46,57,57,32,76,49,55,
52,46,48,49,44,54,53,55,57,32,76,49,55,55,46,53,52,53,44,54,53,55,53,46,52,54,52,32,76,49,55,54,46,49,51,49,44,54,53,55,52,46,48,53,32,76,49,55,53,44,54,53,55,53,46,49,56,49,32,76,49,55,53,44,54,53,55,
51,32,76,49,55,56,44,54,53,55,51,32,76,49,55,56,44,54,53,55,48,32,76,49,56,48,46,49,56,49,44,54,53,55,48,32,76,49,55,57,46,48,53,44,54,53,55,49,46,49,51,49,32,76,49,56,48,46,52,54,53,44,54,53,55,50,46,
53,52,53,32,76,49,56,52,44,54,53,54,57,46,48,49,32,76,49,56,51,46,57,57,44,54,53,54,57,32,76,49,56,52,44,54,53,54,56,46,57,57,32,90,34,32,105,100,61,34,97,114,114,111,119,95,97,108,108,45,91,35,51,50,
56,93,34,62,60,47,112,97,116,104,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,60,47,103,62,10,60,47,115,118,103,62,0,0};

const char* VASTWaveTableEditorViewHeader::select_all_svg = (const char*) resource_VASTWaveTableEditorViewHeader_select_all_svg;
const int VASTWaveTableEditorViewHeader::select_all_svgSize = 1470;

// JUCER_RESOURCE: to_editor_svg, 1192, "../../Resources/Icons/to_editor.svg"
static const unsigned char resource_VASTWaveTableEditorViewHeader_to_editor_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,70,
45,56,34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,32,119,105,100,116,104,61,34,50,48,112,120,34,32,104,101,105,103,104,116,61,34,57,112,120,34,32,118,105,101,119,
66,111,120,61,34,48,32,48,32,50,48,32,57,34,32,118,101,114,115,105,111,110,61,34,49,46,49,34,32,120,109,108,110,115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,
115,118,103,34,32,120,109,108,110,115,58,120,108,105,110,107,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,120,108,105,110,107,34,62,10,32,32,32,32,60,33,45,45,
32,71,101,110,101,114,97,116,111,114,58,32,83,107,101,116,99,104,32,51,46,56,46,49,32,40,50,57,54,56,55,41,32,45,32,104,116,116,112,58,47,47,119,119,119,46,98,111,104,101,109,105,97,110,99,111,100,105,
110,103,46,99,111,109,47,115,107,101,116,99,104,32,45,45,62,10,32,32,32,32,60,116,105,116,108,101,62,97,99,116,105,118,105,116,121,32,91,35,57,56,53,93,60,47,116,105,116,108,101,62,10,32,32,32,32,60,100,
101,115,99,62,67,114,101,97,116,101,100,32,119,105,116,104,32,83,107,101,116,99,104,46,60,47,100,101,115,99,62,10,32,32,32,32,60,100,101,102,115,62,60,47,100,101,102,115,62,10,32,32,32,32,60,103,32,105,
100,61,34,80,97,103,101,45,49,34,32,115,116,114,111,107,101,61,34,110,111,110,101,34,32,115,116,114,111,107,101,45,119,105,100,116,104,61,34,49,34,32,102,105,108,108,61,34,110,111,110,101,34,32,102,105,
108,108,45,114,117,108,101,61,34,101,118,101,110,111,100,100,34,62,10,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,68,114,105,98,98,98,108,101,45,68,97,114,107,45,80,114,101,118,105,101,119,34,32,116,
114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,45,50,54,48,46,48,48,48,48,48,48,44,32,45,51,56,52,53,46,48,48,48,48,48,48,41,34,32,102,105,108,108,61,34,35,70,70,70,70,70,70,
34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,105,99,111,110,115,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,53,54,46,48,48,48,48,48,48,44,
32,49,54,48,46,48,48,48,48,48,48,41,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,60,112,97,116,104,32,100,61,34,77,50,49,54,46,53,49,50,44,51,54,56,53,32,76,50,49,48,46,55,57,54,44,51,54,57,
48,46,55,57,32,76,50,48,56,46,48,53,49,44,51,54,56,56,46,49,53,53,32,76,50,48,56,46,48,52,52,44,51,54,56,56,46,49,54,50,32,76,50,48,56,46,48,51,56,44,51,54,56,56,46,49,53,54,32,76,50,48,52,44,51,54,57,
50,46,51,56,54,32,76,50,48,53,46,52,53,51,44,51,54,57,51,46,56,32,76,50,48,56,46,49,49,53,44,51,54,57,49,46,48,51,51,32,76,50,49,48,46,56,53,57,44,51,54,57,51,46,54,53,51,32,76,50,49,54,46,53,49,50,44,
51,54,56,55,46,56,56,53,32,76,50,49,57,46,50,53,50,44,51,54,57,48,46,54,54,32,67,50,49,57,46,53,52,53,44,51,54,57,48,46,48,50,56,32,50,50,48,46,48,52,54,44,51,54,56,57,46,53,49,52,32,50,50,48,46,54,54,
54,44,51,54,56,57,46,50,48,56,32,76,50,49,54,46,53,49,50,44,51,54,56,53,32,90,32,77,50,50,52,44,51,54,57,49,46,57,54,32,67,50,50,52,44,51,54,57,51,46,48,56,55,32,50,50,51,46,48,57,56,44,51,54,57,52,32,
50,50,49,46,57,56,54,44,51,54,57,52,32,67,50,50,48,46,56,55,52,44,51,54,57,52,32,50,49,57,46,57,55,51,44,51,54,57,51,46,48,56,55,32,50,49,57,46,57,55,51,44,51,54,57,49,46,57,54,32,67,50,49,57,46,57,55,
51,44,51,54,57,48,46,56,51,51,32,50,50,48,46,56,55,52,44,51,54,56,57,46,57,50,32,50,50,49,46,57,56,54,44,51,54,56,57,46,57,50,32,67,50,50,51,46,48,57,56,44,51,54,56,57,46,57,50,32,50,50,52,44,51,54,57,
48,46,56,51,51,32,50,50,52,44,51,54,57,49,46,57,54,32,76,50,50,52,44,51,54,57,49,46,57,54,32,90,34,32,105,100,61,34,97,99,116,105,118,105,116,121,45,91,35,57,56,53,93,34,62,60,47,112,97,116,104,62,10,
32,32,32,32,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,60,47,103,62,10,60,47,115,118,103,62,0,0};

const char* VASTWaveTableEditorViewHeader::to_editor_svg = (const char*) resource_VASTWaveTableEditorViewHeader_to_editor_svg;
const int VASTWaveTableEditorViewHeader::to_editor_svgSize = 1192;

// JUCER_RESOURCE: waves_halfsine_svg, 2720, "../../Resources/Icons/waves_halfsine.svg"
static const unsigned char resource_VASTWaveTableEditorViewHeader_waves_halfsine_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,
84,70,45,56,34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,10,32,32,32,120,109,108,110,115,58,100,99,61,34,104,116,116,112,58,47,47,112,117,114,108,46,111,114,103,
47,100,99,47,101,108,101,109,101,110,116,115,47,49,46,49,47,34,10,32,32,32,120,109,108,110,115,58,99,99,61,34,104,116,116,112,58,47,47,99,114,101,97,116,105,118,101,99,111,109,109,111,110,115,46,111,114,
103,47,110,115,35,34,10,32,32,32,120,109,108,110,115,58,114,100,102,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,48,50,47,50,50,45,114,100,102,45,115,121,110,116,
97,120,45,110,115,35,34,10,32,32,32,120,109,108,110,115,58,115,118,103,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,118,103,34,10,32,32,32,120,109,108,110,
115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,118,103,34,10,32,32,32,120,109,108,110,115,58,115,111,100,105,112,111,100,105,61,34,104,116,116,112,58,47,
47,115,111,100,105,112,111,100,105,46,115,111,117,114,99,101,102,111,114,103,101,46,110,101,116,47,68,84,68,47,115,111,100,105,112,111,100,105,45,48,46,100,116,100,34,10,32,32,32,120,109,108,110,115,58,
105,110,107,115,99,97,112,101,61,34,104,116,116,112,58,47,47,119,119,119,46,105,110,107,115,99,97,112,101,46,111,114,103,47,110,97,109,101,115,112,97,99,101,115,47,105,110,107,115,99,97,112,101,34,10,
32,32,32,119,105,100,116,104,61,34,50,48,112,120,34,10,32,32,32,104,101,105,103,104,116,61,34,50,48,112,120,34,10,32,32,32,118,105,101,119,66,111,120,61,34,48,32,48,32,50,48,32,50,48,34,10,32,32,32,118,
101,114,115,105,111,110,61,34,49,46,49,34,10,32,32,32,105,100,61,34,115,118,103,49,50,34,10,32,32,32,115,111,100,105,112,111,100,105,58,100,111,99,110,97,109,101,61,34,119,97,118,101,115,95,104,97,108,
102,115,105,110,101,46,115,118,103,34,10,32,32,32,105,110,107,115,99,97,112,101,58,118,101,114,115,105,111,110,61,34,48,46,57,50,46,51,32,40,50,52,48,53,53,52,54,44,32,50,48,49,56,45,48,51,45,49,49,41,
34,62,10,32,32,60,109,101,116,97,100,97,116,97,10,32,32,32,32,32,105,100,61,34,109,101,116,97,100,97,116,97,49,54,34,62,10,32,32,32,32,60,114,100,102,58,82,68,70,62,10,32,32,32,32,32,32,60,99,99,58,87,
111,114,107,10,32,32,32,32,32,32,32,32,32,114,100,102,58,97,98,111,117,116,61,34,34,62,10,32,32,32,32,32,32,32,32,60,100,99,58,102,111,114,109,97,116,62,105,109,97,103,101,47,115,118,103,43,120,109,108,
60,47,100,99,58,102,111,114,109,97,116,62,10,32,32,32,32,32,32,32,32,60,100,99,58,116,121,112,101,10,32,32,32,32,32,32,32,32,32,32,32,114,100,102,58,114,101,115,111,117,114,99,101,61,34,104,116,116,112,
58,47,47,112,117,114,108,46,111,114,103,47,100,99,47,100,99,109,105,116,121,112,101,47,83,116,105,108,108,73,109,97,103,101,34,32,47,62,10,32,32,32,32,32,32,32,32,60,100,99,58,116,105,116,108,101,62,115,
116,97,116,115,32,91,35,49,49,57,48,93,60,47,100,99,58,116,105,116,108,101,62,10,32,32,32,32,32,32,60,47,99,99,58,87,111,114,107,62,10,32,32,32,32,60,47,114,100,102,58,82,68,70,62,10,32,32,60,47,109,101,
116,97,100,97,116,97,62,10,32,32,60,115,111,100,105,112,111,100,105,58,110,97,109,101,100,118,105,101,119,10,32,32,32,32,32,112,97,103,101,99,111,108,111,114,61,34,35,57,53,57,53,57,53,34,10,32,32,32,
32,32,98,111,114,100,101,114,99,111,108,111,114,61,34,35,54,54,54,54,54,54,34,10,32,32,32,32,32,98,111,114,100,101,114,111,112,97,99,105,116,121,61,34,49,34,10,32,32,32,32,32,111,98,106,101,99,116,116,
111,108,101,114,97,110,99,101,61,34,49,48,34,10,32,32,32,32,32,103,114,105,100,116,111,108,101,114,97,110,99,101,61,34,49,48,34,10,32,32,32,32,32,103,117,105,100,101,116,111,108,101,114,97,110,99,101,
61,34,49,48,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,112,97,103,101,111,112,97,99,105,116,121,61,34,49,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,112,97,103,101,115,104,97,100,
111,119,61,34,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,119,105,100,116,104,61,34,51,56,52,48,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,
100,111,119,45,104,101,105,103,104,116,61,34,50,48,53,48,34,10,32,32,32,32,32,105,100,61,34,110,97,109,101,100,118,105,101,119,49,52,34,10,32,32,32,32,32,115,104,111,119,103,114,105,100,61,34,102,97,108,
115,101,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,122,111,111,109,61,34,52,55,46,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,120,61,34,54,46,56,48,56,53,52,56,34,10,32,32,32,
32,32,105,110,107,115,99,97,112,101,58,99,121,61,34,54,46,52,53,57,53,57,56,51,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,120,61,34,45,49,50,34,10,32,32,32,32,32,
105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,121,61,34,45,49,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,109,97,120,105,109,105,122,101,100,61,34,
49,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,117,114,114,101,110,116,45,108,97,121,101,114,61,34,115,118,103,49,50,34,32,47,62,10,32,32,60,33,45,45,32,71,101,110,101,114,97,116,111,114,
58,32,83,107,101,116,99,104,32,51,46,56,46,49,32,40,50,57,54,56,55,41,32,45,32,104,116,116,112,58,47,47,119,119,119,46,98,111,104,101,109,105,97,110,99,111,100,105,110,103,46,99,111,109,47,115,107,101,
116,99,104,32,45,45,62,10,32,32,60,116,105,116,108,101,10,32,32,32,32,32,105,100,61,34,116,105,116,108,101,50,34,62,115,116,97,116,115,32,91,35,49,49,57,48,93,60,47,116,105,116,108,101,62,10,32,32,60,
100,101,115,99,10,32,32,32,32,32,105,100,61,34,100,101,115,99,52,34,62,67,114,101,97,116,101,100,32,119,105,116,104,32,83,107,101,116,99,104,46,60,47,100,101,115,99,62,10,32,32,60,100,101,102,115,10,32,
32,32,32,32,105,100,61,34,100,101,102,115,54,34,32,47,62,10,32,32,60,103,10,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,45,49,52,48,44,45,50,56,51,57,41,
34,10,32,32,32,32,32,105,100,61,34,68,114,105,98,98,98,108,101,45,76,105,103,104,116,45,80,114,101,118,105,101,119,34,10,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,102,102,102,102,
102,102,59,102,105,108,108,45,114,117,108,101,58,101,118,101,110,111,100,100,59,115,116,114,111,107,101,58,110,111,110,101,59,115,116,114,111,107,101,45,119,105,100,116,104,58,49,59,102,105,108,108,45,
111,112,97,99,105,116,121,58,48,46,57,57,55,50,56,57,57,54,34,62,10,32,32,32,32,60,103,10,32,32,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,53,54,44,49,
54,48,41,34,10,32,32,32,32,32,32,32,105,100,61,34,105,99,111,110,115,34,10,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,102,102,102,102,102,102,59,102,105,108,108,45,111,112,97,
99,105,116,121,58,48,46,57,57,55,50,56,57,57,54,34,62,10,32,32,32,32,32,32,60,112,97,116,104,10,32,32,32,32,32,32,32,32,32,115,111,100,105,112,111,100,105,58,110,111,100,101,116,121,112,101,115,61,34,
99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,34,10,32,32,32,32,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,111,110,110,101,99,116,111,114,45,99,117,114,118,97,116,
117,114,101,61,34,48,34,10,32,32,32,32,32,32,32,32,32,105,100,61,34,115,116,97,116,115,45,91,35,49,49,57,48,93,34,10,32,32,32,32,32,32,32,32,32,100,61,34,109,32,49,48,50,44,50,54,56,56,32,118,32,50,32,
72,32,56,54,32,99,32,45,49,46,49,53,54,44,45,49,46,57,55,49,32,48,46,50,48,54,44,48,46,51,53,32,48,44,48,32,118,32,48,32,48,32,48,32,45,50,32,45,55,32,104,32,49,54,32,122,32,109,32,48,44,57,32,72,32,56,
54,32,118,32,45,55,32,48,32,48,32,99,32,50,46,55,50,51,44,52,46,54,53,52,32,45,49,46,50,57,52,44,45,50,46,50,50,50,32,48,44,48,32,104,32,49,54,32,118,32,48,32,122,32,109,32,45,49,56,44,50,32,104,32,50,
48,32,118,32,45,50,48,32,72,32,56,52,32,90,34,10,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,102,102,102,102,102,102,59,102,105,108,108,45,111,112,97,99,105,116,121,58,48,
46,57,57,55,50,56,57,57,54,34,32,47,62,10,32,32,32,32,60,47,103,62,10,32,32,60,47,103,62,10,32,32,60,112,97,116,104,10,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,110,111,110,101,59,115,
116,114,111,107,101,58,35,102,102,102,102,102,102,59,115,116,114,111,107,101,45,119,105,100,116,104,58,50,59,115,116,114,111,107,101,45,108,105,110,101,99,97,112,58,114,111,117,110,100,59,115,116,114,
111,107,101,45,108,105,110,101,106,111,105,110,58,114,111,117,110,100,59,115,116,114,111,107,101,45,109,105,116,101,114,108,105,109,105,116,58,52,59,115,116,114,111,107,101,45,100,97,115,104,97,114,114,
97,121,58,110,111,110,101,59,115,116,114,111,107,101,45,111,112,97,99,105,116,121,58,49,34,10,32,32,32,32,32,100,61,34,109,32,51,46,57,54,48,51,50,56,54,44,49,53,46,50,53,54,55,57,53,32,99,32,48,44,48,
32,52,46,57,53,54,56,48,53,49,44,45,49,54,46,49,54,51,52,48,51,56,56,32,56,46,51,57,57,48,51,48,52,44,45,56,46,49,48,51,50,53,51,49,32,51,46,52,52,50,50,50,54,44,56,46,48,54,48,49,53,50,49,32,51,46,52,
52,50,50,50,54,44,56,46,48,54,48,49,53,50,49,32,51,46,52,52,50,50,50,54,44,56,46,48,54,48,49,53,50,49,34,10,32,32,32,32,32,105,100,61,34,112,97,116,104,49,52,52,54,34,10,32,32,32,32,32,105,110,107,115,
99,97,112,101,58,99,111,110,110,101,99,116,111,114,45,99,117,114,118,97,116,117,114,101,61,34,48,34,32,47,62,10,60,47,115,118,103,62,10,0,0};

const char* VASTWaveTableEditorViewHeader::waves_halfsine_svg = (const char*) resource_VASTWaveTableEditorViewHeader_waves_halfsine_svg;
const int VASTWaveTableEditorViewHeader::waves_halfsine_svgSize = 2720;

// JUCER_RESOURCE: waves_ramp_svg, 2629, "../../Resources/Icons/waves_ramp.svg"
static const unsigned char resource_VASTWaveTableEditorViewHeader_waves_ramp_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,
70,45,56,34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,10,32,32,32,120,109,108,110,115,58,100,99,61,34,104,116,116,112,58,47,47,112,117,114,108,46,111,114,103,47,
100,99,47,101,108,101,109,101,110,116,115,47,49,46,49,47,34,10,32,32,32,120,109,108,110,115,58,99,99,61,34,104,116,116,112,58,47,47,99,114,101,97,116,105,118,101,99,111,109,109,111,110,115,46,111,114,
103,47,110,115,35,34,10,32,32,32,120,109,108,110,115,58,114,100,102,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,48,50,47,50,50,45,114,100,102,45,115,121,110,116,
97,120,45,110,115,35,34,10,32,32,32,120,109,108,110,115,58,115,118,103,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,118,103,34,10,32,32,32,120,109,108,110,
115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,118,103,34,10,32,32,32,120,109,108,110,115,58,115,111,100,105,112,111,100,105,61,34,104,116,116,112,58,47,
47,115,111,100,105,112,111,100,105,46,115,111,117,114,99,101,102,111,114,103,101,46,110,101,116,47,68,84,68,47,115,111,100,105,112,111,100,105,45,48,46,100,116,100,34,10,32,32,32,120,109,108,110,115,58,
105,110,107,115,99,97,112,101,61,34,104,116,116,112,58,47,47,119,119,119,46,105,110,107,115,99,97,112,101,46,111,114,103,47,110,97,109,101,115,112,97,99,101,115,47,105,110,107,115,99,97,112,101,34,10,
32,32,32,119,105,100,116,104,61,34,50,48,112,120,34,10,32,32,32,104,101,105,103,104,116,61,34,50,48,112,120,34,10,32,32,32,118,105,101,119,66,111,120,61,34,48,32,48,32,50,48,32,50,48,34,10,32,32,32,118,
101,114,115,105,111,110,61,34,49,46,49,34,10,32,32,32,105,100,61,34,115,118,103,49,50,34,10,32,32,32,115,111,100,105,112,111,100,105,58,100,111,99,110,97,109,101,61,34,119,97,118,101,115,95,114,97,109,
112,46,115,118,103,34,10,32,32,32,105,110,107,115,99,97,112,101,58,118,101,114,115,105,111,110,61,34,48,46,57,50,46,51,32,40,50,52,48,53,53,52,54,44,32,50,48,49,56,45,48,51,45,49,49,41,34,62,10,32,32,
60,109,101,116,97,100,97,116,97,10,32,32,32,32,32,105,100,61,34,109,101,116,97,100,97,116,97,49,54,34,62,10,32,32,32,32,60,114,100,102,58,82,68,70,62,10,32,32,32,32,32,32,60,99,99,58,87,111,114,107,10,
32,32,32,32,32,32,32,32,32,114,100,102,58,97,98,111,117,116,61,34,34,62,10,32,32,32,32,32,32,32,32,60,100,99,58,102,111,114,109,97,116,62,105,109,97,103,101,47,115,118,103,43,120,109,108,60,47,100,99,
58,102,111,114,109,97,116,62,10,32,32,32,32,32,32,32,32,60,100,99,58,116,121,112,101,10,32,32,32,32,32,32,32,32,32,32,32,114,100,102,58,114,101,115,111,117,114,99,101,61,34,104,116,116,112,58,47,47,112,
117,114,108,46,111,114,103,47,100,99,47,100,99,109,105,116,121,112,101,47,83,116,105,108,108,73,109,97,103,101,34,32,47,62,10,32,32,32,32,32,32,32,32,60,100,99,58,116,105,116,108,101,62,115,116,97,116,
115,32,91,35,49,49,57,48,93,60,47,100,99,58,116,105,116,108,101,62,10,32,32,32,32,32,32,60,47,99,99,58,87,111,114,107,62,10,32,32,32,32,60,47,114,100,102,58,82,68,70,62,10,32,32,60,47,109,101,116,97,100,
97,116,97,62,10,32,32,60,115,111,100,105,112,111,100,105,58,110,97,109,101,100,118,105,101,119,10,32,32,32,32,32,112,97,103,101,99,111,108,111,114,61,34,35,57,53,57,53,57,53,34,10,32,32,32,32,32,98,111,
114,100,101,114,99,111,108,111,114,61,34,35,54,54,54,54,54,54,34,10,32,32,32,32,32,98,111,114,100,101,114,111,112,97,99,105,116,121,61,34,49,34,10,32,32,32,32,32,111,98,106,101,99,116,116,111,108,101,
114,97,110,99,101,61,34,49,48,34,10,32,32,32,32,32,103,114,105,100,116,111,108,101,114,97,110,99,101,61,34,49,48,34,10,32,32,32,32,32,103,117,105,100,101,116,111,108,101,114,97,110,99,101,61,34,49,48,
34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,112,97,103,101,111,112,97,99,105,116,121,61,34,49,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,112,97,103,101,115,104,97,100,111,119,61,34,
50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,119,105,100,116,104,61,34,51,56,52,48,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,
45,104,101,105,103,104,116,61,34,50,48,53,48,34,10,32,32,32,32,32,105,100,61,34,110,97,109,101,100,118,105,101,119,49,52,34,10,32,32,32,32,32,115,104,111,119,103,114,105,100,61,34,102,97,108,115,101,34,
10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,122,111,111,109,61,34,52,55,46,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,120,61,34,54,46,56,48,56,53,52,56,34,10,32,32,32,32,32,105,
110,107,115,99,97,112,101,58,99,121,61,34,54,46,52,53,57,53,57,56,51,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,120,61,34,45,49,50,34,10,32,32,32,32,32,105,110,107,
115,99,97,112,101,58,119,105,110,100,111,119,45,121,61,34,45,49,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,109,97,120,105,109,105,122,101,100,61,34,49,34,10,32,
32,32,32,32,105,110,107,115,99,97,112,101,58,99,117,114,114,101,110,116,45,108,97,121,101,114,61,34,115,118,103,49,50,34,32,47,62,10,32,32,60,33,45,45,32,71,101,110,101,114,97,116,111,114,58,32,83,107,
101,116,99,104,32,51,46,56,46,49,32,40,50,57,54,56,55,41,32,45,32,104,116,116,112,58,47,47,119,119,119,46,98,111,104,101,109,105,97,110,99,111,100,105,110,103,46,99,111,109,47,115,107,101,116,99,104,32,
45,45,62,10,32,32,60,116,105,116,108,101,10,32,32,32,32,32,105,100,61,34,116,105,116,108,101,50,34,62,115,116,97,116,115,32,91,35,49,49,57,48,93,60,47,116,105,116,108,101,62,10,32,32,60,100,101,115,99,
10,32,32,32,32,32,105,100,61,34,100,101,115,99,52,34,62,67,114,101,97,116,101,100,32,119,105,116,104,32,83,107,101,116,99,104,46,60,47,100,101,115,99,62,10,32,32,60,100,101,102,115,10,32,32,32,32,32,105,
100,61,34,100,101,102,115,54,34,32,47,62,10,32,32,60,103,10,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,45,49,52,48,44,45,50,56,51,57,41,34,10,32,32,32,
32,32,105,100,61,34,68,114,105,98,98,98,108,101,45,76,105,103,104,116,45,80,114,101,118,105,101,119,34,10,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,102,102,102,102,102,102,59,102,
105,108,108,45,114,117,108,101,58,101,118,101,110,111,100,100,59,115,116,114,111,107,101,58,110,111,110,101,59,115,116,114,111,107,101,45,119,105,100,116,104,58,49,59,102,105,108,108,45,111,112,97,99,
105,116,121,58,48,46,57,57,55,50,56,57,57,54,34,62,10,32,32,32,32,60,103,10,32,32,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,53,54,44,49,54,48,41,34,10,
32,32,32,32,32,32,32,105,100,61,34,105,99,111,110,115,34,10,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,102,102,102,102,102,102,59,102,105,108,108,45,111,112,97,99,105,116,121,
58,48,46,57,57,55,50,56,57,57,54,34,62,10,32,32,32,32,32,32,60,112,97,116,104,10,32,32,32,32,32,32,32,32,32,115,111,100,105,112,111,100,105,58,110,111,100,101,116,121,112,101,115,61,34,99,99,99,99,99,
99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,34,10,32,32,32,32,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,111,110,110,101,99,116,111,114,45,99,117,114,118,97,116,117,114,101,61,
34,48,34,10,32,32,32,32,32,32,32,32,32,105,100,61,34,115,116,97,116,115,45,91,35,49,49,57,48,93,34,10,32,32,32,32,32,32,32,32,32,100,61,34,109,32,49,48,50,44,50,54,56,56,32,118,32,50,32,72,32,56,54,32,
99,32,45,49,46,49,53,54,44,45,49,46,57,55,49,32,48,46,50,48,54,44,48,46,51,53,32,48,44,48,32,118,32,48,32,48,32,48,32,45,50,32,45,55,32,104,32,49,54,32,122,32,109,32,48,44,57,32,72,32,56,54,32,118,32,
45,55,32,48,32,48,32,99,32,50,46,55,50,51,44,52,46,54,53,52,32,45,49,46,50,57,52,44,45,50,46,50,50,50,32,48,44,48,32,104,32,49,54,32,118,32,48,32,122,32,109,32,45,49,56,44,50,32,104,32,50,48,32,118,32,
45,50,48,32,72,32,56,52,32,90,34,10,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,102,102,102,102,102,102,59,102,105,108,108,45,111,112,97,99,105,116,121,58,48,46,57,57,55,
50,56,57,57,54,34,32,47,62,10,32,32,32,32,60,47,103,62,10,32,32,60,47,103,62,10,32,32,60,112,97,116,104,10,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,110,111,110,101,59,115,116,114,111,
107,101,58,35,102,102,102,102,102,102,59,115,116,114,111,107,101,45,119,105,100,116,104,58,50,59,115,116,114,111,107,101,45,108,105,110,101,99,97,112,58,114,111,117,110,100,59,115,116,114,111,107,101,
45,108,105,110,101,106,111,105,110,58,114,111,117,110,100,59,115,116,114,111,107,101,45,111,112,97,99,105,116,121,58,49,59,115,116,114,111,107,101,45,109,105,116,101,114,108,105,109,105,116,58,52,59,115,
116,114,111,107,101,45,100,97,115,104,97,114,114,97,121,58,110,111,110,101,34,10,32,32,32,32,32,100,61,34,77,32,52,46,52,52,57,49,53,50,53,44,49,53,46,54,57,57,49,53,51,32,49,53,46,53,50,57,54,54,49,44,
52,46,55,50,52,53,55,54,51,34,10,32,32,32,32,32,105,100,61,34,112,97,116,104,49,52,52,48,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,111,110,110,101,99,116,111,114,45,99,117,114,118,97,116,
117,114,101,61,34,48,34,32,47,62,10,60,47,115,118,103,62,10,0,0};

const char* VASTWaveTableEditorViewHeader::waves_ramp_svg = (const char*) resource_VASTWaveTableEditorViewHeader_waves_ramp_svg;
const int VASTWaveTableEditorViewHeader::waves_ramp_svgSize = 2629;

// JUCER_RESOURCE: waves_random_svg, 2908, "../../Resources/Icons/waves_random.svg"
static const unsigned char resource_VASTWaveTableEditorViewHeader_waves_random_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,
70,45,56,34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,10,32,32,32,120,109,108,110,115,58,100,99,61,34,104,116,116,112,58,47,47,112,117,114,108,46,111,114,103,47,
100,99,47,101,108,101,109,101,110,116,115,47,49,46,49,47,34,10,32,32,32,120,109,108,110,115,58,99,99,61,34,104,116,116,112,58,47,47,99,114,101,97,116,105,118,101,99,111,109,109,111,110,115,46,111,114,
103,47,110,115,35,34,10,32,32,32,120,109,108,110,115,58,114,100,102,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,48,50,47,50,50,45,114,100,102,45,115,121,110,116,
97,120,45,110,115,35,34,10,32,32,32,120,109,108,110,115,58,115,118,103,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,118,103,34,10,32,32,32,120,109,108,110,
115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,118,103,34,10,32,32,32,120,109,108,110,115,58,115,111,100,105,112,111,100,105,61,34,104,116,116,112,58,47,
47,115,111,100,105,112,111,100,105,46,115,111,117,114,99,101,102,111,114,103,101,46,110,101,116,47,68,84,68,47,115,111,100,105,112,111,100,105,45,48,46,100,116,100,34,10,32,32,32,120,109,108,110,115,58,
105,110,107,115,99,97,112,101,61,34,104,116,116,112,58,47,47,119,119,119,46,105,110,107,115,99,97,112,101,46,111,114,103,47,110,97,109,101,115,112,97,99,101,115,47,105,110,107,115,99,97,112,101,34,10,
32,32,32,119,105,100,116,104,61,34,50,48,112,120,34,10,32,32,32,104,101,105,103,104,116,61,34,50,48,112,120,34,10,32,32,32,118,105,101,119,66,111,120,61,34,48,32,48,32,50,48,32,50,48,34,10,32,32,32,118,
101,114,115,105,111,110,61,34,49,46,49,34,10,32,32,32,105,100,61,34,115,118,103,49,50,34,10,32,32,32,115,111,100,105,112,111,100,105,58,100,111,99,110,97,109,101,61,34,119,97,118,101,115,95,114,97,110,
100,111,109,46,115,118,103,34,10,32,32,32,105,110,107,115,99,97,112,101,58,118,101,114,115,105,111,110,61,34,48,46,57,50,46,51,32,40,50,52,48,53,53,52,54,44,32,50,48,49,56,45,48,51,45,49,49,41,34,62,10,
32,32,60,109,101,116,97,100,97,116,97,10,32,32,32,32,32,105,100,61,34,109,101,116,97,100,97,116,97,49,54,34,62,10,32,32,32,32,60,114,100,102,58,82,68,70,62,10,32,32,32,32,32,32,60,99,99,58,87,111,114,
107,10,32,32,32,32,32,32,32,32,32,114,100,102,58,97,98,111,117,116,61,34,34,62,10,32,32,32,32,32,32,32,32,60,100,99,58,102,111,114,109,97,116,62,105,109,97,103,101,47,115,118,103,43,120,109,108,60,47,
100,99,58,102,111,114,109,97,116,62,10,32,32,32,32,32,32,32,32,60,100,99,58,116,121,112,101,10,32,32,32,32,32,32,32,32,32,32,32,114,100,102,58,114,101,115,111,117,114,99,101,61,34,104,116,116,112,58,47,
47,112,117,114,108,46,111,114,103,47,100,99,47,100,99,109,105,116,121,112,101,47,83,116,105,108,108,73,109,97,103,101,34,32,47,62,10,32,32,32,32,32,32,32,32,60,100,99,58,116,105,116,108,101,62,115,116,
97,116,115,32,91,35,49,49,57,48,93,60,47,100,99,58,116,105,116,108,101,62,10,32,32,32,32,32,32,60,47,99,99,58,87,111,114,107,62,10,32,32,32,32,60,47,114,100,102,58,82,68,70,62,10,32,32,60,47,109,101,116,
97,100,97,116,97,62,10,32,32,60,115,111,100,105,112,111,100,105,58,110,97,109,101,100,118,105,101,119,10,32,32,32,32,32,112,97,103,101,99,111,108,111,114,61,34,35,57,53,57,53,57,53,34,10,32,32,32,32,32,
98,111,114,100,101,114,99,111,108,111,114,61,34,35,54,54,54,54,54,54,34,10,32,32,32,32,32,98,111,114,100,101,114,111,112,97,99,105,116,121,61,34,49,34,10,32,32,32,32,32,111,98,106,101,99,116,116,111,108,
101,114,97,110,99,101,61,34,49,48,34,10,32,32,32,32,32,103,114,105,100,116,111,108,101,114,97,110,99,101,61,34,49,48,34,10,32,32,32,32,32,103,117,105,100,101,116,111,108,101,114,97,110,99,101,61,34,49,
48,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,112,97,103,101,111,112,97,99,105,116,121,61,34,49,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,112,97,103,101,115,104,97,100,111,119,61,
34,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,119,105,100,116,104,61,34,51,56,52,48,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,
45,104,101,105,103,104,116,61,34,50,48,53,48,34,10,32,32,32,32,32,105,100,61,34,110,97,109,101,100,118,105,101,119,49,52,34,10,32,32,32,32,32,115,104,111,119,103,114,105,100,61,34,102,97,108,115,101,34,
10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,122,111,111,109,61,34,52,55,46,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,120,61,34,49,46,52,51,55,55,56,53,51,34,10,32,32,32,32,32,
105,110,107,115,99,97,112,101,58,99,121,61,34,54,46,52,53,57,53,57,56,51,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,120,61,34,45,49,50,34,10,32,32,32,32,32,105,110,
107,115,99,97,112,101,58,119,105,110,100,111,119,45,121,61,34,45,49,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,109,97,120,105,109,105,122,101,100,61,34,49,34,10,
32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,117,114,114,101,110,116,45,108,97,121,101,114,61,34,115,118,103,49,50,34,32,47,62,10,32,32,60,33,45,45,32,71,101,110,101,114,97,116,111,114,58,32,83,
107,101,116,99,104,32,51,46,56,46,49,32,40,50,57,54,56,55,41,32,45,32,104,116,116,112,58,47,47,119,119,119,46,98,111,104,101,109,105,97,110,99,111,100,105,110,103,46,99,111,109,47,115,107,101,116,99,104,
32,45,45,62,10,32,32,60,116,105,116,108,101,10,32,32,32,32,32,105,100,61,34,116,105,116,108,101,50,34,62,115,116,97,116,115,32,91,35,49,49,57,48,93,60,47,116,105,116,108,101,62,10,32,32,60,100,101,115,
99,10,32,32,32,32,32,105,100,61,34,100,101,115,99,52,34,62,67,114,101,97,116,101,100,32,119,105,116,104,32,83,107,101,116,99,104,46,60,47,100,101,115,99,62,10,32,32,60,100,101,102,115,10,32,32,32,32,32,
105,100,61,34,100,101,102,115,54,34,32,47,62,10,32,32,60,103,10,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,45,49,52,48,44,45,50,56,51,57,41,34,10,32,32,
32,32,32,105,100,61,34,68,114,105,98,98,98,108,101,45,76,105,103,104,116,45,80,114,101,118,105,101,119,34,10,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,102,102,102,102,102,102,59,102,
105,108,108,45,114,117,108,101,58,101,118,101,110,111,100,100,59,115,116,114,111,107,101,58,110,111,110,101,59,115,116,114,111,107,101,45,119,105,100,116,104,58,49,59,102,105,108,108,45,111,112,97,99,
105,116,121,58,48,46,57,57,55,50,56,57,57,54,34,62,10,32,32,32,32,60,103,10,32,32,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,53,54,44,49,54,48,41,34,10,
32,32,32,32,32,32,32,105,100,61,34,105,99,111,110,115,34,10,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,102,102,102,102,102,102,59,102,105,108,108,45,111,112,97,99,105,116,121,
58,48,46,57,57,55,50,56,57,57,54,34,62,10,32,32,32,32,32,32,60,112,97,116,104,10,32,32,32,32,32,32,32,32,32,115,111,100,105,112,111,100,105,58,110,111,100,101,116,121,112,101,115,61,34,99,99,99,99,99,
99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,34,10,32,32,32,32,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,111,110,110,101,99,116,111,114,45,99,117,114,118,97,116,117,114,101,61,
34,48,34,10,32,32,32,32,32,32,32,32,32,105,100,61,34,115,116,97,116,115,45,91,35,49,49,57,48,93,34,10,32,32,32,32,32,32,32,32,32,100,61,34,109,32,49,48,50,44,50,54,56,56,32,118,32,50,32,72,32,56,54,32,
99,32,45,49,46,49,53,54,44,45,49,46,57,55,49,32,48,46,50,48,54,44,48,46,51,53,32,48,44,48,32,118,32,48,32,48,32,48,32,45,50,32,45,55,32,104,32,49,54,32,122,32,109,32,48,44,57,32,72,32,56,54,32,118,32,
45,55,32,48,32,48,32,99,32,50,46,55,50,51,44,52,46,54,53,52,32,45,49,46,50,57,52,44,45,50,46,50,50,50,32,48,44,48,32,104,32,49,54,32,118,32,48,32,122,32,109,32,45,49,56,44,50,32,104,32,50,48,32,118,32,
45,50,48,32,72,32,56,52,32,90,34,10,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,102,102,102,102,102,102,59,102,105,108,108,45,111,112,97,99,105,116,121,58,48,46,57,57,55,
50,56,57,57,54,34,32,47,62,10,32,32,32,32,60,47,103,62,10,32,32,60,47,103,62,10,32,32,60,112,97,116,104,10,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,110,111,110,101,59,115,116,114,111,
107,101,58,35,102,102,102,102,102,102,59,115,116,114,111,107,101,45,119,105,100,116,104,58,49,59,115,116,114,111,107,101,45,108,105,110,101,99,97,112,58,98,117,116,116,59,115,116,114,111,107,101,45,108,
105,110,101,106,111,105,110,58,109,105,116,101,114,59,115,116,114,111,107,101,45,111,112,97,99,105,116,121,58,49,59,115,116,114,111,107,101,45,109,105,116,101,114,108,105,109,105,116,58,52,59,115,116,
114,111,107,101,45,100,97,115,104,97,114,114,97,121,58,110,111,110,101,34,10,32,32,32,32,32,100,61,34,77,32,50,46,57,50,51,55,50,56,56,44,49,48,46,55,50,48,51,51,57,32,51,46,53,53,57,51,50,50,44,51,46,
52,49,49,48,49,54,57,32,52,46,51,52,51,50,50,48,51,44,49,54,46,56,50,50,48,51,52,32,53,46,51,51,56,57,56,51,49,44,51,46,52,57,53,55,54,50,55,32,54,46,49,54,53,50,53,52,50,44,49,54,46,53,54,55,55,57,55,
32,55,46,52,49,53,50,53,52,50,44,51,46,51,56,57,56,51,48,53,32,56,46,52,55,52,53,55,54,51,44,49,54,46,54,49,48,49,54,57,32,57,46,53,53,53,48,56,52,55,44,51,46,51,54,56,54,52,52,49,32,49,48,46,50,55,53,
52,50,52,44,49,54,46,53,50,53,52,50,52,32,49,49,46,54,57,52,57,49,53,44,51,46,53,51,56,49,51,53,54,32,49,50,46,52,53,55,54,50,55,44,49,54,46,48,49,54,57,52,57,32,49,51,46,52,53,51,51,57,44,51,46,52,53,
51,51,56,57,56,32,49,52,46,49,57,52,57,49,53,44,49,54,46,51,53,53,57,51,50,32,49,53,46,56,48,53,48,56,53,44,51,46,49,49,52,52,48,54,56,32,49,54,46,53,54,55,55,57,55,44,49,48,46,57,53,51,51,57,32,49,54,
46,53,48,52,50,51,55,44,49,48,46,56,52,55,52,53,56,34,10,32,32,32,32,32,105,100,61,34,112,97,116,104,50,48,49,49,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,111,110,110,101,99,116,111,114,
45,99,117,114,118,97,116,117,114,101,61,34,48,34,32,47,62,10,60,47,115,118,103,62,10,0,0};

const char* VASTWaveTableEditorViewHeader::waves_random_svg = (const char*) resource_VASTWaveTableEditorViewHeader_waves_random_svg;
const int VASTWaveTableEditorViewHeader::waves_random_svgSize = 2908;

// JUCER_RESOURCE: waves_saw_svg, 2662, "../../Resources/Icons/waves_saw.svg"
static const unsigned char resource_VASTWaveTableEditorViewHeader_waves_saw_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,70,
45,56,34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,10,32,32,32,120,109,108,110,115,58,100,99,61,34,104,116,116,112,58,47,47,112,117,114,108,46,111,114,103,47,100,
99,47,101,108,101,109,101,110,116,115,47,49,46,49,47,34,10,32,32,32,120,109,108,110,115,58,99,99,61,34,104,116,116,112,58,47,47,99,114,101,97,116,105,118,101,99,111,109,109,111,110,115,46,111,114,103,
47,110,115,35,34,10,32,32,32,120,109,108,110,115,58,114,100,102,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,48,50,47,50,50,45,114,100,102,45,115,121,110,116,97,
120,45,110,115,35,34,10,32,32,32,120,109,108,110,115,58,115,118,103,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,118,103,34,10,32,32,32,120,109,108,110,115,
61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,118,103,34,10,32,32,32,120,109,108,110,115,58,115,111,100,105,112,111,100,105,61,34,104,116,116,112,58,47,47,115,
111,100,105,112,111,100,105,46,115,111,117,114,99,101,102,111,114,103,101,46,110,101,116,47,68,84,68,47,115,111,100,105,112,111,100,105,45,48,46,100,116,100,34,10,32,32,32,120,109,108,110,115,58,105,110,
107,115,99,97,112,101,61,34,104,116,116,112,58,47,47,119,119,119,46,105,110,107,115,99,97,112,101,46,111,114,103,47,110,97,109,101,115,112,97,99,101,115,47,105,110,107,115,99,97,112,101,34,10,32,32,32,
119,105,100,116,104,61,34,50,48,112,120,34,10,32,32,32,104,101,105,103,104,116,61,34,50,48,112,120,34,10,32,32,32,118,105,101,119,66,111,120,61,34,48,32,48,32,50,48,32,50,48,34,10,32,32,32,118,101,114,
115,105,111,110,61,34,49,46,49,34,10,32,32,32,105,100,61,34,115,118,103,49,50,34,10,32,32,32,115,111,100,105,112,111,100,105,58,100,111,99,110,97,109,101,61,34,119,97,118,101,115,95,115,97,119,46,115,
118,103,34,10,32,32,32,105,110,107,115,99,97,112,101,58,118,101,114,115,105,111,110,61,34,48,46,57,50,46,51,32,40,50,52,48,53,53,52,54,44,32,50,48,49,56,45,48,51,45,49,49,41,34,62,10,32,32,60,109,101,
116,97,100,97,116,97,10,32,32,32,32,32,105,100,61,34,109,101,116,97,100,97,116,97,49,54,34,62,10,32,32,32,32,60,114,100,102,58,82,68,70,62,10,32,32,32,32,32,32,60,99,99,58,87,111,114,107,10,32,32,32,32,
32,32,32,32,32,114,100,102,58,97,98,111,117,116,61,34,34,62,10,32,32,32,32,32,32,32,32,60,100,99,58,102,111,114,109,97,116,62,105,109,97,103,101,47,115,118,103,43,120,109,108,60,47,100,99,58,102,111,114,
109,97,116,62,10,32,32,32,32,32,32,32,32,60,100,99,58,116,121,112,101,10,32,32,32,32,32,32,32,32,32,32,32,114,100,102,58,114,101,115,111,117,114,99,101,61,34,104,116,116,112,58,47,47,112,117,114,108,46,
111,114,103,47,100,99,47,100,99,109,105,116,121,112,101,47,83,116,105,108,108,73,109,97,103,101,34,32,47,62,10,32,32,32,32,32,32,32,32,60,100,99,58,116,105,116,108,101,62,115,116,97,116,115,32,91,35,49,
49,57,48,93,60,47,100,99,58,116,105,116,108,101,62,10,32,32,32,32,32,32,60,47,99,99,58,87,111,114,107,62,10,32,32,32,32,60,47,114,100,102,58,82,68,70,62,10,32,32,60,47,109,101,116,97,100,97,116,97,62,
10,32,32,60,115,111,100,105,112,111,100,105,58,110,97,109,101,100,118,105,101,119,10,32,32,32,32,32,112,97,103,101,99,111,108,111,114,61,34,35,57,53,57,53,57,53,34,10,32,32,32,32,32,98,111,114,100,101,
114,99,111,108,111,114,61,34,35,54,54,54,54,54,54,34,10,32,32,32,32,32,98,111,114,100,101,114,111,112,97,99,105,116,121,61,34,49,34,10,32,32,32,32,32,111,98,106,101,99,116,116,111,108,101,114,97,110,99,
101,61,34,49,48,34,10,32,32,32,32,32,103,114,105,100,116,111,108,101,114,97,110,99,101,61,34,49,48,34,10,32,32,32,32,32,103,117,105,100,101,116,111,108,101,114,97,110,99,101,61,34,49,48,34,10,32,32,32,
32,32,105,110,107,115,99,97,112,101,58,112,97,103,101,111,112,97,99,105,116,121,61,34,49,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,112,97,103,101,115,104,97,100,111,119,61,34,50,34,10,32,32,
32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,119,105,100,116,104,61,34,51,56,52,48,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,104,101,105,
103,104,116,61,34,50,48,53,48,34,10,32,32,32,32,32,105,100,61,34,110,97,109,101,100,118,105,101,119,49,52,34,10,32,32,32,32,32,115,104,111,119,103,114,105,100,61,34,102,97,108,115,101,34,10,32,32,32,32,
32,105,110,107,115,99,97,112,101,58,122,111,111,109,61,34,52,55,46,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,120,61,34,54,46,56,48,56,53,52,56,34,10,32,32,32,32,32,105,110,107,115,99,
97,112,101,58,99,121,61,34,54,46,52,53,57,53,57,56,51,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,120,61,34,45,49,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,
101,58,119,105,110,100,111,119,45,121,61,34,45,49,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,109,97,120,105,109,105,122,101,100,61,34,49,34,10,32,32,32,32,32,105,
110,107,115,99,97,112,101,58,99,117,114,114,101,110,116,45,108,97,121,101,114,61,34,115,118,103,49,50,34,32,47,62,10,32,32,60,33,45,45,32,71,101,110,101,114,97,116,111,114,58,32,83,107,101,116,99,104,
32,51,46,56,46,49,32,40,50,57,54,56,55,41,32,45,32,104,116,116,112,58,47,47,119,119,119,46,98,111,104,101,109,105,97,110,99,111,100,105,110,103,46,99,111,109,47,115,107,101,116,99,104,32,45,45,62,10,32,
32,60,116,105,116,108,101,10,32,32,32,32,32,105,100,61,34,116,105,116,108,101,50,34,62,115,116,97,116,115,32,91,35,49,49,57,48,93,60,47,116,105,116,108,101,62,10,32,32,60,100,101,115,99,10,32,32,32,32,
32,105,100,61,34,100,101,115,99,52,34,62,67,114,101,97,116,101,100,32,119,105,116,104,32,83,107,101,116,99,104,46,60,47,100,101,115,99,62,10,32,32,60,100,101,102,115,10,32,32,32,32,32,105,100,61,34,100,
101,102,115,54,34,32,47,62,10,32,32,60,103,10,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,45,49,52,48,44,45,50,56,51,57,41,34,10,32,32,32,32,32,105,100,
61,34,68,114,105,98,98,98,108,101,45,76,105,103,104,116,45,80,114,101,118,105,101,119,34,10,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,102,102,102,102,102,102,59,102,105,108,108,45,
114,117,108,101,58,101,118,101,110,111,100,100,59,115,116,114,111,107,101,58,110,111,110,101,59,115,116,114,111,107,101,45,119,105,100,116,104,58,49,59,102,105,108,108,45,111,112,97,99,105,116,121,58,
48,46,57,57,55,50,56,57,57,54,34,62,10,32,32,32,32,60,103,10,32,32,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,53,54,44,49,54,48,41,34,10,32,32,32,32,32,
32,32,105,100,61,34,105,99,111,110,115,34,10,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,102,102,102,102,102,102,59,102,105,108,108,45,111,112,97,99,105,116,121,58,48,46,57,57,
55,50,56,57,57,54,34,62,10,32,32,32,32,32,32,60,112,97,116,104,10,32,32,32,32,32,32,32,32,32,115,111,100,105,112,111,100,105,58,110,111,100,101,116,121,112,101,115,61,34,99,99,99,99,99,99,99,99,99,99,
99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,34,10,32,32,32,32,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,111,110,110,101,99,116,111,114,45,99,117,114,118,97,116,117,114,101,61,34,48,34,10,32,
32,32,32,32,32,32,32,32,105,100,61,34,115,116,97,116,115,45,91,35,49,49,57,48,93,34,10,32,32,32,32,32,32,32,32,32,100,61,34,109,32,49,48,50,44,50,54,56,56,32,118,32,50,32,72,32,56,54,32,99,32,45,49,46,
49,53,54,44,45,49,46,57,55,49,32,48,46,50,48,54,44,48,46,51,53,32,48,44,48,32,118,32,48,32,48,32,48,32,45,50,32,45,55,32,104,32,49,54,32,122,32,109,32,48,44,57,32,72,32,56,54,32,118,32,45,55,32,48,32,
48,32,99,32,50,46,55,50,51,44,52,46,54,53,52,32,45,49,46,50,57,52,44,45,50,46,50,50,50,32,48,44,48,32,104,32,49,54,32,118,32,48,32,122,32,109,32,45,49,56,44,50,32,104,32,50,48,32,118,32,45,50,48,32,72,
32,56,52,32,90,34,10,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,102,102,102,102,102,102,59,102,105,108,108,45,111,112,97,99,105,116,121,58,48,46,57,57,55,50,56,57,57,54,
34,32,47,62,10,32,32,32,32,60,47,103,62,10,32,32,60,47,103,62,10,32,32,60,112,97,116,104,10,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,110,111,110,101,59,115,116,114,111,107,101,58,35,
101,52,101,52,101,52,59,115,116,114,111,107,101,45,119,105,100,116,104,58,50,59,115,116,114,111,107,101,45,108,105,110,101,99,97,112,58,114,111,117,110,100,59,115,116,114,111,107,101,45,108,105,110,101,
106,111,105,110,58,114,111,117,110,100,59,115,116,114,111,107,101,45,111,112,97,99,105,116,121,58,49,59,115,116,114,111,107,101,45,109,105,116,101,114,108,105,109,105,116,58,52,59,115,116,114,111,107,
101,45,100,97,115,104,97,114,114,97,121,58,110,111,110,101,34,10,32,32,32,32,32,100,61,34,77,32,51,46,54,56,54,52,52,48,55,44,49,48,46,49,57,48,54,55,56,32,49,48,46,48,56,52,55,52,54,44,53,46,53,57,51,
50,50,48,52,32,118,32,57,46,49,51,49,51,53,53,54,32,108,32,54,46,51,49,51,53,53,57,44,45,52,46,56,55,50,56,56,48,56,34,10,32,32,32,32,32,105,100,61,34,112,97,116,104,49,52,51,54,34,10,32,32,32,32,32,105,
110,107,115,99,97,112,101,58,99,111,110,110,101,99,116,111,114,45,99,117,114,118,97,116,117,114,101,61,34,48,34,32,47,62,10,60,47,115,118,103,62,10,0,0};

const char* VASTWaveTableEditorViewHeader::waves_saw_svg = (const char*) resource_VASTWaveTableEditorViewHeader_waves_saw_svg;
const int VASTWaveTableEditorViewHeader::waves_saw_svgSize = 2662;

// JUCER_RESOURCE: waves_sinc_svg, 3208, "../../Resources/Icons/waves_sinc.svg"
static const unsigned char resource_VASTWaveTableEditorViewHeader_waves_sinc_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,
70,45,56,34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,10,32,32,32,120,109,108,110,115,58,100,99,61,34,104,116,116,112,58,47,47,112,117,114,108,46,111,114,103,47,
100,99,47,101,108,101,109,101,110,116,115,47,49,46,49,47,34,10,32,32,32,120,109,108,110,115,58,99,99,61,34,104,116,116,112,58,47,47,99,114,101,97,116,105,118,101,99,111,109,109,111,110,115,46,111,114,
103,47,110,115,35,34,10,32,32,32,120,109,108,110,115,58,114,100,102,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,48,50,47,50,50,45,114,100,102,45,115,121,110,116,
97,120,45,110,115,35,34,10,32,32,32,120,109,108,110,115,58,115,118,103,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,118,103,34,10,32,32,32,120,109,108,110,
115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,118,103,34,10,32,32,32,120,109,108,110,115,58,115,111,100,105,112,111,100,105,61,34,104,116,116,112,58,47,
47,115,111,100,105,112,111,100,105,46,115,111,117,114,99,101,102,111,114,103,101,46,110,101,116,47,68,84,68,47,115,111,100,105,112,111,100,105,45,48,46,100,116,100,34,10,32,32,32,120,109,108,110,115,58,
105,110,107,115,99,97,112,101,61,34,104,116,116,112,58,47,47,119,119,119,46,105,110,107,115,99,97,112,101,46,111,114,103,47,110,97,109,101,115,112,97,99,101,115,47,105,110,107,115,99,97,112,101,34,10,
32,32,32,119,105,100,116,104,61,34,50,48,112,120,34,10,32,32,32,104,101,105,103,104,116,61,34,50,48,112,120,34,10,32,32,32,118,105,101,119,66,111,120,61,34,48,32,48,32,50,48,32,50,48,34,10,32,32,32,118,
101,114,115,105,111,110,61,34,49,46,49,34,10,32,32,32,105,100,61,34,115,118,103,49,50,34,10,32,32,32,115,111,100,105,112,111,100,105,58,100,111,99,110,97,109,101,61,34,119,97,118,101,115,95,115,105,110,
99,46,115,118,103,34,10,32,32,32,105,110,107,115,99,97,112,101,58,118,101,114,115,105,111,110,61,34,48,46,57,50,46,51,32,40,50,52,48,53,53,52,54,44,32,50,48,49,56,45,48,51,45,49,49,41,34,62,10,32,32,60,
109,101,116,97,100,97,116,97,10,32,32,32,32,32,105,100,61,34,109,101,116,97,100,97,116,97,49,54,34,62,10,32,32,32,32,60,114,100,102,58,82,68,70,62,10,32,32,32,32,32,32,60,99,99,58,87,111,114,107,10,32,
32,32,32,32,32,32,32,32,114,100,102,58,97,98,111,117,116,61,34,34,62,10,32,32,32,32,32,32,32,32,60,100,99,58,102,111,114,109,97,116,62,105,109,97,103,101,47,115,118,103,43,120,109,108,60,47,100,99,58,
102,111,114,109,97,116,62,10,32,32,32,32,32,32,32,32,60,100,99,58,116,121,112,101,10,32,32,32,32,32,32,32,32,32,32,32,114,100,102,58,114,101,115,111,117,114,99,101,61,34,104,116,116,112,58,47,47,112,117,
114,108,46,111,114,103,47,100,99,47,100,99,109,105,116,121,112,101,47,83,116,105,108,108,73,109,97,103,101,34,32,47,62,10,32,32,32,32,32,32,32,32,60,100,99,58,116,105,116,108,101,62,115,116,97,116,115,
32,91,35,49,49,57,48,93,60,47,100,99,58,116,105,116,108,101,62,10,32,32,32,32,32,32,60,47,99,99,58,87,111,114,107,62,10,32,32,32,32,60,47,114,100,102,58,82,68,70,62,10,32,32,60,47,109,101,116,97,100,97,
116,97,62,10,32,32,60,115,111,100,105,112,111,100,105,58,110,97,109,101,100,118,105,101,119,10,32,32,32,32,32,112,97,103,101,99,111,108,111,114,61,34,35,57,53,57,53,57,53,34,10,32,32,32,32,32,98,111,114,
100,101,114,99,111,108,111,114,61,34,35,54,54,54,54,54,54,34,10,32,32,32,32,32,98,111,114,100,101,114,111,112,97,99,105,116,121,61,34,49,34,10,32,32,32,32,32,111,98,106,101,99,116,116,111,108,101,114,
97,110,99,101,61,34,49,48,34,10,32,32,32,32,32,103,114,105,100,116,111,108,101,114,97,110,99,101,61,34,49,48,34,10,32,32,32,32,32,103,117,105,100,101,116,111,108,101,114,97,110,99,101,61,34,49,48,34,10,
32,32,32,32,32,105,110,107,115,99,97,112,101,58,112,97,103,101,111,112,97,99,105,116,121,61,34,49,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,112,97,103,101,115,104,97,100,111,119,61,34,50,34,
10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,119,105,100,116,104,61,34,51,56,52,48,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,104,
101,105,103,104,116,61,34,50,48,53,48,34,10,32,32,32,32,32,105,100,61,34,110,97,109,101,100,118,105,101,119,49,52,34,10,32,32,32,32,32,115,104,111,119,103,114,105,100,61,34,102,97,108,115,101,34,10,32,
32,32,32,32,105,110,107,115,99,97,112,101,58,122,111,111,109,61,34,52,55,46,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,120,61,34,49,46,52,51,55,55,56,53,51,34,10,32,32,32,32,32,105,110,
107,115,99,97,112,101,58,99,121,61,34,54,46,52,53,57,53,57,56,51,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,120,61,34,45,49,50,34,10,32,32,32,32,32,105,110,107,115,
99,97,112,101,58,119,105,110,100,111,119,45,121,61,34,45,49,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,109,97,120,105,109,105,122,101,100,61,34,49,34,10,32,32,
32,32,32,105,110,107,115,99,97,112,101,58,99,117,114,114,101,110,116,45,108,97,121,101,114,61,34,115,118,103,49,50,34,10,32,32,32,32,32,115,104,111,119,103,117,105,100,101,115,61,34,102,97,108,115,101,
34,32,47,62,10,32,32,60,33,45,45,32,71,101,110,101,114,97,116,111,114,58,32,83,107,101,116,99,104,32,51,46,56,46,49,32,40,50,57,54,56,55,41,32,45,32,104,116,116,112,58,47,47,119,119,119,46,98,111,104,
101,109,105,97,110,99,111,100,105,110,103,46,99,111,109,47,115,107,101,116,99,104,32,45,45,62,10,32,32,60,116,105,116,108,101,10,32,32,32,32,32,105,100,61,34,116,105,116,108,101,50,34,62,115,116,97,116,
115,32,91,35,49,49,57,48,93,60,47,116,105,116,108,101,62,10,32,32,60,100,101,115,99,10,32,32,32,32,32,105,100,61,34,100,101,115,99,52,34,62,67,114,101,97,116,101,100,32,119,105,116,104,32,83,107,101,116,
99,104,46,60,47,100,101,115,99,62,10,32,32,60,100,101,102,115,10,32,32,32,32,32,105,100,61,34,100,101,102,115,54,34,32,47,62,10,32,32,60,103,10,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,
116,114,97,110,115,108,97,116,101,40,45,49,52,48,44,45,50,56,51,57,41,34,10,32,32,32,32,32,105,100,61,34,68,114,105,98,98,98,108,101,45,76,105,103,104,116,45,80,114,101,118,105,101,119,34,10,32,32,32,
32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,102,102,102,102,102,102,59,102,105,108,108,45,114,117,108,101,58,101,118,101,110,111,100,100,59,115,116,114,111,107,101,58,110,111,110,101,59,115,
116,114,111,107,101,45,119,105,100,116,104,58,49,59,102,105,108,108,45,111,112,97,99,105,116,121,58,48,46,57,57,55,50,56,57,57,54,34,62,10,32,32,32,32,60,103,10,32,32,32,32,32,32,32,116,114,97,110,115,
102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,53,54,44,49,54,48,41,34,10,32,32,32,32,32,32,32,105,100,61,34,105,99,111,110,115,34,10,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,105,
108,108,58,35,102,102,102,102,102,102,59,102,105,108,108,45,111,112,97,99,105,116,121,58,48,46,57,57,55,50,56,57,57,54,34,62,10,32,32,32,32,32,32,60,112,97,116,104,10,32,32,32,32,32,32,32,32,32,115,111,
100,105,112,111,100,105,58,110,111,100,101,116,121,112,101,115,61,34,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,34,10,32,32,32,32,32,32,32,32,32,105,110,107,115,99,97,112,
101,58,99,111,110,110,101,99,116,111,114,45,99,117,114,118,97,116,117,114,101,61,34,48,34,10,32,32,32,32,32,32,32,32,32,105,100,61,34,115,116,97,116,115,45,91,35,49,49,57,48,93,34,10,32,32,32,32,32,32,
32,32,32,100,61,34,109,32,49,48,50,44,50,54,56,56,32,118,32,50,32,72,32,56,54,32,99,32,45,49,46,49,53,54,44,45,49,46,57,55,49,32,48,46,50,48,54,44,48,46,51,53,32,48,44,48,32,118,32,48,32,48,32,48,32,45,
50,32,45,55,32,104,32,49,54,32,122,32,109,32,48,44,57,32,72,32,56,54,32,118,32,45,55,32,48,32,48,32,99,32,50,46,55,50,51,44,52,46,54,53,52,32,45,49,46,50,57,52,44,45,50,46,50,50,50,32,48,44,48,32,104,
32,49,54,32,118,32,48,32,122,32,109,32,45,49,56,44,50,32,104,32,50,48,32,118,32,45,50,48,32,72,32,56,52,32,90,34,10,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,102,102,102,
102,102,102,59,102,105,108,108,45,111,112,97,99,105,116,121,58,48,46,57,57,55,50,56,57,57,54,34,32,47,62,10,32,32,32,32,60,47,103,62,10,32,32,60,47,103,62,10,32,32,60,112,97,116,104,10,32,32,32,32,32,
115,116,121,108,101,61,34,102,105,108,108,58,110,111,110,101,59,115,116,114,111,107,101,58,35,102,102,102,102,102,102,59,115,116,114,111,107,101,45,119,105,100,116,104,58,49,59,115,116,114,111,107,101,
45,108,105,110,101,99,97,112,58,98,117,116,116,59,115,116,114,111,107,101,45,108,105,110,101,106,111,105,110,58,109,105,116,101,114,59,115,116,114,111,107,101,45,109,105,116,101,114,108,105,109,105,116,
58,52,59,115,116,114,111,107,101,45,100,97,115,104,97,114,114,97,121,58,110,111,110,101,59,115,116,114,111,107,101,45,111,112,97,99,105,116,121,58,49,34,10,32,32,32,32,32,100,61,34,109,32,49,54,46,54,
49,48,54,52,55,44,49,49,46,49,48,49,54,57,53,32,99,32,48,44,48,32,51,101,45,54,44,48,46,50,55,53,52,50,32,45,48,46,51,55,49,53,51,44,49,46,53,56,56,57,56,51,32,45,48,46,51,55,49,53,51,49,44,49,46,51,49,
51,53,53,57,32,45,49,46,50,50,48,55,52,54,44,45,48,46,54,49,52,52,48,54,32,45,49,46,57,49,48,55,51,50,44,45,51,46,56,55,55,49,49,56,55,32,45,48,46,54,56,57,57,56,54,44,45,51,46,50,54,50,55,49,49,57,32,
45,49,46,51,55,57,57,55,50,44,51,46,49,51,53,53,57,51,55,32,45,49,46,57,50,56,52,50,51,44,54,46,51,49,51,53,53,57,55,32,45,48,46,53,52,56,52,53,44,51,46,49,55,55,57,54,54,32,45,49,46,51,52,52,53,56,56,
44,45,51,46,50,50,48,51,51,57,32,45,49,46,51,52,52,53,56,56,44,45,51,46,50,50,48,51,51,57,32,76,32,49,48,46,53,53,57,57,57,57,44,54,46,53,48,52,50,51,55,51,32,67,32,57,46,57,49,54,50,56,49,50,44,45,48,
46,53,49,54,49,50,52,48,51,32,56,46,54,56,52,54,53,49,56,44,49,49,46,54,55,51,55,50,56,32,56,46,54,56,52,54,53,49,56,44,49,49,46,54,55,51,55,50,56,32,99,32,48,44,48,32,45,49,46,49,51,50,50,56,53,50,44,
54,46,56,52,51,50,50,49,32,45,49,46,54,50,55,54,54,44,51,46,51,50,54,50,55,50,32,67,32,54,46,53,54,49,54,49,55,49,44,49,49,46,52,56,51,48,53,49,32,54,46,50,57,54,50,51,55,56,44,53,46,57,49,49,48,49,54,
57,32,53,46,54,48,54,50,53,49,53,44,57,46,48,54,55,55,57,54,54,32,52,46,57,49,54,50,54,53,50,44,49,50,46,50,50,52,53,55,54,32,52,46,56,57,56,53,55,51,50,44,49,50,46,50,50,52,53,55,54,32,52,46,56,57,56,
53,55,51,50,44,49,50,46,50,50,52,53,55,54,32,99,32,48,44,48,32,45,48,46,56,51,49,55,57,50,52,44,50,46,55,53,51,56,56,56,32,45,49,46,50,51,56,52,51,54,57,44,48,46,54,51,53,53,57,52,32,76,32,51,46,51,48,
54,50,57,55,50,44,49,49,46,48,49,54,57,52,57,34,10,32,32,32,32,32,105,100,61,34,112,97,116,104,50,48,49,53,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,111,110,110,101,99,116,111,114,45,99,
117,114,118,97,116,117,114,101,61,34,48,34,10,32,32,32,32,32,115,111,100,105,112,111,100,105,58,110,111,100,101,116,121,112,101,115,61,34,99,99,115,115,99,115,99,115,115,99,115,99,34,32,47,62,10,60,47,
115,118,103,62,10,0,0};

const char* VASTWaveTableEditorViewHeader::waves_sinc_svg = (const char*) resource_VASTWaveTableEditorViewHeader_waves_sinc_svg;
const int VASTWaveTableEditorViewHeader::waves_sinc_svgSize = 3208;

// JUCER_RESOURCE: waves_sine_svg, 2805, "../../Resources/Icons/waves_sine.svg"
static const unsigned char resource_VASTWaveTableEditorViewHeader_waves_sine_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,
70,45,56,34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,10,32,32,32,120,109,108,110,115,58,100,99,61,34,104,116,116,112,58,47,47,112,117,114,108,46,111,114,103,47,
100,99,47,101,108,101,109,101,110,116,115,47,49,46,49,47,34,10,32,32,32,120,109,108,110,115,58,99,99,61,34,104,116,116,112,58,47,47,99,114,101,97,116,105,118,101,99,111,109,109,111,110,115,46,111,114,
103,47,110,115,35,34,10,32,32,32,120,109,108,110,115,58,114,100,102,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,48,50,47,50,50,45,114,100,102,45,115,121,110,116,
97,120,45,110,115,35,34,10,32,32,32,120,109,108,110,115,58,115,118,103,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,118,103,34,10,32,32,32,120,109,108,110,
115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,118,103,34,10,32,32,32,120,109,108,110,115,58,115,111,100,105,112,111,100,105,61,34,104,116,116,112,58,47,
47,115,111,100,105,112,111,100,105,46,115,111,117,114,99,101,102,111,114,103,101,46,110,101,116,47,68,84,68,47,115,111,100,105,112,111,100,105,45,48,46,100,116,100,34,10,32,32,32,120,109,108,110,115,58,
105,110,107,115,99,97,112,101,61,34,104,116,116,112,58,47,47,119,119,119,46,105,110,107,115,99,97,112,101,46,111,114,103,47,110,97,109,101,115,112,97,99,101,115,47,105,110,107,115,99,97,112,101,34,10,
32,32,32,119,105,100,116,104,61,34,50,48,112,120,34,10,32,32,32,104,101,105,103,104,116,61,34,50,48,112,120,34,10,32,32,32,118,105,101,119,66,111,120,61,34,48,32,48,32,50,48,32,50,48,34,10,32,32,32,118,
101,114,115,105,111,110,61,34,49,46,49,34,10,32,32,32,105,100,61,34,115,118,103,49,50,34,10,32,32,32,115,111,100,105,112,111,100,105,58,100,111,99,110,97,109,101,61,34,119,97,118,101,115,95,115,105,110,
101,46,115,118,103,34,10,32,32,32,105,110,107,115,99,97,112,101,58,118,101,114,115,105,111,110,61,34,48,46,57,50,46,51,32,40,50,52,48,53,53,52,54,44,32,50,48,49,56,45,48,51,45,49,49,41,34,62,10,32,32,
60,109,101,116,97,100,97,116,97,10,32,32,32,32,32,105,100,61,34,109,101,116,97,100,97,116,97,49,54,34,62,10,32,32,32,32,60,114,100,102,58,82,68,70,62,10,32,32,32,32,32,32,60,99,99,58,87,111,114,107,10,
32,32,32,32,32,32,32,32,32,114,100,102,58,97,98,111,117,116,61,34,34,62,10,32,32,32,32,32,32,32,32,60,100,99,58,102,111,114,109,97,116,62,105,109,97,103,101,47,115,118,103,43,120,109,108,60,47,100,99,
58,102,111,114,109,97,116,62,10,32,32,32,32,32,32,32,32,60,100,99,58,116,121,112,101,10,32,32,32,32,32,32,32,32,32,32,32,114,100,102,58,114,101,115,111,117,114,99,101,61,34,104,116,116,112,58,47,47,112,
117,114,108,46,111,114,103,47,100,99,47,100,99,109,105,116,121,112,101,47,83,116,105,108,108,73,109,97,103,101,34,32,47,62,10,32,32,32,32,32,32,32,32,60,100,99,58,116,105,116,108,101,62,115,116,97,116,
115,32,91,35,49,49,57,48,93,60,47,100,99,58,116,105,116,108,101,62,10,32,32,32,32,32,32,60,47,99,99,58,87,111,114,107,62,10,32,32,32,32,60,47,114,100,102,58,82,68,70,62,10,32,32,60,47,109,101,116,97,100,
97,116,97,62,10,32,32,60,115,111,100,105,112,111,100,105,58,110,97,109,101,100,118,105,101,119,10,32,32,32,32,32,112,97,103,101,99,111,108,111,114,61,34,35,57,53,57,53,57,53,34,10,32,32,32,32,32,98,111,
114,100,101,114,99,111,108,111,114,61,34,35,54,54,54,54,54,54,34,10,32,32,32,32,32,98,111,114,100,101,114,111,112,97,99,105,116,121,61,34,49,34,10,32,32,32,32,32,111,98,106,101,99,116,116,111,108,101,
114,97,110,99,101,61,34,49,48,34,10,32,32,32,32,32,103,114,105,100,116,111,108,101,114,97,110,99,101,61,34,49,48,34,10,32,32,32,32,32,103,117,105,100,101,116,111,108,101,114,97,110,99,101,61,34,49,48,
34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,112,97,103,101,111,112,97,99,105,116,121,61,34,49,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,112,97,103,101,115,104,97,100,111,119,61,34,
50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,119,105,100,116,104,61,34,51,56,52,48,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,
45,104,101,105,103,104,116,61,34,50,48,53,48,34,10,32,32,32,32,32,105,100,61,34,110,97,109,101,100,118,105,101,119,49,52,34,10,32,32,32,32,32,115,104,111,119,103,114,105,100,61,34,102,97,108,115,101,34,
10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,122,111,111,109,61,34,52,55,46,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,120,61,34,49,46,52,51,55,55,56,53,51,34,10,32,32,32,32,32,
105,110,107,115,99,97,112,101,58,99,121,61,34,54,46,52,53,57,53,57,56,51,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,120,61,34,45,49,50,34,10,32,32,32,32,32,105,110,
107,115,99,97,112,101,58,119,105,110,100,111,119,45,121,61,34,45,49,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,109,97,120,105,109,105,122,101,100,61,34,49,34,10,
32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,117,114,114,101,110,116,45,108,97,121,101,114,61,34,115,118,103,49,50,34,32,47,62,10,32,32,60,33,45,45,32,71,101,110,101,114,97,116,111,114,58,32,83,
107,101,116,99,104,32,51,46,56,46,49,32,40,50,57,54,56,55,41,32,45,32,104,116,116,112,58,47,47,119,119,119,46,98,111,104,101,109,105,97,110,99,111,100,105,110,103,46,99,111,109,47,115,107,101,116,99,104,
32,45,45,62,10,32,32,60,116,105,116,108,101,10,32,32,32,32,32,105,100,61,34,116,105,116,108,101,50,34,62,115,116,97,116,115,32,91,35,49,49,57,48,93,60,47,116,105,116,108,101,62,10,32,32,60,100,101,115,
99,10,32,32,32,32,32,105,100,61,34,100,101,115,99,52,34,62,67,114,101,97,116,101,100,32,119,105,116,104,32,83,107,101,116,99,104,46,60,47,100,101,115,99,62,10,32,32,60,100,101,102,115,10,32,32,32,32,32,
105,100,61,34,100,101,102,115,54,34,32,47,62,10,32,32,60,103,10,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,45,49,52,48,44,45,50,56,51,57,41,34,10,32,32,
32,32,32,105,100,61,34,68,114,105,98,98,98,108,101,45,76,105,103,104,116,45,80,114,101,118,105,101,119,34,10,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,102,102,102,102,102,102,59,102,
105,108,108,45,114,117,108,101,58,101,118,101,110,111,100,100,59,115,116,114,111,107,101,58,110,111,110,101,59,115,116,114,111,107,101,45,119,105,100,116,104,58,49,59,102,105,108,108,45,111,112,97,99,
105,116,121,58,48,46,57,57,55,50,56,57,57,54,34,62,10,32,32,32,32,60,103,10,32,32,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,53,54,44,49,54,48,41,34,10,
32,32,32,32,32,32,32,105,100,61,34,105,99,111,110,115,34,10,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,102,102,102,102,102,102,59,102,105,108,108,45,111,112,97,99,105,116,121,
58,48,46,57,57,55,50,56,57,57,54,34,62,10,32,32,32,32,32,32,60,112,97,116,104,10,32,32,32,32,32,32,32,32,32,115,111,100,105,112,111,100,105,58,110,111,100,101,116,121,112,101,115,61,34,99,99,99,99,99,
99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,34,10,32,32,32,32,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,111,110,110,101,99,116,111,114,45,99,117,114,118,97,116,117,114,101,61,
34,48,34,10,32,32,32,32,32,32,32,32,32,105,100,61,34,115,116,97,116,115,45,91,35,49,49,57,48,93,34,10,32,32,32,32,32,32,32,32,32,100,61,34,109,32,49,48,50,44,50,54,56,56,32,118,32,50,32,72,32,56,54,32,
99,32,45,49,46,49,53,54,44,45,49,46,57,55,49,32,48,46,50,48,54,44,48,46,51,53,32,48,44,48,32,118,32,48,32,48,32,48,32,45,50,32,45,55,32,104,32,49,54,32,122,32,109,32,48,44,57,32,72,32,56,54,32,118,32,
45,55,32,48,32,48,32,99,32,50,46,55,50,51,44,52,46,54,53,52,32,45,49,46,50,57,52,44,45,50,46,50,50,50,32,48,44,48,32,104,32,49,54,32,118,32,48,32,122,32,109,32,45,49,56,44,50,32,104,32,50,48,32,118,32,
45,50,48,32,72,32,56,52,32,90,34,10,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,102,102,102,102,102,102,59,102,105,108,108,45,111,112,97,99,105,116,121,58,48,46,57,57,55,
50,56,57,57,54,34,32,47,62,10,32,32,32,32,60,47,103,62,10,32,32,60,47,103,62,10,32,32,60,112,97,116,104,10,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,110,111,110,101,59,115,116,114,111,
107,101,58,35,102,98,102,98,102,98,59,115,116,114,111,107,101,45,119,105,100,116,104,58,50,59,115,116,114,111,107,101,45,108,105,110,101,99,97,112,58,114,111,117,110,100,59,115,116,114,111,107,101,45,
108,105,110,101,106,111,105,110,58,114,111,117,110,100,59,115,116,114,111,107,101,45,111,112,97,99,105,116,121,58,49,59,115,116,114,111,107,101,45,109,105,116,101,114,108,105,109,105,116,58,52,59,115,
116,114,111,107,101,45,100,97,115,104,97,114,114,97,121,58,110,111,110,101,34,10,32,32,32,32,32,100,61,34,109,32,51,46,55,55,49,49,56,54,52,44,49,48,46,49,50,55,49,49,57,32,99,32,48,44,48,32,50,46,53,
44,45,56,46,49,51,53,53,57,51,54,32,52,46,49,57,52,57,49,53,51,44,45,52,46,48,54,55,55,57,55,32,49,46,54,57,52,57,49,53,50,44,52,46,48,54,55,55,57,55,32,49,46,55,51,55,50,56,56,49,44,52,46,49,49,48,49,
55,32,49,46,55,51,55,50,56,56,49,44,52,46,49,49,48,49,55,32,48,44,48,32,51,46,48,50,57,54,54,49,50,44,56,46,50,54,50,55,49,49,32,52,46,55,48,51,51,57,48,50,44,52,46,49,51,49,51,53,53,32,49,46,54,55,51,
55,50,56,44,45,52,46,49,51,49,51,53,53,32,49,46,54,55,51,55,50,56,44,45,52,46,49,51,49,51,53,53,32,49,46,54,55,51,55,50,56,44,45,52,46,49,51,49,51,53,53,34,10,32,32,32,32,32,105,100,61,34,112,97,116,104,
50,48,48,53,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,111,110,110,101,99,116,111,114,45,99,117,114,118,97,116,117,114,101,61,34,48,34,32,47,62,10,60,47,115,118,103,62,10,0,0};

const char* VASTWaveTableEditorViewHeader::waves_sine_svg = (const char*) resource_VASTWaveTableEditorViewHeader_waves_sine_svg;
const int VASTWaveTableEditorViewHeader::waves_sine_svgSize = 2805;

// JUCER_RESOURCE: waves_square_svg, 3000, "../../Resources/Icons/waves_square.svg"
static const unsigned char resource_VASTWaveTableEditorViewHeader_waves_square_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,
70,45,56,34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,10,32,32,32,120,109,108,110,115,58,100,99,61,34,104,116,116,112,58,47,47,112,117,114,108,46,111,114,103,47,
100,99,47,101,108,101,109,101,110,116,115,47,49,46,49,47,34,10,32,32,32,120,109,108,110,115,58,99,99,61,34,104,116,116,112,58,47,47,99,114,101,97,116,105,118,101,99,111,109,109,111,110,115,46,111,114,
103,47,110,115,35,34,10,32,32,32,120,109,108,110,115,58,114,100,102,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,48,50,47,50,50,45,114,100,102,45,115,121,110,116,
97,120,45,110,115,35,34,10,32,32,32,120,109,108,110,115,58,115,118,103,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,118,103,34,10,32,32,32,120,109,108,110,
115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,118,103,34,10,32,32,32,120,109,108,110,115,58,115,111,100,105,112,111,100,105,61,34,104,116,116,112,58,47,
47,115,111,100,105,112,111,100,105,46,115,111,117,114,99,101,102,111,114,103,101,46,110,101,116,47,68,84,68,47,115,111,100,105,112,111,100,105,45,48,46,100,116,100,34,10,32,32,32,120,109,108,110,115,58,
105,110,107,115,99,97,112,101,61,34,104,116,116,112,58,47,47,119,119,119,46,105,110,107,115,99,97,112,101,46,111,114,103,47,110,97,109,101,115,112,97,99,101,115,47,105,110,107,115,99,97,112,101,34,10,
32,32,32,119,105,100,116,104,61,34,50,48,112,120,34,10,32,32,32,104,101,105,103,104,116,61,34,50,48,112,120,34,10,32,32,32,118,105,101,119,66,111,120,61,34,48,32,48,32,50,48,32,50,48,34,10,32,32,32,118,
101,114,115,105,111,110,61,34,49,46,49,34,10,32,32,32,105,100,61,34,115,118,103,49,50,34,10,32,32,32,115,111,100,105,112,111,100,105,58,100,111,99,110,97,109,101,61,34,119,97,118,101,115,95,115,113,117,
97,114,101,46,115,118,103,34,10,32,32,32,105,110,107,115,99,97,112,101,58,118,101,114,115,105,111,110,61,34,48,46,57,50,46,51,32,40,50,52,48,53,53,52,54,44,32,50,48,49,56,45,48,51,45,49,49,41,34,62,10,
32,32,60,109,101,116,97,100,97,116,97,10,32,32,32,32,32,105,100,61,34,109,101,116,97,100,97,116,97,49,54,34,62,10,32,32,32,32,60,114,100,102,58,82,68,70,62,10,32,32,32,32,32,32,60,99,99,58,87,111,114,
107,10,32,32,32,32,32,32,32,32,32,114,100,102,58,97,98,111,117,116,61,34,34,62,10,32,32,32,32,32,32,32,32,60,100,99,58,102,111,114,109,97,116,62,105,109,97,103,101,47,115,118,103,43,120,109,108,60,47,
100,99,58,102,111,114,109,97,116,62,10,32,32,32,32,32,32,32,32,60,100,99,58,116,121,112,101,10,32,32,32,32,32,32,32,32,32,32,32,114,100,102,58,114,101,115,111,117,114,99,101,61,34,104,116,116,112,58,47,
47,112,117,114,108,46,111,114,103,47,100,99,47,100,99,109,105,116,121,112,101,47,83,116,105,108,108,73,109,97,103,101,34,32,47,62,10,32,32,32,32,32,32,32,32,60,100,99,58,116,105,116,108,101,62,115,116,
97,116,115,32,91,35,49,49,57,48,93,60,47,100,99,58,116,105,116,108,101,62,10,32,32,32,32,32,32,60,47,99,99,58,87,111,114,107,62,10,32,32,32,32,60,47,114,100,102,58,82,68,70,62,10,32,32,60,47,109,101,116,
97,100,97,116,97,62,10,32,32,60,115,111,100,105,112,111,100,105,58,110,97,109,101,100,118,105,101,119,10,32,32,32,32,32,112,97,103,101,99,111,108,111,114,61,34,35,57,53,57,53,57,53,34,10,32,32,32,32,32,
98,111,114,100,101,114,99,111,108,111,114,61,34,35,54,54,54,54,54,54,34,10,32,32,32,32,32,98,111,114,100,101,114,111,112,97,99,105,116,121,61,34,49,34,10,32,32,32,32,32,111,98,106,101,99,116,116,111,108,
101,114,97,110,99,101,61,34,49,48,34,10,32,32,32,32,32,103,114,105,100,116,111,108,101,114,97,110,99,101,61,34,49,48,34,10,32,32,32,32,32,103,117,105,100,101,116,111,108,101,114,97,110,99,101,61,34,49,
48,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,112,97,103,101,111,112,97,99,105,116,121,61,34,49,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,112,97,103,101,115,104,97,100,111,119,61,
34,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,119,105,100,116,104,61,34,51,56,52,48,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,
45,104,101,105,103,104,116,61,34,50,48,53,48,34,10,32,32,32,32,32,105,100,61,34,110,97,109,101,100,118,105,101,119,49,52,34,10,32,32,32,32,32,115,104,111,119,103,114,105,100,61,34,102,97,108,115,101,34,
10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,122,111,111,109,61,34,52,55,46,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,120,61,34,54,46,56,48,56,53,52,56,34,10,32,32,32,32,32,105,
110,107,115,99,97,112,101,58,99,121,61,34,54,46,52,53,57,53,57,56,51,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,120,61,34,45,49,50,34,10,32,32,32,32,32,105,110,107,
115,99,97,112,101,58,119,105,110,100,111,119,45,121,61,34,45,49,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,109,97,120,105,109,105,122,101,100,61,34,49,34,10,32,
32,32,32,32,105,110,107,115,99,97,112,101,58,99,117,114,114,101,110,116,45,108,97,121,101,114,61,34,115,118,103,49,50,34,32,47,62,10,32,32,60,33,45,45,32,71,101,110,101,114,97,116,111,114,58,32,83,107,
101,116,99,104,32,51,46,56,46,49,32,40,50,57,54,56,55,41,32,45,32,104,116,116,112,58,47,47,119,119,119,46,98,111,104,101,109,105,97,110,99,111,100,105,110,103,46,99,111,109,47,115,107,101,116,99,104,32,
45,45,62,10,32,32,60,116,105,116,108,101,10,32,32,32,32,32,105,100,61,34,116,105,116,108,101,50,34,62,115,116,97,116,115,32,91,35,49,49,57,48,93,60,47,116,105,116,108,101,62,10,32,32,60,100,101,115,99,
10,32,32,32,32,32,105,100,61,34,100,101,115,99,52,34,62,67,114,101,97,116,101,100,32,119,105,116,104,32,83,107,101,116,99,104,46,60,47,100,101,115,99,62,10,32,32,60,100,101,102,115,10,32,32,32,32,32,105,
100,61,34,100,101,102,115,54,34,32,47,62,10,32,32,60,103,10,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,45,49,52,48,44,45,50,56,51,57,41,34,10,32,32,32,
32,32,105,100,61,34,68,114,105,98,98,98,108,101,45,76,105,103,104,116,45,80,114,101,118,105,101,119,34,10,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,102,102,102,102,102,102,59,102,
105,108,108,45,114,117,108,101,58,101,118,101,110,111,100,100,59,115,116,114,111,107,101,58,110,111,110,101,59,115,116,114,111,107,101,45,119,105,100,116,104,58,49,59,102,105,108,108,45,111,112,97,99,
105,116,121,58,48,46,57,57,55,50,56,57,57,54,34,62,10,32,32,32,32,60,103,10,32,32,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,53,54,44,49,54,48,41,34,10,
32,32,32,32,32,32,32,105,100,61,34,105,99,111,110,115,34,10,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,102,102,102,102,102,102,59,102,105,108,108,45,111,112,97,99,105,116,121,
58,48,46,57,57,55,50,56,57,57,54,34,62,10,32,32,32,32,32,32,60,112,97,116,104,10,32,32,32,32,32,32,32,32,32,115,111,100,105,112,111,100,105,58,110,111,100,101,116,121,112,101,115,61,34,99,99,99,99,99,
99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,34,10,32,32,32,32,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,111,110,110,101,99,116,111,114,45,99,117,114,118,97,116,117,114,101,61,
34,48,34,10,32,32,32,32,32,32,32,32,32,105,100,61,34,115,116,97,116,115,45,91,35,49,49,57,48,93,34,10,32,32,32,32,32,32,32,32,32,100,61,34,109,32,49,48,50,44,50,54,56,56,32,118,32,50,32,72,32,56,54,32,
99,32,45,49,46,49,53,54,44,45,49,46,57,55,49,32,48,46,50,48,54,44,48,46,51,53,32,48,44,48,32,118,32,48,32,48,32,48,32,45,50,32,45,55,32,104,32,49,54,32,122,32,109,32,48,44,57,32,72,32,56,54,32,118,32,
45,55,32,48,32,48,32,99,32,50,46,55,50,51,44,52,46,54,53,52,32,45,49,46,50,57,52,44,45,50,46,50,50,50,32,48,44,48,32,104,32,49,54,32,118,32,48,32,122,32,109,32,45,49,56,44,50,32,104,32,50,48,32,118,32,
45,50,48,32,72,32,56,52,32,90,34,10,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,102,102,102,102,102,102,59,102,105,108,108,45,111,112,97,99,105,116,121,58,48,46,57,57,55,
50,56,57,57,54,34,32,47,62,10,32,32,32,32,60,47,103,62,10,32,32,60,47,103,62,10,32,32,60,112,97,116,104,10,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,110,111,110,101,59,115,116,114,111,
107,101,58,35,102,102,102,102,102,102,59,115,116,114,111,107,101,45,119,105,100,116,104,58,50,46,48,57,57,57,57,57,57,59,115,116,114,111,107,101,45,108,105,110,101,99,97,112,58,98,117,116,116,59,115,116,
114,111,107,101,45,108,105,110,101,106,111,105,110,58,109,105,116,101,114,59,115,116,114,111,107,101,45,109,105,116,101,114,108,105,109,105,116,58,52,59,115,116,114,111,107,101,45,100,97,115,104,97,114,
114,97,121,58,110,111,110,101,59,115,116,114,111,107,101,45,111,112,97,99,105,116,121,58,48,46,57,50,57,53,51,57,51,50,34,10,32,32,32,32,32,100,61,34,109,32,50,46,50,50,52,53,55,54,51,44,54,46,49,50,50,
56,56,49,52,32,99,32,55,46,54,48,53,57,51,50,50,44,48,32,55,46,54,50,55,49,49,56,54,44,48,32,55,46,54,50,55,49,49,56,54,44,48,32,48,44,55,46,50,56,56,49,51,53,54,32,48,44,55,46,51,48,57,51,50,49,54,32,
48,44,55,46,51,48,57,51,50,49,54,34,10,32,32,32,32,32,105,100,61,34,112,97,116,104,49,51,56,56,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,111,110,110,101,99,116,111,114,45,99,117,114,118,
97,116,117,114,101,61,34,48,34,32,47,62,10,32,32,60,112,97,116,104,10,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,110,111,110,101,59,115,116,114,111,107,101,58,35,102,102,102,102,102,102,
59,115,116,114,111,107,101,45,119,105,100,116,104,58,50,46,48,57,57,57,57,57,57,59,115,116,114,111,107,101,45,108,105,110,101,99,97,112,58,98,117,116,116,59,115,116,114,111,107,101,45,108,105,110,101,
106,111,105,110,58,109,105,116,101,114,59,115,116,114,111,107,101,45,109,105,116,101,114,108,105,109,105,116,58,52,59,115,116,114,111,107,101,45,100,97,115,104,97,114,114,97,121,58,110,111,110,101,59,
115,116,114,111,107,101,45,111,112,97,99,105,116,121,58,49,34,10,32,32,32,32,32,100,61,34,109,32,56,46,56,48,57,55,48,51,50,44,49,51,46,52,51,50,50,48,52,32,99,32,56,46,57,54,53,55,49,56,56,44,48,32,56,
46,57,54,53,55,49,56,56,44,48,32,56,46,57,54,53,55,49,56,56,44,48,34,10,32,32,32,32,32,105,100,61,34,112,97,116,104,49,51,57,48,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,111,110,110,101,
99,116,111,114,45,99,117,114,118,97,116,117,114,101,61,34,48,34,32,47,62,10,60,47,115,118,103,62,10,0,0};

const char* VASTWaveTableEditorViewHeader::waves_square_svg = (const char*) resource_VASTWaveTableEditorViewHeader_waves_square_svg;
const int VASTWaveTableEditorViewHeader::waves_square_svgSize = 3000;

// JUCER_RESOURCE: waves_triangle_svg, 2693, "../../Resources/Icons/waves_triangle.svg"
static const unsigned char resource_VASTWaveTableEditorViewHeader_waves_triangle_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,
84,70,45,56,34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,10,32,32,32,120,109,108,110,115,58,100,99,61,34,104,116,116,112,58,47,47,112,117,114,108,46,111,114,103,
47,100,99,47,101,108,101,109,101,110,116,115,47,49,46,49,47,34,10,32,32,32,120,109,108,110,115,58,99,99,61,34,104,116,116,112,58,47,47,99,114,101,97,116,105,118,101,99,111,109,109,111,110,115,46,111,114,
103,47,110,115,35,34,10,32,32,32,120,109,108,110,115,58,114,100,102,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,48,50,47,50,50,45,114,100,102,45,115,121,110,116,
97,120,45,110,115,35,34,10,32,32,32,120,109,108,110,115,58,115,118,103,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,118,103,34,10,32,32,32,120,109,108,110,
115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,118,103,34,10,32,32,32,120,109,108,110,115,58,115,111,100,105,112,111,100,105,61,34,104,116,116,112,58,47,
47,115,111,100,105,112,111,100,105,46,115,111,117,114,99,101,102,111,114,103,101,46,110,101,116,47,68,84,68,47,115,111,100,105,112,111,100,105,45,48,46,100,116,100,34,10,32,32,32,120,109,108,110,115,58,
105,110,107,115,99,97,112,101,61,34,104,116,116,112,58,47,47,119,119,119,46,105,110,107,115,99,97,112,101,46,111,114,103,47,110,97,109,101,115,112,97,99,101,115,47,105,110,107,115,99,97,112,101,34,10,
32,32,32,119,105,100,116,104,61,34,50,48,112,120,34,10,32,32,32,104,101,105,103,104,116,61,34,50,48,112,120,34,10,32,32,32,118,105,101,119,66,111,120,61,34,48,32,48,32,50,48,32,50,48,34,10,32,32,32,118,
101,114,115,105,111,110,61,34,49,46,49,34,10,32,32,32,105,100,61,34,115,118,103,49,50,34,10,32,32,32,115,111,100,105,112,111,100,105,58,100,111,99,110,97,109,101,61,34,119,97,118,101,115,95,116,114,105,
97,110,103,108,101,46,115,118,103,34,10,32,32,32,105,110,107,115,99,97,112,101,58,118,101,114,115,105,111,110,61,34,48,46,57,50,46,51,32,40,50,52,48,53,53,52,54,44,32,50,48,49,56,45,48,51,45,49,49,41,
34,62,10,32,32,60,109,101,116,97,100,97,116,97,10,32,32,32,32,32,105,100,61,34,109,101,116,97,100,97,116,97,49,54,34,62,10,32,32,32,32,60,114,100,102,58,82,68,70,62,10,32,32,32,32,32,32,60,99,99,58,87,
111,114,107,10,32,32,32,32,32,32,32,32,32,114,100,102,58,97,98,111,117,116,61,34,34,62,10,32,32,32,32,32,32,32,32,60,100,99,58,102,111,114,109,97,116,62,105,109,97,103,101,47,115,118,103,43,120,109,108,
60,47,100,99,58,102,111,114,109,97,116,62,10,32,32,32,32,32,32,32,32,60,100,99,58,116,121,112,101,10,32,32,32,32,32,32,32,32,32,32,32,114,100,102,58,114,101,115,111,117,114,99,101,61,34,104,116,116,112,
58,47,47,112,117,114,108,46,111,114,103,47,100,99,47,100,99,109,105,116,121,112,101,47,83,116,105,108,108,73,109,97,103,101,34,32,47,62,10,32,32,32,32,32,32,32,32,60,100,99,58,116,105,116,108,101,62,115,
116,97,116,115,32,91,35,49,49,57,48,93,60,47,100,99,58,116,105,116,108,101,62,10,32,32,32,32,32,32,60,47,99,99,58,87,111,114,107,62,10,32,32,32,32,60,47,114,100,102,58,82,68,70,62,10,32,32,60,47,109,101,
116,97,100,97,116,97,62,10,32,32,60,115,111,100,105,112,111,100,105,58,110,97,109,101,100,118,105,101,119,10,32,32,32,32,32,112,97,103,101,99,111,108,111,114,61,34,35,57,53,57,53,57,53,34,10,32,32,32,
32,32,98,111,114,100,101,114,99,111,108,111,114,61,34,35,54,54,54,54,54,54,34,10,32,32,32,32,32,98,111,114,100,101,114,111,112,97,99,105,116,121,61,34,49,34,10,32,32,32,32,32,111,98,106,101,99,116,116,
111,108,101,114,97,110,99,101,61,34,49,48,34,10,32,32,32,32,32,103,114,105,100,116,111,108,101,114,97,110,99,101,61,34,49,48,34,10,32,32,32,32,32,103,117,105,100,101,116,111,108,101,114,97,110,99,101,
61,34,49,48,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,112,97,103,101,111,112,97,99,105,116,121,61,34,49,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,112,97,103,101,115,104,97,100,
111,119,61,34,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,119,105,100,116,104,61,34,51,56,52,48,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,
100,111,119,45,104,101,105,103,104,116,61,34,50,48,53,48,34,10,32,32,32,32,32,105,100,61,34,110,97,109,101,100,118,105,101,119,49,52,34,10,32,32,32,32,32,115,104,111,119,103,114,105,100,61,34,102,97,108,
115,101,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,122,111,111,109,61,34,52,55,46,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,120,61,34,54,46,56,48,56,53,52,56,34,10,32,32,32,
32,32,105,110,107,115,99,97,112,101,58,99,121,61,34,54,46,52,53,57,53,57,56,51,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,120,61,34,45,49,50,34,10,32,32,32,32,32,
105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,121,61,34,45,49,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,109,97,120,105,109,105,122,101,100,61,34,
49,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,117,114,114,101,110,116,45,108,97,121,101,114,61,34,115,118,103,49,50,34,32,47,62,10,32,32,60,33,45,45,32,71,101,110,101,114,97,116,111,114,
58,32,83,107,101,116,99,104,32,51,46,56,46,49,32,40,50,57,54,56,55,41,32,45,32,104,116,116,112,58,47,47,119,119,119,46,98,111,104,101,109,105,97,110,99,111,100,105,110,103,46,99,111,109,47,115,107,101,
116,99,104,32,45,45,62,10,32,32,60,116,105,116,108,101,10,32,32,32,32,32,105,100,61,34,116,105,116,108,101,50,34,62,115,116,97,116,115,32,91,35,49,49,57,48,93,60,47,116,105,116,108,101,62,10,32,32,60,
100,101,115,99,10,32,32,32,32,32,105,100,61,34,100,101,115,99,52,34,62,67,114,101,97,116,101,100,32,119,105,116,104,32,83,107,101,116,99,104,46,60,47,100,101,115,99,62,10,32,32,60,100,101,102,115,10,32,
32,32,32,32,105,100,61,34,100,101,102,115,54,34,32,47,62,10,32,32,60,103,10,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,45,49,52,48,44,45,50,56,51,57,41,
34,10,32,32,32,32,32,105,100,61,34,68,114,105,98,98,98,108,101,45,76,105,103,104,116,45,80,114,101,118,105,101,119,34,10,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,102,102,102,102,
102,102,59,102,105,108,108,45,114,117,108,101,58,101,118,101,110,111,100,100,59,115,116,114,111,107,101,58,110,111,110,101,59,115,116,114,111,107,101,45,119,105,100,116,104,58,49,59,102,105,108,108,45,
111,112,97,99,105,116,121,58,48,46,57,57,55,50,56,57,57,54,34,62,10,32,32,32,32,60,103,10,32,32,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,53,54,44,49,
54,48,41,34,10,32,32,32,32,32,32,32,105,100,61,34,105,99,111,110,115,34,10,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,102,102,102,102,102,102,59,102,105,108,108,45,111,112,97,
99,105,116,121,58,48,46,57,57,55,50,56,57,57,54,34,62,10,32,32,32,32,32,32,60,112,97,116,104,10,32,32,32,32,32,32,32,32,32,115,111,100,105,112,111,100,105,58,110,111,100,101,116,121,112,101,115,61,34,
99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,34,10,32,32,32,32,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,111,110,110,101,99,116,111,114,45,99,117,114,118,97,116,
117,114,101,61,34,48,34,10,32,32,32,32,32,32,32,32,32,105,100,61,34,115,116,97,116,115,45,91,35,49,49,57,48,93,34,10,32,32,32,32,32,32,32,32,32,100,61,34,109,32,49,48,50,44,50,54,56,56,32,118,32,50,32,
72,32,56,54,32,99,32,45,49,46,49,53,54,44,45,49,46,57,55,49,32,48,46,50,48,54,44,48,46,51,53,32,48,44,48,32,118,32,48,32,48,32,48,32,45,50,32,45,55,32,104,32,49,54,32,122,32,109,32,48,44,57,32,72,32,56,
54,32,118,32,45,55,32,48,32,48,32,99,32,50,46,55,50,51,44,52,46,54,53,52,32,45,49,46,50,57,52,44,45,50,46,50,50,50,32,48,44,48,32,104,32,49,54,32,118,32,48,32,122,32,109,32,45,49,56,44,50,32,104,32,50,
48,32,118,32,45,50,48,32,72,32,56,52,32,90,34,10,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,102,102,102,102,102,102,59,102,105,108,108,45,111,112,97,99,105,116,121,58,48,
46,57,57,55,50,56,57,57,54,34,32,47,62,10,32,32,32,32,60,47,103,62,10,32,32,60,47,103,62,10,32,32,60,112,97,116,104,10,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,110,111,110,101,59,115,
116,114,111,107,101,58,35,102,102,102,102,102,102,59,115,116,114,111,107,101,45,119,105,100,116,104,58,50,59,115,116,114,111,107,101,45,108,105,110,101,99,97,112,58,114,111,117,110,100,59,115,116,114,
111,107,101,45,108,105,110,101,106,111,105,110,58,114,111,117,110,100,59,115,116,114,111,107,101,45,109,105,116,101,114,108,105,109,105,116,58,52,59,115,116,114,111,107,101,45,100,97,115,104,97,114,114,
97,121,58,110,111,110,101,59,115,116,114,111,107,101,45,111,112,97,99,105,116,121,58,49,34,10,32,32,32,32,32,100,61,34,77,32,52,46,54,54,49,48,49,54,57,44,49,48,46,54,55,55,57,54,54,32,56,46,54,48,49,
54,57,52,57,44,52,46,51,56,53,53,57,51,50,32,56,46,53,56,48,53,48,56,52,44,52,46,51,52,51,50,50,48,51,32,49,49,46,55,49,54,49,48,50,44,49,53,46,52,50,51,55,50,57,32,49,53,46,48,50,49,49,56,55,44,49,48,
46,51,49,55,55,57,55,34,10,32,32,32,32,32,105,100,61,34,112,97,116,104,49,52,50,55,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,111,110,110,101,99,116,111,114,45,99,117,114,118,97,116,117,
114,101,61,34,48,34,32,47,62,10,60,47,115,118,103,62,10,0,0};

const char* VASTWaveTableEditorViewHeader::waves_triangle_svg = (const char*) resource_VASTWaveTableEditorViewHeader_waves_triangle_svg;
const int VASTWaveTableEditorViewHeader::waves_triangle_svgSize = 2693;

// JUCER_RESOURCE: waves_zeros_svg, 2623, "../../Resources/Icons/waves_zeros.svg"
static const unsigned char resource_VASTWaveTableEditorViewHeader_waves_zeros_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,
70,45,56,34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,10,32,32,32,120,109,108,110,115,58,100,99,61,34,104,116,116,112,58,47,47,112,117,114,108,46,111,114,103,47,
100,99,47,101,108,101,109,101,110,116,115,47,49,46,49,47,34,10,32,32,32,120,109,108,110,115,58,99,99,61,34,104,116,116,112,58,47,47,99,114,101,97,116,105,118,101,99,111,109,109,111,110,115,46,111,114,
103,47,110,115,35,34,10,32,32,32,120,109,108,110,115,58,114,100,102,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,48,50,47,50,50,45,114,100,102,45,115,121,110,116,
97,120,45,110,115,35,34,10,32,32,32,120,109,108,110,115,58,115,118,103,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,118,103,34,10,32,32,32,120,109,108,110,
115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,118,103,34,10,32,32,32,120,109,108,110,115,58,115,111,100,105,112,111,100,105,61,34,104,116,116,112,58,47,
47,115,111,100,105,112,111,100,105,46,115,111,117,114,99,101,102,111,114,103,101,46,110,101,116,47,68,84,68,47,115,111,100,105,112,111,100,105,45,48,46,100,116,100,34,10,32,32,32,120,109,108,110,115,58,
105,110,107,115,99,97,112,101,61,34,104,116,116,112,58,47,47,119,119,119,46,105,110,107,115,99,97,112,101,46,111,114,103,47,110,97,109,101,115,112,97,99,101,115,47,105,110,107,115,99,97,112,101,34,10,
32,32,32,119,105,100,116,104,61,34,50,48,112,120,34,10,32,32,32,104,101,105,103,104,116,61,34,50,48,112,120,34,10,32,32,32,118,105,101,119,66,111,120,61,34,48,32,48,32,50,48,32,50,48,34,10,32,32,32,118,
101,114,115,105,111,110,61,34,49,46,49,34,10,32,32,32,105,100,61,34,115,118,103,49,50,34,10,32,32,32,115,111,100,105,112,111,100,105,58,100,111,99,110,97,109,101,61,34,119,97,118,101,115,95,122,101,114,
111,115,46,115,118,103,34,10,32,32,32,105,110,107,115,99,97,112,101,58,118,101,114,115,105,111,110,61,34,48,46,57,50,46,51,32,40,50,52,48,53,53,52,54,44,32,50,48,49,56,45,48,51,45,49,49,41,34,62,10,32,
32,60,109,101,116,97,100,97,116,97,10,32,32,32,32,32,105,100,61,34,109,101,116,97,100,97,116,97,49,54,34,62,10,32,32,32,32,60,114,100,102,58,82,68,70,62,10,32,32,32,32,32,32,60,99,99,58,87,111,114,107,
10,32,32,32,32,32,32,32,32,32,114,100,102,58,97,98,111,117,116,61,34,34,62,10,32,32,32,32,32,32,32,32,60,100,99,58,102,111,114,109,97,116,62,105,109,97,103,101,47,115,118,103,43,120,109,108,60,47,100,
99,58,102,111,114,109,97,116,62,10,32,32,32,32,32,32,32,32,60,100,99,58,116,121,112,101,10,32,32,32,32,32,32,32,32,32,32,32,114,100,102,58,114,101,115,111,117,114,99,101,61,34,104,116,116,112,58,47,47,
112,117,114,108,46,111,114,103,47,100,99,47,100,99,109,105,116,121,112,101,47,83,116,105,108,108,73,109,97,103,101,34,32,47,62,10,32,32,32,32,32,32,32,32,60,100,99,58,116,105,116,108,101,62,115,116,97,
116,115,32,91,35,49,49,57,48,93,60,47,100,99,58,116,105,116,108,101,62,10,32,32,32,32,32,32,60,47,99,99,58,87,111,114,107,62,10,32,32,32,32,60,47,114,100,102,58,82,68,70,62,10,32,32,60,47,109,101,116,
97,100,97,116,97,62,10,32,32,60,115,111,100,105,112,111,100,105,58,110,97,109,101,100,118,105,101,119,10,32,32,32,32,32,112,97,103,101,99,111,108,111,114,61,34,35,57,53,57,53,57,53,34,10,32,32,32,32,32,
98,111,114,100,101,114,99,111,108,111,114,61,34,35,54,54,54,54,54,54,34,10,32,32,32,32,32,98,111,114,100,101,114,111,112,97,99,105,116,121,61,34,49,34,10,32,32,32,32,32,111,98,106,101,99,116,116,111,108,
101,114,97,110,99,101,61,34,49,48,34,10,32,32,32,32,32,103,114,105,100,116,111,108,101,114,97,110,99,101,61,34,49,48,34,10,32,32,32,32,32,103,117,105,100,101,116,111,108,101,114,97,110,99,101,61,34,49,
48,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,112,97,103,101,111,112,97,99,105,116,121,61,34,49,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,112,97,103,101,115,104,97,100,111,119,61,
34,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,119,105,100,116,104,61,34,51,56,52,48,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,
45,104,101,105,103,104,116,61,34,50,48,53,48,34,10,32,32,32,32,32,105,100,61,34,110,97,109,101,100,118,105,101,119,49,52,34,10,32,32,32,32,32,115,104,111,119,103,114,105,100,61,34,102,97,108,115,101,34,
10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,122,111,111,109,61,34,52,55,46,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,120,61,34,49,46,52,51,55,55,56,53,51,34,10,32,32,32,32,32,
105,110,107,115,99,97,112,101,58,99,121,61,34,54,46,52,53,57,53,57,56,51,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,120,61,34,45,49,50,34,10,32,32,32,32,32,105,110,
107,115,99,97,112,101,58,119,105,110,100,111,119,45,121,61,34,45,49,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,109,97,120,105,109,105,122,101,100,61,34,49,34,10,
32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,117,114,114,101,110,116,45,108,97,121,101,114,61,34,115,118,103,49,50,34,32,47,62,10,32,32,60,33,45,45,32,71,101,110,101,114,97,116,111,114,58,32,83,
107,101,116,99,104,32,51,46,56,46,49,32,40,50,57,54,56,55,41,32,45,32,104,116,116,112,58,47,47,119,119,119,46,98,111,104,101,109,105,97,110,99,111,100,105,110,103,46,99,111,109,47,115,107,101,116,99,104,
32,45,45,62,10,32,32,60,116,105,116,108,101,10,32,32,32,32,32,105,100,61,34,116,105,116,108,101,50,34,62,115,116,97,116,115,32,91,35,49,49,57,48,93,60,47,116,105,116,108,101,62,10,32,32,60,100,101,115,
99,10,32,32,32,32,32,105,100,61,34,100,101,115,99,52,34,62,67,114,101,97,116,101,100,32,119,105,116,104,32,83,107,101,116,99,104,46,60,47,100,101,115,99,62,10,32,32,60,100,101,102,115,10,32,32,32,32,32,
105,100,61,34,100,101,102,115,54,34,32,47,62,10,32,32,60,103,10,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,45,49,52,48,44,45,50,56,51,57,41,34,10,32,32,
32,32,32,105,100,61,34,68,114,105,98,98,98,108,101,45,76,105,103,104,116,45,80,114,101,118,105,101,119,34,10,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,102,102,102,102,102,102,59,102,
105,108,108,45,114,117,108,101,58,101,118,101,110,111,100,100,59,115,116,114,111,107,101,58,110,111,110,101,59,115,116,114,111,107,101,45,119,105,100,116,104,58,49,59,102,105,108,108,45,111,112,97,99,
105,116,121,58,48,46,57,57,55,50,56,57,57,54,34,62,10,32,32,32,32,60,103,10,32,32,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,53,54,44,49,54,48,41,34,10,
32,32,32,32,32,32,32,105,100,61,34,105,99,111,110,115,34,10,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,102,102,102,102,102,102,59,102,105,108,108,45,111,112,97,99,105,116,121,
58,48,46,57,57,55,50,56,57,57,54,34,62,10,32,32,32,32,32,32,60,112,97,116,104,10,32,32,32,32,32,32,32,32,32,115,111,100,105,112,111,100,105,58,110,111,100,101,116,121,112,101,115,61,34,99,99,99,99,99,
99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,34,10,32,32,32,32,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,111,110,110,101,99,116,111,114,45,99,117,114,118,97,116,117,114,101,61,
34,48,34,10,32,32,32,32,32,32,32,32,32,105,100,61,34,115,116,97,116,115,45,91,35,49,49,57,48,93,34,10,32,32,32,32,32,32,32,32,32,100,61,34,109,32,49,48,50,44,50,54,56,56,32,118,32,50,32,72,32,56,54,32,
99,32,45,49,46,49,53,54,44,45,49,46,57,55,49,32,48,46,50,48,54,44,48,46,51,53,32,48,44,48,32,118,32,48,32,48,32,48,32,45,50,32,45,55,32,104,32,49,54,32,122,32,109,32,48,44,57,32,72,32,56,54,32,118,32,
45,55,32,48,32,48,32,99,32,50,46,55,50,51,44,52,46,54,53,52,32,45,49,46,50,57,52,44,45,50,46,50,50,50,32,48,44,48,32,104,32,49,54,32,118,32,48,32,122,32,109,32,45,49,56,44,50,32,104,32,50,48,32,118,32,
45,50,48,32,72,32,56,52,32,90,34,10,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,102,102,102,102,102,102,59,102,105,108,108,45,111,112,97,99,105,116,121,58,48,46,57,57,55,
50,56,57,57,54,34,32,47,62,10,32,32,32,32,60,47,103,62,10,32,32,60,47,103,62,10,32,32,60,112,97,116,104,10,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,110,111,110,101,59,115,116,114,111,
107,101,58,35,101,101,101,101,101,101,59,115,116,114,111,107,101,45,119,105,100,116,104,58,50,59,115,116,114,111,107,101,45,108,105,110,101,99,97,112,58,114,111,117,110,100,59,115,116,114,111,107,101,
45,108,105,110,101,106,111,105,110,58,114,111,117,110,100,59,115,116,114,111,107,101,45,111,112,97,99,105,116,121,58,49,59,115,116,114,111,107,101,45,109,105,116,101,114,108,105,109,105,116,58,52,59,115,
116,114,111,107,101,45,100,97,115,104,97,114,114,97,121,58,110,111,110,101,34,10,32,32,32,32,32,100,61,34,77,32,51,46,57,56,51,48,53,48,56,44,57,46,57,55,56,56,49,51,54,32,72,32,49,53,46,54,49,52,52,48,
55,34,10,32,32,32,32,32,105,100,61,34,112,97,116,104,50,48,48,55,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,111,110,110,101,99,116,111,114,45,99,117,114,118,97,116,117,114,101,61,34,48,34,
32,47,62,10,60,47,115,118,103,62,10,0,0};

const char* VASTWaveTableEditorViewHeader::waves_zeros_svg = (const char*) resource_VASTWaveTableEditorViewHeader_waves_zeros_svg;
const int VASTWaveTableEditorViewHeader::waves_zeros_svgSize = 2623;

// JUCER_RESOURCE: zero_find_svg, 1166, "../../Resources/Icons/zero_find.svg"
static const unsigned char resource_VASTWaveTableEditorViewHeader_zero_find_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,70,
45,56,34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,32,119,105,100,116,104,61,34,50,49,112,120,34,32,104,101,105,103,104,116,61,34,50,48,112,120,34,32,118,105,101,
119,66,111,120,61,34,48,32,48,32,50,49,32,50,48,34,32,118,101,114,115,105,111,110,61,34,49,46,49,34,32,120,109,108,110,115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,
48,48,47,115,118,103,34,32,120,109,108,110,115,58,120,108,105,110,107,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,120,108,105,110,107,34,62,10,32,32,32,32,60,
33,45,45,32,71,101,110,101,114,97,116,111,114,58,32,83,107,101,116,99,104,32,51,46,56,46,49,32,40,50,57,54,56,55,41,32,45,32,104,116,116,112,58,47,47,119,119,119,46,98,111,104,101,109,105,97,110,99,111,
100,105,110,103,46,99,111,109,47,115,107,101,116,99,104,32,45,45,62,10,32,32,32,32,60,116,105,116,108,101,62,115,101,97,114,99,104,95,108,101,102,116,32,91,35,49,53,48,52,93,60,47,116,105,116,108,101,
62,10,32,32,32,32,60,100,101,115,99,62,67,114,101,97,116,101,100,32,119,105,116,104,32,83,107,101,116,99,104,46,60,47,100,101,115,99,62,10,32,32,32,32,60,100,101,102,115,62,60,47,100,101,102,115,62,10,
32,32,32,32,60,103,32,105,100,61,34,80,97,103,101,45,49,34,32,115,116,114,111,107,101,61,34,110,111,110,101,34,32,115,116,114,111,107,101,45,119,105,100,116,104,61,34,49,34,32,102,105,108,108,61,34,110,
111,110,101,34,32,102,105,108,108,45,114,117,108,101,61,34,101,118,101,110,111,100,100,34,62,10,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,68,114,105,98,98,98,108,101,45,68,97,114,107,45,80,114,101,
118,105,101,119,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,45,50,57,57,46,48,48,48,48,48,48,44,32,45,50,56,48,46,48,48,48,48,48,48,41,34,32,102,105,108,108,61,
34,35,70,70,70,70,70,70,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,105,99,111,110,115,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,53,54,
46,48,48,48,48,48,48,44,32,49,54,48,46,48,48,48,48,48,48,41,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,60,112,97,116,104,32,100,61,34,77,50,54,52,44,49,51,56,46,53,56,54,32,76,50,54,50,46,
53,49,53,51,44,49,52,48,32,76,50,53,56,46,48,54,48,49,53,44,49,51,53,46,55,53,56,32,76,50,53,57,46,53,52,52,56,53,44,49,51,52,46,51,52,51,32,76,50,54,52,44,49,51,56,46,53,56,54,32,90,32,77,50,53,49,46,
52,44,49,51,52,32,67,50,52,55,46,57,50,54,54,44,49,51,52,32,50,52,53,46,49,44,49,51,49,46,51,48,57,32,50,52,53,46,49,44,49,50,56,32,67,50,52,53,46,49,44,49,50,52,46,54,57,50,32,50,52,55,46,57,50,54,54,
44,49,50,50,32,50,53,49,46,52,44,49,50,50,32,67,50,53,52,46,56,55,51,52,44,49,50,50,32,50,53,55,46,55,44,49,50,52,46,54,57,50,32,50,53,55,46,55,44,49,50,56,32,67,50,53,55,46,55,44,49,51,49,46,51,48,57,
32,50,53,52,46,56,55,51,52,44,49,51,52,32,50,53,49,46,52,44,49,51,52,32,76,50,53,49,46,52,44,49,51,52,32,90,32,77,50,53,49,46,52,44,49,50,48,32,67,50,52,54,46,55,54,49,49,44,49,50,48,32,50,52,51,44,49,
50,51,46,53,56,50,32,50,52,51,44,49,50,56,32,67,50,52,51,44,49,51,50,46,52,49,56,32,50,52,54,46,55,54,49,49,44,49,51,54,32,50,53,49,46,52,44,49,51,54,32,67,50,53,54,46,48,51,56,57,44,49,51,54,32,50,53,
57,46,56,44,49,51,50,46,52,49,56,32,50,53,57,46,56,44,49,50,56,32,67,50,53,57,46,56,44,49,50,51,46,53,56,50,32,50,53,54,46,48,51,56,57,44,49,50,48,32,50,53,49,46,52,44,49,50,48,32,76,50,53,49,46,52,44,
49,50,48,32,90,34,32,105,100,61,34,115,101,97,114,99,104,95,108,101,102,116,45,91,35,49,53,48,52,93,34,62,60,47,112,97,116,104,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,32,
32,32,32,60,47,103,62,10,32,32,32,32,60,47,103,62,10,60,47,115,118,103,62,0,0};

const char* VASTWaveTableEditorViewHeader::zero_find_svg = (const char*) resource_VASTWaveTableEditorViewHeader_zero_find_svg;
const int VASTWaveTableEditorViewHeader::zero_find_svgSize = 1166;

// JUCER_RESOURCE: zoom_in_svg, 1528, "../../Resources/Icons/zoom_in.svg"
static const unsigned char resource_VASTWaveTableEditorViewHeader_zoom_in_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,70,
45,56,34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,32,119,105,100,116,104,61,34,50,49,112,120,34,32,104,101,105,103,104,116,61,34,50,48,112,120,34,32,118,105,101,
119,66,111,120,61,34,48,32,48,32,50,49,32,50,48,34,32,118,101,114,115,105,111,110,61,34,49,46,49,34,32,120,109,108,110,115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,
48,48,47,115,118,103,34,32,120,109,108,110,115,58,120,108,105,110,107,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,120,108,105,110,107,34,62,10,32,32,32,32,60,
33,45,45,32,71,101,110,101,114,97,116,111,114,58,32,83,107,101,116,99,104,32,51,46,56,46,49,32,40,50,57,54,56,55,41,32,45,32,104,116,116,112,58,47,47,119,119,119,46,98,111,104,101,109,105,97,110,99,111,
100,105,110,103,46,99,111,109,47,115,107,101,116,99,104,32,45,45,62,10,32,32,32,32,60,116,105,116,108,101,62,122,111,111,109,95,105,110,32,91,35,49,52,54,49,93,60,47,116,105,116,108,101,62,10,32,32,32,
32,60,100,101,115,99,62,67,114,101,97,116,101,100,32,119,105,116,104,32,83,107,101,116,99,104,46,60,47,100,101,115,99,62,10,32,32,32,32,60,100,101,102,115,62,60,47,100,101,102,115,62,10,32,32,32,32,60,
103,32,105,100,61,34,80,97,103,101,45,49,34,32,115,116,114,111,107,101,61,34,110,111,110,101,34,32,115,116,114,111,107,101,45,119,105,100,116,104,61,34,49,34,32,102,105,108,108,61,34,110,111,110,101,34,
32,102,105,108,108,45,114,117,108,101,61,34,101,118,101,110,111,100,100,34,62,10,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,68,114,105,98,98,98,108,101,45,68,97,114,107,45,80,114,101,118,105,101,
119,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,45,52,49,57,46,48,48,48,48,48,48,44,32,45,52,52,48,46,48,48,48,48,48,48,41,34,32,102,105,108,108,61,34,35,70,70,
70,70,70,70,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,105,99,111,110,115,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,53,54,46,48,48,48,
48,48,48,44,32,49,54,48,46,48,48,48,48,48,48,41,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,60,112,97,116,104,32,100,61,34,77,51,55,50,46,52,53,44,50,56,54,46,57,54,55,52,49,57,32,76,51,55,
53,46,54,44,50,56,54,46,57,54,55,52,49,57,32,76,51,55,53,46,54,44,50,56,56,46,57,55,50,52,51,49,32,76,51,55,50,46,52,53,44,50,56,56,46,57,55,50,52,51,49,32,76,51,55,50,46,52,53,44,50,57,49,46,57,55,57,
57,53,32,76,51,55,48,46,51,53,44,50,57,49,46,57,55,57,57,53,32,76,51,55,48,46,51,53,44,50,56,56,46,57,55,50,52,51,49,32,76,51,54,55,46,50,44,50,56,56,46,57,55,50,52,51,49,32,76,51,54,55,46,50,44,50,56,
54,46,57,54,55,52,49,57,32,76,51,55,48,46,51,53,44,50,56,54,46,57,54,55,52,49,57,32,76,51,55,48,46,51,53,44,50,56,51,46,57,53,57,57,32,76,51,55,50,46,52,53,44,50,56,51,46,57,53,57,57,32,76,51,55,50,46,
52,53,44,50,56,54,46,57,54,55,52,49,57,32,90,32,77,51,56,52,44,50,57,56,46,53,56,50,52,53,54,32,76,51,56,50,46,53,49,53,51,44,51,48,48,32,76,51,55,56,46,48,54,48,49,53,44,50,57,53,46,55,52,54,51,54,54,
32,76,51,55,57,46,53,52,52,56,53,44,50,57,52,46,51,50,56,56,50,50,32,76,51,56,52,44,50,57,56,46,53,56,50,52,53,54,32,90,32,77,51,55,49,46,52,44,50,57,52,46,48,51,53,48,56,56,32,67,51,54,55,46,57,50,54,
54,44,50,57,52,46,48,51,53,48,56,56,32,51,54,53,46,49,44,50,57,49,46,51,51,55,51,52,51,32,51,54,53,46,49,44,50,56,56,46,48,50,48,48,53,32,67,51,54,53,46,49,44,50,56,52,46,55,48,51,55,53,57,32,51,54,55,
46,57,50,54,54,44,50,56,50,46,48,48,53,48,49,51,32,51,55,49,46,52,44,50,56,50,46,48,48,53,48,49,51,32,67,51,55,52,46,56,55,51,52,44,50,56,50,46,48,48,53,48,49,51,32,51,55,55,46,55,44,50,56,52,46,55,48,
51,55,53,57,32,51,55,55,46,55,44,50,56,56,46,48,50,48,48,53,32,67,51,55,55,46,55,44,50,57,49,46,51,51,55,51,52,51,32,51,55,52,46,56,55,51,52,44,50,57,52,46,48,51,53,48,56,56,32,51,55,49,46,52,44,50,57,
52,46,48,51,53,48,56,56,32,76,51,55,49,46,52,44,50,57,52,46,48,51,53,48,56,56,32,90,32,77,51,55,49,46,52,44,50,56,48,32,67,51,54,54,46,55,54,49,49,44,50,56,48,32,51,54,51,44,50,56,51,46,53,57,48,57,55,
55,32,51,54,51,44,50,56,56,46,48,50,48,48,53,32,67,51,54,51,44,50,57,50,46,52,53,48,49,50,53,32,51,54,54,46,55,54,49,49,44,50,57,54,46,48,52,48,49,32,51,55,49,46,52,44,50,57,54,46,48,52,48,49,32,67,51,
55,54,46,48,51,56,57,44,50,57,54,46,48,52,48,49,32,51,55,57,46,56,44,50,57,50,46,52,53,48,49,50,53,32,51,55,57,46,56,44,50,56,56,46,48,50,48,48,53,32,67,51,55,57,46,56,44,50,56,51,46,53,57,48,57,55,55,
32,51,55,54,46,48,51,56,57,44,50,56,48,32,51,55,49,46,52,44,50,56,48,32,76,51,55,49,46,52,44,50,56,48,32,90,34,32,105,100,61,34,122,111,111,109,95,105,110,45,91,35,49,52,54,49,93,34,62,60,47,112,97,116,
104,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,60,47,103,62,10,60,47,115,118,103,62,0,0};

const char* VASTWaveTableEditorViewHeader::zoom_in_svg = (const char*) resource_VASTWaveTableEditorViewHeader_zoom_in_svg;
const int VASTWaveTableEditorViewHeader::zoom_in_svgSize = 1528;

// JUCER_RESOURCE: zoom_out_svg, 1506, "../../Resources/Icons/zoom_out.svg"
static const unsigned char resource_VASTWaveTableEditorViewHeader_zoom_out_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,70,
45,56,34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,32,119,105,100,116,104,61,34,50,49,112,120,34,32,104,101,105,103,104,116,61,34,50,48,112,120,34,32,118,105,101,
119,66,111,120,61,34,48,32,48,32,50,49,32,50,48,34,32,118,101,114,115,105,111,110,61,34,49,46,49,34,32,120,109,108,110,115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,
48,48,47,115,118,103,34,32,120,109,108,110,115,58,120,108,105,110,107,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,120,108,105,110,107,34,62,10,32,32,32,32,60,
33,45,45,32,71,101,110,101,114,97,116,111,114,58,32,83,107,101,116,99,104,32,51,46,56,46,49,32,40,50,57,54,56,55,41,32,45,32,104,116,116,112,58,47,47,119,119,119,46,98,111,104,101,109,105,97,110,99,111,
100,105,110,103,46,99,111,109,47,115,107,101,116,99,104,32,45,45,62,10,32,32,32,32,60,116,105,116,108,101,62,122,111,111,109,95,111,117,116,32,91,35,49,52,53,57,93,60,47,116,105,116,108,101,62,10,32,32,
32,32,60,100,101,115,99,62,67,114,101,97,116,101,100,32,119,105,116,104,32,83,107,101,116,99,104,46,60,47,100,101,115,99,62,10,32,32,32,32,60,100,101,102,115,62,60,47,100,101,102,115,62,10,32,32,32,32,
60,103,32,105,100,61,34,80,97,103,101,45,49,34,32,115,116,114,111,107,101,61,34,110,111,110,101,34,32,115,116,114,111,107,101,45,119,105,100,116,104,61,34,49,34,32,102,105,108,108,61,34,110,111,110,101,
34,32,102,105,108,108,45,114,117,108,101,61,34,101,118,101,110,111,100,100,34,62,10,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,68,114,105,98,98,98,108,101,45,68,97,114,107,45,80,114,101,118,105,101,
119,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,45,57,57,46,48,48,48,48,48,48,44,32,45,52,56,48,46,48,48,48,48,48,48,41,34,32,102,105,108,108,61,34,35,70,70,70,
70,70,70,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,105,99,111,110,115,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,53,54,46,48,48,48,48,
48,48,44,32,49,54,48,46,48,48,48,48,48,48,41,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,60,112,97,116,104,32,100,61,34,77,52,55,46,51,50,53,52,51,55,55,44,51,50,56,46,57,55,50,52,51,49,32,
76,53,53,46,57,55,54,51,49,51,49,44,51,50,56,46,57,55,50,52,51,49,32,76,53,53,46,57,55,54,51,49,51,49,44,51,50,54,46,57,54,55,52,49,57,32,76,52,55,46,51,50,53,52,51,55,55,44,51,50,54,46,57,54,55,52,49,
57,32,76,52,55,46,51,50,53,52,51,55,55,44,51,50,56,46,57,55,50,52,51,49,32,90,32,77,54,52,44,51,51,56,46,53,56,50,52,53,54,32,76,54,50,46,52,54,57,56,55,54,52,44,51,52,48,32,76,53,55,46,56,56,50,55,52,
57,55,44,51,51,53,46,55,52,54,51,54,54,32,76,53,57,46,52,49,49,55,57,50,44,51,51,52,46,51,50,56,56,50,50,32,76,54,52,44,51,51,56,46,53,56,50,52,53,54,32,90,32,77,53,49,46,54,53,48,56,55,53,52,44,51,51,
52,46,48,51,53,48,56,56,32,67,52,56,46,48,55,50,54,53,55,49,44,51,51,52,46,48,51,53,48,56,56,32,52,53,46,49,54,50,55,49,56,56,44,51,51,49,46,51,51,55,51,52,51,32,52,53,46,49,54,50,55,49,56,56,44,51,50,
56,46,48,50,48,48,53,32,67,52,53,46,49,54,50,55,49,56,56,44,51,50,52,46,55,48,51,55,53,57,32,52,56,46,48,55,50,54,53,55,49,44,51,50,50,46,48,48,53,48,49,51,32,53,49,46,54,53,48,56,55,53,52,44,51,50,50,
46,48,48,53,48,49,51,32,67,53,53,46,50,50,56,48,49,50,52,44,51,50,50,46,48,48,53,48,49,51,32,53,56,46,49,51,57,48,51,49,57,44,51,50,52,46,55,48,51,55,53,57,32,53,56,46,49,51,57,48,51,49,57,44,51,50,56,
46,48,50,48,48,53,32,67,53,56,46,49,51,57,48,51,49,57,44,51,51,49,46,51,51,55,51,52,51,32,53,53,46,50,50,56,48,49,50,52,44,51,51,52,46,48,51,53,48,56,56,32,53,49,46,54,53,48,56,55,53,52,44,51,51,52,46,
48,51,53,48,56,56,32,76,53,49,46,54,53,48,56,55,53,52,44,51,51,52,46,48,51,53,48,56,56,32,90,32,77,53,49,46,54,53,48,56,55,53,52,44,51,50,48,32,67,52,54,46,56,55,50,51,52,56,49,44,51,50,48,32,52,51,44,
51,50,51,46,53,57,48,57,55,55,32,52,51,44,51,50,56,46,48,50,48,48,53,32,67,52,51,44,51,51,50,46,52,52,57,49,50,51,32,52,54,46,56,55,50,51,52,56,49,44,51,51,54,46,48,52,48,49,32,53,49,46,54,53,48,56,55,
53,52,44,51,51,54,46,48,52,48,49,32,67,53,54,46,52,50,56,51,50,49,51,44,51,51,54,46,48,52,48,49,32,54,48,46,51,48,49,55,53,48,56,44,51,51,50,46,52,52,57,49,50,51,32,54,48,46,51,48,49,55,53,48,56,44,51,
50,56,46,48,50,48,48,53,32,67,54,48,46,51,48,49,55,53,48,56,44,51,50,51,46,53,57,48,57,55,55,32,53,54,46,52,50,56,51,50,49,51,44,51,50,48,32,53,49,46,54,53,48,56,55,53,52,44,51,50,48,32,76,53,49,46,54,
53,48,56,55,53,52,44,51,50,48,32,90,34,32,105,100,61,34,122,111,111,109,95,111,117,116,45,91,35,49,52,53,57,93,34,62,60,47,112,97,116,104,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,
32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,60,47,103,62,10,60,47,115,118,103,62,0,0};

const char* VASTWaveTableEditorViewHeader::zoom_out_svg = (const char*) resource_VASTWaveTableEditorViewHeader_zoom_out_svg;
const int VASTWaveTableEditorViewHeader::zoom_out_svgSize = 1506;

// JUCER_RESOURCE: dynamics_svg, 13088, "../../Resources/dynamics.svg"
static const unsigned char resource_VASTWaveTableEditorViewHeader_dynamics_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,70,
45,56,34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,33,45,45,32,67,114,101,97,116,101,100,32,119,105,116,104,32,73,110,107,115,99,97,112,101,32,40,104,116,116,112,58,47,47,119,
119,119,46,105,110,107,115,99,97,112,101,46,111,114,103,47,41,32,45,45,62,10,10,60,115,118,103,10,32,32,32,120,109,108,110,115,58,100,99,61,34,104,116,116,112,58,47,47,112,117,114,108,46,111,114,103,47,
100,99,47,101,108,101,109,101,110,116,115,47,49,46,49,47,34,10,32,32,32,120,109,108,110,115,58,99,99,61,34,104,116,116,112,58,47,47,99,114,101,97,116,105,118,101,99,111,109,109,111,110,115,46,111,114,
103,47,110,115,35,34,10,32,32,32,120,109,108,110,115,58,114,100,102,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,48,50,47,50,50,45,114,100,102,45,115,121,110,116,
97,120,45,110,115,35,34,10,32,32,32,120,109,108,110,115,58,115,118,103,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,118,103,34,10,32,32,32,120,109,108,110,
115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,118,103,34,10,32,32,32,120,109,108,110,115,58,115,111,100,105,112,111,100,105,61,34,104,116,116,112,58,47,
47,115,111,100,105,112,111,100,105,46,115,111,117,114,99,101,102,111,114,103,101,46,110,101,116,47,68,84,68,47,115,111,100,105,112,111,100,105,45,48,46,100,116,100,34,10,32,32,32,120,109,108,110,115,58,
105,110,107,115,99,97,112,101,61,34,104,116,116,112,58,47,47,119,119,119,46,105,110,107,115,99,97,112,101,46,111,114,103,47,110,97,109,101,115,112,97,99,101,115,47,105,110,107,115,99,97,112,101,34,10,
32,32,32,119,105,100,116,104,61,34,50,49,48,109,109,34,10,32,32,32,104,101,105,103,104,116,61,34,50,57,55,109,109,34,10,32,32,32,105,100,61,34,115,118,103,51,48,50,52,34,10,32,32,32,118,101,114,115,105,
111,110,61,34,49,46,49,34,10,32,32,32,105,110,107,115,99,97,112,101,58,118,101,114,115,105,111,110,61,34,48,46,57,50,46,51,32,40,50,52,48,53,53,52,54,44,32,50,48,49,56,45,48,51,45,49,49,41,34,10,32,32,
32,115,111,100,105,112,111,100,105,58,100,111,99,110,97,109,101,61,34,100,121,110,97,109,105,99,115,46,115,118,103,34,62,10,32,32,60,100,101,102,115,10,32,32,32,32,32,105,100,61,34,100,101,102,115,51,
48,50,54,34,32,47,62,10,32,32,60,115,111,100,105,112,111,100,105,58,110,97,109,101,100,118,105,101,119,10,32,32,32,32,32,105,100,61,34,98,97,115,101,34,10,32,32,32,32,32,112,97,103,101,99,111,108,111,
114,61,34,35,102,102,102,102,102,102,34,10,32,32,32,32,32,98,111,114,100,101,114,99,111,108,111,114,61,34,35,54,54,54,54,54,54,34,10,32,32,32,32,32,98,111,114,100,101,114,111,112,97,99,105,116,121,61,
34,49,46,48,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,112,97,103,101,111,112,97,99,105,116,121,61,34,48,46,48,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,112,97,103,101,115,104,97,
100,111,119,61,34,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,122,111,111,109,61,34,55,46,57,49,57,53,57,53,57,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,120,61,34,50,53,55,
46,57,48,48,49,57,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,121,61,34,53,53,56,46,54,53,52,48,56,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,100,111,99,117,109,101,110,116,45,
117,110,105,116,115,61,34,112,120,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,117,114,114,101,110,116,45,108,97,121,101,114,61,34,108,97,121,101,114,49,34,10,32,32,32,32,32,115,104,111,119,
103,114,105,100,61,34,102,97,108,115,101,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,119,105,100,116,104,61,34,53,52,51,49,34,10,32,32,32,32,32,105,110,107,115,99,
97,112,101,58,119,105,110,100,111,119,45,104,101,105,103,104,116,61,34,50,56,54,56,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,120,61,34,45,49,56,34,10,32,32,32,32,
32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,121,61,34,45,49,56,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,109,97,120,105,109,105,122,101,100,61,
34,49,34,32,47,62,10,32,32,60,109,101,116,97,100,97,116,97,10,32,32,32,32,32,105,100,61,34,109,101,116,97,100,97,116,97,51,48,50,57,34,62,10,32,32,32,32,60,114,100,102,58,82,68,70,62,10,32,32,32,32,32,
32,60,99,99,58,87,111,114,107,10,32,32,32,32,32,32,32,32,32,114,100,102,58,97,98,111,117,116,61,34,34,62,10,32,32,32,32,32,32,32,32,60,100,99,58,102,111,114,109,97,116,62,105,109,97,103,101,47,115,118,
103,43,120,109,108,60,47,100,99,58,102,111,114,109,97,116,62,10,32,32,32,32,32,32,32,32,60,100,99,58,116,121,112,101,10,32,32,32,32,32,32,32,32,32,32,32,114,100,102,58,114,101,115,111,117,114,99,101,61,
34,104,116,116,112,58,47,47,112,117,114,108,46,111,114,103,47,100,99,47,100,99,109,105,116,121,112,101,47,83,116,105,108,108,73,109,97,103,101,34,32,47,62,10,32,32,32,32,32,32,32,32,60,100,99,58,116,105,
116,108,101,62,60,47,100,99,58,116,105,116,108,101,62,10,32,32,32,32,32,32,60,47,99,99,58,87,111,114,107,62,10,32,32,32,32,60,47,114,100,102,58,82,68,70,62,10,32,32,60,47,109,101,116,97,100,97,116,97,
62,10,32,32,60,103,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,108,97,98,101,108,61,34,69,98,101,110,101,32,49,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,103,114,111,117,112,109,111,
100,101,61,34,108,97,121,101,114,34,10,32,32,32,32,32,105,100,61,34,108,97,121,101,114,49,34,62,10,32,32,32,32,60,103,10,32,32,32,32,32,32,32,97,114,105,97,45,108,97,98,101,108,61,34,100,121,110,97,109,
105,99,115,34,10,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,111,110,116,45,115,116,121,108,101,58,111,98,108,105,113,117,101,59,102,111,110,116,45,118,97,114,105,97,110,116,58,110,111,114,109,
97,108,59,102,111,110,116,45,119,101,105,103,104,116,58,110,111,114,109,97,108,59,102,111,110,116,45,115,116,114,101,116,99,104,58,110,111,114,109,97,108,59,108,105,110,101,45,104,101,105,103,104,116,
58,48,37,59,102,111,110,116,45,102,97,109,105,108,121,58,39,80,97,108,97,99,101,32,83,99,114,105,112,116,32,77,84,39,59,45,105,110,107,115,99,97,112,101,45,102,111,110,116,45,115,112,101,99,105,102,105,
99,97,116,105,111,110,58,39,80,97,108,97,99,101,32,83,99,114,105,112,116,32,77,84,32,79,98,108,105,113,117,101,39,59,116,101,120,116,45,97,108,105,103,110,58,115,116,97,114,116,59,108,101,116,116,101,
114,45,115,112,97,99,105,110,103,58,48,112,120,59,119,111,114,100,45,115,112,97,99,105,110,103,58,48,112,120,59,119,114,105,116,105,110,103,45,109,111,100,101,58,108,114,45,116,98,59,116,101,120,116,45,
97,110,99,104,111,114,58,115,116,97,114,116,59,102,105,108,108,58,35,102,57,102,57,102,57,59,102,105,108,108,45,111,112,97,99,105,116,121,58,49,59,115,116,114,111,107,101,58,110,111,110,101,34,10,32,32,
32,32,32,32,32,105,100,61,34,116,101,120,116,51,48,51,50,34,62,10,32,32,32,32,32,32,60,112,97,116,104,10,32,32,32,32,32,32,32,32,32,100,61,34,109,32,50,50,52,46,56,56,48,50,56,44,53,51,56,46,53,56,57,
56,52,32,113,32,45,49,46,54,48,49,53,54,44,48,32,45,52,46,53,55,48,51,49,44,50,46,50,54,53,54,51,32,45,50,46,57,54,56,55,53,44,50,46,50,50,54,53,54,32,45,53,46,50,55,51,52,52,44,53,46,48,55,56,49,50,32,
45,50,46,50,54,53,54,51,44,50,46,56,49,50,53,32,45,50,46,50,54,53,54,51,44,52,46,52,49,52,48,55,32,48,44,49,46,50,56,57,48,54,32,49,46,50,49,48,57,52,44,49,46,50,56,57,48,54,32,49,46,55,49,56,55,53,44,
48,32,52,46,55,50,54,53,54,44,45,50,46,50,50,54,53,54,32,51,46,48,52,54,56,56,44,45,50,46,50,54,53,54,51,32,53,46,51,57,48,54,51,44,45,53,46,48,55,56,49,51,32,50,46,51,56,50,56,49,44,45,50,46,56,49,50,
53,32,50,46,51,56,50,56,49,44,45,52,46,50,57,54,56,55,32,48,44,45,49,46,52,52,53,51,50,32,45,49,46,54,48,49,53,54,44,45,49,46,52,52,53,51,50,32,122,32,109,32,49,46,54,55,57,54,57,44,51,46,50,48,51,49,
51,32,50,49,46,48,57,51,55,53,44,45,50,52,46,49,48,49,53,54,32,104,32,51,46,50,48,51,49,50,32,108,32,45,50,52,46,54,48,57,51,55,44,50,56,46,48,56,53,57,51,32,113,32,45,48,46,56,50,48,51,50,44,48,46,57,
51,55,53,32,45,49,46,55,57,54,56,56,44,50,46,49,48,57,51,56,32,45,48,46,57,55,54,53,54,44,49,46,49,55,49,56,55,32,45,49,46,52,52,53,51,49,44,49,46,56,51,53,57,52,32,45,48,46,52,50,57,54,57,44,48,46,54,
54,52,48,54,32,45,48,46,54,50,53,44,49,46,50,56,57,48,54,32,45,48,46,49,53,54,50,53,44,48,46,54,50,53,32,48,46,53,56,53,57,52,44,48,46,54,50,53,32,48,46,57,55,54,53,54,44,48,32,52,46,56,48,52,54,56,44,
45,51,46,48,48,55,56,49,32,51,46,56,50,56,49,51,44,45,51,46,48,52,54,56,56,32,55,46,49,56,55,53,44,45,54,46,48,53,52,54,57,32,118,32,48,46,54,54,52,48,54,32,113,32,45,52,46,54,48,57,51,55,44,52,46,48,
50,51,52,52,32,45,55,46,55,55,51,52,51,44,54,46,52,56,52,51,56,32,45,51,46,49,50,53,44,50,46,52,54,48,57,51,32,45,52,46,53,55,48,51,50,44,50,46,52,54,48,57,51,32,45,49,46,48,49,53,54,50,44,48,32,45,49,
46,48,49,53,54,50,44,45,49,46,50,49,48,57,51,32,48,44,45,49,46,53,54,50,53,32,49,46,48,57,51,55,53,44,45,52,46,48,54,50,53,32,48,44,45,48,46,48,51,57,49,32,45,48,46,48,55,56,49,44,45,48,46,48,55,56,49,
32,45,48,46,48,51,57,49,44,45,48,46,48,55,56,49,32,45,48,46,48,55,56,49,44,45,48,46,48,55,56,49,32,45,53,46,52,50,57,54,57,44,53,46,52,50,57,54,56,32,45,56,46,53,53,52,54,57,44,53,46,52,50,57,54,56,32,
45,50,46,50,50,54,53,54,44,48,32,45,50,46,50,50,54,53,54,44,45,51,46,54,55,49,56,55,32,48,44,45,50,46,56,57,48,54,51,32,50,46,49,56,55,53,44,45,53,46,51,49,50,53,32,50,46,49,56,55,53,44,45,50,46,52,50,
49,56,56,32,53,46,51,57,48,54,50,44,45,51,46,55,56,57,48,54,32,51,46,50,48,51,49,51,44,45,49,46,51,54,55,49,57,32,53,46,55,48,51,49,51,44,45,49,46,51,54,55,49,57,32,49,46,57,57,50,49,57,44,48,32,49,46,
57,57,50,49,57,44,49,46,54,55,57,54,57,32,48,44,49,46,49,55,49,56,55,32,45,48,46,52,54,56,55,53,44,50,46,48,55,48,51,49,32,122,34,10,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,111,110,116,
45,115,116,121,108,101,58,111,98,108,105,113,117,101,59,102,111,110,116,45,118,97,114,105,97,110,116,58,110,111,114,109,97,108,59,102,111,110,116,45,119,101,105,103,104,116,58,110,111,114,109,97,108,59,
102,111,110,116,45,115,116,114,101,116,99,104,58,110,111,114,109,97,108,59,102,111,110,116,45,115,105,122,101,58,56,48,112,120,59,108,105,110,101,45,104,101,105,103,104,116,58,49,50,53,37,59,102,111,110,
116,45,102,97,109,105,108,121,58,39,80,97,108,97,99,101,32,83,99,114,105,112,116,32,77,84,39,59,45,105,110,107,115,99,97,112,101,45,102,111,110,116,45,115,112,101,99,105,102,105,99,97,116,105,111,110,
58,39,80,97,108,97,99,101,32,83,99,114,105,112,116,32,77,84,32,79,98,108,105,113,117,101,39,59,116,101,120,116,45,97,108,105,103,110,58,115,116,97,114,116,59,119,114,105,116,105,110,103,45,109,111,100,
101,58,108,114,45,116,98,59,116,101,120,116,45,97,110,99,104,111,114,58,115,116,97,114,116,59,102,105,108,108,58,35,102,57,102,57,102,57,34,10,32,32,32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,51,
55,51,53,34,32,47,62,10,32,32,32,32,32,32,60,112,97,116,104,10,32,32,32,32,32,32,32,32,32,100,61,34,109,32,50,51,57,46,57,53,56,52,44,53,53,53,46,53,52,50,57,55,32,113,32,45,56,46,50,48,51,49,50,44,52,
46,49,48,49,53,54,32,45,49,51,46,56,54,55,49,56,44,56,46,50,56,49,50,53,32,45,53,46,55,48,51,49,51,44,52,46,49,55,57,54,57,32,45,53,46,55,48,51,49,51,44,54,46,57,57,50,49,57,32,48,44,48,46,55,48,51,49,
50,32,48,46,53,48,55,56,49,44,49,46,49,51,50,56,49,32,48,46,52,54,56,55,53,44,48,46,52,54,56,55,53,32,49,46,49,55,49,56,56,44,48,46,52,54,56,55,53,32,49,46,51,54,55,49,57,44,48,32,51,46,48,56,53,57,52,
44,45,48,46,57,55,54,53,54,32,49,46,55,49,56,55,53,44,45,48,46,57,51,55,53,32,51,46,56,54,55,49,56,44,45,50,46,57,54,56,55,53,32,50,46,49,48,57,51,56,44,45,50,46,48,51,49,50,53,32,52,46,57,54,48,57,52,
44,45,53,46,51,57,48,54,51,32,49,46,53,50,51,52,52,44,45,49,46,56,55,53,32,51,46,50,56,49,50,53,44,45,52,46,48,54,50,53,32,49,46,55,57,54,56,56,44,45,50,46,49,52,56,52,52,32,50,46,54,57,53,51,49,44,45,
51,46,52,55,54,53,54,32,122,32,109,32,45,53,46,57,55,54,53,54,44,45,49,49,46,51,54,55,49,57,32,118,32,45,48,46,54,50,53,32,113,32,51,46,49,50,53,44,45,50,46,54,57,53,51,49,32,53,46,49,49,55,49,57,44,45,
52,46,49,48,49,53,54,32,50,46,48,51,49,50,53,44,45,49,46,52,48,54,50,53,32,51,46,53,49,53,54,50,44,45,49,46,52,48,54,50,53,32,49,46,50,49,48,57,52,44,48,32,49,46,50,49,48,57,52,44,49,46,54,48,49,53,54,
32,48,44,51,46,51,57,56,52,52,32,45,52,46,54,52,56,52,52,44,55,46,53,32,45,51,46,51,53,57,51,55,44,50,46,57,54,56,55,53,32,45,51,46,51,53,57,51,55,44,51,46,57,48,54,50,53,32,48,44,48,46,53,56,53,57,52,
32,48,46,55,48,51,49,50,44,48,46,53,56,53,57,52,32,49,46,52,52,53,51,50,44,48,32,53,44,45,50,46,51,56,50,56,49,32,51,46,53,57,51,55,53,44,45,50,46,52,50,49,56,56,32,55,46,52,50,49,56,56,44,45,53,46,53,
56,53,57,52,32,51,46,56,54,55,49,57,44,45,51,46,50,48,51,49,51,32,53,46,57,55,54,53,54,44,45,53,46,51,49,50,53,32,104,32,50,46,57,54,56,55,53,32,108,32,45,49,50,46,53,51,57,48,54,44,49,52,46,50,57,54,
56,55,32,113,32,52,46,51,55,53,44,45,49,46,55,53,55,56,49,32,55,46,53,44,45,51,46,56,50,56,49,50,32,51,46,49,54,52,48,54,44,45,50,46,49,48,57,51,56,32,55,46,57,50,57,54,57,44,45,54,46,50,53,32,118,32,
48,46,54,54,52,48,54,32,113,32,45,52,46,52,53,51,49,51,44,52,46,49,48,49,53,54,32,45,55,46,56,49,50,53,44,54,46,50,56,57,48,54,32,45,51,46,51,53,57,51,56,44,50,46,49,56,55,53,32,45,56,46,53,49,53,54,51,
44,52,46,49,48,49,53,55,32,108,32,45,48,46,55,48,51,49,50,44,48,46,55,56,49,50,53,32,113,32,45,56,46,49,54,52,48,55,44,57,46,49,52,48,54,50,32,45,49,51,46,50,48,51,49,51,44,49,51,46,56,54,55,49,56,32,
45,53,46,48,51,57,48,54,44,52,46,55,50,54,53,55,32,45,56,46,51,50,48,51,49,44,52,46,55,50,54,53,55,32,45,50,46,52,50,49,56,56,44,48,32,45,50,46,52,50,49,56,56,44,45,50,46,50,54,53,54,51,32,48,44,45,49,
46,54,52,48,54,50,32,50,46,50,50,54,53,55,44,45,52,46,49,52,48,54,50,32,50,46,49,56,55,53,44,45,50,46,52,54,48,57,52,32,54,46,56,51,53,57,51,44,45,53,46,53,48,55,56,50,32,52,46,54,52,56,52,52,44,45,51,
46,48,48,55,56,49,32,49,49,46,56,51,53,57,52,44,45,54,46,51,50,56,49,50,32,51,46,56,50,56,49,51,44,45,52,46,57,50,49,56,56,32,57,46,57,50,49,56,56,44,45,49,49,46,54,48,49,53,54,32,45,52,46,49,52,48,54,
51,44,51,46,50,48,51,49,50,32,45,53,46,52,54,56,55,53,44,52,46,50,49,56,55,53,32,45,49,46,51,50,56,49,51,44,49,46,48,49,53,54,50,32,45,51,46,49,54,52,48,55,44,50,46,51,48,52,54,56,32,45,49,46,56,51,53,
57,51,44,49,46,50,53,32,45,51,46,50,56,49,50,53,44,49,46,56,55,53,32,45,49,46,52,48,54,50,53,44,48,46,54,50,53,32,45,50,46,53,44,48,46,54,50,53,32,45,48,46,54,54,52,48,54,44,48,32,45,49,46,48,57,51,55,
53,44,45,48,46,52,50,57,54,56,32,45,48,46,52,50,57,54,56,44,45,48,46,52,50,57,54,57,32,45,48,46,52,50,57,54,56,44,45,49,46,49,55,49,56,56,32,48,44,45,49,46,50,56,57,48,54,32,48,46,53,48,55,56,49,44,45,
50,46,52,54,48,57,52,32,48,46,53,48,55,56,49,44,45,49,46,49,55,49,56,55,32,49,46,54,48,49,53,54,44,45,50,46,51,52,51,55,53,32,49,46,49,51,50,56,49,44,45,49,46,50,49,48,57,51,32,51,46,48,48,55,56,49,44,
45,50,46,54,49,55,49,56,32,51,46,51,53,57,51,56,44,45,50,46,53,32,51,46,51,53,57,51,56,44,45,51,46,56,54,55,49,57,32,48,44,45,48,46,55,48,51,49,51,32,45,48,46,55,56,49,50,53,44,45,48,46,55,48,51,49,51,
32,45,49,46,51,50,56,49,51,44,48,32,45,51,46,52,51,55,53,44,49,46,54,48,49,53,55,32,45,50,46,49,48,57,51,56,44,49,46,54,48,49,53,54,32,45,52,46,57,54,48,57,52,44,51,46,57,56,52,51,55,32,122,34,10,32,32,
32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,111,110,116,45,115,116,121,108,101,58,111,98,108,105,113,117,101,59,102,111,110,116,45,118,97,114,105,97,110,116,58,110,111,114,109,97,108,59,102,111,
110,116,45,119,101,105,103,104,116,58,110,111,114,109,97,108,59,102,111,110,116,45,115,116,114,101,116,99,104,58,110,111,114,109,97,108,59,102,111,110,116,45,115,105,122,101,58,56,48,112,120,59,108,105,
110,101,45,104,101,105,103,104,116,58,49,50,53,37,59,102,111,110,116,45,102,97,109,105,108,121,58,39,80,97,108,97,99,101,32,83,99,114,105,112,116,32,77,84,39,59,45,105,110,107,115,99,97,112,101,45,102,
111,110,116,45,115,112,101,99,105,102,105,99,97,116,105,111,110,58,39,80,97,108,97,99,101,32,83,99,114,105,112,116,32,77,84,32,79,98,108,105,113,117,101,39,59,116,101,120,116,45,97,108,105,103,110,58,
115,116,97,114,116,59,119,114,105,116,105,110,103,45,109,111,100,101,58,108,114,45,116,98,59,116,101,120,116,45,97,110,99,104,111,114,58,115,116,97,114,116,59,102,105,108,108,58,35,102,57,102,57,102,57,
34,10,32,32,32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,51,55,51,55,34,32,47,62,10,32,32,32,32,32,32,60,112,97,116,104,10,32,32,32,32,32,32,32,32,32,100,61,34,109,32,50,53,57,46,56,52,49,50,50,44,
53,52,52,46,49,55,53,55,56,32,118,32,45,48,46,54,50,53,32,113,32,53,46,57,51,55,53,44,45,53,46,53,48,55,56,49,32,56,46,51,50,48,51,49,44,45,53,46,53,48,55,56,49,32,49,46,51,50,56,49,50,44,48,32,49,46,
51,50,56,49,50,44,49,46,54,55,57,54,57,32,48,44,49,46,51,50,56,49,50,32,45,48,46,56,50,48,51,49,44,50,46,54,49,55,49,56,32,45,48,46,56,50,48,51,49,44,49,46,50,56,57,48,55,32,45,51,46,51,50,48,51,49,44,
52,46,52,57,50,49,57,32,52,46,56,52,51,55,53,44,45,52,46,48,50,51,52,52,32,56,46,51,57,56,52,52,44,45,54,46,52,48,54,50,53,32,51,46,53,53,52,54,56,44,45,50,46,51,56,50,56,49,32,54,46,48,57,51,55,53,44,
45,50,46,51,56,50,56,49,32,49,46,51,50,56,49,50,44,48,32,49,46,51,50,56,49,50,44,49,46,56,51,53,57,52,32,48,44,49,46,55,57,54,56,55,32,45,49,46,50,53,44,51,46,53,57,51,55,53,32,45,49,46,50,53,44,49,46,
55,53,55,56,49,32,45,52,46,49,48,49,53,54,44,51,46,56,50,56,49,50,32,45,51,46,48,52,54,56,56,44,50,46,51,48,52,54,57,32,45,51,46,48,52,54,56,56,44,51,46,50,56,49,50,53,32,48,44,48,46,51,57,48,54,51,32,
48,46,51,53,49,53,55,44,48,46,55,52,50,49,57,32,48,46,51,57,48,54,50,44,48,46,51,49,50,53,32,48,46,55,56,49,50,53,44,48,46,51,49,50,53,32,49,46,51,54,55,49,56,44,48,32,51,46,53,57,51,55,53,44,45,49,46,
52,56,52,51,56,32,50,46,50,54,53,54,50,44,45,49,46,53,50,51,52,51,32,52,46,57,50,49,56,55,44,45,51,46,56,50,56,49,50,32,50,46,54,53,54,50,53,44,45,50,46,51,52,51,55,53,32,52,46,50,57,54,56,56,44,45,51,
46,55,53,32,118,32,48,46,54,54,52,48,54,32,113,32,45,50,46,51,56,50,56,50,44,50,46,50,50,54,53,54,32,45,52,46,54,52,56,52,52,44,52,46,49,55,57,54,57,32,45,50,46,50,54,53,54,51,44,49,46,57,49,52,48,54,
32,45,52,46,53,51,49,50,53,44,51,46,51,53,57,51,55,32,45,50,46,50,50,54,53,54,44,49,46,52,48,54,50,53,32,45,51,46,55,49,48,57,52,44,49,46,52,48,54,50,53,32,45,48,46,57,51,55,53,44,48,32,45,49,46,51,54,
55,49,57,44,45,48,46,54,50,53,32,45,48,46,51,57,48,54,50,44,45,48,46,54,50,53,32,45,48,46,51,57,48,54,50,44,45,49,46,50,56,57,48,54,32,48,44,45,49,46,52,56,52,51,55,32,49,46,49,55,49,56,55,44,45,51,46,
51,50,48,51,49,32,49,46,49,55,49,56,56,44,45,49,46,56,55,53,32,51,46,50,52,50,49,57,44,45,51,46,54,51,50,56,49,32,50,46,51,56,50,56,49,44,45,50,46,49,48,57,51,56,32,51,46,50,56,49,50,53,44,45,51,46,51,
53,57,51,56,32,48,46,57,51,55,53,44,45,49,46,50,56,57,48,54,32,45,48,46,53,48,55,56,49,44,45,49,46,50,56,57,48,54,32,45,49,46,56,51,53,57,52,44,48,32,45,53,46,53,52,54,56,56,44,50,46,53,32,45,51,46,55,
49,48,57,51,44,50,46,52,54,48,57,52,32,45,54,46,57,53,51,49,50,44,53,46,50,51,52,51,55,32,45,51,46,50,52,50,49,57,44,50,46,55,51,52,51,56,32,45,54,46,49,51,50,56,49,44,53,46,51,57,48,54,51,32,104,32,45,
51,46,48,52,54,56,56,32,108,32,53,46,49,57,53,51,49,44,45,53,46,56,53,57,51,56,32,113,32,53,46,55,48,51,49,51,44,45,54,46,51,54,55,49,56,32,53,46,55,48,51,49,51,44,45,54,46,57,53,51,49,50,32,48,44,45,
48,46,51,57,48,54,51,32,45,48,46,52,54,56,55,53,44,45,48,46,51,57,48,54,51,32,45,49,46,55,49,56,55,53,44,48,32,45,56,46,49,54,52,48,54,44,53,46,53,56,53,57,52,32,122,34,10,32,32,32,32,32,32,32,32,32,115,
116,121,108,101,61,34,102,111,110,116,45,115,116,121,108,101,58,111,98,108,105,113,117,101,59,102,111,110,116,45,118,97,114,105,97,110,116,58,110,111,114,109,97,108,59,102,111,110,116,45,119,101,105,103,
104,116,58,110,111,114,109,97,108,59,102,111,110,116,45,115,116,114,101,116,99,104,58,110,111,114,109,97,108,59,102,111,110,116,45,115,105,122,101,58,56,48,112,120,59,108,105,110,101,45,104,101,105,103,
104,116,58,49,50,53,37,59,102,111,110,116,45,102,97,109,105,108,121,58,39,80,97,108,97,99,101,32,83,99,114,105,112,116,32,77,84,39,59,45,105,110,107,115,99,97,112,101,45,102,111,110,116,45,115,112,101,
99,105,102,105,99,97,116,105,111,110,58,39,80,97,108,97,99,101,32,83,99,114,105,112,116,32,77,84,32,79,98,108,105,113,117,101,39,59,116,101,120,116,45,97,108,105,103,110,58,115,116,97,114,116,59,119,114,
105,116,105,110,103,45,109,111,100,101,58,108,114,45,116,98,59,116,101,120,116,45,97,110,99,104,111,114,58,115,116,97,114,116,59,102,105,108,108,58,35,102,57,102,57,102,57,34,10,32,32,32,32,32,32,32,32,
32,105,100,61,34,112,97,116,104,51,55,51,57,34,32,47,62,10,32,32,32,32,32,32,60,112,97,116,104,10,32,32,32,32,32,32,32,32,32,100,61,34,109,32,50,57,55,46,51,48,50,49,53,44,53,51,56,46,53,56,57,56,52,32,
113,32,45,50,46,51,52,51,55,53,44,48,32,45,53,46,50,51,52,51,55,44,50,46,51,48,52,54,57,32,45,50,46,56,53,49,53,54,44,50,46,51,48,52,54,57,32,45,53,44,53,46,50,51,52,51,56,32,45,50,46,49,48,57,51,56,44,
50,46,57,50,57,54,56,32,45,50,46,49,48,57,51,56,44,52,46,49,52,48,54,50,32,48,44,48,46,53,56,53,57,52,32,48,46,51,57,48,54,51,44,48,46,57,55,54,53,54,32,48,46,51,57,48,54,50,44,48,46,51,57,48,54,51,32,
48,46,57,55,54,53,54,44,48,46,51,57,48,54,51,32,49,46,54,48,49,53,54,44,48,32,52,46,54,48,57,51,56,44,45,50,46,50,50,54,53,54,32,51,46,48,48,55,56,49,44,45,50,46,50,50,54,53,55,32,53,46,52,50,57,54,56,
44,45,53,46,48,51,57,48,55,32,50,46,52,50,49,56,56,44,45,50,46,56,49,50,53,32,50,46,52,50,49,56,56,44,45,52,46,52,49,52,48,54,32,48,44,45,49,46,51,54,55,49,57,32,45,49,46,52,56,52,51,56,44,45,49,46,51,
54,55,49,57,32,122,32,109,32,49,48,46,51,49,50,53,44,52,46,56,48,52,54,57,32,45,49,46,57,57,50,49,56,44,49,46,54,55,57,54,57,32,113,32,45,52,46,54,56,55,53,44,51,46,57,48,54,50,53,32,45,54,46,57,53,51,
49,51,44,53,46,53,48,55,56,49,32,45,50,46,50,50,54,53,54,44,49,46,54,48,49,53,54,32,45,51,46,57,52,53,51,49,44,49,46,54,48,49,53,54,32,45,49,46,49,51,50,56,49,44,48,32,45,49,46,49,51,50,56,49,44,45,49,
46,50,56,57,48,54,32,48,44,45,49,46,51,54,55,49,57,32,49,46,48,53,52,54,56,44,45,51,46,56,50,56,49,50,32,45,53,46,53,56,53,57,51,44,53,46,49,49,55,49,56,32,45,56,46,50,52,50,49,56,44,53,46,49,49,55,49,
56,32,45,49,46,49,55,49,56,56,44,48,32,45,49,46,57,49,52,48,55,44,45,48,46,56,57,56,52,51,32,45,48,46,55,52,50,49,56,44,45,48,46,57,51,55,53,32,45,48,46,55,52,50,49,56,44,45,50,46,54,57,53,51,50,32,48,
44,45,49,46,57,57,50,49,56,32,49,46,49,55,49,56,55,44,45,51,46,57,48,54,50,53,32,49,46,49,55,49,56,56,44,45,49,46,57,53,51,49,50,32,51,46,49,50,53,44,45,51,46,52,51,55,53,32,49,46,57,57,50,49,57,44,45,
49,46,52,56,52,51,55,32,52,46,52,49,52,48,54,44,45,50,46,51,52,51,55,53,32,50,46,52,54,48,57,52,44,45,48,46,56,53,57,51,55,32,52,46,55,54,53,54,51,44,45,48,46,56,53,57,51,55,32,49,46,49,51,50,56,49,44,
48,32,49,46,54,52,48,54,50,44,48,46,52,50,57,54,57,32,48,46,53,52,54,56,56,44,48,46,51,57,48,54,50,32,48,46,53,56,53,57,52,44,49,46,51,50,56,49,50,32,45,48,46,48,51,57,49,44,48,46,49,57,53,51,49,32,45,
48,46,48,55,56,49,44,48,46,51,57,48,54,51,32,48,44,48,46,49,53,54,50,53,32,45,48,46,48,55,56,49,44,48,46,52,54,56,55,53,32,45,48,46,48,51,57,49,44,48,46,51,49,50,53,32,45,48,46,48,55,56,49,44,48,46,53,
48,55,56,49,32,108,32,50,46,52,54,48,57,51,44,45,50,46,56,57,48,54,51,32,51,46,50,56,49,50,53,44,45,48,46,50,51,52,51,55,32,45,49,46,48,49,53,54,50,44,49,46,50,53,32,113,32,45,49,46,50,53,44,49,46,52,
56,52,51,55,32,45,50,46,48,51,49,50,53,44,50,46,52,54,48,57,52,32,45,48,46,55,56,49,50,53,44,48,46,57,51,55,53,32,45,49,46,51,50,56,49,51,44,49,46,53,54,50,53,32,45,54,46,48,57,51,55,53,44,54,46,54,48,
49,53,54,32,45,54,46,48,57,51,55,53,44,55,46,55,51,52,51,55,32,48,44,48,46,53,56,53,57,52,32,48,46,53,52,54,56,56,44,48,46,53,56,53,57,52,32,48,46,55,52,50,49,57,44,48,32,50,46,50,50,54,53,54,44,45,48,
46,56,53,57,51,56,32,49,46,52,56,52,51,56,44,45,48,46,56,57,56,52,51,32,51,46,51,50,48,51,49,44,45,50,46,51,52,51,55,53,32,49,46,56,55,53,44,45,49,46,52,52,53,51,49,32,52,46,50,53,55,56,50,44,45,51,46,
51,57,56,52,51,32,50,46,51,56,50,56,49,44,45,49,46,57,57,50,49,57,32,50,46,55,55,51,52,51,44,45,50,46,51,48,52,54,57,32,122,34,10,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,111,110,116,45,
115,116,121,108,101,58,111,98,108,105,113,117,101,59,102,111,110,116,45,118,97,114,105,97,110,116,58,110,111,114,109,97,108,59,102,111,110,116,45,119,101,105,103,104,116,58,110,111,114,109,97,108,59,102,
111,110,116,45,115,116,114,101,116,99,104,58,110,111,114,109,97,108,59,102,111,110,116,45,115,105,122,101,58,56,48,112,120,59,108,105,110,101,45,104,101,105,103,104,116,58,49,50,53,37,59,102,111,110,116,
45,102,97,109,105,108,121,58,39,80,97,108,97,99,101,32,83,99,114,105,112,116,32,77,84,39,59,45,105,110,107,115,99,97,112,101,45,102,111,110,116,45,115,112,101,99,105,102,105,99,97,116,105,111,110,58,39,
80,97,108,97,99,101,32,83,99,114,105,112,116,32,77,84,32,79,98,108,105,113,117,101,39,59,116,101,120,116,45,97,108,105,103,110,58,115,116,97,114,116,59,119,114,105,116,105,110,103,45,109,111,100,101,58,
108,114,45,116,98,59,116,101,120,116,45,97,110,99,104,111,114,58,115,116,97,114,116,59,102,105,108,108,58,35,102,57,102,57,102,57,34,10,32,32,32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,51,55,52,
49,34,32,47,62,10,32,32,32,32,32,32,60,112,97,116,104,10,32,32,32,32,32,32,32,32,32,100,61,34,109,32,51,48,54,46,54,51,56,48,57,44,53,52,52,46,49,55,53,55,56,32,118,32,45,48,46,55,48,51,49,50,32,113,32,
50,46,56,57,48,54,51,44,45,50,46,54,49,55,49,57,32,52,46,56,52,51,55,53,44,45,52,46,48,50,51,52,52,32,49,46,57,53,51,49,51,44,45,49,46,52,48,54,50,53,32,51,46,52,55,54,53,54,44,45,49,46,52,48,54,50,53,
32,49,46,50,53,44,48,32,49,46,50,53,44,49,46,54,55,57,54,57,32,48,44,49,46,52,48,54,50,53,32,45,48,46,56,50,48,51,49,44,50,46,55,51,52,51,55,32,45,48,46,56,50,48,51,49,44,49,46,50,56,57,48,54,32,45,50,
46,55,55,51,52,52,44,51,46,55,53,32,53,46,52,54,56,55,53,44,45,52,46,52,57,50,49,57,32,56,46,50,48,51,49,51,44,45,54,46,51,50,56,49,50,32,50,46,55,55,51,52,52,44,45,49,46,56,51,53,57,52,32,52,46,55,54,
53,54,50,44,45,49,46,56,51,53,57,52,32,49,46,52,52,53,51,50,44,48,32,49,46,52,52,53,51,50,44,49,46,51,50,56,49,50,32,48,44,50,46,54,53,54,50,53,32,45,51,46,57,48,54,50,53,44,55,46,51,48,52,54,57,32,108,
32,49,46,48,57,51,55,53,44,45,48,46,56,50,48,51,49,32,113,32,50,46,54,49,55,49,56,44,45,49,46,57,49,52,48,54,32,53,44,45,51,46,54,55,49,56,56,32,50,46,51,56,50,56,49,44,45,49,46,55,53,55,56,49,32,52,46,
53,55,48,51,49,44,45,50,46,57,50,57,54,56,32,50,46,50,50,54,53,54,44,45,49,46,50,49,48,57,52,32,51,46,53,53,52,54,57,44,45,49,46,50,49,48,57,52,32,48,46,54,50,53,44,48,32,48,46,57,55,54,53,54,44,48,46,
53,48,55,56,49,32,48,46,51,57,48,54,50,44,48,46,52,54,56,55,53,32,48,46,51,57,48,54,50,44,49,46,52,48,54,50,53,32,48,44,49,46,55,57,54,56,56,32,45,49,46,50,49,48,57,51,44,51,46,53,53,52,54,57,32,45,49,
46,49,55,49,56,56,44,49,46,55,49,56,55,53,32,45,52,46,48,54,50,53,44,51,46,56,54,55,49,57,32,45,49,46,49,55,49,56,56,44,48,46,56,53,57,51,55,32,45,50,46,50,50,54,53,55,44,49,46,57,57,50,49,56,32,45,49,
46,48,53,52,54,56,44,49,46,49,51,50,56,50,32,45,49,46,48,53,52,54,56,44,49,46,53,50,51,52,52,32,48,44,48,46,55,52,50,49,57,32,49,46,48,57,51,55,53,44,48,46,55,52,50,49,57,32,49,46,55,49,56,55,53,44,48,
32,52,46,53,51,49,50,53,44,45,50,46,48,55,48,51,49,32,50,46,56,53,49,53,54,44,45,50,46,49,48,57,51,56,32,56,46,52,51,55,53,44,45,54,46,56,51,53,57,52,32,118,32,48,46,54,54,52,48,54,32,113,32,45,53,46,
51,49,50,53,44,52,46,53,55,48,51,49,32,45,56,46,49,54,52,48,55,44,54,46,54,55,57,54,57,32,45,50,46,56,49,50,53,44,50,46,49,48,57,51,55,32,45,52,46,57,54,48,57,51,44,50,46,49,48,57,51,55,32,45,49,46,53,
50,51,52,52,44,48,32,45,49,46,53,50,51,52,52,44,45,49,46,53,50,51,52,51,32,48,44,45,49,46,54,55,57,54,57,32,49,46,49,51,50,56,49,44,45,51,46,51,57,56,52,52,32,49,46,49,51,50,56,49,44,45,49,46,55,49,56,
55,53,32,51,46,56,50,56,49,51,44,45,52,46,50,53,55,56,49,32,51,46,49,50,53,44,45,50,46,57,50,57,54,57,32,51,46,49,50,53,44,45,51,46,55,49,48,57,52,32,48,44,45,48,46,55,48,51,49,51,32,45,48,46,54,50,53,
44,45,48,46,55,48,51,49,51,32,45,49,46,48,49,53,54,51,44,48,32,45,52,46,52,49,52,48,55,44,50,46,49,56,55,53,32,45,51,46,51,57,56,52,51,44,50,46,49,52,56,52,52,32,45,55,46,53,51,57,48,54,44,53,46,51,53,
49,53,55,32,45,52,46,49,48,49,53,54,44,51,46,49,54,52,48,54,32,45,54,46,55,53,55,56,49,44,53,46,54,54,52,48,54,32,104,32,45,51,46,50,56,49,50,53,32,108,32,54,46,48,53,52,54,57,44,45,54,46,54,52,48,54,
51,32,113,32,48,46,54,50,53,44,45,48,46,54,54,52,48,54,32,49,46,56,55,53,44,45,49,46,57,57,50,49,56,32,49,46,50,56,57,48,54,44,45,49,46,51,50,56,49,51,32,50,46,49,56,55,53,44,45,50,46,51,56,50,56,50,32,
48,46,56,57,56,52,51,44,45,49,46,48,57,51,55,53,32,48,46,56,57,56,52,51,44,45,49,46,52,56,52,51,55,32,48,44,45,48,46,49,57,53,51,49,32,45,48,46,49,57,53,51,49,44,45,48,46,51,57,48,54,51,32,45,48,46,49,
53,54,50,53,44,45,48,46,50,51,52,51,55,32,45,48,46,53,48,55,56,49,44,45,48,46,50,51,52,51,55,32,45,50,46,49,56,55,53,44,48,32,45,54,46,53,50,51,52,52,44,51,46,51,57,56,52,52,32,45,52,46,51,51,53,57,52,
44,51,46,51,57,56,52,51,32,45,49,48,46,49,57,53,51,49,44,56,46,54,55,49,56,55,32,108,32,45,49,46,49,51,50,56,49,44,49,46,48,53,52,54,57,32,104,32,45,51,46,51,53,57,51,56,32,108,32,56,46,50,52,50,49,57,
44,45,57,46,50,49,56,55,53,32,113,32,50,46,56,49,50,53,44,45,50,46,56,57,48,54,51,32,50,46,56,49,50,53,44,45,51,46,52,51,55,53,32,48,44,45,48,46,53,52,54,56,56,32,45,48,46,52,54,56,55,53,44,45,48,46,53,
52,54,56,56,32,45,48,46,54,54,52,48,54,44,48,32,45,49,46,57,57,50,49,57,44,48,46,56,53,57,51,56,32,45,49,46,51,50,56,49,50,44,48,46,56,50,48,51,49,32,45,50,46,56,57,48,54,50,44,50,46,48,55,48,51,49,32,
45,49,46,53,54,50,53,44,49,46,50,53,32,45,51,46,50,48,51,49,51,44,50,46,54,53,54,50,53,32,122,34,10,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,111,110,116,45,115,116,121,108,101,58,111,98,
108,105,113,117,101,59,102,111,110,116,45,118,97,114,105,97,110,116,58,110,111,114,109,97,108,59,102,111,110,116,45,119,101,105,103,104,116,58,110,111,114,109,97,108,59,102,111,110,116,45,115,116,114,
101,116,99,104,58,110,111,114,109,97,108,59,102,111,110,116,45,115,105,122,101,58,56,48,112,120,59,108,105,110,101,45,104,101,105,103,104,116,58,49,50,53,37,59,102,111,110,116,45,102,97,109,105,108,121,
58,39,80,97,108,97,99,101,32,83,99,114,105,112,116,32,77,84,39,59,45,105,110,107,115,99,97,112,101,45,102,111,110,116,45,115,112,101,99,105,102,105,99,97,116,105,111,110,58,39,80,97,108,97,99,101,32,83,
99,114,105,112,116,32,77,84,32,79,98,108,105,113,117,101,39,59,116,101,120,116,45,97,108,105,103,110,58,115,116,97,114,116,59,119,114,105,116,105,110,103,45,109,111,100,101,58,108,114,45,116,98,59,116,
101,120,116,45,97,110,99,104,111,114,58,115,116,97,114,116,59,102,105,108,108,58,35,102,57,102,57,102,57,34,10,32,32,32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,51,55,52,51,34,32,47,62,10,32,32,
32,32,32,32,60,112,97,116,104,10,32,32,32,32,32,32,32,32,32,100,61,34,109,32,51,53,54,46,57,53,48,53,57,44,53,51,48,46,49,49,51,50,56,32,113,32,48,46,56,53,57,51,56,44,48,32,48,46,56,53,57,51,56,44,49,
46,48,53,52,54,57,32,48,44,48,46,54,50,53,32,45,48,46,51,53,49,53,55,44,49,46,48,49,53,54,50,32,45,48,46,51,53,49,53,54,44,48,46,51,57,48,54,51,32,45,48,46,56,57,56,52,51,44,48,46,51,57,48,54,51,32,45,
49,46,48,53,52,54,57,44,48,32,45,49,46,48,53,52,54,57,44,45,48,46,57,51,55,53,32,48,44,45,48,46,53,52,54,56,56,32,48,46,52,50,57,54,57,44,45,49,46,48,49,53,54,51,32,48,46,52,50,57,54,56,44,45,48,46,53,
48,55,56,49,32,49,46,48,49,53,54,50,44,45,48,46,53,48,55,56,49,32,122,32,109,32,45,49,51,46,53,49,53,54,50,44,49,51,46,57,56,52,51,56,32,45,48,46,48,55,56,49,44,45,48,46,54,50,53,32,113,32,50,46,57,54,
56,55,53,44,45,50,46,54,57,53,51,50,32,54,46,57,53,51,49,51,44,45,53,46,49,49,55,49,57,32,104,32,51,46,53,49,53,54,50,32,108,32,45,57,46,50,49,56,55,53,44,49,48,46,51,49,50,53,32,113,32,45,48,46,57,55,
54,53,54,44,49,46,48,57,51,55,53,32,45,49,46,50,53,44,50,46,48,51,49,50,53,32,45,48,46,50,55,51,52,52,44,48,46,57,51,55,53,32,48,46,52,54,56,55,53,44,48,46,57,51,55,53,32,49,46,57,53,51,49,51,44,48,32,
53,46,56,50,48,51,49,44,45,50,46,56,57,48,54,51,32,51,46,56,54,55,49,57,44,45,50,46,56,57,48,54,50,32,55,46,49,52,56,52,52,44,45,54,46,48,57,51,55,53,32,118,32,48,46,54,54,52,48,55,32,113,32,45,51,46,
55,56,57,48,54,44,51,46,54,51,50,56,49,32,45,55,46,51,48,52,54,57,44,54,46,50,53,32,45,51,46,53,49,53,54,50,44,50,46,54,49,55,49,56,32,45,53,46,55,52,50,49,56,44,50,46,54,49,55,49,56,32,45,49,46,50,56,
57,48,55,44,48,32,45,49,46,50,56,57,48,55,44,45,49,46,51,54,55,49,56,32,48,44,45,52,46,48,54,50,53,32,54,46,51,50,56,49,51,44,45,49,48,46,51,57,48,54,51,32,48,46,51,53,49,53,54,44,45,48,46,51,53,49,53,
54,32,48,46,54,50,53,44,45,48,46,54,54,52,48,54,32,48,46,51,49,50,53,44,45,48,46,51,53,49,53,54,32,48,46,53,56,53,57,52,44,45,48,46,54,50,53,32,45,51,46,53,49,53,54,51,44,50,46,49,48,57,51,55,32,45,54,
46,53,54,50,53,44,52,46,57,54,48,57,52,32,122,34,10,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,111,110,116,45,115,116,121,108,101,58,111,98,108,105,113,117,101,59,102,111,110,116,45,118,
97,114,105,97,110,116,58,110,111,114,109,97,108,59,102,111,110,116,45,119,101,105,103,104,116,58,110,111,114,109,97,108,59,102,111,110,116,45,115,116,114,101,116,99,104,58,110,111,114,109,97,108,59,102,
111,110,116,45,115,105,122,101,58,56,48,112,120,59,108,105,110,101,45,104,101,105,103,104,116,58,49,50,53,37,59,102,111,110,116,45,102,97,109,105,108,121,58,39,80,97,108,97,99,101,32,83,99,114,105,112,
116,32,77,84,39,59,45,105,110,107,115,99,97,112,101,45,102,111,110,116,45,115,112,101,99,105,102,105,99,97,116,105,111,110,58,39,80,97,108,97,99,101,32,83,99,114,105,112,116,32,77,84,32,79,98,108,105,
113,117,101,39,59,116,101,120,116,45,97,108,105,103,110,58,115,116,97,114,116,59,119,114,105,116,105,110,103,45,109,111,100,101,58,108,114,45,116,98,59,116,101,120,116,45,97,110,99,104,111,114,58,115,
116,97,114,116,59,102,105,108,108,58,35,102,57,102,57,102,57,34,10,32,32,32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,51,55,52,53,34,32,47,62,10,32,32,32,32,32,32,60,112,97,116,104,10,32,32,32,32,
32,32,32,32,32,100,61,34,109,32,51,55,50,46,54,53,51,55,50,44,53,52,50,46,53,55,52,50,50,32,118,32,48,46,54,54,52,48,54,32,113,32,45,57,46,50,53,55,56,50,44,56,46,57,52,53,51,49,32,45,49,53,46,52,50,57,
54,57,44,56,46,57,52,53,51,49,32,45,49,46,54,52,48,54,51,44,48,32,45,50,46,51,56,50,56,49,44,45,48,46,56,50,48,51,49,32,45,48,46,55,52,50,49,57,44,45,48,46,56,53,57,51,55,32,45,48,46,55,52,50,49,57,44,
45,51,46,48,48,55,56,49,32,48,44,45,49,46,56,55,53,32,49,46,50,49,48,57,52,44,45,51,46,55,56,57,48,54,32,49,46,49,55,49,56,55,44,45,49,46,57,49,52,48,55,32,51,46,49,50,53,44,45,51,46,51,53,57,51,56,32,
49,46,57,53,51,49,50,44,45,49,46,52,56,52,51,55,32,52,46,50,57,54,56,55,44,45,50,46,51,48,52,54,57,32,50,46,51,52,51,55,53,44,45,48,46,56,53,57,51,55,32,52,46,53,55,48,51,49,44,45,48,46,56,53,57,51,55,
32,49,46,48,49,53,54,51,44,48,32,49,46,54,52,48,54,51,44,48,46,53,48,55,56,49,32,48,46,54,54,52,48,54,44,48,46,53,48,55,56,49,32,48,46,54,54,52,48,54,44,49,46,52,48,54,50,53,32,48,44,49,46,48,49,53,54,
51,32,45,48,46,56,50,48,51,49,44,49,46,56,51,53,57,52,32,45,48,46,55,56,49,50,53,44,48,46,55,56,49,50,53,32,45,49,46,55,53,55,56,49,44,48,46,55,56,49,50,53,32,45,49,46,49,55,49,56,56,44,48,32,45,49,46,
49,55,49,56,56,44,45,49,46,48,49,53,54,51,32,48,44,45,49,46,53,50,51,52,51,32,49,46,51,54,55,49,57,44,45,49,46,53,50,51,52,51,32,48,46,55,56,49,50,53,44,48,32,48,46,57,51,55,53,44,48,46,55,48,51,49,50,
32,48,46,49,53,54,50,53,44,48,46,50,51,52,51,56,32,48,46,51,49,50,53,44,48,46,50,51,52,51,56,32,48,46,49,57,53,51,49,44,48,32,48,46,51,57,48,54,50,44,45,48,46,51,49,50,53,32,48,46,49,57,53,51,50,44,45,
48,46,51,49,50,53,32,48,46,49,57,53,51,50,44,45,48,46,55,48,51,49,51,32,48,44,45,48,46,54,50,53,32,45,48,46,52,50,57,54,57,44,45,48,46,57,55,54,53,54,32,45,48,46,52,50,57,54,57,44,45,48,46,51,57,48,54,
51,32,45,49,46,49,55,49,56,56,44,45,48,46,51,57,48,54,51,32,45,50,46,48,55,48,51,49,44,48,32,45,53,46,48,51,57,48,54,44,50,46,50,50,54,53,55,32,45,50,46,57,50,57,54,57,44,50,46,50,50,54,53,54,32,45,53,
46,48,55,56,49,50,44,53,46,48,55,56,49,50,32,45,50,46,49,52,56,52,52,44,50,46,56,53,49,53,54,32,45,50,46,49,52,56,52,52,44,52,46,52,53,51,49,51,32,48,44,49,46,50,56,57,48,54,32,49,46,57,49,52,48,54,44,
49,46,50,56,57,48,54,32,51,46,53,53,52,54,57,44,48,32,54,46,57,49,52,48,54,44,45,50,46,49,56,55,53,32,51,46,51,53,57,51,56,44,45,50,46,49,56,55,53,32,56,46,54,51,50,56,50,44,45,54,46,56,55,53,32,122,34,
10,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,111,110,116,45,115,116,121,108,101,58,111,98,108,105,113,117,101,59,102,111,110,116,45,118,97,114,105,97,110,116,58,110,111,114,109,97,108,59,
102,111,110,116,45,119,101,105,103,104,116,58,110,111,114,109,97,108,59,102,111,110,116,45,115,116,114,101,116,99,104,58,110,111,114,109,97,108,59,102,111,110,116,45,115,105,122,101,58,56,48,112,120,59,
108,105,110,101,45,104,101,105,103,104,116,58,49,50,53,37,59,102,111,110,116,45,102,97,109,105,108,121,58,39,80,97,108,97,99,101,32,83,99,114,105,112,116,32,77,84,39,59,45,105,110,107,115,99,97,112,101,
45,102,111,110,116,45,115,112,101,99,105,102,105,99,97,116,105,111,110,58,39,80,97,108,97,99,101,32,83,99,114,105,112,116,32,77,84,32,79,98,108,105,113,117,101,39,59,116,101,120,116,45,97,108,105,103,
110,58,115,116,97,114,116,59,119,114,105,116,105,110,103,45,109,111,100,101,58,108,114,45,116,98,59,116,101,120,116,45,97,110,99,104,111,114,58,115,116,97,114,116,59,102,105,108,108,58,35,102,57,102,57,
102,57,34,10,32,32,32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,51,55,52,55,34,32,47,62,10,32,32,32,32,32,32,60,112,97,116,104,10,32,32,32,32,32,32,32,32,32,100,61,34,109,32,51,55,49,46,55,57,52,
51,52,44,53,52,51,46,53,53,48,55,56,32,56,46,49,54,52,48,54,44,45,55,46,48,51,49,50,53,32,113,32,48,46,57,51,55,53,44,45,49,46,49,51,50,56,49,32,49,46,55,49,56,55,53,44,45,50,46,48,55,48,51,49,32,48,46,
55,56,49,50,53,44,45,48,46,57,55,54,53,54,32,49,46,49,51,50,56,50,44,45,48,46,57,55,54,53,54,32,48,46,51,49,50,53,44,48,32,48,46,53,52,54,56,55,44,48,46,49,57,53,51,49,32,48,46,50,55,51,52,52,44,48,46,
49,53,54,50,53,32,48,46,50,55,51,52,52,44,48,46,53,48,55,56,49,32,48,44,48,46,55,52,50,49,57,32,45,50,46,49,52,56,52,52,44,49,46,56,51,53,57,52,32,49,46,50,53,44,49,46,56,55,53,32,49,46,55,57,54,56,56,
44,51,46,50,56,49,50,53,32,48,46,53,52,54,56,55,44,49,46,52,48,54,50,53,32,48,46,54,54,52,48,54,44,51,46,53,53,52,54,57,32,45,48,46,49,49,55,49,57,44,49,46,51,50,56,49,50,32,45,48,46,51,49,50,53,44,50,
46,50,50,54,53,54,32,49,46,52,48,54,50,53,44,48,46,51,53,49,53,54,32,50,46,53,51,57,48,54,44,45,48,46,48,51,57,49,32,49,46,49,55,49,56,56,44,45,48,46,52,50,57,54,57,32,51,46,50,48,51,49,51,44,45,50,46,
52,54,48,57,52,32,118,32,48,46,54,54,52,48,54,32,113,32,45,49,46,53,54,50,53,44,49,46,57,53,51,49,51,32,45,50,46,56,57,48,54,51,44,50,46,52,50,49,56,56,32,45,49,46,51,50,56,49,50,44,48,46,52,54,56,55,
53,32,45,51,46,48,56,53,57,52,44,45,48,46,48,51,57,49,32,45,48,46,57,51,55,53,44,50,46,49,48,57,51,56,32,45,51,46,48,52,54,56,55,44,51,46,54,51,50,56,50,32,45,50,46,49,48,57,51,56,44,49,46,52,56,52,51,
55,32,45,52,46,51,51,53,57,52,44,50,46,50,50,54,53,54,32,45,50,46,50,50,54,53,54,44,48,46,55,48,51,49,50,32,45,51,46,56,50,56,49,50,44,48,46,55,48,51,49,50,32,45,49,46,53,50,51,52,52,44,48,32,45,50,46,
52,54,48,57,52,44,45,48,46,53,56,53,57,51,32,45,48,46,57,55,54,53,54,44,45,48,46,54,50,53,32,45,48,46,57,55,54,53,54,44,45,49,46,55,49,56,55,53,32,48,44,45,48,46,57,55,54,53,55,32,48,46,53,56,53,57,51,
44,45,49,46,53,54,50,53,32,48,46,53,52,54,56,56,44,45,48,46,54,50,53,32,49,46,54,52,48,54,51,44,45,48,46,54,50,53,32,48,46,55,48,51,49,50,44,48,32,49,46,49,55,49,56,55,44,48,46,51,57,48,54,50,32,48,46,
52,50,57,54,57,44,48,46,51,53,49,53,54,32,48,46,52,50,57,54,57,44,48,46,57,55,54,53,54,32,48,44,48,46,53,48,55,56,50,32,45,48,46,52,54,56,55,53,44,48,46,56,57,56,52,52,32,45,48,46,53,48,55,56,49,44,48,
46,51,57,48,54,51,32,45,49,46,48,53,52,54,57,44,48,46,51,57,48,54,51,32,45,48,46,55,52,50,49,56,44,48,32,45,49,46,48,57,51,55,53,44,45,49,46,50,49,48,57,52,32,45,48,46,53,48,55,56,49,44,48,46,51,53,49,
53,54,32,45,48,46,53,48,55,56,49,44,49,46,48,53,52,54,57,32,48,44,48,46,52,50,57,54,56,32,48,46,56,50,48,51,49,44,48,46,57,51,55,53,32,48,46,55,56,49,50,53,44,48,46,53,48,55,56,49,32,49,46,56,51,53,57,
52,44,48,46,53,48,55,56,49,32,50,46,48,55,48,51,49,44,48,32,52,46,51,55,53,44,45,49,46,55,53,55,56,49,32,50,46,51,48,52,54,57,44,45,49,46,55,57,54,56,56,32,51,46,56,50,56,49,51,44,45,52,46,52,57,50,49,
57,32,49,46,53,54,50,53,44,45,50,46,54,57,53,51,49,32,49,46,53,54,50,53,44,45,53,46,49,57,53,51,49,32,48,44,45,49,46,55,57,54,56,56,32,45,49,46,49,51,50,56,50,44,45,51,46,54,55,49,56,56,32,108,32,45,56,
46,57,52,53,51,49,44,55,46,54,53,54,50,53,32,122,34,10,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,111,110,116,45,115,116,121,108,101,58,111,98,108,105,113,117,101,59,102,111,110,116,45,118,
97,114,105,97,110,116,58,110,111,114,109,97,108,59,102,111,110,116,45,119,101,105,103,104,116,58,110,111,114,109,97,108,59,102,111,110,116,45,115,116,114,101,116,99,104,58,110,111,114,109,97,108,59,102,
111,110,116,45,115,105,122,101,58,56,48,112,120,59,108,105,110,101,45,104,101,105,103,104,116,58,49,50,53,37,59,102,111,110,116,45,102,97,109,105,108,121,58,39,80,97,108,97,99,101,32,83,99,114,105,112,
116,32,77,84,39,59,45,105,110,107,115,99,97,112,101,45,102,111,110,116,45,115,112,101,99,105,102,105,99,97,116,105,111,110,58,39,80,97,108,97,99,101,32,83,99,114,105,112,116,32,77,84,32,79,98,108,105,
113,117,101,39,59,116,101,120,116,45,97,108,105,103,110,58,115,116,97,114,116,59,119,114,105,116,105,110,103,45,109,111,100,101,58,108,114,45,116,98,59,116,101,120,116,45,97,110,99,104,111,114,58,115,
116,97,114,116,59,102,105,108,108,58,35,102,57,102,57,102,57,34,10,32,32,32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,51,55,52,57,34,32,47,62,10,32,32,32,32,60,47,103,62,10,32,32,60,47,103,62,10,
60,47,115,118,103,62,10,0,0};

const char* VASTWaveTableEditorViewHeader::dynamics_svg = (const char*) resource_VASTWaveTableEditorViewHeader_dynamics_svg;
const int VASTWaveTableEditorViewHeader::dynamics_svgSize = 13088;

// JUCER_RESOURCE: waves_samplehold_svg, 3506, "../../Resources/Icons/waves_samplehold.svg"
static const unsigned char resource_VASTWaveTableEditorViewHeader_waves_samplehold_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,
85,84,70,45,56,34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,10,32,32,32,120,109,108,110,115,58,100,99,61,34,104,116,116,112,58,47,47,112,117,114,108,46,111,114,
103,47,100,99,47,101,108,101,109,101,110,116,115,47,49,46,49,47,34,10,32,32,32,120,109,108,110,115,58,99,99,61,34,104,116,116,112,58,47,47,99,114,101,97,116,105,118,101,99,111,109,109,111,110,115,46,111,
114,103,47,110,115,35,34,10,32,32,32,120,109,108,110,115,58,114,100,102,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,48,50,47,50,50,45,114,100,102,45,115,121,110,
116,97,120,45,110,115,35,34,10,32,32,32,120,109,108,110,115,58,115,118,103,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,118,103,34,10,32,32,32,120,109,108,
110,115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,118,103,34,10,32,32,32,120,109,108,110,115,58,115,111,100,105,112,111,100,105,61,34,104,116,116,112,58,
47,47,115,111,100,105,112,111,100,105,46,115,111,117,114,99,101,102,111,114,103,101,46,110,101,116,47,68,84,68,47,115,111,100,105,112,111,100,105,45,48,46,100,116,100,34,10,32,32,32,120,109,108,110,115,
58,105,110,107,115,99,97,112,101,61,34,104,116,116,112,58,47,47,119,119,119,46,105,110,107,115,99,97,112,101,46,111,114,103,47,110,97,109,101,115,112,97,99,101,115,47,105,110,107,115,99,97,112,101,34,
10,32,32,32,119,105,100,116,104,61,34,50,48,112,120,34,10,32,32,32,104,101,105,103,104,116,61,34,50,48,112,120,34,10,32,32,32,118,105,101,119,66,111,120,61,34,48,32,48,32,50,48,32,50,48,34,10,32,32,32,
118,101,114,115,105,111,110,61,34,49,46,49,34,10,32,32,32,105,100,61,34,115,118,103,49,50,34,10,32,32,32,115,111,100,105,112,111,100,105,58,100,111,99,110,97,109,101,61,34,119,97,118,101,115,95,115,97,
109,112,108,101,104,111,108,100,46,115,118,103,34,10,32,32,32,105,110,107,115,99,97,112,101,58,118,101,114,115,105,111,110,61,34,48,46,57,50,46,51,32,40,50,52,48,53,53,52,54,44,32,50,48,49,56,45,48,51,
45,49,49,41,34,62,10,32,32,60,109,101,116,97,100,97,116,97,10,32,32,32,32,32,105,100,61,34,109,101,116,97,100,97,116,97,49,54,34,62,10,32,32,32,32,60,114,100,102,58,82,68,70,62,10,32,32,32,32,32,32,60,
99,99,58,87,111,114,107,10,32,32,32,32,32,32,32,32,32,114,100,102,58,97,98,111,117,116,61,34,34,62,10,32,32,32,32,32,32,32,32,60,100,99,58,102,111,114,109,97,116,62,105,109,97,103,101,47,115,118,103,43,
120,109,108,60,47,100,99,58,102,111,114,109,97,116,62,10,32,32,32,32,32,32,32,32,60,100,99,58,116,121,112,101,10,32,32,32,32,32,32,32,32,32,32,32,114,100,102,58,114,101,115,111,117,114,99,101,61,34,104,
116,116,112,58,47,47,112,117,114,108,46,111,114,103,47,100,99,47,100,99,109,105,116,121,112,101,47,83,116,105,108,108,73,109,97,103,101,34,32,47,62,10,32,32,32,32,32,32,32,32,60,100,99,58,116,105,116,
108,101,62,115,116,97,116,115,32,91,35,49,49,57,48,93,60,47,100,99,58,116,105,116,108,101,62,10,32,32,32,32,32,32,60,47,99,99,58,87,111,114,107,62,10,32,32,32,32,60,47,114,100,102,58,82,68,70,62,10,32,
32,60,47,109,101,116,97,100,97,116,97,62,10,32,32,60,115,111,100,105,112,111,100,105,58,110,97,109,101,100,118,105,101,119,10,32,32,32,32,32,112,97,103,101,99,111,108,111,114,61,34,35,57,53,57,53,57,53,
34,10,32,32,32,32,32,98,111,114,100,101,114,99,111,108,111,114,61,34,35,54,54,54,54,54,54,34,10,32,32,32,32,32,98,111,114,100,101,114,111,112,97,99,105,116,121,61,34,49,34,10,32,32,32,32,32,111,98,106,
101,99,116,116,111,108,101,114,97,110,99,101,61,34,49,48,34,10,32,32,32,32,32,103,114,105,100,116,111,108,101,114,97,110,99,101,61,34,49,48,34,10,32,32,32,32,32,103,117,105,100,101,116,111,108,101,114,
97,110,99,101,61,34,49,48,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,112,97,103,101,111,112,97,99,105,116,121,61,34,49,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,112,97,103,101,115,
104,97,100,111,119,61,34,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,119,105,100,116,104,61,34,51,56,52,48,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,
119,105,110,100,111,119,45,104,101,105,103,104,116,61,34,50,48,53,48,34,10,32,32,32,32,32,105,100,61,34,110,97,109,101,100,118,105,101,119,49,52,34,10,32,32,32,32,32,115,104,111,119,103,114,105,100,61,
34,102,97,108,115,101,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,122,111,111,109,61,34,51,51,46,51,55,53,52,52,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,120,61,34,49,57,46,50,
56,56,48,55,52,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,121,61,34,50,54,46,54,49,56,49,51,52,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,120,61,34,
45,49,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,121,61,34,45,49,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,109,97,120,
105,109,105,122,101,100,61,34,49,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,117,114,114,101,110,116,45,108,97,121,101,114,61,34,115,118,103,49,50,34,32,47,62,10,32,32,60,33,45,45,32,71,101,
110,101,114,97,116,111,114,58,32,83,107,101,116,99,104,32,51,46,56,46,49,32,40,50,57,54,56,55,41,32,45,32,104,116,116,112,58,47,47,119,119,119,46,98,111,104,101,109,105,97,110,99,111,100,105,110,103,46,
99,111,109,47,115,107,101,116,99,104,32,45,45,62,10,32,32,60,116,105,116,108,101,10,32,32,32,32,32,105,100,61,34,116,105,116,108,101,50,34,62,115,116,97,116,115,32,91,35,49,49,57,48,93,60,47,116,105,116,
108,101,62,10,32,32,60,100,101,115,99,10,32,32,32,32,32,105,100,61,34,100,101,115,99,52,34,62,67,114,101,97,116,101,100,32,119,105,116,104,32,83,107,101,116,99,104,46,60,47,100,101,115,99,62,10,32,32,
60,100,101,102,115,10,32,32,32,32,32,105,100,61,34,100,101,102,115,54,34,32,47,62,10,32,32,60,103,10,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,45,49,
52,48,44,45,50,56,51,57,41,34,10,32,32,32,32,32,105,100,61,34,68,114,105,98,98,98,108,101,45,76,105,103,104,116,45,80,114,101,118,105,101,119,34,10,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,
108,58,35,102,102,102,102,102,102,59,102,105,108,108,45,114,117,108,101,58,101,118,101,110,111,100,100,59,115,116,114,111,107,101,58,110,111,110,101,59,115,116,114,111,107,101,45,119,105,100,116,104,58,
49,59,102,105,108,108,45,111,112,97,99,105,116,121,58,48,46,57,57,55,50,56,57,57,54,34,62,10,32,32,32,32,60,103,10,32,32,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,
97,116,101,40,53,54,44,49,54,48,41,34,10,32,32,32,32,32,32,32,105,100,61,34,105,99,111,110,115,34,10,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,102,102,102,102,102,102,59,102,
105,108,108,45,111,112,97,99,105,116,121,58,48,46,57,57,55,50,56,57,57,54,34,62,10,32,32,32,32,32,32,60,112,97,116,104,10,32,32,32,32,32,32,32,32,32,115,111,100,105,112,111,100,105,58,110,111,100,101,
116,121,112,101,115,61,34,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,34,10,32,32,32,32,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,111,110,110,101,99,116,111,114,
45,99,117,114,118,97,116,117,114,101,61,34,48,34,10,32,32,32,32,32,32,32,32,32,105,100,61,34,115,116,97,116,115,45,91,35,49,49,57,48,93,34,10,32,32,32,32,32,32,32,32,32,100,61,34,109,32,49,48,50,44,50,
54,56,56,32,118,32,50,32,72,32,56,54,32,99,32,45,49,46,49,53,54,44,45,49,46,57,55,49,32,48,46,50,48,54,44,48,46,51,53,32,48,44,48,32,118,32,48,32,48,32,48,32,45,50,32,45,55,32,104,32,49,54,32,122,32,109,
32,48,44,57,32,72,32,56,54,32,118,32,45,55,32,48,32,48,32,99,32,50,46,55,50,51,44,52,46,54,53,52,32,45,49,46,50,57,52,44,45,50,46,50,50,50,32,48,44,48,32,104,32,49,54,32,118,32,48,32,122,32,109,32,45,
49,56,44,50,32,104,32,50,48,32,118,32,45,50,48,32,72,32,56,52,32,90,34,10,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,102,102,102,102,102,102,59,102,105,108,108,45,111,112,
97,99,105,116,121,58,48,46,57,57,55,50,56,57,57,54,34,32,47,62,10,32,32,32,32,60,47,103,62,10,32,32,60,47,103,62,10,32,32,60,112,97,116,104,10,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,
58,110,111,110,101,59,115,116,114,111,107,101,58,35,102,102,102,102,102,102,59,115,116,114,111,107,101,45,119,105,100,116,104,58,49,46,57,57,57,57,57,57,56,56,59,115,116,114,111,107,101,45,108,105,110,
101,99,97,112,58,114,111,117,110,100,59,115,116,114,111,107,101,45,108,105,110,101,106,111,105,110,58,114,111,117,110,100,59,115,116,114,111,107,101,45,109,105,116,101,114,108,105,109,105,116,58,52,59,
115,116,114,111,107,101,45,100,97,115,104,97,114,114,97,121,58,110,111,110,101,59,115,116,114,111,107,101,45,111,112,97,99,105,116,121,58,49,34,10,32,32,32,32,32,100,61,34,77,32,54,46,53,56,54,52,50,53,
54,44,57,46,53,49,56,48,55,50,51,32,72,32,49,49,46,49,50,53,51,53,32,86,32,49,53,46,55,54,56,49,49,32,104,32,52,46,48,50,52,50,48,50,34,10,32,32,32,32,32,105,100,61,34,112,97,116,104,49,52,51,56,34,10,
32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,111,110,110,101,99,116,111,114,45,99,117,114,118,97,116,117,114,101,61,34,48,34,32,47,62,10,32,32,60,112,97,116,104,10,32,32,32,32,32,115,116,121,108,
101,61,34,102,105,108,108,58,110,111,110,101,59,115,116,114,111,107,101,58,35,102,102,102,102,102,102,59,115,116,114,111,107,101,45,119,105,100,116,104,58,50,59,115,116,114,111,107,101,45,108,105,110,
101,99,97,112,58,114,111,117,110,100,59,115,116,114,111,107,101,45,108,105,110,101,106,111,105,110,58,114,111,117,110,100,59,115,116,114,111,107,101,45,109,105,116,101,114,108,105,109,105,116,58,52,59,
115,116,114,111,107,101,45,100,97,115,104,97,114,114,97,121,58,110,111,110,101,59,115,116,114,111,107,101,45,111,112,97,99,105,116,121,58,49,34,10,32,32,32,32,32,100,61,34,109,32,51,46,51,55,49,48,52,
53,54,44,54,46,50,49,50,57,48,51,51,32,104,32,51,46,50,49,53,51,56,32,118,32,51,46,51,48,53,49,54,57,32,104,32,50,46,56,53,48,55,52,57,56,34,10,32,32,32,32,32,105,100,61,34,112,97,116,104,49,52,51,56,
45,53,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,111,110,110,101,99,116,111,114,45,99,117,114,118,97,116,117,114,101,61,34,48,34,32,47,62,10,32,32,60,116,101,120,116,10,32,32,32,32,32,120,
109,108,58,115,112,97,99,101,61,34,112,114,101,115,101,114,118,101,34,10,32,32,32,32,32,115,116,121,108,101,61,34,102,111,110,116,45,115,116,121,108,101,58,110,111,114,109,97,108,59,102,111,110,116,45,
119,101,105,103,104,116,58,110,111,114,109,97,108,59,102,111,110,116,45,115,105,122,101,58,53,46,51,51,51,51,51,51,52,57,112,120,59,108,105,110,101,45,104,101,105,103,104,116,58,49,46,50,53,59,102,111,
110,116,45,102,97,109,105,108,121,58,115,97,110,115,45,115,101,114,105,102,59,108,101,116,116,101,114,45,115,112,97,99,105,110,103,58,48,112,120,59,119,111,114,100,45,115,112,97,99,105,110,103,58,48,112,
120,59,102,105,108,108,58,35,102,102,102,102,102,102,59,102,105,108,108,45,111,112,97,99,105,116,121,58,49,59,115,116,114,111,107,101,58,110,111,110,101,34,10,32,32,32,32,32,120,61,34,57,46,51,56,57,56,
49,53,51,34,10,32,32,32,32,32,121,61,34,55,46,52,56,55,53,56,57,52,34,10,32,32,32,32,32,105,100,61,34,116,101,120,116,56,51,52,34,62,60,116,115,112,97,110,10,32,32,32,32,32,32,32,115,111,100,105,112,111,
100,105,58,114,111,108,101,61,34,108,105,110,101,34,10,32,32,32,32,32,32,32,105,100,61,34,116,115,112,97,110,56,51,50,34,10,32,32,32,32,32,32,32,120,61,34,57,46,51,56,57,56,49,53,51,34,10,32,32,32,32,
32,32,32,121,61,34,55,46,52,56,55,53,56,57,52,34,10,32,32,32,32,32,32,32,115,116,121,108,101,61,34,45,105,110,107,115,99,97,112,101,45,102,111,110,116,45,115,112,101,99,105,102,105,99,97,116,105,111,110,
58,39,115,97,110,115,45,115,101,114,105,102,32,66,111,108,100,39,59,102,111,110,116,45,102,97,109,105,108,121,58,115,97,110,115,45,115,101,114,105,102,59,102,111,110,116,45,119,101,105,103,104,116,58,
98,111,108,100,59,102,111,110,116,45,115,116,121,108,101,58,110,111,114,109,97,108,59,102,111,110,116,45,115,116,114,101,116,99,104,58,110,111,114,109,97,108,59,102,111,110,116,45,118,97,114,105,97,110,
116,58,110,111,114,109,97,108,34,62,83,72,60,47,116,115,112,97,110,62,60,47,116,101,120,116,62,10,60,47,115,118,103,62,10,0,0};

const char* VASTWaveTableEditorViewHeader::waves_samplehold_svg = (const char*) resource_VASTWaveTableEditorViewHeader_waves_samplehold_svg;
const int VASTWaveTableEditorViewHeader::waves_samplehold_svgSize = 3506;

// JUCER_RESOURCE: arrow_right_corner_svg, 1715, "../../Resources/Icons/arrow_right_corner.svg"
static const unsigned char resource_VASTWaveTableEditorViewHeader_arrow_right_corner_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,
85,84,70,45,56,34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,32,119,105,100,116,104,61,34,50,48,112,120,34,32,104,101,105,103,104,116,61,34,50,48,112,120,34,32,118,
105,101,119,66,111,120,61,34,48,32,48,32,50,48,32,50,48,34,32,118,101,114,115,105,111,110,61,34,49,46,49,34,32,120,109,108,110,115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,
50,48,48,48,47,115,118,103,34,32,120,109,108,110,115,58,120,108,105,110,107,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,120,108,105,110,107,34,62,10,32,32,32,
32,60,33,45,45,32,71,101,110,101,114,97,116,111,114,58,32,83,107,101,116,99,104,32,51,46,56,46,49,32,40,50,57,54,56,55,41,32,45,32,104,116,116,112,58,47,47,119,119,119,46,98,111,104,101,109,105,97,110,
99,111,100,105,110,103,46,99,111,109,47,115,107,101,116,99,104,32,45,45,62,10,32,32,32,32,60,116,105,116,108,101,62,97,114,114,111,119,95,114,105,103,104,116,95,99,111,114,110,101,114,32,91,35,50,55,57,
93,60,47,116,105,116,108,101,62,10,32,32,32,32,60,100,101,115,99,62,67,114,101,97,116,101,100,32,119,105,116,104,32,83,107,101,116,99,104,46,60,47,100,101,115,99,62,10,32,32,32,32,60,100,101,102,115,62,
60,47,100,101,102,115,62,10,32,32,32,32,60,103,32,105,100,61,34,80,97,103,101,45,49,34,32,115,116,114,111,107,101,61,34,110,111,110,101,34,32,115,116,114,111,107,101,45,119,105,100,116,104,61,34,49,34,
32,102,105,108,108,61,34,110,111,110,101,34,32,102,105,108,108,45,114,117,108,101,61,34,101,118,101,110,111,100,100,34,62,10,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,68,114,105,98,98,98,108,101,
45,68,97,114,107,45,80,114,101,118,105,101,119,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,45,49,56,48,46,48,48,48,48,48,48,44,32,45,54,57,49,57,46,48,48,48,48,
48,48,41,34,32,102,105,108,108,61,34,35,70,70,70,70,70,70,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,105,99,111,110,115,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,
97,110,115,108,97,116,101,40,53,54,46,48,48,48,48,48,48,44,32,49,54,48,46,48,48,48,48,48,48,41,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,60,112,97,116,104,32,100,61,34,77,49,51,50,44,54,
55,54,52,32,67,49,51,50,44,54,55,54,51,46,52,52,56,32,49,51,50,46,52,52,56,44,54,55,54,51,32,49,51,51,44,54,55,54,51,32,76,49,51,56,44,54,55,54,51,32,67,49,51,57,46,49,48,53,44,54,55,54,51,32,49,52,48,
44,54,55,54,51,46,56,57,53,32,49,52,48,44,54,55,54,53,32,76,49,52,48,44,54,55,55,48,32,67,49,52,48,44,54,55,55,48,46,53,53,50,32,49,51,57,46,53,53,50,44,54,55,55,49,32,49,51,57,44,54,55,55,49,32,67,49,
51,56,46,52,52,56,44,54,55,55,49,32,49,51,56,44,54,55,55,48,46,53,53,50,32,49,51,56,44,54,55,55,48,32,76,49,51,56,44,54,55,54,55,46,54,50,49,32,67,49,51,56,44,54,55,54,55,46,49,55,54,32,49,51,55,46,52,
54,49,44,54,55,54,54,46,57,53,51,32,49,51,55,46,49,52,54,44,54,55,54,55,46,50,54,56,32,76,49,51,52,46,49,50,49,44,54,55,55,48,46,50,57,51,32,67,49,51,51,46,55,51,49,44,54,55,55,48,46,54,56,51,32,49,51,
51,46,48,57,56,44,54,55,55,48,46,54,56,51,32,49,51,50,46,55,48,55,44,54,55,55,48,46,50,57,51,32,67,49,51,50,46,51,49,55,44,54,55,54,57,46,57,48,50,32,49,51,50,46,51,49,55,44,54,55,54,57,46,50,54,57,32,
49,51,50,46,55,48,55,44,54,55,54,56,46,56,55,57,32,76,49,51,53,46,55,51,50,44,54,55,54,53,46,56,53,52,32,67,49,51,54,46,48,52,55,44,54,55,54,53,46,53,51,57,32,49,51,53,46,56,50,52,44,54,55,54,53,32,49,
51,53,46,51,55,57,44,54,55,54,53,32,76,49,51,51,44,54,55,54,53,32,67,49,51,50,46,52,52,56,44,54,55,54,53,32,49,51,50,44,54,55,54,52,46,53,53,50,32,49,51,50,44,54,55,54,52,32,76,49,51,50,44,54,55,54,52,
32,90,32,77,49,50,54,44,54,55,54,50,32,67,49,50,54,44,54,55,54,49,46,52,52,56,32,49,50,54,46,52,52,56,44,54,55,54,49,32,49,50,55,44,54,55,54,49,32,76,49,52,49,44,54,55,54,49,32,67,49,52,49,46,53,53,50,
44,54,55,54,49,32,49,52,50,44,54,55,54,49,46,52,52,56,32,49,52,50,44,54,55,54,50,32,76,49,52,50,44,54,55,55,54,32,67,49,52,50,44,54,55,55,54,46,53,53,50,32,49,52,49,46,53,53,50,44,54,55,55,55,32,49,52,
49,44,54,55,55,55,32,76,49,51,51,44,54,55,55,55,32,67,49,51,50,46,52,52,56,44,54,55,55,55,32,49,51,50,44,54,55,55,54,46,53,53,50,32,49,51,50,44,54,55,55,54,32,76,49,51,50,44,54,55,55,51,32,67,49,51,50,
44,54,55,55,49,46,56,57,53,32,49,51,49,46,49,48,53,44,54,55,55,49,32,49,51,48,44,54,55,55,49,32,76,49,50,55,44,54,55,55,49,32,67,49,50,54,46,52,52,56,44,54,55,55,49,32,49,50,54,44,54,55,55,48,46,53,53,
50,32,49,50,54,44,54,55,55,48,32,76,49,50,54,44,54,55,54,50,32,90,32,77,49,52,52,44,54,55,54,49,32,67,49,52,52,44,54,55,53,57,46,56,57,53,32,49,52,51,46,49,48,53,44,54,55,53,57,32,49,52,50,44,54,55,53,
57,32,76,49,50,54,44,54,55,53,57,32,67,49,50,52,46,56,57,53,44,54,55,53,57,32,49,50,52,44,54,55,53,57,46,56,57,53,32,49,50,52,44,54,55,54,49,32,76,49,50,52,44,54,55,55,55,32,67,49,50,52,44,54,55,55,56,
46,49,48,53,32,49,50,52,46,56,57,53,44,54,55,55,57,32,49,50,54,44,54,55,55,57,32,76,49,52,50,44,54,55,55,57,32,67,49,52,51,46,49,48,53,44,54,55,55,57,32,49,52,52,44,54,55,55,56,46,49,48,53,32,49,52,52,
44,54,55,55,55,32,76,49,52,52,44,54,55,54,49,32,90,34,32,105,100,61,34,97,114,114,111,119,95,114,105,103,104,116,95,99,111,114,110,101,114,45,91,35,50,55,57,93,34,62,60,47,112,97,116,104,62,10,32,32,32,
32,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,60,47,103,62,10,60,47,115,118,103,62,0,0};

const char* VASTWaveTableEditorViewHeader::arrow_right_corner_svg = (const char*) resource_VASTWaveTableEditorViewHeader_arrow_right_corner_svg;
const int VASTWaveTableEditorViewHeader::arrow_right_corner_svgSize = 1715;

// JUCER_RESOURCE: record_svg, 1191, "../../Resources/Icons/record.svg"
static const unsigned char resource_VASTWaveTableEditorViewHeader_record_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,70,45,
56,34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,32,119,105,100,116,104,61,34,50,48,112,120,34,32,104,101,105,103,104,116,61,34,50,48,112,120,34,32,118,105,101,119,
66,111,120,61,34,48,32,48,32,50,48,32,50,48,34,32,118,101,114,115,105,111,110,61,34,49,46,49,34,32,120,109,108,110,115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,
47,115,118,103,34,32,120,109,108,110,115,58,120,108,105,110,107,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,120,108,105,110,107,34,62,10,32,32,32,32,60,33,45,
45,32,71,101,110,101,114,97,116,111,114,58,32,83,107,101,116,99,104,32,51,46,56,46,49,32,40,50,57,54,56,55,41,32,45,32,104,116,116,112,58,47,47,119,119,119,46,98,111,104,101,109,105,97,110,99,111,100,
105,110,103,46,99,111,109,47,115,107,101,116,99,104,32,45,45,62,10,32,32,32,32,60,116,105,116,108,101,62,114,101,99,111,114,100,32,91,35,57,56,50,93,60,47,116,105,116,108,101,62,10,32,32,32,32,60,100,
101,115,99,62,67,114,101,97,116,101,100,32,119,105,116,104,32,83,107,101,116,99,104,46,60,47,100,101,115,99,62,10,32,32,32,32,60,100,101,102,115,62,60,47,100,101,102,115,62,10,32,32,32,32,60,103,32,105,
100,61,34,80,97,103,101,45,49,34,32,115,116,114,111,107,101,61,34,110,111,110,101,34,32,115,116,114,111,107,101,45,119,105,100,116,104,61,34,49,34,32,102,105,108,108,61,34,110,111,110,101,34,32,102,105,
108,108,45,114,117,108,101,61,34,101,118,101,110,111,100,100,34,62,10,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,68,114,105,98,98,98,108,101,45,68,97,114,107,45,80,114,101,118,105,101,119,34,32,116,
114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,45,51,56,48,46,48,48,48,48,48,48,44,32,45,51,56,51,57,46,48,48,48,48,48,48,41,34,32,102,105,108,108,61,34,35,70,70,70,70,70,70,
34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,105,99,111,110,115,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,53,54,46,48,48,48,48,48,48,44,
32,49,54,48,46,48,48,48,48,48,48,41,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,60,112,97,116,104,32,100,61,34,77,51,51,56,44,51,54,56,57,32,67,51,51,56,44,51,54,57,49,46,50,48,57,32,51,51,
54,46,50,48,57,44,51,54,57,51,32,51,51,52,44,51,54,57,51,32,67,51,51,49,46,55,57,49,44,51,54,57,51,32,51,51,48,44,51,54,57,49,46,50,48,57,32,51,51,48,44,51,54,56,57,32,67,51,51,48,44,51,54,56,54,46,55,
57,49,32,51,51,49,46,55,57,49,44,51,54,56,53,32,51,51,52,44,51,54,56,53,32,67,51,51,54,46,50,48,57,44,51,54,56,53,32,51,51,56,44,51,54,56,54,46,55,57,49,32,51,51,56,44,51,54,56,57,32,77,51,51,52,44,51,
54,57,55,32,67,51,50,57,46,53,56,57,44,51,54,57,55,32,51,50,54,44,51,54,57,51,46,52,49,49,32,51,50,54,44,51,54,56,57,32,67,51,50,54,44,51,54,56,52,46,53,56,57,32,51,50,57,46,53,56,57,44,51,54,56,49,32,
51,51,52,44,51,54,56,49,32,67,51,51,56,46,52,49,49,44,51,54,56,49,32,51,52,50,44,51,54,56,52,46,53,56,57,32,51,52,50,44,51,54,56,57,32,67,51,52,50,44,51,54,57,51,46,52,49,49,32,51,51,56,46,52,49,49,44,
51,54,57,55,32,51,51,52,44,51,54,57,55,32,77,51,51,52,44,51,54,55,57,32,67,51,50,56,46,52,55,55,44,51,54,55,57,32,51,50,52,44,51,54,56,51,46,52,55,55,32,51,50,52,44,51,54,56,57,32,67,51,50,52,44,51,54,
57,52,46,53,50,51,32,51,50,56,46,52,55,55,44,51,54,57,57,32,51,51,52,44,51,54,57,57,32,67,51,51,57,46,53,50,51,44,51,54,57,57,32,51,52,52,44,51,54,57,52,46,53,50,51,32,51,52,52,44,51,54,56,57,32,67,51,
52,52,44,51,54,56,51,46,52,55,55,32,51,51,57,46,53,50,51,44,51,54,55,57,32,51,51,52,44,51,54,55,57,34,32,105,100,61,34,114,101,99,111,114,100,45,91,35,57,56,50,93,34,62,60,47,112,97,116,104,62,10,32,32,
32,32,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,60,47,103,62,10,60,47,115,118,103,62,0,0};

const char* VASTWaveTableEditorViewHeader::record_svg = (const char*) resource_VASTWaveTableEditorViewHeader_record_svg;
const int VASTWaveTableEditorViewHeader::record_svgSize = 1191;


//[EndFile] You can add extra defines here...
//[/EndFile]

