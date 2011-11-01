/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

// uOS.h used instead of OS.h to get APP_CFGSTR_GREETING
#include "portable/kernel/include/uOS.h"

#include <string.h>

#if defined(DEBUG) && !defined(OS_EXCLUDE_MULTITASKING)

OSDeviceDebug::OSDeviceDebug()
{
  OSDeviceDebug::putConstructor_P(PSTR("OSDeviceDebug"), this);
}
#endif /* OS_EXCLUDE_MULTITASKING */

#if defined(DEBUG) || defined(OS_INCLUDE_GREETING)
extern const char greeting[] __attribute__((weak));

// Default greeting message
const char greeting[] = APP_CFGSTR_GREETING;
#endif /* defined(DEBUG) || defined(OS_INCLUDE_GREETING) */

#if defined(DEBUG)

#if defined(OS_INCLUDE_NAKED_INIT)

// ----- init5 ---------------------------------------------------------------

void OSDeviceDebug::nakedEarlyInit(void)
  {
    earlyInit();
  }

#endif

void
OSDeviceDebug::earlyInit(void)
{
  DeviceDebugImpl_t::implEarlyInit();

  commonPutBytes("\r\n\r\n", 4);
  commonPutBytes(greeting, strlen(greeting));
  commonPutBytes("\r\n", 2);
  commonPutBytes(OS_CFGSTR_GREETING, sizeof(OS_CFGSTR_GREETING));
  commonPutBytes("\r\n", 4);
}

// ----------------------------------------------------------------------------

#if false
int
OSDeviceDebug::commonPutByte(uchar_t c)
  {
#if !defined(OS_INCLUDE_OSDEVICEDEBUG_BUFFER)
    if (!implDevicePresent())
    return -1;

    while (!implCanTransmit())
    ;

    implPutByte(c);

    return (int) c;
#else
    return commonPutBytes(&c, 1);
#endif
  }
#endif

int
OSDeviceDebug::commonPutBytes(const char* s, uint_t n)
{
  if (!DeviceDebugImpl_t::implDevicePresent())
    return -1;

  while (!DeviceDebugImpl_t::implCanTransmit())
    ;

  return DeviceDebugImpl_t::implPutBytes(s, n);
}

int
OSDeviceDebug::commonPutBytesSynchronised(const char* s, uint_t n)
{
  int ret;
#if !defined(OS_INCLUDE_OSDEVICEDEBUG_BUFFER)
  register OSStack_t mask;
  mask = DeviceDebugImpl_t::criticalEnter();
#endif /* !defined(OS_INCLUDE_OSDEVICEDEBUG_BUFFER) */
    {
      ret = commonPutBytes(s, n);
    }
#if !defined(OS_INCLUDE_OSDEVICEDEBUG_BUFFER)
    DeviceDebugImpl_t::criticalExit(mask);
#endif /* !defined(OS_INCLUDE_OSDEVICEDEBUG_BUFFER) */

  return ret;
}

#if defined(OS_DEBUG_CONSTRUCTORS)

// display constructor name and object address
void
OSDeviceDebug::putConstructor(const char* pc, const void* p)
  {
    register OSStack_t mask;

    mask = criticalEnter();
      {
        putString(pc);
        putString("() @");
        putPtr(p);
        putNewLine();
      }
    criticalExit(mask);
  }

// display constructor name and object address
void
OSDeviceDebug::putConstructorWithIndex(const char* pc, uint16_t i,
    const void* p)
  {
    register OSStack_t mask;

    mask = criticalEnter();
      {
        putString(pc);
        putChar('(');
        putDec(i);
        putString(") @");
        putPtr(p);
        putNewLine();
      }
    criticalExit(mask);
  }

// display destructor name and object address
void
OSDeviceDebug::putDestructor(const char* pc, const void* p)
  {
    register OSStack_t mask;

    mask = criticalEnter();
      {
        putChar('~');
        putString(pc);
        putString("() @");
        putPtr(p);
        putNewLine();
      }
    criticalExit(mask);
  }

#endif /* defined(OS_DEBUG_CONSTRUCTORS) */

void
OSDeviceDebug::putChar(uchar_t c)
{
  char ch;
  ch = c;

  commonPutBytesSynchronised(&ch, 1);
}

void
OSDeviceDebug::putNewLine(void)
{
  commonPutBytesSynchronised("\r\n", 2);
}

void
OSDeviceDebug::putString(const char* pc)
{
  if (pc == 0)
    return;

  commonPutBytesSynchronised(pc, strlen(pc));
}

void
OSDeviceDebug::putString(uchar_t* pc)
{
  putString((const char*) pc);
}

