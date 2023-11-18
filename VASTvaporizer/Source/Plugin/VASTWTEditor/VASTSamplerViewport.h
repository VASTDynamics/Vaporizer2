/*
VAST Dynamics
*/
#ifndef VASTSAMPLERVIEWPORT_H_INCLUDED
#define VASTSAMPLERVIEWPORT_H_INCLUDED

#include "../../Engine/VASTEngineHeader.h"
#include "../../Engine/VASTSingleNote.h"
#include "VASTFreqDomainViewport.h"
#include "VASTPositionViewport.h"

class VASTAudioProcessorEditor; //forward declaration
class VASTAudioProcessor; //forward declaration
class VASTWaveTableEditorComponent; //forward declaration

typedef struct sSelectionWav {
	int iWavSelectionStartSample = 0;
	int iWavSelectionEndSample = 0;
	int iZeroCrossings = 0;
	double dDeterminedFreq = 0.f;
	int estimatedZeroCrossingsPerCycle = 0;
	int estimatedCycleLengthinSamples = 0;
} sSelectionWav;

class VASTSamplerViewport : public Component, public FileDragAndDropTarget, public Timer, public TooltipClient
{
public:
	VASTSamplerViewport();

	~VASTSamplerViewport();

	void resized() override;
	void paint(Graphics& g) override;
	
	void timerCallback() override;
	void startAutoUpdate();
	void stopAutoUpdate();

	void visibleAreaChanged(const juce::Rectangle<int>& newVisibleArea);
	
	static void threadedUpdate(VASTSamplerViewport* viewport);
	
	void updateContent(bool force);
	void setZoomFactor(int zoom);

	void setEditor(VASTAudioProcessorEditor* editor);
	void setProcessor(VASTAudioProcessor* processor);
	void setWTEditor(VASTWaveTableEditorComponent* wtEditor);

	void selectAll();
	void selectNothing();
	void notifySelectionChanged();

	void sampleDetermineFreq();

	void setSelectionFromWavSamples(int startSample, int endSample);
	void setSelectionFromSound();
	sSelectionWav* getSelection();
	bool isInterestedInFileDrag(const StringArray& files) override;
	void filesDropped(const StringArray& files, int x, int y) override;
	void lookAndFeelChanged() override;
	bool m_needsUpdate = false;
	bool m_needsPositionUpdate = false;
	bool m_needsRescale = false;
	void updateContentAsync();
	
	String getTooltip() override;

private:
	//==============================================================================

	Image waveformImage;	
	Image waveformImageBufferOff;
	Image waveformImageWithBorder;

	VASTAudioProcessorEditor *myEditor = nullptr;
	VASTAudioProcessor *myProcessor = nullptr;
	VASTWaveTableEditorComponent *myWtEditor = nullptr;

	//Font myFont;

	sSelectionWav m_selection;

	int m_scaling = 4;
	float m_screenWidthScale = 1.f;
	float m_screenHeightScale = 1.f;

	float m_wtImageWidth = 20;
	float m_wtImageHeight = 20;
	float m_ImageTotalWidth = m_wtImageWidth + 2;
	float m_Offset = 1;
	bool m_bMouseover = false;
	void handleBorderDisplay();

	int m_lastPostMarker = 0;

	void mouseDown(const MouseEvent &e) override; 
	void mouseDrag(const MouseEvent &e) override;
	void mouseDoubleClick(const MouseEvent &e) override;
	void mouseMove(const MouseEvent& event) override;
	void mouseExit(const MouseEvent& event) override;
	void mouseWheelMove(const MouseEvent& event, const MouseWheelDetails& wheel) override;

	ULong64_t m_lastUIUpdate = 0;

	//==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VASTSamplerViewport);
};

class VASTViewport : public Viewport
{
public:
	VASTViewport(const String& componentName = String()) : Viewport(componentName) {
	};

	bool keyPressed(const KeyPress& ) override
	{
		/*
		const bool isUpDownKey = Viewport::isUpDownKeyPress(key);

		if (getVerticalScrollBar().isVisible() && isUpDownKey)
			return getVerticalScrollBar().keyPressed(key);

		const bool isLeftRightKey = Viewport::isLeftRightKeyPress(key);

		if (getHorizontalScrollBar().isVisible() && (isUpDownKey || isLeftRightKey))
			return getHorizontalScrollBar().keyPressed(key);
			*/
		return false;
	}

	void setBounds(int x, int y, int width, int height) {
		Viewport::setBounds(x, y, width, height);
		getViewedComponent()->setBounds(0, 0, getViewedComponent()->getWidth(), height);
	}

	void visibleAreaChanged(const juce::Rectangle<int>& newVisibleArea) override {
		if (this->isVisible()) {
			VASTSamplerViewport* svp = dynamic_cast<VASTSamplerViewport*>(getViewedComponent());
			if (svp != nullptr) {
				svp->visibleAreaChanged(newVisibleArea);
			}
			else {
				VASTFreqDomainViewport* fdp = dynamic_cast<VASTFreqDomainViewport*>(getViewedComponent());
				if (fdp != nullptr) {
					fdp->visibleAreaChanged(newVisibleArea);
				}
				else {
					VASTPositionViewport* pos = dynamic_cast<VASTPositionViewport*>(getViewedComponent());
					if (pos != nullptr) {
						pos->visibleAreaChanged(newVisibleArea);
					}
				}
			}
		}
	};
};


#endif
