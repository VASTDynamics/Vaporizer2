/*
VAST Dynamics
*/

#ifndef VASTARPEDITOR_H_INCLUDED
#define VASTARPEDITOR_H_INCLUDED

#include "../../Engine/VASTEngineHeader.h"
#include "../../Engine/VASTARPData.h"
#include "../VASTAudioProcessor.h"

class VASTARPEditor : public Component, public Timer, public FileDragAndDropTarget, public TooltipClient
{
public:
	VASTARPEditor(AudioProcessor* processor, VASTARPData* data, VASTARPData* datalive);
	~VASTARPEditor();

	VASTARPData& getData() { return *myData; }
	void updateContent(bool force);
	void timerCallback() override;
	void startAutoUpdate();
	void stopAutoUpdate();

	bool isInterestedInFileDrag(const StringArray& files) override;
	void filesDropped(const StringArray& files, int x, int y) override;
    String getTooltip() override;

private:
	void resized() override;

	void mouseDown(const MouseEvent &) override;
	void mouseUp(const MouseEvent & e) override;
	void mouseDrag(const MouseEvent &) override;
	void mouseMove(const MouseEvent& event) override;
	void mouseExit(const MouseEvent& event) override;
	void lookAndFeelChanged() override;
	void paint(Graphics &) override;
	
	float m_screenWidthScale = 1.f;
	float m_screenHeightScale = 1.f;

	VASTAudioProcessor* myProcessor;
    VASTARPData* myData = nullptr;
	VASTARPData* myDataLive = nullptr;

	enum SegmentOrder
	{
		kAttack,
		kDecay,
		kSustain,
		kRelease
	};

	float m_xbounds = 0.0f;
	float m_ybounds = 0.0f;
	float m_drawheight = 0.0f;
	float m_drawwidth = 0.0f;
	
	Image waveformImage;
	bool m_dirty = false;

	int m_isDragged = -1;
	bool m_isDraggedCurve = false;
	int m_mouseOverStep = -1;

	//Font myFont;
	
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VASTARPEditor)
};

#endif  // VASTARPEDITOR_H_INCLUDED
