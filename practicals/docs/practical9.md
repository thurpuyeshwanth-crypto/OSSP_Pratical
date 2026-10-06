# Practical 9 - File I/O and `dup2()` Redirection

## Compare low-level and standard-library file copying

`copy_lowlevel.c` uses `open()`, `read()`, `write()`, `lseek()`, and `close()`. `copy_stdio.c` uses buffered `fopen()`, `fread()`, `fwrite()`, and `fclose()`.

```bash
cd practicals/src
make copy_lowlevel copy_stdio
printf 'OSSP file I/O test\n' > input.txt
./copy_lowlevel input.txt output_low.dat
./copy_stdio input.txt output_stdio.dat
cmp input.txt output_low.dat
cmp input.txt output_stdio.dat
```

To compare timing, use a larger input and run `time ./copy_lowlevel bigfile low-copy` and `time ./copy_stdio bigfile stdio-copy`. Run each several times and compare checksums. Cache state, storage, buffer sizes, compiler optimization, and background activity affect results; a single run is not a reliable benchmark.

## Redirect standard input and output

`redirect_output.c` uses `open()` and `dup2()` to make standard output point to `output.txt`. `redirect_input.c` makes standard input read from `input.txt`.

```bash
make redirect_output redirect_input
./redirect_output
cat output.txt
printf 'first line\nsecond line\n' > input.txt
./redirect_input
```

The standard descriptors are 0 for stdin, 1 for stdout, and 2 for stderr. After `dup2(file_fd, STDOUT_FILENO)`, writes to stdout go to the file; after `dup2(file_fd, STDIN_FILENO)`, reads from stdin come from the file.
