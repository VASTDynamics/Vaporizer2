/*
VAST Dynamics
*/

#ifndef VASTIMAGEBUTTON_H_INCLUDED
#define VASTIMAGEBUTTON_H_INCLUDED

#include "../../Engine/VASTEngineHeader.h"
#include "VASTPopupHandler.h"

class VASTImageButton : public ImageButton

{
public:
	VASTImageButton(const juce::String &componentName);

	~VASTImageButton();

	void setAudioProcessor(VASTAudioProcessor &processor);
	VASTAudioProcessor* getAudioProcessor();

private:
	void mouseDrag(const MouseEvent &e) override;
	//void mouseDown(const MouseEvent &e);

	VASTPopupHandler vastPopupHandler;
	VASTAudioProcessor *m_processor;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VASTImageButton)
};
#endif