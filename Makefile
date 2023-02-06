CC = gcc
CFLAGS = -Wall
PROGRAMS = main


all: $(PROGRAMS)

sema: sema.c
	$(CC) main.c -o main $(CFLAGS)

clean:
	rm -rf $(PROGRAMS)