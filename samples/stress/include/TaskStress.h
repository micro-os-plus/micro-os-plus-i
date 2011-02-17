/*
 * TaskStress.h
 *
 *  Created on: Feb 17, 2011
 *      Author: alex
 */

#ifndef TASKSTRESS_H_
#define TASKSTRESS_H_

#include "portable/kernel/include/uOS.h"
#include <stdlib.h>

class TaskStress : public OSTask
  {
public:
  // task constructor
  TaskStress(const char *pName, unsigned int minMicros, unsigned int maxMicros, unsigned int minTicks, unsigned int maxTicks);

  // actual task main code
  virtual void taskMain(void);

private:
  // members
  OSStack_t m_stack[ (OSTask::STACK_MINIMAL_SIZE + 400) / sizeof(OSStack_t) ];

  unsigned int m_minMicros;
  unsigned int m_maxMicros;
  unsigned int m_minTicks;
  unsigned int m_maxTicks;
  };

#endif /* TASKSTRESS_H_ */
