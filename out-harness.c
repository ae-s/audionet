/* out-harness.c
 *
 * Harness for testing output framework.
 *
 * Copyright 2008 Duncan Smith.
 * GPL.
 */

#include "mod_fsk.h"
#include "modem.h"
#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv)
{
	int len = 0;
//	sample_t *data = fsk_modulate(38, "Hey, this is a rather longer message?", &len);
	sample_t *data = fsk_modulate(580, "\x05\x4A\x34", &len);
	write( 1,
	       (void *) (data),
	       len );
	
}
