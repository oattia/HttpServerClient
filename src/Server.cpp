#include "Server.h"

Server::Server(string workingDirectory, int maxParWorkers, int maxBacklog,
		unsigned short port, unsigned long connectionTimeOut) {
	this->workingDirectory = workingDirectory;
	this->maxParWorkers = maxParWorkers;
	this->maxBacklog = maxBacklog;
	this->port = port;
	socket_fd = 0;
	this->connectionTimeOut = connectionTimeOut;
}

Server::~Server() {

}

int Server::init() {
	chdir(workingDirectory.c_str());
	struct addrinfo hints;
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	int rv;
	struct addrinfo* servinfo;
	stringstream ss;
	ss << port;
	if ((rv = getaddrinfo(NULL, ss.str().c_str(), &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	struct addrinfo *p;
	int yes = 1;
	for (p = servinfo; p != NULL; p = p->ai_next) {
		if ((socket_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol))
				== -1) {
			perror("server: socket");
			continue;
		}

		if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int))
				== -1) {
			perror("setsockopt");
			exit(1);
		}

		if (bind(socket_fd, p->ai_addr, p->ai_addrlen) == -1) {
			close(socket_fd);
			perror("server: bind");
			continue;
		}
		break;
	}

	if (p == NULL) {
		fprintf(stderr, "server: failed to bind\n");
		return 2;
	}

	freeaddrinfo(servinfo);

	if (listen(socket_fd, maxBacklog) == -1) {
		perror("listen");
		exit(1);
	}
	printWelcome();
	return 0;
}

void Server::printWelcome() {
	printf(
			"\n\n\n ██████╗██╗      ██████╗ ██╗   ██╗██████╗       ███████╗███████╗██████╗ ██╗   ██╗███████╗██████╗        ██████╗ ██████╗ ███╗   ███╗\n");
	printf(
			"██╔════╝██║     ██╔═══██╗██║   ██║██╔══██╗      ██╔════╝██╔════╝██╔══██╗██║   ██║██╔════╝██╔══██╗      ██╔════╝██╔═══██╗████╗ ████║\n");
	printf(
			"██║     ██║     ██║   ██║██║   ██║██║  ██║█████╗███████╗█████╗  ██████╔╝██║   ██║█████╗  ██████╔╝█████╗██║     ██║   ██║██╔████╔██║\n");
	printf(
			"██║     ██║     ██║   ██║██║   ██║██║  ██║╚════╝╚════██║██╔══╝  ██╔══██╗╚██╗ ██╔╝██╔══╝  ██╔══██╗╚════╝██║     ██║   ██║██║╚██╔╝██║\n");
	printf(
			"╚██████╗███████╗╚██████╔╝╚██████╔╝██████╔╝      ███████║███████╗██║  ██║ ╚████╔╝ ███████╗██║  ██║      ╚██████╗╚██████╔╝██║ ╚═╝ ██║\n");
	printf(
			"╚═════╝╚══════╝ ╚═════╝  ╚═════╝ ╚═════╝       ╚══════╝╚══════╝╚═╝  ╚═╝  ╚═══╝  ╚══════╝╚═╝  ╚═╝       ╚═════╝ ╚═════╝ ╚═╝     ╚═╝\n");
	printf("\n\n*************************************************************************\n");
	printf("|       Welcome to Cloud-Server-Com HTTP server v1.0.0                  |\n");
	printf("|       The server is now ready to accept connections ...               |\n");
	printf("*************************************************************************\n\n");
}

bool Server::canCreateNewWorker() {
	time_t now;
	time(&now);
	for (vector<Thread*>::iterator it = threads.begin(); it != threads.end();) {
		Thread* t = *it;
		if (t->isDone()
				|| difftime(now, t->getCreateTs()) > connectionTimeOut) {
			it = threads.erase(it);
			delete t;
		} else {
			it++;
		}
	}
	return threads.size() < maxParWorkers;
}

void Server::run() {
	while (1) {

		while (!canCreateNewWorker()) {
			sleep(1);
		}

		struct sockaddr_storage their_addr;
		socklen_t sin_size = sizeof(sockaddr_storage);
		int incoming_socket_fd = accept(socket_fd,
				(struct sockaddr *) &their_addr, &sin_size);

		if (incoming_socket_fd == -1) {
			perror("accept");
			continue;
		}

		Thread* worker = new HttpHandler(new SocketHandler(incoming_socket_fd),
				new FileSystemHandler(workingDirectory), SERVER_NAME);

		if (worker->start()) {
			threads.push_back(worker);
		} else {
			delete worker;
		}
	}
}

