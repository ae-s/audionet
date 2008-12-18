/* mod_psk.c
 *
 * Basic PSK modulation.
 *
 * Copyright 2008 Duncan Smith.
 * GPL.
 */

#include "mod_psk.h"
#include <math.h>
#include "modem.h"
#include <stdlib.h>

#define BAUD 50
#define BITSBAUD 4
#define TONE_DUR (RATE/BAUD)

int freq = 1000

/* Modulate a bitstream into an audio stream (sequence of ints).
 * Return a pointer to the audio buffer, or NULL on error.
 */

sample_t *psk_modulate(int len, char *data, int *outlen)
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
#if BITSBAUD == 1
		fsk_modulate_symbol((data[i] & 0x80) >> 7, position, &distance);
		fsk_modulate_symbol((data[i] & 0x40) >> 6, position, &distance);
		fsk_modulate_symbol((data[i] & 0x20) >> 5, position, &distance);
		fsk_modulate_symbol((data[i] & 0x10) >> 4, position, &distance);
		fsk_modulate_symbol((data[i] & 0x08) >> 3, position, &distance);
		fsk_modulate_symbol((data[i] & 0x04) >> 2, position, &distance);
		fsk_modulate_symbol((data[i] & 0x02) >> 1, position, &distance);
		fsk_modulate_symbol((data[i] & 0x01), position, &distance);
#elif BITSBAUD == 2
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

int psk_modulate_symbol(int twit, sample_t *store, int *distance)
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

