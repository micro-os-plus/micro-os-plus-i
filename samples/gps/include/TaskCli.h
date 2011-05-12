/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
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
  TaskCli(const char *pName, OSDeviceCharacter& dev);

  // actual thread main code
  virtual void
  threadMain(void);

  OSReturn_t
  commandShowThreads(void);
  OSReturn_t
  commandShowStacks(void);
  OSReturn_t
  commandShowPosition(void);

  OSReturn_t
  commandTest(void);

private:
  void
  processLine(void);

  OSReturn_t
  recurse(cliToken_t* p);

  // members
  OSStack_t m_stack[(OSThread::STACK_MINIMAL_SIZE + 400) / sizeof(OSStack_t)];
  OSDeviceCharacter & m_dev;

  istream m_cin;
  ostream m_cout;

  CommandLineInterface m_cli;
  Parser m_parser;

  unsigned char m_line[80]; // input line
  unsigned char m_token[20]; // parsed token
};

#endif /* TASKCLI_H_ */
