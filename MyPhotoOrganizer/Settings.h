///////////////////////////////////////////////////////////////////////////////
// Settings.h
// ==========
// Setting class
//
//  AUTHOR: Dorian.Sharevich (dorian.sharevich@gmail.com)
// CREATED: 2016-11-03
// UPDATED: 2016-11-06
///////////////////////////////////////////////////////////////////////////////
#pragma once
#include <vector>
#include <atlstr.h>

class CSettings
{
public:
	CSettings();
	~CSettings();
	void addExtension(CString extStr);
	void deleteExtensionsList();
	void setStartPath(CString startpathStr);
	CString getStartPath();
	std::vector <CString> getExtensionsArray();
private:
	std::vector <CString> extensionsArray;
	CString startPath;

};

