/*
VAST Dynamics Audio Software (TM)
*/

#include "../../Engine/VASTEngineHeader.h"
#include "VASTPresetData.h"
#include "../VASTAudioProcessor.h"
#include <thread>
#include <fstream>
#include <iostream>

//Prefix Category AR Arpeggio AT Atmosphere BA Bass BR Brass BL Bell CH Chord DK Drum kit DR Drum DL Drum loop FX Effect GT Guitar IN Instrument KB Keyboard LD Lead MA Mallet OR Organ OC Orchestral PN Piano PL Plucked RD Reed ST String SY Synth SQ Sequence / Split TG Trancegate VC Vocal / Voice WW Woodwind

bool VASTPresetData::isLoadThreadRunning; //static

VASTPresetData::VASTPresetData(VASTAudioProcessor* proc) : myProcessor(proc) {
	m_numFavorites[0] = 0;
	m_numFavorites[1] = 0;
	m_numFavorites[2] = 0;
	m_numFavorites[3] = 0;
	m_numFavorites[4] = 0;

	swap_PresetArray.clear();
	swap_usedAuthors.clear();
	swap_usedCategories.clear();
	swap_usedTags.clear();
	VASTPresetData::isLoadThreadRunning = false;
	m_presetsloaded.store(false);
}

VASTPresetData::~VASTPresetData()
{
	if (m_reloadPresetThread.joinable())
		m_reloadPresetThread.join();
	VASTPresetData::isLoadThreadRunning = false;
}

int VASTPresetData::getNumPresets() const {
	return m_numUserPresets;
}

VASTPresetElement* VASTPresetData::getPreset(int index) {
	if (index < m_PresetArray.size())
		return m_PresetArray[index];
	else
		return nullptr;
}

OwnedArray<VASTPresetElement>& VASTPresetData::getSearchArray() {
	return m_SearchArray;
}

StringArray* VASTPresetData::getUsedAuthors() {
	return &m_usedAuthors;
}

StringArray* VASTPresetData::getUsedCategories() {
	return &m_usedCategories;
}

StringArray* VASTPresetData::getUsedTags() {
	return &m_usedTags;
}

void VASTPresetData::writeToSettings() {
	if (myProcessor->m_initCompleted.load())
		myProcessor->writeSettingsToFileAsync();
}

void VASTPresetData::loadFromSettings() {
	myProcessor->readSettingsFromFile();
	calcNumFavorites();	
}

