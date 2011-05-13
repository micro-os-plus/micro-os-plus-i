/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 * 	Based on open source code with Embedded C++ constraints.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_OSTREAM)

#include "portable/devices/debug/include/OSDeviceDebug.h"
#include "portable/stdlib/include/ostream"

ostream::ostream(streambuf * sb) :
  ios(sb)
{
  OSDeviceDebug::putConstructor_P(PSTR("ostream"), this);
}

#if 0
ostream::ostream( OSDeviceCharacter * dev ) : ios( dev )
  {
    OSDeviceDebug::putConstructor_P(PSTR("ostream"), this);
  }
#endif

/*
 ostream::ostream( OSDeviceDebug * dbg ) : ios( dbg )
 {
 OSDeviceDebug::putString( "ostream()" );
 OSDeviceDebug::putNewLine();
 }
 */

ostream::~ostream()
{
  ;
}

ostream &
ostream::operator <<(bool n)
{
  //sentry s( *this );
  if (ios::flags() & ios_base::boolalpha)
    {
      if (n)
        {
          write("true", 4);
        }
      else
        {
          write("false", 5);
        }
    }
  else
    {
      if (n)
        {
          put('1');
        }
      else
        {
          put('0');
        }
    }

  if (ios::flags() & ios_base::unitbuf)
    {
      flush();
    }
  return *this;
}

#if defined(OS_INCLUDE_OSTREAM_LONG)

void print32(ostream& out, unsigned long l, bool sign = false);

void print32(ostream& out, unsigned long l, bool sign)
  {
    streamsize width;
    width = out.width();
    if ( (out.flags() & ios_base::basefield ) == ios_base::hex)
      {
        unsigned char upp;
        upp = 0;
        if ( !(out.flags() & ios_base::uppercase ))
        upp = 'a' - 'A';

        if (out.flags() & ios_base::showbase)
          {
            out.put( '0');
            out.put( 'X' + upp);
          }

        upp = 0; // ! explicitly set all hex to upper case

        unsigned char c;
        unsigned char b;
        b = (l >> 24);
        if (width == 0|| width > 7)
          {
            c = ( (b >> 4) & 0x0F);
            out.put(c < 10 ? c + '0' : c + 'A'+ upp - 10);
          }
        if (width == 0|| width > 6)
          {
            c = (b & 0x0F);
            out.put(c < 10 ? c + '0' : c + 'A'+ upp - 10);
          }
        b = (l >> 16);
        if (width == 0|| width > 5)
          {
            c = ( (b >> 4) & 0x0F);
            out.put(c < 10 ? c + '0' : c + 'A'+ upp - 10);
          }
        if (width == 0|| width > 4)
          {
            c = (b & 0x0F);
            out.put(c < 10 ? c + '0' : c + 'A'+ upp - 10);
          }
        b = (l >> 8);
        if (width == 0|| width > 3)
          {
            c = ( (b >> 4) & 0x0F);
            out.put(c < 10 ? c + '0' : c + 'A'+ upp - 10);
          }
        if (width == 0|| width > 2)
          {
            c = (b & 0x0F);
            out.put(c < 10 ? c + '0' : c + 'A'+ upp - 10);
          }
        b = (l & 0xFF);
        if (width == 0|| width > 1)
          {
            c = ( (b >> 4) & 0x0F);
            out.put(c < 10 ? c + '0' : c + 'A'+ upp - 10);
          }
        c = (b & 0x0F);
        out.put(c < 10 ? c + '0' : c + 'A'+ upp - 10);
      }
    else if ( (out.flags() & ios_base::basefield ) == ios_base::oct)
      {
        ; // not yet implemented
      }
    else // decimal

      {
        if (sign)
          {
            long n;
            n = l;
            if (n < 0)
              {
                out.put( '-');
                l = -n;
              }
          }
        int i;
        unsigned char buff[ 10 ];

        for (i = sizeof(buff ) - 1; i >= 0; i--)
          {
            buff[ i ] = (l % 10) + '0';
            l /= 10;
          }

        if ( 0 < width && width <= ( int ) sizeof(buff ) )
        i = ( int ) sizeof(buff ) - width;
        else
          {
            for (i = 0; i < ( int ) sizeof(buff ) - 1; ++i)
            if (buff[ i ] != '0')
            break;
          }
        for (; i < ( int ) sizeof(buff ); ++i)
        out.put(buff[ i ]);
      }
    out.width( 0);
  }

#endif

#if (__SIZEOF_INT__ == 2)

void print16(ostream& out, unsigned short w, bool sign = false);

