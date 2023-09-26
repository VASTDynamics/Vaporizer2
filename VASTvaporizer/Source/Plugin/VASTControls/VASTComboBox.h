/*
VAST Dynamics
*/

#ifndef VASTCOMBOBOX_H_INCLUDED
#define VASTCOMBOBOX_H_INCLUDED

#include "../../Engine/VASTEngineHeader.h"
#include "VASTPopupHandler.h"

class VASTComboBox : public ComboBox, public LookAndFeel_V2

{
public:
	VASTComboBox(const juce::String &componentName);
	~VASTComboBox();

	void setEditor(VASTAudioProcessorEditor &editor);
	VASTAudioProcessorEditor* getEditor();
	void setAudioProcessor(VASTAudioProcessor &processor);
	VASTAudioProcessor* getAudioProcessor();

private:
	VASTPopupHandler vastPopupHandler;
	VASTAudioProcessorEditor *m_editor;
	VASTAudioProcessor *m_processor;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VASTComboBox)
};

#endif