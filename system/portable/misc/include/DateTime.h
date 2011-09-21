/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef DATETIME_H_
#define DATETIME_H_

#include "portable/kernel/include/uOS.h"

#if !defined(OS_CFGINT_DATETIME_EPOCH_YEAR)
#define OS_CFGINT_DATETIME_EPOCH_YEAR 2010
#endif

// ----------------------------------------------------------------------------

class DateTime
{
public:

  // Names are singular, since they do not represent durations
  typedef uint8_t Second_t; // 0-59
  typedef uint8_t Minute_t; // 0-59
  typedef uint8_t Hour_t; // 0-23
  typedef uint8_t Day_t; // 1-31
  typedef uint16_t DayOfYear_t; // 1-366
  typedef uint8_t Month_t; // 1-12
  typedef uint16_t Year_t; // >= EPOCH

  const static uint16_t EPOCH_YEAR = OS_CFGINT_DATETIME_EPOCH_YEAR;

  typedef uint32_t DurationSeconds_t;

  const static Day_t daysInMonths[];

  // ----- Constructors & destructors -----------------------------------------

  DateTime();
  ~DateTime();

  // ----- Public methods -----------------------------------------------------

  // Names are singular, since they do not return durations
  Second_t
  getSecond(void);

  Minute_t
  getMinute(void);

  Hour_t
  getHour();

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

  DurationSeconds_t
  computeSecondsFromEpoch(void);

  // January 1st is day 1
  DayOfYear_t
  computeDayOfYear(void);

  bool
  isLeapYear(void);

private:
  Second_t m_second; // 0-59
  Minute_t m_minute; // 0-59
  Hour_t m_hour; // 0-23
  Day_t m_day; // 1-31
  Month_t m_month; // 1-12
  Year_t m_year; // >= EPOCH
};

inline DateTime::Second_t
DateTime::getSecond(void)
{
  return m_second;
}

inline DateTime::Minute_t
DateTime::getMinute(void)
{
  return m_minute;
}

inline DateTime::Hour_t
DateTime::getHour()
{
  return m_hour;
}

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

// ----------------------------------------------------------------------------

#endif /* DATETIME_H_ */
