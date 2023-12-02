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
#include "../VASTControls/VASTComboBox.h"
#include "../VASTPreset/VASTPresetComponent.h"
#include "../VASTWTEditor/VASTSamplerViewport.h"
#include "../VASTWaveTableEditorComponent.h"
//[/Headers]

#include "VASTManualScaleEditor.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
VASTManualScaleEditor::VASTManualScaleEditor (VASTAudioProcessor* processor, VASTWaveTableEditorComponent* wtEditor, VASTSamplerViewport* samplerViewport)
    : myProcessor(processor), mSamplerViewport(samplerViewport), mWTEditor(wtEditor)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    c_cycles.reset (new juce::TextEditor ("c_cycles"));
    addAndMakeVisible (c_cycles.get());
    c_cycles->setExplicitFocusOrder (1);
    c_cycles->setMultiLine (false);
    c_cycles->setReturnKeyStartsNewLine (false);
    c_cycles->setReadOnly (false);
    c_cycles->setScrollbarsShown (true);
    c_cycles->setCaretVisible (true);
    c_cycles->setPopupMenuEnabled (true);
    c_cycles->setText (juce::String());

    c_OK.reset (new juce::TextButton ("c_OK"));
    addAndMakeVisible (c_OK.get());
    c_OK->setButtonText (TRANS("OK"));
    c_OK->addListener (this);

    c_Cancel.reset (new juce::TextButton ("c_Cancel"));
    addAndMakeVisible (c_Cancel.get());
    c_Cancel->setButtonText (TRANS("Cancel"));
    c_Cancel->addListener (this);

    c_samples.reset (new juce::TextEditor ("c_samples"));
    addAndMakeVisible (c_samples.get());
    c_samples->setExplicitFocusOrder (1);
    c_samples->setMultiLine (false);
    c_samples->setReturnKeyStartsNewLine (false);
    c_samples->setReadOnly (false);
    c_samples->setScrollbarsShown (true);
    c_samples->setCaretVisible (true);
    c_samples->setPopupMenuEnabled (true);
    c_samples->setText (juce::String());

    label.reset (new juce::Label ("new label",
                                  TRANS("Chop after #samples")));
    addAndMakeVisible (label.get());
    label->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label->setJustificationType (juce::Justification::centredLeft);
    label->setEditable (false, false, false);
    label->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label2.reset (new juce::Label ("new label",
                                   TRANS("Scale into #cycles")));
    addAndMakeVisible (label2.get());
    label2->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label2->setJustificationType (juce::Justification::centredLeft);
    label2->setEditable (false, false, false);
    label2->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label2->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));


    //[UserPreSize]
	c_samples->addListener(this);
	c_cycles->addListener(this);
	c_Cancel->addShortcut(KeyPress(KeyPress::escapeKey));
    //[/UserPreSize]

    setSize (400, 60);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

