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
#include "VASTControls/VASTParameterComboBox.h"
#include "VASTControls/VASTParameterSlider.h"
#include "VASTControls/VASTParameterButton.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class VASTOscillatorComponent  : public Component,
                                 public juce::Slider::Listener,
                                 public juce::ComboBox::Listener,
                                 public juce::Button::Listener
{
public:
    //==============================================================================
    VASTOscillatorComponent (AudioProcessorEditor *editor, AudioProcessor* processor);
    ~VASTOscillatorComponent() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	void assignComponentIDsForChildren(const String &compID);
	void updateAll();
	void initAll();
	VASTOscilloscope* getAudioOscilloscope() { return c_audioOscilloscope.get(); };
    void lookAndFeelChanged() override;
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void sliderValueChanged (juce::Slider* sliderThatWasMoved) override;
    void comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged) override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	VASTAudioProcessorEditor * myEditor = nullptr;
	VASTAudioProcessor* myProcessor = nullptr;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<VASTOscilloscope> c_audioOscilloscope;
    std::unique_ptr<VASTParameterSlider> m_fOscDetune;
    std::unique_ptr<VASTParameterSlider> m_fOscGain;
    std::unique_ptr<VASTParameterSlider> m_fOscCents;
    std::unique_ptr<VASTParameterSlider> m_fOscWTPos;
    std::unique_ptr<VASTParameterSlider> m_fOscMorph;
    std::unique_ptr<VASTParameterSlider> m_fSpread;
    std::unique_ptr<juce::Label> headerLabel;
    std::unique_ptr<juce::Label> label18;
    std::unique_ptr<juce::Label> label108;
    std::unique_ptr<juce::Label> label134;
    std::unique_ptr<juce::Label> label135;
    std::unique_ptr<juce::Label> label2;
    std::unique_ptr<juce::Label> label3;
    std::unique_ptr<VASTParameterSlider> m_fOscPan;
    std::unique_ptr<VASTParameterSlider> m_fOscPhase;
    std::unique_ptr<juce::Label> label4;
    std::unique_ptr<juce::Label> label5;
    std::unique_ptr<juce::Label> label6;
    std::unique_ptr<juce::Label> label7;
    std::unique_ptr<juce::Label> label8;
    std::unique_ptr<juce::Label> label9;
    std::unique_ptr<juce::Label> label13;
    std::unique_ptr<juce::Label> label14;
    std::unique_ptr<juce::Label> label15;
    std::unique_ptr<juce::Label> label12;
    std::unique_ptr<VASTParameterComboBox> m_uOscRouting2;
    std::unique_ptr<VASTParameterComboBox> m_uOscRouting1;
    std::unique_ptr<VASTParameterSlider> m_iOscOct;
    std::unique_ptr<VASTParameterSlider> m_iNumOscs;
    std::unique_ptr<VASTParameterButton> m_bOscOnOff;
    std::unique_ptr<VASTParameterComboBox> m_uVCAEnv;
    std::unique_ptr<VASTParameterButton> m_bOscInverterOnOff;
    std::unique_ptr<VASTParameterButton> m_bExciterOnOff;
    std::unique_ptr<VASTParameterButton> m_bOscRetrigOnOff;
    std::unique_ptr<juce::Label> label10;
    std::unique_ptr<VASTParameterComboBox> m_uWTFX;
    std::unique_ptr<VASTParameterSlider> m_fWTFXVal;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VASTOscillatorComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

