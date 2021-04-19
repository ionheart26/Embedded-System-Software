#ifndef __DEVICE_H
#define __DEVICE_H

/* device file */
#define EVENT_DEVICE	      "/dev/input/event0"
#define FND_DEVICE	      "/dev/fpga_fnd"
#define LED_DEVICE	      "/dev/fpga_led"
#define PUSH_SWITCH_DEVICE    "/dev/fpga_push_switch"
#define FPGA_DOT_DEVICE       "/dev/fpga_dot"
#define FPGA_TEXT_LCD_DEVICE  "/dev/fpga_text_lcd"

#define KEY_RELEASE 0
#define KEY_PRESS 1 

/* Readkey code */
#define BACK 158
#define VOL_P 115
#define VOL_M 114

/* device */
#define BUZZER 0
#define DIP_SWITCH 1
#define DOT 2
#define FND 3
#define LED 4
#define PUSH_SWITCH 5
#define STEP_MOTOR 6
#define TEXT_LCD 7

#define READKEY 8

/* device data size */
#define DOT_MAX 10
#define FND_MAX 4
#define PUSH_SWITCH_MAX 9
#define TEXT_LCD_MAX 32

/* readkey & fpga devices */
union device_data {
                unsigned short readkey; //code
                //buzzer
                //dip switch
                unsigned char dot[DOT_MAX];
                unsigned char fnd[FND_MAX];
                unsigned char led;
                unsigned char push_switch[PUSH_SWITCH_MAX];
                //motor
                unsigned char text_lcd[TEXT_LCD_MAX];
};

#endif
