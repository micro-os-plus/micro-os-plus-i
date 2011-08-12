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
  static const OSReturn_t OS_OK = 0;
  static const OSReturn_t OS_ERROR = -1;
  static const OSReturn_t OS_TIMEOUT = -2;
  static const OSReturn_t OS_DISCONNECTED = -3;
  static const OSReturn_t OS_WOULD_BLOCK = -4;
  static const OSReturn_t OS_NOT_OWNER = -5;
  static const OSReturn_t OS_NOT_ACQUIRED = -6;
  static const OSReturn_t OS_ALREADY_OPENED = -7;
  static const OSReturn_t OS_NOT_OPENED = -8;
  static const OSReturn_t OS_TOO_LONG = -9;
  static const OSReturn_t OS_ILLEGAL_CHARACTER = -10;
  static const OSReturn_t OS_BAD_CHECKSUM = -11;
  static const OSReturn_t OS_NOT_INITIALISED = -12;
  static const OSReturn_t OS_BAD_PARAMETER = -13;
  static const OSReturn_t OS_BUFFER_FULL = -14;
  static const OSReturn_t OS_IMMEDIATELY = -15;
  static const OSReturn_t OS_ITEM_NOT_FOUND = -16;
  static const OSReturn_t OS_NOT_ENOUGH_SPACE = -17;
  static const OSReturn_t OS_BAD_COMMAND = -18;
  static const OSReturn_t OS_NULL_POINTER = -19;
  static const OSReturn_t OS_BAD_SEQUENCE = -20;
  static const OSReturn_t OS_NOT_FOUND = -21;
  static const OSReturn_t OS_NOT_IMPLEMENTED = -22;
  static const OSReturn_t OS_BUSY = -23;
  static const OSReturn_t OS_BAD_STATE = -24;
  static const OSReturn_t OS_TOO_SHORT = -25;
  static const OSReturn_t OS_SIZE_EXCEEDED = -26;
  static const OSReturn_t OS_CANCELLED = -27;
  static const OSReturn_t OS_IGNORED = -28;
  static const OSReturn_t OS_NACK = -29;
  static const OSReturn_t OS_EMPTY_STRING = -30;
};

// ----------------------------------------------------------------------------

#endif /* OSRETURN_H_ */
