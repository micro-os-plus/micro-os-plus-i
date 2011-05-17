/*
 *      Copyright (C) 2008-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef DEVICERTC4574_H_
#define DEVICERTC4574_H_

#include "portable/kernel/include/OS.h"

#include "DeviceRTC4574Select.h"

class DeviceRTC4574 : public DeviceRTC4574Select
{
public:
  static void
  init(void);

  static unsigned char
  readBytes(unsigned char addr, unsigned char* pBuf, unsigned char len);
  static void
  writeBytes(unsigned char addr, unsigned char* pBuf, unsigned char len);

private:
  static void
  clkInit(void);
  static void
  clkHigh(void);
  static void
  clkLow(void);

  static void
  dataIn(void);
  static void
  dataOut(void);
  static void
  dataHigh(void);
  static void
  dataLow(void);
  static bool
  dataIsLow(void);

  static void
  fconInit(void);
  static void
  fconHigh(void);
  static void
  fconLow(void);

  static void
  delay(void);
  static unsigned char
  readByte(void);
  static void
  writeByte(unsigned char c);

};

#if defined(OS_CONFIG_BOARD_AVI_A07524)
#include "hal/boards/AVI/a07524/include/DeviceRTC4574_Defines.h"
#else
#error "Missing OS_CONFIG_BOARD_* definition"
#endif

#if defined(OS_CONFIG_ARCH_AVR8)  
#include "hal/arch/avr8/devices/misc/include/DeviceRTC4574_Inlines.h"
#else
#error "Missing OS_CONFIG_ARCH_* definition"
#endif

#endif /* DEVICERTC4574_H_ */