void VASTPresetData::reloadPresetArrayThreaded(Component::SafePointer<VASTPresetData> presetData, VASTAudioProcessor* myProcessor) {
	VDBG("Start reloadPresetArrayThreaded");
	if (presetData == nullptr)
		return;
	if (presetData->m_swapNeeded.load())
		return; //only one process
	OwnedArray<VASTPresetElement> l_thread_PresetArray;
	if (presetData == nullptr)
		return;
	else 
		l_thread_PresetArray.addCopiesOf(presetData->m_PresetArray);

	StringArray l_thread_usedAuthors;
	StringArray l_thread_usedCategories;
	StringArray l_thread_usedTags;

	l_thread_usedAuthors.clear();
	l_thread_usedCategories.clear();
	l_thread_usedTags.clear();

	for (int i = 1; i < l_thread_PresetArray.size(); i++) { //0 is factory init preset
		VASTPresetElement* l_PresetArray = new VASTPresetElement(); //new OK because of owned Array
		const String filename = l_thread_PresetArray[i]->internalid;
		bool successfullyLoaded = myProcessor->loadUserPatchMetaData(File(filename), *l_PresetArray);

		if (!successfullyLoaded) {
			l_thread_PresetArray[i]->invalid = true;
			VDBG("PresetArray invalid: " << l_thread_PresetArray[i]->internalid);
			l_thread_PresetArray[i]->authorname = "Invalid preset XML data!";
			delete l_PresetArray;
			continue;
		}
		l_thread_PresetArray[i]->invalid = false;
		l_thread_PresetArray[i]->presetname = l_PresetArray->presetname;
		l_thread_PresetArray[i]->category = l_PresetArray->category;
		l_thread_PresetArray[i]->freetag = l_PresetArray->freetag;
		l_thread_PresetArray[i]->mpepreset = l_PresetArray->mpepreset;
		l_thread_PresetArray[i]->mpebendrange = l_PresetArray->mpebendrange;
		l_thread_PresetArray[i]->authorname = l_PresetArray->authorname;
		l_thread_PresetArray[i]->comments = l_PresetArray->comments;
		l_thread_PresetArray[i]->customModulator1Text = l_PresetArray->customModulator1Text;
		l_thread_PresetArray[i]->customModulator2Text = l_PresetArray->customModulator2Text;
		l_thread_PresetArray[i]->customModulator3Text = l_PresetArray->customModulator3Text;
		l_thread_PresetArray[i]->customModulator4Text = l_PresetArray->customModulator4Text;

		if (l_PresetArray->category.trim() != "")
			l_thread_usedCategories.addIfNotAlreadyThere(l_PresetArray->category.trim(), true);
		if (l_PresetArray->authorname.trim() != "")
			l_thread_usedAuthors.addIfNotAlreadyThere(l_PresetArray->authorname.trim(), true);

		StringArray tags;
		tags.addTokens(l_PresetArray->freetag, " ,#", "\"");
		for (int j = 0; j < tags.size(); j++)
			if (tags[j].trim() != "")
				l_thread_usedTags.addIfNotAlreadyThere(tags[j].trim(), true);
		VDBG("Added to PresetArray: " << l_thread_PresetArray[i]->internalid);
		delete l_PresetArray;
	}
	VDBG("End reloadPresetArrayThreaded");
	
	if (presetData == nullptr)
		return;

	if (VASTPresetData::isLoadThreadRunning) {
		const ScopedLock sl(presetData->m_arraySwapLock);
		presetData->swap_PresetArray.swapWith(l_thread_PresetArray);
		l_thread_usedAuthors.sortNatural();
		presetData->swap_usedAuthors.swapWith(l_thread_usedAuthors);
		l_thread_usedCategories.sortNatural();
		presetData->swap_usedCategories.swapWith(l_thread_usedCategories);
		l_thread_usedTags.sortNatural();
		presetData->swap_usedTags.swapWith(l_thread_usedTags);

		l_thread_PresetArray.clear();
		l_thread_usedAuthors.clear();
		l_thread_usedCategories.clear();
		l_thread_usedTags.clear();		

		presetData->m_swapNeeded.store(true);
	//} else {
		//vassertfalse; //it was destroyed in the meantime
	}
}

void VASTPresetData::swapPresetArraysIfNeeded() {
	if (!m_swapNeeded.load())
		return;
	m_swapNeeded.store(false);

	const ScopedLock sl(m_arraySwapLock);
	m_PresetArray.swapWith(swap_PresetArray);
	m_usedAuthors.swapWith(swap_usedAuthors);
	m_usedCategories.swapWith(swap_usedCategories);
	m_usedTags.swapWith(swap_usedTags);

	swap_PresetArray.clear();
	swap_usedAuthors.clear();
	swap_usedCategories.clear();
	swap_usedTags.clear();
	
	loadFromSettings();
	m_numUserPresets = m_PresetArray.size() - 1; //factory is 1
	doSearchWithVector();
	myProcessor->requestUIPresetUpdate();
	m_needsTreeUpdate = true;
	m_presetsloaded.store(true);

	VASTPresetData::isLoadThreadRunning = false;
}

