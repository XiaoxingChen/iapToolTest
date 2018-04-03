#include <iostream>
#include <fstream>
#include <stdint.h>
#include "stmflash.h"

using namespace std;
FLASH_Status FLASH_ProgramWord(uint32_t Address, uint32_t Data)
{
    fstream fout("virtual.bin", std::fstream::out| std::fstream::in | std::fstream::binary| std::fstream::ate);
    const uint32_t BASE_OFF = 0x08000000;
    uint32_t fileaddr = Address - BASE_OFF;
    if(Address < BASE_OFF)
    {
        return FLASH_ERROR_OPERATION;
    }
    
    if (0 == fout.tellp())
    {
		printf("bin imagine open error, erase all flash\r\n");
		// memset(virtual_flash, 0xff, VIRTUAL_FLASH_SIZE);
        // return;
    }    
    
    uint32_t prevVal;
    
    fout.seekg(fileaddr);
    fout.read((char*)&prevVal, sizeof(uint32_t));
    
    prevVal |= Data;
    
    fout.seekp(fileaddr);
    fout.write((const char*)&prevVal, 4);
    fout.close();
    return FLASH_COMPLETE;
}

FLASH_Status FLASH_EraseSector(uint32_t FLASH_Sector, uint8_t VoltageRange)
{
    fstream fout("virtual.bin", std::fstream::out| std::fstream::in | std::fstream::binary| std::fstream::ate);
    const uint32_t BASE_OFF = 0x08000000;
    uint8_t idx = sectorStdDefToIdx(FLASH_Sector);
    if(idx >= SECTOR_NUM)
        return FLASH_ERROR_OPERATION;
        
    for(int i = sectorAddr(idx); i < sectorAddr(idx + 1); i++)
    {
        fout.seekp(i - BASE_OFF);
        fout.write("\xff", 1);
    }
    
    fout.close();
    return FLASH_COMPLETE;
}

uint16_t vflashMemcpy(void* dstAddr, uint32_t srcAddr, uint16_t len)
{
    fstream fin("virtual.bin", std::fstream::in | std::fstream::binary);
    const uint32_t BASE_OFF = 0x08000000;
    fin.seekg(srcAddr - BASE_OFF);
    for(int i = 0; i < len; i++)
    {
        fin.read((char*)dstAddr, len);
        }
}