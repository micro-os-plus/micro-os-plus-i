#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_PERSISTENTCONFIGURATIONEEPROM)

#include "portable/kernel/include/OS.h"

#include "portable/misc/include/PersistentConfigurationEeprom.h"
#include "portable/util/crc/include/Crc7x42.h"

PersistentConfigurationEeprom::PersistentConfigurationEeprom(Eeprom& device) :
  m_device(device)
{
  debug.putConstructor("PersistentConfigurationEeprom", this);

  m_version = 0;
  m_magic = MAGIC;
  m_payloadLength = 0;
  m_beginAddress = 0;
  m_persistentPayloadLength = 0;
}

PersistentConfigurationEeprom::~PersistentConfigurationEeprom()
{
  debug.putDestructor("PersistentConfigurationEeprom", this);
}

void
PersistentConfigurationEeprom::initialise(void)
{
  //m_device.initialise();
}

OSReturn_t
PersistentConfigurationEeprom::readPersistentLength(void)
{
  OSReturn_t ret;
  if (sizeof(Length_t) == 2)
    {
      ret = m_device.readUnsigned16(m_beginAddress + OFFSET_OF_LENGTH,
          &m_persistentPayloadLength);
    }
  else
    {
      OSDeviceDebug::putString(" NOT_IMPLEMENTED ");
      return OSReturn::OS_NOT_IMPLEMENTED;
    }
  if (ret != OSReturn::OS_OK)
    return ret;

  return OSReturn::OS_OK;
}

OSReturn_t
PersistentConfigurationEeprom::verifyHeader(void)
{
  Version_t version;

  OSReturn_t ret;
  ret = m_device.readUnsigned8(m_beginAddress + OFFSET_OF_VERSION, &version);
  if (ret != OSReturn::OS_OK)
    return ret;

  if (version != m_version)
    {
      OSDeviceDebug::putString(" BAD_VERSION ");
      return OSReturn::OS_BAD_VERSION;
    }

  Magic_t magic;

  ret = m_device.readUnsigned16(m_beginAddress + OFFSET_OF_MAGIC, &magic);
  if (ret != OSReturn::OS_OK)
    return ret;

  if (magic != m_magic)
    {
      OSDeviceDebug::putString(" BAD_MAGIC ");
      return OSReturn::OS_BAD_MAGIC;
    }

  // Compute the checksum for the entire stored content
  ret = verifyPersistentChecksum();
  if (ret != OSReturn::OS_OK)
    return ret;

  // Finally compare the length. The application may or may not reuse the
  // existing content.
  ret = readPersistentLength();
  if (m_persistentPayloadLength != m_payloadLength)
    {
      OSDeviceDebug::putString(" BAD_LENGTH ");
      return OSReturn::OS_BAD_LENGTH;
    }

  return OSReturn::OS_OK;
}

OSReturn_t
PersistentConfigurationEeprom::computeChecksum(Length_t length, Crc_t& crc)
{
  OSReturn_t ret;
  ret = OSReturn::OS_OK;

  crc = util::crc::Crc7x42::INITIAL_VALUE;

  Length_t remainingLength;
  remainingLength = length + ADDITIONAL_LENGTH;

  Eeprom::Address_t address;
  address = m_beginAddress + OFFSET_OF_CRC_COMPUTATION_BEGIN;

  // As an optimisation, we process slices of bytes, that we read in a local
  // byte array with one call and compute crc again with one call.
  while (remainingLength > 0)
    {
      uint8_t ba[16];

      Length_t sliceLength;
      sliceLength = (remainingLength > sizeof(ba)) ? sizeof(ba)
          : remainingLength;

      size_t incomingBytesLength;

      ret = m_device.readByteArray(address, ba, sliceLength,
          &incomingBytesLength);
      if (ret != OSReturn::OS_OK)
        return ret;

      if (incomingBytesLength != sliceLength)
        {
          OSDeviceDebug::putString(" NOT_ENOUGH_DATA ");
          return OSReturn::OS_NOT_ENOUGH_DATA;
        }

      util::crc::Crc7x42::computePolynomial(crc, ba, sliceLength);

      remainingLength -= sliceLength;
      address += sliceLength;
    }

  return ret;
}

OSReturn_t
PersistentConfigurationEeprom::verifyPersistentChecksum(void)
{
  OSReturn_t ret;

  if (m_persistentPayloadLength == 0)
    {
      ret = readPersistentLength();
      if (ret != OSReturn::OS_OK)
        return ret;
    }

  Crc_t crc;
  ret = computeChecksum(m_persistentPayloadLength, crc);
  if (ret != OSReturn::OS_OK)
    return ret;

  Crc_t finalValue;
  finalValue = 0;
  util::crc::Crc7x42::computePolynomial(crc, finalValue);

  if (!util::crc::Crc7x42::isFinalValueCorrect(finalValue))
    {
      OSDeviceDebug::putString(" fV=");
      OSDeviceDebug::putHex(finalValue);
      OSDeviceDebug::putString(" BAD_CHECKSUM ");
      return OSReturn::OS_BAD_CHECKSUM;
    }

  return OSReturn::OS_OK;
}

OSReturn_t
PersistentConfigurationEeprom::updatePersistentHeader(void)
{
  OSDeviceDebug::putString(" updatePersistentHeader() ");

  OSReturn_t ret;

  if (m_payloadLength == 0)
    {
      OSDeviceDebug::putString(" NOT_INITIALISED ");
      return OSReturn::OS_NOT_INITIALISED;
    }

  ret = m_device.writeUnsigned8(m_beginAddress + OFFSET_OF_VERSION, m_version);
  if (ret != OSReturn::OS_OK)
    return ret;

  ret = m_device.writeUnsigned16(m_beginAddress + OFFSET_OF_MAGIC, m_magic);
  if (ret != OSReturn::OS_OK)
    return ret;

  ret = m_device.writeUnsigned16(m_beginAddress + OFFSET_OF_LENGTH,
      m_payloadLength);
  if (ret != OSReturn::OS_OK)
    return ret;

  return OSReturn::OS_OK;
}

OSReturn_t
PersistentConfigurationEeprom::fillEntirePayload(uint8_t fillByte)
{
  OSDeviceDebug::putString(" fillEntirePayload() ");

  OSReturn_t ret;

  // Fill all payload bytes
  ret = fillLastBytesOfPayload(0, fillByte);

  return ret;
}

OSReturn_t
PersistentConfigurationEeprom::fillLastBytesOfPayload(Length_t startFrom,
    uint8_t fillByte)
{
  OSDeviceDebug::putString(" fillLastBytesOfPayload() ");

  OSReturn_t ret;
  ret = OSReturn::OS_OK;

  for (uint_t i = startFrom; i < m_payloadLength; ++i)
    {
      ret = m_device.writeUnsigned8(m_beginAddress + OFFSET_OF_PAYLOAD + 1,
          fillByte);
      if (ret != OSReturn::OS_OK)
        return ret;
    }

  return ret;
}

OSReturn_t
PersistentConfigurationEeprom::updatePersistentChecksum(void)
{
  OSDeviceDebug::putString(" updatePersistentChecksum() ");

  OSReturn_t ret;

  Crc_t crc;
  crc = 0;

  crc = util::crc::Crc7x42::INITIAL_VALUE;
  ret = computeChecksum(m_payloadLength, crc);
  if (ret != OSReturn::OS_OK)
    return ret;

  ret = m_device.writeUnsigned8(m_beginAddress + OFFSET_OF_CRC7, crc);
  return ret;
}

#endif
