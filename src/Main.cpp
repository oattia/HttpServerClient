#include <iostream>
#include "Server.h"

using namespace std;

#define MAX_PAR_WORKERS		10
#define MAX_BACKLOG			10
#define PORT				8080
#define WORKING_DIRECTORY   "."
#define CONNECTION_TIME_OUT 20

int main(int argc, char* argv[]) {
	Server* server = new Server(WORKING_DIRECTORY, MAX_PAR_WORKERS, MAX_BACKLOG, PORT, CONNECTION_TIME_OUT);
	int initStatus = server->init();
	if(initStatus == 0) {
		server->run();	// Does not return.
	} else {
		return initStatus;
	}
}
