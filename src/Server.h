#ifndef SERVER_H_
#define SERVER_H_

#include <vector>
#include <string>
#include <sstream>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "HttpHandler.h"

#define SERVER_NAME "CLoudServerCom/1.0.0"

using namespace std;

class Server {
	public:
		Server(string workingDirectory, int maxParWorkers, int maxBacklog, unsigned short port, unsigned long connectionTimeOut);
		~Server();
		int init();
		void run();

	private:
		vector<Thread*> threads;
		int socket_fd;
		unsigned int maxParWorkers;
		unsigned int maxBacklog;
		unsigned short port;
		string workingDirectory;
		unsigned long connectionTimeOut;
		bool canCreateNewWorker();
		void printWelcome();
};

#endif
