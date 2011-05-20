/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 * 	Based on open source code with Embedded C++ constraints.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_ISTREAM)

#include "portable/devices/debug/include/OSDeviceDebug.h"
#include "portable/stdlib/include/istream"
#include <ctype.h>

void __skipws(istream& is);

istream::istream(streambuf* sb)
  {
    OSDeviceDebug::putConstructor_P(PSTR("istream"), this);
    
    count_last_ufmt_input = 0;
    init(sb /*, 0, 0 */);
  }

#if 0
istream::istream(OSDeviceCharacter* dev)
  {
    OSDeviceDebug::putConstructor_P(PSTR("istream"), this);
    
    count_last_ufmt_input = 0;
    init(0 /*, dev, 0 */);
  }
#endif

istream::~istream()
  {
  OSDeviceDebug::putDestructor_P(PSTR("istream"), this);
  }

#if 0
istream&
istream::get(char* s, streamsize n)
  {
    return get(s, n, '\n');
  }

istream&
istream::get(char* s, streamsize n, char delim)
  {
    //sentry(*this, true);

    streamsize i = 0;
    int c;
    for (i = 0;i < n;++i)
      {
        c = m_streambuf->sgetc();
        m_streambuf->sbumpc();
        if (c == traits::eof())
          {
            if (i == 0)
              {
                setstate(ios_base::failbit);
              }
            else
              {
                setstate(ios_base::eofbit);
              }
            count_last_ufmt_input = i;
            return *this;
          }
        if (c == delim)
          {
            if (i == 0)
              {
                setstate(ios_base::failbit);
              }
            m_streambuf->sputbackc(c);
            count_last_ufmt_input = i;
            return *this;
          }
        s[i] = c;
      }
    return *this;
  }
#endif

#if 0
istream&
istream::get(streambuf& sb, char delim)
  {
    //sentry(*this, true);
    streamsize i = 0;
    int c;
    while (1)
      { //We will exit internally based upon error conditions
        c = m_streambuf->sgetc();
        if (c == traits::eof())
          {
            if (i == 0)
              {
                setstate(ios_base::failbit);
              }
            else
              {
                setstate(ios_base::eofbit);
              }
            count_last_ufmt_input = i;
            return *this;
          }
        if (c == delim)
          {
            if (i == 0)
              {
                setstate(ios_base::failbit);
              }
            count_last_ufmt_input = i;
            return *this;
          }
        if (sb.sputc(c) != c)
          { //Error doing output
            count_last_ufmt_input = i;
            return *this;
          }
        ++i;
        m_streambuf->sbumpc();
      }
  }

istream&
istream::get(streambuf& sb)
  {
    return get(sb, '\n');
  }

istream&
istream::getline(char* s, streamsize n, char delim)
  {
    //sentry(*this, true);

    streamsize i = 0;
    int c;
    for (i = 0;i < n - 1;++i)
      {
        c = m_streambuf->sgetc();
        if (c == traits::eof())
          {
            if (eof())
              {
                setstate(ios_base::failbit);
              }
            else
              {
                setstate(ios_base::eofbit);
              }
            count_last_ufmt_input = i;
            s[i] = traits::eos();
            return *this;
          }
        if (m_streambuf->sbumpc() == traits::eof())
          {
            setstate(ios_base::eofbit);
          }
        if (c == delim)
          {
            count_last_ufmt_input = i + 1;
            s[i] = traits::eos();
            return *this;
          }
        s[i] = c;
      }
    s[n] = traits::eos();
    return *this;
  }

istream&
istream::ignore(streamsize n, int delim)
  {
    //sentry(*this, true);

    streamsize i;
    int c;
    for (i = 0;i < n;++i)
      {
        c = m_streambuf->sgetc();
        if (c == traits::eof())
          {
            setstate(ios_base::eofbit);
            return *this;
          }
        m_streambuf->sbumpc();
        if (c == delim)
          {
            return * this;
          }
      }
    return *this;
  }

int
istream::peek(void)
  {
    if (good() == false)
      {
        return traits::eof();
      }
    else
      {
        int c = m_streambuf->sgetc();
        if (c == traits::eof())
          {
            setstate(ios_base::eofbit);
          }
        return m_streambuf->sgetc();
      }
  }

