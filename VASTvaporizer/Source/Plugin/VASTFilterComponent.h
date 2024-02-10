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
#include "../Engine/VASTEngineHeader.h"
#include "VASTControls/VASTComboBrev.h"
#include "VASTControls/VASTComboBox.h"
#include "VASTControls/VASTSlider.h"
#include "VASTControls/VASTComboPreset.h"
#include "VASTControls/VASTImageButton.h"
#include "VASTControls/VASTDrawableButton.h"
#include "VASTScopeDisplay/VASTFilterDisplay.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class VASTFilterComponent  : public Component,
                             public juce::Slider::Listener,
                             public juce::Button::Listener,
                             public juce::ComboBox::Listener
{
public:
    //==============================================================================
    VASTFilterComponent (AudioProcessorEditor *editor, AudioProcessor* processor);
    ~VASTFilterComponent() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	void startAutoUpdate();
	void stopAutoUpdate();
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void sliderValueChanged (juce::Slider* sliderThatWasMoved) override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;
    void comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged) override;

    // Binary resources:
    static const char* arrow_right_corner_svg;
    static const int arrow_right_corner_svgSize;


private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	VASTAudioProcessorEditor * myEditor = nullptr;
	VASTAudioProcessor* myProcessor = nullptr;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::Label> label40;
    std::unique_ptr<juce::Label> label42;
    std::unique_ptr<juce::Label> label75;
    std::unique_ptr<juce::Label> label26;
    std::unique_ptr<juce::Label> label28;
    std::unique_ptr<juce::Label> label44;
    std::unique_ptr<VASTParameterSlider> m_fFilterCutoff_Filter2;
    std::unique_ptr<VASTParameterSlider> m_fFilterReso_Filter1;
    std::unique_ptr<VASTParameterSlider> m_fVCFEnvMod_Filter2;
    std::unique_ptr<VASTParameterSlider> m_fFilterCutoff_Filter1;
    std::unique_ptr<VASTParameterSlider> m_fFilterDrive_Filter2;
    std::unique_ptr<VASTParameterSlider> m_fFilterReso_Filter2;
    std::unique_ptr<VASTParameterSlider> m_fFilterDrive_Filter1;
    std::unique_ptr<juce::Label> label77;
    std::unique_ptr<juce::Label> label78;
    std::unique_ptr<juce::Label> label81;
    std::unique_ptr<VASTParameterSlider> m_fFilterCutoff_Filter3;
    std::unique_ptr<VASTParameterSlider> m_fFilterDrive_Filter3;
    std::unique_ptr<VASTParameterSlider> m_fFilterReso_Filter3;
    std::unique_ptr<VASTParameterSlider> m_fFilterDryWet_Filter1;
    std::unique_ptr<VASTParameterSlider> m_fFilterDryWet_Filter3;
    std::unique_ptr<juce::Label> label97;
    std::unique_ptr<juce::Label> label112;
    std::unique_ptr<juce::Label> label113;
    std::unique_ptr<VASTParameterSlider> m_fFilterDryWet_Filter2;
    std::unique_ptr<VASTParameterSlider> m_fVCFEnvMod_Filter1;
    std::unique_ptr<VASTParameterSlider> m_fVCFEnvMod_Filter3;
    std::unique_ptr<VASTParameterButton> m_bOnOff_Filter1;
    std::unique_ptr<VASTParameterButton> m_bOnOff_Filter2;
    std::unique_ptr<VASTParameterButton> m_bOnOff_Filter3;
    std::unique_ptr<VASTFilterDisplay> c_filterDisplay;
    std::unique_ptr<VASTParameterComboBox> m_uVCFEnv_Filter1;
    std::unique_ptr<juce::Label> label13;
    std::unique_ptr<VASTParameterComboBox> m_uVCFEnv_Filter2;
    std::unique_ptr<VASTParameterComboBox> m_uVCFEnv_Filter3;
    std::unique_ptr<VASTParameterComboBox> m_uFilterRouting_Filter1;
    std::unique_ptr<juce::Label> label6;
    std::unique_ptr<VASTParameterComboBox> m_uFilterRouting_Filter2;
    std::unique_ptr<VASTParameterComboBox> m_uFilterRouting_Filter3;
    std::unique_ptr<VASTParameterComboBox> m_uFilterType_Filter1;
    std::unique_ptr<VASTParameterComboBox> m_uFilterType_Filter2;
    std::unique_ptr<VASTParameterComboBox> m_uFilterType_Filter3;
    std::unique_ptr<juce::Label> label2;
    std::unique_ptr<VASTParameterComboBox> m_uFilterRouting2_Filter1;
    std::unique_ptr<juce::Label> label3;
    std::unique_ptr<VASTParameterComboBox> m_uFilterRouting2_Filter2;
    std::unique_ptr<VASTParameterComboBox> m_uFilterRouting2_Filter3;
    std::unique_ptr<juce::Label> label4;
    std::unique_ptr<juce::Label> label5;
    std::unique_ptr<VASTParameterSlider> m_fFilterScale_Filter1;
    std::unique_ptr<VASTParameterSlider> m_fFilterScale_Filter2;
    std::unique_ptr<juce::Label> label7;
    std::unique_ptr<VASTParameterSlider> m_fFilterScale_Filter3;
    std::unique_ptr<juce::Label> label8;
    std::unique_ptr<juce::Label> label9;
    std::unique_ptr<VASTParameterSlider> m_fFilterGain_Filter1;
    std::unique_ptr<VASTParameterSlider> m_fFilterGain_Filter2;
    std::unique_ptr<juce::Label> label10;
    std::unique_ptr<VASTParameterSlider> m_fFilterGain_Filter3;
    std::unique_ptr<juce::Label> label11;
    std::unique_ptr<VASTDrawableButton> c_iconMaximizeEditor;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VASTFilterComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

