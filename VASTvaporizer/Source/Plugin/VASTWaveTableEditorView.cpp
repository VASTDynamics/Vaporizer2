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
#include "VASTWTEditor/VASTFreqDomainViewport.h"
#include "VASTWTEditor/VASTPositionViewport.h"
#include "../Engine/Oscillator/VASTWaveTable.h"
#include "VASTSamplerEditorComponentHeader.h"
#include <thread>
//[/Headers]

#include "VASTWaveTableEditorView.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
VASTWaveTableEditorView::VASTWaveTableEditorView (AudioProcessorEditor *editor, AudioProcessor* processor, VASTWaveTableEditorComponent *wteditor )
    : myEditor((VASTAudioProcessorEditor*)editor), myProcessor((VASTAudioProcessor*)processor), myWTEditor(wteditor)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    c_editorOscilloscope.reset (new VASTOscilloscope());
    addAndMakeVisible (c_editorOscilloscope.get());
    c_editorOscilloscope->setName ("c_editorOscilloscope");

    c_viewportPositions.reset (new VASTViewport ("c_viewportPositions"));
    addAndMakeVisible (c_viewportPositions.get());
    c_viewportPositions->setExplicitFocusOrder (1);
    c_viewportPositions->setScrollBarsShown (false, true);
    c_viewportPositions->setScrollBarThickness (12);
    c_viewportPositions->setViewedComponent (new VASTPositionViewport());

    c_viewportFreqDomain.reset (new VASTViewport ("c_viewportFreqDomain"));
    addAndMakeVisible (c_viewportFreqDomain.get());
    c_viewportFreqDomain->setScrollBarsShown (false, true);
    c_viewportFreqDomain->setScrollBarThickness (12);
    c_viewportFreqDomain->setViewedComponent (new VASTFreqDomainViewport());

    c_editorHeaderComponent.reset (new VASTWaveTableEditorViewHeader (myEditor,
                                                                      myProcessor, myWTEditor));
    addAndMakeVisible (c_editorHeaderComponent.get());
    c_editorHeaderComponent->setName ("c_editorHeaderComponent");


    //[UserPreSize]
	this->setComponentID("WTEditorView");
	c_editorOscilloscope->setComponentID("WTEditor");
	c_editorOscilloscope->setProcessor(myProcessor);
	c_editorOscilloscope->setEditor(myEditor);
	c_editorOscilloscope->setWTEditor(myWTEditor);

	//CHECK
	myWTEditor->m_freqviewport = (VASTFreqDomainViewport*)c_viewportFreqDomain->getViewedComponent();
	myWTEditor->m_freqviewport->setProcessor(myProcessor);
	myWTEditor->m_freqviewport->setEditor(myEditor);
	myWTEditor->m_freqviewport->setWTEditor(myWTEditor);

	myWTEditor->m_positionviewport = (VASTPositionViewport*)c_viewportPositions->getViewedComponent();
	myWTEditor->m_positionviewport->setProcessor(myProcessor);
	myWTEditor->m_positionviewport->setEditor(myEditor);
	myWTEditor->m_positionviewport->setWTEditor(myWTEditor);

	c_viewportPositions->setScrollOnDragMode(Viewport::ScrollOnDragMode::never);

	myWTEditor->m_samplerviewport = (VASTSamplerViewport*)myWTEditor->getSamplerEditor()->getSamplerViewport()->getViewedComponent();
	myWTEditor->m_samplerviewport->setProcessor(myProcessor);
	myWTEditor->m_samplerviewport->setEditor(myEditor);
	myWTEditor->m_samplerviewport->setWTEditor(myWTEditor);

	//myWTEditor->m_freqviewport->setZoomFactor(m_iWTEditorZoom.get()->getValue());
	//myWTEditor->m_positionviewport->setZoomFactor(m_iWTEditorZoom.get()->getValue());
	//myWTEditor->m_samplerviewport->setZoomFactor(c_samplerEditorComponent->getHeader()->getWAVScale()->getValue());

	//manage parameter bindings -> set ComponentID = Name and processor
	Array<Component*> children = this->getChildren();
	for (auto child : children) {
		if (child->getName().startsWith("m_")) {
			auto* aSlider = dynamic_cast<VASTParameterSlider*> (child);
			if (aSlider != nullptr) {
				aSlider->setAudioProcessor(*myProcessor);
				aSlider->bindParameter(myEditor, aSlider->getName(), VASTGUIRuntimeModel::GUIComponents::WavetableEditorView, 0);
			}
			auto* aCombobox = dynamic_cast<VASTParameterComboBox*> (child);
			if (aCombobox != nullptr) {
				aCombobox->setAudioProcessor(*myProcessor);
				aCombobox->bindParameter(myEditor, aCombobox->getName(), VASTGUIRuntimeModel::GUIComponents::WavetableEditorView, 0);
			}
			auto* aButton = dynamic_cast<VASTParameterButton*> (child);
			if (aButton != nullptr) {
				aButton->setAudioProcessor(*myProcessor);
				aButton->bindParameter(myEditor, aButton->getName(), VASTGUIRuntimeModel::GUIComponents::WavetableEditorView, 0);
			}
		}
	}

	setOpaque(true);
    //[/UserPreSize]

    setSize (800, 460);


    //[Constructor] You can add your own custom stuff here..
	//setLookAndFeel(myEditor->getCurrentVASTLookAndFeel());
    //[/Constructor]
}

