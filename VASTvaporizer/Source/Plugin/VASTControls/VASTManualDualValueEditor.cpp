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

//[Headers] You can add your own extra header files here...
#include "../VASTAudioProcessor.h"
#include "../VASTControls/VASTParameterSlider.h"
//[/Headers]

#include "VASTManualDualValueEditor.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
VASTManualDualValueEditor::VASTManualDualValueEditor (VASTAudioProcessor* processor, String paramid, Component* comp)
    : myProcessor(processor), mParamid(paramid), mComponent(comp)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    c_textEditorFrom.reset (new juce::TextEditor ("c_textEditorFrom"));
    addAndMakeVisible (c_textEditorFrom.get());
    c_textEditorFrom->setExplicitFocusOrder (1);
    c_textEditorFrom->setMultiLine (false);
    c_textEditorFrom->setReturnKeyStartsNewLine (false);
    c_textEditorFrom->setReadOnly (false);
    c_textEditorFrom->setScrollbarsShown (true);
    c_textEditorFrom->setCaretVisible (true);
    c_textEditorFrom->setPopupMenuEnabled (true);
    c_textEditorFrom->setText (juce::String());

    c_OK.reset (new juce::TextButton ("c_OK"));
    addAndMakeVisible (c_OK.get());
    c_OK->setButtonText (TRANS("OK"));
    c_OK->addListener (this);

    c_Cancel.reset (new juce::TextButton ("c_Cancel"));
    addAndMakeVisible (c_Cancel.get());
    c_Cancel->setButtonText (TRANS("Cancel"));
    c_Cancel->addListener (this);

    c_textEditorTo.reset (new juce::TextEditor ("c_textEditorTo"));
    addAndMakeVisible (c_textEditorTo.get());
    c_textEditorTo->setExplicitFocusOrder (1);
    c_textEditorTo->setMultiLine (false);
    c_textEditorTo->setReturnKeyStartsNewLine (false);
    c_textEditorTo->setReadOnly (false);
    c_textEditorTo->setScrollbarsShown (true);
    c_textEditorTo->setCaretVisible (true);
    c_textEditorTo->setPopupMenuEnabled (true);
    c_textEditorTo->setText (juce::String());

    label.reset (new juce::Label ("new label",
                                  TRANS("Start")));
    addAndMakeVisible (label.get());
    label->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label->setJustificationType (juce::Justification::centredLeft);
    label->setEditable (false, false, false);
    label->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label2.reset (new juce::Label ("new label",
                                   TRANS("End\n")));
    addAndMakeVisible (label2.get());
    label2->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label2->setJustificationType (juce::Justification::centredLeft);
    label2->setEditable (false, false, false);
    label2->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label2->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    c_unipolar.reset (new juce::ToggleButton ("c_unipolar"));
    addAndMakeVisible (c_unipolar.get());
    c_unipolar->setButtonText (juce::String());
    c_unipolar->addListener (this);

    label3.reset (new juce::Label ("new label",
                                   TRANS("Unipolar")));
    addAndMakeVisible (label3.get());
    label3->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label3->setJustificationType (juce::Justification::centredLeft);
    label3->setEditable (false, false, false);
    label3->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label3->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));


    //[UserPreSize]
	c_textEditorFrom->addListener(this);
	c_textEditorTo->addListener(this);
	c_Cancel->addShortcut(KeyPress(KeyPress::escapeKey));
    //[/UserPreSize]

    setSize (300, 60);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

