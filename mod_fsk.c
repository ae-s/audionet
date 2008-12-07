/* mod-fsk.c
 *
 * Basic MFSK modulation.
 *
 * Copyright 2008 Duncan Smith.
 * GPL.
 */

#include "mod_fsk.h"
#include <math.h>
#include "modem.h"
#include <stdlib.h>

#define BAUD 50
#define BITSBAUD 4
#define RATE 8000
#define TONE_DUR (RATE/BAUD)

#if BITSBAUD == 2
/* 2 bits/baud, 4 freqs */
int freqs[] = {1000, 1500, 2000, 2500};
#elif BITSBAUD == 4
/* 4 bits/baud, 16 freqs */
int freqs[] = { 1000, 1125, 1250, 1375,
		1500, 1625, 1750, 1875,
		2000, 2125, 2250, 2375,
		2500, 2625, 2750, 2875 };
#endif

/* Modulate a bitstream into an audio stream (sequence of ints).
 * Return a pointer to the audio buffer, or NULL on error.
 */

sample_t *fsk_modulate(int len, char *data, int *outlen)
{
	int buflen = sizeof(char) * TONE_DUR * len * 8/BITSBAUD;
	sample_t *buf = malloc(buflen);
	int i;
	sample_t *position = buf;
	int distance = 0;

	if (buf == NULL)
		return buf;

	*outlen = buflen;

	for ( i=0; i<len; i++ ) {
#if BITSBAUD == 2
		fsk_modulate_symbol((data[i] & 0xC0) >> 6, position, &distance);
		fsk_modulate_symbol((data[i] & 0x30) >> 4, position, &distance);
		fsk_modulate_symbol((data[i] & 0x0C) >> 2, position, &distance);
		fsk_modulate_symbol((data[i] & 0x03), position, &distance);
#elif BITSBAUD == 4
		fsk_modulate_symbol((data[i] & 0xF0) >> 4, position, &distance);
		fsk_modulate_symbol((data[i] & 0x0F), position, &distance);
#endif
	}

	return buf;
}

int fsk_modulate_symbol(int twit, sample_t *store, int *distance)
{
	int samp_left = TONE_DUR;
	int freq = freqs[twit];

	for ( ; samp_left > 0 ; samp_left-- ) {
		(*distance)++;
		*(store + *distance) = 
			(sample_t) (127 + 32 * (
					    cos(((float)(freq + *distance) * 2 * 3.141593 * freq)
						/ (float)RATE)));
	}

	return TONE_DUR;
}

