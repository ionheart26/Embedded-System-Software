#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include "../module/stopwatch.h"

int main(void){
	int fd;
	fd = open(DEVICE_FILE, O_RDWR);
	if(fd < 0) {
		printf("Open error\n");
		return 1;
	}
	close(fd);
	printf("app exiting...\n");
	return 0;
}
