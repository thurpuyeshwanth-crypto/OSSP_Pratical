#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define NUM_MESSAGES 10000
#define MESSAGE_SIZE 100

static int read_record(int fd, char *buffer, size_t size)
{
    size_t received = 0;
    while (received < size) {
        ssize_t n = read(fd, buffer + received, size - received);
        if (n == 0) return received == 0 ? 0 : -1;
        if (n < 0) { if (errno == EINTR) continue; return -1; }
        received += (size_t)n;
    }
    return 1;
}

int main(void)
{
    int pipefd[2];
    struct timespec start, end;
    if (pipe(pipefd) < 0) { perror("pipe"); return EXIT_FAILURE; }
    pid_t pid = fork();
    if (pid < 0) { perror("fork"); return EXIT_FAILURE; }
    if (pid == 0) {
        close(pipefd[1]);
        char buffer[MESSAGE_SIZE];
        int count = 0, result;
        while ((result = read_record(pipefd[0], buffer, sizeof(buffer))) > 0) count++;
        if (result < 0) { perror("consumer read"); _exit(EXIT_FAILURE); }
        close(pipefd[0]);
        printf("Consumer received %d complete messages.\n", count);
        fflush(stdout);
        _exit(EXIT_SUCCESS);
    }

    close(pipefd[0]);
    if (clock_gettime(CLOCK_MONOTONIC, &start) < 0) { perror("clock_gettime"); return EXIT_FAILURE; }
    char message[MESSAGE_SIZE];
    for (int i = 0; i < NUM_MESSAGES; i++) {
        int length = snprintf(message, sizeof(message), "Message %d from producer", i + 1);
        if (length < 0 || (size_t)length >= sizeof(message)) { fprintf(stderr, "message formatting failed\n"); return EXIT_FAILURE; }
        size_t sent = 0;
        while (sent < sizeof(message)) {
            ssize_t n = write(pipefd[1], message + sent, sizeof(message) - sent);
            if (n < 0) { if (errno == EINTR) continue; perror("write"); return EXIT_FAILURE; }
            sent += (size_t)n;
        }
    }
    close(pipefd[1]);
    int status;
    while (waitpid(pid, &status, 0) < 0) { if (errno != EINTR) { perror("waitpid"); return EXIT_FAILURE; } }
    if (clock_gettime(CLOCK_MONOTONIC, &end) < 0) { perror("clock_gettime"); return EXIT_FAILURE; }

    double elapsed = (double)(end.tv_sec - start.tv_sec) + (double)(end.tv_nsec - start.tv_nsec) / 1e9;
    long total_bytes = (long)NUM_MESSAGES * MESSAGE_SIZE;
    printf("Producer sent %d messages (%ld bytes) in %.6f seconds.\n", NUM_MESSAGES, total_bytes, elapsed);
    if (elapsed > 0) printf("Approximate throughput: %.2f MiB/s\n", total_bytes / elapsed / (1024.0 * 1024.0));
    return WIFEXITED(status) && WEXITSTATUS(status) == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
