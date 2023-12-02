/*
VAST Dynamics
*/

#ifndef VASTDRAGFX_H_INCLUDED
#define VASTDRAGFX_H_INCLUDED

#include "../../Engine/VASTEngineHeader.h"
#include "VASTPopupHandler.h"
#include "VASTImageButton.h"
#include "VASTParameterButton.h"
#include "../VASTUtils/VASTGUINotifier.h"

class VASTDragFX : public GroupComponent, public Button::Listener, public DragAndDropTarget

{
public:
	VASTDragFX(VASTAudioProcessorEditor *editor, VASTAudioProcessor *processor, int busnr, int effectno, String paramID, VASTGUINotifier* guinotifier);

	~VASTDragFX();
	void resized() override;
	void paint(Graphics& g) override;

	VASTAudioProcessor* getAudioProcessor();
	void setModString(const juce::String& dragText);
	void lookAndFeelChanged() override;
	void buttonClicked(Button* b) override;
	void buttonStateChanged(Button* b) override;

	bool isInterestedInDragSource(const SourceDetails& dragSourceDetails) override;
	void itemDropped(const SourceDetails& dragSourceDetails) override;

private:
	VASTAudioProcessor* my_processor;
	VASTAudioProcessorEditor* my_editor;
	VASTGUINotifier* myGUINotifier = nullptr;
	int myBusnr = 0;
	int myEffectno = 0;

    std::unique_ptr<VASTImageButton> ddImageButton;
    std::unique_ptr<Label> ddLabel;
    std::unique_ptr<VASTParameterButton> ddToggleButton;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VASTDragFX)
};
#endif
