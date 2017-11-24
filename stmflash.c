#include "stmflash.h"
#include <string.h>
#include <fstream>
using namespace std;
void init_virtual_flash()
{
    ifstream fin("virtual.bin", std::fstream::in | std::fstream::binary);
    if (!fin)
    {
		printf("bin imagine open error, erase all flash\r\n");
		memset(virtual_flash, 0xff, VIRTUAL_FLASH_SIZE);
        return;
    }

	int length;
    fin.seekg(0, std::ios::end);
    length = fin.tellg();
    fin.seekg(0, std::ios::beg);

    fin.read((char*)virtual_flash, length);
    fin.close();
//	memset(virtual_flash, 0xff, VIRTUAL_FLASH_SIZE);
}

uint8_t* virtual_flash_addr(uint32_t addr)
{
    return (addr - STM32_FLASH_BASE + virtual_flash);
}

int STMFLASH_write_bytes(uint32_t appxaddr,uint8_t *buf,uint16_t len)
{
	for(int i = 0; i < len; i++)
	{
		virtual_flash_addr(appxaddr + i)[0] &= buf[i];
	}
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




