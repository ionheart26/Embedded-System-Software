#ifndef __PROCESS_H
#define __PROCESS_H

#include "shared_memory.h"
#include "device.h"
#include "led_mmap.h"

struct property {
        int ev_fd, fnd_fd, push_switch_fd, dot_fd, text_lcd_fd;
} prop;

#endif
