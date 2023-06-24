// Copyright 2023 Christopher Briscoe.  All rights reserved.

#ifndef THREAD_H
#define THREAD_H

#include "env.h"
#include "task.h"

struct threadEntry {
  Trigger* trig;  // keep track of when a thread should execute
  Task* task;     // pointer to the task
  byte priority;  // priority of the thread (1-255, lower number is highest priority)
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
  void add(const char* name, const byte priority, const unsigned long interval, Task* task, const bool immediate = false) {
    assert(numThreads < MAX_THREADS);

    auto te = new threadEntry;
    te->priority = priority;
    te->trig = new Trigger(interval, immediate);
    te->task = task;
#ifdef DEBUG
    te->name = name;
#else
    (void)name;  // prevent compiler warning on unused parameter in the non-debug path
#endif

    insert(te);

    // run the init() function of the task
    task->setup();
  }

  void run() {
    byte execCount;
    unsigned long minNextRun;

    // keep executin the while loop until no threads are ready to run yet
    do {
      execCount = 0;
      minNextRun = MAX_TIME;
      for (auto i = 0; i < numThreads; i++) {
        auto currTime = now();
        // check if thread[i] is ready to run or not
        if (thread[i]->trig->triggered(currTime)) {
          // execute the thread
          thread[i]->task->exec(currTime);
          execCount++;
        } else if (!execCount) {
          // in the case where no threads are executed in the while loop, record the minimum next time a thread
          // needs to run to have a good value to delay until.
          auto nextRun = thread[i]->trig->next(currTime);
          if (nextRun < minNextRun) {
            minNextRun = nextRun;
          }
        }
      }
    } while (execCount);
    wait(minNextRun);
  }
};

#endif