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
#include "VASTWaveTableEditorView.h"
#include "VASTWaveTableEditorViewHeader.h"
#include <thread>
#include <fstream>
#include <iostream>
//[/Headers]

#include "VASTWaveTableEditorComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
VASTWaveTableEditorComponent::VASTWaveTableEditorComponent (AudioProcessorEditor *editor, AudioProcessor* processor)
    : myEditor((VASTAudioProcessorEditor*)editor), myProcessor((VASTAudioProcessor*)processor)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    c_concertinaEditor.reset (new VASTConcertinaPanel (myProcessor, true));
    addAndMakeVisible (c_concertinaEditor.get());
    c_concertinaEditor->setName ("c_concertinaEditor");


    //[UserPreSize]
	std::atomic_store(&m_copypaste_wavetable, std::shared_ptr<CVASTWaveTable>{}); //thread safe nullptr https://stackoverflow.com/questions/30117975/is-thread-safe-to-assign-a-shared-ptr-nullptr
	std::shared_ptr<CVASTWaveTable> curWT(new CVASTWaveTable(myProcessor->m_pVASTXperience.m_Set));
	curWT->addPosition();
	curWT->clearMultiSelect();
	setCurWavetable(curWT);
	this->setComponentID("WTEditor");

	c_samplerEditorComponent.reset(new VASTSamplerEditorComponent(myEditor, myProcessor, this));
	c_samplerEditorComponent->setName("c_samplerEditorComponent");

	c_waveTableEditorView.reset(new VASTWaveTableEditorView(myEditor, myProcessor, this));
	c_waveTableEditorView->setName("c_waveTableEditorView");

	c_concertinaEditor->addVASTPanels(c_waveTableEditorView.get(), c_samplerEditorComponent.get(), 20000, 20000, 0.8f, true, true, true);

	m_freqviewport = (VASTFreqDomainViewport*)c_waveTableEditorView->getViewportFreqDomain()->getViewedComponent();
	m_freqviewport->setProcessor(myProcessor);
	m_freqviewport->setEditor(myEditor);
	m_freqviewport->setWTEditor(this);

	m_positionviewport = (VASTPositionViewport*)c_waveTableEditorView->getViewportPositions()->getViewedComponent();
	m_positionviewport->setProcessor(myProcessor);
	m_positionviewport->setEditor(myEditor);
	m_positionviewport->setWTEditor(this);

	m_samplerviewport = (VASTSamplerViewport*)c_samplerEditorComponent->getSamplerViewport()->getViewedComponent();
	m_samplerviewport->setProcessor(myProcessor);
	m_samplerviewport->setEditor(myEditor);
	m_samplerviewport->setWTEditor(this);

	m_freqviewport->setZoomFactor(c_waveTableEditorView->getHeader()->getWTEditorZoom()->getValue());
	m_positionviewport->setZoomFactor(c_waveTableEditorView->getHeader()->getWTEditorZoom()->getValue());
	m_samplerviewport->setZoomFactor(c_samplerEditorComponent->getHeader()->getWAVScale()->getValue());


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
	setOpaque(true);
	startTimer(0, 50); //0 is update timer

	setWantsKeyboardFocus(false);
	mFileChoserIsOpen = false;
    
    //[/UserPreSize]

    setSize (800, 575);


    //[Constructor] You can add your own custom stuff here..
	//setLookAndFeel(myEditor->getCurrentVASTLookAndFeel());
	c_waveTableEditorView->getHeader()->getOscAButton()->setToggleState(true, juce::NotificationType::sendNotification);
    //[/Constructor]
}

VASTWaveTableEditorComponent::~VASTWaveTableEditorComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    VDBG("Destructing VASTWaveTableEditorComponent");
	m_cur_wavetable.reset();
	m_copypaste_wavetable.reset();
	newWTToUpdate.reset();

    this->setLookAndFeel(nullptr);
    if (isTimerRunning(0))
        stopTimer(0);
    if (isTimerRunning(1))
        stopTimer(1);
    if (isTimerRunning(2))
        stopTimer(2);
    //[/Destructor_pre]

    c_concertinaEditor = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
     VDBG("Destructing VASTWaveTableEditorComponent");
    //[/Destructor]
}

//==============================================================================
void VASTWaveTableEditorComponent::paint (juce::Graphics& g)
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

void VASTWaveTableEditorComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    c_concertinaEditor->setBounds (0, 0, proportionOfWidth (1.0000f), proportionOfHeight (1.0000f));
    //[UserResized] Add your own custom resize handling here..

	m_freqviewport->setZoomFactor(c_waveTableEditorView->getHeader()->getWTEditorZoom()->getValue());
	m_positionviewport->setZoomFactor(c_waveTableEditorView->getHeader()->getWTEditorZoom()->getValue());
	c_waveTableEditorView->getHeader()->getWTEditorZoom()->setTextBoxStyle(Slider::TextBoxLeft, true, c_waveTableEditorView->getHeader()->getWTEditorZoom()->getWidth() * 0.45f, c_waveTableEditorView->getHeader()->getWTEditorZoom()->getHeight());

	c_waveTableEditorView->getHeader()->getWTNameBox()->applyFontToAllText(myEditor->getCurrentVASTLookAndFeel()->getTextEditorFont(*c_waveTableEditorView->getHeader()->getWTNameBox()));

	m_samplerviewport->setZoomFactor(c_samplerEditorComponent->getHeader()->getWAVScale()->getValue());
	c_samplerEditorComponent->getHeader()->getWAVWTPosSlider()->setTextBoxStyle(Slider::TextBoxLeft, true, c_samplerEditorComponent->getHeader()->getWAVWTPosSlider()->getWidth() * 0.45f, c_samplerEditorComponent->getHeader()->getWAVWTPosSlider()->getHeight());
	c_samplerEditorComponent->getHeader()->getWAVScale()->setTextBoxStyle(Slider::TextBoxLeft, true, c_samplerEditorComponent->getHeader()->getWAVScale()->getWidth() * 0.45f, c_samplerEditorComponent->getHeader()->getWAVScale()->getHeight());
	c_samplerEditorComponent->getHeader()->getWAVNameBox()->applyFontToAllText(myEditor->getCurrentVASTLookAndFeel()->getTextEditorFont(*c_samplerEditorComponent->getHeader()->getWAVNameBox()));
    //[/UserResized]
}

void VASTWaveTableEditorComponent::visibilityChanged()
{
    //[UserCode_visibilityChanged] -- Add your code here...
	if (this->isVisible()) { //skip on close
		m_freqviewport->setZoomFactor(c_waveTableEditorView->getHeader()->getWTEditorZoom()->getValue());
		m_positionviewport->setZoomFactor(c_waveTableEditorView->getHeader()->getWTEditorZoom()->getValue());
		m_samplerviewport->setZoomFactor(c_samplerEditorComponent->getHeader()->getWAVScale()->getValue());
		c_waveTableEditorView->getEditorOscilloscope()->selectAll(true); //noUIUpdate
		notifySelectionChanged();
		requestUIUpdate(); //was true
	}
    //[/UserCode_visibilityChanged]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...




void VASTWaveTableEditorComponent::sliderValueChanged(Slider* sliderThatWasMoved)
{
	if (c_waveTableEditorView != nullptr)
		if (c_waveTableEditorView->getHeader() != nullptr) {

			if (sliderThatWasMoved == c_waveTableEditorView->getHeader()->getPhaseSlider())
			{
				if (sliderThatWasLastMoved != sliderThatWasMoved) copySelectionToLocalBuffer();
				sSelection* sel = c_waveTableEditorView->getEditorOscilloscope()->getSelection();
				if (sel->iWTSelectionStart != sel->iWTSelectionEnd) {
					std::thread edit_thread(&VASTWaveTableEditorComponent::threadedEditorFunction, EditorFunction::Phase, c_waveTableEditorView->getHeader()->c_Phase->getValue(), /*msbeg, msend,*/ sel->iWTSelectionStart, sel->iWTSelectionEnd, this, 0.f, 0.f, nullptr, true);
					edit_thread.detach();
				}
			}
			else if (sliderThatWasMoved == c_waveTableEditorView->getHeader()->getFreqSlider())
			{
				if (sliderThatWasLastMoved != sliderThatWasMoved)
					copySelectionToLocalBuffer();
				sSelection* sel = c_waveTableEditorView->getEditorOscilloscope()->getSelection();
				if (sel->iWTSelectionStart != sel->iWTSelectionEnd) {
					std::thread edit_thread(&VASTWaveTableEditorComponent::threadedEditorFunction, EditorFunction::Freq, c_waveTableEditorView->getHeader()->c_Freq->getValue(), /*msbeg, msend,*/ sel->iWTSelectionStart, sel->iWTSelectionEnd, this, 0.f, 0.f, nullptr, true);
					edit_thread.detach();
				}
			}
			else if (sliderThatWasMoved == c_waveTableEditorView->getHeader()->getOffsetSlider())
			{
				if (sliderThatWasLastMoved != sliderThatWasMoved) copySelectionToLocalBuffer();
				sSelection* sel = c_waveTableEditorView->getEditorOscilloscope()->getSelection();
				if (sel->iWTSelectionStart != sel->iWTSelectionEnd) {
					std::thread edit_thread(&VASTWaveTableEditorComponent::threadedEditorFunction, EditorFunction::Offset, c_waveTableEditorView->getHeader()->c_Offset->getValue(), /*msbeg, msend,*/ sel->iWTSelectionStart, sel->iWTSelectionEnd, this, 0.f, 0.f, nullptr, true);
					edit_thread.detach();
				}

			}
			else if (sliderThatWasMoved == c_waveTableEditorView->getHeader()->getBitDepthSlider())
			{
				if (sliderThatWasLastMoved != sliderThatWasMoved)
					copySelectionToLocalBuffer();
				sSelection* sel = c_waveTableEditorView->getEditorOscilloscope()->getSelection();
				if (sel->iWTSelectionStart != sel->iWTSelectionEnd) {
					std::thread edit_thread(&VASTWaveTableEditorComponent::threadedEditorFunction, EditorFunction::BitDepth, c_waveTableEditorView->getHeader()->c_bitDepth->getValue(), /*msbeg, msend,*/ sel->iWTSelectionStart, sel->iWTSelectionEnd, this, 0.f, 0.f, nullptr, true);
					edit_thread.detach();
				}
			}
			else if (sliderThatWasMoved == c_waveTableEditorView->getHeader()->getSmoothSlider())
			{
				if (sliderThatWasLastMoved != sliderThatWasMoved) copySelectionToLocalBuffer();
				sSelection* sel = c_waveTableEditorView->getEditorOscilloscope()->getSelection();
				if (sel->iWTSelectionStart != sel->iWTSelectionEnd) {
					std::thread edit_thread(&VASTWaveTableEditorComponent::threadedEditorFunction, EditorFunction::Smooth, c_waveTableEditorView->getHeader()->c_Smooth->getValue(), /*msbeg, msend,*/ sel->iWTSelectionStart, sel->iWTSelectionEnd, this, 0.f, 0.f, nullptr, true);
					edit_thread.detach();
				}
			}
			else if (sliderThatWasMoved == c_waveTableEditorView->getHeader()->getCleanSlider())
			{
				if (sliderThatWasLastMoved != sliderThatWasMoved) copySelectionToLocalBuffer();
				sSelection* sel = c_waveTableEditorView->getEditorOscilloscope()->getSelection();
				if (sel->iWTSelectionStart != sel->iWTSelectionEnd) {
					std::thread edit_thread(&VASTWaveTableEditorComponent::threadedEditorFunction, EditorFunction::Clean, c_waveTableEditorView->getHeader()->c_Clean->getValue(), /*msbeg, msend,*/ sel->iWTSelectionStart, sel->iWTSelectionEnd, this, 0.f, 0.f, nullptr, true);
					edit_thread.detach();
				}
			}
			else if (sliderThatWasMoved == c_waveTableEditorView->getHeader()->getWTEditorZoom())
			{
				m_freqviewport->setZoomFactor(c_waveTableEditorView->getHeader()->m_iWTEditorZoom.get()->getValue());
				m_positionviewport->setZoomFactor(c_waveTableEditorView->getHeader()->m_iWTEditorZoom.get()->getValue());
				m_samplerviewport->setZoomFactor(c_samplerEditorComponent->getHeader()->m_iWAVScale.get()->getValue());
				return; //dont update UI
			}
			else if (sliderThatWasMoved == c_waveTableEditorView->getHeader()->getFreqModSlider())
			{
				if (sliderThatWasLastMoved != sliderThatWasMoved) copySelectionToLocalBuffer();
				sSelection* sel = c_waveTableEditorView->getEditorOscilloscope()->getSelection();
				if (sel->iWTSelectionStart != sel->iWTSelectionEnd) {
					std::thread edit_thread(&VASTWaveTableEditorComponent::threadedEditorFunction, EditorFunction::FreqMod, c_waveTableEditorView->getHeader()->c_FreqMod->getValue(), /*msbeg, msend,*/ sel->iWTSelectionStart, sel->iWTSelectionEnd, this, 0.f, 0.f, nullptr, true);
					edit_thread.detach();
				}
			}
			else if (sliderThatWasMoved == c_waveTableEditorView->getHeader()->getApplyMaxSlider())
			{
			}
			else if (sliderThatWasMoved == c_waveTableEditorView->getHeader()->getBendSlider())
			{
				//y = (1 - x) ^ 2 * startval + 2x(1 - x) * intermediateval + x ^ 2 * endval


				if (sliderThatWasLastMoved != sliderThatWasMoved)
					copySelectionToLocalBuffer();
				sSelection* sel = c_waveTableEditorView->getEditorOscilloscope()->getSelection();
				if (sel->iWTSelectionStart != sel->iWTSelectionEnd) {
					std::thread edit_thread(&VASTWaveTableEditorComponent::threadedEditorFunction, EditorFunction::Bend, c_waveTableEditorView->getHeader()->c_Bend->getValue() * 0.999f, /*msbeg, msend,*/ sel->iWTSelectionStart, sel->iWTSelectionEnd, this, 0.f, 0.f, nullptr, true);
					edit_thread.detach();
				}

			}
			else if (sliderThatWasMoved == c_waveTableEditorView->getHeader()->getBloatSlider())
			{
				if (sliderThatWasLastMoved != sliderThatWasMoved)
					copySelectionToLocalBuffer();
				sSelection* sel = c_waveTableEditorView->getEditorOscilloscope()->getSelection();
				if (sel->iWTSelectionStart != sel->iWTSelectionEnd) {
					std::thread edit_thread(&VASTWaveTableEditorComponent::threadedEditorFunction, EditorFunction::Bloat, c_waveTableEditorView->getHeader()->c_Bloat->getValue() * 0.999f, /*msbeg, msend,*/ sel->iWTSelectionStart, sel->iWTSelectionEnd, this, 0.f, 0.f, nullptr, true);
					edit_thread.detach();
				}

			}
			else if (sliderThatWasMoved == c_waveTableEditorView->getHeader()->getLevelSlider())
			{
				if (sliderThatWasLastMoved != sliderThatWasMoved) copySelectionToLocalBuffer();
				sSelection* sel = c_waveTableEditorView->getEditorOscilloscope()->getSelection();
				if (sel->iWTSelectionStart != sel->iWTSelectionEnd) {
					std::thread edit_thread(&VASTWaveTableEditorComponent::threadedEditorFunction, EditorFunction::Level, c_waveTableEditorView->getHeader()->c_Level->getValue(), /*msbeg, msend,*/ sel->iWTSelectionStart, sel->iWTSelectionEnd, this, 0.f, 0.f, nullptr, true);
					edit_thread.detach();
				}
			}
			else if (sliderThatWasMoved == c_waveTableEditorView->getHeader()->getConnectSlider())
			{
				if (sliderThatWasLastMoved != sliderThatWasMoved)
					copySelectionToLocalBuffer();
				sSelection* sel = c_waveTableEditorView->getEditorOscilloscope()->getSelection();
				if (sel->iWTSelectionStart != sel->iWTSelectionEnd) {
					std::thread edit_thread(&VASTWaveTableEditorComponent::threadedEditorFunction, EditorFunction::Connect, c_waveTableEditorView->getHeader()->c_Connect->getValue(), /*msbeg, msend,*/ sel->iWTSelectionStart, sel->iWTSelectionEnd, this, 0.f, 0.f, nullptr, true);
					edit_thread.detach();
				}
			}
		}

	if (c_samplerEditorComponent != nullptr)
		if (c_samplerEditorComponent->getHeader() != nullptr) {

			if (sliderThatWasMoved == c_samplerEditorComponent->getHeader()->m_iWAVScale.get())
			{
				m_samplerviewport->setZoomFactor(c_samplerEditorComponent->getHeader()->m_iWAVScale.get()->getValue());
				return; //dont update UI
			}
			else if (sliderThatWasMoved == c_samplerEditorComponent->getHeader()->getWAVWTPosSlider())
			{
			}
		}


	if (sliderThatWasLastMoved == nullptr)
		initSliders();

	sliderThatWasLastMoved = sliderThatWasMoved;

	//m_needsUIUpdate = true; //50ms delay // timer needed?
	if (m_freqviewport != nullptr)
		m_freqviewport->updateContentAsync();
	if (m_positionviewport != nullptr)
		m_positionviewport->updateContentAsync();
	if (c_waveTableEditorView != nullptr)
		if (c_waveTableEditorView->getEditorOscilloscope() != nullptr)
			c_waveTableEditorView->getEditorOscilloscope()->updateContentAsync(); //cannot be async
	if (m_samplerviewport != nullptr)
		m_samplerviewport->updateContentAsync();

}

void VASTWaveTableEditorComponent::randomizeSample() {
	struct timeval tp;
	myProcessor->m_pVASTXperience.m_Set._gettimeofday(&tp);
	juce::Random rand(tp.tv_sec); //seed

	Array<File> l_sampleFiles;
	FileSearchPath sPath = FileSearchPath(myProcessor->m_UserWavRootFolder);
	sPath.findChildFiles(l_sampleFiles, File::findFiles, true, "*.wav;*.aiff");
	if (l_sampleFiles.size() != 0) {
		File loadWAVFile = l_sampleFiles[abs(rand.nextInt()) % l_sampleFiles.size()];
		VASTSamplerSound* lVASTSamplerSound = loadWavFile(loadWAVFile.getFullPathName());
		if (lVASTSamplerSound != nullptr) {
			VASTSynthesiserSound* lSynthSound = (VASTSynthesiserSound*)(myProcessor->m_pVASTXperience.m_Poly.getSynthesizer()->getSound(0));
			lSynthSound->addSamplerSoundChanged(lVASTSamplerSound);
		}
	}
}

void VASTWaveTableEditorComponent::randomizeBankWavetable(int bank, bool analog, bool digital) {
	setOscBank(bank);
	if (!myProcessor->m_pVASTXperience.audioProcessLock()) {
		myProcessor->setErrorState(myProcessor->vastErrorState::errorState18_prepareToPlayFailed);
	}

	struct timeval tp;
	myProcessor->m_pVASTXperience.m_Set._gettimeofday(&tp);
	juce::Random rand(tp.tv_sec); //seed
	int l_numCycles = abs(rand.nextInt()) % 255;

	if (analog || digital) {
		Array<File> l_presetWTFiles;
		FileSearchPath sPath = FileSearchPath(myProcessor->m_UserWavetableRootFolder);
		String searchMask = "";
		if (analog)
			searchMask = "*analog*.wav;*analog*.aiff";
		else 
			if (digital)
				searchMask = "*digital*.wav;*digital*.aiff";
		sPath.findChildFiles(l_presetWTFiles, File::findFiles, true, searchMask);
		if (l_presetWTFiles.size() != 0) {
			File loadWTFile = l_presetWTFiles[abs(rand.nextInt()) % l_presetWTFiles.size()];
			loadWTFileThread(loadWTFile, this);
			myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_bank].addSoftFadeEditor();
			myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_bank].setWavetableSoftFade(newWTToUpdate);
			myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_bank].removeSoftFadeEditor();
			myProcessor->m_pVASTXperience.beginSoftFade();
			myProcessor->m_pVASTXperience.endSoftFade();
		}
	}
	else {
		sSelection* sel = c_waveTableEditorView->getEditorOscilloscope()->getSelection();

		myProcessor->m_pVASTXperience.m_Poly.m_OscBank[bank].recalcWavetable();
		std::shared_ptr<CVASTWaveTable> wavetable = std::make_shared<CVASTWaveTable>(myProcessor->m_pVASTXperience.m_Set);
		wavetable->addPosition();
		if (bank == 0) {
			//init bank A to saw
			std::vector<myFloat>* samples = wavetable->getNaiveTable(0);
			wavetable->getNaiveSamplesFromWave(*samples, abs(rand.nextInt()) % 7);
			wavetable->setWaveTableName(TRANS("Random"));
			wavetable->setNaiveTableFast(0, false, myProcessor->getWTmode());
		}
		myProcessor->m_pVASTXperience.m_Poly.m_OscBank[bank].setWavetableSoftFade(wavetable);
		myProcessor->m_pVASTXperience.beginSoftFade();
		myProcessor->m_pVASTXperience.endSoftFade();

		setCopyPasteWavetable(getBankWavetable()); //set copy paste WT for convolution etc

		startWTRecording();
		int counter = 0;
		while (counter < l_numCycles) {
			int l_editorFunction = abs(rand.nextInt()) % 48;
			float l_sliderValue = rand.nextFloat() * 100.f; //check ranges
			float l_connectVal1 = rand.nextFloat() * 2.f - 1.f;
			float l_connectVal2 = rand.nextFloat() * 2.f - 1.f;
			threadedEditorFunction(l_editorFunction, l_sliderValue, sel->iWTSelectionStart, sel->iWTSelectionEnd, this, l_connectVal1, l_connectVal2, nullptr, false); //do not pregenerate
			myProcessor->m_pVASTXperience.beginSoftFade();
			myProcessor->m_pVASTXperience.endSoftFade();
			counter++;
		}
		stopWTRecording();
	}

	normalizePositions(0, getBankWavetable()->getNumPositions()-1, getBankWavetable().get(), getBankWavetable(), false, myProcessor->getWTmode());

	if (!myProcessor->m_pVASTXperience.audioProcessUnlock()) {
		myProcessor->setErrorState(myProcessor->vastErrorState::errorState18_prepareToPlayFailed);
	}
}

void VASTWaveTableEditorComponent::setOscBank(int bank) {
	m_bank = bank;
	getBankWavetable()->setSelectedWtPos(0);
	std::shared_ptr<CVASTWaveTable> wt = getBankWavetable();
	vassert(wt->getNumPositions() > 0); //CHECK SAFETY
	setCurWavetable(wt);
	getBankWavetable()->setSelectedWtPos(0);
	c_waveTableEditorView->getHeader()->c_soloSelection->setToggleState(false, NotificationType::sendNotification);
	myEditor->vaporizerComponent->getOscillatorComponent(0)->updateAll(); //for borders
	myEditor->vaporizerComponent->getOscillatorComponent(1)->updateAll(); //for borders
	myEditor->vaporizerComponent->getOscillatorComponent(2)->updateAll(); //for borders
	myEditor->vaporizerComponent->getOscillatorComponent(3)->updateAll(); //for borders
	notifySelectionChanged();
}

void VASTWaveTableEditorComponent::buttonClicked(Button* buttonThatWasClicked)
{
	if (c_waveTableEditorView != nullptr)
		if (c_waveTableEditorView->getHeader() != nullptr) {
			if (buttonThatWasClicked == c_waveTableEditorView->getHeader()->c_oscA.get())
			{
				stopWTRecording();
				setOscBank(0);
			}
			else if (buttonThatWasClicked == c_waveTableEditorView->getHeader()->c_oscB.get())
			{
				stopWTRecording();
				setOscBank(1);
			}
			else if (buttonThatWasClicked == c_waveTableEditorView->getHeader()->c_oscC.get())
			{
				stopWTRecording();
				setOscBank(2);
			}
			else if (buttonThatWasClicked == c_waveTableEditorView->getHeader()->c_oscD.get())
			{
				stopWTRecording();
				setOscBank(3);
			}
			else if (buttonThatWasClicked == c_waveTableEditorView->getHeader()->c_soloSelection.get())
			{
				stopWTRecording();
				myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_bank].setSoloMode(c_waveTableEditorView->getHeader()->c_soloSelection->getToggleState());
			}
			else if (buttonThatWasClicked == c_waveTableEditorView->getHeader()->c_snap.get())
			{
				m_bSnapToZero = c_waveTableEditorView->getHeader()->c_snap->getToggleState();
			}
			else if (buttonThatWasClicked == c_waveTableEditorView->getHeader()->c_stretch.get())
			{
				m_bStretchToZero = c_waveTableEditorView->getHeader()->c_stretch->getToggleState();
			}

			//ICONS
			else if (buttonThatWasClicked == c_waveTableEditorView->getHeader()->c_iconSave.get()) {
				stopWTRecording();
				std::shared_ptr<CVASTWaveTable> wt = getBankWavetable();
				String m_UserWTTablesFolder = myProcessor->m_UserWavetableRootFolder;
				File initialFile = File(m_UserWTTablesFolder);
				//FileChooser myChooser(TRANS("Please specify name and location of the WAV wavetable file you want to save ..."), initialFile, "*.wav", true, false, myEditor->vaporizerComponent);
				//mFileChoserIsOpen = true;
				//if (myChooser.browseForFileToSave(true))
				//{
					//File wtFile(myChooser.getResult());
				myChooser = std::make_unique<FileChooser>(TRANS("Please specify name and location of the WAV wavetable file you want to save ..."), initialFile, "*.wav");
				myChooser->launchAsync(FileBrowserComponent::saveMode | FileBrowserComponent::canSelectFiles | FileBrowserComponent::warnAboutOverwriting,
					[this, wt](const FileChooser& fileChooser) {
					File wtFile(fileChooser.getResult());
					saveWTFileThreaded(wtFile, wt.get());
					mFileChoserIsOpen = false;
				});
			}
			else if (buttonThatWasClicked == c_waveTableEditorView->getHeader()->c_iconLoadWT.get()) {
				stopWTRecording();
				String m_UserWTTablesFolder = myProcessor->m_UserWavetableRootFolder;
				File initialFile = File(m_UserWTTablesFolder);
				/*/
				FileChooser myChooser(TRANS("Please select the WAV / AIFF file you want to load as wavetable ..."), initialFile, "*.wav;*.aiff", true, false, myEditor->vaporizerComponent);
				mFileChoserIsOpen = true;
				if (myChooser.browseForFileToOpen())
				{
					File wtFile(myChooser.getResult());
					*/

				myChooser = std::make_unique<FileChooser>(TRANS("Please select the WAV / AIFF file you want to load as wavetable ..."), initialFile, "*.wav;*.aiff");
				myChooser->launchAsync(FileBrowserComponent::openMode | FileBrowserComponent::canSelectFiles,
					[this, initialFile](const FileChooser& fileChooser) {
					File wtFile(fileChooser.getResult());

					loadWTFileThreaded(wtFile.getFullPathName());
					mFileChoserIsOpen = false;
				});
			}
			else if (buttonThatWasClicked == c_waveTableEditorView->getHeader()->c_iconClearWT.get()) {
				stopWTRecording();
				myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_bank].addSoftFadeEditor();
				std::shared_ptr<CVASTWaveTable> wavetable = myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_bank].getSoftOrCopyWavetable();
				wavetable->clear();
				wavetable->addPosition(); //there shall be no wt without position
				wavetable->setSelectedWtPos(0);
				myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_bank].setWavetableSoftFade(wavetable);
				myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_bank].removeSoftFadeEditor();
				//m_positionviewport->setZoomFactor(c_waveTableEditorView->getHeader()->m_iWTEditorZoom.get()->getValue());
			}
			else if (buttonThatWasClicked == c_waveTableEditorView->getHeader()->c_iconRecordWT.get()) {
				if (m_bIsRecordingWT == false) {
					startWTRecording();
				}
				else {
					stopWTRecording();
				}
			}
			else if (buttonThatWasClicked == c_waveTableEditorView->getHeader()->c_WTableNext.get()) {
				stopWTRecording();
				std::shared_ptr<CVASTWaveTable> wavetable = getBankWavetable();
				String m_UserWTTablesFolder = myProcessor->m_UserWavetableRootFolder;
				File initialFile = File(m_UserWTTablesFolder);
				//find out index
				Array<File> foundFiles = initialFile.findChildFiles(File::TypesOfFileToFind::findFiles, true, "*.wav;*.aiff"); 
				if (foundFiles.size() == 0)
					return;
                foundFiles.sort();
				int dirindex = 0;
				bool lFound = false;
				for (int i=0; i< foundFiles.size(); i++) {
					if (foundFiles[i].getFileName().equalsIgnoreCase(wavetable->getWaveTableName())) {
						lFound = true;
						dirindex = i + 1;
						break;
					}
				}
				if (!lFound) 
					dirindex ++;				
				loadWTFileThreaded(foundFiles[dirindex % foundFiles.size()].getFullPathName());
			}
			else if (buttonThatWasClicked == c_waveTableEditorView->getHeader()->c_WTablePrevious.get()) {
				stopWTRecording();
				std::shared_ptr<CVASTWaveTable> wavetable = getBankWavetable();
				String m_UserWTTablesFolder = myProcessor->m_UserWavetableRootFolder;
				File initialFile = File(m_UserWTTablesFolder);
				//find out index
				Array<File> foundFiles = initialFile.findChildFiles(File::TypesOfFileToFind::findFiles, true, "*.wav;*.aiff");
				if (foundFiles.size() == 0)
					return;
                foundFiles.sort();
				int dirindex = foundFiles.size();
				bool lFound = false;
				for (int i = 1; i < foundFiles.size(); i++) {
					if (foundFiles[i].getFileName().equalsIgnoreCase(wavetable->getWaveTableName())) {
						lFound = true;
						dirindex = i - 1;
						break;
					}
				}
				if (!lFound) {
					if (dirindex > 0)
						dirindex--;
				}
				loadWTFileThreaded(foundFiles[dirindex].getFullPathName());
			}
			else if (buttonThatWasClicked == c_waveTableEditorView->getHeader()->c_iconSelectAll.get()) {
				c_waveTableEditorView->getEditorOscilloscope()->selectAll(false); //UI update later
				multiSelectAll();
				requestUIUpdate();
			}
			else if (buttonThatWasClicked == c_waveTableEditorView->getHeader()->c_iconAddPos.get()) { //add position after selected
				stopWTRecording();
				myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_bank].addSoftFadeEditor();
				std::shared_ptr<CVASTWaveTable> wavetable = myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_bank].getSoftOrCopyWavetable();
				int curPos = wavetable->getSelectedWtPos();
				if (wavetable->isMultiSelected()) {
					curPos = wavetable->getMultiSelectEnd();
				}
				wavetable->insertEmptyPosition(curPos);
				wavetable->setSelectedWtPos(curPos + 1);
				myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_bank].setWavetableSoftFade(wavetable);
				myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_bank].removeSoftFadeEditor();
				//m_positionviewport->setZoomFactor(c_waveTableEditorView->getHeader()->m_iWTEditorZoom.get()->getValue());
			}
			else if (buttonThatWasClicked == c_waveTableEditorView->getHeader()->c_iconCopy.get()) {
				copyPositions();
			}
			else if (buttonThatWasClicked == c_waveTableEditorView->getHeader()->c_iconPaste.get()) {
				insertCutCopiedPositions(false, false);
			}
			else if (buttonThatWasClicked == c_waveTableEditorView->getHeader()->c_iconDuplicate.get()) {
				stopWTRecording();
				myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_bank].addSoftFadeEditor();
				std::shared_ptr<CVASTWaveTable> wavetable = myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_bank].getSoftOrCopyWavetable();
				if (wavetable->isMultiSelected()) {
					int beg = wavetable->getMultiSelectBegin();
					int end = wavetable->getMultiSelectEnd();
					for (int wtPos = end; wtPos >= beg; wtPos--) {
						if (wavetable->getNumPositions() >= C_MAX_NUM_POSITIONS) break;
						wavetable->duplicatePosition(wtPos, end + 1);
					}
				}
				else {
					int curPos = wavetable->getSelectedWtPos();
					if (!(wavetable->getNumPositions() >= C_MAX_NUM_POSITIONS))
						wavetable->duplicatePosition(curPos, curPos + 1);
				}
				myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_bank].setWavetableSoftFade(wavetable);
				myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_bank].removeSoftFadeEditor();
				//m_positionviewport->setZoomFactor(c_waveTableEditorView->getHeader()->m_iWTEditorZoom.get()->getValue());
			}
			else if (buttonThatWasClicked == c_waveTableEditorView->getHeader()->c_iconThinOut.get()) {
				deletePositions(true);
			}
			else if (buttonThatWasClicked == c_waveTableEditorView->getHeader()->c_iconDelPos.get()) {
				deletePositions(false);
			}
			else if (buttonThatWasClicked == c_waveTableEditorView->getHeader()->c_iconNormalize.get()) {
				//sSelection* sel = c_editorOscilloscope->getSelection();
				std::thread edit_thread(&VASTWaveTableEditorComponent::threadedEditorFunction, EditorFunction::Normalize, 0.f, /*msbeg, msend,*/ 0, 0, this, 0.f, 0.f, nullptr, true);
				edit_thread.detach();
			}
			//========================================================================================================
			// Waveform quick select buttons
			//========================================================================================================
			else if (buttonThatWasClicked == c_waveTableEditorView->getHeader()->c_iconSine.get()) {
				sSelection* sel = c_waveTableEditorView->getEditorOscilloscope()->getSelection();
				if (sel->iWTSelectionStart != sel->iWTSelectionEnd) {
					std::thread edit_thread(&VASTWaveTableEditorComponent::threadedEditorFunction, EditorFunction::Sine, 0, /*msbeg, msend,*/ sel->iWTSelectionStart, sel->iWTSelectionEnd, this, 0.f, 0.f, nullptr, true);
					edit_thread.detach();
				}
			}
			else if (buttonThatWasClicked == c_waveTableEditorView->getHeader()->c_iconHalfSine.get()) {
				sSelection* sel = c_waveTableEditorView->getEditorOscilloscope()->getSelection();
				if (sel->iWTSelectionStart != sel->iWTSelectionEnd) {
					std::thread edit_thread(&VASTWaveTableEditorComponent::threadedEditorFunction, EditorFunction::HalfSine, 0, /*msbeg, msend,*/ sel->iWTSelectionStart, sel->iWTSelectionEnd, this, 0.f, 0.f, nullptr, true);
					edit_thread.detach();
				}
			}
			else if (buttonThatWasClicked == c_waveTableEditorView->getHeader()->c_iconTriangle.get()) {
				sSelection* sel = c_waveTableEditorView->getEditorOscilloscope()->getSelection();
				if (sel->iWTSelectionStart != sel->iWTSelectionEnd) {
					std::thread edit_thread(&VASTWaveTableEditorComponent::threadedEditorFunction, EditorFunction::Triangle, 0, /*msbeg, msend,*/ sel->iWTSelectionStart, sel->iWTSelectionEnd, this, 0.f, 0.f, nullptr, true);
					edit_thread.detach();
				}
			}
			else if (buttonThatWasClicked == c_waveTableEditorView->getHeader()->c_iconZeros.get()) {
				sSelection* sel = c_waveTableEditorView->getEditorOscilloscope()->getSelection();
				if (sel->iWTSelectionStart != sel->iWTSelectionEnd) {
					std::thread edit_thread(&VASTWaveTableEditorComponent::threadedEditorFunction, EditorFunction::Zeros, 0, /*msbeg, msend,*/ sel->iWTSelectionStart, sel->iWTSelectionEnd, this, 0.f, 0.f, nullptr, true);
					edit_thread.detach();
				}
			}
			else if (buttonThatWasClicked == c_waveTableEditorView->getHeader()->c_iconSaw.get()) {
				sSelection* sel = c_waveTableEditorView->getEditorOscilloscope()->getSelection();
				if (sel->iWTSelectionStart != sel->iWTSelectionEnd) {
					std::thread edit_thread(&VASTWaveTableEditorComponent::threadedEditorFunction, EditorFunction::Sawtooth, 0, /*msbeg, msend,*/ sel->iWTSelectionStart, sel->iWTSelectionEnd, this, 0.f, 0.f, nullptr, true);
					edit_thread.detach();
				}
			}
			else if (buttonThatWasClicked == c_waveTableEditorView->getHeader()->c_iconSquare.get()) {
					sSelection* sel = c_waveTableEditorView->getEditorOscilloscope()->getSelection();
				if (sel->iWTSelectionStart != sel->iWTSelectionEnd) {
					std::thread edit_thread(&VASTWaveTableEditorComponent::threadedEditorFunction, EditorFunction::Square, 0, /*msbeg, msend,*/ sel->iWTSelectionStart, sel->iWTSelectionEnd, this, 0.f, 0.f, nullptr, true);
					edit_thread.detach();
				}
			}
			else if (buttonThatWasClicked == c_waveTableEditorView->getHeader()->c_iconRamp.get()) {
				sSelection* sel = c_waveTableEditorView->getEditorOscilloscope()->getSelection();
				if (sel->iWTSelectionStart != sel->iWTSelectionEnd) {
					std::thread edit_thread(&VASTWaveTableEditorComponent::threadedEditorFunction, EditorFunction::Ramp, 0, /*msbeg, msend,*/ sel->iWTSelectionStart, sel->iWTSelectionEnd, this, 0.f, 0.f, nullptr, true);
					edit_thread.detach();
				}
			}
			else if (buttonThatWasClicked == c_waveTableEditorView->getHeader()->c_iconRandom.get()) {
				sSelection* sel = c_waveTableEditorView->getEditorOscilloscope()->getSelection();
				if (sel->iWTSelectionStart != sel->iWTSelectionEnd) {
					std::thread edit_thread(&VASTWaveTableEditorComponent::threadedEditorFunction, EditorFunction::Random, 0, /*msbeg, msend,*/ sel->iWTSelectionStart, sel->iWTSelectionEnd, this, 0.f, 0.f, nullptr, true);
					edit_thread.detach();
				}
			}
			else if (buttonThatWasClicked == c_waveTableEditorView->getHeader()->c_iconSinc.get()) {
				sSelection* sel = c_waveTableEditorView->getEditorOscilloscope()->getSelection();
				if (sel->iWTSelectionStart != sel->iWTSelectionEnd) {
					std::thread edit_thread(&VASTWaveTableEditorComponent::threadedEditorFunction, EditorFunction::Sinc, 0, /*msbeg, msend,*/ sel->iWTSelectionStart, sel->iWTSelectionEnd, this, 0.f, 0.f, nullptr, true);
					edit_thread.detach();
				}
			}
			else if (buttonThatWasClicked == c_waveTableEditorView->getHeader()->c_iconSampleHold.get()) {
				sSelection* sel = c_waveTableEditorView->getEditorOscilloscope()->getSelection();
				if (sel->iWTSelectionStart != sel->iWTSelectionEnd) {
					std::thread edit_thread(&VASTWaveTableEditorComponent::threadedEditorFunction, EditorFunction::SampleHold, 0, /*msbeg, msend,*/ sel->iWTSelectionStart, sel->iWTSelectionEnd, this, 0.f, 0.f, nullptr, true);
					edit_thread.detach();
				}
			}
			else if (buttonThatWasClicked == c_waveTableEditorView->getHeader()->c_iconReverse.get()) {
				sSelection* sel = c_waveTableEditorView->getEditorOscilloscope()->getSelection();
				if (sel->iWTSelectionStart != sel->iWTSelectionEnd) {
					std::thread edit_thread(&VASTWaveTableEditorComponent::threadedEditorFunction, EditorFunction::Reverse, 0, /*msbeg, msend,*/ sel->iWTSelectionStart, sel->iWTSelectionEnd, this, 0.f, 0.f, nullptr, true);
					edit_thread.detach();
				}
			}
			else if (buttonThatWasClicked == c_waveTableEditorView->getHeader()->c_iconInvert.get()) {
				sSelection* sel = c_waveTableEditorView->getEditorOscilloscope()->getSelection();
				if (sel->iWTSelectionStart != sel->iWTSelectionEnd) {
					std::thread edit_thread(&VASTWaveTableEditorComponent::threadedEditorFunction, EditorFunction::Invert, 0, /*msbeg, msend,*/ sel->iWTSelectionStart, sel->iWTSelectionEnd, this, 0.f, 0.f, nullptr, true);
					edit_thread.detach();
				}
			}
			else if (buttonThatWasClicked == c_waveTableEditorView->getHeader()->c_iconArcConnect.get()) {
				sSelection* sel = c_waveTableEditorView->getEditorOscilloscope()->getSelection();
				if (sel->iWTSelectionStart != sel->iWTSelectionEnd) {
					std::thread edit_thread(&VASTWaveTableEditorComponent::threadedEditorFunction, EditorFunction::ArcConnect, 0, /*msbeg, msend,*/ sel->iWTSelectionStart, sel->iWTSelectionEnd, this, 0.f, 0.f, nullptr, true);
					edit_thread.detach();
				}
			}
			else if (buttonThatWasClicked == c_waveTableEditorView->getHeader()->c_iconRampConnect.get()) {
					sSelection* sel = c_waveTableEditorView->getEditorOscilloscope()->getSelection();
				if (sel->iWTSelectionStart != sel->iWTSelectionEnd) {
					std::thread edit_thread(&VASTWaveTableEditorComponent::threadedEditorFunction, EditorFunction::RampConnect, 0, /*msbeg, msend,*/ sel->iWTSelectionStart, sel->iWTSelectionEnd, this, 0.f, 0.f, nullptr, true);
					edit_thread.detach();
				}
			}
			else if (buttonThatWasClicked == c_waveTableEditorView->getHeader()->c_iconMaximizeEditor.get()) {
				if (!myEditor->vaporizerComponent->isTabConcertinaMaximized()) {
					myEditor->vaporizerComponent->maximizeTabConcertina();
					c_concertinaEditor->maximizePanel(0); //wt editor
				}
				else {
					myEditor->vaporizerComponent->defaultConcertina();
					c_concertinaEditor->maximizeBothPanels();
				}
			}

//==================================================================================================================================
// Wavetable modulations / apply
//==================================================================================================================================
			else if (buttonThatWasClicked == c_waveTableEditorView->getHeader()->c_iconApply.get()) {
				int lSelected = c_waveTableEditorView->getHeader()->c_applyOptions->getSelectedItemIndex();
				switch (lSelected) {
				case ApplyOption::ApplyInterpolation: {
					std::thread edit_thread(&VASTWaveTableEditorComponent::threadedEditorFunction, EditorFunction::Interpolate, c_waveTableEditorView->getHeader()->c_applyMax->getValue(), /*msbeg, msend,*/ 0, 0, this, 0.f, 0.f, nullptr, true); //modmax not used yet
					edit_thread.detach();
					break;
				}
				case ApplyOption::ApplyAmplitudeModulation: {
					std::shared_ptr<CVASTWaveTable> copypastewavetable = getCopyPasteWavetable();
					if ((copypastewavetable == nullptr) || (copypastewavetable->getNumPositions() == 0)) {
						AlertWindow::showMessageBoxAsync(MessageBoxIconType::InfoIcon, TRANS("Nothing copied"), TRANS("The wavetable copy buffer is empty. Use copy function before."), TRANS("Continue"), this);
					}
					else {
						std::thread edit_thread(&VASTWaveTableEditorComponent::threadedEditorFunction, EditorFunction::AmplModCopy, 0.f, /*msbeg, msend,*/ 0, 0, this, 0.f, 0.f, nullptr, true); //mod max not usable here
						edit_thread.detach();
					}
					break;
				}
				case ApplyOption::ApplyMixIn: {
					std::shared_ptr<CVASTWaveTable> copypastewavetable = getCopyPasteWavetable();
					if ((copypastewavetable == nullptr) || (copypastewavetable->getNumPositions() == 0)) {
						AlertWindow::showMessageBoxAsync(MessageBoxIconType::InfoIcon, TRANS("Nothing copied"), TRANS("The wavetable copy buffer is empty. Use copy function before."), TRANS("Continue"), this);
					}
					else {
						std::thread edit_thread(&VASTWaveTableEditorComponent::threadedEditorFunction, EditorFunction::MixInCopy, c_waveTableEditorView->getHeader()->c_applyMax->getValue(), /*msbeg, msend,*/ 0, 0, this, 0.f, 0.f, nullptr, true); //modmax not used yet
						edit_thread.detach();
					}
					break;
				}
				case ApplyOption::ApplyConvolution: {
					std::shared_ptr<CVASTWaveTable> copypastewavetable = getCopyPasteWavetable();
					if ((copypastewavetable == nullptr) || (copypastewavetable->getNumPositions() == 0)) {
						AlertWindow::showMessageBoxAsync(MessageBoxIconType::InfoIcon, TRANS("Nothing copied"), TRANS("The wavetable copy buffer is empty. Use copy function before."), TRANS("Continue"), this);
					}
					else {
						std::thread edit_thread(&VASTWaveTableEditorComponent::threadedEditorFunction, EditorFunction::Convolution, 0.f, /*msbeg, msend,*/ 0, 0, this, 0.f, 0.f, nullptr, true); //mod max not usable here
						edit_thread.detach();
					}
					break;
				}
				case ApplyOption::ApplyTimeToFrequency: {
					std::thread edit_thread(&VASTWaveTableEditorComponent::threadedEditorFunction, EditorFunction::TimeToFrequency, 0.f, /*msbeg, msend,*/ 0, 0, this, 0.f, 0.f, nullptr, true);
					edit_thread.detach();
					break;
				}
				case ApplyOption::ApplyFrequencyToTime: {
						std::thread edit_thread(&VASTWaveTableEditorComponent::threadedEditorFunction, EditorFunction::FrequencyToTime, 0.f, /*msbeg, msend,*/ 0, 0, this, 0.f, 0.f, nullptr, true);
					edit_thread.detach();
					break;
				}
				case ApplyOption::ApplyNormalize: {
					//sSelection* sel = c_editorOscilloscope->getSelection();
					std::thread edit_thread(&VASTWaveTableEditorComponent::threadedEditorFunction, EditorFunction::Normalize, 0.f, /*msbeg, msend,*/ 0, 0, this, 0.f, 0.f, nullptr, true);
					edit_thread.detach();
					break;
				}
				case ApplyOption::ApplyNormalizePerCycle: {
					//sSelection* sel = c_editorOscilloscope->getSelection();
					std::thread edit_thread(&VASTWaveTableEditorComponent::threadedEditorFunction, EditorFunction::NormalizePerCycle , 0.f, /*msbeg, msend,*/ 0, 0, this, 0.f, 0.f, nullptr, true);
					edit_thread.detach();
					break;
				}
				}
			}
//==================================================================================================================================
// Wavetable generators
//==================================================================================================================================
			else if (buttonThatWasClicked == c_waveTableEditorView->getHeader()->c_iconGenerate.get()) {
				int lSelected = c_waveTableEditorView->getHeader()->c_generationOptions->getSelectedItemIndex();
				switch (lSelected) {
				case GenerateOption::GenerateHardSync: {
					std::thread edit_thread(&VASTWaveTableEditorComponent::threadedEditorFunction, EditorFunction::HardSyncGenerate, c_waveTableEditorView->getHeader()->c_genMax->getValue(), /*getWtPos(), 0,*/ 0, 0, this, 0.f, 0.f, nullptr, true);
					edit_thread.detach();
					break;
				}
				case GenerateOption::GeneratePulseWaveModualtion: {
					std::thread edit_thread(&VASTWaveTableEditorComponent::threadedEditorFunction, EditorFunction::PWMGenerate, c_waveTableEditorView->getHeader()->c_genMax->getValue(), /*getWtPos(), 0,*/ 0, 0, this, 0.f, 0.f, nullptr, true);
					edit_thread.detach();
					break;
				}
				case GenerateOption::GenerateFrequencyModulation: {
					std::thread edit_thread(&VASTWaveTableEditorComponent::threadedEditorFunction, EditorFunction::FMGenerate, c_waveTableEditorView->getHeader()->c_genMax->getValue(), /*getWtPos(), 0,*/ 0, 0, this, 0.f, 0.f, nullptr, true);
					edit_thread.detach();
					break;
				}
				case GenerateOption::GenerateBloatTable: {
					std::thread edit_thread(&VASTWaveTableEditorComponent::threadedEditorFunction, EditorFunction::BloatGenerate, c_waveTableEditorView->getHeader()->c_genMax->getValue(), /*getWtPos(), 0,*/ 0, 0, this, 0.f, 0.f, nullptr, true);
					edit_thread.detach();
					break;
				}
				case GenerateOption::GenerateBendTable: {
					std::thread edit_thread(&VASTWaveTableEditorComponent::threadedEditorFunction, EditorFunction::BendGenerate, c_waveTableEditorView->getHeader()->c_genMax->getValue(), /*getWtPos(), 0,*/ 0, 0, this, 0.f, 0.f, nullptr, true);
					edit_thread.detach();
					break;
				}
				case GenerateOption::GenerateFormantsMale: {
					std::thread edit_thread(&VASTWaveTableEditorComponent::threadedEditorFunction, EditorFunction::FormantsMaleGenerate, c_waveTableEditorView->getHeader()->c_genMax->getValue(), /*getWtPos(), 0,*/ 0, 0, this, 0.f, 0.f, nullptr, true);
					edit_thread.detach();
					break;
				}
				case GenerateOption::GenerateFormantsFemale: {
					std::thread edit_thread(&VASTWaveTableEditorComponent::threadedEditorFunction, EditorFunction::FormantsFemaleGenerate, c_waveTableEditorView->getHeader()->c_genMax->getValue(), /*getWtPos(), 0,*/ 0, 0, this, 0.f, 0.f, nullptr, true);
					edit_thread.detach();
					break;
				}
				case GenerateOption::GenerateHarmonicMorph: {
					std::thread edit_thread(&VASTWaveTableEditorComponent::threadedEditorFunction, EditorFunction::HarmonicMorph, c_waveTableEditorView->getHeader()->c_genMax->getValue(), /*getWtPos(), 0,*/ 0, 0, this, 0.f, 0.f, nullptr, true);
					edit_thread.detach();
					break;
				}
				case GenerateOption::GenerateHarmonicReduce: {
					std::thread edit_thread(&VASTWaveTableEditorComponent::threadedEditorFunction, EditorFunction::HarmonicReduce, c_waveTableEditorView->getHeader()->c_genMax->getValue(), /*getWtPos(), 0,*/ 0, 0, this, 0.f, 0.f, nullptr, true);
					edit_thread.detach();
					break;
				}
				case GenerateOption::GenerateHighpassFilter: {
					std::thread edit_thread(&VASTWaveTableEditorComponent::threadedEditorFunction, EditorFunction::HighpassFilter, c_waveTableEditorView->getHeader()->c_genMax->getValue(), /*getWtPos(), 0,*/ 0, 0, this, 0.f, 0.f, nullptr, true);
					edit_thread.detach();
					break;
				}
				case GenerateOption::GenerateLowpassFilter: {
					std::thread edit_thread(&VASTWaveTableEditorComponent::threadedEditorFunction, EditorFunction::LowpassFilter, c_waveTableEditorView->getHeader()->c_genMax->getValue(), /*getWtPos(), 0,*/ 0, 0, this, 0.f, 0.f, nullptr, true);
					edit_thread.detach();
					break;
				}
				case GenerateOption::GenerateLowpassSoft: {
					sSelection* sel = c_waveTableEditorView->getEditorOscilloscope()->getSelection();
					int wtselbeg = sel->iWTSelectionStart;
					int wtselend = sel->iWTSelectionEnd;
					if (sel->iWTSelectionStart == sel->iWTSelectionEnd) {
						wtselbeg = 0;
						wtselend = C_WAVE_TABLE_SIZE;
					}
					std::thread edit_thread(&VASTWaveTableEditorComponent::threadedEditorFunction, EditorFunction::LowpassSoft, c_waveTableEditorView->getHeader()->c_genMax->getValue(), /*getWtPos(), 0,*/ wtselbeg, wtselend, this, 0.f, 0.f, nullptr, true);
					edit_thread.detach();
					break;
				}
				case GenerateOption::GenerateBandpassFilter: {
					std::thread edit_thread(&VASTWaveTableEditorComponent::threadedEditorFunction, EditorFunction::BandpassFilter, c_waveTableEditorView->getHeader()->c_genMax->getValue(), /*getWtPos(), 0,*/ 0, 0, this, 0.f, 0.f, nullptr, true);
					edit_thread.detach();
					break;
				}
				case GenerateOption::GeneratePulseFilter: {
					std::thread edit_thread(&VASTWaveTableEditorComponent::threadedEditorFunction, EditorFunction::PulseFilter, c_waveTableEditorView->getHeader()->c_genMax->getValue(), /*getWtPos(), 0,*/ 0, 0, this, 0.f, 0.f, nullptr, true);
					edit_thread.detach();
					break;
				}
				case GenerateOption::GenerateCombFilter: {
					std::thread edit_thread(&VASTWaveTableEditorComponent::threadedEditorFunction, EditorFunction::CombFilter, c_waveTableEditorView->getHeader()->c_genMax->getValue(), /*getWtPos(), 0,*/ 0, 0, this, 0.f, 0.f, nullptr, true);
					edit_thread.detach();
					break;
				}

				}
			}
		}

	if (c_samplerEditorComponent != nullptr)
		if (c_samplerEditorComponent->getHeader() != nullptr) {
			if (buttonThatWasClicked == c_samplerEditorComponent->getHeader()->c_iconClearWav.get()) {
				myProcessor->m_pVASTXperience.m_Poly.clearSamplerSoundChanged();
			}
			else if (buttonThatWasClicked == c_samplerEditorComponent->getHeader()->c_iconLoadWav.get()) {
				String m_UserWTTablesFolder = myProcessor->m_UserWavRootFolder;
				File initialFile = File(m_UserWTTablesFolder);
				mFileChoserIsOpen = true;
				/*FileChooser myChooser(TRANS("Please select the WAV file you want to load to the sampler ..."), initialFile, "*.wav;*.aiff", true, false, myEditor->vaporizerComponent);
				if (myChooser.browseForFileToOpen())
				{
				*/

				myChooser = std::make_unique<FileChooser>(TRANS("Please select the WAV / AIFF file you want to load to the sampler ..."), initialFile, "*.wav;*.aiff");
				myChooser->launchAsync(FileBrowserComponent::openMode | FileBrowserComponent::canSelectFiles,
					[this](const FileChooser& fileChooser) {
					File wtFile(fileChooser.getResult());

					VASTSamplerSound* lVASTSamplerSound = loadWavFile(wtFile.getFullPathName());
					if (lVASTSamplerSound != nullptr) {
						VASTSynthesiserSound* lSynthSound = (VASTSynthesiserSound*)(myProcessor->m_pVASTXperience.m_Poly.getSynthesizer()->getSound(0));
						lSynthSound->addSamplerSoundChanged(lVASTSamplerSound);
						m_samplerviewport->selectAll();
					}
					mFileChoserIsOpen = false;
				});
			}
			else if (buttonThatWasClicked == c_samplerEditorComponent->getHeader()->c_iconNormalizeWav.get()) {
				VASTSamplerSound* samplerSound = getCurSamplerSound();
				if (samplerSound != nullptr) {
					sSelectionWav* sel = m_samplerviewport->getSelection();
					int selStart = sel->iWavSelectionStartSample;
					int selEnd = sel->iWavSelectionEndSample;
					int numchannels = samplerSound->getAudioDataChanged()->getNumChannels();
					float maxVal = 0.f;
					for (int i = selStart; i < selEnd; i++) {
						for (int ch = 0; ch < numchannels; ch++) {
							float sample = samplerSound->getAudioDataChanged()->getReadPointer(ch)[i];
							if (abs(sample) > maxVal) maxVal = abs(sample);
						}
					}
					if (maxVal != 0.f) {
						for (int i = selStart; i < selEnd; i++) {
							for (int ch = 0; ch < numchannels; ch++) {
								samplerSound->getAudioDataChanged()->setSample(ch, i, samplerSound->getAudioDataChanged()->getReadPointer(ch)[i] / maxVal);
							}
						}
						requestUIUpdate();
					}
					samplerSound->setAudioDataChangedFlag();
				}
			}
			else if (buttonThatWasClicked == c_samplerEditorComponent->getHeader()->c_iconToEditor.get()) {
				wavSelectionToWt(0, nullptr);
			}
			else if (buttonThatWasClicked == c_samplerEditorComponent->getHeader()->c_iconZeroLoop.get()) {
				findZeroLoop();
			}
			else if (buttonThatWasClicked == c_samplerEditorComponent->getHeader()->c_iconMaximizeSampler.get()) {
				if (!myEditor->vaporizerComponent->isTabConcertinaMaximized()) {
					myEditor->vaporizerComponent->maximizeTabConcertina();
					c_concertinaEditor->maximizePanel(1); //sampler
				}
				else {
					myEditor->vaporizerComponent->defaultConcertina();
					c_concertinaEditor->maximizeBothPanels();
				}
			}
		}

	//ICONS

	initSliders();
	requestUIUpdate();
}

void VASTWaveTableEditorComponent::threadedFreehandDraw(/*int msbeg, int msend,*/ VASTWaveTableEditorComponent* editor, juce::Array<juce::Point<float>> spoints) {
	if (getEditorThreadsRunning() >= C_MAX_EDITOR_THREADS) return; //just skip it - check number
	int lIndexAtStart = editor->myProcessor->m_loadedPresetIndexCount;
	getEditorThreadsRunning()++;

	editor->myProcessor->m_pVASTXperience.m_Poly.m_OscBank[editor->m_bank].addSoftFadeEditor();

	std::shared_ptr<CVASTWaveTable> wavetable = editor->myProcessor->m_pVASTXperience.m_Poly.m_OscBank[editor->m_bank].getSoftOrCopyWavetable();
	VDBG("threadedFreehandDraw on wavetable: " << wavetable->getID());

	//do stuff
	int msbeg = wavetable->getMultiSelectBegin();
	int msend = wavetable->getMultiSelectEnd();
	std::vector<myFloat> samples = std::vector<myFloat>(C_WAVE_TABLE_SIZE);
	std::vector<myFloat> samplescopy = std::vector<myFloat>(C_WAVE_TABLE_SIZE);

	for (int wtPos = msbeg; wtPos <= msend; wtPos++) {
		samples = *editor->getCurWavetable()->getNaiveTable(wtPos);
		int wtselmax = 0;
		int wtselmin = C_WAVE_TABLE_SIZE - 1;

		juce::Point<float> lastpoint(-1,-1);
		for (juce::Point<float>& pnt : spoints) {
			if (lastpoint.getX() > 0) {
				double curvy = 0.5f;
				int sstart = lastpoint.getX();
				int send = pnt.getX();
				float val1 = lastpoint.getY();
				float val2 = pnt.getY();
				if (send < sstart) {
					sstart = pnt.getX();
					send = lastpoint.getX();
					val1 = pnt.getY();
					val2 = lastpoint.getY();
				}
				for (int i = sstart; i <= send; i++) {
					double xpos = (double(i - sstart)) / double(send);
					double y = cubicBezierApproximation(xpos, curvy);

					samples[i] = val1 + (val2 - val1) * y;

					if (samples[i] > 1) samples[i] = 1;
					if (samples[i] < -1) samples[i] = -1;
				}

				if (sstart < wtselmin) wtselmin = sstart;
				if (send > wtselmax) wtselmax = send;
			}
			lastpoint = pnt;
		}

		samplescopy = samples;

		//smooth
		float smoothvalue = 2.f;
		float e = ((smoothvalue + .5f) / 100.f) * 100; //no zero values
		int r = 0;
		float s = 2 * e + 1;
		for (int i = wtselmin; i <= wtselmax; i++) {
			if (!(s <= 2)) {
				//if (!(s <= 1)) {
				float o = 0;
				for (int n = 0; n < s; n++) {
					r = int(i - e + n + C_WAVE_TABLE_SIZE) % C_WAVE_TABLE_SIZE;
					o += samplescopy[r];
				}
				samples[i] = o / s;
			}
		}
		wavetable->setNaiveTable(wtPos, samples, true, editor->myProcessor->getWTmode()); //do pregeneration

	}
	//do stuff

	if (lIndexAtStart != editor->myProcessor->m_loadedPresetIndexCount) //changed preset in between
		VDBG("Not merged! Preset changed in meantime!");
	else {
		editor->myProcessor->m_pVASTXperience.m_Poly.m_OscBank[editor->m_bank].setWavetableSoftFade(wavetable);
	}
	editor->myProcessor->m_pVASTXperience.m_Poly.m_OscBank[editor->m_bank].removeSoftFadeEditor();

	getEditorThreadsRunning()--;
}

//----------------------------------------------------------------------------------------------------------------------------------------------
//thread editor function
//can be called as thread or with tranaction wavetable
//----------------------------------------------------------------------------------------------------------------------------------------------

void VASTWaveTableEditorComponent::threadedEditorFunction(int editorFunction, double sliderValue, /*int msbeg, int msend,*/ int wtselstart, int wtselend, VASTWaveTableEditorComponent* editor, float connectval1, float connectval2, std::shared_ptr<CVASTWaveTable> transactionWavetable, bool pregenerate) {
	if (getEditorThreadsRunning() >= C_MAX_EDITOR_THREADS) return; //just skip it - check number
	int lIndexAtStart = editor->myProcessor->m_loadedPresetIndexCount;
	getEditorThreadsRunning()++;

	int lBank = editor->m_bank;

#ifdef _DEBUG
	//VDBG("m_editorThreadsRunning " + String(getEditorThreadsRunning()));
	std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
#endif

	std::shared_ptr<CVASTWaveTable> wavetable = nullptr;
	std::shared_ptr<CVASTWaveTable> activeWavetable = nullptr;
	if (transactionWavetable == nullptr) {
		editor->myProcessor->m_pVASTXperience.m_Poly.m_OscBank[lBank].addSoftFadeEditor();
		wavetable = editor->myProcessor->m_pVASTXperience.m_Poly.m_OscBank[editor->m_bank].getSoftOrCopyWavetable();

		activeWavetable = editor->getBankWavetable(lBank);
	}
	else {
		//softfade must be handled externally
		editor->setCurWavetable(transactionWavetable); //make a copy
		activeWavetable = editor->getCurWavetable();

		wavetable = transactionWavetable;
	}

	int msbeg = wavetable->getSelectedWtPos();
	int msend = wavetable->getSelectedWtPos();
	if (wavetable->isMultiSelected()) {
		msbeg = wavetable->getMultiSelectBegin();
		msend = wavetable->getMultiSelectEnd();
	}

	VDBG("editorFunction on wavetable ID: " << wavetable->getID());

#ifdef _DEBUG
	std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
#endif

	switch (editorFunction) {
	case EditorFunction::Level: {
		std::vector<myFloat> samples = std::vector<myFloat>(C_WAVE_TABLE_SIZE);
		for (int wtPos = msbeg; wtPos <= msend; wtPos++) {
			samples = (*editor->getCurWavetable()->getNaiveTable(wtPos));
			for (int i = wtselstart; i <= wtselend; i++) {
				samples[i] = (*editor->getCurWavetable()->getNaiveTable(wtPos))[i] * ((sliderValue + 100.f) / 100.0f);
				if (samples[i] > 1) samples[i] = 1;
				if (samples[i] < -1) samples[i] = -1;
			}

			if ((sliderValue != 0.f) && (editor->m_bSnapToZero || editor->m_bStretchToZero)) editor->stretchToZeroCrossings(samples, wtselstart, wtselend, !editor->m_bStretchToZero);
			wavetable->setNaiveTable(wtPos, samples, false, editor->myProcessor->getWTmode());
		}
		break;
	}
	case EditorFunction::Phase: {
		std::vector<myFloat> samples = std::vector<myFloat>(C_WAVE_TABLE_SIZE);
		for (int wtPos = msbeg; wtPos <= msend; wtPos++) {
			samples = (*editor->getCurWavetable()->getNaiveTable(wtPos));
			//int j = 0;
			int end = wtselend - wtselstart;
			float o = int(end + floor(end * sliderValue / 100.0f)) % end;
			float r = wtselstart + o;
			for (int i = wtselstart; i <= wtselend; i++) {
				samples[i] = (*editor->getCurWavetable()->getNaiveTable(wtPos))[r];
				o = int(o + 1) % end;
				r = wtselstart + o;
			}

			if ((sliderValue != 0.f) && (editor->m_bSnapToZero || editor->m_bStretchToZero)) editor->stretchToZeroCrossings(samples, wtselstart, wtselend, !editor->m_bStretchToZero);
			wavetable->setNaiveTable(wtPos, samples, false, editor->myProcessor->getWTmode());
		}
		break;
	}
	case EditorFunction::Freq: {
		std::vector<myFloat> samples = std::vector<myFloat>(C_WAVE_TABLE_SIZE);
		for (int wtPos = msbeg; wtPos <= msend; wtPos++) {
			samples = (*editor->getCurWavetable()->getNaiveTable(wtPos));
			int end = wtselend - wtselstart + 1;
			int o = wtselend;
			for (int i = wtselstart; i <= wtselend; i++) {
				float lFreq = sliderValue;
				o = wtselstart + int((i - wtselstart) * (powf(2.0f, lFreq))) % end;
				samples[i] = (*editor->getCurWavetable()->getNaiveTable(wtPos))[o]; // todo linterp
			}

			if ((sliderValue != 0.f) && (editor->m_bSnapToZero || editor->m_bStretchToZero)) editor->stretchToZeroCrossings(samples, wtselstart, wtselend, !editor->m_bStretchToZero);
			wavetable->setNaiveTable(wtPos, samples, false, editor->myProcessor->getWTmode());
		}
		break;
	}
	case EditorFunction::FreqMod: {
		std::vector<myFloat> samples = std::vector<myFloat>(C_WAVE_TABLE_SIZE);
		for (int wtPos = msbeg; wtPos <= msend; wtPos++) {
			int wtPosTarget = wtPos;
			samples = (*editor->getCurWavetable()->getNaiveTable(wtPos));

			editor->frequencyModulate((*editor->getCurWavetable()->getNaiveTable(wtPos)), samples, wtselstart, wtselend, sliderValue);

			if ((sliderValue != 0.f) && (editor->m_bSnapToZero || editor->m_bStretchToZero)) editor->stretchToZeroCrossings(samples, wtselstart, wtselend, !editor->m_bStretchToZero);
			wavetable->setNaiveTable(wtPosTarget, samples, false, editor->myProcessor->getWTmode());
		}
		break;
	}
	case EditorFunction::BitDepth: {
		std::vector<myFloat> samples = std::vector<myFloat>(C_WAVE_TABLE_SIZE);
		for (int wtPos = msbeg; wtPos <= msend; wtPos++) {
			samples = (*editor->getCurWavetable()->getNaiveTable(wtPos));
			for (int i = wtselstart; i <= wtselend; i++) {
				int e = sliderValue;
				int n = pow(2, e) - 1;
				double j = 1.0 / n;
				double o = ((*editor->getCurWavetable()->getNaiveTable(wtPos))[i] + 1.0) / 2.0;
				o = round(o * n) * j;
				samples[i] = 2 * o - 1;
			}

			if ((sliderValue != 32.f)  && (editor->m_bSnapToZero || editor->m_bStretchToZero)) editor->stretchToZeroCrossings(samples, wtselstart, wtselend, !editor->m_bStretchToZero);
			wavetable->setNaiveTable(wtPos, samples, false, editor->myProcessor->getWTmode());
		}
		break;
	}
	case EditorFunction::Offset: {
		std::vector<myFloat> samples = std::vector<myFloat>(C_WAVE_TABLE_SIZE);
		for (int wtPos = msbeg; wtPos <= msend; wtPos++) {
			samples = (*editor->getCurWavetable()->getNaiveTable(wtPos));
			for (int i = wtselstart; i <= wtselend; i++) {
				samples[i] = (*editor->getCurWavetable()->getNaiveTable(wtPos))[i] + (sliderValue / 100.0f);
				if (samples[i] > 1.0f) samples[i] = 1.0f;
				if (samples[i] < -1.0f) samples[i] = -1.0f;
			}

			if ((sliderValue != 0.f) && (editor->m_bSnapToZero || editor->m_bStretchToZero)) editor->stretchToZeroCrossings(samples, wtselstart, wtselend, !editor->m_bStretchToZero);
			wavetable->setNaiveTable(wtPos, samples, false, editor->myProcessor->getWTmode());
		}
		break;
	}
	case EditorFunction::Bend: {
		std::vector<myFloat> samples = std::vector<myFloat>(C_WAVE_TABLE_SIZE);
		for (int wtPos = msbeg; wtPos <= msend; wtPos++) {
			samples = (*editor->getCurWavetable()->getNaiveTable(wtPos));

			editor->calcBend((*editor->getCurWavetable()->getNaiveTable(wtPos)), samples, wtselstart, wtselend, sliderValue);

			if ((sliderValue != 0.f) && (editor->m_bSnapToZero || editor->m_bStretchToZero)) editor->stretchToZeroCrossings(samples, wtselstart, wtselend, !editor->m_bStretchToZero);
			wavetable->setNaiveTable(wtPos, samples, false, editor->myProcessor->getWTmode());
		}
		break;
	}
	case EditorFunction::Bloat: {
		std::vector<myFloat> samples = std::vector<myFloat>(C_WAVE_TABLE_SIZE);
		for (int wtPos = msbeg; wtPos <= msend; wtPos++) {
			samples = (*editor->getCurWavetable()->getNaiveTable(wtPos));

			editor->calcBloat((*editor->getCurWavetable()->getNaiveTable(wtPos)), samples, wtselstart, wtselend, sliderValue);

			if ((sliderValue != 0.f) && (editor->m_bSnapToZero || editor->m_bStretchToZero)) editor->stretchToZeroCrossings(samples, wtselstart, wtselend, !editor->m_bStretchToZero);
			wavetable->setNaiveTable(wtPos, samples, false, editor->myProcessor->getWTmode());
		}
		break;
	}
	case EditorFunction::Smooth: {
		std::vector<myFloat> samples = std::vector<myFloat>(C_WAVE_TABLE_SIZE);
		std::vector<myFloat> samplescopy = std::vector<myFloat>(C_WAVE_TABLE_SIZE);
		for (int wtPos = msbeg; wtPos <= msend; wtPos++) {
			samples = (*editor->getCurWavetable()->getNaiveTable(wtPos));
			samplescopy = (*editor->getCurWavetable()->getNaiveTable(wtPos));
			//int end = wtselend - wtselstart;

			float local_max_before = 0.0f; //for normalize
			float local_max_after = 0.0f; //for normalize
			float e = ((sliderValue + .5f) / 100.f) * 100; //no zero values
			int r = 0;
			float s = 2 * e + 1;
			for (int i = wtselstart; i <= wtselend; i++) {
				if (!(s <= 2)) {
					//if (!(s <= 1)) {
					float o = 0;
					for (int n = 0; n < s; n++) {
						r = int(i - e + n + C_WAVE_TABLE_SIZE) % C_WAVE_TABLE_SIZE;
						o += samplescopy[r];
					}
					samples[i] = o / s;
				}

				float sample = abs(samplescopy[i]); //for normalize
				if (sample > local_max_before) local_max_before = sample; //for normalize
				if (abs(samples[i]) > local_max_after) local_max_after = abs(samples[i]); //for normalize
			}

			//normalize
			float gain = 1.f;
			if ((local_max_after != 0.f) && (local_max_before != 0.f))
				gain = 1.f / (local_max_after / local_max_before);
			for (int i = wtselstart; i <= wtselend; i++) {
				samples[i] *= gain;
			}

			if ((sliderValue != 0.f) && (editor->m_bSnapToZero || editor->m_bStretchToZero)) editor->stretchToZeroCrossings(samples, wtselstart, wtselend, !editor->m_bStretchToZero);
			wavetable->setNaiveTable(wtPos, samples, false, editor->myProcessor->getWTmode());
		}
		break;
	}
	case EditorFunction::Clean: {
		std::vector<myFloat> samples = std::vector<myFloat>(C_WAVE_TABLE_SIZE);
		for (int wtPos = msbeg; wtPos <= msend; wtPos++) {
			samples = (*editor->getCurWavetable()->getNaiveTable(wtPos));
			int end = wtselend - wtselstart;
			float o = sliderValue / 100.f * (end * 0.5f);
			float r = 1.0f / o;
			float s = 0;
			if (o > 0) {
				for (int i = 0; i <= o; i++) {
					samples[wtselstart + i] = (*editor->getCurWavetable()->getNaiveTable(wtPos))[wtselstart + i] * s;
					samples[wtselend - i] = (*editor->getCurWavetable()->getNaiveTable(wtPos))[wtselend - i] * s;
					s += r;
					if (samples[i] > 1) samples[i] = 1;
					if (samples[i] < -1) samples[i] = -1;
				}
			}

			if ((sliderValue != 0.f) && (editor->m_bSnapToZero || editor->m_bStretchToZero)) editor->stretchToZeroCrossings(samples, wtselstart, wtselend, !editor->m_bStretchToZero);
			wavetable->setNaiveTable(wtPos, samples, false, editor->myProcessor->getWTmode());
		}
		break;
	}
	case EditorFunction::Connect: {
		std::vector<myFloat> samples = std::vector<myFloat>(C_WAVE_TABLE_SIZE);
		for (int wtPos = msbeg; wtPos <= msend; wtPos++) {
			for (int i = 0; i < C_WAVE_TABLE_SIZE; i++) {
				samples[i] = (*wavetable->getNaiveTable(wtPos))[i];
			}
			int j = 0;
			int end = wtselend - wtselstart;
			if (end != 0) {
				//float val1 = (*editor->getCurWavetable()->getNaiveTable(wtPos))[(wtselstart > 0) ? wtselstart - 1 : wtselstart];
				//float val2 = (*editor->getCurWavetable()->getNaiveTable(wtPos))[(wtselend < C_WAVE_TABLE_SIZE - 1) ? wtselend + 1 : wtselend];
				float val1 = (*editor->getCurWavetable()->getNaiveTable(wtPos))[(wtselstart > 0) ? wtselstart : wtselstart];
				float val2 = (*editor->getCurWavetable()->getNaiveTable(wtPos))[(wtselend < C_WAVE_TABLE_SIZE - 1) ? wtselend : wtselend];
				double curvy = (1.0 - sliderValue * 0.01) * 0.5;
				//float e = 2 * M_PI;
				for (int i = wtselstart; i <= wtselend; i++) {
					double xpos = (double(i - wtselstart)) / double(end);
					double y = cubicBezierApproximation(xpos, curvy);

					//samples[i] = val1 + (val2 - val1) * float(j) / float(end) *  sinf(0.25f * e * float(j) / float(end));
					samples[i] = val1 + (val2 - val1) * y;

					if (samples[i] > 1) samples[i] = 1;
					if (samples[i] < -1) samples[i] = -1;
					j++;
				}

				wavetable->setNaiveTable(wtPos, samples, false, editor->myProcessor->getWTmode());
			}
		}
		break;
	}
	case EditorFunction::DrawConnect: {
		std::vector<myFloat> samples = std::vector<myFloat>(C_WAVE_TABLE_SIZE);
		for (int wtPos = msbeg; wtPos <= msend; wtPos++) {
			samples = (*editor->getCurWavetable()->getNaiveTable(wtPos));
			int j = 0;
			int end = wtselend - wtselstart;
			if (end != 0) {
				float val1 = connectval1;
				float val2 = connectval2;

				double curvy = (1.0 - sliderValue * 0.01) * 0.5;
				//float e = 2 * M_PI;
				for (int i = wtselstart; i <= wtselend; i++) {
					double xpos = (double(i - wtselstart)) / double(end);
					double y = cubicBezierApproximation(xpos, curvy);

					//samples[i] = val1 + (val2 - val1) * float(j) / float(end) *  sinf(0.25f * e * float(j) / float(end));
					samples[i] = val1 + (val2 - val1) * y;

					if (samples[i] > 1) samples[i] = 1;
					if (samples[i] < -1) samples[i] = -1;
					j++;
				}
				wavetable->setNaiveTable(wtPos, samples, false, editor->myProcessor->getWTmode());
			}
			else { //single value
				float val1 = connectval1;
				samples[wtselstart] = val1;
				wavetable->setNaiveTable(wtPos, samples, false, editor->myProcessor->getWTmode());
			}
		}
		break;
	}
	case EditorFunction::Normalize: {

		normalizePositions(msbeg, msend, wavetable.get(), activeWavetable, false, editor->myProcessor->getWTmode()); //static call
		break;
	}
	case EditorFunction::NormalizePerCycle: {

		normalizePositions(msbeg, msend, wavetable.get(), activeWavetable, false, editor->myProcessor->getWTmode()); //static call
		break;
	}
	case EditorFunction::Interpolate: {
		std::vector<myFloat> samples = std::vector<myFloat>(C_WAVE_TABLE_SIZE);
		for (int wtPos = msbeg + 1; wtPos <= msend - 1; wtPos++) {
			for (int i = 0; i < C_WAVE_TABLE_SIZE; i++) {
				float sample_beg = (*activeWavetable->getNaiveTable(msbeg))[i];
				float sample_end = (*activeWavetable->getNaiveTable(msend))[i];

				float interp = dLinTerp(msbeg, msend, sample_beg, sample_end, wtPos);
				samples[i] = interp;
			}
			wavetable->setNaiveTable(wtPos, samples, pregenerate, editor->myProcessor->getWTmode());
		}
		break;
	}
	case EditorFunction::AmplModCopy: {
		int copyPos = editor->getCopyPasteWavetable()->getNumPositions();
		std::vector<myFloat> samples = std::vector<myFloat>(C_WAVE_TABLE_SIZE);
		for (int wtPos = msbeg; wtPos <= msend; wtPos++) {
			float posfromCopy = float(wtPos - msbeg) * (float(copyPos - 1) / float(msend - msbeg + 1));
			for (int i = 0; i < C_WAVE_TABLE_SIZE; i++) {
				//interpolate;
				int intpart = int(posfromCopy);
				float fracpart = posfromCopy - intpart;
				float interpolated = 0.f;
				if (copyPos == 1)
					interpolated = (*editor->getCopyPasteWavetable()->getNaiveTable(intpart))[i];
				else
					interpolated = (1.f - fracpart) * (*editor->getCopyPasteWavetable()->getNaiveTable(intpart))[i] + fracpart * (*editor->getCopyPasteWavetable()->getNaiveTable((intpart + 1)))[i];
				samples[i] = (*activeWavetable->getNaiveTable(wtPos))[i] * interpolated;
			}
			wavetable->setNaiveTable(wtPos, samples, pregenerate, editor->myProcessor->getWTmode());
		}
		break;
	}
	case EditorFunction::MixInCopy: {
		int copyPos = editor->getCopyPasteWavetable()->getNumPositions();
		std::vector<myFloat> samples = std::vector<myFloat>(C_WAVE_TABLE_SIZE);
		for (int wtPos = msbeg; wtPos <= msend; wtPos++) {
			float posfromCopy = float(wtPos - msbeg) * (float(copyPos - 1) / float(msend - msbeg + 1));
			for (int i = 0; i < C_WAVE_TABLE_SIZE; i++) {
				//interpolate;
				int intpart = int(posfromCopy);
				float fracpart = posfromCopy - intpart;
				float interpolated = 0.f;
				if (copyPos == 1)
					interpolated = (*editor->getCopyPasteWavetable()->getNaiveTable(intpart))[i];
				else
					interpolated = (1.f - fracpart) * (*editor->getCopyPasteWavetable()->getNaiveTable(intpart))[i] + fracpart * (*editor->getCopyPasteWavetable()->getNaiveTable((intpart + 1)))[i];
				float factor = (sliderValue * 0.01f);
				samples[i] = (1.f - factor) * (*activeWavetable->getNaiveTable(wtPos))[i] + factor * interpolated;
			}
			wavetable->setNaiveTable(wtPos, samples, pregenerate, editor->myProcessor->getWTmode());
		}
		break;
	}
	case EditorFunction::Convolution: {
		int copyPos = editor->getCopyPasteWavetable()->getNumPositions();
		for (int wtPos = msbeg; wtPos <= msend; wtPos++) {
			float posfromCopy = float(wtPos - msbeg) * (float(copyPos - 1) / float(msend - msbeg + 1));
			std::vector<dsp::Complex<float>> domainBuffer = *wavetable->getFreqDomainBuffer(wtPos);
			std::vector<dsp::Complex<float>> domainCopyBuffer = *editor->getCopyPasteWavetable()->getFreqDomainBuffer(posfromCopy);
			std::vector<dsp::Complex<float>> domainCopyBufferNext;
			if (posfromCopy + 1 < copyPos)
				domainCopyBufferNext = *editor->getCopyPasteWavetable()->getFreqDomainBuffer(posfromCopy + 1); //for interpolation between wtpos

			for (int i = 1; i < C_WAVE_TABLE_SIZE / 2; i++) {
				int intpart = int(posfromCopy);
				float fracpart = posfromCopy - intpart;
				if ((copyPos == 1) || (posfromCopy + 1 >= copyPos))
					domainBuffer[i] = domainBuffer[i] * domainCopyBuffer[i]; //complex multiplication --> convolute
				else
					domainBuffer[i] = (1.f - fracpart) * domainBuffer[i] * domainCopyBuffer[i] + fracpart * domainBuffer[i] * domainCopyBufferNext[i]; //complex multiplication --> convolute
			}
			wavetable->setFreqDomainTables(wtPos, &domainBuffer, false, true, editor->myProcessor->getWTmode()); //CHECK clip --> to maintain volume
		}
		//when clipped need to normalize to global maximum
		editor->normalizePositions(msbeg, msend, wavetable.get(), wavetable, pregenerate, editor->myProcessor->getWTmode());
		break;
	}
	case EditorFunction::TimeToFrequency: {
		int numGridlines = C_WAVE_TABLE_SIZE / 2;
		if (editor->myProcessor->m_iWTEditorGridMode != OscillatorGridMode::NoGrid)
			numGridlines = pow(2, editor->myProcessor->m_iWTEditorGridMode + 1); //is dependent from grid
		std::vector<myFloat> samples = std::vector<myFloat>(C_WAVE_TABLE_SIZE);
		for (int wtPos = msbeg; wtPos <= msend; wtPos++) {
			samples = *activeWavetable->getNaiveTable(wtPos);
			std::vector<dsp::Complex<float>> domainBuffer = *wavetable->getFreqDomainBuffer(wtPos);
			//for (int i = wtselstart; i <= wtselend; i++) //do not regard selection?
			for (int i = 1; i < C_WAVE_TABLE_SIZE / 2; i++) {
				if (i <= numGridlines) {
					float newPerc = (samples[(i - 1) * (C_WAVE_TABLE_SIZE / numGridlines) + 1] + 1.f) * 0.5f; //use every grid sample
					domainBuffer[i] = VASTFreqDomainViewport::setDomainBufferSlotRealPercentage(newPerc, domainBuffer[i]);
				} else
					domainBuffer[i] = VASTFreqDomainViewport::setDomainBufferSlotRealPercentage(0.f, domainBuffer[i]);
				domainBuffer[i] = VASTFreqDomainViewport::setDomainBufferSlotImagPercentage(0.f, domainBuffer[i]); //what about imag?
			}
			wavetable->setFreqDomainTables(wtPos, &domainBuffer, pregenerate, false, editor->myProcessor->getWTmode()); //CHECK clip
		}
		break;
	}
	case EditorFunction::FrequencyToTime: {
		int numGridlines = C_WAVE_TABLE_SIZE / 2;
		if (editor->myProcessor->m_iWTEditorGridMode != OscillatorGridMode::NoGrid)
			numGridlines = pow(2, editor->myProcessor->m_iWTEditorGridMode + 1); //is dependent from grid
		std::vector<myFloat> samples = std::vector<myFloat>(C_WAVE_TABLE_SIZE);
		for (int wtPos = msbeg; wtPos <= msend; wtPos++) {
			float local_max_before = 0.0f; //for normalize
			samples = *activeWavetable->getNaiveTable(wtPos);
			std::vector<dsp::Complex<float>> domainBuffer = *wavetable->getFreqDomainBuffer(wtPos);
			//for (int i = wtselstart; i <= wtselend; i++) //do not regard selection?
			for (int i = 1; i < C_WAVE_TABLE_SIZE; i++) {
				int slotToUse = ((i - 1) / (C_WAVE_TABLE_SIZE / numGridlines)) + 1;
				float newVal = VASTFreqDomainViewport::getDomainBufferSlotRealPercentage(domainBuffer[slotToUse]); //disregard imag part, no interpolation?
				if (abs(newVal) > local_max_before) local_max_before = abs(newVal); //for normalize
				samples[i] = (newVal * 2.f) - 1.f;
			}
			//normalize
			float gain = 1.f;
			if (local_max_before != 0.f)
				gain = 1.f / (local_max_before);
			for (int i = wtselstart; i <= wtselend; i++) {
				samples[i] *= gain;
			}
			wavetable->setNaiveTable(wtPos, samples, pregenerate, editor->myProcessor->getWTmode());
		}
		break;
	}
	case EditorFunction::PWMGenerate: {
		wavetable->clear(); //delete all pos
		int selPos = msbeg;
		int wtPos = 0;
		float pwmQuota = sliderValue / 100.f;
		std::vector<myFloat> naivePWMTable = std::vector<myFloat>(C_WAVE_TABLE_SIZE);
		for (int i = 0; i <= C_MAX_NUM_POSITIONS - 1; i++) {
			if (wtPos >= wavetable->getNumPositions())
				wavetable->addPosition();

			naivePWMTable = activeWavetable->calcPWMNaiveWave(i, selPos, pwmQuota); //read original
			if (!(std::adjacent_find(naivePWMTable.begin(), naivePWMTable.end(), std::not_equal_to<float>()) == naivePWMTable.end())) { //are all elements the same?
				wavetable->setNaiveTable(wtPos, naivePWMTable, pregenerate, editor->myProcessor->getWTmode()); // preGenerate
				wtPos++;
			}
		}
		wavetable->setSelectedWtPos(0);
		break;
	}
	case EditorFunction::FMGenerate: {
		float maxFM = sliderValue;
		int selPos = msbeg;
		std::vector<myFloat> samples = std::vector<myFloat>(C_WAVE_TABLE_SIZE);
		for (int wtPos = 0; wtPos <= C_MAX_NUM_POSITIONS - 1; wtPos++) {
			if (wtPos >= wavetable->getNumPositions())
				wavetable->addPosition();
			float mod = maxFM * (float(wtPos) / (float(C_MAX_NUM_POSITIONS - 1)));
			if (wtPos == 0) mod = 0.f;
			frequencyModulate(*activeWavetable->getNaiveTable(selPos), samples, 0, C_WAVE_TABLE_SIZE - 1, mod);
			wavetable->setNaiveTable(wtPos, samples, pregenerate, editor->myProcessor->getWTmode());
		}
		wavetable->setSelectedWtPos(0);
		break;
	}
	case EditorFunction::HardSyncGenerate: {
		wavetable->clear(); //delete all pos
		int selPos = msbeg;
		std::vector<myFloat> naiveSynchTable = std::vector<myFloat>(C_WAVE_TABLE_SIZE);
		const int m_SYNCHASHQUOTA = 64;
		float fQuota = (sliderValue / 50.f) * float(m_SYNCHASHQUOTA);
		int wtPos = 0;
		for (int i = 0; i <= C_MAX_NUM_POSITIONS - 1; i++) {
			if (wtPos >= wavetable->getNumPositions())
				wavetable->addPosition();
			naiveSynchTable = activeWavetable->calcSyncNaiveWave(i, selPos, fQuota); //read original

			if (!(std::adjacent_find(naiveSynchTable.begin(), naiveSynchTable.end(), std::not_equal_to<float>()) == naiveSynchTable.end())) { //are all elements the same?
				wavetable->setNaiveTable(wtPos, naiveSynchTable, pregenerate, editor->myProcessor->getWTmode()); // preGenerate
				wtPos++;
			}
		}
		wavetable->setSelectedWtPos(0);
		break;
	}
	case EditorFunction::BloatGenerate: {
		float maxBloat = sliderValue;
		int selPos = msbeg;
		std::vector<myFloat> samples = std::vector<myFloat>(C_WAVE_TABLE_SIZE);
		for (int wtPos = 0; wtPos <= C_MAX_NUM_POSITIONS - 1; wtPos++) {
			if (wtPos >= wavetable->getNumPositions())
				wavetable->addPosition();
			float mod = -maxBloat + 2.f * maxBloat * (float(wtPos) / (float(C_MAX_NUM_POSITIONS - 1)));
			//if (wtPos == 0) mod = 0.f;
			calcBloat(*activeWavetable->getNaiveTable(selPos), samples, 0, C_WAVE_TABLE_SIZE - 1, mod);
			wavetable->setNaiveTable(wtPos, samples, pregenerate, editor->myProcessor->getWTmode()); //generate
		}
		wavetable->setSelectedWtPos(0);
		break;
	}
	case EditorFunction::BendGenerate: {
		float maxBend = sliderValue;
		int selPos = msbeg;
		std::vector<myFloat> samples = std::vector<myFloat>(C_WAVE_TABLE_SIZE);
		for (int wtPos = 0; wtPos <= C_MAX_NUM_POSITIONS - 1; wtPos++) {
			if (wtPos >= wavetable->getNumPositions())
				wavetable->addPosition();
			float mod = -maxBend + 2.f * maxBend * (float(wtPos) / (float(C_MAX_NUM_POSITIONS - 1)));
			//if (wtPos == 0) mod = 0.f;
			calcBend(*activeWavetable->getNaiveTable(selPos), samples, 0, C_WAVE_TABLE_SIZE - 1, mod);
			wavetable->setNaiveTable(wtPos, samples, pregenerate, editor->myProcessor->getWTmode());
		}
		wavetable->setSelectedWtPos(0);
		break;
	}
	case EditorFunction::FormantsMaleGenerate: {
		//sliderValue;
		wavetable->clear();
		for (int wtPos = 0; wtPos < 5; wtPos++) {
			activeWavetable->copyPositionToOtherWavetable(msbeg, wtPos, wavetable.get());
			std::vector<dsp::Complex<float>> domainBuffer = *wavetable->getFreqDomainBuffer(wtPos);
			int slot = 1;
			float newPerc = 1.f;
			//slot				//freq
			//1					105
			//2					210
			//3					315
			//4					420
			//5					525
			//6					630
			//7					735
			//8					840
			//9					945
			//10				1050
			//11				1155
			//12				1260
			//13				1365
			//14				1470
			//15				1575
			//16				1680
			//17				1785
			//18				1890
			//19				1995
			//20				2100
			//21				2205
			//22				2310
			//23				2415
			//24				2520
			//25				2625
			//25				2730
			//26				2835
			//27				2940
			//28				3045
			//29				3150
			//30				3255
			//31				3360
			//https://www.researchgate.net/figure/Mean-harmonic-frequency-values-for-the-first-three-formants-F1-F2-F3-in-Hz-for_tbl2_38071353

			switch (wtPos) {
			case 0: //A
				slot = 1;
				newPerc = +0.3f;
				domainBuffer[slot] = VASTFreqDomainViewport::setDomainBufferSlotRealPercentage(VASTFreqDomainViewport::getDomainBufferSlotRealPercentage(domainBuffer[slot]) + newPerc * sliderValue / 100.f, domainBuffer[slot]);
				slot = 7; //754
				newPerc = +0.8f;
				domainBuffer[slot] = VASTFreqDomainViewport::setDomainBufferSlotRealPercentage(VASTFreqDomainViewport::getDomainBufferSlotRealPercentage(domainBuffer[slot]) + newPerc * sliderValue / 100.f, domainBuffer[slot]);
				slot = 8; //754
				newPerc = +0.2f;
				domainBuffer[slot] = VASTFreqDomainViewport::setDomainBufferSlotRealPercentage(VASTFreqDomainViewport::getDomainBufferSlotRealPercentage(domainBuffer[slot]) + newPerc * sliderValue / 100.f, domainBuffer[slot]);
				slot = 12; //1279
				newPerc = +0.6f;
				domainBuffer[slot] = VASTFreqDomainViewport::setDomainBufferSlotRealPercentage(VASTFreqDomainViewport::getDomainBufferSlotRealPercentage(domainBuffer[slot]) + newPerc * sliderValue / 100.f, domainBuffer[slot]);
				slot = 13; //1279
				newPerc = +0.1f;
				domainBuffer[slot] = VASTFreqDomainViewport::setDomainBufferSlotRealPercentage(VASTFreqDomainViewport::getDomainBufferSlotRealPercentage(domainBuffer[slot]) + newPerc * sliderValue / 100.f, domainBuffer[slot]);
				slot = 23; //2483
				newPerc = +0.3f;
				domainBuffer[slot] = VASTFreqDomainViewport::setDomainBufferSlotRealPercentage(VASTFreqDomainViewport::getDomainBufferSlotRealPercentage(domainBuffer[slot]) + newPerc * sliderValue / 100.f, domainBuffer[slot]);
				slot = 24; //2483
				newPerc = +0.35f;
				domainBuffer[slot] = VASTFreqDomainViewport::setDomainBufferSlotRealPercentage(VASTFreqDomainViewport::getDomainBufferSlotRealPercentage(domainBuffer[slot]) + newPerc * sliderValue / 100.f, domainBuffer[slot]);
				break;
			case 1: //E
				slot = 1;
				newPerc = +0.3f;
				domainBuffer[slot] = VASTFreqDomainViewport::setDomainBufferSlotRealPercentage(VASTFreqDomainViewport::getDomainBufferSlotRealPercentage(domainBuffer[slot]) + newPerc * sliderValue / 100.f, domainBuffer[slot]);
				slot = 3; //406
				newPerc = +0.1f;
				domainBuffer[slot] = VASTFreqDomainViewport::setDomainBufferSlotRealPercentage(VASTFreqDomainViewport::getDomainBufferSlotRealPercentage(domainBuffer[slot]) + newPerc * sliderValue / 100.f, domainBuffer[slot]);
				slot = 4; //406
				newPerc = +0.7f;
				domainBuffer[slot] = VASTFreqDomainViewport::setDomainBufferSlotRealPercentage(VASTFreqDomainViewport::getDomainBufferSlotRealPercentage(domainBuffer[slot]) + newPerc * sliderValue / 100.f, domainBuffer[slot]);
				slot = 18; //1955
				newPerc = +0.1f;
				domainBuffer[slot] = VASTFreqDomainViewport::setDomainBufferSlotRealPercentage(VASTFreqDomainViewport::getDomainBufferSlotRealPercentage(domainBuffer[slot]) + newPerc * sliderValue / 100.f, domainBuffer[slot]);
				slot = 19; //1955
				newPerc = +0.6f;
				domainBuffer[slot] = VASTFreqDomainViewport::setDomainBufferSlotRealPercentage(VASTFreqDomainViewport::getDomainBufferSlotRealPercentage(domainBuffer[slot]) + newPerc * sliderValue / 100.f, domainBuffer[slot]);
				slot = 24; //2540
				newPerc = +0.5f;
				domainBuffer[slot] = VASTFreqDomainViewport::setDomainBufferSlotRealPercentage(VASTFreqDomainViewport::getDomainBufferSlotRealPercentage(domainBuffer[slot]) + newPerc * sliderValue / 100.f, domainBuffer[slot]);
				slot = 25; //2540
				newPerc = +0.1f;
				domainBuffer[slot] = VASTFreqDomainViewport::setDomainBufferSlotRealPercentage(VASTFreqDomainViewport::getDomainBufferSlotRealPercentage(domainBuffer[slot]) + newPerc * sliderValue / 100.f, domainBuffer[slot]);
				break;
			case 2: //I
				slot = 1;
				newPerc = +0.3f;
				domainBuffer[slot] = VASTFreqDomainViewport::setDomainBufferSlotRealPercentage(VASTFreqDomainViewport::getDomainBufferSlotRealPercentage(domainBuffer[slot]) + newPerc * sliderValue / 100.f, domainBuffer[slot]);
				slot = 2; //297
				newPerc = +0.1f;
				domainBuffer[slot] = VASTFreqDomainViewport::setDomainBufferSlotRealPercentage(VASTFreqDomainViewport::getDomainBufferSlotRealPercentage(domainBuffer[slot]) + newPerc * sliderValue / 100.f, domainBuffer[slot]);
				slot = 3; //297
				newPerc = +0.7f;
				domainBuffer[slot] = VASTFreqDomainViewport::setDomainBufferSlotRealPercentage(VASTFreqDomainViewport::getDomainBufferSlotRealPercentage(domainBuffer[slot]) + newPerc * sliderValue / 100.f, domainBuffer[slot]);
				slot = 20; //2150
				newPerc = +0.4f;
				domainBuffer[slot] = VASTFreqDomainViewport::setDomainBufferSlotRealPercentage(VASTFreqDomainViewport::getDomainBufferSlotRealPercentage(domainBuffer[slot]) + newPerc * sliderValue / 100.f, domainBuffer[slot]);
				slot = 21; //2150
				newPerc = +0.4f;
				domainBuffer[slot] = VASTFreqDomainViewport::setDomainBufferSlotRealPercentage(VASTFreqDomainViewport::getDomainBufferSlotRealPercentage(domainBuffer[slot]) + newPerc * sliderValue / 100.f, domainBuffer[slot]);
				slot = 26; //2925
				newPerc = +0.1f;
				domainBuffer[slot] = VASTFreqDomainViewport::setDomainBufferSlotRealPercentage(VASTFreqDomainViewport::getDomainBufferSlotRealPercentage(domainBuffer[slot]) + newPerc * sliderValue / 100.f, domainBuffer[slot]);
				slot = 27; //2925
				newPerc = +0.5f;
				domainBuffer[slot] = VASTFreqDomainViewport::setDomainBufferSlotRealPercentage(VASTFreqDomainViewport::getDomainBufferSlotRealPercentage(domainBuffer[slot]) + newPerc * sliderValue / 100.f, domainBuffer[slot]);
				break;
			case 3: //O
				slot = 1;
				newPerc = +0.3f;
				domainBuffer[slot] = VASTFreqDomainViewport::setDomainBufferSlotRealPercentage(VASTFreqDomainViewport::getDomainBufferSlotRealPercentage(domainBuffer[slot]) + newPerc * sliderValue / 100.f, domainBuffer[slot]);
				slot = 3; //411
				newPerc = +0.1f;
				domainBuffer[slot] = VASTFreqDomainViewport::setDomainBufferSlotRealPercentage(VASTFreqDomainViewport::getDomainBufferSlotRealPercentage(domainBuffer[slot]) + newPerc * sliderValue / 100.f, domainBuffer[slot]);
				slot = 4; //411
				newPerc = +0.8f;
				domainBuffer[slot] = VASTFreqDomainViewport::setDomainBufferSlotRealPercentage(VASTFreqDomainViewport::getDomainBufferSlotRealPercentage(domainBuffer[slot]) + newPerc * sliderValue / 100.f, domainBuffer[slot]);
				slot = 8; //832
				newPerc = +0.6f;
				domainBuffer[slot] = VASTFreqDomainViewport::setDomainBufferSlotRealPercentage(VASTFreqDomainViewport::getDomainBufferSlotRealPercentage(domainBuffer[slot]) + newPerc * sliderValue / 100.f, domainBuffer[slot]);
				slot = 22; //2376
				newPerc = +0.4f;
				domainBuffer[slot] = VASTFreqDomainViewport::setDomainBufferSlotRealPercentage(VASTFreqDomainViewport::getDomainBufferSlotRealPercentage(domainBuffer[slot]) + newPerc * sliderValue / 100.f, domainBuffer[slot]);
				slot = 23; //2376
				newPerc = +0.5f;
				domainBuffer[slot] = VASTFreqDomainViewport::setDomainBufferSlotRealPercentage(VASTFreqDomainViewport::getDomainBufferSlotRealPercentage(domainBuffer[slot]) + newPerc * sliderValue / 100.f, domainBuffer[slot]);
				break;
			case 4: //U
				slot = 1;
				newPerc = +0.3f;
				domainBuffer[slot] = VASTFreqDomainViewport::setDomainBufferSlotRealPercentage(VASTFreqDomainViewport::getDomainBufferSlotRealPercentage(domainBuffer[slot]) + newPerc * sliderValue / 100.f, domainBuffer[slot]);
				slot = 3; //345
				newPerc = +0.7f;
				domainBuffer[slot] = VASTFreqDomainViewport::setDomainBufferSlotRealPercentage(VASTFreqDomainViewport::getDomainBufferSlotRealPercentage(domainBuffer[slot]) + newPerc * sliderValue / 100.f, domainBuffer[slot]);
				slot = 4; //345
				newPerc = +0.4f;
				domainBuffer[slot] = VASTFreqDomainViewport::setDomainBufferSlotRealPercentage(VASTFreqDomainViewport::getDomainBufferSlotRealPercentage(domainBuffer[slot]) + newPerc * sliderValue / 100.f, domainBuffer[slot]);
				slot = 7; //799
				newPerc = +0.6f;
				domainBuffer[slot] = VASTFreqDomainViewport::setDomainBufferSlotRealPercentage(VASTFreqDomainViewport::getDomainBufferSlotRealPercentage(domainBuffer[slot]) + newPerc * sliderValue / 100.f, domainBuffer[slot]);
				slot = 8; //799
				newPerc = +0.6f;
				domainBuffer[slot] = VASTFreqDomainViewport::setDomainBufferSlotRealPercentage(VASTFreqDomainViewport::getDomainBufferSlotRealPercentage(domainBuffer[slot]) + newPerc * sliderValue / 100.f, domainBuffer[slot]);
				slot = 22; //2351
				newPerc = +0.4f;
				domainBuffer[slot] = VASTFreqDomainViewport::setDomainBufferSlotRealPercentage(VASTFreqDomainViewport::getDomainBufferSlotRealPercentage(domainBuffer[slot]) + newPerc * sliderValue / 100.f, domainBuffer[slot]);
				slot = 23; //2351
				newPerc = +0.4f;
				domainBuffer[slot] = VASTFreqDomainViewport::setDomainBufferSlotRealPercentage(VASTFreqDomainViewport::getDomainBufferSlotRealPercentage(domainBuffer[slot]) + newPerc * sliderValue / 100.f, domainBuffer[slot]);
				break;
			}
			wavetable->setFreqDomainTables(wtPos, &domainBuffer, pregenerate, false, editor->myProcessor->getWTmode()); //CHECK clip
		}
		wavetable->setSelectedWtPos(0);
		break;
	}
	case EditorFunction::FormantsFemaleGenerate: {
		//sliderValue;
		wavetable->clear();
		for (int wtPos = 0; wtPos < 5; wtPos++) {
			activeWavetable->copyPositionToOtherWavetable(msbeg, wtPos, wavetable.get());
			std::vector<dsp::Complex<float>> domainBuffer = *wavetable->getFreqDomainBuffer(wtPos);
			int slot = 1;
			float newPerc = 1.f;

			//slot				//freq
			//1					105
			//2					210
			//3					315
			//4					420
			//5					525
			//6					630
			//7					735
			//8					840
			//9					945
			//10				1050
			//11				1155
			//12				1260
			//13				1365
			//14				1470
			//15				1575
			//16				1680
			//17				1785
			//18				1890
			//19				1995
			//20				2100
			//21				2205
			//22				2310
			//23				2415
			//24				2520
			//25				2625
			//25				2730
			//26				2835
			//27				2940
			//28				3045
			//29				3150
			//30				3255
			//31				3360
			//https://www.researchgate.net/figure/Mean-harmonic-frequency-values-for-the-first-three-formants-F1-F2-F3-in-Hz-for_tbl2_38071353

			switch (wtPos) {
			case 0: //A
				slot = 1;
				newPerc = +0.3f;
				domainBuffer[slot] = VASTFreqDomainViewport::setDomainBufferSlotRealPercentage(VASTFreqDomainViewport::getDomainBufferSlotRealPercentage(domainBuffer[slot]) + newPerc * sliderValue / 100.f, domainBuffer[slot]);
				slot = 9; //1003
				newPerc = +0.8f;
				domainBuffer[slot] = VASTFreqDomainViewport::setDomainBufferSlotRealPercentage(VASTFreqDomainViewport::getDomainBufferSlotRealPercentage(domainBuffer[slot]) + newPerc * sliderValue / 100.f, domainBuffer[slot]);
				slot = 10; //1003
				newPerc = +0.7f;
				domainBuffer[slot] = VASTFreqDomainViewport::setDomainBufferSlotRealPercentage(VASTFreqDomainViewport::getDomainBufferSlotRealPercentage(domainBuffer[slot]) + newPerc * sliderValue / 100.f, domainBuffer[slot]);
				slot = 14; //1550
				newPerc = +0.1f;
				domainBuffer[slot] = VASTFreqDomainViewport::setDomainBufferSlotRealPercentage(VASTFreqDomainViewport::getDomainBufferSlotRealPercentage(domainBuffer[slot]) + newPerc * sliderValue / 100.f, domainBuffer[slot]);
				slot = 15; //1550
				newPerc = +0.6f;
				domainBuffer[slot] = VASTFreqDomainViewport::setDomainBufferSlotRealPercentage(VASTFreqDomainViewport::getDomainBufferSlotRealPercentage(domainBuffer[slot]) + newPerc * sliderValue / 100.f, domainBuffer[slot]);
				slot = 27; //2960
				newPerc = +0.4f;
				domainBuffer[slot] = VASTFreqDomainViewport::setDomainBufferSlotRealPercentage(VASTFreqDomainViewport::getDomainBufferSlotRealPercentage(domainBuffer[slot]) + newPerc * sliderValue / 100.f, domainBuffer[slot]);
				break;
			case 1: //E
				slot = 1;
				newPerc = +0.3f;
				domainBuffer[slot] = VASTFreqDomainViewport::setDomainBufferSlotRealPercentage(VASTFreqDomainViewport::getDomainBufferSlotRealPercentage(domainBuffer[slot]) + newPerc * sliderValue / 100.f, domainBuffer[slot]);
				slot = 4; //437
				newPerc = +0.9f;
				domainBuffer[slot] = VASTFreqDomainViewport::setDomainBufferSlotRealPercentage(VASTFreqDomainViewport::getDomainBufferSlotRealPercentage(domainBuffer[slot]) + newPerc * sliderValue / 100.f, domainBuffer[slot]);
				slot = 5; //437
				newPerc = +0.1f;
				domainBuffer[slot] = VASTFreqDomainViewport::setDomainBufferSlotRealPercentage(VASTFreqDomainViewport::getDomainBufferSlotRealPercentage(domainBuffer[slot]) + newPerc * sliderValue / 100.f, domainBuffer[slot]);
				slot = 23; //2430
				newPerc = +0.6f;
				domainBuffer[slot] = VASTFreqDomainViewport::setDomainBufferSlotRealPercentage(VASTFreqDomainViewport::getDomainBufferSlotRealPercentage(domainBuffer[slot]) + newPerc * sliderValue / 100.f, domainBuffer[slot]);
				slot = 24; //2430
				newPerc = +0.1f;
				domainBuffer[slot] = VASTFreqDomainViewport::setDomainBufferSlotRealPercentage(VASTFreqDomainViewport::getDomainBufferSlotRealPercentage(domainBuffer[slot]) + newPerc * sliderValue / 100.f, domainBuffer[slot]);
				slot = 28; //3087
				newPerc = +0.4f;
				domainBuffer[slot] = VASTFreqDomainViewport::setDomainBufferSlotRealPercentage(VASTFreqDomainViewport::getDomainBufferSlotRealPercentage(domainBuffer[slot]) + newPerc * sliderValue / 100.f, domainBuffer[slot]);
				slot = 29; //3087
				newPerc = +0.2f;
				domainBuffer[slot] = VASTFreqDomainViewport::setDomainBufferSlotRealPercentage(VASTFreqDomainViewport::getDomainBufferSlotRealPercentage(domainBuffer[slot]) + newPerc * sliderValue / 100.f, domainBuffer[slot]);
				break;
			case 2: //I
				slot = 1;
				newPerc = +0.3f;
				domainBuffer[slot] = VASTFreqDomainViewport::setDomainBufferSlotRealPercentage(VASTFreqDomainViewport::getDomainBufferSlotRealPercentage(domainBuffer[slot]) + newPerc * sliderValue / 100.f, domainBuffer[slot]);
				slot = 3; //361
				newPerc = +0.9f;
				domainBuffer[slot] = VASTFreqDomainViewport::setDomainBufferSlotRealPercentage(VASTFreqDomainViewport::getDomainBufferSlotRealPercentage(domainBuffer[slot]) + newPerc * sliderValue / 100.f, domainBuffer[slot]);
				slot = 4; //361
				newPerc = +0.6f;
				domainBuffer[slot] = VASTFreqDomainViewport::setDomainBufferSlotRealPercentage(VASTFreqDomainViewport::getDomainBufferSlotRealPercentage(domainBuffer[slot]) + newPerc * sliderValue / 100.f, domainBuffer[slot]);
				slot = 24; //2583
				newPerc = +0.5f;
				domainBuffer[slot] = VASTFreqDomainViewport::setDomainBufferSlotRealPercentage(VASTFreqDomainViewport::getDomainBufferSlotRealPercentage(domainBuffer[slot]) + newPerc * sliderValue / 100.f, domainBuffer[slot]);
				slot = 25; //2583
				newPerc = +0.5f;
				domainBuffer[slot] = VASTFreqDomainViewport::setDomainBufferSlotRealPercentage(VASTFreqDomainViewport::getDomainBufferSlotRealPercentage(domainBuffer[slot]) + newPerc * sliderValue / 100.f, domainBuffer[slot]);
				slot = 31; //3378
				newPerc = +0.5f;
				domainBuffer[slot] = VASTFreqDomainViewport::setDomainBufferSlotRealPercentage(VASTFreqDomainViewport::getDomainBufferSlotRealPercentage(domainBuffer[slot]) + newPerc * sliderValue / 100.f, domainBuffer[slot]);
				slot = 32; //3378
				newPerc = +0.1f;
				domainBuffer[slot] = VASTFreqDomainViewport::setDomainBufferSlotRealPercentage(VASTFreqDomainViewport::getDomainBufferSlotRealPercentage(domainBuffer[slot]) + newPerc * sliderValue / 100.f, domainBuffer[slot]);
				break;
			case 3: //O
				slot = 1;
				newPerc = +0.3f;
				domainBuffer[slot] = VASTFreqDomainViewport::setDomainBufferSlotRealPercentage(VASTFreqDomainViewport::getDomainBufferSlotRealPercentage(domainBuffer[slot]) + newPerc * sliderValue / 100.f, domainBuffer[slot]);
				slot = 4; //445
				newPerc = +0.8f;
				domainBuffer[slot] = VASTFreqDomainViewport::setDomainBufferSlotRealPercentage(VASTFreqDomainViewport::getDomainBufferSlotRealPercentage(domainBuffer[slot]) + newPerc * sliderValue / 100.f, domainBuffer[slot]);
				slot = 5; //445
				newPerc = +0.2f;
				domainBuffer[slot] = VASTFreqDomainViewport::setDomainBufferSlotRealPercentage(VASTFreqDomainViewport::getDomainBufferSlotRealPercentage(domainBuffer[slot]) + newPerc * sliderValue / 100.f, domainBuffer[slot]);
				slot = 8; //914
				newPerc = +0.1f;
				domainBuffer[slot] = VASTFreqDomainViewport::setDomainBufferSlotRealPercentage(VASTFreqDomainViewport::getDomainBufferSlotRealPercentage(domainBuffer[slot]) + newPerc * sliderValue / 100.f, domainBuffer[slot]);
				slot = 9; //914
				newPerc = +0.6f;
				domainBuffer[slot] = VASTFreqDomainViewport::setDomainBufferSlotRealPercentage(VASTFreqDomainViewport::getDomainBufferSlotRealPercentage(domainBuffer[slot]) + newPerc * sliderValue / 100.f, domainBuffer[slot]);
				slot = 26; //2900
				newPerc = +0.4f;
				domainBuffer[slot] = VASTFreqDomainViewport::setDomainBufferSlotRealPercentage(VASTFreqDomainViewport::getDomainBufferSlotRealPercentage(domainBuffer[slot]) + newPerc * sliderValue / 100.f, domainBuffer[slot]);
				slot = 27; //2900
				newPerc = +0.5f;
				domainBuffer[slot] = VASTFreqDomainViewport::setDomainBufferSlotRealPercentage(VASTFreqDomainViewport::getDomainBufferSlotRealPercentage(domainBuffer[slot]) + newPerc * sliderValue / 100.f, domainBuffer[slot]);
				break;
			case 4: //U
				slot = 1;
				newPerc = +0.3f;
				domainBuffer[slot] = VASTFreqDomainViewport::setDomainBufferSlotRealPercentage(VASTFreqDomainViewport::getDomainBufferSlotRealPercentage(domainBuffer[slot]) + newPerc * sliderValue / 100.f, domainBuffer[slot]);
				slot = 4; //462
				newPerc = +0.7f;
				domainBuffer[slot] = VASTFreqDomainViewport::setDomainBufferSlotRealPercentage(VASTFreqDomainViewport::getDomainBufferSlotRealPercentage(domainBuffer[slot]) + newPerc * sliderValue / 100.f, domainBuffer[slot]);
				slot = 5; //462
				newPerc = +0.7f;
				domainBuffer[slot] = VASTFreqDomainViewport::setDomainBufferSlotRealPercentage(VASTFreqDomainViewport::getDomainBufferSlotRealPercentage(domainBuffer[slot]) + newPerc * sliderValue / 100.f, domainBuffer[slot]);
				slot = 7; //763
				newPerc = +0.6f;
				domainBuffer[slot] = VASTFreqDomainViewport::setDomainBufferSlotRealPercentage(VASTFreqDomainViewport::getDomainBufferSlotRealPercentage(domainBuffer[slot]) + newPerc * sliderValue / 100.f, domainBuffer[slot]);
				slot = 8; //763
				newPerc = +0.6f;
				domainBuffer[slot] = VASTFreqDomainViewport::setDomainBufferSlotRealPercentage(VASTFreqDomainViewport::getDomainBufferSlotRealPercentage(domainBuffer[slot]) + newPerc * sliderValue / 100.f, domainBuffer[slot]);
				slot = 26; //2903
				newPerc = +0.4f;
				domainBuffer[slot] = VASTFreqDomainViewport::setDomainBufferSlotRealPercentage(VASTFreqDomainViewport::getDomainBufferSlotRealPercentage(domainBuffer[slot]) + newPerc * sliderValue / 100.f, domainBuffer[slot]);
				slot = 2; //2903
				newPerc = +0.4f;
				domainBuffer[slot] = VASTFreqDomainViewport::setDomainBufferSlotRealPercentage(VASTFreqDomainViewport::getDomainBufferSlotRealPercentage(domainBuffer[slot]) + newPerc * sliderValue / 100.f, domainBuffer[slot]);
				break;
			}
			wavetable->setFreqDomainTables(wtPos, &domainBuffer, pregenerate, false, editor->myProcessor->getWTmode()); //CHECK clip
		}
		wavetable->setSelectedWtPos(0);
		break;
	}
	case EditorFunction::HarmonicMorph: {
		int selPos = msbeg;
		std::vector<myFloat> samples = std::vector<myFloat>(C_WAVE_TABLE_SIZE);
		wavetable->clear();
		for (int wtPos = 0; wtPos <= 9; wtPos++) { //2^10 = 1024
			if (wtPos >= wavetable->getNumPositions())
				wavetable->addPosition();

			if (wtPos == 0) {
				samples = *activeWavetable->getNaiveTable(selPos);
				wavetable->setNaiveTable(wtPos, samples, pregenerate, editor->myProcessor->getWTmode());
			}
			else {
				std::vector<dsp::Complex<float>> domainBuffer = *activeWavetable->getFreqDomainBuffer(selPos);
				int slot = pow(2, wtPos);
				if (slot <= 1024) {
					float newPerc = VASTFreqDomainViewport::getDomainBufferSlotRealPercentage(domainBuffer[1]) * (sliderValue) / 100.0f;
					domainBuffer[1] = VASTFreqDomainViewport::setDomainBufferSlotRealPercentage(0.f, domainBuffer[1]);
					domainBuffer[slot] = VASTFreqDomainViewport::setDomainBufferSlotRealPercentage(newPerc, domainBuffer[slot]);
					wavetable->setFreqDomainTables(wtPos, &domainBuffer, pregenerate, false, editor->myProcessor->getWTmode()); //CHECK clip
				}
			}
		}
		wavetable->setSelectedWtPos(0);
		break;
	}
	case EditorFunction::HarmonicReduce: {
		int selPos = msbeg;
		std::vector<myFloat> samples = std::vector<myFloat>(C_WAVE_TABLE_SIZE);
		wavetable->clear();
		for (int wtPos = 0; wtPos <= 9; wtPos++) { //2^10 = 1024
			if (wtPos >= wavetable->getNumPositions())
				wavetable->addPosition();

			if (wtPos == 0) {
				samples = *activeWavetable->getNaiveTable(selPos);
				wavetable->setNaiveTable(wtPos, samples, pregenerate, editor->myProcessor->getWTmode());
			}
			else {
				std::vector<dsp::Complex<float>> domainBuffer = *activeWavetable->getFreqDomainBuffer(selPos);
				for (int j = 0; j < wtPos; j++) {
					int slot = pow(2, j);
					if (slot <= 1024) {
						float newPerc = VASTFreqDomainViewport::getDomainBufferSlotRealPercentage(domainBuffer[slot]) * (100.f - sliderValue) / 100.0f;
						domainBuffer[slot] = VASTFreqDomainViewport::setDomainBufferSlotRealPercentage(newPerc, domainBuffer[slot]);
						wavetable->setFreqDomainTables(wtPos, &domainBuffer, pregenerate, false, editor->myProcessor->getWTmode()); //CHECK clip
					}
				}
			}
		}
		wavetable->setSelectedWtPos(0);
		break;
	}
	case EditorFunction::HighpassFilter: {
		int selPos = msbeg;
		std::vector<myFloat> samples = std::vector<myFloat>(C_WAVE_TABLE_SIZE);
		wavetable->clear();
		const int cmax = 64; //64 is arbitrary
		for (int wtPos = 0; wtPos < cmax - 1; wtPos++) {
			if (wtPos >= wavetable->getNumPositions())
				wavetable->addPosition();

			if (wtPos == 0) {
				samples = *activeWavetable->getNaiveTable(selPos);
				wavetable->setNaiveTable(wtPos, samples, pregenerate, editor->myProcessor->getWTmode());
			}
			else {
				std::vector<dsp::Complex<float>> domainBuffer = *activeWavetable->getFreqDomainBuffer(selPos);
				for (int j = 1; j <= wtPos; j++) {
					//int slot = j;
					int slot = float(j) * (sliderValue / 100.0);
					//float newPerc = VASTFreqDomainViewport::getDomainBufferSlotRealPercentage(domainBuffer[slot]) * (100.f - sliderValue) / 100.0f;
					float newPerc = 0.f;
					domainBuffer[slot] = VASTFreqDomainViewport::setDomainBufferSlotRealPercentage(newPerc, domainBuffer[slot]);
				}
				wavetable->setFreqDomainTables(wtPos, &domainBuffer, pregenerate, false, editor->myProcessor->getWTmode()); //CHECK clip
			}
		}
		wavetable->setSelectedWtPos(0);
		break;
	}
	case EditorFunction::LowpassFilter: {
		int selPos = msbeg;
		std::vector<myFloat> samples = std::vector<myFloat>(C_WAVE_TABLE_SIZE);
		wavetable->clear();
		const int cmax = 64; //64 is arbitrary
		for (int wtPos = 0; wtPos < cmax - 1; wtPos++) {
			if (wtPos >= wavetable->getNumPositions())
				wavetable->addPosition();

			if (wtPos == 0) {
				samples = *activeWavetable->getNaiveTable(selPos);
				wavetable->setNaiveTable(wtPos, samples, pregenerate, editor->myProcessor->getWTmode());
			}
			else {
				std::vector<dsp::Complex<float>> domainBuffer = *activeWavetable->getFreqDomainBuffer(selPos);
				for (int i = cmax; i < C_WAVE_TABLE_SIZE / 2; i++)
					domainBuffer[i] = VASTFreqDomainViewport::setDomainBufferSlotRealPercentage(0.f, domainBuffer[i]);
				for (int j = 1; j <= wtPos; j++) {
					//int slot = cmax - j;
					int slot = cmax - float(j) * (sliderValue / 100.0);
					//float newPerc = VASTFreqDomainViewport::getDomainBufferSlotRealPercentage(domainBuffer[slot]) * (100.f - sliderValue) / 100.0f;
					float newPerc = 0.f;
					domainBuffer[slot] = VASTFreqDomainViewport::setDomainBufferSlotRealPercentage(newPerc, domainBuffer[slot]);
				}
				wavetable->setFreqDomainTables(wtPos, &domainBuffer, pregenerate, false, editor->myProcessor->getWTmode()); //CHECK clip
			}
		}
		wavetable->setSelectedWtPos(0);
		break;
	}
	case EditorFunction::LowpassSoft: {
		int selPos = msbeg;
		std::vector<myFloat> samples = std::vector<myFloat>(C_WAVE_TABLE_SIZE);
		wavetable->clear();
		const int cmax = 64; //64 is arbitrary
		for (int wtPos = 0; wtPos < cmax - 1; wtPos++) {
			if (wtPos >= wavetable->getNumPositions())
				wavetable->addPosition();

			if (wtPos == 0) {
				samples = *activeWavetable->getNaiveTable(selPos);
				wavetable->setNaiveTable(wtPos, samples, pregenerate, editor->myProcessor->getWTmode());
			}
			else {
 				std::vector<myFloat> samples = std::vector<myFloat>(C_WAVE_TABLE_SIZE);
				std::vector<myFloat> samplescopy = std::vector<myFloat>(C_WAVE_TABLE_SIZE);
				samples = *wavetable->getNaiveTable(wtPos - 1);
				samplescopy = *wavetable->getNaiveTable(wtPos - 1);

				float smoothval = sliderValue * (float(wtPos) / cmax); //less in the beginning more later

				float local_max_before = 0.0f; //for normalize
				float local_max_after = 0.0f; //for normalize
				float e = ((smoothval + .5f) / 100.f) * 100; //no zero values
				int r = 0;
				float s = 2 * e + 1;
				for (int i = wtselstart; i <= wtselend; i++) {
					if (!(s <= 2)) {
						//if (!(s <= 1)) {
						float o = 0;
						for (int n = 0; n < s; n++) {
							r = int(i - e + n + C_WAVE_TABLE_SIZE) % C_WAVE_TABLE_SIZE;
							o += samplescopy[r];
						}
						samples[i] = o / s;
					}

					float sample = abs(samplescopy[i]); //for normalize
					if (sample > local_max_before) local_max_before = sample; //for normalize
					if (abs(samples[i]) > local_max_after) local_max_after = abs(samples[i]); //for normalize
				}

				//normalize
				float gain = 1.f;
				if ((local_max_after != 0.f) && (local_max_before != 0.f))
					gain = 1.f / (local_max_after / local_max_before);
				for (int i = wtselstart; i <= wtselend; i++) {
					samples[i] *= gain;
				}
				wavetable->setNaiveTable(wtPos, samples, pregenerate, editor->myProcessor->getWTmode());
			}
		}
		wavetable->setSelectedWtPos(0);
		break;
	}
	case EditorFunction::BandpassFilter: {
		int selPos = msbeg;
		std::vector<myFloat> samples = std::vector<myFloat>(C_WAVE_TABLE_SIZE);
		wavetable->clear();
		const int cmax = 64; //64 is arbitrary
		for (int wtPos = 0; wtPos < cmax / 3; wtPos++) {
			if (wtPos >= wavetable->getNumPositions())
				wavetable->addPosition();

			if (wtPos == 0) {
				samples = *activeWavetable->getNaiveTable(selPos);
				wavetable->setNaiveTable(wtPos, samples, pregenerate, editor->myProcessor->getWTmode());
			}
			else {
				std::vector<dsp::Complex<float>> domainBuffer = *activeWavetable->getFreqDomainBuffer(selPos);
				for (int i = cmax; i < C_WAVE_TABLE_SIZE; i++)
					domainBuffer[i] = VASTFreqDomainViewport::setDomainBufferSlotRealPercentage(0.f, domainBuffer[i]);
				for (int j = 1; j <= wtPos; j++) {
					//int slot = cmax - j;
					int slot = cmax - float(j) * (sliderValue / 100.0);
					//float newPerc = VASTFreqDomainViewport::getDomainBufferSlotRealPercentage(domainBuffer[slot]) * (100.f - sliderValue) / 100.0f;
					float newPerc = 0.f;
					domainBuffer[slot] = VASTFreqDomainViewport::setDomainBufferSlotRealPercentage(newPerc, domainBuffer[slot]);
					//slot = j;
					slot = float(j) * (sliderValue / 100.0);
					//newPerc = VASTFreqDomainViewport::getDomainBufferSlotRealPercentage(domainBuffer[slot]) * (100.f - sliderValue) / 100.0f;
					newPerc = 0.f;
					domainBuffer[slot] = VASTFreqDomainViewport::setDomainBufferSlotRealPercentage(newPerc, domainBuffer[slot]);
				}
				wavetable->setFreqDomainTables(wtPos, &domainBuffer, pregenerate, false, editor->myProcessor->getWTmode()); //CHECK clip
			}
		}
		wavetable->setSelectedWtPos(0);
		break;
	}
	case EditorFunction::PulseFilter: {
		int selPos = msbeg;
		std::vector<myFloat> samples = std::vector<myFloat>(C_WAVE_TABLE_SIZE);
		wavetable->clear();
		const int cmax = 64; //64 is arbitrary
		for (int wtPos = 0; wtPos < cmax; wtPos++) {
			if (wtPos >= wavetable->getNumPositions())
				wavetable->addPosition();

			if (wtPos == 0) {
				samples = *activeWavetable->getNaiveTable(selPos);
				wavetable->setNaiveTable(wtPos, samples, pregenerate, editor->myProcessor->getWTmode());
			}
			else {
				std::vector<dsp::Complex<float>> domainBuffer = *activeWavetable->getFreqDomainBuffer(selPos);
				for (int j = 1 + (cmax - wtPos); j <= 1024; j += ((cmax - wtPos) + 1)) {
					int slot = j;
					float newPerc = VASTFreqDomainViewport::getDomainBufferSlotRealPercentage(domainBuffer[slot]) * (100.f - sliderValue) / 100.0f;
					domainBuffer[slot] = VASTFreqDomainViewport::setDomainBufferSlotRealPercentage(newPerc, domainBuffer[slot]);
				}
				wavetable->setFreqDomainTables(wtPos, &domainBuffer, pregenerate, false, editor->myProcessor->getWTmode()); //CHECK clip
			}
		}
		wavetable->setSelectedWtPos(0);
		break;
	}
	case EditorFunction::CombFilter: {
		int selPos = msbeg;
		std::vector<myFloat> samples = std::vector<myFloat>(C_WAVE_TABLE_SIZE);
		wavetable->clear();
		const int cmax = 64; //64 is arbitrary
		for (int wtPos = 0; wtPos < cmax; wtPos++) {
			if (wtPos >= wavetable->getNumPositions())
				wavetable->addPosition();

			if (wtPos == 0) {
				samples = *activeWavetable->getNaiveTable(selPos);
				wavetable->setNaiveTable(wtPos, samples, pregenerate, editor->myProcessor->getWTmode());
			}
			else {
				std::vector<dsp::Complex<float>> domainBuffer = *activeWavetable->getFreqDomainBuffer(selPos);
				int step = (100 - sliderValue) / 10.f;
				for (int j = 0; j < (step + 2) * wtPos; j += step + 2) {
					int slot = j + 2;
					domainBuffer[slot] = VASTFreqDomainViewport::setDomainBufferSlotRealPercentage(0.f, domainBuffer[slot]);
				}
				wavetable->setFreqDomainTables(wtPos, &domainBuffer, pregenerate, false, editor->myProcessor->getWTmode()); //CHECK clip
			}
		}
		wavetable->setSelectedWtPos(0);
		break;
	}
	//============================================================================================================
	// Quick select icons
	//============================================================================================================
	case EditorFunction::Sine: {
		std::vector<myFloat> samples = std::vector<myFloat>(C_WAVE_TABLE_SIZE);
		for (int wtPos = msbeg; wtPos <= msend; wtPos++) {
			samples = (*editor->getCurWavetable()->getNaiveTable(wtPos));
			float e = 2 * float(M_PI);
			int j = 0;
			int end = wtselend - wtselstart + 1;
			for (int i = wtselstart; i <= wtselend; i++) {
				samples[i] = sinf(e * float(j) / float(end));
				j++;
			}
			wavetable->setNaiveTable(wtPos, samples, false, editor->myProcessor->getWTmode());
		}
		break;
	}
	case EditorFunction::Zeros: {
		std::vector<myFloat> samples = std::vector<myFloat>(C_WAVE_TABLE_SIZE);
		for (int wtPos = msbeg; wtPos <= msend; wtPos++) {
			samples = (*editor->getCurWavetable()->getNaiveTable(wtPos));
			int j = 0;
			for (int i = wtselstart; i <= wtselend; i++) {
				samples[i] = 0;
				j++;
			}
			wavetable->setNaiveTable(wtPos, samples, false, editor->myProcessor->getWTmode());
		}
		break;
	}
	case EditorFunction::Sawtooth: {
		std::vector<myFloat> samples = std::vector<myFloat>(C_WAVE_TABLE_SIZE);
		for (int wtPos = msbeg; wtPos <= msend; wtPos++) {
			samples = (*editor->getCurWavetable()->getNaiveTable(wtPos));
			int j = 0;
			int end = wtselend - wtselstart + 1;
			for (int i = wtselstart; i <= wtselend; i++) {
				if (j < end / 2)
					samples[i] = 2.0f * (1.0f / end * j);
				else
					samples[i] = -2.0f + 2.0f * (1.0f / end * j);
				j++;
			}
			wavetable->setNaiveTable(wtPos, samples, false, editor->myProcessor->getWTmode());
		}
		break;
	}
	case EditorFunction::Sinc: {
		std::vector<myFloat> samples = std::vector<myFloat>(C_WAVE_TABLE_SIZE);
		for (int wtPos = msbeg; wtPos <= msend; wtPos++) {
			//sinc = r33,33% - i75% / 25%
			samples = (*editor->getCurWavetable()->getNaiveTable(wtPos));
			int j = 0;
			int end = wtselend - wtselstart + 1;
			float compress = M_PI * 22.f * (float(end) / float(C_WAVE_TABLE_SIZE));
			for (int i = wtselstart; i <= wtselend; i++) {
				float pos = j / compress - end * 0.5f / compress;
				float sinc = 1.f;
				if (pos != 0.f)
					sinc = sin(M_PI * pos) / (M_PI * pos);   //sinc(x) = sin(pi x) / pi x // center of wavetable is 0

				samples[i] = sinc;
				j++;
			}

			//smooth
			/*
			std::vector<myFloat> samplescopy = std::vector<myFloat>(C_WAVE_TABLE_SIZE);
			samplescopy = samples;

			float smoothvalue = 10.f;
			float e = ((smoothvalue + .5f) / 100.f) * 100; //no zero values
			int r = 0;
			float s = 2 * e + 1;
			for (int i = wtselstart; i <= wtselend; i++) {
				if (!(s <= 2)) {
					//if (!(s <= 1)) {
					float o = 0;
					for (int n = 0; n < s; n++) {
						r = int(i - e + n + C_WAVE_TABLE_SIZE) % C_WAVE_TABLE_SIZE;
						o += samplescopy[r];
					}
					samples[i] = o / s;
				}
			}
			*/

			wavetable->setNaiveTable(wtPos, samples, false, editor->myProcessor->getWTmode());
		}
		break;
	}
	case EditorFunction::HalfSine: {
		std::vector<myFloat> samples = std::vector<myFloat>(C_WAVE_TABLE_SIZE);
		for (int wtPos = msbeg; wtPos <= msend; wtPos++) {
			samples = (*editor->getCurWavetable()->getNaiveTable(wtPos));
			float e = 2 * float(M_PI);
			int j = 0;
			int end = wtselend - wtselstart + 1;
			for (int i = wtselstart; i <= wtselend; i++) {
				samples[i] = sinf(0.5f * e * float(j) / float(end));
				j++;
			}
			wavetable->setNaiveTable(wtPos, samples, false, editor->myProcessor->getWTmode());
		}
		break;
	}
	case EditorFunction::Square: {
		std::vector<myFloat> samples = std::vector<myFloat>(C_WAVE_TABLE_SIZE);
		for (int wtPos = msbeg; wtPos <= msend; wtPos++) {
			samples = (*editor->getCurWavetable()->getNaiveTable(wtPos));
			int j = 0;
			int end = wtselend - wtselstart + 1;
			for (int i = wtselstart; i <= wtselend; i++) {
				if (j < end / 2)
					samples[i] = 1.0;
				else
					samples[i] = -1.0;
				j++;
			}
			wavetable->setNaiveTable(wtPos, samples, false, editor->myProcessor->getWTmode());
		}
		break;
	}
	case EditorFunction::Triangle: {
		std::vector<myFloat> samples = std::vector<myFloat>(C_WAVE_TABLE_SIZE);
		for (int wtPos = msbeg; wtPos <= msend; wtPos++) {
			samples = (*editor->getCurWavetable()->getNaiveTable(wtPos));
			int j = 0;
			int end = wtselend - wtselstart + 1;
			for (int i = wtselstart; i <= wtselend; i++) {
				samples[i] = -1.0f + (2.0f / (end / 2)) * (end / 2 - abs((j + end / 4) % end - end / 2));
				j++;
			}
			wavetable->setNaiveTable(wtPos, samples, false, editor->myProcessor->getWTmode());
		}
		break;
	}
	case EditorFunction::Ramp: {
		std::vector<myFloat> samples = std::vector<myFloat>(C_WAVE_TABLE_SIZE);
		for (int wtPos = msbeg; wtPos <= msend; wtPos++) {
			samples = (*editor->getCurWavetable()->getNaiveTable(wtPos));
			int j = 0;
			int end = wtselend - wtselstart + 1;
			for (int i = wtselstart; i <= wtselend; i++) {
				samples[i] = -1.0f + 2 * float(j) / float(end);
				j++;
			}
			wavetable->setNaiveTable(wtPos, samples, false, editor->myProcessor->getWTmode());
		}
		break;
	}
	case EditorFunction::Random: {
		juce::Random rnd = juce::Random();
		std::vector<myFloat> samples = std::vector<myFloat>(C_WAVE_TABLE_SIZE);
		for (int wtPos = msbeg; wtPos <= msend; wtPos++) {
			samples = (*editor->getCurWavetable()->getNaiveTable(wtPos));
			int j = 0;
			for (int i = wtselstart; i <= wtselend; i++) {
				samples[i] = 2 * rnd.nextDouble() - 1;
				j++;
			}
			wavetable->setNaiveTable(wtPos, samples, false, editor->myProcessor->getWTmode());
		}
		break;
	}
	case EditorFunction::SampleHold: {
		juce::Random rnd = juce::Random();
		std::vector<myFloat> samples = std::vector<myFloat>(C_WAVE_TABLE_SIZE);
		for (int wtPos = msbeg; wtPos <= msend; wtPos++) {
			samples = (*editor->getCurWavetable()->getNaiveTable(wtPos));
			int length = -1;
			float height = 0.f;
			int realLength = 0;
			for (int i = wtselstart; i <= wtselend; i++) {
				if (realLength >= length) {
					length = rnd.nextDouble() * C_WAVE_TABLE_SIZE * 0.25; //maximum 25% of table
					height = rnd.nextDouble() * 2 - 1;
					realLength = 0;
				}
				realLength++;
				samples[i] = height;
			}
			wavetable->setNaiveTable(wtPos, samples, false, editor->myProcessor->getWTmode());
		}
		break;
	}
	case EditorFunction::Invert: {
		std::vector<myFloat> samples = std::vector<myFloat>(C_WAVE_TABLE_SIZE);
		for (int wtPos = msbeg; wtPos <= msend; wtPos++) {
			samples = (*editor->getCurWavetable()->getNaiveTable(wtPos));
			int j = 0;
			for (int i = wtselstart; i <= wtselend; i++) {
				samples[i] = -1.0 * (*activeWavetable->getNaiveTable(wtPos))[i];
				j++;
			}
			wavetable->setNaiveTable(wtPos, samples, false, editor->myProcessor->getWTmode());
		}
		break;
	}
	case EditorFunction::Reverse: {
		std::vector<myFloat> samples = std::vector<myFloat>(C_WAVE_TABLE_SIZE);
		for (int wtPos = msbeg; wtPos <= msend; wtPos++) {
			samples = (*editor->getCurWavetable()->getNaiveTable(wtPos));
			int j = 0;
			int end = wtselend - wtselstart;
			for (int i = wtselstart; i <= wtselend; i++) {
				samples[i] = (*activeWavetable->getNaiveTable(wtPos))[wtselstart + end - j];
				j++;
			}
			wavetable->setNaiveTable(wtPos, samples, false, editor->myProcessor->getWTmode());
		}
		break;
	}
	case EditorFunction::ArcConnect: {
		std::vector<myFloat> samples = std::vector<myFloat>(C_WAVE_TABLE_SIZE);
		for (int wtPos = msbeg; wtPos <= msend; wtPos++) {
			samples = (*editor->getCurWavetable()->getNaiveTable(wtPos));
			int j = 0;
			float val1 = (*editor->getCurWavetable()->getNaiveTable(wtPos))[wtselstart];
			float val2 = (*editor->getCurWavetable()->getNaiveTable(wtPos))[wtselend];
			float e = 2 * float(M_PI);
			int end = wtselend - wtselstart;
			for (int i = wtselstart; i <= wtselend; i++) {
				samples[i] = val1 + (val2 - val1) * float(j) / float(end) *  sinf(0.25f * e * float(j) / float(end));
				if (samples[i] > 1) samples[i] = 1;
				if (samples[i] < -1) samples[i] = -1;
				j++;
			}
			wavetable->setNaiveTable(wtPos, samples, false, editor->myProcessor->getWTmode());
		}
		break;
	}
	case EditorFunction::RampConnect: {
		std::vector<myFloat> samples = std::vector<myFloat>(C_WAVE_TABLE_SIZE);
		for (int wtPos = msbeg; wtPos <= msend; wtPos++) {
			samples = (*editor->getCurWavetable()->getNaiveTable(wtPos));
			float val1 = (*editor->getCurWavetable()->getNaiveTable(wtPos))[wtselstart];
			float val2 = (*editor->getCurWavetable()->getNaiveTable(wtPos))[wtselend];
			int j = 0;
			int end = wtselend - wtselstart;
			for (int i = wtselstart; i <= wtselend; i++) {
				samples[i] = val1 + (val2 - val1) * float(j) / float(end);
				j++;
			}
			wavetable->setNaiveTable(wtPos, samples, false, editor->myProcessor->getWTmode());
		}
		break;
	}
	//============================================================================================================
	break;
	}

#ifdef _DEBUG
	std::chrono::high_resolution_clock::time_point t3 = std::chrono::high_resolution_clock::now();
#endif

	if (transactionWavetable == nullptr) {
		if (lIndexAtStart != editor->myProcessor->m_loadedPresetIndexCount) //changed preset in between
			VDBG("Not merged! Preset changed in meantime!");
		else {
			wavetable->deleteGeneratedContent(); //all buffers are invalid now
			editor->myProcessor->m_pVASTXperience.m_Poly.m_OscBank[lBank].setWavetableSoftFade(wavetable);
		}
		editor->myProcessor->m_pVASTXperience.m_Poly.m_OscBank[lBank].removeSoftFadeEditor();
	}

#ifdef _DEBUG
	std::chrono::high_resolution_clock::time_point t4 = std::chrono::high_resolution_clock::now();
	auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() * 0.001;
	auto duration2 = std::chrono::duration_cast<std::chrono::microseconds>(t3 - t2).count() * 0.001;
	auto duration3 = std::chrono::duration_cast<std::chrono::microseconds>(t4 - t3).count() * 0.001;
	VDBG("Thread runtime (milliseconds): " << duration1 << " " << duration2 << " " << duration3);
#endif
 	getEditorThreadsRunning()--;
}

void VASTWaveTableEditorComponent::textEditorTextChanged(TextEditor& textEditorThatWasChanged) {
	if (&textEditorThatWasChanged == c_waveTableEditorView->getHeader()->getWTNameBox())
	{
		myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_bank].getWavetablePointer()->setWaveTableName(c_waveTableEditorView->getHeader()->getWTNameBox()->getText());
		m_cur_wavetable->setWaveTableName(c_waveTableEditorView->getHeader()->getWTNameBox()->getText());
	}
	else if (&textEditorThatWasChanged == c_samplerEditorComponent->getHeader()->getWAVNameBox())
	{
		VASTSamplerSound* sound = myProcessor->m_pVASTXperience.m_Poly.getSamplerSoundChanged();
		if (sound != nullptr)
			sound->setName(c_samplerEditorComponent->getHeader()->getWAVNameBox()->getText());
		sound = myProcessor->m_pVASTXperience.m_Poly.getSamplerSound();
		if (sound != nullptr)
			sound->setName(c_samplerEditorComponent->getHeader()->getWAVNameBox()->getText());
	}
}

void VASTWaveTableEditorComponent::loadWTFileThreaded(juce::File file) {
	newWTToUpdate = nullptr;
	std::thread load_thread(&VASTWaveTableEditorComponent::loadWTFileThread, file, this);
	newWTToUpdate_failed = false;
	load_thread.detach();
	startTimer(1, 200);
}

void VASTWaveTableEditorComponent::saveWTFileThreaded(juce::File file, CVASTWaveTable* wavetable) {
	StringPairArray sarray;
	file.deleteFile(); //for overwrite
	std::thread save_thread(&VASTWaveTableEditorComponent::saveWTFileThread, file, wavetable, sarray, this);
	save_thread.detach();
}

void VASTWaveTableEditorComponent::loadWTFileThread(juce::File file, VASTWaveTableEditorComponent* editor) { //this actually loads a WT
	//expensive part
	std::unique_ptr<AudioFormatManager> formatManager(new AudioFormatManager());
	formatManager->registerBasicFormats();
    std::unique_ptr<AudioFormatReader> reader(formatManager->createReaderFor(file)); //use first hit
	if (reader != nullptr)
	{
		int len = static_cast<int>(reader->lengthInSamples);
		int wTIndex = 0;
		//serum format has always 2048 samples

        std::unique_ptr<wavFile> wFile(new wavFile);
		memset(wFile->WTBuffer, 0, sizeof(wFile->WTBuffer)); //init

		while ((len >= C_WAV_FORMAT_WT_SIZE) && (wTIndex < C_WAV_FORMAT_WT_SIZE)) {
			len -= C_WAV_FORMAT_WT_SIZE;
			AudioSampleBuffer fileBuffer;
			fileBuffer.setSize(1, C_WAV_FORMAT_WT_SIZE);
			reader->read(&fileBuffer,
				0,
				C_WAV_FORMAT_WT_SIZE,
				C_WAV_FORMAT_WT_SIZE * wTIndex, //offset
				true,
				true);
			wFile->fullpathname = file.getFullPathName();
			wFile->wtname = file.getFileName();
			for (int i = 0; i < C_WAV_FORMAT_WT_SIZE; i++) {
				wFile->WTBuffer[wTIndex][i] = fileBuffer.getSample(0, i);
			}
			wTIndex++;
		}
		wFile->length = wTIndex;

		if ((len != 0) || (wFile->length > C_MAX_NUM_POSITIONS) || (wFile->length == 0)) {
			editor->newWTToUpdate_failed = true;
		}
		else {
			std::shared_ptr<CVASTWaveTable> l_wavetable(new CVASTWaveTable(editor->myProcessor->m_pVASTXperience.m_Set));
			l_wavetable->loadWavFile(wFile.get(), true, editor->myProcessor->getWTmode()); //pre generate!
			editor->newWTToUpdate = l_wavetable;
			editor->newWTToUpdate_failed = false;
		}
	}
	//expensive part
	editor->setWtPos(0);
}

template <typename Word>
void VASTWaveTableEditorComponent::write_word(FileOutputStream* outs, Word value, unsigned size)
{
	for (; size; --size, value >>= 8)
		outs->writeByte((char)(value & 0xFF));
}

void VASTWaveTableEditorComponent::saveWTFileThread(juce::File file, CVASTWaveTable* wavetable, StringPairArray sarray, VASTWaveTableEditorComponent* editor) { //this actually saves a WT
	//expensive part
	// Write the file headers
	std::unique_ptr<FileOutputStream> fos(file.createOutputStream());
	if (fos == nullptr)
		return;
	fos->writeText("RIFF----WAVEfmt ", false, false, nullptr);     // (chunk size to be filled in later)

	editor->write_word(fos.get(), 16, 4);  // no extension data
	editor->write_word(fos.get(), 3, 2);  // type 3
	editor->write_word(fos.get(), 1, 2);  // two channels (stereo file)
	editor->write_word(fos.get(), 44100, 4);  // samples per second (Hz)
	editor->write_word(fos.get(), 176400, 4);  // (Sample Rate * BitsPerSample * Channels) / 8
	editor->write_word(fos.get(), 4, 2);  // data block size (size of two integer samples, one for each channel, in bytes)
	editor->write_word(fos.get(), 32, 2);  // number of bits per sample (use a multiple of 8)

	//Write Serum header
	fos->writeText("clm 0", false, false, nullptr);
	fos->writeByte(0);
	fos->writeByte(0);
	fos->writeByte(0);
	fos->writeText("<!>2048 00000000 wavetable ( vast-dynamics.com )", false, false, nullptr);

	// Write the data chunk header
	size_t data_chunk_pos = fos->getPosition();
	fos->writeText("data----", false, false, nullptr);

	for (int wtPos = 0; wtPos < wavetable->getNumPositions(); wtPos++) {
		for (int i = 0; i < C_WAVE_TABLE_SIZE; i++) {
			float val = (*wavetable->getNaiveTable(wtPos))[i];
			int* ival = reinterpret_cast<int*>(&val);
			editor->write_word(fos.get(), *ival, 4);
		}
	}
	size_t file_length = fos->getPosition();
	// Fix the data chunk header to contain the data size
	fos->setPosition(data_chunk_pos + 4);
	editor->write_word(fos.get(), file_length - data_chunk_pos - 8, 4);

	// Fix the file header to contain the proper RIFF chunk size, which is (file size - 8) bytes
	fos->setPosition(0 + 4);
	editor->write_word(fos.get(), file_length - 4, 4);
	fos->flush();
	//expensive part
}

void VASTWaveTableEditorComponent::startWTRecording() {
	m_bIsRecordingWT = true;
	startTimer(2, 500); //2 is record timer
	Drawable *img = c_waveTableEditorView->getHeader()->c_iconRecordWT.get()->getNormalImage();
	img->replaceColour(Colours::white, Colours::red);
	img = c_waveTableEditorView->getHeader()->c_iconRecordWT.get()->getOverImage();
	img->replaceColour(Colours::white, Colours::red);
	m_bIsRecordingWTBlink = true;
	c_waveTableEditorView->getHeader()->c_iconRecordWT.get()->repaint();
	m_iRecordingOscBank = getOscBank();
	myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_iRecordingOscBank].startRecording(getWtPos());
}

void VASTWaveTableEditorComponent::stopWTRecording() {
	if (m_bIsRecordingWT) {
		m_bIsRecordingWT = false;
		stopTimer(2); //2 is record timer
		Drawable *img = c_waveTableEditorView->getHeader()->c_iconRecordWT.get()->getNormalImage();
		img->replaceColour(Colours::red, Colours::white);
		img = c_waveTableEditorView->getHeader()->c_iconRecordWT.get()->getOverImage();
		img->replaceColour(Colours::red, Colours::white);
		c_waveTableEditorView->getHeader()->c_iconRecordWT.get()->repaint();
		myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_iRecordingOscBank].stopRecording();
		m_iRecordingOscBank = -1;
	}
}

void VASTWaveTableEditorComponent::timerCallback(int timerID) {
	vassert(getWtPos() == getBankWavetable()->getSelectedWtPos());
	//vassert(getMultiSelectBegin() == getBankWavetable()->getMultiSelectBegin());
	//vassert(getMultiSelectEnd() == getBankWavetable()->getMultiSelectEnd());
	if (timerID == 0) { // UI update timer
		if (m_needsUIUpdate) {
			if (isShowing()) {
				updateAll(false);
				m_needsUIUpdate = false;
			}
		}
	}
	else if (timerID == 1) { //wt load timer
		if (newWTToUpdate_failed == true) {
			AlertWindow::showMessageBoxAsync(MessageBoxIconType::InfoIcon, TRANS("Failed"), TRANS("The wavetable file has an invalid structure. Use a WAV file mith max 256 chunks of 2048 mono samples."), TRANS("Continue"), this);
			stopTimer(1);
			requestUIUpdate();
			m_positionviewport->setZoomFactor(c_waveTableEditorView->getHeader()->getWTEditorZoom()->getValue());
		}

		if (newWTToUpdate == nullptr) return;
		vassert(newWTToUpdate->getNumPositions() > 0);
		myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_bank].addSoftFadeEditor();
		myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_bank].setWavetableSoftFade(newWTToUpdate);
		myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_bank].removeSoftFadeEditor();
		//delete newWTToUpdate;
		//newWTToUpdate = nullptr;
		requestUIUpdate();
		m_positionviewport->setZoomFactor(c_waveTableEditorView->getHeader()->getWTEditorZoom()->getValue());
		stopTimer(1);
	}
	if (timerID == 2) { // record blink timer
		if (m_bIsRecordingWT) {
			m_bIsRecordingWTBlink = !m_bIsRecordingWTBlink;
			if (m_bIsRecordingWTBlink) {
				Drawable *img = c_waveTableEditorView->getHeader()->c_iconRecordWT.get()->getNormalImage();
				img->replaceColour(Colours::white, Colours::red);
				img = c_waveTableEditorView->getHeader()->c_iconRecordWT.get()->getOverImage();
				img->replaceColour(Colours::white, Colours::red);
			}
			else {
				Drawable *img = c_waveTableEditorView->getHeader()->c_iconRecordWT.get()->getNormalImage();
				img->replaceColour(Colours::red, Colours::white);
				img = c_waveTableEditorView->getHeader()->c_iconRecordWT.get()->getOverImage();
				img->replaceColour(Colours::red, Colours::white);
			}
			c_waveTableEditorView->getHeader()->c_iconRecordWT.get()->repaint();

			if (myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_iRecordingOscBank].getWavetablePointer()->getNumPositions() >= C_MAX_NUM_POSITIONS)
				stopWTRecording();
		} else
			stopWTRecording();
	}
}

void VASTWaveTableEditorComponent::deletePositions(bool onlyOdds) {
	stopWTRecording();
	myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_bank].addSoftFadeEditor();
	std::shared_ptr<CVASTWaveTable> wavetable = myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_bank].getSoftOrCopyWavetable(true); //need a copy

	int beg = wavetable->getMultiSelectBegin();
	int end = wavetable->getMultiSelectEnd();
	if (wavetable->isMultiSelected()) {
		for (int wtPos = end; wtPos >= beg; wtPos--) {
			if ((onlyOdds) && (wtPos % 2 == 0)) continue;
			wavetable->deletePosition(wtPos);
		}
		if (wavetable->getNumPositions() == 0) wavetable->addPosition();
		if ((beg - 1) >= 0) {
			wavetable->setSelectedWtPos(beg - 1);
			//setWtPos(beg - 1);
		}
		else {
			wavetable->setSelectedWtPos(0);
			//setWtPos(0);
		}
		//setCurWavetable(wavetable); //make a copy
	}
	else {
		int curPos = wavetable->getSelectedWtPos();
		if (!((onlyOdds) && (curPos % 2 == 0))) {
			wavetable->deletePosition(curPos);
			if (wavetable->getNumPositions() == 0) wavetable->addPosition();
			if (curPos > 0) {
				//setWtPos(curPos - 1);
				wavetable->setSelectedWtPos(curPos - 1);
			}
			else {
				//setWtPos(0);
				wavetable->setSelectedWtPos(0);
			}
			//setCurWavetable(wavetable); //make a copy
		}
	}
	if (onlyOdds) {
		//multiSelectAll
		wavetable->setSelection(beg, beg + (end - beg) / 2);
	}
	myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_bank].setWavetableSoftFade(wavetable);
	myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_bank].removeSoftFadeEditor();
	//m_positionviewport->setZoomFactor(c_waveTableEditorView->getHeader()->getWTEditorZoom()->getValue());
}

void VASTWaveTableEditorComponent::copyPositions() {
	stopWTRecording();

	std::shared_ptr<CVASTWaveTable> wavetable = getBankWavetable();
	std::shared_ptr<CVASTWaveTable> copypastewavetable(new CVASTWaveTable(myProcessor->m_pVASTXperience.m_Set));
	if (wavetable->isMultiSelected()) {
		int beg = wavetable->getMultiSelectBegin();
		int end = wavetable->getMultiSelectEnd();
		//for (int wtPos = end; wtPos >= beg; wtPos--) {
		for (int wtPos = beg; wtPos <= end; wtPos++) {
			wavetable->copyPositionToOtherWavetable(wtPos, copypastewavetable->getNumPositions(), copypastewavetable.get());
		}
	}
	else {
		int curPos = wavetable->getSelectedWtPos();
		wavetable->copyPositionToOtherWavetable(curPos, copypastewavetable->getNumPositions(), copypastewavetable.get());
	}
	if (copypastewavetable->getNumPositions() > 0) {
		setCopyPasteWavetable(copypastewavetable); //make a copy
	}
}

void VASTWaveTableEditorComponent::cutPositions() {
	stopWTRecording();
	copyPositions();
	deletePositions(false);
}

void VASTWaveTableEditorComponent::deleteAllButSelectedPositions() {
	copyPositions();
	insertCutCopiedPositions(true, true); //with softfade
	//setWtPos(wavetable->getNumPositions() - 1);
	//deletePositions(false);
}

void VASTWaveTableEditorComponent::normalizePositions(int beg, int end, CVASTWaveTable* wavetable, std::shared_ptr<CVASTWaveTable> bankWavetable, bool perCycle, int wtMode) {
	float global_max = 0.0f;
	if (perCycle == false) {
		for (int wtPos = beg; wtPos <= end; wtPos++) {
			for (int i = 0; i < C_WAVE_TABLE_SIZE; i++) {
				float sample = abs((*bankWavetable->getNaiveTable(wtPos))[i]);
				if (sample > global_max) global_max = sample;
			}
		}
	}
	std::vector<myFloat> samples = std::vector<myFloat>(C_WAVE_TABLE_SIZE);
	for (int wtPos = beg; wtPos <= end; wtPos++) {
		float local_max = 0.0f;
		float maximum = 0.0f;
		if (perCycle == true) {
			for (int i = 0; i < C_WAVE_TABLE_SIZE; i++) {
				float sample = abs((*bankWavetable->getNaiveTable(wtPos))[i]);
				if (sample > local_max) local_max = sample;
			}
			maximum = local_max;
		}
		else maximum = global_max;

		if (maximum == 0.f) continue;
		float gain = 1.f / maximum;

		for (int i = 0; i < C_WAVE_TABLE_SIZE; i++) {
			samples[i] = (*bankWavetable->getNaiveTable(wtPos))[i] * gain;
		}
		wavetable->setNaiveTable(wtPos, samples, true, wtMode);
	}
}

void VASTWaveTableEditorComponent::replaceWithCutCopiedPositions() {
	stopWTRecording();
	myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_bank].addSoftFadeEditor();
	std::shared_ptr<CVASTWaveTable> wavetable = myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_bank].getSoftOrCopyWavetable();
	std::shared_ptr<CVASTWaveTable> copypastewavetable = getCopyPasteWavetable();
	if (copypastewavetable == nullptr) {
		myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_bank].removeSoftFadeEditor();
		return;
	}

	int origPos = 0;
	if (wavetable->isMultiSelected()) {
		origPos = wavetable->getMultiSelectBegin();
	}
	else {
		origPos = wavetable->getSelectedWtPos();
	}

	if (wavetable->isMultiSelected()) {
		int beg = wavetable->getMultiSelectBegin();
		int end = wavetable->getMultiSelectEnd();
		for (int wtPos = end; wtPos >= beg; wtPos--) {
			wavetable->deletePosition(wtPos);
		}
		//if (wavetable->getNumPositions() == 0)
		//wavetable->addPosition();
	}
	else {
		int curPos = wavetable->getSelectedWtPos();
		wavetable->deletePosition(curPos);
		//if (wavetable->getNumPositions() == 0)
			//wavetable->addPosition();
	}

	//copy
	for (int i = copypastewavetable->getNumPositions() - 1; i >= 0; i--) {
		int newPos = 0;
		if (wavetable->isMultiSelected()) {
			newPos = wavetable->getMultiSelectBegin();
		}
		else {
			newPos = wavetable->getSelectedWtPos();
		}
		copypastewavetable->copyPositionToOtherWavetable(i, newPos, wavetable.get());
	}

	wavetable->setSelectedWtPos(origPos);
	myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_bank].setWavetableSoftFade(wavetable);
	myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_bank].removeSoftFadeEditor();
}

void VASTWaveTableEditorComponent::insertCutCopiedPositions(bool beforeSelection, bool deleteAllPosBefore) {
	stopWTRecording();
	myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_bank].addSoftFadeEditor();
	std::shared_ptr<CVASTWaveTable> wavetable = myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_bank].getSoftOrCopyWavetable();

	//setCurWavetable(wavetable); //make a copy
	if (deleteAllPosBefore) {
		while (wavetable->getNumPositions() > 0)
			wavetable->deletePosition(0);
	}
	std::shared_ptr<CVASTWaveTable> copypastewavetable = getCopyPasteWavetable();
	if (copypastewavetable == nullptr) {
		myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_bank].removeSoftFadeEditor();
		return;
	}
	for (int i = copypastewavetable->getNumPositions() - 1; i >= 0; i--) {

		int newPos = 0;
		if (wavetable->isMultiSelected()) {
			newPos = wavetable->getMultiSelectEnd();
		}
		else {
			newPos = wavetable->getSelectedWtPos();
		}
		if (!beforeSelection) newPos++;
		copypastewavetable->copyPositionToOtherWavetable(i, newPos, wavetable.get());
	}
	myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_bank].setWavetableSoftFade(wavetable);
	myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_bank].removeSoftFadeEditor();
	//m_positionviewport->setZoomFactor(c_waveTableEditorView->getHeader()->getWTEditorZoom()->getValue());
}

void VASTWaveTableEditorComponent::duplicateSelectedPositions() {
	stopWTRecording();
	myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_bank].addSoftFadeEditor();
	std::shared_ptr<CVASTWaveTable> wavetable = myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_bank].getSoftOrCopyWavetable();

	if (wavetable->isMultiSelected()) {
		int beg = wavetable->getMultiSelectBegin();
		int end = wavetable->getMultiSelectEnd();
		for (int wtPos = end; wtPos >= beg; wtPos--) {
			if (wavetable->getNumPositions() >= C_MAX_NUM_POSITIONS) break;
			wavetable->duplicatePosition(wtPos, end + 1);
		}
	}
	else {
		int curPos = wavetable->getSelectedWtPos();
		if (!(wavetable->getNumPositions() >= C_MAX_NUM_POSITIONS))
			wavetable->duplicatePosition(curPos, curPos + 1);
	}
	myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_bank].setWavetableSoftFade(wavetable);
	myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_bank].removeSoftFadeEditor();
	//m_positionviewport->setZoomFactor(c_waveTableEditorView->getHeader()->getWTEditorZoom()->getValue());
}

void VASTWaveTableEditorComponent::thinOutSelectedPositions() {
	deletePositions(true);
}

void VASTWaveTableEditorComponent::stretchAndInterpolateSelectedPositions() {
	stopWTRecording();
	myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_bank].addSoftFadeEditor();
	std::shared_ptr<CVASTWaveTable> wavetable = myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_bank].getSoftOrCopyWavetable();

	if (wavetable->isMultiSelected()) {
		int beg = wavetable->getMultiSelectBegin();
		int end = wavetable->getMultiSelectEnd();
		std::vector<myFloat> samples = std::vector<myFloat>(C_WAVE_TABLE_SIZE);
		for (int i = 0; i < (end - beg); i++) {
			wavetable->duplicatePosition(beg + i * 2, beg + i * 2 + 1);
			if ((beg + i * 2 + 2) < wavetable->getNumPositions()) {

				//interpolate
				for (int j = 0; j < C_WAVE_TABLE_SIZE; j++) {
					int msbeg = beg + i * 2;
					int msend = beg + i * 2 + 2;
					float sample_beg = (*wavetable->getNaiveTable(msbeg))[j];
					float sample_end = (*wavetable->getNaiveTable(msend))[j];
					float interp = dLinTerp(msbeg, msend, sample_beg, sample_end, beg + i * 2 + 1);
					samples[j] = interp;
				}
				wavetable->setNaiveTable(beg + i * 2 + 1, samples, true, myProcessor->m_pVASTXperience.m_Set.m_WTmode);
			}
		}
		wavetable->setSelection(beg, beg + (end - beg) * 2);
	}
	myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_bank].setWavetableSoftFade(wavetable);
	myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_bank].removeSoftFadeEditor();
}

void VASTWaveTableEditorComponent::reverseOrderSelectedPositions() {
	stopWTRecording();
	myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_bank].addSoftFadeEditor();
	std::shared_ptr<CVASTWaveTable> wavetable = myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_bank].getSoftOrCopyWavetable();
	if (wavetable->isMultiSelected()) {
		int beg = wavetable->getMultiSelectBegin();
		int end = wavetable->getMultiSelectEnd();
		for (int wtPos = beg; wtPos <= beg + int((end - beg) / 2); wtPos++) {
			std::vector<float> first = *wavetable->getNaiveTable(wtPos);
			std::vector<float> second = *wavetable->getNaiveTable(beg + end - wtPos);
			wavetable->setNaiveTable(wtPos, second, true, myProcessor->m_pVASTXperience.m_Set.m_WTmode);
			wavetable->setNaiveTable(beg + end - wtPos, first, true, myProcessor->m_pVASTXperience.m_Set.m_WTmode);
		}
	}
	myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_bank].setWavetableSoftFade(wavetable);
	myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_bank].removeSoftFadeEditor();
}

void VASTWaveTableEditorComponent::calcBloat(const std::vector<myFloat> &inSamples, std::vector<myFloat> &outSamples, int startSample, int endSample, float bloatValue) {
	if (approximatelyEqual(bloatValue, 0.f))
		return;
	int length = endSample - startSample + 1;
	//int o = endSample;
	double curvy = 0.5 + bloatValue / 200.0;

	double start = startSample;
	double len1 = length * 0.5;
	double end = start + len1;
	for (int i = start; i < end - 1; i++) { //skip the middle
											//y = offset + y * coeff; //0 to 1 or 1 to 0 per segment, old recursive logic, faster??
		double x = double(i - start) / double(len1); //0..1
		double y = cubicBezierApproximation(x, curvy);
		//samples[i] = (y - 1.f) * 0.5f + 0.5f; //test draw
		double val = start + ((y - 1.0) * 0.5 + 0.5) * (length - 2); //y from 0..1 --> val from start to (length -1) / 2
																	 //linterp
		int intpart = val;
		int intpartp1 = intpart + 1;
		intpart = (intpart <= start) ? start : intpart;
		intpart = (intpart > end - 1) ? end - 1 : intpart;
		intpartp1 = (intpartp1 <= start) ? start : intpartp1;
		intpartp1 = (intpartp1 > end - 1) ? end - 1 : intpartp1;
		double fracpart = val - intpart;
		outSamples[i] = (1.0 - fracpart) * inSamples[intpart] + fracpart * inSamples[intpartp1];
	}

	start = end;
	end = endSample;
	double len2 = length * 0.5;
	for (int i = start + 1; i <= end; i++) {
		//y = offset + y * coeff; //0 to 1 or 1 to 0 per segment, old recursive logic, faster??
		double x = double(i - start) / double(len2); //0..1
		double y = cubicBezierApproximation(x, curvy);
		int pos = end - (i - start) + 1; //skip the middle
										 //samples[pos] = (1.f - y) * 0.5f + 0.5f; //test draw
		double val = end - ((y - 1.0) * 0.5 + 0.5) * (length - 2);  //y from 0..1 --> val from end to (length -1) / 2 + 1
																	//linterp
		int intpart = val;
		int intpartp1 = intpart + 1;
		intpart = (intpart <= start) ? start : intpart;
		intpart = (intpart > end - 1) ? end - 1 : intpart;
		intpartp1 = (intpartp1 <= start) ? start : intpartp1;
		intpartp1 = (intpartp1 > end - 1) ? end - 1 : intpartp1;
		double fracpart = val - intpart;
		//samples[pos] = (1.0 - fracpart) * getCurWavetable()->getNaiveTable(wtPos)[intpart % C_WAVE_TABLE_SIZE] + fracpart * getCurWavetable()->getNaiveTable(wtPos)[(intpart + 1) % C_WAVE_TABLE_SIZE];
		outSamples[pos] = (1.0 - fracpart) * inSamples[intpart] + fracpart * inSamples[intpartp1];
	}
	if (length > 3) {
		outSamples[start - 1] = (outSamples[start - 2] + outSamples[start + 1]) * 0.5f; //calc middle median
		outSamples[start] = (outSamples[start - 2] + outSamples[start + 1]) * 0.5f; //calc middle median
	}
}

void VASTWaveTableEditorComponent::calcBend(const std::vector<myFloat> &inSamples, std::vector<myFloat> &outSamples, int startSample, int endSample, float bendValue) {
	if (approximatelyEqual(bendValue, 0.f))
		return;
	int length = endSample - startSample + 1;
	//int o = endSample;
	double curvy = 0.5 + (-1.0 * bendValue) / 200.0;

	int start = startSample;
	int end = endSample;
	for (int i = start; i <= end; i++) {
		//y = offset + y * coeff; //0 to 1 or 1 to 0 per segment, old recursive logic, faster??
		double x = double(i - start) / double(length - 1); //0..1
		double y = cubicBezierApproximation(x, curvy);
		//samples[i] = (y * 2.f - 1.f) * 0.5f + 0.5f; //test draw
		double val = start + ((y * 2.0 - 1.0) * 0.5 + 0.5) * (length - 1);
		//linterp
		int intpart = val;
		int intpartp1 = intpart + 1;
		intpart = (intpart <= start) ? start : intpart;
		intpart = (intpart > end - 1) ? end - 1 : intpart;
		intpartp1 = (intpartp1 <= start) ? start : intpartp1;
		intpartp1 = (intpartp1 > end - 1) ? end - 1 : intpartp1;
		double fracpart = val - intpart;
		outSamples[i] = (1.0 - fracpart) * inSamples[intpart] + fracpart * inSamples[intpartp1];
	}
}

void VASTWaveTableEditorComponent::frequencyModulate(const std::vector<myFloat> &inSamples, std::vector<myFloat> &outSamples, int startSample, int endSample, float freqModValue) {
	if (approximatelyEqual(freqModValue, 0.f))
		return;
	double phaseInc = 1.0f / float(C_WAVE_TABLE_SIZE);
	double phasor = 0.0f; //0..1
	double sinPhaseInc = phaseInc * (freqModValue / 20.f) * 12.f; // 6 is bell?
	double sinPhasor = 0.0f;

	for (int i = startSample; i <= endSample; i++) {
		double modulator = sin(sinPhasor * M_PI * 2.0f); //-1..1

		double ph = phasor + modulator;
		int wp = int((ph* double(C_WAVE_TABLE_SIZE)) + 2000.0 * C_WAVE_TABLE_SIZE) % C_WAVE_TABLE_SIZE;
		outSamples[i] = inSamples[wp];

		//float freq = 1.f / phaseInc;
		//float modPhaseInc = 1.f / (freq * (1.f + modulator * 0.6f)); //0.6f????
		//phasor += modPhaseInc;
		//float modPhaseInc = 0.0001f + 1.0f / modulator;
		phasor += phaseInc;
		//if (phasor >= 1.0f) phasor -= 1.0f;
		//vassert((phasor >= 0.f) && (phasor < 1.0f));

		sinPhasor += sinPhaseInc;
		if (sinPhasor >= 1.0f) sinPhasor -= 1.0f;
		vassert((sinPhasor >= 0.f) && (sinPhasor < 1.0f));
	}
}

void VASTWaveTableEditorComponent::calcFlip(const std::vector<myFloat> &inSamples, std::vector<myFloat> &outSamples, int startSample, int endSample, float flipValue) {
	if (approximatelyEqual(flipValue, 0.f))
		return;
	float j = 0;
	int end = endSample - startSample;
	float distance = (flipValue / 100.f) * (endSample - startSample);
	float endRange = startSample + distance;
	float step = distance / float(int(endRange) - startSample);
	outSamples = inSamples; //copy it
	for (int i = startSample; i <= endRange; i++) {
		int intpart = j;
		float fracpart = j - intpart;
		outSamples[i] = (1.f - fracpart)  * inSamples[startSample + end - intpart] + fracpart * inSamples[startSample + end - ((intpart + 1) < C_WAVE_TABLE_SIZE ? intpart + 1 : intpart)]; //linTerp
		j = j + step;
	}
}

void VASTWaveTableEditorComponent::calcMirror(const std::vector<myFloat> &inSamples, std::vector<myFloat> &outSamples, int startSample, int endSample, float mirrorValue) {
	if (approximatelyEqual(mirrorValue, 0.f))
		return;
	int j = 0;
	float distance = (mirrorValue / 100.f) * (endSample - startSample);
	float endRange = startSample + distance;
	float step = distance / float(int(endRange) - startSample);
	outSamples = inSamples; //copy it
	for (int i = startSample; i <= endRange; i++) {
		int intpart = j;
		float fracpart = j - intpart;
		outSamples[i] = (1.f - fracpart)  * (-1.f * inSamples[intpart]) + fracpart * (-1.f * inSamples[(intpart + 1) < C_WAVE_TABLE_SIZE ? intpart + 1 : intpart]); //linTerp
		j = j + step;
	}
}

void VASTWaveTableEditorComponent::calcBreed(const std::vector<myFloat> &inSamples, std::vector<myFloat> &outSamples, int startSample, int endSample, float breedValue) {
	if (approximatelyEqual(breedValue, 0.f))
		return;
	int length = endSample - startSample + 1;

	double start = startSample;
	double len1 = length * 0.5;
	double end = start + len1;
	float counter = 0.f;
	for (int i = start; i < end - 1; i++) { //skip the middle
		counter = (breedValue / 100.f) * 5.f + 1.f;
		int intpart = i * counter;
		float fracpart = i * counter - intpart;
		outSamples[i] = (1.f - fracpart) * inSamples[intpart % C_WAVE_TABLE_SIZE] + fracpart * inSamples[(intpart + 1) % C_WAVE_TABLE_SIZE];
		intpart = (end - i) * counter;
		fracpart = (end - i) * counter - intpart;
		outSamples[end + i] = (1.f - fracpart) * inSamples[C_WAVE_TABLE_SIZE - 1 - intpart % C_WAVE_TABLE_SIZE] + fracpart * inSamples[C_WAVE_TABLE_SIZE - 1 - (intpart + 1) % C_WAVE_TABLE_SIZE];
	}

	if (length > 3) {
		outSamples[end - 1] = (outSamples[end - 2] + outSamples[end + 1]) * 0.5f; //calc middle median
		outSamples[end] = (outSamples[end - 2] + outSamples[end + 1]) * 0.5f; //calc middle median
	}
}

void VASTWaveTableEditorComponent::calcBrew(const std::vector<myFloat> &inSamples, std::vector<myFloat> &outSamples, int startSample, int endSample, float brewValue) {
	if (approximatelyEqual(brewValue, 0.f))
		return;
	int length = endSample - startSample + 1;
	float factor = brewValue * 0.25f;
	int start = startSample;
	int end = endSample;
	int j = 0;
	for (int i = start; i <= end; i++) {
		float x = -1.f + (float(j) / length) * 2.f; //-1..1
		float tval = ((sin(x * factor) * (1.f - abs(x)) + x) + 1.f) * 0.5f; //-1..1, -1..1 --> 0..1
		int intpart = tval * (C_WAVE_TABLE_SIZE - 1);
		float fracpart = tval * (C_WAVE_TABLE_SIZE - 1) - intpart;
		outSamples[i] = (1.f - fracpart) * inSamples[intpart] + fracpart * inSamples[(intpart + 1) < C_WAVE_TABLE_SIZE ? intpart + 1 : intpart];
		j++;
	}
}

void VASTWaveTableEditorComponent::calcWindow(const std::vector<myFloat> &inSamples, std::vector<myFloat> &outSamples, int startSample, int endSample, float windowValue) {
	double curvy = (100.f - windowValue) / 100.0;
	int length = endSample - startSample + 1;
	double start = startSample;
	double len1 = length * 0.5;
	double end = start + len1;
	float j = 0;
	for (int i = start; i < end - 1; i++) { //skip the middle
		float fval = j / (end - 1 - start); //0..1
		float cubic = cubicBezierApproximation(fval, curvy);
		outSamples[i] = cubic * inSamples[i];
		outSamples[endSample - i] = cubic * inSamples[endSample - i];
		j++;
	}
}

void VASTWaveTableEditorComponent::calcSincify(const std::vector<myFloat> &inSamples, std::vector<myFloat> &outSamples, int startSample, int endSample, float sincifyValue) {
	int length = endSample - startSample + 1;

	float sindivval = 20.f + ((100.f - sincifyValue) / 100.f) * 300.f;
	double start = startSample;
	double len1 = length * 0.5;
	double end = start + len1;
	float maxabs = 0.f;
	for (int i = start; i < end - 1; i++) { //skip the middle
		float c1 = (float(i) / (endSample - startSample)) * 2.f - 1.f; //-1..0
		float c2 = sin((end - 1 - i) / sindivval); //start with 0
		float c3 = (i - start) / (end - 1 - start); //0..1
		float c4 = c2 * c3;
		float c5 = c1 * c4;
		float c6 = c1 * c5;
		outSamples[i] = c6;
		if (abs(c6) > maxabs)
			maxabs = abs(c6);
	}

	start = end;
	end = endSample;
	for (int i = start + 1; i <= end; i++) {
		float c1 = (float(i) / (endSample - startSample)) * 2.f - 1.f; //0..1
		float c2 = sin((end - 1 - i) / sindivval); //start with 0
		float c3 = (end - i) / (start + 1); //1..0
		float c4 = c2 * c3;
		float c5 = c1 * c4;
		float c6 = c1 * c5;
		outSamples[i] = c6;
		if (abs(c6) > maxabs)
			maxabs = abs(c6);
	}

	if (length > 3) {
		outSamples[start - 1] = 0.f;
		outSamples[start] = 0.f;
	}

	for (int i = startSample; i <= endSample; i++) {
		outSamples[i] = outSamples[i] / maxabs; //normalize to -1..1
	}
	//normalize

	for (int i = startSample; i <= endSample; i++) {
		float posval = ((outSamples[i] + 1.f) * 0.5f) * (C_WAVE_TABLE_SIZE - 1); //check for != 0..2047!
		int intpart = posval;
		float fracpart = posval - intpart;
		outSamples[i] = (1.f - fracpart)  * (inSamples[intpart]) + fracpart * (inSamples[(intpart + 1) < C_WAVE_TABLE_SIZE ? intpart + 1 : intpart]); //linTerp
	}
}

void VASTWaveTableEditorComponent::stretchToZeroCrossings(std::vector<myFloat> &inSamples, int wtselstart, int wtselend, bool snapFirstOnly) {

	std::vector<myFloat> outSamples = std::vector<myFloat>(C_WAVE_TABLE_SIZE);
	//1 check if first an last are (close to) zero
	//2 get next zero crossing from begin
	//3 get previous zero crossing from end
	//4 check that not the same
	//5 stretch / interpolate

	float tolerance = 0.001f;
	int zc_beg = wtselstart;
	int zc_end = wtselend;

	outSamples = inSamples; //copy it
	if (!(abs(inSamples[zc_beg]) < tolerance)) {
		while (zc_beg < wtselend - 1) {
			zc_beg++;
			if (((inSamples[zc_beg] <= 0.f) && (inSamples[zc_beg + 1] >= 0.f)) ||
				((inSamples[zc_beg] >= 0.f) && (inSamples[zc_beg + 1] <= 0.f)))
				break;
		}
	}
	if (!snapFirstOnly) {
		if (!(abs(inSamples[zc_end]) < tolerance)) {
			while (zc_end > wtselstart) {
				zc_end--;
				if (((inSamples[zc_end] <= 0.f) && (inSamples[zc_end + 1] >= 0.f)) ||
					((inSamples[zc_end] >= 0.f) && (inSamples[zc_end + 1] <= 0.f)))
					break;
			}
		}
		if (zc_beg >= zc_end) return;
	}

	for (int i = wtselstart; i <= wtselend; i++) {
		float fracpart = 0.f;
		if (!snapFirstOnly)
			fracpart = (((zc_end - zc_beg) / float(wtselend - wtselstart + 1)) * (i - wtselstart)) + zc_beg;
		else
			fracpart = wtselstart + ((i - wtselstart + zc_beg - wtselstart)) % (wtselend - wtselstart + 1);

		int intpart = fracpart;
		fracpart -= intpart;

		//linTerp
		outSamples[i] = (1 - fracpart) * inSamples[intpart] + fracpart * (inSamples[(intpart + 1) % (C_WAVE_TABLE_SIZE)]);
	}
	inSamples = outSamples; //copy it
}

void VASTWaveTableEditorComponent::scaleAudioBufferToWTPos(CVASTWaveTable* wavetable, int startWtPos, int targetWtPosCount, const float* audioBuffer, int numSamples, int fixedSamplesPerCycle) {
	if (targetWtPosCount < 1) return;
	if (targetWtPosCount + startWtPos > C_MAX_NUM_POSITIONS) return;
	float step = (float(numSamples) / float(C_WAVE_TABLE_SIZE)) / float(targetWtPosCount);

	if (fixedSamplesPerCycle > 0)
		step = fixedSamplesPerCycle / float(C_WAVE_TABLE_SIZE);

	std::vector<myFloat> samples = std::vector<myFloat>(C_WAVE_TABLE_SIZE);
	for (int wtPos = 0; wtPos < targetWtPosCount; wtPos++) {
		while ((wavetable->getNumPositions() - 1) < (wtPos + startWtPos))
			wavetable->addPosition();

		for (int i = 0; i < C_WAVE_TABLE_SIZE; i++) {
			int sindex = wtPos * C_WAVE_TABLE_SIZE + i;

			float fracpart = step * sindex;
			int intpart = fracpart;
			fracpart -= intpart;

			//linTerp
			if ((intpart + 1) > numSamples)
				samples[i] = 0.f;
			else
				samples[i] = (1 - fracpart) * audioBuffer[intpart] + fracpart * (audioBuffer[(intpart + 1) % (numSamples)]);
		}
		wavetable->setNaiveTable(wtPos + startWtPos, samples, true, myProcessor->m_pVASTXperience.m_Set.m_WTmode);
	}
}

void VASTWaveTableEditorComponent::wavSelectionToWavetablePatch(bool stereo, bool lfo) {
	VASTSamplerSound* VASTSamplerSound = getCurSamplerSound();
	if (VASTSamplerSound == nullptr) return;

	sSelectionWav* sel = m_samplerviewport->getSelection();
	int copyStart = sel->iWavSelectionStartSample;
	int copyEnd = sel->iWavSelectionEndSample;
	float time = float(copyEnd - copyStart) / float(VASTSamplerSound->getSourceSampleRate()) * 1000.f; //in ms

	if (!lfo) {
		myProcessor->m_pVASTXperience.m_Set.m_MSEGData_changed[4].initToRamp(4);
		myProcessor->m_pVASTXperience.m_Set.m_MSEGData[4].initToRamp(4);
		myProcessor->setParameterText("m_fAttackTime_MSEG5", String(time), false); //in ms
		myProcessor->m_pVASTXperience.m_Set.m_MSEGData[4].setAttackTimeSlider(time); // in ms
		myProcessor->m_pVASTXperience.m_Set.m_MSEGData_changed[4].setAttackTimeSlider(time); // in ms
		myProcessor->setParameterText("m_fReleaseTime_MSEG5", String(time), false); //calc
		myProcessor->m_pVASTXperience.m_Set.m_MSEGData[4].setReleaseTimeSlider(time); // in ms
		myProcessor->m_pVASTXperience.m_Set.m_MSEGData_changed[4].setReleaseTimeSlider(time); // in ms
		myProcessor->setParameterText("m_uMSEGPolarity_MSEG5", "Bipolar", false);
		myProcessor->m_pVASTXperience.m_Set.m_MSEGData_changed[4].setEnvMode(VASTMSEGData::EnvelopeMode::biploar);
		myProcessor->m_pVASTXperience.m_Set.m_MSEGData[4].setEnvMode(VASTMSEGData::EnvelopeMode::biploar);

		myProcessor->setParameterText("m_uModMatSrce1", "MSEG5Env", false);
		myProcessor->setParameterText("m_uModMatDest1", "OscAWTPos", false);
		myProcessor->setParameterText("m_fModMatVal1", "100", false);
		myProcessor->setParameterText("m_fModMatCurve1", "0", false);

		if (stereo) {
			myProcessor->setParameterText("m_uModMatSrce2", "MSEG5Env", false);
			myProcessor->setParameterText("m_uModMatDest2", "OscBWTPos", false);
			myProcessor->setParameterText("m_fModMatVal2", "100", false);
			myProcessor->setParameterText("m_fModMatCurve2", "0", false);
		}
	}
	else {
		float hz = 1.f / (time * 0.001f * 2.f); //triangle 2 times
		myProcessor->setParameterText("m_fLFOFreq_LFO5", String(hz), false);
		myProcessor->setParameterText("m_uLFOWave_LFO5", "Saw", false);
		myProcessor->setParameterText("m_uLFOPolarity_LFO5", "Bipolar", false);
		myProcessor->setParameterText("m_fLFORamp_LFO5", "0", false);
		myProcessor->setParameterText("m_fLFOPhase_LFO5", "50", false); //for triangle ramp up
		myProcessor->setParameterText("m_bLFOPerVoice_LFO5", "Off", false);
		myProcessor->setParameterText("m_bLFORetrigOnOff_LFO5", "Off", false);
		myProcessor->setParameterText("m_bLFOSynch_LFO5", "Off", false);

		myProcessor->setParameterText("m_uModMatSrce1", "LFO5", false);
		myProcessor->setParameterText("m_uModMatDest1", "OscAWTPos", false);
		myProcessor->setParameterText("m_fModMatVal1", "100", false);
		myProcessor->setParameterText("m_fModMatCurve1", "0", false);

		if (stereo) {
			myProcessor->setParameterText("m_uModMatSrce2", "LFO5", false);
			myProcessor->setParameterText("m_uModMatDest2", "OscBWTPos", false);
			myProcessor->setParameterText("m_fModMatVal2", "100", false);
			myProcessor->setParameterText("m_fModMatCurve2", "0", false);
		}
	}

	if (stereo) {
		setOscBank(0);
		//wavetable transaction chain
		myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_bank].addSoftFadeEditor();
		std::shared_ptr<CVASTWaveTable> wavetable = myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_bank].getSoftOrCopyWavetable();

		wavSelectionToWt(0, wavetable);
		wavetable->setSelection(0, wavetable->getNumPositions() - 1);
		threadedEditorFunction(EditorFunction::Normalize, 0.f, /*0, wavetable->getNumPositions() - 1,*/ 0, 0, this, 0.f, 0.f, wavetable, true);

		myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_bank].setWavetableSoftFade(wavetable);
		myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_bank].removeSoftFadeEditor();

		setOscBank(1);
		//wavetable transaction chain
		myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_bank].addSoftFadeEditor();
		wavetable = myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_bank].getSoftOrCopyWavetable();;

		wavSelectionToWt(1, wavetable);
		wavetable->setSelection(0, wavetable->getNumPositions() - 1);
		threadedEditorFunction(EditorFunction::Normalize, 0.f, /*0, wavetable->getNumPositions() - 1,*/ 0, 0, this, 0.f, 0.f, wavetable, true);

		myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_bank].setWavetableSoftFade(wavetable);
		myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_bank].removeSoftFadeEditor();

		setOscBank(0);

		myProcessor->setParameterText("m_bOscOnOff_OscA", "On", false);
		myProcessor->setParameterText("m_bOscOnOff_OscB", "On", false);
		myProcessor->setParameterText("m_fOscMorph_OscA", "100", false);
		myProcessor->setParameterText("m_fOscMorph_OscB", "100", false);
		myProcessor->setParameterText("m_fOscPan_OscA", "-100", false);
		myProcessor->setParameterText("m_fOscPan_OscB", "100", false);
		myProcessor->setParameterText("m_fOscGain_OscA", "200", false); //check
		myProcessor->setParameterText("m_fOscGain_OscB", "200", false); //check
		myProcessor->setParameterText("m_iOscOct_OscA", "-2", false); //calc
		myProcessor->setParameterText("m_iOscOct_OscB", "-2", false); //calc
	}
	else {
		setOscBank(0);
		//wavetable transaction chain
		myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_bank].addSoftFadeEditor();
		std::shared_ptr<CVASTWaveTable> wavetable = myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_bank].getSoftOrCopyWavetable();

		wavSelectionToWt(0, wavetable);
		wavetable->setSelection(0, wavetable->getNumPositions() - 1);
		threadedEditorFunction(EditorFunction::Normalize, 0.f, /*0, wavetable->getNumPositions() - 1,*/ 0, 0, this, 0.f, 0.f, wavetable, true);

		myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_bank].setWavetableSoftFade(wavetable);
		myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_bank].removeSoftFadeEditor();

		myProcessor->setParameterText("m_bOscOnOff_OscA", "On", false);
		myProcessor->setParameterText("m_fOscMorph_OscA", "100", false);
		myProcessor->setParameterText("m_fOscPan_OscA", "0", false);
		myProcessor->setParameterText("m_fOscGain_OscA", "200", false); //check
		myProcessor->setParameterText("m_iOscOct_OscA", "-2", false); //calc
	}

	myProcessor->setParameterText("m_bSamplerOnOff", "Off", false);
}


float VASTWaveTableEditorComponent::sampleDetermineFreq(int s_start, int s_end, int channel) {
	//based on FFT
	const int FFTFactor = 32;
	const int FFTAccuracy = C_WAVE_TABLE_SIZE * FFTFactor;
    std::unique_ptr<dsp::FFT> fft(new dsp::FFT(log2(FFTAccuracy)));
	std::vector<dsp::Complex<float>> l_filterBuffer_in = std::vector<dsp::Complex<float>>(FFTAccuracy);
	std::vector<dsp::Complex<float>> l_filterBuffer_out = std::vector<dsp::Complex<float>>(FFTAccuracy);
	VASTSamplerSound* samplerSound = getCurSamplerSound();
	if (samplerSound == nullptr) return 0.f;

	int copyStart = s_start;
	int copyEnd = s_end;
	auto* inBuffer = samplerSound->getAudioDataChanged()->getReadPointer(channel, copyStart);
	int numSamples = copyEnd - copyStart;
	if (numSamples <= 1) {
		return 0.f;
	}
	//CVASTWaveTable wavetable;
	//myWtEditor->scaleAudioBufferToWTPos(&wavetable, 0, 1, inBuffer, numSamples);
	//double scalefactor = double(FFTAccuracy) / double(numSamples);

	float buffer[FFTAccuracy];
	for (int idx = 0; idx < FFTAccuracy; idx++) {
		//buffer[idx] = wavetable.getNaiveTable(0)[idx / FFTFactor];
		if (idx < (copyEnd - copyStart))
			buffer[idx] = inBuffer[idx];
		else
			buffer[idx] = 0.f;
	}
    std::unique_ptr<dsp::WindowingFunction<float>> window(new dsp::WindowingFunction<float>(samplerSound->getLoopEndChanged() - samplerSound->getLoopStartChanged(), dsp::WindowingFunction<float>::hann, true, 0.0f));
	window->multiplyWithWindowingTable(buffer, FFTAccuracy);

	for (int idx = 0; idx < FFTAccuracy; idx++) {
		l_filterBuffer_in[idx].imag(buffer[idx]);
		l_filterBuffer_in[idx].real(0.0f);
	}

	fft->perform(&*l_filterBuffer_in.begin(), &*l_filterBuffer_out.begin(), false);

	//find max
	int maxslot = 0;
	for (int idx = 1; idx < FFTAccuracy / 2.f; idx++) {
		if (abs(l_filterBuffer_out[idx].imag()) > abs(l_filterBuffer_out[maxslot].imag()))
			maxslot = idx;
	}

	/*
	int octavelower = maxslot * 0.5f;
	while (octavelower > 0) {
		if (abs(l_filterBuffer_out[octavelower].imag()) > abs(l_filterBuffer_out[maxslot].imag() * 0.3f)) { //octave lower is more than half of magnitude -> this is fundamental!
			maxslot = octavelower;
		}
		else if (abs(l_filterBuffer_out[octavelower - 1].imag()) > abs(l_filterBuffer_out[maxslot].imag() * 0.3f)) { //octave lower is more than half of magnitude -> this is fundamental!
			maxslot = octavelower - 1;
		}
		else if ((octavelower > 1) && (abs(l_filterBuffer_out[octavelower + 1].imag()) > abs(l_filterBuffer_out[maxslot].imag() * 0.3f))) { //octave lower is more than half of magnitude -> this is fundamental!
			maxslot = octavelower + 1;
		}
		else break;
		octavelower = maxslot * 0.5f;
	}
	*/

	//second try
	for (int idx = 2; idx < maxslot * 0.9f; idx++) { //find maximum with magnitude of 60% max --> this is fundamental
		//is local maximum?
		if ((abs(l_filterBuffer_out[idx].imag()) > abs(l_filterBuffer_out[idx - 1].imag())) &&
			(abs(l_filterBuffer_out[idx].imag()) > abs(l_filterBuffer_out[idx + 1].imag()))) {
			if (abs(l_filterBuffer_out[idx].imag()) > (abs(l_filterBuffer_out[maxslot].imag() * 0.6f))) {
				maxslot = idx;
				break;
			}
		}
	}

	double fftFreq = 0.f;
	if ((maxslot >= 2) && (maxslot < FFTAccuracy - 2)) {
		double main = (double(samplerSound->getSourceSampleRate()) / double(FFTAccuracy)) * maxslot;
		//double main = (double(myProcessor->m_pVASTXperience.m_Set.m_nSampleRate) / double(FFTAccuracy)) * maxslot;

		/*
		double mainmult = pow(2.0, abs(l_filterBuffer_out[maxslot].imag()));
		double mainp1 = (samplerSound->getSourceSampleRate() / C_WAVE_TABLE_SIZE) * (maxslot + 1);
		double mainp1mult = pow(2.0, abs(l_filterBuffer_out[maxslot + 1].imag()));
		double mainp2 = (samplerSound->getSourceSampleRate() / C_WAVE_TABLE_SIZE) * (maxslot + 2);
		double mainp2mult = pow(2.0, abs(l_filterBuffer_out[maxslot + 2].imag()));
		double mainm1 = (samplerSound->getSourceSampleRate() / C_WAVE_TABLE_SIZE) * (maxslot - 1);
		double mainm1mult = pow(2.0, abs(l_filterBuffer_out[maxslot - 1].imag()));
		double mainm2 = (samplerSound->getSourceSampleRate() / C_WAVE_TABLE_SIZE) * (maxslot - 2);
		double mainm2mult = pow(2.0, abs(l_filterBuffer_out[maxslot - 2].imag()));
		double sum = main * mainmult + mainp1 * mainp1mult + mainp2 * mainp2mult + mainm1 * mainm1mult + mainm2 * mainm2mult;
		fftFreq = sum / (mainmult + mainp1mult + mainp2mult + mainm1mult + mainm2mult);
		fftFreq *= scalefactor;
		*/

		fftFreq = main;
	}
	else {
		fftFreq = (samplerSound->getSourceSampleRate() / FFTAccuracy) * maxslot;
	}
	if (fftFreq == 0.0) return 0.f;

	return fftFreq;
}

void VASTWaveTableEditorComponent::wavSelectionToWt(int channel, std::shared_ptr<CVASTWaveTable> transactionWavetable) { //nullptr means no chain transaction
	vassert(channel < 2);
	vassert(channel >= 0);
	int lChannel = channel;
	VASTSamplerSound* VASTSamplerSound = getCurSamplerSound();
	sSelectionWav* sel = m_samplerviewport->getSelection();

	if (VASTSamplerSound != nullptr) {
		if (VASTSamplerSound->getAudioDataChanged()->getNumChannels() == 1) //mono
			lChannel = 0;
		std::shared_ptr<CVASTWaveTable> wavetable = nullptr;
		if (transactionWavetable == nullptr) {
			myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_bank].addSoftFadeEditor();
			wavetable = getBankWavetable();
			setCurWavetable(wavetable); //make a copy
			wavetable = getCurWavetable(); //read the copy and work on the copy
		}
		else {
			//must handle softfade externally!
			wavetable = transactionWavetable;
		}

		int end = wavetable->getNumPositions() - 1;
		for (int wtPos = end; wtPos >= 0; wtPos--) {
			wavetable->deletePosition(wtPos);
		}

		int copyStart = sel->iWavSelectionStartSample;
		int copyEnd = sel->iWavSelectionEndSample;

		if (!VASTSamplerSound->hasLoopChanged()) { // use all if nothing selected
			copyStart = 0;
			copyEnd = VASTSamplerSound->getAudioDataChanged()->getNumSamples();
		}

		int numSamples = copyEnd - copyStart;

			//third Logic

		int targetPosNo = c_samplerEditorComponent->getHeader()->getWAVWTPosSlider()->getValue();
		int counter = -1;
		int targetWtPos = 0;
		float desiredRange = float(numSamples) / float(targetPosNo);

		int startPos = copyStart;
		auto* subBuffer = VASTSamplerSound->getAudioDataChanged()->getReadPointer(lChannel, 0);
		bool bFinished = false;
		while (!bFinished) {
			float starttVal = subBuffer[startPos];
			bool bRisingEdge = subBuffer[startPos] < subBuffer[startPos + 1]; //check buffer overflow
			int distanceBack = 0;

			int endPosBack = startPos + desiredRange;
			bool bExit = false;
			if (endPosBack > copyEnd) bExit = true;
			while (!bExit) {
				endPosBack--;
				distanceBack++;
				if (bRisingEdge) {
					if ((subBuffer[endPosBack] < starttVal) && (subBuffer[endPosBack + 1] >= starttVal)) { //check buffer overflow
						endPosBack++;
						bExit = true;
					}
				}
				else { //falling edge
					if ((subBuffer[endPosBack] > starttVal) && (subBuffer[endPosBack + 1] <= starttVal)) { //check buffer overflow
						endPosBack++;
						bExit = true;
					}
				}
				if (endPosBack <= startPos) {
					endPosBack = -1;
					bExit = true;
				}
			}

			int distanceFront = 0;
			int endPosFront = startPos + desiredRange;
			bExit = false;
			if (endPosFront > copyEnd) bExit = true;
			while (!bExit) {
				endPosFront++;
				distanceFront++;
				if (bRisingEdge) {
					if ((subBuffer[endPosFront] < starttVal) && (subBuffer[endPosFront + 1] >= starttVal)) { //check buffer overflow
						endPosFront--;
						bExit = true;
					}
				}
				else { //falling edge
					if ((subBuffer[endPosFront] > starttVal) && (subBuffer[endPosFront + 1] <= starttVal)) { //check buffer overflow
						endPosFront--;
						bExit = true;
					}
				}
				if (endPosFront >= copyEnd) {
					endPosFront = -1;
					bExit = true;
				}
			}

			int endPos = -1;
			counter = -1;
			if ((endPosFront == -1) && (endPosBack == -1)) {
				counter = -1;
			}
			else {
				if (endPosFront == -1) endPos = endPosBack;
				else if (endPosBack == -1) endPos = endPosFront;
				else {
					if (distanceBack < distanceFront)
						endPos = endPosBack;
					else
						endPos = endPosFront;
				}
				counter = endPos - startPos;
			}

			if ((counter > 0) && (counter >= desiredRange * 0.6f) && (counter <= desiredRange * 1.8f)) {
				auto* subBufferTarget = VASTSamplerSound->getAudioDataChanged()->getReadPointer(lChannel, startPos);
				scaleAudioBufferToWTPos(wavetable.get(), targetWtPos, 1, subBufferTarget, counter);

				/*
				wavetable->addPosition();
				std::vector<myFloat> samples = std::vector<myFloat>(C_WAVE_TABLE_SIZE);
				for (int i = 0; i < C_WAVE_TABLE_SIZE; i++) {
					if (i < counter)
						samples[i] = subBuffer[startPos + i];
					else
						samples[i] = 0.f;

				}
				wavetable->setNaiveTable(targetWtPos, samples, true);
				*/

				targetWtPos++;
				startPos = endPos; //+ - 1??
			}
			else {
				startPos++;
			}

			if ((endPos + desiredRange) > copyEnd)
				bFinished = true;
		}
		//

		wavetable->setWaveTableName(VASTSamplerSound->getName() +  ( lChannel == 0 ? " (L)" : " (R)")  );

		if (wavetable->getNumPositions() == 0) wavetable->addPosition();

		if (transactionWavetable == nullptr) {
			wavetable->setSelectedWtPos(0);
			myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_bank].setWavetableSoftFade(wavetable);
			myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_bank].removeSoftFadeEditor();
		}
		else {
			//setWtPos(0);
		}
		m_positionviewport->setZoomFactor(c_waveTableEditorView->getHeader()->getWTEditorZoom()->getValue());
	}
}

void VASTWaveTableEditorComponent::findZeroLoop() {
	VASTSamplerSound* samplerSound = getCurSamplerSound();
	if (samplerSound == nullptr) return;
	//m_samplerviewport->msele
	int lstart = 0;
	int lend = 0;
	if (!samplerSound->hasLoopChanged()) {
		lstart = 0;
		lend = samplerSound->getLengthChanged() - 1;
	}
	else {
		lstart = samplerSound->getLoopStartChanged();
		lend = samplerSound->getLoopEndChanged();
	}
	int lstartOld = lstart;
	int lendOld = lend;

	float initVal = samplerSound->getAudioDataChanged()->getSample(0, lstart);
	float actVal = initVal;
	//if (samplerSound->getData()->getNumChannels() == 1) {
		while (((initVal * actVal) >= 0.f) && (lstart < lend - 8)) { //arbitray min 8 sample length
			actVal = samplerSound->getAudioDataChanged()->getSample(0, lstart);
			lstart++;
		}
		if ((initVal * actVal) >= 0.f) lstart = lstartOld;

		initVal = samplerSound->getAudioDataChanged()->getSample(0, lend);
		actVal = initVal;
		while (((initVal * actVal) >= 0.f) && (lstart < lend - 8)) { //arbitray min 8 sample length
			actVal = samplerSound->getAudioDataChanged()->getSample(0, lend);
			lend--;
		}
		if ((initVal * actVal) >= 0.f) lend = lendOld;
	/*
	}
	else if (samplerSound->getData()->getNumChannels() == 2) {
		while ((valDiff > 0.01f) && (lstart < lend - 64)) { //arbitray min 64 sample length
			valDiff = abs(samplerSound->getAudioDataChanged()->getSample(0, lstart)) + abs(samplerSound->getData()->getSample(1, lstart));
			lstart++;
		}
		valDiff = 9999.f;
		while ((valDiff > 0.01f) && (lstart < lend - 64)) { //arbitray min 64 sample length
			valDiff = abs(samplerSound->getAudioDataChanged()->getSample(0, lend)) + abs(samplerSound->getData()->getSample(1, lend));
			lend--;
		}
	}
	else jassertfalse;
	*/

	samplerSound->setLoopChanged(lstart, lend);
	m_samplerviewport->setSelectionFromWavSamples(lstart, lend);
}

void VASTWaveTableEditorComponent::importFilesAsCycles(const StringArray& files, int startPos) {
	std::shared_ptr<CVASTWaveTable> wavetable = getBankWavetable();
	setCurWavetable(wavetable); //make a copy
	wavetable = getCurWavetable(); //read the copy and work on the copy

	int added = 0;
	int positionsBefore = wavetable->getNumPositions();
	for (int i = 0; i < files.size(); i++) {
		if (!(files[i].endsWithIgnoreCase(".wav") || files[i].endsWithIgnoreCase(".aif"))) continue;
		String fname = files[i];
        std::unique_ptr<VASTSamplerSound> lVASTSamplerSound(loadWavFile(fname));
		if (lVASTSamplerSound == nullptr) continue;
		added++;

		/*
		if (startPos >= positionsBefore)
			wavetable->duplicatePosition(startPos + added - 2, startPos + added - 2);
		else
			wavetable->duplicatePosition(startPos + added - 1, startPos + added - 1);
			*/

		if (startPos >= positionsBefore)
			wavetable->duplicatePosition(startPos + added - 2, startPos + added - 2);

		auto* inBuffer = lVASTSamplerSound->getAudioDataChanged()->getReadPointer(0, 0); //only left channel?
		int targetPosNo = 1;
		int numSamples = lVASTSamplerSound->getAudioDataChanged()->getNumSamples();

		scaleAudioBufferToWTPos(wavetable.get(), startPos + added - 1, targetPosNo, inBuffer, numSamples);
	}

	if (added > 0) {
		wavetable->setSelectedWtPos(startPos);
		myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_bank].addSoftFadeEditor();
		myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_bank].setWavetableSoftFade(wavetable);
		myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_bank].removeSoftFadeEditor();
		//setWtPos(startPos);
	}
	m_positionviewport->setZoomFactor(c_waveTableEditorView->getHeader()->getWTEditorZoom()->getValue());
	requestUIUpdate();
}

void VASTWaveTableEditorComponent::loadWTFile(String filename) {
	File wavFile(filename);
	if (!wavFile.existsAsFile()) {
		AlertWindow::showMessageBoxAsync(MessageBoxIconType::WarningIcon, TRANS("Failed"), TRANS("Wavetable file does not exist."), TRANS("Continue"), this);
		return;
	}
	loadWTFileThreaded(wavFile);
	initSliders();
	setWtPos(0);
	requestUIUpdate();
}

VASTSamplerSound* VASTWaveTableEditorComponent::loadWavFile(String filename) {

	AudioFormatManager formatManager;
	formatManager.registerBasicFormats();
    std::unique_ptr<AudioFormatReader> reader(formatManager.createReaderFor(File(filename)));
	if (reader != nullptr && reader->lengthInSamples > 0 && reader->numChannels > 0)
	{
		auto duration = reader->lengthInSamples / reader->sampleRate;
		if (duration < 600) { //max seconds
			StringArray metadataValues = reader->metadataValues.getAllValues();
			StringArray metadataKeys = reader->metadataValues.getAllKeys();
			VDBG(metadataKeys.joinIntoString(";"));
			VDBG(metadataValues.joinIntoString(";"));

			/*
			https://forum.juce.com/t/how-to-read-extensible-non-catalogue-metadata-properties-from-a-wav-file/24250
			bwav description
			bwav originator
			bwav originator ref
			bwav origination date
			bwav origination time
			bwav time reference
			bwav coding history
			NumCuePoints
			Cue0Identifier
			Cue0Order
			Cue0ChunkID
			Cue0ChunkStart
			Cue0BlockStart
			Cue0Offset
			Manufacturer
			Product
			SamplePeriod
			MidiUnityNote
			MidiPitchFraction
			SmpteFormat
			SmpteOffset
			NumSampleLoops
			SamplerData
			Loop0Identifier
			Loop0Type
			Loop0Start
			Loop0End
			Loop0Fraction
			Loop0PlayCount
			CueLabel0Identifier
			CueLabel0Text
			IART
			NumCueLabels
			MetaDataSource
			*/

			int samplerRootKey = *myProcessor->m_pVASTXperience.m_Set.m_State->m_uSamplerRootKey;
			if (metadataKeys.contains("MidiUnityNote")) {
				String val = metadataValues[metadataKeys.indexOf("MidiUnityNote")];
				if (val.getIntValue() > 0) {
					samplerRootKey = val.getIntValue();
					myProcessor->setParameterText("m_uSamplerRootKey", CVASTParamState::comboBoxValueToTextFunction_MIDINOTES(samplerRootKey), true);
				}
			}

			BigInteger allNotes;
			allNotes.setRange(0, 128, true);
			File fl = File(filename);
			VASTSamplerSound* lVASTSamplerSound = new VASTSamplerSound(fl.getFileName(), *reader, allNotes, samplerRootKey); // OK owned array

			if (metadataKeys.contains("Loop0Start")) {
				String valStart = metadataValues[metadataKeys.indexOf("Loop0Start")];
				String valEnd = metadataValues[metadataKeys.indexOf("Loop0End")];
				String valType = metadataValues[metadataKeys.indexOf("Loop0Type")];
				if (valStart.getIntValue() > 0) {
					if (valEnd.getIntValue() > 0) {
						VDBG("LoopFound: " << valStart << " " << valEnd << " " << valType);
						lVASTSamplerSound->setLoopChanged(valStart.getIntValue(), valEnd.getIntValue());
					}
				}
			}

			return lVASTSamplerSound;
		}
		else {
			AlertWindow::showMessageBoxAsync(MessageBoxIconType::InfoIcon, TRANS("Failed"), TRANS("The audio file is too long. 600 seconds is the maximum."), TRANS("Continue"), this);
		}
	}

	return nullptr;
}

void VASTWaveTableEditorComponent::initAll() {
	setWtPos(0);
	m_samplerviewport->notifySelectionChanged();
	m_samplerviewport->startAutoUpdate();
	m_freqviewport->startAutoUpdate();
	m_positionviewport->startAutoUpdate();
	c_waveTableEditorView->getHeader()->getGenMaxSlider()->setValue(100.f, juce::NotificationType::dontSendNotification); //changed from 75
	c_waveTableEditorView->getHeader()->getApplyMaxSlider()->setValue(100.f, juce::NotificationType::dontSendNotification); //changed from 75
	c_waveTableEditorView->getHeader()->c_soloSelection->setToggleState(false, NotificationType::sendNotification);
	c_waveTableEditorView->getHeader()->c_snap->setToggleState(false, NotificationType::sendNotification);
	requestUIUpdate(); //was true
}

/*
void VASTWaveTableEditorComponent::addPosition() {
	std::shared_ptr<CVASTWaveTable> wavetable = getBankWavetable();
	wavetable->addPosition();
	setCurWavetable(wavetable); //make a copy
	//setWtPos(wavetable->getNumPositions() - 1);
	wavetable->setSelectedWtPos(wavetable->getNumPositions() - 1);
}
*/

void VASTWaveTableEditorComponent::insertPosition() {
	myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_bank].addSoftFadeEditor();
	std::shared_ptr<CVASTWaveTable> wavetable = myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_bank].getSoftOrCopyWavetable();
	wavetable->insertEmptyPosition(wavetable->getSelectedWtPos());
	//setCurWavetable(wavetable); //make a copy
	//setWtPos(wavetable->getSelectedWtPos() + 1);
	wavetable->setSelectedWtPos(wavetable->getSelectedWtPos() + 1);
	myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_bank].setWavetableSoftFade(wavetable);
	myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_bank].removeSoftFadeEditor();
}

void VASTWaveTableEditorComponent::updateAll(bool force) {
	//check time since last update to avoid multiples
	/*
	struct timeval tp;
	myProcessor->m_pVASTXperience.m_Set._gettimeofday(&tp);
	ULong64_t l_lastUIUpdate = tp.tv_sec * 1000 + tp.tv_usec / 1000;
	VDBG("Time: " + String(l_lastUIUpdate - m_lastUIUpdate));
	if (!force) {
		if (l_lastUIUpdate - m_lastUIUpdate < 1000) //1 millisecond
			return;
	}
	m_lastUIUpdate = l_lastUIUpdate;
	*/

	VDBG("VASTWaveTableEditorComponent::updateAll");
	//VDBG((force == true ? "true" : "false"));
	if ((c_waveTableEditorView == nullptr) || (c_samplerEditorComponent == nullptr)) return;
	if (c_waveTableEditorView != nullptr)
		if (c_waveTableEditorView->getHeader() != nullptr) {
			std::shared_ptr<CVASTWaveTable> wavetable = myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_bank].getSoftFadeWavetablePointerNext();
			if (wavetable == nullptr)
				wavetable = myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_bank].getSoftFadeWavetablePointer();
				if (wavetable == nullptr)
				wavetable = myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_bank].getWavetablePointer();
			c_waveTableEditorView->getHeader()->getWTNameBox()->setText(wavetable->getWaveTableName());
		}
	if (c_samplerEditorComponent!= nullptr)
		if (c_samplerEditorComponent->getHeader() != nullptr) {
			VASTSamplerSound* sound = (VASTSamplerSound*)myProcessor->m_pVASTXperience.m_Poly.getSamplerSoundChanged();
			if (sound != nullptr)
				c_samplerEditorComponent->getHeader()->getWAVNameBox()->setText(sound->getName());
			else
				c_samplerEditorComponent->getHeader()->getWAVNameBox()->setText("");
		}

	if (m_freqviewport != nullptr)
		//m_freqviewport->updateContent(force);
		m_freqviewport->updateContentAsync();
	if (m_positionviewport != nullptr)
		//m_positionviewport->updateContent(force);
		m_positionviewport->updateContentAsync();
	if (c_waveTableEditorView->getEditorOscilloscope() != nullptr) {
		if (force)
			c_waveTableEditorView->getEditorOscilloscope()->updateContent(force); //cannot be async
		else
			c_waveTableEditorView->getEditorOscilloscope()->updateContentAsync(); //cannot be async
	}
	if (m_samplerviewport != nullptr)
		m_samplerviewport->updateContentAsync();
	c_waveTableEditorView->getHeader()->updateAll();

	//check wtpos selection
	//std::shared_ptr<CVASTWaveTable> wavetable = getBankWavetable();
	//if (wavetable != nullptr) {
	//	if (wavetable->getSelectedWtPos() >= wavetable->getNumPositions())
	//		setWtPos(0);
	//}

	updateHeaderSelectionLabel();
	//force not used anymore
}

void VASTWaveTableEditorComponent::copySelectionToLocalBuffer() {
	sliderThatWasLastMoved = nullptr;
	//sSelection* sel = c_editorOscilloscope->getSelection();
	//std::unique_ptr<CVASTWaveTable> wavetable = new CVASTWaveTable(*getBankWavetable()); //copy constructor
	//CVASTWaveTable wavetable;
	//wavetable.copyFrom(*getBankWavetable());

	//CHECK SAFETY
	//if (wavetable.wtheader.waveTablePositions.size() == 0) return;

	//std::vector<myFloat> samples = std::vector<myFloat>(C_WAVE_TABLE_SIZE);
	//samples = wavetable.getNaiveTable(wavetable->getSelectedWtPos());

	/*
	std::shared_ptr<CVASTWaveTable> curWT = getCurWavetable();
	for (int i = sel->iWTSelectionStart; i <= sel->iWTSelectionEnd; i++) {
	samples[i] = getCurWavetable()->getNaiveTable(wavetable->getSelectedWtPos())[i];
	}
	*/

	//CVASTWaveTable wavetablecopy;
	//wavetablecopy.copyFrom(wavetable);
	//wavetablecopy.setNaiveTable(wavetable->getSelectedWtPos(), samples, false); //dont generate
	std::shared_ptr<CVASTWaveTable> table = getBankWavetable();
	setCurWavetable(table);
}

void VASTWaveTableEditorComponent::initSliders() {
	//rest sliders to initial

	if (c_waveTableEditorView != nullptr)
		if (c_waveTableEditorView->getHeader() != nullptr) {
		c_waveTableEditorView->getHeader()->getBitDepthSlider()->setValue(c_waveTableEditorView->getHeader()->getBitDepthSlider()->getDoubleClickReturnValue(), juce::NotificationType::dontSendNotification);
		c_waveTableEditorView->getHeader()->getBendSlider()->setValue(c_waveTableEditorView->getHeader()->getBendSlider()->getDoubleClickReturnValue(), juce::NotificationType::dontSendNotification);
		c_waveTableEditorView->getHeader()->getBloatSlider()->setValue(c_waveTableEditorView->getHeader()->getBloatSlider()->getDoubleClickReturnValue(), juce::NotificationType::dontSendNotification);
		c_waveTableEditorView->getHeader()->getCleanSlider()->setValue(c_waveTableEditorView->getHeader()->getCleanSlider()->getDoubleClickReturnValue(), juce::NotificationType::dontSendNotification);
		c_waveTableEditorView->getHeader()->getSmoothSlider()->setValue(c_waveTableEditorView->getHeader()->getSmoothSlider()->getDoubleClickReturnValue(), juce::NotificationType::dontSendNotification);
		c_waveTableEditorView->getHeader()->getOffsetSlider()->setValue(c_waveTableEditorView->getHeader()->getOffsetSlider()->getDoubleClickReturnValue(), juce::NotificationType::dontSendNotification);
		c_waveTableEditorView->getHeader()->getFreqSlider()->setValue(c_waveTableEditorView->getHeader()->getFreqSlider()->getDoubleClickReturnValue(), juce::NotificationType::dontSendNotification);
		c_waveTableEditorView->getHeader()->getFreqModSlider()->setValue(c_waveTableEditorView->getHeader()->getFreqModSlider()->getDoubleClickReturnValue(), juce::NotificationType::dontSendNotification);
		c_waveTableEditorView->getHeader()->getLevelSlider()->setValue(c_waveTableEditorView->getHeader()->getLevelSlider()->getDoubleClickReturnValue(), juce::NotificationType::dontSendNotification);
		c_waveTableEditorView->getHeader()->getPhaseSlider()->setValue(c_waveTableEditorView->getHeader()->getPhaseSlider()->getDoubleClickReturnValue(), juce::NotificationType::dontSendNotification);

		c_waveTableEditorView->getHeader()->getConnectSlider()->setValue(c_waveTableEditorView->getHeader()->getConnectSlider()->getDoubleClickReturnValue(), juce::NotificationType::dontSendNotification);
	}
	sliderThatWasLastMoved = nullptr;
}

void VASTWaveTableEditorComponent::notifySelectionChanged() {
	initSliders();
}

void VASTWaveTableEditorComponent::setDrawMode(int mode) {
	if (myProcessor->m_iWTEditorDrawMode != mode) {
		myProcessor->m_iWTEditorDrawMode = mode;
		c_waveTableEditorView->getEditorOscilloscope()->updateContent(true); //force update
		if (myProcessor->m_initCompleted.load())
			myProcessor->writeSettingsToFileAsync();
	}
}

void VASTWaveTableEditorComponent::setGridMode(int mode) {
	if (myProcessor->m_iWTEditorGridMode != mode) {
		myProcessor->m_iWTEditorGridMode = mode;
		c_waveTableEditorView->getEditorOscilloscope()->updateContent(true); //force update
		if (myProcessor->m_initCompleted.load())
			myProcessor->writeSettingsToFileAsync();
	}
}

void VASTWaveTableEditorComponent::setBinMode(int mode) {
	if (myProcessor->m_iWTEditorBinMode != mode) {
		myProcessor->m_iWTEditorBinMode = mode;
		myProcessor->m_iWTEditorBinMode = jlimit<int>(0, BinMode::BinMode_NUM - 1, myProcessor->m_iWTEditorBinMode); //safety BinMode
		m_freqviewport->updateContent(true);
		if (myProcessor->m_initCompleted.load())
			myProcessor->writeSettingsToFileAsync();
	}
}

void VASTWaveTableEditorComponent::setBinEditMode(int mode) {
	if (myProcessor->m_iWTEditorBinEditMode != mode) {
		myProcessor->m_iWTEditorBinEditMode = mode;
		myProcessor->m_iWTEditorBinEditMode = jlimit<int>(0, FreqEditMode::FreqEditMode_NUM - 1, myProcessor->m_iWTEditorBinEditMode); //safety FreqEditMode
		m_freqviewport->updateContent(true);
		if (myProcessor->m_initCompleted.load())
			myProcessor->writeSettingsToFileAsync();
	}
}

void VASTWaveTableEditorComponent::startAutoUpdate() {
	if (isShowing()) grabKeyboardFocus();
	m_freqviewport->startAutoUpdate();
	m_positionviewport->startAutoUpdate();
	c_waveTableEditorView->getEditorOscilloscope()->startAutoUpdate(true);	
}

void VASTWaveTableEditorComponent::stopAutoUpdate() {
	m_freqviewport->stopAutoUpdate();
	m_positionviewport->stopAutoUpdate();
	c_waveTableEditorView->getEditorOscilloscope()->stopAutoUpdate();
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="VASTWaveTableEditorComponent"
                 componentName="" parentClasses="public Component, public MultiTimer, public TextEditor::Listener, public Button::Listener, public Slider::Listener"
                 constructorParams="AudioProcessorEditor *editor, AudioProcessor* processor"
                 variableInitialisers="myEditor((VASTAudioProcessorEditor*)editor), myProcessor((VASTAudioProcessor*)processor)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="800" initialHeight="575">
  <METHODS>
    <METHOD name="visibilityChanged()"/>
  </METHODS>
  <BACKGROUND backgroundColour="ff323e44">
    <RECT pos="0 0 0M 0M" fill="linear: -4% -1.538%, 94.667% 106.154%, 0=4fa2a7b5, 1=bd141618"
          hasStroke="0"/>
  </BACKGROUND>
  <GENERICCOMPONENT name="c_concertinaEditor" id="4543d638402b642" memberName="c_concertinaEditor"
                    virtualName="" explicitFocusOrder="0" pos="0 0 100% 100%" class="VASTConcertinaPanel"
                    params="myProcessor, true"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif

//==============================================================================
// Binary resources - be careful not to edit any of these sections!

// JUCER_RESOURCE: add_svg, 1396, "../../Resources/Icons/add.svg"
static const unsigned char resource_VASTWaveTableEditorComponent_add_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,70,45,56,
34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,32,119,105,100,116,104,61,34,50,49,112,120,34,32,104,101,105,103,104,116,61,34,50,48,112,120,34,32,118,105,101,119,
66,111,120,61,34,48,32,48,32,50,49,32,50,48,34,32,118,101,114,115,105,111,110,61,34,49,46,49,34,32,120,109,108,110,115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,
47,115,118,103,34,32,120,109,108,110,115,58,120,108,105,110,107,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,120,108,105,110,107,34,62,10,32,32,32,32,60,33,45,
45,32,71,101,110,101,114,97,116,111,114,58,32,83,107,101,116,99,104,32,51,46,56,46,49,32,40,50,57,54,56,55,41,32,45,32,104,116,116,112,58,47,47,119,119,119,46,98,111,104,101,109,105,97,110,99,111,100,
105,110,103,46,99,111,109,47,115,107,101,116,99,104,32,45,45,62,10,32,32,32,32,60,116,105,116,108,101,62,112,108,117,115,32,91,35,49,52,53,53,93,60,47,116,105,116,108,101,62,10,32,32,32,32,60,100,101,
115,99,62,67,114,101,97,116,101,100,32,119,105,116,104,32,83,107,101,116,99,104,46,60,47,100,101,115,99,62,10,32,32,32,32,60,100,101,102,115,62,60,47,100,101,102,115,62,10,32,32,32,32,60,103,32,105,100,
61,34,80,97,103,101,45,49,34,32,115,116,114,111,107,101,61,34,110,111,110,101,34,32,115,116,114,111,107,101,45,119,105,100,116,104,61,34,49,34,32,102,105,108,108,61,34,110,111,110,101,34,32,102,105,108,
108,45,114,117,108,101,61,34,101,118,101,110,111,100,100,34,62,10,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,68,114,105,98,98,98,108,101,45,68,97,114,107,45,80,114,101,118,105,101,119,34,32,116,114,
97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,45,50,53,57,46,48,48,48,48,48,48,44,32,45,52,56,48,46,48,48,48,48,48,48,41,34,32,102,105,108,108,61,34,35,70,70,70,70,70,70,34,62,
10,32,32,32,32,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,105,99,111,110,115,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,53,54,46,48,48,48,48,48,48,44,32,49,
54,48,46,48,48,48,48,48,48,41,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,60,112,97,116,104,32,100,61,34,77,50,49,52,46,53,53,44,51,50,54,32,76,50,49,50,46,52,53,44,51,50,54,32,76,50,49,50,
46,52,53,44,51,50,57,32,76,50,48,57,46,51,44,51,50,57,32,76,50,48,57,46,51,44,51,51,49,32,76,50,49,50,46,52,53,44,51,51,49,32,76,50,49,50,46,52,53,44,51,51,52,32,76,50,49,52,46,53,53,44,51,51,52,32,76,
50,49,52,46,53,53,44,51,51,49,32,76,50,49,55,46,55,44,51,51,49,32,76,50,49,55,46,55,44,51,50,57,32,76,50,49,52,46,53,53,44,51,50,57,32,76,50,49,52,46,53,53,44,51,50,54,32,90,32,77,50,50,49,46,57,44,51,
51,52,32,76,50,50,52,44,51,51,52,32,76,50,50,52,44,51,50,54,32,76,50,50,49,46,57,44,51,50,54,32,76,50,50,49,46,57,44,51,51,52,32,90,32,77,50,50,49,46,57,44,51,51,56,32,76,50,49,57,46,56,44,51,51,56,32,
76,50,49,57,46,56,44,51,52,48,32,76,50,50,52,44,51,52,48,32,76,50,50,52,44,51,51,54,32,76,50,50,49,46,57,44,51,51,54,32,76,50,50,49,46,57,44,51,51,56,32,90,32,77,50,49,57,46,56,44,51,50,48,32,76,50,49,
57,46,56,44,51,50,50,32,76,50,50,49,46,57,44,51,50,50,32,76,50,50,49,46,57,44,51,50,52,32,76,50,50,52,44,51,50,52,32,76,50,50,52,44,51,50,48,32,76,50,49,57,46,56,44,51,50,48,32,90,32,77,50,48,51,44,51,
51,52,32,76,50,48,53,46,49,44,51,51,52,32,76,50,48,53,46,49,44,51,50,54,32,76,50,48,51,44,51,50,54,32,76,50,48,51,44,51,51,52,32,90,32,77,50,48,53,46,49,44,51,51,54,32,76,50,48,51,44,51,51,54,32,76,50,
48,51,44,51,52,48,32,76,50,48,55,46,50,44,51,52,48,32,76,50,48,55,46,50,44,51,51,56,32,76,50,48,53,46,49,44,51,51,56,32,76,50,48,53,46,49,44,51,51,54,32,90,32,77,50,48,51,44,51,50,48,32,76,50,48,51,44,
51,50,52,32,76,50,48,53,46,49,44,51,50,52,32,76,50,48,53,46,49,44,51,50,50,32,76,50,48,55,46,50,44,51,50,50,32,76,50,48,55,46,50,44,51,50,48,32,76,50,48,51,44,51,50,48,32,90,32,77,50,48,57,46,51,44,51,
52,48,32,76,50,49,55,46,55,44,51,52,48,32,76,50,49,55,46,55,44,51,51,56,32,76,50,48,57,46,51,44,51,51,56,32,76,50,48,57,46,51,44,51,52,48,32,90,32,77,50,48,57,46,51,44,51,50,50,32,76,50,49,55,46,55,44,
51,50,50,32,76,50,49,55,46,55,44,51,50,48,32,76,50,48,57,46,51,44,51,50,48,32,76,50,48,57,46,51,44,51,50,50,32,90,34,32,105,100,61,34,112,108,117,115,45,91,35,49,52,53,53,93,34,62,60,47,112,97,116,104,
62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,60,47,103,62,10,60,47,115,118,103,62,0,0};

const char* VASTWaveTableEditorComponent::add_svg = (const char*) resource_VASTWaveTableEditorComponent_add_svg;
const int VASTWaveTableEditorComponent::add_svgSize = 1396;

// JUCER_RESOURCE: copy_svg, 1135, "../../Resources/Icons/copy.svg"
static const unsigned char resource_VASTWaveTableEditorComponent_copy_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,70,45,56,
34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,32,119,105,100,116,104,61,34,50,48,112,120,34,32,104,101,105,103,104,116,61,34,50,48,112,120,34,32,118,105,101,119,
66,111,120,61,34,48,32,48,32,50,48,32,50,48,34,32,118,101,114,115,105,111,110,61,34,49,46,49,34,32,120,109,108,110,115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,
47,115,118,103,34,32,120,109,108,110,115,58,120,108,105,110,107,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,120,108,105,110,107,34,62,10,32,32,32,32,60,33,45,
45,32,71,101,110,101,114,97,116,111,114,58,32,83,107,101,116,99,104,32,51,46,56,46,49,32,40,50,57,54,56,55,41,32,45,32,104,116,116,112,58,47,47,119,119,119,46,98,111,104,101,109,105,97,110,99,111,100,
105,110,103,46,99,111,109,47,115,107,101,116,99,104,32,45,45,62,10,32,32,32,32,60,116,105,116,108,101,62,111,98,106,101,99,116,95,112,108,97,99,101,109,101,110,116,32,91,35,54,54,93,60,47,116,105,116,
108,101,62,10,32,32,32,32,60,100,101,115,99,62,67,114,101,97,116,101,100,32,119,105,116,104,32,83,107,101,116,99,104,46,60,47,100,101,115,99,62,10,32,32,32,32,60,100,101,102,115,62,60,47,100,101,102,115,
62,10,32,32,32,32,60,103,32,105,100,61,34,80,97,103,101,45,49,34,32,115,116,114,111,107,101,61,34,110,111,110,101,34,32,115,116,114,111,107,101,45,119,105,100,116,104,61,34,49,34,32,102,105,108,108,61,
34,110,111,110,101,34,32,102,105,108,108,45,114,117,108,101,61,34,101,118,101,110,111,100,100,34,62,10,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,68,114,105,98,98,98,108,101,45,68,97,114,107,45,80,
114,101,118,105,101,119,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,45,49,52,48,46,48,48,48,48,48,48,44,32,45,55,57,49,57,46,48,48,48,48,48,48,41,34,32,102,105,
108,108,61,34,35,70,70,70,70,70,70,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,105,99,111,110,115,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,
101,40,53,54,46,48,48,48,48,48,48,44,32,49,54,48,46,48,48,48,48,48,48,41,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,60,112,97,116,104,32,100,61,34,77,57,51,44,55,55,55,57,32,76,57,53,44,
55,55,55,57,32,76,57,53,44,55,55,53,57,32,76,57,51,44,55,55,53,57,32,76,57,51,44,55,55,55,57,32,90,32,77,56,54,44,55,55,55,53,32,76,56,57,44,55,55,55,53,32,76,56,57,44,55,55,54,51,32,76,56,54,44,55,55,
54,51,32,76,56,54,44,55,55,55,53,32,90,32,77,56,54,44,55,55,54,49,32,76,56,52,44,55,55,54,49,32,76,56,52,44,55,55,54,51,32,76,56,52,44,55,55,55,53,32,76,56,52,44,55,55,55,55,32,76,56,54,44,55,55,55,55,
32,76,56,57,44,55,55,55,55,32,76,57,49,44,55,55,55,55,32,76,57,49,44,55,55,55,53,32,76,57,49,44,55,55,54,51,32,76,57,49,44,55,55,54,49,32,76,56,57,44,55,55,54,49,32,76,56,54,44,55,55,54,49,32,90,32,77,
57,57,44,55,55,55,53,32,76,49,48,50,44,55,55,55,53,32,76,49,48,50,44,55,55,54,51,32,76,57,57,44,55,55,54,51,32,76,57,57,44,55,55,55,53,32,90,32,77,49,48,50,44,55,55,54,49,32,76,57,57,44,55,55,54,49,32,
76,57,55,44,55,55,54,49,32,76,57,55,44,55,55,54,51,32,76,57,55,44,55,55,55,53,32,76,57,55,44,55,55,55,55,32,76,57,57,44,55,55,55,55,32,76,49,48,50,44,55,55,55,55,32,76,49,48,52,44,55,55,55,55,32,76,49,
48,52,44,55,55,55,53,32,76,49,48,52,44,55,55,54,51,32,76,49,48,52,44,55,55,54,49,32,76,49,48,50,44,55,55,54,49,32,90,34,32,105,100,61,34,111,98,106,101,99,116,95,112,108,97,99,101,109,101,110,116,45,91,
35,54,54,93,34,62,60,47,112,97,116,104,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,60,47,103,62,10,60,47,115,118,103,62,0,0};

const char* VASTWaveTableEditorComponent::copy_svg = (const char*) resource_VASTWaveTableEditorComponent_copy_svg;
const int VASTWaveTableEditorComponent::copy_svgSize = 1135;

// JUCER_RESOURCE: cut_svg, 1095, "../../Resources/Icons/cut.svg"
static const unsigned char resource_VASTWaveTableEditorComponent_cut_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,70,45,56,
34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,32,119,105,100,116,104,61,34,50,48,112,120,34,32,104,101,105,103,104,116,61,34,50,48,112,120,34,32,118,105,101,119,
66,111,120,61,34,48,32,48,32,50,48,32,50,48,34,32,118,101,114,115,105,111,110,61,34,49,46,49,34,32,120,109,108,110,115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,
47,115,118,103,34,32,120,109,108,110,115,58,120,108,105,110,107,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,120,108,105,110,107,34,62,10,32,32,32,32,60,33,45,
45,32,71,101,110,101,114,97,116,111,114,58,32,83,107,101,116,99,104,32,51,46,56,46,49,32,40,50,57,54,56,55,41,32,45,32,104,116,116,112,58,47,47,119,119,119,46,98,111,104,101,109,105,97,110,99,111,100,
105,110,103,46,99,111,109,47,115,107,101,116,99,104,32,45,45,62,10,32,32,32,32,60,116,105,116,108,101,62,99,117,116,35,52,32,91,35,56,50,49,93,60,47,116,105,116,108,101,62,10,32,32,32,32,60,100,101,115,
99,62,67,114,101,97,116,101,100,32,119,105,116,104,32,83,107,101,116,99,104,46,60,47,100,101,115,99,62,10,32,32,32,32,60,100,101,102,115,62,60,47,100,101,102,115,62,10,32,32,32,32,60,103,32,105,100,61,
34,80,97,103,101,45,49,34,32,115,116,114,111,107,101,61,34,110,111,110,101,34,32,115,116,114,111,107,101,45,119,105,100,116,104,61,34,49,34,32,102,105,108,108,61,34,110,111,110,101,34,32,102,105,108,108,
45,114,117,108,101,61,34,101,118,101,110,111,100,100,34,62,10,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,68,114,105,98,98,98,108,101,45,68,97,114,107,45,80,114,101,118,105,101,119,34,32,116,114,97,
110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,45,52,50,48,46,48,48,48,48,48,48,44,32,45,52,53,49,57,46,48,48,48,48,48,48,41,34,32,102,105,108,108,61,34,35,70,70,70,70,70,70,34,62,
10,32,32,32,32,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,105,99,111,110,115,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,53,54,46,48,48,48,48,48,48,44,32,49,
54,48,46,48,48,48,48,48,48,41,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,60,112,97,116,104,32,100,61,34,77,51,55,56,44,52,51,55,57,32,76,51,56,48,44,52,51,55,57,32,76,51,56,48,44,52,51,55,
54,32,76,51,55,56,44,52,51,55,54,32,76,51,55,56,44,52,51,55,57,32,90,32,77,51,56,48,44,52,51,55,51,32,76,51,55,56,44,52,51,55,51,32,76,51,55,48,44,52,51,55,51,32,76,51,55,48,44,52,51,54,54,32,76,51,54,
56,44,52,51,54,54,32,76,51,54,56,44,52,51,55,51,32,76,51,54,56,44,52,51,55,53,32,76,51,55,48,44,52,51,55,53,32,76,51,55,56,44,52,51,55,53,32,76,51,56,48,44,52,51,55,53,32,76,51,56,52,44,52,51,55,53,32,
76,51,56,52,44,52,51,55,51,32,76,51,56,48,44,52,51,55,51,32,90,32,77,51,55,48,44,52,51,54,53,32,76,51,55,56,44,52,51,54,53,32,76,51,55,56,44,52,51,55,50,32,76,51,56,48,44,52,51,55,50,32,76,51,56,48,44,
52,51,54,53,32,76,51,56,48,44,52,51,54,51,32,76,51,55,56,44,52,51,54,51,32,76,51,55,48,44,52,51,54,51,32,76,51,54,56,44,52,51,54,51,32,76,51,54,52,44,52,51,54,51,32,76,51,54,52,44,52,51,54,53,32,76,51,
54,56,44,52,51,54,53,32,76,51,55,48,44,52,51,54,53,32,90,32,77,51,54,56,44,52,51,54,50,32,76,51,55,48,44,52,51,54,50,32,76,51,55,48,44,52,51,53,57,32,76,51,54,56,44,52,51,53,57,32,76,51,54,56,44,52,51,
54,50,32,90,34,32,105,100,61,34,99,117,116,35,52,45,91,35,56,50,49,93,34,62,60,47,112,97,116,104,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,
32,32,60,47,103,62,10,60,47,115,118,103,62,0,0};

const char* VASTWaveTableEditorComponent::cut_svg = (const char*) resource_VASTWaveTableEditorComponent_cut_svg;
const int VASTWaveTableEditorComponent::cut_svgSize = 1095;

// JUCER_RESOURCE: delete_svg, 1124, "../../Resources/Icons/delete.svg"
static const unsigned char resource_VASTWaveTableEditorComponent_delete_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,70,45,
56,34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,32,119,105,100,116,104,61,34,50,49,112,120,34,32,104,101,105,103,104,116,61,34,50,48,112,120,34,32,118,105,101,119,
66,111,120,61,34,48,32,48,32,50,49,32,50,48,34,32,118,101,114,115,105,111,110,61,34,49,46,49,34,32,120,109,108,110,115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,
47,115,118,103,34,32,120,109,108,110,115,58,120,108,105,110,107,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,120,108,105,110,107,34,62,10,32,32,32,32,60,33,45,
45,32,71,101,110,101,114,97,116,111,114,58,32,83,107,101,116,99,104,32,51,46,56,46,49,32,40,50,57,54,56,55,41,32,45,32,104,116,116,112,58,47,47,119,119,119,46,98,111,104,101,109,105,97,110,99,111,100,
105,110,103,46,99,111,109,47,115,107,101,116,99,104,32,45,45,62,10,32,32,32,32,60,116,105,116,108,101,62,100,101,108,101,116,101,32,91,35,49,52,56,55,93,60,47,116,105,116,108,101,62,10,32,32,32,32,60,
100,101,115,99,62,67,114,101,97,116,101,100,32,119,105,116,104,32,83,107,101,116,99,104,46,60,47,100,101,115,99,62,10,32,32,32,32,60,100,101,102,115,62,60,47,100,101,102,115,62,10,32,32,32,32,60,103,32,
105,100,61,34,80,97,103,101,45,49,34,32,115,116,114,111,107,101,61,34,110,111,110,101,34,32,115,116,114,111,107,101,45,119,105,100,116,104,61,34,49,34,32,102,105,108,108,61,34,110,111,110,101,34,32,102,
105,108,108,45,114,117,108,101,61,34,101,118,101,110,111,100,100,34,62,10,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,68,114,105,98,98,98,108,101,45,68,97,114,107,45,80,114,101,118,105,101,119,34,
32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,45,49,55,57,46,48,48,48,48,48,48,44,32,45,51,54,48,46,48,48,48,48,48,48,41,34,32,102,105,108,108,61,34,35,70,70,70,70,70,
70,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,105,99,111,110,115,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,53,54,46,48,48,48,48,48,48,
44,32,49,54,48,46,48,48,48,48,48,48,41,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,60,112,97,116,104,32,100,61,34,77,49,51,48,46,51,53,44,50,49,54,32,76,49,51,50,46,52,53,44,50,49,54,32,76,
49,51,50,46,52,53,44,50,48,56,32,76,49,51,48,46,51,53,44,50,48,56,32,76,49,51,48,46,51,53,44,50,49,54,32,90,32,77,49,51,52,46,53,53,44,50,49,54,32,76,49,51,54,46,54,53,44,50,49,54,32,76,49,51,54,46,54,
53,44,50,48,56,32,76,49,51,52,46,53,53,44,50,48,56,32,76,49,51,52,46,53,53,44,50,49,54,32,90,32,77,49,50,56,46,50,53,44,50,49,56,32,76,49,51,56,46,55,53,44,50,49,56,32,76,49,51,56,46,55,53,44,50,48,54,
32,76,49,50,56,46,50,53,44,50,48,54,32,76,49,50,56,46,50,53,44,50,49,56,32,90,32,77,49,51,48,46,51,53,44,50,48,52,32,76,49,51,54,46,54,53,44,50,48,52,32,76,49,51,54,46,54,53,44,50,48,50,32,76,49,51,48,
46,51,53,44,50,48,50,32,76,49,51,48,46,51,53,44,50,48,52,32,90,32,77,49,51,56,46,55,53,44,50,48,52,32,76,49,51,56,46,55,53,44,50,48,48,32,76,49,50,56,46,50,53,44,50,48,48,32,76,49,50,56,46,50,53,44,50,
48,52,32,76,49,50,51,44,50,48,52,32,76,49,50,51,44,50,48,54,32,76,49,50,54,46,49,53,44,50,48,54,32,76,49,50,54,46,49,53,44,50,50,48,32,76,49,52,48,46,56,53,44,50,50,48,32,76,49,52,48,46,56,53,44,50,48,
54,32,76,49,52,52,44,50,48,54,32,76,49,52,52,44,50,48,52,32,76,49,51,56,46,55,53,44,50,48,52,32,90,34,32,105,100,61,34,100,101,108,101,116,101,45,91,35,49,52,56,55,93,34,62,60,47,112,97,116,104,62,10,
32,32,32,32,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,60,47,103,62,10,60,47,115,118,103,62,0,0};

const char* VASTWaveTableEditorComponent::delete_svg = (const char*) resource_VASTWaveTableEditorComponent_delete_svg;
const int VASTWaveTableEditorComponent::delete_svgSize = 1124;

// JUCER_RESOURCE: duplicate_svg, 1641, "../../Resources/Icons/duplicate.svg"
static const unsigned char resource_VASTWaveTableEditorComponent_duplicate_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,70,
45,56,34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,32,119,105,100,116,104,61,34,50,48,112,120,34,32,104,101,105,103,104,116,61,34,50,48,112,120,34,32,118,105,101,
119,66,111,120,61,34,48,32,48,32,50,48,32,50,48,34,32,118,101,114,115,105,111,110,61,34,49,46,49,34,32,120,109,108,110,115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,
48,48,47,115,118,103,34,32,120,109,108,110,115,58,120,108,105,110,107,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,120,108,105,110,107,34,62,10,32,32,32,32,60,
33,45,45,32,71,101,110,101,114,97,116,111,114,58,32,83,107,101,116,99,104,32,51,46,56,46,49,32,40,50,57,54,56,55,41,32,45,32,104,116,116,112,58,47,47,119,119,119,46,98,111,104,101,109,105,97,110,99,111,
100,105,110,103,46,99,111,109,47,115,107,101,116,99,104,32,45,45,62,10,32,32,32,32,60,116,105,116,108,101,62,100,117,112,108,105,99,97,116,101,95,114,111,117,110,100,32,91,35,56,51,53,93,60,47,116,105,
116,108,101,62,10,32,32,32,32,60,100,101,115,99,62,67,114,101,97,116,101,100,32,119,105,116,104,32,83,107,101,116,99,104,46,60,47,100,101,115,99,62,10,32,32,32,32,60,100,101,102,115,62,60,47,100,101,102,
115,62,10,32,32,32,32,60,103,32,105,100,61,34,80,97,103,101,45,49,34,32,115,116,114,111,107,101,61,34,110,111,110,101,34,32,115,116,114,111,107,101,45,119,105,100,116,104,61,34,49,34,32,102,105,108,108,
61,34,110,111,110,101,34,32,102,105,108,108,45,114,117,108,101,61,34,101,118,101,110,111,100,100,34,62,10,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,68,114,105,98,98,98,108,101,45,68,97,114,107,45,
80,114,101,118,105,101,119,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,45,50,54,48,46,48,48,48,48,48,48,44,32,45,52,52,55,57,46,48,48,48,48,48,48,41,34,32,102,105,
108,108,61,34,35,70,70,70,70,70,70,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,105,99,111,110,115,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,
101,40,53,54,46,48,48,48,48,48,48,44,32,49,54,48,46,48,48,48,48,48,48,41,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,60,112,97,116,104,32,100,61,34,77,50,50,49,44,52,51,51,51,32,76,50,50,
48,44,52,51,51,51,32,76,50,50,48,44,52,51,50,53,32,67,50,50,48,44,52,51,50,51,46,56,57,53,32,50,49,57,46,49,48,53,44,52,51,50,51,32,50,49,56,44,52,51,50,51,32,76,50,49,48,44,52,51,50,51,32,76,50,49,48,
44,52,51,50,50,32,67,50,49,48,44,52,51,50,49,46,52,52,56,32,50,49,48,46,52,52,56,44,52,51,50,49,32,50,49,49,44,52,51,50,49,32,76,50,50,49,44,52,51,50,49,32,67,50,50,49,46,53,53,50,44,52,51,50,49,32,50,
50,50,44,52,51,50,49,46,52,52,56,32,50,50,50,44,52,51,50,50,32,76,50,50,50,44,52,51,51,50,32,67,50,50,50,44,52,51,51,50,46,53,53,50,32,50,50,49,46,53,53,50,44,52,51,51,51,32,50,50,49,44,52,51,51,51,32,
76,50,50,49,44,52,51,51,51,32,90,32,77,50,49,56,44,52,51,51,51,32,76,50,49,49,44,52,51,51,51,32,67,50,49,48,46,52,52,56,44,52,51,51,51,32,50,49,48,44,52,51,51,50,46,53,53,50,32,50,49,48,44,52,51,51,50,
32,76,50,49,48,44,52,51,50,53,32,76,50,49,55,44,52,51,50,53,32,67,50,49,55,46,53,53,50,44,52,51,50,53,32,50,49,56,44,52,51,50,53,46,52,52,56,32,50,49,56,44,52,51,50,54,32,76,50,49,56,44,52,51,51,51,32,
90,32,77,50,49,55,44,52,51,51,55,32,76,50,48,55,44,52,51,51,55,32,67,50,48,54,46,52,52,56,44,52,51,51,55,32,50,48,54,44,52,51,51,54,46,53,53,50,32,50,48,54,44,52,51,51,54,32,76,50,48,54,44,52,51,50,54,
32,67,50,48,54,44,52,51,50,53,46,52,52,56,32,50,48,54,46,52,52,56,44,52,51,50,53,32,50,48,55,44,52,51,50,53,32,76,50,48,56,44,52,51,50,53,32,76,50,48,56,44,52,51,51,51,32,67,50,48,56,44,52,51,51,52,46,
49,48,53,32,50,48,56,46,56,57,53,44,52,51,51,53,32,50,49,48,44,52,51,51,53,32,76,50,49,56,44,52,51,51,53,32,76,50,49,56,44,52,51,51,54,32,67,50,49,56,44,52,51,51,54,46,53,53,50,32,50,49,55,46,53,53,50,
44,52,51,51,55,32,50,49,55,44,52,51,51,55,32,76,50,49,55,44,52,51,51,55,32,90,32,77,50,50,50,44,52,51,49,57,32,76,50,49,48,44,52,51,49,57,32,67,50,48,56,46,56,57,53,44,52,51,49,57,32,50,48,56,44,52,51,
49,57,46,56,57,53,32,50,48,56,44,52,51,50,49,32,76,50,48,56,44,52,51,50,51,32,76,50,48,54,44,52,51,50,51,32,67,50,48,52,46,56,57,53,44,52,51,50,51,32,50,48,52,44,52,51,50,51,46,56,57,53,32,50,48,52,44,
52,51,50,53,32,76,50,48,52,44,52,51,51,55,32,67,50,48,52,44,52,51,51,56,46,49,48,53,32,50,48,52,46,56,57,53,44,52,51,51,57,32,50,48,54,44,52,51,51,57,32,76,50,49,56,44,52,51,51,57,32,67,50,49,57,46,49,
48,53,44,52,51,51,57,32,50,50,48,44,52,51,51,56,46,49,48,53,32,50,50,48,44,52,51,51,55,32,76,50,50,48,44,52,51,51,53,32,76,50,50,50,44,52,51,51,53,32,67,50,50,51,46,49,48,53,44,52,51,51,53,32,50,50,52,
44,52,51,51,52,46,49,48,53,32,50,50,52,44,52,51,51,51,32,76,50,50,52,44,52,51,50,49,32,67,50,50,52,44,52,51,49,57,46,56,57,53,32,50,50,51,46,49,48,53,44,52,51,49,57,32,50,50,50,44,52,51,49,57,32,76,50,
50,50,44,52,51,49,57,32,90,34,32,105,100,61,34,100,117,112,108,105,99,97,116,101,95,114,111,117,110,100,45,91,35,56,51,53,93,34,62,60,47,112,97,116,104,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,47,
103,62,10,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,60,47,103,62,10,60,47,115,118,103,62,0,0};

const char* VASTWaveTableEditorComponent::duplicate_svg = (const char*) resource_VASTWaveTableEditorComponent_duplicate_svg;
const int VASTWaveTableEditorComponent::duplicate_svgSize = 1641;

// JUCER_RESOURCE: minus_svg, 1301, "../../Resources/Icons/minus.svg"
static const unsigned char resource_VASTWaveTableEditorComponent_minus_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,70,45,
56,34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,32,119,105,100,116,104,61,34,50,49,112,120,34,32,104,101,105,103,104,116,61,34,50,48,112,120,34,32,118,105,101,119,
66,111,120,61,34,48,32,48,32,50,49,32,50,48,34,32,118,101,114,115,105,111,110,61,34,49,46,49,34,32,120,109,108,110,115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,
47,115,118,103,34,32,120,109,108,110,115,58,120,108,105,110,107,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,120,108,105,110,107,34,62,10,32,32,32,32,60,33,45,
45,32,71,101,110,101,114,97,116,111,114,58,32,83,107,101,116,99,104,32,51,46,56,46,49,32,40,50,57,54,56,55,41,32,45,32,104,116,116,112,58,47,47,119,119,119,46,98,111,104,101,109,105,97,110,99,111,100,
105,110,103,46,99,111,109,47,115,107,101,116,99,104,32,45,45,62,10,32,32,32,32,60,116,105,116,108,101,62,109,105,110,117,115,32,91,35,49,52,53,54,93,60,47,116,105,116,108,101,62,10,32,32,32,32,60,100,
101,115,99,62,67,114,101,97,116,101,100,32,119,105,116,104,32,83,107,101,116,99,104,46,60,47,100,101,115,99,62,10,32,32,32,32,60,100,101,102,115,62,60,47,100,101,102,115,62,10,32,32,32,32,60,103,32,105,
100,61,34,80,97,103,101,45,49,34,32,115,116,114,111,107,101,61,34,110,111,110,101,34,32,115,116,114,111,107,101,45,119,105,100,116,104,61,34,49,34,32,102,105,108,108,61,34,110,111,110,101,34,32,102,105,
108,108,45,114,117,108,101,61,34,101,118,101,110,111,100,100,34,62,10,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,68,114,105,98,98,98,108,101,45,68,97,114,107,45,80,114,101,118,105,101,119,34,32,116,
114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,45,50,49,57,46,48,48,48,48,48,48,44,32,45,52,56,48,46,48,48,48,48,48,48,41,34,32,102,105,108,108,61,34,35,70,70,70,70,70,70,34,
62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,105,99,111,110,115,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,53,54,46,48,48,48,48,48,48,44,32,
49,54,48,46,48,48,48,48,48,48,41,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,60,112,97,116,104,32,100,61,34,77,49,54,57,46,51,44,51,51,49,32,76,49,55,55,46,55,44,51,51,49,32,76,49,55,55,46,
55,44,51,50,57,32,76,49,54,57,46,51,44,51,50,57,32,76,49,54,57,46,51,44,51,51,49,32,90,32,77,49,56,49,46,57,44,51,51,52,32,76,49,56,52,44,51,51,52,32,76,49,56,52,44,51,50,54,32,76,49,56,49,46,57,44,51,
50,54,32,76,49,56,49,46,57,44,51,51,52,32,90,32,77,49,56,49,46,57,44,51,51,56,32,76,49,55,57,46,56,44,51,51,56,32,76,49,55,57,46,56,44,51,52,48,32,76,49,56,52,44,51,52,48,32,76,49,56,52,44,51,51,54,32,
76,49,56,49,46,57,44,51,51,54,32,76,49,56,49,46,57,44,51,51,56,32,90,32,77,49,55,57,46,56,44,51,50,48,32,76,49,55,57,46,56,44,51,50,50,32,76,49,56,49,46,57,44,51,50,50,32,76,49,56,49,46,57,44,51,50,52,
32,76,49,56,52,44,51,50,52,32,76,49,56,52,44,51,50,48,32,76,49,55,57,46,56,44,51,50,48,32,90,32,77,49,54,51,44,51,51,52,32,76,49,54,53,46,49,44,51,51,52,32,76,49,54,53,46,49,44,51,50,54,32,76,49,54,51,
44,51,50,54,32,76,49,54,51,44,51,51,52,32,90,32,77,49,54,53,46,49,44,51,51,54,32,76,49,54,51,44,51,51,54,32,76,49,54,51,44,51,52,48,32,76,49,54,55,46,50,44,51,52,48,32,76,49,54,55,46,50,44,51,51,56,32,
76,49,54,53,46,49,44,51,51,56,32,76,49,54,53,46,49,44,51,51,54,32,90,32,77,49,54,51,44,51,50,48,32,76,49,54,51,44,51,50,52,32,76,49,54,53,46,49,44,51,50,52,32,76,49,54,53,46,49,44,51,50,50,32,76,49,54,
55,46,50,44,51,50,50,32,76,49,54,55,46,50,44,51,50,48,32,76,49,54,51,44,51,50,48,32,90,32,77,49,54,57,46,51,44,51,52,48,32,76,49,55,55,46,55,44,51,52,48,32,76,49,55,55,46,55,44,51,51,56,32,76,49,54,57,
46,51,44,51,51,56,32,76,49,54,57,46,51,44,51,52,48,32,90,32,77,49,54,57,46,51,44,51,50,50,32,76,49,55,55,46,55,44,51,50,50,32,76,49,55,55,46,55,44,51,50,48,32,76,49,54,57,46,51,44,51,50,48,32,76,49,54,
57,46,51,44,51,50,50,32,90,34,32,105,100,61,34,109,105,110,117,115,45,91,35,49,52,53,54,93,34,62,60,47,112,97,116,104,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,32,32,32,32,
60,47,103,62,10,32,32,32,32,60,47,103,62,10,60,47,115,118,103,62,0,0};

const char* VASTWaveTableEditorComponent::minus_svg = (const char*) resource_VASTWaveTableEditorComponent_minus_svg;
const int VASTWaveTableEditorComponent::minus_svgSize = 1301;

// JUCER_RESOURCE: normalize_svg, 1091, "../../Resources/Icons/normalize.svg"
static const unsigned char resource_VASTWaveTableEditorComponent_normalize_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,70,
45,56,34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,32,119,105,100,116,104,61,34,50,48,112,120,34,32,104,101,105,103,104,116,61,34,50,48,112,120,34,32,118,105,101,
119,66,111,120,61,34,48,32,48,32,50,48,32,50,48,34,32,118,101,114,115,105,111,110,61,34,49,46,49,34,32,120,109,108,110,115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,
48,48,47,115,118,103,34,32,120,109,108,110,115,58,120,108,105,110,107,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,120,108,105,110,107,34,62,10,32,32,32,32,60,
33,45,45,32,71,101,110,101,114,97,116,111,114,58,32,83,107,101,116,99,104,32,51,46,56,46,49,32,40,50,57,54,56,55,41,32,45,32,104,116,116,112,58,47,47,119,119,119,46,98,111,104,101,109,105,97,110,99,111,
100,105,110,103,46,99,111,109,47,115,107,101,116,99,104,32,45,45,62,10,32,32,32,32,60,116,105,116,108,101,62,118,111,108,117,109,101,95,108,111,119,32,91,35,57,50,53,93,60,47,116,105,116,108,101,62,10,
32,32,32,32,60,100,101,115,99,62,67,114,101,97,116,101,100,32,119,105,116,104,32,83,107,101,116,99,104,46,60,47,100,101,115,99,62,10,32,32,32,32,60,100,101,102,115,62,60,47,100,101,102,115,62,10,32,32,
32,32,60,103,32,105,100,61,34,80,97,103,101,45,49,34,32,115,116,114,111,107,101,61,34,110,111,110,101,34,32,115,116,114,111,107,101,45,119,105,100,116,104,61,34,49,34,32,102,105,108,108,61,34,110,111,
110,101,34,32,102,105,108,108,45,114,117,108,101,61,34,101,118,101,110,111,100,100,34,62,10,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,68,114,105,98,98,98,108,101,45,68,97,114,107,45,80,114,101,118,
105,101,119,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,45,50,54,48,46,48,48,48,48,48,48,44,32,45,52,48,55,57,46,48,48,48,48,48,48,41,34,32,102,105,108,108,61,34,
35,70,70,70,70,70,70,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,105,99,111,110,115,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,53,54,46,
48,48,48,48,48,48,44,32,49,54,48,46,48,48,48,48,48,48,41,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,60,112,97,116,104,32,100,61,34,77,50,50,50,44,51,57,51,51,46,53,32,76,50,50,52,44,51,57,
51,51,46,53,32,76,50,50,52,44,51,57,50,53,46,53,32,76,50,50,50,44,51,57,50,53,46,53,32,76,50,50,50,44,51,57,51,51,46,53,32,90,32,77,50,48,54,44,51,57,51,49,46,53,32,76,50,48,56,44,51,57,51,49,46,53,32,
76,50,48,56,44,51,57,50,55,46,53,32,76,50,48,54,44,51,57,50,55,46,53,32,76,50,48,54,44,51,57,51,49,46,53,32,90,32,77,50,49,48,44,51,57,50,54,46,50,51,54,32,67,50,49,50,46,57,50,50,44,51,57,50,52,46,55,
55,53,32,50,49,53,46,49,54,52,44,51,57,50,51,46,54,53,52,32,50,49,56,44,51,57,50,50,46,50,51,54,32,76,50,49,56,44,51,57,51,53,46,55,54,52,32,67,50,49,53,46,49,54,53,44,51,57,51,52,46,51,52,54,32,50,49,
50,46,57,50,49,44,51,57,51,51,46,50,50,53,32,50,49,48,44,51,57,51,49,46,55,54,52,32,76,50,49,48,44,51,57,50,54,46,50,51,54,32,90,32,77,50,48,52,44,51,57,50,53,46,53,32,76,50,48,52,44,51,57,51,51,46,53,
32,76,50,48,55,46,54,54,55,44,51,57,51,51,46,53,32,76,50,50,48,44,51,57,51,57,32,76,50,50,48,44,51,57,49,57,32,76,50,48,55,46,54,54,55,44,51,57,50,53,46,53,32,76,50,48,52,44,51,57,50,53,46,53,32,90,34,
32,105,100,61,34,118,111,108,117,109,101,95,108,111,119,45,91,35,57,50,53,93,34,62,60,47,112,97,116,104,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,32,32,32,32,60,47,103,62,10,
32,32,32,32,60,47,103,62,10,60,47,115,118,103,62,0,0};

const char* VASTWaveTableEditorComponent::normalize_svg = (const char*) resource_VASTWaveTableEditorComponent_normalize_svg;
const int VASTWaveTableEditorComponent::normalize_svgSize = 1091;

// JUCER_RESOURCE: normalize2_svg, 960, "../../Resources/Icons/normalize2.svg"
static const unsigned char resource_VASTWaveTableEditorComponent_normalize2_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,70,
45,56,34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,32,119,105,100,116,104,61,34,50,49,112,120,34,32,104,101,105,103,104,116,61,34,50,48,112,120,34,32,118,105,101,
119,66,111,120,61,34,48,32,48,32,50,49,32,50,48,34,32,118,101,114,115,105,111,110,61,34,49,46,49,34,32,120,109,108,110,115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,
48,48,47,115,118,103,34,32,120,109,108,110,115,58,120,108,105,110,107,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,120,108,105,110,107,34,62,10,32,32,32,32,60,
33,45,45,32,71,101,110,101,114,97,116,111,114,58,32,83,107,101,116,99,104,32,51,46,56,46,49,32,40,50,57,54,56,55,41,32,45,32,104,116,116,112,58,47,47,119,119,119,46,98,111,104,101,109,105,97,110,99,111,
100,105,110,103,46,99,111,109,47,115,107,101,116,99,104,32,45,45,62,10,32,32,32,32,60,116,105,116,108,101,62,119,105,110,100,111,119,95,101,113,117,97,108,32,91,35,49,52,54,53,93,60,47,116,105,116,108,
101,62,10,32,32,32,32,60,100,101,115,99,62,67,114,101,97,116,101,100,32,119,105,116,104,32,83,107,101,116,99,104,46,60,47,100,101,115,99,62,10,32,32,32,32,60,100,101,102,115,62,60,47,100,101,102,115,62,
10,32,32,32,32,60,103,32,105,100,61,34,80,97,103,101,45,49,34,32,115,116,114,111,107,101,61,34,110,111,110,101,34,32,115,116,114,111,107,101,45,119,105,100,116,104,61,34,49,34,32,102,105,108,108,61,34,
110,111,110,101,34,32,102,105,108,108,45,114,117,108,101,61,34,101,118,101,110,111,100,100,34,62,10,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,68,114,105,98,98,98,108,101,45,68,97,114,107,45,80,114,
101,118,105,101,119,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,45,50,53,57,46,48,48,48,48,48,48,44,32,45,52,52,48,46,48,48,48,48,48,48,41,34,32,102,105,108,108,
61,34,35,70,70,70,70,70,70,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,105,99,111,110,115,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,53,
54,46,48,48,48,48,48,48,44,32,49,54,48,46,48,48,48,48,48,48,41,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,60,112,97,116,104,32,100,61,34,77,50,48,55,46,50,44,50,57,51,32,76,50,49,57,46,56,
44,50,57,51,32,76,50,49,57,46,56,44,50,57,49,32,76,50,48,55,46,50,44,50,57,49,32,76,50,48,55,46,50,44,50,57,51,32,90,32,77,50,48,55,46,50,44,50,56,57,32,76,50,49,57,46,56,44,50,56,57,32,76,50,49,57,46,
56,44,50,56,55,32,76,50,48,55,46,50,44,50,56,55,32,76,50,48,55,46,50,44,50,56,57,32,90,32,77,50,48,53,46,49,44,50,57,56,32,76,50,50,49,46,57,44,50,57,56,32,76,50,50,49,46,57,44,50,56,50,32,76,50,48,53,
46,49,44,50,56,50,32,76,50,48,53,46,49,44,50,57,56,32,90,32,77,50,48,51,44,51,48,48,32,76,50,50,52,44,51,48,48,32,76,50,50,52,44,50,56,48,32,76,50,48,51,44,50,56,48,32,76,50,48,51,44,51,48,48,32,90,34,
32,105,100,61,34,119,105,110,100,111,119,95,101,113,117,97,108,45,91,35,49,52,54,53,93,34,62,60,47,112,97,116,104,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,32,32,32,32,60,47,
103,62,10,32,32,32,32,60,47,103,62,10,60,47,115,118,103,62,0,0};

const char* VASTWaveTableEditorComponent::normalize2_svg = (const char*) resource_VASTWaveTableEditorComponent_normalize2_svg;
const int VASTWaveTableEditorComponent::normalize2_svgSize = 960;

// JUCER_RESOURCE: normalize3_svg, 1964, "../../Resources/Icons/normalize3.svg"
static const unsigned char resource_VASTWaveTableEditorComponent_normalize3_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,70,
45,56,34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,32,119,105,100,116,104,61,34,50,48,112,120,34,32,104,101,105,103,104,116,61,34,50,48,112,120,34,32,118,105,101,
119,66,111,120,61,34,48,32,48,32,50,48,32,50,48,34,32,118,101,114,115,105,111,110,61,34,49,46,49,34,32,120,109,108,110,115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,
48,48,47,115,118,103,34,32,120,109,108,110,115,58,120,108,105,110,107,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,120,108,105,110,107,34,62,10,32,32,32,32,60,
33,45,45,32,71,101,110,101,114,97,116,111,114,58,32,83,107,101,116,99,104,32,51,46,56,46,49,32,40,50,57,54,56,55,41,32,45,32,104,116,116,112,58,47,47,119,119,119,46,98,111,104,101,109,105,97,110,99,111,
100,105,110,103,46,99,111,109,47,115,107,101,116,99,104,32,45,45,62,10,32,32,32,32,60,116,105,116,108,101,62,97,114,114,111,119,95,105,110,115,105,100,101,32,91,35,50,54,49,93,60,47,116,105,116,108,101,
62,10,32,32,32,32,60,100,101,115,99,62,67,114,101,97,116,101,100,32,119,105,116,104,32,83,107,101,116,99,104,46,60,47,100,101,115,99,62,10,32,32,32,32,60,100,101,102,115,62,60,47,100,101,102,115,62,10,
32,32,32,32,60,103,32,105,100,61,34,80,97,103,101,45,49,34,32,115,116,114,111,107,101,61,34,110,111,110,101,34,32,115,116,114,111,107,101,45,119,105,100,116,104,61,34,49,34,32,102,105,108,108,61,34,110,
111,110,101,34,32,102,105,108,108,45,114,117,108,101,61,34,101,118,101,110,111,100,100,34,62,10,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,68,114,105,98,98,98,108,101,45,68,97,114,107,45,80,114,101,
118,105,101,119,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,45,49,48,48,46,48,48,48,48,48,48,44,32,45,54,57,57,57,46,48,48,48,48,48,48,41,34,32,102,105,108,108,
61,34,35,70,70,70,70,70,70,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,105,99,111,110,115,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,53,
54,46,48,48,48,48,48,48,44,32,49,54,48,46,48,48,48,48,48,48,41,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,60,112,97,116,104,32,100,61,34,77,54,51,44,54,56,53,55,32,76,53,56,44,54,56,53,55,
32,76,53,56,44,54,56,53,50,46,56,54,51,32,67,53,56,44,54,56,53,50,46,52,49,56,32,53,56,46,53,51,57,44,54,56,53,50,46,49,57,53,32,53,56,46,56,53,52,44,54,56,53,50,46,53,49,32,76,54,48,46,53,51,54,44,54,
56,53,52,46,49,57,51,32,67,54,48,46,57,50,54,44,54,56,53,52,46,53,56,51,32,54,49,46,53,54,44,54,56,53,52,46,53,56,51,32,54,49,46,57,53,44,54,56,53,52,46,49,57,51,32,67,54,50,46,51,52,49,44,54,56,53,51,
46,56,48,50,32,54,50,46,51,52,49,44,54,56,53,51,46,49,54,57,32,54,49,46,57,53,44,54,56,53,50,46,55,55,57,32,76,53,56,46,52,49,51,44,54,56,52,57,46,50,52,50,32,67,53,55,46,54,51,51,44,54,56,52,56,46,52,
54,49,32,53,54,46,51,54,55,44,54,56,52,56,46,52,54,49,32,53,53,46,53,56,55,44,54,56,52,57,46,50,52,50,32,67,53,53,46,48,50,49,44,54,56,52,57,46,56,48,56,32,53,52,46,57,48,49,44,54,56,52,57,46,57,50,55,
32,53,50,46,48,53,44,54,56,53,50,46,55,55,56,32,67,53,49,46,54,54,44,54,56,53,51,46,49,54,57,32,53,49,46,54,54,44,54,56,53,51,46,56,48,50,32,53,50,46,48,53,49,44,54,56,53,52,46,49,57,51,32,67,53,50,46,
52,52,49,44,54,56,53,52,46,53,56,51,32,53,51,46,48,55,52,44,54,56,53,52,46,53,56,51,32,53,51,46,52,54,53,44,54,56,53,52,46,49,57,51,32,76,53,53,46,49,52,54,44,54,56,53,50,46,53,49,50,32,67,53,53,46,52,
54,49,44,54,56,53,50,46,49,57,55,32,53,54,44,54,56,53,50,46,52,50,32,53,54,44,54,56,53,50,46,56,54,53,32,76,53,54,44,54,56,53,55,32,76,52,53,44,54,56,53,55,32,67,52,52,46,52,52,56,44,54,56,53,55,32,52,
52,44,54,56,53,55,46,52,52,56,32,52,52,44,54,56,53,56,32,67,52,52,44,54,56,53,56,46,53,53,50,32,52,52,46,52,52,56,44,54,56,53,57,32,52,53,44,54,56,53,57,32,76,54,51,44,54,56,53,57,32,67,54,51,46,53,53,
50,44,54,56,53,57,32,54,52,44,54,56,53,56,46,53,53,50,32,54,52,44,54,56,53,56,32,67,54,52,44,54,56,53,55,46,52,52,56,32,54,51,46,53,53,50,44,54,56,53,55,32,54,51,44,54,56,53,55,32,77,52,53,44,54,56,52,
49,32,76,53,48,44,54,56,52,49,32,76,53,48,44,54,56,52,53,46,49,51,55,32,67,53,48,44,54,56,52,53,46,53,56,50,32,52,57,46,52,54,49,44,54,56,52,53,46,56,48,53,32,52,57,46,49,52,54,44,54,56,52,53,46,52,57,
32,76,52,55,46,52,54,52,44,54,56,52,51,46,56,48,55,32,67,52,55,46,48,55,52,44,54,56,52,51,46,52,49,55,32,52,54,46,52,52,44,54,56,52,51,46,52,49,55,32,52,54,46,48,53,44,54,56,52,51,46,56,48,55,32,67,52,
53,46,54,54,44,54,56,52,52,46,49,57,56,32,52,53,46,54,54,44,54,56,52,52,46,56,51,49,32,52,54,46,48,53,44,54,56,52,53,46,50,50,49,32,76,52,57,46,53,56,55,44,54,56,52,56,46,55,53,56,32,67,53,48,46,51,54,
56,44,54,56,52,57,46,53,51,57,32,53,49,46,54,51,51,44,54,56,52,57,46,53,51,57,32,53,50,46,52,49,51,44,54,56,52,56,46,55,53,56,32,67,53,50,46,57,56,51,44,54,56,52,56,46,49,56,57,32,53,51,46,48,57,56,44,
54,56,52,56,46,48,55,52,32,53,53,46,57,53,44,54,56,52,53,46,50,50,50,32,67,53,54,46,51,52,44,54,56,52,52,46,56,51,49,32,53,54,46,51,52,44,54,56,52,52,46,49,57,56,32,53,53,46,57,53,44,54,56,52,51,46,56,
48,55,32,67,53,53,46,53,53,57,44,54,56,52,51,46,52,49,55,32,53,52,46,57,50,54,44,54,56,52,51,46,52,49,55,32,53,52,46,53,51,53,44,54,56,52,51,46,56,48,55,32,76,53,50,46,56,53,52,44,54,56,52,53,46,52,56,
56,32,67,53,50,46,53,51,57,44,54,56,52,53,46,56,48,51,32,53,50,44,54,56,52,53,46,53,56,32,53,50,44,54,56,52,53,46,49,51,53,32,76,53,50,44,54,56,52,49,32,76,54,51,44,54,56,52,49,32,67,54,51,46,53,53,50,
44,54,56,52,49,32,54,52,44,54,56,52,48,46,53,53,50,32,54,52,44,54,56,52,48,32,67,54,52,44,54,56,51,57,46,52,52,56,32,54,51,46,53,53,50,44,54,56,51,57,32,54,51,44,54,56,51,57,32,76,52,53,44,54,56,51,57,
32,67,52,52,46,52,52,56,44,54,56,51,57,32,52,52,44,54,56,51,57,46,52,52,56,32,52,52,44,54,56,52,48,32,67,52,52,44,54,56,52,48,46,53,53,50,32,52,52,46,52,52,56,44,54,56,52,49,32,52,53,44,54,56,52,49,34,
32,105,100,61,34,97,114,114,111,119,95,105,110,115,105,100,101,45,91,35,50,54,49,93,34,62,60,47,112,97,116,104,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,32,32,32,32,60,47,103,
62,10,32,32,32,32,60,47,103,62,10,60,47,115,118,103,62,0,0};

const char* VASTWaveTableEditorComponent::normalize3_svg = (const char*) resource_VASTWaveTableEditorComponent_normalize3_svg;
const int VASTWaveTableEditorComponent::normalize3_svgSize = 1964;

// JUCER_RESOURCE: paste_svg, 1977, "../../Resources/Icons/paste.svg"
static const unsigned char resource_VASTWaveTableEditorComponent_paste_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,70,45,
56,34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,32,119,105,100,116,104,61,34,50,48,112,120,34,32,104,101,105,103,104,116,61,34,50,48,112,120,34,32,118,105,101,119,
66,111,120,61,34,48,32,48,32,50,48,32,50,48,34,32,118,101,114,115,105,111,110,61,34,49,46,49,34,32,120,109,108,110,115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,
47,115,118,103,34,32,120,109,108,110,115,58,120,108,105,110,107,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,120,108,105,110,107,34,62,10,32,32,32,32,60,33,45,
45,32,71,101,110,101,114,97,116,111,114,58,32,83,107,101,116,99,104,32,51,46,56,46,49,32,40,50,57,54,56,55,41,32,45,32,104,116,116,112,58,47,47,119,119,119,46,98,111,104,101,109,105,97,110,99,111,100,
105,110,103,46,99,111,109,47,115,107,101,116,99,104,32,45,45,62,10,32,32,32,32,60,116,105,116,108,101,62,97,114,114,111,119,95,105,110,95,100,111,119,110,32,91,35,51,52,53,93,60,47,116,105,116,108,101,
62,10,32,32,32,32,60,100,101,115,99,62,67,114,101,97,116,101,100,32,119,105,116,104,32,83,107,101,116,99,104,46,60,47,100,101,115,99,62,10,32,32,32,32,60,100,101,102,115,62,60,47,100,101,102,115,62,10,
32,32,32,32,60,103,32,105,100,61,34,80,97,103,101,45,49,34,32,115,116,114,111,107,101,61,34,110,111,110,101,34,32,115,116,114,111,107,101,45,119,105,100,116,104,61,34,49,34,32,102,105,108,108,61,34,110,
111,110,101,34,32,102,105,108,108,45,114,117,108,101,61,34,101,118,101,110,111,100,100,34,62,10,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,68,114,105,98,98,98,108,101,45,68,97,114,107,45,80,114,101,
118,105,101,119,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,45,51,52,48,46,48,48,48,48,48,48,44,32,45,54,54,51,57,46,48,48,48,48,48,48,41,34,32,102,105,108,108,
61,34,35,70,70,70,70,70,70,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,105,99,111,110,115,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,53,
54,46,48,48,48,48,48,48,44,32,49,54,48,46,48,48,48,48,48,48,41,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,60,112,97,116,104,32,100,61,34,77,50,57,56,46,50,55,52,44,54,52,56,53,46,54,55,51,
32,76,50,57,56,46,50,55,49,44,54,52,56,53,46,54,54,57,32,67,50,57,55,46,56,57,54,44,54,52,56,53,46,50,54,54,32,50,57,55,46,50,54,53,44,54,52,56,53,46,50,52,52,32,50,57,54,46,56,54,51,44,54,52,56,53,46,
54,50,32,76,50,57,53,46,56,52,50,44,54,52,56,54,46,53,55,55,32,67,50,57,53,46,53,50,50,44,54,52,56,54,46,56,55,54,32,50,57,53,44,54,52,56,54,46,54,52,57,32,50,57,53,44,54,52,56,54,46,50,49,50,32,76,50,
57,53,44,54,52,56,48,32,67,50,57,53,44,54,52,55,57,46,52,52,55,32,50,57,52,46,53,53,50,44,54,52,55,57,32,50,57,52,44,54,52,55,57,32,67,50,57,51,46,52,52,56,44,54,52,55,57,32,50,57,51,44,54,52,55,57,46,
52,52,55,32,50,57,51,44,54,52,56,48,32,76,50,57,51,44,54,52,56,54,46,49,57,55,32,67,50,57,51,44,54,52,56,54,46,54,51,55,32,50,57,50,46,52,55,51,44,54,52,56,54,46,56,54,50,32,50,57,50,46,49,53,53,44,54,
52,56,54,46,53,53,56,32,76,50,57,49,46,49,56,49,44,54,52,56,53,46,54,51,32,67,50,57,48,46,55,56,50,44,54,52,56,53,46,50,52,56,32,50,57,48,46,49,52,56,44,54,52,56,53,46,50,54,54,32,50,56,57,46,55,55,49,
44,54,52,56,53,46,54,54,57,32,67,50,56,57,46,51,57,55,44,54,52,56,54,46,48,54,56,32,50,56,57,46,52,49,54,44,54,52,56,54,46,54,57,53,32,50,56,57,46,56,49,52,44,54,52,56,55,46,48,55,49,32,76,50,57,50,46,
54,54,50,44,54,52,56,57,46,55,53,57,32,67,50,57,51,46,52,51,54,44,54,52,57,48,46,52,57,32,50,57,52,46,54,52,54,44,54,52,57,48,46,52,56,55,32,50,57,53,46,52,49,53,44,54,52,56,57,46,55,53,51,32,76,50,57,
56,46,50,51,49,44,54,52,56,55,46,48,54,55,32,67,50,57,56,46,54,50,52,44,54,52,56,54,46,54,57,50,32,50,57,56,46,54,52,51,44,54,52,56,54,46,48,55,49,32,50,57,56,46,50,55,52,44,54,52,56,53,46,54,55,51,32,
76,50,57,56,46,50,55,52,44,54,52,56,53,46,54,55,51,32,90,32,77,51,48,52,44,54,52,57,55,46,50,50,50,32,67,51,48,52,44,54,52,57,56,46,50,32,51,48,51,46,51,50,44,54,52,57,57,32,51,48,49,46,56,56,56,44,54,
52,57,57,32,76,50,56,53,46,55,55,56,44,54,52,57,57,32,67,50,56,52,46,55,57,54,44,54,52,57,57,32,50,56,52,44,54,52,57,56,46,50,48,52,32,50,56,52,44,54,52,57,55,46,50,50,50,32,76,50,56,52,44,54,52,57,48,
46,55,55,55,32,67,50,56,52,44,54,52,56,57,46,56,52,55,32,50,56,52,46,55,48,55,44,54,52,56,57,32,50,56,53,46,56,56,56,44,54,52,56,57,32,76,50,56,55,46,48,48,55,44,54,52,56,57,32,67,50,56,55,46,53,53,53,
44,54,52,56,57,32,50,56,56,44,54,52,56,57,46,52,52,52,32,50,56,56,44,54,52,56,57,46,57,57,51,32,76,50,56,56,44,54,52,57,48,46,48,48,54,32,67,50,56,56,44,54,52,57,48,46,53,53,53,32,50,56,55,46,53,53,53,
44,54,52,57,49,32,50,56,55,46,48,48,55,44,54,52,57,49,32,76,50,56,54,46,57,57,51,44,54,52,57,49,32,67,50,56,54,46,52,52,53,44,54,52,57,49,32,50,56,54,44,54,52,57,49,46,52,52,52,32,50,56,54,44,54,52,57,
49,46,57,57,51,32,76,50,56,54,44,54,52,57,54,46,48,48,54,32,67,50,56,54,44,54,52,57,54,46,53,53,53,32,50,56,54,46,52,52,53,44,54,52,57,55,32,50,56,54,46,57,57,51,44,54,52,57,55,32,76,51,48,49,46,48,48,
55,44,54,52,57,55,32,67,51,48,49,46,53,53,53,44,54,52,57,55,32,51,48,50,44,54,52,57,54,46,53,53,53,32,51,48,50,44,54,52,57,54,46,48,48,54,32,76,51,48,50,44,54,52,57,49,46,57,57,51,32,67,51,48,50,44,54,
52,57,49,46,52,52,52,32,51,48,49,46,53,53,53,44,54,52,57,49,32,51,48,49,46,48,48,55,44,54,52,57,49,32,76,51,48,48,46,57,57,51,44,54,52,57,49,32,67,51,48,48,46,52,52,53,44,54,52,57,49,32,51,48,48,44,54,
52,57,48,46,53,53,53,32,51,48,48,44,54,52,57,48,46,48,48,54,32,76,51,48,48,44,54,52,56,57,46,57,57,51,32,67,51,48,48,44,54,52,56,57,46,52,52,52,32,51,48,48,46,52,52,53,44,54,52,56,57,32,51,48,48,46,57,
57,51,44,54,52,56,57,32,76,51,48,50,46,50,50,50,44,54,52,56,57,32,67,51,48,51,46,50,48,52,44,54,52,56,57,32,51,48,52,44,54,52,56,57,46,55,57,54,32,51,48,52,44,54,52,57,48,46,55,55,55,32,76,51,48,52,44,
54,52,57,55,46,50,50,50,32,90,34,32,105,100,61,34,97,114,114,111,119,95,105,110,95,100,111,119,110,45,91,35,51,52,53,93,34,62,60,47,112,97,116,104,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,47,103,62,
10,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,60,47,103,62,10,60,47,115,118,103,62,0,0};

const char* VASTWaveTableEditorComponent::paste_svg = (const char*) resource_VASTWaveTableEditorComponent_paste_svg;
const int VASTWaveTableEditorComponent::paste_svgSize = 1977;

// JUCER_RESOURCE: plus_svg, 1650, "../../Resources/Icons/plus.svg"
static const unsigned char resource_VASTWaveTableEditorComponent_plus_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,70,45,56,
34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,32,119,105,100,116,104,61,34,50,49,112,120,34,32,104,101,105,103,104,116,61,34,50,48,112,120,34,32,118,105,101,119,
66,111,120,61,34,48,32,48,32,50,49,32,50,48,34,32,118,101,114,115,105,111,110,61,34,49,46,49,34,32,120,109,108,110,115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,
47,115,118,103,34,32,120,109,108,110,115,58,120,108,105,110,107,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,120,108,105,110,107,34,62,10,32,32,32,32,60,33,45,
45,32,71,101,110,101,114,97,116,111,114,58,32,83,107,101,116,99,104,32,51,46,56,46,49,32,40,50,57,54,56,55,41,32,45,32,104,116,116,112,58,47,47,119,119,119,46,98,111,104,101,109,105,97,110,99,111,100,
105,110,103,46,99,111,109,47,115,107,101,116,99,104,32,45,45,62,10,32,32,32,32,60,116,105,116,108,101,62,110,101,119,95,116,97,98,32,91,35,49,53,48,50,93,60,47,116,105,116,108,101,62,10,32,32,32,32,60,
100,101,115,99,62,67,114,101,97,116,101,100,32,119,105,116,104,32,83,107,101,116,99,104,46,60,47,100,101,115,99,62,10,32,32,32,32,60,100,101,102,115,62,60,47,100,101,102,115,62,10,32,32,32,32,60,103,32,
105,100,61,34,80,97,103,101,45,49,34,32,115,116,114,111,107,101,61,34,110,111,110,101,34,32,115,116,114,111,107,101,45,119,105,100,116,104,61,34,49,34,32,102,105,108,108,61,34,110,111,110,101,34,32,102,
105,108,108,45,114,117,108,101,61,34,101,118,101,110,111,100,100,34,62,10,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,68,114,105,98,98,98,108,101,45,68,97,114,107,45,80,114,101,118,105,101,119,34,
32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,45,51,55,57,46,48,48,48,48,48,48,44,32,45,50,56,48,46,48,48,48,48,48,48,41,34,32,102,105,108,108,61,34,35,70,70,70,70,70,
70,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,105,99,111,110,115,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,53,54,46,48,48,48,48,48,48,
44,32,49,54,48,46,48,48,48,48,48,48,41,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,60,112,97,116,104,32,100,61,34,77,51,52,49,46,57,44,49,51,55,46,50,54,49,32,67,51,52,49,46,57,44,49,51,55,
46,56,49,49,32,51,52,49,46,52,57,55,56,53,44,49,51,56,32,51,52,48,46,57,50,48,51,53,44,49,51,56,32,76,51,51,49,46,52,55,48,51,53,44,49,51,56,32,67,51,51,48,46,56,57,50,56,53,44,49,51,56,32,51,51,48,46,
51,53,44,49,51,55,46,56,49,49,32,51,51,48,46,51,53,44,49,51,55,46,50,54,49,32,76,51,51,48,46,51,53,44,49,51,53,32,76,51,51,55,46,50,52,53,51,53,44,49,51,53,32,67,51,51,56,46,49,49,52,55,53,44,49,51,53,
32,51,51,56,46,55,53,44,49,51,52,46,53,56,57,32,51,51,56,46,55,53,44,49,51,51,46,55,54,49,32,76,51,51,56,46,55,53,44,49,50,55,32,76,51,52,48,46,57,50,48,51,53,44,49,50,55,32,67,51,52,49,46,52,57,57,57,
53,44,49,50,55,32,51,52,49,46,57,44,49,50,55,46,55,48,57,32,51,52,49,46,57,44,49,50,56,46,50,54,49,32,76,51,52,49,46,57,44,49,51,55,46,50,54,49,32,90,32,77,51,50,54,46,50,50,48,51,53,44,49,51,51,32,67,
51,50,53,46,54,52,50,56,53,44,49,51,51,32,51,50,53,46,49,44,49,51,50,46,56,49,49,32,51,50,53,46,49,44,49,51,50,46,50,54,49,32,76,51,50,53,46,49,44,49,50,51,46,50,54,49,32,67,51,50,53,46,49,44,49,50,50,
46,55,48,57,32,51,50,53,46,54,51,57,55,44,49,50,50,32,51,50,54,46,50,50,48,51,53,44,49,50,50,32,76,51,51,53,46,54,55,48,51,53,44,49,50,50,32,67,51,51,54,46,50,52,57,57,53,44,49,50,50,32,51,51,54,46,54,
53,44,49,50,50,46,55,48,57,32,51,51,54,46,54,53,44,49,50,51,46,50,54,49,32,76,51,51,54,46,54,53,44,49,51,50,46,50,54,49,32,67,51,51,54,46,54,53,44,49,51,50,46,56,49,49,32,51,51,54,46,50,52,55,56,53,44,
49,51,51,32,51,51,53,46,54,55,48,51,53,44,49,51,51,32,76,51,50,54,46,50,50,48,51,53,44,49,51,51,32,90,32,77,51,52,50,46,52,57,53,51,53,44,49,50,53,32,76,51,51,56,46,55,53,44,49,50,53,32,76,51,51,56,46,
55,53,44,49,50,49,46,55,54,49,32,67,51,51,56,46,55,53,44,49,50,48,46,57,51,51,32,51,51,56,46,49,49,52,55,53,44,49,50,48,32,51,51,55,46,50,52,53,51,53,44,49,50,48,32,76,51,50,52,46,54,52,53,51,53,44,49,
50,48,32,67,51,50,51,46,55,55,57,49,44,49,50,48,32,51,50,51,44,49,50,48,46,57,51,54,32,51,50,51,44,49,50,49,46,55,54,49,32,76,51,50,51,44,49,51,51,46,55,54,49,32,67,51,50,51,44,49,51,52,46,53,56,54,32,
51,50,51,46,55,55,57,49,44,49,51,53,32,51,50,52,46,54,52,53,51,53,44,49,51,53,32,76,51,50,56,46,50,53,44,49,51,53,32,76,51,50,56,46,50,53,44,49,51,56,46,55,54,49,32,67,51,50,56,46,50,53,44,49,51,57,46,
53,56,54,32,51,50,57,46,48,50,57,49,44,49,52,48,32,51,50,57,46,56,57,53,51,53,44,49,52,48,32,76,51,52,50,46,52,57,53,51,53,44,49,52,48,32,67,51,52,51,46,51,54,52,55,53,44,49,52,48,32,51,52,52,44,49,51,
57,46,53,56,57,32,51,52,52,44,49,51,56,46,55,54,49,32,76,51,52,52,44,49,50,54,46,55,54,49,32,67,51,52,52,44,49,50,53,46,57,51,51,32,51,52,51,46,51,54,52,55,53,44,49,50,53,32,51,52,50,46,52,57,53,51,53,
44,49,50,53,32,76,51,52,50,46,52,57,53,51,53,44,49,50,53,32,90,34,32,105,100,61,34,110,101,119,95,116,97,98,45,91,35,49,53,48,50,93,34,62,60,47,112,97,116,104,62,10,32,32,32,32,32,32,32,32,32,32,32,32,
60,47,103,62,10,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,60,47,103,62,10,60,47,115,118,103,62,0,0};

const char* VASTWaveTableEditorComponent::plus_svg = (const char*) resource_VASTWaveTableEditorComponent_plus_svg;
const int VASTWaveTableEditorComponent::plus_svgSize = 1650;

// JUCER_RESOURCE: save_svg, 1353, "../../Resources/Icons/save.svg"
static const unsigned char resource_VASTWaveTableEditorComponent_save_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,70,45,56,
34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,32,119,105,100,116,104,61,34,50,49,112,120,34,32,104,101,105,103,104,116,61,34,50,48,112,120,34,32,118,105,101,119,
66,111,120,61,34,48,32,48,32,50,49,32,50,48,34,32,118,101,114,115,105,111,110,61,34,49,46,49,34,32,120,109,108,110,115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,
47,115,118,103,34,32,120,109,108,110,115,58,120,108,105,110,107,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,120,108,105,110,107,34,62,10,32,32,32,32,60,33,45,
45,32,71,101,110,101,114,97,116,111,114,58,32,83,107,101,116,99,104,32,51,46,56,46,49,32,40,50,57,54,56,55,41,32,45,32,104,116,116,112,58,47,47,119,119,119,46,98,111,104,101,109,105,97,110,99,111,100,
105,110,103,46,99,111,109,47,115,107,101,116,99,104,32,45,45,62,10,32,32,32,32,60,116,105,116,108,101,62,115,97,118,101,95,105,116,101,109,32,91,35,49,52,48,56,93,60,47,116,105,116,108,101,62,10,32,32,
32,32,60,100,101,115,99,62,67,114,101,97,116,101,100,32,119,105,116,104,32,83,107,101,116,99,104,46,60,47,100,101,115,99,62,10,32,32,32,32,60,100,101,102,115,62,60,47,100,101,102,115,62,10,32,32,32,32,
60,103,32,105,100,61,34,80,97,103,101,45,49,34,32,115,116,114,111,107,101,61,34,110,111,110,101,34,32,115,116,114,111,107,101,45,119,105,100,116,104,61,34,49,34,32,102,105,108,108,61,34,110,111,110,101,
34,32,102,105,108,108,45,114,117,108,101,61,34,101,118,101,110,111,100,100,34,62,10,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,68,114,105,98,98,98,108,101,45,68,97,114,107,45,80,114,101,118,105,101,
119,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,45,49,51,57,46,48,48,48,48,48,48,44,32,45,54,56,48,46,48,48,48,48,48,48,41,34,32,102,105,108,108,61,34,35,70,70,
70,70,70,70,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,105,99,111,110,115,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,53,54,46,48,48,48,
48,48,48,44,32,49,54,48,46,48,48,48,48,48,48,41,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,60,112,97,116,104,32,100,61,34,77,57,48,46,50,49,56,55,53,44,53,50,53,32,76,57,50,46,51,49,56,55,
53,44,53,50,53,32,76,57,50,46,51,49,56,55,53,44,53,50,51,32,76,57,48,46,50,49,56,55,53,44,53,50,51,32,76,57,48,46,50,49,56,55,53,44,53,50,53,32,90,32,77,56,55,46,50,44,53,51,54,32,76,57,57,46,56,44,53,
51,54,32,76,57,57,46,56,44,53,51,52,32,76,56,55,46,50,44,53,51,52,32,76,56,55,46,50,44,53,51,54,32,90,32,77,56,55,46,50,44,53,51,50,32,76,57,57,46,56,44,53,51,50,32,76,57,57,46,56,44,53,51,48,32,76,56,
55,46,50,44,53,51,48,32,76,56,55,46,50,44,53,51,50,32,90,32,77,49,48,49,46,57,44,53,51,56,32,76,56,53,46,49,44,53,51,56,32,76,56,53,46,49,44,53,50,54,46,56,51,55,32,76,56,55,46,50,44,53,50,52,46,56,51,
55,32,76,56,55,46,50,44,53,50,56,32,76,56,56,46,49,49,56,55,53,44,53,50,56,32,76,56,57,46,51,44,53,50,56,32,76,57,55,46,55,44,53,50,56,32,76,57,57,46,52,55,49,51,53,44,53,50,56,32,76,57,57,46,56,44,53,
50,56,32,76,57,57,46,56,44,53,50,50,32,76,49,48,49,46,57,44,53,50,50,32,76,49,48,49,46,57,44,53,51,56,32,90,32,77,56,57,46,51,44,53,50,50,46,56,51,55,32,76,57,48,46,49,55,56,56,53,44,53,50,50,32,76,57,
55,46,55,44,53,50,50,32,76,57,55,46,55,44,53,50,54,32,76,56,57,46,51,44,53,50,54,32,76,56,57,46,51,44,53,50,50,46,56,51,55,32,90,32,77,49,48,51,46,57,54,54,52,44,53,50,48,32,76,49,48,49,46,56,54,54,52,
44,53,50,48,32,76,56,57,46,51,44,53,50,48,32,76,56,57,46,51,44,53,50,48,46,48,48,56,32,76,56,55,46,50,48,56,52,44,53,50,50,32,76,56,55,46,50,44,53,50,50,32,76,56,55,46,50,44,53,50,50,46,48,48,56,32,76,
56,51,46,48,48,56,52,44,53,50,54,32,76,56,51,44,53,50,54,32,76,56,51,44,53,51,56,32,76,56,51,44,53,52,48,32,76,56,53,46,49,44,53,52,48,32,76,49,48,49,46,56,54,54,52,44,53,52,48,32,76,49,48,51,46,57,54,
54,52,44,53,52,48,32,76,49,48,52,44,53,52,48,32,76,49,48,52,44,53,51,56,32,76,49,48,52,44,53,50,50,32,76,49,48,52,44,53,50,48,32,76,49,48,51,46,57,54,54,52,44,53,50,48,32,90,34,32,105,100,61,34,115,97,
118,101,95,105,116,101,109,45,91,35,49,52,48,56,93,34,62,60,47,112,97,116,104,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,60,47,103,62,
10,60,47,115,118,103,62,0,0};

const char* VASTWaveTableEditorComponent::save_svg = (const char*) resource_VASTWaveTableEditorComponent_save_svg;
const int VASTWaveTableEditorComponent::save_svgSize = 1353;

// JUCER_RESOURCE: select_all_svg, 1470, "../../Resources/Icons/select_all.svg"
static const unsigned char resource_VASTWaveTableEditorComponent_select_all_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,70,
45,56,34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,32,119,105,100,116,104,61,34,50,48,112,120,34,32,104,101,105,103,104,116,61,34,50,48,112,120,34,32,118,105,101,
119,66,111,120,61,34,48,32,48,32,50,48,32,50,48,34,32,118,101,114,115,105,111,110,61,34,49,46,49,34,32,120,109,108,110,115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,
48,48,47,115,118,103,34,32,120,109,108,110,115,58,120,108,105,110,107,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,120,108,105,110,107,34,62,10,32,32,32,32,60,
33,45,45,32,71,101,110,101,114,97,116,111,114,58,32,83,107,101,116,99,104,32,51,46,56,46,49,32,40,50,57,54,56,55,41,32,45,32,104,116,116,112,58,47,47,119,119,119,46,98,111,104,101,109,105,97,110,99,111,
100,105,110,103,46,99,111,109,47,115,107,101,116,99,104,32,45,45,62,10,32,32,32,32,60,116,105,116,108,101,62,97,114,114,111,119,95,97,108,108,32,91,35,51,50,56,93,60,47,116,105,116,108,101,62,10,32,32,
32,32,60,100,101,115,99,62,67,114,101,97,116,101,100,32,119,105,116,104,32,83,107,101,116,99,104,46,60,47,100,101,115,99,62,10,32,32,32,32,60,100,101,102,115,62,60,47,100,101,102,115,62,10,32,32,32,32,
60,103,32,105,100,61,34,80,97,103,101,45,49,34,32,115,116,114,111,107,101,61,34,110,111,110,101,34,32,115,116,114,111,107,101,45,119,105,100,116,104,61,34,49,34,32,102,105,108,108,61,34,110,111,110,101,
34,32,102,105,108,108,45,114,117,108,101,61,34,101,118,101,110,111,100,100,34,62,10,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,68,114,105,98,98,98,108,101,45,68,97,114,107,45,80,114,101,118,105,101,
119,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,45,50,50,48,46,48,48,48,48,48,48,44,32,45,54,55,49,57,46,48,48,48,48,48,48,41,34,32,102,105,108,108,61,34,35,70,
70,70,70,70,70,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,105,99,111,110,115,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,53,54,46,48,48,
48,48,48,48,44,32,49,54,48,46,48,48,48,48,48,48,41,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,60,112,97,116,104,32,100,61,34,77,49,55,50,44,54,53,55,49,32,76,49,55,54,44,54,53,55,49,32,76,
49,55,54,44,54,53,54,55,32,76,49,55,50,44,54,53,54,55,32,76,49,55,50,44,54,53,55,49,32,90,32,77,49,56,52,44,54,53,54,56,46,57,57,32,76,49,56,48,46,52,54,53,44,54,53,54,53,46,52,53,53,32,76,49,55,57,46,
48,53,44,54,53,54,54,46,56,54,57,32,76,49,56,48,46,49,56,49,44,54,53,54,56,32,76,49,55,56,44,54,53,54,56,32,76,49,55,56,44,54,53,54,53,32,76,49,55,53,44,54,53,54,53,32,76,49,55,53,44,54,53,54,50,46,56,
49,57,32,76,49,55,54,46,49,51,49,44,54,53,54,51,46,57,53,32,76,49,55,55,46,53,52,53,44,54,53,54,50,46,53,51,53,32,76,49,55,52,46,48,49,44,54,53,53,57,32,76,49,55,52,44,54,53,53,57,46,48,49,32,76,49,55,
51,46,57,57,44,54,53,53,57,32,76,49,55,48,46,52,53,53,44,54,53,54,50,46,53,51,53,32,76,49,55,49,46,56,54,57,44,54,53,54,51,46,57,53,32,76,49,55,51,44,54,53,54,50,46,56,49,57,32,76,49,55,51,44,54,53,54,
53,32,76,49,55,48,44,54,53,54,53,32,76,49,55,48,44,54,53,54,56,32,76,49,54,55,46,56,49,57,44,54,53,54,56,32,76,49,54,56,46,57,53,44,54,53,54,54,46,56,54,57,32,76,49,54,55,46,53,51,54,44,54,53,54,53,46,
52,53,53,32,76,49,54,52,44,54,53,54,56,46,57,57,32,76,49,54,52,46,48,49,44,54,53,54,57,32,76,49,54,52,44,54,53,54,57,46,48,49,32,76,49,54,55,46,53,51,54,44,54,53,55,50,46,53,52,53,32,76,49,54,56,46,57,
53,44,54,53,55,49,46,49,51,49,32,76,49,54,55,46,56,49,57,44,54,53,55,48,32,76,49,55,48,44,54,53,55,48,32,76,49,55,48,44,54,53,55,51,32,76,49,55,51,44,54,53,55,51,32,76,49,55,51,44,54,53,55,53,46,49,56,
49,32,76,49,55,49,46,56,54,57,44,54,53,55,52,46,48,53,32,76,49,55,48,46,52,53,53,44,54,53,55,53,46,52,54,52,32,76,49,55,51,46,57,57,44,54,53,55,57,32,76,49,55,52,44,54,53,55,56,46,57,57,32,76,49,55,52,
46,48,49,44,54,53,55,57,32,76,49,55,55,46,53,52,53,44,54,53,55,53,46,52,54,52,32,76,49,55,54,46,49,51,49,44,54,53,55,52,46,48,53,32,76,49,55,53,44,54,53,55,53,46,49,56,49,32,76,49,55,53,44,54,53,55,51,
32,76,49,55,56,44,54,53,55,51,32,76,49,55,56,44,54,53,55,48,32,76,49,56,48,46,49,56,49,44,54,53,55,48,32,76,49,55,57,46,48,53,44,54,53,55,49,46,49,51,49,32,76,49,56,48,46,52,54,53,44,54,53,55,50,46,53,
52,53,32,76,49,56,52,44,54,53,54,57,46,48,49,32,76,49,56,51,46,57,57,44,54,53,54,57,32,76,49,56,52,44,54,53,54,56,46,57,57,32,90,34,32,105,100,61,34,97,114,114,111,119,95,97,108,108,45,91,35,51,50,56,
93,34,62,60,47,112,97,116,104,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,60,47,103,62,10,60,47,115,118,103,62,0,0};

const char* VASTWaveTableEditorComponent::select_all_svg = (const char*) resource_VASTWaveTableEditorComponent_select_all_svg;
const int VASTWaveTableEditorComponent::select_all_svgSize = 1470;

// JUCER_RESOURCE: zoom_in_svg, 1528, "../../Resources/Icons/zoom_in.svg"
static const unsigned char resource_VASTWaveTableEditorComponent_zoom_in_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,70,45,
56,34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,32,119,105,100,116,104,61,34,50,49,112,120,34,32,104,101,105,103,104,116,61,34,50,48,112,120,34,32,118,105,101,119,
66,111,120,61,34,48,32,48,32,50,49,32,50,48,34,32,118,101,114,115,105,111,110,61,34,49,46,49,34,32,120,109,108,110,115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,
47,115,118,103,34,32,120,109,108,110,115,58,120,108,105,110,107,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,120,108,105,110,107,34,62,10,32,32,32,32,60,33,45,
45,32,71,101,110,101,114,97,116,111,114,58,32,83,107,101,116,99,104,32,51,46,56,46,49,32,40,50,57,54,56,55,41,32,45,32,104,116,116,112,58,47,47,119,119,119,46,98,111,104,101,109,105,97,110,99,111,100,
105,110,103,46,99,111,109,47,115,107,101,116,99,104,32,45,45,62,10,32,32,32,32,60,116,105,116,108,101,62,122,111,111,109,95,105,110,32,91,35,49,52,54,49,93,60,47,116,105,116,108,101,62,10,32,32,32,32,
60,100,101,115,99,62,67,114,101,97,116,101,100,32,119,105,116,104,32,83,107,101,116,99,104,46,60,47,100,101,115,99,62,10,32,32,32,32,60,100,101,102,115,62,60,47,100,101,102,115,62,10,32,32,32,32,60,103,
32,105,100,61,34,80,97,103,101,45,49,34,32,115,116,114,111,107,101,61,34,110,111,110,101,34,32,115,116,114,111,107,101,45,119,105,100,116,104,61,34,49,34,32,102,105,108,108,61,34,110,111,110,101,34,32,
102,105,108,108,45,114,117,108,101,61,34,101,118,101,110,111,100,100,34,62,10,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,68,114,105,98,98,98,108,101,45,68,97,114,107,45,80,114,101,118,105,101,119,
34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,45,52,49,57,46,48,48,48,48,48,48,44,32,45,52,52,48,46,48,48,48,48,48,48,41,34,32,102,105,108,108,61,34,35,70,70,70,70,
70,70,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,105,99,111,110,115,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,53,54,46,48,48,48,48,48,
48,44,32,49,54,48,46,48,48,48,48,48,48,41,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,60,112,97,116,104,32,100,61,34,77,51,55,50,46,52,53,44,50,56,54,46,57,54,55,52,49,57,32,76,51,55,53,46,
54,44,50,56,54,46,57,54,55,52,49,57,32,76,51,55,53,46,54,44,50,56,56,46,57,55,50,52,51,49,32,76,51,55,50,46,52,53,44,50,56,56,46,57,55,50,52,51,49,32,76,51,55,50,46,52,53,44,50,57,49,46,57,55,57,57,53,
32,76,51,55,48,46,51,53,44,50,57,49,46,57,55,57,57,53,32,76,51,55,48,46,51,53,44,50,56,56,46,57,55,50,52,51,49,32,76,51,54,55,46,50,44,50,56,56,46,57,55,50,52,51,49,32,76,51,54,55,46,50,44,50,56,54,46,
57,54,55,52,49,57,32,76,51,55,48,46,51,53,44,50,56,54,46,57,54,55,52,49,57,32,76,51,55,48,46,51,53,44,50,56,51,46,57,53,57,57,32,76,51,55,50,46,52,53,44,50,56,51,46,57,53,57,57,32,76,51,55,50,46,52,53,
44,50,56,54,46,57,54,55,52,49,57,32,90,32,77,51,56,52,44,50,57,56,46,53,56,50,52,53,54,32,76,51,56,50,46,53,49,53,51,44,51,48,48,32,76,51,55,56,46,48,54,48,49,53,44,50,57,53,46,55,52,54,51,54,54,32,76,
51,55,57,46,53,52,52,56,53,44,50,57,52,46,51,50,56,56,50,50,32,76,51,56,52,44,50,57,56,46,53,56,50,52,53,54,32,90,32,77,51,55,49,46,52,44,50,57,52,46,48,51,53,48,56,56,32,67,51,54,55,46,57,50,54,54,44,
50,57,52,46,48,51,53,48,56,56,32,51,54,53,46,49,44,50,57,49,46,51,51,55,51,52,51,32,51,54,53,46,49,44,50,56,56,46,48,50,48,48,53,32,67,51,54,53,46,49,44,50,56,52,46,55,48,51,55,53,57,32,51,54,55,46,57,
50,54,54,44,50,56,50,46,48,48,53,48,49,51,32,51,55,49,46,52,44,50,56,50,46,48,48,53,48,49,51,32,67,51,55,52,46,56,55,51,52,44,50,56,50,46,48,48,53,48,49,51,32,51,55,55,46,55,44,50,56,52,46,55,48,51,55,
53,57,32,51,55,55,46,55,44,50,56,56,46,48,50,48,48,53,32,67,51,55,55,46,55,44,50,57,49,46,51,51,55,51,52,51,32,51,55,52,46,56,55,51,52,44,50,57,52,46,48,51,53,48,56,56,32,51,55,49,46,52,44,50,57,52,46,
48,51,53,48,56,56,32,76,51,55,49,46,52,44,50,57,52,46,48,51,53,48,56,56,32,90,32,77,51,55,49,46,52,44,50,56,48,32,67,51,54,54,46,55,54,49,49,44,50,56,48,32,51,54,51,44,50,56,51,46,53,57,48,57,55,55,32,
51,54,51,44,50,56,56,46,48,50,48,48,53,32,67,51,54,51,44,50,57,50,46,52,53,48,49,50,53,32,51,54,54,46,55,54,49,49,44,50,57,54,46,48,52,48,49,32,51,55,49,46,52,44,50,57,54,46,48,52,48,49,32,67,51,55,54,
46,48,51,56,57,44,50,57,54,46,48,52,48,49,32,51,55,57,46,56,44,50,57,50,46,52,53,48,49,50,53,32,51,55,57,46,56,44,50,56,56,46,48,50,48,48,53,32,67,51,55,57,46,56,44,50,56,51,46,53,57,48,57,55,55,32,51,
55,54,46,48,51,56,57,44,50,56,48,32,51,55,49,46,52,44,50,56,48,32,76,51,55,49,46,52,44,50,56,48,32,90,34,32,105,100,61,34,122,111,111,109,95,105,110,45,91,35,49,52,54,49,93,34,62,60,47,112,97,116,104,
62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,60,47,103,62,10,60,47,115,118,103,62,0,0};

const char* VASTWaveTableEditorComponent::zoom_in_svg = (const char*) resource_VASTWaveTableEditorComponent_zoom_in_svg;
const int VASTWaveTableEditorComponent::zoom_in_svgSize = 1528;

// JUCER_RESOURCE: zoom_out_svg, 1506, "../../Resources/Icons/zoom_out.svg"
static const unsigned char resource_VASTWaveTableEditorComponent_zoom_out_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,70,
45,56,34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,32,119,105,100,116,104,61,34,50,49,112,120,34,32,104,101,105,103,104,116,61,34,50,48,112,120,34,32,118,105,101,
119,66,111,120,61,34,48,32,48,32,50,49,32,50,48,34,32,118,101,114,115,105,111,110,61,34,49,46,49,34,32,120,109,108,110,115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,
48,48,47,115,118,103,34,32,120,109,108,110,115,58,120,108,105,110,107,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,120,108,105,110,107,34,62,10,32,32,32,32,60,
33,45,45,32,71,101,110,101,114,97,116,111,114,58,32,83,107,101,116,99,104,32,51,46,56,46,49,32,40,50,57,54,56,55,41,32,45,32,104,116,116,112,58,47,47,119,119,119,46,98,111,104,101,109,105,97,110,99,111,
100,105,110,103,46,99,111,109,47,115,107,101,116,99,104,32,45,45,62,10,32,32,32,32,60,116,105,116,108,101,62,122,111,111,109,95,111,117,116,32,91,35,49,52,53,57,93,60,47,116,105,116,108,101,62,10,32,32,
32,32,60,100,101,115,99,62,67,114,101,97,116,101,100,32,119,105,116,104,32,83,107,101,116,99,104,46,60,47,100,101,115,99,62,10,32,32,32,32,60,100,101,102,115,62,60,47,100,101,102,115,62,10,32,32,32,32,
60,103,32,105,100,61,34,80,97,103,101,45,49,34,32,115,116,114,111,107,101,61,34,110,111,110,101,34,32,115,116,114,111,107,101,45,119,105,100,116,104,61,34,49,34,32,102,105,108,108,61,34,110,111,110,101,
34,32,102,105,108,108,45,114,117,108,101,61,34,101,118,101,110,111,100,100,34,62,10,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,68,114,105,98,98,98,108,101,45,68,97,114,107,45,80,114,101,118,105,101,
119,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,45,57,57,46,48,48,48,48,48,48,44,32,45,52,56,48,46,48,48,48,48,48,48,41,34,32,102,105,108,108,61,34,35,70,70,70,
70,70,70,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,105,99,111,110,115,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,53,54,46,48,48,48,48,
48,48,44,32,49,54,48,46,48,48,48,48,48,48,41,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,60,112,97,116,104,32,100,61,34,77,52,55,46,51,50,53,52,51,55,55,44,51,50,56,46,57,55,50,52,51,49,32,
76,53,53,46,57,55,54,51,49,51,49,44,51,50,56,46,57,55,50,52,51,49,32,76,53,53,46,57,55,54,51,49,51,49,44,51,50,54,46,57,54,55,52,49,57,32,76,52,55,46,51,50,53,52,51,55,55,44,51,50,54,46,57,54,55,52,49,
57,32,76,52,55,46,51,50,53,52,51,55,55,44,51,50,56,46,57,55,50,52,51,49,32,90,32,77,54,52,44,51,51,56,46,53,56,50,52,53,54,32,76,54,50,46,52,54,57,56,55,54,52,44,51,52,48,32,76,53,55,46,56,56,50,55,52,
57,55,44,51,51,53,46,55,52,54,51,54,54,32,76,53,57,46,52,49,49,55,57,50,44,51,51,52,46,51,50,56,56,50,50,32,76,54,52,44,51,51,56,46,53,56,50,52,53,54,32,90,32,77,53,49,46,54,53,48,56,55,53,52,44,51,51,
52,46,48,51,53,48,56,56,32,67,52,56,46,48,55,50,54,53,55,49,44,51,51,52,46,48,51,53,48,56,56,32,52,53,46,49,54,50,55,49,56,56,44,51,51,49,46,51,51,55,51,52,51,32,52,53,46,49,54,50,55,49,56,56,44,51,50,
56,46,48,50,48,48,53,32,67,52,53,46,49,54,50,55,49,56,56,44,51,50,52,46,55,48,51,55,53,57,32,52,56,46,48,55,50,54,53,55,49,44,51,50,50,46,48,48,53,48,49,51,32,53,49,46,54,53,48,56,55,53,52,44,51,50,50,
46,48,48,53,48,49,51,32,67,53,53,46,50,50,56,48,49,50,52,44,51,50,50,46,48,48,53,48,49,51,32,53,56,46,49,51,57,48,51,49,57,44,51,50,52,46,55,48,51,55,53,57,32,53,56,46,49,51,57,48,51,49,57,44,51,50,56,
46,48,50,48,48,53,32,67,53,56,46,49,51,57,48,51,49,57,44,51,51,49,46,51,51,55,51,52,51,32,53,53,46,50,50,56,48,49,50,52,44,51,51,52,46,48,51,53,48,56,56,32,53,49,46,54,53,48,56,55,53,52,44,51,51,52,46,
48,51,53,48,56,56,32,76,53,49,46,54,53,48,56,55,53,52,44,51,51,52,46,48,51,53,48,56,56,32,90,32,77,53,49,46,54,53,48,56,55,53,52,44,51,50,48,32,67,52,54,46,56,55,50,51,52,56,49,44,51,50,48,32,52,51,44,
51,50,51,46,53,57,48,57,55,55,32,52,51,44,51,50,56,46,48,50,48,48,53,32,67,52,51,44,51,51,50,46,52,52,57,49,50,51,32,52,54,46,56,55,50,51,52,56,49,44,51,51,54,46,48,52,48,49,32,53,49,46,54,53,48,56,55,
53,52,44,51,51,54,46,48,52,48,49,32,67,53,54,46,52,50,56,51,50,49,51,44,51,51,54,46,48,52,48,49,32,54,48,46,51,48,49,55,53,48,56,44,51,51,50,46,52,52,57,49,50,51,32,54,48,46,51,48,49,55,53,48,56,44,51,
50,56,46,48,50,48,48,53,32,67,54,48,46,51,48,49,55,53,48,56,44,51,50,51,46,53,57,48,57,55,55,32,53,54,46,52,50,56,51,50,49,51,44,51,50,48,32,53,49,46,54,53,48,56,55,53,52,44,51,50,48,32,76,53,49,46,54,
53,48,56,55,53,52,44,51,50,48,32,90,34,32,105,100,61,34,122,111,111,109,95,111,117,116,45,91,35,49,52,53,57,93,34,62,60,47,112,97,116,104,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,
32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,60,47,103,62,10,60,47,115,118,103,62,0,0};

const char* VASTWaveTableEditorComponent::zoom_out_svg = (const char*) resource_VASTWaveTableEditorComponent_zoom_out_svg;
const int VASTWaveTableEditorComponent::zoom_out_svgSize = 1506;

// JUCER_RESOURCE: dynamics_svg, 13088, "../../Resources/dynamics.svg"
static const unsigned char resource_VASTWaveTableEditorComponent_dynamics_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,70,
45,56,34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,33,45,45,32,67,114,101,97,116,101,100,32,119,105,116,104,32,73,110,107,115,99,97,112,101,32,40,104,116,116,112,58,47,47,119,
119,119,46,105,110,107,115,99,97,112,101,46,111,114,103,47,41,32,45,45,62,10,10,60,115,118,103,10,32,32,32,120,109,108,110,115,58,100,99,61,34,104,116,116,112,58,47,47,112,117,114,108,46,111,114,103,47,
100,99,47,101,108,101,109,101,110,116,115,47,49,46,49,47,34,10,32,32,32,120,109,108,110,115,58,99,99,61,34,104,116,116,112,58,47,47,99,114,101,97,116,105,118,101,99,111,109,109,111,110,115,46,111,114,
103,47,110,115,35,34,10,32,32,32,120,109,108,110,115,58,114,100,102,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,48,50,47,50,50,45,114,100,102,45,115,121,110,116,
97,120,45,110,115,35,34,10,32,32,32,120,109,108,110,115,58,115,118,103,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,118,103,34,10,32,32,32,120,109,108,110,
115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,118,103,34,10,32,32,32,120,109,108,110,115,58,115,111,100,105,112,111,100,105,61,34,104,116,116,112,58,47,
47,115,111,100,105,112,111,100,105,46,115,111,117,114,99,101,102,111,114,103,101,46,110,101,116,47,68,84,68,47,115,111,100,105,112,111,100,105,45,48,46,100,116,100,34,10,32,32,32,120,109,108,110,115,58,
105,110,107,115,99,97,112,101,61,34,104,116,116,112,58,47,47,119,119,119,46,105,110,107,115,99,97,112,101,46,111,114,103,47,110,97,109,101,115,112,97,99,101,115,47,105,110,107,115,99,97,112,101,34,10,
32,32,32,119,105,100,116,104,61,34,50,49,48,109,109,34,10,32,32,32,104,101,105,103,104,116,61,34,50,57,55,109,109,34,10,32,32,32,105,100,61,34,115,118,103,51,48,50,52,34,10,32,32,32,118,101,114,115,105,
111,110,61,34,49,46,49,34,10,32,32,32,105,110,107,115,99,97,112,101,58,118,101,114,115,105,111,110,61,34,48,46,57,50,46,51,32,40,50,52,48,53,53,52,54,44,32,50,48,49,56,45,48,51,45,49,49,41,34,10,32,32,
32,115,111,100,105,112,111,100,105,58,100,111,99,110,97,109,101,61,34,100,121,110,97,109,105,99,115,46,115,118,103,34,62,10,32,32,60,100,101,102,115,10,32,32,32,32,32,105,100,61,34,100,101,102,115,51,
48,50,54,34,32,47,62,10,32,32,60,115,111,100,105,112,111,100,105,58,110,97,109,101,100,118,105,101,119,10,32,32,32,32,32,105,100,61,34,98,97,115,101,34,10,32,32,32,32,32,112,97,103,101,99,111,108,111,
114,61,34,35,102,102,102,102,102,102,34,10,32,32,32,32,32,98,111,114,100,101,114,99,111,108,111,114,61,34,35,54,54,54,54,54,54,34,10,32,32,32,32,32,98,111,114,100,101,114,111,112,97,99,105,116,121,61,
34,49,46,48,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,112,97,103,101,111,112,97,99,105,116,121,61,34,48,46,48,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,112,97,103,101,115,104,97,
100,111,119,61,34,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,122,111,111,109,61,34,55,46,57,49,57,53,57,53,57,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,120,61,34,50,53,55,
46,57,48,48,49,57,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,121,61,34,53,53,56,46,54,53,52,48,56,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,100,111,99,117,109,101,110,116,45,
117,110,105,116,115,61,34,112,120,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,117,114,114,101,110,116,45,108,97,121,101,114,61,34,108,97,121,101,114,49,34,10,32,32,32,32,32,115,104,111,119,
103,114,105,100,61,34,102,97,108,115,101,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,119,105,100,116,104,61,34,53,52,51,49,34,10,32,32,32,32,32,105,110,107,115,99,
97,112,101,58,119,105,110,100,111,119,45,104,101,105,103,104,116,61,34,50,56,54,56,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,120,61,34,45,49,56,34,10,32,32,32,32,
32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,121,61,34,45,49,56,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,109,97,120,105,109,105,122,101,100,61,
34,49,34,32,47,62,10,32,32,60,109,101,116,97,100,97,116,97,10,32,32,32,32,32,105,100,61,34,109,101,116,97,100,97,116,97,51,48,50,57,34,62,10,32,32,32,32,60,114,100,102,58,82,68,70,62,10,32,32,32,32,32,
32,60,99,99,58,87,111,114,107,10,32,32,32,32,32,32,32,32,32,114,100,102,58,97,98,111,117,116,61,34,34,62,10,32,32,32,32,32,32,32,32,60,100,99,58,102,111,114,109,97,116,62,105,109,97,103,101,47,115,118,
103,43,120,109,108,60,47,100,99,58,102,111,114,109,97,116,62,10,32,32,32,32,32,32,32,32,60,100,99,58,116,121,112,101,10,32,32,32,32,32,32,32,32,32,32,32,114,100,102,58,114,101,115,111,117,114,99,101,61,
34,104,116,116,112,58,47,47,112,117,114,108,46,111,114,103,47,100,99,47,100,99,109,105,116,121,112,101,47,83,116,105,108,108,73,109,97,103,101,34,32,47,62,10,32,32,32,32,32,32,32,32,60,100,99,58,116,105,
116,108,101,62,60,47,100,99,58,116,105,116,108,101,62,10,32,32,32,32,32,32,60,47,99,99,58,87,111,114,107,62,10,32,32,32,32,60,47,114,100,102,58,82,68,70,62,10,32,32,60,47,109,101,116,97,100,97,116,97,
62,10,32,32,60,103,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,108,97,98,101,108,61,34,69,98,101,110,101,32,49,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,103,114,111,117,112,109,111,
100,101,61,34,108,97,121,101,114,34,10,32,32,32,32,32,105,100,61,34,108,97,121,101,114,49,34,62,10,32,32,32,32,60,103,10,32,32,32,32,32,32,32,97,114,105,97,45,108,97,98,101,108,61,34,100,121,110,97,109,
105,99,115,34,10,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,111,110,116,45,115,116,121,108,101,58,111,98,108,105,113,117,101,59,102,111,110,116,45,118,97,114,105,97,110,116,58,110,111,114,109,
97,108,59,102,111,110,116,45,119,101,105,103,104,116,58,110,111,114,109,97,108,59,102,111,110,116,45,115,116,114,101,116,99,104,58,110,111,114,109,97,108,59,108,105,110,101,45,104,101,105,103,104,116,
58,48,37,59,102,111,110,116,45,102,97,109,105,108,121,58,39,80,97,108,97,99,101,32,83,99,114,105,112,116,32,77,84,39,59,45,105,110,107,115,99,97,112,101,45,102,111,110,116,45,115,112,101,99,105,102,105,
99,97,116,105,111,110,58,39,80,97,108,97,99,101,32,83,99,114,105,112,116,32,77,84,32,79,98,108,105,113,117,101,39,59,116,101,120,116,45,97,108,105,103,110,58,115,116,97,114,116,59,108,101,116,116,101,
114,45,115,112,97,99,105,110,103,58,48,112,120,59,119,111,114,100,45,115,112,97,99,105,110,103,58,48,112,120,59,119,114,105,116,105,110,103,45,109,111,100,101,58,108,114,45,116,98,59,116,101,120,116,45,
97,110,99,104,111,114,58,115,116,97,114,116,59,102,105,108,108,58,35,102,57,102,57,102,57,59,102,105,108,108,45,111,112,97,99,105,116,121,58,49,59,115,116,114,111,107,101,58,110,111,110,101,34,10,32,32,
32,32,32,32,32,105,100,61,34,116,101,120,116,51,48,51,50,34,62,10,32,32,32,32,32,32,60,112,97,116,104,10,32,32,32,32,32,32,32,32,32,100,61,34,109,32,50,50,52,46,56,56,48,50,56,44,53,51,56,46,53,56,57,
56,52,32,113,32,45,49,46,54,48,49,53,54,44,48,32,45,52,46,53,55,48,51,49,44,50,46,50,54,53,54,51,32,45,50,46,57,54,56,55,53,44,50,46,50,50,54,53,54,32,45,53,46,50,55,51,52,52,44,53,46,48,55,56,49,50,32,
45,50,46,50,54,53,54,51,44,50,46,56,49,50,53,32,45,50,46,50,54,53,54,51,44,52,46,52,49,52,48,55,32,48,44,49,46,50,56,57,48,54,32,49,46,50,49,48,57,52,44,49,46,50,56,57,48,54,32,49,46,55,49,56,55,53,44,
48,32,52,46,55,50,54,53,54,44,45,50,46,50,50,54,53,54,32,51,46,48,52,54,56,56,44,45,50,46,50,54,53,54,51,32,53,46,51,57,48,54,51,44,45,53,46,48,55,56,49,51,32,50,46,51,56,50,56,49,44,45,50,46,56,49,50,
53,32,50,46,51,56,50,56,49,44,45,52,46,50,57,54,56,55,32,48,44,45,49,46,52,52,53,51,50,32,45,49,46,54,48,49,53,54,44,45,49,46,52,52,53,51,50,32,122,32,109,32,49,46,54,55,57,54,57,44,51,46,50,48,51,49,
51,32,50,49,46,48,57,51,55,53,44,45,50,52,46,49,48,49,53,54,32,104,32,51,46,50,48,51,49,50,32,108,32,45,50,52,46,54,48,57,51,55,44,50,56,46,48,56,53,57,51,32,113,32,45,48,46,56,50,48,51,50,44,48,46,57,
51,55,53,32,45,49,46,55,57,54,56,56,44,50,46,49,48,57,51,56,32,45,48,46,57,55,54,53,54,44,49,46,49,55,49,56,55,32,45,49,46,52,52,53,51,49,44,49,46,56,51,53,57,52,32,45,48,46,52,50,57,54,57,44,48,46,54,
54,52,48,54,32,45,48,46,54,50,53,44,49,46,50,56,57,48,54,32,45,48,46,49,53,54,50,53,44,48,46,54,50,53,32,48,46,53,56,53,57,52,44,48,46,54,50,53,32,48,46,57,55,54,53,54,44,48,32,52,46,56,48,52,54,56,44,
45,51,46,48,48,55,56,49,32,51,46,56,50,56,49,51,44,45,51,46,48,52,54,56,56,32,55,46,49,56,55,53,44,45,54,46,48,53,52,54,57,32,118,32,48,46,54,54,52,48,54,32,113,32,45,52,46,54,48,57,51,55,44,52,46,48,
50,51,52,52,32,45,55,46,55,55,51,52,51,44,54,46,52,56,52,51,56,32,45,51,46,49,50,53,44,50,46,52,54,48,57,51,32,45,52,46,53,55,48,51,50,44,50,46,52,54,48,57,51,32,45,49,46,48,49,53,54,50,44,48,32,45,49,
46,48,49,53,54,50,44,45,49,46,50,49,48,57,51,32,48,44,45,49,46,53,54,50,53,32,49,46,48,57,51,55,53,44,45,52,46,48,54,50,53,32,48,44,45,48,46,48,51,57,49,32,45,48,46,48,55,56,49,44,45,48,46,48,55,56,49,
32,45,48,46,48,51,57,49,44,45,48,46,48,55,56,49,32,45,48,46,48,55,56,49,44,45,48,46,48,55,56,49,32,45,53,46,52,50,57,54,57,44,53,46,52,50,57,54,56,32,45,56,46,53,53,52,54,57,44,53,46,52,50,57,54,56,32,
45,50,46,50,50,54,53,54,44,48,32,45,50,46,50,50,54,53,54,44,45,51,46,54,55,49,56,55,32,48,44,45,50,46,56,57,48,54,51,32,50,46,49,56,55,53,44,45,53,46,51,49,50,53,32,50,46,49,56,55,53,44,45,50,46,52,50,
49,56,56,32,53,46,51,57,48,54,50,44,45,51,46,55,56,57,48,54,32,51,46,50,48,51,49,51,44,45,49,46,51,54,55,49,57,32,53,46,55,48,51,49,51,44,45,49,46,51,54,55,49,57,32,49,46,57,57,50,49,57,44,48,32,49,46,
57,57,50,49,57,44,49,46,54,55,57,54,57,32,48,44,49,46,49,55,49,56,55,32,45,48,46,52,54,56,55,53,44,50,46,48,55,48,51,49,32,122,34,10,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,111,110,116,
45,115,116,121,108,101,58,111,98,108,105,113,117,101,59,102,111,110,116,45,118,97,114,105,97,110,116,58,110,111,114,109,97,108,59,102,111,110,116,45,119,101,105,103,104,116,58,110,111,114,109,97,108,59,
102,111,110,116,45,115,116,114,101,116,99,104,58,110,111,114,109,97,108,59,102,111,110,116,45,115,105,122,101,58,56,48,112,120,59,108,105,110,101,45,104,101,105,103,104,116,58,49,50,53,37,59,102,111,110,
116,45,102,97,109,105,108,121,58,39,80,97,108,97,99,101,32,83,99,114,105,112,116,32,77,84,39,59,45,105,110,107,115,99,97,112,101,45,102,111,110,116,45,115,112,101,99,105,102,105,99,97,116,105,111,110,
58,39,80,97,108,97,99,101,32,83,99,114,105,112,116,32,77,84,32,79,98,108,105,113,117,101,39,59,116,101,120,116,45,97,108,105,103,110,58,115,116,97,114,116,59,119,114,105,116,105,110,103,45,109,111,100,
101,58,108,114,45,116,98,59,116,101,120,116,45,97,110,99,104,111,114,58,115,116,97,114,116,59,102,105,108,108,58,35,102,57,102,57,102,57,34,10,32,32,32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,51,
55,51,53,34,32,47,62,10,32,32,32,32,32,32,60,112,97,116,104,10,32,32,32,32,32,32,32,32,32,100,61,34,109,32,50,51,57,46,57,53,56,52,44,53,53,53,46,53,52,50,57,55,32,113,32,45,56,46,50,48,51,49,50,44,52,
46,49,48,49,53,54,32,45,49,51,46,56,54,55,49,56,44,56,46,50,56,49,50,53,32,45,53,46,55,48,51,49,51,44,52,46,49,55,57,54,57,32,45,53,46,55,48,51,49,51,44,54,46,57,57,50,49,57,32,48,44,48,46,55,48,51,49,
50,32,48,46,53,48,55,56,49,44,49,46,49,51,50,56,49,32,48,46,52,54,56,55,53,44,48,46,52,54,56,55,53,32,49,46,49,55,49,56,56,44,48,46,52,54,56,55,53,32,49,46,51,54,55,49,57,44,48,32,51,46,48,56,53,57,52,
44,45,48,46,57,55,54,53,54,32,49,46,55,49,56,55,53,44,45,48,46,57,51,55,53,32,51,46,56,54,55,49,56,44,45,50,46,57,54,56,55,53,32,50,46,49,48,57,51,56,44,45,50,46,48,51,49,50,53,32,52,46,57,54,48,57,52,
44,45,53,46,51,57,48,54,51,32,49,46,53,50,51,52,52,44,45,49,46,56,55,53,32,51,46,50,56,49,50,53,44,45,52,46,48,54,50,53,32,49,46,55,57,54,56,56,44,45,50,46,49,52,56,52,52,32,50,46,54,57,53,51,49,44,45,
51,46,52,55,54,53,54,32,122,32,109,32,45,53,46,57,55,54,53,54,44,45,49,49,46,51,54,55,49,57,32,118,32,45,48,46,54,50,53,32,113,32,51,46,49,50,53,44,45,50,46,54,57,53,51,49,32,53,46,49,49,55,49,57,44,45,
52,46,49,48,49,53,54,32,50,46,48,51,49,50,53,44,45,49,46,52,48,54,50,53,32,51,46,53,49,53,54,50,44,45,49,46,52,48,54,50,53,32,49,46,50,49,48,57,52,44,48,32,49,46,50,49,48,57,52,44,49,46,54,48,49,53,54,
32,48,44,51,46,51,57,56,52,52,32,45,52,46,54,52,56,52,52,44,55,46,53,32,45,51,46,51,53,57,51,55,44,50,46,57,54,56,55,53,32,45,51,46,51,53,57,51,55,44,51,46,57,48,54,50,53,32,48,44,48,46,53,56,53,57,52,
32,48,46,55,48,51,49,50,44,48,46,53,56,53,57,52,32,49,46,52,52,53,51,50,44,48,32,53,44,45,50,46,51,56,50,56,49,32,51,46,53,57,51,55,53,44,45,50,46,52,50,49,56,56,32,55,46,52,50,49,56,56,44,45,53,46,53,
56,53,57,52,32,51,46,56,54,55,49,57,44,45,51,46,50,48,51,49,51,32,53,46,57,55,54,53,54,44,45,53,46,51,49,50,53,32,104,32,50,46,57,54,56,55,53,32,108,32,45,49,50,46,53,51,57,48,54,44,49,52,46,50,57,54,
56,55,32,113,32,52,46,51,55,53,44,45,49,46,55,53,55,56,49,32,55,46,53,44,45,51,46,56,50,56,49,50,32,51,46,49,54,52,48,54,44,45,50,46,49,48,57,51,56,32,55,46,57,50,57,54,57,44,45,54,46,50,53,32,118,32,
48,46,54,54,52,48,54,32,113,32,45,52,46,52,53,51,49,51,44,52,46,49,48,49,53,54,32,45,55,46,56,49,50,53,44,54,46,50,56,57,48,54,32,45,51,46,51,53,57,51,56,44,50,46,49,56,55,53,32,45,56,46,53,49,53,54,51,
44,52,46,49,48,49,53,55,32,108,32,45,48,46,55,48,51,49,50,44,48,46,55,56,49,50,53,32,113,32,45,56,46,49,54,52,48,55,44,57,46,49,52,48,54,50,32,45,49,51,46,50,48,51,49,51,44,49,51,46,56,54,55,49,56,32,
45,53,46,48,51,57,48,54,44,52,46,55,50,54,53,55,32,45,56,46,51,50,48,51,49,44,52,46,55,50,54,53,55,32,45,50,46,52,50,49,56,56,44,48,32,45,50,46,52,50,49,56,56,44,45,50,46,50,54,53,54,51,32,48,44,45,49,
46,54,52,48,54,50,32,50,46,50,50,54,53,55,44,45,52,46,49,52,48,54,50,32,50,46,49,56,55,53,44,45,50,46,52,54,48,57,52,32,54,46,56,51,53,57,51,44,45,53,46,53,48,55,56,50,32,52,46,54,52,56,52,52,44,45,51,
46,48,48,55,56,49,32,49,49,46,56,51,53,57,52,44,45,54,46,51,50,56,49,50,32,51,46,56,50,56,49,51,44,45,52,46,57,50,49,56,56,32,57,46,57,50,49,56,56,44,45,49,49,46,54,48,49,53,54,32,45,52,46,49,52,48,54,
51,44,51,46,50,48,51,49,50,32,45,53,46,52,54,56,55,53,44,52,46,50,49,56,55,53,32,45,49,46,51,50,56,49,51,44,49,46,48,49,53,54,50,32,45,51,46,49,54,52,48,55,44,50,46,51,48,52,54,56,32,45,49,46,56,51,53,
57,51,44,49,46,50,53,32,45,51,46,50,56,49,50,53,44,49,46,56,55,53,32,45,49,46,52,48,54,50,53,44,48,46,54,50,53,32,45,50,46,53,44,48,46,54,50,53,32,45,48,46,54,54,52,48,54,44,48,32,45,49,46,48,57,51,55,
53,44,45,48,46,52,50,57,54,56,32,45,48,46,52,50,57,54,56,44,45,48,46,52,50,57,54,57,32,45,48,46,52,50,57,54,56,44,45,49,46,49,55,49,56,56,32,48,44,45,49,46,50,56,57,48,54,32,48,46,53,48,55,56,49,44,45,
50,46,52,54,48,57,52,32,48,46,53,48,55,56,49,44,45,49,46,49,55,49,56,55,32,49,46,54,48,49,53,54,44,45,50,46,51,52,51,55,53,32,49,46,49,51,50,56,49,44,45,49,46,50,49,48,57,51,32,51,46,48,48,55,56,49,44,
45,50,46,54,49,55,49,56,32,51,46,51,53,57,51,56,44,45,50,46,53,32,51,46,51,53,57,51,56,44,45,51,46,56,54,55,49,57,32,48,44,45,48,46,55,48,51,49,51,32,45,48,46,55,56,49,50,53,44,45,48,46,55,48,51,49,51,
32,45,49,46,51,50,56,49,51,44,48,32,45,51,46,52,51,55,53,44,49,46,54,48,49,53,55,32,45,50,46,49,48,57,51,56,44,49,46,54,48,49,53,54,32,45,52,46,57,54,48,57,52,44,51,46,57,56,52,51,55,32,122,34,10,32,32,
32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,111,110,116,45,115,116,121,108,101,58,111,98,108,105,113,117,101,59,102,111,110,116,45,118,97,114,105,97,110,116,58,110,111,114,109,97,108,59,102,111,
110,116,45,119,101,105,103,104,116,58,110,111,114,109,97,108,59,102,111,110,116,45,115,116,114,101,116,99,104,58,110,111,114,109,97,108,59,102,111,110,116,45,115,105,122,101,58,56,48,112,120,59,108,105,
110,101,45,104,101,105,103,104,116,58,49,50,53,37,59,102,111,110,116,45,102,97,109,105,108,121,58,39,80,97,108,97,99,101,32,83,99,114,105,112,116,32,77,84,39,59,45,105,110,107,115,99,97,112,101,45,102,
111,110,116,45,115,112,101,99,105,102,105,99,97,116,105,111,110,58,39,80,97,108,97,99,101,32,83,99,114,105,112,116,32,77,84,32,79,98,108,105,113,117,101,39,59,116,101,120,116,45,97,108,105,103,110,58,
115,116,97,114,116,59,119,114,105,116,105,110,103,45,109,111,100,101,58,108,114,45,116,98,59,116,101,120,116,45,97,110,99,104,111,114,58,115,116,97,114,116,59,102,105,108,108,58,35,102,57,102,57,102,57,
34,10,32,32,32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,51,55,51,55,34,32,47,62,10,32,32,32,32,32,32,60,112,97,116,104,10,32,32,32,32,32,32,32,32,32,100,61,34,109,32,50,53,57,46,56,52,49,50,50,44,
53,52,52,46,49,55,53,55,56,32,118,32,45,48,46,54,50,53,32,113,32,53,46,57,51,55,53,44,45,53,46,53,48,55,56,49,32,56,46,51,50,48,51,49,44,45,53,46,53,48,55,56,49,32,49,46,51,50,56,49,50,44,48,32,49,46,
51,50,56,49,50,44,49,46,54,55,57,54,57,32,48,44,49,46,51,50,56,49,50,32,45,48,46,56,50,48,51,49,44,50,46,54,49,55,49,56,32,45,48,46,56,50,48,51,49,44,49,46,50,56,57,48,55,32,45,51,46,51,50,48,51,49,44,
52,46,52,57,50,49,57,32,52,46,56,52,51,55,53,44,45,52,46,48,50,51,52,52,32,56,46,51,57,56,52,52,44,45,54,46,52,48,54,50,53,32,51,46,53,53,52,54,56,44,45,50,46,51,56,50,56,49,32,54,46,48,57,51,55,53,44,
45,50,46,51,56,50,56,49,32,49,46,51,50,56,49,50,44,48,32,49,46,51,50,56,49,50,44,49,46,56,51,53,57,52,32,48,44,49,46,55,57,54,56,55,32,45,49,46,50,53,44,51,46,53,57,51,55,53,32,45,49,46,50,53,44,49,46,
55,53,55,56,49,32,45,52,46,49,48,49,53,54,44,51,46,56,50,56,49,50,32,45,51,46,48,52,54,56,56,44,50,46,51,48,52,54,57,32,45,51,46,48,52,54,56,56,44,51,46,50,56,49,50,53,32,48,44,48,46,51,57,48,54,51,32,
48,46,51,53,49,53,55,44,48,46,55,52,50,49,57,32,48,46,51,57,48,54,50,44,48,46,51,49,50,53,32,48,46,55,56,49,50,53,44,48,46,51,49,50,53,32,49,46,51,54,55,49,56,44,48,32,51,46,53,57,51,55,53,44,45,49,46,
52,56,52,51,56,32,50,46,50,54,53,54,50,44,45,49,46,53,50,51,52,51,32,52,46,57,50,49,56,55,44,45,51,46,56,50,56,49,50,32,50,46,54,53,54,50,53,44,45,50,46,51,52,51,55,53,32,52,46,50,57,54,56,56,44,45,51,
46,55,53,32,118,32,48,46,54,54,52,48,54,32,113,32,45,50,46,51,56,50,56,50,44,50,46,50,50,54,53,54,32,45,52,46,54,52,56,52,52,44,52,46,49,55,57,54,57,32,45,50,46,50,54,53,54,51,44,49,46,57,49,52,48,54,
32,45,52,46,53,51,49,50,53,44,51,46,51,53,57,51,55,32,45,50,46,50,50,54,53,54,44,49,46,52,48,54,50,53,32,45,51,46,55,49,48,57,52,44,49,46,52,48,54,50,53,32,45,48,46,57,51,55,53,44,48,32,45,49,46,51,54,
55,49,57,44,45,48,46,54,50,53,32,45,48,46,51,57,48,54,50,44,45,48,46,54,50,53,32,45,48,46,51,57,48,54,50,44,45,49,46,50,56,57,48,54,32,48,44,45,49,46,52,56,52,51,55,32,49,46,49,55,49,56,55,44,45,51,46,
51,50,48,51,49,32,49,46,49,55,49,56,56,44,45,49,46,56,55,53,32,51,46,50,52,50,49,57,44,45,51,46,54,51,50,56,49,32,50,46,51,56,50,56,49,44,45,50,46,49,48,57,51,56,32,51,46,50,56,49,50,53,44,45,51,46,51,
53,57,51,56,32,48,46,57,51,55,53,44,45,49,46,50,56,57,48,54,32,45,48,46,53,48,55,56,49,44,45,49,46,50,56,57,48,54,32,45,49,46,56,51,53,57,52,44,48,32,45,53,46,53,52,54,56,56,44,50,46,53,32,45,51,46,55,
49,48,57,51,44,50,46,52,54,48,57,52,32,45,54,46,57,53,51,49,50,44,53,46,50,51,52,51,55,32,45,51,46,50,52,50,49,57,44,50,46,55,51,52,51,56,32,45,54,46,49,51,50,56,49,44,53,46,51,57,48,54,51,32,104,32,45,
51,46,48,52,54,56,56,32,108,32,53,46,49,57,53,51,49,44,45,53,46,56,53,57,51,56,32,113,32,53,46,55,48,51,49,51,44,45,54,46,51,54,55,49,56,32,53,46,55,48,51,49,51,44,45,54,46,57,53,51,49,50,32,48,44,45,
48,46,51,57,48,54,51,32,45,48,46,52,54,56,55,53,44,45,48,46,51,57,48,54,51,32,45,49,46,55,49,56,55,53,44,48,32,45,56,46,49,54,52,48,54,44,53,46,53,56,53,57,52,32,122,34,10,32,32,32,32,32,32,32,32,32,115,
116,121,108,101,61,34,102,111,110,116,45,115,116,121,108,101,58,111,98,108,105,113,117,101,59,102,111,110,116,45,118,97,114,105,97,110,116,58,110,111,114,109,97,108,59,102,111,110,116,45,119,101,105,103,
104,116,58,110,111,114,109,97,108,59,102,111,110,116,45,115,116,114,101,116,99,104,58,110,111,114,109,97,108,59,102,111,110,116,45,115,105,122,101,58,56,48,112,120,59,108,105,110,101,45,104,101,105,103,
104,116,58,49,50,53,37,59,102,111,110,116,45,102,97,109,105,108,121,58,39,80,97,108,97,99,101,32,83,99,114,105,112,116,32,77,84,39,59,45,105,110,107,115,99,97,112,101,45,102,111,110,116,45,115,112,101,
99,105,102,105,99,97,116,105,111,110,58,39,80,97,108,97,99,101,32,83,99,114,105,112,116,32,77,84,32,79,98,108,105,113,117,101,39,59,116,101,120,116,45,97,108,105,103,110,58,115,116,97,114,116,59,119,114,
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

const char* VASTWaveTableEditorComponent::dynamics_svg = (const char*) resource_VASTWaveTableEditorComponent_dynamics_svg;
const int VASTWaveTableEditorComponent::dynamics_svgSize = 13088;

// JUCER_RESOURCE: close_svg, 1012, "../../Resources/Icons/close.svg"
static const unsigned char resource_VASTWaveTableEditorComponent_close_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,70,45,
56,34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,32,119,105,100,116,104,61,34,50,49,112,120,34,32,104,101,105,103,104,116,61,34,50,48,112,120,34,32,118,105,101,119,
66,111,120,61,34,48,32,48,32,50,49,32,50,48,34,32,118,101,114,115,105,111,110,61,34,49,46,49,34,32,120,109,108,110,115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,
47,115,118,103,34,32,120,109,108,110,115,58,120,108,105,110,107,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,120,108,105,110,107,34,62,10,32,32,32,32,60,33,45,
45,32,71,101,110,101,114,97,116,111,114,58,32,83,107,101,116,99,104,32,51,46,56,46,49,32,40,50,57,54,56,55,41,32,45,32,104,116,116,112,58,47,47,119,119,119,46,98,111,104,101,109,105,97,110,99,111,100,
105,110,103,46,99,111,109,47,115,107,101,116,99,104,32,45,45,62,10,32,32,32,32,60,116,105,116,108,101,62,99,108,111,115,101,32,91,35,49,53,51,56,93,60,47,116,105,116,108,101,62,10,32,32,32,32,60,100,101,
115,99,62,67,114,101,97,116,101,100,32,119,105,116,104,32,83,107,101,116,99,104,46,60,47,100,101,115,99,62,10,32,32,32,32,60,100,101,102,115,62,60,47,100,101,102,115,62,10,32,32,32,32,60,103,32,105,100,
61,34,80,97,103,101,45,49,34,32,115,116,114,111,107,101,61,34,110,111,110,101,34,32,115,116,114,111,107,101,45,119,105,100,116,104,61,34,49,34,32,102,105,108,108,61,34,110,111,110,101,34,32,102,105,108,
108,45,114,117,108,101,61,34,101,118,101,110,111,100,100,34,62,10,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,68,114,105,98,98,98,108,101,45,68,97,114,107,45,80,114,101,118,105,101,119,34,32,116,114,
97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,45,49,51,57,46,48,48,48,48,48,48,44,32,45,49,54,48,46,48,48,48,48,48,48,41,34,32,102,105,108,108,61,34,35,70,70,70,70,70,70,34,62,
10,32,32,32,32,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,105,99,111,110,115,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,53,54,46,48,48,48,48,48,48,44,32,49,
54,48,46,48,48,48,48,48,48,41,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,60,112,97,116,104,32,100,61,34,77,56,53,46,49,44,49,56,32,76,49,48,49,46,57,44,49,56,32,76,49,48,49,46,57,44,50,32,
76,56,53,46,49,44,50,32,76,56,53,46,49,44,49,56,32,90,32,77,56,51,44,50,48,32,76,49,48,52,44,50,48,32,76,49,48,52,44,48,32,76,56,51,44,48,32,76,56,51,44,50,48,32,90,32,77,56,57,46,55,56,55,50,44,49,49,
46,56,53,54,32,76,57,50,46,48,49,52,50,53,44,57,46,55,51,53,32,76,56,57,46,55,56,55,50,44,55,46,54,49,51,32,76,57,49,46,50,55,49,57,44,54,46,49,57,57,32,76,57,51,46,53,44,56,46,51,50,49,32,76,57,53,46,
55,50,55,48,53,44,54,46,49,57,57,32,76,57,55,46,50,49,49,55,53,44,55,46,54,49,51,32,76,57,52,46,57,56,52,55,44,57,46,55,51,53,32,76,57,55,46,50,49,49,55,53,44,49,49,46,56,53,54,32,76,57,53,46,55,50,55,
48,53,44,49,51,46,50,55,32,76,57,51,46,53,44,49,49,46,49,52,57,32,76,57,49,46,50,55,49,57,44,49,51,46,50,55,32,76,56,57,46,55,56,55,50,44,49,49,46,56,53,54,32,90,34,32,105,100,61,34,99,108,111,115,101,
45,91,35,49,53,51,56,93,34,62,60,47,112,97,116,104,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,60,47,103,62,10,60,47,115,118,103,62,0,0};

const char* VASTWaveTableEditorComponent::close_svg = (const char*) resource_VASTWaveTableEditorComponent_close_svg;
const int VASTWaveTableEditorComponent::close_svgSize = 1012;

// JUCER_RESOURCE: ramp_svg, 3304, "../../Resources/Icons/ramp.svg"
static const unsigned char resource_VASTWaveTableEditorComponent_ramp_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,70,45,56,
34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,33,45,45,32,67,114,101,97,116,101,100,32,119,105,116,104,32,73,110,107,115,99,97,112,101,32,40,104,116,116,112,58,47,47,119,119,
119,46,105,110,107,115,99,97,112,101,46,111,114,103,47,41,32,45,45,62,10,10,60,115,118,103,10,32,32,32,120,109,108,110,115,58,100,99,61,34,104,116,116,112,58,47,47,112,117,114,108,46,111,114,103,47,100,
99,47,101,108,101,109,101,110,116,115,47,49,46,49,47,34,10,32,32,32,120,109,108,110,115,58,99,99,61,34,104,116,116,112,58,47,47,99,114,101,97,116,105,118,101,99,111,109,109,111,110,115,46,111,114,103,
47,110,115,35,34,10,32,32,32,120,109,108,110,115,58,114,100,102,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,48,50,47,50,50,45,114,100,102,45,115,121,110,116,97,
120,45,110,115,35,34,10,32,32,32,120,109,108,110,115,58,115,118,103,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,118,103,34,10,32,32,32,120,109,108,110,115,
61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,118,103,34,10,32,32,32,120,109,108,110,115,58,115,111,100,105,112,111,100,105,61,34,104,116,116,112,58,47,47,115,
111,100,105,112,111,100,105,46,115,111,117,114,99,101,102,111,114,103,101,46,110,101,116,47,68,84,68,47,115,111,100,105,112,111,100,105,45,48,46,100,116,100,34,10,32,32,32,120,109,108,110,115,58,105,110,
107,115,99,97,112,101,61,34,104,116,116,112,58,47,47,119,119,119,46,105,110,107,115,99,97,112,101,46,111,114,103,47,110,97,109,101,115,112,97,99,101,115,47,105,110,107,115,99,97,112,101,34,10,32,32,32,
119,105,100,116,104,61,34,50,49,48,109,109,34,10,32,32,32,104,101,105,103,104,116,61,34,50,57,55,109,109,34,10,32,32,32,118,105,101,119,66,111,120,61,34,48,32,48,32,50,49,48,32,50,57,55,34,10,32,32,32,
118,101,114,115,105,111,110,61,34,49,46,49,34,10,32,32,32,105,100,61,34,115,118,103,55,53,49,53,34,10,32,32,32,105,110,107,115,99,97,112,101,58,118,101,114,115,105,111,110,61,34,48,46,57,50,46,51,32,40,
50,52,48,53,53,52,54,44,32,50,48,49,56,45,48,51,45,49,49,41,34,10,32,32,32,115,111,100,105,112,111,100,105,58,100,111,99,110,97,109,101,61,34,114,97,109,112,46,115,118,103,34,62,10,32,32,60,100,101,102,
115,10,32,32,32,32,32,105,100,61,34,100,101,102,115,55,53,48,57,34,32,47,62,10,32,32,60,115,111,100,105,112,111,100,105,58,110,97,109,101,100,118,105,101,119,10,32,32,32,32,32,105,100,61,34,98,97,115,
101,34,10,32,32,32,32,32,112,97,103,101,99,111,108,111,114,61,34,35,102,102,102,102,102,102,34,10,32,32,32,32,32,98,111,114,100,101,114,99,111,108,111,114,61,34,35,54,54,54,54,54,54,34,10,32,32,32,32,
32,98,111,114,100,101,114,111,112,97,99,105,116,121,61,34,49,46,48,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,112,97,103,101,111,112,97,99,105,116,121,61,34,48,46,48,34,10,32,32,32,32,32,105,
110,107,115,99,97,112,101,58,112,97,103,101,115,104,97,100,111,119,61,34,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,122,111,111,109,61,34,49,46,52,34,10,32,32,32,32,32,105,110,107,115,99,
97,112,101,58,99,120,61,34,50,49,49,46,56,51,54,55,51,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,121,61,34,54,53,49,46,53,53,51,55,53,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,
100,111,99,117,109,101,110,116,45,117,110,105,116,115,61,34,109,109,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,117,114,114,101,110,116,45,108,97,121,101,114,61,34,108,97,121,101,114,49,34,
10,32,32,32,32,32,115,104,111,119,103,114,105,100,61,34,102,97,108,115,101,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,119,105,100,116,104,61,34,51,56,52,48,34,10,
32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,104,101,105,103,104,116,61,34,50,48,53,48,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,120,
61,34,45,49,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,121,61,34,45,49,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,109,
97,120,105,109,105,122,101,100,61,34,49,34,32,47,62,10,32,32,60,109,101,116,97,100,97,116,97,10,32,32,32,32,32,105,100,61,34,109,101,116,97,100,97,116,97,55,53,49,50,34,62,10,32,32,32,32,60,114,100,102,
58,82,68,70,62,10,32,32,32,32,32,32,60,99,99,58,87,111,114,107,10,32,32,32,32,32,32,32,32,32,114,100,102,58,97,98,111,117,116,61,34,34,62,10,32,32,32,32,32,32,32,32,60,100,99,58,102,111,114,109,97,116,
62,105,109,97,103,101,47,115,118,103,43,120,109,108,60,47,100,99,58,102,111,114,109,97,116,62,10,32,32,32,32,32,32,32,32,60,100,99,58,116,121,112,101,10,32,32,32,32,32,32,32,32,32,32,32,114,100,102,58,
114,101,115,111,117,114,99,101,61,34,104,116,116,112,58,47,47,112,117,114,108,46,111,114,103,47,100,99,47,100,99,109,105,116,121,112,101,47,83,116,105,108,108,73,109,97,103,101,34,32,47,62,10,32,32,32,
32,32,32,32,32,60,100,99,58,116,105,116,108,101,62,60,47,100,99,58,116,105,116,108,101,62,10,32,32,32,32,32,32,60,47,99,99,58,87,111,114,107,62,10,32,32,32,32,60,47,114,100,102,58,82,68,70,62,10,32,32,
60,47,109,101,116,97,100,97,116,97,62,10,32,32,60,103,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,108,97,98,101,108,61,34,69,98,101,110,101,32,49,34,10,32,32,32,32,32,105,110,107,115,99,97,112,
101,58,103,114,111,117,112,109,111,100,101,61,34,108,97,121,101,114,34,10,32,32,32,32,32,105,100,61,34,108,97,121,101,114,49,34,62,10,32,32,32,32,60,112,97,116,104,10,32,32,32,32,32,32,32,115,116,121,
108,101,61,34,99,111,108,111,114,58,35,48,48,48,48,48,48,59,102,111,110,116,45,115,116,121,108,101,58,110,111,114,109,97,108,59,102,111,110,116,45,118,97,114,105,97,110,116,58,110,111,114,109,97,108,59,
102,111,110,116,45,119,101,105,103,104,116,58,110,111,114,109,97,108,59,102,111,110,116,45,115,116,114,101,116,99,104,58,110,111,114,109,97,108,59,102,111,110,116,45,115,105,122,101,58,109,101,100,105,
117,109,59,108,105,110,101,45,104,101,105,103,104,116,58,110,111,114,109,97,108,59,102,111,110,116,45,102,97,109,105,108,121,58,115,97,110,115,45,115,101,114,105,102,59,102,111,110,116,45,118,97,114,105,
97,110,116,45,108,105,103,97,116,117,114,101,115,58,110,111,114,109,97,108,59,102,111,110,116,45,118,97,114,105,97,110,116,45,112,111,115,105,116,105,111,110,58,110,111,114,109,97,108,59,102,111,110,116,
45,118,97,114,105,97,110,116,45,99,97,112,115,58,110,111,114,109,97,108,59,102,111,110,116,45,118,97,114,105,97,110,116,45,110,117,109,101,114,105,99,58,110,111,114,109,97,108,59,102,111,110,116,45,118,
97,114,105,97,110,116,45,97,108,116,101,114,110,97,116,101,115,58,110,111,114,109,97,108,59,102,111,110,116,45,102,101,97,116,117,114,101,45,115,101,116,116,105,110,103,115,58,110,111,114,109,97,108,59,
116,101,120,116,45,105,110,100,101,110,116,58,48,59,116,101,120,116,45,97,108,105,103,110,58,115,116,97,114,116,59,116,101,120,116,45,100,101,99,111,114,97,116,105,111,110,58,110,111,110,101,59,116,101,
120,116,45,100,101,99,111,114,97,116,105,111,110,45,108,105,110,101,58,110,111,110,101,59,116,101,120,116,45,100,101,99,111,114,97,116,105,111,110,45,115,116,121,108,101,58,115,111,108,105,100,59,116,
101,120,116,45,100,101,99,111,114,97,116,105,111,110,45,99,111,108,111,114,58,35,48,48,48,48,48,48,59,108,101,116,116,101,114,45,115,112,97,99,105,110,103,58,110,111,114,109,97,108,59,119,111,114,100,
45,115,112,97,99,105,110,103,58,110,111,114,109,97,108,59,116,101,120,116,45,116,114,97,110,115,102,111,114,109,58,110,111,110,101,59,119,114,105,116,105,110,103,45,109,111,100,101,58,108,114,45,116,98,
59,100,105,114,101,99,116,105,111,110,58,108,116,114,59,116,101,120,116,45,111,114,105,101,110,116,97,116,105,111,110,58,109,105,120,101,100,59,100,111,109,105,110,97,110,116,45,98,97,115,101,108,105,
110,101,58,97,117,116,111,59,98,97,115,101,108,105,110,101,45,115,104,105,102,116,58,98,97,115,101,108,105,110,101,59,116,101,120,116,45,97,110,99,104,111,114,58,115,116,97,114,116,59,119,104,105,116,
101,45,115,112,97,99,101,58,110,111,114,109,97,108,59,115,104,97,112,101,45,112,97,100,100,105,110,103,58,48,59,99,108,105,112,45,114,117,108,101,58,110,111,110,122,101,114,111,59,100,105,115,112,108,
97,121,58,105,110,108,105,110,101,59,111,118,101,114,102,108,111,119,58,118,105,115,105,98,108,101,59,118,105,115,105,98,105,108,105,116,121,58,118,105,115,105,98,108,101,59,111,112,97,99,105,116,121,
58,49,59,105,115,111,108,97,116,105,111,110,58,97,117,116,111,59,109,105,120,45,98,108,101,110,100,45,109,111,100,101,58,110,111,114,109,97,108,59,99,111,108,111,114,45,105,110,116,101,114,112,111,108,
97,116,105,111,110,58,115,82,71,66,59,99,111,108,111,114,45,105,110,116,101,114,112,111,108,97,116,105,111,110,45,102,105,108,116,101,114,115,58,108,105,110,101,97,114,82,71,66,59,115,111,108,105,100,
45,99,111,108,111,114,58,35,48,48,48,48,48,48,59,115,111,108,105,100,45,111,112,97,99,105,116,121,58,49,59,118,101,99,116,111,114,45,101,102,102,101,99,116,58,110,111,110,101,59,102,105,108,108,58,35,
102,102,102,102,102,102,59,102,105,108,108,45,111,112,97,99,105,116,121,58,49,59,102,105,108,108,45,114,117,108,101,58,110,111,110,122,101,114,111,59,115,116,114,111,107,101,58,110,111,110,101,59,115,
116,114,111,107,101,45,119,105,100,116,104,58,48,46,50,54,52,53,56,51,51,50,112,120,59,115,116,114,111,107,101,45,108,105,110,101,99,97,112,58,98,117,116,116,59,115,116,114,111,107,101,45,108,105,110,
101,106,111,105,110,58,109,105,116,101,114,59,115,116,114,111,107,101,45,109,105,116,101,114,108,105,109,105,116,58,52,59,115,116,114,111,107,101,45,100,97,115,104,97,114,114,97,121,58,110,111,110,101,
59,115,116,114,111,107,101,45,100,97,115,104,111,102,102,115,101,116,58,48,59,115,116,114,111,107,101,45,111,112,97,99,105,116,121,58,49,59,99,111,108,111,114,45,114,101,110,100,101,114,105,110,103,58,
97,117,116,111,59,105,109,97,103,101,45,114,101,110,100,101,114,105,110,103,58,97,117,116,111,59,115,104,97,112,101,45,114,101,110,100,101,114,105,110,103,58,97,117,116,111,59,116,101,120,116,45,114,101,
110,100,101,114,105,110,103,58,97,117,116,111,59,101,110,97,98,108,101,45,98,97,99,107,103,114,111,117,110,100,58,97,99,99,117,109,117,108,97,116,101,34,10,32,32,32,32,32,32,32,100,61,34,109,32,49,54,
53,46,54,50,51,48,53,44,55,51,46,51,51,53,57,51,56,32,99,32,45,55,46,50,50,51,49,57,44,49,46,56,51,52,50,53,54,32,45,49,50,46,57,56,50,55,53,44,55,46,56,50,48,57,56,32,45,49,57,46,55,48,52,57,52,44,49,
49,46,48,51,54,57,53,52,32,67,32,49,48,48,46,49,52,57,54,54,44,49,49,50,46,51,48,50,50,55,32,53,52,46,54,52,53,51,51,54,44,49,52,48,46,54,54,52,55,51,32,56,46,57,51,57,52,53,51,49,44,49,54,56,46,55,48,
49,49,55,32,55,46,52,53,52,50,54,54,44,49,55,57,46,48,48,54,53,56,32,50,49,46,53,56,50,48,49,57,44,49,56,55,46,55,56,56,53,51,32,50,57,46,52,53,55,48,51,49,44,49,56,48,46,50,51,54,51,51,32,56,50,46,54,
57,53,52,50,57,44,49,52,54,46,53,56,54,48,56,32,49,51,54,46,54,56,54,50,57,44,49,49,52,46,49,48,54,53,55,32,49,57,48,46,49,54,55,57,55,44,56,48,46,56,49,50,53,32,99,32,45,53,46,48,56,56,51,54,44,45,49,
46,48,57,56,50,52,55,32,45,49,49,46,48,50,56,48,50,44,49,46,53,48,56,56,55,50,32,45,49,53,46,55,51,50,52,50,44,45,48,46,55,55,55,51,52,52,32,45,49,46,52,51,50,49,57,44,45,51,46,53,56,50,53,51,50,32,45,
52,46,55,48,50,57,53,44,45,54,46,56,48,56,54,49,49,32,45,56,46,56,49,50,53,44,45,54,46,54,57,57,50,49,56,32,122,34,10,32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,56,48,54,48,34,10,32,32,32,32,32,
32,32,105,110,107,115,99,97,112,101,58,99,111,110,110,101,99,116,111,114,45,99,117,114,118,97,116,117,114,101,61,34,48,34,32,47,62,10,32,32,60,47,103,62,10,60,47,115,118,103,62,10,0,0};

const char* VASTWaveTableEditorComponent::ramp_svg = (const char*) resource_VASTWaveTableEditorComponent_ramp_svg;
const int VASTWaveTableEditorComponent::ramp_svgSize = 3304;

// JUCER_RESOURCE: invert_svg, 1014, "../../Resources/Icons/invert.svg"
static const unsigned char resource_VASTWaveTableEditorComponent_invert_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,70,45,
56,34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,32,119,105,100,116,104,61,34,50,53,112,120,34,32,104,101,105,103,104,116,61,34,50,49,112,120,34,32,118,105,101,119,
66,111,120,61,34,48,32,48,32,50,53,32,50,49,34,32,118,101,114,115,105,111,110,61,34,49,46,49,34,32,120,109,108,110,115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,
47,115,118,103,34,32,120,109,108,110,115,58,120,108,105,110,107,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,120,108,105,110,107,34,62,10,32,32,32,32,60,33,45,
45,32,71,101,110,101,114,97,116,111,114,58,32,83,107,101,116,99,104,32,51,46,56,46,49,32,40,50,57,54,56,55,41,32,45,32,104,116,116,112,58,47,47,119,119,119,46,98,111,104,101,109,105,97,110,99,111,100,
105,110,103,46,99,111,109,47,115,107,101,116,99,104,32,45,45,62,10,32,32,32,32,60,116,105,116,108,101,62,97,114,114,111,119,95,117,112,32,91,35,51,54,57,93,60,47,116,105,116,108,101,62,10,32,32,32,32,
60,100,101,115,99,62,67,114,101,97,116,101,100,32,119,105,116,104,32,83,107,101,116,99,104,46,60,47,100,101,115,99,62,10,32,32,32,32,60,100,101,102,115,62,60,47,100,101,102,115,62,10,32,32,32,32,60,103,
32,105,100,61,34,80,97,103,101,45,49,34,32,115,116,114,111,107,101,61,34,110,111,110,101,34,32,115,116,114,111,107,101,45,119,105,100,116,104,61,34,49,34,32,102,105,108,108,61,34,110,111,110,101,34,32,
102,105,108,108,45,114,117,108,101,61,34,101,118,101,110,111,100,100,34,62,10,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,68,114,105,98,98,98,108,101,45,68,97,114,107,45,80,114,101,118,105,101,119,
34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,45,49,56,48,46,48,48,48,48,48,48,44,32,45,54,53,53,57,46,48,48,48,48,48,48,41,34,32,102,105,108,108,61,34,35,70,70,70,
70,70,70,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,105,99,111,110,115,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,53,54,46,48,48,48,48,
48,48,44,32,49,54,48,46,48,48,48,48,48,48,41,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,60,112,97,116,104,32,100,61,34,77,49,52,50,44,54,52,49,55,46,49,52,54,32,76,49,52,50,44,54,52,48,50,
46,56,50,56,32,76,49,51,56,46,54,56,52,44,54,52,48,54,46,48,55,49,32,76,49,51,55,46,50,51,52,44,54,52,48,52,46,54,53,55,32,67,49,51,57,46,51,53,53,44,54,52,48,50,46,53,51,53,32,49,52,48,46,55,50,56,44,
54,52,48,49,46,49,52,52,32,49,52,50,46,56,55,50,44,54,51,57,57,32,67,49,52,52,46,57,51,56,44,54,52,48,49,46,48,54,54,32,49,52,54,46,53,49,53,44,54,52,48,50,46,54,53,50,32,49,52,56,46,53,50,44,54,52,48,
52,46,54,53,55,32,76,49,52,55,46,49,55,52,44,54,52,48,54,46,48,55,49,32,76,49,52,52,44,54,52,48,50,46,56,50,56,32,76,49,52,52,44,54,52,49,57,46,49,52,54,32,76,49,50,52,44,54,52,49,57,46,49,52,54,32,76,
49,50,52,44,54,51,57,57,46,49,52,54,32,76,49,50,54,44,54,51,57,57,46,49,52,54,32,76,49,50,54,44,54,52,49,55,46,49,52,54,32,76,49,52,50,44,54,52,49,55,46,49,52,54,32,90,34,32,105,100,61,34,97,114,114,111,
119,95,117,112,45,91,35,51,54,57,93,34,62,60,47,112,97,116,104,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,60,47,103,62,10,60,47,115,118,
103,62,0,0};

const char* VASTWaveTableEditorComponent::invert_svg = (const char*) resource_VASTWaveTableEditorComponent_invert_svg;
const int VASTWaveTableEditorComponent::invert_svgSize = 1014;

// JUCER_RESOURCE: reverse_svg, 996, "../../Resources/Icons/reverse.svg"
static const unsigned char resource_VASTWaveTableEditorComponent_reverse_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,70,45,
56,34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,32,119,105,100,116,104,61,34,50,48,112,120,34,32,104,101,105,103,104,116,61,34,50,53,112,120,34,32,118,105,101,119,
66,111,120,61,34,48,32,48,32,50,48,32,50,53,34,32,118,101,114,115,105,111,110,61,34,49,46,49,34,32,120,109,108,110,115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,
47,115,118,103,34,32,120,109,108,110,115,58,120,108,105,110,107,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,120,108,105,110,107,34,62,10,32,32,32,32,60,33,45,
45,32,71,101,110,101,114,97,116,111,114,58,32,83,107,101,116,99,104,32,51,46,56,46,49,32,40,50,57,54,56,55,41,32,45,32,104,116,116,112,58,47,47,119,119,119,46,98,111,104,101,109,105,97,110,99,111,100,
105,110,103,46,99,111,109,47,115,107,101,116,99,104,32,45,45,62,10,32,32,32,32,60,116,105,116,108,101,62,97,114,114,111,119,95,114,105,103,104,116,32,91,35,51,55,48,93,60,47,116,105,116,108,101,62,10,
32,32,32,32,60,100,101,115,99,62,67,114,101,97,116,101,100,32,119,105,116,104,32,83,107,101,116,99,104,46,60,47,100,101,115,99,62,10,32,32,32,32,60,100,101,102,115,62,60,47,100,101,102,115,62,10,32,32,
32,32,60,103,32,105,100,61,34,80,97,103,101,45,49,34,32,115,116,114,111,107,101,61,34,110,111,110,101,34,32,115,116,114,111,107,101,45,119,105,100,116,104,61,34,49,34,32,102,105,108,108,61,34,110,111,
110,101,34,32,102,105,108,108,45,114,117,108,101,61,34,101,118,101,110,111,100,100,34,62,10,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,68,114,105,98,98,98,108,101,45,68,97,114,107,45,80,114,101,118,
105,101,119,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,45,49,52,48,46,48,48,48,48,48,48,44,32,45,54,53,53,57,46,48,48,48,48,48,48,41,34,32,102,105,108,108,61,34,
35,70,70,70,70,70,70,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,105,99,111,110,115,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,53,54,46,
48,48,48,48,48,48,44,32,49,54,48,46,48,48,48,48,48,48,41,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,60,112,97,116,104,32,100,61,34,77,56,54,44,54,52,49,55,46,48,48,50,49,57,32,76,57,57,46,
56,50,56,44,54,52,49,55,46,48,48,50,49,57,32,76,57,54,46,53,56,54,44,54,52,49,51,46,55,53,56,56,32,76,57,56,44,54,52,49,50,46,51,52,52,54,50,32,76,49,48,51,46,54,53,55,44,54,52,49,56,46,48,48,50,51,49,
32,67,49,48,49,46,53,57,49,44,54,52,50,48,46,48,54,56,53,54,32,49,48,48,46,48,48,53,44,54,52,50,49,46,54,53,52,55,54,32,57,56,44,54,52,50,51,46,54,54,32,76,57,54,46,53,56,54,44,54,52,50,50,46,50,52,53,
56,51,32,76,57,57,46,56,50,56,44,54,52,49,57,46,48,48,50,52,51,32,76,56,52,44,54,52,49,57,46,48,48,50,52,51,32,76,56,52,44,54,51,57,57,32,76,49,48,52,44,54,51,57,57,32,76,49,48,52,44,54,52,48,49,46,48,
48,48,50,52,32,76,56,54,44,54,52,48,49,46,48,48,48,50,52,32,76,56,54,44,54,52,49,55,46,48,48,50,49,57,32,90,34,32,105,100,61,34,97,114,114,111,119,95,114,105,103,104,116,45,91,35,51,55,48,93,34,62,60,
47,112,97,116,104,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,60,47,103,62,10,60,47,115,118,103,62,0,0};

const char* VASTWaveTableEditorComponent::reverse_svg = (const char*) resource_VASTWaveTableEditorComponent_reverse_svg;
const int VASTWaveTableEditorComponent::reverse_svgSize = 996;

// JUCER_RESOURCE: to_editor_svg, 1192, "../../Resources/Icons/to_editor.svg"
static const unsigned char resource_VASTWaveTableEditorComponent_to_editor_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,70,
45,56,34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,32,119,105,100,116,104,61,34,50,48,112,120,34,32,104,101,105,103,104,116,61,34,57,112,120,34,32,118,105,101,119,
66,111,120,61,34,48,32,48,32,50,48,32,57,34,32,118,101,114,115,105,111,110,61,34,49,46,49,34,32,120,109,108,110,115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,
115,118,103,34,32,120,109,108,110,115,58,120,108,105,110,107,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,120,108,105,110,107,34,62,10,32,32,32,32,60,33,45,45,
32,71,101,110,101,114,97,116,111,114,58,32,83,107,101,116,99,104,32,51,46,56,46,49,32,40,50,57,54,56,55,41,32,45,32,104,116,116,112,58,47,47,119,119,119,46,98,111,104,101,109,105,97,110,99,111,100,105,
110,103,46,99,111,109,47,115,107,101,116,99,104,32,45,45,62,10,32,32,32,32,60,116,105,116,108,101,62,97,99,116,105,118,105,116,121,32,91,35,57,56,53,93,60,47,116,105,116,108,101,62,10,32,32,32,32,60,100,
101,115,99,62,67,114,101,97,116,101,100,32,119,105,116,104,32,83,107,101,116,99,104,46,60,47,100,101,115,99,62,10,32,32,32,32,60,100,101,102,115,62,60,47,100,101,102,115,62,10,32,32,32,32,60,103,32,105,
100,61,34,80,97,103,101,45,49,34,32,115,116,114,111,107,101,61,34,110,111,110,101,34,32,115,116,114,111,107,101,45,119,105,100,116,104,61,34,49,34,32,102,105,108,108,61,34,110,111,110,101,34,32,102,105,
108,108,45,114,117,108,101,61,34,101,118,101,110,111,100,100,34,62,10,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,68,114,105,98,98,98,108,101,45,68,97,114,107,45,80,114,101,118,105,101,119,34,32,116,
114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,45,50,54,48,46,48,48,48,48,48,48,44,32,45,51,56,52,53,46,48,48,48,48,48,48,41,34,32,102,105,108,108,61,34,35,70,70,70,70,70,70,
34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,105,99,111,110,115,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,53,54,46,48,48,48,48,48,48,44,
32,49,54,48,46,48,48,48,48,48,48,41,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,60,112,97,116,104,32,100,61,34,77,50,49,54,46,53,49,50,44,51,54,56,53,32,76,50,49,48,46,55,57,54,44,51,54,57,
48,46,55,57,32,76,50,48,56,46,48,53,49,44,51,54,56,56,46,49,53,53,32,76,50,48,56,46,48,52,52,44,51,54,56,56,46,49,54,50,32,76,50,48,56,46,48,51,56,44,51,54,56,56,46,49,53,54,32,76,50,48,52,44,51,54,57,
50,46,51,56,54,32,76,50,48,53,46,52,53,51,44,51,54,57,51,46,56,32,76,50,48,56,46,49,49,53,44,51,54,57,49,46,48,51,51,32,76,50,49,48,46,56,53,57,44,51,54,57,51,46,54,53,51,32,76,50,49,54,46,53,49,50,44,
51,54,56,55,46,56,56,53,32,76,50,49,57,46,50,53,50,44,51,54,57,48,46,54,54,32,67,50,49,57,46,53,52,53,44,51,54,57,48,46,48,50,56,32,50,50,48,46,48,52,54,44,51,54,56,57,46,53,49,52,32,50,50,48,46,54,54,
54,44,51,54,56,57,46,50,48,56,32,76,50,49,54,46,53,49,50,44,51,54,56,53,32,90,32,77,50,50,52,44,51,54,57,49,46,57,54,32,67,50,50,52,44,51,54,57,51,46,48,56,55,32,50,50,51,46,48,57,56,44,51,54,57,52,32,
50,50,49,46,57,56,54,44,51,54,57,52,32,67,50,50,48,46,56,55,52,44,51,54,57,52,32,50,49,57,46,57,55,51,44,51,54,57,51,46,48,56,55,32,50,49,57,46,57,55,51,44,51,54,57,49,46,57,54,32,67,50,49,57,46,57,55,
51,44,51,54,57,48,46,56,51,51,32,50,50,48,46,56,55,52,44,51,54,56,57,46,57,50,32,50,50,49,46,57,56,54,44,51,54,56,57,46,57,50,32,67,50,50,51,46,48,57,56,44,51,54,56,57,46,57,50,32,50,50,52,44,51,54,57,
48,46,56,51,51,32,50,50,52,44,51,54,57,49,46,57,54,32,76,50,50,52,44,51,54,57,49,46,57,54,32,90,34,32,105,100,61,34,97,99,116,105,118,105,116,121,45,91,35,57,56,53,93,34,62,60,47,112,97,116,104,62,10,
32,32,32,32,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,60,47,103,62,10,60,47,115,118,103,62,0,0};

const char* VASTWaveTableEditorComponent::to_editor_svg = (const char*) resource_VASTWaveTableEditorComponent_to_editor_svg;
const int VASTWaveTableEditorComponent::to_editor_svgSize = 1192;

// JUCER_RESOURCE: load_svg, 907, "../../Resources/Icons/load.svg"
static const unsigned char resource_VASTWaveTableEditorComponent_load_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,70,45,56,
34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,32,119,105,100,116,104,61,34,50,48,112,120,34,32,104,101,105,103,104,116,61,34,50,48,112,120,34,32,118,105,101,119,
66,111,120,61,34,48,32,48,32,50,48,32,50,48,34,32,118,101,114,115,105,111,110,61,34,49,46,49,34,32,120,109,108,110,115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,
47,115,118,103,34,32,120,109,108,110,115,58,120,108,105,110,107,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,120,108,105,110,107,34,62,10,32,32,32,32,60,33,45,
45,32,71,101,110,101,114,97,116,111,114,58,32,83,107,101,116,99,104,32,51,46,56,46,49,32,40,50,57,54,56,55,41,32,45,32,104,116,116,112,58,47,47,119,119,119,46,98,111,104,101,109,105,97,110,99,111,100,
105,110,103,46,99,111,109,47,115,107,101,116,99,104,32,45,45,62,10,32,32,32,32,60,116,105,116,108,101,62,98,114,111,119,115,101,114,32,91,35,50,51,51,93,60,47,116,105,116,108,101,62,10,32,32,32,32,60,
100,101,115,99,62,67,114,101,97,116,101,100,32,119,105,116,104,32,83,107,101,116,99,104,46,60,47,100,101,115,99,62,10,32,32,32,32,60,100,101,102,115,62,60,47,100,101,102,115,62,10,32,32,32,32,60,103,32,
105,100,61,34,80,97,103,101,45,49,34,32,115,116,114,111,107,101,61,34,110,111,110,101,34,32,115,116,114,111,107,101,45,119,105,100,116,104,61,34,49,34,32,102,105,108,108,61,34,110,111,110,101,34,32,102,
105,108,108,45,114,117,108,101,61,34,101,118,101,110,111,100,100,34,62,10,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,68,114,105,98,98,98,108,101,45,68,97,114,107,45,80,114,101,118,105,101,119,34,
32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,45,54,48,46,48,48,48,48,48,48,44,32,45,55,49,53,57,46,48,48,48,48,48,48,41,34,32,102,105,108,108,61,34,35,70,70,70,70,70,
70,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,105,99,111,110,115,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,53,54,46,48,48,48,48,48,48,
44,32,49,54,48,46,48,48,48,48,48,48,41,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,60,112,97,116,104,32,100,61,34,77,49,48,44,55,48,48,51,32,76,50,50,44,55,48,48,51,32,76,50,50,44,55,48,48,
49,32,76,49,48,44,55,48,48,49,32,76,49,48,44,55,48,48,51,32,90,32,77,54,44,55,48,49,55,32,76,50,50,44,55,48,49,55,32,76,50,50,44,55,48,48,53,32,76,54,44,55,48,48,53,32,76,54,44,55,48,49,55,32,90,32,77,
54,44,55,48,48,51,32,76,56,44,55,48,48,51,32,76,56,44,55,48,48,49,32,76,54,44,55,48,48,49,32,76,54,44,55,48,48,51,32,90,32,77,52,44,55,48,49,57,32,76,50,52,44,55,48,49,57,32,76,50,52,44,54,57,57,57,32,
76,52,44,54,57,57,57,32,76,52,44,55,48,49,57,32,90,34,32,105,100,61,34,98,114,111,119,115,101,114,45,91,35,50,51,51,93,34,62,60,47,112,97,116,104,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,47,103,62,
10,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,60,47,103,62,10,60,47,115,118,103,62,0,0};

const char* VASTWaveTableEditorComponent::load_svg = (const char*) resource_VASTWaveTableEditorComponent_load_svg;
const int VASTWaveTableEditorComponent::load_svgSize = 907;

// JUCER_RESOURCE: zero_find_svg, 1166, "../../Resources/Icons/zero_find.svg"
static const unsigned char resource_VASTWaveTableEditorComponent_zero_find_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,70,
45,56,34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,32,119,105,100,116,104,61,34,50,49,112,120,34,32,104,101,105,103,104,116,61,34,50,48,112,120,34,32,118,105,101,
119,66,111,120,61,34,48,32,48,32,50,49,32,50,48,34,32,118,101,114,115,105,111,110,61,34,49,46,49,34,32,120,109,108,110,115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,
48,48,47,115,118,103,34,32,120,109,108,110,115,58,120,108,105,110,107,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,120,108,105,110,107,34,62,10,32,32,32,32,60,
33,45,45,32,71,101,110,101,114,97,116,111,114,58,32,83,107,101,116,99,104,32,51,46,56,46,49,32,40,50,57,54,56,55,41,32,45,32,104,116,116,112,58,47,47,119,119,119,46,98,111,104,101,109,105,97,110,99,111,
100,105,110,103,46,99,111,109,47,115,107,101,116,99,104,32,45,45,62,10,32,32,32,32,60,116,105,116,108,101,62,115,101,97,114,99,104,95,108,101,102,116,32,91,35,49,53,48,52,93,60,47,116,105,116,108,101,
62,10,32,32,32,32,60,100,101,115,99,62,67,114,101,97,116,101,100,32,119,105,116,104,32,83,107,101,116,99,104,46,60,47,100,101,115,99,62,10,32,32,32,32,60,100,101,102,115,62,60,47,100,101,102,115,62,10,
32,32,32,32,60,103,32,105,100,61,34,80,97,103,101,45,49,34,32,115,116,114,111,107,101,61,34,110,111,110,101,34,32,115,116,114,111,107,101,45,119,105,100,116,104,61,34,49,34,32,102,105,108,108,61,34,110,
111,110,101,34,32,102,105,108,108,45,114,117,108,101,61,34,101,118,101,110,111,100,100,34,62,10,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,68,114,105,98,98,98,108,101,45,68,97,114,107,45,80,114,101,
118,105,101,119,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,45,50,57,57,46,48,48,48,48,48,48,44,32,45,50,56,48,46,48,48,48,48,48,48,41,34,32,102,105,108,108,61,
34,35,70,70,70,70,70,70,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,105,99,111,110,115,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,53,54,
46,48,48,48,48,48,48,44,32,49,54,48,46,48,48,48,48,48,48,41,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,60,112,97,116,104,32,100,61,34,77,50,54,52,44,49,51,56,46,53,56,54,32,76,50,54,50,46,
53,49,53,51,44,49,52,48,32,76,50,53,56,46,48,54,48,49,53,44,49,51,53,46,55,53,56,32,76,50,53,57,46,53,52,52,56,53,44,49,51,52,46,51,52,51,32,76,50,54,52,44,49,51,56,46,53,56,54,32,90,32,77,50,53,49,46,
52,44,49,51,52,32,67,50,52,55,46,57,50,54,54,44,49,51,52,32,50,52,53,46,49,44,49,51,49,46,51,48,57,32,50,52,53,46,49,44,49,50,56,32,67,50,52,53,46,49,44,49,50,52,46,54,57,50,32,50,52,55,46,57,50,54,54,
44,49,50,50,32,50,53,49,46,52,44,49,50,50,32,67,50,53,52,46,56,55,51,52,44,49,50,50,32,50,53,55,46,55,44,49,50,52,46,54,57,50,32,50,53,55,46,55,44,49,50,56,32,67,50,53,55,46,55,44,49,51,49,46,51,48,57,
32,50,53,52,46,56,55,51,52,44,49,51,52,32,50,53,49,46,52,44,49,51,52,32,76,50,53,49,46,52,44,49,51,52,32,90,32,77,50,53,49,46,52,44,49,50,48,32,67,50,52,54,46,55,54,49,49,44,49,50,48,32,50,52,51,44,49,
50,51,46,53,56,50,32,50,52,51,44,49,50,56,32,67,50,52,51,44,49,51,50,46,52,49,56,32,50,52,54,46,55,54,49,49,44,49,51,54,32,50,53,49,46,52,44,49,51,54,32,67,50,53,54,46,48,51,56,57,44,49,51,54,32,50,53,
57,46,56,44,49,51,50,46,52,49,56,32,50,53,57,46,56,44,49,50,56,32,67,50,53,57,46,56,44,49,50,51,46,53,56,50,32,50,53,54,46,48,51,56,57,44,49,50,48,32,50,53,49,46,52,44,49,50,48,32,76,50,53,49,46,52,44,
49,50,48,32,90,34,32,105,100,61,34,115,101,97,114,99,104,95,108,101,102,116,45,91,35,49,53,48,52,93,34,62,60,47,112,97,116,104,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,32,
32,32,32,60,47,103,62,10,32,32,32,32,60,47,103,62,10,60,47,115,118,103,62,0,0};

const char* VASTWaveTableEditorComponent::zero_find_svg = (const char*) resource_VASTWaveTableEditorComponent_zero_find_svg;
const int VASTWaveTableEditorComponent::zero_find_svgSize = 1166;

// JUCER_RESOURCE: rampconnect_svg, 6750, "../../Resources/Icons/rampconnect.svg"
static const unsigned char resource_VASTWaveTableEditorComponent_rampconnect_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,
70,45,56,34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,33,45,45,32,67,114,101,97,116,101,100,32,119,105,116,104,32,73,110,107,115,99,97,112,101,32,40,104,116,116,112,58,47,47,
119,119,119,46,105,110,107,115,99,97,112,101,46,111,114,103,47,41,32,45,45,62,10,10,60,115,118,103,10,32,32,32,120,109,108,110,115,58,100,99,61,34,104,116,116,112,58,47,47,112,117,114,108,46,111,114,103,
47,100,99,47,101,108,101,109,101,110,116,115,47,49,46,49,47,34,10,32,32,32,120,109,108,110,115,58,99,99,61,34,104,116,116,112,58,47,47,99,114,101,97,116,105,118,101,99,111,109,109,111,110,115,46,111,114,
103,47,110,115,35,34,10,32,32,32,120,109,108,110,115,58,114,100,102,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,48,50,47,50,50,45,114,100,102,45,115,121,110,116,
97,120,45,110,115,35,34,10,32,32,32,120,109,108,110,115,58,115,118,103,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,118,103,34,10,32,32,32,120,109,108,110,
115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,118,103,34,10,32,32,32,120,109,108,110,115,58,115,111,100,105,112,111,100,105,61,34,104,116,116,112,58,47,
47,115,111,100,105,112,111,100,105,46,115,111,117,114,99,101,102,111,114,103,101,46,110,101,116,47,68,84,68,47,115,111,100,105,112,111,100,105,45,48,46,100,116,100,34,10,32,32,32,120,109,108,110,115,58,
105,110,107,115,99,97,112,101,61,34,104,116,116,112,58,47,47,119,119,119,46,105,110,107,115,99,97,112,101,46,111,114,103,47,110,97,109,101,115,112,97,99,101,115,47,105,110,107,115,99,97,112,101,34,10,
32,32,32,119,105,100,116,104,61,34,50,49,48,109,109,34,10,32,32,32,104,101,105,103,104,116,61,34,50,57,55,109,109,34,10,32,32,32,118,105,101,119,66,111,120,61,34,48,32,48,32,50,49,48,32,50,57,55,34,10,
32,32,32,118,101,114,115,105,111,110,61,34,49,46,49,34,10,32,32,32,105,100,61,34,115,118,103,56,34,10,32,32,32,105,110,107,115,99,97,112,101,58,118,101,114,115,105,111,110,61,34,48,46,57,50,46,51,32,40,
50,52,48,53,53,52,54,44,32,50,48,49,56,45,48,51,45,49,49,41,34,10,32,32,32,115,111,100,105,112,111,100,105,58,100,111,99,110,97,109,101,61,34,114,97,109,112,99,111,110,110,101,99,116,46,115,118,103,34,
62,10,32,32,60,100,101,102,115,10,32,32,32,32,32,105,100,61,34,100,101,102,115,50,34,62,10,32,32,32,32,60,109,97,114,107,101,114,10,32,32,32,32,32,32,32,105,110,107,115,99,97,112,101,58,105,115,115,116,
111,99,107,61,34,116,114,117,101,34,10,32,32,32,32,32,32,32,115,116,121,108,101,61,34,111,118,101,114,102,108,111,119,58,118,105,115,105,98,108,101,34,10,32,32,32,32,32,32,32,105,100,61,34,109,97,114,
107,101,114,49,49,54,49,34,10,32,32,32,32,32,32,32,114,101,102,88,61,34,48,46,48,34,10,32,32,32,32,32,32,32,114,101,102,89,61,34,48,46,48,34,10,32,32,32,32,32,32,32,111,114,105,101,110,116,61,34,97,117,
116,111,34,10,32,32,32,32,32,32,32,105,110,107,115,99,97,112,101,58,115,116,111,99,107,105,100,61,34,65,114,114,111,119,49,76,115,116,97,114,116,34,62,10,32,32,32,32,32,32,60,112,97,116,104,10,32,32,32,
32,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,115,99,97,108,101,40,48,46,56,41,32,116,114,97,110,115,108,97,116,101,40,49,50,46,53,44,48,41,34,10,32,32,32,32,32,32,32,32,32,115,116,121,108,
101,61,34,102,105,108,108,45,114,117,108,101,58,101,118,101,110,111,100,100,59,115,116,114,111,107,101,58,35,49,57,49,57,49,57,59,115,116,114,111,107,101,45,119,105,100,116,104,58,49,112,116,59,115,116,
114,111,107,101,45,111,112,97,99,105,116,121,58,49,59,102,105,108,108,58,35,49,57,49,57,49,57,59,102,105,108,108,45,111,112,97,99,105,116,121,58,49,34,10,32,32,32,32,32,32,32,32,32,100,61,34,77,32,48,
46,48,44,48,46,48,32,76,32,53,46,48,44,45,53,46,48,32,76,32,45,49,50,46,53,44,48,46,48,32,76,32,53,46,48,44,53,46,48,32,76,32,48,46,48,44,48,46,48,32,122,32,34,10,32,32,32,32,32,32,32,32,32,105,100,61,
34,112,97,116,104,49,49,53,57,34,32,47,62,10,32,32,32,32,60,47,109,97,114,107,101,114,62,10,32,32,32,32,60,109,97,114,107,101,114,10,32,32,32,32,32,32,32,105,110,107,115,99,97,112,101,58,115,116,111,99,
107,105,100,61,34,65,114,114,111,119,49,76,115,116,97,114,116,34,10,32,32,32,32,32,32,32,111,114,105,101,110,116,61,34,97,117,116,111,34,10,32,32,32,32,32,32,32,114,101,102,89,61,34,48,46,48,34,10,32,
32,32,32,32,32,32,114,101,102,88,61,34,48,46,48,34,10,32,32,32,32,32,32,32,105,100,61,34,65,114,114,111,119,49,76,115,116,97,114,116,34,10,32,32,32,32,32,32,32,115,116,121,108,101,61,34,111,118,101,114,
102,108,111,119,58,118,105,115,105,98,108,101,34,10,32,32,32,32,32,32,32,105,110,107,115,99,97,112,101,58,105,115,115,116,111,99,107,61,34,116,114,117,101,34,10,32,32,32,32,32,32,32,105,110,107,115,99,
97,112,101,58,99,111,108,108,101,99,116,61,34,97,108,119,97,121,115,34,62,10,32,32,32,32,32,32,60,112,97,116,104,10,32,32,32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,56,53,49,34,10,32,32,32,32,32,
32,32,32,32,100,61,34,77,32,48,46,48,44,48,46,48,32,76,32,53,46,48,44,45,53,46,48,32,76,32,45,49,50,46,53,44,48,46,48,32,76,32,53,46,48,44,53,46,48,32,76,32,48,46,48,44,48,46,48,32,122,32,34,10,32,32,
32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,45,114,117,108,101,58,101,118,101,110,111,100,100,59,115,116,114,111,107,101,58,35,49,57,49,57,49,57,59,115,116,114,111,107,101,45,119,105,
100,116,104,58,49,112,116,59,115,116,114,111,107,101,45,111,112,97,99,105,116,121,58,49,59,102,105,108,108,58,35,49,57,49,57,49,57,59,102,105,108,108,45,111,112,97,99,105,116,121,58,49,34,10,32,32,32,
32,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,115,99,97,108,101,40,48,46,56,41,32,116,114,97,110,115,108,97,116,101,40,49,50,46,53,44,48,41,34,32,47,62,10,32,32,32,32,60,47,109,97,114,107,
101,114,62,10,32,32,32,32,60,102,105,108,116,101,114,10,32,32,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,111,108,108,101,99,116,61,34,97,108,119,97,121,115,34,10,32,32,32,32,32,32,32,115,116,121,
108,101,61,34,99,111,108,111,114,45,105,110,116,101,114,112,111,108,97,116,105,111,110,45,102,105,108,116,101,114,115,58,115,82,71,66,34,10,32,32,32,32,32,32,32,105,100,61,34,102,105,108,116,101,114,49,
50,53,55,34,10,32,32,32,32,32,32,32,120,61,34,45,49,46,49,52,56,51,57,50,56,34,10,32,32,32,32,32,32,32,119,105,100,116,104,61,34,51,46,50,57,54,55,56,53,54,34,10,32,32,32,32,32,32,32,121,61,34,45,49,46,
51,51,52,54,50,50,51,34,10,32,32,32,32,32,32,32,104,101,105,103,104,116,61,34,51,46,54,54,57,50,52,52,55,34,62,10,32,32,32,32,32,32,60,102,101,71,97,117,115,115,105,97,110,66,108,117,114,10,32,32,32,32,
32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,111,108,108,101,99,116,61,34,97,108,119,97,121,115,34,10,32,32,32,32,32,32,32,32,32,115,116,100,68,101,118,105,97,116,105,111,110,61,34,51,52,46,57,57,
54,54,51,57,34,10,32,32,32,32,32,32,32,32,32,105,100,61,34,102,101,71,97,117,115,115,105,97,110,66,108,117,114,49,50,53,57,34,32,47,62,10,32,32,32,32,60,47,102,105,108,116,101,114,62,10,32,32,32,32,60,
102,105,108,116,101,114,10,32,32,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,111,108,108,101,99,116,61,34,97,108,119,97,121,115,34,10,32,32,32,32,32,32,32,115,116,121,108,101,61,34,99,111,108,111,
114,45,105,110,116,101,114,112,111,108,97,116,105,111,110,45,102,105,108,116,101,114,115,58,115,82,71,66,34,10,32,32,32,32,32,32,32,105,100,61,34,102,105,108,116,101,114,49,50,54,49,34,10,32,32,32,32,
32,32,32,120,61,34,45,48,46,53,54,49,56,53,56,49,49,34,10,32,32,32,32,32,32,32,119,105,100,116,104,61,34,50,46,49,50,51,55,49,54,50,34,10,32,32,32,32,32,32,32,121,61,34,45,49,46,51,51,52,54,50,50,51,34,
10,32,32,32,32,32,32,32,104,101,105,103,104,116,61,34,51,46,54,54,57,50,52,52,54,34,62,10,32,32,32,32,32,32,60,102,101,71,97,117,115,115,105,97,110,66,108,117,114,10,32,32,32,32,32,32,32,32,32,105,110,
107,115,99,97,112,101,58,99,111,108,108,101,99,116,61,34,97,108,119,97,121,115,34,10,32,32,32,32,32,32,32,32,32,115,116,100,68,101,118,105,97,116,105,111,110,61,34,49,51,50,46,50,55,48,55,54,34,10,32,
32,32,32,32,32,32,32,32,105,100,61,34,102,101,71,97,117,115,115,105,97,110,66,108,117,114,49,50,54,51,34,32,47,62,10,32,32,32,32,60,47,102,105,108,116,101,114,62,10,32,32,60,47,100,101,102,115,62,10,32,
32,60,115,111,100,105,112,111,100,105,58,110,97,109,101,100,118,105,101,119,10,32,32,32,32,32,105,100,61,34,98,97,115,101,34,10,32,32,32,32,32,112,97,103,101,99,111,108,111,114,61,34,35,102,102,102,102,
102,102,34,10,32,32,32,32,32,98,111,114,100,101,114,99,111,108,111,114,61,34,35,54,54,54,54,54,54,34,10,32,32,32,32,32,98,111,114,100,101,114,111,112,97,99,105,116,121,61,34,49,46,48,34,10,32,32,32,32,
32,105,110,107,115,99,97,112,101,58,112,97,103,101,111,112,97,99,105,116,121,61,34,48,46,48,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,112,97,103,101,115,104,97,100,111,119,61,34,50,34,10,32,
32,32,32,32,105,110,107,115,99,97,112,101,58,122,111,111,109,61,34,49,46,52,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,120,61,34,50,57,53,46,53,56,52,57,51,34,10,32,32,32,32,32,105,110,107,
115,99,97,112,101,58,99,121,61,34,53,50,55,46,56,56,49,52,49,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,100,111,99,117,109,101,110,116,45,117,110,105,116,115,61,34,109,109,34,10,32,32,32,32,
32,105,110,107,115,99,97,112,101,58,99,117,114,114,101,110,116,45,108,97,121,101,114,61,34,108,97,121,101,114,49,34,10,32,32,32,32,32,115,104,111,119,103,114,105,100,61,34,102,97,108,115,101,34,10,32,
32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,119,105,100,116,104,61,34,51,56,52,48,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,104,101,105,
103,104,116,61,34,50,48,53,48,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,120,61,34,45,49,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,
111,119,45,121,61,34,45,49,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,109,97,120,105,109,105,122,101,100,61,34,49,34,32,47,62,10,32,32,60,109,101,116,97,100,97,
116,97,10,32,32,32,32,32,105,100,61,34,109,101,116,97,100,97,116,97,53,34,62,10,32,32,32,32,60,114,100,102,58,82,68,70,62,10,32,32,32,32,32,32,60,99,99,58,87,111,114,107,10,32,32,32,32,32,32,32,32,32,
114,100,102,58,97,98,111,117,116,61,34,34,62,10,32,32,32,32,32,32,32,32,60,100,99,58,102,111,114,109,97,116,62,105,109,97,103,101,47,115,118,103,43,120,109,108,60,47,100,99,58,102,111,114,109,97,116,62,
10,32,32,32,32,32,32,32,32,60,100,99,58,116,121,112,101,10,32,32,32,32,32,32,32,32,32,32,32,114,100,102,58,114,101,115,111,117,114,99,101,61,34,104,116,116,112,58,47,47,112,117,114,108,46,111,114,103,
47,100,99,47,100,99,109,105,116,121,112,101,47,83,116,105,108,108,73,109,97,103,101,34,32,47,62,10,32,32,32,32,32,32,32,32,60,100,99,58,116,105,116,108,101,62,60,47,100,99,58,116,105,116,108,101,62,10,
32,32,32,32,32,32,60,47,99,99,58,87,111,114,107,62,10,32,32,32,32,60,47,114,100,102,58,82,68,70,62,10,32,32,60,47,109,101,116,97,100,97,116,97,62,10,32,32,60,103,10,32,32,32,32,32,105,110,107,115,99,97,
112,101,58,108,97,98,101,108,61,34,69,98,101,110,101,32,49,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,103,114,111,117,112,109,111,100,101,61,34,108,97,121,101,114,34,10,32,32,32,32,32,105,100,
61,34,108,97,121,101,114,49,34,62,10,32,32,32,32,60,112,97,116,104,10,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,110,111,110,101,59,115,116,114,111,107,101,58,35,49,57,49,57,49,
57,59,115,116,114,111,107,101,45,119,105,100,116,104,58,48,46,48,49,48,48,49,53,55,53,59,115,116,114,111,107,101,45,108,105,110,101,99,97,112,58,98,117,116,116,59,115,116,114,111,107,101,45,108,105,110,
101,106,111,105,110,58,109,105,116,101,114,59,115,116,114,111,107,101,45,111,112,97,99,105,116,121,58,49,59,115,116,114,111,107,101,45,109,105,116,101,114,108,105,109,105,116,58,52,59,115,116,114,111,
107,101,45,100,97,115,104,97,114,114,97,121,58,48,46,48,49,48,48,49,53,55,53,44,32,48,46,48,49,48,48,49,53,55,53,48,48,48,48,48,48,48,48,48,59,115,116,114,111,107,101,45,100,97,115,104,111,102,102,115,
101,116,58,48,46,49,48,48,49,53,55,53,59,109,97,114,107,101,114,45,115,116,97,114,116,58,117,114,108,40,35,65,114,114,111,119,49,76,115,116,97,114,116,41,59,102,105,108,116,101,114,58,117,114,108,40,35,
102,105,108,116,101,114,49,50,54,49,41,34,10,32,32,32,32,32,32,32,100,61,34,77,32,52,54,50,46,56,53,55,52,50,32,52,53,48,46,51,55,54,57,53,32,76,32,54,49,52,46,50,56,53,49,54,32,52,53,48,46,51,55,54,57,
53,32,76,32,52,54,50,46,56,53,55,52,50,32,52,53,48,46,51,55,54,57,53,32,122,32,77,32,52,57,46,50,56,53,49,53,54,32,54,56,56,46,50,51,52,51,56,32,76,32,49,56,54,46,52,50,55,55,51,32,54,56,56,46,50,51,52,
51,56,32,76,32,52,57,46,50,56,53,49,53,54,32,54,56,56,46,50,51,52,51,56,32,122,32,34,10,32,32,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,115,99,97,108,101,40,48,46,50,54,52,53,56,51,51,51,
41,34,10,32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,56,49,53,34,32,47,62,10,32,32,32,32,60,112,97,116,104,10,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,110,111,110,101,59,
115,116,114,111,107,101,58,35,49,57,49,57,49,57,59,115,116,114,111,107,101,45,119,105,100,116,104,58,48,59,115,116,114,111,107,101,45,108,105,110,101,99,97,112,58,98,117,116,116,59,115,116,114,111,107,
101,45,108,105,110,101,106,111,105,110,58,109,105,116,101,114,59,115,116,114,111,107,101,45,111,112,97,99,105,116,121,58,49,59,115,116,114,111,107,101,45,109,105,116,101,114,108,105,109,105,116,58,52,
59,115,116,114,111,107,101,45,100,97,115,104,97,114,114,97,121,58,48,44,32,48,59,115,116,114,111,107,101,45,100,97,115,104,111,102,102,115,101,116,58,48,59,109,97,114,107,101,114,45,115,116,97,114,116,
58,117,114,108,40,35,109,97,114,107,101,114,49,49,54,49,41,59,102,105,108,116,101,114,58,117,114,108,40,35,102,105,108,116,101,114,49,50,53,55,41,34,10,32,32,32,32,32,32,32,100,61,34,109,32,52,57,46,51,
50,53,54,55,44,49,56,50,46,48,57,53,51,52,32,99,32,55,51,46,49,51,56,54,56,44,45,54,50,46,57,51,51,49,49,32,55,51,46,49,51,56,54,56,44,45,54,50,46,57,51,51,49,49,32,55,51,46,49,51,56,54,56,44,45,54,50,
46,57,51,51,49,49,34,10,32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,56,51,54,34,10,32,32,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,111,110,110,101,99,116,111,114,45,99,117,114,118,97,116,
117,114,101,61,34,48,34,32,47,62,10,32,32,32,32,60,112,97,116,104,10,32,32,32,32,32,32,32,115,116,121,108,101,61,34,99,111,108,111,114,58,35,48,48,48,48,48,48,59,102,111,110,116,45,115,116,121,108,101,
58,110,111,114,109,97,108,59,102,111,110,116,45,118,97,114,105,97,110,116,58,110,111,114,109,97,108,59,102,111,110,116,45,119,101,105,103,104,116,58,110,111,114,109,97,108,59,102,111,110,116,45,115,116,
114,101,116,99,104,58,110,111,114,109,97,108,59,102,111,110,116,45,115,105,122,101,58,109,101,100,105,117,109,59,108,105,110,101,45,104,101,105,103,104,116,58,110,111,114,109,97,108,59,102,111,110,116,
45,102,97,109,105,108,121,58,115,97,110,115,45,115,101,114,105,102,59,102,111,110,116,45,118,97,114,105,97,110,116,45,108,105,103,97,116,117,114,101,115,58,110,111,114,109,97,108,59,102,111,110,116,45,
118,97,114,105,97,110,116,45,112,111,115,105,116,105,111,110,58,110,111,114,109,97,108,59,102,111,110,116,45,118,97,114,105,97,110,116,45,99,97,112,115,58,110,111,114,109,97,108,59,102,111,110,116,45,
118,97,114,105,97,110,116,45,110,117,109,101,114,105,99,58,110,111,114,109,97,108,59,102,111,110,116,45,118,97,114,105,97,110,116,45,97,108,116,101,114,110,97,116,101,115,58,110,111,114,109,97,108,59,
102,111,110,116,45,102,101,97,116,117,114,101,45,115,101,116,116,105,110,103,115,58,110,111,114,109,97,108,59,116,101,120,116,45,105,110,100,101,110,116,58,48,59,116,101,120,116,45,97,108,105,103,110,
58,115,116,97,114,116,59,116,101,120,116,45,100,101,99,111,114,97,116,105,111,110,58,110,111,110,101,59,116,101,120,116,45,100,101,99,111,114,97,116,105,111,110,45,108,105,110,101,58,110,111,110,101,59,
116,101,120,116,45,100,101,99,111,114,97,116,105,111,110,45,115,116,121,108,101,58,115,111,108,105,100,59,116,101,120,116,45,100,101,99,111,114,97,116,105,111,110,45,99,111,108,111,114,58,35,48,48,48,
48,48,48,59,108,101,116,116,101,114,45,115,112,97,99,105,110,103,58,110,111,114,109,97,108,59,119,111,114,100,45,115,112,97,99,105,110,103,58,110,111,114,109,97,108,59,116,101,120,116,45,116,114,97,110,
115,102,111,114,109,58,110,111,110,101,59,119,114,105,116,105,110,103,45,109,111,100,101,58,108,114,45,116,98,59,100,105,114,101,99,116,105,111,110,58,108,116,114,59,116,101,120,116,45,111,114,105,101,
110,116,97,116,105,111,110,58,109,105,120,101,100,59,100,111,109,105,110,97,110,116,45,98,97,115,101,108,105,110,101,58,97,117,116,111,59,98,97,115,101,108,105,110,101,45,115,104,105,102,116,58,98,97,
115,101,108,105,110,101,59,116,101,120,116,45,97,110,99,104,111,114,58,115,116,97,114,116,59,119,104,105,116,101,45,115,112,97,99,101,58,110,111,114,109,97,108,59,115,104,97,112,101,45,112,97,100,100,
105,110,103,58,48,59,99,108,105,112,45,114,117,108,101,58,110,111,110,122,101,114,111,59,100,105,115,112,108,97,121,58,105,110,108,105,110,101,59,111,118,101,114,102,108,111,119,58,118,105,115,105,98,
108,101,59,118,105,115,105,98,105,108,105,116,121,58,118,105,115,105,98,108,101,59,111,112,97,99,105,116,121,58,49,59,105,115,111,108,97,116,105,111,110,58,97,117,116,111,59,109,105,120,45,98,108,101,
110,100,45,109,111,100,101,58,110,111,114,109,97,108,59,99,111,108,111,114,45,105,110,116,101,114,112,111,108,97,116,105,111,110,58,115,82,71,66,59,99,111,108,111,114,45,105,110,116,101,114,112,111,108,
97,116,105,111,110,45,102,105,108,116,101,114,115,58,108,105,110,101,97,114,82,71,66,59,115,111,108,105,100,45,99,111,108,111,114,58,35,48,48,48,48,48,48,59,115,111,108,105,100,45,111,112,97,99,105,116,
121,58,49,59,118,101,99,116,111,114,45,101,102,102,101,99,116,58,110,111,110,101,59,102,105,108,108,58,35,102,102,102,102,102,102,59,102,105,108,108,45,111,112,97,99,105,116,121,58,49,59,102,105,108,108,
45,114,117,108,101,58,110,111,110,122,101,114,111,59,115,116,114,111,107,101,58,110,111,110,101,59,115,116,114,111,107,101,45,119,105,100,116,104,58,48,46,50,54,52,53,56,51,51,50,112,120,59,115,116,114,
111,107,101,45,108,105,110,101,99,97,112,58,98,117,116,116,59,115,116,114,111,107,101,45,108,105,110,101,106,111,105,110,58,109,105,116,101,114,59,115,116,114,111,107,101,45,109,105,116,101,114,108,105,
109,105,116,58,52,59,115,116,114,111,107,101,45,100,97,115,104,97,114,114,97,121,58,110,111,110,101,59,115,116,114,111,107,101,45,100,97,115,104,111,102,102,115,101,116,58,48,59,115,116,114,111,107,101,
45,111,112,97,99,105,116,121,58,49,59,99,111,108,111,114,45,114,101,110,100,101,114,105,110,103,58,97,117,116,111,59,105,109,97,103,101,45,114,101,110,100,101,114,105,110,103,58,97,117,116,111,59,115,
104,97,112,101,45,114,101,110,100,101,114,105,110,103,58,97,117,116,111,59,116,101,120,116,45,114,101,110,100,101,114,105,110,103,58,97,117,116,111,59,101,110,97,98,108,101,45,98,97,99,107,103,114,111,
117,110,100,58,97,99,99,117,109,117,108,97,116,101,34,10,32,32,32,32,32,32,32,100,61,34,109,32,49,54,54,46,53,55,50,50,55,44,55,54,46,54,55,49,56,55,53,32,99,32,45,49,52,46,53,48,52,57,44,49,46,53,54,
57,50,54,49,32,45,50,57,46,56,57,49,50,50,44,45,49,46,54,49,54,50,49,50,32,45,52,52,46,51,55,53,44,49,46,52,57,54,48,57,52,32,45,51,46,51,57,53,50,51,44,49,46,51,56,52,55,52,32,45,49,46,55,57,54,52,57,
44,53,46,56,55,52,51,54,49,32,45,51,46,51,52,51,55,53,44,55,46,57,55,54,53,54,50,32,45,52,46,53,56,52,49,54,44,49,46,51,54,49,56,54,53,32,45,55,46,49,56,57,57,44,54,46,55,51,48,55,53,53,32,45,55,46,48,
52,50,57,55,44,49,48,46,56,50,50,50,54,54,32,45,55,46,57,52,51,53,44,52,46,51,54,53,52,49,51,32,45,49,50,46,57,53,48,50,56,55,44,49,50,46,50,55,50,50,57,51,32,45,49,57,46,52,54,54,56,44,49,56,46,51,49,
56,51,54,51,32,45,48,46,48,56,55,54,53,44,55,46,56,50,54,54,51,32,49,49,46,51,49,56,50,56,44,49,48,46,49,54,51,51,50,32,49,53,46,48,55,48,51,49,44,51,46,55,54,57,53,51,32,53,46,48,56,52,44,45,54,46,48,
49,48,48,53,32,49,49,46,54,56,51,57,56,44,45,49,48,46,56,49,49,57,50,32,49,53,46,52,49,48,49,54,44,45,49,55,46,57,48,56,50,49,32,45,50,46,54,56,50,48,56,44,45,48,46,55,55,49,48,57,32,50,46,54,53,55,51,
52,44,45,50,46,48,57,49,55,50,53,32,51,46,50,51,48,52,55,44,45,51,46,54,57,49,52,48,50,32,51,46,52,54,49,53,57,44,45,52,46,48,51,56,54,53,52,32,55,46,55,48,51,53,54,44,45,56,46,54,54,52,56,52,57,32,49,
51,46,54,49,57,49,51,44,45,55,46,50,55,52,53,52,54,32,57,46,51,53,49,54,50,44,48,46,52,57,55,55,51,56,32,49,56,46,55,56,49,57,56,44,48,46,48,51,55,56,55,32,50,56,46,49,50,54,57,54,44,48,46,51,49,55,53,
49,53,32,53,46,57,54,56,50,57,44,48,46,48,49,54,32,56,46,53,49,57,55,52,44,45,55,46,57,55,48,55,50,50,32,51,46,56,56,50,56,49,44,45,49,49,46,54,48,51,53,49,54,32,45,49,46,51,53,56,55,49,44,45,49,46,51,
50,53,52,51,56,32,45,51,46,50,48,50,57,53,44,45,50,46,49,55,55,56,49,54,32,45,53,46,49,49,49,51,50,44,45,50,46,50,50,50,54,53,54,32,122,32,77,32,56,51,46,49,49,49,51,50,56,44,49,50,55,46,53,54,50,53,32,
99,32,45,56,46,57,52,56,56,57,53,44,50,46,51,52,54,56,57,32,45,49,51,46,50,56,54,55,56,44,49,50,46,48,48,55,51,57,32,45,50,48,46,51,54,51,50,56,49,44,49,55,46,50,56,55,49,49,32,48,46,50,48,51,53,51,50,
44,56,46,51,56,48,52,51,32,49,50,46,56,49,51,49,54,49,44,49,48,46,49,55,55,57,52,32,49,54,46,48,54,52,52,53,51,44,50,46,56,55,54,57,53,32,52,46,57,53,56,53,54,51,44,45,53,46,49,48,53,48,56,32,49,48,46,
48,57,52,50,50,53,44,45,49,48,46,48,57,53,51,50,32,49,53,46,48,54,50,53,44,45,49,53,46,50,49,52,56,52,32,45,52,46,53,49,49,49,48,55,44,48,46,48,51,54,49,32,45,54,46,52,56,52,50,51,52,44,45,52,46,56,50,
57,49,55,32,45,49,48,46,55,54,51,54,55,50,44,45,52,46,57,52,57,50,50,32,122,32,109,32,45,51,48,46,49,57,57,50,49,57,44,51,49,46,50,57,56,56,51,32,99,32,45,56,46,52,55,57,51,51,54,44,49,46,51,57,49,54,
49,32,45,49,49,46,49,57,52,51,48,53,44,49,52,46,51,56,52,51,53,32,45,50,48,46,56,48,52,54,49,51,44,49,50,46,51,50,53,55,51,32,45,56,46,55,49,51,52,49,56,44,45,48,46,49,48,48,57,52,32,45,49,55,46,52,54,
53,57,56,55,44,45,48,46,53,49,57,54,32,45,50,54,46,49,52,50,54,53,50,50,44,48,46,53,50,57,55,52,32,45,52,46,56,48,50,54,57,51,57,44,54,46,52,52,52,54,56,32,50,46,57,48,50,53,55,49,57,44,49,53,46,50,53,
52,54,55,32,49,48,46,48,50,53,51,57,48,50,44,49,50,46,56,54,51,50,56,32,49,48,46,55,56,50,55,48,50,44,45,49,46,55,48,51,55,57,32,50,52,46,52,54,51,53,56,44,51,46,56,48,54,57,55,32,51,50,46,57,52,51,51,
54,44,45,53,46,53,56,50,48,51,32,53,46,48,57,57,56,56,54,44,45,52,46,56,48,56,53,50,32,49,48,46,50,54,56,56,57,49,44,45,57,46,54,56,56,51,57,32,49,51,46,57,49,48,49,53,54,44,45,49,53,46,55,53,32,45,51,
46,56,55,53,52,48,57,44,45,48,46,51,48,53,54,57,32,45,54,46,48,49,52,54,55,50,44,45,52,46,53,49,49,48,54,32,45,57,46,57,51,49,54,52,49,44,45,52,46,51,56,54,55,50,32,122,34,10,32,32,32,32,32,32,32,105,
100,61,34,112,97,116,104,49,50,54,53,34,10,32,32,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,111,110,110,101,99,116,111,114,45,99,117,114,118,97,116,117,114,101,61,34,48,34,32,47,62,10,32,32,60,
47,103,62,10,60,47,115,118,103,62,10,0,0};

const char* VASTWaveTableEditorComponent::rampconnect_svg = (const char*) resource_VASTWaveTableEditorComponent_rampconnect_svg;
const int VASTWaveTableEditorComponent::rampconnect_svgSize = 6750;

// JUCER_RESOURCE: apply_svg, 1969, "../../Resources/Icons/apply.svg"
static const unsigned char resource_VASTWaveTableEditorComponent_apply_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,70,45,
56,34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,32,119,105,100,116,104,61,34,50,48,112,120,34,32,104,101,105,103,104,116,61,34,49,55,112,120,34,32,118,105,101,119,
66,111,120,61,34,48,32,48,32,50,48,32,49,55,34,32,118,101,114,115,105,111,110,61,34,49,46,49,34,32,120,109,108,110,115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,
47,115,118,103,34,32,120,109,108,110,115,58,120,108,105,110,107,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,120,108,105,110,107,34,62,10,32,32,32,32,60,33,45,
45,32,71,101,110,101,114,97,116,111,114,58,32,83,107,101,116,99,104,32,51,46,56,46,49,32,40,50,57,54,56,55,41,32,45,32,104,116,116,112,58,47,47,119,119,119,46,98,111,104,101,109,105,97,110,99,111,100,
105,110,103,46,99,111,109,47,115,107,101,116,99,104,32,45,45,62,10,32,32,32,32,60,116,105,116,108,101,62,115,107,101,116,99,104,32,91,35,49,50,53,93,60,47,116,105,116,108,101,62,10,32,32,32,32,60,100,
101,115,99,62,67,114,101,97,116,101,100,32,119,105,116,104,32,83,107,101,116,99,104,46,60,47,100,101,115,99,62,10,32,32,32,32,60,100,101,102,115,62,60,47,100,101,102,115,62,10,32,32,32,32,60,103,32,105,
100,61,34,80,97,103,101,45,49,34,32,115,116,114,111,107,101,61,34,110,111,110,101,34,32,115,116,114,111,107,101,45,119,105,100,116,104,61,34,49,34,32,102,105,108,108,61,34,110,111,110,101,34,32,102,105,
108,108,45,114,117,108,101,61,34,101,118,101,110,111,100,100,34,62,10,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,68,114,105,98,98,98,108,101,45,68,97,114,107,45,80,114,101,118,105,101,119,34,32,116,
114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,45,52,50,48,46,48,48,48,48,48,48,44,32,45,55,54,48,49,46,48,48,48,48,48,48,41,34,32,102,105,108,108,61,34,35,70,70,70,70,70,70,
34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,105,99,111,110,115,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,53,54,46,48,48,48,48,48,48,44,
32,49,54,48,46,48,48,48,48,48,48,41,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,60,112,97,116,104,32,100,61,34,77,51,54,56,46,50,51,56,51,50,53,44,55,52,52,51,46,51,52,57,54,57,32,76,51,54,
57,46,51,57,57,51,50,53,44,55,52,52,54,46,49,54,51,57,55,32,76,51,54,54,46,55,51,51,51,50,53,44,55,52,52,54,46,49,54,51,57,55,32,76,51,54,56,46,50,51,56,51,50,53,44,55,52,52,51,46,51,52,57,54,57,32,90,
32,77,51,55,55,46,53,56,52,51,50,53,44,55,52,52,56,46,50,50,52,50,32,76,51,56,48,46,53,55,55,51,50,53,44,55,52,52,56,46,50,50,52,50,32,76,51,55,53,46,48,57,56,51,50,53,44,55,52,53,52,46,50,52,56,51,50,
32,76,51,55,55,46,53,56,52,51,50,53,44,55,52,52,56,46,50,50,52,50,32,90,32,77,51,55,50,46,55,52,49,51,50,53,44,55,52,53,52,46,50,54,50,55,52,32,76,51,54,55,46,49,48,48,51,50,53,44,55,52,52,56,46,50,50,
52,50,32,76,51,55,48,46,50,52,57,51,50,53,44,55,52,52,56,46,50,50,52,50,32,76,51,55,50,46,55,52,49,51,50,53,44,55,52,53,52,46,50,54,50,55,52,32,90,32,77,51,55,49,46,53,54,56,51,50,53,44,55,52,52,54,46,
49,54,51,57,55,32,76,51,55,48,46,50,57,51,51,50,53,44,55,52,52,51,46,48,55,51,54,50,32,76,51,55,55,46,53,52,48,51,50,53,44,55,52,52,51,46,48,55,51,54,50,32,76,51,55,54,46,50,54,53,51,50,53,44,55,52,52,
54,46,49,54,51,57,55,32,76,51,55,49,46,53,54,56,51,50,53,44,55,52,52,54,46,49,54,51,57,55,32,90,32,77,51,55,53,46,52,49,53,51,50,53,44,55,52,52,56,46,50,50,52,50,32,76,51,55,51,46,57,49,55,51,50,53,44,
55,52,53,49,46,56,53,53,51,54,32,76,51,55,50,46,52,49,56,51,50,53,44,55,52,52,56,46,50,50,52,50,32,76,51,55,53,46,52,49,53,51,50,53,44,55,52,52,56,46,50,50,52,50,32,90,32,77,51,56,49,46,50,54,48,51,50,
53,44,55,52,52,54,46,49,54,51,57,55,32,76,51,55,56,46,52,51,52,51,50,53,44,55,52,52,54,46,49,54,51,57,55,32,76,51,55,57,46,54,49,49,51,50,53,44,55,52,52,51,46,51,49,48,53,53,32,76,51,56,49,46,50,54,48,
51,50,53,44,55,52,52,54,46,49,54,51,57,55,32,90,32,77,51,56,51,46,55,56,55,51,50,53,44,55,52,52,54,46,52,51,53,57,50,32,76,51,56,48,46,57,51,51,51,50,53,44,55,52,52,49,46,50,54,55,56,51,32,67,51,56,48,
46,56,49,55,51,50,53,44,55,52,52,49,46,48,56,54,53,51,32,51,56,48,46,54,54,49,51,50,53,44,55,52,52,49,32,51,56,48,46,52,56,50,51,50,53,44,55,52,52,49,32,67,51,56,48,46,50,57,51,51,50,53,44,55,52,52,49,
32,51,56,48,46,48,55,55,51,50,53,44,55,52,52,49,46,48,49,51,51,57,32,51,55,57,46,56,53,51,51,50,53,44,55,52,52,49,46,48,49,51,51,57,32,76,51,54,56,46,49,56,56,51,50,53,44,55,52,52,49,46,48,49,51,51,57,
32,67,51,54,55,46,57,54,55,51,50,53,44,55,52,52,49,46,48,49,51,51,57,32,51,54,55,46,55,53,52,51,50,53,44,55,52,52,49,46,48,48,53,49,53,32,51,54,55,46,53,54,55,51,50,53,44,55,52,52,49,46,48,48,53,49,53,
32,67,51,54,55,46,51,55,55,51,50,53,44,55,52,52,49,46,48,48,53,49,53,32,51,54,55,46,50,49,51,51,50,53,44,55,52,52,49,46,48,55,51,49,52,32,51,54,55,46,48,57,52,51,50,53,44,55,52,52,49,46,50,54,55,56,51,
32,76,51,54,52,46,49,57,56,51,50,53,44,55,52,52,54,46,55,53,50,49,55,32,67,51,54,51,46,56,57,50,51,50,53,44,55,52,52,55,46,50,53,51,56,51,32,51,54,51,46,57,52,52,51,50,53,44,55,52,52,55,46,56,51,51,55,
57,32,51,54,52,46,51,50,55,51,50,53,44,55,52,52,56,46,50,55,53,55,49,32,76,51,55,51,46,48,50,49,51,50,53,44,55,52,53,55,46,53,53,52,57,57,32,67,51,55,51,46,50,55,55,51,50,53,44,55,52,53,55,46,56,53,49,
54,54,32,51,55,51,46,54,51,48,51,50,53,44,55,52,53,56,32,51,55,51,46,57,56,51,51,50,53,44,55,52,53,56,32,67,51,55,52,46,51,52,50,51,50,53,44,55,52,53,56,32,51,55,52,46,55,48,49,51,50,53,44,55,52,53,55,
46,56,52,56,53,55,32,51,55,52,46,57,53,57,51,50,53,44,55,52,53,55,46,53,52,51,54,54,32,76,51,56,51,46,54,56,49,51,50,53,44,55,52,52,56,46,48,49,53,48,57,32,67,51,56,52,46,48,54,51,51,50,53,44,55,52,52,
55,46,53,54,51,57,32,51,56,52,46,49,48,54,51,50,53,44,55,52,52,54,46,57,51,52,53,32,51,56,51,46,55,56,55,51,50,53,44,55,52,52,54,46,52,51,53,57,50,32,76,51,56,51,46,55,56,55,51,50,53,44,55,52,52,54,46,
52,51,53,57,50,32,90,34,32,105,100,61,34,115,107,101,116,99,104,45,91,35,49,50,53,93,34,62,60,47,112,97,116,104,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,32,32,32,32,60,47,
103,62,10,32,32,32,32,60,47,103,62,10,60,47,115,118,103,62,0,0};

const char* VASTWaveTableEditorComponent::apply_svg = (const char*) resource_VASTWaveTableEditorComponent_apply_svg;
const int VASTWaveTableEditorComponent::apply_svgSize = 1969;

// JUCER_RESOURCE: generate_svg, 1044, "../../Resources/Icons/generate.svg"
static const unsigned char resource_VASTWaveTableEditorComponent_generate_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,70,
45,56,34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,32,119,105,100,116,104,61,34,50,48,112,120,34,32,104,101,105,103,104,116,61,34,50,48,112,120,34,32,118,105,101,
119,66,111,120,61,34,48,32,48,32,50,48,32,50,48,34,32,118,101,114,115,105,111,110,61,34,49,46,49,34,32,120,109,108,110,115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,
48,48,47,115,118,103,34,32,120,109,108,110,115,58,120,108,105,110,107,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,120,108,105,110,107,34,62,10,32,32,32,32,60,
33,45,45,32,71,101,110,101,114,97,116,111,114,58,32,83,107,101,116,99,104,32,51,46,56,46,49,32,40,50,57,54,56,55,41,32,45,32,104,116,116,112,58,47,47,119,119,119,46,98,111,104,101,109,105,97,110,99,111,
100,105,110,103,46,99,111,109,47,115,107,101,116,99,104,32,45,45,62,10,32,32,32,32,60,116,105,116,108,101,62,119,97,110,100,32,91,35,55,50,49,93,60,47,116,105,116,108,101,62,10,32,32,32,32,60,100,101,
115,99,62,67,114,101,97,116,101,100,32,119,105,116,104,32,83,107,101,116,99,104,46,60,47,100,101,115,99,62,10,32,32,32,32,60,100,101,102,115,62,60,47,100,101,102,115,62,10,32,32,32,32,60,103,32,105,100,
61,34,80,97,103,101,45,49,34,32,115,116,114,111,107,101,61,34,110,111,110,101,34,32,115,116,114,111,107,101,45,119,105,100,116,104,61,34,49,34,32,102,105,108,108,61,34,110,111,110,101,34,32,102,105,108,
108,45,114,117,108,101,61,34,101,118,101,110,111,100,100,34,62,10,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,68,114,105,98,98,98,108,101,45,68,97,114,107,45,80,114,101,118,105,101,119,34,32,116,114,
97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,45,51,52,48,46,48,48,48,48,48,48,44,32,45,52,57,57,57,46,48,48,48,48,48,48,41,34,32,102,105,108,108,61,34,35,70,70,70,70,70,70,34,
62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,105,99,111,110,115,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,53,54,46,48,48,48,48,48,48,44,32,
49,54,48,46,48,48,48,48,48,48,41,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,60,112,111,108,121,103,111,110,32,105,100,61,34,119,97,110,100,45,91,35,55,50,49,93,34,32,112,111,105,110,116,
115,61,34,51,48,52,32,52,56,52,51,32,51,48,49,46,52,54,55,32,52,56,52,51,32,51,48,51,46,50,57,54,32,52,56,52,49,46,49,55,50,32,51,48,49,46,56,53,53,32,52,56,51,57,46,55,53,55,32,51,48,48,32,52,56,52,49,
46,53,56,54,32,51,48,48,32,52,56,51,57,32,50,57,56,32,52,56,51,57,32,50,57,56,32,52,56,52,49,46,53,56,54,32,50,57,54,46,49,57,56,32,52,56,51,57,46,55,53,55,32,50,57,52,46,55,57,55,32,52,56,52,49,46,49,
55,50,32,50,57,54,46,54,51,57,32,52,56,52,51,32,50,57,52,32,52,56,52,51,32,50,57,52,32,52,56,52,53,32,50,57,54,46,54,51,57,32,52,56,52,53,32,50,56,52,32,52,56,53,55,46,54,51,57,32,50,56,53,46,51,56,56,
32,52,56,53,57,46,48,53,51,32,50,57,56,32,52,56,52,54,46,52,49,52,32,50,57,56,32,52,56,52,57,32,51,48,48,32,52,56,52,57,32,51,48,48,32,52,56,52,54,46,52,49,52,32,51,48,49,46,56,53,53,32,52,56,52,56,46,
50,52,51,32,51,48,51,46,50,56,50,32,52,56,52,54,46,56,50,56,32,51,48,49,46,52,54,55,32,52,56,52,53,32,51,48,52,32,52,56,52,53,34,62,60,47,112,111,108,121,103,111,110,62,10,32,32,32,32,32,32,32,32,32,32,
32,32,60,47,103,62,10,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,60,47,103,62,10,60,47,115,118,103,62,0,0};

const char* VASTWaveTableEditorComponent::generate_svg = (const char*) resource_VASTWaveTableEditorComponent_generate_svg;
const int VASTWaveTableEditorComponent::generate_svgSize = 1044;

// JUCER_RESOURCE: waves_square_svg, 3000, "../../Resources/Icons/waves_square.svg"
static const unsigned char resource_VASTWaveTableEditorComponent_waves_square_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,
70,45,56,34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,10,32,32,32,120,109,108,110,115,58,100,99,61,34,104,116,116,112,58,47,47,112,117,114,108,46,111,114,103,47,
100,99,47,101,108,101,109,101,110,116,115,47,49,46,49,47,34,10,32,32,32,120,109,108,110,115,58,99,99,61,34,104,116,116,112,58,47,47,99,114,101,97,116,105,118,101,99,111,109,109,111,110,115,46,111,114,
103,47,110,115,35,34,10,32,32,32,120,109,108,110,115,58,114,100,102,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,48,50,47,50,50,45,114,100,102,45,115,121,110,116,
97,120,45,110,115,35,34,10,32,32,32,120,109,108,110,115,58,115,118,103,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,118,103,34,10,32,32,32,120,109,108,110,
115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,118,103,34,10,32,32,32,120,109,108,110,115,58,115,111,100,105,112,111,100,105,61,34,104,116,116,112,58,47,
47,115,111,100,105,112,111,100,105,46,115,111,117,114,99,101,102,111,114,103,101,46,110,101,116,47,68,84,68,47,115,111,100,105,112,111,100,105,45,48,46,100,116,100,34,10,32,32,32,120,109,108,110,115,58,
105,110,107,115,99,97,112,101,61,34,104,116,116,112,58,47,47,119,119,119,46,105,110,107,115,99,97,112,101,46,111,114,103,47,110,97,109,101,115,112,97,99,101,115,47,105,110,107,115,99,97,112,101,34,10,
32,32,32,119,105,100,116,104,61,34,50,48,112,120,34,10,32,32,32,104,101,105,103,104,116,61,34,50,48,112,120,34,10,32,32,32,118,105,101,119,66,111,120,61,34,48,32,48,32,50,48,32,50,48,34,10,32,32,32,118,
101,114,115,105,111,110,61,34,49,46,49,34,10,32,32,32,105,100,61,34,115,118,103,49,50,34,10,32,32,32,115,111,100,105,112,111,100,105,58,100,111,99,110,97,109,101,61,34,119,97,118,101,115,95,115,113,117,
97,114,101,46,115,118,103,34,10,32,32,32,105,110,107,115,99,97,112,101,58,118,101,114,115,105,111,110,61,34,48,46,57,50,46,51,32,40,50,52,48,53,53,52,54,44,32,50,48,49,56,45,48,51,45,49,49,41,34,62,10,
32,32,60,109,101,116,97,100,97,116,97,10,32,32,32,32,32,105,100,61,34,109,101,116,97,100,97,116,97,49,54,34,62,10,32,32,32,32,60,114,100,102,58,82,68,70,62,10,32,32,32,32,32,32,60,99,99,58,87,111,114,
107,10,32,32,32,32,32,32,32,32,32,114,100,102,58,97,98,111,117,116,61,34,34,62,10,32,32,32,32,32,32,32,32,60,100,99,58,102,111,114,109,97,116,62,105,109,97,103,101,47,115,118,103,43,120,109,108,60,47,
100,99,58,102,111,114,109,97,116,62,10,32,32,32,32,32,32,32,32,60,100,99,58,116,121,112,101,10,32,32,32,32,32,32,32,32,32,32,32,114,100,102,58,114,101,115,111,117,114,99,101,61,34,104,116,116,112,58,47,
47,112,117,114,108,46,111,114,103,47,100,99,47,100,99,109,105,116,121,112,101,47,83,116,105,108,108,73,109,97,103,101,34,32,47,62,10,32,32,32,32,32,32,32,32,60,100,99,58,116,105,116,108,101,62,115,116,
97,116,115,32,91,35,49,49,57,48,93,60,47,100,99,58,116,105,116,108,101,62,10,32,32,32,32,32,32,60,47,99,99,58,87,111,114,107,62,10,32,32,32,32,60,47,114,100,102,58,82,68,70,62,10,32,32,60,47,109,101,116,
97,100,97,116,97,62,10,32,32,60,115,111,100,105,112,111,100,105,58,110,97,109,101,100,118,105,101,119,10,32,32,32,32,32,112,97,103,101,99,111,108,111,114,61,34,35,57,53,57,53,57,53,34,10,32,32,32,32,32,
98,111,114,100,101,114,99,111,108,111,114,61,34,35,54,54,54,54,54,54,34,10,32,32,32,32,32,98,111,114,100,101,114,111,112,97,99,105,116,121,61,34,49,34,10,32,32,32,32,32,111,98,106,101,99,116,116,111,108,
101,114,97,110,99,101,61,34,49,48,34,10,32,32,32,32,32,103,114,105,100,116,111,108,101,114,97,110,99,101,61,34,49,48,34,10,32,32,32,32,32,103,117,105,100,101,116,111,108,101,114,97,110,99,101,61,34,49,
48,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,112,97,103,101,111,112,97,99,105,116,121,61,34,49,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,112,97,103,101,115,104,97,100,111,119,61,
34,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,119,105,100,116,104,61,34,51,56,52,48,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,
45,104,101,105,103,104,116,61,34,50,48,53,48,34,10,32,32,32,32,32,105,100,61,34,110,97,109,101,100,118,105,101,119,49,52,34,10,32,32,32,32,32,115,104,111,119,103,114,105,100,61,34,102,97,108,115,101,34,
10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,122,111,111,109,61,34,52,55,46,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,120,61,34,54,46,56,48,56,53,52,56,34,10,32,32,32,32,32,105,
110,107,115,99,97,112,101,58,99,121,61,34,54,46,52,53,57,53,57,56,51,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,120,61,34,45,49,50,34,10,32,32,32,32,32,105,110,107,
115,99,97,112,101,58,119,105,110,100,111,119,45,121,61,34,45,49,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,109,97,120,105,109,105,122,101,100,61,34,49,34,10,32,
32,32,32,32,105,110,107,115,99,97,112,101,58,99,117,114,114,101,110,116,45,108,97,121,101,114,61,34,115,118,103,49,50,34,32,47,62,10,32,32,60,33,45,45,32,71,101,110,101,114,97,116,111,114,58,32,83,107,
101,116,99,104,32,51,46,56,46,49,32,40,50,57,54,56,55,41,32,45,32,104,116,116,112,58,47,47,119,119,119,46,98,111,104,101,109,105,97,110,99,111,100,105,110,103,46,99,111,109,47,115,107,101,116,99,104,32,
45,45,62,10,32,32,60,116,105,116,108,101,10,32,32,32,32,32,105,100,61,34,116,105,116,108,101,50,34,62,115,116,97,116,115,32,91,35,49,49,57,48,93,60,47,116,105,116,108,101,62,10,32,32,60,100,101,115,99,
10,32,32,32,32,32,105,100,61,34,100,101,115,99,52,34,62,67,114,101,97,116,101,100,32,119,105,116,104,32,83,107,101,116,99,104,46,60,47,100,101,115,99,62,10,32,32,60,100,101,102,115,10,32,32,32,32,32,105,
100,61,34,100,101,102,115,54,34,32,47,62,10,32,32,60,103,10,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,45,49,52,48,44,45,50,56,51,57,41,34,10,32,32,32,
32,32,105,100,61,34,68,114,105,98,98,98,108,101,45,76,105,103,104,116,45,80,114,101,118,105,101,119,34,10,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,102,102,102,102,102,102,59,102,
105,108,108,45,114,117,108,101,58,101,118,101,110,111,100,100,59,115,116,114,111,107,101,58,110,111,110,101,59,115,116,114,111,107,101,45,119,105,100,116,104,58,49,59,102,105,108,108,45,111,112,97,99,
105,116,121,58,48,46,57,57,55,50,56,57,57,54,34,62,10,32,32,32,32,60,103,10,32,32,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,53,54,44,49,54,48,41,34,10,
32,32,32,32,32,32,32,105,100,61,34,105,99,111,110,115,34,10,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,102,102,102,102,102,102,59,102,105,108,108,45,111,112,97,99,105,116,121,
58,48,46,57,57,55,50,56,57,57,54,34,62,10,32,32,32,32,32,32,60,112,97,116,104,10,32,32,32,32,32,32,32,32,32,115,111,100,105,112,111,100,105,58,110,111,100,101,116,121,112,101,115,61,34,99,99,99,99,99,
99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,34,10,32,32,32,32,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,111,110,110,101,99,116,111,114,45,99,117,114,118,97,116,117,114,101,61,
34,48,34,10,32,32,32,32,32,32,32,32,32,105,100,61,34,115,116,97,116,115,45,91,35,49,49,57,48,93,34,10,32,32,32,32,32,32,32,32,32,100,61,34,109,32,49,48,50,44,50,54,56,56,32,118,32,50,32,72,32,56,54,32,
99,32,45,49,46,49,53,54,44,45,49,46,57,55,49,32,48,46,50,48,54,44,48,46,51,53,32,48,44,48,32,118,32,48,32,48,32,48,32,45,50,32,45,55,32,104,32,49,54,32,122,32,109,32,48,44,57,32,72,32,56,54,32,118,32,
45,55,32,48,32,48,32,99,32,50,46,55,50,51,44,52,46,54,53,52,32,45,49,46,50,57,52,44,45,50,46,50,50,50,32,48,44,48,32,104,32,49,54,32,118,32,48,32,122,32,109,32,45,49,56,44,50,32,104,32,50,48,32,118,32,
45,50,48,32,72,32,56,52,32,90,34,10,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,102,102,102,102,102,102,59,102,105,108,108,45,111,112,97,99,105,116,121,58,48,46,57,57,55,
50,56,57,57,54,34,32,47,62,10,32,32,32,32,60,47,103,62,10,32,32,60,47,103,62,10,32,32,60,112,97,116,104,10,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,110,111,110,101,59,115,116,114,111,
107,101,58,35,102,102,102,102,102,102,59,115,116,114,111,107,101,45,119,105,100,116,104,58,50,46,48,57,57,57,57,57,57,59,115,116,114,111,107,101,45,108,105,110,101,99,97,112,58,98,117,116,116,59,115,116,
114,111,107,101,45,108,105,110,101,106,111,105,110,58,109,105,116,101,114,59,115,116,114,111,107,101,45,109,105,116,101,114,108,105,109,105,116,58,52,59,115,116,114,111,107,101,45,100,97,115,104,97,114,
114,97,121,58,110,111,110,101,59,115,116,114,111,107,101,45,111,112,97,99,105,116,121,58,48,46,57,50,57,53,51,57,51,50,34,10,32,32,32,32,32,100,61,34,109,32,50,46,50,50,52,53,55,54,51,44,54,46,49,50,50,
56,56,49,52,32,99,32,55,46,54,48,53,57,51,50,50,44,48,32,55,46,54,50,55,49,49,56,54,44,48,32,55,46,54,50,55,49,49,56,54,44,48,32,48,44,55,46,50,56,56,49,51,53,54,32,48,44,55,46,51,48,57,51,50,49,54,32,
48,44,55,46,51,48,57,51,50,49,54,34,10,32,32,32,32,32,105,100,61,34,112,97,116,104,49,51,56,56,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,111,110,110,101,99,116,111,114,45,99,117,114,118,
97,116,117,114,101,61,34,48,34,32,47,62,10,32,32,60,112,97,116,104,10,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,110,111,110,101,59,115,116,114,111,107,101,58,35,102,102,102,102,102,102,
59,115,116,114,111,107,101,45,119,105,100,116,104,58,50,46,48,57,57,57,57,57,57,59,115,116,114,111,107,101,45,108,105,110,101,99,97,112,58,98,117,116,116,59,115,116,114,111,107,101,45,108,105,110,101,
106,111,105,110,58,109,105,116,101,114,59,115,116,114,111,107,101,45,109,105,116,101,114,108,105,109,105,116,58,52,59,115,116,114,111,107,101,45,100,97,115,104,97,114,114,97,121,58,110,111,110,101,59,
115,116,114,111,107,101,45,111,112,97,99,105,116,121,58,49,34,10,32,32,32,32,32,100,61,34,109,32,56,46,56,48,57,55,48,51,50,44,49,51,46,52,51,50,50,48,52,32,99,32,56,46,57,54,53,55,49,56,56,44,48,32,56,
46,57,54,53,55,49,56,56,44,48,32,56,46,57,54,53,55,49,56,56,44,48,34,10,32,32,32,32,32,105,100,61,34,112,97,116,104,49,51,57,48,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,111,110,110,101,
99,116,111,114,45,99,117,114,118,97,116,117,114,101,61,34,48,34,32,47,62,10,60,47,115,118,103,62,10,0,0};

const char* VASTWaveTableEditorComponent::waves_square_svg = (const char*) resource_VASTWaveTableEditorComponent_waves_square_svg;
const int VASTWaveTableEditorComponent::waves_square_svgSize = 3000;

// JUCER_RESOURCE: waves_sine_svg, 2805, "../../Resources/Icons/waves_sine.svg"
static const unsigned char resource_VASTWaveTableEditorComponent_waves_sine_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,70,
45,56,34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,10,32,32,32,120,109,108,110,115,58,100,99,61,34,104,116,116,112,58,47,47,112,117,114,108,46,111,114,103,47,100,
99,47,101,108,101,109,101,110,116,115,47,49,46,49,47,34,10,32,32,32,120,109,108,110,115,58,99,99,61,34,104,116,116,112,58,47,47,99,114,101,97,116,105,118,101,99,111,109,109,111,110,115,46,111,114,103,
47,110,115,35,34,10,32,32,32,120,109,108,110,115,58,114,100,102,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,48,50,47,50,50,45,114,100,102,45,115,121,110,116,97,
120,45,110,115,35,34,10,32,32,32,120,109,108,110,115,58,115,118,103,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,118,103,34,10,32,32,32,120,109,108,110,115,
61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,118,103,34,10,32,32,32,120,109,108,110,115,58,115,111,100,105,112,111,100,105,61,34,104,116,116,112,58,47,47,115,
111,100,105,112,111,100,105,46,115,111,117,114,99,101,102,111,114,103,101,46,110,101,116,47,68,84,68,47,115,111,100,105,112,111,100,105,45,48,46,100,116,100,34,10,32,32,32,120,109,108,110,115,58,105,110,
107,115,99,97,112,101,61,34,104,116,116,112,58,47,47,119,119,119,46,105,110,107,115,99,97,112,101,46,111,114,103,47,110,97,109,101,115,112,97,99,101,115,47,105,110,107,115,99,97,112,101,34,10,32,32,32,
119,105,100,116,104,61,34,50,48,112,120,34,10,32,32,32,104,101,105,103,104,116,61,34,50,48,112,120,34,10,32,32,32,118,105,101,119,66,111,120,61,34,48,32,48,32,50,48,32,50,48,34,10,32,32,32,118,101,114,
115,105,111,110,61,34,49,46,49,34,10,32,32,32,105,100,61,34,115,118,103,49,50,34,10,32,32,32,115,111,100,105,112,111,100,105,58,100,111,99,110,97,109,101,61,34,119,97,118,101,115,95,115,105,110,101,46,
115,118,103,34,10,32,32,32,105,110,107,115,99,97,112,101,58,118,101,114,115,105,111,110,61,34,48,46,57,50,46,51,32,40,50,52,48,53,53,52,54,44,32,50,48,49,56,45,48,51,45,49,49,41,34,62,10,32,32,60,109,
101,116,97,100,97,116,97,10,32,32,32,32,32,105,100,61,34,109,101,116,97,100,97,116,97,49,54,34,62,10,32,32,32,32,60,114,100,102,58,82,68,70,62,10,32,32,32,32,32,32,60,99,99,58,87,111,114,107,10,32,32,
32,32,32,32,32,32,32,114,100,102,58,97,98,111,117,116,61,34,34,62,10,32,32,32,32,32,32,32,32,60,100,99,58,102,111,114,109,97,116,62,105,109,97,103,101,47,115,118,103,43,120,109,108,60,47,100,99,58,102,
111,114,109,97,116,62,10,32,32,32,32,32,32,32,32,60,100,99,58,116,121,112,101,10,32,32,32,32,32,32,32,32,32,32,32,114,100,102,58,114,101,115,111,117,114,99,101,61,34,104,116,116,112,58,47,47,112,117,114,
108,46,111,114,103,47,100,99,47,100,99,109,105,116,121,112,101,47,83,116,105,108,108,73,109,97,103,101,34,32,47,62,10,32,32,32,32,32,32,32,32,60,100,99,58,116,105,116,108,101,62,115,116,97,116,115,32,
91,35,49,49,57,48,93,60,47,100,99,58,116,105,116,108,101,62,10,32,32,32,32,32,32,60,47,99,99,58,87,111,114,107,62,10,32,32,32,32,60,47,114,100,102,58,82,68,70,62,10,32,32,60,47,109,101,116,97,100,97,116,
97,62,10,32,32,60,115,111,100,105,112,111,100,105,58,110,97,109,101,100,118,105,101,119,10,32,32,32,32,32,112,97,103,101,99,111,108,111,114,61,34,35,57,53,57,53,57,53,34,10,32,32,32,32,32,98,111,114,100,
101,114,99,111,108,111,114,61,34,35,54,54,54,54,54,54,34,10,32,32,32,32,32,98,111,114,100,101,114,111,112,97,99,105,116,121,61,34,49,34,10,32,32,32,32,32,111,98,106,101,99,116,116,111,108,101,114,97,110,
99,101,61,34,49,48,34,10,32,32,32,32,32,103,114,105,100,116,111,108,101,114,97,110,99,101,61,34,49,48,34,10,32,32,32,32,32,103,117,105,100,101,116,111,108,101,114,97,110,99,101,61,34,49,48,34,10,32,32,
32,32,32,105,110,107,115,99,97,112,101,58,112,97,103,101,111,112,97,99,105,116,121,61,34,49,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,112,97,103,101,115,104,97,100,111,119,61,34,50,34,10,32,
32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,119,105,100,116,104,61,34,51,56,52,48,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,104,101,105,
103,104,116,61,34,50,48,53,48,34,10,32,32,32,32,32,105,100,61,34,110,97,109,101,100,118,105,101,119,49,52,34,10,32,32,32,32,32,115,104,111,119,103,114,105,100,61,34,102,97,108,115,101,34,10,32,32,32,32,
32,105,110,107,115,99,97,112,101,58,122,111,111,109,61,34,52,55,46,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,120,61,34,49,46,52,51,55,55,56,53,51,34,10,32,32,32,32,32,105,110,107,115,
99,97,112,101,58,99,121,61,34,54,46,52,53,57,53,57,56,51,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,120,61,34,45,49,50,34,10,32,32,32,32,32,105,110,107,115,99,97,
112,101,58,119,105,110,100,111,119,45,121,61,34,45,49,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,109,97,120,105,109,105,122,101,100,61,34,49,34,10,32,32,32,32,
32,105,110,107,115,99,97,112,101,58,99,117,114,114,101,110,116,45,108,97,121,101,114,61,34,115,118,103,49,50,34,32,47,62,10,32,32,60,33,45,45,32,71,101,110,101,114,97,116,111,114,58,32,83,107,101,116,
99,104,32,51,46,56,46,49,32,40,50,57,54,56,55,41,32,45,32,104,116,116,112,58,47,47,119,119,119,46,98,111,104,101,109,105,97,110,99,111,100,105,110,103,46,99,111,109,47,115,107,101,116,99,104,32,45,45,
62,10,32,32,60,116,105,116,108,101,10,32,32,32,32,32,105,100,61,34,116,105,116,108,101,50,34,62,115,116,97,116,115,32,91,35,49,49,57,48,93,60,47,116,105,116,108,101,62,10,32,32,60,100,101,115,99,10,32,
32,32,32,32,105,100,61,34,100,101,115,99,52,34,62,67,114,101,97,116,101,100,32,119,105,116,104,32,83,107,101,116,99,104,46,60,47,100,101,115,99,62,10,32,32,60,100,101,102,115,10,32,32,32,32,32,105,100,
61,34,100,101,102,115,54,34,32,47,62,10,32,32,60,103,10,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,45,49,52,48,44,45,50,56,51,57,41,34,10,32,32,32,32,
32,105,100,61,34,68,114,105,98,98,98,108,101,45,76,105,103,104,116,45,80,114,101,118,105,101,119,34,10,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,102,102,102,102,102,102,59,102,105,
108,108,45,114,117,108,101,58,101,118,101,110,111,100,100,59,115,116,114,111,107,101,58,110,111,110,101,59,115,116,114,111,107,101,45,119,105,100,116,104,58,49,59,102,105,108,108,45,111,112,97,99,105,
116,121,58,48,46,57,57,55,50,56,57,57,54,34,62,10,32,32,32,32,60,103,10,32,32,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,53,54,44,49,54,48,41,34,10,32,
32,32,32,32,32,32,105,100,61,34,105,99,111,110,115,34,10,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,102,102,102,102,102,102,59,102,105,108,108,45,111,112,97,99,105,116,121,58,
48,46,57,57,55,50,56,57,57,54,34,62,10,32,32,32,32,32,32,60,112,97,116,104,10,32,32,32,32,32,32,32,32,32,115,111,100,105,112,111,100,105,58,110,111,100,101,116,121,112,101,115,61,34,99,99,99,99,99,99,
99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,34,10,32,32,32,32,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,111,110,110,101,99,116,111,114,45,99,117,114,118,97,116,117,114,101,61,34,
48,34,10,32,32,32,32,32,32,32,32,32,105,100,61,34,115,116,97,116,115,45,91,35,49,49,57,48,93,34,10,32,32,32,32,32,32,32,32,32,100,61,34,109,32,49,48,50,44,50,54,56,56,32,118,32,50,32,72,32,56,54,32,99,
32,45,49,46,49,53,54,44,45,49,46,57,55,49,32,48,46,50,48,54,44,48,46,51,53,32,48,44,48,32,118,32,48,32,48,32,48,32,45,50,32,45,55,32,104,32,49,54,32,122,32,109,32,48,44,57,32,72,32,56,54,32,118,32,45,
55,32,48,32,48,32,99,32,50,46,55,50,51,44,52,46,54,53,52,32,45,49,46,50,57,52,44,45,50,46,50,50,50,32,48,44,48,32,104,32,49,54,32,118,32,48,32,122,32,109,32,45,49,56,44,50,32,104,32,50,48,32,118,32,45,
50,48,32,72,32,56,52,32,90,34,10,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,102,102,102,102,102,102,59,102,105,108,108,45,111,112,97,99,105,116,121,58,48,46,57,57,55,50,
56,57,57,54,34,32,47,62,10,32,32,32,32,60,47,103,62,10,32,32,60,47,103,62,10,32,32,60,112,97,116,104,10,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,110,111,110,101,59,115,116,114,111,107,
101,58,35,102,98,102,98,102,98,59,115,116,114,111,107,101,45,119,105,100,116,104,58,50,59,115,116,114,111,107,101,45,108,105,110,101,99,97,112,58,114,111,117,110,100,59,115,116,114,111,107,101,45,108,
105,110,101,106,111,105,110,58,114,111,117,110,100,59,115,116,114,111,107,101,45,111,112,97,99,105,116,121,58,49,59,115,116,114,111,107,101,45,109,105,116,101,114,108,105,109,105,116,58,52,59,115,116,
114,111,107,101,45,100,97,115,104,97,114,114,97,121,58,110,111,110,101,34,10,32,32,32,32,32,100,61,34,109,32,51,46,55,55,49,49,56,54,52,44,49,48,46,49,50,55,49,49,57,32,99,32,48,44,48,32,50,46,53,44,45,
56,46,49,51,53,53,57,51,54,32,52,46,49,57,52,57,49,53,51,44,45,52,46,48,54,55,55,57,55,32,49,46,54,57,52,57,49,53,50,44,52,46,48,54,55,55,57,55,32,49,46,55,51,55,50,56,56,49,44,52,46,49,49,48,49,55,32,
49,46,55,51,55,50,56,56,49,44,52,46,49,49,48,49,55,32,48,44,48,32,51,46,48,50,57,54,54,49,50,44,56,46,50,54,50,55,49,49,32,52,46,55,48,51,51,57,48,50,44,52,46,49,51,49,51,53,53,32,49,46,54,55,51,55,50,
56,44,45,52,46,49,51,49,51,53,53,32,49,46,54,55,51,55,50,56,44,45,52,46,49,51,49,51,53,53,32,49,46,54,55,51,55,50,56,44,45,52,46,49,51,49,51,53,53,34,10,32,32,32,32,32,105,100,61,34,112,97,116,104,50,
48,48,53,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,111,110,110,101,99,116,111,114,45,99,117,114,118,97,116,117,114,101,61,34,48,34,32,47,62,10,60,47,115,118,103,62,10,0,0};

const char* VASTWaveTableEditorComponent::waves_sine_svg = (const char*) resource_VASTWaveTableEditorComponent_waves_sine_svg;
const int VASTWaveTableEditorComponent::waves_sine_svgSize = 2805;

// JUCER_RESOURCE: waves_triangle_svg, 2693, "../../Resources/Icons/waves_triangle.svg"
static const unsigned char resource_VASTWaveTableEditorComponent_waves_triangle_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,
84,70,45,56,34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,10,32,32,32,120,109,108,110,115,58,100,99,61,34,104,116,116,112,58,47,47,112,117,114,108,46,111,114,103,
47,100,99,47,101,108,101,109,101,110,116,115,47,49,46,49,47,34,10,32,32,32,120,109,108,110,115,58,99,99,61,34,104,116,116,112,58,47,47,99,114,101,97,116,105,118,101,99,111,109,109,111,110,115,46,111,114,
103,47,110,115,35,34,10,32,32,32,120,109,108,110,115,58,114,100,102,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,48,50,47,50,50,45,114,100,102,45,115,121,110,116,
97,120,45,110,115,35,34,10,32,32,32,120,109,108,110,115,58,115,118,103,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,118,103,34,10,32,32,32,120,109,108,110,
115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,118,103,34,10,32,32,32,120,109,108,110,115,58,115,111,100,105,112,111,100,105,61,34,104,116,116,112,58,47,
47,115,111,100,105,112,111,100,105,46,115,111,117,114,99,101,102,111,114,103,101,46,110,101,116,47,68,84,68,47,115,111,100,105,112,111,100,105,45,48,46,100,116,100,34,10,32,32,32,120,109,108,110,115,58,
105,110,107,115,99,97,112,101,61,34,104,116,116,112,58,47,47,119,119,119,46,105,110,107,115,99,97,112,101,46,111,114,103,47,110,97,109,101,115,112,97,99,101,115,47,105,110,107,115,99,97,112,101,34,10,
32,32,32,119,105,100,116,104,61,34,50,48,112,120,34,10,32,32,32,104,101,105,103,104,116,61,34,50,48,112,120,34,10,32,32,32,118,105,101,119,66,111,120,61,34,48,32,48,32,50,48,32,50,48,34,10,32,32,32,118,
101,114,115,105,111,110,61,34,49,46,49,34,10,32,32,32,105,100,61,34,115,118,103,49,50,34,10,32,32,32,115,111,100,105,112,111,100,105,58,100,111,99,110,97,109,101,61,34,119,97,118,101,115,95,116,114,105,
97,110,103,108,101,46,115,118,103,34,10,32,32,32,105,110,107,115,99,97,112,101,58,118,101,114,115,105,111,110,61,34,48,46,57,50,46,51,32,40,50,52,48,53,53,52,54,44,32,50,48,49,56,45,48,51,45,49,49,41,
34,62,10,32,32,60,109,101,116,97,100,97,116,97,10,32,32,32,32,32,105,100,61,34,109,101,116,97,100,97,116,97,49,54,34,62,10,32,32,32,32,60,114,100,102,58,82,68,70,62,10,32,32,32,32,32,32,60,99,99,58,87,
111,114,107,10,32,32,32,32,32,32,32,32,32,114,100,102,58,97,98,111,117,116,61,34,34,62,10,32,32,32,32,32,32,32,32,60,100,99,58,102,111,114,109,97,116,62,105,109,97,103,101,47,115,118,103,43,120,109,108,
60,47,100,99,58,102,111,114,109,97,116,62,10,32,32,32,32,32,32,32,32,60,100,99,58,116,121,112,101,10,32,32,32,32,32,32,32,32,32,32,32,114,100,102,58,114,101,115,111,117,114,99,101,61,34,104,116,116,112,
58,47,47,112,117,114,108,46,111,114,103,47,100,99,47,100,99,109,105,116,121,112,101,47,83,116,105,108,108,73,109,97,103,101,34,32,47,62,10,32,32,32,32,32,32,32,32,60,100,99,58,116,105,116,108,101,62,115,
116,97,116,115,32,91,35,49,49,57,48,93,60,47,100,99,58,116,105,116,108,101,62,10,32,32,32,32,32,32,60,47,99,99,58,87,111,114,107,62,10,32,32,32,32,60,47,114,100,102,58,82,68,70,62,10,32,32,60,47,109,101,
116,97,100,97,116,97,62,10,32,32,60,115,111,100,105,112,111,100,105,58,110,97,109,101,100,118,105,101,119,10,32,32,32,32,32,112,97,103,101,99,111,108,111,114,61,34,35,57,53,57,53,57,53,34,10,32,32,32,
32,32,98,111,114,100,101,114,99,111,108,111,114,61,34,35,54,54,54,54,54,54,34,10,32,32,32,32,32,98,111,114,100,101,114,111,112,97,99,105,116,121,61,34,49,34,10,32,32,32,32,32,111,98,106,101,99,116,116,
111,108,101,114,97,110,99,101,61,34,49,48,34,10,32,32,32,32,32,103,114,105,100,116,111,108,101,114,97,110,99,101,61,34,49,48,34,10,32,32,32,32,32,103,117,105,100,101,116,111,108,101,114,97,110,99,101,
61,34,49,48,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,112,97,103,101,111,112,97,99,105,116,121,61,34,49,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,112,97,103,101,115,104,97,100,
111,119,61,34,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,119,105,100,116,104,61,34,51,56,52,48,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,
100,111,119,45,104,101,105,103,104,116,61,34,50,48,53,48,34,10,32,32,32,32,32,105,100,61,34,110,97,109,101,100,118,105,101,119,49,52,34,10,32,32,32,32,32,115,104,111,119,103,114,105,100,61,34,102,97,108,
115,101,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,122,111,111,109,61,34,52,55,46,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,120,61,34,54,46,56,48,56,53,52,56,34,10,32,32,32,
32,32,105,110,107,115,99,97,112,101,58,99,121,61,34,54,46,52,53,57,53,57,56,51,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,120,61,34,45,49,50,34,10,32,32,32,32,32,
105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,121,61,34,45,49,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,109,97,120,105,109,105,122,101,100,61,34,
49,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,117,114,114,101,110,116,45,108,97,121,101,114,61,34,115,118,103,49,50,34,32,47,62,10,32,32,60,33,45,45,32,71,101,110,101,114,97,116,111,114,
58,32,83,107,101,116,99,104,32,51,46,56,46,49,32,40,50,57,54,56,55,41,32,45,32,104,116,116,112,58,47,47,119,119,119,46,98,111,104,101,109,105,97,110,99,111,100,105,110,103,46,99,111,109,47,115,107,101,
116,99,104,32,45,45,62,10,32,32,60,116,105,116,108,101,10,32,32,32,32,32,105,100,61,34,116,105,116,108,101,50,34,62,115,116,97,116,115,32,91,35,49,49,57,48,93,60,47,116,105,116,108,101,62,10,32,32,60,
100,101,115,99,10,32,32,32,32,32,105,100,61,34,100,101,115,99,52,34,62,67,114,101,97,116,101,100,32,119,105,116,104,32,83,107,101,116,99,104,46,60,47,100,101,115,99,62,10,32,32,60,100,101,102,115,10,32,
32,32,32,32,105,100,61,34,100,101,102,115,54,34,32,47,62,10,32,32,60,103,10,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,45,49,52,48,44,45,50,56,51,57,41,
34,10,32,32,32,32,32,105,100,61,34,68,114,105,98,98,98,108,101,45,76,105,103,104,116,45,80,114,101,118,105,101,119,34,10,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,102,102,102,102,
102,102,59,102,105,108,108,45,114,117,108,101,58,101,118,101,110,111,100,100,59,115,116,114,111,107,101,58,110,111,110,101,59,115,116,114,111,107,101,45,119,105,100,116,104,58,49,59,102,105,108,108,45,
111,112,97,99,105,116,121,58,48,46,57,57,55,50,56,57,57,54,34,62,10,32,32,32,32,60,103,10,32,32,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,53,54,44,49,
54,48,41,34,10,32,32,32,32,32,32,32,105,100,61,34,105,99,111,110,115,34,10,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,102,102,102,102,102,102,59,102,105,108,108,45,111,112,97,
99,105,116,121,58,48,46,57,57,55,50,56,57,57,54,34,62,10,32,32,32,32,32,32,60,112,97,116,104,10,32,32,32,32,32,32,32,32,32,115,111,100,105,112,111,100,105,58,110,111,100,101,116,121,112,101,115,61,34,
99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,34,10,32,32,32,32,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,111,110,110,101,99,116,111,114,45,99,117,114,118,97,116,
117,114,101,61,34,48,34,10,32,32,32,32,32,32,32,32,32,105,100,61,34,115,116,97,116,115,45,91,35,49,49,57,48,93,34,10,32,32,32,32,32,32,32,32,32,100,61,34,109,32,49,48,50,44,50,54,56,56,32,118,32,50,32,
72,32,56,54,32,99,32,45,49,46,49,53,54,44,45,49,46,57,55,49,32,48,46,50,48,54,44,48,46,51,53,32,48,44,48,32,118,32,48,32,48,32,48,32,45,50,32,45,55,32,104,32,49,54,32,122,32,109,32,48,44,57,32,72,32,56,
54,32,118,32,45,55,32,48,32,48,32,99,32,50,46,55,50,51,44,52,46,54,53,52,32,45,49,46,50,57,52,44,45,50,46,50,50,50,32,48,44,48,32,104,32,49,54,32,118,32,48,32,122,32,109,32,45,49,56,44,50,32,104,32,50,
48,32,118,32,45,50,48,32,72,32,56,52,32,90,34,10,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,102,102,102,102,102,102,59,102,105,108,108,45,111,112,97,99,105,116,121,58,48,
46,57,57,55,50,56,57,57,54,34,32,47,62,10,32,32,32,32,60,47,103,62,10,32,32,60,47,103,62,10,32,32,60,112,97,116,104,10,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,110,111,110,101,59,115,
116,114,111,107,101,58,35,102,102,102,102,102,102,59,115,116,114,111,107,101,45,119,105,100,116,104,58,50,59,115,116,114,111,107,101,45,108,105,110,101,99,97,112,58,114,111,117,110,100,59,115,116,114,
111,107,101,45,108,105,110,101,106,111,105,110,58,114,111,117,110,100,59,115,116,114,111,107,101,45,109,105,116,101,114,108,105,109,105,116,58,52,59,115,116,114,111,107,101,45,100,97,115,104,97,114,114,
97,121,58,110,111,110,101,59,115,116,114,111,107,101,45,111,112,97,99,105,116,121,58,49,34,10,32,32,32,32,32,100,61,34,77,32,52,46,54,54,49,48,49,54,57,44,49,48,46,54,55,55,57,54,54,32,56,46,54,48,49,
54,57,52,57,44,52,46,51,56,53,53,57,51,50,32,56,46,53,56,48,53,48,56,52,44,52,46,51,52,51,50,50,48,51,32,49,49,46,55,49,54,49,48,50,44,49,53,46,52,50,51,55,50,57,32,49,53,46,48,50,49,49,56,55,44,49,48,
46,51,49,55,55,57,55,34,10,32,32,32,32,32,105,100,61,34,112,97,116,104,49,52,50,55,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,111,110,110,101,99,116,111,114,45,99,117,114,118,97,116,117,
114,101,61,34,48,34,32,47,62,10,60,47,115,118,103,62,10,0,0};

const char* VASTWaveTableEditorComponent::waves_triangle_svg = (const char*) resource_VASTWaveTableEditorComponent_waves_triangle_svg;
const int VASTWaveTableEditorComponent::waves_triangle_svgSize = 2693;

// JUCER_RESOURCE: waves_saw_svg, 2662, "../../Resources/Icons/waves_saw.svg"
static const unsigned char resource_VASTWaveTableEditorComponent_waves_saw_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,70,
45,56,34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,10,32,32,32,120,109,108,110,115,58,100,99,61,34,104,116,116,112,58,47,47,112,117,114,108,46,111,114,103,47,100,
99,47,101,108,101,109,101,110,116,115,47,49,46,49,47,34,10,32,32,32,120,109,108,110,115,58,99,99,61,34,104,116,116,112,58,47,47,99,114,101,97,116,105,118,101,99,111,109,109,111,110,115,46,111,114,103,
47,110,115,35,34,10,32,32,32,120,109,108,110,115,58,114,100,102,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,48,50,47,50,50,45,114,100,102,45,115,121,110,116,97,
120,45,110,115,35,34,10,32,32,32,120,109,108,110,115,58,115,118,103,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,118,103,34,10,32,32,32,120,109,108,110,115,
61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,118,103,34,10,32,32,32,120,109,108,110,115,58,115,111,100,105,112,111,100,105,61,34,104,116,116,112,58,47,47,115,
111,100,105,112,111,100,105,46,115,111,117,114,99,101,102,111,114,103,101,46,110,101,116,47,68,84,68,47,115,111,100,105,112,111,100,105,45,48,46,100,116,100,34,10,32,32,32,120,109,108,110,115,58,105,110,
107,115,99,97,112,101,61,34,104,116,116,112,58,47,47,119,119,119,46,105,110,107,115,99,97,112,101,46,111,114,103,47,110,97,109,101,115,112,97,99,101,115,47,105,110,107,115,99,97,112,101,34,10,32,32,32,
119,105,100,116,104,61,34,50,48,112,120,34,10,32,32,32,104,101,105,103,104,116,61,34,50,48,112,120,34,10,32,32,32,118,105,101,119,66,111,120,61,34,48,32,48,32,50,48,32,50,48,34,10,32,32,32,118,101,114,
115,105,111,110,61,34,49,46,49,34,10,32,32,32,105,100,61,34,115,118,103,49,50,34,10,32,32,32,115,111,100,105,112,111,100,105,58,100,111,99,110,97,109,101,61,34,119,97,118,101,115,95,115,97,119,46,115,
118,103,34,10,32,32,32,105,110,107,115,99,97,112,101,58,118,101,114,115,105,111,110,61,34,48,46,57,50,46,51,32,40,50,52,48,53,53,52,54,44,32,50,48,49,56,45,48,51,45,49,49,41,34,62,10,32,32,60,109,101,
116,97,100,97,116,97,10,32,32,32,32,32,105,100,61,34,109,101,116,97,100,97,116,97,49,54,34,62,10,32,32,32,32,60,114,100,102,58,82,68,70,62,10,32,32,32,32,32,32,60,99,99,58,87,111,114,107,10,32,32,32,32,
32,32,32,32,32,114,100,102,58,97,98,111,117,116,61,34,34,62,10,32,32,32,32,32,32,32,32,60,100,99,58,102,111,114,109,97,116,62,105,109,97,103,101,47,115,118,103,43,120,109,108,60,47,100,99,58,102,111,114,
109,97,116,62,10,32,32,32,32,32,32,32,32,60,100,99,58,116,121,112,101,10,32,32,32,32,32,32,32,32,32,32,32,114,100,102,58,114,101,115,111,117,114,99,101,61,34,104,116,116,112,58,47,47,112,117,114,108,46,
111,114,103,47,100,99,47,100,99,109,105,116,121,112,101,47,83,116,105,108,108,73,109,97,103,101,34,32,47,62,10,32,32,32,32,32,32,32,32,60,100,99,58,116,105,116,108,101,62,115,116,97,116,115,32,91,35,49,
49,57,48,93,60,47,100,99,58,116,105,116,108,101,62,10,32,32,32,32,32,32,60,47,99,99,58,87,111,114,107,62,10,32,32,32,32,60,47,114,100,102,58,82,68,70,62,10,32,32,60,47,109,101,116,97,100,97,116,97,62,
10,32,32,60,115,111,100,105,112,111,100,105,58,110,97,109,101,100,118,105,101,119,10,32,32,32,32,32,112,97,103,101,99,111,108,111,114,61,34,35,57,53,57,53,57,53,34,10,32,32,32,32,32,98,111,114,100,101,
114,99,111,108,111,114,61,34,35,54,54,54,54,54,54,34,10,32,32,32,32,32,98,111,114,100,101,114,111,112,97,99,105,116,121,61,34,49,34,10,32,32,32,32,32,111,98,106,101,99,116,116,111,108,101,114,97,110,99,
101,61,34,49,48,34,10,32,32,32,32,32,103,114,105,100,116,111,108,101,114,97,110,99,101,61,34,49,48,34,10,32,32,32,32,32,103,117,105,100,101,116,111,108,101,114,97,110,99,101,61,34,49,48,34,10,32,32,32,
32,32,105,110,107,115,99,97,112,101,58,112,97,103,101,111,112,97,99,105,116,121,61,34,49,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,112,97,103,101,115,104,97,100,111,119,61,34,50,34,10,32,32,
32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,119,105,100,116,104,61,34,51,56,52,48,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,104,101,105,
103,104,116,61,34,50,48,53,48,34,10,32,32,32,32,32,105,100,61,34,110,97,109,101,100,118,105,101,119,49,52,34,10,32,32,32,32,32,115,104,111,119,103,114,105,100,61,34,102,97,108,115,101,34,10,32,32,32,32,
32,105,110,107,115,99,97,112,101,58,122,111,111,109,61,34,52,55,46,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,120,61,34,54,46,56,48,56,53,52,56,34,10,32,32,32,32,32,105,110,107,115,99,
97,112,101,58,99,121,61,34,54,46,52,53,57,53,57,56,51,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,120,61,34,45,49,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,
101,58,119,105,110,100,111,119,45,121,61,34,45,49,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,109,97,120,105,109,105,122,101,100,61,34,49,34,10,32,32,32,32,32,105,
110,107,115,99,97,112,101,58,99,117,114,114,101,110,116,45,108,97,121,101,114,61,34,115,118,103,49,50,34,32,47,62,10,32,32,60,33,45,45,32,71,101,110,101,114,97,116,111,114,58,32,83,107,101,116,99,104,
32,51,46,56,46,49,32,40,50,57,54,56,55,41,32,45,32,104,116,116,112,58,47,47,119,119,119,46,98,111,104,101,109,105,97,110,99,111,100,105,110,103,46,99,111,109,47,115,107,101,116,99,104,32,45,45,62,10,32,
32,60,116,105,116,108,101,10,32,32,32,32,32,105,100,61,34,116,105,116,108,101,50,34,62,115,116,97,116,115,32,91,35,49,49,57,48,93,60,47,116,105,116,108,101,62,10,32,32,60,100,101,115,99,10,32,32,32,32,
32,105,100,61,34,100,101,115,99,52,34,62,67,114,101,97,116,101,100,32,119,105,116,104,32,83,107,101,116,99,104,46,60,47,100,101,115,99,62,10,32,32,60,100,101,102,115,10,32,32,32,32,32,105,100,61,34,100,
101,102,115,54,34,32,47,62,10,32,32,60,103,10,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,45,49,52,48,44,45,50,56,51,57,41,34,10,32,32,32,32,32,105,100,
61,34,68,114,105,98,98,98,108,101,45,76,105,103,104,116,45,80,114,101,118,105,101,119,34,10,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,102,102,102,102,102,102,59,102,105,108,108,45,
114,117,108,101,58,101,118,101,110,111,100,100,59,115,116,114,111,107,101,58,110,111,110,101,59,115,116,114,111,107,101,45,119,105,100,116,104,58,49,59,102,105,108,108,45,111,112,97,99,105,116,121,58,
48,46,57,57,55,50,56,57,57,54,34,62,10,32,32,32,32,60,103,10,32,32,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,53,54,44,49,54,48,41,34,10,32,32,32,32,32,
32,32,105,100,61,34,105,99,111,110,115,34,10,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,102,102,102,102,102,102,59,102,105,108,108,45,111,112,97,99,105,116,121,58,48,46,57,57,
55,50,56,57,57,54,34,62,10,32,32,32,32,32,32,60,112,97,116,104,10,32,32,32,32,32,32,32,32,32,115,111,100,105,112,111,100,105,58,110,111,100,101,116,121,112,101,115,61,34,99,99,99,99,99,99,99,99,99,99,
99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,34,10,32,32,32,32,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,111,110,110,101,99,116,111,114,45,99,117,114,118,97,116,117,114,101,61,34,48,34,10,32,
32,32,32,32,32,32,32,32,105,100,61,34,115,116,97,116,115,45,91,35,49,49,57,48,93,34,10,32,32,32,32,32,32,32,32,32,100,61,34,109,32,49,48,50,44,50,54,56,56,32,118,32,50,32,72,32,56,54,32,99,32,45,49,46,
49,53,54,44,45,49,46,57,55,49,32,48,46,50,48,54,44,48,46,51,53,32,48,44,48,32,118,32,48,32,48,32,48,32,45,50,32,45,55,32,104,32,49,54,32,122,32,109,32,48,44,57,32,72,32,56,54,32,118,32,45,55,32,48,32,
48,32,99,32,50,46,55,50,51,44,52,46,54,53,52,32,45,49,46,50,57,52,44,45,50,46,50,50,50,32,48,44,48,32,104,32,49,54,32,118,32,48,32,122,32,109,32,45,49,56,44,50,32,104,32,50,48,32,118,32,45,50,48,32,72,
32,56,52,32,90,34,10,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,102,102,102,102,102,102,59,102,105,108,108,45,111,112,97,99,105,116,121,58,48,46,57,57,55,50,56,57,57,54,
34,32,47,62,10,32,32,32,32,60,47,103,62,10,32,32,60,47,103,62,10,32,32,60,112,97,116,104,10,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,110,111,110,101,59,115,116,114,111,107,101,58,35,
101,52,101,52,101,52,59,115,116,114,111,107,101,45,119,105,100,116,104,58,50,59,115,116,114,111,107,101,45,108,105,110,101,99,97,112,58,114,111,117,110,100,59,115,116,114,111,107,101,45,108,105,110,101,
106,111,105,110,58,114,111,117,110,100,59,115,116,114,111,107,101,45,111,112,97,99,105,116,121,58,49,59,115,116,114,111,107,101,45,109,105,116,101,114,108,105,109,105,116,58,52,59,115,116,114,111,107,
101,45,100,97,115,104,97,114,114,97,121,58,110,111,110,101,34,10,32,32,32,32,32,100,61,34,77,32,51,46,54,56,54,52,52,48,55,44,49,48,46,49,57,48,54,55,56,32,49,48,46,48,56,52,55,52,54,44,53,46,53,57,51,
50,50,48,52,32,118,32,57,46,49,51,49,51,53,53,54,32,108,32,54,46,51,49,51,53,53,57,44,45,52,46,56,55,50,56,56,48,56,34,10,32,32,32,32,32,105,100,61,34,112,97,116,104,49,52,51,54,34,10,32,32,32,32,32,105,
110,107,115,99,97,112,101,58,99,111,110,110,101,99,116,111,114,45,99,117,114,118,97,116,117,114,101,61,34,48,34,32,47,62,10,60,47,115,118,103,62,10,0,0};

const char* VASTWaveTableEditorComponent::waves_saw_svg = (const char*) resource_VASTWaveTableEditorComponent_waves_saw_svg;
const int VASTWaveTableEditorComponent::waves_saw_svgSize = 2662;

// JUCER_RESOURCE: waves_ramp_svg, 2629, "../../Resources/Icons/waves_ramp.svg"
static const unsigned char resource_VASTWaveTableEditorComponent_waves_ramp_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,70,
45,56,34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,10,32,32,32,120,109,108,110,115,58,100,99,61,34,104,116,116,112,58,47,47,112,117,114,108,46,111,114,103,47,100,
99,47,101,108,101,109,101,110,116,115,47,49,46,49,47,34,10,32,32,32,120,109,108,110,115,58,99,99,61,34,104,116,116,112,58,47,47,99,114,101,97,116,105,118,101,99,111,109,109,111,110,115,46,111,114,103,
47,110,115,35,34,10,32,32,32,120,109,108,110,115,58,114,100,102,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,48,50,47,50,50,45,114,100,102,45,115,121,110,116,97,
120,45,110,115,35,34,10,32,32,32,120,109,108,110,115,58,115,118,103,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,118,103,34,10,32,32,32,120,109,108,110,115,
61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,118,103,34,10,32,32,32,120,109,108,110,115,58,115,111,100,105,112,111,100,105,61,34,104,116,116,112,58,47,47,115,
111,100,105,112,111,100,105,46,115,111,117,114,99,101,102,111,114,103,101,46,110,101,116,47,68,84,68,47,115,111,100,105,112,111,100,105,45,48,46,100,116,100,34,10,32,32,32,120,109,108,110,115,58,105,110,
107,115,99,97,112,101,61,34,104,116,116,112,58,47,47,119,119,119,46,105,110,107,115,99,97,112,101,46,111,114,103,47,110,97,109,101,115,112,97,99,101,115,47,105,110,107,115,99,97,112,101,34,10,32,32,32,
119,105,100,116,104,61,34,50,48,112,120,34,10,32,32,32,104,101,105,103,104,116,61,34,50,48,112,120,34,10,32,32,32,118,105,101,119,66,111,120,61,34,48,32,48,32,50,48,32,50,48,34,10,32,32,32,118,101,114,
115,105,111,110,61,34,49,46,49,34,10,32,32,32,105,100,61,34,115,118,103,49,50,34,10,32,32,32,115,111,100,105,112,111,100,105,58,100,111,99,110,97,109,101,61,34,119,97,118,101,115,95,114,97,109,112,46,
115,118,103,34,10,32,32,32,105,110,107,115,99,97,112,101,58,118,101,114,115,105,111,110,61,34,48,46,57,50,46,51,32,40,50,52,48,53,53,52,54,44,32,50,48,49,56,45,48,51,45,49,49,41,34,62,10,32,32,60,109,
101,116,97,100,97,116,97,10,32,32,32,32,32,105,100,61,34,109,101,116,97,100,97,116,97,49,54,34,62,10,32,32,32,32,60,114,100,102,58,82,68,70,62,10,32,32,32,32,32,32,60,99,99,58,87,111,114,107,10,32,32,
32,32,32,32,32,32,32,114,100,102,58,97,98,111,117,116,61,34,34,62,10,32,32,32,32,32,32,32,32,60,100,99,58,102,111,114,109,97,116,62,105,109,97,103,101,47,115,118,103,43,120,109,108,60,47,100,99,58,102,
111,114,109,97,116,62,10,32,32,32,32,32,32,32,32,60,100,99,58,116,121,112,101,10,32,32,32,32,32,32,32,32,32,32,32,114,100,102,58,114,101,115,111,117,114,99,101,61,34,104,116,116,112,58,47,47,112,117,114,
108,46,111,114,103,47,100,99,47,100,99,109,105,116,121,112,101,47,83,116,105,108,108,73,109,97,103,101,34,32,47,62,10,32,32,32,32,32,32,32,32,60,100,99,58,116,105,116,108,101,62,115,116,97,116,115,32,
91,35,49,49,57,48,93,60,47,100,99,58,116,105,116,108,101,62,10,32,32,32,32,32,32,60,47,99,99,58,87,111,114,107,62,10,32,32,32,32,60,47,114,100,102,58,82,68,70,62,10,32,32,60,47,109,101,116,97,100,97,116,
97,62,10,32,32,60,115,111,100,105,112,111,100,105,58,110,97,109,101,100,118,105,101,119,10,32,32,32,32,32,112,97,103,101,99,111,108,111,114,61,34,35,57,53,57,53,57,53,34,10,32,32,32,32,32,98,111,114,100,
101,114,99,111,108,111,114,61,34,35,54,54,54,54,54,54,34,10,32,32,32,32,32,98,111,114,100,101,114,111,112,97,99,105,116,121,61,34,49,34,10,32,32,32,32,32,111,98,106,101,99,116,116,111,108,101,114,97,110,
99,101,61,34,49,48,34,10,32,32,32,32,32,103,114,105,100,116,111,108,101,114,97,110,99,101,61,34,49,48,34,10,32,32,32,32,32,103,117,105,100,101,116,111,108,101,114,97,110,99,101,61,34,49,48,34,10,32,32,
32,32,32,105,110,107,115,99,97,112,101,58,112,97,103,101,111,112,97,99,105,116,121,61,34,49,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,112,97,103,101,115,104,97,100,111,119,61,34,50,34,10,32,
32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,119,105,100,116,104,61,34,51,56,52,48,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,104,101,105,
103,104,116,61,34,50,48,53,48,34,10,32,32,32,32,32,105,100,61,34,110,97,109,101,100,118,105,101,119,49,52,34,10,32,32,32,32,32,115,104,111,119,103,114,105,100,61,34,102,97,108,115,101,34,10,32,32,32,32,
32,105,110,107,115,99,97,112,101,58,122,111,111,109,61,34,52,55,46,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,120,61,34,54,46,56,48,56,53,52,56,34,10,32,32,32,32,32,105,110,107,115,99,
97,112,101,58,99,121,61,34,54,46,52,53,57,53,57,56,51,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,120,61,34,45,49,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,
101,58,119,105,110,100,111,119,45,121,61,34,45,49,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,109,97,120,105,109,105,122,101,100,61,34,49,34,10,32,32,32,32,32,105,
110,107,115,99,97,112,101,58,99,117,114,114,101,110,116,45,108,97,121,101,114,61,34,115,118,103,49,50,34,32,47,62,10,32,32,60,33,45,45,32,71,101,110,101,114,97,116,111,114,58,32,83,107,101,116,99,104,
32,51,46,56,46,49,32,40,50,57,54,56,55,41,32,45,32,104,116,116,112,58,47,47,119,119,119,46,98,111,104,101,109,105,97,110,99,111,100,105,110,103,46,99,111,109,47,115,107,101,116,99,104,32,45,45,62,10,32,
32,60,116,105,116,108,101,10,32,32,32,32,32,105,100,61,34,116,105,116,108,101,50,34,62,115,116,97,116,115,32,91,35,49,49,57,48,93,60,47,116,105,116,108,101,62,10,32,32,60,100,101,115,99,10,32,32,32,32,
32,105,100,61,34,100,101,115,99,52,34,62,67,114,101,97,116,101,100,32,119,105,116,104,32,83,107,101,116,99,104,46,60,47,100,101,115,99,62,10,32,32,60,100,101,102,115,10,32,32,32,32,32,105,100,61,34,100,
101,102,115,54,34,32,47,62,10,32,32,60,103,10,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,45,49,52,48,44,45,50,56,51,57,41,34,10,32,32,32,32,32,105,100,
61,34,68,114,105,98,98,98,108,101,45,76,105,103,104,116,45,80,114,101,118,105,101,119,34,10,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,102,102,102,102,102,102,59,102,105,108,108,45,
114,117,108,101,58,101,118,101,110,111,100,100,59,115,116,114,111,107,101,58,110,111,110,101,59,115,116,114,111,107,101,45,119,105,100,116,104,58,49,59,102,105,108,108,45,111,112,97,99,105,116,121,58,
48,46,57,57,55,50,56,57,57,54,34,62,10,32,32,32,32,60,103,10,32,32,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,53,54,44,49,54,48,41,34,10,32,32,32,32,32,
32,32,105,100,61,34,105,99,111,110,115,34,10,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,102,102,102,102,102,102,59,102,105,108,108,45,111,112,97,99,105,116,121,58,48,46,57,57,
55,50,56,57,57,54,34,62,10,32,32,32,32,32,32,60,112,97,116,104,10,32,32,32,32,32,32,32,32,32,115,111,100,105,112,111,100,105,58,110,111,100,101,116,121,112,101,115,61,34,99,99,99,99,99,99,99,99,99,99,
99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,34,10,32,32,32,32,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,111,110,110,101,99,116,111,114,45,99,117,114,118,97,116,117,114,101,61,34,48,34,10,32,
32,32,32,32,32,32,32,32,105,100,61,34,115,116,97,116,115,45,91,35,49,49,57,48,93,34,10,32,32,32,32,32,32,32,32,32,100,61,34,109,32,49,48,50,44,50,54,56,56,32,118,32,50,32,72,32,56,54,32,99,32,45,49,46,
49,53,54,44,45,49,46,57,55,49,32,48,46,50,48,54,44,48,46,51,53,32,48,44,48,32,118,32,48,32,48,32,48,32,45,50,32,45,55,32,104,32,49,54,32,122,32,109,32,48,44,57,32,72,32,56,54,32,118,32,45,55,32,48,32,
48,32,99,32,50,46,55,50,51,44,52,46,54,53,52,32,45,49,46,50,57,52,44,45,50,46,50,50,50,32,48,44,48,32,104,32,49,54,32,118,32,48,32,122,32,109,32,45,49,56,44,50,32,104,32,50,48,32,118,32,45,50,48,32,72,
32,56,52,32,90,34,10,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,102,102,102,102,102,102,59,102,105,108,108,45,111,112,97,99,105,116,121,58,48,46,57,57,55,50,56,57,57,54,
34,32,47,62,10,32,32,32,32,60,47,103,62,10,32,32,60,47,103,62,10,32,32,60,112,97,116,104,10,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,110,111,110,101,59,115,116,114,111,107,101,58,35,
102,102,102,102,102,102,59,115,116,114,111,107,101,45,119,105,100,116,104,58,50,59,115,116,114,111,107,101,45,108,105,110,101,99,97,112,58,114,111,117,110,100,59,115,116,114,111,107,101,45,108,105,110,
101,106,111,105,110,58,114,111,117,110,100,59,115,116,114,111,107,101,45,111,112,97,99,105,116,121,58,49,59,115,116,114,111,107,101,45,109,105,116,101,114,108,105,109,105,116,58,52,59,115,116,114,111,
107,101,45,100,97,115,104,97,114,114,97,121,58,110,111,110,101,34,10,32,32,32,32,32,100,61,34,77,32,52,46,52,52,57,49,53,50,53,44,49,53,46,54,57,57,49,53,51,32,49,53,46,53,50,57,54,54,49,44,52,46,55,50,
52,53,55,54,51,34,10,32,32,32,32,32,105,100,61,34,112,97,116,104,49,52,52,48,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,111,110,110,101,99,116,111,114,45,99,117,114,118,97,116,117,114,101,
61,34,48,34,32,47,62,10,60,47,115,118,103,62,10,0,0};

const char* VASTWaveTableEditorComponent::waves_ramp_svg = (const char*) resource_VASTWaveTableEditorComponent_waves_ramp_svg;
const int VASTWaveTableEditorComponent::waves_ramp_svgSize = 2629;

// JUCER_RESOURCE: waves_halfsine_svg, 2720, "../../Resources/Icons/waves_halfsine.svg"
static const unsigned char resource_VASTWaveTableEditorComponent_waves_halfsine_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,
84,70,45,56,34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,10,32,32,32,120,109,108,110,115,58,100,99,61,34,104,116,116,112,58,47,47,112,117,114,108,46,111,114,103,
47,100,99,47,101,108,101,109,101,110,116,115,47,49,46,49,47,34,10,32,32,32,120,109,108,110,115,58,99,99,61,34,104,116,116,112,58,47,47,99,114,101,97,116,105,118,101,99,111,109,109,111,110,115,46,111,114,
103,47,110,115,35,34,10,32,32,32,120,109,108,110,115,58,114,100,102,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,48,50,47,50,50,45,114,100,102,45,115,121,110,116,
97,120,45,110,115,35,34,10,32,32,32,120,109,108,110,115,58,115,118,103,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,118,103,34,10,32,32,32,120,109,108,110,
115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,118,103,34,10,32,32,32,120,109,108,110,115,58,115,111,100,105,112,111,100,105,61,34,104,116,116,112,58,47,
47,115,111,100,105,112,111,100,105,46,115,111,117,114,99,101,102,111,114,103,101,46,110,101,116,47,68,84,68,47,115,111,100,105,112,111,100,105,45,48,46,100,116,100,34,10,32,32,32,120,109,108,110,115,58,
105,110,107,115,99,97,112,101,61,34,104,116,116,112,58,47,47,119,119,119,46,105,110,107,115,99,97,112,101,46,111,114,103,47,110,97,109,101,115,112,97,99,101,115,47,105,110,107,115,99,97,112,101,34,10,
32,32,32,119,105,100,116,104,61,34,50,48,112,120,34,10,32,32,32,104,101,105,103,104,116,61,34,50,48,112,120,34,10,32,32,32,118,105,101,119,66,111,120,61,34,48,32,48,32,50,48,32,50,48,34,10,32,32,32,118,
101,114,115,105,111,110,61,34,49,46,49,34,10,32,32,32,105,100,61,34,115,118,103,49,50,34,10,32,32,32,115,111,100,105,112,111,100,105,58,100,111,99,110,97,109,101,61,34,119,97,118,101,115,95,104,97,108,
102,115,105,110,101,46,115,118,103,34,10,32,32,32,105,110,107,115,99,97,112,101,58,118,101,114,115,105,111,110,61,34,48,46,57,50,46,51,32,40,50,52,48,53,53,52,54,44,32,50,48,49,56,45,48,51,45,49,49,41,
34,62,10,32,32,60,109,101,116,97,100,97,116,97,10,32,32,32,32,32,105,100,61,34,109,101,116,97,100,97,116,97,49,54,34,62,10,32,32,32,32,60,114,100,102,58,82,68,70,62,10,32,32,32,32,32,32,60,99,99,58,87,
111,114,107,10,32,32,32,32,32,32,32,32,32,114,100,102,58,97,98,111,117,116,61,34,34,62,10,32,32,32,32,32,32,32,32,60,100,99,58,102,111,114,109,97,116,62,105,109,97,103,101,47,115,118,103,43,120,109,108,
60,47,100,99,58,102,111,114,109,97,116,62,10,32,32,32,32,32,32,32,32,60,100,99,58,116,121,112,101,10,32,32,32,32,32,32,32,32,32,32,32,114,100,102,58,114,101,115,111,117,114,99,101,61,34,104,116,116,112,
58,47,47,112,117,114,108,46,111,114,103,47,100,99,47,100,99,109,105,116,121,112,101,47,83,116,105,108,108,73,109,97,103,101,34,32,47,62,10,32,32,32,32,32,32,32,32,60,100,99,58,116,105,116,108,101,62,115,
116,97,116,115,32,91,35,49,49,57,48,93,60,47,100,99,58,116,105,116,108,101,62,10,32,32,32,32,32,32,60,47,99,99,58,87,111,114,107,62,10,32,32,32,32,60,47,114,100,102,58,82,68,70,62,10,32,32,60,47,109,101,
116,97,100,97,116,97,62,10,32,32,60,115,111,100,105,112,111,100,105,58,110,97,109,101,100,118,105,101,119,10,32,32,32,32,32,112,97,103,101,99,111,108,111,114,61,34,35,57,53,57,53,57,53,34,10,32,32,32,
32,32,98,111,114,100,101,114,99,111,108,111,114,61,34,35,54,54,54,54,54,54,34,10,32,32,32,32,32,98,111,114,100,101,114,111,112,97,99,105,116,121,61,34,49,34,10,32,32,32,32,32,111,98,106,101,99,116,116,
111,108,101,114,97,110,99,101,61,34,49,48,34,10,32,32,32,32,32,103,114,105,100,116,111,108,101,114,97,110,99,101,61,34,49,48,34,10,32,32,32,32,32,103,117,105,100,101,116,111,108,101,114,97,110,99,101,
61,34,49,48,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,112,97,103,101,111,112,97,99,105,116,121,61,34,49,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,112,97,103,101,115,104,97,100,
111,119,61,34,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,119,105,100,116,104,61,34,51,56,52,48,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,
100,111,119,45,104,101,105,103,104,116,61,34,50,48,53,48,34,10,32,32,32,32,32,105,100,61,34,110,97,109,101,100,118,105,101,119,49,52,34,10,32,32,32,32,32,115,104,111,119,103,114,105,100,61,34,102,97,108,
115,101,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,122,111,111,109,61,34,52,55,46,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,120,61,34,54,46,56,48,56,53,52,56,34,10,32,32,32,
32,32,105,110,107,115,99,97,112,101,58,99,121,61,34,54,46,52,53,57,53,57,56,51,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,120,61,34,45,49,50,34,10,32,32,32,32,32,
105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,121,61,34,45,49,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,109,97,120,105,109,105,122,101,100,61,34,
49,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,117,114,114,101,110,116,45,108,97,121,101,114,61,34,115,118,103,49,50,34,32,47,62,10,32,32,60,33,45,45,32,71,101,110,101,114,97,116,111,114,
58,32,83,107,101,116,99,104,32,51,46,56,46,49,32,40,50,57,54,56,55,41,32,45,32,104,116,116,112,58,47,47,119,119,119,46,98,111,104,101,109,105,97,110,99,111,100,105,110,103,46,99,111,109,47,115,107,101,
116,99,104,32,45,45,62,10,32,32,60,116,105,116,108,101,10,32,32,32,32,32,105,100,61,34,116,105,116,108,101,50,34,62,115,116,97,116,115,32,91,35,49,49,57,48,93,60,47,116,105,116,108,101,62,10,32,32,60,
100,101,115,99,10,32,32,32,32,32,105,100,61,34,100,101,115,99,52,34,62,67,114,101,97,116,101,100,32,119,105,116,104,32,83,107,101,116,99,104,46,60,47,100,101,115,99,62,10,32,32,60,100,101,102,115,10,32,
32,32,32,32,105,100,61,34,100,101,102,115,54,34,32,47,62,10,32,32,60,103,10,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,45,49,52,48,44,45,50,56,51,57,41,
34,10,32,32,32,32,32,105,100,61,34,68,114,105,98,98,98,108,101,45,76,105,103,104,116,45,80,114,101,118,105,101,119,34,10,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,102,102,102,102,
102,102,59,102,105,108,108,45,114,117,108,101,58,101,118,101,110,111,100,100,59,115,116,114,111,107,101,58,110,111,110,101,59,115,116,114,111,107,101,45,119,105,100,116,104,58,49,59,102,105,108,108,45,
111,112,97,99,105,116,121,58,48,46,57,57,55,50,56,57,57,54,34,62,10,32,32,32,32,60,103,10,32,32,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,53,54,44,49,
54,48,41,34,10,32,32,32,32,32,32,32,105,100,61,34,105,99,111,110,115,34,10,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,102,102,102,102,102,102,59,102,105,108,108,45,111,112,97,
99,105,116,121,58,48,46,57,57,55,50,56,57,57,54,34,62,10,32,32,32,32,32,32,60,112,97,116,104,10,32,32,32,32,32,32,32,32,32,115,111,100,105,112,111,100,105,58,110,111,100,101,116,121,112,101,115,61,34,
99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,34,10,32,32,32,32,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,111,110,110,101,99,116,111,114,45,99,117,114,118,97,116,
117,114,101,61,34,48,34,10,32,32,32,32,32,32,32,32,32,105,100,61,34,115,116,97,116,115,45,91,35,49,49,57,48,93,34,10,32,32,32,32,32,32,32,32,32,100,61,34,109,32,49,48,50,44,50,54,56,56,32,118,32,50,32,
72,32,56,54,32,99,32,45,49,46,49,53,54,44,45,49,46,57,55,49,32,48,46,50,48,54,44,48,46,51,53,32,48,44,48,32,118,32,48,32,48,32,48,32,45,50,32,45,55,32,104,32,49,54,32,122,32,109,32,48,44,57,32,72,32,56,
54,32,118,32,45,55,32,48,32,48,32,99,32,50,46,55,50,51,44,52,46,54,53,52,32,45,49,46,50,57,52,44,45,50,46,50,50,50,32,48,44,48,32,104,32,49,54,32,118,32,48,32,122,32,109,32,45,49,56,44,50,32,104,32,50,
48,32,118,32,45,50,48,32,72,32,56,52,32,90,34,10,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,102,102,102,102,102,102,59,102,105,108,108,45,111,112,97,99,105,116,121,58,48,
46,57,57,55,50,56,57,57,54,34,32,47,62,10,32,32,32,32,60,47,103,62,10,32,32,60,47,103,62,10,32,32,60,112,97,116,104,10,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,110,111,110,101,59,115,
116,114,111,107,101,58,35,102,102,102,102,102,102,59,115,116,114,111,107,101,45,119,105,100,116,104,58,50,59,115,116,114,111,107,101,45,108,105,110,101,99,97,112,58,114,111,117,110,100,59,115,116,114,
111,107,101,45,108,105,110,101,106,111,105,110,58,114,111,117,110,100,59,115,116,114,111,107,101,45,109,105,116,101,114,108,105,109,105,116,58,52,59,115,116,114,111,107,101,45,100,97,115,104,97,114,114,
97,121,58,110,111,110,101,59,115,116,114,111,107,101,45,111,112,97,99,105,116,121,58,49,34,10,32,32,32,32,32,100,61,34,109,32,51,46,57,54,48,51,50,56,54,44,49,53,46,50,53,54,55,57,53,32,99,32,48,44,48,
32,52,46,57,53,54,56,48,53,49,44,45,49,54,46,49,54,51,52,48,51,56,56,32,56,46,51,57,57,48,51,48,52,44,45,56,46,49,48,51,50,53,51,49,32,51,46,52,52,50,50,50,54,44,56,46,48,54,48,49,53,50,49,32,51,46,52,
52,50,50,50,54,44,56,46,48,54,48,49,53,50,49,32,51,46,52,52,50,50,50,54,44,56,46,48,54,48,49,53,50,49,34,10,32,32,32,32,32,105,100,61,34,112,97,116,104,49,52,52,54,34,10,32,32,32,32,32,105,110,107,115,
99,97,112,101,58,99,111,110,110,101,99,116,111,114,45,99,117,114,118,97,116,117,114,101,61,34,48,34,32,47,62,10,60,47,115,118,103,62,10,0,0};

const char* VASTWaveTableEditorComponent::waves_halfsine_svg = (const char*) resource_VASTWaveTableEditorComponent_waves_halfsine_svg;
const int VASTWaveTableEditorComponent::waves_halfsine_svgSize = 2720;

// JUCER_RESOURCE: waves_zeros_svg, 2623, "../../Resources/Icons/waves_zeros.svg"
static const unsigned char resource_VASTWaveTableEditorComponent_waves_zeros_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,
70,45,56,34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,10,32,32,32,120,109,108,110,115,58,100,99,61,34,104,116,116,112,58,47,47,112,117,114,108,46,111,114,103,47,
100,99,47,101,108,101,109,101,110,116,115,47,49,46,49,47,34,10,32,32,32,120,109,108,110,115,58,99,99,61,34,104,116,116,112,58,47,47,99,114,101,97,116,105,118,101,99,111,109,109,111,110,115,46,111,114,
103,47,110,115,35,34,10,32,32,32,120,109,108,110,115,58,114,100,102,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,48,50,47,50,50,45,114,100,102,45,115,121,110,116,
97,120,45,110,115,35,34,10,32,32,32,120,109,108,110,115,58,115,118,103,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,118,103,34,10,32,32,32,120,109,108,110,
115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,118,103,34,10,32,32,32,120,109,108,110,115,58,115,111,100,105,112,111,100,105,61,34,104,116,116,112,58,47,
47,115,111,100,105,112,111,100,105,46,115,111,117,114,99,101,102,111,114,103,101,46,110,101,116,47,68,84,68,47,115,111,100,105,112,111,100,105,45,48,46,100,116,100,34,10,32,32,32,120,109,108,110,115,58,
105,110,107,115,99,97,112,101,61,34,104,116,116,112,58,47,47,119,119,119,46,105,110,107,115,99,97,112,101,46,111,114,103,47,110,97,109,101,115,112,97,99,101,115,47,105,110,107,115,99,97,112,101,34,10,
32,32,32,119,105,100,116,104,61,34,50,48,112,120,34,10,32,32,32,104,101,105,103,104,116,61,34,50,48,112,120,34,10,32,32,32,118,105,101,119,66,111,120,61,34,48,32,48,32,50,48,32,50,48,34,10,32,32,32,118,
101,114,115,105,111,110,61,34,49,46,49,34,10,32,32,32,105,100,61,34,115,118,103,49,50,34,10,32,32,32,115,111,100,105,112,111,100,105,58,100,111,99,110,97,109,101,61,34,119,97,118,101,115,95,122,101,114,
111,115,46,115,118,103,34,10,32,32,32,105,110,107,115,99,97,112,101,58,118,101,114,115,105,111,110,61,34,48,46,57,50,46,51,32,40,50,52,48,53,53,52,54,44,32,50,48,49,56,45,48,51,45,49,49,41,34,62,10,32,
32,60,109,101,116,97,100,97,116,97,10,32,32,32,32,32,105,100,61,34,109,101,116,97,100,97,116,97,49,54,34,62,10,32,32,32,32,60,114,100,102,58,82,68,70,62,10,32,32,32,32,32,32,60,99,99,58,87,111,114,107,
10,32,32,32,32,32,32,32,32,32,114,100,102,58,97,98,111,117,116,61,34,34,62,10,32,32,32,32,32,32,32,32,60,100,99,58,102,111,114,109,97,116,62,105,109,97,103,101,47,115,118,103,43,120,109,108,60,47,100,
99,58,102,111,114,109,97,116,62,10,32,32,32,32,32,32,32,32,60,100,99,58,116,121,112,101,10,32,32,32,32,32,32,32,32,32,32,32,114,100,102,58,114,101,115,111,117,114,99,101,61,34,104,116,116,112,58,47,47,
112,117,114,108,46,111,114,103,47,100,99,47,100,99,109,105,116,121,112,101,47,83,116,105,108,108,73,109,97,103,101,34,32,47,62,10,32,32,32,32,32,32,32,32,60,100,99,58,116,105,116,108,101,62,115,116,97,
116,115,32,91,35,49,49,57,48,93,60,47,100,99,58,116,105,116,108,101,62,10,32,32,32,32,32,32,60,47,99,99,58,87,111,114,107,62,10,32,32,32,32,60,47,114,100,102,58,82,68,70,62,10,32,32,60,47,109,101,116,
97,100,97,116,97,62,10,32,32,60,115,111,100,105,112,111,100,105,58,110,97,109,101,100,118,105,101,119,10,32,32,32,32,32,112,97,103,101,99,111,108,111,114,61,34,35,57,53,57,53,57,53,34,10,32,32,32,32,32,
98,111,114,100,101,114,99,111,108,111,114,61,34,35,54,54,54,54,54,54,34,10,32,32,32,32,32,98,111,114,100,101,114,111,112,97,99,105,116,121,61,34,49,34,10,32,32,32,32,32,111,98,106,101,99,116,116,111,108,
101,114,97,110,99,101,61,34,49,48,34,10,32,32,32,32,32,103,114,105,100,116,111,108,101,114,97,110,99,101,61,34,49,48,34,10,32,32,32,32,32,103,117,105,100,101,116,111,108,101,114,97,110,99,101,61,34,49,
48,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,112,97,103,101,111,112,97,99,105,116,121,61,34,49,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,112,97,103,101,115,104,97,100,111,119,61,
34,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,119,105,100,116,104,61,34,51,56,52,48,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,
45,104,101,105,103,104,116,61,34,50,48,53,48,34,10,32,32,32,32,32,105,100,61,34,110,97,109,101,100,118,105,101,119,49,52,34,10,32,32,32,32,32,115,104,111,119,103,114,105,100,61,34,102,97,108,115,101,34,
10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,122,111,111,109,61,34,52,55,46,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,120,61,34,49,46,52,51,55,55,56,53,51,34,10,32,32,32,32,32,
105,110,107,115,99,97,112,101,58,99,121,61,34,54,46,52,53,57,53,57,56,51,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,120,61,34,45,49,50,34,10,32,32,32,32,32,105,110,
107,115,99,97,112,101,58,119,105,110,100,111,119,45,121,61,34,45,49,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,109,97,120,105,109,105,122,101,100,61,34,49,34,10,
32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,117,114,114,101,110,116,45,108,97,121,101,114,61,34,115,118,103,49,50,34,32,47,62,10,32,32,60,33,45,45,32,71,101,110,101,114,97,116,111,114,58,32,83,
107,101,116,99,104,32,51,46,56,46,49,32,40,50,57,54,56,55,41,32,45,32,104,116,116,112,58,47,47,119,119,119,46,98,111,104,101,109,105,97,110,99,111,100,105,110,103,46,99,111,109,47,115,107,101,116,99,104,
32,45,45,62,10,32,32,60,116,105,116,108,101,10,32,32,32,32,32,105,100,61,34,116,105,116,108,101,50,34,62,115,116,97,116,115,32,91,35,49,49,57,48,93,60,47,116,105,116,108,101,62,10,32,32,60,100,101,115,
99,10,32,32,32,32,32,105,100,61,34,100,101,115,99,52,34,62,67,114,101,97,116,101,100,32,119,105,116,104,32,83,107,101,116,99,104,46,60,47,100,101,115,99,62,10,32,32,60,100,101,102,115,10,32,32,32,32,32,
105,100,61,34,100,101,102,115,54,34,32,47,62,10,32,32,60,103,10,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,45,49,52,48,44,45,50,56,51,57,41,34,10,32,32,
32,32,32,105,100,61,34,68,114,105,98,98,98,108,101,45,76,105,103,104,116,45,80,114,101,118,105,101,119,34,10,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,102,102,102,102,102,102,59,102,
105,108,108,45,114,117,108,101,58,101,118,101,110,111,100,100,59,115,116,114,111,107,101,58,110,111,110,101,59,115,116,114,111,107,101,45,119,105,100,116,104,58,49,59,102,105,108,108,45,111,112,97,99,
105,116,121,58,48,46,57,57,55,50,56,57,57,54,34,62,10,32,32,32,32,60,103,10,32,32,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,53,54,44,49,54,48,41,34,10,
32,32,32,32,32,32,32,105,100,61,34,105,99,111,110,115,34,10,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,102,102,102,102,102,102,59,102,105,108,108,45,111,112,97,99,105,116,121,
58,48,46,57,57,55,50,56,57,57,54,34,62,10,32,32,32,32,32,32,60,112,97,116,104,10,32,32,32,32,32,32,32,32,32,115,111,100,105,112,111,100,105,58,110,111,100,101,116,121,112,101,115,61,34,99,99,99,99,99,
99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,34,10,32,32,32,32,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,111,110,110,101,99,116,111,114,45,99,117,114,118,97,116,117,114,101,61,
34,48,34,10,32,32,32,32,32,32,32,32,32,105,100,61,34,115,116,97,116,115,45,91,35,49,49,57,48,93,34,10,32,32,32,32,32,32,32,32,32,100,61,34,109,32,49,48,50,44,50,54,56,56,32,118,32,50,32,72,32,56,54,32,
99,32,45,49,46,49,53,54,44,45,49,46,57,55,49,32,48,46,50,48,54,44,48,46,51,53,32,48,44,48,32,118,32,48,32,48,32,48,32,45,50,32,45,55,32,104,32,49,54,32,122,32,109,32,48,44,57,32,72,32,56,54,32,118,32,
45,55,32,48,32,48,32,99,32,50,46,55,50,51,44,52,46,54,53,52,32,45,49,46,50,57,52,44,45,50,46,50,50,50,32,48,44,48,32,104,32,49,54,32,118,32,48,32,122,32,109,32,45,49,56,44,50,32,104,32,50,48,32,118,32,
45,50,48,32,72,32,56,52,32,90,34,10,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,102,102,102,102,102,102,59,102,105,108,108,45,111,112,97,99,105,116,121,58,48,46,57,57,55,
50,56,57,57,54,34,32,47,62,10,32,32,32,32,60,47,103,62,10,32,32,60,47,103,62,10,32,32,60,112,97,116,104,10,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,110,111,110,101,59,115,116,114,111,
107,101,58,35,101,101,101,101,101,101,59,115,116,114,111,107,101,45,119,105,100,116,104,58,50,59,115,116,114,111,107,101,45,108,105,110,101,99,97,112,58,114,111,117,110,100,59,115,116,114,111,107,101,
45,108,105,110,101,106,111,105,110,58,114,111,117,110,100,59,115,116,114,111,107,101,45,111,112,97,99,105,116,121,58,49,59,115,116,114,111,107,101,45,109,105,116,101,114,108,105,109,105,116,58,52,59,115,
116,114,111,107,101,45,100,97,115,104,97,114,114,97,121,58,110,111,110,101,34,10,32,32,32,32,32,100,61,34,77,32,51,46,57,56,51,48,53,48,56,44,57,46,57,55,56,56,49,51,54,32,72,32,49,53,46,54,49,52,52,48,
55,34,10,32,32,32,32,32,105,100,61,34,112,97,116,104,50,48,48,55,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,111,110,110,101,99,116,111,114,45,99,117,114,118,97,116,117,114,101,61,34,48,34,
32,47,62,10,60,47,115,118,103,62,10,0,0};

const char* VASTWaveTableEditorComponent::waves_zeros_svg = (const char*) resource_VASTWaveTableEditorComponent_waves_zeros_svg;
const int VASTWaveTableEditorComponent::waves_zeros_svgSize = 2623;

// JUCER_RESOURCE: waves_random_svg, 2908, "../../Resources/Icons/waves_random.svg"
static const unsigned char resource_VASTWaveTableEditorComponent_waves_random_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,
70,45,56,34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,10,32,32,32,120,109,108,110,115,58,100,99,61,34,104,116,116,112,58,47,47,112,117,114,108,46,111,114,103,47,
100,99,47,101,108,101,109,101,110,116,115,47,49,46,49,47,34,10,32,32,32,120,109,108,110,115,58,99,99,61,34,104,116,116,112,58,47,47,99,114,101,97,116,105,118,101,99,111,109,109,111,110,115,46,111,114,
103,47,110,115,35,34,10,32,32,32,120,109,108,110,115,58,114,100,102,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,48,50,47,50,50,45,114,100,102,45,115,121,110,116,
97,120,45,110,115,35,34,10,32,32,32,120,109,108,110,115,58,115,118,103,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,118,103,34,10,32,32,32,120,109,108,110,
115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,118,103,34,10,32,32,32,120,109,108,110,115,58,115,111,100,105,112,111,100,105,61,34,104,116,116,112,58,47,
47,115,111,100,105,112,111,100,105,46,115,111,117,114,99,101,102,111,114,103,101,46,110,101,116,47,68,84,68,47,115,111,100,105,112,111,100,105,45,48,46,100,116,100,34,10,32,32,32,120,109,108,110,115,58,
105,110,107,115,99,97,112,101,61,34,104,116,116,112,58,47,47,119,119,119,46,105,110,107,115,99,97,112,101,46,111,114,103,47,110,97,109,101,115,112,97,99,101,115,47,105,110,107,115,99,97,112,101,34,10,
32,32,32,119,105,100,116,104,61,34,50,48,112,120,34,10,32,32,32,104,101,105,103,104,116,61,34,50,48,112,120,34,10,32,32,32,118,105,101,119,66,111,120,61,34,48,32,48,32,50,48,32,50,48,34,10,32,32,32,118,
101,114,115,105,111,110,61,34,49,46,49,34,10,32,32,32,105,100,61,34,115,118,103,49,50,34,10,32,32,32,115,111,100,105,112,111,100,105,58,100,111,99,110,97,109,101,61,34,119,97,118,101,115,95,114,97,110,
100,111,109,46,115,118,103,34,10,32,32,32,105,110,107,115,99,97,112,101,58,118,101,114,115,105,111,110,61,34,48,46,57,50,46,51,32,40,50,52,48,53,53,52,54,44,32,50,48,49,56,45,48,51,45,49,49,41,34,62,10,
32,32,60,109,101,116,97,100,97,116,97,10,32,32,32,32,32,105,100,61,34,109,101,116,97,100,97,116,97,49,54,34,62,10,32,32,32,32,60,114,100,102,58,82,68,70,62,10,32,32,32,32,32,32,60,99,99,58,87,111,114,
107,10,32,32,32,32,32,32,32,32,32,114,100,102,58,97,98,111,117,116,61,34,34,62,10,32,32,32,32,32,32,32,32,60,100,99,58,102,111,114,109,97,116,62,105,109,97,103,101,47,115,118,103,43,120,109,108,60,47,
100,99,58,102,111,114,109,97,116,62,10,32,32,32,32,32,32,32,32,60,100,99,58,116,121,112,101,10,32,32,32,32,32,32,32,32,32,32,32,114,100,102,58,114,101,115,111,117,114,99,101,61,34,104,116,116,112,58,47,
47,112,117,114,108,46,111,114,103,47,100,99,47,100,99,109,105,116,121,112,101,47,83,116,105,108,108,73,109,97,103,101,34,32,47,62,10,32,32,32,32,32,32,32,32,60,100,99,58,116,105,116,108,101,62,115,116,
97,116,115,32,91,35,49,49,57,48,93,60,47,100,99,58,116,105,116,108,101,62,10,32,32,32,32,32,32,60,47,99,99,58,87,111,114,107,62,10,32,32,32,32,60,47,114,100,102,58,82,68,70,62,10,32,32,60,47,109,101,116,
97,100,97,116,97,62,10,32,32,60,115,111,100,105,112,111,100,105,58,110,97,109,101,100,118,105,101,119,10,32,32,32,32,32,112,97,103,101,99,111,108,111,114,61,34,35,57,53,57,53,57,53,34,10,32,32,32,32,32,
98,111,114,100,101,114,99,111,108,111,114,61,34,35,54,54,54,54,54,54,34,10,32,32,32,32,32,98,111,114,100,101,114,111,112,97,99,105,116,121,61,34,49,34,10,32,32,32,32,32,111,98,106,101,99,116,116,111,108,
101,114,97,110,99,101,61,34,49,48,34,10,32,32,32,32,32,103,114,105,100,116,111,108,101,114,97,110,99,101,61,34,49,48,34,10,32,32,32,32,32,103,117,105,100,101,116,111,108,101,114,97,110,99,101,61,34,49,
48,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,112,97,103,101,111,112,97,99,105,116,121,61,34,49,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,112,97,103,101,115,104,97,100,111,119,61,
34,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,119,105,100,116,104,61,34,51,56,52,48,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,
45,104,101,105,103,104,116,61,34,50,48,53,48,34,10,32,32,32,32,32,105,100,61,34,110,97,109,101,100,118,105,101,119,49,52,34,10,32,32,32,32,32,115,104,111,119,103,114,105,100,61,34,102,97,108,115,101,34,
10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,122,111,111,109,61,34,52,55,46,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,120,61,34,49,46,52,51,55,55,56,53,51,34,10,32,32,32,32,32,
105,110,107,115,99,97,112,101,58,99,121,61,34,54,46,52,53,57,53,57,56,51,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,120,61,34,45,49,50,34,10,32,32,32,32,32,105,110,
107,115,99,97,112,101,58,119,105,110,100,111,119,45,121,61,34,45,49,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,109,97,120,105,109,105,122,101,100,61,34,49,34,10,
32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,117,114,114,101,110,116,45,108,97,121,101,114,61,34,115,118,103,49,50,34,32,47,62,10,32,32,60,33,45,45,32,71,101,110,101,114,97,116,111,114,58,32,83,
107,101,116,99,104,32,51,46,56,46,49,32,40,50,57,54,56,55,41,32,45,32,104,116,116,112,58,47,47,119,119,119,46,98,111,104,101,109,105,97,110,99,111,100,105,110,103,46,99,111,109,47,115,107,101,116,99,104,
32,45,45,62,10,32,32,60,116,105,116,108,101,10,32,32,32,32,32,105,100,61,34,116,105,116,108,101,50,34,62,115,116,97,116,115,32,91,35,49,49,57,48,93,60,47,116,105,116,108,101,62,10,32,32,60,100,101,115,
99,10,32,32,32,32,32,105,100,61,34,100,101,115,99,52,34,62,67,114,101,97,116,101,100,32,119,105,116,104,32,83,107,101,116,99,104,46,60,47,100,101,115,99,62,10,32,32,60,100,101,102,115,10,32,32,32,32,32,
105,100,61,34,100,101,102,115,54,34,32,47,62,10,32,32,60,103,10,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,45,49,52,48,44,45,50,56,51,57,41,34,10,32,32,
32,32,32,105,100,61,34,68,114,105,98,98,98,108,101,45,76,105,103,104,116,45,80,114,101,118,105,101,119,34,10,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,102,102,102,102,102,102,59,102,
105,108,108,45,114,117,108,101,58,101,118,101,110,111,100,100,59,115,116,114,111,107,101,58,110,111,110,101,59,115,116,114,111,107,101,45,119,105,100,116,104,58,49,59,102,105,108,108,45,111,112,97,99,
105,116,121,58,48,46,57,57,55,50,56,57,57,54,34,62,10,32,32,32,32,60,103,10,32,32,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,53,54,44,49,54,48,41,34,10,
32,32,32,32,32,32,32,105,100,61,34,105,99,111,110,115,34,10,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,102,102,102,102,102,102,59,102,105,108,108,45,111,112,97,99,105,116,121,
58,48,46,57,57,55,50,56,57,57,54,34,62,10,32,32,32,32,32,32,60,112,97,116,104,10,32,32,32,32,32,32,32,32,32,115,111,100,105,112,111,100,105,58,110,111,100,101,116,121,112,101,115,61,34,99,99,99,99,99,
99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,34,10,32,32,32,32,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,111,110,110,101,99,116,111,114,45,99,117,114,118,97,116,117,114,101,61,
34,48,34,10,32,32,32,32,32,32,32,32,32,105,100,61,34,115,116,97,116,115,45,91,35,49,49,57,48,93,34,10,32,32,32,32,32,32,32,32,32,100,61,34,109,32,49,48,50,44,50,54,56,56,32,118,32,50,32,72,32,56,54,32,
99,32,45,49,46,49,53,54,44,45,49,46,57,55,49,32,48,46,50,48,54,44,48,46,51,53,32,48,44,48,32,118,32,48,32,48,32,48,32,45,50,32,45,55,32,104,32,49,54,32,122,32,109,32,48,44,57,32,72,32,56,54,32,118,32,
45,55,32,48,32,48,32,99,32,50,46,55,50,51,44,52,46,54,53,52,32,45,49,46,50,57,52,44,45,50,46,50,50,50,32,48,44,48,32,104,32,49,54,32,118,32,48,32,122,32,109,32,45,49,56,44,50,32,104,32,50,48,32,118,32,
45,50,48,32,72,32,56,52,32,90,34,10,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,102,102,102,102,102,102,59,102,105,108,108,45,111,112,97,99,105,116,121,58,48,46,57,57,55,
50,56,57,57,54,34,32,47,62,10,32,32,32,32,60,47,103,62,10,32,32,60,47,103,62,10,32,32,60,112,97,116,104,10,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,110,111,110,101,59,115,116,114,111,
107,101,58,35,102,102,102,102,102,102,59,115,116,114,111,107,101,45,119,105,100,116,104,58,49,59,115,116,114,111,107,101,45,108,105,110,101,99,97,112,58,98,117,116,116,59,115,116,114,111,107,101,45,108,
105,110,101,106,111,105,110,58,109,105,116,101,114,59,115,116,114,111,107,101,45,111,112,97,99,105,116,121,58,49,59,115,116,114,111,107,101,45,109,105,116,101,114,108,105,109,105,116,58,52,59,115,116,
114,111,107,101,45,100,97,115,104,97,114,114,97,121,58,110,111,110,101,34,10,32,32,32,32,32,100,61,34,77,32,50,46,57,50,51,55,50,56,56,44,49,48,46,55,50,48,51,51,57,32,51,46,53,53,57,51,50,50,44,51,46,
52,49,49,48,49,54,57,32,52,46,51,52,51,50,50,48,51,44,49,54,46,56,50,50,48,51,52,32,53,46,51,51,56,57,56,51,49,44,51,46,52,57,53,55,54,50,55,32,54,46,49,54,53,50,53,52,50,44,49,54,46,53,54,55,55,57,55,
32,55,46,52,49,53,50,53,52,50,44,51,46,51,56,57,56,51,48,53,32,56,46,52,55,52,53,55,54,51,44,49,54,46,54,49,48,49,54,57,32,57,46,53,53,53,48,56,52,55,44,51,46,51,54,56,54,52,52,49,32,49,48,46,50,55,53,
52,50,52,44,49,54,46,53,50,53,52,50,52,32,49,49,46,54,57,52,57,49,53,44,51,46,53,51,56,49,51,53,54,32,49,50,46,52,53,55,54,50,55,44,49,54,46,48,49,54,57,52,57,32,49,51,46,52,53,51,51,57,44,51,46,52,53,
51,51,56,57,56,32,49,52,46,49,57,52,57,49,53,44,49,54,46,51,53,53,57,51,50,32,49,53,46,56,48,53,48,56,53,44,51,46,49,49,52,52,48,54,56,32,49,54,46,53,54,55,55,57,55,44,49,48,46,57,53,51,51,57,32,49,54,
46,53,48,52,50,51,55,44,49,48,46,56,52,55,52,53,56,34,10,32,32,32,32,32,105,100,61,34,112,97,116,104,50,48,49,49,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,111,110,110,101,99,116,111,114,
45,99,117,114,118,97,116,117,114,101,61,34,48,34,32,47,62,10,60,47,115,118,103,62,10,0,0};

const char* VASTWaveTableEditorComponent::waves_random_svg = (const char*) resource_VASTWaveTableEditorComponent_waves_random_svg;
const int VASTWaveTableEditorComponent::waves_random_svgSize = 2908;

// JUCER_RESOURCE: waves_sinc_svg, 3208, "../../Resources/Icons/waves_sinc.svg"
static const unsigned char resource_VASTWaveTableEditorComponent_waves_sinc_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,70,
45,56,34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,10,32,32,32,120,109,108,110,115,58,100,99,61,34,104,116,116,112,58,47,47,112,117,114,108,46,111,114,103,47,100,
99,47,101,108,101,109,101,110,116,115,47,49,46,49,47,34,10,32,32,32,120,109,108,110,115,58,99,99,61,34,104,116,116,112,58,47,47,99,114,101,97,116,105,118,101,99,111,109,109,111,110,115,46,111,114,103,
47,110,115,35,34,10,32,32,32,120,109,108,110,115,58,114,100,102,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,48,50,47,50,50,45,114,100,102,45,115,121,110,116,97,
120,45,110,115,35,34,10,32,32,32,120,109,108,110,115,58,115,118,103,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,118,103,34,10,32,32,32,120,109,108,110,115,
61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,118,103,34,10,32,32,32,120,109,108,110,115,58,115,111,100,105,112,111,100,105,61,34,104,116,116,112,58,47,47,115,
111,100,105,112,111,100,105,46,115,111,117,114,99,101,102,111,114,103,101,46,110,101,116,47,68,84,68,47,115,111,100,105,112,111,100,105,45,48,46,100,116,100,34,10,32,32,32,120,109,108,110,115,58,105,110,
107,115,99,97,112,101,61,34,104,116,116,112,58,47,47,119,119,119,46,105,110,107,115,99,97,112,101,46,111,114,103,47,110,97,109,101,115,112,97,99,101,115,47,105,110,107,115,99,97,112,101,34,10,32,32,32,
119,105,100,116,104,61,34,50,48,112,120,34,10,32,32,32,104,101,105,103,104,116,61,34,50,48,112,120,34,10,32,32,32,118,105,101,119,66,111,120,61,34,48,32,48,32,50,48,32,50,48,34,10,32,32,32,118,101,114,
115,105,111,110,61,34,49,46,49,34,10,32,32,32,105,100,61,34,115,118,103,49,50,34,10,32,32,32,115,111,100,105,112,111,100,105,58,100,111,99,110,97,109,101,61,34,119,97,118,101,115,95,115,105,110,99,46,
115,118,103,34,10,32,32,32,105,110,107,115,99,97,112,101,58,118,101,114,115,105,111,110,61,34,48,46,57,50,46,51,32,40,50,52,48,53,53,52,54,44,32,50,48,49,56,45,48,51,45,49,49,41,34,62,10,32,32,60,109,
101,116,97,100,97,116,97,10,32,32,32,32,32,105,100,61,34,109,101,116,97,100,97,116,97,49,54,34,62,10,32,32,32,32,60,114,100,102,58,82,68,70,62,10,32,32,32,32,32,32,60,99,99,58,87,111,114,107,10,32,32,
32,32,32,32,32,32,32,114,100,102,58,97,98,111,117,116,61,34,34,62,10,32,32,32,32,32,32,32,32,60,100,99,58,102,111,114,109,97,116,62,105,109,97,103,101,47,115,118,103,43,120,109,108,60,47,100,99,58,102,
111,114,109,97,116,62,10,32,32,32,32,32,32,32,32,60,100,99,58,116,121,112,101,10,32,32,32,32,32,32,32,32,32,32,32,114,100,102,58,114,101,115,111,117,114,99,101,61,34,104,116,116,112,58,47,47,112,117,114,
108,46,111,114,103,47,100,99,47,100,99,109,105,116,121,112,101,47,83,116,105,108,108,73,109,97,103,101,34,32,47,62,10,32,32,32,32,32,32,32,32,60,100,99,58,116,105,116,108,101,62,115,116,97,116,115,32,
91,35,49,49,57,48,93,60,47,100,99,58,116,105,116,108,101,62,10,32,32,32,32,32,32,60,47,99,99,58,87,111,114,107,62,10,32,32,32,32,60,47,114,100,102,58,82,68,70,62,10,32,32,60,47,109,101,116,97,100,97,116,
97,62,10,32,32,60,115,111,100,105,112,111,100,105,58,110,97,109,101,100,118,105,101,119,10,32,32,32,32,32,112,97,103,101,99,111,108,111,114,61,34,35,57,53,57,53,57,53,34,10,32,32,32,32,32,98,111,114,100,
101,114,99,111,108,111,114,61,34,35,54,54,54,54,54,54,34,10,32,32,32,32,32,98,111,114,100,101,114,111,112,97,99,105,116,121,61,34,49,34,10,32,32,32,32,32,111,98,106,101,99,116,116,111,108,101,114,97,110,
99,101,61,34,49,48,34,10,32,32,32,32,32,103,114,105,100,116,111,108,101,114,97,110,99,101,61,34,49,48,34,10,32,32,32,32,32,103,117,105,100,101,116,111,108,101,114,97,110,99,101,61,34,49,48,34,10,32,32,
32,32,32,105,110,107,115,99,97,112,101,58,112,97,103,101,111,112,97,99,105,116,121,61,34,49,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,112,97,103,101,115,104,97,100,111,119,61,34,50,34,10,32,
32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,119,105,100,116,104,61,34,51,56,52,48,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,104,101,105,
103,104,116,61,34,50,48,53,48,34,10,32,32,32,32,32,105,100,61,34,110,97,109,101,100,118,105,101,119,49,52,34,10,32,32,32,32,32,115,104,111,119,103,114,105,100,61,34,102,97,108,115,101,34,10,32,32,32,32,
32,105,110,107,115,99,97,112,101,58,122,111,111,109,61,34,52,55,46,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,120,61,34,49,46,52,51,55,55,56,53,51,34,10,32,32,32,32,32,105,110,107,115,
99,97,112,101,58,99,121,61,34,54,46,52,53,57,53,57,56,51,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,120,61,34,45,49,50,34,10,32,32,32,32,32,105,110,107,115,99,97,
112,101,58,119,105,110,100,111,119,45,121,61,34,45,49,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,109,97,120,105,109,105,122,101,100,61,34,49,34,10,32,32,32,32,
32,105,110,107,115,99,97,112,101,58,99,117,114,114,101,110,116,45,108,97,121,101,114,61,34,115,118,103,49,50,34,10,32,32,32,32,32,115,104,111,119,103,117,105,100,101,115,61,34,102,97,108,115,101,34,32,
47,62,10,32,32,60,33,45,45,32,71,101,110,101,114,97,116,111,114,58,32,83,107,101,116,99,104,32,51,46,56,46,49,32,40,50,57,54,56,55,41,32,45,32,104,116,116,112,58,47,47,119,119,119,46,98,111,104,101,109,
105,97,110,99,111,100,105,110,103,46,99,111,109,47,115,107,101,116,99,104,32,45,45,62,10,32,32,60,116,105,116,108,101,10,32,32,32,32,32,105,100,61,34,116,105,116,108,101,50,34,62,115,116,97,116,115,32,
91,35,49,49,57,48,93,60,47,116,105,116,108,101,62,10,32,32,60,100,101,115,99,10,32,32,32,32,32,105,100,61,34,100,101,115,99,52,34,62,67,114,101,97,116,101,100,32,119,105,116,104,32,83,107,101,116,99,104,
46,60,47,100,101,115,99,62,10,32,32,60,100,101,102,115,10,32,32,32,32,32,105,100,61,34,100,101,102,115,54,34,32,47,62,10,32,32,60,103,10,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,116,114,
97,110,115,108,97,116,101,40,45,49,52,48,44,45,50,56,51,57,41,34,10,32,32,32,32,32,105,100,61,34,68,114,105,98,98,98,108,101,45,76,105,103,104,116,45,80,114,101,118,105,101,119,34,10,32,32,32,32,32,115,
116,121,108,101,61,34,102,105,108,108,58,35,102,102,102,102,102,102,59,102,105,108,108,45,114,117,108,101,58,101,118,101,110,111,100,100,59,115,116,114,111,107,101,58,110,111,110,101,59,115,116,114,111,
107,101,45,119,105,100,116,104,58,49,59,102,105,108,108,45,111,112,97,99,105,116,121,58,48,46,57,57,55,50,56,57,57,54,34,62,10,32,32,32,32,60,103,10,32,32,32,32,32,32,32,116,114,97,110,115,102,111,114,
109,61,34,116,114,97,110,115,108,97,116,101,40,53,54,44,49,54,48,41,34,10,32,32,32,32,32,32,32,105,100,61,34,105,99,111,110,115,34,10,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,
35,102,102,102,102,102,102,59,102,105,108,108,45,111,112,97,99,105,116,121,58,48,46,57,57,55,50,56,57,57,54,34,62,10,32,32,32,32,32,32,60,112,97,116,104,10,32,32,32,32,32,32,32,32,32,115,111,100,105,112,
111,100,105,58,110,111,100,101,116,121,112,101,115,61,34,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,34,10,32,32,32,32,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,
111,110,110,101,99,116,111,114,45,99,117,114,118,97,116,117,114,101,61,34,48,34,10,32,32,32,32,32,32,32,32,32,105,100,61,34,115,116,97,116,115,45,91,35,49,49,57,48,93,34,10,32,32,32,32,32,32,32,32,32,
100,61,34,109,32,49,48,50,44,50,54,56,56,32,118,32,50,32,72,32,56,54,32,99,32,45,49,46,49,53,54,44,45,49,46,57,55,49,32,48,46,50,48,54,44,48,46,51,53,32,48,44,48,32,118,32,48,32,48,32,48,32,45,50,32,45,
55,32,104,32,49,54,32,122,32,109,32,48,44,57,32,72,32,56,54,32,118,32,45,55,32,48,32,48,32,99,32,50,46,55,50,51,44,52,46,54,53,52,32,45,49,46,50,57,52,44,45,50,46,50,50,50,32,48,44,48,32,104,32,49,54,
32,118,32,48,32,122,32,109,32,45,49,56,44,50,32,104,32,50,48,32,118,32,45,50,48,32,72,32,56,52,32,90,34,10,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,102,102,102,102,102,
102,59,102,105,108,108,45,111,112,97,99,105,116,121,58,48,46,57,57,55,50,56,57,57,54,34,32,47,62,10,32,32,32,32,60,47,103,62,10,32,32,60,47,103,62,10,32,32,60,112,97,116,104,10,32,32,32,32,32,115,116,
121,108,101,61,34,102,105,108,108,58,110,111,110,101,59,115,116,114,111,107,101,58,35,102,102,102,102,102,102,59,115,116,114,111,107,101,45,119,105,100,116,104,58,49,59,115,116,114,111,107,101,45,108,
105,110,101,99,97,112,58,98,117,116,116,59,115,116,114,111,107,101,45,108,105,110,101,106,111,105,110,58,109,105,116,101,114,59,115,116,114,111,107,101,45,109,105,116,101,114,108,105,109,105,116,58,52,
59,115,116,114,111,107,101,45,100,97,115,104,97,114,114,97,121,58,110,111,110,101,59,115,116,114,111,107,101,45,111,112,97,99,105,116,121,58,49,34,10,32,32,32,32,32,100,61,34,109,32,49,54,46,54,49,48,
54,52,55,44,49,49,46,49,48,49,54,57,53,32,99,32,48,44,48,32,51,101,45,54,44,48,46,50,55,53,52,50,32,45,48,46,51,55,49,53,51,44,49,46,53,56,56,57,56,51,32,45,48,46,51,55,49,53,51,49,44,49,46,51,49,51,53,
53,57,32,45,49,46,50,50,48,55,52,54,44,45,48,46,54,49,52,52,48,54,32,45,49,46,57,49,48,55,51,50,44,45,51,46,56,55,55,49,49,56,55,32,45,48,46,54,56,57,57,56,54,44,45,51,46,50,54,50,55,49,49,57,32,45,49,
46,51,55,57,57,55,50,44,51,46,49,51,53,53,57,51,55,32,45,49,46,57,50,56,52,50,51,44,54,46,51,49,51,53,53,57,55,32,45,48,46,53,52,56,52,53,44,51,46,49,55,55,57,54,54,32,45,49,46,51,52,52,53,56,56,44,45,
51,46,50,50,48,51,51,57,32,45,49,46,51,52,52,53,56,56,44,45,51,46,50,50,48,51,51,57,32,76,32,49,48,46,53,53,57,57,57,57,44,54,46,53,48,52,50,51,55,51,32,67,32,57,46,57,49,54,50,56,49,50,44,45,48,46,53,
49,54,49,50,52,48,51,32,56,46,54,56,52,54,53,49,56,44,49,49,46,54,55,51,55,50,56,32,56,46,54,56,52,54,53,49,56,44,49,49,46,54,55,51,55,50,56,32,99,32,48,44,48,32,45,49,46,49,51,50,50,56,53,50,44,54,46,
56,52,51,50,50,49,32,45,49,46,54,50,55,54,54,44,51,46,51,50,54,50,55,50,32,67,32,54,46,53,54,49,54,49,55,49,44,49,49,46,52,56,51,48,53,49,32,54,46,50,57,54,50,51,55,56,44,53,46,57,49,49,48,49,54,57,32,
53,46,54,48,54,50,53,49,53,44,57,46,48,54,55,55,57,54,54,32,52,46,57,49,54,50,54,53,50,44,49,50,46,50,50,52,53,55,54,32,52,46,56,57,56,53,55,51,50,44,49,50,46,50,50,52,53,55,54,32,52,46,56,57,56,53,55,
51,50,44,49,50,46,50,50,52,53,55,54,32,99,32,48,44,48,32,45,48,46,56,51,49,55,57,50,52,44,50,46,55,53,51,56,56,56,32,45,49,46,50,51,56,52,51,54,57,44,48,46,54,51,53,53,57,52,32,76,32,51,46,51,48,54,50,
57,55,50,44,49,49,46,48,49,54,57,52,57,34,10,32,32,32,32,32,105,100,61,34,112,97,116,104,50,48,49,53,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,111,110,110,101,99,116,111,114,45,99,117,114,
118,97,116,117,114,101,61,34,48,34,10,32,32,32,32,32,115,111,100,105,112,111,100,105,58,110,111,100,101,116,121,112,101,115,61,34,99,99,115,115,99,115,99,115,115,99,115,99,34,32,47,62,10,60,47,115,118,
103,62,10,0,0};

const char* VASTWaveTableEditorComponent::waves_sinc_svg = (const char*) resource_VASTWaveTableEditorComponent_waves_sinc_svg;
const int VASTWaveTableEditorComponent::waves_sinc_svgSize = 3208;

// JUCER_RESOURCE: arc_connect_svg, 3051, "../../Resources/Icons/arc_connect.svg"
static const unsigned char resource_VASTWaveTableEditorComponent_arc_connect_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,
70,45,56,34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,10,32,32,32,120,109,108,110,115,58,100,99,61,34,104,116,116,112,58,47,47,112,117,114,108,46,111,114,103,47,
100,99,47,101,108,101,109,101,110,116,115,47,49,46,49,47,34,10,32,32,32,120,109,108,110,115,58,99,99,61,34,104,116,116,112,58,47,47,99,114,101,97,116,105,118,101,99,111,109,109,111,110,115,46,111,114,
103,47,110,115,35,34,10,32,32,32,120,109,108,110,115,58,114,100,102,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,48,50,47,50,50,45,114,100,102,45,115,121,110,116,
97,120,45,110,115,35,34,10,32,32,32,120,109,108,110,115,58,115,118,103,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,118,103,34,10,32,32,32,120,109,108,110,
115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,118,103,34,10,32,32,32,120,109,108,110,115,58,115,111,100,105,112,111,100,105,61,34,104,116,116,112,58,47,
47,115,111,100,105,112,111,100,105,46,115,111,117,114,99,101,102,111,114,103,101,46,110,101,116,47,68,84,68,47,115,111,100,105,112,111,100,105,45,48,46,100,116,100,34,10,32,32,32,120,109,108,110,115,58,
105,110,107,115,99,97,112,101,61,34,104,116,116,112,58,47,47,119,119,119,46,105,110,107,115,99,97,112,101,46,111,114,103,47,110,97,109,101,115,112,97,99,101,115,47,105,110,107,115,99,97,112,101,34,10,
32,32,32,119,105,100,116,104,61,34,50,48,112,120,34,10,32,32,32,104,101,105,103,104,116,61,34,50,48,112,120,34,10,32,32,32,118,105,101,119,66,111,120,61,34,48,32,48,32,50,48,32,50,48,34,10,32,32,32,118,
101,114,115,105,111,110,61,34,49,46,49,34,10,32,32,32,105,100,61,34,115,118,103,49,50,34,10,32,32,32,115,111,100,105,112,111,100,105,58,100,111,99,110,97,109,101,61,34,97,114,99,95,99,111,110,110,101,
99,116,46,115,118,103,34,10,32,32,32,105,110,107,115,99,97,112,101,58,118,101,114,115,105,111,110,61,34,48,46,57,50,46,51,32,40,50,52,48,53,53,52,54,44,32,50,48,49,56,45,48,51,45,49,49,41,34,62,10,32,
32,60,109,101,116,97,100,97,116,97,10,32,32,32,32,32,105,100,61,34,109,101,116,97,100,97,116,97,49,54,34,62,10,32,32,32,32,60,114,100,102,58,82,68,70,62,10,32,32,32,32,32,32,60,99,99,58,87,111,114,107,
10,32,32,32,32,32,32,32,32,32,114,100,102,58,97,98,111,117,116,61,34,34,62,10,32,32,32,32,32,32,32,32,60,100,99,58,102,111,114,109,97,116,62,105,109,97,103,101,47,115,118,103,43,120,109,108,60,47,100,
99,58,102,111,114,109,97,116,62,10,32,32,32,32,32,32,32,32,60,100,99,58,116,121,112,101,10,32,32,32,32,32,32,32,32,32,32,32,114,100,102,58,114,101,115,111,117,114,99,101,61,34,104,116,116,112,58,47,47,
112,117,114,108,46,111,114,103,47,100,99,47,100,99,109,105,116,121,112,101,47,83,116,105,108,108,73,109,97,103,101,34,32,47,62,10,32,32,32,32,32,32,32,32,60,100,99,58,116,105,116,108,101,62,115,116,97,
116,115,32,91,35,49,49,57,48,93,60,47,100,99,58,116,105,116,108,101,62,10,32,32,32,32,32,32,60,47,99,99,58,87,111,114,107,62,10,32,32,32,32,60,47,114,100,102,58,82,68,70,62,10,32,32,60,47,109,101,116,
97,100,97,116,97,62,10,32,32,60,115,111,100,105,112,111,100,105,58,110,97,109,101,100,118,105,101,119,10,32,32,32,32,32,112,97,103,101,99,111,108,111,114,61,34,35,57,53,57,53,57,53,34,10,32,32,32,32,32,
98,111,114,100,101,114,99,111,108,111,114,61,34,35,54,54,54,54,54,54,34,10,32,32,32,32,32,98,111,114,100,101,114,111,112,97,99,105,116,121,61,34,49,34,10,32,32,32,32,32,111,98,106,101,99,116,116,111,108,
101,114,97,110,99,101,61,34,49,48,34,10,32,32,32,32,32,103,114,105,100,116,111,108,101,114,97,110,99,101,61,34,49,48,34,10,32,32,32,32,32,103,117,105,100,101,116,111,108,101,114,97,110,99,101,61,34,49,
48,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,112,97,103,101,111,112,97,99,105,116,121,61,34,49,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,112,97,103,101,115,104,97,100,111,119,61,
34,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,119,105,100,116,104,61,34,51,56,52,48,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,
45,104,101,105,103,104,116,61,34,50,48,53,48,34,10,32,32,32,32,32,105,100,61,34,110,97,109,101,100,118,105,101,119,49,52,34,10,32,32,32,32,32,115,104,111,119,103,114,105,100,61,34,102,97,108,115,101,34,
10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,122,111,111,109,61,34,54,54,46,55,53,48,56,56,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,120,61,34,56,46,52,53,50,56,50,52,53,34,10,32,
32,32,32,32,105,110,107,115,99,97,112,101,58,99,121,61,34,55,46,52,48,51,56,54,49,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,120,61,34,45,49,50,34,10,32,32,32,
32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,121,61,34,45,49,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,109,97,120,105,109,105,122,101,100,
61,34,49,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,117,114,114,101,110,116,45,108,97,121,101,114,61,34,115,118,103,49,50,34,32,47,62,10,32,32,60,33,45,45,32,71,101,110,101,114,97,116,111,
114,58,32,83,107,101,116,99,104,32,51,46,56,46,49,32,40,50,57,54,56,55,41,32,45,32,104,116,116,112,58,47,47,119,119,119,46,98,111,104,101,109,105,97,110,99,111,100,105,110,103,46,99,111,109,47,115,107,
101,116,99,104,32,45,45,62,10,32,32,60,116,105,116,108,101,10,32,32,32,32,32,105,100,61,34,116,105,116,108,101,50,34,62,115,116,97,116,115,32,91,35,49,49,57,48,93,60,47,116,105,116,108,101,62,10,32,32,
60,100,101,115,99,10,32,32,32,32,32,105,100,61,34,100,101,115,99,52,34,62,67,114,101,97,116,101,100,32,119,105,116,104,32,83,107,101,116,99,104,46,60,47,100,101,115,99,62,10,32,32,60,100,101,102,115,10,
32,32,32,32,32,105,100,61,34,100,101,102,115,54,34,32,47,62,10,32,32,60,103,10,32,32,32,32,32,105,100,61,34,103,56,50,56,34,10,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,109,97,116,114,105,
120,40,48,46,56,52,50,57,49,51,54,49,44,48,44,48,44,48,46,55,52,54,54,56,49,56,52,44,45,48,46,49,56,55,55,48,48,52,52,44,51,46,51,56,49,55,49,51,52,41,34,10,32,32,32,32,32,115,116,121,108,101,61,34,115,
116,114,111,107,101,45,119,105,100,116,104,58,49,46,50,54,48,52,57,51,55,54,34,62,10,32,32,32,32,60,101,108,108,105,112,115,101,10,32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,56,49,57,34,10,32,32,
32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,102,102,102,102,102,102,59,102,105,108,108,45,111,112,97,99,105,116,121,58,48,46,48,54,53,48,52,48,54,56,59,115,116,114,111,107,101,58,35,
102,102,102,102,102,102,59,115,116,114,111,107,101,45,119,105,100,116,104,58,49,46,50,54,48,52,57,51,55,54,59,115,116,114,111,107,101,45,108,105,110,101,99,97,112,58,114,111,117,110,100,59,115,116,114,
111,107,101,45,108,105,110,101,106,111,105,110,58,114,111,117,110,100,59,115,116,114,111,107,101,45,109,105,116,101,114,108,105,109,105,116,58,52,59,115,116,114,111,107,101,45,100,97,115,104,97,114,114,
97,121,58,110,111,110,101,59,115,116,114,111,107,101,45,100,97,115,104,111,102,102,115,101,116,58,50,46,50,48,48,48,48,48,48,53,59,115,116,114,111,107,101,45,111,112,97,99,105,116,121,58,49,34,10,32,32,
32,32,32,32,32,114,121,61,34,50,46,50,57,57,53,57,53,49,34,10,32,32,32,32,32,32,32,114,120,61,34,50,46,50,54,50,49,52,50,52,34,10,32,32,32,32,32,32,32,99,121,61,34,49,56,46,53,50,52,51,54,52,34,10,32,
32,32,32,32,32,32,99,120,61,34,51,46,57,53,53,48,48,52,34,32,47,62,10,32,32,60,47,103,62,10,32,32,60,103,10,32,32,32,32,32,105,100,61,34,103,56,50,56,45,54,34,10,32,32,32,32,32,116,114,97,110,115,102,
111,114,109,61,34,109,97,116,114,105,120,40,48,46,57,48,55,55,49,55,54,56,44,48,44,48,44,48,46,55,52,54,54,56,49,56,52,44,49,51,46,54,56,51,49,53,51,44,45,49,49,46,52,57,52,52,57,53,41,34,10,32,32,32,
32,32,115,116,121,108,101,61,34,115,116,114,111,107,101,45,119,105,100,116,104,58,49,46,50,49,52,54,54,53,55,55,34,62,10,32,32,32,32,60,101,108,108,105,112,115,101,10,32,32,32,32,32,32,32,105,100,61,34,
112,97,116,104,56,49,57,45,49,34,10,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,102,102,102,102,102,102,59,102,105,108,108,45,111,112,97,99,105,116,121,58,48,46,48,54,53,48,52,
48,54,56,59,115,116,114,111,107,101,58,35,102,102,102,102,102,102,59,115,116,114,111,107,101,45,119,105,100,116,104,58,49,46,50,49,52,54,54,53,55,55,59,115,116,114,111,107,101,45,108,105,110,101,99,97,
112,58,114,111,117,110,100,59,115,116,114,111,107,101,45,108,105,110,101,106,111,105,110,58,114,111,117,110,100,59,115,116,114,111,107,101,45,109,105,116,101,114,108,105,109,105,116,58,52,59,115,116,114,
111,107,101,45,100,97,115,104,97,114,114,97,121,58,110,111,110,101,59,115,116,114,111,107,101,45,100,97,115,104,111,102,102,115,101,116,58,50,46,50,48,48,48,48,48,48,53,59,115,116,114,111,107,101,45,111,
112,97,99,105,116,121,58,49,34,10,32,32,32,32,32,32,32,114,121,61,34,50,46,50,57,57,53,57,53,49,34,10,32,32,32,32,32,32,32,114,120,61,34,50,46,50,54,50,49,52,50,52,34,10,32,32,32,32,32,32,32,99,121,61,
34,49,56,46,53,50,52,51,54,52,34,10,32,32,32,32,32,32,32,99,120,61,34,51,46,57,53,53,48,48,52,34,32,47,62,10,32,32,60,47,103,62,10,32,32,60,112,97,116,104,10,32,32,32,32,32,115,116,121,108,101,61,34,102,
105,108,108,58,110,111,110,101,59,115,116,114,111,107,101,58,35,102,102,102,102,102,102,59,115,116,114,111,107,101,45,119,105,100,116,104,58,49,46,53,59,115,116,114,111,107,101,45,108,105,110,101,99,97,
112,58,98,117,116,116,59,115,116,114,111,107,101,45,108,105,110,101,106,111,105,110,58,109,105,116,101,114,59,115,116,114,111,107,101,45,109,105,116,101,114,108,105,109,105,116,58,52,59,115,116,114,111,
107,101,45,100,97,115,104,97,114,114,97,121,58,110,111,110,101,59,115,116,114,111,107,101,45,111,112,97,99,105,116,121,58,49,34,10,32,32,32,32,32,100,61,34,109,32,51,46,52,54,48,54,50,56,54,44,49,52,46,
50,57,50,50,49,32,99,32,48,44,48,32,45,48,46,50,56,52,54,52,48,53,44,45,53,46,57,49,55,53,50,52,57,32,51,46,56,50,48,49,55,52,51,44,45,56,46,52,52,57,51,50,54,55,32,67,32,49,49,46,50,50,51,54,55,54,44,
51,46,52,49,48,57,54,53,32,49,51,46,56,49,50,53,53,50,44,51,46,48,55,49,51,56,52,50,32,49,51,46,56,49,50,53,53,50,44,51,46,48,55,49,51,56,52,50,34,10,32,32,32,32,32,105,100,61,34,112,97,116,104,52,53,
53,49,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,111,110,110,101,99,116,111,114,45,99,117,114,118,97,116,117,114,101,61,34,48,34,10,32,32,32,32,32,115,111,100,105,112,111,100,105,58,110,
111,100,101,116,121,112,101,115,61,34,99,115,99,34,32,47,62,10,60,47,115,118,103,62,10,0,0};

const char* VASTWaveTableEditorComponent::arc_connect_svg = (const char*) resource_VASTWaveTableEditorComponent_arc_connect_svg;
const int VASTWaveTableEditorComponent::arc_connect_svgSize = 3051;

// JUCER_RESOURCE: ramp_connect_svg, 2958, "../../Resources/Icons/ramp_connect.svg"
static const unsigned char resource_VASTWaveTableEditorComponent_ramp_connect_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,
70,45,56,34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,10,32,32,32,120,109,108,110,115,58,100,99,61,34,104,116,116,112,58,47,47,112,117,114,108,46,111,114,103,47,
100,99,47,101,108,101,109,101,110,116,115,47,49,46,49,47,34,10,32,32,32,120,109,108,110,115,58,99,99,61,34,104,116,116,112,58,47,47,99,114,101,97,116,105,118,101,99,111,109,109,111,110,115,46,111,114,
103,47,110,115,35,34,10,32,32,32,120,109,108,110,115,58,114,100,102,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,48,50,47,50,50,45,114,100,102,45,115,121,110,116,
97,120,45,110,115,35,34,10,32,32,32,120,109,108,110,115,58,115,118,103,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,118,103,34,10,32,32,32,120,109,108,110,
115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,118,103,34,10,32,32,32,120,109,108,110,115,58,115,111,100,105,112,111,100,105,61,34,104,116,116,112,58,47,
47,115,111,100,105,112,111,100,105,46,115,111,117,114,99,101,102,111,114,103,101,46,110,101,116,47,68,84,68,47,115,111,100,105,112,111,100,105,45,48,46,100,116,100,34,10,32,32,32,120,109,108,110,115,58,
105,110,107,115,99,97,112,101,61,34,104,116,116,112,58,47,47,119,119,119,46,105,110,107,115,99,97,112,101,46,111,114,103,47,110,97,109,101,115,112,97,99,101,115,47,105,110,107,115,99,97,112,101,34,10,
32,32,32,119,105,100,116,104,61,34,50,48,112,120,34,10,32,32,32,104,101,105,103,104,116,61,34,50,48,112,120,34,10,32,32,32,118,105,101,119,66,111,120,61,34,48,32,48,32,50,48,32,50,48,34,10,32,32,32,118,
101,114,115,105,111,110,61,34,49,46,49,34,10,32,32,32,105,100,61,34,115,118,103,49,50,34,10,32,32,32,115,111,100,105,112,111,100,105,58,100,111,99,110,97,109,101,61,34,114,97,109,112,95,99,111,110,110,
101,99,116,46,115,118,103,34,10,32,32,32,105,110,107,115,99,97,112,101,58,118,101,114,115,105,111,110,61,34,48,46,57,50,46,51,32,40,50,52,48,53,53,52,54,44,32,50,48,49,56,45,48,51,45,49,49,41,34,62,10,
32,32,60,109,101,116,97,100,97,116,97,10,32,32,32,32,32,105,100,61,34,109,101,116,97,100,97,116,97,49,54,34,62,10,32,32,32,32,60,114,100,102,58,82,68,70,62,10,32,32,32,32,32,32,60,99,99,58,87,111,114,
107,10,32,32,32,32,32,32,32,32,32,114,100,102,58,97,98,111,117,116,61,34,34,62,10,32,32,32,32,32,32,32,32,60,100,99,58,102,111,114,109,97,116,62,105,109,97,103,101,47,115,118,103,43,120,109,108,60,47,
100,99,58,102,111,114,109,97,116,62,10,32,32,32,32,32,32,32,32,60,100,99,58,116,121,112,101,10,32,32,32,32,32,32,32,32,32,32,32,114,100,102,58,114,101,115,111,117,114,99,101,61,34,104,116,116,112,58,47,
47,112,117,114,108,46,111,114,103,47,100,99,47,100,99,109,105,116,121,112,101,47,83,116,105,108,108,73,109,97,103,101,34,32,47,62,10,32,32,32,32,32,32,32,32,60,100,99,58,116,105,116,108,101,62,115,116,
97,116,115,32,91,35,49,49,57,48,93,60,47,100,99,58,116,105,116,108,101,62,10,32,32,32,32,32,32,60,47,99,99,58,87,111,114,107,62,10,32,32,32,32,60,47,114,100,102,58,82,68,70,62,10,32,32,60,47,109,101,116,
97,100,97,116,97,62,10,32,32,60,115,111,100,105,112,111,100,105,58,110,97,109,101,100,118,105,101,119,10,32,32,32,32,32,112,97,103,101,99,111,108,111,114,61,34,35,57,53,57,53,57,53,34,10,32,32,32,32,32,
98,111,114,100,101,114,99,111,108,111,114,61,34,35,54,54,54,54,54,54,34,10,32,32,32,32,32,98,111,114,100,101,114,111,112,97,99,105,116,121,61,34,49,34,10,32,32,32,32,32,111,98,106,101,99,116,116,111,108,
101,114,97,110,99,101,61,34,49,48,34,10,32,32,32,32,32,103,114,105,100,116,111,108,101,114,97,110,99,101,61,34,49,48,34,10,32,32,32,32,32,103,117,105,100,101,116,111,108,101,114,97,110,99,101,61,34,49,
48,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,112,97,103,101,111,112,97,99,105,116,121,61,34,49,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,112,97,103,101,115,104,97,100,111,119,61,
34,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,119,105,100,116,104,61,34,51,56,52,48,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,
45,104,101,105,103,104,116,61,34,50,48,53,48,34,10,32,32,32,32,32,105,100,61,34,110,97,109,101,100,118,105,101,119,49,52,34,10,32,32,32,32,32,115,104,111,119,103,114,105,100,61,34,102,97,108,115,101,34,
10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,122,111,111,109,61,34,54,54,46,55,53,48,56,56,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,120,61,34,56,46,52,53,50,56,50,52,53,34,10,32,
32,32,32,32,105,110,107,115,99,97,112,101,58,99,121,61,34,55,46,52,48,51,56,54,49,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,120,61,34,45,49,50,34,10,32,32,32,
32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,121,61,34,45,49,50,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,109,97,120,105,109,105,122,101,100,
61,34,49,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,117,114,114,101,110,116,45,108,97,121,101,114,61,34,115,118,103,49,50,34,10,32,32,32,32,32,115,104,111,119,103,117,105,100,101,115,61,
34,102,97,108,115,101,34,32,47,62,10,32,32,60,33,45,45,32,71,101,110,101,114,97,116,111,114,58,32,83,107,101,116,99,104,32,51,46,56,46,49,32,40,50,57,54,56,55,41,32,45,32,104,116,116,112,58,47,47,119,
119,119,46,98,111,104,101,109,105,97,110,99,111,100,105,110,103,46,99,111,109,47,115,107,101,116,99,104,32,45,45,62,10,32,32,60,116,105,116,108,101,10,32,32,32,32,32,105,100,61,34,116,105,116,108,101,
50,34,62,115,116,97,116,115,32,91,35,49,49,57,48,93,60,47,116,105,116,108,101,62,10,32,32,60,100,101,115,99,10,32,32,32,32,32,105,100,61,34,100,101,115,99,52,34,62,67,114,101,97,116,101,100,32,119,105,
116,104,32,83,107,101,116,99,104,46,60,47,100,101,115,99,62,10,32,32,60,100,101,102,115,10,32,32,32,32,32,105,100,61,34,100,101,102,115,54,34,32,47,62,10,32,32,60,103,10,32,32,32,32,32,105,100,61,34,103,
56,50,56,34,10,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,109,97,116,114,105,120,40,48,46,56,52,50,57,49,51,54,49,44,48,44,48,44,48,46,55,52,54,54,56,49,56,52,44,45,48,46,49,56,55,55,48,48,
52,52,44,51,46,51,56,49,55,49,51,52,41,34,10,32,32,32,32,32,115,116,121,108,101,61,34,115,116,114,111,107,101,45,119,105,100,116,104,58,49,46,50,54,48,52,57,51,55,54,34,62,10,32,32,32,32,60,101,108,108,
105,112,115,101,10,32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,56,49,57,34,10,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,102,102,102,102,102,102,59,102,105,108,108,45,111,
112,97,99,105,116,121,58,48,46,48,54,53,48,52,48,54,56,59,115,116,114,111,107,101,58,35,102,102,102,102,102,102,59,115,116,114,111,107,101,45,119,105,100,116,104,58,49,46,50,54,48,52,57,51,55,54,59,115,
116,114,111,107,101,45,108,105,110,101,99,97,112,58,114,111,117,110,100,59,115,116,114,111,107,101,45,108,105,110,101,106,111,105,110,58,114,111,117,110,100,59,115,116,114,111,107,101,45,109,105,116,101,
114,108,105,109,105,116,58,52,59,115,116,114,111,107,101,45,100,97,115,104,97,114,114,97,121,58,110,111,110,101,59,115,116,114,111,107,101,45,100,97,115,104,111,102,102,115,101,116,58,50,46,50,48,48,48,
48,48,48,53,59,115,116,114,111,107,101,45,111,112,97,99,105,116,121,58,49,34,10,32,32,32,32,32,32,32,114,121,61,34,50,46,50,57,57,53,57,53,49,34,10,32,32,32,32,32,32,32,114,120,61,34,50,46,50,54,50,49,
52,50,52,34,10,32,32,32,32,32,32,32,99,121,61,34,49,56,46,53,50,52,51,54,52,34,10,32,32,32,32,32,32,32,99,120,61,34,51,46,57,53,53,48,48,52,34,32,47,62,10,32,32,60,47,103,62,10,32,32,60,103,10,32,32,32,
32,32,105,100,61,34,103,56,50,56,45,54,34,10,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,109,97,116,114,105,120,40,48,46,57,48,55,55,49,55,54,56,44,48,44,48,44,48,46,55,52,54,54,56,49,56,52,
44,49,51,46,54,56,51,49,53,51,44,45,49,49,46,52,57,52,52,57,53,41,34,10,32,32,32,32,32,115,116,121,108,101,61,34,115,116,114,111,107,101,45,119,105,100,116,104,58,49,46,50,49,52,54,54,53,55,55,34,62,10,
32,32,32,32,60,101,108,108,105,112,115,101,10,32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,56,49,57,45,49,34,10,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,102,102,102,102,
102,102,59,102,105,108,108,45,111,112,97,99,105,116,121,58,48,46,48,54,53,48,52,48,54,56,59,115,116,114,111,107,101,58,35,102,102,102,102,102,102,59,115,116,114,111,107,101,45,119,105,100,116,104,58,49,
46,50,49,52,54,54,53,55,55,59,115,116,114,111,107,101,45,108,105,110,101,99,97,112,58,114,111,117,110,100,59,115,116,114,111,107,101,45,108,105,110,101,106,111,105,110,58,114,111,117,110,100,59,115,116,
114,111,107,101,45,109,105,116,101,114,108,105,109,105,116,58,52,59,115,116,114,111,107,101,45,100,97,115,104,97,114,114,97,121,58,110,111,110,101,59,115,116,114,111,107,101,45,100,97,115,104,111,102,
102,115,101,116,58,50,46,50,48,48,48,48,48,48,53,59,115,116,114,111,107,101,45,111,112,97,99,105,116,121,58,49,34,10,32,32,32,32,32,32,32,114,121,61,34,50,46,50,57,57,53,57,53,49,34,10,32,32,32,32,32,
32,32,114,120,61,34,50,46,50,54,50,49,52,50,52,34,10,32,32,32,32,32,32,32,99,121,61,34,49,56,46,53,50,52,51,54,52,34,10,32,32,32,32,32,32,32,99,120,61,34,51,46,57,53,53,48,48,52,34,32,47,62,10,32,32,60,
47,103,62,10,32,32,60,112,97,116,104,10,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,110,111,110,101,59,115,116,114,111,107,101,58,35,102,102,102,102,102,102,59,115,116,114,111,107,101,
45,119,105,100,116,104,58,49,46,53,59,115,116,114,111,107,101,45,108,105,110,101,99,97,112,58,98,117,116,116,59,115,116,114,111,107,101,45,108,105,110,101,106,111,105,110,58,109,105,116,101,114,59,115,
116,114,111,107,101,45,111,112,97,99,105,116,121,58,49,59,115,116,114,111,107,101,45,109,105,116,101,114,108,105,109,105,116,58,52,59,115,116,114,111,107,101,45,100,97,115,104,97,114,114,97,121,58,110,
111,110,101,34,10,32,32,32,32,32,100,61,34,77,32,53,46,51,48,51,51,48,48,57,44,49,52,46,57,53,49,51,55,55,32,49,53,46,48,52,49,44,52,46,57,55,51,57,56,48,57,32,118,32,48,34,10,32,32,32,32,32,105,100,61,
34,112,97,116,104,52,53,53,53,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,111,110,110,101,99,116,111,114,45,99,117,114,118,97,116,117,114,101,61,34,48,34,32,47,62,10,60,47,115,118,103,62,
10,0,0};

const char* VASTWaveTableEditorComponent::ramp_connect_svg = (const char*) resource_VASTWaveTableEditorComponent_ramp_connect_svg;
const int VASTWaveTableEditorComponent::ramp_connect_svgSize = 2958;

// JUCER_RESOURCE: arrow_right_corner_svg, 1715, "../../Resources/Icons/arrow_right_corner.svg"
static const unsigned char resource_VASTWaveTableEditorComponent_arrow_right_corner_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,
85,84,70,45,56,34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,32,119,105,100,116,104,61,34,50,48,112,120,34,32,104,101,105,103,104,116,61,34,50,48,112,120,34,32,118,
105,101,119,66,111,120,61,34,48,32,48,32,50,48,32,50,48,34,32,118,101,114,115,105,111,110,61,34,49,46,49,34,32,120,109,108,110,115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,
50,48,48,48,47,115,118,103,34,32,120,109,108,110,115,58,120,108,105,110,107,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,120,108,105,110,107,34,62,10,32,32,32,
32,60,33,45,45,32,71,101,110,101,114,97,116,111,114,58,32,83,107,101,116,99,104,32,51,46,56,46,49,32,40,50,57,54,56,55,41,32,45,32,104,116,116,112,58,47,47,119,119,119,46,98,111,104,101,109,105,97,110,
99,111,100,105,110,103,46,99,111,109,47,115,107,101,116,99,104,32,45,45,62,10,32,32,32,32,60,116,105,116,108,101,62,97,114,114,111,119,95,114,105,103,104,116,95,99,111,114,110,101,114,32,91,35,50,55,57,
93,60,47,116,105,116,108,101,62,10,32,32,32,32,60,100,101,115,99,62,67,114,101,97,116,101,100,32,119,105,116,104,32,83,107,101,116,99,104,46,60,47,100,101,115,99,62,10,32,32,32,32,60,100,101,102,115,62,
60,47,100,101,102,115,62,10,32,32,32,32,60,103,32,105,100,61,34,80,97,103,101,45,49,34,32,115,116,114,111,107,101,61,34,110,111,110,101,34,32,115,116,114,111,107,101,45,119,105,100,116,104,61,34,49,34,
32,102,105,108,108,61,34,110,111,110,101,34,32,102,105,108,108,45,114,117,108,101,61,34,101,118,101,110,111,100,100,34,62,10,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,68,114,105,98,98,98,108,101,
45,68,97,114,107,45,80,114,101,118,105,101,119,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,45,49,56,48,46,48,48,48,48,48,48,44,32,45,54,57,49,57,46,48,48,48,48,
48,48,41,34,32,102,105,108,108,61,34,35,70,70,70,70,70,70,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,105,99,111,110,115,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,
97,110,115,108,97,116,101,40,53,54,46,48,48,48,48,48,48,44,32,49,54,48,46,48,48,48,48,48,48,41,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,60,112,97,116,104,32,100,61,34,77,49,51,50,44,54,
55,54,52,32,67,49,51,50,44,54,55,54,51,46,52,52,56,32,49,51,50,46,52,52,56,44,54,55,54,51,32,49,51,51,44,54,55,54,51,32,76,49,51,56,44,54,55,54,51,32,67,49,51,57,46,49,48,53,44,54,55,54,51,32,49,52,48,
44,54,55,54,51,46,56,57,53,32,49,52,48,44,54,55,54,53,32,76,49,52,48,44,54,55,55,48,32,67,49,52,48,44,54,55,55,48,46,53,53,50,32,49,51,57,46,53,53,50,44,54,55,55,49,32,49,51,57,44,54,55,55,49,32,67,49,
51,56,46,52,52,56,44,54,55,55,49,32,49,51,56,44,54,55,55,48,46,53,53,50,32,49,51,56,44,54,55,55,48,32,76,49,51,56,44,54,55,54,55,46,54,50,49,32,67,49,51,56,44,54,55,54,55,46,49,55,54,32,49,51,55,46,52,
54,49,44,54,55,54,54,46,57,53,51,32,49,51,55,46,49,52,54,44,54,55,54,55,46,50,54,56,32,76,49,51,52,46,49,50,49,44,54,55,55,48,46,50,57,51,32,67,49,51,51,46,55,51,49,44,54,55,55,48,46,54,56,51,32,49,51,
51,46,48,57,56,44,54,55,55,48,46,54,56,51,32,49,51,50,46,55,48,55,44,54,55,55,48,46,50,57,51,32,67,49,51,50,46,51,49,55,44,54,55,54,57,46,57,48,50,32,49,51,50,46,51,49,55,44,54,55,54,57,46,50,54,57,32,
49,51,50,46,55,48,55,44,54,55,54,56,46,56,55,57,32,76,49,51,53,46,55,51,50,44,54,55,54,53,46,56,53,52,32,67,49,51,54,46,48,52,55,44,54,55,54,53,46,53,51,57,32,49,51,53,46,56,50,52,44,54,55,54,53,32,49,
51,53,46,51,55,57,44,54,55,54,53,32,76,49,51,51,44,54,55,54,53,32,67,49,51,50,46,52,52,56,44,54,55,54,53,32,49,51,50,44,54,55,54,52,46,53,53,50,32,49,51,50,44,54,55,54,52,32,76,49,51,50,44,54,55,54,52,
32,90,32,77,49,50,54,44,54,55,54,50,32,67,49,50,54,44,54,55,54,49,46,52,52,56,32,49,50,54,46,52,52,56,44,54,55,54,49,32,49,50,55,44,54,55,54,49,32,76,49,52,49,44,54,55,54,49,32,67,49,52,49,46,53,53,50,
44,54,55,54,49,32,49,52,50,44,54,55,54,49,46,52,52,56,32,49,52,50,44,54,55,54,50,32,76,49,52,50,44,54,55,55,54,32,67,49,52,50,44,54,55,55,54,46,53,53,50,32,49,52,49,46,53,53,50,44,54,55,55,55,32,49,52,
49,44,54,55,55,55,32,76,49,51,51,44,54,55,55,55,32,67,49,51,50,46,52,52,56,44,54,55,55,55,32,49,51,50,44,54,55,55,54,46,53,53,50,32,49,51,50,44,54,55,55,54,32,76,49,51,50,44,54,55,55,51,32,67,49,51,50,
44,54,55,55,49,46,56,57,53,32,49,51,49,46,49,48,53,44,54,55,55,49,32,49,51,48,44,54,55,55,49,32,76,49,50,55,44,54,55,55,49,32,67,49,50,54,46,52,52,56,44,54,55,55,49,32,49,50,54,44,54,55,55,48,46,53,53,
50,32,49,50,54,44,54,55,55,48,32,76,49,50,54,44,54,55,54,50,32,90,32,77,49,52,52,44,54,55,54,49,32,67,49,52,52,44,54,55,53,57,46,56,57,53,32,49,52,51,46,49,48,53,44,54,55,53,57,32,49,52,50,44,54,55,53,
57,32,76,49,50,54,44,54,55,53,57,32,67,49,50,52,46,56,57,53,44,54,55,53,57,32,49,50,52,44,54,55,53,57,46,56,57,53,32,49,50,52,44,54,55,54,49,32,76,49,50,52,44,54,55,55,55,32,67,49,50,52,44,54,55,55,56,
46,49,48,53,32,49,50,52,46,56,57,53,44,54,55,55,57,32,49,50,54,44,54,55,55,57,32,76,49,52,50,44,54,55,55,57,32,67,49,52,51,46,49,48,53,44,54,55,55,57,32,49,52,52,44,54,55,55,56,46,49,48,53,32,49,52,52,
44,54,55,55,55,32,76,49,52,52,44,54,55,54,49,32,90,34,32,105,100,61,34,97,114,114,111,119,95,114,105,103,104,116,95,99,111,114,110,101,114,45,91,35,50,55,57,93,34,62,60,47,112,97,116,104,62,10,32,32,32,
32,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,60,47,103,62,10,60,47,115,118,103,62,0,0};

const char* VASTWaveTableEditorComponent::arrow_right_corner_svg = (const char*) resource_VASTWaveTableEditorComponent_arrow_right_corner_svg;
const int VASTWaveTableEditorComponent::arrow_right_corner_svgSize = 1715;


//[EndFile] You can add extra defines here...
//[/EndFile]

