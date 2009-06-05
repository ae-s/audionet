#ifndef MODEM_H
#define MODEM_H

#define sample_t signed char
#define sample_to_char(x) (x)

/* The sample frequency.  When I'm unlazy enough to make it grok 48kHz
 * audio, change this.
 */
#define RATE 44100

/* The frequency (in Hz) of the sync pulse at the beginning of each packet.
 */
#define SYNC_FREQ 1200

/* The number of baud-times that a sync pulse lasts.  There is a phase
 * shift of +1/2*PI between each baud-time.
 */
#define SYNC_LEN 3



#endif
