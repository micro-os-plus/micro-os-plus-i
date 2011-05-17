/*
 *      Copyright (C) 2008-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef CANLEDS_H_
#define CANLEDS_H_

#include "portable/kernel/include/OS.h"

class CANLed
  {
public:
  CANLed();

  static const unsigned char STATUS_OFF = 0;
  static const unsigned char STATUS_FLICKERING = 1;
  static const unsigned char STATUS_BLINKING = 2;
  static const unsigned char STATUS_SINGLE_FLASH = 3;
  static const unsigned char STATUS_DOUBLE_FLASH = 4;
  static const unsigned char STATUS_TRIPLE_FLASH = 5;
  static const unsigned char STATUS_ON = 6;

  static const unsigned char STATUS_UNPOWERED = STATUS_OFF;

  void on(void);
  void off(void);
  void init(void);

  unsigned char getStatus(void);
  void setStatus(unsigned char status);

  unsigned char m_crtStatus;
  unsigned char m_status;
  unsigned char m_count;
  };

class CANLedGreen : public CANLed
  {
public:
  CANLedGreen();

  static const unsigned char STATUS_AUTOBAUD = STATUS_FLICKERING;
  static const unsigned char STATUS_PREOPERATIONAL = STATUS_BLINKING;
  static const unsigned char STATUS_STOPPED = STATUS_SINGLE_FLASH;
  static const unsigned char STATUS_OPERATIONAL = STATUS_ON;

  void on(void);
  void off(void);
  void init(void);

  void update(bool last);

private:

  };

class CANLedRed : public CANLed
  {
public:
  CANLedRed();

  static const unsigned char STATUS_AUTOBAUD = STATUS_FLICKERING;
  static const unsigned char STATUS_WARNING = STATUS_SINGLE_FLASH;
  static const unsigned char STATUS_ERROR_EVENT = STATUS_DOUBLE_FLASH;
  static const unsigned char STATUS_SYNC_ERROR = STATUS_TRIPLE_FLASH;
  static const unsigned char STATUS_BUS_OFF = STATUS_ON;

  void on(void);
  void off(void);
  void init(void);

  void update(bool last);

private:

  };

class CANLeds : public OSThread
  {
public:
  CANLeds(const char* pName);

  // actual thread main code
  virtual void threadMain(void);

  CANLedGreen green;
  CANLedRed red;

private:
  // members
  unsigned char m_stack[OSThread::STACK_MINIMAL_SIZE + 150];

  unsigned char m_semiperiods;

  };

#if defined(OS_CONFIG_BOARD_AVI_A07524)
#include "hal/boards/AVI/a07524/include/CANLeds_Defines.h"
#elif defined(OS_CONFIG_BOARD_AVI_A07527)
#include "hal/boards/AVI/a07527/include/CANLeds_Defines.h"
#elif defined(OS_CONFIG_BOARD_AVI_A07521)
#include "hal/boards/AVI/a07521/include/CANLeds_Defines.h"
#else
#error "Missing OS_CONFIG_BOARD_* definition"
#endif

#if defined(OS_CONFIG_ARCH_AVR8)  
#include "hal/arch/avr8/tasks/include/CANLeds_Inlines.h"
#else
#error "Missing OS_CONFIG_ARCH_* definition"
#endif

#endif /* TASKCANLEDS_H_ */
