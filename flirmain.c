#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

#define ARRAY_SIZE(array) sizeof(array)/sizeof(array[0])

static const char *device = "/dev/spidev1.0";
static uint8_t mode = 3;
static uint8_t bits = 8;
static uint16_t delay;
static uint32_t speed = 10000000;

void set_up_SPI(int fd);

int main () {
	
	int fd, i;
	char rd_buf[10] = { 0 };
	
	fd = open(device, O_RDWR); //opens device
	if (fd <= 0) {
		printf("Device not found\n");
		return -1;
	}
	
	set_up_SPI(fd);
	
	//Shows empty array
	for (i=0;i<ARRAY_SIZE(rd_buf);i++) {
		printf("0x%02X ", rd_buf[i]);
		if (i%2)
			printf("\n");
	}
	
	//Actually Reads SPI
	if (read(fd, rd_buf, ARRAY_SIZE(rd_buf)) != ARRAY_SIZE(rd_buf)) {
		printf("Read Error \n");
	}
	else { //Prints out data into console
		for (i=0;i<ARRAY_SIZE(rd_buf);i++) {
		printf("0x%02X ", rd_buf[i]);
		if (i%2)
			printf("\n");
		}
	}
	
	
	close(fd);
	
	return 0;
}

void set_up_SPI(int fd) {
	
	int ret = 0;
	uint8_t b = 0;
	uint32_t s = 0;
	uint8_t m = 0;
	
	/*
	 * spi mode
	 */
	ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);
	if (ret == -1) {
		printf("can't set spi mode\n");
	}
		


	/*
	 * bits per word
	 */
	ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
	if (ret == -1)
		printf("can't set bits per word");


	/*
	 * max speed hz
	 */
	ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
	if (ret == -1)
		printf("can't set max speed hz");

	//Reads Current SPI Value
	ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &b);
	ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &s);
	ioctl(fd, SPI_IOC_RD_MODE, &m);
	
	//Prints current values
	printf("spi mode: %u\n", m);
	printf("bits per word: %u\n", b);
	printf("max speed: %u Hz (%d MHz)\n", s, s/1000000);
}