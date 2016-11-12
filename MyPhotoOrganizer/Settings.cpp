///////////////////////////////////////////////////////////////////////////////
// Settings.cpp
// ==========
// Setting class
//
//  AUTHOR: Dorian.Sharevich (dorian.sharevich@gmail.com)
// CREATED: 2016-11-03
// UPDATED: 2016-11-06
///////////////////////////////////////////////////////////////////////////////
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


void CSettings::setLibRootPath(CString libRootpathStr)
{
	this->libRootpathStr = libRootpathStr;
}


CString CSettings::getLibRootPath()
{
	return this->libRootpathStr;
}


std::vector <CString> CSettings::getExtensionsArray()
{
	return this->extensionsArray;
}


void CSettings::deleteExtensionsList()
{
	this->extensionsArray.clear();
}