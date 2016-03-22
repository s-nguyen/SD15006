#include "FlirCamera.h"

#include <string>

#if HAVE_LEPTON
//Change if it does not work
const char *FlirCamera::device = "/dev/spidev1.0"; 
unsigned char FlirCamera::mode = 3;
unsigned char FlirCamera::bits = 8;
unsigned int FlirCamera::speed = 10000000;
unsigned short FlirCamera::delay = 0;
vector<unsigned char > FlirCamera::tx(FlirCamera::RowPacketBytes, 0);

FlirCamera::FlirCamera() {
	
}

FlirCamera::~FlirCamera() {
	
}

bool FlirCamera::initCamera() {
	fd = open(device, O_RDWR);
	if (fd < 0)
		cout << "Can't open device" << endl;
	else if (-1 == ioctl(fd, SPI_IOC_WR_MODE, &mode))
        cout << "Can't set SPI mode" << endl;
    else if (-1 == ioctl(fd, SPI_IOC_RD_MODE, &mode))
        cout << "Can't get SPI mode" << endl;
    else if (-1 == ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits))
        cout << "Can't set bits per word" << endl;
    else if (-1 == ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits))
        cout << "Can't get bits per word" << endl;
    else if (-1 == ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed))
        cout << "Can't set max speed" << endl;
    else if (-1 == ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed))
        cout << "Can't get max speed" << endl;
    else
        return true;
    return false;
}
#else
static int counter = 0;
#endif


int FlirCamera::getPacket(int iRow, unsigned char *packetData) {
#if HAVE_LEPTON
	_tr.rx_buf = (unsigned long) packetData;
	return ioctl(fd, SPI_IOC_MESSAGE(1), &_tr);
	cout << "It shouldn't be here" << endl;
	(void)iRow;
#else
	packetData[0] = 0;
	packetData[1] = iRow;
	for (int i = 4; i < RowPacketBytes; i += 2)
        *(short *)(packetData+i) = ((iRow+counter) % 20) + ((i/2-2) % 26);
    return 1;
#endif
}

void FlirCamera::run() {
#if HAVE_LEPTON
	if (!initCamera()) return;
	
	usleep(250000);
	
	_tr.tx_buf = (unsigned long) &tx[0];
	_tr.len = RowPacketBytes;
	_tr.delay_usecs = delay;
	_tr.speed_hz = speed;
	_tr.bits_per_word = bits;
#endif

	int resets = 0;
	int errors = 0;
	while (true) {
		int iRow;
		for (iRow = 0; iRow < FrameHeight; ) {
			unsigned char *packet = &result[iRow*RowPacketBytes];
			
			if (getPacket(iRow, packet) < 1) {
				cout << "Error transferring SPI packet" << endl;
				return;
			}
			
			int packetNumber;
			if ((packet[0] & 0xf) == 0xf) //Discard packet?
				packetNumber = -1;
			else
				packetNumber = packet[1];
			
			if (packetNumber == -1) {
				usleep(1000); //Wait for a real packet
				if (++errors > 300) break;
				continue;
			}
			
			if (packetNumber != iRow) {
				usleep(1000); //Wrong Row
				break;
			}
			
			++iRow;
		}
		
		if (iRow < FrameHeight) {
			if (++resets >= 750) {
				cout << "Packet reset counter hit 750" << endl;
				resets = 0;
				usleep(750000);
			}
			continue;
		}
		
		resets = 0; errors = 0;
		
		uint16_t minValue = 65535;
		uint16_t maxValue = 0;
		unsigned char *in = &result[0];
		unsigned short *out = &rawData[0];
		for (int iRow = 0; iRow < FrameHeight; ++iRow) {
			in += 4;
			for (int iCol = 0; iCol < FrameWidth; ++iCol) {
				unsigned short value = in[0];
				value <<= 8;
				value |= in[1];
				in += 2;
				if (value > maxValue) maxValue = value;
				if (value < minValue) minValue = value;
				*(out++) = value;
			}
		}
		// Do something with data
		//Test print function
		for(unsigned int i = 0; i < rawData.size(); i++) {
			cout << rawData[i] << ' ';
		}
		
#if !HAVE_LEPTON
		//need to slow things down if no ioctl call!
		usleep(50000); 
		counter = (counter + 1)%520;
#endif
	}
}