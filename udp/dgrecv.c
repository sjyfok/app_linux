/************************************************************************
 * dgrecv.c  - datagram receiver
 * 	         usage: dgrecv portnum 
 * 	        action: listens at the specfied port and reports messages
 */

#include	<stdio.h>
#include	<stdlib.h>
#include	<sys/types.h>
#include	<sys/socket.h>
#include	<netinet/in.h>
#include <string.h>

#define oops(m,x)  { perror(m);exit(x);}

int  make_dgram_server_socket(int);
int  get_internet_address(char *, int,  int *, struct sockaddr_in *);
void say_who_called(struct sockaddr_in *);
void reply_to_sender(int sock, char *buf, int buflen,  struct sockaddr_in *saddr, socklen_t len);

int main(int ac, char *av[])
{
	int	port;			/* use this port		*/
	int	sock;			/* for this socket		*/
	char	buf[BUFSIZ];		/* to receive data here		*/
	size_t	msglen;			/* store its length here	*/
	struct  sockaddr_in   saddr;	/* put sender's address here	*/
	socklen_t saddrlen;		/* and its length here		*/
	int frm_idx = 0;

	if ( ac == 1 || (port = atoi(av[1])) <= 0 ){
		fprintf(stderr,"usage: dgrecv portnumber\n");
		exit(1);
	}

    /*  get a socket and assign it a port number */

	if( (sock = make_dgram_server_socket(port)) == -1 )
		oops("cannot make socket",2);

    /* receive messaages on that socket */

	saddrlen = sizeof(saddr);
	int read_fd;
	int idx = 0;
	char reply[BUFSIZ];
	int rbyte;
	while( (msglen = recvfrom(sock,buf,BUFSIZ,0,
				(struct sockaddr *) &saddr,&saddrlen))>0 ) {
		buf[msglen] = '\0';
		//printf("%s\n", buf);
	
		if (strncmp(buf, "GLEN", 4) == 0) //获取文件长度 
		{
			int len =	getfilelen(&buf[4]);
			if (len > 0) 
			{
				close(read_fd);
				read_fd = openrdfile(&buf[4]);
				if (read_fd == -1) 
				{
				  printf("can't read file\n");
				  len = 0;
				  break;
				}
			}
			char reply[20];
			sprintf(reply, "%d", len);
		//	printf("reply:%s\n", reply);
			reply_to_sender(sock, reply, strlen(reply), &saddr, saddrlen);
			frm_idx = 0;
//			printf("getlen\n");
		}
	
		if (strncmp(buf, "GFIL", 4) == 0) //读取文件文件内容
		{
			idx = atoi(&buf[4]);
			//printf("idx = %d  frm_idx = %d\n", idx, frm_idx);
			if (idx != frm_idx)
			{
				rbyte = readfile(read_fd, reply, BUFSIZ);
				//printf("read = %s, %d\n", reply, rbyte);
				if (rbyte < 0) //error
				{
					break;
				}
				frm_idx ++;
			}
			reply_to_sender(sock, reply, rbyte, &saddr, saddrlen);		
		}

//		printf("dgrecv: got a message: %s", buf);
//		say_who_called(&saddr);
	}
	close(read_fd);
	return 0;
}


void reply_to_sender(int sock, char *buf, int buflen, struct sockaddr_in *saddr, socklen_t len)
{

//	sprintf(reply, "Thanks for you %d char message\n", (int)strlen(buf));
	sendto(sock, (const char*) buf, buflen, 0, (struct sockaddr*)saddr, len);
}


void say_who_called(struct sockaddr_in *addrp)
{
	char	host[BUFSIZ];
	int	port;

	get_internet_address(host,BUFSIZ,&port,addrp);
	printf("  from: %s:%d\n", host, port);
}

