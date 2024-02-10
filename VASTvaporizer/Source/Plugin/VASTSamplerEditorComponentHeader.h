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
#include "VASTWTEditor/VASTSamplerViewport.h"
#include "VASTControls/VASTDrawableButton.h"
#include "VASTUtils/VASTLookAndFeel.h"
#include "VASTWaveTableEditorComponent.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
An auto-generated component, created by the Projucer.

Describe your class and how it works here!
                                                                    //[/Comments]
*/
class VASTSamplerEditorComponentHeader  : public Component,
                                          public juce::Slider::Listener,
                                          public juce::Button::Listener
{
public:
    //==============================================================================
    VASTSamplerEditorComponentHeader (VASTAudioProcessorEditor *editor, VASTAudioProcessor* processor, VASTWaveTableEditorComponent *wteditor);
    ~VASTSamplerEditorComponentHeader() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	VASTParameterSlider* getScaleComponent() {
		return m_iWAVScale.get();
	}
	VASTParameterSlider* getWAVScale() { return m_iWAVScale.get(); };
	TextEditor* getWAVNameBox() { return c_wavName.get();  };
	ToggleButton* getSnapBox() { return c_snap.get(); };
	VASTParameterSlider* getWAVWTPosSlider() { return m_iWavWTPos.get();  };
	VASTDrawableButton* getIconLoadWav() { return c_iconLoadWav.get(); };
	VASTDrawableButton* getIconClearWav() { return c_iconClearWav.get(); };
	VASTDrawableButton* getIconToEditor() { return c_iconToEditor.get(); };
	VASTDrawableButton* getIconZeroLoop() { return c_iconZeroLoop.get(); };

	void lookAndFeelChanged() override;
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void sliderValueChanged (juce::Slider* sliderThatWasMoved) override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;
    void visibilityChanged() override;

    // Binary resources:
    static const char* close_svg;
    static const int close_svgSize;
    static const char* load_svg;
    static const int load_svgSize;
    static const char* normalize3_svg;
    static const int normalize3_svgSize;
    static const char* to_editor_svg;
    static const int to_editor_svgSize;
    static const char* zero_find_svg;
    static const int zero_find_svgSize;
    static const char* zoom_in_svg;
    static const int zoom_in_svgSize;
    static const char* arrow_right_corner_svg;
    static const int arrow_right_corner_svgSize;


private:
    //[UserVariables]   -- You can add your own custom variables in this section.

	VASTAudioProcessorEditor* myEditor = nullptr;
	VASTAudioProcessor* myProcessor = nullptr;
	VASTWaveTableEditorComponent* myWTEditor = nullptr;

	friend class VASTWaveTableEditorComponent;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<VASTParameterSlider> m_iWAVScale;
    std::unique_ptr<juce::TextEditor> c_wavName;
    std::unique_ptr<juce::Label> label14;
    std::unique_ptr<VASTParameterSlider> m_iWavWTPos;
    std::unique_ptr<juce::Label> label16;
    std::unique_ptr<VASTDrawableButton> c_iconLoadWav;
    std::unique_ptr<VASTDrawableButton> c_iconToEditor;
    std::unique_ptr<VASTDrawableButton> c_iconClearWav;
    std::unique_ptr<VASTDrawableButton> c_iconNormalizeWav;
    std::unique_ptr<VASTDrawableButton> c_iconMaximizeSampler;
    std::unique_ptr<VASTDrawableButton> c_iconZeroLoop;
    std::unique_ptr<juce::Label> label2;
    std::unique_ptr<juce::ToggleButton> c_snap;
    std::unique_ptr<juce::Drawable> drawable1;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VASTSamplerEditorComponentHeader)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

