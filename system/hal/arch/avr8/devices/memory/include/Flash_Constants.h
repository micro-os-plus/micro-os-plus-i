/*
 *      Copyright (C) 2008-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef HAL_FLASH_CONSTANTS_H_
#define HAL_FLASH_CONSTANTS_H_

public:
#if defined(__AVR_ATmega644P__)

const static unsigned short BOOTLOADER_START = 0xE000;
const static unsigned short BLOCK_SIZE = 256;
const static unsigned short BLOCK_WRITE_MILLIS = 5;

#elif defined(__AVR_ATmega324P__)

const static unsigned short BOOTLOADER_START = 0x7000;
const static unsigned short BLOCK_SIZE = 128;
const static unsigned short BLOCK_WRITE_MILLIS = 5;

#endif

#endif /* HAL_FLASH_CONSTANTS_H_ */