void
OSDeviceDebug::putHex(uint8_t b)
{
  unsigned char c;
  unsigned char buff[2];

  c = ((b >> 4) & 0x0F);
  buff[0] = c < 10 ? c + '0' : c + 'A' - 10;

  c = (b & 0x0F);
  buff[1] = c < 10 ? c + '0' : c + 'A' - 10;

  commonPutBytesSynchronised((const char*) buff, sizeof(buff));
}

void
OSDeviceDebug::putHex(uint16_t w)
{
  unsigned char c;
  unsigned char b;
  unsigned char buff[4];

  b = (w >> 8);
  c = ((b >> 4) & 0x0F);
  buff[0] = c < 10 ? c + '0' : c + 'A' - 10;

  c = (b & 0x0F);
  buff[1] = c < 10 ? c + '0' : c + 'A' - 10;

  b = (w & 0xFF);
  c = ((b >> 4) & 0x0F);
  buff[2] = c < 10 ? c + '0' : c + 'A' - 10;

  c = (b & 0x0F);
  buff[3] = c < 10 ? c + '0' : c + 'A' - 10;

  commonPutBytesSynchronised((const char*) buff, sizeof(buff));
}

void
OSDeviceDebug::putHex(uint32_t l)
{
  unsigned char c;
  unsigned char b;
  unsigned char buff[8];

  b = (l >> 24);
  c = ((b >> 4) & 0x0F);
  buff[0] = c < 10 ? c + '0' : c + 'A' - 10;

  c = (b & 0x0F);
  buff[1] = c < 10 ? c + '0' : c + 'A' - 10;

  b = (l >> 16);
  c = ((b >> 4) & 0x0F);
  buff[2] = c < 10 ? c + '0' : c + 'A' - 10;

  c = (b & 0x0F);
  buff[3] = c < 10 ? c + '0' : c + 'A' - 10;

  b = (l >> 8);
  c = ((b >> 4) & 0x0F);
  buff[4] = c < 10 ? c + '0' : c + 'A' - 10;

  c = (b & 0x0F);
  buff[5] = c < 10 ? c + '0' : c + 'A' - 10;

  b = (l & 0xFF);
  c = ((b >> 4) & 0x0F);
  buff[6] = c < 10 ? c + '0' : c + 'A' - 10;

  c = (b & 0x0F);
  buff[7] = c < 10 ? c + '0' : c + 'A' - 10;

  commonPutBytesSynchronised((const char*) buff, sizeof(buff));
}

#if !defined(OS_CONFIG_ARCH_AVR8)

void
OSDeviceDebug::putHex(uint_t i)
{
#if (__SIZEOF_INT__ == 2)
  putHex((uint16_t)i);
#else
  putHex((uint32_t) i);
#endif
}

#endif /* !defined(OS_CONFIG_ARCH_AVR8) */

void
OSDeviceDebug::putPC(const char* PROGMEM pc)
{
  unsigned char c;
  unsigned char b;

  int i;
  i = 0;

#if defined(OS_CONFIG_ARCH_AVR8)
  unsigned char buff[5];
  unsigned short l;
  l = (unsigned short)pc;

  c = ((l >> 15) & 0x01);
  buff[i++] = c + '0';

  l <<= 1;
#else
  unsigned char buff[8];
  unsigned long l;
  l = (unsigned long) pc;

  b = (l >> 24);
  c = ((b >> 4) & 0x0F);
  buff[i++] = c < 10 ? c + '0' : c + 'A' - 10;

  c = (b & 0x0F);
  buff[i++] = c < 10 ? c + '0' : c + 'A' - 10;

  b = (l >> 16);
  c = ((b >> 4) & 0x0F);
  buff[i++] = c < 10 ? c + '0' : c + 'A' - 10;

  c = (b & 0x0F);
  buff[i++] = c < 10 ? c + '0' : c + 'A' - 10;
#endif

  b = (l >> 8);
  c = ((b >> 4) & 0x0F);
  buff[i++] = c < 10 ? c + '0' : c + 'A' - 10;

  c = (b & 0x0F);
  buff[i++] = c < 10 ? c + '0' : c + 'A' - 10;

  b = (l & 0xFF);
  c = ((b >> 4) & 0x0F);
  buff[i++] = c < 10 ? c + '0' : c + 'A' - 10;

  c = (b & 0x0F);
  buff[i++] = c < 10 ? c + '0' : c + 'A' - 10;

  commonPutBytesSynchronised((const char*) buff, sizeof(buff));
}

