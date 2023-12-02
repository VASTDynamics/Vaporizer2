/*
  ==============================================================================

    VASTPresetData.h

  ==============================================================================
*/

#pragma once

#include "../../Engine/VASTEngineHeader.h"
#include "VASTPresetElement.h"
#include <map>
#include <unordered_map>

class VASTAudioProcessor; //forward declaration
class VASTPresetData
{
public:
	struct sSearchVector {
		StringArray searchFiles;
		StringArray searchCategories;
		StringArray searchAuthors;
		StringArray searchTags;
		StringArray searchFreeText;
		void clear() {
			searchFiles.clear();
			searchCategories.clear();
			searchAuthors.clear();
			searchTags.clear();
			searchFreeText.clear();
		};
	} mSearchVector;

	VASTPresetData(VASTAudioProcessor* proc) : myProcessor(proc)	{
		m_numFavorites[0] = 0;
		m_numFavorites[1] = 0;
		m_numFavorites[2] = 0;
		m_numFavorites[3] = 0;
		m_numFavorites[4] = 0;
	};

	/*
	~VASTPresetData() {
	}
	*/

	void reloadPresetArray();
	int getNumPresets() const;
	VASTPresetElement* getPreset(int index);
	OwnedArray<VASTPresetElement>& getSearchArray();
	void initSearch();
	void doSearchWithVector();

	int getIndexInSearchArray(String internalid) const;
	int getIndexInPresetArray(String internalid) const;

	StringArray* getUsedCategories();
	StringArray* getUsedAuthors();
	StringArray* getUsedTags();

	const VASTPresetElement& getCurPatchData();
	void exchangeCurPatchData(VASTPresetElement& newPatchData);
	
	bool needsTreeUpdate() const;
	void clearNeedsTreeUpdate();

	int bankProgramGetPresetIndex(int bank, int program);
	void setProgramChangeArray(int bank, StringArray progChangeArray);
	void setProgramChangeData(String internalid, int bank, int position);
	void swapProgramChangeData(int bankFrom, int bankTo, int position1, int position2);
	void moveProgramChangeData(int bankFrom, int bankTo, int position1, int position2);
	void removeProgramChangeData(int bank, int position);
	StringArray getProgramChangeData(int bank) const;
	StringArray getProgramChangeDisplayData(int bank) const;
	void setFavorite(String internalid, int favoriteNo);
	bool getFavorite(String internalid, int favoriteNo) const;
	int getNumFavorites(int favoriteNo);
	void removeAllFavorites(int favoriteNo);
	void removeFromAllFavorites(String internalid);
	void setStars(String internalid, int stars);
	int getStars(String internalid);

	void writeToSettings();
	void loadFromSettings();
	void calcNumFavorites();

	void addTag(String tagToAdd);

	std::map<String,int> m_stars; //fast: hashed 
	std::unordered_multimap<String,int> m_favorites; //fast: hashed 
	StringArray m_programChangeData[4]; //banka-d

private:
	VASTAudioProcessor* myProcessor = nullptr;
	OwnedArray<VASTPresetElement> m_PresetArray;
	OwnedArray<VASTPresetElement> m_SearchArray;

	void loadSearchData();

	VASTPresetElement m_curPatchData;
	
	StringArray m_usedAuthors;
	StringArray m_usedCategories;
	StringArray m_usedTags;

	int m_numFavorites[5];

	int m_numUserPresets = 0;

	bool m_needsTreeUpdate = false;

	//ElementComparator
	class VASTPresetElementCompareDates {
	public:
		int compareElements(VASTPresetElement* first, const VASTPresetElement* second);
	};
};
