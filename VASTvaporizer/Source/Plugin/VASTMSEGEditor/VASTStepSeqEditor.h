/*
VAST Dynamics
*/

#ifndef VASTSTEPSEQEDITOR_H_INCLUDED
#define VASTSTEPSEQEDITOR_H_INCLUDED

#include "../../Engine/VASTEngineHeader.h"
#include "../../Engine/VASTMSEGData.h"
#include "../VASTAudioProcessor.h"

class VASTStepSeqEditor : public Component, public Timer, public TooltipClient
{
public:
	VASTStepSeqEditor(AudioProcessor* processor, VASTMSEGData* data, VASTMSEGData* datalive);
	~VASTStepSeqEditor();

	VASTMSEGData& getData();
	void updateContent(bool force);
	void timerCallback() override;
	void startAutoUpdate();
	void stopAutoUpdate();
	void passBackPopupResult(float resultVal, int numIsClicked);
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
	float screenToValX(float screenX);
	float screenToValY(float screenY);
	float valToScreenX(float xVal);
	float valToScreenY(float yVal);
	void handleBorderDisplay();

    VASTAudioProcessor* myProcessor;
	VASTMSEGData* myData = nullptr;
	VASTMSEGData* myDataLive = nullptr;

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
	
	float m_screenWidthScale = 1.f;
	float m_screenHeightScale = 1.f;

	Image waveformImage;
	Image waveformImageWithBorder;
	Path m_lastLinePath;

	bool m_dirty = false;
	bool m_needsRescale = false;
	bool m_needsPositionUpdate = false;

	int m_isDragged = -1;
	bool m_isDraggedCurve = false;

	//Font myFont;
	
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VASTStepSeqEditor)
};

#endif  // VASTSTEPSEQEDITOR_H_INCLUDED
