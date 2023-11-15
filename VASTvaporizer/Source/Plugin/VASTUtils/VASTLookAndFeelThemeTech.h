/*
VAST Dynamics
*/
#ifndef VASTLOOKANDFEELTHEMETECH_H_INCLUDED
#define VASTLOOKANDFEELTHEMETECH_H_INCLUDED

#include "../../Engine/VASTEngineHeader.h"
#include "../VASTVUMeter.h"
#include "VASTLookAndFeel.h"

class VASTAudioProcessor; // forward declaration
class VASTAudioProcessorEditor; // forward declaration
class CVASTSettings; // forward declaration
class VASTLookAndFeelThemeTech : public VASTLookAndFeel
{
public:
	VASTLookAndFeelThemeTech(CVASTSettings& set, VASTAudioProcessor* processor);
	    
    ~VASTLookAndFeelThemeTech();
  
	Colour findVASTColour(int colourID);
	
};

#endif  // VASTLOOKANDFEELTHEMETECH_H_INCLUDED
