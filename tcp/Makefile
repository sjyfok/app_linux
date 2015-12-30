#
# Makefile for Chapter 12
#
# Type  make    to compile all the programs
# in the chapter 
#
CC=gcc
all: server client
server: tcpsocklib.c server.c
	$(CC) -o server tcpsocklib.c server.c 
client: client.c tcpsocklib.c
	$(CC) -o client tcpsocklib.c client.c
clean:
	rm -f *.o server client



