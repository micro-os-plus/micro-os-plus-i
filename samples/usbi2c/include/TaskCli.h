/*
 *	Copyright (C) 2007-2012 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef TASKCLI_H_
#define TASKCLI_H_

#include "portable/kernel/include/uOS.h"

class TaskCli : public OSThread
{
public:
  // active object constructor
  TaskCli(const char *pName, OSDeviceCharacter& dev, OSDeviceCharacter& devIn,
      std::istream& cin, std::ostream& cout);

  // actual thread main code
  virtual void
  threadMain(void);

private:
  void
  lineProcess(void);

  int
  xorCheck(unsigned char *pc);

  // members
  unsigned char m_stack[OSThread::STACK_MINIMAL_SIZE + 100];
  OSDeviceCharacter& m_dev;

  std::istream& m_cin;
  std::ostream& m_cout;
  SimpleCli m_cli;

  OSDeviceCharacter& m_devIn;

  unsigned char m_line[80];
};

#endif /* TASKCLI_H_ */
