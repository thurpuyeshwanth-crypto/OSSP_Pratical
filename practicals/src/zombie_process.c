#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    int status;
    fflush(NULL);
    printf("Parent PID: %ld\n", (long)getpid());
    pid_t pid = fork();
    if (pid < 0) { perror("fork"); return EXIT_FAILURE; }
    if (pid == 0) {
        printf("Child PID: %ld; exiting now\n", (long)getpid());
        fflush(stdout);
        _exit(10);
    }

    printf("Child PID: %ld. Parent will wait 10 seconds before collecting it.\n", (long)pid);
    puts("During the pause, inspect from another terminal: ps -o pid,ppid,state,cmd -p <child-pid>");
    fflush(stdout);
    sleep(10);
    pid_t reaped;
    do { reaped = waitpid(pid, &status, 0); } while (reaped < 0 && errno == EINTR);
    if (reaped < 0) { perror("waitpid"); return EXIT_FAILURE; }
    if (WIFEXITED(status)) printf("Child reaped; exit status %d.\n", WEXITSTATUS(status));
    return EXIT_SUCCESS;
}
