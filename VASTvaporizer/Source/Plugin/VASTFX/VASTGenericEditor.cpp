/*
VAST Dynamics Audio Software (TM)
*/

#include "../../Engine/VASTEngineHeader.h"
#include "VASTGenericEditor.h"
#include "../VASTControls/VASTParameterSlider.h"
#include "../VASTControls/VASTParameterButton.h"
#include "../VASTControls/VASTParameterComboBox.h"
#include "../VASTAudioProcessor.h"
#include "../VASTAudioProcessorEditor.h"
#include "../../Engine/FX/VASTEffect.h"
#include <wchar.h>


VASTGenericEditor::VASTGenericEditor(CVASTEffect *effect, AudioProcessor *processor, AudioProcessorEditor *editor, int busNr): myEffect(effect), my_processor((VASTAudioProcessor*)processor), my_editor((VASTAudioProcessorEditor*)editor)
{
    myBusNr = busNr;
    AudioProcessorValueTreeState& parameterState = my_processor->getParameterTree();

	auto& params = effect->getParameters(); //returns them in display sequence from left to right

	group.setColour(GroupComponent::outlineColourId, my_processor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colGroupComponentOutline));
	group.setText(effect->getEffectName());
	addAndMakeVisible(group);

	for (auto p : params)
	{
		if (auto* param = dynamic_cast<AudioProcessorParameterWithID*> (p))
		{
			if (param->paramID.startsWith("m_f") || param->paramID.startsWith("m_i")) {
				VASTParameterSlider* aSlider;
				paramSliders.add(aSlider = new VASTParameterSlider(param->paramID));
				aSlider->setAudioProcessor(*my_processor);
				aSlider->bindParameter(my_editor, param->paramID, VASTGUIRuntimeModel::GUIComponents::GenericEditor, myBusNr);
				aSlider->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
				aSlider->setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
				group.addAndMakeVisible(aSlider);
				Label* aLabel;
				String lText = param->label.toUpperCase();
				//lText = (lText.length() <= 7) ? lText.paddedRight(*spaceChar, 8) : lText.dropLastCharacters(lText.length() - 7);
				paramSliderLabels.add(aLabel = new Label(param->label.toUpperCase(), lText));
				aLabel->setJustificationType(Justification::centredBottom);
				//aLabel->setColour(Label::textColourId, Colour(0xffe2e2e2));
				aLabel->setColour(Label::textColourId, my_processor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colLabelText));
				group.addAndMakeVisible(aLabel);
				controls.add(aSlider);
			}
			else if (param->paramID.startsWith("m_b")) {
				VASTParameterButton* aButton;
				paramButtons.add(aButton = new VASTParameterButton(param->paramID));
				aButton->setAudioProcessor(*my_processor);
				aButton->bindParameter(my_editor, param->paramID, VASTGUIRuntimeModel::GUIComponents::GenericEditor, myBusNr);
				group.addAndMakeVisible(aButton);
				Label* aLabel;
				String lText = param->label.toUpperCase();
				//lText = (lText.length() <= 7) ? lText.paddedRight(*spaceChar, 8) : lText.dropLastCharacters(lText.length() - 7);
				paramButtonLabels.add(aLabel = new Label(param->label.toUpperCase(), lText));
				aLabel->setJustificationType(Justification::centredBottom);
				//aLabel->setColour(Label::textColourId, Colour(0xffe2e2e2));
				aLabel->setColour(Label::textColourId, my_processor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colLabelText));
				group.addAndMakeVisible(aLabel);
				controls.add(aButton);
			}
			else if (param->paramID.startsWith("m_u")) {
				VASTParameterComboBox* aCombobox;
				paramCombobox.add(aCombobox = new VASTParameterComboBox(param->paramID));
				NormalisableRange<float> range = parameterState.getParameterRange(param->paramID);
				for (int i = 0; i <= int(range.end); i += int(range.interval)) {
					String text = param->getText(range.convertTo0to1(float(i)), 128);
					aCombobox->addItem(text, i + 1);
				}
				aCombobox->setAudioProcessor(*my_processor);
				aCombobox->bindParameter(my_editor, param->paramID, VASTGUIRuntimeModel::GUIComponents::GenericEditor, myBusNr);
				group.addAndMakeVisible(aCombobox);
				Label* aLabel;
				String lText = param->label.toUpperCase();
				//lText = (lText.length() <= 7) ? lText.paddedRight(*spaceChar, 8) : lText.dropLastCharacters(lText.length() - 7);
				paramComboBoxLabels.add(aLabel = new Label(param->label.toUpperCase(), lText));
				aLabel->setJustificationType(Justification::centredBottom);
				//aLabel->setColour(Label::textColourId, Colour(0xffe2e2e2));
				aLabel->setColour(Label::textColourId, my_processor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colLabelText));
				group.addAndMakeVisible(aLabel);
				controls.add(aCombobox);
			}
		}
	}

	noParameterLabel.setJustificationType(Justification::horizontallyCentred | Justification::verticallyCentred);
	noParameterLabel.setFont(noParameterLabel.getFont().withStyle(Font::italic));

	if (paramSliders.size() == 0)
	{
		group.addAndMakeVisible(noParameterLabel);
		setSize(300, 100);
	}
	else
	{
		setSize(paramSliderWidth + paramLabelWidth,
			jmax(100, int(paramControlHeight * paramSliders.size())));
	}
}

