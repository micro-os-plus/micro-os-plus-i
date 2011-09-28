/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_EEPROM)

#include "portable/kernel/include/OS.h"

#include "portable/devices/generic/include/Eeprom.h"
#include "portable/util/endianness/include/BigEndian.h"

Eeprom::Eeprom()
{
  OSDeviceDebug::putConstructor_P(PSTR("Eeprom"), this);
}

Eeprom::~Eeprom()
{
  OSDeviceDebug::putDestructor_P(PSTR("Eeprom"), this);
}

void
Eeprom::powerUp(void)
{
  return;
}

void
Eeprom::powerDown(void)
{
  return;
}

OSReturn_t
Eeprom::readUnsigned8(Address_t address, uint8_t* pValue)
{
  DEBUG_ASSERT(pValue != NULL);

  uint8_t ba[1];
  size_t incomingLength;

  OSReturn_t ret;
  ret = readByteArray(address, ba, sizeof(ba), &incomingLength);
  if (ret != OSReturn::OS_OK)
    return ret;

  if (incomingLength != 1)
    {
      OSDeviceDebug::putString(" NOT_ENOUGH_DATA ");
      return OSReturn::OS_NOT_ENOUGH_DATA;
    }
  *pValue = ba[0];

  return OSReturn::OS_OK;
}

OSReturn_t
Eeprom::readUnsigned16(Address_t address, uint16_t* pValue)
{
  DEBUG_ASSERT(pValue != NULL);

  uint8_t ba[2];
  size_t incomingLength;

  OSReturn_t ret;
  ret = readByteArray(address, ba, sizeof(ba), &incomingLength);
  if (ret != OSReturn::OS_OK)
    return ret;

  if (incomingLength != 2)
    {
      OSDeviceDebug::putString(" NOT_ENOUGH_DATA ");
      return OSReturn::OS_NOT_ENOUGH_DATA;
    }

  *pValue = util::endianness::BigEndian::readUnsigned16(ba);

  return OSReturn::OS_OK;
}

OSReturn_t
Eeprom::readUnsigned32(Address_t address, uint32_t* pValue)
{
  DEBUG_ASSERT(pValue != NULL);

  uint8_t ba[4];
  size_t incomingLength;

  OSReturn_t ret;
  ret = readByteArray(address, ba, sizeof(ba), &incomingLength);
  if (ret != OSReturn::OS_OK)
    return ret;

  if (incomingLength != 4)
    {
      OSDeviceDebug::putString(" NOT_ENOUGH_DATA ");
      return OSReturn::OS_NOT_ENOUGH_DATA;
    }

  *pValue = util::endianness::BigEndian::readUnsigned32(ba);

  return OSReturn::OS_OK;
}

OSReturn_t
Eeprom::readString(Address_t address, uint8_t* pString, BytesCount_t size)
{
  DEBUG_ASSERT(pString != NULL);
  DEBUG_ASSERT(size > 1);

  OSReturn_t ret;
  size_t incomingBytesLength;

  ret = readByteArray(address, pString, size, &incomingBytesLength);
  if (ret != OSReturn::OS_OK)
     return ret;

  pString[incomingBytesLength-1] = '\0';

  return OSReturn::OS_OK;
}

OSReturn_t
Eeprom::writeUnsigned8(Address_t address, uint8_t value)
{
  uint8_t ba[1];

  ba[0] = value;

  OSReturn_t ret;
  ret = writeByteArray(address, ba, sizeof(ba));
  if (ret != OSReturn::OS_OK)
    return ret;

  return OSReturn::OS_OK;
}

OSReturn_t
Eeprom::writeUnsigned16(Address_t address, uint16_t value)
{
  uint8_t ba[2];

  util::endianness::BigEndian::writeUnsigned16(value, ba);

  OSReturn_t ret;
  ret = writeByteArray(address, ba, sizeof(ba));
  if (ret != OSReturn::OS_OK)
    return ret;

  return OSReturn::OS_OK;
}

OSReturn_t
Eeprom::writeUnsigned32(Address_t address, uint32_t value)
{
  uint8_t ba[4];

  util::endianness::BigEndian::writeUnsigned32(value, ba);

  OSReturn_t ret;
  ret = writeByteArray(address, ba, sizeof(ba));
  if (ret != OSReturn::OS_OK)
    return ret;

  return OSReturn::OS_OK;
}

OSReturn_t
Eeprom::writeString(Address_t address, uint8_t* pString, BytesCount_t size)
{
  DEBUG_ASSERT(pString != NULL);
  DEBUG_ASSERT(size > 1);

  OSReturn_t ret;

  size_t len;
  len = strlen((char*) pString);

  size_t lenToWrite;
  lenToWrite = len + 1; // Include terminating '\0'

  if (lenToWrite > size)
    {
      lenToWrite = size-1; // Leave space for terminating '\0'
    }
  ret = writeByteArray(address, pString, lenToWrite);
  if (ret != OSReturn::OS_OK)
    return ret;

  if (lenToWrite != (len + 1))
    {
      // Add the terminator when the string was too long
      ret = writeUnsigned8(address + lenToWrite, '\0');
      if (ret != OSReturn::OS_OK)
        return ret;
    }
  return OSReturn::OS_OK;
}

#endif /* defined(OS_INCLUDE_EEPROM) */

