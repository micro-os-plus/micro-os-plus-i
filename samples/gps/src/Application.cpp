/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "Application.h"

Application::Application():
  cliUsb("usb", devUsb0),
      devUsart0(tx_store, sizeof(tx_store), rx_store, sizeof(rx_store)),
      gps("gps", devUsart0)
{
  debug.putConstructor("Application", this);
}
