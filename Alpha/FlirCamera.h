#ifndef FLIRCAMERA
#define FLIRCAMERA

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <vector>
#include <iostream>

#if HAVE_LEPTON
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#endif

using namespace std;

class FlirCamera {

	vector<unsigned char> result;
	vector<unsigned short> rawData;

#if HAVE_LEPTON
	static const char *device;
	static unsigned char mode, bits;
	static unsigned int speed;
	static unsigned short delay;
	static vector<unsigned char> tx;

	int fd;
	struct spi_ioc_transfer _tr;
#endif
	
	bool initCamera();
	int getPacket(int iRow, unsigned char *packetData);
	
public:
	enum {
		FrameWidth = 80,
        FrameHeight = 60,
        RowPacketWords = FrameWidth + 2,
        RowPacketBytes = 2*RowPacketWords,
        FrameWords = FrameWidth*FrameHeight
	};
	
	FlirCamera();
	~FlirCamera();
	
	void run();
	
		
};

#endif