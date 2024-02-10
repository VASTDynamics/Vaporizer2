/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 7.0.9

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
#include "../../Engine/VASTEngineHeader.h"
#include "../VASTAudioProcessorEditor.h"
#include "../VASTControls/VASTComboPreset.h"
#include "../VASTControls/VASTManualTagsEditor.h"
#include "../VASTControls/VASTManualRenameEditor.h"
#include "VASTPresetTreeHeader.h"
#include "VASTPresetStars.h"
//[/Headers]

#include "VASTPresetComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...

VASTValueTreeItem::VASTValueTreeItem(const ValueTree& v, VASTAudioProcessor* proc, VASTPresetComponent* presetCompopnent)
	: _processor(proc), _presetComponent(presetCompopnent), tree(v)
{
	tree.addListener(this);
	lookAndFeelChanged();
}

String VASTValueTreeItem::getUniqueName() const
{
	return tree["name"].toString();
}

String VASTValueTreeItem::getID() const
{
	return tree["id"].toString();
}

void VASTValueTreeItem::resetTree(const ValueTree& v) {
	tree = v;
	refreshSubItems();
}

bool VASTValueTreeItem::mightContainSubItems()
{
	return tree.getNumChildren() > 0;
}

void VASTValueTreeItem::itemClicked(const MouseEvent &e) {
	ModifierKeys modifiers = ModifierKeys::getCurrentModifiersRealtime();
	if (modifiers.isPopupMenu() || modifiers.isCtrlDown()) {
		if ((this->getID() == "favorites1") || (this->getID() == "favorites2") || (this->getID() == "favorites3") || (this->getID() == "favorites4") || (this->getID() == "favorites5")) {
			PopupMenu mainMenu;
			mainMenu.setLookAndFeel(_processor->getCurrentVASTLookAndFeel());
			mainMenu.addItem(1, "Remove all favorites", true);
			mainMenu.addSeparator();
			mainMenu.showMenuAsync(PopupMenu::Options().withTargetComponent(e.eventComponent).withTargetScreenArea(juce::Rectangle<int>{}.withPosition(Desktop::getMousePosition())),
				juce::ModalCallbackFunction::create([this](int result) {
				if (result == 0) {
				}
				else if (result == 1) {
					int fav = 0;
					if (this->getID() == "favorites1")
						fav = 0;
					else if (this->getID() == "favorites2")
						fav = 1;
					else if (this->getID() == "favorites3")
						fav = 2;
					else if (this->getID() == "favorites4")
						fav = 3;
					else if (this->getID() == "favorites5")
						fav = 4;
					_presetComponent->myProcessor->m_presetData.removeAllFavorites(fav);
					repaintItem();
					_presetComponent->setSearchVector();
				}
			}));
		}
		else if ((this->getID() == "midimappreseta") || (this->getID() == "midimappresetb") || (this->getID() == "midimappresetc") || (this->getID() == "midimappresetd")) {
			PopupMenu mainMenu;
			mainMenu.setLookAndFeel(_processor->getCurrentVASTLookAndFeel());
			mainMenu.addItem(1, "Delete MIDI program change mapping", true);
			mainMenu.addSeparator();

			mainMenu.showMenuAsync(PopupMenu::Options().withTargetComponent(e.eventComponent).withTargetScreenArea(juce::Rectangle<int>{}.withPosition(Desktop::getMousePosition())),
				juce::ModalCallbackFunction::create([this](int result) {

				if (result == 0) {
				}
				else if (result == 1) {
					if (this->getID() == "midimappreseta") {
						int numSelected = _presetComponent->getTreeViewFiles()->getNumSelectedItems();
						for (int i = numSelected - 1; i >= 0; i--)
							if (auto* vti = dynamic_cast<VASTValueTreeItem*> (_presetComponent->getTreeViewFiles()->getSelectedItem(i))) {
								int idx = -1;
								for (int j = 0; j < vti->getParentItem()->getNumSubItems(); j++)
									if (vti->getParentItem()->getSubItem(j) == vti) {
										idx = j;
										break;
									}
								if (idx >= 0)
									_presetComponent->myProcessor->m_presetData.removeProgramChangeData(0, idx);
							}
					}
					else if (this->getID() == "midimappresetb") {
						int numSelected = _presetComponent->getTreeViewFiles()->getNumSelectedItems();
						for (int i = numSelected - 1; i >= 0; i--)
							if (auto* vti = dynamic_cast<VASTValueTreeItem*> (_presetComponent->getTreeViewFiles()->getSelectedItem(i))) {
								int idx = -1;
								for (int j = 0; j < vti->getParentItem()->getNumSubItems(); j++)
									if (vti->getParentItem()->getSubItem(j) == vti) {
										idx = j;
										break;
									}
								if (idx >= 0)
									_presetComponent->myProcessor->m_presetData.removeProgramChangeData(1, idx);
							}
					}
					else if (this->getID() == "midimappresetc") {
						int numSelected = _presetComponent->getTreeViewFiles()->getNumSelectedItems();
						for (int i = numSelected - 1; i >= 0; i--)
							if (auto* vti = dynamic_cast<VASTValueTreeItem*> (_presetComponent->getTreeViewFiles()->getSelectedItem(i))) {
								int idx = -1;
								for (int j = 0; j < vti->getParentItem()->getNumSubItems(); j++)
									if (vti->getParentItem()->getSubItem(j) == vti) {
										idx = j;
										break;
									}
								if (idx >= 0)
									_presetComponent->myProcessor->m_presetData.removeProgramChangeData(2, idx);
							}
					}
					else if (this->getID() == "midimappresetd") {
						int numSelected = _presetComponent->getTreeViewFiles()->getNumSelectedItems();
						for (int i = numSelected - 1; i >= 0; i--)
							if (auto* vti = dynamic_cast<VASTValueTreeItem*> (_presetComponent->getTreeViewFiles()->getSelectedItem(i))) {
								int idx = -1;
								for (int j = 0; j < vti->getParentItem()->getNumSubItems(); j++)
									if (vti->getParentItem()->getSubItem(j) == vti) {
										idx = j;
										break;
									}
								if (idx >= 0)
									_presetComponent->myProcessor->m_presetData.removeProgramChangeData(3, idx);
							}
					}
					repaintItem();
					_processor->requestUIPresetUpdate();
				}
			}));
		}
		else if ((this->getID() == "presetfolder") || (this->getID() == "folder")) {
			PopupMenu mainMenu;
			mainMenu.setLookAndFeel(_processor->getCurrentVASTLookAndFeel());
			mainMenu.addItem(1, "Manage preset files and folders", true);
			mainMenu.addSeparator();
			mainMenu.showMenuAsync(PopupMenu::Options().withTargetComponent(e.eventComponent).withTargetScreenArea(juce::Rectangle<int>{}.withPosition(Desktop::getMousePosition())),
				juce::ModalCallbackFunction::create([this](int result) {
				if (result == 0) {
				}
				else if (result == 1) {
					String m_UserPresetFolder = _processor->m_UserPresetRootFolder;
					File initialFile = File(m_UserPresetFolder);

					//FileChooser myChooser(TRANS("Organize preset files and folders  (right click in dialog)"), initialFile, "*.vvp", true, false, _presetComponent);
					//if (myChooser.browseForMultipleFilesToOpen())

					myChooser = std::make_unique<FileChooser>(TRANS("Organize preset files and folders  (right click in dialog)"), initialFile, "*.vvp");
					myChooser->launchAsync(FileBrowserComponent::openMode | FileBrowserComponent::canSelectFiles | FileBrowserComponent::canSelectMultipleItems,
						[this](const FileChooser& fileChooser) {
						File wtFile(fileChooser.getResult());
						//Do nothing here
						_presetComponent->reloadPresets();
					});
				}
			}));
		}
	}
}

void VASTValueTreeItem::itemDoubleClicked(const MouseEvent &) {
	this->setSelected(false, false, NotificationType::sendNotification);
}

void VASTValueTreeItem::paintItem(Graphics& g, int width, int height)
{
	String ldrawText = tree["name"].toString();
	vComboBackgroundColor = _processor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colBoxTextBackground).darker(0.6f).withAlpha(0.7f);
	vComboTextColor = _processor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colBoxText);
	float wf = _presetComponent->myProcessor->getPluginScaleWidthFactor();
	int textindent = 0;
	if (tree["id"].toString() == "root") {
		g.setFont(Font(height * 0.5f, Font::bold));
		g.fillAll(Colours::white.withAlpha(0.6f)); //TableHeaderComponent::backgroundColourId, Colours::white.withAlpha(0.6f)
		g.setColour(Colours::black.withAlpha(0.5f)); //TableHeaderComponent::outlineColourId, Colours::black.withAlpha(0.5f)
		g.drawRect(g.getClipBounds(), 1.0f);
		g.setColour(Colours::black);
	}
	else if ((tree["id"].toString() == "presetfolder") || (tree["id"].toString() == "folder")) {
        if (_presetComponent->m_folderImg != nullptr)
            _presetComponent->m_folderImg->drawWithin(g, juce::Rectangle<float>(0, 0, 10 * wf, height), RectanglePlacement::centred, 0.6f);
		g.setColour(vComboTextColor);
		g.setFont(_presetComponent->m_font);
		if (isSelected())
			g.fillAll(_processor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colPresetComponentRowSelected));
		textindent = 10 * wf;
	}
	else if (tree["id"].toString() == "midimapping") {
		g.setColour(Colours::darkslategrey.withAlpha(0.7f).contrasting(0.4f));
		g.setFont(_presetComponent->m_font);
		if (isSelected())
			g.fillAll(_processor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colPresetComponentRowSelected));
	}
	else if (tree["id"].toString() == "midibanka") {
		g.setColour(Colours::darkslategrey.withAlpha(0.7f).contrasting(0.3f));
		g.setFont(_presetComponent->m_font);
		if (isSelected())
			g.fillAll(_processor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colPresetComponentRowSelected));
	}
	else if (tree["id"].toString() == "midibankb") {
		g.setColour(Colours::darkslategrey.withAlpha(0.7f).contrasting(0.2f));
		g.setFont(_presetComponent->m_font);
		if (isSelected())
			g.fillAll(_processor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colPresetComponentRowSelected));
	}
	else if (tree["id"].toString() == "midibankc") {
		g.setColour(Colours::darkslategrey.withAlpha(0.7f).contrasting(0.1f));
		g.setFont(_presetComponent->m_font);
		if (isSelected())
			g.fillAll(_processor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colPresetComponentRowSelected));
	}
	else if (tree["id"].toString() == "midibankd") {
		g.setColour(Colours::darkslategrey.withAlpha(0.7f).contrasting(0.0f));
		g.setFont(_presetComponent->m_font);
		if (isSelected())
			g.fillAll(_processor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colPresetComponentRowSelected));
	}
	else {
		g.setColour(vComboTextColor);
		g.setFont(_presetComponent->m_font);
		if (isSelected())
			g.fillAll(_processor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colPresetComponentRowSelected));
	}

	int numFavs = 0;
	if (tree["id"].toString() == "favorites1") {
		g.setColour(Colours::darkseagreen.darker(0.0f).withAlpha(0.7f));
		numFavs = _presetComponent->myProcessor->m_presetData.getNumFavorites(0);
		ldrawText = ldrawText + " (" + String(numFavs) + ")";
	}
	else if (tree["id"].toString() == "favorites2") {
		g.setColour(Colours::darkseagreen.darker(0.1f).withAlpha(0.7f));
		numFavs = _presetComponent->myProcessor->m_presetData.getNumFavorites(1);
		ldrawText = ldrawText + " (" + String(numFavs) + ")";
	}
	else if (tree["id"].toString() == "favorites3") {
		g.setColour(Colours::darkseagreen.darker(0.2f).withAlpha(0.7f));
		numFavs = _presetComponent->myProcessor->m_presetData.getNumFavorites(2);
		ldrawText = ldrawText + " (" + String(numFavs) + ")";
	}
	else if (tree["id"].toString() == "favorites4") {
		g.setColour(Colours::darkseagreen.darker(0.3f).withAlpha(0.7f));
		numFavs = _presetComponent->myProcessor->m_presetData.getNumFavorites(3);
		ldrawText = ldrawText + " (" + String(numFavs) + ")";
	}
	else if (tree["id"].toString() == "favorites5") {
		g.setColour(Colours::darkseagreen.darker(0.4f).withAlpha(0.7f));
		numFavs = _presetComponent->myProcessor->m_presetData.getNumFavorites(4);
		ldrawText = ldrawText + " (" + String(numFavs) + ")";
	}
	else if (tree["id"].toString() == "newest") {
		g.setColour(Colours::darkcyan.darker(0.4f).withAlpha(0.7f));
	}
	else if (tree["id"].toString() == "stars") {
		textindent = 10 * wf;
		g.setColour(Colours::white.withAlpha(0.5f));
		Path p1;
		p1.addStar(Point<float>(5.f * wf, height * 0.5f), 5, 10 * 0.2f * wf, 10 * 0.55f * wf, 0.f);
		g.fillPath(p1);
		g.setColour(Colours::yellow.darker(0.5f).withAlpha(0.7f));
	}

	g.drawText(ldrawText,
		4 * wf + textindent, 0, width - 4 * wf - textindent, height,
		Justification::centredLeft, true);
}

