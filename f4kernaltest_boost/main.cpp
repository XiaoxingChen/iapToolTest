#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include "boostUdpIapDevice.h"
#include "iap.h"
#include "stmflash.h"
#include <thread>
#include <chrono>
#include "vflash.h"


using namespace std;
extern const uint8_t FIRMWARE_VERSION = 0x14;
int main()
{
	cout <<"IAP simulator start!" << endl;
    
    vflashInit();
    iapDevice.open();

    printf("Firmware virsion: V%d.%d\r\n", FIRMWARE_VERSION >> 4, FIRMWARE_VERSION & 0xF);
	while(1)
	{
		 iapDevice.run();
		 iap_run();
        //this_thread::sleep_for(chrono::seconds(1));
	}
	return 0;
}
