#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include "shared_memory.h"
#include "input_process.h"
#include "main_process.h"
#include "output_process.h"

/* create IPCs and fork */
int main(void) {
	pid_t pid;

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
		shm_destroy();
	}
	return 0;
}
