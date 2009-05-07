/* mod_sync.c
 *
 * Generate sync waveform for beginning of frames.
 *
 * Copyright 2008 Duncan Smith.
 * GPL.
 */

#include "mod_sync.h"
#include "modem.h"
#include <math.h>

/* Allocates a space big enough for the sync pulse.  Returns the size,
 * in samples, and modifies the pointer `area' to point to the pulse.
 */
int sync_modulate(sample_t **area, int baud)
{
	
}

int sync_modulate_single(float phase)
{
	int samp_left = ;
	int freq = SYNC_FREQ;

	for ( ; samp_left > 0 ; samp_left-- ) {
		(*distance)++;
		*(store + *distance) = 
			(sample_t) (127 + 32 * (
					    cos(((float)(freq + *distance) * 2 * M_PI * freq)
						/ (float)RATE)));
	}

	return TONE_DUR;
}

