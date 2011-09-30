/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef OSDEVICEDEBUG_H_
#define OSDEVICEDEBUG_H_

#include "portable/kernel/include/OS.h"

#if defined(DEBUG) && defined(OS_INCLUDE_OSDEVICEDEBUG_STREAMBUF)
#include "portable/stdlib/include/streambuf"
#endif /* defined(DEBUG) && defined(OS_INCLUDE_OSDEVICEDEBUG_STREAMBUF) */

#if defined(OS_CONFIG_DEBUG_DEVICE_I2C)
#include "portable/devices/debug/include/DeviceDebugI2cImpl.h"
#endif /* defined(OS_CONFIG_DEBUG_DEVICE_I2C) */

#undef assert

// ============================================================================

// This class is used for displaying the trace messages.
// The actual device output is passed to an implementation helper.

class OSDeviceDebug

#if defined(DEBUG)
:

#if defined(OS_CONFIG_DEBUG_DEVICE_I2C)
public DeviceDebugI2cImpl
#elif defined(OS_CONFIG_DEBUG_DEVICE_USART)
public OSDeviceDebugUsartImpl
#else
#error "Missing OS_CONFIG_DEBUG_DEVICE_* definition"
#endif

#if defined(OS_INCLUDE_OSDEVICEDEBUG_STREAMBUF)
, public std::streambuf
#endif
#endif

