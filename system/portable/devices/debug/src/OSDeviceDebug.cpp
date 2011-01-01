/*
 *	Copyright (C) 2007-2008 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS.h"
//#include "portable/devices/debug/include/OSDeviceDebug.h"
//#include "portable/kernel/include/OSScheduler.h"

#include <string.h>

#if defined(DEBUG) && !defined(OS_EXCLUDE_MULTITASKING)

OSDeviceDebug::OSDeviceDebug()
  {
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
    OSDeviceDebug::putString_P(PSTR("OSDeviceDebug()="));
    OSDeviceDebug::putPtr(this);
    OSDeviceDebug::putNewLine();
#endif
  }
#endif /*OS_EXCLUDE_MULTITASKING*/

#if defined(DEBUG)

extern const char greeting[] __attribute__( ( weak ) );

const char greeting[] = "uOS++"; // default debug greeting

#if defined(OS_INCLUDE_NAKED_INIT)

// ----- init5 ---------------------------------------------------------------

void OSDeviceDebug::nakedEarlyInit(void)
  {
    earlyInit();
  }

#endif

void OSDeviceDebug::earlyInit(void)
  {
    implEarlyInit();

    commonPutBytes("\r\n\r\n", 4);
    commonPutBytes(greeting, strlen(greeting));
    commonPutBytes("\r\n", 2);
    commonPutBytes(OS_STRING_OSGREETING, sizeof(OS_STRING_OSGREETING));
    commonPutBytes("\r\n\r\n", 4);
  }

// ----- ------------------------------------------------------

int OSDeviceDebug::commonPutByte(unsigned char c)
  {
    if (!implDevicePresent())
      return -1;

    while ( !implCanTransmit() )
      ;

    implPutByte(c);

    return ( int ) c;
  }

int OSDeviceDebug::commonPutBytes(const char *s, unsigned int n)
  {
    if (!implDevicePresent())
      return -1;

    while ( !implCanTransmit() )
      ;

    return implPutBytes(s, n);
  }

void OSDeviceDebug::putChar(unsigned char c)
  {
    OSScheduler::criticalEnter();
      {
        commonPutByte(c);
      }
    OSScheduler::criticalExit();
  }

void OSDeviceDebug::putNewLine(void)
  {
    OSScheduler::criticalEnter();
      {
        commonPutByte( '\r');
        commonPutByte( '\n');
      }
    OSScheduler::criticalExit();
  }

void OSDeviceDebug::putString(const char *pc)
  {
    if (pc == 0)
      return;

    OSScheduler::criticalEnter();
      {
        commonPutBytes(pc, strlen(pc));
      }
    OSScheduler::criticalExit();
  }

void OSDeviceDebug::putHex(unsigned char b)
  {
    OSScheduler::criticalEnter();
      {
        unsigned char c;
        unsigned char buff[ 2 ];

        c = ( (b >> 4 ) & 0x0F );
        buff[0] = c < 10 ? c + '0' : c + 'A' - 10;

        c = (b & 0x0F );
        buff[1] =c < 10 ? c + '0' : c + 'A' - 10;

        commonPutBytes((const char*)buff, sizeof(buff));
      }
    OSScheduler::criticalExit();
  }

void OSDeviceDebug::putHex(unsigned short w)
  {
    OSScheduler::criticalEnter();
      {
        unsigned char c;
        unsigned char b;
        unsigned char buff[ 4 ];

        b = (w >> 8 );
        c = ( (b >> 4 ) & 0x0F );
        buff[0] =c < 10 ? c + '0' : c + 'A' - 10;

        c = (b & 0x0F );
        buff[1] = c < 10 ? c + '0' : c + 'A' - 10;

        b = (w & 0xFF );
        c = ( (b >> 4 ) & 0x0F );
        buff[2] = c < 10 ? c + '0' : c + 'A' - 10;

        c = (b & 0x0F );
        buff[3] = c < 10 ? c + '0' : c + 'A' - 10;

        commonPutBytes((const char*)buff, sizeof(buff));
      }
    OSScheduler::criticalExit();
  }

