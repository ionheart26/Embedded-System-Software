#include "input_process.h"
#include <stdio.h>
#include <unistd.h>
#include <linux/input.h>
#include <string.h>
#include "../lib/device.h"

int input_process() {
	printf("Input process: start\n");
	struct data data;
	struct input_event ev_buf[EV_BUF_SIZE];
	int ev_size = sizeof(struct input_event);
	unsigned char push_sw_buf[PUSH_SWITCH_MAX];
	int is_running = 1;

	memset(push_sw_buf, 0, sizeof(push_sw_buf));

	while(is_running) {
		if(read(prop.ev_fd, ev_buf, ev_size * EV_BUF_SIZE) >= ev_size) {
			if(ev_buf[0].value == KEY_RELEASE)
				continue;

			int code = ev_buf[0].code;

			if(code == BACK)
				is_running = 0;
			printf("====Input process: write readkey\n");
			data.device = READKEY;
			data.device_data.readkey = ev_buf[0].code;
	     	  	shm_write(IPC1, &data);
		}

		read(prop.push_switch_fd, &push_sw_buf, sizeof(push_sw_buf));
		int i;
		int is_push = 0;
		for(i = 0; i < PUSH_SWITCH_MAX; i++) {
			if(push_sw_buf[i]) {
				is_push = 1;
				break;
			}
		}
		if(is_push) {
			printf("===Input process: write switch\n");
                        data.device = PUSH_SWITCH;
                        memcpy(data.device_data.push_switch, push_sw_buf, sizeof(push_sw_buf));
                        shm_write(IPC1, &data);
			usleep(200000);
		}
	}

	printf("Input process: exit\n");
	return 0;
}
