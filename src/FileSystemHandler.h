#ifndef FILESYSTEMHANDLER_H_
#define FILESYSTEMHANDLER_H_

#include <string>
#include <sstream>
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <vector>
using namespace std;

class FileSystemHandler {
public:
	FileSystemHandler(string workingDirectory);
	virtual ~FileSystemHandler();

	bool exists(string& uri);
	int sizeOfFile(string& uri);
	time_t lastModified(string& uri);
	vector<char> ReadAllBytes(string& uri);

	int write(void* buffer, int len);

private:
	string workingDir;
};

#endif
