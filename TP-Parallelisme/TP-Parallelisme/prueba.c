#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
int main(int argc, char *argv[])
{
    sem_t *semaphore;
    int semValue;
if ((semaphore = sem_open("/semaphore", O_CREAT, 0644, 1)) == SEM_FAILED ) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }
    // sem_init(semaphore, 0, 10);

    sem_wait(semaphore);
    int res = sem_getvalue(semaphore, &semValue);
    printf("Result of GetValue(empty): %d \n", res);
    printf("value of empty: %d \n", semValue);
    sem_post(semaphore);
if (sem_close(semaphore) == -1) {
        perror("sem_close");
        exit(EXIT_FAILURE);
    }
if (sem_unlink("/semaphore") == -1) {
        perror("sem_unlink");
        exit(EXIT_FAILURE);
    }
puts("Done");
    exit(EXIT_SUCCESS);
}
