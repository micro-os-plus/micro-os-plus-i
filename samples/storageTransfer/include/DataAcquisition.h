/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef DATAACQUISITION_H_
#define DATAACQUISITION_H_

#include "portable/kernel/include/uOS.h"

#include "Storage.h"

class DataAcquisition : public OSThread
{
public:
  // active object constructor
  DataAcquisition(int maxCount);

  // actual thread main code
  virtual void
  threadMain(void);

private:
  // members
  OSStack_t m_stack[(OSThread::STACK_MINIMAL_SIZE + 400) / sizeof(OSStack_t)];

  AcquisitionSample_t m_buffer[1000];

  int m_maxCount;

  // Assume DAQ uses little endian
  static const int BYTE0_INDEX = 0;
  static const int BYTE1_INDEX = 1;
  static const int BYTE2_INDEX = 2;
  static const int BYTE3_INDEX = 3;
};

#endif /* DATAACQUISITION_H_ */
