#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 4096

int main(int argc, char **argv)
{
    char buffer[BUFFER_SIZE];
    int source_fd, destination_fd, failed = 0;
    ssize_t bytes_read;

    if (argc != 3) {
        fprintf(stderr, "Usage: %s <source_file> <destination_file>\n", argv[0]);
        return EXIT_FAILURE;
    }
    source_fd = open(argv[1], O_RDONLY);
    if (source_fd < 0) { perror("open source"); return EXIT_FAILURE; }
    destination_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (destination_fd < 0) {
        perror("open destination"); close(source_fd); return EXIT_FAILURE;
    }

    while ((bytes_read = read(source_fd, buffer, sizeof(buffer))) != 0) {
        if (bytes_read < 0) {
            if (errno == EINTR) continue;
            perror("read"); failed = 1; break;
        }
        ssize_t offset = 0;
        while (offset < bytes_read) {
            ssize_t written = write(destination_fd, buffer + offset, (size_t)(bytes_read - offset));
            if (written < 0) {
                if (errno == EINTR) continue;
                perror("write"); failed = 1; break;
            }
            offset += written;
        }
        if (failed) break;
    }
    if (close(source_fd) < 0) { perror("close source"); failed = 1; }
    if (close(destination_fd) < 0) { perror("close destination"); failed = 1; }
    if (failed) return EXIT_FAILURE;
    puts("File copied successfully.");
    return EXIT_SUCCESS;
}
