# Practical 10 - Inodes, Hard/Symbolic Links, and Memory-Mapped I/O (`mmap`)

## Aim & Objectives

- **Aim**: To investigate Linux filesystem inode structures using `ls -i`, `stat`, and `find`, explore the creation and behavior of hard and symbolic links, and implement and evaluate memory-mapped file I/O (`mmap()`) in comparison with traditional `read()`/`write()` operations.
- **Objectives**:
  1. Inspect inode numbers, link counts, and file metadata using `ls -li` and `stat`.
  2. Create hard links (`ln`) and symbolic links (`ln -s`), observing inode allocation and file persistence when the original file is deleted.
  3. Use `find` to discover directory entries sharing an inode and locate symbolic links.
  4. Develop and analyze a C program using `mmap()`, `msync()`, and `munmap()` for memory-mapped file reading and modification.
  5. Develop and analyze a C program performing equivalent operations using standard `read()`, `lseek()`, and `write()`.
  6. Compare both I/O mechanisms based on execution performance, system call overhead, and implementation characteristics.

---

## Part 1: Investigating Inodes, Hard Links, and Symbolic Links

### 1. Understanding Inodes

In Linux filesystems (ext4, XFS, etc.), an **inode (index node)** is an internal data structure that stores all metadata about a filesystem object, except its filename and actual data contents:

- **Metadata stored in an inode**:
  - File type (regular, directory, symlink, FIFO, socket, device)
  - Permissions (rwxrwxrwx)
  - Owner (UID) and Group (GID)
  - File size in bytes
  - Timestamps (Access `atime`, Modification `mtime`, Change `ctime`)
  - Hard link count
  - Pointers to data blocks on storage

A directory is essentially a table of `(filename, inode_number)` mappings:

```text
Directory Entry
   │
   └─── filename ───────► inode ───────► [File Metadata]
                           │
                           └───────────► [Disk Data Blocks]
```

---

### 2. Inspecting Inodes with `ls -li` and `stat`

Navigate to `practicals/src/` and create a sample file:

```bash
cd practicals/src
echo "Linux inode investigation" > original.txt
```

#### Check Inode with `ls -li`
```bash
ls -li original.txt
```
Example output:
```text
1234567 -rw-r--r-- 1 user user 26 Oct 09 14:00 original.txt
   ▲        ▲      ▲              ▲
 Inode  Permissions Links       Size
```

#### Detailed Metadata with `stat`
```bash
stat original.txt
```
Key fields to examine:
- `Inode`: The filesystem index node number allocated to this file.
- `Links`: Number of directory entries (hard links) pointing to this inode (initially `1`).
- `Blocks`: Number of 512-byte filesystem blocks allocated.
- `Access`, `Modify`, `Change`: Last access, content change, and inode metadata change timestamps.

---

### 3. Creating and Testing Hard Links

Create a hard link to `original.txt`:

```bash
ln original.txt hardlink.txt
ls -li
```

#### Observations:
1. **Identical Inode**: Both `original.txt` and `hardlink.txt` show the exact same inode number.
   ```text
   original.txt ───┐
                   ├──► Inode 1234567 ──► [Data Blocks]
   hardlink.txt ───┘
   ```
2. **Link Count Increments**: The link count (`Links`) increases from `1` to `2`.
3. **Data Sharing**:
   ```bash
   echo "Appended through hard link" >> hardlink.txt
   cat original.txt
   ```
   Both names refer to the exact same disk blocks; modifying one immediately updates the other.

4. **Deleting the Original File**:
   ```bash
   rm original.txt
   ls -li hardlink.txt
   cat hardlink.txt
   ```
   - `rm` removes only the directory entry `original.txt` and decrements the inode link count from `2` to `1`.
   - The inode and file data remain completely accessible via `hardlink.txt`.
   - Disk space is only reclaimed when the link count reaches `0` and no running process holds an open file descriptor to it.

---

### 4. Creating and Testing Symbolic (Soft) Links

Create a fresh file and a symbolic link:

```bash
echo "Symbolic link demonstration" > original.txt
ln -s original.txt symlink.txt
ls -li
```

#### Observations:
1. **Independent Inodes**: `original.txt` and `symlink.txt` have **different inode numbers**:
   ```text
   original.txt ──► Inode 1234568 ──► [Data Blocks]
   symlink.txt  ──► Inode 1234569 ──► "original.txt" (Path string)
   ```
2. **File Type**: The permissions show `l` (e.g., `lrwxrwxrwx`).
3. **Inspecting Link vs Target**:
   - `stat symlink.txt`: Shows metadata of the symbolic link itself.
   - `stat -L symlink.txt`: Follows (dereferences) the link and displays metadata of `original.txt`.
   - `readlink symlink.txt`: Displays the stored target path (`original.txt`).

4. **Broken (Dangling) Symlink**:
   ```bash
   rm original.txt
   ls -l symlink.txt
   cat symlink.txt
   ```
   - Deleting the target makes the symbolic link point to a non-existent path.
   - `cat` returns `cat: symlink.txt: No such file or directory`.

---

### 5. Inode Searching with `find`

Find all hard links pointing to a specific inode number:
```bash
INODE_NUM=$(ls -i hardlink.txt | awk '{print $1}')
find . -inum "$INODE_NUM"
```

Find all symbolic links in the directory:
```bash
find . -type l
find . -type l -ls
```

