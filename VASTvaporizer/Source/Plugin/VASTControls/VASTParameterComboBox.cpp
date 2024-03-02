/*
VAST Dynamics
*/

#include "VASTParameterComboBox.h"
#include "../../Engine/VASTEngineHeader.h"
#include "../VASTAudioProcessor.h"
#include "../VASTAudioProcessorEditor.h"

VASTParameterComboBox::VASTParameterComboBox(const juce::String &componentName) {
	this->setComponentID("");
	this->setName(componentName);
	vastPopupHandler.setComponentName(componentName);
	this->addMouseListener(&vastPopupHandler, true);

	setRepaintsOnMouseActivity(true); //performance
	setBufferedToImage(true); //performance

	m_processor = nullptr;
}

VASTParameterComboBox ::~VASTParameterComboBox() {
	this->setLookAndFeel(nullptr);
	comboBoxAttachment = nullptr;
}

void VASTParameterComboBox::showPopup()
{
	auto menu = ComboBox::getRootMenu();
	if (menu->getNumItems() > 0)
	{
		auto selectedId = getSelectedId();
		for (PopupMenu::MenuItemIterator iterator(*menu, true); iterator.next();)
		{
			auto& item = iterator.getItem();			
			if (item.itemID != 0)
				item.isTicked = (item.itemID == selectedId);
		}
	}
	else
	{
		menu->addItem(1, this->getTextWhenNothingSelected(), false, false);
	}

	//int result = 0;
	menu->setLookAndFeel(&getLookAndFeel());
	menu->showMenuAsync(PopupMenu::Options().withTargetComponent(this)
		.withItemThatMustBeVisible(getSelectedId())
		.withMinimumWidth(getWidth() * 2.0f)				//HACK TODO
		.withMaximumNumColumns(1)
		.withStandardItemHeight(this->getHeight()),
		[=](int result) { 		
			((ComboBox*)this)->hidePopup();
			if (result != 0)
				((ComboBox*)this)->setSelectedId(result);
			}		
	);
}
void VASTParameterComboBox::bindParameter(VASTAudioProcessorEditor* editor, const String& newID, VASTGUIRuntimeModel::GUIComponents guiComponent, int tabNo) {
	ComboBox::setComponentID(newID); //call super
	if (m_processor != nullptr) { //set AudioProcessor has to be called before
		this->clear();
		auto* param = m_processor->getParameterTree().getParameter(newID);
		if (param == nullptr) {
			vassert(false);
			return;
		}     
		setTooltip(m_processor->getParameters()[param->getParameterIndex()]->getName(1024));

		NormalisableRange<float> range = m_processor->getParameterTree().getParameterRange(this->getComponentID());
		if ((range.interval) == 1.0f) {
			for (int i = 0; i <= int(range.end); i += int(range.interval)) {
				String text = param->getText(range.convertTo0to1(float(i)), 128);
				this->addItem(text, i + 1);
			}
		}
		comboBoxAttachment.reset(new VASTComboBoxAttachment(m_processor->getParameterTree(), this->getComponentID(), (juce::ComboBox&)*this));

		//do sorting
		if (newID.startsWith("m_uModMatDest")) {
			StringArray addedSubmenus;
			std::vector<PopupMenu> menus;
			auto menu = getRootMenu();
			PopupMenu oldmenu = *menu;
			menu->clear();
			for (PopupMenu::MenuItemIterator iterator(oldmenu, true); iterator.next();)
			{
				auto& item = iterator.getItem();
				if ((item.text.containsIgnoreCase("OscA")) ||
					(item.text.containsIgnoreCase("OscB")) ||
					(item.text.containsIgnoreCase("OscC")) ||
					(item.text.containsIgnoreCase("OscD")) ||
					(item.text.containsIgnoreCase("MSEG")) ||
					(item.text.containsIgnoreCase("LFO1")) ||
					(item.text.containsIgnoreCase("LFO2")) ||
					(item.text.containsIgnoreCase("LFO3"))  ||
					(item.text.containsIgnoreCase("LFO4")) ||
					(item.text.containsIgnoreCase("LFO5")) ||
					(item.text.containsIgnoreCase("Filter1")) ||
					(item.text.containsIgnoreCase("Filter2")) ||
					(item.text.containsIgnoreCase("Filter3")) ||
					(item.text.containsIgnoreCase("ModMatrix")) ||
					(item.text.containsIgnoreCase("Sampler")) ||
					(item.text.containsIgnoreCase("Arp")) ||
					(item.text.containsIgnoreCase("StepSeq")) ||
					(item.text.containsIgnoreCase("EQ")) ||
					(item.text.containsIgnoreCase("Chorus")) ||
					(item.text.containsIgnoreCase("Flanger")) ||
					(item.text.containsIgnoreCase("Distortion")) ||
					(item.text.containsIgnoreCase("Compressor")) ||
					(item.text.containsIgnoreCase("Reverb")) ||
					(item.text.containsIgnoreCase("Delay")) ||
					(item.text.containsIgnoreCase("Atomizer")) ||
					(item.text.containsIgnoreCase("Comb")) ||
					(item.text.containsIgnoreCase("FormantFilter")) ||
					(item.text.containsIgnoreCase("Limiter")) ||
					(item.text.containsIgnoreCase("Bitcrush")) ||
					(item.text.containsIgnoreCase("Phaser")) ||
					(item.text.containsIgnoreCase("Waveshaper")) ||
					(item.text.containsIgnoreCase("Noise")))
				{
					String token = "";
					if (item.text.containsIgnoreCase("OscA"))
						token = "OscA";
					else if (item.text.containsIgnoreCase("OscB"))
						token = "OscB";
					else if (item.text.containsIgnoreCase("OscC"))
						token = "OscC";
					else if (item.text.containsIgnoreCase("OscD"))
						token = "OscD";
					else if (item.text.containsIgnoreCase("MSEG"))
						token = "MSEG";
					else if (item.text.containsIgnoreCase("LFO1"))
						token = "LFO1";
					else if (item.text.containsIgnoreCase("LFO2"))
						token = "LFO2";
					else if (item.text.containsIgnoreCase("LFO3"))
						token = "LFO3";
					else if (item.text.containsIgnoreCase("LFO4"))
						token = "LFO4";
					else if (item.text.containsIgnoreCase("LFO5"))
						token = "LFO5";
					else if (item.text.containsIgnoreCase("Noise"))
						token = "Noise";
					else if (item.text.containsIgnoreCase("Sampler"))
						token = "Sampler";
					else if (item.text.containsIgnoreCase("ModMatrix"))
						token = "ModMatrix";
					else if (item.text.containsIgnoreCase("Filter1"))
						token = "Filter1";
					else if (item.text.containsIgnoreCase("Filter2"))
						token = "Filter2";
					else if (item.text.containsIgnoreCase("Filter3"))
						token = "Filter3";
					else if (item.text.containsIgnoreCase("Arp"))
						token = "Arp";
					else if (item.text.containsIgnoreCase("StepSeq"))
						token = "StepSeq";
					else if ((item.text.containsIgnoreCase("EQ")) ||
							(item.text.containsIgnoreCase("Chorus")) ||
							(item.text.containsIgnoreCase("Flanger")) ||
							(item.text.containsIgnoreCase("Distortion")) ||
							(item.text.containsIgnoreCase("Compressor")) ||
							(item.text.containsIgnoreCase("Reverb")) ||
							(item.text.containsIgnoreCase("Delay")) ||
							(item.text.containsIgnoreCase("Comb")) ||
							(item.text.containsIgnoreCase("FormantFilter")) ||
							(item.text.containsIgnoreCase("Limiter")) ||
							(item.text.containsIgnoreCase("Bitcrush")) ||
							(item.text.containsIgnoreCase("Phaser")) ||
							(item.text.containsIgnoreCase("Waveshaper")) ||
							(item.text.containsIgnoreCase("Atomizer")))
						token = "FX";


					int newItemID = MODMATDEST_Array.indexOf(item.text) + 1; //    // IDs must be non-zero, as zero is used to indicate a lack of selection.
					vassert(newItemID > 0);

					PopupMenu submenu; 
					if (!addedSubmenus.contains(token, true)) {
						//menu->addSectionHeader(token);
						addedSubmenus.add(token);
						menus.push_back(submenu);		
					}
					int i = addedSubmenus.indexOf(token, true);
					//menus[i].addItem(item.itemID, item.text);
					menus[i].addItem(newItemID, item.text);
					
				}
				else {
					menu->addItem(item.itemID, item.text);
				}
			}

			for (int i = 0; i < menus.size(); i++) {
				menu->addSubMenu(addedSubmenus[i], menus[i], true);
			}
		}
		else if (newID.startsWith("m_uFilterType")) {
			StringArray addedSubmenus;
			std::vector<PopupMenu> menus;
			auto menu = getRootMenu();
			PopupMenu oldmenu = *menu;
			menu->clear();
			for (PopupMenu::MenuItemIterator iterator(oldmenu, true); iterator.next();)
			{
				auto& item = iterator.getItem();
				String token = "";
				if (item.text.startsWith("DQF"))
					token = TRANS("Digital Quad Filters");
				else if (item.text.startsWith("BQF"))
					token = TRANS("Biquad Filters");
				else if (item.text.startsWith("LDF"))
					token = TRANS("Ladder Filters");
				else if (item.text.startsWith("SVF"))
					token = TRANS("State Variable Filters");
				//else if (item.text.startsWith("KRG"))
				//	token = TRANS("Korg(tm) Emulated Filters");
				else if (item.text.startsWith("COMB") || (item.text.startsWith("SCREAM")) || (item.text.startsWith("KRG")))
					token = TRANS("Special Filters");

				PopupMenu submenu;
				if (!addedSubmenus.contains(token, true)) {
					//menu->addSectionHeader(token);
					addedSubmenus.add(token);
					menus.push_back(submenu);
				}
				int i = addedSubmenus.indexOf(token, true);
				menus[i].addItem(item.itemID, item.text);
			}
			for (int i = 0; i < menus.size(); i++) {
				menu->addSubMenu(addedSubmenus[i], menus[i], true);
			}
		}
		else { //all others
			auto menu = getRootMenu();
			int i = 1; // IDs must be non-zero, as zero is used to indicate a lack of selection.
			for (PopupMenu::MenuItemIterator iterator(*menu, true); iterator.next();)
			{
				auto& item = iterator.getItem();
				item.itemID = i;
				i++;
			}
		}
	}
	else
		vassert(m_processor != nullptr);
}

void VASTParameterComboBox::setAudioProcessor(VASTAudioProcessor &processor) {
	m_processor = &processor;
}
VASTAudioProcessor* VASTParameterComboBox::getAudioProcessor() {
	return m_processor;
}

