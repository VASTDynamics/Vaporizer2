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
#include "VASTControls/VASTComboBrev.h"
#include "VASTControls/VASTComboBox.h"
#include "VASTControls/VASTImageButton.h"
#include "VASTControls/VASTDrawableButton.h"
#include "VASTControls/VASTDragMatrix.h"
//[/Headers]

#include "VASTMatrixComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
VASTMatrixComponent::VASTMatrixComponent (AudioProcessorEditor *editor, AudioProcessor* processor)
    : myEditor((VASTAudioProcessorEditor*)editor), myProcessor((VASTAudioProcessor*)processor)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    c_iconDelSlots.reset (new VASTDrawableButton ("c_iconDelSlots", delete_svg, delete_svgSize, "Delete all matrix modulation slots" ));
    addAndMakeVisible (c_iconDelSlots.get());
    c_iconDelSlots->setName ("c_iconDelSlots");

    label.reset (new juce::Label ("new label",
                                  TRANS("SOURCE")));
    addAndMakeVisible (label.get());
    label->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label->setJustificationType (juce::Justification::centredLeft);
    label->setEditable (false, false, false);
    label->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label2.reset (new juce::Label ("new label",
                                   TRANS("DESTINATION")));
    addAndMakeVisible (label2.get());
    label2->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label2->setJustificationType (juce::Justification::centredLeft);
    label2->setEditable (false, false, false);
    label2->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label2->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label3.reset (new juce::Label ("new label",
                                   TRANS("POLARITY")));
    addAndMakeVisible (label3.get());
    label3->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label3->setJustificationType (juce::Justification::centredLeft);
    label3->setEditable (false, false, false);
    label3->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label3->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label4.reset (new juce::Label ("new label",
                                   TRANS("VALUE")));
    addAndMakeVisible (label4.get());
    label4->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label4->setJustificationType (juce::Justification::centredLeft);
    label4->setEditable (false, false, false);
    label4->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label4->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label5.reset (new juce::Label ("new label",
                                   TRANS("CURVE")));
    addAndMakeVisible (label5.get());
    label5->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label5->setJustificationType (juce::Justification::centredLeft);
    label5->setEditable (false, false, false);
    label5->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label5->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    c_iconMaximizeEditor.reset (new VASTDrawableButton ("c_iconMaximize", arrow_right_corner_svg, arrow_right_corner_svgSize, "Maximize area"));
    addAndMakeVisible (c_iconMaximizeEditor.get());
    c_iconMaximizeEditor->setName ("c_iconMaximizeEditor");


    //[UserPreSize]
	setOpaque(true);
    if (m_lazyInit == false)
        lazyInit();
    return; //dont call setSize
    //[/UserPreSize]

    setSize (800, 575);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

