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

//[Headers] You can add your own extra header files here...
#include "../Engine/VASTEngineHeader.h"
#include "VASTAudioProcessor.h"
#include "VASTAudioProcessorEditor.h"
#include "VASTControls/VASTParameterComboBox.h"
#include "VASTControls/VASTParameterSlider.h"
#include "VASTControls/VASTDrawableButton.h"
#include "VASTVUMeter.h"
#include "VASTFX/VASTFXPane.h"
//[/Headers]

#include "VASTMasterVoicingComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
VASTMasterVoicingComponent::VASTMasterVoicingComponent (AudioProcessorEditor *editor, AudioProcessor* processor)
    : myEditor((VASTAudioProcessorEditor*)editor), myProcessor((VASTAudioProcessor*)processor)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    m_fMasterVolumedB.reset (new VASTParameterSlider ("m_fMasterVolumedB"));
    addAndMakeVisible (m_fMasterVolumedB.get());
    m_fMasterVolumedB->setTooltip (TRANS("Master volume in dB. Parameter is saved as well with preset."));
    m_fMasterVolumedB->setRange (-60, 20, 0.01);
    m_fMasterVolumedB->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    m_fMasterVolumedB->setTextBoxStyle (juce::Slider::NoTextBox, false, 30, 14);
    m_fMasterVolumedB->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffc9c9c9));
    m_fMasterVolumedB->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0x00ffffff));
    m_fMasterVolumedB->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x00808080));
    m_fMasterVolumedB->addListener (this);

    m_fMasterTune.reset (new VASTParameterSlider ("m_fMasterTune"));
    addAndMakeVisible (m_fMasterTune.get());
    m_fMasterTune->setTooltip (TRANS("Master tune in hz. Parameter is saved as well with preset."));
    m_fMasterTune->setRange (400, 480, 0.01);
    m_fMasterTune->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    m_fMasterTune->setTextBoxStyle (juce::Slider::NoTextBox, false, 30, 14);
    m_fMasterTune->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffc9c9c9));
    m_fMasterTune->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0x00ffffff));
    m_fMasterTune->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x00808080));
    m_fMasterTune->addListener (this);

    label82.reset (new juce::Label ("new label",
                                    TRANS("VOLUME")));
    addAndMakeVisible (label82.get());
    label82->setFont (juce::Font ("Syntax", 11.00f, juce::Font::plain));
    label82->setJustificationType (juce::Justification::centred);
    label82->setEditable (false, false, false);
    label82->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label82->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label82->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label120.reset (new juce::Label ("new label",
                                     TRANS("TUNE")));
    addAndMakeVisible (label120.get());
    label120->setFont (juce::Font ("Syntax", 11.00f, juce::Font::plain));
    label120->setJustificationType (juce::Justification::centred);
    label120->setEditable (false, false, false);
    label120->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label120->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label120->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    m_uPolyMode.reset (new VASTParameterComboBox ("m_uPolyMode"));
    addAndMakeVisible (m_uPolyMode.get());
    m_uPolyMode->setTooltip (TRANS("Mono mode - one note at at time\n"
    "Poly4 mode - four notes at at time\n"
    "Poly16 mode - onemax. 16 notes at at time"));
    m_uPolyMode->setEditableText (false);
    m_uPolyMode->setJustificationType (juce::Justification::centredLeft);
    m_uPolyMode->setTextWhenNothingSelected (TRANS("POLY16"));
    m_uPolyMode->setTextWhenNoChoicesAvailable (TRANS("POLY16"));
    m_uPolyMode->addListener (this);

    m_fVoiceGain.reset (new VASTParameterSlider ("m_fVoiceGain"));
    addAndMakeVisible (m_fVoiceGain.get());
    m_fVoiceGain->setTooltip (TRANS("Voice gain percentage for velocity modulation"));
    m_fVoiceGain->setRange (0, 100, 0);
    m_fVoiceGain->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    m_fVoiceGain->setTextBoxStyle (juce::Slider::NoTextBox, false, 30, 14);
    m_fVoiceGain->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffc9c9c9));
    m_fVoiceGain->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0x00ffffff));
    m_fVoiceGain->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x00808080));
    m_fVoiceGain->addListener (this);

    m_fPitchMod.reset (new VASTParameterSlider ("m_fPitchMod"));
    addAndMakeVisible (m_fPitchMod.get());
    m_fPitchMod->setTooltip (TRANS("Voice pitch for modulation"));
    m_fPitchMod->setRange (400, 480, 0.01);
    m_fPitchMod->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    m_fPitchMod->setTextBoxStyle (juce::Slider::NoTextBox, false, 30, 14);
    m_fPitchMod->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffc9c9c9));
    m_fPitchMod->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0x00ffffff));
    m_fPitchMod->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x00808080));
    m_fPitchMod->addListener (this);

    label35.reset (new juce::Label ("new label",
                                    TRANS("GAINM")));
    addAndMakeVisible (label35.get());
    label35->setFont (juce::Font ("Syntax", 11.00f, juce::Font::plain));
    label35->setJustificationType (juce::Justification::centred);
    label35->setEditable (false, false, false);
    label35->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label35->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label35->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label36.reset (new juce::Label ("new label",
                                    TRANS("PITCHM")));
    addAndMakeVisible (label36.get());
    label36->setFont (juce::Font ("Syntax", 11.00f, juce::Font::plain));
    label36->setJustificationType (juce::Justification::centred);
    label36->setEditable (false, false, false);
    label36->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label36->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label36->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label15.reset (new juce::Label ("new label",
                                    TRANS("LEGATO")));
    addAndMakeVisible (label15.get());
    label15->setFont (juce::Font ("Syntax", 11.00f, juce::Font::plain));
    label15->setJustificationType (juce::Justification::centredLeft);
    label15->setEditable (false, false, false);
    label15->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label15->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label15->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    m_bLegatoMode.reset (new VASTParameterButton ("m_bLegatoMode"));
    addAndMakeVisible (m_bLegatoMode.get());
    m_bLegatoMode->setTooltip (TRANS("Legato mode for mono voicing"));
    m_bLegatoMode->setButtonText (juce::String());
    m_bLegatoMode->addListener (this);

    c_LabelVersion.reset (new juce::Label ("c_LabelVersion",
                                           TRANS("v0.00")));
    addAndMakeVisible (c_LabelVersion.get());
    c_LabelVersion->setFont (juce::Font ("Syntax", 11.00f, juce::Font::plain));
    c_LabelVersion->setJustificationType (juce::Justification::centredRight);
    c_LabelVersion->setEditable (false, false, false);
    c_LabelVersion->setColour (juce::Label::textColourId, juce::Colour (0x80adbac6));
    c_LabelVersion->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    c_LabelVersion->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    c_iconSidePanel.reset (new VASTDrawableButton ("c_iconSidePanel", sidepanel_svg, sidepanel_svgSize, "Show modulation sources side panel"));
    addAndMakeVisible (c_iconSidePanel.get());
    c_iconSidePanel->setName ("c_iconSidePanel");

    c_VUmeter.reset (new VASTVUMeter());
    addAndMakeVisible (c_VUmeter.get());
    c_VUmeter->setName ("c_VUmeter");

    c_groupComponent.reset (new juce::GroupComponent ("c_groupComponent",
                                                      juce::String()));
    addAndMakeVisible (c_groupComponent.get());
    c_groupComponent->setExplicitFocusOrder (1);
    c_groupComponent->setTextLabelPosition (juce::Justification::centredLeft);
    c_groupComponent->setColour (juce::GroupComponent::outlineColourId, juce::Colour (0x008e989b));

    c_DBG_dump_file.reset (new juce::ToggleButton ("c_DBG_dump_file"));
    addAndMakeVisible (c_DBG_dump_file.get());
    c_DBG_dump_file->setButtonText (juce::String());
    c_DBG_dump_file->addListener (this);

    c_DBG_dump_file_label.reset (new juce::Label ("c_DBG_dump_file_label",
                                                  TRANS("DBG: Dump File")));
    addAndMakeVisible (c_DBG_dump_file_label.get());
    c_DBG_dump_file_label->setFont (juce::Font ("Syntax", 11.00f, juce::Font::plain));
    c_DBG_dump_file_label->setJustificationType (juce::Justification::centredLeft);
    c_DBG_dump_file_label->setEditable (false, false, false);
    c_DBG_dump_file_label->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    c_DBG_dump_file_label->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    c_DBG_dump_file_label->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    oscilloscopeOLG2D.reset (new VASTOscilloscopeOGL2D (myEditor, &myProcessor->m_pVASTXperience.oscilloscopeRingBuffer));
    addAndMakeVisible (oscilloscopeOLG2D.get());
    oscilloscopeOLG2D->setName ("oscilloscopeOLG2D");


    //[UserPreSize]
	c_LabelVersion->setEnabled(false);

	c_iconSidePanel->addListener(this);
	c_VUmeter->setMeterSource(myProcessor->getMeterSource());
	c_VUmeter->setMeterFlags(VASTVUMeter::MeterFlags::Minimal);
	c_VUmeter->setOpaque(true);
	c_VUmeter->setPaintingIsUnclipped(true);

    c_groupComponent->setOpaque(false); //check
	c_groupComponent->setComponentID("Voicing");
	//c_underruns.reset(new Label("c_underruns", TRANS("underruns")));
	//c_groupComponent->addAndMakeVisible(c_underruns.get());
	//c_underruns->setTooltip(TRANS("Underruns"));
	//c_underruns->setFont(Font(11.00f, Font::plain).withTypefaceStyle("Regular"));
	//c_underruns->setJustificationType(Justification::centred);
	//c_underruns->setEditable(false, false, false);
	//c_underruns->setColour(Label::backgroundColourId, Colour(0xff26485f));
	//c_underruns->setColour(Label::outlineColourId, Colour(0xff26485f));
	//c_underruns->setColour(TextEditor::textColourId, Colours::black);
	//c_underruns->setColour(TextEditor::backgroundColourId, Colours::black);
	//c_underruns->setColour(TextEditor::highlightColourId, Colour(0xffe0ec65));
	//c_underruns->setComponentID("Voicing");

	c_voices.reset(new Label("c_voices",
		TRANS("voices / oscs")));
	c_groupComponent->addAndMakeVisible(c_voices.get());
	c_voices->setTooltip(TRANS("Voices / oscillators"));
	c_voices->setFont(Font(11.00f, Font::plain).withTypefaceStyle("Regular"));
	c_voices->setJustificationType(Justification::centred);
	c_voices->setEditable(false, false, false);
	c_voices->setColour(Label::backgroundColourId, Colour(0xff26485f));
	c_voices->setColour(Label::outlineColourId, Colour(0xff26485f));
	c_voices->setColour(TextEditor::textColourId, Colours::black);
	c_voices->setColour(TextEditor::backgroundColourId, Colours::black);
	c_voices->setColour(TextEditor::highlightColourId, Colour(0xffe0ec65));
	c_voices->setComponentID("Voicing");

	label15->setComponentID("Headertext");
	c_LabelVersion->setComponentID("Headertext");
	//c_underruns->setOpaque(true);
	c_voices->setOpaque(true);

    oscilloscopeOLG2D->setVisible(false); //make visible after initAll()
	oscilloscopeOLG2D->setOpaque(true);

	//debug controls
	c_DBG_dump_file->setVisible(false);
	c_DBG_dump_file_label->setVisible(false);
