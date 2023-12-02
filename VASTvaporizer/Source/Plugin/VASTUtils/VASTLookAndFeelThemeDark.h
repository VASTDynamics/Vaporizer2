/*
VAST Dynamics
*/
#ifndef VASTLookAndFeelThemeDark_H_INCLUDED
#define VASTLookAndFeelThemeDark_H_INCLUDED

#include "../../Engine/VASTEngineHeader.h"
#include "../VASTVUMeter.h"
#include "VASTLookAndFeel.h"

class VASTAudioProcessor; // forward declaration
class VASTAudioProcessorEditor; // forward declaration
class CVASTSettings; // forward declaration
class VASTLookAndFeelThemeDark : public VASTLookAndFeel
{
public:
	VASTLookAndFeelThemeDark(CVASTSettings& set, VASTAudioProcessor* processor);
    ~VASTLookAndFeelThemeDark();
  
	Colour findVASTColour(int colourID);
};

#endif  // VASTLookAndFeelThemeDark_H_INCLUDED
