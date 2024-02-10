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
#include "../Engine/VASTSampler.h"
#include "VASTWTEditor/VASTFreqDomainViewport.h"
#include "VASTWTEditor/VASTPositionViewport.h"
#include "VASTControls/VASTDrawableButton.h"
#include "VASTUtils/VASTLookAndFeel.h"
#include "VASTScopeDisplay/VASTOscilloscope.h"
#include "VASTWaveTableEditorViewHeader.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
An auto-generated component, created by the Projucer.

Describe your class and how it works here!
                                                                    //[/Comments]
*/
class VASTWaveTableEditorView  : public Component
{
public:
    //==============================================================================
    VASTWaveTableEditorView (AudioProcessorEditor *editor, AudioProcessor* processor, VASTWaveTableEditorComponent *wteditor );
    ~VASTWaveTableEditorView() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	VASTWaveTableEditorViewHeader* getHeader() { return c_editorHeaderComponent.get(); };
	VASTOscilloscope* getEditorOscilloscope() {	return c_editorOscilloscope.get();	};
	VASTViewport* getViewportPositions() { return c_viewportPositions.get(); };
	VASTViewport* getViewportFreqDomain() { return c_viewportFreqDomain.get(); };
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void visibilityChanged() override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	VASTAudioProcessorEditor* myEditor = nullptr;
	VASTAudioProcessor* myProcessor = nullptr;
	VASTWaveTableEditorComponent* myWTEditor = nullptr;

	friend class VASTWaveTableEditorComponent;
	friend class VASTPositionViewport;
	friend class VASTFreqDomainViewport;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<VASTOscilloscope> c_editorOscilloscope;
    std::unique_ptr<VASTViewport> c_viewportPositions;
    std::unique_ptr<VASTViewport> c_viewportFreqDomain;
    std::unique_ptr<VASTWaveTableEditorViewHeader> c_editorHeaderComponent;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VASTWaveTableEditorView)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

