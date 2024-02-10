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
#include "../Engine/VASTSampler.h"
#include "../Engine/VASTPluginConstants.h"
#include "VASTWTEditor/VASTFreqDomainViewport.h"
#include "VASTWTEditor/VASTPositionViewport.h"
#include "VASTWTEditor/VASTSamplerViewport.h"
#include "VASTControls/VASTDrawableButton.h"
#include "VASTControls/VASTConcertinaPanel.h"
#include "VASTUtils/VASTLookAndFeel.h"
#include "VASTSamplerEditorComponent.h"
#include "VASTSamplerEditorComponentHeader.h"
#include "VASTWaveTableEditorView.h"
#include "VASTWaveTableEditorViewHeader.h"

enum EditorFunction { Level, Phase, Freq, FreqMod, BitDepth, Offset, Bend, Bloat, Smooth, Clean, Connect, DrawConnect, Normalize, NormalizePerCycle, AmplModCopy, MixInCopy, Interpolate, PWMGenerate, FMGenerate, HardSyncGenerate, BloatGenerate, BendGenerate, FormantsMaleGenerate, FormantsFemaleGenerate, HarmonicMorph, HarmonicReduce, HighpassFilter, LowpassFilter, LowpassSoft, BandpassFilter, PulseFilter, CombFilter, Sine, Zeros, Sawtooth, Sinc, HalfSine, Square, Triangle, Ramp, Random, Invert, Reverse, ArcConnect, RampConnect, SampleHold, Convolution, TimeToFrequency, FrequencyToTime,
};
enum ApplyOption { ApplyInterpolation, ApplyAmplitudeModulation, ApplyMixIn, ApplyConvolution, ApplyTimeToFrequency, ApplyFrequencyToTime, ApplyNormalize, ApplyNormalizePerCycle };
enum GenerateOption { GenerateHardSync, GeneratePulseWaveModualtion, GenerateFrequencyModulation, GenerateBloatTable, GenerateBendTable, GenerateFormantsMale, GenerateFormantsFemale, GenerateHarmonicMorph, GenerateHarmonicReduce, GenerateHighpassFilter, GenerateLowpassFilter, GenerateLowpassSoft, GenerateBandpassFilter, GeneratePulseFilter, GenerateCombFilter };
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
An auto-generated component, created by the Projucer.

Describe your class and how it works here!
                                                                    //[/Comments]
