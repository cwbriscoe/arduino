#ifndef THREAD_H
#define THREAD_H

#include "env.h"

struct threadEntry {
  byte priority;                                 // priority of the thread (1-255, lower number is highest priority)
  word interval;                                 // interval betweeen executions
  unsigned long lastTime;                        // last time thread was started
  void (*thread)(unsigned long, unsigned long);  // function pointer to thread task
#ifdef DEBUG
  const char* name;  // in debug mode store name of thread
#endif
};

class ThreadManager {
private:
  byte numThreads = 0;               // number of threads currently added to the manager
  threadEntry* thread[MAX_THREADS];  // array of thread config

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
  void add(const char* name, byte priority, word interval, void (*thread)(unsigned long, unsigned long)) {
    // bail early if we are about to overflow the thread array
    if (numThreads + 1 > MAX_THREADS) {
      return;
    }
    
    auto te = new threadEntry;
    auto currTime = millis();
    te->priority = priority;
    te->interval = interval;
    te->thread = thread;
    if (interval > currTime) {
      te->lastTime = currTime - interval;
    } else {
      te->lastTime = 0;
    }
#ifdef DEBUG
    te->name = name;
#else
    (void)name;  // prevent compiler warning on unused parameter in the non-debug path
#endif
    insert(te);
  }

  void run() {
    auto execCount = 1;
    while (execCount > 0) {
      execCount = 0;
      for (auto i = 0; i < numThreads; i++) {
        auto currTime = millis();
        auto nextRun = thread[i]->lastTime + thread[i]->interval;
        if (nextRun <= currTime) {
          thread[i]->thread(currTime, thread[i]->lastTime);
          thread[i]->lastTime += thread[i]->interval;
          execCount++;
        }
      }
    }
    delay(1);
  }
};

#endif