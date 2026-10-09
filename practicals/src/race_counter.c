#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define NUM_THREADS 4
#define INCREMENTS 1000000

/* Shared variable */
long long counter = 0;

/* Thread function */
void *increment_counter(void *arg)
{
    (void)arg;
    for (long long i = 0; i < INCREMENTS; i++)
    {
        counter++;
    }
    return NULL;
}

int main(void)
{
    pthread_t threads[NUM_THREADS];
    clock_t start, end;

    start = clock();

    /* Create threads */
    for (int i = 0; i < NUM_THREADS; i++)
    {
        if (pthread_create(&threads[i], NULL, increment_counter, NULL) != 0)
        {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }

    /* Wait for all threads */
    for (int i = 0; i < NUM_THREADS; i++)
    {
        if (pthread_join(threads[i], NULL) != 0)
        {
            perror("pthread_join");
            exit(EXIT_FAILURE);
        }
    }

    end = clock();

    printf("Expected counter value: %lld\n", (long long)NUM_THREADS * INCREMENTS);
    printf("Actual counter value:   %lld\n", counter);
    printf("Execution time:         %.6f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);

    return 0;
}
