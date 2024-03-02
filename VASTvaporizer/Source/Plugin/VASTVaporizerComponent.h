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

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "../Engine/VASTEngineHeader.h"
#include "VASTUtils/VASTLookAndFeel.h"
#include "VASTUtils/VASTLookAndFeelThemeDefault.h"
#include "VASTUtils/VASTLookAndFeelThemeIce.h"
#include "VASTScopeDisplay/VASTOscilloscope.h"
#include "VASTControls/VASTComboBrev.h"
#include "VASTControls/VASTComboBox.h"
#include "VASTControls/VASTComboPreset.h"
#include "VASTControls/VASTImageButton.h"
#include "VASTControls/VASTButton.h"
#include "VASTControls/VASTSlider.h"
#include "VASTOscillatorComponent.h"
#include "VASTFilterComponent.h"
#include "VASTFXComponent.h"
#include "VASTMatrixComponent.h"
#include "VASTPresetEditorComponent.h"
#include "VASTMasterVoicingComponent.h"
#include "VASTHeaderComponent.h"
#include "VASTSamplerNoiseComponent.h"
#include "VASTSidePanelComponent.h"
#include "VASTGeneratorsComponent.h"
#include "VASTSidePanelHeader.h"
#include "VASTKeyboardComponent.h"
#include "VASTLogoComponent.h"
#include "VASTArpComponent.h"
#include "VASTWaveTableEditorComponent.h"
#include "VASTLFOMsegComponent.h"
#include "../Engine/VASTSingleNote.h"

class VASTTabbedComponent; //forward declaration
class VASTTabBarButton : public TabBarButton, public DragAndDropTarget
{
public:
    VASTTabBarButton (const String& name, TabbedButtonBar& ownerBar);
    bool isInterestedInDragSource(const SourceDetails &dragSourceDetails) override;
    void itemDragEnter (const SourceDetails& dragSourceDetails) override;
    void itemDragMove (const SourceDetails& dragSourceDetails) override {};
    void itemDragExit (const SourceDetails& dragSourceDetails) override {};
    void itemDropped (const SourceDetails& dragSourceDetails) override {};
    bool shouldDrawDragImageWhenOver() override { return true; };
    VASTTabbedComponent* tabbedComponent = nullptr;
    int tabIndex = -1;
    String tabName = "";
};

class VASTTabbedComponent : public TabbedComponent
{
public:
    enum TabSequence { WTEDITOR, FILTER, LFOMSEG, MATRIX, FX, ARP, PRESET };
	std::function<void(int)> TabChangedFunc;
    VASTTabbedComponent(TabbedButtonBar::Orientation orientation, AudioProcessorEditor *editor, AudioProcessor* processor);
    void currentTabChanged(int index, const String&) override;
    AudioProcessor* myProcessor;
    AudioProcessorEditor* myEditor;
    bool m_bInitialized = false;
    
