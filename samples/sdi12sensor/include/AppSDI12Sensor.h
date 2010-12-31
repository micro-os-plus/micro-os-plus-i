/*
 *      Copyright (C) 2007-2008 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef APPSDI12SENSOR_H_
#define APPSDI12SENSOR_H_

#include "portable/kernel/include/uOS.h"

class AppSDI12Sensor : public SDI12Sensor
  {
public:
  AppSDI12Sensor(const char *pNameAquire, const char *pNamePeriodic,
      const char *pNameSDI12,
      OSTaskPriority_t priorityAquire = OSTask::DEFAULT_PRIORITY,
      OSTaskPriority_t priorityPeriodic = OSTask::DEFAULT_PRIORITY,
      OSTaskPriority_t prioritySDI12 = OSTask::DEFAULT_PRIORITY + 1);

  virtual ~AppSDI12Sensor();
  virtual void customInit(void);

  virtual void storeAddressToNonVolatileMemory(unsigned char newAddress);

  virtual void prepareResponseSendIdentification(void);
  virtual void prepareResponseContinuousMeasurement(unsigned char digit);
  virtual void prepareResponseStartMeasurement(unsigned char cmd,
      unsigned char digit);
  virtual bool prepareResponseExtendedCommands(const char *pBuf,
      unsigned int count);

  virtual void prepareMeasuredData(unsigned char cmd, unsigned char digit,
      unsigned int seconds, unsigned char count);

  virtual void processDelayedActions(void);

  virtual void processPeriodicMeasurements(void);

private:
  static unsigned char m_stackSDI12[APP_CFGINT_SDI12SENSOR_PROTO_STACK_SIZE];
  static unsigned char m_stackAquire[APP_CFGINT_SDI12SENSOR_DACQ_STACK_SIZE];
  static unsigned char
      m_stackPeriodic[APP_CFGINT_SDI12SENSOR_PERIODIC_STACK_SIZE];

  static unsigned short ms_periodic;

  };

#endif /*APPSDI12SENSOR_H_*/
