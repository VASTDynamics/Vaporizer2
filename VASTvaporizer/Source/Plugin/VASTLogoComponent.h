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
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class VASTLogoComponent  : public Component,
                           public juce::Button::Listener
{
public:
    //==============================================================================
    VASTLogoComponent (AudioProcessorEditor *editor, AudioProcessor* processor);
    ~VASTLogoComponent() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;

    // Binary resources:
    static const char* copyright_svg;
    static const int copyright_svgSize;
    static const char* dynamics_svg;
    static const int dynamics_svgSize;
    static const char* logokreisv_svg;
    static const int logokreisv_svgSize;
    static const char* logokreis_svg;
    static const int logokreis_svgSize;
    static const char* vaporizer2_svg;
    static const int vaporizer2_svgSize;
    static const char* vast_svg;
    static const int vast_svgSize;


private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	VASTAudioProcessorEditor * myEditor = nullptr;
	VASTAudioProcessor* myProcessor = nullptr;

    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::TextButton> c_screenSize;
    std::unique_ptr<juce::Drawable> drawable1;
    std::unique_ptr<juce::Drawable> drawable2;
    std::unique_ptr<juce::Drawable> drawable3;
    std::unique_ptr<juce::Drawable> drawable4;
    std::unique_ptr<juce::Drawable> drawable5;
    std::unique_ptr<juce::Drawable> drawable6;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VASTLogoComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

