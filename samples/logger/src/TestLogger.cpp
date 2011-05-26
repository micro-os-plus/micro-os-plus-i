/*
 *      Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/uOS.h"

#include "TestLogger.h"


TestLogger::TestLogger(const char* name) : OSLogger(name)
{

}

void
TestLogger::implLog(logLevel_t level, logCode_t code, const char* msg)
{
#if defined(DEBUG)
      OSDeviceDebug::putString("TEST: log ");
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

