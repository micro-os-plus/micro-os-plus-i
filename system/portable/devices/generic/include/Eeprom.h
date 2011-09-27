/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef EEPROM_H_
#define EEPROM_H_

#include "portable/kernel/include/OS.h"

class Eeprom
{
public:

  // ----- Public type definitions --------------------------------------------

  typedef uint16_t Address_t;
  typedef uint16_t BytesCount_t;

  // ----- Constructors & destructors -----------------------------------------

  Eeprom();

  virtual
  ~Eeprom();

  // ----- Virtual methods ----------------------------------------------------

  virtual void
  powerUp(void);

  virtual void
  powerDown(void);

  virtual OSReturn_t
  readByteArray(Address_t address, uint8_t* pIncomingBytes,
      BytesCount_t incomingBytesSize, size_t* pIncomingBytesLength) = 0;

  OSReturn_t
  readUnsigned8(Address_t address, uint8_t* pValue);

  OSReturn_t
  readUnsigned16(Address_t address, uint16_t* pValue);

  OSReturn_t
  readUnsigned32(Address_t address, uint32_t* pValue);

  // Read a null terminated string, up to the given size
  OSReturn_t
  readString(Address_t address, uint8_t* pString, BytesCount_t size);

  virtual OSReturn_t
  writeByteArray(Address_t address, uint8_t* pOutgoingBytes,
      BytesCount_t outgoingBytesLength) = 0;

  OSReturn_t
  writeUnsigned8(Address_t address, uint8_t value);

  OSReturn_t
  writeUnsigned16(Address_t address, uint16_t value);

  OSReturn_t
  writeUnsigned32(Address_t address, uint32_t value);

  // Write a null terminated string, up to the given size
  OSReturn_t
  writeString(Address_t address, uint8_t* pString, BytesCount_t size);

  // --------------------------------------------------------------------------
};

#endif /* EEPROM_H_ */
