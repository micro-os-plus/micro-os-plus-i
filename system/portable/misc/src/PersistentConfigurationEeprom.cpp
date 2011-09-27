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
PersistentConfigurationEeprom::checkHeader(void)
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

  Length_t length;
  ret = m_device.readUnsigned16(m_beginAddress + OFFSET_OF_LENGTH, &length);
  if (ret != OSReturn::OS_OK)
    return ret;

  if (length != m_payloadLength)
    {
      OSDeviceDebug::putString(" BAD_LENGTH ");
      return OSReturn::OS_BAD_LENGTH;
    }

  Crc_t crc;
  ret = m_device.readUnsigned8(m_beginAddress + OFFSET_OF_CRC7, &crc);
  if (ret != OSReturn::OS_OK)
    return ret;

  Crc_t computedCrc;
  computeCRC(computedCrc);

  if (crc != computedCrc)
    {
      OSDeviceDebug::putString(" BAD_CHECKSUM ");
      return OSReturn::OS_BAD_CHECKSUM;
    }

  return OSReturn::OS_OK;
}

OSReturn_t
PersistentConfigurationEeprom::computeCRC(Crc_t& crc)
{
  OSReturn_t ret;
  ret = OSReturn::OS_OK;

  crc = 0;

  uint_t i;
  for (i = 0; i < m_payloadLength + ADDITIONAL_LENGTH; ++i)
    {
      uint8_t b;
      ret = m_device.readUnsigned8(
          m_beginAddress + OFFSET_OF_CRC_COMPUTATION_BEGIN + i, &b);
      if (ret != OSReturn::OS_OK)
        return ret;

      util::crc::Crc7x42::computePolynomial(crc, b);
    }

  return ret;
}

OSReturn_t
PersistentConfigurationEeprom::writeHeader(void)
{
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
PersistentConfigurationEeprom::fillPayload(uint8_t fillByte)
{
  OSReturn_t ret;
  ret = OSReturn::OS_OK;

  for (uint_t i = 0; i < m_payloadLength; ++i)
    {
      ret = m_device.writeUnsigned8(m_beginAddress + OFFSET_OF_PAYLOAD + 1,
          fillByte);
      if (ret != OSReturn::OS_OK)
        return ret;
    }

  return ret;
}

OSReturn_t
PersistentConfigurationEeprom::updateChecksum(void)
{
  Crc_t crc;
  crc = 0;

  OSReturn_t ret;
  ret = computeCRC(crc);
  if (ret != OSReturn::OS_OK)
    return ret;

  ret = m_device.writeUnsigned8(m_beginAddress + OFFSET_OF_CRC7, crc);
  return ret;
}

#endif
