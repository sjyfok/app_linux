#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <time.h>

 
int port = 6789;
char buf[1024];

void main() {
  int socket_descriptor;
  int iter = 0;
  struct sockaddr_in address;
	time_t timeval = 0;
	int bytecnt = 0;

  bzero(&address, sizeof(address)); /* empty data structure */
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = inet_addr("192.168.200.101"); 
  address.sin_port = htons(port);
  socket_descriptor = socket(AF_INET, SOCK_DGRAM, 0);
	memset(buf, '#', 1024);

	timeval = time((time_t *)0);
	printf("$$$%d\n", timeval);
 	while(iter < 102400)
  {
    sendto(socket_descriptor,
           buf, sizeof(buf),
           0, (struct sockaddr *)&address, sizeof(address));
     bytecnt += 1024; 
//     sleep(1);
     iter ++;
  }

	timeval = time((time_t *)0);
	strcpy(buf, "stop");
	sendto(socket_descriptor,
				buf, sizeof(buf), 0, (struct sockaddr*)&address, sizeof(address));
	bytecnt += 4;

	printf("$$$%d\n", timeval);
	printf("###%d\n", bytecnt);
   close(socket_descriptor); 
   printf("Messages Sent, Terminating\n");
   exit (0);
}

