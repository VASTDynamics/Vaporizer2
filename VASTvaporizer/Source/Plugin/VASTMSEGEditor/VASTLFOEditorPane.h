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
#include "VASTLFOEditor.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class VASTLFOEditorPane  : public Component,
                           public juce::ComboBox::Listener,
                           public juce::Button::Listener,
                           public juce::Slider::Listener
{
public:
    //==============================================================================
    VASTLFOEditorPane (AudioProcessorEditor *editor, AudioProcessor* processor, String parameterSuffix, int lfoNo);
    ~VASTLFOEditorPane() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	void startAutoUpdate();
	void stopAutoUpdate();

    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged) override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;
    void sliderValueChanged (juce::Slider* sliderThatWasMoved) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	VASTAudioProcessorEditor * myEditor;
	VASTAudioProcessor* myProcessor;
	String mySuffix = "";
    int myLFONo = 0;

    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<VASTLFOEditor> c_lfoEditor;
    std::unique_ptr<juce::Label> label108;
    std::unique_ptr<juce::Label> label2;
    std::unique_ptr<juce::Label> label3;
    std::unique_ptr<juce::Label> label4;
    std::unique_ptr<VASTParameterComboBox> m_uLFOWave;
    std::unique_ptr<juce::Label> label6;
    std::unique_ptr<VASTParameterComboBox> m_uLFOTimeBeats;
    std::unique_ptr<VASTParameterButton> m_bLFOSynch;
    std::unique_ptr<VASTParameterButton> m_bLFORetrigOnOff;
    std::unique_ptr<juce::Label> label5;
    std::unique_ptr<VASTParameterButton> m_bLFOPerVoice;
    std::unique_ptr<VASTParameterComboBox> m_uLFOPolarity;
    std::unique_ptr<juce::Label> label7;
    std::unique_ptr<VASTParameterSlider> m_fLFORamp;
    std::unique_ptr<VASTParameterSlider> m_fLFOPhase;
    std::unique_ptr<VASTParameterSlider> m_fLFOFreq;
    std::unique_ptr<VASTParameterComboBox> m_uLFOMSEG;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VASTLFOEditorPane)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

