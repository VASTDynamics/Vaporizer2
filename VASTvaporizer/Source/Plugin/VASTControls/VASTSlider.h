/*
VAST Dynamics
*/

#ifndef VASTSLIDER_H_INCLUDED
#define VASTSLIDER_H_INCLUDED

#include "../../Engine/VASTEngineHeader.h"
#include "../VASTUtils/VASTLookAndFeel.h"
#include "VASTPopupHandler.h"

class VASTSlider : public Slider
{
public:
	VASTSlider(const juce::String &componentName);
	~VASTSlider();
	void setEditor(VASTAudioProcessorEditor &editor);
	VASTAudioProcessorEditor* getEditor();
	void setAudioProcessor(VASTAudioProcessor &processor);
	VASTAudioProcessor* getAudioProcessor();
	void setDefaultValue(float defaultVal);

private:
	VASTPopupHandler vastPopupHandler;
	VASTAudioProcessorEditor *m_editor;
	VASTAudioProcessor *m_processor;

	void mouseDown(const MouseEvent &e) override;
	bool hasDefaultValue = false;
	float mDefaultVal = 0.0f;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VASTSlider)
};

#endif
