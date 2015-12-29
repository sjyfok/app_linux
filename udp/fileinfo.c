/*              文件操作
*/
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>


//获得文件长度
int getfilelen(char *pathname)
{
	struct stat fl_stat;
	int len;

	if (stat(pathname, &fl_stat) == -1)
	{
		perror("Fail stat:");
		return -1;
	}
	len = fl_stat.st_size;

	return len;
}

//打开文件
int openrdfile(char *pathname)
{
	int fd;
	if (pathname != NULL)
	{
		if ((fd = open(pathname, O_RDONLY)) == -1)
		{
			perror("Fail to open:");
			return -1;
		}
		return fd;
	}
	else 
	{
		printf("File name error!\n");
		return -1;
	}
}

//读取文件
int readfile(int fd, char buf[], int readcnt)
{
	if (fd < 0)
	{
		printf("FD error\n");
		return -1;
	}
	
	int ret = 0;
	ret = read(fd, buf, readcnt);
	if (ret == -1)
	{
		perror("Fail to read:");
	}
	return ret;
}
