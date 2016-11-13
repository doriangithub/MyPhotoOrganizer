///////////////////////////////////////////////////////////////////////////////
// Main.cpp
// ==========
// Main file of My Photo Organizer
//
//  AUTHOR: Dorian.Sharevich (dorian.sharevich@gmail.com)
// CREATED: 2016-11-03
// UPDATED: 2016-11-06
///////////////////////////////////////////////////////////////////////////////
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <tchar.h>
#include <atlstr.h>
#include <Strsafe.h>
#include <gdiplus.h>
#include <comdef.h>  // you will need this
#include "FilesDB.h"
#include "Settings.h"

using namespace std;
using namespace Gdiplus;

CString GetPathToExeFileFolder();
int ReadIniFileToMemmory(CSettings *appSettings);
bool dirExists(LPCSTR dirName_in);
void promtForStartPath(CSettings *appSettings);
void promtForExtensions(CSettings *appSettings);
void RecurseSearch(TCHAR* path, CSettings *appSettings, CFilesDB *photosDB);
bool hasExtension(const WCHAR* fileName, CSettings *appSettings);
HRESULT PropertyTypeFromWORD(WORD index, WCHAR* string, UINT maxChars);
char* metadataDateTaken(TCHAR* fullPathToFile);

static int numberOfMediaFiles = 0;

//CString months[] = {	"January", "February" , "March", "April", "May", "June",
//				"July", "August", "September", "October", "November", "December" };

/***********************************************************************************************/
/*	Main function of application My Photo Organizer                                            */
/***********************************************************************************************/
int main(int argv, char* args[])
{
	CFilesDB photosDB = CFilesDB();

	int rezult = photosDB.initializeDB();

	if (rezult != 1)
	{
		// cannot initialize db
		// show error and quirt
		return 1;
	}

	// if database is opened successfully, read ini file
	CSettings appSettings = CSettings();
	rezult = ReadIniFileToMemmory(&appSettings);

	if (rezult != 1)
	{
		// cannot read ini file
		// show error and quirt
		return 1;
	}

	// create table in db
	/* Create SQL statement */
	char* sql = "CREATE TABLE MEDIAFILES("
		"ID		INT 	PRIMARY KEY      NOT NULL,"
		"FILE_NAME				TEXT     NOT NULL,"		//	- original file name -> fileName
		"FILE_PATH				TEXT     NOT NULL,"		//	- original file path -> filePath
		"PICTURE_TAKEN_DATE		TEXT     NOT NULL,"		//	- data picture taken
		"FILE_SIZE				INT     NOT NULL);";	//	- file size
														//	- history of copying/moving

	rezult = photosDB.createTable(sql);


	//==============================================================================
	// we prompt for start folder, from where we start search files
	promtForStartPath(&appSettings);
	
	//==============================================================================
	// we prompt for extensions of files we gonna search saved in ini file
	promtForExtensions(&appSettings);

	//==============================================================================
	// print cirent settings for debug purpose
	CString currentStartPath =  (LPCTSTR)appSettings.getStartPath();

	std::vector <CString> currentExtensionsArr = appSettings.getExtensionsArray();
	CString currentExtensionsList = "";
	for (std::vector<CString>::iterator it = currentExtensionsArr.begin(); it != currentExtensionsArr.end(); ++it)
	{
		currentExtensionsList += ((CString)*it + L";");
	}
	
	_tprintf(_T("Current star folder[%s]\n"), currentStartPath);
	_tprintf(_T("Current extensions list[%s]:\n"), currentExtensionsList);
	
	//===============================================================================
	// Start recursevly search files in all subdirectories

	TCHAR* currentStartPathTchar = new TCHAR[currentStartPath.GetLength() + 1];
	lstrcpy(currentStartPathTchar, currentStartPath);

	RecurseSearch(currentStartPathTchar, &appSettings, &photosDB);
	
	//===============================================================================
	// print out content of database

	/* Create SQL statement */
	sql = "SELECT * from MEDIAFILES";

	photosDB.retriveDataFormDataBase(sql);

	//===============================================================================
	// make new folders structure

	// check existance of root folder
	CString rootPath = appSettings.getLibRootPath();

	bool rezultBool;

	// Convert a TCHAR string to a LPCSTR
	// CT2CA pszConvertedAnsiString(rootPath);
	// construct a std::string using the LPCSTR input
	// std::string strStd(pszConvertedAnsiString);

	CStringA strARootPath(rootPath); // a helper string
	LPCSTR ptrRootPath = strARootPath;

	rezultBool = dirExists(ptrRootPath);	//bool dirExists(const std::string& dirName_in)

	CA2CT prtWcharRootPath(ptrRootPath);

	if (!rezultBool)
	{
		// make the root folder
		rezultBool = CreateDirectory(prtWcharRootPath, NULL);
		if (!rezultBool)
		{
			printf("CreateDirectory failed (%d)\n", GetLastError());
			return -1;
		}
	}


	std::vector<FileMetadata> filesMetaDataFromDB = photosDB.getFileMetadata();
	CString tempCString;
	CString tempCStringRight;

	for (std::vector<FileMetadata>::iterator itVec3d = filesMetaDataFromDB.begin(); itVec3d != filesMetaDataFromDB.end(); itVec3d++)
	{
		FileMetadata nextFileMetadata = (FileMetadata)(*itVec3d);

		char *nextFilePicTakenDate = nextFileMetadata.getPictureTakenDate();

		// make path from date
		CString csNextFilePicTakenDate(nextFilePicTakenDate);
		int stringLength = csNextFilePicTakenDate.GetLength();
		int found = csNextFilePicTakenDate.Find(L":");
		
		int year = 0;
		int month = 0;
		int day = 0;
		LPCSTR yearStr;
		LPCSTR monthStr;
		LPCSTR dayStr;

		tempCString = csNextFilePicTakenDate.Left(found);
		year = _wtoi(tempCString);
		if ( year > 1980 && year < 3000)
		{
			CStringA strA(tempCString); // a helper string CString to LPCSTR
			yearStr = strA;
		}
		tempCStringRight = csNextFilePicTakenDate.Right(stringLength - found - 1);
		found = tempCStringRight.Find(L":");
		tempCString = tempCStringRight.Left(found);
		stringLength = tempCStringRight.GetLength();
		month = _wtoi(tempCString);
		if (month > 0 && month <= 12)
		{
			CStringA strA(tempCString); // a helper string CString to LPCSTR
			monthStr = strA;
		}
		tempCStringRight = csNextFilePicTakenDate.Right(stringLength - found - 1);
		found = tempCStringRight.Find(L" ");
		tempCString = tempCStringRight.Left(found);
		day = _wtoi(tempCString);
		if (day > 0 && day <= 31)
		{
			CStringA strA(tempCString); // a helper string CString to LPCSTR
			dayStr = strA;
		}



	}

	photosDB.printFileMetadata();

	delete[]currentStartPathTchar;
	system("pause");
	return 1;
}


