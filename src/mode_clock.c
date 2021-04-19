#include <time.h>
#include <stdio.h>

static unsigned int hour, min;
static unsigned char led;

void mode_clock_main() {
	printf("mode_clock_main\n");
}

void mode_clock_init() {
	printf("mode_clock_init\n");
	time_t delta;
        struct tm* timeinfo;

        time(&delta);
        timeinfo = localtime(&delta);
	hour = timeinfo->tm_hour;
	min = timeinfo->tm_min;
	
	led = 128;
}

void mode_clock_destroy() {
	printf("mode_clock_destroy\n");
}
