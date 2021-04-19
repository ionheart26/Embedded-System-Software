#ifndef __INPUT_PROCESS_H
#define __INPUT_PROCESS_H

#include "process.h"

#define EV_BUF_SIZE 16

/* only for global use in c file */
struct input_property {
        int ev_fd, fnd_fd, led_fd, push_switch_fd, dot_fd, lcd_fd;
};

int input_process(struct property*);

#endif
