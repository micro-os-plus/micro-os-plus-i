/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef TASKCLI_H_
#define TASKCLI_H_

#include "portable/kernel/include/uOS.h"

class TaskCli : public OSTask
{
public:
  // task constructor
  TaskCli(const char *pName, OSDeviceCharacter& dev, OSDeviceCharacter& devIn);

  // actual task main code
  virtual void
  taskMain(void);

private:
  void
  lineProcess(void);

  int
  xorCheck(unsigned char *pc);

  // members
  unsigned char m_stack[OSTask::STACK_MINIMAL_SIZE + 100];
  OSDeviceCharacter & m_dev;

  istream m_cin;
  ostream m_cout;
  SimpleCli m_cli;

  OSDeviceCharacter& m_devIn;

  unsigned char m_line[80];
};

#endif /* TASKCLI_H_ */
