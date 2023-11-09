/*
  ==============================================================================

    VASTPresetElement.h

  ==============================================================================
*/

#ifndef VASTPRESETELEMENT_H_INCLUDED
#define VASTPRESETELEMENT_H_INCLUDED

#include "../../Engine/VASTEngineHeader.h"

class VASTPresetElement
{
public:
	VASTPresetElement()	{
		clear();
	};

	~VASTPresetElement() {
	}

	bool containsString(const String text) {
		if (presetid.isNotEmpty() && (presetid.containsIgnoreCase(text) == true)) return true;
		if (presetname.isNotEmpty() && (presetname.containsIgnoreCase(text) == true)) return true;
		if (presetdate.isNotEmpty() && (presetdate.containsIgnoreCase(text) == true)) return true;
		if (category.isNotEmpty() && (category.containsIgnoreCase(text) == true)) return true;
		if (freetag.isNotEmpty() && (freetag.containsIgnoreCase(text) == true)) return true;
		if (authorname.isNotEmpty() && (authorname.containsIgnoreCase(text) == true)) return true;
		if (foldername.isNotEmpty() && (foldername.containsIgnoreCase(text) == true)) return true;
		if (comments.isNotEmpty() && (comments.containsIgnoreCase(text) == true)) return true;
		return false;
	};
	void calcID() {
		if (isFactory == true) {			
			presetid = String("INIT");
		}
		else {
			String numberString = (userpatchindex + 1 < 10) ? "00" + String(userpatchindex + 1) :
				((userpatchindex + 1 < 100) ? "0" + String(userpatchindex + 1) : String(userpatchindex + 1));
			presetid = String("USR" + numberString);
		}
	};
	const String getPresetIdDisplay() {
		return presetid;
	}
	const String getCategoryDisplay() {
		return category;
	}
	const String getPresetNameDisplay() {
		return presetname;
	}
	const String getPresetCategoryAndNameDisplay() {
		return category + " " + presetname;
	}
	const String getTagDisplay() {
		return freetag;
	}
	const String getAuthorDisplay() {
		return authorname;
	}
	const String getFolderDisplay() {
		return foldername;
	}
	const String getDateDisplay() {
		return presetdate;
	}
	const String getComments() {
		return comments;
	}
	const String getCustomMoudlatortext1() {
		return customModulator1Text;
	}
	const String getCustomMoudlatortext2() {
		return customModulator2Text;
	}
	const String getCustomMoudlatortext3() {
		return customModulator3Text;
	}
	const String getCustomMoudlatortext4() {
		return customModulator4Text;
	}
	const int getRanking() const {
		return ranking;
	}

	void clear() {
		presetarrayindex = 0;
		userpatchindex = 0;
		internalid = "";
		isFactory = false;
		mpepreset = false;
		mpebendrange = 64;
		presetid = "";
		presetname = "";
		presetdate = "";
		category = "";
		freetag = "";
		authorname = "";
		foldername = "";
		filename = "";
		version = "";
		comments = "";
		ranking = 0;
		customModulator1Text = "Custom1";
		customModulator2Text = "Custom2";
		customModulator3Text = "Custom3";
		customModulator4Text = "Custom4";
	}

	int presetarrayindex = 0;
	int userpatchindex = 0;	
	bool isFactory = false;
	String internalid;
	String presetid;
	String presetname;
	String presetdate;
	String category;
	String freetag;
	String authorname;
	String foldername;
	String filename;
	String version;
	String comments;
	bool mpepreset = false;
	int mpebendrange = 64;
	String customModulator1Text;
	String customModulator2Text;
	String customModulator3Text;
	String customModulator4Text;
	int ranking = 0;
};

#endif  // VASTPRESETELEMENT_H_INCLUDED
