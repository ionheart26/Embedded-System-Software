#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <unistd.h>
#include "semaphore.h"
#include "shared_memory.h"

#define PATH_MAX 100

static int shmid[2];
static int semid[2];
static struct shm* shm[2];

/* 
 * create two shared memory
 * create semephores for each
 */
int shm_init() {
	key_t shmkey[2];
	key_t semkey[2];
	int i;
	int keyid = 0;
	for(i = 0; i < 2; i++){
		char path[PATH_MAX];

		getcwd(path, sizeof(path));
		semkey[i] = ftok(path, keyid++);
	        if((semid[i] = semget(semkey[i], 2, SEM_FLAGS)) == -1) {
        	        //already exists
	                if(errno == EEXIST) {
                	        perror("sem id exists");
                        	exit(1);
                	}
        	}

		union semun arg;
        	arg.val = 0;
	        semctl(semid[i], SHM_FULL, SETVAL, arg);
		arg.val = 1;
        	semctl(semid[i], SHM_EMPTY, SETVAL, arg);
	}

	for(i = 0; i < 2; i++){
		char path[PATH_MAX];

                getcwd(path, sizeof(path));
                shmkey[i] = ftok(path, keyid++);
		shmid[i] = shmget(shmkey[i], sizeof(struct shm), SHM_FLAGS);
		shm[i] = (struct shm*)shmat(shmid[i], 0, SHM_FLAGS);
	}
	return 0;
}
	
int shm_read(int ipc, struct data* dst) {
	p(semid[ipc], SHM_FULL);
	memcpy(dst, &(shm[ipc]->data), sizeof(struct data));
	v(semid[ipc], SHM_EMPTY);
	return 0;
}
	
int shm_write(int ipc, struct data* src) {
	p(semid[ipc], SHM_EMPTY);
	memcpy(&(shm[ipc]->data), src, sizeof(struct data));
	v(semid[ipc], SHM_FULL);
	return 0;
}

int shm_destroy(void) {
	int i;
	for(i = 0; i < 2; i++){
		if(shmdt(shm[i]) == -1) {
			perror("shm delete");
			exit(1);
		}
		if(shmctl(shmid[i], IPC_RMID, 0) == -1) {
			perror("shm rmid");
			exit(1);
		}
		if(semctl(semid[i], 0, IPC_RMID, 0) == -1) {
	                perror("sem exit");
        	        exit(1);
        	}
	}
	return 0;
}
