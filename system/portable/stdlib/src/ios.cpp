/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 * 	Based on open source code with Embedded C++ constraints.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_OSTREAM) || defined(OS_INCLUDE_ISTREAM)

#include "portable/devices/debug/include/OSDeviceDebug.h"
#include "portable/stdlib/include/ios"

#if 0
int ios_base::Init::init_cnt = 0; //Needed to ensure the static value is created

ios_base::Init::Init()
  {
    if ( init_cnt == 0 )
      { //Need to construct cout et al
        ;
      }
    init_cnt++;
  }

ios_base::Init::~Init()
  {
    --init_cnt;
    if ( init_cnt == 0 )
      {
        ;
      }
  }
#endif

ios_base::ios_base() :
  m_format(dec | unitbuf), m_state(goodbit), m_precision( 6), m_width( 0)
  {
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
    OSDeviceDebug::putString("ios_base()");
    OSDeviceDebug::putNewLine();
#endif
  }

ios_base::~ios_base()
  {
    ;
  }

streamsize ios_base::precision(streamsize prec)
  {
    streamsize temp = m_precision;
    m_precision = prec;
    return temp;
  }

streamsize ios_base::width(streamsize wide)
  {
    streamsize temp = m_width;
    m_width = wide;
    return temp;
  }

ios::ios()
  {
    ;
  }

ios::ios(streambuf * sb)
  {
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
    OSDeviceDebug::putString("ios()");
    OSDeviceDebug::putNewLine();
#endif
    init(sb/*, 0 , 0 */);
  }

#if 0
ios::ios( OSDeviceCharacter * dev )
  {
    OSDeviceDebug::putString( "ios()" );
    OSDeviceDebug::putNewLine();
    init( 0 /*, dev, 0 */);
  }

ios::ios( OSDeviceDebug * dbg )
  {
    OSDeviceDebug::putString( "ios()" );
    OSDeviceDebug::putNewLine();
    init( 0 /*, 0, dbg */);
  }
#endif

ios::~ios()
  {
    ;
  }

void ios::init(streambuf * sb /*, OSDeviceCharacter * dev , OSDeviceDebug * dbg*/)
  {
    m_streambuf = sb;
    //m_device = dev;
    // m_debug = dbg;
    m_fill_char = ' ';

    flags(goodbit);
  }

#if 0
ostream *
ios::tie( ostream * tiestr )
  {
    ostream * retval = m_tied;
    m_tied = tiestr;
    return retval;
  }
#endif

streambuf * ios::rdbuf(streambuf * sb)
  {
    streambuf * retval = m_streambuf;
    m_streambuf = sb;
    return retval;
  }

char ios::fill(char ch)
  {
    char temp = m_fill_char;
    m_fill_char = ch;
    return temp;
  }

void ios::setstate(iostate state)
  {
    clear(rdstate() | state);
  }

#endif

