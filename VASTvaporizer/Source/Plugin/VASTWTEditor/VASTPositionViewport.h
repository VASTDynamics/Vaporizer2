/*
VAST Dynamics
*/
#ifndef VASTPOSITIONVIEWPORT_H_INCLUDED
#define VASTPOSITIONVIEWPORT_H_INCLUDED

#include "../../Engine/VASTEngineHeader.h"
#include "../../Engine/VASTSingleNote.h"

class VASTAudioProcessorEditor; //forward declaration
class VASTAudioProcessor; //forward declaration
class VASTWaveTableEditorComponent; //forward declaration

class VASTPositionViewport : public Component, 
	public TooltipClient,
	public Timer,
	public FileDragAndDropTarget
{
public:
	VASTPositionViewport();

	~VASTPositionViewport();

	void resized() override;
	void paint(Graphics& g) override;	
	void timerCallback() override;

	void visibleAreaChanged(const juce::Rectangle<int>& newVisibleArea);

	void updateContent(bool force);
	void startAutoUpdate();
	void stopAutoUpdate();
	void updateContentAsync();
	void setZoomFactor(int zoom);

	void setEditor(VASTAudioProcessorEditor* editor);
	void setProcessor(VASTAudioProcessor* processor);
	void setWTEditor(VASTWaveTableEditorComponent* wtEditor);

	//bool keyPressed(const KeyPress& key, Component* originatingComponent) override;

	bool isInterestedInFileDrag(const StringArray& files) override {
		for (int i = 0; i < files.size(); i++) {
			if (files[i].endsWithIgnoreCase(".wav")) return true;
			if (files[i].endsWithIgnoreCase(".aif")) return true;
			if (files[i].endsWithIgnoreCase(".flac")) return true;
			if (files[i].endsWithIgnoreCase(".mp3")) return true;
		}
		return false;
	};

	void filesDropped(const StringArray& files, int x, int y) override;
	float getPositionTotalWidth() {
		return m_ImageTotalWidth + m_Offset * 1.f;
	};

	float getScreenWidthScale() {
		return m_screenWidthScale;
	}

	String getTooltip() override;

private:
	//==============================================================================
	friend class VASTWaveTableEditorView;
	bool m_dirty = true;

	std::unique_ptr<FileChooser> myChooser;

	Image waveformImage;
	Image waveformImageBufferOff;

	VASTAudioProcessorEditor *myEditor = nullptr;
	VASTAudioProcessor *myProcessor = nullptr;
	VASTWaveTableEditorComponent *myWtEditor = nullptr;
	int getArrayIdx(float logicalX) const;

	float m_screenWidthScale = 1.f;
	float m_screenHeightScale = 1.f;

	int m_scaling = 1;
	float  m_wtImageWidth = 20;
	float m_wtImageHeight = 20;
	float m_ImageTotalWidth = m_wtImageWidth + 2;
	float m_Offset = 1;
	
	bool m_dragging = false;
	bool m_dragging_addnew = false;
	int m_arrayidxdragfrom = 0;
	int m_arrayidxdragto = 0;
	int m_mouseover_arrayidx = -1;

	void mouseDrag(const MouseEvent &e) override;
	void mouseDoubleClick(const MouseEvent &e) override;
	void mouseDown(const MouseEvent &e) override;
	void mouseUp(const MouseEvent &e) override;
	void mouseMove(const MouseEvent&) override;
	void mouseExit(const MouseEvent &event) override;
	
	bool m_needsUpdate = false;

	//==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VASTPositionViewport);
};

#endif
