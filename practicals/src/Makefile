CC = gcc
CFLAGS = -Wall -Wextra -g -std=c11 -D_POSIX_C_SOURCE=200809L
PROGRAMS = prog1 prog2 prog3 wait_waitpid_demo zombie_process prog5 ls_grep_pipe \
           prog6_fifo_server prog6_fifo_client signal_handler \
           memory_layout memory_demo dynamic_memory cow_demo \
           copy_lowlevel copy_stdio redirect_output redirect_input

.PHONY: all clean
all: $(PROGRAMS)

prog1: prog1.c
	$(CC) $(CFLAGS) $< -o $@
prog2: prog2.c
	$(CC) $(CFLAGS) $< -o $@
prog3: prog3.c
	$(CC) $(CFLAGS) $< -o $@
wait_waitpid_demo: wait_waitpid_demo.c
	$(CC) $(CFLAGS) $< -o $@
zombie_process: zombie_process.c
	$(CC) $(CFLAGS) $< -o $@
prog5: prog5.c
	$(CC) $(CFLAGS) $< -o $@
ls_grep_pipe: ls_grep_pipe.c
	$(CC) $(CFLAGS) $< -o $@
prog6_fifo_server: prog6_fifo_server.c
	$(CC) $(CFLAGS) $< -o $@
prog6_fifo_client: prog6_fifo_client.c
	$(CC) $(CFLAGS) $< -o $@
fifo_server: prog6_fifo_server.c
	$(CC) $(CFLAGS) $< -o $@
fifo_client: prog6_fifo_client.c
	$(CC) $(CFLAGS) $< -o $@
signal_handler: signal_handler.c
	$(CC) $(CFLAGS) $< -o $@
memory_layout: memory_layout.c
	$(CC) $(CFLAGS) $< -o $@
memory_demo: memory_demo.c
	$(CC) $(CFLAGS) $< -o $@
dynamic_memory: dynamic_memory.c
	$(CC) $(CFLAGS) $< -o $@
cow_demo: cow_demo.c
	$(CC) $(CFLAGS) $< -o $@
copy_lowlevel: copy_lowlevel.c
	$(CC) $(CFLAGS) $< -o $@
copy_stdio: copy_stdio.c
	$(CC) $(CFLAGS) $< -o $@
redirect_output: redirect_output.c
	$(CC) $(CFLAGS) $< -o $@
redirect_input: redirect_input.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(PROGRAMS) fifo_server fifo_client output.txt input.txt output_low.dat output_stdio.dat
