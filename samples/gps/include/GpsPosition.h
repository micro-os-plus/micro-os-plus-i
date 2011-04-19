/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef GPSPOSITION_H_
#define GPSPOSITION_H_

#include "portable/kernel/include/uOS.h"

#include "portable/stdlib/include/ostream"

typedef int gpsAltitude_t;
typedef unsigned char gpsHemisphere_t;
typedef unsigned char gpsDegrees_t;
typedef unsigned char gpsMinutesInteger_t;
typedef unsigned short gpsMinutesDecimals_t;

class GpsCoordinate;
typedef GpsCoordinate gpsCoordinate_t;

class GpsPosition;
typedef GpsPosition gpsPosition_t;

// ----------------------------------------------------------------------------

class GpsCoordinate
{
public:
  GpsCoordinate();

  gpsHemisphere_t
  getHemisphere(void) const;
  void
  setHemisphere(gpsHemisphere_t hemisphere);

  gpsDegrees_t
  getDegrees() const;
  void
  setDegrees(gpsDegrees_t degrees);

  gpsMinutesInteger_t
  getMinutesInteger() const;
  void
  setMinutesInteger(gpsMinutesInteger_t minutesInteger);

  gpsMinutesDecimals_t
  getMinutesDecimals() const;
  void
  setMinutesDecimals(gpsMinutesDecimals_t minutesDecimals);

private:
  gpsHemisphere_t m_hemisphere; // N/S, E/W
  gpsDegrees_t m_degrees; // 0-89, 0-179
  gpsMinutesInteger_t m_minutesInteger; // 0-59
  gpsMinutesDecimals_t m_minutesDecimals; // 0-9999
};

// ----------------------------------------------------------------------------

ostream&
operator <<(ostream& out, GpsCoordinate& p);

// ----------------------------------------------------------------------------

class GpsPosition
{
public:
  GpsPosition();

  gpsCoordinate_t*
  getLatitude();

  gpsCoordinate_t*
  getLongitude();

  gpsAltitude_t
  getAltitude() const;
  void
  setAltitude(gpsAltitude_t m_altitude);

private:
  gpsCoordinate_t m_latitude; // N/S
  gpsCoordinate_t m_longitude; // E/W
  gpsAltitude_t m_altitude; // altitude in meters, may be negative
};

// ----------------------------------------------------------------------------

ostream&
operator <<(ostream& out, GpsPosition& p);

// ----------------------------------------------------------------------------

inline gpsHemisphere_t
GpsCoordinate::getHemisphere(void) const
{
  return m_hemisphere;
}

inline void
GpsCoordinate::setHemisphere(gpsHemisphere_t hemisphere)
{
  m_hemisphere = hemisphere;
}

inline gpsDegrees_t
GpsCoordinate::getDegrees() const
{
  return m_degrees;
}

inline void
GpsCoordinate::setDegrees(gpsDegrees_t degrees)
{
  m_degrees = degrees;
}

inline gpsMinutesInteger_t
GpsCoordinate::getMinutesInteger() const
{
  return m_minutesInteger;
}

inline void
GpsCoordinate::setMinutesInteger(gpsMinutesInteger_t minutesInteger)
{
  m_minutesInteger = minutesInteger;
}

inline gpsMinutesDecimals_t
GpsCoordinate::getMinutesDecimals() const
{
  return m_minutesDecimals;
}

inline void
GpsCoordinate::setMinutesDecimals(gpsMinutesDecimals_t minutesDecimals)
{
  m_minutesDecimals = minutesDecimals;
}

// ----------------------------------------------------------------------------

inline gpsCoordinate_t*
GpsPosition::getLatitude()
{
  return &m_latitude;
}

inline gpsCoordinate_t*
GpsPosition::getLongitude()
{
  return &m_longitude;
}

inline gpsAltitude_t
GpsPosition::getAltitude() const
{
  return m_altitude;
}

inline void
GpsPosition::setAltitude(gpsAltitude_t altitude)
{
  m_altitude = altitude;
}

// ----------------------------------------------------------------------------

#endif /* GPSPOSITION_H_ */
