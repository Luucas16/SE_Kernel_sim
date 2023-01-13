#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

extern struct PCB *rootpcb; /*PCB-aren lehenengo osagaia*/
extern struct PCB *azkenapcb; /*PCB-aren azkeneko osagaia*/

extern int estrategi_zenb; /*Erabiliko den esatrategia: 1-> FIFO 2->RoundRobin*/
extern int done; /*Timerrak kontatzeko*/
extern long tik; /*Loader-en erabili erlojuaren seinaleak kontatzeko*/ 
extern long tuk; /*Scheduler-an erabili erlojuaren seinaleak kontatzeko*/
extern long tid; /*Prozesuak sortzerakoan tid-a gordetzeko*/
extern long tam; /*PCB struct-aren tamaina gordetzeko eta gero memoria reserbatzean erabili*/

extern int processqueue_tam; /*Processqueue-aren momentuko tamaina*/
extern pthread_mutex_t mutex_cl; /*Erlojuaren eta timerraren mutexa*/
extern pthread_mutex_t mutex_pg; /*Timerraren eta Loader-aren mutexa*/
extern pthread_mutex_t mutex_sc; /*Timerra eta Scheduler-aren mutexa*/
extern pthread_cond_t cond; /*Timerrak eta Erlojua artean erabili*/
extern pthread_cond_t cond2; /*Timerrak eta Erlojua artean erabili*/

extern sem_t semaforo_pg; /*Loader-aren eta Loader-Timer-aren semaforoa*/
extern sem_t semaforo_sc; /*Scheduler eta Scheduler-Timer-aren semaforoa*/

extern pthread_t threads[50]; /*Prozesuak sortzeko harien array bat*/
