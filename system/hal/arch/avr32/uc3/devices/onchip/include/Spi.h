/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef AVR32_UC3_SPI_H_
#define AVR32_UC3_SPI_H_

namespace avr32
{
  namespace uc3
  {
    class Spi
    {
    public:
      enum ModuleId_t
      {
        zero = 0, one = 1
      };

      Spi(ModuleId_t id);

    private:
      void* m_channelAddress;
    };
  }
}

#endif /* AVR32_UC3_SPI_H_ */
