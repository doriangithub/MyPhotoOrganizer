#include <stdlib.h> 
#include <string.h>
#include "FileMetadata.h"

FileMetadata::FileMetadata()
{
}


FileMetadata::~FileMetadata()
{
}


void FileMetadata::setId(int id)
{
	this->id = id;
}


void FileMetadata::setFileName(char *fileName)
{
	this->fileName = (char*)malloc(strlen(fileName) + 1);
	strcpy(this->fileName, fileName);
}


void FileMetadata::setFilePath(char *filePath)
{
	this->filePath = (char*)malloc(strlen(filePath) + 1);
	strcpy(this->filePath, filePath);
}


void FileMetadata::setPictureTakenDate(char* pictureTakenDate)
{
	this->pictureTakenDate = (char*)malloc(strlen(pictureTakenDate) + 1);
	strcpy(this->pictureTakenDate, pictureTakenDate);
}


void FileMetadata::setFileSize(int fileSize)
{
	this->fileSize = fileSize;
}


int FileMetadata::getId()
{
	return id;
}


char* FileMetadata::getFileName()
{
	return fileName;
}


char* FileMetadata::getFilePath()
{
	return filePath;
}

char* FileMetadata::getPictureTakenDate()
{
	return pictureTakenDate;
}

int FileMetadata::getFileSize()
{
	return fileSize;
}
