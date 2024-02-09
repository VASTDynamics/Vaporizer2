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
#include "../Engine/VASTSampler.h"
#include "VASTControls/VASTButton.h"
#include "VASTControls/VASTSlider.h"
#include "VASTScopeDisplay/VASTOscilloscope.h"
#include "VASTWTEditor/VASTSamplerViewport.h"
#include "VASTWaveTableEditorComponent.h"
#include <thread>
//[/Headers]

#include "VASTSamplerEditorComponentHeader.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
VASTSamplerEditorComponentHeader::VASTSamplerEditorComponentHeader (VASTAudioProcessorEditor *editor, VASTAudioProcessor* processor, VASTWaveTableEditorComponent *wteditor)
    : myEditor((VASTAudioProcessorEditor*)editor), myProcessor((VASTAudioProcessor*)processor), myWTEditor(wteditor)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    m_iWAVScale.reset (new VASTParameterSlider ("m_iWAVScale"));
    addAndMakeVisible (m_iWAVScale.get());
    m_iWAVScale->setTooltip (TRANS ("Display scaling of wav file"));
    m_iWAVScale->setRange (1, 256, 1);
    m_iWAVScale->setSliderStyle (juce::Slider::IncDecButtons);
    m_iWAVScale->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 28, 20);
    m_iWAVScale->addListener (this);

    c_wavName.reset (new juce::TextEditor ("c_wavName"));
    addAndMakeVisible (c_wavName.get());
    c_wavName->setMultiLine (false);
    c_wavName->setReturnKeyStartsNewLine (false);
    c_wavName->setReadOnly (false);
    c_wavName->setScrollbarsShown (false);
    c_wavName->setCaretVisible (true);
    c_wavName->setPopupMenuEnabled (true);
    c_wavName->setText (juce::String());

    label14.reset (new juce::Label ("new label",
                                    TRANS ("WAV")));
    addAndMakeVisible (label14.get());
    label14->setFont (juce::Font ("Code Pro Demo", 11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label14->setJustificationType (juce::Justification::centredRight);
    label14->setEditable (false, false, false);
    label14->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label14->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label14->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    m_iWavWTPos.reset (new VASTParameterSlider ("m_iWavWTPos"));
    addAndMakeVisible (m_iWavWTPos.get());
    m_iWavWTPos->setTooltip (TRANS ("Number of positions to create from wav file"));
    m_iWavWTPos->setRange (1, 256, 1);
    m_iWavWTPos->setSliderStyle (juce::Slider::IncDecButtons);
    m_iWavWTPos->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 28, 20);
    m_iWavWTPos->addListener (this);

    label16.reset (new juce::Label ("new label",
                                    TRANS ("FIT")));
    addAndMakeVisible (label16.get());
    label16->setFont (juce::Font ("Code Pro Demo", 11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label16->setJustificationType (juce::Justification::centredLeft);
    label16->setEditable (false, false, false);
    label16->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label16->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label16->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    c_iconLoadWav.reset (new VASTDrawableButton ("c_iconLoadWav",load_svg, load_svgSize, "Load wav file to sampler"));
    addAndMakeVisible (c_iconLoadWav.get());
    c_iconLoadWav->setName ("c_iconLoadWav");

    c_iconToEditor.reset (new VASTDrawableButton ("c_iconToEditor", to_editor_svg, to_editor_svgSize, "Resample wav to wavetables"));
    addAndMakeVisible (c_iconToEditor.get());
    c_iconToEditor->setName ("c_iconToEditor");

    c_iconClearWav.reset (new VASTDrawableButton ("c_iconClearWav",close_svg, close_svgSize, "Clear sampler sound"));
    addAndMakeVisible (c_iconClearWav.get());
    c_iconClearWav->setName ("c_iconClearWav");

    c_iconNormalizeWav.reset (new VASTDrawableButton ("c_iconNormalizeWav",normalize3_svg, normalize3_svgSize, "Normalize selection"));
    addAndMakeVisible (c_iconNormalizeWav.get());
    c_iconNormalizeWav->setName ("c_iconNormalizeWav");

    c_iconMaximizeSampler.reset (new VASTDrawableButton ("c_iconMaximizeSampler", arrow_right_corner_svg, arrow_right_corner_svgSize, "Maximize area"));
    addAndMakeVisible (c_iconMaximizeSampler.get());
    c_iconMaximizeSampler->setName ("c_iconMaximizeSampler");

    c_iconZeroLoop.reset (new VASTDrawableButton ("c_iconZeroLoop",zero_find_svg, zero_find_svgSize, "Find loop points at zero crossings"));
    addAndMakeVisible (c_iconZeroLoop.get());
    c_iconZeroLoop->setName ("c_iconZeroLoop");

    label2.reset (new juce::Label ("new label",
                                   TRANS ("SNAP")));
    addAndMakeVisible (label2.get());
    label2->setFont (juce::Font ("Code Pro Demo", 11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label2->setJustificationType (juce::Justification::centredLeft);
    label2->setEditable (false, false, false);
    label2->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label2->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label2->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    c_snap.reset (new juce::ToggleButton ("c_snap"));
    addAndMakeVisible (c_snap.get());
    c_snap->setTooltip (TRANS ("Snap editor functions to zero crossings "));
    c_snap->setButtonText (juce::String());
    c_snap->addListener (this);

    drawable1 = juce::Drawable::createFromImageData (zoom_in_svg, zoom_in_svgSize);

    //[UserPreSize]
	this->setComponentID("SamplerEditorHeader");

	c_iconClearWav->addListener(myWTEditor);
	c_iconLoadWav->addListener(myWTEditor);
	c_iconToEditor->addListener(myWTEditor);
	c_iconZeroLoop->addListener(myWTEditor);
	c_iconNormalizeWav->addListener(myWTEditor);
	c_iconMaximizeSampler->addListener(myWTEditor);

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

	m_iWavWTPos->setIncDecButtonsMode(Slider::incDecButtonsDraggable_Vertical);
	m_iWAVScale->setIncDecButtonsMode(Slider::incDecButtonsDraggable_Vertical);

	c_wavName->applyFontToAllText(myEditor->getCurrentVASTLookAndFeel()->myDefaultFontBold, true); //after bindings
	c_wavName->onTextChange = [this]() {
		c_wavName->applyFontToAllText(myEditor->getCurrentVASTLookAndFeel()->getTextEditorFont(*c_wavName.get()));
	};
	c_wavName->addListener(myWTEditor);
	c_wavName->setComponentID("textEditor"); //hack for l&f

	setOpaque(true);

    return; //dont call setSize
    //[/UserPreSize]

    setSize (800, 20);


    //[Constructor] You can add your own custom stuff here..
	//setLookAndFeel(myEditor->getCurrentVASTLookAndFeel());
    //[/Constructor]
}

VASTSamplerEditorComponentHeader::~VASTSamplerEditorComponentHeader()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
	this->setLookAndFeel(nullptr);
    //[/Destructor_pre]

    m_iWAVScale = nullptr;
    c_wavName = nullptr;
    label14 = nullptr;
    m_iWavWTPos = nullptr;
    label16 = nullptr;
    c_iconLoadWav = nullptr;
    c_iconToEditor = nullptr;
    c_iconClearWav = nullptr;
    c_iconNormalizeWav = nullptr;
    c_iconMaximizeSampler = nullptr;
    c_iconZeroLoop = nullptr;
    label2 = nullptr;
    c_snap = nullptr;
    drawable1 = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void VASTSamplerEditorComponentHeader::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff323e44));

    {
        int x = 0, y = 0, width = getWidth() - 0, height = getHeight() - 0;
        juce::Colour fillColour1 = juce::Colour (0x4fa2a7b5), fillColour2 = juce::Colour (0xbd141618);
        //[UserPaintCustomArguments] Customize the painting arguments here..
    		fillColour1 = myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colWTEditorHeaderBackgroundGradientFrom);
    		fillColour2 = myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colWTEditorHeaderBackgroundGradientTo);
        //[/UserPaintCustomArguments]
        g.setGradientFill (juce::ColourGradient (fillColour1,
                                             static_cast<float> (proportionOfWidth (-0.0200f)) - 0.0f + x,
                                             static_cast<float> (proportionOfHeight (-0.4000f)) - 0.0f + y,
                                             fillColour2,
                                             static_cast<float> (proportionOfWidth (1.0200f)) - 0.0f + x,
                                             static_cast<float> (proportionOfHeight (1.6000f)) - 0.0f + y,
                                             false));
        g.fillRect (x, y, width, height);
    }

    {
        int x = proportionOfWidth (0.8730f), y = proportionOfHeight (0.0500f), width = proportionOfWidth (0.0150f), height = proportionOfHeight (0.8500f);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (juce::Colours::black);
        jassert (drawable1 != nullptr);
        if (drawable1 != nullptr)
            drawable1->drawWithin (g, juce::Rectangle<int> (x, y, width, height).toFloat(),
                                   juce::RectanglePlacement::centred, 1.000f);
    }

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void VASTSamplerEditorComponentHeader::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    m_iWAVScale->setBounds (proportionOfWidth (0.8938f), proportionOfHeight (0.1000f), proportionOfWidth (0.0788f), proportionOfHeight (0.8000f));
    c_wavName->setBounds (proportionOfWidth (0.2213f), proportionOfHeight (0.1500f), proportionOfWidth (0.3500f), proportionOfHeight (0.7500f));
    label14->setBounds (proportionOfWidth (0.1225f), proportionOfHeight (0.2000f), proportionOfWidth (0.0900f), proportionOfHeight (0.6000f));
    m_iWavWTPos->setBounds (proportionOfWidth (0.7063f), proportionOfHeight (0.1000f), proportionOfWidth (0.0788f), proportionOfHeight (0.8000f));
    label16->setBounds (proportionOfWidth (0.7500f), proportionOfHeight (0.0000f), proportionOfWidth (0.0775f), proportionOfHeight (0.0000f));
    c_iconLoadWav->setBounds (proportionOfWidth (0.0037f), proportionOfHeight (0.0500f), proportionOfWidth (0.0437f), proportionOfHeight (0.8500f));
    c_iconToEditor->setBounds (proportionOfWidth (0.6763f), proportionOfHeight (0.0500f), proportionOfWidth (0.0250f), proportionOfHeight (0.8500f));
    c_iconClearWav->setBounds (proportionOfWidth (0.0037f) + proportionOfWidth (0.0437f), proportionOfHeight (0.0500f), proportionOfWidth (0.0437f), proportionOfHeight (0.8500f));
    c_iconNormalizeWav->setBounds (proportionOfWidth (0.8300f), proportionOfHeight (0.0000f), proportionOfWidth (0.0250f), proportionOfHeight (0.8500f));
    c_iconMaximizeSampler->setBounds (getWidth() - 1 - proportionOfWidth (0.0150f), 1, proportionOfWidth (0.0150f), proportionOfHeight (0.6000f));
    c_iconZeroLoop->setBounds ((proportionOfWidth (0.0037f) + proportionOfWidth (0.0437f)) + proportionOfWidth (0.0437f), proportionOfHeight (0.0500f), proportionOfWidth (0.0437f), proportionOfHeight (0.8500f));
    label2->setBounds (proportionOfWidth (0.6012f), proportionOfHeight (0.2000f), proportionOfWidth (0.0775f), proportionOfHeight (0.6000f));
    c_snap->setBounds (proportionOfWidth (0.5825f), proportionOfHeight (0.0500f), proportionOfWidth (0.0187f), proportionOfHeight (0.8500f));
    //[UserResized] Add your own custom resize handling here..

	//c_editorOscilloscope->resized();


	//myWTEditor->m_samplerviewport->setZoomFactor(m_iWAVScale.get()->getValue());
	m_iWavWTPos->setTextBoxStyle(Slider::TextBoxLeft, true, m_iWavWTPos.get()->getWidth() * 0.45f, m_iWavWTPos.get()->getHeight());
	m_iWAVScale->setTextBoxStyle(Slider::TextBoxLeft, true, m_iWAVScale.get()->getWidth() * 0.45f, m_iWAVScale.get()->getHeight());
	c_wavName->applyFontToAllText(myEditor->getCurrentVASTLookAndFeel()->getTextEditorFont(*c_wavName.get()));
    //[/UserResized]
}

