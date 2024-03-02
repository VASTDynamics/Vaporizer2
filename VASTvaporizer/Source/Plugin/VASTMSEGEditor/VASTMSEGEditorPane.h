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
#include "VASTMSEGEditor.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class VASTMSEGEditorPane  : public Component,
                            public juce::ComboBox::Listener,
                            public juce::Slider::Listener,
                            public juce::Button::Listener
{
public:
    //==============================================================================
    VASTMSEGEditorPane (AudioProcessorEditor *editor, AudioProcessor* processor, VASTMSEGData* data, VASTMSEGData* datalive, String parameterSuffix, int msegNo);
    ~VASTMSEGEditorPane() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	void initAll() {
		c_loadEnv->setSelectedItemIndex(0, NotificationType::dontSendNotification); //dont send change
	};
	void startAutoUpdate();
	void stopAutoUpdate();
	void updateAll();
    void synchToggled();
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged) override;
    void sliderValueChanged (juce::Slider* sliderThatWasMoved) override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	VASTAudioProcessorEditor * myEditor;
	VASTAudioProcessor* myProcessor;
    VASTMSEGData * myData;
	VASTMSEGData * myDataLive;
	int myMSEGNo = -1;
	String mySuffix = "";

    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<VASTMSEGEditor> c_msegEditor;
    std::unique_ptr<VASTComboBox> c_loadEnv;
    std::unique_ptr<juce::Label> label3;
    std::unique_ptr<juce::Label> label4;
    std::unique_ptr<juce::Label> label5;
    std::unique_ptr<juce::Label> label6;
    std::unique_ptr<VASTParameterComboBox> m_uMSEGPolarity;
    std::unique_ptr<VASTParameterSlider> m_fAttackTime;
    std::unique_ptr<VASTParameterSlider> m_fDecayTime;
    std::unique_ptr<VASTParameterSlider> m_fSustainLevel;
    std::unique_ptr<VASTParameterSlider> m_fReleaseTime;
    std::unique_ptr<juce::Label> label108;
    std::unique_ptr<juce::Label> label2;
    std::unique_ptr<VASTParameterComboBox> m_uMSEGTimeBeats;
    std::unique_ptr<VASTParameterButton> m_bMSEGSynch;
    std::unique_ptr<juce::Label> label7;
    std::unique_ptr<VASTParameterButton> m_bMSEGPerVoice;
    std::unique_ptr<VASTParameterSlider> m_fAttackSteps;
    std::unique_ptr<VASTParameterSlider> m_fDecaySteps;
    std::unique_ptr<VASTParameterSlider> m_fReleaseSteps;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VASTMSEGEditorPane)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

