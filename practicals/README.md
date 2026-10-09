# OSSP Practical Work

Operating Systems and Systems Programming practical programs, organized by session. Each session page explains what to compile, run, and observe.

| Practical | Status | Topic |
|---|---|---|
| 1 | Included | Run a Linux command with `fork()` and `execvp()`; inspect OS and hardware information |
| 2 | Included | Copy a file with system calls; trace `cat` using `strace` |
| 3 | Included | Observe parent and child PIDs and process states |
| 4 | Included | Compare `wait()` and `waitpid()`; observe and reap a zombie |
| 5 | Included | Producer-consumer communication with an anonymous pipe; connect `ls` and `grep` |
| 6 | Included | Client–Server application using Named Pipes (FIFOs); POSIX signal handling with `sigaction()` |
| 7 | Included | Process address space inspection via `/proc/<PID>/maps`, `status`, and `pmap` |
| 8 | Included | Dynamic memory allocation, Valgrind leak checking, and Copy-on-Write (`fork()`) |
| 9 | Included | Low-level vs stdio file copying; standard I/O redirection with `dup2()` |
| 10 | Included | Inodes, hard links, symbolic links; memory-mapped I/O (`mmap()`) vs `read()`/`write()` |
| 11 | Included | POSIX threads (`pthreads`), race condition demonstration, and mutex synchronization |

Build the programs with `cd src && make`. See the session notes in `docs/` for run commands, explanations, and observations. Generated executables and temporary sample files are excluded from the repository.
