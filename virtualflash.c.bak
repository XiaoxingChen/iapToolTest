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


uint8_t virtual_flash[VIRTUAL_FLASH_SIZE];




