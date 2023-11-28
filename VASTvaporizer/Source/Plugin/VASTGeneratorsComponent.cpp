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
#include "VASTSamplerNoiseComponent.h"
#include "VASTAudioProcessor.h"
#include "VASTAudioProcessorEditor.h"
//[/Headers]

#include "VASTGeneratorsComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
VASTGeneratorsComponent::VASTGeneratorsComponent (AudioProcessorEditor *editor, AudioProcessor* processor)
    : myEditor((VASTAudioProcessorEditor*)editor), myProcessor((VASTAudioProcessor*)processor)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    oscillatorAComponent.reset (new VASTOscillatorComponent (myEditor, myProcessor));
    addAndMakeVisible (oscillatorAComponent.get());
    oscillatorAComponent->setName ("oscillatorAComponent");

    oscillatorBComponent.reset (new VASTOscillatorComponent (myEditor, myProcessor));
    addAndMakeVisible (oscillatorBComponent.get());
    oscillatorBComponent->setName ("oscillatorBComponent");

    oscillatorCComponent.reset (new VASTOscillatorComponent (myEditor, myProcessor));
    addAndMakeVisible (oscillatorCComponent.get());
    oscillatorCComponent->setName ("oscillatorCComponent");

    oscillatorDComponent.reset (new VASTOscillatorComponent (myEditor, myProcessor));
    addAndMakeVisible (oscillatorDComponent.get());
    oscillatorDComponent->setName ("oscillatorDComponent");

    samplerNoiseComponent.reset (new VASTSamplerNoiseComponent (myEditor, myProcessor));
    addAndMakeVisible (samplerNoiseComponent.get());
    samplerNoiseComponent->setName ("samplerNoiseComponent");


    //[UserPreSize]
	oscillatorAComponent->assignComponentIDsForChildren(String("OscA"));
	oscillatorBComponent->assignComponentIDsForChildren(String("OscB"));
	oscillatorCComponent->assignComponentIDsForChildren(String("OscC"));
	oscillatorDComponent->assignComponentIDsForChildren(String("OscD"));

	setOpaque(true);
    return; //dont call setSize
    //[/UserPreSize]

    setSize (536, 641);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

VASTGeneratorsComponent::~VASTGeneratorsComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    oscillatorAComponent = nullptr;
    oscillatorBComponent = nullptr;
    oscillatorCComponent = nullptr;
    oscillatorDComponent = nullptr;
    samplerNoiseComponent = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void VASTGeneratorsComponent::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    /*
    if (myEditor->getCurrentVASTLookAndFeel()->m_isUsingBackgroundImages) {
        g.setColour(juce::Colours::black);
        int x = -22, y = -114, width = proportionOfWidth(1.2948f), height = proportionOfHeight(1.2512f);
        g.drawImage(cachedImage_vaporizer2background_dark_png_1,
            x, y, width, height,
            0, 0, cachedImage_vaporizer2background_dark_png_1.getWidth(), cachedImage_vaporizer2background_dark_png_1.getHeight());
        return;
    }
    */

    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void VASTGeneratorsComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    oscillatorAComponent->setBounds (proportionOfWidth (0.0000f), proportionOfHeight (0.0000f), proportionOfWidth (1.0000f), proportionOfHeight (0.1981f));
    oscillatorBComponent->setBounds (proportionOfWidth (0.0000f), proportionOfHeight (0.1997f), proportionOfWidth (1.0000f), proportionOfHeight (0.1981f));
    oscillatorCComponent->setBounds (proportionOfWidth (0.0000f), proportionOfHeight (0.3994f), proportionOfWidth (1.0000f), proportionOfHeight (0.1981f));
    oscillatorDComponent->setBounds (proportionOfWidth (0.0000f), proportionOfHeight (0.5991f), proportionOfWidth (1.0000f), proportionOfHeight (0.1981f));
    samplerNoiseComponent->setBounds (proportionOfWidth (0.0000f), proportionOfHeight (0.7987f), proportionOfWidth (1.0000f), proportionOfHeight (0.2028f));
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="VASTGeneratorsComponent"
                 componentName="" parentClasses="public Component" constructorParams="AudioProcessorEditor *editor, AudioProcessor* processor"
                 variableInitialisers="myEditor((VASTAudioProcessorEditor*)editor), myProcessor((VASTAudioProcessor*)processor)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="536" initialHeight="641">
  <BACKGROUND backgroundColour="ff323e44"/>
  <GENERICCOMPONENT name="oscillatorAComponent" id="c246caaae79b177c" memberName="oscillatorAComponent"
                    virtualName="VASTOscillatorComponent" explicitFocusOrder="0"
                    pos="0% 0% 100% 19.813%" class="VASTOscillatorComponent" params="myEditor, myProcessor"/>
  <GENERICCOMPONENT name="oscillatorBComponent" id="c15ec5e3319a8d92" memberName="oscillatorBComponent"
                    virtualName="VASTOscillatorComponent" explicitFocusOrder="0"
                    pos="0% 19.969% 100% 19.813%" class="VASTOscillatorComponent"
                    params="myEditor, myProcessor"/>
  <GENERICCOMPONENT name="oscillatorCComponent" id="1def2f77ed4c48ee" memberName="oscillatorCComponent"
                    virtualName="VASTOscillatorComponent" explicitFocusOrder="0"
                    pos="0% 39.938% 100% 19.813%" class="VASTOscillatorComponent"
                    params="myEditor, myProcessor"/>
  <GENERICCOMPONENT name="oscillatorDComponent" id="6eb99f02097120e3" memberName="oscillatorDComponent"
                    virtualName="VASTOscillatorComponent" explicitFocusOrder="0"
                    pos="0% 59.906% 100% 19.813%" class="VASTOscillatorComponent"
                    params="myEditor, myProcessor"/>
  <GENERICCOMPONENT name="samplerNoiseComponent" id="556192163ff9f9d0" memberName="samplerNoiseComponent"
                    virtualName="" explicitFocusOrder="0" pos="0% 79.875% 100% 20.281%"
                    class="VASTSamplerNoiseComponent" params="myEditor, myProcessor"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

