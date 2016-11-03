#pragma once
#include <vector>
#include <atlstr.h>

class CSettings
{
public:
	CSettings();
	~CSettings();
	void addExtension(CString extStr);
	void setStartPath(CString startpathStr);
	CString getStartPath();
private:
	std::vector <CString> extensionsArray;
	CString startPath;

};