*/
class VASTWaveTableEditorComponent  : public Component,
                                      public MultiTimer,
                                      public TextEditor::Listener,
                                      public Button::Listener,
                                      public Slider::Listener
{
public:
    //==============================================================================
    VASTWaveTableEditorComponent (AudioProcessorEditor *editor, AudioProcessor* processor);
    ~VASTWaveTableEditorComponent() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.

	Viewport* getPositionViewport() { return c_waveTableEditorView->getViewportPositions(); };
	Viewport* getSamplerViewport() { return c_samplerEditorComponent->getSamplerViewport(); };
	Viewport* getFreqDomainViewport() { return c_waveTableEditorView->getViewportFreqDomain(); };

	VASTSamplerEditorComponent* getSamplerEditor() { return c_samplerEditorComponent.get(); };
	VASTWaveTableEditorView* getEditorView() { return c_waveTableEditorView.get(); };

	void sliderValueChanged(Slider* sliderThatWasMoved) override;
	void buttonClicked(Button* buttonThatWasClicked) override;

	std::atomic<int> numFreqThreads = 0;

	void textEditorTextChanged(TextEditor &) override;
	void notifySelectionChanged();
	void copySelectionToLocalBuffer();
	void initSliders();
	//void addPosition();
	void insertPosition();
	void toggleDrawMode() {
		int dmode = myProcessor->m_iWTEditorDrawMode;
		dmode++;
		dmode = dmode % 3;
		setDrawMode(dmode);
	};

	void randomizeBankWavetable(int bank, bool analog, bool digital);
	void randomizeSample();

	int getOscBank()  const { return m_bank; };
	void setOscBank(int bank);
	std::shared_ptr<CVASTWaveTable> getBankWavetable() const {
		return myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_bank].getNewSharedWavetable(); };
	std::shared_ptr<CVASTWaveTable> getBankWavetable(int bank) {
		return myProcessor->m_pVASTXperience.m_Poly.m_OscBank[bank].getNewSharedWavetable(); };
	void setCurWavetable(const std::shared_ptr<CVASTWaveTable> &wavetable) { //const is faster https://stackoverflow.com/questions/3310737/should-we-pass-a-shared-ptr-by-reference-or-by-value#:~:text=Most%20of%20the%20time%20passing,by%20const%20reference%2C%20would%20do.&text=An%20example%20of%20when%20passing,the%20callee%20completes%20its%20job.
		//m_cur_wavetable = std::make_shared<CVASTWaveTable>(*wavetable); //copies it
		std::atomic_store(&m_cur_wavetable, std::make_shared<CVASTWaveTable>(*wavetable)); //copies it //assigning a new instance to a shared pointer make thread safe: https://www.modernescpp.com/index.php/atomic-smart-pointers
	};
	void setCopyPasteWavetable(const std::shared_ptr<CVASTWaveTable> &wavetable) { //const is faster https://stackoverflow.com/questions/3310737/should-we-pass-a-shared-ptr-by-reference-or-by-value#:~:text=Most%20of%20the%20time%20passing,by%20const%20reference%2C%20would%20do.&text=An%20example%20of%20when%20passing,the%20callee%20completes%20its%20job.
		//m_copypaste_wavetable = std::make_shared<CVASTWaveTable>(*wavetable); //copies it
		std::atomic_store(&m_copypaste_wavetable, std::make_shared<CVASTWaveTable>(*wavetable)); //copies it //assigning a new instance to a shared pointer make thread safe: https://www.modernescpp.com/index.php/atomic-smart-pointers
	};
	std::shared_ptr<CVASTWaveTable> getCurWavetable() {
		return m_cur_wavetable;
	};
	std::shared_ptr<CVASTWaveTable> getCopyPasteWavetable() {
		return m_copypaste_wavetable;
	};

	VASTSamplerSound* getCurSamplerSound() {
		VASTSynthesiserSound* synSound = (VASTSynthesiserSound*)myProcessor->m_pVASTXperience.m_Poly.getSynthesizer()->getSound(0);
		if (synSound == nullptr) return nullptr;
		return synSound->getSamplerSoundChanged();
	};

	void setDrawMode(int mode);
	void setGridMode(int mode);
	void setBinMode(int mode);
	void setBinEditMode(int mode);

	void startAutoUpdate();
	void stopAutoUpdate();
	void initAll();
	void updateAll(bool force);
	int getWtPos() const {
		return getBankWavetable()->getSelectedWtPos();
		//return m_wtPos;
	};
	void setWtPos(int wtpos) { //normally use setSelectedWtPos on Wavetable!!
        if (getBankWavetable()->m_isBeingUpdated)
            return;
		getBankWavetable()->setSelectedWtPos(wtpos);
		myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_bank].setChangedFlag();
		//m_wtPos = wtpos;
		//clearMultiSelect();
		//scroll viewport
		float left = m_positionviewport->getPositionTotalWidth() * (wtpos);
		float left5 = m_positionviewport->getPositionTotalWidth() * (wtpos - 5);
		if (left5 < 0.f) left5 = 0.f;

		//viewed area = c_waveTableEditorView->getViewportPositions()->getViewPositionX()  to c_waveTableEditorView->getViewportPositions()->getViewPositionX()  + c_waveTableEditorView->getViewportPositions()->getWidth()
		if ((left < c_waveTableEditorView->getViewportPositions()->getViewPositionX()) || (left > (c_waveTableEditorView->getViewportPositions()->getViewPositionX() + c_waveTableEditorView->getViewportPositions()->getWidth())))
			c_waveTableEditorView->getViewportPositions()->setViewPosition(left5 / m_positionviewport->getScreenWidthScale(), 0);
	};
	void clearMultiSelect() {
		myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_bank].addSoftFadeEditor();
		std::shared_ptr<CVASTWaveTable> wavetable = myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_bank].getSoftOrCopyWavetable();
		wavetable->clearMultiSelect();
		myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_bank].setWavetableSoftFade(wavetable);
		myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_bank].removeSoftFadeEditor();


		//m_multiSelect = false;
		//m_multiSelectBegin = 0;
		//m_multiSelectEnd = 0;
		//wavetableNotifySelection();
	};
	void multiSelectAll() {
		myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_bank].addSoftFadeEditor();
		std::shared_ptr<CVASTWaveTable> wavetable = myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_bank].getSoftOrCopyWavetable(false, true); //keep freqs when selecting only
		wavetable->multiSelectAll();
		myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_bank].setWavetableSoftFade(wavetable);
		myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_bank].removeSoftFadeEditor();


		//m_multiSelect = true;
		//m_multiSelectBegin = 0;
		//m_multiSelectEnd = getBankWavetable()->getNumPositions() - 1;
		//wavetableNotifySelection();
	};
	/*bool isMultiSelected() {
		return m_multiSelect;
	};
	int getMultiSelectBegin()  const {
		return m_multiSelectBegin;
	};
	int getMultiSelectEnd()  const {
		return m_multiSelectEnd;
	};

	void setMultiSelect(int wtPos) {
		m_multiSelect = true;
		if (wtPos < getWtPos()) {
			m_multiSelectBegin = wtPos;
			m_multiSelectEnd = (getWtPos() <= getBankWavetable()->getNumPositions() - 1 ? getWtPos() : getBankWavetable()->getNumPositions() - 1);
		}
		else {
			m_multiSelectBegin = getWtPos();
			m_multiSelectEnd = (wtPos <= getBankWavetable()->getNumPositions() - 1 ? wtPos : getBankWavetable()->getNumPositions() - 1);
		}
		wavetableNotifySelection();
	};
	*/

	void updateHeaderSelectionLabel() {
		std::shared_ptr<CVASTWaveTable> wavetable = getBankWavetable();
		if (wavetable->isMultiSelected()) {
			//wavetable->setSelection(getMultiSelectBegin(), getMultiSelectEnd());
			if (c_waveTableEditorView != nullptr)
				if (c_waveTableEditorView->getHeader() != nullptr)
					c_waveTableEditorView->getHeader()->setSelectionLabel(wavetable->getMultiSelectBegin(), wavetable->getMultiSelectEnd(), wavetable->getNumPositions());
		}
		else {
			//wavetable->setSelection(getWtPos(), getWtPos());
			if (c_waveTableEditorView != nullptr)
				if (c_waveTableEditorView->getHeader() != nullptr)
					c_waveTableEditorView->getHeader()->setSelectionLabel(wavetable->getSelectedWtPos(), getWtPos(), wavetable->getNumPositions());
		}
	}

	void wavetableNotifySelection() {
		updateHeaderSelectionLabel();
		/*
		std::shared_ptr<CVASTWaveTable> wavetable = getBankWavetable();
		if (isMultiSelected()) {
			wavetable->setSelection(getMultiSelectBegin(), getMultiSelectEnd());
		}
		else {
			wavetable->setSelection(getWtPos(), getWtPos());
		}
		if (myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_bank].getSoftFadeWavetablePointerNext() != nullptr) {
			wavetable = myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_bank].getNewSharedSoftFadeWavetableNext(); //CHECK if new share
			if (isMultiSelected()) {
				wavetable->setSelection(getMultiSelectBegin(), getMultiSelectEnd());
			}
			else {
				wavetable->setSelection(getWtPos(), getWtPos());
			}
		}
		*/
		myProcessor->m_pVASTXperience.m_Poly.m_OscBank[m_bank].setChangedFlag(); //CHECK!!!!
	}

	static void loadWTFileThread(juce::File file, VASTWaveTableEditorComponent* editor);
	static void saveWTFileThread(juce::File file, CVASTWaveTable* wavetable, StringPairArray sarray, VASTWaveTableEditorComponent* editor);
	static void threadedEditorFunction(int editorFunction, double sliderValue, /*int msbeg, int msend,*/ int wtselstart, int wtselend, VASTWaveTableEditorComponent* editor, float connectval1, float connectval2, std::shared_ptr<CVASTWaveTable> transactionWavetable, bool pregenerate);
	static void threadedFreehandDraw(/*int msbeg, int msend,*/ VASTWaveTableEditorComponent* editor, juce::Array<juce::Point<float>> spoints);
	static void normalizePositions(int beg, int end, CVASTWaveTable* wavetable, std::shared_ptr<CVASTWaveTable> bankWavetable, bool perCycle, int wtMode);
	static void frequencyModulate(const std::vector<myFloat> &inSamples, std::vector<myFloat> &outSamples, int startSample, int endSample, float sinPhaseInc);
	static void stretchToZeroCrossings(std::vector<myFloat> &inSamples, int wtselstart, int wtselend, bool snapFirstOnly);
	static void calcBend(const std::vector<myFloat> &inSamples, std::vector<myFloat> &outSamples, int startSample, int endSample, float bvendValue);
	static void calcBloat(const std::vector<myFloat> &inSamples, std::vector<myFloat> &outSamples, int startSample, int endSample, float bloatValue);
	static void calcFlip(const std::vector<myFloat> &inSamples, std::vector<myFloat> &outSamples, int startSample, int endSample, float flipValue);
	static void calcMirror(const std::vector<myFloat> &inSamples, std::vector<myFloat> &outSamples, int startSample, int endSample, float flipValue);
	static void calcBreed(const std::vector<myFloat> &inSamples, std::vector<myFloat> &outSamples, int startSample, int endSample, float breedValue);
	static void calcSincify(const std::vector<myFloat> &inSamples, std::vector<myFloat> &outSamples, int startSample, int endSample, float breedValue);
	static void calcBrew(const std::vector<myFloat> &inSamples, std::vector<myFloat> &outSamples, int startSample, int endSample, float brewValue);
	static void calcWindow(const std::vector<myFloat> &inSamples, std::vector<myFloat> &outSamples, int startSample, int endSample, float windowValue);

	void scaleAudioBufferToWTPos(CVASTWaveTable* wavetable, int startWtPos, int targetWtPosCount, const float* audioBuffer, int numSamples, int fixedSamplesPerCycle = 0);

	bool m_bSnapToZero = false;
	bool m_bStretchToZero = false;

	VASTSamplerSound* loadWavFile(String filename);
	void loadWTFile(String filename);
	void timerCallback(int timerID) override;
	float sampleDetermineFreq(int start, int end, int channel);
	void findZeroLoop();
	void wavSelectionToWt(int channel, std::shared_ptr<CVASTWaveTable> transactionWavetable);
	void wavSelectionToWavetablePatch(bool stereo, bool lfo);

	bool isInterestedInFileDrag(const juce::StringArray &) { return true; }; //todo check for consumable files

	void deletePositions(bool onlyOdds);
	void copyPositions();
	void cutPositions();
	void deleteAllButSelectedPositions();
	void insertCutCopiedPositions(bool beforeSelection, bool deleteAllPosBefore);
	void duplicateSelectedPositions();
	void replaceWithCutCopiedPositions();
	void thinOutSelectedPositions();
	void stretchAndInterpolateSelectedPositions();
	void reverseOrderSelectedPositions();

	void importFilesAsCycles(const StringArray& files, int startPos);

	int m_bank = 0;
	static int &getEditorThreadsRunning() {
		static int s_EditorThreadsRunning = 0;
		return s_EditorThreadsRunning;
	};

	VASTFreqDomainViewport* m_freqviewport = nullptr;
	VASTPositionViewport* m_positionviewport = nullptr;
	VASTSamplerViewport* m_samplerviewport = nullptr;

	friend class VASTWaveTableEditorView;
	friend class VASTWaveTableEditorViewHeader;
	friend class VASTSamplerEditorComponent;
	friend class VASTSamplerEditorComponentHeader;
	friend class VASTPositionViewport;
	friend class VASTFreqDomainViewport;

	template <typename Word>
	void write_word(FileOutputStream* outs, Word value, unsigned size);

	void requestUIUpdate() {
		m_needsUIUpdate = true;
	};

	void startWTRecording();
	void stopWTRecording();

    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void visibilityChanged() override;

    // Binary resources:
    static const char* add_svg;
    static const int add_svgSize;
    static const char* copy_svg;
    static const int copy_svgSize;
    static const char* cut_svg;
    static const int cut_svgSize;
    static const char* delete_svg;
    static const int delete_svgSize;
    static const char* duplicate_svg;
    static const int duplicate_svgSize;
    static const char* minus_svg;
    static const int minus_svgSize;
    static const char* normalize_svg;
    static const int normalize_svgSize;
    static const char* normalize2_svg;
    static const int normalize2_svgSize;
    static const char* normalize3_svg;
    static const int normalize3_svgSize;
    static const char* paste_svg;
    static const int paste_svgSize;
    static const char* plus_svg;
    static const int plus_svgSize;
    static const char* save_svg;
    static const int save_svgSize;
    static const char* select_all_svg;
    static const int select_all_svgSize;
    static const char* zoom_in_svg;
    static const int zoom_in_svgSize;
    static const char* zoom_out_svg;
    static const int zoom_out_svgSize;
    static const char* dynamics_svg;
    static const int dynamics_svgSize;
    static const char* close_svg;
    static const int close_svgSize;
    static const char* ramp_svg;
    static const int ramp_svgSize;
    static const char* invert_svg;
    static const int invert_svgSize;
    static const char* reverse_svg;
    static const int reverse_svgSize;
    static const char* to_editor_svg;
    static const int to_editor_svgSize;
    static const char* load_svg;
    static const int load_svgSize;
    static const char* zero_find_svg;
    static const int zero_find_svgSize;
    static const char* rampconnect_svg;
    static const int rampconnect_svgSize;
    static const char* apply_svg;
    static const int apply_svgSize;
    static const char* generate_svg;
    static const int generate_svgSize;
    static const char* waves_square_svg;
    static const int waves_square_svgSize;
    static const char* waves_sine_svg;
    static const int waves_sine_svgSize;
    static const char* waves_triangle_svg;
    static const int waves_triangle_svgSize;
    static const char* waves_saw_svg;
    static const int waves_saw_svgSize;
    static const char* waves_ramp_svg;
    static const int waves_ramp_svgSize;
    static const char* waves_halfsine_svg;
    static const int waves_halfsine_svgSize;
    static const char* waves_zeros_svg;
    static const int waves_zeros_svgSize;
    static const char* waves_random_svg;
    static const int waves_random_svgSize;
    static const char* waves_sinc_svg;
    static const int waves_sinc_svgSize;
    static const char* arc_connect_svg;
    static const int arc_connect_svgSize;
    static const char* ramp_connect_svg;
    static const int ramp_connect_svgSize;
    static const char* arrow_right_corner_svg;
    static const int arrow_right_corner_svgSize;


