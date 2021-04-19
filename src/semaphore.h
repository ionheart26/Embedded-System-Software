#ifndef __SEMAPHORE_H
#define __SEMAPHORE_H

#include <sys/sem.h>

#define SEM_FLAGS (IPC_CREAT|0660)
//#define SEM_FLAGS (IPC_CREAT|IPC_EXCL|0660)

union semun {
        int val;
        struct semid_ds *buf;
        unsigned short *array;
};

/* semop */
int p(int semid, int sem_num);
int v(int semid, int sem_num); 

#endif
