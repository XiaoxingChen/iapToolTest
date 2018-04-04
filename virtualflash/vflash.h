#ifndef VFLASH_H
#define VFLASH_H

#include "stmflash.h"
FLASH_Status FLASH_EraseSector(uint32_t FLASH_Sector, uint8_t VoltageRange);
FLASH_Status FLASH_ProgramWord(uint32_t Address, uint32_t Data);
uint16_t vflashMemcpy(void* dstAddr, uint32_t srcAddr, uint16_t len);
void vflashInit();

#endif
