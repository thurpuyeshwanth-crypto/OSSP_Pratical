# Practical 7 - Linux Process Address Space

## Print representative addresses

Build and run `memory_layout.c`:

```bash
cd practicals/src
make memory_layout
./memory_layout
```

The program prints addresses for code, initialized global and static variables, BSS, heap, and stack. Address layouts vary because of ASLR and system/compiler differences. Initialized static storage belongs to data; uninitialized global/static storage belongs to BSS. There is no separate memory region just because a variable uses the `static` keyword.

## Inspect a live process

```bash
make memory_demo
./memory_demo
```

It prints its PID and stays alive for 45 seconds. In another terminal replace `<PID>` with that value:

```bash
cat /proc/<PID>/maps
cat /proc/<PID>/status
pmap <PID>
```

`maps` shows virtual address ranges, access permissions, and mapping names. `status` summarizes memory use; `pmap` gives a convenient mapping summary. Virtual addresses are process-local mappings, not direct physical RAM addresses.