private:
    //[UserVariables]   -- You can add your own custom variables in this section.

    std::shared_ptr<CVASTWaveTable> m_cur_wavetable;
	std::shared_ptr<CVASTWaveTable> m_copypaste_wavetable;

    std::unique_ptr<juce::AlertWindow> m_alertWindow;
	std::unique_ptr<FileChooser> myChooser;

	bool mFileChoserIsOpen = false;
	void loadWTFileThreaded (juce::File file);
	void saveWTFileThreaded(juce::File file, CVASTWaveTable* wavetable);

	VASTAudioProcessorEditor* myEditor = nullptr;
	VASTAudioProcessor* myProcessor = nullptr;

	std::unique_ptr<VASTSamplerEditorComponent> c_samplerEditorComponent;
	std::unique_ptr<VASTWaveTableEditorView> c_waveTableEditorView;

	Slider* sliderThatWasLastMoved;
	std::shared_ptr<CVASTWaveTable> newWTToUpdate;
	bool newWTToUpdate_failed = false;

	bool m_needsUIUpdate = false;

	bool m_multiSelect = false;
	int m_multiSelectBegin = 0;
	int m_multiSelectEnd = 0;

	bool m_bIsRecordingWT = false;
	bool m_bIsRecordingWTBlink = false;
	int m_iRecordingOscBank = -1;

	//ULong64_t m_lastUIUpdate = 0;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<VASTConcertinaPanel> c_concertinaEditor;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VASTWaveTableEditorComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

