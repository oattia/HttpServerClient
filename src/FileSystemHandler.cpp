#include "FileSystemHandler.h"

FileSystemHandler::FileSystemHandler(string workingDirectory) {
	this->workingDir = workingDirectory;
}

int FileSystemHandler::write(void* buffer, int len) {
	return 0;
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

int FileSystemHandler::ReadBytes(string& uri, vector<char>& bytes, int len) {
	stringstream absPath;
	absPath << workingDir << uri;
	ifstream ifs(absPath.str().c_str(), ios::binary);
	ifs.read(&bytes[0], len);
	return bytes.size();
}

FileSystemHandler::~FileSystemHandler() {

}
