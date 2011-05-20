/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include "portable/kernel/include/OS.h"

// Generic logger facility.

typedef int logLevel_t;
typedef int logCode_t;

// Plenty of log levels to choose from
class OSLogLevel
{
public:
  static const logLevel_t OS_OFF = 0;

  static const logLevel_t OS_FATAL = 10;
  static const logLevel_t OS_ERROR = 20;
  static const logLevel_t OS_WARNING = 30;
  static const logLevel_t OS_INFO = 40;

  static const logLevel_t OS_CONFIG = 50;
  static const logLevel_t OS_DEBUG = 60;
  static const logLevel_t OS_TRACE = 70;
  static const logLevel_t OS_INSANE = 80;

  static const logLevel_t OS_ALL = 90;
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

  const char*
  getName(void) const;

  logLevel_t
  getLevel(void) const;
  void
  setLevel(logLevel_t level);

  static const char*
  convertLevelToString(logLevel_t level);

private:
  void
  log(logLevel_t level, logCode_t code, const char* msg);

  virtual void
  implLog(logLevel_t level, logCode_t code, const char* msg);

  // minimum log level for the line to be logged
  logLevel_t m_minLevel;

  const char* m_name;
};

inline const char*
OSLogger::getName(void) const
{
  return m_name;
}

inline logLevel_t
OSLogger::getLevel(void) const
{
  return m_minLevel;
}

inline void
OSLogger::setLevel(logLevel_t level)
{
  m_minLevel = level;
}

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

#endif /* LOGGER_H_ */
