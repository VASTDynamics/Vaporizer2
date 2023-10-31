/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 6.1.2

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "../Engine/VASTEngineHeader.h"
#include "VASTAudioProcessor.h"
#include "VASTAudioProcessorEditor.h"
#include "VASTControls/VASTPitchbendSlider.h"
#include "VASTControls/VASTModwheelSlider.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class VASTKeyboardComponent  : public Component,
                               public Slider::Listener
{
public:
    //==============================================================================
    VASTKeyboardComponent (AudioProcessorEditor *editor, AudioProcessor* processor);
    ~VASTKeyboardComponent() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	MidiKeyboardComponent* getMidiKeyboard() {
		return c_midiKeyboard.get();
	}

	MidiKeyboardState* getMidiKeyboardState() {
		return &myMidiKeyboardState;
	}

	VASTModwheelSlider* getModwheel() {
		return c_modWheel.get();
	};

	VASTPitchbendSlider* getPitchbend() {
		return c_pitchBend.get();
	};

	void sliderValueChanged(Slider* sliderThatWasMoved) override;

    void updateModwheel(float modWheelVal) {

        Component::SafePointer<VASTKeyboardComponent> kbc_(this);
        Timer::callAfterDelay(10, [kbc_, modWheelVal, this] {
            if (kbc_ != nullptr)
               if (this->getModwheel() != nullptr)
                   this->getModwheel()->setValue(modWheelVal, juce::NotificationType::dontSendNotification); });
	};
	void updatePitchbend(float pitchWheelVal) {

        Component::SafePointer<VASTKeyboardComponent> kbc_(this);
        Timer::callAfterDelay(10, [kbc_, pitchWheelVal, this] {
            if (kbc_ != nullptr)
                if (this->getPitchbend() != nullptr)
                    this->getPitchbend()->setValue(pitchWheelVal, juce::NotificationType::dontSendNotification); });
	};

	void updateAll() {
		c_iBendRange->setValue(myProcessor->getBendRange(), NotificationType::dontSendNotification);
	};

    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	MidiKeyboardState myMidiKeyboardState;

	VASTAudioProcessorEditor* myEditor = nullptr;
	VASTAudioProcessor* myProcessor = nullptr;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<MidiKeyboardComponent> c_midiKeyboard;
    std::unique_ptr<VASTPitchbendSlider> c_pitchBend;
    std::unique_ptr<VASTModwheelSlider> c_modWheel;
    std::unique_ptr<VASTSlider> c_iBendRange;
    std::unique_ptr<juce::Label> label100;
    std::unique_ptr<juce::Label> label2;
    std::unique_ptr<VASTParameterSlider> m_fPortamento;
    juce::Path internalPath1;
    juce::Path internalPath2;
    juce::Path internalPath3;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VASTKeyboardComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

