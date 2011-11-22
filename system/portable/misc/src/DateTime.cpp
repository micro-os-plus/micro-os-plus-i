/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_DATETIME)

#include "portable/misc/include/DateTime.h"
#include "portable/misc/include/DateTime_ostream.h"

#include <stdlib.h>

const DateTime::Day_t DateTime::daysInMonths[] =
  { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

// ============================================================================

Time::Time()
{
  OSDeviceDebug::putConstructor_P(PSTR("Time"), this);

  setTime(0, 0, 0);
}

Time::Time(Hour_t hour,
    Minute_t minute, Second_t second)
{
  OSDeviceDebug::putConstructor_P(PSTR("Time"), this);

  setTime(hour, minute, second);
}

Time::~Time()
{
  OSDeviceDebug::putDestructor_P(PSTR("Time"), this);
}

void
Time::setTime(Hour_t hour, Minute_t minute, Second_t second)
{
  m_second = second;
  m_minute = minute;
  m_hour = hour;
}

bool
Time::areTimeFieldsValid(void)
{
  if ((m_hour > 24) || (m_minute > 60) || (m_second > 60))
    {
      return false;
    }

  return true;
}

bool
Time::incrementSecond(void)
{
  if (++m_second < 60)
    return false;

  m_second = 0;
  if (++m_minute < 60)
    return false;

  m_minute = 0;
  if (++m_hour < 24)
    return false;

  m_hour = 0;
  return true;
}

OSReturn_t
Time::parseNmeaTime(const char* pTime)
{
  DEBUG_ASSERT(pTime != NULL);

  if ((strlen(pTime) != 10) || pTime[6] != '.')
    {
      OSDeviceDebug::putString(" BAD_TIME ");
      return OSReturn::OS_BAD_TIME;
    }

  for (int i = 0; i < 6; ++i)
    {
      uchar_t ch;
      ch = pTime[i];

      uint8_t b;
      if (('0' <= ch) && (ch <= '9'))
        {
          b = ch - '0';
        }
      else
        {
          OSDeviceDebug::putString(" BAD_TIME (ch) ");
          return OSReturn::OS_BAD_TIME;
        }

      switch (i)
        {
      case 0:
        m_hour = b;
        break;

      case 1:
        m_hour = m_hour * 10 + b;
        break;

      case 2:
        m_minute = b;
        break;

      case 3:
        m_minute = m_minute * 10 + b;
        break;

      case 4:
        m_second = b;
        break;

      case 5:
        m_second = m_second * 10 + b;
        break;

        }
    }

  if (!areTimeFieldsValid())
    {
      OSDeviceDebug::putString(" BAD_TIME (fields) ");
      return OSReturn::OS_BAD_TIME;
    }

  OSDeviceDebug::putString(" time ");
  OSDeviceDebug::putDec(m_hour, 2);
  OSDeviceDebug::putChar(':');
  OSDeviceDebug::putDec(m_minute, 2);
  OSDeviceDebug::putChar(':');
  OSDeviceDebug::putDec(m_second, 2);
  OSDeviceDebug::putChar('.');
  OSDeviceDebug::putString(&pTime[7]);
  OSDeviceDebug::putChar(' ');

  return OSReturn::OS_OK;
}

// ============================================================================

DateTime::DateTime()
{
  OSDeviceDebug::putConstructor_P(PSTR("DateTime"), this);

  m_second = 0;
  m_minute = 0;
  m_hour = 0;
  m_day = 0;
  m_month = 0;
  m_year = DEFAULT_EPOCH_YEAR;
}

DateTime::DateTime(Year_t year, Month_t month, Day_t day, Hour_t hour,
    Minute_t minute, Second_t second)
{
  OSDeviceDebug::putConstructor_P(PSTR("DateTime"), this);

  m_second = second;
  m_minute = minute;
  m_hour = hour;
  m_day = day;
  m_month = month;
  m_year = year;
}

DateTime::DateTime(DurationSeconds_t secondsFromEpoch)
{
  OSDeviceDebug::putConstructor_P(PSTR("DateTime"), this);

  processSecondsFromEpoch(secondsFromEpoch);
}

DateTime::~DateTime()
{
  OSDeviceDebug::putDestructor_P(PSTR("DateTime"), this);
}

// Parse dates like YYYYMMDDTHHMMSS into members
// Return OS_OK or OS_BAD_DATE

OSReturn_t
DateTime::parseIso(const char* pStr)
{
  DEBUG_ASSERT(pStr != NULL);

  if ((strlen(pStr) != 15) || pStr[8] != 'T')
    {
      OSDeviceDebug::putString(" parseIso() bad format ");
      return OSReturn::OS_BAD_DATE;
    }

  int i;
  i = 0;

  for (i = 0; i < 15; ++i)
    {
      if (i == 8)
        continue; // skip over 'T'

      uchar_t ch;
      ch = pStr[i];
      uint8_t b;
      if (('0' <= ch) && (ch <= '9'))
        {
          b = ch - '0';
        }
      else
        {
          OSDeviceDebug::putString(" parseIso() bad character ");
          return OSReturn::OS_BAD_DATE;
        }
      switch (i)
        {
      case 0:
        m_year = b;
        break;

      case 1:
      case 2:
      case 3:
        m_year = m_year * 10 + b;
        break;

      case 4:
        m_month = b;
        break;

      case 5:
        m_month = m_month * 10 + b;
        break;

      case 6:
        m_day = b;
        break;

      case 7:
        m_day = m_day * 10 + b;
        break;

      case 9:
        m_hour = b;
        break;

      case 10:
        m_hour = m_hour * 10 + b;
        break;

      case 11:
        m_minute = b;
        break;

      case 12:
        m_minute = m_minute * 10 + b;
        break;

      case 13:
        m_second = b;
        break;

      case 14:
        m_second = m_second * 10 + b;
        break;

        }
    }

  if (!areTimeFieldsValid())
    {
      OSDeviceDebug::putString(" BAD_TIME (fields) ");
      return OSReturn::OS_BAD_TIME;
    }

  if (!areDateFieldsValid())
    {
      OSDeviceDebug::putString(" BAD_DATE (fields) ");
      return OSReturn::OS_BAD_DATE;
    }

  OSDeviceDebug::putString(" date ");
  OSDeviceDebug::putDec(m_year, 4);
  OSDeviceDebug::putChar('-');
  OSDeviceDebug::putDec(m_month, 2);
  OSDeviceDebug::putChar('-');
  OSDeviceDebug::putDec(m_day, 2);
  OSDeviceDebug::putChar(' ');
  OSDeviceDebug::putDec(m_hour, 2);
  OSDeviceDebug::putChar(':');
  OSDeviceDebug::putDec(m_minute, 2);
  OSDeviceDebug::putChar(':');
  OSDeviceDebug::putDec(m_second, 2);
  OSDeviceDebug::putChar(' ');

  return OSReturn::OS_OK;
}

bool
DateTime::areDateFieldsValid(void)
{
  if ((m_year < getEpochYear()) || (m_month < 1) || (m_month > 12) || (m_day
      < 1) || (m_day > 31))
    {
      return false;
    }

  Day_t daysInMonth;
  daysInMonth = daysInMonths[m_month - 1];

  if (daysInMonth < 31)
    {
      if (m_month == 2 && isLeapYear())
        ++daysInMonth; // Adjust for February 29

      // Check for impossible dates, like 'February 30', 'April 31', etc
      if (m_day > daysInMonth)
        {
          return false;
        }
    }
  return true;
}

bool
DateTime::areTimeFieldsValid(void)
{
  if ((m_hour > 24) || (m_minute > 60) || (m_second > 60))
    {
      return false;
    }

  return true;
}

OSReturn_t
DateTime::parseNmeaDate(const char* pDate)
{
  DEBUG_ASSERT(pDate != NULL);

  if ((strlen(pDate) != 6))
    {
      OSDeviceDebug::putString(" BAD_DATE ");
      return OSReturn::OS_BAD_DATE;
    }

  for (int i = 0; i < 6; ++i)
    {
      uchar_t ch;
      ch = pDate[i];

      uint8_t b;
      if (('0' <= ch) && (ch <= '9'))
        {
          b = ch - '0';
        }
      else
        {
          OSDeviceDebug::putString(" BAD_DATE (ch) ");
          return OSReturn::OS_BAD_DATE;
        }
      switch (i)
        {
      case 0:
        m_day = b;
        break;

      case 1:
        m_day = m_day * 10 + b;
        break;

      case 2:
        m_month = b;
        break;

      case 3:
        m_month = m_month * 10 + b;
        break;

      case 4:
        m_year = b;
        break;

      case 5:
        m_year = m_year * 10 + b;
        m_year = m_year + 2000;
        break;
        }
    }

  if (!areDateFieldsValid())
    {
      OSDeviceDebug::putString(" BAD_DATE (fields) ");
      return OSReturn::OS_BAD_DATE;
    }

  OSDeviceDebug::putString(" date ");
  OSDeviceDebug::putDec(m_year, 4);
  OSDeviceDebug::putChar('-');
  OSDeviceDebug::putDec(m_month, 2);
  OSDeviceDebug::putChar('-');
  OSDeviceDebug::putDec(m_day, 2);
  OSDeviceDebug::putChar(' ');

  return OSReturn::OS_OK;
}

OSReturn_t
DateTime::parseNmeaTime(const char* pTime)
{
  DEBUG_ASSERT(pTime != NULL);

  if ((strlen(pTime) != 10) || pTime[6] != '.')
    {
      OSDeviceDebug::putString(" BAD_TIME ");
      return OSReturn::OS_BAD_TIME;
    }

  for (int i = 0; i < 6; ++i)
    {
      uchar_t ch;
      ch = pTime[i];

      uint8_t b;
      if (('0' <= ch) && (ch <= '9'))
        {
          b = ch - '0';
        }
      else
        {
          OSDeviceDebug::putString(" BAD_TIME (ch) ");
          return OSReturn::OS_BAD_TIME;
        }

      switch (i)
        {
      case 0:
        m_hour = b;
        break;

      case 1:
        m_hour = m_hour * 10 + b;
        break;

      case 2:
        m_minute = b;
        break;

      case 3:
        m_minute = m_minute * 10 + b;
        break;

      case 4:
        m_second = b;
        break;

      case 5:
        m_second = m_second * 10 + b;
        break;

        }
    }

  if (!areTimeFieldsValid())
    {
      OSDeviceDebug::putString(" BAD_TIME (fields) ");
      return OSReturn::OS_BAD_TIME;
    }

  OSDeviceDebug::putString(" time ");
  OSDeviceDebug::putDec(m_hour, 2);
  OSDeviceDebug::putChar(':');
  OSDeviceDebug::putDec(m_minute, 2);
  OSDeviceDebug::putChar(':');
  OSDeviceDebug::putDec(m_second, 2);
  OSDeviceDebug::putChar('.');
  OSDeviceDebug::putString(&pTime[7]);
  OSDeviceDebug::putChar(' ');

  return OSReturn::OS_OK;
}

DateTime::DurationSeconds_t
DateTime::computeSecondsFromEpoch()
{
  DurationSeconds_t seconds;
  Year_t fullYears;

  if (m_year > getEpochYear())
    fullYears = m_year - getEpochYear() - 1;
  else
    fullYears = 0;

  DayOfYear_t fullDays;
  fullDays = computeDayOfYear() - 1;

  seconds = (((((fullYears * 365) + fullDays) * 24) + m_hour) * 60 + m_minute)
      * 60 + m_second;

  if (fullYears > 0)
    {
      // adjust for leap years
      Year_t count;
      count = 0;

      Year_t y;
      y = (getEpochYear() + 3) & ~0x3; // First possible leap year from EPOCH

      // TODO: compute without loop
      for (; y < m_year; y += 4)
        {
          // I doubt this will ever live to need this, but for just in case
          if (((y % 400) == 0) || ((y % 100) != 0))
            ++count;
        }

      if (count > 0)
        {
          // Add one more day for each leap year
          seconds += (24 * 60 * 60) * count;
        }
    }

  return seconds;
}

void
DateTime::processSecondsFromEpoch(DurationSeconds_t secondsFromEpoch)
{
  ldiv_t daysAndSeconds;
  daysAndSeconds = ldiv(secondsFromEpoch, 60 * 60 * 24);

  // daysAndSeconds.quot = full days
  // daysAndSeconds.rem = secondsInDay (0-86399)

  div_t hoursAndSeconds;
  hoursAndSeconds = div(daysAndSeconds.rem, 60 * 60);

  m_hour = hoursAndSeconds.quot; // full hours (0-23)
  // hoursAndSeconds.rem = seconds in hour (0-3599)

  div_t minutesAndSeconds;
  minutesAndSeconds = div(hoursAndSeconds.rem, 60);

  m_minute = minutesAndSeconds.quot; // full minutes (0-59)
  m_second = minutesAndSeconds.rem; // seconds (0-59)

  // The hard part is to process the number of day and
  // get the year/month/day fields

  DurationDays_t remainingDays;
  remainingDays = daysAndSeconds.quot;

  bool isLeap;
  isLeap = false;

  // First identify year
  Year_t year;
  for (year = getEpochYear();; ++year)
    {
      DayOfYear_t daysInYear;
      daysInYear = 365;

      isLeap = isLeapYear(year);
      if (isLeap)
        ++daysInYear;

      if (remainingDays < daysInYear)
        break;

      remainingDays -= daysInYear;
    }

  m_year = year;

  // Now we know
  // - the year
  // - if it is leap
  // - the remaining days in year

  // Identify month
  Month_t month;
  for (month = 1; month <= 12; ++month)
    {
      Day_t daysInMonth;
      daysInMonth = daysInMonths[month - 1];
      if ((month == 2) && isLeap)
        {
          ++daysInMonth;
        }

      if (remainingDays < daysInMonth)
        break;

      remainingDays -= daysInMonth;
    }

  m_month = month; // already starts at 1
  m_day = remainingDays + 1; // to make it start at 1
}

bool
DateTime::isLeapYear(Year_t year)
{
  if ((year % 4) == 0)
    {
      if (((year % 400) == 0) || ((year % 100) != 0))
        return true;
      else
        return false;
    }
  else
    {
      return false;
    }
}

DateTime::DayOfYear_t
DateTime::computeDayOfYear()
{
  DayOfYear_t dayOfYear;
  dayOfYear = 0;

  // First add full months
  Month_t month;
  for (month = 1; month < m_month; ++month)
    {
      Day_t daysInMonth;
      daysInMonth = daysInMonths[month - 1];

      if (month == 2 && isLeapYear())
        ++daysInMonth;

      dayOfYear += daysInMonth;
    }

  // Add days in current month
  dayOfYear += m_day;

  return dayOfYear;
}

std::ostream&
operator <<(std::ostream& out, DateTime& dt)
{
  out << std::dec;
  out.width(4);
  out << (uint_t) dt.getYear() << '-';
  out.width(2);
  out << (uint_t) dt.getMonth() << '-';
  out.width(2);
  out << (uint_t) dt.getDay() << ' ';
  out.width(2);
  out << (uint_t) dt.getHour() << ':';
  out.width(2);
  out << (uint_t) dt.getMinute() << ':';
  out.width(2);
  out << (uint_t) dt.getSecond();

  return out;
}
#endif /* defined(OS_INCLUDE_DATETIME) */
