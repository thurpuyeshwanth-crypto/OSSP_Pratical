#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    int pipefd[2];
    if (pipe(pipefd) < 0) { perror("pipe"); return EXIT_FAILURE; }
    pid_t ls_pid = fork();
    if (ls_pid < 0) { perror("fork ls"); return EXIT_FAILURE; }
    if (ls_pid == 0) {
        close(pipefd[0]);
        if (dup2(pipefd[1], STDOUT_FILENO) < 0) { perror("dup2 ls"); _exit(127); }
        close(pipefd[1]);
        execlp("ls", "ls", "-l", (char *)NULL);
        perror("exec ls"); _exit(127);
    }

    pid_t grep_pid = fork();
    if (grep_pid < 0) {
        perror("fork grep"); close(pipefd[0]); close(pipefd[1]);
        (void)waitpid(ls_pid, NULL, 0); return EXIT_FAILURE;
    }
    if (grep_pid == 0) {
        close(pipefd[1]);
        if (dup2(pipefd[0], STDIN_FILENO) < 0) { perror("dup2 grep"); _exit(127); }
        close(pipefd[0]);
        execlp("grep", "grep", ".c", (char *)NULL);
        perror("exec grep"); _exit(127);
    }

    close(pipefd[0]); close(pipefd[1]);
    int status1, status2;
    while (waitpid(ls_pid, &status1, 0) < 0) { if (errno != EINTR) { perror("waitpid ls"); return EXIT_FAILURE; } }
    while (waitpid(grep_pid, &status2, 0) < 0) { if (errno != EINTR) { perror("waitpid grep"); return EXIT_FAILURE; } }
    puts("Pipeline execution completed.");
    return EXIT_SUCCESS;
}
