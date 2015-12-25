#include <stdio.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>

#define PORT   6789

int process_request(int fd);



int main(void)
{
	int sock, fd;

	sock = make_server_socket(PORT);
	if (sock == -1)
	{
		return -1;
	}
	while (1)
	{
		fd = accept(sock, NULL, NULL);
		if (fd == -1)
		{
			perror("Fail accept");
			break;
		}
		if (process_request(fd) >= 0) //commount ok
		{
			break;
		}
		else   //wait for right connect
		{ //none
		}
	}
	close(fd);
	return 0;
}

#define	   PACK_SIZE  16384
static char testbuf[PACK_SIZE];

int process_request(int fd)
{
	int byte_cnt;
	time_t cur_timsec = 0, end_timsec = 0, tmp_tim;
	int send_byte_tol = 0;
	int fd_read = 0;
	int byte_rd = 0;
	int tmp;
	struct stat fl_stat;

	printf("have a connect\n");
	//read client cmd

	cur_timsec = time((time_t *)0);
	printf("cur_timesec = %d\n", (int)cur_timsec);
	//end_timsec = cur_timsec + last_tm;
	//printf("end_timesec = %d\n", (int)end_timsec);
	//tmp_tim = cur_timsec;
	//memset(testbuf, '#', PACK_SIZE);  //send data ready
	fd_read = open("/root/test.tar", O_RDONLY);
	if (fd_read == -1)
	{
		perror("Fail to open file:");
		return -1;
	}

	while (1) 
	{
		byte_rd = read(fd_read, testbuf, PACK_SIZE);
		if (byte_rd == -1)
		{
			perror("Fail to read:");
			break;
		}
		if (byte_rd == 0)
		{
			printf("file is over\n");
			break;
		}
//		printf("%s\n", testbuf);
		byte_cnt = write(fd, testbuf, byte_rd);
		tmp = 0;
		while (byte_cnt <  byte_rd)
		{	
			tmp += byte_cnt;
			usleep(1);
			//NOP();
		
			byte_rd = byte_rd - byte_cnt;
			byte_cnt = write(fd, &testbuf[tmp], byte_rd);
			if (byte_cnt < 0)
			{
				perror("Fail to write:");
				break;
			}
		}
		send_byte_tol += byte_cnt;
	//	cur_timsec = time((time_t*)0);
	
		//if (cur_timsec-tmp_tim > 1)  //for 1 sec
		//{
			//output 
		//	printf("send bytes %d\n", send_byte_tol);
		//	tmp_tim = cur_timsec;
		//	printf("##cur_timsec = %d\n", (int)cur_timsec);
		//}
		//if (cur_timsec >= end_timsec)
	//	{
		//	printf("cur_timsec = %d\n", (int)cur_timsec);
		//	break;
	//	}
	//	usleep(1);
	}
	cur_timsec = time((time_t*)0);
	printf("endtime: %d\n", (int)cur_timsec);
	printf("Send_byte_tol = %d\n", send_byte_tol);
	close(fd_read);
	return 0;
}

