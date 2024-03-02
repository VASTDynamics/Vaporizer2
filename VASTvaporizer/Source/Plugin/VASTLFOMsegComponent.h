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

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "../Engine/VASTEngineHeader.h"
#include "VASTControls/VASTComboBrev.h"
#include "VASTControls/VASTComboBox.h"
#include "VASTControls/VASTSlider.h"
#include "VASTControls/VASTComboPreset.h"
#include "VASTControls/VASTImageButton.h"
#include "VASTMSEGEditor/VASTMSEGEditor.h"
#include "VASTMSEGEditor/VASTMSEGEditorPane.h"
#include "VASTMSEGEditor/VASTLFOEditor.h"
#include "VASTMSEGEditor/VASTLFOEditorPane.h"

class VASTDnDTabBarButton : public TabBarButton, public DragAndDropTarget
{
public:
    VASTAudioProcessor* myProcessor = nullptr;
    VASTAudioProcessorEditor* myEditor = nullptr;
    VASTDnDTabBarButton(VASTAudioProcessor* processor, VASTAudioProcessorEditor* editor, const String& name, TabbedButtonBar& ownerBar, int tabIndex, TabbedComponent* tabComponent) : TabBarButton(name, ownerBar), myProcessor(processor), myEditor(editor), tabbedComponent(tabComponent) {
        String compName = "";
        this->tabIndex = tabIndex;
        if (name.equalsIgnoreCase("MSEG1"))
            compName = "c_dd_MSEG1Env";
        else if (name.equalsIgnoreCase("MSEG2"))
            compName = "c_dd_MSEG2Env";
        else if (name.equalsIgnoreCase("MSEG3"))
            compName = "c_dd_MSEG3Env";
        else if (name.equalsIgnoreCase("MSEG4"))
            compName = "c_dd_MSEG4Env";
        else if (name.equalsIgnoreCase("MSEG5"))
            compName = "c_dd_MSEG5Env";
        else if (name.equalsIgnoreCase("LFO1"))
            compName = "c_dd_LFO1";
        else if (name.equalsIgnoreCase("LFO2"))
            compName = "c_dd_LFO2";
        else if (name.equalsIgnoreCase("LFO3"))
            compName = "c_dd_LFO3";
        else if (name.equalsIgnoreCase("LFO4"))
            compName = "c_dd_LFO4";
        else if (name.equalsIgnoreCase("LFO5"))
            compName = "c_dd_LFO5";
        else if (name.equalsIgnoreCase("STEPSEQ1"))
            compName = "c_dd_StepSeq1";
        else if (name.equalsIgnoreCase("STEPSEQ2"))
            compName = "c_dd_StepSeq2";
        else if (name.equalsIgnoreCase("STEPSEQ3"))
            compName = "c_dd_StepSeq3";

        VASTDragSource* dragSource = new VASTDragSource(0, compName, "n/a", "n/a");
        dragSource->setAudioProcessor(*myProcessor, *myEditor);
        dragSource->setSize(50, 50);
        setExtraComponent(dragSource, TabBarButton::ExtraComponentPlacement::afterText);
    }

    void itemDragEnter (const SourceDetails& dragSourceDetails) override {
        if (tabbedComponent->getCurrentTabIndex() != tabIndex)
            tabbedComponent->setCurrentTabIndex(tabIndex);
    }
    void itemDragMove (const SourceDetails& dragSourceDetails) override {};
    void itemDragExit (const SourceDetails& dragSourceDetails) override {};
    void itemDropped (const SourceDetails& dragSourceDetails) override {};
    bool shouldDrawDragImageWhenOver() override { return true; };
    bool isInterestedInDragSource(const SourceDetails &dragSourceDetails) override { return true; }
    int tabIndex = -1;
    TabbedComponent* tabbedComponent = nullptr;
};

class VASTTabbedMSEGComponent : public TabbedComponent
{
public:
	std::function<void(int)> TabChangedFunc;
	VASTAudioProcessor* myProcessor = nullptr;
    VASTAudioProcessorEditor* myEditor = nullptr;
	VASTTabbedMSEGComponent(VASTAudioProcessor* processor, VASTAudioProcessorEditor* editor, TabbedButtonBar::Orientation orientation) : TabbedComponent(orientation), myProcessor(processor), myEditor(editor) {
		TabChangedFunc = [](int) {};
	}
	void currentTabChanged(int index, const String&) override
	{
		VASTMSEGEditorPane* tab1 = dynamic_cast<VASTMSEGEditorPane*>(getTabContentComponent(0));
		if (tab1 != nullptr)
            tab1->stopAutoUpdate();
		VASTMSEGEditorPane* tab2 = dynamic_cast<VASTMSEGEditorPane*>(getTabContentComponent(1));
		if (tab2 != nullptr)
            tab2->stopAutoUpdate();
		VASTMSEGEditorPane* tab3 = dynamic_cast<VASTMSEGEditorPane*>(getTabContentComponent(2));
		if (tab3 != nullptr)
            tab3->stopAutoUpdate();
		VASTMSEGEditorPane* tab4 = dynamic_cast<VASTMSEGEditorPane*>(getTabContentComponent(3));
		if (tab4 != nullptr)
            tab4->stopAutoUpdate();
		VASTMSEGEditorPane* tab5 = dynamic_cast<VASTMSEGEditorPane*>(getTabContentComponent(4));
		if (tab5 != nullptr)
            tab5->stopAutoUpdate();

		switch (index) {
		case 0:
                if (tab1 != nullptr)
                    tab1->startAutoUpdate();
			break;
		case 1:
                if (tab2 != nullptr)
                    tab2->startAutoUpdate();
			break;
		case 2:
                if (tab3 != nullptr)
                    tab3->startAutoUpdate();
			break;
		case 3:
                if (tab4 != nullptr)
                    tab4->startAutoUpdate();
			break;
		case 4:
                if (tab5 != nullptr)
                    tab5->startAutoUpdate();
			break;
		}
		TabChangedFunc(index);
	}