void VASTSamplerEditorComponentHeader::sliderValueChanged (juce::Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == m_iWAVScale.get())
    {
        //[UserSliderCode_m_iWAVScale] -- add your slider handling code here..
		if (myWTEditor->m_samplerviewport != nullptr) //happend with mellow chop loop preset
			myWTEditor->m_samplerviewport->setZoomFactor(m_iWAVScale.get()->getValue());
		return; //dont update UI
        //[/UserSliderCode_m_iWAVScale]
    }
    else if (sliderThatWasMoved == m_iWavWTPos.get())
    {
        //[UserSliderCode_m_iWavWTPos] -- add your slider handling code here..
        //[/UserSliderCode_m_iWavWTPos]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void VASTSamplerEditorComponentHeader::buttonClicked (juce::Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == c_snap.get())
    {
        //[UserButtonCode_c_snap] -- add your button handler code here..
		myWTEditor->m_samplerviewport->updateContentAsync();
        //[/UserButtonCode_c_snap]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void VASTSamplerEditorComponentHeader::visibilityChanged()
{
    //[UserCode_visibilityChanged] -- Add your code here...
	if (this->isVisible()) { //skip on close
		if (myWTEditor->m_samplerviewport != nullptr)
			myWTEditor->m_samplerviewport->setZoomFactor(m_iWAVScale.get()->getValue());
	}
    //[/UserCode_visibilityChanged]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void VASTSamplerEditorComponentHeader::lookAndFeelChanged() {
	c_wavName->applyFontToAllText(myEditor->getCurrentVASTLookAndFeel()->getTextEditorFont(*c_wavName.get()));
	c_wavName->applyColourToAllText(myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colBoxText), true);
	c_wavName->setColour(TextEditor::backgroundColourId, myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colBoxTextBackground));
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="VASTSamplerEditorComponentHeader"
                 componentName="" parentClasses="public Component" constructorParams="VASTAudioProcessorEditor *editor, VASTAudioProcessor* processor, VASTWaveTableEditorComponent *wteditor"
                 variableInitialisers="myEditor((VASTAudioProcessorEditor*)editor), myProcessor((VASTAudioProcessor*)processor), myWTEditor(wteditor)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="800" initialHeight="20">
  <METHODS>
    <METHOD name="visibilityChanged()"/>
  </METHODS>
  <BACKGROUND backgroundColour="ff323e44">
    <RECT pos="0 0 0M 0M" fill="linear: -2% -40%, 102% 160%, 0=4fa2a7b5, 1=bd141618"
          hasStroke="0"/>
    <IMAGE pos="87.3% 5% 1.5% 85%" resource="zoom_in_svg" opacity="1.0"
           mode="1"/>
  </BACKGROUND>
  <SLIDER name="m_iWAVScale" id="230e5f41174ef54a" memberName="m_iWAVScale"
          virtualName="VASTParameterSlider" explicitFocusOrder="0" pos="89.375% 10% 7.875% 80%"
          tooltip="Display scaling of wav file" min="1.0" max="256.0" int="1.0"
          style="IncDecButtons" textBoxPos="TextBoxLeft" textBoxEditable="1"
          textBoxWidth="28" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <TEXTEDITOR name="c_wavName" id="5aaa5d00cb7f957f" memberName="c_wavName"
              virtualName="" explicitFocusOrder="0" pos="22.125% 15% 35% 75%"
              initialText="" multiline="0" retKeyStartsLine="0" readonly="0"
              scrollbars="0" caret="1" popupmenu="1"/>
  <LABEL name="new label" id="6ffa73d39d534afe" memberName="label14" virtualName=""
         explicitFocusOrder="0" pos="12.25% 20% 9% 60%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="WAV" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Code Pro Demo"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="34"/>
  <SLIDER name="m_iWavWTPos" id="1e174a6e2d3b3dff" memberName="m_iWavWTPos"
          virtualName="VASTParameterSlider" explicitFocusOrder="0" pos="70.625% 10% 7.875% 80%"
          tooltip="Number of positions to create from wav file" min="1.0"
          max="256.0" int="1.0" style="IncDecButtons" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="28" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="new label" id="a6f4bd268565f12a" memberName="label16" virtualName=""
         explicitFocusOrder="0" pos="75% 0% 7.75% 0%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="FIT" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Code Pro Demo"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <GENERICCOMPONENT name="c_iconLoadWav" id="910821e4edd1f116" memberName="c_iconLoadWav"
                    virtualName="" explicitFocusOrder="0" pos="0.375% 5% 4.375% 85%"
                    class="VASTDrawableButton" params="&quot;c_iconLoadWav&quot;,load_svg, load_svgSize, &quot;Load wav file to sampler&quot;"/>
  <GENERICCOMPONENT name="c_iconToEditor" id="707388016693c14f" memberName="c_iconToEditor"
                    virtualName="" explicitFocusOrder="0" pos="67.625% 5% 2.5% 85%"
                    class="VASTDrawableButton" params="&quot;c_iconToEditor&quot;, to_editor_svg, to_editor_svgSize, &quot;Resample wav to wavetables&quot;"/>
  <GENERICCOMPONENT name="c_iconClearWav" id="fb23372ad91f711f" memberName="c_iconClearWav"
                    virtualName="" explicitFocusOrder="0" pos="0R 5% 4.375% 85%"
                    posRelativeX="910821e4edd1f116" class="VASTDrawableButton" params="&quot;c_iconClearWav&quot;,close_svg, close_svgSize, &quot;Clear sampler sound&quot;"/>
  <GENERICCOMPONENT name="c_iconNormalizeWav" id="9b8db803e32ff61f" memberName="c_iconNormalizeWav"
                    virtualName="" explicitFocusOrder="0" pos="83% 0% 2.5% 85%" class="VASTDrawableButton"
                    params="&quot;c_iconNormalizeWav&quot;,normalize3_svg, normalize3_svgSize, &quot;Normalize selection&quot;"/>
  <GENERICCOMPONENT name="c_iconMaximizeSampler" id="ded548631676712b" memberName="c_iconMaximizeSampler"
                    virtualName="" explicitFocusOrder="0" pos="1Rr 1 1.5% 60%" class="VASTDrawableButton"
                    params="&quot;c_iconMaximizeSampler&quot;, arrow_right_corner_svg, arrow_right_corner_svgSize, &quot;Maximize area&quot;"/>
  <GENERICCOMPONENT name="c_iconZeroLoop" id="dd518adb93fb7363" memberName="c_iconZeroLoop"
                    virtualName="" explicitFocusOrder="0" pos="0R 5% 4.375% 85%"
                    posRelativeX="fb23372ad91f711f" class="VASTDrawableButton" params="&quot;c_iconZeroLoop&quot;,zero_find_svg, zero_find_svgSize, &quot;Find loop points at zero crossings&quot;"/>
  <LABEL name="new label" id="bc0c315892ddf7cb" memberName="label2" virtualName=""
         explicitFocusOrder="0" pos="60.125% 20% 7.75% 60%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="SNAP" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Code Pro Demo"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <TOGGLEBUTTON name="c_snap" id="ec7ed958fca494e8" memberName="c_snap" virtualName=""
                explicitFocusOrder="0" pos="58.25% 5% 1.875% 85%" tooltip="Snap editor functions to zero crossings "
                buttonText="" connectedEdges="0" needsCallback="1" radioGroupId="0"
                state="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif

//==============================================================================
// Binary resources - be careful not to edit any of these sections!

// JUCER_RESOURCE: close_svg, 1012, "../../Resources/Icons/close.svg"
static const unsigned char resource_VASTSamplerEditorComponentHeader_close_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,70,
45,56,34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,32,119,105,100,116,104,61,34,50,49,112,120,34,32,104,101,105,103,104,116,61,34,50,48,112,120,34,32,118,105,101,
119,66,111,120,61,34,48,32,48,32,50,49,32,50,48,34,32,118,101,114,115,105,111,110,61,34,49,46,49,34,32,120,109,108,110,115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,
48,48,47,115,118,103,34,32,120,109,108,110,115,58,120,108,105,110,107,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,120,108,105,110,107,34,62,10,32,32,32,32,60,
33,45,45,32,71,101,110,101,114,97,116,111,114,58,32,83,107,101,116,99,104,32,51,46,56,46,49,32,40,50,57,54,56,55,41,32,45,32,104,116,116,112,58,47,47,119,119,119,46,98,111,104,101,109,105,97,110,99,111,
100,105,110,103,46,99,111,109,47,115,107,101,116,99,104,32,45,45,62,10,32,32,32,32,60,116,105,116,108,101,62,99,108,111,115,101,32,91,35,49,53,51,56,93,60,47,116,105,116,108,101,62,10,32,32,32,32,60,100,
101,115,99,62,67,114,101,97,116,101,100,32,119,105,116,104,32,83,107,101,116,99,104,46,60,47,100,101,115,99,62,10,32,32,32,32,60,100,101,102,115,62,60,47,100,101,102,115,62,10,32,32,32,32,60,103,32,105,
100,61,34,80,97,103,101,45,49,34,32,115,116,114,111,107,101,61,34,110,111,110,101,34,32,115,116,114,111,107,101,45,119,105,100,116,104,61,34,49,34,32,102,105,108,108,61,34,110,111,110,101,34,32,102,105,
108,108,45,114,117,108,101,61,34,101,118,101,110,111,100,100,34,62,10,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,68,114,105,98,98,98,108,101,45,68,97,114,107,45,80,114,101,118,105,101,119,34,32,116,
114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,45,49,51,57,46,48,48,48,48,48,48,44,32,45,49,54,48,46,48,48,48,48,48,48,41,34,32,102,105,108,108,61,34,35,70,70,70,70,70,70,34,
62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,105,99,111,110,115,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,53,54,46,48,48,48,48,48,48,44,32,
49,54,48,46,48,48,48,48,48,48,41,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,60,112,97,116,104,32,100,61,34,77,56,53,46,49,44,49,56,32,76,49,48,49,46,57,44,49,56,32,76,49,48,49,46,57,44,50,
32,76,56,53,46,49,44,50,32,76,56,53,46,49,44,49,56,32,90,32,77,56,51,44,50,48,32,76,49,48,52,44,50,48,32,76,49,48,52,44,48,32,76,56,51,44,48,32,76,56,51,44,50,48,32,90,32,77,56,57,46,55,56,55,50,44,49,
49,46,56,53,54,32,76,57,50,46,48,49,52,50,53,44,57,46,55,51,53,32,76,56,57,46,55,56,55,50,44,55,46,54,49,51,32,76,57,49,46,50,55,49,57,44,54,46,49,57,57,32,76,57,51,46,53,44,56,46,51,50,49,32,76,57,53,
46,55,50,55,48,53,44,54,46,49,57,57,32,76,57,55,46,50,49,49,55,53,44,55,46,54,49,51,32,76,57,52,46,57,56,52,55,44,57,46,55,51,53,32,76,57,55,46,50,49,49,55,53,44,49,49,46,56,53,54,32,76,57,53,46,55,50,
55,48,53,44,49,51,46,50,55,32,76,57,51,46,53,44,49,49,46,49,52,57,32,76,57,49,46,50,55,49,57,44,49,51,46,50,55,32,76,56,57,46,55,56,55,50,44,49,49,46,56,53,54,32,90,34,32,105,100,61,34,99,108,111,115,
101,45,91,35,49,53,51,56,93,34,62,60,47,112,97,116,104,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,60,47,103,62,10,60,47,115,118,103,62,
0,0};

