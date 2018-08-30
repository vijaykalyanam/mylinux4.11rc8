#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>

int main(void)
{

	int fd;

	fd = open("/dev/eudyptula", O_RDWR);
	printf("fd : %d\n", fd);
	char str[] = "VIJAY";
	if (fd != -1)
		ioctl(fd, 1, str);
	return 0;
}