VASTMatrixComponent::~VASTMatrixComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    VDBG("Destructing VASTMatrixComponent");
	this->setLookAndFeel(nullptr);
    //[/Destructor_pre]

    c_iconDelSlots = nullptr;
    label = nullptr;
    label2 = nullptr;
    label3 = nullptr;
    label4 = nullptr;
    label5 = nullptr;
    c_iconMaximizeEditor = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void VASTMatrixComponent::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
	if (!m_lazyInit)
		lazyInit();
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff323e44));

    {
        int x = 0, y = 0, width = getWidth() - 0, height = getHeight() - 0;
        juce::Colour fillColour1 = juce::Colour (0xbd1c341a), fillColour2 = juce::Colour (0x4307200a);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        fillColour1 = myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colMatrixComponentBackgroundGradientFrom);
        fillColour2 = myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colMatrixComponentBackgroundGradientTo);
        //[/UserPaintCustomArguments]
        g.setGradientFill (juce::ColourGradient (fillColour1,
                                             static_cast<float> (proportionOfWidth (0.1000f)) - 0.0f + x,
                                             static_cast<float> (proportionOfHeight (0.3448f)) - 0.0f + y,
                                             fillColour2,
                                             static_cast<float> (proportionOfWidth (0.0800f)) - 0.0f + x,
                                             static_cast<float> (proportionOfHeight (0.1948f)) - 0.0f + y,
                                             false));
        g.fillRect (x, y, width, height);
    }

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void VASTMatrixComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
	if (dragMatrixArray.size() > 0) {
		float slotHeight = getHeight() / (dragMatrixArray.size() + 1);
		float inset = slotHeight * 0.05f;
		float componentHeight = slotHeight - 2 * inset;
		for (auto i = 0; i < dragMatrixArray.size(); ++i)
		{
			dragMatrixArray[i]->setBounds(0, slotHeight * (i + 1), getWidth(), componentHeight); //check
		}
	}
    //[/UserPreResize]

    c_iconDelSlots->setBounds (proportionOfWidth (0.8500f), proportionOfHeight (0.0104f), proportionOfWidth (0.0575f), proportionOfHeight (0.0296f));
    label->setBounds (proportionOfWidth (0.0187f), proportionOfHeight (0.0104f), proportionOfWidth (0.1200f), proportionOfHeight (0.0209f));
    label2->setBounds (proportionOfWidth (0.3787f), proportionOfHeight (0.0104f), proportionOfWidth (0.1200f), proportionOfHeight (0.0209f));
    label3->setBounds (proportionOfWidth (0.2387f), proportionOfHeight (0.0104f), proportionOfWidth (0.1200f), proportionOfHeight (0.0209f));
    label4->setBounds (proportionOfWidth (0.6000f), proportionOfHeight (0.0104f), proportionOfWidth (0.1200f), proportionOfHeight (0.0209f));
    label5->setBounds (proportionOfWidth (0.6725f), proportionOfHeight (0.0104f), proportionOfWidth (0.1200f), proportionOfHeight (0.0209f));
    c_iconMaximizeEditor->setBounds (getWidth() - 1 - proportionOfWidth (0.0150f), 1, proportionOfWidth (0.0150f), proportionOfHeight (0.0209f));
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void VASTMatrixComponent::updateAll() {
	for (auto i = 0; i < dragMatrixArray.size(); ++i)
	{
		dragMatrixArray[i]->updateContent(true);
	}
}

void VASTMatrixComponent::buttonClicked(Button* buttonThatWasClicked) {
	//ICONS
	if (buttonThatWasClicked == c_iconDelSlots.get()) {
		myProcessor->m_pVASTXperience.m_Set.modMatrixRemoveAll();
		myProcessor->requestUIUpdate();
	}

	if (buttonThatWasClicked == c_iconMaximizeEditor.get()) {
		if (!myEditor->vaporizerComponent->isTabConcertinaMaximized()) {
			myEditor->vaporizerComponent->maximizeTabConcertina();
		}
		else {
			myEditor->vaporizerComponent->defaultConcertina();
		}
		return;
	}
}

void VASTMatrixComponent::startAutoUpdate() {
    if (isShowing()) grabKeyboardFocus();
	for (auto i = 0; i < dragMatrixArray.size(); ++i)
	{
		dragMatrixArray[i]->startAutoUpdate();
	}
}
void VASTMatrixComponent::stopAutoUpdate() {
	for (auto i = 0; i < dragMatrixArray.size(); ++i)
	{
		dragMatrixArray[i]->stopAutoUpdate();
	}
}

