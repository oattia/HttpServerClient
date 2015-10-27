#ifndef SOCKETHANDLER_H
#define SOCKETHANDLER_H

#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>

/**
 * Wrapper around socket systems calls.
 * Functions might block.
 */
class SocketHandler {
public:
	SocketHandler(int socket_fd);
	~SocketHandler();

	int read(void* buffer, int len);
	int write(void* buffer, int len);
	int closeSocket();

private:
	int socket_fd;
};

#endif
