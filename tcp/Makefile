#
# Makefile for Chapter 12
#
# Type  make    to compile all the programs
# in the chapter 
#

clean:
	rm -f *.o server client
server: socklib.c tcps.c
	arm-linux-gcc -o server socklib.c tcps.c 
client: tcpc.c socklib.c
	cc -o client socklib.c tcpc.c