void VASTValueTreeItem::lookAndFeelChanged() {
	vComboBackgroundColor = _processor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colBoxTextBackground).darker(0.6f).withAlpha(0.7f);
	vComboTextColor = _processor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colBoxText);

	repaintItem();
}

void VASTValueTreeItem::itemOpennessChanged(bool isNowOpen)
{
	if (isNowOpen && getNumSubItems() == 0)
		refreshSubItems();
	else
		clearSubItems();
}

var VASTValueTreeItem::getDragSourceDescription()
{
	if (this->getID().startsWith("midimappreset")) {
		int idx = -1;
		for (int i = 0; i < this->getParentItem()->getNumSubItems(); i++)
			if (this->getParentItem()->getSubItem(i) == this) {
				idx = i;
				return this->getID()+String(idx);
				break;
			}
	}
	return "tree";
}

int VASTValueTreeItem::getItemHeight() const {
	return _presetComponent->m_fRowHeight;
}

bool VASTValueTreeItem::isInterestedInDragSource(const DragAndDropTarget::SourceDetails& dragSourceDetails)
{
	if (dragSourceDetails.description.toString().startsWith("midimappreset"))
		return true;

	return (dragSourceDetails.description.toString().dropLastCharacters(dragSourceDetails.description.toString().length() - 5) == "table");
}

void VASTValueTreeItem::itemDropped(const DragAndDropTarget::SourceDetails &dragSourceDetails, int insertIndex)
{
	if (this->getParentItem() == nullptr)
		return;
	if (dragSourceDetails.description.toString().startsWith("midimappreset")) {
		VDBG(dragSourceDetails.description.toString());
		int pos = dragSourceDetails.description.toString().substring(14).getIntValue();
		int idx = -1;
		for (int i = 0; i < this->getParentItem()->getNumSubItems(); i++) {
			VDBG(((VASTValueTreeItem*)this->getParentItem())->getID());
			if (((VASTValueTreeItem*)this->getParentItem())->getID() == "midimapping") //"MIDI Prog. Mapping" item
				break;
			if (this->getParentItem()->getSubItem(i) == this) {
				idx = i;
				break;
			}
		}
		String thisID = this->getID();
		String parentID = ((VASTValueTreeItem*)this->getParentItem())->getID();
		if (thisID == "midimapping") {
			int bankFrom = 0;
			if (dragSourceDetails.description.toString().startsWith("midimappreseta"))
				bankFrom = 0;
			else if (dragSourceDetails.description.toString().startsWith("midimappresetb"))
				bankFrom = 1;
			else if (dragSourceDetails.description.toString().startsWith("midimappresetc"))
				bankFrom = 2;
			else if (dragSourceDetails.description.toString().startsWith("midimappresetd"))
				bankFrom = 3;

			int bankTo = jlimit<int>(0, 3, insertIndex - 1);
			int lLast = _presetComponent->myProcessor->m_presetData.getProgramChangeData(bankTo).size();
			_presetComponent->myProcessor->m_presetData.moveProgramChangeData(bankFrom, bankTo, pos, lLast);
		}
		else if (idx >= 0) { //swap drop
			int bankTo = 0;
			if (parentID == "midibankb") bankTo = 1;
			else if (parentID == "midibankc") bankTo = 2;
			else if (parentID == "midibankd") bankTo = 3;
			if (dragSourceDetails.description.toString().startsWith("midimappreseta")) {
				_presetComponent->myProcessor->m_presetData.swapProgramChangeData(0, bankTo, pos, idx);
			}
			else if (dragSourceDetails.description.toString().startsWith("midimappresetb")) {
				_presetComponent->myProcessor->m_presetData.swapProgramChangeData(1, bankTo, pos, idx);
			}
			else if (dragSourceDetails.description.toString().startsWith("midimappresetc")) {
				_presetComponent->myProcessor->m_presetData.swapProgramChangeData(2, bankTo, pos, idx);
			}
			else if (dragSourceDetails.description.toString().startsWith("midimappresetd")) {
				_presetComponent->myProcessor->m_presetData.swapProgramChangeData(3, bankTo, pos, idx);
			}
		}
		else { //move drop
			int bankFrom = 0;
			if (dragSourceDetails.description.toString().startsWith("midimappreseta"))
				bankFrom = 0;
			else if (dragSourceDetails.description.toString().startsWith("midimappresetb"))
				bankFrom = 1;
			else if (dragSourceDetails.description.toString().startsWith("midimappresetc"))
				bankFrom = 2;
			else if (dragSourceDetails.description.toString().startsWith("midimappresetd"))
				bankFrom = 3;
			if (thisID == "midibanka") {
				_presetComponent->myProcessor->m_presetData.moveProgramChangeData(bankFrom, 0, pos, insertIndex);
			}
			else if (thisID == "midibankb") {
				_presetComponent->myProcessor->m_presetData.moveProgramChangeData(bankFrom, 1, pos, insertIndex);
			}
			else if (thisID == "midibankc") {
				_presetComponent->myProcessor->m_presetData.moveProgramChangeData(bankFrom, 2, pos, insertIndex);
			}
			else if (thisID == "midibankd") {
				_presetComponent->myProcessor->m_presetData.moveProgramChangeData(bankFrom, 3, pos, insertIndex);
			}
		}
		_processor->requestUIPresetUpdate();
		return;
	}

	VDBG(this->getID());

	String internalid = dragSourceDetails.description.toString().substring(6);

	//VASTValueTreeItem* droppedOnItem = (VASTValueTreeItem*)getOwnerView()->getItemOnRow(insertIndex);
	if (this->getID() == "favorites1")
		_presetComponent->myProcessor->m_presetData.setFavorite(internalid, 0);
	else if (this->getID() == "favorites2")
		_presetComponent->myProcessor->m_presetData.setFavorite(internalid, 1);
	else if (this->getID() == "favorites3")
		_presetComponent->myProcessor->m_presetData.setFavorite(internalid, 2);
	else if (this->getID() == "favorites4")
		_presetComponent->myProcessor->m_presetData.setFavorite(internalid, 3);
	else if (this->getID() == "favorites5")
		_presetComponent->myProcessor->m_presetData.setFavorite(internalid, 4);
	else if (this->getID() == "midibanka") {
		_presetComponent->myProcessor->m_presetData.setProgramChangeData(internalid, 0, insertIndex);
		_processor->requestUIPresetUpdate();
	}
	else if (this->getID() == "midimapping") { //"MIDI Prog. Mapping" item select - insert at last item in bank insertIndex -1 = a
		int bank = jlimit<int>(0, 3, insertIndex - 1);
		int lLast = _presetComponent->myProcessor->m_presetData.getProgramChangeData(bank).size();
		_presetComponent->myProcessor->m_presetData.setProgramChangeData(internalid, bank, lLast);
		_processor->requestUIPresetUpdate();
	}
	else if (this->getID() == "midimappreseta") {
		int idx = -1;
		for (int i = 0; i < this->getParentItem()->getNumSubItems(); i++)
			if (this->getParentItem()->getSubItem(i) == this) {
				idx = i;
				break;
			}
		if (idx >= 0) {
			_presetComponent->myProcessor->m_presetData.setProgramChangeData(internalid, 0, idx + 1);
			_processor->requestUIPresetUpdate();
		}
	}
	else if (this->getID() == "midibankb") {
		_presetComponent->myProcessor->m_presetData.setProgramChangeData(internalid, 1, insertIndex);
		_processor->requestUIPresetUpdate();
	}
	else if (this->getID() == "midimappresetb") {
		int idx = -1;
		for (int i = 0; i < this->getParentItem()->getNumSubItems(); i++)
			if (this->getParentItem()->getSubItem(i) == this) {
				idx = i;
				break;
			}
		if (idx >= 0) {
			_presetComponent->myProcessor->m_presetData.setProgramChangeData(internalid, 1, idx + 1);
			_processor->requestUIPresetUpdate();
		}
	}
	else if (this->getID() == "midibankc") {
		_presetComponent->myProcessor->m_presetData.setProgramChangeData(internalid, 2, insertIndex);
		_processor->requestUIPresetUpdate();
	}
	else if (this->getID() == "midimappresetc") {
		int idx = -1;
		for (int i = 0; i < this->getParentItem()->getNumSubItems(); i++)
			if (this->getParentItem()->getSubItem(i) == this) {
				idx = i;
				break;
			}
		if (idx >= 0) {
			_presetComponent->myProcessor->m_presetData.setProgramChangeData(internalid, 2, idx + 1);
			_processor->requestUIPresetUpdate();
		}
	}
	else if (this->getID() == "midibankd") {
		_presetComponent->myProcessor->m_presetData.setProgramChangeData(internalid, 3, insertIndex);
		_processor->requestUIPresetUpdate();
	}
	else if (this->getID() == "midimappresetd") {
		int idx = -1;
		for (int i = 0; i < this->getParentItem()->getNumSubItems(); i++)
			if (this->getParentItem()->getSubItem(i) == this) {
				idx = i;
				break;
			}
		if (idx >= 0) {
			_presetComponent->myProcessor->m_presetData.setProgramChangeData(internalid, 3, idx + 1);
			_processor->requestUIPresetUpdate();
		}
	}
	else if (this->getID() == "tag") {
		_presetComponent->myProcessor->m_presetData.addTag(this->getUniqueName());
		//save preset!!
		File presetFile(_presetComponent->myProcessor->m_presetData.getCurPatchData().internalid); //only curpatch!
		_processor->savePatchXML(&presetFile);
		_presetComponent->reloadPresets();
	}

	/*
	OwnedArray<ValueTree> selectedTrees;
	getSelectedTreeViewItems(*getOwnerView(), selectedTrees);

	moveItems(*getOwnerView(), selectedTrees, tree, insertIndex);
	*/
}

