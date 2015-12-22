#include <stdio.h>
#include <time.h>
#include <string.h>

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
	int last_tm  = 0;
	int byte_cnt;
	time_t cur_timsec = 0, end_timsec = 0, tmp_tim;
	int send_byte_tol = 0;

	printf("have a connect\n");
	//read client cmd
	byte_cnt = read(fd, &last_tm, sizeof(int)) ;
	printf("last_tm = %d\n", last_tm);
	if (byte_cnt != sizeof(int))
	{
		//cmd error
		return -1;
	}

	cur_timsec = time((time_t *)0);
	printf("cur_timesec = %d\n", (int)cur_timsec);
	end_timsec = cur_timsec + last_tm;
	printf("end_timesec = %d\n", (int)end_timsec);
	tmp_tim = cur_timsec;
	memset(testbuf, '#', PACK_SIZE);  //send data ready


	while (1) 
	{
		byte_cnt = write(fd, testbuf, PACK_SIZE);
		if (byte_cnt != PACK_SIZE)
		{
			//send error
			perror("Fail write");
		}
		send_byte_tol += byte_cnt;
		cur_timsec = time((time_t*)0);
	
		//if (cur_timsec-tmp_tim > 1)  //for 1 sec
		//{
			//output 
		//	printf("send bytes %d\n", send_byte_tol);
		//	tmp_tim = cur_timsec;
		//	printf("##cur_timsec = %d\n", (int)cur_timsec);
		//}
		if (cur_timsec >= end_timsec)
		{
		//	printf("cur_timsec = %d\n", (int)cur_timsec);
			break;
		}
	//	usleep(1);
	}
	printf("Stop %d\n", send_byte_tol);

	strcpy(testbuf, "Stop");
	while (strlen(testbuf) != write(fd, testbuf, strlen(testbuf)));

	return 0;
}
