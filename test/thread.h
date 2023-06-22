#ifndef THREAD_H
#define THREAD_H

#include "env.h"
#include "time.h"

struct threadEntry {
  byte priority;                  // priority of the thread (1-255, lower number is highest priority)
  unsigned long interval;         // interval betweeen executions
  unsigned long nextTime;         // next time thread is scheduled to run
  void (*thread)(unsigned long);  // function pointer to thread task
  bool overflowed;                // keep track when the next scheduled iteration time of the task overflowed
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
  void add(const char* name, const byte priority, const unsigned long interval, void (*thread)(unsigned long), const bool immediate = false) {
    // bail early if we are about to overflow the thread array
    if (numThreads + 1 > MAX_THREADS) {
      return;
    }

    auto te = new threadEntry;
    te->priority = priority;
    te->interval = interval;
    te->thread = thread;
    te->nextTime = getTime();
    if (!immediate) {
      te->nextTime += interval;
    }
    te->overflowed = false;
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
        // reset thread overflow flags if the standard timer has finally overflowed
        auto currTime = getTime();
        if (currTime < prevTime) {
          //Serial.println("currTime overflowed");
          for (auto j = 0; j < numThreads; j++) {
            thread[j]->overflowed = false;
            // this should be edge case but if any threads hasn't rolled over before currTime,
            // we need to reset their nextTime to zero or they won't run again for a long time
            if (thread[j]->nextTime > MAX_TIME / 2) {  // (MAX_TIME / 2) implies (in non-debug mode) that all threads have to
              thread[j]->nextTime = 0;                 // to have a duration less than ~30 minutes or bad things may happen
            }
          }
        }
        prevTime = currTime;

        // check if thread[i] is ready to run or not
        if (!thread[i]->overflowed && thread[i]->nextTime <= currTime) {
          // execute the thread
          thread[i]->thread(currTime);

          // calculate next schedule time, taking in account of possible overflow
          auto diff = MAX_TIME - thread[i]->nextTime;
          if (diff < thread[i]->interval) {
            thread[i]->nextTime = thread[i]->interval - diff;
            thread[i]->overflowed = true;
            //Serial.print("thread ");
            //Serial.print(i);
            //Serial.println(" overflowed");
          } else {
            thread[i]->nextTime += thread[i]->interval;
          }

          execCount++;
        } else {
          // in the case where no threads are executed in the while loop, record the minimum next time a thread
          // needs to run to have a good value to delay until.
          unsigned long nextRun;
          if (thread[i]->overflowed) {
            nextRun = MAX_TIME - currTime + thread[i]->nextTime;
          } else {
            nextRun = thread[i]->nextTime - currTime;
          }
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