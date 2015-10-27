#include "HttpHandler.h"

HttpHandler::HttpHandler(SocketHandler* sh, FileSystemHandler* fs,
		string serverName) :
		Thread() {
	this->socketHandler = sh;
	this->fsHandler = fs;
	this->serverName = serverName;
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
			handlePostRequest(requestLineTokens[1], requestLineTokens[2],
					headers);
		} else {
			string empty("");
			handlePostRequest(empty, requestLineTokens[1], headers);
		}
	} else {
		// TODO
	}

}

void HttpHandler::handleGetRequest(string& uri, string& protocol, vector<string>& headers) {

	stringstream httpResponseBuf;

	bool fileExists = fsHandler->exists(uri);

	if (fileExists) {
		httpResponseBuf << protocol << " 200 OK\r\n";
	} else {
		httpResponseBuf << protocol << " 404 Not Found\r\n";
	}

	time_t lastModDate = fsHandler->lastModified(uri);
	char* cLastModDate = ctime(&lastModDate);
	cLastModDate[strlen(cLastModDate) - 1] = '\0'; // remove '\n' from it

	time_t now;
	time(&now);
	char* cNowDate = ctime(&now);
	cNowDate[strlen(cNowDate) - 1] = '\0'; // remove '\n' from it

	string contentType;
	if (strcasecmp(uri.substr(uri.find_last_of(".") + 1).c_str(), "html") == 0) {
		contentType = "text/html";
	} else if (strcasecmp(uri.substr(uri.find_last_of(".") + 1).c_str(), "txt") == 0) {
		contentType = "text/txt";
	} else if (strcasecmp(uri.substr(uri.find_last_of(".") + 1).c_str(), "png") == 0) {
		contentType = "image/png";
	} else if (strcasecmp(uri.substr(uri.find_last_of(".") + 1).c_str(), "jpg") == 0) {
		contentType = "image/jpg";
	}else if (strcasecmp(uri.substr(uri.find_last_of(".") + 1).c_str(), "jpeg") == 0) {
		contentType = "image/jpg";
	} else if (strcasecmp(uri.substr(uri.find_last_of(".") + 1).c_str(), "gif") == 0) {
		contentType = "image/gif";
	} else {
		contentType = "none";
	}

	httpResponseBuf << "Date: " << string(cNowDate) << "\r\n";
	httpResponseBuf << "Server: " << serverName << "\r\n";
	httpResponseBuf << "Last-Modified: " << string(cLastModDate) << "\r\n";
	httpResponseBuf << "Content-Length: " << fsHandler->sizeOfFile(uri) << "\r\n";
	httpResponseBuf << "Content-Type: " << contentType << "\r\n";
	httpResponseBuf << "\r\n";

	if (fileExists) {
		vector<char> dataBytes = fsHandler->ReadAllBytes(uri);
		stringstream dataStream(string(dataBytes.begin(), dataBytes.end()));
		httpResponseBuf << dataStream.str();
	}

	string httpResponse = httpResponseBuf.str();
	socketHandler->write((char*) httpResponse.c_str(), httpResponse.length());
}

void HttpHandler::handlePostRequest(string& uri, string& protocol,
		vector<string>& headers) {

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
