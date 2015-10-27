#include "Thread.h"

Thread::Thread() {
	done = false;
	t_id = 0;
	createTs = 0L;
}

Thread::~Thread() {
	// TODO
}

bool Thread::start() {
	time(&createTs);
	return (pthread_create(&t_id, NULL, startHelper, (void*) this) == 0);
}

bool Thread::isDone() {
	return done;
}

pthread_t Thread::getID() {
	return t_id;
}

time_t Thread::getCreateTs() {
	return createTs;
}
