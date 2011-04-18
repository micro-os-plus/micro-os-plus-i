/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_OSLOGGER)

#include "portable/kernel/include/OS.h"

#include "portable/misc/include/OSLogger.h"

OSLogger::OSLogger(const char* name)
{
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
  OSDeviceDebug::putString_P(PSTR("OSLogger('"));
  OSDeviceDebug::putString(name);
  OSDeviceDebug::putString_P(PSTR("')="));
  OSDeviceDebug::putPtr(this);
  OSDeviceDebug::putNewLine();
#endif

  m_minLevel = OSLogLevel::OS_INFO;
  m_name = name;
}

const char*
OSLogger::convertLevelToString(logLevel_t level)
{
  const char* p;
  switch (level)
    {
  case OSLogLevel::OS_OFF:
    p = "off";
    break;

  case OSLogLevel::OS_FATAL:
    p = "fatal";
    break;

  case OSLogLevel::OS_ERROR:
    p = "error";
    break;

  case OSLogLevel::OS_WARNING:
    p = "warning";
    break;

  case OSLogLevel::OS_INFO:
    p = "info";
    break;

  case OSLogLevel::OS_CONFIG:
    p = "config";
    break;

  case OSLogLevel::OS_DEBUG:
    p = "debug";
    break;

  case OSLogLevel::OS_TRACE:
    p = "trace";
    break;

  case OSLogLevel::OS_INSANE:
    p = "insane";
    break;

  case OSLogLevel::OS_ALL:
    p = "all";
    break;

  default:
    p = "unknown";
    }

  return p;
}

void
OSLogger::log(logLevel_t level, logCode_t code, const char* msg)
{
  if (level <= m_minLevel)
    {
      implLog(level, code, msg);
    }
}

void
OSLogger::implLog(logLevel_t level, logCode_t code, const char* msg)
{
#if defined(DEBUG)
  OSDeviceDebug::putString("log ");
  OSDeviceDebug::putString(convertLevelToString(level));
  OSDeviceDebug::putString(" ");
  OSDeviceDebug::putString(m_name);
  OSDeviceDebug::putString(" ");
  OSDeviceDebug::putDec((unsigned long) code);
  OSDeviceDebug::putString(" '");
  OSDeviceDebug::putString(msg);
  OSDeviceDebug::putString("'");
  OSDeviceDebug::putNewLine();
#endif /* defined(DEBUG) */
}

#endif /* defined(OS_INCLUDE_OSLOGGER) */