const char* VASTSamplerEditorComponentHeader::close_svg = (const char*) resource_VASTSamplerEditorComponentHeader_close_svg;
const int VASTSamplerEditorComponentHeader::close_svgSize = 1012;

// JUCER_RESOURCE: load_svg, 907, "../../Resources/Icons/load.svg"
static const unsigned char resource_VASTSamplerEditorComponentHeader_load_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,70,
45,56,34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,32,119,105,100,116,104,61,34,50,48,112,120,34,32,104,101,105,103,104,116,61,34,50,48,112,120,34,32,118,105,101,
119,66,111,120,61,34,48,32,48,32,50,48,32,50,48,34,32,118,101,114,115,105,111,110,61,34,49,46,49,34,32,120,109,108,110,115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,
48,48,47,115,118,103,34,32,120,109,108,110,115,58,120,108,105,110,107,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,120,108,105,110,107,34,62,10,32,32,32,32,60,
33,45,45,32,71,101,110,101,114,97,116,111,114,58,32,83,107,101,116,99,104,32,51,46,56,46,49,32,40,50,57,54,56,55,41,32,45,32,104,116,116,112,58,47,47,119,119,119,46,98,111,104,101,109,105,97,110,99,111,
100,105,110,103,46,99,111,109,47,115,107,101,116,99,104,32,45,45,62,10,32,32,32,32,60,116,105,116,108,101,62,98,114,111,119,115,101,114,32,91,35,50,51,51,93,60,47,116,105,116,108,101,62,10,32,32,32,32,
60,100,101,115,99,62,67,114,101,97,116,101,100,32,119,105,116,104,32,83,107,101,116,99,104,46,60,47,100,101,115,99,62,10,32,32,32,32,60,100,101,102,115,62,60,47,100,101,102,115,62,10,32,32,32,32,60,103,
32,105,100,61,34,80,97,103,101,45,49,34,32,115,116,114,111,107,101,61,34,110,111,110,101,34,32,115,116,114,111,107,101,45,119,105,100,116,104,61,34,49,34,32,102,105,108,108,61,34,110,111,110,101,34,32,
102,105,108,108,45,114,117,108,101,61,34,101,118,101,110,111,100,100,34,62,10,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,68,114,105,98,98,98,108,101,45,68,97,114,107,45,80,114,101,118,105,101,119,
34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,45,54,48,46,48,48,48,48,48,48,44,32,45,55,49,53,57,46,48,48,48,48,48,48,41,34,32,102,105,108,108,61,34,35,70,70,70,70,
70,70,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,105,99,111,110,115,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,53,54,46,48,48,48,48,48,
48,44,32,49,54,48,46,48,48,48,48,48,48,41,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,60,112,97,116,104,32,100,61,34,77,49,48,44,55,48,48,51,32,76,50,50,44,55,48,48,51,32,76,50,50,44,55,48,
48,49,32,76,49,48,44,55,48,48,49,32,76,49,48,44,55,48,48,51,32,90,32,77,54,44,55,48,49,55,32,76,50,50,44,55,48,49,55,32,76,50,50,44,55,48,48,53,32,76,54,44,55,48,48,53,32,76,54,44,55,48,49,55,32,90,32,
77,54,44,55,48,48,51,32,76,56,44,55,48,48,51,32,76,56,44,55,48,48,49,32,76,54,44,55,48,48,49,32,76,54,44,55,48,48,51,32,90,32,77,52,44,55,48,49,57,32,76,50,52,44,55,48,49,57,32,76,50,52,44,54,57,57,57,
32,76,52,44,54,57,57,57,32,76,52,44,55,48,49,57,32,90,34,32,105,100,61,34,98,114,111,119,115,101,114,45,91,35,50,51,51,93,34,62,60,47,112,97,116,104,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,47,103,
62,10,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,60,47,103,62,10,60,47,115,118,103,62,0,0};

