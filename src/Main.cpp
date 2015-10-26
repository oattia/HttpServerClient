#include <iostream>
#include "Server.h"

using namespace std;

#define MAX_PAR_WORKERS		10
#define MAX_BACKLOG			10
#define PORT				3490

int main(int argc, char* argv[]) {
	Server* server = new Server(MAX_PAR_WORKERS, MAX_BACKLOG, PORT);
	int initStatus = server->init();
	if(initStatus == 0) {
		server->run();
	} else {
		return initStatus;
	}
}
