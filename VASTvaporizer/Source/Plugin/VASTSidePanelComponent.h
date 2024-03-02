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
#include "VASTControls/VASTDragSource.h"
#include "VASTControls/VASTParameterSlider.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class VASTSidePanelComponent  : public Component,
                                public juce::Slider::Listener
{
public:
    //==============================================================================
    VASTSidePanelComponent (AudioProcessorEditor *editor, AudioProcessor* processor);
    ~VASTSidePanelComponent() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	VASTDragSource* getComponentCustomMod1() {
		return c_dd_CustomModulator1.get();
	}
	VASTDragSource* getComponentCustomMod2() {
		return c_dd_CustomModulator2.get();
	}
	VASTDragSource* getComponentCustomMod3() {
		return c_dd_CustomModulator3.get();
	}
	VASTDragSource* getComponentCustomMod4() {
		return c_dd_CustomModulator4.get();
	}
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void sliderValueChanged (juce::Slider* sliderThatWasMoved) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	VASTAudioProcessorEditor * myEditor = nullptr;
	VASTAudioProcessor* myProcessor = nullptr;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<VASTDragSource> c_dd_Velocity;
    std::unique_ptr<VASTDragSource> c_dd_ModWheel;
    std::unique_ptr<VASTDragSource> c_dd_KeyTrack;
    std::unique_ptr<VASTDragSource> c_dd_Aftertouch;
    std::unique_ptr<VASTParameterSlider> m_fCustomModulator1;
    std::unique_ptr<VASTParameterSlider> m_fCustomModulator2;
    std::unique_ptr<VASTParameterSlider> m_fCustomModulator3;
    std::unique_ptr<VASTParameterSlider> m_fCustomModulator4;
    std::unique_ptr<VASTDragSource> c_dd_MSEG1Env;
    std::unique_ptr<VASTDragSource> c_dd_MSEG2Env;
    std::unique_ptr<VASTDragSource> c_dd_MSEG3Env;
    std::unique_ptr<VASTDragSource> c_dd_MSEG4Env;
    std::unique_ptr<VASTDragSource> c_dd_MSEG5Env;
    std::unique_ptr<VASTDragSource> c_dd_LFO1;
    std::unique_ptr<VASTDragSource> c_dd_LFO2;
    std::unique_ptr<VASTDragSource> c_dd_LFO3;
    std::unique_ptr<VASTDragSource> c_dd_StepSeq2;
    std::unique_ptr<VASTDragSource> c_dd_StepSeq3;
    std::unique_ptr<VASTDragSource> c_dd_StepSeq1;
    std::unique_ptr<VASTDragSource> c_dd_InputEnvelope;
    std::unique_ptr<VASTDragSource> c_dd_CustomModulator1;
    std::unique_ptr<VASTDragSource> c_dd_CustomModulator2;
    std::unique_ptr<VASTDragSource> c_dd_CustomModulator3;
    std::unique_ptr<VASTDragSource> c_dd_CustomModulator4;
    std::unique_ptr<VASTDragSource> c_dd_LFO4;
    std::unique_ptr<VASTDragSource> c_dd_LFO5;
    std::unique_ptr<VASTDragSource> c_dd_RandomDriftSlow;
    std::unique_ptr<VASTDragSource> c_dd_RandomDriftFast;
    std::unique_ptr<VASTDragSource> c_dd_MPETimbreMinMax;
    std::unique_ptr<VASTDragSource> c_dd_MPETimbreMax;
    std::unique_ptr<VASTDragSource> c_dd_MPETimbreMin;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VASTSidePanelComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