void print16(ostream& out, unsigned short w, bool sign)
  {
    streamsize width;
    width = out.width();
    if ( (out.flags() & ios_base::basefield ) == ios_base::hex)
      {
        unsigned char upp;
        upp = 0;
        if ( !(out.flags() & ios_base::uppercase ))
        upp = 'a' - 'A';

        if (out.flags() & ios_base::showbase)
          {
            out.put( '0');
            out.put( 'X' + upp);
          }

        upp = 0; // ! explicitly set all hex to upper case

        unsigned char c;
        unsigned char b;
        b = (w >> 8);
        if (width == 0|| width > 3)
          {
            c = ( (b >> 4) & 0x0F);
            out.put(c < 10 ? c + '0' : c + 'A'+ upp - 10);
          }
        if (width == 0|| width > 2)
          {
            c = (b & 0x0F);
            out.put(c < 10 ? c + '0' : c + 'A'+ upp - 10);
          }
        b = (w & 0xFF);
        if (width == 0|| width > 1)
          {
            c = ( (b >> 4) & 0x0F);
            out.put(c < 10 ? c + '0' : c + 'A'+ upp - 10);
          }
        c = (b & 0x0F);
        out.put(c < 10 ? c + '0' : c + 'A'+ upp - 10);
      }
    else if ( (out.flags() & ios_base::basefield ) == ios_base::oct)
      {
        ; // not yet implemented
      }
    else // decimal

      {
        if (sign)
          {
            int n;
            n = w;
            if (n < 0)
              {
                out.put( '-');
                w = -n;
              }
          }
        int i;
        unsigned char buff[ 5 ];

        for (i = sizeof(buff ) - 1; i >= 0; i--)
          {
            buff[ i ] = (w % 10) + '0';
            w /= 10;
          }

        if ( 0 < width && width <= ( int ) sizeof(buff ) )
        i = ( int ) sizeof(buff ) - width;
        else
          {
            for (i = 0; i < ( int ) sizeof(buff ) - 1; ++i)
            if (buff[ i ] != '0')
            break;
          }
        for (; i < ( int ) sizeof(buff ); ++i)
        out.put(buff[ i ]);
      }
    out.width( 0);
  }
#endif

ostream&
ostream::operator <<(unsigned short n)
{
  //sentry s( *this );
#if (__SIZEOF_INT__ == 2)
  print16( *this, n );
#else
  print32(*this, (unsigned long) n);
#endif /* (__SIZEOF_INT__ == 2) */
  return *this;
}

ostream&
ostream::operator <<(short n)
{
  //sentry s( *this );
#if (__SIZEOF_INT__ == 2)
  print16( *this, ( unsigned short ) n, true );
#else
  print32(*this, (unsigned long) n, true);
#endif /* (__SIZEOF_INT__ == 2) */
  return *this;
}

ostream&
ostream::operator <<(int n)
{
  //sentry s( *this );
#if (__SIZEOF_INT__ == 2)
  print16( *this, ( unsigned short ) n, true );
#else
  print32(*this, (unsigned long) n, true);
#endif /* (__SIZEOF_INT__ == 2) */
  return *this;
}

ostream&
ostream::operator <<(unsigned int n)
{
  //sentry s( *this );
#if (__SIZEOF_INT__ == 2)
  print16(*this, (unsigned short) n);
#else
  print32( *this, ( unsigned long ) n );
#endif /* (__SIZEOF_INT__ == 2) */
  return *this;
}

#if defined(OS_INCLUDE_OSTREAM_LONG)

ostream& ostream::operator <<( long n )
  {
    //sentry s( *this );
    print32( *this, ( unsigned long ) n, true );
    return *this;
  }

ostream& ostream::operator <<( unsigned long n )
  {
    //sentry s( *this );
    print32( *this, ( unsigned long ) n );
    return *this;
  }

#endif /* defined(OS_INCLUDE_OSTREAM_LONG) */

#if defined(OS_INCLUDE_OSTREAM_FLOAT)

ostream&
ostream::operator <<( float f )
  {
    //sentry s( *this );
    f = f;
    //-- __ostream_printout::printout( *this, f );
    return *this;
  }

ostream&
ostream::operator <<( double f )
  {
    //sentry s( *this );
    f = f;
    //-- __ostream_printout::printout( *this, f );
    return *this;
  }

ostream&
ostream::operator <<( long double f )
  {
    //sentry s( *this );
    f = f;
    //-- __ostream_printout::printout( *this, f );
    return *this;
  }
#endif /* defined(OS_INCLUDE_OSTREAM_FLOAT) */

