#ifndef __STMFLASH_H__
#define __STMFLASH_H__ 

#define STM32_FLASH_BASE 	0x08000000 

#define BOOT_PARAM_APP	 	0x5555AAAA
#define BOOT_PARAM_BL		0x2b2b6666

#include <stdint.h>

typedef enum
{ 
  FLASH_BUSY = 1,
  FLASH_ERROR_PGS,
  FLASH_ERROR_PGP,
  FLASH_ERROR_PGA,
  FLASH_ERROR_WRP,
  FLASH_ERROR_PROGRAM,
  FLASH_ERROR_OPERATION,
  FLASH_COMPLETE
}FLASH_Status;

int STMFLASH_write_bytes(uint32_t appxaddr,uint8_t *buf,uint16_t len);
int STMFLASH_write_bytes_noerase(uint32_t appxaddr,uint8_t *buf,uint16_t len);
void rst_addr_erase_mark(uint32_t addr);

extern const uint16_t SECTOR_NUM;
uint32_t sectorFlashSize();

uint32_t sectorAddr(uint16_t idx);
uint16_t sectorStdDef(uint16_t idx);

uint16_t sectorStdDefToIdx(uint16_t stddef);
uint16_t sectorAddrToIdx(uint32_t addr);

#endif

















