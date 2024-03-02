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
#include "../../Engine/VASTEngineHeader.h"
#include "../VASTAudioProcessorEditor.h"
#include "../VASTFX/VASTGenericSelector.h"
#include "../VASTUtils/VASTGUINotifier.h"
//[/Headers]

#include "VASTFXSelectionPane.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
VASTFXSelectionPane::VASTFXSelectionPane (AudioProcessorEditor *editor, AudioProcessor* processor, int busnr, VASTGUINotifier* guinotifier)
    : myEditor((VASTAudioProcessorEditor*)editor), myProcessor((VASTAudioProcessor*)processor), myBusnr(busnr), myGUINotifier(guinotifier)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    label125.reset (new juce::Label ("new label",
                                     TRANS ("MONO CUT")));
    addAndMakeVisible (label125.get());
    label125->setFont (juce::Font ("Syntax", 11.00f, juce::Font::plain));
    label125->setJustificationType (juce::Justification::centred);
    label125->setEditable (false, false, false);
    label125->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label125->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label125->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    m_fMBMonoFrequency.reset (new VASTParameterSlider ("m_fMBMonoFrequency"));
    addAndMakeVisible (m_fMBMonoFrequency.get());
    m_fMBMonoFrequency->setTooltip (TRANS ("Multi band frequency - below only mono processing and no delay & reverb"));
    m_fMBMonoFrequency->setRange (0, 20000, 0.01);
    m_fMBMonoFrequency->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    m_fMBMonoFrequency->setTextBoxStyle (juce::Slider::NoTextBox, false, 30, 14);
    m_fMBMonoFrequency->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffc9c9c9));
    m_fMBMonoFrequency->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0x00ffffff));
    m_fMBMonoFrequency->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x00808080));
    m_fMBMonoFrequency->addListener (this);

    c_selector.reset (new VASTGenericSelector (myEditor, myProcessor, myBusnr, myGUINotifier));
    addAndMakeVisible (c_selector.get());
    c_selector->setName ("c_selector");

    m_uFxBusRouting.reset (new VASTParameterComboBox ("m_uFxBusRouting"));
    addAndMakeVisible (m_uFxBusRouting.get());
    m_uFxBusRouting->setTooltip (TRANS ("Effect bus routing"));
    m_uFxBusRouting->setEditableText (false);
    m_uFxBusRouting->setJustificationType (juce::Justification::centredLeft);
    m_uFxBusRouting->setTextWhenNothingSelected (juce::String());
    m_uFxBusRouting->setTextWhenNoChoicesAvailable (TRANS ("---"));
    m_uFxBusRouting->addItem (TRANS ("MSEG1"), 1);
    m_uFxBusRouting->addItem (TRANS ("MSEG2"), 2);
    m_uFxBusRouting->addItem (TRANS ("MSEG3"), 3);
    m_uFxBusRouting->addItem (TRANS ("MSEG4"), 4);
    m_uFxBusRouting->addItem (TRANS ("MSEG5"), 5);
    m_uFxBusRouting->addListener (this);

    label14.reset (new juce::Label ("new label",
                                    TRANS ("ROUTING")));
    addAndMakeVisible (label14.get());
    label14->setFont (juce::Font ("Syntax", 11.00f, juce::Font::plain));
    label14->setJustificationType (juce::Justification::centred);
    label14->setEditable (false, false, false);
    label14->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label14->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label14->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));


    //[UserPreSize]
	Array<Component*> children = this->getChildren();
	for (auto child : children) {
		if (child->getName().startsWith("m_")) {
			auto* aSlider = dynamic_cast<VASTParameterSlider*> (child);
			if (aSlider != nullptr) {
				aSlider->setAudioProcessor(*myProcessor);
				if (myBusnr>0)
					aSlider->bindParameter(myEditor, aSlider->getName() + "_Bus" + String(myBusnr + 1), VASTGUIRuntimeModel::GUIComponents::FXSelectionPane, myBusnr);
				else
					aSlider->bindParameter(myEditor, aSlider->getName(), VASTGUIRuntimeModel::GUIComponents::FXSelectionPane, myBusnr);
			}
			auto* aCombobox = dynamic_cast<VASTParameterComboBox*> (child);
			if (aCombobox != nullptr) {
				aCombobox->setAudioProcessor(*myProcessor);
				if (myBusnr>0)
					aCombobox->bindParameter(myEditor, aCombobox->getName() + "_Bus" + String(myBusnr + 1), VASTGUIRuntimeModel::GUIComponents::FXSelectionPane, myBusnr);
				else
					aCombobox->bindParameter(myEditor, aCombobox->getName(), VASTGUIRuntimeModel::GUIComponents::FXSelectionPane, myBusnr);
			}
			auto* aButton = dynamic_cast<VASTParameterButton*> (child);
			if (aButton != nullptr) {
				aButton->setAudioProcessor(*myProcessor);
				if (myBusnr>0)
					aButton->bindParameter(myEditor, aButton->getName() + "_Bus" + String(myBusnr + 1), VASTGUIRuntimeModel::GUIComponents::FXSelectionPane, myBusnr);
				else
					aButton->bindParameter(myEditor, aButton->getName(), VASTGUIRuntimeModel::GUIComponents::FXSelectionPane, myBusnr);

			}
		}
	}

	setOpaque(true);

    //[/UserPreSize]

    setSize (198, 600);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

