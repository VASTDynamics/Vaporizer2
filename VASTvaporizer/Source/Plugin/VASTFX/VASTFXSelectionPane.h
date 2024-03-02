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

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "../../Engine/VASTEngineHeader.h"
#include "../VASTFX/VASTGenericSelector.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class VASTFXSelectionPane  : public Component,
                             public juce::Slider::Listener,
                             public juce::ComboBox::Listener
{
public:
    //==============================================================================
    VASTFXSelectionPane (AudioProcessorEditor *editor, AudioProcessor* processor, int busnr, VASTGUINotifier* guinotifier);
    ~VASTFXSelectionPane() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	void updateContent();
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void sliderValueChanged (juce::Slider* sliderThatWasMoved) override;
    void comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	VASTAudioProcessorEditor * myEditor = nullptr;
	VASTAudioProcessor* myProcessor = nullptr;
    int myBusnr = 0;
	VASTGUINotifier* myGUINotifier = nullptr;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::Label> label125;
    std::unique_ptr<VASTParameterSlider> m_fMBMonoFrequency;
    std::unique_ptr<VASTGenericSelector> c_selector;
    std::unique_ptr<VASTParameterComboBox> m_uFxBusRouting;
    std::unique_ptr<juce::Label> label14;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VASTFXSelectionPane)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

