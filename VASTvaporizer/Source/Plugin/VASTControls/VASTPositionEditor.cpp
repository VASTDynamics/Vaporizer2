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
#include "../VASTMSEGEditor/VASTMSEGEditor.h"
#include "../VASTMSEGEditor/VASTStepSeqEditor.h"
//[/Headers]

#include "VASTPositionEditor.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
VASTPositionEditor::VASTPositionEditor(VASTAudioProcessor* processor, float value, VASTStepSeqEditor* stepseqeditor, bool isX, int numIsClicked)
	: myProcessor(processor),
	myValue(value),
	myStepSeqEditor(stepseqeditor), myIsX(isX), myNumIsClicked(numIsClicked)
{
	//[Constructor_pre] You can add your own custom stuff here..
	//[/Constructor_pre]

	c_textEditor.reset(new TextEditor("c_textEditor"));
	addAndMakeVisible(c_textEditor.get());
	c_textEditor->setExplicitFocusOrder(1);
	c_textEditor->setMultiLine(false);
	c_textEditor->setReturnKeyStartsNewLine(false);
	c_textEditor->setReadOnly(false);
	c_textEditor->setScrollbarsShown(true);
	c_textEditor->setCaretVisible(true);
	c_textEditor->setPopupMenuEnabled(true);
	c_textEditor->setText(String());

	c_textEditor->setBounds(8, 6, 176, 18);

	c_OK.reset(new TextButton("c_OK"));
	addAndMakeVisible(c_OK.get());
	c_OK->setButtonText(TRANS("OK"));
	c_OK->addListener(this);

	c_OK->setBounds(192, 8, 50, 15);

	c_Cancel.reset(new TextButton("c_Cancel"));
	addAndMakeVisible(c_Cancel.get());
	c_Cancel->setButtonText(TRANS("Cancel"));
	c_Cancel->addListener(this);

	c_Cancel->setBounds(248, 8, 50, 15);


	//[UserPreSize]
	c_textEditor->addListener(this);
	c_Cancel->addShortcut(KeyPress(KeyPress::escapeKey));
	//[/UserPreSize]

	setSize(300, 30);


	//[Constructor] You can add your own custom stuff here..
	//[/Constructor]
}

//[/MiscUserDefs]

//==============================================================================
VASTPositionEditor::VASTPositionEditor (VASTAudioProcessor* processor, float value, VASTMSEGEditor* msegeditor, bool isX, int numIsClicked)
    : myProcessor(processor),
      myValue(value),
      myMSEGEditor(msegeditor), myIsX(isX), myNumIsClicked(numIsClicked)
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


    //[UserPreSize]
	c_textEditor->addListener(this);
	c_Cancel->addShortcut(KeyPress(KeyPress::escapeKey));
    //[/UserPreSize]

    setSize (300, 30);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

VASTPositionEditor::~VASTPositionEditor()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    c_textEditor = nullptr;
    c_OK = nullptr;
    c_Cancel = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void VASTPositionEditor::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void VASTPositionEditor::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    c_textEditor->setBounds (proportionOfWidth (0.0267f), proportionOfHeight (0.2000f), proportionOfWidth (0.5867f), proportionOfHeight (0.6000f));
    c_OK->setBounds (proportionOfWidth (0.6400f), proportionOfHeight (0.2667f), proportionOfWidth (0.1667f), proportionOfHeight (0.5000f));
    c_Cancel->setBounds (proportionOfWidth (0.8267f), proportionOfHeight (0.2667f), proportionOfWidth (0.1667f), proportionOfHeight (0.5000f));
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void VASTPositionEditor::buttonClicked (juce::Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == c_OK.get())
    {
        //[UserButtonCode_c_OK] -- add your button handler code here..
		float val = c_textEditor->getText().getFloatValue();
		if (myMSEGEditor != nullptr)
			myMSEGEditor->passBackPopupResult(val, myIsX, myNumIsClicked);
		else if (myStepSeqEditor != nullptr)
			myStepSeqEditor->passBackPopupResult(val, myNumIsClicked);
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
void VASTPositionEditor::textEditorReturnKeyPressed(TextEditor& textEditorThatWasChanged) {
	if (&textEditorThatWasChanged == c_textEditor.get())
	{
		c_OK->triggerClick();
	}
}
void VASTPositionEditor::textEditorEscapeKeyPressed(TextEditor& textEditorThatWasChanged) {
	if (&textEditorThatWasChanged == c_textEditor.get())
	{
		c_Cancel->triggerClick();
	}
}
void VASTPositionEditor::setText(String text) {
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

<JUCER_COMPONENT documentType="Component" className="VASTPositionEditor" componentName=""
                 parentClasses="public Component, public TextEditor::Listener"
                 constructorParams="VASTAudioProcessor* processor, float value, VASTMSEGEditor* msegeditor, bool isX, int numIsClicked"
                 variableInitialisers="myProcessor(processor),  &#10;myValue(value),&#10;myMSEGEditor(msegeditor), myIsX(isX), myNumIsClicked(numIsClicked)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="300" initialHeight="30">
  <BACKGROUND backgroundColour="ff323e44"/>
  <TEXTEDITOR name="c_textEditor" id="56e34f616e36cd4f" memberName="c_textEditor"
              virtualName="" explicitFocusOrder="1" pos="2.667% 20% 58.667% 60%"
              initialText="" multiline="0" retKeyStartsLine="0" readonly="0"
              scrollbars="1" caret="1" popupmenu="1"/>
  <TEXTBUTTON name="c_OK" id="8746701f60483199" memberName="c_OK" virtualName=""
              explicitFocusOrder="0" pos="64% 26.667% 16.667% 50%" buttonText="OK"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="c_Cancel" id="a6f8f52e5b0cabd3" memberName="c_Cancel" virtualName=""
              explicitFocusOrder="0" pos="82.667% 26.667% 16.667% 50%" buttonText="Cancel"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

