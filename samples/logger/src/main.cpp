/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include <string.h>
#include "portable/kernel/include/uOS.h"
#include "TestLogger.h"

// ----------------------------------------------------------------------------
// Here include headers for other object required by active objects,
// like local objects (system objects should be visible via uOS.h)

// ----------------------------------------------------------------------------
// Here include headers for local active objects

void
allLevels(void);

// ----------------------------------------------------------------------------

/*
 * This is a test of the logging facility.
 * 
 * It does not need multitasking
 */

TestLogger logger("root");
TestLogger logger1("logger1");
TestLogger logger2("logger2");

void
allLevels(void)
{
  OSDeviceDebug::putString("--- ");
  OSDeviceDebug::putString(logger.convertLevelToString(logger.getLevel()));
  OSDeviceDebug::putString(" ---");
  OSDeviceDebug::putNewLine();

  logger.logFatal(1, "no way to recover");
  logger.logError(2, "bad, bud we can survive");
  logger.logWarning(3, "something happened");
  logger.logInfo(4, "breaking news");
  logger.logConfig(5, "configuration variable");
  logger.logDebug(6, "we were here");
  logger.logTrace(7, "tracing all");
  logger.logInsane(8, "scary");
}

void
OS::main()
{
  const char *levelName;
  OSLogger *tempLogger;

  // test convertLevelToString / convertStringToLevel
  levelName = OSLogger::convertLevelToString(OSLogLevel::OS_ALL);
  if(strcmp(levelName, "all"))
    {
      OSDeviceDebug::putString("FAILED: OSLogger::convertLevelToString()");
      OSDeviceDebug::putNewLine();
    }

  levelName = OSLogger::convertLevelToString(OSLogLevel::OS_OFF);
  if(strcmp(levelName, "off"))
    {
      OSDeviceDebug::putString("FAILED: OSLogger::convertLevelToString()");
      OSDeviceDebug::putNewLine();
    }

  // test getLoggersCount
  if(OSLogger::getLoggersCount() != 3)
    {
      OSDeviceDebug::putString("FAILED: OSLogger::getLoggersCount()");
      OSDeviceDebug::putNewLine();
    }

  // test geetLogger methods
  tempLogger = OSLogger::getLogger("root");
  if(tempLogger != &logger)
    {
      OSDeviceDebug::putString("FAILED: OSLogger::getLogger() by name");
      OSDeviceDebug::putNewLine();
    }
  tempLogger = OSLogger::getLogger("logger1");
  if(tempLogger != &logger1)
    {
      OSDeviceDebug::putString("FAILED: OSLogger::getLogger() by name");
      OSDeviceDebug::putNewLine();
    }
  tempLogger = OSLogger::getLogger("logger2");
  if(tempLogger != &logger2)
    {
      OSDeviceDebug::putString("FAILED: OSLogger::getLogger() by name");
      OSDeviceDebug::putNewLine();
    }

  tempLogger = OSLogger::getLogger(0);
  if(tempLogger != &logger)
    {
      OSDeviceDebug::putString("FAILED: OSLogger::getLogger() by index");
      OSDeviceDebug::putNewLine();
    }
  tempLogger = OSLogger::getLogger(1);
  if(tempLogger != &logger1)
    {
      OSDeviceDebug::putString("FAILED: OSLogger::getLogger() by index");
      OSDeviceDebug::putNewLine();
    }
  tempLogger = OSLogger::getLogger(2);
  if(tempLogger != &logger2)
    {
      OSDeviceDebug::putString("FAILED: OSLogger::getLogger() by index");
      OSDeviceDebug::putNewLine();
    }

  allLevels();

  // test setLevel / getLevel / log methods
  logger.setLevel(OSLogLevel::OS_ALL);
  if(logger.getLevel() != OSLogLevel::OS_ALL)
  {
    OSDeviceDebug::putString("FAILED: OSLogger::getLevel()");
    OSDeviceDebug::putNewLine();
  }
  logger.setDebugLevel(OSLogLevel::OS_ALL);
  if(logger.getDebugLevel() != OSLogLevel::OS_ALL)
  {
    OSDeviceDebug::putString("FAILED: OSLogger::getDebugLevel()");
    OSDeviceDebug::putNewLine();
  }

  allLevels();

  // test setLevel / getLevel / log methods
  logger.setLevel(OSLogLevel::OS_OFF);
  if(logger.getLevel() != OSLogLevel::OS_OFF)
  {
    OSDeviceDebug::putString("FAILED: OSLogger::getLevel()");
    OSDeviceDebug::putNewLine();
  }
  logger.setDebugLevel(OSLogLevel::OS_OFF);
  if(logger.getDebugLevel() != OSLogLevel::OS_OFF)
  {
    OSDeviceDebug::putString("FAILED: OSLogger::getDebugLevel()");
    OSDeviceDebug::putNewLine();
  }

  allLevels();

  OSDeviceDebug::putString("--- done ---");
  OSDeviceDebug::putNewLine();

  for (;;)
    ;
}

// ----------------------------------------------------------------------------

