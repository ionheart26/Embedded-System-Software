#include <asm/io.h>

#define FND_ADDRESS 0x08000004 // FND physical address

unsigned char *fnd_addr;

void fnd_write(const unsigned char* data){
	unsigned short s = 0;
	s = (data[0] << 12) | (data[1] << 8) | (data[2] << 4) | data[3];
	memcpy(fnd_addr, &s, sizeof(s));
}

void fnd_init(void){
	fnd_addr = ioremap(FND_ADDRESS, 0x4);
}

void fnd_exit(void){
	iounmap(fnd_addr);
}

void fnd_reset(void){
	unsigned short s = 0;
        memcpy(fnd_addr, &s, sizeof(s));
}
