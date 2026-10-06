#include <stdio.h>
#include <stdlib.h>

int global_initialized = 10;
int global_uninitialized;

static void show_code_address(void)
{
    printf("Code address       : %p\n", (void *)show_code_address);
}

int main(void)
{
    static int static_initialized = 20;
    int stack_value = 30;
    int *heap_value = malloc(sizeof(*heap_value));
    if (heap_value == NULL) { perror("malloc"); return EXIT_FAILURE; }
    *heap_value = 40;

    puts("Linux process address-space sample (addresses vary due to ASLR):");
    show_code_address();
    printf("Initialized global : %p\n", (void *)&global_initialized);
    printf("Static variable    : %p\n", (void *)&static_initialized);
    printf("BSS global         : %p\n", (void *)&global_uninitialized);
    printf("Heap allocation    : %p\n", (void *)heap_value);
    printf("Stack local        : %p\n", (void *)&stack_value);
    free(heap_value);
    return EXIT_SUCCESS;
}
