#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define NUM_CHILDREN 3

static int report_child(pid_t pid, int status, const char *method)
{
    if (pid < 0) { perror(method); return -1; }
    if (WIFEXITED(status))
        printf("%s: child PID %ld exited with status %d\n", method, (long)pid, WEXITSTATUS(status));
    else if (WIFSIGNALED(status))
        printf("%s: child PID %ld ended from signal %d\n", method, (long)pid, WTERMSIG(status));
    return 0;
}

int main(void)
{
    pid_t children[NUM_CHILDREN];
    int status;
    fflush(NULL);
    printf("Parent PID: %ld\n", (long)getpid());
    fflush(stdout);

    for (int i = 0; i < NUM_CHILDREN; i++) {
        pid_t pid = fork();
        if (pid < 0) { perror("fork"); return EXIT_FAILURE; }
        if (pid == 0) {
            printf("Child %d PID=%ld PPID=%ld\n", i + 1, (long)getpid(), (long)getppid());
            fflush(stdout);
            sleep((unsigned int)i + 1);
            _exit((i + 1) * 10);
        }
        children[i] = pid;
    }

    puts("-- wait(): collects whichever child finishes first --");
    pid_t done;
    do { done = wait(&status); } while (done < 0 && errno == EINTR);
    if (report_child(done, status, "wait()") < 0) return EXIT_FAILURE;

    puts("-- waitpid(): waits for a selected child --");
    for (int i = 0; i < NUM_CHILDREN; i++) {
        if (children[i] == done) continue;
        do { done = waitpid(children[i], &status, 0); } while (done < 0 && errno == EINTR);
        if (report_child(done, status, "waitpid()") < 0) return EXIT_FAILURE;
    }
    puts("Parent: all children have been reaped.");
    return EXIT_SUCCESS;
}
