#include "leptonSPI.h"


int main(int argc, char *argv[]) {
	leptonSPI c;

	while (c.getFrame() != 59) {}


	/*
	int i;
	int j;
	unsigned int maxval = 0;
	unsigned int minval = UINT_MAX;
	char image_name[32];
	int image_index = 0;

	do {
		sprintf(image_name, "IMG_%.4d.pgm", image_index);
		image_index += 1;
		if (image_index > 9999)
		{
			image_index = 0;
			break;
		}

	} while (access(image_name, F_OK) == 0);

	FILE *f = fopen(image_name, "w");
	if (f == NULL)
	{
		printf("Error opening file!\n");
		exit(1);
	}

	printf("Calculating min/max values for proper scaling...\n");
	for (i = 0; i<60; i++)
	{
		for (j = 0; j<80; j++)
		{
			if (c.lepton_image[i][j] > maxval) {
				maxval = c.lepton_image[i][j];
			}
			if (c.lepton_image[i][j] < minval) {
				minval = c.lepton_image[i][j];
			}
		}
	}
	printf("maxval = %u\n", maxval);
	printf("minval = %u\n", minval);

	fprintf(f, "P2\n80 60\n%u\n", maxval - minval);
	for (i = 0; i<60; i++)
	{
		for (j = 0; j<80; j++)
		{
			fprintf(f, "%d ", c.lepton_image[i][j] - minval);
		}
		fprintf(f, "\n");
	}
	fprintf(f, "\n\n");

	fclose(f);
	*/
	return 0;
}