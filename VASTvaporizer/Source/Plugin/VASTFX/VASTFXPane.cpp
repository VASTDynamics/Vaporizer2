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
#include "VASTFXSelectionPane.h"
#include "VASTFXEffectPane.h"
//[/Headers]

#include "VASTFXPane.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
VASTFXPane::VASTFXPane (AudioProcessorEditor *editor, AudioProcessor* processor, int busnr)
    : myEditor((VASTAudioProcessorEditor*)editor), myProcessor((VASTAudioProcessor*)processor),myBusnr(busnr)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    c_fxselect.reset (new juce::Viewport ("c_fxselect"));
    addAndMakeVisible (c_fxselect.get());
    c_fxselect->setScrollBarsShown (true, false);
    c_fxselect->setViewedComponent (new VASTFXSelectionPane (myEditor, myProcessor, myBusnr, this));

    c_fxviewport.reset (new juce::Viewport ("c_fxviewport"));
    addAndMakeVisible (c_fxviewport.get());
    c_fxviewport->setViewedComponent (new VASTFXEffectPane (myEditor, myProcessor, myBusnr));


    //[UserPreSize]
	c_fxviewport->setScrollOnDragMode(Viewport::ScrollOnDragMode::never); //not good when on
	setOpaque(true);
    //[/UserPreSize]

    setSize (858, 600);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

VASTFXPane::~VASTFXPane()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
	this->setLookAndFeel(nullptr);
    //[/Destructor_pre]

    c_fxselect = nullptr;
    c_fxviewport = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void VASTFXPane::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff323e44));

    {
        int x = proportionOfWidth (0.2401f), y = proportionOfHeight (0.0000f), width = proportionOfWidth (0.7483f), height = proportionOfHeight (1.0000f);
        juce::Colour fillColour = juce::Colours::black;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.fillRect (x, y, width, height);
    }

    {
        int x = proportionOfWidth (0.0000f), y = proportionOfHeight (0.0000f), width = proportionOfWidth (0.2400f), height = proportionOfHeight (1.0000f);
        juce::Colour fillColour = juce::Colours::black;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.fillRect (x, y, width, height);
    }

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void VASTFXPane::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    c_fxselect->setBounds (proportionOfWidth (0.0000f), proportionOfHeight (0.0000f), proportionOfWidth (0.2401f), proportionOfHeight (1.0000f));
    c_fxviewport->setBounds (proportionOfWidth (0.2401f), proportionOfHeight (0.0000f), proportionOfWidth (0.7483f), proportionOfHeight (1.0000f));
    //[UserResized] Add your own custom resize handling here..

	c_fxselect->getViewedComponent()->setBounds(c_fxselect->getLocalBounds().getX(), c_fxselect->getLocalBounds().getY(), c_fxselect->getLocalBounds().getWidth(), c_fxselect->getLocalBounds().getHeight());
	c_fxviewport->getViewedComponent()->setBounds(c_fxviewport->getLocalBounds().getX(), c_fxviewport->getLocalBounds().getY(), c_fxviewport->getLocalBounds().getWidth() * 1.5f, c_fxviewport->getLocalBounds().getHeight() * 3);

	c_fxviewport->setScrollBarThickness(10.f * myProcessor->getPluginScaleHeightFactor());

    //[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void VASTFXPane::shallUpdateGUI(const String compID, bool toggleState) {
	VASTFXEffectPane* effectspane = (VASTFXEffectPane*)c_fxviewport->getViewedComponent();
	if (effectspane!=nullptr)
		effectspane->updateGUI(compID, toggleState);
}

void VASTFXPane::updateAll() {
	VASTFXSelectionPane* selectionpane = (VASTFXSelectionPane*)c_fxselect->getViewedComponent();
	selectionpane->updateContent();

	VASTFXEffectPane* effectspane = (VASTFXEffectPane*)c_fxviewport->getViewedComponent();
	if (effectspane != nullptr) {
		effectspane->updateGUI("", false);
	}
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="VASTFXPane" componentName=""
                 parentClasses="public Component, public VASTGUINotifier" constructorParams="AudioProcessorEditor *editor, AudioProcessor* processor, int busnr"
                 variableInitialisers="myEditor((VASTAudioProcessorEditor*)editor), myProcessor((VASTAudioProcessor*)processor),myBusnr(busnr)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="858" initialHeight="600">
  <BACKGROUND backgroundColour="ff323e44">
    <RECT pos="24.009% 0% 74.825% 100%" fill="solid: ff000000" hasStroke="0"/>
    <RECT pos="0% 0% 24% 100%" fill="solid: ff000000" hasStroke="0"/>
  </BACKGROUND>
  <VIEWPORT name="c_fxselect" id="7e8ef267e8098aec" memberName="c_fxselect"
            virtualName="" explicitFocusOrder="0" pos="0% 0% 24.009% 100%"
            vscroll="1" hscroll="0" scrollbarThickness="8" contentType="2"
            jucerFile="" contentClass="VASTFXSelectionPane" constructorParams="myEditor, myProcessor, myBusnr, this"/>
  <VIEWPORT name="c_fxviewport" id="f47f77181e666697" memberName="c_fxviewport"
            virtualName="" explicitFocusOrder="0" pos="24.009% 0% 74.825% 100%"
            vscroll="1" hscroll="1" scrollbarThickness="8" contentType="2"
            jucerFile="" contentClass="VASTFXEffectPane" constructorParams="myEditor, myProcessor, myBusnr"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

