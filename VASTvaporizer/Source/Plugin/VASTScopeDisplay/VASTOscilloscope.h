/*
VAST Dynamics
*/
#ifndef VASTOSCILLOSCOPE_H_INCLUDED
#define VASTOSCILLOSCOPE_H_INCLUDED

#include "../../Engine/VASTEngineHeader.h"
#include "../../Engine/VASTSingleNote.h"

#define OGL2D_VISUAL_SIZE 512

class VASTAudioProcessorEditor; //forward declaration
class VASTAudioProcessor; //forward declaration
class VASTWaveTableEditorComponent; //forward declaration

typedef struct sSelection {
	int iScreenSelectionStart = 0;
	int iScreenSelectionEnd = 0;
	int iWTSelectionStart = 0;
	int iWTSelectionEnd = C_WAVE_TABLE_SIZE;
} sSelection;

class VASTOscilloscope : public Component, public Timer, public FileDragAndDropTarget, public TooltipClient /*, public juce::OpenGLRenderer*/
{
public:
	VASTOscilloscope();

	~VASTOscilloscope();

	void resized() override;

	void paint(Graphics& g) override;

	void timerCallback() override;
	void updateContent(bool force);
	void updateContentAsync();

	void startAutoUpdate(bool isEditor);
	void stopAutoUpdate();

	void setToOpenGLRender();
	void setToStandardRender();
	
	void setEditor(VASTAudioProcessorEditor* editor);
	void setProcessor(VASTAudioProcessor* processor);
	void setWTEditor(VASTWaveTableEditorComponent* wtEditor);

	String getTooltip() override;

	void selectAll(bool noUIUpdate);

	sSelection* getSelection() {
		jassert(m_selection.iWTSelectionStart >= 0);
		jassert(m_selection.iWTSelectionEnd <= C_WAVE_TABLE_SIZE - 1);	
		return &m_selection;
	}

	bool isInterestedInFileDrag(const StringArray& files) override;
	void filesDropped(const StringArray& files, int x, int y) override;
	//void fileDragExit(const StringArray& files) override;

	void lookAndFeelChanged() override;

private:
	//==============================================================================
	
	juce::OpenGLContext mOpenGLContext;
	//juce::OpenGLTexture mBackgroundTexture;
	//int m_count = -1;
	bool m_newImageForRepaint = true; //needed?

	std::atomic<bool> m_dirty = true;
	Image waveformImage;
	Image waveformImageWithBorder;
	Image waveformImageLastSafe;
	Image waveformImageBufferOff;
	Image waveformImagePerspectiveBuffer;
	bool m_hasWaveformImagePerspectiveBuffer = false;
	bool m_soloMode = false;

	int m_wtselstart = 0;
	int m_wtselend = 0;
	float m_val1 = 0.f;
	float m_val2 = 0.f;
	int m_rightDownY = -1;

	VASTAudioProcessorEditor *myEditor = nullptr;
	VASTAudioProcessor *myProcessor = nullptr;
	VASTWaveTableEditorComponent *myWtEditor = nullptr;
		
	sSelection m_selection;

	void mouseDown(const MouseEvent &e) override; 
	void mouseUp(const MouseEvent &e) override;
	void mouseDrag(const MouseEvent &e) override; 
	void mouseDoubleClick(const MouseEvent &e) override;
	void mouseMove(const MouseEvent&) override;
	void mouseExit(const MouseEvent &event) override;
	void mouseWheelMove(const MouseEvent &event, const MouseWheelDetails &wheel) override;

	void handleBorderDisplay();

	bool m_isBeingDragged = false;
	bool m_bMouseover = false;
	juce::Point<float> toScreenCoordinate(juce::Point<float> inCoord);
	juce::Point<float> fromScreenCoordinate(juce::Point<float> inCoord);
	juce::Point<float> toSnapToGridCoordinate(juce::Point<float> inCoord);

	void singleCycleFromMSEG(int msegNo);

	void notifySelectionChanged();

	std::atomic<float> m_safeWtPosFloat = 0.0f;
    std::atomic<float> m_safePhaseFloat = 0.0f;
    std::atomic<int> m_safeWTFXType = 0;
    std::atomic<float> m_safeWTFXVal = 0.f;

    std::atomic<float> m_screenWidthScale = 1.f;
    std::atomic<float> m_screenHeightScale = 1.f;

    std::atomic<int> m_last_active_voice = -1;
    std::atomic<int> m_last_active_counter = 10;

	juce::Array<juce::Point<float>> m_freeHandBuffer;
    std::atomic<bool> m_bLast_update_was_with_voice_playing = false;

	//==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VASTOscilloscope);
};

#endif
