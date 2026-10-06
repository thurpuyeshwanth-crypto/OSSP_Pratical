# Practical 8 - Dynamic Memory, Valgrind, and Copy-on-Write

## Dynamic allocation

`dynamic_memory.c` demonstrates `malloc()`, zero-initialized `calloc()`, safe `realloc()` using a temporary pointer, and `free()`.

```bash
cd practicals/src
make dynamic_memory
./dynamic_memory
valgrind --leak-check=full --show-leak-kinds=all ./dynamic_memory
```

A clean Memcheck run should report no lost heap blocks and no invalid memory operations. To see how a leak looks, temporarily omit a `free()` and rerun Valgrind, then restore the cleanup.

## Copy-on-Write after `fork()`

`cow_demo.c` allocates and initializes 100 MiB, then forks. The child waits for the parent's signal, writes one byte per 4 KiB page, and stays alive briefly for inspection.

```bash
make cow_demo
./cow_demo
```

Use the printed parent and child PIDs in another terminal with `cat /proc/<PID>/smaps` or `grep -E '^(Rss|Pss|Shared_Dirty|Private_Dirty)' /proc/<PID>/smaps`. Pages are initially shared where possible; a child write creates private copies of affected pages. Exact measurements depend on Linux, page size, and the allocator.
