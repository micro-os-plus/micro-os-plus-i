/*
 *      Copyright (C) 2008 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef FLASH_H_
#define FLASH_H_

#include "portable/kernel/include/OS.h"

typedef unsigned short FlashAddress_t;

class Flash
  {

#if defined(OS_CONFIG_ARCH_AVR)
#include "hal/arch/avr/devices/memory/include/Flash_Constants.h"
#endif

public:
  static unsigned char getUninitializedValue(void);
  static unsigned char readByte(FlashAddress_t addr);

  static void pageErase(FlashAddress_t addr);
  static void pageEraseBusyWait(void);

  static void pageFillWord(FlashAddress_t addr, unsigned short w);
  static void pageWrite(FlashAddress_t addr);
  static void pageWriteBusyWait(void);

  static void enableRead(void);

  // implementation methods
  static unsigned char implReadByte(FlashAddress_t addr);

  static void implPageErase(FlashAddress_t addr);
  static void implPageEraseBusyWait(void);

  static void implPageFillWord(FlashAddress_t addr, unsigned short w);
  static void implPageWrite(FlashAddress_t addr);
  static void implPageWriteBusyWait(void);

  static void implEnableRead(void);

  };

#if defined(OS_CONFIG_ARCH_AVR)
#include "hal/arch/avr/devices/memory/include/Flash_Inlines.h"
#endif

#if defined(OS_EXCLUDE_MULTITASKING)

inline unsigned char Flash::readByte(unsigned short offset)
  {
    return implReadByte(offset);
  }

inline void Flash::pageErase(FlashAddress_t addr)
  {
    implPageErase(addr);
  }

inline void Flash::pageEraseBusyWait(void)
  {
    implPageEraseBusyWait();
  }

inline void Flash::pageFillWord(FlashAddress_t addr, unsigned short w)
  {
    implPageFillWord(addr, w);
  }

inline void Flash::pageWrite(FlashAddress_t addr)
  {
    implPageWrite(addr);
  }

inline void Flash::pageWriteBusyWait(void)
  {
    implPageWriteBusyWait();
  }

inline void Flash::enableRead(void)
  {
    implEnableRead();
  }

#endif

#endif /*FLASH_H_*/
