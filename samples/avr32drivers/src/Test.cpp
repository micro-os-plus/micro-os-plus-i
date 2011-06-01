/*
 *	Copyright (C) 2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "Test.h"

#include "hal/arch/avr32/uc3/devices/onchip/include/Spi.h"

/*
 * Active object constructor.
 * Initialise parent system thread and store parameters in private members.
 */

Test::Test(void) :
  OSThread("Test", m_stack, sizeof(m_stack))
{
  debug.putConstructor_P(PSTR("Test"), this);
}

/*
 * Thread main code.
 */

void
Test::threadMain(void)
{
  if (os.isDebug())
    {
      os.sched.lock.enter();
        {
          debug.putString("Thread '");
          debug.putString(getName());
          debug.putString("'");
          debug.putNewLine();
        }
      os.sched.lock.exit();
    }

#if true
  testAlloca(77);
#endif

  testSpi(avr32::uc3::spi::MODULE_0, avr32::uc3::spi::BITS_8);
  testSpi(avr32::uc3::spi::MODULE_0, avr32::uc3::spi::BITS_16);
  testSpi(avr32::uc3::spi::MODULE_1, avr32::uc3::spi::BITS_8);
  testSpi(avr32::uc3::spi::MODULE_1, avr32::uc3::spi::BITS_16);
  // Thread endless loop
  for (;;)
    {
      os.sched.timerSeconds.sleep(10);
    }
}

void
Test::testSpi(avr32::uc3::spi::ModuleId_t moduleId,
    avr32::uc3::spi::BitsPerTransfer_t bitsPerTransfer)
{
  //avr32::uc3::Spim spi0m(avr32::uc3::spi::MODULE_1);
  avr32::uc3::Spim spi0m(moduleId);

  debug.putString("Spi test start");
  debug.putNewLine();
  debug.putString("Module ");
  debug.putHex((uint8_t) moduleId);
  debug.putNewLine();

  if (bitsPerTransfer == avr32::uc3::spi::BITS_8)
    debug.putString("Bits per transfer 8");
  else
    debug.putString("Bits per transfer 16");
  debug.putNewLine();

  spi0m.init();
  spi0m.enableLocalLoopback();

  // 1Mbps speed, no delays, 8 bits/transfer
  spi0m.configChipSelect(16, 0, bitsPerTransfer);

  spi0m.enable();

  bool failed;
  failed = false;
  for (uint32_t i = 0; i < 100; ++i)
    {
      uint32_t valueReceived, valueSent;

      if (bitsPerTransfer == avr32::uc3::spi::BITS_8)
        {
          valueSent = i & 0xFF;
          valueReceived = spi0m.writeWaitReadByte(valueSent);
        }
      else
        {
          valueSent = i;
          valueReceived = spi0m.writeWaitReadWord(valueSent);
        }
      if (valueReceived != valueSent)
        {
          debug.putString("word ");
          debug.putHex(valueSent);
          debug.putString(" received as ");
          debug.putHex(valueReceived);
          debug.putNewLine();

          failed = true;
        }
    }

  if (!failed)
    {
      debug.putString("Spi test... passed.");
      debug.putNewLine();
    }
  else
    {
      debug.putString("Spi test... failed.");
      debug.putNewLine();
    }
  spi0m.disableLocalLoopback();
  spi0m.disable();
}

int
Test::testAlloca(int x)
{
  register OSStack_t tmp1;

  tmp1 = OSCPUImpl::getInterruptsMask();
  *((OSStack_t*) __builtin_alloca(1)) = tmp1;
  tmp1 |= (OS_CFGINT_OSCRITICALSECTION_MASK);
  OSCPUImpl::setInterruptsMask(tmp1);

  debug.putString("alloca 1 ");
  debug.putNewLine();

  register OSStack_t tmp2;

  tmp2 = OSCPUImpl::stackPop();
  OSCPUImpl::setInterruptsMask(tmp2);

  register OSStack_t tmp3;

  tmp3 = OSCPUImpl::getInterruptsMask();
  *((OSStack_t*) __builtin_alloca(1)) = tmp3;
  tmp3 |= (OS_CFGINT_OSCRITICALSECTION_MASK);
  OSCPUImpl::setInterruptsMask(tmp3);

  debug.putString("alloca 2 ");
  debug.putNewLine();

  register OSStack_t tmp4;

  tmp4 = OSCPUImpl::stackPop();
  OSCPUImpl::setInterruptsMask(tmp4);

  return x + 123;
}
