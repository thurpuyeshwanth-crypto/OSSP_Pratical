# OSSP Practical Source Code

This directory contains the C source programs and Makefile for all Operating Systems and Systems Programming (OSSP) practical sessions.

## Programs Directory

| Session | Source Files | Output Binary | Description |
|---|---|---|---|
| **Practical 1** | `prog1.c` | `prog1` | Runs a system command with `fork()` and `execvp()` |
| **Practical 2** | `prog2.c` | `prog2` | Copies a file byte-by-byte using low-level system calls |
| **Practical 3** | `prog3.c` | `prog3` | Inspects parent and child process IDs and states |
| **Practical 4** | `wait_waitpid_demo.c`<br>`zombie_process.c` | `wait_waitpid_demo`<br>`zombie_process` | Demonstrates `wait()` vs `waitpid()`, creates and reaps zombies |
| **Practical 5** | `prog5.c`<br>`ls_grep_pipe.c` | `prog5`<br>`ls_grep_pipe` | Producer-consumer anonymous pipe, connects `ls` and `grep` |
| **Practical 6** | `prog6_fifo_server.c`<br>`prog6_fifo_client.c`<br>`signal_handler.c` | `prog6_fifo_server`<br>`prog6_fifo_client`<br>`signal_handler` | Client-server FIFO communication; POSIX signal handling with `sigaction()` |
| **Practical 7** | `memory_layout.c`<br>`memory_demo.c` | `memory_layout`<br>`memory_demo` | Prints process segment addresses; holds process open for `/proc` inspection |
| **Practical 8** | `dynamic_memory.c`<br>`cow_demo.c` | `dynamic_memory`<br>`cow_demo` | Safe dynamic memory allocation; Copy-on-Write page sharing demo |
| **Practical 9** | `copy_lowlevel.c`<br>`copy_stdio.c`<br>`redirect_output.c`<br>`redirect_input.c` | `copy_lowlevel`<br>`copy_stdio`<br>`redirect_output`<br>`redirect_input` | Low-level vs stdio file copying; `dup2()` stream redirection |
| **Practical 10** | `mmap_file.c`<br>`read_write_file.c` | `mmap_file`<br>`read_write_file` | Memory-mapped file I/O (`mmap`) vs traditional `read()`/`write()` |
| **Practical 11** | `race_counter.c`<br>`safe_counter.c`<br>`mutex_counter.c` | `race_counter`<br>`safe_counter`<br>`mutex_counter` | POSIX threads race condition and mutex synchronization |

## Build Instructions

Compile all programs:
```bash
make
```

Compile a specific program:
```bash
make mmap_file
make race_counter
```

Clean build artifacts and temporary data files:
```bash
make clean
```
