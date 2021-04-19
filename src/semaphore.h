#ifndef __SEMAPHORE_H
#define __SEMAPHORE_H

#include <sys/sem.h>

#define SEM_FLAGS (IPC_CREAT|0660)
//#define SEM_FLAGS (IPC_CREAT|IPC_EXCL|0660)

/* shared memory */
#define SHM_SEM_FILE "/"
#define SHM_SEM_KEY_ID 0
/* sem num */
#define WRT 0
#define MUTEX 1

/* ipc */
#define IPC1_SEM_FILE "/"
#define IPC2_SEM_FILE "/"
#define IPC1_SEM_KEY_ID 1
#define IPC2_SEM_KEY_ID 2
/* sem num */
#define IPC1_EMPTY 0
#define IPC1_FULL 1
#define IPC2_EMPTY 0
#define IPC2_FULL 1

union semun {
        int val;
        struct semid_ds *buf;
        unsigned short *array;
};

/* semop */
int p(int semid, int sem_num);
int v(int semid, int sem_num); 

#endif
