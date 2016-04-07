#include "leptonSPI.h"

const char *leptonSPI::device = "/dev/spidev1.0";
uint8_t leptonSPI::mode;
uint8_t leptonSPI::bits = 8;
uint32_t leptonSPI::speed = 16000000;
uint16_t leptonSPI::delay;

static void pabort(const char *s)
{
	perror(s);
	abort();
}

bool leptonSPI::initCamera() {
	int ret = 0;
	fd = open(device, O_RDWR);
	if (fd < 0)
	{
		pabort("can't open device");
	}

	ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);
	if (ret == -1)
	{
		pabort("can't set spi mode");
	}

	ret = ioctl(fd, SPI_IOC_RD_MODE, &mode);
	if (ret == -1)
	{
		pabort("can't get spi mode");
	}

	ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
	if (ret == -1)
	{
		pabort("can't set bits per word");
	}

	ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
	if (ret == -1)
	{
		pabort("can't get bits per word");
	}

	ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
	if (ret == -1)
	{
		pabort("can't set max speed hz");
	}

	ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
	if (ret == -1)
	{
		pabort("can't get max speed hz");
	}

	printf("spi mode: %d\n", mode);
	printf("bits per word: %d\n", bits);
	printf("max speed: %d Hz (%d KHz)\n", speed, speed / 1000);

	return true;
}

int leptonSPI::getFrame() {
	int ret;
	int i;
	int frame_number;
	uint8_t tx[PACKET_SIZE] = { 0, };

	//Sets up where to write
	
	tr.tx_buf = (unsigned long)tx,
	tr.rx_buf = (unsigned long)lepton_frame_packet,
	tr.len = PACKET_SIZE,
	tr.delay_usecs = delay,
	tr.speed_hz = speed,
	tr.bits_per_word = bits,
	

	//Actually Get Data
	ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
	if (ret < 1)
		pabort("can't send spi message");

	//Check Discard Packets xFxx
	if (((lepton_frame_packet[0] & 0xf) != 0x0f))
	{
		frame_number = lepton_frame_packet[1];

		if (frame_number < 60)
		{
			for (i = 0; i<80; i++)
			{
				lepton_image[frame_number][i] = (lepton_frame_packet[2 * i + 4] << 8 | lepton_frame_packet[2 * i + 5]);

			}

		}
        for(i = 0; i < 160; i++){
            frame[frame_number*80+i] = lepton_frame_packet[i];
        }
	}

	return frame_number;
}

leptonSPI::leptonSPI() {
	initCamera();
	//Make PGM file
}

leptonSPI::~leptonSPI() {

}
