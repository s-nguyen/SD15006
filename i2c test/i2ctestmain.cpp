#include <stdio.h>
#include <iostream>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/i2c-dev.h> /* for I2C_SLAVE */


#define COMMAND_0 0x80  // starts measurments, relays data ready info
#define PRODUCT_ID 0x81  // product ID/revision ID, should read 0x11
#define IR_CURRENT 0x83  // sets IR current in steps of 10mA 0-200mA
#define AMBIENT_PARAMETER 0x84  // Configures ambient light measures
#define AMBIENT_RESULT_MSB 0x85  // high byte of ambient light measure
#define AMBIENT_RESULT_LSB 0x86  // low byte of ambient light measure
#define PROXIMITY_RESULT_MSB 0x87  // High byte of proximity measure
#define PROXIMITY_RESULT_LSB 0x88  // low byte of proximity measure
#define PROXIMITY_FREQ 0x89  // Proximity IR test signal freq, 0-3
#define PROXIMITY_MOD 0x8A  // proximity modulator timing


using namespace std;

int write_reg(int fh, unsigned short reg, unsigned short val) {
	uint8_t data[2]; //Needs to be uint8_t
	
	data[0] = reg;
	data[1] = val;
	
	if (write(fh, &data, 2) != 2 ) {
		cout << "error" << endl;
	} 
	
	return 1;
}

unsigned short read_reg(int fh, unsigned short reg) {
	uint8_t data;
	
	data = reg;
	
	if (write(fh, &data, 1) != 1)  {
		cout << "error" << endl;
	}
	
	if (read(fh, &data, 1) != 1) {
		cout << "error" << endl;
	}
	
	return data;
}

void initVNCL(int fh) {
	

	
	write_reg(fh, AMBIENT_PARAMETER, 0x0F);
	write_reg(fh, IR_CURRENT, 20);
	write_reg(fh, PROXIMITY_FREQ, 3);
	write_reg(fh, PROXIMITY_MOD, 0x81);
	usleep(900);
}

unsigned int readAmbient(int fh) {
	unsigned int data;
    unsigned char temp;

    temp = read_reg(fh, COMMAND_0);
    write_reg(fh, COMMAND_0, 0b10010000 /*temp | 0x10*/); 
   
	while(!(read_reg(fh, COMMAND_0)&0b01000000) /*0x20*/); 
	
    data = read_reg(fh, AMBIENT_RESULT_MSB) << 8;
    data |= read_reg(fh, AMBIENT_RESULT_LSB);

    return data;
}

unsigned int readProximity(int fh){
    unsigned int data = 0;
    unsigned char temp;

    temp = read_reg(fh, COMMAND_0); 
    write_reg(fh, COMMAND_0, 0b10001000/* temp | 0x08*/); 

    while(!(read_reg(fh, COMMAND_0)&0b00100000) /*0x20*/); 
    data = read_reg(fh, PROXIMITY_RESULT_MSB) << 8;
    data |= read_reg(fh, PROXIMITY_RESULT_LSB);

    return data;
}

int main() {
	int fh;
	uint8_t data[2];

	
	fh = open("/dev/i2c-2", O_RDWR);
	
	if (fh < 0 ) {
		cout << "error" << endl;
	}
	
	if(ioctl(fh, I2C_SLAVE, 0x13) < 0) {
		cout << "error" << endl;
	}
	
	cout << read_reg(fh, 0x81) << endl;
	
	data[0] = 0x81;

	initVNCL(fh);

	
	while (true) {
		cout << readAmbient(fh) << endl;
	}
	
	
	
	
	
	
	
	
	return 0;
}

