#include "portable/kernel/include/uOS.h"

#include "portable/devices/generic/include/Eeprom.h"

#ifndef PERSISTENTCONFIGURATIONEEPROM_H_
#define PERSISTENTCONFIGURATIONEEPROM_H_

class PersistentConfigurationEeprom
{
public:

  // ----- Public type definitions --------------------------------------------

  typedef Eeprom::Address_t Offset_t;
  typedef std::size_t Size_t;

  typedef uint8_t Version_t;
  typedef uint8_t Crc_t;
  typedef uint16_t Magic_t;
  typedef uint16_t Length_t;

  // This should be different from the eeprom default value
  const static Magic_t MAGIC = 0x55AA;

  // Header definitions
  const static Offset_t OFFSET_OF_VERSION = 0;
  const static Size_t SIZE_OF_VERSION = sizeof(Version_t);

  const static Offset_t OFFSET_OF_CRC7 = OFFSET_OF_VERSION + SIZE_OF_VERSION;
  const static Size_t SIZE_OF_CRC7 = sizeof(Crc_t);

  const static Offset_t OFFSET_OF_MAGIC = OFFSET_OF_CRC7 + SIZE_OF_CRC7;
  const static Size_t SIZE_OF_MAGIC = sizeof(Magic_t);

  const static Offset_t OFFSET_OF_LENGTH = OFFSET_OF_MAGIC + SIZE_OF_MAGIC;
  const static Size_t SIZE_OF_LENGTH = sizeof(Length_t);

  const static Size_t SIZE_OF_HEADER = SIZE_OF_VERSION + SIZE_OF_CRC7
      + SIZE_OF_MAGIC + SIZE_OF_LENGTH;
  const static Offset_t OFFSET_OF_PAYLOAD = OFFSET_OF_LENGTH + SIZE_OF_LENGTH;

  const static Offset_t OFFSET_OF_CRC_COMPUTATION_BEGIN = OFFSET_OF_MAGIC;
  const static Size_t ADDITIONAL_LENGTH = SIZE_OF_MAGIC + SIZE_OF_LENGTH;

  PersistentConfigurationEeprom(Eeprom& device);
  ~PersistentConfigurationEeprom();

  void
  initialise(void);

  void
  setVersion(Version_t version);

  void
  setPayloadLength(Length_t length);

  void
  setBeginAddress(Eeprom::Address_t beginAddress);

  Eeprom&
  getDevice(void);

  OSReturn_t
  checkHeader(void);

  OSReturn_t
  computeCRC(Crc_t& crc);

  OSReturn_t
  writeHeader(void);

  OSReturn_t
  fillPayload(uint8_t fillByte);

  OSReturn_t
  updateChecksum(void);

private:

  // ----- Private members ----------------------------------------------------

  Eeprom& m_device;

  Eeprom::Address_t m_beginAddress;

  Version_t m_version;
  Magic_t m_magic;
  Length_t m_payloadLength;

  // --------------------------------------------------------------------------
};

inline void
PersistentConfigurationEeprom::setVersion(Version_t version)
{
  m_version = version;
}

inline void
PersistentConfigurationEeprom::setPayloadLength(Length_t length)
{
  m_payloadLength = length;
}

inline void
PersistentConfigurationEeprom::setBeginAddress(Eeprom::Address_t beginAddress)
{
  m_beginAddress = beginAddress;
}

inline Eeprom&
PersistentConfigurationEeprom::getDevice(void)
{
  return m_device;
}

#endif /* PERSISTENTCONFIGURATIONEEPROM_H_ */
