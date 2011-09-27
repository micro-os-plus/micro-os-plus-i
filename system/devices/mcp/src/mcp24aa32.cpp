/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_DEVICE_MCP_MCP24AA32)

#include "devices/mcp/include/mcp24aa32.h"

namespace device
{
  namespace mcp
  {

    Mcp24aa32::Mcp24aa32(I2c_t& device) :
      m_i2cDevice(device)
    {
      OSDeviceDebug::putConstructor("device::mcp::Mcp24aa32", this);
    }

    Mcp24aa32::~Mcp24aa32()
    {
      OSDeviceDebug::putDestructor("device::mcp::Mcp24aa32", this);
    }

    void
    Mcp24aa32::powerUp(void)
    {
      m_i2cDevice.powerUp();
    }

    void
    Mcp24aa32::powerDown(void)
    {
      m_i2cDevice.powerDown();
    }

    void
    Mcp24aa32::initialise(void)
    {
      m_i2cDevice.initialise();
    }

    OSReturn_t
    Mcp24aa32::readByteArray(Address_t address, uint8_t* pIncomingBytes,
        BytesCount_t incomingBytesSize, size_t* pIncomingBytesLength)
    {
      DEBUG_ASSERT(pIncomingBytes != NULL);
#if defined(OS_DEBUG_DEVICE_MCP_MCP24AA32_READBYTEARRAY)
      OSDeviceDebug::putString(" readByteArray(");
      OSDeviceDebug::putDec(address);
      OSDeviceDebug::putChar(',');
      OSDeviceDebug::putDec(incomingBytesSize);
      OSDeviceDebug::putString(")=");
#endif /* defined(OS_DEBUG_DEVICE_MCP_MCP24AA32_READBYTEARRAY) */

      uint8_t outgoingBytes[2];
      outgoingBytes[0] = ((address >> 8) & 0xFF);
      outgoingBytes[1] = ((address) & 0xFF);

      OSReturn_t ret;
      ret = m_i2cDevice.writeByteArrayReadByteArray(m_i2cAddress,
          outgoingBytes, sizeof(outgoingBytes), pIncomingBytes,
          (size_t) incomingBytesSize, (size_t*) pIncomingBytesLength);
      if (ret != OSReturn::OS_OK)
        return ret;

#if defined(OS_DEBUG_DEVICE_MCP_MCP24AA32_READBYTEARRAY)
      if (pIncomingBytesLength != NULL)
        {
          OSDeviceDebug::putDec(*pIncomingBytesLength);
          OSDeviceDebug::putNewLine();
        }
      else
        {
          OSDeviceDebug::putString(" pIn_null ");
        }
#endif /* defined(OS_DEBUG_DEVICE_MCP_MCP24AA32_READBYTEARRAY) */

      return ret;
    }

    OSReturn_t
    Mcp24aa32::writeByteArray(Address_t address, uint8_t* pOutgoingBytes,
        BytesCount_t outgoingBytesLength)
    {
#if defined(OS_DEBUG_DEVICE_MCP_MCP24AA32_WRITEBYTEARRAY)
      OSDeviceDebug::putString(" writeByteArray(");
      OSDeviceDebug::putDec(address);
      OSDeviceDebug::putChar(',');
      OSDeviceDebug::putDec(outgoingBytesLength);
      OSDeviceDebug::putString(")");
      OSDeviceDebug::putNewLine();
#endif /* defined(OS_DEBUG_DEVICE_MCP_MCP24AA32_WRITEBYTEARRAY) */

      uint8_t outgoingBytes[2 + 4];
      outgoingBytes[0] = ((address >> 8) & 0xFF);
      outgoingBytes[1] = ((address) & 0xFF);
      memcpy(outgoingBytes + 2, pOutgoingBytes, 4);

      size_t firstSliceLength;
      if (outgoingBytesLength <= 4)
        {
          firstSliceLength = 2 + outgoingBytesLength;
          outgoingBytesLength = 0;
        }
      else
        {
          firstSliceLength = 2 + 4;
          outgoingBytesLength -= 4;
          pOutgoingBytes += 4;
        }

      OSReturn_t ret;
      ret = m_i2cDevice.writeByteArray(m_i2cAddress, outgoingBytes,
          firstSliceLength);
      if (ret != OSReturn::OS_OK)
        return ret;

      if (outgoingBytesLength > 0)
        {
          ret = m_i2cDevice.writeByteArray(m_i2cAddress, pOutgoingBytes,
              outgoingBytesLength);
        }

      return ret;
    }
  }
}

#endif /* defined(OS_INCLUDE_DEVICE_MCP_MCP24AA32) */
