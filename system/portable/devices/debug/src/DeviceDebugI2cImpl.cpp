/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(DEBUG) && defined(OS_CONFIG_DEBUG_DEVICE_I2C)

#include "portable/devices/debug/include/DeviceDebugI2cImpl.h"

// ----- private methods ------------------------------------------------------

#if false
unsigned char
DeviceDebugI2cImpl::masterGetByte(void)
  {
    int i;
    unsigned char b = 0;

    masterSdaHigh();
    for (i = 8; i != 0; --i)
      {
        masterDelay(OS_CFGINT_DEBUG_I2C_EMU_DELAY_COUNT / 2);
        masterSclHigh();
        masterSclSynchronize();
        masterDelay(OS_CFGINT_DEBUG_I2C_EMU_DELAY_COUNT / 2);
        b <<= 1;
        if (!masterSdaIsLow())
          {
            b |= 1;
          }
        masterDelay(OS_CFGINT_DEBUG_I2C_EMU_DELAY_COUNT / 2);
        masterSclLow();
        masterDelay(OS_CFGINT_DEBUG_I2C_EMU_DELAY_COUNT / 2);
        ;
      }
    return b;
  }

// Extra half delay at end
void
DeviceDebugI2cImpl::masterPutAck(bool b)
  {
    if (b)
    masterSdaHigh();
    else
    masterSdaLow();

    masterDelay(OS_CFGINT_DEBUG_I2C_EMU_DELAY_COUNT / 2);
    masterSclHigh();
    masterSclSynchronize();
    masterDelay(OS_CFGINT_DEBUG_I2C_EMU_DELAY_COUNT);
    masterSclLow();
    masterDelay(OS_CFGINT_DEBUG_I2C_EMU_DELAY_COUNT / 2);
    masterDelay(OS_CFGINT_DEBUG_I2C_EMU_DELAY_COUNT / 2);
  }
#endif

// Extra half delay at end
bool
DeviceDebugI2cImpl::masterGetAck(void)
{
  bool r;

  masterSdaHigh();
  masterDelay(OS_CFGINT_DEBUG_I2C_EMU_DELAY_COUNT / 2);
  masterSclHigh();
  masterSclSynchronize();
  masterDelay(OS_CFGINT_DEBUG_I2C_EMU_DELAY_COUNT / 2);
  r = masterSdaIsLow();

  masterDelay(OS_CFGINT_DEBUG_I2C_EMU_DELAY_COUNT / 2);
  masterSclLow();
  masterDelay(OS_CFGINT_DEBUG_I2C_EMU_DELAY_COUNT / 2);
  masterDelay(OS_CFGINT_DEBUG_I2C_EMU_DELAY_COUNT / 2);

  return r;
}

void
DeviceDebugI2cImpl::masterCleanup(void)
{
  masterSclHigh();
  masterSdaHigh();
}

// No arbitration implemented yet

void
DeviceDebugI2cImpl::masterStart(void)
{
  masterSdaHigh();
  masterSdaSynchronize();
  masterSclHigh();
  masterSclSynchronize();
  masterDelay(OS_CFGINT_DEBUG_I2C_EMU_DELAY_COUNT / 2);
  masterSdaLow();
  masterDelay( OS_CFGINT_DEBUG_I2C_EMU_DELAY_COUNT);
  masterSclLow();
  masterDelay(OS_CFGINT_DEBUG_I2C_EMU_DELAY_COUNT / 2);
}

void
DeviceDebugI2cImpl::masterPutAddress(unsigned char addr, unsigned char mode)
{
  masterPutByte((addr << 1) | (mode & 1));
}

// Also wait for SDA to go high to confirm stop condition
void
DeviceDebugI2cImpl::masterStop(void)
{
  masterSdaLow();
  masterDelay(OS_CFGINT_DEBUG_I2C_EMU_DELAY_COUNT / 2);
  masterSclHigh();
  masterSclSynchronize();
  masterDelay( OS_CFGINT_DEBUG_I2C_EMU_DELAY_COUNT);
  masterSdaHigh();
  masterSdaSynchronize();
  masterDelay(OS_CFGINT_DEBUG_I2C_EMU_DELAY_COUNT / 2);
}

void
DeviceDebugI2cImpl::masterDelay(int n)
{
  implWDReset();
  for (++n; --n != 0;)
    {
      implWDReset();
    }
}

void
DeviceDebugI2cImpl::masterPutByte(unsigned char b)
{
  for (int i = 8; i != 0; --i, b <<= 1)
    {
      if ((b & 0x80) != 0)
        masterSdaHigh();
      else
        masterSdaLow();

      masterDelay(OS_CFGINT_DEBUG_I2C_EMU_DELAY_COUNT / 2);
      masterSclHigh();
      masterSclSynchronize();
      masterDelay( OS_CFGINT_DEBUG_I2C_EMU_DELAY_COUNT);
      masterSclLow();
      masterDelay(OS_CFGINT_DEBUG_I2C_EMU_DELAY_COUNT / 2);
    }
  masterSdaHigh();
}

#endif /* defined(DEBUG) && defined(OS_CONFIG_DEBUG_DEVICE_I2C) */

