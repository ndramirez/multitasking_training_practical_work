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
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include "msg.h"

#define BUFF_SIZE   4           /* total number of slots */
#define NP          4           /* total number of producers */
#define NC          1           /* total number of consumers */

unsigned int followed_element = 0;

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
    int index, currentIn, i, err;
    MSG_BLOCK item; // new MSG_BLOCK to produce

    for (i=0; i < 4; i++)
    {

        /* Produce item */
        currentIn = shared.in;
        MessageFill(&item, idP); // fill the correspondent item
        if((err = MessageDisplay(&item))) continue; // go to next iteration if the ckecksum isn't validated
        printf("[Producer %d] Producing item in Slot %d of shared memory\n", (int)arg, currentIn);
        fflush(stdout);

        /* If there are no empty slots, wait */
        sem_wait(shared.SL);
        /* If another thread is writing/reading from memory, wait */
        pthread_mutex_lock(&shared.SM);
        shared.shared_memory[shared.in] = item;
        shared.in = (shared.in+1)%BUFF_SIZE;
        /* Release the buffer */
        pthread_mutex_unlock(&shared.SM);
        /* Increment the number of full slots */
        sem_post(shared.SO);

        /* Interleave  producer and consumer execution */
        if (i % 2 == 1) sleep(1);
        // sleep(1);
    }
    pthread_exit(0);
    //return NULL;
}

void *Consumer(void *arg)
{
    int i, currentOut;
    MSG_BLOCK item, addBlock;

    for (i=0; i < 16; i++) {
        /* If there are no full slots, wait */
        sem_wait(shared.SO);

        pthread_mutex_lock(&shared.SM);

        currentOut = shared.out;
        item = shared.shared_memory[shared.out];
        shared.out = (shared.out+1)%BUFF_SIZE;
        printf("[Consumer %d] Consuming item from slot %d in shared memory\n", (int)arg, currentOut);
        fflush(stdout);
        /* Release the buffer */
        pthread_mutex_unlock(&shared.SM);
        /* Increment the number of full slots */
        sem_post(shared.SL);


        /* Add inputs */
        MessageAddition(&addBlock, &item);

        /* Diag Out */
        printf("[Control Message] %d inputs Added, %d inputs Left\n", currentOut+1, BUFF_SIZE - (currentOut+1));
        if (currentOut == (BUFF_SIZE-1)) {
            printf("\n[Control Message] ********* 4 vectors added: reinitializing the addition operation *********\n");
            MessageReset(&addBlock);
        }


        /* Interleave  producer and consumer execution */
    }
    pthread_exit(0);
    //return NULL;
}

int main()
{
    if(sem_unlink("/full")){
      printf("semaphore S.O. wasn't linked\n");
    }
    if(sem_unlink("/empty")){
     printf("semaphore S.L. wasn't linked\n\n");
    }
    int producer_index, consumer_index;

    /*Using unnamed semaphores*/
    // sem_init(shared.SO, 0, 0);
    // sem_init(shared.SL, 0, BUFF_SIZE);
    printf("Each vector produced has 256 element between 1 and 100\n");
    fflush(stdout);
    printf("The resulting vector is a sum (element by element) from vectors produced by each thread [1 - 4]\n");
    fflush(stdout);
    printf("When the last vector (4th) is added, the result vector is reinitialized\n");
    fflush(stdout);
    printf("The user can follow an specific element in the resulting vector\n");
    fflush(stdout);
    printf("\nWhich vector element do you want to follow its value ? [1 - 255]\n");
    fflush(stdout);
    scanf("%d", &followed_element);
    printf("Start\n\n");
    fflush(stdout);
    shared.SO = sem_open("/full", O_CREAT, 0644, 0);           /* keep track of the number of full spots */
    shared.SL = sem_open("/empty", O_CREAT, 0644, BUFF_SIZE);
    pthread_mutex_init(&shared.SM, NULL);

    /* Create NP producers  */
    for (producer_index = 0; producer_index < NP; producer_index++){
      pthread_create(&idP, NULL, Producer, (void*)producer_index);
    }
    /*Create Consumer*/
    consumer_index = 1;
    pthread_create(&idC, NULL, Consumer, (void*)consumer_index);

    //pthread_exit(NULL);
    (void)pthread_join(idP, NULL);
    (void)pthread_join(idC, NULL);
    sem_close(shared.SO);
    sem_close(shared.SL);
    sem_unlink("/full");
    sem_unlink("/empty");
    //pthread_exit(NULL);
    return(0);
}
