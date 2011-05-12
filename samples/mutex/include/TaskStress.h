/*
 * TaskStress.h
 *
 *  Created on: Feb 17, 2011
 *      Author: alex
 */

#ifndef TASKSTRESS_H_
#define TASKSTRESS_H_

#include "portable/kernel/include/uOS.h"

class TaskStress : public OSThread
  {
public:
  // active object constructor
  // m_taskId is the numeric id of the task, used as index in resourceAccessNum
  TaskStress(const char *pName, int m_threadId);

  // actual thread main code
  virtual void threadMain(void);

  // returns a pseudo-random generated unsigned int number
  static unsigned int rand(void);

private:
  // members
  OSStack_t m_stack[ (OSThread::STACK_MINIMAL_SIZE + 400) / sizeof(OSStack_t) ];

  // the numeric id of the thread, used as index in resourceAccessNum
  int m_threadId;

  // the random seed
  static unsigned int ms_rand;

  static const unsigned int m_maxSleepTicks = 500;
  static const unsigned int m_minSleepTicks = 2;

  static const unsigned int m_maxActivityTicks = 200;
  };

#endif /* TASKSTRESS_H_ */
