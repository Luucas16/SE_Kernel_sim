#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include "../include/aldagai_generalak.h"
#include "../include/Scheduler.h"



long tok = 0; 
long tid ;
pthread_t threads[];



int Prozesuak_egin_behar_duena()
{

    return 1;
}

void *Loader(long *lag)
{
    
  int i=0;
   
    while (1)
    { 
      //for (int i = 0; i < 20; i++)
      //{
           
	  
                sem_wait(&semaforo_pg);
	     
		//if (processqueue_tam<50)
                //{

		if(i<50){
                        
		pthread_create(&threads[i], NULL, (void *)Prozesuak_egin_behar_duena, NULL);
              
               
		tid = (long)threads[i];
               
                printf("LOADER: %ld \n",tid);
                fflush(stdout);
                sartu_Pcb_eta_processqueuean(tid);
               
		// }
                   
		i++;

		}else{
                    i=0;   
                }
               // if (i==50)
                //{
                 //       i=0;
                
                
		//sem_post(&semaforo_pg);
		//printf("LOADERRAK DESAKTIBATUTA\n");
		//}
    }
}