#ifdef _DEBUG
	c_DBG_dump_file->setVisible(true);
	c_DBG_dump_file_label->setVisible(true);
#endif

	//manage parameter bindings -> set ComponentID = Name and processor
	Array<Component*> children = this->getChildren();
	for (auto child : children) {
		if (child->getName().startsWith("m_")) {
			auto* aSlider = dynamic_cast<VASTParameterSlider*> (child);
			if (aSlider != nullptr) {
				aSlider->setAudioProcessor(*myProcessor);
				aSlider->bindParameter(myEditor, aSlider->getName(), VASTGUIRuntimeModel::GUIComponents::MasterVoicingComponent, 0);
			}
			auto* aCombobox = dynamic_cast<VASTParameterComboBox*> (child);
			if (aCombobox != nullptr) {
				aCombobox->setAudioProcessor(*myProcessor);
				aCombobox->bindParameter(myEditor, aCombobox->getName(), VASTGUIRuntimeModel::GUIComponents::MasterVoicingComponent, 0);
			}
			auto* aButton = dynamic_cast<VASTParameterButton*> (child);
			if (aButton != nullptr) {
				aButton->setAudioProcessor(*myProcessor);
				aButton->bindParameter(myEditor, aButton->getName(), VASTGUIRuntimeModel::GUIComponents::MasterVoicingComponent, 0);
			}
		}
	}
	setOpaque(true);
    m_initDone = false;
    
    return; //dont call setSize
    //[/UserPreSize]

    setSize (668, 76);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

