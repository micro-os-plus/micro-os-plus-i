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
  TaskCli(const char *pName, OSDeviceCharacter& dev, DeviceMemCard & card);

  // actual thread main code
  virtual void threadMain(void);

private:
  void lineProcess(void);

  void cmdMI(void);
  void cmdMC(void);
  void cmdST(void);
  void cmdRI(void);
  void cmdRD(unsigned long l, unsigned short v);
  void cmdX(unsigned short v);

  int xorCheck(unsigned char *pc);

  int searchLast(unsigned long max, unsigned long *plast, unsigned long *pid);
  int mmc_read_blkid(unsigned char *pbuf, unsigned long blk,
      unsigned long *pid, unsigned long *pbeg);

  // members
  unsigned char m_stack[OSThread::STACK_MINIMAL_SIZE + 150];
  OSDeviceCharacter & m_dev;

  istream m_cin;
  ostream m_cout;
  SimpleCli m_cli;
  DeviceMemCard & m_card;

  unsigned char m_line[ 80 ];

  bool bInit;

  unsigned char m_buf[ 512 ];
  };

#endif /*TASKCLI_H_*/
