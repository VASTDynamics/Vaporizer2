/*
VAST Dynamics
*/

#ifndef VASTBUTTON_H_INCLUDED
#define VASTBUTTON_H_INCLUDED

#include "../../Engine/VASTEngineHeader.h"
#include "VASTPopupHandler.h"

class VASTButton : public ToggleButton

{
public:
	VASTButton(const juce::String &componentName);

	~VASTButton();

	void setEditor(VASTAudioProcessorEditor &editor);
	VASTAudioProcessorEditor* getEditor();
	void setAudioProcessor(VASTAudioProcessor &processor);
	VASTAudioProcessor* getAudioProcessor();

private:
	VASTPopupHandler vastPopupHandler;
	VASTAudioProcessorEditor *m_editor;
	VASTAudioProcessor *m_processor;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VASTButton)
};
#endif