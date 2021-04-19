#include "output_process.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

/* for global use */
static struct output_property op;

static int init();
static int destroy();

int output_process(struct property* prop) {
	printf("output process: start.\n");
	struct data data;
	int is_running = 1;

	while(is_running) {
		p(prop->semid_ipc2, IPC2_FULL);
		shm_read(&data);
		
		switch(data.device){
			case READKEY:
				printf("output : readkey[%d]\n", data.device_data.readkey);
				if(data.device_data.readkey == BACK)
                                	is_running = 0;
				break;
			case DOT:
                                printf("output: dot\n");
                          //      write(op.dot_fd, data.device_data.dot, sizeof(data.device_data.dot));
                                break;
			case FND:
				printf("output: fnd\n");
			//	write(op.fnd_fd, data.device_data.fnd, sizeof(data.device_data.fnd));
				break;
			case PUSH_SWITCH:
                                printf("output: push switch\n");
                        //      write(op.fnd_fd, data.device_data.fnd, sizeof(data.device_data.fnd));  
                                break;
			case TEXT_LCD:
                                printf("output: text_lcd\n");
                         //       write(op.text_lcd_fd, data.device_data.text_lcd, sizeof(data.device_data.text_lcd));
                                break;
		}

		v(prop->semid_ipc2, IPC2_EMPTY);
	}
	printf("output: exit\n");
	return 0;
}

/* open device files */
static int init() {
        op.ev_fd = open(EVENT_DEVICE, O_RDWR | O_NONBLOCK);
        op.push_switch_fd = open(PUSH_SWITCH_DEVICE, O_RDWR | O_NONBLOCK);
        op.fnd_fd = open(FND_DEVICE, O_RDWR);
        op.led_fd = open(LED_DEVICE, O_RDWR);
        op.dot_fd = open(FPGA_DOT_DEVICE, O_WRONLY);
        op.lcd_fd = open(FPGA_TEXT_LCD_DEVICE, O_WRONLY);
        return 0;
}

/* close device files */
static int destroy() {
        close(op.ev_fd);
        close(op.push_switch_fd);
        close(op.fnd_fd);
        close(op.led_fd);
        close(op.dot_fd);
        close(op.lcd_fd);
        return 0;
}
