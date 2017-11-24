#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include "udpIapDevice.h"
#include <unistd.h>
#include "iap.h"
#include "stmflash.h"

using namespace std;
extern const uint8_t FIRMWARE_VERSION = 0x11;
int main()
{
	cout <<"IAP simulator start!" << endl;
    init_virtual_flash();
	iapDevice.open();

	printf("Firmware virsion: V%d.%d\r\n", FIRMWARE_VERSION>>4, FIRMWARE_VERSION&0xF);
	while(1)
	{
		iapDevice.run();
		iap_run();
		usleep(1000);
	}
	return 0;
}
