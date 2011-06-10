/*
 *      Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef TASKTEST_H_
#define TASKTEST_H_

#include "portable/kernel/include/uOS.h"

#if !defined OS_INCLUDE_SIMPLECLI

#include "portable/devices/character/include/OSDeviceCharacter.h"

class TaskTest : public OSThread
  {
public:
  // active object constructor
  TaskTest(const char *pName, OSDeviceCharacter& dev);

  // actual thread main code
  virtual void threadMain(void);

private:
  // members
  OSStack_t m_stack[ (OSThread::STACK_MINIMAL_SIZE + 400) / sizeof(OSStack_t) ];
  OSDeviceCharacter & m_dev;
  unsigned char m_loopBuff[512];
  int m_countTest;
  };

#endif // OS_INCLUDE_SIMPLECLI

#endif /* TASKCLI_H_ */
