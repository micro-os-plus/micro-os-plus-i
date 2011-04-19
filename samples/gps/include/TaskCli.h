/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef TASKCLI_H_
#define TASKCLI_H_

#include "portable/kernel/include/uOS.h"

//#include "portable/devices/character/include/OSDeviceCharacter.h"
//#include "portable/misc/include/SimpleCli.h"

class TaskCli : public OSTask
{
public:
  // task constructor
  TaskCli(const char *pName, OSDeviceCharacter& dev);

  // actual task main code
  virtual void
  taskMain(void);

private:
  void
  processLine(void);

  // members
  OSStack_t m_stack[(OSTask::STACK_MINIMAL_SIZE + 400) / sizeof(OSStack_t)];
  OSDeviceCharacter & m_dev;

  istream m_cin;
  ostream m_cout;

  SimpleCli m_cli;
  Parser m_parser;

  unsigned char m_line[80]; // input line
  unsigned char m_token[20]; // parsed token
};

#endif /* TASKCLI_H_ */