/***********************************************************************************************/
/*	Utility function returns path to application's exe file                                    */
/***********************************************************************************************/
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


/***********************************************************************************************/
/*	Function reads ini file and stores settings in class CSettings                             */
/***********************************************************************************************/
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
	CString strLibRootPath = _T("[LIBRARYROOT]");	
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

			// check if it [LIBRARYROOT] section
			if (strLibRootPath.Compare(csLine) == 0)
			{
				// read next line with values
				getline(standardFileRead, line);
				if (line.length() != 0)
				{
					CString csLine(line.c_str());
					appSettings->setLibRootPath(csLine);
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


/***********************************************************************************************/
/*	Utility function to check if the directory is exists                                       */
/***********************************************************************************************/
bool dirExists(LPCSTR dirName_in)
{
	DWORD ftyp = GetFileAttributesA(dirName_in);
	if (ftyp == INVALID_FILE_ATTRIBUTES)
	{
		cout << "Error: NO SUCH FOLDER!!!" << endl;
		return false;  //something is wrong with your path!
	}

	if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
		return true;   // this is a directory!

	return false;    // this is not a directory!
}


/***********************************************************************************************/
/*	Show to the user existing start path from settings and ask new files extensions to search  */
/***********************************************************************************************/
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
			cout << "Is it correct (Y/N)?";
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

		answerRezult = dirExists(startPath.c_str());

	} while (!answerRezult);

	appSettings->setStartPath(answer.c_str());
	return;
}


/*******************************************************************************************************/
/*	Show to the user existing list of extensions from settings and ask new files extensions to search  */
/*******************************************************************************************************/
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

	

	return;

}


