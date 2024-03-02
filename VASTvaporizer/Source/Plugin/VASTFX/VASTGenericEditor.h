/*
VAST Dynamics Audio Software (TM)
*/

#ifndef VASTGENERICEDITOR_H_INCLUDED
#define VASTGENERICEDITOR_H_INCLUDED

#include "../../Engine/VASTEngineHeader.h"
#include "../../Engine/FX/VASTEffect.h"
#include "../VASTAudioProcessor.h"
#include "../VASTAudioProcessorEditor.h"

//==============================================================================
class VASTGenericEditor : public Component
{
public:
	float paramControlHeight = 40;
	float paramControlWidth = 80;
	float paramLabelWidth = 80;
	float paramLabelHeight = 40;
	float paramSliderWidth = 300;

	VASTGenericEditor(CVASTEffect *effect, AudioProcessor *processor, AudioProcessorEditor *editor, int busNr);
	~VASTGenericEditor() {};

	void resized() override;    
	void paint(Graphics& g) override;
	
	CVASTEffect* myEffect = nullptr;

private:
	VASTAudioProcessor* my_processor = nullptr;
    VASTAudioProcessorEditor* my_editor = nullptr;
	
	Label noParameterLabel { "noparam", "No parameters available" };

	GroupComponent group;

    OwnedArray<Slider> paramSliders;
    OwnedArray<Label> paramSliderLabels;
	OwnedArray<Label> paramButtonLabels;
	OwnedArray<Label> paramComboBoxLabels;
    OwnedArray<Button> paramButtons;  
	OwnedArray<ComboBox> paramCombobox;
    Array<Component*> controls;     
    int myBusNr = 0;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VASTGenericEditor)
};

#endif