void VASTPresetData::reloadPresetArray(bool synchronous) {
	VDBG("Start reloadPresetArray");
	if (VASTPresetData::isLoadThreadRunning)
		return;
	VASTPresetData::isLoadThreadRunning = true;

	m_PresetArray.clear();
	m_usedAuthors.clear();
	m_usedTags.clear();
	m_usedCategories.clear();
	m_presetsloaded.store(false);

	//Factory
	int lNum = 1; //starts with 1 due to init preset - which is VST factory
	for (int i = 0; i < lNum; i++) {
		VASTPresetElement* l_PresetArray = new VASTPresetElement(); //new OK because of owned Array
		l_PresetArray->authorname = String("VASTDynamics");
		l_PresetArray->category = String("SY");
		l_PresetArray->presetname = String("Init Patch");
		l_PresetArray->freetag = String("Init Patch");
		l_PresetArray->comments = String("This is the initial preset that has default values for all parameters and a basic saw wavetable in OSC A.");

		l_PresetArray->internalid = String("Init");
		l_PresetArray->isFactory = true;
		l_PresetArray->mpepreset = false;
		l_PresetArray->foldername = String("");
		l_PresetArray->presetarrayindex = i;
		l_PresetArray->calcID();
		m_PresetArray.add(l_PresetArray);
	}
	int l_numUserPresets = 0; //factory is 1
	
	Array<File> l_presetFiles;
	FileSearchPath sPath = FileSearchPath(myProcessor->m_UserPresetRootFolder);
	sPath.addIfNotAlreadyThere(File(myProcessor->getVSTPath()).getChildFile("Presets").getFullPathName()); //add search path for factory presets in app folder (not configurable) //not needed with symlink solution	
	sPath.findChildFiles(l_presetFiles, File::findFiles, true, "*.vvp");
	
	for (int i = 0; i < l_presetFiles.size(); i++) {
		VASTPresetElement* l_PresetArray = new VASTPresetElement(); //new OK because of owned Array
			l_numUserPresets++;			
			l_PresetArray->internalid = String(l_presetFiles[i].getFullPathName()); //this is unique
			l_PresetArray->foldername = String(l_presetFiles[i].getParentDirectory().getFileName());
			l_PresetArray->filename = String(l_presetFiles[i].getFileNameWithoutExtension());
			l_PresetArray->presetdate = String(l_presetFiles[i].getLastModificationTime().formatted("%Y-%m-%d"));
			l_PresetArray->presetarrayindex = i + 1; //init preset
			l_PresetArray->userpatchindex = i;
			l_PresetArray->isFactory = false;
			l_PresetArray->presetname = l_PresetArray->filename.substring(3); //temporarily until loaded
			l_PresetArray->category = l_PresetArray->filename.dropLastCharacters(l_PresetArray->filename.length() - 3); //temporarily until loaded
			l_PresetArray->calcID();
			m_PresetArray.add(l_PresetArray);
		}
	m_numUserPresets = l_numUserPresets;

	if (synchronous) {
		reloadPresetArrayThreaded(SafePointer<VASTPresetData>(this), myProcessor);
		swapPresetArraysIfNeeded();
	}
	else {
		m_reloadPresetThread = std::thread(&VASTPresetData::reloadPresetArrayThreaded, SafePointer<VASTPresetData>(this), myProcessor);
	}
}

