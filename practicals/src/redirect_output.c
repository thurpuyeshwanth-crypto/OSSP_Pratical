#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    int fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) { perror("open"); return EXIT_FAILURE; }
    puts("This line is printed in the terminal before redirection.");
    fflush(stdout);
    if (dup2(fd, STDOUT_FILENO) < 0) { perror("dup2"); close(fd); return EXIT_FAILURE; }
    close(fd);
    puts("This line goes into output.txt.");
    puts("stdout now refers to the output file.");
    return EXIT_SUCCESS;
}
