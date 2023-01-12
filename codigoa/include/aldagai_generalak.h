#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>




//extern struct PCB *processqueue[50];
extern struct PCB *rootpcb;
extern struct PCB *azkenapcb;

extern int estrategi_zenb;
extern int done;
extern long tik;
extern long tok;
extern long tak;
extern long tuk; 
extern long tid; 
extern long max; 
extern long tam;

extern long processqueue_tam;
extern pthread_mutex_t mutex_cl; /*Erlojuaren eta timerraren mutexa*/
extern pthread_mutex_t mutex_pg; /*Timerraren eta Prozesu sortzailearen mutexa*/
extern pthread_mutex_t mutex_sc; /*Timerra eta Planifikatzaileraren mutexa*/
extern pthread_cond_t cond;
extern pthread_cond_t cond2;
extern pthread_cond_t cond3;
extern pthread_cond_t cond4;

extern sem_t semaforo_pg;
extern sem_t semaforo_pg1;
extern sem_t semaforo_sc;
extern sem_t semaforo_sc1;

extern pthread_t threads[50];
