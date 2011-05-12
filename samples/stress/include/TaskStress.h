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
  TaskStress(const char *pName, unsigned int minMicros, unsigned int maxMicros, unsigned int minTicks, unsigned int maxTicks);

  // actual thread main code
  virtual void threadMain(void);

  static unsigned int rand(void);

private:
  // members
  OSStack_t m_stack[ (OSThread::STACK_MINIMAL_SIZE + 400) / sizeof(OSStack_t) ];

  unsigned int m_minMicros;
  unsigned int m_maxMicros;
  unsigned int m_minTicks;
  unsigned int m_maxTicks;

  static unsigned int ms_rand;
  };

#endif /* TASKSTRESS_H_ */
