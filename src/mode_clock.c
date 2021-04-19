#include <time.h>
#include <stdio.h>

static unsigned int cur_hour, cur_min;

void mode_clock_main() {
	printf("mode_clock_main\n");
}

void mode_clock_init() {
	printf("mode_clock_init\n");
	time_t delta;
        struct tm* timeinfo;

        time(&delta);
        timeinfo = localtime(&delta);
	cur_hour = timeinfo->tm_hour;
	cur_min = timeinfo->tm_min;
	
}

void mode_clock_destroy() {
	printf("mode_clock_destroy\n");
}
