#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    int fd = open("input.txt", O_RDONLY);
    if (fd < 0) { perror("open"); return EXIT_FAILURE; }
    if (dup2(fd, STDIN_FILENO) < 0) { perror("dup2"); close(fd); return EXIT_FAILURE; }
    close(fd);
    char line[256];
    puts("Reading lines from redirected stdin:");
    while (fgets(line, sizeof(line), stdin) != NULL) fputs(line, stdout);
    if (ferror(stdin)) { perror("fgets"); return EXIT_FAILURE; }
    return EXIT_SUCCESS;
}
