#ifndef __PROCESS_H
#define __PROCESS_H

#include "../lib/shared_memory.h"
#include "../lib/device.h"
#include "../lib/led_mmap.h"

struct property {
        int ev_fd, fnd_fd, push_switch_fd, dot_fd, text_lcd_fd;
} prop;

#endif
