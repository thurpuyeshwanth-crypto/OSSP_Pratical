CC = gcc
CFLAGS = -Wall -Wextra -g -std=c11 -D_POSIX_C_SOURCE=200809L
PROGRAMS = prog1 prog2 prog3 wait_waitpid_demo zombie_process prog5 ls_grep_pipe

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

clean:
	rm -f $(PROGRAMS) output.txt input.txt
