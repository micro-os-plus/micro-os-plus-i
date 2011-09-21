/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_DATETIME)

#include "portable/misc/include/DateTime.h"

const DateTime::Day_t DateTime::daysInMonths[] =
  { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

// ----------------------------------------------------------------------------

DateTime::DateTime()
{
  debug.putConstructor_P(PSTR("DateTime"), this);

  m_second = 0;
  m_minute = 0;
  m_hour = 0;
  m_day = 0;
  m_month = 0;
  m_year = EPOCH_YEAR;
}

DateTime::~DateTime()
{
  debug.putDestructor_P(PSTR("DateTime"), this);
}

// Parse dates like YYYYMMDDTHHMMSS into members
// Return OS_OK or OS_BAD_DATE

OSReturn_t
DateTime::parseIso(const char* pStr)
{
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
  if ((m_year < EPOCH_YEAR) || (m_month < 1) || (m_month > 12) || (m_day < 1)
      || (m_day > 31) || (m_hour > 24) || (m_minute > 60) || (m_second > 60))
    {
      OSDeviceDebug::putString(" parseIso() bad fields ");
      return OSReturn::OS_BAD_DATE;
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
          OSDeviceDebug::putString(" parseIso() bad day ");
          return OSReturn::OS_BAD_DATE;
        }
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
  OSDeviceDebug::putNewLine();

  return OSReturn::OS_OK;
}

DateTime::DurationSeconds_t
DateTime::computeSecondsFromEpoch()
{
  DurationSeconds_t seconds;
  Year_t fullYears;

  if (m_year > EPOCH_YEAR)
    fullYears = m_year - EPOCH_YEAR - 1;
  else
    fullYears = 0;

  Day_t fullDays;
  fullDays = computeDayOfYear() - 1;

  seconds = (((((fullYears * 365) + fullDays) * 24) + m_hour) * 60 + m_minute)
      * 60 + m_second;

  if (fullYears > 0)
    {
      // adjust for leap years
      Year_t count;
      count = 0;

      Year_t y;
      y = (EPOCH_YEAR + 3) & ~0x3; // First possible leap year from EPOCH

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

bool
DateTime::isLeapYear(void)
{
  Year_t y;
  y = m_year;

  if ((y % 4) == 0)
    {
      if (((y % 400) == 0) || ((y % 100) != 0))
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

#endif /* defined(OS_INCLUDE_DATETIME) */
