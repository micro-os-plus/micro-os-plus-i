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

Test::Test(void) :
  OSThread("Test", m_stack, sizeof(m_stack)),
      m_pdcaReceive(avr32::uc3::pdca::CHANNEL_0),
      m_pdcaTransmit(avr32::uc3::pdca::CHANNEL_1)
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
  testSpi(avr32::uc3::spi::MODULE_0, avr32::uc3::spi::BITS_8);
  testSpi(avr32::uc3::spi::MODULE_0, avr32::uc3::spi::BITS_16);
  testSpi(avr32::uc3::spi::MODULE_1, avr32::uc3::spi::BITS_8);
  testSpi(avr32::uc3::spi::MODULE_1, avr32::uc3::spi::BITS_16);
#endif

#if true
  testPdcaSpiSingleTransfer();
#endif

#if true
  testPdcaSpiMultipleTransfer();
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

  debug.putNewLine();
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

// test for a single buffer transfer on SPI using PDCA
int
Test::testPdcaSpiSingleTransfer(void)
{
  avr32::uc3::Spim spi0m(avr32::uc3::spi::MODULE_0);
  //avr32::uc3::PdcaReceive m_pdcaReceive(avr32::uc3::pdca::CHANNEL_0);
  //avr32::uc3::PdcaTransmit m_pdcaTransmit(avr32::uc3::pdca::CHANNEL_1);

  avr32::uc3::pdca::Region_t rxRegion[1];
  avr32::uc3::pdca::Region_t txRegion[1];

  debug.putNewLine();
  debug.putString("Pdca-Spi Single Transfer Test start");
  debug.putNewLine();

  spi0m.init();
  spi0m.enableLocalLoopback();

  // 1Mbps speed, no delays, 8 bits/transfer
  spi0m.configChipSelect(255, 0, avr32::uc3::spi::BITS_8);

  // set SPI0 RX and TX pdma channels
  m_pdcaReceive.setPeripheralId(avr32::uc3::pdca::SPI0_RX);
  m_pdcaTransmit.setPeripheralId(avr32::uc3::pdca::SPI0_TX);

  // create buffers
  for (int i = 0; i < TEST_PDCA_SPI_BUFF_SIZE; i++)
    {
      m_singleTransfBufRx[i] = 0;
      m_singleTransfBufTx[i] = (uint8_t) i;
    }
  rxRegion[0].address = m_singleTransfBufRx;
  rxRegion[0].size = TEST_PDCA_SPI_BUFF_SIZE;
  txRegion[0].address = m_singleTransfBufTx;
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
  if ((m_pdcaTransmit.getStatus() != avr32::uc3::pdca::STATUS_OK)
      || (m_pdcaReceive.getStatus() != avr32::uc3::pdca::STATUS_OK))
    {
      debug.putString("wrong status");
      failed = true;
    }
  // check the transferred data


  for (int i = 0; i < TEST_PDCA_SPI_BUFF_SIZE; i++)
    {
      if (m_singleTransfBufRx[i] != m_singleTransfBufTx[i])
        {
          debug.putString("sent: ");
          debug.putHex(m_singleTransfBufTx[i]);
          debug.putString(" received as ");
          debug.putHex(m_singleTransfBufRx[i]);
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

  // make sure everything is disabled and stopped
  spi0m.disableLocalLoopback();
  spi0m.disable();
  m_pdcaTransmit.stopTransfer();
  m_pdcaReceive.stopTransfer();
  return 1;
}

// test for a single buffer transfer on SPI using PDCA
int
Test::testPdcaSpiMultipleTransfer(void)
{
  avr32::uc3::Spim spi0m(avr32::uc3::spi::MODULE_0);
  avr32::uc3::pdca::Region_t rxRegion[TEST_PDCA_SPI_BUFF_NUM];
  avr32::uc3::pdca::Region_t txRegion[TEST_PDCA_SPI_BUFF_NUM];

  debug.putNewLine();
  debug.putString("Pdca-Spi Multiple Transfer Test start");
  debug.putNewLine();

  spi0m.init();
  spi0m.enableLocalLoopback();

  // 1Mbps speed, no delays, 8 bits/transfer
  spi0m.configChipSelect(255, 0, avr32::uc3::spi::BITS_8);

  // set SPI0 RX and TX pdma channels
  m_pdcaReceive.setPeripheralId(avr32::uc3::pdca::SPI0_RX);
  m_pdcaTransmit.setPeripheralId(avr32::uc3::pdca::SPI0_TX);

  // create buffers
  for (uint16_t regIdx = 0; regIdx < TEST_PDCA_SPI_BUFF_NUM; regIdx++)
    {
      for (uint16_t i = 0; i < TEST_PDCA_SPI_BUFF_SIZE2; i++)
        {
          m_multTransfBufRx[regIdx][i] = 0;
          m_multTransfBufTx[regIdx][i] = (uint8_t) (regIdx
              * TEST_PDCA_SPI_BUFF_SIZE2 + i);
        }
      rxRegion[regIdx].address = m_multTransfBufRx[regIdx];
      rxRegion[regIdx].size = TEST_PDCA_SPI_BUFF_SIZE2;
      txRegion[regIdx].address = m_multTransfBufTx[regIdx];
      txRegion[regIdx].size = TEST_PDCA_SPI_BUFF_SIZE2;
    }

  // set regions circular
  m_pdcaReceive.setRegionsArray(rxRegion, TEST_PDCA_SPI_BUFF_NUM, true);
  m_pdcaTransmit.setRegionsArray(txRegion, TEST_PDCA_SPI_BUFF_NUM, true);

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

  bool failed = false;

  avr32::uc3::pdca::RegionAddress_t regionAddress;

  for (uint16_t transferNum = 0; transferNum < 2 * TEST_PDCA_SPI_BUFF_NUM; transferNum++)
    {
      debug.putString("Transfer ");
      debug.putDec(transferNum);

      // block until transfer is over
      m_pdcaReceive.readRegion(regionAddress, false);

      debug.putString(" finished");
      debug.putNewLine();

      // check the pdca(transfer) status
      if ((m_pdcaTransmit.getStatus() != avr32::uc3::pdca::STATUS_OK)
          || (m_pdcaReceive.getStatus() != avr32::uc3::pdca::STATUS_OK))
        {
          debug.putString("wrong status");
          failed = true;
        }
      // check the transferred data
      int bufIdx = transferNum % TEST_PDCA_SPI_BUFF_NUM;

      for (int i = 0; i < TEST_PDCA_SPI_BUFF_SIZE; i++)
        {
          if (m_multTransfBufRx[bufIdx][i] != m_multTransfBufTx[bufIdx][i])
            {
              debug.putString("sent: ");
              debug.putHex(m_multTransfBufTx[bufIdx][i]);
              debug.putString(" received as ");
              debug.putHex(m_multTransfBufRx[bufIdx][i]);
              debug.putNewLine();

              failed = true;
            }
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

  // make sure everything is disabled and stopped
  spi0m.disableLocalLoopback();
  spi0m.disable();
  m_pdcaTransmit.stopTransfer();
  m_pdcaReceive.stopTransfer();
  return 1;
}

void
Test::testPdcaSpiSingleTransferRxHandler(void)
{
  OSScheduler::ISR_ledActiveOn();

  debug.putString("+");
  test.m_pdcaReceive.interruptServiceRoutine();
}

void
Test::testPdcaSpiSingleTransferTxHandler(void)
{
  OSScheduler::ISR_ledActiveOn();

  debug.putString("-");
  test.m_pdcaTransmit.interruptServiceRoutine();
}