/************************************************************************************/
/*	Search Files In Subfolders Recursively                                          */
/************************************************************************************/
void RecurseSearch(TCHAR* path, CSettings *appSettings, CFilesDB *photosDB)
{
	wprintf(L"call Recurse for %s.\n", path);
	WIN32_FIND_DATA FileData;
	HANDLE          hSearch;
	DWORD           dwAttrs;

	BOOL            fFinished = FALSE;

	// build a string with wildcards
	TCHAR* extra = L"\\*.*";
	TCHAR* backslash = L"\\";

	TCHAR result[250];   // array to hold the result.
	memset(result, 0, sizeof(TCHAR) * 250);

	wcscpy_s(result, path); // copy string one into the result.
	wcscat_s(result, extra); // append string two to the result.


	// Start searching for files in the current directory. 

	hSearch = FindFirstFile(result, &FileData);
	if (hSearch == INVALID_HANDLE_VALUE)
	{
		wprintf(L"No files found.\n");
		return;
	}

	while (!fFinished)
	{
		//wprintf(L"File name: %s\n", FileData.cFileName);

		// skip . and .. files; otherwise, we'd
		// recur infinitely!
		if (FileData.cFileName[0] == '.')
		{
			if (!FindNextFile(hSearch, &FileData))
			{
				if (GetLastError() == ERROR_NO_MORE_FILES)
				{
					fFinished = TRUE;
				}
				else
				{
					wprintf(L"Could not find next file.\n");
					return;
				}
			}
			continue;
		}

		dwAttrs = FileData.dwFileAttributes;

		if (dwAttrs & FILE_ATTRIBUTE_DIRECTORY)
		{
			//wprintf(L"%s is a directory\n", FileData.cFileName);
			// if it's a directory, recursively search it
			memset(result, 0, sizeof(TCHAR) * 250);  // reset path
			wcscpy_s(result, path); // copy string one into the result.
			wcscat_s(result, backslash);
			wcscat_s(result, FileData.cFileName); // copy string one into the result.
												  //wprintf(L"Path: %s\n", result); 
			RecurseSearch(result, appSettings, photosDB);
		}
		else
		{
			wprintf(L"File name: %s\n", FileData.cFileName);

			// check for the extension
			if (hasExtension(FileData.cFileName, appSettings))
			{
				numberOfMediaFiles++;

				wcout << L"Add file " << FileData.cFileName << " to database " << endl;
				//	If you find JPG file, add to the database:
				//	- original file name -> fileName
				//	- original file path -> filePath


				//	- original file modified date 
				//	- data picture taken
				//	- file size
				//	- history of copying/moving

				
				WCHAR* fileName = FileData.cFileName;
				int fileSizeLow = FileData.nFileSizeLow;	// The low-order DWORD value of the file size, in bytes.
				int fileSizeHigh = FileData.nFileSizeHigh;	// The high-order DWORD value of the file size, in bytes.
															// This value is zero unless the file size is greater than MAXDWORD.
															// The size of the file is equal to(nFileSizeHigh * (MAXDWORD + 1)) + nFileSizeLow.

				TCHAR* filePath = path;

				TCHAR fullPathToFile[256];
				_stprintf(fullPathToFile, _T("%s%s%s"), filePath, _T("\\"), fileName);

				

				



				char sqlStatement[800];
	
				

				// ID
				int id = numberOfMediaFiles;
				
				// FILE_NAME
				_bstr_t fileNameWchar(fileName);
				char* fileNameChar = fileNameWchar;

				// FILE_PATH
				char filePathChar[256];
				wcstombs(filePathChar, filePath, wcslen(filePath) + 1);

				// PICTURE_TAKEN_DATE
				char* datePtotoTaken = metadataDateTaken(fullPathToFile);		//  YYYY:MM:DD HH:MM:SS

				// FILE_SIZE
				int fileSize = (fileSizeHigh * (MAXDWORD + 1)) + fileSizeLow;

				if (strcmp(datePtotoTaken, "") != 0)
				{

					sprintf(sqlStatement, "%s%d,'%s','%s','%s',%d);",
						"INSERT INTO MEDIAFILES(ID, FILE_NAME, FILE_PATH, PICTURE_TAKEN_DATE, FILE_SIZE ) VALUES (",
						id,
						fileNameChar,
						filePathChar,
						datePtotoTaken,
						fileSize
					);

					//
					// if the path is not in library root
					//
					CString rootPath = appSettings->getLibRootPath();
					int rootPathLength = rootPath.GetLength();
					
					CString filePathCString(filePath);
					int filePathLength = filePathCString.GetLength();

					if (filePathLength >= rootPathLength)
					{
						CString leftOfFilePath = filePathCString.Left(rootPathLength);
						if (rootPath != leftOfFilePath)
							photosDB->insertData(sqlStatement);
					}
				}
			}

		}

		if (!FindNextFile(hSearch, &FileData))
		{
			if (GetLastError() == ERROR_NO_MORE_FILES)
			{
				fFinished = TRUE;
			}
			else
			{
				printf("Could not find next file.\n");
				return;
			}
		}
	}

	// Close the search handle. 

	FindClose(hSearch);
}


