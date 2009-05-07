/* demod_sync.c
 *
 * Acquire sync with incoming signal.
 *
 * Copyright 2008 Duncan Smith.
 * GPL.
 */

#include "modem.h"
#include "demod_sync.h"

/* Acquire sync has to happen once we've identified a packet start.
 */

int find_sync_in(int len, sample_t *buf, int min_snr)
{
	
}