void VASTGenericEditor::resized()
{
	if (myEffect->isOffAndShallBeOff()) return;

	auto outside = getLocalBounds();
	group.setBounds(outside);
	auto inside = group.getLocalBounds();
	noParameterLabel.setBounds(inside);

	//int max_display_controls = 13; //check was 10
	//auto paramBounds = inside.removeFromLeft(20); //left border
	
	//int max_display_controls = float(inside.getWidth()) / float(inside.getHeight()) * 2.f + 1.f;
	int max_display_controls = 20; //compressor

	paramControlWidth = inside.getWidth() / (max_display_controls);

	for (auto i = 0; i < controls.size(); ++i)
	{

		auto paramBounds = inside.removeFromLeft(paramControlWidth); //this cuts off a piece and puts it in return
		auto paramLabelBounds = paramBounds.removeFromBottom(inside.getHeight() * 0.18f);
		paramLabelBounds.removeFromTop(paramLabelBounds.getHeight() * 0.1f);
		paramLabelBounds.removeFromBottom(paramLabelBounds.getHeight() * 0.1f);

		paramBounds.removeFromTop(paramBounds.getHeight() * 0.3f);
		paramBounds.removeFromBottom(paramBounds.getHeight() * 0.2f);

		//auto charWidth = paramLabelBounds.getWidth() / 10.f; //max 8 chars in label
		if (auto* aSlider = dynamic_cast<Slider*> (controls[i]))
		{
			auto sliderIndex = paramSliders.indexOf(aSlider);

			//int lwidth = 2.0f * paramSliderLabels[sliderIndex]->getFont().getStringWidthFloat(paramSliderLabels[sliderIndex]->getText());
			//lwidth = (lwidth < minLabelWidth) ? minLabelWidth : lwidth;
			//lwidth = (lwidth < paramLabelBounds.getWidth()) ? lwidth : paramLabelBounds.getWidth();
			//paramLabelBounds.setWidth(lwidth);

			//String lText = paramSliderLabels[sliderIndex]->getText();
			//int nwidth = lText.length();
			//paramLabelBounds.removeFromLeft(paramLabelBounds.getWidth() * (8.f - nwidth) / 20.f);
			//paramLabelBounds.removeFromRight(paramLabelBounds.getWidth() * (8.f - nwidth) / 20.f);
			paramSliderLabels[sliderIndex]->setBounds(paramLabelBounds);
			paramSliderLabels[sliderIndex]->setFont(getLookAndFeel().getLabelFont(*paramSliderLabels[sliderIndex]));
			paramSliderLabels[sliderIndex]->setCentrePosition(paramLabelBounds.getCentre().getX(), paramLabelBounds.getTopLeft().y);
			aSlider->setBounds(paramBounds);
			aSlider->setCentrePosition(paramBounds.getCentre());
		}
		else if (auto* aButton = dynamic_cast<ToggleButton*> (controls[i]))
		{
			auto buttonIndex = paramButtons.indexOf(aButton);

			//int lwidth = 2.0f * paramButtonLabels[buttonIndex]->getFont().getStringWidthFloat(paramButtonLabels[buttonIndex]->getText());
			//lwidth = (lwidth < minLabelWidth) ? minLabelWidth : lwidth;
			//lwidth = (lwidth < paramLabelBounds.getWidth()) ? lwidth : paramLabelBounds.getWidth();
			//paramLabelBounds.setWidth(lwidth);

			//String lText = paramButtonLabels[buttonIndex]->getText();
			//int nwidth = lText.length();
			//paramLabelBounds.removeFromLeft(paramLabelBounds.getWidth() * (8.f - nwidth) / 20.f);
			//paramLabelBounds.removeFromRight(paramLabelBounds.getWidth() * (8.f - nwidth) / 20.f);
			paramButtonLabels[buttonIndex]->setBounds(paramLabelBounds);
			paramButtonLabels[buttonIndex]->setFont(getLookAndFeel().getLabelFont(*paramButtonLabels[buttonIndex]));
			paramButtonLabels[buttonIndex]->setCentrePosition(paramLabelBounds.getCentre().getX(), paramLabelBounds.getTopLeft().y);

			int size = jmin(paramBounds.getWidth(), paramBounds.getHeight())*0.5f;
			aButton->setSize(size, size);
			aButton->setCentrePosition(paramBounds.getCentre());
		}
		else if (auto* aCombobox = dynamic_cast<ComboBox*> (controls[i]))
		{
			auto boxIndex = paramCombobox.indexOf(aCombobox);

			//int lwidth = 2.0f * paramComboBoxLabels[boxIndex]->getFont().getStringWidthFloat(paramComboBoxLabels[boxIndex]->getText());
			//lwidth = (lwidth < minLabelWidth) ? minLabelWidth : lwidth;
			//lwidth = (lwidth < paramLabelBounds.getWidth()) ? lwidth : paramLabelBounds.getWidth();
			//paramLabelBounds.setWidth(lwidth);

			//String lText = paramComboBoxLabels[boxIndex]->getText();
			//float nwidth = lText.length();
			//paramLabelBounds.removeFromLeft(paramLabelBounds.getWidth() * (8.f - nwidth) / 20.f);
			//paramLabelBounds.removeFromRight(paramLabelBounds.getWidth() * (8.f - nwidth) / 20.f);
			paramComboBoxLabels[boxIndex]->setBounds(paramLabelBounds);
			paramComboBoxLabels[boxIndex]->setFont(getLookAndFeel().getLabelFont(*paramComboBoxLabels[boxIndex]));
			paramComboBoxLabels[boxIndex]->setCentrePosition(paramLabelBounds.getCentre().getX(), paramLabelBounds.getTopLeft().y);

			aCombobox->setBounds(paramBounds);
			aCombobox->setSize(paramBounds.getWidth() * 0.8f, paramBounds.getHeight() * 0.25f);
			aCombobox->setCentrePosition(paramBounds.getCentre());
		}
	}
}

void VASTGenericEditor::paint(Graphics& g)
{
	g.setGradientFill(ColourGradient(
		my_processor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colFXEditorBackgroundFrom), 0.0f, 0.0f,
		my_processor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colFXEditorBackgroundTo),
		getWidth(), getHeight(), false));
	g.fillAll();
}
	
