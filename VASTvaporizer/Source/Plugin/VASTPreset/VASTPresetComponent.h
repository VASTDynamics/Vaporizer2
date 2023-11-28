/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 6.0.1

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "../../Engine/VASTEngineHeader.h"
#include "../VASTAudioProcessor.h"
#include "VASTPresetElement.h"
#include "VASTPresetTreeHeader.h"

//  ==============================================================================
class VASTPresetComponent; //forward declaration
class VASTValueTreeItem : public TreeViewItem, private ValueTree::Listener
{
public:
	juce::Colour vComboBackgroundColor = Colours::darkgreen.darker(1.0f).darker(1.0f).darker(0.8f);
	juce::Colour vComboTextColor = Colours::lightgreen.darker(0.7f);
	VASTAudioProcessor* _processor;
	VASTPresetComponent* _presetComponent;

	VASTValueTreeItem(const ValueTree& v, VASTAudioProcessor* proc, VASTPresetComponent* presetCompopnent);
	void resetTree(const ValueTree& v);

	String getUniqueName() const override;
	String getID() const;
	bool mightContainSubItems() override;

	void paintItem(Graphics& g, int width, int height) override;
	int getItemHeight() const override;
	bool canBeSelected() const override;
	void lookAndFeelChanged();
	void itemClicked(const MouseEvent &) override;
	void itemDoubleClicked(const MouseEvent &) override;
	void itemOpennessChanged(bool isNowOpen) override;
	void itemSelectionChanged(bool isNowSelected) override;
	var getDragSourceDescription() override;
	bool isInterestedInDragSource(const DragAndDropTarget::SourceDetails& dragSourceDetails) override;
	void itemDropped(const DragAndDropTarget::SourceDetails&, int insertIndex) override;
	static void moveItems(TreeView& treeView, const OwnedArray<ValueTree>& items, ValueTree newParent, int insertIndex);
	static void getSelectedTreeViewItems(TreeView& treeView, OwnedArray<ValueTree>& items);
    
private:
	ValueTree tree;
	std::unique_ptr<FileChooser> myChooser;
	void refreshSubItems();
	void valueTreePropertyChanged(ValueTree&, const Identifier&) override;
	void valueTreeChildAdded(ValueTree& parentTree, ValueTree&) override;
	void valueTreeChildRemoved(ValueTree& parentTree, ValueTree&, int) override;
	void valueTreeChildOrderChanged(ValueTree& parentTree, int, int) override;
	void valueTreeParentChanged(ValueTree&) override {}

	void treeChildrenChanged(const ValueTree& parentTree);
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VASTValueTreeItem)
};
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class VASTPresetComponent  : public Component,
                             public TextEditor::Listener,
                             public KeyListener,
                             public juce::Button::Listener
{
public:
    //==============================================================================
    VASTPresetComponent (VASTAudioProcessor* proc, ComboBox* box);
    ~VASTPresetComponent() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	VASTAudioProcessor* myProcessor;
	ComboBox* myBox;
	float m_fRowHeight = 50.f;
	Font m_font;

	void textEditorTextChanged(TextEditor &) override;
	bool keyPressed(const KeyPress& key, Component* originatingComponent) override;

	TextEditor* getSearchTextEditor();

	void setSearchVector();
	void clearFilter();
	void lookAndFeelChanged() override;
	void updateAll();
	void buildTreeContent();
	void reloadPresets();
	std::unique_ptr<Drawable> mFolderDrawable = nullptr;

	juce::TreeView* getTreeViewFiles();
    std::unique_ptr<Drawable> m_folderImg;
    
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;

    // Binary resources:
    static const char* folder_fill_svg;
    static const int folder_fill_svgSize;


private:
    //[UserVariables]   -- You can add your own custom variables in this section.

	VASTValueTreeItem rootItemTreeFiles { ValueTree(), myProcessor, this };
	VASTValueTreeItem rootItemTreeCategory{ ValueTree(), myProcessor, this };
	VASTValueTreeItem rootItemTreeAuthor{ ValueTree(), myProcessor, this };
	VASTValueTreeItem rootItemTreeTags{ ValueTree(), myProcessor, this };


	/*
     std::unique_ptr<VASTValueTreeItem> rootItemTreeFiles;
     std::unique_ptr<VASTValueTreeItem> rootItemTreeCategory;
     std::unique_ptr<VASTValueTreeItem> rootItemTreeAuthor;
     std::unique_ptr<VASTValueTreeItem> rootItemTreeTags;
	*/

	class PresetTableComponent : public Component,
		public juce::TableListBoxModel
	{
	public:
		juce::TableListBox table;     // the table component itself
		juce::Colour vComboBackgroundColor = Colours::darkgreen.darker(1.0f).darker(1.0f).darker(0.8f);
		juce::Colour vComboTextColor = Colours::lightgreen.darker(0.7f);
		VASTAudioProcessor* _processor;
		ComboBox* _box;
		VASTPresetComponent* _presetComponent;

		PresetTableComponent(VASTAudioProcessor* proc, ComboBox* box, VASTPresetComponent* presetCompopnent);

		void lookAndFeelChanged() override;

		void updateContent(bool clearFilterIfNotFound);
		Component* refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component* existingComponentToUpdate) override;

		void selectedRowsChanged(int lastRowSelected) override;
		void cellClicked(int rowNumber, int columnId, const MouseEvent &) override;

		var getDragSourceDescription(const SparseSet<int> &currentlySelectedRows) override;

		void cellDoubleClicked(int rowNumber, int columnId, const MouseEvent &) override;

		// This is overloaded from TableListBoxModel, and must return the total number of rows in our table
		int getNumRows() override;
		// This is overloaded from TableListBoxModel, and should fill in the background of the whole row
		void paintRowBackground(Graphics& g, int /*rowNumber*/, int /*width*/, int /*height*/, bool rowIsSelected) override;

		// This is overloaded from TableListBoxModel, and must paint any cells that aren't using custom
		// components.
		void paintCell(Graphics& g, int rowNumber, int columnId,
			int width, int height, bool /*rowIsSelected*/) override;

		// This is overloaded from TableListBoxModel, and tells us that the user has clicked a table header
		// to change the sort order.
		void sortOrderChanged(int newSortColumnId, bool isForwards) override;

		//==============================================================================
		void resized() override;

		//==============================================================================

	private:
		String getAttributeValue(const int rowNumber, const int columnId) const;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PresetTableComponent)
	};

    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<PresetTableComponent> m_preset_table_component;
    std::unique_ptr<juce::TextEditor> m_searchText;
    std::unique_ptr<juce::Label> label;
    std::unique_ptr<juce::TextButton> c_clearFilter;
    std::unique_ptr<juce::TextButton> c_close;
    std::unique_ptr<juce::Label> label2;
    std::unique_ptr<juce::TreeView> treeViewFiles;
    std::unique_ptr<juce::TreeView> treeViewCategory;
    std::unique_ptr<juce::TreeView> treeViewAuthor;
    std::unique_ptr<juce::TreeView> treeViewTags;
    std::unique_ptr<VASTPresetTreeHeader> c_treeheader1;
    std::unique_ptr<VASTPresetTreeHeader> c_treeheader2;
    std::unique_ptr<VASTPresetTreeHeader> c_treeheader3;
    std::unique_ptr<VASTPresetTreeHeader> c_treeheader4;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VASTPresetComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