VASTMasterVoicingComponent::~VASTMasterVoicingComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    m_fMasterVolumedB = nullptr;
    m_fMasterTune = nullptr;
    label82 = nullptr;
    label120 = nullptr;
    m_uPolyMode = nullptr;
    m_fVoiceGain = nullptr;
    m_fPitchMod = nullptr;
    label35 = nullptr;
    label36 = nullptr;
    label15 = nullptr;
    m_bLegatoMode = nullptr;
    c_LabelVersion = nullptr;
    c_iconSidePanel = nullptr;
    c_VUmeter = nullptr;
    c_groupComponent = nullptr;
    c_DBG_dump_file = nullptr;
    c_DBG_dump_file_label = nullptr;
    oscilloscopeOLG2D = nullptr;


    //[Destructor]. You can add your own custom destruction code here..

	//c_underruns = nullptr;
	c_voices = nullptr;
    //[/Destructor]
}

//==============================================================================
void VASTMasterVoicingComponent::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    if (m_initDone) {
#if defined JUCE_LINUX
        oscilloscopeOLG2D->setVisible(false);
#else
        oscilloscopeOLG2D->setVisible(!myProcessor->m_disableOpenGLGFX);
#endif
    }
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff0b0b0b));

    {
        int x = 0, y = 0, width = getWidth() - 0, height = getHeight() - 0;
        juce::Colour fillColour1 = juce::Colour (0xff171818), fillColour2 = juce::Colour (0xff0b0b0b);
        //[UserPaintCustomArguments] Customize the painting arguments here..
    		fillColour1 = myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colMasterVoicingComponentBackgroundGradientFrom);
    		fillColour2 = myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colMasterVoicingComponentBackgroundGradientTo);
        //[/UserPaintCustomArguments]
        g.setGradientFill (juce::ColourGradient (fillColour1,
                                             static_cast<float> (proportionOfWidth (-0.0120f)) - 0.0f + x,
                                             static_cast<float> (proportionOfHeight (-0.1053f)) - 0.0f + y,
                                             fillColour2,
                                             static_cast<float> (proportionOfWidth (1.0092f)) - 0.0f + x,
                                             static_cast<float> (proportionOfHeight (1.0526f)) - 0.0f + y,
                                             false));
        g.fillRect (x, y, width, height);
    }

    {
        int x = proportionOfWidth (0.0180f), y = proportionOfHeight (0.4868f), width = proportionOfWidth (0.3054f), height = proportionOfHeight (0.1447f);
        juce::Colour fillColour1 = juce::Colour (0xff3a4145), fillColour2 = juce::Colour (0xff33393c);
        //[UserPaintCustomArguments] Customize the painting arguments here..
    		fillColour1 = myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscillatorComponentHeaderGradientFrom);
    		fillColour2 = myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscillatorComponentHeaderGradientTo);
        //[/UserPaintCustomArguments]
        g.setGradientFill (juce::ColourGradient (fillColour1,
                                             static_cast<float> (proportionOfWidth (0.0194f)) - static_cast<float> (proportionOfWidth (0.0180f)) + x,
                                             static_cast<float> (proportionOfHeight (0.5106f)) - static_cast<float> (proportionOfHeight (0.4868f)) + y,
                                             fillColour2,
                                             static_cast<float> (proportionOfWidth (0.5629f)) - static_cast<float> (proportionOfWidth (0.0180f)) + x,
                                             static_cast<float> (proportionOfHeight (0.5106f)) - static_cast<float> (proportionOfHeight (0.4868f)) + y,
                                             false));
        g.fillRect (x, y, width, height);
    }

    {
        int x = proportionOfWidth (0.3817f), y = proportionOfHeight (0.0526f), width = proportionOfWidth (0.3174f), height = proportionOfHeight (0.8421f);
        juce::Colour fillColour = juce::Colours::black;
        juce::Colour strokeColour = juce::Colours::black;
        //[UserPaintCustomArguments] Customize the painting arguments here..
    		if (myProcessor->m_disableOpenGLGFX == true) {
    			fillColour = Colours::transparentBlack;
    			strokeColour = Colours::transparentBlack;
    		}			   
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.fillRect (x, y, width, height);
        g.setColour (strokeColour);
        g.drawRect (x, y, width, height, 2);

    }

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void VASTMasterVoicingComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    m_fMasterVolumedB->setBounds (proportionOfWidth (0.0255f), proportionOfHeight (0.0132f), proportionOfWidth (0.0524f), proportionOfHeight (0.4737f));
    m_fMasterTune->setBounds (proportionOfWidth (0.0988f), proportionOfHeight (0.0132f), proportionOfWidth (0.0524f), proportionOfHeight (0.4737f));
    label82->setBounds (proportionOfWidth (0.0150f), proportionOfHeight (0.4868f), proportionOfWidth (0.0838f), proportionOfHeight (0.1447f));
    label120->setBounds (proportionOfWidth (0.0808f), proportionOfHeight (0.4868f), proportionOfWidth (0.0838f), proportionOfHeight (0.1447f));
    m_uPolyMode->setBounds (proportionOfWidth (0.0180f), proportionOfHeight (0.6842f), proportionOfWidth (0.1198f), proportionOfHeight (0.1842f));
    m_fVoiceGain->setBounds (proportionOfWidth (0.1796f), proportionOfHeight (0.0132f), proportionOfWidth (0.0524f), proportionOfHeight (0.4737f));
    m_fPitchMod->setBounds (proportionOfWidth (0.2530f), proportionOfHeight (0.0132f), proportionOfWidth (0.0524f), proportionOfHeight (0.4737f));
    label35->setBounds (proportionOfWidth (0.1647f), proportionOfHeight (0.4868f), proportionOfWidth (0.0838f), proportionOfHeight (0.1447f));
    label36->setBounds (proportionOfWidth (0.2365f), proportionOfHeight (0.4868f), proportionOfWidth (0.0838f), proportionOfHeight (0.1447f));
    label15->setBounds (proportionOfWidth (0.1722f), proportionOfHeight (0.7105f), proportionOfWidth (0.0838f), proportionOfHeight (0.1447f));
    m_bLegatoMode->setBounds (proportionOfWidth (0.1482f), proportionOfHeight (0.6711f), proportionOfWidth (0.0239f), proportionOfHeight (0.2105f));
    c_LabelVersion->setBounds (proportionOfWidth (0.7470f), proportionOfHeight (0.1579f), proportionOfWidth (0.2275f), proportionOfHeight (0.1184f));
    c_iconSidePanel->setBounds (proportionOfWidth (0.7066f), proportionOfHeight (0.6316f), proportionOfWidth (0.0524f), proportionOfHeight (0.2237f));
    c_VUmeter->setBounds (proportionOfWidth (0.3293f), proportionOfHeight (0.0789f), proportionOfWidth (0.0464f), proportionOfHeight (0.7895f));
    c_groupComponent->setBounds (proportionOfWidth (0.2410f), proportionOfHeight (0.6974f), proportionOfWidth (0.0838f), proportionOfHeight (0.1974f));
    c_DBG_dump_file->setBounds (proportionOfWidth (0.8623f), proportionOfHeight (0.7368f), proportionOfWidth (0.0239f), proportionOfHeight (0.2105f));
    c_DBG_dump_file_label->setBounds (proportionOfWidth (0.8862f), proportionOfHeight (0.7763f), proportionOfWidth (0.1003f), proportionOfHeight (0.1447f));
    oscilloscopeOLG2D->setBounds (proportionOfWidth (0.3847f), proportionOfHeight (0.0789f), proportionOfWidth (0.3114f), proportionOfHeight (0.7895f));
    //[UserResized] Add your own custom resize handling here..

	c_voices->setBounds(c_groupComponent->proportionOfWidth(0.02f), c_groupComponent->proportionOfHeight(0.02f), c_groupComponent->proportionOfWidth(0.96f), c_groupComponent->proportionOfHeight(0.96f));
	//c_underruns->setBounds(c_groupComponent->proportionOfWidth(0.52f), c_groupComponent->proportionOfHeight(0.02f), c_groupComponent->proportionOfWidth(0.46f), c_groupComponent->proportionOfHeight(0.96f));
    //[/UserResized]
}

