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
} SHARED_INFO;

SHARED_INFO shared;

void *Producer(void *arg)
{
    int index, currentIn, i, err;
    MSG_BLOCK item; // new MSG_BLOCK to produce

    for (i=0; i < 4; i++)
    {

        /* Simulation time */
        /* Produce a new item */
        MessageFill(&item, idP); // fill the correspondent item
        if((err = MessageDisplay(&item))) continue; // go to next iteration if the ckecksum isn't validated
        fflush(stdout);

        /* If there are no empty slots, wait */
        sem_wait(shared.SL);

        /********** Start of Critical Section **********/
        /* If another thread is writing/reading from memory, wait */
        pthread_mutex_lock(&shared.SM);
        currentIn = shared.in;
        printf("[Producer %d] Producing item in Slot %d of shared memory\n", (int)arg, currentIn);
        shared.shared_memory[shared.in] = item;
        shared.in = (shared.in+1)%BUFF_SIZE;
        /* Release the shared memory mutex */
        pthread_mutex_unlock(&shared.SM);
        /********** End of Critical Section **********/

        /* Increment the number of full slots */
        sem_post(shared.SO);

        /* go to sleep for 1 second if you want a slower execution */
        sleep(1);
    }
    // pthread_exit(0);
}

void *Consumer(void *arg)
{
    int i, currentOut, waitingTime;
    MSG_BLOCK item, addBlock; // new MSG BLOCK to add and resulted MSG BLOCK

    /* consume 4 messages of each thread (4 threads) => 16 MSG BLOCKS */
    for (i=0; i < 16; i++) {

        /*
         * Simulation of event trigger:
         * Each iteration it waits for a random time
         * between 1 and 5 seconds to read.
         */
        waitingTime = rand() % 2 + 1;
        sleep(waitingTime);

        fflush(stdout);
        /* If there are no full slots, wait */
        sem_wait(shared.SO);

        /********** Start of Critical Section **********/
        /* If another thread is writing/reading from memory, wait */
        pthread_mutex_lock(&shared.SM);
        currentOut = shared.out;
        printf("[Consumer %d] Consuming item from slot %d in shared memory\n", (int)arg+1, currentOut);
        /* consume the item */
        item = shared.shared_memory[shared.out];
        shared.out = (shared.out+1)%BUFF_SIZE;
        printf("[Control Message] %d inputs Added, %d inputs Left\n", currentOut+1, BUFF_SIZE - (currentOut+1));

        /* Release the shared memory mutex */
        pthread_mutex_unlock(&shared.SM);
        //********** End of Critical Section **********/
        /* Increment the number of empty slots */
        sem_post(shared.SL);

        /* add new item to resulted vector */
        MessageAddition(&addBlock, &item);
        /* if the 4th vector is added, reinitialize */
        if (currentOut == (BUFF_SIZE-1)) {
            printf("\n[Control Message] ********* 4 vectors added: reinitializing the addition operation *********\n");
            MessageReset(&addBlock);
        }
    }
    // pthread_exit(0);
}

int main()
{   /* unlink named semaphores if they were not well closed in the last execution */
    if(sem_unlink("/full")){
      printf("semaphore S.O. wasn't linked\n"); // last execution was O.K.
    }
    if(sem_unlink("/empty")){
     printf("semaphore S.L. wasn't linked\n\n"); // last execution was O.K.
    }
    /* 4 producers and 1 consumer */
    int producer_index, consumer_index;

    /*Initializing rand function seed*/
    srand(time(NULL));

    /* Some explanation of the algortithm */
    printf("Each vector produced has 256 element between 1 and 100\n");
    fflush(stdout);
    printf("The resulting vector is a sum (element by element) from vectors produced by each thread [1 - 4]\n");
    fflush(stdout);
    printf("When the last vector (4th) is added, the result vector is reinitialized\n");
    fflush(stdout);
    printf("The user can follow an specific element in the resulting vector\n");
    fflush(stdout);
    /* choose the element that the user can follow during the execution */
    printf("\nWhich vector element do you want to follow its value ? [0 - 255]\n");
    fflush(stdout);
    scanf("%d", &followed_element);
    /* start the execution */
    printf("Start\n\n");
    fflush(stdout);
    shared.SO = sem_open("/full", O_CREAT, 0644, 0); /* keep track of the number of full spots */
    shared.SL = sem_open("/empty", O_CREAT, 0644, BUFF_SIZE); /* keep track of the number of empty spots */
    pthread_mutex_init(&shared.SM, NULL); /* memory access (shared resource) */

    /* Create NP producers  */
    for (producer_index = 0; producer_index < NP; producer_index++){
      pthread_create(&idP, NULL, Producer, (void*)producer_index);
    }

    /* Create NC Consumer */
    for (consumer_index = 0; consumer_index < NC; consumer_index++){
      pthread_create(&idC, NULL, Consumer, (void*)consumer_index);
    }

    /* wait to threads to complet */
    (void)pthread_join(idP, NULL);
    (void)pthread_join(idC, NULL);
    /* after complete thread execution, close and unlink them */
    sem_close(shared.SO);
    sem_close(shared.SL);
    sem_unlink("/full");
    sem_unlink("/empty");

    return(0);
}