VASTFXSelectionPane::~VASTFXSelectionPane()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
	this->setLookAndFeel(nullptr);
    //[/Destructor_pre]

    label125 = nullptr;
    m_fMBMonoFrequency = nullptr;
    c_selector = nullptr;
    m_uFxBusRouting = nullptr;
    label14 = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void VASTFXSelectionPane::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    g.fillAll (myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colFXSelectionPaneBackground));
    //[/UserPaint]
}

void VASTFXSelectionPane::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    label125->setBounds (proportionOfWidth (0.0252f), proportionOfHeight (0.0800f), proportionOfWidth (0.3939f), proportionOfHeight (0.0200f));
    m_fMBMonoFrequency->setBounds (proportionOfWidth (0.1263f), proportionOfHeight (0.0133f), proportionOfWidth (0.2020f), proportionOfHeight (0.0667f));
    c_selector->setBounds (proportionOfWidth (0.0303f), proportionOfHeight (0.1333f), proportionOfWidth (0.9394f), proportionOfHeight (0.8667f));
    m_uFxBusRouting->setBounds (proportionOfWidth (0.5050f), proportionOfHeight (0.0433f), proportionOfWidth (0.3636f), proportionOfHeight (0.0233f));
    label14->setBounds (proportionOfWidth (0.4899f), proportionOfHeight (0.0800f), proportionOfWidth (0.3939f), proportionOfHeight (0.0200f));
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void VASTFXSelectionPane::sliderValueChanged (juce::Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == m_fMBMonoFrequency.get())
    {
        //[UserSliderCode_m_fMBMonoFrequency] -- add your slider handling code here..
        //[/UserSliderCode_m_fMBMonoFrequency]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void VASTFXSelectionPane::comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == m_uFxBusRouting.get())
    {
        //[UserComboBoxCode_m_uFxBusRouting] -- add your combo box handling code here..
        //[/UserComboBoxCode_m_uFxBusRouting]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void VASTFXSelectionPane::updateContent() {
	c_selector->updateContent();
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="VASTFXSelectionPane" componentName=""
                 parentClasses="public Component" constructorParams="AudioProcessorEditor *editor, AudioProcessor* processor, int busnr, VASTGUINotifier* guinotifier"
                 variableInitialisers="myEditor((VASTAudioProcessorEditor*)editor), myProcessor((VASTAudioProcessor*)processor), myBusnr(busnr), myGUINotifier(guinotifier)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="198" initialHeight="600">
  <BACKGROUND backgroundColour="ff323e44"/>
  <LABEL name="new label" id="d62fb1998f23de00" memberName="label125"
         virtualName="" explicitFocusOrder="0" pos="2.525% 8% 39.394% 2%"
         textCol="ffe2e2e2" edTextCol="ff000000" edBkgCol="0" labelText="MONO CUT"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Syntax" fontsize="11.0" kerning="0.0" bold="0" italic="0"
         justification="36"/>
  <SLIDER name="m_fMBMonoFrequency" id="ade2773e0ef16710" memberName="m_fMBMonoFrequency"
          virtualName="VASTParameterSlider" explicitFocusOrder="0" pos="12.626% 1.333% 20.202% 6.667%"
          tooltip="Multi band frequency - below only mono processing and no delay &amp; reverb"
          textboxtext="ffc9c9c9" textboxbkgd="ffffff" textboxoutline="808080"
          min="0.0" max="20000.0" int="0.01" style="RotaryVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="30"
          textBoxHeight="14" skewFactor="1.0" needsCallback="1"/>
  <GENERICCOMPONENT name="c_selector" id="d6dae28bccd4bfc2" memberName="c_selector"
                    virtualName="" explicitFocusOrder="0" pos="3.03% 13.333% 93.939% 86.667%"
                    class="VASTGenericSelector" params="myEditor, myProcessor, myBusnr, myGUINotifier"/>
  <COMBOBOX name="m_uFxBusRouting" id="c4f75fd447cf8661" memberName="m_uFxBusRouting"
            virtualName="VASTParameterComboBox" explicitFocusOrder="0" pos="50.505% 4.333% 36.364% 2.333%"
            tooltip="Effect bus routing" editable="0" layout="33" items="MSEG1&#10;MSEG2&#10;MSEG3&#10;MSEG4&#10;MSEG5"
            textWhenNonSelected="" textWhenNoItems="---"/>
  <LABEL name="new label" id="4cdfea23a10875e0" memberName="label14" virtualName=""
         explicitFocusOrder="0" pos="48.99% 8% 39.394% 2%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="ROUTING" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Syntax"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="36"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

