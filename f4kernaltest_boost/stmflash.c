#include "stmflash.h"
#include <stdint.h>
#include <string.h>
#include <fstream>
#include "vflash.h"
using namespace std;

const uint8_t MAX_SECTOR_IDX = 12;
uint8_t sector_erase_state_tab[MAX_SECTOR_IDX] = 
{
	0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00
};

#define RESET_SECTOR_ERASE_MARK() \
	memset(sector_erase_state_tab, 0x00, MAX_SECTOR_IDX)

uint8_t is_sector_erased(uint8_t sector_idx)
{
	if(sector_idx < MAX_SECTOR_IDX)
	{
		return (sector_erase_state_tab[sector_idx] == 0xFF);
	}
	return 0;
}

void mark_sector_erased(uint8_t sector_idx)
{
	if(sector_idx < MAX_SECTOR_IDX)
		sector_erase_state_tab[sector_idx] = 0xFF;
}

void rst_addr_erase_mark(uint32_t addr)
{
	uint8_t sector_idx = sectorAddrToIdx(addr);
	sector_erase_state_tab[sector_idx] = 0x00;
}

/**
  * @brief  Check before erasing. If erased, do nothing
	* @param  sector index
	* @retval None
  */
FLASH_Status erase_sector_once(uint16_t idx)
{
	FLASH_Status ret = FLASH_COMPLETE;
	if(!is_sector_erased(idx))
	{
		printf("Erasing sector %d\r\n", idx);
		
		//TODO: call erase
		FLASH_EraseSector(sectorStdDef(idx), 0);

		mark_sector_erased(idx);
	}
	return ret;
}

int STMFLASH_write_bytes_noerase(uint32_t appxaddr,uint8_t *buf,uint16_t len)
{
	uint8_t idx = sectorAddrToIdx(appxaddr);
	mark_sector_erased(idx);
	return STMFLASH_write_bytes(appxaddr, buf, len);
}


int STMFLASH_write_bytes(uint32_t appxaddr,uint8_t *buf,uint16_t len)
{
	int i;
	int ret = 0;
	FLASH_Status flash_ret;

	if((len & 0x3) != 0 || len > 256 || appxaddr<STM32_FLASH_BASE)
	{
		printf("buffer length check failed\r\n");
		return -1;
	}	

	if((flash_ret = erase_sector_once(sectorAddrToIdx(appxaddr))) != FLASH_COMPLETE)
	{
		printf("erase sector failed, error code: %d\r\n", flash_ret);
		RESET_SECTOR_ERASE_MARK();
		ret = -1;
	}

	if(0 == ret)
	{
		
		for(i = 0; i < len; i+= 4)
		{
			if((flash_ret = FLASH_ProgramWord(appxaddr + i, *(uint32_t*)(buf + i))) != FLASH_COMPLETE)
			{ 
				printf("flash program failed, error code: %d\r\n", flash_ret);
				RESET_SECTOR_ERASE_MARK();
				ret = -1;
				break;
			}
		}
	}
	return ret;
}

uint32_t sectorFlashSize()
{
	return 0x00100000;
}

const uint16_t SECTOR_NUM = 12;

uint32_t sectorAddr(uint16_t idx)
{
	const uint32_t ADDR_FLASH_SECTOR[SECTOR_NUM + 1] = 
	{
		0x08000000,
		0x08004000,
		0x08008000,
		0x0800C000,
		0x08010000,
		0x08020000,
		0x08040000,
		0x08060000,
		0x08080000,
		0x080A0000,
		0x080C0000,
		0x080E0000,
		0x08000000 + sectorFlashSize()
	};	
	if(idx > SECTOR_NUM) return 0;
	return ADDR_FLASH_SECTOR[idx];
}

uint16_t sectorStdDef(uint16_t idx)
{
	return (idx << 3);
}

uint16_t sectorStdDefToIdx(uint16_t stddef)
{
	return (stddef >> 3);
}

uint16_t sectorAddrToIdx(uint32_t addr)
{
	for(int i = 0; i < SECTOR_NUM; i++)
	{
		if(addr < sectorAddr(i))
			return i;
	}
	return SECTOR_NUM - 1;	
}


