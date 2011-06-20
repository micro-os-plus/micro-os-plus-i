/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef TASKCLI_H_
#define TASKCLI_H_

#include "portable/kernel/include/uOS.h"

#include "portable/devices/character/include/OSDeviceCharacter.h"
#include "portable/misc/include/SimpleCli.h"

class TaskCli : public OSThread
  {
public:
  // active object constructor
  TaskCli(const char *pName, OSDeviceCharacter& dev);

  // actual thread main code
  virtual void threadMain(void);

private:
  void lineProcess(void);

  int xorCheck(unsigned char *pc);

  // members
  OSStack_t m_stack[ (OSThread::STACK_MINIMAL_SIZE + 400) / sizeof(OSStack_t) ];
  OSDeviceCharacter & m_dev;

  std::istream m_cin;
  std::ostream m_cout;

  SimpleCli m_cli;
  unsigned char m_line[ 80 ];

#if OS_TEST_PHASE == 2
  unsigned char m_loopBuff[512];
  int m_countTest;
#endif
  };

#endif /* TASKCLI_H_ */
