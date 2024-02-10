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
#include "../Engine/VASTEngineHeader.h"
#include "VASTAudioProcessor.h"
#include "VASTAudioProcessorEditor.h"
//[/Headers]

#include "VASTKeyboardComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
VASTKeyboardComponent::VASTKeyboardComponent (AudioProcessorEditor *editor, AudioProcessor* processor)
    : myEditor((VASTAudioProcessorEditor*)editor), myProcessor((VASTAudioProcessor*)processor)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    c_midiKeyboard.reset (new MidiKeyboardComponent (myProcessor->m_midiKeyboardState, MidiKeyboardComponent::horizontalKeyboard));
    addAndMakeVisible (c_midiKeyboard.get());
    c_midiKeyboard->setName ("c_midiKeyboard");

    c_pitchBend.reset (new VASTPitchbendSlider ("Pitchbend"));
    addAndMakeVisible (c_pitchBend.get());
    c_pitchBend->setName ("c_pitchBend");

    c_modWheel.reset (new VASTModwheelSlider ("Pitchbend"));
    addAndMakeVisible (c_modWheel.get());
    c_modWheel->setName ("c_modWheel");

    c_iBendRange.reset (new VASTSlider ("c_iBendRange"));
    addAndMakeVisible (c_iBendRange.get());
    c_iBendRange->setTooltip (TRANS ("Pitch bend wheel sensitivity in semitiones (up/down)"));
    c_iBendRange->setExplicitFocusOrder (1);
    c_iBendRange->setRange (1, 96, 1);
    c_iBendRange->setSliderStyle (juce::Slider::IncDecButtons);
    c_iBendRange->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 28, 20);

    label100.reset (new juce::Label ("new label",
                                     TRANS ("BEND RNG")));
    addAndMakeVisible (label100.get());
    label100->setFont (juce::Font ("Syntax", 11.00f, juce::Font::plain));
    label100->setJustificationType (juce::Justification::centred);
    label100->setEditable (false, false, false);
    label100->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label100->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label100->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label2.reset (new juce::Label ("new label",
                                   TRANS ("PORTAM")));
    addAndMakeVisible (label2.get());
    label2->setFont (juce::Font ("Syntax", 11.00f, juce::Font::plain));
    label2->setJustificationType (juce::Justification::centred);
    label2->setEditable (false, false, false);
    label2->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label2->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label2->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    m_fPortamento.reset (new VASTParameterSlider ("m_fPortamento"));
    addAndMakeVisible (m_fPortamento.get());
    m_fPortamento->setTooltip (TRANS ("Portamento time in ms - available in poly modes and mono mode"));
    m_fPortamento->setRange (0, 5000, 0.001);
    m_fPortamento->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    m_fPortamento->setTextBoxStyle (juce::Slider::NoTextBox, false, 30, 14);
    m_fPortamento->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffc9c9c9));
    m_fPortamento->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0x00ffffff));
    m_fPortamento->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x00808080));


    //[UserPreSize]

	c_midiKeyboard->setVelocity(1.0, true); // use mouse position for velocity
	c_midiKeyboard->setMidiChannelsToDisplay(0xffff); // display all channels
	c_midiKeyboard->setMidiChannel(2); // own channel 2
	c_midiKeyboard->setKeyPressBaseOctave(2); //checked
	c_midiKeyboard->setOpaque(true);

	c_pitchBend->setRange(Range<double>(-8192.f, 8192.f), 0.f);
	c_pitchBend->setDefaultValue(0.f);
	c_pitchBend->addListener(this);
	c_modWheel->setRange(Range<double>(0.f, 127.f), 0.f);
	c_modWheel->setDefaultValue(0.f);
	c_modWheel->addListener(this);

    updateMidiKeyboardCharLayout(); //initialize it
    updateMidiKeyboardBaseOctave();
    
    c_iBendRange->setDefaultValue(12);
	c_iBendRange->setValue(12);
	c_iBendRange->setIncDecButtonsMode(Slider::incDecButtonsDraggable_Vertical);
	c_iBendRange->setAudioProcessor(*myProcessor);
	c_iBendRange->addListener(this);

	//manage parameter bindings -> set ComponentID = Name and processor
	Array<Component*> children = this->getChildren();
	for (auto child : children) {
		if (child->getName().startsWith("m_")) {
			auto* aSlider = dynamic_cast<VASTParameterSlider*> (child);
			if (aSlider != nullptr) {
				aSlider->setAudioProcessor(*myProcessor);
				aSlider->bindParameter(aSlider->getName());
			}
			auto* aCombobox = dynamic_cast<VASTParameterComboBox*> (child);
			if (aCombobox != nullptr) {
				aCombobox->setAudioProcessor(*myProcessor);
				aCombobox->bindParameter(aCombobox->getName());
			}
			auto* aButton = dynamic_cast<VASTParameterButton*> (child);
			if (aButton != nullptr) {
				aButton->setAudioProcessor(*myProcessor);
				aButton->bindParameter(aButton->getName());
			}
		}
	}
	setOpaque(true);
	updateAll();
    startTimer(50);
    return; //dont call setSize
    //[/UserPreSize]

    setSize (1420, 88);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

