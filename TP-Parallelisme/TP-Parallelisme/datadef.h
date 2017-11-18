#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#define BUFF_SIZE   5           /* total number of slots */

typedef struct sbuf_t_tag
{
    int buf[BUFF_SIZE];   /* shared var */
    int in;               /* buf[in%BUFF_SIZE] is the first empty slot */
    int out;              /* buf[out%BUFF_SIZE] is the first full slot */
    sem_t *full;           /* keep track of the number of full spots */
    // sem_t full = sem_open("full", O_CREAT);           /* keep track of the number of full spots */

    sem_t *empty;          /* keep track of the number of empty spots */
    // sem_t empty = sem_open("empty", O_CREAT);           /* keep track of the number of full spots */

    // use correct type here
    pthread_mutex_t mutex;          /* enforce mutual exclusion to shared data */
} sbuf_t;