VASTWaveTableEditorView::~VASTWaveTableEditorView()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
	this->setLookAndFeel(nullptr);
    //[/Destructor_pre]

    c_editorOscilloscope = nullptr;
    c_viewportPositions = nullptr;
    c_viewportFreqDomain = nullptr;
    c_editorHeaderComponent = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void VASTWaveTableEditorView::paint (juce::Graphics& g)
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

void VASTWaveTableEditorView::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    c_editorOscilloscope->setBounds (proportionOfWidth (0.0000f), proportionOfHeight (0.5391f), proportionOfWidth (1.0000f), proportionOfHeight (0.4652f));
    c_viewportPositions->setBounds (proportionOfWidth (0.0000f), proportionOfHeight (0.2196f), proportionOfWidth (1.0000f), proportionOfHeight (0.0826f));
    c_viewportFreqDomain->setBounds (proportionOfWidth (0.0000f), proportionOfHeight (0.2957f), proportionOfWidth (1.0000f), proportionOfHeight (0.2500f));
    c_editorHeaderComponent->setBounds (proportionOfWidth (0.0000f), proportionOfHeight (0.0000f), proportionOfWidth (1.0000f), proportionOfHeight (0.2087f));
    //[UserResized] Add your own custom resize handling here..

	//layout concertina
	getHeader()->setBounds(proportionOfWidth(0.0000f), proportionOfHeight(0.0000f), proportionOfWidth(1.0000f), myWTEditor->proportionOfHeight(0.16696f));  //normal header height: 96 / 575 = 0.16696f
	float editorRest = getHeight() - 0.16696f * myWTEditor->getHeight();
	if (editorRest < 0.f) editorRest = 0.f;
	getViewportPositions()->setBounds(proportionOfWidth(0.0000f), myWTEditor->proportionOfHeight(0.16696f), proportionOfWidth(1.0000f), 0.12f * editorRest);
	getViewportFreqDomain()->setBounds(proportionOfWidth(0.0000f), myWTEditor->proportionOfHeight(0.16696f) + 0.12f * editorRest, proportionOfWidth(1.0000f), 0.23f * editorRest);
	getEditorOscilloscope()->setBounds(proportionOfWidth(0.0000f), myWTEditor->proportionOfHeight(0.16696f) + (0.12f + 0.23f) * editorRest, proportionOfWidth(1.0000f), 0.65f * editorRest);


	//myWTEditor->m_freqviewport->setZoomFactor(getHeader()->getWTEditorZoom()->getValue());
	//myWTEditor->m_positionviewport->setZoomFactor(getHeader()->getWTEditorZoom()->getValue());
	getHeader()->getWTEditorZoom()->setTextBoxStyle(Slider::TextBoxLeft, true, getHeader()->getWTEditorZoom()->getWidth() * 0.45f, getHeader()->getWTEditorZoom()->getHeight());

	c_viewportPositions->setScrollBarThickness(10.f * myProcessor->getPluginScaleHeightFactor());
	c_viewportFreqDomain->setScrollBarThickness(10.f * myProcessor->getPluginScaleHeightFactor());

    //[/UserResized]
}

