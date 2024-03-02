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

//[Headers] You can add your own extra header files here...
#include "VASTControls/VASTParameterComboBox.h"
#include "VASTControls/VASTParameterSlider.h"
#include "VASTControls/VASTParameterButton.h"
#include "VASTAudioProcessor.h"
#include "VASTAudioProcessorEditor.h"
//[/Headers]

#include "VASTSidePanelComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
VASTSidePanelComponent::VASTSidePanelComponent (AudioProcessorEditor *editor, AudioProcessor* processor)
    : myEditor((VASTAudioProcessorEditor*)editor), myProcessor((VASTAudioProcessor*)processor)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    c_dd_Velocity.reset (new VASTDragSource (1,"c_dd_Velocity", "VELOC","Drag modulation source to parameter control that you want to modulate. Velocity is the force with which a note is played."));
    addAndMakeVisible (c_dd_Velocity.get());
    c_dd_Velocity->setName ("c_dd_Velocity");

    c_dd_ModWheel.reset (new VASTDragSource (3,"c_dd_ModWheel", "MODWH","Drag modulation source to parameter control that you want to modulate. The ModWheel modulation source is linked to MIDI CC 1. Please make sure that this is properly mapped in your DAW."));
    addAndMakeVisible (c_dd_ModWheel.get());
    c_dd_ModWheel->setName ("c_dd_ModWheel");

    c_dd_KeyTrack.reset (new VASTDragSource (21,"c_dd_KeyTrack", "KYTRK","Drag modulation source to parameter control that you want to modulate. KeyTrack distributes the MIDI notes from c0 to c5 to a modulation range from -1 to 1."));
    addAndMakeVisible (c_dd_KeyTrack.get());
    c_dd_KeyTrack->setName ("c_dd_KeyTrack");

    c_dd_Aftertouch.reset (new VASTDragSource (2,"c_dd_Aftertouch", "ATOUC","Drag modulation source to parameter control that you want to modulate. The plug-in supports both channel and polyphonic aftertouch."));
    addAndMakeVisible (c_dd_Aftertouch.get());
    c_dd_Aftertouch->setName ("c_dd_Aftertouch");

    m_fCustomModulator1.reset (new VASTParameterSlider ("m_fCustomModulator1"));
    addAndMakeVisible (m_fCustomModulator1.get());
    m_fCustomModulator1->setTooltip (TRANS ("Custom modulator 1"));
    m_fCustomModulator1->setRange (0, 100, 0.01);
    m_fCustomModulator1->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    m_fCustomModulator1->setTextBoxStyle (juce::Slider::NoTextBox, false, 30, 14);
    m_fCustomModulator1->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffc9c9c9));
    m_fCustomModulator1->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0x00ffffff));
    m_fCustomModulator1->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x00808080));
    m_fCustomModulator1->addListener (this);

    m_fCustomModulator2.reset (new VASTParameterSlider ("m_fCustomModulator2"));
    addAndMakeVisible (m_fCustomModulator2.get());
    m_fCustomModulator2->setTooltip (TRANS ("Custom modulator 2"));
    m_fCustomModulator2->setRange (0, 100, 0.01);
    m_fCustomModulator2->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    m_fCustomModulator2->setTextBoxStyle (juce::Slider::NoTextBox, false, 30, 14);
    m_fCustomModulator2->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffc9c9c9));
    m_fCustomModulator2->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0x00ffffff));
    m_fCustomModulator2->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x00808080));
    m_fCustomModulator2->addListener (this);

    m_fCustomModulator3.reset (new VASTParameterSlider ("m_fCustomModulator3"));
    addAndMakeVisible (m_fCustomModulator3.get());
    m_fCustomModulator3->setTooltip (TRANS ("Custom modulator 3"));
    m_fCustomModulator3->setRange (0, 100, 0.01);
    m_fCustomModulator3->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    m_fCustomModulator3->setTextBoxStyle (juce::Slider::NoTextBox, false, 30, 14);
    m_fCustomModulator3->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffc9c9c9));
    m_fCustomModulator3->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0x00ffffff));
    m_fCustomModulator3->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x00808080));
    m_fCustomModulator3->addListener (this);

    m_fCustomModulator4.reset (new VASTParameterSlider ("m_fCustomModulator4"));
    addAndMakeVisible (m_fCustomModulator4.get());
    m_fCustomModulator4->setTooltip (TRANS ("Custom modulator 4"));
    m_fCustomModulator4->setRange (0, 100, 0.01);
    m_fCustomModulator4->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    m_fCustomModulator4->setTextBoxStyle (juce::Slider::NoTextBox, false, 30, 14);
    m_fCustomModulator4->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffc9c9c9));
    m_fCustomModulator4->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0x00ffffff));
    m_fCustomModulator4->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x00808080));
    m_fCustomModulator4->addListener (this);

    c_dd_MSEG1Env.reset (new VASTDragSource (4,"c_dd_MSEG1Env", "MSEG1","Drag modulation source to parameter control that you want to modulate. Use the editors on the LFO MSEG ENV tab to define MSEG envelopes."));
    addAndMakeVisible (c_dd_MSEG1Env.get());
    c_dd_MSEG1Env->setName ("c_dd_MSEG1Env");

    c_dd_MSEG2Env.reset (new VASTDragSource (5,"c_dd_MSEG2Env", "MSEG2","Drag modulation source to parameter control that you want to modulate. Use the editors on the LFO MSEG ENV tab to define MSEG envelopes."));
    addAndMakeVisible (c_dd_MSEG2Env.get());
    c_dd_MSEG2Env->setName ("c_dd_MSEG2Env");

    c_dd_MSEG3Env.reset (new VASTDragSource (6,"c_dd_MSEG3Env", "MSEG3","Drag modulation source to parameter control that you want to modulate. Use the editors on the LFO MSEG ENV tab to define MSEG envelopes."));
    addAndMakeVisible (c_dd_MSEG3Env.get());
    c_dd_MSEG3Env->setName ("c_dd_MSEG3Env");

    c_dd_MSEG4Env.reset (new VASTDragSource (7,"c_dd_MSEG4Env", "MSEG4","Drag modulation source to parameter control that you want to modulate. Use the editors on the LFO MSEG ENV tab to define MSEG envelopes."));
    addAndMakeVisible (c_dd_MSEG4Env.get());
    c_dd_MSEG4Env->setName ("c_dd_MSEG4Env");

    c_dd_MSEG5Env.reset (new VASTDragSource (8,"c_dd_MSEG5Env", "MSEG5","Drag modulation source to parameter control that you want to modulate. Use the editors on the LFO MSEG ENV tab to define MSEG envelopes."));
    addAndMakeVisible (c_dd_MSEG5Env.get());
    c_dd_MSEG5Env->setName ("c_dd_MSEG5Env");

    c_dd_LFO1.reset (new VASTDragSource (9,"c_dd_LFO1", "LFO1","Drag modulation source to parameter control that you want to modulate. Use the editors on the LFO MSEG ENV tab to define LFO shapes and synchronization."));
    addAndMakeVisible (c_dd_LFO1.get());
    c_dd_LFO1->setName ("c_dd_LFO1");

    c_dd_LFO2.reset (new VASTDragSource (10,"c_dd_LFO2", "LFO2","Drag modulation source to parameter control that you want to modulate. Use the editors on the LFO MSEG ENV tab to define LFO shapes and synchronization."));
    addAndMakeVisible (c_dd_LFO2.get());
    c_dd_LFO2->setName ("c_dd_LFO2");

    c_dd_LFO3.reset (new VASTDragSource (11,"c_dd_LFO3", "LFO3","Drag modulation source to parameter control that you want to modulate. Use the editors on the LFO MSEG ENV tab to define LFO shapes and synchronization."));
    addAndMakeVisible (c_dd_LFO3.get());
    c_dd_LFO3->setName ("c_dd_LFO3");

    c_dd_StepSeq2.reset (new VASTDragSource (15,"c_dd_StepSeq2", "STSEQ2","Drag modulation source to parameter control that you want to modulate. Use the editors on the ARP tab to edit step sequencer shapes and synchronization."));
    addAndMakeVisible (c_dd_StepSeq2.get());
    c_dd_StepSeq2->setName ("c_dd_StepSeq2");

    c_dd_StepSeq3.reset (new VASTDragSource (16,"c_dd_StepSeq3", "STSEQ3","Drag modulation source to parameter control that you want to modulate. Use the editors on the ARP tab to edit step sequencer shapes and synchronization."));
    addAndMakeVisible (c_dd_StepSeq3.get());
    c_dd_StepSeq3->setName ("c_dd_StepSeq3");

    c_dd_StepSeq1.reset (new VASTDragSource (14,"c_dd_StepSeq1", "STSEQ1","Drag modulation source to parameter control that you want to modulate. Use the editors on the ARP tab to edit step sequencer shapes and synchronization."));
    addAndMakeVisible (c_dd_StepSeq1.get());
    c_dd_StepSeq1->setName ("c_dd_StepSeq1");

    c_dd_InputEnvelope.reset (new VASTDragSource (23,"c_dd_InputEnvelope", "INENV","Drag modulation source to parameter control that you want to modulate. The input envelope modualtion sources processes the audio input signal the plug-in with an envelope follower. This can be used to modulate parameters."));
    addAndMakeVisible (c_dd_InputEnvelope.get());
    c_dd_InputEnvelope->setName ("c_dd_InputEnvelope");

    c_dd_CustomModulator1.reset (new VASTDragSource (17,"c_dd_CustomModulator1", "Custom1","Drag modulation source to parameter control that you want to modulate. Custom modulatrors are macros that shall predefine parameters that might be automated from within the DAW. You can change the name by clicking on the text. You can also link the mod. wheel to one of the custome modulatiors in setting."));
    addAndMakeVisible (c_dd_CustomModulator1.get());
    c_dd_CustomModulator1->setName ("c_dd_CustomModulator1");

    c_dd_CustomModulator2.reset (new VASTDragSource (18,"c_dd_CustomModulator2", "Custom2","Drag modulation source to parameter control that you want to modulate. Custom modulatrors are macros that shall predefine parameters that might be automated from within the DAW. You can change the name by clicking on the text. You can also link the mod. wheel to one of the custome modulatiors in setting."));
    addAndMakeVisible (c_dd_CustomModulator2.get());
    c_dd_CustomModulator2->setName ("c_dd_CustomModulator2");

    c_dd_CustomModulator3.reset (new VASTDragSource (19,"c_dd_CustomModulator3", "Custom3","Drag modulation source to parameter control that you want to modulate. Custom modulatrors are macros that shall predefine parameters that might be automated from within the DAW. You can change the name by clicking on the text. You can also link the mod. wheel to one of the custome modulatiors in setting."));
    addAndMakeVisible (c_dd_CustomModulator3.get());
    c_dd_CustomModulator3->setName ("c_dd_CustomModulator3");

    c_dd_CustomModulator4.reset (new VASTDragSource (20,"c_dd_CustomModulator4", "Custom4","Drag modulation source to parameter control that you want to modulate. Custom modulatrors are macros that shall predefine parameters that might be automated from within the DAW. You can change the name by clicking on the text. You can also link the mod. wheel to one of the custome modulatiors in setting."));
    addAndMakeVisible (c_dd_CustomModulator4.get());
    c_dd_CustomModulator4->setName ("c_dd_CustomModulator4");

    c_dd_LFO4.reset (new VASTDragSource (12,"c_dd_LFO4", "LFO4","Drag modulation source to parameter control that you want to modulate. Use the editors on the LFO MSEG ENV tab to define LFO shapes and synchronization."));
    addAndMakeVisible (c_dd_LFO4.get());
    c_dd_LFO4->setName ("c_dd_LFO4");

    c_dd_LFO5.reset (new VASTDragSource (13,"c_dd_LFO5", "LFO5","Drag modulation source to parameter control that you want to modulate. Use the editors on the LFO MSEG ENV tab to define LFO shapes and synchronization."));
    addAndMakeVisible (c_dd_LFO5.get());
    c_dd_LFO5->setName ("c_dd_LFO5");

    c_dd_RandomDriftSlow.reset (new VASTDragSource (24,"c_dd_RandomDriftSlow", "RNDDRIFT-","Drag modulation source to parameter control that you want to modulate. This adds slow random movement to the parameter within the modulation range."));
    addAndMakeVisible (c_dd_RandomDriftSlow.get());
    c_dd_RandomDriftSlow->setName ("c_dd_RandomDriftSlow");

    c_dd_RandomDriftFast.reset (new VASTDragSource (25,"c_dd_RandomDriftFast", "RNDDRIFT+","Drag modulation source to parameter control that you want to modulate. This adds very fast random movement to the parameter within the modulation range."));
    addAndMakeVisible (c_dd_RandomDriftFast.get());
    c_dd_RandomDriftFast->setName ("c_dd_RandomDriftFast");

    c_dd_MPETimbreMinMax.reset (new VASTDragSource (26,"c_dd_MPETimbreMinMax", "MPE-/+","Drag modulation source to parameter control that you want to modulate. This assigns the MPE timbre dimension (CC74) to a parameter. In this case the full value range is used to create values from -1 to 1."));
    addAndMakeVisible (c_dd_MPETimbreMinMax.get());
    c_dd_MPETimbreMinMax->setName ("c_dd_MPETimbreMinMax");

    c_dd_MPETimbreMax.reset (new VASTDragSource (27,"c_dd_MPETimbreMax", "MPE+","Drag modulation source to parameter control that you want to modulate. This assigns the MPE timbre dimension (CC74) to a parameter. In this case values from 64 to 127 are used to create values from -1 to 1."));
    addAndMakeVisible (c_dd_MPETimbreMax.get());
    c_dd_MPETimbreMax->setName ("c_dd_MPETimbreMax");

    c_dd_MPETimbreMin.reset (new VASTDragSource (28,"c_dd_MPETimbreMin", "MPE-","Drag modulation source to parameter control that you want to modulate. This assigns the MPE timbre dimension (CC74) to a parameter. In this case values from 0 to 64 are used to create values from -1 to 1."));
    addAndMakeVisible (c_dd_MPETimbreMin.get());
    c_dd_MPETimbreMin->setName ("c_dd_MPETimbreMin");


    //[UserPreSize]
	c_dd_Velocity->setAudioProcessor(*myProcessor, *myEditor);
	c_dd_Velocity->setRepaintsOnMouseActivity(false);
	c_dd_Aftertouch->setAudioProcessor(*myProcessor, *myEditor);
	c_dd_Aftertouch->setRepaintsOnMouseActivity(false);
	c_dd_ModWheel->setAudioProcessor(*myProcessor, *myEditor);
	c_dd_ModWheel->setRepaintsOnMouseActivity(false);
	c_dd_MSEG1Env->setAudioProcessor(*myProcessor, *myEditor);
	c_dd_MSEG1Env->setRepaintsOnMouseActivity(false);
	c_dd_MSEG2Env->setAudioProcessor(*myProcessor, *myEditor);
	c_dd_MSEG2Env->setRepaintsOnMouseActivity(false);
	c_dd_MSEG3Env->setAudioProcessor(*myProcessor, *myEditor);
	c_dd_MSEG3Env->setRepaintsOnMouseActivity(false);
	c_dd_MSEG4Env->setAudioProcessor(*myProcessor, *myEditor);
	c_dd_MSEG4Env->setRepaintsOnMouseActivity(false);
	c_dd_MSEG5Env->setAudioProcessor(*myProcessor, *myEditor);
	c_dd_MSEG5Env->setRepaintsOnMouseActivity(false);
	c_dd_LFO1->setAudioProcessor(*myProcessor, *myEditor);
	c_dd_LFO1->setRepaintsOnMouseActivity(false);
	c_dd_LFO2->setAudioProcessor(*myProcessor, *myEditor);
	c_dd_LFO2->setRepaintsOnMouseActivity(false);
	c_dd_LFO3->setAudioProcessor(*myProcessor, *myEditor);
	c_dd_LFO3->setRepaintsOnMouseActivity(false);
	c_dd_LFO4->setAudioProcessor(*myProcessor, *myEditor);
	c_dd_LFO4->setRepaintsOnMouseActivity(false);
	c_dd_LFO5->setAudioProcessor(*myProcessor, *myEditor);
	c_dd_LFO5->setRepaintsOnMouseActivity(false);
	c_dd_StepSeq1->setAudioProcessor(*myProcessor, *myEditor);
	c_dd_StepSeq1->setRepaintsOnMouseActivity(false);
	c_dd_StepSeq2->setAudioProcessor(*myProcessor, *myEditor);
	c_dd_StepSeq2->setRepaintsOnMouseActivity(false);
	c_dd_StepSeq3->setAudioProcessor(*myProcessor, *myEditor);
	c_dd_StepSeq3->setRepaintsOnMouseActivity(false);
	c_dd_KeyTrack->setAudioProcessor(*myProcessor, *myEditor);
	c_dd_KeyTrack->setRepaintsOnMouseActivity(false);
	c_dd_InputEnvelope->setAudioProcessor(*myProcessor, *myEditor);
	c_dd_InputEnvelope->setRepaintsOnMouseActivity(false);
	c_dd_RandomDriftSlow->setAudioProcessor(*myProcessor, *myEditor);
	c_dd_RandomDriftSlow->setRepaintsOnMouseActivity(false);
	c_dd_RandomDriftFast->setAudioProcessor(*myProcessor, *myEditor);
	c_dd_RandomDriftFast->setRepaintsOnMouseActivity(false);
	c_dd_MPETimbreMinMax->setAudioProcessor(*myProcessor, *myEditor);
	c_dd_MPETimbreMinMax->setRepaintsOnMouseActivity(false);
	c_dd_MPETimbreMin->setAudioProcessor(*myProcessor, *myEditor);
	c_dd_MPETimbreMin->setRepaintsOnMouseActivity(false);
	c_dd_MPETimbreMax->setAudioProcessor(*myProcessor, *myEditor);
	c_dd_MPETimbreMax->setRepaintsOnMouseActivity(false);

	c_dd_CustomModulator1->setAudioProcessor(*myProcessor, *myEditor);
	c_dd_CustomModulator1->setRepaintsOnMouseActivity(false);
	c_dd_CustomModulator2->setAudioProcessor(*myProcessor, *myEditor);
	c_dd_CustomModulator2->setRepaintsOnMouseActivity(false);
	c_dd_CustomModulator3->setAudioProcessor(*myProcessor, *myEditor);
	c_dd_CustomModulator3->setRepaintsOnMouseActivity(false);
	c_dd_CustomModulator4->setAudioProcessor(*myProcessor, *myEditor);
	c_dd_CustomModulator4->setRepaintsOnMouseActivity(false);

	//manage parameter bindings -> set ComponentID = Name and processor
	Array<Component*> children = this->getChildren();
	for (auto child : children) {
		if (child->getName().startsWith("m_")) {
			auto* aSlider = dynamic_cast<VASTParameterSlider*> (child);
			if (aSlider != nullptr) {
				aSlider->setAudioProcessor(*myProcessor);
				aSlider->bindParameter(myEditor, aSlider->getName(), VASTGUIRuntimeModel::GUIComponents::SidePanelComponent, 0);
			}
			auto* aCombobox = dynamic_cast<VASTParameterComboBox*> (child);
			if (aCombobox != nullptr) {
				aCombobox->setAudioProcessor(*myProcessor);
				aCombobox->bindParameter(myEditor, aCombobox->getName(), VASTGUIRuntimeModel::GUIComponents::SidePanelComponent, 0);
			}
			auto* aButton = dynamic_cast<VASTParameterButton*> (child);
			if (aButton != nullptr) {
				aButton->setAudioProcessor(*myProcessor);
				aButton->bindParameter(myEditor, aButton->getName(), VASTGUIRuntimeModel::GUIComponents::SidePanelComponent, 0);
			}
		}
	}

	setOpaque(true);
    //[/UserPreSize]

    setSize (144, 780);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

