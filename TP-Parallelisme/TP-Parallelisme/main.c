//
//  main.c
//  TP-Parallelisme
//
//  Created by NelsonRamirez and LuisPerezBustos on 18/11/2017.
//  Copyright © 2017 Nelson - Luis. All rights reserved.
//

#include <stdlib.h>
#include "mainfunctions.h"

#define NP          3           /* total number of producers */
#define NC          3           /* total number of consumers */

sbuf_t shared;


int main()
{
    pthread_t idP, idC;
    int index, semValue;
    sem_t s;

    shared.full = sem_open("full", O_CREAT, 0644, 0);           /* keep track of the number of full spots */
    shared.empty = sem_open("empty", O_CREAT, 0644, BUFF_SIZE);           /* keep track of the number of full spots */

    // sem_init(&shared.full, 0, 0);
    // sem_init(&shared.empty, 0, BUFF_SIZE);
    // sem_init(&s, 0, 10);
    // int res = sem_getvalue(&s, &semValue);
    // printf("Result of GetValue(empty): %d \n", res);
    printf("value of shared: %#07x \n", shared);

    pthread_mutex_init(&shared.mutex, NULL);
    for (index = 0; index < NP; index++)
    {
         // Create a new producer
        pthread_create(&idP, NULL, Producer, (void*)index);
    }
    /*create a new Consumer*/
    // for(index=0; index<NC; index++)
    // {
    //     pthread_create(&idC, NULL, Consumer, (void*)index);
    // }



    pthread_exit(NULL);
}
