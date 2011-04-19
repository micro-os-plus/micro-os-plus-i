/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef APPLICATION_H_
#define APPLICATION_H_

#include "CommunicationProxy.h"
#include "GpsManager.h"
#include "TaskCli.h"

class Application;
extern Application app;

// ----------------------------------------------------------------------------

class Application
{
public:
  Application();

  CommunicationProxy com;

private:
  // USB device
  DeviceCharacterUsb devUsb0;

public:
  // Cli Tasks
  TaskCli cliUsb;

private:
  // USART buffers
  unsigned char
      tx_store[OS_CFGINT_DEVICECHARACTERBUFFEREDUSART0_TXBUF_ARRAY_SIZE];
  unsigned char
      rx_store[OS_CFGINT_DEVICECHARACTERBUFFEREDUSART0_RXBUF_ARRAY_SIZE];

  DeviceCharacterBufferedUsart0 devUsart0;

public:
  // GPS manager active object
  GpsManager gps;

};

#endif /* APPLICATION_H_ */
