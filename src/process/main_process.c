#include "main_process.h"
#include "../mode/mode_clock.h"
#include "../mode/mode_counter.h"
#include "../mode/mode_text_editor.h"
#include "../mode/mode_draw_board.h"
#include <stdio.h>
#include <unistd.h>

static mode_init_func mode_init[4];
static mode_main_func mode_main[4];
static mode_destroy_func mode_destroy[4];

static int set_mode_init();
static int set_mode_main();
static int set_mode_destroy();

/* change mode, control semaphore, do not update shared memory 
 * 
 */
int main_process() {
	printf("Main process: start\n");
	int mode = 0;
	struct data data;
	int is_running = 1;

	set_mode_init();
	set_mode_main();
	set_mode_destroy();
	
	mode_init[mode]();
	while(is_running) {
		shm_read(IPC1, &data);
		if(data.device == READKEY) {
			int code = data.device_data.readkey;
			printf("Main : readkey[%d]\n", code);

			switch(data.device_data.readkey) {
				case BACK:
					is_running = 0;
					break;
				case VOL_P:
					mode_destroy[mode]();
					mode = (mode+1)%4;
					mode_init[mode]();
					break;
				case VOL_M:
					mode_destroy[mode]();
                                        mode = (mode+3)%4;
                                        mode_init[mode]();
					break;
			}
			
		}
		else if(data.device == PUSH_SWITCH) {
			printf("Main : switch\n");
			mode_main[mode](&data);
		}

	}
	
	shm_write(IPC2, &data);
	printf("main : exit\n");
	return 0;
}

int set_mode_init(){
	mode_init[0] = mode_clock_init;
	mode_init[1] = mode_counter_init;
	mode_init[2] = mode_text_editor_init;
	mode_init[3] = mode_draw_board_init;
	return 0;
}

int set_mode_main(){
        mode_main[0] = mode_clock_main;
        mode_main[1] = mode_counter_main;
        mode_main[2] = mode_text_editor_main;
        mode_main[3] = mode_draw_board_main;
        return 0;
}

int set_mode_destroy(){
        mode_destroy[0] = mode_clock_destroy;
        mode_destroy[1] = mode_counter_destroy;
        mode_destroy[2] = mode_text_editor_destroy;
        mode_destroy[3] = mode_draw_board_destroy;
        return 0;
}
