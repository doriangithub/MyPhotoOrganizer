#pragma once
class FileMetadata
{
public:
	FileMetadata();
	~FileMetadata();

	void setId(int id);
	void setFileName(char *fileName);
	void setFilePath(char *filePath);
	void setPictureTakenDate(char* pictureTakenDate);
	void setFileSize(int fileSize);

	int getId();
	char* getFileName();
	char* getFilePath();
	char* getPictureTakenDate();
	int getFileSize();

private:
	int id;
	char* fileName;
	char *filePath;
	char* pictureTakenDate;
	int fileSize;
};

