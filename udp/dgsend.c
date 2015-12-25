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
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#define oops(m,x)  { perror(m);exit(x);}

#define	 PACK_SIZE  4096

struct readbuf
{
	char buf[PACK_SIZE];
	int byte_cnt;
};

struct readbuf testbuf[2];


int make_dgram_client_socket();
int make_internet_address(char *,int, struct sockaddr *);

int main(int ac, char *av[])
{
	int	sock;			/* use this socket to send	*/
	char	*msg;			/* send this messag		*/
	struct  sockaddr  saddr;	/* put sender's address here	*/
	socklen_t saddrlen;		/* and its length here		*/
	char buf[BUFSIZ];
	int msglen = 0;
	int fd_read = 0;
	
	int file_size = 0;
	struct stat file_info;

	int send_byte = 0;
	int read_byte = 0;

	int idx = 0;

	fd_set read_set;
	int max_fd = 0, ret = 0;
	struct timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 1;

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

    /* send a string through the socket to that address */

	//open a big file
	fd_read = open("/root/test.tar", O_RDONLY);
	if (fd_read == -1)
	{
		perror("Fail to open:");
		return -1;
	}
	if (stat("/root/test.tar", &file_info) == -1)
	{
		perror("Fail to stat:");
		return -1;
	}
	file_size = file_info.st_size;
	send_byte = 0;

	testbuf[0].byte_cnt = read(fd_read, testbuf[0].buf, PACK_SIZE);
	if (testbuf[0].byte_cnt == -1)
	{
		perror("Fail to read:");
		close(fd_read);
		return -1;
	}

  idx = 0;

	FD_ZERO(&read_set);
	memcpy(&read_set, &sock, sizeof(fd_set));
	max_fd = sock+1;
  while (send_byte < file_size)   //send a big file
  {
		if (sendto(sock, testbuf[0].buf, PACK_SIZE, 0, &saddr, (size_t)&saddrlen) == -1)
		{
			oops("sendto failed", 3);
		}
		ret =	select(max_fd, &read_set, NULL, 0, &timeout);
		if (ret == -1)
		{
			perror("Fail to select:");
		}
		if (ret == 0)  //timeover
		{
			continue; //resend	
		}
		if (FD_ISSET(sock, &read_set)) 
		{
			if ((msglen = recvfrom(sock, buf, BUFSIZ, 0, &saddr, &saddrlen)) > 0)
			{
				printf("recv: %s\n", buf);
				send_byte += PACK_SIZE;
				testbuf[0].byte_cnt = read(fd_read, testbuf[0].buf, PACK_SIZE);
				if (testbuf[0].byte_cnt == -1)
				{
					perror("Fail to read:");
					close(fd_read);
					return -1;
				}
			}
			else
			{
				perror("Fail to recv:");
				return -1;
			}
		}

  }

	return 0;
}
