#include "Thread.h"

Thread::Thread(){
	done = false;
	t_id = 0;
}

Thread::~Thread(){
	// TODO
}

bool Thread::start(){
	return (pthread_create(&t_id, NULL, startHelper, (void*) this) == 0);
}

bool Thread::isDone(){
	return done;
}

pthread_t Thread::getID(){
	return t_id;
}
