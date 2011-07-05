/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef GPSPOSITION_H_
#define GPSPOSITION_H_

#include "portable/kernel/include/uOS.h"

#if defined(OS_INCLUDE_OSTREAM_GPSPOSITION)
#include "portable/stdlib/include/ostream"
#endif /* defined(OS_INCLUDE_OSTREAM_GPSPOSITION) */

typedef int16_t GpsAltitude_t; // +/- 32Km
typedef uchar_t GpsHemisphere_t; // N/S, E/W
typedef uint8_t GpsDegrees_t; // 0-179
typedef uint8_t GpsMinutesInteger_t; // 0-59
typedef uint16_t GpsMinutesDecimals_t; // 0-9999
typedef uint16_t GpsFix_t;

class GpsCoordinate;
typedef GpsCoordinate GpsCoordinate_t;

class GpsLatitude;
typedef GpsLatitude GpsLatitude_t;

class GpsLongitude;
typedef GpsLongitude GpsLongitude_t;

class GpsPosition;
typedef GpsPosition GpsPosition_t;

// ----------------------------------------------------------------------------

class GpsCoordinate
{
public:
  GpsCoordinate();
  ~GpsCoordinate();

  GpsHemisphere_t
  getHemisphere(void) const;
  void
  setHemisphere(GpsHemisphere_t hemisphere);

  GpsDegrees_t
  getDegrees() const;
  void
  setDegrees(GpsDegrees_t degrees);

  GpsMinutesInteger_t
  getMinutesInteger() const;
  void
  setMinutesInteger(GpsMinutesInteger_t minutesInteger);

  GpsMinutesDecimals_t
  getMinutesDecimals() const;
  void
  setMinutesDecimals(GpsMinutesDecimals_t minutesDecimals);

private:
  GpsHemisphere_t m_hemisphere; // N/S, E/W
  GpsDegrees_t m_degrees; // 0-89, 0-179
  GpsMinutesInteger_t m_minutesInteger; // 0-59
  GpsMinutesDecimals_t m_minutesDecimals; // 0-9999
};

// ----------------------------------------------------------------------------

class GpsLatitude : public GpsCoordinate
{
public:
  GpsLatitude();
  ~GpsLatitude();

  bool
  isHemispereNorth(void);
  bool
  isHemispereSouth(void);

  void
  setHemisphereNorth(void);
  void
  setHemisphereSouth(void);
};

// ----------------------------------------------------------------------------

class GpsLongitude : public GpsCoordinate
{
public:
  GpsLongitude();
  ~GpsLongitude();

  bool
  isHemispereEst(void);
  bool
  isHemispereWest(void);

  void
  setHemisphereEst(void);
  void
  setHemisphereWest(void);
};

// ----------------------------------------------------------------------------

#if defined(OS_INCLUDE_OSTREAM_GPSPOSITION)

std::ostream&
operator <<(std::ostream& out, const GpsCoordinate& coordinate);

#endif /* defined(OS_INCLUDE_OSTREAM_GPSPOSITION) */

// ----------------------------------------------------------------------------

class GpsPosition
{
public:
  GpsPosition();
  ~GpsPosition();

  GpsLatitude_t&
  getLatitude();

  GpsLongitude_t&
  getLongitude();

  GpsAltitude_t
  getAltitude() const;
  void
  setAltitude(GpsAltitude_t m_altitude);

  GpsFix_t
  getFix();

  void
  setFix(GpsFix_t fix);

private:
  GpsLatitude_t m_latitude; // N/S
  GpsLongitude_t m_longitude; // E/W
  GpsAltitude_t m_altitude; // altitude in meters, may be negative
  GpsFix_t m_fix;
};

// ----------------------------------------------------------------------------

#if defined(OS_INCLUDE_OSTREAM_GPSPOSITION)

std::ostream&
operator <<(std::ostream& out, GpsPosition& position);

#endif /* defined(OS_INCLUDE_OSTREAM_GPSPOSITION) */

// ----------------------------------------------------------------------------

inline GpsHemisphere_t
GpsCoordinate::getHemisphere(void) const
{
  return m_hemisphere;
}

inline void
GpsCoordinate::setHemisphere(GpsHemisphere_t hemisphere)
{
  m_hemisphere = hemisphere;
}

inline GpsDegrees_t
GpsCoordinate::getDegrees() const
{
  return m_degrees;
}

inline void
GpsCoordinate::setDegrees(GpsDegrees_t degrees)
{
  m_degrees = degrees;
}

inline GpsMinutesInteger_t
GpsCoordinate::getMinutesInteger() const
{
  return m_minutesInteger;
}

inline void
GpsCoordinate::setMinutesInteger(GpsMinutesInteger_t minutesInteger)
{
  m_minutesInteger = minutesInteger;
}

inline GpsMinutesDecimals_t
GpsCoordinate::getMinutesDecimals() const
{
  return m_minutesDecimals;
}

inline void
GpsCoordinate::setMinutesDecimals(GpsMinutesDecimals_t minutesDecimals)
{
  m_minutesDecimals = minutesDecimals;
}

// ----------------------------------------------------------------------------

inline bool
GpsLatitude::isHemispereNorth(void)
{
  return (getHemisphere() == 'N');
}
inline bool
GpsLatitude::isHemispereSouth(void)
{
  return (getHemisphere() == 'S');
}

inline void
GpsLatitude::setHemisphereNorth(void)
{
  setHemisphere('N');
}
inline void
GpsLatitude::setHemisphereSouth(void)
{
  setHemisphere('S');
}

// ----------------------------------------------------------------------------

inline bool
GpsLongitude::isHemispereEst(void)
{
  return (getHemisphere() == 'E');
}
inline bool
GpsLongitude::isHemispereWest(void)
{
  return (getHemisphere() == 'W');
}

inline void
GpsLongitude::setHemisphereEst(void)
{
  setHemisphere('E');
}
inline void
GpsLongitude::setHemisphereWest(void)
{
  setHemisphere('W');
}

// ----------------------------------------------------------------------------

inline GpsLatitude_t&
GpsPosition::getLatitude()
{
  return m_latitude;
}

inline GpsLongitude_t&
GpsPosition::getLongitude()
{
  return m_longitude;
}

inline GpsAltitude_t
GpsPosition::getAltitude() const
{
  return m_altitude;
}

inline void
GpsPosition::setAltitude(GpsAltitude_t altitude)
{
  m_altitude = altitude;
}

inline GpsFix_t
GpsPosition::getFix()
{
  return m_fix;
}

inline void
GpsPosition::setFix(GpsFix_t fix)
{
  m_fix = fix;
}

// ----------------------------------------------------------------------------

#endif /* GPSPOSITION_H_ */