#if true
ostream&
ostream::operator <<(void *p)
{
  //sentry s( *this );
  p = p;
  //-- char buffer[ 20 ];
  //-- write( buffer, snprintf( buffer, 20, "%p", p ));
#if (__SIZEOF_INT__ == 2)
  print16(*this, (unsigned int) p);
#else
  print32(*this, (unsigned int) p);
#endif /* (__SIZEOF_INT__ == 2) */

  if (ios::flags() & ios_base::unitbuf)
    {
      flush();
    }
  return *this;
}
#endif

#if 0
ostream&
ostream::operator <<( streambuf * sb )
  {
    //sentry s( *this );
    if ( sb == 0 )
      {
        ios::setstate( ios_base::badbit );
        return *this;
      }

    int c;

    while ( ios::good() && ( c = sb->sbumpc() ) != traits::eof() )
      {
        put( c );
      }

    if ( ios::flags() & ios_base::unitbuf )
      {
        flush();
      }
    return *this;
  }
#endif

ostream&
ostream::flush()
{
  /* if ( ios::m_device != 0 )
   {
   if ( ios::m_device->flush() < 0 )
   ios::setstate( ios_base::badbit );
   }
   else */if (ios::m_streambuf != 0)
    {
      if (ios::m_streambuf->pubsync() == -1)
        {
          ios::setstate(ios_base::badbit);
        }
    }
  return *this;
}

ostream&
ostream::put(char c)
{
  /* if ( ios::m_debug != 0 )
   {
   if ( ios::m_debug->writeByte( ( unsigned char ) c ) < 0 )
   ios::setstate( ios_base::eofbit );
   }
   else if ( ios::m_device != 0 )
   {
   if ( ios::m_device->writeByte( ( unsigned char ) c ) < 0 )
   ios::setstate( ios_base::eofbit );
   }

   else */if (ios::m_streambuf != 0)
    {
      if (traits::eq_int(ios::m_streambuf->sputc(c), traits::eof()))
        {
          ios::setstate(ios_base::eofbit);
        }
    }

  return *this;
}

ostream&
ostream::write(const char* s, streamsize n)
{
  /* f ( ios::m_debug != 0 )
   {
   for ( int i = 0; i < n; ++i )
   if ( ios::m_debug->writeByte( ( unsigned char ) *s++ ) < 0 )
   {
   ios::setstate( ios_base::eofbit );
   break;
   }
   }
   else if ( ios::m_device != 0 )
   {
   for ( int i = 0; i < n; ++i )
   if ( ios::m_device->writeByte( ( unsigned char ) *s++ ) < 0 )
   {
   ios::setstate( ios_base::eofbit );
   break;
   }
   }
   else */if (ios::m_streambuf != 0)
    {
      if (traits::eq_int(ios::m_streambuf->sputn(s, n), traits::eof()))
        {
          ios::setstate(ios_base::eofbit);
        }
    }

  return *this;
}

//----------------------------------------------------------------------------

#if 0
ostream::sentry::sentry( ostream & os ) : ok_( true )
  {
    if ( os.good() != 0 )
      {
        ; //Prepare for output
      }

    //Flush any tied buffer
    if ( os.tie() != 0 )
      {
        os.tie() ->flush();
      }
    ; //Add custom prefix code here

  };

ostream::sentry::~sentry()
  {
    ; //Add custom suffix code here
  }
#endif

//----------------------------------------------------------------------------
// Non - class functions

ostream&
operator <<(ostream & out, char c)
{
  //ostream::sentry s( out );
  out.put(c);
  return out;
}

// signed and unsigned
ostream&
operator <<(ostream & out, signed char c)
{
  //ostream::sentry s( out );
  out.put(c);
  return out;
}

ostream&
operator <<(ostream & out, unsigned char c)
{
  //ostream::sentry s( out );
  out.put(c);
  return out;
}

// partial specializations
ostream&
operator <<(ostream & out, const char *c)
{
  //ostream::sentry s( out );
  out.write(c, traits::length(c));
  return out;
}

//  signed and unsigned
ostream&
operator <<(ostream & out, const signed char *c)
{
  //ostream::sentry s( out );
  out.write(reinterpret_cast<const char *> (c), traits::length(
      reinterpret_cast<const char *> (c)));
  return out;
}

ostream&
operator <<(ostream & out, const unsigned char *c)
{
  out.write(reinterpret_cast<const char *> (c), traits::length(
      reinterpret_cast<const char *> (c)));
  return out;
}

ostream&
endl(ostream & os)
{
  //ostream::sentry s( os );
  os.put('\r');
  os.put('\n');
  os.flush();
  return os;
}

ostream&
ends(ostream & os)
{
  //ostream::sentry s( os );
  os.put(traits::eos());
  return os;
}

ostream&
flush(ostream & os)
{
  //ostream::sentry s( os );
  os.flush();
  return os;
}

#endif
