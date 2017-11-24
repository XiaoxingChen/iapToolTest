#include "stmflash.h"
#include <string.h>
void init_virtual_flash()
{
    memset(virtual_flash, 0xff, VIRTUAL_FLASH_SIZE);
}

uint8_t* virtual_flash_addr(uint32_t addr)
{
    return (addr - STM32_FLASH_BASE + virtual_flash);
}

int STMFLASH_write_bytes(uint32_t appxaddr,uint8_t *buf,uint16_t len)
{
	return 0;
}

int STMFLASH_write_bytes_noerase(uint32_t appxaddr,uint8_t *buf,uint16_t len)
{
	return 0;
}
void rst_addr_erase_mark(uint32_t addr)
{
	return ;
}

uint8_t virtual_flash[VIRTUAL_FLASH_SIZE];




