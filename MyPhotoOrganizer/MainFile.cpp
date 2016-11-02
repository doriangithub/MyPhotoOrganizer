#include <Windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <tchar.h>
#include <atlstr.h>
#include "FilesDB.h"
#include "Settings.h"

using namespace std;


CString GetPathToExeFileFolder();
int ReadStandardFileToMemmory(CSettings *appSettings);

int main(int argv, char* args[])
{
	CFilesDB photosDB = CFilesDB();

	int rezult = photosDB.initializeDB();

	if (rezult != 1)
	{
		// cannot initialize db
	}

	// if database is opened successfully, read ini file
	CSettings appSettings = CSettings();
	ReadStandardFileToMemmory(&appSettings);

	//we prompt for start folder,
	// from where we start search files

	// then we display extensions of files we gonna search saved in ini file





	return 1;
}


CString GetPathToExeFileFolder()
{
	// getting location of this exe file to get the path of working folder
	TCHAR exePath[MAX_PATH];
	if (!GetModuleFileName(NULL, exePath, MAX_PATH))
	{
		printf("Cannot find location (%d)\n", GetLastError());
		return _T("0");
	}

	CString strPathToFile(exePath);							    // casting to CString
	CT2CA pszConvertedAnsiString(strPathToFile);				// Convert a TCHAR string to a LPCSTR
	std::string strStdPathToFolder(pszConvertedAnsiString);		// construct a std::string using the LPCSTR input

	unsigned found = strStdPathToFolder.find_last_of("/\\");	// getting location of the last "\" in the path

	CString strPathToFolder = strPathToFile.Left(found);		// cut off file name from the path

	return strPathToFolder;
}


int ReadStandardFileToMemmory(CSettings *appSettings)
{

	CString pathToExeFileFolder = GetPathToExeFileFolder();
	CString pathToIniFile = pathToExeFileFolder + "\\settings.ini";
	ifstream standardFileRead(pathToIniFile);
	if (!standardFileRead.is_open())
	{              
	}
	                               
	OutputDebugString(_T("file open\n"));

	// cnange status text in the dialog
	//ChangeStatusText(_T("Reading standards to memmory"));

	// read standards file to memmory

	// ReadStandardsFileToArray();
	string line;

	string partOfLine;
	CString strExtensions = _T("[EXTENSIONS]");
	CString strStartPath = _T("[STARTPATH]");
	CString value;
	CString srtRighttPartOfLine;
	bool bKeysFolder = false;
	CString strKeyName;
	CString strKeyValue;
	unsigned lengthLine;
	unsigned found;

	while (standardFileRead.good())
	{
		getline(standardFileRead, line);
		if (line.length() != 0)
		{
			CString csLine(line.c_str());

			// check if it [EXTENSIONS] section
			if (strExtensions.Compare(csLine) == 0)
			{
				// read next line with values
				getline(standardFileRead, line);
				if (line.length() != 0)
				{
					CString csLine(line.c_str());
					found = csLine.Find(L";");
					while (found != -1)
					{
						value = csLine.Left(found);
						if (value.Compare(L"") != 0)
						{
							appSettings->addExtension(value);
						}
						csLine = csLine.Right(found);
						found = csLine.Find(L";");
					}
					if (csLine.Compare(L"") != 0)
					{
						appSettings->addExtension(csLine);
					}


				}
			}

			// check if it [STARTPATH] section
			if (strStartPath.Compare(csLine) == 0)
			{
				// read next line with values
				getline(standardFileRead, line);
				if (line.length() != 0)
				{
					CString csLine(line.c_str());
					appSettings->setStartPath(csLine);
				}
			}
		}
		else
		{
			// empty line;
		}
	}
	standardFileRead.close();

	return 1;
}