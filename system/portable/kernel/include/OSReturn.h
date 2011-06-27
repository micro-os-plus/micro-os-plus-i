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
  static const OSReturn_t OS_TIMEOUT = -1;
  static const OSReturn_t OS_DISCONNECTED = -2;
  static const OSReturn_t OS_WOULD_BLOCK = -3;
  static const OSReturn_t OS_NOT_OWNER = -4;
  static const OSReturn_t OS_NOT_ACQUIRED = -5;
  static const OSReturn_t OS_ALREADY_OPENED = -6;
  static const OSReturn_t OS_NOT_OPENED = -7;
  static const OSReturn_t OS_TOO_LONG = -8;
  static const OSReturn_t OS_ILLEGAL_CHARACTER = -9;
  static const OSReturn_t OS_BAD_CHECKSUM = -10;
  static const OSReturn_t OS_NOT_INITIALIZED = -11;
  static const OSReturn_t OS_BAD_PARAMETER = -12;
  static const OSReturn_t OS_BUFFER_FULL = -13;
  static const OSReturn_t OS_IMMEDIATELY = -14;
  static const OSReturn_t OS_ITEM_NOT_FOUND = -15;
  static const OSReturn_t OS_NOT_ENOUGH_SPACE = -16;
  static const OSReturn_t OS_BAD_COMMAND = -17;
  static const OSReturn_t OS_NULL_POINTER = -18;
  static const OSReturn_t OS_BAD_SEQUENCE = -19;
  static const OSReturn_t OS_NOT_FOUND = -20;
};

// ----------------------------------------------------------------------------

#endif /* OSRETURN_H_ */
