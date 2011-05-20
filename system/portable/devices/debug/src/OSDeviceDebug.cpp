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

#if defined(DEBUG)

#if defined(OS_INCLUDE_NAKED_INIT)

// ----- init5 ---------------------------------------------------------------

void OSDeviceDebug::nakedEarlyInit(void)
  {
    earlyInit();
  }

#endif

extern const char greeting[] __attribute__((weak));

// Default greeting message
const char greeting[] = APP_CFGSTR_GREETING;

void
OSDeviceDebug::earlyInit(void)
{
  implEarlyInit();

  commonPutBytes("\r\n\r\n", 4);
  commonPutBytes(greeting, strlen(greeting));
  commonPutBytes("\r\n", 2);
  commonPutBytes(OS_CFGSTR_GREETING, sizeof(OS_CFGSTR_GREETING));
  commonPutBytes("\r\n\r\n", 4);
}

// ----------------------------------------------------------------------------

int
OSDeviceDebug::commonPutByte(unsigned char c)
{
  if (!implDevicePresent())
    return -1;

  while (!implCanTransmit())
    ;

  implPutByte(c);

  return (int) c;
}

int
OSDeviceDebug::commonPutBytes(const char* s, unsigned int n)
{
  if (!implDevicePresent())
    return -1;

  while (!implCanTransmit())
    ;

  return implPutBytes(s, n);
}

#if defined(OS_DEBUG_CONSTRUCTORS)

// display constructor name and object address
void
OSDeviceDebug::putConstructor(const char* pc, const void* p)
{
  OSCriticalSection::enter();
    {
      putString(pc);
      putString("() @");
      putPtr(p);
      putNewLine();
    }
  OSCriticalSection::exit();
}

// display destructor name and object address
void
OSDeviceDebug::putDestructor(const char* pc, const void* p)
{
  OSCriticalSection::enter();
    {
      putChar('~');
      putString(pc);
      putString("() @");
      putPtr(p);
      putNewLine();
    }
  OSCriticalSection::exit();
}

#endif /* defined(OS_DEBUG_CONSTRUCTORS) */

void
OSDeviceDebug::putChar(unsigned char c)
{
  OSCriticalSection::enter();
    {
      commonPutByte(c);
    }
  OSCriticalSection::exit();
}

void
OSDeviceDebug::putNewLine(void)
{
  OSCriticalSection::enter();
    {
      commonPutByte('\r');
      commonPutByte('\n');
    }
  OSCriticalSection::exit();
}

void
OSDeviceDebug::putString(const char* pc)
{
  if (pc == 0)
    return;

  OSCriticalSection::enter();
    {
      commonPutBytes(pc, strlen(pc));
    }
  OSCriticalSection::exit();
}

void
OSDeviceDebug::putHex(unsigned char b)
{
  unsigned char c;
  unsigned char buff[2];

  c = ((b >> 4) & 0x0F);
  buff[0] = c < 10 ? c + '0' : c + 'A' - 10;

  c = (b & 0x0F);
  buff[1] = c < 10 ? c + '0' : c + 'A' - 10;

  OSCriticalSection::enter();
    {
      commonPutBytes((const char*) buff, sizeof(buff));
    }
  OSCriticalSection::exit();
}

void
OSDeviceDebug::putHex(unsigned short w)
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

  OSCriticalSection::enter();
    {
      commonPutBytes((const char*) buff, sizeof(buff));
    }
  OSCriticalSection::exit();
}

#if defined(DEBUG) && defined(OS_INCLUDE_OSDEVICEDEBUG_PUTHEX_LONG)
void
OSDeviceDebug::putHex(unsigned long l)
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

  OSCriticalSection::enter();
    {
      commonPutBytes((const char*) buff, sizeof(buff));
    }
  OSCriticalSection::exit();
}

#endif

#if defined(DEBUG) && defined(OS_INCLUDE_OSDEVICEDEBUG_PUTHEX_INT)

void
OSDeviceDebug::putHex(unsigned int l)
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

  OSCriticalSection::enter();
    {
      commonPutBytes((const char*) buff, sizeof(buff));
    }
  OSCriticalSection::exit();
}

#endif

void
OSDeviceDebug::putPC(const char*  PROGMEM pc)
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

  OSCriticalSection::enter();
    {
      commonPutBytes((const char*) buff, sizeof(buff));
    }
  OSCriticalSection::exit();
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

  OSCriticalSection::enter();
    {
      commonPutBytes((const char*) buff, sizeof(buff));
    }
  OSCriticalSection::exit();
}

void
OSDeviceDebug::putDec(unsigned short w, unsigned short n)
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
      OSCriticalSection::enter();
        {
          commonPutBytes((const char*) &buff[i], (int) sizeof(buff) - i);
        }
      OSCriticalSection::exit();
    }
}

#if defined(OS_INCLUDE_OSDEVICEDEBUG_PUTDEC_LONG)

void
OSDeviceDebug::putDec(unsigned long w, unsigned short n)
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
      OSCriticalSection::enter();
        {
          commonPutBytes((const char*) &buff[i], (int) sizeof(buff) - i);
        }
      OSCriticalSection::exit();
    }
}
#endif /* OS_INCLUDE_OSDEVICEDEBUG_PUTDEC_LONG */

void
OSDeviceDebug::__assert(const char* func, const char* file, int lineno,
    const char* sexp)
{
  OSCriticalSection::enter();
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
        implWDReset();
    }
  OSCriticalSection::exit();
}

// streambuf methods
#if defined(OS_INCLUDE_OSDEVICEDEBUG_STREAMBUF)

int
OSDeviceDebug::overflow(int c)
{
  int r;
  OSCriticalSection::enter();
    {
      r = commonPutByte(c);
    }
  OSCriticalSection::exit();

  return r;
}

streamsize
OSDeviceDebug::xsputn(const char* s, streamsize n)
{
#if defined(DEBUG)

  int r;
  OSCriticalSection::enter();
    {
      r = commonPutBytes(s, n);
    }
  OSCriticalSection::exit();

  return r;
#else

  return -1;
#endif /* defined(DEBUG) */
}

#endif /* defined(OS_INCLUDE_OSDEVICEDEBUG_STREAMBUF) */

#endif /* defined(DEBUG) */
