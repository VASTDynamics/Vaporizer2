/*
VAST Dynamics
*/

#ifndef VASTMSEGEDITOR_H_INCLUDED
#define VASTMSEGEDITOR_H_INCLUDED

#include "../../Engine/VASTEngineHeader.h"
#include "../../Engine/VASTMSEGData.h"

class VASTAudioProcessor; //forward declaration
class VASTAudioProcessorEditor; //forward declaration
class VASTMSEGEditor : public Component, public Timer, public TooltipClient
{
public:
	VASTMSEGEditor(AudioProcessor* processor, AudioProcessorEditor* editor, VASTMSEGData* data, VASTMSEGData* datalive, int msegNo);
	~VASTMSEGEditor();

	VASTMSEGData& getData() { return *myData; }
	void updateContent(bool force);
	void timerCallback() override;
	void passBackPopupResult(float resultVal, bool isX, int numIsClicked);
	void startAutoUpdate();
	void stopAutoUpdate();
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
    VASTAudioProcessorEditor* myEditor;
	VASTMSEGData* myData = nullptr;
	VASTMSEGData* myDataLive = nullptr;

	enum SegmentOrder
	{
		kAttack,
		kDecay,
		kSustain,
		kRelease
	};

	float m_pointSize = 8.0f;
	float m_xbounds = 0.0f;
	float m_ybounds = 0.0f;
	float m_drawheight = 0.0f;
	float m_drawwidth = 0.0f;

	float m_screenWidthScale = 1.f;
	float m_screenHeightScale = 1.f;
	
	bool m_dirty = false;
	bool m_needsPositionUpdate = false;
	bool m_needsRescale = false;

	Image waveformImage;	
	Image waveformImageWithBorder;
	Path m_lastLinePath;
	std::chrono::high_resolution_clock::time_point t1; //DEBUG

	int myMSEGNo = -1;
	int m_numIsOver = -1;
	
	int m_isDragged = -1;
	bool m_isDraggedCurve = false;
	float m_isDraggedCurveVal = 0.5f;

	//Font myFont;
	
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VASTMSEGEditor)
};

#endif  // VASTMSEGEDITOR_H_INCLUDED
