#include "output_process.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int output_process() {
	printf("output process: start.\n");
	struct data data;
	int is_running = 1;

	while(is_running) {
		shm_read(IPC2, &data);
		
		switch(data.device){
			case READKEY:
				printf("output : readkey[%d]\n", data.device_data.readkey);
				if(data.device_data.readkey == BACK)
                                	is_running = 0;
				break;
			case DOT:
                                printf("output: dot\n");
                                write(prop.dot_fd, data.device_data.dot, sizeof(data.device_data.dot));
                                break;
			case FND:
				printf("output: fnd\n");
				write(prop.fnd_fd, data.device_data.fnd, sizeof(data.device_data.fnd));
				break;
			case LED:
				printf("output: led\n");
				led_mmap(data.device_data.led);
				break;
			case TEXT_LCD:
                                printf("output: text_lcd\n");
                                write(prop.text_lcd_fd, data.device_data.text_lcd, sizeof(data.device_data.text_lcd));
                                break;
		}
	}

	device_clear();
	printf("output: exit\n");
	return 0;
}

int device_clear() {
        struct data data;

        memset(&data.device_data, 0, sizeof(data.device_data));
	write(prop.fnd_fd, data.device_data.fnd, sizeof(data.device_data.fnd));
	led_mmap(0);
	write(prop.push_switch_fd, data.device_data.push_switch, sizeof(data.device_data.push_switch));
	write(prop.dot_fd, data.device_data.dot, sizeof(data.device_data.dot));
        memset(data.device_data.text_lcd, ' ', sizeof(data.device_data.text_lcd));
        return 0;
}
