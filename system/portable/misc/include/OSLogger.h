/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef OSLOGGER_H_
#define OSLOGGER_H_

#include "portable/kernel/include/OS.h"

// Generic logger facility.

typedef int logLevel_t;
typedef int logCode_t;

// common log classes

// Plenty of log levels to choose from
class OSLogLevel
{
public:
  const static logLevel_t OS_OFF = 0;

  const static logLevel_t OS_FATAL = 10;
  const static logLevel_t OS_ERROR = 20;
  const static logLevel_t OS_WARNING = 30;
  const static logLevel_t OS_INFO = 40;

  const static logLevel_t OS_CONFIG = 50;
  const static logLevel_t OS_DEBUG = 60;
  const static logLevel_t OS_TRACE = 70;
  const static logLevel_t OS_INSANE = 80;

  const static logLevel_t OS_ALL = 90;
};

class OSLogger
{
public:
  // Loggers have names, to distinguish between them
  OSLogger(const char* name);

  void
  logFatal(logCode_t code, const char* msg);
  void
  logError(logCode_t code, const char* msg);
  void
  logWarning(logCode_t code, const char* msg);
  void
  logInfo(logCode_t code, const char* msg);
  void
  logConfig(logCode_t code, const char* msg);
  void
  logDebug(logCode_t code, const char* msg);
  void
  logTrace(logCode_t code, const char* msg);
  void
  logInsane(logCode_t code, const char* msg);

  void
  log(logLevel_t level, logCode_t code, const char* msg);

  void
  flush(void);

  const char*
  getName(void) const;

  logLevel_t
  getLevel(void) const;
  void
  setLevel(logLevel_t level);

  logLevel_t
  getDebugLevel(void) const;
  void
  setDebugLevel(logLevel_t level);

  const static char*
  convertLevelToString(logLevel_t level);
  static logLevel_t
  convertStringToLevel(const char* level);

  static OSLogger*
  getLogger(const char *name);
  static OSLogger*
  getLogger(int index);

  static int
  getLoggersCount(void);

protected:

  // ----- Virtual methods ----------------------------------------------------

  virtual void
  implLog(logLevel_t level, logCode_t code, const char* msg) = 0;

  virtual void
  implFlush(void) = 0;

  // ----- Static methods -----------------------------------------------------

  static OSLogger*
  registerLogger(OSLogger *log);

  // ----- Private members ----------------------------------------------------

  // minimum log level for the line to be logged
  logLevel_t m_minLevel;

  // minimum debug log level for the line to be logged
  logLevel_t m_minDebugLevel;

  const char* m_name;

  // ----- Static members -----------------------------------------------------

  static OSLogger* ms_loggersArray[OS_CFGINT_OSLOGGER_ARRAY_SIZE];
  static int ms_loggersArrayCount;
};

inline void
OSLogger::logFatal(logCode_t code, const char* msg)
{
  log(OSLogLevel::OS_FATAL, code, msg);
}

inline void
OSLogger::logError(logCode_t code, const char* msg)
{
  log(OSLogLevel::OS_FATAL, code, msg);
}

inline void
OSLogger::logWarning(logCode_t code, const char* msg)
{
  log(OSLogLevel::OS_WARNING, code, msg);
}

inline void
OSLogger::logInfo(logCode_t code, const char* msg)
{
  log(OSLogLevel::OS_INFO, code, msg);
}

inline void
OSLogger::logConfig(logCode_t code, const char* msg)
{
  log(OSLogLevel::OS_CONFIG, code, msg);
}

inline void
OSLogger::logDebug(logCode_t code, const char* msg)
{
  log(OSLogLevel::OS_DEBUG, code, msg);
}

inline void
OSLogger::logTrace(logCode_t code, const char* msg)
{
  log(OSLogLevel::OS_TRACE, code, msg);
}

inline void
OSLogger::logInsane(logCode_t code, const char* msg)
{
  log(OSLogLevel::OS_INSANE, code, msg);
}

inline const char*
OSLogger::getName(void) const
{
  return m_name;
}

inline void
OSLogger::setLevel(logLevel_t level)
{
  m_minLevel = level;
}

inline logLevel_t
OSLogger::getLevel(void) const
{
  return m_minLevel;
}

inline void
OSLogger::setDebugLevel(logLevel_t level)
{
  m_minDebugLevel = level;
}

inline logLevel_t
OSLogger::getDebugLevel(void) const
{
  return m_minDebugLevel;
}

#endif /* OSLOGGER_H_ */