#if defined(DEBUG) && defined(OS_INCLUDE_OSDEVICEDEBUG_PUTHEX_LONG)
void OSDeviceDebug::putHex(unsigned long l)
  {
    OSScheduler::criticalEnter();
      {
        unsigned char c;
        unsigned char b;
        unsigned char buff[ 8 ];

        b = (l >> 24 );
        c = ( (b >> 4 ) & 0x0F );
        buff[0] =c < 10 ? c + '0' : c + 'A' - 10;

        c = (b & 0x0F );
        buff[1] = c < 10 ? c + '0' : c + 'A' - 10;

        b = (l >> 16 );
        c = ( (b >> 4 ) & 0x0F );
        buff[2] = c < 10 ? c + '0' : c + 'A' - 10;

        c = (b & 0x0F );
        buff[3] = c < 10 ? c + '0' : c + 'A' - 10;

        b = (l >> 8 );
        c = ( (b >> 4 ) & 0x0F );
        buff[4] =c < 10 ? c + '0' : c + 'A' - 10;

        c = (b & 0x0F );
        buff[5] = c < 10 ? c + '0' : c + 'A' - 10;

        b = (l & 0xFF );
        c = ( (b >> 4 ) & 0x0F );
        buff[6] = c < 10 ? c + '0' : c + 'A' - 10;

        c = (b & 0x0F );
        buff[7] = c < 10 ? c + '0' : c + 'A' - 10;

        commonPutBytes((const char*)buff, sizeof(buff));
      }
    OSScheduler::criticalExit();
  }

#endif

#if defined(DEBUG) && defined(OS_INCLUDE_OSDEVICEDEBUG_PUTHEX_INT)

void OSDeviceDebug::putHex(unsigned int l)
  {
    OSScheduler::criticalEnter();
      {
        unsigned char c;
        unsigned char b;
        unsigned char buff[ 8 ];

        b = (l >> 24 );
        c = ( (b >> 4 ) & 0x0F );
        buff[0] =c < 10 ? c + '0' : c + 'A' - 10;

        c = (b & 0x0F );
        buff[1] = c < 10 ? c + '0' : c + 'A' - 10;

        b = (l >> 16 );
        c = ( (b >> 4 ) & 0x0F );
        buff[2] = c < 10 ? c + '0' : c + 'A' - 10;

        c = (b & 0x0F );
        buff[3] = c < 10 ? c + '0' : c + 'A' - 10;

        b = (l >> 8 );
        c = ( (b >> 4 ) & 0x0F );
        buff[4] =c < 10 ? c + '0' : c + 'A' - 10;

        c = (b & 0x0F );
        buff[5] = c < 10 ? c + '0' : c + 'A' - 10;

        b = (l & 0xFF );
        c = ( (b >> 4 ) & 0x0F );
        buff[6] = c < 10 ? c + '0' : c + 'A' - 10;

        c = (b & 0x0F );
        buff[7] = c < 10 ? c + '0' : c + 'A' - 10;

        commonPutBytes((const char*)buff, sizeof(buff));
      }
    OSScheduler::criticalExit();
  }

#endif

void OSDeviceDebug::putPC(const char * PROGMEM pc)
  {
    OSScheduler::criticalEnter();
      {
        unsigned char c;
        unsigned char b;

        int i;
        i = 0;

#if defined(OS_CONFIG_ARCH_AVR8)
        unsigned char buff[ 5 ];
        unsigned short l;
        l = (unsigned short)pc;

        c = ( (l >> 15 ) & 0x01 );
        buff[i++] = c + '0';

        l <<= 1;
#else
        unsigned char buff[ 8 ];
        unsigned long l;
        l = (unsigned long)pc;

        b = (l >> 24 );
        c = ( (b >> 4 ) & 0x0F );
        buff[i++] = c < 10 ? c + '0' : c + 'A' - 10;

        c = (b & 0x0F );
        buff[i++] = c < 10 ? c + '0' : c + 'A' - 10;

        b = (l >> 16 );
        c = ( (b >> 4 ) & 0x0F );
        buff[i++] = c < 10 ? c + '0' : c + 'A' - 10;

        c = (b & 0x0F );
        buff[i++] = c < 10 ? c + '0' : c + 'A' - 10;
#endif

        b = (l >> 8 );
        c = ( (b >> 4 ) & 0x0F );
        buff[i++] = c < 10 ? c + '0' : c + 'A' - 10;

        c = (b & 0x0F );
        buff[i++] = c < 10 ? c + '0' : c + 'A' - 10;

        b = (l & 0xFF );
        c = ( (b >> 4 ) & 0x0F );
        buff[i++] = c < 10 ? c + '0' : c + 'A' - 10;

        c = (b & 0x0F );
        buff[i++] = c < 10 ? c + '0' : c + 'A' - 10;

        commonPutBytes((const char*)buff, sizeof(buff));
      }
    OSScheduler::criticalExit();
  }

