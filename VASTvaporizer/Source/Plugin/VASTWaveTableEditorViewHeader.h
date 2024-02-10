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
#include "VASTControls/VASTDrawableButton.h"
#include "VASTControls/VASTSlider.h"
#include "VASTControls/VASTComboBox.h"
#include "VASTUtils/VASTLookAndFeel.h"
#include "VASTSamplerEditorComponent.h"
#include "VASTSamplerEditorComponentHeader.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
An auto-generated component, created by the Projucer.

Describe your class and how it works here!
                                                                    //[/Comments]
*/
class VASTWaveTableEditorViewHeader  : public Component,
                                       public juce::Slider::Listener,
                                       public juce::ComboBox::Listener
{
public:
    //==============================================================================
    VASTWaveTableEditorViewHeader (AudioProcessorEditor *editor, AudioProcessor* processor, VASTWaveTableEditorComponent *wteditor );
    ~VASTWaveTableEditorViewHeader() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	VASTParameterSlider* getWTEditorZoom() { return m_iWTEditorZoom.get(); };
	TextEditor* getWTNameBox() { return c_wtName.get();	};
	VASTSlider* getGenMaxSlider() { return c_genMax.get(); };
	VASTSlider* getApplyMaxSlider() { return c_applyMax.get(); };

	VASTSlider* getBitDepthSlider() { return c_bitDepth.get(); };
	VASTSlider* getBendSlider() { return c_Bend.get(); };
	VASTSlider* getBloatSlider() { return c_Bloat.get(); };
	VASTSlider* getCleanSlider() { return c_Clean.get(); };
	VASTSlider* getSmoothSlider() { return c_Smooth.get(); };
	VASTSlider* getOffsetSlider() { return c_Offset.get(); };
	VASTSlider* getFreqSlider() { return c_Freq.get(); };
	VASTSlider* getFreqModSlider() { return c_FreqMod.get(); };
	VASTSlider* getLevelSlider() { return c_Level.get(); };
	VASTSlider* getPhaseSlider() { return c_Phase.get(); };
	VASTSlider* getConnectSlider() { return c_Connect.get(); };

	ToggleButton* getOscAButton() { return c_oscA.get(); };
	ToggleButton* getOscBButton() { return c_oscB.get(); };
	ToggleButton* getOscCButton() { return c_oscC.get(); };
	ToggleButton* getOscDButton() { return c_oscD.get(); };

	void lookAndFeelChanged() override;
	void updateAll();

	void setSelectionLabel(int from, int to, int numPos) {
		c_positionLabel->setText(String(from + 1) + "-" + String(to + 1) + "/" + String(numPos), NotificationType::dontSendNotification);
	};

    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void sliderValueChanged (juce::Slider* sliderThatWasMoved) override;
    void comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged) override;
    void visibilityChanged() override;

    // Binary resources:
    static const char* add_svg;
    static const int add_svgSize;
    static const char* apply_svg;
    static const int apply_svgSize;
    static const char* arc_connect_svg;
    static const int arc_connect_svgSize;
    static const char* close_svg;
    static const int close_svgSize;
    static const char* copy_svg;
    static const int copy_svgSize;
    static const char* cut_svg;
    static const int cut_svgSize;
    static const char* delete_svg;
    static const int delete_svgSize;
    static const char* duplicate_svg;
    static const int duplicate_svgSize;
    static const char* generate_svg;
    static const int generate_svgSize;
    static const char* invert_svg;
    static const int invert_svgSize;
    static const char* load_svg;
    static const int load_svgSize;
    static const char* minus_svg;
    static const int minus_svgSize;
    static const char* normalize_svg;
    static const int normalize_svgSize;
    static const char* normalize2_svg;
    static const int normalize2_svgSize;
    static const char* normalize3_svg;
    static const int normalize3_svgSize;
    static const char* paste_svg;
    static const int paste_svgSize;
    static const char* plus_svg;
    static const int plus_svgSize;
    static const char* ramp_svg;
    static const int ramp_svgSize;
    static const char* ramp_connect_svg;
    static const int ramp_connect_svgSize;
    static const char* rampconnect_svg;
    static const int rampconnect_svgSize;
    static const char* reverse_svg;
    static const int reverse_svgSize;
    static const char* save_svg;
    static const int save_svgSize;
    static const char* select_all_svg;
    static const int select_all_svgSize;
    static const char* to_editor_svg;
    static const int to_editor_svgSize;
    static const char* waves_halfsine_svg;
    static const int waves_halfsine_svgSize;
    static const char* waves_ramp_svg;
    static const int waves_ramp_svgSize;
    static const char* waves_random_svg;
    static const int waves_random_svgSize;
    static const char* waves_saw_svg;
    static const int waves_saw_svgSize;
    static const char* waves_sinc_svg;
    static const int waves_sinc_svgSize;
    static const char* waves_sine_svg;
    static const int waves_sine_svgSize;
    static const char* waves_square_svg;
    static const int waves_square_svgSize;
    static const char* waves_triangle_svg;
    static const int waves_triangle_svgSize;
    static const char* waves_zeros_svg;
    static const int waves_zeros_svgSize;
    static const char* zero_find_svg;
    static const int zero_find_svgSize;
    static const char* zoom_in_svg;
    static const int zoom_in_svgSize;
    static const char* zoom_out_svg;
    static const int zoom_out_svgSize;
    static const char* dynamics_svg;
    static const int dynamics_svgSize;
    static const char* waves_samplehold_svg;
    static const int waves_samplehold_svgSize;
    static const char* arrow_right_corner_svg;
    static const int arrow_right_corner_svgSize;
    static const char* record_svg;
    static const int record_svgSize;


