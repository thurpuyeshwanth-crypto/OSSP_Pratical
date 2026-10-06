#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int global_value = 100;
int global_zero;
static int static_value = 200;

int main(void)
{
    int stack_value = 300;
    int *heap_value = malloc(sizeof(*heap_value));
    if (heap_value == NULL) { perror("malloc"); return EXIT_FAILURE; }
    *heap_value = 400;
    printf("PID: %ld\nCode: %p\nGlobal: %p\nStatic: %p\nBSS: %p\nHeap: %p\nStack: %p\n",
           (long)getpid(), (void *)main, (void *)&global_value, (void *)&static_value,
           (void *)&global_zero, (void *)heap_value, (void *)&stack_value);
    puts("Inspect /proc/<PID>/maps, /proc/<PID>/status, or use pmap <PID> now.");
    fflush(stdout);
    sleep(45);
    free(heap_value);
    return EXIT_SUCCESS;
}
