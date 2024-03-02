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
#include "../Engine/VASTEngineHeader.h"
#include "VASTAudioProcessor.h"
#include "VASTAudioProcessorEditor.h"
#include "../Engine/VASTSampler.h"
#include "VASTControls/VASTButton.h"
#include "VASTControls/VASTSlider.h"
#include "VASTScopeDisplay/VASTOscilloscope.h"
#include "VASTWTEditor/VASTSamplerViewport.h"
#include "../Engine/Oscillator/VASTWaveTable.h"
#include "VASTSamplerEditorComponentHeader.h"
#include "VASTWaveTableEditorComponent.h"
#include <thread>
//[/Headers]

#include "VASTSamplerEditorComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
VASTSamplerEditorComponent::VASTSamplerEditorComponent (AudioProcessorEditor *editor, AudioProcessor* processor, VASTWaveTableEditorComponent *wteditor )
    : myEditor((VASTAudioProcessorEditor*)editor), myProcessor((VASTAudioProcessor*)processor), myWTEditor(wteditor)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    c_viewportWAVLoop.reset (new VASTViewport ("c_viewportWAVLoop"));
    addAndMakeVisible (c_viewportWAVLoop.get());
    c_viewportWAVLoop->setScrollBarsShown (false, true);
    c_viewportWAVLoop->setScrollBarThickness (12);
    c_viewportWAVLoop->setViewedComponent (new VASTSamplerViewport());

    c_samplerHeaderComponent.reset (new VASTSamplerEditorComponentHeader (myEditor,
                                                                          myProcessor, myWTEditor));
    addAndMakeVisible (c_samplerHeaderComponent.get());
    c_samplerHeaderComponent->setName ("c_samplerHeaderComponent");


    //[UserPreSize]
	this->setComponentID("SamplerEditor");

	//manage parameter bindings -> set ComponentID = Name and processor
	Array<Component*> children = this->getChildren();
	for (auto child : children) {
		if (child->getName().startsWith("m_")) {
			auto* aSlider = dynamic_cast<VASTParameterSlider*> (child);
			if (aSlider != nullptr) {
				aSlider->setAudioProcessor(*myProcessor);
				aSlider->bindParameter(myEditor, aSlider->getName(), VASTGUIRuntimeModel::GUIComponents::SamplerEditorComponent, 0);
			}
			auto* aCombobox = dynamic_cast<VASTParameterComboBox*> (child);
			if (aCombobox != nullptr) {
				aCombobox->setAudioProcessor(*myProcessor);
				aCombobox->bindParameter(myEditor, aCombobox->getName(), VASTGUIRuntimeModel::GUIComponents::SamplerEditorComponent, 0);
			}
			auto* aButton = dynamic_cast<VASTParameterButton*> (child);
			if (aButton != nullptr) {
				aButton->setAudioProcessor(*myProcessor);
				aButton->bindParameter(myEditor, aButton->getName(), VASTGUIRuntimeModel::GUIComponents::SamplerEditorComponent, 0);
			}
		}
	}
	setOpaque(true);

    return; //dont call setSize
    //[/UserPreSize]

    setSize (800, 115);


    //[Constructor] You can add your own custom stuff here..
	//setLookAndFeel(myEditor->getCurrentVASTLookAndFeel());
	//myWTEditor->getEditorView()->getHeader()->getOscAButton()->setToggleState(true, juce::NotificationType::sendNotification);
    //[/Constructor]
}

