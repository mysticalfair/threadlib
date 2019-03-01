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
    thread_t* owner;
    queue<thread_t*> q;
} lock_t;

typedef struct cv_t {
  	lock_t lock;
	queue<thread_t*> q;
} cv_t;

static bool initialized = false;
static queue<thread_t*> ready;
static thread_t* current_thread;
static int num_threads = 0;
static map<unsigned int, lock_t*> lock_map;
static map<unsigned int, cv_t*> cv_map;

static int execute_function(thread_startfunc_t func, void *arg) {
	func(arg);
	if(!ready.empty()) {
		thread_t* next_thread = ready.front();
		ready.pop();
		swapcontext(current_thread->ucon, next_thread->ucon);
	}
	return 0;
}

int thread_libinit(thread_startfunc_t func, void *arg) {
	if(initialized) {
		return -1;
	}
	initialized = true;

	thread_create(func, arg);
	thread_t* initial_thread = ready.front();
	ready.pop();

	setcontext(initial_thread->ucon);

	current_thread = initial_thread;
	execute_function(func, arg);

	return 0;
}

int thread_create(thread_startfunc_t func, void *arg) {
	thread_t *new_thread = new thread_t;
	new_thread->stack = new char [STACK_SIZE];
	new_thread->ucon = new ucontext_t;
	getcontext(new_thread->ucon);
	new_thread->ucon->uc_stack = new_thread->stack;
	new_thread->ucon->uc_stack = NULL;
	makecontext(new_thread->ucon, (void (*)()) execute_function, 2, func, arg);
	num_threads++;
	new_thread->id = num_threads;
	return 0;
}

int thread_yield(void) {
	ready.push(current_thread);
	thread_t* next_thread = ready.front();
	ready.pop();
	swapcontext(current_thread->ucon, next_thread->ucon);
	current_thread = next_thread;
	return 0;
}

int thread_lock(unsigned int lock) {
	interrupt_disable();
	map<unsigned int, lock_t*>::const_iterator temp_it = lock_map.find(lock);
	if(temp_it == lock_map.end()) {
		lock_t* new_lock = new lock_t;
		new_lock->owner = current_thread;
		//new_lock->flag = 1;
		new_lock->q = new queue<thread_t*>
		lock_map.insert(temp_it, std::pair<unsigned int, lock_t>(lock, new_lock));
	}
	else {
		if(lock_map.at(lock)->owner != NULL) {
			lock_map.at(lock)->q.push(current_thread);
			thread_yield();
		}
		else {
			lock_map.at(lock)->owner = current_thread;
		}
	}
	interrupt_enable();
	return 0;
}

int thread_unlock(unsigned int lock) {
  interrupt_disable();
  map<unsigned int, lock_t*>::const_iterator temp_it = lock_map.find(lock);
  if(temp_it == lock_map.end()) {
    return -1;
  }
  else{
    lock_t foundLock = temp_it->second;
    if (foundLock.owner = NULL)
      return -1;
    if (foundLock.owner->id == current.id)
        foundLock.owner = NULL;
    else{
      return -1;
    }

  }
  }
  interrupt_enable();
	return 0;
}

int thread_wait(unsigned int lock, unsigned int cond) {
  //unlocks, sleeps, wake up with lock
  if (thread_unlock(lock))
  {
    map<unsigned int, cv_t*>::const_iterator temp_it = cv_map.find(cond);
    if(temp_it == cv_map.end()) {
  		cv_t* new_cv = new cv_t;
      new_cv->q = new queue<thread_t*>;
      new_cv->lock = lock;
      new_cv->q.push(current_thread);
      cv_map.insert(temp_it, std::pair<unsigned int, cv_t>(cond, new_cv));

    }
    else{
      cv_t* foundCV = temp_it->second;
      foundCV->q.push(current_thread);
    }
    thread_t* temp = ready.front(); // if front is empty will throw error
    ready.pop()
    swapcontext(current->ucon, temp->ucon)
    current_thread = temp_thread;
    return thread_lock(lock);
  }
	return -1;
}

int thread_signal(unsigned int lock, unsigned int cond) {
  map<unsigned int, cv_t*>::const_iterator temp_it = cv_map.find(cond);
  if(temp_it != cv_map.end())
  {
    cv_t* foundCV = temp_it->second;
    queue<thread_t*> temp = foundCV->q;
    while(!temp.empty())
    {
      ready.push(temp.front());
      temp.pop();
      break;
    }
    return 0;
  }
	return -1;
}

int thread_broadcast(unsigned int lock, unsigned int cond) {

  map<unsigned int, cv_t*>::const_iterator temp_it = cv_map.find(cond);
  if(temp_it != cv_map.end())
  {
    cv_t* foundCV = temp_it->second;
    queue<thread_t*> temp = foundCV->q;
    while(!temp.empty())
    {
      ready.push(temp.front());
      temp.pop();
    }
    return 0;
  }
  return -1;
}
