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
#include "../VASTControls/VASTSlider.h"
#include "../VASTControls/VASTComboBox.h"
#include "../VASTPreset/VASTPresetComponent.h"
//[/Headers]

#include "VASTManualRenameEditor.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
VASTManualRenameEditor::VASTManualRenameEditor (VASTAudioProcessor* processor, String internalid, VASTPresetComponent* presetComponent)
    : myProcessor(processor), mInternalid(internalid), mPresetComponent(presetComponent)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    c_textEditor.reset (new juce::TextEditor ("c_textEditor"));
    addAndMakeVisible (c_textEditor.get());
    c_textEditor->setExplicitFocusOrder (1);
    c_textEditor->setMultiLine (false);
    c_textEditor->setReturnKeyStartsNewLine (false);
    c_textEditor->setReadOnly (false);
    c_textEditor->setScrollbarsShown (true);
    c_textEditor->setCaretVisible (true);
    c_textEditor->setPopupMenuEnabled (true);
    c_textEditor->setText (juce::String());

    c_OK.reset (new juce::TextButton ("c_OK"));
    addAndMakeVisible (c_OK.get());
    c_OK->setButtonText (TRANS("OK"));
    c_OK->addListener (this);

    c_Cancel.reset (new juce::TextButton ("c_Cancel"));
    addAndMakeVisible (c_Cancel.get());
    c_Cancel->setButtonText (TRANS("Cancel"));
    c_Cancel->addListener (this);

    c_presetCategoryCombo.reset (new VASTComboBox ("c_presetCategoryCombo"));
    addAndMakeVisible (c_presetCategoryCombo.get());
    c_presetCategoryCombo->setExplicitFocusOrder (1);
    c_presetCategoryCombo->setEditableText (false);
    c_presetCategoryCombo->setJustificationType (juce::Justification::centredLeft);
    c_presetCategoryCombo->setTextWhenNothingSelected (juce::String());
    c_presetCategoryCombo->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    c_presetCategoryCombo->addItem (TRANS("AR Arpeggio "), 1);
    c_presetCategoryCombo->addItem (TRANS("AT Atmosphere"), 2);
    c_presetCategoryCombo->addItem (TRANS("BA Bass"), 3);
    c_presetCategoryCombo->addItem (TRANS("BR Brass "), 4);
    c_presetCategoryCombo->addItem (TRANS("BL Bell"), 5);
    c_presetCategoryCombo->addItem (TRANS("CH Chord"), 6);
    c_presetCategoryCombo->addItem (TRANS("DK Drum kit"), 7);
    c_presetCategoryCombo->addItem (TRANS("DR Drum"), 8);
    c_presetCategoryCombo->addItem (TRANS("DL Drum loop"), 9);
    c_presetCategoryCombo->addItem (TRANS("FX Effect"), 10);
    c_presetCategoryCombo->addItem (TRANS("GT Guitar"), 11);
    c_presetCategoryCombo->addItem (TRANS("IN Instrument"), 12);
    c_presetCategoryCombo->addItem (TRANS("KB Keyboard"), 13);
    c_presetCategoryCombo->addItem (TRANS("LD Lead"), 14);
    c_presetCategoryCombo->addItem (TRANS("MA Mallet"), 15);
    c_presetCategoryCombo->addItem (TRANS("OR Organ"), 16);
    c_presetCategoryCombo->addItem (TRANS("OC Orchestral"), 17);
    c_presetCategoryCombo->addItem (TRANS("PD Pad"), 18);
    c_presetCategoryCombo->addItem (TRANS("PN Piano"), 19);
    c_presetCategoryCombo->addItem (TRANS("PL Plucked"), 20);
    c_presetCategoryCombo->addItem (TRANS("RI Riser"), 21);
    c_presetCategoryCombo->addItem (TRANS("RD Reed"), 22);
    c_presetCategoryCombo->addItem (TRANS("ST String"), 23);
    c_presetCategoryCombo->addItem (TRANS("SY Synth"), 24);
    c_presetCategoryCombo->addItem (TRANS("SQ Sequence / Split"), 25);
    c_presetCategoryCombo->addItem (TRANS("TG Trancegate"), 26);
    c_presetCategoryCombo->addItem (TRANS("VC Vocal / Voice"), 27);
    c_presetCategoryCombo->addItem (TRANS("WW Woodwind"), 28);
    c_presetCategoryCombo->addListener (this);


    //[UserPreSize]
	c_textEditor->addListener(this);
	c_Cancel->addShortcut(KeyPress(KeyPress::escapeKey));
    //[/UserPreSize]

    setSize (600, 60);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

