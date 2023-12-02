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
#include "VASTControls/VASTComboBrev.h"
#include "VASTControls/VASTComboBox.h"
#include "VASTControls/VASTSlider.h"
#include "VASTControls/VASTComboPreset.h"
#include "VASTControls/VASTImageButton.h"
#include "VASTControls/VASTDrawableButton.h"
#include "VASTControls/VASTDragMatrix.h"
#include "VASTScopeDisplay/VASTModMatCurveDisplay.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class VASTMatrixComponent  : public Component,
                             public Button::Listener
{
public:
    //==============================================================================
    VASTMatrixComponent (AudioProcessorEditor *editor, AudioProcessor* processor);
    ~VASTMatrixComponent() override;
    
    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void updateAll(); //repaint all
    void buttonClicked(Button* buttonThatWasClicked) override;
    void startAutoUpdate();
    void stopAutoUpdate();
    void lazyInit();
    //[/UserMethods]
    
    void paint (juce::Graphics& g) override;
    void resized() override;
    
    // Binary resources:
    static const char* delete_svg;
    static const int delete_svgSize;
    static const char* arrow_right_corner_svg;
    static const int arrow_right_corner_svgSize;
    
    
private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    VASTAudioProcessorEditor * myEditor = nullptr;
    VASTAudioProcessor* myProcessor = nullptr;
    
    OwnedArray<VASTDragMatrix> dragMatrixArray{};
	bool m_lazyInit = false;

    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<VASTDrawableButton> c_iconDelSlots;
    std::unique_ptr<juce::Label> label;
    std::unique_ptr<juce::Label> label2;
    std::unique_ptr<juce::Label> label3;
    std::unique_ptr<juce::Label> label4;
    std::unique_ptr<juce::Label> label5;
    std::unique_ptr<VASTDrawableButton> c_iconMaximizeEditor;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VASTMatrixComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

