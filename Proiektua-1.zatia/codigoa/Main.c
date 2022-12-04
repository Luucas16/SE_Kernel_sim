#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

struct PCB
{
    long tid;
    struct PCB *hurrengoa;
};


struct PCB *processqueue;



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


long done, tik, tok, tak, tuk, tid, max, processqueue_tam = 0;
pthread_mutex_t mutex_cl /*Erlojuaren eta timerraren mutexa*/,
    mutex_pg /*Timerraren eta Prozesu sortzailearen mutexa*/,
    mutex_sc /*Timerra eta Planifikatzaileraren mutexa*/;
pthread_cond_t cond,
    cond2,
    cond3,
    cond4;
pthread_t threads[20];

int hutsik()
{
    if (rootpcb == NULL)
    
        return 1;
    else
        return 0;
}

void sartu_Pcb_eta_processqueuean(long x)
{
    //struct PCB_LIST *berria;
    //struct PROCESSQUEUE *proces;
    //berria = malloc(sizeof(struct PCB_LIST));
    //proces = malloc(sizeof(struct PROCESSQUEUE));
    struct PCB *berria;
    berria->tid = x;
    berria->hurrengoa = NULL;

    if (hutsik())
    {
        rootpcb = berria;
        azkenapcb = berria;
        processqueue[0] = *berria; 
        processqueue_tam++;
        
    }
    else
    {
        
        processqueue_tam++;
        
        for (int  i = processqueue_tam-1; i >= 0; i--)
        {
            processqueue[i] = processqueue[i-1];
        }
        processqueue[0] = *berria;
        rootpcb = berria;
        azkenapcb->hurrengoa = berria;
        azkenapcb = berria;
        
        

    }

}


void kendu_Processqueuetik()
{
    azkenapcb = &processqueue[processqueue_tam-2];
    
    processqueue_tam--;

   
}

void imprimitudena()
{
    struct PCB *reco = rootpcb;
    printf("Listako PCB_LIST elemtu guztiak:\n");
    while (reco != NULL)
    {
        printf("%i - ", reco->tid);
        reco = reco->hurrengoa;
    }
    printf("\n");
}

// void dena_borratu()
// {
//     struct PCB *reco = rootpcb;
//     struct PCB *bor;
//     while (reco != NULL)
//     {
//         bor = reco;
//         reco = reco->hurrengoa;
//         printf("c");
//         //fflush(stdout);
//         //free(bor);
//     }
// }
int Prozesuak_egin_behar_duena()
{
    
    return 1;    
}
void printqueue()
{
    
    printf("Process Queueko elemtu guztiak:\n");
    for(int i=0;i<processqueue_tam-1;i++)
    {
        printf("%d  ", processqueue[i].tid);
        //printf("\n");
        
    }
    printf("\n");
}

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

void *Tenporizadorea_pg(long *frekuentzia) // 100000
{

    pthread_mutex_lock(&mutex_cl);
    while (1)
    {
        done++;

        if (tik == *frekuentzia)
        {
            pthread_mutex_lock(&mutex_pg);
            pthread_cond_wait(&cond3, &mutex_pg);
            tik = 0;
        }
        else
        {
            tik++;
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
    

        if (tuk == *frekuentzia)
        {
            
            pthread_mutex_lock(&mutex_sc);
            pthread_cond_wait(&cond4, &mutex_sc);
            tuk = 0;
        }
        else
        {
            tuk++;
        }

        pthread_cond_signal(&cond);
        pthread_cond_wait(&cond2, &mutex_cl);
    }
}

void *Prozesuak_Sortu(long *lag)
{
    printf("Prozesuak sortu\n");
    //dena_borratu();
    pthread_mutex_lock(&mutex_pg);
    while (1)
    {   //printf("AAAAA");
        for (int i = 0; i < 100; i++)
        {
            if (tok == *lag)
            {
                pthread_create(&threads[i], NULL, (void *)Prozesuak_egin_behar_duena, NULL);
                //pthread_join(threads[i],NULL);
                //pthread_exit(&tid);
                tid = (long)threads[i];
                sartu_Pcb_eta_processqueuean(tid);
                // struct PCB_LIST *a = prozesua_bilatu_pcb_listean(tid);
                //sartu_queuean(a);
                //imprimitudena();

                tok = 0;
            }
            else
            {
                tok++;
            }
            pthread_cond_signal(&cond3);
            pthread_mutex_unlock(&mutex_pg);
        }
    }
}

void *Scheduler(long *freq_sc)
{
    
    pthread_mutex_lock(&mutex_sc);
    while (1)
    {
        if (tak == *freq_sc)
        {
                
                printf("%d-a duen prozesua sartuko da exekuziora\n", processqueue[processqueue_tam-1].tid );
                kendu_Processqueuetik();
                printqueue();
            
            tak = 0;
        }
        else
        {
            tak++;
        }

        pthread_cond_signal(&cond4);
        pthread_mutex_unlock(&mutex_sc);
    }
}

int main(int argc, char *argv[])
{
    processqueue = (struct PCB*)malloc(sizeof(struct PCB));
    
     /*Erlojuaren zenbat seinalero egin +1 Timerran*/
    long freq = strtol(argv[1], NULL, 10);
    /*Timerraren zenbat seinalero egin +1 Prozesu sortzailean*/
    long freq_pg = strtol(argv[2], NULL, 10);
    /*Timerraren zenbat seinalero egin +1 Schedulerran*/
    long freq_sc = strtol(argv[3], NULL, 10);

    /*Hariak erazagutu*/
    pthread_t h1, h2, h3, h4, h5;

    /*Kondizioak eta Mutex inizializatu*/
    pthread_mutex_init(&mutex_cl, NULL);
    pthread_mutex_init(&mutex_pg, NULL);
    pthread_mutex_init(&mutex_sc, NULL);
    pthread_cond_init(&cond, NULL);
    pthread_cond_init(&cond2, NULL);
    pthread_cond_init(&cond3, NULL);
    pthread_cond_init(&cond4, NULL);

    /*Erlojua hasi*/
    pthread_create(&h1, NULL, (void *)Erlojua, NULL);
    /*Prozesu sortzailearen Timerraren haria hasi*/
    pthread_create(&h2, NULL, (void *)Tenporizadorea_pg, &freq);
    /*Schedulerraren Timerraren haria hasi*/
    pthread_create(&h3, NULL, (void *)Tenporizadorea_sch, &freq);
    /*Prozesu sortzailearen haria hasi*/
    pthread_create(&h4, NULL, (void *)Prozesuak_Sortu, &freq_pg);
    /*Schedulerraren haria hasi*/
    pthread_create(&h5, NULL, (void *)Scheduler, &freq_sc);
    /*Hariek leno zuten harira itzuli*/
    pthread_join(h1, NULL);
    pthread_join(h2, NULL);
    pthread_join(h3, NULL);
    pthread_join(h4, NULL);
    pthread_join(h5, NULL);

    /*Mutex-ak bukarazi*/
    pthread_mutex_destroy(&mutex_cl);
    pthread_mutex_destroy(&mutex_pg);
    pthread_mutex_destroy(&mutex_sc);

    free(processqueue);
}