	TabBarButton* createTabButton(const String &tabName, int tabIndex ) override {
		return new VASTDnDTabBarButton(myProcessor, myEditor, tabName, getTabbedButtonBar(), tabIndex, this);
	}
};

class VASTTabbedLFOComponent : public TabbedComponent
{
public:
	std::function<void(int)> TabChangedFunc;
	VASTAudioProcessor* myProcessor = nullptr;
    VASTAudioProcessorEditor* myEditor = nullptr;
	VASTTabbedLFOComponent(VASTAudioProcessor* processor, VASTAudioProcessorEditor* editor, TabbedButtonBar::Orientation orientation) : TabbedComponent(orientation), myProcessor(processor), myEditor(editor) {
		TabChangedFunc = [](int) {};
	}
	void currentTabChanged(int index, const String&) override
	{
		VASTLFOEditorPane* tab1 = dynamic_cast<VASTLFOEditorPane*>(getTabContentComponent(0));
		if (tab1 != nullptr)
            tab1->stopAutoUpdate();
		VASTLFOEditorPane* tab2 = dynamic_cast<VASTLFOEditorPane*>(getTabContentComponent(1));
		if (tab2 != nullptr)
            tab2->stopAutoUpdate();
		VASTLFOEditorPane* tab3 = dynamic_cast<VASTLFOEditorPane*>(getTabContentComponent(2));
		if (tab3 != nullptr)
            tab3->stopAutoUpdate();
		VASTLFOEditorPane* tab4 = dynamic_cast<VASTLFOEditorPane*>(getTabContentComponent(3));
		if (tab4 != nullptr)
            tab4->stopAutoUpdate();
		VASTLFOEditorPane* tab5 = dynamic_cast<VASTLFOEditorPane*>(getTabContentComponent(4));
		if (tab5 != nullptr)
            tab5->stopAutoUpdate();

		switch (index) {
		case 0:
                if (tab1 != nullptr)
                    tab1->startAutoUpdate();
			break;
		case 1:
                if (tab2 != nullptr)
                    tab2->startAutoUpdate();
			break;
		case 2:
                if (tab3 != nullptr)
                    tab3->startAutoUpdate();
			break;
		case 3:
                if (tab1 != nullptr)
                    tab4->startAutoUpdate();
			break;
		case 4:
                if (tab5 != nullptr)
                    tab5->startAutoUpdate();
			break;
		}
		TabChangedFunc(index);
	}

	TabBarButton* createTabButton(const String &tabName, int tabIndex) override {
		return new VASTDnDTabBarButton(myProcessor, myEditor, tabName, getTabbedButtonBar(), tabIndex, this);
	}
};
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class VASTLFOMsegComponent  : public Component,
                              public Button::Listener
{
public:
    //==============================================================================
    VASTLFOMsegComponent (AudioProcessorEditor *editor, AudioProcessor* processor);
    ~VASTLFOMsegComponent() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	void initAll() {
		((VASTMSEGEditorPane*)c_envTab->getTabContentComponent(0))->initAll();
		((VASTMSEGEditorPane*)c_envTab->getTabContentComponent(1))->initAll();
		((VASTMSEGEditorPane*)c_envTab->getTabContentComponent(2))->initAll();
		((VASTMSEGEditorPane*)c_envTab->getTabContentComponent(3))->initAll();
		((VASTMSEGEditorPane*)c_envTab->getTabContentComponent(4))->initAll();
	};
	void lookAndFeelChanged() override;
	void startAutoUpdate();
	void stopAutoUpdate();
	void updateAll();
	void buttonClicked(Button* buttonThatWasClicked) override;
	void mouseWheelMove(const MouseEvent &event, const MouseWheelDetails &wheel) override;
    VASTTabbedMSEGComponent* getEnvTab() { return c_envTab.get(); };
    VASTTabbedLFOComponent* getLFOTab(){ return c_lfoTab.get(); };
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;

    // Binary resources:
    static const char* arrow_right_corner_svg;
    static const int arrow_right_corner_svgSize;


private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	VASTAudioProcessorEditor * myEditor = nullptr;
	VASTAudioProcessor* myProcessor = nullptr;
	juce::Time lastMouseWheelEvent = juce::Time::getCurrentTime();
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<VASTTabbedMSEGComponent> c_envTab;
    std::unique_ptr<VASTDrawableButton> c_iconMaximizeEditor;
    std::unique_ptr<VASTTabbedLFOComponent> c_lfoTab;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VASTLFOMsegComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