void VASTValueTreeItem::moveItems(TreeView& treeView, const OwnedArray<ValueTree>& items,
	ValueTree newParent, int insertIndex)
{
	if (items.size() > 0)
	{
		std::unique_ptr<XmlElement> oldOpenness(treeView.getOpennessState(false));

		for (auto* v : items)
		{
			if (v->getParent().isValid() && newParent != *v && !newParent.isAChildOf(*v))
			{
				if (v->getParent() == newParent && newParent.indexOf(*v) < insertIndex)
					--insertIndex;

				v->getParent().removeChild(*v, nullptr);
				newParent.addChild(*v, insertIndex, nullptr);
			}
		}

		if (oldOpenness.get() != nullptr)
			treeView.restoreOpennessState(*oldOpenness, false);
	}
}

void VASTValueTreeItem::getSelectedTreeViewItems(TreeView& treeView, OwnedArray<ValueTree>& items)
{
	auto numSelected = treeView.getNumSelectedItems();

	for (int i = 0; i < numSelected; ++i)
		if (auto* vti = dynamic_cast<VASTValueTreeItem*> (treeView.getSelectedItem(i)))
			items.add(new ValueTree(vti->tree));
}

void VASTValueTreeItem::itemSelectionChanged(bool ) {
	_presetComponent->setSearchVector();
}

bool VASTValueTreeItem::canBeSelected() const {
	return (tree["id"].toString() != "root");
}

void VASTValueTreeItem::refreshSubItems()
{
	clearSubItems();

	for (int i = 0; i < tree.getNumChildren(); ++i)
		addSubItem(new VASTValueTreeItem(tree.getChild(i), _processor, _presetComponent));
}

void VASTValueTreeItem::valueTreePropertyChanged(ValueTree&, const Identifier&)
{
	repaintItem();
}

void VASTValueTreeItem::valueTreeChildAdded(ValueTree& parentTree, ValueTree&) { treeChildrenChanged(parentTree); }

void VASTValueTreeItem::valueTreeChildRemoved(ValueTree& parentTree, ValueTree&, int) { treeChildrenChanged(parentTree); }

void VASTValueTreeItem::valueTreeChildOrderChanged(ValueTree& parentTree, int, int) { treeChildrenChanged(parentTree); }

void VASTValueTreeItem::treeChildrenChanged(const ValueTree& parentTree)
{
	if (parentTree == tree)
	{
		refreshSubItems();
		treeHasChanged();
		setOpen(true);
	}
}
//[/MiscUserDefs]

//==============================================================================
VASTPresetComponent::VASTPresetComponent (VASTAudioProcessor* proc, ComboBox* box)
    : myProcessor(proc), myBox(box)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    setName ("VASTPresetComponent");
    m_preset_table_component.reset (new PresetTableComponent (myProcessor, myBox, this));
    addAndMakeVisible (m_preset_table_component.get());
    m_preset_table_component->setName ("m_preset_table_component");

    m_searchText.reset (new juce::TextEditor ("m_searchText"));
    addAndMakeVisible (m_searchText.get());
    m_searchText->setTooltip (TRANS ("Enter search text to search all preset attributes"));
    m_searchText->setMultiLine (false);
    m_searchText->setReturnKeyStartsNewLine (false);
    m_searchText->setReadOnly (false);
    m_searchText->setScrollbarsShown (false);
    m_searchText->setCaretVisible (true);
    m_searchText->setPopupMenuEnabled (false);
    m_searchText->setColour (juce::TextEditor::textColourId, juce::Colour (0xffe2e2e2));
    m_searchText->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0xff091809));
    m_searchText->setColour (juce::TextEditor::highlightColourId, juce::Colour (0x40606060));
    m_searchText->setColour (juce::TextEditor::outlineColourId, juce::Colour (0x00000000));
    m_searchText->setColour (juce::CaretComponent::caretColourId, juce::Colour (0xffbfbfbf));
    m_searchText->setText (juce::String());

    label.reset (new juce::Label ("new label",
                                  TRANS ("PRESET BROWSER")));
    addAndMakeVisible (label.get());
    label->setFont (juce::Font ("Code Pro Demo", 12.00f, juce::Font::plain).withTypefaceStyle ("Bold"));
    label->setJustificationType (juce::Justification::centredLeft);
    label->setEditable (false, false, false);
    label->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    c_clearFilter.reset (new juce::TextButton ("c_clearFilter"));
    addAndMakeVisible (c_clearFilter.get());
    c_clearFilter->setButtonText (TRANS ("Clear Filter"));
    c_clearFilter->addListener (this);

    c_close.reset (new juce::TextButton ("c_close"));
    addAndMakeVisible (c_close.get());
    c_close->setButtonText (TRANS ("Close"));
    c_close->addListener (this);

    label2.reset (new juce::Label ("new label",
                                   TRANS ("Search Filter")));
    addAndMakeVisible (label2.get());
    label2->setFont (juce::Font ("Code Pro Demo", 12.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label2->setJustificationType (juce::Justification::centredRight);
    label2->setEditable (false, false, false);
    label2->setColour (juce::Label::textColourId, juce::Colour (0xffe2e2e2));
    label2->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label2->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    treeViewFiles.reset (new juce::TreeView ("treeViewFiles"));
    addAndMakeVisible (treeViewFiles.get());
    treeViewFiles->setDefaultOpenness (true);
    treeViewFiles->setColour (juce::TreeView::backgroundColourId, juce::Colour (0xa78f959c));
    treeViewFiles->setColour (juce::TreeView::linesColourId, juce::Colour (0xb2777777));

    treeViewCategory.reset (new juce::TreeView ("treeViewCategory"));
    addAndMakeVisible (treeViewCategory.get());
    treeViewCategory->setDefaultOpenness (true);
    treeViewCategory->setColour (juce::TreeView::backgroundColourId, juce::Colour (0xa78f959c));
    treeViewCategory->setColour (juce::TreeView::linesColourId, juce::Colour (0xb2777777));

    treeViewAuthor.reset (new juce::TreeView ("treeViewAuthor"));
    addAndMakeVisible (treeViewAuthor.get());
    treeViewAuthor->setDefaultOpenness (true);
    treeViewAuthor->setColour (juce::TreeView::backgroundColourId, juce::Colour (0xa78f9597));
    treeViewAuthor->setColour (juce::TreeView::linesColourId, juce::Colour (0xb2777777));

    treeViewTags.reset (new juce::TreeView ("treeViewTags"));
    addAndMakeVisible (treeViewTags.get());
    treeViewTags->setDefaultOpenness (true);
    treeViewTags->setColour (juce::TreeView::backgroundColourId, juce::Colour (0xa78f959c));
    treeViewTags->setColour (juce::TreeView::linesColourId, juce::Colour (0xb2777777));

    c_treeheader1.reset (new VASTPresetTreeHeader (this, "Folder & Favorites" ));
    addAndMakeVisible (c_treeheader1.get());
    c_treeheader1->setName ("c_treeheader1");

    c_treeheader2.reset (new VASTPresetTreeHeader (this, "Categories" ));
    addAndMakeVisible (c_treeheader2.get());
    c_treeheader2->setName ("c_treeheader2");

    c_treeheader3.reset (new VASTPresetTreeHeader (this, "Authors" ));
    addAndMakeVisible (c_treeheader3.get());
    c_treeheader3->setName ("c_treeheader3");

    c_treeheader4.reset (new VASTPresetTreeHeader (this, "Tags" ));
    addAndMakeVisible (c_treeheader4.get());
    c_treeheader4->setName ("c_treeheader4");


    //[UserPreSize]
	m_searchText->addListener(this);
	getTopLevelComponent()->addKeyListener(this);

	//Trees
	treeViewFiles->setMultiSelectEnabled(true);
	treeViewFiles->setDefaultOpenness(true);
	treeViewFiles->setRootItemVisible(false);
	treeViewCategory->setMultiSelectEnabled(true);
	treeViewCategory->setDefaultOpenness(true);
	treeViewCategory->setIndentSize(0);
	treeViewCategory->setOpenCloseButtonsVisible(false);
	treeViewCategory->setRootItemVisible(false);
	treeViewAuthor->setMultiSelectEnabled(true);
	treeViewAuthor->setDefaultOpenness(true);
	treeViewAuthor->setIndentSize(0);
	treeViewAuthor->setOpenCloseButtonsVisible(false);
	treeViewAuthor->setRootItemVisible(false);
	treeViewTags->setMultiSelectEnabled(true);
	treeViewTags->setDefaultOpenness(true);
	treeViewTags->setIndentSize(0);
	treeViewTags->setOpenCloseButtonsVisible(false);
	treeViewTags->setRootItemVisible(false);

	treeViewFiles->setRootItem(&rootItemTreeFiles);
	treeViewCategory->setRootItem(&rootItemTreeCategory);
	treeViewAuthor->setRootItem(&rootItemTreeAuthor);
	treeViewTags->setRootItem(&rootItemTreeTags);

	buildTreeContent();

	//setOpaque(true);

    //[/UserPreSize]

    setSize (1420, 653);


    //[Constructor] You can add your own custom stuff here..
	lookAndFeelChanged();
    //[/Constructor]
}

VASTPresetComponent::~VASTPresetComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
	m_searchText->setLookAndFeel(nullptr);
	this->setLookAndFeel(nullptr);
    //[/Destructor_pre]

    m_preset_table_component = nullptr;
    m_searchText = nullptr;
    label = nullptr;
    c_clearFilter = nullptr;
    c_close = nullptr;
    label2 = nullptr;
    treeViewFiles = nullptr;
    treeViewCategory = nullptr;
    treeViewAuthor = nullptr;
    treeViewTags = nullptr;
    c_treeheader1 = nullptr;
    c_treeheader2 = nullptr;
    c_treeheader3 = nullptr;
    c_treeheader4 = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void VASTPresetComponent::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    {
        int x = 0, y = 0, width = getWidth() - 0, height = proportionOfHeight (0.0858f);
        juce::Colour fillColour1 = juce::Colour (0xe056646c), fillColour2 = juce::Colour (0xff232527);
        //[UserPaintCustomArguments] Customize the painting arguments here..
    		fillColour1 = ((VASTAudioProcessorEditor*)myProcessor->getActiveEditor())->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscillatorComponentBackgroundGradientFrom);
    		fillColour2 = ((VASTAudioProcessorEditor*)myProcessor->getActiveEditor())->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscillatorComponentBackgroundGradientTo);
        //[/UserPaintCustomArguments]
        g.setGradientFill (juce::ColourGradient (fillColour1,
                                             static_cast<float> (proportionOfWidth (-0.0400f)) - 0.0f + x,
                                             static_cast<float> (proportionOfHeight (-0.0154f)) - 0.0f + y,
                                             fillColour2,
                                             static_cast<float> (proportionOfWidth (0.9467f)) - 0.0f + x,
                                             static_cast<float> (proportionOfHeight (1.0615f)) - 0.0f + y,
                                             false));
        g.fillRect (x, y, width, height);
    }

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void VASTPresetComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    m_preset_table_component->setBounds ((((proportionOfWidth (0.0000f) + juce::roundToInt (proportionOfWidth (0.1127f) * 1.0000f)) + juce::roundToInt (proportionOfWidth (0.0845f) * 1.0000f)) + juce::roundToInt (proportionOfWidth (0.0845f) * 1.0000f)) + juce::roundToInt (proportionOfWidth (0.0845f) * 1.0000f), proportionOfHeight (0.0551f), proportionOfWidth (0.6310f), proportionOfHeight (0.9449f));
    m_searchText->setBounds (proportionOfWidth (0.2084f), proportionOfHeight (0.0123f), proportionOfWidth (0.4176f), proportionOfHeight (0.0276f));
    label->setBounds (proportionOfWidth (0.0028f), proportionOfHeight (0.0138f), proportionOfWidth (0.1380f), proportionOfHeight (0.0245f));
    c_clearFilter->setBounds (proportionOfWidth (0.6373f), proportionOfHeight (0.0107f), proportionOfWidth (0.0444f), proportionOfHeight (0.0276f));
    c_close->setBounds (proportionOfWidth (0.9507f), proportionOfHeight (0.0107f), proportionOfWidth (0.0444f), proportionOfHeight (0.0276f));
    label2->setBounds (proportionOfWidth (0.1275f), proportionOfHeight (0.0169f), proportionOfWidth (0.0768f), proportionOfHeight (0.0169f));
    treeViewFiles->setBounds (proportionOfWidth (0.0000f), proportionOfHeight (0.0858f), proportionOfWidth (0.1127f), proportionOfHeight (0.9142f));
    treeViewCategory->setBounds (proportionOfWidth (0.0000f) + juce::roundToInt (proportionOfWidth (0.1127f) * 1.0000f), proportionOfHeight (0.0858f), proportionOfWidth (0.0845f), proportionOfHeight (0.9142f));
    treeViewAuthor->setBounds ((proportionOfWidth (0.0000f) + juce::roundToInt (proportionOfWidth (0.1127f) * 1.0000f)) + juce::roundToInt (proportionOfWidth (0.0845f) * 1.0000f), proportionOfHeight (0.0858f), proportionOfWidth (0.0845f), proportionOfHeight (0.9142f));
    treeViewTags->setBounds (((proportionOfWidth (0.0000f) + juce::roundToInt (proportionOfWidth (0.1127f) * 1.0000f)) + juce::roundToInt (proportionOfWidth (0.0845f) * 1.0000f)) + juce::roundToInt (proportionOfWidth (0.0845f) * 1.0000f), proportionOfHeight (0.0858f), proportionOfWidth (0.0845f), proportionOfHeight (0.9142f));
    c_treeheader1->setBounds (proportionOfWidth (0.0000f), proportionOfHeight (0.0551f), proportionOfWidth (0.1127f), proportionOfHeight (0.0306f));
    c_treeheader2->setBounds (proportionOfWidth (0.1127f), proportionOfHeight (0.0551f), proportionOfWidth (0.0845f), proportionOfHeight (0.0306f));
    c_treeheader3->setBounds (proportionOfWidth (0.1972f), proportionOfHeight (0.0551f), proportionOfWidth (0.0845f), proportionOfHeight (0.0306f));
    c_treeheader4->setBounds (proportionOfWidth (0.2817f), proportionOfHeight (0.0551f), proportionOfWidth (0.0845f), proportionOfHeight (0.0306f));
    //[UserResized] Add your own custom resize handling here..
	lookAndFeelChanged();
    //[/UserResized]
}

