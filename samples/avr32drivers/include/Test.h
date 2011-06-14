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

  int
  testAlloca(int x);

  // test for a single buffer transfer on SPI using PDCA
  int
  testPdcaSpiSingleTransfer(void);

  // test for multiple buffer transfer on SPI using PDCA
  int
  testPdcaSpiMultipleTransfer(void);

  static __attribute__((__interrupt__)) void testPdcaSpiSingleTransferRxHandler( void);
  static __attribute__((__interrupt__)) void testPdcaSpiSingleTransferTxHandler( void);

private:
  // members
  OSStack_t m_stack[(OSThread::STACK_MINIMAL_SIZE + 400) / sizeof(OSStack_t)];

  // members for Pdca Test
  static avr32::uc3::PdcaReceive m_pdcaReceive;
  static avr32::uc3::PdcaTransmit m_pdcaTransmit;


  // PDCA-SPI Transfer test
  uint8_t *m_singleTransfBufRx;
  uint8_t *m_singleTransfBufTx;
  uint8_t **m_multTransfBufRx;
  uint8_t **m_multTransfBufTx;

};

#endif /* TEST_H_ */
