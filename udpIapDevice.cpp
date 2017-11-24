#include "udpIapDevice.h"
#include <sys/types.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

CUdpIapDevice iapDevice(5000);
CCharDev& iap_device(iapDevice);

CUdpIapDevice::CUdpIapDevice(uint16_t native_port)	
:CCharDev(200),
native_port_(native_port)
{
		
}

int CUdpIapDevice::open()
{
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(native_port_);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

	int sock;

	if ( (sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket");
        exit(1);
    }
    if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind");
        exit(1);
    }

	struct timeval timeOut;
	timeOut.tv_sec = 0;                 //设置5s超时
	timeOut.tv_usec = 1000;
	if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeOut, sizeof(timeOut)) < 0)
	{
	    printf("time out setting failed\n");
	}

	printf("Open socket success\r\n");
	socket_n_ = sock;
	return 0;
}

int CUdpIapDevice::close()
{
	return 0;
}

int CUdpIapDevice::write(const uint8_t* buff, uint32_t len)
{
	return sendto(socket_n_, buff, len, 0, (struct sockaddr *)&clientAddr_, sizeof(clientAddr_));
}

int CUdpIapDevice::read(uint8_t* buff, uint32_t len)
{
	if(len > rxBufQue_.size())
		len = rxBufQue_.size();

	for(int i = 0; i < len; i++)
	{
		buff[i] = rxBufQue_[0];
		rxBufQue_.erase(rxBufQue_.begin());
	}
	return 0;
}


/**
 ** @brief  runReceiver
 ** @param  None
 ** @retval None
 **/
void CUdpIapDevice::runReceiver()
{
	const uint16_t BUFF_LEN = 300;
	uint8_t buff[BUFF_LEN];
    int n;
    int len = sizeof(clientAddr_);
	n = recvfrom(socket_n_, buff, BUFF_LEN, 0, (struct sockaddr*)&clientAddr_, (socklen_t*)&len);

	for(int i = 0; i < n; i++)
	{
		rxBufQue_.push_back(buff[i]);
	}

	update_data_break_flag();
}


/**
 ** @brief  runTransmitter
 ** @param  None
 ** @retval None
 **/
void CUdpIapDevice::runTransmitter()
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
