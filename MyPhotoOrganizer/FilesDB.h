#pragma once
class CFilesDB
{
private:
	int i;
public:
	CFilesDB();
	~CFilesDB();

	bool dbFileExists();
	int initializeDB();
	int createTable(char* sqlStr);

};

