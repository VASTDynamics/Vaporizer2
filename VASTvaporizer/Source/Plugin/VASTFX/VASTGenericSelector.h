/*
VAST Dynamics Audio Software (TM)
*/

#ifndef VASTGENERICSELECTOR_H_INCLUDED
#define VASTGENERICSELECTOR_H_INCLUDED

#include "../../Engine/VASTEngineHeader.h"
#include "../VASTAudioProcessorEditor.h"
#include "../VASTUtils/VASTGUINotifier.h"
#include "../VASTControls/VASTDragFX.h"

//==============================================================================
class VASTGenericSelector : public Component, public juce::Button::Listener
{
public:
	VASTGenericSelector(VASTAudioProcessorEditor *editor, VASTAudioProcessor *processor, int busnr, VASTGUINotifier* guinotifier);
	~VASTGenericSelector();

	void buttonClicked(Button* b) override;
	void buttonStateChanged(Button* b) override;
	
	void resized() override;
	void paint(Graphics& g) override;

	float paramControlHeight = 40;
	float paramControlWidth = 80;
	float paramLabelWidth = 80;
	float paramLabelHeight = 40;
	float paramSliderWidth = 300;
	void updateContent();

private:
	VASTAudioProcessor* my_processor;
	VASTAudioProcessorEditor* my_editor;
	VASTGUINotifier* myGUINotifier = nullptr;
	int myBusnr = 0;

	//Label noParameterLabel { "noparam", "No parameters available" };

	//GroupComponent group;
	
	OwnedArray<VASTDragFX> dragFXArray;
	//OwnedArray<Button> paramButtons;
	//OwnedArray<Label> paramButtonLabels;
	//Array<Component*> controls;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VASTGenericSelector)
};

#endif
