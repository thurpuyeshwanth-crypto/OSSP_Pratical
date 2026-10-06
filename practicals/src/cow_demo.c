#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#define SIZE (100U * 1024U * 1024U)
#define PAGE_STEP 4096U

int main(void)
{
    int gate[2];
    char *data = malloc(SIZE);
    if (data == NULL) { perror("malloc"); return EXIT_FAILURE; }
    for (size_t i = 0; i < SIZE; i += PAGE_STEP) data[i] = 1;
    if (pipe(gate) < 0) { perror("pipe"); free(data); return EXIT_FAILURE; }
    printf("Parent PID %ld initialized 100 MiB at %p.\n", (long)getpid(), (void *)data);
    puts("Inspect /proc/<PID>/smaps now, then press Enter for fork()."); fflush(stdout);
    (void)getchar();

    pid_t pid = fork();
    if (pid < 0) { perror("fork"); close(gate[0]); close(gate[1]); free(data); return EXIT_FAILURE; }
    if (pid == 0) {
        close(gate[1]);
        char go;
        if (read(gate[0], &go, 1) != 1) _exit(EXIT_FAILURE);
        close(gate[0]);
        for (size_t i = 0; i < SIZE; i += PAGE_STEP) data[i] = 2;
        printf("Child PID %ld wrote one byte per page; inspect this PID for about 15 seconds.\n", (long)getpid());
        fflush(stdout);
        sleep(15);
        free(data);
        _exit(EXIT_SUCCESS);
    }

    close(gate[0]);
    printf("Child PID %ld inherited the mappings. Inspect both PIDs now; press Enter to let the child write.\n", (long)pid);
    fflush(stdout);
    (void)getchar();
    const char go = 'x';
    if (write(gate[1], &go, 1) != 1) { perror("write gate"); close(gate[1]); waitpid(pid, NULL, 0); free(data); return EXIT_FAILURE; }
    close(gate[1]);
    int status;
    if (waitpid(pid, &status, 0) < 0) { perror("waitpid"); free(data); return EXIT_FAILURE; }
    free(data);
    return WIFEXITED(status) ? WEXITSTATUS(status) : EXIT_FAILURE;
}
