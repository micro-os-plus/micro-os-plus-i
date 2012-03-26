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
  TaskStress(const char *pName, uint16_t minMicros, uint16_t maxMicros,
      uint16_t minTicks, uint16_t maxTicks);

  // actual thread main code
  virtual void
  threadMain(void);

  static uint16_t
  rand(void);

private:
  // members
  OSStack_t m_stack[(OSThread::STACK_MINIMAL_SIZE
      + APP_CFGINT_TASKSTRESS_EXTRA_STACK_SIZE) / sizeof(OSStack_t)];

  uint16_t m_minMicros;
  uint16_t m_maxMicros;
  uint16_t m_minTicks;
  uint16_t m_maxTicks;

  static uint32_t ms_rand;
};

#endif /* TASKSTRESS_H_ */
