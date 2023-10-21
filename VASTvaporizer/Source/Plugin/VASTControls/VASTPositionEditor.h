/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 6.1.2

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "../../Engine/VASTEngineHeader.h"
class VASTAudioProcessor; //forward decklaration
class VASTAudioProcessorEditor; //forward decklaration
class VASTMSEGEditor; //forward decklaration
class VASTStepSeqEditor; //forward decklaration
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class VASTPositionEditor  : public Component,
                            public TextEditor::Listener,
                            public juce::Button::Listener
{
public:
    //==============================================================================
    VASTPositionEditor (VASTAudioProcessor* processor, float value, VASTMSEGEditor* msegeditor, bool isX, int numIsClicked);
    ~VASTPositionEditor() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	VASTPositionEditor(VASTAudioProcessor* processor, float value, VASTStepSeqEditor* stepseqeditor, bool isX, int numIsClicked);
	void textEditorReturnKeyPressed(TextEditor& textEditorThatWasChanged) override;
	void textEditorEscapeKeyPressed(TextEditor& textEditorThatWasChanged) override;
    void setText(String text);
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	VASTAudioProcessor* myProcessor = nullptr;
	float myValue = 0.0f;
	VASTMSEGEditor* myMSEGEditor = nullptr;
	VASTStepSeqEditor* myStepSeqEditor = nullptr;
	bool myIsX = false;
	int myNumIsClicked = -1;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::TextEditor> c_textEditor;
    std::unique_ptr<juce::TextButton> c_OK;
    std::unique_ptr<juce::TextButton> c_Cancel;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VASTPositionEditor)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

