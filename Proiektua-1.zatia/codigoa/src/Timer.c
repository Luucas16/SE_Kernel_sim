#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include "../include/aldagai_generalak.h"

long tik,tuk=0;
sem_t semaforo_pg, semaforo_pg1, semaforo_sc, semaforo_sc1;

void *Tenporizadorea_pg(long *frekuentzia) // 100000
{

   
  pthread_mutex_lock(&mutex_cl);

    
    while (1)
    {
        done++;
        tik++;
	
	//fflush(stdout);
	if (tik == *frekuentzia)
        { //printf("PG %ld\n",tik);
	  //printf("TIK PG\n");
	  sem_post(&semaforo_pg);
           
	   
	  //sem_wait(&semaforo_pg);
	    //printf("AURRERA TENP\n");

            tik = 0;
        }

        pthread_cond_signal(&cond);
        pthread_cond_wait(&cond2, &mutex_cl);
    }
}

void *Tenporizadorea_sch(long *frekuentzia) // 100000
{
  
   
    pthread_mutex_lock(&mutex_cl);

    while (1)
    {

        done++;
        tuk++;

	
        if (tuk == *frekuentzia)
        {
	  //printf("SCH %ld\n",tuk);
            //printf("TIK SCH\n");
           
            //fflush(stdout); 
            sem_post(&semaforo_sc);
            
            //sem_wait(&semaforo_sc);
           
            tuk = 0;
        }
        

        pthread_cond_signal(&cond);
        pthread_cond_wait(&cond2, &mutex_cl);
    }
}
