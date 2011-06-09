/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef OSDEVICEDEBUG_H_
#define OSDEVICEDEBUG_H_

#include "portable/kernel/include/OS_Defines.h"

#if defined(DEBUG) && defined(OS_INCLUDE_OSDEVICEDEBUG_STREAMBUF)
#include "portable/stdlib/include/streambuf"
#endif

// The parent class of all debug classes used for displaying trace messages.
class OSDeviceDebug
#if defined(DEBUG) && defined(OS_INCLUDE_OSDEVICEDEBUG_STREAMBUF)
: public std::streambuf
#endif

{
public:

#if defined(DEBUG) && !defined(OS_EXCLUDE_MULTITASKING)
  // Initialise the internal variables.
  OSDeviceDebug();
#endif
  // Output, on debug interface,  one character.
  static void
  putChar(unsigned char c);
  // Output, on debug interface, CR+LF.
  static void
  putNewLine(void);
  // Output, on debug interface, a string.
  static void
  putString(const char* pc);

  static void
  putConstructor(const char* pc, const void* p);
  static void
  putConstructorWithIndex(const char* pc, unsigned short i, const void* p);
  static void
  putDestructor(const char* pc, const void* p);

#if defined(OS_CONFIG_ARCH_AVR8)
  // Output, on debug interface, the bytes at pc address,
  // until the first zero byte.
  static void putString_P(const char* PROGMEM pc);

  static void putConstructor_P(const char* PROGMEM pc, const void* p);
  static void
  putConstructorWithIndex_P(const char* pc, unsigned short i, const void* p);
  static void putDestructor_P(const char* PROGMEM pc, const void* p);

#endif
  // Output, on debug interface, the hex value of a byte;
  // i.e. 2 hexadecimal characters.
  static void
  putHex(unsigned char c);
  // Output, on debug interface, the hex value of a 2-byte word;
  // i.e. 4 hexadecimal characters.
  static void
  putHex(unsigned short w);
#if defined(DEBUG) && defined(OS_INCLUDE_OSDEVICEDEBUG_PUTHEX_LONG)
  // Output, on debug interface, the hex value of a long;
  // i.e. 8 hexadecimal characters.
  static void
  putHex(unsigned long l);
#endif
#if defined(DEBUG) && defined(OS_INCLUDE_OSDEVICEDEBUG_PUTHEX_INT)
  // Output, on debug interface, the hex value of an integer;
  // i.e. 8 hexadecimal characters.
  static void
  putHex(unsigned int l);
#endif
  // Output, on debug interface, the decimal value of a 2-byte word,
  // <n> last digits will not be output, max 5 decimal digits.
  static void
  putDec(unsigned short w, unsigned short n = 0);
#if defined(DEBUG) && defined(OS_INCLUDE_OSDEVICEDEBUG_PUTDEC_LONG)
  // Output, on debug interface, the decimal value of a short
  // <n> last digits will not be output, max 10 decimal digits.
  static void putDec(unsigned long l, unsigned short n = 0);
#endif
  // Output, on debug interface, the address p, in hexadecimal.
  static void
  putPtr(const void* p);
  // Output, on debug interface, the address p from program memory, in hexadecimal.
  static void
  putPC(const char* PROGMEM pc);
  // Output, on debug interface, function name, file name,
  // line number and expression string.
  // It is used to in case of a failed assertion.
  static void
  __assert(const char* func, const char* file, int lineno, const char* sexp);

#if defined(DEBUG)
  // Watchdog reset; implementation specific.
  static void
  implWDReset(void);
#endif
  // Initialise debug interface and print greeting and build strings.
  static void
  earlyInit();

private:

#if defined(DEBUG)
#if defined(OS_INCLUDE_NAKED_INIT)
  // Initialise the debug interface.
  static void nakedEarlyInit() __attribute__((naked, section(".init5")));
#endif
  // Send one byte to debug interface.
  static int
  commonPutByte(unsigned char c);
  // Send the given buffer to debug interface.
  static int
  commonPutBytes(const char* pc, unsigned int n);

  // implementation code, defined in separated files
  // Initialise debug interface.
  static void
  implEarlyInit(void);
  // Ouput one byte on the debug interface.
  static void
  implPutByte(unsigned char c);
  // Ouput the given buffer on the debug interface.
  static int
  implPutBytes(const char* s, unsigned int n);
  // Return true if the debug interface can output now.
  static bool
  implCanTransmit(void);
  // Return true if the debug interface is connected with the MCU.
  static bool
  implDevicePresent(void);
#endif

#if defined(DEBUG) && defined(OS_INCLUDE_OSDEVICEDEBUG_STREAMBUF)
  // streambuf definitions
  // Output, on debug interface, one character.
  virtual int
  overflow(int c = std::traits::eof());
  // Output, on debug interface, a buffer.
  virtual std::streamsize
  xsputn(const char* s, std::streamsize n);
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

#if !defined(OS_DEBUG_CONSTRUCTORS)

inline void
OSDeviceDebug::putConstructor(const char* pc __attribute__((unused)), const void* p __attribute__((unused)))
  {
  }

inline void
OSDeviceDebug::putConstructorWithIndex(const char* pc __attribute__((unused)), unsigned short i __attribute__((unused)), const void* p __attribute__((unused)))
  {
  }

inline void
OSDeviceDebug::putDestructor(const char* pc __attribute__((unused)), const void* p __attribute__((unused)))
  {
  }

#endif /* !defined(OS_DEBUG_CONSTRUCTORS) */

#else /* !DEBUG */

// define empty functions for Release
inline void
OSDeviceDebug::putChar(unsigned char __attribute__((unused)) c)
{
}

inline void
OSDeviceDebug::putNewLine(void)
{
}

inline void
OSDeviceDebug::putString(const char __attribute__((unused)) * pc)
{
}

inline void
OSDeviceDebug::putConstructor(const char* pc __attribute__((unused)), const void* p __attribute__((unused)))
{
}

inline void
OSDeviceDebug::putConstructorWithIndex(const char* pc __attribute__((unused)), unsigned short i __attribute__((unused)),
    const void* p  __attribute__((unused)))
{
}

inline void
OSDeviceDebug::putDestructor(const char* pc __attribute__((unused)), const void* p __attribute__((unused)))
{
}

#if defined(OS_INCLUDE_SEPARATE_PROGMEM)

inline void OSDeviceDebug::putString_P(const char* __attribute__((unused)) PROGMEM pc)
  {
  }

inline void
OSDeviceDebug::putConstructor_P(const char* __attribute__((unused)) PROGMEM pc, const void* __attribute__((unused)) p)
  {
  }

inline void
OSDeviceDebug::putConstructorWithIndex_P(const char* __attribute__((unused)) PROGMEM pc, unsigned short i __attribute__((unused)), const void* __attribute__((unused)) p)
  {
  }

inline void
OSDeviceDebug::putDestructor_P(const char* __attribute__((unused)) PROGMEM pc, const void* __attribute__((unused)) p)
  {
  }

#endif

inline void
OSDeviceDebug::putHex(unsigned char __attribute__((unused)) c)
{
}

inline void
OSDeviceDebug::putHex(unsigned short __attribute__((unused)) w)
{
}

inline void
OSDeviceDebug::putDec(unsigned short __attribute__((unused)) w,
    unsigned short __attribute__((unused)) n)
{
}

#if defined(DEBUG) && defined(OS_INCLUDE_OSDEVICEDEBUG_PUTDEC_LONG)
inline void OSDeviceDebug::putDec(unsigned long __attribute__((unused)) l, unsigned short
    __attribute__((unused)) n)
  {
  }
#endif

inline void
OSDeviceDebug::putPC(const char* PROGMEM __attribute__((unused)) pc)
{
  pc = pc;
}

inline void
OSDeviceDebug::__assert(const char __attribute__((unused)) *func,
    const char __attribute__((unused)) *xfile,
    int __attribute__((unused)) lineno,
    const char __attribute__((unused)) *sexp)
{
}

#endif

#endif /* OSDEVICEDEBUG_H_ */