    TabBarButton* createTabButton (const String& tabName, int tabIndex) override;
};
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class VASTVaporizerComponent  : public Component,                                
                                public DragAndDropContainer,
                                public KeyListener
{
public:
    //==============================================================================
    VASTVaporizerComponent (AudioProcessorEditor *editor, AudioProcessor* processor);
    ~VASTVaporizerComponent() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.

	//VASTLookAndFeel vastLookAndFeel; //should be first declaration

	/*
	OwnedArray<VASTLookAndFeel> vastLookAndFeels; //should be first declaration
	VASTLookAndFeel* getCurrentVASTLookAndFeel();
	void setActiveLookAndFeel(int no);
		*/

	void initAll(); //repaint all
	void updateAll(); //repaint all
	void updateHeader();
	void updateMatrixDisplay(); //repaint all
	void processBlock(AudioSampleBuffer &buffer);
	VASTComboPreset* getComponentCPreset();
	VASTOscillatorComponent* getOscillatorComponent(int num);
	VASTMasterVoicingComponent* getMasterVoicingComponent() {
		return masterVoicingComponent.get();
	};
	juce::SidePanel* getSidePanel() {
		return c_sidePanel.get();
	};
	VASTSidePanelComponent* getSidePanelComponent() {
		return sidePanelComponent.get();
	};
    VASTWaveTableEditorComponent* getWaveTableEditorComponent() {
        return ((VASTWaveTableEditorComponent*)c_tabbedComponent->getTabContentComponent(0));
    };

	VASTConcertinaPanel* getConcertinaPanel() {
		return c_concertinaPanel.get();
	};
	VASTGeneratorsComponent* getGeneratorsComponent() {
		return c_generatorsComponent.get();
	}

	bool keyPressed(const KeyPress& key, Component* originatingComponent) override; //return true for the ableton bug
    bool keyStateChanged(bool isKeyDown, Component* originatingComponent) override;

	std::atomic<bool> isActive = false;
	bool m_wasShown = false;
	void setLicenseText(StringRef text, bool bInErrorState, VASTAudioProcessor::vastErrorState iErrorState);
	void setVersionText(StringRef text);

	TabbedComponent* getTabbedComponent();

	bool isInterestedInFileDrag(const juce::StringArray &) { return true; }; //todo check for consumable files

	void lookAndFeelChanged() override;
	VASTAudioProcessor* getVASTProcessor() { return myProcessor;  };

	VASTKeyboardComponent* getKeyboardComponent() {
		return c_keyboardComponent.get(); };

	bool isTabConcertinaMaximized() {
		if (!c_concertinaCenter->isPanelMaximized(1)) //generator
			return false;
		if (!c_concertinaPanel->isPanelMaximized(0)) //keyboard
			return false;
		return true;
	};
	void maximizeTabConcertina() {
		c_concertinaCenter->maximizePanel(1); //generator
		c_concertinaPanel->maximizePanel(0); //keyboard
	};
	void defaultConcertina() {
		c_concertinaCenter->maximizeBothPanels(); //generator
		c_concertinaPanel->maximizeBothPanels(); //keyboard
	};

	void mouseWheelMove(const MouseEvent &event, const MouseWheelDetails &wheel) override;

	void parentSizeChanged() override;
	void selectSizeMenu();

	void startPaintingToImage();
	void endPaintingToImage();
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;

    // Binary resources:
    static const char* logokreis_svg;
    static const int logokreis_svgSize;
    static const char* logokreisv_svg;
    static const int logokreisv_svgSize;
    static const char* dynamics_svg;
    static const int dynamics_svgSize;
    static const char* copyright_svg;
    static const int copyright_svgSize;
    static const char* vaporizer2_svg;
    static const int vaporizer2_svgSize;
    static const char* vast_svg;
    static const int vast_svgSize;


private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	VASTAudioProcessorEditor* myEditor;
	VASTAudioProcessor* myProcessor;
	int sidePanelWidth = 0;
	std::unique_ptr<VASTSidePanelComponent> sidePanelComponent;
	std::unique_ptr<VASTGeneratorsComponent> c_generatorsComponent;
	std::unique_ptr<VASTTabbedComponent> c_tabbedComponent;
	std::unique_ptr<VASTConcertinaPanel> c_concertinaCenter;
	std::unique_ptr<VASTSidePanelHeader> c_sidePanelHeader;
	std::unique_ptr<VASTKeyboardComponent> c_keyboardComponent;

	juce::Time lastMouseWheelEvent = juce::Time::getCurrentTime();

	Point<int> m_lastDisplayTopLeftPoint { Point<int>(-1, -1) };

	bool m_paintToImage = false;
	Image m_SnapShotImage;

    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<VASTMasterVoicingComponent> masterVoicingComponent;
    std::unique_ptr<VASTHeaderComponent> headerComponent;
    std::unique_ptr<VASTConcertinaPanel> c_concertinaPanel;
    std::unique_ptr<VASTLogoComponent> logoComponent;
    std::unique_ptr<SidePanel> c_sidePanel;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VASTVaporizerComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

