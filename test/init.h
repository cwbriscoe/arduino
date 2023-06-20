#ifndef INIT_H
#define INIT_H

#include "env.h"
#include "thread.h"
#include "tasks.h"

ThreadManager threadManager;

void initialize() {
  // initialize the thread manager and worker threads
  threadManager.add("blink", 1, 200, task1);
}

#endif