const char* VASTSamplerEditorComponentHeader::load_svg = (const char*) resource_VASTSamplerEditorComponentHeader_load_svg;
const int VASTSamplerEditorComponentHeader::load_svgSize = 907;

// JUCER_RESOURCE: normalize3_svg, 1964, "../../Resources/Icons/normalize3.svg"
static const unsigned char resource_VASTSamplerEditorComponentHeader_normalize3_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,
84,70,45,56,34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,32,119,105,100,116,104,61,34,50,48,112,120,34,32,104,101,105,103,104,116,61,34,50,48,112,120,34,32,118,
105,101,119,66,111,120,61,34,48,32,48,32,50,48,32,50,48,34,32,118,101,114,115,105,111,110,61,34,49,46,49,34,32,120,109,108,110,115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,
50,48,48,48,47,115,118,103,34,32,120,109,108,110,115,58,120,108,105,110,107,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,120,108,105,110,107,34,62,10,32,32,32,
32,60,33,45,45,32,71,101,110,101,114,97,116,111,114,58,32,83,107,101,116,99,104,32,51,46,56,46,49,32,40,50,57,54,56,55,41,32,45,32,104,116,116,112,58,47,47,119,119,119,46,98,111,104,101,109,105,97,110,
99,111,100,105,110,103,46,99,111,109,47,115,107,101,116,99,104,32,45,45,62,10,32,32,32,32,60,116,105,116,108,101,62,97,114,114,111,119,95,105,110,115,105,100,101,32,91,35,50,54,49,93,60,47,116,105,116,
108,101,62,10,32,32,32,32,60,100,101,115,99,62,67,114,101,97,116,101,100,32,119,105,116,104,32,83,107,101,116,99,104,46,60,47,100,101,115,99,62,10,32,32,32,32,60,100,101,102,115,62,60,47,100,101,102,115,
62,10,32,32,32,32,60,103,32,105,100,61,34,80,97,103,101,45,49,34,32,115,116,114,111,107,101,61,34,110,111,110,101,34,32,115,116,114,111,107,101,45,119,105,100,116,104,61,34,49,34,32,102,105,108,108,61,
34,110,111,110,101,34,32,102,105,108,108,45,114,117,108,101,61,34,101,118,101,110,111,100,100,34,62,10,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,68,114,105,98,98,98,108,101,45,68,97,114,107,45,80,
114,101,118,105,101,119,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,45,49,48,48,46,48,48,48,48,48,48,44,32,45,54,57,57,57,46,48,48,48,48,48,48,41,34,32,102,105,
108,108,61,34,35,70,70,70,70,70,70,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,105,99,111,110,115,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,
101,40,53,54,46,48,48,48,48,48,48,44,32,49,54,48,46,48,48,48,48,48,48,41,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,60,112,97,116,104,32,100,61,34,77,54,51,44,54,56,53,55,32,76,53,56,44,
54,56,53,55,32,76,53,56,44,54,56,53,50,46,56,54,51,32,67,53,56,44,54,56,53,50,46,52,49,56,32,53,56,46,53,51,57,44,54,56,53,50,46,49,57,53,32,53,56,46,56,53,52,44,54,56,53,50,46,53,49,32,76,54,48,46,53,
51,54,44,54,56,53,52,46,49,57,51,32,67,54,48,46,57,50,54,44,54,56,53,52,46,53,56,51,32,54,49,46,53,54,44,54,56,53,52,46,53,56,51,32,54,49,46,57,53,44,54,56,53,52,46,49,57,51,32,67,54,50,46,51,52,49,44,
54,56,53,51,46,56,48,50,32,54,50,46,51,52,49,44,54,56,53,51,46,49,54,57,32,54,49,46,57,53,44,54,56,53,50,46,55,55,57,32,76,53,56,46,52,49,51,44,54,56,52,57,46,50,52,50,32,67,53,55,46,54,51,51,44,54,56,
52,56,46,52,54,49,32,53,54,46,51,54,55,44,54,56,52,56,46,52,54,49,32,53,53,46,53,56,55,44,54,56,52,57,46,50,52,50,32,67,53,53,46,48,50,49,44,54,56,52,57,46,56,48,56,32,53,52,46,57,48,49,44,54,56,52,57,
46,57,50,55,32,53,50,46,48,53,44,54,56,53,50,46,55,55,56,32,67,53,49,46,54,54,44,54,56,53,51,46,49,54,57,32,53,49,46,54,54,44,54,56,53,51,46,56,48,50,32,53,50,46,48,53,49,44,54,56,53,52,46,49,57,51,32,
67,53,50,46,52,52,49,44,54,56,53,52,46,53,56,51,32,53,51,46,48,55,52,44,54,56,53,52,46,53,56,51,32,53,51,46,52,54,53,44,54,56,53,52,46,49,57,51,32,76,53,53,46,49,52,54,44,54,56,53,50,46,53,49,50,32,67,
53,53,46,52,54,49,44,54,56,53,50,46,49,57,55,32,53,54,44,54,56,53,50,46,52,50,32,53,54,44,54,56,53,50,46,56,54,53,32,76,53,54,44,54,56,53,55,32,76,52,53,44,54,56,53,55,32,67,52,52,46,52,52,56,44,54,56,
53,55,32,52,52,44,54,56,53,55,46,52,52,56,32,52,52,44,54,56,53,56,32,67,52,52,44,54,56,53,56,46,53,53,50,32,52,52,46,52,52,56,44,54,56,53,57,32,52,53,44,54,56,53,57,32,76,54,51,44,54,56,53,57,32,67,54,
51,46,53,53,50,44,54,56,53,57,32,54,52,44,54,56,53,56,46,53,53,50,32,54,52,44,54,56,53,56,32,67,54,52,44,54,56,53,55,46,52,52,56,32,54,51,46,53,53,50,44,54,56,53,55,32,54,51,44,54,56,53,55,32,77,52,53,
44,54,56,52,49,32,76,53,48,44,54,56,52,49,32,76,53,48,44,54,56,52,53,46,49,51,55,32,67,53,48,44,54,56,52,53,46,53,56,50,32,52,57,46,52,54,49,44,54,56,52,53,46,56,48,53,32,52,57,46,49,52,54,44,54,56,52,
53,46,52,57,32,76,52,55,46,52,54,52,44,54,56,52,51,46,56,48,55,32,67,52,55,46,48,55,52,44,54,56,52,51,46,52,49,55,32,52,54,46,52,52,44,54,56,52,51,46,52,49,55,32,52,54,46,48,53,44,54,56,52,51,46,56,48,
55,32,67,52,53,46,54,54,44,54,56,52,52,46,49,57,56,32,52,53,46,54,54,44,54,56,52,52,46,56,51,49,32,52,54,46,48,53,44,54,56,52,53,46,50,50,49,32,76,52,57,46,53,56,55,44,54,56,52,56,46,55,53,56,32,67,53,
48,46,51,54,56,44,54,56,52,57,46,53,51,57,32,53,49,46,54,51,51,44,54,56,52,57,46,53,51,57,32,53,50,46,52,49,51,44,54,56,52,56,46,55,53,56,32,67,53,50,46,57,56,51,44,54,56,52,56,46,49,56,57,32,53,51,46,
48,57,56,44,54,56,52,56,46,48,55,52,32,53,53,46,57,53,44,54,56,52,53,46,50,50,50,32,67,53,54,46,51,52,44,54,56,52,52,46,56,51,49,32,53,54,46,51,52,44,54,56,52,52,46,49,57,56,32,53,53,46,57,53,44,54,56,
52,51,46,56,48,55,32,67,53,53,46,53,53,57,44,54,56,52,51,46,52,49,55,32,53,52,46,57,50,54,44,54,56,52,51,46,52,49,55,32,53,52,46,53,51,53,44,54,56,52,51,46,56,48,55,32,76,53,50,46,56,53,52,44,54,56,52,
53,46,52,56,56,32,67,53,50,46,53,51,57,44,54,56,52,53,46,56,48,51,32,53,50,44,54,56,52,53,46,53,56,32,53,50,44,54,56,52,53,46,49,51,53,32,76,53,50,44,54,56,52,49,32,76,54,51,44,54,56,52,49,32,67,54,51,
46,53,53,50,44,54,56,52,49,32,54,52,44,54,56,52,48,46,53,53,50,32,54,52,44,54,56,52,48,32,67,54,52,44,54,56,51,57,46,52,52,56,32,54,51,46,53,53,50,44,54,56,51,57,32,54,51,44,54,56,51,57,32,76,52,53,44,
54,56,51,57,32,67,52,52,46,52,52,56,44,54,56,51,57,32,52,52,44,54,56,51,57,46,52,52,56,32,52,52,44,54,56,52,48,32,67,52,52,44,54,56,52,48,46,53,53,50,32,52,52,46,52,52,56,44,54,56,52,49,32,52,53,44,54,
56,52,49,34,32,105,100,61,34,97,114,114,111,119,95,105,110,115,105,100,101,45,91,35,50,54,49,93,34,62,60,47,112,97,116,104,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,32,32,32,
32,60,47,103,62,10,32,32,32,32,60,47,103,62,10,60,47,115,118,103,62,0,0};