void VASTPresetComponent::buttonClicked (juce::Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == c_clearFilter.get())
    {
        //[UserButtonCode_c_clearFilter] -- add your button handler code here..
		clearFilter();
        //[/UserButtonCode_c_clearFilter]
    }
    else if (buttonThatWasClicked == c_close.get())
    {
        //[UserButtonCode_c_close] -- add your button handler code here..
		((VASTComboPreset*)myBox)->removePopup();
        //[/UserButtonCode_c_close]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void VASTPresetComponent::clearFilter() {
	treeViewFiles->clearSelectedItems();
	treeViewCategory->clearSelectedItems();
	treeViewAuthor->clearSelectedItems();
	treeViewTags->clearSelectedItems();
	m_searchText->setText("", true);
}

void VASTPresetComponent::reloadPresets() {
	myProcessor->m_presetData.reloadPresetArray(false);
	setSearchVector();
}

juce::TreeView* VASTPresetComponent::getTreeViewFiles() { return treeViewFiles.get(); }

void VASTPresetComponent::buildTreeContent() {

	//files
	StringArray saveSelTree;
	for (int i = 0; i < treeViewFiles->getNumSelectedItems(); i++)
		saveSelTree.add(treeViewFiles->getSelectedItem(i)->getUniqueName());
	ValueTree ltreeFiles("Item");
	ltreeFiles.setProperty("name", "Preset Folders & Favorites", nullptr);
	ltreeFiles.setProperty("id", "root", nullptr);
	FileSearchPath sPath = FileSearchPath(myProcessor->m_UserPresetRootFolder);
	ValueTree vpf("Item");
	vpf.setProperty("name", File(myProcessor->m_UserPresetRootFolder).getFileName(), nullptr);
	vpf.setProperty("id", "presetfolder", nullptr);
	ltreeFiles.appendChild(vpf, nullptr);

	Array<File> presetFiles;
	sPath.findChildFiles(presetFiles, File::findDirectories, true);
	for (int i = 0; i < presetFiles.size(); i++) {
		ValueTree vadd("Item");
		vadd.setProperty("name", presetFiles[i].getFileName(), nullptr);
		vadd.setProperty("id", "folder", nullptr);
		vpf.appendChild(vadd, nullptr);

		//l_PresetArray->foldername = String(presetFiles[i].getParentDirectory().getFileName());
		//l_PresetArray->filename = String(presetFiles[i].getFileNameWithoutExtension());
		//l_PresetArray->presetdate = String(presetFiles[i].getLastModificationTime().formatted("%Y-%m-%d"));
		//l_PresetArray->presetarrayindex = i + lNum;
		//l_PresetArray->userpatchindex = i;
		//l_PresetArray->calcID();
	}
	ValueTree vfav1("Item");
	vfav1.setProperty("name", "Favorites 1", nullptr);
	vfav1.setProperty("id", "favorites1", nullptr);
	ltreeFiles.appendChild(vfav1, nullptr);
	ValueTree vfav2("Item");
	vfav2.setProperty("name", "Favorites 2", nullptr);
	vfav2.setProperty("id", "favorites2", nullptr);
	ltreeFiles.appendChild(vfav2, nullptr);
	ValueTree vfav3("Item");
	vfav3.setProperty("name", "Favorites 3", nullptr);
	vfav3.setProperty("id", "favorites3", nullptr);
	ltreeFiles.appendChild(vfav3, nullptr);
	ValueTree vfav4("Item");
	vfav4.setProperty("name", "Favorites 4", nullptr);
	vfav4.setProperty("id", "favorites4", nullptr);
	ltreeFiles.appendChild(vfav4, nullptr);
	ValueTree vfav5("Item");
	vfav5.setProperty("name", "Favorites 5", nullptr);
	vfav5.setProperty("id", "favorites5", nullptr);
	ltreeFiles.appendChild(vfav5, nullptr);
	ValueTree vnewest("Item");
	vnewest.setProperty("name", "Last changed", nullptr);
	vnewest.setProperty("id", "newest", nullptr);
	ltreeFiles.appendChild(vnewest, nullptr);
	ValueTree vstars("Item");
	vstars.setProperty("name", "Stars", nullptr);
	vstars.setProperty("id", "stars", nullptr);
	ltreeFiles.appendChild(vstars, nullptr);
	ValueTree vmidimap("Item");
	vmidimap.setProperty("name", "MIDI Prog. Mapping", nullptr);
	vmidimap.setProperty("id", "midimapping", nullptr);
	ltreeFiles.appendChild(vmidimap, nullptr);
	ValueTree vmidia("Item");
	vmidia.setProperty("name", "Bank A", nullptr);
	vmidia.setProperty("id", "midibanka", nullptr);
	vmidimap.appendChild(vmidia, nullptr);
	ValueTree vaddmidimap("Item");
	StringArray pcdd = myProcessor->m_presetData.getProgramChangeDisplayData(0);
	for (int j = 0; j < pcdd.size(); j++) {
		vaddmidimap.setProperty("name", pcdd[j], nullptr);
		vaddmidimap.setProperty("id", "midimappreseta", nullptr);
		vmidia.appendChild(vaddmidimap.createCopy(), nullptr);
	}
	ValueTree vmidib("Item");
	vmidib.setProperty("name", "Bank B", nullptr);
	vmidib.setProperty("id", "midibankb", nullptr);
	vmidimap.appendChild(vmidib, nullptr);
	pcdd = myProcessor->m_presetData.getProgramChangeDisplayData(1);
	for (int j = 0; j < pcdd.size(); j++) {
		vaddmidimap.setProperty("name", pcdd[j], nullptr);
		vaddmidimap.setProperty("id", "midimappresetb", nullptr);
		vmidib.appendChild(vaddmidimap.createCopy(), nullptr);
	}
	ValueTree vmidic("Item");
	vmidic.setProperty("name", "Bank C", nullptr);
	vmidic.setProperty("id", "midibankc", nullptr);
	vmidimap.appendChild(vmidic, nullptr);
	pcdd = myProcessor->m_presetData.getProgramChangeDisplayData(2);
	for (int j = 0; j < pcdd.size(); j++) {
		vaddmidimap.setProperty("name", pcdd[j], nullptr);
		vaddmidimap.setProperty("id", "midimappresetc", nullptr);
		vmidic.appendChild(vaddmidimap.createCopy(), nullptr);
	}
	ValueTree vmidid("Item");
	vmidid.setProperty("name", "Bank D", nullptr);
	vmidid.setProperty("id", "midibankd", nullptr);
	vmidimap.appendChild(vmidid, nullptr);
	rootItemTreeFiles.resetTree(ltreeFiles);
	pcdd = myProcessor->m_presetData.getProgramChangeDisplayData(3);
	for (int j = 0; j < pcdd.size(); j++) {
		vaddmidimap.setProperty("name", pcdd[j], nullptr);
		vaddmidimap.setProperty("id", "midimappresetd", nullptr);
		vmidid.appendChild(vaddmidimap.createCopy(), nullptr);
	}
	for (int i = 0; i < treeViewFiles->getNumRowsInTree(); i++)
		if (saveSelTree.contains(treeViewFiles->getItemOnRow(i)->getUniqueName()))
			treeViewFiles->getItemOnRow(i)->setSelected(true, false, NotificationType::dontSendNotification);

	//categories
	saveSelTree.clear();
	for (int i = 0; i < treeViewCategory->getNumSelectedItems(); i++)
		saveSelTree.add(treeViewCategory->getSelectedItem(i)->getUniqueName());
	ValueTree ltreeCategories("Item");
	ltreeCategories.setProperty("name", "Categories", nullptr);
	ltreeCategories.setProperty("id", "root", nullptr);
	ValueTree vadd("Item");
	vadd.setProperty("id", "category", nullptr);
	vadd.setProperty("name", "AR Arpeggio", nullptr);
	ltreeCategories.appendChild(vadd.createCopy(), nullptr);
	vadd.setProperty("name", "AT Atmosphere", nullptr);
	ltreeCategories.appendChild(vadd.createCopy(), nullptr);
	vadd.setProperty("name", "BA Bass", nullptr);
	ltreeCategories.appendChild(vadd.createCopy(), nullptr);
	vadd.setProperty("name", "BR Brass", nullptr);
	ltreeCategories.appendChild(vadd.createCopy(), nullptr);
	vadd.setProperty("name", "BL Bell", nullptr);
	ltreeCategories.appendChild(vadd.createCopy(), nullptr);
	vadd.setProperty("name", "CH Chord", nullptr);
	ltreeCategories.appendChild(vadd.createCopy(), nullptr);
	vadd.setProperty("name", "DK Drum kit", nullptr);
	ltreeCategories.appendChild(vadd.createCopy(), nullptr);
	vadd.setProperty("name", "DR Drum", nullptr);
	ltreeCategories.appendChild(vadd.createCopy(), nullptr);
	vadd.setProperty("name", "DL Drum loop", nullptr);
	ltreeCategories.appendChild(vadd.createCopy(), nullptr);
	vadd.setProperty("name", "FX Effect", nullptr);
	ltreeCategories.appendChild(vadd.createCopy(), nullptr);
	vadd.setProperty("name", "GT Guitar", nullptr);
	ltreeCategories.appendChild(vadd.createCopy(), nullptr);
	vadd.setProperty("name", "IN Instrument", nullptr);
	ltreeCategories.appendChild(vadd.createCopy(), nullptr);
	vadd.setProperty("name", "KB Keyboard", nullptr);
	ltreeCategories.appendChild(vadd.createCopy(), nullptr);
	vadd.setProperty("name", "LD Lead", nullptr);
	ltreeCategories.appendChild(vadd.createCopy(), nullptr);
	vadd.setProperty("name", "MA Mallet", nullptr);
	ltreeCategories.appendChild(vadd.createCopy(), nullptr);
	vadd.setProperty("name", "OR Organ", nullptr);
	ltreeCategories.appendChild(vadd.createCopy(), nullptr);
	vadd.setProperty("name", "OC Orchestral", nullptr);
	ltreeCategories.appendChild(vadd.createCopy(), nullptr);
	vadd.setProperty("name", "PD Pad", nullptr);
	ltreeCategories.appendChild(vadd.createCopy(), nullptr);
	vadd.setProperty("name", "PN Piano", nullptr);
	ltreeCategories.appendChild(vadd.createCopy(), nullptr);
	vadd.setProperty("name", "PL Plucked", nullptr);
	ltreeCategories.appendChild(vadd.createCopy(), nullptr);
	vadd.setProperty("name", "RI Riser", nullptr);
	ltreeCategories.appendChild(vadd.createCopy(), nullptr);
	vadd.setProperty("name", "RD Reed", nullptr);
	ltreeCategories.appendChild(vadd.createCopy(), nullptr);
	vadd.setProperty("name", "ST String", nullptr);
	ltreeCategories.appendChild(vadd.createCopy(), nullptr);
	vadd.setProperty("name", "SY Synth", nullptr);
	ltreeCategories.appendChild(vadd.createCopy(), nullptr);
	vadd.setProperty("name", "SQ Sequence / Split", nullptr);
	ltreeCategories.appendChild(vadd.createCopy(), nullptr);
	vadd.setProperty("name", "TG Trancegate", nullptr);
	ltreeCategories.appendChild(vadd.createCopy(), nullptr);
	vadd.setProperty("name", "VC Vocal / Voice", nullptr);
	ltreeCategories.appendChild(vadd.createCopy(), nullptr);
	vadd.setProperty("name", "WW Woodwind", nullptr);
	ltreeCategories.appendChild(vadd.createCopy(), nullptr);
	rootItemTreeCategory.resetTree(ltreeCategories);
	for (int i = 0; i < treeViewCategory->getNumRowsInTree(); i++)
		if (saveSelTree.contains(treeViewCategory->getItemOnRow(i)->getUniqueName()))
			treeViewCategory->getItemOnRow(i)->setSelected(true, false, NotificationType::dontSendNotification);

	//authors
	saveSelTree.clear();
	for (int i = 0; i < treeViewAuthor->getNumSelectedItems(); i++)
		saveSelTree.add(treeViewAuthor->getSelectedItem(i)->getUniqueName());
	ValueTree ltreeAuthors("Item");
	ltreeAuthors.setProperty("name", "Authors", nullptr);
	ltreeAuthors.setProperty("id", "root", nullptr);
	StringArray usedAuthors = *myProcessor->m_presetData.getUsedAuthors();
	usedAuthors.sortNatural();
	for (int j = 0; j < usedAuthors.size(); j++) {
		vadd.setProperty("name", usedAuthors[j], nullptr);
		vadd.setProperty("id", "author", nullptr);
		ltreeAuthors.appendChild(vadd.createCopy(), nullptr);
	}
	rootItemTreeAuthor.resetTree(ltreeAuthors);
	for (int i = 0; i < treeViewAuthor->getNumRowsInTree(); i++)
		if (saveSelTree.contains(treeViewAuthor->getItemOnRow(i)->getUniqueName()))
			treeViewAuthor->getItemOnRow(i)->setSelected(true, false, NotificationType::dontSendNotification);

	//tags
	saveSelTree.clear();
	for (int i = 0; i < treeViewTags->getNumSelectedItems(); i++)
		saveSelTree.add(treeViewTags->getSelectedItem(i)->getUniqueName());
	ValueTree ltreeTags("Item");
	ltreeTags.setProperty("name", "Tags", nullptr);
	ltreeTags.setProperty("id", "root", nullptr);
	StringArray usedTags = *myProcessor->m_presetData.getUsedTags();
	usedTags.sortNatural();
	for (int j = 0; j < usedTags.size(); j++) {
		vadd.setProperty("name", usedTags[j], nullptr);
		vadd.setProperty("id", "tag", nullptr);
		ltreeTags.appendChild(vadd.createCopy(), nullptr);
	}
	rootItemTreeTags.resetTree(ltreeTags);
	for (int i = 0; i < treeViewTags->getNumRowsInTree(); i++)
		if (saveSelTree.contains(treeViewTags->getItemOnRow(i)->getUniqueName()))
			treeViewTags->getItemOnRow(i)->setSelected(true, false, NotificationType::dontSendNotification);
}

