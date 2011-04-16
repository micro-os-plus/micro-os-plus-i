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
class LogLevel
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

class Logger
{
public:
  // Loggers have names, to distinguish between them
  Logger(const char* name);

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
  getName(void);

  logLevel_t
  getLevel(void);
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

inline logLevel_t
Logger::getLevel(void)
{
  return m_minLevel;
}

inline void
Logger::setLevel(logLevel_t level)
{
  m_minLevel = level;
}

inline void
Logger::logFatal(logCode_t code, const char* msg)
{
  log(LogLevel::OS_FATAL, code, msg);
}

inline void
Logger::logError(logCode_t code, const char* msg)
{
  log(LogLevel::OS_FATAL, code, msg);
}

inline void
Logger::logWarning(logCode_t code, const char* msg)
{
  log(LogLevel::OS_WARNING, code, msg);
}

inline void
Logger::logInfo(logCode_t code, const char* msg)
{
  log(LogLevel::OS_INFO, code, msg);
}

inline void
Logger::logConfig(logCode_t code, const char* msg)
{
  log(LogLevel::OS_CONFIG, code, msg);
}

inline void
Logger::logDebug(logCode_t code, const char* msg)
{
  log(LogLevel::OS_DEBUG, code, msg);
}

inline void
Logger::logTrace(logCode_t code, const char* msg)
{
  log(LogLevel::OS_TRACE, code, msg);
}

inline void
Logger::logInsane(logCode_t code, const char* msg)
{
  log(LogLevel::OS_INSANE, code, msg);
}

#endif /* LOGGER_H_ */
