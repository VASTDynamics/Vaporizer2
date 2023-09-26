
/*
VAST Dynamics
*/

#ifndef VASTSIDEPANELHEADER_H_INCLUDED
#define VASTSIDEPANELHEADER_H_INCLUDED

#include "../Engine/VASTEngineHeader.h"

class VASTVaporizerComponent; //forward declaration
class VASTSidePanelHeader : public Component
{
public:
	VASTSidePanelHeader(VASTVaporizerComponent* o);
	void paint(Graphics& g) override;
	void resized() override;
	void lookAndFeelChanged() override;
	
	void hidePanel();

	VASTVaporizerComponent* owner;
	Label titleLabel;
	ShapeButton dismissButton{ "dismissButton", Colours::lightgrey, Colours::lightgrey, Colours::white };
};

#endif
