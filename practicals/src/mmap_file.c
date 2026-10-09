#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>

int main(int argc, char *argv[])
{
    const char *filepath = (argc > 1) ? argv[1] : "data.txt";
    int fd;
    struct stat st;
    char *data;

    /* Open file for reading and writing */
    fd = open(filepath, O_RDWR);
    if (fd == -1)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }

    /* Obtain file size */
    if (fstat(fd, &st) == -1)
    {
        perror("fstat");
        close(fd);
        exit(EXIT_FAILURE);
    }

    if (st.st_size == 0)
    {
        printf("File is empty.\n");
        close(fd);
        return 0;
    }

    /* Map file into memory */
    data = mmap(NULL,
                (size_t)st.st_size,
                PROT_READ | PROT_WRITE,
                MAP_SHARED,
                fd,
                0);
    if (data == MAP_FAILED)
    {
        perror("mmap");
        close(fd);
        exit(EXIT_FAILURE);
    }

    /* Read file through memory mapping */
    printf("Original file contents:\n");
    fwrite(data, 1, (size_t)st.st_size, stdout);
    printf("\n\nModifying file...\n");

    /* Modify first 5 characters.
     * Make sure the file contains at least 5 bytes.
     */
    if (st.st_size >= 5)
    {
        memcpy(data, "HELLO", 5);
    }

    /* Synchronize changes with the file */
    if (msync(data, (size_t)st.st_size, MS_SYNC) == -1)
    {
        perror("msync");
    }

    printf("File modified using mmap().\n");

    /* Remove mapping */
    if (munmap(data, (size_t)st.st_size) == -1)
    {
        perror("munmap");
    }

    close(fd);
    return 0;
}
