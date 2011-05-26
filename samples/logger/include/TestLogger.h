/*
 *      Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef LOGGER_H
#define LOGGER_H


#include "portable/kernel/include/uOS.h"

class TestLogger : public OSLogger
{
public:
  // Loggers have names, to distinguish between them
  TestLogger(const char* name);

protected:

  virtual void
  implLog(logLevel_t level, logCode_t code, const char* msg);
};

#endif // LOGGER_H