private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	VASTAudioProcessorEditor * myEditor = nullptr;
	VASTAudioProcessor* myProcessor = nullptr;
	VASTWaveTableEditorComponent* myWTEditor = nullptr;

	friend class VASTWaveTableEditorComponent;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::Label> label10;
    std::unique_ptr<juce::Label> label9;
    std::unique_ptr<juce::Label> label7;
    std::unique_ptr<juce::Label> label8;
    std::unique_ptr<VASTSlider> c_Phase;
    std::unique_ptr<juce::Label> label108;
    std::unique_ptr<juce::Label> label135;
    std::unique_ptr<VASTSlider> c_Freq;
    std::unique_ptr<juce::Label> label2;
    std::unique_ptr<VASTSlider> c_Offset;
    std::unique_ptr<VASTSlider> c_bitDepth;
    std::unique_ptr<juce::Label> label3;
    std::unique_ptr<juce::Label> label4;
    std::unique_ptr<VASTSlider> c_Smooth;
    std::unique_ptr<juce::Label> label5;
    std::unique_ptr<VASTSlider> c_Clean;
    std::unique_ptr<juce::Label> label6;
    std::unique_ptr<juce::ToggleButton> c_oscA;
    std::unique_ptr<juce::ToggleButton> c_oscB;
    std::unique_ptr<juce::ToggleButton> c_oscC;
    std::unique_ptr<juce::ToggleButton> c_oscD;
    std::unique_ptr<VASTParameterSlider> m_iWTEditorZoom;
    std::unique_ptr<juce::TextEditor> c_wtName;
    std::unique_ptr<juce::Label> label13;
    std::unique_ptr<VASTSlider> c_FreqMod;
    std::unique_ptr<juce::Label> label15;
    std::unique_ptr<VASTSlider> c_genMax;
    std::unique_ptr<VASTSlider> c_Bend;
    std::unique_ptr<juce::Label> label18;
    std::unique_ptr<VASTSlider> c_Bloat;
    std::unique_ptr<juce::Label> label19;
    std::unique_ptr<VASTSlider> c_Level;
    std::unique_ptr<VASTDrawableButton> c_iconSelectAll;
    std::unique_ptr<VASTDrawableButton> c_iconAddPos;
    std::unique_ptr<VASTDrawableButton> c_iconDelPos;
    std::unique_ptr<VASTDrawableButton> c_iconPaste;
    std::unique_ptr<VASTDrawableButton> c_iconDuplicate;
    std::unique_ptr<VASTDrawableButton> c_iconThinOut;
    std::unique_ptr<VASTDrawableButton> c_iconNormalize;
    std::unique_ptr<VASTDrawableButton> c_iconCopy;
    std::unique_ptr<VASTDrawableButton> c_iconSine;
    std::unique_ptr<VASTDrawableButton> c_iconHalfSine;
    std::unique_ptr<VASTDrawableButton> c_iconTriangle;
    std::unique_ptr<VASTDrawableButton> c_iconZeros;
    std::unique_ptr<VASTDrawableButton> c_iconSaw;
    std::unique_ptr<VASTDrawableButton> c_iconSquare;
    std::unique_ptr<VASTDrawableButton> c_iconRamp;
    std::unique_ptr<VASTDrawableButton> c_iconRandom;
    std::unique_ptr<VASTDrawableButton> c_iconSinc;
    std::unique_ptr<VASTDrawableButton> c_iconReverse;
    std::unique_ptr<VASTDrawableButton> c_iconInvert;
    std::unique_ptr<VASTDrawableButton> c_iconClearWT;
    std::unique_ptr<VASTDrawableButton> c_iconLoadWT;
    std::unique_ptr<VASTDrawableButton> c_iconArcConnect;
    std::unique_ptr<VASTDrawableButton> c_iconRampConnect;
    std::unique_ptr<VASTDrawableButton> c_iconSave;
    std::unique_ptr<VASTComboBox> c_applyOptions;
    std::unique_ptr<VASTComboBox> c_generationOptions;
    std::unique_ptr<VASTDrawableButton> c_iconApply;
    std::unique_ptr<VASTDrawableButton> c_iconGenerate;
    std::unique_ptr<VASTSlider> c_Connect;
    std::unique_ptr<juce::Label> label11;
    std::unique_ptr<VASTSlider> c_applyMax;
    std::unique_ptr<juce::TextButton> c_WTableNext;
    std::unique_ptr<juce::TextButton> c_WTablePrevious;
    std::unique_ptr<juce::Label> label12;
    std::unique_ptr<juce::ToggleButton> c_soloSelection;
    std::unique_ptr<VASTComboBox> c_gridMode;
    std::unique_ptr<VASTComboBox> c_drawMode;
    std::unique_ptr<VASTComboBox> c_binMode;
    std::unique_ptr<VASTComboBox> c_binEditMode;
    std::unique_ptr<VASTDrawableButton> c_iconSampleHold;
    std::unique_ptr<juce::Label> c_positionLabel;
    std::unique_ptr<juce::Label> label14;
    std::unique_ptr<VASTDrawableButton> c_iconMaximizeEditor;
    std::unique_ptr<juce::ToggleButton> c_snap;
    std::unique_ptr<juce::ToggleButton> c_stretch;
    std::unique_ptr<juce::Label> label16;
    std::unique_ptr<VASTDrawableButton> c_iconRecordWT;
    std::unique_ptr<juce::Drawable> drawable1;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VASTWaveTableEditorViewHeader)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

