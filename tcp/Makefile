#
# Makefile for Chapter 12
#
# Type  make    to compile all the programs
# in the chapter 
#
CC=gcc
all: server client
server: socklib.c tcps.c
	$(CC) -o server socklib.c tcps.c 
client: tcpc.c socklib.c
	$(CC) -o client socklib.c tcpc.c
clean:
	rm -f *.o server client



