/*********************************************************************
 * dgsend.c  - datagram sender
 * 	         usage: dgsend hostname portnum "message"
 * 	        action: sends message to hostname:portnum
 */

#include	<stdio.h>
#include	<stdlib.h>
#include	<sys/types.h>
#include	<sys/socket.h>
#include	<netinet/in.h>
#include <string.h>

#define oops(m,x)  { perror(m);exit(x);}


int make_dgram_client_socket();
int make_internet_address(char *,int, struct sockaddr_in *);
int sendpack(int sock, struct sockaddr_in *saddr, char buf[], int len, struct timeval timeout);

#define		PACK_SIZ		1024

char testbuf[PACK_SIZ];

int main(int ac, char *av[])
{
	int	sock;			/* use this socket to send	*/
	char	*msg;			/* send this messag		*/
	struct  sockaddr_in   saddr;	/* put sender's address here	*/
//	socklen_t saddrlen;
	//fd_set read_set;
//	int max_fd, ret;
	struct timeval timeout;
	int rxlen = 0;

	if ( ac != 4 ){
		fprintf(stderr,"usage: dgsend host port 'message'\n");
		exit(1);
	}
	msg = av[3];

    /* get a datagram socket */

	if( (sock = make_dgram_client_socket()) == -1 )
		oops("cannot make socket",2);

    /* combine hostname and portnumber of destination into an address */

	if ( make_internet_address(av[1], atoi(av[2]), &saddr) == -1 )
		oops("make addr",4);

//	saddrlen = sizeof(saddr);
//	max_fd = sock+1;


	while (1)
	{
		timeout.tv_sec = 10;
		timeout.tv_usec = 0;
		if (sendpack(sock, &saddr, msg, strlen(msg), timeout) == -1) //传送失败
		{
		}
		else
		{
			printf("send ok\n");
			break;
		}

/*	if ( sendto(sock, msg, strlen(msg), 0, 
				(struct sockaddr *)&saddr,sizeof(saddr)) == -1)
		{
			oops("sendto failed", 3);
		}

		FD_ZERO(&read_set);
		FD_SET(sock, &read_set);


		ret =	select(max_fd, &read_set, NULL, NULL, &timeout);
		if (ret == -1)
		{
			perror("Fail to select:");
		}
		if (FD_ISSET(sock, &read_set)) 
		{		
			if ((rxlen = recvfrom(sock, testbuf, PACK_SIZ, 0, (struct sockaddr*)&saddr, &saddrlen)) > 0)
			{
				testbuf[rxlen] = NULL;
				printf("get reply: %s\n", testbuf);
				break;
			}
		}
		else if(ret == 0) //timeout
		{
			//resend 
		}*/

	}
	return 0;
}


//发送一包数据，一直等到服务器确认，或者超时 
int sendpack(int sock, struct sockaddr_in *saddr, char buf[], int len, struct timeval timeout)
{
	int max_fd;
	int ret, rxlen;
	char reply[BUFSIZ];
	socklen_t saddrlen;

	fd_set read_fds;

	FD_ZERO(&read_fds);
	FD_SET(sock, &read_fds);
	max_fd = sock+1;

	saddrlen = sizeof(struct sockaddr_in);

	if (sendto(sock, buf, len, 0, (struct sockaddr*)saddr, saddrlen) == -1)
	{
		perror("Fail to sendto:");		
		return -1;
	}
	ret = select(max_fd, &read_fds, NULL, NULL, &timeout);
	if (ret == -1)
	{
		perror("Fail to select:");
		return -1;
	}
	if (FD_ISSET(sock, &read_fds))
	{
		if ((rxlen = recvfrom(sock, reply, BUFSIZ, 0, (struct sockaddr*)&saddr, &saddrlen)) > 0)
		{
			if (strncmp(reply, "ACK", 3) == 0)   //接受到ＡＣＫ将正确返回
			{
				return 0;
			}
		}
	}
	return -1;  //超时或者出错
}

