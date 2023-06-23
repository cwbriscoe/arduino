#ifndef THREAD_H
#define THREAD_H

#include "env.h"
#include "time.h"

struct threadEntry {
  Trigger* trig;                // keep track of when a thread should execute
  void (*thread)(const Time&);  // function pointer to thread task
  byte priority;                // priority of the thread (1-255, lower number is highest priority)
#ifdef DEBUG
  const char* name;  // in debug mode store name of thread
#endif
};

class ThreadManager {
private:
  byte numThreads = 0;               // number of threads currently added to the manager
  threadEntry* thread[MAX_THREADS];  // array of thread config
  unsigned long prevTime = 0;        // the previous time we polled from micros()

  void insert(threadEntry* te) {  // insert thread at end of array then quick bubble sort
    thread[numThreads] = te;

    for (auto i = numThreads; i > 0; i--) {
      if (thread[i]->priority < thread[i - 1]->priority) {
        auto temp = thread[i - 1];
        thread[i - 1] = thread[i];
        thread[i] = temp;
      } else {
        break;
      }
    }
    numThreads++;
  }

public:
  void add(const char* name, const byte priority, const unsigned long interval, void (*thread)(const Time&), const bool immediate = false) {
    assert(numThreads < MAX_THREADS);

    auto te = new threadEntry;
    te->priority = priority;
    te->trig = new Trigger(interval, immediate);
    te->thread = thread;
#ifdef DEBUG
    te->name = name;
#else
    (void)name;  // prevent compiler warning on unused parameter in the non-debug path
#endif

    insert(te);
  }

  void run() {
    auto execCount = 1;
    unsigned long minNextRun;

    // keep executin the while loop until no threads are ready to run yet
    while (execCount > 0) {
      execCount = 0;
      minNextRun = MAX_TIME;
      for (auto i = 0; i < numThreads; i++) {
        auto currTime = now();
        // check if thread[i] is ready to run or not
        if (thread[i]->trig->triggered(currTime)) {
          // execute the thread
          thread[i]->thread(currTime);
          execCount++;
        } else if (execCount == 0) {
          // in the case where no threads are executed in the while loop, record the minimum next time a thread
          // needs to run to have a good value to delay until.
          auto nextRun = thread[i]->trig->next(currTime);
          if (nextRun < minNextRun) {
            minNextRun = nextRun;
          }
        }
      }
    }
    wait(minNextRun);
  }
};

#endif