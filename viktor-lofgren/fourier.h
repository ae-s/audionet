#include <stdio.h>

#ifndef FOURIER_H
#define FOURIER_H

void lowpass(double x[], int length, double frq, double rate);
void highpass(double x[], int length, double frq, double rate);

double fourier1(double x_in[], double n, int length);
void fourier_w(double x_in[], double x_out[], int length);
void fourier_xpm_start(int seg_length, int file_length, FILE* output);
void fourier_xpm_line(double x_in[], int length, FILE* output);

double fourier1p(double x_in[], double n, int length, double* phase_r, double* phase_i);
#endif
