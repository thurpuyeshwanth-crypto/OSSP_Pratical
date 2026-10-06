#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int *values = malloc(5 * sizeof(*values));
    int *zeroed = calloc(5, sizeof(*zeroed));
    if (values == NULL || zeroed == NULL) {
        perror("malloc/calloc"); free(values); free(zeroed); return EXIT_FAILURE;
    }
    for (int i = 0; i < 5; i++) values[i] = (i + 1) * 10;
    printf("malloc values: ");
    for (int i = 0; i < 5; i++) printf("%d ", values[i]);
    printf("\ncalloc values: ");
    for (int i = 0; i < 5; i++) printf("%d ", zeroed[i]);

    int *grown = realloc(values, 10 * sizeof(*values));
    if (grown == NULL) { perror("realloc"); free(values); free(zeroed); return EXIT_FAILURE; }
    values = grown;
    for (int i = 5; i < 10; i++) values[i] = (i + 1) * 10;
    printf("\nAfter realloc: ");
    for (int i = 0; i < 10; i++) printf("%d ", values[i]);
    puts("\nFreeing allocations.");
    free(values); free(zeroed);
    return EXIT_SUCCESS;
}
