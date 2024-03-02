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
#include "../../Engine/VASTMSEGData.h"
#include "VASTStepSeqEditor.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class VASTStepSeqEditorPane  : public Component,
                               public juce::ComboBox::Listener,
                               public juce::Button::Listener,
                               public juce::Slider::Listener
{
public:
    //==============================================================================
    VASTStepSeqEditorPane (AudioProcessorEditor *editor, AudioProcessor* processor, VASTMSEGData* data, VASTMSEGData* datalive, String parameterSuffix, int stepSeqNo);
    ~VASTStepSeqEditorPane() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	void updateAll();
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
    VASTMSEGData * myData;
	VASTMSEGData * myDataLive;
	String mySuffix = "";
    int myStepSeqNo = 0;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<VASTStepSeqEditor> c_stepseqEditor;
    std::unique_ptr<VASTComboBox> c_loadEnv;
    std::unique_ptr<juce::Label> label3;
    std::unique_ptr<juce::Label> label4;
    std::unique_ptr<juce::Label> label5;
    std::unique_ptr<juce::Label> label108;
    std::unique_ptr<juce::Label> label2;
    std::unique_ptr<juce::Label> label7;
    std::unique_ptr<VASTParameterComboBox> m_uStepSeqTimeBeats;
    std::unique_ptr<VASTParameterButton> m_bStepSeqSynch;
    std::unique_ptr<VASTParameterSlider> m_fStepSeqSpeed;
    std::unique_ptr<VASTParameterSlider> m_fGate;
    std::unique_ptr<VASTParameterSlider> m_fGlide;
    std::unique_ptr<VASTSlider> c_numSteps;
    std::unique_ptr<juce::Label> label6;
    std::unique_ptr<VASTButton> c_StepSeqInvert;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VASTStepSeqEditorPane)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