VASTSamplerEditorComponent::~VASTSamplerEditorComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
	this->setLookAndFeel(nullptr);
    //[/Destructor_pre]

    c_viewportWAVLoop = nullptr;
    c_samplerHeaderComponent = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void VASTSamplerEditorComponent::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff323e44));

    {
        int x = 0, y = 0, width = getWidth() - 0, height = getHeight() - 0;
        juce::Colour fillColour1 = juce::Colour (0x4fa2a7b5), fillColour2 = juce::Colour (0xbd141618);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setGradientFill (juce::ColourGradient (fillColour1,
                                             static_cast<float> (proportionOfWidth (-0.0400f)) - 0.0f + x,
                                             static_cast<float> (proportionOfHeight (-0.0154f)) - 0.0f + y,
                                             fillColour2,
                                             static_cast<float> (proportionOfWidth (0.9467f)) - 0.0f + x,
                                             static_cast<float> (proportionOfHeight (1.0615f)) - 0.0f + y,
                                             false));
        g.fillRect (x, y, width, height);
    }

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void VASTSamplerEditorComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    c_viewportWAVLoop->setBounds (proportionOfWidth (0.0000f), proportionOfHeight (0.2000f), proportionOfWidth (1.0000f), proportionOfHeight (0.8000f));
    c_samplerHeaderComponent->setBounds (proportionOfWidth (0.0000f), proportionOfHeight (0.0000f), proportionOfWidth (1.0000f), proportionOfHeight (0.1739f));
    //[UserResized] Add your own custom resize handling here..

	//layout concertina
	getHeader()->setBounds(proportionOfWidth(0.0000f), proportionOfHeight(0.0000f), proportionOfWidth(1.0000f), myWTEditor->proportionOfHeight(0.034782f));  //normal header height: 20 / 575 = 0.034782f
	float editorRest = getHeight() - 0.034782f * myWTEditor->getHeight();
	if (editorRest < 0.f) editorRest = 0.f;
	c_viewportWAVLoop->setBounds(proportionOfWidth(0.0000f), myWTEditor->proportionOfHeight(0.034782f), proportionOfWidth(1.0000f), editorRest);
	c_viewportWAVLoop->getViewedComponent()->setBounds(proportionOfWidth(0.0000f), myWTEditor->proportionOfHeight(0.034782f), proportionOfWidth(1.0000f), editorRest);
	if (myWTEditor->m_samplerviewport != nullptr) myWTEditor->m_samplerviewport->setZoomFactor(c_samplerHeaderComponent->getWAVScale()->getValue());
	c_viewportWAVLoop->setScrollBarThickness(10.f * myProcessor->getPluginScaleHeightFactor());
    //[/UserResized]
}

void VASTSamplerEditorComponent::visibilityChanged()
{
    //[UserCode_visibilityChanged] -- Add your code here...

	//m_samplerviewport->setZoomFactor(m_iWAVScale.get()->getValue()); //TODO


    //[/UserCode_visibilityChanged]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="VASTSamplerEditorComponent"
                 componentName="" parentClasses="public Component" constructorParams="AudioProcessorEditor *editor, AudioProcessor* processor, VASTWaveTableEditorComponent *wteditor "
                 variableInitialisers="myEditor((VASTAudioProcessorEditor*)editor), myProcessor((VASTAudioProcessor*)processor), myWTEditor(wteditor)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="800" initialHeight="115">
  <METHODS>
    <METHOD name="visibilityChanged()"/>
  </METHODS>
  <BACKGROUND backgroundColour="ff323e44">
    <RECT pos="0 0 0M 0M" fill="linear: -4% -1.538%, 94.667% 106.154%, 0=4fa2a7b5, 1=bd141618"
          hasStroke="0"/>
  </BACKGROUND>
  <VIEWPORT name="c_viewportWAVLoop" id="cbd8920ab4da86af" memberName="c_viewportWAVLoop"
            virtualName="VASTViewport" explicitFocusOrder="0" pos="0% 20% 100% 80%"
            vscroll="0" hscroll="1" scrollbarThickness="12" contentType="2"
            jucerFile="" contentClass="VASTSamplerViewport" constructorParams=""/>
  <GENERICCOMPONENT name="c_samplerHeaderComponent" id="c19225047fcd02bd" memberName="c_samplerHeaderComponent"
                    virtualName="" explicitFocusOrder="0" pos="0% 0% 100% 17.391%"
                    class="VASTSamplerEditorComponentHeader" params="myEditor,&#10;myProcessor, myWTEditor"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

