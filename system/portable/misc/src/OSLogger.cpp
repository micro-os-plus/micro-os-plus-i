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
  OSDeviceDebug::putString_P(PSTR("')="));
  OSDeviceDebug::putPtr(this);
  OSDeviceDebug::putNewLine();
#endif

  m_minLevel = OSLogLevel::OS_INFO;
  m_minDebugLevel = OSLogLevel::OS_INFO;
  m_name = name;

  registerLogger(this);
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

logLevel_t
OSLogger::convertStringToLevel(const char* level)
{
  if(level == NULL)
    return -1;

  if(!strcmp(level, "off"))
    return OSLogLevel::OS_OFF;

  if(!strcmp(level, "fatal"))
    return OSLogLevel::OS_FATAL;

  if(!strcmp(level, "error"))
    return OSLogLevel::OS_ERROR;

  if(!strcmp(level, "warning"))
    return OSLogLevel::OS_WARNING;

  if(!strcmp(level, "info"))
    return OSLogLevel::OS_INFO;

  if(!strcmp(level, "config"))
    return OSLogLevel::OS_CONFIG;

  if(!strcmp(level, "debug"))
    return OSLogLevel::OS_DEBUG;

  if(!strcmp(level, "trace"))
    return OSLogLevel::OS_TRACE;

  if(!strcmp(level, "insane"))
    return OSLogLevel::OS_INSANE;

  if(!strcmp(level, "all"))
    return OSLogLevel::OS_ALL;

  return -1;
}

OSLogger*
OSLogger::registerLogger(OSLogger *logger)
{
  if(ms_loggersArrayCount == OS_CFGINT_OSLOGGER_ARRAY_SIZE)
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

  for(i=0; i<ms_loggersArrayCount; i++)
    {
      tempName = (char*)ms_loggersArray[i]->m_name;

      if(!strcmp(tempName, name))
        return ms_loggersArray[i];
    }

  return NULL;
}

OSLogger*
OSLogger::getLogger(int index)
{
  if(index >= ms_loggersArrayCount)
    return NULL;

  return ms_loggersArray[index];
}

void
OSLogger::log(logLevel_t level, logCode_t code, const char* msg)
{
  if (level <= m_minDebugLevel)
    {
#if defined(DEBUG)
      OSDeviceDebug::putString("DEBUG: log ");
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
  if (level <= m_minLevel)
    {
      implLog(level, code, msg);
    }
}

void
OSLogger::implLog(logLevel_t level, logCode_t code, const char* msg)
{
  // make compiler happy
  level = level;
  code = code;
  msg = msg;
}

#endif /* defined(OS_INCLUDE_OSLOGGER) */