void VASTPresetComponent::textEditorTextChanged(TextEditor &textEd) {
	if (textEd.getName().equalsIgnoreCase("m_searchText") == true) {
		//m_preset_table_component->searchText(textEd.getText());
		setSearchVector();
	}
}

bool VASTPresetComponent::keyPressed(const KeyPress& key, Component* originatingComponent) {
	if (key == KeyPress::escapeKey)
		((VASTComboPreset*)myBox)->removePopup();
	return true;
}

TextEditor* VASTPresetComponent::getSearchTextEditor() { return m_searchText.get(); }

void VASTPresetComponent::setSearchVector() {
	myProcessor->m_presetData.mSearchVector.clear();
	for (int j = 0; j < treeViewFiles->getNumSelectedItems(); j++) { //search depths all
		myProcessor->m_presetData.mSearchVector.searchFiles.addIfNotAlreadyThere(treeViewFiles->getSelectedItem(j)->getUniqueName(), true);
	}
	for (int j = 0; j < treeViewCategory->getNumSelectedItems(1); j++) { //search depths 1
		myProcessor->m_presetData.mSearchVector.searchCategories.addIfNotAlreadyThere(treeViewCategory->getSelectedItem(j)->getUniqueName().dropLastCharacters(treeViewCategory->getSelectedItem(j)->getUniqueName().length() - 2), true);
	}
	for (int j = 0; j < treeViewAuthor->getNumSelectedItems(1); j++) { //search depths 1
		myProcessor->m_presetData.mSearchVector.searchAuthors.addIfNotAlreadyThere(treeViewAuthor->getSelectedItem(j)->getUniqueName(), true);
	}
	for (int j = 0; j < treeViewTags->getNumSelectedItems(1); j++) { //search depths 1
		myProcessor->m_presetData.mSearchVector.searchTags.addIfNotAlreadyThere(treeViewTags->getSelectedItem(j)->getUniqueName(), true);
	}
	StringArray freetexttokens;
	freetexttokens.addTokens(m_searchText->getText(), " ,#", "\"");
	for (int j = 0; j < freetexttokens.size(); j++) { //search depths 0
		if (freetexttokens[j].trim() != "")
			myProcessor->m_presetData.mSearchVector.searchFreeText.addIfNotAlreadyThere(freetexttokens[j].trim(), true);
	}

	myProcessor->m_presetData.doSearchWithVector();
	m_preset_table_component->sortOrderChanged(myProcessor->getCurrentVASTLookAndFeel()->presetTableSortColumn, myProcessor->getCurrentVASTLookAndFeel()->presetTableSortColumnForward);
	m_preset_table_component->updateContent(false);
}

void VASTPresetComponent::updateAll() {
	if (myProcessor->m_presetData.needsTreeUpdate()) {
		buildTreeContent();
		myProcessor->m_presetData.clearNeedsTreeUpdate();
	}
	lookAndFeelChanged(); //is in there: m_preset_table_component->updateContent();
	//setSearchVector();
	repaint();

}

