#include "stmflash.h"
#include "iap.h" 
#include "st_bootloader_protocol.h"
#include "printf.h"
#include <stdlib.h>
#include <fstream>
using namespace std;

iapfun jump2app; 

void iap_load_app(uint32_t appxaddr)
{
	printf("load to app");
    
	exit(0);
}		 

/**
  * @brief  run iap logic
	* @param  None
	* @retval None
  */
void iap_run()
{
	static uint8_t iap_tick = 0;
	static uint8_t bl_cmd = 0;
	switch(iap_tick)
	{
		case 0:
		{
			uint8_t sum_check = 0;
			
			if(iap_device.data_in_read_buf() < 2)
			{
					if(iap_device.is_dataflow_break())
					{
						printf("timeout and break %s(%d), ",__FUNCTION__, __LINE__);
						printf("give up byte: 0x%X in rxbuf\r\n", iapdev_read_byte());
						iap_device.clear_read_buf();
					}
					break;
			}
					
			bl_cmd = iapdev_read_byte();
			sum_check = iapdev_read_byte();
			
			/* XOR != 0 */
			if((bl_cmd ^ sum_check) != 0xFF)
			{
				printf("get %d bytes\r\n", iap_device.data_in_read_buf());
				printf("get cmd: 0x%X, sum_check 0x%X\r\n",bl_cmd, sum_check);
				printf("bl_cmd sumcheck faild\r\n");
				iapdev_write_byte(NACK);
				break;
			}
			iap_tick++;
			break;
		}
		
		case 1:
		{
			bl_err_t ret_code = bl_excute_cmd(bl_cmd);
			if(ret_code == BL_OK || ret_code == BL_ERR)
			{
				iap_tick = 0;
			}
			//else: ret_code == BL_YIELD
			//excute until BL_OK or BL_ERR
			break;
		}
		default:
			break;
	}
}










