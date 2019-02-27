#include "thread.h"
#include <deque>
#include "interrupt.h"
#include <ucontext.h>

typedef struct thread {
	unsigned int id;
	char* stack
} thread_t;

typedef struct lock_t {
    unsigned int lockid;
    unsigned int CV;
}

int thread_create(thread_startfunc_t func, void *arg) {
	return 0;
}

int thread_yield(void) {
	return 0;
}

int thread_lock(unsigned int lock) {
	return 0;
}

int thread_unlock(unsigned int lock) {
	return 0;
}

int thread_wait(unsigned int lock, unsigned int cond) {
	return 0;
}

int thread_signal(unsigned int lock, unsigned int cond) {
	return 0;
}

int thread_broadcast(unsigned int lock, unsigned int cond) {
	return 0;
}