---

### 6. Comparison: Hard Link vs Symbolic Link

| Feature | Hard Link (`ln`) | Symbolic Link (`ln -s`) |
|---|---|---|
| **Inode Number** | Shares the same inode with target | Gets a new, separate inode |
| **Inode Link Count** | Increments target inode's link count | Does not affect target inode's link count |
| **Content** | Points directly to storage data blocks | Stores path string pointing to target file |
| **Target Deletion** | File data preserved (link count decrements) | Becomes a broken / dangling symlink |
| **Cross-Filesystem** | Cannot span across different filesystems | Can link across different filesystems/mounts |
| **Directories** | Not allowed (prevents filesystem cycles) | Allowed (can link directories) |

---

## Part 2: Memory-Mapped I/O (`mmap`) vs Traditional `read()`/`write()`

### 1. Architectural Difference

```text
Traditional File I/O:
Disk ──► Page Cache ──[read() system call]──► User Buffer ──► Process
Process ──► User Buffer ──[write() system call]──► Page Cache ──► Disk
(Two buffer copies, multiple context switches between user and kernel space)

Memory-Mapped File I/O:
Disk ──► Page Cache ──[Page Fault Mapping]──► Process Virtual Address Space
Process accesses mapped memory directly via pointers: data[i] = 'H';
Synchronized to storage via msync(..., MS_SYNC);
(Zero-copy data access directly within mapped virtual pages)
```

---

### 2. Building the Programs

```bash
cd practicals/src
make mmap_file read_write_file
```

---

### 3. Running `mmap_file.c`

Create the test input file:
```bash
echo "Linux memory mapped file I/O demonstration" > data.txt
./mmap_file
cat data.txt
```

#### Output:
```text
Original file contents:
Linux memory mapped file I/O demonstration

Modifying file...
File modified using mmap().
```

Verifying modification (`cat data.txt`):
```text
HELLO memory mapped file I/O demonstration
```

#### How `mmap_file.c` Works:
1. `open("data.txt", O_RDWR)`: Opens the file for both reading and writing.
2. `fstat(fd, &st)`: Retrieves the exact file size (`st.st_size`).
3. `mmap(NULL, st.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)`:
   - `NULL`: Lets the Linux kernel choose the starting virtual address.
   - `PROT_READ | PROT_WRITE`: Mapped pages allow reading and writing.
   - `MAP_SHARED`: Modifications are shared and written back to the underlying file.
   - `fd, 0`: Map starting from byte offset 0.
4. `memcpy(data, "HELLO", 5)`: Modifies the file in-place through direct pointer access without system call overhead.
5. `msync(data, st.st_size, MS_SYNC)`: Flushes modified (dirty) pages back to disk storage synchronously.
6. `munmap(data, st.st_size)`: Unmaps the virtual memory region.
7. `close(fd)`: Closes the file descriptor.

---

### 4. Running `read_write_file.c`

Reset `data.txt` and run the traditional `read()`/`write()` version:

```bash
echo "Linux memory mapped file I/O demonstration" > data.txt
./read_write_file
cat data.txt
```

#### Output:
```text
Original file contents:
Linux memory mapped file I/O demonstration

File modified using read()/write().
```

Verifying modification (`cat data.txt`):
```text
HELLO memory mapped file I/O demonstration
```

#### How `read_write_file.c` Works:
1. `open("data.txt", O_RDWR)`: Opens file.
2. `read(fd, buffer, sizeof(buffer) - 1)`: Reads bytes from kernel cache into user-space `buffer`.
3. `lseek(fd, 0, SEEK_SET)`: Explicitly resets the file offset back to position 0 before overwriting.
4. `write(fd, buffer, bytes_read)`: Invokes kernel write to copy data from user buffer back to the file.
5. `close(fd)`: Closes file.

---

### 5. Performance and Benchmarking

For large files with frequent random reads or updates, `mmap()` minimizes kernel copying overhead:

```bash
# Generate a 50 MB test file
dd if=/dev/urandom of=bench_data.dat bs=1M count=50

# Benchmark execution times
time ./mmap_file bench_data.dat
time ./read_write_file bench_data.dat

# Cleanup benchmark file
rm -f bench_data.dat
```

---

### 6. Comparison: `read()`/`write()` vs `mmap()`

| Feature | `read()` / `write()` | `mmap()` |
|---|---|---|
| **Mechanism** | Explicit system calls copying buffers | File mapped into process virtual address space |
| **Data Access** | Buffer-based via user-allocated memory | Direct memory pointer indexing (`data[i]`) |
| **System Call Overhead** | High (context switch on every read/write) | Low (only during setup `mmap` and sync `msync`) |
| **Page Faults** | Managed internally by kernel during I/O | Triggered on demand when referencing mapped memory |
| **Random Access** | Requires repeated `lseek()` calls | Instant pointer arithmetic (`data + offset`) |
| **Data Copying** | Two copies (disk ➔ kernel cache ➔ user buffer) | Zero-copy (process accesses kernel page cache directly) |
| **Synchronization** | Written to cache immediately via `write()` | Explicitly flushed with `msync()` or on unmap/page-out |
| **Virtual Address Space** | Minimal (only user buffer size) | Consumes virtual address space equal to mapping size |
| **Best Used For** | Simple sequential stream processing | Large files, databases, shared memory, frequent random access |