VASTManualScaleEditor::~VASTManualScaleEditor()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    c_cycles = nullptr;
    c_OK = nullptr;
    c_Cancel = nullptr;
    c_samples = nullptr;
    label = nullptr;
    label2 = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void VASTManualScaleEditor::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void VASTManualScaleEditor::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    c_cycles->setBounds (proportionOfWidth (0.3800f), proportionOfHeight (0.5000f), proportionOfWidth (0.3000f), proportionOfHeight (0.3000f));
    c_OK->setBounds (proportionOfWidth (0.7200f), proportionOfHeight (0.5333f), proportionOfWidth (0.1250f), proportionOfHeight (0.2500f));
    c_Cancel->setBounds (proportionOfWidth (0.8600f), proportionOfHeight (0.5333f), proportionOfWidth (0.1250f), proportionOfHeight (0.2500f));
    c_samples->setBounds (proportionOfWidth (0.3800f), proportionOfHeight (0.1333f), proportionOfWidth (0.3000f), proportionOfHeight (0.3000f));
    label->setBounds (proportionOfWidth (0.0200f), proportionOfHeight (0.1833f), proportionOfWidth (0.3750f), proportionOfHeight (0.2000f));
    label2->setBounds (proportionOfWidth (0.0200f), proportionOfHeight (0.5667f), proportionOfWidth (0.3750f), proportionOfHeight (0.2000f));
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void VASTManualScaleEditor::buttonClicked (juce::Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == c_OK.get())
    {
        //[UserButtonCode_c_OK] -- add your button handler code here..

		int lCycles = c_cycles->getText().getIntValue();
		int lSamples = c_samples->getText().getIntValue();
		VASTSamplerSound* samplerSound = mWTEditor->getCurSamplerSound();
		if (samplerSound == nullptr) {
			getParentComponent()->exitModalState(0);
			getParentComponent()->grabKeyboardFocus();
			return;
		}

		if ((mSamplerViewport->getSelection()->iWavSelectionStartSample < 0) || (mSamplerViewport->getSelection()->iWavSelectionEndSample < 0)) mSamplerViewport->selectAll();
		int lChannel = 0; //left only?
		auto* subBufferTarget = samplerSound->getAudioDataChanged()->getReadPointer(lChannel, mSamplerViewport->getSelection()->iWavSelectionStartSample);
		int selectionLength = mSamplerViewport->getSelection()->iWavSelectionEndSample - mSamplerViewport->getSelection()->iWavSelectionStartSample + 1;
		//int selectionLength = mSamplerViewport->getSelection()->iWavSelectionEndSample - mSamplerViewport->getSelection()->iWavSelectionStartSample;
		int fixedSamplesPerCycle = lSamples;

		if ((lCycles >= 1) && (lCycles <= C_MAX_NUM_POSITIONS)) {
			myProcessor->m_pVASTXperience.m_Poly.m_OscBank[mWTEditor->m_bank]->addSoftFadeEditor();
			std::shared_ptr<CVASTWaveTable> wavetableupdate = myProcessor->m_pVASTXperience.m_Poly.m_OscBank[mWTEditor->m_bank]->getSoftOrCopyWavetable();
			wavetableupdate->clear(); //delete all positions
			mWTEditor->scaleAudioBufferToWTPos(wavetableupdate.get(), mWTEditor->getWtPos(), lCycles, subBufferTarget, selectionLength, fixedSamplesPerCycle);
			mWTEditor->updateAll(false);
			myProcessor->m_pVASTXperience.m_Poly.m_OscBank[mWTEditor->m_bank]->setWavetableSoftFade(wavetableupdate);
			myProcessor->m_pVASTXperience.m_Poly.m_OscBank[mWTEditor->m_bank]->removeSoftFadeEditor();
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



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void VASTManualScaleEditor::textEditorReturnKeyPressed(TextEditor& textEditorThatWasChanged) {
	if (&textEditorThatWasChanged == c_cycles.get())
	{
		c_OK->triggerClick();
	}
}
void VASTManualScaleEditor::textEditorEscapeKeyPressed(TextEditor& textEditorThatWasChanged) {
	c_Cancel->triggerClick();
}
void VASTManualScaleEditor::setSamples(String text) {
    c_samples->setText(text, NotificationType::sendNotification);
    c_samples->applyFontToAllText(((VASTLookAndFeel*)&getLookAndFeel())->getTextEditorFont(*c_samples));
    c_samples->selectAll();
}
void VASTManualScaleEditor::setCycles(String text) {
    c_cycles->setText(text, NotificationType::sendNotification);
    c_cycles->applyFontToAllText(((VASTLookAndFeel*)&getLookAndFeel())->getTextEditorFont(*c_cycles));
    c_cycles->selectAll();
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="VASTManualScaleEditor" componentName=""
                 parentClasses="public Component, public TextEditor::Listener"
                 constructorParams="VASTAudioProcessor* processor, VASTWaveTableEditorComponent* wtEditor, VASTSamplerViewport* samplerViewport"
                 variableInitialisers="myProcessor(processor), mWTEditor(wtEditor), mSamplerViewport(samplerViewport)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="400" initialHeight="60">
  <BACKGROUND backgroundColour="ff323e44"/>
  <TEXTEDITOR name="c_cycles" id="56e34f616e36cd4f" memberName="c_cycles" virtualName=""
              explicitFocusOrder="1" pos="38% 50% 30% 30%" initialText="" multiline="0"
              retKeyStartsLine="0" readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <TEXTBUTTON name="c_OK" id="8746701f60483199" memberName="c_OK" virtualName=""
              explicitFocusOrder="0" pos="72% 53.333% 12.5% 25%" buttonText="OK"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="c_Cancel" id="a6f8f52e5b0cabd3" memberName="c_Cancel" virtualName=""
              explicitFocusOrder="0" pos="86% 53.333% 12.5% 25%" buttonText="Cancel"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTEDITOR name="c_samples" id="1e422cce67ddce8b" memberName="c_samples"
              virtualName="" explicitFocusOrder="1" pos="38% 13.333% 30% 30%"
              initialText="" multiline="0" retKeyStartsLine="0" readonly="0"
              scrollbars="1" caret="1" popupmenu="1"/>
  <LABEL name="new label" id="7b05d0c01dfdce00" memberName="label" virtualName=""
         explicitFocusOrder="0" pos="2% 18.333% 37.5% 20%" edTextCol="ff000000"
         edBkgCol="0" labelText="Chop after #samples" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="5f08c2b69ae7655a" memberName="label2" virtualName=""
         explicitFocusOrder="0" pos="2% 56.667% 37.5% 20%" edTextCol="ff000000"
         edBkgCol="0" labelText="Scale into #cycles" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

