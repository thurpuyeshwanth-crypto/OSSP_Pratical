#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    fflush(NULL);
    pid_t pid = fork();
    if (pid < 0) { perror("fork"); return EXIT_FAILURE; }
    if (pid == 0) {
        printf("Child: PID=%ld PPID=%ld (running)\n", (long)getpid(), (long)getppid());
        sleep(3);
        printf("Child: PID=%ld PPID=%ld (running after sleep)\n", (long)getpid(), (long)getppid());
        fflush(stdout);
        _exit(EXIT_SUCCESS);
    }

    printf("Parent: PID=%ld PPID=%ld ChildPID=%ld (running)\n", (long)getpid(), (long)getppid(), (long)pid);
    sleep(1);
    puts("Parent: waiting for child (blocked in waitpid)");
    if (waitpid(pid, NULL, 0) < 0) { perror("waitpid"); return EXIT_FAILURE; }
    puts("Parent: child terminated and was reaped; parent resumes");
    return EXIT_SUCCESS;
}
