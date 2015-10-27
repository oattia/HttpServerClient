#include "HttpHandler.h"

HttpHandler::HttpHandler(SocketHandler* sh, FileSystemHandler* fs) : Thread() {
	this->socketHandler = sh;
	this->fsHandler = fs;
}

HttpHandler::~HttpHandler() {
	delete socketHandler;
}

void HttpHandler::run() {

	char clientMsg[MAX_HTTP_REQ_LENGTH];
	memset(clientMsg, '\0', MAX_HTTP_REQ_LENGTH);

	int readSize = socketHandler->read(clientMsg, MAX_HTTP_REQ_LENGTH);

	if (readSize == 0) {
		printf("Client disconnected\n");
	} else if (readSize == -1) {
		perror("reading from socket failed\n");
	} else {
		string request(clientMsg);
		handleRequest(request);
	}
}

void HttpHandler::handleRequest(string& request) {
	vector<string> tokens;
	tokenize(request, tokens, "\r\n");
	string requestLine = tokens[0];

	vector<string> headers;
	copy(tokens.begin() + 1, tokens.end(), headers.begin());

	vector<string> requestLineTokens;
	tokenize(request, requestLineTokens, " ");

	assert(requestLineTokens.size() == 2 || requestLineTokens.size() == 3);

	if (strcasecmp(requestLineTokens[0].c_str(), "GET") == 0) {
		if (requestLineTokens.size() == 3) {
			handleGetRequest(requestLineTokens[1], requestLineTokens[2], headers);
		} else {
			string empty("");
			handleGetRequest(empty, requestLineTokens[1], headers);
		}
	} else if (strcasecmp(requestLineTokens[0].c_str(), "POST") == 0) {
		if (requestLineTokens.size() == 3) {
			handlePostRequest(requestLineTokens[1], requestLineTokens[2], headers);
		} else {
			string empty("");
			handlePostRequest(empty, requestLineTokens[1], headers);
		}
	} else {
		// TODO
	}

}

void HttpHandler::handleGetRequest(string& requested, string& protocol, vector<string>& headers) {
	socketHandler->write();
}

void HttpHandler::handlePostRequest(string& requested, string& protocol, vector<string>& headers) {

}

template<class ContainerT>
void HttpHandler::tokenize(const string& str, ContainerT& tokens,
		const string& delimiters, bool trimEmpty) {
	string::size_type pos, lastPos = 0;

	using value_type = typename ContainerT::value_type;
	using size_type = typename ContainerT::size_type;

	while (true) {
		pos = str.find_first_of(delimiters, lastPos);
		if (pos == string::npos) {
			pos = str.length();

			if (pos != lastPos || !trimEmpty)
				tokens.push_back(
						value_type(str.data() + lastPos,
								(size_type) pos - lastPos));

			break;
		} else {
			if (pos != lastPos || !trimEmpty)
				tokens.push_back(
						value_type(str.data() + lastPos,
								(size_type) pos - lastPos));
		}

		lastPos = pos + 1;
	}
}

void HttpHandler::cleanUp() {
	socketHandler->closeSocket();
	done = true;
}
