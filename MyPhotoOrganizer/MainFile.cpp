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
int ReadIniFileToMemmory(CSettings *appSettings);
bool dirExists(const std::string& dirName_in);
void promtForStartPath(CSettings *appSettings);
void promtForExtensions(CSettings *appSettings);

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
	ReadIniFileToMemmory(&appSettings);

	//==============================================================================
	//we prompt for start folder,
	// from where we start search files
	promtForStartPath(&appSettings);
	
	//==============================================================================

	// then we display extensions of files we gonna search saved in ini file
	promtForExtensions(&appSettings);




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


int ReadIniFileToMemmory(CSettings *appSettings)
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
	int stringLength;

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
						stringLength = csLine.GetLength();
						if ((found + 1) < stringLength)
						{
							csLine = csLine.Right(found + 1);
							found = csLine.Find(L";");
						}
						else
						{
							csLine = L"";
							found = csLine.Find(L";");
						}

					}
					//if (csLine.Compare(L"") != 0)
					//{
					//	appSettings->addExtension(csLine);
					//}


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


bool dirExists(const std::string& dirName_in)
{
	DWORD ftyp = GetFileAttributesA(dirName_in.c_str());
	if (ftyp == INVALID_FILE_ATTRIBUTES)
	{
		cout << "Error: NO SUCH FOLDER!!!" << endl;
		return false;  //something is wrong with your path!
	}

	if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
		return true;   // this is a directory!

	return false;    // this is not a directory!
}


void promtForStartPath(CSettings *appSettings)
{
	string startPath;
	string answer;
	bool folderExists = false;
	bool answerRezult = false;

	//	Get root directory -  prompt the start folder

	do
	{
		wcout << "Please enter the path to folder to start search from." << endl;
		_tprintf(_T("Star folder[%s]:"), (LPCTSTR)appSettings->getStartPath());
		getline(cin, startPath);

		if (startPath != "")
		{
			cout << "You have entered this path:" << endl;
			cout << startPath << endl;
			wcout << "Is it correct (Y/N)?";
			getline(cin, answer);
		}
		else
		{
			return;
		}

		if (answer == "n")
		{
			answerRezult = false;
			continue;
		}
		if (answer == "y")
		{
			answerRezult = true;
		}

		answerRezult = dirExists(startPath);

	} while (!answerRezult);

	appSettings->setStartPath(answer.c_str());
	return;
}


void promtForExtensions(CSettings *appSettings)
{
	string extensions;
	string answer;
	bool answerRezult = false;

	//	Get list of extensions -  prompt the extensions of files to search

	do
	{
		wcout << "Please enter extensions of files separated by ';'" << endl;
		std::vector <CString> localExtensionsArr = appSettings->getExtensionsArray();
		
		CString extensionsFromIni= "";

		for (std::vector<CString>::iterator it = localExtensionsArr.begin(); it != localExtensionsArr.end(); ++it)
		{
			extensionsFromIni += ((CString)*it + L";");
		}

		//int numElements = localExtensionsArr.size();

		_tprintf(_T("Star folder[%s]:"), (LPCTSTR)extensionsFromIni);

		getline(cin, extensions);

		if (extensions != "")
		{
			cout << "You have entered this extension(s):" << endl;
			cout << extensions << endl;
			wcout << "Is it correct (Y/N)?";
			getline(cin, answer);
		}
		else
		{
			return;
		}

		if (answer == "n")
		{
			answerRezult = false;
			continue;
		}
		if (answer == "y")
		{
			answerRezult = true;
		}

		//verify here format of entered extensions
		//answerRezult = checkFormatExtensions(extensions);

	} while (!answerRezult);

	unsigned found;
	CString value;
	int stringLength;

	if (extensions.length() != 0)
	{
		// delete existing list
		appSettings->deleteExtensionsList();

		CString csLine(extensions.c_str());
		found = csLine.Find(L";");
		while (found != -1)
		{
			value = csLine.Left(found);
			if (value.Compare(L"") != 0)
			{
				appSettings->addExtension(value);
			}
			stringLength = csLine.GetLength();
			if ((found + 1) < stringLength)
			{
				csLine = csLine.Right(found + 1);
				found = csLine.Find(L";");
			}
			else
			{
				csLine = L"";
				found = csLine.Find(L";");
			}

		}
		if (csLine.Compare(L"") != 0)
		{
			appSettings->addExtension(csLine);
		}
	}

	std::vector <CString> localExtensionsArr = appSettings->getExtensionsArray();

	return;

}


