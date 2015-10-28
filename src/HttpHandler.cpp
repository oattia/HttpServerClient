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
	for (vector<string>::iterator it = tokens.begin() + 1; it != tokens.end();
			it++) {
		headers.push_back(*it);
	}

	vector<string> requestLineTokens;
	tokenize(requestLine, requestLineTokens, " ");

	assert(requestLineTokens.size() == 2 || requestLineTokens.size() == 3);

	if (strcasecmp(requestLineTokens[0].c_str(), "GET") == 0) {
		if (requestLineTokens.size() == 3) {
			handleGetRequest(requestLineTokens[1], requestLineTokens[2],
					headers);
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
	int fileSize = fsHandler->sizeOfFile(uri);

	httpResponseBuf << "HTTP/1.0 " << (fileExists ? "200 OK" : "404 Not Found") << "\r\n";

	time_t now;
	time(&now);
	httpResponseBuf << "Date: " << getTimeAsString(now) << "\r\n";
	httpResponseBuf << "Server: " << serverName << "\r\n";
	httpResponseBuf << "Last-Modified: " << getTimeAsString(fsHandler->lastModified(uri)) << "\r\n";
	httpResponseBuf << "Content-Length: " << fileSize << "\r\n";
	httpResponseBuf << "Content-Type: " << getContentType(uri) << "\r\n";
	httpResponseBuf << "\r\n";

	if (fileExists) {
		char* dataBytes = new char[fileSize];
		fsHandler->readBytes(uri, dataBytes, fileSize);
		httpResponseBuf.write(dataBytes, fileSize);
		delete dataBytes;
	}

	string httpResponse = httpResponseBuf.str();
	socketHandler->write((char*) httpResponse.c_str(), httpResponse.length());
}

string HttpHandler::getContentType(string& uri) {
	string contentType;
	if (strcasecmp(uri.substr(uri.find_last_of(".") + 1).c_str(), "html")
			== 0) {
		contentType = "text/html";
	} else if (strcasecmp(uri.substr(uri.find_last_of(".") + 1).c_str(), "txt")
			== 0) {
		contentType = "text/txt";
	} else if (strcasecmp(uri.substr(uri.find_last_of(".") + 1).c_str(), "png")
			== 0) {
		contentType = "image/png";
	} else if (strcasecmp(uri.substr(uri.find_last_of(".") + 1).c_str(), "jpg")
			== 0) {
		contentType = "image/jpg";
	} else if (strcasecmp(uri.substr(uri.find_last_of(".") + 1).c_str(), "jpeg")
			== 0) {
		contentType = "image/jpg";
	} else if (strcasecmp(uri.substr(uri.find_last_of(".") + 1).c_str(), "gif")
			== 0) {
		contentType = "image/gif";
	} else {
		contentType = "none/none";
	}
	return contentType;

}

string HttpHandler::getTimeAsString(time_t t) {
	char* cDate = ctime(&t);
	cDate[strlen(cDate) - 1] = '\0'; // remove '\n' from it
	return string(cDate);
}

void HttpHandler::handlePostRequest(string& uri, string& protocol,
		vector<string>& headers) {
	int contentLength = 0;
	for (vector<string>::iterator it = headers.begin(); it != headers.end(); it++) {
		vector<string> splitted;
		tokenize(*it, splitted, ": ");
		cout << *it << endl;
		if (splitted[0] == "Content-Length") {
			contentLength = atoi(splitted[1].c_str());
		}
		splitted.clear();
	}

	cout << contentLength << endl;

	if (contentLength <= 0) {
		string badResonse = "HTTP/1.0 400 Bad Request\r\n";
		socketHandler->write((char*) badResonse.c_str(), badResonse.length());
		return;
	}

	char* dataBuffer = new char[contentLength];
	memset(dataBuffer, 0, contentLength);

	string okResonse = "HTTP/1.0 200 OK\r\n";
	socketHandler->write((char*) okResonse.c_str(), okResonse.length());

	socketHandler->read(dataBuffer, contentLength);

	fsHandler->writeBytes(uri, dataBuffer, contentLength);

	delete dataBuffer;
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