const char* VASTSamplerEditorComponentHeader::normalize3_svg = (const char*) resource_VASTSamplerEditorComponentHeader_normalize3_svg;
const int VASTSamplerEditorComponentHeader::normalize3_svgSize = 1964;

// JUCER_RESOURCE: to_editor_svg, 1192, "../../Resources/Icons/to_editor.svg"
static const unsigned char resource_VASTSamplerEditorComponentHeader_to_editor_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,
70,45,56,34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,32,119,105,100,116,104,61,34,50,48,112,120,34,32,104,101,105,103,104,116,61,34,57,112,120,34,32,118,105,101,
119,66,111,120,61,34,48,32,48,32,50,48,32,57,34,32,118,101,114,115,105,111,110,61,34,49,46,49,34,32,120,109,108,110,115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,
48,47,115,118,103,34,32,120,109,108,110,115,58,120,108,105,110,107,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,120,108,105,110,107,34,62,10,32,32,32,32,60,33,
45,45,32,71,101,110,101,114,97,116,111,114,58,32,83,107,101,116,99,104,32,51,46,56,46,49,32,40,50,57,54,56,55,41,32,45,32,104,116,116,112,58,47,47,119,119,119,46,98,111,104,101,109,105,97,110,99,111,100,
105,110,103,46,99,111,109,47,115,107,101,116,99,104,32,45,45,62,10,32,32,32,32,60,116,105,116,108,101,62,97,99,116,105,118,105,116,121,32,91,35,57,56,53,93,60,47,116,105,116,108,101,62,10,32,32,32,32,
60,100,101,115,99,62,67,114,101,97,116,101,100,32,119,105,116,104,32,83,107,101,116,99,104,46,60,47,100,101,115,99,62,10,32,32,32,32,60,100,101,102,115,62,60,47,100,101,102,115,62,10,32,32,32,32,60,103,
32,105,100,61,34,80,97,103,101,45,49,34,32,115,116,114,111,107,101,61,34,110,111,110,101,34,32,115,116,114,111,107,101,45,119,105,100,116,104,61,34,49,34,32,102,105,108,108,61,34,110,111,110,101,34,32,
102,105,108,108,45,114,117,108,101,61,34,101,118,101,110,111,100,100,34,62,10,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,68,114,105,98,98,98,108,101,45,68,97,114,107,45,80,114,101,118,105,101,119,
34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,45,50,54,48,46,48,48,48,48,48,48,44,32,45,51,56,52,53,46,48,48,48,48,48,48,41,34,32,102,105,108,108,61,34,35,70,70,70,
70,70,70,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,105,99,111,110,115,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,53,54,46,48,48,48,48,
48,48,44,32,49,54,48,46,48,48,48,48,48,48,41,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,60,112,97,116,104,32,100,61,34,77,50,49,54,46,53,49,50,44,51,54,56,53,32,76,50,49,48,46,55,57,54,44,
51,54,57,48,46,55,57,32,76,50,48,56,46,48,53,49,44,51,54,56,56,46,49,53,53,32,76,50,48,56,46,48,52,52,44,51,54,56,56,46,49,54,50,32,76,50,48,56,46,48,51,56,44,51,54,56,56,46,49,53,54,32,76,50,48,52,44,
51,54,57,50,46,51,56,54,32,76,50,48,53,46,52,53,51,44,51,54,57,51,46,56,32,76,50,48,56,46,49,49,53,44,51,54,57,49,46,48,51,51,32,76,50,49,48,46,56,53,57,44,51,54,57,51,46,54,53,51,32,76,50,49,54,46,53,
49,50,44,51,54,56,55,46,56,56,53,32,76,50,49,57,46,50,53,50,44,51,54,57,48,46,54,54,32,67,50,49,57,46,53,52,53,44,51,54,57,48,46,48,50,56,32,50,50,48,46,48,52,54,44,51,54,56,57,46,53,49,52,32,50,50,48,
46,54,54,54,44,51,54,56,57,46,50,48,56,32,76,50,49,54,46,53,49,50,44,51,54,56,53,32,90,32,77,50,50,52,44,51,54,57,49,46,57,54,32,67,50,50,52,44,51,54,57,51,46,48,56,55,32,50,50,51,46,48,57,56,44,51,54,
57,52,32,50,50,49,46,57,56,54,44,51,54,57,52,32,67,50,50,48,46,56,55,52,44,51,54,57,52,32,50,49,57,46,57,55,51,44,51,54,57,51,46,48,56,55,32,50,49,57,46,57,55,51,44,51,54,57,49,46,57,54,32,67,50,49,57,
46,57,55,51,44,51,54,57,48,46,56,51,51,32,50,50,48,46,56,55,52,44,51,54,56,57,46,57,50,32,50,50,49,46,57,56,54,44,51,54,56,57,46,57,50,32,67,50,50,51,46,48,57,56,44,51,54,56,57,46,57,50,32,50,50,52,44,
51,54,57,48,46,56,51,51,32,50,50,52,44,51,54,57,49,46,57,54,32,76,50,50,52,44,51,54,57,49,46,57,54,32,90,34,32,105,100,61,34,97,99,116,105,118,105,116,121,45,91,35,57,56,53,93,34,62,60,47,112,97,116,104,
62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,60,47,103,62,10,60,47,115,118,103,62,0,0};

