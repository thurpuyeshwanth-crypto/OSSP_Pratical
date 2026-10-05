# Practical 5 - Pipes and Producer-Consumer Communication

## Anonymous pipe

`src/prog5.c` makes a pipe before `fork()`. The parent sends 10,000 fixed-size records and the child reads complete records. It reports elapsed time and approximate throughput. Pipe reads are a byte stream, so the consumer assembles complete records rather than assuming one `read()` always equals one message.

```bash
cd practicals/src
make prog5
./prog5
```

Throughput changes between runs and computers; it is a measurement, not a fixed result.

## Connect two commands with a pipe

`src/ls_grep_pipe.c` recreates `ls -l | grep ".c"` using `pipe()`, `fork()`, `dup2()`, and `exec`.

```bash
make ls_grep_pipe
./ls_grep_pipe
```

The first child redirects standard output to the pipe. The second redirects standard input from it. The parent closes both pipe ends and waits for both children.
