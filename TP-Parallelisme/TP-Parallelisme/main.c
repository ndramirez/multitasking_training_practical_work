//
//  main.c
//  TP-Parallelisme
//
//  Created by NelsonRamirez and LuisPerezBustos on 18/11/2017.
//  Copyright © 2017 Nelson - Luis. All rights reserved.
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include </usr/include/semaphore.h>
#include <unistd.h> // sleep function

#include "msg.h"

#define BUFF_SIZE   4           /* total number of slots */
#define NP          4           /* total number of producers */
#define NC          1           /* total number of consumers */

typedef struct
{
    MSG_BLOCK shared_memory[BUFF_SIZE];   /* shared var */
    int in;               /* buf[in%BUFF_SIZE] is the first empty slot */
    int out;              /* buf[out%BUFF_SIZE] is the first full slot */
    sem_t SO;           /* keep track of the number of full spots */
    sem_t SL;          /* keep track of the number of empty spots */
    pthread_mutex_t SM;          /* enforce mutual exclusion to shared data */
} sbuf_t;

sbuf_t shared;

void *Producer(void *arg)
{
    int index, currentIn;
    MSG_BLOCK item;
    thread_id = (pthread_t)arg;

    MessageFill(&item, pthread_t threadId)

    for (i=0; i < NITERS; i++)
    {

        /* Produce item */
        item = i;

        /* Prepare to write item to buf */

        /* If there are no empty slots, wait */
        // sem_wait(&shared.empty);
        sem_wait(&shared.empty);
        /* If another thread uses the buffer, wait */
        pthread_mutex_lock(&shared.mutex);
        shared.buf[shared.in] = item;
        shared.in = (shared.in+1)%BUFF_SIZE;
        printf("[P%d] Producing %d in Slot-> %d...\n", index, item, shared.in - 1);
        //getResult = sem_getvalue(&shared.empty, &sem_value);
        //printf("Status of Sem EMPTY: %d \n", sem_value);
        fflush(stdout);
        /* Release the buffer */
        pthread_mutex_unlock(&shared.mutex);
        /* Increment the number of full slots */
        sem_post(&shared.full);

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
        //(void)sem_getvalue(&shared.full, &sem_value2);
        //printf("Status of Sem FULL: %d \n", sem_value2);
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

int main()
{
    pthread_t idP, idC;
    int index, semValue;

    sem_init(&shared.full, 0, 0);
    sem_init(&shared.empty, 0, BUFF_SIZE);

    /* Create a new producer */
    pthread_mutex_init(&shared.mutex, NULL);
     for (index = 0; index < NP; index++)
     {
          pthread_create(&idP, NULL, Producer, (void*)idP);
     }
    /*create a new Consumer*/
     for(index=0; index<NC; index++)
     {
         pthread_create(&idC, NULL, Consumer, (void*)idC);
     }

    pthread_exit(NULL);
}
