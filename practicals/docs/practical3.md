# Practical 3 - Process IDs and States

`src/prog3.c` creates one child with `fork()`. It prints PID and PPID, pauses in both processes, then has the parent wait for and reap the child.

```bash
cd practicals/src
make prog3
./prog3
```

The child and parent execute concurrently, so the order of some output can vary. `sleep()` places a process in a waiting/sleeping state; `waitpid()` blocks the parent until its selected child changes state and terminates. Use `ps -o pid,ppid,state,cmd -p <pid>` in another terminal if you want to inspect a live process while it is sleeping.
