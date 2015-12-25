#include <stdio.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>

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

#define	   PACK_SIZE  4096//16384
static char testbuf[PACK_SIZE];

int talk_with_server(int fd)
{
	int byte_cnt;
	time_t cur_timsec = 0, end_timsec = 0, tmp_tim;
	int recv_byte_tol = 0;
	int fd_wr = 0, byte_wr = 0;

	fd_wr = creat("test.tar.gz", 0755);
	if (fd_wr == -1)
	{
		perror("Fail to open:");
		return -1;
	}
	//read client cmd
/*	byte_cnt = write(fd, &last_tm, sizeof(int)) ;
	if (byte_cnt != sizeof(int))
	{
		//cmd error
		return -1;
	}
*/

	while (recv_byte_tol < 1687941120)// 106754626) 
	{
		byte_cnt = read(fd, testbuf, PACK_SIZE);
		if (byte_cnt == -1)
		{
			perror("Fail to read:");
			break;
		}
		byte_wr = write(fd_wr, testbuf, byte_cnt) ;
		if (byte_wr == -1)
		{
			//send error
			perror("Fail write");
			break;
		} 
		recv_byte_tol += byte_cnt;
	}

	close(fd_wr);
	return 0;
}
