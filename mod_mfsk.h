#ifndef MOD_MFSK_H
#define MOD_MFSK_H

#define MOD_MFSK_PROTO 2

#include "modem.h"

sample_t *mfsk_modulate(int len, char *data, int *outlen);
int mfsk_choose_tone(int data, int slot, int prev_freq);
int mfsk_encode_baud(int *tones, sample_t *position);

#define M_BAUD 40

/* 1-of-N, what is log2(N)? */
#define M_BITS 4
#define M_WIDTH 16
/* Number of 1-of-N slots to have */
#define M_SLOTS 16

/* 80 bits/baud with 20 slots of width 16+2, takes 360 tones */
#define M_BITSBAUD (M_BITS*M_SLOTS)

/* Guard band width, in tones */
#define M_GUARD 0
/* Non-reuse interval, in bauds */
#define M_NO_REUSE 1

#define M_TONE_DUR (RATE/M_BAUD)
#define M_SIL_DUR (RATE/80)

#define M_BASE_FREQ 200
#define M_FREQ_STEP 60
#define M_FREQ_MULT 1.2


#endif
