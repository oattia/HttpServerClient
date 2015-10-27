#ifndef THREAD_H
#define THREAD_H

#include <pthread.h>
#include <time.h>

class Thread {

    public:
		explicit Thread();
        
        virtual ~Thread();
        
        bool start();

        bool isDone();

        pthread_t getID();

        time_t getCreateTs();
    
    protected:
        virtual void run() = 0;
        virtual void cleanUp() = 0;
        bool done;
        pthread_t t_id;
        time_t createTs;

    private:
        void runAndClean() {
        	 pthread_cleanup_push(cleanUpHelper, (void*) this);
        	 run();
        	 pthread_cleanup_pop(1);
        }

        static void* startHelper(void* runner) {
        	((Thread*)runner)->runAndClean();
        	return NULL;
        }

        static void cleanUpHelper(void* runner) {
               	((Thread*)runner)->cleanUp();
        }
};

#endif