VASTKeyboardComponent::~VASTKeyboardComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
	this->setLookAndFeel(nullptr);
    //[/Destructor_pre]

    c_midiKeyboard = nullptr;
    c_pitchBend = nullptr;
    c_modWheel = nullptr;
    c_iBendRange = nullptr;
    label100 = nullptr;
    label2 = nullptr;
    m_fPortamento = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void VASTKeyboardComponent::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff323e44));

    {
        int x = 0, y = 0, width = getWidth() - 0, height = getHeight() - 0;
        juce::Colour fillColour1 = juce::Colour (0xff212527), fillColour2 = juce::Colour (0xff0b0b0b);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        fillColour1 = myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colMasterVoicingComponentBackgroundGradientFrom);
        fillColour2 = myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colMasterVoicingComponentBackgroundGradientTo);

        if (myProcessor->m_keyboardHoldMode.load()) {
            fillColour1 = fillColour1.contrasting();
            fillColour2 = fillColour2.contrasting();
        }

        //[/UserPaintCustomArguments]
        g.setGradientFill (juce::ColourGradient (fillColour1,
                                             0.0f - 0.0f + x,
                                             0.0f - 0.0f + y,
                                             fillColour2,
                                             static_cast<float> (getWidth()) - 0.0f + x,
                                             static_cast<float> (getHeight()) - 0.0f + y,
                                             false));
        g.fillRect (x, y, width, height);
    }

    {
        float x = 0, y = 0;
        juce::Colour strokeColour = juce::Colour (0x20ffffff);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (strokeColour);
        g.strokePath (internalPath1, juce::PathStrokeType (1.000f), juce::AffineTransform::translation (x, y));
    }

    {
        float x = 0, y = 0;
        juce::Colour strokeColour = juce::Colour (0x20ffffff);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (strokeColour);
        g.strokePath (internalPath2, juce::PathStrokeType (1.000f), juce::AffineTransform::translation (x, y));
    }

    {
        float x = 0, y = 0;
        juce::Colour strokeColour = juce::Colour (0x20ffffff);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (strokeColour);
        g.strokePath (internalPath3, juce::PathStrokeType (1.000f), juce::AffineTransform::translation (x, y));
    }

    {
        int x = proportionOfWidth (0.0080f), y = proportionOfHeight (0.2614f), width = proportionOfWidth (0.0394f), height = proportionOfHeight (0.1250f);
        juce::Colour fillColour1 = juce::Colour (0xff3a4145), fillColour2 = juce::Colour (0xff33393c);
        //[UserPaintCustomArguments] Customize the painting arguments here..
    		fillColour1 = myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscillatorComponentHeaderGradientFrom);
    		fillColour2 = myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscillatorComponentHeaderGradientTo);
        //[/UserPaintCustomArguments]
        g.setGradientFill (juce::ColourGradient (fillColour1,
                                             static_cast<float> (proportionOfWidth (0.0056f)) - static_cast<float> (proportionOfWidth (0.0080f)) + x,
                                             static_cast<float> (proportionOfHeight (0.2727f)) - static_cast<float> (proportionOfHeight (0.2614f)) + y,
                                             fillColour2,
                                             static_cast<float> (proportionOfWidth (0.0507f)) - static_cast<float> (proportionOfWidth (0.0080f)) + x,
                                             static_cast<float> (proportionOfHeight (0.4546f)) - static_cast<float> (proportionOfHeight (0.2614f)) + y,
                                             false));
        g.fillRect (x, y, width, height);
    }

    {
        int x = proportionOfWidth (0.0080f), y = proportionOfHeight (0.8182f), width = proportionOfWidth (0.0394f), height = proportionOfHeight (0.1250f);
        juce::Colour fillColour1 = juce::Colour (0xff3a4145), fillColour2 = juce::Colour (0xff33393c);
        //[UserPaintCustomArguments] Customize the painting arguments here..
    		fillColour1 = myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscillatorComponentHeaderGradientFrom);
    		fillColour2 = myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscillatorComponentHeaderGradientTo);
        //[/UserPaintCustomArguments]
        g.setGradientFill (juce::ColourGradient (fillColour1,
                                             static_cast<float> (proportionOfWidth (0.0056f)) - static_cast<float> (proportionOfWidth (0.0080f)) + x,
                                             static_cast<float> (proportionOfHeight (0.7273f)) - static_cast<float> (proportionOfHeight (0.8182f)) + y,
                                             fillColour2,
                                             static_cast<float> (proportionOfWidth (0.0451f)) - static_cast<float> (proportionOfWidth (0.0080f)) + x,
                                             static_cast<float> (proportionOfHeight (0.9091f)) - static_cast<float> (proportionOfHeight (0.8182f)) + y,
                                             false));
        g.fillRect (x, y, width, height);
    }

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void VASTKeyboardComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    c_midiKeyboard->setBounds (proportionOfWidth (0.1127f), proportionOfHeight (0.0000f), proportionOfWidth (0.8873f), proportionOfHeight (1.0000f));
    c_pitchBend->setBounds (proportionOfWidth (0.0655f), proportionOfHeight (0.0341f), proportionOfWidth (0.0169f), proportionOfHeight (0.9318f));
    c_modWheel->setBounds (proportionOfWidth (0.0901f), proportionOfHeight (0.0341f), proportionOfWidth (0.0169f), proportionOfHeight (0.9318f));
    c_iBendRange->setBounds (proportionOfWidth (0.0070f), proportionOfHeight (0.0568f), proportionOfWidth (0.0422f), proportionOfHeight (0.1591f));
    label100->setBounds (proportionOfWidth (0.0014f), proportionOfHeight (0.2614f), proportionOfWidth (0.0521f), proportionOfHeight (0.1250f));
    label2->setBounds (proportionOfWidth (0.0000f), proportionOfHeight (0.8182f), proportionOfWidth (0.0528f), proportionOfHeight (0.1250f));
    m_fPortamento->setBounds (proportionOfWidth (0.0148f), proportionOfHeight (0.3977f), proportionOfWidth (0.0246f), proportionOfHeight (0.4091f));
    internalPath1.clear();
    internalPath1.startNewSubPath (static_cast<float> (proportionOfWidth (0.0865f)), static_cast<float> (proportionOfHeight (0.9227f)));
    internalPath1.lineTo (static_cast<float> (proportionOfWidth (0.0885f)), static_cast<float> (proportionOfHeight (0.0841f)));
    internalPath1.lineTo (static_cast<float> (proportionOfWidth (0.0845f)), static_cast<float> (proportionOfHeight (0.0841f)));
    internalPath1.closeSubPath();

    internalPath2.clear();
    internalPath2.startNewSubPath (static_cast<float> (proportionOfWidth (0.0615f)), static_cast<float> (proportionOfHeight (0.4886f)));
    internalPath2.lineTo (static_cast<float> (proportionOfWidth (0.0635f)), static_cast<float> (proportionOfHeight (0.0841f)));
    internalPath2.lineTo (static_cast<float> (proportionOfWidth (0.0595f)), static_cast<float> (proportionOfHeight (0.0841f)));
    internalPath2.closeSubPath();

    internalPath3.clear();
    internalPath3.startNewSubPath (static_cast<float> (proportionOfWidth (0.0615f)), static_cast<float> (proportionOfHeight (0.4886f)));
    internalPath3.lineTo (static_cast<float> (proportionOfWidth (0.0635f)), static_cast<float> (proportionOfHeight (0.9227f)));
    internalPath3.lineTo (static_cast<float> (proportionOfWidth (0.0595f)), static_cast<float> (proportionOfHeight (0.9227f)));
    internalPath3.closeSubPath();

    //[UserResized] Add your own custom resize handling here..
	if (c_midiKeyboard->getWidth() > 0)
		c_midiKeyboard->setKeyWidth(c_midiKeyboard->getWidth() / (7.f * 10.f + 5.f));
	c_iBendRange->setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxLeft, false, c_iBendRange->getWidth() * 0.45f, c_iBendRange->getHeight());
    //[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

