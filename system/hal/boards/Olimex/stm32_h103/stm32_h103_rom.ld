/* Linker script for lm3s6730
 *
 * Version:Sourcery G++ 4.2-62
 * BugURL:https://support.codesourcery.com/GNUToolchain/
 *
 *  Copyright 2007 CodeSourcery.
 *
 * The authors hereby grant permission to use, copy, modify, distribute,
 * and license this software and its documentation for any purpose, provided
 * that existing copyright notices are retained in all copies and that this
 * notice is included verbatim in any distributions. No written agreement,
 * license, or royalty fee is required for any of the authorized uses.
 * Modifications to this software may be copyrighted by their authors
 * and need not follow the licensing terms described here, provided that
 * the new terms are clearly indicated on the first page of each file where
 * they apply. */
/*
OUTPUT_FORMAT ("elf32-littlearm", "elf32-bigarm", "elf32-littlearm")
ENTRY(_start)
SEARCH_DIR(.)*/
/* GROUP(-lgcc -lc -lcs3 -lcs3unhosted -lcs3stellaris) here */

MEMORY
{
  text 	(rx) 	: ORIGIN = 0x00000000, LENGTH = 128K
  data 	(rwx) 	: ORIGIN = 0x20000000, LENGTH = 20K
}

INCLUDE "hal/arch/arm_cortex_m3/stm32f10x/lib/stm32f103xB_rom.ld"

/*========== end of file ==========*/
