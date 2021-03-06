/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef COMMUNICATIONPROXY_H_
#define COMMUNICATIONPROXY_H_

#include "portable/kernel/include/uOS.h"

#include "portable/misc/include/GpsPosition.h"

class CommunicationProxy
{
public:
  CommunicationProxy();

  OSReturn_t rpcSendGpsPosition(GpsPosition_t& pPos);
};

#endif /* COMMUNICATIONPROXY_H_ */
