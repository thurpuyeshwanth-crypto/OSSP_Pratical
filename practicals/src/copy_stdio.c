#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 8192

int main(int argc, char **argv)
{
    if (argc != 3) { fprintf(stderr, "Usage: %s <source> <destination>\n", argv[0]); return EXIT_FAILURE; }
    FILE *src = fopen(argv[1], "rb");
    if (src == NULL) { perror("fopen source"); return EXIT_FAILURE; }
    FILE *dst = fopen(argv[2], "wb");
    if (dst == NULL) { perror("fopen destination"); fclose(src); return EXIT_FAILURE; }

    char buffer[BUFFER_SIZE];
    size_t n;
    int failed = 0;
    while ((n = fread(buffer, 1, sizeof(buffer), src)) > 0) {
        size_t offset = 0;
        while (offset < n) {
            size_t written = fwrite(buffer + offset, 1, n - offset, dst);
            if (written == 0) { perror("fwrite"); failed = 1; break; }
            offset += written;
        }
        if (failed) break;
    }
    if (ferror(src)) { perror("fread"); failed = 1; }
    if (fseek(src, 0, SEEK_END) == 0) {
        long size = ftell(src);
        if (size >= 0) printf("Source file size: %ld bytes\n", size);
    }
    if (fclose(src) != 0) { perror("fclose source"); failed = 1; }
    if (fclose(dst) != 0) { perror("fclose destination"); failed = 1; }
    if (failed) return EXIT_FAILURE;
    puts("Standard-library copy complete.");
    return EXIT_SUCCESS;
}
