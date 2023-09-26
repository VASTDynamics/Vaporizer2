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
#include "VASTUtils/VASTLookAndFeel.h"
#include "VASTOscillatorComponent.h"
#include "VASTSamplerNoiseComponent.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class VASTGeneratorsComponent  : public Component
{
public:
    //==============================================================================
    VASTGeneratorsComponent (AudioProcessorEditor *editor, AudioProcessor* processor);
    ~VASTGeneratorsComponent() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	VASTOscillatorComponent* getOscillatorAComponent() {
		return oscillatorAComponent.get();
	};
	VASTOscillatorComponent* getOscillatorBComponent() {
		return oscillatorBComponent.get();
	};
	VASTOscillatorComponent* getOscillatorCComponent() {
		return oscillatorCComponent.get();
	};
	VASTOscillatorComponent* getOscillatorDComponent() {
		return oscillatorDComponent.get();
	};

    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	VASTAudioProcessorEditor * myEditor = nullptr;
	VASTAudioProcessor* myProcessor = nullptr;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<VASTOscillatorComponent> oscillatorAComponent;
    std::unique_ptr<VASTOscillatorComponent> oscillatorBComponent;
    std::unique_ptr<VASTOscillatorComponent> oscillatorCComponent;
    std::unique_ptr<VASTOscillatorComponent> oscillatorDComponent;
    std::unique_ptr<VASTSamplerNoiseComponent> samplerNoiseComponent;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VASTGeneratorsComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

