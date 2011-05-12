/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/uOS.h"

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

OSLogger logger("root");

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
  allLevels();

  logger.setLevel(OSLogLevel::OS_ALL);
  allLevels();

  logger.setLevel(OSLogLevel::OS_OFF);
  allLevels();

  OSDeviceDebug::putString("--- done ---");
  OSDeviceDebug::putNewLine();

  for (;;)
    ;
}

// ----------------------------------------------------------------------------

