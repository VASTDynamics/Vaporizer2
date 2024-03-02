/*
VAST Dynamics
*/

#ifndef VASTPARAMETERSLIDER_H_INCLUDED
#define VASTPARAMETERSLIDER_H_INCLUDED

#include "../../Engine/VASTEngineHeader.h"
#include "VASTPopupHandler.h"
#include "../../Engine/VASTPluginConstants.h"
#include "../VASTGUIRuntimeModel.h"

class VASTParameterSlider : public Slider, public DragAndDropTarget, public Timer
{
public:
	typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

	VASTParameterSlider(const juce::String &componentName);
	~VASTParameterSlider();
	VASTAudioProcessor* getAudioProcessor();
	void setAudioProcessor(VASTAudioProcessor &processor);
    void bindParameter(VASTAudioProcessorEditor* editor, const String& newID, VASTGUIRuntimeModel::GUIComponents guiComponent, int tabNo);

	bool isInterestedInDragSource(const SourceDetails& dragSourceDetails) override;
	void itemDropped(const SourceDetails& dragSourceDetails) override;
    bool shouldDrawDragImageWhenOver() override { return true; };
    void itemDragExit  (const SourceDetails&) override {    
        if (m_draganddropinterested == true) {
            m_draganddropinterested = false;
            repaint();
    }};
    void setAutomationDestination(int destination);
	void mouseDrag(const MouseEvent &e) override;
    void mouseEnter(const MouseEvent &e) override;
    void mouseExit(const MouseEvent &e) override;
	void updateContent(bool force);
	void startAutoUpdate();
	void stopAutoUpdate();

	float m_lastDrawnValPercentageUI[C_MAX_POLY] {}; //for l&f
	void paint(Graphics& g) override;
    void setHighlighted();
    bool getHighlighted();
    void clearHighlighted();
    bool m_draganddropinterested = false;

private:
	VASTPopupHandler vastPopupHandler;
	VASTAudioProcessor* m_processor = nullptr;
    VASTAudioProcessorEditor* myEditor = nullptr;
	std::unique_ptr<SliderAttachment> sliderAttachment;

	void timerCallback() override;
	bool m_timerRunning = false;
	float m_lastSrceVals[C_MAX_POLY] {};
    bool m_highlighted = false;
    
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VASTParameterSlider)
};

#endif
