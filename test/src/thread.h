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
#ifdef DEBUG
  unsigned long runTime = 0;   // accumulative run time of the scheduler
  unsigned long waitTime = 0;  // accumulative wait (idle) time
  unsigned long prevTime = 0;  // to keep track of time overflow
#endif

  void insert(threadEntry* te) {  // insert thread at end of array then quick bubble sort
    this->thread[this->numThreads] = te;

    for (auto i = this->numThreads; i > 0; i--) {
      if (this->thread[i]->priority < this->thread[i - 1]->priority) {
        auto temp = this->thread[i - 1];
        this->thread[i - 1] = this->thread[i];
        this->thread[i] = temp;
      } else {
        break;
      }
    }
    this->numThreads++;
  }

public:
  void add(const char* name, const byte priority, const unsigned long interval, Task* task, const bool immediate = false) {
    assert(this->numThreads < MAX_THREADS);

    auto te = new threadEntry;
    te->priority = priority;
    te->trig = new Trigger(interval, immediate);
    te->task = task;
#ifdef DEBUG
    te->name = name;
#else
    (void)name;  // prevent compiler warning on unused parameter in the non-debug path
#endif

    this->insert(te);

    // run the init() function of the task
    te->task->setup();
  }

  void run() {
#ifdef DEBUG
    auto start = micros();
    if (start < prevTime) {
      for (auto i = 0; i < this->numThreads; i++) {
        this->thread[i]->task->resetStats();
      }
      this->runTime = 0;
      this->waitTime = 0;
    }
    if ((start & MAX_TIME) < (prevTime & MAX_TIME)) { this->printStats(); }
    prevTime = start;
#endif

    byte execCount;
    unsigned long minNextRun;

    // keep executin the while loop until no threads are ready to run yet
    do {
      execCount = 0;
      minNextRun = MAX_TIME;
      for (auto i = 0; i < numThreads; i++) {
        auto currTime = now();
        // check if thread[i] is ready to run or not
        if (this->thread[i]->trig->triggered(currTime)) {
          // execute the thread
          this->thread[i]->task->exec(currTime);
          execCount++;
        } else if (!execCount) {
          // in the case where no threads are executed in the while loop, record the minimum next time a thread
          // needs to run to have a good value to delay until.
          auto nextRun = this->thread[i]->trig->next(currTime);
          if (nextRun < minNextRun) {
            minNextRun = nextRun;
          }
        }
      }
    } while (execCount);

#ifdef DEBUG
    this->runTime += micros() - start;
    this->waitTime += minNextRun;
#endif

    wait(minNextRun);
  }

#ifdef DEBUG
  void printStats() {
    auto display = [](const char* name, const float pct) {
      print(name);
      print(" ");
      println(pct);
    };

    auto totalTime = this->runTime + this->waitTime;
    unsigned long totalTaskTime = 0;

    for (auto i = 0; i < this->numThreads; i++) {
      auto time = this->thread[i]->task->getRunTime();
      auto pct = float(time) / float(totalTime) * 100.0f;
      display(this->thread[i]->name, pct);
      totalTaskTime += time;
    }

    auto pct = float(this->runTime - totalTaskTime) / float(totalTime) * 100.0f;
    display("manager", pct);

    pct = float(this->waitTime) / float(totalTime) * 100.0f;
    display("idle", pct);
  }
#endif
};

#endif