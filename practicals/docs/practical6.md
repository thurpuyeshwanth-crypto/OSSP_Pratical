# Practical 6 - Named Pipes (FIFOs) and POSIX Signal Handling

## Client–Server Application using Named Pipes (FIFOs)

`prog6_fifo_server.c` and `prog6_fifo_client.c` implement a multi-client server communication system using two-way FIFOs:
- **Request Channel**: All clients send requests to a shared server FIFO (`/tmp/server_fifo`).
- **Response Channel**: Each client creates a private FIFO named `/tmp/client_<PID>_fifo` where the server delivers its response.

### Compile and Run

Open three Linux / WSL terminals:

**Terminal 1 — Server:**
```bash
cd practicals/src
make prog6_fifo_server
./prog6_fifo_server
```

**Terminal 2 — Client 1:**
```bash
cd practicals/src
make prog6_fifo_client
./prog6_fifo_client
```

**Terminal 3 — Client 2:**
```bash
cd practicals/src
./prog6_fifo_client
```

### Multiple-Client FIFO Behavior

- **Atomic Writes**: Messages smaller than or equal to `PIPE_BUF` (4096 bytes on Linux) are guaranteed to be written atomically, preventing interleaved requests from simultaneous clients.
- **Sequential Processing**: The server reads incoming requests from the shared FIFO in the order they arrive and writes the response back to each client's specific FIFO.
- **Preventing Premature EOF**: The server opens the FIFO with `O_RDWR` so that `read()` does not return EOF when all clients temporarily close their writing descriptors.

To clean up remaining FIFO special files after stopping the server:
```bash
rm -f /tmp/server_fifo /tmp/client_*_fifo
```

---

## POSIX Signal Handling

`signal_handler.c` demonstrates asynchronous event handling by catching `SIGINT`, `SIGTERM`, and `SIGUSR1` using `sigaction()`.

### Compile and Run

**Terminal 1 — Run Program:**
```bash
cd practicals/src
make signal_handler
./signal_handler
```

It prints its PID and waits for signals with `pause()`.

**Terminal 2 — Send Signals:**
Replace `<PID>` with the PID displayed:

```bash
# Test SIGUSR1 (User-defined signal)
kill -SIGUSR1 <PID>

# Test SIGINT (Interrupt signal, or press Ctrl+C in Terminal 1)
kill -SIGINT <PID>

# Test SIGTERM (Graceful termination request)
kill -SIGTERM <PID>
```

### Key Concept: Async-Signal Safety

The signal handler only updates `volatile sig_atomic_t` flag variables. The main execution loop checks these flags and performs actions like `printf()`. This avoids invoking non-reentrant functions directly inside signal handler contexts.
