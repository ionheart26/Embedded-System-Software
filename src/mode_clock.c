#include "mode_clock.h"
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "led_mmap.h"

static unsigned int hour, min;
static unsigned char led;
static int is_flick;
static pthread_t flicker;

static int set_cur_time();
static int show_time();
static void* flicker_func(void* arg);

void mode_clock_main(struct data* input) {
	printf("mode_clock_main\n");
	struct data data;
	int sw;
	int i;
	
	for(i = 0; i < PUSH_SWITCH_MAX; i++){
		if(input->device_data.push_switch[i]) {
			sw = i + 1;
			break;
		}
	}

	
	switch(sw) {
		case 1:
			is_flick ^= 1;
			if(is_flick)
				pthread_create(&flicker, 0, flicker_func, 0);
			else if(!is_flick)
				pthread_join(flicker, NULL);	
			break;
		case 2:
			is_flick = 0;
			set_cur_time();
			show_time();
			break;
		case 3:
			if(is_flick){
				hour++;
				show_time();
			}
			break;
		case 4:
			if(is_flick){
                                min++;
                                show_time();
                        }
			break;
	}
}

void mode_clock_init() {
	printf("mode_clock_init\n");
	struct data data;

	is_flick = 0;

	set_cur_time();
	show_time();

	led = 128;
	data.device = LED;
	data.device_data.led = led;
	shm_write(IPC2, &data);
}

void mode_clock_destroy() {
	printf("mode_clock_destroy\n");
	device_clear();
}

static int set_cur_time(){
	time_t delta;
        struct tm* timeinfo;

	time(&delta);
        timeinfo = localtime(&delta);
        hour = timeinfo->tm_hour;
        min = timeinfo->tm_min;
	return 0;
}

static int show_time() {
	struct data data;
	
	data.device = FND;
	data.device_data.fnd[0] = hour / 10;
	data.device_data.fnd[1] = hour % 10;
	data.device_data.fnd[2] = min / 10;
        data.device_data.fnd[3] = min % 10;
        shm_write(IPC2, &data);

	return 0;
}

static void* flicker_func(void* arg) {
	int i;

	while(is_flick) {
		led = 128 | 32;
		led_mmap(led);

		i = 4;
		while(i-- && is_flick)
			usleep(250000);
		if(!is_flick) break;

		led = 128 | 16;
		led_mmap(led);

		i = 4;
                while(i-- && is_flick)
                        usleep(250000);
	}

	led = 128;
	led_mmap(led);

	pthread_exit(NULL);
}