void VASTPresetData::doSearchWithVector() {
	initSearch();
	int i = 0;
	//Files
	if (mSearchVector.searchFiles.size() > 0) {

		//stars
		StringArray searchFileCopy = mSearchVector.searchFiles;
		if (searchFileCopy.contains("Stars", true)) {
			i = 0;
			while (i < m_SearchArray.size()) {
				bool keep = false;
				if (getStars(m_SearchArray[i]->internalid) > 0)
					keep = true;
				if (!keep)
					m_SearchArray.remove(i, true);
				else
					i++;
			}
			searchFileCopy.removeString("Stars", true);
		}

		//favorites 
		if (searchFileCopy.contains("Favorites 1", true)) {
			i = 0;
			while (i < m_SearchArray.size()) {
				bool keep = false;
				if (getFavorite(m_SearchArray[i]->internalid, 0))
					keep = true;
				if (!keep)
					m_SearchArray.remove(i, true);
				else
					i++;
			}
			searchFileCopy.removeString("Favorites 1", true);
		}
		if (searchFileCopy.contains("Favorites 2", true)) {
			i = 0;
			while (i < m_SearchArray.size()) {
				bool keep = false;
				if (getFavorite(m_SearchArray[i]->internalid, 1))
					keep = true;
				if (!keep)
					m_SearchArray.remove(i, true);
				else
					i++;
			}
			searchFileCopy.removeString("Favorites 2", true);
		}
		if (searchFileCopy.contains("Favorites 3", true)) {
			i = 0;
			while (i < m_SearchArray.size()) {
				bool keep = false;
				if (getFavorite(m_SearchArray[i]->internalid, 2))
					keep = true;
				if (!keep)
					m_SearchArray.remove(i, true);
				else
					i++;
			}
			searchFileCopy.removeString("Favorites 3", true);
		}
		if (searchFileCopy.contains("Favorites 4", true)) {
			i = 0;
			while (i < m_SearchArray.size()) {
				bool keep = false;
				if (getFavorite(m_SearchArray[i]->internalid, 3))
					keep = true;
				if (!keep)
					m_SearchArray.remove(i, true);
				else
					i++;
			}
			searchFileCopy.removeString("Favorites 4", true);
		}
		if (searchFileCopy.contains("Favorites 5", true)) {
			i = 0;
			while (i < m_SearchArray.size()) {
				bool keep = false;
				if (getFavorite(m_SearchArray[i]->internalid, 4))
					keep = true;
				if (!keep)
					m_SearchArray.remove(i, true);
				else
					i++;
			}
			searchFileCopy.removeString("Favorites 5", true);
		}
		//last changed
		if (searchFileCopy.contains("Last changed", true)) {
			VASTPresetElementCompareDates comp;
			m_SearchArray.sort(comp, true);			
			while (m_SearchArray.size() > 20) { //keep 20
				m_SearchArray.remove(i, true);
			}
			searchFileCopy.removeString("Last changed", true);
		}

		//folders
		if (searchFileCopy.size() > 0) {
			i = 0;
			while (i < m_SearchArray.size()) {
				bool keep = false;
				for (int j = 0; j < mSearchVector.searchFiles.size(); j++) {
					if (m_SearchArray[i]->foldername.isNotEmpty() && (m_SearchArray[i]->foldername.containsIgnoreCase(mSearchVector.searchFiles[j]) == true)) {
						keep = true;
						break;
					}
				}
				if (!keep)
					m_SearchArray.remove(i, true);
				else
					i++;
			}
		}
	}
	//Categories
	if (mSearchVector.searchCategories.size() > 0) {
		i = 0;
		while (i < m_SearchArray.size()) {
			bool keep = false;
			for (int j = 0; j < mSearchVector.searchCategories.size(); j++) {
				if (m_SearchArray[i]->category.isNotEmpty() && (m_SearchArray[i]->category.containsIgnoreCase(mSearchVector.searchCategories[j]) == true)) {
					keep = true;
					break;
				}
			}
			if (!keep)
				m_SearchArray.remove(i, true);
			else
				i++;
		}
	}
	//Authors
	if (mSearchVector.searchAuthors.size() > 0) {
		i = 0;
		while (i < m_SearchArray.size()) {
			bool keep = false;
			for (int j = 0; j < mSearchVector.searchAuthors.size(); j++) {
				if (m_SearchArray[i]->authorname.isNotEmpty() && (m_SearchArray[i]->authorname.equalsIgnoreCase(mSearchVector.searchAuthors[j]) == true)) {
					keep = true;
					break;
				}
			}
			if (!keep)
				m_SearchArray.remove(i, true);
			else
				i++;
		}
	}
	//Tags
	if (mSearchVector.searchTags.size() > 0) {
		i = 0;
		while (i < m_SearchArray.size()) {
			bool keep = false;
			for (int j = 0; j < mSearchVector.searchTags.size(); j++) {
				if (m_SearchArray[i]->freetag.isNotEmpty() && (m_SearchArray[i]->freetag.containsIgnoreCase(mSearchVector.searchTags[j]) == true)) {
					keep = true;
					break;
				}
			}
			if (!keep)
				m_SearchArray.remove(i, true);
			else
				i++;
		}
	}
	if (mSearchVector.searchFreeText.size() > 0) {
		i = 0;
		while (i < m_SearchArray.size()) {
			bool keep = true; //do AND
			for (int j = 0; j < mSearchVector.searchFreeText.size(); j++) { 
				if (!m_SearchArray[i]->containsString(mSearchVector.searchFreeText[j])) {
					keep = false;
					break;
				}
			}
			if (!keep)
				m_SearchArray.remove(i, true);
			else
				i++;
		}
	}
}


