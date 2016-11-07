///////////////////////////////////////////////////////////////////////////////
// FilesDB.cpp
// ==========
// FilesDB class
//
//  AUTHOR: Dorian.Sharevich (dorian.sharevich@gmail.com)
// CREATED: 2016-11-03
// UPDATED: 2016-11-06
///////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <sqlite3.h> 
#include <iostream>

#include "FilesDB.h"

static const char DBFILENAME[] = "Files.db";

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
	int i;
	for (i = 0; i<argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}


CFilesDB::CFilesDB()
{
}


CFilesDB::~CFilesDB()
{
}


int CFilesDB::initializeDB()
{
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	char *sql;

	/* Open database */
	rc = sqlite3_open(DBFILENAME, &db);

	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		return -1;
	}
	else {
		fprintf(stderr, "Opened database successfully\n");
	}
	sqlite3_close(db);
	return 1;
}


int CFilesDB::createTable(const char* sqlStr)
{
	sqlite3 *db;
	char *zErrMsg = 0;
	int  rc;

	/* Open database */
	rc = sqlite3_open(DBFILENAME, &db);
	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		return(0);
	}
	else {
		fprintf(stdout, "Opened database successfully\n");
	}

	/* Execute SQL statement */
	rc = sqlite3_exec(db, sqlStr, callback, 0, &zErrMsg);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else {
		fprintf(stdout, "Table created successfully\n");
	}
	sqlite3_close(db);
	return 0;
}
