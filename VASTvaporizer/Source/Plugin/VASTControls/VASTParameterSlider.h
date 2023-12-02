/*
VAST Dynamics
*/

#ifndef VASTPARAMETERSLIDER_H_INCLUDED
#define VASTPARAMETERSLIDER_H_INCLUDED

#include "../../Engine/VASTEngineHeader.h"
#include "VASTPopupHandler.h"
#include "../../Engine/VASTPluginConstants.h"

class VASTParameterSlider : public Slider, public DragAndDropTarget, public Timer
{
public:
	typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

	VASTParameterSlider(const juce::String &componentName);
	~VASTParameterSlider();
	VASTAudioProcessor* getAudioProcessor();
	void setAudioProcessor(VASTAudioProcessor &processor);
	void bindParameter(const String& newID);

	bool isInterestedInDragSource(const SourceDetails& dragSourceDetails) override;
	void itemDropped(const SourceDetails& dragSourceDetails) override;
	void setAutomationDestination(int destination);
	void mouseDrag(const MouseEvent &e) override;

	void updateContent(bool force);
	void startAutoUpdate();
	void stopAutoUpdate();

	float m_lastDrawnValPercentageUI[C_MAX_POLY] {}; //for l&f
	void paint(Graphics& g) override;

private:
	VASTPopupHandler vastPopupHandler;
	VASTAudioProcessor* m_processor = nullptr;
	std::unique_ptr<SliderAttachment> sliderAttachment;

	void timerCallback() override;
	bool m_timerRunning = false;
	float m_lastSrceVals[C_MAX_POLY] {};

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VASTParameterSlider)
};

#endif
