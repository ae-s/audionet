/* mod-mfsk.c
 *
 * Basic MFSK modulation.
 *
 * Copyright 2008 Duncan Smith.
 * GPL.
 */

#include "mod_mfsk.h"
#include "modem.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define M_DEBUG

/* Modulate a bitstream into an audio stream (sequence of ints).
 * Return a pointer to the audio buffer, or NULL on error.
 */

sample_t *mfsk_modulate(int len, char *data, int *outlen)
{
	int buflen = 4 * sizeof(sample_t) * M_TONE_DUR * (len * 8.0)/M_BITSBAUD;
	int tones[M_SLOTS];
	int prev_tones[M_SLOTS];
	int i, slot;
	sample_t *buf = malloc(buflen);
	sample_t *position = buf;

	int written = 0;

	printf("samp/tone: %d, len: %d, buffer len: %d\n", M_TONE_DUR, len, buflen);

	if (buf == NULL)
		return buf;

	memset(prev_tones, 0, M_SLOTS*sizeof(int));

	*outlen = buflen;

	for ( i=0; i<len; ) {
		/* Consume M_BITS bits at a time */
#ifdef M_DEBUG
		printf("\nData: ");
#endif
		for (slot = 0; slot < M_SLOTS; slot++) {

#ifdef M_DEBUG
			printf("\n%%%hhx", data[i]);
#endif
#if M_BITS == 2
			tones[slot] = mfsk_choose_tone((data[i] & 0xC0) >> 6, slot, prev_tones[slot]);
			prev_tones[slot] = tones[slot];
			slot++;
			tones[slot] = mfsk_choose_tone((data[i] & 0x30) >> 4, slot, prev_tones[slot]);
			prev_tones[slot] = tones[slot];
			slot++;
			tones[slot] = mfsk_choose_tone((data[i] & 0x0C) >> 2, slot, prev_tones[slot]);
			prev_tones[slot] = tones[slot];
			slot++;
			tones[slot] = mfsk_choose_tone((data[i] & 0x03),      slot, prev_tones[slot]);
			prev_tones[slot] = tones[slot];

			i++;
#elif M_BITS == 4
			tones[slot] = mfsk_choose_tone((data[i] & 0xF0) >> 4, slot, prev_tones[slot]);
			prev_tones[slot] = tones[slot];
			slot++;
			tones[slot] = mfsk_choose_tone((data[i] & 0x0F),      slot, prev_tones[slot]);
			prev_tones[slot] = tones[slot];
			i++;

#endif
#ifdef M_DEBUG
			printf(" %d / %d %d", slot, tones[slot-1], tones[slot]);
#endif

		}

		written = mfsk_encode_baud(tones, position);
		(*outlen) += written * sizeof(sample_t);
		position += written;

#ifdef M_DEBUG
		printf("\n\n Loop cond: i = %d, len = %d, end = %d, outlen = %d\n", i, len, ((8*len)/(M_BITSBAUD)), *outlen);
#endif
	}

	return buf;
}

/* Choose a tone for the slot, given the data, slot number, and
 * previous slot contents.
 */

int mfsk_choose_tone(int data, int slot, int prev_freq)
{
	float base = M_BASE_MULT + (float)(M_NO_REUSE + M_WIDTH + M_GUARD) * M_FREQ_MULT * (float)slot;

	int freq = pow(2, base + data * M_FREQ_MULT);

#ifdef M_DEBUG
	printf("base: %f\n", base);
#endif

	if ((freq >= prev_freq) && (prev_freq != 0)) {
		freq += M_FREQ_STEP;
#ifdef M_DEBUG
		printf("Skipping up\n");
#endif
	}

	return freq;
}

/* Encode a baud (20 tones) into PCM for transmission.
 */

int mfsk_encode_baud(int *tones, sample_t *position)
{
	int samp_left = M_TONE_DUR;
	int distance = 0;

	for ( ; samp_left > 0 ; samp_left-- ) {
		float sample = 0;
		int slot;
		distance++;
		for (slot = 0; slot < M_SLOTS; slot++) {
			sample += (cos(((float)(tones[slot] + distance) * 2 * M_PI * tones[slot])
				       / (float)RATE));
		}

		*(position++) = (int)((256 * sample) / (M_SLOTS + 1));
	}

	return M_TONE_DUR + M_SIL_DUR;
}
