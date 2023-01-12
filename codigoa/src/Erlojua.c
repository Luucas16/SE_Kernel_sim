#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include "../include/aldagai_generalak.h"

int done;
pthread_mutex_t mutex_cl;
pthread_cond_t cond, cond2;

void *Erlojua()
{
    done = 0;
    while (1)
    {
        pthread_mutex_lock(&mutex_cl);
        while (done < 2)
        {

            pthread_cond_wait(&cond, &mutex_cl);
        }
        done = 0;
        pthread_cond_broadcast(&cond2);
        pthread_mutex_unlock(&mutex_cl);
    }
}
