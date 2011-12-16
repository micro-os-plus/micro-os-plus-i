/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef DATETIME_H_
#define DATETIME_H_

#include "portable/kernel/include/OS_Defines.h"

#include "portable/kernel/include/OS_StdTypes.h"
#include "portable/kernel/include/OSReturn.h"

#if !defined(OS_CFGINT_DATETIME_DEFAULT_EPOCH_YEAR)
// With 32 bits, this gives us 135 years, i.e. up to 2145
#define OS_CFGINT_DATETIME_DEFAULT_EPOCH_YEAR 2010
#endif

// ============================================================================

class Time
{
public:

  // Names are singular, since they do not represent durations
  typedef uint16_t Millisecond_t; // 0-999
  typedef uint8_t Second_t; // 0-59
  typedef uint8_t Minute_t; // 0-59
  typedef uint8_t Hour_t; // 0-23

  typedef uint32_t DurationSeconds_t;

  // ----- Constructors & destructors -----------------------------------------

  Time();
  Time(Hour_t hour, Minute_t minute, Second_t second,
      Millisecond_t millisecond = 0);

  ~Time();

  // ----- Public methods -----------------------------------------------------

  void
  setTime(Hour_t hour, Minute_t minute, Second_t second,
      Millisecond_t millisecond = 0);

  // Names are singular, since they do not return durations
  Millisecond_t
  getMillisecond(void);

  Second_t
  getSecond(void);

  Minute_t
  getMinute(void);

  Hour_t
  getHour();

  void
  setMilliecond(Millisecond_t millisecond);

  void
  setSecond(Second_t second);

  void
  setMinute(Minute_t minute);

  void
  setHour(Hour_t hour);

  OSReturn_t
  parseNmeaTime(const char* pTime);

  bool
  areTimeFieldsValid(void);

  // Return true when day change
  bool
  incrementSecond(void);

  void
  serialiseIsoTime(char* pTime, size_t size);

protected:

  Millisecond_t m_millisecond; // 0-999
  Second_t m_second; // 0-59
  Minute_t m_minute; // 0-59
  Hour_t m_hour; // 0-23
};

// ============================================================================

class DateTime : public Time
{
public:

  // Names are singular, since they do not represent durations
  typedef uint8_t Day_t; // 1-31
  typedef uint16_t DayOfYear_t; // 1-366
  typedef uint8_t Month_t; // 1-12
  typedef uint16_t Year_t; // >= EPOCH

  typedef uint32_t DurationDays_t;

  const static Day_t daysInMonths[];

  // ----- Constructors & destructors -----------------------------------------

  DateTime();
  DateTime(Year_t year, Month_t month, Day_t day, Hour_t hour, Minute_t minute,
      Second_t second, Millisecond_t millisecond = 0);
  DateTime(DurationSeconds_t secondsFromEpoch, Millisecond_t millisecond = 0);

  ~DateTime();

  // ----- Public methods -----------------------------------------------------

  // Names are singular, since they do not return durations

  Day_t
  getDay();

  Month_t
  getMonth();

  Year_t
  getYear();

  // Parse dates like YYYYMMDDTHHMMSS into members
  // Return OS_OK or OS_BAD_DATE
  OSReturn_t
  parseIso(const char* pStr);

  OSReturn_t
  parseNmeaDate(const char* pDate);

  DurationSeconds_t
  computeSecondsFromEpoch(void);

  void
  processSecondsFromEpoch(DurationSeconds_t secondsFromEpoch);

  // January 1st is day 1
  DayOfYear_t
  computeDayOfYear(void);

  bool
  isLeapYear(void);

  static bool
  isLeapYear(Year_t year);

  Year_t
  getEpochYear(void);

  bool
  areDateFieldsValid(void);


#if defined(OS_INCLUDE_DATETIME_YEAR_EPOCH)

  void
  setYearEpoch(Year_t year);

#endif /* defined(OS_INCLUDE_DATETIME_YEAR_EPOCH) */

  Year_t
  getDefaultEpochYear(void);

  void
  serialiseIsoDate(char* pDate, size_t size);

  void
  serialiseIsoDateTime(char* pDate, size_t size);

private:

  const static Year_t DEFAULT_EPOCH_YEAR =
      OS_CFGINT_DATETIME_DEFAULT_EPOCH_YEAR;

  Day_t m_day; // 1-31
  Month_t m_month; // 1-12
  Year_t m_year; // >= EPOCH

#if defined(OS_INCLUDE_DATETIME_YEAR_EPOCH)

  Year_t m_yearEpoch;

#endif /* defined(OS_INCLUDE_DATETIME_YEAR_EPOCH) */
};

// ============================================================================

inline Time::Millisecond_t
Time::getMillisecond(void)
{
  return m_millisecond;
}

inline Time::Second_t
Time::getSecond(void)
{
  return m_second;
}

inline Time::Minute_t
Time::getMinute(void)
{
  return m_minute;
}

inline Time::Hour_t
Time::getHour()
{
  return m_hour;
}

inline void
Time::setMilliecond(Millisecond_t millisecond)
{
  m_millisecond = millisecond;
}

inline void
Time::setSecond(Second_t second)
{
  m_second = second;
}

inline void
Time::setMinute(Minute_t minute)
{
  m_minute = minute;
}

inline void
Time::setHour(Hour_t hour)
{
  m_hour = hour;
}

// ============================================================================

inline DateTime::Day_t
DateTime::getDay()
{
  return m_day;
}

inline DateTime::Month_t
DateTime::getMonth()
{
  return m_month;
}

inline DateTime::Year_t
DateTime::getYear()
{
  return m_year;
}

inline DateTime::Year_t
DateTime::getEpochYear(void)
{
#if defined(OS_INCLUDE_DATETIME_YEAR_EPOCH)

  return m_yearEpoch;

#else

  return getDefaultEpochYear();

#endif /* defined(OS_INCLUDE_DATETIME_YEAR_EPOCH) */

}

#if defined(OS_INCLUDE_DATETIME_YEAR_EPOCH)

inline void
DateTime::setYearEpoch(Year_t year)
  {
    m_yearEpoch = year;
  }

#endif /* defined(OS_INCLUDE_DATETIME_YEAR_EPOCH) */

inline DateTime::Year_t
DateTime::getDefaultEpochYear(void)
{
  return DEFAULT_EPOCH_YEAR;
}

inline bool
DateTime::isLeapYear(void)
{
  return isLeapYear(m_year);
}

// ----------------------------------------------------------------------------

#endif /* DATETIME_H_ */
