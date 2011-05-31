/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef TEST_H_
#define TEST_H_

#include "portable/kernel/include/uOS.h"

class Test : public OSThread
{
public:
  // active object constructor
  Test();

  // actual thread main code
  virtual void
  threadMain(void);

  void testSpi(void);

private:
  // members
  OSStack_t m_stack[(OSThread::STACK_MINIMAL_SIZE + 400) / sizeof(OSStack_t)];

};

#endif /* TEST_H_ */
