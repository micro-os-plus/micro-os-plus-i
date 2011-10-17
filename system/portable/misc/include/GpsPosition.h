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

typedef int32_t GpsAltitude_t; // +/- 32Km * 10
typedef uchar_t GpsHemisphere_t; // N/S, E/W
typedef uint8_t GpsDegrees_t; // 0-179 (0-89)
typedef uint8_t GpsMinutesInteger_t; // 0-59
typedef uint16_t GpsMinutesDecimals_t; // 0-9999
typedef uint8_t GpsFixStatus_t;
typedef uint8_t GpsSatellitesInView_t;
typedef uint_t GpsHdop_t; // >1, * 100

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

protected:

  OSReturn_t
  parseDegrees(const char* pString, size_t length);

  OSReturn_t
  parseMinutes(const char* pString);

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

  // Parse NMEA usual format ddmm.mmmm,N/S
  OSReturn_t
  parseNmea(const char* pString, const char* pHemisphere);

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

  // Parse NMEA usual format dddmm.mmmm,E/W
  OSReturn_t
  parseNmea(const char* pString, const char* pHemisphere);
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

  bool
  hasAltitude(void);

  GpsFixStatus_t
  getFixStatus();

  void
  setFixStatus(GpsFixStatus_t fix);

  GpsSatellitesInView_t
  getSatellitesInView(void);

  void
  setSatellitesInView(GpsSatellitesInView_t satellites);

  GpsHdop_t
  getHdop(void);

  void
  setHdop(GpsHdop_t hdop);

private:
  GpsLatitude_t m_latitude; // N/S
  GpsLongitude_t m_longitude; // E/W
  GpsAltitude_t m_altitude; // altitude in meters * 10, may be negative
  bool m_hasAltitude;
  GpsFixStatus_t m_fixStatus;

  GpsSatellitesInView_t m_satellites; // 0-12
  GpsHdop_t m_hdop; // HDOP * 100
  // 1 = ideal
  // 1-2 = excellent
  // 2-5 = good
  // 5-10 = moderate
  // 10-20 = fair
  // >20 = poor

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

inline bool
GpsPosition::hasAltitude(void)
{
  return m_hasAltitude;
}

inline GpsFixStatus_t
GpsPosition::getFixStatus()
{
  return m_fixStatus;
}

inline void
GpsPosition::setFixStatus(GpsFixStatus_t fix)
{
  m_fixStatus = fix;
}

inline GpsSatellitesInView_t
GpsPosition::getSatellitesInView(void)
{
  return m_satellites;
}

inline void
GpsPosition::setSatellitesInView(GpsSatellitesInView_t satellites)
{
  m_satellites = satellites;
}

inline GpsHdop_t
GpsPosition::getHdop(void)
{
  return m_hdop;
}

inline void
GpsPosition::setHdop(GpsHdop_t hdop)
{
  m_hdop = hdop;
}

// ----------------------------------------------------------------------------

#endif /* GPSPOSITION_H_ */
