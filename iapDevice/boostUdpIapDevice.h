#ifndef _BOOST_UDP_IAP_DEVICE_H
#define _BOOST_UDP_IAP_DEVICE_H
#include "chardev.h"
#include <vector>
#include "boost/thread.hpp"
#include "boost/asio.hpp"

class CBoostUdpIapDevice
	:public CCharDev
{
	public:
		CBoostUdpIapDevice(uint16_t native_port);	
		virtual int open();
		virtual int close();
	
		virtual int write(const uint8_t*, uint32_t);
		virtual int read(uint8_t*, uint32_t);
	
		virtual uint32_t data_in_write_buf();
		virtual uint32_t freesize_in_write_buf();
		virtual uint32_t data_in_read_buf();
		virtual void clear_read_buf();
		virtual bool isTransmitterIdle();
	
		virtual void runTransmitter();
		virtual void runReceiver();

        
	
	private:

        void asyncRecv();

        void handleUdpRecv(const boost::system::error_code& error,
            std::size_t bytes);

		std::vector<uint8_t> rxBufQue_;
		std::vector<uint8_t> txBufQue_;

        boost::asio::io_service io_service_;
        boost::asio::ip::udp::socket udp_socket_;
        boost::asio::ip::udp::endpoint local_add_;

        uint16_t local_port_;
        boost::asio::ip::udp::endpoint remote_point_;
		
};

extern CBoostUdpIapDevice iapDevice;

#endif
//end of file
