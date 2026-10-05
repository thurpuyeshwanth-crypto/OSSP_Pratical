#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define LINE_SIZE 1024
#define ARG_LIMIT 64

int main(void)
{
    char line[LINE_SIZE];
    char *args[ARG_LIMIT];
    size_t count = 0;
    pid_t pid;
    int status;

    printf("Enter a Linux command: ");
    fflush(stdout);
    if (fgets(line, sizeof(line), stdin) == NULL) return 0;
    line[strcspn(line, "\n")] = '\0';

    char *token = strtok(line, " \t");
    while (token != NULL && count + 1 < ARG_LIMIT) {
        args[count++] = token;
        token = strtok(NULL, " \t");
    }
    if (token != NULL) {
        fprintf(stderr, "Too many arguments (maximum %d).\n", ARG_LIMIT - 1);
        return EXIT_FAILURE;
    }
    if (count == 0) {
        fprintf(stderr, "Please enter a command.\n");
        return EXIT_FAILURE;
    }
    args[count] = NULL;

    pid = fork();
    if (pid < 0) { perror("fork"); return EXIT_FAILURE; }
    if (pid == 0) {
        fprintf(stderr, "Child PID: %ld, Parent PID: %ld\n", (long)getpid(), (long)getppid());
        execvp(args[0], args);
        perror("execvp");
        _exit(127);
    }

    printf("Parent PID: %ld, Child PID: %ld\n", (long)getpid(), (long)pid);
    while (waitpid(pid, &status, 0) < 0) {
        if (errno != EINTR) { perror("waitpid"); return EXIT_FAILURE; }
    }
    puts("Child process has completed.");
    return WIFEXITED(status) ? WEXITSTATUS(status) : EXIT_FAILURE;
}
