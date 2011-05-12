/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef GPSMANAGER_H_
#define GPSMANAGER_H_

#include "portable/kernel/include/uOS.h"

#include "GpsPosition.h"


class GpsManager : public OSThread
{
public:
  static const OSEventFlagsBits_t OS_REQUEST_GPS_POSITION = 0x1;
  static const OSEventFlagsBits_t OS_ALL_FLAGS = OS_REQUEST_GPS_POSITION;

  // task constructor
  GpsManager(const char *pName, OSDeviceCharacter& dev);

  // actual task main code
  virtual void
  taskMain(void);

  OSReturn_t requestGpsPosition(void);

  OSReturn_t getPosition(gpsPosition_t* pPos);

private:
  // members
  OSStack_t m_stack[(OSThread::STACK_MINIMAL_SIZE + 400) / sizeof(OSStack_t)];
  OSDeviceCharacter& m_dev;

  OSEventFlags m_eventFlags;
  GpsPosition m_gpsPosition;

};

#endif /* GPSMANAGER_H_ */
