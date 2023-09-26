/*
VAST Dynamics
*/
#ifndef VASTMODMATCURVEDISPLAY_H_INCLUDED
#define VASTMODMATCURVEDISPLAY_H_INCLUDED

#include "../../Engine/VASTEngineHeader.h"
#include "../../Engine/VASTSingleNote.h"

class VASTAudioProcessorEditor; //forward declaration
class VASTAudioProcessor; //forward declaration

class VASTModMatCurveDisplay : public Component, public Timer
{
public:
	VASTModMatCurveDisplay(int slot, VASTAudioProcessorEditor* editor, VASTAudioProcessor* processor);

	~VASTModMatCurveDisplay();

	void resized() override;
	void paint(Graphics& g) override;

	void updateContent(bool force);

	void startAutoUpdate();
	void stopAutoUpdate();
	void lookAndFeelChanged() override;
	void timerCallback() override;

private:
	//==============================================================================

	Image waveformImage;
	Image waveformImageBuffer;
	Image waveformImageBufferOff;
	
    VASTAudioProcessor *myProcessor = nullptr;
	VASTAudioProcessorEditor *myEditor = nullptr;
		
	void mouseDown(const MouseEvent &e) override; 
	void mouseDrag(const MouseEvent &e) override;

	double lastCurvy = -999.0;
	float lastModVal = -999.f;
	float m_lastSrceVals[C_MAX_POLY] = { 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f };
	int m_slot = 0;
	bool m_offWasPainted = false;

	float m_screenWidthScale = 1.f;
	float m_screenHeightScale = 1.f;

	//==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VASTModMatCurveDisplay);
};

#endif
