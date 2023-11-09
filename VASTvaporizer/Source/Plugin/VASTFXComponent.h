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
#include "../Engine/VASTEngineHeader.h"
#include "VASTControls/VASTComboBrev.h"
#include "VASTControls/VASTComboBox.h"
#include "VASTControls/VASTSlider.h"
#include "VASTControls/VASTComboPreset.h"
#include "VASTControls/VASTImageButton.h"
#include "VASTFX/VASTGenericEditor.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class VASTFXComponent  : public Component,
                         public Button::Listener
{
public:
    //==============================================================================
    VASTFXComponent (AudioProcessorEditor *editor, AudioProcessor* processor);
    ~VASTFXComponent() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	void updateAll();
	void lookAndFeelChanged() override;
	void startAutoUpdate() {
        if (isShowing()) grabKeyboardFocus();
    };
	void stopAutoUpdate() {};
	void buttonClicked(Button* buttonThatWasClicked) override;
	void mouseWheelMove(const MouseEvent &event, const MouseWheelDetails &wheel) override;
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;

    // Binary resources:
    static const char* arrow_right_corner_svg;
    static const int arrow_right_corner_svgSize;


private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	VASTAudioProcessorEditor * myEditor = nullptr;
	VASTAudioProcessor* myProcessor = nullptr;
	juce::Time lastMouseWheelEvent = juce::Time::getCurrentTime();
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::TabbedComponent> c_fxBusTab;
    std::unique_ptr<VASTDrawableButton> c_iconMaximizeEditor;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VASTFXComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

