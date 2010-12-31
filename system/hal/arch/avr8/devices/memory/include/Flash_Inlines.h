/*
 *      Copyright (C) 2008 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef HAL_FLASH_INLINES_H_
#define HAL_FLASH_INLINES_H_

#include <avr/boot.h>

inline unsigned char Flash::getUninitializedValue(void)
  {
    return 0xFF;
  }

inline unsigned char Flash::implReadByte(unsigned short offset)
  {
    return pgm_read_byte((const uint8_t *)offset) ;
  }

inline void Flash::implPageErase(FlashAddress_t addr)
  {
    boot_page_erase(addr);
  }

inline void Flash::implPageEraseBusyWait(void)
  {
    boot_spm_busy_wait();
  }

inline void Flash::implPageFillWord(FlashAddress_t addr, unsigned short w)
  {
    boot_page_fill(addr, w);
  }

inline void Flash::implPageWrite(FlashAddress_t addr)
  {
    boot_page_write(addr);
  }

inline void Flash::implPageWriteBusyWait(void)
  {
    boot_spm_busy_wait();
  }

inline void Flash::implEnableRead(void)
  {
    boot_rww_enable();
  }

#endif /*HAL_FLASH_INLINES_H_*/
