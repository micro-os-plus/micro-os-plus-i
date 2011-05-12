/*
 *      Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef TASKDBGIN_H_
#define TASKDBGIN_H_

#include "portable/kernel/include/uOS.h"

class TaskDbgIn : public OSThread
{
public:
  // active object constructor
  TaskDbgIn(const char *pName, OSDeviceCharacter& outDev,
      OSDeviceCharacter& inDev2);

  // actual thread main code
  virtual void
  threadMain(void);

private:

  // members
  unsigned char m_stack[OSThread::STACK_MINIMAL_SIZE + 100];
  OSDeviceCharacter & m_outDev;
  OSDeviceCharacter & m_inDev2;
};

#endif /* TASKDBGIN_H_ */
