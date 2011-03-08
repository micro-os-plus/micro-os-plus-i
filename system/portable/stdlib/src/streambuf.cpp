/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 * 	Based on open source code with Embedded C++ constraints.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_OSTREAM) || defined(OS_INCLUDE_ISTREAM)

#include "portable/devices/debug/include/OSDeviceDebug.h"
#include "portable/stdlib/include/streambuf"

streambuf::streambuf()
  {
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
    OSDeviceDebug::putString("streambuf()=");
    OSDeviceDebug::putPtr(this);
    OSDeviceDebug::putNewLine();
#endif
    //mgbeg = mgnext = mgend = mpbeg = mpnext = mpend = 0;
    //openedFor = 0;
  }

streambuf::~streambuf()
  {
    ;
  }

#if 0

void
streambuf::setg( char* gbeg, char* gnext, char* gend )
  {
    mgbeg = gbeg;
    mgnext = gnext;
    mgend = gend;
  }

void
streambuf::setp( char* pbeg, char* pend )
  {
    mpbeg = pbeg;
    mpnext = pbeg;
    mpend = pend;
  }
#endif

streamsize streambuf::in_avail()
  {
#if 0
    if ( mgend != 0 && mgnext != 0 )
      {
        return mgend - mgnext;
      }
#endif
    return showmanyc();
  }

int streambuf::sbumpc()
  {
#if 0
    if ( mgbeg == 0 )
    return traits::eof();

    if ( mgnext == mgend )
      {
        return uflow();
      }
    int retval = traits::to_int( *gptr() );
    gbump( 1 );
    return retval;
#else

    return traits::eof();
#endif
  }

int streambuf::snextc()
  {
#if 0
    if ( sbumpc() == traits::eof() )
      {
        return traits::eof();
      }
#endif
    return sgetc();
  }

int streambuf::sgetc()
  {
#if 0
    if ( mgbeg == 0 || mgnext == mgend )
      {
        return underflow();
      }
    return traits::to_int( *gptr() );
#else

    return underflow();

#endif
  }

#if 0

int
streambuf::sputbackc( char c )
  {
    if ( mgbeg == 0 || mgnext == mgbeg || !traits::eq( c, gptr() [ -1 ] ) )
      {
        return pbackfail( traits::to_int( c ) );
      }
    gbump( -1 );
    return traits::to_int( *gptr() );
  }

int
streambuf::sungetc()
  {
    if ( mgbeg == 0 || mgnext == mgbeg )
      {
        return ios_base::failbit;
      }
    gbump( -1 );
    return traits::to_int( *gptr() );
  }
#endif

int streambuf::sputc(char c)
  {
#if 0
    if ( openedFor & ios_base::app )
      {
        seekoff( 0, ios_base::end, ios_base::out );
      }
    if ( mpnext < mpend )
      {
        *mpnext = c;
        ++mpnext;
      }
    else
      {
        return overflow( traits::to_int( c ) );
      }
    return traits::to_int( c );

#else

    return overflow(traits::to_int(c) );
#endif
  }

int streambuf::sputn(const char* s, streamsize n)
  {
#if 0
    if ( openedFor & ios_base::app )
      {
        seekoff( 0, ios_base::end, ios_base::out );
      }
#endif
    return xsputn(s, n);
  }

int streambuf::sgetn(char* s, streamsize n)
  {
    return xsgetn(s, n);
  }

//----- virtual methods ------------------------------------------------------

streambuf* streambuf::setbuf(char* s __attribute__( ( unused ) ), streamsize n __attribute__( ( unused ) ) )
  {
    return 0;
  }

pos_type streambuf::seekoff(off_type off __attribute__( ( unused ) ), ios_base::seekdir way __attribute__( ( unused ) ),
    ios_base::openmode which __attribute__( ( unused ) ) )
  {
    return 0;
  }

pos_type streambuf::seekpos(pos_type sp __attribute__( ( unused ) ), ios_base::openmode which __attribute__( ( unused ) ) )
  {
    return 0;
  }

int streambuf::sync()
  {
    return 0;
  }

// 27.5.2.4.3 Get area:

int streambuf::showmanyc()
  {
    return 0;
  }

streamsize streambuf::xsgetn(char* s __attribute__( ( unused ) ), streamsize n __attribute__( ( unused ) ) )
  {
    return 0;
  }

// return char or eof

int streambuf::underflow()
  {
    return traits::eof();
  }

int streambuf::uflow()
  {
    int ret = underflow();
#if 0

    if ( !traits::eq_int( ret, traits::eof() ) )
    gbump( 1 );
#endif

    return ret;
  }

// 27.5.2.4.4 Putback:

int streambuf::pbackfail(int c)
  {
    return c;
  }

// 27.5.2.2.5 Put area:

streamsize streambuf::xsputn(const char* s, streamsize n)
  {
    //This function is designed to be replaced by subclasses
    for (streamsize i = 0; i < n; ++i)
      {
        if (sputc(s[ i ]) == traits::eof() )
          {
            return i;
          }
      }
    return n;
  }

// return EOF if
int streambuf::overflow(int c)
  {
    return c;
  }

#endif