void VASTPresetData::initSearch() {
	loadSearchData();
}

void VASTPresetData::loadSearchData() {
	m_SearchArray.clear();
	for (int i = 0; i < getNumPresets(); i++) {
		VASTPresetElement* lElement = new VASTPresetElement(); //new OK because of owned array
		*lElement = *getPreset(i); //copy it
		if (lElement != nullptr)
			m_SearchArray.add(lElement);
	}
}

int VASTPresetData::getIndexInSearchArray(String internalid) const {
	for (int i = 0; i < m_SearchArray.size(); i++) {
		if (m_SearchArray[i]->internalid.equalsIgnoreCase(internalid))
			return i;
	}
	return -1;
}

int VASTPresetData::getIndexInPresetArray(String internalid) const {
	for (int i = 0; i < m_PresetArray.size(); i++) {
		if (m_PresetArray[i]->internalid.equalsIgnoreCase(internalid))
			return i;
	}
	return -1;
}

const VASTPresetElement& VASTPresetData::getCurPatchData() {
	return m_curPatchData;
}

void VASTPresetData::exchangeCurPatchData(VASTPresetElement& newPatchData) {
	m_curPatchData = newPatchData;
	myProcessor->requestUIPresetUpdate();
}

bool VASTPresetData::needsTreeUpdate() const {
	return m_needsTreeUpdate;
}

void VASTPresetData::clearNeedsTreeUpdate() {
	m_needsTreeUpdate = false;
}

int VASTPresetData::bankProgramGetPresetIndex(int bank, int program) {
	if ((bank < 0) || (bank > 3)) //A-D
		return -1;
	if (program >= m_programChangeData[bank].size())
		return -1;
	int idx = getIndexInPresetArray(m_programChangeData[bank][program]);
	if (idx < 0)
		return -1;
	int presetarrayindex = m_PresetArray[idx]->presetarrayindex;
	return presetarrayindex;
}

void VASTPresetData::removeProgramChangeData(int bank, int position) {
	m_programChangeData[bank].remove(position);
	m_needsTreeUpdate = true;
	writeToSettings();
}

void VASTPresetData::setProgramChangeArray(int bank, StringArray progChangeArray) {
	m_programChangeData[bank] = progChangeArray;
}

StringArray VASTPresetData::getProgramChangeData(int bank) const {
	return m_programChangeData[bank];
}

StringArray VASTPresetData::getProgramChangeDisplayData(int bank) const {
	StringArray pcdd;
	for (int i = 0; i < m_programChangeData[bank].size(); i++) {
		String sstr = m_programChangeData[bank][i];
		int idx = getIndexInPresetArray(sstr);
		if (idx >= 0)
			pcdd.add(String(i) + ": " + m_PresetArray[idx]->getPresetCategoryAndNameDisplay());
		else 
			pcdd.add(String(i) + ": " + "---");
	}
	return pcdd;
}

void VASTPresetData::swapProgramChangeData(int bankFrom, int bankTo, int position1, int position2) {
	String s1 = m_programChangeData[bankFrom][position1];
	String s2 = m_programChangeData[bankTo][position2];
	m_programChangeData[bankFrom].remove(position1);
	m_programChangeData[bankFrom].insert(position1, s2);
	m_programChangeData[bankTo].remove(position2);
	m_programChangeData[bankTo].insert(position2, s1);
	m_needsTreeUpdate = true;
	writeToSettings();
}

