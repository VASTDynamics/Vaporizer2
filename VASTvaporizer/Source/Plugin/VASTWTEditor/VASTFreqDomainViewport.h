/*
VAST Dynamics
*/
#ifndef VASTFREQDOMAINVIEWPORT_H_INCLUDED
#define VASTFREQDOMAINVIEWPORT_H_INCLUDED

#include "../../Engine/VASTEngineHeader.h"
#include "../../Engine/VASTSingleNote.h"

class VASTAudioProcessorEditor; //forward declaration
class VASTAudioProcessor; //forward declaration
class VASTWaveTableEditorComponent; //forward declaration

typedef struct sFreqDomainBuffer {
	int wtPos;
	std::vector<dsp::Complex<float>> domainBuffer;
} sFreqDomainBuffer;

class VASTFreqDomainViewport : public Component,
	public Timer
{
public:
	VASTFreqDomainViewport();

	~VASTFreqDomainViewport();

	void resized() override;
	void paint(Graphics& g) override;
	void timerCallback() override;

	void updateContent(bool force);
	void updateContentAsync();
	void startAutoUpdate();
	void stopAutoUpdate();
	void setZoomFactor(int zoom);

	void visibleAreaChanged(const juce::Rectangle<int>& newVisibleArea);

	void adjustFreqDomainImag(int slot, double percentage);
	void adjustFreqDomainReal(int slot, double percentage);

	static void adjustFreqDomainInternalThreaded(std::vector<sFreqDomainBuffer> domainBuffer, bool clipBins, VASTWaveTableEditorComponent* myWtEditor, VASTAudioProcessor* myProcessor);
	static dsp::Complex<float> setDomainBufferSlotImagPercentage(double percentage, dsp::Complex<float> currentSlotVal);
	static dsp::Complex<float> setDomainBufferSlotRealPercentage(double percentage, dsp::Complex<float> currentSlotVal);
	static double getDomainBufferSlotRealPercentage(dsp::Complex<float> currentSlotVal);
	static double getDomainBufferSlotImagPercentage(dsp::Complex<float> currentSlotVal);

	void setEditor(VASTAudioProcessorEditor* editor);
	void setProcessor(VASTAudioProcessor* processor);
	void setWTEditor(VASTWaveTableEditorComponent* wtEditor);

private:
	//==============================================================================
	bool m_dirty = true;

	Image waveformImage;
	VASTAudioProcessorEditor *myEditor = nullptr;
	VASTAudioProcessor *myProcessor = nullptr;
	VASTWaveTableEditorComponent *myWtEditor = nullptr;

	int m_scaling = 1;
	float m_screenWidthScale = 1.f;
	float m_screenHeightScale = 1.f;
	bool m_needsInit = true;

	float m_sliderThumbWidth = 3;
	float m_sliderWidth = 0;
	float m_reSliderBottomY = 0.0f;
	float m_reSliderTopY = 0.0f;
	float m_imSliderBottomY = 0.0f;
	float m_imSliderTopY = 0.0f;
	float maxRe = 0.f;
	float maxReDisp = 0.f;
	float maxIm = 0.f;
	float m_Offset = 1.0f;
	int m_numSliders = static_cast<int>((C_WAVE_TABLE_SIZE - 1) * 0.5f); //skip first
	int m_arrayidxWhenDragStart = 0;
	std::vector<sFreqDomainBuffer> m_domainBufferWhenDragStart;

	//void mouseDown(const MouseEvent &e) override; 
	void mouseDrag(const MouseEvent &e) override; 
	void mouseDoubleClick(const MouseEvent &e) override;
	void mouseDown(const MouseEvent &e) override;
	void mouseMove(const MouseEvent& event) override;
	void mouseExit(const MouseEvent& event) override;
	int mouseXGetBinArrayIdx(int posX);
	int m_mouseover_arrayidx = -1;

	//==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VASTFreqDomainViewport);
};

#endif
