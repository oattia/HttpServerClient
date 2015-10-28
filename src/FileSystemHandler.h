#ifndef FILESYSTEMHANDLER_H_
#define FILESYSTEMHANDLER_H_

#include <string>
#include <sstream>
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <vector>
#include <iostream>
using namespace std;

class FileSystemHandler {
public:
	FileSystemHandler(string workingDirectory);
	virtual ~FileSystemHandler();

	bool exists(string& uri);
	int sizeOfFile(string& uri);
	time_t lastModified(string& uri);

	int readBytes(string& uri, void* dataBytes, int len);
	int writeBytes(string& uri, void* dataBytes, int len);

private:
	string workingDir;
};

#endif
