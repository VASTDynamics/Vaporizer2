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

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "../../Engine/VASTEngineHeader.h"
class VASTPresetComponent; //forward deklaration
class VASTAudioProcessor; //forward declaration
class VASTAudioProcessorEditor; //forward declaration
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class VASTManualRenameEditor  : public Component,
                                public TextEditor::Listener,
                                public juce::Button::Listener,
                                public juce::ComboBox::Listener
{
public:
    //==============================================================================
    VASTManualRenameEditor (VASTAudioProcessor* processor, String internalid, VASTPresetComponent* presetComponent);
    ~VASTManualRenameEditor() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	void textEditorReturnKeyPressed(TextEditor& textEditorThatWasChanged) override;
	void textEditorEscapeKeyPressed(TextEditor& textEditorThatWasChanged) override;
    void setName(String text);
    void setCategory(String text);
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;
    void comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	VASTAudioProcessor* myProcessor = nullptr;
	String mInternalid;
	VASTPresetComponent* mPresetComponent;


    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::TextEditor> c_textEditor;
    std::unique_ptr<juce::TextButton> c_OK;
    std::unique_ptr<juce::TextButton> c_Cancel;
    std::unique_ptr<VASTComboBox> c_presetCategoryCombo;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VASTManualRenameEditor)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

