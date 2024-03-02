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
#include "../VASTUtils/VASTGUINotifier.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class VASTFXPane  : public Component,
                    public VASTGUINotifier
{
public:
    //==============================================================================
    VASTFXPane (AudioProcessorEditor *editor, AudioProcessor* processor, int busnr);
    ~VASTFXPane() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	Viewport* getEffectViewport() { return c_fxviewport.get(); };
	void shallUpdateGUI(const String compID, bool toggleState) override;
	void updateAll();
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	VASTAudioProcessorEditor * myEditor;
	VASTAudioProcessor* myProcessor;
	int myBusnr = 0;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::Viewport> c_fxselect;
    std::unique_ptr<juce::Viewport> c_fxviewport;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VASTFXPane)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

