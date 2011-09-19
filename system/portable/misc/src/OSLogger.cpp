/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_OSLOGGER)

#include "portable/kernel/include/OS.h"

#include "portable/misc/include/OSLogger.h"

#include <string.h>

OSLogger* OSLogger::ms_loggersArray[OS_CFGINT_OSLOGGER_ARRAY_SIZE];
int OSLogger::ms_loggersArrayCount = 0;

OSLogger::OSLogger(const char* name)
{
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
  OSDeviceDebug::putString_P(PSTR("OSLogger('"));
  OSDeviceDebug::putString(name);
  OSDeviceDebug::putString_P(PSTR("') @"));
  OSDeviceDebug::putPtr(this);
  OSDeviceDebug::putNewLine();
#endif

  m_minLevel = LogLevel::OS_INFO;
  m_minDebugLevel = LogLevel::OS_INFO;
  m_name = name;

  registerLogger(this);
}

const char*
OSLogger::convertLevelToString(LogLevel_t level)
{
  const char* p;
  switch (level)
    {
  case LogLevel::OS_OFF:
    p = "off";
    break;

  case LogLevel::OS_FATAL:
    p = "fatal";
    break;

  case LogLevel::OS_ERROR:
    p = "error";
    break;

  case LogLevel::OS_WARNING:
    p = "warning";
    break;

  case LogLevel::OS_INFO:
    p = "info";
    break;

  case LogLevel::OS_CONFIG:
    p = "config";
    break;

  case LogLevel::OS_DEBUG:
    p = "debug";
    break;

  case LogLevel::OS_TRACE:
    p = "trace";
    break;

  case LogLevel::OS_INSANE:
    p = "insane";
    break;

  case LogLevel::OS_ALL:
    p = "all";
    break;

  default:
    p = "unknown";
    }

  return p;
}

OSLogger::LogLevel_t
OSLogger::convertStringToLevel(const char* level)
{
  if (level == NULL)
    return -1;

  if (!strcmp(level, "off"))
    return LogLevel::OS_OFF;

  if (!strcmp(level, "fatal"))
    return LogLevel::OS_FATAL;

  if (!strcmp(level, "error"))
    return LogLevel::OS_ERROR;

  if (!strcmp(level, "warning"))
    return LogLevel::OS_WARNING;

  if (!strcmp(level, "info"))
    return LogLevel::OS_INFO;

  if (!strcmp(level, "config"))
    return LogLevel::OS_CONFIG;

  if (!strcmp(level, "debug"))
    return LogLevel::OS_DEBUG;

  if (!strcmp(level, "trace"))
    return LogLevel::OS_TRACE;

  if (!strcmp(level, "insane"))
    return LogLevel::OS_INSANE;

  if (!strcmp(level, "all"))
    return LogLevel::OS_ALL;

  return -1;
}

OSLogger*
OSLogger::registerLogger(OSLogger *logger)
{
  if (ms_loggersArrayCount == OS_CFGINT_OSLOGGER_ARRAY_SIZE)
    return NULL;

  ms_loggersArray[ms_loggersArrayCount] = logger;
  ms_loggersArrayCount++;

  return logger;
}

int
OSLogger::getLoggersCount(void)
{
  return ms_loggersArrayCount;
}

OSLogger*
OSLogger::getLogger(const char *name)
{
  int i;
  char *tempName;

  for (i = 0; i < ms_loggersArrayCount; i++)
    {
      tempName = (char*) ms_loggersArray[i]->m_name;

      if (!strcmp(tempName, name))
        return ms_loggersArray[i];
    }

  return NULL;
}

OSLogger*
OSLogger::getLogger(int index)
{
  if (index >= ms_loggersArrayCount)
    return NULL;

  return ms_loggersArray[index];
}

void
OSLogger::log(LogLevel_t level, LogCode_t code, const char* msg)
{
#if defined(DEBUG)
  if (level <= m_minDebugLevel)
    {
      OSSchedulerLock::enter();
        {
          OSDeviceDebug::putString("DBGLOG(");
          OSDeviceDebug::putString(convertLevelToString(level));
          OSDeviceDebug::putChar(',');
          OSDeviceDebug::putString(m_name);
          OSDeviceDebug::putChar(',');
          OSDeviceDebug::putDec((unsigned long) code);
          OSDeviceDebug::putChar(',');
          if (msg != NULL)
            {
              OSDeviceDebug::putChar('\'');
              OSDeviceDebug::putString(msg);
              OSDeviceDebug::putChar('\'');
            }
          else
            {
              OSDeviceDebug::putString("null");
            }
          OSDeviceDebug::putChar(')');
          OSDeviceDebug::putNewLine();
        }
      OSSchedulerLock::exit();
    }
#endif /* defined(DEBUG) */

  if (level <= m_minLevel)
    {
      implLog(level, code, msg);
    }
}

void
OSLogger::flush(void)
{
  implFlush();
}

#if false
// ----- Implementation methods -----------------------------------------------

void
OSLogger::implLog(LogLevel_t level __attribute__((unused)), LogCode_t code __attribute__((unused)), const char* msg __attribute__((unused)))
  {
    ; // Nothing here, implement it in children classes

  }

#endif

#endif /* defined(OS_INCLUDE_OSLOGGER) */