VASTManualDualValueEditor::~VASTManualDualValueEditor()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    c_textEditorFrom = nullptr;
    c_OK = nullptr;
    c_Cancel = nullptr;
    c_textEditorTo = nullptr;
    label = nullptr;
    label2 = nullptr;
    c_unipolar = nullptr;
    label3 = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void VASTManualDualValueEditor::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void VASTManualDualValueEditor::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    c_textEditorFrom->setBounds (proportionOfWidth (0.2400f), proportionOfHeight (0.1000f), proportionOfWidth (0.3000f), proportionOfHeight (0.3000f));
    c_OK->setBounds (proportionOfWidth (0.6000f), proportionOfHeight (0.5333f), proportionOfWidth (0.1667f), proportionOfHeight (0.2500f));
    c_Cancel->setBounds (proportionOfWidth (0.7867f), proportionOfHeight (0.5333f), proportionOfWidth (0.1667f), proportionOfHeight (0.2500f));
    c_textEditorTo->setBounds (proportionOfWidth (0.2400f), proportionOfHeight (0.5333f), proportionOfWidth (0.3000f), proportionOfHeight (0.3000f));
    label->setBounds (proportionOfWidth (0.0267f), proportionOfHeight (0.0667f), proportionOfWidth (0.2133f), proportionOfHeight (0.4000f));
    label2->setBounds (proportionOfWidth (0.0267f), proportionOfHeight (0.4667f), proportionOfWidth (0.2133f), proportionOfHeight (0.4000f));
    c_unipolar->setBounds (proportionOfWidth (0.6000f), proportionOfHeight (0.1167f), proportionOfWidth (0.0567f), proportionOfHeight (0.2833f));
    label3->setBounds (proportionOfWidth (0.6600f), proportionOfHeight (0.0500f), proportionOfWidth (0.2133f), proportionOfHeight (0.4000f));
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void VASTManualDualValueEditor::buttonClicked (juce::Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == c_OK.get())
    {
        //[UserButtonCode_c_OK] -- add your button handler code here..
		if (mParamid != "") {
			float valFrom = c_textEditorFrom->getText().getFloatValue();
			float valTo = c_textEditorTo->getText().getFloatValue();
			VASTParameterSlider* _parameterslider = dynamic_cast<VASTParameterSlider*>(mComponent);
			int modmatdest = myProcessor->autoParamGetDestination(_parameterslider->getComponentID());
			int slot = myProcessor->m_pVASTXperience.m_Set.modMatrixGetFirstSlotWithDestination(modmatdest);
			juce::Range<double> range = (juce::Range<double>)_parameterslider->getRange();

			float newVal = 0.f;
			int polarity = c_unipolar->getToggleState() ? 0 : 1;
			if (polarity == POLARITY::Unipolar) {
				myProcessor->setParameterText(mParamid, String(valFrom), false);
				newVal = (valTo - valFrom) / (range.getEnd() - range.getStart());
			}
			else { //bipolar
				myProcessor->setParameterText(mParamid, String((valFrom + valTo) * 0.5f), false);
				newVal = ((valTo - valFrom) / (range.getEnd() - range.getStart())) * 0.5f;
			}
			if (newVal > 1.0f) newVal = 1.0f;
			if (newVal < -1.0f) newVal = -1.0f;

			myProcessor->setParameterText("m_fModMatVal" + String(slot + 1), String(newVal * 100.f), false);
			myProcessor->setParameterText("m_uModMatPolarity" + String(slot + 1), CVASTParamState::comboBoxValueToTextFunction_POLARITY(polarity), false);
		}
		else
			vassertfalse;
		/*
		else {
			float valFrom = c_textEditorFrom->getText().getFloatValue();
			float valTo = c_textEditorTo->getText().getFloatValue();
			VASTSlider* _nonparamslider = dynamic_cast<VASTSlider*>(mComponent);
			if (_nonparamslider != nullptr)
				_nonparamslider->setValue(val, NotificationType::sendNotificationSync);
		}
		*/
		getParentComponent()->exitModalState(0);
		getParentComponent()->grabKeyboardFocus();
        //[/UserButtonCode_c_OK]
    }
    else if (buttonThatWasClicked == c_Cancel.get())
    {
        //[UserButtonCode_c_Cancel] -- add your button handler code here..
		getParentComponent()->exitModalState(0);
		getParentComponent()->grabKeyboardFocus();
        //[/UserButtonCode_c_Cancel]
    }
    else if (buttonThatWasClicked == c_unipolar.get())
    {
        //[UserButtonCode_c_unipolar] -- add your button handler code here..
        //[/UserButtonCode_c_unipolar]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void VASTManualDualValueEditor::textEditorReturnKeyPressed(TextEditor& textEditorThatWasChanged) {
	if (&textEditorThatWasChanged == c_textEditorFrom.get())
	{
		c_OK->triggerClick();
	} else
	if (&textEditorThatWasChanged == c_textEditorTo.get())
	{
		c_OK->triggerClick();
	}
}
void VASTManualDualValueEditor::textEditorEscapeKeyPressed(TextEditor& textEditorThatWasChanged) {
	if (&textEditorThatWasChanged == c_textEditorFrom.get())
	{
		c_Cancel->triggerClick();
	} else
	if (&textEditorThatWasChanged == c_textEditorTo.get())
	{
		c_Cancel->triggerClick();
	}
}

