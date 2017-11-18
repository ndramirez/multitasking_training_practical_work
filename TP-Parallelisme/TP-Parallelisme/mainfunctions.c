#include "mainfunctions.h"
extern sbuf_t shared;
void *Producer(void *arg)
{
    int i, item, index, currentIn, sem_value, getResult;

    index = (int)arg;


    for (i=0; i < NITERS; i++)
    {

        /* Produce item */
        item = i;

        /* Prepare to write item to buf */

        /* If there are no empty slots, wait */
        // sem_wait(&shared.empty);
        printf("value of shared: %#07x \n", shared);
        sem_wait(shared.empty);
        printf("Sem empty...\n");

        /* If another thread uses the buffer, wait */
        pthread_mutex_lock(&shared.mutex);
        shared.buf[shared.in] = item;
        shared.in = (shared.in+1)%BUFF_SIZE;
        printf("[P%d] Producing %d in Slot-> %d...\n", index, item, shared.in - 1);
        // getResult = sem_getvalue(shared.empty, &sem_value);
        // printf("Result of GetValue(empty): %d \n", getResult);
        // printf("Status of Sem EMPTY: %d \n", sem_value);
        fflush(stdout);
        /* Release the buffer */
        pthread_mutex_unlock(&shared.mutex);
        /* Increment the number of full slots */
        sem_post(shared.full);

        /* Interleave  producer and consumer execution */
        if (i % 2 == 1) sleep(1);
    }
    return NULL;
}

void *Consumer(void *arg)
{
    int i, item, index;

    index = (int)arg;
    for (i=NITERS; i > 0; i--) {
        sem_wait(&shared.full);
        pthread_mutex_lock(&shared.mutex);
        item=i;
        item=shared.buf[shared.out];
        shared.out = (shared.out+1)%BUFF_SIZE;
        printf("[C%d] Consuming  %d from Slot -> %d...\n", index, item, shared.out - 1);
        fflush(stdout);
        /* Release the buffer */
        pthread_mutex_unlock(&shared.mutex);
        /* Increment the number of full slots */
        sem_post(&shared.empty);

        /* Interleave  producer and consumer execution */
        if (i % 2 == 1) sleep(1);
    }
    return NULL;
}