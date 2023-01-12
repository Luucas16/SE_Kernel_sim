#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include "../include/aldagai_generalak.h"
#include "../include/Erlojua.h"
#include "../include/Loader.h"
#include "../include/Timer.h"
#include "../include/Scheduler.h"

pthread_t h1, h2, h3, h4, h5;
int estrategi_zenb;

int main(int argc, char *argv[])
{
    printf("Idatzi 1 FIFO estrategiarentzat edo 2 RoundRobin estrategiarentzat:");
    scanf("%d", &estrategi_zenb);

    /*Timerraren zenbat seinalero egin +1 Prozesu sortzailean*/
    long freq_pg = strtol(argv[1], NULL, 10);

    /*Timerraren zenbat seinalero egin +1 Schedulerran*/
    long freq_sc = strtol(argv[2], NULL, 10);

    /*Kondizioak, Mutex eta Semaforoak inizializatu*/
    pthread_mutex_init(&mutex_cl, NULL);

    pthread_cond_init(&cond, NULL);
    pthread_cond_init(&cond2, NULL);

    sem_init(&semaforo_sc, 1, 0);
    sem_init(&semaforo_pg, 1, 0);

    /*Erlojua hasi*/
    pthread_create(&h1, NULL, (void *)Erlojua, NULL);
    
    /*Prozesu sortzailearen Timerraren haria hasi*/
    pthread_create(&h2, NULL, (void *)Tenporizadorea_pg, &freq_pg);
    
    /*Schedulerraren Timerraren haria hasi*/
    pthread_create(&h3, NULL, (void *)Tenporizadorea_sch, &freq_sc);
    
    /*Prozesu sortzailearen haria hasi*/
    pthread_create(&h4, NULL, (void *)Loader, NULL);
    
    /*Schedulerraren haria hasi*/
    pthread_create(&h5, NULL, (void *)Scheduler, NULL);
    
    /*Hariek leno zuten harira itzuli*/
    pthread_join(h1, NULL);
    pthread_join(h2, NULL);
    pthread_join(h3, NULL);
    pthread_join(h4, NULL);
    pthread_join(h5, NULL);

    /*Mutex-ak bukarazi*/
    pthread_mutex_destroy(&mutex_cl);

    /*Semaforoak bukarazi*/
    sem_destroy(&semaforo_pg);
    sem_destroy(&semaforo_sc);

   
}
