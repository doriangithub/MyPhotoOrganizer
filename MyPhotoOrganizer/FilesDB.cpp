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
#include <stdlib.h>



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

static int callbackSelect(void *data, int argc, char **argv, char **azColName) {
	int i;
	fprintf(stderr, "%s: ", (const char*)data);
	for (i = 0; i<argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}

typedef int(*sqlite3_callback)(
	void*,    /* Data provided in the 4th argument of sqlite3_exec() */
	int,      /* The number of columns in row */
	char**,   /* An array of strings representing fields in the row */
	char**    /* An array of strings representing column names */
	);


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


int CFilesDB::insertData(const char* sqlStr)
{
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	//char *sql;

	/* Open database */
	rc = sqlite3_open(DBFILENAME, &db);
	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		return(0);
	}
	else {
		fprintf(stderr, "Opened database successfully\n");
	}

	/* Create SQL statement */
	//sql = "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "  \
	//	"VALUES (1, 'Paul', 32, 'California', 20000.00 ); " \
	//	"INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "  \
	//	"VALUES (2, 'Allen', 25, 'Texas', 15000.00 ); "     \
	//	"INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)" \
	//	"VALUES (3, 'Teddy', 23, 'Norway', 20000.00 );" \
	//	"INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)" \
	//	"VALUES (4, 'Mark', 25, 'Rich-Mond ', 65000.00 );";

	/* Execute SQL statement */
	rc = sqlite3_exec(db, sqlStr, callback, 0, &zErrMsg);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else {
		fprintf(stdout, "Records created successfully\n");
	}
	sqlite3_close(db);
	return 0;
}


int CFilesDB::printData(const char* sqlStr)  //callbackSelect
{
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	char *sql;
	const char* data = "Callback function called";

	/* Open database */
	rc = sqlite3_open(DBFILENAME, &db);
	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		return(0);
	}
	else {
		fprintf(stderr, "Opened database successfully\n");
	}



	/* Execute SQL statement */
	rc = sqlite3_exec(db, sqlStr, callbackSelect, (void*)data, &zErrMsg);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else {
		fprintf(stdout, "Operation done successfully\n");
	}
	sqlite3_close(db);
	return 0;
}