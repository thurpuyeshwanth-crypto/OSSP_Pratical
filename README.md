# OSSP_PraticalWork

Operating Systems and Systems Programming practical programs, organized by session.

## Practical sessions

| Practical | Topic |
|---|---|
| 1 | Execute Linux commands with `fork()` and `execvp()`; inspect OS and hardware information |
| 2 | Copy files with system calls; inspect `cat` with `strace` |
| 3 | Observe parent and child PIDs and process states |
| 4 | Compare `wait()` and `waitpid()`; inspect and reap a zombie |
| 5 | Anonymous pipes, producer-consumer communication, and a two-command pipeline |

## Source code

- [Practical source files](practicals/src/)
- [Build instructions](practicals/src/Makefile)
- [Practical guide](practicals/README.md)

## Build

```bash
cd practicals/src
make
```

The docs folder has the run steps and explanations for each session. Generated executables and temporary sample files are excluded from version control.
