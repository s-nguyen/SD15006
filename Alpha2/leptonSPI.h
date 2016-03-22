#ifndef LEPTONSPI
#define LEPTONSPI

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <limits.h>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

#define PACKET_SIZE (164)
using namespace std;

class leptonSPI {
	
	static const char *device;
	static uint8_t mode;
	static uint8_t bits;
	static uint32_t speed;
	static uint16_t delay;
	
	struct spi_ioc_transfer tr;
	
public:
	unsigned int lepton_image[80][80];
	uint8_t lepton_frame_packet[PACKET_SIZE];


	int fd;

	
	bool initCamera();
	int getFrame();

	leptonSPI();
	~leptonSPI();

};

#endif