VASTManualRenameEditor::~VASTManualRenameEditor()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    c_textEditor = nullptr;
    c_OK = nullptr;
    c_Cancel = nullptr;
    c_presetCategoryCombo = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void VASTManualRenameEditor::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void VASTManualRenameEditor::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    c_textEditor->setBounds (proportionOfWidth (0.0267f), proportionOfHeight (0.5000f), proportionOfWidth (0.7733f), proportionOfHeight (0.3000f));
    c_OK->setBounds (proportionOfWidth (0.8133f), proportionOfHeight (0.5333f), proportionOfWidth (0.0833f), proportionOfHeight (0.2500f));
    c_Cancel->setBounds (proportionOfWidth (0.9067f), proportionOfHeight (0.5333f), proportionOfWidth (0.0833f), proportionOfHeight (0.2500f));
    c_presetCategoryCombo->setBounds (proportionOfWidth (0.0267f), proportionOfHeight (0.1333f), proportionOfWidth (0.1150f), proportionOfHeight (0.3000f));
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void VASTManualRenameEditor::buttonClicked (juce::Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == c_OK.get())
    {
        //[UserButtonCode_c_OK] -- add your button handler code here..

		VASTPresetElement lElem = myProcessor->m_presetData.getCurPatchData();
		lElem.category = c_presetCategoryCombo->getText();
		lElem.presetname = c_textEditor->getText();

		//save preset!!
		File presetFile(mPresetComponent->myProcessor->m_presetData.getCurPatchData().internalid); //only curpatch!
		String newfilename = presetFile.getParentDirectory().getFullPathName() + presetFile.getSeparatorString() + lElem.category + " " + lElem.presetname + ".vvp";
		File targetFile(newfilename);
		if (!targetFile.existsAsFile()) {
			lElem.internalid = newfilename;
			myProcessor->m_presetData.exchangeCurPatchData(lElem);
			myProcessor->savePatchXML(&presetFile);
			bool UNUSED(success) = presetFile.moveFileTo(newfilename);
			mPresetComponent->reloadPresets();
		}
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

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void VASTManualRenameEditor::comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == c_presetCategoryCombo.get())
    {
        //[UserComboBoxCode_c_presetCategoryCombo] -- add your combo box handling code here..
        //[/UserComboBoxCode_c_presetCategoryCombo]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void VASTManualRenameEditor::textEditorReturnKeyPressed(TextEditor& textEditorThatWasChanged) {
	if (&textEditorThatWasChanged == c_textEditor.get())
	{
		c_OK->triggerClick();
	}
}
void VASTManualRenameEditor::textEditorEscapeKeyPressed(TextEditor& textEditorThatWasChanged) {
	if (&textEditorThatWasChanged == c_textEditor.get())
	{
		c_Cancel->triggerClick();
	}
}
void VASTManualRenameEditor::setName(String text) {
    c_textEditor->setText(text, NotificationType::sendNotification);
    c_textEditor->applyFontToAllText(((VASTLookAndFeel*)&getLookAndFeel())->getTextEditorFont(*c_textEditor));
    c_textEditor->selectAll();
}
void VASTManualRenameEditor::setCategory(String text) {
    c_presetCategoryCombo->setText(text, NotificationType::sendNotification);
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="VASTManualRenameEditor" componentName=""
                 parentClasses="public Component, public TextEditor::Listener"
                 constructorParams="VASTAudioProcessor* processor, String internalid, VASTPresetComponent* presetComponent"
                 variableInitialisers="myProcessor(processor), mInternalid(internalid), mPresetComponent(presetComponent)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="600" initialHeight="60">
  <BACKGROUND backgroundColour="ff323e44"/>
  <TEXTEDITOR name="c_textEditor" id="56e34f616e36cd4f" memberName="c_textEditor"
              virtualName="" explicitFocusOrder="1" pos="2.667% 50% 77.333% 30%"
              initialText="" multiline="0" retKeyStartsLine="0" readonly="0"
              scrollbars="1" caret="1" popupmenu="1"/>
  <TEXTBUTTON name="c_OK" id="8746701f60483199" memberName="c_OK" virtualName=""
              explicitFocusOrder="0" pos="81.333% 53.333% 8.333% 25%" buttonText="OK"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="c_Cancel" id="a6f8f52e5b0cabd3" memberName="c_Cancel" virtualName=""
              explicitFocusOrder="0" pos="90.667% 53.333% 8.333% 25%" buttonText="Cancel"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <COMBOBOX name="c_presetCategoryCombo" id="90780feb0180733d" memberName="c_presetCategoryCombo"
            virtualName="VASTComboBox" explicitFocusOrder="1" pos="2.667% 13.333% 11.5% 30%"
            editable="0" layout="33" items="AR Arpeggio &#10;AT Atmosphere&#10;BA Bass&#10;BR Brass &#10;BL Bell&#10;CH Chord&#10;DK Drum kit&#10;DR Drum&#10;DL Drum loop&#10;FX Effect&#10;GT Guitar&#10;IN Instrument&#10;KB Keyboard&#10;LD Lead&#10;MA Mallet&#10;OR Organ&#10;OC Orchestral&#10;PD Pad&#10;PN Piano&#10;PL Plucked&#10;RI Riser&#10;RD Reed&#10;ST String&#10;SY Synth&#10;SQ Sequence / Split&#10;TG Trancegate&#10;VC Vocal / Voice&#10;WW Woodwind"
            textWhenNonSelected="" textWhenNoItems="(no choices)"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

