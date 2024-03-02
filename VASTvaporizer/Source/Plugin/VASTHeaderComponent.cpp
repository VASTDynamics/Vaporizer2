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
#include "VASTControls/VASTParameterComboBox.h"
#include "VASTControls/VASTParameterSlider.h"
#include "VASTControls/VASTParameterButton.h"
#include "VASTWaveTableEditorComponent.h"

#ifdef VASTCOMMERCIAL
	#include "VASTCommercial/VASTLicenseEditor.h"
#endif

//[/Headers]

#include "VASTHeaderComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
VASTHeaderComponent::VASTHeaderComponent (AudioProcessorEditor *editor, AudioProcessor* processor)
    : myEditor((VASTAudioProcessorEditor*)editor), myProcessor((VASTAudioProcessor*)processor)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    c_Preset.reset (new VASTComboPreset ("c_Preset"));
    addAndMakeVisible (c_Preset.get());
    c_Preset->setTooltip (TRANS ("Choose presets"));
    c_Preset->setEditableText (false);
    c_Preset->setJustificationType (juce::Justification::centredLeft);
    c_Preset->setTextWhenNothingSelected (TRANS ("no preset"));
    c_Preset->setTextWhenNoChoicesAvailable (TRANS ("no presets"));
    c_Preset->addListener (this);

    c_ReloadPresets.reset (new juce::TextButton ("c_ReloadPresets"));
    addAndMakeVisible (c_ReloadPresets.get());
    c_ReloadPresets->setTooltip (TRANS ("Reload presets from folder"));
    c_ReloadPresets->setButtonText (TRANS ("Reload"));
    c_ReloadPresets->addListener (this);

    c_SavePreset.reset (new juce::TextButton ("c_SavePreset"));
    addAndMakeVisible (c_SavePreset.get());
    c_SavePreset->setTooltip (TRANS ("Save preset"));
    c_SavePreset->setButtonText (TRANS ("Save"));
    c_SavePreset->addListener (this);

    c_PresetUp.reset (new juce::TextButton ("c_PresetUp"));
    addAndMakeVisible (c_PresetUp.get());
    c_PresetUp->setTooltip (TRANS ("Preset Up (Shortcut PgUp)"));
    c_PresetUp->setButtonText (TRANS (">"));
    c_PresetUp->addListener (this);

    c_PresetDown.reset (new juce::TextButton ("c_PresetDown"));
    addAndMakeVisible (c_PresetDown.get());
    c_PresetDown->setTooltip (TRANS ("Preset Down (Shortcut PgDwn)"));
    c_PresetDown->setButtonText (TRANS ("<"));
    c_PresetDown->addListener (this);

    c_Undo.reset (new juce::TextButton ("c_Undo"));
    addAndMakeVisible (c_Undo.get());
    c_Undo->setTooltip (TRANS ("Undo last change"));
    c_Undo->setButtonText (TRANS ("Undo"));
    c_Undo->addListener (this);

    c_Redo.reset (new juce::TextButton ("c_Redo"));
    addAndMakeVisible (c_Redo.get());
    c_Redo->setTooltip (TRANS ("Redo last undo"));
    c_Redo->setButtonText (TRANS ("Redo"));
    c_Redo->addListener (this);

    c_LabelLicense.reset (new juce::Label ("c_LabelLicense",
                                           TRANS ("Free version")));
    addAndMakeVisible (c_LabelLicense.get());
    c_LabelLicense->setFont (juce::Font ("Syntax", 11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    c_LabelLicense->setJustificationType (juce::Justification::centred);
    c_LabelLicense->setEditable (false, false, false);
    c_LabelLicense->setColour (juce::Label::textColourId, juce::Colour (0xff838d95));
    c_LabelLicense->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    c_LabelLicense->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    c_InitPreset.reset (new juce::TextButton ("c_InitPreset"));
    addAndMakeVisible (c_InitPreset.get());
    c_InitPreset->setButtonText (TRANS ("Init"));
    c_InitPreset->addListener (this);

    c_Randomize.reset (new juce::TextButton ("c_Randomize"));
    addAndMakeVisible (c_Randomize.get());
    c_Randomize->setTooltip (TRANS ("Randomize preset"));
    c_Randomize->setButtonText (TRANS ("Random"));
    c_Randomize->addListener (this);


    //[UserPreSize]
	c_PresetUp->addShortcut(KeyPress(KeyPress::pageUpKey));
	c_PresetUp->setRepaintsOnMouseActivity(false);
	c_PresetDown->addShortcut(KeyPress(KeyPress::pageDownKey));
	c_PresetDown->setRepaintsOnMouseActivity(false);
	c_Preset->setScrollWheelEnabled(true);

	//manage parameter bindings -> set ComponentID = Name and processor
	c_Preset->setAudioProcessor(*myProcessor);
	c_LabelLicense->setEnabled(false);
	c_LabelLicense->setComponentID("Headertext");

	Array<Component*> children = this->getChildren();
	for (auto child : children) {
		if (child->getName().startsWith("m_")) {
			auto* aSlider = dynamic_cast<VASTParameterSlider*> (child);
			if (aSlider != nullptr) {
				aSlider->setAudioProcessor(*myProcessor);
				aSlider->bindParameter(myEditor, aSlider->getName(), VASTGUIRuntimeModel::GUIComponents::HeaderComponent, 0);
			}
			auto* aCombobox = dynamic_cast<VASTParameterComboBox*> (child);
			if (aCombobox != nullptr) {
				aCombobox->setAudioProcessor(*myProcessor);
				aCombobox->bindParameter(myEditor, aCombobox->getName(), VASTGUIRuntimeModel::GUIComponents::HeaderComponent, 0);
			}
			auto* aButton = dynamic_cast<VASTParameterButton*> (child);
			if (aButton != nullptr) {
				aButton->setAudioProcessor(*myProcessor);
				aButton->bindParameter(myEditor, aButton->getName(), VASTGUIRuntimeModel::GUIComponents::HeaderComponent, 0);
			}
		}
	}
	setOpaque(true);

	c_LabelLicense->addMouseListener(this, true);

	String presetdisplay = myProcessor->m_presetData.getCurPatchData().category + " " + myProcessor->m_presetData.getCurPatchData().presetname;
	c_Preset->setText(presetdisplay, juce::NotificationType::dontSendNotification);

    return; //dont call setSize
    //[/UserPreSize]

    setSize (325, 76);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

VASTHeaderComponent::~VASTHeaderComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    c_Preset = nullptr;
    c_ReloadPresets = nullptr;
    c_SavePreset = nullptr;
    c_PresetUp = nullptr;
    c_PresetDown = nullptr;
    c_Undo = nullptr;
    c_Redo = nullptr;
    c_LabelLicense = nullptr;
    c_InitPreset = nullptr;
    c_Randomize = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void VASTHeaderComponent::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff323e44));

    {
        int x = 0, y = 0, width = getWidth() - 0, height = getHeight() - 0;
        juce::Colour fillColour1 = juce::Colour (0xff212527), fillColour2 = juce::Colour (0xff0b0b0b);
        //[UserPaintCustomArguments] Customize the painting arguments here..
    		fillColour1 = myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colHeaderComponentBackgroundGradientFrom);
    		fillColour2 = myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colHeaderComponentBackgroundGradientTo);
        //[/UserPaintCustomArguments]
        g.setGradientFill (juce::ColourGradient (fillColour1,
                                             static_cast<float> (proportionOfWidth (-0.4677f)) - 0.0f + x,
                                             static_cast<float> (proportionOfHeight (-0.4210f)) - 0.0f + y,
                                             fillColour2,
                                             static_cast<float> (proportionOfWidth (1.0092f)) - 0.0f + x,
                                             static_cast<float> (proportionOfHeight (1.0526f)) - 0.0f + y,
                                             false));
        g.fillRect (x, y, width, height);
    }

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void VASTHeaderComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    c_Preset->setBounds (proportionOfWidth (0.0154f), proportionOfHeight (0.3947f), proportionOfWidth (0.7754f), proportionOfHeight (0.2368f));
    c_ReloadPresets->setBounds (proportionOfWidth (0.3415f), proportionOfHeight (0.7237f), proportionOfWidth (0.1600f), proportionOfHeight (0.2368f));
    c_SavePreset->setBounds (proportionOfWidth (0.0154f), proportionOfHeight (0.7237f), proportionOfWidth (0.1600f), proportionOfHeight (0.2368f));
    c_PresetUp->setBounds (proportionOfWidth (0.8985f), proportionOfHeight (0.3947f), proportionOfWidth (0.0923f), proportionOfHeight (0.2368f));
    c_PresetDown->setBounds (proportionOfWidth (0.8031f), proportionOfHeight (0.3947f), proportionOfWidth (0.0923f), proportionOfHeight (0.2368f));
    c_Undo->setBounds (proportionOfWidth (0.5046f), proportionOfHeight (0.7237f), proportionOfWidth (0.1600f), proportionOfHeight (0.2368f));
    c_Redo->setBounds (proportionOfWidth (0.6677f), proportionOfHeight (0.7237f), proportionOfWidth (0.1600f), proportionOfHeight (0.2368f));
    c_LabelLicense->setBounds (proportionOfWidth (0.0092f), proportionOfHeight (0.1579f), proportionOfWidth (0.9815f), proportionOfHeight (0.1184f));
    c_InitPreset->setBounds (proportionOfWidth (0.1785f), proportionOfHeight (0.7237f), proportionOfWidth (0.1600f), proportionOfHeight (0.2368f));
    c_Randomize->setBounds (proportionOfWidth (0.8308f), proportionOfHeight (0.7237f), proportionOfWidth (0.1600f), proportionOfHeight (0.2368f));
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void VASTHeaderComponent::comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == c_Preset.get())
    {
        //[UserComboBoxCode_c_Preset] -- add your combo box handling code here..
        //[/UserComboBoxCode_c_Preset]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}

void VASTHeaderComponent::buttonClicked (juce::Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == c_ReloadPresets.get())
    {
        //[UserButtonCode_c_ReloadPresets] -- add your button handler code here..
        if (!myProcessor->m_pVASTXperience.audioProcessLock()) {
            vassertfalse;
            return;
        }

        bool done = false;
        int counter = 0;
        while (!done) {
            if ((counter < 30) && (myProcessor->m_bAudioThreadCurrentlyRunning.load() && (!myProcessor->m_pVASTXperience.getBlockProcessingIsBlockedSuccessfully()))) {
                VDBG("VASTHeaderComponent::buttonClicked - sleep");
                Thread::sleep(100);
                counter++;
                continue;
            }
            vassert(counter < 30);
            if (counter == 30) {
                return; //dont unlock what is not locked
            }
            done = true;
        }

        myEditor->vaporizerComponent->getWaveTableEditorComponent()->stopWTRecording();

        String lid = myProcessor->m_presetData.getCurPatchData().internalid;

        myProcessor->m_presetData.reloadPresetArray(false);
        if (!myProcessor->m_pVASTXperience.audioProcessUnlock()) {
            vassertfalse;
            //error state
        }

        int lindnex = myProcessor->m_presetData.getIndexInPresetArray(lid);
        if (lindnex >= 0)
            myProcessor->setCurrentProgram(lindnex);

        myEditor->vaporizerComponent->updateAll();
        myProcessor->requestUIPresetReloadUpdate();
        //[/UserButtonCode_c_ReloadPresets]
    }
    else if (buttonThatWasClicked == c_SavePreset.get())
    {
        //[UserButtonCode_c_SavePreset] -- add your button handler code here..
		myEditor->vaporizerComponent->getWaveTableEditorComponent()->stopWTRecording();
		String m_UserPresetFolder = myProcessor->m_UserPresetRootFolder;
		String presetFilename = myProcessor->m_presetData.getCurPatchData().category + " " + myProcessor->m_presetData.getCurPatchData().presetname + ".vvp";
		File initialFile;

#ifdef JUCE_WINDOWS
		initialFile = File(m_UserPresetFolder + "\\" + presetFilename);
#else
		initialFile = File(m_UserPresetFolder + "/" + presetFilename);
#endif
		if ((!myProcessor->m_presetData.getCurPatchData().isFactory) && (!myProcessor->m_presetData.getCurPatchData().internalid.equalsIgnoreCase("init")) && (myProcessor->m_presetData.getCurPatchData().internalid != "")) {
			if (File(myProcessor->m_presetData.getCurPatchData().internalid).existsAsFile())
				initialFile = myProcessor->m_presetData.getCurPatchData().internalid;
		}

        myChooser = std::make_unique<FileChooser>(TRANS("Please specify name and location of the preset file you want to save ..."), initialFile, "*.vvp");
        myChooser->launchAsync(FileBrowserComponent::saveMode
            | FileBrowserComponent::canSelectFiles | FileBrowserComponent::warnAboutOverwriting,
            [this](const FileChooser& fileChooser)
        {
            File presetFile(fileChooser.getResult());
            if (presetFile.getFileName() != "") {
                String lPresetFileName = presetFile.getFileNameWithoutExtension().fromFirstOccurrenceOf(" ", false, true);
                String lPresetCategory = presetFile.getFileNameWithoutExtension().dropLastCharacters(presetFile.getFileNameWithoutExtension().length() - 2); //TODO
                if (!lPresetFileName.equalsIgnoreCase("")) {
                    VASTPresetElement lElem = myProcessor->m_presetData.getCurPatchData();
                    lElem.presetname = lPresetFileName;
                    myProcessor->m_presetData.exchangeCurPatchData(lElem);
                }

                myProcessor->savePatchXML(&presetFile);
                myProcessor->m_presetData.reloadPresetArray(true);
                int intid = -1;
                for (int i = 0; i < myProcessor->getNumPrograms(); i++) {
                    String fname = myProcessor->m_presetData.getPreset(i)->internalid;
                    if (fname == presetFile.getFullPathName()) {
                        intid = i;
                        break;
                    }
                }
                if (intid >= 0)
                    myProcessor->setCurrentProgram(intid);
                else {
                    //was saved outside of path
                    AlertWindow::showMessageBoxAsync(AlertWindow::InfoIcon,
                        "",
                        TRANS("The preset was saved outside of the preset path root folder in settings."), String(), this);
                }
                myEditor->vaporizerComponent->updateAll();
            }
        });
        //[/UserButtonCode_c_SavePreset]
    }
    else if (buttonThatWasClicked == c_PresetUp.get())
    {
        //[UserButtonCode_c_PresetUp] -- add your button handler code here..
		myEditor->vaporizerComponent->getWaveTableEditorComponent()->stopWTRecording();
		int idx = 0;
		const VASTPresetElement* lcurPatchData = &myProcessor->m_presetData.getCurPatchData();
		int ll = myProcessor->m_presetData.getIndexInSearchArray(lcurPatchData->internalid);
		if (ll != -1)
		{
            if (ll < myProcessor->m_presetData.getSearchArray().size() - 1)
                ll++;
            else
                ll = 0;
			String intid = myProcessor->m_presetData.getSearchArray()[ll]->internalid;
			idx = myProcessor->m_presetData.getIndexInPresetArray(intid);
			vassert(idx >= 0);
			myProcessor->setCurrentProgram(idx);
		}
		else {
			idx = myProcessor->m_presetData.getCurPatchData().presetarrayindex;
			idx++;
			if (idx > myProcessor->getNumPrograms() - 1) idx = myProcessor->getNumPrograms() - 1;
			myProcessor->setCurrentProgram(idx);
		}
		//myEditor->vaporizerComponent->updateAll();
        //[/UserButtonCode_c_PresetUp]
    }
    else if (buttonThatWasClicked == c_PresetDown.get())
    {
        //[UserButtonCode_c_PresetDown] -- add your button handler code here..
		myEditor->vaporizerComponent->getWaveTableEditorComponent()->stopWTRecording();
		int idx = 0;
		const VASTPresetElement* lcurPatchData = &myProcessor->m_presetData.getCurPatchData();
		int ll = myProcessor->m_presetData.getIndexInSearchArray(lcurPatchData->internalid);
		if (ll != -1)
		{
            if (ll > 0)
                ll--;
            else
                ll = myProcessor->m_presetData.getSearchArray().size() - 1;
			String intid = myProcessor->m_presetData.getSearchArray()[ll]->internalid;
			idx = myProcessor->m_presetData.getIndexInPresetArray(intid);
			vassert(idx >= 0);
			myProcessor->setCurrentProgram(idx);
		}
		else {
			idx = myProcessor->m_presetData.getCurPatchData().presetarrayindex;
			idx--;
			if (idx < 0) idx = 0;
			myProcessor->setCurrentProgram(idx);
		}
		//myEditor->vaporizerComponent->updateAll();
        //[/UserButtonCode_c_PresetDown]
    }
    else if (buttonThatWasClicked == c_Undo.get())
    {
        //[UserButtonCode_c_Undo] -- add your button handler code here..
		myEditor->vaporizerComponent->getWaveTableEditorComponent()->stopWTRecording();
		bool success = myProcessor->doUndo();

        if (success == false) {
            AlertWindow::showMessageBoxAsync(AlertWindow::InfoIcon,
                "",
                TRANS("Could not undo last changes."), String(), c_Undo.get()); //empty is deprecated
        }
		myEditor->vaporizerComponent->updateAll();
        //[/UserButtonCode_c_Undo]
    }
    else if (buttonThatWasClicked == c_Redo.get())
    {
        //[UserButtonCode_c_Redo] -- add your button handler code here..
		myEditor->vaporizerComponent->getWaveTableEditorComponent()->stopWTRecording();
		bool success = myProcessor->m_parameterState.undoManager->redo();
		if (success == false) {
			AlertWindow::showMessageBoxAsync(AlertWindow::InfoIcon,
				"",
				TRANS("Could not redo last changes."), String(), c_Redo.get()); //empty is deprecated
		}
		myEditor->vaporizerComponent->updateAll();
        //[/UserButtonCode_c_Redo]
    }
    else if (buttonThatWasClicked == c_InitPreset.get())
    {
        //[UserButtonCode_c_InitPreset] -- add your button handler code here..
		myEditor->vaporizerComponent->getWaveTableEditorComponent()->stopWTRecording();
		myProcessor->setCurrentProgram(0); //this is async
        //[/UserButtonCode_c_InitPreset]
    }
    else if (buttonThatWasClicked == c_Randomize.get())
    {
        //[UserButtonCode_c_Randomize] -- add your button handler code here..
        myEditor->vaporizerComponent->getWaveTableEditorComponent()->stopWTRecording();
        myEditor->randomizePatch();
        myEditor->vaporizerComponent->updateAll();
        //[/UserButtonCode_c_Randomize]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void VASTHeaderComponent::filesDropped (const juce::StringArray& filenames, int mouseX, int mouseY)
{
    //[UserCode_filesDropped] -- Add your code here...
    if (filenames.size() < 1)
        return;
    String filename = filenames[0];
    File presetfile = File(filename);
    if ((!presetfile.existsAsFile()) || (!presetfile.getFileExtension().equalsIgnoreCase(".vvp")))
        return;
    myProcessor->loadPresetFile(presetfile);
    //[/UserCode_filesDropped]
}

void VASTHeaderComponent::mouseDown (const juce::MouseEvent& e)
{
    //[UserCode_mouseDown] -- Add your code here...
	if (e.eventComponent == c_LabelLicense.get()) {
#ifdef VASTCOMMERCIAL
		l_veditor.reset(new VASTLicenseEditor(myProcessor, myEditor));
		l_veditor->setLookAndFeel(myEditor->getCurrentVASTLookAndFeel());
		l_veditor->setSize(600.f * myProcessor->getPluginScaleWidthFactor(), 350.f * myProcessor->getPluginScaleHeightFactor());
		l_veditor->setOpaque(true);

		Point<float> cPoint = myEditor->vaporizerComponent->getLocalPoint(c_LabelLicense.get(), Point<float>(0.f, 0.f));
		juce::Rectangle<int> newBounds = juce::Rectangle<int>(cPoint.getX(), cPoint.getY(), c_LabelLicense->getWidth(), c_LabelLicense->getHeight());


		myEditor->vaporizerComponent->addChildComponent(l_veditor.get());
		l_veditor->setCentreRelative(0.5f, 0.4f);
		l_veditor->setVisible(true);
        l_veditor->grabKeyboardFocus();
#endif
		//juce::CallOutBox::launchAsynchronously(l_veditor, newBounds, myEditor->vaporizerComponent);
	}
    //[/UserCode_mouseDown]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
bool VASTHeaderComponent::isInterestedInFileDrag(const juce::StringArray& filenames)
{
    if (filenames.size() < 1)
        return false;
    String filename = filenames[0];
    File presetfile = File(filename);
    if ((!presetfile.existsAsFile()) || (!presetfile.getFileExtension().equalsIgnoreCase(".vvp")))
        return false;
    return true;
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="VASTHeaderComponent" componentName=""
                 parentClasses="public Component, public FileDragAndDropTarget"
                 constructorParams="AudioProcessorEditor *editor, AudioProcessor* processor"
                 variableInitialisers="myEditor((VASTAudioProcessorEditor*)editor), myProcessor((VASTAudioProcessor*)processor)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="325" initialHeight="76">
  <METHODS>
    <METHOD name="mouseDown (const juce::MouseEvent&amp; e)"/>
    <METHOD name="filesDropped (const juce::StringArray&amp; filenames, int mouseX, int mouseY)"/>
  </METHODS>
  <BACKGROUND backgroundColour="ff323e44">
    <RECT pos="0 0 0M 0M" fill="linear: -46.769% -42.105%, 100.923% 105.263%, 0=ff212527, 1=ff0b0b0b"
          hasStroke="0"/>
  </BACKGROUND>
  <COMBOBOX name="c_Preset" id="c3c628918ce03fc3" memberName="c_Preset" virtualName="VASTComboPreset"
            explicitFocusOrder="0" pos="1.538% 39.474% 77.538% 23.684%" tooltip="Choose presets"
            editable="0" layout="33" items="" textWhenNonSelected="no preset"
            textWhenNoItems="no presets"/>
  <TEXTBUTTON name="c_ReloadPresets" id="2beb983bf4e05d5e" memberName="c_ReloadPresets"
              virtualName="" explicitFocusOrder="0" pos="34.154% 72.368% 16% 23.684%"
              tooltip="Reload presets from folder" buttonText="Reload" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="c_SavePreset" id="bcde0cceb53adcad" memberName="c_SavePreset"
              virtualName="" explicitFocusOrder="0" pos="1.538% 72.368% 16% 23.684%"
              tooltip="Save preset" buttonText="Save" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <TEXTBUTTON name="c_PresetUp" id="8f9bd8b448847d40" memberName="c_PresetUp"
              virtualName="" explicitFocusOrder="0" pos="89.846% 39.474% 9.231% 23.684%"
              tooltip="Preset Up (Shortcut PgUp)" buttonText="&gt;" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="c_PresetDown" id="b58b0ccbb24d70e6" memberName="c_PresetDown"
              virtualName="" explicitFocusOrder="0" pos="80.308% 39.474% 9.231% 23.684%"
              tooltip="Preset Down (Shortcut PgDwn)" buttonText="&lt;" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="c_Undo" id="d59a8914e3d9c513" memberName="c_Undo" virtualName=""
              explicitFocusOrder="0" pos="50.462% 72.368% 16% 23.684%" tooltip="Undo last change"
              buttonText="Undo" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="c_Redo" id="edd3483ee43ce979" memberName="c_Redo" virtualName=""
              explicitFocusOrder="0" pos="66.769% 72.368% 16% 23.684%" tooltip="Redo last undo"
              buttonText="Redo" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <LABEL name="c_LabelLicense" id="a925086450924204" memberName="c_LabelLicense"
         virtualName="" explicitFocusOrder="0" pos="0.923% 15.789% 98.154% 11.842%"
         textCol="ff838d95" edTextCol="ff000000" edBkgCol="0" labelText="Free version"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Syntax" fontsize="11.0" kerning="0.0" bold="0" italic="0"
         justification="36"/>
  <TEXTBUTTON name="c_InitPreset" id="54ad2f2aaf6eafff" memberName="c_InitPreset"
              virtualName="" explicitFocusOrder="0" pos="17.846% 72.368% 16% 23.684%"
              buttonText="Init" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="c_Randomize" id="9ec3048bf2785b4c" memberName="c_Randomize"
              virtualName="" explicitFocusOrder="0" pos="83.077% 72.368% 16% 23.684%"
              tooltip="Randomize preset" buttonText="Random" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif

//==============================================================================
// Binary resources - be careful not to edit any of these sections!

// JUCER_RESOURCE: dragdrop_png, 25439, "../../Resources/dragdrop.png"
static const unsigned char resource_VASTHeaderComponent_dragdrop_png[] = { 137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,3,212,0,0,3,210,8,6,0,0,0,154,141,146,153,0,0,0,6,98,75,71,68,0,255,0,255,0,
255,160,189,167,147,0,0,0,9,112,72,89,115,0,0,11,19,0,0,11,19,1,0,154,156,24,0,0,0,7,116,73,77,69,7,226,6,24,7,27,29,66,202,43,63,0,0,32,0,73,68,65,84,120,218,236,221,219,115,212,231,125,248,241,143,137,
169,61,13,237,56,147,56,185,72,114,19,143,15,63,51,53,99,108,108,108,78,118,44,137,196,105,103,60,189,238,191,214,76,220,214,157,230,208,206,36,96,162,253,238,234,44,173,208,249,180,171,213,174,12,6,140,
193,216,28,204,81,32,33,233,119,225,66,87,72,178,5,232,176,135,215,107,38,23,242,18,144,62,187,251,149,222,251,172,158,231,137,197,197,197,0,0,42,207,255,252,207,255,196,246,237,219,227,159,254,233,159,
12,3,0,42,208,147,70,0,0,149,167,167,167,39,210,233,116,108,223,190,61,126,250,211,159,198,238,221,187,13,5,0,42,204,54,35,0,128,202,82,40,20,226,232,209,163,49,63,63,31,183,111,223,142,99,199,142,197,
244,244,180,193,0,128,160,6,0,190,77,103,103,103,92,186,116,233,254,199,103,206,156,137,177,177,49,131,1,0,65,13,0,172,166,181,181,53,134,134,134,150,253,247,246,246,246,24,30,30,54,32,0,16,212,0,192,
131,10,133,66,164,82,169,21,111,155,157,157,141,36,73,12,9,0,4,53,0,240,160,206,206,206,184,114,229,202,253,143,119,236,216,177,184,99,199,142,251,199,113,156,60,121,50,142,28,57,98,80,0,32,168,1,128,
123,58,58,58,150,188,165,187,60,164,203,101,50,153,24,31,31,55,48,0,16,212,0,192,212,212,84,52,55,55,199,226,226,138,13,189,36,174,103,102,102,162,173,173,205,208,0,64,80,3,0,61,61,61,75,118,245,94,105,
117,186,252,191,229,243,121,191,79,13,0,130,26,0,196,244,192,192,192,183,198,244,74,146,36,137,201,201,73,3,4,0,65,13,0,245,103,106,106,42,142,29,59,22,11,11,11,107,250,243,229,177,125,237,218,181,200,
102,179,134,8,0,130,26,0,234,79,111,111,111,124,245,213,87,43,6,243,90,162,122,112,112,48,186,187,187,13,18,0,4,53,0,212,143,108,54,27,125,125,125,15,21,211,15,90,88,88,136,36,73,162,84,42,25,40,0,8,106,
0,168,125,197,98,49,254,250,215,191,174,249,173,222,15,42,143,239,47,190,248,98,201,113,91,0,128,160,6,128,154,53,48,48,16,23,46,92,88,49,144,31,37,170,187,186,186,98,104,104,200,96,1,64,80,3,64,237,234,
239,239,95,178,153,216,163,196,244,131,230,230,230,34,157,78,27,46,0,8,106,0,168,77,211,211,211,145,78,167,227,238,221,187,235,242,247,149,199,248,201,147,39,227,227,143,63,54,100,0,16,212,0,80,123,38,
38,38,226,244,233,211,43,6,241,122,200,100,50,145,203,229,12,26,0,4,53,0,212,142,177,177,177,104,111,111,95,247,152,46,255,123,110,222,188,25,93,93,93,134,13,0,130,26,0,106,71,75,75,75,220,190,125,123,
67,254,238,242,168,30,29,29,93,18,238,0,128,160,6,128,170,149,74,165,98,106,106,106,197,0,94,111,139,139,139,145,36,73,20,139,69,131,7,0,65,13,0,213,43,159,207,71,146,36,27,254,239,148,71,250,197,139,
23,163,191,191,223,240,1,64,80,3,64,245,234,236,236,140,235,215,175,175,24,190,27,25,213,217,108,54,250,250,250,220,1,0,32,168,1,160,58,99,122,116,116,116,83,98,250,65,243,243,243,145,36,73,76,79,79,187,
35,0,64,80,3,64,245,152,154,154,138,230,230,230,88,92,92,220,212,127,183,60,218,63,251,236,179,200,231,243,238,12,0,16,212,0,80,61,250,250,250,226,226,197,139,43,134,238,102,70,117,91,91,91,140,143,143,
187,67,0,64,80,3,64,117,196,116,249,239,47,111,102,76,63,104,102,102,198,49,90,0,32,168,1,160,242,149,74,165,72,165,82,113,247,238,221,45,253,60,202,35,62,151,203,69,38,147,113,231,0,128,160,6,128,202,
53,54,54,22,103,207,158,93,49,108,183,82,58,157,142,66,161,224,14,2,0,65,13,0,149,103,116,116,52,58,59,59,43,38,166,203,255,253,43,87,174,196,241,227,199,221,73,0,32,168,1,160,242,164,211,233,184,115,
231,78,69,125,78,229,81,221,215,215,23,189,189,189,238,40,0,16,212,0,80,57,82,169,212,146,51,159,43,229,173,222,229,22,22,22,162,185,185,57,74,165,146,59,12,0,4,53,0,108,189,201,201,201,138,222,244,171,
60,238,207,159,63,31,99,99,99,238,52,0,16,212,0,176,245,186,186,186,226,234,213,171,43,6,108,37,70,117,71,71,71,140,140,140,184,227,0,64,80,3,192,214,233,238,238,142,225,225,225,138,142,233,7,205,206,
206,70,107,107,171,59,15,0,4,53,0,108,141,82,169,20,233,116,58,22,23,23,171,226,243,45,143,253,98,177,24,205,205,205,238,68,0,16,212,0,176,249,134,135,135,227,252,249,243,43,6,107,53,200,100,50,49,57,
57,233,142,4,0,65,13,0,155,103,100,100,36,122,122,122,170,46,166,203,63,207,107,215,174,69,54,155,117,103,2,128,160,6,128,205,147,201,100,42,238,204,233,71,137,234,193,193,193,37,47,12,0,0,130,26,0,54,
204,177,99,199,42,254,204,233,181,90,88,88,136,36,73,156,77,13,0,130,26,0,54,86,62,159,143,150,150,150,170,255,58,156,77,13,0,130,26,0,54,85,119,119,119,92,191,126,125,197,48,173,230,168,118,54,53,0,8,
106,0,216,208,152,174,182,51,167,215,106,118,118,54,218,219,219,221,201,0,32,168,1,96,125,149,74,165,72,165,82,85,115,230,244,90,149,191,40,80,40,20,34,149,74,185,179,1,64,80,3,192,250,25,26,26,138,11,
23,46,172,24,162,181,196,217,212,0,32,168,1,96,221,12,15,15,87,229,153,211,107,85,254,245,92,189,122,53,142,31,63,238,78,7,0,65,13,0,143,47,147,201,196,236,236,108,77,127,141,229,81,221,223,223,31,217,
108,214,29,15,0,130,26,0,30,221,177,99,199,226,147,79,62,89,49,60,107,213,194,194,66,100,50,153,37,103,109,3,0,130,26,0,214,172,86,206,156,94,171,242,23,11,206,158,61,27,227,227,227,30,4,0,32,168,1,224,
225,213,210,153,211,143,18,213,29,29,29,49,54,54,230,129,0,0,130,26,0,214,174,171,171,171,102,207,156,94,171,219,183,111,59,155,26,0,4,53,0,172,93,169,84,138,36,73,106,238,204,233,181,42,127,241,32,159,
207,71,38,147,241,160,0,0,65,13,0,223,109,112,112,176,46,206,156,94,171,116,58,29,83,83,83,30,24,0,32,168,1,96,117,67,67,67,53,125,230,244,90,149,127,221,87,174,92,113,54,53,0,8,106,0,248,118,45,45,45,
49,55,55,103,16,15,68,117,95,95,159,168,6,0,65,13,0,43,59,122,244,104,221,157,57,189,86,243,243,243,145,36,137,179,169,1,64,80,3,192,82,249,124,62,90,91,91,13,226,1,15,158,77,157,203,229,12,5,0,4,53,0,
252,159,206,206,206,184,113,227,198,138,33,41,170,255,111,22,237,237,237,49,62,62,110,40,0,32,168,1,224,155,152,30,29,29,21,211,107,48,51,51,19,29,29,29,6,1,0,130,26,128,122,87,44,22,235,250,204,233,181,
42,127,145,97,98,98,34,90,90,90,12,5,0,65,109,4,0,212,179,193,193,193,248,242,203,47,87,12,71,86,151,36,137,179,169,1,16,212,70,0,64,189,26,26,26,138,108,54,43,166,215,232,193,179,169,251,251,251,13,5,
0,65,13,0,245,40,147,201,56,115,250,49,162,186,183,183,55,250,250,250,12,5,0,65,13,0,245,228,200,145,35,113,226,196,137,21,67,145,181,113,54,53,0,130,26,0,234,76,46,151,139,182,182,54,131,120,68,229,47,
62,124,246,217,103,49,57,57,105,40,0,8,106,0,168,7,29,29,29,206,156,94,199,168,110,109,109,117,54,53,0,130,26,0,106,93,103,103,231,146,248,19,211,143,111,102,102,38,58,59,59,13,2,0,65,13,0,181,202,153,
211,235,171,252,197,136,241,241,241,104,109,109,53,20,0,4,53,0,212,162,254,254,126,103,78,111,160,76,38,227,108,106,0,4,53,0,212,154,129,129,129,232,237,237,21,211,235,172,124,142,23,47,94,140,193,193,
65,67,1,64,80,3,64,45,73,167,211,113,247,238,93,131,216,224,168,206,102,179,49,48,48,96,40,0,8,106,0,168,5,127,254,243,159,227,212,169,83,43,6,32,235,235,238,221,187,145,78,167,13,2,0,65,13,0,213,110,
98,98,194,153,211,155,160,252,69,138,83,167,78,197,145,35,71,12,5,0,65,13,0,213,172,189,189,61,110,221,186,181,98,248,177,113,90,91,91,35,151,203,25,4,0,130,26,0,170,81,91,91,91,76,76,76,136,233,77,82,
62,223,155,55,111,70,87,87,151,161,0,32,168,1,160,218,76,77,77,69,146,36,6,177,133,81,61,58,58,26,29,29,29,134,2,128,160,6,128,106,210,215,215,23,151,46,93,90,49,244,216,28,139,139,139,145,36,73,20,139,
69,195,0,64,80,3,64,53,232,239,239,143,190,190,62,49,189,69,202,231,253,213,87,95,197,200,200,136,161,0,32,168,1,160,210,77,79,79,59,115,186,194,162,186,187,187,59,134,135,135,13,5,0,65,13,0,149,44,151,
203,197,233,211,167,87,12,59,182,198,236,236,108,180,182,182,26,4,0,130,26,0,42,213,248,248,248,146,77,176,196,244,214,42,159,127,169,84,138,230,230,102,67,1,64,80,3,64,37,106,107,107,91,114,230,52,149,
37,147,201,196,228,228,164,65,0,32,168,1,160,146,180,182,182,70,62,159,191,255,177,213,233,202,80,126,63,92,187,118,45,122,123,123,13,5,0,65,13,0,149,194,153,211,213,19,213,3,3,3,145,205,102,13,5,0,65,
13,0,149,160,183,183,55,46,95,190,188,98,192,81,89,22,22,22,34,147,201,196,244,244,180,97,0,32,168,1,96,43,245,245,245,69,127,127,191,152,174,112,229,247,203,217,179,103,99,98,98,194,80,0,16,212,0,176,
85,238,157,57,61,63,63,111,24,85,22,213,237,237,237,49,62,62,110,40,0,8,106,0,216,10,227,227,227,113,230,204,153,21,131,141,202,118,251,246,237,104,111,111,55,8,0,4,53,0,108,69,76,119,118,118,138,233,
42,83,126,63,229,114,185,104,105,105,49,20,0,4,53,0,108,166,214,214,214,152,153,153,49,136,42,151,36,73,76,77,77,25,4,0,130,26,0,54,67,75,75,75,76,78,78,222,255,216,234,116,117,41,191,191,174,92,185,18,
3,3,3,134,2,128,160,6,128,141,86,40,20,34,149,74,25,68,13,69,117,111,111,175,168,6,64,80,3,192,70,235,237,237,141,175,191,254,122,197,48,163,58,221,189,123,55,50,153,140,65,0,32,168,1,96,35,99,218,153,
211,181,163,252,254,251,244,211,79,227,232,209,163,134,2,128,160,6,128,245,86,42,149,34,73,146,88,88,88,48,140,26,213,210,210,18,185,92,206,32,0,16,212,0,176,158,198,198,198,226,243,207,63,191,255,177,
213,233,218,80,126,63,222,188,121,51,186,187,187,13,5,0,65,13,0,235,101,116,116,52,186,186,186,196,116,29,68,245,200,200,200,146,243,197,1,64,80,3,192,99,104,109,109,141,219,183,111,27,68,29,88,92,92,
140,116,58,29,165,82,201,48,0,16,212,0,240,56,146,36,137,169,169,169,251,31,91,157,174,77,229,247,235,133,11,23,98,100,100,196,80,0,16,212,0,240,168,10,133,130,227,148,234,52,170,187,186,186,68,53,0,130,
26,0,30,85,79,79,143,51,167,235,212,236,236,108,180,182,182,26,4,0,130,26,0,30,86,111,111,111,12,14,14,138,233,58,83,126,63,23,139,197,104,110,110,54,20,0,4,53,0,172,85,169,84,138,84,42,229,204,105,34,
147,201,196,228,228,164,65,0,32,168,1,96,45,70,71,71,227,220,185,115,247,63,182,58,93,95,202,239,239,107,215,174,197,241,227,199,13,5,0,65,13,0,107,137,105,103,78,83,126,191,15,12,12,136,106,0,4,53,0,
124,151,76,38,19,119,238,220,49,8,238,155,159,159,143,36,73,98,122,122,218,48,0,16,212,0,176,146,36,73,162,84,42,221,255,216,234,116,125,43,191,255,207,158,61,27,185,92,206,80,0,16,212,0,240,160,201,201,
201,72,167,211,6,193,170,81,221,222,222,30,227,227,227,134,2,128,160,6,128,114,93,93,93,113,245,234,213,21,67,10,34,34,102,102,102,162,163,163,195,32,0,16,212,0,80,30,211,195,195,195,98,154,21,149,63,
30,38,38,38,162,165,165,197,80,0,16,212,0,80,42,149,34,73,146,88,92,212,208,172,77,75,75,75,20,139,69,131,0,64,80,3,80,223,6,7,7,227,194,133,11,247,63,182,58,205,74,202,31,23,23,47,94,140,129,129,1,67,
1,64,80,3,80,191,134,134,134,162,167,167,71,76,243,208,81,157,205,102,99,112,112,208,80,0,16,212,0,212,167,76,38,19,115,115,115,6,193,67,187,123,247,174,93,225,1,16,212,0,212,167,35,71,142,196,137,19,
39,238,127,108,117,154,181,40,127,156,124,250,233,167,241,241,199,31,27,10,0,130,26,128,250,145,203,229,162,173,173,205,32,120,108,153,76,38,114,185,156,65,0,32,168,1,168,15,29,29,29,113,227,198,141,251,
31,91,157,230,97,148,63,94,110,222,188,185,228,247,240,1,64,80,3,80,211,49,61,62,62,46,166,89,183,168,30,30,30,142,238,238,110,67,1,64,80,3,80,187,166,166,166,162,185,185,217,153,211,172,171,197,197,197,
72,146,36,74,165,146,97,0,32,168,1,168,77,253,253,253,113,233,210,165,251,31,91,157,230,113,148,63,126,190,248,226,139,24,29,29,53,20,0,4,53,0,181,25,211,199,143,31,23,211,108,88,84,119,118,118,198,200,
200,136,161,0,32,168,1,168,29,211,211,211,145,78,167,227,238,221,187,134,193,134,153,157,157,181,123,60,0,130,26,128,218,146,203,229,226,244,233,211,247,63,182,58,205,122,42,127,60,77,77,77,69,42,149,
50,20,0,4,53,0,213,111,124,124,60,58,58,58,196,52,155,166,165,165,37,10,133,130,65,0,32,168,1,168,110,173,173,173,113,235,214,45,131,96,67,149,191,80,243,245,215,95,47,249,125,125,0,16,212,0,84,101,76,
79,78,78,174,24,61,176,145,81,253,224,38,120,0,32,168,1,168,26,133,66,193,239,178,178,101,230,231,231,35,73,146,152,158,158,54,12,0,4,53,0,213,37,155,205,198,149,43,87,238,127,108,117,154,205,80,254,56,
59,123,246,108,228,243,121,67,1,64,80,3,80,61,122,123,123,99,96,96,64,76,179,229,81,221,214,214,22,227,227,227,134,2,128,160,6,160,242,149,74,165,72,146,36,22,22,22,12,131,45,55,51,51,19,157,157,157,6,
1,128,160,6,160,242,141,142,142,198,231,159,127,126,255,99,171,211,108,133,242,199,221,248,248,120,180,183,183,27,10,0,130,26,128,202,142,233,174,174,46,49,77,197,73,146,36,138,197,162,65,0,32,168,1,168,
76,153,76,38,238,220,185,99,16,84,132,242,23,116,46,94,188,24,131,131,131,134,2,128,160,6,160,242,164,82,169,40,149,74,43,198,12,84,66,84,103,179,89,81,13,128,160,6,160,178,76,78,78,70,38,147,49,8,42,
218,220,220,156,199,41,0,130,26,128,202,210,213,213,21,87,175,94,189,255,177,213,105,42,73,249,227,241,228,201,147,113,236,216,49,67,1,64,80,3,80,25,49,61,60,60,44,166,169,26,45,45,45,145,207,231,13,2,
0,65,13,192,214,41,22,139,145,36,73,44,46,106,104,42,91,249,11,61,215,175,95,143,158,158,30,67,1,64,80,3,176,117,134,134,134,226,194,133,11,43,70,11,84,114,84,15,13,13,137,106,0,4,53,0,91,23,211,229,65,
34,166,169,38,139,139,139,203,118,166,7,0,65,13,192,166,200,100,50,49,55,55,103,16,84,149,242,23,126,190,248,226,139,24,27,27,51,20,0,4,53,0,155,231,200,145,35,113,226,196,137,21,35,5,170,41,170,59,58,
58,98,100,100,196,80,0,16,212,0,108,188,92,46,23,109,109,109,6,65,77,152,157,157,141,246,246,118,131,0,64,80,3,176,241,218,219,219,227,198,141,27,247,63,182,58,77,53,42,127,220,22,10,133,200,100,50,134,
2,128,160,6,96,99,99,122,124,124,92,76,83,115,210,233,116,76,77,77,25,4,0,130,26,128,245,55,53,53,21,169,84,202,32,168,25,229,47,8,93,185,114,37,142,31,63,110,40,0,8,106,0,214,95,95,95,95,92,186,116,105,
197,24,129,90,136,234,190,190,190,232,235,235,51,20,0,4,53,0,235,167,191,191,127,73,104,136,105,106,209,252,252,124,36,73,18,211,211,211,134,1,128,160,6,224,241,77,79,79,71,58,157,142,187,119,239,26,6,
53,169,252,5,162,207,62,251,44,38,39,39,13,5,0,65,13,192,227,203,229,114,113,250,244,233,21,227,3,106,49,170,91,91,91,151,108,190,7,0,130,26,128,135,54,62,62,30,29,29,29,98,154,186,50,51,51,19,93,93,93,
6,1,128,160,6,224,209,181,182,182,198,173,91,183,12,130,186,80,254,130,209,131,47,38,1,128,160,6,96,205,90,90,90,150,252,46,169,213,105,234,201,226,226,98,36,73,18,197,98,209,48,0,16,212,0,172,93,161,
80,112,230,52,117,169,252,133,163,175,190,250,42,134,134,134,12,5,0,65,13,192,218,101,179,217,248,250,235,175,87,140,12,168,167,168,238,233,233,17,213,0,8,106,0,214,166,183,183,55,6,6,6,196,52,68,196,
220,220,92,164,211,105,131,0,64,80,3,240,237,74,165,82,36,73,18,11,11,11,134,65,93,43,127,33,233,228,201,147,241,215,191,254,213,80,0,16,212,0,172,110,116,116,52,62,255,252,243,21,163,2,234,89,38,147,
89,178,73,31,0,8,106,0,150,196,116,249,217,187,98,154,122,87,254,28,184,126,253,122,244,244,244,24,10,0,130,26,128,229,50,153,76,220,185,115,199,32,96,149,168,30,26,26,18,213,0,8,106,0,150,74,165,82,81,
42,149,86,140,8,224,27,11,11,11,145,36,201,146,231,10,0,130,26,128,58,150,207,231,35,73,18,131,128,85,148,191,192,116,254,252,249,24,31,31,55,20,0,4,53,0,17,221,221,221,113,253,250,245,21,227,1,88,254,
188,232,232,232,136,209,209,81,67,1,64,80,3,212,179,174,174,174,24,30,30,22,211,240,16,238,220,185,19,29,29,29,6,1,128,160,6,168,87,197,98,49,146,36,137,197,69,13,13,107,81,254,130,83,62,159,143,76,38,
99,40,0,130,26,128,122,52,56,56,24,23,46,92,88,49,22,128,239,150,78,167,99,106,106,202,32,0,4,53,0,245,100,104,104,40,178,217,172,152,134,135,84,254,92,185,114,229,74,244,245,245,25,10,128,160,6,160,158,
100,50,153,152,155,155,51,8,120,204,168,62,126,252,184,168,6,16,212,0,212,139,191,252,229,47,113,226,196,137,21,227,0,120,56,243,243,243,145,36,73,76,79,79,27,6,128,160,6,160,150,77,76,76,68,91,91,155,
65,192,99,42,127,33,234,179,207,62,139,66,161,96,40,0,130,26,128,90,214,217,217,25,55,111,222,92,49,10,128,71,143,234,182,182,182,152,152,152,48,20,0,65,13,64,173,198,244,248,248,184,152,134,13,112,243,
230,205,232,238,238,54,8,0,65,13,64,173,113,230,52,108,140,242,23,166,70,71,71,163,163,163,195,80,0,4,53,0,181,100,96,96,32,190,252,242,203,21,35,0,88,31,139,139,139,145,36,73,20,139,69,195,0,16,212,0,
212,130,161,161,161,232,237,237,21,211,176,65,202,159,83,95,125,245,85,12,15,15,27,10,128,160,6,160,22,164,211,105,103,78,195,38,70,117,119,119,119,12,13,13,25,10,128,160,6,160,154,253,229,47,127,137,
147,39,79,174,248,67,63,176,49,230,230,230,28,79,7,32,168,1,168,102,185,92,46,218,219,219,13,2,54,73,249,11,86,165,82,41,154,155,155,13,5,64,80,3,80,141,58,58,58,226,198,141,27,43,254,176,15,108,188,76,
38,19,147,147,147,6,1,32,168,1,168,38,206,156,134,173,81,254,92,187,118,237,218,146,13,1,1,16,212,0,84,56,103,78,67,229,68,245,192,192,64,100,179,89,67,1,16,212,0,84,3,103,78,67,229,88,88,88,136,84,42,
21,165,82,201,48,0,4,53,0,149,204,153,211,80,25,202,159,123,231,207,159,143,137,137,9,67,1,16,212,0,84,178,76,38,227,204,105,168,192,168,238,232,232,136,177,177,49,67,1,16,212,0,84,162,35,71,142,196,137,
19,39,86,252,97,30,216,90,183,111,223,142,142,142,14,131,0,16,212,0,84,154,124,62,31,109,109,109,6,1,21,166,252,133,173,92,46,23,45,45,45,134,2,32,168,1,168,36,206,156,134,234,144,36,73,76,77,77,25,4,
128,160,6,160,18,116,118,118,46,249,221,76,49,13,149,165,252,57,121,229,202,149,232,239,239,55,20,0,65,13,192,86,115,230,52,84,95,84,247,246,246,138,106,0,65,13,192,86,27,28,28,116,230,52,84,153,249,249,
249,200,100,50,6,1,32,168,1,216,42,67,67,67,145,205,102,197,52,84,137,242,231,232,169,83,167,226,200,145,35,134,2,32,168,1,216,10,206,156,134,234,214,218,218,26,185,92,206,32,0,4,53,0,155,233,232,209,
163,206,156,134,42,84,254,92,189,121,243,102,116,119,119,27,10,128,160,6,96,179,228,243,249,104,109,109,53,8,168,129,168,30,25,25,137,142,142,14,67,1,16,212,0,108,134,206,206,78,103,78,67,141,88,92,92,
140,36,73,162,88,44,26,6,128,160,6,96,35,117,117,117,197,232,232,168,152,134,42,87,254,220,253,234,171,175,150,60,175,1,16,212,0,172,51,103,78,67,237,70,117,87,87,87,140,140,140,24,10,128,160,6,96,35,
12,14,14,198,133,11,23,86,252,97,28,168,110,179,179,179,246,70,0,16,212,0,108,4,103,78,67,109,42,127,46,23,139,197,104,110,110,54,20,0,65,13,192,122,114,230,52,212,207,115,125,114,114,210,32,0,4,53,0,
235,193,153,211,80,219,202,159,211,215,174,93,139,227,199,143,27,10,128,160,6,224,113,57,115,26,234,47,170,251,251,251,151,252,138,7,0,130,26,128,71,208,213,213,229,204,105,168,51,11,11,11,145,201,100,
98,122,122,218,48,0,4,53,0,143,26,211,229,199,232,136,105,168,109,229,207,241,179,103,207,70,46,151,51,20,0,65,13,192,195,114,230,52,136,234,182,182,182,24,31,31,55,20,0,65,13,192,195,24,26,26,114,230,
52,212,185,219,183,111,71,71,71,135,65,0,8,106,0,30,38,166,123,122,122,196,52,212,169,242,231,252,196,196,68,180,180,180,24,10,128,160,6,96,45,90,90,90,156,57,13,220,151,78,167,99,106,106,202,32,0,4,53,
0,223,230,227,143,63,142,79,62,249,228,254,199,86,167,161,62,149,63,247,47,95,190,28,131,131,131,134,2,32,168,1,88,141,51,167,129,213,162,58,155,205,138,106,0,65,13,192,106,186,186,186,226,250,245,235,
43,254,48,13,212,183,187,119,239,70,58,157,54,8,0,65,13,192,74,49,237,204,105,224,65,229,215,130,79,63,253,52,62,254,248,99,67,1,16,212,0,220,83,42,149,156,57,13,172,73,38,147,137,92,46,103,16,0,130,26,
128,8,103,78,3,223,174,252,154,112,243,230,205,37,199,234,1,32,168,1,234,214,240,240,112,116,119,119,139,105,96,205,81,61,60,60,28,157,157,157,134,2,32,168,1,234,91,38,147,113,230,52,240,80,22,23,23,35,
157,78,71,169,84,50,12,0,65,13,80,159,142,29,59,230,204,105,96,205,202,175,17,23,46,92,136,209,209,81,67,1,16,212,0,245,39,159,207,71,75,75,139,65,0,143,28,213,157,157,157,75,78,7,0,64,80,3,212,133,238,
238,110,103,78,3,143,101,118,118,54,218,218,218,12,2,64,80,3,212,87,76,15,15,15,139,105,224,145,148,95,51,166,166,166,34,149,74,25,10,128,160,6,168,125,165,82,41,82,169,148,51,167,129,117,147,201,100,
98,114,114,210,32,0,4,53,64,109,115,230,52,176,30,202,175,29,87,175,94,141,190,190,62,67,1,16,212,0,181,107,120,120,56,122,122,122,196,52,176,238,81,221,223,223,31,199,143,31,55,20,0,65,13,80,155,50,153,
76,204,206,206,26,4,176,238,230,231,231,35,73,146,152,158,158,54,12,0,65,13,80,91,82,169,148,51,167,129,117,87,126,45,57,123,246,108,228,243,121,67,1,16,212,0,181,99,114,114,50,50,153,140,65,0,27,30,213,
109,109,109,49,62,62,110,40,0,130,26,160,54,116,117,117,197,213,171,87,87,252,225,23,96,61,205,204,204,68,103,103,167,65,0,8,106,128,234,151,205,102,99,100,100,68,76,3,27,170,252,218,50,62,62,30,173,173,
173,134,2,32,168,1,170,215,189,51,167,23,22,22,12,3,216,84,153,76,38,138,197,162,65,0,8,106,128,234,52,60,60,28,231,207,159,191,255,177,213,105,96,35,149,95,99,46,94,188,24,131,131,131,134,2,32,168,1,
170,207,232,232,168,51,167,129,45,141,234,108,54,43,170,1,4,53,64,245,73,167,211,113,231,206,29,131,0,182,204,220,220,156,19,6,0,4,53,64,117,73,165,82,49,61,61,125,255,99,171,211,192,102,42,191,230,156,
60,121,50,142,29,59,102,40,0,130,26,160,242,21,10,5,43,66,64,69,73,167,211,145,203,229,12,2,64,80,3,84,54,103,78,3,149,160,252,218,115,243,230,205,200,102,179,134,2,176,1,158,52,2,128,245,145,205,102,
99,120,120,88,76,3,155,226,198,141,27,79,172,245,207,14,13,13,69,79,79,79,236,219,183,207,224,0,4,53,64,101,41,149,74,145,36,201,146,51,167,31,230,135,93,54,142,23,54,170,39,250,216,56,139,139,139,145,
74,165,226,217,103,159,141,23,94,120,193,64,0,214,137,183,124,3,172,131,145,145,145,56,119,238,156,65,8,58,204,126,203,61,249,228,147,241,244,211,79,47,251,223,237,219,183,227,147,79,62,49,32,128,245,
188,230,26,1,192,227,201,229,114,49,49,49,17,207,60,243,140,97,84,144,187,119,239,198,141,27,55,12,162,2,236,216,177,35,158,124,114,245,31,57,158,122,234,169,111,253,255,255,205,223,252,77,108,219,182,
237,145,111,223,190,125,123,124,239,123,223,251,214,0,253,182,207,239,123,223,251,94,108,223,190,125,197,255,254,224,255,239,137,39,158,88,241,235,89,233,115,216,182,109,91,252,205,223,252,205,154,190,
158,109,219,182,173,248,57,60,253,244,211,241,252,243,207,123,144,1,108,145,39,22,23,189,19,14,128,218,211,221,221,29,255,246,111,255,118,47,232,124,179,219,2,247,86,168,255,229,95,254,37,14,30,60,104,
32,0,212,28,111,249,6,0,0,0,65,13,0,0,0,130,26,0,0,0,4,53,0,0,0,8,106,0,0,0,16,212,0,0,0,128,160,6,0,0,0,65,13,0,0,0,130,26,0,0,0,4,53,0,0,0,8,106,0,0,0,64,80,3,0,0,128,160,6,0,0,0,65,13,0,0,0,130,26,
0,0,0,4,53,0,0,0,32,168,1,0,0,64,80,3,0,0,128,160,6,0,0,0,65,13,0,0,0,130,26,0,0,0,16,212,0,0,0,32,168,1,0,0,64,80,3,0,0,128,160,6,0,0,0,65,13,0,0,0,8,106,0,0,0,16,212,0,0,0,32,168,1,0,0,64,80,3,0,0,128,
160,6,0,0,0,4,53,0,0,0,8,106,0,0,0,16,212,0,0,0,32,168,1,0,0,64,80,3,0,0,0,130,26,0,0,0,4,53,0,0,0,8,106,0,0,0,16,212,0,0,0,32,168,1,0,0,0,65,13,0,0,0,130,26,0,0,0,4,53,0,0,0,8,106,0,0,0,16,212,0,0,0,
128,160,6,0,0,0,65,13,0,0,0,130,26,0,0,0,4,53,0,0,0,8,106,0,0,0,64,80,3,0,0,128,160,6,0,0,0,65,13,0,0,0,130,26,0,0,0,4,53,0,0,0,32,168,1,0,0,64,80,3,0,0,128,160,6,0,0,0,65,13,0,0,0,130,26,0,0,0,16,212,
0,0,0,32,168,1,0,0,64,80,3,0,0,128,160,6,0,0,0,65,13,0,0,0,8,106,0,0,0,16,212,0,0,0,32,168,1,0,0,64,80,3,0,0,128,160,6,0,0,0,4,53,0,0,0,8,106,0,0,0,16,212,0,0,0,32,168,1,0,0,64,80,3,0,0,0,130,26,0,0,0,
4,53,0,0,0,8,106,0,0,0,16,212,0,0,0,32,168,1,0,0,0,65,13,0,0,0,130,26,0,0,0,4,53,0,0,0,8,106,0,0,0,16,212,0,0,0,128,160,6,0,0,0,65,13,0,0,0,130,26,0,0,0,4,53,0,0,0,8,106,0,0,0,16,212,0,0,0,128,160,6,0,
0,0,65,13,0,0,0,130,26,0,0,0,4,53,0,0,0,8,106,0,0,0,64,80,3,0,64,181,200,229,114,134,0,130,26,216,12,165,82,41,142,30,61,106,16,0,80,35,254,235,191,254,43,254,248,199,63,70,169,84,50,12,16,212,192,70,
233,233,233,137,143,62,250,40,58,59,59,13,3,0,106,196,237,219,183,35,157,78,199,191,255,251,191,71,119,119,183,129,64,21,121,210,8,160,242,21,10,133,200,102,179,209,223,223,31,139,139,139,241,247,127,
255,247,134,2,0,53,230,194,133,11,241,31,255,241,31,145,207,231,227,192,129,3,241,242,203,47,27,10,8,106,224,113,52,55,55,71,38,147,137,107,215,174,221,255,111,139,139,139,6,3,0,53,104,97,97,33,134,134,
134,226,147,79,62,137,198,198,198,104,106,106,50,20,16,212,192,195,26,29,29,141,142,142,142,200,231,243,43,126,179,5,0,106,215,213,171,87,227,79,127,250,83,140,140,140,68,99,99,99,236,222,189,219,80,64,
80,3,223,101,122,122,58,198,198,198,162,189,189,61,102,103,103,87,252,51,86,168,1,160,54,237,216,177,99,241,198,141,27,79,220,251,248,196,137,19,113,246,236,217,40,149,74,177,123,247,238,120,225,133,23,
12,9,4,53,176,146,108,54,27,73,146,196,185,115,231,190,245,27,172,160,6,128,250,137,234,59,119,238,68,107,107,107,228,243,249,56,124,248,112,236,223,191,223,144,64,80,3,247,20,10,133,232,235,235,139,190,
190,190,152,159,159,95,246,77,245,193,63,239,45,223,0,80,251,81,29,17,81,30,214,54,45,3,65,13,60,32,149,74,69,38,147,137,171,87,175,126,103,72,223,99,133,26,0,234,51,172,109,90,6,130,26,136,111,223,116,
236,219,98,90,80,3,64,125,134,117,249,106,181,77,203,64,80,67,93,154,158,158,142,124,62,31,109,109,109,49,51,51,243,80,33,125,143,183,124,3,64,125,70,117,196,210,183,129,219,180,12,4,53,212,141,254,254,
254,72,146,36,206,156,57,179,234,55,201,181,176,66,13,0,245,29,214,54,45,3,65,13,117,227,97,55,29,251,46,86,168,1,64,84,71,216,180,12,4,53,212,184,214,214,214,72,146,36,46,95,190,252,216,33,93,174,84,
42,121,91,23,0,8,107,155,150,129,160,134,218,51,49,49,17,157,157,157,49,54,54,182,234,55,191,199,225,109,223,0,64,249,207,22,54,45,3,65,13,85,111,61,54,29,91,203,55,75,65,13,0,172,244,115,134,77,203,64,
80,67,85,234,239,239,143,116,58,29,167,79,159,94,245,155,220,122,241,123,212,0,192,106,97,109,211,50,16,212,80,53,138,197,98,12,14,14,70,119,119,247,186,108,58,182,22,86,168,1,128,111,139,234,8,155,150,
129,160,134,10,215,214,214,22,73,146,196,165,75,151,54,37,164,239,177,66,13,0,60,108,88,219,180,12,4,53,84,132,141,222,116,236,187,88,161,6,0,30,38,172,109,90,6,130,26,42,194,209,163,71,35,147,201,196,
173,91,183,54,61,164,5,53,0,240,168,81,29,97,211,50,16,212,176,69,6,6,6,34,157,78,199,169,83,167,86,253,38,181,89,188,229,27,0,120,212,176,182,105,25,8,106,216,52,247,54,29,235,233,233,137,187,119,239,
110,105,72,223,99,133,26,0,120,156,168,142,176,105,25,8,106,216,96,237,237,237,145,36,73,92,188,120,113,213,111,70,130,26,0,168,133,176,182,105,25,8,106,88,23,185,92,46,122,122,122,98,120,120,120,89,188,
110,101,72,223,227,45,223,0,192,122,134,181,77,203,64,80,195,186,168,132,77,199,4,53,0,176,217,81,29,97,211,50,16,212,240,136,6,7,7,35,157,78,199,167,159,126,186,234,55,153,74,225,45,223,0,192,70,133,
181,77,203,64,80,195,154,21,139,197,24,25,25,137,174,174,174,152,155,155,171,232,144,22,212,0,192,102,68,117,132,77,203,64,80,195,119,232,238,238,142,84,42,21,23,46,92,88,245,155,73,37,242,150,111,0,96,
179,195,218,166,101,32,168,33,34,42,127,211,177,239,98,133,26,0,216,204,176,182,105,25,8,106,136,136,136,230,230,230,72,167,211,113,253,250,245,170,11,233,123,172,80,3,0,155,29,213,17,54,45,3,65,77,221,
26,25,25,137,150,150,150,40,149,74,171,126,147,168,22,86,168,1,128,173,10,107,155,150,129,160,166,142,84,227,166,99,130,26,0,168,228,168,142,176,105,25,8,106,106,94,79,79,79,164,82,169,248,226,139,47,
86,253,102,80,141,188,229,27,0,168,180,176,182,105,25,130,26,106,196,228,228,100,244,246,246,70,127,127,127,85,110,58,246,93,172,80,3,0,149,20,214,54,45,67,80,67,141,104,110,110,142,76,38,19,215,174,93,
171,185,144,190,199,10,53,0,80,105,81,29,97,211,50,4,53,84,173,145,145,145,104,109,109,141,98,177,184,234,69,190,86,88,161,6,0,42,53,172,109,90,134,160,134,42,82,42,149,98,124,124,60,218,219,219,99,118,
118,182,166,67,250,30,43,212,0,64,37,71,117,132,77,203,16,212,80,241,178,217,108,164,82,169,56,127,254,252,170,23,243,90,100,133,26,0,168,182,176,182,105,25,130,26,42,68,161,80,136,108,54,91,179,155,142,
9,106,0,160,150,194,218,166,101,8,106,168,16,169,84,42,210,233,116,77,111,58,246,93,188,229,27,0,168,182,168,142,176,105,25,130,26,182,204,200,200,72,180,180,180,68,169,84,90,245,34,93,47,172,80,3,0,213,
26,214,43,109,90,54,53,53,21,135,15,31,142,183,222,122,203,144,16,212,176,158,166,167,167,99,108,108,172,174,54,29,251,46,86,168,1,128,106,142,234,136,165,171,213,231,206,157,139,15,63,252,48,198,199,
199,109,90,134,160,134,245,146,205,102,35,73,146,56,119,238,220,170,23,227,122,100,133,26,0,168,181,176,182,105,25,130,26,214,73,161,80,136,190,190,190,232,235,235,139,249,249,121,33,45,168,1,128,26,14,
235,213,54,45,107,106,106,138,87,95,125,213,144,16,212,176,86,45,45,45,145,36,73,92,185,114,69,72,175,194,91,190,1,128,90,139,234,136,229,155,150,253,246,183,191,141,125,251,246,217,180,12,65,13,223,229,
222,239,73,231,243,249,85,47,178,124,195,10,53,0,80,171,97,189,210,166,101,197,98,49,154,154,154,108,90,134,160,134,7,77,79,79,71,62,159,143,182,182,182,152,153,153,17,210,107,96,133,26,0,168,229,168,
142,88,186,90,253,249,231,159,199,135,31,126,24,147,147,147,177,111,223,190,120,233,165,151,12,10,65,13,253,117,170,252,20,0,0,32,0,73,68,65,84,253,253,145,36,73,156,57,115,102,213,139,41,203,89,161,6,
0,234,45,172,23,22,22,162,175,175,47,138,197,98,52,54,54,70,99,99,163,33,33,168,169,79,54,29,19,212,0,0,107,13,235,242,213,234,175,191,254,58,254,248,199,63,198,248,248,120,52,54,54,198,43,175,188,98,
72,8,106,234,71,107,107,107,36,73,18,151,47,95,22,210,143,200,91,190,1,128,122,139,234,136,165,111,3,47,22,139,113,250,244,233,56,120,240,96,236,218,181,43,158,127,254,121,131,66,80,83,187,38,38,38,162,
179,179,51,198,198,198,86,189,72,178,54,86,168,1,128,122,13,235,242,168,190,125,251,118,36,73,18,147,147,147,209,212,212,20,123,247,238,53,36,4,53,181,197,166,99,130,26,0,96,61,163,58,98,233,106,245,217,
179,103,227,119,191,251,93,228,243,121,155,150,33,168,169,29,3,3,3,145,36,73,156,62,125,122,213,139,33,130,26,0,224,113,195,250,222,166,101,165,82,41,26,27,27,163,161,161,193,144,16,212,84,167,98,177,
24,131,131,131,209,221,221,109,211,49,65,13,0,176,161,97,93,190,90,125,229,202,149,248,195,31,254,16,249,124,62,222,125,247,93,155,150,33,168,169,46,109,109,109,145,36,73,92,186,116,73,72,11,106,0,128,
77,137,234,136,165,111,3,207,231,243,113,226,196,9,155,150,33,168,169,14,54,29,19,212,0,0,91,29,214,43,109,90,86,40,20,162,169,169,41,222,124,243,77,67,66,80,83,121,142,30,61,26,153,76,38,110,221,186,
37,164,5,53,0,192,150,70,117,196,210,213,234,207,62,251,44,62,252,240,195,40,20,10,177,119,239,94,155,150,33,168,169,12,3,3,3,145,78,167,227,212,169,83,171,94,204,0,0,96,171,195,250,238,221,187,145,205,
102,239,175,86,191,247,222,123,134,132,160,102,107,220,219,116,172,167,167,39,238,222,189,43,164,55,153,21,106,0,128,181,135,245,131,155,150,253,254,247,191,143,92,46,103,211,50,4,53,155,175,189,189,61,
146,36,137,139,23,47,174,120,193,50,33,65,13,0,80,105,81,29,177,124,211,178,83,167,78,197,161,67,135,226,131,15,62,48,36,4,53,27,43,151,203,69,79,79,79,12,15,15,47,11,58,33,45,168,1,0,170,33,172,203,163,
250,230,205,155,113,236,216,177,152,154,154,138,198,198,198,120,237,181,215,12,9,65,205,250,107,110,110,142,36,73,226,198,141,27,66,90,80,3,0,84,117,84,71,44,93,173,62,121,242,100,252,235,191,254,107,
76,76,76,216,180,12,65,205,250,25,28,28,140,116,58,29,159,126,250,233,170,23,35,4,53,0,64,53,134,117,121,84,223,219,180,108,122,122,58,154,154,154,226,208,161,67,134,132,160,230,209,20,139,197,24,25,25,
137,174,174,174,152,155,155,19,210,130,26,0,160,38,163,58,98,233,106,245,87,95,125,21,255,249,159,255,25,185,92,46,222,121,231,157,216,185,115,167,65,33,168,89,187,238,238,238,72,165,82,113,225,194,133,
85,47,58,8,106,0,128,90,13,235,197,197,197,24,27,27,139,79,62,249,196,166,101,8,106,214,198,166,99,130,26,0,160,222,195,122,165,77,203,138,197,98,52,52,52,216,180,12,65,205,202,154,155,155,35,157,78,199,
245,235,215,133,180,160,6,0,168,235,168,142,88,250,54,240,19,39,78,196,153,51,103,98,106,106,42,246,236,217,19,47,188,240,130,65,33,168,137,24,25,25,137,150,150,150,40,149,74,171,94,76,16,212,0,0,245,
24,214,229,81,61,55,55,23,29,29,29,81,40,20,162,169,169,41,14,30,60,104,72,130,154,122,101,211,49,65,13,0,192,119,71,117,196,210,213,234,47,191,252,50,62,250,232,163,152,152,152,176,105,153,160,166,30,
245,244,244,68,42,149,138,47,190,248,98,213,139,6,130,26,0,128,149,195,250,222,166,101,39,78,156,136,247,222,123,47,126,243,155,223,24,146,160,166,214,77,78,78,70,111,111,111,244,247,247,219,116,76,80,
3,0,240,8,97,93,190,90,125,227,198,141,248,243,159,255,28,185,92,46,26,27,27,99,247,238,221,134,36,168,169,69,205,205,205,145,201,100,226,218,181,107,66,90,80,3,0,240,24,81,253,191,49,189,108,211,178,
66,161,96,211,50,65,77,45,25,25,25,137,214,214,214,40,22,139,171,94,12,16,212,0,0,60,124,88,175,180,105,217,212,212,84,52,53,53,197,129,3,7,12,73,80,83,173,74,165,82,140,143,143,71,123,123,123,204,206,
206,10,105,65,13,0,192,6,68,117,196,210,213,234,11,23,46,196,71,31,125,20,147,147,147,113,224,192,129,120,249,229,151,13,74,80,83,77,108,58,38,168,1,0,216,186,176,94,88,88,136,161,161,161,248,228,147,
79,162,177,177,49,154,154,154,12,73,80,83,233,10,133,66,244,245,245,197,241,227,199,99,97,97,65,72,3,0,192,38,135,117,249,106,245,213,171,87,227,79,127,250,83,140,140,140,216,180,76,80,83,201,82,169,84,
100,50,153,184,122,245,170,144,174,3,86,168,1,0,42,55,170,35,150,111,90,118,246,236,217,40,149,74,177,123,247,110,155,150,9,106,42,197,232,232,104,116,116,116,68,62,159,95,245,201,140,160,6,0,96,243,195,
186,60,170,239,220,185,115,127,179,224,166,166,166,120,235,173,183,12,73,80,179,85,166,167,167,35,159,207,71,91,91,91,204,204,204,8,105,65,13,0,64,5,70,117,196,210,213,234,207,63,255,60,62,252,240,195,
24,31,31,183,105,153,160,102,43,244,247,247,71,146,36,113,230,204,153,85,159,180,8,106,0,0,42,51,172,239,109,90,118,226,196,137,104,108,108,140,198,198,70,67,18,212,108,180,169,169,169,56,126,252,120,
244,245,245,197,252,252,188,144,174,99,15,110,58,7,0,64,117,132,117,249,106,245,215,95,127,29,127,252,227,31,99,124,124,60,26,26,26,98,215,174,93,134,36,168,217,8,109,109,109,145,74,165,226,242,229,203,
66,26,0,0,170,56,170,35,150,190,13,188,88,44,198,169,83,167,98,223,190,125,54,45,19,212,172,167,137,137,137,232,236,236,140,177,177,177,85,159,140,212,31,111,249,6,0,168,254,176,94,105,211,178,82,169,
20,77,77,77,177,119,239,94,67,18,212,60,142,163,71,143,70,38,147,137,91,183,110,9,105,4,53,0,64,13,70,117,196,210,213,234,179,103,207,198,239,126,247,187,200,231,243,177,111,223,190,120,233,165,151,12,
74,80,243,48,6,6,6,34,157,78,199,169,83,167,86,125,210,33,168,1,0,168,205,176,94,88,88,136,190,190,190,40,149,74,209,216,216,24,13,13,13,134,36,168,249,46,197,98,49,70,70,70,162,171,171,43,230,230,230,
132,52,130,26,0,160,206,194,186,124,181,250,202,149,43,241,135,63,252,33,242,249,124,188,251,238,187,241,202,43,175,24,146,160,102,37,29,29,29,145,74,165,226,226,197,139,43,62,177,76,8,65,13,0,80,31,81,
29,177,244,109,224,249,124,62,78,156,56,17,7,15,30,140,93,187,118,197,243,207,63,111,80,130,154,136,136,92,46,23,61,61,61,49,60,60,188,44,146,132,52,130,26,0,160,126,195,186,60,170,111,223,190,29,73,146,
68,161,80,136,166,166,166,120,243,205,55,13,73,80,215,183,230,230,230,72,167,211,113,253,250,117,33,141,160,6,0,96,197,46,40,15,235,207,62,251,44,62,252,240,195,40,20,10,177,119,239,94,155,150,9,234,250,
51,50,50,18,45,45,45,81,42,149,86,125,210,128,160,6,0,96,165,176,190,123,247,110,100,179,217,251,171,213,239,189,247,158,33,9,234,218,87,42,149,98,124,124,60,218,219,219,99,118,118,86,72,35,168,1,0,120,
168,176,126,112,211,178,223,255,254,247,145,203,229,108,90,38,168,107,91,79,79,79,36,73,18,231,207,159,95,241,137,97,66,8,106,0,0,214,18,213,17,203,55,45,59,117,234,84,28,58,116,40,62,248,224,3,67,18,
212,181,163,80,40,68,54,155,141,254,254,126,155,142,33,168,1,0,88,183,176,46,143,234,155,55,111,198,177,99,199,34,151,203,69,83,83,83,188,241,198,27,134,36,168,171,91,42,149,138,116,58,29,215,174,93,19,
210,0,0,192,186,71,117,196,210,213,234,51,103,206,196,239,126,247,187,152,156,156,180,105,153,160,174,78,163,163,163,209,209,209,17,249,124,126,213,7,61,60,14,43,212,0,0,148,55,70,121,84,223,219,180,108,
122,122,58,154,154,154,226,208,161,67,134,36,168,43,223,244,244,116,228,243,249,104,109,109,141,219,183,111,11,105,54,204,194,194,130,33,0,0,176,172,55,202,195,250,171,175,190,138,143,62,250,40,70,71,
71,227,151,191,252,101,252,195,63,252,131,65,9,234,202,212,219,219,27,169,84,42,206,157,59,183,234,131,27,214,139,21,106,0,0,214,26,214,54,45,19,212,21,171,80,40,68,95,95,95,244,245,245,197,252,252,188,
144,70,80,3,0,80,17,97,189,210,166,101,83,83,83,209,216,216,24,175,189,246,154,33,9,234,173,213,218,218,26,73,146,196,229,203,151,133,52,130,26,0,128,138,139,234,136,165,171,213,39,79,158,140,223,254,
246,183,49,53,53,21,175,191,254,122,188,248,226,139,6,37,168,55,215,196,196,68,116,118,118,198,216,216,216,170,15,90,0,0,128,74,9,235,242,168,158,155,155,139,142,142,142,40,20,10,209,212,212,20,7,15,30,
52,36,65,189,241,166,167,167,35,151,203,69,123,123,123,204,204,204,8,105,182,204,220,220,92,116,119,119,27,4,172,226,210,165,75,134,80,33,46,95,190,236,122,5,223,226,193,95,25,132,141,140,234,136,165,
171,213,95,126,249,101,124,244,209,71,81,40,20,98,255,254,253,177,115,231,78,131,122,8,79,120,219,232,218,13,12,12,68,58,157,142,83,167,78,173,250,224,132,141,86,126,1,4,30,238,7,8,92,175,192,245,10,86,
191,78,239,216,177,35,222,123,239,189,248,205,111,126,99,56,130,122,253,20,139,197,24,28,28,140,158,158,158,184,123,247,174,139,30,126,72,5,63,156,226,122,5,174,87,212,236,117,250,185,231,158,139,134,
134,6,155,150,9,234,199,215,222,222,30,73,146,196,197,139,23,93,240,0,0,128,186,8,235,237,219,183,199,219,111,191,29,123,246,236,137,23,94,120,193,128,4,245,195,201,229,114,209,211,211,19,195,195,195,
203,118,83,22,211,0,0,64,173,71,117,68,196,79,126,242,147,104,106,106,138,3,7,14,24,144,160,94,155,230,230,230,72,167,211,113,253,250,117,33,13,0,0,212,117,88,111,219,182,45,94,125,245,213,56,112,224,
64,188,252,242,203,6,36,168,87,54,50,50,18,153,76,38,166,167,167,151,221,38,166,1,0,128,122,14,235,29,59,118,68,67,67,67,188,255,254,251,134,35,168,255,79,177,88,140,145,145,145,232,234,234,138,185,185,
57,33,13,0,0,16,171,111,90,214,216,216,24,187,119,239,22,212,245,30,212,61,61,61,145,74,165,226,139,47,190,88,118,155,152,6,0,0,88,30,214,79,61,245,84,236,219,183,47,118,239,222,93,215,155,150,213,109,
80,23,10,133,200,102,179,209,223,223,111,211,49,0,0,128,135,140,234,136,111,54,45,59,124,248,112,236,223,191,95,80,215,139,230,230,230,200,100,50,113,237,218,53,33,13,0,0,240,24,97,93,207,155,150,213,
85,80,143,142,142,70,71,71,71,228,243,249,101,183,137,105,0,0,128,71,15,235,103,158,121,38,26,27,27,163,177,177,81,80,215,146,233,233,233,24,27,27,139,246,246,246,152,157,157,21,210,0,0,0,27,16,213,17,
17,47,190,248,98,52,52,52,196,174,93,187,4,117,181,235,237,237,141,84,42,21,231,206,157,91,118,155,152,6,0,0,88,255,176,174,151,77,203,106,54,168,11,133,66,244,245,245,69,95,95,95,204,207,207,11,105,0,
0,128,77,140,234,136,136,159,253,236,103,209,216,216,24,111,189,245,150,160,174,22,173,173,173,145,36,73,92,190,124,89,72,3,0,0,108,97,88,111,219,182,45,246,236,217,19,251,246,237,139,151,94,122,73,80,
87,170,137,137,137,232,236,236,140,177,177,177,101,183,137,105,0,0,128,173,11,235,90,220,180,172,38,130,122,122,122,58,242,249,124,180,181,181,197,204,204,140,144,6,0,0,168,192,168,142,248,102,211,178,
198,198,198,120,229,149,87,4,245,86,27,24,24,136,36,73,226,244,233,211,203,110,19,211,0,0,0,149,23,214,79,63,253,116,28,60,120,48,118,237,218,21,207,63,255,188,160,222,108,197,98,49,6,7,7,163,167,167,
39,238,222,189,43,164,1,0,0,170,40,170,35,190,217,180,236,240,225,195,241,230,155,111,10,234,205,210,222,222,30,169,84,42,46,93,186,36,164,1,0,0,170,56,172,159,124,242,201,120,227,141,55,98,239,222,189,
85,183,105,89,85,5,117,46,151,139,158,158,158,24,30,30,142,7,63,111,49,13,0,0,80,189,97,253,131,31,252,32,154,154,154,226,189,247,222,19,212,235,173,185,185,57,146,36,137,27,55,110,8,105,0,0,128,26,140,
234,136,136,157,59,119,198,187,239,190,91,21,155,150,85,124,80,15,14,14,70,58,157,142,79,63,253,116,217,109,98,26,0,0,160,246,194,250,251,223,255,126,28,58,116,40,62,248,224,3,65,253,40,138,197,98,140,
140,140,68,87,87,87,204,205,205,9,105,0,0,128,58,138,234,136,136,95,252,226,23,209,216,216,24,175,189,246,154,160,94,171,158,158,158,72,165,82,241,197,23,95,44,187,77,76,3,0,0,212,79,88,87,242,166,101,
21,21,212,147,147,147,209,219,219,27,253,253,253,54,29,3,0,0,16,213,247,61,251,236,179,113,248,240,225,56,120,240,160,160,126,208,95,255,250,215,200,100,50,113,253,250,117,33,13,0,0,192,178,176,126,226,
137,39,98,247,238,221,177,127,255,254,216,185,115,167,160,30,29,29,141,150,150,150,40,22,139,203,110,19,211,0,0,0,60,24,214,127,247,119,127,23,13,13,13,241,235,95,255,186,62,131,122,122,122,58,242,249,
124,180,181,181,197,204,204,140,144,6,0,0,96,205,81,29,17,241,220,115,207,69,99,99,99,236,222,189,187,126,130,122,96,96,32,146,36,137,211,167,79,47,187,77,76,3,0,0,176,214,176,126,234,169,167,98,223,190,
125,177,123,247,238,120,225,133,23,106,55,168,29,133,5,0,0,192,122,71,117,68,196,207,126,246,179,104,106,106,138,189,123,247,214,94,80,247,244,244,68,115,115,115,92,184,112,65,72,3,0,0,176,238,97,189,
217,71,108,109,120,80,23,10,133,200,102,179,142,194,2,0,0,96,83,194,250,217,103,159,141,166,166,166,56,116,232,80,245,6,117,146,36,145,36,73,92,187,118,77,72,3,0,0,176,105,81,253,196,19,79,196,43,175,
188,18,239,188,243,206,134,29,177,181,33,65,61,49,49,17,157,157,157,49,54,54,182,236,54,49,13,0,0,192,102,133,245,142,29,59,162,161,161,33,222,127,255,253,202,15,234,163,71,143,70,38,147,137,91,183,110,
9,105,0,0,0,182,60,170,35,34,94,124,241,197,104,104,104,136,93,187,118,85,94,80,15,14,14,70,146,36,113,234,212,169,101,183,137,105,0,0,0,182,58,172,159,126,250,233,56,120,240,96,236,218,181,43,158,127,
254,249,173,15,106,71,97,1,0,0,80,45,81,29,17,241,243,159,255,60,14,31,62,28,111,188,241,198,214,5,245,106,71,97,137,105,0,0,0,42,57,172,183,111,223,30,251,247,239,143,215,94,123,45,94,120,225,133,205,
11,234,66,161,16,125,125,125,209,215,215,23,243,243,243,66,26,0,0,128,170,12,235,31,255,248,199,241,235,95,255,58,246,237,219,183,241,65,221,214,214,22,169,84,42,46,95,190,44,164,1,0,0,168,250,168,222,
182,109,91,236,217,179,39,246,237,219,23,47,189,244,210,250,7,117,46,151,139,142,142,14,71,97,1,0,0,80,147,97,253,236,179,207,198,175,126,245,171,56,112,224,192,250,5,245,145,35,71,162,165,165,197,81,
88,0,0,0,212,116,84,111,219,182,45,222,124,243,205,120,235,173,183,190,115,181,250,91,131,122,116,116,52,50,153,76,148,74,165,101,183,137,105,0,0,0,106,53,172,127,244,163,31,197,175,126,245,171,56,120,
240,224,195,7,245,209,163,71,35,157,78,199,204,204,140,144,6,0,0,160,238,162,250,137,39,158,136,87,94,121,37,222,125,247,221,120,249,229,151,191,59,168,135,134,134,34,149,74,197,169,83,167,150,253,97,
49,13,0,0,64,189,133,245,51,207,60,19,135,15,31,142,247,222,123,111,229,160,158,158,158,142,177,177,177,104,111,111,143,217,217,89,33,13,0,0,128,168,46,179,107,215,174,248,229,47,127,25,255,239,255,253,
191,255,11,234,161,161,161,56,118,236,88,124,246,217,103,203,254,18,49,13,0,0,128,176,254,198,143,126,244,163,248,199,127,252,199,120,251,237,183,227,137,163,71,143,70,42,149,138,219,183,111,11,105,0,
0,0,248,142,168,254,222,247,190,23,141,141,141,177,109,110,110,46,22,22,22,76,8,0,0,0,214,96,219,182,109,49,59,59,251,205,91,190,7,6,6,226,227,143,63,142,115,231,206,45,251,131,86,170,1,0,0,168,87,15,
174,78,255,244,167,63,141,247,223,127,63,246,236,217,243,127,155,146,149,74,165,24,31,31,143,182,182,182,152,155,155,19,213,0,0,0,8,233,255,181,125,251,246,120,251,237,183,99,207,158,61,241,194,11,47,
68,132,99,179,0,0,0,96,213,144,142,136,248,249,207,127,30,135,15,31,142,55,222,120,99,201,127,95,22,212,247,28,61,122,52,210,233,116,204,204,204,136,106,0,0,0,234,46,166,159,122,234,169,56,116,232,80,
236,218,181,43,158,127,254,249,101,127,126,213,160,142,136,251,231,82,231,243,249,101,183,9,107,0,0,0,106,49,164,35,34,158,123,238,185,104,106,106,138,87,95,125,117,213,255,223,183,6,245,61,199,142,29,
139,76,38,19,55,110,220,16,213,0,0,0,212,108,76,239,216,177,35,26,26,26,226,253,247,223,255,206,255,239,154,130,58,34,98,114,114,50,122,123,123,163,191,191,63,30,252,255,8,107,0,0,0,170,57,164,35,34,118,
237,218,21,239,188,243,78,236,220,185,115,77,127,199,154,131,250,158,238,238,238,72,165,82,113,225,194,133,101,183,9,107,0,0,0,170,45,166,159,125,246,217,104,106,106,138,67,135,14,61,212,223,243,208,65,
29,17,81,44,22,99,100,100,36,186,186,186,28,177,5,0,0,64,85,134,244,147,79,62,25,111,188,241,70,236,221,187,55,94,122,233,165,135,254,251,30,41,168,239,25,28,28,140,116,58,29,159,126,250,233,178,219,132,
53,0,0,0,149,24,210,17,223,28,133,213,212,212,20,111,190,249,230,35,255,189,143,21,212,247,28,61,122,52,50,153,76,220,186,117,75,84,3,0,0,80,177,49,253,244,211,79,199,193,131,7,87,61,10,107,211,131,58,
34,98,98,98,34,58,59,59,99,108,108,108,217,109,194,26,0,0,128,173,12,233,136,136,157,59,119,198,187,239,190,27,175,188,242,202,186,252,27,235,22,212,247,180,181,181,69,146,36,113,233,210,37,81,13,0,0,
192,150,199,244,51,207,60,19,77,77,77,209,208,208,176,174,255,206,186,7,117,196,55,155,150,13,14,14,70,119,119,119,204,207,207,11,107,0,0,0,54,61,164,183,109,219,22,123,246,236,137,125,251,246,61,210,
166,99,91,18,212,247,244,247,247,71,146,36,113,230,204,153,101,183,9,107,0,0,0,54,34,164,35,34,126,250,211,159,70,83,83,83,188,245,214,91,27,246,239,110,104,80,71,68,76,79,79,71,62,159,143,182,182,182,
152,153,153,17,213,0,0,0,108,88,76,63,245,212,83,177,111,223,190,216,189,123,119,188,240,194,11,27,250,111,111,120,80,223,51,58,58,26,29,29,29,145,207,231,151,221,38,172,1,0,0,120,156,144,142,136,120,
238,185,231,162,177,177,49,118,239,222,189,41,159,195,166,5,245,61,169,84,42,210,233,116,92,187,118,77,84,3,0,0,240,216,49,189,99,199,142,104,104,104,136,247,223,127,127,83,63,143,77,15,234,136,136,66,
161,16,217,108,54,250,251,251,227,193,127,95,88,3,0,0,176,150,144,222,182,109,91,188,250,234,171,177,127,255,254,216,185,115,231,166,127,62,91,18,212,247,244,244,244,68,42,149,138,47,190,248,98,217,109,
194,26,0,0,128,149,66,58,34,226,199,63,254,113,28,62,124,56,14,28,56,176,101,159,215,150,6,117,68,68,169,84,138,241,241,241,104,111,111,143,217,217,89,81,13,0,0,192,170,49,189,125,251,246,120,251,237,
183,99,207,158,61,27,190,233,88,197,7,245,61,67,67,67,145,78,167,227,228,201,147,203,110,19,214,0,0,0,245,29,210,17,223,108,58,214,208,208,16,175,189,246,90,69,124,142,21,19,212,247,52,55,55,71,58,157,
142,235,215,175,139,106,0,0,0,49,29,223,255,254,247,227,208,161,67,241,193,7,31,84,212,231,89,113,65,29,17,49,57,57,25,189,189,189,54,45,3,0,0,168,227,144,126,226,137,39,226,149,87,94,137,119,222,121,
103,75,54,29,171,202,160,190,199,166,101,0,0,0,245,23,210,17,223,108,58,214,212,212,20,7,15,30,172,216,207,187,162,131,58,194,166,101,0,0,0,245,20,211,149,180,233,88,213,7,245,61,35,35,35,209,218,218,
26,197,98,113,217,109,194,26,0,0,160,186,67,58,162,242,54,29,171,153,160,190,167,185,185,57,50,153,76,92,187,118,77,84,3,0,0,212,64,76,239,216,177,35,222,123,239,189,248,205,111,126,83,85,95,71,213,5,
117,68,68,161,80,136,108,54,107,211,50,0,0,128,42,14,233,109,219,182,197,171,175,190,26,251,247,239,175,200,77,199,106,50,168,239,201,102,179,145,74,165,226,252,249,243,203,110,19,214,0,0,0,149,25,210,
17,17,63,249,201,79,162,169,169,41,14,28,56,80,181,95,87,85,7,117,132,77,203,0,0,0,170,41,166,159,122,234,169,216,183,111,95,236,222,189,187,226,55,29,171,249,160,190,103,116,116,52,58,58,58,34,159,207,
47,187,77,88,3,0,0,108,109,72,71,124,179,233,88,99,99,99,236,222,189,187,38,190,198,154,9,234,123,82,169,84,100,50,153,184,122,245,170,168,6,0,0,168,128,152,222,177,99,71,52,52,52,196,251,239,191,95,83,
95,103,205,5,117,196,55,155,150,245,245,245,69,95,95,95,204,207,207,11,107,0,0,128,45,8,233,123,155,142,29,56,112,32,94,126,249,229,154,251,122,107,50,168,239,201,102,179,145,36,73,156,59,119,110,217,
109,194,26,0,0,96,99,66,58,226,155,77,199,14,31,62,28,251,247,239,175,217,175,187,166,131,58,34,98,122,122,58,242,249,124,180,181,181,197,204,204,140,168,6,0,0,216,192,152,174,165,77,199,234,62,168,239,
177,105,25,0,0,192,198,133,116,196,55,155,142,53,53,53,197,171,175,190,90,23,51,168,155,160,190,167,165,165,37,210,233,116,92,190,124,89,84,3,0,0,172,67,76,63,243,204,51,209,208,208,16,77,77,77,117,53,
135,186,11,234,8,155,150,1,0,0,172,71,72,215,250,166,99,130,250,91,244,247,247,71,146,36,113,230,204,153,101,183,9,107,0,0,128,149,67,58,34,226,103,63,251,89,52,54,54,198,91,111,189,85,183,115,169,235,
160,142,176,105,25,0,0,192,195,196,116,61,109,58,38,168,215,104,98,98,34,58,59,59,99,108,108,108,217,109,194,26,0,0,168,247,144,142,136,120,241,197,23,163,161,161,33,118,237,218,101,64,130,122,185,214,
214,214,72,146,196,166,101,0,0,128,152,254,95,207,60,243,76,52,54,54,70,99,99,163,225,8,234,111,55,53,53,21,199,143,31,183,105,25,0,0,80,215,33,189,109,219,182,216,179,103,79,236,219,183,47,94,122,233,
37,3,18,212,107,215,223,223,31,233,116,58,78,159,62,189,236,54,97,13,0,0,212,106,72,71,124,179,233,88,83,83,83,236,221,187,215,128,4,245,163,153,158,158,142,92,46,23,237,237,237,54,45,3,0,0,106,62,166,
255,246,111,255,54,222,121,231,157,216,185,115,103,60,255,252,243,6,36,168,31,223,196,196,68,100,179,217,24,30,30,142,7,103,38,172,217,234,139,30,176,58,215,104,215,43,112,189,130,181,95,163,119,238,220,
25,239,190,251,110,188,242,202,43,6,36,168,215,95,123,123,123,36,73,18,23,47,94,116,17,196,15,167,224,135,84,92,175,192,245,138,154,184,62,255,224,7,63,136,166,166,166,120,239,189,247,12,72,80,111,172,
98,177,24,3,3,3,209,219,219,27,115,115,115,46,130,108,201,5,112,199,142,29,241,207,255,252,207,6,2,171,184,120,241,98,28,59,118,204,181,185,2,174,87,191,249,205,111,226,135,63,252,161,129,192,42,254,251,
191,255,59,110,222,188,233,122,197,150,196,244,246,237,219,227,237,183,223,142,215,95,127,61,94,124,241,69,3,122,72,79,26,193,195,123,241,197,23,227,197,23,95,140,151,94,122,41,50,153,76,156,60,121,114,
165,216,113,49,100,67,109,223,190,61,246,239,223,111,16,176,138,206,206,78,67,168,16,63,252,225,15,93,175,224,91,252,249,207,127,54,4,54,61,164,35,34,126,241,139,95,68,99,99,99,188,246,218,107,6,36,168,
55,223,235,175,191,30,175,191,254,122,52,55,55,71,38,147,137,107,215,174,45,121,192,138,106,0,0,190,203,19,79,248,237,136,255,223,222,189,61,183,93,223,137,255,127,89,137,227,16,135,96,83,140,115,48,193,
196,145,108,89,182,108,73,62,74,62,201,150,100,135,6,72,255,199,189,216,63,128,139,172,29,66,210,144,182,233,118,201,182,59,238,12,189,216,210,139,210,157,41,48,83,202,66,7,72,249,94,240,35,63,57,54,108,
14,62,232,240,120,204,112,225,66,73,242,250,72,159,183,158,159,207,7,189,57,220,152,62,125,250,116,44,45,45,197,181,107,215,12,71,80,31,189,141,141,141,184,120,241,98,252,242,151,191,140,95,255,250,215,
15,191,180,204,221,106,44,190,0,128,53,157,70,9,233,68,34,17,185,92,46,22,22,22,34,147,201,24,144,160,110,28,163,163,163,49,58,58,26,35,35,35,177,181,181,21,31,125,244,209,174,23,178,176,198,226,11,0,
88,211,57,236,144,142,136,232,239,239,143,90,173,22,139,139,139,6,36,168,27,87,169,84,138,190,190,190,248,253,239,127,31,55,111,222,140,127,252,227,31,59,94,216,162,154,253,146,72,36,12,1,0,4,53,252,104,
76,119,117,117,69,169,84,138,124,62,31,169,84,202,128,4,117,227,75,165,82,145,74,165,98,112,112,48,110,223,190,29,219,219,219,187,94,224,194,26,139,47,0,96,77,231,160,66,58,34,98,104,104,40,170,213,106,
228,243,121,3,18,212,205,103,114,114,50,38,39,39,227,157,119,222,137,173,173,173,248,244,211,79,133,53,22,95,0,192,154,206,129,198,116,79,79,79,84,171,213,168,86,171,134,35,168,155,223,218,218,90,156,
63,127,62,238,221,187,23,247,238,221,139,7,15,30,236,120,241,139,106,0,0,65,13,207,26,210,137,68,34,166,167,167,163,84,42,197,200,200,136,1,9,234,214,145,78,167,35,157,78,199,232,232,104,220,184,113,35,
62,252,240,195,93,111,4,97,141,197,23,0,172,233,240,164,33,29,17,49,48,48,16,181,90,45,230,230,230,12,72,80,183,174,153,153,153,152,153,153,137,183,223,126,59,110,220,184,17,95,124,241,197,142,55,134,
168,198,226,11,0,214,116,120,220,152,62,121,242,100,44,45,45,197,196,196,68,36,147,73,3,18,212,237,225,234,213,171,241,234,171,175,198,47,126,241,139,120,255,253,247,237,93,141,197,23,0,172,233,240,216,
33,29,17,145,201,100,162,92,46,71,54,155,53,32,65,221,126,198,199,199,99,124,124,60,110,221,186,21,155,155,155,241,241,199,31,239,122,195,8,107,44,190,0,96,77,71,72,215,235,235,235,139,90,173,22,203,203,
203,6,36,168,89,89,89,137,115,231,206,197,253,251,247,227,206,157,59,241,245,215,95,239,120,3,137,106,44,190,0,96,77,71,76,119,118,118,198,244,244,116,204,205,205,249,210,49,65,77,189,225,225,225,135,
127,189,243,206,59,241,135,63,252,97,215,27,73,88,99,241,5,0,107,58,237,23,210,17,223,237,41,93,169,84,162,80,40,24,144,160,230,135,228,114,185,200,229,114,113,253,250,245,184,113,227,70,124,246,217,103,
59,222,88,162,26,139,47,0,88,211,105,159,152,62,125,250,116,44,45,45,197,181,107,215,12,71,80,243,184,54,54,54,30,126,105,217,191,255,251,191,199,63,255,249,207,29,111,48,97,141,197,23,0,172,233,180,110,
72,39,18,137,200,229,114,177,176,176,16,153,76,198,128,4,53,79,234,251,189,171,211,233,116,108,110,110,198,71,31,125,180,235,13,39,172,45,190,0,128,53,157,214,9,233,136,136,254,254,254,168,213,106,177,
184,184,104,64,130,154,103,85,44,22,163,175,175,47,182,183,183,227,221,119,223,141,47,191,252,114,199,27,80,84,91,124,1,0,107,58,205,31,211,93,93,93,81,42,149,34,159,207,71,42,149,50,32,65,205,126,73,
38,147,145,76,38,99,112,112,48,110,223,190,29,219,219,219,187,222,136,194,218,226,11,0,88,211,105,190,144,142,248,238,75,199,170,213,106,228,243,121,3,18,212,28,148,201,201,201,152,156,156,140,155,55,
111,198,230,230,102,124,250,233,167,194,218,226,11,0,88,211,105,210,152,238,233,233,137,106,181,26,213,106,213,112,4,53,135,101,117,117,53,206,159,63,31,255,241,31,255,17,239,189,247,94,60,120,240,96,
199,155,84,84,91,124,1,0,104,220,144,78,36,18,49,61,61,29,165,82,201,158,210,130,154,163,48,50,50,18,35,35,35,145,74,165,98,107,107,43,62,252,240,195,93,111,88,97,45,168,1,0,107,58,141,19,210,17,17,175,
188,242,74,84,171,213,152,155,155,51,32,65,205,81,155,158,158,142,233,233,233,120,251,237,183,227,198,141,27,241,197,23,95,236,120,3,139,106,139,47,0,96,77,231,232,99,250,228,201,147,177,180,180,20,19,
19,19,145,76,38,13,72,80,211,72,174,94,189,26,131,131,131,113,247,238,221,120,255,253,247,227,219,111,191,221,241,70,22,214,22,95,0,160,241,36,18,9,67,104,241,144,142,136,200,100,50,81,46,151,35,155,205,
26,144,160,166,81,141,141,141,197,216,216,88,220,190,125,59,54,55,55,227,175,127,253,235,174,55,182,176,22,212,0,128,53,157,195,9,233,190,190,190,168,213,106,177,188,188,108,64,130,154,102,177,188,188,
28,103,207,158,141,251,247,239,199,123,239,189,23,95,125,245,213,142,55,186,168,182,248,2,0,214,116,14,46,166,59,59,59,99,122,122,58,230,230,230,124,233,152,160,166,25,13,15,15,63,252,235,230,205,155,
241,193,7,31,236,122,195,11,107,139,47,0,0,251,23,210,17,223,237,41,93,169,84,162,80,40,24,144,160,166,217,229,114,185,200,229,114,113,253,250,245,184,113,227,70,124,246,217,103,59,78,0,162,90,80,3,0,
214,116,158,61,166,79,159,62,29,171,171,171,113,245,234,85,195,17,212,180,154,141,141,141,120,245,213,87,227,222,189,123,113,239,222,189,135,123,87,187,91,109,241,5,0,172,233,60,125,72,39,18,137,200,229,
114,177,176,176,16,153,76,198,128,4,53,173,42,157,78,71,58,157,142,84,42,21,155,155,155,241,209,71,31,237,58,49,8,107,139,47,0,96,77,231,255,14,233,136,136,254,254,254,168,213,106,177,184,184,104,64,130,
154,118,81,44,22,163,175,175,47,182,183,183,227,221,119,223,141,47,191,252,114,199,137,66,84,3,0,8,106,126,56,166,187,186,186,162,84,42,69,62,159,143,84,42,101,64,130,154,118,147,76,38,35,153,76,198,107,
175,189,22,183,110,221,138,237,237,237,93,39,12,97,109,241,5,0,172,233,66,122,167,225,225,225,168,84,42,49,49,49,97,64,130,154,118,55,49,49,17,19,19,19,113,243,230,205,216,218,218,138,79,62,249,68,88,
55,129,68,34,97,8,0,32,168,57,196,152,238,237,237,141,74,165,18,213,106,213,112,16,212,236,180,186,186,26,23,46,92,136,223,252,230,55,113,247,238,221,248,230,155,111,118,156,76,68,181,197,23,0,176,166,
183,99,72,39,18,137,152,158,158,142,82,169,100,79,105,4,53,63,236,251,125,171,83,169,84,220,184,113,35,254,248,199,63,238,58,177,8,107,139,47,0,96,77,111,135,144,142,136,120,229,149,87,162,86,171,197,
236,236,172,1,33,168,121,60,211,211,211,49,61,61,29,215,175,95,143,205,205,205,248,252,243,207,119,156,104,68,181,197,23,0,176,166,183,114,76,119,119,119,199,242,242,114,100,50,153,72,38,147,6,132,160,
230,201,109,108,108,196,192,192,64,220,189,123,55,222,127,255,253,248,246,219,111,119,156,112,132,53,0,128,160,110,165,144,142,136,200,100,50,81,46,151,35,155,205,26,16,130,154,103,51,54,54,22,99,99,99,
241,222,123,239,197,230,230,102,252,207,255,252,207,174,19,144,176,182,248,2,0,214,244,102,15,233,151,95,126,57,170,213,106,44,47,47,27,16,130,154,253,181,176,176,16,125,125,125,113,255,254,253,120,239,
189,247,226,171,175,190,218,113,66,18,213,22,95,0,192,154,222,140,49,221,217,217,25,211,211,211,49,63,63,31,195,195,195,6,132,160,230,96,124,255,165,101,195,195,195,241,243,159,255,220,222,213,22,95,0,
192,154,222,180,33,29,17,49,52,52,20,149,74,37,10,133,130,1,33,168,57,28,185,92,46,114,185,92,92,191,126,61,222,121,231,157,248,219,223,254,182,227,68,37,170,45,190,0,128,53,189,145,99,250,244,233,211,
177,182,182,22,63,253,233,79,13,7,65,205,209,216,216,216,136,87,95,125,53,238,221,187,23,247,238,221,139,7,15,30,236,56,97,9,235,131,147,72,36,12,1,0,4,53,79,24,210,137,68,34,114,185,92,44,46,46,198,232,
232,168,1,33,168,57,90,233,116,58,210,233,116,12,15,15,199,230,230,102,252,249,207,127,222,117,2,19,214,0,0,130,250,40,67,58,34,226,194,133,11,81,171,213,98,126,126,222,128,16,212,52,150,249,249,249,120,
233,165,151,98,123,123,59,222,125,247,221,248,242,203,47,119,156,208,68,181,197,23,0,176,166,31,69,76,119,117,117,69,169,84,138,124,62,31,169,84,202,128,16,212,52,166,100,50,25,201,100,50,134,134,134,
226,231,63,255,121,252,246,183,191,221,117,98,19,214,22,95,0,192,154,126,24,33,29,241,221,151,234,86,42,149,152,152,152,48,32,4,53,205,97,124,124,60,198,199,199,227,221,119,223,141,205,205,205,248,228,
147,79,132,181,197,23,0,224,208,66,186,183,183,55,42,149,74,84,171,85,3,66,80,211,156,202,229,114,156,63,127,62,238,223,191,31,119,238,220,137,175,191,254,122,199,137,79,84,11,106,0,176,166,91,211,247,
51,166,143,31,63,30,51,51,51,49,55,55,23,35,35,35,6,132,160,166,185,125,191,111,245,229,203,151,99,107,107,43,254,248,199,63,238,58,1,10,107,139,47,0,88,211,121,150,144,142,136,184,120,241,98,84,171,213,
152,157,157,53,32,4,53,173,101,106,106,42,166,166,166,226,250,245,24,171,95,71,0,0,32,0,73,68,65,84,235,177,181,181,21,127,255,251,223,119,156,16,69,181,197,23,0,172,233,60,77,76,119,119,119,199,242,242,
114,92,187,118,205,112,16,212,180,182,141,141,141,24,24,24,136,59,119,238,196,127,254,231,127,198,183,223,126,187,227,196,40,172,45,190,0,208,78,18,137,132,33,60,101,72,119,116,116,196,228,228,100,44,
46,46,198,216,216,152,1,33,168,105,15,99,99,99,49,54,54,22,119,239,222,141,205,205,205,248,203,95,254,178,235,68,41,172,5,53,0,88,211,217,43,164,35,34,94,126,249,229,168,86,171,177,188,188,108,64,8,106,
218,83,169,84,138,190,190,190,248,221,239,126,23,183,110,221,138,175,190,250,106,199,137,83,84,255,48,87,179,1,192,154,222,142,49,221,217,217,25,197,98,49,166,167,167,237,41,141,160,134,84,42,21,169,84,
42,46,95,190,28,183,111,223,142,237,237,237,93,39,80,97,189,155,171,217,0,96,77,111,167,144,142,136,24,26,26,138,74,165,18,133,66,193,128,16,212,80,111,114,114,50,38,39,39,227,223,254,237,223,226,198,
141,27,241,183,191,253,109,199,9,85,84,239,228,106,54,0,8,234,118,137,233,23,94,120,33,42,149,74,172,175,175,27,14,130,26,126,204,250,250,122,92,188,120,49,238,221,187,23,247,238,221,139,7,15,30,236,56,
177,10,107,139,47,0,180,18,23,201,127,56,164,19,137,68,228,114,185,88,92,92,140,209,209,81,3,66,80,195,227,72,167,211,145,78,167,99,116,116,52,182,182,182,226,79,127,250,211,174,19,109,187,135,181,197,
23,0,90,131,139,228,123,63,222,61,48,48,16,213,106,53,230,231,231,189,72,16,212,240,52,102,102,102,162,183,183,55,182,183,183,227,214,173,91,241,197,23,95,236,56,241,182,115,84,91,124,1,160,53,180,251,
69,242,71,99,250,228,201,147,81,44,22,35,159,207,251,210,49,4,53,60,171,100,50,25,201,100,50,134,134,134,226,231,63,255,121,252,246,183,191,221,117,2,110,199,176,22,212,0,96,77,111,165,144,142,136,200,
100,50,81,46,151,35,155,205,122,97,32,168,97,63,141,143,143,199,248,248,120,188,251,238,187,177,181,181,21,31,127,252,113,91,135,181,71,190,1,192,154,222,42,33,221,219,219,27,213,106,53,42,149,138,23,
4,130,26,14,82,185,92,142,243,231,207,199,253,251,247,227,206,157,59,241,245,215,95,239,56,65,183,75,84,11,106,0,104,13,237,116,135,250,209,152,62,126,252,120,204,204,204,196,220,220,92,140,140,140,120,
49,32,168,225,48,12,15,15,199,240,240,112,92,190,124,57,110,220,184,17,255,253,223,255,189,235,68,221,234,97,237,145,111,0,104,13,237,112,145,124,175,187,210,23,47,94,140,106,181,26,179,179,179,94,4,8,
106,56,10,83,83,83,49,53,53,21,215,175,95,143,173,173,173,248,251,223,255,190,227,196,221,202,81,45,168,1,192,154,222,140,49,221,221,221,29,203,203,203,113,237,218,53,7,31,65,13,141,96,99,99,35,46,94,
188,24,191,252,229,47,227,215,191,254,117,124,251,237,183,59,78,224,173,24,214,30,249,6,0,65,221,76,33,221,209,209,17,217,108,54,86,86,86,34,147,201,56,240,8,106,104,36,163,163,163,49,58,58,26,35,35,35,
177,185,185,25,127,249,203,95,118,157,208,91,41,172,221,161,6,128,214,208,106,23,201,247,122,188,187,191,191,63,170,213,106,44,45,45,57,224,8,106,104,100,165,82,41,250,250,250,226,119,191,251,93,220,186,
117,43,190,250,234,171,29,39,248,86,137,106,65,13,0,173,161,149,214,244,71,99,186,179,179,51,138,197,98,76,79,79,219,83,26,65,13,205,34,149,74,69,42,149,138,203,151,47,199,237,219,183,99,123,123,123,215,
137,190,217,195,218,35,223,0,208,26,90,97,77,223,235,174,244,208,208,80,84,171,213,200,231,243,14,50,130,26,154,209,228,228,100,76,78,78,198,59,239,188,19,91,91,91,241,233,167,159,238,56,241,55,115,84,
11,106,0,104,13,205,126,135,250,209,152,238,233,233,137,74,165,18,181,90,205,193,69,80,67,43,88,91,91,139,243,231,207,199,189,123,247,226,222,189,123,241,224,193,131,29,11,64,51,134,181,71,190,1,160,53,
52,235,69,242,71,67,58,145,72,68,46,151,139,197,197,197,24,29,29,117,96,17,212,208,74,210,233,116,164,211,233,24,29,29,141,173,173,173,248,211,159,254,180,107,65,104,166,176,22,212,0,208,26,154,109,77,
223,235,241,238,129,129,129,168,86,171,49,63,63,239,128,34,168,161,149,205,204,204,68,111,111,111,108,111,111,199,173,91,183,226,139,47,190,216,177,64,52,75,84,123,228,27,0,4,245,81,199,244,201,147,39,
99,105,105,41,38,38,38,34,153,76,58,152,8,106,104,7,201,100,50,146,201,100,12,13,13,197,47,126,241,139,120,255,253,247,155,110,239,106,119,168,1,160,53,52,195,69,242,189,238,74,103,50,153,40,151,203,145,
205,102,29,68,4,53,180,163,241,241,241,24,31,31,143,91,183,110,197,230,230,102,124,252,241,199,187,22,142,70,13,107,119,168,1,64,80,31,69,72,247,246,246,70,181,90,141,74,165,226,224,33,168,141,0,34,86,
86,86,226,220,185,115,113,255,254,253,184,115,231,78,124,253,245,215,59,22,146,70,140,106,119,168,1,160,53,52,234,154,254,104,76,31,63,126,60,102,102,102,98,110,110,46,70,70,70,28,56,16,212,240,255,27,
30,30,142,225,225,225,72,165,82,113,243,230,205,248,195,31,254,176,107,65,105,164,176,118,135,26,0,4,245,97,132,116,68,196,165,75,151,162,90,173,70,161,80,112,192,64,80,195,15,203,231,243,145,207,231,
227,250,245,235,113,227,198,141,248,236,179,207,118,44,48,141,18,213,238,80,3,64,107,104,164,139,228,143,198,116,119,119,119,44,47,47,199,181,107,215,28,40,16,212,240,248,54,54,54,226,226,197,139,241,
203,95,254,50,126,253,235,95,55,220,151,150,185,67,13,0,173,161,17,46,146,63,26,210,29,29,29,145,205,102,99,101,101,37,50,153,140,131,4,130,26,158,220,232,232,104,140,142,142,70,58,157,142,205,205,205,
248,232,163,143,118,45,60,71,21,214,238,80,3,64,107,56,202,139,228,123,61,222,221,223,223,31,181,90,45,22,23,23,29,28,16,212,240,236,138,197,98,188,244,210,75,241,251,223,255,62,110,222,188,25,255,248,
199,63,118,44,68,71,17,213,130,26,0,90,195,81,173,233,143,198,116,87,87,87,148,74,165,200,231,243,145,74,165,28,24,16,212,176,127,82,169,84,164,82,169,24,28,28,140,219,183,111,199,246,246,246,174,5,233,
48,195,218,35,223,0,208,26,14,123,77,223,235,174,244,208,208,80,84,171,213,200,231,243,14,8,8,106,56,56,147,147,147,49,57,57,25,239,188,243,78,108,109,109,197,167,159,126,122,36,97,45,168,1,160,53,28,
230,29,234,71,99,186,167,167,39,42,149,74,212,106,53,7,2,4,53,28,158,181,181,181,184,112,225,66,252,234,87,191,138,123,247,238,197,131,7,15,118,44,86,7,29,213,30,249,6,128,214,112,24,23,201,31,13,233,
68,34,17,211,211,211,81,42,149,236,41,13,130,26,142,198,200,200,72,140,140,140,68,38,147,137,173,173,173,248,240,195,15,119,45,92,7,21,214,130,26,0,90,195,65,174,233,123,61,222,61,48,48,16,181,90,45,230,
230,230,12,31,4,53,28,189,233,233,233,152,158,158,142,183,223,126,59,110,220,184,17,95,124,241,197,142,133,236,32,162,218,35,223,0,32,168,159,36,166,79,158,60,25,75,75,75,49,49,49,17,201,100,210,224,65,
80,67,99,185,122,245,106,12,14,14,198,221,187,119,227,253,247,223,63,208,189,171,221,161,6,128,214,176,223,23,201,247,186,43,157,201,100,162,92,46,71,54,155,53,112,16,212,208,184,198,198,198,98,108,108,
44,110,221,186,21,91,91,91,241,215,191,254,117,215,2,247,180,97,93,191,64,10,106,0,104,13,251,181,166,239,21,210,125,125,125,81,171,213,98,121,121,217,160,65,80,67,243,88,89,89,137,115,231,206,197,253,
251,247,227,206,157,59,241,245,215,95,239,88,240,158,245,110,181,71,190,1,160,53,212,175,233,79,251,25,225,209,152,238,236,236,140,233,233,233,152,155,155,243,165,99,32,168,161,57,13,15,15,63,252,235,
230,205,155,241,193,7,31,236,90,248,158,54,172,83,169,148,1,3,64,11,120,150,59,212,63,180,167,116,165,82,137,66,161,96,184,32,168,161,249,229,114,185,200,229,114,113,253,250,245,184,113,227,70,124,246,
217,103,59,22,194,39,141,106,143,123,3,64,235,120,218,139,228,143,198,244,233,211,167,99,105,105,41,174,93,187,102,168,32,168,161,245,108,108,108,196,171,175,190,26,247,238,221,139,95,253,234,87,241,207,
127,254,115,199,130,248,184,97,237,113,111,0,104,45,29,29,29,15,191,204,244,73,67,58,145,72,68,46,151,139,133,133,133,200,100,50,134,9,130,26,90,87,58,157,142,116,58,29,169,84,42,54,55,55,227,163,143,
62,218,181,64,254,95,97,237,14,53,0,180,150,68,34,17,15,30,60,120,162,144,142,136,232,239,239,143,90,173,22,139,139,139,134,8,130,26,218,71,177,88,140,190,190,190,216,222,222,142,119,223,125,55,190,252,
242,203,29,11,230,143,69,181,160,6,128,214,242,127,173,237,143,198,116,87,87,87,148,74,165,200,231,243,190,87,5,4,53,180,167,100,50,25,201,100,50,94,123,237,181,184,117,235,86,108,111,111,239,90,56,247,
10,107,143,124,3,64,107,249,161,181,253,135,190,116,172,86,171,69,46,151,51,56,16,212,192,196,196,68,76,76,76,196,205,155,55,99,107,107,43,62,249,228,147,31,13,107,65,13,0,173,101,175,59,212,143,198,116,
79,79,79,84,171,213,168,86,171,6,6,130,26,120,212,234,234,106,92,184,112,33,126,243,155,223,196,221,187,119,227,155,111,190,217,115,81,245,200,55,0,180,150,71,247,162,126,244,239,77,79,79,71,169,84,178,
167,52,8,106,224,199,124,191,111,117,42,149,138,173,173,173,248,240,195,15,119,253,51,130,26,0,90,203,15,173,237,175,188,242,74,84,171,213,152,155,155,51,36,16,212,192,227,154,158,158,142,233,233,233,
120,251,237,183,227,157,119,222,137,255,253,223,255,125,248,247,60,242,13,0,173,29,212,221,221,221,177,188,188,28,153,76,38,146,201,164,1,129,160,6,158,198,213,171,87,99,112,112,48,238,222,189,27,239,
191,255,126,124,251,237,183,238,80,3,64,139,169,191,88,158,201,100,162,92,46,71,54,155,53,24,16,212,192,179,26,27,27,139,177,177,177,120,239,189,247,98,115,115,51,254,241,143,127,24,10,0,180,144,142,142,
142,232,235,235,139,90,173,22,203,203,203,6,2,130,26,216,111,11,11,11,209,215,215,23,31,124,240,129,97,0,64,11,89,90,90,122,248,61,42,128,160,6,14,136,197,22,0,90,207,155,111,190,105,8,208,132,124,179,
17,0,0,0,8,106,0,0,0,16,212,0,0,0,32,168,1,0,0,64,80,3,0,0,128,160,6,0,0,0,4,53,0,0,0,8,106,0,0,0,16,212,0,0,0,32,168,1,0,0,64,80,3,0,0,0,130,26,0,0,0,4,53,0,0,0,8,106,0,0,0,16,212,0,0,0,32,168,1,0,0,
0,65,13,0,0,0,130,26,0,0,0,4,53,0,0,0,8,106,0,0,0,16,212,0,0,0,128,160,6,0,0,0,65,13,0,0,0,130,26,0,0,0,4,53,0,0,0,8,106,0,0,0,64,80,3,0,0,128,160,6,0,0,0,65,13,0,0,0,130,26,0,0,0,4,53,0,0,0,32,168,1,
0,0,64,80,3,0,0,128,160,6,0,0,0,65,13,0,0,0,130,26,0,0,0,16,212,0,0,0,32,168,1,0,0,64,80,3,0,0,128,160,6,0,0,0,65,13,0,0,0,8,106,0,0,0,16,212,0,0,0,32,168,1,0,0,64,80,3,0,0,128,160,6,0,0,0,4,53,0,0,0,
8,106,0,0,0,16,212,0,0,0,32,168,1,0,0,64,80,3,0,0,0,130,26,0,0,0,4,53,0,0,0,8,106,0,0,0,16,212,0,0,0,32,168,1,0,0,0,65,13,0,0,0,130,26,0,0,0,4,53,0,0,0,8,106,0,0,0,16,212,0,0,0,128,160,6,0,0,0,65,13,0,
0,0,130,26,0,0,0,4,53,0,0,0,8,106,0,0,0,64,80,3,0,0,128,160,6,0,0,0,65,13,0,0,0,130,26,0,0,0,4,53,0,0,0,32,168,1,0,0,64,80,3,0,0,128,160,6,0,0,0,65,13,0,0,0,130,26,0,0,0,16,212,0,0,0,32,168,1,0,0,64,80,
3,0,0,128,160,6,0,0,0,65,13,0,0,0,8,106,0,0,0,16,212,0,0,0,32,168,1,0,0,64,80,3,0,0,128,160,6,0,0,0,4,53,0,0,0,8,106,0,0,0,16,212,0,0,0,32,168,1,0,0,64,80,3,0,0,128,160,6,0,0,0,4,53,0,0,0,8,106,0,0,0,
16,212,0,0,0,32,168,1,0,0,64,80,3,0,0,0,130,26,0,0,0,4,53,0,0,0,8,106,0,0,0,104,13,29,223,126,251,173,41,0,60,131,223,253,238,119,241,175,255,250,175,241,205,55,223,24,70,3,249,230,155,111,226,243,207,
63,143,136,136,211,167,79,91,236,142,192,231,159,127,222,241,255,205,63,142,31,63,110,32,13,226,103,63,251,89,204,207,207,27,4,192,62,176,186,1,60,163,108,54,27,255,245,95,255,21,183,111,223,54,12,216,
59,172,13,161,65,140,143,143,139,105,128,125,228,145,111,128,125,48,53,53,21,253,253,253,6,209,128,220,157,54,123,190,243,220,115,207,197,242,242,178,65,0,236,35,143,124,3,236,147,59,119,238,196,191,252,
203,191,196,247,231,85,49,1,28,181,239,31,187,143,136,120,253,245,215,227,218,181,107,134,2,176,143,220,161,6,216,39,139,139,139,145,207,231,247,252,32,11,112,148,49,253,202,43,175,68,38,147,49,20,0,65,
13,208,216,81,253,194,11,47,136,106,160,97,28,59,118,44,106,181,90,36,147,73,195,0,16,212,0,141,107,116,116,52,170,213,170,65,0,71,170,254,98,222,220,220,92,204,206,206,26,10,128,160,6,104,124,181,90,
45,134,135,135,247,252,96,11,112,152,49,221,219,219,43,166,1,4,53,64,115,89,91,91,139,174,174,46,131,0,142,84,173,86,139,145,145,17,131,0,16,212,0,205,99,114,114,50,150,150,150,30,254,236,46,53,112,24,
234,207,53,227,227,227,177,182,182,102,40,0,130,26,160,249,76,76,76,196,192,192,128,168,6,14,157,61,167,1,4,53,64,83,75,165,82,177,190,190,30,199,142,29,51,12,224,192,213,95,180,43,151,203,145,205,102,
13,5,64,80,3,52,175,217,217,217,152,153,153,217,243,3,47,192,65,196,244,192,192,128,61,167,1,4,53,64,107,152,159,159,143,23,95,124,81,84,3,7,206,158,211,0,130,26,160,165,140,140,140,68,173,86,51,8,224,
64,212,95,164,155,157,157,141,185,185,57,67,1,16,212,0,173,99,117,117,53,198,198,198,246,252,0,12,176,31,49,221,219,219,43,166,1,4,53,64,107,42,151,203,113,234,212,41,81,13,28,136,106,181,106,207,105,
0,65,13,208,154,198,199,199,163,92,46,27,4,176,47,234,47,202,141,141,141,69,165,82,49,20,0,65,13,208,186,222,122,235,173,24,28,28,220,243,3,49,192,211,56,121,242,164,61,167,1,4,53,64,123,88,95,95,143,
206,206,78,131,0,158,218,163,123,78,79,76,76,24,10,128,160,6,104,125,133,66,33,138,197,226,158,31,140,1,158,36,166,7,6,6,118,124,225,33,0,130,26,160,229,77,77,77,69,127,127,191,168,6,158,154,61,167,1,
4,53,64,91,26,30,30,142,90,173,22,29,29,58,26,120,124,246,156,6,16,212,0,68,196,226,226,98,228,114,185,61,63,40,3,252,88,76,219,115,26,64,80,3,136,234,197,197,120,254,249,231,69,53,240,68,236,57,13,32,
168,1,218,94,38,147,137,90,173,102,16,192,143,170,191,216,150,201,100,236,57,13,32,168,1,136,248,110,27,173,250,47,21,114,151,26,248,33,39,79,158,140,149,149,21,131,0,16,212,0,124,175,90,173,70,87,87,
151,65,0,187,212,95,100,91,89,89,177,231,52,128,160,6,160,222,228,228,100,44,46,46,238,249,1,26,16,211,17,223,237,57,61,62,62,110,40,0,130,26,128,189,162,250,194,133,11,162,26,216,197,158,211,0,130,26,
128,31,145,74,165,162,86,171,69,34,225,212,12,236,188,168,54,51,51,99,155,44,0,65,13,192,143,153,159,159,143,153,153,153,61,63,80,3,237,25,211,246,156,6,16,212,0,60,166,98,177,24,47,190,248,162,168,6,
34,34,162,82,169,68,58,157,54,8,0,65,13,192,255,101,100,100,196,222,212,208,198,234,47,162,141,142,142,70,181,90,53,20,0,65,13,192,227,90,93,93,141,76,38,179,231,7,108,160,61,156,60,121,50,202,229,178,
65,0,8,106,0,158,84,185,92,142,83,167,78,137,106,104,35,245,239,243,229,229,101,123,78,3,8,106,0,158,70,54,155,141,149,149,21,131,128,54,140,233,129,129,129,200,102,179,134,2,32,168,1,120,90,215,174,93,
139,87,95,125,117,207,15,220,64,139,126,56,75,36,162,82,169,216,115,26,64,80,3,240,172,106,181,90,28,63,126,220,32,160,133,213,95,44,155,157,157,141,98,177,104,40,0,130,26,128,103,53,61,61,29,243,243,
243,123,126,240,6,90,43,166,95,120,225,133,152,157,157,53,20,0,65,13,192,126,153,153,153,137,151,95,126,89,84,67,139,171,84,42,49,58,58,106,16,0,130,26,128,253,50,60,60,28,181,90,45,58,58,116,52,180,146,
250,139,99,233,116,58,214,215,215,13,5,64,80,3,176,223,150,150,150,98,114,114,114,207,15,226,64,115,235,234,234,242,173,254,0,130,26,128,131,142,234,231,159,127,94,84,67,11,120,116,207,233,92,46,103,40,
0,130,26,128,131,146,201,100,98,109,109,205,32,160,133,98,250,220,185,115,246,156,6,16,212,0,28,134,215,95,127,125,199,254,180,238,82,67,19,127,16,75,36,98,125,125,61,82,169,148,97,0,8,106,0,14,67,165,
82,137,174,174,46,131,128,38,84,127,17,108,102,102,198,158,211,0,130,26,128,195,148,203,229,98,97,97,97,207,15,232,64,115,196,244,153,51,103,98,110,110,206,80,0,4,53,0,71,17,213,231,207,159,23,213,208,
164,170,213,170,61,167,1,4,53,0,71,33,149,74,197,250,250,122,36,18,78,229,208,12,234,47,122,141,140,140,216,115,26,64,80,3,112,148,230,231,231,99,122,122,122,207,15,236,64,99,58,113,226,68,148,203,101,
131,0,16,212,0,28,181,98,177,24,61,61,61,162,26,26,152,61,167,1,4,53,0,13,40,157,78,123,116,20,154,36,166,207,157,59,23,19,19,19,134,2,32,168,1,104,20,107,107,107,59,190,220,200,93,106,104,192,15,93,137,
68,212,106,53,123,78,3,8,106,0,26,205,234,234,106,156,58,117,202,32,160,129,212,95,220,154,158,158,142,82,169,100,40,0,130,26,128,70,147,205,102,99,121,121,121,207,15,242,192,209,198,244,153,51,103,98,
126,126,222,80,0,4,53,0,141,106,108,108,44,94,125,245,85,81,13,13,166,82,169,216,115,26,64,80,3,208,200,146,201,100,84,171,213,56,126,252,184,97,192,17,122,116,207,233,141,141,13,67,1,16,212,0,52,186,
153,153,153,152,155,155,219,243,131,61,112,184,236,57,13,32,168,1,104,194,168,126,233,165,151,68,53,28,1,123,78,3,8,106,0,154,216,247,143,152,118,116,232,104,56,170,152,62,123,246,172,61,167,1,4,53,0,
205,104,105,105,105,199,135,121,119,169,225,16,63,96,37,18,177,190,190,110,207,105,0,65,13,64,179,90,94,94,142,211,167,79,139,106,56,4,245,239,175,169,169,41,123,78,3,8,106,0,154,89,38,147,137,213,213,
85,131,128,67,140,233,51,103,206,68,177,88,52,20,0,65,13,64,179,187,122,245,106,92,190,124,121,207,15,254,192,254,179,231,52,128,160,6,160,133,172,173,173,197,137,19,39,12,2,14,64,253,69,170,225,225,97,
123,78,3,8,106,0,90,73,161,80,136,133,133,133,61,3,0,216,31,39,78,156,240,159,88,0,8,106,0,90,81,62,159,143,243,231,207,139,106,216,71,245,239,163,165,165,37,123,78,3,8,106,0,90,81,42,149,138,90,173,22,
137,132,83,63,236,119,76,159,61,123,54,38,39,39,13,5,64,80,3,208,170,138,197,98,20,10,133,61,131,0,120,202,15,83,246,156,6,16,212,0,180,135,133,133,133,232,233,233,17,213,240,12,234,223,55,133,66,193,
158,211,0,130,26,128,118,144,78,167,163,90,173,26,4,236,67,76,159,57,115,70,76,3,8,106,0,218,73,181,90,141,116,58,189,103,32,0,143,111,109,109,205,158,211,0,130,26,128,118,12,129,231,158,123,206,32,224,
9,212,95,124,74,165,82,113,229,202,21,67,1,16,212,0,180,155,108,54,27,75,75,75,123,134,2,240,227,236,57,13,128,160,6,16,213,113,241,226,69,81,13,143,161,254,253,177,184,184,24,249,124,222,80,0,4,53,0,
237,42,153,76,70,173,86,139,227,199,143,27,6,60,102,76,159,61,123,54,114,185,156,161,0,32,168,1,218,221,204,204,76,204,206,206,238,25,14,192,78,29,29,29,81,171,213,236,57,13,128,160,6,224,59,179,179,179,
241,147,159,252,68,84,195,30,30,221,115,122,97,97,193,80,0,16,212,0,124,103,100,100,36,106,181,154,65,192,143,196,244,243,207,63,47,166,1,16,212,0,236,86,46,151,35,155,205,238,25,18,64,68,165,82,177,231,
52,0,130,26,128,189,173,172,172,196,233,211,167,69,53,132,61,167,1,16,212,0,60,129,177,177,177,40,151,203,6,1,117,236,57,13,128,160,6,224,177,188,241,198,27,49,52,52,244,240,103,119,169,105,71,246,156,
6,64,80,3,240,84,42,149,74,116,118,118,26,4,109,31,211,253,253,253,246,156,6,64,80,3,240,248,10,133,66,148,74,165,61,3,3,218,69,71,71,71,172,175,175,219,115,26,0,65,13,192,147,153,154,154,138,254,254,
126,81,77,91,177,231,52,0,130,26,128,103,246,253,183,26,39,18,150,10,218,47,166,159,127,254,249,29,79,105,0,128,160,6,224,137,20,139,197,29,95,198,228,46,53,237,162,82,169,68,38,147,49,8,0,4,53,0,79,111,
113,113,49,94,120,225,5,81,77,75,171,127,93,95,186,116,201,158,211,0,8,106,0,158,93,58,157,142,106,181,106,16,180,133,206,206,78,175,119,0,4,53,0,251,167,86,171,197,200,200,200,195,159,221,165,166,149,
60,186,231,116,161,80,48,20,0,4,53,0,251,103,117,117,53,78,158,60,105,16,180,108,76,191,252,242,203,246,156,6,64,80,3,176,255,38,39,39,99,105,105,105,207,16,129,102,215,209,209,17,181,90,45,134,135,135,
13,3,0,65,13,192,254,155,152,152,136,139,23,47,138,106,90,66,253,235,55,159,207,239,184,96,4,0,130,26,128,125,149,76,38,163,90,173,198,177,99,199,12,131,150,137,233,238,238,110,123,78,3,32,168,1,56,120,
179,179,179,49,59,59,187,103,152,64,51,170,86,171,49,54,54,102,16,0,8,106,0,14,222,220,220,92,252,228,39,63,17,213,52,165,71,247,156,126,253,245,215,13,5,0,65,13,192,225,24,25,25,137,90,173,102,16,52,
181,206,206,206,168,84,42,6,1,128,160,6,224,112,149,203,229,24,31,31,127,248,179,187,212,52,131,250,215,233,194,194,66,76,77,77,25,10,0,130,26,128,195,183,178,178,18,221,221,221,162,154,166,139,233,190,
190,190,200,231,243,134,2,128,160,6,224,104,140,143,143,71,185,92,54,8,154,138,61,167,1,16,212,0,52,132,55,223,124,51,134,134,134,30,254,236,46,53,141,168,254,117,153,203,229,98,121,121,217,80,0,16,212,
0,28,189,74,165,18,157,157,157,6,65,195,199,116,119,119,119,44,44,44,24,10,0,130,26,128,198,80,40,20,162,88,44,238,25,48,208,72,42,149,138,61,167,1,16,212,0,52,150,169,169,169,232,239,239,23,213,52,148,
71,247,156,254,233,79,127,106,40,0,8,106,0,26,203,240,240,112,212,106,181,232,232,208,209,52,30,123,78,3,32,168,1,104,104,139,139,139,59,182,34,114,151,154,163,84,255,250,43,149,74,246,156,6,64,80,3,208,
248,81,253,194,11,47,136,106,26,38,166,251,250,250,162,80,40,24,10,0,130,26,128,198,54,58,58,26,213,106,213,32,104,8,246,156,6,64,80,3,208,84,30,13,24,119,169,57,76,246,156,6,64,80,3,208,212,214,214,214,
162,171,171,203,32,56,178,152,182,231,52,0,130,26,128,166,52,57,57,25,75,75,75,123,134,14,28,6,123,78,3,32,168,1,104,90,19,19,19,113,225,194,5,81,205,161,168,127,125,189,246,218,107,246,156,6,64,80,3,
208,188,82,169,84,108,108,108,196,177,99,199,12,131,67,211,217,217,233,139,241,0,16,212,0,52,191,217,217,217,152,153,153,121,248,179,187,212,28,132,250,215,85,177,88,180,231,52,0,130,26,128,214,48,63,
63,31,47,190,248,162,168,230,192,99,186,175,175,79,76,3,32,168,1,104,29,35,35,35,81,171,213,12,130,3,101,207,105,0,4,53,0,45,105,117,117,117,199,55,46,187,75,205,126,168,127,29,77,78,78,218,115,26,0,65,
13,64,107,90,89,89,137,83,167,78,137,106,246,61,166,187,187,187,99,113,113,209,80,0,16,212,0,180,166,108,54,27,171,171,171,6,193,190,91,91,91,179,231,52,0,130,26,128,214,246,230,155,111,198,208,208,208,
195,159,221,165,230,105,212,191,110,46,93,186,20,87,175,94,53,20,0,4,53,0,173,175,82,169,68,103,103,167,65,240,204,58,59,59,163,82,169,24,4,0,130,26,128,246,80,40,20,98,97,97,225,225,207,238,82,243,36,
234,95,47,165,82,201,54,89,0,8,106,0,218,47,170,207,159,63,47,170,121,234,152,238,235,235,139,66,161,96,40,0,8,106,0,218,75,42,149,138,90,173,22,137,132,165,136,39,103,207,105,0,4,53,0,109,173,88,44,238,
120,92,215,93,106,126,76,253,235,35,159,207,219,115,26,0,65,13,64,123,43,149,74,209,219,219,43,170,121,236,152,238,238,238,142,82,169,100,40,0,8,106,0,218,91,58,157,142,245,245,117,131,224,177,85,171,
85,123,78,3,32,168,1,32,34,98,117,117,117,71,32,185,75,77,189,71,247,156,126,253,245,215,13,5,0,65,13,0,223,91,89,89,137,83,167,78,137,106,126,208,137,19,39,162,86,171,25,4,0,130,26,0,234,101,179,217,
40,151,203,6,193,14,245,23,86,22,23,23,35,159,207,27,10,0,130,26,0,30,245,214,91,111,197,208,208,208,158,49,69,123,199,244,217,179,103,35,151,203,25,10,0,130,26,0,126,72,165,82,137,206,206,78,131,224,
161,239,247,156,78,165,82,134,1,128,160,6,128,31,82,40,20,98,97,97,225,225,207,238,82,183,167,250,227,62,53,53,181,227,53,1,0,130,26,0,126,36,170,207,158,61,43,170,197,116,156,57,115,198,158,211,0,8,106,
0,120,92,169,84,42,54,54,54,34,145,176,76,181,187,74,165,18,163,163,163,6,1,128,160,6,128,199,85,44,22,99,106,106,234,225,207,238,82,183,135,250,227,60,60,60,28,27,27,27,134,2,128,160,6,128,39,85,42,149,
162,167,167,71,84,183,161,19,39,78,196,234,234,170,65,0,32,168,1,224,105,164,211,233,88,95,95,55,136,54,81,127,193,100,121,121,217,54,89,0,8,106,0,120,22,107,107,107,145,201,100,246,140,46,90,51,166,207,
157,59,23,19,19,19,134,2,128,160,6,128,103,85,46,151,227,212,169,83,162,186,29,62,152,36,18,246,156,6,64,80,3,192,126,201,102,179,81,46,151,13,162,69,213,95,32,153,158,158,182,77,22,0,130,26,0,246,211,
91,111,189,21,67,67,67,123,70,24,173,17,211,103,206,156,137,249,249,121,67,1,64,80,3,192,126,171,84,42,209,217,217,105,16,45,170,90,173,218,115,26,0,65,13,0,7,161,80,40,236,120,28,216,93,234,230,86,127,
252,124,163,59,0,130,26,0,14,216,212,212,84,244,247,247,139,234,22,210,213,213,229,191,145,7,64,80,3,192,65,75,165,82,113,229,202,149,72,36,44,97,205,172,254,66,200,202,202,74,76,78,78,26,10,0,130,26,
0,14,90,177,88,140,169,169,169,61,227,140,230,138,233,129,129,129,200,102,179,134,2,128,160,6,128,195,82,42,149,162,167,167,71,84,55,177,99,199,142,69,173,86,139,100,50,105,24,0,8,106,0,56,44,190,196,
170,57,213,95,248,152,153,153,137,185,185,57,67,1,64,80,3,192,97,91,91,91,139,76,38,179,103,172,209,216,49,221,219,219,107,207,105,0,4,53,0,28,165,114,185,28,167,78,157,18,213,77,166,90,173,198,200,200,
136,65,0,32,168,1,224,168,100,179,89,91,46,53,129,250,11,29,99,99,99,81,169,84,12,5,0,65,13,0,71,237,173,183,222,138,161,161,161,61,227,141,198,242,220,115,207,197,202,202,138,65,0,32,168,1,160,81,84,
42,149,232,236,236,52,136,6,84,127,129,163,92,46,219,38,11,0,65,13,0,141,164,80,40,68,169,84,218,51,226,104,140,152,126,229,149,87,118,124,137,28,0,8,106,0,104,16,83,83,83,209,223,223,47,170,27,144,61,
167,1,16,212,0,208,192,82,169,84,92,185,114,37,18,9,203,91,35,168,191,160,49,63,63,31,179,179,179,134,2,128,160,6,128,70,85,44,22,99,106,106,106,207,168,227,104,98,186,183,183,55,102,102,102,12,5,0,65,
13,0,141,174,84,42,69,79,79,143,168,110,16,181,90,205,158,211,0,8,106,0,104,6,233,116,58,214,215,215,13,226,136,212,95,192,200,102,179,177,182,182,102,40,0,8,106,0,104,22,107,107,107,59,190,81,218,93,
234,195,143,233,231,158,123,46,150,150,150,12,5,0,65,13,0,205,166,92,46,199,169,83,167,68,245,17,89,91,91,179,231,52,0,130,26,0,154,81,54,155,141,114,185,108,16,135,164,254,130,197,224,224,96,188,249,
230,155,134,2,128,160,6,128,102,245,214,91,111,197,208,208,208,158,209,199,193,56,126,252,120,84,171,85,131,0,64,80,3,64,179,171,84,42,209,217,217,105,16,7,168,254,66,69,177,88,140,233,233,105,67,1,64,
80,3,64,179,43,20,10,81,42,149,246,140,63,246,55,166,95,122,233,37,49,13,128,160,6,128,86,50,53,53,21,253,253,253,162,250,0,117,116,116,68,173,86,139,225,225,97,195,0,64,80,3,64,171,72,165,82,113,229,
202,149,72,36,44,125,251,169,254,194,196,196,196,68,172,172,172,24,10,0,130,26,0,90,77,177,88,140,66,161,176,103,12,242,108,49,221,221,221,109,207,105,0,4,53,0,180,178,133,133,133,232,233,233,17,213,251,
108,109,109,45,198,198,198,12,2,0,65,13,0,173,42,157,78,199,250,250,186,65,60,163,250,11,17,175,189,246,90,92,189,122,213,80,0,16,212,0,208,234,214,214,214,34,147,201,236,25,135,60,153,206,206,78,123,
78,3,32,168,1,160,157,148,203,229,56,117,234,148,168,126,10,245,179,42,149,74,49,53,53,101,40,0,8,106,0,104,23,217,108,214,55,82,63,99,76,247,245,245,237,248,146,55,0,16,212,0,208,38,174,93,187,22,131,
131,131,123,198,34,63,206,158,211,0,32,168,1,104,115,235,235,235,209,217,217,105,16,143,161,254,130,67,62,159,143,229,229,101,67,1,64,80,3,64,187,42,20,10,81,42,149,246,140,70,246,142,233,238,238,238,
29,51,3,0,65,13,0,109,106,106,106,42,250,251,251,69,245,99,170,86,171,246,156,6,0,65,13,0,17,169,84,42,174,92,185,18,137,132,101,113,47,245,23,24,46,93,186,20,175,191,254,186,161,0,128,160,6,128,239,20,
139,197,29,223,88,237,46,245,110,39,78,156,136,90,173,102,16,0,32,168,1,96,167,133,133,133,232,233,233,17,213,117,234,103,176,184,184,24,249,124,222,11,5,0,4,53,0,236,148,78,167,99,125,125,221,32,246,
136,233,179,103,207,70,46,151,51,20,0,16,212,0,176,183,181,181,181,24,29,29,221,51,42,219,213,247,123,78,167,82,41,47,16,0,16,212,0,240,195,86,87,87,227,212,169,83,109,29,213,245,127,230,169,169,169,88,
88,88,240,194,0,0,65,13,0,63,46,155,205,198,202,202,74,219,254,249,235,99,250,204,153,51,246,156,6,0,65,13,0,143,239,218,181,107,49,56,56,184,103,100,182,147,71,31,129,7,0,4,53,0,252,159,214,215,215,163,
179,179,179,173,254,204,245,23,14,134,135,135,227,202,149,43,94,8,0,32,168,1,224,201,20,10,133,29,143,59,183,211,93,234,19,39,78,196,234,234,170,23,1,0,8,106,0,120,58,83,83,83,209,223,223,223,22,81,93,
255,103,91,90,90,178,77,22,0,8,106,0,120,122,169,84,42,174,92,185,18,137,68,107,47,153,245,49,125,238,220,185,152,156,156,116,240,1,64,80,3,192,179,41,22,139,81,40,20,246,140,207,150,251,96,144,72,216,
115,26,0,4,53,0,236,159,133,133,133,232,233,233,105,201,168,174,255,179,76,79,79,219,38,11,0,4,53,0,236,159,116,58,29,235,235,235,45,247,231,122,116,207,233,249,249,121,7,27,0,4,53,0,236,175,71,247,100,
110,181,71,191,171,213,170,61,167,1,64,80,3,192,193,88,93,93,141,83,167,78,181,68,84,215,255,222,91,245,14,60,0,8,106,0,104,16,217,108,54,86,86,86,90,234,207,212,213,213,213,114,127,38,0,16,212,0,208,
128,174,93,187,22,131,131,131,15,127,110,198,187,212,245,191,231,149,149,21,123,78,3,128,160,6,128,195,177,190,190,30,157,157,157,77,249,123,175,143,233,129,129,129,200,102,179,14,40,0,8,106,0,56,28,133,
66,33,138,197,226,158,145,218,44,142,29,59,22,181,90,45,146,201,164,3,10,0,130,26,0,14,207,212,212,84,244,247,247,55,85,84,215,255,30,103,102,102,98,110,110,206,129,4,0,65,13,0,135,107,120,120,56,174,
92,185,18,137,68,115,44,167,245,49,221,219,219,107,207,105,0,16,212,0,112,116,138,197,98,76,77,77,237,25,173,141,172,86,171,197,200,200,136,3,8,0,130,26,0,142,78,169,84,138,222,222,222,134,142,234,250,
223,211,248,248,120,172,173,173,57,112,0,32,168,1,224,104,165,211,233,88,95,95,111,216,223,95,125,76,63,247,220,115,177,188,188,236,160,1,128,160,6,128,198,176,186,186,186,99,251,169,70,125,244,187,82,
169,216,38,11,0,4,53,0,52,150,149,149,149,56,125,250,116,67,69,117,253,239,225,210,165,75,241,198,27,111,56,80,0,32,168,1,160,177,140,141,141,69,173,86,107,200,223,91,103,103,103,84,42,21,7,9,0,4,53,0,
52,166,141,141,141,72,165,82,15,127,62,202,187,212,245,191,246,210,210,210,142,111,35,7,0,4,53,0,52,156,74,165,18,39,79,158,60,210,168,174,255,53,207,158,61,27,185,173,29,163,139,0,0,2,200,73,68,65,84,
92,206,129,1,0,65,13,0,141,109,114,114,50,202,229,114,99,44,244,137,68,172,175,175,239,184,107,14,0,8,106,0,104,88,63,251,217,207,98,112,112,240,225,207,135,121,151,186,254,215,154,153,153,137,82,169,
228,128,0,128,160,6,128,230,177,190,190,30,157,157,157,135,250,107,214,199,116,111,111,111,204,207,207,59,16,0,32,168,1,160,185,20,10,133,88,88,88,216,51,118,15,67,181,90,141,116,58,237,64,0,128,160,6,
128,230,140,234,11,23,46,28,74,84,215,255,187,179,217,172,109,178,0,64,80,3,64,243,74,165,82,81,171,213,226,216,177,99,7,250,235,212,199,116,119,119,119,44,47,47,27,62,0,8,106,0,104,110,243,243,243,49,
51,51,179,103,252,30,132,181,181,181,24,31,31,55,120,0,16,212,0,208,26,81,253,210,75,47,29,72,84,215,255,187,46,93,186,20,87,175,94,53,112,0,16,212,0,208,26,70,70,70,98,99,99,35,58,58,14,238,230,244,137,
19,39,162,86,171,25,54,0,8,106,0,104,45,75,75,75,145,203,229,30,254,188,31,119,169,235,255,29,75,75,75,145,207,231,13,26,0,4,53,0,180,102,84,247,244,244,236,75,84,215,255,127,207,157,59,23,147,147,147,
6,12,0,130,26,0,90,211,232,232,232,190,111,103,149,72,36,162,86,171,69,42,149,50,96,0,16,212,0,208,186,106,181,90,140,142,142,62,252,249,105,238,82,215,255,127,102,103,103,163,84,42,25,44,0,8,106,0,104,
125,171,171,171,113,234,212,169,167,138,234,250,127,182,183,183,55,230,231,231,13,20,0,4,53,0,180,135,108,54,27,171,171,171,207,252,239,169,213,106,49,50,50,98,160,0,32,168,1,160,125,188,249,230,155,113,
249,242,229,135,63,63,206,93,234,250,127,102,98,98,34,214,214,214,12,18,0,4,53,0,180,159,106,181,26,93,93,93,143,245,207,214,199,116,119,119,119,44,45,45,25,32,0,8,106,0,104,79,185,92,110,71,24,63,238,
127,75,93,169,84,98,124,124,220,0,1,64,80,3,64,251,154,156,156,140,193,193,193,31,141,234,250,255,237,242,229,203,241,211,159,254,212,224,0,64,80,3,64,123,75,38,147,177,190,190,30,157,157,157,123,254,
253,250,152,62,121,242,100,212,106,53,67,3,0,65,13,0,68,68,20,10,133,199,122,244,123,117,117,53,38,39,39,13,12,0,4,53,0,240,189,92,46,23,3,3,3,59,162,186,62,172,47,93,186,20,215,174,93,51,40,0,16,212,
0,64,189,84,42,21,175,191,254,250,158,143,126,159,56,113,194,163,222,0,32,168,1,128,31,50,53,53,21,11,11,11,187,254,247,149,149,149,200,231,243,6,4,0,130,26,0,248,33,133,66,33,46,92,184,240,240,231,139,
23,47,198,196,196,132,193,0,128,160,6,0,126,76,42,149,138,55,222,120,35,78,158,60,25,93,93,93,113,229,202,149,72,38,147,6,3,0,13,230,184,17,0,64,227,201,231,243,241,231,63,255,57,58,58,58,162,80,40,24,
8,0,52,160,255,7,39,108,193,99,109,22,92,173,0,0,0,0,73,69,78,68,174,66,96,130,0,0};

const char* VASTHeaderComponent::dragdrop_png = (const char*) resource_VASTHeaderComponent_dragdrop_png;
const int VASTHeaderComponent::dragdrop_pngSize = 25439;

// JUCER_RESOURCE: dragdrop_over_png, 25586, "../../Resources/dragdrop_over.png"
static const unsigned char resource_VASTHeaderComponent_dragdrop_over_png[] = { 137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,3,212,0,0,3,210,8,6,0,0,0,154,141,146,153,0,0,0,6,98,75,71,68,0,255,0,
255,0,255,160,189,167,147,0,0,0,9,112,72,89,115,0,0,11,19,0,0,11,19,1,0,154,156,24,0,0,0,7,116,73,77,69,7,226,6,24,7,32,9,100,162,16,122,0,0,32,0,73,68,65,84,120,218,236,221,89,115,220,229,153,240,225,
27,7,7,106,226,153,34,149,144,204,84,146,147,80,24,6,207,224,194,196,108,198,6,98,73,158,192,76,21,53,199,243,53,82,53,31,96,106,82,83,36,7,28,112,56,169,16,134,0,73,160,42,193,56,234,127,183,118,169,
101,237,91,183,150,22,54,182,49,54,6,47,120,149,45,89,210,123,192,43,167,109,201,32,219,90,122,185,174,42,14,218,109,180,220,189,88,191,126,90,207,115,215,252,252,124,0,0,165,231,189,247,222,139,141,27,
55,198,191,253,219,191,25,6,0,148,160,187,141,0,0,74,79,71,71,71,164,211,233,216,184,113,99,252,224,7,63,136,109,219,182,25,10,0,148,152,13,70,0,0,165,101,108,108,44,246,238,221,27,179,179,179,113,249,
242,229,216,183,111,95,76,78,78,26,12,0,8,106,0,224,171,180,182,182,198,169,83,167,174,93,62,114,228,72,12,13,13,25,12,0,8,106,0,224,102,26,27,27,163,175,175,111,209,159,55,55,55,71,127,127,191,1,1,128,
160,6,0,110,52,54,54,22,169,84,106,201,235,166,167,167,35,73,18,67,2,0,65,13,0,220,168,181,181,53,206,156,57,115,237,242,43,175,188,50,255,202,43,175,92,59,142,227,224,193,131,241,254,251,239,27,20,0,
8,106,0,96,65,75,75,203,117,111,233,46,14,233,98,153,76,38,134,135,135,13,12,0,4,53,0,48,62,62,30,245,245,245,49,63,191,100,67,95,23,215,83,83,83,209,212,212,100,104,0,32,168,1,128,142,142,142,235,118,
245,94,106,117,186,248,207,242,249,188,223,167,6,0,65,13,0,98,186,167,167,231,43,99,122,41,73,146,196,232,232,168,1,2,128,160,6,128,234,51,62,62,30,251,246,237,139,185,185,185,101,253,253,226,216,62,119,
238,92,100,179,89,67,4,0,65,13,0,213,167,179,179,51,62,255,252,243,37,131,121,57,81,221,219,219,27,237,237,237,6,9,0,130,26,0,170,71,54,155,141,174,174,174,91,138,233,27,205,205,205,69,146,36,81,40,20,
12,20,0,4,53,0,84,190,137,137,137,248,203,95,254,178,236,183,122,223,168,56,190,63,253,244,211,235,142,219,2,0,4,53,0,84,172,158,158,158,56,113,226,196,146,129,124,59,81,221,214,214,22,125,125,125,6,11,
0,130,26,0,42,87,119,119,247,117,155,137,221,78,76,223,104,102,102,38,210,233,180,225,2,128,160,6,128,202,52,57,57,25,233,116,58,174,94,189,186,34,31,175,56,198,15,30,60,24,31,124,240,129,33,3,128,160,
6,128,202,51,50,50,18,135,15,31,94,50,136,87,66,38,147,137,92,46,103,208,0,32,168,1,160,114,12,13,13,69,115,115,243,138,199,116,241,199,185,120,241,98,180,181,181,25,54,0,8,106,0,168,28,13,13,13,113,249,
242,229,85,249,216,197,81,61,56,56,120,93,184,3,0,130,26,0,202,86,42,149,138,241,241,241,37,3,120,165,205,207,207,71,146,36,49,49,49,97,240,0,32,168,1,160,124,229,243,249,72,146,100,213,63,79,113,164,
159,60,121,50,186,187,187,13,31,0,4,53,0,148,175,214,214,214,56,127,254,252,146,225,187,154,81,157,205,102,163,171,171,203,13,0,0,130,26,0,202,51,166,7,7,7,215,36,166,111,52,59,59,27,73,146,196,228,228,
164,27,2,0,4,53,0,148,143,241,241,241,168,175,175,143,249,249,249,53,253,188,197,209,254,241,199,31,71,62,159,119,99,0,128,160,6,128,242,209,213,213,21,39,79,158,92,50,116,215,50,170,155,154,154,98,120,
120,216,13,2,0,130,26,0,202,35,166,139,127,127,121,45,99,250,70,83,83,83,142,209,2,0,65,13,0,165,175,80,40,68,42,149,138,171,87,175,174,235,215,81,28,241,185,92,46,50,153,140,27,7,0,4,53,0,148,174,161,
161,161,56,122,244,232,146,97,187,158,210,233,116,140,141,141,185,129,0,64,80,3,64,233,25,28,28,140,214,214,214,146,137,233,226,207,127,230,204,153,216,191,127,191,27,9,0,4,53,0,148,158,116,58,29,87,174,
92,41,169,175,169,56,170,187,186,186,162,179,179,211,13,5,0,130,26,0,74,71,42,149,186,238,204,231,82,121,171,119,177,185,185,185,168,175,175,143,66,161,224,6,3,0,65,13,0,235,111,116,116,180,164,55,253,
42,142,251,227,199,143,199,208,208,144,27,13,0,4,53,0,172,191,182,182,182,56,123,246,236,146,1,91,138,81,221,210,210,18,3,3,3,110,56,0,16,212,0,176,126,218,219,219,163,191,191,191,164,99,250,70,211,211,
211,209,216,216,232,198,3,0,65,13,0,235,163,80,40,68,58,157,142,249,249,249,178,248,122,139,99,127,98,98,34,234,235,235,221,136,0,32,168,1,96,237,245,247,247,199,241,227,199,151,12,214,114,144,201,100,
98,116,116,212,13,9,0,130,26,0,214,206,192,192,64,116,116,116,148,93,76,23,127,157,231,206,157,139,108,54,235,198,4,0,65,13,0,107,39,147,201,148,220,153,211,183,19,213,189,189,189,215,189,48,0,0,8,106,
0,88,53,251,246,237,43,249,51,167,151,107,110,110,46,146,36,113,54,53,0,8,106,0,88,93,249,124,62,26,26,26,202,254,251,112,54,53,0,8,106,0,88,83,237,237,237,113,254,252,249,37,195,180,156,163,218,217,212,
0,32,168,1,96,85,99,186,220,206,156,94,174,233,233,233,104,110,110,118,35,3,128,160,6,128,149,85,40,20,34,149,74,149,205,153,211,203,85,252,162,192,216,216,88,164,82,41,55,54,0,8,106,0,88,57,125,125,125,
113,226,196,137,37,67,180,146,56,155,26,0,4,53,0,172,152,254,254,254,178,60,115,122,185,138,191,159,179,103,207,198,254,253,251,221,232,0,32,168,1,224,206,101,50,153,152,158,158,174,232,239,177,56,170,
187,187,187,35,155,205,186,225,1,64,80,3,192,237,219,183,111,95,124,248,225,135,75,134,103,165,154,155,155,139,76,38,115,221,89,219,0,128,160,6,128,101,171,148,51,167,151,171,248,197,130,163,71,143,198,
240,240,176,59,1,0,8,106,0,184,117,149,116,230,244,237,68,117,75,75,75,12,13,13,185,35,0,128,160,6,128,229,107,107,107,171,216,51,167,151,235,242,229,203,206,166,6,0,65,13,0,203,87,40,20,34,73,146,138,
59,115,122,185,138,95,60,200,231,243,145,201,100,220,41,0,64,80,3,192,215,235,237,237,173,138,51,167,151,43,157,78,199,248,248,184,59,6,0,8,106,0,184,185,190,190,190,138,62,115,122,185,138,191,239,51,
103,206,56,155,26,0,4,53,0,124,181,134,134,134,152,153,153,49,136,27,162,186,171,171,75,84,3,128,160,6,128,165,237,221,187,183,234,206,156,94,174,217,217,217,72,146,196,217,212,0,32,168,1,224,122,249,
124,62,26,27,27,13,226,6,55,158,77,157,203,229,12,5,0,4,53,0,252,85,107,107,107,92,184,112,97,201,144,20,213,127,157,69,115,115,115,12,15,15,27,10,0,8,106,0,248,50,166,7,7,7,197,244,50,76,77,77,69,75,
75,139,65,0,128,160,6,160,218,77,76,76,84,245,153,211,203,85,252,34,195,200,200,72,52,52,52,24,10,0,130,218,8,0,168,102,189,189,189,241,217,103,159,45,25,142,220,92,146,36,206,166,6,64,80,27,1,0,213,170,
175,175,47,178,217,172,152,94,166,27,207,166,238,238,238,54,20,0,4,53,0,84,163,76,38,227,204,233,59,136,234,206,206,206,232,234,234,50,20,0,4,53,0,84,147,247,223,127,63,14,28,56,176,100,40,178,60,206,
166,6,64,80,3,64,149,201,229,114,209,212,212,100,16,183,169,248,197,135,143,63,254,56,70,71,71,13,5,0,65,13,0,213,160,165,165,197,153,211,43,24,213,141,141,141,206,166,6,64,80,3,64,165,107,109,109,189,
46,254,196,244,157,155,154,154,138,214,214,86,131,0,64,80,3,64,165,114,230,244,202,42,126,49,98,120,120,56,26,27,27,13,5,0,65,13,0,149,168,187,187,219,153,211,171,40,147,201,56,155,26,0,65,13,0,149,166,
167,167,39,58,59,59,197,244,10,43,158,227,201,147,39,163,183,183,215,80,0,16,212,0,80,73,210,233,116,92,189,122,213,32,86,57,170,179,217,108,244,244,244,24,10,0,130,26,0,42,193,159,254,244,167,56,116,
232,208,146,1,200,202,186,122,245,106,164,211,105,131,0,64,80,3,64,185,27,25,25,113,230,244,26,40,126,145,226,208,161,67,241,254,251,239,27,10,0,130,26,0,202,89,115,115,115,92,186,116,105,201,240,99,245,
52,54,54,70,46,151,51,8,0,4,53,0,148,163,166,166,166,24,25,25,17,211,107,164,120,190,23,47,94,140,182,182,54,67,1,64,80,3,64,185,25,31,31,143,36,73,12,98,29,163,122,112,112,48,90,90,90,12,5,0,65,13,0,
229,164,171,171,43,78,157,58,181,100,232,177,54,230,231,231,35,73,146,152,152,152,48,12,0,4,53,0,148,131,238,238,238,232,234,234,18,211,235,164,120,222,159,127,254,121,12,12,12,24,10,0,130,26,0,74,221,
228,228,164,51,167,75,44,170,219,219,219,163,191,191,223,80,0,16,212,0,80,202,114,185,92,28,62,124,120,201,176,99,125,76,79,79,71,99,99,163,65,0,32,168,1,160,84,13,15,15,95,183,9,150,152,94,95,197,243,
47,20,10,81,95,95,111,40,0,8,106,0,40,69,77,77,77,215,157,57,77,105,201,100,50,49,58,58,106,16,0,8,106,0,40,37,141,141,141,145,207,231,175,93,182,58,93,26,138,111,135,115,231,206,69,103,103,167,161,0,
32,168,1,160,84,56,115,186,124,162,186,167,167,39,178,217,172,161,0,32,168,1,160,20,116,118,118,198,233,211,167,151,12,56,74,203,220,220,92,100,50,153,152,156,156,52,12,0,4,53,0,172,167,174,174,174,232,
238,238,22,211,37,174,248,118,57,122,244,104,140,140,140,24,10,0,130,26,0,214,203,194,153,211,179,179,179,134,81,102,81,221,220,220,28,195,195,195,134,2,128,160,6,128,245,48,60,60,28,71,142,28,89,50,216,
40,109,151,47,95,142,230,230,102,131,0,64,80,3,192,122,196,116,107,107,171,152,46,51,197,183,83,46,151,139,134,134,6,67,1,64,80,3,192,90,106,108,108,140,169,169,41,131,40,115,73,146,196,248,248,184,65,
0,32,168,1,96,45,52,52,52,196,232,232,232,181,203,86,167,203,75,241,237,117,230,204,153,232,233,233,49,20,0,4,53,0,172,182,177,177,177,72,165,82,6,81,65,81,221,217,217,41,170,1,16,212,0,176,218,58,59,
59,227,139,47,190,88,50,204,40,79,87,175,94,141,76,38,99,16,0,8,106,0,88,205,152,118,230,116,229,40,190,253,62,250,232,163,216,187,119,175,161,0,32,168,1,96,165,21,10,133,72,146,36,230,230,230,12,163,
66,53,52,52,68,46,151,51,8,0,4,53,0,172,164,161,161,161,248,228,147,79,174,93,182,58,93,25,138,111,199,139,23,47,70,123,123,187,161,0,32,168,1,96,165,12,14,14,70,91,91,155,152,174,130,168,30,24,24,184,
238,124,113,0,16,212,0,112,7,26,27,27,227,242,229,203,6,81,5,230,231,231,35,157,78,71,161,80,48,12,0,4,53,0,220,137,36,73,98,124,124,252,218,101,171,211,149,169,248,118,61,113,226,68,12,12,12,24,10,0,
130,26,0,110,215,216,216,152,227,148,170,52,170,219,218,218,68,53,0,130,26,0,110,87,71,71,135,51,167,171,212,244,244,116,52,54,54,26,4,0,130,26,0,110,85,103,103,103,244,246,246,138,233,42,83,124,59,79,
76,76,68,125,125,189,161,0,32,168,1,96,185,10,133,66,164,82,41,103,78,19,153,76,38,70,71,71,13,2,0,65,13,0,203,49,56,56,24,199,142,29,187,118,217,234,116,117,41,190,189,207,157,59,23,251,247,239,55,20,
0,4,53,0,44,39,166,157,57,77,241,237,222,211,211,35,170,1,16,212,0,240,117,50,153,76,92,185,114,197,32,184,102,118,118,54,146,36,137,201,201,73,195,0,64,80,3,192,82,146,36,137,66,161,112,237,178,213,233,
234,86,124,251,31,61,122,52,114,185,156,161,0,32,168,1,224,70,163,163,163,145,78,167,13,130,155,70,117,115,115,115,12,15,15,27,10,0,130,26,0,138,181,181,181,197,217,179,103,151,12,41,136,136,152,154,154,
138,150,150,22,131,0,64,80,3,64,113,76,247,247,247,139,105,150,84,124,127,24,25,25,137,134,134,6,67,1,64,80,3,64,161,80,136,36,73,98,126,94,67,179,60,13,13,13,49,49,49,97,16,0,8,106,0,170,91,111,111,111,
156,56,113,226,218,101,171,211,44,165,248,126,113,242,228,201,232,233,233,49,20,0,4,53,0,213,171,175,175,47,58,58,58,196,52,183,28,213,217,108,54,122,123,123,13,5,0,65,13,64,117,202,100,50,49,51,51,99,
16,220,178,171,87,175,218,21,30,0,65,13,64,117,122,255,253,247,227,192,129,3,215,46,91,157,102,57,138,239,39,31,125,244,81,124,240,193,7,134,2,128,160,6,160,122,228,114,185,104,106,106,50,8,238,88,38,
147,137,92,46,103,16,0,8,106,0,170,67,75,75,75,92,184,112,225,218,101,171,211,220,138,226,251,203,197,139,23,175,251,61,124,0,16,212,0,84,116,76,15,15,15,139,105,86,44,170,251,251,251,163,189,189,221,
80,0,16,212,0,84,174,241,241,241,168,175,175,119,230,52,43,106,126,126,62,146,36,137,66,161,96,24,0,8,106,0,42,83,119,119,119,156,58,117,234,218,101,171,211,220,137,226,251,207,167,159,126,26,131,131,
131,134,2,128,160,6,160,50,99,122,255,254,253,98,154,85,139,234,214,214,214,24,24,24,48,20,0,4,53,0,149,99,114,114,50,210,233,116,92,189,122,213,48,88,53,211,211,211,118,143,7,64,80,3,80,89,114,185,92,
28,62,124,248,218,101,171,211,172,164,226,251,211,248,248,120,164,82,41,67,1,64,80,3,80,254,134,135,135,163,165,165,69,76,179,102,26,26,26,98,108,108,204,32,0,16,212,0,148,183,198,198,198,184,116,233,
146,65,176,170,138,95,168,249,226,139,47,174,251,125,125,0,16,212,0,148,101,76,143,142,142,46,25,61,176,154,81,125,227,38,120,0,32,168,1,40,27,99,99,99,126,151,149,117,51,59,59,27,73,146,196,228,228,164,
97,0,32,168,1,40,47,217,108,54,206,156,57,115,237,178,213,105,214,66,241,253,236,232,209,163,145,207,231,13,5,0,65,13,64,249,232,236,236,140,158,158,30,49,205,186,71,117,83,83,83,12,15,15,27,10,0,130,
26,128,210,87,40,20,34,73,146,152,155,155,51,12,214,221,212,212,84,180,182,182,26,4,0,130,26,128,210,55,56,56,24,159,124,242,201,181,203,86,167,89,15,197,247,187,225,225,225,104,110,110,54,20,0,4,53,0,
165,29,211,109,109,109,98,154,146,147,36,73,76,76,76,24,4,0,130,26,128,210,148,201,100,226,202,149,43,6,65,73,40,126,65,231,228,201,147,209,219,219,107,40,0,8,106,0,74,79,42,149,138,66,161,176,100,204,
64,41,68,117,54,155,21,213,0,8,106,0,74,203,232,232,104,100,50,25,131,160,164,205,204,204,184,159,2,32,168,1,40,45,109,109,109,113,246,236,217,107,151,173,78,83,74,138,239,143,7,15,30,140,125,251,246,
25,10,0,130,26,128,210,136,233,254,254,126,49,77,217,104,104,104,136,124,62,111,16,0,8,106,0,214,207,196,196,68,36,73,18,243,243,26,154,210,86,252,66,207,249,243,231,163,163,163,195,80,0,16,212,0,172,
159,190,190,190,56,113,226,196,146,209,2,165,28,213,125,125,125,162,26,0,65,13,192,250,197,116,113,144,136,105,202,201,252,252,252,162,157,233,1,64,80,3,176,38,50,153,76,204,204,204,24,4,101,165,248,133,
159,79,63,253,52,134,134,134,12,5,0,65,13,192,218,121,255,253,247,227,192,129,3,75,70,10,148,83,84,183,180,180,196,192,192,128,161,0,32,168,1,88,125,185,92,46,154,154,154,12,130,138,48,61,61,29,205,205,
205,6,1,128,160,6,96,245,53,55,55,199,133,11,23,174,93,182,58,77,57,42,190,223,142,141,141,69,38,147,49,20,0,4,53,0,171,27,211,195,195,195,98,154,138,147,78,167,99,124,124,220,32,0,16,212,0,172,188,241,
241,241,72,165,82,6,65,197,40,126,65,232,204,153,51,177,127,255,126,67,1,64,80,3,176,242,186,186,186,226,212,169,83,75,198,8,84,66,84,119,117,117,69,87,87,151,161,0,32,168,1,88,57,221,221,221,215,133,
134,152,166,18,205,206,206,70,146,36,49,57,57,105,24,0,8,106,0,238,220,228,228,100,164,211,233,184,122,245,170,97,80,145,138,95,32,250,248,227,143,99,116,116,212,80,0,16,212,0,220,185,92,46,23,135,15,
31,94,50,62,160,18,163,186,177,177,241,186,205,247,0,64,80,3,112,203,134,135,135,163,165,165,69,76,83,85,166,166,166,162,173,173,205,32,0,16,212,0,220,190,198,198,198,184,116,233,146,65,80,21,138,95,48,
186,241,197,36,0,16,212,0,44,91,67,67,195,117,191,75,106,117,154,106,50,63,63,31,73,146,196,196,196,132,97,0,32,168,1,88,190,177,177,49,103,78,83,149,138,95,56,250,252,243,207,163,175,175,207,80,0,16,
212,0,44,95,54,155,141,47,190,248,98,201,200,128,106,138,234,142,142,14,81,13,128,160,6,96,121,58,59,59,163,167,167,71,76,67,68,204,204,204,68,58,157,54,8,0,4,53,0,95,173,80,40,68,146,36,49,55,55,103,
24,84,181,226,23,146,14,30,60,24,127,249,203,95,12,5,0,65,13,192,205,13,14,14,198,39,159,124,178,100,84,64,53,203,100,50,215,109,210,7,0,130,26,128,235,98,186,248,236,93,49,77,181,43,126,12,156,63,127,
62,58,58,58,12,5,0,65,13,192,98,153,76,38,174,92,185,98,16,112,147,168,238,235,235,19,213,0,8,106,0,174,151,74,165,162,80,40,44,25,17,192,151,230,230,230,34,73,146,235,30,43,0,8,106,0,170,88,62,159,143,
36,73,12,2,110,162,248,5,166,227,199,143,199,240,240,176,161,0,32,168,1,136,104,111,111,143,243,231,207,47,25,15,192,226,199,69,75,75,75,12,14,14,26,10,0,130,26,160,154,181,181,181,69,127,127,191,152,
134,91,112,229,202,149,104,105,105,49,8,0,4,53,64,181,154,152,152,136,36,73,98,126,94,67,195,114,20,191,224,148,207,231,35,147,201,24,10,128,160,6,160,26,245,246,246,198,137,19,39,150,140,5,224,235,165,
211,233,24,31,31,55,8,0,65,13,64,53,233,235,235,139,108,54,43,166,225,22,21,63,86,206,156,57,19,93,93,93,134,2,32,168,1,168,38,153,76,38,102,102,102,12,2,238,48,170,247,239,223,47,170,1,4,53,0,213,226,
207,127,254,115,28,56,112,96,201,56,0,110,205,236,236,108,36,73,18,147,147,147,134,1,32,168,1,168,100,35,35,35,209,212,212,100,16,112,135,138,95,136,250,248,227,143,99,108,108,204,80,0,4,53,0,149,172,
181,181,53,46,94,188,184,100,20,0,183,31,213,77,77,77,49,50,50,98,40,0,130,26,128,74,141,233,225,225,97,49,13,171,224,226,197,139,209,222,222,110,16,0,130,26,128,74,227,204,105,88,29,197,47,76,13,14,14,
70,75,75,139,161,0,8,106,0,42,73,79,79,79,124,246,217,103,75,70,0,176,50,230,231,231,35,73,146,152,152,152,48,12,0,65,13,64,37,232,235,235,139,206,206,78,49,13,171,164,248,49,245,249,231,159,71,127,127,
191,161,0,8,106,0,42,65,58,157,118,230,52,172,97,84,183,183,183,71,95,95,159,161,0,8,106,0,202,217,159,255,252,231,56,120,240,224,146,63,244,3,171,99,102,102,198,241,116,0,130,26,128,114,150,203,229,162,
185,185,217,32,96,141,20,191,96,85,40,20,162,190,190,222,80,0,4,53,0,229,168,165,165,37,46,92,184,176,228,15,251,192,234,203,100,50,49,58,58,106,16,0,130,26,128,114,226,204,105,88,31,197,143,181,115,231,
206,93,183,33,32,0,130,26,128,18,231,204,105,40,157,168,238,233,233,137,108,54,107,40,0,130,26,128,114,224,204,105,40,29,115,115,115,145,74,165,162,80,40,24,6,128,160,6,160,148,57,115,26,74,67,241,99,
239,248,241,227,49,50,50,98,40,0,130,26,128,82,150,201,100,156,57,13,37,24,213,45,45,45,49,52,52,100,40,0,130,26,128,82,244,254,251,239,199,129,3,7,150,252,97,30,88,95,151,47,95,142,150,150,22,131,0,16,
212,0,148,154,124,62,31,77,77,77,6,1,37,166,248,133,173,92,46,23,13,13,13,134,2,32,168,1,40,37,206,156,134,242,144,36,73,140,143,143,27,4,128,160,6,160,20,180,182,182,94,247,187,153,98,26,74,75,241,99,
242,204,153,51,209,221,221,109,40,0,130,26,128,245,230,204,105,40,191,168,238,236,236,20,213,0,130,26,128,245,214,219,219,235,204,105,40,51,179,179,179,145,201,100,12,2,64,80,3,176,94,250,250,250,34,155,
205,138,105,40,19,197,143,209,67,135,14,197,251,239,191,111,40,0,130,26,128,245,224,204,105,40,111,141,141,141,145,203,229,12,2,64,80,3,176,150,246,238,221,235,204,105,40,67,197,143,213,139,23,47,70,123,
123,187,161,0,8,106,0,214,74,62,159,143,198,198,70,131,128,10,136,234,129,129,129,104,105,105,49,20,0,65,13,192,90,104,109,109,117,230,52,84,136,249,249,249,72,146,36,38,38,38,12,3,64,80,3,176,154,218,
218,218,98,112,112,80,76,67,153,43,126,236,126,254,249,231,215,61,174,1,16,212,0,172,48,103,78,67,229,70,117,91,91,91,12,12,12,24,10,128,160,6,96,53,244,246,246,198,137,19,39,150,252,97,28,40,111,211,
211,211,246,70,0,16,212,0,172,6,103,78,67,101,42,126,44,79,76,76,68,125,125,189,161,0,8,106,0,86,146,51,167,161,122,30,235,163,163,163,6,1,32,168,1,88,9,206,156,134,202,86,252,152,62,119,238,92,236,223,
191,223,80,0,4,53,0,119,202,153,211,80,125,81,221,221,221,125,221,175,120,0,32,168,1,184,13,109,109,109,206,156,134,42,51,55,55,23,153,76,38,38,39,39,13,3,64,80,3,112,187,49,93,124,140,142,152,134,202,
86,252,24,63,122,244,104,228,114,57,67,1,16,212,0,220,42,103,78,131,168,110,106,106,138,225,225,97,67,1,16,212,0,220,138,190,190,62,103,78,67,149,187,124,249,114,180,180,180,24,4,128,160,6,224,86,98,186,
163,163,67,76,67,149,42,126,204,143,140,140,68,67,67,131,161,0,8,106,0,150,163,161,161,193,153,211,192,53,233,116,58,198,199,199,13,2,64,80,3,240,85,62,248,224,131,248,240,195,15,175,93,182,58,13,213,
169,248,177,127,250,244,233,232,237,237,53,20,0,65,13,192,205,56,115,26,184,89,84,103,179,89,81,13,32,168,1,184,153,182,182,182,56,127,254,252,146,63,76,3,213,237,234,213,171,145,78,167,13,2,64,80,3,176,
84,76,59,115,26,184,81,241,115,193,71,31,125,20,31,124,240,129,161,0,8,106,0,22,20,10,5,103,78,3,203,146,201,100,34,151,203,25,4,128,160,6,32,194,153,211,192,87,43,126,78,184,120,241,226,117,199,234,1,
32,168,1,170,86,127,127,127,180,183,183,139,105,96,217,81,221,223,223,31,173,173,173,134,2,32,168,1,170,91,38,147,113,230,52,112,75,230,231,231,35,157,78,71,161,80,48,12,0,65,13,80,157,246,237,219,231,
204,105,96,217,138,159,35,78,156,56,17,131,131,131,134,2,32,168,1,170,79,62,159,143,134,134,6,131,0,110,59,170,91,91,91,175,59,29,0,0,65,13,80,21,218,219,219,157,57,13,220,145,233,233,233,104,106,106,
50,8,0,65,13,80,93,49,221,223,223,47,166,129,219,82,252,156,49,62,62,30,169,84,202,80,0,4,53,64,229,43,20,10,145,74,165,156,57,13,172,152,76,38,19,163,163,163,6,1,32,168,1,42,155,51,167,129,149,80,252,
220,113,246,236,217,232,234,234,50,20,0,65,13,80,185,250,251,251,163,163,163,67,76,3,43,30,213,221,221,221,177,127,255,126,67,1,16,212,0,149,41,147,201,196,244,244,180,65,0,43,110,118,118,54,146,36,137,
201,201,73,195,0,16,212,0,149,37,149,74,57,115,26,88,113,197,207,37,71,143,30,141,124,62,111,40,0,130,26,160,114,140,142,142,70,38,147,49,8,96,213,163,186,169,169,41,134,135,135,13,5,64,80,3,84,134,182,
182,182,56,123,246,236,146,63,252,2,172,164,169,169,169,104,109,109,53,8,0,65,13,80,254,178,217,108,12,12,12,136,105,96,85,21,63,183,12,15,15,71,99,99,163,161,0,8,106,128,242,181,112,230,244,220,220,156,
97,0,107,42,147,201,196,196,196,132,65,0,8,106,128,242,212,223,223,31,199,143,31,191,118,217,234,52,176,154,138,159,99,78,158,60,25,189,189,189,134,2,32,168,1,202,207,224,224,160,51,167,129,117,141,234,
108,54,43,170,1,4,53,64,249,73,167,211,113,229,202,21,131,0,214,205,204,204,140,19,6,0,4,53,64,121,73,165,82,49,57,57,121,237,178,213,105,96,45,21,63,231,28,60,120,48,246,237,219,103,40,0,130,26,160,244,
141,141,141,89,17,2,74,74,58,157,142,92,46,103,16,0,130,26,160,180,57,115,26,40,5,197,207,61,23,47,94,140,108,54,107,40,0,171,224,110,35,0,88,25,217,108,54,250,251,251,197,52,176,38,126,254,243,159,223,
181,220,191,219,215,215,23,29,29,29,177,99,199,14,131,3,88,65,119,205,207,251,121,15,224,78,21,10,133,248,221,239,126,23,199,142,29,51,140,18,227,133,141,242,137,62,86,215,223,255,253,223,199,127,252,
199,127,196,230,205,155,13,3,96,133,120,203,55,192,10,24,24,24,16,211,130,14,179,47,9,119,223,125,119,220,123,239,189,139,254,187,124,249,114,124,248,225,135,6,4,176,146,207,185,70,0,112,103,114,185,92,
140,140,140,196,125,247,221,103,24,37,228,234,213,171,113,225,194,5,131,40,1,155,54,109,138,187,239,190,249,143,28,247,220,115,207,87,254,255,223,252,230,55,99,195,134,13,183,125,253,198,141,27,227,27,
223,248,198,87,6,232,87,125,125,223,248,198,55,98,227,198,141,75,254,249,141,255,223,93,119,221,181,228,247,179,212,215,176,97,195,134,248,230,55,191,185,172,239,103,195,134,13,75,126,13,247,222,123,111,
60,248,224,131,238,100,0,130,26,160,60,253,211,63,253,83,252,215,127,253,151,65,148,152,246,246,246,248,237,111,127,107,16,37,224,229,151,95,142,93,187,118,25,4,0,21,199,91,190,1,0,0,64,80,3,0,0,128,160,
6,0,0,0,65,13,0,0,0,130,26,0,0,0,4,53,0,0,0,32,168,1,0,0,64,80,3,0,0,128,160,6,0,0,0,65,13,0,0,0,130,26,0,0,0,16,212,0,0,0,32,168,1,0,0,64,80,3,0,0,128,160,6,0,0,0,65,13,0,0,0,8,106,0,0,0,16,212,0,0,0,
32,168,1,0,0,64,80,3,0,0,128,160,6,0,0,0,4,53,0,0,0,8,106,0,0,0,16,212,0,0,0,32,168,1,0,0,64,80,3,0,0,0,130,26,0,0,0,4,53,0,0,0,8,106,0,0,0,16,212,0,0,0,32,168,1,0,0,0,65,13,0,0,0,130,26,0,0,0,4,53,0,
0,0,8,106,0,0,0,16,212,0,0,0,128,160,6,0,0,0,65,13,0,0,0,130,26,0,0,0,4,53,0,0,0,8,106,0,0,0,64,80,3,0,0,128,160,6,0,0,0,65,13,0,0,0,130,26,0,0,0,4,53,0,0,0,32,168,1,0,0,64,80,3,0,0,128,160,6,0,0,0,65,
13,0,0,0,130,26,0,0,0,16,212,0,0,0,32,168,1,0,0,64,80,3,0,0,128,160,6,0,0,0,65,13,0,0,0,8,106,0,0,0,16,212,0,0,0,32,168,1,0,0,64,80,3,0,0,128,160,6,0,0,0,4,53,0,0,0,8,106,0,0,0,16,212,0,0,0,32,168,1,0,
0,64,80,3,0,0,0,130,26,0,0,0,4,53,0,0,0,8,106,0,0,0,16,212,0,0,0,32,168,1,0,0,0,65,13,0,0,0,130,26,0,0,0,4,53,0,0,0,8,106,0,0,0,16,212,0,0,0,128,160,6,0,0,0,65,13,0,0,0,130,26,0,0,0,4,53,0,0,0,8,106,0,
0,0,64,80,3,0,0,128,160,6,0,0,0,65,13,0,0,0,130,26,0,0,0,4,53,0,0,0,32,168,1,0,0,64,80,3,0,0,128,160,6,0,0,0,65,13,0,0,0,130,26,0,0,0,4,53,0,0,0,32,168,1,0,0,64,80,3,0,0,128,160,6,0,0,0,65,13,0,0,0,130,
26,0,0,0,16,212,0,0,80,46,114,185,156,33,128,160,6,214,66,161,80,136,189,123,247,26,4,0,84,136,183,223,126,59,254,240,135,63,68,161,80,48,12,16,212,192,106,233,232,232,136,55,223,124,51,90,91,91,13,3,
0,42,196,229,203,151,35,157,78,199,27,111,188,17,237,237,237,6,2,101,228,110,35,128,210,55,54,54,22,217,108,54,186,187,187,99,126,126,62,254,238,239,254,206,80,0,160,194,156,56,113,34,254,239,255,254,
47,242,249,124,236,220,185,51,30,121,228,17,67,1,65,13,220,137,250,250,250,200,100,50,113,238,220,185,107,127,54,63,63,111,48,0,80,129,230,230,230,162,175,175,47,62,252,240,195,168,173,173,141,186,186,
58,67,1,65,13,220,170,193,193,193,104,105,105,137,124,62,191,228,63,182,0,64,229,58,123,246,108,252,241,143,127,140,129,129,129,168,173,173,141,109,219,182,25,10,8,106,224,235,76,78,78,198,208,208,80,
52,55,55,199,244,244,244,146,127,199,10,53,0,84,166,87,94,121,101,254,231,63,255,249,93,11,151,15,28,56,16,71,143,30,141,66,161,16,219,182,109,139,205,155,55,27,18,148,16,155,146,65,9,201,102,179,241,
230,155,111,70,146,36,139,98,250,149,87,94,153,23,212,0,80,29,81,93,124,249,202,149,43,209,216,216,104,211,50,40,65,86,168,161,4,140,141,141,69,87,87,87,116,117,117,197,236,236,236,87,254,163,26,225,45,
223,0,80,45,81,93,188,90,109,211,50,16,212,192,13,82,169,84,100,50,153,56,123,246,236,215,134,244,2,43,212,0,80,157,97,109,211,50,40,45,222,242,13,235,100,112,112,48,94,125,245,213,120,247,221,119,111,
41,166,5,53,0,84,111,88,47,88,216,180,236,127,254,231,127,162,191,191,223,128,96,157,88,161,134,53,54,57,57,25,249,124,62,154,154,154,98,106,106,234,150,66,122,129,183,124,3,64,245,70,181,77,203,160,116,
88,161,134,53,212,221,221,29,239,188,243,78,236,219,183,239,182,99,58,194,10,53,0,8,235,191,178,105,25,172,31,43,212,176,6,110,117,211,177,175,99,133,26,0,68,117,132,77,203,64,80,67,133,107,108,108,140,
36,73,226,244,233,211,119,28,210,197,10,133,130,183,117,1,128,176,182,105,25,172,35,111,249,134,85,50,50,50,18,175,189,246,90,188,253,246,219,43,30,211,17,222,246,13,0,220,252,103,11,155,150,193,218,176,
66,13,43,108,37,54,29,251,170,127,44,23,94,129,22,212,0,192,82,63,103,216,180,12,214,142,21,106,88,65,221,221,221,241,251,223,255,254,142,55,29,91,14,191,71,13,0,124,85,88,47,176,105,25,172,30,43,212,
176,2,38,38,38,162,183,183,55,218,219,219,87,100,211,177,229,176,66,13,0,124,93,84,219,180,12,4,53,148,180,166,166,166,72,146,36,78,157,58,181,38,33,189,192,10,53,0,112,171,97,109,211,50,88,89,222,242,
13,183,105,97,211,177,183,222,122,107,205,99,58,194,10,53,0,112,235,97,189,192,166,101,176,50,172,80,195,109,216,187,119,111,100,50,153,184,116,233,210,154,135,180,160,6,0,238,36,170,109,90,6,43,199,10,
53,220,130,158,158,158,248,239,255,254,239,248,243,159,255,188,174,49,29,225,45,223,0,192,157,133,245,2,155,150,193,237,179,66,13,203,176,176,233,88,71,71,71,92,189,122,117,93,67,122,129,21,106,0,224,
78,163,218,166,101,32,168,97,85,53,55,55,71,146,36,113,242,228,201,155,254,99,36,168,1,128,74,8,107,155,150,129,160,134,21,145,203,229,162,163,163,35,250,251,251,23,197,235,122,134,244,2,111,249,6,0,86,
50,172,139,87,171,23,54,45,27,24,24,136,218,218,218,216,182,109,155,33,129,160,134,229,41,133,77,199,4,53,0,176,214,81,29,97,211,50,184,21,54,37,131,34,189,189,189,241,139,95,252,162,36,54,29,251,58,222,
242,13,0,172,102,88,47,176,105,25,220,156,21,106,136,47,55,29,27,24,24,136,182,182,182,152,153,153,41,233,144,22,212,0,192,90,69,181,77,203,64,80,195,87,106,111,111,143,84,42,21,39,78,156,184,233,63,38,
165,200,91,190,1,128,181,14,107,155,150,129,160,134,136,40,253,77,199,190,142,21,106,0,96,45,195,218,166,101,32,168,33,34,34,234,235,235,35,157,78,199,249,243,231,203,46,164,23,88,161,6,0,214,58,170,35,
108,90,6,197,108,74,70,85,25,24,24,136,95,254,242,151,241,222,123,239,149,117,76,71,88,161,6,0,214,55,172,23,216,180,140,106,102,133,154,170,80,142,155,142,9,106,0,160,212,163,218,166,101,8,106,168,112,
29,29,29,145,74,165,226,211,79,63,189,233,63,6,229,200,91,190,1,128,82,11,107,155,150,33,168,161,66,140,142,142,70,103,103,103,116,119,119,151,229,166,99,95,199,10,53,0,80,74,97,109,211,50,4,53,84,136,
250,250,250,200,100,50,113,238,220,185,138,11,233,5,86,168,1,128,82,139,234,8,155,150,81,93,108,74,70,69,25,24,24,136,95,253,234,87,241,222,123,239,85,116,76,71,88,161,6,0,74,59,172,23,216,180,140,74,
102,133,154,138,80,40,20,98,120,120,56,154,155,155,99,122,122,186,162,67,122,129,21,106,0,160,212,163,218,166,101,8,106,40,113,217,108,54,82,169,84,28,63,126,252,166,79,230,149,200,10,53,0,80,110,97,109,
211,50,4,53,148,136,177,177,177,200,102,179,21,187,233,152,160,6,0,42,41,172,109,90,134,160,134,18,145,74,165,34,157,78,87,252,239,73,127,21,111,249,6,0,202,45,170,35,108,90,70,101,177,41,25,101,101,96,
96,32,126,249,203,95,198,187,239,190,91,213,49,29,97,133,26,0,40,239,176,94,176,176,105,217,239,126,247,187,232,236,236,52,32,202,138,21,106,202,194,228,228,100,12,13,13,85,213,166,99,95,199,10,53,0,80,
238,81,93,188,90,125,236,216,177,120,253,245,215,99,120,120,216,166,101,8,106,88,41,217,108,54,146,36,137,99,199,142,221,244,201,184,26,89,161,6,0,42,45,172,109,90,134,160,134,21,50,54,54,22,93,93,93,
209,213,213,21,179,179,179,66,90,80,3,0,21,28,214,55,219,180,172,174,174,46,30,123,236,49,67,66,80,195,114,53,52,52,68,146,36,113,230,204,25,33,125,19,222,242,13,0,84,90,84,71,44,222,120,31,190,36,0,0,
32,0,73,68,65,84,180,236,215,191,254,117,236,216,177,195,166,101,148,36,155,146,81,82,134,134,134,226,213,87,95,141,119,222,121,71,76,127,13,43,212,0,64,37,135,245,130,133,77,203,222,122,235,45,155,150,
81,114,172,80,83,18,38,39,39,35,159,207,71,83,83,83,76,77,77,9,233,101,176,66,13,0,84,122,84,23,175,86,127,242,201,39,241,250,235,175,199,232,232,104,236,216,177,35,30,126,248,97,131,66,80,67,119,119,
119,36,73,18,71,142,28,185,233,147,41,139,89,161,6,0,170,45,172,231,230,230,162,171,171,43,38,38,38,162,182,182,54,106,107,107,13,9,65,77,117,178,233,152,160,6,0,88,110,88,23,175,86,127,241,197,23,241,
135,63,252,33,134,135,135,163,182,182,54,30,125,244,81,67,66,80,83,61,26,27,27,35,73,146,56,125,250,180,144,190,77,222,242,13,0,84,91,84,71,92,255,54,240,137,137,137,56,124,248,112,236,218,181,43,182,
110,221,26,15,62,248,160,65,177,166,108,74,198,154,26,25,25,137,215,94,123,45,222,126,251,109,49,125,135,172,80,3,0,213,28,214,11,46,95,190,28,73,146,196,91,111,189,21,251,247,239,55,32,214,148,21,106,
214,132,77,199,4,53,0,192,74,71,117,241,106,245,209,163,71,227,55,191,249,77,228,243,121,155,150,33,168,169,28,61,61,61,145,36,73,28,62,124,248,166,79,134,8,106,0,128,59,13,235,133,77,203,10,133,66,212,
214,214,70,77,77,141,33,33,168,41,79,19,19,19,209,219,219,27,237,237,237,54,29,19,212,0,0,171,26,214,197,171,213,103,206,156,137,223,255,254,247,145,207,231,227,133,23,94,176,105,25,130,154,242,210,212,
212,20,73,146,196,169,83,167,132,180,160,6,0,88,147,168,142,184,254,109,224,249,124,62,14,28,56,96,211,50,86,141,77,201,88,81,11,155,142,189,245,214,91,98,90,80,3,0,172,91,88,47,88,216,180,236,237,183,
223,142,174,174,46,3,98,69,89,161,102,197,236,221,187,55,50,153,76,92,186,116,73,72,11,106,0,128,117,143,234,226,213,234,143,63,254,56,94,127,253,245,24,27,27,139,167,158,122,202,166,101,8,106,74,67,79,
79,79,164,211,233,56,116,232,208,77,159,204,0,0,96,189,195,250,234,213,171,145,205,102,99,108,108,44,234,234,234,98,247,238,221,134,132,160,102,125,44,108,58,214,209,209,17,87,175,94,21,210,107,204,10,
53,0,192,242,195,250,198,77,203,222,121,231,157,200,229,114,54,45,67,80,179,246,154,155,155,35,73,146,56,121,242,228,146,79,88,38,36,168,1,0,74,45,170,35,22,111,90,118,232,208,161,120,238,185,231,226,
229,151,95,54,36,4,53,171,43,151,203,69,71,71,71,244,247,247,47,10,58,33,45,168,1,0,202,33,172,139,163,250,226,197,139,177,111,223,190,24,31,31,143,218,218,218,120,252,241,199,13,9,65,205,202,171,175,
175,143,36,73,226,194,133,11,66,90,80,3,0,148,117,84,71,92,191,90,125,240,224,193,248,223,255,253,223,24,25,25,177,105,25,203,230,216,44,190,86,111,111,111,252,226,23,191,136,247,222,123,79,76,11,106,
0,128,138,11,235,5,11,155,150,189,241,198,27,209,210,210,98,64,124,45,43,212,220,212,196,196,68,12,12,12,68,91,91,91,204,204,204,8,105,65,13,0,80,177,81,93,188,90,253,249,231,159,199,239,126,247,187,200,
229,114,241,252,243,207,199,150,45,91,12,10,65,205,242,181,183,183,71,42,149,138,19,39,78,220,244,73,7,65,13,0,80,169,97,61,63,63,31,67,67,67,241,225,135,31,218,180,12,65,205,242,216,116,76,80,3,0,84,
123,88,47,181,105,217,196,196,68,212,212,212,216,180,12,65,205,210,234,235,235,35,157,78,199,249,243,231,133,180,160,6,0,168,234,168,142,184,254,109,224,7,14,28,136,35,71,142,196,248,248,120,108,223,190,
61,54,111,222,108,80,216,148,140,136,129,129,129,248,229,47,127,25,239,189,247,158,152,22,212,0,0,220,228,103,225,153,153,153,104,105,105,137,223,254,246,183,209,218,218,106,64,88,161,174,102,54,29,19,
212,0,0,44,47,170,139,87,171,63,251,236,179,120,243,205,55,99,100,100,196,166,101,130,154,106,212,209,209,17,169,84,42,62,253,244,211,155,62,105,32,168,1,0,88,58,172,23,54,45,59,112,224,64,236,222,189,
59,94,122,233,37,67,18,212,84,186,209,209,209,232,236,236,140,238,238,110,155,142,9,106,0,0,110,35,172,139,87,171,47,92,184,16,127,250,211,159,34,151,203,69,109,109,109,108,219,182,205,144,4,53,149,168,
190,190,62,50,153,76,156,59,119,78,72,11,106,0,0,238,32,170,35,150,222,180,108,108,108,204,166,101,85,196,166,100,85,96,96,96,32,126,245,171,95,197,123,239,189,39,166,5,53,0,0,43,28,214,11,22,54,45,123,
227,141,55,162,173,173,205,128,170,128,21,234,10,86,40,20,98,120,120,56,154,155,155,99,122,122,90,72,11,106,0,0,86,41,170,139,87,171,79,156,56,17,111,190,249,102,140,142,142,198,206,157,59,227,145,71,
30,49,40,65,77,57,177,233,152,160,6,0,96,253,194,122,110,110,46,250,250,250,226,195,15,63,140,218,218,218,168,171,171,51,36,65,77,169,27,27,27,139,174,174,174,216,191,127,127,204,205,205,9,105,0,0,88,
227,176,46,94,173,62,123,246,108,252,241,143,127,140,129,129,1,155,150,9,106,74,89,42,149,138,76,38,19,103,207,158,21,210,85,192,10,53,0,64,233,70,117,196,226,77,203,142,30,61,26,133,66,33,182,109,219,
102,211,178,10,97,83,178,10,48,56,56,24,175,190,250,106,188,251,238,187,98,90,80,3,0,80,98,97,189,224,202,149,43,209,216,216,24,111,189,245,86,116,118,118,26,80,5,176,66,93,198,38,39,39,35,159,207,71,
83,83,83,76,77,77,9,105,65,13,0,64,137,70,117,241,106,245,39,159,124,18,175,191,254,122,12,15,15,219,180,76,80,179,30,186,187,187,35,73,146,56,114,228,200,77,31,180,8,106,0,0,74,51,172,23,54,45,59,112,
224,64,212,214,214,70,109,109,173,33,9,106,86,219,248,248,120,236,223,191,63,186,186,186,98,118,118,86,72,87,177,27,55,157,3,0,160,60,194,186,120,181,250,139,47,190,136,63,252,225,15,49,60,60,28,53,53,
53,177,117,235,86,67,18,212,172,134,166,166,166,72,165,82,113,250,244,105,33,13,0,0,101,28,213,17,215,191,13,124,98,98,34,14,29,58,20,59,118,236,176,105,89,25,177,41,89,25,24,25,25,137,215,94,123,45,222,
122,235,45,49,205,53,222,242,13,0,80,25,97,189,96,97,211,178,183,223,126,59,246,239,223,111,64,101,192,10,117,137,219,187,119,111,100,50,153,184,116,233,146,144,70,80,3,0,84,104,84,23,175,86,31,61,122,
52,126,243,155,223,68,62,159,143,29,59,118,196,195,15,63,108,80,130,154,91,209,211,211,19,233,116,58,14,29,58,116,211,7,29,130,26,0,128,202,12,235,185,185,185,232,234,234,138,66,161,16,181,181,181,81,
83,83,99,72,130,154,175,51,49,49,17,3,3,3,209,214,214,22,51,51,51,66,26,65,13,0,80,101,97,93,188,90,125,230,204,153,248,253,239,127,31,249,124,62,94,120,225,133,120,244,209,71,13,73,80,179,148,150,150,
150,72,165,82,113,242,228,201,37,31,88,38,132,160,6,0,168,142,168,142,184,254,109,224,249,124,62,14,28,56,16,187,118,237,138,173,91,183,198,131,15,62,104,80,130,154,136,136,92,46,23,29,29,29,209,223,223,
191,40,146,132,52,130,26,0,160,122,195,186,56,170,47,95,190,28,73,146,196,216,216,88,212,213,213,197,147,79,62,105,72,130,186,186,213,215,215,71,58,157,142,243,231,207,11,105,4,53,0,0,75,118,65,113,88,
127,252,241,199,241,250,235,175,199,216,216,88,60,245,212,83,54,45,19,212,213,103,96,96,32,26,26,26,162,80,40,220,244,65,3,130,26,0,128,165,194,250,234,213,171,145,205,102,175,173,86,239,222,189,219,144,
4,117,229,43,20,10,49,60,60,28,205,205,205,49,61,61,45,164,17,212,0,0,220,82,88,223,184,105,217,59,239,188,19,185,92,206,166,101,130,186,178,117,116,116,68,146,36,113,252,248,241,37,31,24,38,132,160,6,
0,96,57,81,29,177,120,211,178,67,135,14,197,115,207,61,23,47,191,252,178,33,9,234,202,49,54,54,22,217,108,54,186,187,187,109,58,134,160,6,0,96,197,194,186,56,170,47,94,188,24,251,246,237,139,92,46,23,
117,117,117,241,196,19,79,24,146,160,46,111,169,84,42,210,233,116,156,59,119,78,72,3,0,0,43,30,213,17,215,175,86,31,57,114,36,126,243,155,223,196,232,232,168,77,203,86,217,6,35,88,29,131,131,131,241,234,
171,175,198,187,239,190,43,166,89,21,86,168,1,0,184,89,99,44,108,90,246,198,27,111,68,75,75,139,1,173,18,43,212,43,108,114,114,50,242,249,124,52,54,54,198,229,203,151,133,52,171,102,110,110,206,16,0,0,
88,212,27,197,171,213,159,127,254,121,188,249,230,155,49,56,56,24,63,253,233,79,227,159,255,249,159,13,74,80,151,166,206,206,206,72,165,82,113,236,216,177,155,222,185,97,165,88,161,6,0,96,185,97,109,211,
50,65,93,178,198,198,198,162,171,171,43,186,186,186,98,118,118,86,72,35,168,1,0,40,137,176,94,106,211,178,241,241,241,168,173,173,141,199,31,127,220,144,4,245,250,106,108,108,140,36,73,226,244,233,211,
66,26,65,13,0,64,201,69,117,196,245,171,213,7,15,30,140,95,255,250,215,49,62,62,30,63,249,201,79,226,161,135,30,50,168,219,100,83,178,219,52,50,50,18,175,189,246,90,188,253,246,219,98,26,0,0,40,139,176,
94,48,51,51,19,45,45,45,241,198,27,111,68,107,107,171,1,221,38,43,212,183,104,114,114,50,114,185,92,52,55,55,199,212,212,148,144,102,221,204,204,204,68,123,123,187,65,192,77,156,58,117,202,16,74,196,233,
211,167,61,95,193,87,184,241,87,6,97,181,163,186,120,181,250,179,207,62,139,55,223,124,51,198,198,198,226,217,103,159,141,45,91,182,24,148,160,94,29,61,61,61,145,78,167,227,208,161,67,55,189,115,194,90,
185,112,225,66,252,246,183,191,53,8,160,228,237,219,183,207,16,0,74,56,172,231,231,231,163,175,175,47,38,38,38,98,247,238,221,241,210,75,47,25,210,50,221,229,247,48,191,222,196,196,68,244,246,246,70,71,
71,71,92,189,122,85,72,179,238,138,95,85,4,150,247,67,3,158,175,192,243,21,44,239,121,250,129,7,30,136,154,154,26,155,150,9,234,59,215,220,220,28,73,146,196,201,147,39,61,225,1,0,0,85,17,214,27,55,110,
140,103,158,121,38,182,111,223,30,155,55,111,54,32,65,125,107,114,185,92,116,116,116,68,127,127,255,162,221,148,197,52,0,0,80,233,81,29,17,241,253,239,127,63,234,234,234,98,231,206,157,6,36,168,151,167,
190,190,62,210,233,116,156,63,127,94,72,3,0,0,85,29,214,27,54,108,136,199,30,123,44,118,238,220,25,143,60,242,136,1,9,234,165,13,12,12,68,38,147,137,201,201,201,69,215,137,105,0,0,160,154,195,122,211,
166,77,81,83,83,19,47,190,248,162,225,8,234,191,154,152,152,136,129,129,129,104,107,107,139,153,153,25,33,13,0,0,16,55,223,180,172,182,182,54,182,109,219,38,168,171,61,168,59,58,58,34,149,74,197,167,159,
126,186,232,58,49,13,0,0,176,56,172,239,185,231,158,216,177,99,71,108,219,182,173,170,55,45,171,218,160,30,27,27,139,108,54,27,221,221,221,54,29,3,0,0,184,197,168,142,248,114,211,178,61,123,246,196,179,
207,62,43,168,171,69,125,125,125,100,50,153,56,119,238,156,144,6,0,0,184,131,176,174,230,77,203,170,42,168,7,7,7,163,165,165,37,242,249,252,162,235,196,52,0,0,192,237,135,245,125,247,221,23,181,181,181,
81,91,91,43,168,43,201,228,228,100,12,13,13,69,115,115,115,76,79,79,11,105,0,0,128,85,136,234,136,136,135,30,122,40,106,106,106,98,235,214,173,130,186,220,117,118,118,70,42,149,138,99,199,142,45,186,78,
76,3,0,0,172,124,88,87,203,166,101,21,27,212,99,99,99,209,213,213,21,93,93,93,49,59,59,43,164,1,0,0,214,48,170,35,34,126,248,195,31,70,109,109,109,60,253,244,211,130,186,92,52,54,54,70,146,36,113,250,
244,105,33,13,0,0,176,142,97,189,97,195,134,216,190,125,123,236,216,177,35,30,126,248,97,65,93,170,70,70,70,162,181,181,53,134,134,134,22,93,39,166,1,0,0,214,47,172,43,113,211,178,138,8,234,201,201,201,
200,231,243,209,212,212,20,83,83,83,66,26,0,0,160,4,163,58,226,203,77,203,106,107,107,227,209,71,31,21,212,235,173,167,167,39,146,36,137,195,135,15,47,186,78,76,3,0,0,148,94,88,223,123,239,189,177,107,
215,174,216,186,117,107,60,248,224,131,130,122,173,77,76,76,68,111,111,111,116,116,116,196,213,171,87,133,52,0,0,64,25,69,117,196,151,155,150,237,217,179,39,158,124,242,73,65,189,86,154,155,155,35,149,
74,197,169,83,167,132,52,0,0,64,25,135,245,221,119,223,29,79,60,241,68,60,245,212,83,101,183,105,89,89,5,117,46,151,139,142,142,142,232,239,239,143,27,191,110,49,13,0,0,80,190,97,253,237,111,127,59,234,
234,234,98,247,238,221,130,122,165,213,215,215,71,146,36,113,225,194,5,33,13,0,0,80,129,81,29,17,177,101,203,150,120,225,133,23,202,98,211,178,146,15,234,222,222,222,72,167,211,241,209,71,31,45,186,78,
76,3,0,0,84,94,88,127,235,91,223,138,231,158,123,46,94,126,249,101,65,125,59,38,38,38,98,96,96,32,218,218,218,98,102,102,70,72,3,0,0,84,81,84,71,68,252,248,199,63,142,218,218,218,120,252,241,199,5,245,
114,117,116,116,68,42,149,138,79,63,253,116,209,117,98,26,0,0,160,122,194,186,148,55,45,43,169,160,30,29,29,141,206,206,206,232,238,238,182,233,24,0,0,128,168,190,230,254,251,239,143,61,123,246,196,174,
93,187,4,245,141,254,242,151,191,68,38,147,137,243,231,207,11,105,0,0,0,22,133,245,93,119,221,21,219,182,109,139,103,159,125,54,182,108,217,34,168,7,7,7,163,161,161,33,38,38,38,22,93,39,166,1,0,0,184,
49,172,255,246,111,255,54,106,106,106,226,103,63,251,89,117,6,245,228,228,100,228,243,249,104,106,106,138,169,169,41,33,13,0,0,192,178,163,58,34,226,129,7,30,136,218,218,218,216,182,109,91,245,4,117,79,
79,79,36,73,18,135,15,31,94,116,157,152,6,0,0,96,185,97,125,207,61,247,196,142,29,59,98,219,182,109,177,121,243,230,202,13,106,71,97,1,0,0,176,210,81,29,17,241,195,31,254,48,234,234,234,226,169,167,158,
170,188,160,238,232,232,136,250,250,250,56,113,226,132,144,6,0,0,96,197,195,122,173,143,216,90,245,160,30,27,27,139,108,54,235,40,44,0,0,0,214,36,172,239,191,255,254,168,171,171,139,231,158,123,174,124,
131,58,73,146,72,146,36,206,157,59,39,164,1,0,0,88,179,168,190,235,174,187,226,209,71,31,141,231,159,127,126,213,142,216,90,149,160,30,25,25,137,214,214,214,24,26,26,90,116,157,152,6,0,0,96,173,194,122,
211,166,77,81,83,83,19,47,190,248,98,233,7,245,222,189,123,35,147,201,196,165,75,151,132,52,0,0,0,235,30,213,17,17,15,61,244,80,212,212,212,196,214,173,91,75,47,168,123,123,123,35,73,146,56,116,232,208,
162,235,196,52,0,0,0,235,29,214,247,222,123,111,236,218,181,43,182,110,221,26,15,62,248,224,250,7,181,163,176,0,0,0,40,151,168,142,136,248,209,143,126,20,123,246,236,137,39,158,120,98,253,130,250,102,
71,97,137,105,0,0,0,74,57,172,55,110,220,24,207,62,251,108,60,254,248,227,177,121,243,230,181,11,234,177,177,177,232,234,234,138,174,174,174,152,157,157,21,210,0,0,0,148,101,88,127,239,123,223,139,159,
253,236,103,177,99,199,142,213,15,234,166,166,166,72,165,82,113,250,244,105,33,13,0,0,64,217,71,245,134,13,27,98,251,246,237,177,99,199,142,120,248,225,135,87,62,168,115,185,92,180,180,180,56,10,11,0,
0,128,138,12,235,251,239,191,63,254,229,95,254,37,118,238,220,185,114,65,253,254,251,239,71,67,67,131,163,176,0,0,0,168,232,168,222,176,97,67,60,249,228,147,241,244,211,79,127,237,106,245,87,6,245,224,
224,96,100,50,153,40,20,10,139,174,19,211,0,0,0,84,106,88,127,247,187,223,141,127,249,151,127,137,93,187,118,221,122,80,239,221,187,55,210,233,116,76,77,77,9,105,0,0,0,170,46,170,239,186,235,174,120,244,
209,71,227,133,23,94,136,71,30,121,228,235,131,186,175,175,47,82,169,84,28,58,116,104,209,95,22,211,0,0,0,84,91,88,223,119,223,125,177,103,207,158,216,189,123,247,210,65,61,57,57,25,67,67,67,209,220,220,
28,211,211,211,66,26,0,0,0,81,93,100,235,214,173,241,211,159,254,52,254,241,31,255,241,175,65,221,215,215,23,251,246,237,139,143,63,254,120,209,7,17,211,0,0,0,8,235,47,125,247,187,223,141,127,253,215,
127,141,103,158,121,38,238,218,187,119,111,164,82,169,184,124,249,178,144,6,0,0,128,175,137,234,111,124,227,27,81,91,91,27,27,102,102,102,98,110,110,206,132,0,0,0,96,25,54,108,216,16,211,211,211,95,190,
229,187,167,167,39,62,248,224,131,56,118,236,216,162,191,104,165,26,0,0,128,106,117,227,234,244,15,126,240,131,120,241,197,23,99,251,246,237,127,221,148,172,80,40,196,240,240,112,52,53,53,197,204,204,
140,168,6,0,0,64,72,255,127,27,55,110,140,103,158,121,38,182,111,223,30,155,55,111,142,8,199,102,1,0,0,192,77,67,58,34,226,71,63,250,81,236,217,179,39,158,120,226,137,235,254,124,81,80,47,216,187,119,
111,164,211,233,152,154,154,18,213,0,0,0,84,93,76,223,115,207,61,241,220,115,207,197,214,173,91,227,193,7,31,92,244,247,111,26,212,17,113,237,92,234,124,62,191,232,58,97,13,0,0,64,37,134,116,68,196,3,
15,60,16,117,117,117,241,216,99,143,221,244,255,251,202,160,94,176,111,223,190,200,100,50,113,225,194,5,81,13,0,0,64,197,198,244,166,77,155,162,166,166,38,94,124,241,197,175,253,127,151,21,212,17,17,163,
163,163,209,217,217,25,221,221,221,113,227,255,35,172,1,0,0,40,231,144,142,136,216,186,117,107,60,255,252,243,177,101,203,150,101,125,140,101,7,245,130,246,246,246,72,165,82,113,226,196,137,69,215,9,107,
0,0,0,202,45,166,239,191,255,254,168,171,171,139,231,158,123,238,150,62,206,45,7,117,68,196,196,196,68,12,12,12,68,91,91,155,35,182,0,0,0,40,203,144,190,251,238,187,227,137,39,158,136,167,158,122,42,30,
126,248,225,91,254,120,183,21,212,11,122,123,123,35,157,78,199,71,31,125,180,232,58,97,13,0,0,64,41,134,116,196,151,71,97,213,213,213,197,147,79,62,121,219,31,247,142,130,122,193,222,189,123,35,147,201,
196,165,75,151,68,53,0,0,0,37,27,211,247,222,123,111,236,218,181,235,166,71,97,173,121,80,71,68,140,140,140,68,107,107,107,12,13,13,45,186,78,88,3,0,0,176,158,33,29,17,177,101,203,150,120,225,133,23,226,
209,71,31,93,145,207,177,98,65,189,160,169,169,41,146,36,137,83,167,78,137,106,0,0,0,214,61,166,239,187,239,190,168,171,171,139,154,154,154,21,253,60,43,30,212,17,95,110,90,214,219,219,27,237,237,237,
49,59,59,43,172,1,0,0,88,243,144,222,176,97,67,108,223,190,61,118,236,216,113,91,155,142,173,75,80,47,232,238,238,142,36,73,226,200,145,35,139,174,19,214,0,0,0,172,70,72,71,68,252,224,7,63,136,186,186,
186,120,250,233,167,87,237,243,174,106,80,71,68,76,78,78,70,62,159,143,166,166,166,152,154,154,18,213,0,0,0,172,90,76,223,115,207,61,177,99,199,142,216,182,109,91,108,222,188,121,85,63,247,170,7,245,130,
193,193,193,104,105,105,137,124,62,191,232,58,97,13,0,0,192,157,132,116,68,196,3,15,60,16,181,181,181,177,109,219,182,53,249,26,214,44,168,23,164,82,169,72,167,211,113,238,220,57,81,13,0,0,192,29,199,
244,166,77,155,162,166,166,38,94,124,241,197,53,253,58,214,60,168,35,34,198,198,198,34,155,205,70,119,119,119,220,248,249,133,53,0,0,0,203,9,233,13,27,54,196,99,143,61,22,207,62,251,108,108,217,178,101,
205,191,158,117,9,234,5,29,29,29,145,74,165,226,211,79,63,93,116,157,176,6,0,0,96,169,144,142,136,248,222,247,190,23,123,246,236,137,157,59,119,174,219,215,181,174,65,29,17,81,40,20,98,120,120,56,154,
155,155,99,122,122,90,84,3,0,0,112,211,152,222,184,113,99,60,243,204,51,177,125,251,246,85,223,116,172,228,131,122,65,95,95,95,164,211,233,56,120,240,224,162,235,132,53,0,0,64,117,135,116,196,151,155,
142,213,212,212,196,227,143,63,94,18,95,99,201,4,245,130,250,250,250,72,167,211,113,254,252,121,81,13,0,0,32,166,227,91,223,250,86,60,247,220,115,241,242,203,47,151,212,215,89,114,65,29,17,49,58,58,26,
157,157,157,54,45,3,0,0,168,226,144,190,235,174,187,226,209,71,31,141,231,159,127,126,93,54,29,43,203,160,94,96,211,50,0,0,128,234,11,233,136,47,55,29,171,171,171,139,93,187,118,149,236,215,93,210,65,
29,97,211,50,0,0,128,106,138,233,82,218,116,172,236,131,122,193,192,192,64,52,54,54,198,196,196,196,162,235,132,53,0,0,64,121,135,116,68,233,109,58,86,49,65,189,160,190,190,62,50,153,76,156,59,119,78,
84,3,0,0,84,64,76,111,218,180,41,118,239,222,29,47,189,244,82,89,125,31,101,23,212,17,17,99,99,99,145,205,102,109,90,6,0,0,80,198,33,189,97,195,134,120,236,177,199,226,217,103,159,45,201,77,199,42,50,
168,23,100,179,217,72,165,82,113,252,248,241,69,215,9,107,0,0,128,210,12,233,136,136,239,127,255,251,81,87,87,23,59,119,238,44,219,239,171,172,131,58,194,166,101,0,0,0,229,20,211,247,220,115,79,236,216,
177,35,182,109,219,86,242,155,142,85,124,80,47,24,28,28,140,150,150,150,200,231,243,139,174,19,214,0,0,0,235,27,210,17,95,110,58,86,91,91,27,219,182,109,171,136,239,177,98,130,122,65,42,149,138,76,38,
19,103,207,158,21,213,0,0,0,37,16,211,155,54,109,138,154,154,154,120,241,197,23,43,234,251,172,184,160,142,248,114,211,178,174,174,174,232,234,234,138,217,217,89,97,13,0,0,176,14,33,189,176,233,216,206,
157,59,227,145,71,30,169,184,239,183,34,131,122,65,54,155,141,36,73,226,216,177,99,139,174,19,214,0,0,0,171,19,210,17,95,110,58,182,103,207,158,120,246,217,103,43,246,251,174,232,160,142,136,152,156,156,
140,124,62,31,77,77,77,49,53,53,37,170,1,0,0,86,49,166,43,105,211,177,170,15,234,5,54,45,3,0,0,88,189,144,142,248,114,211,177,186,186,186,120,236,177,199,170,98,6,85,19,212,11,26,26,26,34,157,78,199,233,
211,167,69,53,0,0,192,10,196,244,125,247,221,23,53,53,53,81,87,87,87,85,115,168,186,160,142,176,105,25,0,0,192,74,132,116,165,111,58,38,168,191,66,119,119,119,36,73,18,71,142,28,89,116,157,176,6,0,0,88,
58,164,35,34,126,248,195,31,70,109,109,109,60,253,244,211,85,59,151,170,14,234,8,155,150,1,0,0,220,74,76,87,211,166,99,130,122,153,70,70,70,162,181,181,53,134,134,134,22,93,39,172,1,0,128,106,15,233,136,
136,135,30,122,40,106,106,106,98,235,214,173,6,36,168,23,107,108,108,140,36,73,108,90,6,0,0,136,233,255,239,190,251,238,139,218,218,218,168,173,173,53,28,65,253,213,198,199,199,99,255,254,253,54,45,3,
0,0,170,58,164,55,108,216,16,219,183,111,143,29,59,118,196,195,15,63,108,64,130,122,249,186,187,187,35,157,78,199,225,195,135,23,93,39,172,1,0,128,74,13,233,136,47,55,29,171,171,171,139,167,158,122,202,
128,4,245,237,153,156,156,140,92,46,23,205,205,205,54,45,3,0,0,42,62,166,255,230,111,254,38,158,127,254,249,216,178,101,75,60,248,224,131,6,36,168,239,220,200,200,72,100,179,217,232,239,239,143,27,103,
38,172,89,239,39,61,224,230,60,71,123,190,2,207,87,176,252,231,232,45,91,182,196,11,47,188,16,143,62,250,168,1,9,234,149,215,220,220,28,73,146,196,201,147,39,61,9,226,135,83,240,67,42,158,175,192,243,
21,21,241,252,252,237,111,127,59,234,234,234,98,247,238,221,6,116,11,238,54,130,91,243,252,243,207,199,63,252,195,63,68,79,79,79,116,118,118,198,204,204,204,117,119,76,79,130,172,149,77,155,54,197,191,
255,251,191,27,4,220,196,201,147,39,99,223,190,125,6,81,2,94,122,233,165,248,206,119,190,99,16,112,19,239,190,251,110,92,188,120,209,32,88,151,152,222,184,113,99,60,243,204,51,241,147,159,252,36,30,122,
232,33,3,18,212,171,239,161,135,30,138,135,30,122,40,30,126,248,225,200,100,50,113,240,224,193,69,119,80,97,205,106,219,184,113,99,60,251,236,179,6,1,55,209,218,218,106,8,37,226,59,223,249,142,231,43,
248,10,127,250,211,159,12,129,53,15,233,136,136,31,255,248,199,81,91,91,27,143,63,254,184,1,221,166,13,70,112,251,126,242,147,159,196,127,254,231,127,198,191,255,191,246,238,165,55,206,243,60,252,255,
197,145,134,164,72,89,38,19,203,148,45,89,166,69,113,200,225,97,120,24,30,196,225,113,134,51,15,41,70,145,21,45,2,31,32,167,121,5,93,36,133,211,188,133,190,141,46,178,234,162,232,34,37,109,199,74,162,
36,202,47,53,154,22,90,56,139,198,1,26,167,64,18,3,69,90,160,139,40,250,47,12,235,63,20,199,214,137,135,57,124,62,128,129,48,62,73,215,28,238,249,62,247,227,185,175,95,143,83,167,78,61,244,9,11,0,0,15,
106,107,243,177,145,195,141,233,147,39,79,198,214,214,86,124,231,59,223,17,211,79,201,14,245,62,216,220,220,140,243,231,207,199,79,127,250,211,248,249,207,127,126,255,75,203,236,86,99,241,5,0,172,233,
212,75,72,167,82,169,152,154,154,138,165,165,165,24,29,29,53,32,65,93,63,70,70,70,98,100,100,36,134,135,135,99,103,103,39,62,254,248,227,61,79,100,97,141,197,23,0,176,166,115,216,33,29,17,209,215,215,
23,73,146,196,242,242,178,1,237,35,183,124,239,179,197,197,197,120,227,141,55,98,107,107,43,58,59,59,31,250,196,134,39,126,241,166,188,124,1,64,80,195,23,199,116,71,71,71,148,74,165,184,113,227,134,152,
62,0,118,168,15,64,38,147,137,76,38,19,253,253,253,113,243,230,205,184,115,231,206,158,39,184,221,106,44,190,0,128,53,157,131,10,233,136,136,129,129,129,168,84,42,49,61,61,109,64,130,186,241,76,78,78,
198,228,228,100,188,251,238,187,177,179,179,19,159,124,242,137,176,198,226,11,0,88,211,57,208,152,238,233,233,137,74,165,18,149,74,197,112,4,117,227,91,95,95,143,23,95,124,49,110,223,190,29,183,111,223,
142,187,119,239,238,122,242,139,106,0,0,65,13,79,27,210,169,84,42,102,103,103,99,113,113,49,134,135,135,13,72,80,55,143,108,54,27,217,108,54,70,70,70,226,157,119,222,137,143,62,250,104,207,11,65,88,99,
241,5,0,107,58,60,110,72,71,68,156,59,119,46,146,36,137,75,151,46,25,208,33,242,173,70,135,108,110,110,46,190,251,221,239,198,213,171,87,163,171,171,235,161,47,12,176,248,2,128,53,29,62,175,25,58,59,59,
35,73,146,120,253,245,215,197,244,17,176,67,125,68,174,92,185,18,47,191,252,114,252,228,39,63,137,15,62,248,192,217,213,88,124,1,192,154,14,143,28,210,17,17,163,163,163,81,44,22,35,151,203,25,144,160,
110,61,227,227,227,49,62,62,30,239,191,255,126,108,111,111,199,31,254,240,135,61,47,24,97,141,197,23,0,172,233,8,233,106,167,79,159,142,36,73,98,117,117,213,128,142,152,91,190,235,192,218,218,90,124,227,
27,223,136,82,169,20,233,116,250,161,47,32,176,248,2,128,53,157,214,139,233,116,58,29,133,66,33,110,220,184,33,166,235,132,29,234,58,49,52,52,116,255,143,119,223,125,55,126,245,171,95,237,121,33,217,173,
198,226,11,0,214,116,90,47,164,35,62,61,83,186,92,46,71,62,159,55,160,58,98,135,186,206,76,77,77,197,183,191,253,237,184,126,253,122,156,58,117,234,161,47,44,44,190,0,128,53,157,230,141,233,147,39,79,
198,214,214,86,188,253,246,219,98,186,14,217,161,174,83,155,155,155,247,191,180,236,255,253,191,255,23,127,249,203,95,118,189,192,236,86,99,241,5,0,107,58,205,27,210,169,84,42,166,166,166,98,105,105,41,
70,71,71,13,72,80,243,184,62,59,187,58,155,205,198,246,246,118,124,252,241,199,123,94,112,194,218,226,11,0,88,211,105,158,144,142,136,232,235,235,139,36,73,98,121,121,217,128,234,156,91,190,27,64,161,
80,136,55,223,124,51,182,182,182,226,196,137,19,15,125,1,98,241,5,0,172,233,52,94,76,119,116,116,68,169,84,138,27,55,110,136,233,6,97,135,186,65,12,14,14,198,224,224,96,244,247,247,199,205,155,55,227,
206,157,59,123,94,136,118,171,45,190,0,128,53,157,198,11,233,136,79,191,116,172,82,169,196,244,244,180,1,9,106,14,202,228,228,100,76,78,78,198,123,239,189,23,219,219,219,241,201,39,159,8,107,139,47,0,
96,77,167,65,99,186,167,167,39,42,149,74,84,42,21,195,17,212,28,150,82,169,20,47,190,248,98,252,203,191,252,75,252,248,199,63,142,187,119,239,238,122,145,138,106,139,47,0,0,245,27,210,169,84,42,102,103,
103,99,113,113,49,134,135,135,13,72,80,115,216,134,135,135,99,120,120,56,50,153,76,236,236,236,196,71,31,125,180,231,5,43,172,5,53,0,96,77,167,126,66,58,34,226,165,151,94,138,74,165,18,151,46,93,50,160,
6,231,75,201,154,192,236,236,108,124,247,187,223,141,171,87,175,70,87,87,215,67,95,192,88,124,1,0,107,58,135,31,211,157,157,157,145,36,73,188,246,218,107,98,186,73,216,161,110,34,87,174,92,137,254,254,
254,184,117,235,86,124,240,193,7,113,239,222,189,93,47,100,187,213,22,95,0,160,254,164,82,246,184,154,61,164,35,34,70,71,71,163,88,44,70,46,151,51,32,65,77,189,26,27,27,139,177,177,177,184,121,243,102,
108,111,111,199,239,127,255,251,61,47,108,97,45,168,1,0,107,58,135,19,210,167,79,159,142,36,73,98,117,117,213,128,154,144,203,97,77,106,117,117,53,222,122,235,173,40,149,74,209,222,222,254,208,23,58,22,
95,0,192,154,206,254,197,116,58,157,142,66,161,16,55,110,220,16,211,77,204,14,117,19,27,26,26,186,255,199,123,239,189,23,31,126,248,225,158,23,188,221,106,139,47,0,0,251,23,210,17,159,158,41,93,46,151,
35,159,207,27,80,147,179,67,221,2,166,166,166,226,91,223,250,86,92,191,126,61,78,157,58,245,208,55,0,4,53,0,96,77,231,241,99,250,228,201,147,113,245,234,213,120,251,237,183,197,116,139,176,67,221,66,54,
55,55,227,229,151,95,142,219,183,111,199,237,219,183,239,159,93,109,183,218,226,11,0,88,211,121,242,144,78,165,82,49,53,53,21,75,75,75,49,58,58,106,64,130,154,102,149,205,102,35,155,205,70,38,147,137,
237,237,237,248,248,227,143,247,188,49,8,107,139,47,0,96,77,231,225,33,29,17,209,215,215,23,73,146,196,242,242,178,1,181,32,183,124,183,168,66,161,16,111,190,249,102,108,109,109,197,137,19,39,30,250,70,
1,0,128,160,22,211,255,191,142,142,142,40,149,74,113,227,198,13,49,221,194,236,80,183,176,193,193,193,24,28,28,140,87,94,121,37,222,127,255,253,184,115,231,206,158,55,12,187,213,22,95,0,192,154,46,164,
119,27,26,26,138,114,185,28,19,19,19,6,36,168,105,117,19,19,19,49,49,49,17,239,189,247,94,236,236,236,196,31,255,248,71,97,221,0,82,41,55,152,0,128,160,230,48,99,186,183,183,55,202,229,114,84,42,21,195,
65,80,179,91,169,84,138,179,103,207,198,47,126,241,139,184,117,235,86,252,249,207,127,222,245,102,34,170,45,190,0,128,53,189,21,67,58,149,74,197,236,236,108,44,46,46,198,240,240,176,1,33,168,169,237,179,
115,171,51,153,76,188,243,206,59,241,235,95,255,122,207,27,139,176,182,248,2,0,214,244,86,8,233,136,136,151,94,122,41,146,36,137,249,249,121,3,98,15,247,140,82,211,236,236,108,252,237,223,254,109,92,191,
126,61,78,158,60,249,208,55,26,44,190,0,128,53,189,153,98,186,187,187,59,182,182,182,226,181,215,94,19,211,124,46,59,212,124,161,205,205,205,56,119,238,92,220,186,117,43,62,248,224,131,184,119,239,222,
174,55,28,187,213,0,0,130,186,153,66,58,34,98,116,116,52,138,197,98,228,114,57,3,66,80,243,116,198,198,198,98,108,108,44,126,252,227,31,199,246,246,118,252,215,127,253,215,158,55,32,97,109,241,5,0,172,
233,141,30,210,207,63,255,124,84,42,149,88,93,93,53,32,30,137,91,190,121,100,75,75,75,241,230,155,111,70,169,84,138,246,246,246,135,190,33,97,241,5,0,172,233,141,16,211,233,116,58,10,133,66,220,184,113,
67,76,243,88,236,80,243,88,62,251,210,178,161,161,161,248,225,15,127,232,236,106,139,47,0,96,77,111,216,144,142,136,24,24,24,136,114,185,28,249,124,222,128,120,108,118,168,121,34,83,83,83,241,215,127,
253,215,113,253,250,245,120,246,217,103,31,250,70,133,197,23,0,176,166,215,83,76,159,60,121,50,94,125,245,213,120,251,237,183,197,52,79,204,14,53,79,101,115,115,51,94,126,249,229,184,125,251,118,220,190,
125,59,238,222,189,187,235,13,203,110,245,193,73,165,92,15,3,0,65,205,227,134,116,42,149,138,169,169,169,88,94,94,142,145,145,17,3,66,80,115,180,178,217,108,100,179,217,24,26,26,138,237,237,237,248,237,
111,127,187,231,13,76,88,3,0,8,234,163,12,233,136,136,179,103,207,70,146,36,177,176,176,96,64,236,11,91,92,236,155,133,133,133,120,227,141,55,98,107,107,43,78,156,56,241,208,55,52,44,190,0,128,53,253,
48,98,186,163,163,35,74,165,82,188,254,250,235,98,154,125,101,135,154,125,53,56,56,24,131,131,131,49,48,48,16,63,252,225,15,227,151,191,252,229,158,55,54,187,213,22,95,0,192,154,126,24,33,29,241,233,151,
234,150,143,145,215,66,0,0,32,0,73,68,65,84,203,229,152,152,152,48,32,4,53,141,97,124,124,60,198,199,199,227,7,63,248,65,108,111,111,199,31,255,248,71,97,109,241,5,0,56,180,144,238,237,237,141,114,185,
28,149,74,197,128,56,48,110,249,230,64,21,139,197,248,171,191,250,171,40,149,74,145,78,167,31,250,198,135,160,6,0,107,58,79,19,211,199,143,31,143,66,161,16,223,252,230,55,197,52,7,206,14,53,7,238,179,
115,171,47,94,188,24,59,59,59,241,235,95,255,122,207,27,160,221,106,139,47,0,88,211,121,154,144,142,136,56,127,254,124,84,42,149,152,159,159,55,32,4,53,205,101,102,102,38,102,102,102,226,251,223,255,126,
236,236,236,196,159,254,244,167,93,111,136,162,218,226,11,0,214,116,158,36,166,187,187,187,99,117,117,53,174,93,187,102,56,8,106,154,219,230,230,102,156,59,119,46,126,244,163,31,197,191,254,235,191,198,
189,123,247,118,189,49,10,107,139,47,0,180,146,84,202,127,133,249,164,33,221,214,214,22,147,147,147,177,188,188,28,99,99,99,6,132,160,166,53,140,141,141,197,216,216,88,220,186,117,43,182,183,183,227,119,
191,251,221,158,55,74,97,45,168,1,192,154,78,173,144,142,136,120,254,249,231,163,82,169,196,234,234,170,1,113,100,92,14,227,72,45,46,46,198,155,111,190,25,73,146,68,123,123,251,67,223,56,169,122,241,186,
154,13,0,214,244,22,140,233,116,58,29,171,171,171,241,214,91,111,137,105,142,156,29,106,142,92,38,147,137,76,38,19,23,47,94,140,155,55,111,198,157,59,119,246,188,129,218,173,222,203,213,108,0,176,166,
183,82,72,71,68,12,12,12,68,185,92,142,124,62,111,64,8,106,168,54,57,57,25,147,147,147,241,207,255,252,207,241,206,59,239,196,127,255,247,127,239,122,67,21,213,187,185,154,13,0,130,186,85,98,250,217,103,
159,141,114,185,28,27,27,27,134,131,160,134,47,178,177,177,17,231,207,159,143,219,183,111,199,237,219,183,227,238,221,187,187,222,88,133,181,197,23,0,154,137,139,228,159,31,210,169,84,42,166,166,166,98,
121,121,57,70,70,70,12,8,65,13,143,34,155,205,70,54,155,141,145,145,145,216,217,217,137,223,252,230,55,123,222,104,91,61,172,45,190,0,208,28,92,36,175,125,123,247,185,115,231,162,82,169,196,194,194,130,
39,9,245,251,153,220,8,168,103,115,115,115,241,245,175,127,61,182,182,182,162,171,171,235,161,111,188,22,95,0,160,225,62,144,183,248,69,242,7,63,211,117,118,118,70,169,84,138,215,94,123,77,76,83,247,236,
80,83,247,6,7,7,99,112,112,48,6,6,6,226,135,63,252,97,252,242,151,191,220,243,6,220,138,187,213,130,26,0,172,233,205,20,210,17,17,163,163,163,81,44,22,35,151,203,121,98,32,168,97,63,141,143,143,199,248,
248,120,252,224,7,63,136,157,157,157,248,195,31,254,208,210,97,237,150,111,0,176,166,55,75,72,247,246,246,70,165,82,137,114,185,236,9,65,99,189,126,141,128,70,83,44,22,227,27,223,248,70,148,74,165,72,
167,211,15,125,131,182,248,2,0,245,172,149,118,168,31,252,172,118,252,248,241,40,20,10,241,205,111,126,83,76,211,144,236,80,211,144,134,134,134,98,104,104,40,46,94,188,24,239,188,243,78,252,199,127,252,
199,158,55,234,102,223,173,118,203,55,0,52,135,86,184,72,94,107,211,227,252,249,243,81,169,84,98,126,126,222,147,0,65,13,71,97,102,102,38,102,102,102,226,251,223,255,126,236,236,236,196,159,254,244,167,
93,111,220,205,28,213,130,26,0,172,233,141,24,211,221,221,221,177,186,186,26,215,174,93,243,224,35,168,161,30,108,110,110,198,249,243,231,227,167,63,253,105,252,252,231,63,143,123,247,238,237,122,3,111,
198,176,118,203,55,0,8,234,70,10,233,182,182,182,200,229,114,177,182,182,22,163,163,163,30,120,4,53,212,147,145,145,145,24,25,25,137,225,225,225,216,222,222,142,223,253,238,119,123,222,208,155,41,172,
237,80,3,64,115,104,182,139,228,181,110,239,238,235,235,139,74,165,18,43,43,43,30,112,154,235,245,107,4,52,155,197,197,197,120,243,205,55,35,73,146,104,111,111,127,232,27,188,160,6,0,172,233,7,19,211,
233,116,58,86,87,87,227,198,141,27,98,154,166,100,135,154,166,148,201,100,34,147,201,196,197,139,23,227,230,205,155,113,231,206,157,61,111,244,141,190,91,237,150,111,0,104,14,205,176,166,215,218,180,24,
24,24,136,74,165,18,211,211,211,30,100,4,53,52,162,201,201,201,152,156,156,140,119,223,125,55,118,118,118,226,147,79,62,217,245,198,223,200,81,45,168,1,160,57,52,250,14,245,131,49,221,211,211,19,229,114,
57,146,36,241,224,34,168,161,25,172,175,175,199,139,47,190,24,183,111,223,142,219,183,111,199,221,187,119,119,45,0,141,24,214,110,249,6,128,230,208,168,23,201,31,12,233,84,42,21,83,83,83,177,188,188,28,
35,35,35,30,88,4,53,52,147,108,54,27,217,108,54,70,70,70,98,103,103,39,126,243,155,223,236,89,16,26,41,172,5,53,0,52,135,70,91,211,107,221,222,125,238,220,185,168,84,42,177,176,176,224,1,165,165,184,103,
148,150,51,55,55,23,95,255,250,215,99,107,107,43,186,186,186,30,186,64,212,237,139,215,45,223,0,32,168,143,56,166,59,59,59,35,73,146,120,253,245,215,197,52,45,201,14,53,45,105,112,112,48,6,7,7,99,96,96,
32,126,242,147,159,196,7,31,124,208,112,103,87,219,161,6,128,230,208,8,23,201,107,109,58,140,142,142,70,177,88,140,92,46,231,65,68,80,67,43,26,31,31,143,241,241,241,120,255,253,247,99,123,123,59,254,240,
135,63,236,89,56,234,53,172,237,80,3,128,160,62,138,144,238,237,237,141,74,165,18,229,114,217,131,135,215,175,17,64,196,218,218,90,124,227,27,223,136,82,169,20,233,116,250,161,11,73,61,176,67,13,0,205,
161,94,215,244,7,63,3,29,63,126,60,10,133,66,124,243,155,223,20,211,240,217,235,194,8,224,83,67,67,67,49,52,52,20,153,76,38,222,123,239,189,248,213,175,126,181,103,65,169,167,221,106,59,212,0,32,168,15,
35,164,35,34,46,92,184,16,149,74,37,242,249,188,7,12,170,63,147,27,1,236,54,61,61,29,223,254,246,183,227,250,245,235,113,234,212,169,135,46,48,22,95,0,224,169,62,144,215,209,69,242,7,63,235,116,119,119,
199,214,214,86,124,231,59,223,17,211,80,131,29,106,248,28,155,155,155,113,254,252,249,248,233,79,127,26,63,255,249,207,235,238,75,203,236,80,3,64,115,168,135,139,228,15,134,116,91,91,91,228,114,185,88,
91,91,139,209,209,81,15,18,8,106,120,124,35,35,35,49,50,50,18,217,108,54,182,183,183,227,227,143,63,222,179,240,28,85,88,219,161,6,128,230,112,148,23,201,107,221,125,215,215,215,23,73,146,196,242,242,
178,7,7,30,246,250,53,2,120,184,66,161,16,111,188,241,70,108,109,109,69,103,103,231,67,23,34,65,13,0,212,251,154,254,224,103,152,142,142,142,40,149,74,113,227,198,13,49,13,143,200,14,53,60,162,76,38,19,
153,76,38,250,251,251,227,230,205,155,113,231,206,157,61,11,210,97,238,86,187,229,27,0,154,195,97,175,233,181,54,3,6,6,6,162,82,169,196,244,244,180,7,4,4,53,28,156,201,201,201,152,156,156,140,119,223,
125,55,118,118,118,226,147,79,62,57,146,176,22,212,0,208,28,14,115,135,250,193,152,238,233,233,137,114,185,28,73,146,120,32,64,80,195,225,89,95,95,143,179,103,207,198,207,126,246,179,184,125,251,118,220,
189,123,119,215,98,117,208,81,237,150,111,0,104,14,135,113,145,252,193,144,78,165,82,49,59,59,27,139,139,139,49,60,60,236,65,0,65,13,135,111,120,120,56,134,135,135,99,116,116,52,118,118,118,226,163,143,
62,218,179,112,29,84,88,11,106,0,104,14,7,185,166,215,186,189,251,220,185,115,145,36,73,92,186,116,201,240,225,41,185,103,20,246,193,236,236,108,124,247,187,223,141,171,87,175,70,87,87,215,67,23,178,125,
121,241,186,229,27,0,4,245,99,196,116,103,103,103,36,73,18,175,191,254,186,152,134,125,98,135,26,246,209,149,43,87,162,191,191,63,110,221,186,21,31,124,240,193,129,158,93,109,135,26,0,154,195,126,95,36,
175,117,49,127,116,116,52,138,197,98,228,114,57,3,7,65,13,245,107,108,108,44,198,198,198,226,253,247,223,143,157,157,157,248,253,239,127,191,103,129,123,210,176,174,94,32,5,53,0,52,135,253,90,211,107,
133,244,233,211,167,35,73,146,88,93,93,53,104,56,0,238,25,133,3,178,182,182,22,111,189,245,86,148,74,165,72,167,211,15,93,240,30,251,197,235,150,111,0,104,142,15,228,85,107,250,147,126,70,120,240,239,
75,167,211,81,40,20,226,198,141,27,98,26,14,144,29,106,56,64,67,67,67,247,255,120,239,189,247,226,195,15,63,220,179,240,61,233,110,117,38,147,49,96,0,104,2,79,179,67,253,121,103,74,151,203,229,200,231,
243,134,11,7,204,22,23,28,130,169,169,169,248,214,183,190,21,215,175,95,143,83,167,78,61,116,33,60,200,133,23,0,168,47,79,122,145,252,193,207,16,39,79,158,140,173,173,173,120,251,237,183,197,52,28,18,
59,212,112,136,54,55,55,227,229,151,95,142,219,183,111,199,207,126,246,179,248,203,95,254,178,107,65,124,212,221,106,183,123,3,64,115,105,107,107,187,255,101,166,143,27,210,169,84,42,166,166,166,98,105,
105,41,70,71,71,13,19,4,53,52,175,108,54,27,217,108,54,50,153,76,108,111,111,199,199,31,127,188,103,129,124,88,88,219,161,6,128,230,146,74,165,226,238,221,187,143,21,210,17,17,125,125,125,145,36,73,44,
47,47,27,34,28,197,107,215,8,224,104,20,10,133,120,243,205,55,99,107,107,43,78,156,56,241,208,5,83,80,3,64,243,122,216,218,254,224,103,131,142,142,142,40,149,74,113,227,198,13,49,13,71,200,14,53,28,161,
193,193,193,24,28,28,140,87,94,121,37,222,127,255,253,184,115,231,206,158,133,179,214,110,181,91,190,1,160,185,124,222,218,254,121,95,58,150,36,73,76,77,77,25,28,8,106,96,98,98,34,38,38,38,226,189,247,
222,139,157,157,157,248,227,31,255,248,133,97,45,168,1,160,185,212,218,161,126,48,166,123,122,122,162,82,169,68,165,82,49,48,16,212,192,131,74,165,82,156,61,123,54,126,241,139,95,196,173,91,183,226,207,
127,254,115,205,69,213,45,223,0,208,92,190,232,44,234,84,42,21,179,179,179,177,184,184,24,195,195,195,134,5,130,26,248,60,159,157,91,157,201,100,98,103,103,39,62,250,232,163,61,127,141,160,6,128,230,242,
121,107,251,75,47,189,20,149,74,37,46,93,186,100,72,32,168,129,71,53,59,59,27,179,179,179,241,79,255,244,79,241,238,187,239,198,255,254,239,255,222,255,115,110,249,6,128,230,14,234,238,238,238,88,93,93,
141,209,209,209,24,28,28,52,32,16,212,192,147,184,114,229,74,244,247,247,199,173,91,183,226,131,15,62,136,123,247,238,217,161,6,128,38,83,125,177,124,116,116,52,138,197,98,228,114,57,131,1,65,13,60,173,
177,177,177,24,27,27,139,31,255,248,199,177,189,189,29,255,247,127,255,103,40,0,208,68,218,218,218,226,244,233,211,145,36,73,172,174,174,26,8,8,106,96,191,45,45,45,197,233,211,167,227,195,15,63,52,12,
0,104,34,43,43,43,247,191,71,5,16,212,192,1,177,216,2,64,243,185,122,245,170,33,64,3,242,205,70,0,0,0,32,168,1,0,0,64,80,3,0,0,128,160,6,0,0,0,65,13,0,0,0,130,26,0,0,0,16,212,0,0,0,32,168,1,0,0,64,80,
3,0,0,128,160,6,0,0,0,65,13,0,0,0,8,106,0,0,0,16,212,0,0,0,32,168,1,0,0,64,80,3,0,0,128,160,6,0,0,0,4,53,0,0,0,8,106,0,0,0,16,212,0,0,0,32,168,1,0,0,64,80,3,0,0,0,130,26,0,0,0,4,53,0,0,0,8,106,0,0,0,16,
212,0,0,0,32,168,1,0,0,0,65,13,0,0,0,130,26,0,0,0,4,53,0,0,0,8,106,0,0,0,16,212,0,0,0,128,160,6,0,0,0,65,13,0,0,0,130,26,0,0,0,4,53,0,0,0,8,106,0,0,0,64,80,3,0,0,128,160,6,0,0,0,65,13,0,0,0,130,26,0,0,
0,4,53,0,0,0,32,168,1,0,0,64,80,3,0,0,128,160,6,0,0,0,65,13,0,0,0,130,26,0,0,0,16,212,0,0,0,32,168,1,0,0,64,80,3,0,0,128,160,6,0,0,0,65,13,0,0,0,8,106,0,0,0,16,212,0,0,0,32,168,1,0,0,64,80,3,0,0,128,160,
6,0,0,0,4,53,0,0,0,8,106,0,0,0,16,212,0,0,0,32,168,1,0,0,64,80,3,0,0,0,130,26,0,0,0,4,53,0,0,0,8,106,0,0,0,16,212,0,0,0,32,168,1,0,0,0,65,13,0,0,0,130,26,0,0,0,4,53,0,0,0,8,106,0,0,0,16,212,0,0,0,128,
160,6,0,0,0,65,13,0,0,0,130,26,0,0,0,4,53,0,0,0,8,106,0,0,0,64,80,3,0,0,128,160,6,0,0,0,65,13,0,0,0,130,26,0,0,0,4,53,0,0,0,32,168,1,0,0,64,80,3,0,0,128,160,6,0,0,0,65,13,0,0,0,130,26,0,0,0,16,212,0,0,
0,32,168,1,0,0,64,80,3,0,0,128,160,6,0,0,0,65,13,0,0,0,130,26,0,0,0,16,212,0,0,0,32,168,1,0,0,64,80,3,0,0,128,160,6,0,0,0,65,13,0,0,0,8,106,0,0,0,16,212,0,0,0,32,168,1,0,0,160,57,28,55,2,128,167,243,111,
255,246,111,241,189,239,125,47,254,252,231,63,27,70,29,241,120,212,143,127,248,135,127,136,127,252,199,127,52,136,58,241,181,175,125,45,22,22,22,12,2,64,80,3,28,189,92,46,23,255,254,239,255,30,55,111,
222,52,12,168,225,127,254,231,127,12,161,78,140,143,143,139,105,128,125,228,150,111,128,125,48,51,51,19,125,125,125,6,81,135,254,238,239,254,238,158,41,152,61,17,39,78,156,136,213,213,85,131,0,216,71,
109,247,238,89,235,0,246,195,143,126,244,163,248,251,191,255,251,248,236,125,85,76,0,71,237,111,254,230,111,218,62,251,223,91,91,91,113,237,218,53,67,1,216,71,118,168,1,246,201,242,242,114,76,79,79,215,
252,32,11,112,148,49,253,210,75,47,197,232,232,168,161,0,8,106,128,250,142,234,103,159,125,86,84,3,117,227,216,177,99,145,36,73,12,14,14,26,6,128,160,6,168,95,35,35,35,81,169,84,12,2,56,82,213,23,243,
46,93,186,20,243,243,243,134,2,32,168,1,234,95,146,36,49,52,52,84,243,131,45,192,97,198,116,111,111,175,152,6,16,212,0,141,101,125,125,61,58,58,58,12,2,56,82,73,146,196,240,240,176,65,0,8,106,128,198,
49,57,57,25,43,43,43,247,127,182,75,13,28,134,234,247,154,241,241,241,88,95,95,55,20,0,65,13,208,120,38,38,38,226,220,185,115,162,26,56,116,206,156,6,16,212,0,13,45,147,201,196,198,198,70,28,59,118,204,
48,128,3,87,125,209,174,88,44,70,46,151,51,20,0,65,13,208,184,230,231,231,99,110,110,174,230,7,94,128,131,136,233,115,231,206,57,115,26,64,80,3,52,135,133,133,133,248,210,151,190,36,170,129,3,231,204,
105,0,65,13,208,84,134,135,135,35,73,18,131,0,14,68,245,69,186,249,249,249,184,116,233,146,161,0,8,106,128,230,81,42,149,98,108,108,172,230,7,96,128,253,136,233,222,222,94,49,13,32,168,1,154,83,177,88,
140,174,174,46,81,13,28,136,74,165,226,204,105,0,65,13,208,156,198,199,199,163,88,44,26,4,176,47,170,47,202,141,141,141,69,185,92,54,20,0,65,13,208,188,94,125,245,213,232,239,239,175,249,129,24,224,73,
116,118,118,58,115,26,64,80,3,180,134,141,141,141,72,167,211,6,1,60,177,7,207,156,158,152,152,48,20,0,65,13,208,252,242,249,124,20,10,133,154,31,140,1,30,39,166,207,157,59,183,235,11,15,1,16,212,0,77,
111,102,102,38,250,250,250,68,53,240,196,156,57,13,32,168,1,90,210,208,208,80,36,73,18,109,109,58,26,120,116,206,156,6,16,212,0,68,196,242,242,114,76,77,77,213,252,160,12,240,69,49,237,204,105,0,65,13,
32,170,151,151,227,153,103,158,17,213,192,99,113,230,52,128,160,6,104,121,163,163,163,145,36,137,65,0,95,168,250,98,219,232,232,168,51,167,1,4,53,0,17,159,30,163,85,253,165,66,118,169,129,207,211,217,
217,25,107,107,107,6,1,32,168,1,248,76,165,82,137,142,142,14,131,0,246,168,190,200,182,182,182,230,204,105,0,65,13,64,181,201,201,201,88,94,94,174,249,1,26,16,211,17,159,158,57,61,62,62,110,40,0,130,26,
128,90,81,125,246,236,89,81,13,236,225,204,105,0,65,13,192,23,200,100,50,145,36,73,164,82,222,154,129,221,23,213,230,230,230,28,147,5,32,168,1,248,34,11,11,11,49,55,55,87,243,3,53,208,154,49,237,204,105,
0,65,13,192,35,42,20,10,241,165,47,125,73,84,3,17,17,81,46,151,35,155,205,26,4,128,160,6,224,97,134,135,135,157,77,13,45,172,250,34,218,200,200,72,84,42,21,67,1,16,212,0,60,170,82,169,20,163,163,163,53,
63,96,3,173,161,179,179,51,138,197,162,65,0,8,106,0,30,87,177,88,140,174,174,46,81,13,45,164,250,117,190,186,186,234,204,105,0,65,13,192,147,200,229,114,177,182,182,102,16,208,130,49,125,238,220,185,200,
229,114,134,2,32,168,1,120,82,215,174,93,139,151,95,126,185,230,7,110,160,73,63,156,165,82,81,46,151,157,57,13,32,168,1,120,90,73,146,196,241,227,199,13,2,154,88,245,197,178,249,249,249,40,20,10,134,2,
32,168,1,120,90,179,179,179,177,176,176,80,243,131,55,208,92,49,253,236,179,207,198,252,252,188,161,0,8,106,0,246,203,220,220,92,60,255,252,243,162,26,154,92,185,92,142,145,145,17,131,0,16,212,0,236,151,
161,161,161,72,146,36,218,218,116,52,52,147,234,139,99,217,108,54,54,54,54,12,5,64,80,3,176,223,86,86,86,98,114,114,178,230,7,113,160,177,117,116,116,248,86,127,0,65,13,192,65,71,245,51,207,60,35,170,
161,9,60,120,230,244,212,212,148,161,0,8,106,0,14,202,232,232,104,172,175,175,27,4,52,81,76,191,240,194,11,206,156,6,16,212,0,28,134,173,173,173,93,231,211,218,165,134,6,254,32,150,74,197,198,198,70,100,
50,25,195,0,16,212,0,28,134,114,185,28,29,29,29,6,1,13,168,250,34,216,220,220,156,51,167,1,4,53,0,135,105,106,106,42,150,150,150,106,126,64,7,26,35,166,79,157,58,21,151,46,93,50,20,0,65,13,192,81,68,245,
139,47,190,40,170,161,65,85,42,21,103,78,3,8,106,0,142,66,38,147,137,141,141,141,72,165,188,149,67,35,168,190,232,53,60,60,236,204,105,0,65,13,192,81,90,88,88,136,217,217,217,154,31,216,129,250,212,222,
222,30,197,98,209,32,0,4,53,0,71,173,80,40,68,79,79,143,168,134,58,230,204,105,0,65,13,64,29,202,102,179,110,29,133,6,137,233,23,94,120,33,38,38,38,12,5,64,80,3,80,47,214,215,215,119,125,185,145,93,106,
168,195,15,93,169,84,36,73,226,204,105,0,65,13,64,189,41,149,74,209,213,213,101,16,80,71,170,47,110,205,206,206,198,226,226,162,161,0,8,106,0,234,77,46,151,139,213,213,213,154,31,228,129,163,141,233,83,
167,78,197,194,194,130,161,0,8,106,0,234,213,216,216,88,188,252,242,203,162,26,234,76,185,92,118,230,52,128,160,6,160,158,13,14,14,70,165,82,137,227,199,143,27,6,28,161,7,207,156,222,220,220,52,20,0,65,
13,64,189,155,155,155,139,75,151,46,213,252,96,15,28,46,103,78,3,8,106,0,26,48,170,159,123,238,57,81,13,71,192,153,211,0,130,26,128,6,246,217,45,166,109,109,58,26,142,42,166,207,156,57,227,204,105,0,65,
13,64,35,90,89,89,217,245,97,222,46,53,28,226,7,172,84,42,54,54,54,156,57,13,32,168,1,104,84,171,171,171,113,242,228,73,81,13,135,160,250,245,53,51,51,227,204,105,0,65,13,64,35,27,29,29,141,82,169,100,
16,112,136,49,125,234,212,169,40,20,10,134,2,32,168,1,104,116,87,174,92,137,139,23,47,214,252,224,15,236,63,103,78,3,8,106,0,154,200,250,250,122,180,183,183,27,4,28,128,234,139,84,67,67,67,206,156,6,16,
212,0,52,147,124,62,31,75,75,75,53,3,0,216,31,237,237,237,254,19,11,0,65,13,64,51,154,158,158,142,23,95,124,81,84,195,62,170,126,29,173,172,172,56,115,26,64,80,3,208,140,50,153,76,36,73,18,169,148,183,
126,216,239,152,62,115,230,76,76,78,78,26,10,128,160,6,160,89,21,10,133,200,231,243,53,131,0,120,194,15,83,206,156,6,16,212,0,180,134,165,165,165,232,233,233,17,213,240,20,170,95,55,249,124,222,153,211,
0,130,26,128,86,144,205,102,163,82,169,24,4,236,67,76,159,58,117,74,76,3,8,106,0,90,73,165,82,137,108,54,91,51,16,128,71,183,190,190,238,204,105,0,65,13,64,43,134,192,137,19,39,12,2,30,67,245,197,167,
76,38,19,151,47,95,54,20,0,65,13,64,171,201,229,114,177,178,178,82,51,20,128,47,230,204,105,0,4,53,128,168,142,243,231,207,139,106,120,4,213,175,143,229,229,229,152,158,158,54,20,0,65,13,64,171,26,28,
28,140,36,73,226,248,241,227,134,1,143,24,211,103,206,156,137,169,169,41,67,1,64,80,3,180,186,185,185,185,152,159,159,175,25,14,192,110,109,109,109,145,36,137,51,167,1,16,212,0,124,106,126,126,62,190,
252,229,47,139,106,168,225,193,51,167,151,150,150,12,5,0,65,13,192,167,134,135,135,35,73,18,131,128,47,136,233,103,158,121,70,76,3,32,168,1,216,171,88,44,70,46,151,171,25,18,64,68,185,92,118,230,52,0,
130,26,128,218,214,214,214,226,228,201,147,162,26,194,153,211,0,8,106,0,30,195,216,216,88,20,139,69,131,128,42,206,156,6,64,80,3,240,72,190,250,213,175,198,192,192,192,253,159,237,82,211,138,156,57,13,
128,160,6,224,137,148,203,229,72,167,211,6,65,203,199,116,95,95,159,51,167,1,16,212,0,60,186,124,62,31,139,139,139,53,3,3,90,69,91,91,91,108,108,108,56,115,26,0,65,13,192,227,153,153,153,137,190,190,62,
81,77,75,113,230,52,0,130,26,128,167,246,217,183,26,167,82,150,10,90,47,166,159,121,230,153,93,119,105,0,128,160,6,224,177,20,10,133,93,95,198,100,151,154,86,81,46,151,99,116,116,212,32,0,16,212,0,60,
185,229,229,229,120,246,217,103,69,53,77,173,250,121,125,225,194,5,103,78,3,32,168,1,120,122,217,108,54,42,149,138,65,208,18,210,233,180,231,59,0,130,26,128,253,147,36,73,12,15,15,223,255,217,46,53,205,
228,193,51,167,243,249,188,161,0,32,168,1,216,63,165,82,41,58,59,59,13,130,166,141,233,231,159,127,222,153,211,0,8,106,0,246,223,228,228,100,172,172,172,212,12,17,104,116,109,109,109,145,36,73,12,13,13,
25,6,0,130,26,128,253,55,49,49,17,231,207,159,23,213,52,133,234,231,239,244,244,244,174,11,70,0,32,168,1,216,87,131,131,131,81,169,84,226,216,177,99,134,65,211,196,116,119,119,183,51,167,1,16,212,0,28,
188,249,249,249,152,159,159,175,25,38,208,136,42,149,74,140,141,141,25,4,0,130,26,128,131,119,233,210,165,248,242,151,191,44,170,105,72,15,158,57,189,181,181,101,40,0,8,106,0,14,199,240,240,112,36,73,
98,16,52,180,116,58,29,229,114,217,32,0,16,212,0,28,174,98,177,24,227,227,227,247,127,182,75,77,35,168,126,158,46,45,45,197,204,204,140,161,0,32,168,1,56,124,107,107,107,209,221,221,45,170,105,184,152,
62,125,250,116,76,79,79,27,10,0,130,26,128,163,49,62,62,30,197,98,209,32,104,40,206,156,6,64,80,3,80,23,174,94,189,26,3,3,3,247,127,182,75,77,61,170,126,94,78,77,77,197,234,234,170,161,0,32,168,1,56,122,
229,114,57,210,233,180,65,80,247,49,221,221,221,29,75,75,75,134,2,128,160,6,160,62,228,243,249,40,20,10,53,3,6,234,73,185,92,118,230,52,0,130,26,128,250,50,51,51,19,125,125,125,162,154,186,242,224,153,
211,95,249,202,87,12,5,0,65,13,64,125,25,26,26,138,36,73,162,173,77,71,83,127,156,57,13,128,160,6,160,174,45,47,47,239,58,138,200,46,53,71,169,250,249,183,184,184,232,204,105,0,4,53,0,245,31,213,207,62,
251,172,168,166,110,98,250,244,233,211,145,207,231,13,5,0,65,13,64,125,27,25,25,137,74,165,98,16,212,5,103,78,3,32,168,1,104,40,15,6,140,93,106,14,147,51,167,1,16,212,0,52,180,245,245,245,232,232,232,
48,8,142,44,166,157,57,13,128,160,6,160,33,77,78,78,198,202,202,74,205,208,129,195,224,204,105,0,4,53,0,13,107,98,98,34,206,158,61,43,170,57,20,213,207,175,87,94,121,197,153,211,0,8,106,0,26,87,38,147,
137,205,205,205,56,118,236,152,97,112,104,210,233,180,47,198,3,64,80,3,208,248,230,231,231,99,110,110,238,254,207,118,169,57,8,213,207,171,66,161,224,204,105,0,4,53,0,205,97,97,97,33,190,244,165,47,137,
106,14,60,166,79,159,62,45,166,1,16,212,0,52,143,225,225,225,72,146,196,32,56,80,206,156,6,64,80,3,208,148,74,165,210,174,111,92,182,75,205,126,168,126,30,77,78,78,58,115,26,0,65,13,64,115,90,91,91,139,
174,174,46,81,205,190,199,116,119,119,119,44,47,47,27,10,0,130,26,128,230,148,203,229,162,84,42,25,4,251,110,125,125,221,153,211,0,8,106,0,154,219,213,171,87,99,96,96,224,254,207,118,169,121,18,213,207,
155,11,23,46,196,149,43,87,12,5,0,65,13,64,243,43,151,203,145,78,167,13,130,167,150,78,167,163,92,46,27,4,0,130,26,128,214,144,207,231,99,105,105,233,254,207,118,169,121,28,213,207,151,197,197,69,199,
100,1,32,168,1,104,189,168,126,241,197,23,69,53,79,28,211,167,79,159,142,124,62,111,40,0,8,106,0,90,75,38,147,137,36,73,34,149,178,20,241,248,156,57,13,128,160,6,160,165,21,10,133,93,183,235,218,165,230,
139,84,63,63,166,167,167,157,57,13,128,160,6,160,181,45,46,46,70,111,111,175,168,230,145,99,186,187,187,59,22,23,23,13,5,0,65,13,64,107,203,102,179,177,177,177,97,16,60,178,74,165,226,204,105,0,4,53,0,
68,68,148,74,165,93,129,100,151,154,106,15,158,57,189,181,181,101,40,0,8,106,0,248,204,218,218,90,116,117,117,137,106,62,87,123,123,123,36,73,98,16,0,8,106,0,168,150,203,229,162,88,44,26,4,187,84,95,88,
89,94,94,142,233,233,105,67,1,64,80,3,192,131,94,125,245,213,24,24,24,168,25,83,180,118,76,159,57,115,38,166,166,166,12,5,0,65,13,0,159,167,92,46,71,58,157,54,8,238,251,236,204,233,76,38,99,24,0,8,106,
0,248,60,249,124,62,150,150,150,238,255,108,151,186,53,85,63,238,51,51,51,187,158,19,0,32,168,1,224,11,162,250,204,153,51,162,90,76,199,169,83,167,156,57,13,128,160,6,128,71,149,201,100,98,115,115,51,
82,41,203,84,171,43,151,203,49,50,50,98,16,0,8,106,0,120,84,133,66,33,102,102,102,238,255,108,151,186,53,84,63,206,67,67,67,177,185,185,105,40,0,8,106,0,120,92,139,139,139,209,211,211,35,170,91,80,123,
123,123,148,74,37,131,0,64,80,3,192,147,200,102,179,177,177,177,97,16,45,162,250,130,201,234,234,170,99,178,0,16,212,0,240,52,214,215,215,99,116,116,180,102,116,209,156,49,253,194,11,47,196,196,196,132,
161,0,32,168,1,224,105,21,139,197,232,234,234,18,213,173,240,193,36,149,114,230,52,0,130,26,0,246,75,46,151,139,98,177,104,16,77,170,250,2,201,236,236,172,99,178,0,16,212,0,176,159,94,125,245,213,24,24,
24,168,25,97,52,71,76,159,58,117,42,22,22,22,12,5,0,65,13,0,251,173,92,46,71,58,157,54,136,38,85,169,84,156,57,13,128,160,6,128,131,144,207,231,119,221,14,108,151,186,177,85,63,126,190,209,29,0,65,13,
0,7,108,102,102,38,250,250,250,68,117,19,233,232,232,240,223,200,3,32,168,1,224,160,101,50,153,184,124,249,114,164,82,150,176,70,86,125,33,100,109,109,45,38,39,39,13,5,0,65,13,0,7,173,80,40,196,204,204,
76,205,56,163,177,98,250,220,185,115,145,203,229,12,5,0,65,13,0,135,101,113,113,49,122,122,122,68,117,3,59,118,236,88,36,73,18,131,131,131,134,1,128,160,6,128,195,226,75,172,26,83,245,133,143,185,185,
185,184,116,233,146,161,0,32,168,1,224,176,173,175,175,199,232,232,104,205,88,163,190,99,186,183,183,215,153,211,0,8,106,0,56,74,197,98,49,186,186,186,68,117,131,169,84,42,49,60,60,108,16,0,8,106,0,56,
42,185,92,206,145,75,13,160,250,66,199,216,216,88,148,203,101,67,1,64,80,3,192,81,123,245,213,87,99,96,96,160,102,188,81,95,78,156,56,17,107,107,107,6,1,128,160,6,128,122,81,46,151,35,157,78,27,68,29,
170,190,192,81,44,22,29,147,5,128,160,6,128,122,146,207,231,99,113,113,177,102,196,81,31,49,253,210,75,47,237,250,18,57,0,16,212,0,80,39,102,102,102,162,175,175,79,84,215,33,103,78,3,32,168,1,160,142,
101,50,153,184,124,249,114,164,82,150,183,122,80,125,65,99,97,97,33,230,231,231,13,5,0,65,13,0,245,170,80,40,196,204,204,76,205,168,227,104,98,186,183,183,55,230,230,230,12,5,0,65,13,0,245,110,113,113,
49,122,122,122,68,117,157,72,146,196,153,211,0,8,106,0,104,4,217,108,54,54,54,54,12,226,136,84,95,192,200,229,114,177,190,190,110,40,0,8,106,0,104,20,235,235,235,187,190,81,218,46,245,225,199,244,137,
19,39,98,101,101,197,80,0,16,212,0,208,104,138,197,98,116,117,117,137,234,35,178,190,190,238,204,105,0,4,53,0,52,162,92,46,23,197,98,209,32,14,73,245,5,139,254,254,254,184,122,245,170,161,0,32,168,1,160,
81,189,250,234,171,49,48,48,80,51,250,56,24,199,143,31,143,74,165,98,16,0,8,106,0,104,116,229,114,57,210,233,180,65,28,160,234,11,21,133,66,33,102,103,103,13,5,0,65,13,0,141,46,159,207,199,226,226,98,
205,248,99,127,99,250,185,231,158,19,211,0,8,106,0,104,38,51,51,51,209,215,215,39,170,15,80,91,91,91,36,73,18,67,67,67,134,1,128,160,6,128,102,145,201,100,226,242,229,203,145,74,89,250,246,83,245,133,
137,137,137,137,88,91,91,51,20,0,4,53,0,52,155,66,161,16,249,124,190,102,12,242,116,49,221,221,221,237,204,105,0,4,53,0,52,179,165,165,165,232,233,233,17,213,251,108,125,125,61,198,198,198,12,2,0,65,13,
0,205,42,155,205,198,198,198,134,65,60,165,234,11,17,175,188,242,74,92,185,114,197,80,0,16,212,0,208,236,214,215,215,99,116,116,180,102,28,242,120,210,233,180,51,167,1,16,212,0,208,74,138,197,98,116,117,
117,137,234,39,80,61,171,197,197,197,152,153,153,49,20,0,4,53,0,180,138,92,46,231,27,169,159,50,166,79,159,62,189,235,75,222,0,64,80,3,64,139,184,118,237,90,244,247,247,215,140,69,190,152,51,167,1,64,
80,3,208,226,54,54,54,34,157,78,27,196,35,168,190,224,48,61,61,29,171,171,171,134,2,128,160,6,128,86,149,207,231,99,113,113,177,102,52,82,59,166,187,187,187,119,205,12,0,4,53,0,180,168,153,153,153,232,
235,235,19,213,143,168,82,169,56,115,26,0,4,53,0,68,100,50,153,184,124,249,114,164,82,150,197,90,170,47,48,92,184,112,33,182,182,182,12,5,0,4,53,0,124,170,80,40,236,250,198,106,187,212,123,181,183,183,
71,146,36,6,1,0,130,26,0,118,91,90,90,138,158,158,30,81,93,165,122,6,203,203,203,49,61,61,237,137,2,0,130,26,0,118,203,102,179,177,177,177,97,16,53,98,250,204,153,51,49,53,53,101,40,0,32,168,1,160,182,
245,245,245,24,25,25,169,25,149,173,234,179,51,167,51,153,140,39,8,0,8,106,0,248,124,165,82,41,186,186,186,90,58,170,171,127,207,51,51,51,177,180,180,228,137,1,0,130,26,0,190,88,46,151,139,181,181,181,
150,253,253,87,199,244,169,83,167,156,57,13,0,130,26,0,30,221,181,107,215,162,191,191,191,102,100,182,146,7,111,129,7,0,4,53,0,60,212,198,198,70,164,211,233,150,250,61,87,95,56,24,26,26,138,203,151,47,
123,34,0,128,160,6,128,199,147,207,231,119,221,238,220,74,187,212,237,237,237,81,42,149,60,9,0,64,80,3,192,147,153,153,153,137,190,190,190,150,136,234,234,223,219,202,202,138,99,178,0,64,80,3,192,147,
203,100,50,113,249,242,229,72,165,154,123,201,172,142,233,23,94,120,33,38,39,39,61,248,0,32,168,1,224,233,20,10,133,200,231,243,53,227,179,233,62,24,164,82,206,156,6,0,65,13,0,251,103,105,105,41,122,122,
122,154,50,170,171,127,47,179,179,179,142,201,2,0,65,13,0,251,39,155,205,198,198,198,70,211,253,190,30,60,115,122,97,97,193,131,13,0,130,26,0,246,215,131,103,50,55,219,173,223,149,74,197,153,211,0,32,
168,1,224,96,148,74,165,232,234,234,106,138,168,174,254,181,55,235,14,60,0,8,106,0,168,19,185,92,46,214,214,214,154,234,247,212,209,209,209,116,191,39,0,16,212,0,80,135,174,93,187,22,253,253,253,247,127,
110,196,93,234,234,95,243,218,218,154,51,167,1,64,80,3,192,225,216,216,216,136,116,58,221,144,191,246,234,152,62,119,238,92,228,114,57,15,40,0,8,106,0,56,28,249,124,62,10,133,66,205,72,109,20,199,142,
29,139,36,73,98,112,112,208,3,10,0,130,26,0,14,207,204,204,76,244,245,245,53,84,84,87,255,26,231,230,230,226,210,165,75,30,72,0,16,212,0,112,184,134,134,134,226,242,229,203,145,74,53,198,114,90,29,211,
189,189,189,206,156,6,0,65,13,0,71,167,80,40,196,204,204,76,205,104,173,103,73,146,196,240,240,176,7,16,0,4,53,0,28,157,197,197,197,232,237,237,173,235,168,174,254,53,141,143,143,199,250,250,186,7,14,
0,4,53,0,28,173,108,54,27,27,27,27,117,251,235,176,46,229,134,0,0,3,91,73,68,65,84,171,142,233,19,39,78,196,234,234,170,7,13,0,4,53,0,212,135,82,169,180,235,248,169,122,189,245,187,92,46,59,38,11,0,4,
53,0,212,151,181,181,181,56,121,242,100,93,69,117,245,175,225,194,133,11,241,213,175,126,213,3,5,0,130,26,0,234,203,216,216,88,36,73,82,151,191,182,116,58,29,229,114,217,131,4,0,130,26,0,234,211,230,230,
102,100,50,153,251,63,31,229,46,117,245,191,123,101,101,101,215,183,145,3,0,130,26,0,234,78,185,92,142,206,206,206,35,141,234,234,127,231,153,51,103,98,106,106,202,3,3,0,130,26,0,234,219,228,228,100,20,
139,197,250,88,232,83,169,216,216,216,216,181,107,14,0,8,106,0,168,91,95,251,218,215,162,191,191,255,254,207,135,185,75,93,253,239,154,155,155,139,197,197,69,15,8,0,8,106,0,104,28,27,27,27,145,78,167,
15,245,223,89,29,211,189,189,189,177,176,176,224,129,0,0,65,13,0,141,37,159,207,199,210,210,82,205,216,61,12,149,74,37,178,217,172,7,2,0,4,53,0,52,102,84,159,61,123,246,80,162,186,250,159,157,203,229,
28,147,5,0,130,26,0,26,87,38,147,137,36,73,226,216,177,99,7,250,239,169,142,233,238,238,238,88,93,93,53,124,0,16,212,0,208,216,22,22,22,98,110,110,174,102,252,30,132,245,245,245,24,31,31,55,120,0,16,212,
0,208,28,81,253,220,115,207,29,72,84,87,255,179,46,92,184,16,87,174,92,49,112,0,16,212,0,208,28,134,135,135,99,115,115,51,218,218,14,110,115,186,189,189,61,146,36,49,108,0,16,212,0,208,92,86,86,86,98,
106,106,234,254,207,251,177,75,93,253,207,88,89,89,137,233,233,105,131,6,0,65,13,0,205,25,213,61,61,61,251,18,213,213,127,239,11,47,188,16,147,147,147,6,12,0,130,26,0,154,211,200,200,200,190,31,103,149,
74,165,34,73,146,200,100,50,6,12,0,130,26,0,154,87,146,36,49,50,50,114,255,231,39,217,165,174,254,123,230,231,231,99,113,113,209,96,1,64,80,3,64,243,43,149,74,209,213,213,245,68,81,93,253,215,246,246,
246,198,194,194,130,129,2,128,160,6,128,214,144,203,229,162,84,42,61,245,63,39,73,146,24,30,30,54,80,0,16,212,0,208,58,174,94,189,26,23,47,94,188,255,243,163,236,82,87,255,53,19,19,19,177,190,190,110,
144,0,32,168,1,160,245,84,42,149,232,232,232,120,164,191,182,58,166,187,187,187,99,101,101,197,0,1,64,80,3,64,107,154,154,154,218,21,198,143,250,223,82,151,203,229,24,31,31,55,64,0,16,212,0,208,186,38,
39,39,163,191,191,255,11,163,186,250,255,187,120,241,98,124,229,43,95,49,56,0,16,212,0,208,218,6,7,7,99,99,99,35,210,233,116,205,63,95,29,211,157,157,157,145,36,137,161,1,128,160,6,0,34,34,242,249,252,
35,221,250,93,42,149,98,114,114,210,192,0,160,14,180,221,187,119,207,20,0,160,14,252,234,87,191,138,239,125,239,123,241,159,255,249,159,53,255,252,133,11,23,226,59,223,249,142,65,1,64,157,176,67,13,0,
117,34,147,201,196,214,214,86,205,91,191,219,219,219,221,234,13,0,130,26,0,248,60,51,51,51,177,180,180,180,231,255,95,91,91,139,233,233,105,3,2,0,65,13,0,124,158,124,62,31,103,207,158,189,255,243,249,
243,231,99,98,98,194,96,0,64,80,3,0,95,36,147,201,196,87,191,250,213,232,236,236,140,142,142,142,184,124,249,114,12,14,14,26,12,0,212,153,227,70,0,0,245,103,122,122,58,126,251,219,223,70,91,91,91,228,
243,121,3,1,128,58,244,255,1,240,201,240,112,163,185,80,62,0,0,0,0,73,69,78,68,174,66,96,130,0,0};

const char* VASTHeaderComponent::dragdrop_over_png = (const char*) resource_VASTHeaderComponent_dragdrop_over_png;
const int VASTHeaderComponent::dragdrop_over_pngSize = 25586;

// JUCER_RESOURCE: move_grabber_png, 1465, "../../Resources/move_grabber.png"
static const unsigned char resource_VASTHeaderComponent_move_grabber_png[] = { 137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,0,128,0,0,0,128,8,6,0,0,0,195,62,97,203,0,0,5,128,73,68,65,84,120,156,237,
157,191,111,29,69,16,199,191,19,72,144,144,252,11,139,0,5,5,80,129,8,33,59,103,59,34,145,168,40,249,3,144,128,20,244,164,167,160,163,162,130,10,33,26,132,16,84,116,52,52,72,20,40,142,119,150,196,36,74,
133,168,16,34,64,176,13,8,33,147,12,58,113,86,192,122,239,238,242,184,31,239,110,230,35,69,118,252,86,183,187,183,159,219,183,183,55,183,11,199,113,28,199,42,84,86,111,85,29,245,105,137,49,62,78,68,143,
50,243,167,115,80,156,214,32,154,222,204,102,5,136,49,46,17,209,78,241,223,13,102,190,216,115,145,90,163,76,128,35,131,175,221,12,136,200,234,191,26,63,103,51,165,180,49,152,10,52,136,57,1,68,100,17,192,
15,135,255,174,170,23,82,74,89,63,165,234,15,83,2,136,200,10,128,93,0,119,77,250,92,85,183,68,196,148,4,102,4,16,145,133,73,87,254,4,182,98,140,161,243,2,246,132,9,1,68,100,25,192,30,128,163,117,210,19,
145,88,145,96,244,2,228,3,190,154,87,254,127,200,37,16,145,179,237,150,174,127,70,125,27,40,34,75,0,118,106,36,45,227,20,51,95,106,183,164,237,98,242,54,48,165,116,223,44,87,254,4,190,18,145,103,26,47,
224,156,48,202,30,32,165,180,168,170,187,13,31,246,36,51,111,55,124,204,78,48,213,3,164,148,86,84,181,137,43,255,48,151,69,100,116,147,69,163,234,1,98,140,11,68,180,215,114,54,39,152,249,74,203,121,52,
138,137,30,32,198,184,66,68,109,92,249,135,249,90,68,214,58,200,167,19,70,209,3,136,72,62,224,251,185,203,60,137,232,201,16,194,213,46,243,156,149,81,247,0,34,242,32,128,235,93,231,171,170,87,82,74,207,
117,157,111,211,12,90,0,17,121,10,192,247,211,230,246,219,70,85,63,19,145,151,251,200,187,41,238,30,114,225,137,40,239,250,207,3,184,90,99,154,247,23,0,79,171,234,59,21,233,222,36,162,143,1,28,175,72,
119,179,72,115,44,198,72,89,150,13,242,158,217,84,64,136,136,220,11,224,247,138,100,79,48,243,181,142,138,212,9,30,16,114,155,82,225,11,76,157,19,147,17,65,206,109,92,0,227,184,0,198,113,1,140,227,2,24,
199,5,48,142,11,96,28,23,192,56,46,128,113,92,0,227,184,0,198,113,1,140,227,2,24,199,154,0,117,158,111,223,234,160,28,115,67,105,64,72,140,241,72,17,104,49,248,147,146,101,217,62,128,229,26,73,31,0,112,
45,198,88,235,61,194,57,39,111,191,253,178,246,43,125,62,30,99,124,21,192,91,35,56,17,150,57,159,101,217,219,211,234,239,99,0,227,184,0,198,113,1,140,227,2,24,199,5,48,142,11,96,28,23,192,56,85,111,6,
125,14,32,127,75,38,89,63,81,3,37,20,109,56,21,211,107,5,91,193,223,12,114,166,226,2,24,199,5,48,142,11,96,28,23,192,56,46,128,113,92,0,227,184,0,198,113,1,140,227,2,24,199,5,48,78,233,195,160,124,225,
229,124,149,108,0,55,172,159,168,129,146,47,163,119,185,88,34,111,34,165,2,168,234,75,30,21,60,120,242,117,20,61,42,216,153,140,11,96,28,23,192,56,46,128,113,92,0,227,184,0,198,113,1,140,83,58,15,64,68,
31,2,248,166,161,253,247,156,238,201,95,117,191,80,218,198,101,31,122,84,240,56,240,168,96,103,42,46,128,113,92,0,227,184,0,198,113,1,140,227,2,24,199,5,48,142,11,96,28,23,192,56,46,128,113,92,0,227,148,
62,12,18,145,71,0,188,80,60,16,114,134,199,99,0,62,2,240,237,180,146,87,173,17,244,60,128,55,188,225,7,205,111,30,21,236,76,197,5,48,142,11,96,28,23,192,56,46,128,113,92,0,227,84,221,6,254,121,232,231,
144,209,66,248,99,21,117,248,171,248,87,26,47,57,16,238,65,69,219,85,9,240,46,128,247,106,238,182,53,215,48,243,45,17,89,5,240,83,69,57,207,50,243,166,136,140,161,119,164,170,13,191,74,5,96,230,188,225,
111,54,94,172,254,248,163,70,206,123,69,221,77,108,31,103,109,12,80,167,91,55,117,78,124,16,104,28,23,192,56,46,128,113,92,0,227,184,0,198,113,1,140,227,2,24,199,5,48,142,11,96,28,23,192,56,46,128,113,
92,0,227,184,0,198,177,38,64,157,184,6,19,143,129,15,24,244,42,97,34,242,34,128,245,252,87,0,71,203,210,18,209,142,170,158,0,240,122,197,97,223,39,162,79,84,245,120,69,186,60,78,226,33,34,90,82,213,215,
152,121,110,227,38,202,86,9,171,138,8,154,119,182,1,124,80,167,140,119,32,243,57,85,61,119,7,245,126,101,158,27,191,138,193,175,19,24,99,92,37,162,239,138,248,183,174,121,150,153,191,232,247,12,84,83,
214,3,140,98,161,200,24,227,253,68,116,189,227,108,79,50,243,118,199,121,206,196,232,23,138,204,178,236,71,85,93,172,138,128,109,144,83,67,105,252,42,70,181,84,108,74,41,31,144,237,180,156,77,222,248,
151,90,206,163,81,204,44,21,27,66,216,205,71,229,0,246,91,202,98,109,104,141,95,197,232,230,1,66,8,123,68,84,117,11,55,11,204,204,177,253,26,116,203,40,39,130,66,8,249,215,192,114,241,134,79,19,172,51,
115,234,183,86,237,48,218,153,64,102,222,5,208,68,79,144,55,254,86,3,199,153,75,70,61,21,204,204,249,142,153,43,179,190,221,164,170,167,199,220,248,176,178,97,132,136,44,151,109,159,58,9,85,93,203,178,
108,20,223,249,163,159,8,170,131,136,44,28,188,247,87,5,17,173,135,16,70,115,229,251,142,33,255,124,29,252,90,108,166,92,10,17,109,140,169,241,171,48,245,56,184,24,19,44,77,251,156,136,78,135,16,46,118,
91,170,126,49,23,16,194,204,123,170,186,58,225,163,51,33,132,205,30,138,212,43,38,35,130,178,44,187,161,170,249,192,240,128,51,204,252,101,191,165,234,7,211,219,198,21,75,225,62,60,132,71,186,255,135,
178,65,160,227,56,142,99,19,0,127,3,14,252,112,244,209,177,24,49,0,0,0,0,73,69,78,68,174,66,96,130,0,0};

const char* VASTHeaderComponent::move_grabber_png = (const char*) resource_VASTHeaderComponent_move_grabber_png;
const int VASTHeaderComponent::move_grabber_pngSize = 1465;


//[EndFile] You can add extra defines here...
//[/EndFile]

