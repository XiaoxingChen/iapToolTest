#ifndef __STMFLASH_H__
#define __STMFLASH_H__ 

#include <stdint.h>

//FLASH起始地址
#define STM32_FLASH_BASE 	0x08000000 	//STM32 FLASH的起始地址

#define BOOT_PARAM_APP	 	0x5555AAAA
#define BOOT_PARAM_BL			0x2b2b6666
 
void init_virtual_flash();
uint8_t* virtual_flash_addr(uint32_t addr);
int STMFLASH_write_bytes(uint32_t appxaddr,uint8_t *buf,uint16_t len);
int STMFLASH_write_bytes_noerase(uint32_t appxaddr,uint8_t *buf,uint16_t len);
void rst_addr_erase_mark(uint32_t addr);

const uint32_t VIRTUAL_FLASH_SIZE = 0x00040000;
uint8_t virtual_falsh[VIRTUAL_FLASH_SIZE];

#endif

















