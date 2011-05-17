/*
 *      Copyright (C) 2008-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef HAL_EEPROM_INLINES_H_
#define HAL_EEPROM_INLINES_H_

inline unsigned char EEPROM::getUninitialisedValue(void)
  {
    return 0xFF;
  }

inline void EEPROM::busyWait(void)
  {
    eeprom_busy_wait();
  }

inline unsigned char EEPROM::implReadByte(unsigned short offset)
  {
    return eeprom_read_byte((const uint8_t* )offset) ;
  }

inline unsigned short EEPROM::implReadWord(unsigned short offset)
  {
    return eeprom_read_word((const uint16_t* )offset) ;
  }

inline void EEPROM::implReadBlock(unsigned short offset, void* pointer_ram,
    size_t n)
  {
    return eeprom_read_block(pointer_ram, (const void*)offset, n);
  }

inline void EEPROM::implWriteByte(unsigned short offset, unsigned char b)
  {
    eeprom_write_byte((uint8_t* )offset, b);
  }

inline void EEPROM::implWriteWord(unsigned short offset, unsigned short w)
  {
    eeprom_write_word((uint16_t* )offset, w);
  }

inline void EEPROM::implWriteBlock(unsigned short offset, void* pointer_ram,
    size_t n)
  {
    eeprom_write_block(pointer_ram, (void*)offset, n);
  }

#endif /* HAL_EEPROM_INLINES_H_ */
