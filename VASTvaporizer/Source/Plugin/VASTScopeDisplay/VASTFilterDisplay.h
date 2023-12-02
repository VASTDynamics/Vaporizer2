/*
VAST Dynamics
*/
#ifndef VASTFILTERDISPLAY_H_INCLUDED
#define VASTFILTERDISPLAY_H_INCLUDED

#include "../../Engine/VASTEngineHeader.h"
#include "../../Engine/VASTVcf.h"
#include "../../Engine/Filter/VASTQFilter.h"
#include <thread>

class VASTAudioProcessorEditor; //forward declaration
class VASTAudioProcessor; //forward declaration
class VASTFilterDisplay : public Component, public Timer
{
public:
	VASTFilterDisplay(VASTAudioProcessor* processor);
	~VASTFilterDisplay();
	
	void resized() override;

	void paint(Graphics& g) override;

	void timerCallback() override;
	//void updateContent(bool force);
	void doUpdates(bool force);
	void setEditor(VASTAudioProcessorEditor* editor);
	void setProcessor(VASTAudioProcessor* processor);
    void lookAndFeelChanged() override;

	static void updateThread(VASTFilterDisplay* display, bool force);
	bool b_newImageReady = false;
	bool b_alreadyPainted = false;
	std::atomic<int> m_iThreadsRunning = 0;
	bool m_bRestart = false;
	int m_lastSampleRate = -1;

	CriticalSection m_criticalSection;

private:
	//==============================================================================
	//Font myFont;
	std::unique_ptr<dsp::FFT> fft;

	float getSkewForTargetFrequency(float targetFreq);
	std::atomic<bool> deleted_ = false;
	float m_screenWidthScale = 1.f;
	float m_screenHeightScale = 1.f;

	std::unique_ptr<Image> waveformImage;
	std::unique_ptr<Image> waveformImageNext;
	std::unique_ptr<Image> waveformImageBuffer;
    std::unique_ptr<CVASTVcf> m_VCF[3];
	VASTQFilter m_QFilter;
	bool mb_init = false;

	int m_iUpdateCounter = -1;

	VASTAudioProcessorEditor *myEditor = nullptr;
	VASTAudioProcessor *myProcessor = nullptr;

	//==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VASTFilterDisplay);
};

#endif