void VASTPresetComponent::lookAndFeelChanged() {
	VASTAudioProcessorEditor* myEditor = (VASTAudioProcessorEditor*)myProcessor->getActiveEditor();
	if (myEditor == nullptr) return;
	m_font = myProcessor->getCurrentVASTLookAndFeel()->myDefaultFontBold;
	//float size = float(sqrt(getHeight() * 1.3)) / 25.f * 14.f;
	float size = float(getHeight()) / 260.f * 6.f;
	m_font.setSizeAndStyle(size, m_font.getStyleFlags(), 1.f, 0.f);

	Font searchTextFont = myProcessor->getCurrentVASTLookAndFeel()->getTextEditorFont(*m_searchText);
	juce::Colour vComboBackgroundColor = myProcessor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colBoxTextBackground);
	juce::Colour vComboTextColor = myProcessor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colBoxText);
	m_searchText->setLookAndFeel(myEditor->getCurrentVASTLookAndFeel());
	m_searchText->setColour(TextEditor::textColourId, vComboTextColor);
	m_searchText->setColour(TextEditor::backgroundColourId, vComboBackgroundColor);
	m_searchText->setColour(TextEditor::focusedOutlineColourId, Colours::lightgrey);
	m_searchText->setFont(searchTextFont); //was 11
	m_searchText->applyFontToAllText(searchTextFont);
	m_searchText->applyColourToAllText(vComboTextColor);
	m_searchText->repaint();

	m_preset_table_component->table.setRowHeight(c_treeheader1->getHeight());
	m_fRowHeight = m_preset_table_component->table.getRowHeight();
	m_preset_table_component->table.setHeaderHeight(m_fRowHeight - 1.f);

	//Colour vBackgroundColor = myProcessor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colBoxTextBackground).darker(0.6f).withAlpha(0.85f);
	//Colour vBackgroundColor = myProcessor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colBoxTextBackground);
	Colour vBackgroundColor = Colours::black.withAlpha(0.85f);
	treeViewFiles->setColour(TreeView::backgroundColourId, vBackgroundColor); //backgroundColourId = 0x1000500, linesColourId = 0x1000501, dragAndDropIndicatorColourId = 0x1000502, selectedItemBackgroundColourId = 0x1000503,	oddItemsColourId = 0x1000504, evenItemsColourId = 0x1000505
	treeViewCategory->setColour(TreeView::backgroundColourId, vBackgroundColor); //backgroundColourId = 0x1000500, linesColourId = 0x1000501, dragAndDropIndicatorColourId = 0x1000502, selectedItemBackgroundColourId = 0x1000503,	oddItemsColourId = 0x1000504, evenItemsColourId = 0x1000505
	treeViewAuthor->setColour(TreeView::backgroundColourId, vBackgroundColor); //backgroundColourId = 0x1000500, linesColourId = 0x1000501, dragAndDropIndicatorColourId = 0x1000502, selectedItemBackgroundColourId = 0x1000503,	oddItemsColourId = 0x1000504, evenItemsColourId = 0x1000505
	treeViewTags->setColour(TreeView::backgroundColourId, vBackgroundColor); //backgroundColourId = 0x1000500, linesColourId = 0x1000501, dragAndDropIndicatorColourId = 0x1000502, selectedItemBackgroundColourId = 0x1000503,	oddItemsColourId = 0x1000504, evenItemsColourId = 0x1000505

	for (int i = 0; i < treeViewFiles->getNumRowsInTree(); i++) {
		((VASTValueTreeItem*)treeViewFiles->getItemOnRow(i))->lookAndFeelChanged();
	}
	for (int i = 0; i < treeViewCategory->getNumRowsInTree(); i++) {
		((VASTValueTreeItem*)treeViewCategory->getItemOnRow(i))->lookAndFeelChanged();
	}
	for (int i = 0; i < treeViewAuthor->getNumRowsInTree(); i++) {
		((VASTValueTreeItem*)treeViewAuthor->getItemOnRow(i))->lookAndFeelChanged();
	}
	for (int i = 0; i < treeViewTags->getNumRowsInTree(); i++) {
		((VASTValueTreeItem*)treeViewTags->getItemOnRow(i))->lookAndFeelChanged();
	}
	m_preset_table_component->lookAndFeelChanged();
	m_preset_table_component->updateContent(false);
}

//PresetTableComponent ----------------------------------------------------------------------------------

VASTPresetComponent::PresetTableComponent::PresetTableComponent(VASTAudioProcessor* proc, ComboBox* box, VASTPresetComponent* presetCompopnent) : _processor(proc), _box(box), _presetComponent(presetCompopnent)
{
	// Load some data from an embedded XML file..
	_processor->m_presetData.initSearch();

	lookAndFeelChanged(); //to set colors at one place

	// Create our table component and add it to this component..
	addAndMakeVisible(table);
	table.setModel(this);

	// give it a border
	table.setOutlineThickness(1);

	if (_processor->getActiveEditor() == nullptr)
		return;
	float width = _processor->getActiveEditor()->getWidth() * 0.60f; //from projucer layout

	// Add some columns to the table header, based on the column list in our database..
	table.getHeader().addColumn("ID", 1,		0.10f * width, 5, 5000, TableHeaderComponent::defaultFlags); //invisible
	table.getHeader().addColumn("Category", 2,	0.08f * width, 5, 5000, TableHeaderComponent::defaultFlags);
	table.getHeader().addColumn("Name", 7,		0.32f * width, 5, 5000, TableHeaderComponent::defaultFlags);
	table.getHeader().addColumn("Author", 3,	0.15f * width, 5, 5000, TableHeaderComponent::defaultFlags);
	table.getHeader().addColumn("Tag", 4,		0.15f * width, 5, 5000, TableHeaderComponent::defaultFlags);
	table.getHeader().addColumn("Folder", 5,	0.10f * width, 5, 5000, TableHeaderComponent::defaultFlags);
	table.getHeader().addColumn("Date", 6,		0.10f * width, 5, 5000, TableHeaderComponent::defaultFlags);
	table.getHeader().addColumn("Rating", 8,	0.10f * width, 5, 5000, TableHeaderComponent::defaultFlags);

	int sortColumn = 1;
	bool sortColumnForward = true;
	if (_processor->getCurrentVASTLookAndFeel()->presetTableSortColumn != -1) {
		sortColumn = _processor->getCurrentVASTLookAndFeel()->presetTableSortColumn;
		sortColumnForward = _processor->getCurrentVASTLookAndFeel()->presetTableSortColumnForward;
	}

	table.getHeader().setSortColumnId(sortColumn, sortColumnForward); // sort forwards by the ID column // remember last setting
	sortOrderChanged(sortColumn, sortColumnForward); //dont wait for async update

	table.getHeader().setColumnVisible(1, false); // hide the "ID" column

	// un-comment this line to have a go of stretch-to-fit mode
	table.getHeader().setStretchToFitActive(false);
	table.setMultipleSelectionEnabled(false);

	//select row of current preset
	for (int i = 0; i < _processor->m_presetData.getSearchArray().size(); i++) {
		if (_processor->m_presetData.getSearchArray()[i]->internalid.equalsIgnoreCase(_processor->m_presetData.getCurPatchData().internalid)) { //ID
			juce::SparseSet<int> _set;
			_set.addRange(Range<int>(i, i + 1));
			table.setSelectedRows(_set, juce::NotificationType::dontSendNotification);
			//if (i > 5) {
				//table.scrollToEnsureRowIsOnscreen(i - 5);
				table.scrollToEnsureRowIsOnscreen(i);
			//}
			break;
		}
	}
}

void VASTPresetComponent::PresetTableComponent::lookAndFeelChanged()  {
	vComboBackgroundColor = _processor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colBoxTextBackground).darker(0.6f).withAlpha(0.7f);
	vComboTextColor = _processor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colBoxText);

	//table.setColour(ListBox::backgroundColourId, vComboBackgroundColor);
	//table.setColour(ListBox::backgroundColourId, vComboBackgroundColor);
	Colour vBackgroundColor = Colours::black.withAlpha(0.85f);
	//table.setColour(ListBox::backgroundColourId, _processor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colBoxTextBackground));
	table.setColour(ListBox::backgroundColourId, vBackgroundColor);
	table.setColour(ListBox::outlineColourId, Colours::grey);
	table.setColour(ListBox::textColourId, vComboTextColor);

	if (_processor->getActiveEditor() == nullptr)
		return;
	float width = _processor->getActiveEditor()->getWidth() * 0.60f; //from projucer layout

// Add some columns to the table header, based on the column list in our database..
	table.getHeader().setColumnWidth(1, 0.10f * width); //invisible
	table.getHeader().setColumnWidth(2, 0.08f * width);
	table.getHeader().setColumnWidth(7, 0.32f * width);
	table.getHeader().setColumnWidth(3, 0.15f * width);
	table.getHeader().setColumnWidth(4, 0.15f * width);
	table.getHeader().setColumnWidth(5, 0.10f * width);
	table.getHeader().setColumnWidth(6, 0.10f * width);
	table.getHeader().setColumnWidth(8, 0.10f * width);
}

void VASTPresetComponent::PresetTableComponent::updateContent(bool clearFilterIfNotFound) {
	//select row of current preset
	bool found = false;
	for (int i = 0; i < _processor->m_presetData.getSearchArray().size(); i++) {
		if (_processor->m_presetData.getSearchArray()[i]->internalid.equalsIgnoreCase(_processor->m_presetData.getCurPatchData().internalid)) { //ID
			juce::SparseSet<int> _set;
			_set.addRange(Range<int>(i, i + 1));
			table.setSelectedRows(_set, juce::NotificationType::dontSendNotification);
			//if (i > 5) {
				//table.scrollToEnsureRowIsOnscreen(i - 5);
			table.scrollToEnsureRowIsOnscreen(i);
			//}
			found = true;
			break;
		}
	}
	if (!found) {
		if (clearFilterIfNotFound)
			_presetComponent->clearFilter(); //recursive
		else {
			juce::SparseSet<int> _set;
			table.setSelectedRows(_set, juce::NotificationType::dontSendNotification);
		}
	}

	table.updateContent();
}

Component* VASTPresetComponent::PresetTableComponent::refreshComponentForCell(int rowNumber, int columnId, bool , Component* existingComponentToUpdate) {
	if (columnId != 8) return nullptr; //only for ranking
	VASTPresetStars *star = static_cast<VASTPresetStars*>(existingComponentToUpdate);

	if (rowNumber < _processor->m_presetData.getSearchArray().size())
	{
		String internalid = _processor->m_presetData.getSearchArray()[rowNumber]->internalid;
		int stars = _presetComponent->myProcessor->m_presetData.getStars(internalid);
		if (!star) {
			star = new VASTPresetStars(_presetComponent, internalid, stars);
		}
		else {
			/* Update all properties of your custom component with the data for the current row  */
			star->update(internalid, stars);
		}
	}
	else
	{
		// Nothing to display, free the custom component
		delete existingComponentToUpdate;
		star = nullptr;
	}

	return star;
}

void VASTPresetComponent::PresetTableComponent::selectedRowsChanged(int lastRowSelected)  {
	if (lastRowSelected < 0) return;

	if (_processor->m_presetData.getCurPatchData().internalid.equalsIgnoreCase(_processor->m_presetData.getSearchArray()[lastRowSelected]->internalid))
		return; //same preset

	_processor->setCurrentProgram(_processor->m_presetData.getSearchArray()[lastRowSelected]->presetarrayindex);
	String presetdisplay = _processor->m_presetData.getCurPatchData().category + " " + _processor->m_presetData.getCurPatchData().presetname;
	_box->setText(presetdisplay, juce::NotificationType::dontSendNotification);
}

