#ifndef __MODE_H
#define __MODE_H

#include <stdio.h>
#include "shared_memory.h"
#include "output_process.h"

typedef void (*mode_init_func) (void);
typedef void (*mode_main_func) (struct data*);
typedef void (*mode_destroy_func) (void);

#endif
