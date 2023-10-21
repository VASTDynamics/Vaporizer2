/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 6.1.2

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
#include "../VASTAudioProcessor.h"
#include "../VASTControls/VASTSlider.h"
#include "../VASTPreset/VASTPresetComponent.h"
//[/Headers]

#include "VASTManualTagsEditor.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
VASTManualTagsEditor::VASTManualTagsEditor (VASTAudioProcessor* processor, String internalid, VASTPresetComponent* presetComponent)
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

    label.reset (new juce::Label ("new label",
                                  TRANS("Separate tags with space ( ), comma (,) or hashtag (#)")));
    addAndMakeVisible (label.get());
    label->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label->setJustificationType (juce::Justification::centredLeft);
    label->setEditable (false, false, false);
    label->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));


    //[UserPreSize]
	c_textEditor->addListener(this);
	c_Cancel->addShortcut(KeyPress(KeyPress::escapeKey));
    //[/UserPreSize]

    setSize (600, 60);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

VASTManualTagsEditor::~VASTManualTagsEditor()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    c_textEditor = nullptr;
    c_OK = nullptr;
    c_Cancel = nullptr;
    label = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void VASTManualTagsEditor::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void VASTManualTagsEditor::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    c_textEditor->setBounds (proportionOfWidth (0.0133f), proportionOfHeight (0.5000f), proportionOfWidth (0.7867f), proportionOfHeight (0.3000f));
    c_OK->setBounds (proportionOfWidth (0.8133f), proportionOfHeight (0.5333f), proportionOfWidth (0.0833f), proportionOfHeight (0.2500f));
    c_Cancel->setBounds (proportionOfWidth (0.9067f), proportionOfHeight (0.5333f), proportionOfWidth (0.0833f), proportionOfHeight (0.2500f));
    label->setBounds (proportionOfWidth (0.0133f), proportionOfHeight (0.1500f), proportionOfWidth (0.9717f), proportionOfHeight (0.1833f));
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void VASTManualTagsEditor::buttonClicked (juce::Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == c_OK.get())
    {
        //[UserButtonCode_c_OK] -- add your button handler code here..
		//if (mInternalid != "")

		VASTPresetElement lElem = myProcessor->m_presetData.getCurPatchData();
		lElem.freetag = c_textEditor->getText();
		myProcessor->m_presetData.exchangeCurPatchData(lElem);
		//save preset!!
		File presetFile(mPresetComponent->myProcessor->m_presetData.getCurPatchData().internalid); //only curpatch!
		myProcessor->savePatchXML(&presetFile);

		mPresetComponent->reloadPresets();
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



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void VASTManualTagsEditor::textEditorReturnKeyPressed(TextEditor& textEditorThatWasChanged) {
	if (&textEditorThatWasChanged == c_textEditor.get())
	{
		c_OK->triggerClick();
	}
}
void VASTManualTagsEditor::textEditorEscapeKeyPressed(TextEditor& textEditorThatWasChanged) {
	if (&textEditorThatWasChanged == c_textEditor.get())
	{
		c_Cancel->triggerClick();
	}
}
void VASTManualTagsEditor::setTags(String text) {
    c_textEditor->setText(text, NotificationType::sendNotification);
    c_textEditor->applyFontToAllText(((VASTLookAndFeel*)&getLookAndFeel())->getTextEditorFont(*c_textEditor));
    c_textEditor->selectAll();
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="VASTManualTagsEditor" componentName=""
                 parentClasses="public Component, public TextEditor::Listener"
                 constructorParams="VASTAudioProcessor* processor, String internalid, VASTPresetComponent* presetComponent"
                 variableInitialisers="myProcessor(processor), mInternalid(internalid), mPresetComponent(presetComponent)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="600" initialHeight="60">
  <BACKGROUND backgroundColour="ff323e44"/>
  <TEXTEDITOR name="c_textEditor" id="56e34f616e36cd4f" memberName="c_textEditor"
              virtualName="" explicitFocusOrder="1" pos="1.333% 50% 78.667% 30%"
              initialText="" multiline="0" retKeyStartsLine="0" readonly="0"
              scrollbars="1" caret="1" popupmenu="1"/>
  <TEXTBUTTON name="c_OK" id="8746701f60483199" memberName="c_OK" virtualName=""
              explicitFocusOrder="0" pos="81.333% 53.333% 8.333% 25%" buttonText="OK"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="c_Cancel" id="a6f8f52e5b0cabd3" memberName="c_Cancel" virtualName=""
              explicitFocusOrder="0" pos="90.667% 53.333% 8.333% 25%" buttonText="Cancel"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <LABEL name="new label" id="4dcebab206f198a7" memberName="label" virtualName=""
         explicitFocusOrder="0" pos="1.333% 15% 97.167% 18.333%" edTextCol="ff000000"
         edBkgCol="0" labelText="Separate tags with space ( ), comma (,) or hashtag (#)"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="33"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

