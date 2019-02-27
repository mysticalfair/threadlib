#include "thread.h"
#include <deque>
#include "interrupt.h"
#include <ucontext.h>

typedef struct thread_t {
	unsigned int id;
	char* stack;
	ucontext_t* ucon;
} thread_t;

typedef struct lock_t {
    unsigned int lockid;
    int flag;
    queue<thread_t*> q;
} lock_t;

typedef struct cv_t {
	unsigned int cvid;
  lock_t lock;
	queue<thread_t*> q;
} cv_t;

static bool initialized = false;
static queue<thread_t*> ready;
static ucontext_t* handler;
static thread_t* currentThread;

int thread_libinit(thread_startfunc_t func, void *arg) {
	if(initialized) {
		return -1;
	}
	initialized = true;

	thread_create(func, arg);
  handler = new uncontext_t;
  getcontext(handler);
  swapcontext(handler, current->ucon);
  while (!ready.empty())
  {
    if (running->finished)
    {
      //delete thread
    }
    current = ready.pop();
    swapcontext(handler, current-> ucon);
  }
  cout << "Thread library exiting. \n";
  exit(0);
}

int thread_create(thread_startfunc_t func, void *arg) {
	thread_t *new_thread = new thread_t;
	new_thread->stack = new char [STACK_SIZE];
	new_thread->ucon->uc_stack = new_thread->stack;
	new_thread->ucon->uc_stack = NULL;
	// makecontext(t->ucontext_ptr)

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
