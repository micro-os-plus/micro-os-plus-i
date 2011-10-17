/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_GPSPOSITION)

#include "portable/misc/include/GpsPosition.h"

// ----------------------------------------------------------------------------

GpsCoordinate::GpsCoordinate()
{
  debug.putConstructor_P(PSTR("GpsCoordinate"), this);

  m_degrees = 0;
  m_minutesInteger = 0;
  m_minutesDecimals = 0;
}

GpsCoordinate::~GpsCoordinate()
{
  debug.putDestructor_P(PSTR("GpsCoordinate"), this);
}

OSReturn_t
GpsCoordinate::parseDegrees(const char* pString, size_t length)
{
  // Parse longitude 'ddd' (0-179) or latitude 'dd' (0-79)
  for (size_t i = 0; i < length; ++i)
    {
      uchar_t ch;
      ch = *pString++;

      uint8_t b;
      if (('0' <= ch) && (ch <= '9'))
        {
          b = ch - '0';
        }
      else
        {
          OSDeviceDebug::putString(" BAD_PARAMETER_(deg) ");
          return OSReturn::OS_BAD_PARAMETER;
        }

      if (i == 0)
        {
          m_degrees = b;
        }
      else
        {
          // Normally there are only 2-3 iterations here
          m_degrees = m_degrees * 10 + b;
        }
    }

  return OSReturn::OS_OK;
}

OSReturn_t
GpsCoordinate::parseMinutes(const char* pString)
{
  // Parse minutes with decimals 'mm.mmmm'
  for (size_t i = 0; i < 7; ++i)
    {
      uchar_t ch;
      ch = *pString++;

      uint8_t b;
      if (('0' <= ch) && (ch <= '9'))
        {
          b = ch - '0';
        }
      else if (i != 2)
        {
          OSDeviceDebug::putString(" BAD_PARAMETER_(min) ");
          return OSReturn::OS_BAD_PARAMETER;
        }

      switch (i)
        {
      case 0:
        m_minutesInteger = b;
        break;

      case 1:
        m_minutesInteger = m_minutesInteger * 10 + b;
        break;

      case 2:
        if (ch != '.')
          {
            OSDeviceDebug::putString(" BAD_PARAMETER_(min.) ");
            return OSReturn::OS_BAD_PARAMETER;
          }
        break;

      case 3:
        m_minutesDecimals = b;
        break;

      case 4:
      case 5:
      case 6:
        m_minutesDecimals = m_minutesDecimals * 10 + b;
        break;
        }
    }
  return OSReturn::OS_OK;
}

#if defined(OS_INCLUDE_OSTREAM_GPSPOSITION)

std::ostream&
operator <<(std::ostream& out, const GpsCoordinate& coordinate)
{
  out << coordinate.getHemisphere() << std::dec
      << (uint_t) coordinate.getDegrees() << "°";
  out.width(2);
  out << (uint_t) coordinate.getMinutesInteger() << ".";
  out.width(4);
  out << coordinate.getMinutesDecimals() << "'";

  return out;
}

#endif /* defined(OS_INCLUDE_OSTREAM_GPSPOSITION) */

// ----------------------------------------------------------------------------

GpsLatitude::GpsLatitude()
{
  debug.putConstructor("GpsLatitude", this);

  setHemisphereNorth();
}

GpsLatitude::~GpsLatitude()
{
  debug.putDestructor("GpsLatitude", this);
}

OSReturn_t
GpsLatitude::parseNmea(const char* pString, const char* pHemisphere)
{
  OSReturn_t ret;

  ret = parseDegrees(pString, 2);
  if (ret == OSReturn::OS_OK)
    {
      ret = parseMinutes(pString + 2);
    }

  setHemisphere(pHemisphere[0]);

  return ret;
}

// ----------------------------------------------------------------------------

GpsLongitude::GpsLongitude()
{
  debug.putConstructor("GpsLongitude", this);

  setHemisphereEst();
}

GpsLongitude::~GpsLongitude()
{
  debug.putDestructor("GpsLongitude", this);
}

OSReturn_t
GpsLongitude::parseNmea(const char* pString, const char* pHemisphere)
{
  OSReturn_t ret;

  ret = parseDegrees(pString, 3);
  if (ret == OSReturn::OS_OK)
    {
      ret = parseMinutes(pString + 3);
    }

  setHemisphere(pHemisphere[0]);

  return ret;
}

// ----------------------------------------------------------------------------

GpsPosition::GpsPosition()
{
  debug.putConstructor("GpsPosition", this);

  m_hasAltitude = false;
  m_fixStatus = 0;
  m_satellites = 0;
  m_hdop = 0;
}

GpsPosition::~GpsPosition()
{
  debug.putDestructor("GpsPosition", this);
}

void
GpsPosition::setAltitude(GpsAltitude_t altitude)
{
  m_altitude = altitude;
  m_hasAltitude = true;
}

#if defined(OS_INCLUDE_OSTREAM_GPSPOSITION)

std::ostream&
operator <<(std::ostream& out, GpsPosition& position)
{
  out << position.getLatitude() << ", " << position.getLongitude();

  if (position.hasAltitude())
    {
      out << ", " << (position.getAltitude() / 10) << "."
          << (position.getAltitude() % 10) << " m";
    }

  if (position.getHdop() != 0)
    {
      out << ", " << (position.getHdop() / 100) << ".";
      out.width(2);
      out << (position.getHdop() % 100);
      out << " hdop";
    }

  return out;
}

#endif /* defined(OS_INCLUDE_OSTREAM_GPSPOSITION) */

// ----------------------------------------------------------------------------

#endif /* defined(OS_INCLUDE_GPSPOSITION) */
