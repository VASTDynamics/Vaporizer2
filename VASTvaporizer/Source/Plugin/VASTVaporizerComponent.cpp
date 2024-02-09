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
#include "../Engine/VASTEngineHeader.h"
#include "VASTAudioProcessor.h"
#include "VASTAudioProcessorEditor.h"
#include "VASTControls/VASTComboBrev.h"
#include "VASTControls/VASTComboBox.h"
#include "VASTControls/VASTImageButton.h"
#include "VASTFilterComponent.h"
#include "VASTFXComponent.h"
#include "VASTMatrixComponent.h"
#include "VASTArpComponent.h"
#include "VASTLFOMsegComponent.h"
#include "VASTWaveTableEditorComponent.h"
#include "VASTPresetEditorComponent.h"
#include "VASTMasterVoicingComponent.h"
#include "VASTHeaderComponent.h"
#include "VASTSidePanelComponent.h"
#include "VASTGeneratorsComponent.h"
#include "VASTKeyboardComponent.h"
#include "../Engine/VASTXperience.h"
#include "../Engine/VASTSingleNote.h"
#include "VASTUtils/VASTLookAndFeel.h"
#include "VASTSidePanelHeader.h"

//[/Headers]

#include "VASTVaporizerComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...

VASTTabbedComponent::VASTTabbedComponent(TabbedButtonBar::Orientation orientation, AudioProcessorEditor *editor, AudioProcessor* processor) : TabbedComponent(orientation), myProcessor(processor), myEditor(editor)
{
    TabChangedFunc = [](int) {};
    
    setTabBarDepth(30);
    addTab(TRANS("WT EDITOR"), Colour(0xff3e3e3e), new VASTWaveTableEditorComponent(myEditor, myProcessor), true, TabSequence::WTEDITOR);
    addTab(TRANS("FILTER"), Colour(0xff334761), new Label(), true, TabSequence::FILTER);
    addTab(TRANS("LFO MSEG ENV"), Colour(0xff68493e), new Label(), true, TabSequence::LFOMSEG);
    addTab(TRANS("MATRIX"), Colour(0xff2d5f33), new Label(), true, TabSequence::MATRIX);
    addTab(TRANS("FX"), Colour(0xff7d7d3b), new Label(), true, TabSequence::FX);
    addTab(TRANS("ARP"), Colour(0xff73376c), new Label(), true, TabSequence::ARP);
    addTab(TRANS("PRESET"), Colour(0xff3d6065), new Label(), true, TabSequence::PRESET);
    setCurrentTabIndex(0);
    currentTabChanged(0, ""); //to force update
    getTabbedButtonBar().setColour(TabbedButtonBar::tabTextColourId, juce::Colour::fromFloatRGBA(0.f, 0.f, 0.f, 1.f));
    getTabbedButtonBar().setColour(TabbedButtonBar::frontTextColourId, juce::Colour::fromFloatRGBA(1.f, 1.f, 1.f, 1.f));
    setOutline(0);
    setOpaque(false); // this does not work!
    m_bInitialized = true;
    VASTWaveTableEditorComponent* tab1 = dynamic_cast<VASTWaveTableEditorComponent*>(getTabContentComponent(TabSequence::WTEDITOR));
    if (tab1 != nullptr)
        tab1->startAutoUpdate();
}

void VASTTabbedComponent::currentTabChanged(int index, const String&) {
    if (!m_bInitialized)
        return;
	if (index == -1)
		return;

    VASTWaveTableEditorComponent* tab1 = dynamic_cast<VASTWaveTableEditorComponent*>(getTabContentComponent(TabSequence::WTEDITOR));
    if (tab1 == nullptr) return; //not initialized
    
    //lazy load
    VASTFilterComponent* tab2 = dynamic_cast<VASTFilterComponent*>(getTabContentComponent(TabSequence::FILTER));
    if (tab2 == nullptr) {
        if (index == TabSequence::FILTER) {
            removeTab(index);
            addTab(TRANS("FILTER"), Colour(0xff334761), new VASTFilterComponent(myEditor, myProcessor), true, TabSequence::FILTER);
            setCurrentTabIndex(TabSequence::FILTER);
            ((VASTAudioProcessorEditor*)myEditor)->vaporizerComponent->lookAndFeelChanged();
        }
    }
    VASTLFOMsegComponent* tab3 = dynamic_cast<VASTLFOMsegComponent*>(getTabContentComponent(TabSequence::LFOMSEG));
    if (tab3 == nullptr) {
        if (index == TabSequence::LFOMSEG) {
            removeTab(index);
            addTab(TRANS("LFO MSEG ENV"), Colour(0xff68493e), new VASTLFOMsegComponent(myEditor, myProcessor), true, TabSequence::LFOMSEG);
            setCurrentTabIndex(TabSequence::LFOMSEG);
            ((VASTAudioProcessorEditor*)myEditor)->vaporizerComponent->lookAndFeelChanged();
        }
    }
    VASTMatrixComponent* tab4 = dynamic_cast<VASTMatrixComponent*>(getTabContentComponent(TabSequence::MATRIX));
    if (tab4 == nullptr) {
        if (index == TabSequence::MATRIX) {
            removeTab(index);
            addTab(TRANS("MATRIX"), Colour(0xff2d5f33), new VASTMatrixComponent(myEditor, myProcessor), true, TabSequence::MATRIX);
            setCurrentTabIndex(TabSequence::MATRIX);
            ((VASTAudioProcessorEditor*)myEditor)->vaporizerComponent->lookAndFeelChanged();
        }
    }
    VASTFXComponent* tab5 = dynamic_cast<VASTFXComponent*>(getTabContentComponent(TabSequence::FX));
    if (tab5 == nullptr) {
        if (index == TabSequence::FX) {
            removeTab(index);
            addTab(TRANS("FX"), Colour(0xff7d7d3b), new VASTFXComponent(myEditor, myProcessor), true, TabSequence::FX);
            setCurrentTabIndex(TabSequence::FX);
            ((VASTAudioProcessorEditor*)myEditor)->vaporizerComponent->lookAndFeelChanged();
        }
    }
    VASTArpComponent* tab6 = dynamic_cast<VASTArpComponent*>(getTabContentComponent(TabSequence::ARP));
    if (tab6 == nullptr) {
        if (index == TabSequence::ARP) {
            removeTab(index);
            addTab(TRANS("ARP"), Colour(0xff73376c), new VASTArpComponent(myEditor, myProcessor), true, TabSequence::ARP);
            setCurrentTabIndex(TabSequence::ARP);
            ((VASTAudioProcessorEditor*)myEditor)->vaporizerComponent->lookAndFeelChanged();
        }
    }
    VASTPresetEditorComponent* tab7 = dynamic_cast<VASTPresetEditorComponent*>(getTabContentComponent(TabSequence::PRESET));
    if (tab7 == nullptr) {
        if (index == TabSequence::PRESET) {
            removeTab(index);
            addTab(TRANS("PRESET"), Colour(0xff3d6065), new VASTPresetEditorComponent(myEditor, myProcessor), true, TabSequence::PRESET);
            setCurrentTabIndex(TabSequence::PRESET);
            ((VASTAudioProcessorEditor*)myEditor)->vaporizerComponent->lookAndFeelChanged();
        }
    }

    if (tab1 != nullptr)
        tab1->stopAutoUpdate();
    if (tab2 != nullptr)
        tab2->stopAutoUpdate();
    if (tab3 != nullptr)
        tab3->stopAutoUpdate();
    if (tab4 != nullptr)
        tab4->stopAutoUpdate();
    if (tab5 != nullptr)
        tab5->stopAutoUpdate();
    if (tab6 != nullptr)
        tab6->stopAutoUpdate();
    if (tab7 != nullptr)
        tab7->stopAutoUpdate();
    
    switch (index) {
    case TabSequence::WTEDITOR:
            if (tab1 != nullptr)
                tab1->startAutoUpdate();
        break;
    case TabSequence::FILTER:
            if (tab2 != nullptr)
                tab2->startAutoUpdate();
        break;
    case TabSequence::LFOMSEG:
            if (tab3 != nullptr)
                tab3->startAutoUpdate();
        break;
    case TabSequence::MATRIX:
            if (tab4 != nullptr)
                tab4->startAutoUpdate();
        break;
    case TabSequence::FX:
            if (tab5 != nullptr)
                tab5->startAutoUpdate();
        break;
    case TabSequence::ARP:
            if (tab6 != nullptr)
                tab6->startAutoUpdate();
        break;
    case TabSequence::PRESET:
            if (tab7 != nullptr)
                tab7->startAutoUpdate();
        break;
    }

    TabChangedFunc(index);
}


//[/MiscUserDefs]

//==============================================================================
VASTVaporizerComponent::VASTVaporizerComponent (AudioProcessorEditor *editor, AudioProcessor* processor)
    : myEditor((VASTAudioProcessorEditor*)editor), myProcessor((VASTAudioProcessor*)processor)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    setName ("VaporizerComponent");
    masterVoicingComponent.reset (new VASTMasterVoicingComponent (myEditor, myProcessor));
    addAndMakeVisible (masterVoicingComponent.get());
    masterVoicingComponent->setName ("masterVoicingComponent");

    headerComponent.reset (new VASTHeaderComponent (myEditor, myProcessor));
    addAndMakeVisible (headerComponent.get());
    headerComponent->setName ("headerComponent");

    c_concertinaPanel.reset (new VASTConcertinaPanel (myProcessor, true));
    addAndMakeVisible (c_concertinaPanel.get());
    c_concertinaPanel->setName ("c_concertinaPanel");

    logoComponent.reset (new VASTLogoComponent (myEditor, myProcessor));
    addAndMakeVisible (logoComponent.get());
    logoComponent->setName ("logoComponent");

    c_sidePanel.reset (new SidePanel ("Modulation Sources", 144, false));
    addAndMakeVisible (c_sidePanel.get());
    c_sidePanel->setExplicitFocusOrder (1);
    c_sidePanel->setName ("c_sidePanel");


    //[UserPreSize]

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

	sidePanelComponent.reset(new VASTSidePanelComponent(myEditor, myProcessor));
	sidePanelComponent->setLookAndFeel(myProcessor->getCurrentVASTLookAndFeel());
	c_sidePanelHeader.reset(new VASTSidePanelHeader(this));
	c_sidePanelHeader->setLookAndFeel(myProcessor->getCurrentVASTLookAndFeel());
	c_sidePanel->setTitleBarComponent(c_sidePanelHeader.get(), false, false);
	c_sidePanel->setContent(sidePanelComponent.get(), false); //do not own

	c_concertinaCenter.reset(new VASTConcertinaPanel(myProcessor, false));
	c_tabbedComponent.reset(new VASTTabbedComponent(TabbedButtonBar::TabsAtLeft, myEditor, myProcessor));
	lastMouseWheelEvent = juce::Time::getCurrentTime();

	c_keyboardComponent.reset(new VASTKeyboardComponent(myEditor, myProcessor));
	c_keyboardComponent->setLookAndFeel(myProcessor->getCurrentVASTLookAndFeel());

	c_generatorsComponent.reset(new VASTGeneratorsComponent(myEditor, myProcessor));
	c_generatorsComponent->setOpaque(true);
	c_concertinaCenter->addVASTPanels(c_generatorsComponent.get(), c_tabbedComponent.get(), 20000, 20000, 0.37465f, true, false, false); //can collapse generators but not the tabstrip
	c_concertinaCenter->setOpaque(false);
	c_concertinaPanel->addVASTPanels(c_concertinaCenter.get(), c_keyboardComponent.get(), 20000, 20000, 0.89339f, false, true, false); //can collapse midikeyboard but not rest

	c_concertinaPanel->setOpaque(false);

	getTopLevelComponent()->addKeyListener(this);
	lookAndFeelChanged();
	setOpaque(true);

	//setSize(myProcessor->m_iUserTargetPluginWidth, myProcessor->m_iUserTargetPluginHeight);

	//initAll();	// calls updateAll(); //otherwise black osci //has to be first Tab!! //CHANGED CHECK!
	myProcessor->requestUIInit();

	isActive = true;

	return;

	//rest not called

    //[/UserPreSize]

    setSize (1420, 820);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

VASTVaporizerComponent::~VASTVaporizerComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    VDBG("Destructing VASTVaporizerComponent");
	isActive = false;
	this->setLookAndFeel(nullptr);
	c_sidePanel->setLookAndFeel(nullptr);
    VDBG("End Pre1 Destructing VASTVaporizerComponent");
	sidePanelComponent->setLookAndFeel(nullptr);

    c_generatorsComponent->setLookAndFeel(nullptr);
    c_generatorsComponent = nullptr;

    c_sidePanelHeader->setLookAndFeel(nullptr);
    c_sidePanelHeader = nullptr;


    c_tabbedComponent->setLookAndFeel(nullptr);
    c_tabbedComponent = nullptr;

    c_keyboardComponent->setLookAndFeel(nullptr);
    c_keyboardComponent = nullptr;
    c_concertinaPanel->setLookAndFeel(nullptr);
    c_concertinaPanel = nullptr;
    c_concertinaCenter->setLookAndFeel(nullptr);
    c_concertinaCenter = nullptr;

        VDBG("End Pre Destructing VASTVaporizerComponent");
    //[/Destructor_pre]

    masterVoicingComponent = nullptr;
    headerComponent = nullptr;
    c_concertinaPanel = nullptr;
    logoComponent = nullptr;
    c_sidePanel = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    VDBG("End Destructing VASTVaporizerComponent");
    //[/Destructor]
}

//==============================================================================
void VASTVaporizerComponent::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
	if (m_paintToImage) {
		//m_SnapShotImage = Image(Image::RGB, getWidth(), getHeight(), true);
		//VDBG(getWidth());
		//m_SnapShotImage.clear(juce::Rectangle<int>(0, 0, getWidth(), getHeight()), Colours::darkcyan);
		g.drawImageAt(m_SnapShotImage, 0, 0);
		return;
	}

	const Displays::Display dsp1 = *Desktop::getInstance().getDisplays().getDisplayForPoint(getScreenBounds().getTopLeft(), false);
	const Displays::Display dsp2 = *Desktop::getInstance().getDisplays().getDisplayForPoint(getScreenBounds().getBottomRight(), false);
	if (dsp1.topLeftPhysical == dsp2.topLeftPhysical) {
		if (m_lastDisplayTopLeftPoint == Point<int>(-1, -1))
			m_lastDisplayTopLeftPoint = dsp1.topLeftPhysical;
		else {
			if (dsp1.topLeftPhysical != m_lastDisplayTopLeftPoint) { //Plugin was moved from one screen to another
				m_lastDisplayTopLeftPoint = dsp1.topLeftPhysical;
				int lwidth = myEditor->getWidth();
				int lheight = myEditor->getHeight();
                Component::SafePointer<VASTAudioProcessorEditor> editor(myEditor);
				Timer::callAfterDelay(10, [editor, lwidth, lheight, this] { //force resize from paint!
					if (editor != nullptr)
						editor.getComponent()->setBoundsConstrained(juce::Rectangle<int>(0, 0, lwidth + 2, lheight)); });
				Timer::callAfterDelay(100, [editor, lwidth, lheight, this] {
					if (editor != nullptr)
						editor.getComponent()->setBoundsConstrained(juce::Rectangle<int>(0, 0, lwidth, lheight)); });
			}
		}
	}

	m_wasShown = true;
    //[/UserPrePaint]

    g.fillAll (juce::Colours::black);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void VASTVaporizerComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
	if (m_paintToImage) {
		m_SnapShotImage = m_SnapShotImage.rescaled(getWidth(), getHeight(), Graphics::highResamplingQuality);
		return;
	}
    //[/UserPreResize]

    masterVoicingComponent->setBounds (proportionOfWidth (0.5296f), proportionOfHeight (0.0000f), proportionOfWidth (0.4704f), proportionOfHeight (0.0927f));
    headerComponent->setBounds (proportionOfWidth (0.2993f), proportionOfHeight (0.0000f), proportionOfWidth (0.2289f), proportionOfHeight (0.0927f));
    c_concertinaPanel->setBounds (proportionOfWidth (0.0000f), proportionOfHeight (0.0951f), proportionOfWidth (1.0000f), proportionOfHeight (0.9049f));
    logoComponent->setBounds (proportionOfWidth (0.0000f), proportionOfHeight (0.0000f), proportionOfWidth (0.2979f), proportionOfHeight (0.0927f));
    c_sidePanel->setBounds (proportionOfWidth (1.0000f) - proportionOfWidth (0.0782f), 0, proportionOfWidth (0.0782f), proportionOfHeight (1.0000f));
    //[UserResized] Add your own custom resize handling here..

	c_sidePanel.reset(new SidePanel("Modulation Sources", proportionOfWidth(0.1014f), false));
	c_sidePanel->setContent(sidePanelComponent.get(), false); //do not own
	c_sidePanel->setTitleBarHeight(proportionOfHeight(0.03f));
	c_sidePanel->setTitleBarComponent(c_sidePanelHeader.get(), false, false);
	c_sidePanel->setLookAndFeel(myProcessor->getCurrentVASTLookAndFeel());
	addAndMakeVisible(c_sidePanel.get());

	c_tabbedComponent->setTabBarDepth(c_tabbedComponent->getWidth() * 0.035f);

	/*
	ModifierKeys modifiers = ModifierKeys::getCurrentModifiersRealtime();
	if (modifiers.isLeftButtonDown()) {
		myProcessor->m_iUserTargetPluginWidth = myEditor->getWidth();
		myProcessor->m_iUserTargetPluginHeight = myEditor->getHeight();
		myProcessor->writeSettingsToFile();
	}
	*/

	VDBG("Vaporizer component resized to w: " << this->getWidth() << " h: " << this->getHeight());
    //[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

TabbedComponent* VASTVaporizerComponent::getTabbedComponent() {
	return c_tabbedComponent.get();
}

VASTOscillatorComponent* VASTVaporizerComponent::getOscillatorComponent(int num) {
	VASTGeneratorsComponent* generators = (VASTGeneratorsComponent*)c_concertinaCenter->getFirstComponent();
	if (num == 0) return generators->getOscillatorAComponent();
	if (num == 1) return generators->getOscillatorBComponent();
	if (num == 2) return generators->getOscillatorCComponent();
	if (num == 3) return generators->getOscillatorDComponent();
	return generators->getOscillatorAComponent(); //default
}

//Label* VASTVaporizerComponent::getComponentUndo() {
//	return c_undoSize.get();
//}

void VASTVaporizerComponent::processBlock(AudioSampleBuffer& buffer) {
	if (!isActive) return;
	//int numSamples = buffer.getNumSamples();
	//if (buffer.getNumChannels() > 0) {
		//const float* samplesL = buffer.getReadPointer(0);
		//audioOscilloscopeL->processBlock(samplesL, numSamples);
	//}
	//if (buffer.getNumChannels() > 1) {
		//const float* samplesR = buffer.getReadPointer(1);
		//audioOscilloscopeR->processBlock(samplesR, numSamples);
	//}
	//this only copys data to buffer - painting is done by timercallback
}

void VASTVaporizerComponent::setLicenseText(StringRef text, bool bInErrorState, VASTAudioProcessor::vastErrorState iErrorState) {
	if (bInErrorState) {		
		String outStr = "Error: " + myProcessor->vastErrorStateToString(iErrorState);
		VDBG("ERRORSTATE: " + outStr);
		headerComponent->getLabelLicense()->setText(outStr, NotificationType::sendNotification);
	}
	else
		headerComponent->getLabelLicense()->setText(text, NotificationType::sendNotification);
}

void VASTVaporizerComponent::setVersionText(StringRef text) {
	masterVoicingComponent->getLabelVersion()->setText(text, NotificationType::sendNotification);
}

void VASTVaporizerComponent::initAll() {
	lookAndFeelChanged();
	VASTWaveTableEditorComponent* tab1 = dynamic_cast<VASTWaveTableEditorComponent*>(c_tabbedComponent->getTabContentComponent(VASTTabbedComponent::TabSequence::WTEDITOR));
    if (tab1 != nullptr)
        tab1->initAll();

	VASTLFOMsegComponent* tab3 = dynamic_cast<VASTLFOMsegComponent*>(c_tabbedComponent->getTabContentComponent(VASTTabbedComponent::TabSequence::LFOMSEG));
    if (tab3 != nullptr)
        tab3->initAll();

	VASTArpComponent* tab6 = dynamic_cast<VASTArpComponent*>(c_tabbedComponent->getTabContentComponent(VASTTabbedComponent::TabSequence::ARP));
    if (tab6 != nullptr)
        tab6->initAll();

	getOscillatorComponent(0)->initAll();
	getOscillatorComponent(1)->initAll();
	getOscillatorComponent(2)->initAll();
	getOscillatorComponent(3)->initAll();

	masterVoicingComponent->initAll();

	//updateAll();
	repaint(); //CHECK
	myProcessor->requestUIUpdate();
}

void VASTVaporizerComponent::updateMatrixDisplay() {
	VASTMatrixComponent* matrix = dynamic_cast<VASTMatrixComponent*>(c_tabbedComponent->getTabContentComponent(VASTTabbedComponent::TabSequence::MATRIX));
	if (matrix != nullptr)
        matrix->updateAll();
}

void VASTVaporizerComponent::updateHeader() {
	String presetdisplay = myProcessor->m_presetData.getCurPatchData().category + " " + myProcessor->m_presetData.getCurPatchData().presetname;
	headerComponent->getComponentCPreset()->setText(presetdisplay, juce::NotificationType::dontSendNotification);
}

void VASTVaporizerComponent::updateAll() {
	VASTWaveTableEditorComponent* tab1 = dynamic_cast<VASTWaveTableEditorComponent*>(c_tabbedComponent->getTabContentComponent(VASTTabbedComponent::TabSequence::WTEDITOR));
    if (tab1 != nullptr)
        tab1->requestUIUpdate();

	VASTLFOMsegComponent* tab3 = dynamic_cast<VASTLFOMsegComponent*>(c_tabbedComponent->getTabContentComponent(VASTTabbedComponent::VASTTabbedComponent::TabSequence::LFOMSEG));
    if (tab3 != nullptr)
        tab3->updateAll();

	VASTFXComponent* tab5 = dynamic_cast<VASTFXComponent*>(c_tabbedComponent->getTabContentComponent(VASTTabbedComponent::TabSequence::FX));
    if (tab5 != nullptr)
        tab5->updateAll();

	VASTArpComponent* tab6 = dynamic_cast<VASTArpComponent*>(c_tabbedComponent->getTabContentComponent(VASTTabbedComponent::TabSequence::ARP));
    if (tab6 != nullptr)
        tab6->updateAll();

	VASTPresetEditorComponent* tab7 = dynamic_cast<VASTPresetEditorComponent*>(c_tabbedComponent->getTabContentComponent(VASTTabbedComponent::TabSequence::PRESET));
	if (tab7 != nullptr)
        tab7->updateAll();

	VASTGeneratorsComponent* generators = dynamic_cast<VASTGeneratorsComponent*>(c_concertinaCenter->getFirstComponent());
	if (generators != nullptr) {
		generators->getOscillatorAComponent()->updateAll();
		generators->getOscillatorBComponent()->updateAll();
		generators->getOscillatorCComponent()->updateAll();
		generators->getOscillatorDComponent()->updateAll();
	}

	updateHeader();
	sidePanelComponent->getComponentCustomMod1()->setModString(myProcessor->m_presetData.getCurPatchData().customModulator1Text);
	sidePanelComponent->getComponentCustomMod2()->setModString(myProcessor->m_presetData.getCurPatchData().customModulator2Text);
	sidePanelComponent->getComponentCustomMod3()->setModString(myProcessor->m_presetData.getCurPatchData().customModulator3Text);
	sidePanelComponent->getComponentCustomMod4()->setModString(myProcessor->m_presetData.getCurPatchData().customModulator4Text);

	c_keyboardComponent->updateAll();

	//if presetBrowser is shown update!
	//if (c_concertinaPanel->getPresetOverlay() != nullptr)
		//c_concertinaPanel->getPresetOverlay()->updateAll();

}

bool VASTVaporizerComponent::keyStateChanged(bool isKeyDown, Component* originatingComponent) {
	Component* focusComp = getCurrentlyFocusedComponent();
	if (auto* textEditor = dynamic_cast<TextEditor*> (focusComp)) {
		//VDBG("TextEditor");
	} else
		c_keyboardComponent->getMidiKeyboard()->keyStateChanged(isKeyDown); //route all input through to midikeyboard
    return true;
}

bool VASTVaporizerComponent::keyPressed(const KeyPress& key, Component* originatingComponent) { //return true for the ableton bug
	VDBG(key.getTextDescription() << "   " << key.getKeyCode());
	ModifierKeys mod = key.getModifiers();

	VASTWaveTableEditorComponent* myWtEditor = ((VASTWaveTableEditorComponent*)c_tabbedComponent->getTabContentComponent(VASTTabbedComponent::TabSequence::WTEDITOR));
	jassert(myWtEditor != nullptr); //sequence changed?
	if (myWtEditor->isVisible()) { //WT Editor
		if (mod.isCtrlDown()) {
			if (key.isKeyCode(65)) {//CTRL + a
				myWtEditor->multiSelectAll();
				myWtEditor->initSliders();
				myWtEditor->updateAll(false);
			}
			else if (key.isKeyCode(67)) {//CTRL + c
				myWtEditor->copyPositions();
			}
			else if (key.isKeyCode(88)) {//CTRL + x
				myWtEditor->cutPositions();
				myWtEditor->initSliders();
				myWtEditor->updateAll(false);
			}
			else if (key.isKeyCode(86)) {//CTRL + v
				myWtEditor->replaceWithCutCopiedPositions();
				myWtEditor->initSliders();
				myWtEditor->updateAll(false);
			}
			else if (key.isKeyCode(82)) {//CTRL + r
				myWtEditor->reverseOrderSelectedPositions();
				myWtEditor->initSliders();
				myWtEditor->updateAll(false);
			}
			else if (key.isKeyCode(83)) {//CTRL + s
				myWtEditor->stretchAndInterpolateSelectedPositions();
				myWtEditor->initSliders();
				myWtEditor->updateAll(false);
			}
			else if (key.isKeyCode(84)) {//CTRL + t
				myWtEditor->thinOutSelectedPositions();
				myWtEditor->initSliders();
				myWtEditor->updateAll(false);
			}
			else if (key.isKeyCode(65581)) {//CTRL + insert
				myWtEditor->insertCutCopiedPositions(false, false);
				myWtEditor->initSliders();
				myWtEditor->updateAll(false);
			}
			else if (key.isKeyCode(68)) {//CTRL + d duplicate
				myWtEditor->duplicateSelectedPositions();
				myWtEditor->initSliders();
				myWtEditor->updateAll(false);
			}
			else if (key.isKeyCode(78)) {//CTRL + n normalize
				std::thread edit_thread(&VASTWaveTableEditorComponent::threadedEditorFunction, EditorFunction::Normalize, 0.f, /*wtPosStart, wtPosEnd,*/ 0, 0, myWtEditor, 0.f, 0.f, nullptr, true);
				edit_thread.detach();
			}
			else if (key.isKeyCode(KeyPress::leftKey)) {//CTRL + Cursor left
				myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->m_bank].addSoftFadeEditor();
				std::shared_ptr<CVASTWaveTable> wavetable = myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->m_bank].getSoftOrCopyWavetable(true); //need a copy as wtpos structure will be changed
				int fromwtPos = wavetable->getMultiSelectBegin();
				int lastwtPos = wavetable->getMultiSelectEnd();
				if ((fromwtPos > 0) && (lastwtPos < wavetable->getNumPositions())) {
					wavetable->copyPositionToOtherWavetable(fromwtPos - 1, lastwtPos + 1, wavetable.get());
					wavetable->deletePosition(fromwtPos - 1);
					wavetable->setSelection(fromwtPos - 1, lastwtPos - 1);
				}
				myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->m_bank].setWavetableSoftFade(wavetable);
				myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->m_bank].removeSoftFadeEditor();
				myWtEditor->initSliders();
				myWtEditor->updateAll(false);
			}
			else if (key.isKeyCode(KeyPress::rightKey)) {//CTRL + Cursor right
				myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->m_bank].addSoftFadeEditor();
				std::shared_ptr<CVASTWaveTable> wavetable = myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->m_bank].getSoftOrCopyWavetable(true); //need a copy as wtpos structure will be changed
				int fromwtPos = wavetable->getMultiSelectBegin();
				int lastwtPos = wavetable->getMultiSelectEnd();
				if ((fromwtPos >= 0) && (lastwtPos < wavetable->getNumPositions() - 1)) {
					wavetable->copyPositionToOtherWavetable(lastwtPos + 1, fromwtPos, wavetable.get());
					wavetable->deletePosition(lastwtPos + 2);
					wavetable->setSelection(fromwtPos + 1, lastwtPos + 1);
				}
				myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->m_bank].setWavetableSoftFade(wavetable);
				myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->m_bank].removeSoftFadeEditor();
				myWtEditor->initSliders();
				myWtEditor->updateAll(false);
			}
		}
		else if (mod.isShiftDown()) {
			if (key.isKeyCode(KeyPress::leftKey)) {//Shift + Cursor left
				myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->m_bank].addSoftFadeEditor();
				std::shared_ptr<CVASTWaveTable> wavetable = myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->m_bank].getSoftOrCopyWavetable(true, true); //need a copy as wtpos structure will be changed - selection only - keep freqs
				int wtPos = wavetable->getSelectedWtPos();
				if (wavetable->isMultiSelected())
					wtPos = wavetable->getMultiSelectBegin();
				wtPos--;
				if (wtPos < 0) wtPos = 0;
				wavetable->setMultiSelect(wtPos);
				myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->m_bank].setWavetableSoftFade(wavetable);
				myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->m_bank].removeSoftFadeEditor();
				myWtEditor->initSliders();
				myWtEditor->updateAll(false);
			}
			else if (key.isKeyCode(KeyPress::rightKey)) {//Shift + Cursor right
				myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->m_bank].addSoftFadeEditor();
				std::shared_ptr<CVASTWaveTable> wavetable = myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->m_bank].getSoftOrCopyWavetable(true, true); //need a copy as wtpos structure will be changed - selection only - keep freqs
				int wtPos = wavetable->getSelectedWtPos();
				if (wavetable->isMultiSelected())
					wtPos = wavetable->getMultiSelectEnd();
				wtPos++;
				if (wtPos < 0) wtPos = 0;
				wavetable->setMultiSelect(wtPos);
				myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->m_bank].setWavetableSoftFade(wavetable);
				myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->m_bank].removeSoftFadeEditor();
				myWtEditor->initSliders();
				myWtEditor->updateAll(false);
			}
			else if (key.isKeyCode(KeyPress::endKey)) {//Shift + end
				myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->m_bank].addSoftFadeEditor();
				std::shared_ptr<CVASTWaveTable> wavetable = myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->m_bank].getSoftOrCopyWavetable(true, true); //need a copy as wtpos structure will be changed - selection only - keep freqs
				int wtPos = wavetable->getNumPositions() - 1;
				wavetable->setMultiSelect(wtPos);
				myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->m_bank].setWavetableSoftFade(wavetable);
				myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->m_bank].removeSoftFadeEditor();
				myWtEditor->initSliders();
				myWtEditor->updateAll(false);
			}
			else if (key.isKeyCode(KeyPress::homeKey)) {//Shift + pos1/home
				myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->m_bank].addSoftFadeEditor();
				std::shared_ptr<CVASTWaveTable> wavetable = myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->m_bank].getSoftOrCopyWavetable(true, true); //need a copy as wtpos structure will be changed - selection only - keep freqs
				int wtPos = 0;
				wavetable->setMultiSelect(wtPos);
				myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->m_bank].setWavetableSoftFade(wavetable);
				myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->m_bank].removeSoftFadeEditor();
				myWtEditor->initSliders();
				myWtEditor->updateAll(false);
			}
			else if (key.isKeyCode(68)) {//Shift + d (toggle draw mode)
				myWtEditor->toggleDrawMode();
				myWtEditor->updateAll(false);
			}
		}
		else
			if (key.isKeyCode(KeyPress::insertKey)) {//INS
				myWtEditor->insertPosition();
				myWtEditor->initSliders();
				myWtEditor->updateAll(false);
			}
			else if (key.isKeyCode(KeyPress::deleteKey)) {//DEL
				myWtEditor->deletePositions(false);
				myWtEditor->initSliders();
				myWtEditor->updateAll(false);
			}
			else if (key.isKeyCode(KeyPress::leftKey)) {//Cursor left
				myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->m_bank].addSoftFadeEditor();
				std::shared_ptr<CVASTWaveTable> wavetable = myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->m_bank].getSoftOrCopyWavetable(true, true); //need a copy as wtpos structure will be changed - selection only - keep freqs
				int wtPos = wavetable->getSelectedWtPos();
				wtPos--;
				if (wtPos < 0) wtPos = 0;
				wavetable->setSelectedWtPos(wtPos);
				myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->m_bank].setWavetableSoftFade(wavetable);
				myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->m_bank].removeSoftFadeEditor();
				myWtEditor->initSliders();
				myWtEditor->updateAll(false);
			}
			else if (key.isKeyCode(KeyPress::rightKey)) {//Cursor right
				myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->m_bank].addSoftFadeEditor();
				std::shared_ptr<CVASTWaveTable> wavetable = myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->m_bank].getSoftOrCopyWavetable(true, true); //need a copy as wtpos structure will be changed - selection only - keep freqs
				int wtPos = wavetable->getSelectedWtPos();
				wtPos++;
				if (wtPos < 0) wtPos = 0;
				wavetable->setSelectedWtPos(wtPos);
				myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->m_bank].setWavetableSoftFade(wavetable);
				myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->m_bank].removeSoftFadeEditor();
				myWtEditor->initSliders();
				myWtEditor->updateAll(false);
			}
			else if (key.isKeyCode(KeyPress::homeKey)) {//pos1/home
				myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->m_bank].addSoftFadeEditor();
				std::shared_ptr<CVASTWaveTable> wavetable = myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->m_bank].getSoftOrCopyWavetable(true, true); //need a copy as wtpos structure will be changed - selection only - keep freqs
				int wtPos = 0;
				wavetable->setSelectedWtPos(wtPos);
				myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->m_bank].setWavetableSoftFade(wavetable);
				myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->m_bank].removeSoftFadeEditor();
				myWtEditor->initSliders();
				myWtEditor->updateAll(false);
			}
			else if (key.isKeyCode(KeyPress::endKey)) {//end
				myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->m_bank].addSoftFadeEditor();
				std::shared_ptr<CVASTWaveTable> wavetable = myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->m_bank].getSoftOrCopyWavetable(true, true); //need a copy as wtpos structure will be changed - selection only - keep freqs
				int wtPos = wavetable->getNumPositions() - 1;
				wavetable->setSelectedWtPos(wtPos);
				myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->m_bank].setWavetableSoftFade(wavetable);
				myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->m_bank].removeSoftFadeEditor();
				myWtEditor->initSliders();
				myWtEditor->updateAll(false);
			}
	}

	if (key.isKeyCode(KeyPress::tabKey)) {
		VDBG(juce::Component::getCurrentlyFocusedComponent()->getName());
		Component* comp = nullptr;
		if (!mod.isShiftDown())
			comp = juce::Component::getCurrentlyFocusedComponent()->createKeyboardFocusTraverser()->getNextComponent(juce::Component::getCurrentlyFocusedComponent());
		else
			comp = juce::Component::getCurrentlyFocusedComponent()->createKeyboardFocusTraverser()->getPreviousComponent(juce::Component::getCurrentlyFocusedComponent());
		if (comp!=nullptr)
			comp->grabKeyboardFocus();
	}

	c_keyboardComponent->getMidiKeyboard()->keyPressed(key);  //route all input through to midikeyboard
	return true;
}

void VASTVaporizerComponent::lookAndFeelChanged() {

	c_tabbedComponent->getTabbedButtonBar().setColour(TabbedButtonBar::tabOutlineColourId, Colours::transparentBlack);
	c_tabbedComponent->getTabbedButtonBar().setColour(TabbedButtonBar::frontOutlineColourId, Colours::transparentBlack);

	c_tabbedComponent->getTabbedButtonBar().setTabBackgroundColour(0, myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colVaporizerComponentTabBarWTEDitorBackground));   // WT EDITOR
	c_tabbedComponent->getTabbedButtonBar().setTabBackgroundColour(1, myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colVaporizerComponentTabBarFilterBackground));   // FILTER
	c_tabbedComponent->getTabbedButtonBar().setTabBackgroundColour(2, myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colVaporizerComponentTabBarLFOMSEGBackground));   // LFO MSEG ENV
	c_tabbedComponent->getTabbedButtonBar().setTabBackgroundColour(3, myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colVaporizerComponentTabBarMatrixBackground));   // MATRIX
	c_tabbedComponent->getTabbedButtonBar().setTabBackgroundColour(4, myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colVaporizerComponentTabBarFXBackground));   // FX
	c_tabbedComponent->getTabbedButtonBar().setTabBackgroundColour(5, myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colVaporizerComponentTabBarARPBackground));   // ARP
	c_tabbedComponent->getTabbedButtonBar().setTabBackgroundColour(6, myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colVaporizerComponentTabBarPresetBackground));   // PRESET

	c_tabbedComponent->getTabbedButtonBar().setColour(TabbedButtonBar::tabTextColourId, myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colVaporizerComponentTabBarText));
	c_tabbedComponent->getTabbedButtonBar().setColour(TabbedButtonBar::frontTextColourId, myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colVaporizerComponentTabBarTextSelected));

	sidePanelComponent->setLookAndFeel(myProcessor->getCurrentVASTLookAndFeel());
	c_sidePanel->setLookAndFeel(myProcessor->getCurrentVASTLookAndFeel());
	c_keyboardComponent->setLookAndFeel(myProcessor->getCurrentVASTLookAndFeel());

}

void VASTVaporizerComponent::mouseWheelMove(const MouseEvent &event, const MouseWheelDetails &wheel) {
	if (dynamic_cast<TabBarButton*>(event.originalComponent) == 0)
		return;
	if (!(c_tabbedComponent->getTabbedButtonBar().getScreenBounds().contains(event.getScreenPosition())))
		return;
	if (abs(lastMouseWheelEvent.getMilliseconds() - event.eventTime.getMilliseconds()) < 50)
		return;
	lastMouseWheelEvent = event.eventTime;
	int curidx = c_tabbedComponent->getTabbedButtonBar().getCurrentTabIndex();
	int numidx = c_tabbedComponent->getTabbedButtonBar().getNumTabs();
	if (wheel.deltaY < 0.f)
		curidx = (curidx + 1) % numidx;
	else if (wheel.deltaY > 0.f)
		curidx = ((curidx - 1) + numidx) % numidx;
	c_tabbedComponent->getTabbedButtonBar().setCurrentTabIndex(curidx);
}

void VASTVaporizerComponent::selectSizeMenu() {
	PopupMenu mainMenu;
	mainMenu.setLookAndFeel(myEditor->getCurrentVASTLookAndFeel());
	mainMenu.addSectionHeader("Set Zoom");
	mainMenu.addItem(1, "25%");
	mainMenu.addItem(2, "50%");
	mainMenu.addItem(3, "75%");
	mainMenu.addItem(4, "80%");
	mainMenu.addItem(5, "90%");
	mainMenu.addItem(6, "100%");
	mainMenu.addItem(7, "125%");
	mainMenu.addItem(8, "150%");
	mainMenu.addItem(9, "175%");
	mainMenu.addItem(10, "200%");
	mainMenu.addSeparator();
	mainMenu.addItem(20, "100% (default)");

	mainMenu.showMenuAsync(PopupMenu::Options().withTargetComponent(this).withTargetScreenArea(juce::Rectangle<int>{}.withPosition(Desktop::getMousePosition())), 
		juce::ModalCallbackFunction::create([this](int result) {
		if (result == 0)
		{
			// user dismissed the menu without picking anything
		}
		else if (result >= 1) {
			myProcessor->m_iUserTargetPluginWidth = myProcessor->m_iDefaultPluginWidth;
			myProcessor->m_iUserTargetPluginHeight = myProcessor->m_iDefaultPluginHeight;

			if (result == 1) {
				myProcessor->m_iUserTargetPluginWidth = myProcessor->m_iDefaultPluginWidth * 0.25f;
				myProcessor->m_iUserTargetPluginHeight = myProcessor->m_iDefaultPluginHeight * 0.25f;
			}
			else  if (result == 2) {
				myProcessor->m_iUserTargetPluginWidth = myProcessor->m_iDefaultPluginWidth * 0.5f;
				myProcessor->m_iUserTargetPluginHeight = myProcessor->m_iDefaultPluginHeight * 0.5f;
			}
			else  if (result == 3) {
				myProcessor->m_iUserTargetPluginWidth = myProcessor->m_iDefaultPluginWidth * 0.75f;
				myProcessor->m_iUserTargetPluginHeight = myProcessor->m_iDefaultPluginHeight * 0.75f;
			}
			else  if (result == 4) {
				myProcessor->m_iUserTargetPluginWidth = myProcessor->m_iDefaultPluginWidth * 0.8f;
				myProcessor->m_iUserTargetPluginHeight = myProcessor->m_iDefaultPluginHeight * 0.8f;
			}
			else  if (result == 5) {
				myProcessor->m_iUserTargetPluginWidth = myProcessor->m_iDefaultPluginWidth * 0.9f;
				myProcessor->m_iUserTargetPluginHeight = myProcessor->m_iDefaultPluginHeight * 0.9f;
			}
			else  if (result == 6) {
				myProcessor->m_iUserTargetPluginWidth = myProcessor->m_iDefaultPluginWidth * 1.f;
				myProcessor->m_iUserTargetPluginHeight = myProcessor->m_iDefaultPluginHeight * 1.f;
			}
			else  if (result == 7) {
				myProcessor->m_iUserTargetPluginWidth = myProcessor->m_iDefaultPluginWidth * 1.25f;
				myProcessor->m_iUserTargetPluginHeight = myProcessor->m_iDefaultPluginHeight * 1.25f;
			}
			else  if (result == 8) {
				myProcessor->m_iUserTargetPluginWidth = myProcessor->m_iDefaultPluginWidth * 1.5f;
				myProcessor->m_iUserTargetPluginHeight = myProcessor->m_iDefaultPluginHeight * 1.5f;
			}
			else  if (result == 9) {
				myProcessor->m_iUserTargetPluginWidth = myProcessor->m_iDefaultPluginWidth * 1.75f;
				myProcessor->m_iUserTargetPluginHeight = myProcessor->m_iDefaultPluginHeight * 1.75f;
			}
			else  if (result == 10) {
				myProcessor->m_iUserTargetPluginWidth = myProcessor->m_iDefaultPluginWidth * 2.f;
				myProcessor->m_iUserTargetPluginHeight = myProcessor->m_iDefaultPluginHeight * 2.f;
			}
			else  if (result == 20) {
				myProcessor->m_iUserTargetPluginWidth = myProcessor->m_iDefaultPluginWidth * 1.f;
				myProcessor->m_iUserTargetPluginHeight = myProcessor->m_iDefaultPluginHeight * 1.f;
			}

			if ((myProcessor->m_iUserTargetPluginWidth < myEditor->m_iMinWidth) || (myProcessor->m_iUserTargetPluginHeight < myEditor->m_iMinHeight)) {
				myProcessor->m_iUserTargetPluginWidth = myEditor->m_iMinWidth;
				myProcessor->m_iUserTargetPluginHeight = myEditor->m_iMinHeight;
			}

			myProcessor->writeSettingsToFile(); //must not be async!
			myEditor->setBoundsConstrained(juce::Rectangle<int>(0, 0, myProcessor->m_iUserTargetPluginWidth, myProcessor->m_iUserTargetPluginHeight));
		}
	}));
}


void VASTVaporizerComponent::startPaintingToImage() {
	m_SnapShotImage = createComponentSnapshot(getBounds());
	m_paintToImage = true;

	masterVoicingComponent->setVisible(false);
	headerComponent->setVisible(false);
	c_concertinaPanel->setVisible(false);
	logoComponent->setVisible(false);
	c_sidePanel->setVisible(false);
	c_tabbedComponent->setVisible(false);
}
void VASTVaporizerComponent::endPaintingToImage() {
	masterVoicingComponent->setVisible(true);
	headerComponent->setVisible(true);
	c_concertinaPanel->setVisible(true);
	logoComponent->setVisible(true);
	c_sidePanel->setVisible(true);
	c_tabbedComponent->setVisible(true);

	m_paintToImage = false;
	resized();
}

void VASTVaporizerComponent::parentSizeChanged() {
	VDBG("VASTVaporizerComponent::parentSizeChanged");
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="VASTVaporizerComponent" componentName="VaporizerComponent"
                 parentClasses="public Component, public Timer, public DragAndDropContainer, public KeyListener, public MidiKeyboardStateListener"
                 constructorParams="AudioProcessorEditor *editor, AudioProcessor* processor"
                 variableInitialisers="myEditor((VASTAudioProcessorEditor*)editor), myProcessor((VASTAudioProcessor*)processor)"
                 snapPixels="4" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="1420" initialHeight="820">
  <BACKGROUND backgroundColour="ff000000"/>
  <GENERICCOMPONENT name="masterVoicingComponent" id="37be3fcc840b4e7" memberName="masterVoicingComponent"
                    virtualName="" explicitFocusOrder="0" pos="52.958% 0% 47.042% 9.268%"
                    class="VASTMasterVoicingComponent" params="myEditor, myProcessor"/>
  <GENERICCOMPONENT name="headerComponent" id="39cd1ffcd2336e3" memberName="headerComponent"
                    virtualName="" explicitFocusOrder="0" pos="29.93% 0% 22.887% 9.268%"
                    class="VASTHeaderComponent" params="myEditor, myProcessor"/>
  <GENERICCOMPONENT name="c_concertinaPanel" id="6a32fd70f7710aad" memberName="c_concertinaPanel"
                    virtualName="" explicitFocusOrder="0" pos="0% 9.512% 100% 90.488%"
                    class="VASTConcertinaPanel" params="myProcessor, true"/>
  <GENERICCOMPONENT name="logoComponent" id="5953f03d6e61e89d" memberName="logoComponent"
                    virtualName="" explicitFocusOrder="0" pos="0% 0% 29.789% 9.268%"
                    class="VASTLogoComponent" params="myEditor, myProcessor"/>
  <GENERICCOMPONENT name="c_sidePanel" id="75d7f634820c0d6" memberName="c_sidePanel"
                    virtualName="" explicitFocusOrder="1" pos="100%r 0 7.817% 100%"
                    class="SidePanel" params="&quot;Modulation Sources&quot;, 144, false"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif

//==============================================================================
// Binary resources - be careful not to edit any of these sections!

// JUCER_RESOURCE: logokreis_svg, 3721, "../../Resources/logokreis.svg"
static const unsigned char resource_VASTVaporizerComponent_logokreis_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,70,45,56,
34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,33,45,45,32,67,114,101,97,116,101,100,32,119,105,116,104,32,73,110,107,115,99,97,112,101,32,40,104,116,116,112,58,47,47,119,119,
119,46,105,110,107,115,99,97,112,101,46,111,114,103,47,41,32,45,45,62,10,10,60,115,118,103,10,32,32,32,120,109,108,110,115,58,100,99,61,34,104,116,116,112,58,47,47,112,117,114,108,46,111,114,103,47,100,
99,47,101,108,101,109,101,110,116,115,47,49,46,49,47,34,10,32,32,32,120,109,108,110,115,58,99,99,61,34,104,116,116,112,58,47,47,99,114,101,97,116,105,118,101,99,111,109,109,111,110,115,46,111,114,103,
47,110,115,35,34,10,32,32,32,120,109,108,110,115,58,114,100,102,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,48,50,47,50,50,45,114,100,102,45,115,121,110,116,97,
120,45,110,115,35,34,10,32,32,32,120,109,108,110,115,58,115,118,103,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,118,103,34,10,32,32,32,120,109,108,110,115,
61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,118,103,34,10,32,32,32,120,109,108,110,115,58,115,111,100,105,112,111,100,105,61,34,104,116,116,112,58,47,47,115,
111,100,105,112,111,100,105,46,115,111,117,114,99,101,102,111,114,103,101,46,110,101,116,47,68,84,68,47,115,111,100,105,112,111,100,105,45,48,46,100,116,100,34,10,32,32,32,120,109,108,110,115,58,105,110,
107,115,99,97,112,101,61,34,104,116,116,112,58,47,47,119,119,119,46,105,110,107,115,99,97,112,101,46,111,114,103,47,110,97,109,101,115,112,97,99,101,115,47,105,110,107,115,99,97,112,101,34,10,32,32,32,
119,105,100,116,104,61,34,49,48,48,109,109,34,10,32,32,32,104,101,105,103,104,116,61,34,49,48,48,109,109,34,10,32,32,32,118,105,101,119,66,111,120,61,34,48,32,48,32,49,48,48,32,49,48,48,34,10,32,32,32,
118,101,114,115,105,111,110,61,34,49,46,49,34,10,32,32,32,105,100,61,34,115,118,103,56,34,10,32,32,32,115,111,100,105,112,111,100,105,58,100,111,99,110,97,109,101,61,34,108,111,103,111,107,114,101,105,
115,46,115,118,103,34,10,32,32,32,105,110,107,115,99,97,112,101,58,118,101,114,115,105,111,110,61,34,48,46,57,50,46,49,32,114,49,53,51,55,49,34,62,10,32,32,60,100,101,102,115,10,32,32,32,32,32,105,100,
61,34,100,101,102,115,50,34,62,10,32,32,32,32,60,102,105,108,116,101,114,10,32,32,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,111,108,108,101,99,116,61,34,97,108,119,97,121,115,34,10,32,32,32,32,
32,32,32,115,116,121,108,101,61,34,99,111,108,111,114,45,105,110,116,101,114,112,111,108,97,116,105,111,110,45,102,105,108,116,101,114,115,58,115,82,71,66,34,10,32,32,32,32,32,32,32,105,100,61,34,102,
105,108,116,101,114,53,51,55,52,34,10,32,32,32,32,32,32,32,120,61,34,45,48,46,48,48,54,48,48,56,53,51,34,10,32,32,32,32,32,32,32,119,105,100,116,104,61,34,49,46,48,49,50,48,49,55,49,34,10,32,32,32,32,
32,32,32,121,61,34,45,48,46,48,48,53,57,57,49,52,57,52,50,34,10,32,32,32,32,32,32,32,104,101,105,103,104,116,61,34,49,46,48,49,49,57,56,51,34,62,10,32,32,32,32,32,32,60,102,101,71,97,117,115,115,105,97,
110,66,108,117,114,10,32,32,32,32,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,111,108,108,101,99,116,61,34,97,108,119,97,121,115,34,10,32,32,32,32,32,32,32,32,32,115,116,100,68,101,118,105,97,116,
105,111,110,61,34,48,46,57,52,50,52,49,50,49,34,10,32,32,32,32,32,32,32,32,32,105,100,61,34,102,101,71,97,117,115,115,105,97,110,66,108,117,114,53,51,55,54,34,32,47,62,10,32,32,32,32,60,47,102,105,108,
116,101,114,62,10,32,32,60,47,100,101,102,115,62,10,32,32,60,115,111,100,105,112,111,100,105,58,110,97,109,101,100,118,105,101,119,10,32,32,32,32,32,105,100,61,34,98,97,115,101,34,10,32,32,32,32,32,112,
97,103,101,99,111,108,111,114,61,34,35,102,102,102,102,102,102,34,10,32,32,32,32,32,98,111,114,100,101,114,99,111,108,111,114,61,34,35,54,54,54,54,54,54,34,10,32,32,32,32,32,98,111,114,100,101,114,111,
112,97,99,105,116,121,61,34,49,46,48,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,112,97,103,101,111,112,97,99,105,116,121,61,34,48,46,48,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,
112,97,103,101,115,104,97,100,111,119,61,34,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,122,111,111,109,61,34,49,46,57,55,57,56,57,57,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,
99,120,61,34,54,53,46,55,57,53,55,57,53,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,121,61,34,49,52,55,46,55,54,51,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,100,111,99,117,
109,101,110,116,45,117,110,105,116,115,61,34,109,109,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,117,114,114,101,110,116,45,108,97,121,101,114,61,34,108,97,121,101,114,50,34,10,32,32,32,32,
32,115,104,111,119,103,114,105,100,61,34,102,97,108,115,101,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,119,105,100,116,104,61,34,50,48,52,56,34,10,32,32,32,32,32,
105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,104,101,105,103,104,116,61,34,49,48,57,57,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,120,61,34,50,48,52,
48,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,121,61,34,45,56,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,109,97,120,105,109,
105,122,101,100,61,34,49,34,32,47,62,10,32,32,60,109,101,116,97,100,97,116,97,10,32,32,32,32,32,105,100,61,34,109,101,116,97,100,97,116,97,53,34,62,10,32,32,32,32,60,114,100,102,58,82,68,70,62,10,32,32,
32,32,32,32,60,99,99,58,87,111,114,107,10,32,32,32,32,32,32,32,32,32,114,100,102,58,97,98,111,117,116,61,34,34,62,10,32,32,32,32,32,32,32,32,60,100,99,58,102,111,114,109,97,116,62,105,109,97,103,101,47,
115,118,103,43,120,109,108,60,47,100,99,58,102,111,114,109,97,116,62,10,32,32,32,32,32,32,32,32,60,100,99,58,116,121,112,101,10,32,32,32,32,32,32,32,32,32,32,32,114,100,102,58,114,101,115,111,117,114,
99,101,61,34,104,116,116,112,58,47,47,112,117,114,108,46,111,114,103,47,100,99,47,100,99,109,105,116,121,112,101,47,83,116,105,108,108,73,109,97,103,101,34,32,47,62,10,32,32,32,32,32,32,32,32,60,100,99,
58,116,105,116,108,101,62,60,47,100,99,58,116,105,116,108,101,62,10,32,32,32,32,32,32,60,47,99,99,58,87,111,114,107,62,10,32,32,32,32,60,47,114,100,102,58,82,68,70,62,10,32,32,60,47,109,101,116,97,100,
97,116,97,62,10,32,32,60,103,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,108,97,98,101,108,61,34,69,98,101,110,101,32,49,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,103,114,111,117,112,
109,111,100,101,61,34,108,97,121,101,114,34,10,32,32,32,32,32,105,100,61,34,108,97,121,101,114,49,34,10,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,48,
44,45,49,57,55,41,34,32,47,62,10,32,32,60,103,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,103,114,111,117,112,109,111,100,101,61,34,108,97,121,101,114,34,10,32,32,32,32,32,105,100,61,34,108,97,
121,101,114,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,108,97,98,101,108,61,34,69,98,101,110,101,32,50,34,10,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,
97,116,101,40,48,44,45,49,57,55,41,34,62,10,32,32,32,32,60,112,97,116,104,10,32,32,32,32,32,32,32,115,116,121,108,101,61,34,111,112,97,99,105,116,121,58,49,59,102,105,108,108,58,35,97,51,98,102,97,56,
59,102,105,108,108,45,111,112,97,99,105,116,121,58,49,59,115,116,114,111,107,101,45,119,105,100,116,104,58,49,46,49,51,55,51,57,48,57,55,59,115,116,114,111,107,101,45,108,105,110,101,99,97,112,58,115,
113,117,97,114,101,59,115,116,114,111,107,101,45,108,105,110,101,106,111,105,110,58,98,101,118,101,108,59,102,105,108,116,101,114,58,117,114,108,40,35,102,105,108,116,101,114,53,51,55,52,41,34,10,32,32,
32,32,32,32,32,100,61,34,77,32,49,56,56,46,51,57,50,53,56,32,48,46,48,57,53,55,48,51,49,50,53,32,65,32,49,56,56,46,50,49,52,50,55,32,49,56,56,46,55,52,57,57,57,32,48,32,48,32,48,32,48,46,49,55,55,55,51,
52,51,56,32,49,56,56,46,56,52,53,55,32,65,32,49,56,56,46,50,49,52,50,55,32,49,56,56,46,55,52,57,57,57,32,48,32,48,32,48,32,49,56,56,46,51,57,50,53,56,32,51,55,55,46,53,57,53,55,32,65,32,49,56,56,46,50,
49,52,50,55,32,49,56,56,46,55,52,57,57,57,32,48,32,48,32,48,32,51,55,54,46,54,48,55,52,50,32,49,56,56,46,56,52,53,55,32,65,32,49,56,56,46,50,49,52,50,55,32,49,56,56,46,55,52,57,57,57,32,48,32,48,32,48,
32,49,56,56,46,51,57,50,53,56,32,48,46,48,57,53,55,48,51,49,50,53,32,122,32,77,32,49,48,48,46,49,52,48,54,50,32,55,48,46,54,55,51,56,50,56,32,76,32,49,56,48,46,56,49,48,53,53,32,55,48,46,54,55,51,56,50,
56,32,67,32,49,57,56,46,57,51,54,54,53,32,55,48,46,54,55,51,56,50,56,32,50,49,52,46,53,55,52,51,32,55,50,46,51,54,49,55,50,57,32,50,50,55,46,55,50,48,55,32,55,53,46,55,51,54,51,50,56,32,67,32,50,52,48,
46,56,54,55,49,49,32,55,57,46,48,48,50,48,54,57,32,50,53,49,46,57,50,49,51,49,32,56,51,46,54,56,49,50,57,53,32,50,54,48,46,56,56,52,55,55,32,56,57,46,55,55,55,51,52,52,32,67,32,50,55,54,46,50,50,50,50,
52,32,49,48,48,46,48,49,32,50,56,56,46,50,55,50,56,52,32,49,49,51,46,56,51,54,54,50,32,50,57,55,46,48,51,55,49,49,32,49,51,49,46,50,53,51,57,49,32,67,32,51,48,53,46,57,48,48,57,55,32,49,52,56,46,53,54,
50,51,51,32,51,49,48,46,51,51,51,57,56,32,49,54,56,46,57,55,51,32,51,49,48,46,51,51,51,57,56,32,49,57,50,46,52,56,54,51,51,32,67,32,51,49,48,46,51,51,51,57,56,32,50,49,53,46,49,50,56,56,32,51,48,53,46,
54,48,50,48,53,32,50,51,53,46,52,51,49,32,50,57,54,46,49,52,48,54,50,32,50,53,51,46,51,57,50,53,56,32,67,32,50,56,54,46,54,55,57,50,32,50,55,49,46,50,52,53,50,57,32,50,55,52,46,55,50,56,50,52,32,50,56,
52,46,57,54,49,53,49,32,50,54,48,46,50,56,55,49,49,32,50,57,52,46,53,52,49,48,50,32,67,32,50,52,57,46,52,51,49,51,55,32,51,48,49,46,55,50,53,54,52,32,50,51,55,46,53,50,57,50,53,32,51,48,54,46,55,51,50,
49,57,32,50,50,52,46,53,56,50,48,51,32,51,48,57,46,53,54,50,53,32,67,32,50,49,49,46,54,51,52,56,50,32,51,49,50,46,51,57,50,56,49,32,49,57,54,46,50,57,56,48,52,32,51,49,51,46,56,48,56,53,57,32,49,55,56,
46,53,55,48,51,49,32,51,49,51,46,56,48,56,53,57,32,76,32,49,48,48,46,49,52,48,54,50,32,51,49,51,46,56,48,56,53,57,32,76,32,49,48,48,46,49,52,48,54,50,32,55,48,46,54,55,51,56,50,56,32,122,32,77,32,49,53,
55,46,53,48,53,56,54,32,49,49,54,46,48,54,56,51,54,32,76,32,49,53,55,46,53,48,53,56,54,32,50,54,56,46,50,53,32,76,32,49,55,49,46,57,57,54,48,57,32,50,54,56,46,50,53,32,67,32,49,56,53,46,54,52,48,52,55,
32,50,54,56,46,50,53,32,49,57,53,46,54,53,49,51,56,32,50,54,55,46,53,57,55,51,49,32,50,48,50,46,48,50,53,51,57,32,50,54,54,46,50,57,49,48,50,32,67,32,50,48,56,46,51,57,57,52,32,50,54,52,46,56,55,53,56,
54,32,50,49,52,46,54,50,51,57,57,32,50,54,50,46,51,55,51,53,54,32,50,50,48,46,54,57,57,50,50,32,50,53,56,46,55,56,49,50,53,32,67,32,50,51,49,46,49,53,54,53,57,32,50,53,50,46,50,52,57,55,55,32,50,51,56,
46,56,50,52,57,55,32,50,52,51,46,53,52,48,49,32,50,52,51,46,55,48,53,48,56,32,50,51,50,46,54,53,52,51,32,67,32,50,52,56,46,53,56,53,49,56,32,50,50,49,46,54,53,57,54,52,32,50,53,49,46,48,50,53,51,57,32,
50,48,56,46,49,48,55,48,56,32,50,53,49,46,48,50,53,51,57,32,49,57,49,46,57,57,54,48,57,32,67,32,50,53,49,46,48,50,53,51,57,32,49,55,53,46,57,57,51,57,54,32,50,52,56,46,51,51,53,49,49,32,49,54,50,46,51,
51,50,57,53,32,50,52,50,46,57,53,55,48,51,32,49,53,49,46,48,49,49,55,50,32,67,32,50,51,55,46,54,55,56,53,53,32,49,51,57,46,53,56,49,54,51,32,50,50,57,46,50,54,51,56,52,32,49,51,48,46,54,53,53,48,53,32,
50,49,55,46,55,49,48,57,52,32,49,50,52,46,50,51,50,52,50,32,67,32,50,49,49,46,56,51,52,56,57,32,49,50,49,46,48,55,53,53,52,32,50,48,53,46,56,48,57,53,57,32,49,49,56,46,57,53,49,56,54,32,49,57,57,46,54,
51,52,55,55,32,49,49,55,46,56,54,51,50,56,32,67,32,49,57,51,46,53,53,57,53,51,32,49,49,54,46,54,54,53,56,52,32,49,56,52,46,51,52,53,55,53,32,49,49,54,46,48,54,56,51,54,32,49,55,49,46,57,57,54,48,57,32,
49,49,54,46,48,54,56,51,54,32,76,32,49,53,55,46,53,48,53,56,54,32,49,49,54,46,48,54,56,51,54,32,122,32,34,10,32,32,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,109,97,116,114,105,120,40,48,
46,50,54,52,53,56,51,51,51,44,48,44,48,44,48,46,50,54,52,53,56,51,51,51,44,48,44,49,57,55,41,34,10,32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,49,51,51,34,32,47,62,10,32,32,60,47,103,62,10,60,47,
115,118,103,62,10,0,0};

const char* VASTVaporizerComponent::logokreis_svg = (const char*) resource_VASTVaporizerComponent_logokreis_svg;
const int VASTVaporizerComponent::logokreis_svgSize = 3721;

// JUCER_RESOURCE: logokreisv_svg, 2555, "../../Resources/logokreis-v.svg"
static const unsigned char resource_VASTVaporizerComponent_logokreisv_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,70,45,56,
34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,33,45,45,32,67,114,101,97,116,101,100,32,119,105,116,104,32,73,110,107,115,99,97,112,101,32,40,104,116,116,112,58,47,47,119,119,
119,46,105,110,107,115,99,97,112,101,46,111,114,103,47,41,32,45,45,62,10,10,60,115,118,103,10,32,32,32,120,109,108,110,115,58,100,99,61,34,104,116,116,112,58,47,47,112,117,114,108,46,111,114,103,47,100,
99,47,101,108,101,109,101,110,116,115,47,49,46,49,47,34,10,32,32,32,120,109,108,110,115,58,99,99,61,34,104,116,116,112,58,47,47,99,114,101,97,116,105,118,101,99,111,109,109,111,110,115,46,111,114,103,
47,110,115,35,34,10,32,32,32,120,109,108,110,115,58,114,100,102,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,48,50,47,50,50,45,114,100,102,45,115,121,110,116,97,
120,45,110,115,35,34,10,32,32,32,120,109,108,110,115,58,115,118,103,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,118,103,34,10,32,32,32,120,109,108,110,115,
61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,118,103,34,10,32,32,32,120,109,108,110,115,58,115,111,100,105,112,111,100,105,61,34,104,116,116,112,58,47,47,115,
111,100,105,112,111,100,105,46,115,111,117,114,99,101,102,111,114,103,101,46,110,101,116,47,68,84,68,47,115,111,100,105,112,111,100,105,45,48,46,100,116,100,34,10,32,32,32,120,109,108,110,115,58,105,110,
107,115,99,97,112,101,61,34,104,116,116,112,58,47,47,119,119,119,46,105,110,107,115,99,97,112,101,46,111,114,103,47,110,97,109,101,115,112,97,99,101,115,47,105,110,107,115,99,97,112,101,34,10,32,32,32,
119,105,100,116,104,61,34,49,48,48,109,109,34,10,32,32,32,104,101,105,103,104,116,61,34,49,48,48,109,109,34,10,32,32,32,118,105,101,119,66,111,120,61,34,48,32,48,32,49,48,48,32,49,48,48,34,10,32,32,32,
118,101,114,115,105,111,110,61,34,49,46,49,34,10,32,32,32,105,100,61,34,115,118,103,56,34,10,32,32,32,115,111,100,105,112,111,100,105,58,100,111,99,110,97,109,101,61,34,108,111,103,111,107,114,101,105,
115,45,118,46,115,118,103,34,10,32,32,32,105,110,107,115,99,97,112,101,58,118,101,114,115,105,111,110,61,34,48,46,57,50,46,49,32,114,49,53,51,55,49,34,62,10,32,32,60,100,101,102,115,10,32,32,32,32,32,
105,100,61,34,100,101,102,115,50,34,32,47,62,10,32,32,60,115,111,100,105,112,111,100,105,58,110,97,109,101,100,118,105,101,119,10,32,32,32,32,32,105,100,61,34,98,97,115,101,34,10,32,32,32,32,32,112,97,
103,101,99,111,108,111,114,61,34,35,102,102,102,102,102,102,34,10,32,32,32,32,32,98,111,114,100,101,114,99,111,108,111,114,61,34,35,54,54,54,54,54,54,34,10,32,32,32,32,32,98,111,114,100,101,114,111,112,
97,99,105,116,121,61,34,49,46,48,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,112,97,103,101,111,112,97,99,105,116,121,61,34,48,46,48,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,112,
97,103,101,115,104,97,100,111,119,61,34,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,122,111,111,109,61,34,49,46,57,55,57,56,57,57,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,
120,61,34,49,55,54,46,49,53,52,57,54,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,121,61,34,49,55,55,46,48,53,55,54,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,100,111,99,117,
109,101,110,116,45,117,110,105,116,115,61,34,109,109,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,117,114,114,101,110,116,45,108,97,121,101,114,61,34,108,97,121,101,114,50,34,10,32,32,32,32,
32,115,104,111,119,103,114,105,100,61,34,102,97,108,115,101,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,119,105,100,116,104,61,34,50,48,52,56,34,10,32,32,32,32,32,
105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,104,101,105,103,104,116,61,34,49,48,57,57,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,120,61,34,50,48,52,
48,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,121,61,34,45,56,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,109,97,120,105,109,
105,122,101,100,61,34,49,34,32,47,62,10,32,32,60,109,101,116,97,100,97,116,97,10,32,32,32,32,32,105,100,61,34,109,101,116,97,100,97,116,97,53,34,62,10,32,32,32,32,60,114,100,102,58,82,68,70,62,10,32,32,
32,32,32,32,60,99,99,58,87,111,114,107,10,32,32,32,32,32,32,32,32,32,114,100,102,58,97,98,111,117,116,61,34,34,62,10,32,32,32,32,32,32,32,32,60,100,99,58,102,111,114,109,97,116,62,105,109,97,103,101,47,
115,118,103,43,120,109,108,60,47,100,99,58,102,111,114,109,97,116,62,10,32,32,32,32,32,32,32,32,60,100,99,58,116,121,112,101,10,32,32,32,32,32,32,32,32,32,32,32,114,100,102,58,114,101,115,111,117,114,
99,101,61,34,104,116,116,112,58,47,47,112,117,114,108,46,111,114,103,47,100,99,47,100,99,109,105,116,121,112,101,47,83,116,105,108,108,73,109,97,103,101,34,32,47,62,10,32,32,32,32,32,32,32,32,60,100,99,
58,116,105,116,108,101,62,60,47,100,99,58,116,105,116,108,101,62,10,32,32,32,32,32,32,60,47,99,99,58,87,111,114,107,62,10,32,32,32,32,60,47,114,100,102,58,82,68,70,62,10,32,32,60,47,109,101,116,97,100,
97,116,97,62,10,32,32,60,103,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,108,97,98,101,108,61,34,69,98,101,110,101,32,49,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,103,114,111,117,112,
109,111,100,101,61,34,108,97,121,101,114,34,10,32,32,32,32,32,105,100,61,34,108,97,121,101,114,49,34,10,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,48,
44,45,49,57,55,41,34,32,47,62,10,32,32,60,103,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,103,114,111,117,112,109,111,100,101,61,34,108,97,121,101,114,34,10,32,32,32,32,32,105,100,61,34,108,97,
121,101,114,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,108,97,98,101,108,61,34,69,98,101,110,101,32,50,34,10,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,
97,116,101,40,48,44,45,49,57,55,41,34,62,10,32,32,32,32,60,103,10,32,32,32,32,32,32,32,97,114,105,97,45,108,97,98,101,108,61,34,86,34,10,32,32,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,115,
99,97,108,101,40,48,46,57,51,57,53,51,53,53,52,44,49,46,48,54,52,51,53,53,55,41,34,10,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,111,110,116,45,115,116,121,108,101,58,110,111,114,109,97,108,59,
102,111,110,116,45,119,101,105,103,104,116,58,110,111,114,109,97,108,59,102,111,110,116,45,115,105,122,101,58,50,49,46,51,49,50,49,57,54,55,51,112,120,59,108,105,110,101,45,104,101,105,103,104,116,58,
49,50,53,37,59,102,111,110,116,45,102,97,109,105,108,121,58,83,97,110,115,59,108,101,116,116,101,114,45,115,112,97,99,105,110,103,58,48,112,120,59,119,111,114,100,45,115,112,97,99,105,110,103,58,48,112,
120,59,102,105,108,108,58,35,102,57,102,57,102,57,59,102,105,108,108,45,111,112,97,99,105,116,121,58,48,46,56,50,54,52,49,53,49,50,59,115,116,114,111,107,101,58,110,111,110,101,59,115,116,114,111,107,
101,45,119,105,100,116,104,58,48,46,53,51,50,56,48,52,57,49,112,120,59,115,116,114,111,107,101,45,108,105,110,101,99,97,112,58,98,117,116,116,59,115,116,114,111,107,101,45,108,105,110,101,106,111,105,
110,58,109,105,116,101,114,59,115,116,114,111,107,101,45,111,112,97,99,105,116,121,58,49,34,10,32,32,32,32,32,32,32,105,100,61,34,116,101,120,116,52,54,49,57,34,62,10,32,32,32,32,32,32,60,112,97,116,104,
10,32,32,32,32,32,32,32,32,32,100,61,34,109,32,57,48,46,55,55,51,56,51,57,44,50,48,48,46,49,55,52,56,53,32,45,50,55,46,49,50,51,49,44,55,52,46,51,55,54,50,52,32,72,32,52,50,46,49,55,50,48,52,32,76,32,
49,53,46,48,52,56,57,51,57,44,50,48,48,46,49,55,52,56,53,32,104,32,50,48,46,49,51,48,48,51,54,32,108,32,49,55,46,57,56,50,49,54,55,44,53,50,46,50,52,56,49,56,32,49,55,46,57,56,50,49,54,54,44,45,53,50,
46,50,52,56,49,56,32,122,34,10,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,111,110,116,45,115,116,121,108,101,58,110,111,114,109,97,108,59,102,111,110,116,45,118,97,114,105,97,110,116,58,
110,111,114,109,97,108,59,102,111,110,116,45,119,101,105,103,104,116,58,98,111,108,100,59,102,111,110,116,45,115,116,114,101,116,99,104,58,110,111,114,109,97,108,59,102,111,110,116,45,115,105,122,101,
58,49,48,50,46,50,57,56,53,52,53,56,52,112,120,59,102,111,110,116,45,102,97,109,105,108,121,58,86,101,114,100,97,110,97,59,45,105,110,107,115,99,97,112,101,45,102,111,110,116,45,115,112,101,99,105,102,
105,99,97,116,105,111,110,58,39,86,101,114,100,97,110,97,32,66,111,108,100,39,59,102,105,108,108,58,35,102,57,102,57,102,57,59,102,105,108,108,45,111,112,97,99,105,116,121,58,48,46,56,50,54,52,49,53,49,
50,59,115,116,114,111,107,101,45,119,105,100,116,104,58,48,46,53,51,50,56,48,52,57,49,112,120,34,10,32,32,32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,52,54,54,53,34,32,47,62,10,32,32,32,32,60,47,
103,62,10,32,32,60,47,103,62,10,60,47,115,118,103,62,10,0,0};

const char* VASTVaporizerComponent::logokreisv_svg = (const char*) resource_VASTVaporizerComponent_logokreisv_svg;
const int VASTVaporizerComponent::logokreisv_svgSize = 2555;

// JUCER_RESOURCE: dynamics_svg, 13088, "../../Resources/dynamics.svg"
static const unsigned char resource_VASTVaporizerComponent_dynamics_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,70,45,56,
34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,33,45,45,32,67,114,101,97,116,101,100,32,119,105,116,104,32,73,110,107,115,99,97,112,101,32,40,104,116,116,112,58,47,47,119,119,
119,46,105,110,107,115,99,97,112,101,46,111,114,103,47,41,32,45,45,62,10,10,60,115,118,103,10,32,32,32,120,109,108,110,115,58,100,99,61,34,104,116,116,112,58,47,47,112,117,114,108,46,111,114,103,47,100,
99,47,101,108,101,109,101,110,116,115,47,49,46,49,47,34,10,32,32,32,120,109,108,110,115,58,99,99,61,34,104,116,116,112,58,47,47,99,114,101,97,116,105,118,101,99,111,109,109,111,110,115,46,111,114,103,
47,110,115,35,34,10,32,32,32,120,109,108,110,115,58,114,100,102,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,48,50,47,50,50,45,114,100,102,45,115,121,110,116,97,
120,45,110,115,35,34,10,32,32,32,120,109,108,110,115,58,115,118,103,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,118,103,34,10,32,32,32,120,109,108,110,115,
61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,118,103,34,10,32,32,32,120,109,108,110,115,58,115,111,100,105,112,111,100,105,61,34,104,116,116,112,58,47,47,115,
111,100,105,112,111,100,105,46,115,111,117,114,99,101,102,111,114,103,101,46,110,101,116,47,68,84,68,47,115,111,100,105,112,111,100,105,45,48,46,100,116,100,34,10,32,32,32,120,109,108,110,115,58,105,110,
107,115,99,97,112,101,61,34,104,116,116,112,58,47,47,119,119,119,46,105,110,107,115,99,97,112,101,46,111,114,103,47,110,97,109,101,115,112,97,99,101,115,47,105,110,107,115,99,97,112,101,34,10,32,32,32,
119,105,100,116,104,61,34,50,49,48,109,109,34,10,32,32,32,104,101,105,103,104,116,61,34,50,57,55,109,109,34,10,32,32,32,105,100,61,34,115,118,103,51,48,50,52,34,10,32,32,32,118,101,114,115,105,111,110,
61,34,49,46,49,34,10,32,32,32,105,110,107,115,99,97,112,101,58,118,101,114,115,105,111,110,61,34,48,46,57,50,46,51,32,40,50,52,48,53,53,52,54,44,32,50,48,49,56,45,48,51,45,49,49,41,34,10,32,32,32,115,
111,100,105,112,111,100,105,58,100,111,99,110,97,109,101,61,34,100,121,110,97,109,105,99,115,46,115,118,103,34,62,10,32,32,60,100,101,102,115,10,32,32,32,32,32,105,100,61,34,100,101,102,115,51,48,50,54,
34,32,47,62,10,32,32,60,115,111,100,105,112,111,100,105,58,110,97,109,101,100,118,105,101,119,10,32,32,32,32,32,105,100,61,34,98,97,115,101,34,10,32,32,32,32,32,112,97,103,101,99,111,108,111,114,61,34,
35,102,102,102,102,102,102,34,10,32,32,32,32,32,98,111,114,100,101,114,99,111,108,111,114,61,34,35,54,54,54,54,54,54,34,10,32,32,32,32,32,98,111,114,100,101,114,111,112,97,99,105,116,121,61,34,49,46,48,
34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,112,97,103,101,111,112,97,99,105,116,121,61,34,48,46,48,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,112,97,103,101,115,104,97,100,111,119,
61,34,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,122,111,111,109,61,34,55,46,57,49,57,53,57,53,57,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,120,61,34,50,53,55,46,57,48,48,
49,57,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,121,61,34,53,53,56,46,54,53,52,48,56,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,100,111,99,117,109,101,110,116,45,117,110,105,
116,115,61,34,112,120,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,117,114,114,101,110,116,45,108,97,121,101,114,61,34,108,97,121,101,114,49,34,10,32,32,32,32,32,115,104,111,119,103,114,105,
100,61,34,102,97,108,115,101,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,119,105,100,116,104,61,34,53,52,51,49,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,
119,105,110,100,111,119,45,104,101,105,103,104,116,61,34,50,56,54,56,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,120,61,34,45,49,56,34,10,32,32,32,32,32,105,110,107,
115,99,97,112,101,58,119,105,110,100,111,119,45,121,61,34,45,49,56,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,109,97,120,105,109,105,122,101,100,61,34,49,34,32,47,
62,10,32,32,60,109,101,116,97,100,97,116,97,10,32,32,32,32,32,105,100,61,34,109,101,116,97,100,97,116,97,51,48,50,57,34,62,10,32,32,32,32,60,114,100,102,58,82,68,70,62,10,32,32,32,32,32,32,60,99,99,58,
87,111,114,107,10,32,32,32,32,32,32,32,32,32,114,100,102,58,97,98,111,117,116,61,34,34,62,10,32,32,32,32,32,32,32,32,60,100,99,58,102,111,114,109,97,116,62,105,109,97,103,101,47,115,118,103,43,120,109,
108,60,47,100,99,58,102,111,114,109,97,116,62,10,32,32,32,32,32,32,32,32,60,100,99,58,116,121,112,101,10,32,32,32,32,32,32,32,32,32,32,32,114,100,102,58,114,101,115,111,117,114,99,101,61,34,104,116,116,
112,58,47,47,112,117,114,108,46,111,114,103,47,100,99,47,100,99,109,105,116,121,112,101,47,83,116,105,108,108,73,109,97,103,101,34,32,47,62,10,32,32,32,32,32,32,32,32,60,100,99,58,116,105,116,108,101,
62,60,47,100,99,58,116,105,116,108,101,62,10,32,32,32,32,32,32,60,47,99,99,58,87,111,114,107,62,10,32,32,32,32,60,47,114,100,102,58,82,68,70,62,10,32,32,60,47,109,101,116,97,100,97,116,97,62,10,32,32,
60,103,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,108,97,98,101,108,61,34,69,98,101,110,101,32,49,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,103,114,111,117,112,109,111,100,101,61,34,
108,97,121,101,114,34,10,32,32,32,32,32,105,100,61,34,108,97,121,101,114,49,34,62,10,32,32,32,32,60,103,10,32,32,32,32,32,32,32,97,114,105,97,45,108,97,98,101,108,61,34,100,121,110,97,109,105,99,115,34,
10,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,111,110,116,45,115,116,121,108,101,58,111,98,108,105,113,117,101,59,102,111,110,116,45,118,97,114,105,97,110,116,58,110,111,114,109,97,108,59,102,
111,110,116,45,119,101,105,103,104,116,58,110,111,114,109,97,108,59,102,111,110,116,45,115,116,114,101,116,99,104,58,110,111,114,109,97,108,59,108,105,110,101,45,104,101,105,103,104,116,58,48,37,59,102,
111,110,116,45,102,97,109,105,108,121,58,39,80,97,108,97,99,101,32,83,99,114,105,112,116,32,77,84,39,59,45,105,110,107,115,99,97,112,101,45,102,111,110,116,45,115,112,101,99,105,102,105,99,97,116,105,
111,110,58,39,80,97,108,97,99,101,32,83,99,114,105,112,116,32,77,84,32,79,98,108,105,113,117,101,39,59,116,101,120,116,45,97,108,105,103,110,58,115,116,97,114,116,59,108,101,116,116,101,114,45,115,112,
97,99,105,110,103,58,48,112,120,59,119,111,114,100,45,115,112,97,99,105,110,103,58,48,112,120,59,119,114,105,116,105,110,103,45,109,111,100,101,58,108,114,45,116,98,59,116,101,120,116,45,97,110,99,104,
111,114,58,115,116,97,114,116,59,102,105,108,108,58,35,102,57,102,57,102,57,59,102,105,108,108,45,111,112,97,99,105,116,121,58,49,59,115,116,114,111,107,101,58,110,111,110,101,34,10,32,32,32,32,32,32,
32,105,100,61,34,116,101,120,116,51,48,51,50,34,62,10,32,32,32,32,32,32,60,112,97,116,104,10,32,32,32,32,32,32,32,32,32,100,61,34,109,32,50,50,52,46,56,56,48,50,56,44,53,51,56,46,53,56,57,56,52,32,113,
32,45,49,46,54,48,49,53,54,44,48,32,45,52,46,53,55,48,51,49,44,50,46,50,54,53,54,51,32,45,50,46,57,54,56,55,53,44,50,46,50,50,54,53,54,32,45,53,46,50,55,51,52,52,44,53,46,48,55,56,49,50,32,45,50,46,50,
54,53,54,51,44,50,46,56,49,50,53,32,45,50,46,50,54,53,54,51,44,52,46,52,49,52,48,55,32,48,44,49,46,50,56,57,48,54,32,49,46,50,49,48,57,52,44,49,46,50,56,57,48,54,32,49,46,55,49,56,55,53,44,48,32,52,46,
55,50,54,53,54,44,45,50,46,50,50,54,53,54,32,51,46,48,52,54,56,56,44,45,50,46,50,54,53,54,51,32,53,46,51,57,48,54,51,44,45,53,46,48,55,56,49,51,32,50,46,51,56,50,56,49,44,45,50,46,56,49,50,53,32,50,46,
51,56,50,56,49,44,45,52,46,50,57,54,56,55,32,48,44,45,49,46,52,52,53,51,50,32,45,49,46,54,48,49,53,54,44,45,49,46,52,52,53,51,50,32,122,32,109,32,49,46,54,55,57,54,57,44,51,46,50,48,51,49,51,32,50,49,
46,48,57,51,55,53,44,45,50,52,46,49,48,49,53,54,32,104,32,51,46,50,48,51,49,50,32,108,32,45,50,52,46,54,48,57,51,55,44,50,56,46,48,56,53,57,51,32,113,32,45,48,46,56,50,48,51,50,44,48,46,57,51,55,53,32,
45,49,46,55,57,54,56,56,44,50,46,49,48,57,51,56,32,45,48,46,57,55,54,53,54,44,49,46,49,55,49,56,55,32,45,49,46,52,52,53,51,49,44,49,46,56,51,53,57,52,32,45,48,46,52,50,57,54,57,44,48,46,54,54,52,48,54,
32,45,48,46,54,50,53,44,49,46,50,56,57,48,54,32,45,48,46,49,53,54,50,53,44,48,46,54,50,53,32,48,46,53,56,53,57,52,44,48,46,54,50,53,32,48,46,57,55,54,53,54,44,48,32,52,46,56,48,52,54,56,44,45,51,46,48,
48,55,56,49,32,51,46,56,50,56,49,51,44,45,51,46,48,52,54,56,56,32,55,46,49,56,55,53,44,45,54,46,48,53,52,54,57,32,118,32,48,46,54,54,52,48,54,32,113,32,45,52,46,54,48,57,51,55,44,52,46,48,50,51,52,52,
32,45,55,46,55,55,51,52,51,44,54,46,52,56,52,51,56,32,45,51,46,49,50,53,44,50,46,52,54,48,57,51,32,45,52,46,53,55,48,51,50,44,50,46,52,54,48,57,51,32,45,49,46,48,49,53,54,50,44,48,32,45,49,46,48,49,53,
54,50,44,45,49,46,50,49,48,57,51,32,48,44,45,49,46,53,54,50,53,32,49,46,48,57,51,55,53,44,45,52,46,48,54,50,53,32,48,44,45,48,46,48,51,57,49,32,45,48,46,48,55,56,49,44,45,48,46,48,55,56,49,32,45,48,46,
48,51,57,49,44,45,48,46,48,55,56,49,32,45,48,46,48,55,56,49,44,45,48,46,48,55,56,49,32,45,53,46,52,50,57,54,57,44,53,46,52,50,57,54,56,32,45,56,46,53,53,52,54,57,44,53,46,52,50,57,54,56,32,45,50,46,50,
50,54,53,54,44,48,32,45,50,46,50,50,54,53,54,44,45,51,46,54,55,49,56,55,32,48,44,45,50,46,56,57,48,54,51,32,50,46,49,56,55,53,44,45,53,46,51,49,50,53,32,50,46,49,56,55,53,44,45,50,46,52,50,49,56,56,32,
53,46,51,57,48,54,50,44,45,51,46,55,56,57,48,54,32,51,46,50,48,51,49,51,44,45,49,46,51,54,55,49,57,32,53,46,55,48,51,49,51,44,45,49,46,51,54,55,49,57,32,49,46,57,57,50,49,57,44,48,32,49,46,57,57,50,49,
57,44,49,46,54,55,57,54,57,32,48,44,49,46,49,55,49,56,55,32,45,48,46,52,54,56,55,53,44,50,46,48,55,48,51,49,32,122,34,10,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,111,110,116,45,115,116,
121,108,101,58,111,98,108,105,113,117,101,59,102,111,110,116,45,118,97,114,105,97,110,116,58,110,111,114,109,97,108,59,102,111,110,116,45,119,101,105,103,104,116,58,110,111,114,109,97,108,59,102,111,110,
116,45,115,116,114,101,116,99,104,58,110,111,114,109,97,108,59,102,111,110,116,45,115,105,122,101,58,56,48,112,120,59,108,105,110,101,45,104,101,105,103,104,116,58,49,50,53,37,59,102,111,110,116,45,102,
97,109,105,108,121,58,39,80,97,108,97,99,101,32,83,99,114,105,112,116,32,77,84,39,59,45,105,110,107,115,99,97,112,101,45,102,111,110,116,45,115,112,101,99,105,102,105,99,97,116,105,111,110,58,39,80,97,
108,97,99,101,32,83,99,114,105,112,116,32,77,84,32,79,98,108,105,113,117,101,39,59,116,101,120,116,45,97,108,105,103,110,58,115,116,97,114,116,59,119,114,105,116,105,110,103,45,109,111,100,101,58,108,
114,45,116,98,59,116,101,120,116,45,97,110,99,104,111,114,58,115,116,97,114,116,59,102,105,108,108,58,35,102,57,102,57,102,57,34,10,32,32,32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,51,55,51,53,
34,32,47,62,10,32,32,32,32,32,32,60,112,97,116,104,10,32,32,32,32,32,32,32,32,32,100,61,34,109,32,50,51,57,46,57,53,56,52,44,53,53,53,46,53,52,50,57,55,32,113,32,45,56,46,50,48,51,49,50,44,52,46,49,48,
49,53,54,32,45,49,51,46,56,54,55,49,56,44,56,46,50,56,49,50,53,32,45,53,46,55,48,51,49,51,44,52,46,49,55,57,54,57,32,45,53,46,55,48,51,49,51,44,54,46,57,57,50,49,57,32,48,44,48,46,55,48,51,49,50,32,48,
46,53,48,55,56,49,44,49,46,49,51,50,56,49,32,48,46,52,54,56,55,53,44,48,46,52,54,56,55,53,32,49,46,49,55,49,56,56,44,48,46,52,54,56,55,53,32,49,46,51,54,55,49,57,44,48,32,51,46,48,56,53,57,52,44,45,48,
46,57,55,54,53,54,32,49,46,55,49,56,55,53,44,45,48,46,57,51,55,53,32,51,46,56,54,55,49,56,44,45,50,46,57,54,56,55,53,32,50,46,49,48,57,51,56,44,45,50,46,48,51,49,50,53,32,52,46,57,54,48,57,52,44,45,53,
46,51,57,48,54,51,32,49,46,53,50,51,52,52,44,45,49,46,56,55,53,32,51,46,50,56,49,50,53,44,45,52,46,48,54,50,53,32,49,46,55,57,54,56,56,44,45,50,46,49,52,56,52,52,32,50,46,54,57,53,51,49,44,45,51,46,52,
55,54,53,54,32,122,32,109,32,45,53,46,57,55,54,53,54,44,45,49,49,46,51,54,55,49,57,32,118,32,45,48,46,54,50,53,32,113,32,51,46,49,50,53,44,45,50,46,54,57,53,51,49,32,53,46,49,49,55,49,57,44,45,52,46,49,
48,49,53,54,32,50,46,48,51,49,50,53,44,45,49,46,52,48,54,50,53,32,51,46,53,49,53,54,50,44,45,49,46,52,48,54,50,53,32,49,46,50,49,48,57,52,44,48,32,49,46,50,49,48,57,52,44,49,46,54,48,49,53,54,32,48,44,
51,46,51,57,56,52,52,32,45,52,46,54,52,56,52,52,44,55,46,53,32,45,51,46,51,53,57,51,55,44,50,46,57,54,56,55,53,32,45,51,46,51,53,57,51,55,44,51,46,57,48,54,50,53,32,48,44,48,46,53,56,53,57,52,32,48,46,
55,48,51,49,50,44,48,46,53,56,53,57,52,32,49,46,52,52,53,51,50,44,48,32,53,44,45,50,46,51,56,50,56,49,32,51,46,53,57,51,55,53,44,45,50,46,52,50,49,56,56,32,55,46,52,50,49,56,56,44,45,53,46,53,56,53,57,
52,32,51,46,56,54,55,49,57,44,45,51,46,50,48,51,49,51,32,53,46,57,55,54,53,54,44,45,53,46,51,49,50,53,32,104,32,50,46,57,54,56,55,53,32,108,32,45,49,50,46,53,51,57,48,54,44,49,52,46,50,57,54,56,55,32,
113,32,52,46,51,55,53,44,45,49,46,55,53,55,56,49,32,55,46,53,44,45,51,46,56,50,56,49,50,32,51,46,49,54,52,48,54,44,45,50,46,49,48,57,51,56,32,55,46,57,50,57,54,57,44,45,54,46,50,53,32,118,32,48,46,54,
54,52,48,54,32,113,32,45,52,46,52,53,51,49,51,44,52,46,49,48,49,53,54,32,45,55,46,56,49,50,53,44,54,46,50,56,57,48,54,32,45,51,46,51,53,57,51,56,44,50,46,49,56,55,53,32,45,56,46,53,49,53,54,51,44,52,46,
49,48,49,53,55,32,108,32,45,48,46,55,48,51,49,50,44,48,46,55,56,49,50,53,32,113,32,45,56,46,49,54,52,48,55,44,57,46,49,52,48,54,50,32,45,49,51,46,50,48,51,49,51,44,49,51,46,56,54,55,49,56,32,45,53,46,
48,51,57,48,54,44,52,46,55,50,54,53,55,32,45,56,46,51,50,48,51,49,44,52,46,55,50,54,53,55,32,45,50,46,52,50,49,56,56,44,48,32,45,50,46,52,50,49,56,56,44,45,50,46,50,54,53,54,51,32,48,44,45,49,46,54,52,
48,54,50,32,50,46,50,50,54,53,55,44,45,52,46,49,52,48,54,50,32,50,46,49,56,55,53,44,45,50,46,52,54,48,57,52,32,54,46,56,51,53,57,51,44,45,53,46,53,48,55,56,50,32,52,46,54,52,56,52,52,44,45,51,46,48,48,
55,56,49,32,49,49,46,56,51,53,57,52,44,45,54,46,51,50,56,49,50,32,51,46,56,50,56,49,51,44,45,52,46,57,50,49,56,56,32,57,46,57,50,49,56,56,44,45,49,49,46,54,48,49,53,54,32,45,52,46,49,52,48,54,51,44,51,
46,50,48,51,49,50,32,45,53,46,52,54,56,55,53,44,52,46,50,49,56,55,53,32,45,49,46,51,50,56,49,51,44,49,46,48,49,53,54,50,32,45,51,46,49,54,52,48,55,44,50,46,51,48,52,54,56,32,45,49,46,56,51,53,57,51,44,
49,46,50,53,32,45,51,46,50,56,49,50,53,44,49,46,56,55,53,32,45,49,46,52,48,54,50,53,44,48,46,54,50,53,32,45,50,46,53,44,48,46,54,50,53,32,45,48,46,54,54,52,48,54,44,48,32,45,49,46,48,57,51,55,53,44,45,
48,46,52,50,57,54,56,32,45,48,46,52,50,57,54,56,44,45,48,46,52,50,57,54,57,32,45,48,46,52,50,57,54,56,44,45,49,46,49,55,49,56,56,32,48,44,45,49,46,50,56,57,48,54,32,48,46,53,48,55,56,49,44,45,50,46,52,
54,48,57,52,32,48,46,53,48,55,56,49,44,45,49,46,49,55,49,56,55,32,49,46,54,48,49,53,54,44,45,50,46,51,52,51,55,53,32,49,46,49,51,50,56,49,44,45,49,46,50,49,48,57,51,32,51,46,48,48,55,56,49,44,45,50,46,
54,49,55,49,56,32,51,46,51,53,57,51,56,44,45,50,46,53,32,51,46,51,53,57,51,56,44,45,51,46,56,54,55,49,57,32,48,44,45,48,46,55,48,51,49,51,32,45,48,46,55,56,49,50,53,44,45,48,46,55,48,51,49,51,32,45,49,
46,51,50,56,49,51,44,48,32,45,51,46,52,51,55,53,44,49,46,54,48,49,53,55,32,45,50,46,49,48,57,51,56,44,49,46,54,48,49,53,54,32,45,52,46,57,54,48,57,52,44,51,46,57,56,52,51,55,32,122,34,10,32,32,32,32,32,
32,32,32,32,115,116,121,108,101,61,34,102,111,110,116,45,115,116,121,108,101,58,111,98,108,105,113,117,101,59,102,111,110,116,45,118,97,114,105,97,110,116,58,110,111,114,109,97,108,59,102,111,110,116,
45,119,101,105,103,104,116,58,110,111,114,109,97,108,59,102,111,110,116,45,115,116,114,101,116,99,104,58,110,111,114,109,97,108,59,102,111,110,116,45,115,105,122,101,58,56,48,112,120,59,108,105,110,101,
45,104,101,105,103,104,116,58,49,50,53,37,59,102,111,110,116,45,102,97,109,105,108,121,58,39,80,97,108,97,99,101,32,83,99,114,105,112,116,32,77,84,39,59,45,105,110,107,115,99,97,112,101,45,102,111,110,
116,45,115,112,101,99,105,102,105,99,97,116,105,111,110,58,39,80,97,108,97,99,101,32,83,99,114,105,112,116,32,77,84,32,79,98,108,105,113,117,101,39,59,116,101,120,116,45,97,108,105,103,110,58,115,116,
97,114,116,59,119,114,105,116,105,110,103,45,109,111,100,101,58,108,114,45,116,98,59,116,101,120,116,45,97,110,99,104,111,114,58,115,116,97,114,116,59,102,105,108,108,58,35,102,57,102,57,102,57,34,10,
32,32,32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,51,55,51,55,34,32,47,62,10,32,32,32,32,32,32,60,112,97,116,104,10,32,32,32,32,32,32,32,32,32,100,61,34,109,32,50,53,57,46,56,52,49,50,50,44,53,52,
52,46,49,55,53,55,56,32,118,32,45,48,46,54,50,53,32,113,32,53,46,57,51,55,53,44,45,53,46,53,48,55,56,49,32,56,46,51,50,48,51,49,44,45,53,46,53,48,55,56,49,32,49,46,51,50,56,49,50,44,48,32,49,46,51,50,
56,49,50,44,49,46,54,55,57,54,57,32,48,44,49,46,51,50,56,49,50,32,45,48,46,56,50,48,51,49,44,50,46,54,49,55,49,56,32,45,48,46,56,50,48,51,49,44,49,46,50,56,57,48,55,32,45,51,46,51,50,48,51,49,44,52,46,
52,57,50,49,57,32,52,46,56,52,51,55,53,44,45,52,46,48,50,51,52,52,32,56,46,51,57,56,52,52,44,45,54,46,52,48,54,50,53,32,51,46,53,53,52,54,56,44,45,50,46,51,56,50,56,49,32,54,46,48,57,51,55,53,44,45,50,
46,51,56,50,56,49,32,49,46,51,50,56,49,50,44,48,32,49,46,51,50,56,49,50,44,49,46,56,51,53,57,52,32,48,44,49,46,55,57,54,56,55,32,45,49,46,50,53,44,51,46,53,57,51,55,53,32,45,49,46,50,53,44,49,46,55,53,
55,56,49,32,45,52,46,49,48,49,53,54,44,51,46,56,50,56,49,50,32,45,51,46,48,52,54,56,56,44,50,46,51,48,52,54,57,32,45,51,46,48,52,54,56,56,44,51,46,50,56,49,50,53,32,48,44,48,46,51,57,48,54,51,32,48,46,
51,53,49,53,55,44,48,46,55,52,50,49,57,32,48,46,51,57,48,54,50,44,48,46,51,49,50,53,32,48,46,55,56,49,50,53,44,48,46,51,49,50,53,32,49,46,51,54,55,49,56,44,48,32,51,46,53,57,51,55,53,44,45,49,46,52,56,
52,51,56,32,50,46,50,54,53,54,50,44,45,49,46,53,50,51,52,51,32,52,46,57,50,49,56,55,44,45,51,46,56,50,56,49,50,32,50,46,54,53,54,50,53,44,45,50,46,51,52,51,55,53,32,52,46,50,57,54,56,56,44,45,51,46,55,
53,32,118,32,48,46,54,54,52,48,54,32,113,32,45,50,46,51,56,50,56,50,44,50,46,50,50,54,53,54,32,45,52,46,54,52,56,52,52,44,52,46,49,55,57,54,57,32,45,50,46,50,54,53,54,51,44,49,46,57,49,52,48,54,32,45,
52,46,53,51,49,50,53,44,51,46,51,53,57,51,55,32,45,50,46,50,50,54,53,54,44,49,46,52,48,54,50,53,32,45,51,46,55,49,48,57,52,44,49,46,52,48,54,50,53,32,45,48,46,57,51,55,53,44,48,32,45,49,46,51,54,55,49,
57,44,45,48,46,54,50,53,32,45,48,46,51,57,48,54,50,44,45,48,46,54,50,53,32,45,48,46,51,57,48,54,50,44,45,49,46,50,56,57,48,54,32,48,44,45,49,46,52,56,52,51,55,32,49,46,49,55,49,56,55,44,45,51,46,51,50,
48,51,49,32,49,46,49,55,49,56,56,44,45,49,46,56,55,53,32,51,46,50,52,50,49,57,44,45,51,46,54,51,50,56,49,32,50,46,51,56,50,56,49,44,45,50,46,49,48,57,51,56,32,51,46,50,56,49,50,53,44,45,51,46,51,53,57,
51,56,32,48,46,57,51,55,53,44,45,49,46,50,56,57,48,54,32,45,48,46,53,48,55,56,49,44,45,49,46,50,56,57,48,54,32,45,49,46,56,51,53,57,52,44,48,32,45,53,46,53,52,54,56,56,44,50,46,53,32,45,51,46,55,49,48,
57,51,44,50,46,52,54,48,57,52,32,45,54,46,57,53,51,49,50,44,53,46,50,51,52,51,55,32,45,51,46,50,52,50,49,57,44,50,46,55,51,52,51,56,32,45,54,46,49,51,50,56,49,44,53,46,51,57,48,54,51,32,104,32,45,51,46,
48,52,54,56,56,32,108,32,53,46,49,57,53,51,49,44,45,53,46,56,53,57,51,56,32,113,32,53,46,55,48,51,49,51,44,45,54,46,51,54,55,49,56,32,53,46,55,48,51,49,51,44,45,54,46,57,53,51,49,50,32,48,44,45,48,46,
51,57,48,54,51,32,45,48,46,52,54,56,55,53,44,45,48,46,51,57,48,54,51,32,45,49,46,55,49,56,55,53,44,48,32,45,56,46,49,54,52,48,54,44,53,46,53,56,53,57,52,32,122,34,10,32,32,32,32,32,32,32,32,32,115,116,
121,108,101,61,34,102,111,110,116,45,115,116,121,108,101,58,111,98,108,105,113,117,101,59,102,111,110,116,45,118,97,114,105,97,110,116,58,110,111,114,109,97,108,59,102,111,110,116,45,119,101,105,103,104,
116,58,110,111,114,109,97,108,59,102,111,110,116,45,115,116,114,101,116,99,104,58,110,111,114,109,97,108,59,102,111,110,116,45,115,105,122,101,58,56,48,112,120,59,108,105,110,101,45,104,101,105,103,104,
116,58,49,50,53,37,59,102,111,110,116,45,102,97,109,105,108,121,58,39,80,97,108,97,99,101,32,83,99,114,105,112,116,32,77,84,39,59,45,105,110,107,115,99,97,112,101,45,102,111,110,116,45,115,112,101,99,
105,102,105,99,97,116,105,111,110,58,39,80,97,108,97,99,101,32,83,99,114,105,112,116,32,77,84,32,79,98,108,105,113,117,101,39,59,116,101,120,116,45,97,108,105,103,110,58,115,116,97,114,116,59,119,114,
105,116,105,110,103,45,109,111,100,101,58,108,114,45,116,98,59,116,101,120,116,45,97,110,99,104,111,114,58,115,116,97,114,116,59,102,105,108,108,58,35,102,57,102,57,102,57,34,10,32,32,32,32,32,32,32,32,
32,105,100,61,34,112,97,116,104,51,55,51,57,34,32,47,62,10,32,32,32,32,32,32,60,112,97,116,104,10,32,32,32,32,32,32,32,32,32,100,61,34,109,32,50,57,55,46,51,48,50,49,53,44,53,51,56,46,53,56,57,56,52,32,
113,32,45,50,46,51,52,51,55,53,44,48,32,45,53,46,50,51,52,51,55,44,50,46,51,48,52,54,57,32,45,50,46,56,53,49,53,54,44,50,46,51,48,52,54,57,32,45,53,44,53,46,50,51,52,51,56,32,45,50,46,49,48,57,51,56,44,
50,46,57,50,57,54,56,32,45,50,46,49,48,57,51,56,44,52,46,49,52,48,54,50,32,48,44,48,46,53,56,53,57,52,32,48,46,51,57,48,54,51,44,48,46,57,55,54,53,54,32,48,46,51,57,48,54,50,44,48,46,51,57,48,54,51,32,
48,46,57,55,54,53,54,44,48,46,51,57,48,54,51,32,49,46,54,48,49,53,54,44,48,32,52,46,54,48,57,51,56,44,45,50,46,50,50,54,53,54,32,51,46,48,48,55,56,49,44,45,50,46,50,50,54,53,55,32,53,46,52,50,57,54,56,
44,45,53,46,48,51,57,48,55,32,50,46,52,50,49,56,56,44,45,50,46,56,49,50,53,32,50,46,52,50,49,56,56,44,45,52,46,52,49,52,48,54,32,48,44,45,49,46,51,54,55,49,57,32,45,49,46,52,56,52,51,56,44,45,49,46,51,
54,55,49,57,32,122,32,109,32,49,48,46,51,49,50,53,44,52,46,56,48,52,54,57,32,45,49,46,57,57,50,49,56,44,49,46,54,55,57,54,57,32,113,32,45,52,46,54,56,55,53,44,51,46,57,48,54,50,53,32,45,54,46,57,53,51,
49,51,44,53,46,53,48,55,56,49,32,45,50,46,50,50,54,53,54,44,49,46,54,48,49,53,54,32,45,51,46,57,52,53,51,49,44,49,46,54,48,49,53,54,32,45,49,46,49,51,50,56,49,44,48,32,45,49,46,49,51,50,56,49,44,45,49,
46,50,56,57,48,54,32,48,44,45,49,46,51,54,55,49,57,32,49,46,48,53,52,54,56,44,45,51,46,56,50,56,49,50,32,45,53,46,53,56,53,57,51,44,53,46,49,49,55,49,56,32,45,56,46,50,52,50,49,56,44,53,46,49,49,55,49,
56,32,45,49,46,49,55,49,56,56,44,48,32,45,49,46,57,49,52,48,55,44,45,48,46,56,57,56,52,51,32,45,48,46,55,52,50,49,56,44,45,48,46,57,51,55,53,32,45,48,46,55,52,50,49,56,44,45,50,46,54,57,53,51,50,32,48,
44,45,49,46,57,57,50,49,56,32,49,46,49,55,49,56,55,44,45,51,46,57,48,54,50,53,32,49,46,49,55,49,56,56,44,45,49,46,57,53,51,49,50,32,51,46,49,50,53,44,45,51,46,52,51,55,53,32,49,46,57,57,50,49,57,44,45,
49,46,52,56,52,51,55,32,52,46,52,49,52,48,54,44,45,50,46,51,52,51,55,53,32,50,46,52,54,48,57,52,44,45,48,46,56,53,57,51,55,32,52,46,55,54,53,54,51,44,45,48,46,56,53,57,51,55,32,49,46,49,51,50,56,49,44,
48,32,49,46,54,52,48,54,50,44,48,46,52,50,57,54,57,32,48,46,53,52,54,56,56,44,48,46,51,57,48,54,50,32,48,46,53,56,53,57,52,44,49,46,51,50,56,49,50,32,45,48,46,48,51,57,49,44,48,46,49,57,53,51,49,32,45,
48,46,48,55,56,49,44,48,46,51,57,48,54,51,32,48,44,48,46,49,53,54,50,53,32,45,48,46,48,55,56,49,44,48,46,52,54,56,55,53,32,45,48,46,48,51,57,49,44,48,46,51,49,50,53,32,45,48,46,48,55,56,49,44,48,46,53,
48,55,56,49,32,108,32,50,46,52,54,48,57,51,44,45,50,46,56,57,48,54,51,32,51,46,50,56,49,50,53,44,45,48,46,50,51,52,51,55,32,45,49,46,48,49,53,54,50,44,49,46,50,53,32,113,32,45,49,46,50,53,44,49,46,52,
56,52,51,55,32,45,50,46,48,51,49,50,53,44,50,46,52,54,48,57,52,32,45,48,46,55,56,49,50,53,44,48,46,57,51,55,53,32,45,49,46,51,50,56,49,51,44,49,46,53,54,50,53,32,45,54,46,48,57,51,55,53,44,54,46,54,48,
49,53,54,32,45,54,46,48,57,51,55,53,44,55,46,55,51,52,51,55,32,48,44,48,46,53,56,53,57,52,32,48,46,53,52,54,56,56,44,48,46,53,56,53,57,52,32,48,46,55,52,50,49,57,44,48,32,50,46,50,50,54,53,54,44,45,48,
46,56,53,57,51,56,32,49,46,52,56,52,51,56,44,45,48,46,56,57,56,52,51,32,51,46,51,50,48,51,49,44,45,50,46,51,52,51,55,53,32,49,46,56,55,53,44,45,49,46,52,52,53,51,49,32,52,46,50,53,55,56,50,44,45,51,46,
51,57,56,52,51,32,50,46,51,56,50,56,49,44,45,49,46,57,57,50,49,57,32,50,46,55,55,51,52,51,44,45,50,46,51,48,52,54,57,32,122,34,10,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,111,110,116,45,
115,116,121,108,101,58,111,98,108,105,113,117,101,59,102,111,110,116,45,118,97,114,105,97,110,116,58,110,111,114,109,97,108,59,102,111,110,116,45,119,101,105,103,104,116,58,110,111,114,109,97,108,59,102,
111,110,116,45,115,116,114,101,116,99,104,58,110,111,114,109,97,108,59,102,111,110,116,45,115,105,122,101,58,56,48,112,120,59,108,105,110,101,45,104,101,105,103,104,116,58,49,50,53,37,59,102,111,110,116,
45,102,97,109,105,108,121,58,39,80,97,108,97,99,101,32,83,99,114,105,112,116,32,77,84,39,59,45,105,110,107,115,99,97,112,101,45,102,111,110,116,45,115,112,101,99,105,102,105,99,97,116,105,111,110,58,39,
80,97,108,97,99,101,32,83,99,114,105,112,116,32,77,84,32,79,98,108,105,113,117,101,39,59,116,101,120,116,45,97,108,105,103,110,58,115,116,97,114,116,59,119,114,105,116,105,110,103,45,109,111,100,101,58,
108,114,45,116,98,59,116,101,120,116,45,97,110,99,104,111,114,58,115,116,97,114,116,59,102,105,108,108,58,35,102,57,102,57,102,57,34,10,32,32,32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,51,55,52,
49,34,32,47,62,10,32,32,32,32,32,32,60,112,97,116,104,10,32,32,32,32,32,32,32,32,32,100,61,34,109,32,51,48,54,46,54,51,56,48,57,44,53,52,52,46,49,55,53,55,56,32,118,32,45,48,46,55,48,51,49,50,32,113,32,
50,46,56,57,48,54,51,44,45,50,46,54,49,55,49,57,32,52,46,56,52,51,55,53,44,45,52,46,48,50,51,52,52,32,49,46,57,53,51,49,51,44,45,49,46,52,48,54,50,53,32,51,46,52,55,54,53,54,44,45,49,46,52,48,54,50,53,
32,49,46,50,53,44,48,32,49,46,50,53,44,49,46,54,55,57,54,57,32,48,44,49,46,52,48,54,50,53,32,45,48,46,56,50,48,51,49,44,50,46,55,51,52,51,55,32,45,48,46,56,50,48,51,49,44,49,46,50,56,57,48,54,32,45,50,
46,55,55,51,52,52,44,51,46,55,53,32,53,46,52,54,56,55,53,44,45,52,46,52,57,50,49,57,32,56,46,50,48,51,49,51,44,45,54,46,51,50,56,49,50,32,50,46,55,55,51,52,52,44,45,49,46,56,51,53,57,52,32,52,46,55,54,
53,54,50,44,45,49,46,56,51,53,57,52,32,49,46,52,52,53,51,50,44,48,32,49,46,52,52,53,51,50,44,49,46,51,50,56,49,50,32,48,44,50,46,54,53,54,50,53,32,45,51,46,57,48,54,50,53,44,55,46,51,48,52,54,57,32,108,
32,49,46,48,57,51,55,53,44,45,48,46,56,50,48,51,49,32,113,32,50,46,54,49,55,49,56,44,45,49,46,57,49,52,48,54,32,53,44,45,51,46,54,55,49,56,56,32,50,46,51,56,50,56,49,44,45,49,46,55,53,55,56,49,32,52,46,
53,55,48,51,49,44,45,50,46,57,50,57,54,56,32,50,46,50,50,54,53,54,44,45,49,46,50,49,48,57,52,32,51,46,53,53,52,54,57,44,45,49,46,50,49,48,57,52,32,48,46,54,50,53,44,48,32,48,46,57,55,54,53,54,44,48,46,
53,48,55,56,49,32,48,46,51,57,48,54,50,44,48,46,52,54,56,55,53,32,48,46,51,57,48,54,50,44,49,46,52,48,54,50,53,32,48,44,49,46,55,57,54,56,56,32,45,49,46,50,49,48,57,51,44,51,46,53,53,52,54,57,32,45,49,
46,49,55,49,56,56,44,49,46,55,49,56,55,53,32,45,52,46,48,54,50,53,44,51,46,56,54,55,49,57,32,45,49,46,49,55,49,56,56,44,48,46,56,53,57,51,55,32,45,50,46,50,50,54,53,55,44,49,46,57,57,50,49,56,32,45,49,
46,48,53,52,54,56,44,49,46,49,51,50,56,50,32,45,49,46,48,53,52,54,56,44,49,46,53,50,51,52,52,32,48,44,48,46,55,52,50,49,57,32,49,46,48,57,51,55,53,44,48,46,55,52,50,49,57,32,49,46,55,49,56,55,53,44,48,
32,52,46,53,51,49,50,53,44,45,50,46,48,55,48,51,49,32,50,46,56,53,49,53,54,44,45,50,46,49,48,57,51,56,32,56,46,52,51,55,53,44,45,54,46,56,51,53,57,52,32,118,32,48,46,54,54,52,48,54,32,113,32,45,53,46,
51,49,50,53,44,52,46,53,55,48,51,49,32,45,56,46,49,54,52,48,55,44,54,46,54,55,57,54,57,32,45,50,46,56,49,50,53,44,50,46,49,48,57,51,55,32,45,52,46,57,54,48,57,51,44,50,46,49,48,57,51,55,32,45,49,46,53,
50,51,52,52,44,48,32,45,49,46,53,50,51,52,52,44,45,49,46,53,50,51,52,51,32,48,44,45,49,46,54,55,57,54,57,32,49,46,49,51,50,56,49,44,45,51,46,51,57,56,52,52,32,49,46,49,51,50,56,49,44,45,49,46,55,49,56,
55,53,32,51,46,56,50,56,49,51,44,45,52,46,50,53,55,56,49,32,51,46,49,50,53,44,45,50,46,57,50,57,54,57,32,51,46,49,50,53,44,45,51,46,55,49,48,57,52,32,48,44,45,48,46,55,48,51,49,51,32,45,48,46,54,50,53,
44,45,48,46,55,48,51,49,51,32,45,49,46,48,49,53,54,51,44,48,32,45,52,46,52,49,52,48,55,44,50,46,49,56,55,53,32,45,51,46,51,57,56,52,51,44,50,46,49,52,56,52,52,32,45,55,46,53,51,57,48,54,44,53,46,51,53,
49,53,55,32,45,52,46,49,48,49,53,54,44,51,46,49,54,52,48,54,32,45,54,46,55,53,55,56,49,44,53,46,54,54,52,48,54,32,104,32,45,51,46,50,56,49,50,53,32,108,32,54,46,48,53,52,54,57,44,45,54,46,54,52,48,54,
51,32,113,32,48,46,54,50,53,44,45,48,46,54,54,52,48,54,32,49,46,56,55,53,44,45,49,46,57,57,50,49,56,32,49,46,50,56,57,48,54,44,45,49,46,51,50,56,49,51,32,50,46,49,56,55,53,44,45,50,46,51,56,50,56,50,32,
48,46,56,57,56,52,51,44,45,49,46,48,57,51,55,53,32,48,46,56,57,56,52,51,44,45,49,46,52,56,52,51,55,32,48,44,45,48,46,49,57,53,51,49,32,45,48,46,49,57,53,51,49,44,45,48,46,51,57,48,54,51,32,45,48,46,49,
53,54,50,53,44,45,48,46,50,51,52,51,55,32,45,48,46,53,48,55,56,49,44,45,48,46,50,51,52,51,55,32,45,50,46,49,56,55,53,44,48,32,45,54,46,53,50,51,52,52,44,51,46,51,57,56,52,52,32,45,52,46,51,51,53,57,52,
44,51,46,51,57,56,52,51,32,45,49,48,46,49,57,53,51,49,44,56,46,54,55,49,56,55,32,108,32,45,49,46,49,51,50,56,49,44,49,46,48,53,52,54,57,32,104,32,45,51,46,51,53,57,51,56,32,108,32,56,46,50,52,50,49,57,
44,45,57,46,50,49,56,55,53,32,113,32,50,46,56,49,50,53,44,45,50,46,56,57,48,54,51,32,50,46,56,49,50,53,44,45,51,46,52,51,55,53,32,48,44,45,48,46,53,52,54,56,56,32,45,48,46,52,54,56,55,53,44,45,48,46,53,
52,54,56,56,32,45,48,46,54,54,52,48,54,44,48,32,45,49,46,57,57,50,49,57,44,48,46,56,53,57,51,56,32,45,49,46,51,50,56,49,50,44,48,46,56,50,48,51,49,32,45,50,46,56,57,48,54,50,44,50,46,48,55,48,51,49,32,
45,49,46,53,54,50,53,44,49,46,50,53,32,45,51,46,50,48,51,49,51,44,50,46,54,53,54,50,53,32,122,34,10,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,111,110,116,45,115,116,121,108,101,58,111,98,
108,105,113,117,101,59,102,111,110,116,45,118,97,114,105,97,110,116,58,110,111,114,109,97,108,59,102,111,110,116,45,119,101,105,103,104,116,58,110,111,114,109,97,108,59,102,111,110,116,45,115,116,114,
101,116,99,104,58,110,111,114,109,97,108,59,102,111,110,116,45,115,105,122,101,58,56,48,112,120,59,108,105,110,101,45,104,101,105,103,104,116,58,49,50,53,37,59,102,111,110,116,45,102,97,109,105,108,121,
58,39,80,97,108,97,99,101,32,83,99,114,105,112,116,32,77,84,39,59,45,105,110,107,115,99,97,112,101,45,102,111,110,116,45,115,112,101,99,105,102,105,99,97,116,105,111,110,58,39,80,97,108,97,99,101,32,83,
99,114,105,112,116,32,77,84,32,79,98,108,105,113,117,101,39,59,116,101,120,116,45,97,108,105,103,110,58,115,116,97,114,116,59,119,114,105,116,105,110,103,45,109,111,100,101,58,108,114,45,116,98,59,116,
101,120,116,45,97,110,99,104,111,114,58,115,116,97,114,116,59,102,105,108,108,58,35,102,57,102,57,102,57,34,10,32,32,32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,51,55,52,51,34,32,47,62,10,32,32,
32,32,32,32,60,112,97,116,104,10,32,32,32,32,32,32,32,32,32,100,61,34,109,32,51,53,54,46,57,53,48,53,57,44,53,51,48,46,49,49,51,50,56,32,113,32,48,46,56,53,57,51,56,44,48,32,48,46,56,53,57,51,56,44,49,
46,48,53,52,54,57,32,48,44,48,46,54,50,53,32,45,48,46,51,53,49,53,55,44,49,46,48,49,53,54,50,32,45,48,46,51,53,49,53,54,44,48,46,51,57,48,54,51,32,45,48,46,56,57,56,52,51,44,48,46,51,57,48,54,51,32,45,
49,46,48,53,52,54,57,44,48,32,45,49,46,48,53,52,54,57,44,45,48,46,57,51,55,53,32,48,44,45,48,46,53,52,54,56,56,32,48,46,52,50,57,54,57,44,45,49,46,48,49,53,54,51,32,48,46,52,50,57,54,56,44,45,48,46,53,
48,55,56,49,32,49,46,48,49,53,54,50,44,45,48,46,53,48,55,56,49,32,122,32,109,32,45,49,51,46,53,49,53,54,50,44,49,51,46,57,56,52,51,56,32,45,48,46,48,55,56,49,44,45,48,46,54,50,53,32,113,32,50,46,57,54,
56,55,53,44,45,50,46,54,57,53,51,50,32,54,46,57,53,51,49,51,44,45,53,46,49,49,55,49,57,32,104,32,51,46,53,49,53,54,50,32,108,32,45,57,46,50,49,56,55,53,44,49,48,46,51,49,50,53,32,113,32,45,48,46,57,55,
54,53,54,44,49,46,48,57,51,55,53,32,45,49,46,50,53,44,50,46,48,51,49,50,53,32,45,48,46,50,55,51,52,52,44,48,46,57,51,55,53,32,48,46,52,54,56,55,53,44,48,46,57,51,55,53,32,49,46,57,53,51,49,51,44,48,32,
53,46,56,50,48,51,49,44,45,50,46,56,57,48,54,51,32,51,46,56,54,55,49,57,44,45,50,46,56,57,48,54,50,32,55,46,49,52,56,52,52,44,45,54,46,48,57,51,55,53,32,118,32,48,46,54,54,52,48,55,32,113,32,45,51,46,
55,56,57,48,54,44,51,46,54,51,50,56,49,32,45,55,46,51,48,52,54,57,44,54,46,50,53,32,45,51,46,53,49,53,54,50,44,50,46,54,49,55,49,56,32,45,53,46,55,52,50,49,56,44,50,46,54,49,55,49,56,32,45,49,46,50,56,
57,48,55,44,48,32,45,49,46,50,56,57,48,55,44,45,49,46,51,54,55,49,56,32,48,44,45,52,46,48,54,50,53,32,54,46,51,50,56,49,51,44,45,49,48,46,51,57,48,54,51,32,48,46,51,53,49,53,54,44,45,48,46,51,53,49,53,
54,32,48,46,54,50,53,44,45,48,46,54,54,52,48,54,32,48,46,51,49,50,53,44,45,48,46,51,53,49,53,54,32,48,46,53,56,53,57,52,44,45,48,46,54,50,53,32,45,51,46,53,49,53,54,51,44,50,46,49,48,57,51,55,32,45,54,
46,53,54,50,53,44,52,46,57,54,48,57,52,32,122,34,10,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,111,110,116,45,115,116,121,108,101,58,111,98,108,105,113,117,101,59,102,111,110,116,45,118,
97,114,105,97,110,116,58,110,111,114,109,97,108,59,102,111,110,116,45,119,101,105,103,104,116,58,110,111,114,109,97,108,59,102,111,110,116,45,115,116,114,101,116,99,104,58,110,111,114,109,97,108,59,102,
111,110,116,45,115,105,122,101,58,56,48,112,120,59,108,105,110,101,45,104,101,105,103,104,116,58,49,50,53,37,59,102,111,110,116,45,102,97,109,105,108,121,58,39,80,97,108,97,99,101,32,83,99,114,105,112,
116,32,77,84,39,59,45,105,110,107,115,99,97,112,101,45,102,111,110,116,45,115,112,101,99,105,102,105,99,97,116,105,111,110,58,39,80,97,108,97,99,101,32,83,99,114,105,112,116,32,77,84,32,79,98,108,105,
113,117,101,39,59,116,101,120,116,45,97,108,105,103,110,58,115,116,97,114,116,59,119,114,105,116,105,110,103,45,109,111,100,101,58,108,114,45,116,98,59,116,101,120,116,45,97,110,99,104,111,114,58,115,
116,97,114,116,59,102,105,108,108,58,35,102,57,102,57,102,57,34,10,32,32,32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,51,55,52,53,34,32,47,62,10,32,32,32,32,32,32,60,112,97,116,104,10,32,32,32,32,
32,32,32,32,32,100,61,34,109,32,51,55,50,46,54,53,51,55,50,44,53,52,50,46,53,55,52,50,50,32,118,32,48,46,54,54,52,48,54,32,113,32,45,57,46,50,53,55,56,50,44,56,46,57,52,53,51,49,32,45,49,53,46,52,50,57,
54,57,44,56,46,57,52,53,51,49,32,45,49,46,54,52,48,54,51,44,48,32,45,50,46,51,56,50,56,49,44,45,48,46,56,50,48,51,49,32,45,48,46,55,52,50,49,57,44,45,48,46,56,53,57,51,55,32,45,48,46,55,52,50,49,57,44,
45,51,46,48,48,55,56,49,32,48,44,45,49,46,56,55,53,32,49,46,50,49,48,57,52,44,45,51,46,55,56,57,48,54,32,49,46,49,55,49,56,55,44,45,49,46,57,49,52,48,55,32,51,46,49,50,53,44,45,51,46,51,53,57,51,56,32,
49,46,57,53,51,49,50,44,45,49,46,52,56,52,51,55,32,52,46,50,57,54,56,55,44,45,50,46,51,48,52,54,57,32,50,46,51,52,51,55,53,44,45,48,46,56,53,57,51,55,32,52,46,53,55,48,51,49,44,45,48,46,56,53,57,51,55,
32,49,46,48,49,53,54,51,44,48,32,49,46,54,52,48,54,51,44,48,46,53,48,55,56,49,32,48,46,54,54,52,48,54,44,48,46,53,48,55,56,49,32,48,46,54,54,52,48,54,44,49,46,52,48,54,50,53,32,48,44,49,46,48,49,53,54,
51,32,45,48,46,56,50,48,51,49,44,49,46,56,51,53,57,52,32,45,48,46,55,56,49,50,53,44,48,46,55,56,49,50,53,32,45,49,46,55,53,55,56,49,44,48,46,55,56,49,50,53,32,45,49,46,49,55,49,56,56,44,48,32,45,49,46,
49,55,49,56,56,44,45,49,46,48,49,53,54,51,32,48,44,45,49,46,53,50,51,52,51,32,49,46,51,54,55,49,57,44,45,49,46,53,50,51,52,51,32,48,46,55,56,49,50,53,44,48,32,48,46,57,51,55,53,44,48,46,55,48,51,49,50,
32,48,46,49,53,54,50,53,44,48,46,50,51,52,51,56,32,48,46,51,49,50,53,44,48,46,50,51,52,51,56,32,48,46,49,57,53,51,49,44,48,32,48,46,51,57,48,54,50,44,45,48,46,51,49,50,53,32,48,46,49,57,53,51,50,44,45,
48,46,51,49,50,53,32,48,46,49,57,53,51,50,44,45,48,46,55,48,51,49,51,32,48,44,45,48,46,54,50,53,32,45,48,46,52,50,57,54,57,44,45,48,46,57,55,54,53,54,32,45,48,46,52,50,57,54,57,44,45,48,46,51,57,48,54,
51,32,45,49,46,49,55,49,56,56,44,45,48,46,51,57,48,54,51,32,45,50,46,48,55,48,51,49,44,48,32,45,53,46,48,51,57,48,54,44,50,46,50,50,54,53,55,32,45,50,46,57,50,57,54,57,44,50,46,50,50,54,53,54,32,45,53,
46,48,55,56,49,50,44,53,46,48,55,56,49,50,32,45,50,46,49,52,56,52,52,44,50,46,56,53,49,53,54,32,45,50,46,49,52,56,52,52,44,52,46,52,53,51,49,51,32,48,44,49,46,50,56,57,48,54,32,49,46,57,49,52,48,54,44,
49,46,50,56,57,48,54,32,51,46,53,53,52,54,57,44,48,32,54,46,57,49,52,48,54,44,45,50,46,49,56,55,53,32,51,46,51,53,57,51,56,44,45,50,46,49,56,55,53,32,56,46,54,51,50,56,50,44,45,54,46,56,55,53,32,122,34,
10,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,111,110,116,45,115,116,121,108,101,58,111,98,108,105,113,117,101,59,102,111,110,116,45,118,97,114,105,97,110,116,58,110,111,114,109,97,108,59,
102,111,110,116,45,119,101,105,103,104,116,58,110,111,114,109,97,108,59,102,111,110,116,45,115,116,114,101,116,99,104,58,110,111,114,109,97,108,59,102,111,110,116,45,115,105,122,101,58,56,48,112,120,59,
108,105,110,101,45,104,101,105,103,104,116,58,49,50,53,37,59,102,111,110,116,45,102,97,109,105,108,121,58,39,80,97,108,97,99,101,32,83,99,114,105,112,116,32,77,84,39,59,45,105,110,107,115,99,97,112,101,
45,102,111,110,116,45,115,112,101,99,105,102,105,99,97,116,105,111,110,58,39,80,97,108,97,99,101,32,83,99,114,105,112,116,32,77,84,32,79,98,108,105,113,117,101,39,59,116,101,120,116,45,97,108,105,103,
110,58,115,116,97,114,116,59,119,114,105,116,105,110,103,45,109,111,100,101,58,108,114,45,116,98,59,116,101,120,116,45,97,110,99,104,111,114,58,115,116,97,114,116,59,102,105,108,108,58,35,102,57,102,57,
102,57,34,10,32,32,32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,51,55,52,55,34,32,47,62,10,32,32,32,32,32,32,60,112,97,116,104,10,32,32,32,32,32,32,32,32,32,100,61,34,109,32,51,55,49,46,55,57,52,
51,52,44,53,52,51,46,53,53,48,55,56,32,56,46,49,54,52,48,54,44,45,55,46,48,51,49,50,53,32,113,32,48,46,57,51,55,53,44,45,49,46,49,51,50,56,49,32,49,46,55,49,56,55,53,44,45,50,46,48,55,48,51,49,32,48,46,
55,56,49,50,53,44,45,48,46,57,55,54,53,54,32,49,46,49,51,50,56,50,44,45,48,46,57,55,54,53,54,32,48,46,51,49,50,53,44,48,32,48,46,53,52,54,56,55,44,48,46,49,57,53,51,49,32,48,46,50,55,51,52,52,44,48,46,
49,53,54,50,53,32,48,46,50,55,51,52,52,44,48,46,53,48,55,56,49,32,48,44,48,46,55,52,50,49,57,32,45,50,46,49,52,56,52,52,44,49,46,56,51,53,57,52,32,49,46,50,53,44,49,46,56,55,53,32,49,46,55,57,54,56,56,
44,51,46,50,56,49,50,53,32,48,46,53,52,54,56,55,44,49,46,52,48,54,50,53,32,48,46,54,54,52,48,54,44,51,46,53,53,52,54,57,32,45,48,46,49,49,55,49,57,44,49,46,51,50,56,49,50,32,45,48,46,51,49,50,53,44,50,
46,50,50,54,53,54,32,49,46,52,48,54,50,53,44,48,46,51,53,49,53,54,32,50,46,53,51,57,48,54,44,45,48,46,48,51,57,49,32,49,46,49,55,49,56,56,44,45,48,46,52,50,57,54,57,32,51,46,50,48,51,49,51,44,45,50,46,
52,54,48,57,52,32,118,32,48,46,54,54,52,48,54,32,113,32,45,49,46,53,54,50,53,44,49,46,57,53,51,49,51,32,45,50,46,56,57,48,54,51,44,50,46,52,50,49,56,56,32,45,49,46,51,50,56,49,50,44,48,46,52,54,56,55,
53,32,45,51,46,48,56,53,57,52,44,45,48,46,48,51,57,49,32,45,48,46,57,51,55,53,44,50,46,49,48,57,51,56,32,45,51,46,48,52,54,56,55,44,51,46,54,51,50,56,50,32,45,50,46,49,48,57,51,56,44,49,46,52,56,52,51,
55,32,45,52,46,51,51,53,57,52,44,50,46,50,50,54,53,54,32,45,50,46,50,50,54,53,54,44,48,46,55,48,51,49,50,32,45,51,46,56,50,56,49,50,44,48,46,55,48,51,49,50,32,45,49,46,53,50,51,52,52,44,48,32,45,50,46,
52,54,48,57,52,44,45,48,46,53,56,53,57,51,32,45,48,46,57,55,54,53,54,44,45,48,46,54,50,53,32,45,48,46,57,55,54,53,54,44,45,49,46,55,49,56,55,53,32,48,44,45,48,46,57,55,54,53,55,32,48,46,53,56,53,57,51,
44,45,49,46,53,54,50,53,32,48,46,53,52,54,56,56,44,45,48,46,54,50,53,32,49,46,54,52,48,54,51,44,45,48,46,54,50,53,32,48,46,55,48,51,49,50,44,48,32,49,46,49,55,49,56,55,44,48,46,51,57,48,54,50,32,48,46,
52,50,57,54,57,44,48,46,51,53,49,53,54,32,48,46,52,50,57,54,57,44,48,46,57,55,54,53,54,32,48,44,48,46,53,48,55,56,50,32,45,48,46,52,54,56,55,53,44,48,46,56,57,56,52,52,32,45,48,46,53,48,55,56,49,44,48,
46,51,57,48,54,51,32,45,49,46,48,53,52,54,57,44,48,46,51,57,48,54,51,32,45,48,46,55,52,50,49,56,44,48,32,45,49,46,48,57,51,55,53,44,45,49,46,50,49,48,57,52,32,45,48,46,53,48,55,56,49,44,48,46,51,53,49,
53,54,32,45,48,46,53,48,55,56,49,44,49,46,48,53,52,54,57,32,48,44,48,46,52,50,57,54,56,32,48,46,56,50,48,51,49,44,48,46,57,51,55,53,32,48,46,55,56,49,50,53,44,48,46,53,48,55,56,49,32,49,46,56,51,53,57,
52,44,48,46,53,48,55,56,49,32,50,46,48,55,48,51,49,44,48,32,52,46,51,55,53,44,45,49,46,55,53,55,56,49,32,50,46,51,48,52,54,57,44,45,49,46,55,57,54,56,56,32,51,46,56,50,56,49,51,44,45,52,46,52,57,50,49,
57,32,49,46,53,54,50,53,44,45,50,46,54,57,53,51,49,32,49,46,53,54,50,53,44,45,53,46,49,57,53,51,49,32,48,44,45,49,46,55,57,54,56,56,32,45,49,46,49,51,50,56,50,44,45,51,46,54,55,49,56,56,32,108,32,45,56,
46,57,52,53,51,49,44,55,46,54,53,54,50,53,32,122,34,10,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,111,110,116,45,115,116,121,108,101,58,111,98,108,105,113,117,101,59,102,111,110,116,45,118,
97,114,105,97,110,116,58,110,111,114,109,97,108,59,102,111,110,116,45,119,101,105,103,104,116,58,110,111,114,109,97,108,59,102,111,110,116,45,115,116,114,101,116,99,104,58,110,111,114,109,97,108,59,102,
111,110,116,45,115,105,122,101,58,56,48,112,120,59,108,105,110,101,45,104,101,105,103,104,116,58,49,50,53,37,59,102,111,110,116,45,102,97,109,105,108,121,58,39,80,97,108,97,99,101,32,83,99,114,105,112,
116,32,77,84,39,59,45,105,110,107,115,99,97,112,101,45,102,111,110,116,45,115,112,101,99,105,102,105,99,97,116,105,111,110,58,39,80,97,108,97,99,101,32,83,99,114,105,112,116,32,77,84,32,79,98,108,105,
113,117,101,39,59,116,101,120,116,45,97,108,105,103,110,58,115,116,97,114,116,59,119,114,105,116,105,110,103,45,109,111,100,101,58,108,114,45,116,98,59,116,101,120,116,45,97,110,99,104,111,114,58,115,
116,97,114,116,59,102,105,108,108,58,35,102,57,102,57,102,57,34,10,32,32,32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,51,55,52,57,34,32,47,62,10,32,32,32,32,60,47,103,62,10,32,32,60,47,103,62,10,
60,47,115,118,103,62,10,0,0};

const char* VASTVaporizerComponent::dynamics_svg = (const char*) resource_VASTVaporizerComponent_dynamics_svg;
const int VASTVaporizerComponent::dynamics_svgSize = 13088;

// JUCER_RESOURCE: copyright_svg, 78864, "../../Resources/copyright.svg"
static const unsigned char resource_VASTVaporizerComponent_copyright_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,70,45,56,
34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,33,45,45,32,67,114,101,97,116,101,100,32,119,105,116,104,32,73,110,107,115,99,97,112,101,32,40,104,116,116,112,58,47,47,119,119,
119,46,105,110,107,115,99,97,112,101,46,111,114,103,47,41,32,45,45,62,10,10,60,115,118,103,10,32,32,32,120,109,108,110,115,58,100,99,61,34,104,116,116,112,58,47,47,112,117,114,108,46,111,114,103,47,100,
99,47,101,108,101,109,101,110,116,115,47,49,46,49,47,34,10,32,32,32,120,109,108,110,115,58,99,99,61,34,104,116,116,112,58,47,47,99,114,101,97,116,105,118,101,99,111,109,109,111,110,115,46,111,114,103,
47,110,115,35,34,10,32,32,32,120,109,108,110,115,58,114,100,102,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,48,50,47,50,50,45,114,100,102,45,115,121,110,116,97,
120,45,110,115,35,34,10,32,32,32,120,109,108,110,115,58,115,118,103,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,118,103,34,10,32,32,32,120,109,108,110,115,
61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,118,103,34,10,32,32,32,120,109,108,110,115,58,115,111,100,105,112,111,100,105,61,34,104,116,116,112,58,47,47,115,
111,100,105,112,111,100,105,46,115,111,117,114,99,101,102,111,114,103,101,46,110,101,116,47,68,84,68,47,115,111,100,105,112,111,100,105,45,48,46,100,116,100,34,10,32,32,32,120,109,108,110,115,58,105,110,
107,115,99,97,112,101,61,34,104,116,116,112,58,47,47,119,119,119,46,105,110,107,115,99,97,112,101,46,111,114,103,47,110,97,109,101,115,112,97,99,101,115,47,105,110,107,115,99,97,112,101,34,10,32,32,32,
119,105,100,116,104,61,34,50,49,48,109,109,34,10,32,32,32,104,101,105,103,104,116,61,34,50,57,55,109,109,34,10,32,32,32,118,105,101,119,66,111,120,61,34,48,32,48,32,50,49,48,32,50,57,55,34,10,32,32,32,
118,101,114,115,105,111,110,61,34,49,46,49,34,10,32,32,32,105,100,61,34,115,118,103,51,55,53,56,34,10,32,32,32,105,110,107,115,99,97,112,101,58,118,101,114,115,105,111,110,61,34,48,46,57,50,46,51,32,40,
50,52,48,53,53,52,54,44,32,50,48,49,56,45,48,51,45,49,49,41,34,10,32,32,32,115,111,100,105,112,111,100,105,58,100,111,99,110,97,109,101,61,34,99,111,112,121,114,105,103,104,116,46,115,118,103,34,62,10,
32,32,60,100,101,102,115,10,32,32,32,32,32,105,100,61,34,100,101,102,115,51,55,53,50,34,62,10,32,32,32,32,60,112,97,116,116,101,114,110,10,32,32,32,32,32,32,32,121,61,34,48,34,10,32,32,32,32,32,32,32,
120,61,34,48,34,10,32,32,32,32,32,32,32,104,101,105,103,104,116,61,34,54,34,10,32,32,32,32,32,32,32,119,105,100,116,104,61,34,54,34,10,32,32,32,32,32,32,32,112,97,116,116,101,114,110,85,110,105,116,115,
61,34,117,115,101,114,83,112,97,99,101,79,110,85,115,101,34,10,32,32,32,32,32,32,32,105,100,61,34,69,77,70,104,98,97,115,101,112,97,116,116,101,114,110,34,32,47,62,10,32,32,32,32,60,112,97,116,116,101,
114,110,10,32,32,32,32,32,32,32,121,61,34,48,34,10,32,32,32,32,32,32,32,120,61,34,48,34,10,32,32,32,32,32,32,32,104,101,105,103,104,116,61,34,54,34,10,32,32,32,32,32,32,32,119,105,100,116,104,61,34,54,
34,10,32,32,32,32,32,32,32,112,97,116,116,101,114,110,85,110,105,116,115,61,34,117,115,101,114,83,112,97,99,101,79,110,85,115,101,34,10,32,32,32,32,32,32,32,105,100,61,34,69,77,70,104,98,97,115,101,112,
97,116,116,101,114,110,45,57,34,32,47,62,10,32,32,60,47,100,101,102,115,62,10,32,32,60,115,111,100,105,112,111,100,105,58,110,97,109,101,100,118,105,101,119,10,32,32,32,32,32,105,100,61,34,98,97,115,101,
34,10,32,32,32,32,32,112,97,103,101,99,111,108,111,114,61,34,35,102,102,102,102,102,102,34,10,32,32,32,32,32,98,111,114,100,101,114,99,111,108,111,114,61,34,35,54,54,54,54,54,54,34,10,32,32,32,32,32,98,
111,114,100,101,114,111,112,97,99,105,116,121,61,34,49,46,48,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,112,97,103,101,111,112,97,99,105,116,121,61,34,48,46,48,34,10,32,32,32,32,32,105,110,
107,115,99,97,112,101,58,112,97,103,101,115,104,97,100,111,119,61,34,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,122,111,111,109,61,34,51,46,57,53,57,55,57,56,34,10,32,32,32,32,32,105,110,
107,115,99,97,112,101,58,99,120,61,34,52,49,48,46,56,56,54,50,57,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,121,61,34,54,53,53,46,49,53,56,56,49,34,10,32,32,32,32,32,105,110,107,115,99,97,
112,101,58,100,111,99,117,109,101,110,116,45,117,110,105,116,115,61,34,109,109,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,117,114,114,101,110,116,45,108,97,121,101,114,61,34,108,97,121,101,
114,49,34,10,32,32,32,32,32,115,104,111,119,103,114,105,100,61,34,102,97,108,115,101,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,119,105,100,116,104,61,34,53,52,51,
49,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,104,101,105,103,104,116,61,34,50,56,54,56,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,
119,45,120,61,34,45,49,56,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,121,61,34,45,49,56,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,
119,45,109,97,120,105,109,105,122,101,100,61,34,49,34,32,47,62,10,32,32,60,109,101,116,97,100,97,116,97,10,32,32,32,32,32,105,100,61,34,109,101,116,97,100,97,116,97,51,55,53,53,34,62,10,32,32,32,32,60,
114,100,102,58,82,68,70,62,10,32,32,32,32,32,32,60,99,99,58,87,111,114,107,10,32,32,32,32,32,32,32,32,32,114,100,102,58,97,98,111,117,116,61,34,34,62,10,32,32,32,32,32,32,32,32,60,100,99,58,102,111,114,
109,97,116,62,105,109,97,103,101,47,115,118,103,43,120,109,108,60,47,100,99,58,102,111,114,109,97,116,62,10,32,32,32,32,32,32,32,32,60,100,99,58,116,121,112,101,10,32,32,32,32,32,32,32,32,32,32,32,114,
100,102,58,114,101,115,111,117,114,99,101,61,34,104,116,116,112,58,47,47,112,117,114,108,46,111,114,103,47,100,99,47,100,99,109,105,116,121,112,101,47,83,116,105,108,108,73,109,97,103,101,34,32,47,62,
10,32,32,32,32,32,32,32,32,60,100,99,58,116,105,116,108,101,62,60,47,100,99,58,116,105,116,108,101,62,10,32,32,32,32,32,32,60,47,99,99,58,87,111,114,107,62,10,32,32,32,32,60,47,114,100,102,58,82,68,70,
62,10,32,32,60,47,109,101,116,97,100,97,116,97,62,10,32,32,60,103,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,108,97,98,101,108,61,34,69,98,101,110,101,32,49,34,10,32,32,32,32,32,105,110,107,115,
99,97,112,101,58,103,114,111,117,112,109,111,100,101,61,34,108,97,121,101,114,34,10,32,32,32,32,32,105,100,61,34,108,97,121,101,114,49,34,62,10,32,32,32,32,60,116,101,120,116,10,32,32,32,32,32,32,32,120,
109,108,58,115,112,97,99,101,61,34,112,114,101,115,101,114,118,101,34,10,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,111,110,116,45,115,116,121,108,101,58,110,111,114,109,97,108,59,102,111,110,
116,45,119,101,105,103,104,116,58,110,111,114,109,97,108,59,102,111,110,116,45,115,105,122,101,58,49,48,46,53,56,51,51,51,51,48,50,112,120,59,108,105,110,101,45,104,101,105,103,104,116,58,49,46,50,53,
59,102,111,110,116,45,102,97,109,105,108,121,58,115,97,110,115,45,115,101,114,105,102,59,108,101,116,116,101,114,45,115,112,97,99,105,110,103,58,48,112,120,59,119,111,114,100,45,115,112,97,99,105,110,
103,58,48,112,120,59,102,105,108,108,58,35,48,48,48,48,48,48,59,102,105,108,108,45,111,112,97,99,105,116,121,58,49,59,115,116,114,111,107,101,58,110,111,110,101,59,115,116,114,111,107,101,45,119,105,100,
116,104,58,48,46,50,54,52,53,56,51,51,50,34,10,32,32,32,32,32,32,32,120,61,34,52,55,46,48,53,56,48,51,55,34,10,32,32,32,32,32,32,32,121,61,34,49,50,57,46,53,53,54,53,51,34,10,32,32,32,32,32,32,32,105,
100,61,34,116,101,120,116,51,55,55,48,34,62,60,116,115,112,97,110,10,32,32,32,32,32,32,32,32,32,115,111,100,105,112,111,100,105,58,114,111,108,101,61,34,108,105,110,101,34,10,32,32,32,32,32,32,32,32,32,
105,100,61,34,116,115,112,97,110,51,55,54,56,34,10,32,32,32,32,32,32,32,32,32,120,61,34,52,55,46,48,53,56,48,51,55,34,10,32,32,32,32,32,32,32,32,32,121,61,34,49,51,57,46,50,49,50,48,52,34,10,32,32,32,
32,32,32,32,32,32,115,116,121,108,101,61,34,115,116,114,111,107,101,45,119,105,100,116,104,58,48,46,50,54,52,53,56,51,51,50,34,32,47,62,60,47,116,101,120,116,62,10,32,32,32,32,60,116,101,120,116,10,32,
32,32,32,32,32,32,105,100,61,34,116,101,120,116,51,55,56,53,34,10,32,32,32,32,32,32,32,121,61,34,49,51,55,46,54,55,48,51,51,34,10,32,32,32,32,32,32,32,120,61,34,49,50,49,46,50,52,52,55,54,34,10,32,32,
32,32,32,32,32,115,116,121,108,101,61,34,102,111,110,116,45,115,116,121,108,101,58,110,111,114,109,97,108,59,102,111,110,116,45,118,97,114,105,97,110,116,58,110,111,114,109,97,108,59,102,111,110,116,45,
119,101,105,103,104,116,58,52,48,48,59,102,111,110,116,45,115,105,122,101,58,51,46,56,56,54,48,54,55,54,51,112,120,59,108,105,110,101,45,104,101,105,103,104,116,58,49,50,53,37,59,102,111,110,116,45,102,
97,109,105,108,121,58,67,97,108,105,98,114,105,59,116,101,120,116,45,97,108,105,103,110,58,115,116,97,114,116,59,108,101,116,116,101,114,45,115,112,97,99,105,110,103,58,48,112,120,59,119,111,114,100,45,
115,112,97,99,105,110,103,58,48,112,120,59,116,101,120,116,45,97,110,99,104,111,114,58,115,116,97,114,116,59,102,105,108,108,58,35,48,48,48,48,48,48,59,102,105,108,108,45,111,112,97,99,105,116,121,58,
49,59,115,116,114,111,107,101,58,110,111,110,101,59,115,116,114,111,107,101,45,119,105,100,116,104,58,48,46,50,54,52,53,56,51,51,50,34,10,32,32,32,32,32,32,32,120,109,108,58,115,112,97,99,101,61,34,112,
114,101,115,101,114,118,101,34,62,60,116,115,112,97,110,10,32,32,32,32,32,32,32,32,32,105,100,61,34,116,115,112,97,110,51,55,56,51,34,10,32,32,32,32,32,32,32,32,32,121,61,34,49,51,55,46,54,55,48,51,51,
34,10,32,32,32,32,32,32,32,32,32,120,61,34,49,50,49,46,50,52,52,55,54,34,10,32,32,32,32,32,32,32,32,32,115,111,100,105,112,111,100,105,58,114,111,108,101,61,34,108,105,110,101,34,10,32,32,32,32,32,32,
32,32,32,115,116,121,108,101,61,34,115,116,114,111,107,101,45,119,105,100,116,104,58,48,46,50,54,52,53,56,51,51,50,34,62,60,116,115,112,97,110,10,32,32,32,32,32,32,32,32,32,32,32,105,100,61,34,116,115,
112,97,110,51,55,55,57,34,10,32,32,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,111,110,116,45,115,116,121,108,101,58,110,111,114,109,97,108,59,102,111,110,116,45,118,97,114,105,97,110,116,
58,110,111,114,109,97,108,59,102,111,110,116,45,119,101,105,103,104,116,58,52,48,48,59,102,111,110,116,45,115,105,122,101,58,51,46,56,56,54,48,54,55,54,51,112,120,59,102,111,110,116,45,102,97,109,105,
108,121,58,67,97,108,105,98,114,105,59,102,105,108,108,58,35,48,48,48,48,48,48,59,115,116,114,111,107,101,45,119,105,100,116,104,58,48,46,50,54,52,53,56,51,51,50,34,10,32,32,32,32,32,32,32,32,32,32,32,
100,121,61,34,48,34,10,32,32,32,32,32,32,32,32,32,32,32,100,120,61,34,48,34,62,194,169,60,47,116,115,112,97,110,62,60,116,115,112,97,110,10,32,32,32,32,32,32,32,32,32,32,32,105,100,61,34,116,115,112,97,
110,51,55,56,49,34,10,32,32,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,111,110,116,45,115,116,121,108,101,58,110,111,114,109,97,108,59,102,111,110,116,45,118,97,114,105,97,110,116,58,110,
111,114,109,97,108,59,102,111,110,116,45,119,101,105,103,104,116,58,52,48,48,59,102,111,110,116,45,115,105,122,101,58,51,46,56,56,54,48,54,55,54,51,112,120,59,102,111,110,116,45,102,97,109,105,108,121,
58,67,97,108,105,98,114,105,59,102,105,108,108,58,35,48,48,48,48,48,48,59,115,116,114,111,107,101,45,119,105,100,116,104,58,48,46,50,54,52,53,56,51,51,50,34,10,32,32,32,32,32,32,32,32,32,32,32,100,121,
61,34,48,34,10,32,32,32,32,32,32,32,32,32,32,32,100,120,61,34,48,34,62,32,60,47,116,115,112,97,110,62,60,47,116,115,112,97,110,62,60,47,116,101,120,116,62,10,32,32,32,32,60,103,10,32,32,32,32,32,32,32,
105,100,61,34,103,51,56,57,57,34,62,10,32,32,32,32,32,32,60,103,10,32,32,32,32,32,32,32,32,32,105,100,61,34,116,101,120,116,51,55,55,52,34,10,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,111,
110,116,45,115,116,121,108,101,58,110,111,114,109,97,108,59,102,111,110,116,45,119,101,105,103,104,116,58,110,111,114,109,97,108,59,102,111,110,116,45,115,105,122,101,58,49,48,46,53,56,51,51,51,51,48,
50,112,120,59,108,105,110,101,45,104,101,105,103,104,116,58,49,46,50,53,59,102,111,110,116,45,102,97,109,105,108,121,58,115,97,110,115,45,115,101,114,105,102,59,108,101,116,116,101,114,45,115,112,97,99,
105,110,103,58,48,112,120,59,119,111,114,100,45,115,112,97,99,105,110,103,58,48,112,120,59,102,105,108,108,58,35,102,50,102,50,102,50,59,102,105,108,108,45,111,112,97,99,105,116,121,58,49,59,115,116,114,
111,107,101,58,110,111,110,101,59,115,116,114,111,107,101,45,119,105,100,116,104,58,48,46,50,54,52,53,56,51,51,50,34,10,32,32,32,32,32,32,32,32,32,97,114,105,97,45,108,97,98,101,108,61,34,86,65,83,84,
32,68,121,110,97,109,105,99,115,32,45,32,84,111,98,105,97,115,32,83,116,101,105,110,34,62,10,32,32,32,32,32,32,32,32,60,112,97,116,104,10,32,32,32,32,32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,
51,56,50,51,34,10,32,32,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,111,110,116,45,115,116,121,108,101,58,110,111,114,109,97,108,59,102,111,110,116,45,118,97,114,105,97,110,116,58,110,111,
114,109,97,108,59,102,111,110,116,45,119,101,105,103,104,116,58,110,111,114,109,97,108,59,102,111,110,116,45,115,116,114,101,116,99,104,58,110,111,114,109,97,108,59,102,111,110,116,45,102,97,109,105,108,
121,58,39,65,108,116,101,32,72,97,97,115,32,71,114,111,116,101,115,107,39,59,45,105,110,107,115,99,97,112,101,45,102,111,110,116,45,115,112,101,99,105,102,105,99,97,116,105,111,110,58,39,65,108,116,101,
32,72,97,97,115,32,71,114,111,116,101,115,107,39,59,102,105,108,108,58,35,102,50,102,50,102,50,59,115,116,114,111,107,101,45,119,105,100,116,104,58,48,46,50,54,52,53,56,51,51,50,34,10,32,32,32,32,32,32,
32,32,32,32,32,100,61,34,109,32,52,55,46,52,48,55,50,56,55,44,49,48,51,46,53,48,48,51,56,32,113,32,48,46,49,54,57,51,51,51,44,48,32,48,46,51,55,48,52,49,54,44,48,32,48,46,50,48,49,48,56,52,44,45,48,46,
48,49,48,54,32,48,46,51,48,54,57,49,55,44,48,46,48,53,50,57,32,48,46,48,57,53,50,53,44,48,46,48,54,51,53,32,48,46,49,50,55,44,48,46,50,48,49,48,56,32,48,46,48,52,50,51,51,44,48,46,49,51,55,53,56,32,48,
46,48,56,52,54,55,44,48,46,50,53,52,32,48,46,48,56,52,54,55,44,48,46,50,49,49,54,55,32,48,46,49,52,56,49,54,54,44,48,46,52,51,51,57,50,32,48,46,48,54,51,53,44,48,46,50,49,49,54,54,32,48,46,49,52,56,49,
54,55,44,48,46,52,50,51,51,51,32,48,46,48,50,49,49,55,44,48,46,48,53,50,57,32,48,46,48,51,49,55,53,44,48,46,49,48,53,56,51,32,48,46,48,49,48,53,56,44,48,46,48,53,50,57,32,48,46,48,50,49,49,55,44,48,46,
49,48,53,56,52,32,48,46,48,55,52,48,56,44,48,46,49,55,57,57,49,32,48,46,49,50,55,44,48,46,51,56,49,32,48,46,48,54,51,53,44,48,46,49,57,48,53,32,48,46,49,50,55,44,48,46,51,57,49,53,56,32,48,46,49,52,56,
49,54,54,44,48,46,52,51,51,57,50,32,48,46,50,56,53,55,53,44,48,46,56,53,55,50,53,32,48,46,49,52,56,49,54,54,44,48,46,52,50,51,51,51,32,48,46,50,57,54,51,51,51,44,48,46,56,53,55,50,53,32,48,46,48,55,52,
48,56,44,48,46,50,49,49,54,55,32,48,46,49,50,55,44,48,46,52,51,51,57,50,32,48,46,48,54,51,53,44,48,46,50,49,49,54,54,32,48,46,49,52,56,49,54,55,44,48,46,52,50,51,51,51,32,48,46,48,50,49,49,55,44,48,46,
48,53,50,57,32,48,46,48,51,49,55,53,44,48,46,49,49,54,52,50,32,48,46,48,50,49,49,55,44,48,46,48,54,51,53,32,48,46,48,52,50,51,51,44,48,46,49,50,55,32,48,46,49,49,54,52,49,55,44,48,46,51,48,54,57,49,32,
48,46,50,49,49,54,54,55,44,48,46,54,51,53,32,48,46,48,52,50,51,51,44,48,46,49,48,53,56,51,32,48,46,48,55,52,48,56,44,48,46,50,50,50,50,53,32,48,46,48,51,49,55,53,44,48,46,49,48,53,56,51,32,48,46,48,57,
53,50,53,44,48,46,49,57,48,53,32,48,46,48,50,49,49,55,44,48,46,48,50,49,50,32,48,46,48,55,52,48,56,44,48,46,48,54,51,53,32,48,46,49,50,55,44,45,48,46,48,49,48,54,32,48,46,49,53,56,55,53,44,45,48,46,49,
48,53,56,52,32,48,46,48,51,49,55,53,44,45,48,46,49,48,53,56,51,32,48,46,48,55,52,48,56,44,45,48,46,50,48,49,48,56,32,48,46,49,49,54,52,49,54,44,45,48,46,50,55,53,49,55,32,48,46,49,57,48,53,44,45,48,46,
53,54,48,57,50,32,48,46,48,55,52,48,56,44,45,48,46,50,56,53,55,53,32,48,46,49,55,57,57,49,54,44,45,48,46,53,54,48,57,49,32,48,46,48,54,51,53,44,45,48,46,49,54,57,51,52,32,48,46,49,48,53,56,51,52,44,45,
48,46,51,51,56,54,55,32,48,46,48,53,50,57,50,44,45,48,46,49,54,57,51,51,32,48,46,49,49,54,52,49,54,44,45,48,46,51,51,56,54,55,32,48,46,48,53,50,57,50,44,45,48,46,49,50,55,32,48,46,48,56,52,54,55,44,45,
48,46,50,53,52,32,48,46,48,52,50,51,51,44,45,48,46,49,51,55,53,56,32,48,46,48,57,53,50,53,44,45,48,46,50,54,52,53,56,32,48,46,49,53,56,55,53,44,45,48,46,52,52,52,53,32,48,46,50,57,54,51,51,51,44,45,48,
46,56,57,57,53,56,32,48,46,49,51,55,53,56,52,44,45,48,46,52,53,53,48,57,32,48,46,51,48,54,57,49,55,44,45,48,46,56,57,57,53,57,32,48,46,48,50,49,49,55,44,45,48,46,48,52,50,51,32,48,46,48,50,49,49,55,44,
45,48,46,48,55,52,49,32,48,44,45,48,46,48,52,50,51,32,48,46,48,50,49,49,55,44,45,48,46,48,56,52,55,32,48,46,48,54,51,53,44,45,48,46,49,53,56,55,53,32,48,46,49,49,54,52,49,55,44,45,48,46,51,50,56,48,56,
32,48,46,48,53,50,57,50,44,45,48,46,49,54,57,51,51,32,48,46,49,48,53,56,51,51,44,45,48,46,51,50,56,48,56,32,48,46,48,54,51,53,44,45,48,46,49,53,56,55,53,32,48,46,49,48,53,56,51,52,44,45,48,46,51,49,55,
53,32,48,46,48,53,50,57,50,44,45,48,46,49,54,57,51,52,32,48,46,49,49,54,52,49,54,44,45,48,46,51,51,56,54,55,32,48,46,48,51,49,55,53,44,45,48,46,48,57,53,50,32,48,46,48,54,51,53,44,45,48,46,49,55,57,57,
50,32,48,46,48,52,50,51,51,44,45,48,46,48,57,53,50,32,48,46,49,50,55,44,45,48,46,49,52,56,49,54,32,48,46,48,54,51,53,44,45,48,46,48,53,50,57,32,48,46,49,54,57,51,51,52,44,45,48,46,48,53,50,57,32,48,46,
49,49,54,52,49,54,44,48,32,48,46,50,51,50,56,51,51,44,48,32,104,32,48,46,50,51,50,56,51,51,32,113,32,48,46,48,56,52,54,55,44,48,46,48,50,49,50,32,48,46,49,52,56,49,54,55,44,48,46,48,52,50,51,32,48,46,
48,54,51,53,44,48,46,48,49,48,54,32,48,46,48,56,52,54,55,44,48,46,48,57,53,51,32,48,44,48,46,48,50,49,50,32,48,44,48,46,48,53,50,57,32,48,46,48,49,48,53,56,44,48,46,48,50,49,50,32,48,44,48,46,48,52,50,
51,32,45,48,46,48,50,49,49,55,44,48,46,49,48,53,56,52,32,45,48,46,48,54,51,53,44,48,46,50,49,49,54,55,32,45,48,46,48,51,49,55,53,44,48,46,48,57,53,51,32,45,48,46,48,54,51,53,44,48,46,49,55,57,57,50,32,
45,48,46,48,52,50,51,51,44,48,46,48,56,52,55,32,45,48,46,48,55,52,48,56,44,48,46,49,55,57,57,49,32,45,48,46,48,50,49,49,55,44,48,46,48,56,52,55,32,45,48,46,48,53,50,57,50,44,48,46,49,54,57,51,52,32,45,
48,46,48,54,51,53,44,48,46,49,52,56,49,54,32,45,48,46,49,49,54,52,49,55,44,48,46,51,49,55,53,32,45,48,46,48,52,50,51,51,44,48,46,49,53,56,55,53,32,45,48,46,49,48,53,56,51,51,44,48,46,51,48,54,57,49,32,
45,48,46,48,50,49,49,55,44,48,46,48,52,50,51,32,45,48,46,48,50,49,49,55,44,48,46,48,55,52,49,32,48,44,48,46,48,51,49,56,32,45,48,46,48,50,49,49,55,44,48,46,48,55,52,49,32,45,48,46,48,56,52,54,55,44,48,
46,49,55,57,57,50,32,45,48,46,49,52,56,49,54,54,44,48,46,51,56,49,32,45,48,46,48,53,50,57,50,44,48,46,50,48,49,48,56,32,45,48,46,49,51,55,53,56,52,44,48,46,51,56,49,32,45,48,46,48,50,49,49,55,44,48,46,
48,52,50,51,32,45,48,46,48,50,49,49,55,44,48,46,48,56,52,55,32,48,44,48,46,48,51,49,56,32,45,48,46,48,50,49,49,55,44,48,46,48,54,51,53,32,45,48,46,48,53,50,57,50,44,48,46,49,51,55,53,56,32,45,48,46,49,
48,53,56,51,51,44,48,46,50,56,53,55,53,32,45,48,46,48,52,50,51,51,44,48,46,49,51,55,53,56,32,45,48,46,48,57,53,50,53,44,48,46,50,55,53,49,54,32,45,48,46,48,50,49,49,55,44,48,46,48,51,49,56,32,45,48,46,
48,51,49,55,53,44,48,46,48,55,52,49,32,48,44,48,46,48,51,49,56,32,45,48,46,48,50,49,49,55,44,48,46,48,55,52,49,32,45,48,46,49,53,56,55,53,44,48,46,52,48,50,49,55,32,45,48,46,50,57,54,51,51,51,44,48,46,
56,49,52,57,50,32,45,48,46,49,50,55,44,48,46,52,49,50,55,53,32,45,48,46,50,56,53,55,53,44,48,46,56,49,52,57,49,32,108,32,45,48,46,48,51,49,55,53,44,48,46,48,57,53,50,32,113,32,45,48,46,48,52,50,51,51,
44,48,46,49,49,54,52,50,32,45,48,46,48,56,52,54,55,44,48,46,50,51,50,56,52,32,45,48,46,48,51,49,55,53,44,48,46,49,48,53,56,51,32,45,48,46,48,55,52,48,56,44,48,46,50,50,50,50,53,32,45,48,46,48,53,50,57,
50,44,48,46,49,50,55,32,45,48,46,48,57,53,50,53,44,48,46,50,54,52,53,56,32,45,48,46,48,52,50,51,51,44,48,46,49,50,55,32,45,48,46,48,57,53,50,53,44,48,46,50,53,52,32,45,48,46,48,50,49,49,55,44,48,46,48,
54,51,53,32,45,48,46,48,52,50,51,51,44,48,46,49,50,55,32,45,48,46,48,50,49,49,55,44,48,46,48,54,51,53,32,45,48,46,48,52,50,51,51,44,48,46,49,50,55,32,45,48,46,48,54,51,53,44,48,46,49,53,56,55,53,32,45,
48,46,49,49,54,52,49,55,44,48,46,51,50,56,48,56,32,45,48,46,48,53,50,57,50,44,48,46,49,54,57,51,52,32,45,48,46,49,49,54,52,49,54,44,48,46,51,49,55,53,32,45,48,46,48,53,50,57,50,44,48,46,49,51,55,53,57,
32,45,48,46,49,48,53,56,51,52,44,48,46,50,57,54,51,52,32,45,48,46,48,52,50,51,51,44,48,46,49,52,56,49,54,32,45,48,46,49,52,56,49,54,54,44,48,46,50,50,50,50,53,32,45,48,46,48,52,50,51,51,44,48,46,48,50,
49,50,32,45,48,46,48,57,53,50,53,44,48,46,48,51,49,56,32,45,48,46,48,53,50,57,50,44,48,46,48,49,48,54,32,45,48,46,49,49,54,52,49,55,44,48,46,48,50,49,50,32,104,32,45,48,46,50,57,54,51,51,51,32,113,32,
45,48,46,49,50,55,44,48,32,45,48,46,50,55,53,49,54,55,44,48,32,45,48,46,49,52,56,49,54,55,44,48,46,48,49,48,54,32,45,48,46,50,51,50,56,51,51,44,45,48,46,48,52,50,51,32,45,48,46,48,57,53,50,53,44,45,48,
46,48,53,50,57,32,45,48,46,49,50,55,44,45,48,46,49,52,56,49,55,32,45,48,46,48,51,49,55,53,44,45,48,46,49,48,53,56,51,32,45,48,46,48,56,52,54,55,44,45,48,46,50,50,50,50,53,32,45,48,46,48,54,51,53,44,45,
48,46,49,53,56,55,53,32,45,48,46,49,49,54,52,49,55,44,45,48,46,51,50,56,48,56,32,45,48,46,48,53,50,57,50,44,45,48,46,49,55,57,57,50,32,45,48,46,49,50,55,44,45,48,46,51,52,57,50,53,32,45,48,46,48,50,49,
49,55,44,45,48,46,48,52,50,51,32,45,48,46,48,51,49,55,53,44,45,48,46,48,56,52,55,32,48,44,45,48,46,48,52,50,51,32,45,48,46,48,50,49,49,55,44,45,48,46,48,56,52,55,32,45,48,46,48,56,52,54,55,44,45,48,46,
50,49,49,54,55,32,45,48,46,49,53,56,55,53,44,45,48,46,52,50,51,51,52,32,45,48,46,48,54,51,53,44,45,48,46,50,50,50,50,53,32,45,48,46,49,52,56,49,54,55,44,45,48,46,52,50,51,51,51,32,45,48,46,48,50,49,49,
55,44,45,48,46,48,53,50,57,32,45,48,46,48,51,49,55,53,44,45,48,46,48,57,53,51,32,48,44,45,48,46,48,52,50,51,32,45,48,46,48,50,49,49,55,44,45,48,46,48,56,52,55,32,45,48,46,48,53,50,57,50,44,45,48,46,49,
52,56,49,54,32,45,48,46,49,48,53,56,51,51,44,45,48,46,50,56,53,55,53,32,45,48,46,48,53,50,57,50,44,45,48,46,49,52,56,49,54,32,45,48,46,49,48,53,56,51,51,44,45,48,46,50,57,54,51,51,32,45,48,46,48,50,49,
49,55,44,45,48,46,48,52,50,51,32,45,48,46,48,51,49,55,53,44,45,48,46,48,56,52,55,32,48,44,45,48,46,48,52,50,51,32,45,48,46,48,50,49,49,55,44,45,48,46,48,56,52,55,32,45,48,46,48,53,50,57,50,44,45,48,46,
49,51,55,53,57,32,45,48,46,49,48,53,56,51,51,44,45,48,46,50,55,53,49,55,32,45,48,46,48,52,50,51,51,44,45,48,46,49,52,56,49,55,32,45,48,46,48,57,53,50,53,44,45,48,46,50,57,54,51,51,32,45,48,46,48,53,50,
57,50,44,45,48,46,49,49,54,52,50,32,45,48,46,48,56,52,54,55,44,45,48,46,50,51,50,56,52,32,45,48,46,48,51,49,55,53,44,45,48,46,49,50,55,32,45,48,46,48,56,52,54,55,44,45,48,46,50,52,51,52,49,32,45,48,46,
49,49,54,52,49,54,44,45,48,46,50,57,54,51,52,32,45,48,46,50,49,49,54,54,54,44,45,48,46,54,48,51,50,53,32,45,48,46,48,57,53,50,53,44,45,48,46,51,49,55,53,32,45,48,46,50,50,50,50,53,44,45,48,46,54,49,51,
56,52,32,45,48,46,48,52,50,51,51,44,45,48,46,48,57,53,50,32,45,48,46,48,55,52,48,56,44,45,48,46,49,57,48,53,32,45,48,46,48,50,49,49,55,44,45,48,46,49,48,53,56,51,32,45,48,46,48,53,50,57,50,44,45,48,46,
49,57,48,53,32,45,48,46,48,51,49,55,53,44,45,48,46,48,54,51,53,32,45,48,46,48,53,50,57,50,44,45,48,46,49,50,55,32,45,48,46,48,49,48,53,56,44,45,48,46,48,55,52,49,32,45,48,46,48,52,50,51,51,44,45,48,46,
49,51,55,53,56,32,45,48,46,48,57,53,50,53,44,45,48,46,50,52,51,52,50,32,45,48,46,49,55,57,57,49,55,44,45,48,46,52,57,55,52,50,32,45,48,46,48,55,52,48,56,44,45,48,46,50,53,52,32,45,48,46,49,54,57,51,51,
51,44,45,48,46,52,57,55,52,49,32,45,48,46,48,50,49,49,55,44,45,48,46,48,55,52,49,32,45,48,46,48,53,50,57,50,44,45,48,46,49,52,56,49,55,32,45,48,46,48,50,49,49,55,44,45,48,46,48,55,52,49,32,45,48,46,48,
52,50,51,51,44,45,48,46,49,53,56,55,53,32,45,48,46,48,52,50,51,51,44,45,48,46,49,48,53,56,51,32,45,48,46,48,49,48,53,56,44,45,48,46,49,55,57,57,50,32,48,46,48,51,49,55,53,44,45,48,46,48,55,52,49,32,48,
46,49,52,56,49,54,55,44,45,48,46,48,57,53,51,32,48,46,48,50,49,49,55,44,45,48,46,48,49,48,54,32,48,46,48,52,50,51,51,44,48,32,48,46,48,50,49,49,55,44,48,32,48,46,48,52,50,51,51,44,45,48,46,48,49,48,54,
32,122,34,32,47,62,10,32,32,32,32,32,32,32,32,60,112,97,116,104,10,32,32,32,32,32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,51,56,50,53,34,10,32,32,32,32,32,32,32,32,32,32,32,115,116,121,108,101,
61,34,102,111,110,116,45,115,116,121,108,101,58,110,111,114,109,97,108,59,102,111,110,116,45,118,97,114,105,97,110,116,58,110,111,114,109,97,108,59,102,111,110,116,45,119,101,105,103,104,116,58,110,111,
114,109,97,108,59,102,111,110,116,45,115,116,114,101,116,99,104,58,110,111,114,109,97,108,59,102,111,110,116,45,102,97,109,105,108,121,58,39,65,108,116,101,32,72,97,97,115,32,71,114,111,116,101,115,107,
39,59,45,105,110,107,115,99,97,112,101,45,102,111,110,116,45,115,112,101,99,105,102,105,99,97,116,105,111,110,58,39,65,108,116,101,32,72,97,97,115,32,71,114,111,116,101,115,107,39,59,102,105,108,108,58,
35,102,50,102,50,102,50,59,115,116,114,111,107,101,45,119,105,100,116,104,58,48,46,50,54,52,53,56,51,51,50,34,10,32,32,32,32,32,32,32,32,32,32,32,100,61,34,109,32,54,48,46,50,52,56,51,52,50,44,49,49,48,
46,52,55,52,56,32,113,32,48,46,48,50,49,49,55,44,48,46,48,53,50,57,32,48,46,48,53,50,57,50,44,48,46,49,50,55,32,48,46,48,51,49,55,53,44,48,46,48,55,52,49,32,48,46,48,52,50,51,51,44,48,46,49,53,56,55,53,
32,48,46,48,50,49,49,55,44,48,46,48,55,52,49,32,48,44,48,46,49,52,56,49,54,32,45,48,46,48,49,48,53,56,44,48,46,48,54,51,53,32,45,48,46,48,56,52,54,55,44,48,46,48,57,53,51,32,45,48,46,48,53,50,57,50,44,
48,46,48,50,49,50,32,45,48,46,49,50,55,44,48,46,48,50,49,50,32,45,48,46,48,55,52,48,56,44,48,32,45,48,46,49,52,56,49,54,55,44,48,32,45,48,46,49,52,56,49,54,55,44,48,32,45,48,46,51,49,55,53,44,48,32,45,
48,46,49,53,56,55,53,44,48,32,45,48,46,50,53,52,44,45,48,46,48,53,50,57,32,45,48,46,49,48,53,56,51,51,44,45,48,46,48,54,51,53,32,45,48,46,49,53,56,55,53,44,45,48,46,50,50,50,50,53,32,45,48,46,48,52,50,
51,51,44,45,48,46,49,53,56,55,53,32,45,48,46,48,57,53,50,53,44,45,48,46,50,56,53,55,53,32,45,48,46,49,49,54,52,49,55,44,45,48,46,50,55,53,49,54,32,45,48,46,50,49,49,54,54,55,44,45,48,46,53,55,49,53,32,
45,48,46,48,57,53,50,53,44,45,48,46,50,57,54,51,51,32,45,48,46,50,49,49,54,54,54,44,45,48,46,53,56,50,48,56,32,45,48,46,48,51,49,55,53,44,45,48,46,48,57,53,51,32,45,48,46,48,54,51,53,44,45,48,46,49,55,
57,57,50,32,45,48,46,48,50,49,49,55,44,45,48,46,48,56,52,55,32,45,48,46,48,54,51,53,44,45,48,46,49,53,56,55,53,32,45,48,46,48,51,49,55,53,44,45,48,46,48,55,52,49,32,45,48,46,48,57,53,50,53,44,45,48,46,
49,50,55,32,45,48,46,48,54,51,53,44,45,48,46,48,53,50,57,32,45,48,46,49,54,57,51,51,52,44,45,48,46,48,56,52,55,32,45,48,46,48,53,50,57,50,44,45,48,46,48,50,49,50,32,45,48,46,49,50,55,44,45,48,46,48,49,
48,54,32,45,48,46,48,54,51,53,44,48,32,45,48,46,49,51,55,53,56,51,44,48,32,104,32,45,50,46,48,48,48,50,53,32,113,32,45,48,46,48,57,53,50,53,44,48,32,45,48,46,50,49,49,54,54,54,44,48,32,45,48,46,49,49,
54,52,49,55,44,45,48,46,48,49,48,54,32,45,48,46,50,50,50,50,53,44,48,32,45,48,46,49,48,53,56,51,52,44,48,46,48,49,48,54,32,45,48,46,50,48,49,48,56,52,44,48,46,48,51,49,56,32,45,48,46,48,56,52,54,55,44,
48,46,48,50,49,50,32,45,48,46,49,50,55,44,48,46,48,56,52,55,32,45,48,46,48,56,52,54,55,44,48,46,49,48,53,56,51,32,45,48,46,49,50,55,44,48,46,50,52,51,52,50,32,45,48,46,48,51,49,55,53,44,48,46,49,50,55,
32,45,48,46,48,56,52,54,55,44,48,46,50,54,52,53,56,32,45,48,46,49,48,53,56,51,52,44,48,46,50,52,51,52,50,32,45,48,46,50,48,49,48,56,52,44,48,46,53,48,56,32,45,48,46,48,56,52,54,55,44,48,46,50,54,52,53,
56,32,45,48,46,49,57,48,53,44,48,46,53,48,56,32,45,48,46,48,50,49,49,55,44,48,46,48,54,51,53,32,45,48,46,48,53,50,57,50,44,48,46,49,52,56,49,55,32,45,48,46,48,50,49,49,55,44,48,46,48,55,52,49,32,45,48,
46,48,53,50,57,50,44,48,46,49,53,56,55,53,32,45,48,46,48,51,49,55,53,44,48,46,48,56,52,55,32,45,48,46,48,55,52,48,56,44,48,46,49,53,56,55,53,32,45,48,46,48,51,49,55,53,44,48,46,48,54,51,53,32,45,48,46,
48,55,52,48,56,44,48,46,48,57,53,50,32,45,48,46,48,56,52,54,55,44,48,46,48,54,51,53,32,45,48,46,50,51,50,56,51,51,44,48,46,48,55,52,49,32,45,48,46,49,51,55,53,56,51,44,48,32,45,48,46,50,57,54,51,51,51,
44,48,32,45,48,46,48,56,52,54,55,44,48,32,45,48,46,49,54,57,51,51,52,44,48,32,45,48,46,48,55,52,48,56,44,45,48,46,48,49,48,54,32,45,48,46,49,50,55,44,45,48,46,48,53,50,57,32,45,48,46,48,53,50,57,50,44,
45,48,46,48,51,49,56,32,45,48,46,48,54,51,53,44,45,48,46,48,57,53,51,32,48,44,45,48,46,48,54,51,53,32,48,46,48,49,48,53,56,44,45,48,46,49,50,55,32,48,46,48,50,49,49,55,44,45,48,46,48,55,52,49,32,48,46,
48,52,50,51,51,44,45,48,46,49,51,55,53,56,32,48,46,48,51,49,55,53,44,45,48,46,48,55,52,49,32,48,46,48,53,50,57,50,44,45,48,46,49,48,53,56,51,32,48,46,49,52,56,49,54,54,44,45,48,46,51,51,56,54,55,32,48,
46,50,54,52,53,56,51,44,45,48,46,54,54,54,55,53,32,48,46,49,49,54,52,49,55,44,45,48,46,51,51,56,54,55,32,48,46,50,53,52,44,45,48,46,54,55,55,51,52,32,48,46,48,57,53,50,53,44,45,48,46,50,51,50,56,51,32,
48,46,49,55,57,57,49,55,44,45,48,46,52,53,53,48,56,32,48,46,48,56,52,54,55,44,45,48,46,50,51,50,56,51,32,48,46,49,55,57,57,49,54,44,45,48,46,52,54,53,54,55,32,48,46,51,49,55,53,44,45,48,46,55,54,50,32,
48,46,54,48,51,50,53,44,45,49,46,53,51,52,53,56,32,48,46,50,56,53,55,53,44,45,48,46,55,56,51,49,55,32,48,46,54,48,51,50,53,44,45,49,46,53,53,53,55,53,32,48,46,49,49,54,52,49,55,44,45,48,46,50,55,53,49,
55,32,48,46,50,49,49,54,54,55,44,45,48,46,53,54,48,57,50,32,48,46,49,48,53,56,51,51,44,45,48,46,50,56,53,55,53,32,48,46,50,50,50,50,53,44,45,48,46,53,54,48,57,49,32,48,46,48,53,50,57,50,44,45,48,46,49,
49,54,52,50,32,48,46,48,57,53,50,53,44,45,48,46,50,53,52,32,48,46,48,53,50,57,50,44,45,48,46,49,52,56,49,55,32,48,46,49,52,56,49,54,55,44,45,48,46,50,49,49,54,55,32,48,46,48,52,50,51,51,44,45,48,46,48,
52,50,51,32,48,46,49,51,55,53,56,51,44,45,48,46,48,54,51,53,32,48,46,48,50,49,49,55,44,45,48,46,48,49,48,54,32,48,46,48,52,50,51,51,44,48,32,48,46,48,51,49,55,53,44,48,32,48,46,48,53,50,57,50,44,45,48,
46,48,49,48,54,32,48,46,49,57,48,53,44,48,32,48,46,52,52,52,53,44,48,32,48,46,50,53,52,44,45,48,46,48,49,48,54,32,48,46,51,55,48,52,49,55,44,48,46,48,55,52,49,32,48,46,48,56,52,54,55,44,48,46,48,54,51,
53,32,48,46,49,50,55,44,48,46,50,49,49,54,55,32,48,46,48,53,50,57,50,44,48,46,49,51,55,53,56,32,48,46,49,48,53,56,51,51,44,48,46,50,53,52,32,48,46,49,48,53,56,51,51,44,48,46,50,52,51,52,49,32,48,46,49,
57,48,53,44,48,46,52,56,54,56,51,32,48,46,48,56,52,54,55,44,48,46,50,52,51,52,50,32,48,46,49,57,48,53,44,48,46,52,56,54,56,51,32,48,46,48,56,52,54,55,44,48,46,50,49,49,54,55,32,48,46,49,53,56,55,53,44,
48,46,52,51,51,57,50,32,48,46,48,56,52,54,55,44,48,46,50,50,50,50,53,32,48,46,49,55,57,57,49,54,44,48,46,52,51,51,57,50,32,48,46,51,49,55,53,44,48,46,55,55,50,53,56,32,48,46,54,48,51,50,53,44,49,46,53,
54,54,51,51,32,48,46,50,57,54,51,51,52,44,48,46,55,56,51,49,55,32,48,46,54,50,52,52,49,55,44,49,46,53,53,53,55,53,32,48,46,48,52,50,51,51,44,48,46,48,57,53,51,32,48,46,48,54,51,53,44,48,46,49,55,57,57,
50,32,48,46,48,51,49,55,53,44,48,46,48,56,52,55,32,48,46,48,54,51,53,44,48,46,49,55,57,57,49,32,48,46,49,49,54,52,49,55,44,48,46,50,55,53,49,55,32,48,46,50,50,50,50,53,44,48,46,53,54,48,57,50,32,48,46,
49,49,54,52,49,55,44,48,46,50,56,53,55,53,32,48,46,50,51,50,56,51,51,44,48,46,53,54,48,57,50,32,122,32,109,32,45,50,46,50,54,52,56,51,51,44,45,50,46,53,55,49,55,53,32,113,32,48,46,48,57,53,50,53,44,45,
48,46,48,51,49,55,32,48,46,49,49,54,52,49,55,44,45,48,46,49,48,53,56,52,32,48,46,48,51,49,55,53,44,45,48,46,48,55,52,49,32,48,46,48,49,48,53,56,44,45,48,46,49,53,56,55,53,32,45,48,46,48,49,48,53,56,44,
45,48,46,48,57,53,50,32,45,48,46,48,53,50,57,50,44,45,48,46,49,55,57,57,49,32,45,48,46,48,51,49,55,53,44,45,48,46,48,57,53,51,32,45,48,46,48,54,51,53,44,45,48,46,49,53,56,55,53,32,45,48,46,48,50,49,49,
55,44,45,48,46,48,52,50,51,32,45,48,46,48,50,49,49,55,44,45,48,46,48,55,52,49,32,48,44,45,48,46,48,51,49,56,32,45,48,46,48,50,49,49,55,44,45,48,46,48,55,52,49,32,45,48,46,48,51,49,55,53,44,45,48,46,48,
56,52,55,32,45,48,46,48,54,51,53,44,45,48,46,49,54,57,51,51,32,45,48,46,48,50,49,49,55,44,45,48,46,48,57,53,50,32,45,48,46,48,54,51,53,44,45,48,46,49,57,48,53,32,45,48,46,49,53,56,55,53,44,45,48,46,51,
56,49,32,45,48,46,50,56,53,55,53,44,45,48,46,55,55,50,53,57,32,45,48,46,49,49,54,52,49,54,44,45,48,46,52,48,50,49,54,32,45,48,46,50,55,53,49,54,54,44,45,48,46,55,56,51,49,54,32,45,48,46,48,53,50,57,50,
44,45,48,46,49,49,54,52,50,32,45,48,46,48,57,53,50,53,44,45,48,46,50,51,50,56,52,32,45,48,46,48,51,49,55,53,44,45,48,46,49,49,54,52,49,32,45,48,46,48,56,52,54,55,44,45,48,46,50,50,50,50,53,32,45,48,46,
48,50,49,49,55,44,45,48,46,48,53,50,57,32,45,48,46,48,53,50,57,50,44,45,48,46,48,57,53,50,32,45,48,46,48,50,49,49,55,44,45,48,46,48,53,50,57,32,45,48,46,49,48,53,56,51,51,44,45,48,46,48,53,50,57,32,45,
48,46,48,50,49,49,55,44,48,46,48,50,49,50,32,45,48,46,48,52,50,51,51,44,48,46,48,51,49,56,32,45,48,46,48,50,49,49,55,44,48,46,48,49,48,54,32,45,48,46,48,52,50,51,51,44,48,46,48,51,49,55,32,45,48,46,48,
52,50,51,51,44,48,46,48,53,50,57,32,45,48,46,48,54,51,53,44,48,46,49,49,54,52,49,32,45,48,46,48,50,49,49,55,44,48,46,48,53,50,57,32,45,48,46,48,52,50,51,51,44,48,46,49,49,54,52,50,32,45,48,46,48,53,50,
57,50,44,48,46,49,49,54,52,50,32,45,48,46,48,56,52,54,55,44,48,46,50,51,50,56,51,32,45,48,46,48,51,49,55,53,44,48,46,49,49,54,52,50,32,45,48,46,48,56,52,54,55,44,48,46,50,51,50,56,52,32,45,48,46,49,55,
57,57,49,55,44,48,46,52,49,50,55,53,32,45,48,46,51,49,55,53,44,48,46,56,51,54,48,56,32,45,48,46,49,50,55,44,48,46,52,50,51,51,51,32,45,48,46,50,57,54,51,51,52,44,48,46,56,50,53,53,32,45,48,46,48,50,49,
49,55,44,48,46,48,53,50,57,32,45,48,46,48,52,50,51,51,44,48,46,49,48,53,56,51,32,45,48,46,48,49,48,53,56,44,48,46,48,53,50,57,32,45,48,46,48,51,49,55,53,44,48,46,49,48,53,56,52,32,45,48,46,48,51,49,55,
53,44,48,46,48,56,52,55,32,45,48,46,48,54,51,53,44,48,46,49,54,57,51,51,32,45,48,46,48,51,49,55,53,44,48,46,48,56,52,55,32,45,48,46,48,53,50,57,50,44,48,46,49,54,57,51,51,32,45,48,46,48,50,49,49,55,44,
48,46,48,53,50,57,32,45,48,46,48,50,49,49,55,44,48,46,49,49,54,52,50,32,48,46,48,49,48,53,56,44,48,46,48,54,51,53,32,48,46,48,51,49,55,53,44,48,46,49,48,53,56,51,32,48,46,48,53,50,57,50,44,48,46,48,56,
52,55,32,48,46,50,50,50,50,53,44,48,46,48,57,53,51,32,48,46,49,54,57,51,51,51,44,48,32,48,46,51,49,55,53,44,48,32,104,32,49,46,49,51,50,52,49,54,32,113,32,48,46,49,49,54,52,49,55,44,48,32,48,46,50,56,
53,55,53,44,48,46,48,49,48,54,32,48,46,49,54,57,51,51,52,44,48,32,48,46,50,53,52,44,45,48,46,48,51,49,56,32,122,34,32,47,62,10,32,32,32,32,32,32,32,32,60,112,97,116,104,10,32,32,32,32,32,32,32,32,32,32,
32,105,100,61,34,112,97,116,104,51,56,50,55,34,10,32,32,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,111,110,116,45,115,116,121,108,101,58,110,111,114,109,97,108,59,102,111,110,116,45,118,
97,114,105,97,110,116,58,110,111,114,109,97,108,59,102,111,110,116,45,119,101,105,103,104,116,58,110,111,114,109,97,108,59,102,111,110,116,45,115,116,114,101,116,99,104,58,110,111,114,109,97,108,59,102,
111,110,116,45,102,97,109,105,108,121,58,39,65,108,116,101,32,72,97,97,115,32,71,114,111,116,101,115,107,39,59,45,105,110,107,115,99,97,112,101,45,102,111,110,116,45,115,112,101,99,105,102,105,99,97,116,
105,111,110,58,39,65,108,116,101,32,72,97,97,115,32,71,114,111,116,101,115,107,39,59,102,105,108,108,58,35,102,50,102,50,102,50,59,115,116,114,111,107,101,45,119,105,100,116,104,58,48,46,50,54,52,53,56,
51,51,50,34,10,32,32,32,32,32,32,32,32,32,32,32,100,61,34,109,32,54,51,46,54,50,57,48,53,54,44,49,48,51,46,51,51,49,48,53,32,113,32,48,46,54,48,51,50,53,44,45,48,46,48,50,49,50,32,49,46,48,54,56,57,49,
55,44,48,46,48,57,53,50,32,48,46,52,54,53,54,54,55,44,48,46,49,49,54,52,49,32,48,46,56,50,53,53,44,48,46,51,52,57,50,53,32,48,46,48,54,51,53,44,48,46,48,52,50,51,32,48,46,49,49,54,52,49,55,44,48,46,48,
55,52,49,32,48,46,48,54,51,53,44,48,46,48,51,49,55,32,48,46,49,49,54,52,49,54,44,48,46,48,56,52,55,32,48,46,48,50,49,49,55,44,48,46,48,50,49,50,32,48,46,48,52,50,51,51,44,48,46,48,52,50,51,32,48,46,48,
50,49,49,55,44,48,46,48,49,48,54,32,48,46,48,52,50,51,51,44,48,46,48,51,49,56,32,48,46,48,50,49,49,55,44,48,46,48,50,49,50,32,48,46,48,53,50,57,50,44,48,46,48,53,50,57,32,48,46,48,51,49,55,53,44,48,46,
48,50,49,50,32,48,46,48,53,50,57,50,44,48,46,48,53,50,57,32,48,46,48,52,50,51,51,44,48,46,48,53,50,57,32,48,46,48,56,52,54,55,44,48,46,49,48,53,56,52,32,48,46,48,52,50,51,51,44,48,46,48,53,50,57,32,48,
46,48,56,52,54,55,44,48,46,49,48,53,56,51,32,48,46,48,52,50,51,51,44,48,46,48,53,50,57,32,48,46,48,54,51,53,44,48,46,49,49,54,52,50,32,48,46,48,51,49,55,53,44,48,46,48,53,50,57,32,48,46,48,54,51,53,44,
48,46,49,48,53,56,51,32,48,46,48,57,53,50,53,44,48,46,49,52,56,49,55,32,48,46,49,52,56,49,54,55,44,48,46,51,51,56,54,55,32,48,46,48,53,50,57,50,44,48,46,49,55,57,57,49,32,48,46,49,48,53,56,51,51,44,48,
46,51,56,49,32,48,46,48,50,49,49,55,44,48,46,48,56,52,55,32,48,46,48,49,48,53,56,44,48,46,49,57,48,53,32,48,44,48,46,48,57,53,51,32,45,48,46,48,51,49,55,53,44,48,46,49,52,56,49,54,32,45,48,46,48,53,50,
57,50,44,48,46,48,55,52,49,32,45,48,46,49,54,57,51,51,52,44,48,46,48,56,52,55,32,45,48,46,49,49,54,52,49,54,44,48,46,48,49,48,54,32,45,48,46,50,52,51,52,49,54,44,48,46,48,49,48,54,32,45,48,46,48,53,50,
57,50,44,48,32,45,48,46,48,57,53,50,53,44,48,32,45,48,46,48,52,50,51,51,44,48,32,45,48,46,48,56,52,54,55,44,45,48,46,48,49,48,54,32,45,48,46,48,53,50,57,50,44,45,48,46,48,49,48,54,32,45,48,46,48,57,53,
50,53,44,45,48,46,48,49,48,54,32,45,48,46,48,51,49,55,53,44,48,32,45,48,46,48,55,52,48,56,44,45,48,46,48,50,49,50,32,45,48,46,48,50,49,49,55,44,45,48,46,48,49,48,54,32,45,48,46,48,52,50,51,51,44,45,48,
46,48,50,49,50,32,45,48,46,48,50,49,49,55,44,45,48,46,48,50,49,50,32,45,48,46,48,52,50,51,51,44,45,48,46,48,52,50,51,32,45,48,46,48,54,51,53,44,45,48,46,48,57,53,50,32,45,48,46,48,57,53,50,53,44,45,48,
46,50,51,50,56,51,32,45,48,46,48,50,49,49,55,44,45,48,46,49,51,55,53,57,32,45,48,46,48,55,52,48,56,44,45,48,46,50,52,51,52,50,32,45,48,46,49,55,57,57,49,55,44,45,48,46,52,50,51,51,51,32,45,48,46,53,48,
56,44,45,48,46,54,51,53,32,45,48,46,50,52,51,52,49,55,44,45,48,46,49,54,57,51,51,32,45,48,46,53,56,50,48,56,51,44,45,48,46,50,53,52,32,45,48,46,48,54,51,53,44,45,48,46,48,50,49,50,32,45,48,46,49,50,55,
44,45,48,46,48,50,49,50,32,45,48,46,48,54,51,53,44,45,48,46,48,49,48,54,32,45,48,46,49,50,55,44,45,48,46,48,51,49,56,32,104,32,45,48,46,48,56,52,54,55,32,113,32,45,48,46,48,56,52,54,55,44,45,48,46,48,
50,49,50,32,45,48,46,49,54,57,51,51,51,44,45,48,46,48,50,49,50,32,45,48,46,48,56,52,54,55,44,48,32,45,48,46,49,54,57,51,51,52,44,48,32,45,48,46,53,53,48,51,51,51,44,48,32,45,48,46,57,50,48,55,53,44,48,
46,49,54,57,51,51,32,45,48,46,50,49,49,54,54,54,44,48,46,48,56,52,55,32,45,48,46,51,56,49,44,48,46,50,51,50,56,51,32,45,48,46,49,54,57,51,51,51,44,48,46,49,51,55,53,57,32,45,48,46,50,53,52,44,48,46,51,
52,57,50,53,32,45,48,46,48,52,50,51,51,44,48,46,48,56,52,55,32,45,48,46,48,54,51,53,44,48,46,49,55,57,57,50,32,45,48,46,48,49,48,53,56,44,48,46,48,56,52,55,32,45,48,46,48,51,49,55,53,44,48,46,49,57,48,
53,32,45,48,46,48,50,49,49,55,44,48,46,48,56,52,55,32,45,48,46,48,50,49,49,55,44,48,46,49,57,48,53,32,48,46,48,49,48,53,56,44,48,46,49,48,53,56,51,32,48,46,48,51,49,55,53,44,48,46,49,55,57,57,50,32,48,
46,48,55,52,48,56,44,48,46,50,52,51,52,49,32,48,46,49,55,57,57,49,54,44,48,46,51,55,48,52,49,32,48,46,49,49,54,52,49,55,44,48,46,49,50,55,32,48,46,51,49,55,53,44,48,46,50,52,51,52,50,32,48,46,50,51,50,
56,51,52,44,48,46,49,52,56,49,55,32,48,46,53,49,56,53,56,52,44,48,46,50,50,50,50,53,32,48,46,50,56,53,55,53,44,48,46,48,55,52,49,32,48,46,53,57,50,54,54,54,44,48,46,49,53,56,55,53,32,48,46,48,57,53,50,
53,44,48,46,48,50,49,50,32,48,46,49,57,48,53,44,48,46,48,51,49,56,32,48,46,48,57,53,50,53,44,48,46,48,49,48,54,32,48,46,49,57,48,53,44,48,46,48,52,50,51,32,48,46,49,52,56,49,54,55,44,48,46,48,52,50,51,
32,48,46,51,48,54,57,49,55,44,48,46,48,55,52,49,32,48,46,49,54,57,51,51,51,44,48,46,48,50,49,50,32,48,46,51,49,55,53,44,48,46,48,55,52,49,32,48,46,48,52,50,51,51,44,48,46,48,49,48,54,32,48,46,48,55,52,
48,56,44,48,46,48,50,49,50,32,48,46,48,51,49,55,53,44,48,32,48,46,48,55,52,48,56,44,48,46,48,49,48,54,32,48,46,48,57,53,50,53,44,48,46,48,50,49,50,32,48,46,49,57,48,53,44,48,46,48,53,50,57,32,48,46,48,
57,53,50,53,44,48,46,48,50,49,50,32,48,46,49,57,48,53,44,48,46,48,53,50,57,32,48,46,50,49,49,54,54,54,44,48,46,48,55,52,49,32,48,46,52,49,50,55,53,44,48,46,49,53,56,55,53,32,48,46,50,48,49,48,56,51,44,
48,46,48,56,52,55,32,48,46,51,56,49,44,48,46,50,48,49,48,57,32,48,46,50,51,50,56,51,51,44,48,46,49,52,56,49,54,32,48,46,52,49,50,55,52,57,44,48,46,51,52,57,50,53,32,48,46,49,57,48,53,44,48,46,49,57,48,
53,32,48,46,51,48,54,57,49,55,44,48,46,52,53,53,48,56,32,48,46,48,52,50,51,51,44,48,46,48,57,53,50,32,48,46,48,54,51,53,44,48,46,49,57,48,53,32,48,46,48,50,49,49,55,44,48,46,48,57,53,50,32,48,46,48,52,
50,51,51,44,48,46,50,48,49,48,56,32,48,46,48,50,49,49,55,44,48,46,48,53,50,57,32,48,46,48,50,49,49,55,44,48,46,49,50,55,32,48,44,48,46,48,54,51,53,32,48,46,48,50,49,49,55,44,48,46,49,50,55,32,118,32,48,
46,49,49,54,52,50,32,48,46,48,57,53,50,32,113,32,45,48,46,48,49,48,53,56,44,48,46,48,52,50,51,32,45,48,46,48,49,48,53,56,44,48,46,48,56,52,55,32,48,44,48,46,48,52,50,51,32,48,44,48,46,48,56,52,55,32,45,
48,46,48,50,49,49,55,44,48,46,48,56,52,55,32,45,48,46,48,51,49,55,53,44,48,46,49,55,57,57,50,32,48,44,48,46,48,56,52,55,32,45,48,46,48,51,49,55,53,44,48,46,49,54,57,51,51,32,45,48,46,48,53,50,57,50,44,
48,46,49,53,56,55,53,32,45,48,46,49,49,54,52,49,54,44,48,46,51,48,54,57,50,32,45,48,46,48,54,51,53,44,48,46,49,52,56,49,55,32,45,48,46,49,53,56,55,53,44,48,46,50,55,53,49,55,32,45,48,46,49,55,57,57,49,
55,44,48,46,50,55,53,49,54,32,45,48,46,52,53,53,48,56,52,44,48,46,52,54,53,54,54,32,45,48,46,50,55,53,49,54,54,44,48,46,49,57,48,53,32,45,48,46,53,57,50,54,54,54,44,48,46,51,51,56,54,55,32,45,48,46,49,
48,53,56,51,51,44,48,46,48,52,50,51,32,45,48,46,50,49,49,54,54,55,44,48,46,48,55,52,49,32,45,48,46,48,57,53,50,53,44,48,46,48,51,49,55,32,45,48,46,50,49,49,54,54,54,44,48,46,48,54,51,53,32,45,48,46,49,
49,54,52,49,55,44,48,46,48,52,50,51,32,45,48,46,50,52,51,52,49,55,44,48,46,48,53,50,57,32,45,48,46,49,50,55,44,48,46,48,50,49,50,32,45,48,46,50,53,52,44,48,46,48,53,50,57,32,45,48,46,48,53,50,57,50,44,
48,46,48,49,48,54,32,45,48,46,49,49,54,52,49,55,44,48,46,48,49,48,54,32,45,48,46,48,53,50,57,50,44,48,32,45,48,46,49,48,53,56,51,51,44,48,46,48,49,48,54,32,45,48,46,48,54,51,53,44,48,32,45,48,46,49,50,
55,44,48,32,45,48,46,48,54,51,53,44,48,46,48,49,48,54,32,45,48,46,49,50,55,44,48,46,48,49,48,54,32,45,48,46,48,53,50,57,50,44,48,46,48,49,48,54,32,45,48,46,49,53,56,55,53,44,48,46,48,49,48,54,32,45,48,
46,48,57,53,50,53,44,48,32,45,48,46,49,52,56,49,54,55,44,45,48,46,48,49,48,54,32,72,32,54,51,46,54,51,57,54,52,32,113,32,45,48,46,49,48,53,56,51,52,44,45,48,46,48,50,49,50,32,45,48,46,50,49,49,54,54,55,
44,45,48,46,48,50,49,50,32,45,48,46,49,48,53,56,51,51,44,48,32,45,48,46,50,48,49,48,56,51,44,45,48,46,48,50,49,50,32,45,48,46,48,52,50,51,51,44,45,48,46,48,49,48,54,32,45,48,46,48,56,52,54,55,44,45,48,
46,48,50,49,50,32,45,48,46,48,52,50,51,51,44,48,32,45,48,46,48,57,53,50,53,44,45,48,46,48,49,48,54,32,45,48,46,49,49,54,52,49,55,44,45,48,46,48,50,49,50,32,45,48,46,50,50,50,50,53,44,45,48,46,48,52,50,
51,32,45,48,46,48,57,53,50,53,44,45,48,46,48,50,49,50,32,45,48,46,50,48,49,48,56,51,44,45,48,46,48,54,51,53,32,45,48,46,53,49,56,53,56,52,44,45,48,46,49,54,57,51,51,32,45,48,46,56,55,56,52,49,55,44,45,
48,46,52,51,51,57,50,32,45,48,46,51,52,57,50,53,44,45,48,46,50,54,52,53,56,32,45,48,46,54,48,51,50,53,44,45,48,46,54,56,55,57,49,32,45,48,46,48,56,52,54,55,44,45,48,46,49,51,55,53,57,32,45,48,46,49,52,
56,49,54,55,44,45,48,46,50,56,53,55,53,32,45,48,46,48,53,50,57,50,44,45,48,46,49,52,56,49,55,32,45,48,46,49,48,53,56,51,51,44,45,48,46,51,49,55,53,32,45,48,46,48,50,49,49,55,44,45,48,46,48,54,51,53,32,
45,48,46,48,51,49,55,53,44,45,48,46,49,50,55,32,48,44,45,48,46,48,54,51,53,32,45,48,46,48,50,49,49,55,44,45,48,46,49,49,54,52,50,32,45,48,46,48,50,49,49,55,44,45,48,46,48,57,53,50,32,45,48,46,48,50,49,
49,55,44,45,48,46,50,49,49,54,55,32,48,44,45,48,46,49,50,55,32,48,46,48,53,50,57,50,44,45,48,46,49,55,57,57,49,32,48,46,48,53,50,57,50,44,45,48,46,48,55,52,49,32,48,46,49,54,57,51,51,52,44,45,48,46,48,
56,52,55,32,48,46,49,50,55,44,45,48,46,48,49,48,54,32,48,46,50,52,51,52,49,54,44,45,48,46,48,49,48,54,32,48,46,48,52,50,51,51,44,48,32,48,46,48,56,52,54,55,44,48,32,48,46,48,53,50,57,50,44,48,32,48,46,
48,56,52,54,55,44,48,46,48,49,48,54,32,48,46,48,53,50,57,50,44,48,46,48,49,48,54,32,48,46,48,56,52,54,55,44,48,46,48,49,48,54,32,48,46,48,51,49,55,53,44,45,48,46,48,49,48,54,32,48,46,48,55,52,48,56,44,
48,46,48,49,48,54,32,48,46,48,55,52,48,56,44,48,46,48,52,50,51,32,48,46,48,57,53,50,53,44,48,46,48,56,52,55,32,48,46,48,51,49,55,53,44,48,46,48,53,50,57,32,48,46,48,51,49,55,53,44,48,46,49,49,54,52,50,
32,48,46,48,49,48,53,56,44,48,46,48,53,50,57,32,48,46,48,51,49,55,53,44,48,46,49,49,54,52,50,32,48,46,48,50,49,49,55,44,48,46,48,55,52,49,32,48,46,48,51,49,55,53,44,48,46,49,52,56,49,54,32,48,46,48,50,
49,49,55,44,48,46,48,55,52,49,32,48,46,48,53,50,57,50,44,48,46,49,52,56,49,55,32,48,46,49,55,57,57,49,55,44,48,46,52,53,53,48,56,32,48,46,53,51,57,55,53,44,48,46,55,49,57,54,55,32,48,46,49,55,57,57,49,
55,44,48,46,49,51,55,53,56,32,48,46,51,57,49,53,56,52,44,48,46,50,50,50,50,53,32,48,46,50,49,49,54,54,54,44,48,46,48,56,52,55,32,48,46,52,53,53,48,56,51,44,48,46,49,53,56,55,53,32,48,46,48,56,52,54,55,
44,48,46,48,50,49,50,32,48,46,49,54,57,51,51,51,44,48,46,48,51,49,56,32,48,46,48,56,52,54,55,44,48,46,48,49,48,54,32,48,46,49,54,57,51,51,52,44,48,46,48,50,49,50,32,48,46,48,50,49,49,55,44,48,32,48,46,
48,53,50,57,50,44,48,46,48,49,48,54,32,48,46,48,51,49,55,53,44,48,32,48,46,48,54,51,53,44,48,32,48,46,48,52,50,51,51,44,48,46,48,49,48,54,32,48,46,48,57,53,50,53,44,48,32,48,46,48,54,51,53,44,45,48,46,
48,49,48,54,32,48,46,49,49,54,52,49,55,44,48,46,48,49,48,54,32,48,46,48,52,50,51,51,44,48,46,48,49,48,54,32,48,46,49,49,54,52,49,55,44,48,46,48,49,48,54,32,48,46,48,56,52,54,55,44,48,32,48,46,49,51,55,
53,56,51,44,45,48,46,48,50,49,50,32,104,32,48,46,48,55,52,48,56,32,113,32,48,46,48,53,50,57,50,44,45,48,46,48,49,48,54,32,48,46,49,48,53,56,51,52,44,45,48,46,48,49,48,54,32,48,46,48,53,50,57,50,44,48,
32,48,46,49,48,53,56,51,51,44,45,48,46,48,49,48,54,32,48,46,48,51,49,55,53,44,45,48,46,48,49,48,54,32,48,46,48,53,50,57,50,44,48,32,48,46,48,51,49,55,53,44,48,32,48,46,48,55,52,48,56,44,45,48,46,48,50,
49,50,32,48,46,48,56,52,54,55,44,45,48,46,48,50,49,50,32,48,46,49,55,57,57,49,55,44,45,48,46,48,51,49,56,32,48,46,48,57,53,50,53,44,45,48,46,48,50,49,50,32,48,46,49,55,57,57,49,54,44,45,48,46,48,53,50,
57,32,48,46,50,55,53,49,54,55,44,45,48,46,49,49,54,52,50,32,48,46,52,56,54,56,51,52,44,45,48,46,50,54,52,53,57,32,48,46,50,50,50,50,53,44,45,48,46,49,53,56,55,53,32,48,46,51,51,56,54,54,54,44,45,48,46,
52,50,51,51,51,32,48,46,48,51,49,55,53,44,45,48,46,48,56,52,55,32,48,46,48,53,50,57,50,44,45,48,46,49,54,57,51,51,32,48,46,48,50,49,49,55,44,45,48,46,48,56,52,55,32,48,46,48,52,50,51,51,44,45,48,46,49,
54,57,51,52,32,48,46,48,50,49,49,55,44,45,48,46,48,56,52,55,32,48,46,48,50,49,49,55,44,45,48,46,50,48,49,48,56,32,48,44,45,48,46,49,49,54,52,50,32,45,48,46,48,50,49,49,55,44,45,48,46,49,57,48,53,32,45,
48,46,48,50,49,49,55,44,45,48,46,48,53,50,57,32,45,48,46,48,50,49,49,55,44,45,48,46,48,56,52,55,32,48,44,45,48,46,48,52,50,51,32,45,48,46,48,50,49,49,55,44,45,48,46,48,56,52,55,32,45,48,46,48,54,51,53,
44,45,48,46,49,55,57,57,50,32,45,48,46,49,54,57,51,51,51,44,45,48,46,50,57,54,51,52,32,45,48,46,49,48,53,56,51,52,44,45,48,46,49,50,55,32,45,48,46,50,53,52,44,45,48,46,50,51,50,56,51,32,45,48,46,50,50,
50,50,53,44,45,48,46,49,53,56,55,53,32,45,48,46,52,57,55,52,49,55,44,45,48,46,50,53,52,32,45,48,46,50,55,53,49,54,55,44,45,48,46,48,57,53,50,32,45,48,46,53,56,50,48,56,51,44,45,48,46,49,53,56,55,53,32,
45,48,46,50,57,54,51,51,52,44,45,48,46,48,55,52,49,32,45,48,46,54,49,51,56,51,52,44,45,48,46,49,51,55,53,56,32,45,48,46,51,49,55,53,44,45,48,46,48,54,51,53,32,45,48,46,54,49,51,56,51,51,44,45,48,46,49,
52,56,49,55,32,45,48,46,49,54,57,51,51,51,44,45,48,46,48,53,50,57,32,45,48,46,51,51,56,54,54,55,44,45,48,46,48,56,52,55,32,45,48,46,49,54,57,51,51,51,44,45,48,46,48,52,50,51,32,45,48,46,51,49,55,53,44,
45,48,46,49,48,53,56,51,32,45,48,46,48,50,49,49,55,44,45,48,46,48,49,48,54,32,45,48,46,48,51,49,55,53,44,45,48,46,48,49,48,54,32,45,48,46,48,49,48,53,56,44,48,32,45,48,46,48,51,49,55,53,44,45,48,46,48,
49,48,54,32,45,48,46,49,48,53,56,51,51,44,45,48,46,48,52,50,51,32,45,48,46,50,49,49,54,54,54,44,45,48,46,48,56,52,55,32,45,48,46,49,48,53,56,51,52,44,45,48,46,48,52,50,51,32,45,48,46,50,48,49,48,56,52,
44,45,48,46,49,48,53,56,52,32,45,48,46,50,52,51,52,49,54,44,45,48,46,49,52,56,49,54,32,45,48,46,52,52,52,53,44,45,48,46,51,49,55,53,32,45,48,46,50,48,49,48,56,51,44,45,48,46,49,55,57,57,49,32,45,48,46,
51,49,55,53,44,45,48,46,52,53,53,48,56,32,45,48,46,48,53,50,57,50,44,45,48,46,48,57,53,51,32,45,48,46,48,55,52,48,56,44,45,48,46,50,48,49,48,56,32,45,48,46,48,50,49,49,55,44,45,48,46,49,49,54,52,50,32,
45,48,46,48,53,50,57,50,44,45,48,46,50,51,50,56,52,32,48,44,45,48,46,48,52,50,51,32,45,48,46,48,49,48,53,56,44,45,48,46,48,55,52,49,32,48,44,45,48,46,48,51,49,56,32,48,44,45,48,46,48,54,51,53,32,45,48,
46,48,51,49,55,53,44,45,48,46,49,50,55,32,45,48,46,48,49,48,53,56,44,45,48,46,51,49,55,53,32,48,46,48,50,49,49,55,44,45,48,46,50,48,49,48,56,32,48,46,48,53,50,57,50,44,45,48,46,51,48,54,57,50,32,48,46,
48,51,49,55,53,44,45,48,46,49,49,54,52,49,32,48,46,48,53,50,57,50,44,45,48,46,50,50,50,50,53,32,48,46,48,51,49,55,53,44,45,48,46,49,48,53,56,51,32,48,46,48,55,52,48,56,44,45,48,46,50,48,49,48,56,32,48,
46,49,52,56,49,54,55,44,45,48,46,51,49,55,53,32,48,46,51,56,49,44,45,48,46,53,50,57,49,55,32,48,46,50,51,50,56,51,52,44,45,48,46,50,50,50,50,53,32,48,46,53,49,56,53,56,52,44,45,48,46,52,48,50,49,54,32,
48,46,48,50,49,49,55,44,45,48,46,48,50,49,50,32,48,46,48,53,50,57,50,44,45,48,46,48,50,49,50,32,48,46,48,51,49,55,53,44,45,48,46,48,49,48,54,32,48,46,48,53,50,57,50,44,45,48,46,48,51,49,56,32,48,46,50,
51,50,56,51,51,44,45,48,46,49,49,54,52,50,32,48,46,53,51,57,55,53,44,45,48,46,50,48,49,48,56,32,48,46,48,56,52,54,55,44,45,48,46,48,50,49,50,32,48,46,49,53,56,55,53,44,45,48,46,48,51,49,56,32,48,46,48,
55,52,48,56,44,45,48,46,48,50,49,50,32,48,46,49,53,56,55,53,44,45,48,46,48,52,50,51,32,48,46,48,56,52,54,55,44,45,48,46,48,50,49,50,32,48,46,49,55,57,57,49,55,44,45,48,46,48,50,49,50,32,48,46,49,48,53,
56,51,51,44,48,32,48,46,49,57,48,53,44,45,48,46,48,50,49,50,32,48,46,48,51,49,55,53,44,45,48,46,48,49,48,54,32,48,46,48,57,53,50,53,44,48,32,48,46,48,54,51,53,44,48,46,48,49,48,54,32,48,46,48,56,52,54,
55,44,45,48,46,48,49,48,54,32,122,34,32,47,62,10,32,32,32,32,32,32,32,32,60,112,97,116,104,10,32,32,32,32,32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,51,56,50,57,34,10,32,32,32,32,32,32,32,32,32,
32,32,115,116,121,108,101,61,34,102,111,110,116,45,115,116,121,108,101,58,110,111,114,109,97,108,59,102,111,110,116,45,118,97,114,105,97,110,116,58,110,111,114,109,97,108,59,102,111,110,116,45,119,101,
105,103,104,116,58,110,111,114,109,97,108,59,102,111,110,116,45,115,116,114,101,116,99,104,58,110,111,114,109,97,108,59,102,111,110,116,45,102,97,109,105,108,121,58,39,65,108,116,101,32,72,97,97,115,32,
71,114,111,116,101,115,107,39,59,45,105,110,107,115,99,97,112,101,45,102,111,110,116,45,115,112,101,99,105,102,105,99,97,116,105,111,110,58,39,65,108,116,101,32,72,97,97,115,32,71,114,111,116,101,115,
107,39,59,102,105,108,108,58,35,102,50,102,50,102,50,59,115,116,114,111,107,101,45,119,105,100,116,104,58,48,46,50,54,52,53,56,51,51,50,34,10,32,32,32,32,32,32,32,32,32,32,32,100,61,34,109,32,54,55,46,
53,56,49,50,54,57,44,49,48,51,46,52,55,57,50,49,32,104,32,52,46,52,50,51,56,51,51,32,48,46,56,48,52,51,51,51,32,113,32,48,46,49,48,53,56,51,51,44,48,32,48,46,50,49,49,54,54,55,44,48,32,48,46,49,48,53,
56,51,51,44,48,32,48,46,49,55,57,57,49,54,44,48,46,48,52,50,51,32,48,46,48,56,52,54,55,44,48,46,48,53,50,57,32,48,46,48,57,53,50,53,44,48,46,49,52,56,49,54,32,48,46,48,50,49,49,55,44,48,46,48,56,52,55,
32,48,46,48,50,49,49,55,44,48,46,50,50,50,50,53,32,118,32,48,46,49,50,55,32,113,32,45,48,46,48,50,49,49,55,44,48,46,48,56,52,55,32,45,48,46,48,52,50,51,51,44,48,46,49,51,55,53,57,32,45,48,46,48,49,48,
53,56,44,48,46,48,53,50,57,32,45,48,46,48,54,51,53,44,48,46,48,57,53,51,32,45,48,46,48,56,52,54,55,44,48,46,48,53,50,57,32,45,48,46,50,51,50,56,51,52,44,48,46,48,53,50,57,32,45,48,46,49,52,56,49,54,54,
44,48,32,45,48,46,50,56,53,55,53,44,48,32,104,32,45,49,46,50,48,54,53,32,113,32,45,48,46,49,49,54,52,49,54,44,48,32,45,48,46,50,51,50,56,51,51,44,48,32,45,48,46,49,48,53,56,51,51,44,45,48,46,48,49,48,
54,32,45,48,46,50,48,49,48,56,51,44,48,46,48,49,48,54,32,45,48,46,48,56,52,54,55,44,48,46,48,49,48,54,32,45,48,46,49,52,56,49,54,55,44,48,46,48,54,51,53,32,45,48,46,48,54,51,53,44,48,46,48,52,50,51,32,
45,48,46,48,57,53,50,53,44,48,46,49,51,55,53,56,32,45,48,46,48,51,49,55,53,44,48,46,48,57,53,51,32,45,48,46,48,50,49,49,55,44,48,46,50,52,51,52,50,32,48,46,48,49,48,53,56,44,48,46,49,52,56,49,54,32,48,
46,48,49,48,53,56,44,48,46,50,54,52,53,56,32,118,32,49,46,48,50,54,53,56,32,51,46,51,52,52,51,52,32,49,46,49,50,49,56,51,32,113,32,48,44,48,46,49,53,56,55,53,32,45,48,46,48,50,49,49,55,44,48,46,50,56,
53,55,53,32,45,48,46,48,49,48,53,56,44,48,46,49,50,55,32,45,48,46,49,49,54,52,49,54,44,48,46,49,55,57,57,50,32,45,48,46,48,53,50,57,50,44,48,46,48,50,49,50,32,45,48,46,49,50,55,44,48,46,48,51,49,56,32,
45,48,46,48,55,52,48,56,44,48,32,45,48,46,49,53,56,55,53,44,48,32,45,48,46,49,51,55,53,56,52,44,48,32,45,48,46,50,57,54,51,51,52,44,48,32,45,48,46,49,52,56,49,54,54,44,48,32,45,48,46,50,48,49,48,56,51,
44,45,48,46,48,55,52,49,32,45,48,46,48,54,51,53,44,45,48,46,48,56,52,55,32,45,48,46,48,55,52,48,56,44,45,48,46,50,48,49,48,56,32,48,44,45,48,46,49,49,54,52,50,32,48,44,45,48,46,50,53,52,32,118,32,45,48,
46,57,52,49,57,50,32,45,51,46,52,56,49,57,49,32,45,49,46,48,52,55,55,53,32,113,32,48,44,45,48,46,49,49,54,52,50,32,48,44,45,48,46,50,54,52,53,57,32,48,46,48,49,48,53,56,44,45,48,46,49,53,56,55,53,32,45,
48,46,48,50,49,49,55,44,45,48,46,50,53,52,32,45,48,46,48,53,50,57,50,44,45,48,46,49,54,57,51,51,32,45,48,46,50,52,51,52,49,55,44,45,48,46,49,55,57,57,49,32,45,48,46,49,55,57,57,49,54,44,45,48,46,48,49,
48,54,32,45,48,46,51,57,49,53,56,51,44,45,48,46,48,49,48,54,32,104,32,45,49,46,49,54,52,49,54,55,32,113,32,45,48,46,49,50,55,44,48,32,45,48,46,50,57,54,51,51,51,44,48,46,48,49,48,54,32,45,48,46,49,53,
56,55,53,44,48,32,45,48,46,50,52,51,52,49,55,44,45,48,46,48,51,49,55,32,45,48,46,48,51,49,55,53,44,45,48,46,48,50,49,50,32,45,48,46,48,54,51,53,44,45,48,46,48,52,50,51,32,45,48,46,48,51,49,55,53,44,45,
48,46,48,50,49,50,32,45,48,46,48,53,50,57,50,44,45,48,46,48,53,50,57,32,45,48,46,48,51,49,55,53,44,45,48,46,48,52,50,51,32,45,48,46,48,52,50,51,51,44,45,48,46,49,51,55,53,57,32,45,48,46,48,49,48,53,56,
44,45,48,46,48,57,53,50,32,45,48,46,48,49,48,53,56,44,45,48,46,49,57,48,53,32,48,46,48,49,48,53,56,44,45,48,46,48,57,53,50,32,48,46,48,51,49,55,53,44,45,48,46,49,55,57,57,49,32,48,46,48,50,49,49,55,44,
45,48,46,48,56,52,55,32,48,46,48,53,50,57,50,44,45,48,46,49,49,54,52,50,32,48,46,48,50,49,49,55,44,45,48,46,48,51,49,56,32,48,46,48,53,50,57,50,44,45,48,46,48,52,50,51,32,48,46,48,52,50,51,51,44,45,48,
46,48,50,49,50,32,48,46,48,56,52,54,55,44,45,48,46,48,51,49,56,32,48,46,48,50,49,49,55,44,45,48,46,48,49,48,54,32,48,46,48,52,50,51,51,44,48,32,48,46,48,50,49,49,55,44,48,32,48,46,48,52,50,51,51,44,45,
48,46,48,49,48,54,32,122,34,32,47,62,10,32,32,32,32,32,32,32,32,60,112,97,116,104,10,32,32,32,32,32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,51,56,51,49,34,10,32,32,32,32,32,32,32,32,32,32,32,115,
116,121,108,101,61,34,102,111,110,116,45,115,116,121,108,101,58,110,111,114,109,97,108,59,102,111,110,116,45,118,97,114,105,97,110,116,58,110,111,114,109,97,108,59,102,111,110,116,45,119,101,105,103,104,
116,58,110,111,114,109,97,108,59,102,111,110,116,45,115,116,114,101,116,99,104,58,110,111,114,109,97,108,59,102,111,110,116,45,102,97,109,105,108,121,58,39,65,108,116,101,32,72,97,97,115,32,71,114,111,
116,101,115,107,39,59,45,105,110,107,115,99,97,112,101,45,102,111,110,116,45,115,112,101,99,105,102,105,99,97,116,105,111,110,58,39,65,108,116,101,32,72,97,97,115,32,71,114,111,116,101,115,107,39,59,102,
105,108,108,58,35,102,50,102,50,102,50,59,115,116,114,111,107,101,45,119,105,100,116,104,58,48,46,50,54,52,53,56,51,51,50,34,10,32,32,32,32,32,32,32,32,32,32,32,100,61,34,109,32,56,51,46,50,57,50,56,56,
57,44,49,48,55,46,48,55,55,53,53,32,113,32,48,44,48,46,48,56,52,55,32,48,44,48,46,49,57,48,53,32,48,46,48,49,48,53,56,44,48,46,48,57,53,50,32,45,48,46,48,49,48,53,56,44,48,46,49,54,57,51,51,32,118,32,
48,46,49,48,53,56,51,32,113,32,45,48,46,48,49,48,53,56,44,48,46,48,51,49,56,32,45,48,46,48,49,48,53,56,44,48,46,48,55,52,49,32,48,46,48,49,48,53,56,44,48,46,48,52,50,51,32,48,44,48,46,48,54,51,53,32,45,
48,46,48,49,48,53,56,44,48,46,48,53,50,57,32,45,48,46,48,49,48,53,56,44,48,46,49,49,54,52,49,32,48,44,48,46,48,53,50,57,32,45,48,46,48,49,48,53,56,44,48,46,49,48,53,56,52,32,45,48,46,48,51,49,55,53,44,
48,46,49,52,56,49,54,32,45,48,46,48,52,50,51,51,44,48,46,51,49,55,53,32,45,48,46,48,49,48,53,56,44,48,46,49,53,56,55,53,32,45,48,46,48,54,51,53,44,48,46,51,48,54,57,49,32,45,48,46,48,51,49,55,53,44,48,
46,49,50,55,32,45,48,46,48,55,52,48,56,44,48,46,50,54,52,53,57,32,45,48,46,48,51,49,55,53,44,48,46,49,50,55,32,45,48,46,48,56,52,54,55,44,48,46,50,53,52,32,45,48,46,49,55,57,57,49,55,44,48,46,52,53,53,
48,56,32,45,48,46,52,53,53,48,56,52,44,48,46,56,50,53,53,32,45,48,46,50,54,52,53,56,51,44,48,46,51,53,57,56,51,32,45,48,46,54,54,54,55,52,57,44,48,46,54,49,51,56,51,32,45,48,46,50,48,49,48,56,52,44,48,
46,49,50,55,32,45,48,46,52,51,51,57,49,55,44,48,46,50,50,50,50,53,32,45,48,46,50,50,50,50,53,44,48,46,48,56,52,55,32,45,48,46,52,56,54,56,51,51,44,48,46,49,54,57,51,51,32,45,48,46,49,49,54,52,49,55,44,
48,46,48,52,50,51,32,45,48,46,50,53,52,44,48,46,48,54,51,53,32,45,48,46,49,50,55,44,48,46,48,49,48,54,32,45,48,46,50,53,52,44,48,46,48,52,50,51,32,45,48,46,49,49,54,52,49,55,44,48,46,48,50,49,50,32,45,
48,46,50,51,50,56,51,52,44,48,46,48,50,49,50,32,45,48,46,49,48,53,56,51,51,44,48,32,45,48,46,50,49,49,54,54,54,44,48,46,48,50,49,50,32,104,32,45,48,46,49,52,56,49,54,55,32,113,32,45,48,46,48,51,49,55,
53,44,48,46,48,49,48,54,32,45,48,46,48,55,52,48,56,44,48,46,48,49,48,54,32,45,48,46,48,52,50,51,51,44,45,48,46,48,49,48,54,32,45,48,46,48,56,52,54,55,44,45,48,46,48,49,48,54,32,104,32,45,50,46,49,49,54,
54,54,55,32,113,32,45,48,46,49,53,56,55,53,44,48,32,45,48,46,50,56,53,55,53,44,45,48,46,48,49,48,54,32,45,48,46,49,49,54,52,49,54,44,45,48,46,48,50,49,50,32,45,48,46,49,53,56,55,53,44,45,48,46,49,51,55,
53,57,32,45,48,46,48,52,50,51,51,44,45,48,46,48,56,52,55,32,45,48,46,48,52,50,51,51,44,45,48,46,50,52,51,52,49,32,48,46,48,49,48,53,56,44,45,48,46,49,53,56,55,53,32,48,46,48,49,48,53,56,44,45,48,46,50,
56,53,55,53,32,118,32,45,54,46,50,53,52,55,53,32,113,32,48,44,45,48,46,49,50,55,32,45,48,46,48,49,48,53,56,44,45,48,46,50,54,52,53,57,32,48,44,45,48,46,49,52,56,49,54,32,48,46,48,53,50,57,50,44,45,48,
46,50,51,50,56,51,32,48,46,48,52,50,51,51,44,45,48,46,48,55,52,49,32,48,46,49,53,56,55,53,44,45,48,46,48,57,53,50,32,48,46,48,50,49,49,55,44,45,48,46,48,49,48,54,32,48,46,48,52,50,51,51,44,48,32,48,46,
48,50,49,49,55,44,48,32,48,46,48,52,50,51,51,44,45,48,46,48,49,48,54,32,104,32,50,46,50,57,54,53,56,52,32,113,32,48,46,48,53,50,57,50,44,48,32,48,46,48,57,53,50,53,44,48,32,48,46,48,53,50,57,50,44,45,
48,46,48,49,48,54,32,48,46,48,57,53,50,53,44,48,32,48,46,48,53,50,57,50,44,48,46,48,50,49,50,32,48,46,49,48,53,56,51,51,44,48,46,48,49,48,54,32,48,46,48,54,51,53,44,45,48,46,48,49,48,54,32,48,46,49,49,
54,52,49,55,44,48,32,48,46,48,53,50,57,50,44,48,46,48,49,48,54,32,48,46,49,49,54,52,49,54,44,48,46,48,49,48,54,32,48,46,48,54,51,53,44,48,32,48,46,49,49,54,52,49,55,44,48,46,48,49,48,54,32,48,46,48,53,
50,57,50,44,48,46,48,49,48,54,32,48,46,48,57,53,50,53,44,48,46,48,49,48,54,32,48,46,48,53,50,57,50,44,48,32,48,46,49,48,53,56,51,51,44,48,46,48,49,48,54,32,48,46,49,49,54,52,49,55,44,48,46,48,50,49,50,
32,48,46,50,50,50,50,53,44,48,46,48,52,50,51,32,48,46,49,48,53,56,51,52,44,48,46,48,49,48,54,32,48,46,50,49,49,54,54,55,44,48,46,48,52,50,51,32,48,46,50,51,50,56,51,51,44,48,46,48,55,52,49,32,48,46,52,
52,52,53,44,48,46,49,52,56,49,55,32,48,46,50,49,49,54,54,55,44,48,46,48,54,51,53,32,48,46,51,57,49,53,56,51,44,48,46,49,55,57,57,49,32,48,46,49,52,56,49,54,55,44,48,46,48,56,52,55,32,48,46,50,54,52,53,
56,51,44,48,46,49,54,57,51,52,32,48,46,48,51,49,55,53,44,48,46,48,50,49,50,32,48,46,48,54,51,53,44,48,46,48,53,50,57,32,48,46,48,51,49,55,53,44,48,46,48,50,49,50,32,48,46,48,54,51,53,44,48,46,48,52,50,
51,32,48,46,48,54,51,53,44,48,46,48,53,50,57,32,48,46,49,49,54,52,49,55,44,48,46,49,48,53,56,51,32,48,46,48,53,50,57,50,44,48,46,48,53,50,57,32,48,46,49,48,53,56,51,51,44,48,46,49,48,53,56,51,32,48,46,
48,53,50,57,50,44,48,46,48,53,50,57,32,48,46,48,56,52,54,55,44,48,46,48,55,52,49,32,48,46,48,52,50,51,51,44,48,46,48,54,51,53,32,48,46,48,56,52,54,55,44,48,46,49,49,54,52,49,32,48,46,48,53,50,57,50,44,
48,46,48,53,50,57,32,48,46,48,57,53,50,53,44,48,46,49,49,54,52,50,32,48,46,48,53,50,57,50,44,48,46,48,54,51,53,32,48,46,48,56,52,54,55,44,48,46,49,51,55,53,56,32,48,46,48,52,50,51,51,44,48,46,48,54,51,
53,32,48,46,48,57,53,50,53,44,48,46,49,51,55,53,57,32,48,46,48,50,49,49,55,44,48,46,48,52,50,51,32,48,46,48,51,49,55,53,44,48,46,48,55,52,49,32,48,46,48,49,48,53,56,44,48,46,48,51,49,56,32,48,46,48,51,
49,55,53,44,48,46,48,55,52,49,32,48,46,48,52,50,51,51,44,48,46,48,56,52,55,32,48,46,48,55,52,48,56,44,48,46,49,54,57,51,52,32,48,46,48,51,49,55,53,44,48,46,48,55,52,49,32,48,46,48,55,52,48,56,44,48,46,
49,53,56,55,53,32,48,46,48,52,50,51,51,44,48,46,49,49,54,52,49,32,48,46,48,54,51,53,44,48,46,50,52,51,52,49,32,48,46,48,51,49,55,53,44,48,46,49,49,54,52,50,32,48,46,48,54,51,53,44,48,46,50,51,50,56,52,
32,48,46,48,50,49,49,55,44,48,46,48,54,51,53,32,48,46,48,50,49,49,55,44,48,46,49,50,55,32,48,44,48,46,48,53,50,57,32,48,46,48,50,49,49,55,44,48,46,49,49,54,52,49,32,48,46,48,53,50,57,50,44,48,46,49,57,
48,53,32,48,46,48,53,50,57,50,44,48,46,52,50,51,51,52,32,48,46,48,49,48,53,56,44,48,46,50,50,50,50,53,32,48,46,48,49,48,53,56,44,48,46,52,52,52,53,32,122,32,109,32,45,49,46,48,48,53,52,49,54,44,48,46,
53,53,48,51,51,32,113,32,48,46,48,50,49,49,55,44,45,48,46,49,48,53,56,51,32,48,46,48,50,49,49,55,44,45,48,46,50,50,50,50,53,32,48,44,45,48,46,49,50,55,32,48,44,45,48,46,50,52,51,52,50,32,48,44,45,48,46,
48,56,52,55,32,48,44,45,48,46,49,53,56,55,53,32,48,46,48,49,48,53,56,44,45,48,46,48,56,52,55,32,45,48,46,48,49,48,53,56,44,45,48,46,49,53,56,55,53,32,48,44,45,48,46,48,51,49,56,32,45,48,46,48,49,48,53,
56,44,45,48,46,48,54,51,53,32,48,44,45,48,46,48,51,49,56,32,48,44,45,48,46,48,55,52,49,32,45,48,46,48,50,49,49,55,44,45,48,46,49,49,54,52,50,32,45,48,46,48,51,49,55,53,44,45,48,46,50,51,50,56,51,32,48,
44,45,48,46,49,49,54,52,50,32,45,48,46,48,51,49,55,53,44,45,48,46,50,51,50,56,52,32,45,48,46,48,50,49,49,55,44,45,48,46,48,56,52,55,32,45,48,46,48,52,50,51,51,44,45,48,46,49,53,56,55,53,32,45,48,46,48,
49,48,53,56,44,45,48,46,48,56,52,55,32,45,48,46,48,51,49,55,53,44,45,48,46,49,54,57,51,51,32,45,48,46,49,49,54,52,49,54,44,45,48,46,51,56,49,32,45,48,46,50,57,54,51,51,51,44,45,48,46,54,54,54,55,53,32,
45,48,46,50,57,54,51,51,51,44,45,48,46,52,55,54,50,53,32,45,48,46,56,55,56,52,49,54,44,45,48,46,55,49,57,54,55,32,45,48,46,48,57,53,50,53,44,45,48,46,48,53,50,57,32,45,48,46,50,48,49,48,56,52,44,45,48,
46,48,55,52,49,32,45,48,46,49,48,53,56,51,51,44,45,48,46,48,51,49,55,32,45,48,46,50,50,50,50,53,44,45,48,46,48,54,51,53,32,45,48,46,49,49,54,52,49,54,44,45,48,46,48,51,49,56,32,45,48,46,50,52,51,52,49,
54,44,45,48,46,48,51,49,56,32,45,48,46,49,49,54,52,49,55,44,45,48,46,48,49,48,54,32,45,48,46,50,51,50,56,51,52,44,45,48,46,48,51,49,56,32,104,32,45,48,46,50,50,50,50,53,32,113,32,45,48,46,48,51,49,55,
53,44,45,48,46,48,49,48,54,32,45,48,46,48,55,52,48,56,44,45,48,46,48,49,48,54,32,45,48,46,48,52,50,51,51,44,48,32,45,48,46,48,56,52,54,55,44,48,32,72,32,55,56,46,52,48,51,51,57,32,113,32,45,48,46,48,50,
49,49,55,44,48,46,48,49,48,54,32,45,48,46,48,53,50,57,50,44,48,46,48,49,48,54,32,45,48,46,48,51,49,55,53,44,45,48,46,48,49,48,54,32,45,48,46,48,53,50,57,50,44,48,32,45,48,46,48,53,50,57,50,44,48,46,48,
50,49,50,32,45,48,46,49,48,53,56,51,51,44,48,46,48,52,50,51,32,45,48,46,48,52,50,51,51,44,48,46,48,50,49,50,32,45,48,46,48,54,51,53,44,48,46,48,54,51,53,32,45,48,46,48,50,49,49,55,44,48,46,48,52,50,51,
32,45,48,46,48,52,50,51,51,44,48,46,49,52,56,49,55,32,118,32,48,46,48,56,52,55,32,113,32,45,48,46,48,49,48,53,56,44,48,46,48,52,50,51,32,45,48,46,48,49,48,53,56,44,48,46,48,57,53,50,32,48,44,48,46,48,
53,50,57,32,48,44,48,46,48,57,53,51,32,118,32,52,46,56,48,52,56,51,32,113,32,48,44,48,46,48,53,50,57,32,48,44,48,46,49,48,53,56,51,32,48,44,48,46,48,52,50,51,32,48,46,48,49,48,53,56,44,48,46,48,56,52,
55,32,118,32,48,46,48,56,52,55,32,113,32,48,46,48,50,49,49,55,44,48,46,49,48,53,56,51,32,48,46,48,52,50,51,51,44,48,46,49,52,56,49,54,32,48,46,48,56,52,54,55,44,48,46,49,50,55,32,48,46,51,49,55,53,44,
48,46,49,50,55,32,48,46,50,51,50,56,51,51,44,45,48,46,48,49,48,54,32,48,46,52,52,52,53,44,45,48,46,48,49,48,54,32,104,32,48,46,54,49,51,56,51,51,32,113,32,48,46,48,55,52,48,56,44,48,32,48,46,49,53,56,
55,53,44,48,46,48,49,48,54,32,48,46,48,56,52,54,55,44,48,32,48,46,49,52,56,49,54,55,44,45,48,46,48,50,49,50,32,104,32,48,46,49,49,54,52,49,55,32,113,32,48,46,48,53,50,57,50,44,45,48,46,48,49,48,54,32,
48,46,49,49,54,52,49,54,44,48,32,48,46,48,54,51,53,44,48,32,48,46,49,50,55,44,45,48,46,48,49,48,54,32,48,46,48,54,51,53,44,45,48,46,48,50,49,50,32,48,46,49,50,55,44,45,48,46,48,50,49,50,32,48,46,48,54,
51,53,44,45,48,46,48,49,48,54,32,48,46,49,51,55,53,56,52,44,45,48,46,48,51,49,56,32,48,46,49,49,54,52,49,54,44,45,48,46,48,51,49,56,32,48,46,50,50,50,50,53,44,45,48,46,48,54,51,53,32,48,46,49,49,54,52,
49,54,44,45,48,46,48,51,49,56,32,48,46,50,51,50,56,51,51,44,45,48,46,48,56,52,55,32,48,46,51,57,49,53,56,51,44,45,48,46,49,54,57,51,51,32,48,46,54,54,54,55,53,44,45,48,46,52,53,53,48,56,32,48,46,50,56,
53,55,53,44,45,48,46,50,57,54,51,52,32,48,46,52,54,53,54,54,54,44,45,48,46,54,57,56,53,32,48,46,48,53,50,57,50,44,45,48,46,49,49,54,52,50,32,48,46,48,56,52,54,55,44,45,48,46,50,52,51,52,50,32,48,46,48,
52,50,51,51,44,45,48,46,49,51,55,53,56,32,48,46,48,56,52,54,55,44,45,48,46,50,55,53,49,55,32,48,46,48,51,49,55,53,44,45,48,46,49,49,54,52,49,32,48,46,48,52,50,51,51,44,45,48,46,50,52,51,52,49,32,48,46,
48,50,49,49,55,44,45,48,46,49,51,55,53,57,32,48,46,48,52,50,51,51,44,45,48,46,50,54,52,53,57,32,48,46,48,49,48,53,56,44,45,48,46,48,53,50,57,32,48,46,48,49,48,53,56,44,45,48,46,48,56,52,55,32,48,44,45,
48,46,48,52,50,51,32,48,44,45,48,46,48,56,52,55,32,122,34,32,47,62,10,32,32,32,32,32,32,32,32,60,112,97,116,104,10,32,32,32,32,32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,51,56,51,51,34,10,32,32,
32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,111,110,116,45,115,116,121,108,101,58,110,111,114,109,97,108,59,102,111,110,116,45,118,97,114,105,97,110,116,58,110,111,114,109,97,108,59,102,111,
110,116,45,119,101,105,103,104,116,58,110,111,114,109,97,108,59,102,111,110,116,45,115,116,114,101,116,99,104,58,110,111,114,109,97,108,59,102,111,110,116,45,102,97,109,105,108,121,58,39,65,108,116,101,
32,72,97,97,115,32,71,114,111,116,101,115,107,39,59,45,105,110,107,115,99,97,112,101,45,102,111,110,116,45,115,112,101,99,105,102,105,99,97,116,105,111,110,58,39,65,108,116,101,32,72,97,97,115,32,71,114,
111,116,101,115,107,39,59,102,105,108,108,58,35,102,50,102,50,102,50,59,115,116,114,111,107,101,45,119,105,100,116,104,58,48,46,50,54,52,53,56,51,51,50,34,10,32,32,32,32,32,32,32,32,32,32,32,100,61,34,
109,32,56,52,46,49,53,49,52,54,51,44,49,48,53,46,53,55,52,55,49,32,113,32,48,46,49,51,55,53,56,51,44,48,32,48,46,51,49,55,53,44,48,32,48,46,49,55,57,57,49,54,44,45,48,46,48,49,48,54,32,48,46,50,54,52,
53,56,51,44,48,46,48,52,50,51,32,48,46,48,56,52,54,55,44,48,46,48,53,50,57,32,48,46,49,49,54,52,49,55,44,48,46,49,51,55,53,56,32,48,46,48,52,50,51,51,44,48,46,48,56,52,55,32,48,46,48,56,52,54,55,44,48,
46,49,55,57,57,50,32,48,46,48,53,50,57,50,44,48,46,49,51,55,53,56,32,48,46,48,57,53,50,53,44,48,46,50,55,53,49,54,32,48,46,48,52,50,51,51,44,48,46,49,51,55,53,57,32,48,46,48,57,53,50,53,44,48,46,50,55,
53,49,55,32,48,46,48,53,50,57,50,44,48,46,49,50,55,32,48,46,48,57,53,50,53,44,48,46,50,54,52,53,56,32,48,46,48,52,50,51,51,44,48,46,49,50,55,32,48,46,48,57,53,50,53,44,48,46,50,53,52,32,48,46,48,53,50,
57,50,44,48,46,49,49,54,52,50,32,48,46,48,56,52,54,55,44,48,46,50,52,51,52,50,32,48,46,48,51,49,55,53,44,48,46,49,50,55,32,48,46,48,56,52,54,55,44,48,46,50,53,52,32,48,46,49,50,55,44,48,46,51,48,54,57,
50,32,48,46,50,51,50,56,51,51,44,48,46,54,51,53,32,48,46,49,48,53,56,51,51,44,48,46,51,50,56,48,56,32,48,46,50,51,50,56,51,51,44,48,46,54,51,53,32,48,46,48,50,49,49,55,44,48,46,48,52,50,51,32,48,46,48,
50,49,49,55,44,48,46,48,56,52,55,32,48,44,48,46,48,51,49,56,32,48,46,48,50,49,49,55,44,48,46,48,54,51,53,32,48,46,48,54,51,53,44,48,46,49,52,56,49,54,32,48,46,49,49,54,52,49,54,44,48,46,51,48,54,57,49,
32,48,46,48,53,50,57,50,44,48,46,49,52,56,49,55,32,48,46,49,48,53,56,51,52,44,48,46,50,57,54,51,52,32,48,46,48,51,49,55,53,44,48,46,48,55,52,49,32,48,46,48,54,51,53,44,48,46,49,53,56,55,53,32,48,46,48,
51,49,55,53,44,48,46,48,55,52,49,32,48,46,49,48,53,56,51,51,44,48,46,48,57,53,51,32,48,46,48,56,52,54,55,44,48,46,48,51,49,55,32,48,46,49,50,55,44,45,48,46,48,53,50,57,32,48,46,48,53,50,57,50,44,45,48,
46,48,57,53,51,32,48,46,48,55,52,48,56,44,45,48,46,49,52,56,49,55,32,48,46,48,56,52,54,55,44,45,48,46,49,55,57,57,49,32,48,46,49,51,55,53,56,51,44,45,48,46,51,56,49,32,48,46,48,53,50,57,50,44,45,48,46,
50,48,49,48,56,32,48,46,49,50,55,44,45,48,46,51,57,49,53,56,32,48,46,48,57,53,50,53,44,45,48,46,50,52,51,52,50,32,48,46,49,54,57,51,51,52,44,45,48,46,52,56,54,56,51,32,48,46,48,55,52,48,56,44,45,48,46,
50,53,52,32,48,46,49,55,57,57,49,54,44,45,48,46,52,56,54,56,52,32,48,46,48,49,48,53,56,44,45,48,46,48,53,50,57,32,48,46,48,51,49,55,53,44,45,48,46,49,51,55,53,56,32,48,46,48,52,50,51,51,44,45,48,46,48,
57,53,50,32,48,46,48,55,52,48,56,44,45,48,46,49,57,48,53,32,48,46,48,51,49,55,53,44,45,48,46,49,48,53,56,51,32,48,46,48,54,51,53,44,45,48,46,49,57,48,53,32,48,46,48,50,49,49,55,44,45,48,46,48,53,50,57,
32,48,46,48,51,49,55,53,44,45,48,46,48,57,53,50,32,48,46,48,49,48,53,56,44,45,48,46,48,52,50,51,32,48,46,48,51,49,55,53,44,45,48,46,48,57,53,51,32,48,46,48,53,50,57,50,44,45,48,46,49,50,55,32,48,46,48,
56,52,54,55,44,45,48,46,50,53,52,32,48,46,48,52,50,51,51,44,45,48,46,49,51,55,53,56,32,48,46,48,57,53,50,53,44,45,48,46,50,54,52,53,56,32,48,46,48,50,49,49,55,44,45,48,46,48,53,50,57,32,48,46,48,51,49,
55,53,44,45,48,46,48,57,53,50,32,48,46,48,49,48,53,56,44,45,48,46,48,52,50,51,32,48,46,48,51,49,55,53,44,45,48,46,48,57,53,51,32,48,46,48,52,50,51,51,44,45,48,46,48,57,53,50,32,48,46,48,55,52,48,56,44,
45,48,46,49,57,48,53,32,48,46,48,51,49,55,53,44,45,48,46,49,48,53,56,52,32,48,46,48,54,51,53,44,45,48,46,50,48,49,48,57,32,48,46,48,51,49,55,53,44,45,48,46,48,56,52,55,32,48,46,48,53,50,57,50,44,45,48,
46,49,54,57,51,51,32,48,46,48,51,49,55,53,44,45,48,46,48,57,53,50,32,48,46,48,56,52,54,55,44,45,48,46,49,53,56,55,53,32,48,46,48,55,52,48,56,44,45,48,46,48,57,53,51,32,48,46,50,48,49,48,56,51,44,45,48,
46,49,48,53,56,51,32,48,46,49,51,55,53,56,52,44,45,48,46,48,49,48,54,32,48,46,50,57,54,51,51,52,44,45,48,46,48,49,48,54,32,48,46,48,56,52,54,55,44,48,32,48,46,49,53,56,55,53,44,48,46,48,49,48,54,32,48,
46,48,56,52,54,55,44,48,32,48,46,49,51,55,53,56,51,44,48,46,48,51,49,56,32,48,46,48,57,53,50,53,44,48,46,48,54,51,53,32,48,46,48,53,50,57,50,44,48,46,50,48,49,48,56,32,45,48,46,48,52,50,51,51,44,48,46,
49,51,55,53,56,32,45,48,46,48,55,52,48,56,44,48,46,50,49,49,54,55,32,45,48,46,48,49,48,53,56,44,48,46,48,50,49,50,32,45,48,46,48,50,49,49,55,44,48,46,48,52,50,51,32,48,44,48,46,48,50,49,50,32,45,48,46,
48,49,48,53,56,44,48,46,48,52,50,51,32,45,48,46,49,49,54,52,49,54,44,48,46,50,57,54,51,52,32,45,48,46,50,51,50,56,51,51,44,48,46,54,48,51,50,53,32,45,48,46,49,49,54,52,49,55,44,48,46,50,57,54,51,52,32,
45,48,46,50,51,50,56,51,51,44,48,46,53,57,50,54,55,32,45,48,46,48,51,49,55,53,44,48,46,48,56,52,55,32,45,48,46,48,53,50,57,50,44,48,46,49,53,56,55,53,32,45,48,46,48,50,49,49,55,44,48,46,48,54,51,53,32,
45,48,46,48,53,50,57,50,44,48,46,49,51,55,53,56,32,45,48,46,48,56,52,54,55,44,48,46,50,49,49,54,55,32,45,48,46,49,54,57,51,51,51,44,48,46,52,51,51,57,50,32,45,48,46,48,55,52,48,56,44,48,46,50,49,49,54,
55,32,45,48,46,49,53,56,55,53,44,48,46,52,50,51,51,51,32,45,48,46,51,48,54,57,49,55,44,48,46,55,51,48,50,53,32,45,48,46,53,55,49,53,44,49,46,52,55,49,48,57,32,45,48,46,50,54,52,53,56,51,44,48,46,55,52,
48,56,51,32,45,48,46,53,55,49,53,44,49,46,52,55,49,48,56,32,45,48,46,48,50,49,49,55,44,48,46,48,53,50,57,32,45,48,46,48,52,50,51,51,44,48,46,49,48,53,56,51,32,45,48,46,48,49,48,53,56,44,48,46,48,54,51,
53,32,45,48,46,48,51,49,55,53,44,48,46,49,50,55,32,45,48,46,49,54,57,51,51,51,44,48,46,51,57,49,53,57,32,45,48,46,51,53,57,56,51,51,44,48,46,55,55,50,53,57,32,45,48,46,49,57,48,53,44,48,46,51,57,49,53,
56,32,45,48,46,53,50,57,49,54,55,44,48,46,54,48,51,50,53,32,45,48,46,49,49,54,52,49,55,44,48,46,48,54,51,53,32,45,48,46,50,51,50,56,51,51,44,48,46,48,57,53,51,32,45,48,46,49,49,54,52,49,55,44,48,46,48,
52,50,51,32,45,48,46,50,54,52,53,56,52,44,48,46,48,55,52,49,32,45,48,46,48,53,50,57,50,44,48,46,48,50,49,50,32,45,48,46,49,49,54,52,49,54,44,48,46,48,49,48,54,32,45,48,46,48,53,50,57,50,44,45,48,46,48,
49,48,54,32,45,48,46,49,49,54,52,49,55,44,48,32,45,48,46,48,56,52,54,55,44,48,46,48,50,49,50,32,45,48,46,49,54,57,51,51,51,44,48,32,45,48,46,48,52,50,51,51,44,45,48,46,48,49,48,54,32,45,48,46,49,48,53,
56,51,52,44,45,48,46,48,49,48,54,32,45,48,46,48,56,52,54,55,44,45,48,46,48,50,49,50,32,45,48,46,49,54,57,51,51,51,44,45,48,46,48,52,50,51,32,45,48,46,48,55,52,48,56,44,45,48,46,48,50,49,50,32,45,48,46,
49,49,54,52,49,55,44,45,48,46,48,56,52,55,32,45,48,46,48,53,50,57,50,44,45,48,46,48,54,51,53,32,45,48,46,48,54,51,53,44,45,48,46,49,53,56,55,53,32,48,44,45,48,46,48,56,52,55,32,48,44,45,48,46,49,57,48,
53,32,48,44,45,48,46,48,55,52,49,32,48,44,45,48,46,49,52,56,49,55,32,48,46,48,49,48,53,56,44,45,48,46,48,54,51,53,32,48,46,48,52,50,51,51,44,45,48,46,49,48,53,56,51,32,48,46,48,55,52,48,56,44,45,48,46,
49,49,54,52,50,32,48,46,50,53,52,44,45,48,46,48,54,51,53,32,48,46,49,49,54,52,49,54,44,48,46,48,50,49,50,32,48,46,50,53,52,44,48,46,48,50,49,50,32,48,46,49,51,55,53,56,51,44,48,32,48,46,50,52,51,52,49,
54,44,45,48,46,48,52,50,51,32,48,46,49,53,56,55,53,44,45,48,46,48,53,50,57,32,48,46,50,53,52,44,45,48,46,49,53,56,55,53,32,48,46,49,48,53,56,51,52,44,45,48,46,49,48,53,56,51,32,48,46,49,55,57,57,49,55,
44,45,48,46,50,52,51,52,49,32,48,46,48,55,52,48,56,44,45,48,46,49,50,55,32,48,46,49,50,55,44,45,48,46,50,55,53,49,55,32,48,46,48,54,51,53,44,45,48,46,49,52,56,49,55,32,48,46,49,50,55,44,45,48,46,50,57,
54,51,51,32,48,46,48,52,50,51,51,44,45,48,46,48,57,53,51,32,48,46,48,56,52,54,55,44,45,48,46,50,51,50,56,52,32,48,46,48,53,50,57,50,44,45,48,46,49,51,55,53,56,32,48,46,48,49,48,53,56,44,45,48,46,50,56,
53,55,53,32,45,48,46,48,53,50,57,50,44,45,48,46,49,54,57,51,51,32,45,48,46,49,49,54,52,49,55,44,45,48,46,51,49,55,53,32,45,48,46,48,53,50,57,50,44,45,48,46,49,52,56,49,54,32,45,48,46,49,49,54,52,49,54,
44,45,48,46,50,57,54,51,51,32,45,48,46,48,57,53,50,53,44,45,48,46,50,50,50,50,53,32,45,48,46,49,55,57,57,49,55,44,45,48,46,52,52,52,53,32,45,48,46,48,56,52,54,55,44,45,48,46,50,50,50,50,53,32,45,48,46,
49,54,57,51,51,51,44,45,48,46,52,51,51,57,50,32,45,48,46,48,54,51,53,44,45,48,46,49,52,56,49,54,32,45,48,46,49,49,54,52,49,55,44,45,48,46,50,56,53,55,53,32,45,48,46,48,53,50,57,50,44,45,48,46,49,51,55,
53,56,32,45,48,46,49,48,53,56,51,51,44,45,48,46,50,56,53,55,53,32,45,48,46,49,57,48,53,44,45,48,46,52,54,53,54,54,32,45,48,46,51,56,49,44,45,48,46,57,51,49,51,51,32,45,48,46,49,55,57,57,49,55,44,45,48,
46,52,54,53,54,55,32,45,48,46,51,55,48,52,49,55,44,45,48,46,57,51,49,51,51,32,45,48,46,49,48,53,56,51,51,44,45,48,46,50,52,51,52,50,32,45,48,46,49,57,48,53,44,45,48,46,52,54,53,54,55,32,45,48,46,48,56,
52,54,55,44,45,48,46,50,51,50,56,51,32,45,48,46,49,55,57,57,49,55,44,45,48,46,52,54,53,54,55,32,45,48,46,48,50,49,49,55,44,45,48,46,48,54,51,53,32,45,48,46,48,54,51,53,44,45,48,46,49,54,57,51,51,32,45,
48,46,48,52,50,51,51,44,45,48,46,49,49,54,52,50,32,48,46,48,49,48,53,56,44,45,48,46,49,57,48,53,32,48,46,48,51,49,55,53,44,45,48,46,48,53,50,57,32,48,46,49,50,55,44,45,48,46,48,55,52,49,32,48,46,48,50,
49,49,55,44,45,48,46,48,49,48,54,32,48,46,48,52,50,51,51,44,48,32,48,46,48,51,49,55,53,44,48,32,48,46,48,53,50,57,50,44,45,48,46,48,49,48,54,32,122,34,32,47,62,10,32,32,32,32,32,32,32,32,60,112,97,116,
104,10,32,32,32,32,32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,51,56,51,53,34,10,32,32,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,111,110,116,45,115,116,121,108,101,58,110,111,114,
109,97,108,59,102,111,110,116,45,118,97,114,105,97,110,116,58,110,111,114,109,97,108,59,102,111,110,116,45,119,101,105,103,104,116,58,110,111,114,109,97,108,59,102,111,110,116,45,115,116,114,101,116,99,
104,58,110,111,114,109,97,108,59,102,111,110,116,45,102,97,109,105,108,121,58,39,65,108,116,101,32,72,97,97,115,32,71,114,111,116,101,115,107,39,59,45,105,110,107,115,99,97,112,101,45,102,111,110,116,
45,115,112,101,99,105,102,105,99,97,116,105,111,110,58,39,65,108,116,101,32,72,97,97,115,32,71,114,111,116,101,115,107,39,59,102,105,108,108,58,35,102,50,102,50,102,50,59,115,116,114,111,107,101,45,119,
105,100,116,104,58,48,46,50,54,52,53,56,51,51,50,34,10,32,32,32,32,32,32,32,32,32,32,32,100,61,34,109,32,57,50,46,50,52,55,55,49,44,49,48,53,46,52,54,56,56,56,32,113,32,48,46,54,54,54,55,53,44,45,48,46,
48,50,49,50,32,49,46,48,54,56,57,49,55,44,48,46,49,55,57,57,50,32,48,46,52,48,50,49,54,54,44,48,46,50,48,49,48,56,32,48,46,54,52,53,53,56,51,44,48,46,54,48,51,50,53,32,48,46,48,54,51,53,44,48,46,49,48,
53,56,51,32,48,46,48,57,53,50,53,44,48,46,50,51,50,56,51,32,48,46,48,52,50,51,51,44,48,46,49,49,54,52,50,32,48,46,48,55,52,48,56,44,48,46,50,52,51,52,50,32,48,46,48,53,50,57,50,44,48,46,50,48,49,48,56,
32,48,46,48,54,51,53,44,48,46,52,51,51,57,49,32,48,46,48,49,48,53,56,44,48,46,50,51,50,56,52,32,48,46,48,49,48,53,56,44,48,46,52,55,54,50,53,32,118,32,49,46,49,51,50,52,50,32,49,46,54,53,49,32,113,32,
48,44,48,46,49,55,57,57,50,32,45,48,46,48,49,48,53,56,44,48,46,51,53,57,56,51,32,48,44,48,46,49,55,57,57,50,32,45,48,46,49,51,55,53,56,51,44,48,46,50,51,50,56,52,32,45,48,46,48,53,50,57,50,44,48,46,48,
50,49,50,32,45,48,46,49,49,54,52,49,55,44,48,46,48,50,49,50,32,45,48,46,48,53,50,57,50,44,48,46,48,49,48,54,32,45,48,46,49,49,54,52,49,54,44,48,46,48,49,48,54,32,45,48,46,49,51,55,53,56,52,44,48,32,45,
48,46,50,54,52,53,56,52,44,45,48,46,48,49,48,54,32,45,48,46,49,50,55,44,48,32,45,48,46,49,55,57,57,49,54,44,45,48,46,48,56,52,55,32,45,48,46,48,53,50,57,50,44,45,48,46,48,55,52,49,32,45,48,46,48,53,50,
57,50,44,45,48,46,49,57,48,53,32,48,44,45,48,46,49,49,54,52,50,32,48,44,45,48,46,50,51,50,56,52,32,118,32,45,48,46,57,49,48,49,54,32,45,49,46,55,50,53,48,57,32,113,32,48,44,45,48,46,50,56,53,55,53,32,
45,48,46,48,49,48,53,56,44,45,48,46,53,54,48,57,49,32,45,48,46,48,49,48,53,56,44,45,48,46,50,56,53,55,53,32,45,48,46,48,57,53,50,53,44,45,48,46,52,56,54,56,52,32,45,48,46,48,56,52,54,55,44,45,48,46,50,
49,49,54,54,32,45,48,46,50,53,52,44,45,48,46,51,51,56,54,54,32,45,48,46,49,54,57,51,51,52,44,45,48,46,49,51,55,53,57,32,45,48,46,51,57,49,53,56,52,44,45,48,46,50,49,49,54,55,32,45,48,46,48,54,51,53,44,
45,48,46,48,50,49,50,32,45,48,46,49,51,55,53,56,51,44,45,48,46,48,50,49,50,32,45,48,46,48,54,51,53,44,45,48,46,48,49,48,54,32,45,48,46,49,50,55,44,45,48,46,48,51,49,56,32,45,48,46,48,53,50,57,50,44,45,
48,46,48,49,48,54,32,45,48,46,49,54,57,51,51,51,44,48,32,45,48,46,49,48,53,56,51,52,44,48,32,45,48,46,49,52,56,49,54,55,44,48,46,48,49,48,54,32,45,48,46,48,52,50,51,51,44,48,46,48,50,49,50,32,45,48,46,
48,55,52,48,56,44,48,46,48,50,49,50,32,45,48,46,48,51,49,55,53,44,45,48,46,48,49,48,54,32,45,48,46,48,55,52,48,56,44,48,32,45,48,46,49,51,55,53,56,51,44,48,46,48,51,49,55,32,45,48,46,50,54,52,53,56,51,
44,48,46,48,55,52,49,32,45,48,46,49,49,54,52,49,55,44,48,46,48,51,49,56,32,45,48,46,50,50,50,50,53,44,48,46,48,57,53,50,32,45,48,46,48,52,50,51,51,44,48,46,48,50,49,50,32,45,48,46,48,55,52,48,56,44,48,
46,48,52,50,51,32,45,48,46,48,51,49,55,53,44,48,46,48,50,49,50,32,45,48,46,48,55,52,48,56,44,48,46,48,52,50,51,32,45,48,46,51,55,48,52,49,54,44,48,46,50,54,52,53,57,32,45,48,46,53,51,57,55,53,44,48,46,
55,57,51,55,53,32,45,48,46,48,51,49,55,53,44,48,46,48,56,52,55,32,45,48,46,48,52,50,51,51,44,48,46,49,55,57,57,50,32,45,48,46,48,49,48,53,56,44,48,46,48,56,52,55,32,45,48,46,48,51,49,55,53,44,48,46,49,
54,57,51,51,32,45,48,46,48,50,49,49,55,44,48,46,48,53,50,57,32,45,48,46,48,49,48,53,56,44,48,46,49,48,53,56,52,32,48,46,48,49,48,53,56,44,48,46,48,52,50,51,32,45,48,46,48,49,48,53,56,44,48,46,48,56,52,
55,32,118,32,48,46,50,50,50,50,53,32,113,32,45,48,46,48,49,48,53,56,44,48,46,48,51,49,56,32,45,48,46,48,49,48,53,56,44,48,46,48,55,52,49,32,48,46,48,49,48,53,56,44,48,46,48,52,50,51,32,48,46,48,49,48,
53,56,44,48,46,48,57,53,51,32,118,32,48,46,51,49,55,53,32,49,46,48,49,54,32,48,46,57,51,49,51,51,32,113,32,48,44,48,46,49,49,54,52,50,32,48,44,48,46,50,52,51,52,50,32,48,44,48,46,49,49,54,52,49,32,45,
48,46,48,53,50,57,50,44,48,46,49,57,48,53,32,45,48,46,48,53,50,57,50,44,48,46,48,56,52,55,32,45,48,46,49,57,48,53,44,48,46,48,56,52,55,32,45,48,46,49,50,55,44,48,46,48,49,48,54,32,45,48,46,50,54,52,53,
56,52,44,48,46,48,49,48,54,32,45,48,46,48,56,52,54,55,44,48,32,45,48,46,49,53,56,55,53,44,45,48,46,48,49,48,54,32,45,48,46,48,55,52,48,56,44,45,48,46,48,49,48,54,32,45,48,46,49,50,55,44,45,48,46,48,53,
50,57,32,45,48,46,48,56,52,54,55,44,45,48,46,48,53,50,57,32,45,48,46,48,56,52,54,55,44,45,48,46,50,48,49,48,57,32,48,44,45,48,46,49,52,56,49,54,32,48,44,45,48,46,50,56,53,55,53,32,118,32,45,49,46,50,52,
56,56,51,32,45,50,46,53,54,49,49,55,32,45,48,46,54,52,53,53,56,32,113,32,48,44,45,48,46,48,57,53,50,32,48,44,45,48,46,49,57,48,53,32,48,44,45,48,46,48,57,53,50,32,48,46,48,52,50,51,51,44,45,48,46,49,52,
56,49,55,32,48,46,48,51,49,55,53,44,45,48,46,48,54,51,53,32,48,46,49,48,53,56,51,51,44,45,48,46,48,56,52,55,32,48,46,48,52,50,51,51,44,45,48,46,48,49,48,54,32,48,46,49,49,54,52,49,55,44,45,48,46,48,50,
49,50,32,48,46,48,56,52,54,55,44,45,48,46,48,49,48,54,32,48,46,49,54,57,51,51,51,44,45,48,46,48,49,48,54,32,48,46,48,56,52,54,55,44,48,32,48,46,49,53,56,55,53,44,48,46,48,49,48,54,32,48,46,48,56,52,54,
55,44,48,46,48,49,48,54,32,48,46,49,49,54,52,49,55,44,48,46,48,51,49,56,32,48,46,49,49,54,52,49,55,44,48,46,48,55,52,49,32,48,46,49,48,53,56,51,51,44,48,46,49,57,48,53,32,45,48,46,48,49,48,53,56,44,48,
46,49,48,53,56,51,32,48,46,48,51,49,55,53,44,48,46,50,52,51,52,50,32,48,46,48,50,49,49,55,44,48,46,48,54,51,53,32,48,46,48,56,52,54,55,44,48,46,49,48,53,56,51,32,48,46,49,50,55,44,48,46,48,49,48,54,32,
48,46,50,48,49,48,56,51,44,45,48,46,48,57,53,50,32,48,46,48,56,52,54,55,44,45,48,46,49,48,53,56,51,32,48,46,49,54,57,51,51,52,44,45,48,46,49,54,57,51,51,32,48,46,49,52,56,49,54,54,44,45,48,46,49,48,53,
56,52,32,48,46,50,57,54,51,51,51,44,45,48,46,49,57,48,53,32,48,46,49,53,56,55,53,44,45,48,46,48,57,53,50,32,48,46,51,53,57,56,51,51,44,45,48,46,49,53,56,55,53,32,48,46,48,56,52,54,55,44,45,48,46,48,51,
49,56,32,48,46,49,55,57,57,49,55,44,45,48,46,48,52,50,51,32,48,46,49,48,53,56,51,51,44,45,48,46,48,49,48,54,32,48,46,50,48,49,48,56,51,44,45,48,46,48,51,49,56,32,48,46,48,53,50,57,50,44,45,48,46,48,49,
48,54,32,48,46,49,48,53,56,51,52,44,45,48,46,48,49,48,54,32,48,46,48,53,50,57,50,44,48,32,48,46,49,48,53,56,51,51,44,48,32,122,34,32,47,62,10,32,32,32,32,32,32,32,32,60,112,97,116,104,10,32,32,32,32,32,
32,32,32,32,32,32,105,100,61,34,112,97,116,104,51,56,51,55,34,10,32,32,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,111,110,116,45,115,116,121,108,101,58,110,111,114,109,97,108,59,102,111,
110,116,45,118,97,114,105,97,110,116,58,110,111,114,109,97,108,59,102,111,110,116,45,119,101,105,103,104,116,58,110,111,114,109,97,108,59,102,111,110,116,45,115,116,114,101,116,99,104,58,110,111,114,109,
97,108,59,102,111,110,116,45,102,97,109,105,108,121,58,39,65,108,116,101,32,72,97,97,115,32,71,114,111,116,101,115,107,39,59,45,105,110,107,115,99,97,112,101,45,102,111,110,116,45,115,112,101,99,105,102,
105,99,97,116,105,111,110,58,39,65,108,116,101,32,72,97,97,115,32,71,114,111,116,101,115,107,39,59,102,105,108,108,58,35,102,50,102,50,102,50,59,115,116,114,111,107,101,45,119,105,100,116,104,58,48,46,
50,54,52,53,56,51,51,50,34,10,32,32,32,32,32,32,32,32,32,32,32,100,61,34,109,32,49,48,48,46,51,57,51,48,55,44,49,49,48,46,54,51,51,53,53,32,113,32,48,46,48,51,49,56,44,48,46,49,54,57,51,51,32,45,48,46,
48,51,49,55,44,48,46,50,56,53,55,53,32,45,48,46,48,54,51,53,44,48,46,49,48,53,56,51,32,45,48,46,49,54,57,51,51,44,48,46,49,53,56,55,53,32,45,48,46,48,53,50,57,44,48,46,48,50,49,50,32,45,48,46,49,49,54,
52,50,44,48,46,48,50,49,50,32,45,48,46,48,53,50,57,44,48,32,45,48,46,49,49,54,52,49,51,44,48,46,48,50,49,50,32,45,48,46,50,51,50,56,51,52,44,48,46,48,53,50,57,32,45,48,46,52,54,53,54,54,55,44,45,48,46,
48,51,49,56,32,45,48,46,50,50,50,50,53,44,45,48,46,48,55,52,49,32,45,48,46,51,48,54,57,49,55,44,45,48,46,49,57,48,53,32,45,48,46,48,53,50,57,50,44,45,48,46,48,55,52,49,32,45,48,46,48,57,53,50,53,44,45,
48,46,49,54,57,51,51,32,45,48,46,48,52,50,51,51,44,45,48,46,49,48,53,56,52,32,45,48,46,49,48,53,56,51,51,44,45,48,46,49,54,57,51,52,32,45,48,46,48,50,49,49,55,44,45,48,46,48,50,49,50,32,45,48,46,48,53,
50,57,50,44,45,48,46,48,52,50,51,32,45,48,46,48,50,49,49,55,44,45,48,46,48,50,49,50,32,45,48,46,48,54,51,53,44,45,48,46,48,49,48,54,32,45,48,46,49,49,54,52,49,54,44,48,46,48,51,49,56,32,45,48,46,50,50,
50,50,53,44,48,46,49,49,54,52,49,32,45,48,46,48,57,53,50,53,44,48,46,48,56,52,55,32,45,48,46,49,57,48,53,44,48,46,49,52,56,49,55,32,45,48,46,49,51,55,53,56,51,44,48,46,48,56,52,55,32,45,48,46,50,57,54,
51,51,51,44,48,46,49,53,56,55,53,32,45,48,46,49,53,56,55,53,44,48,46,48,54,51,53,32,45,48,46,51,51,56,54,54,55,44,48,46,49,49,54,52,50,32,45,48,46,49,48,53,56,51,51,44,48,46,48,51,49,56,32,45,48,46,50,
51,50,56,51,51,44,48,46,48,52,50,51,32,45,48,46,49,49,54,52,49,55,44,48,46,48,49,48,54,32,45,48,46,50,51,50,56,51,51,44,48,46,48,51,49,56,32,45,48,46,48,53,50,57,50,44,48,46,48,49,48,54,32,45,48,46,48,
57,53,50,53,44,48,46,48,49,48,54,32,45,48,46,48,51,49,55,53,44,48,32,45,48,46,48,55,52,48,56,44,48,32,45,48,46,49,49,54,52,49,54,44,48,46,48,50,49,50,32,45,48,46,50,50,50,50,53,44,48,32,104,32,45,48,46,
48,57,53,50,53,32,113,32,45,48,46,48,53,50,57,50,44,45,48,46,48,49,48,54,32,45,48,46,49,48,53,56,51,51,44,45,48,46,48,49,48,54,32,45,48,46,48,53,50,57,50,44,48,32,45,48,46,48,57,53,50,53,44,45,48,46,48,
50,49,50,32,45,48,46,48,52,50,51,51,44,45,48,46,48,49,48,54,32,45,48,46,48,55,52,48,56,44,45,48,46,48,49,48,54,32,45,48,46,48,50,49,49,55,44,48,46,48,49,48,54,32,45,48,46,48,53,50,57,50,44,48,32,45,48,
46,48,56,52,54,55,44,45,48,46,48,50,49,50,32,45,48,46,49,54,57,51,51,51,44,45,48,46,48,52,50,51,32,45,48,46,48,56,52,54,55,44,45,48,46,48,49,48,54,32,45,48,46,49,53,56,55,53,44,45,48,46,48,52,50,51,32,
45,48,46,51,49,55,53,44,45,48,46,49,51,55,53,56,32,45,48,46,53,49,56,53,56,52,44,45,48,46,51,48,54,57,49,32,45,48,46,49,57,48,53,44,45,48,46,49,55,57,57,50,32,45,48,46,51,49,55,53,44,45,48,46,52,57,55,
52,50,32,45,48,46,48,57,53,50,53,44,45,48,46,50,51,50,56,51,32,45,48,46,48,57,53,50,53,44,45,48,46,53,54,48,57,50,32,118,32,45,48,46,50,50,50,50,53,32,113,32,48,46,48,49,48,53,56,44,45,48,46,48,53,50,
57,32,48,46,48,49,48,53,56,44,45,48,46,49,48,53,56,51,32,48,46,48,49,48,53,56,44,45,48,46,48,54,51,53,32,48,46,48,50,49,49,55,44,45,48,46,49,50,55,32,48,46,48,52,50,51,51,44,45,48,46,49,49,54,52,50,32,
48,46,48,56,52,54,55,44,45,48,46,50,51,50,56,51,32,48,46,48,52,50,51,51,44,45,48,46,49,49,54,52,50,32,48,46,49,48,53,56,51,51,44,45,48,46,50,49,49,54,55,32,48,46,49,57,48,53,44,45,48,46,51,48,54,57,50,
32,48,46,53,49,56,53,56,52,44,45,48,46,52,53,53,48,56,32,48,46,51,50,56,48,56,51,44,45,48,46,49,52,56,49,55,32,48,46,55,49,57,54,54,54,44,45,48,46,50,54,52,53,57,32,48,46,49,52,56,49,54,55,44,45,48,46,
48,52,50,51,32,48,46,51,48,54,57,49,55,44,45,48,46,48,54,51,53,32,48,46,49,53,56,55,53,44,45,48,46,48,50,49,50,32,48,46,51,49,55,53,44,45,48,46,48,53,50,57,32,48,46,48,55,52,48,56,44,45,48,46,48,50,49,
50,32,48,46,49,51,55,53,56,51,44,45,48,46,48,50,49,50,32,48,46,48,55,52,48,56,44,48,32,48,46,49,53,56,55,53,44,45,48,46,48,50,49,50,32,48,46,48,55,52,48,56,44,45,48,46,48,50,49,50,32,48,46,49,52,56,49,
54,55,44,45,48,46,48,50,49,50,32,48,46,48,55,52,48,56,44,45,48,46,48,49,48,54,32,48,46,49,53,56,55,53,44,45,48,46,48,51,49,56,32,48,46,50,54,52,53,56,51,44,45,48,46,48,54,51,53,32,48,46,52,57,55,52,49,
55,44,45,48,46,49,48,53,56,52,32,48,46,50,51,50,56,51,51,44,45,48,46,48,53,50,57,32,48,46,51,55,48,52,49,54,44,45,48,46,50,52,51,52,49,32,48,46,48,54,51,53,44,45,48,46,48,56,52,55,32,48,46,49,48,53,56,
51,52,44,45,48,46,50,49,49,54,55,32,48,46,48,52,50,51,51,44,45,48,46,49,51,55,53,56,32,48,44,45,48,46,50,57,54,51,51,32,45,48,46,48,54,51,53,44,45,48,46,50,53,52,32,45,48,46,49,57,48,53,44,45,48,46,52,
48,50,49,55,32,45,48,46,49,49,54,52,49,55,44,45,48,46,49,52,56,49,55,32,45,48,46,51,51,56,54,54,55,44,45,48,46,50,51,50,56,51,32,45,48,46,49,50,55,44,45,48,46,48,53,50,57,32,45,48,46,51,48,54,57,49,55,
44,45,48,46,48,55,52,49,32,45,48,46,49,54,57,51,51,51,44,45,48,46,48,50,49,50,32,45,48,46,51,53,57,56,51,51,44,45,48,46,48,50,49,50,32,45,48,46,49,57,48,53,44,48,32,45,48,46,51,55,48,52,49,55,44,48,46,
48,50,49,50,32,45,48,46,49,55,57,57,49,54,44,48,46,48,50,49,50,32,45,48,46,51,48,54,57,49,54,44,48,46,48,54,51,53,32,45,48,46,49,55,57,57,49,55,44,48,46,48,54,51,53,32,45,48,46,51,52,57,50,53,44,48,46,
49,55,57,57,50,32,45,48,46,48,56,52,54,55,44,48,46,48,54,51,53,32,45,48,46,49,53,56,55,53,44,48,46,49,54,57,51,51,32,45,48,46,48,54,51,53,44,48,46,48,57,53,50,32,45,48,46,49,49,54,52,49,55,44,48,46,50,
49,49,54,55,32,45,48,46,48,51,49,55,53,44,48,46,48,56,52,55,32,45,48,46,48,54,51,53,44,48,46,49,55,57,57,50,32,45,48,46,48,50,49,49,55,44,48,46,48,57,53,51,32,45,48,46,48,57,53,50,53,44,48,46,49,52,56,
49,54,32,45,48,46,48,52,50,51,51,44,48,46,48,51,49,56,32,45,48,46,49,52,56,49,54,55,44,48,46,48,53,50,57,32,45,48,46,48,57,53,50,53,44,48,46,48,49,48,54,32,45,48,46,50,48,49,48,56,51,44,48,46,48,49,48,
54,32,45,48,46,49,48,53,56,51,51,44,48,32,45,48,46,50,49,49,54,54,55,44,45,48,46,48,49,48,54,32,45,48,46,48,57,53,50,53,44,45,48,46,48,50,49,50,32,45,48,46,49,51,55,53,56,51,44,45,48,46,48,53,50,57,32,
45,48,46,48,52,50,51,51,44,45,48,46,48,51,49,56,32,45,48,46,48,54,51,53,44,45,48,46,49,48,53,56,51,32,45,48,46,48,50,49,49,55,44,45,48,46,48,55,52,49,32,48,44,45,48,46,49,53,56,55,53,32,48,46,48,51,49,
55,53,44,45,48,46,49,51,55,53,56,32,48,46,48,54,51,53,44,45,48,46,50,53,52,32,48,46,48,52,50,51,51,44,45,48,46,49,50,55,32,48,46,48,57,53,50,53,44,45,48,46,50,52,51,52,50,32,48,46,50,48,49,48,56,51,44,
45,48,46,51,55,48,52,49,32,48,46,53,48,56,44,45,48,46,53,55,49,53,32,48,46,51,48,54,57,49,55,44,45,48,46,50,49,49,54,54,32,48,46,55,54,50,44,45,48,46,51,51,56,54,54,32,48,46,49,48,53,56,51,51,44,45,48,
46,48,50,49,50,32,48,46,50,49,49,54,54,55,44,45,48,46,48,51,49,56,32,48,46,49,49,54,52,49,54,44,45,48,46,48,49,48,54,32,48,46,50,51,50,56,51,51,44,45,48,46,48,51,49,56,32,48,46,48,52,50,51,51,44,45,48,
46,48,49,48,54,32,48,46,49,51,55,53,56,51,44,48,32,48,46,48,57,53,50,53,44,48,32,48,46,49,49,54,52,49,55,44,45,48,46,48,50,49,50,32,48,46,55,52,48,56,51,51,44,45,48,46,48,49,48,54,32,49,46,50,57,49,49,
54,55,44,48,46,49,54,57,51,51,32,48,46,53,53,48,51,51,51,44,48,46,49,55,57,57,50,32,48,46,55,55,50,53,56,51,44,48,46,54,56,55,57,50,32,48,46,48,57,53,50,53,44,48,46,50,49,49,54,55,32,48,46,49,48,53,56,
51,51,44,48,46,52,57,55,52,50,32,48,46,48,50,49,49,55,44,48,46,50,55,53,49,54,32,48,46,48,50,49,49,55,44,48,46,53,56,50,48,56,32,118,32,50,46,51,51,56,57,50,32,113,32,48,44,48,46,49,52,56,49,54,32,48,
46,48,49,48,53,56,44,48,46,50,56,53,55,53,32,48,46,48,49,48,53,56,44,48,46,49,51,55,53,56,32,48,46,48,54,51,53,44,48,46,50,50,50,50,53,32,48,46,48,53,50,57,50,44,48,46,48,55,52,49,32,48,46,49,51,55,53,
56,55,44,48,46,49,48,53,56,51,32,48,46,48,57,53,50,44,48,46,48,52,50,51,32,48,46,49,55,57,57,49,44,48,46,48,54,51,53,32,48,46,48,56,52,55,44,48,46,48,49,48,54,32,48,46,49,50,55,44,48,46,48,57,53,51,32,
48,46,48,50,49,50,44,48,46,48,51,49,55,32,48,46,48,50,49,50,44,48,46,48,54,51,53,32,48,44,48,46,48,51,49,56,32,48,46,48,49,48,54,44,48,46,48,56,52,55,32,122,32,109,32,45,49,46,53,49,51,52,49,51,44,45,
49,46,49,55,52,55,53,32,113,32,48,46,48,54,51,53,44,45,48,46,49,52,56,49,55,32,48,46,48,54,51,53,44,45,48,46,51,51,56,54,55,32,48,46,48,49,48,53,56,44,45,48,46,50,48,49,48,56,32,48,46,48,49,48,53,56,44,
45,48,46,52,49,50,55,53,32,48,44,45,48,46,49,49,54,52,50,32,45,48,46,48,49,48,53,56,44,45,48,46,50,49,49,54,55,32,45,48,46,48,49,48,53,56,44,45,48,46,48,57,53,51,32,45,48,46,48,56,52,54,55,44,45,48,46,
49,52,56,49,54,32,45,48,46,48,51,49,55,53,44,45,48,46,48,50,49,50,32,45,48,46,49,52,56,49,54,55,44,45,48,46,48,50,49,50,32,45,48,46,48,50,49,49,55,44,48,46,48,50,49,50,32,45,48,46,48,55,52,48,56,44,48,
46,48,50,49,50,32,45,48,46,48,53,50,57,50,44,48,32,45,48,46,48,57,53,50,53,44,48,46,48,50,49,50,32,45,48,46,48,56,52,54,55,44,48,46,48,50,49,50,32,45,48,46,49,57,48,53,44,48,46,48,52,50,51,32,45,48,46,
49,48,53,56,51,51,44,48,46,48,50,49,50,32,45,48,46,50,49,49,54,54,55,44,48,46,48,52,50,51,32,45,48,46,48,51,49,55,53,44,48,32,45,48,46,48,55,52,48,56,44,48,46,48,49,48,54,32,45,48,46,48,51,49,55,53,44,
48,32,45,48,46,48,55,52,48,56,44,48,32,45,48,46,49,53,56,55,53,44,48,46,48,52,50,51,32,45,48,46,51,52,57,50,53,44,48,46,48,53,50,57,32,45,48,46,49,55,57,57,49,55,44,48,32,45,48,46,51,51,56,54,54,55,44,
48,46,48,52,50,51,32,45,48,46,48,56,52,54,55,44,48,46,48,50,49,50,32,45,48,46,49,53,56,55,53,44,48,46,48,52,50,51,32,45,48,46,48,54,51,53,44,48,46,48,49,48,54,32,45,48,46,49,52,56,49,54,55,44,48,46,48,
51,49,56,32,45,48,46,49,54,57,51,51,51,44,48,46,48,53,50,57,32,45,48,46,51,49,55,53,44,48,46,49,51,55,53,56,32,45,48,46,49,52,56,49,54,54,44,48,46,48,55,52,49,32,45,48,46,50,53,52,44,48,46,49,57,48,53,
32,45,48,46,49,48,53,56,51,51,44,48,46,49,49,54,52,50,32,45,48,46,49,53,56,55,53,44,48,46,50,56,53,55,53,32,45,48,46,48,53,50,57,50,44,48,46,49,53,56,55,53,32,45,48,46,48,52,50,51,51,44,48,46,51,57,49,
53,56,32,48,44,48,46,49,48,53,56,52,32,48,46,48,51,49,55,53,44,48,46,50,48,49,48,57,32,48,46,48,52,50,51,51,44,48,46,48,56,52,55,32,48,46,48,57,53,50,53,44,48,46,49,53,56,55,53,32,48,46,48,56,52,54,55,
44,48,46,49,49,54,52,49,32,48,46,50,50,50,50,53,44,48,46,50,48,49,48,56,32,48,46,49,51,55,53,56,51,44,48,46,48,55,52,49,32,48,46,50,57,54,51,51,51,44,48,46,49,50,55,32,48,46,48,53,50,57,50,44,48,46,48,
49,48,54,32,48,46,49,48,53,56,51,52,44,48,46,48,49,48,54,32,48,46,48,53,50,57,50,44,48,32,48,46,49,48,53,56,51,51,44,48,46,48,49,48,54,32,48,46,48,55,52,48,56,44,48,46,48,50,49,50,32,48,46,49,55,57,57,
49,55,44,48,46,48,51,49,55,32,48,46,49,49,54,52,49,54,44,48,32,48,46,49,57,48,53,44,45,48,46,48,50,49,50,32,48,46,48,54,51,53,44,45,48,46,48,50,49,50,32,48,46,49,50,55,44,45,48,46,48,49,48,54,32,48,46,
48,54,51,53,44,48,32,48,46,49,50,55,44,45,48,46,48,50,49,50,32,48,46,48,56,52,54,55,44,45,48,46,48,50,49,50,32,48,46,49,53,56,55,53,44,45,48,46,48,51,49,55,32,48,46,48,56,52,54,55,44,45,48,46,48,49,48,
54,32,48,46,49,53,56,55,53,44,45,48,46,48,52,50,51,32,48,46,51,48,54,57,49,54,44,45,48,46,49,50,55,32,48,46,53,49,56,53,56,51,44,45,48,46,51,48,54,57,50,32,48,46,50,50,50,50,53,44,45,48,46,49,55,57,57,
50,32,48,46,51,51,56,54,54,55,44,45,48,46,52,56,54,56,51,32,122,34,32,47,62,10,32,32,32,32,32,32,32,32,60,112,97,116,104,10,32,32,32,32,32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,51,56,51,57,34,
10,32,32,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,111,110,116,45,115,116,121,108,101,58,110,111,114,109,97,108,59,102,111,110,116,45,118,97,114,105,97,110,116,58,110,111,114,109,97,108,
59,102,111,110,116,45,119,101,105,103,104,116,58,110,111,114,109,97,108,59,102,111,110,116,45,115,116,114,101,116,99,104,58,110,111,114,109,97,108,59,102,111,110,116,45,102,97,109,105,108,121,58,39,65,
108,116,101,32,72,97,97,115,32,71,114,111,116,101,115,107,39,59,45,105,110,107,115,99,97,112,101,45,102,111,110,116,45,115,112,101,99,105,102,105,99,97,116,105,111,110,58,39,65,108,116,101,32,72,97,97,
115,32,71,114,111,116,101,115,107,39,59,102,105,108,108,58,35,102,50,102,50,102,50,59,115,116,114,111,107,101,45,119,105,100,116,104,58,48,46,50,54,52,53,56,51,51,50,34,10,32,32,32,32,32,32,32,32,32,32,
32,100,61,34,109,32,49,48,51,46,56,50,51,50,51,44,49,48,53,46,52,52,55,55,49,32,113,32,48,46,52,48,50,49,55,44,45,48,46,48,49,48,54,32,48,46,54,55,55,51,52,44,48,46,48,56,52,55,32,48,46,50,56,53,55,53,
44,48,46,48,57,53,50,32,48,46,53,49,56,53,56,44,48,46,50,55,53,49,55,32,48,46,48,56,52,55,44,48,46,48,53,50,57,32,48,46,49,51,55,53,56,44,48,46,49,52,56,49,54,32,48,46,48,54,51,53,44,48,46,48,56,52,55,
32,48,46,49,51,55,53,57,44,48,46,49,53,56,55,53,32,48,46,48,50,49,50,44,48,46,48,50,49,50,32,48,46,48,54,51,53,44,48,46,48,53,50,57,32,48,46,48,52,50,51,44,48,46,48,51,49,56,32,48,46,49,48,53,56,51,44,
48,46,48,50,49,50,32,48,46,49,48,53,56,51,44,45,48,46,48,51,49,56,32,48,46,49,55,57,57,50,44,45,48,46,49,50,55,32,48,46,48,56,52,55,44,45,48,46,49,48,53,56,52,32,48,46,49,54,57,51,51,44,45,48,46,49,53,
56,55,53,32,48,46,49,52,56,49,55,44,45,48,46,49,49,54,52,50,32,48,46,51,48,54,57,50,44,45,48,46,50,48,49,48,57,32,48,46,49,53,56,55,53,44,45,48,46,48,57,53,51,32,48,46,51,52,57,50,53,44,45,48,46,49,53,
56,55,53,32,48,46,48,56,52,55,44,45,48,46,48,51,49,55,32,48,46,49,55,57,57,49,44,45,48,46,48,52,50,51,32,48,46,49,48,53,56,52,44,45,48,46,48,50,49,50,32,48,46,50,48,49,48,57,44,45,48,46,48,52,50,51,32,
48,46,48,53,50,57,44,45,48,46,48,49,48,54,32,48,46,48,57,53,50,44,45,48,46,48,49,48,54,32,48,46,48,53,50,57,44,48,32,48,46,49,48,53,56,51,44,48,32,48,46,48,53,50,57,44,45,48,46,48,50,49,50,32,48,46,49,
52,56,49,55,44,45,48,46,48,49,48,54,32,48,46,48,57,53,50,44,48,32,48,46,49,51,55,53,56,44,48,46,48,49,48,54,32,48,46,48,53,50,57,44,48,46,48,49,48,54,32,48,46,49,53,56,55,53,44,48,46,48,49,48,54,32,48,
46,48,57,53,51,44,48,46,48,50,49,50,32,48,46,49,55,57,57,50,44,48,46,48,52,50,51,32,48,46,48,57,53,51,44,48,46,48,49,48,54,32,48,46,49,55,57,57,49,44,48,46,48,52,50,51,32,48,46,48,53,50,57,44,48,46,48,
50,49,50,32,48,46,48,57,53,50,44,48,46,48,51,49,56,32,48,46,48,53,50,57,44,48,32,48,46,49,48,53,56,52,44,48,46,48,50,49,50,32,48,46,53,50,57,49,54,44,48,46,50,51,50,56,51,32,48,46,55,51,48,50,53,44,48,
46,55,49,57,54,55,32,48,46,48,52,50,51,44,48,46,48,56,52,55,32,48,46,48,54,51,53,44,48,46,49,57,48,53,32,48,46,48,50,49,50,44,48,46,48,57,53,50,32,48,46,48,52,50,51,44,48,46,50,49,49,54,54,32,48,46,48,
50,49,50,44,48,46,48,51,49,56,32,48,46,48,49,48,54,44,48,46,48,55,52,49,32,48,44,48,46,48,51,49,56,32,48,46,48,49,48,54,44,48,46,48,54,51,53,32,118,32,48,46,48,57,53,51,32,113,32,48,46,48,49,48,54,44,
48,46,48,52,50,51,32,48,44,48,46,48,57,53,50,32,48,44,48,46,48,53,50,57,32,48,44,48,46,49,48,53,56,51,32,118,32,48,46,52,49,50,55,53,32,48,46,54,55,55,51,51,32,50,46,48,49,48,56,52,32,113,32,48,44,48,
46,49,54,57,51,51,32,48,44,48,46,52,48,50,49,54,32,48,46,48,49,48,54,44,48,46,50,51,50,56,52,32,45,48,46,48,56,52,55,44,48,46,51,48,54,57,50,32,45,48,46,48,52,50,51,44,48,46,48,51,49,56,32,45,48,46,49,
51,55,53,56,44,48,46,48,53,50,57,32,45,48,46,48,57,53,50,44,48,46,48,49,48,54,32,45,48,46,50,48,49,48,57,44,48,46,48,49,48,54,32,45,48,46,49,48,53,56,51,44,48,32,45,48,46,50,49,49,54,54,44,45,48,46,48,
49,48,54,32,45,48,46,48,57,53,50,44,45,48,46,48,49,48,54,32,45,48,46,49,51,55,53,57,44,45,48,46,48,52,50,51,32,45,48,46,48,54,51,53,44,45,48,46,48,51,49,56,32,45,48,46,48,56,52,55,44,45,48,46,49,50,55,
32,45,48,46,48,50,49,50,44,45,48,46,48,54,51,53,32,45,48,46,48,49,48,54,44,45,48,46,49,50,55,32,48,46,48,49,48,54,44,45,48,46,48,55,52,49,32,45,48,46,48,49,48,54,44,45,48,46,49,51,55,53,56,32,45,48,46,
48,49,48,54,44,45,48,46,48,53,50,57,32,45,48,46,48,49,48,54,44,45,48,46,49,49,54,52,50,32,48,46,48,49,48,54,44,45,48,46,48,55,52,49,32,48,46,48,49,48,54,44,45,48,46,49,51,55,53,56,32,118,32,45,48,46,52,
55,54,50,53,32,45,50,46,48,50,49,52,50,32,113,32,48,44,45,48,46,51,52,57,50,53,32,45,48,46,48,50,49,50,44,45,48,46,54,53,54,49,54,32,45,48,46,48,49,48,54,44,45,48,46,51,49,55,53,32,45,48,46,49,52,56,49,
54,44,45,48,46,53,50,57,49,55,32,45,48,46,49,53,56,55,53,44,45,48,46,50,53,52,32,45,48,46,52,56,54,56,52,44,45,48,46,51,55,48,52,50,32,45,48,46,48,54,51,53,44,45,48,46,48,50,49,50,32,45,48,46,49,51,55,
53,56,44,45,48,46,48,50,49,50,32,45,48,46,48,54,51,53,44,45,48,46,48,49,48,54,32,45,48,46,49,51,55,53,56,44,45,48,46,48,51,49,56,32,45,48,46,48,52,50,51,44,45,48,46,48,49,48,54,32,45,48,46,49,51,55,53,
57,44,48,32,45,48,46,48,57,53,51,44,48,32,45,48,46,49,50,55,44,48,46,48,49,48,54,32,45,48,46,48,53,50,57,44,48,46,48,49,48,54,32,45,48,46,49,48,53,56,51,44,48,46,48,49,48,54,32,45,48,46,48,53,50,57,44,
45,48,46,48,49,48,54,32,45,48,46,49,48,53,56,51,44,48,46,48,49,48,54,32,45,48,46,49,52,56,49,55,44,48,46,48,51,49,56,32,45,48,46,50,56,53,55,53,44,48,46,48,56,52,55,32,45,48,46,53,53,48,51,52,44,48,46,
50,51,50,56,52,32,45,48,46,55,52,48,56,52,44,48,46,56,49,52,57,50,32,45,48,46,48,53,50,57,44,48,46,49,55,57,57,50,32,45,48,46,48,54,51,53,44,48,46,52,49,50,55,53,32,48,44,48,46,50,51,50,56,51,32,48,44,
48,46,52,55,54,50,53,32,118,32,49,46,51,56,54,52,50,32,49,46,48,50,54,53,56,32,113,32,48,44,48,46,49,50,55,32,48,44,48,46,50,54,52,53,56,32,48,44,48,46,49,50,55,32,45,48,46,48,53,50,57,44,48,46,50,48,
49,48,57,32,45,48,46,48,53,50,57,44,48,46,48,56,52,55,32,45,48,46,49,55,57,57,50,44,48,46,48,57,53,50,32,45,48,46,49,50,55,44,48,46,48,49,48,54,32,45,48,46,50,54,52,53,56,44,48,46,48,49,48,54,32,45,48,
46,49,55,57,57,50,44,48,32,45,48,46,50,54,52,53,57,44,45,48,46,48,52,50,51,32,45,48,46,48,57,53,50,44,45,48,46,48,53,50,57,32,45,48,46,49,49,54,52,49,44,45,48,46,49,54,57,51,52,32,45,48,46,48,49,48,54,
44,45,48,46,49,50,55,32,45,48,46,48,49,48,54,44,45,48,46,50,55,53,49,54,32,118,32,45,49,46,48,48,53,52,50,32,45,49,46,55,52,54,50,53,32,113,32,48,44,45,48,46,51,51,56,54,55,32,45,48,46,48,50,49,50,44,
45,48,46,54,50,52,52,50,32,45,48,46,48,50,49,50,44,45,48,46,50,57,54,51,51,32,45,48,46,49,52,56,49,55,44,45,48,46,53,48,56,32,45,48,46,49,49,54,52,50,44,45,48,46,49,57,48,53,32,45,48,46,50,54,52,53,56,
44,45,48,46,50,55,53,49,54,32,45,48,46,49,52,56,49,55,44,45,48,46,48,57,53,51,32,45,48,46,52,48,50,49,55,44,45,48,46,49,52,56,49,55,32,45,48,46,48,52,50,51,44,45,48,46,48,49,48,54,32,45,48,46,48,57,53,
51,44,45,48,46,48,49,48,54,32,45,48,46,48,52,50,51,44,48,32,45,48,46,48,57,53,50,44,48,32,45,48,46,50,51,50,56,51,44,48,32,45,48,46,52,49,50,55,53,44,48,46,48,53,50,57,32,45,48,46,49,54,57,51,51,44,48,
46,48,53,50,57,32,45,48,46,51,48,54,57,50,44,48,46,49,51,55,53,57,32,45,48,46,53,51,57,55,53,44,48,46,51,50,56,48,56,32,45,48,46,55,49,57,54,54,44,48,46,57,56,52,50,53,32,45,48,46,48,52,50,51,44,48,46,
49,51,55,53,56,32,45,48,46,48,52,50,51,44,48,46,51,49,55,53,32,48,46,48,49,48,54,44,48,46,49,54,57,51,51,32,48,46,48,49,48,54,44,48,46,51,51,56,54,54,32,118,32,49,46,49,57,53,57,50,32,49,46,49,54,52,49,
55,32,113,32,48,44,48,46,49,51,55,53,56,32,48,44,48,46,50,56,53,55,53,32,48,44,48,46,49,52,56,49,54,32,45,48,46,48,54,51,53,44,48,46,50,51,50,56,51,32,45,48,46,48,53,50,57,44,48,46,48,55,52,49,32,45,48,
46,49,53,56,55,53,44,48,46,48,56,52,55,32,45,48,46,49,48,53,56,52,44,48,46,48,49,48,54,32,45,48,46,50,50,50,50,53,44,48,46,48,49,48,54,32,104,32,45,48,46,49,53,56,55,53,32,113,32,45,48,46,48,54,51,53,
44,45,48,46,48,50,49,50,32,45,48,46,49,50,55,44,45,48,46,48,51,49,56,32,45,48,46,48,53,50,57,44,45,48,46,48,49,48,54,32,45,48,46,48,57,53,51,44,45,48,46,48,54,51,53,32,45,48,46,48,53,50,57,44,45,48,46,
48,54,51,53,32,45,48,46,48,53,50,57,44,45,48,46,49,53,56,55,53,32,48,44,45,48,46,49,48,53,56,51,32,48,44,45,48,46,50,50,50,50,53,32,118,32,45,48,46,54,56,55,57,50,32,45,51,46,48,50,54,56,51,32,45,48,46,
55,56,51,49,55,32,113,32,48,44,45,48,46,49,48,53,56,51,32,48,44,45,48,46,50,48,49,48,56,32,48,44,45,48,46,49,48,53,56,51,32,48,46,48,52,50,51,44,45,48,46,49,54,57,51,51,32,48,46,48,51,49,55,44,45,48,46,
48,54,51,53,32,48,46,48,57,53,50,44,45,48,46,48,56,52,55,32,48,46,48,56,52,55,44,45,48,46,48,51,49,55,32,48,46,50,54,52,53,57,44,45,48,46,48,51,49,55,32,48,46,49,57,48,53,44,48,32,48,46,50,55,53,49,54,
44,48,46,48,51,49,55,32,48,46,48,56,52,55,44,48,46,48,52,50,51,32,48,46,49,49,54,52,50,44,48,46,49,48,53,56,51,32,48,46,48,50,49,50,44,48,46,48,53,50,57,32,48,46,48,50,49,50,44,48,46,49,50,55,32,48,46,
48,49,48,54,44,48,46,48,55,52,49,32,48,46,48,50,49,50,44,48,46,49,51,55,53,57,32,48,46,48,49,48,54,44,48,46,48,54,51,53,32,48,46,48,52,50,51,44,48,46,49,48,53,56,51,32,48,46,48,51,49,56,44,48,46,48,52,
50,51,32,48,46,49,48,53,56,51,44,48,46,48,52,50,51,32,48,46,48,50,49,50,44,45,48,46,48,50,49,50,32,48,46,48,52,50,51,44,45,48,46,48,50,49,50,32,48,46,48,51,49,56,44,48,32,48,46,48,53,50,57,44,45,48,46,
48,50,49,50,32,48,46,48,54,51,53,44,45,48,46,48,53,50,57,32,48,46,49,49,54,52,50,44,45,48,46,49,48,53,56,51,32,48,46,48,54,51,53,44,45,48,46,48,53,50,57,32,48,46,49,50,55,44,45,48,46,49,48,53,56,52,32,
48,46,50,51,50,56,51,44,45,48,46,49,54,57,51,51,32,48,46,53,50,57,49,54,44,45,48,46,50,57,54,51,51,32,48,46,49,48,53,56,52,44,45,48,46,48,52,50,51,32,48,46,50,49,49,54,55,44,45,48,46,48,53,50,57,32,48,
46,49,48,53,56,51,44,45,48,46,48,50,49,50,32,48,46,50,50,50,50,53,44,45,48,46,48,53,50,57,32,48,46,48,54,51,53,44,48,32,48,46,49,50,55,44,48,32,48,46,48,55,52,49,44,45,48,46,48,49,48,54,32,48,46,49,51,
55,53,56,44,45,48,46,48,49,48,54,32,122,34,32,47,62,10,32,32,32,32,32,32,32,32,60,112,97,116,104,10,32,32,32,32,32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,51,56,52,49,34,10,32,32,32,32,32,32,32,
32,32,32,32,115,116,121,108,101,61,34,102,111,110,116,45,115,116,121,108,101,58,110,111,114,109,97,108,59,102,111,110,116,45,118,97,114,105,97,110,116,58,110,111,114,109,97,108,59,102,111,110,116,45,119,
101,105,103,104,116,58,110,111,114,109,97,108,59,102,111,110,116,45,115,116,114,101,116,99,104,58,110,111,114,109,97,108,59,102,111,110,116,45,102,97,109,105,108,121,58,39,65,108,116,101,32,72,97,97,115,
32,71,114,111,116,101,115,107,39,59,45,105,110,107,115,99,97,112,101,45,102,111,110,116,45,115,112,101,99,105,102,105,99,97,116,105,111,110,58,39,65,108,116,101,32,72,97,97,115,32,71,114,111,116,101,115,
107,39,59,102,105,108,108,58,35,102,50,102,50,102,50,59,115,116,114,111,107,101,45,119,105,100,116,104,58,48,46,50,54,52,53,56,51,51,50,34,10,32,32,32,32,32,32,32,32,32,32,32,100,61,34,109,32,49,49,48,
46,52,53,51,56,54,44,49,48,52,46,53,49,54,51,56,32,113,32,45,48,46,48,56,52,55,44,45,48,46,48,54,51,53,32,45,48,46,48,57,53,51,44,45,48,46,50,50,50,50,53,32,48,44,45,48,46,49,54,57,51,51,32,48,44,45,48,
46,51,50,56,48,56,32,48,44,45,48,46,48,56,52,55,32,48,44,45,48,46,49,55,57,57,50,32,48,44,45,48,46,48,57,53,50,32,48,46,48,51,49,56,44,45,48,46,49,53,56,55,53,32,48,46,48,52,50,51,44,45,48,46,48,56,52,
55,32,48,46,49,54,57,51,51,44,45,48,46,49,50,55,32,48,46,48,50,49,50,44,45,48,46,48,49,48,54,32,48,46,48,52,50,51,44,48,32,48,46,48,51,49,56,44,48,32,48,46,48,53,50,57,44,45,48,46,48,49,48,54,32,48,46,
49,55,57,57,50,44,48,32,48,46,51,51,56,54,55,44,48,46,48,49,48,54,32,48,46,49,53,56,55,53,44,48,46,48,49,48,54,32,48,46,50,49,49,54,54,44,48,46,49,50,55,32,48,46,48,51,49,56,44,48,46,48,54,51,53,32,48,
46,48,51,49,56,44,48,46,49,53,56,55,53,32,48,44,48,46,48,56,52,55,32,48,44,48,46,49,55,57,57,50,32,48,44,48,46,49,57,48,53,32,45,48,46,48,49,48,54,44,48,46,51,53,57,56,51,32,48,44,48,46,49,54,57,51,51,
32,45,48,46,49,51,55,53,56,44,48,46,50,50,50,50,53,32,45,48,46,48,53,50,57,44,48,46,48,50,49,50,32,45,48,46,49,49,54,52,50,44,48,46,48,50,49,50,32,45,48,46,48,53,50,57,44,48,32,45,48,46,49,49,54,52,50,
44,48,32,45,48,46,49,49,54,52,49,44,48,32,45,48,46,50,50,50,50,53,44,48,32,45,48,46,49,48,53,56,51,44,48,32,45,48,46,49,55,57,57,49,44,45,48,46,48,53,50,57,32,122,32,109,32,48,46,55,56,51,49,54,44,54,
46,48,48,48,55,53,32,113,32,48,44,48,46,49,48,53,56,51,32,48,44,48,46,50,50,50,50,53,32,48,46,48,49,48,54,44,48,46,49,48,53,56,51,32,45,48,46,48,52,50,51,44,48,46,49,54,57,51,51,32,45,48,46,48,51,49,55,
44,48,46,48,54,51,53,32,45,48,46,48,57,53,50,44,48,46,48,56,52,55,32,45,48,46,48,54,51,53,44,48,46,48,49,48,54,32,45,48,46,49,52,56,49,55,44,48,46,48,51,49,56,32,104,32,45,48,46,49,53,56,55,53,32,113,
32,45,48,46,49,51,55,53,56,44,48,32,45,48,46,50,52,51,52,49,44,45,48,46,48,49,48,54,32,45,48,46,48,57,53,50,44,45,48,46,48,50,49,50,32,45,48,46,49,52,56,49,55,44,45,48,46,49,48,53,56,52,32,45,48,46,48,
52,50,51,44,45,48,46,48,54,51,53,32,45,48,46,48,52,50,51,44,45,48,46,49,53,56,55,53,32,48,44,45,48,46,49,48,53,56,51,32,48,44,45,48,46,50,48,49,48,56,32,118,32,45,52,46,52,55,54,55,53,32,113,32,48,44,
45,48,46,49,49,54,52,50,32,48,44,45,48,46,50,50,50,50,53,32,48,44,45,48,46,49,49,54,52,50,32,48,46,48,53,50,57,44,45,48,46,49,55,57,57,50,32,48,46,48,51,49,56,44,45,48,46,48,53,50,57,32,48,46,48,56,52,
55,44,45,48,46,48,54,51,53,32,48,46,48,53,50,57,44,45,48,46,48,49,48,54,32,48,46,49,49,54,52,50,44,45,48,46,48,51,49,56,32,48,46,48,50,49,50,44,48,32,48,46,48,52,50,51,44,48,46,48,49,48,54,32,48,46,48,
51,49,56,44,48,32,48,46,48,53,50,57,44,45,48,46,48,49,48,54,32,48,46,49,51,55,53,56,44,48,32,48,46,50,55,53,49,54,44,48,46,48,49,48,54,32,48,46,49,52,56,49,55,44,48,32,48,46,50,48,49,48,57,44,48,46,48,
56,52,55,32,48,46,48,53,50,57,44,48,46,48,54,51,53,32,48,46,48,53,50,57,44,48,46,49,54,57,51,52,32,48,44,48,46,48,57,53,51,32,48,44,48,46,50,48,49,48,56,32,122,34,32,47,62,10,32,32,32,32,32,32,32,32,60,
112,97,116,104,10,32,32,32,32,32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,51,56,52,51,34,10,32,32,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,111,110,116,45,115,116,121,108,101,58,110,
111,114,109,97,108,59,102,111,110,116,45,118,97,114,105,97,110,116,58,110,111,114,109,97,108,59,102,111,110,116,45,119,101,105,103,104,116,58,110,111,114,109,97,108,59,102,111,110,116,45,115,116,114,101,
116,99,104,58,110,111,114,109,97,108,59,102,111,110,116,45,102,97,109,105,108,121,58,39,65,108,116,101,32,72,97,97,115,32,71,114,111,116,101,115,107,39,59,45,105,110,107,115,99,97,112,101,45,102,111,110,
116,45,115,112,101,99,105,102,105,99,97,116,105,111,110,58,39,65,108,116,101,32,72,97,97,115,32,71,114,111,116,101,115,107,39,59,102,105,108,108,58,35,102,50,102,50,102,50,59,115,116,114,111,107,101,45,
119,105,100,116,104,58,48,46,50,54,52,53,56,51,51,50,34,10,32,32,32,32,32,32,32,32,32,32,32,100,61,34,109,32,49,49,52,46,56,50,49,49,51,44,49,48,53,46,52,52,55,55,49,32,113,32,48,46,55,56,51,49,55,44,
45,48,46,48,49,48,54,32,49,46,51,48,49,55,53,44,48,46,50,51,50,56,52,32,48,46,53,50,57,49,55,44,48,46,50,51,50,56,51,32,48,46,56,51,54,48,56,44,48,46,55,48,57,48,56,32,48,46,49,51,55,53,57,44,48,46,50,
49,49,54,55,32,48,46,50,51,50,56,52,44,48,46,53,49,56,53,56,32,48,46,48,50,49,50,44,48,46,48,53,50,57,32,48,46,48,51,49,56,44,48,46,49,51,55,53,57,32,48,46,48,49,48,54,44,48,46,48,56,52,55,32,45,48,46,
48,49,48,54,44,48,46,49,52,56,49,54,32,45,48,46,48,51,49,56,44,48,46,48,53,50,57,32,45,48,46,49,48,53,56,51,44,48,46,48,57,53,50,32,45,48,46,48,51,49,56,44,48,46,48,50,49,50,32,45,48,46,48,54,51,53,44,
48,46,48,50,49,50,32,45,48,46,48,51,49,55,44,45,48,46,48,49,48,54,32,45,48,46,48,55,52,49,44,48,32,45,48,46,48,52,50,51,44,48,46,48,49,48,54,32,45,48,46,48,56,52,55,44,48,46,48,49,48,54,32,45,48,46,48,
52,50,51,44,45,48,46,48,49,48,54,32,45,48,46,48,56,52,55,44,45,48,46,48,49,48,54,32,45,48,46,49,49,54,52,49,44,48,32,45,48,46,50,50,50,50,53,44,48,32,45,48,46,49,48,53,56,51,44,45,48,46,48,49,48,54,32,
45,48,46,49,55,57,57,49,44,45,48,46,48,54,51,53,32,45,48,46,48,56,52,55,44,45,48,46,48,54,51,53,32,45,48,46,49,49,54,52,50,44,45,48,46,49,55,57,57,50,32,45,48,46,48,51,49,56,44,45,48,46,49,49,54,52,49,
32,45,48,46,48,56,52,55,44,45,48,46,50,48,49,48,56,32,45,48,46,49,53,56,55,53,44,45,48,46,50,55,53,49,55,32,45,48,46,52,49,50,55,53,44,45,48,46,52,51,51,57,50,32,45,48,46,48,56,52,55,44,45,48,46,48,53,
50,57,32,45,48,46,49,55,57,57,49,44,45,48,46,48,56,52,55,32,45,48,46,48,57,53,50,44,45,48,46,48,52,50,51,32,45,48,46,50,48,49,48,57,44,45,48,46,48,55,52,49,32,45,48,46,49,52,56,49,54,44,45,48,46,48,52,
50,51,32,45,48,46,51,57,49,53,56,44,45,48,46,48,52,50,51,32,45,48,46,50,52,51,52,50,44,45,48,46,48,49,48,54,32,45,48,46,52,48,50,49,55,44,48,46,48,52,50,51,32,45,48,46,48,55,52,49,44,48,46,48,50,49,50,
32,45,48,46,49,51,55,53,56,44,48,46,48,51,49,55,32,45,48,46,48,54,51,53,44,48,46,48,49,48,54,32,45,48,46,49,51,55,53,56,44,48,46,48,51,49,56,32,45,48,46,50,55,53,49,55,44,48,46,49,49,54,52,50,32,45,48,
46,52,56,54,56,52,44,48,46,51,49,55,53,32,45,48,46,50,49,49,54,54,44,48,46,50,48,49,48,57,32,45,48,46,51,50,56,48,56,44,48,46,52,55,54,50,53,32,45,48,46,48,52,50,51,44,48,46,48,57,53,51,32,45,48,46,48,
55,52,49,44,48,46,50,48,49,48,57,32,45,48,46,48,51,49,56,44,48,46,48,57,53,51,32,45,48,46,48,54,51,53,44,48,46,50,49,49,54,54,32,108,32,45,48,46,48,54,51,53,44,48,46,50,56,53,55,53,32,113,32,45,48,46,
48,49,48,54,44,48,46,48,53,50,57,32,45,48,46,48,49,48,54,44,48,46,49,49,54,52,50,32,48,46,48,49,48,54,44,48,46,48,54,51,53,32,45,48,46,48,49,48,54,44,48,46,49,49,54,52,50,32,118,32,48,46,48,56,52,55,32,
113,32,45,48,46,48,49,48,54,44,48,46,48,52,50,51,32,45,48,46,48,50,49,50,44,48,46,49,51,55,53,57,32,48,44,48,46,48,57,53,51,32,48,46,48,50,49,50,44,48,46,49,51,55,53,56,32,48,46,48,49,48,54,44,48,46,48,
53,50,57,32,48,44,48,46,49,48,53,56,51,32,45,48,46,48,49,48,54,44,48,46,48,53,50,57,32,48,44,48,46,49,48,53,56,52,32,48,46,48,49,48,54,44,48,46,48,52,50,51,32,48,46,48,49,48,54,44,48,46,49,52,56,49,54,
32,48,46,48,50,49,50,44,48,46,48,56,52,55,32,48,46,48,51,49,56,44,48,46,49,55,57,57,50,32,48,46,48,50,49,50,44,48,46,48,57,53,50,32,48,46,48,52,50,51,44,48,46,49,55,57,57,50,32,48,46,49,49,54,52,49,44,
48,46,51,51,56,54,54,32,48,46,50,54,52,53,56,44,48,46,53,57,50,54,54,32,48,46,49,53,56,55,53,44,48,46,50,52,51,52,50,32,48,46,52,51,51,57,50,44,48,46,52,50,51,51,52,32,48,46,48,57,53,51,44,48,46,48,53,
50,57,32,48,46,49,57,48,53,44,48,46,48,57,53,50,32,48,46,49,48,53,56,51,44,48,46,48,52,50,51,32,48,46,50,50,50,50,53,44,48,46,48,56,52,55,32,48,46,48,53,50,57,44,48,46,48,49,48,54,32,48,46,48,56,52,55,
44,48,46,48,49,48,54,32,48,46,48,51,49,56,44,48,32,48,46,48,56,52,55,44,48,46,48,49,48,54,32,48,46,48,51,49,56,44,48,46,48,49,48,54,32,48,46,49,48,53,56,51,44,48,46,48,49,48,54,32,104,32,48,46,49,49,54,
52,50,32,113,32,48,44,48,46,48,49,48,54,32,48,46,48,52,50,51,44,48,46,48,49,48,54,32,48,46,48,50,49,50,44,45,48,46,48,49,48,54,32,48,46,48,53,50,57,44,48,32,48,46,48,51,49,56,44,48,32,48,46,48,54,51,53,
44,45,48,46,48,49,48,54,32,48,46,48,55,52,49,44,45,48,46,48,50,49,50,32,48,46,49,51,55,53,56,44,45,48,46,48,49,48,54,32,48,46,48,55,52,49,44,48,32,48,46,49,52,56,49,55,44,45,48,46,48,50,49,50,32,48,46,
50,55,53,49,55,44,45,48,46,48,56,52,55,32,48,46,52,55,54,50,53,44,45,48,46,50,49,49,54,54,32,48,46,50,48,49,48,56,44,45,48,46,49,51,55,53,57,32,48,46,51,51,56,54,55,44,45,48,46,51,52,57,50,53,32,48,46,
48,53,50,57,44,45,48,46,48,56,52,55,32,48,46,48,57,53,51,44,45,48,46,49,55,57,57,50,32,48,46,48,52,50,51,44,45,48,46,48,57,53,50,32,48,46,48,56,52,55,44,45,48,46,49,57,48,53,32,48,46,48,50,49,50,44,45,
48,46,48,56,52,55,32,48,46,48,52,50,51,44,45,48,46,49,53,56,55,53,32,48,46,48,51,49,56,44,45,48,46,48,56,52,55,32,48,46,49,48,53,56,51,44,45,48,46,49,51,55,53,56,32,48,46,48,51,49,55,44,45,48,46,48,51,
49,56,32,48,46,49,50,55,44,45,48,46,48,52,50,51,32,48,46,48,57,53,50,44,45,48,46,48,50,49,50,32,48,46,50,48,49,48,56,44,45,48,46,48,50,49,50,32,48,46,49,49,54,52,50,44,45,48,46,48,49,48,54,32,48,46,50,
49,49,54,55,44,48,32,48,46,49,48,53,56,51,44,48,46,48,49,48,54,32,48,46,49,52,56,49,55,44,48,46,48,52,50,51,32,48,46,48,54,51,53,44,48,46,48,52,50,51,32,48,46,48,55,52,49,44,48,46,49,50,55,32,48,46,48,
50,49,50,44,48,46,48,56,52,55,32,48,44,48,46,49,55,57,57,50,32,45,48,46,48,53,50,57,44,48,46,49,53,56,55,53,32,45,48,46,49,48,53,56,51,44,48,46,51,48,54,57,49,32,45,48,46,48,52,50,51,44,48,46,49,51,55,
53,57,32,45,48,46,49,49,54,52,50,44,48,46,50,55,53,49,55,32,45,48,46,49,53,56,55,53,44,48,46,51,49,55,53,32,45,48,46,52,48,50,49,55,44,48,46,53,54,48,57,50,32,45,48,46,50,51,50,56,51,44,48,46,50,52,51,
52,49,32,45,48,46,53,55,49,53,44,48,46,51,57,49,53,56,32,45,48,46,48,56,52,55,44,48,46,48,52,50,51,32,45,48,46,49,55,57,57,49,44,48,46,48,55,52,49,32,45,48,46,48,57,53,50,44,48,46,48,50,49,50,32,45,48,
46,50,48,49,48,57,44,48,46,48,53,50,57,32,45,48,46,49,49,54,52,49,44,48,46,48,52,50,51,32,45,48,46,50,51,50,56,51,44,48,46,48,52,50,51,32,45,48,46,49,49,54,52,50,44,48,46,48,49,48,54,32,45,48,46,50,51,
50,56,51,44,48,46,48,51,49,56,32,45,48,46,48,52,50,51,44,48,46,48,50,49,50,32,45,48,46,48,56,52,55,44,48,46,48,49,48,54,32,45,48,46,48,52,50,51,44,45,48,46,48,49,48,54,32,45,48,46,48,56,52,55,44,48,32,
104,32,45,48,46,48,57,53,51,32,113,32,45,48,46,48,56,52,55,44,48,32,45,48,46,49,54,57,51,51,44,48,32,45,48,46,48,55,52,49,44,48,46,48,49,48,54,32,45,48,46,49,51,55,53,56,44,45,48,46,48,49,48,54,32,104,
32,45,48,46,48,53,50,57,32,108,32,45,48,46,50,53,52,44,45,48,46,48,51,49,56,32,113,32,45,48,46,49,51,55,53,56,44,45,48,46,48,52,50,51,32,45,48,46,50,54,52,53,56,44,45,48,46,48,55,52,49,32,45,48,46,49,
50,55,44,45,48,46,48,51,49,56,32,45,48,46,50,52,51,52,50,44,45,48,46,48,56,52,55,32,45,48,46,51,56,49,44,45,48,46,49,54,57,51,51,32,45,48,46,54,51,53,44,45,48,46,52,48,50,49,54,32,45,48,46,50,52,51,52,
50,44,45,48,46,50,51,50,56,52,32,45,48,46,52,53,53,48,56,44,45,48,46,53,55,49,53,32,45,48,46,48,53,50,57,44,45,48,46,48,56,52,55,32,45,48,46,48,57,53,51,44,45,48,46,49,57,48,53,32,45,48,46,48,52,50,51,
44,45,48,46,49,48,53,56,52,32,45,48,46,48,56,52,55,44,45,48,46,50,49,49,54,55,32,45,48,46,48,53,50,57,44,45,48,46,49,49,54,52,50,32,45,48,46,48,55,52,49,44,45,48,46,50,52,51,52,50,32,45,48,46,48,50,49,
50,44,45,48,46,49,50,55,32,45,48,46,48,53,50,57,44,45,48,46,50,53,52,32,45,48,46,48,49,48,54,44,45,48,46,48,52,50,51,32,45,48,46,48,49,48,54,44,45,48,46,48,56,52,55,32,48,44,45,48,46,48,52,50,51,32,45,
48,46,48,49,48,54,44,45,48,46,48,56,52,55,32,45,48,46,48,50,49,50,44,45,48,46,48,54,51,53,32,45,48,46,48,50,49,50,44,45,48,46,49,51,55,53,56,32,48,46,48,49,48,54,44,45,48,46,48,55,52,49,32,45,48,46,48,
49,48,54,44,45,48,46,49,51,55,53,57,32,118,32,45,48,46,49,48,53,56,51,32,113,32,45,48,46,48,49,48,54,44,45,48,46,48,53,50,57,32,45,48,46,48,49,48,54,44,45,48,46,49,52,56,49,55,32,48,44,45,48,46,48,57,
53,50,32,48,46,48,49,48,54,44,45,48,46,49,52,56,49,54,32,48,46,48,49,48,54,44,45,48,46,48,53,50,57,32,48,44,45,48,46,48,57,53,51,32,48,44,45,48,46,48,52,50,51,32,48,46,48,49,48,54,44,45,48,46,48,56,52,
55,32,48,46,48,50,49,50,44,45,48,46,48,55,52,49,32,48,46,48,49,48,54,44,45,48,46,49,52,56,49,55,32,48,44,45,48,46,48,55,52,49,32,48,46,48,50,49,50,44,45,48,46,49,51,55,53,56,32,48,46,48,52,50,51,44,45,
48,46,49,55,57,57,50,32,48,46,48,55,52,49,44,45,48,46,51,53,57,56,51,32,48,46,48,51,49,56,44,45,48,46,49,55,57,57,50,32,48,46,48,57,53,50,44,45,48,46,51,51,56,54,55,32,48,46,50,50,50,50,53,44,45,48,46,
53,53,48,51,51,32,48,46,53,51,57,55,53,44,45,48,46,56,56,57,32,48,46,51,49,55,53,44,45,48,46,51,51,56,54,55,32,48,46,56,54,55,56,51,44,45,48,46,53,54,48,57,50,32,48,46,49,49,54,52,50,44,45,48,46,48,53,
50,57,32,48,46,50,53,52,44,45,48,46,48,55,52,49,32,48,46,49,51,55,53,57,44,45,48,46,48,50,49,50,32,48,46,50,56,53,55,53,44,45,48,46,48,53,50,57,32,48,46,48,55,52,49,44,45,48,46,48,49,48,54,32,48,46,49,
52,56,49,55,44,45,48,46,48,49,48,54,32,48,46,48,55,52,49,44,45,48,46,48,49,48,54,32,48,46,49,53,56,55,53,44,45,48,46,48,50,49,50,32,122,34,32,47,62,10,32,32,32,32,32,32,32,32,60,112,97,116,104,10,32,32,
32,32,32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,51,56,52,53,34,10,32,32,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,111,110,116,45,115,116,121,108,101,58,110,111,114,109,97,108,59,
102,111,110,116,45,118,97,114,105,97,110,116,58,110,111,114,109,97,108,59,102,111,110,116,45,119,101,105,103,104,116,58,110,111,114,109,97,108,59,102,111,110,116,45,115,116,114,101,116,99,104,58,110,111,
114,109,97,108,59,102,111,110,116,45,102,97,109,105,108,121,58,39,65,108,116,101,32,72,97,97,115,32,71,114,111,116,101,115,107,39,59,45,105,110,107,115,99,97,112,101,45,102,111,110,116,45,115,112,101,
99,105,102,105,99,97,116,105,111,110,58,39,65,108,116,101,32,72,97,97,115,32,71,114,111,116,101,115,107,39,59,102,105,108,108,58,35,102,50,102,50,102,50,59,115,116,114,111,107,101,45,119,105,100,116,104,
58,48,46,50,54,52,53,56,51,51,50,34,10,32,32,32,32,32,32,32,32,32,32,32,100,61,34,109,32,49,50,48,46,48,48,56,49,50,44,49,48,53,46,52,52,55,55,49,32,113,32,48,46,55,54,50,44,45,48,46,48,49,48,54,32,49,
46,50,56,48,53,57,44,48,46,49,55,57,57,50,32,48,46,53,49,56,53,56,44,48,46,49,55,57,57,50,32,48,46,56,48,52,51,51,44,48,46,54,52,53,53,56,32,48,46,48,55,52,49,44,48,46,49,49,54,52,50,32,48,46,49,49,54,
52,50,44,48,46,50,52,51,52,50,32,48,46,48,52,50,51,44,48,46,49,49,54,52,50,32,48,46,48,56,52,55,44,48,46,50,54,52,53,56,32,48,46,48,50,49,50,44,48,46,48,55,52,49,32,48,46,48,49,48,54,44,48,46,49,53,56,
55,53,32,45,48,46,48,49,48,54,44,48,46,48,55,52,49,32,45,48,46,48,51,49,56,44,48,46,49,49,54,52,50,32,45,48,46,48,50,49,50,44,48,46,48,50,49,50,32,45,48,46,48,53,50,57,44,48,46,48,52,50,51,32,45,48,46,
48,51,49,56,44,48,46,48,49,48,54,32,45,48,46,48,53,50,57,44,48,46,48,51,49,56,32,45,48,46,48,53,50,57,44,48,46,48,50,49,50,32,45,48,46,49,48,53,56,51,44,48,46,48,49,48,54,32,45,48,46,48,53,50,57,44,45,
48,46,48,49,48,54,32,45,48,46,49,48,53,56,51,44,48,32,45,48,46,48,56,52,55,44,48,46,48,50,49,50,32,45,48,46,50,50,50,50,53,44,48,46,48,49,48,54,32,45,48,46,49,51,55,53,57,44,45,48,46,48,50,49,50,32,45,
48,46,49,57,48,53,44,45,48,46,48,53,50,57,32,45,48,46,48,56,52,55,44,45,48,46,48,53,50,57,32,45,48,46,49,49,54,52,50,44,45,48,46,49,51,55,53,56,32,45,48,46,48,50,49,50,44,45,48,46,48,57,53,50,32,45,48,
46,48,55,52,49,44,45,48,46,49,55,57,57,50,32,45,48,46,48,57,53,50,44,45,48,46,49,54,57,51,51,32,45,48,46,50,54,52,53,57,44,45,48,46,51,48,54,57,49,32,45,48,46,49,49,54,52,49,44,45,48,46,48,56,52,55,32,
45,48,46,50,53,52,44,45,48,46,49,50,55,32,45,48,46,49,51,55,53,56,44,45,48,46,48,52,50,51,32,45,48,46,50,57,54,51,51,44,45,48,46,48,56,52,55,32,45,48,46,48,53,50,57,44,45,48,46,48,50,49,50,32,45,48,46,
49,48,53,56,51,44,45,48,46,48,49,48,54,32,45,48,46,48,53,50,57,44,48,32,45,48,46,49,48,53,56,52,44,45,48,46,48,49,48,54,32,45,48,46,48,54,51,53,44,45,48,46,48,50,49,50,32,45,48,46,49,53,56,55,53,44,45,
48,46,48,50,49,50,32,45,48,46,48,57,53,51,44,48,32,45,48,46,49,53,56,55,53,44,48,46,48,50,49,50,32,104,32,45,48,46,48,54,51,53,32,113,32,45,48,46,48,53,50,57,44,48,46,48,49,48,54,32,45,48,46,49,48,53,
56,51,44,48,46,48,50,49,50,32,45,48,46,48,53,50,57,44,48,32,45,48,46,49,48,53,56,51,44,48,46,48,49,48,54,32,45,48,46,50,55,53,49,55,44,48,46,48,54,51,53,32,45,48,46,52,53,53,48,57,44,48,46,49,55,57,57,
50,32,45,48,46,49,55,57,57,49,44,48,46,49,49,54,52,50,32,45,48,46,50,52,51,52,49,44,48,46,51,57,49,53,56,32,45,48,46,48,50,49,50,44,48,46,48,55,52,49,32,45,48,46,48,49,48,54,44,48,46,49,52,56,49,55,32,
48,46,48,50,49,50,44,48,46,48,55,52,49,32,48,46,48,52,50,51,44,48,46,49,50,55,32,48,46,48,54,51,53,44,48,46,49,53,56,55,53,32,48,46,49,52,56,49,54,44,48,46,50,51,50,56,51,32,48,46,48,57,53,50,44,48,46,
48,54,51,53,32,48,46,50,51,50,56,52,44,48,46,49,52,56,49,55,32,48,46,49,54,57,51,51,44,48,46,49,48,53,56,51,32,48,46,51,56,49,44,48,46,49,53,56,55,53,32,48,46,50,49,49,54,54,44,48,46,48,52,50,51,32,48,
46,52,50,51,51,51,44,48,46,49,48,53,56,51,32,48,46,49,51,55,53,56,44,48,46,48,52,50,51,32,48,46,50,55,53,49,55,44,48,46,48,54,51,53,32,48,46,49,51,55,53,56,44,48,46,48,50,49,50,32,48,46,50,55,53,49,54,
44,48,46,48,54,51,53,32,48,46,50,50,50,50,53,44,48,46,48,54,51,53,32,48,46,52,51,51,57,50,44,48,46,49,50,55,32,48,46,50,49,49,54,55,44,48,46,48,53,50,57,32,48,46,52,50,51,51,51,44,48,46,49,51,55,53,57,
32,48,46,48,53,50,57,44,48,46,48,50,49,50,32,48,46,49,48,53,56,52,44,48,46,48,52,50,51,32,48,46,48,53,50,57,44,48,46,48,50,49,50,32,48,46,49,48,53,56,51,44,48,46,48,53,50,57,32,48,46,50,49,49,54,55,44,
48,46,49,50,55,32,48,46,51,55,48,52,50,44,48,46,50,56,53,55,53,32,48,46,49,53,56,55,53,44,48,46,49,53,56,55,53,32,48,46,50,54,52,53,56,44,48,46,52,48,50,49,54,32,48,46,48,50,49,50,44,48,46,48,55,52,49,
32,48,46,48,52,50,51,44,48,46,50,48,49,48,57,32,48,46,48,50,49,50,44,48,46,48,55,52,49,32,48,46,48,50,49,50,44,48,46,49,53,56,55,53,32,48,46,48,49,48,54,44,48,46,48,55,52,49,32,48,46,48,49,48,54,44,48,
46,49,54,57,51,51,32,118,32,48,46,49,48,53,56,51,32,113,32,45,48,46,48,50,49,50,44,48,46,48,55,52,49,32,45,48,46,48,50,49,50,44,48,46,49,52,56,49,55,32,48,44,48,46,48,55,52,49,32,45,48,46,48,50,49,50,
44,48,46,49,52,56,49,55,32,45,48,46,48,51,49,56,44,48,46,49,49,54,52,49,32,45,48,46,48,55,52,49,44,48,46,50,50,50,50,53,32,45,48,46,48,52,50,51,44,48,46,49,48,53,56,51,32,45,48,46,48,57,53,50,44,48,46,
49,57,48,53,32,45,48,46,49,54,57,51,52,44,48,46,50,55,53,49,54,32,45,48,46,52,51,51,57,50,44,48,46,52,52,52,53,32,45,48,46,50,54,52,53,56,44,48,46,49,53,56,55,53,32,45,48,46,53,57,50,54,55,44,48,46,50,
55,53,49,54,32,45,48,46,48,57,53,51,44,48,46,48,51,49,55,32,45,48,46,50,48,49,48,56,44,48,46,48,52,50,51,32,45,48,46,48,57,53,51,44,48,46,48,50,49,50,32,45,48,46,49,57,48,53,44,48,46,48,53,50,57,32,45,
48,46,49,48,53,56,51,44,48,46,48,50,49,50,32,45,48,46,50,49,49,54,55,44,48,46,48,50,49,50,32,45,48,46,49,48,53,56,51,44,48,32,45,48,46,50,49,49,54,54,44,48,46,48,50,49,50,32,104,32,45,48,46,49,48,53,56,
52,32,113,32,45,48,46,48,53,50,57,44,48,46,48,49,48,54,32,45,48,46,49,52,56,49,54,44,48,46,48,49,48,54,32,45,48,46,48,57,53,51,44,48,32,45,48,46,49,52,56,49,55,44,45,48,46,48,49,48,54,32,104,32,45,48,
46,49,49,54,52,50,32,113,32,45,48,46,48,54,51,53,44,45,48,46,48,50,49,50,32,45,48,46,49,51,55,53,56,44,45,48,46,48,50,49,50,32,45,48,46,48,55,52,49,44,48,46,48,49,48,54,32,45,48,46,49,50,55,44,45,48,46,
48,49,48,54,32,45,48,46,48,53,50,57,44,45,48,46,48,49,48,54,32,45,48,46,49,51,55,53,56,44,45,48,46,48,49,48,54,32,45,48,46,48,56,52,55,44,45,48,46,48,50,49,50,32,45,48,46,49,54,57,51,52,44,45,48,46,48,
52,50,51,32,45,48,46,48,55,52,49,44,45,48,46,48,49,48,54,32,45,48,46,49,53,56,55,53,44,45,48,46,48,52,50,51,32,45,48,46,51,50,56,48,56,44,45,48,46,49,48,53,56,52,32,45,48,46,53,55,49,53,44,45,48,46,50,
54,52,53,57,32,45,48,46,50,52,51,52,49,44,45,48,46,49,53,56,55,53,32,45,48,46,52,50,51,51,51,44,45,48,46,52,49,50,55,53,32,45,48,46,48,57,53,50,44,45,48,46,49,51,55,53,56,32,45,48,46,49,53,56,55,53,44,
45,48,46,50,56,53,55,53,32,45,48,46,48,54,51,53,44,45,48,46,49,52,56,49,54,32,45,48,46,49,49,54,52,50,44,45,48,46,51,50,56,48,56,32,45,48,46,48,50,49,50,44,45,48,46,48,54,51,53,32,45,48,46,48,50,49,50,
44,45,48,46,49,52,56,49,55,32,48,46,48,49,48,54,44,45,48,46,48,57,53,50,32,48,46,48,51,49,56,44,45,48,46,49,52,56,49,54,32,48,46,48,51,49,56,44,45,48,46,48,52,50,51,32,48,46,49,48,53,56,51,44,45,48,46,
48,56,52,55,32,48,46,48,53,50,57,44,45,48,46,48,50,49,50,32,48,46,49,52,56,49,55,44,45,48,46,48,50,49,50,32,48,46,48,57,53,51,44,45,48,46,48,49,48,54,32,48,46,49,57,48,53,44,48,32,48,46,48,57,53,51,44,
48,46,48,49,48,54,32,48,46,49,55,57,57,49,44,48,46,48,51,49,56,32,48,46,48,57,53,50,44,48,46,48,49,48,54,32,48,46,49,50,55,44,48,46,48,51,49,56,32,48,46,48,55,52,49,44,48,46,48,53,50,57,32,48,46,48,57,
53,51,44,48,46,49,53,56,55,53,32,48,46,48,51,49,56,44,48,46,48,57,53,51,32,48,46,48,55,52,49,44,48,46,49,55,57,57,50,32,48,46,49,48,53,56,51,44,48,46,50,51,50,56,51,32,48,46,50,56,53,55,53,44,48,46,51,
53,57,56,51,32,48,46,49,50,55,44,48,46,48,57,53,51,32,48,46,50,56,53,55,53,44,48,46,49,52,56,49,55,32,48,46,49,53,56,55,53,44,48,46,48,53,50,57,32,48,46,51,51,56,54,54,44,48,46,49,48,53,56,51,32,108,32,
48,46,50,50,50,50,53,44,48,46,48,51,49,56,32,104,32,48,46,48,54,51,53,32,113,32,48,46,48,55,52,49,44,48,46,48,50,49,50,32,48,46,49,57,48,53,44,48,46,48,50,49,50,32,48,46,49,50,55,44,48,32,48,46,50,49,
49,54,55,44,45,48,46,48,50,49,50,32,48,46,48,55,52,49,44,48,32,48,46,49,49,54,52,50,44,45,48,46,48,49,48,54,32,48,46,48,54,51,53,44,45,48,46,48,49,48,54,32,48,46,49,50,55,44,45,48,46,48,49,48,54,32,48,
46,48,54,51,53,44,45,48,46,48,49,48,54,32,48,46,49,49,54,52,49,44,45,48,46,48,50,49,50,32,48,46,51,48,54,57,50,44,45,48,46,48,56,52,55,32,48,46,52,56,54,56,52,44,45,48,46,50,50,50,50,53,32,48,46,49,55,
57,57,49,44,45,48,46,49,51,55,53,56,32,48,46,50,53,52,44,45,48,46,52,54,53,54,55,32,48,46,48,50,49,50,44,45,48,46,48,53,50,57,32,48,46,48,49,48,54,44,45,48,46,49,49,54,52,49,32,45,48,46,48,49,48,54,44,
45,48,46,48,54,51,53,32,45,48,46,48,50,49,50,44,45,48,46,48,57,53,51,32,45,48,46,48,53,50,57,44,45,48,46,50,48,49,48,57,32,45,48,46,49,52,56,49,54,44,45,48,46,50,57,54,51,52,32,45,48,46,48,57,53,50,44,
45,48,46,49,48,53,56,51,32,45,48,46,50,52,51,52,50,44,45,48,46,49,57,48,53,32,45,48,46,48,57,53,50,44,45,48,46,48,54,51,53,32,45,48,46,50,49,49,54,55,44,45,48,46,49,48,53,56,51,32,45,48,46,49,49,54,52,
49,44,45,48,46,48,52,50,51,32,45,48,46,50,51,50,56,51,44,45,48,46,48,56,52,55,32,45,48,46,48,51,49,56,44,45,48,46,48,49,48,54,32,45,48,46,48,54,51,53,44,45,48,46,48,49,48,54,32,45,48,46,48,50,49,50,44,
48,32,45,48,46,48,53,50,57,44,45,48,46,48,50,49,50,32,45,48,46,51,53,57,56,51,44,45,48,46,49,49,54,52,49,32,45,48,46,55,53,49,52,49,44,45,48,46,49,54,57,51,51,32,45,48,46,51,56,49,44,45,48,46,48,54,51,
53,32,45,48,46,55,49,57,54,55,44,45,48,46,50,48,49,48,56,32,45,48,46,49,48,53,56,51,44,45,48,46,48,52,50,51,32,45,48,46,50,48,49,48,56,44,45,48,46,48,55,52,49,32,45,48,46,48,57,53,50,44,45,48,46,48,51,
49,56,32,45,48,46,49,55,57,57,50,44,45,48,46,48,56,52,55,32,45,48,46,50,48,49,48,56,44,45,48,46,49,49,54,52,50,32,45,48,46,51,55,48,52,50,44,45,48,46,50,54,52,53,57,32,45,48,46,49,54,57,51,51,44,45,48,
46,49,52,56,49,54,32,45,48,46,50,54,52,53,56,44,45,48,46,51,55,48,52,49,32,45,48,46,48,51,49,56,44,45,48,46,48,55,52,49,32,45,48,46,48,53,50,57,44,45,48,46,49,52,56,49,55,32,45,48,46,48,49,48,54,44,45,
48,46,48,55,52,49,32,45,48,46,48,51,49,56,44,45,48,46,49,53,56,55,53,32,45,48,46,48,51,49,56,44,45,48,46,49,50,55,32,45,48,46,48,50,49,50,44,45,48,46,50,56,53,55,53,32,48,46,48,49,48,54,44,45,48,46,49,
54,57,51,51,32,48,46,48,51,49,56,44,45,48,46,50,56,53,55,53,32,48,46,48,50,49,50,44,45,48,46,48,53,50,57,32,48,46,48,50,49,50,44,45,48,46,48,56,52,55,32,48,46,48,49,48,54,44,45,48,46,48,52,50,51,32,48,
46,48,51,49,55,44,45,48,46,48,56,52,55,32,48,46,48,57,53,51,44,45,48,46,50,52,51,52,50,32,48,46,50,53,52,44,45,48,46,52,50,51,51,52,32,48,46,49,54,57,51,52,44,45,48,46,49,55,57,57,49,32,48,46,51,56,49,
44,45,48,46,51,49,55,53,32,48,46,49,53,56,55,53,44,45,48,46,48,57,53,50,32,48,46,51,52,57,50,53,44,45,48,46,49,52,56,49,54,32,48,46,49,57,48,53,44,45,48,46,48,54,51,53,32,48,46,52,48,50,49,55,44,45,48,
46,49,49,54,52,50,32,48,46,48,55,52,49,44,45,48,46,48,49,48,54,32,48,46,49,51,55,53,56,44,45,48,46,48,50,49,50,32,48,46,48,55,52,49,44,45,48,46,48,49,48,54,32,48,46,49,53,56,55,53,44,45,48,46,48,50,49,
50,32,48,46,48,51,49,56,44,45,48,46,48,49,48,54,32,48,46,48,56,52,55,44,48,32,48,46,48,53,50,57,44,48,46,48,49,48,54,32,48,46,48,55,52,49,44,45,48,46,48,49,48,54,32,122,34,32,47,62,10,32,32,32,32,32,32,
32,32,60,112,97,116,104,10,32,32,32,32,32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,51,56,52,55,34,10,32,32,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,111,110,116,45,115,116,121,108,
101,58,110,111,114,109,97,108,59,102,111,110,116,45,118,97,114,105,97,110,116,58,110,111,114,109,97,108,59,102,111,110,116,45,119,101,105,103,104,116,58,110,111,114,109,97,108,59,102,111,110,116,45,115,
116,114,101,116,99,104,58,110,111,114,109,97,108,59,102,111,110,116,45,102,97,109,105,108,121,58,39,65,108,116,101,32,72,97,97,115,32,71,114,111,116,101,115,107,39,59,45,105,110,107,115,99,97,112,101,
45,102,111,110,116,45,115,112,101,99,105,102,105,99,97,116,105,111,110,58,39,65,108,116,101,32,72,97,97,115,32,71,114,111,116,101,115,107,39,59,102,105,108,108,58,35,102,50,102,50,102,50,59,115,116,114,
111,107,101,45,119,105,100,116,104,58,48,46,50,54,52,53,56,51,51,50,34,10,32,32,32,32,32,32,32,32,32,32,32,100,61,34,109,32,49,50,54,46,54,55,57,50,54,44,49,48,55,46,54,57,49,51,56,32,104,32,49,46,57,
49,53,53,57,32,113,32,48,46,48,56,52,55,44,48,32,48,46,49,57,48,53,44,48,32,48,46,49,49,54,52,49,44,45,48,46,48,49,48,54,32,48,46,50,50,50,50,53,44,45,48,46,48,49,48,54,32,48,46,49,49,54,52,49,44,48,32,
48,46,50,48,49,48,56,44,48,46,48,50,49,50,32,48,46,48,57,53,51,44,48,46,48,49,48,54,32,48,46,49,52,56,49,55,44,48,46,48,53,50,57,32,48,46,48,53,50,57,44,48,46,48,51,49,56,32,48,46,48,55,52,49,44,48,46,
49,49,54,52,50,32,48,46,48,49,48,54,44,48,46,48,51,49,55,32,48,46,48,49,48,54,44,48,46,48,57,53,50,32,48,46,48,49,48,54,44,48,46,48,53,50,57,32,48,46,48,49,48,54,44,48,46,49,50,55,32,48,44,48,46,48,54,
51,53,32,45,48,46,48,49,48,54,44,48,46,49,50,55,32,48,44,48,46,48,54,51,53,32,45,48,46,48,49,48,54,44,48,46,48,57,53,50,32,45,48,46,48,50,49,50,44,48,46,49,48,53,56,51,32,45,48,46,48,55,52,49,44,48,46,
49,52,56,49,54,32,45,48,46,49,48,53,56,52,44,48,46,48,55,52,49,32,45,48,46,51,52,57,50,53,44,48,46,48,54,51,53,32,45,48,46,50,51,50,56,52,44,45,48,46,48,49,48,54,32,45,48,46,52,49,50,55,53,44,45,48,46,
48,49,48,54,32,104,32,45,49,46,52,48,55,53,57,32,113,32,45,48,46,49,52,56,49,54,44,48,32,45,48,46,51,52,57,50,53,44,48,46,48,49,48,54,32,45,48,46,50,48,49,48,56,44,48,46,48,49,48,54,32,45,48,46,50,57,
54,51,51,44,45,48,46,48,51,49,56,32,45,48,46,48,55,52,49,44,45,48,46,48,51,49,56,32,45,48,46,49,49,54,52,50,44,45,48,46,49,49,54,52,49,32,45,48,46,48,51,49,55,44,45,48,46,48,55,52,49,32,45,48,46,48,51,
49,55,44,45,48,46,50,53,52,32,48,44,45,48,46,49,57,48,53,32,48,46,48,51,49,55,44,45,48,46,50,55,53,49,55,32,48,46,48,49,48,54,44,45,48,46,48,50,49,50,32,48,46,48,49,48,54,44,45,48,46,48,52,50,51,32,48,
46,48,49,48,54,44,45,48,46,48,50,49,50,32,48,46,48,51,49,56,44,45,48,46,48,52,50,51,32,48,46,48,50,49,50,44,45,48,46,48,51,49,56,32,48,46,48,54,51,53,44,45,48,46,48,52,50,51,32,48,46,48,53,50,57,44,45,
48,46,48,49,48,54,32,48,46,48,57,53,51,44,45,48,46,48,51,49,56,32,122,34,32,47,62,10,32,32,32,32,32,32,32,32,60,112,97,116,104,10,32,32,32,32,32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,51,56,52,
57,34,10,32,32,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,111,110,116,45,115,116,121,108,101,58,110,111,114,109,97,108,59,102,111,110,116,45,118,97,114,105,97,110,116,58,110,111,114,109,
97,108,59,102,111,110,116,45,119,101,105,103,104,116,58,110,111,114,109,97,108,59,102,111,110,116,45,115,116,114,101,116,99,104,58,110,111,114,109,97,108,59,102,111,110,116,45,102,97,109,105,108,121,58,
39,65,108,116,101,32,72,97,97,115,32,71,114,111,116,101,115,107,39,59,45,105,110,107,115,99,97,112,101,45,102,111,110,116,45,115,112,101,99,105,102,105,99,97,116,105,111,110,58,39,65,108,116,101,32,72,
97,97,115,32,71,114,111,116,101,115,107,39,59,102,105,108,108,58,35,102,50,102,50,102,50,59,115,116,114,111,107,101,45,119,105,100,116,104,58,48,46,50,54,52,53,56,51,51,50,34,10,32,32,32,32,32,32,32,32,
32,32,32,100,61,34,109,32,49,51,51,46,50,53,49,54,55,44,49,48,51,46,52,55,57,50,49,32,104,32,52,46,52,50,51,56,52,32,48,46,56,48,52,51,51,32,113,32,48,46,49,48,53,56,51,44,48,32,48,46,50,49,49,54,55,44,
48,32,48,46,49,48,53,56,51,44,48,32,48,46,49,55,57,57,49,44,48,46,48,52,50,51,32,48,46,48,56,52,55,44,48,46,48,53,50,57,32,48,46,48,57,53,50,44,48,46,49,52,56,49,54,32,48,46,48,50,49,50,44,48,46,48,56,
52,55,32,48,46,48,50,49,50,44,48,46,50,50,50,50,53,32,118,32,48,46,49,50,55,32,113,32,45,48,46,48,50,49,50,44,48,46,48,56,52,55,32,45,48,46,48,52,50,51,44,48,46,49,51,55,53,57,32,45,48,46,48,49,48,54,
44,48,46,48,53,50,57,32,45,48,46,48,54,51,53,44,48,46,48,57,53,51,32,45,48,46,48,56,52,55,44,48,46,48,53,50,57,32,45,48,46,50,51,50,56,52,44,48,46,48,53,50,57,32,45,48,46,49,52,56,49,54,44,48,32,45,48,
46,50,56,53,55,53,44,48,32,104,32,45,49,46,50,48,54,53,32,113,32,45,48,46,49,49,54,52,49,44,48,32,45,48,46,50,51,50,56,51,44,48,32,45,48,46,49,48,53,56,51,44,45,48,46,48,49,48,54,32,45,48,46,50,48,49,
48,56,44,48,46,48,49,48,54,32,45,48,46,48,56,52,55,44,48,46,48,49,48,54,32,45,48,46,49,52,56,49,55,44,48,46,48,54,51,53,32,45,48,46,48,54,51,53,44,48,46,48,52,50,51,32,45,48,46,48,57,53,50,44,48,46,49,
51,55,53,56,32,45,48,46,48,51,49,55,44,48,46,48,57,53,51,32,45,48,46,48,50,49,50,44,48,46,50,52,51,52,50,32,48,46,48,49,48,54,44,48,46,49,52,56,49,54,32,48,46,48,49,48,54,44,48,46,50,54,52,53,56,32,118,
32,49,46,48,50,54,53,56,32,51,46,51,52,52,51,52,32,49,46,49,50,49,56,51,32,113,32,48,44,48,46,49,53,56,55,53,32,45,48,46,48,50,49,50,44,48,46,50,56,53,55,53,32,45,48,46,48,49,48,54,44,48,46,49,50,55,32,
45,48,46,49,49,54,52,50,44,48,46,49,55,57,57,50,32,45,48,46,48,53,50,57,44,48,46,48,50,49,50,32,45,48,46,49,50,55,44,48,46,48,51,49,56,32,45,48,46,48,55,52,49,44,48,32,45,48,46,49,53,56,55,53,44,48,32,
45,48,46,49,51,55,53,56,44,48,32,45,48,46,50,57,54,51,51,44,48,32,45,48,46,49,52,56,49,55,44,48,32,45,48,46,50,48,49,48,56,44,45,48,46,48,55,52,49,32,45,48,46,48,54,51,53,44,45,48,46,48,56,52,55,32,45,
48,46,48,55,52,49,44,45,48,46,50,48,49,48,56,32,48,44,45,48,46,49,49,54,52,50,32,48,44,45,48,46,50,53,52,32,118,32,45,48,46,57,52,49,57,50,32,45,51,46,52,56,49,57,49,32,45,49,46,48,52,55,55,53,32,113,
32,48,44,45,48,46,49,49,54,52,50,32,48,44,45,48,46,50,54,52,53,57,32,48,46,48,49,48,54,44,45,48,46,49,53,56,55,53,32,45,48,46,48,50,49,50,44,45,48,46,50,53,52,32,45,48,46,48,53,50,57,44,45,48,46,49,54,
57,51,51,32,45,48,46,50,52,51,52,50,44,45,48,46,49,55,57,57,49,32,45,48,46,49,55,57,57,50,44,45,48,46,48,49,48,54,32,45,48,46,51,57,49,53,56,44,45,48,46,48,49,48,54,32,104,32,45,49,46,49,54,52,49,55,32,
113,32,45,48,46,49,50,55,44,48,32,45,48,46,50,57,54,51,51,44,48,46,48,49,48,54,32,45,48,46,49,53,56,55,53,44,48,32,45,48,46,50,52,51,52,50,44,45,48,46,48,51,49,55,32,45,48,46,48,51,49,56,44,45,48,46,48,
50,49,50,32,45,48,46,48,54,51,53,44,45,48,46,48,52,50,51,32,45,48,46,48,51,49,55,44,45,48,46,48,50,49,50,32,45,48,46,48,53,50,57,44,45,48,46,48,53,50,57,32,45,48,46,48,51,49,55,44,45,48,46,48,52,50,51,
32,45,48,46,48,52,50,51,44,45,48,46,49,51,55,53,57,32,45,48,46,48,49,48,54,44,45,48,46,48,57,53,50,32,45,48,46,48,49,48,54,44,45,48,46,49,57,48,53,32,48,46,48,49,48,54,44,45,48,46,48,57,53,50,32,48,46,
48,51,49,55,44,45,48,46,49,55,57,57,49,32,48,46,48,50,49,50,44,45,48,46,48,56,52,55,32,48,46,48,53,50,57,44,45,48,46,49,49,54,52,50,32,48,46,48,50,49,50,44,45,48,46,48,51,49,56,32,48,46,48,53,50,57,44,
45,48,46,48,52,50,51,32,48,46,48,52,50,51,44,45,48,46,48,50,49,50,32,48,46,48,56,52,55,44,45,48,46,48,51,49,56,32,48,46,48,50,49,50,44,45,48,46,48,49,48,54,32,48,46,48,52,50,51,44,48,32,48,46,48,50,49,
50,44,48,32,48,46,48,52,50,51,44,45,48,46,48,49,48,54,32,122,34,32,47,62,10,32,32,32,32,32,32,32,32,60,112,97,116,104,10,32,32,32,32,32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,51,56,53,49,34,10,
32,32,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,111,110,116,45,115,116,121,108,101,58,110,111,114,109,97,108,59,102,111,110,116,45,118,97,114,105,97,110,116,58,110,111,114,109,97,108,59,
102,111,110,116,45,119,101,105,103,104,116,58,110,111,114,109,97,108,59,102,111,110,116,45,115,116,114,101,116,99,104,58,110,111,114,109,97,108,59,102,111,110,116,45,102,97,109,105,108,121,58,39,65,108,
116,101,32,72,97,97,115,32,71,114,111,116,101,115,107,39,59,45,105,110,107,115,99,97,112,101,45,102,111,110,116,45,115,112,101,99,105,102,105,99,97,116,105,111,110,58,39,65,108,116,101,32,72,97,97,115,
32,71,114,111,116,101,115,107,39,59,102,105,108,108,58,35,102,50,102,50,102,50,59,115,116,114,111,107,101,45,119,105,100,116,104,58,48,46,50,54,52,53,56,51,51,50,34,10,32,32,32,32,32,32,32,32,32,32,32,
100,61,34,109,32,49,52,52,46,54,55,51,52,49,44,49,48,56,46,48,54,49,56,32,113,32,48,46,48,50,49,50,44,48,46,48,54,51,53,32,48,46,48,50,49,50,44,48,46,50,48,49,48,56,32,48,44,48,46,49,50,55,32,45,48,46,
48,49,48,54,44,48,46,49,55,57,57,50,32,118,32,48,46,49,48,53,56,51,32,113,32,45,48,46,48,49,48,54,44,48,46,48,56,52,55,32,45,48,46,48,50,49,50,44,48,46,49,53,56,55,53,32,48,44,48,46,48,54,51,53,32,45,
48,46,48,49,48,54,44,48,46,49,52,56,49,55,32,45,48,46,48,50,49,50,44,48,46,48,54,51,53,32,45,48,46,48,51,49,56,44,48,46,49,50,55,32,48,44,48,46,48,54,51,53,32,45,48,46,48,50,49,50,44,48,46,49,50,55,32,
45,48,46,48,52,50,51,44,48,46,49,51,55,53,56,32,45,48,46,48,55,52,49,44,48,46,50,55,53,49,54,32,45,48,46,48,51,49,55,44,48,46,49,51,55,53,57,32,45,48,46,48,56,52,55,44,48,46,50,53,52,32,45,48,46,50,50,
50,50,53,44,48,46,53,48,56,32,45,48,46,53,57,50,54,54,44,48,46,56,53,55,50,53,32,45,48,46,51,55,48,52,50,44,48,46,51,51,56,54,55,32,45,48,46,57,49,48,49,55,44,48,46,53,49,56,53,57,32,45,48,46,49,49,54,
52,50,44,48,46,48,52,50,51,32,45,48,46,50,53,52,44,48,46,48,54,51,53,32,45,48,46,49,50,55,44,48,46,48,50,49,50,32,45,48,46,50,54,52,53,56,44,48,46,48,52,50,51,32,45,48,46,48,54,51,53,44,48,46,48,49,48,
54,32,45,48,46,49,51,55,53,57,44,48,46,48,49,48,54,32,45,48,46,48,54,51,53,44,48,46,48,49,48,54,32,45,48,46,49,50,55,44,48,46,48,49,48,54,32,45,48,46,48,52,50,51,44,48,46,48,49,48,54,32,45,48,46,49,50,
55,44,48,46,48,49,48,54,32,45,48,46,48,55,52,49,44,48,32,45,48,46,49,48,53,56,51,44,45,48,46,48,49,48,54,32,45,48,46,48,52,50,51,44,45,48,46,48,49,48,54,32,45,48,46,48,56,52,55,44,45,48,46,48,49,48,54,
32,45,48,46,48,52,50,51,44,48,46,48,49,48,54,32,45,48,46,48,55,52,49,44,48,32,104,32,45,48,46,48,54,51,53,32,108,32,45,48,46,50,53,52,44,45,48,46,48,51,49,56,32,113,32,45,48,46,49,52,56,49,55,44,45,48,
46,48,52,50,51,32,45,48,46,50,56,53,55,53,44,45,48,46,48,56,52,55,32,45,48,46,49,51,55,53,56,44,45,48,46,48,52,50,51,32,45,48,46,50,54,52,53,56,44,45,48,46,48,57,53,50,32,45,48,46,52,55,54,50,53,44,45,
48,46,50,49,49,54,55,32,45,48,46,55,56,51,49,55,44,45,48,46,53,53,48,51,52,32,45,48,46,51,48,54,57,50,44,45,48,46,51,51,56,54,54,32,45,48,46,53,48,56,44,45,48,46,56,50,53,53,32,45,48,46,48,53,50,57,44,
45,48,46,49,48,53,56,51,32,45,48,46,48,56,52,55,44,45,48,46,50,49,49,54,54,32,45,48,46,48,50,49,50,44,45,48,46,49,48,53,56,52,32,45,48,46,48,52,50,51,44,45,48,46,50,50,50,50,53,32,45,48,46,48,50,49,50,
44,45,48,46,48,52,50,51,32,45,48,46,48,50,49,50,44,45,48,46,48,54,51,53,32,48,46,48,49,48,54,44,45,48,46,48,51,49,55,32,48,44,45,48,46,48,55,52,49,32,45,48,46,48,50,49,50,44,45,48,46,48,56,52,55,32,45,
48,46,48,51,49,55,44,45,48,46,49,54,57,51,51,32,48,44,45,48,46,48,57,53,50,32,45,48,46,48,50,49,50,44,45,48,46,49,55,57,57,50,32,48,44,45,48,46,48,53,50,57,32,48,44,45,48,46,48,56,52,55,32,48,44,45,48,
46,48,52,50,51,32,45,48,46,48,49,48,54,44,45,48,46,48,57,53,50,32,45,48,46,48,49,48,54,44,45,48,46,48,53,50,57,32,45,48,46,48,49,48,54,44,45,48,46,49,55,57,57,50,32,48,44,45,48,46,49,51,55,53,56,32,48,
46,48,50,49,50,44,45,48,46,50,48,49,48,56,32,48,46,48,50,49,50,44,45,48,46,48,55,52,49,32,48,46,48,49,48,54,44,45,48,46,49,52,56,49,55,32,45,48,46,48,49,48,54,44,45,48,46,48,55,52,49,32,48,46,48,49,48,
54,44,45,48,46,49,52,56,49,55,32,48,46,48,49,48,54,44,45,48,46,48,53,50,57,32,48,46,48,49,48,54,44,45,48,46,48,56,52,55,32,48,46,48,49,48,54,44,45,48,46,48,52,50,51,32,48,46,48,50,49,50,44,45,48,46,48,
57,53,51,32,48,46,48,51,49,56,44,45,48,46,49,50,55,32,48,46,48,53,50,57,44,45,48,46,50,53,52,32,48,46,48,51,49,56,44,45,48,46,49,50,55,32,48,46,48,56,52,55,44,45,48,46,50,52,51,52,50,32,48,46,49,53,56,
55,53,44,45,48,46,51,57,49,53,56,32,48,46,51,56,49,44,45,48,46,54,57,56,53,32,48,46,50,51,50,56,51,44,45,48,46,51,49,55,53,32,48,46,53,54,48,57,49,44,45,48,46,53,50,57,49,55,32,48,46,49,50,55,44,45,48,
46,48,56,52,55,32,48,46,50,54,52,53,57,44,45,48,46,49,51,55,53,56,32,48,46,49,51,55,53,56,44,45,48,46,48,54,51,53,32,48,46,50,56,53,55,53,44,45,48,46,49,50,55,32,48,46,49,50,55,44,45,48,46,48,53,50,57,
32,48,46,50,55,53,49,54,44,45,48,46,48,54,51,53,32,48,46,49,52,56,49,55,44,45,48,46,48,50,49,50,32,48,46,50,57,54,51,52,44,45,48,46,48,54,51,53,32,48,46,48,55,52,49,44,48,32,48,46,49,51,55,53,56,44,48,
32,48,46,48,55,52,49,44,45,48,46,48,49,48,54,32,48,46,49,51,55,53,56,44,45,48,46,48,50,49,50,32,48,46,57,52,49,57,50,44,45,48,46,48,49,48,54,32,49,46,53,54,54,51,52,44,48,46,51,57,49,53,57,32,48,46,54,
50,52,52,49,44,48,46,52,48,50,49,54,32,48,46,56,57,57,53,56,44,49,46,48,52,55,55,53,32,48,46,48,53,50,57,44,48,46,49,48,53,56,51,32,48,46,48,56,52,55,44,48,46,50,49,49,54,54,32,48,46,48,51,49,55,44,48,
46,49,48,53,56,52,32,48,46,48,55,52,49,44,48,46,50,50,50,50,53,32,48,46,48,51,49,55,44,48,46,49,49,54,52,50,32,48,46,48,53,50,57,44,48,46,50,53,52,32,48,46,48,50,49,50,44,48,46,49,50,55,32,48,46,48,53,
50,57,44,48,46,50,54,52,53,57,32,48,46,48,49,48,54,44,48,46,48,53,50,57,32,48,44,48,46,49,49,54,52,49,32,48,44,48,46,48,53,50,57,32,48,46,48,49,48,54,44,48,46,49,48,53,56,52,32,122,32,109,32,45,48,46,
57,52,49,57,50,44,48,46,54,48,51,50,53,32,113,32,48,46,48,50,49,50,44,45,48,46,48,55,52,49,32,48,46,48,49,48,54,44,45,48,46,49,54,57,51,52,32,48,44,45,48,46,48,57,53,51,32,48,44,45,48,46,49,55,57,57,49,
32,48,44,45,48,46,52,49,50,55,53,32,45,48,46,48,55,52,49,44,45,48,46,54,57,56,53,32,45,48,46,48,51,49,56,44,45,48,46,48,56,52,55,32,45,48,46,48,53,50,57,44,45,48,46,49,54,57,51,52,32,45,48,46,48,50,49,
50,44,45,48,46,48,56,52,55,32,45,48,46,48,53,50,57,44,45,48,46,49,53,56,55,53,32,45,48,46,50,57,54,51,52,44,45,48,46,54,56,55,57,49,32,45,48,46,57,52,49,57,50,44,45,48,46,57,53,50,53,32,45,48,46,48,55,
52,49,44,45,48,46,48,51,49,56,32,45,48,46,49,53,56,55,53,44,45,48,46,48,52,50,51,32,45,48,46,48,56,52,55,44,45,48,46,48,50,49,50,32,45,48,46,49,54,57,51,51,44,45,48,46,48,52,50,51,32,45,48,46,48,56,52,
55,44,45,48,46,48,50,49,50,32,45,48,46,49,55,57,57,50,44,45,48,46,48,50,49,50,32,45,48,46,48,57,53,51,44,48,32,45,48,46,49,57,48,53,44,48,32,108,32,45,48,46,49,57,48,53,44,48,46,48,51,49,56,32,113,32,
45,48,46,49,48,53,56,51,44,48,46,48,50,49,50,32,45,48,46,50,48,49,48,56,44,48,46,48,53,50,57,32,45,48,46,48,56,52,55,44,48,46,48,50,49,50,32,45,48,46,49,54,57,51,52,44,48,46,48,54,51,53,32,45,48,46,48,
55,52,49,44,48,46,48,50,49,50,32,45,48,46,49,51,55,53,56,44,48,46,48,54,51,53,32,45,48,46,48,54,51,53,44,48,46,48,52,50,51,32,45,48,46,49,49,54,52,50,44,48,46,48,56,52,55,32,45,48,46,50,53,52,44,48,46,
49,55,57,57,50,32,45,48,46,52,49,50,55,53,44,48,46,52,51,51,57,50,32,45,48,46,49,53,56,55,53,44,48,46,50,53,52,32,45,48,46,50,55,53,49,54,44,48,46,53,57,50,54,55,32,45,48,46,48,51,49,56,44,48,46,49,48,
53,56,51,32,45,48,46,48,52,50,51,44,48,46,50,50,50,50,53,32,45,48,46,48,49,48,54,44,48,46,49,48,53,56,51,32,45,48,46,48,51,49,55,44,48,46,50,50,50,50,53,32,45,48,46,48,49,48,54,44,48,46,48,52,50,51,32,
45,48,46,48,49,48,54,44,48,46,49,51,55,53,56,32,45,48,46,48,49,48,54,44,48,46,48,51,49,56,32,45,48,46,48,49,48,54,44,48,46,48,55,52,49,32,48,46,48,49,48,54,44,48,46,48,52,50,51,32,48,46,48,49,48,54,44,
48,46,48,57,53,51,32,48,44,48,46,49,48,53,56,52,32,48,44,48,46,50,49,49,54,55,32,48,44,48,46,49,48,53,56,51,32,48,46,48,50,49,50,44,48,46,49,57,48,53,32,108,32,48,46,48,50,49,50,44,48,46,49,50,55,32,113,
32,48,46,48,50,49,50,44,48,46,49,48,53,56,51,32,48,46,48,51,49,56,44,48,46,50,49,49,54,55,32,48,46,48,50,49,50,44,48,46,48,57,53,51,32,48,46,48,54,51,53,44,48,46,49,55,57,57,49,32,48,46,49,48,53,56,52,
44,48,46,50,53,52,32,48,46,50,52,51,52,50,44,48,46,52,55,54,50,53,32,48,46,49,52,56,49,55,44,48,46,50,50,50,50,53,32,48,46,51,53,57,56,51,44,48,46,51,55,48,52,50,32,48,46,48,53,50,57,44,48,46,48,51,49,
56,32,48,46,48,57,53,50,44,48,46,48,53,50,57,32,48,46,48,52,50,51,44,48,46,48,50,49,50,32,48,46,48,57,53,50,44,48,46,48,53,50,57,32,48,46,48,56,52,55,44,48,46,48,53,50,57,32,48,46,49,55,57,57,50,44,48,
46,48,57,53,50,32,48,46,49,48,53,56,51,44,48,46,48,51,49,56,32,48,46,50,50,50,50,53,44,48,46,48,54,51,53,32,48,46,48,53,50,57,44,48,46,48,50,49,50,32,48,46,48,57,53,50,44,48,46,48,50,49,50,32,48,46,48,
53,50,57,44,45,48,46,48,49,48,54,32,48,46,49,48,53,56,51,44,48,32,48,46,48,51,49,55,44,48,46,48,49,48,54,32,48,46,49,50,55,44,48,46,48,50,49,50,32,48,46,49,48,53,56,52,44,48,46,48,49,48,54,32,48,46,49,
53,56,55,53,44,45,48,46,48,49,48,54,32,48,46,48,52,50,51,44,45,48,46,48,49,48,54,32,48,46,48,56,52,55,44,48,32,48,46,48,52,50,51,44,48,32,48,46,48,56,52,55,44,45,48,46,48,50,49,50,32,48,46,48,56,52,55,
44,45,48,46,48,50,49,50,32,48,46,49,54,57,51,51,44,45,48,46,48,51,49,56,32,48,46,48,56,52,55,44,45,48,46,48,50,49,50,32,48,46,49,53,56,55,53,44,45,48,46,48,53,50,57,32,48,46,51,53,57,56,51,44,45,48,46,
49,52,56,49,55,32,48,46,53,56,50,48,56,44,45,48,46,52,49,50,55,53,32,48,46,50,50,50,50,53,44,45,48,46,50,54,52,53,56,32,48,46,51,55,48,52,50,44,45,48,46,54,50,52,52,50,32,48,46,48,53,50,57,44,45,48,46,
49,50,55,32,48,46,48,54,51,53,44,45,48,46,50,54,52,53,56,32,48,46,48,50,49,50,44,45,48,46,49,52,56,49,55,32,48,46,48,54,51,53,44,45,48,46,50,57,54,51,51,32,122,34,32,47,62,10,32,32,32,32,32,32,32,32,60,
112,97,116,104,10,32,32,32,32,32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,51,56,53,51,34,10,32,32,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,111,110,116,45,115,116,121,108,101,58,110,
111,114,109,97,108,59,102,111,110,116,45,118,97,114,105,97,110,116,58,110,111,114,109,97,108,59,102,111,110,116,45,119,101,105,103,104,116,58,110,111,114,109,97,108,59,102,111,110,116,45,115,116,114,101,
116,99,104,58,110,111,114,109,97,108,59,102,111,110,116,45,102,97,109,105,108,121,58,39,65,108,116,101,32,72,97,97,115,32,71,114,111,116,101,115,107,39,59,45,105,110,107,115,99,97,112,101,45,102,111,110,
116,45,115,112,101,99,105,102,105,99,97,116,105,111,110,58,39,65,108,116,101,32,72,97,97,115,32,71,114,111,116,101,115,107,39,59,102,105,108,108,58,35,102,50,102,50,102,50,59,115,116,114,111,107,101,45,
119,105,100,116,104,58,48,46,50,54,52,53,56,51,51,50,34,10,32,32,32,32,32,32,32,32,32,32,32,100,61,34,109,32,49,53,48,46,57,54,50,50,50,44,49,48,56,46,49,55,56,50,49,32,113,32,48,46,48,49,48,54,44,48,
46,48,53,50,57,32,48,46,48,49,48,54,44,48,46,49,52,56,49,55,32,48,44,48,46,48,56,52,55,32,45,48,46,48,49,48,54,44,48,46,49,51,55,53,56,32,48,44,48,46,48,52,50,51,32,45,48,46,48,49,48,54,44,48,46,48,56,
52,55,32,48,44,48,46,48,51,49,56,32,48,44,48,46,48,56,52,55,32,45,48,46,48,50,49,50,44,48,46,48,56,52,55,32,45,48,46,48,50,49,50,44,48,46,49,54,57,51,51,32,48,44,48,46,48,56,52,55,32,45,48,46,48,50,49,
50,44,48,46,49,54,57,51,51,32,45,48,46,48,49,48,54,44,48,46,48,52,50,51,32,45,48,46,48,49,48,54,44,48,46,48,56,52,55,32,48,44,48,46,48,51,49,56,32,45,48,46,48,49,48,54,44,48,46,48,55,52,49,32,45,48,46,
48,51,49,55,44,48,46,49,49,54,52,50,32,45,48,46,48,54,51,53,44,48,46,50,50,50,50,53,32,45,48,46,48,50,49,50,44,48,46,49,48,53,56,52,32,45,48,46,48,54,51,53,44,48,46,50,49,49,54,55,32,45,48,46,49,52,56,
49,55,44,48,46,51,55,48,52,50,32,45,48,46,51,53,57,56,52,44,48,46,54,54,54,55,53,32,45,48,46,50,49,49,54,54,44,48,46,50,57,54,51,51,32,45,48,46,53,50,57,49,54,44,48,46,53,48,56,32,45,48,46,49,53,56,55,
53,44,48,46,49,48,53,56,51,32,45,48,46,51,48,54,57,50,44,48,46,49,55,57,57,50,32,45,48,46,49,49,54,52,49,44,48,46,48,53,50,57,32,45,48,46,50,51,50,56,51,44,48,46,48,56,52,55,32,45,48,46,49,48,53,56,51,
44,48,46,48,51,49,55,32,45,48,46,50,50,50,50,53,44,48,46,48,54,51,53,32,45,48,46,48,55,52,49,44,48,46,48,50,49,50,32,45,48,46,49,51,55,53,56,44,48,46,48,50,49,50,32,45,48,46,48,54,51,53,44,48,46,48,49,
48,54,32,45,48,46,49,50,55,44,48,46,48,51,49,56,32,104,32,45,48,46,49,48,53,56,52,32,113,32,45,48,46,48,53,50,57,44,48,46,48,50,49,50,32,45,48,46,49,53,56,55,53,44,48,46,48,50,49,50,32,45,48,46,49,48,
53,56,51,44,48,46,48,49,48,54,32,45,48,46,49,54,57,51,51,44,45,48,46,48,49,48,54,32,104,32,45,48,46,49,48,53,56,51,32,113,32,45,48,46,48,52,50,51,44,45,48,46,48,49,48,54,32,45,48,46,48,57,53,51,44,45,
48,46,48,49,48,54,32,45,48,46,48,52,50,51,44,48,32,45,48,46,48,57,53,50,44,45,48,46,48,49,48,54,32,45,48,46,49,53,56,55,53,44,45,48,46,48,52,50,51,32,45,48,46,51,49,55,53,44,45,48,46,48,55,52,49,32,45,
48,46,49,52,56,49,55,44,45,48,46,48,51,49,55,32,45,48,46,50,57,54,51,52,44,45,48,46,48,57,53,50,32,45,48,46,50,49,49,54,54,44,45,48,46,48,56,52,55,32,45,48,46,51,56,49,44,45,48,46,50,49,49,54,54,32,45,
48,46,48,54,51,53,44,45,48,46,48,53,50,57,32,45,48,46,49,50,55,44,45,48,46,49,48,53,56,52,32,45,48,46,48,54,51,53,44,45,48,46,48,54,51,53,32,45,48,46,49,52,56,49,54,44,45,48,46,48,56,52,55,32,45,48,46,
48,54,51,53,44,45,48,46,48,49,48,54,32,45,48,46,49,48,53,56,52,44,48,46,48,51,49,55,32,45,48,46,48,51,49,56,44,48,46,48,51,49,56,32,45,48,46,48,53,50,57,44,48,46,48,57,53,51,32,45,48,46,48,49,48,54,44,
48,46,48,54,51,53,32,45,48,46,48,50,49,50,44,48,46,49,51,55,53,56,32,48,44,48,46,48,54,51,53,32,45,48,46,48,50,49,50,44,48,46,48,57,53,50,32,45,48,46,48,52,50,51,44,48,46,48,53,50,57,32,45,48,46,48,57,
53,50,44,48,46,48,57,53,51,32,45,48,46,48,53,50,57,44,48,46,48,50,49,50,32,45,48,46,49,52,56,49,54,44,48,46,48,51,49,56,32,45,48,46,48,56,52,55,44,48,32,45,48,46,49,57,48,53,44,48,32,45,48,46,48,57,53,
50,44,48,32,45,48,46,49,55,57,57,50,44,45,48,46,48,49,48,54,32,45,48,46,48,56,52,55,44,45,48,46,48,50,49,50,32,45,48,46,49,50,55,44,45,48,46,48,52,50,51,32,45,48,46,48,50,49,50,44,45,48,46,48,50,49,50,
32,45,48,46,48,52,50,51,44,45,48,46,48,53,50,57,32,45,48,46,48,50,49,50,44,45,48,46,48,51,49,56,32,45,48,46,48,52,50,51,44,45,48,46,48,54,51,53,32,45,48,46,48,51,49,56,44,45,48,46,48,56,52,55,32,45,48,
46,48,51,49,56,44,45,48,46,50,53,52,32,48,46,48,49,48,54,44,45,48,46,49,54,57,51,52,32,48,46,48,49,48,54,44,45,48,46,50,56,53,55,53,32,118,32,45,54,46,50,49,50,52,50,32,113,32,48,44,45,48,46,49,50,55,
32,45,48,46,48,49,48,54,44,45,48,46,50,54,52,53,56,32,48,44,45,48,46,49,52,56,49,55,32,48,46,48,53,50,57,44,45,48,46,50,51,50,56,52,32,48,46,48,52,50,51,44,45,48,46,48,54,51,53,32,48,46,49,51,55,53,56,
44,45,48,46,49,48,53,56,51,32,48,46,48,50,49,50,44,48,32,48,46,48,53,50,57,44,48,32,48,46,48,51,49,55,44,45,48,46,48,49,48,54,32,48,46,48,53,50,57,44,45,48,46,48,49,48,54,32,48,46,49,52,56,49,54,44,45,
48,46,48,49,48,54,32,48,46,51,48,54,57,49,44,48,32,48,46,49,54,57,51,52,44,48,32,48,46,50,51,50,56,52,44,48,46,48,56,52,55,32,48,46,48,53,50,57,44,48,46,48,56,52,55,32,48,46,48,53,50,57,44,48,46,50,52,
51,52,50,32,48,44,48,46,49,52,56,49,55,32,48,44,48,46,50,56,53,55,53,32,118,32,49,46,50,51,56,50,53,32,113,32,48,44,48,46,48,55,52,49,32,48,44,48,46,49,55,57,57,50,32,48,44,48,46,48,57,53,51,32,48,44,
48,46,50,48,49,48,56,32,48,46,48,49,48,54,44,48,46,48,57,53,50,32,48,46,48,50,49,50,44,48,46,49,55,57,57,50,32,48,46,48,50,49,50,44,48,46,48,55,52,49,32,48,46,48,54,51,53,44,48,46,49,48,53,56,51,32,48,
46,48,52,50,51,44,48,46,48,51,49,56,32,48,46,48,57,53,50,44,48,46,48,49,48,54,32,48,46,48,53,50,57,44,45,48,46,48,50,49,50,32,48,46,48,56,52,55,44,45,48,46,48,52,50,51,32,48,46,48,53,50,57,44,45,48,46,
48,52,50,51,32,48,46,48,57,53,50,44,45,48,46,48,56,52,55,32,48,46,48,53,50,57,44,45,48,46,48,52,50,51,32,48,46,49,48,53,56,51,44,45,48,46,48,56,52,55,32,48,46,48,55,52,49,44,45,48,46,48,53,50,57,32,48,
46,49,52,56,49,55,44,45,48,46,48,57,53,51,32,48,46,48,56,52,55,44,45,48,46,48,52,50,51,32,48,46,49,54,57,51,51,44,45,48,46,48,56,52,55,32,48,46,49,57,48,53,44,45,48,46,48,57,53,50,32,48,46,52,53,53,48,
56,44,45,48,46,49,51,55,53,56,32,48,46,50,54,52,53,57,44,45,48,46,48,52,50,51,32,48,46,53,51,57,55,53,44,45,48,46,48,52,50,51,32,48,46,50,55,53,49,55,44,48,32,48,46,53,51,57,55,53,44,48,46,48,53,50,57,
32,48,46,50,54,52,53,57,44,48,46,48,52,50,51,32,48,46,52,54,53,54,55,44,48,46,49,50,55,32,48,46,52,50,51,51,51,44,48,46,49,55,57,57,50,32,48,46,55,51,48,50,53,44,48,46,52,57,55,52,50,32,48,46,51,48,54,
57,49,44,48,46,51,49,55,53,32,48,46,52,56,54,56,51,44,48,46,55,52,48,56,51,32,48,46,48,53,50,57,44,48,46,49,49,54,52,50,32,48,46,48,56,52,55,44,48,46,50,51,50,56,51,32,48,46,48,52,50,51,44,48,46,49,48,
53,56,52,32,48,46,48,56,52,55,44,48,46,50,50,50,50,53,32,48,46,48,51,49,56,44,48,46,49,49,54,52,50,32,48,46,48,52,50,51,44,48,46,50,52,51,52,50,32,48,46,48,50,49,50,44,48,46,49,49,54,52,50,32,48,46,48,
52,50,51,44,48,46,50,51,50,56,51,32,48,46,48,49,48,54,44,48,46,48,51,49,55,32,48,46,48,49,48,54,44,48,46,48,55,52,49,32,48,44,48,46,48,51,49,55,32,48,46,48,49,48,54,44,48,46,48,54,51,53,32,48,46,48,50,
49,50,44,48,46,48,54,51,53,32,48,44,48,46,49,50,55,32,45,48,46,48,49,48,54,44,48,46,48,53,50,57,32,48,46,48,49,48,54,44,48,46,49,49,54,52,49,32,122,32,109,32,45,48,46,57,52,49,57,49,44,48,46,50,55,53,
49,55,32,113,32,48,46,48,50,49,50,44,45,48,46,48,53,50,57,32,48,46,48,49,48,54,44,45,48,46,49,50,55,32,45,48,46,48,49,48,54,44,45,48,46,48,55,52,49,32,45,48,46,48,49,48,54,44,45,48,46,49,51,55,53,56,32,
48,44,45,48,46,49,55,57,57,50,32,45,48,46,48,50,49,50,44,45,48,46,51,51,56,54,55,32,45,48,46,48,49,48,54,44,45,48,46,49,53,56,55,53,32,45,48,46,48,54,51,53,44,45,48,46,50,56,53,55,53,32,45,48,46,48,51,
49,56,44,45,48,46,49,48,53,56,51,32,45,48,46,48,53,50,57,44,45,48,46,49,57,48,53,32,45,48,46,48,50,49,50,44,45,48,46,48,56,52,55,32,45,48,46,48,54,51,53,44,45,48,46,49,54,57,51,51,32,45,48,46,49,49,54,
52,49,44,45,48,46,50,54,52,53,57,32,45,48,46,51,49,55,53,44,45,48,46,52,56,54,56,52,32,45,48,46,50,48,49,48,56,44,45,48,46,50,51,50,56,51,32,45,48,46,52,54,53,54,54,44,45,48,46,51,52,57,50,53,32,45,48,
46,48,56,52,55,44,45,48,46,48,51,49,56,32,45,48,46,49,54,57,51,52,44,45,48,46,48,53,50,57,32,45,48,46,48,55,52,49,44,45,48,46,48,51,49,56,32,45,48,46,49,53,56,55,53,44,45,48,46,48,53,50,57,32,45,48,46,
49,49,54,52,49,44,45,48,46,48,51,49,56,32,45,48,46,50,51,50,56,51,44,45,48,46,48,51,49,56,32,45,48,46,49,49,54,52,50,44,45,48,46,48,49,48,54,32,45,48,46,50,53,52,44,45,48,46,48,49,48,54,32,45,48,46,48,
53,50,57,44,48,46,48,49,48,54,32,45,48,46,48,57,53,50,44,48,46,48,50,49,50,32,45,48,46,48,52,50,51,44,48,32,45,48,46,48,57,53,50,44,48,32,45,48,46,48,57,53,50,44,48,46,48,50,49,50,32,45,48,46,49,57,48,
53,44,48,46,48,52,50,51,32,45,48,46,48,56,52,55,44,48,46,48,50,49,50,32,45,48,46,49,54,57,51,51,44,48,46,48,54,51,53,32,45,48,46,51,53,57,56,52,44,48,46,49,52,56,49,54,32,45,48,46,53,55,49,53,44,48,46,
52,52,52,53,32,45,48,46,50,49,49,54,55,44,48,46,50,56,53,55,53,32,45,48,46,51,51,56,54,55,44,48,46,54,55,55,51,51,32,45,48,46,48,52,50,51,44,48,46,49,49,54,52,50,32,45,48,46,48,53,50,57,44,48,46,50,51,
50,56,51,32,45,48,46,48,49,48,54,44,48,46,49,49,54,52,50,32,45,48,46,48,52,50,51,44,48,46,50,52,51,52,50,32,45,48,46,48,49,48,54,44,48,46,48,53,50,57,32,48,44,48,46,48,57,53,50,32,48,46,48,49,48,54,44,
48,46,48,52,50,51,32,45,48,46,48,49,48,54,44,48,46,48,57,53,51,32,45,48,46,48,49,48,54,44,48,46,48,53,50,57,32,45,48,46,48,49,48,54,44,48,46,49,53,56,55,53,32,48,44,48,46,48,57,53,51,32,48,46,48,49,48,
54,44,48,46,49,52,56,49,55,32,118,32,48,46,48,56,52,55,32,113,32,48,46,48,49,48,54,44,48,46,48,52,50,51,32,48,46,48,49,48,54,44,48,46,48,56,52,55,32,48,44,48,46,48,52,50,51,32,48,46,48,49,48,54,44,48,
46,48,55,52,49,32,48,46,48,49,48,54,44,48,46,48,51,49,56,32,48,46,48,49,48,54,44,48,46,48,55,52,49,32,48,44,48,46,48,51,49,56,32,48,46,48,49,48,54,44,48,46,48,54,51,53,32,48,46,48,50,49,50,44,48,46,48,
56,52,55,32,48,46,48,51,49,56,44,48,46,49,54,57,51,51,32,48,46,48,50,49,50,44,48,46,48,56,52,55,32,48,46,48,52,50,51,44,48,46,49,54,57,51,51,32,48,46,49,49,54,52,50,44,48,46,51,53,57,56,52,32,48,46,51,
49,55,53,44,48,46,54,49,51,56,52,32,48,46,50,49,49,54,55,44,48,46,50,53,52,32,48,46,53,49,56,53,57,44,48,46,52,50,51,51,51,32,48,46,48,57,53,50,44,48,46,48,53,50,57,32,48,46,50,48,49,48,56,44,48,46,48,
56,52,55,32,48,46,49,48,53,56,51,44,48,46,48,50,49,50,32,48,46,50,50,50,50,53,44,48,46,48,53,50,57,32,48,46,48,53,50,57,44,48,46,48,50,49,50,32,48,46,48,57,53,50,44,48,46,48,50,49,50,32,48,46,48,53,50,
57,44,45,48,46,48,49,48,54,32,48,46,49,48,53,56,51,44,48,32,48,46,48,52,50,51,44,48,46,48,49,48,54,32,48,46,48,57,53,50,44,48,46,48,49,48,54,32,48,46,48,53,50,57,44,48,32,48,46,48,57,53,51,44,48,32,104,
32,48,46,48,57,53,50,32,113,32,48,46,48,52,50,51,44,45,48,46,48,49,48,54,32,48,46,48,55,52,49,44,45,48,46,48,49,48,54,32,48,46,48,51,49,56,44,48,32,48,46,48,55,52,49,44,45,48,46,48,49,48,54,32,48,46,48,
56,52,55,44,45,48,46,48,50,49,50,32,48,46,49,53,56,55,53,44,45,48,46,48,51,49,56,32,48,46,48,55,52,49,44,45,48,46,48,50,49,50,32,48,46,49,53,56,55,53,44,45,48,46,48,53,50,57,32,48,46,51,50,56,48,56,44,
45,48,46,49,51,55,53,56,32,48,46,53,50,57,49,55,44,45,48,46,51,53,57,56,51,32,48,46,50,48,49,48,56,44,45,48,46,50,51,50,56,52,32,48,46,51,51,56,54,54,44,45,48,46,53,54,48,57,50,32,48,46,48,53,50,57,44,
45,48,46,49,52,56,49,55,32,48,46,48,55,52,49,44,45,48,46,50,57,54,51,51,32,48,46,48,51,49,55,44,45,48,46,49,53,56,55,53,32,48,46,48,55,52,49,44,45,48,46,51,49,55,53,32,48,46,48,50,49,50,44,45,48,46,48,
53,50,57,32,48,46,48,49,48,54,44,45,48,46,48,57,53,50,32,48,44,45,48,46,48,53,50,57,32,48,46,48,49,48,54,44,45,48,46,49,48,53,56,52,32,122,34,32,47,62,10,32,32,32,32,32,32,32,32,60,112,97,116,104,10,32,
32,32,32,32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,51,56,53,53,34,10,32,32,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,111,110,116,45,115,116,121,108,101,58,110,111,114,109,97,108,
59,102,111,110,116,45,118,97,114,105,97,110,116,58,110,111,114,109,97,108,59,102,111,110,116,45,119,101,105,103,104,116,58,110,111,114,109,97,108,59,102,111,110,116,45,115,116,114,101,116,99,104,58,110,
111,114,109,97,108,59,102,111,110,116,45,102,97,109,105,108,121,58,39,65,108,116,101,32,72,97,97,115,32,71,114,111,116,101,115,107,39,59,45,105,110,107,115,99,97,112,101,45,102,111,110,116,45,115,112,
101,99,105,102,105,99,97,116,105,111,110,58,39,65,108,116,101,32,72,97,97,115,32,71,114,111,116,101,115,107,39,59,102,105,108,108,58,35,102,50,102,50,102,50,59,115,116,114,111,107,101,45,119,105,100,116,
104,58,48,46,50,54,52,53,56,51,51,50,34,10,32,32,32,32,32,32,32,32,32,32,32,100,61,34,109,32,49,53,50,46,50,48,56,52,49,44,49,48,52,46,53,49,54,51,56,32,113,32,45,48,46,48,56,52,55,44,45,48,46,48,54,51,
53,32,45,48,46,48,57,53,50,44,45,48,46,50,50,50,50,53,32,48,44,45,48,46,49,54,57,51,51,32,48,44,45,48,46,51,50,56,48,56,32,48,44,45,48,46,48,56,52,55,32,48,44,45,48,46,49,55,57,57,50,32,48,44,45,48,46,
48,57,53,50,32,48,46,48,51,49,56,44,45,48,46,49,53,56,55,53,32,48,46,48,52,50,51,44,45,48,46,48,56,52,55,32,48,46,49,54,57,51,52,44,45,48,46,49,50,55,32,48,46,48,50,49,50,44,45,48,46,48,49,48,54,32,48,
46,48,52,50,51,44,48,32,48,46,48,51,49,55,44,48,32,48,46,48,53,50,57,44,45,48,46,48,49,48,54,32,48,46,49,55,57,57,49,44,48,32,48,46,51,51,56,54,54,44,48,46,48,49,48,54,32,48,46,49,53,56,55,53,44,48,46,
48,49,48,54,32,48,46,50,49,49,54,55,44,48,46,49,50,55,32,48,46,48,51,49,55,44,48,46,48,54,51,53,32,48,46,48,51,49,55,44,48,46,49,53,56,55,53,32,48,44,48,46,48,56,52,55,32,48,44,48,46,49,55,57,57,50,32,
48,44,48,46,49,57,48,53,32,45,48,46,48,49,48,54,44,48,46,51,53,57,56,51,32,48,44,48,46,49,54,57,51,51,32,45,48,46,49,51,55,53,57,44,48,46,50,50,50,50,53,32,45,48,46,48,53,50,57,44,48,46,48,50,49,50,32,
45,48,46,49,49,54,52,49,44,48,46,48,50,49,50,32,45,48,46,48,53,50,57,44,48,32,45,48,46,49,49,54,52,50,44,48,32,45,48,46,49,49,54,52,50,44,48,32,45,48,46,50,50,50,50,53,44,48,32,45,48,46,49,48,53,56,51,
44,48,32,45,48,46,49,55,57,57,50,44,45,48,46,48,53,50,57,32,122,32,109,32,48,46,55,56,51,49,55,44,54,46,48,48,48,55,53,32,113,32,48,44,48,46,49,48,53,56,51,32,48,44,48,46,50,50,50,50,53,32,48,46,48,49,
48,54,44,48,46,49,48,53,56,51,32,45,48,46,48,52,50,51,44,48,46,49,54,57,51,51,32,45,48,46,48,51,49,56,44,48,46,48,54,51,53,32,45,48,46,48,57,53,50,44,48,46,48,56,52,55,32,45,48,46,48,54,51,53,44,48,46,
48,49,48,54,32,45,48,46,49,52,56,49,55,44,48,46,48,51,49,56,32,104,32,45,48,46,49,53,56,55,53,32,113,32,45,48,46,49,51,55,53,56,44,48,32,45,48,46,50,52,51,52,50,44,45,48,46,48,49,48,54,32,45,48,46,48,
57,53,51,44,45,48,46,48,50,49,50,32,45,48,46,49,52,56,49,54,44,45,48,46,49,48,53,56,52,32,45,48,46,48,52,50,51,44,45,48,46,48,54,51,53,32,45,48,46,48,52,50,51,44,45,48,46,49,53,56,55,53,32,48,44,45,48,
46,49,48,53,56,51,32,48,44,45,48,46,50,48,49,48,56,32,118,32,45,52,46,52,55,54,55,53,32,113,32,48,44,45,48,46,49,49,54,52,50,32,48,44,45,48,46,50,50,50,50,53,32,48,44,45,48,46,49,49,54,52,50,32,48,46,
48,53,50,57,44,45,48,46,49,55,57,57,50,32,48,46,48,51,49,56,44,45,48,46,48,53,50,57,32,48,46,48,56,52,55,44,45,48,46,48,54,51,53,32,48,46,48,53,50,57,44,45,48,46,48,49,48,54,32,48,46,49,49,54,52,49,44,
45,48,46,48,51,49,56,32,48,46,48,50,49,50,44,48,32,48,46,48,52,50,51,44,48,46,48,49,48,54,32,48,46,48,51,49,55,44,48,32,48,46,48,53,50,57,44,45,48,46,48,49,48,54,32,48,46,49,51,55,53,57,44,48,32,48,46,
50,55,53,49,55,44,48,46,48,49,48,54,32,48,46,49,52,56,49,55,44,48,32,48,46,50,48,49,48,56,44,48,46,48,56,52,55,32,48,46,48,53,50,57,44,48,46,48,54,51,53,32,48,46,48,53,50,57,44,48,46,49,54,57,51,52,32,
48,44,48,46,48,57,53,51,32,48,44,48,46,50,48,49,48,56,32,122,34,32,47,62,10,32,32,32,32,32,32,32,32,60,112,97,116,104,10,32,32,32,32,32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,51,56,53,55,34,10,
32,32,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,111,110,116,45,115,116,121,108,101,58,110,111,114,109,97,108,59,102,111,110,116,45,118,97,114,105,97,110,116,58,110,111,114,109,97,108,59,
102,111,110,116,45,119,101,105,103,104,116,58,110,111,114,109,97,108,59,102,111,110,116,45,115,116,114,101,116,99,104,58,110,111,114,109,97,108,59,102,111,110,116,45,102,97,109,105,108,121,58,39,65,108,
116,101,32,72,97,97,115,32,71,114,111,116,101,115,107,39,59,45,105,110,107,115,99,97,112,101,45,102,111,110,116,45,115,112,101,99,105,102,105,99,97,116,105,111,110,58,39,65,108,116,101,32,72,97,97,115,
32,71,114,111,116,101,115,107,39,59,102,105,108,108,58,35,102,50,102,50,102,50,59,115,116,114,111,107,101,45,119,105,100,116,104,58,48,46,50,54,52,53,56,51,51,50,34,10,32,32,32,32,32,32,32,32,32,32,32,
100,61,34,109,32,49,53,57,46,50,50,49,53,51,44,49,49,48,46,54,51,51,53,53,32,113,32,48,46,48,51,49,55,44,48,46,49,54,57,51,51,32,45,48,46,48,51,49,55,44,48,46,50,56,53,55,53,32,45,48,46,48,54,51,53,44,
48,46,49,48,53,56,51,32,45,48,46,49,54,57,51,52,44,48,46,49,53,56,55,53,32,45,48,46,48,53,50,57,44,48,46,48,50,49,50,32,45,48,46,49,49,54,52,49,44,48,46,48,50,49,50,32,45,48,46,48,53,50,57,44,48,32,45,
48,46,49,49,54,52,50,44,48,46,48,50,49,50,32,45,48,46,50,51,50,56,51,44,48,46,48,53,50,57,32,45,48,46,52,54,53,54,55,44,45,48,46,48,51,49,56,32,45,48,46,50,50,50,50,53,44,45,48,46,48,55,52,49,32,45,48,
46,51,48,54,57,49,44,45,48,46,49,57,48,53,32,45,48,46,48,53,50,57,44,45,48,46,48,55,52,49,32,45,48,46,48,57,53,50,44,45,48,46,49,54,57,51,51,32,45,48,46,48,52,50,51,44,45,48,46,49,48,53,56,52,32,45,48,
46,49,48,53,56,52,44,45,48,46,49,54,57,51,52,32,45,48,46,48,50,49,50,44,45,48,46,48,50,49,50,32,45,48,46,48,53,50,57,44,45,48,46,48,52,50,51,32,45,48,46,48,50,49,50,44,45,48,46,48,50,49,50,32,45,48,46,
48,54,51,53,44,45,48,46,48,49,48,54,32,45,48,46,49,49,54,52,50,44,48,46,48,51,49,56,32,45,48,46,50,50,50,50,53,44,48,46,49,49,54,52,49,32,45,48,46,48,57,53,50,44,48,46,48,56,52,55,32,45,48,46,49,57,48,
53,44,48,46,49,52,56,49,55,32,45,48,46,49,51,55,53,57,44,48,46,48,56,52,55,32,45,48,46,50,57,54,51,52,44,48,46,49,53,56,55,53,32,45,48,46,49,53,56,55,53,44,48,46,48,54,51,53,32,45,48,46,51,51,56,54,54,
44,48,46,49,49,54,52,50,32,45,48,46,49,48,53,56,52,44,48,46,48,51,49,56,32,45,48,46,50,51,50,56,52,44,48,46,48,52,50,51,32,45,48,46,49,49,54,52,49,44,48,46,48,49,48,54,32,45,48,46,50,51,50,56,51,44,48,
46,48,51,49,56,32,45,48,46,48,53,50,57,44,48,46,48,49,48,54,32,45,48,46,48,57,53,50,44,48,46,48,49,48,54,32,45,48,46,48,51,49,56,44,48,32,45,48,46,48,55,52,49,44,48,32,45,48,46,49,49,54,52,50,44,48,46,
48,50,49,50,32,45,48,46,50,50,50,50,53,44,48,32,104,32,45,48,46,48,57,53,50,32,113,32,45,48,46,48,53,50,57,44,45,48,46,48,49,48,54,32,45,48,46,49,48,53,56,52,44,45,48,46,48,49,48,54,32,45,48,46,48,53,
50,57,44,48,32,45,48,46,48,57,53,50,44,45,48,46,48,50,49,50,32,45,48,46,48,52,50,51,44,45,48,46,48,49,48,54,32,45,48,46,48,55,52,49,44,45,48,46,48,49,48,54,32,45,48,46,48,50,49,50,44,48,46,48,49,48,54,
32,45,48,46,48,53,50,57,44,48,32,45,48,46,48,56,52,55,44,45,48,46,48,50,49,50,32,45,48,46,49,54,57,51,51,44,45,48,46,48,52,50,51,32,45,48,46,48,56,52,55,44,45,48,46,48,49,48,54,32,45,48,46,49,53,56,55,
53,44,45,48,46,48,52,50,51,32,45,48,46,51,49,55,53,44,45,48,46,49,51,55,53,56,32,45,48,46,53,49,56,53,56,44,45,48,46,51,48,54,57,49,32,45,48,46,49,57,48,53,44,45,48,46,49,55,57,57,50,32,45,48,46,51,49,
55,53,44,45,48,46,52,57,55,52,50,32,45,48,46,48,57,53,50,44,45,48,46,50,51,50,56,51,32,45,48,46,48,57,53,50,44,45,48,46,53,54,48,57,50,32,118,32,45,48,46,50,50,50,50,53,32,113,32,48,46,48,49,48,54,44,
45,48,46,48,53,50,57,32,48,46,48,49,48,54,44,45,48,46,49,48,53,56,51,32,48,46,48,49,48,54,44,45,48,46,48,54,51,53,32,48,46,48,50,49,50,44,45,48,46,49,50,55,32,48,46,48,52,50,51,44,45,48,46,49,49,54,52,
50,32,48,46,48,56,52,55,44,45,48,46,50,51,50,56,51,32,48,46,48,52,50,51,44,45,48,46,49,49,54,52,50,32,48,46,49,48,53,56,52,44,45,48,46,50,49,49,54,55,32,48,46,49,57,48,53,44,45,48,46,51,48,54,57,50,32,
48,46,53,49,56,53,56,44,45,48,46,52,53,53,48,56,32,48,46,51,50,56,48,56,44,45,48,46,49,52,56,49,55,32,48,46,55,49,57,54,55,44,45,48,46,50,54,52,53,57,32,48,46,49,52,56,49,54,44,45,48,46,48,52,50,51,32,
48,46,51,48,54,57,49,44,45,48,46,48,54,51,53,32,48,46,49,53,56,55,53,44,45,48,46,48,50,49,50,32,48,46,51,49,55,53,44,45,48,46,48,53,50,57,32,48,46,48,55,52,49,44,45,48,46,48,50,49,50,32,48,46,49,51,55,
53,57,44,45,48,46,48,50,49,50,32,48,46,48,55,52,49,44,48,32,48,46,49,53,56,55,53,44,45,48,46,48,50,49,50,32,48,46,48,55,52,49,44,45,48,46,48,50,49,50,32,48,46,49,52,56,49,54,44,45,48,46,48,50,49,50,32,
48,46,48,55,52,49,44,45,48,46,48,49,48,54,32,48,46,49,53,56,55,53,44,45,48,46,48,51,49,56,32,48,46,50,54,52,53,57,44,45,48,46,48,54,51,53,32,48,46,52,57,55,52,50,44,45,48,46,49,48,53,56,52,32,48,46,50,
51,50,56,51,44,45,48,46,48,53,50,57,32,48,46,51,55,48,52,50,44,45,48,46,50,52,51,52,49,32,48,46,48,54,51,53,44,45,48,46,48,56,52,55,32,48,46,49,48,53,56,51,44,45,48,46,50,49,49,54,55,32,48,46,48,52,50,
51,44,45,48,46,49,51,55,53,56,32,48,44,45,48,46,50,57,54,51,51,32,45,48,46,48,54,51,53,44,45,48,46,50,53,52,32,45,48,46,49,57,48,53,44,45,48,46,52,48,50,49,55,32,45,48,46,49,49,54,52,50,44,45,48,46,49,
52,56,49,55,32,45,48,46,51,51,56,54,55,44,45,48,46,50,51,50,56,51,32,45,48,46,49,50,55,44,45,48,46,48,53,50,57,32,45,48,46,51,48,54,57,49,44,45,48,46,48,55,52,49,32,45,48,46,49,54,57,51,52,44,45,48,46,
48,50,49,50,32,45,48,46,51,53,57,56,52,44,45,48,46,48,50,49,50,32,45,48,46,49,57,48,53,44,48,32,45,48,46,51,55,48,52,49,44,48,46,48,50,49,50,32,45,48,46,49,55,57,57,50,44,48,46,48,50,49,50,32,45,48,46,
51,48,54,57,50,44,48,46,48,54,51,53,32,45,48,46,49,55,57,57,50,44,48,46,48,54,51,53,32,45,48,46,51,52,57,50,53,44,48,46,49,55,57,57,50,32,45,48,46,48,56,52,55,44,48,46,48,54,51,53,32,45,48,46,49,53,56,
55,53,44,48,46,49,54,57,51,51,32,45,48,46,48,54,51,53,44,48,46,48,57,53,50,32,45,48,46,49,49,54,52,50,44,48,46,50,49,49,54,55,32,45,48,46,48,51,49,56,44,48,46,48,56,52,55,32,45,48,46,48,54,51,53,44,48,
46,49,55,57,57,50,32,45,48,46,48,50,49,50,44,48,46,48,57,53,51,32,45,48,46,48,57,53,50,44,48,46,49,52,56,49,54,32,45,48,46,48,52,50,51,44,48,46,48,51,49,56,32,45,48,46,49,52,56,49,54,44,48,46,48,53,50,
57,32,45,48,46,48,57,53,50,44,48,46,48,49,48,54,32,45,48,46,50,48,49,48,57,44,48,46,48,49,48,54,32,45,48,46,49,48,53,56,51,44,48,32,45,48,46,50,49,49,54,54,44,45,48,46,48,49,48,54,32,45,48,46,48,57,53,
50,44,45,48,46,48,50,49,50,32,45,48,46,49,51,55,53,57,44,45,48,46,48,53,50,57,32,45,48,46,48,52,50,51,44,45,48,46,48,51,49,56,32,45,48,46,48,54,51,53,44,45,48,46,49,48,53,56,51,32,45,48,46,48,50,49,50,
44,45,48,46,48,55,52,49,32,48,44,45,48,46,49,53,56,55,53,32,48,46,48,51,49,55,44,45,48,46,49,51,55,53,56,32,48,46,48,54,51,53,44,45,48,46,50,53,52,32,48,46,48,52,50,51,44,45,48,46,49,50,55,32,48,46,48,
57,53,51,44,45,48,46,50,52,51,52,50,32,48,46,50,48,49,48,57,44,45,48,46,51,55,48,52,49,32,48,46,53,48,56,44,45,48,46,53,55,49,53,32,48,46,51,48,54,57,50,44,45,48,46,50,49,49,54,54,32,48,46,55,54,50,44,
45,48,46,51,51,56,54,54,32,48,46,49,48,53,56,52,44,45,48,46,48,50,49,50,32,48,46,50,49,49,54,55,44,45,48,46,48,51,49,56,32,48,46,49,49,54,52,50,44,45,48,46,48,49,48,54,32,48,46,50,51,50,56,51,44,45,48,
46,48,51,49,56,32,48,46,48,52,50,51,44,45,48,46,48,49,48,54,32,48,46,49,51,55,53,57,44,48,32,48,46,48,57,53,50,44,48,32,48,46,49,49,54,52,49,44,45,48,46,48,50,49,50,32,48,46,55,52,48,56,52,44,45,48,46,
48,49,48,54,32,49,46,50,57,49,49,55,44,48,46,49,54,57,51,51,32,48,46,53,53,48,51,51,44,48,46,49,55,57,57,50,32,48,46,55,55,50,53,56,44,48,46,54,56,55,57,50,32,48,46,48,57,53,51,44,48,46,50,49,49,54,55,
32,48,46,49,48,53,56,52,44,48,46,52,57,55,52,50,32,48,46,48,50,49,50,44,48,46,50,55,53,49,54,32,48,46,48,50,49,50,44,48,46,53,56,50,48,56,32,118,32,50,46,51,51,56,57,50,32,113,32,48,44,48,46,49,52,56,
49,54,32,48,46,48,49,48,54,44,48,46,50,56,53,55,53,32,48,46,48,49,48,54,44,48,46,49,51,55,53,56,32,48,46,48,54,51,53,44,48,46,50,50,50,50,53,32,48,46,48,53,50,57,44,48,46,48,55,52,49,32,48,46,49,51,55,
53,56,44,48,46,49,48,53,56,51,32,48,46,48,57,53,50,44,48,46,48,52,50,51,32,48,46,49,55,57,57,50,44,48,46,48,54,51,53,32,48,46,48,56,52,55,44,48,46,48,49,48,54,32,48,46,49,50,55,44,48,46,48,57,53,51,32,
48,46,48,50,49,50,44,48,46,48,51,49,55,32,48,46,48,50,49,50,44,48,46,48,54,51,53,32,48,44,48,46,48,51,49,56,32,48,46,48,49,48,54,44,48,46,48,56,52,55,32,122,32,109,32,45,49,46,53,49,51,52,50,44,45,49,
46,49,55,52,55,53,32,113,32,48,46,48,54,51,53,44,45,48,46,49,52,56,49,55,32,48,46,48,54,51,53,44,45,48,46,51,51,56,54,55,32,48,46,48,49,48,54,44,45,48,46,50,48,49,48,56,32,48,46,48,49,48,54,44,45,48,46,
52,49,50,55,53,32,48,44,45,48,46,49,49,54,52,50,32,45,48,46,48,49,48,54,44,45,48,46,50,49,49,54,55,32,45,48,46,48,49,48,54,44,45,48,46,48,57,53,51,32,45,48,46,48,56,52,55,44,45,48,46,49,52,56,49,54,32,
45,48,46,48,51,49,55,44,45,48,46,48,50,49,50,32,45,48,46,49,52,56,49,54,44,45,48,46,48,50,49,50,32,45,48,46,48,50,49,50,44,48,46,48,50,49,50,32,45,48,46,48,55,52,49,44,48,46,48,50,49,50,32,45,48,46,48,
53,50,57,44,48,32,45,48,46,48,57,53,50,44,48,46,48,50,49,50,32,45,48,46,48,56,52,55,44,48,46,48,50,49,50,32,45,48,46,49,57,48,53,44,48,46,48,52,50,51,32,45,48,46,49,48,53,56,51,44,48,46,48,50,49,50,32,
45,48,46,50,49,49,54,54,44,48,46,48,52,50,51,32,45,48,46,48,51,49,55,44,48,32,45,48,46,48,55,52,49,44,48,46,48,49,48,54,32,45,48,46,48,51,49,56,44,48,32,45,48,46,48,55,52,49,44,48,32,45,48,46,49,53,56,
55,53,44,48,46,48,52,50,51,32,45,48,46,51,52,57,50,53,44,48,46,48,53,50,57,32,45,48,46,49,55,57,57,50,44,48,32,45,48,46,51,51,56,54,55,44,48,46,48,52,50,51,32,45,48,46,48,56,52,55,44,48,46,48,50,49,50,
32,45,48,46,49,53,56,55,53,44,48,46,48,52,50,51,32,45,48,46,48,54,51,53,44,48,46,48,49,48,54,32,45,48,46,49,52,56,49,54,44,48,46,48,51,49,56,32,45,48,46,49,54,57,51,52,44,48,46,48,53,50,57,32,45,48,46,
51,49,55,53,44,48,46,49,51,55,53,56,32,45,48,46,49,52,56,49,55,44,48,46,48,55,52,49,32,45,48,46,50,53,52,44,48,46,49,57,48,53,32,45,48,46,49,48,53,56,52,44,48,46,49,49,54,52,50,32,45,48,46,49,53,56,55,
53,44,48,46,50,56,53,55,53,32,45,48,46,48,53,50,57,44,48,46,49,53,56,55,53,32,45,48,46,48,52,50,51,44,48,46,51,57,49,53,56,32,48,44,48,46,49,48,53,56,52,32,48,46,48,51,49,55,44,48,46,50,48,49,48,57,32,
48,46,48,52,50,51,44,48,46,48,56,52,55,32,48,46,48,57,53,50,44,48,46,49,53,56,55,53,32,48,46,48,56,52,55,44,48,46,49,49,54,52,49,32,48,46,50,50,50,50,53,44,48,46,50,48,49,48,56,32,48,46,49,51,55,53,57,
44,48,46,48,55,52,49,32,48,46,50,57,54,51,52,44,48,46,49,50,55,32,48,46,48,53,50,57,44,48,46,48,49,48,54,32,48,46,49,48,53,56,51,44,48,46,48,49,48,54,32,48,46,48,53,50,57,44,48,32,48,46,49,48,53,56,51,
44,48,46,48,49,48,54,32,48,46,48,55,52,49,44,48,46,48,50,49,50,32,48,46,49,55,57,57,50,44,48,46,48,51,49,55,32,48,46,49,49,54,52,50,44,48,32,48,46,49,57,48,53,44,45,48,46,48,50,49,50,32,48,46,48,54,51,
53,44,45,48,46,48,50,49,50,32,48,46,49,50,55,44,45,48,46,48,49,48,54,32,48,46,48,54,51,53,44,48,32,48,46,49,50,55,44,45,48,46,48,50,49,50,32,48,46,48,56,52,55,44,45,48,46,48,50,49,50,32,48,46,49,53,56,
55,53,44,45,48,46,48,51,49,55,32,48,46,48,56,52,55,44,45,48,46,48,49,48,54,32,48,46,49,53,56,55,53,44,45,48,46,48,52,50,51,32,48,46,51,48,54,57,50,44,45,48,46,49,50,55,32,48,46,53,49,56,53,56,44,45,48,
46,51,48,54,57,50,32,48,46,50,50,50,50,53,44,45,48,46,49,55,57,57,50,32,48,46,51,51,56,54,55,44,45,48,46,52,56,54,56,51,32,122,34,32,47,62,10,32,32,32,32,32,32,32,32,60,112,97,116,104,10,32,32,32,32,32,
32,32,32,32,32,32,105,100,61,34,112,97,116,104,51,56,53,57,34,10,32,32,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,111,110,116,45,115,116,121,108,101,58,110,111,114,109,97,108,59,102,111,
110,116,45,118,97,114,105,97,110,116,58,110,111,114,109,97,108,59,102,111,110,116,45,119,101,105,103,104,116,58,110,111,114,109,97,108,59,102,111,110,116,45,115,116,114,101,116,99,104,58,110,111,114,109,
97,108,59,102,111,110,116,45,102,97,109,105,108,121,58,39,65,108,116,101,32,72,97,97,115,32,71,114,111,116,101,115,107,39,59,45,105,110,107,115,99,97,112,101,45,102,111,110,116,45,115,112,101,99,105,102,
105,99,97,116,105,111,110,58,39,65,108,116,101,32,72,97,97,115,32,71,114,111,116,101,115,107,39,59,102,105,108,108,58,35,102,50,102,50,102,50,59,115,116,114,111,107,101,45,119,105,100,116,104,58,48,46,
50,54,52,53,56,51,51,50,34,10,32,32,32,32,32,32,32,32,32,32,32,100,61,34,109,32,49,54,49,46,55,54,50,54,56,44,49,48,53,46,52,52,55,55,49,32,113,32,48,46,55,54,50,44,45,48,46,48,49,48,54,32,49,46,50,56,
48,53,56,44,48,46,49,55,57,57,50,32,48,46,53,49,56,53,57,44,48,46,49,55,57,57,50,32,48,46,56,48,52,51,52,44,48,46,54,52,53,53,56,32,48,46,48,55,52,49,44,48,46,49,49,54,52,50,32,48,46,49,49,54,52,49,44,
48,46,50,52,51,52,50,32,48,46,48,52,50,51,44,48,46,49,49,54,52,50,32,48,46,48,56,52,55,44,48,46,50,54,52,53,56,32,48,46,48,50,49,50,44,48,46,48,55,52,49,32,48,46,48,49,48,54,44,48,46,49,53,56,55,53,32,
45,48,46,48,49,48,54,44,48,46,48,55,52,49,32,45,48,46,48,51,49,55,44,48,46,49,49,54,52,50,32,45,48,46,48,50,49,50,44,48,46,48,50,49,50,32,45,48,46,48,53,50,57,44,48,46,48,52,50,51,32,45,48,46,48,51,49,
56,44,48,46,48,49,48,54,32,45,48,46,48,53,50,57,44,48,46,48,51,49,56,32,45,48,46,48,53,50,57,44,48,46,48,50,49,50,32,45,48,46,49,48,53,56,51,44,48,46,48,49,48,54,32,45,48,46,48,53,50,57,44,45,48,46,48,
49,48,54,32,45,48,46,49,48,53,56,52,44,48,32,45,48,46,48,56,52,55,44,48,46,48,50,49,50,32,45,48,46,50,50,50,50,53,44,48,46,48,49,48,54,32,45,48,46,49,51,55,53,56,44,45,48,46,48,50,49,50,32,45,48,46,49,
57,48,53,44,45,48,46,48,53,50,57,32,45,48,46,48,56,52,55,44,45,48,46,48,53,50,57,32,45,48,46,49,49,54,52,49,44,45,48,46,49,51,55,53,56,32,45,48,46,48,50,49,50,44,45,48,46,48,57,53,50,32,45,48,46,48,55,
52,49,44,45,48,46,49,55,57,57,50,32,45,48,46,48,57,53,51,44,45,48,46,49,54,57,51,51,32,45,48,46,50,54,52,53,56,44,45,48,46,51,48,54,57,49,32,45,48,46,49,49,54,52,50,44,45,48,46,48,56,52,55,32,45,48,46,
50,53,52,44,45,48,46,49,50,55,32,45,48,46,49,51,55,53,56,44,45,48,46,48,52,50,51,32,45,48,46,50,57,54,51,51,44,45,48,46,48,56,52,55,32,45,48,46,48,53,50,57,44,45,48,46,48,50,49,50,32,45,48,46,49,48,53,
56,52,44,45,48,46,48,49,48,54,32,45,48,46,48,53,50,57,44,48,32,45,48,46,49,48,53,56,51,44,45,48,46,48,49,48,54,32,45,48,46,48,54,51,53,44,45,48,46,48,50,49,50,32,45,48,46,49,53,56,55,53,44,45,48,46,48,
50,49,50,32,45,48,46,48,57,53,50,44,48,32,45,48,46,49,53,56,55,53,44,48,46,48,50,49,50,32,104,32,45,48,46,48,54,51,53,32,113,32,45,48,46,48,53,50,57,44,48,46,48,49,48,54,32,45,48,46,49,48,53,56,51,44,
48,46,48,50,49,50,32,45,48,46,48,53,50,57,44,48,32,45,48,46,49,48,53,56,52,44,48,46,48,49,48,54,32,45,48,46,50,55,53,49,54,44,48,46,48,54,51,53,32,45,48,46,52,53,53,48,56,44,48,46,49,55,57,57,50,32,45,
48,46,49,55,57,57,50,44,48,46,49,49,54,52,50,32,45,48,46,50,52,51,52,50,44,48,46,51,57,49,53,56,32,45,48,46,48,50,49,50,44,48,46,48,55,52,49,32,45,48,46,48,49,48,54,44,48,46,49,52,56,49,55,32,48,46,48,
50,49,50,44,48,46,48,55,52,49,32,48,46,48,52,50,51,44,48,46,49,50,55,32,48,46,48,54,51,53,44,48,46,49,53,56,55,53,32,48,46,49,52,56,49,55,44,48,46,50,51,50,56,51,32,48,46,48,57,53,50,44,48,46,48,54,51,
53,32,48,46,50,51,50,56,51,44,48,46,49,52,56,49,55,32,48,46,49,54,57,51,52,44,48,46,49,48,53,56,51,32,48,46,51,56,49,44,48,46,49,53,56,55,53,32,48,46,50,49,49,54,55,44,48,46,48,52,50,51,32,48,46,52,50,
51,51,52,44,48,46,49,48,53,56,51,32,48,46,49,51,55,53,56,44,48,46,48,52,50,51,32,48,46,50,55,53,49,54,44,48,46,48,54,51,53,32,48,46,49,51,55,53,57,44,48,46,48,50,49,50,32,48,46,50,55,53,49,55,44,48,46,
48,54,51,53,32,48,46,50,50,50,50,53,44,48,46,48,54,51,53,32,48,46,52,51,51,57,50,44,48,46,49,50,55,32,48,46,50,49,49,54,54,44,48,46,48,53,50,57,32,48,46,52,50,51,51,51,44,48,46,49,51,55,53,57,32,48,46,
48,53,50,57,44,48,46,48,50,49,50,32,48,46,49,48,53,56,51,44,48,46,48,52,50,51,32,48,46,48,53,50,57,44,48,46,48,50,49,50,32,48,46,49,48,53,56,52,44,48,46,48,53,50,57,32,48,46,50,49,49,54,54,44,48,46,49,
50,55,32,48,46,51,55,48,52,49,44,48,46,50,56,53,55,53,32,48,46,49,53,56,55,53,44,48,46,49,53,56,55,53,32,48,46,50,54,52,53,57,44,48,46,52,48,50,49,54,32,48,46,48,50,49,50,44,48,46,48,55,52,49,32,48,46,
48,52,50,51,44,48,46,50,48,49,48,57,32,48,46,48,50,49,50,44,48,46,48,55,52,49,32,48,46,48,50,49,50,44,48,46,49,53,56,55,53,32,48,46,48,49,48,54,44,48,46,48,55,52,49,32,48,46,48,49,48,54,44,48,46,49,54,
57,51,51,32,118,32,48,46,49,48,53,56,51,32,113,32,45,48,46,48,50,49,50,44,48,46,48,55,52,49,32,45,48,46,48,50,49,50,44,48,46,49,52,56,49,55,32,48,44,48,46,48,55,52,49,32,45,48,46,48,50,49,50,44,48,46,
49,52,56,49,55,32,45,48,46,48,51,49,56,44,48,46,49,49,54,52,49,32,45,48,46,48,55,52,49,44,48,46,50,50,50,50,53,32,45,48,46,48,52,50,51,44,48,46,49,48,53,56,51,32,45,48,46,48,57,53,50,44,48,46,49,57,48,
53,32,45,48,46,49,54,57,51,51,44,48,46,50,55,53,49,54,32,45,48,46,52,51,51,57,49,44,48,46,52,52,52,53,32,45,48,46,50,54,52,53,57,44,48,46,49,53,56,55,53,32,45,48,46,53,57,50,54,55,44,48,46,50,55,53,49,
54,32,45,48,46,48,57,53,50,44,48,46,48,51,49,55,32,45,48,46,50,48,49,48,56,44,48,46,48,52,50,51,32,45,48,46,48,57,53,51,44,48,46,48,50,49,50,32,45,48,46,49,57,48,53,44,48,46,48,53,50,57,32,45,48,46,49,
48,53,56,52,44,48,46,48,50,49,50,32,45,48,46,50,49,49,54,55,44,48,46,48,50,49,50,32,45,48,46,49,48,53,56,51,44,48,32,45,48,46,50,49,49,54,55,44,48,46,48,50,49,50,32,104,32,45,48,46,49,48,53,56,51,32,113,
32,45,48,46,48,53,50,57,44,48,46,48,49,48,54,32,45,48,46,49,52,56,49,55,44,48,46,48,49,48,54,32,45,48,46,48,57,53,51,44,48,32,45,48,46,49,52,56,49,54,44,45,48,46,48,49,48,54,32,104,32,45,48,46,49,49,54,
52,50,32,113,32,45,48,46,48,54,51,53,44,45,48,46,48,50,49,50,32,45,48,46,49,51,55,53,56,44,45,48,46,48,50,49,50,32,45,48,46,48,55,52,49,44,48,46,48,49,48,54,32,45,48,46,49,50,55,44,45,48,46,48,49,48,54,
32,45,48,46,48,53,50,57,44,45,48,46,48,49,48,54,32,45,48,46,49,51,55,53,57,44,45,48,46,48,49,48,54,32,45,48,46,48,56,52,55,44,45,48,46,48,50,49,50,32,45,48,46,49,54,57,51,51,44,45,48,46,48,52,50,51,32,
45,48,46,48,55,52,49,44,45,48,46,48,49,48,54,32,45,48,46,49,53,56,55,53,44,45,48,46,48,52,50,51,32,45,48,46,51,50,56,48,56,44,45,48,46,49,48,53,56,52,32,45,48,46,53,55,49,53,44,45,48,46,50,54,52,53,57,
32,45,48,46,50,52,51,52,50,44,45,48,46,49,53,56,55,53,32,45,48,46,52,50,51,51,51,44,45,48,46,52,49,50,55,53,32,45,48,46,48,57,53,50,44,45,48,46,49,51,55,53,56,32,45,48,46,49,53,56,55,53,44,45,48,46,50,
56,53,55,53,32,45,48,46,48,54,51,53,44,45,48,46,49,52,56,49,54,32,45,48,46,49,49,54,52,50,44,45,48,46,51,50,56,48,56,32,45,48,46,48,50,49,50,44,45,48,46,48,54,51,53,32,45,48,46,48,50,49,50,44,45,48,46,
49,52,56,49,55,32,48,46,48,49,48,54,44,45,48,46,48,57,53,50,32,48,46,48,51,49,55,44,45,48,46,49,52,56,49,54,32,48,46,48,51,49,55,44,45,48,46,48,52,50,51,32,48,46,49,48,53,56,52,44,45,48,46,48,56,52,55,
32,48,46,48,53,50,57,44,45,48,46,48,50,49,50,32,48,46,49,52,56,49,54,44,45,48,46,48,50,49,50,32,48,46,48,57,53,51,44,45,48,46,48,49,48,54,32,48,46,49,57,48,53,44,48,32,48,46,48,57,53,50,44,48,46,48,49,
48,54,32,48,46,49,55,57,57,50,44,48,46,48,51,49,56,32,48,46,48,57,53,50,44,48,46,48,49,48,54,32,48,46,49,50,55,44,48,46,48,51,49,56,32,48,46,48,55,52,49,44,48,46,48,53,50,57,32,48,46,48,57,53,50,44,48,
46,49,53,56,55,53,32,48,46,48,51,49,56,44,48,46,48,57,53,51,32,48,46,48,55,52,49,44,48,46,49,55,57,57,50,32,48,46,49,48,53,56,52,44,48,46,50,51,50,56,51,32,48,46,50,56,53,55,53,44,48,46,51,53,57,56,51,
32,48,46,49,50,55,44,48,46,48,57,53,51,32,48,46,50,56,53,55,53,44,48,46,49,52,56,49,55,32,48,46,49,53,56,55,53,44,48,46,48,53,50,57,32,48,46,51,51,56,54,55,44,48,46,49,48,53,56,51,32,108,32,48,46,50,50,
50,50,53,44,48,46,48,51,49,56,32,104,32,48,46,48,54,51,53,32,113,32,48,46,48,55,52,49,44,48,46,48,50,49,50,32,48,46,49,57,48,53,44,48,46,48,50,49,50,32,48,46,49,50,55,44,48,32,48,46,50,49,49,54,55,44,
45,48,46,48,50,49,50,32,48,46,48,55,52,49,44,48,32,48,46,49,49,54,52,49,44,45,48,46,48,49,48,54,32,48,46,48,54,51,53,44,45,48,46,48,49,48,54,32,48,46,49,50,55,44,45,48,46,48,49,48,54,32,48,46,48,54,51,
53,44,45,48,46,48,49,48,54,32,48,46,49,49,54,52,50,44,45,48,46,48,50,49,50,32,48,46,51,48,54,57,50,44,45,48,46,48,56,52,55,32,48,46,52,56,54,56,51,44,45,48,46,50,50,50,50,53,32,48,46,49,55,57,57,50,44,
45,48,46,49,51,55,53,56,32,48,46,50,53,52,44,45,48,46,52,54,53,54,55,32,48,46,48,50,49,50,44,45,48,46,48,53,50,57,32,48,46,48,49,48,54,44,45,48,46,49,49,54,52,49,32,45,48,46,48,49,48,54,44,45,48,46,48,
54,51,53,32,45,48,46,48,50,49,50,44,45,48,46,48,57,53,51,32,45,48,46,48,53,50,57,44,45,48,46,50,48,49,48,57,32,45,48,46,49,52,56,49,55,44,45,48,46,50,57,54,51,52,32,45,48,46,48,57,53,50,44,45,48,46,49,
48,53,56,51,32,45,48,46,50,52,51,52,49,44,45,48,46,49,57,48,53,32,45,48,46,48,57,53,50,44,45,48,46,48,54,51,53,32,45,48,46,50,49,49,54,55,44,45,48,46,49,48,53,56,51,32,45,48,46,49,49,54,52,50,44,45,48,
46,48,52,50,51,32,45,48,46,50,51,50,56,51,44,45,48,46,48,56,52,55,32,45,48,46,48,51,49,55,44,45,48,46,48,49,48,54,32,45,48,46,48,54,51,53,44,45,48,46,48,49,48,54,32,45,48,46,48,50,49,50,44,48,32,45,48,
46,48,53,50,57,44,45,48,46,48,50,49,50,32,45,48,46,51,53,57,56,51,44,45,48,46,49,49,54,52,49,32,45,48,46,55,53,49,52,50,44,45,48,46,49,54,57,51,51,32,45,48,46,51,56,49,44,45,48,46,48,54,51,53,32,45,48,
46,55,49,57,54,54,44,45,48,46,50,48,49,48,56,32,45,48,46,49,48,53,56,52,44,45,48,46,48,52,50,51,32,45,48,46,50,48,49,48,57,44,45,48,46,48,55,52,49,32,45,48,46,48,57,53,50,44,45,48,46,48,51,49,56,32,45,
48,46,49,55,57,57,49,44,45,48,46,48,56,52,55,32,45,48,46,50,48,49,48,57,44,45,48,46,49,49,54,52,50,32,45,48,46,51,55,48,52,50,44,45,48,46,50,54,52,53,57,32,45,48,46,49,54,57,51,51,44,45,48,46,49,52,56,
49,54,32,45,48,46,50,54,52,53,56,44,45,48,46,51,55,48,52,49,32,45,48,46,48,51,49,56,44,45,48,46,48,55,52,49,32,45,48,46,48,53,50,57,44,45,48,46,49,52,56,49,55,32,45,48,46,48,49,48,54,44,45,48,46,48,55,
52,49,32,45,48,46,48,51,49,56,44,45,48,46,49,53,56,55,53,32,45,48,46,48,51,49,55,44,45,48,46,49,50,55,32,45,48,46,48,50,49,50,44,45,48,46,50,56,53,55,53,32,48,46,48,49,48,54,44,45,48,46,49,54,57,51,51,
32,48,46,48,51,49,55,44,45,48,46,50,56,53,55,53,32,48,46,48,50,49,50,44,45,48,46,48,53,50,57,32,48,46,48,50,49,50,44,45,48,46,48,56,52,55,32,48,46,48,49,48,54,44,45,48,46,48,52,50,51,32,48,46,48,51,49,
55,44,45,48,46,48,56,52,55,32,48,46,48,57,53,51,44,45,48,46,50,52,51,52,50,32,48,46,50,53,52,44,45,48,46,52,50,51,51,52,32,48,46,49,54,57,51,51,44,45,48,46,49,55,57,57,49,32,48,46,51,56,49,44,45,48,46,
51,49,55,53,32,48,46,49,53,56,55,53,44,45,48,46,48,57,53,50,32,48,46,51,52,57,50,53,44,45,48,46,49,52,56,49,54,32,48,46,49,57,48,53,44,45,48,46,48,54,51,53,32,48,46,52,48,50,49,55,44,45,48,46,49,49,54,
52,50,32,48,46,48,55,52,49,44,45,48,46,48,49,48,54,32,48,46,49,51,55,53,56,44,45,48,46,48,50,49,50,32,48,46,48,55,52,49,44,45,48,46,48,49,48,54,32,48,46,49,53,56,55,53,44,45,48,46,48,50,49,50,32,48,46,
48,51,49,56,44,45,48,46,48,49,48,54,32,48,46,48,56,52,55,44,48,32,48,46,48,53,50,57,44,48,46,48,49,48,54,32,48,46,48,55,52,49,44,45,48,46,48,49,48,54,32,122,34,32,47,62,10,32,32,32,32,32,32,32,32,60,112,
97,116,104,10,32,32,32,32,32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,51,56,54,49,34,10,32,32,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,111,110,116,45,115,116,121,108,101,58,110,111,
114,109,97,108,59,102,111,110,116,45,118,97,114,105,97,110,116,58,110,111,114,109,97,108,59,102,111,110,116,45,119,101,105,103,104,116,58,110,111,114,109,97,108,59,102,111,110,116,45,115,116,114,101,116,
99,104,58,110,111,114,109,97,108,59,102,111,110,116,45,102,97,109,105,108,121,58,39,65,108,116,101,32,72,97,97,115,32,71,114,111,116,101,115,107,39,59,45,105,110,107,115,99,97,112,101,45,102,111,110,116,
45,115,112,101,99,105,102,105,99,97,116,105,111,110,58,39,65,108,116,101,32,72,97,97,115,32,71,114,111,116,101,115,107,39,59,102,105,108,108,58,35,102,50,102,50,102,50,59,115,116,114,111,107,101,45,119,
105,100,116,104,58,48,46,50,54,52,53,56,51,51,50,34,10,32,32,32,32,32,32,32,32,32,32,32,100,61,34,109,32,49,55,48,46,56,54,55,57,57,44,49,48,51,46,51,51,49,48,53,32,113,32,48,46,54,48,51,50,53,44,45,48,
46,48,50,49,50,32,49,46,48,54,56,57,49,44,48,46,48,57,53,50,32,48,46,52,54,53,54,55,44,48,46,49,49,54,52,49,32,48,46,56,50,53,53,44,48,46,51,52,57,50,53,32,48,46,48,54,51,53,44,48,46,48,52,50,51,32,48,
46,49,49,54,52,50,44,48,46,48,55,52,49,32,48,46,48,54,51,53,44,48,46,48,51,49,55,32,48,46,49,49,54,52,50,44,48,46,48,56,52,55,32,48,46,48,50,49,50,44,48,46,48,50,49,50,32,48,46,48,52,50,51,44,48,46,48,
52,50,51,32,48,46,48,50,49,50,44,48,46,48,49,48,54,32,48,46,48,52,50,51,44,48,46,48,51,49,56,32,48,46,48,50,49,50,44,48,46,48,50,49,50,32,48,46,48,53,50,57,44,48,46,48,53,50,57,32,48,46,48,51,49,55,44,
48,46,48,50,49,50,32,48,46,48,53,50,57,44,48,46,48,53,50,57,32,48,46,48,52,50,51,44,48,46,48,53,50,57,32,48,46,48,56,52,55,44,48,46,49,48,53,56,52,32,48,46,48,52,50,51,44,48,46,48,53,50,57,32,48,46,48,
56,52,55,44,48,46,49,48,53,56,51,32,48,46,48,52,50,51,44,48,46,48,53,50,57,32,48,46,48,54,51,53,44,48,46,49,49,54,52,50,32,48,46,48,51,49,56,44,48,46,48,53,50,57,32,48,46,48,54,51,53,44,48,46,49,48,53,
56,51,32,48,46,48,57,53,50,44,48,46,49,52,56,49,55,32,48,46,49,52,56,49,55,44,48,46,51,51,56,54,55,32,48,46,48,53,50,57,44,48,46,49,55,57,57,49,32,48,46,49,48,53,56,51,44,48,46,51,56,49,32,48,46,48,50,
49,50,44,48,46,48,56,52,55,32,48,46,48,49,48,54,44,48,46,49,57,48,53,32,48,44,48,46,48,57,53,51,32,45,48,46,48,51,49,55,44,48,46,49,52,56,49,54,32,45,48,46,48,53,50,57,44,48,46,48,55,52,49,32,45,48,46,
49,54,57,51,51,44,48,46,48,56,52,55,32,45,48,46,49,49,54,52,50,44,48,46,48,49,48,54,32,45,48,46,50,52,51,52,50,44,48,46,48,49,48,54,32,45,48,46,48,53,50,57,44,48,32,45,48,46,48,57,53,51,44,48,32,45,48,
46,48,52,50,51,44,48,32,45,48,46,48,56,52,55,44,45,48,46,48,49,48,54,32,45,48,46,48,53,50,57,44,45,48,46,48,49,48,54,32,45,48,46,48,57,53,50,44,45,48,46,48,49,48,54,32,45,48,46,48,51,49,56,44,48,32,45,
48,46,48,55,52,49,44,45,48,46,48,50,49,50,32,45,48,46,48,50,49,50,44,45,48,46,48,49,48,54,32,45,48,46,48,52,50,51,44,45,48,46,48,50,49,50,32,45,48,46,48,50,49,50,44,45,48,46,48,50,49,50,32,45,48,46,48,
52,50,51,44,45,48,46,48,52,50,51,32,45,48,46,48,54,51,53,44,45,48,46,48,57,53,50,32,45,48,46,48,57,53,50,44,45,48,46,50,51,50,56,51,32,45,48,46,48,50,49,50,44,45,48,46,49,51,55,53,57,32,45,48,46,48,55,
52,49,44,45,48,46,50,52,51,52,50,32,45,48,46,49,55,57,57,49,44,45,48,46,52,50,51,51,51,32,45,48,46,53,48,56,44,45,48,46,54,51,53,32,45,48,46,50,52,51,52,49,44,45,48,46,49,54,57,51,51,32,45,48,46,53,56,
50,48,56,44,45,48,46,50,53,52,32,45,48,46,48,54,51,53,44,45,48,46,48,50,49,50,32,45,48,46,49,50,55,44,45,48,46,48,50,49,50,32,45,48,46,48,54,51,53,44,45,48,46,48,49,48,54,32,45,48,46,49,50,55,44,45,48,
46,48,51,49,56,32,104,32,45,48,46,48,56,52,55,32,113,32,45,48,46,48,56,52,55,44,45,48,46,48,50,49,50,32,45,48,46,49,54,57,51,51,44,45,48,46,48,50,49,50,32,45,48,46,48,56,52,55,44,48,32,45,48,46,49,54,
57,51,51,44,48,32,45,48,46,53,53,48,51,52,44,48,32,45,48,46,57,50,48,55,53,44,48,46,49,54,57,51,51,32,45,48,46,50,49,49,54,55,44,48,46,48,56,52,55,32,45,48,46,51,56,49,44,48,46,50,51,50,56,51,32,45,48,
46,49,54,57,51,52,44,48,46,49,51,55,53,57,32,45,48,46,50,53,52,44,48,46,51,52,57,50,53,32,45,48,46,48,52,50,51,44,48,46,48,56,52,55,32,45,48,46,48,54,51,53,44,48,46,49,55,57,57,50,32,45,48,46,48,49,48,
54,44,48,46,48,56,52,55,32,45,48,46,48,51,49,56,44,48,46,49,57,48,53,32,45,48,46,48,50,49,50,44,48,46,48,56,52,55,32,45,48,46,48,50,49,50,44,48,46,49,57,48,53,32,48,46,48,49,48,54,44,48,46,49,48,53,56,
51,32,48,46,48,51,49,55,44,48,46,49,55,57,57,50,32,48,46,48,55,52,49,44,48,46,50,52,51,52,49,32,48,46,49,55,57,57,50,44,48,46,51,55,48,52,49,32,48,46,49,49,54,52,49,44,48,46,49,50,55,32,48,46,51,49,55,
53,44,48,46,50,52,51,52,50,32,48,46,50,51,50,56,51,44,48,46,49,52,56,49,55,32,48,46,53,49,56,53,56,44,48,46,50,50,50,50,53,32,48,46,50,56,53,55,53,44,48,46,48,55,52,49,32,48,46,53,57,50,54,55,44,48,46,
49,53,56,55,53,32,48,46,48,57,53,50,44,48,46,48,50,49,50,32,48,46,49,57,48,53,44,48,46,48,51,49,56,32,48,46,48,57,53,50,44,48,46,48,49,48,54,32,48,46,49,57,48,53,44,48,46,48,52,50,51,32,48,46,49,52,56,
49,54,44,48,46,48,52,50,51,32,48,46,51,48,54,57,49,44,48,46,48,55,52,49,32,48,46,49,54,57,51,52,44,48,46,48,50,49,50,32,48,46,51,49,55,53,44,48,46,48,55,52,49,32,48,46,48,52,50,51,44,48,46,48,49,48,54,
32,48,46,48,55,52,49,44,48,46,48,50,49,50,32,48,46,48,51,49,55,44,48,32,48,46,48,55,52,49,44,48,46,48,49,48,54,32,48,46,48,57,53,50,44,48,46,48,50,49,50,32,48,46,49,57,48,53,44,48,46,48,53,50,57,32,48,
46,48,57,53,50,44,48,46,48,50,49,50,32,48,46,49,57,48,53,44,48,46,48,53,50,57,32,48,46,50,49,49,54,55,44,48,46,48,55,52,49,32,48,46,52,49,50,55,53,44,48,46,49,53,56,55,53,32,48,46,50,48,49,48,56,44,48,
46,48,56,52,55,32,48,46,51,56,49,44,48,46,50,48,49,48,57,32,48,46,50,51,50,56,51,44,48,46,49,52,56,49,54,32,48,46,52,49,50,55,53,44,48,46,51,52,57,50,53,32,48,46,49,57,48,53,44,48,46,49,57,48,53,32,48,
46,51,48,54,57,50,44,48,46,52,53,53,48,56,32,48,46,48,52,50,51,44,48,46,48,57,53,50,32,48,46,48,54,51,53,44,48,46,49,57,48,53,32,48,46,48,50,49,50,44,48,46,48,57,53,50,32,48,46,48,52,50,51,44,48,46,50,
48,49,48,56,32,48,46,48,50,49,50,44,48,46,48,53,50,57,32,48,46,48,50,49,50,44,48,46,49,50,55,32,48,44,48,46,48,54,51,53,32,48,46,48,50,49,50,44,48,46,49,50,55,32,118,32,48,46,49,49,54,52,50,32,48,46,48,
57,53,50,32,113,32,45,48,46,48,49,48,54,44,48,46,48,52,50,51,32,45,48,46,48,49,48,54,44,48,46,48,56,52,55,32,48,44,48,46,48,52,50,51,32,48,44,48,46,48,56,52,55,32,45,48,46,48,50,49,50,44,48,46,48,56,52,
55,32,45,48,46,48,51,49,55,44,48,46,49,55,57,57,50,32,48,44,48,46,48,56,52,55,32,45,48,46,48,51,49,56,44,48,46,49,54,57,51,51,32,45,48,46,48,53,50,57,44,48,46,49,53,56,55,53,32,45,48,46,49,49,54,52,50,
44,48,46,51,48,54,57,50,32,45,48,46,48,54,51,53,44,48,46,49,52,56,49,55,32,45,48,46,49,53,56,55,53,44,48,46,50,55,53,49,55,32,45,48,46,49,55,57,57,49,44,48,46,50,55,53,49,54,32,45,48,46,52,53,53,48,56,
44,48,46,52,54,53,54,54,32,45,48,46,50,55,53,49,55,44,48,46,49,57,48,53,32,45,48,46,53,57,50,54,55,44,48,46,51,51,56,54,55,32,45,48,46,49,48,53,56,51,44,48,46,48,52,50,51,32,45,48,46,50,49,49,54,54,44,
48,46,48,55,52,49,32,45,48,46,48,57,53,51,44,48,46,48,51,49,55,32,45,48,46,50,49,49,54,55,44,48,46,48,54,51,53,32,45,48,46,49,49,54,52,50,44,48,46,48,52,50,51,32,45,48,46,50,52,51,52,50,44,48,46,48,53,
50,57,32,45,48,46,49,50,55,44,48,46,48,50,49,50,32,45,48,46,50,53,52,44,48,46,48,53,50,57,32,45,48,46,48,53,50,57,44,48,46,48,49,48,54,32,45,48,46,49,49,54,52,49,44,48,46,48,49,48,54,32,45,48,46,48,53,
50,57,44,48,32,45,48,46,49,48,53,56,52,44,48,46,48,49,48,54,32,45,48,46,48,54,51,53,44,48,32,45,48,46,49,50,55,44,48,32,45,48,46,48,54,51,53,44,48,46,48,49,48,54,32,45,48,46,49,50,55,44,48,46,48,49,48,
54,32,45,48,46,48,53,50,57,44,48,46,48,49,48,54,32,45,48,46,49,53,56,55,53,44,48,46,48,49,48,54,32,45,48,46,48,57,53,50,44,48,32,45,48,46,49,52,56,49,54,44,45,48,46,48,49,48,54,32,104,32,45,48,46,49,49,
54,52,50,32,113,32,45,48,46,49,48,53,56,51,44,45,48,46,48,50,49,50,32,45,48,46,50,49,49,54,55,44,45,48,46,48,50,49,50,32,45,48,46,49,48,53,56,51,44,48,32,45,48,46,50,48,49,48,56,44,45,48,46,48,50,49,50,
32,45,48,46,48,52,50,51,44,45,48,46,48,49,48,54,32,45,48,46,48,56,52,55,44,45,48,46,48,50,49,50,32,45,48,46,48,52,50,51,44,48,32,45,48,46,48,57,53,50,44,45,48,46,48,49,48,54,32,45,48,46,49,49,54,52,49,
44,45,48,46,48,50,49,50,32,45,48,46,50,50,50,50,53,44,45,48,46,48,52,50,51,32,45,48,46,48,57,53,50,44,45,48,46,48,50,49,50,32,45,48,46,50,48,49,48,56,44,45,48,46,48,54,51,53,32,45,48,46,53,49,56,53,56,
44,45,48,46,49,54,57,51,51,32,45,48,46,56,55,56,52,50,44,45,48,46,52,51,51,57,50,32,45,48,46,51,52,57,50,53,44,45,48,46,50,54,52,53,56,32,45,48,46,54,48,51,50,53,44,45,48,46,54,56,55,57,49,32,45,48,46,
48,56,52,55,44,45,48,46,49,51,55,53,57,32,45,48,46,49,52,56,49,54,44,45,48,46,50,56,53,55,53,32,45,48,46,48,53,50,57,44,45,48,46,49,52,56,49,55,32,45,48,46,49,48,53,56,52,44,45,48,46,51,49,55,53,32,45,
48,46,48,50,49,50,44,45,48,46,48,54,51,53,32,45,48,46,48,51,49,56,44,45,48,46,49,50,55,32,48,44,45,48,46,48,54,51,53,32,45,48,46,48,50,49,50,44,45,48,46,49,49,54,52,50,32,45,48,46,48,50,49,50,44,45,48,
46,48,57,53,50,32,45,48,46,48,50,49,50,44,45,48,46,50,49,49,54,55,32,48,44,45,48,46,49,50,55,32,48,46,48,53,50,57,44,45,48,46,49,55,57,57,49,32,48,46,48,53,50,57,44,45,48,46,48,55,52,49,32,48,46,49,54,
57,51,51,44,45,48,46,48,56,52,55,32,48,46,49,50,55,44,45,48,46,48,49,48,54,32,48,46,50,52,51,52,50,44,45,48,46,48,49,48,54,32,48,46,48,52,50,51,44,48,32,48,46,48,56,52,55,44,48,32,48,46,48,53,50,57,44,
48,32,48,46,48,56,52,55,44,48,46,48,49,48,54,32,48,46,48,53,50,57,44,48,46,48,49,48,54,32,48,46,48,56,52,55,44,48,46,48,49,48,54,32,48,46,48,51,49,55,44,45,48,46,48,49,48,54,32,48,46,48,55,52,49,44,48,
46,48,49,48,54,32,48,46,48,55,52,49,44,48,46,48,52,50,51,32,48,46,48,57,53,50,44,48,46,48,56,52,55,32,48,46,48,51,49,55,44,48,46,48,53,50,57,32,48,46,48,51,49,55,44,48,46,49,49,54,52,50,32,48,46,48,49,
48,54,44,48,46,48,53,50,57,32,48,46,48,51,49,56,44,48,46,49,49,54,52,50,32,48,46,48,50,49,50,44,48,46,48,55,52,49,32,48,46,48,51,49,55,44,48,46,49,52,56,49,54,32,48,46,48,50,49,50,44,48,46,48,55,52,49,
32,48,46,48,53,50,57,44,48,46,49,52,56,49,55,32,48,46,49,55,57,57,49,44,48,46,52,53,53,48,56,32,48,46,53,51,57,55,53,44,48,46,55,49,57,54,55,32,48,46,49,55,57,57,49,44,48,46,49,51,55,53,56,32,48,46,51,
57,49,53,56,44,48,46,50,50,50,50,53,32,48,46,50,49,49,54,55,44,48,46,48,56,52,55,32,48,46,52,53,53,48,56,44,48,46,49,53,56,55,53,32,48,46,48,56,52,55,44,48,46,48,50,49,50,32,48,46,49,54,57,51,52,44,48,
46,48,51,49,56,32,48,46,48,56,52,55,44,48,46,48,49,48,54,32,48,46,49,54,57,51,51,44,48,46,48,50,49,50,32,48,46,48,50,49,50,44,48,32,48,46,48,53,50,57,44,48,46,48,49,48,54,32,48,46,48,51,49,56,44,48,32,
48,46,48,54,51,53,44,48,32,48,46,48,52,50,51,44,48,46,48,49,48,54,32,48,46,48,57,53,50,44,48,32,48,46,48,54,51,53,44,45,48,46,48,49,48,54,32,48,46,49,49,54,52,49,44,48,46,48,49,48,54,32,48,46,48,52,50,
51,44,48,46,48,49,48,54,32,48,46,49,49,54,52,50,44,48,46,48,49,48,54,32,48,46,48,56,52,55,44,48,32,48,46,49,51,55,53,56,44,45,48,46,48,50,49,50,32,104,32,48,46,48,55,52,49,32,113,32,48,46,48,53,50,57,
44,45,48,46,48,49,48,54,32,48,46,49,48,53,56,51,44,45,48,46,48,49,48,54,32,48,46,48,53,50,57,44,48,32,48,46,49,48,53,56,51,44,45,48,46,48,49,48,54,32,48,46,48,51,49,56,44,45,48,46,48,49,48,54,32,48,46,
48,53,50,57,44,48,32,48,46,48,51,49,56,44,48,32,48,46,48,55,52,49,44,45,48,46,48,50,49,50,32,48,46,48,56,52,55,44,45,48,46,48,50,49,50,32,48,46,49,55,57,57,50,44,45,48,46,48,51,49,56,32,48,46,48,57,53,
50,44,45,48,46,48,50,49,50,32,48,46,49,55,57,57,50,44,45,48,46,48,53,50,57,32,48,46,50,55,53,49,54,44,45,48,46,49,49,54,52,50,32,48,46,52,56,54,56,51,44,45,48,46,50,54,52,53,57,32,48,46,50,50,50,50,53,
44,45,48,46,49,53,56,55,53,32,48,46,51,51,56,54,55,44,45,48,46,52,50,51,51,51,32,48,46,48,51,49,56,44,45,48,46,48,56,52,55,32,48,46,48,53,50,57,44,45,48,46,49,54,57,51,51,32,48,46,48,50,49,50,44,45,48,
46,48,56,52,55,32,48,46,48,52,50,51,44,45,48,46,49,54,57,51,52,32,48,46,48,50,49,50,44,45,48,46,48,56,52,55,32,48,46,48,50,49,50,44,45,48,46,50,48,49,48,56,32,48,44,45,48,46,49,49,54,52,50,32,45,48,46,
48,50,49,50,44,45,48,46,49,57,48,53,32,45,48,46,48,50,49,50,44,45,48,46,48,53,50,57,32,45,48,46,48,50,49,50,44,45,48,46,48,56,52,55,32,48,44,45,48,46,48,52,50,51,32,45,48,46,48,50,49,50,44,45,48,46,48,
56,52,55,32,45,48,46,48,54,51,53,44,45,48,46,49,55,57,57,50,32,45,48,46,49,54,57,51,51,44,45,48,46,50,57,54,51,52,32,45,48,46,49,48,53,56,51,44,45,48,46,49,50,55,32,45,48,46,50,53,52,44,45,48,46,50,51,
50,56,51,32,45,48,46,50,50,50,50,53,44,45,48,46,49,53,56,55,53,32,45,48,46,52,57,55,52,50,44,45,48,46,50,53,52,32,45,48,46,50,55,53,49,54,44,45,48,46,48,57,53,50,32,45,48,46,53,56,50,48,56,44,45,48,46,
49,53,56,55,53,32,45,48,46,50,57,54,51,51,44,45,48,46,48,55,52,49,32,45,48,46,54,49,51,56,51,44,45,48,46,49,51,55,53,56,32,45,48,46,51,49,55,53,44,45,48,46,48,54,51,53,32,45,48,46,54,49,51,56,52,44,45,
48,46,49,52,56,49,55,32,45,48,46,49,54,57,51,51,44,45,48,46,48,53,50,57,32,45,48,46,51,51,56,54,54,44,45,48,46,48,56,52,55,32,45,48,46,49,54,57,51,52,44,45,48,46,48,52,50,51,32,45,48,46,51,49,55,53,44,
45,48,46,49,48,53,56,51,32,45,48,46,48,50,49,50,44,45,48,46,48,49,48,54,32,45,48,46,48,51,49,55,44,45,48,46,48,49,48,54,32,45,48,46,48,49,48,54,44,48,32,45,48,46,48,51,49,55,44,45,48,46,48,49,48,54,32,
45,48,46,49,48,53,56,52,44,45,48,46,48,52,50,51,32,45,48,46,50,49,49,54,55,44,45,48,46,48,56,52,55,32,45,48,46,49,48,53,56,51,44,45,48,46,48,52,50,51,32,45,48,46,50,48,49,48,56,44,45,48,46,49,48,53,56,
52,32,45,48,46,50,52,51,52,50,44,45,48,46,49,52,56,49,54,32,45,48,46,52,52,52,53,44,45,48,46,51,49,55,53,32,45,48,46,50,48,49,48,57,44,45,48,46,49,55,57,57,49,32,45,48,46,51,49,55,53,44,45,48,46,52,53,
53,48,56,32,45,48,46,48,53,50,57,44,45,48,46,48,57,53,51,32,45,48,46,48,55,52,49,44,45,48,46,50,48,49,48,56,32,45,48,46,48,50,49,50,44,45,48,46,49,49,54,52,50,32,45,48,46,48,53,50,57,44,45,48,46,50,51,
50,56,52,32,48,44,45,48,46,48,52,50,51,32,45,48,46,48,49,48,54,44,45,48,46,48,55,52,49,32,48,44,45,48,46,48,51,49,56,32,48,44,45,48,46,48,54,51,53,32,45,48,46,48,51,49,56,44,45,48,46,49,50,55,32,45,48,
46,48,49,48,54,44,45,48,46,51,49,55,53,32,48,46,48,50,49,50,44,45,48,46,50,48,49,48,56,32,48,46,48,53,50,57,44,45,48,46,51,48,54,57,50,32,48,46,48,51,49,55,44,45,48,46,49,49,54,52,49,32,48,46,48,53,50,
57,44,45,48,46,50,50,50,50,53,32,48,46,48,51,49,56,44,45,48,46,49,48,53,56,51,32,48,46,48,55,52,49,44,45,48,46,50,48,49,48,56,32,48,46,49,52,56,49,54,44,45,48,46,51,49,55,53,32,48,46,51,56,49,44,45,48,
46,53,50,57,49,55,32,48,46,50,51,50,56,51,44,45,48,46,50,50,50,50,53,32,48,46,53,49,56,53,56,44,45,48,46,52,48,50,49,54,32,48,46,48,50,49,50,44,45,48,46,48,50,49,50,32,48,46,48,53,50,57,44,45,48,46,48,
50,49,50,32,48,46,48,51,49,56,44,45,48,46,48,49,48,54,32,48,46,48,53,50,57,44,45,48,46,48,51,49,56,32,48,46,50,51,50,56,52,44,45,48,46,49,49,54,52,50,32,48,46,53,51,57,55,53,44,45,48,46,50,48,49,48,56,
32,48,46,48,56,52,55,44,45,48,46,48,50,49,50,32,48,46,49,53,56,55,53,44,45,48,46,48,51,49,56,32,48,46,48,55,52,49,44,45,48,46,48,50,49,50,32,48,46,49,53,56,55,53,44,45,48,46,48,52,50,51,32,48,46,48,56,
52,55,44,45,48,46,48,50,49,50,32,48,46,49,55,57,57,50,44,45,48,46,48,50,49,50,32,48,46,49,48,53,56,51,44,48,32,48,46,49,57,48,53,44,45,48,46,48,50,49,50,32,48,46,48,51,49,55,44,45,48,46,48,49,48,54,32,
48,46,48,57,53,50,44,48,32,48,46,48,54,51,53,44,48,46,48,49,48,54,32,48,46,48,56,52,55,44,45,48,46,48,49,48,54,32,122,34,32,47,62,10,32,32,32,32,32,32,32,32,60,112,97,116,104,10,32,32,32,32,32,32,32,32,
32,32,32,105,100,61,34,112,97,116,104,51,56,54,51,34,10,32,32,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,111,110,116,45,115,116,121,108,101,58,110,111,114,109,97,108,59,102,111,110,116,45,
118,97,114,105,97,110,116,58,110,111,114,109,97,108,59,102,111,110,116,45,119,101,105,103,104,116,58,110,111,114,109,97,108,59,102,111,110,116,45,115,116,114,101,116,99,104,58,110,111,114,109,97,108,59,
102,111,110,116,45,102,97,109,105,108,121,58,39,65,108,116,101,32,72,97,97,115,32,71,114,111,116,101,115,107,39,59,45,105,110,107,115,99,97,112,101,45,102,111,110,116,45,115,112,101,99,105,102,105,99,
97,116,105,111,110,58,39,65,108,116,101,32,72,97,97,115,32,71,114,111,116,101,115,107,39,59,102,105,108,108,58,35,102,50,102,50,102,50,59,115,116,114,111,107,101,45,119,105,100,116,104,58,48,46,50,54,
52,53,56,51,51,50,34,10,32,32,32,32,32,32,32,32,32,32,32,100,61,34,109,32,49,55,53,46,56,48,52,52,53,44,49,48,51,46,57,50,51,55,49,32,113,32,48,46,49,53,56,55,53,44,48,32,48,46,51,49,55,53,44,48,46,48,
49,48,54,32,48,46,49,53,56,55,53,44,48,32,48,46,50,50,50,50,53,44,48,46,48,56,52,55,32,48,46,48,53,50,57,44,48,46,48,54,51,53,32,48,46,48,53,50,57,44,48,46,49,55,57,57,50,32,48,46,48,49,48,54,44,48,46,
49,48,53,56,51,32,48,46,48,49,48,54,44,48,46,50,50,50,50,53,32,118,32,48,46,54,49,51,56,51,32,113,32,48,44,48,46,48,56,52,55,32,45,48,46,48,49,48,54,44,48,46,49,55,57,57,50,32,48,44,48,46,48,56,52,55,
32,48,46,48,50,49,50,44,48,46,49,52,56,49,55,32,48,46,48,52,50,51,44,48,46,49,52,56,49,54,32,48,46,49,57,48,53,44,48,46,49,57,48,53,32,48,46,48,53,50,57,44,48,46,48,49,48,54,32,48,46,48,57,53,50,44,48,
46,48,49,48,54,32,48,46,48,53,50,57,44,48,32,48,46,49,48,53,56,51,44,48,32,48,46,49,49,54,52,50,44,48,32,48,46,50,56,53,55,53,44,48,32,48,46,49,54,57,51,51,44,45,48,46,48,49,48,54,32,48,46,50,52,51,52,
50,44,48,46,48,50,49,50,32,48,46,48,57,53,50,44,48,46,48,54,51,53,32,48,46,49,50,55,44,48,46,49,50,55,32,48,46,48,50,49,50,44,48,46,48,52,50,51,32,48,46,48,50,49,50,44,48,46,48,57,53,51,32,48,44,48,46,
48,53,50,57,32,48,44,48,46,49,48,53,56,51,32,48,44,48,46,49,49,54,52,50,32,45,48,46,48,49,48,54,44,48,46,50,50,50,50,53,32,45,48,46,48,49,48,54,44,48,46,48,57,53,50,32,45,48,46,48,55,52,49,44,48,46,49,
52,56,49,55,32,45,48,46,48,54,51,53,44,48,46,48,53,50,57,32,45,48,46,49,55,57,57,50,44,48,46,48,54,51,53,32,45,48,46,49,48,53,56,51,44,48,32,45,48,46,50,51,50,56,51,44,48,32,45,48,46,49,49,54,52,50,44,
45,48,46,48,49,48,54,32,45,48,46,50,51,50,56,52,44,45,48,46,48,49,48,54,32,45,48,46,49,49,54,52,49,44,48,32,45,48,46,50,48,49,48,56,44,48,46,48,51,49,56,32,45,48,46,48,55,52,49,44,48,46,48,51,49,56,32,
45,48,46,49,48,53,56,51,44,48,46,49,48,53,56,52,32,45,48,46,48,51,49,55,44,48,46,48,55,52,49,32,45,48,46,48,52,50,51,44,48,46,49,55,57,57,49,32,45,48,46,48,49,48,54,44,48,46,48,57,53,50,32,45,48,46,48,
49,48,54,44,48,46,50,49,49,54,55,32,48,46,48,49,48,54,44,48,46,49,48,53,56,51,32,48,46,48,49,48,54,44,48,46,50,49,49,54,55,32,118,32,50,46,48,57,53,53,32,113,32,48,44,48,46,50,51,50,56,51,32,45,48,46,
48,49,48,54,44,48,46,52,57,55,52,49,32,48,44,48,46,50,53,52,32,48,46,48,57,53,50,44,48,46,51,57,49,53,57,32,48,46,48,53,50,57,44,48,46,48,56,52,55,32,48,46,49,51,55,53,56,44,48,46,49,49,54,52,49,32,48,
46,48,56,52,55,44,48,46,48,51,49,56,32,48,46,50,48,49,48,57,44,48,46,48,53,50,57,32,48,46,48,51,49,55,44,48,46,48,49,48,54,32,48,46,48,54,51,53,44,48,46,48,49,48,54,32,48,46,48,52,50,51,44,45,48,46,48,
49,48,54,32,48,46,48,55,52,49,44,48,32,104,32,48,46,49,51,55,53,56,32,113,32,48,46,49,48,53,56,52,44,48,32,48,46,49,57,48,53,44,48,46,48,50,49,50,32,48,46,48,57,53,51,44,48,46,48,49,48,54,32,48,46,49,
51,55,53,57,44,48,46,48,55,52,49,32,48,46,48,50,49,50,44,48,46,48,51,49,56,32,48,46,48,51,49,55,44,48,46,49,49,54,52,50,32,48,46,48,50,49,50,44,48,46,48,55,52,49,32,48,46,48,50,49,50,44,48,46,49,53,56,
55,53,32,48,46,48,49,48,54,44,48,46,48,56,52,55,32,48,44,48,46,49,54,57,51,51,32,45,48,46,48,49,48,54,44,48,46,48,55,52,49,32,45,48,46,48,52,50,51,44,48,46,49,49,54,52,50,32,45,48,46,48,52,50,51,44,48,
46,48,55,52,49,32,45,48,46,49,53,56,55,53,44,48,46,49,48,53,56,51,32,45,48,46,48,56,52,55,44,48,46,48,50,49,50,32,45,48,46,49,57,48,53,44,48,46,48,50,49,50,32,45,48,46,49,48,53,56,51,44,45,48,46,48,49,
48,54,32,45,48,46,49,57,48,53,44,45,48,46,48,49,48,54,32,45,48,46,48,54,51,53,44,48,32,45,48,46,49,50,55,44,48,46,48,49,48,54,32,45,48,46,48,54,51,53,44,48,32,45,48,46,49,49,54,52,50,44,45,48,46,48,49,
48,54,32,104,32,45,48,46,49,50,55,32,113,32,45,48,46,48,53,50,57,44,45,48,46,48,49,48,54,32,45,48,46,49,48,53,56,51,44,45,48,46,48,49,48,54,32,45,48,46,48,52,50,51,44,45,48,46,48,49,48,54,32,45,48,46,
48,57,53,50,44,45,48,46,48,49,48,54,32,45,48,46,50,49,49,54,55,44,45,48,46,48,53,50,57,32,45,48,46,51,55,48,52,50,44,45,48,46,49,49,54,52,50,32,45,48,46,49,52,56,49,54,44,45,48,46,48,54,51,53,32,45,48,
46,50,53,52,44,45,48,46,50,50,50,50,53,32,45,48,46,49,52,56,49,54,44,45,48,46,50,49,49,54,54,32,45,48,46,49,54,57,51,51,44,45,48,46,53,50,57,49,54,32,45,48,46,48,50,49,50,44,45,48,46,51,50,56,48,57,32,
45,48,46,48,50,49,50,44,45,48,46,54,57,56,53,32,118,32,45,50,46,55,55,50,56,52,32,113,32,45,48,46,48,51,49,55,44,45,48,46,49,51,55,53,56,32,45,48,46,48,54,51,53,44,45,48,46,50,48,49,48,56,32,45,48,46,
48,50,49,50,44,45,48,46,48,54,51,53,32,45,48,46,49,52,56,49,54,44,45,48,46,49,48,53,56,51,32,45,48,46,48,52,50,51,44,45,48,46,48,49,48,54,32,45,48,46,48,56,52,55,44,48,32,45,48,46,48,52,50,51,44,48,46,
48,49,48,54,32,45,48,46,48,56,52,55,44,45,48,46,48,49,48,54,32,104,32,45,48,46,49,54,57,51,51,32,113,32,45,48,46,49,51,55,53,56,44,48,32,45,48,46,50,51,50,56,51,44,45,48,46,48,50,49,50,32,45,48,46,48,
56,52,55,44,45,48,46,48,51,49,56,32,45,48,46,49,49,54,52,50,44,45,48,46,49,50,55,32,45,48,46,48,50,49,50,44,45,48,46,48,52,50,51,32,45,48,46,48,51,49,56,44,45,48,46,48,57,53,51,32,48,44,45,48,46,48,53,
50,57,32,48,44,45,48,46,49,48,53,56,52,32,48,44,45,48,46,49,52,56,49,54,32,48,46,48,50,49,50,44,45,48,46,50,52,51,52,49,32,48,46,48,50,49,50,44,45,48,46,49,48,53,56,52,32,48,46,49,50,55,44,45,48,46,49,
53,56,55,53,32,48,46,49,49,54,52,49,44,45,48,46,48,52,50,51,32,48,46,50,57,54,51,51,44,45,48,46,48,50,49,50,32,48,46,49,57,48,53,44,48,46,48,50,49,50,32,48,46,51,49,55,53,44,45,48,46,48,50,49,50,32,48,
46,48,57,53,50,44,45,48,46,48,53,50,57,32,48,46,49,52,56,49,55,44,45,48,46,49,52,56,49,54,32,48,46,48,50,49,50,44,45,48,46,48,54,51,53,32,48,46,48,50,49,50,44,45,48,46,49,53,56,55,53,32,48,44,45,48,46,
48,57,53,51,32,48,44,45,48,46,49,55,57,57,50,32,118,32,45,48,46,54,53,54,49,55,32,113,32,48,44,45,48,46,48,56,52,55,32,48,44,45,48,46,49,54,57,51,51,32,48,44,45,48,46,48,57,53,51,32,48,46,48,50,49,50,
44,45,48,46,49,53,56,55,53,32,48,46,48,52,50,51,44,45,48,46,48,56,52,55,32,48,46,49,53,56,55,53,44,45,48,46,49,51,55,53,56,32,48,46,48,51,49,55,44,48,32,48,46,48,53,50,57,44,48,32,48,46,48,51,49,55,44,
45,48,46,48,49,48,54,32,48,46,48,53,50,57,44,45,48,46,48,49,48,54,32,122,34,32,47,62,10,32,32,32,32,32,32,32,32,60,112,97,116,104,10,32,32,32,32,32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,51,56,
54,53,34,10,32,32,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,111,110,116,45,115,116,121,108,101,58,110,111,114,109,97,108,59,102,111,110,116,45,118,97,114,105,97,110,116,58,110,111,114,109,
97,108,59,102,111,110,116,45,119,101,105,103,104,116,58,110,111,114,109,97,108,59,102,111,110,116,45,115,116,114,101,116,99,104,58,110,111,114,109,97,108,59,102,111,110,116,45,102,97,109,105,108,121,58,
39,65,108,116,101,32,72,97,97,115,32,71,114,111,116,101,115,107,39,59,45,105,110,107,115,99,97,112,101,45,102,111,110,116,45,115,112,101,99,105,102,105,99,97,116,105,111,110,58,39,65,108,116,101,32,72,
97,97,115,32,71,114,111,116,101,115,107,39,59,102,105,108,108,58,35,102,50,102,50,102,50,59,115,116,114,111,107,101,45,119,105,100,116,104,58,48,46,50,54,52,53,56,51,51,50,34,10,32,32,32,32,32,32,32,32,
32,32,32,100,61,34,109,32,49,56,51,46,50,50,56,49,54,44,49,48,56,46,48,56,50,57,54,32,113,32,48,46,48,50,49,50,44,48,46,49,48,53,56,52,32,48,44,48,46,50,52,51,52,50,32,45,48,46,48,49,48,54,44,48,46,49,
50,55,32,45,48,46,48,54,51,53,44,48,46,49,55,57,57,50,32,45,48,46,48,53,50,57,44,48,46,48,52,50,51,32,45,48,46,49,53,56,55,53,44,48,46,48,54,51,53,32,45,48,46,48,57,53,50,44,48,46,48,49,48,54,32,45,48,
46,50,50,50,50,53,44,48,46,48,49,48,54,32,45,48,46,49,50,55,44,48,32,45,48,46,50,53,52,44,48,32,45,48,46,49,50,55,44,45,48,46,48,49,48,54,32,45,48,46,50,50,50,50,53,44,45,48,46,48,49,48,54,32,104,32,45,
50,46,55,48,57,51,51,32,113,32,45,48,46,48,56,52,55,44,48,32,45,48,46,49,54,57,51,52,44,48,46,48,49,48,54,32,45,48,46,48,56,52,55,44,48,32,45,48,46,49,51,55,53,56,44,48,46,48,51,49,55,32,45,48,46,48,52,
50,51,44,48,46,48,50,49,50,32,45,48,46,48,56,52,55,44,48,46,48,56,52,55,32,45,48,46,48,50,49,50,44,48,46,48,51,49,56,32,45,48,46,48,52,50,51,44,48,46,48,57,53,51,32,45,48,46,48,50,49,50,44,48,46,48,53,
50,57,32,48,44,48,46,49,48,53,56,51,32,48,46,48,49,48,54,44,48,46,48,53,50,57,32,48,46,48,49,48,54,44,48,46,48,57,53,50,32,48,44,48,46,48,52,50,51,32,48,46,48,49,48,54,44,48,46,48,56,52,55,32,48,46,48,
50,49,50,44,48,46,48,56,52,55,32,48,46,48,52,50,51,44,48,46,49,55,57,57,49,32,48,46,48,50,49,50,44,48,46,48,56,52,55,32,48,46,48,54,51,53,44,48,46,49,54,57,51,52,32,48,46,50,52,51,52,50,44,48,46,53,55,
49,53,32,48,46,56,48,52,51,51,44,48,46,56,49,52,57,49,32,48,46,48,54,51,53,44,48,46,48,51,49,55,32,48,46,49,51,55,53,57,44,48,46,48,53,50,57,32,48,46,48,55,52,49,44,48,46,48,50,49,50,32,48,46,49,52,56,
49,54,44,48,46,48,52,50,51,32,48,46,48,51,49,56,44,48,46,48,49,48,54,32,48,46,49,48,53,56,52,44,48,46,48,49,48,54,32,48,46,49,52,56,49,54,44,48,46,48,52,50,51,32,48,46,51,57,49,53,56,44,48,46,48,52,50,
51,32,48,46,50,52,51,52,50,44,48,32,48,46,51,57,49,53,56,44,45,48,46,48,52,50,51,32,48,46,49,52,56,49,55,44,45,48,46,48,52,50,51,32,48,46,50,54,52,53,57,44,45,48,46,48,56,52,55,32,48,46,49,50,55,44,45,
48,46,48,53,50,57,32,48,46,50,51,50,56,51,44,45,48,46,49,50,55,32,48,46,49,49,54,52,50,44,45,48,46,48,55,52,49,32,48,46,50,48,49,48,56,44,45,48,46,49,54,57,51,51,32,48,46,48,56,52,55,44,45,48,46,49,48,
53,56,52,32,48,46,49,52,56,49,55,44,45,48,46,50,50,50,50,53,32,48,46,48,54,51,53,44,45,48,46,49,48,53,56,52,32,48,46,49,48,53,56,51,44,45,48,46,50,50,50,50,53,32,48,46,48,53,50,57,44,45,48,46,49,49,54,
52,50,32,48,46,49,54,57,51,52,44,45,48,46,49,54,57,51,52,32,48,46,48,52,50,51,44,45,48,46,48,50,49,50,32,48,46,49,51,55,53,56,44,45,48,46,48,51,49,56,32,48,46,48,57,53,50,44,45,48,46,48,49,48,54,32,48,
46,49,57,48,53,44,48,32,48,46,48,57,53,51,44,48,32,48,46,49,55,57,57,50,44,48,46,48,49,48,54,32,48,46,48,56,52,55,44,48,46,48,49,48,54,32,48,46,49,49,54,52,49,44,48,46,48,52,50,51,32,48,46,49,48,53,56,
52,44,48,46,48,55,52,49,32,48,46,48,53,50,57,44,48,46,50,52,51,52,50,32,45,48,46,48,56,52,55,44,48,46,50,56,53,55,53,32,45,48,46,50,50,50,50,53,44,48,46,53,48,56,32,45,48,46,50,49,49,54,55,44,48,46,51,
51,56,54,54,32,45,48,46,52,56,54,56,51,44,48,46,53,54,48,57,49,32,45,48,46,50,54,52,53,57,44,48,46,50,49,49,54,55,32,45,48,46,54,55,55,51,52,44,48,46,51,52,57,50,53,32,45,48,46,49,48,53,56,51,44,48,46,
48,52,50,51,32,45,48,46,50,49,49,54,54,44,48,46,48,53,50,57,32,45,48,46,49,48,53,56,52,44,48,46,48,50,49,50,32,45,48,46,50,50,50,50,53,44,48,46,48,52,50,51,32,45,48,46,48,53,50,57,44,48,46,48,49,48,54,
32,45,48,46,49,48,53,56,52,44,48,46,48,49,48,54,32,45,48,46,48,52,50,51,44,48,32,45,48,46,48,57,53,50,44,48,46,48,49,48,54,32,104,32,45,48,46,49,48,53,56,51,32,113,32,45,48,46,48,53,50,57,44,48,46,48,
49,48,54,32,45,48,46,49,50,55,44,48,46,48,49,48,54,32,45,48,46,48,54,51,53,44,48,32,45,48,46,49,48,53,56,51,44,45,48,46,48,49,48,54,32,104,32,45,48,46,48,57,53,50,32,113,32,45,48,46,48,53,50,57,44,45,
48,46,48,49,48,54,32,45,48,46,48,57,53,50,44,45,48,46,48,49,48,54,32,45,48,46,48,52,50,51,44,48,32,45,48,46,48,56,52,55,44,45,48,46,48,49,48,54,32,45,48,46,48,53,50,57,44,45,48,46,48,49,48,54,32,45,48,
46,49,49,54,52,50,44,45,48,46,48,50,49,50,32,45,48,46,48,54,51,53,44,48,32,45,48,46,49,49,54,52,49,44,45,48,46,48,49,48,54,32,45,48,46,48,57,53,51,44,45,48,46,48,50,49,50,32,45,48,46,49,57,48,53,44,45,
48,46,48,53,50,57,32,45,48,46,48,57,53,50,44,45,48,46,48,50,49,50,32,45,48,46,49,55,57,57,50,44,45,48,46,48,53,50,57,32,45,48,46,48,52,50,51,44,45,48,46,48,50,49,50,32,45,48,46,48,55,52,49,44,45,48,46,
48,50,49,50,32,45,48,46,48,51,49,56,44,45,48,46,48,49,48,54,32,45,48,46,48,55,52,49,44,45,48,46,48,51,49,56,32,45,48,46,49,53,56,55,53,44,45,48,46,48,55,52,49,32,45,48,46,51,51,56,54,54,44,45,48,46,49,
55,57,57,49,32,45,48,46,51,48,54,57,50,44,45,48,46,50,48,49,48,57,32,45,48,46,53,50,57,49,55,44,45,48,46,52,56,54,56,52,32,45,48,46,50,49,49,54,55,44,45,48,46,50,57,54,51,51,32,45,48,46,51,53,57,56,51,
44,45,48,46,54,53,54,49,54,32,45,48,46,48,53,50,57,44,45,48,46,49,49,54,52,50,32,45,48,46,48,56,52,55,44,45,48,46,50,52,51,52,50,32,45,48,46,48,50,49,50,44,45,48,46,49,50,55,32,45,48,46,48,54,51,53,44,
45,48,46,50,53,52,32,45,48,46,48,49,48,54,44,45,48,46,48,52,50,51,32,45,48,46,48,49,48,54,44,45,48,46,48,55,52,49,32,48,44,45,48,46,48,51,49,55,32,45,48,46,48,49,48,54,44,45,48,46,48,56,52,55,32,45,48,
46,48,50,49,50,44,45,48,46,48,56,52,55,32,45,48,46,48,51,49,56,44,45,48,46,49,55,57,57,50,32,45,48,46,48,49,48,54,44,45,48,46,48,57,53,50,32,45,48,46,48,51,49,55,44,45,48,46,49,57,48,53,32,118,32,45,48,
46,49,52,56,49,54,32,113,32,45,48,46,48,49,48,54,44,45,48,46,48,52,50,51,32,45,48,46,48,50,49,50,44,45,48,46,49,49,54,52,50,32,48,44,45,48,46,48,56,52,55,32,48,46,48,49,48,54,44,45,48,46,49,51,55,53,56,
32,118,32,45,48,46,49,49,54,52,50,32,113,32,48,46,48,50,49,50,44,45,48,46,48,53,50,57,32,48,46,48,49,48,54,44,45,48,46,49,48,53,56,51,32,48,44,45,48,46,48,54,51,53,32,48,46,48,49,48,54,44,45,48,46,49,
49,54,52,50,32,48,46,48,52,50,51,44,45,48,46,49,54,57,51,51,32,48,46,48,54,51,53,44,45,48,46,51,51,56,54,55,32,48,46,48,51,49,55,44,45,48,46,49,54,57,51,51,32,48,46,48,57,53,50,44,45,48,46,51,49,55,53,
32,48,46,48,53,50,57,44,45,48,46,49,49,54,52,49,32,48,46,48,57,53,50,44,45,48,46,50,51,50,56,51,32,48,46,48,53,50,57,44,45,48,46,49,50,55,32,48,46,49,49,54,52,49,44,45,48,46,50,52,51,52,50,32,48,46,50,
51,50,56,52,44,45,48,46,51,55,48,52,49,32,48,46,53,50,57,49,55,44,45,48,46,54,51,53,32,48,46,50,57,54,51,51,44,45,48,46,50,55,53,49,54,32,48,46,55,51,48,50,53,44,45,48,46,52,53,53,48,56,32,48,46,49,50,
55,44,45,48,46,48,53,50,57,32,48,46,50,53,52,44,45,48,46,48,55,52,49,32,48,46,49,51,55,53,56,44,45,48,46,48,51,49,56,32,48,46,50,56,53,55,53,44,45,48,46,48,54,51,53,32,48,46,48,53,50,57,44,45,48,46,48,
49,48,54,32,48,46,48,57,53,50,44,45,48,46,48,49,48,54,32,48,46,48,52,50,51,44,45,48,46,48,49,48,54,32,48,46,48,57,53,50,44,45,48,46,48,49,48,54,32,48,46,48,50,49,50,44,45,48,46,48,49,48,54,32,48,46,48,
53,50,57,44,48,32,48,46,48,51,49,56,44,48,46,48,49,48,54,32,48,46,48,53,50,57,44,45,48,46,48,49,48,54,32,48,46,52,52,52,53,44,45,48,46,48,49,48,54,32,48,46,55,55,50,53,57,44,48,46,48,57,53,51,32,48,46,
51,50,56,48,56,44,48,46,48,57,53,50,32,48,46,53,57,50,54,54,44,48,46,50,54,52,53,56,32,48,46,48,53,50,57,44,48,46,48,50,49,50,32,48,46,48,56,52,55,44,48,46,48,54,51,53,32,48,46,48,51,49,55,44,48,46,48,
51,49,56,32,48,46,48,55,52,49,44,48,46,48,53,50,57,32,48,46,48,56,52,55,44,48,46,48,54,51,53,32,48,46,49,54,57,51,52,44,48,46,49,52,56,49,54,32,48,46,48,57,53,50,44,48,46,48,56,52,55,32,48,46,49,53,56,
55,53,44,48,46,49,54,57,51,52,32,48,46,49,54,57,51,51,44,48,46,50,50,50,50,53,32,48,46,50,56,53,55,53,44,48,46,52,50,51,51,51,32,48,46,48,55,52,49,44,48,46,49,49,54,52,50,32,48,46,49,49,54,52,49,44,48,
46,50,53,52,32,48,46,48,52,50,51,44,48,46,49,51,55,53,56,32,48,46,48,57,53,50,44,48,46,50,55,53,49,55,32,48,46,48,53,50,57,44,48,46,49,51,55,53,56,32,48,46,48,55,52,49,44,48,46,50,56,53,55,53,32,48,46,
48,51,49,55,44,48,46,49,52,56,49,54,32,48,46,48,55,52,49,44,48,46,50,57,54,51,51,32,48,46,48,49,48,54,44,48,46,48,55,52,49,32,48,46,48,49,48,54,44,48,46,49,52,56,49,55,32,48,46,48,49,48,54,44,48,46,48,
55,52,49,32,48,46,48,50,49,50,44,48,46,49,52,56,49,54,32,122,32,109,32,45,49,46,48,50,54,53,56,44,45,48,46,51,53,57,56,51,32,113,32,48,46,48,51,49,55,44,45,48,46,48,52,50,51,32,48,46,48,53,50,57,44,45,
48,46,49,48,53,56,51,32,48,46,48,50,49,50,44,45,48,46,48,55,52,49,32,48,44,45,48,46,49,52,56,49,55,32,45,48,46,48,54,51,53,44,45,48,46,50,51,50,56,51,32,45,48,46,49,55,57,57,49,44,45,48,46,52,51,51,57,
50,32,45,48,46,49,52,56,49,55,44,45,48,46,50,52,51,52,49,32,45,48,46,51,50,56,48,57,44,45,48,46,52,49,50,55,53,32,45,48,46,49,55,57,57,49,44,45,48,46,49,54,57,51,51,32,45,48,46,52,54,53,54,54,44,45,48,
46,50,56,53,55,53,32,45,48,46,48,55,52,49,44,45,48,46,48,50,49,50,32,45,48,46,49,52,56,49,55,44,45,48,46,48,51,49,56,32,45,48,46,48,55,52,49,44,45,48,46,48,50,49,50,32,45,48,46,49,53,56,55,53,44,45,48,
46,48,52,50,51,32,45,48,46,48,56,52,55,44,45,48,46,48,50,49,50,32,45,48,46,49,55,57,57,50,44,45,48,46,48,50,49,50,32,45,48,46,48,57,53,51,44,48,32,45,48,46,49,55,57,57,49,44,48,32,45,48,46,48,51,49,55,
44,48,46,48,50,49,50,32,45,48,46,48,55,52,49,44,48,46,48,50,49,50,32,45,48,46,48,52,50,51,44,45,48,46,48,49,48,54,32,45,48,46,48,55,52,49,44,48,32,45,48,46,48,56,52,55,44,48,46,48,50,49,50,32,45,48,46,
49,54,57,51,51,44,48,46,48,52,50,51,32,45,48,46,48,55,52,49,44,48,46,48,49,48,54,32,45,48,46,49,52,56,49,55,44,48,46,48,51,49,56,32,45,48,46,51,48,54,57,50,44,48,46,49,50,55,32,45,48,46,52,57,55,52,50,
44,48,46,51,49,55,53,32,45,48,46,49,55,57,57,49,44,48,46,49,57,48,53,32,45,48,46,51,50,56,48,56,44,48,46,52,55,54,50,53,32,45,48,46,48,51,49,56,44,48,46,48,53,50,57,32,45,48,46,48,54,51,53,44,48,46,49,
51,55,53,57,32,45,48,46,48,51,49,56,44,48,46,48,55,52,49,32,45,48,46,48,53,50,57,44,48,46,49,54,57,51,51,32,45,48,46,48,50,49,50,44,48,46,48,57,53,50,32,45,48,46,48,49,48,54,44,48,46,49,55,57,57,50,32,
48,46,48,49,48,54,44,48,46,48,56,52,55,32,48,46,48,55,52,49,44,48,46,49,50,55,32,48,46,48,56,52,55,44,48,46,48,54,51,53,32,48,46,50,56,53,55,53,44,48,46,48,54,51,53,32,48,46,50,49,49,54,55,44,45,48,46,
48,49,48,54,32,48,46,51,53,57,56,52,44,45,48,46,48,49,48,54,32,104,32,49,46,54,48,56,54,54,32,113,32,48,46,49,53,56,55,53,44,48,32,48,46,51,56,49,44,48,46,48,49,48,54,32,48,46,50,50,50,50,53,44,48,32,
48,46,50,57,54,51,52,44,45,48,46,48,56,52,55,32,122,34,32,47,62,10,32,32,32,32,32,32,32,32,60,112,97,116,104,10,32,32,32,32,32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,51,56,54,55,34,10,32,32,32,
32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,111,110,116,45,115,116,121,108,101,58,110,111,114,109,97,108,59,102,111,110,116,45,118,97,114,105,97,110,116,58,110,111,114,109,97,108,59,102,111,
110,116,45,119,101,105,103,104,116,58,110,111,114,109,97,108,59,102,111,110,116,45,115,116,114,101,116,99,104,58,110,111,114,109,97,108,59,102,111,110,116,45,102,97,109,105,108,121,58,39,65,108,116,101,
32,72,97,97,115,32,71,114,111,116,101,115,107,39,59,45,105,110,107,115,99,97,112,101,45,102,111,110,116,45,115,112,101,99,105,102,105,99,97,116,105,111,110,58,39,65,108,116,101,32,72,97,97,115,32,71,114,
111,116,101,115,107,39,59,102,105,108,108,58,35,102,50,102,50,102,50,59,115,116,114,111,107,101,45,119,105,100,116,104,58,48,46,50,54,52,53,56,51,51,50,34,10,32,32,32,32,32,32,32,32,32,32,32,100,61,34,
109,32,49,56,52,46,51,53,49,49,53,44,49,48,52,46,53,49,54,51,56,32,113,32,45,48,46,48,56,52,55,44,45,48,46,48,54,51,53,32,45,48,46,48,57,53,50,44,45,48,46,50,50,50,50,53,32,48,44,45,48,46,49,54,57,51,
51,32,48,44,45,48,46,51,50,56,48,56,32,48,44,45,48,46,48,56,52,55,32,48,44,45,48,46,49,55,57,57,50,32,48,44,45,48,46,48,57,53,50,32,48,46,48,51,49,56,44,45,48,46,49,53,56,55,53,32,48,46,48,52,50,51,44,
45,48,46,48,56,52,55,32,48,46,49,54,57,51,51,44,45,48,46,49,50,55,32,48,46,48,50,49,50,44,45,48,46,48,49,48,54,32,48,46,48,52,50,51,44,48,32,48,46,48,51,49,55,44,48,32,48,46,48,53,50,57,44,45,48,46,48,
49,48,54,32,48,46,49,55,57,57,50,44,48,32,48,46,51,51,56,54,55,44,48,46,48,49,48,54,32,48,46,49,53,56,55,53,44,48,46,48,49,48,54,32,48,46,50,49,49,54,54,44,48,46,49,50,55,32,48,46,48,51,49,55,44,48,46,
48,54,51,53,32,48,46,48,51,49,55,44,48,46,49,53,56,55,53,32,48,44,48,46,48,56,52,55,32,48,44,48,46,49,55,57,57,50,32,48,44,48,46,49,57,48,53,32,45,48,46,48,49,48,54,44,48,46,51,53,57,56,51,32,48,44,48,
46,49,54,57,51,51,32,45,48,46,49,51,55,53,56,44,48,46,50,50,50,50,53,32,45,48,46,48,53,50,57,44,48,46,48,50,49,50,32,45,48,46,49,49,54,52,50,44,48,46,48,50,49,50,32,45,48,46,48,53,50,57,44,48,32,45,48,
46,49,49,54,52,50,44,48,32,45,48,46,49,49,54,52,49,44,48,32,45,48,46,50,50,50,50,53,44,48,32,45,48,46,49,48,53,56,51,44,48,32,45,48,46,49,55,57,57,49,44,45,48,46,48,53,50,57,32,122,32,109,32,48,46,55,
56,51,49,54,44,54,46,48,48,48,55,53,32,113,32,48,44,48,46,49,48,53,56,51,32,48,44,48,46,50,50,50,50,53,32,48,46,48,49,48,54,44,48,46,49,48,53,56,51,32,45,48,46,48,52,50,51,44,48,46,49,54,57,51,51,32,45,
48,46,48,51,49,56,44,48,46,48,54,51,53,32,45,48,46,48,57,53,50,44,48,46,48,56,52,55,32,45,48,46,48,54,51,53,44,48,46,48,49,48,54,32,45,48,46,49,52,56,49,55,44,48,46,48,51,49,56,32,104,32,45,48,46,49,53,
56,55,53,32,113,32,45,48,46,49,51,55,53,56,44,48,32,45,48,46,50,52,51,52,49,44,45,48,46,48,49,48,54,32,45,48,46,48,57,53,51,44,45,48,46,48,50,49,50,32,45,48,46,49,52,56,49,55,44,45,48,46,49,48,53,56,52,
32,45,48,46,48,52,50,51,44,45,48,46,48,54,51,53,32,45,48,46,48,52,50,51,44,45,48,46,49,53,56,55,53,32,48,44,45,48,46,49,48,53,56,51,32,48,44,45,48,46,50,48,49,48,56,32,118,32,45,52,46,52,55,54,55,53,32,
113,32,48,44,45,48,46,49,49,54,52,50,32,48,44,45,48,46,50,50,50,50,53,32,48,44,45,48,46,49,49,54,52,50,32,48,46,48,53,50,57,44,45,48,46,49,55,57,57,50,32,48,46,48,51,49,56,44,45,48,46,48,53,50,57,32,48,
46,48,56,52,55,44,45,48,46,48,54,51,53,32,48,46,48,53,50,57,44,45,48,46,48,49,48,54,32,48,46,49,49,54,52,50,44,45,48,46,48,51,49,56,32,48,46,48,50,49,50,44,48,32,48,46,48,52,50,51,44,48,46,48,49,48,54,
32,48,46,48,51,49,55,44,48,32,48,46,48,53,50,57,44,45,48,46,48,49,48,54,32,48,46,49,51,55,53,56,44,48,32,48,46,50,55,53,49,54,44,48,46,48,49,48,54,32,48,46,49,52,56,49,55,44,48,32,48,46,50,48,49,48,57,
44,48,46,48,56,52,55,32,48,46,48,53,50,57,44,48,46,48,54,51,53,32,48,46,48,53,50,57,44,48,46,49,54,57,51,52,32,48,44,48,46,48,57,53,51,32,48,44,48,46,50,48,49,48,56,32,122,34,32,47,62,10,32,32,32,32,32,
32,32,32,60,112,97,116,104,10,32,32,32,32,32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,51,56,54,57,34,10,32,32,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,111,110,116,45,115,116,121,
108,101,58,110,111,114,109,97,108,59,102,111,110,116,45,118,97,114,105,97,110,116,58,110,111,114,109,97,108,59,102,111,110,116,45,119,101,105,103,104,116,58,110,111,114,109,97,108,59,102,111,110,116,45,
115,116,114,101,116,99,104,58,110,111,114,109,97,108,59,102,111,110,116,45,102,97,109,105,108,121,58,39,65,108,116,101,32,72,97,97,115,32,71,114,111,116,101,115,107,39,59,45,105,110,107,115,99,97,112,
101,45,102,111,110,116,45,115,112,101,99,105,102,105,99,97,116,105,111,110,58,39,65,108,116,101,32,72,97,97,115,32,71,114,111,116,101,115,107,39,59,102,105,108,108,58,35,102,50,102,50,102,50,59,115,116,
114,111,107,101,45,119,105,100,116,104,58,48,46,50,54,52,53,56,51,51,50,34,10,32,32,32,32,32,32,32,32,32,32,32,100,61,34,109,32,49,56,57,46,49,49,48,48,49,44,49,48,53,46,52,54,56,56,56,32,113,32,48,46,
54,54,54,55,53,44,45,48,46,48,50,49,50,32,49,46,48,54,56,57,49,44,48,46,49,55,57,57,50,32,48,46,52,48,50,49,55,44,48,46,50,48,49,48,56,32,48,46,54,52,53,53,57,44,48,46,54,48,51,50,53,32,48,46,48,54,51,
53,44,48,46,49,48,53,56,51,32,48,46,48,57,53,50,44,48,46,50,51,50,56,51,32,48,46,48,52,50,51,44,48,46,49,49,54,52,50,32,48,46,48,55,52,49,44,48,46,50,52,51,52,50,32,48,46,48,53,50,57,44,48,46,50,48,49,
48,56,32,48,46,48,54,51,53,44,48,46,52,51,51,57,49,32,48,46,48,49,48,54,44,48,46,50,51,50,56,52,32,48,46,48,49,48,54,44,48,46,52,55,54,50,53,32,118,32,49,46,49,51,50,52,50,32,49,46,54,53,49,32,113,32,
48,44,48,46,49,55,57,57,50,32,45,48,46,48,49,48,54,44,48,46,51,53,57,56,51,32,48,44,48,46,49,55,57,57,50,32,45,48,46,49,51,55,53,56,44,48,46,50,51,50,56,52,32,45,48,46,48,53,50,57,44,48,46,48,50,49,50,
32,45,48,46,49,49,54,52,50,44,48,46,48,50,49,50,32,45,48,46,48,53,50,57,44,48,46,48,49,48,54,32,45,48,46,49,49,54,52,50,44,48,46,48,49,48,54,32,45,48,46,49,51,55,53,56,44,48,32,45,48,46,50,54,52,53,56,
44,45,48,46,48,49,48,54,32,45,48,46,49,50,55,44,48,32,45,48,46,49,55,57,57,50,44,45,48,46,48,56,52,55,32,45,48,46,48,53,50,57,44,45,48,46,48,55,52,49,32,45,48,46,48,53,50,57,44,45,48,46,49,57,48,53,32,
48,44,45,48,46,49,49,54,52,50,32,48,44,45,48,46,50,51,50,56,52,32,118,32,45,48,46,57,49,48,49,54,32,45,49,46,55,50,53,48,57,32,113,32,48,44,45,48,46,50,56,53,55,53,32,45,48,46,48,49,48,54,44,45,48,46,
53,54,48,57,49,32,45,48,46,48,49,48,54,44,45,48,46,50,56,53,55,53,32,45,48,46,48,57,53,50,44,45,48,46,52,56,54,56,52,32,45,48,46,48,56,52,55,44,45,48,46,50,49,49,54,54,32,45,48,46,50,53,52,44,45,48,46,
51,51,56,54,54,32,45,48,46,49,54,57,51,51,44,45,48,46,49,51,55,53,57,32,45,48,46,51,57,49,53,56,44,45,48,46,50,49,49,54,55,32,45,48,46,48,54,51,53,44,45,48,46,48,50,49,50,32,45,48,46,49,51,55,53,56,44,
45,48,46,48,50,49,50,32,45,48,46,48,54,51,53,44,45,48,46,48,49,48,54,32,45,48,46,49,50,55,44,45,48,46,48,51,49,56,32,45,48,46,48,53,50,57,44,45,48,46,48,49,48,54,32,45,48,46,49,54,57,51,52,44,48,32,45,
48,46,49,48,53,56,51,44,48,32,45,48,46,49,52,56,49,54,44,48,46,48,49,48,54,32,45,48,46,48,52,50,51,44,48,46,48,50,49,50,32,45,48,46,48,55,52,49,44,48,46,48,50,49,50,32,45,48,46,48,51,49,56,44,45,48,46,
48,49,48,54,32,45,48,46,48,55,52,49,44,48,32,45,48,46,49,51,55,53,56,44,48,46,48,51,49,55,32,45,48,46,50,54,52,53,56,44,48,46,48,55,52,49,32,45,48,46,49,49,54,52,50,44,48,46,48,51,49,56,32,45,48,46,50,
50,50,50,53,44,48,46,48,57,53,50,32,45,48,46,48,52,50,51,44,48,46,48,50,49,50,32,45,48,46,48,55,52,49,44,48,46,48,52,50,51,32,45,48,46,48,51,49,55,44,48,46,48,50,49,50,32,45,48,46,48,55,52,49,44,48,46,
48,52,50,51,32,45,48,46,51,55,48,52,50,44,48,46,50,54,52,53,57,32,45,48,46,53,51,57,55,53,44,48,46,55,57,51,55,53,32,45,48,46,48,51,49,55,44,48,46,48,56,52,55,32,45,48,46,48,52,50,51,44,48,46,49,55,57,
57,50,32,45,48,46,48,49,48,54,44,48,46,48,56,52,55,32,45,48,46,48,51,49,55,44,48,46,49,54,57,51,51,32,45,48,46,48,50,49,50,44,48,46,48,53,50,57,32,45,48,46,48,49,48,54,44,48,46,49,48,53,56,52,32,48,46,
48,49,48,54,44,48,46,48,52,50,51,32,45,48,46,48,49,48,54,44,48,46,48,56,52,55,32,118,32,48,46,50,50,50,50,53,32,113,32,45,48,46,48,49,48,54,44,48,46,48,51,49,56,32,45,48,46,48,49,48,54,44,48,46,48,55,
52,49,32,48,46,48,49,48,54,44,48,46,48,52,50,51,32,48,46,48,49,48,54,44,48,46,48,57,53,51,32,118,32,48,46,51,49,55,53,32,49,46,48,49,54,32,48,46,57,51,49,51,51,32,113,32,48,44,48,46,49,49,54,52,50,32,
48,44,48,46,50,52,51,52,50,32,48,44,48,46,49,49,54,52,49,32,45,48,46,48,53,50,57,44,48,46,49,57,48,53,32,45,48,46,48,53,50,57,44,48,46,48,56,52,55,32,45,48,46,49,57,48,53,44,48,46,48,56,52,55,32,45,48,
46,49,50,55,44,48,46,48,49,48,54,32,45,48,46,50,54,52,53,56,44,48,46,48,49,48,54,32,45,48,46,48,56,52,55,44,48,32,45,48,46,49,53,56,55,53,44,45,48,46,48,49,48,54,32,45,48,46,48,55,52,49,44,45,48,46,48,
49,48,54,32,45,48,46,49,50,55,44,45,48,46,48,53,50,57,32,45,48,46,48,56,52,55,44,45,48,46,48,53,50,57,32,45,48,46,48,56,52,55,44,45,48,46,50,48,49,48,57,32,48,44,45,48,46,49,52,56,49,54,32,48,44,45,48,
46,50,56,53,55,53,32,118,32,45,49,46,50,52,56,56,51,32,45,50,46,53,54,49,49,55,32,45,48,46,54,52,53,53,56,32,113,32,48,44,45,48,46,48,57,53,50,32,48,44,45,48,46,49,57,48,53,32,48,44,45,48,46,48,57,53,
50,32,48,46,48,52,50,51,44,45,48,46,49,52,56,49,55,32,48,46,48,51,49,55,44,45,48,46,48,54,51,53,32,48,46,49,48,53,56,51,44,45,48,46,48,56,52,55,32,48,46,48,52,50,51,44,45,48,46,48,49,48,54,32,48,46,49,
49,54,52,50,44,45,48,46,48,50,49,50,32,48,46,48,56,52,55,44,45,48,46,48,49,48,54,32,48,46,49,54,57,51,51,44,45,48,46,48,49,48,54,32,48,46,48,56,52,55,44,48,32,48,46,49,53,56,55,53,44,48,46,48,49,48,54,
32,48,46,48,56,52,55,44,48,46,48,49,48,54,32,48,46,49,49,54,52,50,44,48,46,48,51,49,56,32,48,46,49,49,54,52,49,44,48,46,48,55,52,49,32,48,46,49,48,53,56,51,44,48,46,49,57,48,53,32,45,48,46,48,49,48,54,
44,48,46,49,48,53,56,51,32,48,46,48,51,49,56,44,48,46,50,52,51,52,50,32,48,46,48,50,49,50,44,48,46,48,54,51,53,32,48,46,48,56,52,55,44,48,46,49,48,53,56,51,32,48,46,49,50,55,44,48,46,48,49,48,54,32,48,
46,50,48,49,48,56,44,45,48,46,48,57,53,50,32,48,46,48,56,52,55,44,45,48,46,49,48,53,56,51,32,48,46,49,54,57,51,51,44,45,48,46,49,54,57,51,51,32,48,46,49,52,56,49,55,44,45,48,46,49,48,53,56,52,32,48,46,
50,57,54,51,52,44,45,48,46,49,57,48,53,32,48,46,49,53,56,55,53,44,45,48,46,48,57,53,50,32,48,46,51,53,57,56,51,44,45,48,46,49,53,56,55,53,32,48,46,48,56,52,55,44,45,48,46,48,51,49,56,32,48,46,49,55,57,
57,50,44,45,48,46,48,52,50,51,32,48,46,49,48,53,56,51,44,45,48,46,48,49,48,54,32,48,46,50,48,49,48,56,44,45,48,46,48,51,49,56,32,48,46,48,53,50,57,44,45,48,46,48,49,48,54,32,48,46,49,48,53,56,51,44,45,
48,46,48,49,48,54,32,48,46,48,53,50,57,44,48,32,48,46,49,48,53,56,52,44,48,32,122,34,32,47,62,10,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,32,32,60,103,10,32,32,32,32,32,32,32,32,32,105,100,61,34,
116,101,120,116,51,56,48,55,34,10,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,111,110,116,45,115,116,121,108,101,58,110,111,114,109,97,108,59,102,111,110,116,45,118,97,114,105,97,110,116,
58,110,111,114,109,97,108,59,102,111,110,116,45,119,101,105,103,104,116,58,52,48,48,59,102,111,110,116,45,115,105,122,101,58,49,46,48,50,54,53,54,52,51,54,112,120,59,108,105,110,101,45,104,101,105,103,
104,116,58,49,50,53,37,59,102,111,110,116,45,102,97,109,105,108,121,58,67,97,108,105,98,114,105,59,116,101,120,116,45,97,108,105,103,110,58,115,116,97,114,116,59,108,101,116,116,101,114,45,115,112,97,
99,105,110,103,58,48,112,120,59,119,111,114,100,45,115,112,97,99,105,110,103,58,48,112,120,59,116,101,120,116,45,97,110,99,104,111,114,58,115,116,97,114,116,59,102,105,108,108,58,35,102,50,102,50,102,
50,59,102,105,108,108,45,111,112,97,99,105,116,121,58,49,59,115,116,114,111,107,101,58,110,111,110,101,59,115,116,114,111,107,101,45,119,105,100,116,104,58,48,46,48,54,57,56,57,51,55,53,34,10,32,32,32,
32,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,115,99,97,108,101,40,48,46,57,57,55,56,54,52,55,50,44,49,46,48,48,50,49,51,57,57,41,34,10,32,32,32,32,32,32,32,32,32,97,114,105,97,45,108,97,
98,101,108,61,34,194,169,32,34,62,10,32,32,32,32,32,32,32,32,60,112,97,116,104,10,32,32,32,32,32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,51,56,50,48,34,10,32,32,32,32,32,32,32,32,32,32,32,115,116,
121,108,101,61,34,102,111,110,116,45,115,116,121,108,101,58,110,111,114,109,97,108,59,102,111,110,116,45,118,97,114,105,97,110,116,58,110,111,114,109,97,108,59,102,111,110,116,45,119,101,105,103,104,116,
58,52,48,48,59,102,111,110,116,45,115,105,122,101,58,49,51,46,52,49,57,53,57,57,53,51,112,120,59,102,111,110,116,45,102,97,109,105,108,121,58,67,97,108,105,98,114,105,59,102,105,108,108,58,35,102,50,102,
50,102,50,59,115,116,114,111,107,101,45,119,105,100,116,104,58,48,46,48,54,57,56,57,51,55,53,34,10,32,32,32,32,32,32,32,32,32,32,32,100,61,34,109,32,52,48,46,55,56,54,50,54,54,44,49,49,49,46,50,51,52,
52,57,32,113,32,49,46,48,49,53,54,52,52,44,48,32,49,46,54,57,48,53,53,53,44,45,48,46,51,48,49,52,50,32,48,46,54,55,52,57,49,50,44,45,48,46,51,48,55,57,55,32,49,46,48,56,49,49,54,57,44,45,48,46,56,52,53,
50,55,32,48,46,52,48,54,50,53,55,44,45,48,46,53,51,55,51,49,32,48,46,53,55,54,54,50,51,44,45,49,46,50,54,52,54,52,32,48,46,49,55,54,57,49,57,44,45,48,46,55,51,51,56,57,32,48,46,49,55,54,57,49,57,44,45,
49,46,53,55,50,54,49,32,48,44,45,48,46,57,48,52,50,53,32,45,48,46,49,57,48,48,50,52,44,45,49,46,54,48,53,51,56,32,45,48,46,49,57,48,48,50,51,44,45,48,46,55,48,55,54,55,32,45,48,46,54,50,50,52,57,49,44,
45,49,46,49,57,50,53,54,32,45,48,46,52,51,50,52,54,55,44,45,48,46,52,56,52,56,57,32,45,49,46,49,50,55,48,51,55,44,45,48,46,55,52,48,52,51,32,45,48,46,54,56,56,48,49,54,44,45,48,46,50,53,53,53,53,32,45,
49,46,54,57,48,53,53,53,44,45,48,46,50,53,53,53,53,32,45,48,46,56,57,49,49,52,53,44,48,32,45,49,46,53,53,50,57,53,49,44,48,46,50,53,53,53,53,32,45,48,46,54,54,49,56,48,55,44,48,46,50,52,56,57,57,32,45,
49,46,49,48,48,56,50,55,44,48,46,55,52,54,57,57,32,45,48,46,52,51,57,48,50,44,48,46,52,57,55,57,57,32,45,48,46,54,53,53,50,53,52,44,49,46,50,52,52,57,56,32,45,48,46,50,48,57,54,56,49,44,48,46,55,52,54,
57,57,32,45,48,46,50,48,57,54,56,49,44,49,46,55,52,50,57,55,32,48,44,48,46,57,51,55,48,50,32,48,46,50,48,51,49,50,57,44,49,46,54,53,49,50,52,32,48,46,50,48,51,49,50,57,44,48,46,55,48,55,54,56,32,48,46,
54,51,53,53,57,54,44,49,46,49,56,54,48,49,32,48,46,52,51,57,48,50,44,48,46,52,55,49,55,57,32,49,46,49,50,55,48,51,55,44,48,46,55,49,52,50,51,32,48,46,54,56,56,48,49,54,44,48,46,50,51,53,56,57,32,49,46,
54,53,55,55,57,50,44,48,46,50,51,53,56,57,32,122,32,109,32,52,46,52,48,51,51,48,54,44,45,52,46,48,53,54,48,50,32,113,32,48,44,49,46,50,52,52,57,56,32,45,48,46,50,57,52,56,54,52,44,50,46,49,53,53,55,56,
32,45,48,46,50,56,56,51,49,50,44,48,46,57,48,52,50,53,32,45,48,46,56,53,56,51,56,51,44,49,46,53,48,48,53,52,32,45,48,46,53,55,48,48,55,49,44,48,46,53,56,57,55,50,32,45,49,46,52,48,50,50,52,51,44,48,46,
56,55,56,48,52,32,45,48,46,56,51,50,49,55,50,44,48,46,50,56,49,55,53,32,45,49,46,57,49,51,51,52,49,44,48,46,50,56,49,55,53,32,45,49,46,49,48,55,51,55,57,44,48,32,45,49,46,57,51,57,53,53,50,44,45,48,46,
50,56,49,55,53,32,45,48,46,56,51,50,49,55,50,44,45,48,46,50,56,49,55,54,32,45,49,46,51,56,57,49,51,56,44,45,48,46,56,51,56,55,51,32,45,48,46,53,53,48,52,49,51,44,45,48,46,53,54,51,53,50,32,45,48,46,56,
51,50,49,55,50,44,45,49,46,52,48,50,50,52,32,45,48,46,50,55,53,50,48,55,44,45,48,46,56,51,56,55,51,32,45,48,46,50,55,53,50,48,55,44,45,49,46,57,53,57,50,49,32,48,44,45,49,46,50,49,56,55,55,32,48,46,50,
56,49,55,53,57,44,45,50,46,49,50,51,48,50,32,48,46,50,56,56,51,49,50,44,45,48,46,57,48,52,50,53,32,48,46,56,52,53,50,55,56,44,45,49,46,53,48,48,53,52,32,48,46,53,54,51,53,49,56,44,45,48,46,53,57,54,50,
56,32,49,46,51,57,53,54,57,44,45,48,46,56,57,49,49,52,32,48,46,56,51,56,55,50,53,44,45,48,46,51,48,49,52,50,32,49,46,57,51,57,53,53,50,44,45,48,46,51,48,49,52,50,32,49,46,49,52,54,54,57,52,44,48,32,49,
46,57,55,56,56,54,55,44,48,46,51,48,49,52,50,32,48,46,56,51,50,49,55,50,44,48,46,50,57,52,56,54,32,49,46,51,56,50,53,56,53,44,48,46,56,54,52,57,51,32,48,46,53,53,48,52,49,52,44,48,46,53,55,48,48,56,32,
48,46,56,49,50,53,49,53,44,49,46,52,48,56,56,32,48,46,50,54,56,54,53,52,44,48,46,56,51,50,49,55,32,48,46,50,54,56,54,53,52,44,49,46,57,48,54,55,57,32,122,32,109,32,45,50,46,54,51,52,49,50,44,50,46,48,
49,56,49,56,32,113,32,48,44,48,46,48,57,56,51,32,45,48,46,48,48,54,54,44,48,46,49,55,48,51,55,32,45,48,46,48,48,54,54,44,48,46,48,55,50,49,32,45,48,46,48,50,54,50,49,44,48,46,49,51,49,48,53,32,45,48,46,
48,49,51,49,49,44,48,46,48,53,50,52,32,45,48,46,48,51,50,55,54,44,48,46,48,57,49,55,32,45,48,46,48,49,57,54,54,44,48,46,48,51,57,51,32,45,48,46,48,56,53,49,56,44,48,46,48,57,56,51,32,45,48,46,48,53,56,
57,55,44,48,46,48,53,57,32,45,48,46,50,49,54,50,51,52,44,48,46,49,54,51,56,49,32,45,48,46,49,53,55,50,54,49,44,48,46,49,48,52,56,53,32,45,48,46,51,54,54,57,52,50,44,48,46,49,57,54,53,56,32,45,48,46,50,
48,57,54,56,49,44,48,46,48,56,53,50,32,45,48,46,52,55,49,55,56,51,44,48,46,49,52,52,49,54,32,45,48,46,50,54,50,49,48,49,44,48,46,48,53,50,52,32,45,48,46,53,53,54,57,54,53,44,48,46,48,53,50,52,32,45,48,
46,53,56,51,49,55,54,44,48,32,45,49,46,48,50,56,55,52,57,44,45,48,46,49,56,51,52,55,32,45,48,46,52,52,53,53,55,51,44,45,48,46,49,56,51,52,56,32,45,48,46,55,52,54,57,56,57,44,45,48,46,53,52,51,56,54,32,
45,48,46,51,48,49,52,49,55,44,45,48,46,51,54,54,57,53,32,45,48,46,52,53,56,54,55,56,44,45,48,46,56,57,49,49,53,32,45,48,46,49,53,48,55,48,57,44,45,48,46,53,51,48,55,54,32,45,48,46,49,53,48,55,48,57,44,
45,49,46,50,50,53,51,50,32,48,44,45,48,46,55,54,54,54,53,32,48,46,49,56,51,52,55,50,44,45,49,46,51,50,51,54,50,32,48,46,49,57,48,48,50,51,44,45,48,46,53,53,54,57,54,32,48,46,53,49,55,54,53,44,45,48,46,
57,49,48,56,32,48,46,51,51,52,49,56,44,45,48,46,51,54,48,51,57,32,48,46,55,56,54,51,48,53,44,45,48,46,53,51,48,55,54,32,48,46,52,53,50,49,50,53,44,45,48,46,49,55,54,57,49,32,48,46,57,55,54,51,50,56,44,
45,48,46,49,55,54,57,49,32,48,46,50,53,53,53,52,57,44,48,32,48,46,52,56,52,56,56,56,44,48,46,48,53,50,52,32,48,46,50,51,53,56,57,49,44,48,46,48,52,53,57,32,48,46,52,51,50,52,54,56,44,48,46,49,50,52,52,
57,32,48,46,50,48,51,49,50,56,44,48,46,48,55,56,54,32,48,46,51,54,54,57,52,50,44,48,46,49,56,51,52,55,32,48,46,49,54,51,56,49,51,44,48,46,49,48,52,56,53,32,48,46,50,51,53,56,57,49,44,48,46,49,55,54,57,
50,32,48,46,48,55,56,54,51,44,48,46,48,55,50,49,32,48,46,48,57,56,50,57,44,48,46,49,49,49,52,32,48,46,48,50,54,50,49,44,48,46,48,51,57,51,32,48,46,48,51,57,51,50,44,48,46,48,57,49,55,32,48,46,48,49,57,
54,54,44,48,46,48,53,50,52,32,48,46,48,50,54,50,49,44,48,46,49,50,52,53,32,48,46,48,48,54,54,44,48,46,48,55,50,49,32,48,46,48,48,54,54,44,48,46,49,53,55,50,54,32,48,44,48,46,48,56,53,50,32,45,48,46,48,
49,51,49,49,44,48,46,49,53,55,50,54,32,45,48,46,48,49,51,49,44,48,46,48,55,50,49,32,45,48,46,48,51,50,55,54,44,48,46,49,51,49,48,53,32,45,48,46,48,49,57,54,54,44,48,46,48,53,50,52,32,45,48,46,48,53,56,
57,55,44,48,46,48,56,53,50,32,45,48,46,48,51,50,55,54,44,48,46,48,50,54,50,32,45,48,46,48,56,53,49,56,44,48,46,48,50,54,50,32,45,48,46,49,49,49,51,57,51,44,48,32,45,48,46,50,50,57,51,51,56,44,45,48,46,
48,57,49,55,32,45,48,46,49,49,49,51,57,52,44,45,48,46,48,57,56,51,32,45,48,46,50,56,49,55,54,44,45,48,46,50,48,57,54,56,32,45,48,46,49,54,51,56,49,51,44,45,48,46,49,49,55,57,53,32,45,48,46,52,48,54,50,
53,55,44,45,48,46,50,48,57,54,56,32,45,48,46,50,51,53,56,57,49,44,45,48,46,48,57,56,51,32,45,48,46,53,57,54,50,56,49,44,45,48,46,48,57,56,51,32,45,48,46,55,48,49,49,50,50,44,48,32,45,49,46,48,56,49,49,
54,57,44,48,46,53,51,48,55,54,32,45,48,46,51,55,51,52,57,53,44,48,46,53,51,48,55,53,32,45,48,46,51,55,51,52,57,53,44,49,46,53,52,54,51,57,32,48,44,48,46,53,51,48,55,54,32,48,46,48,57,56,50,57,44,48,46,
57,49,55,51,54,32,48,46,48,57,56,50,57,44,48,46,51,56,54,54,32,48,46,50,56,56,51,49,50,44,48,46,54,52,56,55,32,48,46,49,57,54,53,55,54,44,48,46,50,53,53,53,53,32,48,46,52,55,56,51,51,54,44,48,46,51,56,
48,48,53,32,48,46,50,56,49,55,53,57,44,48,46,49,50,52,53,32,48,46,54,52,56,55,48,49,44,48,46,49,50,52,53,32,48,46,51,51,52,49,55,57,44,48,32,48,46,53,55,54,54,50,51,44,45,48,46,48,57,56,51,32,48,46,50,
52,50,52,52,52,44,45,48,46,49,48,52,56,52,32,48,46,52,49,50,56,49,44,45,48,46,50,50,57,51,52,32,48,46,49,55,54,57,49,57,44,45,48,46,49,50,52,53,32,48,46,50,57,52,56,54,52,44,45,48,46,50,50,50,55,57,32,
48,46,49,49,55,57,52,54,44,45,48,46,49,48,52,56,52,32,48,46,50,48,57,54,56,50,44,45,48,46,49,48,52,56,52,32,48,46,48,51,50,55,54,44,48,32,48,46,48,53,56,57,55,44,48,46,48,49,57,55,32,48,46,48,51,50,55,
54,44,48,46,48,49,57,55,32,48,46,48,52,53,56,55,44,48,46,48,55,50,49,32,48,46,48,49,57,54,54,44,48,46,48,53,50,52,32,48,46,48,50,54,50,49,44,48,46,49,51,55,54,32,48,46,48,49,51,49,49,44,48,46,48,56,53,
50,32,48,46,48,49,51,49,49,44,48,46,50,48,51,49,51,32,122,34,32,47,62,10,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,60,47,103,62,10,32,32,60,47,103,62,10,60,47,115,118,103,62,10,0,0};

const char* VASTVaporizerComponent::copyright_svg = (const char*) resource_VASTVaporizerComponent_copyright_svg;
const int VASTVaporizerComponent::copyright_svgSize = 78864;

// JUCER_RESOURCE: vaporizer2_svg, 14741, "../../Resources/vaporizer2.svg"
static const unsigned char resource_VASTVaporizerComponent_vaporizer2_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,70,45,56,
34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,33,45,45,32,67,114,101,97,116,101,100,32,119,105,116,104,32,73,110,107,115,99,97,112,101,32,40,104,116,116,112,58,47,47,119,119,
119,46,105,110,107,115,99,97,112,101,46,111,114,103,47,41,32,45,45,62,10,10,60,115,118,103,10,32,32,32,120,109,108,110,115,58,100,99,61,34,104,116,116,112,58,47,47,112,117,114,108,46,111,114,103,47,100,
99,47,101,108,101,109,101,110,116,115,47,49,46,49,47,34,10,32,32,32,120,109,108,110,115,58,99,99,61,34,104,116,116,112,58,47,47,99,114,101,97,116,105,118,101,99,111,109,109,111,110,115,46,111,114,103,
47,110,115,35,34,10,32,32,32,120,109,108,110,115,58,114,100,102,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,48,50,47,50,50,45,114,100,102,45,115,121,110,116,97,
120,45,110,115,35,34,10,32,32,32,120,109,108,110,115,58,115,118,103,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,118,103,34,10,32,32,32,120,109,108,110,115,
61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,118,103,34,10,32,32,32,120,109,108,110,115,58,115,111,100,105,112,111,100,105,61,34,104,116,116,112,58,47,47,115,
111,100,105,112,111,100,105,46,115,111,117,114,99,101,102,111,114,103,101,46,110,101,116,47,68,84,68,47,115,111,100,105,112,111,100,105,45,48,46,100,116,100,34,10,32,32,32,120,109,108,110,115,58,105,110,
107,115,99,97,112,101,61,34,104,116,116,112,58,47,47,119,119,119,46,105,110,107,115,99,97,112,101,46,111,114,103,47,110,97,109,101,115,112,97,99,101,115,47,105,110,107,115,99,97,112,101,34,10,32,32,32,
119,105,100,116,104,61,34,50,49,48,109,109,34,10,32,32,32,104,101,105,103,104,116,61,34,50,57,55,109,109,34,10,32,32,32,118,105,101,119,66,111,120,61,34,48,32,48,32,50,49,48,32,50,57,55,34,10,32,32,32,
118,101,114,115,105,111,110,61,34,49,46,49,34,10,32,32,32,105,100,61,34,115,118,103,51,57,48,55,34,10,32,32,32,105,110,107,115,99,97,112,101,58,118,101,114,115,105,111,110,61,34,48,46,57,50,46,51,32,40,
50,52,48,53,53,52,54,44,32,50,48,49,56,45,48,51,45,49,49,41,34,10,32,32,32,115,111,100,105,112,111,100,105,58,100,111,99,110,97,109,101,61,34,118,97,112,111,114,105,122,101,114,50,46,115,118,103,34,62,
10,32,32,60,100,101,102,115,10,32,32,32,32,32,105,100,61,34,100,101,102,115,51,57,48,49,34,32,47,62,10,32,32,60,115,111,100,105,112,111,100,105,58,110,97,109,101,100,118,105,101,119,10,32,32,32,32,32,
105,100,61,34,98,97,115,101,34,10,32,32,32,32,32,112,97,103,101,99,111,108,111,114,61,34,35,102,102,102,102,102,102,34,10,32,32,32,32,32,98,111,114,100,101,114,99,111,108,111,114,61,34,35,54,54,54,54,
54,54,34,10,32,32,32,32,32,98,111,114,100,101,114,111,112,97,99,105,116,121,61,34,49,46,48,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,112,97,103,101,111,112,97,99,105,116,121,61,34,48,46,48,
34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,112,97,103,101,115,104,97,100,111,119,61,34,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,122,111,111,109,61,34,55,46,57,49,57,53,57,54,
34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,120,61,34,52,49,49,46,50,55,57,56,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,121,61,34,54,53,52,46,52,57,55,51,49,34,10,32,32,
32,32,32,105,110,107,115,99,97,112,101,58,100,111,99,117,109,101,110,116,45,117,110,105,116,115,61,34,109,109,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,117,114,114,101,110,116,45,108,97,
121,101,114,61,34,108,97,121,101,114,49,34,10,32,32,32,32,32,115,104,111,119,103,114,105,100,61,34,102,97,108,115,101,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,119,
105,100,116,104,61,34,54,53,56,51,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,104,101,105,103,104,116,61,34,51,53,49,54,34,10,32,32,32,32,32,105,110,107,115,99,97,
112,101,58,119,105,110,100,111,119,45,120,61,34,45,49,56,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,121,61,34,45,49,56,34,10,32,32,32,32,32,105,110,107,115,99,97,
112,101,58,119,105,110,100,111,119,45,109,97,120,105,109,105,122,101,100,61,34,49,34,32,47,62,10,32,32,60,109,101,116,97,100,97,116,97,10,32,32,32,32,32,105,100,61,34,109,101,116,97,100,97,116,97,51,57,
48,52,34,62,10,32,32,32,32,60,114,100,102,58,82,68,70,62,10,32,32,32,32,32,32,60,99,99,58,87,111,114,107,10,32,32,32,32,32,32,32,32,32,114,100,102,58,97,98,111,117,116,61,34,34,62,10,32,32,32,32,32,32,
32,32,60,100,99,58,102,111,114,109,97,116,62,105,109,97,103,101,47,115,118,103,43,120,109,108,60,47,100,99,58,102,111,114,109,97,116,62,10,32,32,32,32,32,32,32,32,60,100,99,58,116,121,112,101,10,32,32,
32,32,32,32,32,32,32,32,32,114,100,102,58,114,101,115,111,117,114,99,101,61,34,104,116,116,112,58,47,47,112,117,114,108,46,111,114,103,47,100,99,47,100,99,109,105,116,121,112,101,47,83,116,105,108,108,
73,109,97,103,101,34,32,47,62,10,32,32,32,32,32,32,32,32,60,100,99,58,116,105,116,108,101,32,47,62,10,32,32,32,32,32,32,60,47,99,99,58,87,111,114,107,62,10,32,32,32,32,60,47,114,100,102,58,82,68,70,62,
10,32,32,60,47,109,101,116,97,100,97,116,97,62,10,32,32,60,103,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,108,97,98,101,108,61,34,69,98,101,110,101,32,49,34,10,32,32,32,32,32,105,110,107,115,99,
97,112,101,58,103,114,111,117,112,109,111,100,101,61,34,108,97,121,101,114,34,10,32,32,32,32,32,105,100,61,34,108,97,121,101,114,49,34,62,10,32,32,32,32,60,103,10,32,32,32,32,32,32,32,97,114,105,97,45,
108,97,98,101,108,61,34,86,65,80,79,82,73,90,69,82,50,34,10,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,111,110,116,45,115,116,121,108,101,58,110,111,114,109,97,108,59,102,111,110,116,45,119,101,
105,103,104,116,58,110,111,114,109,97,108,59,102,111,110,116,45,115,105,122,101,58,49,48,46,53,56,51,51,51,51,48,50,112,120,59,108,105,110,101,45,104,101,105,103,104,116,58,49,46,50,53,59,102,111,110,
116,45,102,97,109,105,108,121,58,115,97,110,115,45,115,101,114,105,102,59,108,101,116,116,101,114,45,115,112,97,99,105,110,103,58,48,112,120,59,119,111,114,100,45,115,112,97,99,105,110,103,58,48,112,120,
59,102,105,108,108,58,35,54,50,97,52,56,55,59,102,105,108,108,45,111,112,97,99,105,116,121,58,49,59,115,116,114,111,107,101,58,110,111,110,101,59,115,116,114,111,107,101,45,119,105,100,116,104,58,48,46,
50,54,52,53,56,51,51,50,34,10,32,32,32,32,32,32,32,105,100,61,34,116,101,120,116,56,51,53,34,62,10,32,32,32,32,32,32,60,112,97,116,104,10,32,32,32,32,32,32,32,32,32,100,61,34,109,32,53,56,46,50,54,52,
57,57,44,49,51,53,46,52,55,51,48,50,32,113,32,45,48,46,48,56,52,54,55,44,45,48,46,49,55,57,57,49,32,45,48,46,48,56,52,54,55,44,45,48,46,51,55,48,52,49,32,48,44,45,48,46,52,48,50,49,55,32,48,46,51,52,57,
50,53,44,45,48,46,54,56,55,57,50,32,48,46,51,52,57,50,53,44,45,48,46,50,56,53,55,53,32,48,46,55,55,50,53,56,51,44,45,48,46,50,56,53,55,53,32,48,46,53,54,48,57,49,55,44,48,32,48,46,55,57,51,55,53,44,48,
46,52,56,54,56,51,32,108,32,50,46,51,49,55,55,53,44,52,46,56,52,55,49,55,32,113,32,48,46,50,49,49,54,54,55,44,48,46,52,52,52,53,32,48,46,53,55,49,53,44,48,46,52,52,52,53,32,48,46,51,51,56,54,54,55,44,
48,32,48,46,53,49,56,53,56,51,44,45,48,46,51,57,49,53,56,32,108,32,50,46,51,50,56,51,51,52,44,45,53,46,49,57,54,52,50,32,113,32,45,48,46,48,52,50,51,51,44,48,46,49,54,57,51,51,32,45,48,46,48,52,50,51,
51,44,48,46,51,50,56,48,56,32,48,44,48,46,54,55,55,51,52,32,48,46,54,49,51,56,51,52,44,48,46,57,53,50,53,32,48,46,50,49,49,54,54,54,44,48,46,48,57,53,50,32,48,46,52,50,51,51,51,51,44,48,46,48,57,53,50,
32,48,46,52,56,54,56,51,51,44,48,32,48,46,56,53,55,50,53,44,45,48,46,53,48,56,32,108,32,45,48,46,54,51,53,44,49,46,51,57,55,32,45,50,46,50,55,53,52,49,55,44,52,46,57,54,51,53,57,32,113,32,45,48,46,50,
51,50,56,51,51,44,48,46,53,48,56,32,45,48,46,55,48,57,48,56,51,44,48,46,55,56,51,49,54,32,45,48,46,52,55,54,50,53,44,48,46,50,54,52,53,57,32,45,49,46,48,51,55,49,54,55,44,48,46,50,54,52,53,57,32,45,48,
46,53,55,49,53,44,48,32,45,49,46,48,53,56,51,51,51,44,45,48,46,50,55,53,49,55,32,45,48,46,53,50,57,49,54,54,44,45,48,46,51,48,54,57,50,32,45,48,46,55,57,51,55,53,44,45,48,46,56,54,55,56,51,32,122,34,10,
32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,111,110,116,45,115,116,121,108,101,58,110,111,114,109,97,108,59,102,111,110,116,45,118,97,114,105,97,110,116,58,110,111,114,109,97,108,59,102,111,
110,116,45,119,101,105,103,104,116,58,110,111,114,109,97,108,59,102,111,110,116,45,115,116,114,101,116,99,104,58,110,111,114,109,97,108,59,102,111,110,116,45,102,97,109,105,108,121,58,39,49,51,95,82,111,
115,104,105,39,59,45,105,110,107,115,99,97,112,101,45,102,111,110,116,45,115,112,101,99,105,102,105,99,97,116,105,111,110,58,39,49,51,95,82,111,115,104,105,39,59,108,101,116,116,101,114,45,115,112,97,
99,105,110,103,58,48,46,50,54,52,53,56,51,51,50,112,120,59,115,116,114,111,107,101,45,119,105,100,116,104,58,48,46,50,54,52,53,56,51,51,50,59,102,105,108,108,58,35,54,50,97,52,56,55,59,102,105,108,108,
45,111,112,97,99,105,116,121,58,49,34,10,32,32,32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,56,55,52,34,32,47,62,10,32,32,32,32,32,32,60,112,97,116,104,10,32,32,32,32,32,32,32,32,32,100,61,34,109,
32,55,53,46,52,50,52,49,50,56,44,49,52,49,46,50,53,49,53,50,32,113,32,48,46,48,56,52,54,55,44,48,46,49,55,57,57,50,32,48,46,48,56,52,54,55,44,48,46,51,53,57,56,52,32,48,44,48,46,52,49,50,55,53,32,45,48,
46,51,52,57,50,53,44,48,46,54,57,56,53,32,45,48,46,51,52,57,50,53,44,48,46,50,55,53,49,54,32,45,48,46,55,55,50,53,56,51,44,48,46,50,55,53,49,54,32,45,48,46,53,54,48,57,49,55,44,48,32,45,48,46,55,57,51,
55,53,44,45,48,46,52,55,54,50,53,32,108,32,45,50,46,51,49,55,55,53,44,45,52,46,56,52,55,49,54,32,113,32,45,48,46,50,49,49,54,54,55,44,45,48,46,52,52,52,53,32,45,48,46,53,56,50,48,56,52,44,45,48,46,52,
52,52,53,32,45,48,46,51,50,56,48,56,51,44,48,32,45,48,46,53,48,56,44,48,46,51,56,49,32,108,32,45,50,46,51,50,56,51,51,51,44,53,46,50,48,55,32,113,32,48,46,48,52,50,51,51,44,45,48,46,49,54,57,51,52,32,
48,46,48,52,50,51,51,44,45,48,46,51,50,56,48,57,32,48,44,45,48,46,54,56,55,57,49,32,45,48,46,54,49,51,56,51,51,44,45,48,46,57,54,51,48,56,32,45,48,46,50,49,49,54,54,55,44,45,48,46,48,57,53,51,32,45,48,
46,52,50,51,51,51,51,44,45,48,46,48,57,53,51,32,45,48,46,52,56,54,56,51,51,44,48,32,45,48,46,56,53,55,50,53,44,48,46,53,49,56,53,56,32,108,32,48,46,54,50,52,52,49,55,44,45,49,46,52,48,55,53,56,32,50,46,
50,56,53,57,57,57,44,45,52,46,57,53,51,32,113,32,48,46,50,51,50,56,51,52,44,45,48,46,53,48,56,32,48,46,55,48,57,48,56,52,44,45,48,46,55,56,51,49,55,32,48,46,52,55,54,50,53,44,45,48,46,50,55,53,49,54,32,
49,46,48,50,54,53,56,51,44,45,48,46,50,55,53,49,54,32,48,46,53,56,50,48,56,51,44,48,32,49,46,48,54,56,57,49,55,44,48,46,50,56,53,55,53,32,48,46,53,50,57,49,54,54,44,48,46,51,48,54,57,49,32,48,46,55,57,
51,55,53,44,48,46,56,53,55,50,53,32,122,32,109,32,45,53,46,53,52,53,54,54,54,44,45,49,46,54,52,48,52,49,32,113,32,48,44,45,48,46,51,55,48,52,50,32,48,46,50,52,51,52,49,54,44,45,48,46,54,51,53,32,48,46,
50,52,51,52,49,55,44,45,48,46,50,55,53,49,55,32,48,46,54,48,51,50,53,44,45,48,46,50,55,53,49,55,32,48,46,51,53,57,56,51,52,44,48,32,48,46,54,48,51,50,53,44,48,46,50,55,53,49,55,32,48,46,50,53,52,44,48,
46,50,54,52,53,56,32,48,46,50,53,52,44,48,46,54,51,53,32,48,44,48,46,51,53,57,56,51,32,45,48,46,50,53,52,44,48,46,54,51,53,32,45,48,46,50,52,51,52,49,54,44,48,46,50,55,53,49,54,32,45,48,46,54,48,51,50,
53,44,48,46,50,55,53,49,54,32,45,48,46,51,53,57,56,51,51,44,48,32,45,48,46,54,48,51,50,53,44,45,48,46,50,55,53,49,54,32,45,48,46,50,52,51,52,49,54,44,45,48,46,50,55,53,49,55,32,45,48,46,50,52,51,52,49,
54,44,45,48,46,54,51,53,32,122,34,10,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,111,110,116,45,115,116,121,108,101,58,110,111,114,109,97,108,59,102,111,110,116,45,118,97,114,105,97,110,116,
58,110,111,114,109,97,108,59,102,111,110,116,45,119,101,105,103,104,116,58,110,111,114,109,97,108,59,102,111,110,116,45,115,116,114,101,116,99,104,58,110,111,114,109,97,108,59,102,111,110,116,45,102,97,
109,105,108,121,58,39,49,51,95,82,111,115,104,105,39,59,45,105,110,107,115,99,97,112,101,45,102,111,110,116,45,115,112,101,99,105,102,105,99,97,116,105,111,110,58,39,49,51,95,82,111,115,104,105,39,59,
108,101,116,116,101,114,45,115,112,97,99,105,110,103,58,48,46,50,54,52,53,56,51,51,50,112,120,59,115,116,114,111,107,101,45,119,105,100,116,104,58,48,46,50,54,52,53,56,51,51,50,59,102,105,108,108,58,35,
54,50,97,52,56,55,59,102,105,108,108,45,111,112,97,99,105,116,121,58,49,34,10,32,32,32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,56,55,54,34,32,47,62,10,32,32,32,32,32,32,60,112,97,116,104,10,32,
32,32,32,32,32,32,32,32,100,61,34,109,32,56,52,46,56,50,55,56,51,51,44,49,51,54,46,54,50,54,54,49,32,113,32,48,44,48,46,56,56,57,32,45,48,46,54,51,53,44,49,46,54,53,49,32,45,48,46,54,54,54,55,53,44,48,
46,56,48,52,51,51,32,45,49,46,53,50,52,44,48,46,56,48,52,51,51,32,104,32,45,51,46,56,52,49,55,53,32,113,32,45,48,46,50,54,52,53,56,51,44,48,32,45,48,46,52,55,54,50,53,44,45,48,46,51,48,54,57,50,32,45,
48,46,49,57,48,53,44,45,48,46,50,55,53,49,54,32,45,48,46,49,57,48,53,44,45,48,46,53,54,48,57,49,32,48,44,45,48,46,50,54,52,53,57,32,48,46,50,50,50,50,53,44,45,48,46,53,55,49,53,32,48,46,50,51,50,56,51,
51,44,45,48,46,51,48,54,57,50,32,48,46,52,57,55,52,49,55,44,45,48,46,51,48,54,57,50,32,104,32,51,46,56,51,49,49,54,54,32,113,32,48,46,50,48,49,48,56,52,44,48,32,48,46,52,48,50,49,54,55,44,45,48,46,50,
52,51,52,50,32,48,46,50,49,49,54,54,55,44,45,48,46,50,52,51,52,49,32,48,46,50,49,49,54,54,55,44,45,48,46,52,51,51,57,49,32,48,44,45,48,46,50,50,50,50,53,32,45,48,46,49,55,57,57,49,55,44,45,48,46,52,56,
54,56,52,32,45,48,46,49,55,57,57,49,55,44,45,48,46,50,55,53,49,54,32,45,48,46,51,57,49,53,56,51,44,45,48,46,50,55,53,49,54,32,104,32,45,53,46,53,52,53,54,54,55,32,113,32,48,46,55,55,50,53,56,52,44,45,
48,46,48,57,53,50,32,48,46,55,55,50,53,56,52,44,45,48,46,56,53,55,50,53,32,48,44,45,48,46,55,49,57,54,55,32,45,48,46,56,52,54,54,54,55,44,45,48,46,57,50,48,55,53,32,72,32,56,50,46,54,57,32,113,32,48,46,
57,57,52,56,51,51,44,48,32,49,46,53,56,55,53,44,48,46,55,55,50,53,56,32,48,46,53,53,48,51,51,51,44,48,46,55,49,57,54,55,32,48,46,53,53,48,51,51,51,44,49,46,55,51,53,54,55,32,122,32,109,32,45,56,46,53,
49,57,53,56,51,44,53,46,48,52,56,50,53,32,113,32,48,44,45,48,46,51,53,57,56,52,32,48,46,50,52,51,52,49,55,44,45,48,46,54,51,53,32,48,46,50,53,52,44,45,48,46,50,55,53,49,55,32,48,46,54,49,51,56,51,51,44,
45,48,46,50,55,53,49,55,32,48,46,51,53,57,56,51,51,44,48,32,48,46,54,48,51,50,53,44,48,46,50,55,53,49,55,32,48,46,50,52,51,52,49,55,44,48,46,50,55,53,49,54,32,48,46,50,52,51,52,49,55,44,48,46,54,51,53,
32,48,44,48,46,51,55,48,52,49,32,45,48,46,50,52,51,52,49,55,44,48,46,54,52,53,53,56,32,45,48,46,50,52,51,52,49,55,44,48,46,50,54,52,53,56,32,45,48,46,54,48,51,50,53,44,48,46,50,54,52,53,56,32,45,48,46,
51,53,57,56,51,51,44,48,32,45,48,46,54,49,51,56,51,51,44,45,48,46,50,54,52,53,56,32,45,48,46,50,52,51,52,49,55,44,45,48,46,50,55,53,49,55,32,45,48,46,50,52,51,52,49,55,44,45,48,46,54,52,53,53,56,32,122,
32,109,32,45,48,46,48,49,48,53,56,44,45,50,46,51,52,57,53,32,113,32,48,44,45,48,46,51,55,48,52,50,32,48,46,50,52,51,52,49,54,44,45,48,46,54,51,53,32,48,46,50,52,51,52,49,55,44,45,48,46,50,55,53,49,55,
32,48,46,54,48,51,50,53,44,45,48,46,50,55,53,49,55,32,48,46,51,53,57,56,51,52,44,48,32,48,46,54,48,51,50,53,44,48,46,50,55,53,49,55,32,48,46,50,53,52,44,48,46,50,54,52,53,56,32,48,46,50,53,52,44,48,46,
54,51,53,32,48,44,48,46,51,53,57,56,51,32,45,48,46,50,53,52,44,48,46,54,51,53,32,45,48,46,50,52,51,52,49,54,44,48,46,50,54,52,53,56,32,45,48,46,54,48,51,50,53,44,48,46,50,54,52,53,56,32,45,48,46,51,53,
57,56,51,51,44,48,32,45,48,46,54,48,51,50,53,44,45,48,46,50,54,52,53,56,32,45,48,46,50,52,51,52,49,54,44,45,48,46,50,55,53,49,55,32,45,48,46,50,52,51,52,49,54,44,45,48,46,54,51,53,32,122,32,109,32,48,
44,45,50,46,50,49,49,57,50,32,113,32,48,44,45,48,46,51,55,48,52,50,32,48,46,50,52,51,52,49,54,44,45,48,46,54,51,53,32,48,46,50,52,51,52,49,55,44,45,48,46,50,55,53,49,55,32,48,46,54,48,51,50,53,44,45,48,
46,50,55,53,49,55,32,48,46,51,53,57,56,51,52,44,48,32,48,46,54,48,51,50,53,44,48,46,50,55,53,49,55,32,48,46,50,53,52,44,48,46,50,54,52,53,56,32,48,46,50,53,52,44,48,46,54,51,53,32,48,44,48,46,51,53,57,
56,51,32,45,48,46,50,53,52,44,48,46,54,51,53,32,45,48,46,50,52,51,52,49,54,44,48,46,50,55,53,49,55,32,45,48,46,54,48,51,50,53,44,48,46,50,55,53,49,55,32,45,48,46,51,53,57,56,51,51,44,48,32,45,48,46,54,
48,51,50,53,44,45,48,46,50,55,53,49,55,32,45,48,46,50,52,51,52,49,54,44,45,48,46,50,55,53,49,55,32,45,48,46,50,52,51,52,49,54,44,45,48,46,54,51,53,32,122,34,10,32,32,32,32,32,32,32,32,32,115,116,121,108,
101,61,34,102,111,110,116,45,115,116,121,108,101,58,110,111,114,109,97,108,59,102,111,110,116,45,118,97,114,105,97,110,116,58,110,111,114,109,97,108,59,102,111,110,116,45,119,101,105,103,104,116,58,110,
111,114,109,97,108,59,102,111,110,116,45,115,116,114,101,116,99,104,58,110,111,114,109,97,108,59,102,111,110,116,45,102,97,109,105,108,121,58,39,49,51,95,82,111,115,104,105,39,59,45,105,110,107,115,99,
97,112,101,45,102,111,110,116,45,115,112,101,99,105,102,105,99,97,116,105,111,110,58,39,49,51,95,82,111,115,104,105,39,59,108,101,116,116,101,114,45,115,112,97,99,105,110,103,58,48,46,50,54,52,53,56,51,
51,50,112,120,59,115,116,114,111,107,101,45,119,105,100,116,104,58,48,46,50,54,52,53,56,51,51,50,59,102,105,108,108,58,35,54,50,97,52,56,55,59,102,105,108,108,45,111,112,97,99,105,116,121,58,49,34,10,
32,32,32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,56,55,56,34,32,47,62,10,32,32,32,32,32,32,60,112,97,116,104,10,32,32,32,32,32,32,32,32,32,100,61,34,109,32,57,49,46,53,56,48,52,49,51,44,49,52,48,
46,55,55,53,50,55,32,113,32,48,46,52,48,50,49,54,54,44,48,32,48,46,54,50,52,52,49,54,44,45,48,46,50,57,54,51,51,32,48,46,50,50,50,50,53,44,45,48,46,50,57,54,51,51,32,48,46,50,50,50,50,53,44,45,48,46,54,
57,56,53,32,108,32,48,46,48,49,48,53,56,44,45,50,46,57,57,53,48,56,32,113,32,48,44,45,48,46,54,55,55,51,52,32,45,48,46,54,54,54,55,53,44,45,48,46,56,53,55,50,53,32,45,48,46,50,49,49,54,54,55,44,45,48,
46,48,53,50,57,32,45,49,46,48,53,56,51,51,52,44,45,48,46,48,53,50,57,32,48,46,56,48,52,51,51,52,44,45,48,46,50,50,50,50,53,32,48,46,56,48,52,51,51,52,44,45,48,46,56,57,57,53,56,32,48,44,45,48,46,55,48,
57,48,57,32,45,48,46,55,53,49,52,49,55,44,45,48,46,56,53,55,50,53,32,104,32,48,46,56,53,55,50,53,32,113,32,49,46,51,54,53,50,53,44,48,46,48,49,48,54,32,49,46,57,56,57,54,54,55,44,48,46,55,53,49,52,49,
32,48,46,53,57,50,54,54,54,44,48,46,54,56,55,57,50,32,48,46,53,57,50,54,54,54,44,50,46,48,55,52,51,52,32,118,32,50,46,56,56,57,50,53,32,113,32,45,48,46,48,49,48,53,56,44,49,46,49,48,48,54,54,32,45,48,
46,54,50,52,52,49,54,44,49,46,56,56,51,56,51,32,45,48,46,54,54,54,55,53,44,48,46,56,54,55,56,51,32,45,49,46,55,50,53,48,56,52,44,48,46,56,54,55,56,51,32,104,32,45,52,46,48,56,53,49,54,54,32,113,32,45,
49,46,48,53,56,51,51,51,44,48,32,45,49,46,55,50,53,48,56,51,44,45,48,46,56,52,54,54,54,32,45,48,46,54,49,51,56,51,52,44,45,48,46,55,56,51,49,55,32,45,48,46,54,49,51,56,51,52,44,45,49,46,56,55,51,50,53,
32,118,32,45,50,46,56,56,57,50,53,32,113,32,48,44,45,50,46,56,53,55,53,32,50,46,53,55,49,55,53,44,45,50,46,56,52,54,57,50,32,104,32,48,46,57,49,48,49,54,55,32,113,32,45,48,46,55,57,51,55,53,44,48,46,49,
52,56,49,55,32,45,48,46,55,57,51,55,53,44,48,46,56,55,56,52,50,32,48,44,48,46,55,51,48,50,53,32,48,46,55,56,51,49,54,54,44,48,46,56,56,57,32,45,48,46,57,50,48,55,52,57,44,48,32,45,49,46,49,50,49,56,51,
51,44,48,46,48,53,50,57,32,45,48,46,53,56,50,48,56,51,44,48,46,49,54,57,51,52,32,45,48,46,53,56,50,48,56,51,44,48,46,56,54,55,56,52,32,108,32,48,46,48,49,48,53,56,44,50,46,57,57,53,48,56,32,113,32,48,
44,48,46,52,49,50,55,53,32,48,46,50,49,49,54,54,55,44,48,46,54,56,55,57,50,32,48,46,50,49,49,54,54,55,44,48,46,50,55,53,49,54,32,48,46,54,49,51,56,51,51,44,48,46,50,55,53,49,54,32,122,32,109,32,45,50,
46,53,56,50,51,51,52,44,45,53,46,55,52,54,55,53,32,113,32,48,44,45,48,46,51,53,57,56,51,32,48,46,50,52,51,52,49,55,44,45,48,46,54,50,52,52,49,32,48,46,50,53,52,44,45,48,46,50,55,53,49,55,32,48,46,54,48,
51,50,53,44,45,48,46,50,55,53,49,55,32,48,46,51,53,57,56,51,51,44,48,32,48,46,54,48,51,50,53,44,48,46,50,55,53,49,55,32,48,46,50,52,51,52,49,55,44,48,46,50,54,52,53,56,32,48,46,50,52,51,52,49,55,44,48,
46,54,50,52,52,49,32,48,44,48,46,51,53,57,56,52,32,45,48,46,50,52,51,52,49,55,44,48,46,54,51,53,32,45,48,46,50,52,51,52,49,55,44,48,46,50,54,52,53,57,32,45,48,46,54,48,51,50,53,44,48,46,50,54,52,53,57,
32,45,48,46,51,52,57,50,53,44,48,32,45,48,46,54,48,51,50,53,44,45,48,46,50,54,52,53,57,32,45,48,46,50,52,51,52,49,55,44,45,48,46,50,55,53,49,54,32,45,48,46,50,52,51,52,49,55,44,45,48,46,54,51,53,32,122,
34,10,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,111,110,116,45,115,116,121,108,101,58,110,111,114,109,97,108,59,102,111,110,116,45,118,97,114,105,97,110,116,58,110,111,114,109,97,108,59,
102,111,110,116,45,119,101,105,103,104,116,58,110,111,114,109,97,108,59,102,111,110,116,45,115,116,114,101,116,99,104,58,110,111,114,109,97,108,59,102,111,110,116,45,102,97,109,105,108,121,58,39,49,51,
95,82,111,115,104,105,39,59,45,105,110,107,115,99,97,112,101,45,102,111,110,116,45,115,112,101,99,105,102,105,99,97,116,105,111,110,58,39,49,51,95,82,111,115,104,105,39,59,108,101,116,116,101,114,45,115,
112,97,99,105,110,103,58,48,46,50,54,52,53,56,51,51,50,112,120,59,115,116,114,111,107,101,45,119,105,100,116,104,58,48,46,50,54,52,53,56,51,51,50,59,102,105,108,108,58,35,54,50,97,52,56,55,59,102,105,
108,108,45,111,112,97,99,105,116,121,58,49,34,10,32,32,32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,56,56,48,34,32,47,62,10,32,32,32,32,32,32,60,112,97,116,104,10,32,32,32,32,32,32,32,32,32,100,61,
34,109,32,49,48,48,46,49,52,48,55,54,44,49,51,56,46,57,57,55,50,55,32,104,32,49,46,53,53,53,55,53,32,113,32,48,46,49,52,56,49,55,44,48,32,48,46,52,51,51,57,50,44,45,48,46,49,49,54,52,49,32,48,46,54,50,
52,52,49,44,45,48,46,50,53,52,32,49,46,48,52,55,55,53,44,45,48,46,57,51,49,51,52,32,48,46,52,51,51,57,49,44,45,48,46,54,55,55,51,51,32,48,46,52,51,51,57,49,44,45,49,46,51,55,53,56,51,32,48,44,45,48,46,
57,49,48,49,55,32,45,48,46,54,54,54,55,53,44,45,49,46,54,55,50,49,55,32,45,48,46,54,54,54,55,53,44,45,48,46,55,55,50,53,56,32,45,49,46,53,52,53,49,54,44,45,48,46,55,55,50,53,56,32,45,48,46,48,49,48,54,
44,48,32,45,50,46,54,54,55,48,48,53,44,48,32,45,50,46,54,52,53,56,51,51,44,48,32,45,50,46,54,52,53,56,51,51,44,48,32,48,46,56,49,52,57,49,55,44,48,46,51,51,56,54,55,32,48,46,56,49,52,57,49,55,44,48,46,
56,50,53,53,32,48,44,48,46,55,52,48,56,51,32,45,48,46,55,54,50,44,48,46,56,57,57,53,56,32,104,32,53,46,52,50,57,50,53,49,32,113,32,48,46,50,54,52,53,56,44,48,32,48,46,52,51,51,57,50,44,48,46,50,54,52,
53,57,32,48,46,49,52,56,49,54,44,48,46,50,50,50,50,53,32,48,46,49,52,56,49,54,44,48,46,52,56,54,56,51,32,48,44,48,46,50,53,52,32,45,48,46,49,48,53,56,51,44,48,46,52,52,52,53,32,45,48,46,49,50,55,44,48,
46,50,51,50,56,51,32,45,48,46,51,53,57,56,51,44,48,46,50,51,50,56,51,32,104,32,45,51,46,57,50,54,52,50,49,32,113,32,45,48,46,51,52,57,50,53,44,48,32,45,48,46,53,51,57,55,53,44,48,46,50,53,52,32,45,48,
46,49,57,48,53,44,48,46,50,53,52,32,45,48,46,49,57,48,53,44,48,46,54,49,51,56,52,32,48,44,48,46,51,55,48,52,49,32,48,46,51,48,54,57,49,54,44,48,46,54,56,55,57,49,32,108,32,51,46,52,56,49,57,49,53,44,51,
46,52,56,49,57,50,32,113,32,48,46,50,54,52,53,57,44,48,46,50,54,52,53,56,32,48,46,55,53,49,52,50,44,48,46,50,54,52,53,56,32,48,46,51,51,56,54,55,44,48,32,48,46,53,57,50,54,55,44,45,48,46,49,55,57,57,49,
32,108,32,45,48,46,49,55,57,57,50,44,48,46,48,56,52,55,32,113,32,48,46,49,54,57,51,51,44,48,32,48,46,51,50,56,48,56,44,45,48,46,51,51,56,54,54,32,48,46,49,52,56,49,55,44,45,48,46,51,48,54,57,50,32,48,
46,49,52,56,49,55,44,45,48,46,53,49,56,53,57,32,48,44,45,48,46,51,50,56,48,56,32,45,48,46,49,52,56,49,55,44,45,48,46,52,54,53,54,54,32,122,32,109,32,45,52,46,56,50,54,48,48,49,44,50,46,53,57,50,57,50,
32,113,32,48,44,45,48,46,51,53,57,56,51,32,48,46,50,52,51,52,49,55,44,45,48,46,54,50,52,52,50,32,48,46,50,52,51,52,49,54,44,45,48,46,50,54,52,53,56,32,48,46,53,57,50,54,54,54,44,45,48,46,50,54,52,53,56,
32,48,46,51,53,57,56,51,52,44,48,32,48,46,53,57,50,54,54,55,44,48,46,50,54,52,53,56,32,48,46,50,52,51,52,49,54,44,48,46,50,54,52,53,57,32,48,46,50,52,51,52,49,54,44,48,46,54,50,52,52,50,32,48,44,48,46,
51,53,57,56,51,32,45,48,46,50,52,51,52,49,54,44,48,46,54,50,52,52,50,32,45,48,46,50,51,50,56,51,51,44,48,46,50,54,52,53,56,32,45,48,46,53,57,50,54,54,55,44,48,46,50,54,52,53,56,32,45,48,46,51,52,57,50,
53,44,48,32,45,48,46,53,57,50,54,54,54,44,45,48,46,50,54,52,53,56,32,45,48,46,50,52,51,52,49,55,44,45,48,46,50,54,52,53,57,32,45,48,46,50,52,51,52,49,55,44,45,48,46,54,50,52,52,50,32,122,32,109,32,45,
48,46,48,49,48,53,56,44,45,50,46,51,48,55,49,55,32,113,32,48,44,45,48,46,51,53,57,56,51,32,48,46,50,51,50,56,51,51,44,45,48,46,54,50,52,52,49,32,48,46,50,52,51,52,49,55,44,45,48,46,50,55,53,49,55,32,48,
46,54,48,51,50,53,44,45,48,46,50,55,53,49,55,32,48,46,51,52,57,50,53,44,48,32,48,46,53,56,50,48,56,51,44,48,46,50,55,53,49,55,32,48,46,50,52,51,52,49,55,44,48,46,50,54,52,53,56,32,48,46,50,52,51,52,49,
55,44,48,46,54,50,52,52,49,32,48,44,48,46,51,52,57,50,53,32,45,48,46,50,52,51,52,49,55,44,48,46,54,50,52,52,50,32,45,48,46,50,51,50,56,51,51,44,48,46,50,54,52,53,56,32,45,48,46,53,56,50,48,56,51,44,48,
46,50,54,52,53,56,32,45,48,46,51,53,57,56,51,51,44,48,32,45,48,46,54,48,51,50,53,44,45,48,46,50,54,52,53,56,32,45,48,46,50,51,50,56,51,51,44,45,48,46,50,55,53,49,55,32,45,48,46,50,51,50,56,51,51,44,45,
48,46,54,50,52,52,50,32,122,32,109,32,48,44,45,50,46,49,54,57,53,56,32,113,32,48,44,45,48,46,51,53,57,56,51,32,48,46,50,51,50,56,51,51,44,45,48,46,54,50,52,52,50,32,48,46,50,52,51,52,49,55,44,45,48,46,
50,54,52,53,56,32,48,46,54,48,51,50,53,44,45,48,46,50,54,52,53,56,32,48,46,51,52,57,50,53,44,48,32,48,46,53,56,50,48,56,51,44,48,46,50,54,52,53,56,32,48,46,50,52,51,52,49,55,44,48,46,50,54,52,53,57,32,
48,46,50,52,51,52,49,55,44,48,46,54,50,52,52,50,32,48,44,48,46,51,53,57,56,51,32,45,48,46,50,52,51,52,49,55,44,48,46,54,50,52,52,50,32,45,48,46,50,51,50,56,51,51,44,48,46,50,54,52,53,56,32,45,48,46,53,
56,50,48,56,51,44,48,46,50,54,52,53,56,32,45,48,46,51,53,57,56,51,51,44,48,32,45,48,46,54,48,51,50,53,44,45,48,46,50,54,52,53,56,32,45,48,46,50,51,50,56,51,51,44,45,48,46,50,54,52,53,57,32,45,48,46,50,
51,50,56,51,51,44,45,48,46,54,50,52,52,50,32,122,34,10,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,111,110,116,45,115,116,121,108,101,58,110,111,114,109,97,108,59,102,111,110,116,45,118,97,
114,105,97,110,116,58,110,111,114,109,97,108,59,102,111,110,116,45,119,101,105,103,104,116,58,110,111,114,109,97,108,59,102,111,110,116,45,115,116,114,101,116,99,104,58,110,111,114,109,97,108,59,102,111,
110,116,45,102,97,109,105,108,121,58,39,49,51,95,82,111,115,104,105,39,59,45,105,110,107,115,99,97,112,101,45,102,111,110,116,45,115,112,101,99,105,102,105,99,97,116,105,111,110,58,39,49,51,95,82,111,
115,104,105,39,59,108,101,116,116,101,114,45,115,112,97,99,105,110,103,58,48,46,50,54,52,53,56,51,51,50,112,120,59,115,116,114,111,107,101,45,119,105,100,116,104,58,48,46,50,54,52,53,56,51,51,50,59,102,
105,108,108,58,35,54,50,97,52,56,55,59,102,105,108,108,45,111,112,97,99,105,116,121,58,49,34,10,32,32,32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,56,56,50,34,32,47,62,10,32,32,32,32,32,32,60,112,
97,116,104,10,32,32,32,32,32,32,32,32,32,100,61,34,109,32,49,48,52,46,51,50,57,56,54,44,49,51,52,46,49,49,56,51,54,32,104,32,54,46,53,55,50,50,53,32,113,32,48,46,51,57,49,53,56,44,48,32,48,46,54,50,52,
52,49,44,48,46,51,48,54,57,49,32,48,46,50,48,49,48,57,44,48,46,50,54,52,53,57,32,48,46,50,48,49,48,57,44,48,46,54,53,54,49,55,32,48,44,48,46,51,56,49,32,45,48,46,49,57,48,53,44,48,46,54,51,53,32,45,48,
46,50,50,50,50,53,44,48,46,50,56,53,55,53,32,45,48,46,53,57,50,54,55,44,48,46,50,56,53,55,53,32,104,32,45,49,46,55,56,56,53,56,32,118,32,52,46,54,57,57,32,104,32,50,46,52,56,55,48,56,32,113,32,45,48,46,
52,49,50,55,53,44,48,46,48,55,52,49,32,45,48,46,54,52,53,53,56,44,48,46,51,52,57,50,53,32,45,48,46,50,50,50,50,53,44,48,46,50,55,53,49,55,32,45,48,46,50,50,50,50,53,44,48,46,54,49,51,56,51,32,48,44,48,
46,51,50,56,48,57,32,48,46,50,48,49,48,56,44,48,46,53,55,49,53,32,48,46,50,51,50,56,51,44,48,46,50,55,53,49,55,32,48,46,54,51,53,44,48,46,51,52,57,50,53,32,104,32,45,54,46,53,55,50,50,53,32,113,32,45,
48,46,51,56,49,44,48,32,45,48,46,53,57,50,54,55,44,45,48,46,50,56,53,55,53,32,45,48,46,50,48,49,48,56,44,45,48,46,50,53,52,32,45,48,46,50,48,49,48,56,44,45,48,46,54,51,53,32,48,44,45,48,46,51,57,49,53,
56,32,48,46,50,50,50,50,53,44,45,48,46,54,55,55,51,51,32,48,46,50,50,50,50,53,44,45,48,46,50,56,53,55,53,32,48,46,54,48,51,50,53,44,45,48,46,50,56,53,55,53,32,104,32,50,46,50,48,49,51,51,32,118,32,45,
52,46,54,57,57,32,104,32,45,50,46,57,49,48,52,49,32,113,32,48,46,52,49,50,55,53,44,45,48,46,48,55,52,49,32,48,46,54,51,53,44,45,48,46,51,52,57,50,53,32,48,46,50,49,49,54,54,44,45,48,46,50,52,51,52,50,
32,48,46,50,49,49,54,54,44,45,48,46,53,55,49,53,32,48,44,45,48,46,51,50,56,48,56,32,45,48,46,50,51,50,56,51,44,45,48,46,54,48,51,50,53,32,45,48,46,50,51,50,56,51,44,45,48,46,50,56,53,55,53,32,45,48,46,
54,52,53,53,56,44,45,48,46,51,53,57,56,51,32,122,34,10,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,111,110,116,45,115,116,121,108,101,58,110,111,114,109,97,108,59,102,111,110,116,45,118,97,
114,105,97,110,116,58,110,111,114,109,97,108,59,102,111,110,116,45,119,101,105,103,104,116,58,110,111,114,109,97,108,59,102,111,110,116,45,115,116,114,101,116,99,104,58,110,111,114,109,97,108,59,102,111,
110,116,45,102,97,109,105,108,121,58,39,49,51,95,82,111,115,104,105,39,59,45,105,110,107,115,99,97,112,101,45,102,111,110,116,45,115,112,101,99,105,102,105,99,97,116,105,111,110,58,39,49,51,95,82,111,
115,104,105,39,59,108,101,116,116,101,114,45,115,112,97,99,105,110,103,58,48,46,50,54,52,53,56,51,51,50,112,120,59,115,116,114,111,107,101,45,119,105,100,116,104,58,48,46,50,54,52,53,56,51,51,50,59,102,
105,108,108,58,35,54,50,97,52,56,55,59,102,105,108,108,45,111,112,97,99,105,116,121,58,49,34,10,32,32,32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,56,56,52,34,32,47,62,10,32,32,32,32,32,32,60,112,
97,116,104,10,32,32,32,32,32,32,32,32,32,100,61,34,109,32,49,50,48,46,48,53,49,51,49,44,49,52,50,46,53,57,53,54,49,32,113,32,48,46,52,49,50,55,53,44,48,32,48,46,54,53,54,49,55,44,45,48,46,50,53,52,32,
48,46,50,52,51,52,50,44,45,48,46,50,54,52,53,57,32,48,46,50,52,51,52,50,44,45,48,46,54,53,54,49,55,32,48,44,45,48,46,51,57,49,53,56,32,45,48,46,50,52,51,52,50,44,45,48,46,54,50,52,52,50,32,45,48,46,50,
51,50,56,51,44,45,48,46,50,52,51,52,49,32,45,48,46,54,53,54,49,55,44,45,48,46,50,52,51,52,49,32,108,32,45,52,46,52,51,52,52,49,44,48,46,48,49,48,54,32,113,32,45,48,46,53,48,56,44,48,32,45,48,46,53,48,
56,44,45,48,46,51,48,54,57,50,32,48,44,45,48,46,50,52,51,52,49,32,48,46,51,49,55,53,44,45,48,46,52,56,54,56,51,32,108,32,53,46,49,54,52,54,54,44,45,51,46,56,56,52,48,56,32,113,32,48,46,51,51,56,54,55,
44,45,48,46,50,57,54,51,52,32,48,46,51,51,56,54,55,44,45,48,46,55,49,57,54,55,32,48,44,45,48,46,51,50,56,48,56,32,45,48,46,50,50,50,50,53,44,45,48,46,54,49,51,56,51,32,45,48,46,50,56,53,55,53,44,45,48,
46,51,56,49,32,45,48,46,55,49,57,54,55,44,45,48,46,51,56,49,32,45,48,46,51,49,55,53,44,48,32,45,48,46,54,49,51,56,51,44,48,46,50,50,50,50,53,32,108,32,45,54,46,49,53,57,53,44,52,46,53,50,57,54,54,32,113,
32,45,48,46,54,56,55,57,50,44,48,46,53,48,56,32,45,48,46,54,56,55,57,50,44,49,46,52,49,56,49,55,32,48,44,48,46,55,55,50,53,56,32,48,46,52,56,54,56,52,44,49,46,51,53,52,54,55,32,48,46,53,50,57,49,54,44,
48,46,54,50,52,52,49,32,49,46,51,49,50,51,51,44,48,46,54,50,52,52,49,32,122,32,109,32,45,52,46,50,49,50,49,54,44,45,54,46,55,57,52,53,32,113,32,45,48,46,51,52,57,50,53,44,48,32,45,48,46,54,50,52,52,50,
44,45,48,46,50,52,51,52,50,32,45,48,46,50,54,52,53,56,44,45,48,46,50,52,51,52,50,32,45,48,46,50,54,52,53,56,44,45,48,46,53,57,50,54,55,32,48,44,45,48,46,51,53,57,56,51,32,48,46,50,54,52,53,56,44,45,48,
46,53,57,50,54,54,32,48,46,50,55,53,49,55,44,45,48,46,50,52,51,52,50,32,48,46,54,50,52,52,50,44,45,48,46,50,52,51,52,50,32,48,46,51,53,57,56,51,44,48,32,48,46,54,50,52,52,49,44,48,46,50,52,51,52,50,32,
48,46,50,55,53,49,55,44,48,46,50,51,50,56,51,32,48,46,50,55,53,49,55,44,48,46,53,57,50,54,54,32,48,44,48,46,51,52,57,50,53,32,45,48,46,50,55,53,49,55,44,48,46,53,57,50,54,55,32,45,48,46,50,54,52,53,56,
44,48,46,50,52,51,52,50,32,45,48,46,54,50,52,52,49,44,48,46,50,52,51,52,50,32,122,32,109,32,45,50,46,52,52,52,55,53,44,48,32,113,32,45,48,46,51,52,57,50,53,44,48,32,45,48,46,54,50,52,52,50,44,45,48,46,
50,51,50,56,52,32,45,48,46,50,54,52,53,56,44,45,48,46,50,52,51,52,49,32,45,48,46,50,54,52,53,56,44,45,48,46,54,48,51,50,53,32,48,44,45,48,46,51,52,57,50,53,32,48,46,50,54,52,53,56,44,45,48,46,53,57,50,
54,54,32,48,46,50,55,53,49,55,44,45,48,46,50,52,51,52,50,32,48,46,54,50,52,52,50,44,45,48,46,50,52,51,52,50,32,48,46,51,53,57,56,51,44,48,32,48,46,54,50,52,52,49,44,48,46,50,52,51,52,50,32,48,46,50,55,
53,49,55,44,48,46,50,52,51,52,49,32,48,46,50,55,53,49,55,44,48,46,53,57,50,54,54,32,48,44,48,46,51,53,57,56,52,32,45,48,46,50,55,53,49,55,44,48,46,54,48,51,50,53,32,45,48,46,50,54,52,53,56,44,48,46,50,
51,50,56,52,32,45,48,46,54,50,52,52,49,44,48,46,50,51,50,56,52,32,122,34,10,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,111,110,116,45,115,116,121,108,101,58,110,111,114,109,97,108,59,102,
111,110,116,45,118,97,114,105,97,110,116,58,110,111,114,109,97,108,59,102,111,110,116,45,119,101,105,103,104,116,58,110,111,114,109,97,108,59,102,111,110,116,45,115,116,114,101,116,99,104,58,110,111,114,
109,97,108,59,102,111,110,116,45,102,97,109,105,108,121,58,39,49,51,95,82,111,115,104,105,39,59,45,105,110,107,115,99,97,112,101,45,102,111,110,116,45,115,112,101,99,105,102,105,99,97,116,105,111,110,
58,39,49,51,95,82,111,115,104,105,39,59,108,101,116,116,101,114,45,115,112,97,99,105,110,103,58,48,46,50,54,52,53,56,51,51,50,112,120,59,115,116,114,111,107,101,45,119,105,100,116,104,58,48,46,50,54,52,
53,56,51,51,50,59,102,105,108,108,58,35,54,50,97,52,56,55,59,102,105,108,108,45,111,112,97,99,105,116,121,58,49,34,10,32,32,32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,56,56,54,34,32,47,62,10,32,
32,32,32,32,32,60,112,97,116,104,10,32,32,32,32,32,32,32,32,32,100,61,34,109,32,49,50,55,46,53,52,55,55,49,44,49,51,52,46,49,49,56,51,54,32,113,32,48,46,51,52,57,50,53,44,48,32,48,46,53,53,48,51,51,44,
48,46,50,54,52,53,56,32,48,46,50,49,49,54,55,44,48,46,50,53,52,32,48,46,50,49,49,54,55,44,48,46,54,49,51,56,51,32,48,44,48,46,51,55,48,52,50,32,45,48,46,50,49,49,54,55,44,48,46,54,50,52,52,50,32,45,48,
46,50,50,50,50,53,44,48,46,50,56,53,55,53,32,45,48,46,53,56,50,48,56,44,48,46,50,56,53,55,53,32,108,32,45,51,46,50,52,57,48,57,44,45,48,46,48,49,48,54,32,113,32,45,48,46,52,48,50,49,54,44,48,32,45,48,
46,54,51,53,44,48,46,51,51,56,54,54,32,45,48,46,50,49,49,54,54,44,48,46,50,57,54,51,52,32,45,48,46,50,49,49,54,54,44,48,46,55,49,57,54,55,32,118,32,50,46,56,56,57,50,53,32,113,32,48,44,48,46,51,56,49,
32,48,46,50,50,50,50,53,44,48,46,54,53,54,49,55,32,48,46,50,50,50,50,53,44,48,46,50,55,53,49,54,32,48,46,54,48,51,50,53,44,48,46,50,55,53,49,54,32,104,32,50,46,57,55,51,57,49,32,113,32,45,48,46,56,51,
54,48,56,44,48,46,49,53,56,55,53,32,45,48,46,56,51,54,48,56,44,48,46,57,51,49,51,52,32,48,44,48,46,55,49,57,54,54,32,48,46,56,48,52,51,51,44,48,46,56,55,56,52,49,32,104,32,45,50,46,57,49,48,52,49,32,113,
32,45,50,46,54,51,53,50,53,44,48,32,45,50,46,54,51,53,50,53,44,45,50,46,56,52,54,57,49,32,118,32,45,50,46,56,57,57,56,52,32,113,32,48,44,45,49,46,48,57,48,48,56,32,48,46,54,49,51,56,51,44,45,49,46,56,
55,51,50,53,32,48,46,54,54,54,55,53,44,45,48,46,56,52,54,54,54,32,49,46,55,51,53,54,55,44,45,48,46,56,52,54,54,54,32,122,32,109,32,45,51,46,51,51,51,55,53,44,52,46,50,55,53,54,54,32,113,32,48,44,45,48,
46,51,53,57,56,51,32,48,46,50,52,51,52,49,44,45,48,46,54,51,53,32,48,46,50,52,51,52,50,44,45,48,46,50,55,53,49,54,32,48,46,54,48,51,50,53,44,45,48,46,50,55,53,49,54,32,48,46,51,53,57,56,52,44,48,32,48,
46,54,48,51,50,53,44,48,46,50,55,53,49,54,32,48,46,50,52,51,52,50,44,48,46,50,55,53,49,55,32,48,46,50,52,51,52,50,44,48,46,54,51,53,32,48,44,48,46,51,53,57,56,52,32,45,48,46,50,52,51,52,50,44,48,46,54,
51,53,32,45,48,46,50,52,51,52,49,44,48,46,50,54,52,53,57,32,45,48,46,54,48,51,50,53,44,48,46,50,54,52,53,57,32,45,48,46,51,53,57,56,51,44,48,32,45,48,46,54,48,51,50,53,44,45,48,46,50,54,52,53,57,32,45,
48,46,50,52,51,52,49,44,45,48,46,50,55,53,49,54,32,45,48,46,50,52,51,52,49,44,45,48,46,54,51,53,32,122,34,10,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,111,110,116,45,115,116,121,108,101,
58,110,111,114,109,97,108,59,102,111,110,116,45,118,97,114,105,97,110,116,58,110,111,114,109,97,108,59,102,111,110,116,45,119,101,105,103,104,116,58,110,111,114,109,97,108,59,102,111,110,116,45,115,116,
114,101,116,99,104,58,110,111,114,109,97,108,59,102,111,110,116,45,102,97,109,105,108,121,58,39,49,51,95,82,111,115,104,105,39,59,45,105,110,107,115,99,97,112,101,45,102,111,110,116,45,115,112,101,99,
105,102,105,99,97,116,105,111,110,58,39,49,51,95,82,111,115,104,105,39,59,108,101,116,116,101,114,45,115,112,97,99,105,110,103,58,48,46,50,54,52,53,56,51,51,50,112,120,59,115,116,114,111,107,101,45,119,
105,100,116,104,58,48,46,50,54,52,53,56,51,51,50,59,102,105,108,108,58,35,54,50,97,52,56,55,59,102,105,108,108,45,111,112,97,99,105,116,121,58,49,34,10,32,32,32,32,32,32,32,32,32,105,100,61,34,112,97,
116,104,56,56,56,34,32,47,62,10,32,32,32,32,32,32,60,112,97,116,104,10,32,32,32,32,32,32,32,32,32,100,61,34,109,32,49,51,52,46,54,54,52,55,53,44,49,51,56,46,57,57,55,50,55,32,104,32,49,46,53,53,53,55,
53,32,113,32,48,46,49,52,56,49,55,44,48,32,48,46,52,51,51,57,50,44,45,48,46,49,49,54,52,49,32,48,46,54,50,52,52,49,44,45,48,46,50,53,52,32,49,46,48,52,55,55,53,44,45,48,46,57,51,49,51,52,32,48,46,52,51,
51,57,49,44,45,48,46,54,55,55,51,51,32,48,46,52,51,51,57,49,44,45,49,46,51,55,53,56,51,32,48,44,45,48,46,57,49,48,49,55,32,45,48,46,54,54,54,55,53,44,45,49,46,54,55,50,49,55,32,45,48,46,54,54,54,55,53,
44,45,48,46,55,55,50,53,56,32,45,49,46,53,52,53,49,54,44,45,48,46,55,55,50,53,56,32,45,48,46,48,49,48,54,44,48,32,45,50,46,54,54,55,44,48,32,45,50,46,54,52,53,56,52,44,48,32,45,50,46,54,52,53,56,52,44,
48,32,48,46,56,49,52,57,50,44,48,46,51,51,56,54,55,32,48,46,56,49,52,57,50,44,48,46,56,50,53,53,32,48,44,48,46,55,52,48,56,51,32,45,48,46,55,54,50,44,48,46,56,57,57,53,56,32,104,32,53,46,52,50,57,50,53,
32,113,32,48,46,50,54,52,53,56,44,48,32,48,46,52,51,51,57,50,44,48,46,50,54,52,53,57,32,48,46,49,52,56,49,54,44,48,46,50,50,50,50,53,32,48,46,49,52,56,49,54,44,48,46,52,56,54,56,51,32,48,44,48,46,50,53,
52,32,45,48,46,49,48,53,56,51,44,48,46,52,52,52,53,32,45,48,46,49,50,55,44,48,46,50,51,50,56,51,32,45,48,46,51,53,57,56,51,44,48,46,50,51,50,56,51,32,104,32,45,51,46,57,50,54,52,50,32,113,32,45,48,46,
51,52,57,50,53,44,48,32,45,48,46,53,51,57,55,53,44,48,46,50,53,52,32,45,48,46,49,57,48,53,44,48,46,50,53,52,32,45,48,46,49,57,48,53,44,48,46,54,49,51,56,52,32,48,44,48,46,51,55,48,52,49,32,48,46,51,48,
54,57,50,44,48,46,54,56,55,57,49,32,108,32,51,46,52,56,49,57,49,44,51,46,52,56,49,57,50,32,113,32,48,46,50,54,52,53,57,44,48,46,50,54,52,53,56,32,48,46,55,53,49,52,50,44,48,46,50,54,52,53,56,32,48,46,
51,51,56,54,55,44,48,32,48,46,53,57,50,54,55,44,45,48,46,49,55,57,57,49,32,108,32,45,48,46,49,55,57,57,50,44,48,46,48,56,52,55,32,113,32,48,46,49,54,57,51,51,44,48,32,48,46,51,50,56,48,56,44,45,48,46,
51,51,56,54,54,32,48,46,49,52,56,49,55,44,45,48,46,51,48,54,57,50,32,48,46,49,52,56,49,55,44,45,48,46,53,49,56,53,57,32,48,44,45,48,46,51,50,56,48,56,32,45,48,46,49,52,56,49,55,44,45,48,46,52,54,53,54,
54,32,122,32,109,32,45,52,46,56,50,54,44,50,46,53,57,50,57,50,32,113,32,48,44,45,48,46,51,53,57,56,51,32,48,46,50,52,51,52,50,44,45,48,46,54,50,52,52,50,32,48,46,50,52,51,52,49,44,45,48,46,50,54,52,53,
56,32,48,46,53,57,50,54,54,44,45,48,46,50,54,52,53,56,32,48,46,51,53,57,56,52,44,48,32,48,46,53,57,50,54,55,44,48,46,50,54,52,53,56,32,48,46,50,52,51,52,50,44,48,46,50,54,52,53,57,32,48,46,50,52,51,52,
50,44,48,46,54,50,52,52,50,32,48,44,48,46,51,53,57,56,51,32,45,48,46,50,52,51,52,50,44,48,46,54,50,52,52,50,32,45,48,46,50,51,50,56,51,44,48,46,50,54,52,53,56,32,45,48,46,53,57,50,54,55,44,48,46,50,54,
52,53,56,32,45,48,46,51,52,57,50,53,44,48,32,45,48,46,53,57,50,54,54,44,45,48,46,50,54,52,53,56,32,45,48,46,50,52,51,52,50,44,45,48,46,50,54,52,53,57,32,45,48,46,50,52,51,52,50,44,45,48,46,54,50,52,52,
50,32,122,32,109,32,45,48,46,48,49,48,54,44,45,50,46,51,48,55,49,55,32,113,32,48,44,45,48,46,51,53,57,56,51,32,48,46,50,51,50,56,51,44,45,48,46,54,50,52,52,49,32,48,46,50,52,51,52,50,44,45,48,46,50,55,
53,49,55,32,48,46,54,48,51,50,53,44,45,48,46,50,55,53,49,55,32,48,46,51,52,57,50,53,44,48,32,48,46,53,56,50,48,56,44,48,46,50,55,53,49,55,32,48,46,50,52,51,52,50,44,48,46,50,54,52,53,56,32,48,46,50,52,
51,52,50,44,48,46,54,50,52,52,49,32,48,44,48,46,51,52,57,50,53,32,45,48,46,50,52,51,52,50,44,48,46,54,50,52,52,50,32,45,48,46,50,51,50,56,51,44,48,46,50,54,52,53,56,32,45,48,46,53,56,50,48,56,44,48,46,
50,54,52,53,56,32,45,48,46,51,53,57,56,51,44,48,32,45,48,46,54,48,51,50,53,44,45,48,46,50,54,52,53,56,32,45,48,46,50,51,50,56,51,44,45,48,46,50,55,53,49,55,32,45,48,46,50,51,50,56,51,44,45,48,46,54,50,
52,52,50,32,122,32,109,32,48,44,45,50,46,49,54,57,53,56,32,113,32,48,44,45,48,46,51,53,57,56,51,32,48,46,50,51,50,56,51,44,45,48,46,54,50,52,52,50,32,48,46,50,52,51,52,50,44,45,48,46,50,54,52,53,56,32,
48,46,54,48,51,50,53,44,45,48,46,50,54,52,53,56,32,48,46,51,52,57,50,53,44,48,32,48,46,53,56,50,48,56,44,48,46,50,54,52,53,56,32,48,46,50,52,51,52,50,44,48,46,50,54,52,53,57,32,48,46,50,52,51,52,50,44,
48,46,54,50,52,52,50,32,48,44,48,46,51,53,57,56,51,32,45,48,46,50,52,51,52,50,44,48,46,54,50,52,52,50,32,45,48,46,50,51,50,56,51,44,48,46,50,54,52,53,56,32,45,48,46,53,56,50,48,56,44,48,46,50,54,52,53,
56,32,45,48,46,51,53,57,56,51,44,48,32,45,48,46,54,48,51,50,53,44,45,48,46,50,54,52,53,56,32,45,48,46,50,51,50,56,51,44,45,48,46,50,54,52,53,57,32,45,48,46,50,51,50,56,51,44,45,48,46,54,50,52,52,50,32,
122,34,10,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,108,101,116,116,101,114,45,115,112,97,99,105,110,103,58,48,46,50,55,50,53,50,48,56,52,112,120,59,102,105,108,108,58,35,54,50,97,52,56,55,
59,102,105,108,108,45,111,112,97,99,105,116,121,58,49,34,10,32,32,32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,56,57,48,34,32,47,62,10,32,32,32,32,32,32,60,112,97,116,104,10,32,32,32,32,32,32,32,
32,32,100,61,34,109,32,49,51,57,46,53,50,49,52,44,49,51,55,46,53,56,53,48,50,32,104,32,51,46,54,55,52,53,52,32,118,32,45,48,46,56,56,48,53,51,32,104,32,45,50,46,50,57,52,52,55,32,118,32,45,48,46,48,49,
54,57,32,113,32,48,46,53,55,53,55,51,44,45,48,46,52,51,49,56,32,48,46,57,56,50,49,51,44,45,48,46,55,56,55,52,32,48,46,52,48,54,52,44,45,48,46,51,54,52,48,55,32,48,46,55,48,50,55,52,44,45,48,46,55,50,56,
49,52,32,48,46,50,57,54,51,51,44,45,48,46,51,55,50,53,51,32,48,46,52,53,55,50,44,45,48,46,55,53,51,53,51,32,48,46,49,54,48,56,54,44,45,48,46,51,56,57,52,55,32,48,46,49,54,48,56,54,44,45,48,46,56,48,52,
51,51,32,48,44,45,48,46,56,54,51,54,32,45,48,46,53,54,55,50,54,44,45,49,46,51,55,49,54,32,45,48,46,53,54,55,50,55,44,45,48,46,53,49,54,52,55,32,45,49,46,52,53,54,50,55,44,45,48,46,53,49,54,52,55,32,45,
48,46,53,54,55,50,55,44,48,32,45,48,46,57,57,57,48,55,44,48,46,49,56,54,50,55,32,45,48,46,52,51,49,56,44,48,46,49,56,54,50,54,32,45,48,46,56,56,57,44,48,46,53,57,50,54,54,32,108,32,48,46,53,54,55,50,55,
44,48,46,54,54,48,52,32,113,32,48,46,51,54,52,48,55,44,45,48,46,51,49,51,50,54,32,48,46,54,49,56,48,55,44,45,48,46,52,51,49,56,32,48,46,50,54,50,52,54,44,45,48,46,49,50,55,32,48,46,53,51,51,52,44,45,48,
46,49,50,55,32,48,46,52,55,52,49,51,44,48,32,48,46,55,54,50,44,48,46,50,56,55,56,55,32,48,46,50,57,54,51,51,44,48,46,50,56,55,56,55,32,48,46,50,57,54,51,51,44,48,46,55,55,48,52,55,32,48,44,48,46,50,57,
54,51,51,32,45,48,46,48,57,51,49,44,48,46,53,53,48,51,51,32,45,48,46,48,57,51,49,44,48,46,50,52,53,53,51,32,45,48,46,51,56,49,44,48,46,53,56,52,50,32,45,48,46,50,55,57,52,44,48,46,51,51,56,54,55,32,45,
48,46,56,50,49,50,55,44,48,46,56,50,57,55,51,32,45,48,46,53,52,49,56,55,44,48,46,52,57,49,48,55,32,45,49,46,52,57,48,49,51,44,49,46,50,55,56,52,55,32,122,34,10,32,32,32,32,32,32,32,32,32,115,116,121,108,
101,61,34,102,111,110,116,45,115,116,121,108,101,58,110,111,114,109,97,108,59,102,111,110,116,45,118,97,114,105,97,110,116,58,110,111,114,109,97,108,59,102,111,110,116,45,119,101,105,103,104,116,58,98,
111,108,100,59,102,111,110,116,45,115,116,114,101,116,99,104,58,110,111,114,109,97,108,59,102,111,110,116,45,115,105,122,101,58,56,46,52,54,54,54,54,54,50,50,112,120,59,102,111,110,116,45,102,97,109,105,
108,121,58,83,121,110,116,97,120,59,45,105,110,107,115,99,97,112,101,45,102,111,110,116,45,115,112,101,99,105,102,105,99,97,116,105,111,110,58,39,83,121,110,116,97,120,32,66,111,108,100,39,59,108,101,
116,116,101,114,45,115,112,97,99,105,110,103,58,45,49,46,51,50,50,57,49,54,54,51,112,120,59,119,111,114,100,45,115,112,97,99,105,110,103,58,45,49,46,48,53,56,51,51,51,50,56,112,120,59,102,105,108,108,
58,35,54,50,97,52,56,55,59,102,105,108,108,45,111,112,97,99,105,116,121,58,49,34,10,32,32,32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,56,57,50,34,32,47,62,10,32,32,32,32,60,47,103,62,10,32,32,32,
32,60,116,101,120,116,10,32,32,32,32,32,32,32,120,109,108,58,115,112,97,99,101,61,34,112,114,101,115,101,114,118,101,34,10,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,111,110,116,45,115,116,121,
108,101,58,110,111,114,109,97,108,59,102,111,110,116,45,119,101,105,103,104,116,58,110,111,114,109,97,108,59,102,111,110,116,45,115,105,122,101,58,49,48,46,53,56,51,51,51,51,48,50,112,120,59,108,105,110,
101,45,104,101,105,103,104,116,58,49,46,50,53,59,102,111,110,116,45,102,97,109,105,108,121,58,115,97,110,115,45,115,101,114,105,102,59,108,101,116,116,101,114,45,115,112,97,99,105,110,103,58,48,112,120,
59,119,111,114,100,45,115,112,97,99,105,110,103,58,48,112,120,59,102,105,108,108,58,35,48,48,48,48,48,48,59,102,105,108,108,45,111,112,97,99,105,116,121,58,49,59,115,116,114,111,107,101,58,110,111,110,
101,59,115,116,114,111,107,101,45,119,105,100,116,104,58,48,46,50,54,52,53,56,51,51,50,34,10,32,32,32,32,32,32,32,120,61,34,49,49,53,46,51,57,51,54,50,34,10,32,32,32,32,32,32,32,121,61,34,49,50,54,46,
56,52,57,53,52,34,10,32,32,32,32,32,32,32,105,100,61,34,116,101,120,116,56,52,51,34,62,60,116,115,112,97,110,10,32,32,32,32,32,32,32,32,32,115,111,100,105,112,111,100,105,58,114,111,108,101,61,34,108,
105,110,101,34,10,32,32,32,32,32,32,32,32,32,105,100,61,34,116,115,112,97,110,56,52,49,34,10,32,32,32,32,32,32,32,32,32,120,61,34,49,49,53,46,51,57,51,54,50,34,10,32,32,32,32,32,32,32,32,32,121,61,34,
49,51,54,46,53,48,53,48,52,34,10,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,115,116,114,111,107,101,45,119,105,100,116,104,58,48,46,50,54,52,53,56,51,51,50,34,32,47,62,60,47,116,101,120,116,
62,10,32,32,60,47,103,62,10,60,47,115,118,103,62,10,0,0};

const char* VASTVaporizerComponent::vaporizer2_svg = (const char*) resource_VASTVaporizerComponent_vaporizer2_svg;
const int VASTVaporizerComponent::vaporizer2_svgSize = 14741;

// JUCER_RESOURCE: vast_svg, 4982, "../../Resources/vast.svg"
static const unsigned char resource_VASTVaporizerComponent_vast_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,70,45,56,34,32,
115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,33,45,45,32,67,114,101,97,116,101,100,32,119,105,116,104,32,73,110,107,115,99,97,112,101,32,40,104,116,116,112,58,47,47,119,119,119,46,
105,110,107,115,99,97,112,101,46,111,114,103,47,41,32,45,45,62,10,10,60,115,118,103,10,32,32,32,120,109,108,110,115,58,100,99,61,34,104,116,116,112,58,47,47,112,117,114,108,46,111,114,103,47,100,99,47,
101,108,101,109,101,110,116,115,47,49,46,49,47,34,10,32,32,32,120,109,108,110,115,58,99,99,61,34,104,116,116,112,58,47,47,99,114,101,97,116,105,118,101,99,111,109,109,111,110,115,46,111,114,103,47,110,
115,35,34,10,32,32,32,120,109,108,110,115,58,114,100,102,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,48,50,47,50,50,45,114,100,102,45,115,121,110,116,97,120,45,
110,115,35,34,10,32,32,32,120,109,108,110,115,58,115,118,103,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,118,103,34,10,32,32,32,120,109,108,110,115,61,34,
104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,118,103,34,10,32,32,32,120,109,108,110,115,58,120,108,105,110,107,61,34,104,116,116,112,58,47,47,119,119,119,46,119,
51,46,111,114,103,47,49,57,57,57,47,120,108,105,110,107,34,10,32,32,32,120,109,108,110,115,58,115,111,100,105,112,111,100,105,61,34,104,116,116,112,58,47,47,115,111,100,105,112,111,100,105,46,115,111,
117,114,99,101,102,111,114,103,101,46,110,101,116,47,68,84,68,47,115,111,100,105,112,111,100,105,45,48,46,100,116,100,34,10,32,32,32,120,109,108,110,115,58,105,110,107,115,99,97,112,101,61,34,104,116,
116,112,58,47,47,119,119,119,46,105,110,107,115,99,97,112,101,46,111,114,103,47,110,97,109,101,115,112,97,99,101,115,47,105,110,107,115,99,97,112,101,34,10,32,32,32,119,105,100,116,104,61,34,50,49,48,
109,109,34,10,32,32,32,104,101,105,103,104,116,61,34,50,57,55,109,109,34,10,32,32,32,118,105,101,119,66,111,120,61,34,48,32,48,32,50,49,48,32,50,57,55,34,10,32,32,32,118,101,114,115,105,111,110,61,34,
49,46,49,34,10,32,32,32,105,100,61,34,115,118,103,56,34,10,32,32,32,105,110,107,115,99,97,112,101,58,118,101,114,115,105,111,110,61,34,48,46,57,50,46,49,32,114,49,53,51,55,49,34,10,32,32,32,115,111,100,
105,112,111,100,105,58,100,111,99,110,97,109,101,61,34,118,97,115,116,46,115,118,103,34,62,10,32,32,60,100,101,102,115,10,32,32,32,32,32,105,100,61,34,100,101,102,115,50,34,62,10,32,32,32,32,60,108,105,
110,101,97,114,71,114,97,100,105,101,110,116,10,32,32,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,111,108,108,101,99,116,61,34,97,108,119,97,121,115,34,10,32,32,32,32,32,32,32,105,100,61,34,108,
105,110,101,97,114,71,114,97,100,105,101,110,116,52,57,48,52,34,62,10,32,32,32,32,32,32,60,115,116,111,112,10,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,115,116,111,112,45,99,111,108,111,114,
58,35,99,50,99,50,99,50,59,115,116,111,112,45,111,112,97,99,105,116,121,58,49,34,10,32,32,32,32,32,32,32,32,32,111,102,102,115,101,116,61,34,48,34,10,32,32,32,32,32,32,32,32,32,105,100,61,34,115,116,111,
112,52,57,48,48,34,32,47,62,10,32,32,32,32,32,32,60,115,116,111,112,10,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,115,116,111,112,45,99,111,108,111,114,58,35,99,50,99,50,99,50,59,115,116,111,
112,45,111,112,97,99,105,116,121,58,48,59,34,10,32,32,32,32,32,32,32,32,32,111,102,102,115,101,116,61,34,49,34,10,32,32,32,32,32,32,32,32,32,105,100,61,34,115,116,111,112,52,57,48,50,34,32,47,62,10,32,
32,32,32,60,47,108,105,110,101,97,114,71,114,97,100,105,101,110,116,62,10,32,32,32,32,60,102,105,108,116,101,114,10,32,32,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,111,108,108,101,99,116,61,34,
97,108,119,97,121,115,34,10,32,32,32,32,32,32,32,115,116,121,108,101,61,34,99,111,108,111,114,45,105,110,116,101,114,112,111,108,97,116,105,111,110,45,102,105,108,116,101,114,115,58,115,82,71,66,34,10,
32,32,32,32,32,32,32,105,100,61,34,102,105,108,116,101,114,52,56,56,50,34,10,32,32,32,32,32,32,32,120,61,34,45,48,46,48,48,56,50,49,55,57,54,51,49,34,10,32,32,32,32,32,32,32,119,105,100,116,104,61,34,
49,46,48,49,54,52,51,53,57,34,10,32,32,32,32,32,32,32,121,61,34,45,48,46,48,50,53,51,51,51,56,51,57,34,10,32,32,32,32,32,32,32,104,101,105,103,104,116,61,34,49,46,48,53,48,54,54,55,55,34,62,10,32,32,32,
32,32,32,60,102,101,71,97,117,115,115,105,97,110,66,108,117,114,10,32,32,32,32,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,111,108,108,101,99,116,61,34,97,108,119,97,121,115,34,10,32,32,32,32,32,
32,32,32,32,115,116,100,68,101,118,105,97,116,105,111,110,61,34,48,46,52,48,52,48,48,55,52,57,34,10,32,32,32,32,32,32,32,32,32,105,100,61,34,102,101,71,97,117,115,115,105,97,110,66,108,117,114,52,56,56,
52,34,32,47,62,10,32,32,32,32,60,47,102,105,108,116,101,114,62,10,32,32,32,32,60,108,105,110,101,97,114,71,114,97,100,105,101,110,116,10,32,32,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,111,108,
108,101,99,116,61,34,97,108,119,97,121,115,34,10,32,32,32,32,32,32,32,120,108,105,110,107,58,104,114,101,102,61,34,35,108,105,110,101,97,114,71,114,97,100,105,101,110,116,52,57,48,52,34,10,32,32,32,32,
32,32,32,105,100,61,34,108,105,110,101,97,114,71,114,97,100,105,101,110,116,52,57,49,52,34,10,32,32,32,32,32,32,32,120,49,61,34,50,54,46,50,50,53,55,54,57,34,10,32,32,32,32,32,32,32,121,49,61,34,49,51,
48,46,54,51,54,51,50,34,10,32,32,32,32,32,32,32,120,50,61,34,49,53,49,46,56,50,50,50,55,34,10,32,32,32,32,32,32,32,121,50,61,34,49,53,50,46,48,55,53,56,56,34,10,32,32,32,32,32,32,32,103,114,97,100,105,
101,110,116,85,110,105,116,115,61,34,117,115,101,114,83,112,97,99,101,79,110,85,115,101,34,32,47,62,10,32,32,60,47,100,101,102,115,62,10,32,32,60,115,111,100,105,112,111,100,105,58,110,97,109,101,100,
118,105,101,119,10,32,32,32,32,32,105,100,61,34,98,97,115,101,34,10,32,32,32,32,32,112,97,103,101,99,111,108,111,114,61,34,35,102,102,102,102,102,102,34,10,32,32,32,32,32,98,111,114,100,101,114,99,111,
108,111,114,61,34,35,54,54,54,54,54,54,34,10,32,32,32,32,32,98,111,114,100,101,114,111,112,97,99,105,116,121,61,34,49,46,48,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,112,97,103,101,111,112,
97,99,105,116,121,61,34,48,46,48,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,112,97,103,101,115,104,97,100,111,119,61,34,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,122,111,111,
109,61,34,49,46,52,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,120,61,34,49,50,54,46,57,50,57,56,56,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,121,61,34,53,57,52,46,55,56,51,
54,57,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,100,111,99,117,109,101,110,116,45,117,110,105,116,115,61,34,109,109,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,117,114,114,101,
110,116,45,108,97,121,101,114,61,34,108,97,121,101,114,49,34,10,32,32,32,32,32,115,104,111,119,103,114,105,100,61,34,102,97,108,115,101,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,
100,111,119,45,119,105,100,116,104,61,34,49,52,54,51,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,104,101,105,103,104,116,61,34,55,55,48,34,10,32,32,32,32,32,105,110,
107,115,99,97,112,101,58,119,105,110,100,111,119,45,120,61,34,45,56,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,121,61,34,54,48,34,10,32,32,32,32,32,105,110,107,115,
99,97,112,101,58,119,105,110,100,111,119,45,109,97,120,105,109,105,122,101,100,61,34,49,34,32,47,62,10,32,32,60,109,101,116,97,100,97,116,97,10,32,32,32,32,32,105,100,61,34,109,101,116,97,100,97,116,97,
53,34,62,10,32,32,32,32,60,114,100,102,58,82,68,70,62,10,32,32,32,32,32,32,60,99,99,58,87,111,114,107,10,32,32,32,32,32,32,32,32,32,114,100,102,58,97,98,111,117,116,61,34,34,62,10,32,32,32,32,32,32,32,
32,60,100,99,58,102,111,114,109,97,116,62,105,109,97,103,101,47,115,118,103,43,120,109,108,60,47,100,99,58,102,111,114,109,97,116,62,10,32,32,32,32,32,32,32,32,60,100,99,58,116,121,112,101,10,32,32,32,
32,32,32,32,32,32,32,32,114,100,102,58,114,101,115,111,117,114,99,101,61,34,104,116,116,112,58,47,47,112,117,114,108,46,111,114,103,47,100,99,47,100,99,109,105,116,121,112,101,47,83,116,105,108,108,73,
109,97,103,101,34,32,47,62,10,32,32,32,32,32,32,32,32,60,100,99,58,116,105,116,108,101,62,60,47,100,99,58,116,105,116,108,101,62,10,32,32,32,32,32,32,60,47,99,99,58,87,111,114,107,62,10,32,32,32,32,60,
47,114,100,102,58,82,68,70,62,10,32,32,60,47,109,101,116,97,100,97,116,97,62,10,32,32,60,103,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,108,97,98,101,108,61,34,69,98,101,110,101,32,49,34,10,32,
32,32,32,32,105,110,107,115,99,97,112,101,58,103,114,111,117,112,109,111,100,101,61,34,108,97,121,101,114,34,10,32,32,32,32,32,105,100,61,34,108,97,121,101,114,49,34,62,10,32,32,32,32,60,103,10,32,32,
32,32,32,32,32,97,114,105,97,45,108,97,98,101,108,61,34,86,65,83,84,34,10,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,111,110,116,45,115,116,121,108,101,58,110,111,114,109,97,108,59,102,111,110,
116,45,119,101,105,103,104,116,58,110,111,114,109,97,108,59,102,111,110,116,45,115,105,122,101,58,49,48,46,53,56,51,51,51,51,48,50,112,120,59,108,105,110,101,45,104,101,105,103,104,116,58,49,50,53,37,
59,102,111,110,116,45,102,97,109,105,108,121,58,83,97,110,115,59,108,101,116,116,101,114,45,115,112,97,99,105,110,103,58,48,112,120,59,119,111,114,100,45,115,112,97,99,105,110,103,58,48,112,120,59,102,
105,108,108,58,117,114,108,40,35,108,105,110,101,97,114,71,114,97,100,105,101,110,116,52,57,49,52,41,59,102,105,108,108,45,111,112,97,99,105,116,121,58,49,59,115,116,114,111,107,101,58,110,111,110,101,
59,115,116,114,111,107,101,45,119,105,100,116,104,58,48,46,50,54,52,53,56,51,51,50,112,120,59,115,116,114,111,107,101,45,108,105,110,101,99,97,112,58,98,117,116,116,59,115,116,114,111,107,101,45,108,105,
110,101,106,111,105,110,58,109,105,116,101,114,59,115,116,114,111,107,101,45,111,112,97,99,105,116,121,58,49,59,102,105,108,116,101,114,58,117,114,108,40,35,102,105,108,116,101,114,52,56,56,50,41,59,111,
112,97,99,105,116,121,58,49,59,102,105,108,108,45,114,117,108,101,58,110,111,110,122,101,114,111,34,10,32,32,32,32,32,32,32,105,100,61,34,116,101,120,116,52,52,56,55,34,10,32,32,32,32,32,32,32,116,114,
97,110,115,102,111,114,109,61,34,109,97,116,114,105,120,40,49,44,48,44,48,44,48,46,56,56,49,52,57,50,52,53,44,48,44,49,52,46,48,55,55,51,56,56,41,34,62,10,32,32,32,32,32,32,60,112,97,116,104,10,32,32,
32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,52,56,53,57,34,10,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,111,110,116,45,115,116,121,108,101,58,110,111,114,109,97,108,59,102,111,110,
116,45,118,97,114,105,97,110,116,58,110,111,114,109,97,108,59,102,111,110,116,45,119,101,105,103,104,116,58,98,111,108,100,59,102,111,110,116,45,115,116,114,101,116,99,104,58,110,111,114,109,97,108,59,
102,111,110,116,45,115,105,122,101,58,53,48,46,55,57,57,57,57,57,50,52,112,120,59,102,111,110,116,45,102,97,109,105,108,121,58,84,97,104,111,109,97,59,45,105,110,107,115,99,97,112,101,45,102,111,110,116,
45,115,112,101,99,105,102,105,99,97,116,105,111,110,58,39,84,97,104,111,109,97,32,66,111,108,100,39,59,108,101,116,116,101,114,45,115,112,97,99,105,110,103,58,45,49,46,48,53,56,51,51,51,50,56,112,120,
59,115,116,114,111,107,101,45,119,105,100,116,104,58,48,46,50,54,52,53,56,51,51,50,112,120,59,102,105,108,108,58,117,114,108,40,35,108,105,110,101,97,114,71,114,97,100,105,101,110,116,52,57,49,52,41,59,
102,105,108,108,45,111,112,97,99,105,116,121,58,49,59,115,116,114,111,107,101,58,110,111,110,101,59,115,116,114,111,107,101,45,111,112,97,99,105,116,121,58,49,59,102,105,108,108,45,114,117,108,101,58,
110,111,110,122,101,114,111,34,10,32,32,32,32,32,32,32,32,32,100,61,34,77,32,49,49,50,46,53,49,56,55,56,44,49,53,54,46,51,57,50,56,53,32,86,32,49,50,54,46,53,48,51,50,32,104,32,45,49,48,46,57,49,52,48,
54,32,118,32,45,55,46,48,52,52,53,51,32,104,32,51,49,46,49,48,53,48,56,32,118,32,55,46,48,52,52,53,51,32,104,32,45,49,48,46,57,49,52,48,55,32,118,32,50,57,46,56,56,57,54,53,32,122,32,109,32,45,50,53,46,
52,52,57,54,48,56,44,48,46,54,54,57,55,51,32,113,32,45,52,46,50,52,49,54,48,49,44,48,32,45,55,46,52,54,54,50,49,49,44,45,48,46,56,49,56,53,54,32,45,51,46,49,57,57,56,48,52,44,45,48,46,56,49,56,53,53,32,
45,53,46,57,53,51,49,50,53,44,45,50,46,48,51,51,57,56,32,118,32,45,56,46,55,56,48,56,54,32,104,32,48,46,56,52,51,51,54,32,113,32,50,46,55,55,56,49,50,53,44,50,46,51,53,54,52,53,32,54,46,49,50,54,55,53,
55,44,51,46,54,50,49,52,57,32,51,46,51,52,56,54,51,51,44,49,46,50,54,53,48,51,32,54,46,52,57,56,56,50,56,44,49,46,50,54,53,48,51,32,48,46,55,57,51,55,53,44,48,32,50,46,48,56,51,53,57,52,44,45,48,46,49,
55,51,54,51,32,49,46,51,49,52,54,52,57,44,45,48,46,49,55,51,54,51,32,50,46,49,48,56,51,57,57,44,45,48,46,53,52,53,55,32,48,46,57,54,55,51,56,50,44,45,48,46,52,50,49,54,56,32,49,46,54,51,55,49,48,57,44,
45,49,46,49,54,53,56,50,32,48,46,54,54,57,55,50,54,44,45,48,46,55,54,56,57,53,32,48,46,54,54,57,55,50,54,44,45,50,46,48,48,57,49,56,32,48,44,45,49,46,51,56,57,48,54,32,45,49,46,48,57,49,52,48,54,44,45,
50,46,50,53,55,50,51,32,45,49,46,48,57,49,52,48,54,44,45,48,46,56,54,56,49,54,32,45,50,46,54,48,52,52,57,50,44,45,49,46,50,54,53,48,52,32,45,49,46,56,56,53,49,53,54,44,45,48,46,52,57,54,48,57,32,45,52,
46,48,52,51,49,54,52,44,45,48,46,57,52,50,53,56,32,45,50,46,49,51,51,50,48,51,44,45,48,46,52,52,54,52,56,32,45,51,46,57,57,51,53,53,53,44,45,49,46,48,57,49,52,32,45,52,46,50,57,49,50,49,44,45,49,46,53,
49,51,48,57,32,45,54,46,49,53,49,53,54,50,44,45,52,46,48,54,55,57,55,32,45,49,46,56,54,48,51,53,49,44,45,50,46,53,53,52,56,56,32,45,49,46,56,54,48,51,53,49,44,45,54,46,51,57,57,54,49,32,48,44,45,53,46,
49,56,52,49,56,32,52,46,51,57,48,52,50,57,44,45,56,46,51,56,51,57,56,32,52,46,51,57,48,52,51,44,45,51,46,50,50,52,54,49,32,49,48,46,57,54,51,54,55,50,44,45,51,46,50,50,52,54,49,32,51,46,50,57,57,48,50,
51,44,48,32,54,46,53,52,56,52,51,55,44,48,46,54,54,57,55,50,32,51,46,50,55,52,50,49,57,44,48,46,54,54,57,55,51,32,53,46,56,53,51,57,48,51,44,49,46,56,49,48,55,53,32,118,32,56,46,52,51,51,53,57,32,104,
32,45,48,46,56,49,56,53,53,32,113,32,45,50,46,48,53,56,55,57,44,45,49,46,55,54,49,49,51,32,45,53,46,48,51,53,51,53,51,44,45,50,46,57,55,54,53,54,32,45,50,46,57,53,49,55,53,56,44,45,49,46,50,49,53,52,51,
32,45,54,46,48,55,55,49,52,56,44,45,49,46,50,49,53,52,51,32,45,49,46,50,49,53,52,51,44,48,32,45,50,46,49,56,50,56,49,51,44,48,46,49,55,51,54,51,32,45,48,46,57,54,55,51,56,50,44,48,46,49,55,51,54,51,32,
45,50,46,48,51,51,57,56,52,44,48,46,54,54,57,55,51,32,45,48,46,56,54,56,49,54,52,44,48,46,51,57,54,56,55,32,45,49,46,53,49,51,48,56,54,44,49,46,49,57,48,54,50,32,45,48,46,54,50,48,49,49,55,44,48,46,55,
54,56,57,53,32,45,48,46,54,50,48,49,49,55,44,49,46,55,51,54,51,51,32,48,44,49,46,52,49,51,56,55,32,48,46,57,57,50,49,56,55,44,50,46,50,53,55,50,51,32,49,46,48,49,54,57,57,51,44,48,46,56,49,56,53,53,32,
51,46,56,57,52,51,51,54,44,49,46,52,56,56,50,56,32,49,46,56,54,48,51,53,50,44,48,46,52,50,49,54,56,32,51,46,53,55,49,56,55,53,44,48,46,56,49,56,53,53,32,49,46,55,51,54,51,50,56,44,48,46,51,57,54,56,56,
32,51,46,54,57,53,56,57,57,44,49,46,48,57,49,52,49,32,51,46,56,52,52,55,50,54,44,49,46,51,56,57,48,54,32,53,46,55,48,53,48,55,52,44,51,46,55,52,53,53,49,32,49,46,56,56,53,49,54,44,50,46,51,51,49,54,52,
32,49,46,56,56,53,49,54,44,54,46,49,55,54,51,54,32,48,44,53,46,53,48,54,54,52,32,45,52,46,51,52,48,56,50,44,56,46,56,53,53,50,56,32,45,52,46,51,49,54,48,49,54,44,51,46,51,52,56,54,51,32,45,49,49,46,54,
56,51,48,48,56,44,51,46,51,52,56,54,51,32,122,32,109,32,45,52,53,46,55,48,54,55,54,56,44,45,48,46,54,54,57,55,51,32,49,50,46,52,55,54,55,53,56,44,45,51,54,46,57,51,52,49,56,32,104,32,49,48,46,49,50,48,
51,49,50,32,108,32,49,50,46,52,55,54,55,53,56,44,51,54,46,57,51,52,49,56,32,104,32,45,57,46,53,52,57,56,48,53,32,108,32,45,50,46,51,48,54,56,51,54,44,45,55,46,53,52,48,54,50,32,72,32,53,50,46,57,55,48,
57,57,56,32,108,32,45,50,46,51,48,54,56,51,54,44,55,46,53,52,48,54,50,32,122,32,109,32,50,49,46,49,56,51,50,48,51,44,45,49,52,46,50,51,55,56,57,32,45,51,46,55,55,48,51,49,50,44,45,49,50,46,49,55,57,49,
32,45,51,46,55,55,48,51,49,51,44,49,50,46,49,55,57,49,32,122,32,77,32,52,57,46,55,57,54,44,49,49,57,46,52,53,56,54,55,32,108,32,45,49,50,46,55,44,51,54,46,57,51,52,49,56,32,104,32,45,57,46,54,55,51,56,
50,56,32,108,32,45,49,50,46,55,44,45,51,54,46,57,51,52,49,56,32,104,32,57,46,54,50,52,50,49,57,32,108,32,56,46,48,51,54,55,49,56,44,50,53,46,48,50,55,57,51,32,56,46,48,51,54,55,49,57,44,45,50,53,46,48,
50,55,57,51,32,122,34,32,47,62,10,32,32,32,32,60,47,103,62,10,32,32,60,47,103,62,10,60,47,115,118,103,62,10,0,0};

const char* VASTVaporizerComponent::vast_svg = (const char*) resource_VASTVaporizerComponent_vast_svg;
const int VASTVaporizerComponent::vast_svgSize = 4982;


//[EndFile] You can add extra defines here...

//[/EndFile]

