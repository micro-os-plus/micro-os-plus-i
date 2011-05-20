/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "GpsPosition.h"

// ----------------------------------------------------------------------------

GpsCoordinate::GpsCoordinate()
{
  debug.putConstructor("GpsCoordinate", this);
}

GpsCoordinate::~GpsCoordinate()
{
  debug.putDestructor("GpsCoordinate", this);
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
  debug.putConstructor("GpsPosition", this);
}

GpsPosition::~GpsPosition()
{
  debug.putDestructor("GpsPosition", this);
}

ostream&
operator <<(ostream& out, GpsPosition& p)
{
  out << *p.getLatitude() << ", " << *p.getLongitude() << ", "
      << p.getAltitude() << "m";

  return out;
}

// ----------------------------------------------------------------------------