MidiKeyboardComponent* VASTKeyboardComponent::getMidiKeyboard() {
    return c_midiKeyboard.get();
}

void VASTKeyboardComponent::sliderValueChanged(Slider* sliderThatWasMoved)
{
	if (sliderThatWasMoved == c_pitchBend.get())
	{
		float pitchWheelVal = c_pitchBend->getValue() + 8192.f; //0..16384
		VASTSynthesiser* synth = myProcessor->m_pVASTXperience.m_Poly.getSynthesizer();
		if (synth != nullptr) {
			for (int i = 0; i< 16; i++)
				synth->handlePitchWheel(i, pitchWheelVal);
		}
	}
	else if (sliderThatWasMoved == c_modWheel.get())
	{
		float modWheelVal = c_modWheel->getValue();
		VASTSynthesiser* synth = myProcessor->m_pVASTXperience.m_Poly.getSynthesizer();
		if (synth != nullptr) {
			for (int i = 0; i < 16; i++)
				synth->handleController(i, 1, modWheelVal); //controlle# 1 is modwheel
		}
	}
	else if (sliderThatWasMoved == c_iBendRange.get())
	{
		if (c_iBendRange->getValue() != myProcessor->m_pVASTXperience.m_Set.m_iBendRange) {
			myProcessor->m_pVASTXperience.m_Set.m_iBendRange = c_iBendRange->getValue();
            if (myProcessor->m_initCompleted.load())
			    myProcessor->writeSettingsToFileAsync();
		}
	}
}

