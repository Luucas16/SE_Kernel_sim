#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include "../include/aldagai_generalak.h"

struct PCB
{
    long tid;
    struct PCB *hurrengoa;
};

struct CPU
{
    int CPU_Numb;
    struct CORE *core;
    struct CPU *hurrengoa;
};

struct CORE
{
    int core_Numb;
    struct HARI *hari;
    struct CORE *hurrengoa;
};

struct HARI
{
    int tid;
    struct HARI *hurrengoa;
};

struct PCB *rootpcb = NULL;
struct PCB *azkenapcb = NULL;
long processqueue_tam = 0;
long tak = 0;
long tam = sizeof(struct PCB);
struct PCB processqueue[1000];
// int *ptr=processqueue;

int hutsik()
{
    if (rootpcb == NULL)

        return 1;
    else
        return 0;
}

void sartu_Pcb_eta_processqueuean(long x)
{
    // struct PCB_LIST *berria;
    // struct PROCESSQUEUE *proces;
    // berria = malloc(sizeof(struct PCB_LIST));
    // proces = malloc(sizeof(struct PROCESSQUEUE));
    struct PCB berria;
    // printf("HASIERA\n");
    berria.tid = x;
    // printf("%ld",berria.tid);

    berria.hurrengoa = NULL;

    // printf("BUK\n");

    if (hutsik())
    { // printf("IF\n");

        rootpcb = &berria;
        azkenapcb = &berria;

        // printf("%p %p %ld", rootpcb, azkenapcb, processqueue[0].tid);
    }
    else if (&rootpcb == &azkenapcb)
    {
        // printf("ELSIF\n");

        rootpcb->hurrengoa = &berria;
        azkenapcb = &berria;

        // printf("%p %p\n",processqueue[0],processqueue[1]);
    }
    else
    {

        azkenapcb->hurrengoa = &berria;
        azkenapcb = &berria;
        // printf("ESLE\n");
        // fflush(stdout);
    }

    if (processqueue_tam == 0)
    {
        processqueue[0] = berria;
        processqueue_tam++;
    }
    else if (processqueue_tam == 1)
    {
        processqueue_tam++;
        processqueue[1] = berria;
    }else if(processqueue_tam>2 && processqueue_tam<1000){

        processqueue_tam++;
        //printf("%ld \n", processqueue_tam);  

        //for (int i = 0; i < processqueue_tam-2; i++)
        //{

        //processqueue[i] = processqueue[i+1];
        // printf("%p %p",processqueue[i],processqueue[i-1]);
        //}
            processqueue[processqueue_tam-1] = berria;
            //azkenapcb->hurrengoa = &berria;
            //azkenapcb = &berria;
    }else{
        printf("procesqueuea beteta itxaron libratu arte");
    }

    

    

}

void kendu_Processqueuetik()
{
    // if (!processqueue_tam == 0)
    // {
    //     processqueue_tam--;
    // }
    for (int i = 0; i < processqueue_tam-2; i++)
        {
            processqueue[i] = processqueue[i+1];
        }
        processqueue_tam--;
}


void printqueue()
{

    printf("Process Queueko elemtu guztiak:\n");

    if (processqueue_tam > 2)
    {
        for (int i = 0; i <= processqueue_tam - 1; i++)
        {
            printf("%ld  ", processqueue[i].tid);
            // printf("%d\n",processqueue_tam);
        }
    }
    else
    {
         printf("PROZESU GUTXIEGI");
    }

    printf("\n");
}

void *Scheduler(long *freq_sc)
{

   
    while (1)
    {
      
            sem_wait(&semaforo_sc);

	    if (processqueue_tam > 0) {
            
	      printf("%ld-a duen prozesua sartuko da exekuziora\n", processqueue[processqueue_tam -1].tid);
	      fflush(stdout);
                // printf("POINTER %ld",processqueue[processqueue_tam-1]);
                kendu_Processqueuetik();
                //printqueue();
	     }

	      //sem_post(&semaforo_sc);
        
    }
}
