/*
VAST Dynamics
*/

#ifndef VASTLFOEditor_H_INCLUDED
#define VASTLFOEditor_H_INCLUDED

#include "../../Engine/VASTEngineHeader.h"
#include "../VASTAudioProcessor.h"

typedef struct sSampleAndHold {
	int length;
	float height;
} sSampleAndHold;

class VASTLFOEditor : public Component, public Timer
{
public:
	VASTLFOEditor(AudioProcessor* processor, String suffix);
	~VASTLFOEditor();

	void updateContent(bool force);
	void timerCallback() override;
	void startAutoUpdate();
	void stopAutoUpdate();


private:

	struct drawState {
		int lfonr = 0;
		bool pervoice = false;
		MYUINT wave = 0;
		float ramp = 0;
		float freq = 0;
		MYUINT polarity = 0;
		bool sync = false;
		int beats = 0;
		float phase = 0.f;
		float lIntervalTime = 0.f;
	};
	drawState myDrawState;
	drawState getActualDrawState();
	bool checkForChanges();
	bool checkForPositionChanges();

	float m_screenWidthScale = 1.f;
	float m_screenHeightScale = 1.f;

	bool m_needsRescale = false;
	bool m_needsPositionUpdate = false;

	void resized() override;

	//void mouseDown(const MouseEvent &) override;
	//void mouseUp(const MouseEvent & e) override;
	//void mouseDrag(const MouseEvent &) override;
	//void mouseMove(const MouseEvent& event) override;
	//void mouseExit(const MouseEvent& event) override;
	void lookAndFeelChanged() override;
	void paint(Graphics &) override;
	
	void fillBuffers();
	void handleBorderDisplay();
	bool m_dispReset[C_MAX_POLY]{}; 
	float lastLFOVal[C_MAX_POLY] {};

	VASTAudioProcessor* myProcessor;
	std::vector<float> m_noise_buffer;
	std::vector<sSampleAndHold> m_sh_buffer;

	float m_xbounds = 0.0f;
	float m_ybounds = 0.0f;
	float m_drawheight = 0.0f;
	float m_drawwidth = 0.0f;

	Image waveformImage;
	Image waveformImageWithBorder;
	bool m_dirty = false;

	int m_isDragged = -1;
	bool m_isDraggedCurve = false;

	//Font myFont;

	String mySuffix = "";
	
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VASTLFOEditor)
};

#endif  // VASTLFOEditor_H_INCLUDED
