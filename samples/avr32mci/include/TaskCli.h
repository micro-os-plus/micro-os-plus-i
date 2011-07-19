/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef TASKCLI_H_
#define TASKCLI_H_

#include "portable/kernel/include/uOS.h"

#include "hal/arch/avr32/uc3/devices/block/include/MemoryCardMci.h"

class TaskCli : public OSThread
  {
public:
  // active object constructor
  TaskCli(const char* pName, OSDeviceCharacter& dev, avr32::uc3::MemoryCardMci& card);

  // actual thread main code
  virtual void threadMain(void);

private:
  void lineProcess(void);

  void cmdMI(void);
  void cmdMC(void);
  void cmdST(void);
  void cmdRI(void);
  void cmdMR(unsigned long l);
  void cmdX(unsigned short v);

  // members
  OSStack_t m_stack[OSThread::STACK_MINIMAL_SIZE + 150];
  OSDeviceCharacter& m_dev;

  std::istream m_cin;
  std::ostream m_cout;
  SimpleCli m_cli;
  avr32::uc3::MemoryCardMci& m_card;

  unsigned char m_line[80];

  bool bInit;

  unsigned int m_buf[512/4];
  };

#endif /* TASKCLI_H_ */
