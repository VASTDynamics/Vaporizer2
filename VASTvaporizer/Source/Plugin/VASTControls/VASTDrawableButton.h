/*
VAST Dynamics
*/

#ifndef VASTDRAWABLEBUTTON_H_INCLUDED
#define VASTDRAWABLEBUTTON_H_INCLUDED

#include "../../Engine/VASTEngineHeader.h"
#include "../VASTAudioProcessor.h"

class VASTDrawableButton : public DrawableButton

{
public:
	VASTDrawableButton(const juce::String &componentName, const char* normalImage, const int normalImageSize, const juce::String tooltip);
	~VASTDrawableButton();

	void setAudioProcessor(VASTAudioProcessor &processor);
	VASTAudioProcessor* getAudioProcessor();
    const char* m_normalImage;
    int m_normalImageSize;
    
private:
	//void mouseDrag(const MouseEvent &e);
	//void mouseDown(const MouseEvent &e);
    
	//VASTPopupHandler vastPopupHandler;
	VASTAudioProcessor *m_processor;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VASTDrawableButton)
};
#endif
