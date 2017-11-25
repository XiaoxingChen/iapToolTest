#include "stmflash.h"
#include <string.h>
#include <fstream>
using namespace std;

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


const uint32_t ADDR_FLASH_SECTOR[12] = 
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
	0x080E0000
};

#define FLASH_Sector_0     ((uint16_t)0x0000) /*!< Sector Number 0 */
#define FLASH_Sector_1     ((uint16_t)0x0008) /*!< Sector Number 1 */
#define FLASH_Sector_2     ((uint16_t)0x0010) /*!< Sector Number 2 */
#define FLASH_Sector_3     ((uint16_t)0x0018) /*!< Sector Number 3 */
#define FLASH_Sector_4     ((uint16_t)0x0020) /*!< Sector Number 4 */
#define FLASH_Sector_5     ((uint16_t)0x0028) /*!< Sector Number 5 */
#define FLASH_Sector_6     ((uint16_t)0x0030) /*!< Sector Number 6 */
#define FLASH_Sector_7     ((uint16_t)0x0038) /*!< Sector Number 7 */
#define FLASH_Sector_8     ((uint16_t)0x0040) /*!< Sector Number 8 */
#define FLASH_Sector_9     ((uint16_t)0x0048) /*!< Sector Number 9 */
#define FLASH_Sector_10    ((uint16_t)0x0050) /*!< Sector Number 10 */
#define FLASH_Sector_11    ((uint16_t)0x0058) /*!< Sector Number 11 */

uint16_t STMFLASH_GetFlashSector(uint32_t addr)
{
	if(addr<ADDR_FLASH_SECTOR[1])return FLASH_Sector_0;
	else if(addr<ADDR_FLASH_SECTOR[2])return FLASH_Sector_1;
	else if(addr<ADDR_FLASH_SECTOR[3])return FLASH_Sector_2;
	else if(addr<ADDR_FLASH_SECTOR[4])return FLASH_Sector_3;
	else if(addr<ADDR_FLASH_SECTOR[5])return FLASH_Sector_4;
	else if(addr<ADDR_FLASH_SECTOR[6])return FLASH_Sector_5;
	else if(addr<ADDR_FLASH_SECTOR[7])return FLASH_Sector_6;
	else if(addr<ADDR_FLASH_SECTOR[8])return FLASH_Sector_7;
	else if(addr<ADDR_FLASH_SECTOR[9])return FLASH_Sector_8;
	else if(addr<ADDR_FLASH_SECTOR[10])return FLASH_Sector_9;
	else if(addr<ADDR_FLASH_SECTOR[11])return FLASH_Sector_10; 
	return FLASH_Sector_11;	
}

uint8_t FLASH_Sector_to_index(uint16_t FLASH_Sector)
{
	return (FLASH_Sector>>3);
}

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
	uint8_t sector_idx = FLASH_Sector_to_index(STMFLASH_GetFlashSector(addr));
	sector_erase_state_tab[sector_idx] = 0x00;
}

/**
  * @brief  Check before erasing. If erased, do nothing
	* @param  FLASH_Sector
	* @retval None
  */
FLASH_Status erase_sector_once(uint32_t FLASH_Sector)
{
	FLASH_Status ret = FLASH_COMPLETE;
	uint8_t idx = FLASH_Sector_to_index(FLASH_Sector);
	if(!is_sector_erased(idx))
	{
		printf("Erasing sector %d\r\n", FLASH_Sector_to_index(FLASH_Sector));
		//ret = FLASH_EraseSector(FLASH_Sector, VoltageRange_3);
		for(int i = ADDR_FLASH_SECTOR[idx]; i < ADDR_FLASH_SECTOR[idx + 1]; i++)
		{
			*virtual_flash_addr(i) = 0xFF;	
		}
		mark_sector_erased(idx);
	}
	return ret;
}

int STMFLASH_write_bytes_noerase(uint32_t appxaddr,uint8_t *buf,uint16_t len)
{
	uint8_t idx = FLASH_Sector_to_index(STMFLASH_GetFlashSector(appxaddr));
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

	if((flash_ret = erase_sector_once(STMFLASH_GetFlashSector(appxaddr))) != FLASH_COMPLETE)
	{
		printf("erase sector failed, error code: %d\r\n", flash_ret);
		RESET_SECTOR_ERASE_MARK();
		ret = -1;
	}

	if(0 == ret)
	{
		for(int i = 0; i < len; i++)
			virtual_flash_addr(appxaddr + i)[0] &= buf[i];
	}
	return ret;
}



