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

