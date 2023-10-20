/*
VAST Dynamics
*/

#ifndef VASTPITCHBENDSLIDER_H_INCLUDED
#define VASTPITCHBENDSLIDER_H_INCLUDED

#include "../../Engine/VASTEngineHeader.h"
#include "../VASTUtils/VASTLookAndFeel.h"
#include "VASTPopupHandler.h"

class VASTPitchbendSlider : public Slider
{
public:
	VASTPitchbendSlider(const juce::String &componentName);
	~VASTPitchbendSlider();
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
	void mouseExit(const MouseEvent &e) override;
	void mouseUp(const MouseEvent &e) override;
	bool hasDefaultValue = false;
	float mDefaultVal = 0.0f;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VASTPitchbendSlider)
};

#endif
