/*
 *	Copyright (C) 2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "Test.h"

#include "hal/arch/avr32/uc3/devices/onchip/include/Spi.h"
#include "hal/arch/avr32/uc3/devices/onchip/include/Pdca.h"

/*
 * Active object constructor.
 * Initialise parent system thread and store parameters in private members.
 */

avr32::uc3::PdcaReceive Test::m_pdcaReceive(avr32::uc3::pdca::CHANNEL_0);
avr32::uc3::PdcaTransmit Test::m_pdcaTransmit(avr32::uc3::pdca::CHANNEL_1);

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

#if false
  testAlloca(77);
#endif

#if true
  testSpi(avr32::uc3::spi::MODULE_0, avr32::uc3::spi::BITS_8);
  testSpi(avr32::uc3::spi::MODULE_0, avr32::uc3::spi::BITS_16);
  testSpi(avr32::uc3::spi::MODULE_1, avr32::uc3::spi::BITS_8);
  testSpi(avr32::uc3::spi::MODULE_1, avr32::uc3::spi::BITS_16);
#endif

#if true
  testPdcaSpiSingleTransfer();
#endif

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


#define TEST_PDCA_SPI_BUFF_SIZE         256
// test for a single buffer transfer on SPI using PDCA
int
Test::testPdcaSpiSingleTransfer(void)
{
  avr32::uc3::Spim spi0m(avr32::uc3::spi::MODULE_0);
  //avr32::uc3::PdcaReceive m_pdcaReceive(avr32::uc3::pdca::CHANNEL_0);
  //avr32::uc3::PdcaTransmit m_pdcaTransmit(avr32::uc3::pdca::CHANNEL_1);

  uint8_t rxBuf[TEST_PDCA_SPI_BUFF_SIZE];
  uint8_t txBuf[TEST_PDCA_SPI_BUFF_SIZE];
  avr32::uc3::pdca::Region_t rxRegion[1];
  avr32::uc3::pdca::Region_t txRegion[1];

  debug.putString("Pdca-Spi Single Transfer Test start");
  debug.putNewLine();

  spi0m.init();
  spi0m.enableLocalLoopback();

  // 1Mbps speed, no delays, 8 bits/transfer
  spi0m.configChipSelect(1, 0, avr32::uc3::spi::BITS_8);

  // set SPI0 RX and TX pdma channels
  m_pdcaReceive.setPeripheralId(avr32::uc3::pdca::SPI0_RX);
  m_pdcaTransmit.setPeripheralId(avr32::uc3::pdca::SPI0_TX);

  // create buffers
  for (int i = 0 ; i < TEST_PDCA_SPI_BUFF_SIZE; i++)
    {
      rxBuf[i] = 0;
      txBuf[i] = (uint8_t)i;
    }
  rxRegion[0].address = rxBuf;
  rxRegion[0].size = TEST_PDCA_SPI_BUFF_SIZE;
  txRegion[0].address = txBuf;
  txRegion[0].size = TEST_PDCA_SPI_BUFF_SIZE;

  m_pdcaReceive.setRegionsArray(rxRegion, 1, false);
  m_pdcaTransmit.setRegionsArray(txRegion, 1, false);

  m_pdcaReceive.registerInterruptHandler(testPdcaSpiSingleTransferRxHandler);
  m_pdcaTransmit.registerInterruptHandler(testPdcaSpiSingleTransferTxHandler);

  m_pdcaReceive.prepareTransfer();
  m_pdcaTransmit.prepareTransfer();

  debug.putString("Transfer started");
  debug.putNewLine();

  spi0m.enable();
  m_pdcaReceive.startTransfer();
  m_pdcaTransmit.startTransfer();

  // busy wait - low level version
//  while ((m_pdcaReceive.registers.readTransferCounter() !=0 ) ||
//        (m_pdcaTransmit.registers.readTransferCounter() !=0 ))
//    ;

  // busy wait - high level version
//  avr32::uc3::pdca::Status_t txStatus, rxStatus;
//  do
//    {
//      txStatus = m_pdcaTransmit.getStatus();
//      rxStatus = m_pdcaReceive.getStatus();
//    }while (txStatus == avr32::uc3::pdca::STATUS_BUSY ||
//        rxStatus == avr32::uc3::pdca::STATUS_BUSY);
//  debug.putString("status not busy");
//  debug.putNewLine();

  // block until transfer is over
  avr32::uc3::pdca::RegionAddress_t regionAddress;
  m_pdcaReceive.readRegion(regionAddress, false);
  debug.putString("Transfer finished");
  debug.putNewLine();

  bool failed = false;

  // check the pdca(transfer) status
  if ((m_pdcaTransmit.getStatus() != avr32::uc3::pdca::STATUS_OK) ||
      (m_pdcaReceive.getStatus() != avr32::uc3::pdca::STATUS_OK))
    {
      debug.putString("wrong status");
      failed = true;
    }
  // check the transferred data


  for (int i = 0 ; i < TEST_PDCA_SPI_BUFF_SIZE; i++)
    {
      if (rxBuf[i] != txBuf[i])
      {
        debug.putString("sent: ");
        debug.putHex(txBuf[i]);
        debug.putString(" received as ");
        debug.putHex(rxBuf[i]);
        debug.putNewLine();

        failed = true;
      }
    }

  if (!failed)
    {
      debug.putString("Pdca-Spi Single Transfer Test... passed.");
      debug.putNewLine();
    }
  else
    {
      debug.putString("Pdca-Spi Single Transfer Test... failed.");
      debug.putNewLine();
    }

  spi0m.disableLocalLoopback();
  spi0m.disable();
  return 1;
}

__attribute__((__interrupt__)) void
Test::testPdcaSpiSingleTransferRxHandler( void)
{
  debug.putString("+");
  m_pdcaReceive.interruptServiceRoutine();
}

__attribute__((__interrupt__)) void
Test::testPdcaSpiSingleTransferTxHandler( void)
{
  debug.putString("-");
  m_pdcaTransmit.interruptServiceRoutine();
}
