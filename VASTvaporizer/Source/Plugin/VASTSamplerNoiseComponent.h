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
class VASTSamplerNoiseComponent  : public Component,
                                   public juce::Slider::Listener,
                                   public juce::Button::Listener,
                                   public juce::ComboBox::Listener
{
public:
    //==============================================================================
    VASTSamplerNoiseComponent (AudioProcessorEditor *editor, AudioProcessor* processor);
    ~VASTSamplerNoiseComponent() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void sliderValueChanged (juce::Slider* sliderThatWasMoved) override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;
    void comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	VASTAudioProcessorEditor * myEditor = nullptr;
	VASTAudioProcessor* myProcessor = nullptr;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<VASTParameterSlider> m_uSamplerRootKey;
    std::unique_ptr<VASTParameterSlider> m_fNoiseGain;
    std::unique_ptr<juce::Label> label4;
    std::unique_ptr<juce::Label> label2;
    std::unique_ptr<VASTParameterButton> m_bSamplerOnOff;
    std::unique_ptr<juce::Label> label12;
    std::unique_ptr<juce::Label> label7;
    std::unique_ptr<juce::Label> label13;
    std::unique_ptr<juce::Label> label14;
    std::unique_ptr<juce::Label> label15;
    std::unique_ptr<juce::Label> label16;
    std::unique_ptr<juce::Label> label17;
    std::unique_ptr<juce::Label> label5;
    std::unique_ptr<VASTParameterSlider> m_fNoisePan;
    std::unique_ptr<juce::Label> label6;
    std::unique_ptr<VASTParameterSlider> m_fNoiseLowCut;
    std::unique_ptr<juce::Label> label8;
    std::unique_ptr<juce::Label> label9;
    std::unique_ptr<juce::Label> label10;
    std::unique_ptr<juce::Label> label11;
    std::unique_ptr<juce::Label> label18;
    std::unique_ptr<juce::Label> label19;
    std::unique_ptr<juce::Label> label20;
    std::unique_ptr<VASTParameterComboBox> m_uSamplerRouting2;
    std::unique_ptr<VASTParameterComboBox> m_uSamplerRouting1;
    std::unique_ptr<VASTParameterButton> m_bNoiseOnOff;
    std::unique_ptr<VASTParameterComboBox> m_uNoiseRouting1;
    std::unique_ptr<VASTParameterSlider> m_fSamplerGain;
    std::unique_ptr<VASTParameterSlider> m_fSamplerPan;
    std::unique_ptr<VASTParameterButton> m_bSamplerPoly;
    std::unique_ptr<VASTParameterButton> m_bSamplerKeytrack;
    std::unique_ptr<VASTParameterButton> m_bSamplerLoop;
    std::unique_ptr<VASTParameterComboBox> m_uVCAEnv_Sampler;
    std::unique_ptr<VASTParameterComboBox> m_uVCAEnv_Noise;
    std::unique_ptr<VASTParameterComboBox> m_uNoiseType;
    std::unique_ptr<VASTParameterComboBox> m_uNoiseRouting2;
    std::unique_ptr<juce::Label> label3;
    std::unique_ptr<VASTParameterSlider> m_fSamplerCents;
    std::unique_ptr<juce::Label> label21;
    std::unique_ptr<juce::Label> label22;
    std::unique_ptr<VASTParameterButton> m_bSamplerLoopOnly;
    std::unique_ptr<juce::Label> label23;
    std::unique_ptr<VASTParameterButton> m_bSamplerLoopPingpong;
    std::unique_ptr<VASTParameterSlider> m_fSamplerLoopRange;
    std::unique_ptr<juce::Label> label25;
    std::unique_ptr<VASTParameterSlider> m_fNoiseResonance;
    std::unique_ptr<juce::Label> label26;
    std::unique_ptr<VASTParameterSlider> m_fNoiseHighCut;
    std::unique_ptr<juce::Label> label24;
    std::unique_ptr<VASTParameterSlider> m_fSamplerGrainInteronset;
    std::unique_ptr<juce::Label> label27;
    std::unique_ptr<VASTParameterSlider> m_fSamplerGrainRandom;
    std::unique_ptr<juce::Label> label28;
    std::unique_ptr<juce::Label> label29;
    std::unique_ptr<VASTParameterSlider> m_fSamplerGrainShape;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VASTSamplerNoiseComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