void VASTWaveTableEditorView::visibilityChanged()
{
    //[UserCode_visibilityChanged] -- Add your code here...
	if (this->isVisible()) { //skip on close
		myWTEditor->m_freqviewport->setZoomFactor(getHeader()->getWTEditorZoom()->getValue());
		myWTEditor->m_positionviewport->setZoomFactor(getHeader()->getWTEditorZoom()->getValue());
		myWTEditor->m_samplerviewport->setZoomFactor(myWTEditor->getSamplerEditor()->getHeader()->getWAVScale()->getValue());
		c_editorOscilloscope->selectAll(true); //UI Update later
		myWTEditor->notifySelectionChanged();
		myWTEditor->updateAll(false);
	}
    //[/UserCode_visibilityChanged]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
/*
void VASTWaveTableEditorView::lookAndFeelChanged() {
	c_editorOscilloscope->setLookAndFeel(myEditor->getCurrentVASTLookAndFeel());
	c_viewportPositions->setLookAndFeel(myEditor->getCurrentVASTLookAndFeel());
	c_viewportFreqDomain->setLookAndFeel(myEditor->getCurrentVASTLookAndFeel());
	c_editorHeaderComponent->setLookAndFeel(myEditor->getCurrentVASTLookAndFeel());
}
*/
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="VASTWaveTableEditorView"
                 componentName="" parentClasses="public Component" constructorParams="AudioProcessorEditor *editor, AudioProcessor* processor, VASTWaveTableEditorComponent *wteditor "
                 variableInitialisers="myEditor((VASTAudioProcessorEditor*)editor), myProcessor((VASTAudioProcessor*)processor), myWTEditor(wteditor)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="800" initialHeight="460">
  <METHODS>
    <METHOD name="visibilityChanged()"/>
  </METHODS>
  <BACKGROUND backgroundColour="ff323e44">
    <RECT pos="0 0 0M 0M" fill="linear: -4% -1.538%, 94.667% 106.154%, 0=4fa2a7b5, 1=bd141618"
          hasStroke="0"/>
  </BACKGROUND>
  <GENERICCOMPONENT name="c_editorOscilloscope" id="b0678625d42e6d2" memberName="c_editorOscilloscope"
                    virtualName="" explicitFocusOrder="0" pos="0% 53.913% 100% 46.522%"
                    class="VASTOscilloscope" params=""/>
  <VIEWPORT name="c_viewportPositions" id="41b8b33a9be4de1" memberName="c_viewportPositions"
            virtualName="VASTViewport" explicitFocusOrder="1" pos="0% 21.957% 100% 8.261%"
            vscroll="0" hscroll="1" scrollbarThickness="12" contentType="2"
            jucerFile="" contentClass="VASTPositionViewport" constructorParams=""/>
  <VIEWPORT name="c_viewportFreqDomain" id="fef12dd078b8780f" memberName="c_viewportFreqDomain"
            virtualName="VASTViewport" explicitFocusOrder="0" pos="0% 29.565% 100% 25%"
            vscroll="0" hscroll="1" scrollbarThickness="12" contentType="2"
            jucerFile="" contentClass="VASTFreqDomainViewport" constructorParams=""/>
  <GENERICCOMPONENT name="c_editorHeaderComponent" id="c19225047fcd02bd" memberName="c_editorHeaderComponent"
                    virtualName="" explicitFocusOrder="0" pos="0% 0% 100% 20.87%"
                    class="VASTWaveTableEditorViewHeader" params="myEditor,&#10;myProcessor, myWTEditor"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

