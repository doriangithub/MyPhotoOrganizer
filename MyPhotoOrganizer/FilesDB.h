///////////////////////////////////////////////////////////////////////////////
// FilesDB.h
// ==========
// FilesDB class
//
//  AUTHOR: Dorian.Sharevich (dorian.sharevich@gmail.com)
// CREATED: 2016-11-03
// UPDATED: 2016-11-06
///////////////////////////////////////////////////////////////////////////////
#pragma once
#include <vector>
#include "FileMetadata.h"

class CFilesDB
{
private:
	int i;
	
	
public:
	CFilesDB();
	~CFilesDB();

	int initializeDB();
	int createTable(const char* sqlStr);
	int insertData(const char* sqlStr);
	int printData(const char* sqlStr);
	//std::vector<FileMetadata> getFileMetadata();
	//std::vector<FileMetadata> fileMetadata;
	void printFileMetadata();
};

