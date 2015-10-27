#ifndef FILESYSTEMHANDLER_H_
#define FILESYSTEMHANDLER_H_

#include <string>
using namespace std;

class FileSystemHandler {
public:
	FileSystemHandler(string workingDirectory);
	virtual ~FileSystemHandler();

	int read(void* buffer, int len);
	int write(void* buffer, int len);

private:
	string workingDir;
};

#endif
