#ifndef __SHARED_MEMORY_H
#define __SHARED_MEMORY_H

#include "device.h"

#define SHM_FILE "shm_file"
#define SHM_ID 1
#define SHM_FLAGS (IPC_CREAT)
		
struct data {
	int device;
	union device_data device_data; 
};

struct shm {
        int read_count;
        struct data data;
};

/* called in main.c before fork */
int shm_init(void);

/* synchronized */
int shm_read(struct data* dst);
int shm_write(struct data* src);

int shm_destroy(void);

#endif
