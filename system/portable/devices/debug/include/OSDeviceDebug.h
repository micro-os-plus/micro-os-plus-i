/*
 *	Copyright (C) 2007-2008 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef OSDEVICEDEBUG_H_
#define OSDEVICEDEBUG_H_

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_OSDEVICEDEBUG_STREAMBUF)
#include "portable/stdlib/include/streambuf"
#endif

class OSDeviceDebug
#if defined(OS_INCLUDE_OSDEVICEDEBUG_STREAMBUF)
: public streambuf
#endif

  {
public:

#if defined(DEBUG) && !defined(OS_EXCLUDE_MULTITASKING)
  OSDeviceDebug();
#endif

  static void putChar(unsigned char c);
  static void putNewLine(void);
  static void putString(const char *pc);
#if defined(OS_CONFIG_ARCH_AVR8)
  static void putString_P(const char * PROGMEM pc);
#endif
  static void putHex(unsigned char c);
  static void putHex(unsigned short w);
#if defined(DEBUG) && defined(OS_INCLUDE_OSDEVICEDEBUG_PUTHEX_LONG)
  static void putHex(unsigned long l);
#endif
#if defined(DEBUG) && defined(OS_INCLUDE_OSDEVICEDEBUG_PUTHEX_INT)
  static void putHex(unsigned int l);
#endif
  static void putDec(unsigned short w, unsigned short n = 0);
#if defined(DEBUG) && defined(OS_INCLUDE_OSDEVICEDEBUG_PUTDEC_LONG)
  static void putDec(unsigned long l, unsigned short n = 0);
#endif

  static void putPtr(const void *p);
  static void putPC(const char * PROGMEM pc);

  static void __assert(const char *func, const char *file, int lineno,
      const char *sexp);

#if defined(DEBUG)
  static void implWDReset(void);
#endif

  static void earlyInit();

private:

#if defined(DEBUG)
#if defined(OS_INCLUDE_NAKED_INIT)
  static void nakedEarlyInit() __attribute__( ( naked, section( ".init5" ) ) );
#endif

  static int commonPutByte(unsigned char c);
  static int commonPutBytes(const char *pc, unsigned int n);

  // implementation code, defined in separated files
  static void implEarlyInit(void);
  static void implPutByte(unsigned char c);
  static int implPutBytes(const char *s, unsigned int n);
  static bool implCanTransmit(void);
  static bool implDevicePresent(void);
#endif

#if defined(DEBUG) && defined(OS_INCLUDE_OSDEVICEDEBUG_STREAMBUF)
  // streambuf definitions
  virtual int overflow(int c = traits::eof());
  virtual streamsize xsputn(const char* s, streamsize n);
#endif

  };

#if defined(DEBUG)

#if defined(OS_CONFIG_DEBUG_DEVICE_I2C)
#include "portable/devices/debug/include/DeviceDebugI2C_Inlines.h"
#elif defined(OS_CONFIG_DEBUG_DEVICE_USART)
#include "portable/devices/debug/include/DeviceDebugUsart_Inlines.h"
#else
#error "Missing OS_CONFIG_DEBUG_DEVICE_* definition"
#endif

#else /* !DEBUG */

// define empty functions for Release
inline void OSDeviceDebug::putChar(unsigned char __attribute__( ( unused ) ) c)
  {
  }

inline void OSDeviceDebug::putNewLine(void)
  {
  }

inline void OSDeviceDebug::putString(const char __attribute__( ( unused ) ) * pc)
  {
  }

#if defined(OS_INCLUDE_SEPARATE_PROGMEM)
inline void OSDeviceDebug::putString_P(const char __attribute__( ( unused) ) * PROGMEM pc)
  {
  }
#endif

inline void OSDeviceDebug::putHex(unsigned char __attribute__( ( unused ) ) c)
  {
  }

inline void OSDeviceDebug::putHex(unsigned short __attribute__( ( unused ) ) w)
  {
  }

inline void OSDeviceDebug::putDec(unsigned short __attribute__( ( unused ) ) w, unsigned short
    __attribute__( ( unused ) ) n)
  {
  }

#if defined(DEBUG) && defined(OS_INCLUDE_OSDEVICEDEBUG_PUTDEC_LONG)
inline void OSDeviceDebug::putDec(unsigned long __attribute__( ( unused ) ) l, unsigned short
    __attribute__( ( unused ) ) n)
  {
  }
#endif

inline void OSDeviceDebug::putPC(const char * PROGMEM __attribute__((unused)) pc)
  {
    pc = pc;
  }

inline void OSDeviceDebug::__assert(const char __attribute__((unused)) *func, const char __attribute__((unused)) *xfile,
    int __attribute__((unused)) lineno, const char __attribute__((unused)) *sexp)
  {
  }

#endif

#endif /*OSDEVICEDEBUG_H_*/
