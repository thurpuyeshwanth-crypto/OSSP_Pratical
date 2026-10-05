# Practical 4 - `wait()`, `waitpid()`, and Zombies

## Compare child-waiting calls

`src/wait_waitpid_demo.c` starts three children with different delays. `wait()` collects whichever child finishes first; `waitpid(pid, ...)` lets the parent select a particular child.

```bash
cd practicals/src
make wait_waitpid_demo
./wait_waitpid_demo
```

All children are collected so they do not remain as zombies.

## Observe and reap a zombie

`src/zombie_process.c` intentionally lets a short-lived child finish and waits ten seconds before calling `waitpid()`.

```bash
make zombie_process
./zombie_process
```

While it pauses, use the child PID printed by the program in another terminal:

```bash
ps -o pid,ppid,state,cmd -p <child-pid>
```

A `Z` state means the child has exited but its parent has not yet collected its status. After the pause, `waitpid()` reaps it and its process-table entry disappears. This delay is only for demonstration; real programs should reap children promptly.
