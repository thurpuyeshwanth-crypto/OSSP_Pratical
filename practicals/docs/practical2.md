# Practical 2 - File System Calls and `strace`

## Copy a file using system calls

`src/prog2.c` uses `open()`, `read()`, `write()`, and `close()` to copy bytes from a source to a destination. It handles interrupted and partial reads or writes.

```bash
cd practicals/src
make prog2
printf 'Operating Systems Lab Program\n' > input.txt
./prog2 input.txt output.txt
cmp input.txt output.txt
```

A system call switches execution from user mode into the kernel, where Linux performs the requested file operation, then returns a result to the program.

## Trace `cat`

```bash
printf 'Operating Systems\n' > sample.txt
strace cat sample.txt
```

You will typically see calls such as `execve()` to start the program, `openat()` to open the file, `read()` to get bytes, `write()` to display them, `close()` to release descriptors, and `exit_group()` to finish. Dynamic loader and memory calls such as `mmap()` may also appear. Exact calls vary by Linux system and library version.