const char* VASTSamplerEditorComponentHeader::to_editor_svg = (const char*) resource_VASTSamplerEditorComponentHeader_to_editor_svg;
const int VASTSamplerEditorComponentHeader::to_editor_svgSize = 1192;

// JUCER_RESOURCE: zero_find_svg, 1166, "../../Resources/Icons/zero_find.svg"
static const unsigned char resource_VASTSamplerEditorComponentHeader_zero_find_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,
70,45,56,34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,32,119,105,100,116,104,61,34,50,49,112,120,34,32,104,101,105,103,104,116,61,34,50,48,112,120,34,32,118,105,
101,119,66,111,120,61,34,48,32,48,32,50,49,32,50,48,34,32,118,101,114,115,105,111,110,61,34,49,46,49,34,32,120,109,108,110,115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,
48,48,48,47,115,118,103,34,32,120,109,108,110,115,58,120,108,105,110,107,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,120,108,105,110,107,34,62,10,32,32,32,32,
60,33,45,45,32,71,101,110,101,114,97,116,111,114,58,32,83,107,101,116,99,104,32,51,46,56,46,49,32,40,50,57,54,56,55,41,32,45,32,104,116,116,112,58,47,47,119,119,119,46,98,111,104,101,109,105,97,110,99,
111,100,105,110,103,46,99,111,109,47,115,107,101,116,99,104,32,45,45,62,10,32,32,32,32,60,116,105,116,108,101,62,115,101,97,114,99,104,95,108,101,102,116,32,91,35,49,53,48,52,93,60,47,116,105,116,108,
101,62,10,32,32,32,32,60,100,101,115,99,62,67,114,101,97,116,101,100,32,119,105,116,104,32,83,107,101,116,99,104,46,60,47,100,101,115,99,62,10,32,32,32,32,60,100,101,102,115,62,60,47,100,101,102,115,62,
10,32,32,32,32,60,103,32,105,100,61,34,80,97,103,101,45,49,34,32,115,116,114,111,107,101,61,34,110,111,110,101,34,32,115,116,114,111,107,101,45,119,105,100,116,104,61,34,49,34,32,102,105,108,108,61,34,
110,111,110,101,34,32,102,105,108,108,45,114,117,108,101,61,34,101,118,101,110,111,100,100,34,62,10,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,68,114,105,98,98,98,108,101,45,68,97,114,107,45,80,114,
101,118,105,101,119,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,45,50,57,57,46,48,48,48,48,48,48,44,32,45,50,56,48,46,48,48,48,48,48,48,41,34,32,102,105,108,108,
61,34,35,70,70,70,70,70,70,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,105,99,111,110,115,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,53,
54,46,48,48,48,48,48,48,44,32,49,54,48,46,48,48,48,48,48,48,41,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,60,112,97,116,104,32,100,61,34,77,50,54,52,44,49,51,56,46,53,56,54,32,76,50,54,50,
46,53,49,53,51,44,49,52,48,32,76,50,53,56,46,48,54,48,49,53,44,49,51,53,46,55,53,56,32,76,50,53,57,46,53,52,52,56,53,44,49,51,52,46,51,52,51,32,76,50,54,52,44,49,51,56,46,53,56,54,32,90,32,77,50,53,49,
46,52,44,49,51,52,32,67,50,52,55,46,57,50,54,54,44,49,51,52,32,50,52,53,46,49,44,49,51,49,46,51,48,57,32,50,52,53,46,49,44,49,50,56,32,67,50,52,53,46,49,44,49,50,52,46,54,57,50,32,50,52,55,46,57,50,54,
54,44,49,50,50,32,50,53,49,46,52,44,49,50,50,32,67,50,53,52,46,56,55,51,52,44,49,50,50,32,50,53,55,46,55,44,49,50,52,46,54,57,50,32,50,53,55,46,55,44,49,50,56,32,67,50,53,55,46,55,44,49,51,49,46,51,48,
57,32,50,53,52,46,56,55,51,52,44,49,51,52,32,50,53,49,46,52,44,49,51,52,32,76,50,53,49,46,52,44,49,51,52,32,90,32,77,50,53,49,46,52,44,49,50,48,32,67,50,52,54,46,55,54,49,49,44,49,50,48,32,50,52,51,44,
49,50,51,46,53,56,50,32,50,52,51,44,49,50,56,32,67,50,52,51,44,49,51,50,46,52,49,56,32,50,52,54,46,55,54,49,49,44,49,51,54,32,50,53,49,46,52,44,49,51,54,32,67,50,53,54,46,48,51,56,57,44,49,51,54,32,50,
53,57,46,56,44,49,51,50,46,52,49,56,32,50,53,57,46,56,44,49,50,56,32,67,50,53,57,46,56,44,49,50,51,46,53,56,50,32,50,53,54,46,48,51,56,57,44,49,50,48,32,50,53,49,46,52,44,49,50,48,32,76,50,53,49,46,52,
44,49,50,48,32,90,34,32,105,100,61,34,115,101,97,114,99,104,95,108,101,102,116,45,91,35,49,53,48,52,93,34,62,60,47,112,97,116,104,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,
32,32,32,32,60,47,103,62,10,32,32,32,32,60,47,103,62,10,60,47,115,118,103,62,0,0};

