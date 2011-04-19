/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "CommunicationProxy.h"

CommunicationProxy::CommunicationProxy()
{
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
  debug.putString("CommunicationProxy()=");
  debug.putPtr(this);
  debug.putNewLine();
#endif
}

OSReturn_t
CommunicationProxy::sendGpsPosition(gpsPosition_t* p)
{
  p = p;
  return OSReturn::OS_OK;
}
