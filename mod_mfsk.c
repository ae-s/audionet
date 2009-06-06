/* mod-fsk.c
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



/* Modulate a bitstream into an audio stream (sequence of ints).
 * Return a pointer to the audio buffer, or NULL on error.
 */

sample_t *mfsk_modulate(int len, char *data, int *outlen)
{
	int buflen = sizeof(sample_t) * M_TONE_DUR * (len * 8)/M_BITSBAUD;
	int tones[M_SLOTS];
	int prev_tones[M_SLOTS];
	int i, slot;
	sample_t *buf = malloc(buflen);
	sample_t *position = buf;
	
	int written = 0;

	if (buf == NULL)
		return buf;

	memset(prev_tones, 0, M_SLOTS*sizeof(int));

	*outlen = buflen;

	for ( i=0; i<len; ) {
		/* Consume M_BITS bits at a time */
		printf("\nData: ");
		for (slot = 0; slot < M_SLOTS; slot++) {

			printf("\n%%%hhx/%c", data[i], data[i]);
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
			printf(" %d / %d %d", slot, tones[slot-1], tones[slot]);

		}

		written = mfsk_encode_baud(tones, position);
		(*outlen) += written * sizeof(sample_t);
		position += written;

		printf("\n\n Loop cond: i = %d, len = %d, end = %d, outlen = %d\n", i, len, ((8*len)/(M_BITSBAUD)), *outlen);
	}

	return buf;
}

/* Choose a tone for the slot, given the data, slot number, and
 * previous slot contents.
 */

int mfsk_choose_tone(int data, int slot, int prev_freq)
{
	int base = M_BASE_FREQ + (M_NO_REUSE + M_WIDTH + M_GUARD) * M_FREQ_STEP * slot;

	int freq = base + data * M_FREQ_STEP;

	if ((freq >= prev_freq) && (prev_freq != 0)) {
		freq += M_FREQ_STEP;
//		printf("Skipping up\n");
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