void VASTMatrixComponent::lazyInit() {
	for (int i = 0; i < M_MODMATRIX_MAX_SLOTS; i++) {
		VASTDragMatrix* dragMatrix = new VASTDragMatrix(myEditor, myProcessor, i);
		addAndMakeVisible(dragMatrix);
		dragMatrixArray.add(dragMatrix);
	}

	c_iconDelSlots->addListener(this);
	c_iconMaximizeEditor->addListener(this);

	//manage parameter bindings -> set ComponentID = Name and processor
	Array<Component*> children = this->getChildren();
	for (auto child : children) {
		if (child->getName().startsWith("m_")) {
			auto* aSlider = dynamic_cast<VASTParameterSlider*> (child);
			if (aSlider != nullptr) {
				aSlider->setAudioProcessor(*myProcessor);
				aSlider->bindParameter(myEditor, aSlider->getName(), VASTGUIRuntimeModel::GUIComponents::MatrixComponent, 0);
			}
			auto* aCombobox = dynamic_cast<VASTParameterComboBox*> (child);
			if (aCombobox != nullptr) {
				aCombobox->setAudioProcessor(*myProcessor);
				aCombobox->bindParameter(myEditor, aCombobox->getName(), VASTGUIRuntimeModel::GUIComponents::MatrixComponent, 0);
			}
			auto* aButton = dynamic_cast<VASTParameterButton*> (child);
			if (aButton != nullptr) {
				aButton->setAudioProcessor(*myProcessor);
				aButton->bindParameter(myEditor, aButton->getName(), VASTGUIRuntimeModel::GUIComponents::MatrixComponent, 0);
			}
		}
	}
	m_lazyInit = true;
	resized();
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="VASTMatrixComponent" componentName=""
                 parentClasses="public Component, public Button::Listener" constructorParams="AudioProcessorEditor *editor, AudioProcessor* processor"
                 variableInitialisers="myEditor((VASTAudioProcessorEditor*)editor), myProcessor((VASTAudioProcessor*)processor)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="800" initialHeight="575">
  <BACKGROUND backgroundColour="ff323e44">
    <RECT pos="0 0 0M 0M" fill="linear: 10% 34.483%, 8% 19.478%, 0=bd1c341a, 1=4307200a"
          hasStroke="0"/>
  </BACKGROUND>
  <GENERICCOMPONENT name="c_iconDelSlots" id="1f6315974ecb9524" memberName="c_iconDelSlots"
                    virtualName="" explicitFocusOrder="0" pos="85% 1.043% 5.75% 2.957%"
                    class="VASTDrawableButton" params="&quot;c_iconDelSlots&quot;, delete_svg, delete_svgSize, &quot;Delete all matrix modulation slots&quot; "/>
  <LABEL name="new label" id="f496470fdd1e59df" memberName="label" virtualName=""
         explicitFocusOrder="0" pos="1.875% 1.043% 12% 2.087%" edTextCol="ff000000"
         edBkgCol="0" labelText="SOURCE" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="cb18b8ffc520fae0" memberName="label2" virtualName=""
         explicitFocusOrder="0" pos="37.875% 1.043% 12% 2.087%" edTextCol="ff000000"
         edBkgCol="0" labelText="DESTINATION" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="e5601054d203c1ed" memberName="label3" virtualName=""
         explicitFocusOrder="0" pos="23.875% 1.043% 12% 2.087%" edTextCol="ff000000"
         edBkgCol="0" labelText="POLARITY" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="43a1d987ac3fc2f6" memberName="label4" virtualName=""
         explicitFocusOrder="0" pos="60% 1.043% 12% 2.087%" edTextCol="ff000000"
         edBkgCol="0" labelText="VALUE" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="dabb1714eaeaaeb7" memberName="label5" virtualName=""
         explicitFocusOrder="0" pos="67.25% 1.043% 12% 2.087%" edTextCol="ff000000"
         edBkgCol="0" labelText="CURVE" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <GENERICCOMPONENT name="c_iconMaximizeEditor" id="ded548631676712b" memberName="c_iconMaximizeEditor"
                    virtualName="" explicitFocusOrder="0" pos="1Rr 1 1.5% 2.087%"
                    class="VASTDrawableButton" params="&quot;c_iconMaximize&quot;, arrow_right_corner_svg, arrow_right_corner_svgSize, &quot;Maximize area&quot;"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif

//==============================================================================
// Binary resources - be careful not to edit any of these sections!

// JUCER_RESOURCE: delete_svg, 1124, "../../Resources/Icons/delete.svg"
static const unsigned char resource_VASTMatrixComponent_delete_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,70,45,56,34,32,
115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,32,119,105,100,116,104,61,34,50,49,112,120,34,32,104,101,105,103,104,116,61,34,50,48,112,120,34,32,118,105,101,119,66,111,
120,61,34,48,32,48,32,50,49,32,50,48,34,32,118,101,114,115,105,111,110,61,34,49,46,49,34,32,120,109,108,110,115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,
118,103,34,32,120,109,108,110,115,58,120,108,105,110,107,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,120,108,105,110,107,34,62,10,32,32,32,32,60,33,45,45,32,71,
101,110,101,114,97,116,111,114,58,32,83,107,101,116,99,104,32,51,46,56,46,49,32,40,50,57,54,56,55,41,32,45,32,104,116,116,112,58,47,47,119,119,119,46,98,111,104,101,109,105,97,110,99,111,100,105,110,103,
46,99,111,109,47,115,107,101,116,99,104,32,45,45,62,10,32,32,32,32,60,116,105,116,108,101,62,100,101,108,101,116,101,32,91,35,49,52,56,55,93,60,47,116,105,116,108,101,62,10,32,32,32,32,60,100,101,115,
99,62,67,114,101,97,116,101,100,32,119,105,116,104,32,83,107,101,116,99,104,46,60,47,100,101,115,99,62,10,32,32,32,32,60,100,101,102,115,62,60,47,100,101,102,115,62,10,32,32,32,32,60,103,32,105,100,61,
34,80,97,103,101,45,49,34,32,115,116,114,111,107,101,61,34,110,111,110,101,34,32,115,116,114,111,107,101,45,119,105,100,116,104,61,34,49,34,32,102,105,108,108,61,34,110,111,110,101,34,32,102,105,108,108,
45,114,117,108,101,61,34,101,118,101,110,111,100,100,34,62,10,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,68,114,105,98,98,98,108,101,45,68,97,114,107,45,80,114,101,118,105,101,119,34,32,116,114,97,
110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,45,49,55,57,46,48,48,48,48,48,48,44,32,45,51,54,48,46,48,48,48,48,48,48,41,34,32,102,105,108,108,61,34,35,70,70,70,70,70,70,34,62,10,
32,32,32,32,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,105,99,111,110,115,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,53,54,46,48,48,48,48,48,48,44,32,49,54,
48,46,48,48,48,48,48,48,41,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,60,112,97,116,104,32,100,61,34,77,49,51,48,46,51,53,44,50,49,54,32,76,49,51,50,46,52,53,44,50,49,54,32,76,49,51,50,46,
52,53,44,50,48,56,32,76,49,51,48,46,51,53,44,50,48,56,32,76,49,51,48,46,51,53,44,50,49,54,32,90,32,77,49,51,52,46,53,53,44,50,49,54,32,76,49,51,54,46,54,53,44,50,49,54,32,76,49,51,54,46,54,53,44,50,48,
56,32,76,49,51,52,46,53,53,44,50,48,56,32,76,49,51,52,46,53,53,44,50,49,54,32,90,32,77,49,50,56,46,50,53,44,50,49,56,32,76,49,51,56,46,55,53,44,50,49,56,32,76,49,51,56,46,55,53,44,50,48,54,32,76,49,50,
56,46,50,53,44,50,48,54,32,76,49,50,56,46,50,53,44,50,49,56,32,90,32,77,49,51,48,46,51,53,44,50,48,52,32,76,49,51,54,46,54,53,44,50,48,52,32,76,49,51,54,46,54,53,44,50,48,50,32,76,49,51,48,46,51,53,44,
50,48,50,32,76,49,51,48,46,51,53,44,50,48,52,32,90,32,77,49,51,56,46,55,53,44,50,48,52,32,76,49,51,56,46,55,53,44,50,48,48,32,76,49,50,56,46,50,53,44,50,48,48,32,76,49,50,56,46,50,53,44,50,48,52,32,76,
49,50,51,44,50,48,52,32,76,49,50,51,44,50,48,54,32,76,49,50,54,46,49,53,44,50,48,54,32,76,49,50,54,46,49,53,44,50,50,48,32,76,49,52,48,46,56,53,44,50,50,48,32,76,49,52,48,46,56,53,44,50,48,54,32,76,49,
52,52,44,50,48,54,32,76,49,52,52,44,50,48,52,32,76,49,51,56,46,55,53,44,50,48,52,32,90,34,32,105,100,61,34,100,101,108,101,116,101,45,91,35,49,52,56,55,93,34,62,60,47,112,97,116,104,62,10,32,32,32,32,
32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,60,47,103,62,10,60,47,115,118,103,62,0,0};

