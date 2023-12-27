/*
VAST Dynamics
*/

#include "VASTPopupHandler.h"
#include "../../Engine/VASTEngineHeader.h"
#include "../VASTAudioProcessor.h"
#include "../VASTAudioProcessorEditor.h"
#include "VASTManualValueEditor.h"
#include "VASTManualDualValueEditor.h"
#include "../VASTPreset/VASTPresetComponent.h"
#include "../VASTUtils/VASTLookAndFeel.h"
#include "../../Engine/VASTParamState.h"

VASTPopupHandler::VASTPopupHandler() {
	m_valueWindow = NULL;
}

void VASTPopupHandler::setComponentName(const String cName) {
	m_cName = cName;
}

void VASTPopupHandler::drawValueBubble(Component* slider) {
	VASTParameterSlider* _slider = dynamic_cast<VASTParameterSlider*>(slider);
	if (_slider != NULL) {
		// check the mod keys ..
		String lCurValue;
		AudioProcessorParameterWithID* param = _slider->getAudioProcessor()->getParameterTree().getParameter(slider->getComponentID());
		lCurValue = param->getText(param->getValue(), 200);

		VASTAudioProcessorEditor* myEditor = (VASTAudioProcessorEditor*)_slider->getAudioProcessor()->getActiveEditor();
		if (m_valueWindow == NULL) {
			m_valueWindow = std::make_unique<BubbleMessageComponent>();
			m_valueWindow->setLookAndFeel(myEditor->getCurrentVASTLookAndFeel());
			m_valueWindow->setRepaintsOnMouseActivity(false);
			//m_valueWindow->setBounds(0, 0, 100 * myEditor->getProcessor()->getPluginScaleWidthFactor(), 10 * myEditor->getProcessor()->getPluginScaleHeightFactor());
			//slider->getParentComponent()->addChildComponent(m_valueWindow);
			myEditor->vaporizerComponent->addChildComponent(m_valueWindow.get());
		}
		juce::AttributedString sText(lCurValue);
		sText.setJustification(Justification::centred);
		sText.setColour(juce::Colour::fromRGBA(255, 255, 255, 255));
		Font font = myEditor->getCurrentVASTLookAndFeel()->getDefaultFont();
		font.setSizeAndStyle(myEditor->getCurrentVASTLookAndFeel()->m_lastTextEditorFontHeight * 0.75f, font.getStyleFlags(), 1.0f, 0.0f);
		sText.setFont(font);		
		m_valueWindow->showAt(slider, sText, 2000, true, false);
	}
	else {
		VASTSlider* _slider = dynamic_cast<VASTSlider*>(slider);
		if (_slider != NULL) {
			// check the mod keys ..
			String lCurValue;
			lCurValue = String(_slider->getValue());

			VASTAudioProcessorEditor* myEditor = (VASTAudioProcessorEditor*)_slider->getAudioProcessor()->getActiveEditor();
			if (m_valueWindow == NULL) {
				m_valueWindow = std::make_unique<BubbleMessageComponent>();
				m_valueWindow->setLookAndFeel(myEditor->getCurrentVASTLookAndFeel());
				m_valueWindow->setRepaintsOnMouseActivity(false);
				myEditor->vaporizerComponent->addChildComponent(m_valueWindow.get());
			}
			juce::AttributedString sText(lCurValue);
			sText.setJustification(Justification::centred);
			Font font = myEditor->getCurrentVASTLookAndFeel()->getDefaultFont();
			font.setSizeAndStyle(myEditor->getCurrentVASTLookAndFeel()->m_lastTextEditorFontHeight * 0.75f, font.getStyleFlags(), 1.0f, 0.0f);
			sText.setFont(font);
			sText.setColour(juce::Colour::fromRGBA(255, 255, 255, 255));
			m_valueWindow->showAt(slider, sText, 2000, true, false);
		}
	}
}

