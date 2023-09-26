/*
VAST Dynamics
*/

#ifndef VASTCOMBOBREV_H_INCLUDED
#define VASTCOMBOBREV_H_INCLUDED

#include "../../Engine/VASTEngineHeader.h"
#include "VASTPopupHandler.h"

class VASTComboBrev : public ComboBox, public LookAndFeel_V2

{
public:
	VASTComboBrev(const juce::String &componentName);

	~VASTComboBrev();

	void resized() override;


	Label *briefLabel;

	void setEditor(VASTAudioProcessorEditor &editor);
	VASTAudioProcessorEditor* getEditor();
	void setAudioProcessor(VASTAudioProcessor &processor);
	VASTAudioProcessor* getAudioProcessor();

private:
	VASTPopupHandler vastPopupHandler;
	VASTAudioProcessorEditor *m_editor;
	VASTAudioProcessor *m_processor;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VASTComboBrev)
};

#endif