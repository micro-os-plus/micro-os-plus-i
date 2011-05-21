/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "CommunicationProxy.h"

CommunicationProxy::CommunicationProxy()
{
  debug.putConstructor("CommunicationProxy", this);
}

OSReturn_t
CommunicationProxy::rpcSendGpsPosition(GpsPosition_t& p)
{
  p = p;
  return OSReturn::OS_OK;
}
