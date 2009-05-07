#include <fcntl.h>
#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <stdarg.h>
#include <memory.h>
#include <math.h>
#include <ctype.h>

#include "fourier.h"

int rate = 8000;
int bps = 10;
double threshold = 0.01;

double carrier = 8000.0;

FILE* infile = NULL;
FILE* outfile = NULL;

void usage(char* cmd) {
	printf("Usage: %s [-r sample rate] [-b baud] [-c carrier] [-h] [-x] [ -o file ] [ file ]\n" 
		"\t -r : Set sample rate (default = 8000)\n" 
		"\t -o : Write data to file\n"
		"\t -x : Generate XPM with frequency spectrum\n"
		"\t -b : Set transferred bits per second\n" 
		"\t -c : Set carrier frequency\n"
		"\t -h : Display this message\n\n", cmd);
}

int main(int argc, char* argv[]) {
	infile = stdin;
	outfile = stdout;
	int i = 0;

	for(;;) switch(getopt(argc, argv, "r:shb:o:c:")) {
		case 'r':
			rate = atoi(optarg);
			break;
		case 'b': bps = atoi(optarg);
			break;
		case 'c':
			carrier = atof(optarg);
			break;
		case 'o':
			outfile = fopen(optarg, "w+");
			if(outfile == NULL) {
				perror(optarg);
				exit(EXIT_FAILURE);
			}
			break;
		case 'h': usage(argv[0]);
			return EXIT_SUCCESS;
			break;
		case -1: goto done;
		default: exit(EXIT_FAILURE);
	}

	done:


	if(optind < argc) {
		infile = fopen(argv[optind], "r");
		if(infile == NULL) {
			perror(argv[optind]);
			exit(EXIT_FAILURE);
		}
	}

	/* Calculate file length */
	int file_length = 0;
	fseek(infile, 0, SEEK_END);
	file_length = ftell(infile);
	fseek(infile, 0, SEEK_SET);

	/* The buffer length to use */
	int length = rate / (4 * bps);

	/* Raw data buffer */
	char* buffer = malloc(length);

	/* Massaged data buffer */
	double* dbuffer = malloc(length);

	unsigned char bit_data = 0;

	double old_carrier_phase[2] = { 0.0, 0.0 };
	
	while((fread(buffer, 1, length, infile))) {
		/* Transform into a double in the range -1 ... 1 */
		for (i = 0; i < length; i++) {
			dbuffer[i] = buffer[i] / 128.0;
		}
		
		double carrier_phase[2];
		double carrier_strength = fourier1p(dbuffer, (float) length * carrier / (float)rate, length, &carrier_phase[0], &carrier_phase[1]);
		
		
		if(carrier_strength < threshold) continue;

/*  -- this is pretty useful in debuging */
//		printf("C: %f (%f %f)\n",
//			carrier_strength, carrier_phase[0], carrier_phase[1]);

	/*	(A+Bi)*(C-Di) = A*C + B*D + i(-A*D + B*C) */
		double delta_re = carrier_phase[0] * old_carrier_phase[0] + carrier_phase[1]*old_carrier_phase[1];
		double delta_im = -carrier_phase[1]*old_carrier_phase[0] + carrier_phase[0] * old_carrier_phase[1];

		if(delta_re * delta_re > delta_im * delta_im) { /* phase diff is a multiple of pi */
			if(delta_re > 0); /* No change */
			else {
				bit_data = bit_data * 2;
			}
		} else {
			if(delta_im > 0) {
				bit_data = bit_data * 2 + 1;
			} else {
				if(isprint(bit_data)) printf("%c", bit_data);
				else printf("<%.2x>", bit_data);
				bit_data = 0;
			}
		}

		old_carrier_phase[0] = carrier_phase[0];
		old_carrier_phase[1] = carrier_phase[1];


	}
	return EXIT_SUCCESS;

}
