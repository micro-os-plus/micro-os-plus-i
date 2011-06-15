/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef TEST_H_
#define TEST_H_

#include "portable/kernel/include/uOS.h"
#include "hal/arch/avr32/uc3/devices/onchip/include/Spi.h"
#include "hal/arch/avr32/uc3/devices/onchip/include/Pdca.h"

class Test;
extern Test test;

class Test : public OSThread
{
public:
  // active object constructor
  Test();

  // actual thread main code
  virtual void
  threadMain(void);

  void
  testSpi(avr32::uc3::spi::ModuleId_t moduleId,
      avr32::uc3::spi::BitsPerTransfer_t bitsPerTransfer);

  // test for a single buffer transfer on SPI using PDCA
  int
  testPdcaSpiSingleTransfer(void);

  // test for multiple buffer transfer on SPI using PDCA
  int
  testPdcaSpiMultipleTransfer(void);

  static void __attribute__((__interrupt__))
  testPdcaSpiSingleTransferRxHandler(void);
  static void __attribute__((__interrupt__))
  testPdcaSpiSingleTransferTxHandler(void);

private:
  // members
  OSStack_t m_stack[(OSThread::STACK_MINIMAL_SIZE + 800) / sizeof(OSStack_t)];

  // members for Pdca Test
  avr32::uc3::PdcaReceive m_pdcaReceive;
  avr32::uc3::PdcaTransmit m_pdcaTransmit;

  // PDCA-SPI Transfer test
  // PDCA-SPI Single Transfer test globals
#define TEST_PDCA_SPI_BUFF_SIZE         10
  uint8_t m_singleTransfBufRx[TEST_PDCA_SPI_BUFF_SIZE];
  uint8_t m_singleTransfBufTx[TEST_PDCA_SPI_BUFF_SIZE];

  // PDCA-SPI Multiple Transfer test globals
#define TEST_PDCA_SPI_BUFF_NUM         10
#define TEST_PDCA_SPI_BUFF_SIZE2       128
  volatile uint8_t m_multTransfBufRx[TEST_PDCA_SPI_BUFF_NUM][TEST_PDCA_SPI_BUFF_SIZE2];
  volatile uint8_t m_multTransfBufTx[TEST_PDCA_SPI_BUFF_NUM][TEST_PDCA_SPI_BUFF_SIZE2];

};

#endif /* TEST_H_ */
