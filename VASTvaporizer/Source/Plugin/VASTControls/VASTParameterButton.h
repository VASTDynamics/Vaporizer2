/*
VAST Dynamics
*/

#ifndef VASTPARAMETERBUTTON_H_INCLUDED
#define VASTPARAMETERBUTTON_H_INCLUDED

#include "../../Engine/VASTEngineHeader.h"
#include "VASTPopupHandler.h"

class VASTParameterButton : public ToggleButton

{
public:
	typedef AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;

	VASTParameterButton(const juce::String &componentName);
	~VASTParameterButton();
	void setAudioProcessor(VASTAudioProcessor &processor);
	VASTAudioProcessor* getAudioProcessor();
	void bindParameter(const String& newID);

private:
	VASTPopupHandler vastPopupHandler;
	VASTAudioProcessor *m_processor;

	std::unique_ptr<ButtonAttachment> buttonAttachment;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VASTParameterButton)
};
#endif