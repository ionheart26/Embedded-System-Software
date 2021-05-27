#include <linux/kernel.h>
#include <asm/io.h>
#include "submodule.h"
#include "fpga_dot_font.h"
	
/* Use ioremap to control devices */
static struct struct_addr{
	unsigned char *fnd, *led, *dot, *text_lcd;
} addr;

/* data used to update devices */
static struct struct_data{
	unsigned char fnd[4];
	unsigned char led;
	unsigned char dot[10];
	unsigned char text_lcd[32];

	int pat; /* pattern */
        int loc; /* location */
        int rotation_cnt; /* 0 - 8 */

	/* text lcd */
	int top_len, bottom_len; /* string length */
	int top_cnt, bottom_cnt; /* remaining cnt to shift */
	int top_direction, bottom_direction; /* left 0 right 1 */
} data;

static int submodule_write(void);
static int shift_text_right(unsigned char*);
static int shift_text_left(unsigned char*);
static int shift_text(unsigned char*, int);

int submodule_ioremap(){
	addr.fnd = ioremap(FND_ADDRESS, 0x32);
	addr.led = ioremap(LED_ADDRESS, 0x1);
	addr.dot = ioremap(DOT_ADDRESS, 0x10);
	addr.text_lcd = ioremap(TEXT_LCD_ADDRESS, 0x32);
	return 0;
}

int submodule_iounmap(){
	iounmap(addr.fnd);
	iounmap(addr.led);
	iounmap(addr.dot);
	iounmap(addr.text_lcd);
	return 0;
}

int submodule_update(){
	//fnd
	data.pat++;
	if(data.pat > 8) data.pat = 1;

	data.rotation_cnt++;
	if(data.rotation_cnt >= 8){
		data.rotation_cnt = 0;
		data.loc = (data.loc + 1) % 4;
	}

	memset(data.fnd, 0, sizeof(data.fnd));
	data.fnd[data.loc] = data.pat;

	//led
	data.led >>= 1;
	if(data.led == 0) data.led = 0b10000000;

	//dot
	memcpy(data.dot, &fpga_number[data.pat], sizeof(data.dot));

	//text_lcd
	if(data.top_cnt == 0){
		data.top_cnt = 16 - data.top_len;
		data.top_direction ^= 1;
	}
	if(data.bottom_cnt == 0){
                data.bottom_cnt = 16 - data.bottom_len;
                data.bottom_direction ^= 1;
        }

	shift_text(data.text_lcd, data.top_direction);
	shift_text(data.text_lcd + 16, data.bottom_direction);

	data.top_cnt--;
	data.bottom_cnt--;

	submodule_write();
	return 0;
}
	
int submodule_init(char* _data) {
	int i;

	//fnd
	memcpy(data.fnd, _data, sizeof(data.fnd));
	for(i = 0; i < 4; i++){
		if(data.fnd[i] != 0){
			data.pat = data.fnd[i];
			data.loc = i;
			break;
		}
	}
	data.rotation_cnt = 0;

	//led
	data.led = 0b10000000;
	data.led >>= (data.pat-1);

	//dot
	memcpy(data.dot, &fpga_number[data.pat], sizeof(data.dot));

	//text_lcd
	data.top_len = strlen(STUDENT_NUMBER);
	data.bottom_len = strlen(STUDENT_NAME);
	data.top_cnt = 16 - data.top_len;
	data.bottom_cnt = 16 - data.bottom_len;
	data.top_direction = 1;
	data.bottom_direction = 1;
	memset(data.text_lcd, ' ', 32);
	strncpy(data.text_lcd, STUDENT_NUMBER, data.top_len);
	strncpy(data.text_lcd+16, STUDENT_NAME, data.bottom_len);

	submodule_write();
	return 0;
}

int submodule_clear(){
	memset(data.fnd, 0, sizeof(data.fnd));
	data.led = 0;
	memset(data.dot, 0, sizeof(data.dot));
	memset(data.text_lcd, 0, sizeof(data.text_lcd));

	submodule_write();
	return 0;
}

static int 
submodule_write(){
	unsigned short s = 0;
	int i;

	//fnd
	s = (data.fnd[0] << 12) | (data.fnd[1] << 8) | (data.fnd[2] << 4) | data.fnd[3];
	memcpy(addr.fnd, &s, sizeof(s));

	//led
	*(addr.led) = data.led;

	//dot
	for(i = 0; i < 10; i++){
      		s = data.dot[i] & 0x7F;
      		memcpy(addr.dot + i*2, &s, sizeof(s));
    	}

	//text_lcd
	for(i = 0; i < 32; i++){
		s = (data.text_lcd[i] & 0xFF) << 8 | (data.text_lcd[i + 1] & 0xFF);
      		memcpy(addr.text_lcd + i, &s, sizeof(s));
		i++;
	}
	return 0;
}

static int
shift_text_right(unsigned char* str){
	int i;
	for (i = 15; i > 0; i--) {
		str[i] = str[i - 1];
	}
	str[0] = ' ';
	return 0;
}

static int
shift_text_left(unsigned char* str){
	int i;
	for(i = 0; i < 15; i++){
		str[i] = str[i + 1];
	}
	str[15] = ' ';
	return 0;
}

static int
shift_text(unsigned char* str, int direction){
	if(direction)
		shift_text_right(str);
	else
		shift_text_left(str);
	return 0;
}
