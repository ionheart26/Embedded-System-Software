#ifndef __OUTPUT_PROCESS_H
#define __OUTPUT_PROCESS_H

#include "process.h"

/* only for global use in c file */
struct output_property {
        int ev_fd, fnd_fd, led_fd, push_switch_fd, dot_fd, lcd_fd;
};

int output_process();

#endif
