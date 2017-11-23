#ifndef UDP_IAP_DEVICE_H
#define UDP_IAP_DEVICE_H
#include "chardev.h"
#include <queue>

class CUdpIapDevice
	:public CCharDev
{
	public:
		CUdpIapDevice();	
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
		std::queue<uint8_t> rxBufQue_;
		std::queue<uint8_t> txBufQue_;
};

extern CUdpIapDevice iapDevice;

#endif
//end of file
