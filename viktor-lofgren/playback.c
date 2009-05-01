#include <stdlib.h>
#include <fcntl.h>
#include <linux/soundcard.h>
#include <errno.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <string.h>

int rate = 48000;
int fmt_signed = 1;
char* dsp = "/dev/dsp";

void initialize(int fd) {
	int arg = 0;

	arg = AFMT_S8;
	if(ioctl(fd, SNDCTL_DSP_SETFMT, &arg) == -1) {
		perror("SNDCTL_DSP_SETFMT");
		exit(EXIT_FAILURE);
	}
	if(arg != AFMT_S8) {
		if(arg == AFMT_U8) fmt_signed = 0;
		else {
			 fprintf(stderr, "Unable to set 8 bit format\n");
			exit(EXIT_FAILURE);
		}
	}

	arg = 1;
	if(ioctl(fd, SOUND_PCM_WRITE_CHANNELS, &arg) == -1)  {
		perror("SNDCTL_DSP_STEREO");
		exit(EXIT_FAILURE);
	}
	if(arg != 1) {
		fprintf(stderr, "Unable to set mono mode\n");
		exit(EXIT_FAILURE);
	}

	arg = rate;
	if(ioctl(fd, SNDCTL_DSP_SPEED, &arg) == -1) { 
		perror("SNDCTL_DSP_SPEED");
		exit(EXIT_FAILURE);
	}
	if(arg != rate) {
		fprintf(stderr, "Unable to set rate\n");
		exit(EXIT_FAILURE);
	}

}
void usage(char* cmd) {
	fprintf(stderr, "usage: %s [-h] [-d dev] [-i outfile] [-r rate]\n"
			"\t -h : Display this message\n"
			"\t -d : Specify device (e.g. /dev/dsp)\n"
			"\t -i : Set input file (default stdin)\n"
			"\t -r : Set rate\n\n", cmd);
			
}

int main(int argc, char* argv[]) {
	FILE* infile = stdin;
	char* buffer = NULL;
	int fd = 0, i;

	while((i = getopt(argc, argv, "r:d:hi:"))) switch(i) {
		case 'h': usage(argv[0]); return EXIT_SUCCESS;
		case 'r': rate = atoi(optarg); break;
		case 'd': dsp = strdup(optarg); break;
		case 'i': 
			infile = fopen(optarg, "r"); 
			if(!infile) perror(optarg);
			break;
		case -1: goto done;
		default: exit(EXIT_FAILURE);
		
	}	
	done:
	buffer = malloc(rate);

	fd = open(dsp, O_WRONLY);
	if(fd < 0) { perror(dsp); exit(EXIT_FAILURE); }
	initialize(fd);

	while(fread(buffer, 1, rate, infile) == rate) {
		if(!fmt_signed) for(i = 0; i < rate; i++)
				((unsigned char*)buffer)[i] = buffer[i] + 127;
		if(fmt_signed) write(fd, buffer, rate);
		
	}

	return EXIT_SUCCESS;
}
