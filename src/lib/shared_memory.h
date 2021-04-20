#ifndef __SHARED_MEMORY_H
#define __SHARED_MEMORY_H

#include "device.h"

#define SHM_FLAGS (IPC_CREAT)

/* 
 * IPC index
 * Use this index to use shm
 */
#define IPC1 0
#define IPC2 1

/* sem num */
#define SHM_FULL 0
#define SHM_EMPTY 1
		
struct data {
	int device;
	union device_data device_data; 
};

struct shm {
        struct data data;
};

/* called in main.c before fork */
int shm_init(void);

/* synchronized */
int shm_read(int ipc, struct data* dst);
int shm_write(int ipc, struct data* src);

int shm_destroy(void);

#endif
