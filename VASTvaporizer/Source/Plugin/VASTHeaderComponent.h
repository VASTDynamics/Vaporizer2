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
#ifdef VASTCOMMERCIAL
	#include "VASTCommercial/VASTLicenseEditor.h"
#endif
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class VASTHeaderComponent  : public Component,
                             public FileDragAndDropTarget,
                             public juce::ComboBox::Listener,
                             public juce::Button::Listener
{
public:
    //==============================================================================
    VASTHeaderComponent (AudioProcessorEditor *editor, AudioProcessor* processor);
    ~VASTHeaderComponent() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	Label* getLabelLicense() {
		return c_LabelLicense.get();
	};
	VASTComboPreset* getComponentCPreset() {
		return c_Preset.get();
	}
    bool isInterestedInFileDrag(const juce::StringArray& filenames);
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged) override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;
    void filesDropped (const juce::StringArray& filenames, int mouseX, int mouseY) override;
    void mouseDown (const juce::MouseEvent& e) override;

    // Binary resources:
    static const char* dragdrop_png;
    static const int dragdrop_pngSize;
    static const char* dragdrop_over_png;
    static const int dragdrop_over_pngSize;
    static const char* move_grabber_png;
    static const int move_grabber_pngSize;


private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	VASTAudioProcessorEditor* myEditor = nullptr;
	VASTAudioProcessor* myProcessor = nullptr;

#ifdef VASTCOMMERCIAL
	std::unique_ptr<VASTLicenseEditor> l_veditor;
#endif

    std::unique_ptr<FileChooser> myChooser;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<VASTComboPreset> c_Preset;
    std::unique_ptr<juce::TextButton> c_ReloadPresets;
    std::unique_ptr<juce::TextButton> c_SavePreset;
    std::unique_ptr<juce::TextButton> c_PresetUp;
    std::unique_ptr<juce::TextButton> c_PresetDown;
    std::unique_ptr<juce::TextButton> c_Undo;
    std::unique_ptr<juce::TextButton> c_Redo;
    std::unique_ptr<juce::Label> c_LabelLicense;
    std::unique_ptr<juce::TextButton> c_InitPreset;
    std::unique_ptr<juce::TextButton> c_Randomize;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VASTHeaderComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