VASTSidePanelComponent::~VASTSidePanelComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    c_dd_Velocity = nullptr;
    c_dd_ModWheel = nullptr;
    c_dd_KeyTrack = nullptr;
    c_dd_Aftertouch = nullptr;
    m_fCustomModulator1 = nullptr;
    m_fCustomModulator2 = nullptr;
    m_fCustomModulator3 = nullptr;
    m_fCustomModulator4 = nullptr;
    c_dd_MSEG1Env = nullptr;
    c_dd_MSEG2Env = nullptr;
    c_dd_MSEG3Env = nullptr;
    c_dd_MSEG4Env = nullptr;
    c_dd_MSEG5Env = nullptr;
    c_dd_LFO1 = nullptr;
    c_dd_LFO2 = nullptr;
    c_dd_LFO3 = nullptr;
    c_dd_StepSeq2 = nullptr;
    c_dd_StepSeq3 = nullptr;
    c_dd_StepSeq1 = nullptr;
    c_dd_InputEnvelope = nullptr;
    c_dd_CustomModulator1 = nullptr;
    c_dd_CustomModulator2 = nullptr;
    c_dd_CustomModulator3 = nullptr;
    c_dd_CustomModulator4 = nullptr;
    c_dd_LFO4 = nullptr;
    c_dd_LFO5 = nullptr;
    c_dd_RandomDriftSlow = nullptr;
    c_dd_RandomDriftFast = nullptr;
    c_dd_MPETimbreMinMax = nullptr;
    c_dd_MPETimbreMax = nullptr;
    c_dd_MPETimbreMin = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void VASTSidePanelComponent::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff323e44));

    {
        int x = 0, y = 0, width = getWidth() - 0, height = getHeight() - 0;
        juce::Colour fillColour1 = juce::Colour (0xff212527), fillColour2 = juce::Colour (0xff0b0b0b);
        //[UserPaintCustomArguments] Customize the painting arguments here..
    		fillColour1 = myProcessor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscillatorComponentBackgroundGradientFrom);
    		fillColour2 = myProcessor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscillatorComponentBackgroundGradientTo);
        //[/UserPaintCustomArguments]
        g.setGradientFill (juce::ColourGradient (fillColour1,
                                             static_cast<float> (proportionOfWidth (-0.0556f)) - 0.0f + x,
                                             static_cast<float> (proportionOfHeight (0.5073f)) - 0.0f + y,
                                             fillColour2,
                                             static_cast<float> (proportionOfWidth (1.1111f)) - 0.0f + x,
                                             static_cast<float> (proportionOfHeight (0.5561f)) - 0.0f + y,
                                             false));
        g.fillRect (x, y, width, height);
    }

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void VASTSidePanelComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    c_dd_Velocity->setBounds (proportionOfWidth (0.1111f), proportionOfHeight (0.0115f), proportionOfWidth (0.8333f), proportionOfHeight (0.0256f));
    c_dd_ModWheel->setBounds (proportionOfWidth (0.1111f), proportionOfHeight (0.0397f), proportionOfWidth (0.8333f), proportionOfHeight (0.0256f));
    c_dd_KeyTrack->setBounds (proportionOfWidth (0.1111f), proportionOfHeight (0.0679f), proportionOfWidth (0.8333f), proportionOfHeight (0.0256f));
    c_dd_Aftertouch->setBounds (proportionOfWidth (0.1111f), proportionOfHeight (0.0961f), proportionOfWidth (0.8333f), proportionOfHeight (0.0256f));
    m_fCustomModulator1->setBounds (proportionOfWidth (0.5139f), proportionOfHeight (0.6654f), proportionOfWidth (0.2778f), proportionOfHeight (0.0513f));
    m_fCustomModulator2->setBounds (proportionOfWidth (0.5139f), proportionOfHeight (0.7474f), proportionOfWidth (0.2778f), proportionOfHeight (0.0513f));
    m_fCustomModulator3->setBounds (proportionOfWidth (0.5139f), proportionOfHeight (0.8295f), proportionOfWidth (0.2778f), proportionOfHeight (0.0513f));
    m_fCustomModulator4->setBounds (proportionOfWidth (0.5139f), proportionOfHeight (0.9115f), proportionOfWidth (0.2778f), proportionOfHeight (0.0513f));
    c_dd_MSEG1Env->setBounds (proportionOfWidth (0.1111f), proportionOfHeight (0.1244f), proportionOfWidth (0.8333f), proportionOfHeight (0.0256f));
    c_dd_MSEG2Env->setBounds (proportionOfWidth (0.1111f), proportionOfHeight (0.1526f), proportionOfWidth (0.8333f), proportionOfHeight (0.0256f));
    c_dd_MSEG3Env->setBounds (proportionOfWidth (0.1111f), proportionOfHeight (0.1808f), proportionOfWidth (0.8333f), proportionOfHeight (0.0256f));
    c_dd_MSEG4Env->setBounds (proportionOfWidth (0.1111f), proportionOfHeight (0.2090f), proportionOfWidth (0.8333f), proportionOfHeight (0.0256f));
    c_dd_MSEG5Env->setBounds (proportionOfWidth (0.1111f), proportionOfHeight (0.2372f), proportionOfWidth (0.8333f), proportionOfHeight (0.0256f));
    c_dd_LFO1->setBounds (proportionOfWidth (0.1111f), proportionOfHeight (0.2654f), proportionOfWidth (0.8333f), proportionOfHeight (0.0256f));
    c_dd_LFO2->setBounds (proportionOfWidth (0.1111f), proportionOfHeight (0.2936f), proportionOfWidth (0.8333f), proportionOfHeight (0.0256f));
    c_dd_LFO3->setBounds (proportionOfWidth (0.1111f), proportionOfHeight (0.3218f), proportionOfWidth (0.8333f), proportionOfHeight (0.0256f));
    c_dd_StepSeq2->setBounds (proportionOfWidth (0.1111f), proportionOfHeight (0.4346f), proportionOfWidth (0.8333f), proportionOfHeight (0.0256f));
    c_dd_StepSeq3->setBounds (proportionOfWidth (0.1111f), proportionOfHeight (0.4628f), proportionOfWidth (0.8333f), proportionOfHeight (0.0256f));
    c_dd_StepSeq1->setBounds (proportionOfWidth (0.1111f), proportionOfHeight (0.4064f), proportionOfWidth (0.8333f), proportionOfHeight (0.0256f));
    c_dd_InputEnvelope->setBounds (proportionOfWidth (0.1111f), proportionOfHeight (0.4910f), proportionOfWidth (0.8333f), proportionOfHeight (0.0256f));
    c_dd_CustomModulator1->setBounds (proportionOfWidth (0.1111f), proportionOfHeight (0.7167f), proportionOfWidth (0.8333f), proportionOfHeight (0.0256f));
    c_dd_CustomModulator2->setBounds (proportionOfWidth (0.1111f), proportionOfHeight (0.7987f), proportionOfWidth (0.8333f), proportionOfHeight (0.0256f));
    c_dd_CustomModulator3->setBounds (proportionOfWidth (0.1111f), proportionOfHeight (0.8808f), proportionOfWidth (0.8333f), proportionOfHeight (0.0256f));
    c_dd_CustomModulator4->setBounds (proportionOfWidth (0.1111f), proportionOfHeight (0.9628f), proportionOfWidth (0.8333f), proportionOfHeight (0.0256f));
    c_dd_LFO4->setBounds (proportionOfWidth (0.1181f), proportionOfHeight (0.3500f), proportionOfWidth (0.8333f), proportionOfHeight (0.0256f));
    c_dd_LFO5->setBounds (proportionOfWidth (0.1111f), proportionOfHeight (0.3782f), proportionOfWidth (0.8333f), proportionOfHeight (0.0256f));
    c_dd_RandomDriftSlow->setBounds (proportionOfWidth (0.1111f), proportionOfHeight (0.5192f), proportionOfWidth (0.8333f), proportionOfHeight (0.0256f));
    c_dd_RandomDriftFast->setBounds (proportionOfWidth (0.1111f), proportionOfHeight (0.5474f), proportionOfWidth (0.8333f), proportionOfHeight (0.0256f));
    c_dd_MPETimbreMinMax->setBounds (proportionOfWidth (0.1111f), proportionOfHeight (0.5744f), proportionOfWidth (0.8333f), proportionOfHeight (0.0256f));
    c_dd_MPETimbreMax->setBounds (proportionOfWidth (0.1111f), proportionOfHeight (0.6026f), proportionOfWidth (0.8333f), proportionOfHeight (0.0256f));
    c_dd_MPETimbreMin->setBounds (proportionOfWidth (0.1111f), proportionOfHeight (0.6308f), proportionOfWidth (0.8333f), proportionOfHeight (0.0256f));
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void VASTSidePanelComponent::sliderValueChanged (juce::Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == m_fCustomModulator1.get())
    {
        //[UserSliderCode_m_fCustomModulator1] -- add your slider handling code here..
        //[/UserSliderCode_m_fCustomModulator1]
    }
    else if (sliderThatWasMoved == m_fCustomModulator2.get())
    {
        //[UserSliderCode_m_fCustomModulator2] -- add your slider handling code here..
        //[/UserSliderCode_m_fCustomModulator2]
    }
    else if (sliderThatWasMoved == m_fCustomModulator3.get())
    {
        //[UserSliderCode_m_fCustomModulator3] -- add your slider handling code here..
        //[/UserSliderCode_m_fCustomModulator3]
    }
    else if (sliderThatWasMoved == m_fCustomModulator4.get())
    {
        //[UserSliderCode_m_fCustomModulator4] -- add your slider handling code here..
        //[/UserSliderCode_m_fCustomModulator4]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="VASTSidePanelComponent" componentName=""
                 parentClasses="public Component" constructorParams="AudioProcessorEditor *editor, AudioProcessor* processor"
                 variableInitialisers="myEditor((VASTAudioProcessorEditor*)editor), myProcessor((VASTAudioProcessor*)processor)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="144" initialHeight="780">
  <BACKGROUND backgroundColour="ff323e44">
    <RECT pos="0 0 0M 0M" fill="linear: -5.556% 50.732%, 111.111% 55.61%, 0=ff212527, 1=ff0b0b0b"
          hasStroke="0"/>
  </BACKGROUND>
  <GENERICCOMPONENT name="c_dd_Velocity" id="cb6672d81b2a4b00" memberName="c_dd_Velocity"
                    virtualName="" explicitFocusOrder="0" pos="11.111% 1.154% 83.333% 2.564%"
                    class="VASTDragSource" params="1,&quot;c_dd_Velocity&quot;, &quot;VELOC&quot;,&quot;Drag modulation source to parameter control that you want to modulate. Velocity is the force with which a note is played.&quot;"/>
  <GENERICCOMPONENT name="c_dd_ModWheel" id="6d43bd574ef52c2" memberName="c_dd_ModWheel"
                    virtualName="" explicitFocusOrder="0" pos="11.111% 3.974% 83.333% 2.564%"
                    class="VASTDragSource" params="3,&quot;c_dd_ModWheel&quot;, &quot;MODWH&quot;,&quot;Drag modulation source to parameter control that you want to modulate. The ModWheel modulation source is linked to MIDI CC 1. Please make sure that this is properly mapped in your DAW.&quot;"/>
  <GENERICCOMPONENT name="c_dd_KeyTrack" id="70d4ccb10d8f0065" memberName="c_dd_KeyTrack"
                    virtualName="" explicitFocusOrder="0" pos="11.111% 6.795% 83.333% 2.564%"
                    class="VASTDragSource" params="21,&quot;c_dd_KeyTrack&quot;, &quot;KYTRK&quot;,&quot;Drag modulation source to parameter control that you want to modulate. KeyTrack distributes the MIDI notes from c0 to c5 to a modulation range from -1 to 1.&quot;"/>
  <GENERICCOMPONENT name="c_dd_Aftertouch" id="8bdc0a14aeb18fa" memberName="c_dd_Aftertouch"
                    virtualName="" explicitFocusOrder="0" pos="11.111% 9.615% 83.333% 2.564%"
                    class="VASTDragSource" params="2,&quot;c_dd_Aftertouch&quot;, &quot;ATOUC&quot;,&quot;Drag modulation source to parameter control that you want to modulate. The plug-in supports both channel and polyphonic aftertouch.&quot;"/>
  <SLIDER name="m_fCustomModulator1" id="663798e54a450c23" memberName="m_fCustomModulator1"
          virtualName="VASTParameterSlider" explicitFocusOrder="0" pos="51.389% 66.538% 27.778% 5.128%"
          tooltip="Custom modulator 1" textboxtext="ffc9c9c9" textboxbkgd="ffffff"
          textboxoutline="808080" min="0.0" max="100.0" int="0.01" style="RotaryVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="30"
          textBoxHeight="14" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="m_fCustomModulator2" id="3a03ba6a13fc4c04" memberName="m_fCustomModulator2"
          virtualName="VASTParameterSlider" explicitFocusOrder="0" pos="51.389% 74.744% 27.778% 5.128%"
          tooltip="Custom modulator 2" textboxtext="ffc9c9c9" textboxbkgd="ffffff"
          textboxoutline="808080" min="0.0" max="100.0" int="0.01" style="RotaryVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="30"
          textBoxHeight="14" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="m_fCustomModulator3" id="36d1a4924a1ce788" memberName="m_fCustomModulator3"
          virtualName="VASTParameterSlider" explicitFocusOrder="0" pos="51.389% 82.949% 27.778% 5.128%"
          tooltip="Custom modulator 3" textboxtext="ffc9c9c9" textboxbkgd="ffffff"
          textboxoutline="808080" min="0.0" max="100.0" int="0.01" style="RotaryVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="30"
          textBoxHeight="14" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="m_fCustomModulator4" id="8cce8913f1d9600b" memberName="m_fCustomModulator4"
          virtualName="VASTParameterSlider" explicitFocusOrder="0" pos="51.389% 91.154% 27.778% 5.128%"
          tooltip="Custom modulator 4" textboxtext="ffc9c9c9" textboxbkgd="ffffff"
          textboxoutline="808080" min="0.0" max="100.0" int="0.01" style="RotaryVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="30"
          textBoxHeight="14" skewFactor="1.0" needsCallback="1"/>
  <GENERICCOMPONENT name="c_dd_MSEG1Env" id="828017bd94192143" memberName="c_dd_MSEG1Env"
                    virtualName="" explicitFocusOrder="0" pos="11.111% 12.436% 83.333% 2.564%"
                    class="VASTDragSource" params="4,&quot;c_dd_MSEG1Env&quot;, &quot;MSEG1&quot;,&quot;Drag modulation source to parameter control that you want to modulate. Use the editors on the LFO MSEG ENV tab to define MSEG envelopes.&quot;"/>
  <GENERICCOMPONENT name="c_dd_MSEG2Env" id="5afc3abc8a0a8240" memberName="c_dd_MSEG2Env"
                    virtualName="" explicitFocusOrder="0" pos="11.111% 15.256% 83.333% 2.564%"
                    class="VASTDragSource" params="5,&quot;c_dd_MSEG2Env&quot;, &quot;MSEG2&quot;,&quot;Drag modulation source to parameter control that you want to modulate. Use the editors on the LFO MSEG ENV tab to define MSEG envelopes.&quot;"/>
  <GENERICCOMPONENT name="c_dd_MSEG3Env" id="912ac9bbba7f2c06" memberName="c_dd_MSEG3Env"
                    virtualName="" explicitFocusOrder="0" pos="11.111% 18.077% 83.333% 2.564%"
                    class="VASTDragSource" params="6,&quot;c_dd_MSEG3Env&quot;, &quot;MSEG3&quot;,&quot;Drag modulation source to parameter control that you want to modulate. Use the editors on the LFO MSEG ENV tab to define MSEG envelopes.&quot;"/>
  <GENERICCOMPONENT name="c_dd_MSEG4Env" id="586676331140a6b4" memberName="c_dd_MSEG4Env"
                    virtualName="" explicitFocusOrder="0" pos="11.111% 20.897% 83.333% 2.564%"
                    class="VASTDragSource" params="7,&quot;c_dd_MSEG4Env&quot;, &quot;MSEG4&quot;,&quot;Drag modulation source to parameter control that you want to modulate. Use the editors on the LFO MSEG ENV tab to define MSEG envelopes.&quot;"/>
  <GENERICCOMPONENT name="c_dd_MSEG5Env" id="c13dcc5b28ff8fb3" memberName="c_dd_MSEG5Env"
                    virtualName="" explicitFocusOrder="0" pos="11.111% 23.718% 83.333% 2.564%"
                    class="VASTDragSource" params="8,&quot;c_dd_MSEG5Env&quot;, &quot;MSEG5&quot;,&quot;Drag modulation source to parameter control that you want to modulate. Use the editors on the LFO MSEG ENV tab to define MSEG envelopes.&quot;"/>
  <GENERICCOMPONENT name="c_dd_LFO1" id="c3da502617bd7321" memberName="c_dd_LFO1"
                    virtualName="" explicitFocusOrder="0" pos="11.111% 26.538% 83.333% 2.564%"
                    class="VASTDragSource" params="9,&quot;c_dd_LFO1&quot;, &quot;LFO1&quot;,&quot;Drag modulation source to parameter control that you want to modulate. Use the editors on the LFO MSEG ENV tab to define LFO shapes and synchronization.&quot;"/>
  <GENERICCOMPONENT name="c_dd_LFO2" id="d26c063ca2400c01" memberName="c_dd_LFO2"
                    virtualName="" explicitFocusOrder="0" pos="11.111% 29.359% 83.333% 2.564%"
                    class="VASTDragSource" params="10,&quot;c_dd_LFO2&quot;, &quot;LFO2&quot;,&quot;Drag modulation source to parameter control that you want to modulate. Use the editors on the LFO MSEG ENV tab to define LFO shapes and synchronization.&quot;"/>
  <GENERICCOMPONENT name="c_dd_LFO3" id="7710fd00290d0ee2" memberName="c_dd_LFO3"
                    virtualName="" explicitFocusOrder="0" pos="11.111% 32.179% 83.333% 2.564%"
                    class="VASTDragSource" params="11,&quot;c_dd_LFO3&quot;, &quot;LFO3&quot;,&quot;Drag modulation source to parameter control that you want to modulate. Use the editors on the LFO MSEG ENV tab to define LFO shapes and synchronization.&quot;"/>
  <GENERICCOMPONENT name="c_dd_StepSeq2" id="f144f9221b1a920" memberName="c_dd_StepSeq2"
                    virtualName="" explicitFocusOrder="0" pos="11.111% 43.462% 83.333% 2.564%"
                    class="VASTDragSource" params="15,&quot;c_dd_StepSeq2&quot;, &quot;STSEQ2&quot;,&quot;Drag modulation source to parameter control that you want to modulate. Use the editors on the ARP tab to edit step sequencer shapes and synchronization.&quot;"/>
  <GENERICCOMPONENT name="c_dd_StepSeq3" id="3ee5df82e4998631" memberName="c_dd_StepSeq3"
                    virtualName="" explicitFocusOrder="0" pos="11.111% 46.282% 83.333% 2.564%"
                    class="VASTDragSource" params="16,&quot;c_dd_StepSeq3&quot;, &quot;STSEQ3&quot;,&quot;Drag modulation source to parameter control that you want to modulate. Use the editors on the ARP tab to edit step sequencer shapes and synchronization.&quot;"/>
  <GENERICCOMPONENT name="c_dd_StepSeq1" id="477205ebe09cf6ef" memberName="c_dd_StepSeq1"
                    virtualName="" explicitFocusOrder="0" pos="11.111% 40.641% 83.333% 2.564%"
                    class="VASTDragSource" params="14,&quot;c_dd_StepSeq1&quot;, &quot;STSEQ1&quot;,&quot;Drag modulation source to parameter control that you want to modulate. Use the editors on the ARP tab to edit step sequencer shapes and synchronization.&quot;"/>
  <GENERICCOMPONENT name="c_dd_InputEnvelope" id="6c7596bdff021db0" memberName="c_dd_InputEnvelope"
                    virtualName="" explicitFocusOrder="0" pos="11.111% 49.103% 83.333% 2.564%"
                    class="VASTDragSource" params="23,&quot;c_dd_InputEnvelope&quot;, &quot;INENV&quot;,&quot;Drag modulation source to parameter control that you want to modulate. The input envelope modualtion sources processes the audio input signal the plug-in with an envelope follower. This can be used to modulate parameters.&quot;"/>
  <GENERICCOMPONENT name="c_dd_CustomModulator1" id="e11ebecaabadab4d" memberName="c_dd_CustomModulator1"
                    virtualName="" explicitFocusOrder="0" pos="11.111% 71.667% 83.333% 2.564%"
                    class="VASTDragSource" params="17,&quot;c_dd_CustomModulator1&quot;, &quot;Custom1&quot;,&quot;Drag modulation source to parameter control that you want to modulate. Custom modulatrors are macros that shall predefine parameters that might be automated from within the DAW. You can change the name by clicking on the text. You can also link the mod. wheel to one of the custome modulatiors in setting.&quot;"/>
  <GENERICCOMPONENT name="c_dd_CustomModulator2" id="df2d3c9fcb773f8b" memberName="c_dd_CustomModulator2"
                    virtualName="" explicitFocusOrder="0" pos="11.111% 79.872% 83.333% 2.564%"
                    class="VASTDragSource" params="18,&quot;c_dd_CustomModulator2&quot;, &quot;Custom2&quot;,&quot;Drag modulation source to parameter control that you want to modulate. Custom modulatrors are macros that shall predefine parameters that might be automated from within the DAW. You can change the name by clicking on the text. You can also link the mod. wheel to one of the custome modulatiors in setting.&quot;"/>
  <GENERICCOMPONENT name="c_dd_CustomModulator3" id="bef45674eb4b2223" memberName="c_dd_CustomModulator3"
                    virtualName="" explicitFocusOrder="0" pos="11.111% 88.077% 83.333% 2.564%"
                    class="VASTDragSource" params="19,&quot;c_dd_CustomModulator3&quot;, &quot;Custom3&quot;,&quot;Drag modulation source to parameter control that you want to modulate. Custom modulatrors are macros that shall predefine parameters that might be automated from within the DAW. You can change the name by clicking on the text. You can also link the mod. wheel to one of the custome modulatiors in setting.&quot;"/>
  <GENERICCOMPONENT name="c_dd_CustomModulator4" id="232de315b6088b26" memberName="c_dd_CustomModulator4"
                    virtualName="" explicitFocusOrder="0" pos="11.111% 96.282% 83.333% 2.564%"
                    class="VASTDragSource" params="20,&quot;c_dd_CustomModulator4&quot;, &quot;Custom4&quot;,&quot;Drag modulation source to parameter control that you want to modulate. Custom modulatrors are macros that shall predefine parameters that might be automated from within the DAW. You can change the name by clicking on the text. You can also link the mod. wheel to one of the custome modulatiors in setting.&quot;"/>
  <GENERICCOMPONENT name="c_dd_LFO4" id="845b2a4e90d7395" memberName="c_dd_LFO4"
                    virtualName="" explicitFocusOrder="0" pos="11.806% 35% 83.333% 2.564%"
                    class="VASTDragSource" params="12,&quot;c_dd_LFO4&quot;, &quot;LFO4&quot;,&quot;Drag modulation source to parameter control that you want to modulate. Use the editors on the LFO MSEG ENV tab to define LFO shapes and synchronization.&quot;"/>
  <GENERICCOMPONENT name="c_dd_LFO5" id="58e95d32bcc1eec7" memberName="c_dd_LFO5"
                    virtualName="" explicitFocusOrder="0" pos="11.111% 37.821% 83.333% 2.564%"
                    class="VASTDragSource" params="13,&quot;c_dd_LFO5&quot;, &quot;LFO5&quot;,&quot;Drag modulation source to parameter control that you want to modulate. Use the editors on the LFO MSEG ENV tab to define LFO shapes and synchronization.&quot;"/>
  <GENERICCOMPONENT name="c_dd_RandomDriftSlow" id="3dcca671d5325513" memberName="c_dd_RandomDriftSlow"
                    virtualName="" explicitFocusOrder="0" pos="11.111% 51.923% 83.333% 2.564%"
                    class="VASTDragSource" params="24,&quot;c_dd_RandomDriftSlow&quot;, &quot;RNDDRIFT-&quot;,&quot;Drag modulation source to parameter control that you want to modulate. This adds slow random movement to the parameter within the modulation range.&quot;"/>
  <GENERICCOMPONENT name="c_dd_RandomDriftFast" id="e37549028f2085b1" memberName="c_dd_RandomDriftFast"
                    virtualName="" explicitFocusOrder="0" pos="11.111% 54.744% 83.333% 2.564%"
                    class="VASTDragSource" params="25,&quot;c_dd_RandomDriftFast&quot;, &quot;RNDDRIFT+&quot;,&quot;Drag modulation source to parameter control that you want to modulate. This adds very fast random movement to the parameter within the modulation range.&quot;"/>
  <GENERICCOMPONENT name="c_dd_MPETimbreMinMax" id="1f4ff0120a5f132e" memberName="c_dd_MPETimbreMinMax"
                    virtualName="" explicitFocusOrder="0" pos="11.111% 57.436% 83.333% 2.564%"
                    class="VASTDragSource" params="26,&quot;c_dd_MPETimbreMinMax&quot;, &quot;MPE-/+&quot;,&quot;Drag modulation source to parameter control that you want to modulate. This assigns the MPE timbre dimension (CC74) to a parameter. In this case the full value range is used to create values from -1 to 1.&quot;"/>
  <GENERICCOMPONENT name="c_dd_MPETimbreMax" id="edc901223b69bf41" memberName="c_dd_MPETimbreMax"
                    virtualName="" explicitFocusOrder="0" pos="11.111% 60.256% 83.333% 2.564%"
                    class="VASTDragSource" params="27,&quot;c_dd_MPETimbreMax&quot;, &quot;MPE+&quot;,&quot;Drag modulation source to parameter control that you want to modulate. This assigns the MPE timbre dimension (CC74) to a parameter. In this case values from 64 to 127 are used to create values from -1 to 1.&quot;"/>
  <GENERICCOMPONENT name="c_dd_MPETimbreMin" id="733436e2755ee6a1" memberName="c_dd_MPETimbreMin"
                    virtualName="" explicitFocusOrder="0" pos="11.111% 63.077% 83.333% 2.564%"
                    class="VASTDragSource" params="28,&quot;c_dd_MPETimbreMin&quot;, &quot;MPE-&quot;,&quot;Drag modulation source to parameter control that you want to modulate. This assigns the MPE timbre dimension (CC74) to a parameter. In this case values from 0 to 64 are used to create values from -1 to 1.&quot;"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