void VASTKeyboardComponent::updateAll() {
    c_iBendRange->setValue(myProcessor->getBendRange(), NotificationType::dontSendNotification);
    updateMidiKeyboardCharLayout();
    updateMidiKeyboardBaseOctave();
}

void VASTKeyboardComponent::timerCallback() {
    if (!c_pitchBend->isMouseOverOrDragging()) {
        float wheelpos = myProcessor->m_pVASTXperience.m_Poly.getSynthesizer()->lastPitchWheelUIValue.load() - 8192.f; //all channels
        c_pitchBend->setValue(wheelpos, NotificationType::dontSendNotification); //send only on
    }

    if (!c_modWheel->isMouseOverOrDragging()) {
        //float wheelpos = myProcessor->m_pVASTXperience.m_Set.m_uModWheel.load(); //0..127
        float wheelpos = myProcessor->m_pVASTXperience.m_Poly.getSynthesizer()->m_fModWheel_smoothed[0].getTargetValue(); //0..127
        c_modWheel->setValue(wheelpos, NotificationType::dontSendNotification); //send only on
    }
}

void VASTKeyboardComponent::mouseDown (const MouseEvent &) {
    ModifierKeys modifiers = ModifierKeys::getCurrentModifiersRealtime();
    if (modifiers.isRightButtonDown()) {
        myProcessor->toggleKeyboardHoldMode();
        repaint();
    }
}

String VASTKeyboardComponent::getTooltip() {
    return (TRANS("Right click to toggle keyboard hold mode"));
}

void VASTKeyboardComponent::updateMidiKeyboardCharLayout()
{
    getMidiKeyboard()->clearKeyMappings();
    int note = 0;
    for (char c : myProcessor->getMidiKeyboardCharLayout())
        getMidiKeyboard()->setKeyPressForNote(KeyPress(c, 0, 0), note++);
}

