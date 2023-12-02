/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 7.0.7

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
#include "VASTAudioProcessor.h"
#include "VASTAudioProcessorEditor.h"
#include "VASTControls/VASTComboBox.h"
//[/Headers]

#include "VASTPresetEditorComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
VASTPresetEditorComponent::VASTPresetEditorComponent (AudioProcessorEditor *editor, AudioProcessor* processor)
    : myEditor((VASTAudioProcessorEditor*)editor), myProcessor((VASTAudioProcessor*)processor)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    groupComponent2.reset (new juce::GroupComponent ("new group",
                                                     TRANS("SETTINGS")));
    addAndMakeVisible (groupComponent2.get());
    groupComponent2->setTextLabelPosition (juce::Justification::centredLeft);

    groupComponent.reset (new juce::GroupComponent ("new group",
                                                    TRANS("PRESET DATA")));
    addAndMakeVisible (groupComponent.get());
    groupComponent->setTextLabelPosition (juce::Justification::centredLeft);

    c_presetAuthor.reset (new juce::TextEditor ("c_presetAuthor"));
    addAndMakeVisible (c_presetAuthor.get());
    c_presetAuthor->setExplicitFocusOrder (4);
    c_presetAuthor->setMultiLine (false);
    c_presetAuthor->setReturnKeyStartsNewLine (false);
    c_presetAuthor->setReadOnly (false);
    c_presetAuthor->setScrollbarsShown (false);
    c_presetAuthor->setCaretVisible (true);
    c_presetAuthor->setPopupMenuEnabled (true);
    c_presetAuthor->setText (juce::String());

    label4.reset (new juce::Label ("new label",
                                   TRANS("AUTHOR")));
    addAndMakeVisible (label4.get());
    label4->setFont (juce::Font ("Code Pro Demo", 11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label4->setJustificationType (juce::Justification::centredRight);
    label4->setEditable (false, false, false);
    label4->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label4->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label4->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    c_presetName.reset (new juce::TextEditor ("c_presetName"));
    addAndMakeVisible (c_presetName.get());
    c_presetName->setExplicitFocusOrder (1);
    c_presetName->setMultiLine (false);
    c_presetName->setReturnKeyStartsNewLine (false);
    c_presetName->setReadOnly (false);
    c_presetName->setScrollbarsShown (false);
    c_presetName->setCaretVisible (true);
    c_presetName->setPopupMenuEnabled (true);
    c_presetName->setText (juce::String());

    c_presetTag.reset (new juce::TextEditor ("c_presetTag"));
    addAndMakeVisible (c_presetTag.get());
    c_presetTag->setTooltip (TRANS("Add several tags separated by space ( ) or comma (,) or hashtag (#). Tags can be used to filter in the preset browser."));
    c_presetTag->setExplicitFocusOrder (3);
    c_presetTag->setMultiLine (false);
    c_presetTag->setReturnKeyStartsNewLine (false);
    c_presetTag->setReadOnly (false);
    c_presetTag->setScrollbarsShown (false);
    c_presetTag->setCaretVisible (true);
    c_presetTag->setPopupMenuEnabled (true);
    c_presetTag->setText (juce::String());

    label3.reset (new juce::Label ("new label",
                                   TRANS("TAG")));
    addAndMakeVisible (label3.get());
    label3->setFont (juce::Font ("Code Pro Demo", 11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label3->setJustificationType (juce::Justification::centredRight);
    label3->setEditable (false, false, false);
    label3->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label3->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label3->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label2.reset (new juce::Label ("new label",
                                   TRANS("CATEGORY")));
    addAndMakeVisible (label2.get());
    label2->setFont (juce::Font ("Code Pro Demo", 11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label2->setJustificationType (juce::Justification::centredRight);
    label2->setEditable (false, false, false);
    label2->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label2->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label2->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label13.reset (new juce::Label ("new label",
                                    TRANS("PRESET NAME")));
    addAndMakeVisible (label13.get());
    label13->setFont (juce::Font ("Code Pro Demo", 11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label13->setJustificationType (juce::Justification::centredRight);
    label13->setEditable (false, false, false);
    label13->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label13->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label13->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    c_presetFolder.reset (new juce::TextEditor ("c_presetFolder"));
    addAndMakeVisible (c_presetFolder.get());
    c_presetFolder->setMultiLine (false);
    c_presetFolder->setReturnKeyStartsNewLine (false);
    c_presetFolder->setReadOnly (true);
    c_presetFolder->setScrollbarsShown (false);
    c_presetFolder->setCaretVisible (false);
    c_presetFolder->setPopupMenuEnabled (true);
    c_presetFolder->setText (juce::String());

    label7.reset (new juce::Label ("new label",
                                   TRANS("PRESETS")));
    addAndMakeVisible (label7.get());
    label7->setFont (juce::Font ("Code Pro Demo", 11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label7->setJustificationType (juce::Justification::centredRight);
    label7->setEditable (false, false, false);
    label7->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label7->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label7->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    c_folderSelect.reset (new juce::TextButton ("c_folderSelect"));
    addAndMakeVisible (c_folderSelect.get());
    c_folderSelect->setExplicitFocusOrder (8);
    c_folderSelect->setButtonText (TRANS("Select Preset Folder"));
    c_folderSelect->addListener (this);

    c_pluginWidth.reset (new juce::TextEditor ("c_pluginWidth"));
    addAndMakeVisible (c_pluginWidth.get());
    c_pluginWidth->setMultiLine (false);
    c_pluginWidth->setReturnKeyStartsNewLine (false);
    c_pluginWidth->setReadOnly (true);
    c_pluginWidth->setScrollbarsShown (false);
    c_pluginWidth->setCaretVisible (false);
    c_pluginWidth->setPopupMenuEnabled (true);
    c_pluginWidth->setText (juce::String());

    label8.reset (new juce::Label ("new label",
                                   TRANS("WIDTH")));
    addAndMakeVisible (label8.get());
    label8->setFont (juce::Font ("Code Pro Demo", 11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label8->setJustificationType (juce::Justification::centredRight);
    label8->setEditable (false, false, false);
    label8->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label8->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label8->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    c_pluginHeight.reset (new juce::TextEditor ("c_pluginHeight"));
    addAndMakeVisible (c_pluginHeight.get());
    c_pluginHeight->setMultiLine (false);
    c_pluginHeight->setReturnKeyStartsNewLine (false);
    c_pluginHeight->setReadOnly (true);
    c_pluginHeight->setScrollbarsShown (false);
    c_pluginHeight->setCaretVisible (false);
    c_pluginHeight->setPopupMenuEnabled (true);
    c_pluginHeight->setText (juce::String());

    label9.reset (new juce::Label ("new label",
                                   TRANS("HEIGHT")));
    addAndMakeVisible (label9.get());
    label9->setFont (juce::Font ("Code Pro Demo", 11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label9->setJustificationType (juce::Justification::centredRight);
    label9->setEditable (false, false, false);
    label9->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label9->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label9->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    c_currentSize.reset (new juce::TextButton ("c_currentSize"));
    addAndMakeVisible (c_currentSize.get());
    c_currentSize->setExplicitFocusOrder (11);
    c_currentSize->setButtonText (TRANS("Select Size"));
    c_currentSize->addListener (this);

    c_defaultSize.reset (new juce::TextButton ("c_defaultSize"));
    addAndMakeVisible (c_defaultSize.get());
    c_defaultSize->setExplicitFocusOrder (12);
    c_defaultSize->setButtonText (TRANS("Use Default Size"));
    c_defaultSize->addListener (this);

    c_wavetableFolder.reset (new juce::TextEditor ("c_wavetableFolder"));
    addAndMakeVisible (c_wavetableFolder.get());
    c_wavetableFolder->setMultiLine (false);
    c_wavetableFolder->setReturnKeyStartsNewLine (false);
    c_wavetableFolder->setReadOnly (true);
    c_wavetableFolder->setScrollbarsShown (false);
    c_wavetableFolder->setCaretVisible (false);
    c_wavetableFolder->setPopupMenuEnabled (true);
    c_wavetableFolder->setText (juce::String());

    label5.reset (new juce::Label ("new label",
                                   TRANS("WAVETABLES")));
    addAndMakeVisible (label5.get());
    label5->setFont (juce::Font ("Code Pro Demo", 11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label5->setJustificationType (juce::Justification::centredRight);
    label5->setEditable (false, false, false);
    label5->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label5->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label5->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    c_folderWTSelect.reset (new juce::TextButton ("c_folderWTSelect"));
    addAndMakeVisible (c_folderWTSelect.get());
    c_folderWTSelect->setExplicitFocusOrder (9);
    c_folderWTSelect->setButtonText (TRANS("Select WT Folder"));
    c_folderWTSelect->addListener (this);

    c_wavFolder.reset (new juce::TextEditor ("c_wavFolder"));
    addAndMakeVisible (c_wavFolder.get());
    c_wavFolder->setMultiLine (false);
    c_wavFolder->setReturnKeyStartsNewLine (false);
    c_wavFolder->setReadOnly (true);
    c_wavFolder->setScrollbarsShown (false);
    c_wavFolder->setCaretVisible (false);
    c_wavFolder->setPopupMenuEnabled (true);
    c_wavFolder->setText (juce::String());

    label6.reset (new juce::Label ("new label",
                                   TRANS("SAMPLES")));
    addAndMakeVisible (label6.get());
    label6->setFont (juce::Font ("Code Pro Demo", 11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label6->setJustificationType (juce::Justification::centredRight);
    label6->setEditable (false, false, false);
    label6->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label6->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label6->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    c_folderWavSelect.reset (new juce::TextButton ("c_folderWavSelect"));
    addAndMakeVisible (c_folderWavSelect.get());
    c_folderWavSelect->setExplicitFocusOrder (10);
    c_folderWavSelect->setButtonText (TRANS("Select WAV Folder"));
    c_folderWavSelect->addListener (this);

    c_presetComments.reset (new juce::TextEditor ("c_presetComments"));
    addAndMakeVisible (c_presetComments.get());
    c_presetComments->setExplicitFocusOrder (5);
    c_presetComments->setMultiLine (false);
    c_presetComments->setReturnKeyStartsNewLine (false);
    c_presetComments->setReadOnly (false);
    c_presetComments->setScrollbarsShown (false);
    c_presetComments->setCaretVisible (true);
    c_presetComments->setPopupMenuEnabled (true);
    c_presetComments->setText (juce::String());

    label10.reset (new juce::Label ("new label",
                                    TRANS("COMMENTS")));
    addAndMakeVisible (label10.get());
    label10->setFont (juce::Font ("Code Pro Demo", 11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label10->setJustificationType (juce::Justification::centredRight);
    label10->setEditable (false, false, false);
    label10->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label10->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label10->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    c_presetCategoryCombo.reset (new VASTComboBox ("c_presetCategoryCombo"));
    addAndMakeVisible (c_presetCategoryCombo.get());
    c_presetCategoryCombo->setExplicitFocusOrder (2);
    c_presetCategoryCombo->setEditableText (false);
    c_presetCategoryCombo->setJustificationType (juce::Justification::centredLeft);
    c_presetCategoryCombo->setTextWhenNothingSelected (juce::String());
    c_presetCategoryCombo->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    c_presetCategoryCombo->addItem (TRANS("AR Arpeggio "), 1);
    c_presetCategoryCombo->addItem (TRANS("AT Atmosphere"), 2);
    c_presetCategoryCombo->addItem (TRANS("BA Bass"), 3);
    c_presetCategoryCombo->addItem (TRANS("BR Brass "), 4);
    c_presetCategoryCombo->addItem (TRANS("BL Bell"), 5);
    c_presetCategoryCombo->addItem (TRANS("CH Chord"), 6);
    c_presetCategoryCombo->addItem (TRANS("DK Drum kit"), 7);
    c_presetCategoryCombo->addItem (TRANS("DR Drum"), 8);
    c_presetCategoryCombo->addItem (TRANS("DL Drum loop"), 9);
    c_presetCategoryCombo->addItem (TRANS("FX Effect"), 10);
    c_presetCategoryCombo->addItem (TRANS("GT Guitar"), 11);
    c_presetCategoryCombo->addItem (TRANS("IN Instrument"), 12);
    c_presetCategoryCombo->addItem (TRANS("KB Keyboard"), 13);
    c_presetCategoryCombo->addItem (TRANS("LD Lead"), 14);
    c_presetCategoryCombo->addItem (TRANS("MA Mallet"), 15);
    c_presetCategoryCombo->addItem (TRANS("OR Organ"), 16);
    c_presetCategoryCombo->addItem (TRANS("OC Orchestral"), 17);
    c_presetCategoryCombo->addItem (TRANS("PD Pad"), 18);
    c_presetCategoryCombo->addItem (TRANS("PN Piano"), 19);
    c_presetCategoryCombo->addItem (TRANS("PL Plucked"), 20);
    c_presetCategoryCombo->addItem (TRANS("RI Riser"), 21);
    c_presetCategoryCombo->addItem (TRANS("RD Reed"), 22);
    c_presetCategoryCombo->addItem (TRANS("ST String"), 23);
    c_presetCategoryCombo->addItem (TRANS("SY Synth"), 24);
    c_presetCategoryCombo->addItem (TRANS("SQ Sequence / Split"), 25);
    c_presetCategoryCombo->addItem (TRANS("TG Trancegate"), 26);
    c_presetCategoryCombo->addItem (TRANS("VC Vocal / Voice"), 27);
    c_presetCategoryCombo->addItem (TRANS("WW Woodwind"), 28);
    c_presetCategoryCombo->addListener (this);

    label11.reset (new juce::Label ("new label",
                                    TRANS("SKIN")));
    addAndMakeVisible (label11.get());
    label11->setFont (juce::Font ("Code Pro Demo", 11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label11->setJustificationType (juce::Justification::centredRight);
    label11->setEditable (false, false, false);
    label11->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label11->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label11->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    c_uiThemeCombo.reset (new VASTComboBox ("c_uiThemeCombo"));
    addAndMakeVisible (c_uiThemeCombo.get());
    c_uiThemeCombo->setTooltip (TRANS("Ui colour and display theme"));
    c_uiThemeCombo->setExplicitFocusOrder (13);
    c_uiThemeCombo->setEditableText (false);
    c_uiThemeCombo->setJustificationType (juce::Justification::centredLeft);
    c_uiThemeCombo->setTextWhenNothingSelected (juce::String());
    c_uiThemeCombo->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    c_uiThemeCombo->addItem (TRANS("Classic"), 1);
    c_uiThemeCombo->addItem (TRANS("IceCubes"), 2);
    c_uiThemeCombo->addItem (TRANS("BloodTech"), 3);
    c_uiThemeCombo->addItem (TRANS("Dark"), 4);
    c_uiThemeCombo->addListener (this);

    label12.reset (new juce::Label ("new label",
                                    TRANS("WAVET. MODE")));
    addAndMakeVisible (label12.get());
    label12->setFont (juce::Font ("Code Pro Demo", 11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label12->setJustificationType (juce::Justification::centredRight);
    label12->setEditable (false, false, false);
    label12->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label12->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label12->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    c_WTmode.reset (new VASTComboBox ("c_WTmode"));
    addAndMakeVisible (c_WTmode.get());
    c_WTmode->setTooltip (TRANS("Wavetable frequency calculation mode - Sharp (ideal), Soft (butterworth), Dull (linear)"));
    c_WTmode->setExplicitFocusOrder (16);
    c_WTmode->setEditableText (false);
    c_WTmode->setJustificationType (juce::Justification::centredLeft);
    c_WTmode->setTextWhenNothingSelected (juce::String());
    c_WTmode->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    c_WTmode->addItem (TRANS("Sharp"), 1);
    c_WTmode->addItem (TRANS("Soft"), 2);
    c_WTmode->addItem (TRANS("Dull"), 3);
    c_WTmode->addListener (this);

    c_iconMaximizeEditor.reset (new VASTDrawableButton ("c_iconMaximize", arrow_right_corner_svg, arrow_right_corner_svgSize, "Maximize area"));
    addAndMakeVisible (c_iconMaximizeEditor.get());
    c_iconMaximizeEditor->setName ("c_iconMaximizeEditor");

    label14.reset (new juce::Label ("new label",
                                    TRANS("DISABLE GFX")));
    addAndMakeVisible (label14.get());
    label14->setFont (juce::Font ("Code Pro Demo", 11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label14->setJustificationType (juce::Justification::centredRight);
    label14->setEditable (false, false, false);
    label14->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label14->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label14->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    c_disableGFX.reset (new juce::ToggleButton ("c_disableGFX"));
    addAndMakeVisible (c_disableGFX.get());
    c_disableGFX->setTooltip (TRANS("Disable openGL graphics to improve performance on older systems (oscilloscope on top)"));
    c_disableGFX->setExplicitFocusOrder (15);
    c_disableGFX->setButtonText (juce::String());
    c_disableGFX->addListener (this);

    label15.reset (new juce::Label ("new label",
                                    TRANS("MPE MODE")));
    addAndMakeVisible (label15.get());
    label15->setFont (juce::Font ("Code Pro Demo", 11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label15->setJustificationType (juce::Justification::centredRight);
    label15->setEditable (false, false, false);
    label15->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label15->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label15->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label16.reset (new juce::Label ("new label",
                                    TRANS("FONT SIZE")));
    addAndMakeVisible (label16.get());
    label16->setFont (juce::Font ("Code Pro Demo", 11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label16->setJustificationType (juce::Justification::centredRight);
    label16->setEditable (false, false, false);
    label16->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label16->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label16->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    c_uiFontSize.reset (new VASTComboBox ("c_uiFontSize"));
    addAndMakeVisible (c_uiFontSize.get());
    c_uiFontSize->setTooltip (TRANS("Font size"));
    c_uiFontSize->setExplicitFocusOrder (14);
    c_uiFontSize->setEditableText (false);
    c_uiFontSize->setJustificationType (juce::Justification::centredLeft);
    c_uiFontSize->setTextWhenNothingSelected (juce::String());
    c_uiFontSize->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    c_uiFontSize->addItem (TRANS("Normal"), 1);
    c_uiFontSize->addItem (TRANS("Larger"), 2);
    c_uiFontSize->addItem (TRANS("Smaller"), 3);
    c_uiFontSize->addListener (this);

    label17.reset (new juce::Label ("new label",
                                    TRANS("MPE PRESET")));
    addAndMakeVisible (label17.get());
    label17->setFont (juce::Font ("Code Pro Demo", 11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label17->setJustificationType (juce::Justification::centredRight);
    label17->setEditable (false, false, false);
    label17->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label17->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label17->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    c_presetMPE.reset (new juce::ToggleButton ("c_presetMPE"));
    addAndMakeVisible (c_presetMPE.get());
    c_presetMPE->setTooltip (TRANS("Enable MIDI Polyphonic Expression (MPE) mode for supported controllers for preset"));
    c_presetMPE->setExplicitFocusOrder (6);
    c_presetMPE->setButtonText (juce::String());
    c_presetMPE->addListener (this);

    c_MPEmode.reset (new VASTComboBox ("c_MPEmode"));
    addAndMakeVisible (c_MPEmode.get());
    c_MPEmode->setTooltip (TRANS("Enable MIDI Polyphonic Expression (MPE) mode for supported controllers always per default"));
    c_MPEmode->setExplicitFocusOrder (17);
    c_MPEmode->setEditableText (false);
    c_MPEmode->setJustificationType (juce::Justification::centredLeft);
    c_MPEmode->setTextWhenNothingSelected (juce::String());
    c_MPEmode->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    c_MPEmode->addItem (TRANS("From preset"), 1);
    c_MPEmode->addItem (TRANS("Always on"), 2);
    c_MPEmode->addItem (TRANS("Always off"), 3);
    c_MPEmode->addListener (this);

    c_presetBendRange.reset (new VASTSlider ("c_presetBendRange"));
    addAndMakeVisible (c_presetBendRange.get());
    c_presetBendRange->setTooltip (TRANS("MPE pitch bend sensitivity"));
    c_presetBendRange->setExplicitFocusOrder (7);
    c_presetBendRange->setRange (1, 96, 1);
    c_presetBendRange->setSliderStyle (juce::Slider::IncDecButtons);
    c_presetBendRange->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 28, 20);
    c_presetBendRange->addListener (this);

    label18.reset (new juce::Label ("new label",
                                    TRANS("MPE BEND RANGE")));
    addAndMakeVisible (label18.get());
    label18->setFont (juce::Font ("Code Pro Demo", 11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label18->setJustificationType (juce::Justification::centredRight);
    label18->setEditable (false, false, false);
    label18->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label18->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label18->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    c_tuning.reset (new juce::TextEditor ("c_tuning"));
    addAndMakeVisible (c_tuning.get());
    c_tuning->setTooltip (TRANS("AnaMark Tuning file (.TUN) for custom tuning"));
    c_tuning->setMultiLine (false);
    c_tuning->setReturnKeyStartsNewLine (false);
    c_tuning->setReadOnly (true);
    c_tuning->setScrollbarsShown (false);
    c_tuning->setCaretVisible (false);
    c_tuning->setPopupMenuEnabled (true);
    c_tuning->setText (juce::String());

    label19.reset (new juce::Label ("new label",
                                    TRANS("TUNING")));
    addAndMakeVisible (label19.get());
    label19->setFont (juce::Font ("Code Pro Demo", 11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label19->setJustificationType (juce::Justification::centredRight);
    label19->setEditable (false, false, false);
    label19->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label19->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label19->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    c_tuningSelect.reset (new juce::TextButton ("c_tuningSelect"));
    addAndMakeVisible (c_tuningSelect.get());
    c_tuningSelect->setExplicitFocusOrder (18);
    c_tuningSelect->setButtonText (TRANS("Select .TUN File"));
    c_tuningSelect->addListener (this);

    c_tuningRemove.reset (new juce::TextButton ("c_tuningRemove"));
    addAndMakeVisible (c_tuningRemove.get());
    c_tuningRemove->setExplicitFocusOrder (18);
    c_tuningRemove->setButtonText (TRANS("Use Default Tuning"));
    c_tuningRemove->addListener (this);

    c_permaLink.reset (new VASTComboBox ("c_permaLink"));
    addAndMakeVisible (c_permaLink.get());
    c_permaLink->setTooltip (TRANS("Permanently link Modulation Wheel to Custom Modulators"));
    c_permaLink->setExplicitFocusOrder (16);
    c_permaLink->setEditableText (false);
    c_permaLink->setJustificationType (juce::Justification::centredLeft);
    c_permaLink->setTextWhenNothingSelected (juce::String());
    c_permaLink->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    c_permaLink->addItem (TRANS("---"), 1);
    c_permaLink->addItem (TRANS("CUSTOM MODULATOR 1"), 2);
    c_permaLink->addItem (TRANS("CUSTOM MODULATOR 2"), 3);
    c_permaLink->addItem (TRANS("CUSTOM MODULATOR 3"), 4);
    c_permaLink->addItem (TRANS("CUSTOM MODULATOR 4"), 5);
    c_permaLink->addListener (this);

    label20.reset (new juce::Label ("new label",
                                    TRANS("MOD.WHEEL PERM.\n")));
    addAndMakeVisible (label20.get());
    label20->setFont (juce::Font ("Code Pro Demo", 11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label20->setJustificationType (juce::Justification::centredRight);
    label20->setEditable (false, false, false);
    label20->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label20->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label20->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));


    //[UserPreSize]
	setOpaque(true);

	c_presetTag->addListener(this);
	c_presetAuthor->addListener(this);
	c_presetComments->addListener(this);
	c_presetName->addListener(this);
	c_presetFolder->addListener(this);
	c_wavetableFolder->addListener(this);
	c_wavFolder->addListener(this);
	c_pluginWidth->addListener(this);
	c_pluginHeight->addListener(this);

    c_presetBendRange->setIncDecButtonsMode(Slider::incDecButtonsDraggable_Vertical);
    c_presetBendRange->setAudioProcessor(*myProcessor);
	c_iconMaximizeEditor->addListener(this);

	c_uiThemeCombo->setSelectedItemIndex(myProcessor->m_activeLookAndFeel, NotificationType::dontSendNotification); //dont send
	c_WTmode->setSelectedItemIndex(myProcessor->getWTmode(), NotificationType::dontSendNotification); //dont send
	c_uiFontSize->setSelectedItemIndex(myProcessor->m_uiFontSize, NotificationType::dontSendNotification); //dont send

#ifdef VASTLOG
    logEditorComponent = std::make_unique<CodeEditorComponent>(myProcessor->logCodeDocument, nullptr);
    addAndMakeVisible(*logEditorComponent, -1);
    logEditorComponent->setAlwaysOnTop(true);
    logEditorComponent->setOpaque(true);
#endif

    return; //dont call setSize
    //[/UserPreSize]

    setSize (800, 575);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

VASTPresetEditorComponent::~VASTPresetEditorComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    VDBG("Destructing VASTPresetEditorComponent");
	this->setLookAndFeel(nullptr);
    //[/Destructor_pre]

    groupComponent2 = nullptr;
    groupComponent = nullptr;
    c_presetAuthor = nullptr;
    label4 = nullptr;
    c_presetName = nullptr;
    c_presetTag = nullptr;
    label3 = nullptr;
    label2 = nullptr;
    label13 = nullptr;
    c_presetFolder = nullptr;
    label7 = nullptr;
    c_folderSelect = nullptr;
    c_pluginWidth = nullptr;
    label8 = nullptr;
    c_pluginHeight = nullptr;
    label9 = nullptr;
    c_currentSize = nullptr;
    c_defaultSize = nullptr;
    c_wavetableFolder = nullptr;
    label5 = nullptr;
    c_folderWTSelect = nullptr;
    c_wavFolder = nullptr;
    label6 = nullptr;
    c_folderWavSelect = nullptr;
    c_presetComments = nullptr;
    label10 = nullptr;
    c_presetCategoryCombo = nullptr;
    label11 = nullptr;
    c_uiThemeCombo = nullptr;
    label12 = nullptr;
    c_WTmode = nullptr;
    c_iconMaximizeEditor = nullptr;
    label14 = nullptr;
    c_disableGFX = nullptr;
    label15 = nullptr;
    label16 = nullptr;
    c_uiFontSize = nullptr;
    label17 = nullptr;
    c_presetMPE = nullptr;
    c_MPEmode = nullptr;
    c_presetBendRange = nullptr;
    label18 = nullptr;
    c_tuning = nullptr;
    label19 = nullptr;
    c_tuningSelect = nullptr;
    c_tuningRemove = nullptr;
    c_permaLink = nullptr;
    label20 = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void VASTPresetEditorComponent::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff323e44));

    {
        int x = 0, y = 0, width = getWidth() - 0, height = getHeight() - 0;
        juce::Colour fillColour1 = juce::Colour (0x4f616d7e), fillColour2 = juce::Colour (0xbd03070a);
        //[UserPaintCustomArguments] Customize the painting arguments here..
    		fillColour1 = myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colPresetEditorBackgroundGradientFrom);
    		fillColour2 = myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colPresetEditorBackgroundGradientTo);
        //[/UserPaintCustomArguments]
        g.setGradientFill (juce::ColourGradient (fillColour1,
                                             static_cast<float> (proportionOfWidth (-0.0400f)) - 0.0f + x,
                                             static_cast<float> (proportionOfHeight (-0.0154f)) - 0.0f + y,
                                             fillColour2,
                                             static_cast<float> (proportionOfWidth (1.0200f)) - 0.0f + x,
                                             static_cast<float> (proportionOfHeight (1.0200f)) - 0.0f + y,
                                             false));
        g.fillRect (x, y, width, height);
    }

    //[UserPaint] Add your own custom painting code here..

    //[/UserPaint]
}

void VASTPresetEditorComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
#ifdef VASTLOG
    logEditorComponent->setBounds(proportionOfWidth(0.0f), proportionOfHeight(0.0f), proportionOfWidth(1.0f), proportionOfHeight(1.0f));
#endif
    //[/UserPreResize]

    groupComponent2->setBounds (proportionOfWidth (0.0100f), proportionOfHeight (0.3791f), proportionOfWidth (0.9800f), proportionOfHeight (0.6087f));
    groupComponent->setBounds (proportionOfWidth (0.0100f), proportionOfHeight (0.0139f), proportionOfWidth (0.9800f), proportionOfHeight (0.3478f));
    c_presetAuthor->setBounds (proportionOfWidth (0.1688f), proportionOfHeight (0.1861f), proportionOfWidth (0.3013f), proportionOfHeight (0.0244f));
    label4->setBounds (proportionOfWidth (0.0300f), proportionOfHeight (0.1896f), proportionOfWidth (0.1288f), proportionOfHeight (0.0191f));
    c_presetName->setBounds (proportionOfWidth (0.1688f), proportionOfHeight (0.0626f), proportionOfWidth (0.3038f), proportionOfHeight (0.0244f));
    c_presetTag->setBounds (proportionOfWidth (0.1688f), proportionOfHeight (0.1478f), proportionOfWidth (0.7813f), proportionOfHeight (0.0244f));
    label3->setBounds (proportionOfWidth (0.0300f), proportionOfHeight (0.1513f), proportionOfWidth (0.1288f), proportionOfHeight (0.0191f));
    label2->setBounds (proportionOfWidth (0.0300f), proportionOfHeight (0.1096f), proportionOfWidth (0.1288f), proportionOfHeight (0.0191f));
    label13->setBounds (proportionOfWidth (0.0300f), proportionOfHeight (0.0661f), proportionOfWidth (0.1288f), proportionOfHeight (0.0191f));
    c_presetFolder->setBounds (proportionOfWidth (0.1688f), proportionOfHeight (0.4261f), proportionOfWidth (0.3912f), proportionOfHeight (0.0244f));
    label7->setBounds (proportionOfWidth (0.0300f), proportionOfHeight (0.4296f), proportionOfWidth (0.1288f), proportionOfHeight (0.0191f));
    c_folderSelect->setBounds (proportionOfWidth (0.6088f), proportionOfHeight (0.4261f), proportionOfWidth (0.1600f), proportionOfHeight (0.0244f));
    c_pluginWidth->setBounds (proportionOfWidth (0.1688f), proportionOfHeight (0.5565f), proportionOfWidth (0.1313f), proportionOfHeight (0.0244f));
    label8->setBounds (proportionOfWidth (0.0300f), proportionOfHeight (0.5583f), proportionOfWidth (0.1288f), proportionOfHeight (0.0191f));
    c_pluginHeight->setBounds (proportionOfWidth (0.4300f), proportionOfHeight (0.5565f), proportionOfWidth (0.1313f), proportionOfHeight (0.0244f));
    label9->setBounds (proportionOfWidth (0.3000f), proportionOfHeight (0.5583f), proportionOfWidth (0.1288f), proportionOfHeight (0.0191f));
    c_currentSize->setBounds (proportionOfWidth (0.6088f), proportionOfHeight (0.5583f), proportionOfWidth (0.1600f), proportionOfHeight (0.0244f));
    c_defaultSize->setBounds (proportionOfWidth (0.7900f), proportionOfHeight (0.5583f), proportionOfWidth (0.1600f), proportionOfHeight (0.0244f));
    c_wavetableFolder->setBounds (proportionOfWidth (0.1688f), proportionOfHeight (0.4644f), proportionOfWidth (0.3912f), proportionOfHeight (0.0244f));
    label5->setBounds (proportionOfWidth (0.0300f), proportionOfHeight (0.4678f), proportionOfWidth (0.1288f), proportionOfHeight (0.0191f));
    c_folderWTSelect->setBounds (proportionOfWidth (0.6088f), proportionOfHeight (0.4644f), proportionOfWidth (0.1600f), proportionOfHeight (0.0244f));
    c_wavFolder->setBounds (proportionOfWidth (0.1688f), proportionOfHeight (0.5026f), proportionOfWidth (0.3912f), proportionOfHeight (0.0244f));
    label6->setBounds (proportionOfWidth (0.0300f), proportionOfHeight (0.5061f), proportionOfWidth (0.1288f), proportionOfHeight (0.0191f));
    c_folderWavSelect->setBounds (proportionOfWidth (0.6088f), proportionOfHeight (0.5026f), proportionOfWidth (0.1600f), proportionOfHeight (0.0244f));
    c_presetComments->setBounds (proportionOfWidth (0.1688f), proportionOfHeight (0.2243f), proportionOfWidth (0.7813f), proportionOfHeight (0.0244f));
    label10->setBounds (proportionOfWidth (0.0300f), proportionOfHeight (0.2278f), proportionOfWidth (0.1288f), proportionOfHeight (0.0191f));
    c_presetCategoryCombo->setBounds (proportionOfWidth (0.1688f), proportionOfHeight (0.1043f), proportionOfWidth (0.1313f), proportionOfHeight (0.0261f));
    label11->setBounds (proportionOfWidth (0.0300f), proportionOfHeight (0.5965f), proportionOfWidth (0.1288f), proportionOfHeight (0.0191f));
    c_uiThemeCombo->setBounds (proportionOfWidth (0.1688f), proportionOfHeight (0.5948f), proportionOfWidth (0.2300f), proportionOfHeight (0.0261f));
    label12->setBounds (proportionOfWidth (0.0300f), proportionOfHeight (0.7496f), proportionOfWidth (0.1288f), proportionOfHeight (0.0191f));
    c_WTmode->setBounds (proportionOfWidth (0.1688f), proportionOfHeight (0.7461f), proportionOfWidth (0.1313f), proportionOfHeight (0.0261f));
    c_iconMaximizeEditor->setBounds (getWidth() - 1 - proportionOfWidth (0.0150f), 1, proportionOfWidth (0.0150f), proportionOfHeight (0.0209f));
    label14->setBounds (proportionOfWidth (0.0300f), proportionOfHeight (0.6730f), proportionOfWidth (0.1288f), proportionOfHeight (0.0191f));
    c_disableGFX->setBounds (proportionOfWidth (0.1650f), proportionOfHeight (0.6661f), proportionOfWidth (0.0213f), proportionOfHeight (0.0296f));
    label15->setBounds (proportionOfWidth (0.0300f), proportionOfHeight (0.7878f), proportionOfWidth (0.1288f), proportionOfHeight (0.0191f));
    label16->setBounds (proportionOfWidth (0.0300f), proportionOfHeight (0.6348f), proportionOfWidth (0.1288f), proportionOfHeight (0.0191f));
    c_uiFontSize->setBounds (proportionOfWidth (0.1688f), proportionOfHeight (0.6330f), proportionOfWidth (0.1313f), proportionOfHeight (0.0261f));
    label17->setBounds (proportionOfWidth (0.0300f), proportionOfHeight (0.2748f), proportionOfWidth (0.1288f), proportionOfHeight (0.0191f));
    c_presetMPE->setBounds (proportionOfWidth (0.1650f), proportionOfHeight (0.2696f), proportionOfWidth (0.0213f), proportionOfHeight (0.0296f));
    c_MPEmode->setBounds (proportionOfWidth (0.1688f), proportionOfHeight (0.7826f), proportionOfWidth (0.1313f), proportionOfHeight (0.0261f));
    c_presetBendRange->setBounds (proportionOfWidth (0.1688f), proportionOfHeight (0.3078f), proportionOfWidth (0.0750f), proportionOfHeight (0.0313f));
    label18->setBounds (proportionOfWidth (0.0300f), proportionOfHeight (0.3130f), proportionOfWidth (0.1288f), proportionOfHeight (0.0191f));
    c_tuning->setBounds (proportionOfWidth (0.1688f), proportionOfHeight (0.8522f), proportionOfWidth (0.3912f), proportionOfHeight (0.0244f));
    label19->setBounds (proportionOfWidth (0.0300f), proportionOfHeight (0.8539f), proportionOfWidth (0.1288f), proportionOfHeight (0.0191f));
    c_tuningSelect->setBounds (proportionOfWidth (0.6088f), proportionOfHeight (0.8522f), proportionOfWidth (0.1600f), proportionOfHeight (0.0244f));
    c_tuningRemove->setBounds (proportionOfWidth (0.7900f), proportionOfHeight (0.8522f), proportionOfWidth (0.1600f), proportionOfHeight (0.0244f));
    c_permaLink->setBounds (proportionOfWidth (0.1688f), proportionOfHeight (0.8904f), proportionOfWidth (0.2300f), proportionOfHeight (0.0261f));
    label20->setBounds (proportionOfWidth (0.0300f), proportionOfHeight (0.8956f), proportionOfWidth (0.1288f), proportionOfHeight (0.0191f));
    //[UserResized] Add your own custom resize handling here..
	updateAll();
    //[/UserResized]
}

void VASTPresetEditorComponent::buttonClicked (juce::Button* buttonThatWasClicked)
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

    if (buttonThatWasClicked == c_folderSelect.get())
    {
        //[UserButtonCode_c_folderSelect] -- add your button handler code here..

        /*
        FileChooser fc(TRANS("Choose root folder for user presets. All subfolders will be scanned for .vvp files."),
            File(myProcessor->m_UserPresetRootFolder),
            "*.",
            true);
        */

        myChooser = std::make_unique<FileChooser>(TRANS("Choose root folder for user presets. All subfolders will be scanned for .vvp files."), File(myProcessor->m_UserPresetRootFolder), "*.");
        myChooser->launchAsync(FileBrowserComponent::openMode | FileBrowserComponent::canSelectDirectories,
            [this](const FileChooser& fileChooser) {

            //if (fc.browseForDirectory())
            File chosenDirectory(fileChooser.getResult());

            if (chosenDirectory.isDirectory() == true) {

                AlertWindow::showMessageBoxAsync(AlertWindow::InfoIcon,
                    "",
                    TRANS("Preset root folder changed and user presets are reloaded."), String(), c_folderSelect.get());

                myProcessor->m_UserPresetRootFolder = chosenDirectory.getFullPathName();
                myProcessor->writeSettingsToFile(); //not async
                myProcessor->m_presetData.reloadPresetArray();
            }

            c_presetFolder->setText(myProcessor->m_UserPresetRootFolder, false);
        });
        //[/UserButtonCode_c_folderSelect]
    }
    else if (buttonThatWasClicked == c_currentSize.get())
    {
        //[UserButtonCode_c_currentSize] -- add your button handler code here..
		myEditor->vaporizerComponent->selectSizeMenu();
		/*
		myProcessor->m_iUserTargetPluginWidth = myEditor->getWidth();
		myProcessor->m_iUserTargetPluginHeight = myEditor->getHeight();
		c_pluginWidth->setText(String(myProcessor->m_iUserTargetPluginWidth));
		c_pluginHeight->setText(String(myProcessor->m_iUserTargetPluginHeight));
		myProcessor->writeSettingsToFile();
		*/
        //[/UserButtonCode_c_currentSize]
    }
    else if (buttonThatWasClicked == c_defaultSize.get())
    {
        //[UserButtonCode_c_defaultSize] -- add your button handler code here..
		myProcessor->m_iUserTargetPluginWidth = myProcessor->m_iDefaultPluginWidth; //default size from projucer
		myProcessor->m_iUserTargetPluginHeight = myProcessor->m_iDefaultPluginHeight; //default size from projucer
		c_pluginWidth->setText(String(myProcessor->m_iUserTargetPluginWidth));
		c_pluginHeight->setText(String(myProcessor->m_iUserTargetPluginHeight));
		myProcessor->writeSettingsToFile();

		myEditor->setSize(myProcessor->m_iUserTargetPluginWidth, myProcessor->m_iUserTargetPluginHeight);
        //[/UserButtonCode_c_defaultSize]
    }
    else if (buttonThatWasClicked == c_folderWTSelect.get())
    {
        //[UserButtonCode_c_folderWTSelect] -- add your button handler code here..
        /*
		FileChooser fc(TRANS("Choose root folder for wavetables. All subfolders will be scanned for .wav and .aif files."),
			File(myProcessor->m_UserWavetableRootFolder),
			"*.",
			true);
		if (fc.browseForDirectory())
            */

        myChooser = std::make_unique<FileChooser>(TRANS("Choose root folder for wavetables. All subfolders will be scanned for .wav and .aif files."), File(myProcessor->m_UserWavetableRootFolder), "*.");
        myChooser->launchAsync(FileBrowserComponent::openMode | FileBrowserComponent::canSelectDirectories,
            [this](const FileChooser& fileChooser) {
            File chosenDirectory(fileChooser.getResult());

            if (chosenDirectory.isDirectory() == true) {

                myProcessor->m_UserWavetableRootFolder = chosenDirectory.getFullPathName();
                myProcessor->writeSettingsToFile();
            }

            c_wavetableFolder->setText(myProcessor->m_UserWavetableRootFolder, false);
        });
        //[/UserButtonCode_c_folderWTSelect]
    }
    else if (buttonThatWasClicked == c_folderWavSelect.get())
    {
        //[UserButtonCode_c_folderWavSelect] -- add your button handler code here..
        /*
		FileChooser fc(TRANS("Choose root folder for sample. All subfolders will be scanned for .wav and .aif files."),
			File(myProcessor->m_UserWavRootFolder),
			"*.",
			true);
		if (fc.browseForDirectory())
		{
			File chosenDirectory = fc.getResult();
            */
        myChooser = std::make_unique<FileChooser>(TRANS("Choose root folder for sample. All subfolders will be scanned for .wav and .aif files."), File(myProcessor->m_UserWavRootFolder), "*.");
        myChooser->launchAsync(FileBrowserComponent::openMode | FileBrowserComponent::canSelectDirectories,
            [this](const FileChooser& fileChooser) {
            File chosenDirectory(fileChooser.getResult());

            if (chosenDirectory.isDirectory() == true) {

                myProcessor->m_UserWavRootFolder = chosenDirectory.getFullPathName();
                myProcessor->writeSettingsToFile();
            }
            c_wavFolder->setText(myProcessor->m_UserWavRootFolder, false);
        });
        //[/UserButtonCode_c_folderWavSelect]
    }
    else if (buttonThatWasClicked == c_disableGFX.get())
    {
        //[UserButtonCode_c_disableGFX] -- add your button handler code here..
		myProcessor->m_disableOpenGLGFX = c_disableGFX->getToggleState();
		myProcessor->writeSettingsToFile();
		myEditor->vaporizerComponent->updateAll();
		myEditor->repaint();
        //[/UserButtonCode_c_disableGFX]
    }
    else if (buttonThatWasClicked == c_presetMPE.get())
    {
        //[UserButtonCode_c_presetMPE] -- add your button handler code here..
        VASTPresetElement lElem = myProcessor->m_presetData.getCurPatchData();
        lElem.mpepreset = c_presetMPE->getToggleState();
        myProcessor->m_presetData.exchangeCurPatchData(lElem);
        myEditor->vaporizerComponent->updateAll();
        //[/UserButtonCode_c_presetMPE]
    }
    else if (buttonThatWasClicked == c_tuningSelect.get())
    {
        //[UserButtonCode_c_tuningSelect] -- add your button handler code here..
        myChooser = std::make_unique<FileChooser>(TRANS("Choose AnaMark .tun tuning file."), File(myProcessor->getVSTPath()), "*.tun"); //check - use a different starting path?
        myChooser->launchAsync(FileBrowserComponent::openMode | FileBrowserComponent::canSelectFiles,
            [this](const FileChooser& fileChooser) {
                File chosenFile(fileChooser.getResult());

                if (chosenFile.isDirectory() == false) {
                    myProcessor->setUserTuningFile(chosenFile.getFullPathName());
                    myProcessor->writeSettingsToFile();
                }
                c_tuning->setText(myProcessor->m_UserTuningFile, false);
            });
        //[/UserButtonCode_c_tuningSelect]
    }
    else if (buttonThatWasClicked == c_tuningRemove.get())
    {
        //[UserButtonCode_c_tuningRemove] -- add your button handler code here..

        myProcessor->setUserTuningFile("");
        myProcessor->writeSettingsToFile();
        c_tuning->setText(myProcessor->m_UserTuningFile, false);
        //[/UserButtonCode_c_tuningRemove]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void VASTPresetEditorComponent::comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == c_presetCategoryCombo.get())
    {
        //[UserComboBoxCode_c_presetCategoryCombo] -- add your combo box handling code here..
		VASTPresetElement lElem = myProcessor->m_presetData.getCurPatchData();
		lElem.category = c_presetCategoryCombo->getText().dropLastCharacters(c_presetCategoryCombo->getText().length() -  2);
		myProcessor->m_presetData.exchangeCurPatchData(lElem);
		myEditor->vaporizerComponent->updateAll();
        //[/UserComboBoxCode_c_presetCategoryCombo]
    }
    else if (comboBoxThatHasChanged == c_uiThemeCombo.get())
    {
        //[UserComboBoxCode_c_uiThemeCombo] -- add your combo box handling code here..
		int val = c_uiThemeCombo->getSelectedItemIndex();
		if (val >= 0) {
			myEditor->setActiveLookAndFeel(val);
			myProcessor->writeSettingsToFile();
			myEditor->vaporizerComponent->updateAll();
			myEditor->vaporizerComponent->repaint();
		}

        //[/UserComboBoxCode_c_uiThemeCombo]
    }
    else if (comboBoxThatHasChanged == c_WTmode.get())
    {
        //[UserComboBoxCode_c_WTmode] -- add your combo box handling code here..
		int val = c_WTmode->getSelectedItemIndex();
        if (myProcessor->getWTmode() != val) {
            myProcessor->setWTmode(val);
            myProcessor->writeSettingsToFile();
        }
        //[/UserComboBoxCode_c_WTmode]
    }
    else if (comboBoxThatHasChanged == c_uiFontSize.get())
    {
        //[UserComboBoxCode_c_uiFontSize] -- add your combo box handling code here..
		int val = c_uiFontSize->getSelectedItemIndex();
        if (myProcessor->getUIFontSize() != val) {
            myProcessor->setUIFontSize(val);
            myProcessor->writeSettingsToFile();
        }
        //[/UserComboBoxCode_c_uiFontSize]
    }
    else if (comboBoxThatHasChanged == c_MPEmode.get())
    {
        //[UserComboBoxCode_c_MPEmode] -- add your combo box handling code here..
        int val = c_MPEmode->getSelectedItemIndex();
        if (myProcessor->getMPEmode() != val) {
            myProcessor->setMPEmode(val);
            myProcessor->writeSettingsToFile();
        }
        //[/UserComboBoxCode_c_MPEmode]
    }
    else if (comboBoxThatHasChanged == c_permaLink.get())
    {
        //[UserComboBoxCode_c_permaLink] -- add your combo box handling code here..
        int val = c_permaLink->getSelectedItemIndex();
        if (myProcessor->getModWheelPermaLink() != val) {
            myProcessor->setModWheelPermaLink(val);
            myProcessor->writeSettingsToFile();
        }
        //[/UserComboBoxCode_c_permaLink]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}

void VASTPresetEditorComponent::sliderValueChanged (juce::Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == c_presetBendRange.get())
    {
        //[UserSliderCode_c_presetBendRange] -- add your slider handling code here..
        VASTPresetElement lElem = myProcessor->m_presetData.getCurPatchData();
        lElem.mpebendrange = c_presetBendRange->getValue();
        myProcessor->m_presetData.exchangeCurPatchData(lElem);
        myEditor->vaporizerComponent->updateAll();
        //[/UserSliderCode_c_presetBendRange]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void VASTPresetEditorComponent::textEditorTextChanged(TextEditor& textEditorThatWasChanged) {
	if (&textEditorThatWasChanged == c_presetName.get())
	{
		VASTPresetElement lElem = myProcessor->m_presetData.getCurPatchData();
		lElem.presetname = c_presetName->getText();
        lElem.internalid = ""; //a new file has to be selected
		myProcessor->m_presetData.exchangeCurPatchData(lElem);
		myEditor->vaporizerComponent->updateAll();
	}
	else if (&textEditorThatWasChanged == c_presetAuthor.get())
	{
		VASTPresetElement lElem = myProcessor->m_presetData.getCurPatchData();
		lElem.authorname = c_presetAuthor->getText();
		myProcessor->m_presetData.exchangeCurPatchData(lElem);
	}
	else if (&textEditorThatWasChanged == c_presetTag.get())
	{
		VASTPresetElement lElem = myProcessor->m_presetData.getCurPatchData();
		lElem.freetag = c_presetTag->getText();
		myProcessor->m_presetData.exchangeCurPatchData(lElem);
	}
	else if (&textEditorThatWasChanged == c_presetComments.get())
	{
		VASTPresetElement lElem = myProcessor->m_presetData.getCurPatchData();
		lElem.comments = c_presetComments->getText();
		myProcessor->m_presetData.exchangeCurPatchData(lElem);
	}
	else if (&textEditorThatWasChanged == c_pluginWidth.get())
	{
	}
	else if (&textEditorThatWasChanged == c_pluginHeight.get())
	{
	}
}

void VASTPresetEditorComponent::updateAll() {
	c_presetName->setText(myProcessor->m_presetData.getCurPatchData().presetname, false);
	//c_presetCategoryCombo->setText(myProcessor->m_curPatchData.category, NotificationType::sendNotification);
	c_presetCategoryCombo->setText(myProcessor->m_presetData.getCurPatchData().category, NotificationType::dontSendNotification);
	for (int i = 0; i < c_presetCategoryCombo->getNumItems(); i++)
		if (c_presetCategoryCombo->getItemText(i).startsWithIgnoreCase(myProcessor->m_presetData.getCurPatchData().category.dropLastCharacters(myProcessor->m_presetData.getCurPatchData().category.length() - 2))) {
			c_presetCategoryCombo->setSelectedItemIndex(i, NotificationType::dontSendNotification);
			break;
		}

	c_uiThemeCombo->setSelectedItemIndex(myProcessor->m_activeLookAndFeel, NotificationType::dontSendNotification); //dont send
	c_uiThemeCombo->repaint();
	c_uiFontSize->setSelectedItemIndex(myProcessor->m_uiFontSize, NotificationType::dontSendNotification); //dont send
	c_uiFontSize->repaint();
	c_disableGFX->setToggleState(myProcessor->m_disableOpenGLGFX, NotificationType::dontSendNotification);
	c_WTmode->setSelectedItemIndex(myProcessor->getWTmode(), NotificationType::dontSendNotification);
	c_WTmode->repaint();
    c_MPEmode->setSelectedItemIndex(myProcessor->getMPEmode(), NotificationType::dontSendNotification);
    c_MPEmode->repaint();
    c_permaLink->setSelectedItemIndex(myProcessor->getModWheelPermaLink(), NotificationType::dontSendNotification);
    c_permaLink->repaint();

	c_presetAuthor->setText(myProcessor->m_presetData.getCurPatchData().authorname, NotificationType::dontSendNotification);
	c_presetTag->setText(myProcessor->m_presetData.getCurPatchData().freetag, NotificationType::dontSendNotification);
	c_presetComments->setText(myProcessor->m_presetData.getCurPatchData().comments, NotificationType::dontSendNotification);
    c_presetMPE->setToggleState(myProcessor->m_presetData.getCurPatchData().mpepreset, NotificationType::dontSendNotification);
    c_presetBendRange->setValue(myProcessor->m_presetData.getCurPatchData().mpebendrange, NotificationType::dontSendNotification);

	c_presetFolder->setText(myProcessor->m_UserPresetRootFolder, false);
	c_wavetableFolder->setText(myProcessor->m_UserWavetableRootFolder, false);
	c_wavFolder->setText(myProcessor->m_UserWavRootFolder, false);
	c_pluginWidth->setText(String(myProcessor->m_iUserTargetPluginWidth), false);
	c_pluginHeight->setText(String(myProcessor->m_iUserTargetPluginHeight), false);
    c_tuning->setText(myProcessor->m_UserTuningFile, false);

	c_presetTag->applyFontToAllText(myEditor->getCurrentVASTLookAndFeel()->getTextEditorFont(*c_presetTag.get()));
	c_presetTag->applyColourToAllText(myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colBoxText), true);
	c_presetTag->setColour(TextEditor::backgroundColourId, myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colBoxTextBackground));
	c_presetAuthor->applyFontToAllText(myEditor->getCurrentVASTLookAndFeel()->getTextEditorFont(*c_presetAuthor.get()));
	c_presetAuthor->applyColourToAllText(myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colBoxText), true);
	c_presetAuthor->setColour(TextEditor::backgroundColourId, myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colBoxTextBackground));
	c_presetComments->applyFontToAllText(myEditor->getCurrentVASTLookAndFeel()->getTextEditorFont(*c_presetComments.get()));
	c_presetComments->applyColourToAllText(myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colBoxText), true);
	c_presetComments->setColour(TextEditor::backgroundColourId, myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colBoxTextBackground));
	c_presetName->applyFontToAllText(myEditor->getCurrentVASTLookAndFeel()->getTextEditorFont(*c_presetName.get()));
	c_presetName->applyColourToAllText(myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colBoxText), true);
	c_presetName->setColour(TextEditor::backgroundColourId, myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colBoxTextBackground));
	c_presetFolder->applyFontToAllText(myEditor->getCurrentVASTLookAndFeel()->getTextEditorFont(*c_presetFolder.get()));
	c_presetFolder->applyColourToAllText(myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colBoxText), true);
	c_presetFolder->setColour(TextEditor::backgroundColourId, myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colBoxTextBackground));
	c_wavetableFolder->applyFontToAllText(myEditor->getCurrentVASTLookAndFeel()->getTextEditorFont(*c_wavetableFolder.get()));
	c_wavetableFolder->applyColourToAllText(myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colBoxText), true);
	c_wavetableFolder->setColour(TextEditor::backgroundColourId, myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colBoxTextBackground));
	c_wavFolder->applyFontToAllText(myEditor->getCurrentVASTLookAndFeel()->getTextEditorFont(*c_wavFolder.get()));
	c_wavFolder->applyColourToAllText(myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colBoxText), true);
	c_wavFolder->setColour(TextEditor::backgroundColourId, myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colBoxTextBackground));
	c_pluginWidth->applyFontToAllText(myEditor->getCurrentVASTLookAndFeel()->getTextEditorFont(*c_pluginWidth.get()));
	c_pluginWidth->applyColourToAllText(myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colBoxText), true);
	c_pluginWidth->setColour(TextEditor::backgroundColourId, myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colBoxTextBackground));
	c_pluginHeight->applyFontToAllText(myEditor->getCurrentVASTLookAndFeel()->getTextEditorFont(*c_pluginHeight.get()));
	c_pluginHeight->applyColourToAllText(myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colBoxText), true);
	c_pluginHeight->setColour(TextEditor::backgroundColourId, myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colBoxTextBackground));
    c_tuning->applyFontToAllText(myEditor->getCurrentVASTLookAndFeel()->getTextEditorFont(*c_pluginHeight.get()));
    c_tuning->applyColourToAllText(myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colBoxText), true);
    c_tuning->setColour(TextEditor::backgroundColourId, myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colBoxTextBackground));
}

void VASTPresetEditorComponent::lookAndFeelChanged() {
	updateAll();
	repaint();
/*
	c_presetTag->setLookAndFeel(myEditor->getCurrentVASTLookAndFeel());
	c_presetAuthor->setLookAndFeel(myEditor->getCurrentVASTLookAndFeel());
	c_presetComments->setLookAndFeel(myEditor->getCurrentVASTLookAndFeel());
	c_presetName->setLookAndFeel(myEditor->getCurrentVASTLookAndFeel());
	c_presetFolder->setLookAndFeel(myEditor->getCurrentVASTLookAndFeel());
	c_wavetableFolder->setLookAndFeel(myEditor->getCurrentVASTLookAndFeel());
	c_wavFolder->setLookAndFeel(myEditor->getCurrentVASTLookAndFeel());
	c_pluginWidth->setLookAndFeel(myEditor->getCurrentVASTLookAndFeel());
	c_pluginHeight->setLookAndFeel(myEditor->getCurrentVASTLookAndFeel());
	*/
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="VASTPresetEditorComponent"
                 componentName="" parentClasses="public Component, public TextEditor::Listener"
                 constructorParams="AudioProcessorEditor *editor, AudioProcessor* processor"
                 variableInitialisers="myEditor((VASTAudioProcessorEditor*)editor), myProcessor((VASTAudioProcessor*)processor)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="800" initialHeight="575">
  <BACKGROUND backgroundColour="ff323e44">
    <RECT pos="0 0 0M 0M" fill="linear: -4% -1.538%, 102% 102%, 0=4f616d7e, 1=bd03070a"
          hasStroke="0"/>
  </BACKGROUND>
  <GROUPCOMPONENT name="new group" id="9f561b343cc39aba" memberName="groupComponent2"
                  virtualName="" explicitFocusOrder="0" pos="1% 37.913% 98% 60.87%"
                  title="SETTINGS" textpos="33"/>
  <GROUPCOMPONENT name="new group" id="adbdb33ee5d20e2" memberName="groupComponent"
                  virtualName="" explicitFocusOrder="0" pos="1% 1.391% 98% 34.783%"
                  title="PRESET DATA" textpos="33"/>
  <TEXTEDITOR name="c_presetAuthor" id="3c4f0c34b8ceb2d" memberName="c_presetAuthor"
              virtualName="" explicitFocusOrder="4" pos="16.875% 18.609% 30.125% 2.435%"
              initialText="" multiline="0" retKeyStartsLine="0" readonly="0"
              scrollbars="0" caret="1" popupmenu="1"/>
  <LABEL name="new label" id="bad1b011078023ef" memberName="label4" virtualName=""
         explicitFocusOrder="0" pos="3% 18.957% 12.875% 1.913%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="AUTHOR" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Code Pro Demo"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="34"/>
  <TEXTEDITOR name="c_presetName" id="1f7770d839b337bf" memberName="c_presetName"
              virtualName="" explicitFocusOrder="1" pos="16.875% 6.261% 30.375% 2.435%"
              initialText="" multiline="0" retKeyStartsLine="0" readonly="0"
              scrollbars="0" caret="1" popupmenu="1"/>
  <TEXTEDITOR name="c_presetTag" id="4e6693506aa08477" memberName="c_presetTag"
              virtualName="" explicitFocusOrder="3" pos="16.875% 14.783% 78.125% 2.435%"
              tooltip="Add several tags separated by space ( ) or comma (,) or hashtag (#). Tags can be used to filter in the preset browser."
              initialText="" multiline="0" retKeyStartsLine="0" readonly="0"
              scrollbars="0" caret="1" popupmenu="1"/>
  <LABEL name="new label" id="f9de9a64cb5680c" memberName="label3" virtualName=""
         explicitFocusOrder="0" pos="3% 15.13% 12.875% 1.913%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="TAG" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Code Pro Demo"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="34"/>
  <LABEL name="new label" id="52416f90bd037bdb" memberName="label2" virtualName=""
         explicitFocusOrder="0" pos="3% 10.957% 12.875% 1.913%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="CATEGORY" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Code Pro Demo"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="34"/>
  <LABEL name="new label" id="7969f6422cf2ee4e" memberName="label13" virtualName=""
         explicitFocusOrder="0" pos="3% 6.609% 12.875% 1.913%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="PRESET NAME" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Code Pro Demo"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="34"/>
  <TEXTEDITOR name="c_presetFolder" id="c7d11561c951f260" memberName="c_presetFolder"
              virtualName="" explicitFocusOrder="0" pos="16.875% 42.609% 39.125% 2.435%"
              initialText="" multiline="0" retKeyStartsLine="0" readonly="1"
              scrollbars="0" caret="0" popupmenu="1"/>
  <LABEL name="new label" id="5b623b6fb0e5875c" memberName="label7" virtualName=""
         explicitFocusOrder="0" pos="3% 42.957% 12.875% 1.913%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="PRESETS" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Code Pro Demo"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="34"/>
  <TEXTBUTTON name="c_folderSelect" id="c633b69850f3dd2" memberName="c_folderSelect"
              virtualName="" explicitFocusOrder="8" pos="60.875% 42.609% 16% 2.435%"
              buttonText="Select Preset Folder" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <TEXTEDITOR name="c_pluginWidth" id="33e2ae6172940bd" memberName="c_pluginWidth"
              virtualName="" explicitFocusOrder="0" pos="16.875% 55.652% 13.125% 2.435%"
              initialText="" multiline="0" retKeyStartsLine="0" readonly="1"
              scrollbars="0" caret="0" popupmenu="1"/>
  <LABEL name="new label" id="edd83ca64b70f0a5" memberName="label8" virtualName=""
         explicitFocusOrder="0" pos="3% 55.826% 12.875% 1.913%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="WIDTH" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Code Pro Demo"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="34"/>
  <TEXTEDITOR name="c_pluginHeight" id="3249bf568d5f01c5" memberName="c_pluginHeight"
              virtualName="" explicitFocusOrder="0" pos="43% 55.652% 13.125% 2.435%"
              initialText="" multiline="0" retKeyStartsLine="0" readonly="1"
              scrollbars="0" caret="0" popupmenu="1"/>
  <LABEL name="new label" id="602703543afea8c3" memberName="label9" virtualName=""
         explicitFocusOrder="0" pos="30% 55.826% 12.875% 1.913%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="HEIGHT" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Code Pro Demo"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="34"/>
  <TEXTBUTTON name="c_currentSize" id="23af12fcb694b640" memberName="c_currentSize"
              virtualName="" explicitFocusOrder="11" pos="60.875% 55.826% 16% 2.435%"
              buttonText="Select Size" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <TEXTBUTTON name="c_defaultSize" id="3b68b9b95f0e3bd4" memberName="c_defaultSize"
              virtualName="" explicitFocusOrder="12" pos="79% 55.826% 16% 2.435%"
              buttonText="Use Default Size" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <TEXTEDITOR name="c_wavetableFolder" id="77306cec023b8bd" memberName="c_wavetableFolder"
              virtualName="" explicitFocusOrder="0" pos="16.875% 46.435% 39.125% 2.435%"
              initialText="" multiline="0" retKeyStartsLine="0" readonly="1"
              scrollbars="0" caret="0" popupmenu="1"/>
  <LABEL name="new label" id="dbc0f2492bd981bf" memberName="label5" virtualName=""
         explicitFocusOrder="0" pos="3% 46.783% 12.875% 1.913%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="WAVETABLES" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Code Pro Demo"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="34"/>
  <TEXTBUTTON name="c_folderWTSelect" id="86a30ff78b399d56" memberName="c_folderWTSelect"
              virtualName="" explicitFocusOrder="9" pos="60.875% 46.435% 16% 2.435%"
              buttonText="Select WT Folder" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <TEXTEDITOR name="c_wavFolder" id="4b184295c8681b8b" memberName="c_wavFolder"
              virtualName="" explicitFocusOrder="0" pos="16.875% 50.261% 39.125% 2.435%"
              initialText="" multiline="0" retKeyStartsLine="0" readonly="1"
              scrollbars="0" caret="0" popupmenu="1"/>
  <LABEL name="new label" id="7ada2650fc012320" memberName="label6" virtualName=""
         explicitFocusOrder="0" pos="3% 50.609% 12.875% 1.913%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="SAMPLES" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Code Pro Demo"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="34"/>
  <TEXTBUTTON name="c_folderWavSelect" id="1a550cfe683bb8d5" memberName="c_folderWavSelect"
              virtualName="" explicitFocusOrder="10" pos="60.875% 50.261% 16% 2.435%"
              buttonText="Select WAV Folder" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <TEXTEDITOR name="c_presetComments" id="d90330acdd589ba9" memberName="c_presetComments"
              virtualName="" explicitFocusOrder="5" pos="16.875% 22.435% 78.125% 2.435%"
              initialText="" multiline="0" retKeyStartsLine="0" readonly="0"
              scrollbars="0" caret="1" popupmenu="1"/>
  <LABEL name="new label" id="4ab03e1b1bb99acd" memberName="label10" virtualName=""
         explicitFocusOrder="0" pos="3% 22.783% 12.875% 1.913%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="COMMENTS" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Code Pro Demo"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="34"/>
  <COMBOBOX name="c_presetCategoryCombo" id="90780feb0180733d" memberName="c_presetCategoryCombo"
            virtualName="VASTComboBox" explicitFocusOrder="2" pos="16.875% 10.435% 13.125% 2.609%"
            editable="0" layout="33" items="AR Arpeggio &#10;AT Atmosphere&#10;BA Bass&#10;BR Brass &#10;BL Bell&#10;CH Chord&#10;DK Drum kit&#10;DR Drum&#10;DL Drum loop&#10;FX Effect&#10;GT Guitar&#10;IN Instrument&#10;KB Keyboard&#10;LD Lead&#10;MA Mallet&#10;OR Organ&#10;OC Orchestral&#10;PD Pad&#10;PN Piano&#10;PL Plucked&#10;RI Riser&#10;RD Reed&#10;ST String&#10;SY Synth&#10;SQ Sequence / Split&#10;TG Trancegate&#10;VC Vocal / Voice&#10;WW Woodwind"
            textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <LABEL name="new label" id="413a9c4a8fbe5523" memberName="label11" virtualName=""
         explicitFocusOrder="0" pos="3% 59.652% 12.875% 1.913%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="SKIN" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Code Pro Demo"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="34"/>
  <COMBOBOX name="c_uiThemeCombo" id="c0e2623a507c7127" memberName="c_uiThemeCombo"
            virtualName="VASTComboBox" explicitFocusOrder="13" pos="16.875% 59.478% 23% 2.609%"
            tooltip="Ui colour and display theme" editable="0" layout="33"
            items="Classic&#10;IceCubes&#10;BloodTech&#10;Dark" textWhenNonSelected=""
            textWhenNoItems="(no choices)"/>
  <LABEL name="new label" id="5c44d4c6bdd0b7a4" memberName="label12" virtualName=""
         explicitFocusOrder="0" pos="3% 74.957% 12.875% 1.913%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="WAVET. MODE" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Code Pro Demo"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="34"/>
  <COMBOBOX name="c_WTmode" id="e8666c43909ab931" memberName="c_WTmode" virtualName="VASTComboBox"
            explicitFocusOrder="16" pos="16.875% 74.609% 13.125% 2.609%"
            tooltip="Wavetable frequency calculation mode - Sharp (ideal), Soft (butterworth), Dull (linear)"
            editable="0" layout="33" items="Sharp&#10;Soft&#10;Dull" textWhenNonSelected=""
            textWhenNoItems="(no choices)"/>
  <GENERICCOMPONENT name="c_iconMaximizeEditor" id="ded548631676712b" memberName="c_iconMaximizeEditor"
                    virtualName="" explicitFocusOrder="0" pos="1Rr 1 1.5% 2.087%"
                    class="VASTDrawableButton" params="&quot;c_iconMaximize&quot;, arrow_right_corner_svg, arrow_right_corner_svgSize, &quot;Maximize area&quot;"/>
  <LABEL name="new label" id="6e81c839abaf8532" memberName="label14" virtualName=""
         explicitFocusOrder="0" pos="3% 67.304% 12.875% 1.913%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="DISABLE GFX" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Code Pro Demo"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="34"/>
  <TOGGLEBUTTON name="c_disableGFX" id="ec7ed958fca494e8" memberName="c_disableGFX"
                virtualName="" explicitFocusOrder="15" pos="16.5% 66.609% 2.125% 2.957%"
                tooltip="Disable openGL graphics to improve performance on older systems (oscilloscope on top)"
                buttonText="" connectedEdges="0" needsCallback="1" radioGroupId="0"
                state="0"/>
  <LABEL name="new label" id="531603d21f1ea7f2" memberName="label15" virtualName=""
         explicitFocusOrder="0" pos="3% 78.783% 12.875% 1.913%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="MPE MODE" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Code Pro Demo"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="34"/>
  <LABEL name="new label" id="990b2f73bff98ed0" memberName="label16" virtualName=""
         explicitFocusOrder="0" pos="3% 63.478% 12.875% 1.913%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="FONT SIZE" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Code Pro Demo"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="34"/>
  <COMBOBOX name="c_uiFontSize" id="76e4de2cba671461" memberName="c_uiFontSize"
            virtualName="VASTComboBox" explicitFocusOrder="14" pos="16.875% 63.304% 13.125% 2.609%"
            tooltip="Font size" editable="0" layout="33" items="Normal&#10;Larger&#10;Smaller"
            textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <LABEL name="new label" id="340b1b596a8a2d05" memberName="label17" virtualName=""
         explicitFocusOrder="0" pos="3% 27.478% 12.875% 1.913%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="MPE PRESET" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Code Pro Demo"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="34"/>
  <TOGGLEBUTTON name="c_presetMPE" id="e617562d313f1f14" memberName="c_presetMPE"
                virtualName="" explicitFocusOrder="6" pos="16.5% 26.957% 2.125% 2.957%"
                tooltip="Enable MIDI Polyphonic Expression (MPE) mode for supported controllers for preset"
                buttonText="" connectedEdges="0" needsCallback="1" radioGroupId="0"
                state="0"/>
  <COMBOBOX name="c_MPEmode" id="b9dfd606cbcb0dff" memberName="c_MPEmode"
            virtualName="VASTComboBox" explicitFocusOrder="17" pos="16.875% 78.261% 13.125% 2.609%"
            tooltip="Enable MIDI Polyphonic Expression (MPE) mode for supported controllers always per default"
            editable="0" layout="33" items="From preset&#10;Always on&#10;Always off"
            textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <SLIDER name="c_presetBendRange" id="6a281b071d356eaa" memberName="c_presetBendRange"
          virtualName="VASTSlider" explicitFocusOrder="7" pos="16.875% 30.783% 7.5% 3.13%"
          tooltip="MPE pitch bend sensitivity" min="1.0" max="96.0" int="1.0"
          style="IncDecButtons" textBoxPos="TextBoxLeft" textBoxEditable="1"
          textBoxWidth="28" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <LABEL name="new label" id="5cac5c0ea9033608" memberName="label18" virtualName=""
         explicitFocusOrder="0" pos="3% 31.304% 12.875% 1.913%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="MPE BEND RANGE"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Code Pro Demo" fontsize="11.0" kerning="0.0" bold="0"
         italic="0" justification="34"/>
  <TEXTEDITOR name="c_tuning" id="55645c16f0147aa6" memberName="c_tuning" virtualName=""
              explicitFocusOrder="0" pos="16.875% 85.217% 39.125% 2.435%" tooltip="AnaMark Tuning file (.TUN) for custom tuning"
              initialText="" multiline="0" retKeyStartsLine="0" readonly="1"
              scrollbars="0" caret="0" popupmenu="1"/>
  <LABEL name="new label" id="639d4b3c348badbd" memberName="label19" virtualName=""
         explicitFocusOrder="0" pos="3% 85.391% 12.875% 1.913%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="TUNING" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Code Pro Demo"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="34"/>
  <TEXTBUTTON name="c_tuningSelect" id="226d55cecbcaa147" memberName="c_tuningSelect"
              virtualName="" explicitFocusOrder="18" pos="60.875% 85.217% 16% 2.435%"
              buttonText="Select .TUN File" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <TEXTBUTTON name="c_tuningRemove" id="2188ea1bfef98d5d" memberName="c_tuningRemove"
              virtualName="" explicitFocusOrder="18" pos="79% 85.217% 16% 2.435%"
              buttonText="Use Default Tuning" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <COMBOBOX name="c_permaLink" id="142ebf586a1b0644" memberName="c_permaLink"
            virtualName="VASTComboBox" explicitFocusOrder="16" pos="16.875% 89.043% 23% 2.609%"
            tooltip="Permanently link Modulation Wheel to Custom Modulators"
            editable="0" layout="33" items="---&#10;CUSTOM MODULATOR 1&#10;CUSTOM MODULATOR 2&#10;CUSTOM MODULATOR 3&#10;CUSTOM MODULATOR 4"
            textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <LABEL name="new label" id="9259004066f0bc20" memberName="label20" virtualName=""
         explicitFocusOrder="0" pos="3% 89.565% 12.875% 1.913%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="MOD.WHEEL PERM.&#10;"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Code Pro Demo" fontsize="11.0" kerning="0.0" bold="0"
         italic="0" justification="34"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif

//==============================================================================
// Binary resources - be careful not to edit any of these sections!

// JUCER_RESOURCE: settings_png, 21804, "../../Resources/settings.png"
static const unsigned char resource_VASTPresetEditorComponent_settings_png[] = { 137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,2,0,0,0,2,0,8,6,0,0,0,244,120,212,250,0,0,0,6,98,75,71,68,0,255,0,255,
0,255,160,189,167,147,0,0,32,0,73,68,65,84,120,156,236,221,121,188,212,117,189,199,241,247,231,59,103,225,176,200,166,136,184,0,185,161,84,138,75,106,40,28,52,83,187,185,85,180,153,102,139,180,93,52,186,
92,22,225,204,124,206,204,97,189,60,162,194,107,74,94,51,173,110,55,242,102,101,230,150,158,131,89,86,174,21,224,150,168,87,1,23,112,67,214,51,191,207,253,227,12,10,202,114,56,204,204,231,55,243,123,63,
31,15,30,46,15,132,215,193,51,243,253,204,247,183,9,136,168,170,92,126,249,229,251,213,213,213,29,10,224,16,0,7,0,24,96,102,251,137,200,190,91,254,30,64,207,119,252,103,107,1,172,0,240,34,128,23,69,100,
101,225,239,159,203,231,243,79,164,82,169,39,84,117,85,25,191,12,34,42,49,241,14,32,162,174,153,48,97,66,67,207,158,61,143,75,165,82,71,1,120,63,128,163,205,236,8,188,123,113,47,150,181,34,178,196,204,
30,17,145,71,0,252,13,192,253,170,186,161,68,191,31,17,149,16,7,0,162,10,50,125,250,244,35,66,8,103,134,16,206,0,48,202,204,26,156,147,214,137,72,91,20,69,183,133,16,110,83,213,71,157,123,136,168,147,
56,0,16,197,220,244,233,211,15,174,169,169,185,4,192,103,205,236,64,239,158,93,120,86,68,254,27,192,66,85,125,202,59,134,136,118,140,3,0,81,76,169,106,0,112,19,128,143,152,89,202,187,103,119,136,72,30,
192,205,0,62,166,170,145,119,15,17,189,91,240,14,32,162,237,203,231,243,35,205,236,236,74,91,252,1,192,204,82,102,118,110,62,159,63,193,187,133,136,182,143,3,0,81,76,133,16,206,244,110,216,83,169,84,234,
44,239,6,34,218,62,14,0,68,49,37,34,99,188,27,138,160,209,59,128,136,182,143,231,0,16,197,144,170,118,7,240,170,153,213,122,183,236,9,17,217,8,160,15,47,21,36,138,31,238,0,16,197,211,137,149,190,248,3,
128,153,213,3,248,128,119,7,17,189,27,7,0,162,24,138,162,232,20,239,134,98,169,166,175,133,168,154,112,0,32,138,161,16,66,213,44,154,213,244,181,16,85,19,14,0,68,49,51,118,236,216,148,153,85,211,229,115,
39,22,238,105,64,68,49,194,23,37,81,204,12,27,54,108,24,74,119,63,255,178,51,179,222,232,120,48,17,17,197,8,7,0,162,152,73,165,82,35,188,27,138,45,138,162,170,251,154,136,42,29,7,0,162,152,49,179,170,
91,44,67,8,85,247,53,17,85,58,14,0,68,241,115,140,119,64,9,28,237,29,64,68,219,226,0,64,20,51,34,242,62,239,134,98,51,179,170,251,154,136,42,29,7,0,162,24,185,252,242,203,247,53,179,254,222,29,37,48,72,
85,251,121,71,16,209,219,56,0,16,197,72,93,93,221,17,222,13,37,52,204,59,128,136,222,198,1,128,40,70,162,40,170,218,1,160,154,191,54,162,74,196,1,128,40,70,68,164,106,63,37,135,16,56,0,16,197,8,7,0,162,
24,17,145,170,93,36,205,172,106,191,54,162,74,196,1,128,40,94,14,243,14,40,161,106,254,218,136,42,14,7,0,162,152,24,63,126,124,189,153,29,232,221,81,66,67,198,141,27,87,241,143,56,38,170,22,28,0,136,98,
162,127,255,254,131,81,221,175,201,154,1,3,6,28,224,29,65,68,29,170,249,205,134,168,210,188,199,59,160,212,66,8,85,255,53,18,85,10,14,0,68,49,17,69,209,80,239,134,82,11,33,84,253,215,72,84,41,56,0,16,
197,68,18,22,71,51,171,250,175,145,168,82,112,0,32,138,143,36,108,143,39,225,107,36,170,8,28,0,136,98,34,9,159,142,69,164,234,191,70,162,74,193,1,128,40,38,68,100,176,119,67,169,153,89,213,127,141,68,
149,130,3,0,81,12,76,152,48,161,161,74,159,2,248,78,3,84,181,206,59,130,136,56,0,16,197,66,143,30,61,6,121,55,148,73,0,48,208,59,130,136,56,0,16,197,130,136,36,101,0,64,62,159,223,223,187,129,136,56,0,
16,197,66,42,149,74,204,162,152,164,175,149,40,206,56,0,16,197,64,20,69,137,89,20,147,244,181,18,197,25,7,0,162,24,72,210,33,128,16,2,7,0,162,24,224,0,64,20,3,34,146,152,69,209,204,18,243,181,18,197,25,
7,0,162,120,72,204,14,64,146,134,29,162,56,227,0,64,20,15,137,25,0,144,172,175,149,40,182,56,0,16,57,83,213,0,224,0,239,142,114,49,179,3,1,136,119,7,81,210,113,0,32,242,55,192,204,234,189,35,202,168,219,
212,169,83,247,246,142,32,74,58,14,0,68,254,14,242,14,40,183,250,250,250,3,189,27,136,146,142,3,0,145,179,40,138,18,183,24,38,241,107,38,138,27,14,0,68,254,18,183,3,16,66,224,0,64,228,140,3,0,145,51,17,
73,220,0,128,4,14,61,68,113,195,1,128,200,153,136,28,225,221,80,110,102,150,184,175,153,40,110,56,0,16,249,59,210,59,192,65,18,191,102,162,88,225,0,64,228,104,242,228,201,189,145,160,123,0,108,101,200,
196,137,19,123,120,71,16,37,25,7,0,34,71,117,117,117,195,204,44,137,55,197,9,61,123,246,60,220,59,130,40,201,56,0,16,57,18,145,97,222,13,142,146,252,181,19,185,227,0,64,228,72,68,142,243,110,112,148,228,
175,157,200,29,7,0,34,71,34,114,146,119,131,163,19,189,3,136,146,140,3,0,145,19,85,237,14,224,253,222,29,142,142,25,63,126,124,146,158,129,64,20,43,28,0,136,156,228,243,249,99,205,172,214,187,195,139,
153,213,247,233,211,231,104,239,14,162,164,226,0,64,228,68,68,62,224,221,224,45,149,74,157,224,221,64,148,84,28,0,136,156,136,200,135,189,27,188,153,217,233,222,13,68,73,149,196,235,143,137,220,169,106,
79,0,47,155,89,162,143,129,139,200,155,0,246,86,213,13,222,45,68,73,195,29,0,34,7,81,20,141,78,250,226,15,0,102,214,3,192,40,239,14,162,36,226,0,64,228,32,132,112,150,119,67,92,152,25,255,44,136,28,212,
120,7,36,209,180,105,211,14,172,173,173,189,204,204,222,220,184,113,227,119,102,207,158,253,138,119,19,149,149,0,248,136,119,68,92,136,200,191,0,152,224,221,65,229,165,170,253,204,108,130,136,212,3,248,
158,170,62,231,221,148,52,60,7,160,140,84,245,104,0,19,1,124,114,171,203,191,94,53,179,203,67,8,87,171,106,228,152,71,101,210,212,212,116,82,8,225,143,222,29,113,34,34,199,171,234,253,222,29,84,122,99,
199,142,77,13,31,62,252,107,0,90,204,172,119,225,95,111,2,240,179,40,138,230,229,114,185,191,59,230,37,10,15,1,148,65,83,83,211,240,76,38,115,135,153,61,100,102,23,188,227,218,239,62,34,114,165,153,61,
212,212,212,148,228,187,194,37,70,42,149,186,208,187,33,134,62,231,29,64,165,167,170,31,28,62,124,248,67,102,182,96,171,197,31,0,234,0,92,20,66,248,91,38,147,249,205,244,233,211,15,246,106,76,18,238,0,
148,208,212,169,83,247,169,171,171,107,17,145,47,116,230,134,47,34,98,0,126,188,113,227,198,9,179,102,205,90,93,134,68,42,179,113,227,198,213,238,183,223,126,43,0,236,237,221,18,51,171,150,46,93,122,192,
162,69,139,242,222,33,84,124,170,186,55,128,111,3,248,92,103,158,126,41,34,155,205,236,251,34,146,81,213,87,75,95,152,76,220,1,40,145,116,58,125,122,93,93,221,131,0,198,117,246,110,111,102,38,102,118,
97,93,93,221,195,170,250,209,18,39,146,131,129,3,7,126,24,92,252,183,103,224,17,71,28,113,170,119,4,21,159,170,158,3,224,97,51,187,176,179,143,190,46,188,103,94,10,224,111,170,154,248,251,101,148,10,119,
0,138,108,202,148,41,125,187,117,235,118,181,153,141,221,211,95,75,68,22,1,248,186,170,190,92,132,52,138,1,85,189,211,204,78,243,238,136,35,17,249,157,170,242,228,200,42,81,248,212,127,101,177,222,11,
55,110,220,248,53,238,140,22,23,7,128,34,106,106,106,26,17,66,248,25,128,195,138,248,203,62,43,34,95,82,213,59,139,248,107,146,3,85,61,18,192,63,58,251,41,40,129,34,17,57,92,85,159,244,14,161,61,163,170,
103,154,217,15,0,28,80,172,95,83,68,158,1,240,105,85,189,175,88,191,102,210,241,16,64,145,168,234,87,11,103,118,23,115,241,7,128,131,0,220,174,170,223,83,213,110,69,254,181,169,188,46,229,226,191,83,193,
204,198,123,71,80,215,77,152,48,161,65,85,175,4,112,11,138,184,248,3,128,153,13,54,179,182,116,58,253,141,98,254,186,73,198,55,163,61,52,113,226,196,30,61,123,246,252,129,153,125,166,212,191,151,136,60,
210,222,222,254,153,150,150,150,101,165,254,189,168,184,46,191,252,242,253,106,107,107,159,4,208,221,187,37,230,214,138,200,193,170,250,162,119,8,237,30,85,125,175,153,253,55,128,247,150,250,247,18,145,
159,0,24,167,170,235,74,253,123,85,51,238,0,236,129,233,211,167,15,237,209,163,199,125,229,88,252,1,192,204,142,170,169,169,121,40,157,78,95,6,14,111,21,165,174,174,174,25,92,252,59,163,39,128,180,119,
4,117,158,170,6,85,85,0,15,162,12,139,63,0,152,217,5,0,238,83,213,33,229,248,253,170,21,23,145,46,106,106,106,58,62,149,74,253,202,204,246,115,74,248,133,136,92,194,75,100,226,79,85,223,3,224,209,206,
94,13,146,116,34,178,17,192,97,170,250,172,119,11,237,156,170,246,3,112,141,153,157,239,241,251,139,200,243,249,124,254,220,92,46,247,128,199,239,95,233,184,3,208,5,233,116,250,83,169,84,170,205,113,241,
7,128,79,0,120,176,169,169,233,88,199,6,234,156,22,46,254,157,103,102,245,102,214,236,221,65,59,215,212,212,116,60,128,7,189,22,127,0,48,179,253,67,8,139,211,233,244,30,95,105,144,68,41,239,128,74,147,
78,167,167,134,16,174,140,201,27,122,223,16,194,231,27,27,27,95,110,109,109,229,109,84,99,72,85,79,5,48,23,220,109,219,45,34,114,84,99,99,227,239,91,91,91,185,11,16,67,233,116,250,27,169,84,234,103,102,
214,223,187,5,64,109,8,225,19,163,70,141,218,208,214,214,118,175,119,76,37,225,155,210,110,200,100,50,179,0,76,241,238,216,129,31,191,249,230,155,95,157,55,111,222,155,222,33,212,97,252,248,241,245,253,
251,247,127,196,204,14,247,110,169,68,34,178,4,192,49,170,186,201,187,133,58,168,106,79,0,11,203,117,222,83,23,204,108,110,110,158,230,29,81,41,120,8,160,147,84,117,38,226,187,248,3,192,231,122,246,236,
249,103,85,29,230,29,66,29,250,246,237,59,133,139,127,215,153,217,112,116,60,60,139,98,160,112,31,139,191,196,120,241,7,128,203,51,153,204,12,239,136,74,193,67,0,157,160,170,83,204,76,189,59,58,97,0,128,
139,71,143,30,253,84,91,91,219,18,239,152,36,83,213,19,69,228,135,224,107,108,79,157,50,102,204,152,91,91,91,91,87,120,135,36,89,58,157,254,12,128,95,3,216,223,187,165,19,78,25,51,102,204,250,214,214,
86,30,14,216,5,30,2,216,5,85,189,200,204,126,228,221,209,5,11,87,174,92,249,175,11,23,46,220,236,29,146,52,83,167,78,237,95,87,87,247,48,138,124,35,148,164,18,145,255,219,176,97,195,81,179,103,207,126,
197,187,37,105,10,15,175,186,2,192,56,239,150,221,33,34,22,69,209,231,179,217,236,13,222,45,113,198,67,0,59,161,170,39,154,217,213,222,29,93,52,110,191,253,246,251,181,170,246,241,14,73,24,169,171,171,
187,10,92,252,139,198,204,14,236,214,173,219,2,239,142,164,153,50,101,74,223,65,131,6,253,6,21,182,248,3,29,15,86,11,33,92,221,212,212,116,130,119,75,156,113,123,114,7,166,79,159,126,168,136,220,13,160,
151,119,203,30,56,68,68,46,56,229,148,83,22,47,94,188,120,165,119,76,18,100,50,153,57,0,46,241,238,168,66,239,111,108,108,12,109,109,109,173,222,33,73,160,170,199,165,82,169,54,0,199,120,183,236,129,218,
16,194,249,39,159,124,242,47,23,47,94,188,198,59,38,142,56,0,108,199,148,41,83,250,214,213,213,253,30,192,129,222,45,69,208,91,68,46,26,61,122,244,179,109,109,109,127,243,142,169,102,233,116,250,107,34,
50,211,187,163,90,137,200,168,198,198,198,229,173,173,173,143,120,183,84,179,116,58,125,33,128,95,2,232,231,221,82,4,221,67,8,167,159,116,210,73,63,253,195,31,254,176,193,59,38,110,120,8,224,221,164,190,
190,254,218,42,59,123,187,155,136,92,167,170,211,193,243,62,74,34,157,78,159,33,34,223,241,238,168,102,102,38,102,118,117,58,157,30,227,221,82,165,68,85,211,34,114,29,128,106,122,240,216,176,110,221,186,
253,23,248,222,247,46,252,3,121,7,85,189,212,204,190,235,221,81,42,34,114,35,128,139,85,117,173,119,75,181,80,213,81,102,246,59,240,94,255,229,178,86,68,78,231,99,97,139,103,210,164,73,189,26,26,26,174,
3,240,49,239,150,82,49,179,111,101,179,217,249,222,29,113,194,1,96,43,77,77,77,199,166,82,169,123,205,172,222,187,165,196,150,138,200,249,170,250,184,119,72,165,107,106,106,58,33,132,112,59,128,189,188,
91,18,230,85,17,57,77,85,31,244,14,169,116,211,167,79,63,188,166,166,230,151,102,118,132,119,75,41,137,200,102,0,163,56,56,190,141,135,0,10,84,181,123,8,225,167,9,88,252,1,224,72,51,251,107,58,157,62,
215,59,164,146,53,53,53,141,78,165,82,119,128,139,191,135,62,102,246,251,166,166,166,145,222,33,149,44,157,78,159,159,74,165,254,82,237,139,63,0,20,110,223,254,211,194,221,12,9,28,0,222,98,102,115,0,28,
230,221,81,70,123,133,16,126,153,201,100,114,170,202,239,131,221,148,78,167,207,74,165,82,191,51,179,74,190,74,164,210,245,73,165,82,183,165,211,233,211,189,67,42,205,216,177,99,83,170,58,51,132,112,35,
18,52,192,154,217,80,116,60,155,131,192,67,0,0,128,116,58,125,90,8,225,14,51,75,234,159,199,237,34,242,121,85,93,229,29,82,9,84,245,235,0,190,19,147,7,66,17,176,73,68,198,171,234,66,239,144,74,160,170,
131,0,92,111,102,167,121,183,120,40,220,36,232,244,108,54,251,123,239,22,111,73,93,240,222,50,113,226,196,30,61,122,244,88,10,224,32,239,22,103,47,154,217,23,179,217,236,111,189,67,226,74,85,235,0,92,
97,102,188,206,63,158,174,92,185,114,229,55,121,247,203,29,75,167,211,103,139,200,181,0,246,246,110,241,36,34,207,172,93,187,118,120,210,31,158,150,248,251,0,124,248,195,31,206,1,248,23,239,142,24,232,
33,34,159,29,51,102,204,193,199,29,119,220,237,127,252,227,31,249,38,186,149,105,211,166,13,78,165,82,119,154,25,191,87,226,235,248,94,189,122,157,62,114,228,200,91,238,185,231,30,94,229,178,21,85,237,
217,216,216,120,13,128,217,224,213,42,0,208,167,190,190,94,90,91,91,19,189,11,144,232,29,128,105,211,166,13,174,173,173,93,102,102,13,222,45,49,243,183,40,138,46,206,229,114,15,121,135,196,65,83,83,211,
177,33,132,155,192,219,251,86,4,17,121,6,192,121,170,250,176,119,75,28,20,190,127,175,3,240,94,239,150,56,17,145,245,0,134,169,234,179,222,45,94,18,189,3,112,234,169,167,46,4,112,180,119,71,12,237,43,
34,95,26,51,102,76,143,17,35,70,220,123,223,125,247,181,123,7,121,73,167,211,95,74,165,82,139,0,244,247,110,161,78,235,3,224,162,209,163,71,63,223,214,214,150,216,187,6,170,106,247,209,163,71,207,20,145,
107,0,12,244,238,137,161,90,0,251,181,181,181,221,232,29,226,37,177,59,0,133,107,254,255,154,224,19,255,58,235,73,17,185,68,85,91,189,67,202,73,85,247,2,112,85,204,159,125,78,187,32,34,55,0,248,122,210,
110,124,85,56,177,249,106,51,59,216,187,37,206,68,196,242,249,252,177,73,221,237,76,236,229,95,169,84,234,223,185,248,119,202,33,0,238,202,100,50,55,168,106,34,78,148,84,213,99,204,236,1,46,254,149,207,
204,46,52,179,7,84,53,17,59,125,211,166,77,27,172,170,63,45,92,213,196,197,127,23,10,79,13,156,228,221,225,37,145,11,224,244,233,211,15,77,165,82,143,34,193,3,80,87,136,200,102,51,251,161,136,52,169,234,
139,222,61,197,86,184,31,66,218,204,166,1,168,241,238,161,162,106,23,145,25,0,178,170,26,121,199,20,155,170,14,48,179,156,136,124,129,151,167,238,182,40,159,207,15,107,105,105,121,194,59,164,220,18,121,
14,192,169,167,158,154,5,192,231,68,239,190,20,128,99,69,228,226,198,198,198,205,199,29,119,220,223,171,229,106,129,203,47,191,124,223,84,42,117,165,153,93,10,14,134,213,40,0,104,20,145,33,31,252,224,
7,255,116,207,61,247,84,197,229,95,133,179,251,255,213,204,254,27,192,41,72,232,123,250,30,146,84,42,21,90,91,91,111,241,14,41,183,68,238,0,168,170,154,89,198,187,163,210,137,200,27,102,246,195,205,155,
55,207,158,57,115,230,74,239,158,221,53,110,220,184,218,129,3,7,158,39,34,227,0,156,10,46,252,73,17,1,184,203,204,22,174,90,181,234,166,74,188,111,128,170,14,50,179,201,0,190,8,128,183,182,221,67,102,
166,217,108,182,217,187,163,220,146,58,0,244,1,240,164,153,241,204,238,34,16,145,55,0,92,219,222,222,126,117,75,75,203,50,239,158,93,81,213,3,0,124,25,192,23,205,236,64,239,30,114,245,172,153,93,155,207,
231,175,153,49,99,198,243,222,49,187,210,212,212,52,60,149,74,125,197,204,190,0,46,252,69,33,34,171,215,175,95,127,240,156,57,115,94,243,110,41,183,68,14,0,0,160,170,227,204,236,106,239,142,106,35,34,
109,81,20,93,253,198,27,111,220,52,127,254,252,245,222,61,91,107,106,106,26,29,66,248,87,0,231,129,199,248,105,91,237,34,242,75,0,87,168,234,98,239,152,173,169,106,247,40,138,206,15,33,124,197,204,78,
241,238,169,54,133,171,156,174,241,238,240,144,228,1,32,152,217,189,0,78,244,110,169,82,235,68,228,14,0,191,222,184,113,227,111,102,205,154,245,82,185,3,84,181,110,243,230,205,251,166,82,169,51,69,228,
95,1,188,191,220,13,84,121,68,228,17,0,87,108,222,188,249,182,218,218,218,23,84,117,83,185,27,84,117,0,128,115,10,63,62,196,155,149,149,134,136,252,9,192,201,213,120,98,104,103,36,118,0,0,128,166,166,
166,17,133,123,1,240,196,153,18,18,17,3,240,40,128,251,0,252,17,192,253,171,87,175,94,182,96,193,130,141,197,248,245,11,39,240,29,34,34,195,66,8,135,3,24,6,96,88,225,201,95,252,164,79,123,162,93,68,150,
155,217,50,17,121,44,138,162,199,66,8,203,54,109,218,244,207,153,51,103,190,80,140,223,96,252,248,241,245,253,251,247,63,34,138,162,227,69,228,131,0,78,20,145,195,121,153,114,201,181,71,81,116,92,46,151,
75,236,205,162,18,255,13,166,170,223,49,179,203,188,59,18,168,29,192,147,0,254,33,34,43,1,188,28,69,209,234,16,194,234,40,138,242,91,255,196,16,66,175,40,138,106,67,8,251,154,217,190,0,14,16,145,125,205,
108,255,194,95,121,217,19,149,93,225,178,216,23,0,60,39,34,47,152,217,243,34,242,66,20,69,47,132,16,54,71,81,244,198,214,63,63,132,144,138,162,168,63,128,189,67,8,253,205,108,63,17,121,95,225,122,125,
14,170,101,38,34,243,85,245,91,222,29,158,248,77,7,92,14,224,92,0,67,156,59,146,166,6,111,127,82,7,0,136,8,204,12,34,219,206,165,91,254,221,150,159,183,229,223,109,253,87,162,114,43,12,158,7,0,56,96,235,
239,199,93,125,31,111,249,251,173,255,74,101,247,52,128,233,222,17,222,18,127,217,147,170,174,51,179,68,79,129,68,68,73,98,102,223,84,213,117,222,29,222,120,236,27,64,91,91,219,163,141,141,141,7,1,24,
225,221,66,68,68,165,35,34,63,104,110,110,158,231,221,17,7,137,223,1,216,66,68,38,20,30,35,74,68,68,213,233,201,181,107,215,78,240,142,136,11,14,0,5,170,250,58,128,47,22,206,88,39,34,162,42,34,34,249,
40,138,46,158,55,111,94,85,220,6,186,24,120,8,96,43,173,173,173,203,27,27,27,247,5,112,188,119,11,17,17,21,213,21,217,108,118,161,119,68,156,112,7,224,29,86,175,94,61,65,68,18,249,108,104,34,162,106,36,
34,127,94,185,114,229,68,239,142,184,225,0,240,14,133,155,211,124,178,112,127,123,34,34,170,108,175,183,183,183,95,80,137,15,125,42,53,30,2,216,142,214,214,214,53,163,70,141,122,81,68,206,241,110,33,34,
162,174,51,179,175,228,114,185,187,189,59,226,136,3,192,14,180,181,181,61,212,216,216,120,24,128,247,121,183,16,17,81,151,252,56,155,205,242,209,239,59,192,67,0,59,33,34,227,121,105,32,17,81,229,17,145,
229,27,55,110,188,212,187,35,206,56,0,236,132,170,174,1,112,142,136,196,234,177,182,68,68,180,83,235,242,249,252,185,179,103,207,126,197,59,36,206,120,8,96,23,90,91,91,95,104,108,108,124,9,192,217,222,
45,68,68,180,107,34,242,213,108,54,123,155,119,71,220,113,0,232,132,214,214,214,7,26,27,27,135,0,56,218,187,133,136,136,118,76,68,126,168,170,234,221,81,9,120,8,160,147,94,127,253,245,175,3,120,216,187,
131,136,136,182,175,112,15,151,175,123,119,84,10,14,0,157,52,127,254,252,245,249,124,254,147,0,94,247,110,33,34,162,109,137,200,107,0,62,169,170,27,188,91,42,5,7,128,221,208,210,210,242,132,153,113,186,
36,34,138,153,40,138,190,166,170,79,122,119,84,18,158,3,176,155,218,218,218,254,222,216,216,216,13,192,201,222,45,68,68,4,136,200,140,230,230,230,239,121,119,84,26,238,0,116,129,136,76,3,112,179,119,7,
17,17,225,102,0,105,239,136,74,36,222,1,149,106,210,164,73,189,26,26,26,238,3,112,164,119,11,17,81,66,45,93,191,126,253,137,115,231,206,229,179,91,186,128,59,0,93,52,119,238,220,55,242,249,252,199,0,188,
234,221,66,68,148,64,175,138,200,249,92,252,187,142,3,192,30,104,105,105,121,204,204,62,45,34,121,239,22,34,162,164,16,145,188,153,125,90,85,31,247,110,169,100,60,9,112,15,181,181,181,253,115,212,168,
81,121,17,57,213,187,133,136,40,9,204,44,157,205,102,127,232,221,81,233,184,3,80,4,33,132,89,0,126,237,221,65,68,148,0,191,46,188,231,210,30,226,73,128,69,50,113,226,196,30,61,123,246,252,147,153,241,
241,193,68,68,37,32,34,127,95,187,118,237,73,243,230,205,123,211,187,165,26,112,7,160,72,10,223,144,231,0,120,217,187,133,136,168,10,189,12,224,28,46,254,197,195,1,160,136,84,245,105,17,249,12,79,10,36,
34,42,170,246,194,73,127,79,123,135,84,19,158,4,88,100,173,173,173,79,141,26,53,106,163,136,124,200,187,133,136,168,26,136,200,228,230,230,230,159,120,119,84,27,158,3,80,26,162,170,191,50,179,179,189,
67,136,136,42,220,77,205,205,205,31,3,96,222,33,213,134,135,0,74,195,0,92,4,224,89,239,16,34,162,10,246,244,134,13,27,46,6,23,255,146,224,0,80,34,170,250,106,20,69,159,227,249,0,68,68,187,79,68,242,34,
114,225,156,57,115,94,243,110,169,86,60,7,160,132,22,47,94,252,236,232,209,163,27,192,39,7,18,17,237,174,153,205,205,205,63,242,142,168,102,220,1,40,177,53,107,214,40,128,135,189,59,136,136,42,133,136,
60,32,34,57,239,142,106,199,1,160,196,22,44,88,176,81,68,46,4,176,201,187,133,136,168,2,108,104,111,111,191,64,85,249,158,89,98,60,4,80,6,173,173,173,47,142,25,51,166,14,192,104,239,22,34,162,56,19,145,
92,54,155,253,95,239,142,36,168,241,14,72,138,215,94,123,109,214,94,123,237,245,25,0,135,120,183,80,213,90,35,34,47,152,217,106,17,89,109,102,171,1,188,44,34,47,1,120,29,0,162,40,50,236,248,17,214,125,
66,8,91,46,13,222,11,192,0,51,235,47,34,253,1,236,93,248,251,125,205,172,111,201,191,18,74,36,17,121,12,192,92,239,142,164,224,125,0,202,40,157,78,159,22,66,184,195,204,248,231,78,187,77,68,12,192,83,
102,246,136,136,252,19,29,151,153,62,13,224,233,117,235,214,61,83,174,231,162,171,234,94,249,124,126,176,136,12,9,33,12,54,179,193,34,114,176,153,29,37,34,67,249,253,77,93,81,248,254,110,84,213,197,222,
45,73,193,23,106,153,101,50,153,159,0,248,172,119,7,197,155,136,108,54,179,7,0,60,104,102,127,11,33,60,2,224,31,170,186,214,187,109,103,38,77,154,212,171,123,247,238,239,139,162,232,125,34,114,180,136,
140,0,112,140,153,213,122,183,81,188,137,200,143,84,245,98,239,142,36,225,33,128,50,107,111,111,159,84,91,91,123,174,153,245,240,110,161,88,121,90,68,238,1,240,135,124,62,127,111,42,149,122,172,185,185,
185,221,59,106,119,21,118,33,254,88,248,1,0,80,213,154,40,138,14,79,165,82,35,209,113,73,236,40,51,27,236,213,72,177,244,58,128,73,222,17,73,195,29,0,7,170,58,211,204,166,122,119,144,171,118,17,185,55,
138,162,219,204,236,214,92,46,247,48,18,114,183,51,85,13,249,124,126,132,136,156,25,66,56,195,204,78,2,63,140,36,93,182,185,185,57,227,29,145,52,28,0,28,168,234,94,0,30,55,179,125,189,91,168,124,10,39,
227,221,4,224,86,0,119,170,234,235,206,73,177,48,121,242,228,222,245,245,245,31,10,33,156,101,102,231,2,216,219,187,137,202,71,68,86,2,56,44,238,135,183,170,17,7,0,39,233,116,122,188,136,124,207,187,131,
74,238,101,0,255,43,34,139,150,44,89,114,247,162,69,139,120,107,232,157,80,213,26,0,167,154,217,88,17,57,223,204,250,123,55,81,105,153,217,215,179,217,236,247,189,59,146,136,3,128,19,85,237,110,102,203,
1,12,240,110,161,226,18,145,245,102,182,200,204,126,28,66,184,91,85,43,238,88,126,28,140,27,55,174,118,208,160,65,167,153,217,5,0,62,1,160,155,119,19,21,87,225,211,255,123,84,117,131,119,75,18,113,0,112,
164,170,147,204,108,142,119,7,21,135,136,60,4,224,170,117,235,214,253,119,185,46,201,75,138,194,213,5,159,49,179,175,1,56,218,187,135,138,195,204,190,149,205,102,231,123,119,36,21,7,0,71,147,39,79,238,
221,208,208,240,140,153,245,246,110,161,46,107,23,145,27,243,249,252,247,115,185,220,98,36,228,68,62,71,162,170,99,0,124,213,204,206,7,79,30,172,100,107,214,175,95,63,132,195,178,31,14,0,206,84,245,219,
102,54,193,187,131,118,219,235,0,174,17,145,239,170,234,179,222,49,73,164,170,67,204,236,50,0,95,6,208,211,187,135,118,143,136,204,81,213,41,222,29,73,198,1,192,153,170,30,6,224,81,222,61,173,98,60,42,
34,179,86,175,94,253,63,11,22,44,216,232,29,67,128,170,246,4,240,89,51,251,55,0,135,121,247,80,167,68,249,124,254,144,150,150,150,229,222,33,73,198,69,39,6,84,245,15,102,54,210,187,131,118,234,105,51,
107,89,181,106,213,245,11,23,46,220,236,29,67,239,166,170,117,0,190,88,184,199,198,65,222,61,180,99,34,114,183,170,158,234,221,145,116,60,126,22,15,215,0,224,0,16,67,34,242,76,20,69,51,86,173,90,117,29,
23,254,120,43,60,62,246,42,85,189,54,138,162,47,133,16,166,154,217,129,222,93,180,93,215,120,7,16,119,0,98,161,112,99,160,23,205,172,222,187,133,58,136,200,243,0,90,0,92,203,231,146,87,166,241,227,199,
215,247,237,219,247,203,33,132,105,102,182,159,119,15,189,101,157,136,236,163,170,235,188,67,146,142,3,64,76,100,50,153,223,0,248,168,119,7,225,117,51,75,135,16,190,207,133,191,58,76,152,48,161,161,87,
175,94,151,22,6,129,94,222,61,132,255,109,110,110,254,184,119,4,241,16,64,108,136,200,175,204,140,3,128,19,17,201,155,217,127,138,72,174,185,185,249,101,239,30,42,158,249,243,231,175,7,48,71,85,111,0,
144,65,199,85,3,193,183,42,185,68,228,87,222,13,212,129,3,64,76,108,218,180,233,183,117,117,117,198,171,1,202,79,68,30,202,231,243,223,200,229,114,127,242,110,161,210,81,213,21,0,190,162,170,63,1,112,
133,153,189,207,187,41,129,34,0,183,120,71,80,7,46,54,49,162,170,75,205,236,8,239,142,164,16,145,55,0,76,84,213,31,128,55,240,73,148,194,35,138,191,33,34,45,224,61,4,202,70,68,30,81,85,222,201,49,38,184,
13,22,35,102,118,175,119,67,82,136,200,109,0,222,175,170,11,193,197,63,113,84,181,61,155,205,126,55,159,207,191,31,192,157,222,61,73,193,247,184,120,225,0,16,35,34,194,45,232,18,19,145,55,68,228,107,170,
122,150,170,62,237,221,67,190,90,90,90,150,139,200,25,34,50,94,68,222,244,238,169,118,102,118,159,119,3,189,141,3,64,188,252,197,59,160,202,221,215,222,222,62,66,85,175,2,63,245,83,129,170,70,170,122,
5,128,99,0,220,239,221,83,205,66,8,127,245,110,160,183,113,0,136,151,199,1,240,209,177,197,183,201,204,190,217,220,220,252,193,150,150,150,127,122,199,80,60,169,234,227,75,151,46,61,209,204,166,136,8,
111,250,84,100,34,178,17,192,147,222,29,244,54,158,4,24,51,153,76,230,49,240,126,230,69,35,34,143,229,243,249,11,114,185,220,3,222,45,84,57,84,245,3,0,126,108,102,135,122,183,84,11,17,89,162,170,239,245,
238,160,183,113,7,32,102,68,228,9,239,134,42,114,243,250,245,235,79,224,226,79,187,75,85,255,2,224,68,0,183,123,183,84,11,51,123,204,187,129,182,197,251,0,196,140,153,61,239,221,80,233,68,36,31,69,209,
180,108,54,59,23,60,214,79,93,164,170,107,0,156,153,78,167,39,137,200,76,240,3,211,30,41,220,94,155,98,132,223,208,241,243,162,119,64,133,123,17,192,135,178,217,236,28,112,241,167,61,103,217,108,118,142,
136,156,13,224,85,239,152,74,22,69,209,75,222,13,180,45,14,0,49,99,102,188,13,109,23,21,142,247,143,82,213,86,239,22,170,46,170,122,75,20,69,39,139,8,79,34,237,162,16,2,7,128,152,225,0,16,51,33,132,87,
188,27,42,84,43,128,15,182,180,180,240,56,35,149,68,46,151,91,2,224,68,17,225,205,108,186,134,59,40,49,195,1,32,102,162,40,226,229,71,187,73,68,254,115,233,210,165,31,42,28,179,37,42,25,85,125,25,64,163,
136,92,237,221,82,129,248,116,205,152,225,73,128,49,19,66,216,108,198,67,215,157,20,153,217,183,154,155,155,191,235,29,66,201,161,170,237,0,190,154,78,167,151,137,200,183,193,15,82,157,197,1,32,102,248,
141,27,63,188,17,80,231,180,139,200,23,178,217,44,23,127,114,145,205,102,191,43,34,95,17,145,188,119,75,133,224,123,91,204,112,0,136,153,40,138,234,189,27,42,192,6,17,57,87,85,175,247,14,161,100,83,213,
107,162,40,58,31,192,6,239,150,184,227,123,91,252,112,0,136,159,110,222,1,49,183,86,68,206,82,85,62,83,156,98,33,155,205,254,198,204,62,38,34,235,189,91,226,44,132,208,224,221,64,219,226,0,16,51,124,145,
236,212,58,51,251,24,47,243,163,184,201,102,179,191,139,162,232,227,28,2,118,44,138,34,190,183,197,12,7,128,248,233,231,29,16,83,235,162,40,250,72,54,155,189,195,59,132,104,123,10,67,192,217,28,2,182,
47,132,208,223,187,129,182,197,1,32,102,204,108,127,239,134,184,17,145,55,163,40,250,112,46,151,107,243,110,33,218,153,108,54,251,123,0,31,5,207,9,120,23,190,183,197,15,7,128,152,17,145,253,188,27,98,
102,3,128,243,114,185,28,111,190,66,21,65,85,239,18,145,115,11,143,191,165,2,190,183,197,15,7,128,248,25,228,29,16,23,34,178,89,68,62,165,170,119,122,183,16,237,14,85,189,61,138,162,207,128,151,190,109,
141,239,109,49,195,1,32,102,204,140,83,50,0,17,49,0,227,84,245,215,222,45,68,93,145,205,102,127,41,34,223,240,238,136,11,190,183,197,15,7,128,24,81,213,192,109,178,14,81,20,77,85,213,235,188,59,136,246,
132,170,46,20,145,140,119,71,76,112,7,32,102,56,0,196,203,222,102,86,235,29,17,3,215,101,179,217,185,222,17,68,197,160,170,57,0,63,245,238,136,129,238,170,218,199,59,130,222,198,1,32,70,242,249,252,161,
222,13,49,240,235,165,75,151,126,25,0,31,136,64,213,194,86,174,92,121,177,136,240,18,86,224,48,239,0,122,27,7,128,24,17,145,68,191,56,68,100,201,134,13,27,46,90,180,104,17,239,173,78,85,101,225,194,133,
155,1,124,90,68,146,254,184,234,68,191,199,197,13,7,128,24,9,33,28,225,221,224,232,101,0,31,153,51,103,206,107,222,33,68,165,80,120,92,245,71,1,188,234,221,226,197,204,146,252,30,23,59,28,0,98,196,204,
14,247,110,240,32,34,121,51,251,180,170,62,235,221,66,84,74,170,250,164,136,92,8,32,242,110,113,50,204,59,128,222,198,1,32,70,68,36,145,3,64,20,69,153,194,29,212,136,170,158,170,222,44,34,51,188,59,60,
36,245,61,46,174,56,0,196,196,184,113,227,106,1,188,199,187,163,220,68,228,198,108,54,59,211,187,131,168,204,84,68,126,231,29,81,110,102,118,168,170,214,120,119,80,7,14,0,49,49,104,208,160,125,18,120,
9,224,179,0,198,129,103,252,83,194,168,106,4,224,98,17,89,233,221,82,102,117,0,246,246,142,160,14,28,0,98,34,138,162,115,189,27,202,73,68,54,71,81,244,137,194,137,81,68,137,163,170,47,230,243,249,79,137,
72,210,174,122,57,199,59,128,58,112,0,136,7,17,145,111,122,71,148,83,20,69,51,114,185,220,95,189,59,136,60,229,114,185,123,0,36,237,166,87,151,2,16,239,8,226,0,16,11,170,122,58,18,116,125,172,136,180,
133,16,114,222,29,68,49,145,6,112,159,119,68,185,152,217,240,116,58,125,170,119,7,113,0,136,139,75,189,3,202,69,68,94,217,188,121,243,133,133,99,160,68,137,167,170,237,249,124,254,115,0,94,247,110,41,
151,16,194,120,239,6,226,0,224,78,85,223,99,102,103,121,119,148,75,20,69,211,102,204,152,241,127,222,29,68,113,210,210,210,242,207,132,61,52,232,163,211,166,77,27,236,29,145,116,28,0,156,153,217,215,144,
156,255,15,119,102,179,217,171,188,35,136,98,234,123,0,254,224,29,81,14,102,150,170,169,169,249,170,119,71,210,37,101,225,137,165,194,181,255,23,121,119,148,131,136,172,207,231,243,95,7,47,249,35,218,
46,85,141,242,249,252,151,1,108,240,110,41,7,17,249,66,225,61,144,156,112,0,112,52,104,208,160,179,1,12,240,238,40,147,108,75,75,203,19,222,17,68,113,214,210,210,242,152,227,121,191,72,0,0,32,0,73,68,
65,84,153,205,246,238,40,7,51,219,119,224,192,129,31,241,238,72,50,14,0,190,46,246,14,40,147,127,0,248,182,119,4,81,37,8,33,204,73,202,83,3,69,228,98,239,134,36,227,181,152,78,84,245,32,51,91,142,42,31,
194,68,196,242,249,252,200,92,46,247,39,239,22,162,74,145,78,167,199,136,200,93,222,29,101,208,222,222,222,62,100,198,140,25,207,123,135,36,81,85,47,62,49,38,0,102,35,1,127,254,102,118,35,23,127,162,221,
147,205,102,239,6,112,179,119,71,25,212,212,212,212,204,5,63,140,186,168,250,5,40,142,210,233,244,36,51,251,140,119,71,169,137,200,155,34,50,193,187,131,168,18,137,200,101,72,198,9,129,159,85,213,127,
247,142,72,162,148,119,64,210,168,234,7,1,220,128,4,12,95,102,54,179,185,185,249,55,222,29,68,149,168,181,181,245,149,49,99,198,244,0,112,138,119,75,25,140,25,53,106,212,157,139,23,47,126,206,59,36,73,
170,126,17,138,147,73,147,38,245,2,112,29,128,170,127,28,166,136,60,191,110,221,186,121,222,29,68,149,108,253,250,245,115,0,188,232,221,81,6,53,33,132,235,84,181,167,119,72,146,112,0,40,163,134,134,134,
107,205,236,80,239,142,114,136,162,40,51,111,222,188,55,189,59,136,42,217,156,57,115,94,51,179,172,119,71,153,28,6,224,90,239,136,36,225,0,80,38,233,116,250,18,0,159,240,238,40,147,199,151,45,91,118,157,
119,4,81,53,8,33,252,0,192,211,222,29,229,96,102,99,211,233,244,151,188,59,146,130,3,64,25,168,234,209,33,132,249,222,29,229,98,102,115,23,45,90,148,180,103,156,19,149,132,170,110,50,179,196,28,78,19,
145,239,52,53,53,29,229,221,145,4,28,0,74,76,85,15,0,112,139,153,245,240,110,41,147,71,249,233,159,168,184,86,173,90,181,16,192,83,222,29,101,210,51,149,74,253,110,218,180,105,251,123,135,84,59,14,0,37,
164,170,221,205,236,87,102,182,159,119,75,185,136,72,142,159,254,137,138,107,225,194,133,155,69,100,134,119,71,185,152,217,126,181,181,181,191,82,213,238,222,45,213,140,3,64,105,93,1,224,24,239,136,114,
17,145,229,75,150,44,249,31,239,14,162,42,245,99,0,137,185,76,206,204,142,69,199,19,18,169,68,120,31,128,18,81,213,41,102,54,209,187,163,156,68,100,250,149,87,94,249,23,239,14,162,106,212,218,218,154,
31,61,122,52,68,228,12,239,150,50,58,102,204,152,49,235,91,91,91,239,245,14,169,70,220,1,40,1,85,61,15,64,139,119,71,153,173,89,187,118,237,143,188,35,136,170,217,198,141,27,175,5,240,186,119,71,57,153,
217,12,85,61,199,187,163,26,113,0,40,178,116,58,253,47,0,126,110,102,73,219,93,185,130,215,253,19,149,214,156,57,115,94,3,112,165,119,71,153,213,0,248,69,58,157,62,203,59,164,218,112,0,40,34,85,61,81,
68,126,102,102,181,222,45,229,36,34,27,69,228,251,222,29,68,73,32,34,255,41,34,155,189,59,202,201,204,106,69,228,103,77,77,77,199,123,183,84,19,14,0,69,162,170,31,52,179,219,0,36,241,86,150,63,83,213,
85,222,17,68,73,160,170,207,1,248,185,119,135,131,189,66,8,183,55,53,53,157,224,29,82,45,56,0,20,65,58,157,62,221,204,238,0,176,151,119,139,135,124,62,207,79,255,68,229,117,149,119,128,147,62,33,132,187,
84,245,67,222,33,213,128,3,192,30,74,167,211,31,23,145,155,0,36,245,122,213,165,185,92,238,207,222,17,68,73,162,170,247,138,200,19,222,29,78,186,3,184,41,157,78,159,239,29,82,233,170,254,169,116,165,162,
170,193,204,230,3,184,212,187,197,147,136,92,237,221,64,148,64,6,96,33,128,255,240,14,241,96,102,61,66,8,55,170,234,92,0,151,171,106,228,221,84,137,184,3,208,5,170,218,199,204,254,7,92,252,55,2,248,169,
119,7,81,66,253,56,105,39,3,110,205,204,196,204,38,3,248,233,228,201,147,123,123,247,84,34,14,0,187,169,112,178,223,67,72,206,147,253,118,230,38,85,125,217,59,130,40,137,10,39,222,222,236,221,225,205,
204,62,213,208,208,240,80,83,83,211,73,222,45,149,134,135,0,58,73,85,187,153,217,52,51,155,2,254,185,109,241,99,239,0,162,36,139,162,232,39,34,146,248,99,225,102,54,52,132,176,56,147,201,204,22,145,25,
170,186,193,187,169,18,112,7,160,19,210,233,244,88,51,251,39,128,233,224,226,15,0,16,145,213,43,86,172,184,205,187,131,40,201,94,121,229,149,155,69,228,53,239,142,152,168,65,199,123,244,147,170,202,29,
218,78,16,239,128,56,83,213,195,204,108,6,184,221,255,46,34,242,67,85,253,162,119,7,81,210,101,50,153,159,0,248,172,119,71,12,253,60,159,207,79,111,105,105,73,234,213,18,187,196,79,179,219,161,170,135,
0,152,105,102,31,7,119,73,118,132,39,255,17,197,128,153,253,84,68,56,0,188,219,39,83,169,212,39,84,245,70,116,92,41,240,164,119,80,220,112,0,216,202,244,233,211,143,168,169,169,153,96,102,23,2,232,230,
221,19,99,47,3,104,245,142,32,34,224,149,87,94,185,179,95,191,126,175,2,232,227,221,18,67,193,204,198,2,56,59,147,201,92,47,34,243,85,245,81,239,168,184,72,252,33,0,85,13,0,62,4,224,155,0,206,52,179,196,
255,153,116,194,53,205,205,205,151,120,71,16,81,7,85,189,190,240,193,133,118,66,68,12,192,45,0,190,3,224,174,164,223,63,32,169,59,0,162,170,35,11,47,152,177,102,214,215,59,168,146,136,200,111,189,27,136,
232,109,81,20,253,86,68,56,0,236,66,225,3,222,191,20,126,172,201,100,50,191,16,145,27,84,245,94,116,220,92,41,81,18,51,0,168,106,93,20,69,35,67,8,103,0,248,168,153,13,7,0,179,196,253,63,223,83,237,235,
215,175,191,219,59,130,136,222,22,66,248,189,153,69,224,57,75,187,163,31,128,113,102,54,78,85,151,160,227,158,10,183,174,88,177,226,222,133,11,23,38,226,6,75,85,187,221,173,170,117,249,124,126,68,42,149,
58,1,192,169,102,118,26,146,249,164,190,162,18,145,123,84,117,148,119,7,17,109,43,147,201,252,25,192,7,188,59,42,157,136,188,97,102,191,55,179,187,66,8,127,6,240,176,170,110,242,238,42,133,170,216,1,152,
52,105,82,175,250,250,250,97,34,114,100,8,97,132,153,157,0,96,68,8,161,158,159,240,139,238,86,239,0,34,218,174,91,193,1,96,143,153,89,47,0,231,137,200,121,102,6,17,217,168,170,15,154,217,159,69,228,161,
124,62,191,108,227,198,141,143,206,157,59,247,13,239,214,61,85,17,3,128,170,214,0,24,152,207,231,7,139,200,1,0,14,8,33,12,6,48,12,192,48,51,59,112,203,207,221,178,224,115,225,47,141,124,62,207,155,255,
16,197,144,136,220,102,102,105,239,142,106,99,102,245,0,78,2,112,146,153,33,132,128,134,134,6,100,50,153,103,1,60,42,34,143,69,81,244,12,128,231,204,236,185,84,42,245,12,128,85,170,218,238,26,222,9,238,
3,128,170,118,107,111,111,223,95,68,6,165,82,169,3,1,12,2,112,64,225,199,32,0,7,1,24,104,102,169,16,222,62,188,197,5,190,252,68,228,149,84,42,245,144,119,7,17,109,215,95,10,219,215,189,188,67,18,226,32,
0,7,153,217,135,69,58,142,166,139,8,10,187,6,249,194,179,26,158,5,176,2,192,115,133,31,43,242,249,252,255,165,82,169,231,87,175,94,253,252,130,5,11,54,186,213,163,12,231,0,168,106,29,128,33,81,20,13,13,
33,12,53,179,161,0,134,136,200,80,0,67,204,108,159,82,55,80,209,220,210,220,220,252,47,222,17,68,180,125,153,76,230,14,116,92,214,76,149,225,69,17,121,198,204,150,139,200,242,40,138,150,135,16,150,183,
183,183,47,175,169,169,121,166,212,231,30,20,109,7,64,85,7,0,56,18,29,91,242,71,224,237,237,249,3,0,132,45,147,209,22,252,4,95,121,68,228,94,239,6,34,218,49,17,185,215,204,56,0,84,142,1,102,54,0,192,241,
133,157,3,152,25,82,169,20,204,44,202,100,50,207,161,227,48,195,178,40,138,150,153,217,163,237,237,237,75,103,205,154,245,82,49,126,243,221,30,0,10,199,227,135,69,81,52,66,68,142,22,145,17,102,118,148,
153,245,43,70,16,197,87,62,159,231,0,64,20,99,81,20,221,187,101,59,154,42,94,192,59,14,51,136,8,234,234,234,144,201,100,214,0,120,24,192,195,34,242,80,225,239,31,221,221,243,14,118,57,0,140,31,63,190,
190,79,159,62,31,16,145,70,17,105,68,199,137,16,13,91,190,201,248,73,62,25,68,100,115,42,149,250,171,119,7,17,237,216,134,13,27,238,235,222,189,123,222,204,82,222,45,84,82,253,0,156,138,142,75,220,1,0,
34,178,190,112,41,232,98,51,251,67,8,225,79,170,186,118,103,191,200,187,70,197,194,173,113,143,139,162,232,140,194,162,127,146,153,53,148,224,11,160,202,242,151,230,230,230,19,188,35,136,104,231,10,151,
172,141,240,238,32,119,237,0,30,20,145,123,0,220,188,100,201,146,123,22,45,90,148,223,250,39,108,111,7,224,118,51,59,141,159,240,105,107,34,242,128,119,3,17,237,154,153,61,0,128,3,0,213,0,248,128,153,
125,0,192,191,29,121,228,145,183,3,56,99,235,159,176,205,109,35,167,79,159,126,48,58,182,21,136,182,17,69,209,35,222,13,68,180,107,102,246,55,239,6,138,31,17,57,125,250,244,233,67,183,254,119,219,12,0,
169,84,234,34,62,13,143,182,135,111,42,68,149,129,175,85,218,30,51,147,154,154,154,11,182,254,119,91,15,0,34,34,159,47,115,19,85,134,40,149,74,253,221,59,130,136,118,109,243,230,205,28,0,104,71,46,194,
86,231,254,189,53,0,52,53,53,189,223,204,6,187,36,81,172,137,200,51,187,58,155,148,136,226,97,246,236,217,175,136,200,243,222,29,20,63,102,118,168,170,30,177,229,159,223,26,0,82,169,212,233,62,73,20,119,
102,198,79,255,68,149,133,187,0,180,35,111,221,40,106,235,67,0,31,118,8,161,202,240,184,119,0,17,117,158,153,61,225,221,64,241,100,102,111,93,9,16,0,96,194,132,9,13,102,118,138,95,18,197,153,153,61,237,
221,64,68,157,39,34,203,189,27,40,158,68,100,244,248,241,227,235,129,194,0,208,187,119,239,17,0,186,185,86,81,108,133,16,158,241,110,32,162,206,43,60,158,150,232,93,204,172,71,159,62,125,222,15,188,125,
8,224,56,199,30,138,191,167,188,3,136,168,243,204,140,175,89,218,33,17,57,14,40,12,0,188,109,36,237,136,136,216,107,175,189,198,237,68,162,10,146,74,165,248,154,165,29,10,33,140,0,10,3,128,136,112,0,160,
29,121,121,254,252,249,235,189,35,136,168,243,84,245,117,0,175,122,119,80,60,109,249,208,31,198,142,29,155,50,179,35,118,245,31,80,98,173,242,14,32,162,46,225,107,151,182,75,68,134,3,144,112,248,225,135,
31,4,160,206,59,136,98,235,101,239,0,34,234,18,190,118,105,187,204,172,65,85,15,12,33,132,67,188,99,40,190,204,108,181,119,3,17,237,62,17,225,107,151,118,230,144,0,224,61,222,21,20,95,124,19,33,170,76,
28,222,105,103,162,40,58,56,132,16,246,247,14,161,88,227,54,34,81,101,226,107,151,118,40,132,176,127,0,176,183,119,8,197,218,26,239,0,34,218,125,102,198,215,46,237,204,222,1,192,0,239,10,138,175,40,138,
222,244,110,32,162,221,23,66,224,107,151,118,102,64,48,179,125,188,43,40,214,54,121,7,16,209,238,139,162,104,163,119,3,197,151,153,237,19,68,132,3,0,237,80,8,129,111,34,68,21,136,175,93,218,25,17,217,
39,152,89,127,239,16,138,53,238,0,16,85,38,190,118,105,135,204,172,127,16,145,30,222,33,20,107,124,19,33,170,64,60,4,64,59,35,34,61,130,153,213,123,135,80,172,113,0,32,170,64,60,4,64,187,208,45,236,250,
231,80,146,69,81,100,222,13,68,68,84,124,1,192,6,239,8,138,175,16,2,119,136,136,42,80,20,69,124,237,210,206,108,8,0,214,121,87,80,172,241,65,81,68,21,136,195,59,237,140,153,189,25,68,228,37,239,16,138,
47,126,138,32,170,76,124,237,210,206,136,200,75,193,204,56,0,208,206,112,7,128,168,50,241,181,75,59,100,102,47,113,7,128,118,133,111,34,68,21,136,135,0,104,103,68,228,165,0,62,49,138,118,34,132,192,251,
68,16,85,160,40,138,248,218,165,157,121,57,0,120,209,187,130,226,139,119,138,36,170,76,33,4,190,118,105,103,94,12,81,20,173,244,174,160,248,18,17,190,137,16,85,32,14,239,180,51,81,20,173,12,33,132,39,
188,67,40,214,246,246,14,32,162,46,225,107,151,118,40,132,240,68,0,240,164,119,8,197,26,223,68,136,42,19,95,187,180,51,79,134,37,75,150,60,47,34,188,103,52,237,72,63,239,0,34,234,18,190,118,105,187,68,
100,61,128,21,97,209,162,69,121,51,91,230,29,68,177,53,208,59,128,136,186,132,175,93,218,145,165,170,26,109,121,24,208,253,174,41,20,91,102,182,207,164,73,147,122,121,119,16,81,231,77,153,50,165,47,128,
62,222,29,20,91,247,3,29,15,3,130,136,60,228,219,66,113,214,189,123,247,193,222,13,68,212,121,181,181,181,67,188,27,40,190,162,40,122,8,40,12,0,249,124,158,3,0,237,204,16,239,0,34,234,60,17,25,226,221,
64,241,21,66,120,123,0,72,165,82,15,0,88,235,90,68,177,21,69,209,80,239,6,34,218,45,239,241,14,160,216,122,29,192,131,64,97,0,80,213,77,34,210,230,154,68,177,197,79,19,68,149,133,175,89,218,17,17,105,
83,213,118,160,48,0,20,220,229,212,67,241,119,136,119,0,17,117,158,136,28,236,221,64,177,245,214,90,255,214,0,144,207,231,239,240,105,161,184,19,145,247,121,55,16,209,110,121,191,119,0,197,211,214,107,
253,91,3,64,46,151,251,187,136,240,126,0,180,61,67,84,149,151,20,17,85,128,169,83,167,238,99,102,251,123,119,80,44,253,35,151,203,45,217,242,15,91,31,2,64,20,69,55,148,191,135,226,206,204,4,192,123,189,
59,136,104,215,234,235,235,185,99,71,219,37,34,63,217,250,159,183,25,0,242,249,252,245,34,146,47,111,18,85,8,110,41,18,85,6,190,86,233,93,68,36,191,121,243,230,109,62,228,111,51,0,204,152,49,227,121,240,
100,64,218,62,238,0,16,85,0,51,27,238,221,64,241,99,102,119,23,214,248,183,132,119,254,164,37,75,150,156,37,34,99,0,124,91,68,254,10,160,189,92,129,20,95,102,118,172,119,3,17,237,154,136,240,181,74,0,
176,78,68,126,47,34,77,34,114,130,136,156,241,206,159,32,187,250,21,84,181,103,20,69,39,133,16,78,49,179,83,68,228,4,51,107,40,77,47,197,149,136,108,6,208,71,85,215,121,183,16,209,246,77,154,52,169,87,
247,238,221,95,49,179,148,119,11,149,221,58,17,249,19,128,214,194,143,191,168,234,166,157,253,7,53,187,250,21,85,117,45,128,59,10,63,160,170,53,34,50,44,138,162,17,34,114,180,136,140,48,179,163,192,71,
79,86,53,51,171,141,162,232,120,0,188,97,20,81,76,117,235,214,237,68,46,254,213,79,68,86,155,217,35,133,231,248,60,92,248,241,232,150,27,252,116,214,46,7,128,119,42,252,6,255,40,252,184,97,171,127,63,
0,192,145,0,134,153,217,17,0,134,21,126,28,128,237,28,106,160,202,147,74,165,70,130,3,0,81,108,133,16,70,154,153,119,6,21,71,4,224,57,0,143,138,200,178,40,138,150,153,217,163,237,237,237,75,103,205,154,
245,82,49,126,131,221,30,0,118,68,85,95,4,240,34,58,182,30,182,254,247,117,0,134,68,81,52,52,132,48,212,204,134,138,200,80,51,27,10,224,32,0,3,138,213,64,165,101,102,35,189,27,136,104,199,248,26,173,44,
34,242,2,128,103,205,108,185,136,44,143,162,104,121,8,97,121,123,123,251,242,154,154,154,103,118,181,133,191,199,191,127,41,127,241,206,80,213,186,205,155,55,239,27,66,56,64,68,6,2,56,32,132,176,47,58,
118,14,6,154,217,254,34,50,216,204,248,76,122,103,34,242,10,128,189,85,53,242,110,33,162,109,169,106,13,128,53,124,175,140,133,215,1,60,11,224,185,194,34,255,28,128,85,0,158,203,231,243,47,68,81,244,92,
109,109,237,11,165,94,224,119,165,104,59,0,93,85,248,3,248,191,194,143,157,253,188,126,232,216,73,24,12,96,72,8,97,40,10,135,25,204,236,192,210,151,146,153,245,141,162,104,4,128,7,188,91,136,232,93,62,
192,197,191,172,158,69,199,246,252,99,0,158,138,162,232,25,51,123,58,149,74,61,163,170,107,188,227,58,195,125,0,232,172,194,31,232,26,20,30,99,184,181,73,147,38,245,170,175,175,31,38,34,71,134,16,70,152,
217,9,133,147,19,235,203,95,90,221,82,169,212,89,224,0,64,20,59,102,118,150,119,67,53,18,145,141,0,30,52,179,63,139,200,67,249,124,126,217,198,141,27,31,157,59,119,238,27,222,109,123,170,98,6,128,157,
41,252,143,248,107,225,199,143,128,142,67,11,81,20,141,72,165,82,39,0,56,213,204,78,3,208,211,49,179,90,156,9,160,197,59,130,136,222,229,76,239,128,106,32,34,111,152,217,239,205,236,174,16,194,159,1,60,
236,189,85,95,42,238,231,0,148,75,97,32,24,41,34,103,2,248,40,58,174,88,160,221,36,34,249,13,27,54,236,51,123,246,236,87,188,91,136,168,131,170,14,48,179,149,224,21,87,93,34,34,75,0,220,12,224,214,21,
43,86,220,187,112,225,194,205,222,77,229,80,21,59,0,157,81,152,224,238,46,252,152,172,170,195,0,124,218,204,46,0,159,119,223,105,102,150,170,171,171,59,19,192,127,123,183,16,209,91,62,2,46,254,187,69,
68,158,136,162,232,191,242,249,252,207,102,204,152,241,140,119,143,135,196,12,0,239,164,170,143,118,252,69,179,81,20,157,46,34,223,20,145,51,10,79,190,163,157,8,33,156,1,14,0,68,177,97,102,239,186,205,
43,189,155,136,24,128,91,0,124,7,192,93,217,108,54,209,87,52,113,177,219,138,170,30,105,102,19,0,124,14,64,55,239,158,184,42,92,14,56,176,90,143,139,17,85,146,9,19,38,52,244,238,221,251,5,94,1,176,99,
34,178,30,192,245,0,190,83,248,240,71,72,240,14,192,246,168,234,82,0,151,168,234,84,0,19,205,108,60,128,238,206,89,177,99,102,125,11,231,82,252,218,187,133,40,233,122,245,234,245,81,46,254,59,244,186,
136,204,223,184,113,227,130,89,179,102,173,246,142,137,27,14,0,219,161,170,47,3,152,162,170,87,2,72,3,248,34,15,13,188,203,199,192,1,128,200,157,136,124,204,187,33,134,34,17,249,1,128,22,85,125,206,59,
38,174,184,168,117,130,170,30,13,224,26,62,18,247,109,34,242,198,107,175,189,182,239,252,249,243,215,123,183,16,37,149,170,246,52,179,23,192,157,202,173,221,31,69,209,151,115,185,220,35,222,33,113,199,
179,70,59,65,85,31,94,189,122,245,72,116,92,255,190,91,79,91,170,86,102,214,171,87,175,94,31,242,238,32,74,184,51,193,197,127,139,118,0,89,17,25,201,197,191,115,120,8,160,147,22,44,88,176,17,64,147,170,
222,6,224,199,102,54,216,187,201,91,8,225,2,0,191,241,238,32,74,176,207,122,7,196,129,136,44,207,231,243,23,228,114,185,63,121,183,84,18,30,2,232,2,85,237,131,142,67,2,31,247,110,241,84,184,69,230,1,133,
115,38,136,168,140,84,117,32,58,158,36,87,235,221,226,73,68,254,103,253,250,245,95,153,51,103,206,107,222,45,149,134,59,0,93,160,170,175,2,248,68,58,157,190,76,68,190,141,132,30,74,49,179,122,17,185,24,
192,60,239,22,162,164,137,162,232,75,34,146,228,197,191,93,68,190,161,170,11,189,67,42,85,34,23,174,98,201,102,179,223,53,179,79,3,88,231,221,226,197,204,190,224,221,64,148,64,18,66,248,188,119,132,163,
181,34,50,150,139,255,158,225,0,176,135,178,217,236,34,17,25,45,34,73,189,198,244,72,85,29,229,29,65,148,36,233,116,250,84,51,59,212,187,195,131,136,188,36,34,163,85,245,38,239,150,74,199,1,160,8,84,245,
254,124,62,127,86,130,135,128,47,122,7,16,37,73,8,33,169,59,111,47,2,248,176,170,190,235,177,240,180,251,120,18,96,17,53,53,53,29,21,66,104,5,208,199,187,165,156,10,39,3,14,81,213,85,222,45,68,213,78,
85,15,0,240,84,2,79,254,123,185,240,201,127,169,119,72,181,224,14,64,17,229,114,185,71,68,228,12,0,107,189,91,202,201,204,234,1,124,211,187,131,40,9,204,236,223,18,184,248,175,137,162,232,84,46,254,197,
197,29,128,18,80,213,243,0,252,194,204,82,222,45,101,180,230,205,55,223,60,104,222,188,121,111,122,135,16,85,171,201,147,39,247,238,214,173,219,179,0,246,242,110,41,23,17,217,24,69,209,89,217,108,246,
110,239,150,106,195,29,128,18,40,156,156,146,180,79,196,253,122,246,236,249,21,239,8,162,106,86,95,95,255,117,36,104,241,7,58,46,119,228,226,95,26,73,250,132,90,86,173,173,173,127,25,51,102,204,80,0,71,
123,183,148,139,136,28,182,207,62,251,92,177,116,233,82,243,110,33,170,54,170,90,7,224,199,72,208,0,32,34,11,154,155,155,231,122,119,84,43,238,0,148,208,138,21,43,46,17,145,196,76,174,102,54,116,248,240,
225,73,61,59,153,168,212,198,1,56,192,59,162,92,68,228,183,72,222,78,106,89,241,28,128,18,155,58,117,106,255,250,250,250,63,155,217,193,222,45,101,242,236,154,53,107,14,43,60,59,129,136,138,64,85,187,
3,120,210,204,246,243,110,41,147,165,34,114,146,170,190,238,29,82,205,184,3,80,98,179,102,205,90,157,207,231,199,34,57,119,11,60,168,111,223,190,23,123,71,16,85,147,40,138,46,73,208,226,191,54,138,162,
79,115,241,47,61,238,0,148,137,170,126,210,204,254,199,187,163,28,68,100,37,128,67,84,53,41,67,15,81,201,76,154,52,169,87,67,67,195,83,0,246,246,110,41,7,51,251,68,54,155,189,209,187,35,9,184,3,80,38,
170,250,115,17,249,161,119,71,57,20,62,169,124,221,187,131,168,26,116,235,214,237,82,36,100,241,23,145,31,112,241,47,31,14,0,101,180,110,221,186,203,0,60,233,221,81,14,102,246,239,147,39,79,238,237,221,
65,84,201,166,78,157,218,63,132,240,111,222,29,101,242,56,128,111,121,71,36,9,7,128,50,154,59,119,238,27,34,114,190,136,172,247,110,41,131,1,221,186,117,83,239,8,162,74,86,87,87,215,98,102,125,189,59,
74,77,68,214,71,81,244,49,85,77,212,93,84,189,241,62,0,101,214,218,218,250,226,168,81,163,94,21,145,143,120,183,148,154,136,28,123,242,201,39,255,124,241,226,197,73,125,72,18,81,151,53,53,53,189,47,132,
176,16,9,248,160,102,102,151,229,114,185,91,188,59,146,166,234,191,177,226,40,155,205,94,137,142,27,122,84,53,51,171,173,169,169,153,229,221,65,84,137,82,169,212,156,36,220,78,92,68,110,200,102,179,87,
121,119,36,17,7,0,31,182,121,243,230,137,34,178,217,59,164,212,204,236,252,116,58,125,134,119,7,81,37,73,167,211,103,155,217,89,222,29,165,86,120,146,232,183,0,240,238,161,14,56,0,56,153,57,115,230,11,
0,110,245,238,40,7,17,153,87,184,141,41,17,237,130,170,118,11,33,252,135,119,71,57,152,217,239,84,245,101,239,142,164,226,0,224,40,138,162,171,189,27,202,228,189,0,166,123,71,16,85,2,51,107,54,179,195,
189,59,202,193,204,22,122,55,36,25,7,0,71,203,150,45,187,85,68,254,207,187,163,28,204,108,170,170,38,230,193,72,68,93,209,212,212,116,44,18,114,41,156,136,60,19,66,184,205,187,35,201,56,0,56,90,180,104,
81,30,192,245,222,29,101,82,99,102,87,143,29,59,182,234,79,106,34,234,138,113,227,198,213,166,82,169,255,2,80,227,221,82,14,102,246,35,85,141,188,59,146,140,3,128,191,107,69,36,239,29,81,38,31,56,226,
136,35,198,121,71,16,197,209,160,65,131,190,97,102,71,121,119,148,131,136,228,219,219,219,175,245,238,72,58,14,0,206,84,245,41,51,251,149,119,71,185,20,78,8,28,230,221,65,20,39,77,77,77,71,1,152,237,221,
81,46,102,118,227,140,25,51,158,241,238,72,58,14,0,49,32,34,223,245,110,40,163,238,0,126,202,171,2,136,58,76,152,48,161,33,132,240,83,51,171,247,110,41,151,40,138,190,231,221,64,28,0,98,65,85,23,3,184,
223,187,163,92,204,108,132,153,205,240,238,32,138,131,222,189,123,207,1,112,164,119,71,185,136,200,95,115,185,220,189,222,29,196,1,32,54,68,228,63,189,27,202,236,91,170,250,33,239,8,34,79,233,116,250,
108,0,255,234,221,81,102,87,120,7,80,7,14,0,241,113,187,119,64,153,5,51,251,47,85,237,231,29,66,228,65,85,7,132,16,174,54,51,241,110,41,179,164,189,215,197,22,7,128,152,88,178,100,201,11,133,219,98,38,
201,65,0,126,161,170,137,184,236,137,104,139,194,57,48,55,153,217,126,222,45,229,84,120,18,234,139,222,29,212,129,3,64,76,20,238,9,240,164,119,71,185,153,217,24,0,45,222,29,68,101,54,215,204,78,242,142,
40,55,51,123,130,215,254,199,7,7,128,120,121,212,59,192,201,164,116,58,61,214,59,130,168,28,84,245,34,51,187,204,187,195,73,82,223,227,98,137,3,64,188,36,242,197,97,102,34,34,215,170,106,98,206,132,166,
100,82,213,247,2,184,210,187,195,81,34,223,227,226,138,3,64,188,60,238,29,224,168,39,128,159,79,158,60,185,183,119,8,81,41,20,78,120,253,133,153,245,240,110,241,34,34,143,121,55,208,219,56,0,196,72,62,
159,95,230,221,224,201,204,134,55,52,52,252,102,252,248,241,137,185,33,10,37,131,170,118,7,112,107,82,158,242,183,35,73,127,143,139,27,14,0,49,146,74,165,254,233,221,224,205,204,78,233,215,175,223,85,
0,146,118,105,20,85,47,49,179,31,152,217,241,222,33,222,248,30,23,47,28,0,98,68,85,215,20,46,147,73,186,139,51,153,12,175,12,160,170,160,170,255,1,224,179,222,29,222,68,228,13,85,125,221,187,131,222,198,
1,32,102,204,108,165,119,67,28,136,200,84,85,189,216,187,131,104,79,168,234,56,51,251,55,239,142,152,88,229,29,64,219,226,0,16,63,43,188,3,226,160,112,119,180,107,211,233,244,151,188,91,136,186,66,85,
199,1,184,202,187,35,70,248,222,22,51,28,0,226,135,47,146,2,51,147,16,194,213,170,250,105,239,22,162,221,161,170,159,53,179,239,39,240,54,191,59,195,247,182,152,225,0,16,51,34,194,67,0,91,49,179,20,128,
235,11,15,77,33,138,189,194,77,173,174,7,223,95,183,193,195,155,241,195,111,208,152,137,162,136,47,146,119,48,179,90,17,249,89,83,83,211,104,239,22,162,157,73,167,211,167,133,16,126,84,24,92,105,43,252,
112,19,63,28,0,226,103,141,119,64,76,117,15,33,220,146,78,167,79,247,14,33,218,158,116,58,125,86,8,225,55,102,214,224,221,18,83,124,111,139,25,14,0,49,19,66,224,101,128,59,214,61,132,240,91,85,253,132,
119,8,209,214,84,245,34,46,254,187,180,206,59,128,182,197,1,32,102,162,40,226,0,176,19,102,86,11,224,103,233,116,250,243,222,45,68,0,160,170,95,4,112,45,183,253,119,142,239,109,241,195,1,32,126,54,121,
7,196,157,153,165,68,228,26,85,189,200,187,133,146,77,85,191,12,96,33,23,255,78,225,123,91,204,212,120,7,208,187,240,255,73,231,212,152,217,143,50,153,204,177,34,50,129,207,24,167,114,82,213,96,102,243,
205,236,82,239,150,10,194,247,182,152,225,14,64,252,212,121,7,84,152,75,1,252,124,194,132,9,60,246,74,101,161,170,221,205,236,70,116,124,239,81,231,241,189,45,102,56,0,196,76,8,161,214,187,161,210,152,
217,199,247,218,107,175,91,10,143,91,37,42,25,85,221,27,192,237,0,206,243,110,169,64,28,0,98,134,3,64,204,68,81,212,215,187,161,66,53,2,248,163,170,14,243,14,161,234,164,170,239,5,112,159,153,141,244,
110,169,80,125,188,3,104,91,28,0,98,38,132,176,143,119,67,165,50,179,195,205,236,161,194,89,217,68,69,147,78,167,47,1,112,191,153,29,236,221,82,193,6,120,7,208,182,120,82,70,252,240,69,178,103,186,153,
217,127,101,50,153,19,68,100,188,170,242,204,99,234,178,9,19,38,52,244,238,221,251,106,51,187,208,204,188,115,42,26,63,220,196,15,119,0,226,103,144,119,64,149,24,103,102,183,169,42,7,42,234,18,85,29,212,
187,119,239,59,204,236,66,239,150,42,193,247,182,152,225,0,16,63,60,134,93,60,141,102,182,68,85,207,241,14,161,202,82,120,2,229,82,30,239,47,170,35,189,3,104,91,124,84,101,140,140,31,63,190,190,127,255,
254,111,242,166,34,197,39,34,55,0,248,170,170,242,118,164,180,67,19,39,78,236,209,179,103,207,239,243,83,127,73,180,139,72,15,30,150,139,15,238,0,196,72,255,254,253,15,230,226,95,26,133,99,184,109,211,
167,79,63,212,187,133,226,73,85,135,245,232,209,227,15,92,252,75,166,166,189,189,157,39,81,198,8,79,2,140,151,15,120,7,84,185,227,106,106,106,30,80,213,41,0,174,226,221,3,9,0,198,142,29,155,26,62,124,
248,55,0,204,4,208,195,187,167,154,213,212,212,28,15,96,153,119,7,117,224,14,64,140,152,217,41,222,13,213,206,204,122,153,217,127,2,120,80,85,57,112,37,156,170,158,120,228,145,71,62,104,102,223,53,51,
46,254,165,119,178,119,0,189,141,59,0,49,34,34,35,121,169,81,121,152,217,81,0,238,205,100,50,87,174,95,191,126,250,220,185,115,223,240,110,162,242,81,213,189,204,236,63,204,236,203,224,7,161,178,225,73,
149,241,194,147,0,99,66,85,7,2,88,97,102,252,127,82,102,34,242,68,20,69,151,101,179,217,223,121,183,80,233,165,211,233,115,69,228,219,0,222,227,221,146,64,209,166,77,155,6,206,154,53,235,37,239,16,226,
14,64,156,156,199,197,223,135,153,29,42,34,183,100,50,153,214,40,138,190,149,203,229,30,242,110,162,226,107,106,106,58,41,132,240,29,240,92,27,79,161,190,190,254,92,0,215,120,135,16,7,128,216,48,179,115,
189,27,8,141,33,132,251,85,245,39,0,38,169,234,42,239,32,218,115,170,122,16,128,121,0,62,193,33,219,95,225,189,142,3,64,12,240,197,16,3,147,38,77,234,213,189,123,247,151,204,172,222,187,133,222,178,198,
204,230,174,91,183,238,138,121,243,230,189,233,29,67,187,79,85,123,2,184,212,204,254,29,124,16,77,108,136,200,250,181,107,215,238,195,215,149,63,238,0,196,64,183,110,221,62,198,197,63,118,250,137,200,
236,30,61,122,124,83,85,103,173,94,189,250,234,5,11,22,108,244,142,162,93,43,220,191,255,107,0,166,152,25,239,63,31,51,102,214,208,189,123,247,243,0,252,196,187,37,233,56,0,196,128,136,124,217,187,129,
118,104,160,153,125,183,127,255,254,19,211,233,116,110,213,170,85,215,45,92,184,112,179,119,20,189,155,170,214,69,81,244,165,16,194,52,51,219,223,187,135,118,172,240,158,199,1,192,25,15,1,56,107,106,106,
58,42,132,240,176,119,7,117,142,136,188,4,224,74,0,87,168,234,203,222,61,212,241,208,30,116,108,245,127,9,192,222,222,61,212,57,34,50,92,85,151,122,119,36,25,119,0,156,133,16,46,242,110,160,206,43,108,
41,103,0,76,200,100,50,215,136,200,119,85,245,89,239,174,36,82,213,33,102,118,89,225,90,254,158,222,61,180,123,204,236,243,0,38,123,119,36,25,119,0,28,77,158,60,185,119,67,67,195,51,102,214,219,187,133,
186,172,93,68,110,140,162,232,7,217,108,246,46,0,188,147,83,9,169,106,136,162,232,180,16,194,37,102,118,62,248,33,166,98,137,200,43,235,214,173,27,204,155,112,249,225,139,199,81,125,125,253,165,92,252,
43,94,141,153,125,74,68,62,165,170,43,1,92,15,96,161,170,62,229,29,86,77,166,79,159,126,112,77,77,205,37,0,46,18,145,253,120,199,204,202,103,102,125,187,119,239,62,30,29,207,96,32,7,220,1,112,82,56,83,
121,185,153,237,235,221,66,69,215,14,224,22,17,185,225,181,215,94,251,237,252,249,243,215,123,7,85,34,85,237,30,69,209,217,34,242,57,17,57,139,79,202,172,74,171,68,100,168,170,110,240,14,73,34,238,0,56,
233,221,187,247,151,184,248,87,173,26,0,231,152,217,57,189,123,247,126,67,85,111,2,240,179,21,43,86,220,193,43,8,118,78,85,235,0,124,24,192,167,205,236,92,17,233,9,0,252,196,95,181,6,2,248,34,58,78,172,
165,50,227,14,128,131,194,141,127,158,224,0,144,56,47,139,200,47,1,220,10,224,78,85,125,221,59,40,14,38,79,158,220,187,190,190,254,67,33,132,179,10,199,245,251,121,55,81,249,136,200,74,0,135,169,234,90,
239,150,164,225,14,128,131,134,134,134,41,92,252,19,105,111,51,187,4,192,37,0,218,85,245,222,40,138,110,51,179,91,115,185,220,195,72,200,9,132,170,26,242,249,252,8,17,57,51,132,112,134,153,157,132,142,
115,41,188,211,200,129,153,237,39,34,147,0,164,189,91,146,134,59,0,101,166,170,135,152,217,18,0,117,222,45,20,31,34,242,38,128,135,1,252,33,138,162,123,67,8,247,170,234,26,239,174,98,80,213,126,81,20,
141,12,33,140,4,112,178,153,141,0,208,221,187,139,98,101,131,136,28,161,170,79,123,135,36,9,119,0,202,79,193,197,159,222,193,204,122,0,24,9,96,164,136,0,192,230,76,38,243,0,128,7,205,236,111,33,132,71,
0,252,35,238,219,164,133,195,91,239,139,162,232,125,34,114,180,136,140,0,112,140,136,212,242,19,62,237,68,55,51,107,6,240,121,239,144,36,225,14,64,25,169,234,169,0,238,228,19,201,168,43,68,196,0,60,101,
102,143,136,200,63,1,60,11,224,105,0,79,175,91,183,238,153,114,93,79,173,170,123,229,243,249,193,34,50,36,132,48,216,204,6,139,200,193,102,118,148,136,12,229,247,55,117,133,136,88,62,159,31,147,203,229,
218,188,91,146,130,47,212,50,153,48,97,66,195,94,123,237,245,55,0,135,120,183,80,213,90,35,34,47,152,217,106,17,89,109,102,171,209,113,226,225,75,0,94,7,128,40,138,12,192,171,59,248,239,251,132,16,182,
188,39,236,5,96,128,153,245,23,145,254,232,56,127,161,191,136,236,107,102,125,75,254,149,80,82,61,46,34,71,241,178,192,242,224,33,128,50,233,221,187,119,206,204,184,248,83,41,245,51,179,126,192,182,151,
205,109,253,247,133,195,11,59,180,189,109,250,29,253,90,68,37,112,24,128,44,128,73,222,33,73,192,29,128,50,80,45,214,133,135,0,0,20,201,73,68,65,84,213,227,0,220,199,27,153,16,17,237,82,123,20,69,39,230,
114,185,7,188,67,170,93,240,14,168,118,227,199,143,175,7,176,144,139,63,17,81,167,212,132,16,22,22,110,10,69,37,196,1,160,196,250,247,239,255,31,133,203,158,136,136,168,115,142,1,48,215,59,162,218,241,
16,64,9,165,211,233,179,67,8,191,226,89,209,68,68,187,167,112,213,203,57,170,122,179,119,75,181,226,194,84,34,170,58,16,192,223,10,207,143,39,34,162,221,247,98,225,170,128,85,222,33,213,136,135,0,74,67,
208,113,220,159,139,63,17,81,215,13,0,112,21,248,97,181,36,120,98,90,9,100,50,153,28,58,238,247,78,68,68,123,102,216,152,49,99,66,107,107,235,221,222,33,213,134,3,64,145,169,234,39,69,228,123,224,196,
74,68,84,20,34,50,106,212,168,81,75,219,218,218,150,122,183,84,19,46,82,69,164,170,239,53,179,63,1,232,233,221,66,68,84,101,214,138,200,73,170,250,15,239,144,106,193,115,0,138,100,234,212,169,253,205,
236,55,224,226,79,68,84,10,61,1,252,122,234,212,169,253,189,67,170,5,7,128,34,80,213,80,87,87,119,45,128,33,222,45,68,68,213,202,204,134,214,213,213,93,171,170,92,187,138,128,127,136,69,96,102,179,0,156,
227,221,65,68,148,0,231,0,152,233,29,81,13,120,18,224,30,74,167,211,159,17,145,111,123,119,16,17,37,133,136,140,28,53,106,212,227,109,109,109,60,31,96,15,240,36,192,61,160,170,199,153,217,61,0,186,121,
183,16,17,37,137,136,172,7,48,74,85,239,247,110,169,84,28,0,186,72,85,7,152,217,95,1,28,228,221,66,68,148,80,207,110,218,180,233,184,89,179,102,189,228,29,82,137,120,14,64,23,20,158,82,245,191,224,226,
79,68,228,233,160,186,186,186,255,111,239,206,131,228,172,235,60,142,127,62,79,231,152,8,136,200,85,94,187,30,156,46,174,171,128,32,71,102,198,3,92,87,93,160,96,215,85,23,203,99,97,69,14,173,194,16,66,
166,231,219,221,129,36,154,149,213,1,149,200,82,114,45,151,162,136,174,32,171,246,12,16,45,64,174,5,18,64,4,57,18,209,140,228,24,194,76,102,250,249,238,31,105,53,72,238,116,247,247,233,126,62,175,170,
20,85,84,102,250,253,204,116,250,249,246,243,60,253,252,174,215,202,129,219,70,3,192,182,249,178,187,31,30,29,33,34,34,56,2,192,130,232,136,118,164,139,0,183,82,177,88,252,40,128,121,209,29,34,34,242,
39,135,116,119,119,235,162,192,173,164,107,0,182,130,153,189,221,221,111,135,46,250,19,17,201,154,209,52,77,15,171,84,42,247,68,135,180,11,157,2,216,66,103,157,117,214,206,238,126,13,180,243,23,17,201,
162,174,36,73,174,61,235,172,179,118,142,14,105,23,26,0,182,12,187,186,186,46,3,176,87,116,136,136,136,108,212,94,211,166,77,187,20,58,186,189,69,116,13,192,22,40,22,139,159,39,121,122,116,135,136,136,
108,214,126,189,189,189,43,170,213,234,47,162,67,178,78,83,210,102,152,217,17,238,254,51,0,147,162,91,68,68,100,243,72,142,3,120,151,153,221,22,221,146,101,58,5,176,9,51,103,206,220,197,221,47,135,118,
254,34,34,109,195,221,39,187,251,229,51,103,206,220,37,186,37,203,52,0,108,28,187,186,186,174,132,86,248,19,17,105,71,175,175,191,134,235,72,247,70,232,26,128,141,48,179,211,221,253,180,232,14,17,17,217,
102,123,247,246,246,46,175,86,171,119,68,135,100,145,38,163,13,232,235,235,123,91,161,80,248,185,187,79,141,110,17,17,145,109,71,114,12,192,161,102,118,111,116,75,214,232,20,192,95,152,49,99,198,78,133,
66,225,26,237,252,69,68,218,159,187,79,117,247,43,205,236,101,209,45,89,163,1,224,47,76,155,54,109,158,187,239,29,221,33,34,34,13,243,102,232,22,238,47,161,83,0,235,49,179,227,220,253,59,209,29,34,34,
210,120,238,254,129,114,185,252,195,232,142,172,208,0,80,103,102,123,0,120,192,221,119,143,110,17,17,145,198,35,185,108,108,108,236,45,115,231,206,29,142,110,201,2,157,2,248,179,175,106,231,47,34,210,
185,220,253,85,83,167,78,29,136,238,200,10,29,1,0,80,44,22,143,37,121,125,116,135,136,136,52,159,187,31,83,46,151,111,136,238,136,150,251,1,96,230,204,153,187,116,117,117,45,118,247,61,163,91,68,68,164,
249,72,62,59,58,58,186,255,188,121,243,158,139,110,137,148,251,91,220,78,157,58,213,180,243,15,49,1,224,87,0,30,32,185,12,192,242,52,77,135,147,36,25,78,211,180,182,254,95,76,146,100,167,52,77,39,39,73,
178,103,253,119,245,90,146,123,186,251,107,234,255,157,28,177,1,146,111,36,199,221,253,89,0,79,147,124,214,221,159,33,249,108,154,166,207,38,73,50,158,166,233,234,245,255,126,146,36,133,52,77,119,5,176,
91,146,36,187,186,251,171,72,190,197,221,223,4,189,22,183,148,187,239,57,117,234,212,34,128,207,71,183,68,202,245,17,128,250,13,127,238,116,119,221,17,177,137,72,58,128,37,0,126,1,96,17,128,187,134,135,
135,23,15,12,12,140,53,226,251,207,154,53,107,207,66,161,176,23,201,253,146,36,217,23,192,126,0,246,115,247,55,64,47,172,178,125,38,72,62,238,238,139,73,62,156,166,233,195,73,146,44,94,187,118,237,99,
231,157,119,222,179,141,120,128,211,78,59,109,234,174,187,238,186,127,154,166,7,147,60,12,192,161,36,247,117,247,92,191,62,183,192,68,154,166,7,85,42,149,251,162,67,162,228,246,9,102,102,9,128,69,238,
126,72,116,75,135,90,67,242,22,0,223,31,27,27,187,113,238,220,185,191,111,117,128,153,77,25,31,31,223,179,80,40,188,143,228,169,0,254,182,213,13,210,126,72,222,7,224,130,241,241,241,155,39,79,158,252,
172,153,173,109,117,67,253,83,73,31,170,255,121,143,187,79,107,117,67,30,144,252,57,128,35,204,44,141,110,137,144,219,1,160,88,44,158,76,242,27,209,29,157,164,254,78,127,40,77,211,139,86,175,94,253,189,
243,207,63,255,133,232,166,245,245,245,245,117,39,73,114,42,128,99,160,35,3,242,98,19,36,191,11,224,2,51,27,138,142,89,159,153,189,44,77,211,99,147,36,57,217,221,143,140,238,233,52,36,255,205,204,46,142,
238,136,144,203,1,192,204,94,225,238,143,1,120,101,116,75,135,88,5,224,18,146,23,153,217,146,232,152,205,49,179,215,2,248,52,128,79,186,251,235,162,123,36,212,147,238,126,73,173,86,187,248,220,115,207,
125,38,58,102,115,250,250,250,254,166,80,40,156,236,238,159,0,176,99,116,79,39,32,57,252,194,11,47,188,105,254,252,249,43,163,91,90,45,151,3,64,127,127,255,124,0,51,162,59,58,192,211,238,190,32,73,146,
255,50,179,145,232,152,109,209,215,215,119,96,146,36,39,1,248,24,0,221,43,60,31,214,0,184,34,77,211,133,149,74,229,151,209,49,219,194,204,118,76,211,244,83,73,146,124,193,221,95,19,221,211,238,72,206,
55,179,153,209,29,173,150,215,1,224,66,0,167,68,119,180,43,146,191,7,112,222,200,200,200,55,23,44,88,240,124,116,79,35,204,154,53,107,207,41,83,166,124,201,221,255,53,186,69,154,135,228,165,107,215,174,
61,171,81,23,240,69,51,179,29,1,156,228,238,103,3,216,45,186,167,93,145,188,208,204,78,141,238,104,181,92,14,0,102,246,122,119,127,20,58,15,188,181,86,185,251,121,73,146,12,152,217,154,232,152,102,48,
179,19,1,124,205,221,119,136,110,145,198,33,249,60,128,83,204,236,178,232,150,102,168,95,39,112,90,146,36,231,184,251,78,209,61,237,132,228,56,128,55,154,217,211,209,45,173,150,203,1,0,0,250,251,251,175,
1,240,79,209,29,237,160,126,113,223,21,227,227,227,231,156,123,238,185,79,69,247,52,155,153,189,29,192,213,90,21,178,99,60,66,242,159,243,176,30,252,57,231,156,243,215,147,39,79,158,11,224,195,250,24,
225,22,187,186,84,42,253,75,116,68,132,220,62,65,204,236,32,119,191,51,186,35,235,72,62,10,224,36,51,171,70,183,180,210,140,25,51,118,154,54,109,218,55,0,124,36,186,69,182,29,201,203,177,238,157,127,91,
94,163,178,173,138,197,226,187,147,36,185,168,126,147,33,217,8,146,94,171,213,14,172,84,42,247,68,183,68,200,237,0,0,0,102,118,173,187,159,16,221,145,81,19,0,22,172,90,181,170,156,181,143,243,181,146,
153,125,18,192,5,250,28,118,219,89,227,238,167,148,203,229,75,163,67,162,152,217,203,220,189,4,224,115,208,233,206,141,201,237,187,127,64,3,192,235,1,60,164,23,247,151,184,159,228,199,243,112,200,116,
75,212,79,9,124,79,31,25,108,15,36,127,3,224,24,61,127,215,169,127,210,229,91,0,14,136,110,201,18,146,47,0,216,207,204,158,140,110,137,146,235,91,224,86,171,213,21,61,61,61,5,0,189,209,45,89,64,178,70,
178,66,242,99,102,182,52,186,39,43,170,213,234,178,158,158,158,75,1,28,8,224,141,209,61,178,113,36,111,25,29,29,125,207,156,57,115,158,136,110,201,138,161,161,161,101,189,189,189,223,172,95,203,51,29,
57,127,227,247,71,36,203,102,118,99,116,71,164,220,63,17,206,60,243,204,29,118,216,97,135,135,0,252,85,116,75,176,223,145,252,132,153,253,79,116,72,86,153,217,20,119,31,0,112,82,116,139,108,208,55,150,
45,91,118,250,194,133,11,199,163,67,178,170,88,44,126,144,228,37,208,71,6,159,120,254,249,231,15,232,148,143,49,111,171,220,15,0,192,159,46,152,185,37,199,87,205,254,184,126,200,255,183,209,33,237,160,
88,44,126,38,73,146,175,104,21,194,204,88,235,238,167,151,203,229,139,162,67,218,129,153,189,26,192,101,238,254,238,232,150,32,169,187,191,167,92,46,255,44,58,36,90,174,79,1,252,209,224,224,224,227,221,
221,221,187,1,120,71,116,75,43,145,116,146,115,234,247,194,94,189,249,175,16,0,24,28,28,188,171,167,167,231,78,0,199,1,208,16,16,107,13,201,127,44,149,74,215,70,135,180,139,106,181,186,122,247,221,119,
191,114,143,61,246,152,76,242,8,228,236,141,32,201,175,150,74,37,173,3,131,156,253,226,55,165,126,197,236,61,0,246,137,110,105,5,146,43,1,156,104,102,223,143,110,105,87,102,54,221,221,111,4,240,242,232,
150,60,170,63,135,63,96,102,183,69,183,180,171,98,177,120,44,201,111,33,63,207,225,37,171,86,173,122,123,158,63,217,180,62,13,0,235,233,235,235,59,176,80,40,220,238,238,83,163,91,154,137,228,131,0,142,
51,179,71,162,91,218,93,95,95,223,193,133,66,225,22,119,223,57,186,37,79,72,62,87,171,213,222,219,174,247,242,207,146,217,179,103,239,59,105,210,164,239,186,251,254,209,45,205,84,191,234,255,80,51,187,
63,186,37,43,146,232,128,44,169,191,152,116,250,34,65,223,198,186,127,4,218,249,55,64,165,82,185,19,192,251,72,234,20,74,139,144,92,89,171,213,142,214,206,191,49,230,204,153,243,240,154,53,107,14,1,112,
125,116,75,147,157,174,157,255,139,233,8,192,75,177,191,191,255,122,172,91,51,190,147,164,36,251,205,236,92,0,30,29,211,105,204,236,8,0,55,105,13,129,166,27,73,211,244,168,74,165,242,243,232,144,14,68,
51,235,115,247,126,116,216,155,67,146,151,215,215,249,144,245,116,212,47,185,65,156,228,71,73,118,204,109,130,73,254,62,77,211,119,153,217,28,104,231,223,20,245,243,208,199,147,172,69,183,116,176,9,119,
63,94,59,255,166,113,51,43,187,251,251,73,62,23,29,211,40,36,7,1,124,58,186,35,139,52,0,108,64,125,165,187,99,0,116,194,234,80,15,76,76,76,188,179,82,169,12,70,135,116,58,51,187,9,64,238,214,20,111,21,
146,95,40,151,203,55,71,119,116,186,250,207,248,29,245,107,133,218,26,201,199,0,28,111,102,107,163,91,178,72,167,0,54,161,175,175,239,157,73,146,252,20,64,87,116,203,182,32,249,35,0,31,49,179,21,209,45,
57,194,254,254,254,171,161,149,38,27,138,228,85,102,166,133,153,90,104,230,204,153,187,116,117,117,93,229,238,71,71,183,108,163,53,36,143,52,179,187,163,67,178,74,247,1,216,132,161,161,161,167,123,123,
123,23,3,56,1,237,53,44,165,238,126,118,169,84,250,108,181,90,29,141,142,201,155,131,15,62,248,71,83,166,76,57,14,186,219,90,67,144,92,60,50,50,242,193,69,139,22,233,14,127,45,116,219,109,183,141,86,171,
213,43,187,187,187,95,32,249,46,180,209,107,32,201,154,187,31,91,42,149,110,141,110,201,50,13,0,155,81,173,86,23,247,246,246,78,0,104,139,187,102,145,92,77,242,227,165,82,105,97,116,75,94,45,90,180,104,
124,250,244,233,119,37,73,242,113,232,223,216,246,90,155,166,233,135,230,205,155,247,68,116,72,94,13,14,14,222,222,221,221,253,40,201,163,1,76,137,238,217,18,36,207,46,149,74,185,93,9,114,75,233,197,105,
11,84,171,213,91,123,123,123,167,1,56,34,186,101,83,234,231,236,222,107,102,58,223,31,108,104,104,232,153,158,158,30,66,11,77,109,23,119,239,175,84,42,87,71,119,228,221,224,224,224,3,189,189,189,55,212,
143,4,236,30,221,179,25,231,149,74,165,74,116,68,59,208,69,128,91,200,204,102,146,156,31,221,177,9,87,140,140,140,28,98,102,75,162,67,100,157,225,225,225,249,0,244,251,216,118,15,37,73,242,165,232,8,89,
199,204,30,194,186,139,3,175,138,110,217,24,146,165,82,169,116,78,116,71,187,104,155,115,58,89,97,102,179,0,204,201,202,194,65,36,199,0,124,206,204,116,111,235,12,42,22,139,189,73,146,252,36,43,207,151,
118,65,210,107,181,90,119,165,82,209,57,220,12,50,179,83,0,124,57,43,119,77,173,127,252,246,84,189,14,110,29,157,2,216,74,213,106,245,214,233,211,167,47,73,146,228,31,16,188,16,12,201,199,107,181,218,
251,202,229,114,174,215,180,206,178,193,193,193,39,186,187,187,247,1,240,150,232,150,118,66,242,178,114,185,252,213,232,14,217,176,106,181,122,231,145,71,30,121,115,146,36,71,1,120,69,100,11,201,213,105,
154,30,87,42,149,116,170,104,43,233,20,192,54,40,151,203,215,212,106,181,110,146,203,2,51,190,61,58,58,122,160,110,135,154,125,36,251,0,232,115,200,91,136,228,216,248,248,120,127,116,135,108,90,165,82,
185,115,116,116,244,109,136,189,133,240,147,181,90,237,240,114,185,252,163,192,134,182,165,195,146,219,193,204,118,115,247,239,0,152,222,194,135,93,225,238,39,149,203,229,235,90,248,152,178,157,204,236,
63,221,253,140,232,142,118,64,242,63,204,236,204,232,14,217,114,102,118,162,187,95,8,96,199,86,61,102,253,14,127,199,153,217,31,90,245,152,157,70,71,0,182,131,153,45,39,249,247,0,254,187,69,15,121,63,
201,195,181,243,111,63,107,215,174,157,15,96,77,116,71,27,24,1,240,197,232,8,217,58,102,118,89,154,166,135,146,252,191,22,61,228,21,0,222,175,157,255,246,209,17,128,6,49,179,147,220,253,43,104,194,93,
3,73,58,128,11,0,204,48,51,221,216,167,77,245,247,247,127,29,192,191,71,119,100,25,201,1,51,59,61,186,67,182,141,153,117,1,88,0,224,148,102,92,248,90,95,210,247,116,51,187,184,209,223,59,143,52,0,52,144,
153,253,157,187,95,5,96,191,70,125,79,146,79,165,105,250,169,114,185,124,75,163,190,167,196,152,61,123,246,254,147,38,77,122,80,159,8,216,168,180,86,171,237,55,103,206,156,71,163,67,100,251,152,217,81,
0,46,118,247,215,53,234,123,146,92,12,224,195,90,210,183,113,244,41,128,6,170,86,171,191,221,119,223,125,23,238,184,227,142,207,147,156,142,237,251,249,166,36,191,184,114,229,202,227,231,207,159,255,112,
163,26,37,206,208,208,208,242,158,158,158,131,1,236,19,221,146,81,55,84,42,149,175,69,71,200,246,171,86,171,143,245,244,244,92,68,114,18,128,195,176,125,111,54,215,186,251,57,36,79,52,179,200,11,175,59,
142,222,137,52,137,153,29,6,224,18,119,223,119,27,190,252,105,146,159,49,179,31,52,186,75,98,153,217,251,221,253,135,209,29,89,68,242,104,51,251,113,116,135,52,86,177,88,252,96,146,36,95,119,247,215,108,
237,215,146,124,184,86,171,125,66,75,64,55,135,46,2,108,18,51,91,4,224,0,146,39,3,88,190,37,95,67,114,140,100,105,213,170,85,251,104,231,223,177,110,34,249,76,116,68,6,61,13,224,127,163,35,164,241,202,
229,242,141,43,87,174,220,155,100,169,126,227,178,45,177,188,254,218,121,128,118,254,205,163,35,0,45,96,102,175,116,247,126,146,159,117,247,13,158,22,32,249,147,137,137,137,207,232,252,103,231,235,239,
239,63,31,192,231,162,59,50,102,65,169,84,250,66,116,132,52,151,153,237,3,224,107,238,190,193,197,213,234,171,248,93,72,178,164,43,252,155,79,71,0,90,192,204,254,80,42,149,206,168,213,106,7,146,188,156,
228,250,203,154,174,112,247,83,1,28,165,157,127,62,164,105,122,109,116,67,214,232,103,146,15,102,246,8,128,163,234,175,121,43,254,248,255,73,214,72,94,3,224,160,82,169,116,134,118,254,173,161,35,0,1,204,
236,181,238,126,6,201,23,70,71,71,207,159,55,111,222,115,209,77,210,82,52,179,199,220,253,13,209,33,89,64,242,209,250,59,67,201,145,179,207,62,123,215,41,83,166,204,2,240,242,90,173,246,69,189,1,106,61,
13,0,34,1,116,26,224,69,116,248,95,36,128,78,1,136,4,32,169,123,151,215,233,103,33,18,67,3,128,72,128,225,225,225,65,172,187,237,109,174,145,92,185,116,233,82,45,249,43,18,64,3,128,72,128,129,129,129,
177,250,98,38,121,247,211,133,11,23,142,111,254,175,137,72,163,105,0,16,9,146,166,233,207,162,27,50,160,26,29,32,146,87,26,0,68,130,184,251,162,232,134,12,208,207,64,36,136,6,0,145,32,43,86,172,184,123,
43,238,140,214,137,214,44,93,186,244,190,232,8,145,188,210,0,32,18,100,96,96,96,12,192,189,209,29,129,238,214,249,127,145,56,26,0,68,98,253,34,58,32,10,73,221,227,93,36,144,6,0,145,64,105,154,222,19,221,
16,40,207,71,63,68,194,105,0,16,9,148,36,201,131,209,13,81,106,181,90,110,183,93,36,11,52,0,136,4,26,25,25,89,76,210,163,59,90,141,100,173,80,40,60,28,221,33,146,103,26,0,68,2,45,88,176,224,121,119,255,
77,116,71,128,95,155,217,104,116,132,72,158,105,0,16,137,247,80,116,64,128,60,110,179,72,166,104,0,16,137,247,72,116,64,128,60,110,179,72,166,104,0,16,9,230,238,79,68,55,4,120,34,58,64,36,239,52,0,136,
196,123,50,58,160,213,210,52,125,42,186,65,36,239,52,0,136,4,75,146,36,119,59,195,60,110,179,72,214,104,0,16,137,151,187,35,0,99,99,99,26,0,68,130,49,58,64,68,192,254,254,254,53,0,186,162,67,90,100,77,
169,84,218,33,58,66,36,239,116,4,64,36,158,147,124,38,58,162,85,242,180,173,34,89,166,1,64,36,3,220,61,79,59,197,167,163,3,68,68,3,128,72,38,228,236,93,241,210,232,0,17,209,0,32,146,9,121,58,2,144,167,
109,21,201,50,13,0,34,25,224,238,185,121,87,156,179,163,29,34,153,165,1,64,36,3,146,36,201,205,78,49,77,211,220,108,171,72,150,105,0,16,201,134,220,28,1,200,211,176,35,146,101,26,0,68,50,96,98,98,34,55,
59,197,241,241,241,220,108,171,72,150,105,0,16,201,128,149,43,87,46,37,233,209,29,205,70,178,54,121,242,228,101,209,29,34,162,1,64,36,19,6,6,6,198,220,125,56,186,163,5,150,155,217,68,116,132,136,104,0,
16,201,12,146,143,71,55,52,155,187,119,252,54,138,180,11,13,0,34,217,241,235,232,128,22,200,195,54,138,180,5,13,0,34,217,209,241,59,199,60,28,229,16,105,23,26,0,68,50,34,77,211,142,223,57,230,97,27,69,
218,133,6,0,145,140,72,146,36,15,59,199,142,63,202,33,210,46,52,0,136,100,71,199,239,28,115,50,228,136,180,5,13,0,34,217,241,36,201,241,232,136,102,33,57,254,224,131,15,62,21,221,33,34,235,104,0,16,201,
8,51,155,112,247,78,222,65,62,121,221,117,215,213,162,35,68,100,29,13,0,34,25,66,114,113,116,67,179,184,123,199,110,155,72,59,210,0,32,146,33,157,188,147,236,228,225,70,164,29,105,0,16,201,16,146,15,71,
55,52,75,154,166,26,0,68,50,68,3,128,72,182,44,137,14,104,150,36,73,52,0,136,100,136,6,0,145,12,25,27,27,235,228,157,100,199,14,55,34,237,72,3,128,72,134,204,157,59,119,24,192,242,232,142,70,35,185,204,
204,86,68,119,136,200,159,105,0,16,201,24,146,247,68,55,52,193,189,209,1,34,242,98,26,0,68,50,198,221,59,110,0,232,196,109,18,105,119,26,0,68,50,166,19,143,0,116,226,54,137,180,59,13,0,34,217,115,119,
116,64,19,232,20,128,72,198,104,0,16,201,158,95,145,92,29,29,209,64,171,144,131,133,142,68,218,141,6,0,145,140,49,179,212,221,111,143,238,104,160,219,204,44,141,142,16,145,23,211,0,32,146,65,36,111,141,
110,104,148,78,218,22,145,78,162,1,64,36,155,134,162,3,26,168,147,182,69,164,99,104,0,16,201,160,225,225,225,59,1,140,70,119,52,192,26,0,119,69,71,136,200,75,105,0,16,201,160,129,129,129,49,0,119,68,119,
52,192,29,102,182,54,58,66,68,94,74,3,128,72,118,221,22,29,208,0,157,176,13,34,29,73,3,128,72,70,165,105,250,131,232,134,237,149,166,233,247,163,27,68,100,195,52,0,136,100,212,146,37,75,238,32,57,72,210,
163,91,182,22,201,26,201,27,10,133,194,47,163,91,68,100,195,24,29,32,34,155,102,102,175,78,211,244,132,36,73,78,112,247,195,163,123,54,133,228,237,0,46,3,112,173,86,255,19,201,54,13,0,34,109,100,246,236,
217,251,79,154,52,233,104,119,63,154,100,183,187,79,11,78,90,3,96,136,228,205,0,110,50,179,37,193,61,34,178,133,52,0,136,180,41,51,235,2,112,80,154,166,111,77,146,228,173,238,254,86,0,111,6,176,99,147,
30,114,4,192,67,0,238,117,247,251,147,36,185,15,192,93,102,214,9,31,87,20,201,29,13,0,34,29,102,214,172,89,175,154,50,101,202,222,0,246,2,240,106,0,187,173,247,103,15,0,187,185,251,206,0,94,94,255,146,
85,36,87,2,88,14,224,119,245,255,254,241,207,210,90,173,246,104,161,80,120,212,204,126,219,226,77,17,145,38,250,127,37,201,133,241,87,2,18,11,0,0,0,0,73,69,78,68,174,66,96,130,0,0};

const char* VASTPresetEditorComponent::settings_png = (const char*) resource_VASTPresetEditorComponent_settings_png;
const int VASTPresetEditorComponent::settings_pngSize = 21804;

// JUCER_RESOURCE: arrow_right_corner_svg, 1715, "../../Resources/Icons/arrow_right_corner.svg"
static const unsigned char resource_VASTPresetEditorComponent_arrow_right_corner_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,
84,70,45,56,34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,32,119,105,100,116,104,61,34,50,48,112,120,34,32,104,101,105,103,104,116,61,34,50,48,112,120,34,32,118,
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

const char* VASTPresetEditorComponent::arrow_right_corner_svg = (const char*) resource_VASTPresetEditorComponent_arrow_right_corner_svg;
const int VASTPresetEditorComponent::arrow_right_corner_svgSize = 1715;


//[EndFile] You can add extra defines here...
//[/EndFile]