const char* VASTMatrixComponent::delete_svg = (const char*) resource_VASTMatrixComponent_delete_svg;
const int VASTMatrixComponent::delete_svgSize = 1124;

// JUCER_RESOURCE: arrow_right_corner_svg, 1715, "../../Resources/Icons/arrow_right_corner.svg"
static const unsigned char resource_VASTMatrixComponent_arrow_right_corner_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,70,
45,56,34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,32,119,105,100,116,104,61,34,50,48,112,120,34,32,104,101,105,103,104,116,61,34,50,48,112,120,34,32,118,105,101,
119,66,111,120,61,34,48,32,48,32,50,48,32,50,48,34,32,118,101,114,115,105,111,110,61,34,49,46,49,34,32,120,109,108,110,115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,
48,48,47,115,118,103,34,32,120,109,108,110,115,58,120,108,105,110,107,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,120,108,105,110,107,34,62,10,32,32,32,32,60,
33,45,45,32,71,101,110,101,114,97,116,111,114,58,32,83,107,101,116,99,104,32,51,46,56,46,49,32,40,50,57,54,56,55,41,32,45,32,104,116,116,112,58,47,47,119,119,119,46,98,111,104,101,109,105,97,110,99,111,
100,105,110,103,46,99,111,109,47,115,107,101,116,99,104,32,45,45,62,10,32,32,32,32,60,116,105,116,108,101,62,97,114,114,111,119,95,114,105,103,104,116,95,99,111,114,110,101,114,32,91,35,50,55,57,93,60,
47,116,105,116,108,101,62,10,32,32,32,32,60,100,101,115,99,62,67,114,101,97,116,101,100,32,119,105,116,104,32,83,107,101,116,99,104,46,60,47,100,101,115,99,62,10,32,32,32,32,60,100,101,102,115,62,60,47,
100,101,102,115,62,10,32,32,32,32,60,103,32,105,100,61,34,80,97,103,101,45,49,34,32,115,116,114,111,107,101,61,34,110,111,110,101,34,32,115,116,114,111,107,101,45,119,105,100,116,104,61,34,49,34,32,102,
105,108,108,61,34,110,111,110,101,34,32,102,105,108,108,45,114,117,108,101,61,34,101,118,101,110,111,100,100,34,62,10,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,68,114,105,98,98,98,108,101,45,68,
97,114,107,45,80,114,101,118,105,101,119,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,45,49,56,48,46,48,48,48,48,48,48,44,32,45,54,57,49,57,46,48,48,48,48,48,48,
41,34,32,102,105,108,108,61,34,35,70,70,70,70,70,70,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,105,99,111,110,115,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,
115,108,97,116,101,40,53,54,46,48,48,48,48,48,48,44,32,49,54,48,46,48,48,48,48,48,48,41,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,60,112,97,116,104,32,100,61,34,77,49,51,50,44,54,55,54,
52,32,67,49,51,50,44,54,55,54,51,46,52,52,56,32,49,51,50,46,52,52,56,44,54,55,54,51,32,49,51,51,44,54,55,54,51,32,76,49,51,56,44,54,55,54,51,32,67,49,51,57,46,49,48,53,44,54,55,54,51,32,49,52,48,44,54,
55,54,51,46,56,57,53,32,49,52,48,44,54,55,54,53,32,76,49,52,48,44,54,55,55,48,32,67,49,52,48,44,54,55,55,48,46,53,53,50,32,49,51,57,46,53,53,50,44,54,55,55,49,32,49,51,57,44,54,55,55,49,32,67,49,51,56,
46,52,52,56,44,54,55,55,49,32,49,51,56,44,54,55,55,48,46,53,53,50,32,49,51,56,44,54,55,55,48,32,76,49,51,56,44,54,55,54,55,46,54,50,49,32,67,49,51,56,44,54,55,54,55,46,49,55,54,32,49,51,55,46,52,54,49,
44,54,55,54,54,46,57,53,51,32,49,51,55,46,49,52,54,44,54,55,54,55,46,50,54,56,32,76,49,51,52,46,49,50,49,44,54,55,55,48,46,50,57,51,32,67,49,51,51,46,55,51,49,44,54,55,55,48,46,54,56,51,32,49,51,51,46,
48,57,56,44,54,55,55,48,46,54,56,51,32,49,51,50,46,55,48,55,44,54,55,55,48,46,50,57,51,32,67,49,51,50,46,51,49,55,44,54,55,54,57,46,57,48,50,32,49,51,50,46,51,49,55,44,54,55,54,57,46,50,54,57,32,49,51,
50,46,55,48,55,44,54,55,54,56,46,56,55,57,32,76,49,51,53,46,55,51,50,44,54,55,54,53,46,56,53,52,32,67,49,51,54,46,48,52,55,44,54,55,54,53,46,53,51,57,32,49,51,53,46,56,50,52,44,54,55,54,53,32,49,51,53,
46,51,55,57,44,54,55,54,53,32,76,49,51,51,44,54,55,54,53,32,67,49,51,50,46,52,52,56,44,54,55,54,53,32,49,51,50,44,54,55,54,52,46,53,53,50,32,49,51,50,44,54,55,54,52,32,76,49,51,50,44,54,55,54,52,32,90,
32,77,49,50,54,44,54,55,54,50,32,67,49,50,54,44,54,55,54,49,46,52,52,56,32,49,50,54,46,52,52,56,44,54,55,54,49,32,49,50,55,44,54,55,54,49,32,76,49,52,49,44,54,55,54,49,32,67,49,52,49,46,53,53,50,44,54,
55,54,49,32,49,52,50,44,54,55,54,49,46,52,52,56,32,49,52,50,44,54,55,54,50,32,76,49,52,50,44,54,55,55,54,32,67,49,52,50,44,54,55,55,54,46,53,53,50,32,49,52,49,46,53,53,50,44,54,55,55,55,32,49,52,49,44,
54,55,55,55,32,76,49,51,51,44,54,55,55,55,32,67,49,51,50,46,52,52,56,44,54,55,55,55,32,49,51,50,44,54,55,55,54,46,53,53,50,32,49,51,50,44,54,55,55,54,32,76,49,51,50,44,54,55,55,51,32,67,49,51,50,44,54,
55,55,49,46,56,57,53,32,49,51,49,46,49,48,53,44,54,55,55,49,32,49,51,48,44,54,55,55,49,32,76,49,50,55,44,54,55,55,49,32,67,49,50,54,46,52,52,56,44,54,55,55,49,32,49,50,54,44,54,55,55,48,46,53,53,50,32,
49,50,54,44,54,55,55,48,32,76,49,50,54,44,54,55,54,50,32,90,32,77,49,52,52,44,54,55,54,49,32,67,49,52,52,44,54,55,53,57,46,56,57,53,32,49,52,51,46,49,48,53,44,54,55,53,57,32,49,52,50,44,54,55,53,57,32,
76,49,50,54,44,54,55,53,57,32,67,49,50,52,46,56,57,53,44,54,55,53,57,32,49,50,52,44,54,55,53,57,46,56,57,53,32,49,50,52,44,54,55,54,49,32,76,49,50,52,44,54,55,55,55,32,67,49,50,52,44,54,55,55,56,46,49,
48,53,32,49,50,52,46,56,57,53,44,54,55,55,57,32,49,50,54,44,54,55,55,57,32,76,49,52,50,44,54,55,55,57,32,67,49,52,51,46,49,48,53,44,54,55,55,57,32,49,52,52,44,54,55,55,56,46,49,48,53,32,49,52,52,44,54,
55,55,55,32,76,49,52,52,44,54,55,54,49,32,90,34,32,105,100,61,34,97,114,114,111,119,95,114,105,103,104,116,95,99,111,114,110,101,114,45,91,35,50,55,57,93,34,62,60,47,112,97,116,104,62,10,32,32,32,32,32,
32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,60,47,103,62,10,60,47,115,118,103,62,0,0};

const char* VASTMatrixComponent::arrow_right_corner_svg = (const char*) resource_VASTMatrixComponent_arrow_right_corner_svg;
const int VASTMatrixComponent::arrow_right_corner_svgSize = 1715;


//[EndFile] You can add extra defines here...
//[/EndFile]

