#ifndef UDP_IAP_DEVICE_H
#define UDP_IAP_DEVICE_H
#include "chardev.h"
#include <vector>
#include <netinet/in.h>
#include <sys/socket.h>

class CUdpIapDevice
	:public CCharDev
{
	public:
		CUdpIapDevice(uint16_t native_port);	
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

		std::vector<uint8_t> rxBufQue_;
		std::vector<uint8_t> txBufQue_;

		uint16_t native_port_;
		int socket_n_;
        struct sockaddr_in clientAddr_;
		
};

extern CUdpIapDevice iapDevice;

#endif
//end of file
