/*
VAST Dynamics
*/

#ifndef VASTDRAGSOURCE_H_INCLUDED
#define VASTDRAGSOURCE_H_INCLUDED

#include "../../Engine/VASTEngineHeader.h"
#include "VASTPopupHandler.h"
#include "VASTImageButton.h"

class VASTDragSource : public GroupComponent, Label::Listener

{
public:
	VASTDragSource(int modSource, const juce::String &componentName, const juce::String &dragText, const juce::String &tooltipText);

	~VASTDragSource();
	void resized() override;
	void paint(Graphics& g) override;

    void setAudioProcessor(VASTAudioProcessor &processor, VASTAudioProcessorEditor &editor);
	VASTAudioProcessor* getAudioProcessor();
	void setModString(const juce::String& dragText);
	void lookAndFeelChanged() override;
	void editorShown(Label *, TextEditor &) override;
	void labelTextChanged(Label* labelThatHasChanged) override;
	void setLabelDefaultText(String defaultText);
    void setHighlighted();
    void clearHighlighted();
    int getModSource();
    
private:
    void mouseEnter(const MouseEvent &e) override;
    void mouseExit(const MouseEvent &e) override;
    std::unique_ptr<VASTImageButton> ddImageButton;
    std::unique_ptr<Label> ddLabel;
	bool m_noLabel = false;
    String m_toolTipText = "";
    int m_modSource = 0;
    bool m_isHighlighted;
    
	VASTAudioProcessor *m_processor;
    VASTAudioProcessorEditor* m_editor;
    Colour m_bufferc1{};
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VASTDragSource)
};
#endif
