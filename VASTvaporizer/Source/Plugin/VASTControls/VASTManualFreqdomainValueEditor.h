/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 6.0.0

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "../../Engine/VASTEngineHeader.h"
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
class VASTManualFreqdomainValueEditor  : public Component,
                                         public TextEditor::Listener,
                                         public juce::Button::Listener
{
public:
    //==============================================================================
    VASTManualFreqdomainValueEditor (VASTAudioProcessor* processor, VASTWaveTableEditorComponent* wteditor, int bin);
    ~VASTManualFreqdomainValueEditor() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	void textEditorReturnKeyPressed(TextEditor& textEditorThatWasChanged) override;
	void textEditorEscapeKeyPressed(TextEditor& textEditorThatWasChanged) override;
    void setTextMagnitude(String text);
    void setTextPhase(String text);

    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	VASTAudioProcessor* myProcessor = nullptr;
	VASTWaveTableEditorComponent* myWtEditor = nullptr;
	int mBin = -1;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::TextEditor> c_textEditorMagnitude;
    std::unique_ptr<juce::TextButton> c_OK;
    std::unique_ptr<juce::TextButton> c_Cancel;
    std::unique_ptr<juce::TextEditor> c_textEditorPhase;
    std::unique_ptr<juce::Label> label;
    std::unique_ptr<juce::Label> label2;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VASTManualFreqdomainValueEditor)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

