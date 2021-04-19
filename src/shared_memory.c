#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include "semaphore.h"
#include "shared_memory.h"

static int shmid, semid;
static struct shm* shm;

/* initialize semaphores
 * create shared memory
 */
int shm_init() {
	key_t shmkey, semkey;

	semkey = ftok(SHM_SEM_FILE, SHM_SEM_KEY_ID);
        if((semid = semget(semkey, 2, SEM_FLAGS)) == -1) {
                //already exists
                if(errno == EEXIST) {
                        perror("sem id exists");
                        exit(1);
                }
        }
	union semun arg;
	arg.val = 1;
	semctl(semid, WRT, SETVAL, arg);
	semctl(semid, MUTEX, SETVAL, arg);

	shmkey = ftok(SHM_FILE, SHM_ID);
	shmid = shmget(shmkey, sizeof(struct shm), SHM_FLAGS);
	shm = (struct shm*)shmat(shmid, 0, SHM_FLAGS);
	shm->read_count = 0;
	return 0;
}
	
int shm_read(struct data* dst) {
	p(semid, MUTEX);
	shm->read_count++;
	if(shm->read_count == 1) p(semid, WRT);
	v(semid, MUTEX);
	memcpy(dst, &shm->data, sizeof(struct data));
	p(semid, MUTEX);
	shm->read_count--;
	if(shm->read_count == 0) v(semid, WRT);
	v(semid, MUTEX);
	return 0;
}
	
int shm_write(struct data* src) {
	p(semid, WRT);
	memcpy(&shm->data, src, sizeof(struct data));
	v(semid, WRT);
	return 0;
}

int shm_destroy(void) {
	if(shmdt(shm) == -1) {
		perror("shm delete");
		exit(1);
	}
	if(shmctl(shmid, IPC_RMID, 0) == -1) {
		perror("shm rmid");
		exit(1);
	}

	if(semctl(semid, 0, IPC_RMID, 0) == -1) {
                perror("sem exit");
                exit(1);
        }
	return 0;
}
