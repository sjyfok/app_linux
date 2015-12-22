#include <stdio.h>
#include <time.h>
#include <string.h>

#define PORT   6789
const char *host = "192.168.200.3";

int process_request(int fd);

int main(void)
{
	int fd;
	fd = connect_to_server(host, PORT);
	if (fd == -1)
	{
		return -1;
	}
	talk_with_server(fd);
	close(fd);
	return 0;
}

#define	   PACK_SIZE  16384
static char testbuf[PACK_SIZE];

int talk_with_server(int fd)
{
	int last_tm  = 0;
	int byte_cnt;
	time_t cur_timsec = 0, end_timsec = 0, tmp_tim;
	int recv_byte_tol = 0;

	last_tm = 10;
	//read client cmd
	byte_cnt = write(fd, &last_tm, sizeof(int)) ;
	if (byte_cnt != sizeof(int))
	{
		//cmd error
		return -1;
	}


	while (1) 
	{
		byte_cnt = read(fd, testbuf, PACK_SIZE);
		if (strncmp(testbuf, "Stop", 4) == 0) 
		{
			printf("recv_byte_tol = %d\n", recv_byte_tol);
			break;
		}

		if (byte_cnt != PACK_SIZE)
		{
			//send error
			perror("Fail write");
		}
		recv_byte_tol += byte_cnt;
	}


	return 0;
}
