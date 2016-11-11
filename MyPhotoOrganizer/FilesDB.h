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

};

