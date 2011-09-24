/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef OSRETURN_H_
#define OSRETURN_H_

// ----------------------------------------------------------------------------

typedef int OSReturn_t;

class OSReturn
{
public:
  const static OSReturn_t OS_OK = 0;
  const static OSReturn_t OS_ERROR = -1;
  const static OSReturn_t OS_TIMEOUT = -2;
  const static OSReturn_t OS_DISCONNECTED = -3;
  const static OSReturn_t OS_WOULD_BLOCK = -4;
  const static OSReturn_t OS_NOT_OWNER = -5;
  const static OSReturn_t OS_NOT_ACQUIRED = -6;
  const static OSReturn_t OS_ALREADY_OPENED = -7;
  const static OSReturn_t OS_NOT_OPENED = -8;
  const static OSReturn_t OS_TOO_LONG = -9;
  const static OSReturn_t OS_ILLEGAL_CHARACTER = -10;
  const static OSReturn_t OS_BAD_CHECKSUM = -11;
  const static OSReturn_t OS_NOT_INITIALISED = -12;
  const static OSReturn_t OS_BAD_PARAMETER = -13;
  const static OSReturn_t OS_BUFFER_FULL = -14;
  const static OSReturn_t OS_IMMEDIATELY = -15;
  const static OSReturn_t OS_ITEM_NOT_FOUND = -16;
  const static OSReturn_t OS_NOT_ENOUGH_SPACE = -17;
  const static OSReturn_t OS_BAD_COMMAND = -18;
  const static OSReturn_t OS_NULL_POINTER = -19;
  const static OSReturn_t OS_BAD_SEQUENCE = -20;
  const static OSReturn_t OS_NOT_FOUND = -21;
  const static OSReturn_t OS_NOT_IMPLEMENTED = -22;
  const static OSReturn_t OS_BUSY = -23;
  const static OSReturn_t OS_BAD_STATE = -24;
  const static OSReturn_t OS_TOO_SHORT = -25;
  const static OSReturn_t OS_SIZE_EXCEEDED = -26;
  const static OSReturn_t OS_CANCELLED = -27;
  const static OSReturn_t OS_IGNORED = -28;
  const static OSReturn_t OS_NACK = -29;
  const static OSReturn_t OS_EMPTY_STRING = -30;
  const static OSReturn_t OS_NOT_ENABLED = -31;
  const static OSReturn_t OS_BAD_MAGIC = -32;
  const static OSReturn_t OS_END_OF_COLLECTION = -32;
  const static OSReturn_t OS_OUT_OF_RANGE = -33;
  const static OSReturn_t OS_BAD_DATE = -34;
  const static OSReturn_t OS_SHOULD_WAIT = -35;

};

// ----------------------------------------------------------------------------

#endif /* OSRETURN_H_ */