const char* VASTSamplerEditorComponentHeader::zero_find_svg = (const char*) resource_VASTSamplerEditorComponentHeader_zero_find_svg;
const int VASTSamplerEditorComponentHeader::zero_find_svgSize = 1166;

// JUCER_RESOURCE: zoom_in_svg, 1528, "../../Resources/Icons/zoom_in.svg"
static const unsigned char resource_VASTSamplerEditorComponentHeader_zoom_in_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,
70,45,56,34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,32,119,105,100,116,104,61,34,50,49,112,120,34,32,104,101,105,103,104,116,61,34,50,48,112,120,34,32,118,105,
101,119,66,111,120,61,34,48,32,48,32,50,49,32,50,48,34,32,118,101,114,115,105,111,110,61,34,49,46,49,34,32,120,109,108,110,115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,
48,48,48,47,115,118,103,34,32,120,109,108,110,115,58,120,108,105,110,107,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,120,108,105,110,107,34,62,10,32,32,32,32,
60,33,45,45,32,71,101,110,101,114,97,116,111,114,58,32,83,107,101,116,99,104,32,51,46,56,46,49,32,40,50,57,54,56,55,41,32,45,32,104,116,116,112,58,47,47,119,119,119,46,98,111,104,101,109,105,97,110,99,
111,100,105,110,103,46,99,111,109,47,115,107,101,116,99,104,32,45,45,62,10,32,32,32,32,60,116,105,116,108,101,62,122,111,111,109,95,105,110,32,91,35,49,52,54,49,93,60,47,116,105,116,108,101,62,10,32,32,
32,32,60,100,101,115,99,62,67,114,101,97,116,101,100,32,119,105,116,104,32,83,107,101,116,99,104,46,60,47,100,101,115,99,62,10,32,32,32,32,60,100,101,102,115,62,60,47,100,101,102,115,62,10,32,32,32,32,
60,103,32,105,100,61,34,80,97,103,101,45,49,34,32,115,116,114,111,107,101,61,34,110,111,110,101,34,32,115,116,114,111,107,101,45,119,105,100,116,104,61,34,49,34,32,102,105,108,108,61,34,110,111,110,101,
34,32,102,105,108,108,45,114,117,108,101,61,34,101,118,101,110,111,100,100,34,62,10,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,68,114,105,98,98,98,108,101,45,68,97,114,107,45,80,114,101,118,105,101,
119,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,45,52,49,57,46,48,48,48,48,48,48,44,32,45,52,52,48,46,48,48,48,48,48,48,41,34,32,102,105,108,108,61,34,35,70,70,
70,70,70,70,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,105,99,111,110,115,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,53,54,46,48,48,48,
48,48,48,44,32,49,54,48,46,48,48,48,48,48,48,41,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,60,112,97,116,104,32,100,61,34,77,51,55,50,46,52,53,44,50,56,54,46,57,54,55,52,49,57,32,76,51,55,
53,46,54,44,50,56,54,46,57,54,55,52,49,57,32,76,51,55,53,46,54,44,50,56,56,46,57,55,50,52,51,49,32,76,51,55,50,46,52,53,44,50,56,56,46,57,55,50,52,51,49,32,76,51,55,50,46,52,53,44,50,57,49,46,57,55,57,
57,53,32,76,51,55,48,46,51,53,44,50,57,49,46,57,55,57,57,53,32,76,51,55,48,46,51,53,44,50,56,56,46,57,55,50,52,51,49,32,76,51,54,55,46,50,44,50,56,56,46,57,55,50,52,51,49,32,76,51,54,55,46,50,44,50,56,
54,46,57,54,55,52,49,57,32,76,51,55,48,46,51,53,44,50,56,54,46,57,54,55,52,49,57,32,76,51,55,48,46,51,53,44,50,56,51,46,57,53,57,57,32,76,51,55,50,46,52,53,44,50,56,51,46,57,53,57,57,32,76,51,55,50,46,
52,53,44,50,56,54,46,57,54,55,52,49,57,32,90,32,77,51,56,52,44,50,57,56,46,53,56,50,52,53,54,32,76,51,56,50,46,53,49,53,51,44,51,48,48,32,76,51,55,56,46,48,54,48,49,53,44,50,57,53,46,55,52,54,51,54,54,
32,76,51,55,57,46,53,52,52,56,53,44,50,57,52,46,51,50,56,56,50,50,32,76,51,56,52,44,50,57,56,46,53,56,50,52,53,54,32,90,32,77,51,55,49,46,52,44,50,57,52,46,48,51,53,48,56,56,32,67,51,54,55,46,57,50,54,
54,44,50,57,52,46,48,51,53,48,56,56,32,51,54,53,46,49,44,50,57,49,46,51,51,55,51,52,51,32,51,54,53,46,49,44,50,56,56,46,48,50,48,48,53,32,67,51,54,53,46,49,44,50,56,52,46,55,48,51,55,53,57,32,51,54,55,
46,57,50,54,54,44,50,56,50,46,48,48,53,48,49,51,32,51,55,49,46,52,44,50,56,50,46,48,48,53,48,49,51,32,67,51,55,52,46,56,55,51,52,44,50,56,50,46,48,48,53,48,49,51,32,51,55,55,46,55,44,50,56,52,46,55,48,
51,55,53,57,32,51,55,55,46,55,44,50,56,56,46,48,50,48,48,53,32,67,51,55,55,46,55,44,50,57,49,46,51,51,55,51,52,51,32,51,55,52,46,56,55,51,52,44,50,57,52,46,48,51,53,48,56,56,32,51,55,49,46,52,44,50,57,
52,46,48,51,53,48,56,56,32,76,51,55,49,46,52,44,50,57,52,46,48,51,53,48,56,56,32,90,32,77,51,55,49,46,52,44,50,56,48,32,67,51,54,54,46,55,54,49,49,44,50,56,48,32,51,54,51,44,50,56,51,46,53,57,48,57,55,
55,32,51,54,51,44,50,56,56,46,48,50,48,48,53,32,67,51,54,51,44,50,57,50,46,52,53,48,49,50,53,32,51,54,54,46,55,54,49,49,44,50,57,54,46,48,52,48,49,32,51,55,49,46,52,44,50,57,54,46,48,52,48,49,32,67,51,
55,54,46,48,51,56,57,44,50,57,54,46,48,52,48,49,32,51,55,57,46,56,44,50,57,50,46,52,53,48,49,50,53,32,51,55,57,46,56,44,50,56,56,46,48,50,48,48,53,32,67,51,55,57,46,56,44,50,56,51,46,53,57,48,57,55,55,
32,51,55,54,46,48,51,56,57,44,50,56,48,32,51,55,49,46,52,44,50,56,48,32,76,51,55,49,46,52,44,50,56,48,32,90,34,32,105,100,61,34,122,111,111,109,95,105,110,45,91,35,49,52,54,49,93,34,62,60,47,112,97,116,
104,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,60,47,103,62,10,60,47,115,118,103,62,0,0};

const char* VASTSamplerEditorComponentHeader::zoom_in_svg = (const char*) resource_VASTSamplerEditorComponentHeader_zoom_in_svg;
const int VASTSamplerEditorComponentHeader::zoom_in_svgSize = 1528;

