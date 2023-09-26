/*
VAST Dynamics
*/

#pragma once


#include "../../Engine/VASTEngineHeader.h"
//#include "VASTPresetComponent.h"

class VASTPresetComponent; //forward declaration
class VASTPresetTreeHeader : public Component

{
public:
	VASTPresetTreeHeader(VASTPresetComponent* presetComponent, String caption);

	~VASTPresetTreeHeader();

	void paint(Graphics& g) override;
	void resized() override;

	//void mouseDown(const MouseEvent &e) override; // open popup menu
	//void mouseDrag(const MouseEvent &e) override; // show value

private:
	String m_caption = "";

	std::unique_ptr<juce::Label> headerLabel;

	VASTPresetComponent* _presetComponent = nullptr;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VASTPresetTreeHeader)
};
