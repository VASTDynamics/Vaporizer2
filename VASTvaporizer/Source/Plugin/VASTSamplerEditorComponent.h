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
#include "../Engine/VASTSampler.h"
#include "VASTWTEditor/VASTSamplerViewport.h"
#include "VASTControls/VASTDrawableButton.h"
#include "VASTSamplerEditorComponentHeader.h"
#include "VASTWaveTableEditorComponent.h"

class VASTSamplerEditorComponentHeader; //forward declaration
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
An auto-generated component, created by the Projucer.

Describe your class and how it works here!
                                                                    //[/Comments]
*/
class VASTSamplerEditorComponent  : public Component
{
public:
    //==============================================================================
    VASTSamplerEditorComponent (AudioProcessorEditor *editor, AudioProcessor* processor, VASTWaveTableEditorComponent *wteditor );
    ~VASTSamplerEditorComponent() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	VASTViewport* getSamplerViewport() { return c_viewportWAVLoop.get(); };
	VASTSamplerEditorComponentHeader* getHeader() { return c_samplerHeaderComponent.get(); };
	void updateAll() {
		((VASTSamplerViewport*)c_viewportWAVLoop.get())->updateContentAsync();
	};
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
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<VASTViewport> c_viewportWAVLoop;
    std::unique_ptr<VASTSamplerEditorComponentHeader> c_samplerHeaderComponent;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VASTSamplerEditorComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

