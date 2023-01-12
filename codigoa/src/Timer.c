#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include "../include/aldagai_generalak.h"

long tik, tuk = 0;
sem_t semaforo_pg, semaforo_sc;

void *Tenporizadorea_pg(long *frekuentzia)
{
    pthread_mutex_lock(&mutex_cl);

    while (1)
    {
        done++;
        tik++;

        if (tik == *frekuentzia)
        {
            sem_post(&semaforo_pg);

            tik = 0;
        }

        pthread_cond_signal(&cond);
        pthread_cond_wait(&cond2, &mutex_cl);
    }
}

void *Tenporizadorea_sch(long *frekuentzia)
{

    pthread_mutex_lock(&mutex_cl);

    while (1)
    {

        done++;
        tuk++;

        if (tuk == *frekuentzia)
        {

            sem_post(&semaforo_sc);

            tuk = 0;
        }

        pthread_cond_signal(&cond);
        pthread_cond_wait(&cond2, &mutex_cl);
    }
}
