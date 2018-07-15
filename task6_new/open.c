#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


int main(int argc, char **argp)
{
	int fd;
	int data = 0;
	char buf[32] = {0};
	char ch; 

	fd = open("/dev/eudyptula", O_RDWR);
	if (fd == -1)
		printf("File open failed");
	/* This piece of code will read all the data at time. */

#if 0 
	int i = 0;
	do {
		data = read(fd, buf + i, 32-i);
		i++;
		//printf("Buffer from Dev :%s data :%d\n", buf, data);
	} while(data);
	printf("Buffer from Dev :%s\n", buf);
	/*
OUTPUT:
Buffer from Dev :b data :12
Buffer from Dev :b7 data :11
Buffer from Dev :b76 data :10
Buffer from Dev :b761 data :9
Buffer from Dev :b761d data :8
Buffer from Dev :b761d1 data :7
Buffer from Dev :b761d1a data :6
Buffer from Dev :b761d1a7 data :5
Buffer from Dev :b761d1a77 data :4
Buffer from Dev :b761d1a770 data :3
Buffer from Dev :b761d1a7700 data :2
Buffer from Dev :b761d1a77009 data :1
Buffer from Dev :b761d1a77009 data :0
Buffer from Dev :b761d1a7700
	 */
#else
	int i = 0;
	char *p = buf;

	do {
		data = read(fd,  p + i, 32-i);
		i++;
		//printf("Buffer from Dev :%s data :%d\n", buf, data);
	} while(data);
	printf("Buf :");
	printf("%s", buf);
#endif
	close(fd);
	return 0;
}