void OSDeviceDebug::putPtr(const void *p)
  {
    OSScheduler::criticalEnter();
      {
        unsigned char c;
        unsigned char b;

        int i;
        i = 0;

#if (__SIZEOF_POINTER__ == 2)
        unsigned char buff[ 4 ];
        unsigned short v;
        v = (unsigned short)p;

#else
        unsigned char buff[ 8 ];
        unsigned long v;
        v = (unsigned long)p;

        b = (v >> 24 );
        c = ( (b >> 4 ) & 0x0F );
        buff[i++] = c < 10 ? c + '0' : c + 'A' - 10;

        c = (b & 0x0F );
        buff[i++] = c < 10 ? c + '0' : c + 'A' - 10;

        b = (v >> 16 );
        c = ( (b >> 4 ) & 0x0F );
        buff[i++] = c < 10 ? c + '0' : c + 'A' - 10;

        c = (b & 0x0F );
        buff[i++] = c < 10 ? c + '0' : c + 'A' - 10;
#endif

        b = (v >> 8 );
        c = ( (b >> 4 ) & 0x0F );
        buff[i++] = c < 10 ? c + '0' : c + 'A' - 10;

        c = (b & 0x0F );
        buff[i++] = c < 10 ? c + '0' : c + 'A' - 10;

        b = (v & 0xFF );
        c = ( (b >> 4 ) & 0x0F );
        buff[i++] = c < 10 ? c + '0' : c + 'A' - 10;

        c = (b & 0x0F );
        buff[i++] = c < 10 ? c + '0' : c + 'A' - 10;

        commonPutBytes((const char*)buff, sizeof(buff));
      }
    OSScheduler::criticalExit();
  }

void OSDeviceDebug::putDec(unsigned short w, unsigned short n)
  {
    OSScheduler::criticalEnter();
      {
        int i;
        unsigned char buff[ 5 ];

        for (i = sizeof(buff ) - 1; i >= 0; i--)
          {
            buff[ i ] = (w % 10 ) + '0';
            w /= 10;
          }

        if ( 0 < n && n <= ( int ) sizeof(buff ) )
          i = ( int ) sizeof(buff ) - n;
        else
          {
            for (i = 0; i < ( int ) sizeof(buff ) - 1; ++i)
              if (buff[ i ] != '0')
                break;
          }
        if (i < ( int ) sizeof(buff ))
          commonPutBytes( (const char*)&buff[ i ], ( int ) sizeof(buff ) - i);

      }
    OSScheduler::criticalExit();
  }

#if defined(OS_INCLUDE_OSDEVICEDEBUG_PUTDEC_LONG)

void OSDeviceDebug::putDec(unsigned long w, unsigned short n)
  {
    OSScheduler::criticalEnter();
      {
        int i;
        unsigned char buff[ 10 ];

        for (i = sizeof(buff ) - 1; i >= 0; i--)
          {
            buff[ i ] = (w % 10 ) + '0';
            w /= 10;
          }

        if ( 0 < n && n <= ( int ) sizeof(buff ) )
          i = ( int ) sizeof(buff ) - n;
        else
          {
            for (i = 0; i < ( int ) sizeof(buff ) - 1; ++i)
              if (buff[ i ] != '0')
                break;
          }
        if (i < ( int ) sizeof(buff ))
          commonPutBytes( (const char*)&buff[ i ], ( int ) sizeof(buff ) - i);

      }
    OSScheduler::criticalExit();
  }
#endif /*OS_INCLUDE_OSDEVICEDEBUG_PUTDEC_LONG*/

void OSDeviceDebug::__assert(const char *func, const char *file,
    int lineno, const char *sexp)
  {
    OSScheduler::criticalEnter();
      {
        putString_P(PSTR("assertion '"));
        putString(sexp);
        putString_P(PSTR("' failed in '"));
        putString(func);
        putString_P(PSTR("', file '"));
        putString(file);
        putString_P(PSTR("' at line "));
        putDec((unsigned short)lineno);
        putNewLine();

        for (;;)
          implWDReset();
      }
    OSScheduler::criticalExit();
  }

// streambuf methods
#if defined(OS_INCLUDE_OSDEVICEDEBUG_STREAMBUF)

int OSDeviceDebug::overflow(int c)
  {
    int r;
    OSScheduler::criticalEnter();
      {
        r = commonPutByte(c);
      }
    OSScheduler::criticalExit();

    return r;
  }

streamsize OSDeviceDebug::xsputn(const char* s, streamsize n)
  {
#if defined(DEBUG)

    int r;
    OSScheduler::criticalEnter();
      {
        r = commonPutBytes(s, n);
      }
    OSScheduler::criticalExit();

    return r;
#else

    return -1;
#endif /*DEBUG*/
  }

#endif /* OS_INCLUDE_OSDEVICEDEBUG_STREAMBUF */

#endif /* DEBUG */
