#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include "../module/dev_driver.h"

/* open, ioctl CMD0, ioctl CMD1 */
int main(int argc, char** argv){
	int fd;
	struct dev_driver_data data;
	int i;

	fd = open(DEV_DRIVER_NAME, O_WRONLY);
	if(fd < 0) {
		printf("Open error\n");
		return 1;
	}
	data.timer_interval = atoi(argv[1]);
	data.timer_cnt = atoi(argv[2]);
	for(i = 0; i < 4; i++){
		data.timer_init[i] = argv[3][i] - '0';
	}

	ioctl(fd, CMD0, &data);

	ioctl(fd, CMD1);

	close(fd);
	return 0;
}
