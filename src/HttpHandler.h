#ifndef HTTPHANDLER_H
#define HTTPHANDLER_H

#include "SocketHandler.h"
#include "FileSystemHandler.h"

#include "Thread.h"
#include <vector>
#include <string>
#include <string.h>
#include <sstream>
#include <iostream>
#include <assert.h>
#include <algorithm>
#include <ctime>

using namespace std;

#define MAX_HTTP_REQ_LENGTH 8192

class HttpHandler: public Thread {
    public:
		HttpHandler(SocketHandler* sh, FileSystemHandler* fsHandler, string serverName);
        ~HttpHandler();
        
    protected:
        void run();
        void cleanUp();
        
     private:
        void handleRequest(string& request);
        void handleGetRequest(string& uri, string& protocol, vector<string>& headers);
        void handlePostRequest(string& uri, string& protocol, vector<string>& headers);

        template < class ContainerT >
        void tokenize(const string& str, ContainerT& tokens, const string& delimiters, bool trimEmpty = true);

        SocketHandler* socketHandler;
        FileSystemHandler* fsHandler;
        string serverName;
};

#endif
