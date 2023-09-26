/*
VAST Dynamics
*/

#pragma once


#include "../../Engine/VASTEngineHeader.h"

class VASTPresetComponent; //forward declaration
class VASTPresetStars : public Component

{
public:
	VASTPresetStars(VASTPresetComponent* presetComponent, String internalid, int ranking);

	~VASTPresetStars();

	void paint(Graphics& g) override;
	void resized() override;

	void mouseDown(const MouseEvent &e) override; // open popup menu
	void mouseDrag(const MouseEvent &e) override; // show value
	void update(String internalid, int ranking);
	void setRanking(int rank);

private:
	VASTPresetComponent* _presetComponent;
	String m_internalid;
	int m_ranking = 0;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VASTPresetStars)
};
