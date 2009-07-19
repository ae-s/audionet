/* out-harness.c
 *
 * Harness for testing output framework.
 *
 * Copyright 2008 Duncan Smith.
 * GPL.
 */

#include "mod_mfsk.h"
#include "modem.h"
#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv)
{
	int len = 0;
	sample_t *data = mfsk_modulate(1020, 
"Hey, I know what we should do!  Let's get some SHOES.  Let's get some-", &len);
//	sample_t *data = mfsk_modulate(580, "\x05\x4A\x34", &len);
	write( 2,
	       (void *) (data),
	       len );
	
}
