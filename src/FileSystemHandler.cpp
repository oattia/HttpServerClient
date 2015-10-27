#include "FileSystemHandler.h"

FileSystemHandler::FileSystemHandler(string workingDirectory) {
	this->workingDir = workingDirectory;
}

int FileSystemHandler::read(void* buffer, int len){
	return 0;
}

int FileSystemHandler::write(void* buffer, int len){
	return 0;
}

FileSystemHandler::~FileSystemHandler() {

}