void VASTPresetData::moveProgramChangeData(int bankFrom, int bankTo, int position1, int position2) {
	String s1 = m_programChangeData[bankFrom][position1];
	String s2 = m_programChangeData[bankTo][position2];
	m_programChangeData[bankFrom].remove(position1);
	m_programChangeData[bankTo].insert(position2, s1);
	m_needsTreeUpdate = true;
	writeToSettings();
}

void VASTPresetData::setProgramChangeData(String internalid, int bank, int position) {
	if (m_programChangeData[bank].contains(internalid))
		return;
	if (m_programChangeData[bank].size() < position) //CHECK
		return;
	if ((position < 0) || (position > 126)) //CHECK
		return;
	m_programChangeData[bank].insert(position, internalid);
	m_needsTreeUpdate = true;
	writeToSettings();
}

void VASTPresetData::setFavorite(String internalid, int favoriteNo) {
	if (getFavorite(internalid, favoriteNo)) //already there
		return; 	
	m_favorites.insert(std::make_pair(internalid, favoriteNo));
	m_numFavorites[favoriteNo] ++;
	writeToSettings();
}

void VASTPresetData::setStars(String internalid, int stars) {
	if (internalid == "Init") return;
	auto it = m_stars.find(internalid);
	if (it != m_stars.end()) //already there
		it->second = stars;
	else 
		m_stars.insert(std::make_pair(internalid, stars));

	int idx = getIndexInPresetArray(internalid);
	if (idx >= 0)
		m_PresetArray[idx]->ranking = stars;
	idx = getIndexInSearchArray(internalid);
	if (idx>=0)
		m_SearchArray[idx]->ranking = stars;
	//m_stars.set(internalid, String(stars));
	writeToSettings();
}

int VASTPresetData::getStars(String internalid) {
	if (internalid == "Init") return 0;
	std::map<String, int>::iterator it;
	it = m_stars.find(internalid);
	if (it == m_stars.end()) return 0;
	return it->second;
}

bool VASTPresetData::getFavorite(String internalid, int favoriteNo) const {
	auto result = m_favorites.equal_range(internalid);
	for (auto it = result.first; it != result.second; ++it) {
		if (it->second == favoriteNo)
			return true;
	}
	return false;

	//String s1 = m_favorites[internalid];
	//if (s1 == "") return -1;
	//return s1.getIntValue();
}

void VASTPresetData::calcNumFavorites() {
	m_numFavorites[0] = 0;
	m_numFavorites[1] = 0;
	m_numFavorites[2] = 0;
	m_numFavorites[3] = 0;
	m_numFavorites[4] = 0;

	for (std::unordered_multimap<String, int>::iterator it = m_favorites.begin(); it != m_favorites.end(); ++it) {
		for (int fav = 0; fav < 5; fav++) {
			if (it->second == fav)
				m_numFavorites[fav]++;
		}
	}
}

int VASTPresetData::getNumFavorites(int favoriteNo) {
	return m_numFavorites[favoriteNo];
}

void VASTPresetData::removeAllFavorites(int favoriteNo) {
	for (auto it = m_favorites.begin(); it != m_favorites.end(); ) {
		if (it->second == favoriteNo)
			it = m_favorites.erase(it);
		else
			++it;
	}
	
	writeToSettings();
	calcNumFavorites();
}

void VASTPresetData::removeFromAllFavorites(String internalid) {
	for (auto it = m_favorites.begin(); it != m_favorites.end(); ) {
		if (it->first == internalid)
			it = m_favorites.erase(it);
		else
			++it;
	}

	writeToSettings();
	calcNumFavorites();
}

void VASTPresetData::addTag(String tagToAdd) {
	StringArray tags;
	tags.addTokens(m_curPatchData.freetag, " ,#", "\"");
	if (!(tags.contains(tagToAdd, true)))
		m_curPatchData.freetag = m_curPatchData.freetag + " " + tagToAdd;
}

inline int VASTPresetData::VASTPresetElementCompareDates::compareElements(VASTPresetElement* first, const VASTPresetElement* second)
{
	return (first->presetdate < second->presetdate) ? -1
		: ((first->presetdate == second->presetdate) ? 0 : 1);
}