void VASTManualDualValueEditor::setTextStart(String text) {
    c_textEditorFrom->setText(text, NotificationType::sendNotification);
    c_textEditorFrom->applyFontToAllText(((VASTLookAndFeel*)&getLookAndFeel())->getTextEditorFont(*c_textEditorFrom));
    c_textEditorFrom->selectAll();
}

void VASTManualDualValueEditor::setTextEnd(String text) {
    c_textEditorTo->setText(text, NotificationType::sendNotification);
    c_textEditorTo->applyFontToAllText(((VASTLookAndFeel*)&getLookAndFeel())->getTextEditorFont(*c_textEditorTo));
    c_textEditorTo->selectAll();
}

void VASTManualDualValueEditor::setPolarity(int polarity) {
    c_unipolar->setToggleState(polarity == POLARITY::Unipolar, NotificationType::sendNotification);
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="VASTManualDualValueEditor"
                 componentName="" parentClasses="public Component, public TextEditor::Listener"
                 constructorParams="VASTAudioProcessor* processor, String paramid, Component* comp"
                 variableInitialisers="myProcessor(processor), mParamid(paramid), mComponent(comp)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="300" initialHeight="60">
  <BACKGROUND backgroundColour="ff323e44"/>
  <TEXTEDITOR name="c_textEditorFrom" id="56e34f616e36cd4f" memberName="c_textEditorFrom"
              virtualName="" explicitFocusOrder="1" pos="24% 10% 30% 30%" initialText=""
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <TEXTBUTTON name="c_OK" id="8746701f60483199" memberName="c_OK" virtualName=""
              explicitFocusOrder="0" pos="60% 53.333% 16.667% 25%" buttonText="OK"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="c_Cancel" id="a6f8f52e5b0cabd3" memberName="c_Cancel" virtualName=""
              explicitFocusOrder="0" pos="78.667% 53.333% 16.667% 25%" buttonText="Cancel"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTEDITOR name="c_textEditorTo" id="585d88cfd317f5fc" memberName="c_textEditorTo"
              virtualName="" explicitFocusOrder="1" pos="24% 53.333% 30% 30%"
              initialText="" multiline="0" retKeyStartsLine="0" readonly="0"
              scrollbars="1" caret="1" popupmenu="1"/>
  <LABEL name="new label" id="314bf79ee003157c" memberName="label" virtualName=""
         explicitFocusOrder="0" pos="2.667% 6.667% 21.333% 40%" edTextCol="ff000000"
         edBkgCol="0" labelText="Start" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="fdc86c4485310b8c" memberName="label2" virtualName=""
         explicitFocusOrder="0" pos="2.667% 46.667% 21.333% 40%" edTextCol="ff000000"
         edBkgCol="0" labelText="End&#10;" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <TOGGLEBUTTON name="c_unipolar" id="503780b70abc3661" memberName="c_unipolar"
                virtualName="" explicitFocusOrder="0" pos="60% 11.667% 5.667% 28.333%"
                buttonText="" connectedEdges="0" needsCallback="1" radioGroupId="0"
                state="0"/>
  <LABEL name="new label" id="cbe9e6abf522e8e6" memberName="label3" virtualName=""
         explicitFocusOrder="0" pos="66% 5% 21.333% 40%" edTextCol="ff000000"
         edBkgCol="0" labelText="Unipolar" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

