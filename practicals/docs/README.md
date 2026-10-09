# OSSP Practical Documentation

This directory contains detailed guides, conceptual explanations, commands, observations, and benchmarks for each Operating Systems and Systems Programming (OSSP) practical session.

## Documentation Index

| Session | File | Topic & Key Concepts |
|---|---|---|
| **Practical 1** | [practical1.md](file:///c:/Users/dell9/OneDrive/Desktop/OSSP/practicals/docs/practical1.md) | Command execution using `fork()` and `execvp()`; system and hardware inspection |
| **Practical 2** | [practical2.md](file:///c:/Users/dell9/OneDrive/Desktop/OSSP/practicals/docs/practical2.md) | File copying with system calls (`open`, `read`, `write`); tracing `cat` with `strace` |
| **Practical 3** | [practical3.md](file:///c:/Users/dell9/OneDrive/Desktop/OSSP/practicals/docs/practical3.md) | Parent and child PIDs, execution flow, and Linux process states |
| **Practical 4** | [practical4.md](file:///c:/Users/dell9/OneDrive/Desktop/OSSP/practicals/docs/practical4.md) | Process synchronization with `wait()` and `waitpid()`; zombie process generation and reaping |
| **Practical 5** | [practical5.md](file:///c:/Users/dell9/OneDrive/Desktop/OSSP/practicals/docs/practical5.md) | Anonymous pipes (`pipe()`), producer-consumer communication, and connecting `ls` and `grep` |
| **Practical 6** | [practical6.md](file:///c:/Users/dell9/OneDrive/Desktop/OSSP/practicals/docs/practical6.md) | Named pipes (FIFOs) multi-client server communication; async-signal-safe POSIX signal handling |
| **Practical 7** | [practical7.md](file:///c:/Users/dell9/OneDrive/Desktop/OSSP/practicals/docs/practical7.md) | Linux process address space inspection using `/proc/<PID>/maps`, `status`, and `pmap` |
| **Practical 8** | [practical8.md](file:///c:/Users/dell9/OneDrive/Desktop/OSSP/practicals/docs/practical8.md) | Dynamic memory management (`malloc`, `calloc`, `realloc`, `free`), Valgrind Memcheck, Copy-on-Write (`fork()`) |
| **Practical 9** | [practical9.md](file:///c:/Users/dell9/OneDrive/Desktop/OSSP/practicals/docs/practical9.md) | Low-level system call vs. standard I/O library file copying; standard stream redirection via `dup2()` |
| **Practical 10** | [practical10.md](file:///c:/Users/dell9/OneDrive/Desktop/OSSP/practicals/docs/practical10.md) | Inode structures, hard vs. symbolic links (`ln`, `stat`, `find`); memory-mapped I/O (`mmap()`) vs. `read()`/`write()` |
| **Practical 11** | [practical11.md](file:///c:/Users/dell9/OneDrive/Desktop/OSSP/practicals/docs/practical11.md) | Multithreaded concurrency using POSIX threads (`pthreads`); race condition demonstration and mutex synchronization |

---

## How to Use These Docs

Each document includes:
1. **Aim & Objectives**: Goals and theoretical foundations.
2. **Step-by-Step Instructions**: Exact compilation and execution commands.
3. **Observations & Analysis**: Expected outputs and architectural explanations.
