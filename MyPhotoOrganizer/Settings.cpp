#include "Settings.h"



CSettings::CSettings():extensionsArray(NULL)
{
}


CSettings::~CSettings()
{
}


void CSettings::addExtension(CString extStr)
{
	this->extensionsArray.push_back(extStr);
}


void CSettings::setStartPath(CString startpathStr)
{
	this->startPath = startpathStr;
}


CString CSettings::getStartPath()
{
	return this->startPath;
}


std::vector <CString> CSettings::getExtensionsArray()
{
	return this->extensionsArray;
}


void CSettings::deleteExtensionsList()
{
	this->extensionsArray.clear();
}