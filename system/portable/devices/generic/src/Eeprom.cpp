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

  uint8_t b;
  OSReturn_t ret;

  for (uint8_t i = 0; i < (size - 1); ++i)
    {
      ret = readUnsigned8(address++, &b);
      if (ret != OSReturn::OS_OK)
        return ret;

      *pString++ = b;
    }
  *pString = '\0';

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

  for (uint8_t i = 0; i < (size - 1); ++i, ++pString)
    {
      ret = writeUnsigned8(address++, *pString);
      if (ret != OSReturn::OS_OK)
        return ret;

      if (*pString == '\0')
        break;
    }

  if (*pString != '\0')
    {
      ret = writeUnsigned8(address++, '\0');
      if (ret != OSReturn::OS_OK)
        return ret;
    }

  return OSReturn::OS_OK;
}

#endif /* defined(OS_INCLUDE_EEPROM) */

