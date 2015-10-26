#include "HttpHandler.h"


HttpHandler::HttpHandler(SocketHandler* sh): Thread() {
    this->socketHandler = sh;
}

HttpHandler::~HttpHandler() {
	// TODO
}

void HttpHandler::run() {

}

void HttpHandler::cleanUp(){
	done = true;
}
