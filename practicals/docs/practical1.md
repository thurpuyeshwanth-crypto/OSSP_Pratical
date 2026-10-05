# Practical 1 - Command Execution and OS Overview

## Run a command from a C program

`src/prog1.c` reads one command line, splits it into arguments, and demonstrates the standard process pattern: `fork()` creates a child, the child runs the program with `execvp()`, and the parent collects its completion with `waitpid()`.

```bash
cd practicals/src
make prog1
./prog1
```

Try `ls -l` or `date`. This introductory program splits on whitespace and does not parse shell quotes or pipes. It reports both parent and child PIDs.

## Observe hardware and OS services

Run these Linux commands and compare what each reveals:

| Command | What it shows |
|---|---|
| `uname -a` | Kernel and system identity |
| `lscpu` | Processor architecture and CPU details |
| `lsblk` | Storage devices and partitions |
| `ps` | A snapshot of processes |
| `top` | Live process and resource activity |

The OS abstracts hardware behind processes, virtual memory, files, and device interfaces. Programs request services through system calls instead of controlling CPU, memory chips, disks, or devices directly.
