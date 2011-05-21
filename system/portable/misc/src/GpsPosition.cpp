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
  debug.putConstructor("GpsCoordinate", this);

  m_degrees = 0;
  m_minutesInteger = 0;
  m_minutesDecimals = 0;
}

GpsCoordinate::~GpsCoordinate()
{
  debug.putDestructor("GpsCoordinate", this);
}

#if defined(OS_INCLUDE_OSTREAM_GPSPOSITION)

std::ostream&
operator <<(std::ostream& out, const GpsCoordinate& coordinate)
{
  out << coordinate.getHemisphere() << std::dec
      << (uint_t) coordinate.getDegrees() << "°"
      << (uint_t) coordinate.getMinutesInteger() << "."
      << coordinate.getMinutesDecimals() << "'";

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

// ----------------------------------------------------------------------------

GpsPosition::GpsPosition()
{
  debug.putConstructor("GpsPosition", this);
}

GpsPosition::~GpsPosition()
{
  debug.putDestructor("GpsPosition", this);
}

#if defined(OS_INCLUDE_OSTREAM_GPSPOSITION)

std::ostream&
operator <<(std::ostream& out, GpsPosition& position)
{
  out << position.getLatitude() << ", " << position.getLongitude() << ", "
      << position.getAltitude() << "m";

  return out;
}

#endif /* defined(OS_INCLUDE_OSTREAM_GPSPOSITION) */

// ----------------------------------------------------------------------------

#endif /* defined(OS_INCLUDE_GPSPOSITION) */
