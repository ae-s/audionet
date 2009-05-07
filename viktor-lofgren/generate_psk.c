#include <fcntl.h>
#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <stdarg.h>
#include <memory.h>
#include <math.h>

int rate = 8000;
int bps = 10;
FILE* infile = NULL;
FILE* outfile = NULL;

double carrier = 8000.0;

void usage(char* cmd) {
	printf("Usage: %s [-r sample rate ] [-b baud] [-c carrier] [-s] [-h] [ -o file ] [ file ]\n" 
		"\t -r : Set signal sample rate (default = 8000)\n" 
		"\t -o : Write data to file\n"
		"\t -c : Set carrier frequency\n"
		"\t -b : Set transferred bits per second\n" 
		"\t -h : Display this message\n\n", cmd);
}

double car_phase = 0;

void generate(int length) {
	char* data = malloc(length);
	memset(data, 0, length);
	int i;

	for(i = 0; i < length; i++) {
		data[i] += (int)(64.0 * sin(2*M_PI*carrier * (i) / (float) rate + car_phase));
	}

	car_phase += 2*M_PI*carrier  *i / (float) rate;
	car_phase -= 2*M_PI * (int) (car_phase / (2*M_PI));
	fwrite(data, length, 1, outfile);
	free(data);
}

void generate_nextbit() {
	car_phase += 3 * M_PI / 2.0;
	generate(rate/bps);
}

void generate_bit(char c) {
	car_phase += c ? M_PI / 2.0 : M_PI;
	generate(rate/bps);
}

int main(int argc, char* argv[]) {
	infile = stdin;
	outfile = stdout;

	for(;;) switch(getopt(argc, argv, "r:hb:o:c:")) {
		case 'r':
			rate = atoi(optarg);
			break;
		case 'b': bps = atoi(optarg);
			break;
		case 'c': carrier = atof(optarg);
			break;
		case 'o':
			outfile = fopen(optarg, "w");
			if(outfile == NULL) {
				perror(optarg);
				exit(EXIT_FAILURE);
			}
			break;
		case 'h': usage(argv[0]);
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

	int c;
	while( ( c = fgetc(infile)) != EOF ) {
		generate_bit(c & 128);
		generate_bit(c & 64);
		generate_bit(c & 32);
		generate_bit(c & 16);
		generate_bit(c & 8);
		generate_bit(c & 4);
		generate_bit(c & 2);
		generate_bit(c & 1);
		generate_nextbit();
	
	}
	
	

	return EXIT_SUCCESS;

}