void
OSDeviceDebug::putPtr(const void* p)
{
  unsigned char c;
  unsigned char b;

  int i;
  i = 0;

#if (__SIZEOF_POINTER__ == 2)
  unsigned char buff[4];
  unsigned short v;
  v = (unsigned short)p;

#else
  unsigned char buff[8];
  unsigned long v;
  v = (unsigned long) p;

  b = (v >> 24);
  c = ((b >> 4) & 0x0F);
  buff[i++] = c < 10 ? c + '0' : c + 'A' - 10;

  c = (b & 0x0F);
  buff[i++] = c < 10 ? c + '0' : c + 'A' - 10;

  b = (v >> 16);
  c = ((b >> 4) & 0x0F);
  buff[i++] = c < 10 ? c + '0' : c + 'A' - 10;

  c = (b & 0x0F);
  buff[i++] = c < 10 ? c + '0' : c + 'A' - 10;
#endif

  b = (v >> 8);
  c = ((b >> 4) & 0x0F);
  buff[i++] = c < 10 ? c + '0' : c + 'A' - 10;

  c = (b & 0x0F);
  buff[i++] = c < 10 ? c + '0' : c + 'A' - 10;

  b = (v & 0xFF);
  c = ((b >> 4) & 0x0F);
  buff[i++] = c < 10 ? c + '0' : c + 'A' - 10;

  c = (b & 0x0F);
  buff[i++] = c < 10 ? c + '0' : c + 'A' - 10;

  commonPutBytesSynchronised((const char*) buff, sizeof(buff));
}

void
OSDeviceDebug::putDec(uint8_t b, uint16_t n)
{
  putDec((uint16_t) b, n);
}

void
OSDeviceDebug::putDec(uint16_t w, uint16_t n)
{
  int i;
  unsigned char buff[5];

  for (i = sizeof(buff) - 1; i >= 0; i--)
    {
      buff[i] = (w % 10) + '0';
      w /= 10;
    }

  if (0 < n && n <= (int) sizeof(buff))
    i = (int) sizeof(buff) - n;
  else
    {
      for (i = 0; i < (int) sizeof(buff) - 1; ++i)
        if (buff[i] != '0')
          break;
    }
  if (i < (int) sizeof(buff))
    {
      commonPutBytesSynchronised((const char*) &buff[i], (int) sizeof(buff) - i);
    }
}

void
OSDeviceDebug::putDec(uint32_t w, uint16_t n)
{
  int i;
  unsigned char buff[10];

  for (i = sizeof(buff) - 1; i >= 0; i--)
    {
      buff[i] = (w % 10) + '0';
      w /= 10;
    }

  if (0 < n && n <= (int) sizeof(buff))
    i = (int) sizeof(buff) - n;
  else
    {
      for (i = 0; i < (int) sizeof(buff) - 1; ++i)
        if (buff[i] != '0')
          break;
    }
  if (i < (int) sizeof(buff))
    {
      commonPutBytesSynchronised((const char*) &buff[i], (int) sizeof(buff) - i);
    }
}

#if !defined(OS_CONFIG_ARCH_AVR8)

void
OSDeviceDebug::putDec(uint_t i, uint16_t n)
{
#if (__SIZEOF_INT__ == 2)
  putDec((uint16_t)i, n);
#else
  putDec((uint32_t) i, n);
#endif
}

#endif /* !defined(OS_CONFIG_ARCH_AVR8) */

#undef assert

void
OSDeviceDebug::assert(const char* func, const char* file, int lineno,
    const char* sexp)
{
  register OSStack_t mask;

  mask = DeviceDebugImpl_t::criticalEnter();
    {
      putString_P(PSTR("assertion '"));
      putString(sexp);
      putString_P(PSTR("' failed in '"));
      putString(func);
      putString_P(PSTR("', file '"));
      putString(file);
      putString_P(PSTR("' at line "));
      putDec((unsigned short) lineno);
      putNewLine();

      for (;;)
        DeviceDebugImpl_t::implWDReset();
    }
    DeviceDebugImpl_t::criticalExit(mask);
}

// streambuf methods
#if defined(OS_INCLUDE_OSDEVICEDEBUG_STREAMBUF)

int
OSDeviceDebug::overflow(int c)
{
  char ch;
  ch = c;

  int r;
  r = commonPutBytesSynchronised(&ch, 1);

  return r;
}

std::streamsize
OSDeviceDebug::xsputn(const char* s, std::streamsize n)
{
#if defined(DEBUG)

  int r;
  r = commonPutBytesSynchronised(s, n);

  return r;
#else

  return -1;
#endif /* defined(DEBUG) */
}

#endif /* defined(OS_INCLUDE_OSDEVICEDEBUG_STREAMBUF) */

#endif /* defined(DEBUG) */
