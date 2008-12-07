CFLAGS=-lm -Wall
TARGET=out-harness
OBJS=out-harness.o mod_fsk.o

out-harness: $(OBJS)
	gcc -o $(TARGET) $(OBJS) -lm

out-harness.o: mod_fsk.h modem.h

mod_fsk.o: mod_fsk.h

mod_fsk: mod_fsk.h