void VASTPresetComponent::PresetTableComponent::cellClicked(int rowNumber, int , const MouseEvent &e)  {
	ModifierKeys modifiers = e.mods; //ModifierKeys::getCurrentModifiersRealtime();
	if (modifiers.isPopupMenu() || modifiers.isCtrlDown()) {
		int sel = table.getSelectedRow(0);
		String internalid = _processor->m_presetData.getSearchArray()[sel]->internalid;

		PopupMenu mainMenu;
		mainMenu.setLookAndFeel(_processor->getCurrentVASTLookAndFeel());
		mainMenu.addItem(1, "Rename preset", true);
		mainMenu.addSeparator();
		mainMenu.addItem(5, "Add tags", true);
		mainMenu.addSeparator();
		mainMenu.addItem(10, "Add to favorites 1", true);
		mainMenu.addItem(11, "Add to favorites 2", true);
		mainMenu.addItem(12, "Add to favorites 3", true);
		mainMenu.addItem(13, "Add to favorites 4", true);
		mainMenu.addItem(14, "Add to favorites 5", true);
		mainMenu.addItem(15, "Remove from favorites", true);
		mainMenu.showMenuAsync(PopupMenu::Options().withTargetComponent(this).withTargetScreenArea(juce::Rectangle<int>{}.withPosition(Desktop::getMousePosition())),
			juce::ModalCallbackFunction::create([this, internalid](int result) {
			if (result == 0) {
			}
			else if (result == 1) {
				std::unique_ptr<VASTManualRenameEditor> l_veditor = std::make_unique<VASTManualRenameEditor>(_processor, internalid, _presetComponent);
				l_veditor->setLookAndFeel(_processor->getCurrentVASTLookAndFeel());
				l_veditor->setSize(600.f, 60.f);
				l_veditor->setOpaque(true);
				//Point<float> cPoint = myEditor->vaporizerComponent->getLocalPoint(_comp, Point<float>(0.f, 0.f));
				Point<float> cPoint = _presetComponent->getLocalPoint(_presetComponent, Point<float>(0.f, 0.f));
				juce::Rectangle<int> newBounds = juce::Rectangle<int>(cPoint.getX(), cPoint.getY(), _presetComponent->getWidth(), _presetComponent->getHeight());

				l_veditor->setCategory(_processor->m_presetData.getCurPatchData().category);
				l_veditor->setName(_processor->m_presetData.getCurPatchData().presetname);
				juce::CallOutBox::launchAsynchronously(std::move(l_veditor), newBounds, _presetComponent);
			}
			else if (result == 5) {
				std::unique_ptr<VASTManualTagsEditor> l_veditor = std::make_unique<VASTManualTagsEditor>(_processor, internalid, _presetComponent);
				l_veditor->setLookAndFeel(_processor->getCurrentVASTLookAndFeel());
				l_veditor->setSize(600.f, 60.f);
				l_veditor->setOpaque(true);
				//Point<float> cPoint = myEditor->vaporizerComponent->getLocalPoint(_comp, Point<float>(0.f, 0.f));
				Point<float> cPoint = _presetComponent->getLocalPoint(_presetComponent, Point<float>(0.f, 0.f));
				juce::Rectangle<int> newBounds = juce::Rectangle<int>(cPoint.getX(), cPoint.getY(), _presetComponent->getWidth(), _presetComponent->getHeight());

				l_veditor->setTags(_processor->m_presetData.getCurPatchData().freetag);
				juce::CallOutBox::launchAsynchronously(std::move(l_veditor), newBounds, _presetComponent);
			}
			else if (result == 10) {
				_presetComponent->myProcessor->m_presetData.setFavorite(internalid, 0);
				_presetComponent->treeViewFiles->repaint();
			}
			else if (result == 11) {
				_presetComponent->myProcessor->m_presetData.setFavorite(internalid, 1);
				_presetComponent->treeViewFiles->repaint();
			}
			else if (result == 12) {
				_presetComponent->myProcessor->m_presetData.setFavorite(internalid, 2);
				_presetComponent->treeViewFiles->repaint();
			}
			else if (result == 13) {
				_presetComponent->myProcessor->m_presetData.setFavorite(internalid, 3);
				_presetComponent->treeViewFiles->repaint();
			}
			else if (result == 14) {
				_presetComponent->myProcessor->m_presetData.setFavorite(internalid, 4);
				_presetComponent->treeViewFiles->repaint();
			}
			else if (result == 15) {
				_presetComponent->myProcessor->m_presetData.removeFromAllFavorites(internalid);
				_presetComponent->treeViewFiles->repaint();
			}
		}));
	}
	//else {
	//	if (rowNumber == table.getSelectedRow())
	//		selectedRowsChanged(rowNumber);
	//}
}

void VASTPresetComponent::PresetTableComponent::cellDoubleClicked(int , int , const MouseEvent &)  {
	((VASTComboPreset*)_box)->removePopup();
}

var VASTPresetComponent::PresetTableComponent::getDragSourceDescription(const SparseSet<int> &currentlySelectedRows) {
	int sel = currentlySelectedRows.getRange(0).getStart(); //CHECK
	String internalid = _processor->m_presetData.getSearchArray()[sel]->internalid;
	return "table " + internalid;
}

// This is overloaded from TableListBoxModel, and must return the total number of rows in our table
int VASTPresetComponent::PresetTableComponent::getNumRows()
{
	return _processor->m_presetData.getSearchArray().size();
}

// This is overloaded from TableListBoxModel, and should fill in the background of the whole row
void VASTPresetComponent::PresetTableComponent::paintRowBackground(Graphics& g, int /*rowNumber*/, int /*width*/, int /*height*/, bool rowIsSelected)
{
	if (rowIsSelected)
		//g.fillAll(Colours::lightblue);
		g.fillAll(_processor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colPresetComponentRowSelected));
	//else
		//g.fillAll(vComboBackgroundColor);
		//g.fillAll(_processor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colBoxTextBackground).withAlpha(0.5f));
}

// This is overloaded from TableListBoxModel, and must paint any cells that aren't using custom
// components.
void VASTPresetComponent::PresetTableComponent::paintCell(Graphics& g, int rowNumber, int columnId,
	int width, int height, bool rowIsSelected)
{
	if (columnId == 7) { //name
		if (!rowIsSelected)
			g.setColour(vComboTextColor.brighter(0.5f));
		else
			g.setColour(vComboTextColor.darker(0.5f));
	}
	else
		g.setColour(vComboTextColor);
	g.setFont(_presetComponent->m_font);

	String text = getAttributeValue(rowNumber, columnId);
	g.drawText(text, 2, 0, width - 4, height, Justification::centredLeft, true);

	//g.setColour(Colours::black.withMultipliedAlpha(0.5f));
	//g.fillRect(width - 1, 0, 1, height);
}

// This is overloaded from TableListBoxModel, and tells us that the user has clicked a table header
// to change the sort order.
void VASTPresetComponent::PresetTableComponent::sortOrderChanged(int newSortColumnId, bool isForwards)
{
	if ((_processor->getCurrentVASTLookAndFeel()->presetTableSortColumn != newSortColumnId) ||
		(_processor->getCurrentVASTLookAndFeel()->presetTableSortColumnForward != isForwards)) {
		_processor->getCurrentVASTLookAndFeel()->presetTableSortColumn = newSortColumnId;
		_processor->getCurrentVASTLookAndFeel()->presetTableSortColumnForward = isForwards;
		if (_processor->m_initCompleted.load())
			_processor->writeSettingsToFileAsync();
	}

	int presetNameColumn = 7; //CAUTION - do not change
	if (newSortColumnId != 0)
	{
		for (int j = 0; j < _processor->m_presetData.getSearchArray().size(); j++) {
			for (int i = 0; i < _processor->m_presetData.getSearchArray().size(); i++) {
				if (isForwards == true) {
					if (newSortColumnId == presetNameColumn) {
						if (getAttributeValue(i, newSortColumnId) > getAttributeValue(j, newSortColumnId)) {
							_processor->m_presetData.getSearchArray().swap(i, j);
						}
					}
					else
						if ((getAttributeValue(i, newSortColumnId) > getAttributeValue(j, newSortColumnId)) ||
							((getAttributeValue(i, newSortColumnId) == getAttributeValue(j, newSortColumnId)) && (getAttributeValue(i, presetNameColumn) > getAttributeValue(j, presetNameColumn)))
							) {
							_processor->m_presetData.getSearchArray().swap(i, j);
						}
				}
				else {
					if (newSortColumnId == presetNameColumn) {
						if (getAttributeValue(i, newSortColumnId) < getAttributeValue(j, newSortColumnId)) {
							_processor->m_presetData.getSearchArray().swap(i, j);
						}
					}
					else
						if ((getAttributeValue(i, newSortColumnId) < getAttributeValue(j, newSortColumnId)) ||
							((getAttributeValue(i, newSortColumnId) == getAttributeValue(j, newSortColumnId)) && (getAttributeValue(i, presetNameColumn) < getAttributeValue(j, presetNameColumn)))
							) {
							_processor->m_presetData.getSearchArray().swap(i, j);
						}
				}
			}
		}
	}

	//sort INIT PATCH to 0
	int facPos = -1;
	for (int i = 0; i < _processor->m_presetData.getSearchArray().size(); i++) {
		if (_processor->m_presetData.getSearchArray()[i]->isFactory) {
			facPos = i;
			break;
		}
	}
	_processor->m_presetData.getSearchArray().move(facPos, 0);

	//select row of current preset
	for (int i = 0; i < _processor->m_presetData.getSearchArray().size(); i++) {
		if (_processor->m_presetData.getSearchArray()[i]->internalid.equalsIgnoreCase(_processor->m_presetData.getCurPatchData().internalid)) { //ID
			juce::SparseSet<int> _set;
			_set.addRange(Range<int>(i, i + 1));
			table.setSelectedRows(_set, juce::NotificationType::dontSendNotification);
			if (i > 5) {
				table.scrollToEnsureRowIsOnscreen(i - 5);
				table.scrollToEnsureRowIsOnscreen(i);
			}
			break;
		}
	}
}

//==============================================================================
void VASTPresetComponent::PresetTableComponent::resized()
{
	// position our table with a gap around its edge
	//table.setBoundsInset(BorderSize<int>(8));
	table.setBoundsInset(BorderSize<int>(0));

	table.getViewport()->setScrollBarThickness(10 * _presetComponent->myProcessor->getPluginScaleHeightFactor());
}

