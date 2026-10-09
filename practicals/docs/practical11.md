# Practical 11 - Multithreaded Counter and Race Condition Using POSIX Threads

## Aim & Objectives

- **Aim**: To develop a multithreaded C program using POSIX threads (`pthread_create()` and `pthread_join()`), demonstrate a race condition when multiple threads concurrently update a shared counter, and resolve it using POSIX mutex synchronization (`pthread_mutex_t`) to compare correctness and performance.
- **Objectives**:
  1. Create and manage concurrent execution paths using POSIX threads (`pthreads`).
  2. Demonstrate how unsynchronized concurrent access to a shared resource leads to race conditions and lost updates.
  3. Analyze why `counter++` is non-atomic at the CPU assembly level (Load-Modify-Store).
  4. Protect shared critical sections using mutual exclusion locks (`pthread_mutex_lock()`, `pthread_mutex_unlock()`).
  5. Measure and analyze execution times to understand the trade-off between concurrency correctness and synchronization overhead.

---

## 1. Concepts: Threads and Race Conditions

### Process Memory vs. Thread Memory

A **thread** is an independent unit of execution scheduled within a process. All threads in a process share:
- Virtual address space
- Global and static variables
- Heap memory
- Open file descriptors and signal handlers

However, each thread maintains private context:
- Thread ID (`pthread_t`)
- Stack (local function frames and variables)
- Processor registers and Program Counter (PC)

### Why `counter++` Causes a Race Condition

Even though `counter++` appears as a single statement in C, the CPU executes it as a **three-step read-modify-write instruction sequence**:

```text
1. LOAD counter from RAM/cache into CPU register
2. ADD 1 to the register value
3. STORE updated value from register back to RAM/cache
```

When multiple threads execute this sequence simultaneously without synchronization, their operations interleave unpredictably:

```text
Initial counter = 0

Thread 1                             Thread 2
──────────────────────────────       ──────────────────────────────
1. LOAD counter (reads 0)
                                     1. LOAD counter (reads 0)
2. ADD 1 (register = 1)
                                     2. ADD 1 (register = 1)
3. STORE 1 into counter
                                     3. STORE 1 into counter
───────────────────────────────────────────────────────────────────
Result: counter = 1
Expected Result: counter = 2  (One update is permanently lost!)
```

---

## 2. Unsynchronized Program (`race_counter.c`)

### Build and Run

```bash
cd practicals/src
make race_counter
./race_counter
```

Run it multiple times:

```bash
./race_counter
./race_counter
./race_counter
```

### Sample Output

```text
$ ./race_counter
Expected counter value: 4000000
Actual counter value:   2738461
Execution time:         0.008432 seconds

$ ./race_counter
Expected counter value: 4000000
Actual counter value:   3185724
Execution time:         0.007915 seconds
```

### Key Observations

- The actual counter value is **substantially less than the expected 4,000,000**.
- The result changes with each run because OS thread scheduling is non-deterministic.
- Millions of increments are lost due to uncoordinated overlapping read-modify-write cycles.

### Key API Explanations

1. `pthread_create(&thread_id, attr, start_routine, arg)`:
   - Starts a new thread executing `increment_counter()`.
   - `attr = NULL` selects default stack size and scheduling attributes.
   - `arg = NULL` indicates no argument passed to worker function.
2. `pthread_join(thread_id, retval)`:
   - Suspends the calling thread (main) until the target thread terminates.
   - Prevents the process from exiting before background worker threads finish their work.

---

## 3. Synchronized Programs (`safe_counter.c` & `mutex_counter.c`)

### What is a Mutex?

A **mutex (mutual exclusion lock)** ensures that only one thread can execute a critical section at any given time. Other threads attempting to enter are blocked until the holding thread releases the lock.

```text
Thread 1 ──► pthread_mutex_lock() ──► [counter++] ──► pthread_mutex_unlock()
                                          ▲
Thread 2 ──► pthread_mutex_lock() ──[WAITS]───────► [counter++] ──► pthread_mutex_unlock()
```

### Build and Run

```bash
cd practicals/src
make safe_counter mutex_counter

# Run safe_counter (basic mutex synchronization)
./safe_counter

# Run mutex_counter (includes execution time measurement)
./mutex_counter
```

### Sample Output

```text
$ ./safe_counter
Expected counter value: 4000000
Actual counter value:   4000000

$ ./mutex_counter
Expected counter value: 4000000
Actual counter value:   4000000
Execution time:         0.184321 seconds
```

### Key API Explanations

1. `pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;`:
   - Statically initializes a fast default mutex.
2. `pthread_mutex_lock(&lock);`:
   - Acquires the lock. If another thread currently holds it, blocks the calling thread until it becomes available.
3. `pthread_mutex_unlock(&lock);`:
   - Releases the lock, waking up any waiting thread.
4. `pthread_mutex_destroy(&lock);`:
   - Cleans up and destroys mutex resources when no longer needed.

---

## 4. Performance & Synchronization Overhead

Comparing the execution times reveals the trade-off:

| Metric | `race_counter` (No Lock) | `mutex_counter` (With Mutex) |
|---|---|---|
| **Expected Value** | 4,000,000 | 4,000,000 |
| **Actual Value** | Inconsistent (~2.7M - 3.2M) | Exactly 4,000,000 |
| **Execution Time** | ~0.008 seconds | ~0.180 seconds |
| **Speed Factor** | Very Fast (Unsafe) | ~20x slower (Safe) |

### Why is Mutex Slower?

1. **Serialization**: The critical section cannot run in parallel across CPU cores; threads must execute sequentially.
2. **Context Switching & System Overhead**: Contended lock acquisition involves sleeping, waking up, and switching thread execution states.
3. **Cache Invalidation**: Frequent modification of shared memory and lock state across CPU cores requires cache line synchronization (MESI cache protocol).

> [!TIP]
> **Optimization Strategy**: In production environments where high performance is required, each thread should increment a private thread-local counter during the loop, and only add its final total to the global counter using a mutex once at the end. Alternatively, atomic hardware instructions (such as GCC `__atomic_fetch_add()` or C11 `<stdatomic.h>`) can provide synchronization without heavy mutex lock overhead.

---

## 5. Summary Comparison

| Feature | Without Mutex (`race_counter.c`) | With Mutex (`mutex_counter.c`) |
|---|---|---|
| **Shared Resource** | `long long counter` | `long long counter` |
| **Concurrent Access** | Unrestricted / unsynchronized | Serialized through `pthread_mutex_t` |
| **Race Conditions** | High (frequent lost updates) | Completely prevented |
| **Correctness** | Broken / unpredictable | 100% Deterministic and correct |
| **Lock Overhead** | None | Lock / Unlock cycles on each loop iteration |
| **Compilation Flag** | `-pthread` | `-pthread` |