void VASTPopupHandler::startAutoUpdate() {
	startTimer(50);     
}

void VASTPopupHandler::stopAutoUpdate() {
	stopTimer();
}

void VASTPopupHandler::timerCallback() {
	if (m_lastComponent == nullptr)
		return;
	drawValueBubble(m_lastComponent);
}

void VASTPopupHandler::mouseDrag(const MouseEvent &e) { // show value
	m_lastComponent = e.eventComponent;
	ModifierKeys modifiers = ModifierKeys::getCurrentModifiersRealtime();
	if ((m_isBeingDragged == false) && (modifiers.isLeftButtonDown()))
		startAutoUpdate();
	m_isBeingDragged = true;
	e.eventComponent->mouseDrag(e);   // do the usual thing .... drag the slider
	
	//if ((e.eventTime.toMilliseconds() - m_lastEventTime.toMilliseconds()) >= 100) { //every x ms repainted
	//	VDBG("MouseDrag Paint");
	//	drawValueBubble(e, modifiers);
	//}
}

void VASTPopupHandler::mouseUp(const MouseEvent &e) {
	if (m_isBeingDragged) {
		stopAutoUpdate();
		m_isBeingDragged = false;
		m_lastComponent = nullptr;
	}
}

void VASTPopupHandler::mouseDown(const MouseEvent &e) {
	ModifierKeys modifiers = ModifierKeys::getCurrentModifiersRealtime();

	// check the mod keys ..
	bool isParameterSlider = false;
	if (modifiers.isPopupMenu() || modifiers.isCtrlDown()) {
		String lLabel;
		String lCurValue;
		String compId = "";
		Component* _comp = nullptr;
		VASTAudioProcessorEditor* myEditor;
		VASTParameterSlider* _slider = dynamic_cast<VASTParameterSlider*>(e.eventComponent);
		if (_slider != NULL) {
			myEditor = (VASTAudioProcessorEditor*)_slider->getAudioProcessor()->getActiveEditor();
			compId = e.eventComponent->getComponentID();
			_comp = e.eventComponent;
			isParameterSlider = true;
		}
		else {
			VASTParameterSlider* _slider = dynamic_cast<VASTParameterSlider*>(e.eventComponent->getParentComponent());
			if (_slider != NULL) {
				myEditor = (VASTAudioProcessorEditor*)_slider->getAudioProcessor()->getActiveEditor();
				compId = e.eventComponent->getParentComponent()->getComponentID();
				_comp = e.eventComponent->getParentComponent();
				isParameterSlider = true;
			}
			else {
				VASTParameterComboBox* _combobox = dynamic_cast<VASTParameterComboBox*>(e.eventComponent); //It is a VASTComboBox
				if (_combobox != NULL) {
					myEditor = (VASTAudioProcessorEditor*)_combobox->getAudioProcessor()->getActiveEditor();
					compId = e.eventComponent->getComponentID();
					_comp = e.eventComponent;
				}
				else {
					VASTParameterComboBox* _combobox = dynamic_cast<VASTParameterComboBox*>(e.eventComponent->getParentComponent());
					if (_combobox != NULL) {
						myEditor = (VASTAudioProcessorEditor*)_combobox->getAudioProcessor()->getActiveEditor();
						compId = e.eventComponent->getParentComponent()->getComponentID();
						_comp = e.eventComponent->getParentComponent();
					}
					else {
						VASTParameterButton* _button = dynamic_cast<VASTParameterButton*>(e.eventComponent);
						if (_button != NULL) {
							myEditor = (VASTAudioProcessorEditor*)_button->getAudioProcessor()->getActiveEditor();
							compId = e.eventComponent->getComponentID();
							_comp = e.eventComponent;
						}
						else {
							VASTParameterButton* _button = dynamic_cast<VASTParameterButton*>(e.eventComponent->getParentComponent());
							if (_button != NULL) {
								myEditor = (VASTAudioProcessorEditor*)_button->getAudioProcessor()->getActiveEditor();
								compId = e.eventComponent->getParentComponent()->getComponentID();
								_comp = e.eventComponent->getParentComponent();
							}
							else {
								VASTSlider* _nonparamslider = dynamic_cast<VASTSlider*>(e.eventComponent);
								if (_nonparamslider != NULL) {
									myEditor = (VASTAudioProcessorEditor*)_nonparamslider->getAudioProcessor()->getActiveEditor();
									compId = "";
									_comp = e.eventComponent;
									lCurValue = String(_nonparamslider->getValue());
								}
								else {
									VASTSlider* _nonparamslider = dynamic_cast<VASTSlider*>(e.eventComponent->getParentComponent());
									if (_nonparamslider != NULL) {
										myEditor = (VASTAudioProcessorEditor*)_nonparamslider->getAudioProcessor()->getActiveEditor();
										compId = "";
										_comp = e.eventComponent->getParentComponent();
										lCurValue = String(_nonparamslider->getValue());
									}

									else {
										return;
									}
								}
							}
						}
					}
				}
			}
		}

		VASTAudioProcessor* myProcessor = myEditor->getProcessor();
		PopupMenu mainMenu;

		mainMenu.setLookAndFeel(myEditor->getCurrentVASTLookAndFeel());

		e.eventComponent->mouseDown(e);   // to the usual thing .... drag the slider

		float modStart = 0.f;
		float modEnd = 0.f;
		int modPolarity = 0;

		AudioProcessorParameterWithID* param = nullptr;
		if (!compId.equalsIgnoreCase("")) {			
			param = myProcessor->getParameterTree().getParameter(compId);
			String lDescStr = param->name;
			lCurValue = param->getText(param->getValue(), 200);
			lLabel = param->label;

			bool isDDTarget = false;
			bool isModulated = false;
			if (isParameterSlider) {
				VASTParameterSlider* _parameterslider = (VASTParameterSlider*)_comp;
				String cid = _parameterslider->getComponentID();
				if (cid.endsWith("_bus2") || cid.endsWith("_bus3")) {
					cid = cid.dropLastCharacters(5);
				}
				int modmatdest = myProcessor->autoParamGetDestination(cid);
				if (modmatdest > 0) { //-1 and 0 are not wanted
					isDDTarget = true;
					if (myProcessor->m_pVASTXperience.m_Set.modMatrixDestinationSetFast(modmatdest)) {
						isModulated = true;
					}
				}
			}

			String lMIDIStr;			
			int lMappedCC = myProcessor->parameterIndexGetMidiMapping(param->getParameterIndex());
			if (lMappedCC >= 0)
				lMIDIStr = "CC " + String(lMappedCC);
			else
				lMIDIStr = "---";

			mainMenu.addItem(1, "Parameter description: " + lDescStr, false);
			mainMenu.addItem(2, "Parameter var. name: " + compId, false);
			mainMenu.addItem(3, "Label: " + lLabel, false);
			String targetString = isDDTarget ? "yes" : "no";
			mainMenu.addItem(5, "Cur. MIDI mapping: " + lMIDIStr, false);
			mainMenu.addItem(6, "Is modulatable: " + targetString, false);
			String modString = isModulated ? "yes" : "no";
			mainMenu.addItem(7, "Is modulated: " + modString, false);
			if (isModulated) {
				VASTParameterSlider* _parameterslider = (VASTParameterSlider*)_comp;
				String cid = _parameterslider->getComponentID();
				int modmatdest = myProcessor->autoParamGetDestination(cid);
				if (modmatdest > 0) { //-1 and 0 are not wanted
					if (myProcessor->m_pVASTXperience.m_Set.modMatrixDestinationSetFast(modmatdest)) {
						int slot = myProcessor->m_pVASTXperience.m_Set.modMatrixGetFirstSlotWithDestination(modmatdest);
						float modVal = 0.f;
						double curvy = 0.f;
						float lastSrceVals[C_MAX_POLY] {};
						int polarity = 0;
						int source = 0;
						int destination = 0;
						myProcessor->m_pVASTXperience.m_Set.modMatrixSlotGetValues(slot, modVal, curvy, source, destination, polarity, lastSrceVals);

						modVal *= 0.01f;
						juce::Range<double> range = (juce::Range<double>)_parameterslider->getRange();
						double curVal = _parameterslider->getValue();
						//double skew = _parameterslider->getSkewFactor();
						//double curValPercentage = (curVal - range.getStart()) / (range.getEnd() - range.getStart());
						//double curValPercentageUI = std::pow(curValPercentage, skew);

						modPolarity = polarity;
						modStart = curVal - (range.getEnd() - range.getStart()) * modVal;
						if (modStart > range.getEnd()) modStart = range.getEnd();
						if (modStart < range.getStart()) modStart = range.getStart();
						modEnd = curVal + (range.getEnd() - range.getStart()) * modVal;
						if (modEnd > range.getEnd()) modEnd = range.getEnd();
						if (modEnd < range.getStart()) modEnd = range.getStart();
						if (polarity == POLARITY::Unipolar) {
							modStart = curVal;
						}						
						mainMenu.addItem(8, "Main modulation source: " + CVASTParamState::comboBoxValueToTextFunction_MODMATSRCE(source), false);
						mainMenu.addItem(9, "Modulation matrix value: " + String(modVal), false);
						mainMenu.addItem(10, "Modulation start value: " + String(modStart), false);
						mainMenu.addItem(11, "Modulation end value: " + String(modEnd), false);
						mainMenu.addItem(12, "Modulation polarity: " + String(modPolarity == POLARITY::Bipolar ? "Bipolar" : "Unipolar"), false);
						mainMenu.addItem(13, "Type in mod start and end values");
					}
				}
			}	
			mainMenu.addSeparator();
		}

		mainMenu.addItem(20, "Cur. value: " + lCurValue, false);
		mainMenu.addItem(21, "Type in value"); 

		if (!compId.equalsIgnoreCase("")) {
			mainMenu.addItem(22, "Reset to default (same as doubleclick)");
			mainMenu.addItem(23, "Remove all modulations");
		}
		mainMenu.addSeparator();
		mainMenu.addItem(25, "Learn MIDI");
		mainMenu.addItem(26, "Forget MIDI mapping");
		mainMenu.addSeparator();
		mainMenu.addItem(27, "Reset all MIDI mappings to defaults");
		mainMenu.addItem(28, "Reset all MIDI mappings to not-mapped");
		
		//const int result = mainMenu.showAt(_comp);
		mainMenu.showMenuAsync(PopupMenu::Options().withTargetComponent(_comp).withTargetScreenArea(juce::Rectangle<int>{}.withPosition(Desktop::getMousePosition())),
			juce::ModalCallbackFunction::create([this, param, myProcessor, myEditor, _comp, modStart, modEnd, modPolarity, lCurValue, e, mainMenu](int result) {
			if (result == 0)
			{
				// user dismissed the menu without picking anything
			}
			else if (result == 13)
			{
				String pid = "";
				if (param != nullptr) pid = param->paramID;
				std::unique_ptr<VASTManualDualValueEditor> l_veditor = std::make_unique<VASTManualDualValueEditor>(myProcessor, pid, _comp); //OK will be owned by the cb
				l_veditor->setLookAndFeel(myEditor->getCurrentVASTLookAndFeel());
				l_veditor->setSize(300.f * myProcessor->getPluginScaleWidthFactor(), 60.f * myProcessor->getPluginScaleHeightFactor());
				l_veditor->setOpaque(true);

				Point<float> cPoint = myEditor->vaporizerComponent->getLocalPoint(_comp, Point<float>(0.f, 0.f));
				juce::Rectangle<int> newBounds = juce::Rectangle<int>(cPoint.getX(), cPoint.getY(), _comp->getWidth(), _comp->getHeight());

				l_veditor->setTextStart(String(modStart));
				l_veditor->setTextEnd(String(modEnd));
				l_veditor->setPolarity(modPolarity);

				juce::CallOutBox::launchAsynchronously(std::move(l_veditor), newBounds, myEditor->vaporizerComponent.get());

			}
			else if (result == 21)
			{
				String pid = "";
				if (param != nullptr) pid = param->paramID;
				std::unique_ptr<VASTManualValueEditor> l_veditor = std::make_unique<VASTManualValueEditor>(myProcessor, pid, _comp); //OK will be owned by the cb
				l_veditor->setLookAndFeel(myEditor->getCurrentVASTLookAndFeel());
				l_veditor->setSize(300.f * myProcessor->getPluginScaleWidthFactor(), 30.f * myProcessor->getPluginScaleHeightFactor());
				l_veditor->setOpaque(true);

				Point<float> cPoint = myEditor->vaporizerComponent->getLocalPoint(_comp, Point<float>(0.f, 0.f));
				juce::Rectangle<int> newBounds = juce::Rectangle<int>(cPoint.getX(), cPoint.getY(), _comp->getWidth(), _comp->getHeight());

				l_veditor->setText(lCurValue);

				//CallOutBox &cb =
				juce::CallOutBox::launchAsynchronously(std::move(l_veditor), newBounds, myEditor->vaporizerComponent.get());

			}
			else if (result == 22)
			{
				if (param != nullptr)
					param->setValueNotifyingHost(param->getDefaultValue());
			}
			else if (result == 23)
			{
				String cid = _comp->getComponentID();
				if (cid.endsWith("_bus2") || cid.endsWith("_bus3")) {
					cid = cid.dropLastCharacters(5);
				}
				int modmatdest = myProcessor->autoParamGetDestination(cid); //name to remove _busx
				vassert(modmatdest != -1);

				myProcessor->m_pVASTXperience.m_Set.modMatrixRemoveAllForDestination(modmatdest);
				_comp->repaint();
				myProcessor->requestUIUpdate(true, true, true);
			}
			else if (result == 25)
			{
				if (myProcessor->m_iNowLearningMidiParameter != -1) {
					for (int i = 0; i < myEditor->m_VASTComponentsAll.size(); i++) {
						Component* _comp = myEditor->m_VASTComponentsAll[i];
						if (_comp->getComponentID().equalsIgnoreCase(myProcessor->m_iNowLearningMidiParameterVariableName))
							_comp->repaint(); //to prevent that it remains red
					}
				}
				if ((param != nullptr) && (_comp != nullptr)) {
					myProcessor->midiLearnParameter(param->getParameterIndex(), _comp->getComponentID());
				}
				else if (_comp != nullptr) { //non parameter
					myProcessor->midiLearnParameter(9999, _comp->getComponentID());
				}
				e.eventComponent->repaint();
			}
			else if (result == 26)
			{
				myProcessor->midiForgetParameter(_comp->getComponentID());
			}
			else if (result == 27)
			{
				myProcessor->loadDefaultMidiMapping();
				if (myProcessor->m_initCompleted.load())
					myProcessor->writeSettingsToFileAsync();
			}
			else if (result == 28)
			{
				myProcessor->loadZeroMidiMapping();
				if (myProcessor->m_initCompleted.load())
					myProcessor->writeSettingsToFileAsync();
			}
		}));		
	}
	else
	{
		e.eventComponent->mouseDown(e);   // to the usual thing .... drag the slider
		if (Label* _label = dynamic_cast<Label*>(e.eventComponent))
			e.eventComponent->getParentComponent()->mouseDown(e);   // if combo brev then the label was pressed - need super
	}
}