istream&
istream::read(char* s, streamsize n)
  {
    //sentry(*this, true);

    streamsize i;
    int c;
    for (i = 0;i < n;++i)
      {
        c = m_streambuf->sgetc();

        if (c == traits::eof())
          {
            setstate(ios_base::failbit);
            setstate(ios_base::eofbit);
            count_last_ufmt_input = i;
            return *this;
          }
        m_streambuf->sbumpc();
        s[i] = c;
      }
    count_last_ufmt_input = n;
    return *this;
  }

streamsize
istream::readsome(char* s, streamsize n)
  {
    //sentry(*this, true);

    if (!good())
      {
        count_last_ufmt_input = 0;
        setstate(ios_base::failbit);
        return 0;
      }

    if (m_streambuf->in_avail() == -1)
      {
        count_last_ufmt_input = 0;
        setstate(ios_base::eofbit);
        return 0;
      }

    if (n> m_streambuf->in_avail())
      {
        n = m_streambuf->in_avail();
      }

    streamsize i;
    int c;

    for (i = 0;i < n;++i)
      {
        c = m_streambuf->sgetc();
        m_streambuf->sbumpc();
        s[i] = c;
      }
    count_last_ufmt_input = n;
    return n;
  }

istream&
istream::putback(char c)
  {
    //sentry(*this, true);

    if (!good())
      {
        setstate(ios_base::failbit);
        return *this;
      }
    if (m_streambuf == 0)
      {
        setstate(ios_base::badbit);
        return *this;
      }
    if (m_streambuf->sputbackc(c) == traits::eof())
      {
        setstate(ios_base::badbit);
        return *this;
      }
    return *this;
  }

istream&
istream::unget(void)
  {
    //sentry(*this, true);

    if (!good())
      {
        setstate(ios_base::failbit);
        return *this;
      }
    if (m_streambuf == 0)
      {
        setstate(ios_base::failbit);
        return *this;
      }
    if (m_streambuf->sungetc() == traits::eof())
      {
        setstate(ios_base::failbit);
      }
    return *this;
  }

int
istream::sync(void)
  {
    //sentry(*this, true);

    if (m_streambuf == 0)
      {
        return -1;
      }
    if (m_streambuf->pubsync() == -1)
      {
        setstate(ios_base::badbit);
        return traits::eof();
      }
    return 0;
  }

pos_type
istream::tellg(void)
  {
    if (fail() != false)
      {
        return pos_type(-1);
      }
    return m_streambuf->pubseekoff(0, ios_base::cur, ios_base::in);
  }

istream&
istream::seekg(pos_type pos)
  {
    if (fail() != true)
      {
        m_streambuf->pubseekpos(pos);
      }
    return *this;
  }

istream&
istream::seekg(off_type off, ios_base::seekdir dir)
  {
    if (fail() != true)
      {
        m_streambuf->pubseekoff(off, dir);
      }
    return *this;
  }
#endif

int istream::get()
  {
    //sentry(*this, true);

    //OSDeviceDebug::putChar('^');
    int retval;
    retval = m_streambuf->sgetc();
    //OSDeviceDebug::putHex((unsigned char)retval);

    if (retval == traits::eof())
      {
        count_last_ufmt_input = 0;
        setstate(ios_base::eofbit);
      }
    else
      {
        count_last_ufmt_input = 1;
        m_streambuf->sbumpc();
      }
    return retval;
  }

istream& istream::get(char& c)
  {
    //sentry(*this, true);


    int retval;
    /* if (m_device != 0)
     {
     retval = m_device->readByte();
     c = traits::to_char(retval);
     // TODO: process errors
     }
     else */
    retval = m_streambuf->sgetc();
    if (retval == traits::eof())
      {
        count_last_ufmt_input = 0;
        setstate(ios_base::eofbit);
        setstate(ios_base::failbit);
      }
    else
      {
        count_last_ufmt_input = 1;
        c = traits::to_char(retval);
        m_streambuf->sbumpc();
      }
    return *this;
  }

#if 0
istream&
istream::operator>>(bool& n __attribute__((unused)))
  {
    //sentry(*this);
    //__istream_readin<traits, charT, bool>::readin(*this, n);
    return * this;
  }

istream&
istream::operator>>(short& n __attribute__((unused)))
  {
    //sentry(*this);
    //__istream_readin<traits, charT, short>::readin(*this, n);
    return * this;
  }

istream&
istream::operator>>(unsigned short& n __attribute__((unused)))
  {
    //sentry(*this);
    //__istream_readin<traits, charT, unsigned short>::readin(*this, n);
    return * this;
  }

