#ifndef THREAD_H
#define THREAD_H

#include "env.h"
#include "wait.h"

struct threadEntry {
  byte priority;                                 // priority of the thread (1-255, lower number is highest priority)
  unsigned long interval;                        // interval betweeen executions
  unsigned long nextTime;                        // next time thread is scheduled to run
  void (*thread)(unsigned long);  // function pointer to thread task
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
  void add(const char* name, const byte priority, const unsigned long interval, void (*thread)(unsigned long), const bool immediate = false) {
    // bail early if we are about to overflow the thread array
    if (numThreads + 1 > MAX_THREADS) {
      return;
    }

    auto te = new threadEntry;
    te->priority = priority;
    te->interval = interval;
    te->thread = thread;
    if (immediate) {
      te->nextTime = micros();
    } else {
      te->nextTime = micros() + interval;
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
    unsigned long minNextRun;
    // keep executin the while loop until no threads are ready to run yet
    while (execCount > 0) {
      execCount = 0;
      minNextRun = MAX_ULONG;
      for (auto i = 0; i < numThreads; i++) {
        // check if thread[i] is ready to run or not
        auto currTime = micros();

        if (thread[i]->nextTime <= currTime) {
          // execute the thread and reschedule the next time it should run
          thread[i]->thread(currTime);
          thread[i]->nextTime += thread[i]->interval;
          execCount++;
        } else {
          // in the case where no threads are executed in the while loop, record the minimum next time a thread
          // needs to run to have a good value to delay until.
          auto nextRun = thread[i]->nextTime - currTime;
          if (nextRun < minNextRun) {
            minNextRun = nextRun;
          }
        }
      }
    }
    //Serial.print("possible sleep time (microseconds): ");
    //Serial.println(minNextRun);
    wait(minNextRun);
  }
};

#endif