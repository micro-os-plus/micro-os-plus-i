/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef OSTHREADIDLE_H_
#define OSTHREADIDLE_H_

#include "portable/kernel/include/OS.h"

#include "portable/kernel/include/OSThread.h"

#if !defined(OS_CFGINT_IDLE_THREAD_EXTRA_STACK_SIZE)
#define OS_CFGINT_IDLE_THREAD_EXTRA_STACK_SIZE OSThread::STACK_MINIMAL_SIZE
#endif // !defined(OS_CFGINT_IDLE_THREAD_EXTRA_STACK_SIZE)

#if !defined(OS_CFGINT_IDLE_THREAD_STACK_SIZE)
#define OS_CFGINT_IDLE_THREAD_STACK_SIZE  (OSThread::STACK_MINIMAL_SIZE+OS_CFGINT_IDLE_THREAD_EXTRA_STACK_SIZE)
#endif // !defined(OS_CFGINT_IDLE_THREAD_STACK_SIZE)

class OSThreadIdle : public OSThread
{
public:
  OSThreadIdle();

  // Main function thread.
  // If this function run, it means that no other thread needs the MCU.
  // It will try to put the MCU to sleep. After that (when the MCU is waked up)
  // a yield is made to give the control to other thread, if there is another
  // thread which in the meantime needs the MCU.
  virtual void
  threadMain(void);

#if defined(OS_INCLUDE_OSTHREAD_SLEEP)
  // Check if the MCU can be put at sleep.
  // If the sleep is allowed put the MCU to sleep.
  virtual bool enterSleep(void);
#endif

private:
  // members

  // Stack used by this thread.
  OSStack_t m_stack[OS_CFGINT_IDLE_THREAD_STACK_SIZE / sizeof(OSStack_t)];
};

#endif /* OSTHREADIDLE_H_ */
