#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 8192

int main(int argc, char **argv)
{
    if (argc != 3) { fprintf(stderr, "Usage: %s <source> <destination>\n", argv[0]); return EXIT_FAILURE; }
    int src = open(argv[1], O_RDONLY);
    if (src < 0) { perror("open source"); return EXIT_FAILURE; }
    int dst = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dst < 0) { perror("open destination"); close(src); return EXIT_FAILURE; }
    char buffer[BUFFER_SIZE];
    ssize_t n;
    int failed = 0;
    while ((n = read(src, buffer, sizeof(buffer))) != 0) {
        if (n < 0) { if (errno == EINTR) continue; perror("read"); failed = 1; break; }
        ssize_t offset = 0;
        while (offset < n) {
            ssize_t written = write(dst, buffer + offset, (size_t)(n - offset));
            if (written < 0) { if (errno == EINTR) continue; perror("write"); failed = 1; break; }
            offset += written;
        }
        if (failed) break;
    }
    if (!failed) {
        off_t size = lseek(src, 0, SEEK_END);
        if (size >= 0) printf("Source file size: %lld bytes\n", (long long)size);
        else perror("lseek");
    }
    if (close(src) < 0) { perror("close source"); failed = 1; }
    if (close(dst) < 0) { perror("close destination"); failed = 1; }
    if (failed) return EXIT_FAILURE;
    puts("Low-level copy complete.");
    return EXIT_SUCCESS;
}
