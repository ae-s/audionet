/* demod-msfk.c
 *
 * Basic MFSK demodulation, to match mod_mfsk.
 *
 * Copyright 2009 Duncan Smith.
 * GPL.
 */

#include "demod_mfsk.h"
#include "modem.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define M_DEBUG

/* Demodulate an audiostream into a bitstream.
 * Return a pointer to the data buffer, or NULL on error.
 */

char *mfsk_demodulate(int len, sample_t *data, int *outlen)
{
	int buflen = len / (sizeof(sample_t) * M_TONE_DUR * (len * 8)/M_BITSBAUD);
	int tones[M_SLOTS];
	int prev_tones[M_SLOTS];
	int i, slot;
	char *buf = malloc(buflen);
	char *position = buf;

	int read = 0;
}

char *mfsk_decode_baud(sample_t *audio, sample_t 
