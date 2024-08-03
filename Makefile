FILES = nes.c
CC = gcc
CFLAGS = -g -w
LIBS = -lm

nes: nes.c
	$(CC) $(FILES) $(CFLAGS) $(LIBS) -o nes
