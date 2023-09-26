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
#include "VASTARPEditor.h"
#include "../../Engine/VASTARPData.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class VASTARPEditorPane  : public Component,
                           public juce::ComboBox::Listener,
                           public juce::Button::Listener,
                           public juce::Slider::Listener
{
public:
    //==============================================================================
    VASTARPEditorPane (AudioProcessorEditor *editor, AudioProcessor* processor, VASTARPData* data, VASTARPData* datalive);
    ~VASTARPEditorPane() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	VASTAudioProcessorEditor * myEditor;
	VASTAudioProcessor* myProcessor;
    VASTARPData * myData;
	VASTARPData * myDataLive;
	void initAll();
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
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::Label> label122;
    std::unique_ptr<juce::Label> label4;
    std::unique_ptr<VASTARPEditor> c_arpEditor;
    std::unique_ptr<juce::Label> label3;
    std::unique_ptr<juce::Label> label108;
    std::unique_ptr<juce::Label> label2;
    std::unique_ptr<juce::Label> label7;
    std::unique_ptr<VASTParameterComboBox> m_uARPTimeBeats;
    std::unique_ptr<VASTComboBox> c_ARPPattern;
    std::unique_ptr<VASTParameterButton> m_bARPOnOff;
    std::unique_ptr<VASTParameterComboBox> m_uARPMode;
    std::unique_ptr<VASTSlider> c_numSteps;
    std::unique_ptr<VASTParameterSlider> m_fARPSpeed;
    std::unique_ptr<VASTParameterButton> m_bARPSynch;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VASTARPEditorPane)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

