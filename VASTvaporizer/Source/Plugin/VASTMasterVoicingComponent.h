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
#include "VASTVUMeter.h"
#include "VASTControls/VASTDrawableButton.h"
#include "VASTScopeDisplay/VASTRingBuffer.h"
#include "VASTScopeDisplay/VASTOscilloscopeOGL2D.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class VASTMasterVoicingComponent  : public Component,
                                    public juce::Slider::Listener,
                                    public juce::ComboBox::Listener,
                                    public juce::Button::Listener
{
public:
    //==============================================================================
    VASTMasterVoicingComponent (AudioProcessorEditor *editor, AudioProcessor* processor);
    ~VASTMasterVoicingComponent() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	//Label* getComponentCUnderruns();
	Label* getComponentCVoices();
	Label* getLabelVersion() {
		return c_LabelVersion.get();
	};
    void initAll();
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void sliderValueChanged (juce::Slider* sliderThatWasMoved) override;
    void comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged) override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;

    // Binary resources:
    static const char* sidepanel_svg;
    static const int sidepanel_svgSize;


private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	VASTAudioProcessorEditor * myEditor = nullptr;
	VASTAudioProcessor* myProcessor = nullptr;

	//std::unique_ptr<Label> c_underruns;
	std::unique_ptr<Label> c_voices;
    bool m_initDone = false;
    bool mb_unlazy = false;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<VASTParameterSlider> m_fMasterVolumedB;
    std::unique_ptr<VASTParameterSlider> m_fMasterTune;
    std::unique_ptr<juce::Label> label82;
    std::unique_ptr<juce::Label> label120;
    std::unique_ptr<VASTParameterComboBox> m_uPolyMode;
    std::unique_ptr<VASTParameterSlider> m_fVoiceGain;
    std::unique_ptr<VASTParameterSlider> m_fPitchMod;
    std::unique_ptr<juce::Label> label35;
    std::unique_ptr<juce::Label> label36;
    std::unique_ptr<juce::Label> label15;
    std::unique_ptr<VASTParameterButton> m_bLegatoMode;
    std::unique_ptr<juce::Label> c_LabelVersion;
    std::unique_ptr<VASTDrawableButton> c_iconSidePanel;
    std::unique_ptr<VASTVUMeter> c_VUmeter;
    std::unique_ptr<juce::GroupComponent> c_groupComponent;
    std::unique_ptr<juce::ToggleButton> c_DBG_dump_file;
    std::unique_ptr<juce::Label> c_DBG_dump_file_label;
    std::unique_ptr<VASTOscilloscopeOGL2D> oscilloscopeOLG2D;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VASTMasterVoicingComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

