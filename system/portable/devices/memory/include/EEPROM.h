/*
 *      Copyright (C) 2007-2008 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef EEPROM_H_
#define EEPROM_H_

#include "portable/kernel/include/OS.h"

class EEPROM
  {
public:
  static unsigned char getUninitializedValue(void);
  static void busyWait(void);

  static unsigned char readByte(unsigned short offset);
  static unsigned short readWord(unsigned short offset);
  static float readFloat(unsigned short offset);
  static unsigned char * readString(unsigned short offset, unsigned char *pStr,
      unsigned short len);

  static void writeByte(unsigned short offset, unsigned char b);
  static void writeWord(unsigned short offset, unsigned short w);
  static void writeFloat(unsigned short offset, float f);
  static void writeString(unsigned short offset, const unsigned char *pStr,
      unsigned short len);

  // --- implementation methods
  static unsigned char implReadByte(unsigned short offset);
  static unsigned short implReadWord(unsigned short offset);
  static void implReadBlock(unsigned short offset, void *pointer_ram, size_t n);

  static void implWriteByte(unsigned short offset, unsigned char b);
  static void implWriteWord(unsigned short offset, unsigned short w);
  static void
      implWriteBlock(unsigned short offset, void *pointer_ram, size_t n);

  };

#if defined(OS_CONFIG_ARCH_AVR)
#include "hal/arch/avr/devices/memory/include/EEPROM_Inlines.h"
#else
#error "Missing OS_CONFIG_ARCH_* definition"
#endif

#if defined(OS_EXCLUDE_MULTITASKING)
inline unsigned char EEPROM::readByte(unsigned short offset)
  {
    return implReadByte(offset);
  }

inline void EEPROM::writeByte(unsigned short offset, unsigned char b)
  {
    implWriteByte(offset, b);
  }
#endif

#endif /*EEPROM_H_*/
