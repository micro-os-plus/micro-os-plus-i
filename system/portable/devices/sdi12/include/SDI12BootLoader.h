/*
 *      Copyright (C) 2008-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef SDI12BOOTLOADER_H_
#define SDI12BOOTLOADER_H_

#include "portable/kernel/include/OS.h"

#include "portable/devices/sdi12/include/SDI12Sensor.h"

#include "portable/devices/debug/include/OSDebugLed1.h"
#include "portable/devices/debug/include/OSDebugLed2.h"

#include "portable/devices/memory/include/EEPROM.h"
#include "portable/devices/memory/include/Flash.h"

#include <ctype.h>

class SDI12BootLoader : public SDI12Sensor
{
public:
  static void
  init(void);

  const static unsigned short BLOCK_WRITE_GUARD_MILLIS = 2;

  static void
  base64out(unsigned char ch);
  static unsigned char
  base64in(unsigned char ch);
  static unsigned char
  hexIn(unsigned char ch);
  static void
  storeOkUnsigned(unsigned short w);
  static bool
  processCommand(void);
  static void
  processDelayedActions(void);
  static void
  threadMain(void) __attribute__((noreturn));

  static unsigned char ms_doReset;
  static unsigned short ms_addr;
  static unsigned short ms_len;
  static unsigned short ms_first;
  static unsigned short ms_last;
  static bool ms_hasInputCrc;
  static unsigned short ms_CRCin;
  static unsigned short ms_CRCout;
  static unsigned short ms_prevFirst;

  static unsigned char ms_moreCommand;

  static unsigned char ms_bufW[Flash::BLOCK_SIZE + 3];
  static unsigned char ms_bufOut[Flash::BLOCK_SIZE];
  static unsigned char ms_tmp[4];

  static unsigned char ms_base64chars[65];

};

#endif /* SDI12BOOTLOADER_H_ */