void VASTMasterVoicingComponent::sliderValueChanged (juce::Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == m_fMasterVolumedB.get())
    {
        //[UserSliderCode_m_fMasterVolumedB] -- add your slider handling code here..
        //[/UserSliderCode_m_fMasterVolumedB]
    }
    else if (sliderThatWasMoved == m_fMasterTune.get())
    {
        //[UserSliderCode_m_fMasterTune] -- add your slider handling code here..
        //[/UserSliderCode_m_fMasterTune]
    }
    else if (sliderThatWasMoved == m_fVoiceGain.get())
    {
        //[UserSliderCode_m_fVoiceGain] -- add your slider handling code here..
        //[/UserSliderCode_m_fVoiceGain]
    }
    else if (sliderThatWasMoved == m_fPitchMod.get())
    {
        //[UserSliderCode_m_fPitchMod] -- add your slider handling code here..
        //[/UserSliderCode_m_fPitchMod]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void VASTMasterVoicingComponent::comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == m_uPolyMode.get())
    {
        //[UserComboBoxCode_m_uPolyMode] -- add your combo box handling code here..
        //[/UserComboBoxCode_m_uPolyMode]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}

void VASTMasterVoicingComponent::buttonClicked (juce::Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
	if (buttonThatWasClicked == c_iconSidePanel.get()) {
        
        //un-lazy load all
        if (!mb_unlazy) {
            int currIdx = myEditor->vaporizerComponent->getTabbedComponent()->getCurrentTabIndex();
            myEditor->vaporizerComponent->getTabbedComponent()->currentTabChanged(VASTTabbedComponent::TabSequence::FILTER,"");
            myEditor->vaporizerComponent->getTabbedComponent()->currentTabChanged(VASTTabbedComponent::TabSequence::WTEDITOR,"");
            myEditor->vaporizerComponent->getTabbedComponent()->currentTabChanged(VASTTabbedComponent::TabSequence::LFOMSEG,"");
            myEditor->vaporizerComponent->getTabbedComponent()->currentTabChanged(VASTTabbedComponent::TabSequence::MATRIX,"");
            myEditor->vaporizerComponent->getTabbedComponent()->currentTabChanged(VASTTabbedComponent::TabSequence::FX,"");
            myEditor->vaporizerComponent->getTabbedComponent()->currentTabChanged(VASTTabbedComponent::TabSequence::ARP,"");
            myEditor->vaporizerComponent->getTabbedComponent()->currentTabChanged(VASTTabbedComponent::TabSequence::PRESET,"");
            myEditor->vaporizerComponent->getTabbedComponent()->setCurrentTabIndex(currIdx);
            mb_unlazy = true;
        }
        
		bool isShowing = myEditor->vaporizerComponent->getSidePanel()->isPanelShowing();
		myEditor->vaporizerComponent->getSidePanel()->showOrHide(!isShowing); //toggle it
	}

    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == m_bLegatoMode.get())
    {
        //[UserButtonCode_m_bLegatoMode] -- add your button handler code here..
        //[/UserButtonCode_m_bLegatoMode]
    }
    else if (buttonThatWasClicked == c_DBG_dump_file.get())
    {
        //[UserButtonCode_c_DBG_dump_file] -- add your button handler code here..
		myProcessor->m_pVASTXperience.m_Set.m_bShallDump = buttonThatWasClicked->getToggleState();
        //[/UserButtonCode_c_DBG_dump_file]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
/*
Label* VASTMasterVoicingComponent::getComponentCUnderruns() {
	return c_underruns.get();
}
*/

Label* VASTMasterVoicingComponent::getComponentCVoices() {
	return c_voices.get();
}

void VASTMasterVoicingComponent::initAll()
{
    m_initDone = true;
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="VASTMasterVoicingComponent"
                 componentName="" parentClasses="public Component" constructorParams="AudioProcessorEditor *editor, AudioProcessor* processor"
                 variableInitialisers="myEditor((VASTAudioProcessorEditor*)editor), myProcessor((VASTAudioProcessor*)processor)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="668" initialHeight="76">
  <BACKGROUND backgroundColour="ff0b0b0b">
    <RECT pos="0 0 0M 0M" fill="linear: -1.198% -10.526%, 100.923% 105.263%, 0=ff171818, 1=ff0b0b0b"
          hasStroke="0"/>
    <RECT pos="1.796% 48.684% 30.539% 14.474%" fill="linear: 1.942% 51.064%, 56.287% 51.064%, 0=ff3a4145, 1=ff33393c"
          hasStroke="0"/>
    <RECT pos="38.174% 5.263% 31.737% 84.211%" fill="solid: ff000000" hasStroke="1"
          stroke="2, mitered, butt" strokeColour="solid: ff000000"/>
  </BACKGROUND>
  <SLIDER name="m_fMasterVolumedB" id="ef175312584cdc7e" memberName="m_fMasterVolumedB"
          virtualName="VASTParameterSlider" explicitFocusOrder="0" pos="2.545% 1.316% 5.24% 47.368%"
          tooltip="Master volume in dB. Parameter is saved as well with preset."
          textboxtext="ffc9c9c9" textboxbkgd="ffffff" textboxoutline="808080"
          min="-60.0" max="20.0" int="0.01" style="RotaryVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="30"
          textBoxHeight="14" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="m_fMasterTune" id="9c7c2357b5acb2af" memberName="m_fMasterTune"
          virtualName="VASTParameterSlider" explicitFocusOrder="0" pos="9.88% 1.316% 5.24% 47.368%"
          tooltip="Master tune in hz. Parameter is saved as well with preset."
          textboxtext="ffc9c9c9" textboxbkgd="ffffff" textboxoutline="808080"
          min="400.0" max="480.0" int="0.01" style="RotaryVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="30"
          textBoxHeight="14" skewFactor="1.0" needsCallback="1"/>
  <LABEL name="new label" id="ba806663228eb0fb" memberName="label82" virtualName=""
         explicitFocusOrder="0" pos="1.497% 48.684% 8.383% 14.474%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="VOLUME" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Syntax"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <LABEL name="new label" id="439816a303475598" memberName="label120"
         virtualName="" explicitFocusOrder="0" pos="8.084% 48.684% 8.383% 14.474%"
         textCol="ffe2e2e2" edTextCol="ff000000" edBkgCol="0" labelText="TUNE"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Syntax" fontsize="11.0" kerning="0.0" bold="0" italic="0"
         justification="36"/>
  <COMBOBOX name="m_uPolyMode" id="fe63384085e3527a" memberName="m_uPolyMode"
            virtualName="VASTParameterComboBox" explicitFocusOrder="0" pos="1.796% 68.421% 11.976% 18.421%"
            tooltip="Mono mode - one note at at time&#10;Poly4 mode - four notes at at time&#10;Poly16 mode - onemax. 16 notes at at time"
            editable="0" layout="33" items="" textWhenNonSelected="POLY16"
            textWhenNoItems="POLY16"/>
  <SLIDER name="m_fVoiceGain" id="a6ace012654bfbf8" memberName="m_fVoiceGain"
          virtualName="VASTParameterSlider" explicitFocusOrder="0" pos="17.964% 1.316% 5.24% 47.368%"
          tooltip="Voice gain percentage for velocity modulation" textboxtext="ffc9c9c9"
          textboxbkgd="ffffff" textboxoutline="808080" min="0.0" max="100.0"
          int="0.0" style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="30" textBoxHeight="14" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="m_fPitchMod" id="2dbc69eaa83f8689" memberName="m_fPitchMod"
          virtualName="VASTParameterSlider" explicitFocusOrder="0" pos="25.299% 1.316% 5.24% 47.368%"
          tooltip="Voice pitch for modulation" textboxtext="ffc9c9c9" textboxbkgd="ffffff"
          textboxoutline="808080" min="400.0" max="480.0" int="0.01" style="RotaryVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="30"
          textBoxHeight="14" skewFactor="1.0" needsCallback="1"/>
  <LABEL name="new label" id="2cfe52c1adb94b3f" memberName="label35" virtualName=""
         explicitFocusOrder="0" pos="16.467% 48.684% 8.383% 14.474%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="GAINM" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Syntax"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <LABEL name="new label" id="9bd6d3bd8f8f7e21" memberName="label36" virtualName=""
         explicitFocusOrder="0" pos="23.653% 48.684% 8.383% 14.474%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="PITCHM" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Syntax"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <LABEL name="new label" id="91985a39bf01aa31" memberName="label15" virtualName=""
         explicitFocusOrder="0" pos="17.216% 71.053% 8.383% 14.474%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="LEGATO" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Syntax"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <TOGGLEBUTTON name="m_bLegatoMode" id="2c55c34f9f661244" memberName="m_bLegatoMode"
                virtualName="VASTParameterButton" explicitFocusOrder="0" pos="14.82% 67.105% 2.395% 21.053%"
                tooltip="Legato mode for mono voicing" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <LABEL name="c_LabelVersion" id="1e6b169afbf197f4" memberName="c_LabelVersion"
         virtualName="" explicitFocusOrder="0" pos="74.701% 15.789% 22.754% 11.842%"
         textCol="80adbac6" edTextCol="ff000000" edBkgCol="0" labelText="v0.00"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Syntax" fontsize="11.0" kerning="0.0" bold="0" italic="0"
         justification="34"/>
  <GENERICCOMPONENT name="c_iconSidePanel" id="707388016693c14f" memberName="c_iconSidePanel"
                    virtualName="" explicitFocusOrder="0" pos="70.659% 63.158% 5.24% 22.368%"
                    class="VASTDrawableButton" params="&quot;c_iconSidePanel&quot;, sidepanel_svg, sidepanel_svgSize, &quot;Show modulation sources side panel&quot;"/>
  <GENERICCOMPONENT name="c_VUmeter" id="b40b33d5023c6d4d" memberName="c_VUmeter"
                    virtualName="" explicitFocusOrder="0" pos="32.934% 7.895% 4.641% 78.947%"
                    class="VASTVUMeter" params=""/>
  <GROUPCOMPONENT name="c_groupComponent" id="40a1108ce7d9b939" memberName="c_groupComponent"
                  virtualName="" explicitFocusOrder="1" pos="24.102% 69.737% 8.383% 19.737%"
                  outlinecol="8e989b" title="" textpos="33"/>
  <TOGGLEBUTTON name="c_DBG_dump_file" id="a4321db1cdb5fe11" memberName="c_DBG_dump_file"
                virtualName="" explicitFocusOrder="0" pos="86.228% 73.684% 2.395% 21.053%"
                buttonText="" connectedEdges="0" needsCallback="1" radioGroupId="0"
                state="0"/>
  <LABEL name="c_DBG_dump_file_label" id="ed3a463c38e856cf" memberName="c_DBG_dump_file_label"
         virtualName="" explicitFocusOrder="0" pos="88.623% 77.632% 10.03% 14.474%"
         textCol="ffe2e2e2" edTextCol="ff000000" edBkgCol="0" labelText="DBG: Dump File"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Syntax" fontsize="11.0" kerning="0.0" bold="0" italic="0"
         justification="33"/>
  <GENERICCOMPONENT name="oscilloscopeOLG2D" id="1427efb1d60dd6e5" memberName="oscilloscopeOLG2D"
                    virtualName="" explicitFocusOrder="0" pos="38.473% 7.895% 31.138% 78.947%"
                    class="VASTOscilloscopeOGL2D" params="myEditor, &amp;myProcessor-&gt;m_pVASTXperience.oscilloscopeRingBuffer"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif

//==============================================================================
// Binary resources - be careful not to edit any of these sections!

// JUCER_RESOURCE: sidepanel_svg, 1000, "../../Resources/Icons/sidepanel.svg"
static const unsigned char resource_VASTMasterVoicingComponent_sidepanel_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,70,45,
56,34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,32,119,105,100,116,104,61,34,50,49,112,120,34,32,104,101,105,103,104,116,61,34,50,48,112,120,34,32,118,105,101,119,
66,111,120,61,34,48,32,48,32,50,49,32,50,48,34,32,118,101,114,115,105,111,110,61,34,49,46,49,34,32,120,109,108,110,115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,
47,115,118,103,34,32,120,109,108,110,115,58,120,108,105,110,107,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,120,108,105,110,107,34,62,10,32,32,32,32,60,33,45,
45,32,71,101,110,101,114,97,116,111,114,58,32,83,107,101,116,99,104,32,51,46,56,46,49,32,40,50,57,54,56,55,41,32,45,32,104,116,116,112,58,47,47,119,119,119,46,98,111,104,101,109,105,97,110,99,111,100,
105,110,103,46,99,111,109,47,115,107,101,116,99,104,32,45,45,62,10,32,32,32,32,60,116,105,116,108,101,62,119,105,110,100,111,119,95,112,108,117,115,32,91,35,49,52,54,54,93,60,47,116,105,116,108,101,62,
10,32,32,32,32,60,100,101,115,99,62,67,114,101,97,116,101,100,32,119,105,116,104,32,83,107,101,116,99,104,46,60,47,100,101,115,99,62,10,32,32,32,32,60,100,101,102,115,62,60,47,100,101,102,115,62,10,32,
32,32,32,60,103,32,105,100,61,34,80,97,103,101,45,49,34,32,115,116,114,111,107,101,61,34,110,111,110,101,34,32,115,116,114,111,107,101,45,119,105,100,116,104,61,34,49,34,32,102,105,108,108,61,34,110,111,
110,101,34,32,102,105,108,108,45,114,117,108,101,61,34,101,118,101,110,111,100,100,34,62,10,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,68,114,105,98,98,98,108,101,45,68,97,114,107,45,80,114,101,118,
105,101,119,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,45,50,49,57,46,48,48,48,48,48,48,44,32,45,52,52,48,46,48,48,48,48,48,48,41,34,32,102,105,108,108,61,34,35,
70,70,70,70,70,70,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,105,99,111,110,115,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,53,54,46,48,
48,48,48,48,48,44,32,49,54,48,46,48,48,48,48,48,48,41,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,60,112,97,116,104,32,100,61,34,77,49,56,49,46,57,44,50,56,50,32,76,49,56,49,46,57,44,50,56,
48,32,76,49,55,57,46,56,44,50,56,48,32,76,49,55,57,46,56,44,50,56,50,32,76,49,55,55,46,55,44,50,56,50,32,76,49,55,55,46,55,44,50,56,52,32,76,49,55,57,46,56,44,50,56,52,32,76,49,55,57,46,56,44,50,56,54,
32,76,49,56,49,46,57,44,50,56,54,32,76,49,56,49,46,57,44,50,56,52,32,76,49,56,52,44,50,56,52,32,76,49,56,52,44,50,56,50,32,76,49,56,49,46,57,44,50,56,50,32,90,32,77,49,55,57,46,56,44,50,56,56,32,76,49,
56,49,46,57,44,50,56,56,32,76,49,56,49,46,57,44,51,48,48,32,76,49,54,51,44,51,48,48,32,76,49,54,51,44,50,56,50,32,76,49,55,53,46,54,44,50,56,50,32,76,49,55,53,46,54,44,50,56,52,32,76,49,54,53,46,49,44,
50,56,52,32,76,49,54,53,46,49,44,50,57,56,32,76,49,55,57,46,56,44,50,57,56,32,76,49,55,57,46,56,44,50,56,56,32,90,34,32,105,100,61,34,119,105,110,100,111,119,95,112,108,117,115,45,91,35,49,52,54,54,93,
34,62,60,47,112,97,116,104,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,60,47,103,62,10,60,47,115,118,103,62,0,0};

const char* VASTMasterVoicingComponent::sidepanel_svg = (const char*) resource_VASTMasterVoicingComponent_sidepanel_svg;
const int VASTMasterVoicingComponent::sidepanel_svgSize = 1000;


//[EndFile] You can add extra defines here...
//[/EndFile]

