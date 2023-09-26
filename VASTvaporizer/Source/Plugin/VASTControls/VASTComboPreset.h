/*
  ==============================================================================

    VASTComboPreset.h
    Created: 23 Feb 2015 7:08:50pm
    Author:  D026198

  ==============================================================================
*/

#ifndef VASTCOMBOPRESET_H_INCLUDED
#define VASTCOMBOPRESET_H_INCLUDED

#include "../../Engine/VASTEngineHeader.h"
#include "../VASTPreset/VASTPresetComponent.h"

class VASTComboPreset : public ComboBox
{
public:
	VASTComboPreset(const juce::String &componentName);

	~VASTComboPreset();

	void mouseDown(const MouseEvent &e) override;
	
	void removePopup();
	void showPopup() override;
	void setAudioProcessor(VASTAudioProcessor &processor);
	
	//static void comboBoxPopupMenuFinishedCallback(int result, ComboBox* combo);

private:
	VASTAudioProcessor * m_processor = nullptr;
	VASTPresetComponent* m_presetComponent = nullptr;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VASTComboPreset)
};



#endif  // VASTCOMBOPRESET_H_INCLUDED
