/*********************************************************************
 * dgsend.c  - 从服务器获取信息
 *					STEP1 连接服务器
 *					STEP2 发送文件名字 获取文件的大小
 *					STEP3 接收文件数据并对每包数据进行确认
 * 	         usage: dgsend hostname portnum "file name"
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
int getpack(int sock, struct sockaddr_in *saddr, char buf[], int len, char *reply);
//从服务器获得文件的长度
int get_file_len(int sock, struct sockaddr_in *saddr, char *pfname, struct timeval *timeout);


#define		PACK_SIZ		1024

char testbuf[PACK_SIZ];

int main(int ac, char *av[])
{
	int	sock;			/* use this socket to send	*/
	char	*pfname;			/* 文件的路径和名字 */
	struct  sockaddr_in   saddr;	/* put sender's address here	*/
	struct timeval timeout;
	int rxlen = 0;
	int len = 0;
	int idx = 1;

	if ( ac != 4 )
	{
		fprintf(stderr,"usage: dgsend host port 'message'\n");
		exit(1);
	}
	pfname = av[3];

    /* get a datagram socket */

	if( (sock = make_dgram_client_socket()) == -1 )
		oops("cannot make socket",2);

    /* combine hostname and portnumber of destination into an address */

	if ( make_internet_address(av[1], atoi(av[2]), &saddr) == -1 )
		oops("make addr",4);

	timeout.tv_sec = 10;
	timeout.tv_usec = 0;  //500ms

	//get file length
	rxlen = get_file_len(sock, &saddr, pfname, &timeout);
	if (rxlen == -1)  //error
	{
		printf("Fail to get file length\n");
		return -1;
	}	
	if (rxlen == 0)
	{
		printf("File si empty!\n");
		return 0;
	}
	idx = 1;
	printf("rxlen = %d\n", rxlen);
	int byte_cnt = 0;
	char buf[100];
	strcpy(buf, "GFIL");
	char reply[BUFSIZ];

	int fd_wr;
	fd_wr = creat("test.file", 0755);
	if (fd_wr == -1)
	{
		perror("Fail to creat:");
		return -1;
	}
	int start_tim = (int)time((time_t *)0);
	printf("start_tim = %d\n", start_tim);
	while (byte_cnt < rxlen)  //get file content
	{
		//send  request
		sprintf(&buf[4], "%d", idx);
	 	len = getpack(sock, &saddr, buf, strlen(buf), reply);
	 	int ret = write(fd_wr, reply, len);
	 	if (ret == -1)
	 	{
	 		perror("Fail to write:");
	 		printf("len = %d\n", len);
	 		break;
	 	}
		byte_cnt += len;		
		idx ++;
	}
	close(fd_wr);
	int end_tim = (int)time((time_t *)0);
	printf("end_tim = %d\n", end_tim);
	printf("byte_cnt : %d\n", byte_cnt);
	//printf("buf %s\n", reply);

	return 0;
}

//从服务器获得文件的长度
int get_file_len(int sock, struct sockaddr_in *saddr, char *pfname, struct timeval *timeout)
{
	char buffer[BUFSIZ];
	char reply[BUFSIZ];

	int max_fd, ret, rxlen;
	fd_set read_fds;
	socklen_t saddrlen;

	//CMD is ready
	strcpy(buffer, "GLEN");
	strcat(buffer, pfname);
	
	FD_ZERO(&read_fds);
	FD_SET(sock, &read_fds);
	max_fd = sock+1;
	saddrlen = sizeof(struct sockaddr_in);

	while (1)   //重复发送知道获得文件的信息或者出现错误
	{
		if (sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr*)saddr, saddrlen) == -1)
		{
			perror("Fail to sendto:");
			return -1;
		}
		ret = select(max_fd, &read_fds, NULL, NULL, timeout);
		if (ret == -1)
		{
			perror("select:");
			return -1;
		}
		if (FD_ISSET(sock, &read_fds))
		{
			if ((rxlen = recvfrom(sock, reply, BUFSIZ, 0, (struct sockaddr *)saddr, &saddrlen)) >0)
			{
				rxlen = atoi(reply);	
				return rxlen;
			}
		}
	}
	return rxlen;
}


//发送一包数据，一直等到服务器确认，或者超时 
int getpack(int sock, struct sockaddr_in *saddr, char buf[], int len, char *reply)
{
	int retry = 0;
	int ret, rxlen;
	struct timeval tv;
	struct sockaddr_in addr = *saddr;

	socklen_t saddrlen;

	fd_set read_fds;

	FD_ZERO(&read_fds);
	FD_SET(sock, &read_fds);

	saddrlen = sizeof(struct sockaddr_in);
  while (1)
  {
  	retry ++;
  	if (retry > 3)
  		break;
		if (sendto(sock, buf, len, 0, (struct sockaddr*)&addr, sizeof(struct sockaddr_in)) == -1)
		{
			perror("getpakc Fail to sendto:");		
			return -1;
		}

		FD_ZERO(&read_fds);
		FD_SET(sock, &read_fds);
		tv.tv_sec = 0;
		tv.tv_usec = 5000;

		ret = select(sock+1, &read_fds, NULL, NULL, &tv);
		if (ret == -1)
		{
			perror("Fail to select:");
			return -1;
		}
		if (FD_ISSET(sock, &read_fds))
		{
			if ((rxlen = recvfrom(sock, reply, BUFSIZ, 0, (struct sockaddr*)&saddr, &saddrlen)) > 0)
			{
				return rxlen;
/*			if (strncmp(reply, "ACK", 3) == 0)   //接受到ＡＣＫ将正确返回
			{
				return 0;
			} */
			}
		}
	}
	//
	return -1;  //超时或者出错
}

