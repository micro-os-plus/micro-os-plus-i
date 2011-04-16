/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_LOGGER)

#include "portable/kernel/include/OS.h"

#include "portable/misc/include/Logger.h"

Logger::Logger(const char* name)
{
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
  OSDeviceDebug::putString_P(PSTR("Logger('"));
  OSDeviceDebug::putString(name);
  OSDeviceDebug::putString_P(PSTR("')="));
  OSDeviceDebug::putPtr(this);
  OSDeviceDebug::putNewLine();
#endif

  m_minLevel = LogLevel::OS_INFO;
  m_name = name;
}

const char*
Logger::convertLevelToString(logLevel_t level)
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

void
Logger::log(logLevel_t level, logCode_t code, const char* msg)
{
  if (level <= m_minLevel)
    {
      implLog(level, code, msg);
    }
}

void
Logger::implLog(logLevel_t level, logCode_t code, const char* msg)
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

#endif /* defined(OS_INCLUDE_LOGGER) */
