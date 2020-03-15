#Sample Makefile for Malloc
CC=gcc
CFLAGS=-g -O0 -fPIC

all:	check


malloclib.so: malloclib.o
	$(CC) $(CFLAGS) -shared -Wl,--unresolved-symbols=ignore-all -pthread $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) $< -c -o $@

check:	malloclib.so 
	LD_PRELOAD=`pwd`/malloclib.so firefox

dist:
	dir=`basename $$PWD`; cd ..; tar cvf $$dir.tar ./$$dir; gzip $$dir.tar

clean:
	rm -rf malloclib.so malloclib.o