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

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "../Engine/VASTEngineHeader.h"
#include "VASTControls/VASTComboBox.h"
#include "VASTControls/VASTConcertinaPanel.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class VASTPresetEditorComponent : public Component,
    public TextEditor::Listener,
    public juce::Button::Listener,
    public juce::ComboBox::Listener,
    public juce::Slider::Listener
{
public:
    //==============================================================================
    VASTPresetEditorComponent(AudioProcessorEditor* editor, AudioProcessor* processor);
    ~VASTPresetEditorComponent() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.

    void textEditorTextChanged(TextEditor&) override;
    void updateAll();
    void lookAndFeelChanged() override;
    void startAutoUpdate() {
        if (isShowing()) grabKeyboardFocus();
    };
    void stopAutoUpdate() {};

    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;
    void comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged) override;
    void sliderValueChanged (juce::Slider* sliderThatWasMoved) override;

    // Binary resources:
    static const char* settings_png;
    static const int settings_pngSize;
    static const char* arrow_right_corner_svg;
    static const int arrow_right_corner_svgSize;

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	VASTAudioProcessorEditor * myEditor = nullptr;
	VASTAudioProcessor* myProcessor = nullptr;
#ifdef VASTLOG    
    std::unique_ptr<CodeEditorComponent> logEditorComponent;
#endif

    std::unique_ptr<FileChooser> myChooser;

	friend class VASTWaveTableEditorComponent;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::GroupComponent> groupComponent2;
    std::unique_ptr<juce::GroupComponent> groupComponent;
    std::unique_ptr<juce::TextEditor> c_presetAuthor;
    std::unique_ptr<juce::Label> label4;
    std::unique_ptr<juce::TextEditor> c_presetName;
    std::unique_ptr<juce::TextEditor> c_presetTag;
    std::unique_ptr<juce::Label> label3;
    std::unique_ptr<juce::Label> label2;
    std::unique_ptr<juce::Label> label13;
    std::unique_ptr<juce::TextEditor> c_presetFolder;
    std::unique_ptr<juce::Label> label7;
    std::unique_ptr<juce::TextButton> c_folderSelect;
    std::unique_ptr<juce::TextEditor> c_pluginWidth;
    std::unique_ptr<juce::Label> label8;
    std::unique_ptr<juce::TextEditor> c_pluginHeight;
    std::unique_ptr<juce::Label> label9;
    std::unique_ptr<juce::TextButton> c_currentSize;
    std::unique_ptr<juce::TextButton> c_defaultSize;
    std::unique_ptr<juce::TextEditor> c_wavetableFolder;
    std::unique_ptr<juce::Label> label5;
    std::unique_ptr<juce::TextButton> c_folderWTSelect;
    std::unique_ptr<juce::TextEditor> c_wavFolder;
    std::unique_ptr<juce::Label> label6;
    std::unique_ptr<juce::TextButton> c_folderWavSelect;
    std::unique_ptr<juce::TextEditor> c_presetComments;
    std::unique_ptr<juce::Label> label10;
    std::unique_ptr<VASTComboBox> c_presetCategoryCombo;
    std::unique_ptr<juce::Label> label11;
    std::unique_ptr<VASTComboBox> c_uiThemeCombo;
    std::unique_ptr<juce::Label> label12;
    std::unique_ptr<VASTComboBox> c_WTmode;
    std::unique_ptr<VASTDrawableButton> c_iconMaximizeEditor;
    std::unique_ptr<juce::Label> label14;
    std::unique_ptr<juce::ToggleButton> c_disableGFX;
    std::unique_ptr<juce::Label> label15;
    std::unique_ptr<juce::Label> label16;
    std::unique_ptr<VASTComboBox> c_uiFontSize;
    std::unique_ptr<juce::Label> label17;
    std::unique_ptr<juce::ToggleButton> c_presetMPE;
    std::unique_ptr<VASTComboBox> c_MPEmode;
    std::unique_ptr<VASTSlider> c_presetBendRange;
    std::unique_ptr<juce::Label> label18;
    std::unique_ptr<juce::TextEditor> c_tuning;
    std::unique_ptr<juce::Label> label19;
    std::unique_ptr<juce::TextButton> c_tuningSelect;
    std::unique_ptr<juce::TextButton> c_tuningRemove;
    std::unique_ptr<VASTComboBox> c_permaLink;
    std::unique_ptr<juce::Label> label20;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VASTPresetEditorComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