// JUCER_RESOURCE: arrow_right_corner_svg, 1715, "../../Resources/Icons/arrow_right_corner.svg"
static const unsigned char resource_VASTSamplerEditorComponentHeader_arrow_right_corner_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,
34,85,84,70,45,56,34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,32,119,105,100,116,104,61,34,50,48,112,120,34,32,104,101,105,103,104,116,61,34,50,48,112,120,34,32,
118,105,101,119,66,111,120,61,34,48,32,48,32,50,48,32,50,48,34,32,118,101,114,115,105,111,110,61,34,49,46,49,34,32,120,109,108,110,115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,
47,50,48,48,48,47,115,118,103,34,32,120,109,108,110,115,58,120,108,105,110,107,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,120,108,105,110,107,34,62,10,32,32,
32,32,60,33,45,45,32,71,101,110,101,114,97,116,111,114,58,32,83,107,101,116,99,104,32,51,46,56,46,49,32,40,50,57,54,56,55,41,32,45,32,104,116,116,112,58,47,47,119,119,119,46,98,111,104,101,109,105,97,
110,99,111,100,105,110,103,46,99,111,109,47,115,107,101,116,99,104,32,45,45,62,10,32,32,32,32,60,116,105,116,108,101,62,97,114,114,111,119,95,114,105,103,104,116,95,99,111,114,110,101,114,32,91,35,50,
55,57,93,60,47,116,105,116,108,101,62,10,32,32,32,32,60,100,101,115,99,62,67,114,101,97,116,101,100,32,119,105,116,104,32,83,107,101,116,99,104,46,60,47,100,101,115,99,62,10,32,32,32,32,60,100,101,102,
115,62,60,47,100,101,102,115,62,10,32,32,32,32,60,103,32,105,100,61,34,80,97,103,101,45,49,34,32,115,116,114,111,107,101,61,34,110,111,110,101,34,32,115,116,114,111,107,101,45,119,105,100,116,104,61,34,
49,34,32,102,105,108,108,61,34,110,111,110,101,34,32,102,105,108,108,45,114,117,108,101,61,34,101,118,101,110,111,100,100,34,62,10,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,68,114,105,98,98,98,108,
101,45,68,97,114,107,45,80,114,101,118,105,101,119,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,45,49,56,48,46,48,48,48,48,48,48,44,32,45,54,57,49,57,46,48,48,48,
48,48,48,41,34,32,102,105,108,108,61,34,35,70,70,70,70,70,70,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,105,99,111,110,115,34,32,116,114,97,110,115,102,111,114,109,61,34,116,
114,97,110,115,108,97,116,101,40,53,54,46,48,48,48,48,48,48,44,32,49,54,48,46,48,48,48,48,48,48,41,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,60,112,97,116,104,32,100,61,34,77,49,51,50,44,
54,55,54,52,32,67,49,51,50,44,54,55,54,51,46,52,52,56,32,49,51,50,46,52,52,56,44,54,55,54,51,32,49,51,51,44,54,55,54,51,32,76,49,51,56,44,54,55,54,51,32,67,49,51,57,46,49,48,53,44,54,55,54,51,32,49,52,
48,44,54,55,54,51,46,56,57,53,32,49,52,48,44,54,55,54,53,32,76,49,52,48,44,54,55,55,48,32,67,49,52,48,44,54,55,55,48,46,53,53,50,32,49,51,57,46,53,53,50,44,54,55,55,49,32,49,51,57,44,54,55,55,49,32,67,
49,51,56,46,52,52,56,44,54,55,55,49,32,49,51,56,44,54,55,55,48,46,53,53,50,32,49,51,56,44,54,55,55,48,32,76,49,51,56,44,54,55,54,55,46,54,50,49,32,67,49,51,56,44,54,55,54,55,46,49,55,54,32,49,51,55,46,
52,54,49,44,54,55,54,54,46,57,53,51,32,49,51,55,46,49,52,54,44,54,55,54,55,46,50,54,56,32,76,49,51,52,46,49,50,49,44,54,55,55,48,46,50,57,51,32,67,49,51,51,46,55,51,49,44,54,55,55,48,46,54,56,51,32,49,
51,51,46,48,57,56,44,54,55,55,48,46,54,56,51,32,49,51,50,46,55,48,55,44,54,55,55,48,46,50,57,51,32,67,49,51,50,46,51,49,55,44,54,55,54,57,46,57,48,50,32,49,51,50,46,51,49,55,44,54,55,54,57,46,50,54,57,
32,49,51,50,46,55,48,55,44,54,55,54,56,46,56,55,57,32,76,49,51,53,46,55,51,50,44,54,55,54,53,46,56,53,52,32,67,49,51,54,46,48,52,55,44,54,55,54,53,46,53,51,57,32,49,51,53,46,56,50,52,44,54,55,54,53,32,
49,51,53,46,51,55,57,44,54,55,54,53,32,76,49,51,51,44,54,55,54,53,32,67,49,51,50,46,52,52,56,44,54,55,54,53,32,49,51,50,44,54,55,54,52,46,53,53,50,32,49,51,50,44,54,55,54,52,32,76,49,51,50,44,54,55,54,
52,32,90,32,77,49,50,54,44,54,55,54,50,32,67,49,50,54,44,54,55,54,49,46,52,52,56,32,49,50,54,46,52,52,56,44,54,55,54,49,32,49,50,55,44,54,55,54,49,32,76,49,52,49,44,54,55,54,49,32,67,49,52,49,46,53,53,
50,44,54,55,54,49,32,49,52,50,44,54,55,54,49,46,52,52,56,32,49,52,50,44,54,55,54,50,32,76,49,52,50,44,54,55,55,54,32,67,49,52,50,44,54,55,55,54,46,53,53,50,32,49,52,49,46,53,53,50,44,54,55,55,55,32,49,
52,49,44,54,55,55,55,32,76,49,51,51,44,54,55,55,55,32,67,49,51,50,46,52,52,56,44,54,55,55,55,32,49,51,50,44,54,55,55,54,46,53,53,50,32,49,51,50,44,54,55,55,54,32,76,49,51,50,44,54,55,55,51,32,67,49,51,
50,44,54,55,55,49,46,56,57,53,32,49,51,49,46,49,48,53,44,54,55,55,49,32,49,51,48,44,54,55,55,49,32,76,49,50,55,44,54,55,55,49,32,67,49,50,54,46,52,52,56,44,54,55,55,49,32,49,50,54,44,54,55,55,48,46,53,
53,50,32,49,50,54,44,54,55,55,48,32,76,49,50,54,44,54,55,54,50,32,90,32,77,49,52,52,44,54,55,54,49,32,67,49,52,52,44,54,55,53,57,46,56,57,53,32,49,52,51,46,49,48,53,44,54,55,53,57,32,49,52,50,44,54,55,
53,57,32,76,49,50,54,44,54,55,53,57,32,67,49,50,52,46,56,57,53,44,54,55,53,57,32,49,50,52,44,54,55,53,57,46,56,57,53,32,49,50,52,44,54,55,54,49,32,76,49,50,52,44,54,55,55,55,32,67,49,50,52,44,54,55,55,
56,46,49,48,53,32,49,50,52,46,56,57,53,44,54,55,55,57,32,49,50,54,44,54,55,55,57,32,76,49,52,50,44,54,55,55,57,32,67,49,52,51,46,49,48,53,44,54,55,55,57,32,49,52,52,44,54,55,55,56,46,49,48,53,32,49,52,
52,44,54,55,55,55,32,76,49,52,52,44,54,55,54,49,32,90,34,32,105,100,61,34,97,114,114,111,119,95,114,105,103,104,116,95,99,111,114,110,101,114,45,91,35,50,55,57,93,34,62,60,47,112,97,116,104,62,10,32,32,
32,32,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,60,47,103,62,10,60,47,115,118,103,62,0,0};

const char* VASTSamplerEditorComponentHeader::arrow_right_corner_svg = (const char*) resource_VASTSamplerEditorComponentHeader_arrow_right_corner_svg;
const int VASTSamplerEditorComponentHeader::arrow_right_corner_svgSize = 1715;


//[EndFile] You can add extra defines here...
//[/EndFile]

