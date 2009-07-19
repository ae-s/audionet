CFLAGS=-lm -Wall -ggdb3
TARGET=out-harness
OBJS=out-harness.o mod_mfsk.o
#CC=gcc-4.1

out-harness: $(OBJS)
	gcc -o $(TARGET) $(OBJS) -lm

out-harness.o: mod_mfsk.h modem.h

mod_fsk.o: mod_fsk.h modem.h

mod_fsk: mod_fsk.h modem.h

mod_mfsk.o: mod_mfsk.h modem.h

mod_mfsk: mod_mfsk.h modem.h

tun.c: mod_mfsk.h modem.h tun.h
