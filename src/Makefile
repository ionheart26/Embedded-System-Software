EXE = 20161562

CC = arm-none-linux-gnueabi-gcc
#CC = gcc
CFLAGS = -static -pthread -Wall

$(EXE): main.o input_process.o main_process.o output_process.o shared_memory.o semaphore.o mode_clock.o mode_counter.o mode_text_editor.o mode_draw_board.o led_mmap.o
	$(CC) $(CFLAGS) -o $@ main.o input_process.o main_process.o output_process.o shared_memory.o semaphore.o mode_clock.o mode_counter.o mode_text_editor.o mode_draw_board.o led_mmap.o

main.o: main.c
	$(CC) -o $@ -c main.c

input_process.o: input_process.c input_process.h process.h
	$(CC) -o $@ -c input_process.c

main_process.o: main_process.c main_process.h process.h
	$(CC) -o $@ -c main_process.c

output_process.o: output_process.c output_process.h process.h
	$(CC) -o $@ -c output_process.c

shared_memory.o: shared_memory.c shared_memory.h semaphore.h semaphore.c
	$(CC) -o $@ -c shared_memory.c

semaphore.o: semaphore.c
	$(CC) -o $@ -c semaphore.c

mode_clock.o: mode_clock.c mode_clock.h mode.h
	$(CC) -o $@ -c mode_clock.c

mode_counter.o: mode_counter.c mode_counter.h mode.h
	$(CC) -o $@ -c mode_counter.c

mode_text_editor.o: mode_text_editor.c mode_text_editor.h mode.h
	$(CC) -o $@ -c mode_text_editor.c

mode_draw_board.o: mode_draw_board.c mode_draw_board.h mode.h
	$(CC) -o $@ -c mode_draw_board.c

led_mmap.o : led_mmap.c led_mmap.h
	$(CC) -o $@ -c led_mmap.c

clean:
	rm *.o $(EXE)
