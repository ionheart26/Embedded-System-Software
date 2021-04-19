#include "semaphore.h"
#include <stdlib.h>
#include <errno.h>

int p(int semid, int sem_num) {
	struct sembuf pbuf;
	pbuf.sem_num = sem_num;
	pbuf.sem_op = -1;
	pbuf.sem_flg = SEM_UNDO;
	
	if(semop(semid, &pbuf, 1) == -1) exit(1);
	return 0;
}

int v(int semid, int sem_num) { 
	struct sembuf vbuf;
        vbuf.sem_num = sem_num;
        vbuf.sem_op = 1;
        vbuf.sem_flg = SEM_UNDO;

        if(semop(semid, &vbuf, 1) == -1) exit(1);
        return 0;
}
