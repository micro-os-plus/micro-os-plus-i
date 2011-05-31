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

  testSpi();

  // Thread endless loop
  for (;;)
    {
      os.sched.timerSeconds.sleep(10);
    }
}

void
Test::testSpi(void)
{
  avr32::uc3::Spim spi0m(avr32::uc3::spi::MODULE_0);

  debug.putString("Spi test...");
  debug.putNewLine();

  spi0m.init();

  // Max speed, no delays, 8 bits/transfer
  spi0m.configChipSelect(1, 0, 0);
  // Enable loopback
  spi0m.registers.writeMode(spi0m.registers.readMode() | (1 << 7));

  spi0m.enable();

  bool failed;
  failed = false;
  for (uint_t i = 0; i < 256; ++i)
    {
      uint8_t r;
      r = spi0m.writeWaitReadByte(i);
      if (r != i)
        {
          debug.putString("byte ");
          debug.putHex(i);
          debug.putString(" received as ");
          debug.putHex(r);
          debug.putNewLine();

          failed = true;
        }
    }

  if (!failed)
    {
      debug.putString("Spi test... pased.");
      debug.putNewLine();
    }
  spi0m.disable();
}

