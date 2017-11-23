#include "udpIapDevice.h"

CUdpIapDevice iapDevice;
CCharDev& iap_device(iapDevice);

CUdpIapDevice::CUdpIapDevice()
:CCharDev(200)
{
		
}

int CUdpIapDevice::open()
{
	return 0;
}

int CUdpIapDevice::close()
{
	return 0;
}

int CUdpIapDevice::write(const uint8_t* buff, uint32_t len)
{
	return 0;
}

int CUdpIapDevice::read(uint8_t* buff, uint32_t len)
{
	return 0;
}


/**
 ** @brief  runReceiver
 ** @param  None
 ** @retval None
 **/
void CUdpIapDevice::runReceiver()
{
}

/**
 ** @brief  data_in_read_buf
 ** @param  None
 ** @retval bytes
 **/
uint32_t CUdpIapDevice::data_in_read_buf()
{
	return rxBufQue_.size();
}

/**
  * @brief  clear read buffer
	* @param  None
  * @retval None
  */
void CUdpIapDevice::clear_read_buf()
{
	rxBufQue_.clear();
}

/**
  * @brief  data in write buffer
	* @param  None
  * @retval number of bytes
  */
uint32_t CUdpIapDevice::data_in_write_buf()
{
	return txBufQue_.size();
}

/**
  * @brief  data in write buffer
	* @param  None
  * @retval number of bytes
  */
uint32_t CUdpIapDevice::freesize_in_write_buf()
{
	return 100;
}


bool CUdpIapDevice::isTransmitterIdle()
{ 
	runTransmitter();
	return (txBufQue_.size() == 0);
}
