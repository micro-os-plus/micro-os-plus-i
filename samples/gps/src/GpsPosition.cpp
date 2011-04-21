/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "GpsPosition.h"

// ----------------------------------------------------------------------------

GpsCoordinate::GpsCoordinate()
{
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
  debug.putString("GpsCoordinate()=");
  debug.putPtr(this);
  debug.putNewLine();
#endif
}

GpsCoordinate::~GpsCoordinate()
{
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
  debug.putString("~GpsCoordinate()");
  debug.putNewLine();
#endif
}

ostream&
operator <<(ostream& out, GpsCoordinate& c)
{
  out << c.getHemisphere() << dec << (unsigned) c.getDegrees() << "°"
      << (unsigned) c.getMinutesInteger() << "." << c.getMinutesDecimals()
      << "'";

  return out;
}

// ----------------------------------------------------------------------------

GpsPosition::GpsPosition()
{
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
  debug.putString("GpsPosition()=");
  debug.putPtr(this);
  debug.putNewLine();
#endif
}

GpsPosition::~GpsPosition()
{
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
  debug.putString("~GpsPosition()");
  debug.putNewLine();
#endif
}

ostream&
operator <<(ostream& out, GpsPosition& p)
{
  out << *p.getLatitude() << ", " << *p.getLongitude() << ", "
      << p.getAltitude() << "m";

  return out;
}

// ----------------------------------------------------------------------------
