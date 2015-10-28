#include "FileSystemHandler.h"

FileSystemHandler::FileSystemHandler(string workingDirectory) {
	this->workingDir = workingDirectory;
}

bool FileSystemHandler::exists(string& uri) {
	stringstream absPath;
	absPath << workingDir << uri;
	ifstream f(absPath.str().c_str());
	bool good = f.good();
	f.close();
	struct stat attr;
	stat(absPath.str().c_str(), &attr);
	return good && (attr.st_mode & S_IFREG);
}

int FileSystemHandler::sizeOfFile(string& uri) {
	stringstream absPath;
	absPath << workingDir << uri;
	ifstream f(absPath.str().c_str());
	f.seekg(0, f.end);
	int length = f.tellg();
	f.close();
	return length;
}

time_t FileSystemHandler::lastModified(string& uri) {
	stringstream absPath;
	absPath << workingDir << uri;
	struct stat attr;
	stat(absPath.str().c_str(), &attr);
	return attr.st_mtim.tv_sec;
}

int FileSystemHandler::readBytes(string& uri, void* dataBytes, int len) {
	stringstream absPath;
	absPath << workingDir << uri;
	FILE* fp = fopen(absPath.str().c_str(), "rb");
	int actuallyRead = fread(dataBytes, 1, len, fp);
	fclose(fp);
	return actuallyRead;
}

int FileSystemHandler::writeBytes(string& uri, void* dataBytes, int len) {
	stringstream absPath;
	absPath << workingDir << uri;
	FILE* fp = fopen(absPath.str().c_str(), "wb+");
	int actuallyRead = fwrite(dataBytes, 1, len, fp);
	fclose(fp);
	return actuallyRead;
}

FileSystemHandler::~FileSystemHandler() {

}
