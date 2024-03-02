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
#include "../../Engine/VASTEngineHeader.h"
#include "../VASTAudioProcessorEditor.h"
#include "VASTGenericEditor.h"
//[/Headers]

#include "VASTFXEffectPane.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
VASTFXEffectPane::VASTFXEffectPane (AudioProcessorEditor *editor, AudioProcessor* processor, int busnr)
    : myEditor((VASTAudioProcessorEditor*)editor), myProcessor((VASTAudioProcessor*)processor), myBusnr(busnr)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 4000);


    //[Constructor] You can add your own custom stuff here..
    if (m_lazyInit == false)
        lazyInit();
	setOpaque(true);
    //[/Constructor]
}

VASTFXEffectPane::~VASTFXEffectPane()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
	this->setLookAndFeel(nullptr);
    //[/Destructor_pre]



    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void VASTFXEffectPane::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
	if (m_lazyInit == false)
		lazyInit();
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff202324));

    //[UserPaint] Add your own custom painting code here..

    g.fillAll(myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colFXEffectPaneBackground));

    //[/UserPaint]
}

void VASTFXEffectPane::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
if (juce::Desktop::getInstance().isHeadless() == false)
    {
		updateGUI("", false);
	}
    //[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void VASTFXEffectPane::updateGUI(const String compID, bool toggleState) {
	int visibleComps = 0;
	int width = getWidth();
	int fxheight = this->getParentHeight() / 6.5f;

	CVASTFXBus* fxBus;
	switch (myBusnr) {
	case 0:
		fxBus = &myProcessor->m_pVASTXperience.m_fxBus1;
		break;
	case 1:
		fxBus = &myProcessor->m_pVASTXperience.m_fxBus2;
		break;
	case 2:
		fxBus = &myProcessor->m_pVASTXperience.m_fxBus3;
		break;
	default:
		jassert(false);
		return;
	}

	for (int i = 0; i < m_editors.size(); i++) {
		int fxno = fxBus->getSequence(i);

		bool isOff = m_editors[fxno]->myEffect->shallBeOff();
		if ((compID != "") &&
			(m_editors[fxno]->myEffect->getOnOffParameter().equalsIgnoreCase(compID)))
			isOff = !toggleState;

		if (isOff) {
			m_editors[fxno]->setVisible(false);
		}
		else {
			m_editors[fxno]->setVisible(true);
			visibleComps++;
		}
		//m_editors[i]->setBounds(0, 82 * (visibleComps - 1), width, getHeight()/14.f);
		m_editors[fxno]->setBounds(0, (fxheight + 2.f) * (visibleComps - 1), width, fxheight);
		m_editors[fxno]->resized();
		m_editors[fxno]->repaint();
	}
}

void VASTFXEffectPane::lazyInit() {
	for (int i = 0; i < myProcessor->m_pVASTXperience.m_fxBus1.effectBus.size(); i++) {
		VASTGenericEditor* geditor;
		switch (myBusnr) {
		case 0:
			geditor = new VASTGenericEditor(myProcessor->m_pVASTXperience.m_fxBus1.effectBus[i]->effectPlugin, myProcessor, myEditor, 0);
			break;
		case 1:
			geditor = new VASTGenericEditor(myProcessor->m_pVASTXperience.m_fxBus2.effectBus[i]->effectPlugin, myProcessor, myEditor, 1);
			break;
		case 2:
			geditor = new VASTGenericEditor(myProcessor->m_pVASTXperience.m_fxBus3.effectBus[i]->effectPlugin, myProcessor, myEditor, 2);
			break;
		default:
			jassert(false);
			return;
		}
		addAndMakeVisible(geditor);
		geditor->setBounds(0, 82 * i, getWidth(), 80);
		m_editors.add(geditor);
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

<JUCER_COMPONENT documentType="Component" className="VASTFXEffectPane" componentName=""
                 parentClasses="public Component" constructorParams="AudioProcessorEditor *editor, AudioProcessor* processor, int busnr"
                 variableInitialisers="myEditor((VASTAudioProcessorEditor*)editor), myProcessor((VASTAudioProcessor*)processor), myBusnr(busnr)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="600" initialHeight="4000">
  <BACKGROUND backgroundColour="ff202324"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

