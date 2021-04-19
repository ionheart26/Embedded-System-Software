#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include "shared_memory.h"
#include "input_process.h"
#include "main_process.h"
#include "output_process.h"
#include "property.h"
#include "semaphore.h"

/* create IPCs and fork */
int main(void) {
	pid_t pid;
	key_t semkey_ipc1, semkey_ipc2;
	struct property prop;
	union semun arg;
	semkey_ipc1 = ftok(IPC1_SEM_FILE, IPC1_SEM_KEY_ID);
	prop.semid_ipc1 = semget(semkey_ipc1, 2, SEM_FLAGS);
	arg.val = 1;
	semctl(prop.semid_ipc1, IPC1_EMPTY, SETVAL, arg);
	arg.val = 0;
        semctl(prop.semid_ipc1, IPC1_FULL, SETVAL, arg);

	semkey_ipc2 = ftok(IPC2_SEM_FILE, IPC2_SEM_KEY_ID);
        prop.semid_ipc2 = semget(semkey_ipc2, 2, SEM_FLAGS);
        arg.val = 1;
        semctl(prop.semid_ipc2, IPC2_EMPTY, SETVAL, arg);
        arg.val = 0;
        semctl(prop.semid_ipc2, IPC2_FULL, SETVAL, arg);
	
	shm_init();
	
	//input process
	if((pid = fork()) == 0) {
		input_process(&prop);
		exit(0);
	}
	//output process
        if((pid = fork()) == 0) {
                output_process(&prop);
                exit(0);
        }
	//main process
	else {
		main_process(&prop);

		/* Exit */
		wait();
		wait();
		semctl(prop.semid_ipc1, 0, IPC_RMID, 0);
		semctl(prop.semid_ipc2, 0, IPC_RMID, 0);
		shm_destroy();
	}
	return 0;
}