{
public:

#if defined(DEBUG) && !defined(OS_EXCLUDE_MULTITASKING)
  // Initialise the internal variables.
  OSDeviceDebug();
#endif
  // Output, on debug interface,  one character.
  static void
  putChar(uchar_t c);
  // Output, on debug interface, CR+LF.
  static void
  putNewLine(void);
  // Output, on debug interface, a string.
  static void
  putString(const char* pc);
  static void
  putString(uchar_t* pc);

  static void
  putConstructor(const char* pc, const void* p);
  static void
  putConstructorWithIndex(const char* pc, uint16_t i, const void* p);
  static void
  putDestructor(const char* pc, const void* p);

#if defined(OS_CONFIG_ARCH_AVR8)

  // Output, on debug interface, the bytes at pc address,
  // until the first zero byte.
  static void
  putString_P(const char* PROGMEM pc);
  static void
  putString_P(uchar_t* PROGMEM pc);

  static void
  putConstructor_P(const char* PROGMEM pc, const void* p);
  static void
  putConstructorWithIndex_P(const char* pc, uint16_t i, const void* p);
  static void
  putDestructor_P(const char* PROGMEM pc, const void* p);

#endif
  // Output, on debug interface, the hex value of a byte;
  // i.e. 2 hexadecimal characters.
  static void
  putHex(uint8_t c);

  // Output, on debug interface, the hex value of a 2-byte word;
  // i.e. 4 hexadecimal characters.
  static void
  putHex(uint16_t w);

  // Output, on debug interface, the hex value of a long;
  // i.e. 8 hexadecimal characters.
  static void
  putHex(uint32_t l);

#if !defined(OS_CONFIG_ARCH_AVR8)
  // Output, on debug interface, the hex value of an integer;
  // i.e. 8 hexadecimal characters.
  static void
  putHex(uint_t i);
#endif

  // Output, on debug interface, the decimal value of a 1-byte,
  // <n> last digits will not be output, max 3 decimal digits.
  static void
  putDec(uint8_t b, uint16_t n = 0);

  // Output, on debug interface, the decimal value of a 2-byte word,
  // <n> last digits will not be output, max 5 decimal digits.
  static void
  putDec(uint16_t w, uint16_t n = 0);

  // Output, on debug interface, the decimal value of a short
  // <n> last digits will not be output, max 10 decimal digits.
  static void
  putDec(uint32_t l, uint16_t n = 0);

#if !defined(OS_CONFIG_ARCH_AVR8)
  // Output, on debug interface, the decimal value of an int
  // <n> last digits will not be output, max 10 decimal digits.
  static void
  putDec(uint_t i, uint16_t n = 0);
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
  assert(const char* func, const char* file, int lineno, const char* sexp);

  // Initialise debug interface and print greeting and build strings.
  static void
  earlyInit();

  static OSStack_t
  criticalEnter(void);

  static void
  criticalExit(OSStack_t mask);

private:

#if defined(DEBUG)
#if defined(OS_INCLUDE_NAKED_INIT)
  // Initialise the debug interface.
  static void
  nakedEarlyInit() __attribute__((naked, section(".init5")));
#endif
  // Send one byte to debug interface.
  static int
  commonPutByte(uchar_t c);
  // Send the given buffer to debug interface.
  static int
  commonPutBytes(const char* pc, unsigned int n);

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

// ----------------------------------------------------------------------------

inline OSStack_t
OSDeviceDebug::criticalEnter(void)
  {
    register OSStack_t tmp;

    tmp = OSCPUImpl::getInterruptsMask();
    OSCPUImpl::interruptsDisable();

    return tmp;
  }

inline void
OSDeviceDebug::criticalExit(OSStack_t mask)
  {
    OSCPUImpl::setInterruptsMask(mask);
  }

#if !defined(OS_DEBUG_CONSTRUCTORS)

inline void
OSDeviceDebug::putConstructor(const char* pc __attribute__((unused)), const void* p __attribute__((unused)))
  {
  }

inline void
OSDeviceDebug::putConstructorWithIndex(const char* pc __attribute__((unused)), uint16_t i __attribute__((unused)),
    const void* p __attribute__((unused)))
  {
  }

inline void
OSDeviceDebug::putDestructor(const char* pc __attribute__((unused)), const void* p __attribute__((unused)))
  {
  }

#if defined(OS_INCLUDE_SEPARATE_PROGMEM)

inline void
OSDeviceDebug::putConstructor_P(const char* PROGMEM pc __attribute__((unused)), const void* p __attribute__((unused)))
  {
  }

inline void
OSDeviceDebug::putConstructorWithIndex_P(const char* pc __attribute__((unused)), uint16_t i __attribute__((unused)),
    const void* p __attribute__((unused)))
  {
  }

inline void
OSDeviceDebug::putDestructor_P(const char* PROGMEM pc __attribute__((unused)), const void* p __attribute__((unused)))
  {
  }

#endif /* defined(OS_INCLUDE_SEPARATE_PROGMEM) */

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
OSDeviceDebug::putString(uchar_t __attribute__((unused)) * pc)
{
}

inline void
OSDeviceDebug::putConstructor(const char* pc __attribute__((unused)), const void* p __attribute__((unused)))
{
}

inline void
OSDeviceDebug::putConstructorWithIndex(const char* pc __attribute__((unused)), uint16_t i __attribute__((unused)),
    const void* p __attribute__((unused)))
{
}

inline void
OSDeviceDebug::putDestructor(const char* pc __attribute__((unused)), const void* p __attribute__((unused)))
{
}

#if defined(OS_INCLUDE_SEPARATE_PROGMEM)

inline void OSDeviceDebug::putString_P(const char* PROGMEM pc __attribute__((unused)))
  {
  }

inline void OSDeviceDebug::putString_P(uchar_t* PROGMEM pc __attribute__((unused)))
  {
  }

inline void
OSDeviceDebug::putConstructor_P(const char* PROGMEM pc __attribute__((unused)), const void* p __attribute__((unused)))
  {
  }

inline void
OSDeviceDebug::putConstructorWithIndex_P(const char* PROGMEM pc __attribute__((unused)), uint16_t i __attribute__((unused)), const void* p __attribute__((unused)))
  {
  }

inline void
OSDeviceDebug::putDestructor_P(const char* PROGMEM pc __attribute__((unused)), const void* p __attribute__((unused)))
  {
  }

#endif

inline void
OSDeviceDebug::putHex(uint8_t __attribute__((unused)) c)
{
}

inline void
OSDeviceDebug::putHex(uint16_t __attribute__((unused)) w)
{
}

inline void
OSDeviceDebug::putHex(uint32_t l __attribute__((unused)))
{

}

inline void
OSDeviceDebug::putDec(uint8_t b __attribute__((unused)), uint16_t n __attribute__((unused)))
{

}
inline void
OSDeviceDebug::putDec(uint16_t __attribute__((unused)) w,
    uint16_t __attribute__((unused)) n)
{
}

inline void
OSDeviceDebug::putDec(uint32_t l __attribute__((unused)), uint16_t n __attribute__((unused)))
{

}

#if !defined(OS_CONFIG_ARCH_AVR8)
inline void
OSDeviceDebug::putDec(uint_t i __attribute__((unused)), uint16_t n __attribute__((unused)))
{

}
#endif

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
OSDeviceDebug::putPtr(const void* p __attribute__((unused)))
{
  ;
}

inline void
OSDeviceDebug::assert(const char __attribute__((unused)) *func,
    const char __attribute__((unused)) *xfile,
    int __attribute__((unused)) lineno,
    const char __attribute__((unused)) *sexp)
{
}

#endif

#endif /* OSDEVICEDEBUG_H_ */
