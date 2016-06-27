/*
 *      Copyright (C) 2007-2012 Liviu Ionescu.
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
      OSDeviceCharacter& inDev2, std::ostream& cout);

  // actual thread main code
  virtual void
  threadMain(void);

private:

  // members
  unsigned char m_stack[OSThread::STACK_MINIMAL_SIZE + APP_CFGINT_TASKDBGIN_EXTRA_STACK_SIZE];
  OSDeviceCharacter & m_outDev;
  OSDeviceCharacter & m_inDev2;

  std::ostream& m_cout;

  unsigned char m_buff[100]; // Buffer for multiple bytes reads/writes
};

#endif /* TASKDBGIN_H_ */
