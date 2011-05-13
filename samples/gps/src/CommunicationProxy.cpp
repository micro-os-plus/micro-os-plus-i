/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "CommunicationProxy.h"

CommunicationProxy::CommunicationProxy()
{
  debug.putString("CommunicationProxy", this);
}

OSReturn_t
CommunicationProxy::sendGpsPosition(gpsPosition_t* p)
{
  p = p;
  return OSReturn::OS_OK;
}
