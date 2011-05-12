/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/uOS.h"

#include "TaskBlink.h"
#include "TaskReport.h"
#include "TaskStress.h"

// ----------------------------------------------------------------------------

/*
 * This project is intended to stress test the mutex synchronisation mechanism.
 *
 * The test method uses 10 instances of the stress task, all competing for the
 * same mutex. When each one acquires the mutex, a task counter is incremented.
 *
 * A separate reporting task outputs on the debug interface the current
 * counters.
 *
 * There are two implementations of the mutex, the extended one with a local
 * round-robin notification mechanism, but more expensive, and one using
 * the scheduler mechanism to notify all tasks, that favours tasks by
 * registration order.
 *
 * The extended version should produce a more or less uniform distribution
 * of the counters. The simple one will favour first registered tasks.
 *
 * For applications with lots of active clients waiting for the same mutex,
 * it is recommended to use the extended version.
 *
 * For usual applications with only two clients for a mutex, the simple
 * version is good enough.
 *
 * To activate the extended version, use OS_INCLUDE_OSMUTEX_WAITING_TASKS.
 *
 * For this test, an application definition can switch between standard
 * and extended behaviours: APP_INCLUDE_OSMUTEX_WAITING_TASKS
 */

// ---------------------------------------------------------------------------

// the mutex used to protect the shared variables

#if defined(OS_INCLUDE_OSMUTEX_WAITING_TASKS)
  OSThread* waitingTasksArray[APP_CONFIG_RESOURCES_ARRAY_SIZE];
#if defined(APP_INCLUDE_OSMUTEX_WAITING_TASKS)
  OSMutex mutex(waitingTasksArray, sizeof(waitingTasksArray)/sizeof(waitingTasksArray[0]));
#else
  OSMutex mutex;
#endif
#else
  OSMutex mutex;
#endif /* defined(OS_INCLUDE_OSMUTEX_WAITING_TASKS) */

// the blink task, used to show we are alive
TaskBlink taskB("B", APP_CONFIG_LED1, APP_CFGINT_TASKBLINK_TICKS);

// the reporting task, used to output statistics
TaskReport taskR("R", APP_CFGINT_TASKREPORT_SECONDS);

// the stress tasks, which will attempt to access the protected region
TaskStress task0("0", 0);
TaskStress task1("1", 1);
TaskStress task2("2", 2);
TaskStress task3("3", 3);
TaskStress task4("4", 4);
TaskStress task5("5", 5);
TaskStress task6("6", 6);
TaskStress task7("7", 7);
TaskStress task8("8", 8);
TaskStress task9("9", 9);

// ----------------------------------------------------------------------------
// global variables shared by all tasks and protected by the mutex

// the global counter
int resourceValue;

// global array where each tasks increments its own counter
// it is used for debugging and live statistics
int resourceAccessNum[APP_CONFIG_RESOURCES_ARRAY_SIZE];

// ----------------------------------------------------------------------------

