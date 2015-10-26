#ifndef HTTPHANDLER_H
#define HTTPHANDLER_H

#include "SocketHandler.h"
#include "Thread.h"

class HttpHandler: public Thread {
    public:
		HttpHandler(SocketHandler* sh);
        ~HttpHandler();
        
    protected:
        void run();
        void cleanUp();
        
     private:
        SocketHandler* socketHandler;
};

#endif
