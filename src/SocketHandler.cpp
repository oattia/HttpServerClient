#include "SocketHandler.h"

SocketHandler::SocketHandler(int socket_fd) {
	this->socket_fd = socket_fd;
}

SocketHandler::~SocketHandler() {
	closeSocket();
}

int SocketHandler::read(void* buffer, int len) {
	return recv(socket_fd, buffer, len, 0);
}

int SocketHandler::readExactSize(void* buffer, int len) {
	return recv(socket_fd, buffer, len, MSG_WAITALL);
}

int SocketHandler::write(void* buffer, int len) {
	return send(socket_fd, buffer, len, 0);
}

int SocketHandler::writeExactSize(void* buffer, int len) {
	char *ptr = (char*) buffer;
	int length = len;

	while (length > 0) {
		int i = send(socket_fd, ptr, length, 0);
		if (i < 0)
			return -1;
		if (i == 0)
			break;
		ptr += i;
		length -= i;
	}
	return len;
}

int SocketHandler::closeSocket() {
	return close(socket_fd);
}

