#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include "shared_memory.h"
#include "input_process.h"
#include "main_process.h"
#include "output_process.h"

static int fd_init();
static int fd_destroy();

/* 
 * create IPCs
 * open device driver
 * fork
 */
int main(void) {
	pid_t pid;

	fd_init();
	shm_init();
	
	//input process
	if((pid = fork()) == 0) {
		input_process();
		exit(0);
	}
	//output process
        if((pid = fork()) == 0) {
                output_process();
                exit(0);
        }
	//main process
	else {
		main_process();

		/* Exit */
		wait();
		wait();
		fd_destroy();
		shm_destroy();
	}
	return 0;
}

static int fd_init() {
	prop.ev_fd = open(EVENT_DEVICE, O_RDWR | O_NONBLOCK);
        prop.push_switch_fd = open(PUSH_SWITCH_DEVICE, O_RDWR | O_NONBLOCK);
        prop.fnd_fd = open(FND_DEVICE, O_RDWR);
        prop.dot_fd = open(FPGA_DOT_DEVICE, O_WRONLY);
        prop.text_lcd_fd = open(FPGA_TEXT_LCD_DEVICE, O_WRONLY);
	return 0;
}

static int fd_destroy() {
	close(prop.ev_fd);
        close(prop.push_switch_fd);
        close(prop.fnd_fd);
        close(prop.dot_fd);
        close(prop.text_lcd_fd);
        return 0;
}