void VASTKeyboardComponent::updateMidiKeyboardBaseOctave()
{
    getMidiKeyboard()->setKeyPressBaseOctave(myProcessor->getMidiKeyboardBaseOctave() + 2); //off by two?
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="VASTKeyboardComponent" componentName=""
                 parentClasses="public Component, public Slider::Listener, public Timer, public TooltipClient"
                 constructorParams="AudioProcessorEditor *editor, AudioProcessor* processor"
                 variableInitialisers="myEditor((VASTAudioProcessorEditor*)editor), myProcessor((VASTAudioProcessor*)processor)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="1420" initialHeight="88">
  <BACKGROUND backgroundColour="ff323e44">
    <RECT pos="0 0 0M 0M" fill="linear: 0 0, 0R 0R, 0=ff212527, 1=ff0b0b0b"
          hasStroke="0"/>
    <PATH pos="0 0 100 100" fill="solid: ffffff" hasStroke="1" stroke="1, mitered, butt"
          strokeColour="solid: 20ffffff" nonZeroWinding="1">s 8.652% 92.273% l 8.852% 8.409% l 8.452% 8.409% x</PATH>
    <PATH pos="0 0 100 100" fill="solid: ffffff" hasStroke="1" stroke="1, mitered, butt"
          strokeColour="solid: 20ffffff" nonZeroWinding="1">s 6.152% 48.864% l 6.352% 8.409% l 5.952% 8.409% x</PATH>
    <PATH pos="0 0 100 100" fill="solid: ffffff" hasStroke="1" stroke="1, mitered, butt"
          strokeColour="solid: 20ffffff" nonZeroWinding="1">s 6.152% 48.864% l 6.352% 92.273% l 5.952% 92.273% x</PATH>
    <RECT pos="0.8% 26.136% 3.944% 12.5%" fill="linear: 0.563% 27.273%, 5.07% 45.455%, 0=ff3a4145, 1=ff33393c"
          hasStroke="0"/>
    <RECT pos="0.8% 81.818% 3.944% 12.5%" fill="linear: 0.563% 72.727%, 4.507% 90.909%, 0=ff3a4145, 1=ff33393c"
          hasStroke="0"/>
  </BACKGROUND>
  <GENERICCOMPONENT name="c_midiKeyboard" id="164c4e11cc2226ca" memberName="c_midiKeyboard"
                    virtualName="MidiKeyboardComponent" explicitFocusOrder="0" pos="11.268% 0% 88.732% 100%"
                    class="MidiKeyboardComponent" params="myProcessor-&gt;m_midiKeyboardState, MidiKeyboardComponent::horizontalKeyboard"/>
  <GENERICCOMPONENT name="c_pitchBend" id="235d975d77d1b957" memberName="c_pitchBend"
                    virtualName="VASTPitchbendSlider" explicitFocusOrder="0" pos="6.549% 3.409% 1.69% 93.182%"
                    class="VASTPitchbendSlider" params="&quot;Pitchbend&quot;"/>
  <GENERICCOMPONENT name="c_modWheel" id="311447a3d0125864" memberName="c_modWheel"
                    virtualName="VASTModwheelSlider" explicitFocusOrder="0" pos="9.014% 3.409% 1.69% 93.182%"
                    class="VASTModwheelSlider" params="&quot;Pitchbend&quot;"/>
  <SLIDER name="c_iBendRange" id="6a281b071d356eaa" memberName="c_iBendRange"
          virtualName="VASTSlider" explicitFocusOrder="1" pos="0.704% 5.682% 4.225% 15.909%"
          tooltip="Pitch bend wheel sensitivity in semitiones (up/down)"
          min="1.0" max="96.0" int="1.0" style="IncDecButtons" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="28" textBoxHeight="20" skewFactor="1.0"
          needsCallback="0"/>
  <LABEL name="new label" id="8bc3bfc8c13517a" memberName="label100" virtualName=""
         explicitFocusOrder="0" pos="0.141% 26.136% 5.211% 12.5%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="BEND RNG" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Syntax"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <LABEL name="new label" id="dc7e0cf6cfcb99c7" memberName="label2" virtualName=""
         explicitFocusOrder="0" pos="0% 81.818% 5.282% 12.5%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="PORTAM" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Syntax"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <SLIDER name="m_fPortamento" id="a901afca02fd5dbf" memberName="m_fPortamento"
          virtualName="VASTParameterSlider" explicitFocusOrder="0" pos="1.479% 39.773% 2.465% 40.909%"
          tooltip="Portamento time in ms - available in poly modes and mono mode"
          textboxtext="ffc9c9c9" textboxbkgd="ffffff" textboxoutline="808080"
          min="0.0" max="5000.0" int="0.001" style="RotaryVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="30"
          textBoxHeight="14" skewFactor="1.0" needsCallback="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