/*********************************************************************************************/
/*	 Utility function to check extension of the file                                         */
/*********************************************************************************************/
bool hasExtension(const WCHAR* fileName, CSettings *appSettings)
{
	const int num = wcslen(fileName);	// legth of the fileName

	bool found = false;

	std::vector <CString> currentExtensionsArr = appSettings->getExtensionsArray();
	CString currentExtension = L"";
	for (std::vector<CString>::iterator it = currentExtensionsArr.begin(); it != currentExtensionsArr.end(); ++it)
	{
		currentExtension = (CString)*it;
		currentExtension = currentExtension.MakeLower();	// make it lower case

		const unsigned int NUM_ELEMENTS = wcslen(fileName); // the size of the array which holds path
		WCHAR tempArray[MAX_PATH];
		wmemset(tempArray, 0, MAX_PATH);
		wcsncpy_s(tempArray, fileName, NUM_ELEMENTS);

		const unsigned int NUM_ELEMENTS_TEMP = wcslen(fileName);

		// search "." to separate extension

		CString wholeFileName(fileName);										// casting to CString
		CT2CA pszConvertedAnsiString(wholeFileName);							// Convert a TCHAR string to a LPCSTR
		std::string strStdWholeFileName(pszConvertedAnsiString);				// construct a std::string using the LPCSTR input

		unsigned found = strStdWholeFileName.find_last_of("/.");				// getting location of the last "\" in the path

		unsigned extLengtgh = strStdWholeFileName.length() - ( found +  1);		// 1 is for dot
		
		CString strPathToFolder = wholeFileName.Right(extLengtgh);				// cut off file name from the path

		strPathToFolder = strPathToFolder.MakeLower();

		if (wcscmp(strPathToFolder, currentExtension) == 0)						//extensionFormThefile
		{
			found = true;
			return found;
		}
	}

	return found;
}


/*********************************************************************************************/
/*	 Helper function                                         */
/*********************************************************************************************/
HRESULT PropertyTypeFromWORD(WORD index, WCHAR* string, UINT maxChars)
{
	HRESULT hr = E_FAIL;

	WCHAR* propertyTypes[] = {
		L"Nothing",                   // 0
		L"PropertyTagTypeByte",       // 1
		L"PropertyTagTypeASCII",      // 2
		L"PropertyTagTypeShort",      // 3
		L"PropertyTagTypeLong",       // 4
		L"PropertyTagTypeRational",   // 5
		L"Nothing",                   // 6
		L"PropertyTagTypeUndefined",  // 7
		L"Nothing",                   // 8
		L"PropertyTagTypeSLONG",      // 9
		L"PropertyTagTypeSRational" }; // 10

	hr = StringCchCopyW(string, maxChars, propertyTypes[index]);
	return hr;
}


char* metadataDateTaken(TCHAR* fullPathToFile)
{
	char* dateTaken = "";

	// Initialize <tla rid="tla_gdiplus"/>.
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	UINT    size = 0;
	UINT    count = 0;

#define MAX_PROPTYPE_SIZE 30
	WCHAR strPropertyType[MAX_PROPTYPE_SIZE] = L"";

	Bitmap* bitmap = new Bitmap(fullPathToFile);
	bitmap->GetPropertySize(&size, &count);

	printf("There are %d pieces of metadata in the file.\n", count);
	printf("The total size of the metadata is %d bytes.\n", size);

	// GetAllPropertyItems returns an array of PropertyItem objects.
	// Allocate a buffer large enough to receive that array.
	PropertyItem* pPropBuffer = (PropertyItem*)malloc(size);

	// Get the array of PropertyItem objects.
	bitmap->GetAllPropertyItems(size, count, pPropBuffer);

	// For each PropertyItem in the array, display the id, type, and length.
	for (UINT j = 0; j < count; ++j)
	{
		// Convert the property type from a WORD to a string.
		PropertyTypeFromWORD(pPropBuffer[j].type, strPropertyType, MAX_PROPTYPE_SIZE);

		printf("Property Item %d\n", j);
		printf("  id: 0x%x\n", pPropBuffer[j].id);
		wprintf(L"  type: %s\n", strPropertyType);
		printf("  length: %d bytes\n\n", pPropBuffer[j].length);
		
		unsigned long int tagValue = pPropBuffer[j].id;

		switch (tagValue)
		{
		case 0x9003:
			printf("****Date and time when the original image data was generated(YYYY:MM:DD HH:MM:SS): %s.\n", pPropBuffer[j].value);
			dateTaken = (char*)pPropBuffer[j].value;
			break;
		case 0x013B:
			printf("***######***Who created the image: %s.\n", pPropBuffer[j].value);
			break;
		default:
			break;
		}
	}

	//free(pPropBuffer);
	delete bitmap;
	GdiplusShutdown(gdiplusToken);

	return dateTaken;
}