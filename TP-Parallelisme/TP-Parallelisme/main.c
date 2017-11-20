//
//  main.c
//  TP-Parallelisme
//
//  Created by NelsonRamirez and LuisPerezBustos on 18/11/2017.
// #include <pthread.h>
// #include <stdio.h>
#include <stdlib.h>
#include </usr/include/semaphore.h>
#include <unistd.h> // sleep function
#include "msg.h"

#define BUFF_SIZE   4           /* total number of slots */
#define NP          4           /* total number of producers */
#define NC          1           /* total number of consumers */

typedef struct
{
    MSG_BLOCK         shared_memory[BUFF_SIZE];   /* shared var */
    int               in;                         /* buf[in%BUFF_SIZE] is the first empty slot */
    int               out;                        /* buf[out%BUFF_SIZE] is the first full slot */
    sem_t             *SO;                        /* keep track of the number of full spots */
    sem_t             *SL;                        /* keep track of the number of empty spots */
    pthread_mutex_t   SM;                        /* Semaphore Memoire: enforce mutual exclusion to shared data */
} sbuf_t;

sbuf_t shared;


void *Producer(void *arg)
{
    int index, currentIn, i;
    MSG_BLOCK item;

    for (i=0; i < 4; i++)
    {

        /* Produce item */
        MessageFill(&item, (pthread_t)arg);

        /* Prepare to write item to buf (checksum)*/

        /* If there are no empty slots, wait */
        // sem_wait(shared.SL);
        sem_wait(shared.SL);
        printf("CrSection\n");

        /* If another thread uses the buffer, wait */
        pthread_mutex_lock(&shared.SM);
        shared.shared_memory[shared.in] = item;
        currentIn = shared.in;
        shared.in = (shared.in+1)%BUFF_SIZE;
        printf("[P%d] Producing item: %d in Slot-> %d...\n", index, item.checksum, currentIn);
        //getResult = sem_getvalue(shared.SL, &sem_value);
        //printf("Status of Sem EMPTY: %d \n", sem_value);
        fflush(stdout);
        /* Release the buffer */
        pthread_mutex_unlock(&shared.SM);
        /* Increment the number of full slots */
        sem_post(shared.SO);

        /* Interleave  producer and consumer execution */
        if (i % 2 == 1) sleep(1);
        // sleep(1);
    }
    return NULL;
}

void *Consumer(void *arg)
{
    int i, currentOut, cRead = 0;
    MSG_BLOCK item, *addBlock;

    for (i=8; i > 0; i--) {


        /* If there are no full slots, wait */
        sem_wait(shared.SO);

        pthread_mutex_lock(&shared.SM);

        item = shared.shared_memory[shared.out];
        shared.out = (shared.out+1)%BUFF_SIZE;
        printf("[Additioneur] Consuming item: %d from Slot -> %d...\n", item.checksum, currentOut);
        //(void)sem_getvalue(shared.SO, &sem_value2);
        //printf("Status of Sem FULL: %d \n", sem_value2);
        fflush(stdout);
        /* Release the buffer */
        pthread_mutex_unlock(&shared.SM);
        /* Increment the number of full slots */
        sem_post(shared.SL);


        /* Add inputs */
        //MessageAddition(addBlock, item);

        /* Diag Out */
        cRead++;
        printf("[Control] %d inputs Added, %d inputs Left\n", cRead, BUFF_SIZE - cRead);
        if (cRead == BUFF_SIZE) {
            cRead = 0;
            // MessageReset(addBlock);
        }


        /* Interleave  producer and consumer execution */
    }
    return NULL;
}

int main()
{
    pthread_t idP, idC;
    int index;

    /*Using unnamed semaphores*/
    // sem_init(shared.SO, 0, 0);
    // sem_init(shared.SL, 0, BUFF_SIZE);
    printf("Start\n");
    shared.SO = sem_open("/full", O_CREAT, 0644, 0);           /* keep track of the number of full spots */
    shared.SL = sem_open("/empty", O_CREAT, 0644, BUFF_SIZE);
    pthread_mutex_init(&shared.SM, NULL);

    /* Create NP producers  */
    // for (index = 0; index < NP; index++)
    // {
        pthread_create(&idP, NULL, Producer, (void*)idP);
    // }
    /*Create Consumer*/
    // pthread_create(&idC, NULL, Consumer, (void*)idC);

    (void)pthread_join(idP, NULL);
    sem_close(shared.SO);
    sem_close(shared.SO);
    sem_unlink("/full");
    sem_unlink("/empty");
}
