/*
 *      Copyright (C) 2007-2008 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef TASKDBGIN_H_
#define TASKDBGIN_H_

#include "portable/kernel/include/uOS.h"

class TaskDbgIn : public OSTask
  {
public:
  // task constructor
  TaskDbgIn(const char *pName, OSDeviceCharacter& outDev,
      OSDeviceCharacter& inDev1, OSDeviceCharacter& inDev2);

  // actual task main code
  virtual void taskMain(void);

private:

  // members
  unsigned char m_stack[OSTask::STACK_MINIMAL_SIZE + 100];
  OSDeviceCharacter & m_outDev;
  OSDeviceCharacter & m_inDev1;
  OSDeviceCharacter & m_inDev2;
  };

#endif /*TASKDBGIN_H_*/
