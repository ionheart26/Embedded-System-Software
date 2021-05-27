#ifndef __DEV_DRIVER_H
#define __DEV_DRIVER_H

#include <linux/ioctl.h>

/*
 * data needed by dev_driver module 
 * app.c should pass this data to dev_driver
*/
struct dev_driver_data {
	int timer_interval;
	int timer_cnt;
	unsigned char timer_init[4];
};

#define DEV_DRIVER_NAME "/dev/dev_driver"
#define MAJOR_NUMBER 242

/* ioctl cmd */
#define CMD0 _IOW(MAJOR_NUMBER, 0, struct dev_driver_data*)
#define CMD1 _IO(MAJOR_NUMBER, 1)

#endif