//==============================================================================
String VASTPresetComponent::PresetTableComponent::getAttributeValue(const int rowNumber, const int columnId) const
{
	//safety
	if (rowNumber >= _processor->m_presetData.getSearchArray().size()) return String(); //String::empty is deprecated
	switch (columnId) {
	case 1: //ID
		return _processor->m_presetData.getSearchArray()[rowNumber]->getPresetIdDisplay();
		break;
	case 2: //Category
		return _processor->m_presetData.getSearchArray()[rowNumber]->getCategoryDisplay();
		break;
	case 3: //Author
		return _processor->m_presetData.getSearchArray()[rowNumber]->getAuthorDisplay();
		break;
	case 4: //Tag
		return _processor->m_presetData.getSearchArray()[rowNumber]->getTagDisplay();
		break;
	case 5: //Folder
		return _processor->m_presetData.getSearchArray()[rowNumber]->getFolderDisplay();
		break;
	case 6: //Date
		return _processor->m_presetData.getSearchArray()[rowNumber]->getDateDisplay();
		break;
	case 7: //Name
		return _processor->m_presetData.getSearchArray()[rowNumber]->getPresetNameDisplay();
		break;
	case 8: //Ranking
		return String(_processor->m_presetData.getSearchArray()[rowNumber]->getRanking());
		/*
		String internalid = _processor->m_presetData.getSearchArray()[rowNumber]->internalid;
		int stars = _presetComponent->myProcessor->m_presetData.getStars(internalid);
		return String(stars);
		*/
		break;
	}

	return String(); //String::empty is deprecated
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="VASTPresetComponent" componentName="VASTPresetComponent"
                 parentClasses="public Component, public TextEditor::Listener, public KeyListener"
                 constructorParams="VASTAudioProcessor* proc, ComboBox* box" variableInitialisers="myProcessor(proc), myBox(box)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="1420" initialHeight="653">
  <BACKGROUND backgroundColour="0">
    <RECT pos="0 0 0M 8.576%" fill="linear: -4% -1.538%, 94.667% 106.154%, 0=e056646c, 1=ff232527"
          hasStroke="0"/>
  </BACKGROUND>
  <GENERICCOMPONENT name="m_preset_table_component" id="cef8cf10869bbd23" memberName="m_preset_table_component"
                    virtualName="" explicitFocusOrder="0" pos="100% 5.513% 63.099% 94.487%"
                    posRelativeX="4d3e4cffa25b738b" class="PresetTableComponent"
                    params="myProcessor, myBox, this"/>
  <TEXTEDITOR name="m_searchText" id="545dd2b92a3f7a53" memberName="m_searchText"
              virtualName="" explicitFocusOrder="0" pos="20.845% 1.225% 41.761% 2.757%"
              tooltip="Enter search text to search all preset attributes" textcol="ffe2e2e2"
              bkgcol="ff091809" hilitecol="40606060" outlinecol="0" caretcol="ffbfbfbf"
              initialText="" multiline="0" retKeyStartsLine="0" readonly="0"
              scrollbars="0" caret="1" popupmenu="0"/>
  <LABEL name="new label" id="9f46727e3ab813de" memberName="label" virtualName=""
         explicitFocusOrder="0" pos="0.282% 1.378% 13.803% 2.45%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="PRESET BROWSER"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Code Pro Demo" fontsize="12.0" kerning="0.0" bold="1"
         italic="0" justification="33" typefaceStyle="Bold"/>
  <TEXTBUTTON name="c_clearFilter" id="92dc60385edf8b76" memberName="c_clearFilter"
              virtualName="" explicitFocusOrder="0" pos="63.732% 1.072% 4.437% 2.757%"
              buttonText="Clear Filter" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <TEXTBUTTON name="c_close" id="f52821dea7ffe3ea" memberName="c_close" virtualName=""
              explicitFocusOrder="0" pos="95.07% 1.072% 4.437% 2.757%" buttonText="Close"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <LABEL name="new label" id="5c90bbb6fbd24994" memberName="label2" virtualName=""
         explicitFocusOrder="0" pos="12.746% 1.685% 7.676% 1.685%" textCol="ffe2e2e2"
         edTextCol="ff000000" edBkgCol="0" labelText="Search Filter" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Code Pro Demo"
         fontsize="12.0" kerning="0.0" bold="0" italic="0" justification="34"/>
  <TREEVIEW name="treeViewFiles" id="6f2e947dc45f19d4" memberName="treeViewFiles"
            virtualName="" explicitFocusOrder="0" pos="0% 8.576% 11.268% 91.424%"
            backgroundColour="a78f959c" linecol="b2777777" rootVisible="1"
            openByDefault="1"/>
  <TREEVIEW name="treeViewCategory" id="8cebc4b32e96b58f" memberName="treeViewCategory"
            virtualName="" explicitFocusOrder="0" pos="100% 8.576% 8.451% 91.424%"
            posRelativeX="6f2e947dc45f19d4" backgroundColour="a78f959c" linecol="b2777777"
            rootVisible="1" openByDefault="1"/>
  <TREEVIEW name="treeViewAuthor" id="1cbf55c99ecd5b5" memberName="treeViewAuthor"
            virtualName="" explicitFocusOrder="0" pos="100% 8.576% 8.451% 91.424%"
            posRelativeX="8cebc4b32e96b58f" backgroundColour="a78f9597" linecol="b2777777"
            rootVisible="1" openByDefault="1"/>
  <TREEVIEW name="treeViewTags" id="4d3e4cffa25b738b" memberName="treeViewTags"
            virtualName="" explicitFocusOrder="0" pos="100% 8.576% 8.451% 91.424%"
            posRelativeX="1cbf55c99ecd5b5" backgroundColour="a78f959c" linecol="b2777777"
            rootVisible="1" openByDefault="1"/>
  <GENERICCOMPONENT name="c_treeheader1" id="cfbf7152b3f57256" memberName="c_treeheader1"
                    virtualName="VASTPresetTreeHeader" explicitFocusOrder="0" pos="0% 5.513% 11.268% 3.063%"
                    class="Component" params="this, &quot;Folder &amp; Favorites&quot; "/>
  <GENERICCOMPONENT name="c_treeheader2" id="8d8aa4ce78fd1aa2" memberName="c_treeheader2"
                    virtualName="VASTPresetTreeHeader" explicitFocusOrder="0" pos="11.268% 5.513% 8.451% 3.063%"
                    class="Component" params="this, &quot;Categories&quot; "/>
  <GENERICCOMPONENT name="c_treeheader3" id="b57f5621cbfe2814" memberName="c_treeheader3"
                    virtualName="VASTPresetTreeHeader" explicitFocusOrder="0" pos="19.718% 5.513% 8.451% 3.063%"
                    class="Component" params="this, &quot;Authors&quot; "/>
  <GENERICCOMPONENT name="c_treeheader4" id="ff209661dbc237f7" memberName="c_treeheader4"
                    virtualName="VASTPresetTreeHeader" explicitFocusOrder="0" pos="28.169% 5.513% 8.451% 3.063%"
                    class="Component" params="this, &quot;Tags&quot; "/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif

//==============================================================================
// Binary resources - be careful not to edit any of these sections!

// JUCER_RESOURCE: folder_fill_svg, 1128, "../../../Resources/Icons/folder_fill.svg"
static const unsigned char resource_VASTPresetComponent_folder_fill_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,70,45,56,
34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,32,119,105,100,116,104,61,34,50,48,112,120,34,32,104,101,105,103,104,116,61,34,50,48,112,120,34,32,118,105,101,119,
66,111,120,61,34,48,32,48,32,50,48,32,50,48,34,32,118,101,114,115,105,111,110,61,34,49,46,49,34,32,120,109,108,110,115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,
47,115,118,103,34,32,120,109,108,110,115,58,120,108,105,110,107,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,120,108,105,110,107,34,62,10,32,32,32,32,60,33,45,
45,32,71,101,110,101,114,97,116,111,114,58,32,83,107,101,116,99,104,32,51,46,56,46,49,32,40,50,57,54,56,55,41,32,45,32,104,116,116,112,58,47,47,119,119,119,46,98,111,104,101,109,105,97,110,99,111,100,
105,110,103,46,99,111,109,47,115,107,101,116,99,104,32,45,45,62,10,32,32,32,32,60,116,105,116,108,101,62,102,111,108,100,101,114,95,102,105,108,108,32,91,35,49,55,55,55,93,60,47,116,105,116,108,101,62,
10,32,32,32,32,60,100,101,115,99,62,67,114,101,97,116,101,100,32,119,105,116,104,32,83,107,101,116,99,104,46,60,47,100,101,115,99,62,10,32,32,32,32,60,100,101,102,115,62,60,47,100,101,102,115,62,10,32,
32,32,32,60,103,32,105,100,61,34,80,97,103,101,45,49,34,32,115,116,114,111,107,101,61,34,110,111,110,101,34,32,115,116,114,111,107,101,45,119,105,100,116,104,61,34,49,34,32,102,105,108,108,61,34,110,111,
110,101,34,32,102,105,108,108,45,114,117,108,101,61,34,101,118,101,110,111,100,100,34,62,10,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,68,114,105,98,98,98,108,101,45,68,97,114,107,45,80,114,101,118,
105,101,119,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,45,50,50,48,46,48,48,48,48,48,48,44,32,45,49,49,57,57,46,48,48,48,48,48,48,41,34,32,102,105,108,108,61,34,
35,70,70,70,70,70,70,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,103,32,105,100,61,34,105,99,111,110,115,34,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,53,54,46,
48,48,48,48,48,48,44,32,49,54,48,46,48,48,48,48,48,48,41,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,60,112,97,116,104,32,100,61,34,77,49,56,52,44,49,48,52,53,46,48,48,48,53,32,76,49,56,52,
44,49,48,53,55,46,48,48,48,53,32,67,49,56,52,44,49,48,53,56,46,49,48,53,53,32,49,56,51,46,49,48,53,44,49,48,53,57,46,48,48,48,53,32,49,56,50,44,49,48,53,57,46,48,48,48,53,32,76,49,54,54,44,49,48,53,57,
46,48,48,48,53,32,67,49,54,52,46,56,57,53,44,49,48,53,57,46,48,48,48,53,32,49,54,52,44,49,48,53,56,46,49,48,53,53,32,49,54,52,44,49,48,53,55,46,48,48,48,53,32,76,49,54,52,44,49,48,52,49,46,48,48,48,53,
32,67,49,54,52,44,49,48,51,57,46,56,57,53,53,32,49,54,52,46,56,57,53,44,49,48,51,57,46,48,48,48,53,32,49,54,54,44,49,48,51,57,46,48,48,48,53,32,76,49,55,50,44,49,48,51,57,46,48,48,48,53,32,67,49,55,51,
46,49,48,53,44,49,48,51,57,46,48,48,48,53,32,49,55,52,44,49,48,51,57,46,56,57,53,53,32,49,55,52,44,49,48,52,49,46,48,48,48,53,32,76,49,55,52,44,49,48,52,50,46,48,48,48,53,32,67,49,55,52,44,49,48,52,50,
46,53,53,50,53,32,49,55,52,46,52,52,56,44,49,48,52,51,46,48,48,48,53,32,49,55,53,44,49,48,52,51,46,48,48,48,53,32,76,49,56,50,44,49,48,52,51,46,48,48,48,53,32,67,49,56,51,46,49,48,53,44,49,48,52,51,46,
48,48,48,53,32,49,56,52,44,49,48,52,51,46,56,57,53,53,32,49,56,52,44,49,48,52,53,46,48,48,48,53,34,32,105,100,61,34,102,111,108,100,101,114,95,102,105,108,108,45,91,35,49,55,55,55,93,34,62,60,47,112,97,
116,104,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,32,32,32,32,60,47,103,62,10,32,32,32,32,60,47,103,62,10,60,47,115,118,103,62,0,0};

const char* VASTPresetComponent::folder_fill_svg = (const char*) resource_VASTPresetComponent_folder_fill_svg;
const int VASTPresetComponent::folder_fill_svgSize = 1128;


//[EndFile] You can add extra defines here...
//[/EndFile]