istream&
istream::operator>>(int& n __attribute__((unused)))
  {
    //sentry(*this);
    //__istream_readin<traits, charT, int>::readin(*this, n);
    return * this;
  }

istream&
istream::operator>>(unsigned int& n __attribute__((unused)))
  {
    //sentry(*this);
    //__istream_readin<traits, charT, unsigned int>::readin(*this, n);
    return * this;
  }
#endif

#if defined(OS_INCLUDE_ISTREAM_LONG)

istream&
istream::operator>>(long int& n)
  {
    sentry(*this);
    //__istream_readin<traits, charT, long int>::readin(*this, n);
    return *this;
  }

istream&
istream::operator>>(unsigned long int& n)
  {
    sentry(*this);
    //__istream_readin<traits, charT, unsigned long int>::readin(*this, n);
    return *this;
  }
#endif

#if defined(OS_INCLUDE_ISTREAM_FLOAT)

istream&
istream::operator>>(float& n)
  {
    sentry(*this);
    //__istream_readin<traits, charT, float>::readin(*this, n);
    return *this;
  }

istream&
istream::operator>>(double& n)
  {
    sentry(*this);
    //__istream_readin<traits, charT, double>::readin(*this, n);
    return *this;
  }

istream&
istream::operator>>(long double& n)
  {
    sentry(*this);
    //__istream_readin<traits, charT, long double>::readin(*this, n);
    return *this;
  }
#endif

istream& istream::operator>>(istream&(*pf) (istream&))
  {
    //sentry(*this);
    pf(*this);
    return *this;
  }

istream& istream::operator>>(ios&(*pf) (ios&))
  {
    //sentry(*this);
    pf(*this);
    return *this;
  }

//----------------------------------------------------------------------------
#if 0
istream::sentry::sentry(istream& os, bool noskipws) : ok_(true)
  {
    if (os.good() != 0)
      { //Prepare for output
        ;
      }

    //Flush any tied buffer
    if (os.tie() != 0)
      {
        os.tie() ->flush();
      }
    if (!noskipws)
      {
        __skipws(os);
      }
  }

istream::sentry::~sentry()
  {
    ;
  }
#endif

//----------------------------------------------------------------------------
// Non - class functions

#if 0
istream&
istream::operator>>(void*& n __attribute__((unused)))
  {
    //sentry(*this);
    //__istream_readin<traits, charT, void*>::readin(*this, n);
    return * this;
  }
#endif

istream& operator>>(istream& is, char& c)
  {
    //istream::sentry s(is);
    is.get(c);
    return is;
  }

istream& operator>>(istream& is, unsigned char& c)
  {
    //istream::sentry s(is);

    char b;
    is.get(b);
    c = b;
    return is;
  }

istream& operator>>(istream& is, signed char& c)
  {
    //istream::sentry s(is);
    is.get((char&) c);
    return is;
  }

#if 0
istream&
operator>>(istream& is, char* c)
  {
    //istream::sentry s(is);
    int n = is.width();
    if (n == 0)
      {
        n = __STRING_MAX_UNITS;
      }
    is.get(c, n);
    return is;

  }

istream&
operator>>(istream& is, unsigned char* c)
  {
    //istream::sentry s(is);
    int n = is.width();
    if (n == 0)
      {
        n = __STRING_MAX_UNITS;
      }
    is.get((char*) c, n);
    return is;
  }

istream&
operator>>(istream& is, signed char* c)
  {
    //istream::sentry s(is);
    int n = is.width();
    if (n == 0)
      {
        n = __STRING_MAX_UNITS;
      }
    is.get((char*) c, n);
    return is;
  }
#endif

#if 0
void __skipws(istream& is);

istream&
ws(istream& is)
  {
    __skipws(is);
    return is;
  }

void __skipws(istream& is)
  {
    const int eof = traits::eof();
    int c;
    //While the next character normally read doesn't equal eof
    //and that character is a space, advance to the next read position
    //Thus itterating through al whitespace until we get to the meaty stuff
    while (
        !traits::eq_int((c = is.rdbuf() ->sgetc()), eof)
        && isspace(c)
  )
      {
        is.rdbuf() ->sbumpc();
      }
    if (traits::eq_int(c, eof))
      {
        is.setstate(ios_base::eofbit);
      }
  }
#endif

#endif
