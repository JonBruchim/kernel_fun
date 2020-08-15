#include <sys/ioctl.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#define MY_IOCTL _IOW(0xFF, 0xFF, int32_t*)

int main(int argc, char const *argv[])
{
	int fd, ioctl_ret = 0;

	printf("try ioctl\n");

	fd = open("/dev/my_misc", O_RDWR);

	if (fd < 0) {
		printf("cannot open file\n");
		return 0;
	}

	ioctl(fd, MY_IOCTL, &ioctl_ret);

	printf("ioctl value: %d\n",ioctl_ret);

	return 0;
}
