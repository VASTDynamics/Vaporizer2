/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 6.0.0

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
#include "../VASTAudioProcessor.h"
#include "../VASTControls/VASTParameterSlider.h"
#include "../VASTWTEditor/VASTFreqDomainViewport.h"
#include "../VASTWaveTableEditorComponent.h"
//[/Headers]

#include "VASTManualFreqdomainValueEditor.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
VASTManualFreqdomainValueEditor::VASTManualFreqdomainValueEditor (VASTAudioProcessor* processor, VASTWaveTableEditorComponent* wteditor, int bin)
    : myProcessor(processor), myWtEditor(wteditor), mBin(bin)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    c_textEditorMagnitude.reset (new juce::TextEditor ("c_textEditorMagnitude"));
    addAndMakeVisible (c_textEditorMagnitude.get());
    c_textEditorMagnitude->setExplicitFocusOrder (1);
    c_textEditorMagnitude->setMultiLine (false);
    c_textEditorMagnitude->setReturnKeyStartsNewLine (false);
    c_textEditorMagnitude->setReadOnly (false);
    c_textEditorMagnitude->setScrollbarsShown (true);
    c_textEditorMagnitude->setCaretVisible (true);
    c_textEditorMagnitude->setPopupMenuEnabled (true);
    c_textEditorMagnitude->setText (juce::String());

    c_OK.reset (new juce::TextButton ("c_OK"));
    addAndMakeVisible (c_OK.get());
    c_OK->setButtonText (TRANS("OK"));
    c_OK->addListener (this);

    c_Cancel.reset (new juce::TextButton ("c_Cancel"));
    addAndMakeVisible (c_Cancel.get());
    c_Cancel->setButtonText (TRANS("Cancel"));
    c_Cancel->addListener (this);

    c_textEditorPhase.reset (new juce::TextEditor ("c_textEditorPhase"));
    addAndMakeVisible (c_textEditorPhase.get());
    c_textEditorPhase->setExplicitFocusOrder (1);
    c_textEditorPhase->setMultiLine (false);
    c_textEditorPhase->setReturnKeyStartsNewLine (false);
    c_textEditorPhase->setReadOnly (false);
    c_textEditorPhase->setScrollbarsShown (true);
    c_textEditorPhase->setCaretVisible (true);
    c_textEditorPhase->setPopupMenuEnabled (true);
    c_textEditorPhase->setText (juce::String());

    label.reset (new juce::Label ("new label",
                                  TRANS("Magnitude")));
    addAndMakeVisible (label.get());
    label->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label->setJustificationType (juce::Justification::centredLeft);
    label->setEditable (false, false, false);
    label->setColour (TextEditor::textColourId, juce::Colours::black);
    label->setColour (TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label2.reset (new juce::Label ("new label",
                                   TRANS("Phase")));
    addAndMakeVisible (label2.get());
    label2->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label2->setJustificationType (juce::Justification::centredLeft);
    label2->setEditable (false, false, false);
    label2->setColour (TextEditor::textColourId, juce::Colours::black);
    label2->setColour (TextEditor::backgroundColourId, juce::Colour (0x00000000));


    //[UserPreSize]
	c_textEditorMagnitude->addListener(this);
	c_textEditorPhase->addListener(this);
	c_Cancel->addShortcut(KeyPress(KeyPress::escapeKey));
    //[/UserPreSize]

    setSize (300, 60);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

VASTManualFreqdomainValueEditor::~VASTManualFreqdomainValueEditor()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    c_textEditorMagnitude = nullptr;
    c_OK = nullptr;
    c_Cancel = nullptr;
    c_textEditorPhase = nullptr;
    label = nullptr;
    label2 = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void VASTManualFreqdomainValueEditor::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void VASTManualFreqdomainValueEditor::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    c_textEditorMagnitude->setBounds (proportionOfWidth (0.3000f), proportionOfHeight (0.1000f), proportionOfWidth (0.3000f), proportionOfHeight (0.3000f));
    c_OK->setBounds (proportionOfWidth (0.6400f), proportionOfHeight (0.5333f), proportionOfWidth (0.1667f), proportionOfHeight (0.2500f));
    c_Cancel->setBounds (proportionOfWidth (0.8267f), proportionOfHeight (0.5333f), proportionOfWidth (0.1667f), proportionOfHeight (0.2500f));
    c_textEditorPhase->setBounds (proportionOfWidth (0.3000f), proportionOfHeight (0.5333f), proportionOfWidth (0.3000f), proportionOfHeight (0.3000f));
    label->setBounds (proportionOfWidth (0.0267f), proportionOfHeight (0.0667f), proportionOfWidth (0.2667f), proportionOfHeight (0.4000f));
    label2->setBounds (proportionOfWidth (0.0267f), proportionOfHeight (0.4667f), proportionOfWidth (0.2667f), proportionOfHeight (0.4000f));
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void VASTManualFreqdomainValueEditor::buttonClicked (juce::Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == c_OK.get())
    {
        //[UserButtonCode_c_OK] -- add your button handler code here..
		if (mBin >= 0) {
			float valMagnitude = c_textEditorMagnitude->getText().getFloatValue();
			float valPhase = c_textEditorPhase->getText().getFloatValue();

			std::shared_ptr<CVASTWaveTable> lwavetable = myWtEditor->getBankWavetable();
			std::vector<sFreqDomainBuffer> domainBuffer;
			sFreqDomainBuffer domainBufferPos;
			domainBufferPos.wtPos = myWtEditor->getWtPos();
			domainBufferPos.domainBuffer = *lwavetable->getFreqDomainBuffer(domainBufferPos.wtPos);

			valMagnitude = (valMagnitude > 1.f) ? 1.f : valMagnitude;
			valMagnitude = (valMagnitude < 0.f) ? 0.f : valMagnitude;
			valPhase = (valPhase > 1.f) ? 1.f : valPhase;
			valPhase = (valPhase < 0.f) ? 0.f : valPhase;

			domainBufferPos.domainBuffer[mBin] = VASTFreqDomainViewport::setDomainBufferSlotRealPercentage(valMagnitude, domainBufferPos.domainBuffer[mBin]);
			domainBufferPos.domainBuffer[mBin] = VASTFreqDomainViewport::setDomainBufferSlotImagPercentage(valPhase, domainBufferPos.domainBuffer[mBin]);

			domainBuffer.push_back(domainBufferPos);
			bool binmode = myProcessor->getBinMode();
			bool bClip = (binmode == BinMode::ClipBin);
			std::thread edit_thread(&VASTFreqDomainViewport::adjustFreqDomainInternalThreaded, domainBuffer, bClip, myWtEditor, myProcessor);
			edit_thread.detach();
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

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void VASTManualFreqdomainValueEditor::textEditorReturnKeyPressed(TextEditor& textEditorThatWasChanged) {
	if (&textEditorThatWasChanged == c_textEditorMagnitude.get())
	{
		c_OK->triggerClick();
	} else
	if (&textEditorThatWasChanged == c_textEditorPhase.get())
	{
		c_OK->triggerClick();
	}
}
void VASTManualFreqdomainValueEditor::textEditorEscapeKeyPressed(TextEditor& textEditorThatWasChanged) {
	if (&textEditorThatWasChanged == c_textEditorMagnitude.get())
	{
		c_Cancel->triggerClick();
	} else
	if (&textEditorThatWasChanged == c_textEditorPhase.get())
	{
		c_Cancel->triggerClick();
	}
}

void VASTManualFreqdomainValueEditor::setTextMagnitude(String text) {
    c_textEditorMagnitude->setText(text, NotificationType::sendNotification);
    c_textEditorMagnitude->applyFontToAllText(((VASTLookAndFeel*)&getLookAndFeel())->getTextEditorFont(*c_textEditorMagnitude));
    c_textEditorMagnitude->selectAll();
}

void VASTManualFreqdomainValueEditor::setTextPhase(String text) {
    c_textEditorPhase->setText(text, NotificationType::sendNotification);
    c_textEditorPhase->applyFontToAllText(((VASTLookAndFeel*)&getLookAndFeel())->getTextEditorFont(*c_textEditorPhase));
    c_textEditorPhase->selectAll();
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="VASTManualFreqdomainValueEditor"
                 componentName="" parentClasses="public Component, public TextEditor::Listener"
                 constructorParams="VASTAudioProcessor* processor, VASTWaveTableEditorComponent* wteditor, int bin"
                 variableInitialisers="myProcessor(processor), myWtEditor(wteditor), mBin(bin)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="300" initialHeight="60">
  <BACKGROUND backgroundColour="ff323e44"/>
  <TEXTEDITOR name="c_textEditorMagnitude" id="56e34f616e36cd4f" memberName="c_textEditorMagnitude"
              virtualName="" explicitFocusOrder="1" pos="30% 10% 30% 30%" initialText=""
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <TEXTBUTTON name="c_OK" id="8746701f60483199" memberName="c_OK" virtualName=""
              explicitFocusOrder="0" pos="64% 53.333% 16.667% 25%" buttonText="OK"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="c_Cancel" id="a6f8f52e5b0cabd3" memberName="c_Cancel" virtualName=""
              explicitFocusOrder="0" pos="82.667% 53.333% 16.667% 25%" buttonText="Cancel"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTEDITOR name="c_textEditorPhase" id="585d88cfd317f5fc" memberName="c_textEditorPhase"
              virtualName="" explicitFocusOrder="1" pos="30% 53.333% 30% 30%"
              initialText="" multiline="0" retKeyStartsLine="0" readonly="0"
              scrollbars="1" caret="1" popupmenu="1"/>
  <LABEL name="new label" id="314bf79ee003157c" memberName="label" virtualName=""
         explicitFocusOrder="0" pos="2.667% 6.667% 26.667% 40%" edTextCol="ff000000"
         edBkgCol="0" labelText="Magnitude" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="fdc86c4485310b8c" memberName="label2" virtualName=""
         explicitFocusOrder="0" pos="2.667% 46.667% 26.667% 40%" edTextCol="ff000000"
         edBkgCol="0" labelText="Phase" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

