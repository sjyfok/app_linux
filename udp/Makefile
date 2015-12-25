#
# Makefile for Chapter 12
#
# Type  make    to compile all the programs
# in the chapter 
#
CC=gcc
all: dgserver dgclient
dgserver: dgram.c dgrecv.c
	$(CC) -o dgserver dgram.c dgrecv.c 
dgclient: dgsend.c dgram.c
	$(CC) -o dgclient dgsend.c dgram.c
clean:
	rm -f *.o dgserver dgclient



