#!/bin/sh
gcc -lm analyze_psk.c fourier.c -o analyze_psk
gcc -lm generate_psk.c -o generate_psk
gcc record.c -o record
gcc playback.c -o playback
