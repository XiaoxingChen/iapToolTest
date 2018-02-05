#include "boostUdpIapDevice.h"
#include <stdio.h>

CBoostUdpIapDevice iapDevice(5000);
CCharDev& iap_device(iapDevice);

namespace 
{
    const uint16_t BUFF_LEN = 1024;
    uint8_t transmit_buffer[BUFF_LEN];
    uint8_t receive_buffer[BUFF_LEN];
};


CBoostUdpIapDevice::CBoostUdpIapDevice(uint16_t native_port)	
:CCharDev(200),local_port_(5000), udp_socket_(io_service_)
{
		
}

int CBoostUdpIapDevice::open()
{
    boost::asio::ip::udp::endpoint local_add(boost::asio::ip::address::from_string("127.0.0.1"), local_port_);
    
    udp_socket_.open(local_add.protocol());
    try
    {
        udp_socket_.bind(local_add);
    }
    catch (boost::system::system_error &e)
    {
        std::cout << "process failed:" << e.what() << std::endl;
    }

    asyncRecv();

	return 0;
}

int CBoostUdpIapDevice::close()
{
	return 0;
}

int CBoostUdpIapDevice::write(const uint8_t* buff, uint32_t len)
{
	return udp_socket_.send_to(boost::asio::buffer(buff, len), remote_point_);
}

int CBoostUdpIapDevice::read(uint8_t* buff, uint32_t len)
{
	if(len > rxBufQue_.size())
		len = rxBufQue_.size();

	for(uint32_t i = 0; i < len; i++)
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
void CBoostUdpIapDevice::runReceiver()
{
	/*const uint16_t BUFF_LEN = 300;
	uint8_t buff[BUFF_LEN];
    int n;
    
    n = udp_socket_.receive_from(boost::asio::buffer(buff, BUFF_LEN), remote_point_);

    printf("get %d bytes\r\n", n);

	for(int i = 0; i < n; i++)
	{
		rxBufQue_.push_back(buff[i]);
	}

	update_data_break_flag();*/
    io_service_.poll_one();
}


/**
 ** @brief  runTransmitter
 ** @param  None
 ** @retval None
 **/
void CBoostUdpIapDevice::runTransmitter()
{
}

/**
 ** @brief  data_in_read_buf
 ** @param  None
 ** @retval bytes
 **/
uint32_t CBoostUdpIapDevice::data_in_read_buf()
{
	return rxBufQue_.size();
}

/**
  * @brief  clear read buffer
	* @param  None
  * @retval None
  */
void CBoostUdpIapDevice::clear_read_buf()
{
	rxBufQue_.clear();
}

/**
  * @brief  data in write buffer
	* @param  None
  * @retval number of bytes
  */
uint32_t CBoostUdpIapDevice::data_in_write_buf()
{
	return txBufQue_.size();
}

/**
  * @brief  data in write buffer
	* @param  None
  * @retval number of bytes
  */
uint32_t CBoostUdpIapDevice::freesize_in_write_buf()
{
	return 100;
}


bool CBoostUdpIapDevice::isTransmitterIdle()
{ 
	runTransmitter();
	return (txBufQue_.size() == 0);
}

void CBoostUdpIapDevice::asyncRecv()
{
    udp_socket_.async_receive_from(
        boost::asio::buffer(receive_buffer, BUFF_LEN), remote_point_,
        boost::bind(&CBoostUdpIapDevice::handleUdpRecv, this,
        boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred));
}

void CBoostUdpIapDevice::handleUdpRecv(const boost::system::error_code& error, std::size_t n)
{
    if (!error || error == boost::asio::error::message_size)
    {
        for (int i = 0; i < n; i++)
        {
            rxBufQue_.push_back(receive_buffer[i]);
        }

        update_data_break_flag();

        //接收下一次的信息。  
        asyncRecv();
    }
}

