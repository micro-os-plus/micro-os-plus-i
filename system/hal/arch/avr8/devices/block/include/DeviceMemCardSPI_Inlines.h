/*
 *	Copyright (C) 2007-2008 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

inline void DeviceMemCardSPI::implSpiInitLowSpeed(void)
  {
    // MOSI, SCK, SS out
    OS_CONFIG_DEVICEMEMCARDSPI_PORT_INIT |= _BV(OS_CONFIG_DEVICEMEMCARDSPI_MOSI_BIT)
        | _BV(OS_CONFIG_DEVICEMEMCARDSPI_SCK_BIT)
        | _BV(OS_CONFIG_DEVICEMEMCARDSPI_SS_BIT);
    //  MISO in
    OS_CONFIG_DEVICEMEMCARDSPI_PORT_INIT
        &= ~_BV(OS_CONFIG_DEVICEMEMCARDSPI_MISO_BIT);

    // Enable SPI Master
    SPCR = _BV(SPE) | _BV(MSTR);

    // Set low speed (Fosc/64, 125 KHz at 8 MHz)
    SPSR &= ~_BV(SPI2X);
    SPCR |= _BV(SPR1);
    SPCR &= ~_BV(SPR0);
  }

inline void DeviceMemCardSPI::implSpiInit(void)
  {
    // set high clock rate (Fosc/2, 4 MHz at 8 MHz)
    SPSR |= _BV(SPI2X);
    SPCR &= ~_BV(SPR1);
    SPCR &= ~_BV(SPR0);
  }

inline unsigned char DeviceMemCardSPI::implSpiReadWrite(unsigned char c)
  {
    SPDR = c;
    while ( !(SPSR & _BV(SPIF) ))
      ; // no WDTreset() here to catch SPI lock

    return SPDR;
  }
