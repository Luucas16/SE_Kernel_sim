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
 //pthread_mutex_t mutex_cl;
 //pthread_cond_t cond, cond2;
 //sem_t semaforo_pg, semaforo_sc;


int main(int argc, char *argv[])
{
    //processqueue = (struct PCB*)malloc(tam);
    /*Erlojuaren zenbat seinalero egin +1 Timerran*/
    long freq = strtol(argv[1], NULL, 10);
    /*Timerraren zenbat seinalero egin +1 Prozesu sortzailean*/
    long freq_pg = strtol(argv[2], NULL, 10);
    /*Timerraren zenbat seinalero egin +1 Schedulerran*/
    long freq_sc = strtol(argv[3], NULL, 10);

    /*Hariak erazagutu*/
   


    /*Kondizioak eta Mutex inizializatu*/
    pthread_mutex_init(&mutex_cl, NULL);
    
    pthread_cond_init(&cond, NULL);
    pthread_cond_init(&cond2, NULL);

    sem_init(&semaforo_sc,1,0);
    sem_init(&semaforo_pg,1,0);

     /*Erlojua hasi*/
    pthread_create(&h1, NULL, (void *)Erlojua, NULL);
    //printf("a");
    /*Prozesu sortzailearen Timerraren haria hasi*/
    pthread_create(&h2, NULL, (void *)Tenporizadorea_pg, &freq_pg);
   // printf("b");
    /*Schedulerraren Timerraren haria hasi*/
    pthread_create(&h3, NULL, (void *)Tenporizadorea_sch, &freq_sc);
   // printf("c");
    /*Prozesu sortzailearen haria hasi*/
    pthread_create(&h4, NULL, (void *)Loader, NULL);
    //printf("d");
    /*Schedulerraren haria hasi*/
    pthread_create(&h5, NULL, (void *)Scheduler, NULL);
    //printf("f");
    /*Hariek leno zuten harira itzuli*/
    pthread_join(h1, NULL);
    pthread_join(h2, NULL);
    pthread_join(h3, NULL);
    pthread_join(h4, NULL);
    pthread_join(h5, NULL);

    /*Mutex-ak bukarazi*/
    pthread_mutex_destroy(&mutex_cl);
    

    sem_destroy(&semaforo_pg);
    sem_destroy(&semaforo_sc);
    

    //free(processqueue);
}
