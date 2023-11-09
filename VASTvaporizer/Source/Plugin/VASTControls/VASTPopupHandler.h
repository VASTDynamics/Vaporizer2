/*
VAST Dynamics
*/

#ifndef VASTPOPUPHANDLER_H_INCLUDED
#define VASTPOPUPHANDLER_H_INCLUDED

#include "../../Engine/VASTEngineHeader.h"
#include "../VASTUtils/VASTLookAndFeel.h"

class VASTAudioProcessor; //forward decklaration
class VASTAudioProcessorEditor; //forward decklaration
class VASTManualValueEditor; //forward decklaration
class VASTPopupHandler : public MouseListener, public Timer
{
public:
	VASTPopupHandler();
	void setComponentName(const String cName);

	void timerCallback() override;	
	void startAutoUpdate();
	void stopAutoUpdate();

private:
	void mouseDown(const MouseEvent &e) override; // open popup menu
	void mouseUp(const MouseEvent &e) override; 
	void mouseDrag(const MouseEvent &e) override; // show value
	void drawValueBubble(Component* slider);

	String m_cName = "";
	Component* m_lastComponent = nullptr;
	bool m_isBeingDragged = false;

    std::unique_ptr<BubbleMessageComponent> m_valueWindow;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VASTPopupHandler)
};

#endif
