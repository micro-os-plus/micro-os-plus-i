/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef GPSMANAGER_H_
#define GPSMANAGER_H_

#include "portable/kernel/include/uOS.h"

#include "portable/misc/include/GpsPosition.h"

#define GPS_POWER_SW_PORT               GPIOX
#define GPS_POWER_SW_PIN                AVR32_PIN_PX44

#define GPS_RST_PORT                    GPIOX
#define GPS_RST_PIN                     AVR32_PIN_PX15

#define GPS_PPS_PORT                    GPIOA
#define GPS_PPS_PIN                     AVR32_PIN_PA12

#define GPS_WK_UP_PORT                  GPIOX
#define GPS_WK_UP_PIN                   AVR32_PIN_PX42

#define GPS_START_UP_NR_MSG             (6)

#define GPS_START_UP_READ_TIMEOUT       (1000)
#define GPS_MSG_READ_TIMEOUT            (250)

#define OS_CFGINT_GPSMANAGER_RXBUF_ARRAY_SIZE   (100)
#define OS_CFGINT_GPSMANAGER_TXBUF_ARRAY_SIZE   (100)

class GpsManager : public OSThread
{
public:
  static const OSEventFlagsBits_t OS_REQUEST_GPS_POSITION = 0x1;
  static const OSEventFlagsBits_t OS_ALL_FLAGS = OS_REQUEST_GPS_POSITION;

  // active object constructor
  GpsManager(const char *pName, OSDeviceCharacter& dev);

  // actual thread main code
  virtual void
  threadMain(void);

  OSReturn_t
  RequestGpsPosition(void);
  OSReturn_t
  SetGpsSchedule(int address, int id, int absoluteDateTime, int onDurationSeconds,
      int offDurationSeconds);

//  OSReturn_t
//  getPosition(gpsPosition_t* pPos);


  bool
  getOnStatus(void);

  OSReturn_t getPosition(GpsPosition_t& pPos);


private:
  // members
  OSStack_t m_stack[(OSThread::STACK_MINIMAL_SIZE + 600) / sizeof(OSStack_t)];
  OSDeviceCharacter& m_dev;

  OSEventFlags m_eventFlags;
  GpsPosition m_gpsPosition;

  Parser m_parser;
  unsigned char m_token[20]; // parsed token

  bool m_isOn;

  void
  setOnStatus(bool value);

  OSReturn_t
  sleepEnter(void);
  OSReturn_t
  sleepExit(void);

  void
  init(void);

};

#endif /* GPSMANAGER_H_ */
