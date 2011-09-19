/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef OSLOGGER_H_
#define OSLOGGER_H_

#include "portable/kernel/include/OS.h"

// Generic logger facility.

typedef int LogLevel_t;
typedef int LogCode_t;

// common log classes

// Plenty of log levels to choose from
class OSLogLevel
{
public:
  const static LogLevel_t OS_OFF = 0;

  const static LogLevel_t OS_FATAL = 10;
  const static LogLevel_t OS_ERROR = 20;
  const static LogLevel_t OS_WARNING = 30;
  const static LogLevel_t OS_INFO = 40;

  const static LogLevel_t OS_CONFIG = 50;
  const static LogLevel_t OS_DEBUG = 60;
  const static LogLevel_t OS_TRACE = 70;
  const static LogLevel_t OS_INSANE = 80;

  const static LogLevel_t OS_ALL = 90;
};

class OSLogger
{
public:
  // Loggers have names, to distinguish between them
  OSLogger(const char* name);

  void
  logFatal(LogCode_t code, const char* msg = NULL);
  void
  logError(LogCode_t code, const char* msg = NULL);
  void
  logWarning(LogCode_t code, const char* msg = NULL);
  void
  logInfo(LogCode_t code, const char* msg = NULL);
  void
  logConfig(LogCode_t code, const char* msg = NULL);
  void
  logDebug(LogCode_t code, const char* msg = NULL);
  void
  logTrace(LogCode_t code, const char* msg = NULL);
  void
  logInsane(LogCode_t code, const char* msg = NULL);

  void
  log(LogLevel_t level, LogCode_t code, const char* msg = NULL);

  void
  flush(void);

  const char*
  getName(void) const;

  LogLevel_t
  getLevel(void) const;
  void
  setLevel(LogLevel_t level);

  LogLevel_t
  getDebugLevel(void) const;
  void
  setDebugLevel(LogLevel_t level);

  const static char*
  convertLevelToString(LogLevel_t level);
  static LogLevel_t
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
  implLog(LogLevel_t level, LogCode_t code, const char* msg) = 0;

  virtual void
  implFlush(void) = 0;

  // ----- Static methods -----------------------------------------------------

  static OSLogger*
  registerLogger(OSLogger *log);

  // ----- Private members ----------------------------------------------------

  // minimum log level for the line to be logged
  LogLevel_t m_minLevel;

  // minimum debug log level for the line to be logged
  LogLevel_t m_minDebugLevel;

  const char* m_name;

  // ----- Static members -----------------------------------------------------

  static OSLogger* ms_loggersArray[OS_CFGINT_OSLOGGER_ARRAY_SIZE];
  static int ms_loggersArrayCount;
};

inline void
OSLogger::logFatal(LogCode_t code, const char* msg)
{
  log(OSLogLevel::OS_FATAL, code, msg);
}

inline void
OSLogger::logError(LogCode_t code, const char* msg)
{
  log(OSLogLevel::OS_FATAL, code, msg);
}

inline void
OSLogger::logWarning(LogCode_t code, const char* msg)
{
  log(OSLogLevel::OS_WARNING, code, msg);
}

inline void
OSLogger::logInfo(LogCode_t code, const char* msg)
{
  log(OSLogLevel::OS_INFO, code, msg);
}

inline void
OSLogger::logConfig(LogCode_t code, const char* msg)
{
  log(OSLogLevel::OS_CONFIG, code, msg);
}

inline void
OSLogger::logDebug(LogCode_t code, const char* msg)
{
  log(OSLogLevel::OS_DEBUG, code, msg);
}

inline void
OSLogger::logTrace(LogCode_t code, const char* msg)
{
  log(OSLogLevel::OS_TRACE, code, msg);
}

inline void
OSLogger::logInsane(LogCode_t code, const char* msg)
{
  log(OSLogLevel::OS_INSANE, code, msg);
}

inline const char*
OSLogger::getName(void) const
{
  return m_name;
}

inline void
OSLogger::setLevel(LogLevel_t level)
{
  m_minLevel = level;
}

inline LogLevel_t
OSLogger::getLevel(void) const
{
  return m_minLevel;
}

inline void
OSLogger::setDebugLevel(LogLevel_t level)
{
  m_minDebugLevel = level;
}

inline LogLevel_t
OSLogger::getDebugLevel(void) const
{
  return m_minDebugLevel;
}

#endif /* OSLOGGER_H_ */
